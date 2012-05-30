#include "stdafx.h"
#include <stdio.h>
#include <fstream.h>
#include <math.h>
#include <imsl.h>
#include "niimaq.h"
#include "parameters.h"
#include "structures.h"


//*******This are the matched (by routine "Sort_Points")********************
//*******points*************************************************************
extern double x_gem[MAX_MATCH], y_gem[MAX_MATCH];
extern double X[MAX_MATCH], Y[MAX_MATCH], Z[MAX_MATCH];
extern int number;
extern int first_plane_nr;
extern CCD_par ccd_init;

extern char path[255];  

int Read_image(char *file_name, uInt8 * mem_location, int h, int w)
{
	FILE *file = NULL;
	file= (FILE *) fopen (file_name,"r");

	if(file == NULL)         //File non existing
		return -1;
	else 
	{
		fread(mem_location, sizeof(char), w*h, file);
		fclose(file);

		return 0;    //No Error
	}

}



void Read_cam_param(Extrinsic_par *ex, Intrinsic_par *in, char *file_name_cam)
{
	FILE *file= fopen(file_name_cam,"r");

	char a=0;
	while(a != '\n')
		a=fgetc(file);

	a=fgetc(file);
	while(a != '\n')
		a=fgetc(file);

	fscanf(file,"%lf",&ex->X_0);
	fscanf(file,"%lf",&ex->Y_0);
	fscanf(file,"%lf",&ex->Z_0);
	fscanf(file,"%lf",&ex->omega);
	fscanf(file,"%lf",&ex->phi);
	fscanf(file,"%lf",&ex->kappa);
	a=fgetc(file);
	a=fgetc(file);
	while(a != '\n')
		a=fgetc(file);

	a=fgetc(file);
	while(a != '\n')
		a=fgetc(file);

	fscanf (file,"%lf",&in->focus);
	fscanf (file,"%lf",&in->x_h);
	fscanf (file,"%lf",&in->y_h);
	fscanf (file,"%lf",&in->s_x);
	fscanf (file,"%lf",&in->theta);
	fscanf (file,"%lf",&in->k1);
	fscanf (file,"%lf",&in->k2);
	fscanf (file,"%lf",&in->p1);
	fscanf (file,"%lf",&in->p2);

	fclose(file);
}


void Read_P_matrix(P_matrix *P, char *file_name){

	ifstream indat1;

	char a[255];

	indat1.open(file_name, ios::nocreate);
	indat1.getline(a,71,'\n');
	indat1.getline(a,71,'\n');
	P->p11=atof(a);
	indat1.getline(a,71,'\n');
	P->p12=atof(a);
	indat1.getline(a,71,'\n');
	P->p13=atof(a);
	indat1.getline(a,71,'\n');
	P->p14=atof(a);

	indat1.getline(a,71,'\n');
	P->p21=atof(a);
	indat1.getline(a,71,'\n');
	P->p22=atof(a);
	indat1.getline(a,71,'\n');
	P->p23=atof(a);
	indat1.getline(a,71,'\n');
	P->p24=atof(a);

	indat1.getline(a,71,'\n');
	P->p31=atof(a);
	indat1.getline(a,71,'\n');
	P->p32=atof(a);
	indat1.getline(a,71,'\n');
	P->p33=atof(a);
	indat1.getline(a,71,'\n');
	P->p34=atof(a);

	indat1.close();
}



void Get_parameters(P_matrix P,Extrinsic_par *ex, Intrinsic_par *in)
{
	int n=3;//number of rows
	int m=3;//number of columns

	double *result;
	double * A=(double *)calloc(m*n, sizeof(double));
	double * r=(double *)calloc(m,   sizeof(double)); 

	//***************Calculate the Extrinsic and Intrinsic Parameters out of Matrix P***************
	ex->phi		=asin(P.p31);
	//ex->omega	=asin(-P.p32/cos(ex->phi)); did not work; hell knows why
	ex->omega	=acos(P.p33/cos(ex->phi));
	ex->kappa	=atan(-P.p21/P.p11);

	in->focus	= -P.p11/(cos(ex->phi)*cos(ex->kappa)); 
	
	//************solve for X_0,Y_0 and Z_0**********************************************************
	n=3;//number of rows
	m=3;//number of columns
	//fill in right hand side r
	*r    =P.p14;
	*(r+1)=P.p24;
	*(r+2)=P.p34;
	//fill in 3x3 matrix A
	*A    = in->focus *  cos(ex->phi)*cos(ex->kappa);
	*(A+1)= in->focus * (cos(ex->omega)*sin(ex->kappa)+sin(ex->omega)*sin(ex->phi)*cos(ex->kappa));
	*(A+2)= in->focus * (sin(ex->omega)*sin(ex->kappa)-cos(ex->omega)*sin(ex->phi)*cos(ex->kappa));
	*(A+3)=-in->focus *  cos(ex->phi)*sin(ex->kappa);
	*(A+4)= in->focus * (cos(ex->omega)*cos(ex->kappa)-sin(ex->omega)*sin(ex->phi)*sin(ex->kappa));
	*(A+5)= in->focus * (sin(ex->omega)*cos(ex->kappa)+cos(ex->omega)*sin(ex->phi)*sin(ex->kappa));
	*(A+6)= - sin(ex->phi);
	*(A+7)=   sin(ex->omega)*cos(ex->phi);
	*(A+8)= - cos(ex->omega)*cos(ex->phi);

	result = imsl_d_lin_least_squares_gen (m, n, A, r, 0);

	ex->X_0=*result++;
	ex->Y_0=*result++;
	ex->Z_0=*result;

	free(A);
	free(r);
}

void Get_parameters3(P_matrix P,Extrinsic_par *ex, Intrinsic_par *in)
{
	int n=3;//number of rows
	int m=3;//number of columns

	double *result;
	double * A=(double *)calloc(m*n, sizeof(double));
	double * r=(double *)calloc(m,   sizeof(double)); 

	//***************Calculate the Extrinsic and Intrinsic Parameters out of Matrix P***************
	ex->phi		=asin(P.p31);
	ex->omega	=acos(P.p33/cos(ex->phi));

	double r13 =   sin(ex->phi);
	double r23 = - sin(ex->omega)*cos(ex->phi);
	double r33 =   cos(ex->omega)*cos(ex->phi);
    double k1= (r13/r23);
    double k3= (r13/r33);
	double a= ((P.p11-k3*P.p13)/(P.p11-k1*P.p12)); 
	ex->kappa   = atan( (a*k1*sin(ex->omega)*sin(ex->phi)-a*cos(ex->phi)+k3*cos(ex->omega)*sin(ex->phi)+cos(ex->phi))/
		(k3*sin(ex->omega)-a*k1*cos(ex->omega)) );	
			
	double r11 = cos(ex->phi) * cos(ex->kappa);
    double r12 =-cos(ex->phi) * sin(ex->kappa);
	double r21 = cos(ex->omega)*sin(ex->kappa) + sin(ex->omega)*sin(ex->phi)*cos(ex->kappa);
    double r22 = cos(ex->omega)*cos(ex->kappa) - sin(ex->omega)*sin(ex->phi)*sin(ex->kappa);
	double r31 = sin(ex->omega)*sin(ex->kappa) - cos(ex->omega)*sin(ex->phi)*cos(ex->kappa);
	double r32 = sin(ex->omega)*cos(ex->kappa) + cos(ex->omega)*sin(ex->phi)*sin(ex->kappa);
	

	in->focus = (P.p21 - k1*P.p22)/(k1*r22 - r12);
		
	in->y_h=(P.p22+in->focus*r22)/r23;
	
	in->s_x=(P.p11-k1*P.p12)/(in->focus*(k1*r21-r11));
	in->x_h=(P.p11+in->focus*in->s_x*r11)/r13;

	
	//************solve for X_0,Y_0 and Z_0**********************************************************
	n=3;//number of rows
	m=3;//number of columns
	//fill in right hand side r
	*r    =P.p14;
	*(r+1)=P.p24;
	*(r+2)=P.p34;
	//fill in 3x3 matrix A
	*A    = in->s_x * in->focus *  r11 - in->x_h * r13;
	*(A+1)= in->s_x * in->focus *  r21 - in->x_h * r23;
	*(A+2)= in->s_x * in->focus *  r31 - in->x_h * r33;
	*(A+3)= in->focus *  r12 - in->y_h * r13;
	*(A+4)= in->focus *  r22 - in->y_h * r23;
	*(A+5)= in->focus *  r32 - in->y_h * r33;
	*(A+6)= -r13;
	*(A+7)= -r23;
	*(A+8)= -r33;

	result = imsl_d_lin_least_squares_gen (m, n, A, r, 0);

	ex->X_0=*result++;
	ex->Y_0=*result++;
	ex->Z_0=*result;

	free(A);
	free(r);
}





void display(int number, char text[], CEdit *status)
{
	char buffer[256];
	
	status->ReplaceSel(text,FALSE);

	if(number!=-99){
		itoa(number,buffer,10);
		status->ReplaceSel(buffer, FALSE);
	}
	status->ReplaceSel("\r\n",FALSE);
}


void display_double(double number, char text[], CEdit *status)
{
	char buffer[256];
	
	status->ReplaceSel(text,FALSE);

	if(number!=-99){
		gcvt(number,5,buffer);
		status->ReplaceSel(buffer, FALSE);
	}
	status->ReplaceSel("\r\n",FALSE);
}





int Rd_matched_points()
{
		
	FILE *data= NULL;
	int iref, ical, i;
	char file_name[255];
	
//*********Read the measured control points  ***********************************************
//*********The coordinate system has its origin in the left upper corner********************
//*********and the the values are given in pixels*******************************************
	sprintf(file_name,path);
	strcat(file_name,"match.dat");
	data = (FILE *)fopen(file_name,"r"); 	
	if(data==NULL)
		return -1;

	number = 0;         
	fscanf (data, "%d",&first_plane_nr);

	while(fscanf (data, "%d %d %lf %lf %lf %lf %lf",
			&iref,&ical, &X[number], &Y[number], &Z[number], &x_gem[number], &y_gem[number])  != EOF)
	{
		number++;
		if(number > MAX_MATCH)
			return 1;
	}
	fclose (data);
//*********End Read the measured control points  (in Pixel coordintates)******************
//	f_res = (double*)calloc(2*number,sizeof(double));
//*********Transform points into metric coordinates [mm] with origin in left upper corner)*
	for(i=0;i < number; i++)
	{
		x_gem[i] = x_gem[i]*ccd_init.pix_w;
		y_gem[i] = y_gem[i]*ccd_init.pix_h;
	}
//*********End Transform points into CCD coordinate system********************************
	return 0;
}



int Write_3D_points(GlobalPoint* points, int nr){

	int i;

	FILE *fp=fopen("points.3D","wb");
	fprintf(fp,"%i\n",nr);

	for(i=1;i<=nr;i++)
		fprintf(fp,"%10.3lf%10.3lf%10.3lf\n", points[i].X, points[i].Y, points[i].Z);

	fclose(fp);
	return 0;
}



int Read_3D_points(GlobalPoint* points, char *file){

	int i, nr;

	FILE *data= NULL;
	data = (FILE *)fopen(file,"r"); 	
	if(data==NULL)
		return -2;
      
	fscanf (data,"%d",&nr);

	for(i=0; i<nr; i++)
		fscanf (data, "%lf %lf %lf", &points[i].X, &points[i].Y, &points[i].Z);

	return nr;
}



void Read_table(char *file, Vec2 *table, CProgressCtrl* bar)
{
    FILE *in_file = NULL;
	int i,j;
	char buffer[255];
	strcpy(buffer, file);
	char *file_name=buffer;

	j = strlen(file_name);
	file_name = file_name+j;
	while(*file_name != '.')
		file_name--;

	strcpy(file_name, ".lut");
	file_name+=4-j;

    in_file = fopen (file_name,"r");
 

	bar->SetRange(0,480);
	bar->SetStep(1);

	for(j=0;j<480;j++)
	{
		for(i=0;i<640;i++)
			fscanf(in_file, "%lf %lf", &table[i+j*640].x, &table[i+j*640].y);

		bar->StepIt();
	}

	bar->SetPos(0);
	fclose(in_file);
}