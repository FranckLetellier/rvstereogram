#include "abstractCamera.h"
#include "assert.h"

AbstractCamera::AbstractCamera()
{

	m_bisPositioned = false;
	
}

void AbstractCamera::init()
{

	pos.x() = 0;
	pos.y() = 0;
	pos.z() = 0;

	aim.x() = 0;
	aim.y() = 0;
	aim.z() = 0;

	up.x() = 0;
	up.y() = 1;
	up.z() = 0;

	m_vDirection.x() = 0.0;
	m_vDirection.y() = 0.0;
	m_vDirection.z() = 0.0;

	m_vAim.x() = 0.0;
	m_vAim.y() = 0.0;
	m_vAim.z() = 0.0;


	m_bForward = false;
	m_bBackward = false;
	m_bLeft = false;
	m_bRight = false;

	m_fSpeed = 0.05;

}


bool AbstractCamera::getFlag(CameraFlagEnum f)
{
	switch(f)
	{
	case E_CAMERA_FORWARD:
		return m_bForward;
	case E_CAMERA_BACKWARD:
		return m_bBackward;
	case E_CAMERA_LEFT:
		return m_bLeft;
	case E_CAMERA_RIGHT:
		return m_bRight;
	default:
		return false;
	}
}

bool AbstractCamera::setFlag(CameraFlagEnum f, bool b)
{
	switch(f)
	{
	case E_CAMERA_FORWARD:
		m_bForward = b;
		return true;
		
	case E_CAMERA_BACKWARD:
		m_bBackward = b;
		return true;
		
	case E_CAMERA_LEFT:
		m_bLeft = b;
		return true;
		
	case E_CAMERA_RIGHT:
		m_bRight = b;
		return true;
	default:
		return false;
	}
}

void AbstractCamera::lookAt()
{
	gluLookAt(pos.x(),pos.y(),pos.z(),aim.x(),aim.y(),aim.z(),up.x(),up.y(),up.z());
}

void AbstractCamera::reset()
{
	init();
}

void AbstractCamera::invertLookAt(float fHeight)
{
}
