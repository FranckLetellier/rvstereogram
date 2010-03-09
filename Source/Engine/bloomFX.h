/*************************************
*
*		ImacDemo Project
*		
*		Created :	18/12/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*					Helena Duchaussoy
*
**************************************/

#ifndef __BLOOMFX_H__
#define __BLOOMFX_H__

/**
*	@name bloomFX.h
*	@brief Scene outside, with water and heightmap
*	@author Franck Letellier
*	@date 18/12/09
*/

#include "abstractEffect.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

//Forward declaration
class FBO;

class BloomFX: public AbstractEffect
{
	public:
		BloomFX(const std::string& sName);
		virtual ~BloomFX();

		virtual void preProcess(unsigned int windowWidth, unsigned int windowHeight);
		virtual void process(unsigned int windowWidth, unsigned int windowHeight);
};


#endif //__BLOOMFX_H__

