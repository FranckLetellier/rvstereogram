/*************************************
*
*		ImacDemo Project
*		
*		Created :	10/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/

#ifndef __SHADERMANAGER_H__
#define __SHADERMANAGER_H__

/**
*	@name shaderManager.h
*	@brief Manage the shaderObject, is based on a singleton.
*	@author Franck Letellier
*	@date 10/11/09
*/
#include <string>
#include <map>

#include "singleton.h"
#include "shaderTool.h"

///Forward declaration
class Shader;

class ShaderManager:public Singleton<ShaderManager>
{
	friend class Singleton<ShaderManager>;

	public:
		Shader*	getShader(const std::string& name);
		bool	addShader(const std::string& name,const std::string& sVert,const std::string& sFrag);

		void clear();
		inline bool isSupported(const std::string& ext){
				return isExtensionSupported((char*)ext.c_str());};

	protected:
		std::map<std::string,Shader*> m_mShaders;

		ShaderManager();
		virtual ~ShaderManager(){clear();};

};

#endif //__SHADERMANAGER_H__