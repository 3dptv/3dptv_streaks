#ifndef _STRUCTURES_H_INCLUDED_
#define _STRUCTURES_H_INCLUDED_

#include "nitypes.h"
#include "parameters.h"


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


	typedef struct{
		double x;
		double y;
		double w;
	} CamPoint;

	typedef struct{    
		int           x;
		int           y;
		uInt8         val;
	} Pixel;

	typedef struct{
		double X;
		double Y;
		double Z;
		double W;
	} GlobalPoint;

#endif /* _STRUCTURES_H_INCLUDED_ */