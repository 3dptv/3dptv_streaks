#include "stdafx.h"
#include <math.h>
#include "nitypes.h"
#include "parameters.h"
#include "structures.h"
#include "matrix_op.h"


Matrix trans(T_vec t)
{
	Matrix M;

	M.a11 =  0.0;
	M.a12 = -t.t3;
	M.a13 =  t.t2;
	M.a21 =  t.t3;
	M.a22 =  0.0;
	M.a23 = -t.t1;
	M.a31 = -t.t2;
    M.a32 =  t.t1;
	M.a33 =  0.0;

	return M;
}

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


T_vec mat_vec_mult(Matrix A, T_vec t ){
	T_vec c;

	c.t1 = A.a11*t.t1 + A.a12*t.t2 + A.a13*t.t3;
	c.t2 = A.a21*t.t1 + A.a22*t.t2 + A.a23*t.t3;
	c.t3 = A.a31*t.t1 + A.a32*t.t2 + A.a33*t.t3;

	return c;
}