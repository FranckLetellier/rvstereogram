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

#ifndef __GRASSFIELD_H__
#define __GRASSFIELD_H__

#include <stdlib.h>
#include <time.h>
#include <string>
#include <list>
#include <iostream>
#include <vector>

#include "grass.h"
#include "heightmap.h"

enum GrassType { GRASS_CIRCLE , GRASS_SQUARE };

struct grassPoint{
		float x;
		float y;
		float rotation;
};

class GrassField
{
	public:
		
		GrassField(GrassType type,int dim, int spreadX, int spreadY);
		~GrassField();		
		void draw(HeightMap * const map, const float posX, const float posZ);

	private:
		void init();
		void initSquare();
		void initCircle();
		
	private:
		GrassType type;
		int dim;
		int spreadX;
		int spreadY;

		std::vector<grassPoint> * grassPoints; 
		std::vector<grass *> * herbe;

};


#endif // __GRASSFIELD_H__