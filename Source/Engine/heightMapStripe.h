/*************************************
*
*		ImacDemo Project
*		
*		Created :	19/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/

#ifndef __HEIGHTMAPSTRIPE_H__
#define __HEIGHTMAPSTRIPE_H__

#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

class HeightMapStripe
{
	public:
		HeightMapStripe(unsigned int iNbVertex, GLfloat* pVertex,GLfloat* pNormal,GLfloat* pTexture,GLfloat * pTangent);
		~HeightMapStripe();

		void draw();
		void bind();
		void unbind();

		const GLfloat* getVertexArray();
		void computeNormal(HeightMapStripe* previous, HeightMapStripe* next);

	private:
		void build();
		void generate();
		void destroy();

		unsigned int	m_iNbVertex;
		GLuint			m_iBufferId;
		GLfloat*		m_pVertex;
		GLfloat*		m_pNormal;
		GLfloat*		m_pTexture;
		GLfloat*		m_pTangent;

		GLsizeiptr m_iSizeVertex;
		GLsizeiptr m_iSizeNormal;
		GLsizeiptr m_iSizeTexture;
		GLsizeiptr m_iSizeTangent;

};

#endif //__HEIGHTMAPSTRIPE_H__
