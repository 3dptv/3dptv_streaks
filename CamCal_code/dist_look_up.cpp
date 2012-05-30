#include "stdafx.h"
#include <imsl.h>
#include <math.h>
#include <stdio.h>
#include "parameters.h"
#include "structures.h"
#include "dist_look_up.h"


extern		 Extrinsic_par ex_init;
extern		 Intrinsic_par in_init; 
extern       CCD_par ccd_init;
CamPoint     p_temp;
Vec2	     shift1[640*480], shift2[640*480];

void Get_table(char *file_name, CProgressCtrl *bar)
{
	//this routine callculates the inverse projection of the image coordinates
	//including the lens distortion
    FILE *out_file = NULL;
	int		i,j,m=2,n=2;           //m: Number of Funktions   n: Number of Variables  n<=m
	double	*result, xguess[2];

	j=strlen(file_name);
	file_name+=(j-3);
	strcpy(file_name, "lut");
	file_name-=(j-3);
	out_file = fopen (file_name,"w");


	xguess[0]=0.0;
	xguess[1]=0.0;

	bar->SetRange(0,480);
	bar->SetStep(1);
	for(j=0;j<480;j++)
	{
		for(i=0;i<640;i++)
		{
			p_temp.x=(double)i;
			p_temp.y=(double)j;

			result = imsl_d_nonlin_least_squares(dist_fcn, m, n, IMSL_XGUESS, xguess, 0);

			shift1[i+j*640].x = *result++;
			shift1[i+j*640].y = *result;
			fprintf(out_file, "%f %f\n", shift1[i+j*640].x, shift1[i+j*640].y);

			xguess[0] = shift1[i+j*640].x;
			xguess[1] = shift1[i+j*640].y;
		}
		bar->StepIt();
	}
}


void dist_fcn(int m, int n, double x[], double f[])
{
	//x[] are the normalized coordinates
	double r,x_b,y_b,x_p,y_p,x_c,y_c;

	x_b = in_init.x_h + x[0];
	y_b = in_init.y_h + x[1];

	r=sqrt(x_b*x_b+y_b*y_b);
	x_c=x_b + x_b * (in_init.k1*r*r + in_init.k2*r*r*r*r) +in_init.p1*(r*r +2.0*x_b*x_b) + 2.0*in_init.p2*x_b*y_b;
	y_c=y_b + y_b * (in_init.k1*r*r + in_init.k2*r*r*r*r) +in_init.p2*(r*r +2.0*y_b*y_b) + 2.0*in_init.p1*x_b*y_b;		


	//shearing and stretching -> pixel coordinates (0,0): upper left corner
	x_p = (in_init.s_x * x_c  - sin(in_init.theta) * y_c) / ccd_init.pix_w + (double)ccd_init.width/2.0;
	y_p = (double)ccd_init.height / 2.0 - y_c * cos(in_init.theta) / ccd_init.pix_h;
		
	f[0]  =  x_p  - p_temp.x;
	f[1]  =  y_p  - p_temp.y;
}


