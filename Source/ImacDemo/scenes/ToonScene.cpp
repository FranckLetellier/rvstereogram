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


	meshManager.loadMesh("teapot.obj");
	meshManager.loadMesh("boule.obj");

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

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

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


	/*glPushMatrix();
		glTranslatef(0,0,0);
		glRotatef(55.0,1.0,1.0,0.0);
		glScalef(0.08,0.08,0.08);
		glDisable(GL_CULL_FACE);
		meshManager.getMesh("teapot.obj")->Draw();
		glEnable(GL_CULL_FACE);
	glPopMatrix();*/

	glPushMatrix();
		glTranslatef(0,-5,0);
		glRotatef(fAngle,1.0,0.0,0.0);
		glScalef(0.08,0.08,0.08);
		glDisable(GL_CULL_FACE);
		meshManager.getMesh("boule.obj")->Draw();
		glEnable(GL_CULL_FACE);
	glPopMatrix();

}
		
void ToonScene::update()
{
	AbstractScene::update();

	fAngle += 0.3;

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