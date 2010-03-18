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

#ifndef __ABSTRACTSCENE_H__
#define __ABSTRACTSCENE_H__

/**
*	@name AbstractScene.h
*	@brief Virtual Class of a Scene
*	@author Franck Letellier
*	@date 18/12/09
*/

#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "enum.h"
#include "glh/glh_linear.h"

class FBO;
class AbstractCamera;
class AbstractScene
{
	public:
		AbstractScene(const std::string& sName, AbstractCamera*	cam);
		virtual ~AbstractScene();

		// Initialise a Scene
		virtual bool init() = 0;

		//Pre display (for FBO/lights and such...)
		virtual void display();
		virtual void preRender() = 0;
		virtual void render() = 0;
		
		virtual void update(); 
		virtual bool isFinished() = 0;
		virtual void handleKeyUp(unsigned char c, int x, int y);
		virtual void handleKeyDown(unsigned char c, int x, int y) = 0;
		virtual void reset();

		inline void setCamera(AbstractCamera* pCam){m_pCam = pCam;}

		inline const std::string& getSuccessorName(){return m_sNextSceneName;};
		inline const std::string& getName(){return m_sName;};

		static bool globalInit(int windowWidth, int windowHeight);
		static void displayOnQuad(int windowwidth, int windowheight);

		static FBO * fboColorDepth;
		static FBO * fboWriteAlpha;
		static FBO * fboBlurH;
		static FBO * fboBlurV;
		static FBO * finalFBO;
		static FBO * postProcessFBO0;
		static FBO * postProcessFBO1;


	protected:
		std::string m_sName;
		std::string m_sNextSceneName;

		AbstractCamera*		m_pCam;

		static unsigned int iWindowWidth;
		static unsigned int iWindowHeight;
		
		static bool m_bUseDOF;
		GLuint m_iSIRDS;
		
		glh::matrix4f m_pProjectionMatrix;
		glh::matrix4f m_pModelviewMatrix;
		glh::matrix4f m_pPreviousProjectionMatrix;
		glh::matrix4f m_pInverseProjectionMatrix;


		static GLfloat * m_fWhiteColor;
		static GLfloat * m_fBlackColor;
		static GLfloat * m_fMediumGreyColor;
		static GLfloat * m_fDarkGreyColor;
		static GLfloat * m_fRedColor;
		static GLfloat * m_fOrangeColor;
		static GLfloat * m_fElectricGreenColor;
		static GLfloat * m_fPurpleColor;
		static GLfloat * m_fLightYellow;

};


#endif //__ABSTRACTSCENE_H__

