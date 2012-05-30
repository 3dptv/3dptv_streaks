#include "stdafx.h"
#include <math.h>
#include <imsl.h>
#include <stdio.h>
#include "parameters.h"
#include "matrix_op.h"
#include "pathlist.h"
#include "path.h"
#include "spline.h"

extern FILE  *fp1;
extern FILE  *fp2;
extern FILE  *fp3;

Path_seg::Path_seg()
{
	p_nr		=0;
	circular	=FALSE;
	centroid.x	=0;
	centroid.y	=0;
	end_point_index[0]=-1;
	end_point_index[1]=-1;
}

Path_seg::~Path_seg()
{


}

Path::Path()
{
	cor_indicator=0;
	nr_seg=0;
	finished=0;
	matched=0;
	r_corr=0;
	for(int i=0;i<4;i++)
	{
		i_match[i]=0;
		correspondence[i]=0;
	}
	cor_spline=0;
	nr_p=0;		
}



Path::~Path()
{

}


Pixel* Path_seg::operator =(const Pixel& px)
{
	Pixel *py;

	py->x=px.x;
	py->y=px.y;

	return py;
}

CamPoint* Path_seg::operator =(const CamPoint& px)
{
	CamPoint *py;

	py->x=px.x;
	py->y=px.y;
	py->w=px.w;

	return py;
}

void Path::Init()
{
	nr_seg=0;
	finished=0;
}




int Path::Get_contour_frame(Path_seg path_seg[], uInt8 *img_start)
{
	int i,ii,j,jj,l,p,nr,direction,found,i_start,j_start,c_nr;
	//this settings get the true  8-neighbour boundary!
	int const static	s[13]={1-640,1 ,1+640, 640, 640-1, -1, -1-640, -640, 1-640, 1, 1+640, 640, 640-1};
	int const static	n[13]= {1,0,7,6,5,4,3,2,1,0,7,6,5};
	int const static  id[13]={1,1,1,0,-1,-1,-1,0,1,1,1,0,-1},
				jd[13]={-1,0,1,1,1,0,-1,-1,-1,0,1,1,1};

	uInt8 *neigbour;
	uInt8 *img=img_start;
	c_nr=0;

	for(j=0;j<480;j++)
	{
		for(i=0;i<640;i++)	
		{
			direction=0;
			if(*img==255)
			{
				i_start=i;j_start=j;ii=i;jj=j;
				nr=0;
				neigbour=img;
				do{
					l=0;
					found=0;

					l=7-direction;
					for(p=l;p<l+7;p++)
					{
						if(*(neigbour+s[p]) && ii<640  && ii>=0  && jj<480 && jj>=0)
						{
							path_seg[c_nr].pixel[nr  ].x=ii;
							path_seg[c_nr].pixel[nr++].y=jj;
							*neigbour=1;
							neigbour+=s[p];
							direction=n[p];
							ii+=id[p];jj+=jd[p];
							found=1;
							break;
						}
					}	
				}while(ii!=i_start || jj!=j_start);
				path_seg[c_nr].p_nr=nr;
				//if the streak is too small, do not take it!
				if(nr > 5)
					c_nr++;
				//go to end****************************
				while(*++img!=0){i++;}
			} //end if ==255
			else if(*img==1) //border pixel
			{
				while(*++img!=0){i++;}
			}
			img++;
		}//i for 
	}//j for
	return c_nr;
}




int Path::Get_contour_line(PathList pathlist, uInt8 *img_start, int x_pos)
{
	int i,ii,j,jj,l,p,nr,direction,found,i_start,j_start,c_nr,path_tot_nr=0;
	//this settings get the true  8-neighbour boundary!
	//this code does not search the whole image, instead it does
	//a search only on a predefined vertical line 
	//
	//on this line it finds all path segments and returns the number of
	//segments found
	int const static	s[13]={1-640,1 ,1+640, 640, 640-1, -1, -1-640, -640, 1-640, 1, 1+640, 640, 640-1};
	int const static	n[13]= {1,0,7,6,5,4,3,2,1,0,7,6,5};
	int const static  id[13]={1,1,1,0,-1,-1,-1,0,1,1,1,0,-1},
				jd[13]={-1,0,1,1,1,0,-1,-1,-1,0,1,1,1};

	uInt8 *neigbour;
	uInt8 *img=img_start;

	//set search line!!!
	img+=x_pos;
	//search line
	Path *path=pathlist.create();
	for(j=0;j<480;j++)
	{
			direction=0;

			if(*img==255)
			{
				c_nr=path->nr_seg;
				i_start=x_pos;j_start=j;ii=x_pos;jj=j;
				nr=0;
				neigbour=img;
				do{
					l=0;
					found=0;

					l=7-direction;
					for(p=l;p<l+7;p++)
					{
						if(ii+id[p]<640  && ii+id[p]>=0  && jj+jd[p]<480 && jj+jd[p]>=0)
						{
							if(*(neigbour+s[p]))
							{
								if(nr>MAX_SEG_PIX)
									return -4;
								path->path_seg[c_nr].pixel[nr  ].x=ii;
								path->path_seg[c_nr].pixel[nr++].y=jj;
								*neigbour=1;
								neigbour+=s[p];
								direction=n[p];
								ii+=id[p];jj+=jd[p];
								found=1;
								break;
							}
						}
					}	
				}while(ii!=i_start || jj!=j_start);

				//if the streak is too small, do not take it!
				if(nr > 7)
				{
					path->path_seg[c_nr].p_nr=nr;					
					path_tot_nr++;
					path->nr_seg++;
					//path=pathlist.create;
				}
				//go to end****************************
				while(*++img!=0){i++;}
			} //end if ==255
			else if(*img==1) //border pixel
			{
				while(*++img!=0){i++;}
			}
			img+=640;
	}//j for
	return path_tot_nr;
}




int Path::Get_contour_point(Path_seg *path_seg, uInt8 *img_start, int px, int py)
{
	int i,ii,j,jj,l,p,nr,direction,i_start,j_start,c_nr;
	//this settings get the true  8-neighbour boundary!
	//this code does not search the whole image, instead it does
	//if there it finds something at pk,py and does collects the boundary 
	int const static	s[14]={1-640,1 ,1+640, 640, 640-1, -1, -1-640, -640, 1-640, 1, 1+640, 640, 640-1, -1};
	int const static	n[14]= { 1,0,7,6,5,4,3,2,1,0,7,6,5,4};
	int const static   id[14]= { 1,1,1,0,-1,-1,-1 ,0, 1,1,1,0,-1,-1},
				       jd[14]= {-1,0,1,1, 1, 0,-1,-1,-1,0,1,1, 1, 0};

	uInt8 *neigbour;
	uInt8 *img=img_start;
	c_nr=0;

	//set point
	i=px;
	j=py;
	img+=(i+j*640);

	direction=0;
	if(*img==255)
	{
		//get neighbours in order to find true boarder pixel
		//otherwise the algorithm gets stuck inside a path segment
		neigbour=img;
		int b=0;
		for(p=0;p<8;p++)
		{
			if(*(neigbour+s[p]))
				b++;
		}
		if(b<7)
		{
			i_start=i;j_start=j;ii=i;jj=j;
			nr=0;
			neigbour=img;
			int tot_steps=0;
			do{
				l=7-direction;
				for(p=l;p<l+8;p++)
				{
					if(*(neigbour+s[p]))
					{
						if(nr>MAX_SEG_PIX)
							return -4;
						if((ii+id[p])<0 || (ii+id[p]) >= 640 || (jj+jd[p])<0 || (jj+jd[p]) >= 480)
							p++;
						//while((ii+id[p])<0 || (ii+id[p]) >= 640 || (jj+jd[p])<0 || (jj+jd[p]) >= 480)
						//	p++;
						else
						{
							path_seg->pixel[nr  ].x=ii;
							path_seg->pixel[nr++].y=jj;
							*neigbour=1;
							neigbour+=s[p];
							direction=n[p];
							ii+=id[p];jj+=jd[p];
							break;
						}
					}

				}	
			tot_steps++;
			}while((ii!=i_start || jj!=j_start) && tot_steps < 400);
			if(tot_steps==400)
				return 0;

			path_seg->p_nr=nr;
			return nr;
		}
	} //end if ==255
	return 0;
}

/*
int Path::Get_conture_curvature(Path_seg *path_seg, const int edge)
{
	//this calculates the "curvature" of a particle path
	//using 7 pixel. Less pixel do not do the job, because
	//the sensitivity to the noise. Roughly the number of the pixels
	//in the operator detormine the curvature radius which is 
	// determined.
	static i_mat=1;

	fprintf(fp1,"s%i=[", i_mat);
	fprintf(fp2,"x%i=[", i_mat);
	fprintf(fp3,"y%i=[", i_mat);

	if(path_seg->p_nr >0)
	{
		int *s_beg = new int[path_seg->p_nr];
		int *s=s_beg;

		Pixel p[6];

		p[0]=path_seg->pixel[path_seg->p_nr-3];
		p[1]=path_seg->pixel[path_seg->p_nr-2];
		p[2]=path_seg->pixel[path_seg->p_nr-1];
		p[3]=path_seg->pixel[0];
		p[4]=path_seg->pixel[1];
		p[5]=path_seg->pixel[2];

		int j=0;
		for(int i=0;i<3;i++)
		{
			*s= 3*p[j].x                + 2*p[j+1].x              +p[j+2].x+
				3*path_seg->pixel[i+2].x+ 2*path_seg->pixel[i+1].x+path_seg->pixel[i].x-
			  12*path_seg->pixel[i].x+
				3*p[j].y                + 2*p[j+1].y              +p[j+2].y+
				3*path_seg->pixel[i+2].y+ 2*path_seg->pixel[i+1].y+path_seg->pixel[i].y-
			  12*path_seg->pixel[i].y;

			j++;
			fprintf(fp1,"%i;  ",*s);
			fprintf(fp2,"%i;  ",path_seg->pixel[i].x);	
			fprintf(fp3,"%i;  ",path_seg->pixel[i].y);	
			s++;
		}

		for(i=3;i<path_seg->p_nr-3;i++)
		{
			*s= 3*path_seg->pixel[i-3].x+2*path_seg->pixel[i-2].x+path_seg->pixel[i-1].x+
				3*path_seg->pixel[i+3].x+2*path_seg->pixel[i+2].x+path_seg->pixel[i+1].x-
			  12*path_seg->pixel[i].x+
				3*path_seg->pixel[i-3].y+2*path_seg->pixel[i-2].y+path_seg->pixel[i-1].y+
				3*path_seg->pixel[i+3].y+2*path_seg->pixel[i+2].y+path_seg->pixel[i+1].y-
			  12*path_seg->pixel[i].y;


			fprintf(fp1,"%i;  ",*s);
			fprintf(fp2,"%i;  ",path_seg->pixel[i].x);	
			fprintf(fp3,"%i;  ",path_seg->pixel[i].y);	
			s++;
		}

		j=1;
		for(i=path_seg->p_nr-3;i<path_seg->p_nr;i++)
		{
			*s= 3*path_seg->pixel[i-3].x+2*path_seg->pixel[i-2].x+path_seg->pixel[i-1].x+
				3*p[j+2].x              +2*p[j+1].x              +p[j].x-
			  12*path_seg->pixel[i].x+
				3*path_seg->pixel[i-3].y+2*path_seg->pixel[i-2].y+path_seg->pixel[i-1].y+
				3*p[j+2].y              +2*p[j+1].y              +p[j].y-
			  12*path_seg->pixel[i].y;

			j++;
			fprintf(fp1,"%i;  ",*s);
			fprintf(fp2,"%i;  ",path_seg->pixel[i].x);	
			fprintf(fp3,"%i;  ",path_seg->pixel[i].y);	
			s++;
		}


		//Get maxima
		int endp[5], found=0, val=0,n=0;
		s=s_beg;

		for(i=0;i<path_seg->p_nr;i++)
		{
			while(abs(*s)>edge)
			{
				if(abs(*s)>val)
				{
					endp[n]=i;
					val=abs(*s);
					found=1;
				}
				s++;
				i++;
			}
			if(found)
			{
				path_seg->end_point[n]=path_seg->pixel[endp[n]];
				n++;
				if(n>=MAX_END_P)
					return -5;
				found=0;
				val=0;
			}
			s++;
		}	
		path_seg->end_point[n].x=-1;
		delete[] s_beg;
	}

	i_mat++;
	fprintf(fp1,"];\n");
	fprintf(fp2,"];\n");
	fprintf(fp3,"];\n");
	return 0;
}
*/


double Get_angle(int, int, int, int);

double Get_angle(Pixel p1, Pixel p2, Pixel p3)
{
	double ax=p2.x-p1.x;
	double ay=p2.y-p1.y;
	double bx=p3.x-p2.x;
	double by=p3.y-p2.y;
	double sign=ax*by-ay*bx;
	
	if(sign != 0)
		return (sign)/fabs(sign)*acos((ax*bx+ay*by)/(sqrt(ax*ax+ay*ay)*sqrt(bx*bx+by*by)));
	else
		return 0;

}

int Path::Get_conture_curvature_a(Path_seg *path_seg)
{
	//this calculates the turning angle" of a particle path
	//using 7 pixel. 
	//a=[(x(i)-x(i-1));  (y(i)-y(i-1))];
    //b=[(x(i+1)-x(i));  (y(i+1)-y(i))];    
    //phi=acos((a'*b)/sqrt(a'*a) / sqrt(b'*b));
    //%vorzeichen
    //A=a(1)*b(2)-a(2)*b(1);
    //phi=sign(A)*phi;

	static i_mat=1;

	//fprintf(fp1,"s%i=[", i_mat);
	//fprintf(fp2,"x%i=[", i_mat);
	//fprintf(fp3,"y%i=[", i_mat);

	//Get_property(path_seg);

	if(path_seg->p_nr >7)
	{
		double *s_beg = new double[path_seg->p_nr];
		double *s=s_beg;

		Pixel p[8];
		p[0]=path_seg->pixel[path_seg->p_nr-4];
		p[1]=path_seg->pixel[path_seg->p_nr-3];
		p[2]=path_seg->pixel[path_seg->p_nr-2];
		p[3]=path_seg->pixel[path_seg->p_nr-1];
		p[4]=path_seg->pixel[0];
		p[5]=path_seg->pixel[1];
		p[6]=path_seg->pixel[2];
		p[7]=path_seg->pixel[3];

		int j=4;
		for(int i=0;i<4;i++)
		{	
			*s=Get_angle(              p[j-4],              p[j-3],              p[j-2])+
			   Get_angle(              p[j-3],              p[j-2],              p[j-1])+
			   Get_angle(              p[j-2],              p[j-1],path_seg->pixel[i  ])+
			   Get_angle(              p[j-1],path_seg->pixel[i  ],path_seg->pixel[i+1])+
			   Get_angle(path_seg->pixel[i  ],path_seg->pixel[i+1],path_seg->pixel[i+2])+
			   Get_angle(path_seg->pixel[i+1],path_seg->pixel[i+2],path_seg->pixel[i+3])+
			   Get_angle(path_seg->pixel[i+2],path_seg->pixel[i+3],path_seg->pixel[i+4]);

			j++;
			//fprintf(fp1,"%f;  ",*s);
			//fprintf(fp2,"%i;  ",path_seg->pixel[i].x);	
			//fprintf(fp3,"%i;  ",path_seg->pixel[i].y);	
			s++;
		}

		for(i=4;i<path_seg->p_nr-4;i++)
		{
			*s=Get_angle(path_seg->pixel[i-4],path_seg->pixel[i-3],path_seg->pixel[i-2])+
			   Get_angle(path_seg->pixel[i-3],path_seg->pixel[i-2],path_seg->pixel[i-1])+
			   Get_angle(path_seg->pixel[i-2],path_seg->pixel[i-1],path_seg->pixel[i  ])+
			   Get_angle(path_seg->pixel[i-1],path_seg->pixel[i  ],path_seg->pixel[i+1])+
			   Get_angle(path_seg->pixel[i  ],path_seg->pixel[i+1],path_seg->pixel[i+2])+
			   Get_angle(path_seg->pixel[i+1],path_seg->pixel[i+2],path_seg->pixel[i+3])+
			   Get_angle(path_seg->pixel[i+2],path_seg->pixel[i+3],path_seg->pixel[i+4]);
	
			j++;
			//fprintf(fp1,"%f;  ",*s);
			//fprintf(fp2,"%i;  ",path_seg->pixel[i].x);	
			//fprintf(fp3,"%i;  ",path_seg->pixel[i].y);	
			s++;
		}

		j=0;
		for(i=path_seg->p_nr-4;i<path_seg->p_nr;i++)
		{
			/**s=0; */
			
			*s=Get_angle(path_seg->pixel[i-4],path_seg->pixel[i-3],path_seg->pixel[i-2])+
			   Get_angle(path_seg->pixel[i-3],path_seg->pixel[i-2],path_seg->pixel[i-1])+
			   Get_angle(path_seg->pixel[i-2],path_seg->pixel[i-1],path_seg->pixel[i  ])+
			   Get_angle(path_seg->pixel[i-1],path_seg->pixel[i  ],              p[j+1])+
			   Get_angle(path_seg->pixel[i  ],              p[j+1],              p[j+2])+
			   Get_angle(              p[j+1],              p[j+2],              p[j+3])+
			   Get_angle(              p[j+2],              p[j+3],              p[j+4]);
			  
			j++;
			//fprintf(fp1,"%f;  ",*s);
			//fprintf(fp2,"%i;  ",path_seg->pixel[i].x);	
			//fprintf(fp3,"%i;  ",path_seg->pixel[i].y);	
			s++;
		}

		//****************************************************
		//************  Get the maxima  **********************
		int found=0, n=0;
		double val=0;
		s=s_beg;

		int *candidates =new int[20];
		double i_sum=0;
		double i_cand=0;

		for(i=0;i<path_seg->p_nr;i++)
		{
			if(fabs(*s)>3)
			{
				candidates[(int)i_cand]=i;
				i_sum+=i;
				i_cand++;
			}
			s++;
		}

		if(i_cand !=0)
		{
			double border=i_sum/i_cand;
			int counter=0;

			s=s_beg;
			for(i=0;i<border;i++)
			{ 
				if(fabs(*s)>3)
					counter++;
				s++;
			}

			double ep_index=0;
			for(i=0 ;i<counter;i++)
				ep_index += candidates[i];
			
			path_seg->end_point_index[0]=(int)(ep_index/counter);

			ep_index=0;
			for(i=counter ;i<i_cand;i++)
				ep_index += candidates[i];	
			
			path_seg->end_point_index[1]=(int)(ep_index/(i_cand-counter));


			path_seg->end_point_index[2]=-1;
			delete[] s_beg;
		}
		else
		path_seg->end_point_index[0]=-1;
	}
	else
	{
		path_seg->end_point_index[0]=-1;
	}

	i_mat++;
	//fprintf(fp1,"];\n");
	//fprintf(fp2,"];\n");
	//fprintf(fp3,"];\n");
	return 0;
}


int Path::Get_property(Path_seg *path_seg)
{
	Pixel *p=path_seg->pixel;
	int i;
	const double diag=sqrt(2);

	//Calculate the perimeter 
	double peri=0;
	for(i=1; i<path_seg->p_nr; i++)
	{
		if(p[i].x != p[i-1].x && p[i].y != p[i-1].y)
			peri +=diag;
		else 
			peri+=1;
	}

	//Calculate the area
	double part_area;
	double area=0;
	//and Calculate the centroid 
	CamPoint c;
	c.x=0;
	c.y=0;
	for(i=1; i<path_seg->p_nr; i++)
	{
		part_area= p[i-1].x*p[i].y - p[i].x*p[i-1].y;
		area+=part_area;
		c.x+= (part_area*(p[i-1].x+p[i].x));
		c.y+= (part_area*(p[i-1].y+p[i].y));
	}
	//do not forgett the last element 
	part_area= p[path_seg->p_nr-1].x*p[0].y - p[0].x*p[path_seg->p_nr-1].y;
	area+=part_area;
	c.x+= (part_area*(p[path_seg->p_nr-1].x+p[0].x));
	c.y+= (part_area*(p[path_seg->p_nr-1].y+p[0].y));

	area /=2;

	path_seg->centroid.x = c.x /6/area;
	path_seg->centroid.y = c.y /6/area;
	path_seg->centroid.w = 1;

	//calculate form form parameter
	//image processing for science applications  p506 
	//circularity
	//	cir=p*p/A  min = 4*pi=12.57  (circle) square =16  12*wurzel 3=20.8
	// is equivalent to the form factor in The image processing handbook by john c. russ
	// p554-555
	double cir=peri*peri/area;

	if(cir<18)
	{
		path_seg->circular=TRUE;
		return 1;
	}
	return 0;
}



int Path::Get_hproperty()
{
	CamPoint *p;
	int i;

	Path_seg *seg;
	for(seg=path_seg; seg< path_seg+nr_seg;seg++)
	{
		p=seg->hpixel;

		//Calculate the perimeter 
		double peri=0;
		for(i=1; i<seg->p_nr; i++)
		{
			peri+=sqrt( (p[i-1].x-p[i].x)*(p[i-1].x-p[i].x) + (p[i-1].y-p[i].y)*(p[i-1].y-p[i].y));
		}
		//do not forget the last element 
		peri+=sqrt( (p[seg->p_nr-1].x-p[0].x)*(p[seg->p_nr-1].x-p[0].x) 
				  + (p[seg->p_nr-1].y-p[0].y)*(p[seg->p_nr-1].y-p[0].y));

		//Calculate the area
		double part_area;
		double area=0;
		//and Calculate the centroid 
		CamPoint c;
		c.x=0;
		c.y=0;
		for(i=1; i<seg->p_nr; i++)
		{
			part_area= p[i-1].x*p[i].y - p[i].x*p[i-1].y;
			area+=part_area;
			c.x+= (part_area*(p[i-1].x+p[i].x));
			c.y+= (part_area*(p[i-1].y+p[i].y));
		}
		//do not forget the last element 
		part_area= p[seg->p_nr-1].x*p[0].y - p[0].x*p[seg->p_nr-1].y;
		area+=part_area;
		c.x+= (part_area*(p[seg->p_nr-1].x+p[0].x));
		c.y+= (part_area*(p[seg->p_nr-1].y+p[0].y));

		area /=2;

		path_seg->centroid.x = c.x /6/area;
		path_seg->centroid.y = c.y /6/area;
		path_seg->centroid.w = 1;

		//calculate form form parameter
		//image processing for science applications  p506 
		//circularity
		//	cir=p*p/A  min = 4*pi=12.57  (circle) square =16  12*wurzel 3=20.8
		// is equivalent to the form factor 4 pi * A/ Perimeter^2 in The image processing 
		//handbook by john c. russ p554-555
		double cir=peri*peri/area;

		if(cir<35)
			seg->circular=TRUE;
	}
	return 0;
}