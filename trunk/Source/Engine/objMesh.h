/*************************************
*
*		ImacDemo Project
*		
*		Created :	25/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/

#ifndef __OBJMESH_H__
#define __OBJMESH_H__


#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "Vec3"
#include "Vec2"
/**
*	@name objMesh.h
*	@brief Load a Mesh form a .Obj file
*	
*	@date 25/11/09
*/

class ObjMesh
{
	public:
		bool Load(const std::string& name);
		void Destroy();

		void Draw();
		void Draw(GLuint group);

		ObjMesh();
		~ObjMesh() {Destroy();}
private:
	bool LoadFile(const std::string& filename);

	void ComputeNormals();
	void ComputeTangents();
	void ComputeBoundingBox();
	void bind();
	void unbind();
	bool build();

	unsigned int		m_iNbVertex;
	GLuint				m_iBufferId;

	//DATA for the VBO
	std::vector<osg::Vec3>	m_vPosition;
	std::vector<osg::Vec3>	m_vNormal;
	std::vector<osg::Vec2>	m_vTexcoord;
	std::vector<osg::Vec3>	m_vTangent;

	GLsizeiptr m_iSizeVertex;
	GLsizeiptr m_iSizeNormal;
	GLsizeiptr m_iSizeTexture;
	GLsizeiptr m_iSizeTangent;

private:
	struct sFace {
		GLuint ind[3];
	};

	struct sGroup {
		std::string			strName;
		long				nMaterial;
		std::vector<sFace>	tFace;
	};

private:
	std::vector<sGroup>		m_tGroup;

};

#endif //__OBJMESH_H__