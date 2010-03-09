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

#ifndef __OUTDOORTSCENE_H__
#define __OUTDOORTSCENE_H__

/**
*	@name OutdoorScene.h
*	@brief Scene outside, with water and heightmap
*	@author Franck Letellier
*	@date 18/12/09
*/

#include "abstractScene.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "grassField.h"

//Forward declaration
class HeightMap;
class Plan;
class CubeMapObject;
class FBO;



class OutdoorScene: public AbstractScene
{
	public:
		OutdoorScene(const std::string& sName, AbstractCamera* pCam);
		virtual ~OutdoorScene();

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
		HeightMap		*map;
		Plan			*water;
		Plan			*ground;
		CubeMapObject	*cubeMap;
		
		FBO*			m_pWaterReflexionFBO;
		
		GLfloat*	m_fLightPosition;
		float	m_fCubemapMix;

		float		m_fWaterLevel;
		float m_fSunAngle;
		float m_fSunDistance;
		bool debug;

		GrassField * m_pGrass0;
		float m_fGrassAngle;

		///In order to save the camera state
		unsigned int m_iStartPosition;
		unsigned int m_iStartAim;
		unsigned int m_iStartControlPosition;


		void renderEnvironment(bool m_bWater);
		void renderWater();
		void renderGrass();
		void drawTemple();
		void drawTree();

};


#endif //__OUTDOORTSCENE_H__

