#include "objMesh.h"
#include <iostream>
#include <algorithm>
using namespace std;
#include <string>
#include <sstream>
#include <fstream>
#include <assert.h>

#define NEXT_INDICE do{i++;}while((buf[i]<'0')&&(buf[i]>'9'));
#define BUFFER_OFFSET(i) ((void*)(i))

ObjMesh::ObjMesh() 
{
	m_iBufferId = 0;
}

bool ObjMesh::Load(const std::string& name)
{

	bool ret = false;
	if(name.find(".obj") != std::string::npos) {
		ret = LoadFile(name);
	}
	else {
		std::cerr << "Le mesh " << name << " n'est pas dans un format valide" << std::endl;
		return false;
	}

	if(!ret) {
		std::cerr << "[Error] Impossible de charger le mesh " << name << std::endl;
		return false;
	}

	ComputeTangents();
	
	build();

	return  true;

}

bool ObjMesh::build()
{
	glGenBuffers(1,  &m_iBufferId);
	assert(m_iBufferId);

	bind();
	//Build the VBO

	if(!m_vPosition.empty()) 
		m_iSizeVertex = m_vPosition.size()*sizeof(osg::Vec3);
	else {
		std::cerr << "[ERROR] No position data !" << std::endl;
		return false;
	}

	if(!m_vNormal.empty()) 
		m_iSizeNormal	= m_vNormal.size()*sizeof(osg::Vec3);

	if(!m_vTexcoord.empty()) 
		m_iSizeTexture = m_vTexcoord.size()*sizeof(osg::Vec2);
	

	if(!m_vTangent.empty())
		m_iSizeTangent = m_vTangent.size()*sizeof(osg::Vec3);
	
	//Link the buffer
	glBufferData(GL_ARRAY_BUFFER, m_iSizeVertex + m_iSizeNormal + m_iSizeTexture + m_iSizeTangent, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER_ARB, 0,	m_iSizeVertex,	(const GLvoid*)(&m_vPosition[0]));

	if(m_vNormal.size())
		glBufferSubData(GL_ARRAY_BUFFER_ARB, m_iSizeVertex,	m_iSizeNormal,	(const GLvoid*)(&m_vNormal[0]));

	if(m_vTexcoord.size())
		glBufferSubData(GL_ARRAY_BUFFER_ARB, m_iSizeVertex + m_iSizeNormal,	m_iSizeTexture,	(const GLvoid*)(&m_vTexcoord[0]));

	if(m_vTangent.size())
		glBufferSubData(GL_ARRAY_BUFFER_ARB, m_iSizeVertex + m_iSizeNormal + m_iSizeTexture,	m_iSizeTangent,	(const GLvoid*)(&m_vTangent[0]));






	unbind();

	return true;

}

void ObjMesh::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_iBufferId);
}

void ObjMesh::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ObjMesh::Destroy()
{

	glDeleteBuffersARB(1, &m_iBufferId);
	m_iBufferId = 0;

	m_vPosition.clear();
	m_vNormal.clear();
	m_vTexcoord.clear();
	m_vTangent.clear();

	for(std::vector<sGroup>::iterator it=m_tGroup.begin(); it!=m_tGroup.end(); it++)
		(*it).tFace.clear();
	m_tGroup.clear();

}

bool ObjMesh::LoadFile(const std::string& filename)	
{
	ifstream fp(filename.c_str(), ios::binary);
	if(!fp) return false;

	sFace face;
	sGroup grp;
	osg::Vec3 pt3D;
	osg::Vec2 pt2D;
//	char buftmp[64];
	long i,v,g;	// compteur pour le stockage des données lors de la seconde passe

	GLuint	indFacePosition;
	GLuint	indFaceNormal;
	GLuint	indFaceTexcoord;

	std::vector<osg::Vec3>	tTempPosition;
	std::vector<osg::Vec3>	tTempNormal;
	std::vector<osg::Vec2>	tTempTexcoord;

	string strBuffer;
	while(std::getline(fp, strBuffer)) {
		stringstream strStream(strBuffer);
		const char* buf = strBuffer.c_str();

	//	lenbuf = strlen((const char *)buf);
		if (strBuffer.size() > 0) {
	//		sscanf(buf,"%s",buftmp);
			string buftmp;
			strStream >> buftmp;
		//	buftmp << strStream;
			if(buftmp == "#") {	// on a trouvé un commentaire, on passe
			} else if(buftmp == "mtllib") {	// on a trouvé un matérial à charger ?
	//			materials->Load(&buf[7]);	// on charge le fichier associé
			} else if(buftmp == "v") {	// on a trouvé une vertice ?
				sscanf(&strBuffer[2],"%f%f%f",&pt3D.x(),&pt3D.y(),&pt3D.z());
				tTempPosition.push_back(pt3D);
			} else if(buftmp == "vt") {	// on a trouvé une coordonnée de texture ?
				sscanf(&strBuffer[2],"%f%f",&pt2D.x(),&pt2D.y());
				pt2D.y() = 1.0f - pt2D.y();
				tTempTexcoord.push_back(pt2D);
			} else if(buftmp == "vn") {	// on a trouvé une normale ?
				sscanf(&strBuffer[2],"%f%f%f",&pt3D.x(),&pt3D.y(),&pt3D.z());
				pt3D.normalize();
				tTempNormal.push_back(pt3D);
			} else if(buftmp == "g") {	// on a trouvé un groupe ?
				if(strBuffer.size() > 1) {
					grp.strName = &buf[2];
					grp.nMaterial = 0;
					m_tGroup.push_back(grp);
				}
			} else if(buftmp == "usemtl") {	// on a trouvé un matérial à utiliser ?
				if(m_tGroup.size() <= 0) {
					grp.strName = "No Name";
					grp.nMaterial = 0;
					m_tGroup.push_back(grp);
					g = 0;
				} else {
					g = (long)m_tGroup.size() - 1;
				}
//				m_tGroup[g].Material = materials->GetId(&buf[7]);	// on récupère son id
			} else if(buftmp == "f") {	// on a trouvé une face ?

				
				int max;
				if( tTempPosition.size() >= tTempNormal.size() )	max = 0;
				else												max = 1;

				if(max==0 && tTempTexcoord.size() > tTempPosition.size() )	max = 2;
				if(max==1 && tTempTexcoord.size() > tTempNormal.size() )	max = 2;

		//		m_tNormal.resize( m_tPosition.size() );
		//		m_tTexcoord.resize( m_tPosition.size() );

				switch(max) {
				case 0: {
					m_vPosition.resize( tTempPosition.size() );
					m_vNormal.resize( tTempPosition.size() );
					m_vTexcoord.resize( tTempPosition.size() );
					break;}
				case 1: {
					m_vPosition.resize( tTempNormal.size() );
					m_vNormal.resize( tTempNormal.size() );
					m_vTexcoord.resize( tTempNormal.size() );
					break;}
				case 2: {
					m_vPosition.resize( tTempTexcoord.size() );
					m_vNormal.resize( tTempTexcoord.size() );
					m_vTexcoord.resize( tTempTexcoord.size() );
					break;}
				}

				if(m_tGroup.size() <= 0) {
					grp.strName = "No Name";
					grp.nMaterial = 0;
					m_tGroup.push_back(grp);
					g = 0;
				} else {
					g = (long)m_tGroup.size() - 1;
				}
				for(i=0; (buf[i] < '0') || (buf[i] > '9') ;i++);	// on se positionne à la première valeur
				for(v=0; v < 3 ;v++) { // triangles donc composés de 3 vertices
					indFacePosition = 0;
					for(; (buf[i] >= '0') && (buf[i] <= '9') ;i++) {	// on la récupère
						indFacePosition *= 10;	// première vertice
						indFacePosition += buf[i]-0x30; // 0x30 est la valeur ascii du caractère '0'
					}
					indFacePosition--;	// indice n'est pas de 1 à nbFaces mais de 0 à nbFaces-1
					NEXT_INDICE;	// on se positionne à la valeur suivante
					indFaceTexcoord = 0;
					for(; (buf[i] >= '0') && (buf[i] <= '9') ;i++) {	// on la récupère
						indFaceTexcoord *= 10;	// première coordonnée de texture
						indFaceTexcoord += buf[i]-0x30;
					}
					indFaceTexcoord--;	// indice n'est pas de 1 à nbFaces mais de 0 à nbFaces-1
					NEXT_INDICE; // ect ... il y a 9 indices à récupérer
					indFaceNormal = 0;
					for(; (buf[i] >= '0') && (buf[i] <= '9') ;i++) {
						indFaceNormal *= 10;	// première normale
						indFaceNormal += buf[i]-0x30;
					}
					indFaceNormal--;	// indice n'est pas de 1 à nbFaces mais de 0 à nbFaces-1
					if(v < 2) NEXT_INDICE;


					int idx = 0;
					switch(max) {
					case 0: {idx = indFacePosition;	break;}
					case 1: {idx = indFaceNormal;	break;}
					case 2: {idx = indFaceTexcoord;	break;}
					}

					m_vPosition[idx] = tTempPosition[indFacePosition];
					m_vNormal[idx] = tTempNormal[indFaceNormal];
					m_vTexcoord[idx] = tTempTexcoord[indFaceTexcoord];
					face.ind[v] = idx;

/*
					m_tPosition.push_back( tTempPosition[ indFacePosition ] );
					m_tNormal.push_back( tTempNormal[ indFaceNormal ] );
					m_tTexcoord.push_back( tTempTexcoord[ indFaceTexcoord ] );
					face.ind[v] = m_tPosition.size()-1;
*/
					
				}


				m_tGroup[g].tFace.push_back(face); // on enregistre la face récupérée
			}
		}
//		delete[] buf;
	}
	fp.close();
/*
	cout << "m_tGroup.size() = " << m_tGroup.size() << endl;
	cout << "m_tPosition.size() = " << m_tPosition.size() << endl;
	cout << "m_tNormal.size() = " << m_tNormal.size() << endl;
	cout << "m_tTexcoord.size() = " << m_tTexcoord.size() << endl;
	cout << "tTempPosition.size() = " << tTempPosition.size() << endl;
	cout << "tTempNormal.size() = " << tTempNormal.size() << endl;
	cout << "tTempTexcoord.size() = " << tTempTexcoord.size() << endl;
*/
	tTempPosition.clear();
	tTempNormal.clear();
	tTempTexcoord.clear();



	GLuint nbFaces = 0;
	for(std::vector<sGroup>::iterator it=m_tGroup.begin(); it!=m_tGroup.end(); it++)
		nbFaces += (GLuint)(*it).tFace.size();
/*
	cout << "nbFaces = " << nbFaces << endl;
*/

	return true;
}

void ObjMesh::ComputeBoundingBox()
{

}

void ObjMesh::ComputeTangents()
{

	m_vTangent.resize( m_vNormal.size() );

	for(std::vector<sGroup>::iterator itG=m_tGroup.begin(); itG!=m_tGroup.end(); itG++) {
		for(std::vector<sFace>::iterator itF=(*itG).tFace.begin(); itF!=(*itG).tFace.end(); itF++) {
			GLuint* ind = ((GLuint*)(*itF).ind);
			osg::Vec3 vTangent;

			osg::Vec3 v0 = m_vPosition[ind[0]];
			osg::Vec3 v1 = m_vPosition[ind[1]];
			osg::Vec3 v2 = m_vPosition[ind[2]];

			osg::Vec3 vect10 = v0-v1;
			osg::Vec3 vect12 = v2-v1;

			float deltaT10 = m_vTexcoord[ind[0]].y() - m_vTexcoord[ind[1]].y();
			float deltaT12 = m_vTexcoord[ind[2]].y() - m_vTexcoord[ind[1]].y();

			vTangent = (vect10 * deltaT12 ) - (vect12 * deltaT10 );
			vTangent.normalize();

//			std::cout << "vNormal = " << m_tNormal[ind[0]].x << ", " << m_tNormal[ind[0]].y << ", " << m_tNormal[ind[0]].z << std::endl;
//			std::cout << "vTangent = " << vTangent.x << ", " << vTangent.y << ", " << vTangent.z << std::endl;

			m_vTangent[ind[0]] = m_vTangent[ind[1]] = m_vTangent[ind[2]] = vTangent;

		}
	}

/*
	m_tTangent.resize( m_tNormal.size() );

	vec3 *tan1 = new vec3[m_tPosition.size() * 2];
    vec3 *tan2 = tan1 + m_tPosition.size();
    memset(tan1, 0, m_tPosition.size() * sizeof(vec3) * 2);
    

	for(std::vector<sGroup>::iterator itG=m_tGroup.begin(); itG!=m_tGroup.end(); itG++)
	{
		for(std::vector<sFace>::iterator itF=(*itG).tFace.begin(); itF!=(*itG).tFace.end(); itF++)
		{
			GLuint* ind = ((GLuint*)(*itF).ind);

			long i1 = ind[0];
			long i2 = ind[1];
			long i3 = ind[2];
	        
			const vec3& v1 = m_tPosition[i1];
			const vec3& v2 = m_tPosition[i2];
			const vec3& v3 = m_tPosition[i3];
	        
			const vec2& w1 = m_tTexcoord[i1];
			const vec2& w2 = m_tTexcoord[i2];
			const vec2& w3 = m_tTexcoord[i3];
	        
			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;
	        
			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;
	        
			float r = 1.0f / (s1 * t2 - s2 * t1);
			vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
			vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);
	        
			tan1[i1] += sdir;
			tan1[i2] += sdir;
			tan1[i3] += sdir;
	        
			tan2[i1] += tdir;
			tan2[i2] += tdir;
			tan2[i3] += tdir;
		}
	}
    
	int a=0;
    for(std::vector<vec3>::iterator itV=m_tPosition.begin(); itV!=m_tPosition.end(); itV++)
    {
        const vec3& n = m_tNormal[a];
        const vec3& t = tan1[a];
        
        // Gram-Schmidt orthogonalize
        m_tTangent[a] = (t - n * Dot(n, t));

		m_tTangent[a].normalize();
        
        // Calculate handedness
    //    m_tTangent[a].w = (Dot(Cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;

		a++;
    }
    
    delete[] tan1;
*/



}


void ObjMesh::Draw()
{


	bind();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, 0);

	if(!m_vNormal.empty()) 
	{
		glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex));
	}
	if (!m_vTexcoord.empty()) 
	{
		glClientActiveTexture(GL_TEXTURE0);
		glTexCoordPointer( 2,GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex+m_iSizeNormal));
	}
	//Tangent
	if (!m_vTangent.empty()) 
	{
		glClientActiveTexture(GL_TEXTURE1);
		glTexCoordPointer( 3,GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex+m_iSizeNormal+m_iSizeTexture));
	}
	

	for(std::vector<sGroup>::iterator it=m_tGroup.begin(); it!=m_tGroup.end(); it++)
		glDrawElements(GL_TRIANGLES, (GLsizei)(*it).tFace.size()*3, GL_UNSIGNED_INT, &((*it).tFace[0].ind[0]));

	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY); 

	unbind();
}


void ObjMesh::Draw(GLuint group)
{
	assert(group < (GLuint)m_tGroup.size());

	bind();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, 0);

	if(!m_vNormal.empty()) 
	{
		glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex));
	}
	if (!m_vTexcoord.empty()) 
	{
		glClientActiveTexture(GL_TEXTURE0);
		glTexCoordPointer( 2,GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex+m_iSizeNormal));
	}
	//Tangent
	if (!m_vTangent.empty()) 
	{
		glClientActiveTexture(GL_TEXTURE1);
		glTexCoordPointer( 3,GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex+m_iSizeNormal+m_iSizeTexture));
	}

	glDrawElements(GL_TRIANGLES, (GLsizei)m_tGroup[group].tFace.size()*3, GL_UNSIGNED_INT, &(m_tGroup[group].tFace[0].ind[0]));

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY); 

	unbind();
}




