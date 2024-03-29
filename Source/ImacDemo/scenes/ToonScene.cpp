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
	glEnable(GL_CULL_FACE);
	ShaderManager & shaderManager = ShaderManager::getInstance();
	MeshManager & meshManager = MeshManager::getInstance();

	m_sNextSceneName = "outdoor";

	srand ( time(NULL) );

	meshManager.loadMesh("boule.obj");
	meshManager.loadMesh("pioupiou.obj");

	m_pDepthMapFBO = new FBO(iWindowWidth,iWindowHeight,E_FBO_2D);
	m_pDepthMapFBO->generateDepthOnly();

	return true;
}

void ToonScene::preRender()
{

	m_pDepthMapFBO->activate();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderEnvironment();
	m_pDepthMapFBO->desactivate();

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

	glPushMatrix();
		glRotatef(fRotation,0.0,0.0,1.0);
		glRotatef(fAngle,1.0,0.0,0.0);
		glScalef(2.5,2.5,2.5);
		meshManager.getMesh("boule.obj")->Draw();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0,9.3,4);		
		glRotatef(-90,1,0,0);
		glRotatef(fRotationPiou,0,1,0);
		glScalef(0.5,0.5,0.5);
		meshManager.getMesh("pioupiou.obj")->Draw();
	glPopMatrix();

}
		
void ToonScene::update()
{
	AbstractScene::update();

	if(bRotateLeft){
		if(fLeftStrenght < 1.0) fLeftStrenght += 0.1;
		
	}
	else{
		if(fLeftStrenght > 0.0) fLeftStrenght -= 0.05;
	}
	if(bRotateRight){
		if(fRightStrenght < 1.0) fRightStrenght += 0.1;
		
	}
	else{
		if(fRightStrenght > 0.0) fRightStrenght -= 0.05;
	}
	
	fRotation += 0.8 * fRightStrenght - 0.8 * fLeftStrenght;
	if(fRotation > 33.0) fRotation = 33.0;
	else if(fRotation < -33.0) fRotation = -33.0;

	fRotationPiou = 33.0 * fRightStrenght - 33.0 * fLeftStrenght;

	if(bSpeedUp){
		if(fSpeedUp < 1.0) fSpeedUp += 0.1;
	}
	else{
		if(fSpeedUp > 0.0) fSpeedUp -= 0.05;
	}

	fAngle += 0.2 + 0.5 * fSpeedUp;

}

bool ToonScene::isFinished()
{
	return false;
}

void ToonScene::handleKeyUp(unsigned char c, int x, int y)
{
	AbstractScene::handleKeyUp(c,x,y);

	switch(c){
		case 'q' :
			bRotateLeft = false;
		break;

		case 'd' :
			bRotateRight = false;
		break;
		case 'z' :
			bSpeedUp = false;
		break;
	}
 
}

void ToonScene::handleKeyDown(unsigned char c, int x, int y){

	switch(c){
		case 'q' :
			if(!bRotateLeft) bRotateLeft = true;
		break;

		case 'd' :
			if(!bRotateRight) bRotateRight = true;
		break;
		case 'z' :
			bSpeedUp = true;
		break;
	}

}

void ToonScene::reset()
{

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	AbstractScene::reset();

	fAngle = fRotation = fLeftStrenght = fRightStrenght = fSpeedUp  = fRotationPiou = 0.0;

	bRotateLeft = bRotateRight = bSpeedUp = false;

}