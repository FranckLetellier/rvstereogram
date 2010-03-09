//---------------------------------------------------------------------
//
// PROJECT ....
// Copyright (C) 
//
// All rights reserved
//
//---------------------------------------------------------------------


///\file
/// Matrix Inversion operations
///\author Helena DUCHAUSSOY, Fabien KAPALA, Franck LETELLIER, Baptiste MALAGA


#include "InvMatrix.hpp"

boost::numeric::ublas::matrix<float> gaussJordan(const boost::numeric::ublas::matrix<float> matrix){
	
	int size = matrix.size1(); ///< A matrix to invert has the same number of rows and columns
	float max=0.0;
	int indice = 0, m = 0;

	boost::numeric::ublas::matrix<float> result(boost::numeric::ublas::identity_matrix<float>(size,size));
	boost::numeric::ublas::matrix<float> M(matrix);
	
	// See each row
	for(int l=0 ; l<size ; l++){ 

		// Finding the maximum pivot
		max = abs(M(l,l));
		indice = l;
		for(m=l+1 ; m<size ; m++){
			if(abs(M(m,l))>max) {
				max = fabs(M(m,l));
				indice = m;
			}
		}
	
		// indice contain the row of maximum pivot
		boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<float> > rowML (M,l);
		boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<float> > rowMInd (M,indice);
		boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<float> > rowRL (result,l);
		boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<float> > rowRInd (result,indice);
		
		// Swap the rows
		boost::numeric::ublas::row(M,l) = rowMInd;
		boost::numeric::ublas::row(M,indice) = rowML;
		boost::numeric::ublas::row(result,l) = rowRInd;
		boost::numeric::ublas::row(result,indice) = rowRL;
		
		// Normalize the row
		// Of M matrix
		max = M(l,l);
		
		boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<float> > aux (M,l);
		aux /= max;
		boost::numeric::ublas::row(M,l) = aux;

		// Of result matrix
		boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<float> > aux2 (result,l);
		aux2 /= max;
		boost::numeric::ublas::row(result,l) = aux2;
		
		for(int i=l+1 ; i<size ; i++){
	
			// Modifying the row i of M
			boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<float> > aux3 (M,l);
			boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<float> > aux4 (M,i);
			max = M(i,l);
			aux4 -= aux3*max;
			boost::numeric::ublas::row(M,i) = aux4;
					
			// Modifying the row i of result
			boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<float> > aux5 (result,l);
			boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<float> > aux6 (result,i);
			aux6 -= aux5*max;
			boost::numeric::ublas::row(result,i) = aux6;
		}
	} // By there, M is an up-diagonal matrix, and result is a dow-diagonal one

	
	for(int l=size-1 ; l>0 ; l--){
		boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<float> > aux (M,l);
		
		for(int i=0 ; i<l ; i++){
			// Modifying the row i of M
			max = M(i,l);
			aux = boost::numeric::ublas::row(M,l);
			boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<float> > aux2 (M,i);
			aux2 -= aux*max;
			boost::numeric::ublas::row(M,i) = aux2;
			
			// Modifying the row i of M
			aux = boost::numeric::ublas::row(result,l);
			aux2 = boost::numeric::ublas::row(result,i);
			aux2 -= aux*max;
			boost::numeric::ublas::row(result,i) = aux2;
		}
	
	}
	return result;
	
}

