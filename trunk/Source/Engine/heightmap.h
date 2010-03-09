/*************************************
*
*		ImacDemo Project
*		
*		Created :	18/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/

#ifndef __HEIGHTMAP_H__
#define __HEIGHTMAP_H__

#include <string>
#include <vector>
#include "plan.h"
#include "Vec3f"

class VBO;
class HeightMapStripe;

class HeightMap: public Plan
{

	public:
		HeightMap(const std::string& sName);
		HeightMap(const std::string& sName, unsigned int iDiscretization,unsigned int iWidth, unsigned int iHeight, unsigned int iZmax);

		~HeightMap();

		float getHeightAtPosition(const osg::Vec3f& vPosition);




	private:
		void init(const std::string& sName, unsigned int iDiscretization,unsigned int iWidth, unsigned int iHeight, unsigned int iZmax);

		///Texture attributes
		unsigned int m_iHeightTex;
		unsigned int m_iWidthTex;
		unsigned int *m_pData;
		unsigned int m_iZmax;
};


#endif //__HEIGHTMAP_H__