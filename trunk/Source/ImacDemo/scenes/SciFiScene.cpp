#include "SciFiScene.h"

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


SciFiScene::SciFiScene(const std::string& sName, AbstractCamera* pCam):AbstractScene(sName,pCam)
{


}

SciFiScene::~SciFiScene()
{
	delete [] m_fLightPosition;
	delete [] m_fLightDirection;
	delete [] m_fLightDistance;
	delete [] m_fLightZeros;
	
}

bool SciFiScene::init()
{

	ShaderManager & shaderManager = ShaderManager::getInstance();
	MeshManager & meshManager = MeshManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	m_sNextSceneName = "outdoor";

	meshManager.loadMesh("sceneSciFi.obj");
	meshManager.loadMesh("ball.obj");
	meshManager.loadMesh("geosphere.obj");

	//Spline
	cameraSpline = new Spline("../../data/Spline/scifi_pos.xml");
	cameraAimSpline = new Spline("../../data/Spline/scifi_aim.xml");

	srand ( time(NULL) );

	if (!(shaderManager.addShader("vertexDisplacement", "../Shaders/vertexDisplacement.vert","../Shaders/vertexDisplacement.frag")))
		return false;
	shaderManager.getShader("vertexDisplacement")->Activate();
	shaderManager.getShader("vertexDisplacement")->setUniformf("pas",0.025);
	shaderManager.getShader("vertexDisplacement")->setUniformf("displacementValue",10);
	shaderManager.getShader("vertexDisplacement")->setUniformTexture("texture",0);
	shaderManager.getShader("vertexDisplacement")->setUniformTexture("texture2",1);
	shaderManager.getShader("vertexDisplacement")->setUniformTexture("diffuse",2);
	shaderManager.getShader("vertexDisplacement")->Desactivate();

	//Set the value of the light

	m_fLightPosition = new GLfloat[4];

	m_fLightDirection = new GLfloat[4];

	m_fLightDistance = new GLfloat[3];
	
	m_fLightZeros = new GLfloat[4];

	depLightPSignX = 1;
	depLightPSignY = -1;

	depLightPAttX = 1;
	depLightPAttY = 1;

	depLightPSpeedX = 0.01 + (rand()%10)/1000.0;
	depLightPSpeedY = 0.01 + (rand()%10)/1000.0;

	//Initialize texture

	textureManager.getTexture2D("../../data/metalfloor_diffuse.jpg");
	textureManager.getTexture2D("../../data/metalfloor_normals.jpg");

	textureManager.getTexture2D("../../data/metal_diffuse.jpg");
	textureManager.getTexture2D("../../data/metal_normals.jpg");

	textureManager.getTexture2D("../../data/spacebox_diffuse.jpg");
	textureManager.getTexture2D("../../data/spacebox_normals.jpg");

	textureManager.getTexture2D("../../data/pannel_diffuse.jpg");
	textureManager.getTexture2D("../../data/pannel_normals.jpg");

	textureManager.getTexture2D("../../data/spacecorridor_diffuse.jpg");
	textureManager.getTexture2D("../../data/spacecorridor_normals.jpg");

	textureManager.getTexture2D("../../data/metalfloor2_diffuse.jpg");
	textureManager.getTexture2D("../../data/metalfloor2_NM_height.tga");

	textureManager.getTexture2D("../../data/hazard_diffuse.jpg");

	textureManager.getTexture2D("../../data/walltechno_diffuse.jpg");

	textureManager.getTexture2D("../../data/walltechno_glow.jpg");

	textureManager.getTexture2D("../../data/magma_diffuse.jpg");
	textureManager.getTexture2D("../../data/magma_glow.jpg");

	textureManager.getTexture2D("../../data/glow_diffuse.jpg");

	textureManager.getTexture2D("../../data/height1.png");
	textureManager.getTexture2D("../../data/height2.png");


	return true;
}

void SciFiScene::preRender(){


	ShaderManager & shaderManager = ShaderManager::getInstance();

	//-> scene normal
	AbstractScene::postProcessFBO0->activate();
		renderEnvironment(false);
	AbstractScene::postProcessFBO0->desactivate();
	//-> 

	// scene avec texture glow
	AbstractScene::postProcessFBO1->activate();
		renderEnvironment(true);
	AbstractScene::postProcessFBO1->desactivate();

	//blur V

	shaderManager.getShader("blur")->Activate();
	shaderManager.getShader("blur")->setUniformi("iWindowWidth",fboBlurV->getWidth());
	shaderManager.getShader("blur")->setUniformi("iWindowHeight",fboBlurV->getHeight());
	shaderManager.getShader("blur")->setUniformi("choix",0);
	shaderManager.getShader("blur")->setUniformi("blurValue",35);

	AbstractScene::fboBlurV->activate();
	glActiveTexture(GL_TEXTURE1);
	AbstractScene::postProcessFBO1->activateTexture();
		AbstractScene::displayOnQuad(fboBlurV->getWidth(),fboBlurV->getHeight());
	AbstractScene::postProcessFBO1->desactivateTexture();
	AbstractScene::fboBlurV->desactivate();

	//blurH
	shaderManager.getShader("blur")->setUniformi("choix",1);

	AbstractScene::fboBlurH->activate();
	AbstractScene::fboBlurV->activateTexture();
		AbstractScene::displayOnQuad(fboBlurH->getWidth(),fboBlurH->getHeight());
	AbstractScene::fboBlurV->desactivateTexture();
	AbstractScene::fboBlurH->desactivate();
	shaderManager.getShader("blur")->Desactivate();

	glActiveTexture(GL_TEXTURE0);

}

void SciFiScene::render(){

	ShaderManager & shaderManager = ShaderManager::getInstance();
	
	shaderManager.getShader("filters")->Activate();
	shaderManager.getShader("filters")->setUniformi("choix",4);

	glActiveTexture(GL_TEXTURE0);
	AbstractScene::postProcessFBO0->activateTexture();
	glActiveTexture(GL_TEXTURE1);
	AbstractScene::fboBlurH->activateTexture();

		AbstractScene::displayOnQuad(iWindowWidth,iWindowHeight);

	glActiveTexture(GL_TEXTURE0);
	AbstractScene::postProcessFBO0->desactivateTexture();
	glActiveTexture(GL_TEXTURE1);
	AbstractScene::fboBlurH->desactivateTexture();
	shaderManager.getShader("filters")->Desactivate();
}

void SciFiScene::renderEnvironment(bool m_bGlow){

	MeshManager & meshManager = MeshManager::getInstance();
	ShaderManager & shaderManager = ShaderManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMaterialfv(GL_FRONT, GL_AMBIENT,m_fDarkGreyColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,m_fWhiteColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR,m_fWhiteColor);	
	glMaterialf( GL_FRONT, GL_SHININESS, 120.0f);

	glDisable(GL_BLEND);
	
	if (m_bDisplaySpline)
		displaySpline();
	
	glPushMatrix();
		glTranslatef(m_fLightPosition[0],m_fLightPosition[1],m_fLightPosition[2]);
		glRotatef(fAngle,0.0,1.0,0.0);
		glTranslatef(m_fLightDistance[0],m_fLightDistance[1],m_fLightDistance[2]);
		glLightfv(GL_LIGHT0, GL_POSITION,m_fLightZeros);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,m_fLightDirection);
	glPopMatrix();
	
	

	//display the light
	glPushMatrix();

		glTranslatef(m_fLightPosition[0],m_fLightPosition[1],m_fLightPosition[2]);
		glRotatef(fAngle,0.0,1.0,0.0);
		glTranslatef(m_fLightDistance[0],m_fLightDistance[1],m_fLightDistance[2]);
		glScalef(0.1,0.1,0.1);
		glColor3f(1.0,1.0,1.0);
		meshManager.getMesh("geosphere.obj")->Draw();

	glPopMatrix();

	if(!m_bGlow){
		//enable the light shader
		shaderManager.getShader("light")->Activate();
		shaderManager.getShader("light")->setUniformi("useShadowMap",0);
		shaderManager.getShader("light")->setUniformi("useTexture",1);
	}
	else{
		glColor3f(0.0,0.0,0.0);
	}
	
	////// Scene //////

	
	glPushMatrix();
		glScalef(0.2,0.2,0.2);
	if(!m_bGlow){
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/metalfloor_diffuse.jpg")->activate();
		glActiveTexture(GL_TEXTURE1);
		textureManager.getTexture2D("../../data/metalfloor_normals.jpg")->activate();
		shaderManager.getShader("light")->setUniformi("useBump",1);
	}
		meshManager.getMesh("sceneSciFi.obj")->Draw(0);//floor
	if(!m_bGlow){
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/metal_diffuse.jpg")->activate();
		glActiveTexture(GL_TEXTURE1);
		textureManager.getTexture2D("../../data/metal_normals.jpg")->activate();
	}
		meshManager.getMesh("sceneSciFi.obj")->Draw(5);//"hole"
	if(!m_bGlow){
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/spacebox_diffuse.jpg")->activate();
		glActiveTexture(GL_TEXTURE1);
		textureManager.getTexture2D("../../data/spacebox_normals.jpg")->activate();
	}
		meshManager.getMesh("sceneSciFi.obj")->Draw(6);//boxes
	if(!m_bGlow){
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/pannel_diffuse.jpg")->activate();
		glActiveTexture(GL_TEXTURE1);
		textureManager.getTexture2D("../../data/pannel_normals.jpg")->activate();
	}
		meshManager.getMesh("sceneSciFi.obj")->Draw(7);//"door"

	if(!m_bGlow){
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/spacecorridor_diffuse.jpg")->activate();
		glActiveTexture(GL_TEXTURE1);
		textureManager.getTexture2D("../../data/spacecorridor_normals.jpg")->activate();
	}
		meshManager.getMesh("sceneSciFi.obj")->Draw(3);//columns
	if(!m_bGlow){
		shaderManager.getShader("light")->setUniformi("usePOM",1);
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/metalfloor2_diffuse.jpg")->activate();
		glActiveTexture(GL_TEXTURE1);
		textureManager.getTexture2D("../../data/metalfloor2_NM_height.tga")->activate();
	}
		meshManager.getMesh("sceneSciFi.obj")->Draw(2);//ceiling

	if(!m_bGlow){
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/hazard_diffuse.jpg")->activate();
		shaderManager.getShader("light")->setUniformi("usePOM",0);
		shaderManager.getShader("light")->setUniformi("useBump",0);	

	}
		glDisable(GL_CULL_FACE);
		meshManager.getMesh("sceneSciFi.obj")->Draw(8);//box
		glEnable(GL_CULL_FACE);

	if(!m_bGlow){
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/walltechno_diffuse.jpg")->activate();

	}
	else{
		glEnable(GL_TEXTURE_2D);
		textureManager.getTexture2D("../../data/walltechno_glow.jpg")->activate();
		glColor3f(1.0,1.0,1.0);
	}


	meshManager.getMesh("sceneSciFi.obj")->Draw(1);//walls	

	textureManager.getTexture2D("../../data/walltechno_glow.jpg")->desactivate();

	

	glPopMatrix();


	shaderManager.getShader("light")->Desactivate();

	////// Magma //////

	glPushMatrix();
	glScalef(0.2,0.2,0.2);
		glActiveTexture(GL_TEXTURE0);
		if(!m_bGlow)
			textureManager.getTexture2D("../../data/magma_diffuse.jpg")->activate();
		else
			textureManager.getTexture2D("../../data/magma_glow.jpg")->activate();

		glEnable(GL_TEXTURE_2D);
		meshManager.getMesh("sceneSciFi.obj")->Draw(4);//magma
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	////// Balls //////

	glPushMatrix();
	glScalef(0.2,0.2,0.2);

	if(!m_bGlow){
		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/glow_diffuse.jpg")->activate();
		glEnable(GL_TEXTURE_2D);
	}
	else{
		glColor3f(0.18,0.49,0.57);
	}

	meshManager.getMesh("sceneSciFi.obj")->Draw(10);//balls	
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/metalfloor2_diffuse.jpg")->desactivate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/metalfloor2_NM_height.tga")->desactivate();
	glActiveTexture(GL_TEXTURE0);

	
	shaderManager.getShader("vertexDisplacement")->Activate();
	shaderManager.getShader("vertexDisplacement")->setUniformf("coeff",m_fMixValue);
	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/height1.png")->activate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/height2.png")->activate();
	glActiveTexture(GL_TEXTURE2);
	textureManager.getTexture2D("../../data/glow_diffuse.jpg")->activate();
	glActiveTexture(GL_TEXTURE0);

	glMaterialf( GL_FRONT, GL_SHININESS, 40.0f);

	glPushMatrix();
		glScalef(0.2,0.2,0.2);
		meshManager.getMesh("sceneSciFi.obj")->Draw(9);//thing

	glPopMatrix();

	shaderManager.getShader("vertexDisplacement")->Desactivate();

	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/height1.png")->desactivate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/height2.png")->desactivate();
	glActiveTexture(GL_TEXTURE2);
	textureManager.getTexture2D("../../data/glow_diffuse.jpg")->desactivate();
	glActiveTexture(GL_TEXTURE0);

}
		
void SciFiScene::update()
{

	AbstractScene::update();

	fAngle +=0.3;

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

	   m_fMixValue += 0.005 * m_iMixSign;
   if(m_fMixValue < 0.0 || m_fMixValue > 1.0) m_iMixSign *= -1;
}

bool SciFiScene::isFinished()
{
	if (m_pCam->getCurrentControlPoint() == 9)
	{
		///going back to outdoor
		return true;
	}
	return false;
}

void SciFiScene::handleKeyUp(unsigned char c, int x, int y)
{
	AbstractScene::handleKeyUp(c,x,y);
}

void SciFiScene::handleKeyDown(unsigned char c, int x, int y)
{
	switch(c){
	case 'x' :
		fAngle +=2.2;
	break;
	}

}

void SciFiScene::reset(){

	AbstractScene::reset();

	glLightfv(GL_LIGHT0, GL_SPECULAR, m_fWhiteColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, m_fWhiteColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, m_fWhiteColor);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1f);

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);

	m_fLightPosition[0] = 0.0;
	m_fLightPosition[1] = 0.0;
	m_fLightPosition[2] = -10.0;
	m_fLightPosition[3] = 1.0;

	m_fLightDistance[0] = 0.0;
	m_fLightDistance[1] = 2.0;
	m_fLightDistance[2] = 6.0;

	m_fLightDirection[0] = 0.0;
	m_fLightDirection[1] = 0.0;
	m_fLightDirection[2] = -1.0;
	m_fLightDirection[3] = 1.0;


	m_fLightZeros[0] = 0.0;
	m_fLightZeros[1] = 0.0;
	m_fLightZeros[2] = 0.0;
	m_fLightZeros[3] = 1.0;

	fAngle = 0.0;

	m_iMixSign = 1;
	m_fMixValue = 0.0;

	m_pCam->setSpeed(0.06);

}