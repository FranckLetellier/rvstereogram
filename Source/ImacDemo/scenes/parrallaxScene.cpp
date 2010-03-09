#include "ParallaxScene.h"

#include "plan.h"

#include "FBO.h"
#include "abstractCamera.h"

//Managers
#include "shaderManager.h"
#include "shader.h"
#include "textureManager.h"
#include "texture2D.h"
#include "meshManager.h"
#include "objMesh.h"

#include "Spline.hpp"


ParallaxScene::ParallaxScene(const std::string& sName, AbstractCamera* pCam):AbstractScene(sName,pCam)
{

}

ParallaxScene::~ParallaxScene()
{
	delete[] m_fLightPosition;
}

bool ParallaxScene::init()
{

	MeshManager & meshManager = MeshManager::getInstance();
	ShaderManager & shaderManager = ShaderManager::getInstance();
	m_sNextSceneName = "outdoor";

	//Spline
		cameraSpline = new Spline("../../data/Spline/parallax_pos.xml");
	cameraAimSpline = new Spline("../../data/Spline/parallax_aim.xml");

	//Load meshs
	meshManager.loadMesh("parallaxRoom.obj");
	meshManager.loadMesh("cube.obj");
	meshManager.loadMesh("geosphere.obj");
	//Set the value of the light
	m_fLightPosition = new GLfloat[4];

	m_fLightPosition[0] = 7.0;
	m_fLightPosition[1] = 3.0;
	m_fLightPosition[2] = 0.0;
	m_fLightPosition[3] = 1.0;

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.9f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);

	fAngle = 0.0f;
	fAngleObject = 0.0f;

	//Texture loading
	TextureManager & textureManager = TextureManager::getInstance();
	textureManager.getTexture2D("../../data/face_diffuse.jpg");
	textureManager.getTexture2D("../../data/face_NM_height.png");
	textureManager.getTexture2D("../../data/rocks_diffuse.jpg");
	textureManager.getTexture2D("../../data/rocks_NM_height.tga");
	textureManager.getTexture2D("../../data/metalfloor2_diffuse.jpg");
	textureManager.getTexture2D("../../data/metalfloor2_NM_height.tga");
	textureManager.getTexture2D("../../data/wand_boden_diffuse.jpg");
	textureManager.getTexture2D("../../data/wand_boden_normals.jpg");
	textureManager.getTexture2D("../../data/door_temple_diffuse.jpg");
	textureManager.getTexture2D("../../data/door_temple_normal.jpg");


	return true;
}


void ParallaxScene::preRender()
{

}

void ParallaxScene::render()
{

	MeshManager & meshManager = MeshManager::getInstance();
	ShaderManager & shaderManager = ShaderManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	//Clean
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	m_pCam->lookAt();

	if (m_bDisplaySpline)
		displaySpline();

	//------------LightParameter
	glLightfv(GL_LIGHT0, GL_SPECULAR, m_fWhiteColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, m_fLightYellow);
	glMaterialfv(GL_FRONT, GL_SPECULAR,m_fWhiteColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,m_fLightYellow);
	glMaterialfv(GL_FRONT, GL_AMBIENT,m_fDarkGreyColor);
	glMaterialf( GL_FRONT, GL_SHININESS, 5.0f);

	glPushMatrix();
		glRotatef(fAngle,0.0,1.0,0.0);
		glLightfv(GL_LIGHT0, GL_POSITION,m_fLightPosition);
	glPopMatrix();

	glPushMatrix();
		glRotatef(fAngle,0.0,1.0,0.0);
		glTranslatef(m_fLightPosition[0],m_fLightPosition[1],m_fLightPosition[2]);
		glScalef(0.1,0.1,0.1);
		glColor3f(1.0,1.0,1.0);
		meshManager.getMesh("geosphere.obj")->Draw();
	glPopMatrix();

	//-----------End light parameter

	//-----the walls
	shaderManager.getShader("light")->Activate();
	shaderManager.getShader("light")->setUniformi("useTexture",1);
	shaderManager.getShader("light")->setUniformi("useShadowMap",0);
	shaderManager.getShader("light")->setUniformi("usePOM",1);
	shaderManager.getShader("light")->setUniformi("useBump",0);
	shaderManager.getShader("light")->setUniformf("heightFactor",0.12f);
	shaderManager.getShader("light")->setUniformf("fSize",1.0f);

	glEnable(GL_CULL_FACE);
	
	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/face_diffuse.jpg")->activate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/face_NM_height.png")->activate();
	glPushMatrix();
		meshManager.getMesh("parallaxRoom.obj")->Draw(1);
		meshManager.getMesh("parallaxRoom.obj")->Draw(2);
		meshManager.getMesh("parallaxRoom.obj")->Draw(3);
		meshManager.getMesh("parallaxRoom.obj")->Draw(4);
		meshManager.getMesh("parallaxRoom.obj")->Draw(10);
		meshManager.getMesh("parallaxRoom.obj")->Draw(11);
	glPopMatrix();

	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/rocks_diffuse.jpg")->activate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/rocks_NM_height.tga")->activate();

	//Roof
	glPushMatrix();
		shaderManager.getShader("light")->setUniformf("fSize",3.0f);
		shaderManager.getShader("light")->setUniformf("heightFactor",0.09f);
		meshManager.getMesh("parallaxRoom.obj")->Draw(5);
	glPopMatrix();

	//Objects
	glActiveTexture(GL_TEXTURE0);

	textureManager.getTexture2D("../../data/metalfloor2_diffuse.jpg")->activate();
	glActiveTexture(GL_TEXTURE1);

	textureManager.getTexture2D("../../data/metalfloor2_NM_height.tga")->activate();
	shaderManager.getShader("light")->setUniformf("fSize",1.0f);
	shaderManager.getShader("light")->setUniformf("heightFactor",0.120);
	glMaterialf( GL_FRONT, GL_SHININESS, 100.0f);

	glPushMatrix();
		glTranslatef(-2.348,3.7,-1.52);
		glRotatef(fAngleObject,1.0,1.0,0.0);
		meshManager.getMesh("cube.obj")->Draw();
	glPopMatrix();

	shaderManager.getShader("light")->setUniformf("fSize",3.0f);
	shaderManager.getShader("light")->setUniformf("heightFactor",0.15f);
	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/skin_diffuse.jpg")->activate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/skin_NM_height.png")->activate();
	glPushMatrix();
		glTranslatef(4.149,3.8,0.0);
		glRotatef(-fAngleObject,0.0,1.0,0.0);
		glScalef(0.8,0.8,0.8);
		
		meshManager.getMesh("geosphere.obj")->Draw();
	glPopMatrix();

	glMaterialf( GL_FRONT, GL_SHININESS, 10.0f);
	shaderManager.getShader("light")->setUniformi("usePOM",0);
	shaderManager.getShader("light")->setUniformi("useBump",1);
	shaderManager.getShader("light")->setUniformf("heightFactor",0.04f);
	
	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/wand_boden_diffuse.jpg")->activate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/wand_boden_normals.jpg")->activate();

	glPushMatrix();
		
		//floor
		shaderManager.getShader("light")->setUniformf("fSize",10.0f);
		meshManager.getMesh("parallaxRoom.obj")->Draw(0);
		shaderManager.getShader("light")->setUniformf("fSize",1.0f);
		meshManager.getMesh("parallaxRoom.obj")->Draw(6);
		meshManager.getMesh("parallaxRoom.obj")->Draw(7);

		//Door
		meshManager.getMesh("parallaxRoom.obj")->Draw(8);

		glActiveTexture(GL_TEXTURE0);
		textureManager.getTexture2D("../../data/door_temple_diffuse.jpg")->activate();
		glActiveTexture(GL_TEXTURE1);
		textureManager.getTexture2D("../../data/door_temple_normal.jpg")->activate();
		meshManager.getMesh("parallaxRoom.obj")->Draw(9);

	
	glPopMatrix();

	glActiveTexture(GL_TEXTURE0);
	textureManager.getTexture2D("../../data/rocks_diffuse.jpg")->desactivate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTexture2D("../../data/rocks_NM_height.tga")->desactivate();
	glActiveTexture(GL_TEXTURE0);
	
	shaderManager.getShader("light")->setUniformi("useBump",0);
	shaderManager.getShader("light")->Desactivate();


	//glDisable(GL_CULL_FACE);
}
		
void ParallaxScene::update()
{
	AbstractScene::update();

	fAngle += 0.6;
	fAngleObject += 0.3;
}

bool ParallaxScene::isFinished()
{
	if (m_pCam->getCurrentControlPoint() == 9)
	{
		///going back to outdoor
		return true;
	}
	return false;
}

void ParallaxScene::handleKeyUp(unsigned char c, int x, int y)
{
	AbstractScene::handleKeyUp(c,x,y);
}

void ParallaxScene::reset()
{

	AbstractScene::reset();

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.4f);

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);

	fAngle = 0.0f;
	fAngleObject = 0.0f;

	m_pCam->setSpeed(0.069);

	
}