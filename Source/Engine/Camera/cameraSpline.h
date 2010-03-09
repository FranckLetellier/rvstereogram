/*************************************
*
*		ImacDemo Project
*		
*		Created :	04/01/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*					Helena Duchaussoy
*
**************************************/

#ifndef __CAMERASPLINE_H__
#define __CAMERASPLINE_H__

/**
*	@name cameraSpline.h
*	@brief Camera with Spline control
*	@author : Franck Letellier
*/
#include <stdlib.h>
#include <cmath>

#include "abstractCamera.h"


class CameraSpline:public AbstractCamera
{
	public:
		CameraSpline();
		virtual ~CameraSpline(){};

		virtual void init();
		virtual void update();

		virtual void updateMouse(int x, int y);

		virtual void invertLookAt(float fHeight);
		virtual void reset();
		virtual void setStart(unsigned int iPos, unsigned int iAim, unsigned int iControl);

		inline virtual unsigned int getCurrentControlPoint(){return m_iIndiceControlSpline - 1;}
		inline virtual unsigned int getCurrentPositionPoint(){return m_iIndiceSplineCam -1;}
		inline virtual unsigned int getCurrentAimPoint(){return m_iIndiceAimSplineCam -1;}


	protected:

		unsigned int	m_iIndiceSplineCam;
		unsigned int	m_iIndiceAimSplineCam;
		unsigned int	m_iIndiceControlSpline;

};


#endif //__CAMERASPLINE_H__