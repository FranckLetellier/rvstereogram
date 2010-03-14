#include "main.h"

#include "meshManager.h"
#include "objMesh.h"
#include "FBO.h"

#include "textureManager.h"
#include "texture2D.h"

#include "abstractCamera.h"
#include "cameraFPS.h"


#include "shaderManager.h"
#include "shader.h"

#include "sceneManager.h"

#include "ToonScene.h"

#include "effectManager.h"

#include "glh/glh_linear.h"

#include <cmath>
#define CHECK_ERRORS

static void reshapeGL(int newwidth, int newheight);

static void displayGL(void)
{
	//Reshape to normal
	reshapeGL(windowwidth, windowheight);

	SceneManager::getInstance().display();

	glutSwapBuffers();
	++frames;
}

static void reshapeGL(int newwidth, int newheight)
{
	float ratio = (float)newwidth/(float)newheight;
	glViewport(0, 0, (GLint)newwidth, (GLint)newheight);
  
	windowwidth = newwidth;
	windowheight = newheight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(ouverture, ratio, near, far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

static void idleGL(void)
{

	ShaderManager & shaderManager = ShaderManager::getInstance();

	glutWarpPointer(windowwidth/2,windowheight/2);


	///Debug Mode : Check for OpenGL error
	#ifdef CHECK_ERRORS
		err = glGetError();
		if(err!=GL_NO_ERROR){
			std::cerr << "Erreur GL :" << std::endl;
			std::cerr << gluErrorString(err) << std::endl;
		}
	#endif

	///TODO: class timer
	currentTime= glutGet(GLUT_ELAPSED_TIME);
	///Show FPS
	if((currentTime- timebaseFPS) >= 1000)
	{
		std::cout<<"fps : "<< frames*1000.0f/(currentTime- timebaseFPS)<<std::endl;
		frames = 0;
		timebaseFPS = currentTime;
	}
	///Block to 60 FPS
	if((currentTime- timebase) >= 15)
	{

		glutPostRedisplay();
		
		timebase = currentTime;
		SceneManager::getInstance().update();
		///If the scene is over, we move on to the next scene
		if (SceneManager::getInstance().isFinished())
		{
			SceneManager::getInstance().setCurrentScene(SceneManager::getInstance().getNextScene());
			SceneManager::getInstance().setCamera(cam);
			SceneManager::getInstance().reset();
		}
	}


}

static void quit(void)
{

 	delete cam;
	ShaderManager::killInstance();
	TextureManager::killInstance();
	SceneManager::killInstance();
	MeshManager::killInstance();

	exit(0);
}

static void keyboardGL(unsigned char c, int x, int y){ 

	SceneManager::getInstance().handleKeyDown(c,x,y);

switch(c){ 
	case 27:
			quit();
		break;
	case 'w':
		int wtype[2];
		glGetIntegerv(GL_POLYGON_MODE,wtype);
		if(wtype[0]==GL_FILL)
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		break;

	/*case 'z' :
		cam->setFlag(E_CAMERA_FORWARD,true);
	break;
	
	case 's' : 
		cam->setFlag(E_CAMERA_BACKWARD,true);
	break;

	case 'q' :
		cam->setFlag(E_CAMERA_LEFT,true);
	break;

	case 'd' :
		cam->setFlag(E_CAMERA_RIGHT,true);
	break;*/
	
	//default:
    //break;
  }

}

static void keyboardUpGL(unsigned char c, int x, int y)
{

	SceneManager::getInstance().handleKeyUp(c,x,y);

switch(c){ 
		/*case 'z' :
		cam->setFlag(E_CAMERA_FORWARD,false);
	    break;
	
		case 's' : 
		cam->setFlag(E_CAMERA_BACKWARD,false);
		break;

		case 'q' :
		cam->setFlag(E_CAMERA_LEFT,false);
		break;

		case 'd' :
		cam->setFlag(E_CAMERA_RIGHT,false);
		break;*/

		/*case 'g' :
			if(!EffectManager::getInstance().isQueued("greyScale"))
				EffectManager::getInstance().queueEffect("greyScale");
			else
				EffectManager::getInstance().removeFromQueue("greyScale");
		break;

		case 'b' :
			if(!EffectManager::getInstance().isQueued("bloom"))
				EffectManager::getInstance().queueEffect("bloom");
			else
				EffectManager::getInstance().removeFromQueue("bloom");
		break;

		case 'm' :
			if(!EffectManager::getInstance().isQueued("motionBlur"))
				EffectManager::getInstance().queueEffect("motionBlur");
			else
				EffectManager::getInstance().removeFromQueue("motionBlur");
		break;

		case 'n' :
			EffectManager::getInstance().clearQueue();
		break;*/

		
	
	//default:
    //break;
  }


}

static void keyboardSpecialGL(int c, int x, int y)
{ 

}

static void mouseGL(int button, int state, int x, int y)
{

}

static void motionGL(int x, int y)
{
	//cam->updateMouse(x,y);	
}

static void passiveMotionGL(int x, int y)
{
	//cam->updateMouse(x,y);
}





static void initGL(int argc, char **argv)
{

	///Initialisation of Glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error : " << glewGetErrorString(err) << std::endl;
		return ;
	}

	glClearColor(1.0f,1.0f,1.0f,1.0f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

  
	if(!isExtensionSupported("GL_ARB_shading_language_100"))
		quit();

	///timer initialisation
	timebase = glutGet(GLUT_ELAPSED_TIME);
	timebaseFPS = glutGet(GLUT_ELAPSED_TIME);

	///Create the camera
	cam = new CameraFPS(windowwidth,windowheight);
	m_bCamFPS = true;
	glutSetCursor(GLUT_CURSOR_NONE);

	//generate FBO's
	AbstractScene::globalInit(windowwidth,windowheight);
	
}

static void initScene()
{
	AbstractScene* pScene = new ToonScene("toon",cam);
	if (!pScene->init())
		quit();
	SceneManager::getInstance().addScene(pScene);
	

	//Set the first Stage

	SceneManager::getInstance().setCurrentScene("toon");
	SceneManager::getInstance().reset();

}
static void initMesh()
{

}

static void initShader()
{

	ShaderManager & shaderManager = ShaderManager::getInstance();

	//Shader

	if (!(shaderManager.addShader("filters", "../Shaders/filters.vert","../Shaders/filters.frag")))
		quit();

	    shaderManager.getShader("filters")->Activate();
        shaderManager.getShader("filters")->setUniformTexture("texture",0);
        shaderManager.getShader("filters")->setUniformTexture("texture2",1);
        shaderManager.getShader("filters")->setUniformTexture("depthTexture",2);
        shaderManager.getShader("filters")->setUniformi("choix",0);
        shaderManager.getShader("filters")->setUniformf("thresholdValue",0.9);
        shaderManager.getShader("filters")->Desactivate();

}

static void initEffect()
{

}

int main(int argc, char **argv)
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowPosition(windowx, windowy);
  glutInitWindowSize(windowwidth, windowheight);

  if(glutCreateWindow("ImacDemo") == 0){ 
    return 1;
  }

  glutReshapeFunc(reshapeGL);
  glutDisplayFunc(displayGL);
  glutIdleFunc(idleGL);

  glutKeyboardFunc(keyboardGL);
  glutKeyboardUpFunc(keyboardUpGL);

  glutSpecialFunc(keyboardSpecialGL);
  
  glutMouseFunc(mouseGL);
  glutMotionFunc(motionGL);
  glutPassiveMotionFunc(passiveMotionGL);

  //Initialize the parameter
  initGL(argc, argv);

  //Obj
  initMesh();
  //Shader
  initShader();
  //Initialize the Scene
  initScene();

  //Initialize effects
  initEffect();

  glutMainLoop();

  return 0;
}

