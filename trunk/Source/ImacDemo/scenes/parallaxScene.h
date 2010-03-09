/*************************************
*
*		ImacDemo Project
*		
*		Created :	23/12/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*					Helena Duchaussoy
*
**************************************/

#ifndef __PARALLAXSCENE_H__
#define __PARALLAXSCENE_H__

/**
*	@name parallaxScene.h
*	@brief Scene with parallax
*	@author Franck Letellier
*	@date 23/12/09
*/

#include "abstractScene.h"

//Forward declaration
class FBO;

class ParallaxScene: public AbstractScene
{
	public:
		ParallaxScene(const std::string& sName, AbstractCamera* pCam);
		virtual ~ParallaxScene();

		// Initialise a Scene
		virtual bool init();

		//Pre display (for FBO/lights and such...)
		virtual void preRender();
		virtual void render();
		
		virtual void update(); 
		virtual bool isFinished();
		virtual void handleKeyUp(unsigned char c, int x, int y);
		virtual void handleKeyDown(unsigned char c, int x, int y){};
		virtual void reset();

		static int a;

	private:
				
		GLfloat*	m_fLightPosition;
		float		fAngle;
		float		fAngleObject;

};


#endif //__PARALLAXSCENE_H__

