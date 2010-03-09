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

#ifndef __GREYSCALEFX_H__
#define __GREYSCALEFX_H__

/**
*	@name greyScaleFX.h
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

class GreyScaleFX: public AbstractEffect
{
	public:
		GreyScaleFX(const std::string& sName);
		virtual ~GreyScaleFX();

		virtual void preProcess(unsigned int windowWidth, unsigned int windowHeight);
		virtual void process(unsigned int windowWidth, unsigned int windowHeight);
};


#endif //__GREYSCALEFX_H__

