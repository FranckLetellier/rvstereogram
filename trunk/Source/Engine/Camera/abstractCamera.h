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

#ifndef __ABSTRACTCAMERA_H__
#define __ABSTRACTCAMERA_H__

#include "../Enum.h"
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <vector>
#include "Vec3"

/**
*	@name AbstractCamera.h
*	@brief Abtract camera class
*	@author Franck Letellier
*	@date 06/11/09
*/

class AbstractCamera
{
	public :
		AbstractCamera();
		virtual ~AbstractCamera() {};

		virtual void init();

		virtual void lookAt();

		/**
		*	@name void update()
		*	@brief This method is called when everything has been set up for the next step
		*/
		virtual void update() = 0;

		virtual void updateMouse(int x, int y) = 0;
		

		/**
		*	Getter
		*/
		virtual bool getFlag(CameraFlagEnum f);

		/**
		*	@name virtual bool setFlag(CameraFlagEnum f, bool b);
		*	@param f the flag we want to change
		*	@param b the value of the bool
		*	@return true is the flag exist, false otherwise
		*/
		virtual bool setFlag(CameraFlagEnum f, bool b);


		inline virtual const osg::Vec3& getPos(){return pos;}
		inline virtual const osg::Vec3& getDiection(){return  direct;}
		inline virtual const osg::Vec3& getUp(){return up;}
		inline virtual const osg::Vec3& getAim(){return aim;}

		/*
		* @name virtual void invertLookAt(float fHeight);
		* @param fHeight The height to know where to flip the lookAt
		*/
		virtual void invertLookAt(float fHeight);
		

		inline virtual void setSpeed(float fSpeed){m_fSpeed = fSpeed;}

		virtual void reset();

		inline virtual unsigned int getCurrentControlPoint(){return 0;}
		inline virtual unsigned int getCurrentPositionPoint(){return 0;}
		inline virtual unsigned int getCurrentAimPoint(){return 0;}
		virtual void setStart(unsigned int iPos, unsigned int iAim, unsigned int iControl){};
	protected:
		
		///those bool are here in order to avoid key lagging
		///Mininum flags, but we can add more in the child classes
		bool m_bForward;
		bool m_bBackward;
		bool m_bLeft;
		bool m_bRight;

		osg::Vec3 up;
		osg::Vec3 direct;
		osg::Vec3 aim;
		osg::Vec3 pos;
		osg::Vec3 right;

		osg::Vec3f		m_vDirection;
		osg::Vec3f		m_vAim;

		float m_fSpeed;
		bool m_bisPositioned;

};

#endif //__ABSTRACTCAMERA_H__