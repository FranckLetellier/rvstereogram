/*************************************
*
*		ImacDemo Project
*		
*		Created :	18/12/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*					Helena Duchaussoy
*
**************************************/

#ifndef __SHADOWSCENE_H__
#define __SHADOWSCENE_H__

/**
*	@name ShadowScene.h
*	@brief don't know yet :)
*	@author baptiste malaga
*	@date 21/12/09
*/
#include <time.h>
#include "abstractScene.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

//Forward declaration
class FBO;

class ShadowScene: public AbstractScene
{
	public:
		ShadowScene(const std::string& sName, AbstractCamera* pCam);
		virtual ~ShadowScene();

		// Initialise a Scene
		virtual bool init();

		//Pre display (for FBO/lights and such...)
		virtual void preRender();
		virtual void render();
		
		virtual void update(); 
		virtual bool isFinished();
		virtual void handleKeyUp(unsigned char c, int x, int y);
		virtual void handleKeyDown(unsigned char c, int x, int y);
		virtual void reset();

	private:

		FBO* m_pShadowMapFBO;

		unsigned int iShadowMapWidth;
		unsigned int iShadowMapHeight;

		float modelviewMatrix[16];
		float projectionMatrix[16];
		float shadowmatrix[16];
		float transformationmatrix[16];

		GLfloat*	m_fLightPosition;
		GLfloat*	m_fLightDistance;
		GLfloat*	m_fLightDirection;
		GLfloat*	m_fLightZeros;

		GLfloat spotCutOff0;
		float  fallOff0;
		float  cosfallOff0;

		float fAngle;

		float depLightPSpeedX;
		float depLightPSpeedY;

		float depLightPAttX;
		float depLightPAttY;

		int depLightPSignX;
		int depLightPSignY;

		void renderEnvironment(bool m_bShadow);


};


#endif //__SHADOWSCENE_H__

