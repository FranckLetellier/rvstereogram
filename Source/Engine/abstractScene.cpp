#include "abstractScene.h"
#include "abstractCamera.h"

#include "FBO.h"
#include "shaderManager.h"
#include "shader.h"

#include "abstractEffect.h"
#include "effectManager.h"
#include "imageTool.h"

//static members

bool AbstractScene::m_bUseDOF = false;

unsigned int AbstractScene::iWindowWidth = 0;
unsigned int AbstractScene::iWindowHeight = 0;

FBO * AbstractScene::fboColorDepth = NULL;
FBO * AbstractScene::fboWriteAlpha = NULL;
FBO * AbstractScene::fboBlurH = NULL;
FBO * AbstractScene::fboBlurV = NULL;
FBO * AbstractScene::finalFBO = NULL;
FBO * AbstractScene::postProcessFBO0 = NULL;
FBO * AbstractScene::postProcessFBO1 = NULL;

GLfloat * AbstractScene::m_fWhiteColor = NULL;
GLfloat * AbstractScene::m_fBlackColor = NULL;
GLfloat * AbstractScene::m_fRedColor = NULL;
GLfloat * AbstractScene::m_fOrangeColor = NULL;
GLfloat * AbstractScene::m_fElectricGreenColor = NULL;
GLfloat * AbstractScene::m_fPurpleColor = NULL;
GLfloat * AbstractScene::m_fMediumGreyColor = NULL;
GLfloat * AbstractScene::m_fDarkGreyColor = NULL;
GLfloat * AbstractScene::m_fLightYellow = NULL;




AbstractScene::AbstractScene(const std::string& sName, AbstractCamera*	cam):
m_sName(sName),
m_pCam(cam)
{
	m_bStereo = true;
	
}

AbstractScene::~AbstractScene()
{

	if(m_fWhiteColor){
		delete [] m_fWhiteColor;
		m_fWhiteColor = NULL;
	}

	if(m_fBlackColor){
		delete [] m_fBlackColor;
		m_fBlackColor = NULL;
	}

	if(m_fMediumGreyColor){
		delete [] m_fMediumGreyColor;
		m_fMediumGreyColor = NULL;
	}

	if(m_fDarkGreyColor){
		delete [] m_fDarkGreyColor;
		m_fDarkGreyColor = NULL;
	}

	if(m_fRedColor){
		delete [] m_fRedColor;
		m_fRedColor = NULL;
	}

	if(m_fOrangeColor){
		delete [] m_fOrangeColor;
		m_fOrangeColor = NULL;
	}

	if(m_fElectricGreenColor){
		delete [] m_fElectricGreenColor;
		m_fElectricGreenColor = NULL;
	}

	if(m_fPurpleColor){
		delete [] m_fPurpleColor;
		m_fPurpleColor = NULL;
	}

	if(m_fLightYellow){
		delete [] m_fLightYellow;
		m_fLightYellow = NULL;
	}
}

void AbstractScene::update()
{
	///Camera Operations
	m_pCam->update();
}

void AbstractScene::reset()
{
	m_pCam->reset();
}

void AbstractScene::displayOnQuad(int iWindowWidth, int iWindowHeight){
 
	glClearColor(1, 1, 1, 0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	  
	glViewport(0,0,iWindowWidth,iWindowHeight);
	glOrtho(0,iWindowWidth,0,iWindowHeight,-0.2,0.2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	  
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex2i(0,0);
	glTexCoord2f(0.0f,1.0f);
	glVertex2i(0,iWindowHeight);
	glTexCoord2f(1.0f,1.0f);
	glVertex2i(iWindowWidth,iWindowHeight);
	glTexCoord2f(1.0f,0.0f);
	glVertex2i(iWindowWidth,0);
	glEnd();
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
}

void AbstractScene::display(){


	ShaderManager & shaderManager = ShaderManager::getInstance();

	glClearColor(1, 1, 1, 1);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pCam->lookAt();
	

	//First we calculate everything we need to create the final scene
	preRender();

		//The final scene is render into the final FBO
	finalFBO->activate();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render();			
	finalFBO->desactivate();

	if(m_bStereo){	
		ImageTool::SIRDS(finalFBO,200,m_iSIRDS);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,m_iSIRDS);
		displayOnQuad(iWindowWidth,iWindowHeight);
		glBindTexture(GL_TEXTURE_2D,0);
	}
	else{

		glActiveTexture(GL_TEXTURE0);
		finalFBO->activateTexture();
		shaderManager.getShader("filters")->Activate();
		shaderManager.getShader("filters")->setUniformi("choix",0);

		displayOnQuad(iWindowWidth,iWindowHeight);
		
		shaderManager.getShader("filters")->Desactivate();

		glActiveTexture(GL_TEXTURE0);
		finalFBO->desactivateTexture();
		
		shaderManager.getShader("filters")->Desactivate();

	}
	//finalFBO->desactivateTexture();

	/*//-----------------------------------------------Post Processing

	//activate final color and depth textures
	glActiveTexture(GL_TEXTURE0);
	finalFBO->activateTexture();
	glActiveTexture(GL_TEXTURE2);

	finalFBO->activateDepthTexture();
	
	glActiveTexture(GL_TEXTURE0);

	//get effect queue
	std::list<std::string> queue = EffectManager::getInstance().getQueue();

	std::list<std::string>::iterator itFx = queue.begin();

	int pass = 0;

	//process effects
	while(itFx != queue.end()){

		bool evenPass = pass%2 == 0;

		AbstractEffect* fx = EffectManager::getInstance().getEffect(*itFx);
		fx->preProcess(iWindowWidth,iWindowHeight);
		//we need to switch between 2 FBO's in order to use previous effect image in current effect
		if(evenPass)
			postProcessFBO0->activate();
		else
			postProcessFBO1->activate();

			fx->process(iWindowWidth,iWindowHeight);
		
			
		if(evenPass)
			postProcessFBO0->desactivate();
		else
			postProcessFBO1->desactivate();

		glActiveTexture(GL_TEXTURE0);
		if(evenPass)
			postProcessFBO0->activateTexture();
		else
			postProcessFBO1->activateTexture();

		++itFx;
		++pass;
	}

	//-----------------------------------------------Final Display
	shaderManager.getShader("filters")->Activate();
	shaderManager.getShader("filters")->setUniformi("choix",0);

	displayOnQuad(iWindowWidth,iWindowHeight);
	
	shaderManager.getShader("filters")->Desactivate();

	glActiveTexture(GL_TEXTURE2);
	finalFBO->desactivateDepthTexture();
	glActiveTexture(GL_TEXTURE0);
	finalFBO->desactivateTexture();
	
	shaderManager.getShader("filters")->Desactivate();*/
}

bool AbstractScene::globalInit(int windowWidth, int windowHeight){

	iWindowWidth = windowWidth;
	iWindowHeight = windowHeight;

	fboColorDepth = new FBO(iWindowWidth,iWindowHeight,E_FBO_2D);
	fboColorDepth->generate();

	fboWriteAlpha = new FBO(iWindowWidth,iWindowHeight,E_FBO_2D);
	fboWriteAlpha->generateColorOnly();

	fboBlurV = new FBO(iWindowWidth/2,iWindowHeight/2,E_FBO_2D);
	fboBlurV->generateColorOnly();

	fboBlurH = new FBO(iWindowWidth/2,iWindowHeight/2,E_FBO_2D);
	fboBlurH->generateColorOnly();

	finalFBO = new FBO(iWindowWidth,iWindowHeight,E_FBO_2D);
	finalFBO->generate();

	postProcessFBO0 = new FBO(iWindowWidth,iWindowHeight,E_FBO_2D);
	postProcessFBO0->generateColorOnly();
	
	postProcessFBO1 = new FBO(iWindowWidth,iWindowHeight,E_FBO_2D);
	postProcessFBO1->generateColorOnly();


	//Set the value of the Color
	m_fWhiteColor = new GLfloat[4];
	m_fWhiteColor[0] = 1.0;
	m_fWhiteColor[1] = 1.0;
	m_fWhiteColor[2] = 1.0;
	m_fWhiteColor[3] = 1.0;

	m_fBlackColor = new GLfloat[4];
	m_fBlackColor[0] = 0.0;
	m_fBlackColor[1] = 0.0;
	m_fBlackColor[2] = 0.0;
	m_fBlackColor[3] = 1.0;

	m_fRedColor = new GLfloat[4];
	m_fRedColor[0] = 1.0;
	m_fRedColor[1] = 0.0;
	m_fRedColor[2] = 0.0;
	m_fRedColor[3] = 1.0;

	m_fOrangeColor = new GLfloat[4];
	m_fOrangeColor[0] = 1.0;
	m_fOrangeColor[1] = 0.5;
	m_fOrangeColor[2] = 0.0;
	m_fOrangeColor[3] = 1.0;

	m_fElectricGreenColor = new GLfloat[4];
	m_fElectricGreenColor[0] = 0.6;
	m_fElectricGreenColor[1] = 1.0;
	m_fElectricGreenColor[2] = 0.0;
	m_fElectricGreenColor[3] = 1.0;

	m_fPurpleColor = new GLfloat[4];
	m_fPurpleColor[0] = 0.5;
	m_fPurpleColor[1] = 0.1;
	m_fPurpleColor[2] = 0.7;
	m_fPurpleColor[3] = 1.0;

	m_fDarkGreyColor = new GLfloat[4];
	m_fDarkGreyColor[0] = 0.05;
	m_fDarkGreyColor[1] = 0.05;
	m_fDarkGreyColor[2] = 0.05;
	m_fDarkGreyColor[3] = 1.0;

	m_fMediumGreyColor = new GLfloat[4];
	m_fMediumGreyColor[0] = 0.2;
	m_fMediumGreyColor[1] = 0.2;
	m_fMediumGreyColor[2] = 0.2;
	m_fMediumGreyColor[3] = 1.0;

	m_fLightYellow = new GLfloat[4];
	m_fLightYellow[0] = 1.0;
	m_fLightYellow[1] = 1.0;
	m_fLightYellow[2] = 0.8;
	m_fLightYellow[3] = 1.0;

	return true;

}



void AbstractScene::handleKeyUp(unsigned char c, int x, int y){

	switch(c){

		case 's' :
		case 'S' : 
			m_bStereo = m_bStereo ? false : true;
		break;

	}

}