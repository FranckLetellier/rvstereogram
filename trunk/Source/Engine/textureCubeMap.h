/*************************************
*
*		ImacDemo Project
*		
*		Created :	05/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/

#ifndef __TEXTURECUBEMAP_H__
#define __TEXTURECUBEMAP_H__

#include "abstractTexture.h"
#include "Enum.h"
/**
*	@name textureCubeMap.h
*	@brief CubeMap texture class
*	@date 05/11/09
*/

class TextureCubeMap: public AbstractTexture
{

	public:

		TextureCubeMap ();
		virtual ~TextureCubeMap () {};

		/**
		*	@name loadTexture
		*	@brief load all the data
		*/
		
		virtual bool loadTexture(GLenum target,const std::string &name);
		virtual bool loadTexture(const std::string &name);
		virtual void activate();	
		virtual void desactivate();	




	protected:

		bool m_bFirstInit;
		

};

#endif //__TEXTURECUBEMAP_H__
