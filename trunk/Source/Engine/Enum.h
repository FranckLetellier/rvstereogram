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

#ifndef __ENUM_H__
#define __ENUM_H__

/**
*	@name Enum.h
*	@brief Enum used in the engine
*	@author Franck Letellier
*	@date 06/11/09
*/

enum CameraFlagEnum
{
	E_CAMERA_IDLE = 0,
	E_CAMERA_FORWARD,
	E_CAMERA_BACKWARD,
	E_CAMERA_LEFT,
	E_CAMERA_RIGHT,

};


enum FboEnum
{
	E_FBO_CUBE = 0,
	E_FBO_2D,

};

#endif //__ENUM_H__