//---------------------------------------------------------------------
//
// PROJECT ....
// Copyright (C) 
//
// All rights reserved
//
//---------------------------------------------------------------------


/// Spline 
///\author Helena DUCHAUSSOY, Fabien KAPALA, Franck LETELLIER, Baptiste MALAGA


#include "Spline.hpp"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

Spline::Spline(const char * filename)
{
		
	controlPoints = loadXMLFile(filename);
	
	int size = controlPoints.size();
	boost::numeric::ublas::matrix<float> leftM = fillLeftMatrix(size);
	boost::numeric::ublas::vector<float> vectorDx = initDeltaVector(size);
	boost::numeric::ublas::vector<float> vectorDy = initDeltaVector(size);
	boost::numeric::ublas::vector<float> vectorDz = initDeltaVector(size);
	boost::numeric::ublas::vector<float> vectorBx = fillRightVector(controlPoints,0);
	boost::numeric::ublas::vector<float> vectorBy = fillRightVector(controlPoints,1);
	boost::numeric::ublas::vector<float> vectorBz = fillRightVector(controlPoints,2);
	  
    boost::numeric::ublas::matrix<float> inverseLeftM = gaussJordan(leftM);
    vectorDx = prod(inverseLeftM,vectorBx);
    vectorDy = prod(inverseLeftM,vectorBy);
    vectorDz = prod(inverseLeftM,vectorBz);
  
    for(unsigned int i=0 ; i<vectorDx.size() ; i++){
    	if(abs(vectorDx[i])<0.001) vectorDx[i]=0;
    	if(abs(vectorDy[i])<0.001) vectorDy[i]=0;
    	if(abs(vectorDz[i])<0.001) vectorDz[i]=0;
    }
    
    float sLength = splineLength(0.1, vectorDx, vectorDy, vectorDz);
  	float precision = sLength/500;

	createSpline(0.001, precision, vectorDx, vectorDy, vectorDz);
		
}

Spline::Spline(const char * filename, float precision)
{
		
	controlPoints = loadXMLFile(filename);
	
	int size = controlPoints.size();
	boost::numeric::ublas::matrix<float> leftM = fillLeftMatrix(size);
	boost::numeric::ublas::vector<float> vectorDx = initDeltaVector(size);
	boost::numeric::ublas::vector<float> vectorDy = initDeltaVector(size);
	boost::numeric::ublas::vector<float> vectorDz = initDeltaVector(size);
	boost::numeric::ublas::vector<float> vectorBx = fillRightVector(controlPoints,0);
	boost::numeric::ublas::vector<float> vectorBy = fillRightVector(controlPoints,1);
	boost::numeric::ublas::vector<float> vectorBz = fillRightVector(controlPoints,2);
	  
    boost::numeric::ublas::matrix<float> inverseLeftM = gaussJordan(leftM);
    vectorDx = prod(inverseLeftM,vectorBx);
    vectorDy = prod(inverseLeftM,vectorBy);
    vectorDz = prod(inverseLeftM,vectorBz);
  
    for(unsigned int i=0 ; i<vectorDx.size() ; i++){
    	if(abs(vectorDx[i])<0.001) vectorDx[i]=0;
    	if(abs(vectorDy[i])<0.001) vectorDy[i]=0;
    	if(abs(vectorDz[i])<0.001) vectorDz[i]=0;
    }
    
	createSpline(0.001, precision, vectorDx, vectorDy, vectorDz);
		
}

Spline::~Spline()
{

}

boost::numeric::ublas::matrix<float> Spline::fillLeftMatrix(int size)
{
	
	boost::numeric::ublas::matrix<float> leftMatrix(size,size);
	int i,j;
	
	for(i=0 ; i<size ; i++){
		for(j=0 ; j<size ; j++){
			if(i==j){
				leftMatrix.insert_element(i,j,4);
			}
			else if(i==size-1 && j==0){
				leftMatrix.insert_element(i,j,1);
			}
			else if(i==0 && j==size-1){
				leftMatrix.insert_element(i,j,1);
			}
			else if(j==i+1 || j==i-1){
				leftMatrix.insert_element(i,j,1);
			}
			else{
				leftMatrix.insert_element(i,j,0);
			}
		}
	}
	return leftMatrix;
}

boost::numeric::ublas::vector<float> Spline::initDeltaVector(int rows)
{
	return boost::numeric::ublas::zero_vector<float>(rows);
}

boost::numeric::ublas::vector<float> Spline::fillRightVector(const std::vector<osg::Vec3f> v, int composant)
{
	
	int size = v.size();
	boost::numeric::ublas::vector<float> result(size);
	
	for(int i=0 ; i<size ; i++){
		if(i!=0 && i!= size-1){
			result.insert_element(i , 3*(v.at(i+1)[composant]-v.at(i-1)[composant]));
		}
		if(i==0){
			result.insert_element(i , 3*(v.at(i+1)[composant]-v.at(size-1)[composant]));
		}
		if(i==size-1){
			result.insert_element(i , 3*(v.at(0)[composant]-v.at(i-1)[composant]));
		}
	}
	return result;
}

float Spline::getPointCoordinate (float t, boost::numeric::ublas::vector<float> vecD, unsigned int index, unsigned int composant)
{
	float a = controlPoints.at(index)[composant];
	float b = vecD(index);
	float c;
	float d;
	
	if(index != controlPoints.size()-1){
		c = 3*(controlPoints.at(index+1)[composant] - controlPoints.at(index)[composant]) - 2*vecD(index) - vecD(index+1);
		d = 2*(controlPoints.at(index)[composant] - controlPoints.at(index+1)[composant]) + vecD(index) + vecD(index+1);
	}
	else if(index == controlPoints.size()-1){
		c = 3*(controlPoints.at(0)[composant] - controlPoints.at(index)[composant]) - 2*vecD(index) - vecD(0);
		d = 2*(controlPoints.at(index)[composant] - controlPoints.at(0)[composant]) + vecD(index) + vecD(0);
	}

	return (a+(b*t)+(c*t*t)+(d*pow(t,3))); 	
}

float Spline::splineLength(float step, boost::numeric::ublas::vector<float> vecDx, boost::numeric::ublas::vector<float> vecDy, boost::numeric::ublas::vector<float> vecDz)
{
	
	float t=0;
	unsigned int size = controlPoints.size();
	unsigned int l=0;
	float splineLength = 0;
	osg::Vec3f newPoint, exPoint;
	
	while(l < size){
		
		while(t < 1){
			
			newPoint[0] = getPointCoordinate(t, vecDx, l, 0);
			newPoint[1] = getPointCoordinate(t, vecDy, l, 1);
			newPoint[2] = getPointCoordinate(t, vecDz, l, 2);

			//tmpVector.push_back(newPoint);
			if(t != 0 || l != 0){ 
				// Not the very first point of the Spline so we can calculate the distance 
				// between the previous point and the current one
				float tmp =  pow(newPoint.x()-exPoint.x(),2)+pow(newPoint.y()-exPoint.y(),2)+pow(newPoint.z()-exPoint.z(),2);
				splineLength += pow (tmp,static_cast<float>(0.5));
			}
			exPoint = newPoint;
			t+=step;
		}
		t=0;
		l++;
	}
	return splineLength;
}

void Spline::createSpline(float precision, float step, boost::numeric::ublas::vector<float> vecDx, boost::numeric::ublas::vector<float> vecDy, boost::numeric::ublas::vector<float> vecDz)
{
	
	float t=0;
	float dist = 0;
	unsigned int size = controlPoints.size();
	unsigned int l=0;
	osg::Vec3f newPoint, exPoint;
	
	while(l<size){
		
		exPoint[0] = controlPoints[l][0];
		exPoint[1] = controlPoints[l][1];
		exPoint[2] = controlPoints[l][2];

			while(t<1){
					
				t+=precision;
				newPoint[0] = getPointCoordinate(t, vecDx, l, 0);
				newPoint[1] = getPointCoordinate(t, vecDy, l, 1);
				newPoint[2] = getPointCoordinate(t, vecDz, l, 2);
				dist = pow( pow(newPoint.x()-exPoint.x(),2) + pow(newPoint.y()-exPoint.y(),2) + pow(newPoint.z()-exPoint.z(),2),static_cast<float>(0.5));
				
				if(dist>step){
					splinePoints.push_back(newPoint);
					exPoint = newPoint;
				}
				
			}
		t=0;
		l++;
	}

}

void Spline::draw()
{
	osg::Vec3f point;
	
	glBegin(GL_LINES);
  		point = getSplinePointAt(0);
  		glVertex3f(point.x(),point.y(),point.z());

		for(unsigned int i=1 ; i<getSplineSize() ; ++i)
		{
  			point = getSplinePointAt(i);
  			glVertex3f(point.x(),point.y(),point.z());
  			glVertex3f(point.x(),point.y(),point.z());
		}

		point = getSplinePointAt(0);
  		glVertex3f(point.x(),point.y(),point.z());
	glEnd();

	for(unsigned int i=0 ; i<getControlSize() ; ++i)
	{
  		point = getControlPointAt(i);
  		glPushMatrix();
  			glTranslatef(point.x(),point.y(),point.z());
  			glutSolidSphere(0.1f,8.0,8.0);
  		glPopMatrix();
	}
	
}

