/*************************************
*
*		ImacDemo Project
*		
*		Created :	03/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/

#ifndef __ABSTRACTTEXTURE_H__
#define __ABSTRACTTEXTURE_H__

#include <string>
#include <iostream>

#include <GL/glut.h>
#include <GL/glu.h>

#include "OGLdefine.h"

/**
*	@name abstractTexture.h
*	@brief Abstract Texture
*	@date 03/11/09
*/

class AbstractTexture
{

	public:

		AbstractTexture ();
		virtual ~AbstractTexture ();

		/**
		*	@name loadTexture
		*	@brief load all the data
		*/
		virtual bool loadTexture(const std::string& name) = 0;
		virtual bool loadTexture(GLenum target, const std::string& name) = 0;
		virtual void activate() = 0;
		virtual void desactivate() = 0;

		virtual GLubyte* loadData();
		void generateID();
		void destroyID();
	
	protected:

		std::string	m_sName;
		unsigned int	m_iWidth;
		unsigned int	m_iHeight;
		unsigned int	m_iChanel;
		GLuint			m_iId;
		

};

#endif //__ABSTRACTTEXTURE_H__
