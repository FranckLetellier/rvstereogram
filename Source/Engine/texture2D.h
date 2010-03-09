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

#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

#include "abstractTexture.h"

/**
*	@name texture2D.h
*	@brief regular texture class
*	@date 03/11/09
*/

class Texture2D: public AbstractTexture
{

	public:

		Texture2D ();
		Texture2D (bool bMipMaps);
		virtual ~Texture2D () {};

		/**
		*	@name loadTexture
		*	@brief load all the data
		*/
		virtual bool loadTexture(const std::string &name);
		virtual bool loadTexture(GLenum target, const std::string& name){return loadTexture(name);};
		virtual void activate();	
		virtual void desactivate();	

	protected:
		bool m_bMipMaps;

};

#endif //__TEXTURE2D_H__
