//---------------------------------------------------------------------
//
// PROJECT ....
// Copyright (C) 
//
// All rights reserved
//
//---------------------------------------------------------------------


///\file
///  XML parser for splines's points files
///\author Helena DUCHAUSSOY, Fabien KAPALA, Franck LETELLIER, Baptiste MALAGA



#ifndef __READXMLSPLINE_H__
#define __READXMLSPLINE_H__

#include "tinyxml.h"

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include <osg/Vec3f>


///\brief load the control points of a spline 
///\param the path of the file to read
/// will be used on the Spline's constructor
std::vector<osg::Vec3f> loadXMLFile(const char * name);


#endif
