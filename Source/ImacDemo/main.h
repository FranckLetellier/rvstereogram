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

#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <iostream>

#include "Enum.h"


//Forward class declaration
class AbstractCamera;
class HeightMap;
class Plan;
class ObjMesh;
class FBO;


// Origine de la fenetre
static const unsigned int windowx = 100;
static const unsigned int windowy = 100;

// Dimensions de la fenetre
static unsigned int windowwidth  = 1024;
static unsigned int windowheight = 768;

float near = 0.4f;
float far = 1000.0f;
float ouverture = 45.0f;

//Timer variables
static unsigned int timebaseFPS;
static unsigned int timebase;
static unsigned int currentTime;
static unsigned int frames = 0;

AbstractCamera *cam;
bool m_bCamFPS;

static GLfloat lightPosition[] = {5.0,30.0,0.0,1.0};
static GLfloat red[] = {1.0,0.0,0.0,1.0};
static GLfloat white[] = {1.0,1.0,1.0,1.0};
static GLfloat black[] = {0.0,1.0,0.0,1.0};

GLenum err;

