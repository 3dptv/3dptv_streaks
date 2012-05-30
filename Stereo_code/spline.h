#ifndef _SPLINE_H_INCLUDED_
#define _SPLINE_H_INCLUDED_

//*
//*
//*  This is the basic definition of the class spline. It contains 
//*  the basic polynoms N_k3, N_k2,  N_k1 the parameters describing 
//*  the spline k, g, col, r_max, the number of points m used to calculate 
//*  the spline and the routines to calculate the control points cx, cy
//*  Get_ctrl_points(), to get the coordinates x,y of a spline point 
//*  at a parameter r and the derivative of the spline at a location r
//*********************************************************************************
    const int MAX_M=2000;
	class Spline_Basis{
		public:
			Spline_Basis(){k=0;g=0;col=0;m=0;r_max=0;}
			//~Spline_Basis();
			int     Num_measurements();
			double  Max_parameter();
			int 	Generate_knots(double);
			void	Get_derivative_coeff();
			void	Get_point(CamPoint*, double r);
			void	Get_spline(CamPoint*, double);
			void	Get_derivative(CamPoint*, double);
			void	Get_derivative(double, double*, double*);
			double	Get_x_derivative(double r);
			double	Get_y_derivative(double r);
			double	Get_x_dderivative(double r);
			double	Get_y_dderivative(double r);
			double	Get_x_ddderivative(double r);
			double	Get_y_ddderivative(double r);
			double  parameter[2500];			//the parameters with measurement values
			double  lambda[200];				//the knots of the spline
			int     g;							//number of 0,...,g+1 knots
		protected:
			double  cx[200],cy[200];			//the  B-spline coefficients
			double  dcx[200],dcy[200];			//the  derivative B-spline coefficients
			double  ddcx[200],ddcy[200];		//the  second derivative B-spline coefficients
			double  dddcx[200],dddcy[200];		//the  second derivative B-spline coefficients
			int     k;							//degree of the base function
			int		m;							//number of measurement points 
			double  r_max;						//the parameter range is [0..r_max]
			int     col;       
			double  N(double t, int i, int);
	};
//*********************************************************************************
//*********************************************************************************
//*********************************************************************************
//*********************************************************************************


	class Spline: public Spline_Basis{
		public:
			bool Init_node(Path*, int view);
			//void Init_hnode(Path*);
			bool Init_hnode_all(Path *path);
			void Get_ctrl_points();
			void Plot_ctrl_points(uInt8*);
			void Plot_node_points(uInt8*);
			int  Get_new_hnode(Path*, double, double);
			void Get_approx_intersection(double, double, double, double*,double*, int*);
			int  Line_intersect(double, double, double, CamPoint*, double*, double);
			CamPoint	 node[2500];			//the measurement points 1..m
	};
//*********************************************************************************
//*********************************************************************************
//*********************************************************************************
//*********************************************************************************
	#include "global.h"

	class DSpline: public Spline_Basis{
		public:
			void		 Get_measurement_p(Global);
			void		 Get_ctrl_points();
			void		 Get_spline(int spline_nr);
			void	     Get_derivative_coeff();
			void		 Get_dderivative_coeff();
			void		 Get_ddderivative_coeff();
			double		 Get_z_derivative(double r);
			double		 Get_z_dderivative(double r);
			double		 Get_z_ddderivative(double r);
			GlobalPoint	 node[800];				//the measurement points 1..m
			int			 last_element;
			double		 parameter[800];		//the parameters with measurement values
		private:		
			double		 Get_trihedron(double r, double *curv, double *wind, Vec3 *t, Vec3 *n, Vec3 *b);
			double		 Integ_gauss(double a,double b);
			double		 Integ_simpson(double a,double b);
			double		 Integ_trapez(double a,double b);
			double		 arc_fct(double s);
			double		 arc_d_fct(double s);
			double       r_min;
			double		 cz[200];					//the  B-spline coefficients
			double		 dcz[200];					//the  derivative B-spline coefficients
			double		 ddcz[200];					//the  second derivative B-spline coefficients
			double		 dddcz[200];				//the  second derivative B-spline coefficients
	};
	
#endif /* _SPLINE_H_INCLUDED_ */