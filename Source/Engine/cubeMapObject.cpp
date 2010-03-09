#include "cubeMapObject.h"
#include "textureManager.h"
#include "textureCubeMap.h"
#include "camera/abstractCamera.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

#include "shaderManager.h"
#include "shader.h"

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

CubeMapObject::CubeMapObject(const std::string& sName,const std::string& sNameNight):
m_sName(sName),
m_sNameNight(sNameNight)
{
	m_iSize = 200;
	m_iDiscretisationX = 25;
	m_iDiscretisationY = 25;
	init();
}

CubeMapObject::CubeMapObject(unsigned int iSize,unsigned int iDisX,unsigned int iDisY,const std::string &sName):
m_sName(sName),
m_iSize(iSize),
m_iDiscretisationX(iDisX),
m_iDiscretisationY(iDisY)
{
	init();
}

void CubeMapObject::init()
{
	GLfloat xPlane[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	GLfloat yPlane[] = { 0.0f, 1.0f, 0.0f, 0.0f };
	GLfloat zPlane[] = { 0.0f, 0.0f, 1.0f, 0.0f };

	glTexGenfv (GL_S, GL_OBJECT_PLANE, xPlane);
	glTexGenfv (GL_T, GL_OBJECT_PLANE, yPlane);
	glTexGenfv (GL_R, GL_OBJECT_PLANE, zPlane);

	m_vPos.push_back(0.0);
	m_vPos.push_back(0.0);
	m_vPos.push_back(0.0);
}


CubeMapObject::~CubeMapObject()
{

}

void CubeMapObject::desactivate()
{
	
	TextureManager & textureManager = TextureManager::getInstance();	
	
	glActiveTexture(GL_TEXTURE0);
	textureManager.getTextureCubeMap(m_sName)->desactivate();
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTextureCubeMap(m_sNameNight)->desactivate();
	glActiveTexture(GL_TEXTURE0);
}

void CubeMapObject::display(const float mixValue)
{

	ShaderManager & shaderManager = ShaderManager::getInstance();
	TextureManager & textureManager = TextureManager::getInstance();

	shaderManager.getShader("dayAndNight")->Activate();
	shaderManager.getShader("dayAndNight")->setUniformf("mixValue",mixValue);

	glActiveTexture(GL_TEXTURE0);
	textureManager.getTextureCubeMap(m_sName)->activate();

	
	glActiveTexture(GL_TEXTURE1);
	textureManager.getTextureCubeMap(m_sNameNight)->activate();

	glDisable(GL_CULL_FACE);
	glPushMatrix();
		glTranslatef(m_vPos[0],m_vPos[1]+10,m_vPos[2]);
		glutSolidSphere (m_iSize, m_iDiscretisationX, m_iDiscretisationY);
	glPopMatrix();
	glEnable(GL_CULL_FACE);

	shaderManager.getShader("dayAndNight")->Desactivate();

	//textureManager.getTextureCubeMap(m_sName)->desactivate();
}

void CubeMapObject::update(AbstractCamera* cam, bool bUpsideDown,float fYplane)
{
	///So that the CubeMap follows us
	//const & std::vector<double> tmp =  cam->getPos();
	if (bUpsideDown)
		m_vPos[1] = 3.0*fYplane-cam->getPos().y();
	else
		m_vPos[1] = cam->getPos().y();

	m_vPos[0] = cam->getPos().x();
	m_vPos[2] = cam->getPos().z();
}

bool CubeMapObject::setTexture(CubeMapEnum eSide, const std::string& sName, const unsigned int iTexId)
{

	TextureManager & textureManager = TextureManager::getInstance();

	std::string sTmpName;

	if(iTexId == 0)
		sTmpName = m_sName;
	else 
		sTmpName = m_sNameNight;
	
	//Check if we already have loaded this side
	std::map< CubeMapEnum, std::string>::iterator iter = m_mTextureName.find(eSide);
	if (iter != m_mTextureName.end())
	{
		///We are checking if we already have loaded this texture
		if ((*iter).second != sName)
		{
			m_mTextureName[eSide] = sName;
			switch (eSide)
			{
			case E_CUBEMAP_NEG_X:
				textureManager.getTextureCubeMap(sTmpName)->loadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,sName);
				break;
			case E_CUBEMAP_NEG_Y:
				textureManager.getTextureCubeMap(sTmpName)->loadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,sName);
				break;
			case E_CUBEMAP_NEG_Z:
				textureManager.getTextureCubeMap(sTmpName)->loadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,sName);
				break;
			case E_CUBEMAP_POS_X:
				textureManager.getTextureCubeMap(sTmpName)->loadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X,sName);
				break;
			case E_CUBEMAP_POS_Y:
				textureManager.getTextureCubeMap(sTmpName)->loadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,sName);
				break;
			case E_CUBEMAP_POS_Z:
				textureManager.getTextureCubeMap(sTmpName)->loadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,sName);
				break;
			}
		}
	}
	else
	{
		m_mTextureName[eSide] = sName;
		switch (eSide)
			{
			case E_CUBEMAP_NEG_X:
				textureManager.getTextureCubeMap(sTmpName)->loadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,sName);
				break;
			case E_CUBEMAP_NEG_Y:
				textureManager.getTextureCubeMap(sTmpName)->loadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,sName);
				break;
			case E_CUBEMAP_NEG_Z:
				textureManager.getTextureCubeMap(sTmpName)->loadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,sName);
				break;
			case E_CUBEMAP_POS_X:
				textureManager.getTextureCubeMap(sTmpName)->loadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X,sName);
				break;
			case E_CUBEMAP_POS_Y:
				textureManager.getTextureCubeMap(sTmpName)->loadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,sName);
				break;
			case E_CUBEMAP_POS_Z:
				textureManager.getTextureCubeMap(sTmpName)->loadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,sName);
				break;
			}
	}
	return true;
}