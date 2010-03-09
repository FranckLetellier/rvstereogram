/*************************************
*
*		ImacDemo Project
*		
*		Created :	02/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/

#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include "singleton.h"
#include "external/IL/il.h"

#include <map>
#include <string>
#include <assert.h>

/**
*	@name textureManager.h
*	@brief Manage the texture, is based on a singleton. Once the texture is loaded, we won't load it again
*	@author Franck Letellier
*	@date 02/11/09
*
*	So far, it will manage 2D texture + cubemap textures
*	Initialize DevIL too
*/

//Foward Declaration to save compilation time
class AbstractTexture;
class Texture2D;


class TextureManager: public Singleton<TextureManager>
{
	friend class Singleton<TextureManager>;


	//class 
	public:

		/**
		*	@name	Texture2D* getTexture2D(const &std::string name);
		*	@brief	get the texture2D, and load it if we need to.
		*/
		Texture2D* getTexture2D(const std::string &name, const bool useMipmap =  true);

		/**
		*	@name	void clear();
		*	@brief	Clear all the maps
		*/
		void clear();

	protected:
		std::map<std::string,AbstractTexture*>		m_mTextures;
		//std::map<std::string,int/*TextureCUBE* class*/>	m_mTexturesCubeMap;

		TextureManager();
		virtual ~TextureManager(){clear();};
};


#endif //__TEXTUREMANAGER_H__