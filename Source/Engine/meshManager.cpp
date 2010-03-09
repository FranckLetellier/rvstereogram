#include "meshManager.h"
#include "objMesh.h"


MeshManager::MeshManager()
{

}

void MeshManager::clear()
{
	for (std::map<std::string , ObjMesh*>::iterator iter = m_mMeshs.begin() ; iter != m_mMeshs.end() ; ++iter)
	{
		delete (*iter).second;
	}
	m_mMeshs.clear();
}


ObjMesh* MeshManager::getMesh(const std::string &sName)
{
	std::map<std::string , ObjMesh*>::iterator iter = m_mMeshs.find(sName);
	if (iter == m_mMeshs.end())
	{	
		std::cerr<<"ERROR : couldn't find "<<sName<<std::endl;
		return NULL;
	}
	else
		return (*iter).second;

}

void MeshManager::loadMesh(const std::string &sName)
{
	std::map<std::string , ObjMesh*>::iterator iter = m_mMeshs.find(sName);
	if (iter == m_mMeshs.end()) // We load a new mesh
	{	
		ObjMesh* pMesh = new ObjMesh();
		pMesh->Load("../../Data/obj/"+sName);
		std::cout<<"[LOADING] : "<<sName<<std::endl;
		m_mMeshs[sName] = pMesh;
	}
	else
		std::cerr<<"WARNING : Obj already loaded"<<sName<<std::endl;
}