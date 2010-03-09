/*************************************
*
*		ImacDemo Project
*		
*		Created :	09/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/


#ifndef __CUBEMAPOBJECT_H__
#define __CUBEMAPOBJECT_H__

/**
*	@name cubeMapObject.h
*	@brief Handle a cubemap
*	@author Franck Letellier
*	@date 09/11/09
*/
#include <GL/glew.h>
#include <map>
#include <string>
#include <vector>

#include "Enum.h"

class AbstractCamera;

class CubeMapObject
{
	public:
		CubeMapObject(const std::string& sName,const std::string& sNameNight);

		CubeMapObject(unsigned int iSize,unsigned int iDisX,unsigned int iDisY,const std::string& sName);
		
		~CubeMapObject();

		void display(const float mixValue);
		void desactivate();


		///To be called when we move the camera
		void update(AbstractCamera*	cam,bool bUpsideDown,float fYplane);

		bool setTexture( CubeMapEnum eSide, const std::string& sName, const unsigned int iTexId);
		
	private:

		void init();

		std::map< CubeMapEnum, std::string> m_mTextureName;
		std::string m_sName;
		std::string m_sNameNight;

		///Cubemap Position
		std::vector<double> m_vPos;

		unsigned int m_iSize;
		unsigned int m_iDiscretisationX;
		unsigned int m_iDiscretisationY;

		void drawSphere(const float& radius, const unsigned int& stepx, const unsigned int& stepy);

};



#endif //__CUBEMAPOBJECT_H__