#include "stdafx.h"
#include <stdio.h>
#include <imsls.h>
#include <math.h>
#include "niimaq.h"
#include "parameters.h"
#include "matrix_op.h"
#include "path.h"
#include "camera.h"
#include "plot.h"
#include "vrml_generator.h"


int  VRML::vrml_points_gen(char *file, CProgressCtrl *bar, CDC *device, CEdit *m_status)
{
	int i,j,file_i,number=0,max_id=0;

	j = strlen(file);
	i=j;
	while(file[i] != '\\')
		i--;
	i--;
	while(file[i] != '\\')
		i--;

	file_i=i;
	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\3Dpoints.dat");	

	//sprintf(&file_3d[file_i+1], "points.wrl");
	FILE *fp1 = fopen(file,"rb");
	if(fp1 == 0)
		return 1;


	double dummy;
	int i_dummy;
	int total_elem=0;
	while(fscanf(fp1,"%i%lf%lf%lf",&i_dummy,&dummy, &dummy, &dummy) != EOF)
		total_elem++;	

	//rewind the file 
	rewind(fp1);

	int *seg =new int[total_elem];
	double *x =new double[total_elem];
	double *y =new double[total_elem];
	double *z =new double[total_elem];

	i=0;
	while(fscanf(fp1,"%i%lf%lf%lf",&seg[i], &x[i], &y[i], &z[i]) != EOF)
		i++;
	fclose(fp1);

	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\points.wrl");	
	FILE *fp2=fopen(file,"wb");

	fprintf(fp2,"%s\n","#VRML V2.0 utf8");												  
	fprintf(fp2,"%s\n","	Group{ ");														  
	fprintf(fp2,"%s\n","	children[ ");
	double color;
	for(i=0;i<total_elem;i++)
	{
		if(seg[i]%2 ==0)
			color=1;
		else
			color=0.5;
		Draw_Sphere(fp2, 3, x[i], y[i], z[i], color, 1-color, 0);
	}
	fprintf(fp2,"%s\n","	]#children");														  
	fprintf(fp2,"%s\n","	}#Group");	
	fclose(fp2);
	delete []seg;
	delete []x;
	delete []y;
	delete []z;


//******************THE SPLINE***********************************************************
//***************************************************************************************
	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\3Dspline_points.dat");
	fp1 = fopen(file,"rb");
	if(fp1 == 0)
		return 1;

	total_elem=0;
	while(fscanf(fp1,"%i%i%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",&i_dummy, &i_dummy, &dummy, &dummy, &dummy, 
		                                                        &dummy, &dummy, &dummy, &dummy, &dummy,  &dummy,
															                    &dummy, &dummy, &dummy,
																				&dummy, &dummy, &dummy) != EOF)
	total_elem++;	

	//rewind the file 
	rewind(fp1);


	int    *path	=new int[total_elem];

	seg =new int[total_elem];
	x	=new double[total_elem];
	y	=new double[total_elem];
	z	=new double[total_elem];
	double *s_length=new double[total_elem];
	double *curv	=new double[total_elem];
	double *torsion	=new double[total_elem];
	double *tx	=new double[total_elem];
	double *ty	=new double[total_elem];
	double *tz	=new double[total_elem];
	double *nx	=new double[total_elem];
	double *ny	=new double[total_elem];
	double *nz	=new double[total_elem];
	double *bx	=new double[total_elem];
	double *by	=new double[total_elem];
	double *bz	=new double[total_elem];
	i=0;

	while(fscanf(fp1,"%i%i%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf", 
											&path[i], &seg[i], 
		                                    &x[i], &y[i], &z[i], 
											&s_length[i], &curv[i], &torsion[i], 
											&tx[i], &ty[i], &tz[i], 
											&nx[i], &ny[i], &nz[i],
											&bx[i], &by[i], &bz[i]) != EOF)
		i++;
	fclose(fp1);
	Plot::display(-99,"Data file read: ", m_status);

	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\stat.dat");	
	FILE *stat_f=fopen(file,"wb");
	//***********************the total number of path lines****************************************
	int path_line_tot_nr= path[total_elem-1];
	Plot::display(path_line_tot_nr,"Total number of path lines: ", m_status);
	fprintf(stat_f,"Total number of path lines	%i\n",  path_line_tot_nr);
	//***********************get the path segment number for each path line************************
	double *seg_tot_nr=new double[path_line_tot_nr];
	for(i=0;i<path_line_tot_nr;i++)
		seg_tot_nr[i]=0;
	//***********************total number of path line segments************************************
	j=0;
	for(i=0;i<total_elem-1;i++)
	{
		if((path[i]==path[i+1]) && (seg[i] != seg[i+1]))
			seg_tot_nr[j]++;
		else if(path[i]!=path[i+1])
		{
			seg_tot_nr[j]++;
			j++;
		}
	}
	
	int seg_total=1;
	for(i=0;i<total_elem-1;i++)
	{
		if(seg[i] != seg[i+1])
			seg_total++;
	}
	Plot::display(seg_total,"Total number of segments: ", m_status);

	double *seg_stat=imsls_d_simple_statistics (path_line_tot_nr, 1, seg_tot_nr, 0);
	double mean_seg_nr=		seg_stat[0];
	double max_seg_nr=		seg_stat[6];
	fprintf(stat_f,"Total number of segments                  %i\n",  seg_total);
	fprintf(stat_f,"Average number of segments per path line  %f\n",  mean_seg_nr);
	fprintf(stat_f,"Maximal number of segments per path line  %i\n",  (int)max_seg_nr);
	//*********************************************************************************************
	//get the mean and the standart deviation of the velocity
	const double shutter = 0.0083333333;
	Plot::display(-99,"Calculate segment length statistics ", m_status);
	double *vel_stat=imsls_d_simple_statistics (total_elem, 1, s_length, IMSLS_MEDIAN, 0);
	double mean_vel=		vel_stat[0];
	double variance_vel=	vel_stat[1];
	double deviation_vel=	vel_stat[2];
	double skewness_vel=	vel_stat[3];
	double kurtosis_vel=	vel_stat[4];
	double min_vel=			vel_stat[5];
	double max_vel=			vel_stat[6];
	double range_vel=		vel_stat[7];
	double median_vel=		vel_stat[13];
	double median_dev_vel=	vel_stat[14];

	double min_velocity=mean_vel-1.7*deviation_vel;  //1.5
	double max_velocity=mean_vel+1.7*deviation_vel; //1.5 

	//get the mean  and the standart deviation of the curvature
	double curvature_dev;
	double av_curvature=imsls_d_normal_one_sample(total_elem, curv, IMSLS_STD_DEV, &curvature_dev,0);
	double max_curvature=av_curvature+0.5*curvature_dev;
	double min_curvature=av_curvature-0.6*curvature_dev;

	//get the mean  and the standart deviation of the torsion
	double torsion_dev;
	double av_torsion=imsls_d_normal_one_sample(total_elem, torsion, IMSLS_STD_DEV, &torsion_dev,0);
	double max_torsion=av_torsion+0.1*torsion_dev;
	double min_torsion=av_torsion-0.1*torsion_dev;


	fprintf(stat_f,"Average velocity based on segment length    %4.2f\n",  mean_vel/shutter/1000);
	fprintf(stat_f,"Deviation velocity based on segment length  %4.2f\n",  deviation_vel/shutter/1000);
	fprintf(stat_f,"Maximal velocity based on segment length    %4.2f\n",  max_vel/shutter/1000);
	fprintf(stat_f,"Minimal velocity based on segment length    %4.2f\n",  min_vel/shutter/1000);

	//colors
	//the lowest velocity gets a blue color
	//the highest gets a red color
	double r_col,g_col,b_col;

	//**************************************************************************************************************
	//**************************************************************************************************************
	//**************************************************************************************************************
	//**************************************************************************************************************
	//get average flow direction based on the segment spline points
	double dir_x=0;
	double dir_y=0;
	double dir_z=0;

	for(i=0; i<total_elem;i++)
	{
		if(path[i]==path[i+1])
		{
			dir_x+=x[i+1]-x[i];
			dir_y+=y[i+1]-y[i];
			dir_z+=z[i+1]-z[i];
		}
	}
	dir_x=dir_x/total_elem;
	dir_y=dir_y/total_elem;
	dir_z=dir_z/total_elem;

	double x_vel_av=dir_x/shutter;
	double y_vel_av=dir_y/shutter;
	double z_vel_av=dir_z/shutter;
	double vel_av  = sqrt(x_vel_av*x_vel_av + y_vel_av*y_vel_av + z_vel_av*z_vel_av);
	//**************************************************************************************************************
	//get average flow direction based on the segment end points
	Plot::display(-99,"Calc. seg length based on endpoints", m_status);
	double x1=x[0];
	double y1=y[0];
	double z1=z[0];

	double *dx =new double[seg_total];
	double *dy =new double[seg_total];
	double *dz =new double[seg_total];
	for(i=0;i<seg_total;i++)
	{
		dx[i]=0;
		dy[i]=0;
		dz[i]=0;
	}
	
	j=0;
	for(i=0; i<total_elem-1;i++)
	{
		if((seg[i] != seg[i+1]))
		{	
			dx[j]=x[i]-x1;
			dy[j]=y[i]-y1;
			dz[j]=z[i]-z1;
			j++;
			
			x1=x[i+1];
			y1=y[i+1];
			z1=z[i+1];
		}
		if(j>=seg_total)
		{
			Plot::display(-99,"Problem with seg_total", m_status);
			return 0;
		}
	}

	seg_stat=imsls_d_simple_statistics (seg_total, 1, dx, 0);
	double mean_dx=			seg_stat[0];
	double deviation_dx=	seg_stat[2];
	seg_stat=imsls_d_simple_statistics (seg_total, 1, dy, 0);
	double mean_dy=			seg_stat[0];
	double deviation_dy=	seg_stat[2];
	seg_stat=imsls_d_simple_statistics (seg_total, 1, dz, 0);
	double mean_dz=			seg_stat[0];
	double deviation_dz=	seg_stat[2];

	double av_vel=sqrt(mean_dx*mean_dx + mean_dy*mean_dy + mean_dz*mean_dz);

	fprintf(stat_f,"Average velocity in x dir based on seg endpoints    %4.2f\n",  mean_dx/shutter/1000);
	fprintf(stat_f,"Average velocity in y dir based on seg endpoints    %4.2f\n",  mean_dy/shutter/1000);
	fprintf(stat_f,"Average velocity in z dir based on seg endpoints    %4.2f\n",  mean_dz/shutter/1000);
	fprintf(stat_f,"Average deviation in x dir based on seg endpoints   %4.2f\n",  deviation_dx/shutter/1000);
	fprintf(stat_f,"Average deviation in y dir based on seg endpoints   %4.2f\n",  deviation_dy/shutter/1000);
	fprintf(stat_f,"Average deviation in z dir based on seg endpoints   %4.2f\n",  deviation_dz/shutter/1000);
	fprintf(stat_f,"Average velocity based on endpoints                 %4.2f\n",  av_vel/shutter/1000);

	//**************************************************************************************************************
	//**************************************************************************************************************
	//**************************************************************************************************************
	//**************************************************************************************************************
	Plot::display(-99,"Calculate projections", m_status);
	double *x_stat=imsls_d_simple_statistics (total_elem, 1,x, 0);
	double mean_x=		x_stat[0];
	double *y_stat=imsls_d_simple_statistics (total_elem, 1,y, 0);
	double mean_y=		y_stat[0];
	double *z_stat=imsls_d_simple_statistics (total_elem, 1,z, 0);
	double mean_z=		z_stat[0];

	//define cubus direction
	double cube_dir_x  =  dir_x;
	double cube_dir_y  =  dir_y;
	double cube_dir_z  =  dir_z;
	//define cubus position
	double cube_pos_x  =  -400;
	double cube_pos_y  =  300;
	double cube_pos_z  =  -2000;
	double cube_width  =  2000;

	double cube_dir_l=sqrt(cube_dir_x*cube_dir_x + cube_dir_y*cube_dir_y + cube_dir_z*cube_dir_z);
	cube_dir_x /= cube_dir_l;
	cube_dir_y /= cube_dir_l;
	cube_dir_z /= cube_dir_l;

	double cube_pos_x1=cube_pos_x + cube_width/2*cube_dir_x;
	double cube_pos_y1=cube_pos_y + cube_width/2*cube_dir_y;
	double cube_pos_z1=cube_pos_z + cube_width/2*cube_dir_z;

	double cube_pos_x2=cube_pos_x - cube_width/2*cube_dir_x;
	double cube_pos_y2=cube_pos_y - cube_width/2*cube_dir_y;
	double cube_pos_z2=cube_pos_z - cube_width/2*cube_dir_z;

	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\planes.wrl");
	fp2=fopen(file,"wb");
	fprintf(fp2,"%s\n","#VRML V2.0 utf8");
	Draw_Plane(fp2, cube_pos_x1, cube_pos_y1, cube_pos_z1, dir_x, dir_y, dir_z);
	Draw_Plane(fp2, cube_pos_x2, cube_pos_y2, cube_pos_z2, dir_x, dir_y, dir_z);
	fclose(fp2);

	double d1_x,d1_y, d1_z,  d2_x, d2_y,  d2_z, d;

	double px,py,pz,d1,d2,d_x,d_y,d_z,dm;

	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\projections.wrl");
	fp2=fopen(file,"wb");
	fprintf(fp2,"%s\n","#VRML V2.0 utf8");
	//check if inside cubus
	for(i=0; i<total_elem;i+=40)
	{
		d_x=cube_dir_x*(cube_pos_x-x[i]);
		d_y=cube_dir_y*(cube_pos_y-y[i]);
		d_z=cube_dir_z*(cube_pos_z-z[i]);
		dm=d_x+d_y+d_z;
		d1_x=cube_dir_x*(cube_pos_x1-x[i]);
		d1_y=cube_dir_y*(cube_pos_y1-y[i]);
		d1_z=cube_dir_z*(cube_pos_z1-z[i]);
		d1=d1_x+d1_y+d1_z;
		d2_x=cube_dir_x*(cube_pos_x2-x[i]);
		d2_y=cube_dir_y*(cube_pos_y2-y[i]);
		d2_z=cube_dir_z*(cube_pos_z2-z[i]);
		//check the direction
		d2=d2_x+d2_y+d2_z;

		d=d1*d2;
		if(d<0)
		{
			r_col=(s_length[i]-min_velocity)/(max_velocity-min_velocity);
			if(r_col<0) 
				r_col=0;
			else if(r_col>1) 
				r_col=1;
			g_col=0;
			b_col=1-r_col;

			px=x[i]+dm*cube_dir_x;
			py=y[i]+dm*cube_dir_y;
			pz=z[i]+dm*cube_dir_z;
			Draw_Sphere(fp2,2, px, py, pz, r_col, g_col, b_col);
		}
	}
	fclose(fp2);
	//**************************************************************************************************************
	//**************************************************************************************************************
	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\planes2.wrl");
	fp2=fopen(file,"wb");
	fprintf(fp2,"%s\n","#VRML V2.0 utf8");
	const int pl_nr=20;
	double x_av_vel[pl_nr]={0};
	double y_av_vel[pl_nr]={0};
	double z_av_vel[pl_nr]={0};
	int    av_vel_nr[pl_nr]={0};
	double cube_x[pl_nr];
	double cube_y[pl_nr];
	double cube_z[pl_nr];
	double cube_w     =  100;
	cube_pos_x  =  mean_x-pl_nr/2*cube_w*cube_dir_x;
	cube_pos_y  =  mean_y-pl_nr/2*cube_w*cube_dir_y;
	cube_pos_z  =  mean_z-pl_nr/2*cube_w*cube_dir_z;
	for(i=0;i<pl_nr;i++)
	{
		cube_x[i]=cube_pos_x + i*cube_w*cube_dir_x;
		cube_y[i]=cube_pos_y + i*cube_w*cube_dir_y;
		cube_z[i]=cube_pos_z + i*cube_w*cube_dir_z;
		Draw_Plane(fp2, cube_x[i], cube_y[i], cube_z[i], dir_x, dir_y, dir_z);
	}

	//check if inside cubus
	for(j=0;j<pl_nr-1;j++)
	{
		for(i=0; i<total_elem;i++)
		{
			//d_x=cube_dir_x*(cube_pos_x-x[i]);
			//d_y=cube_dir_y*(cube_pos_y-y[i]);
			//d_z=cube_dir_z*(cube_pos_z-z[i]);
			//dm=d_x+d_y+d_z;
			d1_x=cube_dir_x*(cube_x[j]-x[i]);
			d1_y=cube_dir_y*(cube_y[j]-y[i]);
			d1_z=cube_dir_z*(cube_z[j]-z[i]);
			d1=d1_x+d1_y+d1_z;
			d2_x=cube_dir_x*(cube_x[j+1]-x[i]);
			d2_y=cube_dir_y*(cube_y[j+1]-y[i]);
			d2_z=cube_dir_z*(cube_z[j+1]-z[i]);
			//check the direction
			d2=d2_x+d2_y+d2_z;

			d=d1*d2;
			if(d<0)
			{
				x_av_vel[j]+=s_length[i];
				//y_av_vel[j]+=s_length[i];
				//z_av_vel[j]+=s_length[i];
				av_vel_nr[j]++;

				r_col=(s_length[i]-min_velocity)/(max_velocity-min_velocity);
				if(r_col<0) 
					r_col=0;
				else if(r_col>1) 
					r_col=1;
				g_col=0;
				b_col=1-r_col;

				px=x[i]+d1*cube_dir_x;
				py=y[i]+d1*cube_dir_y;
				pz=z[i]+d1*cube_dir_z;
				Draw_Sphere(fp2,2, px, py, pz, r_col, g_col, b_col);
			}
		}
	}
	fclose(fp2);
	fprintf(stat_f,"\n\n Averages in segments\n\n");
	for(j=0;j<pl_nr ;j++)
	{
		if(av_vel_nr[j] != 0)
		{
			fprintf(stat_f,"Average velocity  in %i is    %4.2f\n", j, x_av_vel[j]/av_vel_nr[j]/shutter/1000);
			fprintf(stat_f,"Sample number in     %i is    %i   \n", j, av_vel_nr[j]);
			//fprintf(stat_f,"Average velocity in y dir in %i is    %4.2f\n", j, y_av_vel[j]/av_vel_nr[j]/shutter/1000);
			//fprintf(stat_f,"Average velocity in z dir in %i is    %4.2f\n", j, z_av_vel[j]/av_vel_nr[j]/shutter/1000);
		}
		else 
			fprintf(stat_f,"No samples in        %i \n", j);
	}
	fclose(stat_f);

	//**************************************************************************************************************
	//**************************************************************************************************************
/*
	for(i=0; i<total_elem;i++)
	{
		mean_x
		mean_y
		mean_z
		x[i]);
		y[i]);
		z[i]);

*/
	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\plate.wrl");
	fp2=fopen(file,"wb");
	fprintf(fp2,"%s\n","#VRML V2.0 utf8");

	Draw_Cylinder_dir(fp2, 570/2, mean_x-320, mean_y, mean_z, cube_dir_x, cube_dir_y, cube_dir_z, 15, 0.2, 0.1, 0.4, 0.1);

	fclose(fp2);

	//**************************************************************************************************************
	//**************************************************************************************************************
	//**************************************************************************************************************
	//**************************************************************************************************************
	//**************************************************************************************************************
	//**************************************************************************************************************
	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\path_velocity.wrl");
	fp2=fopen(file,"wb");
	fprintf(fp2,"%s\n","#VRML V2.0 utf8");	
 
	//Draw the Path lines with cylinder with velocity color code
	Plot::display(-99,"Path lines with velocity code", m_status);
	i=0;
	while(i<total_elem-1)
	{
		r_col=(s_length[i]-min_velocity)/(max_velocity-min_velocity);
		if(r_col<0) 
			r_col=0;
		else if(r_col>1) 
			r_col=1;
		g_col=0;
		b_col=1-r_col;

		if(path[i]==path[i+1])
			Draw_Cylinder(fp2, 2, x[i], y[i], z[i], x[i+1], y[i+1], z[i+1], r_col, g_col, b_col);
		i++;
	}
	fclose(fp2);


	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\path_extrude.wrl");
	fp2=fopen(file,"wb");
	fprintf(fp2,"%s\n","#VRML V2.0 utf8");	

	//Draw the Path lines with Extrusion nodes
	Plot::display(-99,"Extrusion path lines", m_status);
	i=0;
	int j_max;
	while(i<total_elem-1)
	{
		r_col=(s_length[i]-min_velocity)/(max_velocity-min_velocity);
		if(r_col<0) 
			r_col=0;
		else if(r_col>1) 
			r_col=1;
		g_col=0;
		b_col=1-r_col;
		
		fprintf(fp2,"%s\n","	Group{ ");														  
		fprintf(fp2,"%s\n","	children[ ");
		fprintf(fp2,"%s\n","		Shape{ ");														  
		fprintf(fp2,"%s\n","			appearance Appearance {");					  
		fprintf(fp2,"%s\n","			material Material {");	
		//fprintf(fp3,"				      emissiveColor %4.2f  %4.2f %4.2f\n", r_col, g_col, b_col);		
		fprintf(fp2,"      			      diffuseColor  %4.2f  %4.2f %4.2f\n", r_col, g_col, b_col);		
		fprintf(fp2,"%s\n","	 		}");										  
		fprintf(fp2,"%s\n","			}");
		fprintf(fp2,"%s\n","			geometry Extrusion {");	 			  
		fprintf(fp2,"%s\n","				crossSection [");	
		fprintf(fp2,"%s\n","				3 1.5, 3 -1.5, -3 -1.5, -3 1.5, 3 1.5");
		//fprintf(fp2,"%s\n","				1 1, 1 -1, -1 -1, 1 1");
		fprintf(fp2,"%s\n","				]");
		fprintf(fp2,"%s\n","				spine	[");
		j_max=0;
		while(seg[i+1] == seg[i])
		{
			fprintf(fp2,"					%8.1f %8.1f %8.1f,\n", x[i], y[i], z[i]);	
			i++;
			j_max++;
		}
		i++;
		if(path[i] == path[i-1])
			fprintf(fp2,"					%8.1f %8.1f %8.1f,\n", x[i], y[i], z[i]);	
		fprintf(fp2,"%s\n","				]");
		fprintf(fp2,"%s\n","				orientation [");
		for(j=0;j<j_max;j++)
		{
			double angle=(3.1/j_max)*j;
			fprintf(fp2,"						%8.1f %8.1f %8.1f %8.1f,\n", 0.0, 1.0, 0.0,1.0);
		}
		fprintf(fp2,"%s\n","				]");
		fprintf(fp2,"%s\n","				scale [");
		for(j=0;j<j_max;j++)
		{
			fprintf(fp2,"						%8.1f %8.1f,\n", 1.0, 1.0);
		}
		fprintf(fp2,"%s\n","				]");
		fprintf(fp2,"%s\n","				beginCap TRUE");
		fprintf(fp2,"%s\n","				endCap   TRUE");
		fprintf(fp2,"%s\n","				solid    TRUE");
		fprintf(fp2,"%s\n","				creaseAngle 2");
		fprintf(fp2,"%s\n","			}");
		fprintf(fp2,"%s\n","		}");
		fprintf(fp2,"%s\n","	]#children");														  
		fprintf(fp2,"%s\n","	}#Group");	
	}
	fclose(fp2);
	//End Draw the Path lines with Extrusion nodes
	//**************************************************************************************************************
	//**************************************************************************************************************
	//**************************************************************************************************************
	//**************************************************************************************************************
	//Draw the Path lines with cylinder with curvature color code
	Plot::display(-99,"Path lines with curvature code", m_status);
	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\path_curvature.wrl");
	fp2=fopen(file,"wb");
	fprintf(fp2,"%s\n","#VRML V2.0 utf8");	

	i=0;
	j=0;
	while(i<total_elem-1)
	{
		r_col=(curv[i]-min_curvature)/(max_curvature-min_curvature);
		if(r_col<0)
			r_col=0;
		else if(r_col>1) 
			r_col=1;
		g_col=0;
		b_col=1-r_col;

		if(path[i]==path[i+1])
			Draw_Cylinder(fp2, 4, x[i], y[i], z[i], x[i+1], y[i+1], z[i+1], r_col, g_col, b_col);

		i++;
	}
	fclose(fp2);
	//**************************************************************************************************************
	//**************************************************************************************************************

	//**************************************************************************************************************
	//**************************************************************************************************************
	//Draw the Path lines with cylinder with torsion color code
	Plot::display(-99,"Path lines with torsion code", m_status);
	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\path_torsion.wrl");
	fp2=fopen(file,"wb");
	fprintf(fp2,"%s\n","#VRML V2.0 utf8");	
	i=0;
	while(i<total_elem-1)
	{
		if(torsion[i]>0)  //yellow->red
		{
			r_col=1;
			g_col=torsion[i]/max_torsion;
			if(g_col>1)
				g_col=1;
			b_col=0;

		}
		else   
		{
			r_col=1;
			b_col=torsion[i]/min_torsion;
			if(b_col>1)
				b_col=1;
			g_col=b_col;

		}
		/*
		else   //green ->blue
		{
			b_col=torsion[i]/min_torsion;
			if(b_col>1)
				b_col=1;
			g_col=1-b_col;
			r_col=0;
		}
		*/

		if(path[i]==path[i+1])
			Draw_Cylinder(fp2, 4, x[i], y[i], z[i], x[i+1], y[i+1], z[i+1], r_col, g_col, b_col);
		i++;
	}
	fclose(fp2);
	//**************************************************************************************************************
	//**************************************************************************************************************

	//**************************************************************************************************************
	//**************************************************************************************************************
	//Draw the center of rotation
	Plot::display(-99,"Center of rotation", m_status);
	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\vortex.wrl");
	fp2=fopen(file,"wb");
	fprintf(fp2,"%s\n","#VRML V2.0 utf8");	
	i=0;
	while(i<total_elem-1)
	{
		double xr,yr,zr;
		if(curv[i] !=0)
		{
			//the radius
			double radius=1/curv[i];
			if(radius<200)
			{
				xr=x[i]+radius*tx[i];
				yr=y[i]+radius*ty[i];
				zr=z[i]+radius*tz[i];
				Draw_Sphere(fp2, 2, xr,  yr,  zr,  0,  0.8,  0.4);
			}
		}
		i++;
	}
	//**************************************************************************************************************
	//**************************************************************************************************************
	//Draw the center of torsion
	Plot::display(-99,"Center of torsion", m_status);
	fclose(fp2);
	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\torsion.wrl");
	fp2=fopen(file,"wb");
	fprintf(fp2,"%s\n","#VRML V2.0 utf8");	
	i=0;
	while(i<total_elem-1)
	{
		double xr,yr,zr;
		//the radius
		if(torsion[i] !=0)
		{
			double radius=1/torsion[i];
			if(radius<400)
			{
				xr=x[i]+radius*nx[i];
				yr=y[i]+radius*ny[i];
				zr=z[i]+radius*nz[i];
				Draw_Sphere(fp2, 2, xr,  yr,  zr,  0.1,  0.3,  0.4);
				Draw_Cylinder(fp2, 2, x[i], y[i], z[i], xr, yr, zr, 0.7, 0.2, 0.3);
			}
		}
		i++;
	}
	//**************************************************************************************************************
	//**************************************************************************************************************

	//**************************************************************************************************************
	//**************************************************************************************************************
	//draw the Frenet frame: unit tangent, unit normal and unit bi normal vector
	Plot::display(-99,"Frenet frame", m_status);
	fclose(fp2);
	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\diff.wrl");
	fp2=fopen(file,"wb");
	fprintf(fp2,"%s\n","#VRML V2.0 utf8");	
	i=0;
	while(i<total_elem-1)
	{
			Draw_Cylinder(fp2, 0.5, x[i], y[i], z[i], (x[i]+tx[i]), (y[i]+ty[i]), (z[i]+tz[i]), 50, 0.5, 0.2, 0.6);
			Draw_Cylinder(fp2, 0.5, x[i], y[i], z[i], (x[i]+nx[i]), (y[i]+ny[i]), (z[i]+nz[i]), 50, 0.7, 0.2, 0.2);
			Draw_Cylinder(fp2, 0.5, x[i], y[i], z[i], (x[i]+bx[i]), (y[i]+by[i]), (z[i]+bz[i]), 50, 0.9, 0.4, 0.2);
			i+=5;
	}
	//**************************************************************************************************************
	//**************************************************************************************************************
	Plot::display(-99,"Path points", m_status);	
	fclose(fp2);
	file[file_i]='\0';
	strcat(&file[file_i],"\\Result\\path_points.wrl");
	fp2=fopen(file,"wb");
	fprintf(fp2,"%s\n","#VRML V2.0 utf8");													  
	fprintf(fp2,"%s\n","	Group{ ");														  
	fprintf(fp2,"%s\n","	children[ ");
	for(i=0;i<total_elem-1;i++)
		Draw_Sphere(fp2, 3, x[i],  y[i],  z[i],  1,  0.5,  0.3);
		  
	fprintf(fp2,"%s\n","	]#children");														  
	fprintf(fp2,"%s\n","	}#Group");	
	fclose(fp2);

	Plot::display(-99,"Delete variables", m_status);

	delete []path;
	delete []seg;
	delete []x;
	delete []y;
	delete []z;
	delete []s_length;
	delete []curv;
	delete []torsion;
	delete []tx;
	delete []ty;
	delete []tz;
	delete []nx;
	delete []ny;
	delete []nz;
	delete []bx;
	delete []by;
	delete []bz;
	delete []dx;
	delete []dy;
	delete []dz;
	//delete []seg_tot_nr;
	Plot::display(-99,"Deleted", m_status);
	return 0;

}




void VRML::Draw_Sphere(FILE *file, double radius, double x, double y, double z, double r, double g, double b)
{
		fprintf(file,"%s\n","		Transform {");										  
		fprintf(file,"                 translation  %8.1lf %8.1lf %8.1lf\n", x, y, z);	
	    fprintf(file,"%s\n",	"		   children [");
	    fprintf(file,"%s\n",	"				Shape {");								  
		fprintf(file,"%s\n","				appearance Appearance {");					  
		fprintf(file,"%s\n","				material Material {");	
		fprintf(file,"							diffuseColor   %8.2f  %8.2f %8.2f\n", r, g, b);	
		fprintf(file,"							specularColor  %8.2f  %8.2f %8.2f\n", r, g, b);	
		fprintf(file,"					 	    emissiveColor  %8.2f  %8.2f %8.2f\n", r, g, b);
	    fprintf(file,"%s\n","					shininess	0.7");		
		fprintf(file,"%s\n","					transparency  0.3");
		fprintf(file,"%s\n","	 		    }");										  
		fprintf(file,"%s\n","				}");											  
		fprintf(file,"%s\n","				geometry Sphere {");	 			  
		fprintf(file,"							radius %8.2f\n", radius);								  
		fprintf(file,"%s\n","				}");											  
		fprintf(file,"%s\n","				}");
		fprintf(file,"%s\n",	"          ]");
		fprintf(file,"%s\n",	"      }");
		fprintf(file,"\n");		
}


void VRML::Draw_Cylinder(FILE *file, double radius, double x1, double y1, double z1, 
						                double x2, double y2, double z2, double r, double g, double b)
{
		double lx=x2-x1;
		double ly=y2-y1;
		double lz=z2-z1;
		double mx=x2-lx/2.0;
		double my=y2-ly/2.0;
		double mz=z2-lz/2.0;

		double l=sqrt(lx*lx+ly*ly+lz*lz);

		double alpha=acos(ly/l);

		double nx=lz;
		double ny=0.0;
		double nz=-lx;
		fprintf(file,"%s\n","		Group{ ");
		fprintf(file,"%s\n","		children[ ");
		fprintf(file,"%s\n","			Transform {");
		fprintf(file,"					translation  %4.2f  %4.2f %4.2f\n", mx,my,mz);
		fprintf(file,"					rotation     %4.2f  %4.2f %4.2f %4.2f\n", nx,ny,nz,alpha);
		fprintf(file,"%s\n","			children[ "	);
		fprintf(file,"%s\n","			Shape {");
		fprintf(file,"%s\n","			appearance Appearance {");
		fprintf(file,"%s\n","				material Material {");	
		fprintf(file,"						diffuseColor   %8.2f  %8.2f %8.2f\n", r*0.6, g*0.6, b*0.6);
		fprintf(file,"						emissiveColor  %8.2f  %8.2f %8.2f\n", r, g, b);
		fprintf(file,"%s\n","	 			}#material");			
		fprintf(file,"%s\n","	 		}#apperance");				
		fprintf(file,"%s\n","			geometry  Cylinder {"); 	
		fprintf(file,"					height %4.2f",l);	
		fprintf(file,"  			    radius %4.2f",radius);
		fprintf(file,"%s\n","			}#geometry");
		fprintf(file,"%s\n","			}#shape");
		fprintf(file,"%s\n","			]#children");
		fprintf(file,"%s\n","			}#transformation");
		fprintf(file,"%s\n","		] ");	
		fprintf(file,"%s\n","		} ");
}



void VRML::Draw_Cylinder(FILE *file, double radius, double x1, double y1, double z1, 
						                double x2, double y2, double z2, double length, double r, double g, double b)
{
		//This draws a cylinder in the x1-x2 direction with ome end at x1 and 
		//and a lenght of "length"
		double lx=x2-x1;
		double ly=y2-y1;
		double lz=z2-z1;
		double l=sqrt(lx*lx+ly*ly+lz*lz);
		double alpha=acos(ly/l);
		double nx=lz;
		double ny=0.0;
		double nz=-lx;

		double mx=x1+0.5*length*lx/l;
		double my=y1+0.5*length*ly/l;
		double mz=z1+0.5*length*lz/l;

		fprintf(file,"%s\n","		Group{ ");
		fprintf(file,"%s\n","		children[ ");
		fprintf(file,"%s\n","			Transform {");
		fprintf(file,"					translation  %4.2f  %4.2f %4.2f\n", mx,my,mz);
		fprintf(file,"					rotation     %4.2f  %4.2f %4.2f %4.2f\n", nx,ny,nz,alpha);
		fprintf(file,"%s\n","			children[ "	);
		fprintf(file,"%s\n","			Shape {");
		fprintf(file,"%s\n","			appearance Appearance {");
		fprintf(file,"%s\n","				material Material {");	
		fprintf(file,"						diffuseColor   %8.2f  %8.2f %8.2f\n", r*0.6, g*0.6, b*0.6);
		fprintf(file,"						emissiveColor  %8.2f  %8.2f %8.2f\n", r, g, b);
		fprintf(file,"%s\n","	 			}#material");			
		fprintf(file,"%s\n","	 		}#apperance");				
		fprintf(file,"%s\n","			geometry  Cylinder {"); 	
		fprintf(file,"					height %4.2f",length);	
		fprintf(file,"  			    radius %4.2f",radius);
		fprintf(file,"%s\n","			}#geometry");
		fprintf(file,"%s\n","			}#shape");
		fprintf(file,"%s\n","			]#children");
		fprintf(file,"%s\n","			}#transformation");
		fprintf(file,"%s\n","		] ");	
		fprintf(file,"%s\n","		} ");
}


void VRML::Draw_Cylinder_dir(FILE *file, double radius, double x1, double y1, double z1, 
						                double dir_x, double dir_y, double dir_z, 
										double length, double r, double g, double b, double transp)
{
		//This draws a cylinder in the x1-x2 direction with ome end at x1 and 
		//and a lenght of "length"
		double lx=dir_x;
		double ly=dir_y;
		double lz=dir_z;
		double l=sqrt(lx*lx+ly*ly+lz*lz);
		double alpha=acos(ly/l);
		double nx=lz;
		double ny=0.0;
		double nz=-lx;

		//double mx=x1+0.5*length*lx/l;
		//double my=y1+0.5*length*ly/l;
		//double mz=z1+0.5*length*lz/l;
		double mx=x1;
		double my=y1;
		double mz=z1;

		fprintf(file,"%s\n","		Group{ ");
		fprintf(file,"%s\n","		children[ ");
		fprintf(file,"%s\n","			Transform {");
		fprintf(file,"					translation  %4.2f  %4.2f %4.2f\n", mx,my,mz);
		fprintf(file,"					rotation     %4.2f  %4.2f %4.2f %4.2f\n", nx,ny,nz,alpha);
		fprintf(file,"%s\n","			children[ "	);
		fprintf(file,"%s\n","			Shape {");
		fprintf(file,"%s\n","			appearance Appearance {");
		fprintf(file,"%s\n","				material Material {");	
		fprintf(file,"						diffuseColor   %8.2f  %8.2f %8.2f\n", r*0.6, g*0.6, b*0.6);
		fprintf(file,"						emissiveColor  %8.2f  %8.2f %8.2f\n", r, g, b);
	    fprintf(file,"%s\n","				shininess	0.7");		
		fprintf(file," 				        transparency  %8.2f", transp);
		fprintf(file,"%s\n","	 			}#material");			
		fprintf(file,"%s\n","	 		}#apperance");				
		fprintf(file,"%s\n","			geometry  Cylinder {"); 	
		fprintf(file,"					height %4.2f",length);	
		fprintf(file,"  			    radius %4.2f",radius);
		fprintf(file,"%s\n","			}#geometry");
		fprintf(file,"%s\n","			}#shape");
		fprintf(file,"%s\n","			]#children");
		fprintf(file,"%s\n","			}#transformation");
		fprintf(file,"%s\n","		] ");	
		fprintf(file,"%s\n","		} ");
}


void VRML::Draw_Plane(FILE *file, double px, double py, double pz, double dir_x, double dir_y, double dir_z)
{

		double l=sqrt(dir_x*dir_x+dir_y*dir_y+dir_z*dir_z);
		dir_x = dir_x/l;
		dir_y = dir_y/l;
		dir_z = dir_z/l;

		Draw_Cylinder_dir(file, 4,  px, py, pz, dir_x, dir_y, dir_z, 200, 0.1, 0.1, 0.3, 0.9);

		double ux=	dir_y*dir_z;
		double uy=	dir_x*dir_z;
		double uz=-	2*dir_x*dir_y;
		l=sqrt(ux*ux+uy*uy+uz*uz);
		ux= ux/l;
		uy= uy/l;
		uz= uz/l;
		Draw_Cylinder_dir(file, 4,  px, py, pz, ux, uy, uz, 200, 0.1, 0.1, 0.3, 0.9);


		double vx= dir_y*uz-dir_z*uy;
		double vy= dir_z*ux-dir_x*uz;
		double vz= dir_x*uy-dir_y*ux;

		Draw_Cylinder_dir(file, 4,  px, py, pz, vx, vy, vz, 200, 0.1, 0.1, 0.3, 0.9);


		double x1,y1,z1;
		double x2,y2,z2;
		double x3,y3,z3;
		double x4,y4,z4;

		x1= px	+500*ux + 500*vx;
		x2= px	+500*ux - 500*vx;
		x3= px	-500*ux + 500*vx;
		x4= px	-500*ux - 500*vx;

		y1= py	+500*uy + 500*vy;
		y2= py	+500*uy - 500*vy;
		y3= py	-500*uy + 500*vy;
		y4= py	-500*uy - 500*vy;

		z1= pz	+500*uz + 500*vz;
		z2= pz	+500*uz - 500*vz;
		z3= pz	-500*uz + 500*vz;
		z4= pz	-500*uz - 500*vz;

		fprintf(file,"%s\n","	 Group {");
		fprintf(file,"%s\n","	     children [");
		fprintf(file,"%s\n","	 	Shape {");
		fprintf(file,"%s\n","	 	    geometry IndexedFaceSet {");
		fprintf(file,"%s\n","	 		    coord Coordinate {");
		fprintf(file,"		 	 			    point [ %4.2f  %4.2f %4.2f\n",  x1,y1,z1);
		fprintf(file,"		 	 						%4.2f  %4.2f %4.2f\n",  x2,y2,z2);
		fprintf(file,"		 	 						%4.2f  %4.2f %4.2f\n",  x3,y3,z3);
		fprintf(file,"		 	 						%4.2f  %4.2f %4.2f]\n", x4,y4,z4); 
		fprintf(file,"%s\n","	 			}");
		fprintf(file,"%s\n","	 		coordIndex [ 0, 1, 3, 2]");
		fprintf(file,"%s\n","	 			    ccw	TRUE");
		fprintf(file,"%s\n","	 			    solid	FALSE");
		fprintf(file,"%s\n","	 			    creaseAngle	0");
		fprintf(file,"%s\n","	 		}");
		fprintf(file,"%s\n","	 	    appearance Appearance {");
		fprintf(file,"%s\n","				material Material {");
		fprintf(file,"%s\n","						  diffuseColor  0.3 0.2 0.1");
		fprintf(file,"%s\n","						  emissiveColor 0.0 0.0 0.0");
	    fprintf(file,"%s\n","						  specularColor	0.3 0.2 0.1");
	    fprintf(file,"%s\n","						  shininess	0.26");
		fprintf(file,"%s\n","						  transparency  0.7");
		fprintf(file,"%s\n","	 		    }");
		fprintf(file,"%s\n","	 		}");
		fprintf(file,"%s\n","	 	}");
		fprintf(file,"%s\n","	     ]");
		fprintf(file,"%s\n","	 }");

}