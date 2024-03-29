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

#ifndef __TESTSCENE_H__
#define __TESTSCENE_H__

/**
*	@name ToonScene.h
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

class ToonScene: public AbstractScene
{
	public:
		ToonScene(const std::string& sName, AbstractCamera* pCam);
		virtual ~ToonScene();

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

		FBO* m_pDepthMapFBO;

		float fAngle;
		float fSpeedUp;

		float fRotation;
		float fRotationPiou;

		float fLeftStrenght;
		float fRightStrenght;

		bool bRotateLeft;
		bool bRotateRight;
		bool bSpeedUp;

		void renderEnvironment();

};


#endif //__TESTSCENE_H__

