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

#ifndef __SCIFISCENE_H__
#define __SCIFISCENE_H__

/**
*	@name SciFiScene.h
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

class SciFiScene: public AbstractScene
{
	public:
		SciFiScene(const std::string& sName, AbstractCamera* pCam);
		virtual ~SciFiScene();

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


		GLfloat*	m_fLightPosition;
		GLfloat*	m_fLightDistance;
		GLfloat*	m_fLightDirection;
		GLfloat*	m_fLightZeros;
		float fAngle;

		float depLightPSpeedX;
		float depLightPSpeedY;

		float depLightPAttX;
		float depLightPAttY;

		int depLightPSignX;
		int depLightPSignY;

		void renderEnvironment(bool m_bGlow);

		float m_fMixValue;
		int m_iMixSign;

};


#endif //__SCIFISCENE_H__

