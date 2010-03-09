/*************************************
*
*		ImacDemo Project
*		
*		Created :	30/10/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/


#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <iostream>






#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//Forward class declaration
class AbstractCamera;
class CubeMapObject;

// Origine de la fenetre
static const unsigned int windowx = 100;
static const unsigned int windowy = 100;

// Dimensions de la fenetre
static unsigned int windowwidth  = 1024;
static unsigned int windowheight = 768;

//Timer variables
static unsigned int timebase;
static unsigned int time;
static unsigned int frames = 0;

AbstractCamera *cam;

//Value that could be stored into a XML
CubeMapObject *cubeMap;

GLenum err;
