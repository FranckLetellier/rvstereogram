/*************************************
*
*		ImacDemo Project
*		
*		Created :	24/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/

#ifndef __PLAN_H__
#define __PLAN_H__

#include <string>
#include <vector>
#include "Vec3f"

class HeightMapStripe;

class Plan
{

	public:
		Plan();
		Plan( unsigned int iDiscretization,unsigned int iWidth, unsigned int iHeight);

		virtual ~Plan();

		float getHeightAtPosition(const osg::Vec3f& vPosition);
		void draw();

		inline unsigned int getWidth(){return m_iWidth;}
		inline unsigned int getHeight(){return m_iHeight;}
		inline unsigned int getDiscretization(){return m_iDiscretization;}

	protected:
		void init(unsigned int iDiscretization,unsigned int iWidth, unsigned int iHeight);

		std::vector<HeightMapStripe*>	m_pStripes;
		//Mesh attributes
		unsigned int m_iHeight;
		unsigned int m_iWidth;
		unsigned int m_iDiscretization;
};


#endif //__PLAN_H__