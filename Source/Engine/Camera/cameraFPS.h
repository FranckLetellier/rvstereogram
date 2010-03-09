/*************************************
*
*		ImacDemo Project
*		
*		Created :	06/11/09
*		Authors :	Franck Letellier
*					Baptiste Malaga
*					Fabien Kapala
*
**************************************/

#ifndef __CAMERAFPS_H__
#define __CAMERAFPS_H__

/**
*	@name cameraFPS.h
*	@brief Camera with FPS-like control
*	@author : Baptiste Malaga
*/
#include <stdlib.h>
#include <cmath>

#include "abstractCamera.h"

class CameraFPS:public AbstractCamera
{
	public:
		CameraFPS(unsigned int windowwidth,unsigned int windowheight);
		virtual ~CameraFPS(){};

		virtual void init();
		virtual void update();

		virtual void updateMouse(int x, int y);

		virtual void invertLookAt(float fHeight);
		virtual void reset();


	protected:
		void vectorsFromAngles();

		unsigned int windowwidth;
		unsigned int windowheight;

		float vitesseDeplacement;
		float vitesseSouris;

		double alpha;
		double phi;	
};


#endif //__CAMERAFPS_H__