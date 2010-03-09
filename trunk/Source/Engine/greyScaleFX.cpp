#include "greyScaleFX.h"

#include "FBO.h"

//Managers
#include "AbstractScene.h"
#include "shaderManager.h"
#include "shader.h"
#include "textureManager.h"
#include "texture2D.h"


GreyScaleFX::GreyScaleFX(const std::string& sName):AbstractEffect(sName)
{

}

GreyScaleFX::~GreyScaleFX()
{
}

void GreyScaleFX::preProcess(unsigned int windowWidth, unsigned int windowHeight){

}

void GreyScaleFX::process(unsigned int windowWidth, unsigned int windowHeight){

	ShaderManager & shaderManager = ShaderManager::getInstance();

	shaderManager.getShader("filters")->Activate();
	shaderManager.getShader("filters")->setUniformi("choix",1);
	AbstractScene::displayOnQuad(windowWidth,windowHeight);
}