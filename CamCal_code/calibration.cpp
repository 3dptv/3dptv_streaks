#include "stdafx.h"
#include <stdio.h>
#include <fstream.h>
#include <imsl.h>
#include <math.h>
#include "nitypes.h"
#include "parameters.h"
#include "structures.h"
#include "calibration.h"
#include "plot_functions.h"

#include <signal.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct matches { int refind, calind; } ;

CamPoint modell_points[MAX_MATCH];

extern uInt8*     color_image_data;
extern int plane_index;
extern int var[17];
extern Extrinsic_par ex_init;
extern Intrinsic_par in_init;
extern CCD_par       ccd_init;
extern Plane_par     plane_init; 
extern float rms;
//*******This are the matched (by routine "Sort_Points")********************
//*******points*************************************************************
extern int number;
extern int first_plane_nr; 
extern double x_gem[MAX_MATCH], y_gem[MAX_MATCH];
extern double X[MAX_MATCH], Y[MAX_MATCH], Z[MAX_MATCH];

extern char   path[255] ;

int function=1;


int Calibrate(CDC *device)
{
	int i,j, n,m;
	long code;
	double residual=0.0;
	double pi;
	double *f_res;
	double *xguess;
	double *result;


	pi= imsl_d_constant ("pi",0);

	//initial values for the itteration
	double init[15] = {	ex_init.X_0, 
						ex_init.Y_0, 
						ex_init.Z_0,
						ex_init.omega, 
						ex_init.phi, 
						ex_init.kappa,
						in_init.focus,                 
						in_init.x_h, 
						in_init.y_h, 
						in_init.s_x, 
						in_init.theta,
						in_init.k1,  
						in_init.k2,	
						in_init.p1,  
						in_init.p2};  	
	

	n=0;
	for(i=0;i<15;i++)
		{if(var[i]) n++;}

	xguess = (double*)calloc(n,sizeof(double));

	j=0;
	for(i=0;i<15;i++)
	{
		if(var[i]==1)
			xguess[j++]=init[i];
	}

	f_res = (double*)calloc(2*number,sizeof(double));

	if(function==1)
		m=2*number;  // n=15;     //m: Number of Functions   n: Number of Variables  n<=m
	else if(function==2)
		m=number;

	//non-lineare optimation: Marquard-Levenberg Algorithmus, 
	result = imsl_d_nonlin_least_squares(fcn_cal, m, n, IMSL_XGUESS, xguess,
										 IMSL_MAX_ITN, 100000,0); //IMSL_RETURN_USER,  x[],
									
	code = imsl_error_code ( );

	fcn_cal(m, n, result, f_res);

	Plot_points(device, color_image_data, modell_points, ccd_init.height, ccd_init.width, first_plane_nr, 255, 0, 0, 1);

	if(function==1){
		for(i=0; i<number; i++)
			residual += sqrt(f_res[2*i] * f_res[2*i] + f_res[2*i+1] *f_res[2*i+1]);	
	}
	else if(function==2){
		for(i=0; i<number; i++)
			residual += f_res[i];	
	}

	rms = (float)residual/(float)number;

	//Results
	if(var[0]) ex_init.X_0   = *result++; 
    if(var[1]) ex_init.Y_0   = *result++;
	if(var[2]) ex_init.Z_0   = *result++;
	if(var[3]) ex_init.omega = *result++;
	if(var[4]) ex_init.phi   = *result++;
	if(var[5]) ex_init.kappa = *result++;
	if(var[6]) in_init.focus = *result++;                 
	if(var[7]) in_init.x_h   = *result++;   
	if(var[8]) in_init.y_h   = *result++; 
	if(var[9]) in_init.s_x   = *result++;   
	if(var[10])in_init.theta = *result++;
	if(var[11])in_init.k1    = *result++;   
	if(var[12])in_init.k2    = *result++;	
	if(var[13])in_init.p1    = *result++;   
	if(var[14])in_init.p2    = *result;  

	free(f_res);
	return code;
}
 




void fcn_cal(int m, int n, double x[], double f[])
{
//this function includes shear, scaling, two terms of the radial, two terms********
//of the tangential and the optical point shifting*********************************
//the x and y directions are optimized differently!********************************
	int i,j;
	double x_b,y_b,x_c,y_c;
	double x_m,y_m;
	double r;
	double nenner;
	double X_0,Y_0,Z_0,omega,phi,kappa,focus,x_h,y_h,s_x,theta;
	double k1,k2,p1,p2;
	Matrix R;

	j=0;
	if(var[0]) X_0=x[j++];		else X_0=ex_init.X_0;
	if(var[1]) Y_0=x[j++]; 		else Y_0=ex_init.Y_0;
	if(var[2]) Z_0=x[j++];  	else Z_0=ex_init.Z_0;
	if(var[3]) omega=x[j++];	else omega=ex_init.omega;
	if(var[4]) phi=x[j++];   	else phi  =ex_init.phi;
	if(var[5]) kappa=x[j++];	else kappa=ex_init.kappa;
	if(var[6]) focus=x[j++];	else focus=in_init.focus;
	if(var[7]) x_h=x[j++];  	else x_h=in_init.x_h;
	if(var[8]) y_h=x[j++];   	else y_h=in_init.y_h;
	if(var[9]) s_x=x[j++];   	else s_x=in_init.s_x;
	if(var[10]) theta=x[j++];	else theta=in_init.theta;
	if(var[11]) k1=x[j++];   	else k1=in_init.k1;
	if(var[12]) k2=x[j++];   	else k2=in_init.k2;
	if(var[13]) p1=x[j++]; 		else p1=in_init.p1;
	if(var[14]) p2=x[j++];		else p2=in_init.p2;

	//Calculation of the Rotationsmatrix 
	//The Matrix is applied as following:  m : point in global coordinates
	//                                     m': point in cam1   coordinates
	//                       t=X_0, Y_0, Z_0 : position of the camera in global coordinates
	//      m'=transpose(R) (m - t)     
	R.a11 =  cos(phi)*cos(kappa);
	R.a12 = -cos(phi)*sin(kappa);
	R.a13 =  sin(phi);
	R.a21 =  cos(omega)*sin(kappa) + sin(omega)*sin(phi)*cos(kappa);
	R.a22 =  cos(omega)*cos(kappa) - sin(omega)*sin(phi)*sin(kappa);
	R.a23 = -sin(omega)*cos(phi);  
	R.a31 =  sin(omega)*sin(kappa) - cos(omega)*sin(phi)*cos(kappa);
	R.a32 =  sin(omega)*cos(kappa) + cos(omega)*sin(phi)*sin(kappa);
	R.a33 =  cos(omega)*cos(phi);


	for(i=0; i < number; i++)
	{
		nenner =  R.a13*(X[i]- X_0)+R.a23*(Y[i]-Y_0)+R.a33*(Z[i]- Z_0) ;

		//Bildkoordinaten 
	   	x_b   =x_h  - focus * ( R.a11*(X[i]- X_0)+R.a21*(Y[i]-Y_0)+R.a31*(Z[i]-Z_0) ) / nenner;
			                              
	   	y_b   =y_h  - focus * ( R.a12*(X[i]- X_0)+R.a22*(Y[i]-Y_0)+R.a32*(Z[i]-Z_0) ) / nenner;

		//Verzerrungskorrektur
		r=sqrt(x_b*x_b+y_b*y_b);
		x_c=x_b + x_b * (k1*r*r + k2*r*r*r*r) +p1*(r*r +2.0*x_b*x_b) + 2.0*p2*x_b*y_b;
		y_c=y_b + y_b * (k1*r*r + k2*r*r*r*r) +p2*(r*r +2.0*y_b*y_b) + 2.0*p1*x_b*y_b;	
		
		//shearing and sretching  (x_m : metrisch, coordinaten: (0,0)
		x_m = s_x * x_c - sin(theta) * y_c + (double)ccd_init.width/2.0 * ccd_init.pix_w;
		y_m = (double)ccd_init.height/2.0*ccd_init.pix_h - cos(theta) * y_c;
		

		f[2*i]  =  x_m  - x_gem[i] ;
		f[2*i+1]=  y_m  - y_gem[i] ;
		if(i<first_plane_nr)
		{
			modell_points[i].x=(int)(x_m/ccd_init.pix_w);
			modell_points[i].y=(int)(y_m/ccd_init.pix_h);
		}
	}
}


void fcn_cal_r(int m, int n, double x[], double f[])
{
//this function includes shear, scaling, two terms of the radial, two terms********
//of the tangential and the optical point shifting*********************************
//********************************
	int i,j;
	double x_b,y_b,x_c,y_c;
	double x_m,y_m;
	double r;
	double nenner;

	double X_0,Y_0,Z_0,omega,phi,kappa,focus,x_h,y_h,s_x,theta;
	double k1,k2,p1,p2;
	Matrix R;

	j=0;
	if(var[0]) X_0=x[j++];		else X_0=ex_init.X_0;
	if(var[1]) Y_0=x[j++]; 		else Y_0=ex_init.Y_0;
	if(var[2]) Z_0=x[j++];  	else Z_0=ex_init.Z_0;
	if(var[3]) omega=x[j++];	else omega=ex_init.omega;
	if(var[4]) phi=x[j++];   	else phi  =ex_init.phi;
	if(var[5]) kappa=x[j++];	else kappa=ex_init.kappa;
	if(var[6]) focus=x[j++];	else focus=in_init.focus;
	if(var[7]) x_h=x[j++];  	else x_h=in_init.x_h;
	if(var[8]) y_h=x[j++];   	else y_h=in_init.y_h;
	if(var[9]) s_x=x[j++];   	else s_x=in_init.s_x;
	if(var[10]) theta=x[j++];	else theta=in_init.theta;
	if(var[11]) k1=x[j++];   	else k1=in_init.k1;
	if(var[12]) k2=x[j++];   	else k2=in_init.k2;
	if(var[13]) p1=x[j++]; 		else p1=in_init.p1;
	if(var[14]) p2=x[j++];		else p2=in_init.p2;

	//Calculation of the rotation matrix
	R.a11 =  cos(phi)*cos(kappa);
	R.a12 = -cos(phi)*sin(kappa);
	R.a13 =  sin(phi);
	R.a21 =  cos(omega)*sin(kappa) + sin(omega)*sin(phi)*cos(kappa);
	R.a22 =  cos(omega)*cos(kappa) - sin(omega)*sin(phi)*sin(kappa);
	R.a23 = -sin(omega)*cos(phi);  
	R.a31 =  sin(omega)*sin(kappa) - cos(omega)*sin(phi)*cos(kappa);
	R.a32 =  sin(omega)*cos(kappa) + cos(omega)*sin(phi)*sin(kappa);
	R.a33 =  cos(omega)*cos(phi);


	for(i=0; i < number; i++)
	{
		nenner =  R.a13*(X[i]- X_0)+R.a23*(Y[i]-Y_0)+R.a33*(Z[i]- Z_0) ;

		//Bildkoordinaten 
	   	x_b   =x_h  - focus * ( R.a11*(X[i]- X_0)+R.a21*(Y[i]-Y_0)+R.a31*(Z[i]-Z_0) ) / nenner;
			                              
	   	y_b   =y_h  - focus * ( R.a12*(X[i]- X_0)+R.a22*(Y[i]-Y_0)+R.a32*(Z[i]-Z_0) ) / nenner;

		//Verzerrungskorrektur
		r=sqrt(x_b*x_b+y_b*y_b);
		x_c=x_b + x_b * (k1*r*r + k2*r*r*r*r) +p1*(r*r +2.0*x_b*x_b) + 2.0*p2*x_b*y_b;
		y_c=y_b + y_b * (k1*r*r + k2*r*r*r*r) +p2*(r*r +2.0*y_b*y_b) + 2.0*p1*x_b*y_b;	
		
		//shearing and sretching  (x_m : metrisch, coordinaten: (0,0)
		x_m = s_x * x_c - sin(theta) * y_c + (double)ccd_init.width/2.0 * ccd_init.pix_w;
		y_m = (double)ccd_init.height/2.0*ccd_init.pix_h - cos(theta) * y_c;
		

		f[i]  =  sqrt( (x_m -x_gem[i])*(x_m-x_gem[i]) +(y_m-y_gem[i])*(y_m-y_gem[i]));

		if(i<first_plane_nr)
		{
			modell_points[i].x=(int)(x_m/ccd_init.pix_w);
			modell_points[i].y=(int)(y_m/ccd_init.pix_h);
		}
	}
}



int Sort_points (int ref_nr, int x, int y, char *global_file_name, int first_plane) {

   FILE *ref= NULL, *cal= NULL, *outfile= NULL;
   char file_name[255];
   int i,j,k,l,m, oldcal,newcal, minind, iref, ical;
   int nptr, nptc, npt;
   int found=0;
   float mindist[4];
   int newref[4];
   float dx1, dx2, dy1, dy2, len[4], caldist, len1, len2, cphi, minlen;

   float *xref, *yref, *zref, *xcal, *ycal;
   unsigned char *reftouched, *caltouched;
   struct matches *match;


//*******Read the file with the global data****************************************
   ref= (FILE *) fopen (global_file_name,"r");
   if(ref==NULL)
	   return -1;
   fscanf (ref,"%d",&nptr);


   xref= (float *) calloc (nptr,sizeof(float));
   yref= (float *) calloc (nptr,sizeof(float));
   zref= (float *) calloc (nptr,sizeof(float));
   reftouched= (unsigned char *) calloc (nptr,sizeof(char));

   for (i=0;i<nptr;i++) fscanf (ref,"%d %f %f %f",&j,&xref[i],&yref[i],&zref[i]);
   fclose (ref);
//*******End Read the file with the global data************************************

//*******Read the file with the local  data****************************************
   sprintf(file_name,path);
   strcat(file_name,"local_us_plane");
   j=strlen(file_name);
   sprintf(file_name+j, "%d.dat", plane_index);

   cal= (FILE *) fopen (file_name,"r");
   if(cal==NULL)
	   return -2;
   fscanf (cal,"%d",&nptc);

   xcal= (float *) calloc (nptc,sizeof(float));
   ycal= (float *) calloc (nptc,sizeof(float));
   caltouched= (unsigned char *) calloc (nptc,sizeof(char));
  
   for (i=0;i<nptc;i++) fscanf (cal,"%d %f %f",&j,&xcal[i],&ycal[i]);
   fclose (cal);
//*******End Read the file with the local  data************************************

   npt= (nptr < nptc) ? nptr : nptc;

   match= (struct matches *) calloc (npt,sizeof(struct matches));

 
//*******Find the Point with the x,y coordinates in the local_us list**************
   for(i=0; i<nptc; i++)
   {
		if(fabs(xcal[i] - x) < 5 && fabs(ycal[i] - y) < 5)
		{
			found=1;
			break;
		}
   }
   if(found==0)
	   return 0;
    //global
	match[0].refind = ref_nr;
	//local
	match[0].calind = i;
//*******End Find the Point with the x,y cooerdiates in the local_us list**********

   reftouched[match[0].refind]= 1;
   caltouched[match[0].calind]= 1;

   npt= 0; k= -1;

   while (k < npt) {
      k++;
      /* find nearest untouched neighbour for list of identified match points; use calibration image */
	  caldist= 1.e10;
	  newcal= -1;
      for (l=0;l<=npt;l++) {
		/* loop over all identified matchpoints ... */
        for (m=0;m<nptc;m++) {
			/* loop over all unused calibration points */
			if (caltouched[m] == 0) {
				/* compute distance for current calibration point pair */
				dx1= xcal[m] - xcal[match[l].calind]; dy1= ycal[m] - ycal[match[l].calind];
				len1= (dx1*dx1 + dy1*dy1);
				if (len1 < caldist) {
					/* new nearest neighbour found ... */
					caldist= len1;
					newcal= m; oldcal= l;
				}
			}
 		}
	}
	if (newcal >= 0) {
	   /* new neighbour found: find corresponding neigbour in reference image */

	   /* first: get 4 nearest neighbours as candidates */
	   for (m=0;m<4;m++) {
		mindist[m]= 1.e10;
		newref[m]= -1;
	   }

  	   for (m=0;m<nptr;m++) {
		/* loop over all untouched points in reference image */
		if (reftouched[m] == 0) {

		   /* compute distance for current reference point pair */
		   dx1= xref[m] - xref[match[oldcal].refind]; dy1= yref[m] - yref[match[oldcal].refind];
		   len1= (float)sqrt(dx1*dx1 + dy1*dy1);

		   /* sort into list of 4 nearest neighbours */
               if (len1 < mindist[3]) {
		      if (len1 >= mindist[2]) {
			   mindist[3]= len1; newref[3]= m;
		      } else if (len1 >= mindist[1]) {
			   mindist[3]= mindist[2]; newref[3]= newref[2];
   			   mindist[2]= len1; newref[2]= m;
		      } else if (len1 >= mindist[0]) {
			   mindist[3]= mindist[2]; newref[3]= newref[2]; 
		   	   mindist[2]= mindist[1]; newref[2]= newref[1]; 
			   mindist[1]= len1; newref[1]= m;
		      } else {
			   mindist[3]= mindist[2]; newref[3]= newref[2]; 
			   mindist[2]= mindist[1]; newref[2]= newref[1]; 
			   mindist[1]= mindist[0]; newref[1]= newref[0]; 
			   mindist[0]= len1; newref[0]= m; 
		      }
		   }
            }
         }

	   /* next: find best match based on angles */

	   for (m=0;m<4;m++) {
		/* loop over all 4 possible neighbours */
            if (newref[m] >= 0) {
		   /* eliminate candidate if angular deviation between corresponding segments in
			reference and calibration images would be too large */ 
		   dx1= xref[newref[m]] - xref[match[oldcal].refind];
		   dy1= yref[newref[m]] - yref[match[oldcal].refind];
		   len1= (float)sqrt(dx1*dx1 + dy1*dy1);
		   dx2= xcal[newcal] - xcal[match[oldcal].calind];
		   dy2= ycal[newcal] - ycal[match[oldcal].calind];
		   len2= (float)sqrt(dx2*dx2 + dy2*dy2);
		   cphi= (dx1*dx2 + dy1*dy2) / (len1*len2);
		   if (cphi < 0.9) newref[m]= -1;
   		   len[m]= len1;
            }
	   }

	   /* from all neighbours with acceptable angles, pick the closest */
	   minlen= 1.e10; minind= -1;
         for (m=0;m<4;m++) {
		if ((newref[m] >= 0) && (len[m] < minlen)) {
               minlen= len[m];
		   minind= m;
		}
	   }

	   /* finally: have found a new match ! */
         if (minind >= 0) {
            npt ++;
            match[npt].refind= newref[minind]; reftouched[newref[minind]]= 1;
            match[npt].calind= newcal; caltouched[newcal]= 1;
         }
      }
   }

   sprintf(file_name,path);
   strcat(file_name,"match.dat");
   if(first_plane)
   {
      outfile= (FILE *) fopen (file_name,"w");
	  fprintf (outfile,"%5d\n",npt+1);//first_plane_nr
	  first_plane=0;
   }
   else 
   {
      //update the number of points in the first line
      //outfile= (FILE *) fopen (file_name,"r+");
	  //fscanf (outfile, "%d",&matched_nr);
	  //rewind(outfile);
	  //matched_nr += (npt+1);
	  //fprintf (outfile,"%5d\n",matched_nr);
      //fclose (outfile);
      //add the new matched points
	  outfile= (FILE *) fopen (file_name,"a");
   }

   for (i=0;i<=npt;i++) {
	iref= match[i].refind; ical= match[i].calind;
      fprintf (outfile,"%5d %5d %9.4f %9.4f %9.4f %9.4f %9.4f\n",iref+1,ical+1,
		xref[iref],yref[iref],zref[iref],xcal[ical],ycal[ical]);
   }


   fclose (outfile);

   free (xref); free (yref); free (zref); free (xcal); free (ycal); free (match);
   free (reftouched); free (caltouched);

   return npt+1;
}



