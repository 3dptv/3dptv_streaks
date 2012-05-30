#include "stdafx.h"
#include <stdio.h>
#include <imsl.h>
#include <math.h>
#include "niimaq.h"
#include "parameters.h"
#include "structures.h"
#include "plot_functions.h"
#include "io_functions.h"
//#include "CamCalDlg.h"


extern uInt8*     color_image_data;
//*******This are the matched (by routine "Sort_Points")********************
//*******points*************************************************************
extern int number;
extern int first_plane_nr;                              //number of detected points in first plane
extern double x_gem[MAX_MATCH], y_gem[MAX_MATCH];
extern double X[MAX_MATCH], Y[MAX_MATCH], Z[MAX_MATCH];
//*******This parameters are loaded from the user interface*****************
extern Extrinsic_par ex_init;
extern Intrinsic_par in_init;
extern CCD_par       ccd_init;
extern P_matrix P;
CamPoint lin_modell_points[MAX_MATCH];

extern float rms;
//extern CEdit CCamCalDlg::m_status;



int Lin_calibrate(CDC *device)
{
	int i,j=0;
	int n=12;       //Number of the unkown coefficients in matrix A
	int m=2*number; //Number of equation for the coefficients in matrix A
	int  rank;      //rank of matrix

	double * A=(double *)calloc(m*n, sizeof(double));
	double * r=(double *)calloc(m,   sizeof(double)); 
	double *M, *result;
	double pi= imsl_d_constant ("pi",0);
	double *eval, *evec;    //eigen value, eigen vector
	double l,l2;
	double tol=1.0;
	double cond;  
	double x_mod,y_mod,nenner;	//Variables for the estiamtion of the quality of the result



	//*********x_gem and y_gem are values in [mm] with reference to the left upper********
	//*********corner*********************************************************************
	//*********Transform values into coordiante system with origin in image center********
	for(i=0;i<number;i++)
	{
		x_gem[i]=x_gem[i]-(double)ccd_init.width/2.0*ccd_init.pix_w;
		y_gem[i]=(double)ccd_init.height/2.0*ccd_init.pix_h-y_gem[i];
	}

	//*****************************Fill in the matrix A m x n  **********************
	for(i=0;i<number;i++)
	{
		//first row
		*(A+j)    =X[i]; 	
		*(A+j+1)  =Y[i]; 	
		*(A+j+2)  =Z[i]; 	
		*(A+j+3)  =1.0; 
		*(A+j+4)  =0.0; 
		*(A+j+5)  =0.0; 
		*(A+j+6)  =0.0; 
		*(A+j+7)  =0.0;
		*(A+j+8)  =-x_gem[i]* X[i]; 
		*(A+j+9)  =-x_gem[i]* Y[i];	
		*(A+j+10) =-x_gem[i]* Z[i]; 
		*(A+j+11) =-x_gem[i];
		//second row
		*(A+j+12) =0.0; 
		*(A+j+13) =0.0; 
		*(A+j+14) =0.0; 
		*(A+j+15) =0.0;	
		*(A+j+16) =X[i]; 	
		*(A+j+17) =Y[i]; 	
		*(A+j+18) =Z[i];	
		*(A+j+19) =1.0;
		*(A+j+20) =-y_gem[i]* X[i]; 
		*(A+j+21) =-y_gem[i]* Y[i];	
		*(A+j+22) =-y_gem[i]* Z[i]; 
		*(A+j+23) =-y_gem[i];
		j+=24;
	}
	for(i=0;i<m;i++)
		*(r+i)=0.0;
	//*****************************Calculate the rank of A *****************************
	imsl_d_lin_svd_gen (m, n, A, IMSL_RANK, tol, &rank,0);

	//*****************************Calculate M=trans(A)*A ******************************
	//*****************************M is a symetric n*n (12*12) matrix*******************
	M = imsl_d_mat_mul_rect("trans(A)*A",IMSL_A_MATRIX, m, n, A,0);

	//*****************************check the matrix M***********************************
	//**********************************************************************************
	imsl_d_lin_sol_gen (n, M, r, IMSL_CONDITION, &cond, 0);
	imsl_d_lin_svd_gen (n, n, M, IMSL_RANK, tol, &rank,0);

	//*****************************Calculate eigenvalues and ***************************
	//*****************************eigenvectors of M************************************
	eval = imsl_d_eig_sym (n, M, IMSL_VECTORS, &evec, 0);

	P.p11=*(evec+11);	P.p12=*(evec+23); 	P.p13=*(evec+35);  	P.p14=*(evec+47);
	P.p21=*(evec+59); 	P.p22=*(evec+71); 	P.p23=*(evec+83);  	P.p24=*(evec+95);
	P.p31=*(evec+107); 	P.p32=*(evec+119); 	P.p33=*(evec+131); 	P.p34=*(evec+143);


	//*********************find the scaling factor of the eigenvector*****************************
	l=sqrt(P.p31*P.p31+P.p32*P.p32+P.p33*P.p33);

	P.p11=P.p11/l; 	P.p12=P.p12/l; 	P.p13=P.p13/l; 	P.p14=P.p14/l; 
	P.p21=P.p21/l;	P.p22=P.p22/l;	P.p23=P.p23/l;	P.p24=P.p24/l;
	P.p31=P.p31/l;	P.p32=P.p32/l;	P.p33=P.p33/l;	P.p34=P.p34/l;
	//should be 1.0
	l2= sqrt(P.p11*P.p11+P.p12*P.p12+P.p13*P.p13)/sqrt(P.p21*P.p21+P.p22*P.p22+P.p23*P.p23);
	//display_double(l2,"l2 :  ",&m_status);

	//***************calculate the quality of the solution*****************************************
	rms=0.0;
	for(i=0;i<number;i++)
	{
		nenner=X[i]*P.p31+Y[i]*P.p32+Z[i]*P.p33+P.p34;
		x_mod =(X[i]*P.p11+Y[i]*P.p12+Z[i]*P.p13+P.p14)/nenner;
		y_mod =(X[i]*P.p21+Y[i]*P.p22+Z[i]*P.p23+P.p24)/nenner;
		if(i<first_plane_nr){
			lin_modell_points[i].x=(int)(x_mod/ccd_init.pix_w)+(double)ccd_init.width/2.0;
			lin_modell_points[i].y=(double)ccd_init.height/2.0-(int)(y_mod/ccd_init.pix_h);
		}
		rms+=(float)sqrt((x_mod-x_gem[i])*(x_mod-x_gem[i])+(y_mod-y_gem[i])*(y_mod-y_gem[i]));
	}
	rms=rms/(float)number;

	Plot_points(device, color_image_data, lin_modell_points, ccd_init.height, ccd_init.width, first_plane_nr, 255, 0, 0, 1);  //rgb

	//***************Calculate the Extrinsic and Intrinsic Parameters out of Matrix P***************
	ex_init.phi		=asin(P.p31);
	//ex_init.omega	=asin(-P.p32/cos(ex_init.phi)); did not work; hell knows why
	ex_init.omega	=acos(P.p33/cos(ex_init.phi));
	ex_init.kappa	=atan(-P.p21/P.p11);

	in_init.focus	= -P.p11/(cos(ex_init.phi)*cos(ex_init.kappa)); 
	
	//************solve for X_0,Y_0 and Z_0**********************************************************
	n=3;//number of rows
	m=3;//number of columns
	//fill in right hand side r
	*r    =P.p14;
	*(r+1)=P.p24;
	*(r+2)=P.p34;
	//fill in 3x3 matrix A
	*A    = in_init.focus *  cos(ex_init.phi)*cos(ex_init.kappa);
	*(A+1)= in_init.focus * (cos(ex_init.omega)*sin(ex_init.kappa)+sin(ex_init.omega)*sin(ex_init.phi)*cos(ex_init.kappa));
	*(A+2)= in_init.focus * (sin(ex_init.omega)*sin(ex_init.kappa)-cos(ex_init.omega)*sin(ex_init.phi)*cos(ex_init.kappa));
	*(A+3)=-in_init.focus *  cos(ex_init.phi)*sin(ex_init.kappa);
	*(A+4)= in_init.focus * (cos(ex_init.omega)*cos(ex_init.kappa)-sin(ex_init.omega)*sin(ex_init.phi)*sin(ex_init.kappa));
	*(A+5)= in_init.focus * (sin(ex_init.omega)*cos(ex_init.kappa)+cos(ex_init.omega)*sin(ex_init.phi)*sin(ex_init.kappa));
	*(A+6)= - sin(ex_init.phi);
	*(A+7)=   sin(ex_init.omega)*cos(ex_init.phi);
	*(A+8)= - cos(ex_init.omega)*cos(ex_init.phi);

	result = imsl_d_lin_least_squares_gen (m, n, A, r, 0);

	ex_init.X_0=*result++;
	ex_init.Y_0=*result++;
	ex_init.Z_0=*result;

	free(A);
	free(r);
	return 1;
}




int Lin_calibrate2(CDC *device)
{
	int i,j=0;
	int n=12;       //Number of the unkown coefficients in matrix A
	int m=2*number; //Number of equation for the coefficients in matrix A
	int  rank;      //rank of matrix

	double * A=(double *)calloc(m*n, sizeof(double));
	double * r=(double *)calloc(m,   sizeof(double)); 
	double *M, *result;
	double pi= imsl_d_constant ("pi",0);
	double *eval, *evec;    //eigen value, eigen vector
	double l,l2;
	double tol=1.0;
	double cond;  
	double x_mod,y_mod,nenner;	//Variables for the estiamtion of the quality of the result


	//*********x_gem and y_gem are values in [mm] with reference to the left upper********
	//*********corner*********************************************************************
	//*********Transform values into coordiante system with origin in image center********
	for(i=0;i<number;i++)
	{
		x_gem[i]=x_gem[i]-(double)ccd_init.width/2.0*ccd_init.pix_w;
		y_gem[i]=(double)ccd_init.height/2.0*ccd_init.pix_h-y_gem[i];
	}

	//*****************************Fill in the matrix A m x n  **********************
	for(i=0;i<number;i++)
	{
		//first row
		*(A+j)    =X[i]; 	
		*(A+j+1)  =Y[i]; 	
		*(A+j+2)  =Z[i]; 	
		*(A+j+3)  =1.0; 
		*(A+j+4)  =0.0; 
		*(A+j+5)  =0.0; 
		*(A+j+6)  =0.0; 
		*(A+j+7)  =0.0;
		*(A+j+8)  =-x_gem[i]* X[i]; 
		*(A+j+9)  =-x_gem[i]* Y[i];	
		*(A+j+10) =-x_gem[i]* Z[i]; 
		*(A+j+11) =-x_gem[i];
		//second row
		*(A+j+12) =0.0; 
		*(A+j+13) =0.0; 
		*(A+j+14) =0.0; 
		*(A+j+15) =0.0;	
		*(A+j+16) =X[i]; 	
		*(A+j+17) =Y[i]; 	
		*(A+j+18) =Z[i];	
		*(A+j+19) =1.0;
		*(A+j+20) =-y_gem[i]* X[i]; 
		*(A+j+21) =-y_gem[i]* Y[i];	
		*(A+j+22) =-y_gem[i]* Z[i]; 
		*(A+j+23) =-y_gem[i];
		j+=24;
	}
	for(i=0;i<m;i++)
		*(r+i)=0.0;
	//*****************************Calculate the rank of A *****************************
	imsl_d_lin_svd_gen (m, n, A, IMSL_RANK, tol, &rank,0);

	//*****************************Calculate M=trans(A)*A ******************************
	//*****************************M is a symetric n*n (12*12) matrix*******************
	M = imsl_d_mat_mul_rect("trans(A)*A",IMSL_A_MATRIX, m, n, A,0);

	//*****************************check the matrix M***********************************
	//**********************************************************************************
	imsl_d_lin_sol_gen (n, M, r, IMSL_CONDITION, &cond, 0);
	imsl_d_lin_svd_gen (n, n, M, IMSL_RANK, tol, &rank,0);

	//*****************************Calculate eigenvalues and ***************************
	//*****************************eigenvectors of M************************************
	eval = imsl_d_eig_sym (n, M, IMSL_VECTORS, &evec, 0);

	P.p11=*(evec+11);	P.p12=*(evec+23); 	P.p13=*(evec+35);  	P.p14=*(evec+47);
	P.p21=*(evec+59); 	P.p22=*(evec+71); 	P.p23=*(evec+83);  	P.p24=*(evec+95);
	P.p31=*(evec+107); 	P.p32=*(evec+119); 	P.p33=*(evec+131); 	P.p34=*(evec+143);


	//*********************find the scaling factor of the eigenvector*****************************
	l=sqrt(P.p31*P.p31+P.p32*P.p32+P.p33*P.p33);

	P.p11=P.p11/l; 	P.p12=P.p12/l; 	P.p13=P.p13/l; 	P.p14=P.p14/l; 
	P.p21=P.p21/l;	P.p22=P.p22/l;	P.p23=P.p23/l;	P.p24=P.p24/l;
	P.p31=P.p31/l;	P.p32=P.p32/l;	P.p33=P.p33/l;	P.p34=P.p34/l;
	//should be 1.0
	l2= sqrt(P.p11*P.p11+P.p12*P.p12+P.p13*P.p13)/sqrt(P.p21*P.p21+P.p22*P.p22+P.p23*P.p23);
	//display_double(l2,"l2 :  ",&m_status);

	//***************calculate the quality of the solution*****************************************
	rms=0.0;
	for(i=0;i<number;i++)
	{
		nenner=X[i]*P.p31+Y[i]*P.p32+Z[i]*P.p33+P.p34;
		x_mod =(X[i]*P.p11+Y[i]*P.p12+Z[i]*P.p13+P.p14)/nenner;
		y_mod =(X[i]*P.p21+Y[i]*P.p22+Z[i]*P.p23+P.p24)/nenner;
		if(i<first_plane_nr){
			lin_modell_points[i].x=(int)(x_mod/ccd_init.pix_w)+(double)ccd_init.width/2.0;
			lin_modell_points[i].y=(double)ccd_init.height/2.0-(int)(y_mod/ccd_init.pix_h);
		}
		rms+=(float)sqrt((x_mod-x_gem[i])*(x_mod-x_gem[i])+(y_mod-y_gem[i])*(y_mod-y_gem[i]));
	}
	rms=rms/(float)number;

	Plot_points(device, color_image_data, lin_modell_points, ccd_init.height, ccd_init.width, first_plane_nr, 255, 0, 0, 1);  //rgb

	//***************Calculate the Extrinsic and Intrinsic Parameters out of Matrix P***************
	ex_init.phi		=asin(P.p31);
	ex_init.omega	=acos(P.p33/cos(ex_init.phi));

	double r13 = sin(ex_init.phi);
	double r23 = - sin(ex_init.omega)*cos(ex_init.phi);
	double a=  (-P.p11*r23 - P.p12*r13) / (P.p21*r23 - P.p22*r13);

	ex_init.kappa   =-atan( (a*sin(ex_init.phi)*cos(ex_init.omega)-sin(ex_init.omega)*sin(ex_init.phi)*sin(ex_init.phi)-
		                     cos(ex_init.phi)*cos(ex_init.phi)*sin(ex_init.omega))/
							 (a*sin(ex_init.omega)*sin(ex_init.phi)*sin(ex_init.phi) + 
							  a*cos(ex_init.phi)*cos(ex_init.phi)*sin(ex_init.omega)+
							 sin(ex_init.phi)*cos(ex_init.omega)));
	double r11 = cos(ex_init.phi)*cos(ex_init.kappa);
    double r12 = -cos(ex_init.phi)*sin(ex_init.kappa);
	double r21 = cos(ex_init.omega)*sin(ex_init.kappa) + sin(ex_init.omega)*sin(ex_init.phi)*cos(ex_init.kappa);
    double r22 = cos(ex_init.omega)*cos(ex_init.kappa) - sin(ex_init.omega)*sin(ex_init.phi)*sin(ex_init.kappa);
	double r31 = sin(ex_init.omega)*sin(ex_init.kappa) - cos(ex_init.omega)*sin(ex_init.phi)*cos(ex_init.kappa);
	double r32 = sin(ex_init.omega)*cos(ex_init.kappa) + cos(ex_init.omega)*sin(ex_init.phi)*sin(ex_init.kappa);
	double r33 = cos(ex_init.omega)*cos(ex_init.phi);

	in_init.focus = (P.p21*r23 - P.p22*r13)/(r22*r13-r12*r23);
		
	in_init.x_h=(P.p11+in_init.focus*r11)/r13;
	in_init.y_h=(P.p21+in_init.focus*r12)/r13;
		          

	
	//************solve for X_0,Y_0 and Z_0**********************************************************
	n=3;//number of rows
	m=3;//number of columns
	//fill in right hand side r
	*r    =P.p14;
	*(r+1)=P.p24;
	*(r+2)=P.p34;
	//fill in 3x3 matrix A
	*A    = in_init.focus *  r11 - in_init.x_h * r13;
	*(A+1)= in_init.focus *  r21 - in_init.x_h * r23;
	*(A+2)= in_init.focus *  r31 - in_init.x_h * r33;
	*(A+3)= in_init.focus *  r12 - in_init.y_h * r13;
	*(A+4)= in_init.focus *  r22 - in_init.y_h * r23;
	*(A+5)= in_init.focus *  r32 - in_init.y_h * r33;
	*(A+6)= -r13;
	*(A+7)= -r23;
	*(A+8)= -r33;

	result = imsl_d_lin_least_squares_gen (m, n, A, r, 0);

	ex_init.X_0=*result++;
	ex_init.Y_0=*result++;
	ex_init.Z_0=*result;

	free(A);
	free(r);
	return 1;
}




int Lin_calibrate3(CDC *device) 
{
	int i,j=0;
	int n=12;       //Number of the unkown coefficients in matrix A
	int m=2*number; //Number of equation for the coefficients in matrix A
	int  rank;      //rank of matrix

	double * A=(double *)calloc(m*n, sizeof(double));
	double * r=(double *)calloc(m,   sizeof(double)); 
	double *M, *result;
	double pi= imsl_d_constant ("pi",0);
	double *eval, *evec;    //eigen value, eigen vector
	double l,l2;
	double tol=1.0;
	double cond;  
	double x_mod,y_mod,nenner;	//Variables for the estiamtion of the quality of the result


	//*********x_gem and y_gem are values in [mm] with reference to the left upper********
	//*********corner*********************************************************************
	//*********Transform values into coordiante system with origin in image center********
	for(i=0;i<number;i++)
	{
		x_gem[i]=x_gem[i]-(double)ccd_init.width/2.0*ccd_init.pix_w;
		y_gem[i]=(double)ccd_init.height/2.0*ccd_init.pix_h-y_gem[i];
	}

	//*****************************Fill in the matrix A m x n  **********************
	for(i=0;i<number;i++)
	{
		//first row
		*(A+j)    =X[i]; 	
		*(A+j+1)  =Y[i]; 	
		*(A+j+2)  =Z[i]; 	
		*(A+j+3)  =1.0; 
		*(A+j+4)  =0.0; 
		*(A+j+5)  =0.0; 
		*(A+j+6)  =0.0; 
		*(A+j+7)  =0.0;
		*(A+j+8)  =-x_gem[i]* X[i]; 
		*(A+j+9)  =-x_gem[i]* Y[i];	
		*(A+j+10) =-x_gem[i]* Z[i]; 
		*(A+j+11) =-x_gem[i];
		//second row
		*(A+j+12) =0.0; 
		*(A+j+13) =0.0; 
		*(A+j+14) =0.0; 
		*(A+j+15) =0.0;	
		*(A+j+16) =X[i]; 	
		*(A+j+17) =Y[i]; 	
		*(A+j+18) =Z[i];	
		*(A+j+19) =1.0;
		*(A+j+20) =-y_gem[i]* X[i]; 
		*(A+j+21) =-y_gem[i]* Y[i];	
		*(A+j+22) =-y_gem[i]* Z[i]; 
		*(A+j+23) =-y_gem[i];
		j+=24;
	}
	for(i=0;i<m;i++)
		*(r+i)=0.0;
	//*****************************Calculate the rank of A *****************************
	imsl_d_lin_svd_gen (m, n, A, IMSL_RANK, tol, &rank,0);

	//*****************************Calculate M=trans(A)*A ******************************
	//*****************************M is a symetric n*n (12*12) matrix*******************
	M = imsl_d_mat_mul_rect("trans(A)*A",IMSL_A_MATRIX, m, n, A,0);

	//*****************************check the matrix M***********************************
	//**********************************************************************************
	imsl_d_lin_sol_gen (n, M, r, IMSL_CONDITION, &cond, 0);
	imsl_d_lin_svd_gen (n, n, M, IMSL_RANK, tol, &rank,0);

	//*****************************Calculate eigenvalues and ***************************
	//*****************************eigenvectors of M************************************
	eval = imsl_d_eig_sym (n, M, IMSL_VECTORS, &evec, 0);

	P.p11=*(evec+11);	P.p12=*(evec+23); 	P.p13=*(evec+35);  	P.p14=*(evec+47);
	P.p21=*(evec+59); 	P.p22=*(evec+71); 	P.p23=*(evec+83);  	P.p24=*(evec+95);
	P.p31=*(evec+107); 	P.p32=*(evec+119); 	P.p33=*(evec+131); 	P.p34=*(evec+143);


	//*********************find the scaling factor of the eigenvector*****************************
	l=sqrt(P.p31*P.p31+P.p32*P.p32+P.p33*P.p33);

	P.p11=P.p11/l; 	P.p12=P.p12/l; 	P.p13=P.p13/l; 	P.p14=P.p14/l; 
	P.p21=P.p21/l;	P.p22=P.p22/l;	P.p23=P.p23/l;	P.p24=P.p24/l;
	P.p31=P.p31/l;	P.p32=P.p32/l;	P.p33=P.p33/l;	P.p34=P.p34/l;
	//should be 1.0
	l2= sqrt(P.p11*P.p11+P.p12*P.p12+P.p13*P.p13)/sqrt(P.p21*P.p21+P.p22*P.p22+P.p23*P.p23);
	//display_double(l2,"l2 :  ",&m_status);

	//***************calculate the quality of the solution*****************************************
	rms=0.0;
	for(i=0;i<number;i++)
	{
		nenner=X[i]*P.p31+Y[i]*P.p32+Z[i]*P.p33+P.p34;
		x_mod =(X[i]*P.p11+Y[i]*P.p12+Z[i]*P.p13+P.p14)/nenner;
		y_mod =(X[i]*P.p21+Y[i]*P.p22+Z[i]*P.p23+P.p24)/nenner;
		if(i<first_plane_nr){
			lin_modell_points[i].x=(int)(x_mod/ccd_init.pix_w)+(double)ccd_init.width/2.0;
			lin_modell_points[i].y=(double)ccd_init.height/2.0-(int)(y_mod/ccd_init.pix_h);
		}
		rms+=(float)sqrt((x_mod-x_gem[i])*(x_mod-x_gem[i])+(y_mod-y_gem[i])*(y_mod-y_gem[i]));
	}
	rms=rms/(float)number;

	Plot_points(device, color_image_data, lin_modell_points, ccd_init.height, ccd_init.width, first_plane_nr, 255, 0, 0, 1);  //rgb

	//***************Calculate the Extrinsic and Intrinsic Parameters out of Matrix P***************
	ex_init.phi		=asin(P.p31);
	ex_init.omega	=acos(P.p33/cos(ex_init.phi));

	double r13 = sin(ex_init.phi);
	double r23 = - sin(ex_init.omega)*cos(ex_init.phi);
	double r33 = cos(ex_init.omega)*cos(ex_init.phi);
    double k1= (r13/r23);
    double k3= (r13/r33);
	double a= ((P.p11-k3*P.p13)/(P.p11-k1*P.p12)); 
	ex_init.kappa   = atan( (a*k1*sin(ex_init.omega)*sin(ex_init.phi)-a*cos(ex_init.phi)+k3*cos(ex_init.omega)*sin(ex_init.phi)+cos(ex_init.phi))/
		(k3*sin(ex_init.omega)-a*k1*cos(ex_init.omega)) );	
		
		
		
		
		
	double r11 = cos(ex_init.phi)*cos(ex_init.kappa);
    double r12 = -cos(ex_init.phi)*sin(ex_init.kappa);
	double r21 = cos(ex_init.omega)*sin(ex_init.kappa) + sin(ex_init.omega)*sin(ex_init.phi)*cos(ex_init.kappa);
    double r22 = cos(ex_init.omega)*cos(ex_init.kappa) - sin(ex_init.omega)*sin(ex_init.phi)*sin(ex_init.kappa);
	double r31 = sin(ex_init.omega)*sin(ex_init.kappa) - cos(ex_init.omega)*sin(ex_init.phi)*cos(ex_init.kappa);
	double r32 = sin(ex_init.omega)*cos(ex_init.kappa) + cos(ex_init.omega)*sin(ex_init.phi)*sin(ex_init.kappa);
	

	in_init.focus = (P.p21 - k1*P.p22)/(k1*r22 - r12);
		
	in_init.y_h=(P.p22+in_init.focus*r22)/r23;
	
	in_init.s_x=(P.p11-k1*P.p12)/(in_init.focus*(k1*r21-r11));
	in_init.x_h=(P.p11+in_init.focus*in_init.s_x*r11)/r13;

	
	//************solve for X_0,Y_0 and Z_0**********************************************************
	n=3;//number of rows
	m=3;//number of columns
	//fill in right hand side r
	*r    =P.p14;
	*(r+1)=P.p24;
	*(r+2)=P.p34;
	//fill in 3x3 matrix A
	*A    = in_init.s_x * in_init.focus *  r11 - in_init.x_h * r13;
	*(A+1)= in_init.s_x * in_init.focus *  r21 - in_init.x_h * r23;
	*(A+2)= in_init.s_x * in_init.focus *  r31 - in_init.x_h * r33;
	*(A+3)= in_init.focus *  r12 - in_init.y_h * r13;
	*(A+4)= in_init.focus *  r22 - in_init.y_h * r23;
	*(A+5)= in_init.focus *  r32 - in_init.y_h * r33;
	*(A+6)= -r13;
	*(A+7)= -r23;
	*(A+8)= -r33;

	result = imsl_d_lin_least_squares_gen (m, n, A, r, 0);

	ex_init.X_0=*result++;
	ex_init.Y_0=*result++;
	ex_init.Z_0=*result;

	free(A);
	free(r);
	return 1;
}


int Lin_calibrate4(CDC *device) 
{
	int i,j=0;
	int n=12;       //Number of the unkown coefficients in matrix A
	int m=2*number; //Number of equation for the coefficients in matrix A
	int  rank;      //rank of matrix

	double * A=(double *)calloc(m*n, sizeof(double));
	double * r=(double *)calloc(m,   sizeof(double)); 
	double *M, *result;
	double pi= imsl_d_constant ("pi",0);
	double *eval, *evec;    //eigen value, eigen vector
	double l,l2;
	double tol=1.0;
	double cond;  
	double x_mod,y_mod,nenner;	//Variables for the estiamtion of the quality of the result


	//*********x_gem and y_gem are values in [mm] with reference to the left upper********
	//*********corner*********************************************************************
	//*********Transform values into coordiante system with origin in image center********
	for(i=0;i<number;i++)
	{
		x_gem[i]=x_gem[i]-(double)ccd_init.width/2.0*ccd_init.pix_w;
		y_gem[i]=(double)ccd_init.height/2.0*ccd_init.pix_h-y_gem[i];
	}

	//*****************************Fill in the matrix A m x n  **********************
	for(i=0;i<number;i++)
	{
		//first row
		*(A+j)    =X[i]; 	
		*(A+j+1)  =Y[i]; 	
		*(A+j+2)  =Z[i]; 	
		*(A+j+3)  =1.0; 
		*(A+j+4)  =0.0; 
		*(A+j+5)  =0.0; 
		*(A+j+6)  =0.0; 
		*(A+j+7)  =0.0;
		*(A+j+8)  =-x_gem[i]* X[i]; 
		*(A+j+9)  =-x_gem[i]* Y[i];	
		*(A+j+10) =-x_gem[i]* Z[i]; 
		*(A+j+11) =-x_gem[i];
		//second row
		*(A+j+12) =0.0; 
		*(A+j+13) =0.0; 
		*(A+j+14) =0.0; 
		*(A+j+15) =0.0;	
		*(A+j+16) =X[i]; 	
		*(A+j+17) =Y[i]; 	
		*(A+j+18) =Z[i];	
		*(A+j+19) =1.0;
		*(A+j+20) =-y_gem[i]* X[i]; 
		*(A+j+21) =-y_gem[i]* Y[i];	
		*(A+j+22) =-y_gem[i]* Z[i]; 
		*(A+j+23) =-y_gem[i];
		j+=24;
	}
	for(i=0;i<m;i++)
		*(r+i)=0.0;
	//*****************************Calculate the rank of A *****************************
	imsl_d_lin_svd_gen (m, n, A, IMSL_RANK, tol, &rank,0);

	//*****************************Calculate M=trans(A)*A ******************************
	//*****************************M is a symetric n*n (12*12) matrix*******************
	M = imsl_d_mat_mul_rect("trans(A)*A",IMSL_A_MATRIX, m, n, A,0);

	//*****************************check the matrix M***********************************
	//**********************************************************************************
	imsl_d_lin_sol_gen (n, M, r, IMSL_CONDITION, &cond, 0);
	imsl_d_lin_svd_gen (n, n, M, IMSL_RANK, tol, &rank,0);

	//*****************************Calculate eigenvalues and ***************************
	//*****************************eigenvectors of M************************************
	eval = imsl_d_eig_sym (n, M, IMSL_VECTORS, &evec, 0);

	P.p11=*(evec+11);	P.p12=*(evec+23); 	P.p13=*(evec+35);  	P.p14=*(evec+47);
	P.p21=*(evec+59); 	P.p22=*(evec+71); 	P.p23=*(evec+83);  	P.p24=*(evec+95);
	P.p31=*(evec+107); 	P.p32=*(evec+119); 	P.p33=*(evec+131); 	P.p34=*(evec+143);


	//*********************find the scaling factor of the eigenvector*****************************
	l=sqrt(P.p31*P.p31+P.p32*P.p32+P.p33*P.p33);

	P.p11=P.p11/l; 	P.p12=P.p12/l; 	P.p13=P.p13/l; 	P.p14=P.p14/l; 
	P.p21=P.p21/l;	P.p22=P.p22/l;	P.p23=P.p23/l;	P.p24=P.p24/l;
	P.p31=P.p31/l;	P.p32=P.p32/l;	P.p33=P.p33/l;	P.p34=P.p34/l;
	//should be 1.0
	l2= sqrt(P.p11*P.p11+P.p12*P.p12+P.p13*P.p13)/sqrt(P.p21*P.p21+P.p22*P.p22+P.p23*P.p23);
	//display_double(l2,"l2 :  ",&m_status);

	//***************calculate the quality of the solution*****************************************
	rms=0.0;
	for(i=0;i<number;i++)
	{
		nenner=X[i]*P.p31+Y[i]*P.p32+Z[i]*P.p33+P.p34;
		x_mod =(X[i]*P.p11+Y[i]*P.p12+Z[i]*P.p13+P.p14)/nenner;
		y_mod =(X[i]*P.p21+Y[i]*P.p22+Z[i]*P.p23+P.p24)/nenner;
		if(i<first_plane_nr){
			lin_modell_points[i].x=(int)(x_mod/ccd_init.pix_w)+(double)ccd_init.width/2.0;
			lin_modell_points[i].y=(double)ccd_init.height/2.0-(int)(y_mod/ccd_init.pix_h);
		}
		rms+=(float)sqrt((x_mod-x_gem[i])*(x_mod-x_gem[i])+(y_mod-y_gem[i])*(y_mod-y_gem[i]));
	}
	rms=rms/(float)number;

	Plot_points(device, color_image_data, lin_modell_points, ccd_init.height, ccd_init.width, first_plane_nr, 255, 0, 0, 1);  //rgb

	//***************Calculate the Extrinsic and Intrinsic Parameters out of Matrix P***************
	ex_init.phi		=asin(P.p31);
	ex_init.omega	=acos(P.p33/cos(ex_init.phi));

	double r13 = sin(ex_init.phi);
	double r23 = - sin(ex_init.omega)*cos(ex_init.phi);
	double r33 = cos(ex_init.omega)*cos(ex_init.phi);
    double k1= (r13/r23);
    double a = cos(ex_init.omega);
	double b = sin(ex_init.omega)*sin(ex_init.phi);
	double c = ((P.p21-k1*P.p22)/(P.p11-k1*P.p12)); 
	ex_init.kappa   = atan( (k1*a+c*cos(ex_init.phi)-c*k1*b)/
		(c*k1*a+k1*b-cos(ex_init.phi)) );	
		
		
		
		
		
	double r11 = cos(ex_init.phi)*cos(ex_init.kappa);
    double r12 = -cos(ex_init.phi)*sin(ex_init.kappa);
	double r21 = cos(ex_init.omega)*sin(ex_init.kappa) + sin(ex_init.omega)*sin(ex_init.phi)*cos(ex_init.kappa);
    double r22 = cos(ex_init.omega)*cos(ex_init.kappa) - sin(ex_init.omega)*sin(ex_init.phi)*sin(ex_init.kappa);
	double r31 = sin(ex_init.omega)*sin(ex_init.kappa) - cos(ex_init.omega)*sin(ex_init.phi)*cos(ex_init.kappa);
	double r32 = sin(ex_init.omega)*cos(ex_init.kappa) + cos(ex_init.omega)*sin(ex_init.phi)*sin(ex_init.kappa);
	

	in_init.focus = (P.p21 - k1*P.p22)/(k1*r22 - r12);
		
	in_init.y_h=(P.p21+in_init.focus*r12)/r13;
	
	in_init.x_h=(P.p12+in_init.focus*r21)/r23;

	
	//************solve for X_0,Y_0 and Z_0**********************************************************
	n=3;//number of rows
	m=3;//number of columns
	//fill in right hand side r
	*r    =P.p14;
	*(r+1)=P.p24;
	*(r+2)=P.p34;
	//fill in 3x3 matrix A
	*A    = in_init.focus *  r11 - in_init.x_h * r13;
	*(A+1)= in_init.focus *  r21 - in_init.x_h * r23;
	*(A+2)= in_init.focus *  r31 - in_init.x_h * r33;
	*(A+3)= in_init.focus *  r12 - in_init.y_h * r13;
	*(A+4)= in_init.focus *  r22 - in_init.y_h * r23;
	*(A+5)= in_init.focus *  r32 - in_init.y_h * r33;
	*(A+6)= -r13;
	*(A+7)= -r23;
	*(A+8)= -r33;

	result = imsl_d_lin_least_squares_gen (m, n, A, r, 0);

	ex_init.X_0=*result++;
	ex_init.Y_0=*result++;
	ex_init.Z_0=*result;

	free(A);
	free(r);
	return 1;
}