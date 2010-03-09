#include "main.h"

#include "../Engine/textureManager.h"
#include "../Engine/texture2D.h"
#include "../Engine/textureCubeMap.h"

#include "../Engine/cubeMapObject.h"

#include "../Engine/camera/abstractCamera.h"
#include "../Engine/camera/cameraFPS.h"

#include "../Engine/shaderManager.h"
#include "../Engine/shader.h"

#include <cmath>

#define CHECK_ERRORS




static void displayGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam->getPos()[0],cam->getPos()[1],cam->getPos()[2],cam->getAim()[0],cam->getAim()[1],cam->getAim()[2],0.0,1.0,0.0);

	//--TEST
	
	TextureManager::getInstance().getTexture2D("../../data/image.jpg")->activate();
	ShaderManager::getInstance().getShader("test")->Activate();
		glutSolidTeapot(1.0);
	ShaderManager::getInstance().getShader("test")->Desactivate();

	TextureManager::getInstance().getTexture2D("../../data/image.jpg")->desactivate();
	cubeMap->display(); 
	//--FINTEST

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
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

static void idleGL(void)
{
	///Camera Operations
	cam->update();
	glutWarpPointer(windowwidth/2,windowheight/2);

	///CubeMap Operation
	cubeMap->update(cam);

	///Debug Mode : Check for OpenGL error
	#ifdef CHECK_ERRORS
		err = glGetError();
		if(err!=GL_NO_ERROR){
			std::cerr << "Erreur GL :" << std::endl;
			std::cerr << gluErrorString(err) << std::endl;
		}
	#endif

	///TODO: class timer
	time = glutGet(GLUT_ELAPSED_TIME);
	if((time - timebase) >= 1000)
	{
		std::cout<<"fps : "<< frames*1000.0f/(time - timebase)<<std::endl;
		frames = 0;
		timebase = time;
	}
	
	glutPostRedisplay();
  

}

static void quit(void)
{
	delete cubeMap;
	delete cam;

	exit(0);
}

static void keyboardGL(unsigned char c, int x, int y){ 
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

	case 'z' :
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
		break;
  default:
    break;
  }

glutPostRedisplay();
}

static void keyboardUpGL(unsigned char c, int x, int y)
{
switch(c){ 
	case 'z' :
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
		break;
  default:
    break;
  }

glutPostRedisplay();
}

static void keyboardSpecialGL(int c, int x, int y)
{ 
  glutPostRedisplay();
}

static void mouseGL(int button, int state, int x, int y)
{
  glutPostRedisplay();
}

static void motionGL(int x, int y)
{
	cam->updateMouse(x,y);	
}

static void passiveMotionGL(int x, int y)
{
	cam->updateMouse(x,y);
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

	glClearColor(0.0f,0.0f,0.0f,0.0f);

	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

  
	if(!isExtensionSupported("GL_ARB_shading_language_100"))
		quit();

	///timer initialisation
	timebase = glutGet(GLUT_ELAPSED_TIME);

	///Create the camera
	cam = new CameraFPS(windowwidth,windowheight);
	glutSetCursor(GLUT_CURSOR_NONE);

	//--TEST
	//CubeMap
	cubeMap = new CubeMapObject("cube");
	cubeMap->setTexture(E_CUBEMAP_NEG_X,"../../data/interstellar_rt.tga");
	cubeMap->setTexture(E_CUBEMAP_NEG_Y,"../../data/interstellar_dn.tga"); 
	cubeMap->setTexture(E_CUBEMAP_NEG_Z,"../../data/interstellar_bk.tga"); 
	cubeMap->setTexture(E_CUBEMAP_POS_X,"../../data/interstellar_lf.tga");
	cubeMap->setTexture(E_CUBEMAP_POS_Y,"../../data/interstellar_up.tga"); 
	cubeMap->setTexture(E_CUBEMAP_POS_Z,"../../data/interstellar_ft.tga");

	//Shader
	if (!(ShaderManager::getInstance().addShader("test", "../Shaders/color.vert","../Shaders/color.frag")))
		quit();
	//--FINTEST


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

  initGL(argc, argv);

  glutMainLoop();

  return 0;
}

