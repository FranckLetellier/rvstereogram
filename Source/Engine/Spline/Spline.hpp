//---------------------------------------------------------------------
//
// PROJECT ....
// Copyright (C) 
//
// All rights reserved
//
//---------------------------------------------------------------------


///\file
/// Spline header
///\author Helena DUCHAUSSOY, Fabien KAPALA, Franck LETELLIER, Baptiste MALAGA


#ifndef __SPLINE_H__
#define __SPLINE_H__

#include <string>
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "ReadXMLSpline.hpp"
#include "InvMatrix.hpp"

class Spline{
	public : 
		///\brief Create a camera Spline from reading the file named filename.
		///\param the path of the file to read
		/// Calls the load function from ReadXML.hpp
		Spline(const char * filename); 

		///\brief Create a camera Spline from reading the file named filename.
		///\param the path of the file to read
		///\param the precision
		/// Calls the load function from ReadXML.hpp
		Spline(const char * filename,float precision); 
		 ~Spline();
		
		///\brief Get a point of the control points list
		///\param the index of the needed point
		///\return an osg::Vec3 containing the x,y,z composantes of the point
		inline const osg::Vec3f getControlPointAt(const int index){ return controlPoints.at(index); };
		
		///\brief Function to get a point of the Spline
		///\param the index of the needed point
		///\return an osg::Vec3 containing the x,y,z composantes of the point
		inline const osg::Vec3f getSplinePointAt(const int index){ return splinePoints.at(index); };
		
		///\brief Give the spline's number of points
		inline const unsigned int getSplineSize(){ return splinePoints.size(); };
		
		///\brief Give the spline's number of control points
		inline const unsigned int getControlSize(){ return controlPoints.size(); };

		///\brief Display the spline
		void draw();
		
	private :
		
		
		std::vector<osg::Vec3f> controlPoints; ///< Vector containing all the control points of the Spline
		std::vector<osg::Vec3f> splinePoints; ///< Vector containing all the points of the Spline
			
		///\brief Fills the squared left matrix used to create the Spline
		///\return a Matrix of fixed values 
		boost::numeric::ublas::matrix<float> fillLeftMatrix(int size);
		
		///\brief Fills with zero the vector wich will contain the result of one of the composants (x,y, or z)
		///\return a vector of "rows" size 
		boost::numeric::ublas::vector<float> initDeltaVector(int rows);
		
		///\brief Fills a vector with a certain equation of one of the composant of the control points
		///\brief 0 for x, 1 for y and 2 for z
		///\return a vector of the same size that the vector v (the number of control points)
		boost::numeric::ublas::vector<float> fillRightVector(const std::vector<osg::Vec3f> v, int composant);
		
		///\brief calculate one of the composants of the point using the control points and the delta vector
		float getPointCoordinate (float t, boost::numeric::ublas::vector<float> d, unsigned int index, unsigned int composant);
	
		///\brief create a temporary spline to know its length and so be able to discretise it
		///\return the length of the spline
		float splineLength(float step, boost::numeric::ublas::vector<float> vecDx, boost::numeric::ublas::vector<float> vecDy, boost::numeric::ublas::vector<float> vecDz);
		
		///\brief create the spline using the control points and the three delta vectors
		///\return nothing, the spline is stored in the attribute "splinePoints" of the instancied object
		void createSpline(float precision, float step, boost::numeric::ublas::vector<float> vecDx, boost::numeric::ublas::vector<float> vecDy, boost::numeric::ublas::vector<float> vecDz);
};

#endif
