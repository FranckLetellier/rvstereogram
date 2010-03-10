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
	m_pDepthMapFBO	= NULL;
}

ToonScene::~ToonScene()
{
	delete 	m_pDepthMapFBO;
}

bool ToonScene::init()
{

	glDisable(GL_BLEND);

	ShaderManager & shaderManager = ShaderManager::getInstance();
	MeshManager & meshManager = MeshManager::getInstance();

	m_sNextSceneName = "outdoor";

	srand ( time(NULL) );

	meshManager.loadMesh("torus.obj");
	meshManager.loadMesh("teapot.obj");
	meshManager.loadMesh("torusKnot.obj");
	meshManager.loadMesh("pouet.obj");
	meshManager.loadMesh("ball.obj");
	meshManager.loadMesh("geosphere.obj");

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

	m_pDepthMapFBO = new FBO(iWindowWidth,iWindowHeight,E_FBO_2D);
	m_pDepthMapFBO->generateDepthOnly();

	return true;
}

void ToonScene::preRender()
{

	m_pDepthMapFBO->activate();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		renderEnvironment();

	m_pDepthMapFBO->desactivate();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

}

void ToonScene::render(){

	TextureManager & textureManager = TextureManager::getInstance();

	ShaderManager & shaderManager = ShaderManager::getInstance();
	
	shaderManager.getShader("filters")->Activate();
	shaderManager.getShader("filters")->setUniformi("choix",2);

	glActiveTexture(GL_TEXTURE0);
	m_pDepthMapFBO->activateDepthTexture();

		AbstractScene::displayOnQuad(iWindowWidth,iWindowHeight);

	glActiveTexture(GL_TEXTURE0);
	m_pDepthMapFBO->desactivateTexture();
	shaderManager.getShader("filters")->Desactivate();


}

void ToonScene::renderEnvironment()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MeshManager & meshManager = MeshManager::getInstance();
	ShaderManager & shaderManager = ShaderManager::getInstance();

	glColor3f(0.0,0.0,0.0);

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

}
		
void ToonScene::update()
{
	AbstractScene::update();

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

}