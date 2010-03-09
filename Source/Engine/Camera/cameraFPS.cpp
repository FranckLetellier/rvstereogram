#include "cameraFPS.h"
#include <iostream>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

CameraFPS::CameraFPS(unsigned int windowwidth, unsigned int windowheight):
AbstractCamera(),
windowwidth(windowwidth),
windowheight(windowheight)
{
	init();
}

void CameraFPS::init()
{
	AbstractCamera::init();
	m_fSpeed = 0.2;

	vitesseSouris = 0.1f;

	alpha=0;
	phi=-90;

	m_bForward = m_bBackward = m_bLeft = m_bRight = false;

	vectorsFromAngles();

}

void CameraFPS::vectorsFromAngles(){

	direct.x()=sin(phi*M_PI/180)*sin(alpha*M_PI/180);
	direct.y()=cos(phi*M_PI/180);
	direct.z()=sin(phi*M_PI/180)*cos(alpha*M_PI/180);

	//produit vectoriel
	right = up^direct;
	
	//normalisation
	right.normalize();

	aim = pos + direct;

}



void CameraFPS::update(){


	//std::cout<<pos.x() <<" / "<<pos.y()<< " / "<<pos.z()<<std::endl;
	if(m_bForward){
		pos.x()+=m_fSpeed*direct.x();
		pos.y()+=m_fSpeed*direct.y();
		pos.z()+=m_fSpeed*direct.z();
	}

	if(m_bLeft){
		pos.x()+=m_fSpeed*right.x();
		pos.z()+=m_fSpeed*right.z();	
	}

	if(m_bRight){
		pos.x()-=m_fSpeed*right.x();
		pos.z()-=m_fSpeed*right.z();
	}

	if(m_bBackward){
		pos.x()-=m_fSpeed*direct.x();
		pos.y()-=m_fSpeed*direct.y();
		pos.z()-=m_fSpeed*direct.z();	
	}

	vectorsFromAngles();
}

void CameraFPS::updateMouse(int x, int y){
	
	alpha+=(windowwidth/2.0-x)*vitesseSouris;
	phi+=(windowheight/2.0-y)*vitesseSouris;

	if(phi>-1)
	{
		phi=-1;
	}
	if(phi<-179)
	{
		phi=-179;
	}
		/*  */
	if(alpha>360 || alpha < -360) alpha=0.0;

	vectorsFromAngles();
}

void CameraFPS::invertLookAt(float fHeight)
{
	gluLookAt(pos.x(),2.0*fHeight-pos.y(),pos.z(),aim.x(),2.0*fHeight-aim.y(),aim.z(),-up.x(),-up.y(),-up.z());
	
}


void CameraFPS::reset()
{
	init();
}
