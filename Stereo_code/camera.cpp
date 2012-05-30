#include "stdafx.h"
#include <stdio.h>
#include "niimaq.h"
#include "parameters.h"
#include "matrix_op.h"
#include "path.h"
#include "camera.h"
#include "plot.h"
#include <math.h>
#include <imsl.h>
#include <fstream.h>

extern Vec2	 table1[640*480], table2[640*480];

double Camera::K[];

Camera::Camera()
{
	init =0;
}


Camera::~Camera()
{
	//delete []table;
}


void Camera::Read_cam_par(char file_name[])
{
	ifstream indat;
	char a[255];

	indat.open(file_name, ios::nocreate);
	indat.getline(a,71,'\n');
	indat.getline(a,71,'\n');
	//Get Camera position
	indat >> ex.X_0;
	indat >> ex.Y_0;
	indat >> ex.Z_0;
	//Get Camera angular position
	indat >> ex.omega;
	indat >> ex.phi;
	indat >> ex.kappa;

	//Get Instrinsic Parameters
	indat.getline(a,71,'\n');
	indat.getline(a,71,'\n');
	indat.getline(a,71,'\n');
	indat >> in.focus;
	indat >> in.x_h;
	indat >> in.y_h;
	indat >> in.s_x;
	indat >> in.theta;                   
	indat >> in.k1;       
	indat >> in.k2; 
	indat >> in.p1; 
	indat >> in.p2; 

	indat.close();
}


bool Camera::Read_ccd_par(char *file_name)
{
	FILE *file = NULL;
	file= fopen(file_name,"r");

	if(file == NULL)        //File non existing
		return FALSE;
	else if(ferror(file))  //Error
		return FALSE;

	char a=0;
	while(a != '\n')
		a=fgetc(file);

	fscanf(file,"%i", &ccd.width);
	fscanf(file,"%i", &ccd.height);
	fscanf(file,"%lf",&ccd.pix_w);
	fscanf(file,"%lf",&ccd.pix_h);
	fclose(file);

	return TRUE;
}


void Camera::Get_transf_matrix()
{
	t.t1=ex.X_0;
	t.t2=ex.Y_0;	
	t.t3=ex.Z_0;
	//Calculation of the Rotationsmatrix 
	//The Matrix is applied as following:  m : point in global coordinates
	//                                     m': point in cam1   coordinates
	//                       t=X_0, Y_0, Z_0 : position of the camera in global coordinates
	//      m'=transpose(R) (m - t)     
	R.a11 =  cos(ex.phi)*cos(ex.kappa);
	R.a12 = -cos(ex.phi)*sin(ex.kappa);
	R.a13 =  sin(ex.phi);
	R.a21 =  cos(ex.omega)*sin(ex.kappa) + sin(ex.omega)*sin(ex.phi)*cos(ex.kappa);
	R.a22 =  cos(ex.omega)*cos(ex.kappa) - sin(ex.omega)*sin(ex.phi)*sin(ex.kappa);
	R.a23 = -sin(ex.omega)*cos(ex.phi);  
	R.a31 =  sin(ex.omega)*sin(ex.kappa) - cos(ex.omega)*sin(ex.phi)*cos(ex.kappa);
	R.a32 =  sin(ex.omega)*cos(ex.kappa) + cos(ex.omega)*sin(ex.phi)*sin(ex.kappa);
	R.a33 =  cos(ex.omega)*cos(ex.phi);
}


void Camera::Get_camera_matrix()
{
	//!!!!the rotation matrix is defined R^T!!!!!!!!!!!!!!
	P[0]=   R.a11;
	P[1]=   R.a21;
	P[2]=   R.a31; 
	P[3]=  -R.a11*t.t1-R.a21*t.t2-R.a31*t.t3; 
	P[4]=   R.a12;
	P[5]=   R.a22;
	P[6]=   R.a32;
	P[7]=  -R.a12*t.t1-R.a22*t.t2-R.a32*t.t3; 
	P[8]=   R.a13; 
	P[9]=   R.a23; 
	P[10]=  R.a33; 
	P[11]= -R.a13*t.t1-R.a23*t.t2-R.a33*t.t3;
}


void Camera::Set_position(T_vec t_new_origin)
{
	ex.X_0	= t_new_origin.t1;
	ex.Y_0	= t_new_origin.t2;
	ex.Z_0	= t_new_origin.t3;
}



void Camera::Set_orientation(double phi_new, double omega_new, double kappa_new)
{
	ex.phi	=	phi_new;
	ex.omega=	omega_new;
	ex.kappa=	kappa_new;
}


void Camera::Trans_angles(T_vec direction)
{
	const int n=3;
	double r[3];
	double A[9];
	double *result;

	r[0]=1;
	r[1]=0;
	r[2]=0;

	A[0]=direction.t1*R.a11;
	A[1]=direction.t2*R.a21;
	A[2]=direction.t3*R.a31;
	A[3]=direction.t1*R.a12;
	A[4]=direction.t2*R.a22;
	A[5]=direction.t3*R.a32;
	A[6]=direction.t1*R.a13;
	A[7]=direction.t2*R.a23;
	A[8]=direction.t3*R.a33;


	result=imsl_d_lin_sol_gen(n, A, r,0);

/*	P->X = *result++;
	P->Y = *result++;
	P->Z = *result++;
	W	 = *result; 
	P->X = P->X/W;
	P->Y = P->Y/W;
	P->Z = P->Z/W;
*/

}


void Camera::Get_essential(Matrix R_to, T_vec t_to)
{
//*********calculate essential matrix E *********************************************
	//the matrix transforms the coordinates from the view
	//of the current camera instance into the view with
	//R_to and t_to
	Matrix Rot = Neg(  mat_mult    (transpose(R_to),   R));
	T_vec  trans = mat_vec_mult(transpose(R_to), (vec_sub(t,t_to)));
	Matrix T_x;
	T_x.a11 =  0.0;
	T_x.a12 = -trans.t3;
	T_x.a13 =  trans.t2;
	T_x.a21 =  trans.t3;
	T_x.a22 =  0.0;
	T_x.a23 = -trans.t1;
	T_x.a31 = -trans.t2;
    T_x.a32 =  trans.t1;
	T_x.a33 =  0.0;

	E = mat_mult(T_x, Rot);

	//this can be used to check the quality of the Essential matrix
//	double det_R   = Det(R);
//	double det_T_x = Det(T_x);
//	return det_E   = Det(*E);
}



void Camera::Get_epi(EpiPar *epipar, CamPoint hpoint){
//*********calculate normalized epi lines in cam 2            ***************************************
//*********                 "  *E hm1  "                      ***************************************
//*********   a*hm2.x + b*hm.y + c = 0                        ***************************************    
	T_vec t_dummy1;
	T_vec t_dummy2;

	t_dummy1.t1 = hpoint.x;
	t_dummy1.t2 = hpoint.y;
	t_dummy1.t3 = hpoint.w;

	t_dummy2 = mat_vec_mult(E, t_dummy1);  

	epipar->a = t_dummy2.t1;
	epipar->b = t_dummy2.t2;
	epipar->c = t_dummy2.t3;			
}


void Camera::Normalize_look_up(Path *path)
{
	for(Path_seg *s = path->path_seg; s < path->path_seg+path->nr_seg; s++)
	{
		CamPoint *hp = s->hpixel;
		for(Pixel *p = s->pixel; p < s->pixel +s->p_nr; p++)
		{
			//get data from look up table and do the normalization projection
			hp->x= table[p->x + 640*p->y].x / (-in.focus);
			hp->y= table[p->x + 640*p->y].y / (-in.focus);
			hp->w= 1.0;
			hp++;
		}
	}
}



void Camera::Normalize_look_up(Path_seg *path_seg)
{
	//this routine normalizes endpoints of path segments
	Pixel	 *p;
	CamPoint *hp;
	int i=0;

	while(path_seg->end_point_index[i] != -1)
	{
		int index=path_seg->end_point_index[i];
		p = &path_seg->pixel[index];
		hp= &path_seg->hpixel[index];
		//get data from look up table and do the normalization projection
		hp->x= table[p->x + 640*p->y].x / (-in.focus);
		hp->y= table[p->x + 640*p->y].y / (-in.focus);
		hp->w= 1.0;
		i++;
	}
}


bool Camera::Normalize_look_up(const Pixel p, CamPoint *hp)
{
	//get data from look up table and do the normalization projection
	if(p.x >= 0 && p.x < ccd.width && p.y>=0 && p.y < ccd.height)
	{
		hp->x= table[p.x + 640*p.y].x / (-in.focus);
		hp->y= table[p.x + 640*p.y].y / (-in.focus);
		hp->w= 1.0;
		return TRUE;
	}
	else
		return FALSE;
}

bool Camera::Normalize_look_up(const CamPoint p, CamPoint *hp)
{
	//get data from look up table and do the normalization projection
	if(p.x >= 0 && p.x < ccd.width && p.y>=0 && p.y < ccd.height)
	{
		int x_u=(int)p.x;
		int x_o=x_u+1;
		int y_u=(int)p.y;
		int y_o=y_u+1;
		double v1_x=table[x_u + 640*y_u].x;
		double v1_y=table[x_u + 640*y_u].y;
		double v2_x=table[x_u + 640*y_o].x;
		double v2_y=table[x_u + 640*y_o].y;
		double v3_x=table[x_o + 640*y_u].x;
		double v3_y=table[x_o + 640*y_u].y;
		double v4_x=table[x_o + 640*y_o].x;
		double v4_y=table[x_o + 640*y_o].y;

		double vm1_x=v1_x+(v2_x-v1_x)*(p.x-x_u);
		double vm1_y=v1_y+(v2_y-v1_y)*(p.x-x_u);
		double vm2_x=v4_x+(v3_x-v4_x)*(p.x-x_u);
		double vm2_y=v4_y+(v3_y-v4_y)*(p.x-x_u);

		double vm_x=vm1_x+(vm2_x-vm1_x)*(p.y-y_u);
		double vm_y=vm1_y+(vm2_y-vm1_y)*(p.y-y_u);

		hp->x= vm_x / (-in.focus);
		hp->y= vm_y / (-in.focus);

		//hp->x= table[p.x + 640*p.y].x / (-in.focus);
		//hp->y= table[p.x + 640*p.y].y / (-in.focus);
		hp->w= 1.0;
		return TRUE;
	}
	else
		return FALSE;
}

double g_k1,g_k2,g_p1,g_p2,g_x_h,g_y_h,g_theta,g_s_x;
double g_pix_w,g_pix_h;
int    g_height,g_width;
CamPoint     p_temp;
void dist_fcn(int, int, double[], double[]);

bool Camera::Normalize(const CamPoint p, CamPoint *hp)
{
	//get data from look up table and do the normalization projection
	if(p.x >= 0 && p.x < ccd.width && p.y>=0 && p.y < ccd.height)
	{
		int		m=2,n=2;           //m: Number of Funktions   n: Number of Variables  n<=m
		double	*result, xguess[2];

		p_temp.x=p.x;
		p_temp.y=p.y;

		g_k1	=in.k1;
		g_k2	=in.k2;
		g_p1	=in.p1;
		g_p2	=in.p2;
		g_x_h	=in.x_h;
		g_y_h	=in.y_h;
		g_theta	=in.theta;
		g_s_x	=in.s_x;
		g_pix_w =ccd.pix_w;
		g_pix_h =ccd.pix_h;
        g_height=ccd.height;
		g_width =ccd.width;

		xguess[0]=  (p_temp.x  - (double)g_width  /2.0)*g_pix_w;
		xguess[1]=  ((double)g_height /2.0 - p_temp.y )*g_pix_h;

		result = imsl_d_nonlin_least_squares(dist_fcn, m, n, IMSL_XGUESS, xguess, IMSL_MAX_ITN, 500,IMSL_MAX_FCN, 1000, 0);

		hp->x = *result++;
		hp->y = *result;
		hp->w= 1.0;
		hp->x= hp->x / (-in.focus);
		hp->y= hp->y / (-in.focus);
		return TRUE;
	}
	else
		return FALSE;
}


bool Camera::Normalize(const Pixel p, CamPoint *hp)
{
	//get data from look up table and do the normalization projection
	if(p.x >= 0 && p.x < ccd.width && p.y>=0 && p.y < ccd.height)
	{
		int		m=2,n=2;           //m: Number of Funktions   n: Number of Variables  n<=m
		double	*result, xguess[2];

		p_temp.x=(double)p.x;
		p_temp.y=(double)p.y;

		g_k1	=in.k1;
		g_k2	=in.k2;
		g_p1	=in.p1;
		g_p2	=in.p2;
		g_x_h	=in.x_h;
		g_y_h	=in.y_h;
		g_theta	=in.theta;
		g_s_x	=in.s_x;
		g_pix_w =ccd.pix_w;
		g_pix_h =ccd.pix_h;
        g_height=ccd.height;
		g_width =ccd.width;

		xguess[0]=  (p_temp.x  - (double)g_width  /2.0)*g_pix_w;
		xguess[1]=  ((double)g_height /2.0 - p_temp.y )*g_pix_h;

		result = imsl_d_nonlin_least_squares(dist_fcn, m, n, IMSL_XGUESS, xguess, IMSL_MAX_ITN, 500,IMSL_MAX_FCN, 1000, 0);

		hp->x = *result++;
		hp->y = *result;
		hp->w= 1.0;
		hp->x= hp->x / (-in.focus);
		hp->y= hp->y / (-in.focus);
		return TRUE;
	}
	else
		return FALSE;
}


void dist_fcn(int m, int n, double x[], double f[])
{
	//x[] are the normalized coordinates
	double r,x_b,y_b,x_p,y_p,x_c,y_c;

	x_b = g_x_h + x[0];
	y_b = g_y_h + x[1];

	r=sqrt(x_b*x_b+y_b*y_b);
	x_c=x_b + x_b * (g_k1*r*r + g_k2*r*r*r*r) +g_p1*(r*r +2.0*x_b*x_b) + 2.0*g_p2*x_b*y_b;
	y_c=y_b + y_b * (g_k1*r*r + g_k2*r*r*r*r) +g_p2*(r*r +2.0*y_b*y_b) + 2.0*g_p1*x_b*y_b;		


	//shearing and stretching -> pixel coordinates (0,0): upper left corner
	x_p = (g_s_x * x_c  - sin(g_theta) * y_c) /g_pix_w + (double)g_width/2.0;
	y_p = (double)g_height / 2.0 - y_c * cos(g_theta) / g_pix_h;
		
	f[0]  =  x_p  - p_temp.x;
	f[1]  =  y_p  - p_temp.y;
}



CamPoint Camera::Transform_nor2img(CamPoint p)
{
	//this routine transforms normalized coordinates back  
	//this is used for control purposes
	double x= -in.focus * p.x;
	double y= -in.focus * p.y;

	x += in.x_h;
	y += in.y_h;

	double r=sqrt(x*x+y*y);
	double x_c=x + x * (in.k1*r*r + in.k2*r*r*r*r) +in.p1*(r*r +2.0*x*x) + 2.0*in.p2*x*y;
	double y_c=y + y * (in.k1*r*r + in.k2*r*r*r*r) +in.p2*(r*r +2.0*y*y) + 2.0*in.p1*x*y;		

	//shearing and stretching -> metrische Koordinaten (0,0): links oben
	p.x = (in.s_x * x_c  - sin(in.theta) * y_c) / ccd.pix_w + (double)ccd.width/2.0;
	p.y = (double)ccd.height / 2.0 - y_c * cos(in.theta) / ccd.pix_h;
	return p;
}


void Camera::Transform_nor2img(CamPoint *p, CamPoint *hp, int nr)
{
	//this routine transforms normalized coordinates back  
	//this is used for control purposes
    for(int i=0; i<nr; i++)
	{
		double x= -in.focus * hp[i].x;
		double y= -in.focus * hp[i].y;

		double x_b = in.x_h + x;
		double y_b = in.y_h + y;

		double r=sqrt(x_b*x_b+y_b*y_b);
		double x_c=x_b + x_b * (in.k1*r*r + in.k2*r*r*r*r) +in.p1*(r*r +2.0*x_b*x_b) + 2.0*in.p2*x_b*y_b;
		double y_c=y_b + y_b * (in.k1*r*r + in.k2*r*r*r*r) +in.p2*(r*r +2.0*y_b*y_b) + 2.0*in.p1*x_b*y_b;		

		//shearing and stretching -> metrische Koordinaten (0,0): links oben
		p[i].x = (in.s_x * x_c  - sin(in.theta) * y_c) / ccd.pix_w + (double)ccd.width/2.0;
		p[i].y = (double)ccd.height / 2.0 - y_c * cos(in.theta) / ccd.pix_h;
	}
}


CamPoint Camera::Project_point(GlobalPoint P)
{
	CamPoint p;

	double nenner =  R.a13*(P.X - ex.X_0) + R.a23*(P.Y - ex.Y_0) + R.a33*(P.Z - ex.Z_0);

	p.x = (R.a11*(P.X - ex.X_0)  +  R.a21*(P.Y - ex.Y_0)  +  R.a31*(P.Z - ex.Z_0) ) / nenner;
	p.y = (R.a12*(P.X - ex.X_0)  +  R.a22*(P.Y - ex.Y_0)  +  R.a32*(P.Z - ex.Z_0) ) / nenner;
	p.w = 1;
	return p;
}



CamPoint Camera::Project_point_norm(GlobalPoint P)
{
	CamPoint p;

	p.x = R.a11*(P.X - ex.X_0)  +  R.a21*(P.Y - ex.Y_0)  +  R.a31*(P.Z - ex.Z_0);
	p.y = R.a12*(P.X - ex.X_0)  +  R.a22*(P.Y - ex.Y_0)  +  R.a32*(P.Z - ex.Z_0);
	p.w = R.a13*(P.X - ex.X_0)  +  R.a23*(P.Y - ex.Y_0)  +  R.a33*(P.Z - ex.Z_0);
	return p;
}


int Camera::Load_look_up(char *file_name, CProgressCtrl *bar, int view)
{
	FILE *fp=fopen(file_name,"r");

	if(fp == NULL)         //File non existing
		return 1;
	else if(ferror(fp))  //Error
		return 2;

	if(view==1)
		table =table1;
	else 
		table =table2;

	Vec2 *t     = table;
	Vec2 *t_end = table+640*480;
	bar->SetRange(0,480);
	bar->SetStep(1);
	int i=0;

	do	
	{
		fscanf(fp,"%lf%lf", &t->x, &t->y);
		if(i++%640==0)
			bar->StepIt();
	}
	while(++t < t_end);
	fclose(fp);
	bar->SetPos(0);
	return 0;
}



void Camera::Plot_epi(CDC *device, uInt8 imagedata[], double a, double b, double c)
{
	a = -a / b;
	b = -c / b;

	double a_res= -a *ccd.pix_w/ccd.pix_h;
	double b_res= (double)ccd.height/2.0 - in.y_h/ccd.pix_h + in.focus*b/ccd.pix_h +
			      (double)ccd.width /2.0* a *ccd.pix_w/ccd.pix_h + a*in.x_h /ccd.pix_h;

	Plot::Line(device, imagedata, a_res, b_res);
}


void Camera::Get_triangulation_matrix(Camera c1, Camera c2)
{
	//!! wrong matrix , take the transpose R!!!!!!1
	K[0]  =c1.R.a11;
	K[1]  =c1.R.a12;
	K[2]  =c1.R.a13;
	K[3]  =-(c1.R.a11*c1.t.t1 + c1.R.a12*c1.t.t2 + c1.R.a13*c1.t.t3);
	K[4]  =c1.R.a21;
	K[5]  =c1.R.a22;
	K[6]  =c1.R.a23;
	K[7]  =-(c1.R.a21*c1.t.t1 + c1.R.a22*c1.t.t2 + c1.R.a23*c1.t.t3);
	K[8]  =c1.R.a31;
	K[9]  =c1.R.a32;
	K[10] =c1.R.a33;
	K[11] =-(c1.R.a31*c1.t.t1 + c1.R.a32*c1.t.t2 + c1.R.a33*c1.t.t3);
	K[12] =c2.R.a11;
	K[13] =c2.R.a12;
	K[14] =c2.R.a13;
	K[15] =-(c2.R.a11*c2.t.t1 + c2.R.a12*c2.t.t2 + c2.R.a13*c2.t.t3);
	K[16] =c2.R.a21;
	K[17] =c2.R.a22;
	K[18] =c2.R.a23;
	K[19] =-(c2.R.a21*c2.t.t1 + c2.R.a22*c2.t.t2 + c2.R.a23*c2.t.t3);
	K[20] =c2.R.a31;
	K[21] =c2.R.a32;
	K[22] =c2.R.a33;
	K[23] =-(c2.R.a31*c2.t.t1 + c2.R.a32*c2.t.t2 + c2.R.a33*c2.t.t3);
}

extern FILE *f_global;

void Camera::Get_3D_point(GlobalPoint *P, CamPoint *hp1, CamPoint *hp2, int nr)
{
	int n=4;//columns
	int m=6;//rows
	double *result;
	double r[6];
	double W;

	for(int i=0; i<nr; i++)
	{
		if(hp1->x != -1 || hp2->x != -1 || hp1->x == 0 || hp2->x == 0)
		{
			r[0]=hp1->x;
			r[1]=hp1->y;
			r[2]=1;
			r[3]=hp2->x;
			r[4]=hp2->y;
			r[5]=1;

			result=imsl_d_lin_least_squares_gen (m, n, K, r,0);
			P->X = *result++;
			P->Y = *result++;
			P->Z = *result++;
			W	 = *result; 
			P->X = P->X/W;
			P->Y = P->Y/W;
			P->Z = P->Z/W;

			//check if the soluton makes sense, if so write to solution to file
			if(fabs(P->X) < 2000 && fabs(P->Y)<2000 && fabs(P->Z)<2000)
			{
				fprintf(f_global,"%10.6f %10.6f %10.6f\n",P->X,P->Y,P->Z);
			}
			P++;
			hp1++;
			hp2++;
		}
		else
		{
			P->X=-1;
			P++;
			hp1++;
			hp2++;
		}

	}
}



bool Camera::Get_3D_point(GlobalPoint *P, CamPoint *hp1, CamPoint *hp2, double r_par)
{

	if(hp1->x != -1 || hp2->x != -1 || hp1->x == 0 || hp2->x == 0)
	{
		const int n=4;//columns
		const int m=6;//rows
		double *result;
		double r[6];
		double W;

		r[0]=hp1->x;
		r[1]=hp1->y;
		r[2]=1;
		r[3]=hp2->x;
		r[4]=hp2->y;
		r[5]=1;

		result=imsl_d_lin_least_squares_gen (m, n, K, r,0);
		P->X = *result++;
		P->Y = *result++;
		P->Z = *result++;
		W	 = *result; 
		P->X = P->X/W;
		P->Y = P->Y/W;
		P->Z = P->Z/W;

		//check if the soluton makes sense, if so write to solution to file
		if(fabs(P->X) < 2000 || fabs(P->Y)<2000 || fabs(P->Z)<2000)
		{
			P->r=r_par;
			fprintf(f_global,"%4i %10.2f %10.2f %10.2f\n",(int)r_par, P->X, P->Y, P->Z);
			return TRUE;
		}
	}
	return FALSE;


}


bool Camera::Get_3D_point_hom(GlobalPoint *Point, double *P1, double *P2, CamPoint *hp1, CamPoint *hp2, double r_par)
{
	//calculation of the three dim. coordinates according to Multiple 
	//View Geometry p.297 and p298
	//P1 and P2 are the camera matricies of an ideal pin hole camera.
	if(hp1->x != -1 || hp2->x != -1 || hp1->x == 0 || hp2->x == 0)
	{
		const int n=4;//columns
		d_complex *eval;
		d_complex *evec;
		double A[16];

		A[0] =hp1->x * P1[8]  - P1[0];
		A[1] =hp1->x * P1[9]  - P1[1];
		A[2] =hp1->x * P1[10] - P1[2];
		A[3] =hp1->x * P1[11] - P1[3];
		A[4] =hp1->y * P1[8]  - P1[4];
		A[5] =hp1->y * P1[9]  - P1[5];
		A[6] =hp1->y * P1[10] - P1[6];
		A[7] =hp1->y * P1[11] - P1[7];

		A[8]= hp2->x * P2[8]   - P2[0];
		A[9]= hp2->x * P2[9]   - P2[1];
		A[10]=hp2->x * P2[10]  - P2[2];
		A[11]=hp2->x * P2[11]  - P2[3];
		A[12]=hp2->y * P2[8]   - P2[4];
		A[13]=hp2->y * P2[9]   - P2[5];
		A[14]=hp2->y * P2[10]  - P2[6];
		A[15]=hp2->y * P2[11]  - P2[7];

		eval= imsl_d_eig_gen (n, A, IMSL_VECTORS, &evec, 0);
		Point->X = evec[3].re;
		Point->Y = evec[7].re;
		Point->Z = evec[11].re;
		double W=  evec[15].re;

		Point->X = Point->X /W;
		Point->Y = Point->Y /W;
		Point->Z = Point->Z /W;

		//check if the soluton makes sense, if so write to solution to file
//		if(fabs(Point->X) < 2000 || fabs(Point->Y)<2000 || fabs(Point->Z)<2000)
//		{
			Point->r=r_par;
			fprintf(f_global,"%4i %10.2f %10.2f %10.2f\n",(int)r_par, Point->X, Point->Y, Point->Z);
			return TRUE;
//		}
	}
	return FALSE;
}

bool Camera::Get_3D_point_hom(GlobalPoint *Point, const double *P1, const double *P2, const CamPoint hp1, const CamPoint hp2)
{
	//calculation of the three dim. coordinates according to Multiple 
	//View Geometry p.297 and p298
	//P1 and P2 are the camera matricies of an ideal pin hole camera.
	if(hp1.x != -1 || hp2.x != -1 || hp1.x == 0 || hp2.x == 0)
	{
		const int n=4;//columns
		d_complex *eval;
		d_complex *evec;
		double A[16];

		A[0] =hp1.x * P1[8]  - P1[0];
		A[1] =hp1.x * P1[9]  - P1[1];
		A[2] =hp1.x * P1[10] - P1[2];
		A[3] =hp1.x * P1[11] - P1[3];
		A[4] =hp1.y * P1[8]  - P1[4];
		A[5] =hp1.y * P1[9]  - P1[5];
		A[6] =hp1.y * P1[10] - P1[6];
		A[7] =hp1.y * P1[11] - P1[7];

		A[8]= hp2.x * P2[8]   - P2[0];
		A[9]= hp2.x * P2[9]   - P2[1];
		A[10]=hp2.x * P2[10]  - P2[2];
		A[11]=hp2.x * P2[11]  - P2[3];
		A[12]=hp2.y * P2[8]   - P2[4];
		A[13]=hp2.y * P2[9]   - P2[5];
		A[14]=hp2.y * P2[10]  - P2[6];
		A[15]=hp2.y * P2[11]  - P2[7];

		eval= imsl_d_eig_gen (n, A, IMSL_VECTORS, &evec, 0);
		Point->X = evec[3].re;
		Point->Y = evec[7].re;
		Point->Z = evec[11].re;
		double W=  evec[15].re;

		Point->X = Point->X /W;
		Point->Y = Point->Y /W;
		Point->Z = Point->Z /W;

		return TRUE;
	}
	return FALSE;
}


CamPoint p1_h, p2_h;
void fcn(int m, int n, double x[], double f[]);
double X1_0, Y1_0, Z1_0, X2_0, Y2_0, Z2_0;
Matrix R1_temp, R2_temp;

bool Camera::Get_3D_point_non_lin(GlobalPoint *Point, const Camera c1, const Camera c2, CamPoint *hp1, CamPoint *hp2, double r_par)
{
	if(hp1->x != -1 || hp2->x != -1 || hp1->x == 0 || hp2->x == 0)
	{
		int m=4, n=3;           //m: Number of Funktions   n: Number of Variables  n<=m
		double xguess[3];
		double *result;

		xguess[0] =  500.0;
		xguess[1] =  400.0;
		xguess[2] =  -10.0;

		p1_h=*hp1;
		p2_h=*hp2;

		X1_0=c1.ex.X_0;
		Y1_0=c1.ex.Y_0; 
		Z1_0=c1.ex.Z_0; 
		X2_0=c2.ex.X_0; 
		Y2_0=c2.ex.Y_0; 
		Z2_0=c2.ex.Z_0;

		R1_temp=c1.R;
		R2_temp=c2.R;

		result = imsl_d_nonlin_least_squares(fcn, m, n, IMSL_XGUESS, xguess, 0);

		Point->X = *result++;
		Point->Y = *result++;
		Point->Z = *result;

		//check if the soluton makes sense, if so write to solution to file
		if(fabs(Point->X) < 2000 || fabs(Point->Y)<2000 || fabs(Point->Z)<2000)
		{
			Point->r=r_par;
			fprintf(f_global,"%4i %10.2f %10.2f %10.2f\n",(int)r_par, Point->X, Point->Y, Point->Z);
			return TRUE;
		}
	}
	return FALSE;
}

bool Camera::Get_3D_point_non_lin(GlobalPoint *Point, const Camera c1, const Camera c2, const CamPoint hp1, const CamPoint hp2)
{
	if(hp1.x != -1 || hp2.x != -1 || hp1.x == 0 || hp2.x == 0)
	{
		int m=4, n=3;           //m: Number of Funktions   n: Number of Variables  n<=m
		double xguess[3];
		double *result;

		xguess[0] =  500.0;
		xguess[1] =  400.0;
		xguess[2] =  -10.0;

		p1_h=hp1;
		p2_h=hp2;

		X1_0=c1.ex.X_0;
		Y1_0=c1.ex.Y_0; 
		Z1_0=c1.ex.Z_0; 
		X2_0=c2.ex.X_0; 
		Y2_0=c2.ex.Y_0; 
		Z2_0=c2.ex.Z_0;

		R1_temp=c1.R;
		R2_temp=c2.R;

		result = imsl_d_nonlin_least_squares(fcn, m, n, IMSL_XGUESS, xguess, 0);

		Point->X = *result++;
		Point->Y = *result++;
		Point->Z = *result;

		return TRUE;
	}
	return FALSE;
}


void fcn(int m, int n, double x[], double f[])
{
	double X = x[0];
	double Y = x[1];
	double Z = x[2];

	double x_h,y_h;

	double nenner1 =  R1_temp.a13*(X- X1_0)+R1_temp.a23*(Y-Y1_0)+R1_temp.a33*(Z-Z1_0);
	double nenner2 =  R2_temp.a13*(X- X2_0)+R2_temp.a23*(Y-Y2_0)+R2_temp.a33*(Z-Z2_0);
		
	//For camera 1
	//Bildkoordinaten 
	x_h   = ( R1_temp.a11*(X- X1_0)+R1_temp.a21*(Y-Y1_0)+R1_temp.a31*(Z-Z1_0) ) / nenner1;										  
	y_h   = ( R1_temp.a12*(X- X1_0)+R1_temp.a22*(Y-Y1_0)+R1_temp.a32*(Z-Z1_0) ) / nenner1;
		
	f[0]  =  x_h  - p1_h.x;
	f[1]  =  y_h  - p1_h.y;

	//For camera 2
	//Bildkoordinaten 
	x_h   = ( R2_temp.a11*(X- X2_0)+R2_temp.a21*(Y-Y2_0)+R2_temp.a31*(Z-Z2_0) ) / nenner2;			                              
	y_h   = ( R2_temp.a12*(X- X2_0)+R2_temp.a22*(Y-Y2_0)+R2_temp.a32*(Z-Z2_0) ) / nenner2;

	f[2]  =  x_h  - p2_h.x;
	f[3]  =  y_h  - p2_h.y;
}

