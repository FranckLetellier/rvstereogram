#include "ToonScene.h"

#include "FBO.h"
#include "abstractCamera.h"

//Managers
#include "shaderManager.h"
#include "shader.h"
#include "textureManager.h"
#include "texture2D.h"
#include "meshManager.h"
#include "objMesh.h"



ToonScene::ToonScene(const std::string& sName, AbstractCamera* pCam):AbstractScene(sName,pCam)
{

}

ToonScene::~ToonScene()
{
	delete m_fLightPosition;
}

bool ToonScene::init()
{

	ShaderManager & shaderManager = ShaderManager::getInstance();
	MeshManager & meshManager = MeshManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	m_sNextSceneName = "outdoor";

	srand ( time(NULL) );

	meshManager.loadMesh("testIndoor2.obj");
	meshManager.loadMesh("torus.obj");
	meshManager.loadMesh("teapot.obj");
	meshManager.loadMesh("torusKnot.obj");
	meshManager.loadMesh("pouet.obj");
	meshManager.loadMesh("ball.obj");
	meshManager.loadMesh("geosphere.obj");


	if (!(shaderManager.addShader("Toon", "../Shaders/toonPoint.vert","../Shaders/toonPoint.frag")))
		return false;

	//enable black outline, disable texture
	shaderManager.getShader("Toon")->Activate();
	shaderManager.getShader("Toon")->setUniformi("useOutline",1);
	shaderManager.getShader("Toon")->setUniformi("useTexture",0);
	shaderManager.getShader("Toon")->Desactivate();

	//Set the value of the light

	m_fLightPosition = new GLfloat[4];
	m_fLightPosition[0] = 0.0;
	m_fLightPosition[1] = 0.0;
	m_fLightPosition[2] = 0.0;
	m_fLightPosition[3] = 1.0;


	fAngle = 0.0;

	depBallX = 0.0;
	depBallY = 0.0;
	depBallZ = 0.0;
	
	depBallSignX = 1;
	depBallSignY = -1;
	depBallSignZ = 1;

	depBallAttX = 1;
	depBallAttY = 1;
	depBallAttZ = 1;


	depBallSpeedX = 0.01 + (rand()%10)/1000.0;
	depBallSpeedY = 0.01 + (rand()%10)/1000.0;
	depBallSpeedZ = 0.01 + (rand()%10)/1000.0;


	textureManager.getTexture2D("../../data/toonwall.jpg");

	textureManager.getTexture2D("../../data/toonceiling.jpg");

	textureManager.getTexture2D("../../data/toonfloor.jpg");

	textureManager.getTexture2D("../../data/toonfloor2.jpg");


	return true;
}

void ToonScene::preRender()
{
}

void ToonScene::render()
{


	MeshManager & meshManager = MeshManager::getInstance();
	ShaderManager & shaderManager = ShaderManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);

	glMaterialfv(GL_FRONT, GL_SPECULAR,m_fBlackColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,m_fWhiteColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT,m_fBlackColor);
	glMaterialf( GL_FRONT, GL_SHININESS, 125.0f);

	glDisable(GL_BLEND);

	glPushMatrix();
		glRotatef(fAngle,0.0,1.0,0.0);
		glTranslatef(6.5,4.0,0.0);
		glLightfv(GL_LIGHT0, GL_POSITION,m_fLightPosition);
		glScalef(0.1,0.1,0.1);
		glColor3f(1.0,1.0,1.0);
		meshManager.getMesh("geosphere.obj")->Draw();
	glPopMatrix();

	shaderManager.getShader("Toon")->Activate();
	shaderManager.getShader("Toon")->setUniformi("useOutline",0);
	shaderManager.getShader("Toon")->setUniformi("useTexture",1);
	
	glPushMatrix();
		glScalef(0.5,0.5,0.5);
		textureManager.getTexture2D("../../data/toonwall.jpg")->activate();
		meshManager.getMesh("testIndoor2.obj")->Draw(0);
		textureManager.getTexture2D("../../data/toonceiling.jpg")->activate();
		meshManager.getMesh("testIndoor2.obj")->Draw(1);
		textureManager.getTexture2D("../../data/toonfloor.jpg")->activate();
		meshManager.getMesh("testIndoor2.obj")->Draw(2);
		textureManager.getTexture2D("../../data/toonfloor2.jpg")->activate();
		meshManager.getMesh("testIndoor2.obj")->Draw(3);
		textureManager.getTexture2D("../../data/toonfloor2.jpg")->desactivate();
	glPopMatrix();

	
	//-----------------Toon shaded stuffs	
	shaderManager.getShader("Toon")->setUniformi("useOutline",1);
	shaderManager.getShader("Toon")->setUniformi("useTexture",0);


	//flying ball
	glMaterialfv(GL_FRONT, GL_DIFFUSE,m_fElectricGreenColor);
	glPushMatrix();
		glTranslatef(7.0+depBallX,3.0+depBallY,-7.0+depBallZ);
		glScalef(0.3,0.3,0.3);
		meshManager.getMesh("ball.obj")->Draw();
	glPopMatrix();

	//torus + torus knot
	glPushMatrix();
		glTranslatef(-7.0,4,7.0);
		glRotatef(-fAngle/2.0,1.0,0.0,0.0);	
		glRotatef(fAngle*1.5,0.0,0.0,1.0);
		glScalef(0.18,0.18,0.18);
		meshManager.getMesh("torusKnot.obj")->Draw();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE,m_fRedColor);
	glPushMatrix();
		glTranslatef(-7.0,4,7.0);
		glRotatef(fAngle/3.0,1.0,0.0,0.0);	
		glRotatef(fAngle,0.0,0.0,1.0);
		glScalef(0.4,0.4,0.4);
		meshManager.getMesh("torus.obj")->Draw();
	glPopMatrix();

	//strange thing lauching teapots
	glMaterialfv(GL_FRONT, GL_DIFFUSE,m_fPurpleColor);
	glPushMatrix();
		glTranslatef(0,-0.5,0.0);
		glRotatef(90.0,0.0,1.0,0.0);
		glScalef(0.05,0.05,0.05);
		meshManager.getMesh("pouet.obj")->Draw();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE,m_fOrangeColor);
	glPushMatrix();
		glTranslatef(0.0,1.2,0.5);
		glRotatef(-33.0,1.0,1.0,0.0);
		glScalef(0.1,0.1,0.1);
		glDisable(GL_CULL_FACE);
		meshManager.getMesh("teapot.obj")->Draw();
		glEnable(GL_CULL_FACE);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE,m_fOrangeColor);
	glPushMatrix();
		glTranslatef(0,2.2,1.9);
		glRotatef(55.0,1.0,1.0,0.0);
		glScalef(0.08,0.08,0.08);
		glDisable(GL_CULL_FACE);
		meshManager.getMesh("teapot.obj")->Draw();
		glEnable(GL_CULL_FACE);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE,m_fOrangeColor);
	glPushMatrix();
		glTranslatef(0.0,1.1,3.5);
		glRotatef(90.0,1.0,1.0,0.0);
		glScalef(0.05,0.05,0.05);
		glDisable(GL_CULL_FACE);
		meshManager.getMesh("teapot.obj")->Draw();
		glEnable(GL_CULL_FACE);
	glPopMatrix();

	shaderManager.getShader("Toon")->Desactivate();
	//-----------------End Toon shaded stuffs

}
		
void ToonScene::update()
{
	AbstractScene::update();

	fAngle += 0.5;

	depBallX += depBallAttX * 0.01;
	depBallAttX -= depBallSignX * depBallSpeedX;
	if(depBallAttX < -1.0 || depBallAttX > 1.0){
		depBallSignX *= -1;
	}
	
	depBallY += depBallAttY * 0.01;
	depBallAttY -= depBallSignY * depBallSpeedY;
	if(depBallAttY < -1.0 || depBallAttY > 1.0){
		depBallSignY *= -1;
	}

	depBallZ += depBallAttZ * 0.01;
	depBallAttZ -= depBallSignZ * depBallSpeedZ;
	if(depBallAttZ < -1.0 || depBallAttZ > 1.0){
		depBallSignZ *= -1;
	}
}

bool ToonScene::isFinished()
{
	if (m_pCam->getCurrentControlPoint() == 5)
	{
		///going back to outdoor
		return true;
	}
	return false;
}

void ToonScene::handleKeyUp(unsigned char c, int x, int y)
{
	AbstractScene::handleKeyUp(c,x,y);
}

void ToonScene::reset()
{

	AbstractScene::reset();
	m_fLightPosition[0] = 0.0;
	m_fLightPosition[1] = 0.0;
	m_fLightPosition[2] = 0.0;
	m_fLightPosition[3] = 1.0;

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);

	glLightfv(GL_LIGHT0, GL_SPECULAR, m_fWhiteColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, m_fWhiteColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, m_fWhiteColor);


	fAngle = 0.0;

	depBallX = 0.0;
	depBallY = 0.0;
	depBallZ = 0.0;
	
	depBallSignX = 1;
	depBallSignY = -1;
	depBallSignZ = 1;

	depBallAttX = 1;
	depBallAttY = 1;
	depBallAttZ = 1;


	depBallSpeedX = 0.01 + (rand()%10)/1000.0;
	depBallSpeedY = 0.01 + (rand()%10)/1000.0;
	depBallSpeedZ = 0.01 + (rand()%10)/1000.0;

	m_pCam->setSpeed(0.08);

}