#include "heightMapStripe.h"

//Specification given
#define BUFFER_OFFSET(i) ((void*)(i))

#include <iostream>
#include <assert.h>
#include "Vec3f"

HeightMapStripe::HeightMapStripe(unsigned int iNbVertex, GLfloat* pVertex,GLfloat* pNormal, GLfloat* pTexture,GLfloat * pTangent):
	m_iNbVertex(iNbVertex),
	m_pVertex(pVertex),
	m_pNormal(pNormal),
	m_pTexture(pTexture),
	m_pTangent(pTangent)
{

	/*for (unsigned int i=0;i<(m_iNbVertex * 2);i+=2)
	{
		std::cout<<m_pTexture[i]<<std::endl;
		std::cout<<m_pTexture[i+1]<<std::endl;
		std::cout<<"----------------------------"<<std::endl;
	}*/

	m_iBufferId = 0;
	build();

}
HeightMapStripe::~HeightMapStripe()
{
	delete m_pVertex;
	delete m_pNormal;
	delete m_pTexture;
	if (m_pTangent)
		delete m_pTangent;

	destroy();
}

void HeightMapStripe::draw()
{
	bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, 0);
	if (m_pNormal != NULL)
	{
		glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex));
	}
	if (m_pTexture != NULL)
	{
		glClientActiveTexture(GL_TEXTURE0);
		glTexCoordPointer( 2,GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex+m_iSizeNormal));
	}
	//Tangent
	if (m_pTangent != NULL)
	{
		glClientActiveTexture(GL_TEXTURE1);
		glTexCoordPointer( 3,GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex+m_iSizeNormal+m_iSizeTexture));
	}
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_iNbVertex);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY); 
	unbind();
}

void HeightMapStripe::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_iBufferId);
}

void HeightMapStripe::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void HeightMapStripe::build()
{
	destroy();
	generate();
	bind();

	//Size of the buffer needed 
	m_iSizeVertex =  (m_iNbVertex * 3 * sizeof(float));
	m_iSizeNormal = m_iSizeVertex;
	m_iSizeTexture = (m_iNbVertex * 2 * sizeof(float));
	m_iSizeTangent = m_iSizeVertex;
	//Reserve a Buffer for all the data
	glBufferData(GL_ARRAY_BUFFER, m_iSizeVertex + m_iSizeNormal + m_iSizeTexture + m_iSizeTangent, 0, GL_STATIC_DRAW);

	//Sub buffer for the Vertex position
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_iSizeVertex, m_pVertex);
	//Sub buffer for the Normals (from the end of iSizevertex to iSize normal
	if (m_pNormal != NULL)
	glBufferSubData(GL_ARRAY_BUFFER, m_iSizeVertex, m_iSizeNormal, m_pNormal);
	//Sub buffer for the Texture coord - Texture unit 0
	if (m_pTexture != NULL)
		glBufferSubData(GL_ARRAY_BUFFER, m_iSizeNormal + m_iSizeVertex, m_iSizeTexture, m_pTexture);
	//Sub buffer for the Tangent coord - Texture Unit 1
	if (m_pTangent != NULL)
	glBufferSubData(GL_ARRAY_BUFFER, m_iSizeNormal + m_iSizeVertex + m_iSizeTexture, m_iSizeTexture, m_pTangent);

	/*glVertexPointer(3, GL_FLOAT, 0, 0);
	glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex));
	glTexCoordPointer( 2,GL_FLOAT, 0, BUFFER_OFFSET(m_iSizeVertex+m_iSizeNormal));*/

	unbind();

}

const GLfloat* HeightMapStripe::getVertexArray()
{
	return m_pVertex;
}

void HeightMapStripe::computeNormal(HeightMapStripe* previous, HeightMapStripe* next)
{
	
	osg::Vec3f point0, point1, point2, point3, point4, X, Y;
	osg::Vec3f norm, norm1, norm2, norm3, norm4;

	int cpt = 6;

	for(unsigned int i=1;i<((m_iNbVertex/2.0)-1);++i){

		point0.x() = m_pVertex[cpt+0];
		point0.y() = m_pVertex[cpt+1];
		point0.z() = m_pVertex[cpt+2];

		point1.x() = previous->getVertexArray()[cpt+0];
		point1.y() = previous->getVertexArray()[cpt+1];
		point1.z() = previous->getVertexArray()[cpt+2];

		point2.x() = m_pVertex[cpt-6+0];
		point2.y() = m_pVertex[cpt-6+1];
		point2.z() = m_pVertex[cpt-6+2];

		point3.x() = m_pVertex[cpt+6+0];
		point3.y() = m_pVertex[cpt+6+1];
		point3.z() = m_pVertex[cpt+6+2];

		point4.x() = m_pVertex[cpt+3+0];
		point4.y() = m_pVertex[cpt+3+1];
		point4.z() = m_pVertex[cpt+3+2];

		X=point1-point0;
		Y=point3-point0;
		norm1=(X^Y);
		norm1.normalize();

		X=point3-point0;
		Y=point4-point0;
		norm2=(X^Y);
		norm2.normalize();

		X=point4-point0;
		Y=point2-point0;
		norm3=(X^Y);
		norm3.normalize();

		X=point2-point0;
		Y=point1-point0;
		norm4=(X^Y);
		norm4.normalize();

		norm=((norm4+norm3+norm2+norm1)/4.0f);
		norm.normalize();

		m_pNormal[cpt+0]=norm.x();
		m_pNormal[cpt+1]=norm.y();
		m_pNormal[cpt+2]=norm.z();

		cpt+=3;

		point0.x() = m_pVertex[cpt+0];
		point0.y() = m_pVertex[cpt+1];
		point0.z() = m_pVertex[cpt+2];
		
		point1.x() = m_pVertex[cpt-3+0];
		point1.y() = m_pVertex[cpt-3+1];
		point1.z() = m_pVertex[cpt-3+2];

		point2.x() = m_pVertex[cpt-6+0];
		point2.y() = m_pVertex[cpt-6+1];
		point2.z() = m_pVertex[cpt-6+2];

		point3.x() = m_pVertex[cpt+6+0];
		point3.y() = m_pVertex[cpt+6+1];
		point3.z() = m_pVertex[cpt+6+2];

		point4.x() = next->getVertexArray()[cpt+0];
		point4.y() = next->getVertexArray()[cpt+1];
		point4.z() = next->getVertexArray()[cpt+2];

		X=point1-point0;
		Y=point3-point0;
		norm1=(X^Y);
		norm1.normalize();

		X=point3-point0;
		Y=point4-point0;
		norm2=(X^Y);
		norm2.normalize();

		X=point4-point0;
		Y=point2-point0;
		norm3=(X^Y);
		norm3.normalize();

		X=point2-point0;
		Y=point1-point0;
		norm4=(X^Y);
		norm4.normalize();

		norm=((norm4+norm3+norm2+norm1)/4.0f);
		norm.normalize();

		m_pNormal[cpt+0]=norm.x();
		m_pNormal[cpt+1]=norm.y();
		m_pNormal[cpt+2]=norm.z();

		cpt+=3;
	}
	build();
}

void HeightMapStripe::generate()
{
	glGenBuffers(1,  &m_iBufferId);
	assert(m_iBufferId);
}

void HeightMapStripe::destroy()
{
	glDeleteBuffers(1, &m_iBufferId);
}
