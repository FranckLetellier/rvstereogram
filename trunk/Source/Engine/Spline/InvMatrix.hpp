//---------------------------------------------------------------------
//
// PROJECT ....
// Copyright (C) 
//
// All rights reserved
//
//---------------------------------------------------------------------


///\file
/// Matrix Inversion header
///\author Helena DUCHAUSSOY, Fabien KAPALA, Franck LETELLIER, Baptiste MALAGA


#ifndef MATRIX_H
#define MATRIX_H


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

///\brief Inverse a Matrix
///\param matrix : the matrix to inverse
///\return the inversed matrix 
boost::numeric::ublas::matrix<float> gaussJordan(const boost::numeric::ublas::matrix<float> matrix);
	
#endif
