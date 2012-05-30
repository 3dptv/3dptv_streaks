#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <imsl.h>
#include "parameters.h"
#include "matrix_op.h"
#include "path.h"
#include "spline.h"
#include "plot.h"


void DSpline::Get_measurement_p(Global G)
{
	//number of measurement points
	m=G.nr_p;

	GlobalPoint *n=node;
	GlobalPoint *g_p=G.point;

	for(int i=0;i<m;i++)
	{	
		parameter[i]=g_p->r;
		n->X = g_p->X;
		n->Y = g_p->Y;
		n->Z = g_p->Z;
		n++;
		g_p++;
	}
	//set the minimal and maximal parameter 
	// the last path element is NOT TAKEN!!!!!!!!!
	r_min=parameter[0];
	r_max=parameter[m-1]-last_element;
}



void DSpline::Get_ctrl_points()
{
	int i,ii,j;
	double *E    = new double[col*m];		//m is the number of measurement points 
	double *w    = new double[k+g+k+2];
	double *x    = new double[m]; 
	double *y    = new double[m]; 
	double *z    = new double[m]; 

	//set the control point weights
	for(i=0;i<=k+g+1+k;i++)
		w[i]=1;

	for(j=0;j<m;j++)
	{	
		for(i=-k;i<=g;i++)
		{
			ii=i+k;
			E[col*j+ii] = N(parameter[j],   ii, 3);
		}
		x[j] = node[j].X;
		y[j] = node[j].Y;
		z[j] = node[j].Z;
	}

	double *result_x = imsl_d_lin_least_squares_gen (m, col, E, x, 0);
	double *result_y = imsl_d_lin_least_squares_gen (m, col, E, y, 0);
	double *result_z = imsl_d_lin_least_squares_gen (m, col, E, z, 0);
	for(j=0;j<=k+g;j++)
	{
		cx[j]=*result_x++;
		cy[j]=*result_y++;
		cz[j]=*result_z++;
	}

	delete[] E;
	delete[] w;
	delete[] x;
	delete[] y;
	delete[] z;
}


extern FILE *f_spline_global;

void DSpline::Get_spline(int spline_nr)
{
	//get the arc lenght s, the curvature curv, the torsion wind.
	//the direction of the tangent vector t_vec, the normal vector n_vec and the binormal vector b_vec
	//the so called moving trihedron. 
	//or Frenet Frame
	//Info can be found at http://mathworld.wolfram.com/Curvature.html
	//int j=0;
	double X,Y,Z;
	double s_length, curvature, torsion;
	Vec3   t_vec,    n_vec,     b_vec;
	int    i_seg=(int)r_min;

	int nr=(r_max-r_min) / 0.05;			//number of data points
	if(nr>0)
	{
		double *ux = new double[nr];			//the data points
		double *uy = new double[nr];			//the data points
		double *uz = new double[nr];			//the data points

		for(double r=r_min;r<r_max;r+=0.05)
		{
			if(r>=i_seg && r<i_seg+1)
			{
				s_length=Integ_gauss(i_seg,i_seg+1);
				i_seg++;
			}

			Get_trihedron(r, &curvature, &torsion, &t_vec, &n_vec, &b_vec);

			X=0;
			Y=0;
			Z=0;
			for(int i=0;i <= g+k;i++)
			{
				X+=cx[i]*N(r,i,k);
				Y+=cy[i]*N(r,i,k);
				Z+=cz[i]*N(r,i,k);
			}
			fprintf(f_spline_global,"%4i %4i %10.2f %10.2f %10.2f %10.2f %10.5f %10.5f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f%10.4f %10.4f %10.4f\n",
														   spline_nr,i_seg-1,
														   X,Y,Z,
														   s_length,curvature,torsion,
														   t_vec.x, t_vec.y, t_vec.z, 
														   n_vec.x, n_vec.y, n_vec.z,
														   b_vec.x, b_vec.y, b_vec.z);
		}
	}
}


void DSpline::Get_derivative_coeff()
{
//******************************************************************************************
//  The calculation of the knots of the first derivative of a third order spline
//  See Dierckx p.14
//  The function uses the knot values a third order spline
//  So  Get_ctrl_points() must be called first!
//******************************************************************************************
	for(int i=1;i<=g+k;i++)
	{
		dcx[i] = (cx[i]-cx[i-1])/(lambda[i+k]-lambda[i]);
		dcy[i] = (cy[i]-cy[i-1])/(lambda[i+k]-lambda[i]);
		dcz[i] = (cz[i]-cz[i-1])/(lambda[i+k]-lambda[i]);
	}
}

void DSpline::Get_dderivative_coeff()
{
//******************************************************************************************
//  The calculation of the knots of the second derivative of a third order spline
//  See Dierckx p.14
//  The function uses the knot values from the first derivative of a third order spline
//  So  Get_derivative_coeff() must be called first!
//******************************************************************************************
	for(int i=2;i<=g+k;i++)
	{
		ddcx[i] = (dcx[i]-dcx[i-1])/(lambda[i+k-1]-lambda[i]);
		ddcy[i] = (dcy[i]-dcy[i-1])/(lambda[i+k-1]-lambda[i]);
		ddcz[i] = (dcz[i]-dcz[i-1])/(lambda[i+k-1]-lambda[i]);
	}
}

void DSpline::Get_ddderivative_coeff()
{
//******************************************************************************************
//  The calculation of the knots of the second derivative of a third order spline
//  See Dierckx p.14
//  The function uses the knot values from the first derivative of a third order spline
//  So  Get_derivative_coeff() must be called first!
//******************************************************************************************
	for(int i=3;i<=g+k;i++)
	{
		dddcx[i] = (ddcx[i]-ddcx[i-1])/(lambda[i+k-2]-lambda[i]);
		dddcy[i] = (ddcy[i]-ddcy[i-1])/(lambda[i+k-2]-lambda[i]);
		dddcz[i] = (ddcz[i]-ddcz[i-1])/(lambda[i+k-2]-lambda[i]);
	}
}


double  DSpline::Get_z_derivative(double r)
{
	double dz=0;
//	for(int i=-1+k;i <= g+k;i++)
	for(int i=1;i <= g+k;i++)
		dz+=dcz[i]*N(r,i,k-1);

	dz *= k;
	return dz;
}

double DSpline::Get_z_dderivative(double r)
{
	double ddz=0;
//	for(int i=k;i <= g+k;i++)
	for(int i=2;i <= g+k;i++)
		ddz+=ddcz[i]*N(r,i,k-2);

	ddz *= (k*(k-1));
	return ddz;
}

double DSpline::Get_z_ddderivative(double r)
{
	double dddz=0;
	for(int i=3;i <= g+k;i++)
		dddz+=dddcz[i]*N(r,i,k-3);

	dddz *= (k*(k-1)*(k-2));
	return dddz;
}

double  DSpline::Integ_gauss(double a,double b)
{
//*********************************************************************
//		Gauss Integration
//		See Numerical Recepies in C Second Edition
//		Page 147 and 148
//*********************************************************************
	static const double x[]={ 0.0,
						0.1488743389,
						0.4333953941,
						0.6794095683,
						0.8650633666,
						0.9739065285};
	static const double w[]={ 0.0,
						0.2955242247,
						0.2692667193,
						0.2190863625,
						0.1494513491,
						0.0666713443};

	double xm=0.5*(b+a);
	double xr=0.5*(b-a);
	double I=0;
	double dx;
	for(int i=0;i<=5;i++)
	{
		dx=xr*x[i];
		I += w[i] * (arc_fct(xm+dx)+arc_fct(xm-dx));
	}
	return I*=xr;
}

double  DSpline::Get_trihedron(double r, double *curvature, double *torsion, Vec3 *t_vec, Vec3 *n_vec , Vec3 *b_vec)
{
	//******************************************************************************
	//
	//calculates the tangent t_vec=(ux,uy,uz),
	//the direction n_vec=(vx,vy,vz) of the radius (normal vector),
	//the binormal vector b_vec=(vx,vy,vz) of the radius (normal vector),
	//the curvature (Kruemmung) k=|v|,
	//the radius r=1/k,
	//and the torsion w (Windung)
	//of a third order spline depending on the parameter r.
	//see Mathematische Hilfsmittel des Ingenieurs, Teil 3 von  R.Sauer + I.Szabo 
	//******************************************************************************
	// or better Bronstein s596--598

	//Test data. curvature=0.0689 and torsion    =0.1724
/*	double dx= -2*sin(r);
	double dy=  2*cos(r);
	double dz=  5;
	double ddx=-2*cos(r);
	double ddy=-2*sin(r);
	double ddz= 0;
	double dddx= 2*sin(r);
	double dddy=-2*cos(r);
	double dddz= 0;
*/
	double dx=	Get_x_derivative(r);
	double dy=  Get_y_derivative(r);
	double dz=  Get_z_derivative(r);
	double ddx=	Get_x_dderivative(r);
	double ddy=	Get_y_dderivative(r);
	double ddz=	Get_z_dderivative(r);
	double dddx=Get_x_ddderivative(r);
	double dddy=Get_y_ddderivative(r);
	double dddz=Get_z_ddderivative(r);
	double nenner=(dx*dx+dy*dy+dz*dz)*(dx*dx+dy*dy+dz*dz)*(dx*dx+dy*dy+dz*dz);

	if(nenner !=0)
	{
		*curvature=sqrt( ((dx*dx+dy*dy+dz*dz)*(ddx*ddx+ddy*ddy+ddz*ddz)-
						   (dx*ddx+dy*ddy+dz*ddz)*(dx*ddx+dy*ddy+dz*ddz))/nenner);
		if(*curvature != 0)
		{
			double rho =1 / *curvature;
			*torsion=rho*rho*(dx*(ddy*dddz-ddz*dddy)+ddx*(dddy*dz-dy*dddz)+dddx*(dy*ddz-ddy*dz))/nenner;
		}

		//the unit tangent to the curve ( u=(dx/ds, dy/ds, dz/ds))
		double l=sqrt(dx*dx+dy*dy+dz*dz);
		if(l!=0)
		{
			t_vec->x=dx/l;
			t_vec->y=dy/l;
			t_vec->z=dz/l;
		}
		//the unit normal (Hauptnormalenvector t) dr x(dr x ddr)
		double nx=dx*dy*ddy-dy*dy*ddx-dz*dz*ddx+dz*ddz*dx;
		double ny=dy*dz*ddz-dz*dz*ddy-dx*dx*ddy+dx*ddx*dy;
		double nz=dz*dx*ddx-dx*dx*ddz-dy*dy*ddz+dy*ddy*dz;
		l=sqrt(nx*nx+ny*ny+nz*nz);
		if(l!=0)
		{
			n_vec->x=-nx/l;
			n_vec->y=-ny/l;
			n_vec->z=-nz/l;
		}
		//get the unit direction of the bi-normal vector dr x ddr
		double bx=dy*ddz-ddy*dz;
		double by=dz*ddx-ddz*dx;
		double bz=dx*ddy-ddx*dy;
		l=sqrt(bx*bx+by*by+bz*bz);
		if(l!=0)
		{
			b_vec->x=bx/l;
			b_vec->y=by/l;
			b_vec->z=bz/l;
		}
	}
	return 0;
}

double DSpline::Integ_simpson(double a,double b)
{
	//the number of intervalls
	const int n=200;
	//intervall
	const double h=(b-a)/(2*n);
	double x;
	double I=0;

	for(int i=1;i<2*n;i++)
	{
		x=i*h+a;
		if(i%2)
			I+=arc_fct(x)*4;
		else
			I+=arc_fct(x)*2;
	}
	I+=(arc_fct(a)+arc_fct(b));
	return I*h/3;
}

double DSpline::Integ_trapez(double a,double b)
{
	//There is an error here!!! (factor 2)

	//the number of intervalls
	const int n=200;
	//intervall
	const double h=(b-a)/n;
	double x;
	double I=0;

	for(int i=1;i<n;i++)
	{
		x=i*h+a;
		I+=arc_fct(x)*h;
	}
	I+=0.5*(arc_fct(a)+arc_fct(b));
	return I;
}



double DSpline::arc_fct(double s)
{
//******************************************************************************
//the integrand of the arc length Integral 		                ****************
//																****************
//******************************************************************************
	double dx= Get_x_derivative(s);
	double dy= Get_y_derivative(s);
	double dz= Get_z_derivative(s);

	return sqrt(dx*dx+dy*dy+dz*dz);
}


double DSpline::arc_d_fct(double s)
{
	double dx= Get_x_derivative(s);
	double dy= Get_y_derivative(s);
	double dz= Get_z_derivative(s);

	double ddx= Get_x_dderivative(s);
	double ddy= Get_y_dderivative(s);
	double ddz= Get_z_dderivative(s);

	return (pow((dx*dx+dy*dy+dz*dz),-0.5) * (ddx+ddy+ddz));
}