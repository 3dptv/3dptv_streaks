#include "stdafx.h"
#include <math.h>
#include <imsl.h>
#include <windows.h>
#include "parameters.h"
#include "matrix_op.h"
#include "path.h"
#include "spline.h"
#include "plot.h"
#include "global.h"

extern CDC     *device;
extern uInt8   colimagedata1[];
extern uInt8   colimagedata2[];

/*
Spline_Basis::~Spline_Basis()
{

}
*/

int Spline_Basis::Num_measurements()
{
	return m;
}

double Spline_Basis::Max_parameter()
{
	return r_max;
}


int Spline_Basis::Generate_knots(double knots_spacing)
{
	k   = 3;										//degree of B-spline
	g   = (int)(ceil(r_max/knots_spacing))-1;		//number of knots  0,...,g+1
	col = k+g+1;

	if((k+g+k+1) >200)
		return 1;
	else if(m<col)
		return 2;

	//construct the knot points:  0,0,0,2,4,6, .... ,m,m,m
	//the measurement points are set at 0,1,2,3,...,g
	for(int i=0;i<=k;i++)            //this are the knots -k,..-1,0
		lambda[i]=0;
	for(i=k+1;i< k+g+1;i++)          //this are the knots  1,..,g
		lambda[i]=lambda[k-1]+knots_spacing*(i-k);
	for(i=k+g+1;i <= k+g+k+1;i++)      //this are the knots  g+1,..,g+k+1
		lambda[i]=r_max+0.001;
		//lambda[i]= lambda[k+g]+knots_spacing;  

	return 0;
}


/*
void Spline::Init_hnode(Path *path)
{
	//this routine initializes the measurement points (nodes)
	//with HOMOGENEOUS coordinates
	//the number of nodes: m
	//and the parameter r
	Path_seg  *path_seg = path->path_seg;
	int i=0;
	m=0;

	//initialize nodes with starting points
	CamPoint *n=node;

	if(n->x = path_seg->end_point_index[1] != -1)
	{
		parameter[i++]=0;
		n->x = path_seg->hpixel[path_seg->end_point_index[1]].x;
		n->y = path_seg->hpixel[path_seg->end_point_index[1]].y;
		n++;
		m++;
	}
	//the last segment is not taken
	while(path_seg < path->path_seg+path->nr_seg-1)
	{
		if(n->x = path_seg->end_point[path_seg->end_point_index[0]].x != -1)
		{
			parameter[i]=i;
			i++;
			n->x = path_seg->hpixel[path_seg->end_point_index[0]].x;
			n->y = path_seg->hpixel[path_seg->end_point_index[0]].y;

			path_seg++;
			n++;
			m++;
		}
	}
	r_max=(double)path->nr_seg-1;
}
*/

bool Spline::Init_hnode_all(Path *path)
{
	//this routine initializes the measurement points (nodes)
	//the number of nodes: m
	//and the parameter r
	//this routine is used just for a check on the 
	//display 
	Path_seg  *path_seg = path->path_seg;
	int i;
	int seg_nr=0;
	int step,step_tot;
	int pixel_nr;
	double delta;
	CamPoint *n=node;


	while(path_seg < path->path_seg+path->nr_seg-1)
	{
		int start_index = path_seg->end_point_index[1];
		int end_index   = path_seg->end_point_index[0];

		pixel_nr=path_seg->p_nr;
		if(start_index < end_index)
		{
			//first side
			delta=1.0/(double)(end_index-start_index);
			step=0;
			for(i=start_index;i<end_index;i++)
			{
				n->x = path_seg->hpixel[i].x;
				n->y = path_seg->hpixel[i].y;
				parameter[m]=seg_nr+step*delta;
				n++;
				m++;
				step++;
				if(m>=2500)
					return FALSE;
			}
			//second side
			delta=1.0/(double)(start_index+(pixel_nr-end_index));
			step_tot=step;
			step=0;
			for(i=start_index-1; i>=0; i--)
			{
				n->x = path_seg->hpixel[i].x;
				n->y = path_seg->hpixel[i].y;
				parameter[m]=seg_nr+step*delta;
				n++;
				m++;
				step++;
				if(m>=2500)
					return FALSE;
			}
			for(i=pixel_nr-1; i >=end_index; i--)
			{
				n->x = path_seg->hpixel[i].x;
				n->y = path_seg->hpixel[i].y;
				parameter[m]=seg_nr+step*delta;
				n++;
				m++;
				step++;
				if(m>=2500)
					return FALSE;
			}
		}
		else
		{
			//first side
			step=0;
			delta=1.0/(double)(start_index-end_index);
			for(i=start_index-1; i>=0; i--)
			{
				n->x = path_seg->hpixel[i].x;
				n->y = path_seg->hpixel[i].y;
				parameter[m]=seg_nr+step*delta;
				n++;
				m++;
				step++;
				if(m>=2500)
					return FALSE;
			}
			//second side
			delta=1.0/(double)(end_index+(pixel_nr-start_index));
			step_tot=step;
			step=0;
			for(i=start_index;i<pixel_nr;i++)
			{
				n->x = path_seg->hpixel[i].x;
				n->y = path_seg->hpixel[i].y;
				parameter[m]=seg_nr+step*delta;
				n++;
				m++;
				step++;
				if(m>=2500)
					return FALSE;
			}
			for(i=0; i<end_index; i++)
			{
				n->x = path_seg->hpixel[i].x;
				n->y = path_seg->hpixel[i].y;
				parameter[m]=seg_nr+step*delta;
				n++;
				m++;
				step++;
				if(m>=2500)
					return FALSE;
			}
		}
		path_seg++;
		seg_nr++;
	}
	r_max=(double)path->nr_seg-1;
	return TRUE;
}




void Spline::Get_ctrl_points()
{
	int i,ii,j;
	//m is the number of measurement points 
	//col = k+g+1;
	double *E    = new double[col*m];
	double *w    = new double[k+g+k+2];
	double *x    = new double[m]; 
	double *y    = new double[m]; 


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
		x[j] = node[j].x;
		y[j] = node[j].y;
	}

	double *result_x = imsl_d_lin_least_squares_gen (m, col, E, x, 0);
	for(j=0;j<=k+g;j++)
		cx[j]=*result_x++;

	double *result_y = imsl_d_lin_least_squares_gen (m, col, E, y, 0);
	for(j=0;j<=k+g;j++)
		cy[j]=*result_y++;

	delete[] E;
	delete[] w;
	delete[] x;

	delete[] y;
}


void Spline::Plot_ctrl_points(uInt8 *image)
{
	CamPoint p[200];
	for(int i=0;i <= g+k ;i++)
	{
		p[i].x=cx[i];
		p[i].y=cy[i];
	}
//	Camera::Transform_points()
	Plot::Points(device,image ,p, g+k, 0, 0, 0);
}


void Spline::Plot_node_points(uInt8 *image)
{
	CamPoint *p =new CamPoint[m];
	for(int i=0;i < m ;i++)
	{
		p[i].x=node[i].x;
		p[i].y=node[i].y;
	}
//	Camera::Transform_points()
	Plot::Pixels(device,image ,p, m, 0, 0, 255);
	delete [] p;
}

void Spline_Basis::Get_spline(CamPoint *p, double t_max)
{
	int i;

	for(double r=0;r<t_max;r+=0.05)
	{
		p->x=0;
		p->y=0;
		for(i=0;i <= g+k;i++)
		{
			p->x+=cx[i]*N(r,i,k);
			p->y+=cy[i]*N(r,i,k);
		}
		p++;
	}
}


void Spline_Basis::Get_point(CamPoint *p, double r)
{
	p->x=0;
	p->y=0;
	for(int i=0;i <= g+k;i++)
	{
		p->x+=cx[i]*N(r,i,k);
		p->y+=cy[i]*N(r,i,k);
	}
}


void Spline_Basis::Get_derivative_coeff()
{
	for(int i=1;i<=g+k;i++)
	{
		dcx[i] = (cx[i]-cx[i-1])/(lambda[i+k]-lambda[i]);
		dcy[i] = (cy[i]-cy[i-1])/(lambda[i+k]-lambda[i]);
	}
}


void Spline_Basis::Get_derivative(CamPoint *p, double t_max)
{
	int i;

	for(double r=0;r<t_max;r+=0.05)
	{
		p->x=0;
		p->y=0;
//		for(i=-1+k;i <= g+k;i++)
		for(i=1;i <= g+k;i++)
		{
			p->x+=dcx[i]*N(r,i,k-1);
			p->y+=dcy[i]*N(r,i,k-1);
		}
		p->x *= k;
		p->y *= k;
		p++;
	}
}



void Spline_Basis::Get_derivative(double r, double *dx, double *dy)
{
//******************************************************************************
//calculation of dx/dr and dy/dr of a third order spline at r.  ****************
//See Dierckx p. 14												****************
//******************************************************************************
	int i;

	*dx=0;
	*dy=0;
	//for(i=-1+k;i <= g+k;i++)
	for(i=1;i <= g+k;i++)
	{
		*dx+=dcx[i]*N(r,i,k-1);
		*dy+=dcy[i]*N(r,i,k-1);
	}
	*dx *= k;
	*dy *= k;
}

//******************************************************************************
//calculation of d /dr of a third order spline at r.			****************
//See Dierckx p. 14												****************
//******************************************************************************
double  Spline_Basis::Get_x_derivative(double r)
{
	double dx=0;
//	for(int i=-1+k;i <= g+k;i++)
	for(int i=1;i <= g+k;i++)
		dx+=dcx[i]*N(r,i,k-1);

	dx *= k;
	return dx;
}

double  Spline_Basis::Get_y_derivative(double r)
{
	double dy=0;
	for(int i=1;i <= g+k;i++)
		dy+=dcy[i]*N(r,i,k-1);

	dy *= k;
	return dy;
}

//******************************************************************************
//calculation of d^2 /dr^2 of a third order spline at r.      	****************
//See Dierckx p. 14												****************
//******************************************************************************
double Spline_Basis::Get_x_dderivative(double r)
{
	double ddx=0;
	for(int i=2;i <= g+k;i++)
		ddx+=ddcx[i]*N(r,i,k-2);

	ddx *= (k*(k-1));
	return ddx;
}

double Spline_Basis::Get_y_dderivative(double r)
{
	double ddy=0;
	for(int i=2;i <= g+k;i++)
		ddy+=ddcy[i]*N(r,i,k-2);

	ddy *= (k*(k-1));
	return ddy;
}

//******************************************************************************
//calculation of d^3 /dr^3 of a third order spline at r.      	****************
//See Dierckx p. 14												****************
//******************************************************************************
double Spline_Basis::Get_x_ddderivative(double r)
{
	double dddx=0;
	for(int i=3;i <= g+k;i++)
		dddx+=dddcx[i]*N(r,i,k-3);

	dddx *= (k*(k-1)*(k-2));
	return dddx;
}

double Spline_Basis::Get_y_ddderivative(double r)
{
	double dddy=0;
	for(int i=3;i <= g+k;i++)
		dddy+=dddcy[i]*N(r,i,k-3);

	dddy *= (k*(k-1)*(k-2));
	return dddy;
}

//**********************************************************************************
//The normalized B-splines of degree 0, 1,2 and 3 (the order is (k+1)***************
//for a arbitrary knot spacing										****************
//calculated with the recursion formula of de Boor 1972;     		****************
//See Dierckx p.8													****************			
//Ni_xy means: the B-spline defined in [i-x,i-x+1) of degree y-1    ****************
//**********************************************************************************
double Spline_Basis::N(double t, int i, int degree)
{
	
	//Calculate the B-Spline value, this is done recursively.
   
	//If the numerator and denominator are 0 the expression is 0.
	//If the deonimator is 0 the expression is 0
	double value;

	if (degree == 0) 
	{
		if ((lambda[i] <= t) && (t < lambda[i+1]))
			value = 1;
		else
			value = 0;
	} 
	else 
	{
		if ((lambda[i+degree] == lambda[i]) && (lambda[i+degree+1] == lambda[i+1]))
			value = 0;
		else if (lambda[i+degree] == lambda[degree]) 
			value = (lambda[i+degree+1] - t) / (lambda[i+degree+1] - lambda[i+1]) * N(t,i+1, degree-1);
		else if (lambda[i+degree+1] == lambda[i+1])
			value = (t - lambda[i]) /  (lambda[i+degree]   - lambda[i])   * N(t,i,  degree-1);
		else
			value = (t - lambda[i])  / (lambda[i+degree]   - lambda[i])   * N(t,i,  degree-1) + 
                 (lambda[i+degree+1] - t) / (lambda[i+degree+1] - lambda[i+1]) * N(t,i+1,degree-1);
   }
   return value;
}


int Spline::Get_new_hnode(Path* path, double r, double f)
{
	//this routine calculates new points laying on the "skeleton line"
	//of the path line by intersecting perpendicular lines to the initial spline
	//with the path line segments

	//calculate the perpendicular line defined by a point p on the spline 
	//and the slope dy/dx at the parameter r
	double dx,dy;
	CamPoint p;

	Get_point(&p, r);
	Get_derivative(r, &dx, &dy);

	//now intersect the path with the line defined by p and (dy,-dx)
	//(the line pependicular to the b-spline) by calculating the 
	//distance between a pixel and the line and
	//convert the line into a form ax+by+c=0 where -m*x - y-m*x_p +y_p = 0
	double a = -dx/dy;
	double b = -1;
	double c = p.y - a*p.x;

	int i_p1,i_p2;
	bool found1=FALSE, found2=FALSE;
	double nenner = sqrt(a*a+b*b);
	//tolerance is the maximal distance between a point of a line and a point of a 
	//path segment so that the line is defined to intersect the boundary of the path
	//segment. sqrt(2)*0.009/f/2.0 is half of the diagonal length of a normalized pixel
	double tolerance=sqrt(2)*0.009/f/2.0;

	//get the relevant segment number
	double j_p;
	modf(r,&j_p);
	double d;
	for(int i=0; i< path->path_seg[(int)j_p].p_nr; i++)
	{
		d= fabs(a*path->path_seg[(int)j_p].hpixel[i].x + b*path->path_seg[(int)j_p].hpixel[i].y + c) / nenner;
		if(d < tolerance && found1)
		{
			i_p2 = i;
			found2=TRUE;
			break;
		}	
		if(d < tolerance && found1 == FALSE)
		{
			i_p1 = i;
			found1=TRUE;
			//go two pixel further because a wrong intersection on the same segment line might be found
			i+=2;
		}
	}
	if(found1 && found2 && i_p1 != i_p2)
	{
		node[m].x = (path->path_seg[(int)j_p].hpixel[i_p1].x + path->path_seg[(int)j_p].hpixel[i_p2].x)/2.0;
		node[m].y = (path->path_seg[(int)j_p].hpixel[i_p1].y + path->path_seg[(int)j_p].hpixel[i_p2].y)/2.0;
		parameter[m]=r;
		m++;
		if(m>=2500)
			return 2;
	}
	return 0;
}


int Spline::Line_intersect(double a, double b, double c, CamPoint *hp, double *r_l, double r_u)
{
	//if a solution (intersection is found) TRUE is returned otherwise FALSE
	//*************************************************************************************
	//***************************Interval sectioning***************************************
	// *r_l is the lower limit and r_u is the upper limit of the intersection 
	// parameter window
	//the routine returns
	//0   if a regular solution was found
	//1   if a solution with a critical intersection angle was found
	//3   if no or a wrong solution was found
	int i=0,solution=1;
	const int max_iter=100;			//the maximal number of iterations
	double aa=*r_l;
	double bb= r_u;
	double cc;
	double res_a,res_b,res_c=1;
	CamPoint p;
	const double angle_tol=0;//3.14159/25

	Get_point(&p, *r_l);
	res_a=a*p.x + b*p.y + c;
	Get_point(&p, r_u);
	res_b=a*p.x + b*p.y + c;

	if(res_a <0 && res_b>0)
	{
		while(fabs(res_c) > 0.00001)
		{
			cc=(aa+bb)/2.0;
			Get_point(&p, cc);
			res_c=a*p.x + b*p.y + c;
			if(res_c<0)
				aa=cc;
			else 
				bb=cc;
			if(++i > max_iter)   //no solution found
			{
				hp->x=-1;
				return 3;
			}
		}
		//check solution
		if(*r_l == cc)
		{
			hp->x=-1;   //wrong solution
			return 3;
		}
		else
		{
			*r_l=cc;
			//get the intersection angle at the parameter *r_l
			//-first calculate dy/dx of the spline
			//angle of the epiline
			double dx=Get_x_derivative(*r_l);
			double dy=Get_y_derivative(*r_l);
			double m_spline=dy/dx;
			double m_epi   =-a/b;
			double gamma=atan(fabs((m_spline-m_epi)/(1+m_spline+m_epi)));
			if(gamma>angle_tol) //if the intersection angle is large enough: regular solution found
			{
				hp->x=p.x;
				hp->y=p.y;
				hp->w=1.0;
				return 0;
			}
			else
			{
				return 1;
			}
		}


	}
	else if(res_a > 0 && res_b < 0) //change
	{
		res_c=res_a;
		res_a=res_b;
		res_b=res_c;

		res_c=1;
		aa=	r_u;
		bb=*r_l;

		while(fabs(res_c) > 0.00001)
		{
			cc=(aa+bb)/2.0;
			Get_point(&p, cc);
			res_c=a*p.x + b*p.y + c;
			if(res_c<0)
				aa=cc;
			else 
				bb=cc;
			if(++i > max_iter)   //no solution found
			{
				hp->x=-1;
				return 3;
			}
		}
		//check solution
		if(*r_l == cc)
		{
			//wrong solution
			hp->x=-1;
			return 3;
		}
		else
		{
			*r_l=cc;

			//get the intersection angle at the parameter *r_l
			//-first calculate dy/dx of the spline
			//angle of the epiline
			double dx=Get_x_derivative(*r_l);
			double dy=Get_y_derivative(*r_l);
			double m_spline=dy/dx;
			double m_epi   =-a/b;
			double gamma=atan(fabs((m_spline-m_epi)/(1+m_spline+m_epi)));
			if(gamma>angle_tol)  //if the intersection angle is large enough: regular solution found
			{
				hp->x=p.x;
				hp->y=p.y;
				hp->w=1.0;
				return 0;
			}
			else
			{
				return 1;
			}
		}
	
	}
	else //there is no or more than one intersection-divide the domain and repeat search
	{
		hp->x=-1;
		return 3;
	}
	return 0;
}


void Spline::Get_approx_intersection(double a, double b, double c, double *x, double *y, int *i_s)
{
//this functions gets the intersection of a line ax+by+c=0 with 
//the control polygon of a parametric spline (lamda, cx, cy)

//to find out if line ax+by+c intersects a polygon element
//cx[i],cy[i]...cx[i+1],cy[i+1] project intersect the line 
//x axis and check if intersection is between orthogonal 
//projections of cx[i],cy[i] and cx[i+1],cy[i+1]


		//get equation for segment cx[i],cy[i] ------- cx[i+1],cy[i+1]
		double a_cp= (cy[*i_s+1]-cy[*i_s])/(cx[*i_s+1]-cx[*i_s]);
		double b_cp=-(cy[*i_s+1]-cy[*i_s])/(cx[*i_s+1]-cx[*i_s])*cx[*i_s]+cy[*i_s];

		//get intersection point
		*x=-(b*b_cp+c)/(a+b*a_cp);
		*y=a_cp* *x+b_cp;
		//check if intersection point is on segment

		if((*x<=cx[*i_s+1] && *x>=cx[*i_s]) || (*x>=cx[*i_s+1] && *x<=cx[*i_s]))
		{
				return;
		}
		else if((*x<=cx[*i_s+2] && *x>=cx[*i_s+1]) || (*x>=cx[*i_s+2] && *x<=cx[*i_s+1]))
		{
				*i_s+=1;
				return;
		}
		else 
		{
			//*i_s-=1;
			*x=-1;
			*y=-1;
		}




/*	static int first=1;
	static double x_last;

	for(int i=0;i < g+k ; i++)
	{
		//get equation for segment cx[i],cy[i] ------- cx[i+1],cy[i+1]
		double a_cp= (cy[i+1]-cy[i])/(cx[i+1]-cx[i]);
		double b_cp=-(cy[i+1]-cy[i])/(cx[i+1]-cx[i])*cx[i]+cy[i];

		//get intersection point
		*x=-(b*b_cp+c)/(a+b*a_cp);
		*y=a_cp* *x+b_cp;
		//check if intersection point is on segment
		if(first)
		{
			if((*x<=cx[i+1] && *x>=cx[i]) || (*x>=cx[i+1] && *x<=cx[i]))
			{
				if(i>= *i_s)
				{
					*i_s=i;
					x_last=*x;
					first=0;
					return;
				}
			}
		}
		else
		{
			if((*x<=cx[i+1] && *x>=x_last) || (*x>=cx[i+1] && *x<=x_last))
			{
				if(i== *i_s)
				{
					*i_s=i;
					x_last=*x;
					return;
				}
			}
			else
				first=1;

		}
	}
	*x=-1;
	*y=-1;
	*/
}


