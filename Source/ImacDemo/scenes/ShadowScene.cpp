#include "ShadowScene.h"

#include "plan.h"
#include "heightMap.h"
#include "cubeMapObject.h"
#include "abstractCamera.h"
#include "FBO.h"
#include "glh/glh_linear.h"
//Managers
#include "shaderManager.h"
#include "shader.h"
#include "textureManager.h"
#include "texture2D.h"
#include "meshManager.h"
#include "objMesh.h"

#include "Spline.hpp"


ShadowScene::ShadowScene(const std::string& sName, AbstractCamera* pCam):AbstractScene(sName,pCam)
{

	m_pShadowMapFBO	= NULL;

}

ShadowScene::~ShadowScene()
{
	delete [] m_fLightPosition;
	delete [] m_fLightDirection;
	delete [] m_fLightDistance;
	delete [] m_fLightZeros;
	
	delete m_pShadowMapFBO;

}

bool ShadowScene::init()
{

	ShaderManager & shaderManager = ShaderManager::getInstance();
	MeshManager & meshManager = MeshManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	m_sNextSceneName = "outdoor";

	meshManager.loadMesh("cone.obj");
	meshManager.loadMesh("sceneShadow.obj");
	meshManager.loadMesh("teapot.obj");

	//Spline
	cameraSpline = new Spline("../../data/Spline/shadow_pos.xml");
	cameraAimSpline = new Spline("../../data/Spline/shadow_aim.xml");

	srand ( time(NULL) );

	iShadowMapWidth = 2*iWindowWidth;
	iShadowMapHeight = 2*iWindowHeight;

	m_pShadowMapFBO = new FBO(iShadowMapWidth,iShadowMapHeight,E_FBO_2D);
	m_pShadowMapFBO->generateDepthOnly();

	//Set the value of the light

	m_fLightPosition = new GLfloat[4];

	m_fLightDirection = new GLfloat[4];

	m_fLightDistance = new GLfloat[3];
	
	m_fLightZeros = new GLfloat[4];

	spotCutOff0 = 25.0f;
	fallOff0 = 10.0f;

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotCutOff0);

	cosfallOff0 = cos((spotCutOff0+fallOff0)*M_PI/180.0);
	
	shaderManager.getShader("light")->Activate();
	shaderManager.getShader("light")->setUniformf("cosOut",cosfallOff0);
	shaderManager.getShader("light")->Desactivate();
	

	depLightPSignX = 1;
	depLightPSignY = -1;

	depLightPAttX = 1;
	depLightPAttY = 1;

	depLightPSpeedX = 0.01 + (rand()%10)/1000.0;
	depLightPSpeedY = 0.01 + (rand()%10)/1000.0;


	//Initialize textures
	textureManager.getTexture2D("../../data/stonewall_diffuse.jpg");
	textureManager.getTexture2D("../../data/stonewall_normals.jpg");

	textureManager.getTexture2D("../../data/rocks_diffuse.jpg");

	textureManager.getTexture2D("../../data/rocks_NM_height.tga");

	textureManager.getTexture2D("../../data/face_diffuse.jpg");
	textureManager.getTexture2D("../../data/face_NM_height.png");

	textureManager.getTexture2D("../../data/stonefloor_diffuse.jpg");
	textureManager.getTexture2D("../../data/stonefloor_normal.jpg");

	textureManager.getTexture2D("../../data/wood.jpg.");

	textureManager.getTexture2D("../../data/wand_boden_diffuse.jpg.");

	textureManager.getTexture2D("../../data/web.png",false);



	return true;
}

void ShadowScene::preRender(){

	//Render the depth on a texture, form light point of view
	glViewport(0, 0, (GLint)iShadowMapWidth, (GLint)iShadowMapHeight);
		
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(spotCutOff0*2.0+fallOff0*2.0, float(iShadowMapWidth)/float(iShadowMapHeight), 0.4, 1000.0);
	glGetFloatv(GL_PROJECTION_MATRIX,projectionMatrix);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	m_pShadowMapFBO->activate();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glPushMatrix();
			
		glLoadIdentity();
			//light point of view
			glTranslatef(-m_fLightDistance[0],-m_fLightDistance[1],-m_fLightDistance[2]);
			glRotatef(-fAngle,0.0,1.0,0.0);
			glTranslatef(-m_fLightPosition[0],-m_fLightPosition[1],-m_fLightPosition[2]);
			
			
			glGetFloatv(GL_MODELVIEW_MATRIX,modelviewMatrix);
				
			glPushMatrix();
			glLoadIdentity();
			glMultMatrixf (projectionMatrix);
			glMultMatrixf (modelviewMatrix);
			glGetFloatv(GL_MODELVIEW_MATRIX, shadowmatrix);
			glPopMatrix();

			glCullFace(GL_FRONT);
				
			renderEnvironment(true);			

			glCullFace(GL_BACK);

		glPopMatrix();
	
	m_pShadowMapFBO->desactivate();

}

void ShadowScene::render(){

	ShaderManager & shaderManager = ShaderManager::getInstance();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, iWindowWidth, iWindowHeight);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, float(iWindowWidth)/float(iWindowHeight), 0.4, 1000.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();	
		m_pCam->lookAt();
		
		shaderManager.getShader("light")->Activate();
		shaderManager.getShader("light")->setUniformMatrix4fv("eyematrix",shadowmatrix);
		shaderManager.getShader("light")->Desactivate();
		
		renderEnvironment(false);

	glPopMatrix();

}

void ShadowScene::renderEnvironment(bool m_bShadow){

	MeshManager & meshManager = MeshManager::getInstance();
	ShaderManager & shaderManager = ShaderManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	glMaterialfv(GL_FRONT, GL_AMBIENT,m_fDarkGreyColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,m_fWhiteColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR,m_fWhiteColor);	
	glMaterialf( GL_FRONT, GL_SHININESS, 100.0f);

	glDisable(GL_BLEND);
	
	
	
	glPushMatrix();
		glTranslatef(m_fLightPosition[0],m_fLightPosition[1],m_fLightPosition[2]);
		glRotatef(fAngle,0.0,1.0,0.0);
		glTranslatef(m_fLightDistance[0],m_fLightDistance[1],m_fLightDistance[2]);
		glLightfv(GL_LIGHT0, GL_POSITION,m_fLightZeros);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,m_fLightDirection);
	glPopMatrix();
	
	
	
	if(!m_bShadow){

	//display the light
	glPushMatrix();

		glTranslatef(m_fLightPosition[0],m_fLightPosition[1],m_fLightPosition[2]);
		glRotatef(fAngle,0.0,1.0,0.0);
		glTranslatef(m_fLightDistance[0],m_fLightDistance[1],m_fLightDistance[2]);
		glScalef(0.02,0.02,0.02);
		glColor3f(0.0,0.0,0.0);
		meshManager.getMesh("cone.obj")->Draw(0);
		glColor3f(1.0,1.0,1.0);
		meshManager.getMesh("cone.obj")->Draw(1);
	glPopMatrix();

	//enable the light shader
	shaderManager.getShader("light")->Activate();
	shaderManager.getShader("light")->setUniformi("useShadowMap",1);
	shaderManager.getShader("light")->setUniformi("useTexture",1);

	//activate the shadowmap
	glActiveTextureARB(GL_TEXTURE7);
	m_pShadowMapFBO->activateDepthTexture();
	glActiveTextureARB(GL_TEXTURE0);
	}

	////// Temple //////

	//transformation matrix for the shadow mapping
	if(!m_bShadow){
		glPushMatrix();
			glLoadIdentity();
			glScalef(0.2,0.2,0.2);
			glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
			shaderManager.getShader("light")->setUniformMatrix4fv("transmatrix",transformationmatrix);
		glPopMatrix();
		
		glPushMatrix();
		glScalef(0.2,0.2,0.2);
			shaderManager.getShader("light")->setUniformi("useBump",1);		
			glActiveTexture(GL_TEXTURE0);
			textureManager.getTexture2D("../../data/stonefloor_diffuse.jpg")->activate();
			glActiveTexture(GL_TEXTURE1);
			textureManager.getTexture2D("../../data/stonefloor_NM_height.tga")->activate();

			meshManager.getMesh("sceneShadow.obj")->Draw(0);//floor

			

			meshManager.getMesh("sceneShadow.obj")->Draw(2);//columns
			
			meshManager.getMesh("sceneShadow.obj")->Draw(3);//pedestal
			
			glActiveTexture(GL_TEXTURE0);
			textureManager.getTexture2D("../../data/stonewall_diffuse.jpg.")->activate();
			glActiveTexture(GL_TEXTURE1);
			textureManager.getTexture2D("../../data/stonewall_normals.jpg")->activate();
			
			glMaterialf( GL_FRONT, GL_SHININESS, 20.0f);
			meshManager.getMesh("sceneShadow.obj")->Draw(4);//walls
			
			
			glActiveTexture(GL_TEXTURE0);
			textureManager.getTexture2D("../../data/rocks_diffuse.jpg.")->activate();
			glActiveTexture(GL_TEXTURE1);
			textureManager.getTexture2D("../../data/rocks_NM_height.tga")->activate();
			shaderManager.getShader("light")->setUniformi("useBump",0);	
			shaderManager.getShader("light")->setUniformi("usePOM",1);
			shaderManager.getShader("light")->setUniformf("heightFactor",0.03f);
			meshManager.getMesh("sceneShadow.obj")->Draw(1);//ceiling


			glActiveTexture(GL_TEXTURE0);
			textureManager.getTexture2D("../../data/face_diffuse.jpg.")->activate();
			glActiveTexture(GL_TEXTURE1);
			textureManager.getTexture2D("../../data/face_NM_height.png")->activate();
			shaderManager.getShader("light")->setUniformf("heightFactor",0.1f);
			meshManager.getMesh("sceneShadow.obj")->Draw(6);//cube


			glActiveTexture(GL_TEXTURE0);
			textureManager.getTexture2D("../../data/stonefloor_diffuse.jpg")->desactivate();
			glActiveTexture(GL_TEXTURE1);
			textureManager.getTexture2D("../../data/stonefloor_normal.jpg")->desactivate();
			glActiveTexture(GL_TEXTURE0);
			
			
			textureManager.getTexture2D("../../data/wood.jpg.")->activate();
			shaderManager.getShader("light")->setUniformi("usePOM",0);

			meshManager.getMesh("sceneShadow.obj")->Draw(5);//ladder
			

		glPopMatrix();

	}
	else{
		glPushMatrix();
			glScalef(0.2,0.2,0.2);
			meshManager.getMesh("sceneShadow.obj")->Draw();
		glPopMatrix();
	}



	////// END TEMPLE //////

	////// TEAPOT //////
	if(!m_bShadow){
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(0.0,0.68,-10.0);
		glScalef(0.08,0.08,0.08);
		glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
		shaderManager.getShader("light")->setUniformMatrix4fv("transmatrix",transformationmatrix);
		glPopMatrix();
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/wand_boden_diffuse.jpg.")->activate();
	}
		
	glPushMatrix();
		glTranslatef(0.0,0.68,-10.0);
		glScalef(0.08,0.08,0.08);
				
		meshManager.getMesh("teapot.obj")->Draw();//floor
			
		if(!m_bShadow){
			glActiveTexture(GL_TEXTURE0);
			textureManager.getTexture2D("../../data/wand_boden_diffuse.jpg")->desactivate();
		}
			
	glPopMatrix();

	////// END TEAPOT //////


	////// SPIDER WEB //////
	glMaterialfv(GL_FRONT, GL_SPECULAR,m_fBlackColor);	
	glDisable(GL_CULL_FACE);
	if(!m_bShadow){
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(-2.0,2.0,-8.0);
		glRotatef(90,0.0,1.0,0.0);
		glScalef(0.7,0.7,0.7);
		glGetFloatv(GL_MODELVIEW_MATRIX, transformationmatrix);
		shaderManager.getShader("light")->setUniformMatrix4fv("transmatrix",transformationmatrix);
		glPopMatrix();
		
		glEnable(GL_BLEND);
	}
	else{
		shaderManager.getShader("simpleDiscard")->Activate();
	}	
		
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/web.png",false)->activate();
		
		glPushMatrix();
		glTranslatef(-2.0,2.0,-8.0);
		glRotatef(90,0.0,1.0,0.0);
		glScalef(0.7,0.7,0.7);
				
		meshManager.getMesh("plane.obj")->Draw();//web
		
		
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/web.png")->desactivate();
		
			
	if(!m_bShadow){
		glDisable(GL_BLEND);
		shaderManager.getShader("light")->Activate();
	}
	else{
		shaderManager.getShader("simpleDiscard")->Desactivate();
	}

			
	glPopMatrix();
	glEnable(GL_CULL_FACE);
	////// END SPIDER WEB //////

	

	shaderManager.getShader("light")->Desactivate();

	if (m_bDisplaySpline)
		displaySpline();
}
		
void ShadowScene::update()
{
	AbstractScene::update();
	fAngle +=0.2;

	m_fLightPosition[0] += depLightPAttX * 0.007;
	depLightPAttX -= depLightPSignX * depLightPSpeedX;
	if(depLightPAttX < -1.0 || depLightPAttX > 1.0){
		depLightPSignX *= -1;
	}
	
	m_fLightPosition[1] += depLightPAttY * 0.007;
	depLightPAttY -= depLightPSignY * depLightPSpeedY;
	if(depLightPAttY < -1.0 || depLightPAttY > 1.0){
		depLightPSignY *= -1;
	}
}

bool ShadowScene::isFinished()
{
	if (m_pCam->getCurrentControlPoint() == 5)
	{
		///going back to outdoor
		return true;
	}
	return false;
}

void ShadowScene::handleKeyUp(unsigned char c, int x, int y)
{
	AbstractScene::handleKeyUp(c,x,y);
}

void ShadowScene::handleKeyDown(unsigned char c, int x, int y){

	switch(c){
	case 'x' :
		fAngle +=2.2;
	
	}

}

void ShadowScene::reset(){

	AbstractScene::reset();

	glLightfv(GL_LIGHT0, GL_SPECULAR, m_fWhiteColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, m_fWhiteColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, m_fWhiteColor);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.025f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotCutOff0);

	m_fLightPosition[0] = 0.0;
	m_fLightPosition[1] = 0.0;
	m_fLightPosition[2] = -10.0;
	m_fLightPosition[3] = 1.0;

	m_fLightDistance[0] = 0.0;
	m_fLightDistance[1] = 2.0;
	m_fLightDistance[2] = 10.0;

	m_fLightDirection[0] = 0.0;
	m_fLightDirection[1] = 0.0;
	m_fLightDirection[2] = -1.0;
	m_fLightDirection[3] = 1.0;


	m_fLightZeros[0] = 0.0;
	m_fLightZeros[1] = 0.0;
	m_fLightZeros[2] = 0.0;
	m_fLightZeros[3] = 1.0;

	fAngle = 0.0;

	m_pCam->setSpeed(0.05);

}