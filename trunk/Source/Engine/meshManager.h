/*************************************
*
*		ImacDemo Project
*		
*		Created :	20/12/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*					Helena Duchaussoy
*
**************************************/

#ifndef __MESHMANAGER_H__
#define __MESHMANAGER_H__

/**
*	@name meshManager.h
*	@brief Manager all our mesh
*	@author Franck Letellier
*	@date 20/12/09
*/

#include <string>
#include <map>

#include "singleton.h"


class ObjMesh;

class MeshManager:public Singleton<MeshManager>
{
	friend class Singleton<MeshManager>;

	public:
		void loadMesh(const std::string& sName);
		ObjMesh* getMesh(const std::string& sName);

		void clear();
	
	private:
		std::map<std::string,ObjMesh*> m_mMeshs;

		MeshManager();
		virtual ~MeshManager(){clear();};

};


#endif //__MESHMANAGER_H__