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

#ifndef __IMAGETOOL_H__
#define __IMAGETOOL_H__

#include <string>

#include <GL/glut.h>
#include <GL/glu.h>


class ImageTool
{
	public:
		//Load an Image, and return the image data + Width, Height and chanel
		static GLubyte* loadImage(const std::string& sName, unsigned int& iWidthOut,unsigned int& iHeightOut, unsigned int& iChannelOut);
};


#endif