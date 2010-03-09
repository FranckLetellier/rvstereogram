/*************************************
*
*		ImacDemo Project
*		
*		Created :	24/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/

#ifndef __GRASS_H__
#define __GRASS_H__

#include <stdlib.h>
#include <string>
#include <iostream>
#include <time.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "shaderManager.h"
#include "shader.h"
#include "textureManager.h"
#include "texture2D.h"

class grass
{

	public:
		grass(int iType = 1, int iTurn = 1);
		~grass();
		void draw() const;
	
	private:

		// turn direction (1 or -1);
		int m_iTurn;

		// texture to be used 0 = grass, 1 = flowers
		int m_iType;
};


#endif // __GRASS_H__