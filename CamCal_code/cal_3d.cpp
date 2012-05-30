#include "stdafx.h"
#include <math.h>
#include <fstream.h>
#include <imsl.h>
#include <malloc.h>
#include "niimaq.h"
#include "windows.h"
#include "nitypes.h"
#include "parameters.h"
#include "structures.h"
#include "matrix_op.h"
#include "cal_3d.h"
#include "img_proc.h"
#include "io_functions.h"
#include "plot_functions.h"
#include "vrml_generator.h"

//**********************file and path names******************************************
extern char file_image1[255];        //file name of stereo image pair 
extern char file_cam1_par[255];      //file name of the camera parameter file
extern char file_cam1_ccd[255];      //file name of the camera ccd parameter file
extern char file_image2[255];        //file name of stereo image pair 
extern char file_cam2_par[255];      //file name of the camera parameter file
extern char file_cam2_ccd[255];      //file name of the camera ccd parameter file
extern char file_global_coord[255];  //file name of the global coordinates
extern char file_global_exp_coord[255];//file name of the measured global coordinates
extern char file_ref_image1[255];    //file name of stereo reference image pair 
extern char file_ref_image2[255];    //file name of stereo reference image pair 
//***********************************************************************************

Extrinsic_par	ex1,ex2;
Intrinsic_par	in1,in2;
extern CCD_par	ccd1,ccd2;


extern int limits_off;
extern int limits[];
extern double av_dist_h[],av_dist_p[];
extern int m[];
extern double E_res;
extern int E_res_nr;
extern int TABLE_1_LOADED;
extern int TABLE_2_LOADED;

uInt8*	image_data1;
uInt8*	image_data2;
uInt8*	image_ref_data1;
uInt8*	image_ref_data2;
uInt8*	colimage_data1;
uInt8*	colimage_data2;


//********************points extracted from images***********************************
int p_nr_max1, p_nr_max2;
CalPoint  local_cp1[MAX], local_cp2[MAX];
CamPoint  local_p1[MAX], local_p2[MAX];
CamPoint  local_p1_h[MAX],local_p2_h[MAX];
EpiPar	  epipar[MAX];
EpiPar	  epipar_pal1[MAX];
EpiPar	  epipar_pal2[MAX];

Matrix	   R1,R2;
T_vec      t1,t2;
//P_matrix   P1,P2;

extern Vec2     *table1,*table2;
extern bool REF;

int		i_point;    //used by fcn


void show_img(CDC * device)
{
	int stat;
	image_data1 =		(uInt8 *)calloc((ccd1.width*ccd1.height),sizeof(uInt8));
	image_data2 =		(uInt8 *)calloc((ccd2.width*ccd2.height),sizeof(uInt8));
	colimage_data1=		(uInt8 *)calloc((3*ccd1.width*ccd1.height),sizeof(uInt8));
	colimage_data2=		(uInt8 *)calloc((3*ccd2.width*ccd2.height),sizeof(uInt8));
	if(REF)
	{
		image_ref_data1 =	(uInt8 *)calloc((ccd1.width*ccd1.height),sizeof(uInt8));
		image_ref_data2 =	(uInt8 *)calloc((ccd2.width*ccd2.height),sizeof(uInt8));
	}

	//Read the image data
	stat=Read_image(file_image1, image_data1, ccd1.height, ccd1.width);
	if(stat==-1){ 
		//MessageBox("Could not open image 1",0, MB_ICONEXCLAMATION);
		return;	
	}//Error
	stat=Read_image(file_image2, image_data2, ccd2.height, ccd2.width);
	if(stat==-1)
	{
		//MessageBox("Could not open image 2",0, MB_ICONEXCLAMATION);
		return;
	}
	if(REF)
	{
		stat=Read_image(file_ref_image1, image_ref_data1, ccd1.height, ccd1.width);
		if(stat==-1)
		{
			//MessageBox("Could not open ref image 1",0, MB_ICONEXCLAMATION);
			return;
		}
		stat=Read_image(file_ref_image2, image_ref_data2, ccd2.height, ccd2.width);
		if(stat==-1)
		{
			//MessageBox("Could not open ref image 2",0, MB_ICONEXCLAMATION);
			return;
		}
	}

	//do some image enhancement

	if(REF)
	{
		Image_subtract(image_data1,image_ref_data1);
		Image_subtract(image_data2,image_ref_data2);
	}
	else
	{
		Image_row_cor(image_data1);
		Image_row_cor(image_data2);
	}


	//display the images
	Plot_image(device, image_data1, ccd1.height, ccd1.width, 1, 0);
	Plot_image(device, image_data2, ccd2.height, ccd2.width, 2, 0);
}


void 	DrawScreen(CDC * device)
{
//	if(image_data1 !=0)
//		Plot_image(device, image_data1, ccd1.height, ccd1.width, 1, 0);

}


void get_points(CDC * device, int thresh){
	int i;
	//************Extract the points from the two stereo images***************
	p_nr_max1 = Extract_streaks_roes3D (local_cp1, local_p1, image_data1, thresh);    //points: 1..p_nr_max1
	if(p_nr_max1==-1)
	{
		//MessageBox("Error in the point detection 1",0,0);
		return;
	}
	p_nr_max2 = Extract_streaks_roes3D (local_cp2, local_p2, image_data2, thresh);
	if(p_nr_max2==-1)
	{
		//MessageBox("Error in the point detection 2");
		return;
	}

	Plot_points3D(device, colimage_data1, local_p1, ccd1.height, ccd1.width, p_nr_max1, 255, 255, 255,  1); //points: 1..p_nr_max1
	for(i=1;i<=	p_nr_max1;i++)
	  Plot_number(device, colimage_data1, ccd1.height, ccd1.width, i, (int)local_p1[i].x, (int)local_p1[i].y, 1);
	Plot_points3D(device, colimage_data2, local_p2, ccd1.height, ccd1.width, p_nr_max2, 255, 255, 255,  2);
}



int get_3d_lin(CDC * device, int z_min,int z_max, double tolerance){

	int matched,i;
	P_matrix P1, P2;
	GlobalPoint global_point[500];
	CamPoint lim_point1[MAX], lim_point2[MAX];
	CamPoint lim_point1_h[MAX], lim_point2_h[MAX];

	//********************Read the camera parameters********************************* 
	Read_P_matrix(&P1, file_cam1_par);
	Read_P_matrix(&P2, file_cam2_par);

	Get_parameters3(P1, &ex1, &in1);
	Get_parameters3(P2, &ex2, &in2);

	//Calculates the rotation matrix R1, R2 and the translation  vector t1 and t2
	Get_transf_mat();

	//************Normalize the coordinates******************************************
	Normalize_coord_lin(local_p1, local_p1_h, in1, ccd1, p_nr_max1); 
	Normalize_coord_lin(local_p2, local_p2_h, in2, ccd2, p_nr_max2); 

	Get_epi(epipar, local_p1_h, p_nr_max1, 1);
	Get_epi_pal(epipar, epipar_pal1, tolerance, p_nr_max1);
	Get_epi_pal(epipar, epipar_pal2, -tolerance, p_nr_max1);
	//get the limiter points and store them in epivec

	Reduce_epi_z(lim_point1, lim_point2, lim_point1_h, lim_point2_h, local_p1_h, z_min, z_max, p_nr_max1);

    Plot_epi(device, colimage_data2, epipar, ccd2.height, ccd2.width, p_nr_max1, lim_point1, lim_point2, 0);
	for(i=1;i<=	p_nr_max1;i++)
	  Plot_number(device, colimage_data2, ccd2.height, ccd2.width, i, (int)lim_point1[i].x, (int)lim_point1[i].y, 2);

	Plot_epi(device, colimage_data2, epipar_pal1, ccd2.height, ccd2.width, p_nr_max1, lim_point1, lim_point2, 1);
	Plot_epi(device, colimage_data2, epipar_pal2, ccd2.height, ccd2.width, p_nr_max1, lim_point1, lim_point2, 1);
	matched=Match_points_epi(lim_point1_h, lim_point2_h, local_p1, local_p2, local_p1_h, local_p2_h, 
		epipar, p_nr_max1, p_nr_max2, tolerance);
	Plot_points3D(device, colimage_data2, local_p2, ccd2.height, ccd2.width, matched, 255, 0, 255,  2);
	Solve_3D_lin(global_point, P1, P2, local_p1, local_p2, matched);

	int nr=Write_3D_points(global_point, matched);
	vrml_points_gen();
	return matched;
}


int get_3d(CDC * device, int z_min,int z_max, double tolerance, CProgressCtrl* bar){

	int matched,i;
	GlobalPoint global_point[500];
	CamPoint lim_point1[MAX], lim_point2[MAX];
	CamPoint lim_point1_h[MAX], lim_point2_h[MAX];
	E_res=0;
	E_res_nr=0;

	//********************Read the camera parameters********************************* 
	Read_cam_param(&ex1, &in1, file_cam1_par);
	Read_cam_param(&ex2, &in2, file_cam2_par);

	//Calculates the rotation matrix R1, R2 and the translation  vector t1 and t2
	Get_transf_mat();

	//Get some matricies (essential, camera matrix etc) and save results
	Get_stereo_matrix();

	//************Normalize the coordinates******************************************
	if(TABLE_1_LOADED==0)
		{Read_table(file_cam1_par, table1, bar);  TABLE_1_LOADED=1;}
	if(TABLE_2_LOADED==0)
		{Read_table(file_cam2_par, table2, bar);  TABLE_2_LOADED=1;}

	Normalize_look_up(local_cp1, in1.focus, p_nr_max1, table1);
	Normalize_look_up(local_cp2, in2.focus, p_nr_max2, table2);
	//***********Get center of gravity of the normalized calibration points***********
	Get_centroid(local_cp1, local_p1_h, p_nr_max1);
	Get_centroid(local_cp2, local_p2_h, p_nr_max2);

	Get_epi(epipar, local_p1_h, p_nr_max1, 1);
	Get_epi_pal(epipar, epipar_pal1,  tolerance, p_nr_max1);
	Get_epi_pal(epipar, epipar_pal2, -tolerance, p_nr_max1);
	//get the limiter points and store them in epivec
	if(limits_off == 0)
		Reduce_epi_z(lim_point1, lim_point2, lim_point1_h, lim_point2_h, local_p1_h, z_min, z_max, p_nr_max1);
	else 
		Set_epi_z(lim_point1, lim_point2, lim_point1_h, lim_point2_h, local_p1_h, z_min, z_max, p_nr_max1);
    Plot_epi(device, colimage_data2, epipar,      ccd2.height, ccd2.width, p_nr_max1, lim_point1, lim_point2, 0);
	Plot_epi(device, colimage_data2, epipar_pal1, ccd2.height, ccd2.width, p_nr_max1, lim_point1, lim_point2, 1);
	Plot_epi(device, colimage_data2, epipar_pal2, ccd2.height, ccd2.width, p_nr_max1, lim_point1, lim_point2, 1);

	if(limits_off == 0)
		matched=Match_points_epi(lim_point1_h, lim_point2_h,
								 local_p1,     local_p2, 
								 local_p1_h,   local_p2_h, 
								 epipar, p_nr_max1, p_nr_max2, tolerance);
	else
		matched=Match_points_epi_no_lim(local_p1,     local_p2, 
								 local_p1_h,   local_p2_h, 
								 epipar, p_nr_max1, p_nr_max2, tolerance);



	Plot_points3D(device, colimage_data2, local_p2, ccd2.height, ccd2.width, matched, 255, 0, 255,  2);

	//marquar levenberg triangulation
	//Solve_3D(global_point, matched);

	//method according to hartley: Multiple view Geometry
	Get_3D_point_norm(global_point, local_p1_h,   local_p2_h,  matched); 

	//with the linear triangulation
	//Get_3D_point_tria(global_point, local_p1_h,   local_p2_h,  matched);
	
	int nr=Write_3D_points(global_point, matched);
	vrml_points_gen();

	for(i=1;i<7;i++)
		Plot_circle(device, colimage_data2, (double)limits[i]);

	return matched;
}



//************************************************************************************************
//************************************************************************************************
//************************************************************************************************

//************************************************************************************************
//************************************************************************************************
//************************************************************************************************


void Normalize_coord(CamPoint point[], CamPoint hpoint[], Intrinsic_par in, CCD_par ccd, int nr)
{
	int i;
	double x,y,x_b,y_b,rr;        

//*********calculate homogeneous coordintes of point[]  in camera 1 -> hpoint**********************
//*********points m is in pixel coordinates (0,0) left upper corner       *************************
	for(i=0;i <= nr; i++)
	{
		//x_b,y_b: Points in CCD coordinate system [mm]
		y_b =  ((double)ccd.height / 2.0 - point[i].y) * (double)ccd.pix_h / cos(in.theta);
		x_b =  ((point[i].x - (double)ccd.width / 2.0) * (double)ccd.pix_w  + y_b*sin(in.theta) )/in.s_x;

		rr=x_b*x_b+y_b*y_b;
		x=x_b - in.x_h - x_b * (in.k1*rr + in.k2*rr*rr); //+in1.p1*(r*r +2.0*x_b*x_b) + 2.0*in1.p2*x_b*y_b;
		y=y_b - in.y_h - y_b * (in.k1*rr + in.k2*rr*rr); //+in2.p2*(r*r +2.0*y_b*y_b) + 2.0*in2.p1*x_b*y_b;	

		//normalization projection; 
		hpoint[i].x= x / (-in.focus);
		hpoint[i].y= y / (-in.focus);
		hpoint[i].w= 1.0;
	}
}

void Normalize_look_up(CalPoint calpoint[], double focus, int s_nr_max, Vec2* table)
{
	int i,j,n_p;
	double x,y;



	for(i=1;i<=s_nr_max;i++)
	{
		n_p=calpoint[i].n_p;
		for(j=0;j<=n_p;j++)
		{
			//get data from look up table
			x=table[calpoint[i].x[j] + 640*calpoint[i].y[j]].x;
			y=table[calpoint[i].x[j] + 640*calpoint[i].y[j]].y;

			//normalization projection; 
			calpoint[i].hx[j] = x / (-focus);
			calpoint[i].hy[j] = y / (-focus);
			calpoint[i].hw[j] = 1.0;
		}
	}
}


void Normalize_coord_lin(CamPoint point[], CamPoint hpoint[], Intrinsic_par in, CCD_par ccd, int nr)
{
	int i;
	double x,y;        

//*********points m is in pixel coordinates (0,0) left upper corner       *************************
	for(i=1;i <= nr; i++)
	{
		//x,y: Points in CCD coordinate system [mm]
		x =  ((point[i].x - (double)ccd.width / 2.0) * (double)ccd.pix_w)/in.s_x - in.x_h;
		y =  ((double)ccd.height / 2.0 - point[i].y) * (double)ccd.pix_h         - in.y_h;

		//normalization projection; 
		hpoint[i].x= x / (-in.focus);
		hpoint[i].y= y / (-in.focus);
		hpoint[i].w= 1.0;
	}
}


void Get_centroid(CalPoint local_cp[], CamPoint hcentr[], int p_nr)
{
	int i,j,nsum;
	double xsum,ysum;

	for(i=1;i<=p_nr;i++)
	{
		xsum=0;	ysum=0;nsum=0;
		for(j=0;j<=local_cp[i].n_p;j++)
		{
            xsum += local_cp[i].hx[j]; ysum += local_cp[i].hy[j]; nsum++;
		}
		hcentr[i].x = xsum / (double) nsum; 
		hcentr[i].y = ysum / (double) nsum; 
		hcentr[i].w = 1.0;
	}
}


void Get_epi(EpiPar epipar[], CamPoint hpoint[], int nr, int dir){

	int i;

	Matrix         R;
	Matrix         E, T_x;
	T_vec          t;
//*********calculate essential matrix E ************************************************************

	R = Neg(  mat_mult    (transpose(R2),   R1));
	t = mat_vec_mult(transpose(R2), (vec_sub(t1,t2)));
	T_x.a11 =  0.0;
	T_x.a12 = -t.t3;
	T_x.a13 =  t.t2;
	T_x.a21 =  t.t3;
	T_x.a22 =  0.0;
	T_x.a23 = -t.t1;
	T_x.a31 = -t.t2;
    T_x.a32 =  t.t1;
	T_x.a33 =  0.0;

	E = mat_mult(T_x, R);

//*********calculate normalized epi lines in cam 2            ***************************************
//*********                 "  *E hm1  "                      ***************************************
//*********   a*hm2.x + b*hm.y + c = 0                        ***************************************    
	T_vec t_dummy1, t_dummy2;

	for(i=1;i <= nr; i++)
	{
		t_dummy1.t1 = hpoint[i].x;
		t_dummy1.t2 = hpoint[i].y;
		t_dummy1.t3 = hpoint[i].w;

		t_dummy2 = mat_vec_mult(E, t_dummy1);  

		epipar[i].a = t_dummy2.t1;
		epipar[i].b = t_dummy2.t2;
		epipar[i].c = t_dummy2.t3;	
	}	
}


void Get_epi_pal(EpiPar epipar[],  EpiPar epipar_pal[], double tolerance, int nr)
{
	//This routine get a paralell line epipar_pal[]: ax+by+c=0
	//to the line epipar[]: ax+by+c=0 in a distance tolerance!

	double alpha;

	for(int i=1;i <= nr; i++)
	{
		epipar_pal[i].a=epipar[i].a/epipar[i].b;
		epipar_pal[i].b=1.0;
		alpha=atan(-epipar[i].a/epipar[i].b);
		epipar_pal[i].c=epipar[i].c/epipar[i].b-tolerance*cos(alpha);
	}
}


//**************Find the nearest point hm2 to a epi-line in normalized coordinates************
//********************************************************************************************
int Match_points_epi(CamPoint *lim1, CamPoint *lim2, CamPoint *point1, CamPoint *point2, 
					 CamPoint hpoint1[], CamPoint hpoint2[], EpiPar epi_par[], 
					 int max1, int max2, double tolerance)
{
	int i,j;

	int    *candidates = new int[max2];
	int    *neighbour  = new int[max1];
	int     match_nr;			//Number of hm2 with match to hm1 (epi1)
	int     cnd_nr;				//Number of possible correpondences
	int     match_tot;		    //Number of matched points
	double  nenner,distance;
	CamPoint dummy_m, dummy_p;
	T_vec n,p;
	double n_l,s_p;

	match_tot=0;				//total number of matches

	//Variables to calculate the quality of the calibration**********************************
	int cell=5;  //number of sample cells (should be even)
	double f_width=sqrt((double)(ccd2.height*ccd2.height+ ccd2.width*ccd2.width))/double(2*cell);
	double x,y,r;
	int l;


	for(i=0;i<cell+2;i++)
		limits[i] = i*(int)f_width;
	for(i=0;i<cell+1;i++)
	{
		av_dist_h[i]=0;av_dist_p[i]=0;m[i]=0;
	}
	//***************************************************************************************


	for(i=1; i<= max1; i++)  
	{
		nenner = sqrt(epipar[i].a * epipar[i].a + epipar[i].b * epipar[i].b);
		cnd_nr=0;      //number of matches per epi line


		for(j=1; j <= max2; j++)
		{
			distance  =fabs(epipar[i].a * hpoint2[j].x + epipar[i].b * hpoint2[j].y +
						      epipar[i].c) / nenner;

			
			n.t1=lim2[i].x-lim1[i].x;
			n.t2=lim2[i].y-lim1[i].y;
		
			n_l=sqrt((lim2[i].x-lim1[i].x)*(lim2[i].x-lim1[i].x) + (lim2[i].y-lim1[i].y)*(lim2[i].y-lim1[i].y));
			n.t1=n.t1/n_l;
			n.t2=n.t2/n_l;
			
			p.t1=hpoint2[j].x-lim1[i].x;
			p.t2=hpoint2[j].y-lim1[i].y;

			s_p=p.t1*n.t1 + p.t2*n.t2;            //Projection of point onto lim1 lim2

			if(distance < tolerance)
			{
				if(s_p > 0 && s_p < n_l)
				{
					cnd_nr++;                    
					match_nr = j; 

					//******************************************************************
					//********calculate some estimators for the calibration quality*****
					//******************************************************************
					x=point2[j].x-ccd2.width/2.0;
					y=point2[j].y-ccd2.height/2.0;
					r = sqrt(x*x + y*y);
					//the residual x2^T E x1, where  epipar=E x1
					E_res+=fabs(epipar[i].a * hpoint2[j].x + epipar[i].b * hpoint2[j].y + epipar[i].c);
					E_res_nr++;
					for(l=0;l<cell+1;l++)
					{
						if(limits[l] <= r && r < limits[l+1] )
						{
							av_dist_h[l] += distance;  
							av_dist_p[l] += distance*in2.focus;
							m[l]++;
							break;
						}
					}
					//******************************************************************
					//********END calculate some estimators for the calibration quality*
					//******************************************************************
				}
			}
		}

		if(cnd_nr ==1)       
			match_tot++;     //number of exact matches


		//points hpoint with no or more than one correspondence are eliminated
		if((cnd_nr == 0 || cnd_nr > 1) &&  i != max1 ) 
		{
			for(j=i; j <= (max1-1); j++)
			{
				point1[j]  = point1[j+1];
				hpoint1[j] = hpoint1[j+1];
				lim1[j]	   = lim1[j+1];
				lim2[j]	   = lim2[j+1];
				epipar[j]  = epipar[j+1];
			}
			i--;   
			max1--;
		}

		else if(match_nr != i && cnd_nr==1) //sort (if there is only one candidate)
		{
			//sort the point in camera coordinates
			dummy_p  = point2[i];
			point2[i]  = point2[match_nr];
			point2[match_nr]  = dummy_p;
			//sort the point in homogeneos coordinates
			dummy_m = hpoint2[i];
			hpoint2[i] = hpoint2[match_nr];
			hpoint2[match_nr] = dummy_m;
		}	
	}
    return match_tot;   //number of point with correspondences
}


int Match_points_epi_no_lim(CamPoint *point1, CamPoint *point2, 
					 CamPoint hpoint1[], CamPoint hpoint2[], EpiPar epi_par[], 
					 int max1, int max2, double tolerance)
{
	int i,j;

	int    *candidates = new int[max2];
	int    *neighbour  = new int[max1];
	int     match_nr;			//Number of hm2 with match to hm1 (epi1)
	int     cnd_nr;				//Number of possible correpondences
	int     match_tot;		    //Number of matched points
	double  nenner,distance;
	CamPoint dummy_m, dummy_p;

	match_tot=0;				//total number of matches

	//Variables to calculate the quality of the calibration**********************************
	int cell=5;  //number of sample cells (should be even)
	double f_width=sqrt((double)(ccd2.height*ccd2.height+ ccd2.width*ccd2.width))/double(2*cell);
	double x,y,r;
	int l;


	for(i=0;i<cell+2;i++)
		limits[i] = i*(int)f_width;
	for(i=0;i<cell+1;i++)
	{
		av_dist_h[i]=0;av_dist_p[i]=0;m[i]=0;
	}
	//***************************************************************************************


	for(i=1; i<= max1; i++)  
	{
		nenner = sqrt(epipar[i].a * epipar[i].a + epipar[i].b * epipar[i].b);
		cnd_nr=0;      //number of matches per epi line


		for(j=1; j <= max2; j++)
		{
			distance  =fabs(epipar[i].a * hpoint2[j].x + epipar[i].b * hpoint2[j].y +
						      epipar[i].c) / nenner;

			if(distance < tolerance)
			{
				cnd_nr++;                    
				match_nr = j; 
				//******************************************************************
				//********calculate some estimators for the calibration quality*****
				//******************************************************************
				x=point2[j].x-ccd2.width/2.0;
				y=point2[j].y-ccd2.height/2.0;
				r = sqrt(x*x + y*y);
				//the residual x2^T E x1, where  epipar=E x1
				E_res+=fabs(epipar[i].a * hpoint2[j].x + epipar[i].b * hpoint2[j].y + epipar[i].c);
				E_res_nr++;
				for(l=0;l<cell+1;l++)
				{
					if(limits[l] <= r && r < limits[l+1] )
					{
						av_dist_h[l] += distance;  
						av_dist_p[l] += distance*in2.focus;
						m[l]++;
						break;
					}
				}
				//******************************************************************
				//********END calculate some estimators for the calibration quality*
				//******************************************************************
			}
		}

		if(cnd_nr ==1)       
			match_tot++;     //number of exact matches


		//points hpoint with no or more than one correspondence are eliminated
		if((cnd_nr == 0 || cnd_nr > 1) &&  i != max1 ) 
		{
			for(j=i; j <= (max1-1); j++)
			{
				point1[j]  = point1[j+1];
				hpoint1[j] = hpoint1[j+1];
				epipar[j]  = epipar[j+1];
			}
			i--;   
			max1--;
		}

		else if(match_nr != i && cnd_nr==1) //sort (if there is only one candidate)
		{
			//sort the point in camera coordinates
			dummy_p  = point2[i];
			point2[i]  = point2[match_nr];
			point2[match_nr]  = dummy_p;
			//sort the point in homogeneos coordinates
			dummy_m = hpoint2[i];
			hpoint2[i] = hpoint2[match_nr];
			hpoint2[match_nr] = dummy_m;
		}	
	}
    return match_tot;   //number of point with correspondences
}



void Solve_3D_lin(GlobalPoint *global_point, P_matrix P1, P_matrix P2,
			  CamPoint *local_p1, CamPoint*local_p2, int point_nr)
{
	int i,n=3,m=4;
	double x1,x2,y1,y2;
	double *result;
	double * A=(double *)calloc(m*n, sizeof(double));
	double * r=(double *)calloc(m,   sizeof(double)); 

	for(i=1; i<=point_nr; i++)
	{
		//transform the points 
		x1=(local_p1[i].x-(double)ccd1.width/2.0 )*ccd1.pix_w;
		y1=((double)ccd1.height/2.0-local_p1[i].y)*ccd1.pix_h;

		x2=(local_p2[i].x-(double)ccd2.width/2.0 )*ccd2.pix_w;
		y2=((double)ccd2.height/2.0-local_p2[i].y)*ccd2.pix_h;


		A[0] =P1.p11-x1*P1.p31;
		A[1] =P1.p12-x1*P1.p32;
		A[2] =P1.p13-x1*P1.p33;
		A[3] =P1.p21-y1*P1.p31;
		A[4] =P1.p22-y1*P1.p32;
		A[5] =P1.p23-y1*P1.p33;

		A[6] =P2.p11-x2*P2.p31;
		A[7] =P2.p12-x2*P2.p32;
		A[8] =P2.p13-x2*P2.p33;
		A[9] =P2.p21-y2*P2.p31;
		A[10]=P2.p22-y2*P2.p32;
		A[11]=P2.p23-y2*P2.p33;

		r[0]=x1*P1.p34-P1.p14;
		r[1]=y1*P1.p34-P1.p24;
		r[2]=x2*P2.p34-P2.p14;
		r[3]=y2*P2.p34-P2.p24;

		result=imsl_d_lin_least_squares_gen (m, n, A, r,0);
		global_point[i].X = *result++;
		global_point[i].Y = *result++;
		global_point[i].Z = *result;
	}
}



void Solve_3D(GlobalPoint *global_point, int point_nr)
{
	int m=4, n=3;           //m: Number of Funktions   n: Number of Variables  n<=m
	double xguess[3];
	double *result;

	for(i_point=1; i_point <= point_nr; i_point++)
	{
		xguess[0] =  500.0;
		xguess[1] =  400.0;
		xguess[2] =  -10.0;

		result = imsl_d_nonlin_least_squares(fcn, m, n, IMSL_XGUESS, xguess, 0);

		global_point[i_point].X = *result++;
		global_point[i_point].Y = *result++;
		global_point[i_point].Z = *result;
	}
}




void fcn(int m, int n, double x[], double f[])
{
	double X = x[0];
	double Y = x[1];
	double Z = x[2];

	double x_h,y_h;

	double nenner1 =  R1.a13*(X- ex1.X_0)+R1.a23*(Y-ex1.Y_0)+R1.a33*(Z- ex1.Z_0);
	double nenner2 =  R2.a13*(X- ex2.X_0)+R2.a23*(Y-ex2.Y_0)+R2.a33*(Z- ex2.Z_0);  
	
	//For camera 1
	//Bildkoordinaten 
	x_h   = ( R1.a11*(X- ex1.X_0)+R1.a21*(Y-ex1.Y_0)+R1.a31*(Z-ex1.Z_0) ) / nenner1;			                              
	y_h   = ( R1.a12*(X- ex1.X_0)+R1.a22*(Y-ex1.Y_0)+R1.a32*(Z-ex1.Z_0) ) / nenner1;
		
	f[0]  =  x_h  - local_p1_h[i_point].x;
	f[1]  =  y_h  - local_p1_h[i_point].y;

	//For camera 2
	//Bildkoordinaten 
	x_h   = ( R2.a11*(X- ex2.X_0)+R2.a21*(Y-ex2.Y_0)+R2.a31*(Z-ex2.Z_0) ) / nenner2;			                              
	y_h   = ( R2.a12*(X- ex2.X_0)+R2.a22*(Y-ex2.Y_0)+R2.a32*(Z-ex2.Z_0) ) / nenner2;

	f[2]  =  x_h  - local_p2_h[i_point].x;
	f[3]  =  y_h  - local_p2_h[i_point].y;
}


void Get_3D_point_norm(GlobalPoint *Point, CamPoint *hp1, CamPoint *hp2, int point_nr)
{
	//calculation of the three dim. coordinates according to Multiple 
	//View Geometry p.297 and p298
	//P1 and P2 are the camera matricies of an ideal pin hole camera.
	P_matrix P1,P2;

	P1.p11=   R1.a11;
	P1.p12=   R1.a21;
	P1.p13=   R1.a31; 
	P1.p14=  -R1.a11*t1.t1-R1.a21*t1.t2-R1.a31*t1.t3; 
	P1.p21=   R1.a12;
	P1.p22=   R1.a22;
	P1.p23=   R1.a32;
	P1.p24=  -R1.a12*t1.t1-R1.a22*t1.t2-R1.a32*t1.t3; 
	P1.p31=   R1.a13; 
	P1.p32=   R1.a23; 
	P1.p33=   R1.a33; 
	P1.p34=  -R1.a13*t1.t1-R1.a23*t1.t2-R1.a33*t1.t3;

	P2.p11=   R2.a11;
	P2.p12=   R2.a21;
	P2.p13=   R2.a31; 
	P2.p14=  -R2.a11*t2.t1-R2.a21*t2.t2-R2.a31*t2.t3; 
	P2.p21=   R2.a12;
	P2.p22=   R2.a22;
	P2.p23=   R2.a32;
	P2.p24=  -R2.a12*t2.t1-R2.a22*t2.t2-R2.a32*t2.t3; 
	P2.p31=   R2.a13; 
	P2.p32=   R2.a23; 
	P2.p33=   R2.a33; 
	P2.p34=  -R2.a13*t2.t1-R2.a23*t2.t2-R2.a33*t2.t3;

	int n=4;//columns
	d_complex *eval;
	d_complex *evec;
	double A[16];

	for(int i=1; i <= point_nr; i++)
	{
		A[0] =hp1[i].x * P1.p31   - P1.p11;
		A[1] =hp1[i].x * P1.p32   - P1.p12;
		A[2] =hp1[i].x * P1.p33   - P1.p13;
		A[3] =hp1[i].x * P1.p34   - P1.p14;
		A[4] =hp1[i].y * P1.p31   - P1.p21;
		A[5] =hp1[i].y * P1.p32   - P1.p22;
		A[6] =hp1[i].y * P1.p33   - P1.p23;
		A[7] =hp1[i].y * P1.p34   - P1.p24;

		A[8]= hp2[i].x * P2.p31   - P2.p11;
		A[9]= hp2[i].x * P2.p32   - P2.p12;
		A[10]=hp2[i].x * P2.p33   - P2.p13;
		A[11]=hp2[i].x * P2.p34   - P2.p14;
		A[12]=hp2[i].y * P2.p31   - P2.p21;
		A[13]=hp2[i].y * P2.p32   - P2.p22;
		A[14]=hp2[i].y * P2.p33   - P2.p23;
		A[15]=hp2[i].y * P2.p34   - P2.p24;

		eval= imsl_d_eig_gen (n, A, IMSL_VECTORS, &evec, 0);
		Point[i].X = evec[3].re;
		Point[i].Y = evec[7].re;
		Point[i].Z = evec[11].re;
		double   W = evec[15].re;

		Point[i].X = Point[i].X /W;
		Point[i].Y = Point[i].Y /W;
		Point[i].Z = Point[i].Z /W;
	}
}



void Get_3D_point_tria(GlobalPoint *global_point, CamPoint *hp1, CamPoint *hp2, int point_nr)
{
	int n=4;//columns
	int m=6;//rows
	double *result;
	double r[6];
	double X,Y,Z,W;
	double K[24];

	K[0]  =  R1.a11;
	K[1]  =  R1.a12;
	K[2]  =  R1.a13;
	K[3]  =	 (-R1.a11*t1.t1 - R1.a12*t1.t2 - R1.a13*t1.t3);
	K[4]  =  R1.a21;
	K[5]  =  R1.a22;
	K[6]  =  R1.a23;
	K[7]  =  (-R1.a21*t1.t1 - R1.a22*t1.t2 - R1.a23*t1.t3);
	K[8]  =  R1.a31;
	K[9]  =  R1.a32;
	K[10] =  R1.a33;
	K[11] =  (-R1.a31*t1.t1 - R1.a32*t1.t2 - R1.a33*t1.t3);

	K[12] =  R2.a11;
	K[13] =  R2.a12;
	K[14] =  R2.a13;
	K[15] =  (R2.a11*t2.t1 - R2.a12*t2.t2 - R2.a13*t2.t3);
	K[16] =  R2.a21;
	K[17] =  R2.a22;
	K[18] =  R2.a23;
	K[19] =  (R2.a21*t2.t1 - R2.a22*t2.t2 - R2.a23*t2.t3);
	K[20] =  R2.a31;
	K[21] =  R2.a32;
	K[22] =  R2.a33;
	K[23] =  (-R2.a31*t2.t1 - R2.a32*t2.t2 - R2.a33*t2.t3);

	for(int i=1; i <= point_nr; i++)
	{
		if(hp1[i].x != -1 || hp2[i].x != -1 || hp1[i].x == 0 || hp2[i].x == 0)
		{
			r[0]=hp1[i].x;
			r[1]=hp1[i].y;
			r[2]=1;
			r[3]=hp2[i].x;
			r[4]=hp2[i].y;
			r[5]=1;

			result=imsl_d_lin_least_squares_gen (m, n, K, r,0);
			X = *result++;
			Y = *result++;
			Z = *result++;
			W = *result; 

			global_point[i].X = X/W;
			global_point[i].Y = Y/W;
			global_point[i].Z = Z/W;
		}
	}
}


void Get_transf_mat()
{
	t1.t1=ex1.X_0;
	t1.t2=ex1.Y_0;	
	t1.t3=ex1.Z_0;

	t2.t1=ex2.X_0;
	t2.t2=ex2.Y_0;	
	t2.t3=ex2.Z_0;

	//Calculation of the Rotationsmatrix 
	//The Matrix is applied as following:  m : point in global coordinates
	//                                     m': point in cam1   coordinates
	//                       t=X_0, Y_0, Z_0 : position of the camera in global coordinates
	//      m'=transpose(R) (m - t)     
	R1.a11 =  cos(ex1.phi)*cos(ex1.kappa);
	R1.a12 = -cos(ex1.phi)*sin(ex1.kappa);
	R1.a13 =  sin(ex1.phi);
	R1.a21 =  cos(ex1.omega)*sin(ex1.kappa) + sin(ex1.omega)*sin(ex1.phi)*cos(ex1.kappa);
	R1.a22 =  cos(ex1.omega)*cos(ex1.kappa) - sin(ex1.omega)*sin(ex1.phi)*sin(ex1.kappa);
	R1.a23 = -sin(ex1.omega)*cos(ex1.phi);  
	R1.a31 =  sin(ex1.omega)*sin(ex1.kappa) - cos(ex1.omega)*sin(ex1.phi)*cos(ex1.kappa);
	R1.a32 =  sin(ex1.omega)*cos(ex1.kappa) + cos(ex1.omega)*sin(ex1.phi)*sin(ex1.kappa);
	R1.a33 =  cos(ex1.omega)*cos(ex1.phi);

	R2.a11 =  cos(ex2.phi)*cos(ex2.kappa);
	R2.a12 = -cos(ex2.phi)*sin(ex2.kappa);
	R2.a13 =  sin(ex2.phi);
	R2.a21 =  cos(ex2.omega)*sin(ex2.kappa) + sin(ex2.omega)*sin(ex2.phi)*cos(ex2.kappa);
	R2.a22 =  cos(ex2.omega)*cos(ex2.kappa) - sin(ex2.omega)*sin(ex2.phi)*sin(ex2.kappa);
	R2.a23 = -sin(ex2.omega)*cos(ex2.phi);  
	R2.a31 =  sin(ex2.omega)*sin(ex2.kappa) - cos(ex2.omega)*sin(ex2.phi)*cos(ex2.kappa);
	R2.a32 =  sin(ex2.omega)*cos(ex2.kappa) + cos(ex2.omega)*sin(ex2.phi)*sin(ex2.kappa);
	R2.a33 =  cos(ex2.omega)*cos(ex2.phi);
}



void Reduce_epi_z(CamPoint lim1[], CamPoint lim2[], CamPoint hlim1[], CamPoint hlim2[], CamPoint xh[],
				  int zmin, int zmax, int nr)
{
//xh: point in homogeneous coordinates

	int i;
	double s,l,nenner2;
	double x_b, y_b, x_p, y_p;
    T_vec  dummy;
	T_vec  M_min;     //limiter point in global coordinates, camera 1 and 2 coordinates
	T_vec  x_w;
	T_vec  b1,b1_min,b1_max;           

	double z=0.0,z_min,z_max;

	z_min=z-zmin;   //5
	z_max=z+zmax;   //5
	for(i=1; i <= nr; i++)		
	{
		//transform point xh into world-coordinates
		dummy.t1 = xh[i].x;
		dummy.t2 = xh[i].y;
		dummy.t3 = 1.0;
		x_w = vec_add( mat_vec_mult(R1,dummy), t1);
		//get projection line of first camera (b_1)
		b1=vec_sub(x_w,t1);
		//normalize
		l=sqrt(b1.t1*b1.t1+b1.t2*b1.t2+b1.t3*b1.t3);
		b1.t1=b1.t1/l;
		b1.t2=b1.t2/l;
		b1.t3=b1.t3/l;
		//******************************************************************************************
		//the minimum limiter***********************************************************************

		//calculate the factor s_min 
		s=(z_min-t1.t3)/b1.t3;
		b1_min.t1=s*b1.t1;
		b1_min.t2=s*b1.t2;
		b1_min.t3=s*b1.t3;


		//calculate the limiter points in global coordinates
		M_min=vec_add(t1,b1_min);
        //********project limiter points into camera 2*********************************************
		nenner2 =  R2.a13*(M_min.t1- ex2.X_0)+R2.a23*(M_min.t2-ex2.Y_0)+R2.a33*(M_min.t3- ex2.Z_0);

		//For camera 2
		//Bildkoordinaten 
		x_b =  - in2.focus * ( R2.a11*(M_min.t1- ex2.X_0)+R2.a21*(M_min.t2-ex2.Y_0)+R2.a31*(M_min.t3-ex2.Z_0) ) / nenner2;			                              
		y_b =  - in2.focus * ( R2.a12*(M_min.t1- ex2.X_0)+R2.a22*(M_min.t2-ex2.Y_0)+R2.a32*(M_min.t3-ex2.Z_0) ) / nenner2;
	
		//homogeneous coordinates (normalization projection) 
		hlim1[i].x= x_b / (-in2.focus);
		hlim1[i].y= y_b / (-in2.focus);
		hlim1[i].w= 1.0;


		//metrische Koordinaten (0,0): links oben
		//x_p = (x_b+in2.x_h) / (double)ccd2.pix_w + (double)ccd2.width/2.0;
		//y_p = (double)ccd2.height / 2.0 - (y_b+in2.y_h) / ccd2.pix_h;

		//shearing and stretching -> metrische Koordinaten (0,0): links oben
		x_p = in2.s_x * (x_b+in2.x_h)  / ccd2.pix_w + (double)ccd2.width/2.0;
		y_p = (double)ccd2.height / 2.0 - (y_b+in2.y_h) / ccd2.pix_h;



		lim1[i].x = x_p;
		lim1[i].y = y_p;

		//******************************************************************************************
		//the maximum limiter***********************************************************************

		//calculate the factor s_max
		s=(z_max-t1.t3)/b1.t3;
		b1_max.t1=s*b1.t1;
		b1_max.t2=s*b1.t2;
		b1_max.t3=s*b1.t3;
		//calculate the limiter points in global coordinates
		M_min=vec_add(t1,b1_max);
        //********project limiter points into camera 2*********************************************
		nenner2 =  R2.a13*(M_min.t1- ex2.X_0)+R2.a23*(M_min.t2-ex2.Y_0)+R2.a33*(M_min.t3- ex2.Z_0);

		//For camera 2
		//Bildkoordinaten 
		x_b = - in2.focus * ( R2.a11*(M_min.t1- ex2.X_0)+R2.a21*(M_min.t2-ex2.Y_0)+R2.a31*(M_min.t3-ex2.Z_0) ) / nenner2;			                              
		y_b = - in2.focus * ( R2.a12*(M_min.t1- ex2.X_0)+R2.a22*(M_min.t2-ex2.Y_0)+R2.a32*(M_min.t3-ex2.Z_0) ) / nenner2;
	
		//homogeneous coordinates (normalization projection) 
		hlim2[i].x= x_b / (-in2.focus);
		hlim2[i].y= y_b / (-in2.focus);
		hlim2[i].w= 1.0;

		//metrische Koordinaten (0,0): links oben
		//x_p =  (x_b+in2.x_h)  / (double)ccd2.pix_w + (double)ccd2.width/2.0;
		//y_p = (double)ccd2.height / 2.0 - (y_b+in2.y_h) / ccd2.pix_h;

		//shearing and stretching -> metrische Koordinaten (0,0): links oben
		x_p = in2.s_x * (x_b+in2.x_h)  / ccd2.pix_w + (double)ccd2.width/2.0;
		y_p = (double)ccd2.height / 2.0 - (y_b+in2.y_h) / ccd2.pix_h;

		lim2[i].x = x_p;
		lim2[i].y = y_p;	
	}
}



void Set_epi_z(CamPoint lim1[], CamPoint lim2[], CamPoint hlim1[], CamPoint hlim2[], CamPoint xh[],
				  int zmin, int zmax, int nr)
{
	for(int i=1; i <= nr; i++)		
	{
		lim1[i].x = 0;
		lim1[i].y = 0;

		lim2[i].x = 640;
		lim2[i].y = 480;	
	}
}




int Get_accuracy(double x_diff[], double y_diff[], double z_diff[], double diff[], int s_nr[])
{
	int i,j,nr;
	int nr_mes, tot_samp;
	double x_delta,y_delta,z_delta,delta;
    float *xref, *yref, *zref;
	GlobalPoint points[500];
	FILE *file = NULL;

//*******Read the file with the global data****************************************
	file= (FILE *) fopen (file_global_coord,"r");
	if(file==NULL)
		return -1;
	fscanf (file,"%d",&nr);


	xref= (float *) calloc (nr,sizeof(float));
	yref= (float *) calloc (nr,sizeof(float));
	zref= (float *) calloc (nr,sizeof(float));
	//reftouched= (unsigned char *) calloc (nptr,sizeof(char));

	for (i=0;i<nr;i++) fscanf (file,"%d %f %f %f",&j,&xref[i],&yref[i],&zref[i]);
	fclose (file);
//*******End Read the file with the measured global data***************************
	nr_mes = Read_3D_points(points, file_global_exp_coord);

	tot_samp=0;
	for(i=0;i<10;i++)
	{
		diff[i]    =0;
		x_diff[i] = 0;
		y_diff[i] = 0;
		z_diff[i] = 0;
		s_nr[i]    =0;
	}

	for(i=0; i< nr_mes; i++)
	{
		for(j=0;j<nr;j++)
		{
			x_delta=(xref[j]-points[i].X)*(xref[j]-points[i].X);
			y_delta=(yref[j]-points[i].Y)*(yref[j]-points[i].Y);
			z_delta=(zref[j]-points[i].Z)*(zref[j]-points[i].Z);
			delta=sqrt(x_delta + y_delta + z_delta);
			if(delta<5)
			{
				diff[0]   += delta;
				x_diff[0] += sqrt(x_delta);
				y_diff[0] += sqrt(y_delta);
				z_diff[0] += sqrt(z_delta);
				s_nr[0]++;

				tot_samp++;
			}
		}
	}

	for(i=0;i<10;i++)
	{
		if(s_nr[i] != 0)
		{
			diff[i]   =   diff[i]/(double)s_nr[i];
			x_diff[i] = x_diff[i]/(double)s_nr[i];
			y_diff[i] = y_diff[i]/(double)s_nr[i];
			z_diff[i] = z_diff[i]/(double)s_nr[i];
		}
	}
	return tot_samp;
}


void Get_stereo_matrix()
{
	Matrix         R;
	Matrix         T_x;
	T_vec          t;

	//********************************************************************************************
	//Calculate the camera matrix of a ideal pin hole camera with focus f=1: 
	//                      P=[diag(1),0][R -Rt; 0 0 0 1]
	P_matrix P1,P2;
	P1.p11=   R1.a11;
	P1.p12=   R1.a21;
	P1.p13=   R1.a31; 
	P1.p14=  -R1.a11*t1.t1-R1.a21*t1.t2-R1.a31*t1.t3; 
	P1.p21=   R1.a12;
	P1.p22=   R1.a22;
	P1.p23=   R1.a32;
	P1.p24=  -R1.a12*t1.t1-R1.a22*t1.t2-R1.a32*t1.t3; 
	P1.p31=   R1.a13; 
	P1.p32=   R1.a23; 
	P1.p33=   R1.a33; 
	P1.p34=  -R1.a13*t1.t1-R1.a23*t1.t2-R1.a33*t1.t3;


	//********the position of camera 1 expressed in coordinates of camera 2************
	//********(origin is in camera 2  *************************************************
	//in the code R1 and R2 are defined transposed compared to the report!
	R =  mat_mult    (transpose(R2),   R1);
	t =  mat_vec_mult(transpose(R2), (vec_sub(t1,t2)));

	double delta = sqrt(t.t1*t.t1+t.t2*t.t2+t.t3*t.t3);
	double phi    =asin( R.a13);
	double kappa1 =acos( R.a11/cos(phi));
	double kappa2 =asin(-R.a12/cos(phi));
	double omega1 =acos( R.a33/cos(phi));
	double omega2 =asin(-R.a23/cos(phi));
	double kappa;
	double omega;

	//find the right solution
	if(fabs(R.a21 - Rot_mat_elem(omega1,phi,kappa1,2,1)) < 0.00001)
		{kappa=kappa1; omega=omega1;}
	else if(fabs(R.a21 - Rot_mat_elem(omega1,phi,kappa2,2,1)) < 0.00001)
		{kappa=kappa2; omega=omega1;}
	else if(fabs(R.a21 - Rot_mat_elem(omega2,phi,kappa1,2,1)) < 0.00001)
		{kappa=kappa1; omega=omega2;}	
	else if(fabs(R.a21 - Rot_mat_elem(omega2,phi,kappa2,2,1)) < 0.00001)
		{kappa=kappa2;omega=omega2;}

	//*********calculate essential matrix E ******************************************
	T_x=trans(t);
	Matrix E1 = mat_mult(T_x, R);

	double det_R   = Det(R);
	double det_T_x = Det(T_x);
	double det_E   = Det(E1);

	char file[255];
	strcpy(file,file_cam1_par);
	char *c=file+strlen(file);
	while(*c != '\\')
		*c--;
	*c='\0';
	strcat(file,"\\rot2_1_matrix.dat");
	FILE *fp=fopen(file,"wb");
	fprintf(fp,"The position and rotation of first camera with the origin in the second camera\n");
	fprintf(fp,"%10.4lf%10.4lf%10.4lf\n",R.a11,R.a12,R.a13);
	fprintf(fp,"%10.4lf%10.4lf%10.4lf\n",R.a21,R.a22,R.a23);
	fprintf(fp,"%10.4lf%10.4lf%10.4lf\n",R.a31,R.a32,R.a33);
	fprintf(fp,"The rotational angles\n");
	fprintf(fp,"omega = %8.4lf\n",omega);
	fprintf(fp,"phi   = %8.4lf\n",phi);
	fprintf(fp,"kappa = %8.4lf\n",kappa);
	fprintf(fp,"The position\n");
	fprintf(fp,"CX    = %8.4lf\n",t.t1);
	fprintf(fp,"CY    = %8.4lf\n",t.t2);
	fprintf(fp,"CZ    = %8.4lf\n",t.t3);
	fprintf(fp,"The distance between the cameras\n");
	fprintf(fp,"delta = %8.4lf\n",delta);
	fprintf(fp,"The Essential Matrix to project points from view 1 into view 2\n");
	fprintf(fp,"%10.4lf%10.4lf%10.4lf\n",E1.a11,E1.a12,E1.a13);
	fprintf(fp,"%10.4lf%10.4lf%10.4lf\n",E1.a21,E1.a22,E1.a23);
	fprintf(fp,"%10.4lf%10.4lf%10.4lf\n",E1.a31,E1.a32,E1.a33);
	fprintf(fp,"The camera matrix P of the normalized camera (f=1)\n");
	fprintf(fp,"%10.4lf%10.4lf%10.4lf %10.4lf\n",P1.p11,P1.p12,P1.p13,P1.p14);
	fprintf(fp,"%10.4lf%10.4lf%10.4lf %10.4lf\n",P1.p21,P1.p22,P1.p23,P1.p24);
	fprintf(fp,"%10.4lf%10.4lf%10.4lf %10.4lf\n",P1.p31,P1.p32,P1.p33,P1.p34);
	fprintf(fp,"Det(E)\n");
	fprintf(fp,"%10.5lf\n",det_E);
	char tmpbuf[255];
	_strdate(tmpbuf);
	fprintf(fp, "Data created: %s",tmpbuf);
	fclose(fp);


	//********************************************************************************************
	//*********  second camera *******************************************************************
	//********************************************************************************************

	//********************************************************************************************
	//Calculate the camera matrix of a ideal pin hole camera with focus f=1: 
	//                      P=[diag(1),0][R -Rt; 0 0 0 1]
	P2.p11=   R2.a11;
	P2.p12=   R2.a21;
	P2.p13=   R2.a31; 
	P2.p14=  -R2.a11*t2.t1-R2.a21*t2.t2-R2.a31*t2.t3; 
	P2.p21=   R2.a12;
	P2.p22=   R2.a22;
	P2.p23=   R2.a32;
	P2.p24=  -R2.a12*t2.t1-R2.a22*t2.t2-R2.a32*t2.t3; 
	P2.p31=   R2.a13; 
	P2.p32=   R2.a23; 
	P2.p33=   R2.a33; 
	P2.p34=  -R2.a13*t2.t1-R2.a23*t2.t2-R2.a33*t2.t3;


	//************************************************************************************************
	//********the position of camera 2 expressed in coordinates of camera 1************
	//********(origin is in camera 1  *************************************************
	//in the code R1 and R2 are defined transposed compared to the report!
	R = mat_mult    (transpose(R1),   R2);
	t = mat_vec_mult(transpose(R1), (vec_sub(t2,t1)));
	delta = sqrt(t.t1*t.t1+t.t2*t.t2+t.t3*t.t3);
	phi    =asin( R.a13);
	kappa1 =acos( R.a11/cos(phi));
	kappa2 =asin(-R.a12/cos(phi));
	omega1 =acos( R.a33/cos(phi));
	omega2 =asin(-R.a23/cos(phi));

	//find the right solution
	if(fabs(R.a21 - Rot_mat_elem(omega1,phi,kappa1,2,1)) < 0.00001)
		{kappa=kappa1; omega=omega1;}
	else if(fabs(R.a21 - Rot_mat_elem(omega1,phi,kappa2,2,1)) < 0.00001)
		{kappa=kappa2; omega=omega1;}
	else if(fabs(R.a21 - Rot_mat_elem(omega2,phi,kappa1,2,1)) < 0.00001)
		{kappa=kappa1; omega=omega2;}	
	else if(fabs(R.a21 - Rot_mat_elem(omega2,phi,kappa2,2,1)) < 0.00001)
		{kappa=kappa2;omega=omega2;}

	//********************************************************************************************
	//********************************************************************************************
	//*********calculate essential matrix E ******************************************************
	Matrix Tt=trans(t);
	Matrix E2=mat_mult(Tt,R);
	//T_x=trans(t);
	//Matrix E2 = mat_mult(T_x, R);
	det_R   = Det(R);
	det_T_x = Det(T_x);
	det_E   = Det(E2);
	//*********end calculate essential matrix E ***************************************************

	file[255];
	strcpy(file,file_cam1_par);
	c=file+strlen(file);
	while(*c != '\\')
		*c--;
	*c='\0';
	strcat(file,"\\rot1_2_matrix.dat");
	fp=fopen(file,"wb");
	fprintf(fp,"The position and rotation of second camera with the origin in the first camera\n");
	fprintf(fp,"%10.4lf%10.4lf%10.4lf\n",R.a11,R.a12,R.a13);
	fprintf(fp,"%10.4lf%10.4lf%10.4lf\n",R.a21,R.a22,R.a23);
	fprintf(fp,"%10.4lf%10.4lf%10.4lf\n",R.a31,R.a32,R.a33);
	fprintf(fp,"The rotational angles\n");
	fprintf(fp,"omega = %8.4lf\n",omega);
	fprintf(fp,"phi   = %8.4lf\n",phi);
	fprintf(fp,"kappa = %8.4lf\n",kappa);
	fprintf(fp,"The position\n");
	fprintf(fp,"CX    = %8.4lf\n",t.t1);
	fprintf(fp,"CY    = %8.4lf\n",t.t2);
	fprintf(fp,"CZ    = %8.4lf\n",t.t3);
	fprintf(fp,"The distance between the cameras\n");
	fprintf(fp,"delta = %8.4lf\n",delta);
	fprintf(fp,"The Essential Matrix to project points from view 2 into view 1\n");
	fprintf(fp,"%10.4lf%10.4lf%10.4lf\n",E2.a11,E2.a12,E2.a13);
	fprintf(fp,"%10.4lf%10.4lf%10.4lf\n",E2.a21,E2.a22,E2.a23);
	fprintf(fp,"%10.4lf%10.4lf%10.4lf\n",E2.a31,E2.a32,E2.a33);
	fprintf(fp,"The camera matrix P of the normalized camera (f=1)\n");
	fprintf(fp,"%10.4lf%10.4lf%10.4lf %10.4lf\n",P2.p11,P2.p12,P2.p13,P2.p14);
	fprintf(fp,"%10.4lf%10.4lf%10.4lf %10.4lf\n",P2.p21,P2.p22,P2.p23,P2.p24);
	fprintf(fp,"%10.4lf%10.4lf%10.4lf %10.4lf\n",P2.p31,P2.p32,P2.p33,P2.p34);
	fprintf(fp,"Det(E)\n");
	fprintf(fp,"%10.4lf\n",det_E);
	_strdate(tmpbuf);
	fprintf(fp, "Data created: %s",tmpbuf);
	fclose(fp);
}
//************************************************************************************************
//************************************************************************************************
//************************************************************************************************

double Rot_mat_elem(double omega, double phi, double kappa, int row, int col)
{
	if(row==1 && col==1)
		return cos(phi)*cos(kappa);
	else if(row==1 && col==2)
		return -cos(phi)*sin(kappa);
	else if(row==1 && col==3)
		return sin(phi);
	else if(row==2 && col==1)
		return cos(omega)*sin(kappa) + sin(omega)*sin(phi)*cos(kappa);
	else if(row==2 &&col==2)
		return cos(omega)*cos(kappa) - sin(omega)*sin(phi)*sin(kappa);
	else if(row==2 && col==3)
		return -sin(omega)*cos(phi);  
	else if(row==3 && col==1)
		return sin(omega)*sin(kappa) - cos(omega)*sin(phi)*cos(kappa);
	else if(row==3 && col==2)
		return sin(omega)*cos(kappa) + cos(omega)*sin(phi)*sin(kappa);
	else if(row==3 && col==3)
		return cos(omega)*cos(phi);
	else 
		return -9999;
}

