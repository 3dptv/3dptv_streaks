#include "stdafx.h"
#include <math.h>
#include "matrix_op.h"


Matrix mat_mult(Matrix A, Matrix B){
	Matrix C;

	C.a11=A.a11*B.a11+A.a12*B.a21+A.a13*B.a31;
	C.a12=A.a11*B.a12+A.a12*B.a22+A.a13*B.a32;
	C.a13=A.a11*B.a13+A.a12*B.a23+A.a13*B.a33;

	C.a21=A.a21*B.a11+A.a22*B.a21+A.a23*B.a31;
	C.a22=A.a21*B.a12+A.a22*B.a22+A.a23*B.a32;
	C.a23=A.a21*B.a13+A.a22*B.a23+A.a23*B.a33;

	C.a31=A.a31*B.a11+A.a32*B.a21+A.a33*B.a31;
	C.a32=A.a31*B.a12+A.a32*B.a22+A.a33*B.a32;
	C.a33=A.a31*B.a13+A.a32*B.a23+A.a33*B.a33;

	return C;
}


Matrix transpose(Matrix A){
	Matrix C;

	C.a11=A.a11;
	C.a12=A.a21;
	C.a13=A.a31;

	C.a21=A.a12;
	C.a22=A.a22;
	C.a23=A.a32;

	C.a31=A.a13;
	C.a32=A.a23;
	C.a33=A.a33;
	
	return C;
}

double Det(Matrix A){

	double d =   A.a11 * (A.a22 * A.a33 - A.a32 * A.a23) 
		       - A.a21 * (A.a12 * A.a33 - A.a32 * A.a13)
			   + A.a31 * (A.a12 * A.a23 - A.a22 * A.a13);

	return d;
}

Matrix Neg(Matrix A){

	A.a11= -A.a11;
	A.a12= -A.a12;
	A.a13= -A.a13;

	A.a21= -A.a21;
	A.a22= -A.a22;
	A.a23= -A.a23;

	A.a31= -A.a31;
	A.a32= -A.a32;
	A.a33= -A.a33;

	return A;
}



/*
double* inverse(double A[3][3]){
	double C[3][3];

	C.a11=1;
	C.a12=1;
	C.a13=1;

	C.a21=1;
	C.a22=1;
	C.a23=1;

	C.a31=1;
	C.a32=1;
	C.a33=1;
	
	return &C[0][0];

}
*/


Matrix Get_r_x(double alpha){
	Matrix C;


	C.a11=1.0;
	C.a12= 0;
	C.a13= 0;

	C.a21= 0;
	C.a22=cos(alpha);
	C.a23=sin(alpha);

	C.a31= 0;
	C.a32=-sin(alpha);
	C.a33= cos(alpha);	

	return C;
}

Matrix Get_r_y(double alpha){
	Matrix C;

	C.a11=cos(alpha);
	C.a12=0;
	C.a13=-sin(alpha);

	C.a21=0;
	C.a22=1.0;
	C.a23=0;

	C.a31=sin(alpha);
	C.a32=0;
	C.a33=cos(alpha);	
	return C;
}

Matrix Get_r_z(double alpha){
	Matrix C;


	C.a11=cos(alpha);
	C.a12=sin(alpha);
	C.a13=0;

	C.a21=-sin(alpha);
	C.a22=cos(alpha);
	C.a23=0;

	C.a31=0;
	C.a32=0;
	C.a33=1.0;	
	return C;
}


double vec_abs(T_vec a, T_vec b ){
	
	double res;

	res = sqrt((a.t1-b.t1)*(a.t1-b.t1)+(a.t2-b.t2)*(a.t2-b.t2)+(a.t3-b.t3)*(a.t3-b.t3));

	return res;
}

T_vec vec_add(T_vec a, T_vec b ){
	
    a.t1 = a.t1 + b.t1;
	a.t2 = a.t2 + b.t2;
	a.t3 = a.t3 + b.t3;

	return a;
}

T_vec vec_sub(T_vec a,T_vec b){
	
    a.t1 = a.t1 - b.t1;
	a.t2 = a.t2 - b.t2;
	a.t3 = a.t3 - b.t3;

	return a;
}

T_vec mat_vec_mult(Matrix A, T_vec t )
{
	T_vec c;

	c.t1 = A.a11*t.t1 + A.a12*t.t2 + A.a13*t.t3;
	c.t2 = A.a21*t.t1 + A.a22*t.t2 + A.a23*t.t3;
	c.t3 = A.a31*t.t1 + A.a32*t.t2 + A.a33*t.t3;

	return c;
}
/*
double* t_to_tx(T_vec t){
	double T[3][3];
	
	T.a11= 0;
	T.a12=-t.t3;
	T.a13= t.t2;
	T.a21= t.t3;
	T.a22= 0;
	T.a23=-t.t1;
	T.a31=-t.t2;
	T.a32= t.t1;
	T.a33= 0;

	return T;
}



 */


