#include "stdafx.h"
#include "niimaq.h"
#include "math.h"
#include "parameters.h"
#include "structures.h"
#include "io_functions.h"
#include <stdio.h>

extern CCD_par ccd_init;

void histo(uInt8 *image, double *hist)
{
	int i,j,v;
	int tot=0;
	int max=0;

	for(j=0;j<=255;j++)
				*(hist+j)=0;

	for(i=0; i< (ccd_init.height*ccd_init.width) ;i++)
	{
		for(j=0;j<=128;j++)  
		{
			v=2*j;
			if(*image == v || *image == v+1)
			{
				*(hist+j)+=1;
				if(*(hist+j)>max) max=(int)*(hist+j);
				//if(*(hist+j)>max) max=log10(*(hist+j)+1);
				tot++;
				break;
			}
		}
		image++;
	}

	for(j=0;j<=255;j++)
	{
		if(max != 0)
			*(hist+j)=50*log10(*(hist+j)*100.0/(double)max+1);
			//*(hist+j)=log10(*(hist+j)+1)*100.0/(double)max;
			
	}
}


void Image_row_cor(uInt8 *image)
{
	//this routine corrects for the different 
	//gain factors of the two readout lines
	//by imposing the condition that both half images 
	//must have the same averaged intensity

	int i,j;
	int val_e,val_o,p;
	uInt8 *img=image;
	double av_e,av_o;

	//even rows
	val_e=0;
	for(j=0;j<ccd_init.height;j+=2)
	{
		for(i=0;i<ccd_init.width;i++)
		{
			val_e +=*img++;
		}
	}

	//odd rows
	val_o=0;
	img=image;
	for(j=1;j<ccd_init.height;j+=2)
	{
		for(i=0;i<ccd_init.width;i++)
		{
			val_o +=*img++;
		}
	}

	//averages intesities of the half images
	av_e=(double)val_e / (double)(ccd_init.height/2*ccd_init.width);
	av_o=(double)val_o / (double)(ccd_init.height/2*ccd_init.width);

	//correction of the even rows
	img=image;
	for(j=0;j<ccd_init.height;j+=2)
	{
		for(i=0;i<ccd_init.width;i++)
		{
			p = (int)((double)img[j*ccd_init.width+i]*av_o/av_e);
			img[j*ccd_init.width+i] = (p>255) ? 255 : p;
		}		
	}
}


void Background_leveling(uInt8 *image)
{
	int i,val,j,ip,jp,p;
	uInt8 *img=image;
	//double p_av[64];

	//initialize the average variables
	val=0;

	//calculate the average intensity of the image
	for(i=0;i<ccd_init.height*ccd_init.width;i++)
		val+=img[i];

	double av=(double)val/(double)(ccd_init.height*ccd_init.width);

	//cell size and number
	//int i_cell=8,j_cell=8;
	//int i_size=80,j_size=60;

	int i_cell=32,j_cell=16;
	int i_size=20,j_size=30;
	double *p_av = new double[i_cell*j_cell];

	for(i=0;i<i_cell*j_cell;i++)
		p_av[i]=0;

	//calculate the average intensity of subregions of the image
	for(j=0;j<j_cell;j++)
	{
		for(i=0;i<i_cell;i++)
		{
			for(jp=0;jp<j_size;jp++)
			{
				for(ip=0;ip<i_size;ip++)
				{
						p_av[i+i_cell*j] += img[j*i_size*j_size*i_cell +i*i_size +jp*640 +ip];  
				}
			}
			p_av[i+i_cell*j] = p_av[i+i_cell*j]/(double)(i_size*j_size);  
		}
	}

	//equalize the image
	for(j=0;j<j_cell;j++)
	{
		for(i=0;i<i_cell;i++)
		{
			for(jp=0;jp<j_size;jp++)
			{
				for(ip=0;ip<i_size;ip++)
				{
					 p =(int)((double)img[j*i_size*j_size*i_cell +i*i_size +jp*640 +ip]  *  av/p_av[i+i_cell*j]);  
					 img[j*i_size*j_size*i_cell +i*i_size +jp*640 +ip] = (p > 255) ? 255 : p;
				}
			}
		}
	}
}



void Background_leveling_min(uInt8 *image)
{
	int i,j,ip,jp;
	uInt8 *img=image;

	int i_cell=32,j_cell=16;	//this is the number of the cells
	int i_size=20,j_size=30;    //this is the size of the cells in pixel

	int *min_val = new int[i_cell*j_cell];


	for(i=0;i<i_cell*j_cell;i++)
		min_val[i]=255;

	//find the minimum in each cell
	for(j=0;j<j_cell;j++)
	{
		for(i=0;i<i_cell;i++)
		{
			for(jp=0;jp<j_size;jp++)
			{
				for(ip=0;ip<i_size;ip++)
				{
					if(img[j*i_size*j_size*i_cell +i*i_size +jp*640 +ip] < min_val[i+i_cell*j])
						min_val[i+i_cell*j]=img[j*i_size*j_size*i_cell +i*i_size +jp*640 +ip];
				}
			}
		}
	}


	//adjust the pixels
	for(j=0;j<j_cell;j++)
	{
		for(i=0;i<i_cell;i++)
		{
			for(jp=0;jp<j_size;jp++)
			{
				for(ip=0;ip<i_size;ip++)
				{
					img[j*i_size*j_size*i_cell +i*i_size +jp*640 +ip] -= min_val[i+i_cell*j];
					
				}
			}
		}
	}
}



void Box_filter(uInt8 *image_in, uInt8 *image_out)
{

	uInt8 *image=image_in;
	int i,j,i1,j1,i2,j2,k,l;
	double val,nr;


	//boxsize
	int box=4;

	for(j=0;j<ccd_init.height;j++)
	{
		for(i=0;i<ccd_init.width;i++)
		{
			nr=0;
			val=0;
            i1= i-box; if (i1 < 0) i1= 0; 
			i2= i+box; if (i2 > ccd_init.width-box) i2= ccd_init.width-box;
			j1= j-box; if (j1 < 0) j1= 0; 
			j2= j+box; if (j2 > ccd_init.height-box) j2= ccd_init.height-box;
			for (l=j1;l<=j2;l++) 
			{
				for (k=i1;k<=i2;k++) 
				{
					val+=image[k+640*l];
					nr++;
				}
			}
			image_out[i+640*j]=(int)(val/nr);
		}
	}	
}


void Gauss_filter(uInt8 *image_in, uInt8 *image_out)
{
	uInt8 *image=image_in;
	int i,j,i1,j1,i2,j2,k,l,ik,jk;
	double val,sum;

	//Type 3x3, Scale=16
	const int kernel_3[9]={ 1,   2,   1,                   
							2,   4,   2,
							1,   2,   1};

	//Type 5x5,   Scale=256 
	const int kernel_5[25]={  1,	 4,   6,   4,   1,             
							  4,	16,  24,  16,   4,
							  6,    24,  36,  24,   6,
							  4,    16,  24,  16,   4,
							  1,     4,   6,   4,   1};
	//Type 7x7,  Scale=2048 
	const int kernel_7[49]={  0,   5,  13,  16,  13,   5,   0,    
			                  5,  25,  52,  64,  52,  25,   5,
			                 13,  52, 102, 126, 102,  52,  13,
			                 16,  64, 126, 156, 126,  64,  16,
			                 13,  52, 102, 126, 102,  52,  13,
			                  5,  25,  52,  64,  52,  25,   5,
							  0,   5,  13,  16,  13,   5,   0};
	//boxsize
	int box=3;

	for(int step=0;step<10;step++)
	{
		for(j=0;j<ccd_init.height;j++)
		{
			for(i=0;i<ccd_init.width;i++)
			{
				sum=0;
				val=0;
				i1= i-box; if (i1 < 0) i1= 0; 
				i2= i+box; if (i2 > ccd_init.width-box) i2= ccd_init.width-box;
				j1= j-box; if (j1 < 0) j1= 0; 
				j2= j+box; if (j2 > ccd_init.height-box) j2= ccd_init.height-box;
				jk=0;
				for (l=j1;l<=j2;l++) 
				{
					ik=0;
					for (k=i1;k<=i2;k++) 
					{
						val+= kernel_7[ik+5*jk]*image[k+640*l];
						sum+= kernel_7[ik+5*jk];
						ik++;
					}
					jk++;
				}
				image_out[i+640*j]=(int)(val/sum);
			}
		}
	image=image_out;
	}
}


void Hist_equalization(uInt8 *image)
{
	int i,j,n,t;
	uInt8 *ptr=image;


	t=ccd_init.height * ccd_init.width;
	//look-up table
	char *equ_table = new char[256];


	for(i=0; i <= 255; i++)
	{
		n=0;
		for(j=0; j < t; j++)
		{
			if(ptr[j] <=i)
				n++;
		}
		equ_table[i]=(int)(255*(double)n/(double)(t));
	}


	for(i=0; i < t; i++)
		ptr[i] = equ_table[ptr[i]];

	delete[] equ_table;
}



void Image_subtract(uInt8 *image1, uInt8 *image2)
{
	int i;
	uInt8 *ptr1=image1;
	uInt8 *ptr2=image2;

	for(i=0; i < ccd_init.height * ccd_init.width; i++)
		*ptr1++ = abs(*ptr1 - *ptr2++);
}



int Extract_streaks_roes (CamPoint centr[], uInt8 *binimg, int thres, int min_pixel, int max_pixel)
/*
   binimg - binary image to be searched (values: 0, 255)
   xct, yct - arrays of length <nct> with computed centroids
   nct - number of centroids, returned as function value (-1 = error)
*/
   //the feature (blob) has to be larger than a certain value
   //and has to be smaller that an other value
   //blob touching the border are not concidered
{

   int nct;
   int found;
   int i,j,k,l, ij, kl, ptr, onedge;
   int xsum, ysum, nsum, pixel;
   int i1,i2,j1,j2;

   int xsize=ccd_init.width;
   int ysize=ccd_init.height;
   
   int * xstack;
   int * ystack;

   xstack= (int *) calloc (10000,sizeof(int));
   ystack= (int *) calloc (10000,sizeof(int));

   nct= 0;
   for (j=0;j<ysize;j++) {
      for (i=0;i<xsize;i++) {
         ij= j*xsize + i; 
         if (binimg[ij] > thres) {

            /* point found; start centroid accumulation */

            onedge= 0;
            if ((i==0) || (i==xsize-1)) onedge= 1;
            if ((j==0) || (j==ysize-1)) onedge= 1;

			pixel=1;   //the number of pixel contained in the blob
			xsum= i*binimg[ij]; ysum= j*binimg[ij]; nsum = binimg[ij];
            ptr= 0; xstack[ptr]= i; ystack[ptr]= j;
            binimg[ij]= 0;

            /* exhaustive stack search */
            while (ptr >= 0) {
               do {
                  found= 0;
                  i1= xstack[ptr]-1; if (i1 < 0) i1= 0; 
                  i2= xstack[ptr]+1; if (i2 > xsize-1) i2= xsize-1;
                  j1= ystack[ptr]-1; if (j1 < 0) j1= 0; 
                  j2= ystack[ptr]+1; if (j2 > ysize-1) j2= ysize-1;
                  for (l=j1;l<=j2;l++) {
                     for (k=i1;k<=i2;k++) {
                        kl= l*xsize + k;
                        if (binimg[kl] > thres) {
                           if ((k==0) || (k==xsize-1)) onedge= 1;
                           if ((l==0) || (l==ysize-1)) onedge= 1;
                           if (ptr == 10000) {               
                              return (-1);
                           }
                           found= 1;
                           pixel++;
                           xsum += k*binimg[kl]; ysum += l*binimg[kl]; nsum += binimg[kl];
                           ptr++; xstack[ptr]= k; ystack[ptr]= l;
                           binimg[kl]= 0;
                        }
                     }
                  }
               } while (found != 0);
               ptr --;
            }

            if (onedge == 0 && pixel > min_pixel  && pixel <max_pixel) {
			   centr[nct].x = xsum / (float) nsum; 
			   centr[nct].y = ysum / (float) nsum; 
               nct++;
            }
         }
      }         
   }
   free (xstack); free (ystack);
   return (nct);
}


int Extract_streaks_roes3D (CalPoint calpoint[], CamPoint centr[], uInt8 *binimg, int thres)
/*
   binimg - binary image to be searched (values: 0, 255)
   xct, yct - arrays of length <nct> with computed centroids
   nct - number of centroids, returned as function value (-1 = error)
*/
{

   int nct,n_p;
   int found;
   int i,j,k,l, ij, kl, ptr, onedge;
   int xsum, ysum, nsum;
   int i1,i2,j1,j2;

   int xsize=ccd_init.width;
   int ysize=ccd_init.height;
   
   int * xstack;
   int * ystack;

   xstack= (int *) calloc (10000,sizeof(int));
   ystack= (int *) calloc (10000,sizeof(int));

   nct= 0;
   for (j=0;j<ysize;j++) {
      for (i=0;i<xsize;i++) {
         ij= j*xsize + i; 
         if (binimg[ij] > thres) {

            /* point found; start centroid accumulation */
			n_p=0;
			calpoint[nct].x[n_p]=i;
			calpoint[nct].y[n_p]=j;

            onedge= 0;
            if ((i==0) || (i==xsize-1)) onedge= 1;
            if ((j==0) || (j==ysize-1)) onedge= 1;

			xsum= i*binimg[ij]; ysum= j*binimg[ij]; nsum = binimg[ij];
            ptr= 0; xstack[ptr]= i; ystack[ptr]= j;
            binimg[ij]= 0;

            /* exhaustive stack search */
            while (ptr >= 0) {
               do {
                  found= 0;
                  i1= xstack[ptr]-1; if (i1 < 0) i1= 0; 
                  i2= xstack[ptr]+1; if (i2 > xsize-1) i2= xsize-1;
                  j1= ystack[ptr]-1; if (j1 < 0) j1= 0; 
                  j2= ystack[ptr]+1; if (j2 > ysize-1) j2= ysize-1;
                  for (l=j1;l<=j2;l++) {
                     for (k=i1;k<=i2;k++) {
                        kl= l*xsize + k;
                        if (binimg[kl] > thres) {
                           if ((k==0) || (k==xsize-1)) onedge= 1;
                           if ((l==0) || (l==ysize-1)) onedge= 1;
                           if (ptr == 10000) {
                              return (-1);
                           }
                           found= 1;
						   n_p++;
						   calpoint[nct].x[n_p]=k;
						   calpoint[nct].y[n_p]=l;
                           xsum += k*binimg[kl]; ysum += l*binimg[kl]; nsum += binimg[kl];
                           ptr++; xstack[ptr]= k; ystack[ptr]= l;
                           binimg[kl]= 0;
                        }
                     }
                  }
               } while (found != 0);
               ptr --;
            }

            if (onedge == 0) {
			   centr[nct].x = xsum / (float) nsum; 
			   centr[nct].y = ysum / (float) nsum;
			   calpoint[nct].n_p=n_p;
			   nct++;
			}
         }
      }         
   }
   free (xstack); free (ystack);
   return (nct);
}
