#ifndef _STRUCTURES_H_INCLUDED_
#define _STRUCTURES_H_INCLUDED_

#include "parameters.h"

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

typedef struct{
	double t1;
	double t2;
	double t3;
} T_vec;

typedef struct{
	double x;
	double y;
} Vec2;

typedef struct{
	double p11;
	double p12;
	double p13;
	double p14;
	double p21;
	double p22;
	double p23;
	double p24;
	double p31;
	double p32;
	double p33;
	double p34;
} P_matrix;

typedef struct {
	double X_0;
	double Y_0;
	double Z_0;
	double omega;
	double phi;
	double kappa;
} Extrinsic_par;

typedef struct {
	double focus;
    double x_h;
	double y_h;
	double s_x;
	double theta;
	double k1;
	double k2;
	double p1;
	double p2;
} Intrinsic_par;

typedef struct {
	double alpha;
	double beta;
} Plane_par;

typedef struct {
	int    width;
    int    height;
	double pix_w;
	double pix_h;
} CCD_par;

typedef struct{
	double x;
	double y;
	double w;
} CamPoint;

typedef struct{
	int n_p;
	int x[MAX_PIXEL];
	int y[MAX_PIXEL];
	double hx[MAX_PIXEL];
	double hy[MAX_PIXEL];
	double hw[MAX_PIXEL];
} CalPoint;

typedef struct{
	double X;
	double Y;
	double Z;
	double W;
} GlobalPoint;

typedef struct{
	double    a;
	double    b;
	double    c;
} EpiPar;

#endif /* _STRUCTURES_H_INCLUDED_ */