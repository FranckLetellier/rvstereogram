#include "cameraSpline.h"
#include "Spline.hpp"

CameraSpline::CameraSpline():
AbstractCamera()
{
	init();

}

void CameraSpline::init()
{
	AbstractCamera::init();
	m_fSpeed = 0.15f;
	m_iIndiceSplineCam = 1;
	m_iIndiceAimSplineCam = 1;
	m_iIndiceControlSpline = 1;
}


void CameraSpline::update()
{

	if (m_pSpline != NULL)
	{	
		///If we are near a new CONTROL point
		if ((pos - m_pSpline->getControlPointAt(m_iIndiceControlSpline)).length() < 0.5)
		{
			++m_iIndiceControlSpline;
			if (m_iIndiceControlSpline > m_pSpline->getControlSize()-1)
			m_iIndiceControlSpline=0;
		}

		//Check if we are near a point
		if ((pos - m_pSpline->getSplinePointAt(m_iIndiceSplineCam)).length() < m_fSpeed)
		{
			

			//We are setting the new direction vector
			++m_iIndiceSplineCam;

			if (m_iIndiceSplineCam > m_pSpline->getSplineSize()-1)
			m_iIndiceSplineCam=0;

			m_vDirection = m_pSpline->getSplinePointAt(m_iIndiceSplineCam) - pos;
			m_vDirection.normalize();
		}
		pos += m_vDirection * m_fSpeed;
	}
	else
		std::cerr<<"ERROR : The pos spline is NULL"<<std::endl;

	if (m_pAimSpline != NULL)
	{
		//Same for Aim
		if ((aim - m_pAimSpline->getSplinePointAt(m_iIndiceAimSplineCam)).length() < m_fSpeed)
		{
			++m_iIndiceAimSplineCam;

			if (m_iIndiceAimSplineCam > m_pAimSpline->getSplineSize()-1)
				m_iIndiceAimSplineCam=0;
				
			m_vAim = m_pAimSpline->getSplinePointAt(m_iIndiceAimSplineCam) - aim;
			m_vAim.normalize();
		}
 		aim += m_vAim * m_fSpeed;
		
		///aim.x() = 40;aim.y() = 4;aim.z() = -40;
	
	}
	else
		std::cerr<<"ERROR : The Aim spline is NULL"<<std::endl;
			
		/*++m_iIndiceSplineCam;
		if (m_iIndiceSplineCam > m_pSpline->getSplineSize()-1)
			m_iIndiceSplineCam=0;

		pos = m_pSpline->getSplinePointAt(m_iIndiceSplineCam);
		*/
}

void CameraSpline::updateMouse(int x, int y)
{
	
	
}

void CameraSpline::invertLookAt(float fHeight)
{
	gluLookAt(pos.x(),2.0*fHeight-pos.y(),pos.z(),aim.x(),2.0*fHeight-aim.y(),aim.z(),-up.x(),-up.y(),-up.z());
}

void CameraSpline::setStart(unsigned int iPos, unsigned int iAim, unsigned int iControl)
{
	///You can choose to start wherever you want
	m_iIndiceSplineCam = iPos;
	m_iIndiceAimSplineCam = iAim;
	m_iIndiceControlSpline = 1;

	pos = m_pSpline->getSplinePointAt(m_iIndiceSplineCam);
	++m_iIndiceSplineCam;
	m_vDirection = m_pSpline->getSplinePointAt(m_iIndiceSplineCam) - pos;
	m_vDirection.normalize();


	aim = m_pAimSpline->getSplinePointAt(m_iIndiceAimSplineCam);
	++m_iIndiceAimSplineCam;
	m_vAim = m_pAimSpline->getSplinePointAt(m_iIndiceAimSplineCam) - aim;
	m_vAim.normalize();

	m_iIndiceControlSpline = iControl+2;
	
}

void CameraSpline::reset()
{
	init();
}
