#include "motionBlurFX.h"

#include "FBO.h"

//Managers
#include "AbstractScene.h"
#include "shaderManager.h"
#include "shader.h"
#include "textureManager.h"
#include "texture2D.h"


MotionBlurFX::MotionBlurFX(const std::string& sName):AbstractEffect(sName)
{

}

MotionBlurFX::~MotionBlurFX()
{
}

void MotionBlurFX::preProcess(unsigned int windowWidth, unsigned int windowHeight){



}

void MotionBlurFX::process(unsigned int windowWidth, unsigned int windowHeight){

	ShaderManager & shaderManager = ShaderManager::getInstance();

	shaderManager.getShader("filters")->Activate();
	shaderManager.getShader("filters")->setUniformi("choix",5);
	AbstractScene::displayOnQuad(windowWidth, windowHeight);


}