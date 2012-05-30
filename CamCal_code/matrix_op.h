#ifndef _MATRIX_OP_H_INCLUDED_
#define _MATRIX_OP_H_INCLUDED_

	Matrix trans(T_vec t);
	Matrix mat_mult(Matrix, Matrix);
	Matrix transpose(Matrix);
	T_vec vec_add(T_vec ,T_vec);
    T_vec vec_sub(T_vec,T_vec);
	T_vec mat_vec_mult(Matrix, T_vec);
	double Det(Matrix);
	Matrix Neg(Matrix);

#endif /* _MATRIX_OP_H_INCLUDED_ */