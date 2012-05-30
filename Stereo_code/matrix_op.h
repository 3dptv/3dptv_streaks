#ifndef _MATRIX_OP_H_INCLUDED_
#define _MATRIX_OP_H_INCLUDED_

	typedef struct{    
		int           x;
		int           y;
		char        val;
	} Pixel;

	typedef struct{
		double x;
		double y;
		double w;
	} CamPoint;

	typedef struct{
		double t1;
		double t2;
		double t3;
	} T_vec;

	typedef struct{
		double x;
		double y;
		double z;
	} Vec3;

	typedef struct{
		double x;
		double y;
	} Vec2;

	typedef struct{
		double a11;
		double a12;
		double a13;
		double a21;
		double a22;
		double a23;
		double a31;
		double a32;
		double a33;
	} Matrix;


	Matrix mat_mult(Matrix, Matrix);
	//double* inverse(double*);
	Matrix transpose(Matrix);
	Matrix Get_r_x(double);
	Matrix Get_r_y(double);
	Matrix Get_r_z(double);
	T_vec vec_add(T_vec ,T_vec );
	T_vec vec_sub(T_vec ,T_vec );
	T_vec mat_vec_mult(Matrix, T_vec);
	double vec_abs(T_vec ,T_vec );
	double Det(Matrix);
	Matrix Neg(Matrix);
	//double* t_to_tx(T_vec);

#endif /* _MATRIX_OP_H_INCLUDED_ */