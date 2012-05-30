#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include "niimaq.h"
#include "image.h"


int Image::Read(char *file_name, uInt8 * mem_location)
{
   FILE *file = NULL;
   file= (FILE *) fopen (file_name,"r");

   if(file == NULL)         //File non existing
		return 1;
	else if(ferror(file))  //Error
		return 1;
	else 
	{
		fread(mem_location, sizeof(char), 640*480, file);
		fclose(file);
		return 0;    //No Error
	}
}


int Image::Write(char *file_name, uInt8 *mem_location)
{
	FILE  *file = fopen (file_name,"wb");
	size_t written;

   if(file == NULL)         //File non existing
		return 1;
	else if(ferror(file))  //Error
		return 1;
	else 
	{
		written=fwrite(mem_location, sizeof(uInt8), 640*480, file);
		fclose(file);
		return 0;    //No Error
	}
}



void Image::Process(CDC *device, uInt8 *image_addr, uInt8 *ref_image_addr, int place, int thres_level)
{
	uInt8 *image      = image_addr;
	uInt8 *image_end  = &image[640*480];
	uInt8 *ref_image  = ref_image_addr;
	//*********Subtract and thresh***********************************************************
	while (image<image_end)
		*image++= (abs(*image - *ref_image++) >= thres_level) ? 255 : 0;
	/*{
		int a = *image * *image;
		int b = *ref_image * *ref_image;
		int c = (int)sqrt(abs(a-b));
		*image++= c >= thres_level ? 255 : 0;
		*ref_image++;
	}
	*/
	//*********End Subtract and thresh*******************************************************

	//*********Median Filter*****************************************************************
	binary_median_sym(image_addr);
	//*********Median Filter*****************************************************************
}




void Image::binary_median_asym(uInt8 *img)
{
	register int i,j,b,l;
	int static  n_p[8]={1,1-640,-640,-1-640,-1,640-1,640,640+1};

	//Asymetric median filter (correction just for salt-noise)
	//This filter checks if the pixel is black; if so go to next
	//if not check neighbours (size) and decide the pixels value

	img+=480;
	for(j=1; j < 480-1; j++)
	{
		img++;//first pixel in row
		for (i=1; i < 640-1; i++)
		{
			if(*(img))             //in case the pixel is white
			{
				b=0;
				for(l=0; l<8; l++)
				{
					if(*(img+*(n_p+l)) == 0)
							b++;
				}
				if(b > 5)                  //if more than half of the pixel are black
					*img = 0;             //than make pixel black
			}
			img++;//last pixel in row
		}
		img++;
	}
}


void Image::binary_median_sym(uInt8 *img)
{
	register int i,j,b,l;
	int static  n_p[8]={1,1-640,-640,-1-640,-1,640-1,640,640+1};

	//Median filter. Filters salt and pepper noise
	//This filter checks if the pixel is black; if so go to next
	//if not check neighbours (size) and decide the pixels value

	img+=480;
	for(j=1; j < 480-1; j++)
	{
		img++;//first pixel in row
		for (i=1; i < 640-1; i++)
		{
			b=0;
			for(l=0; l<8; l++)
			{
				if(*(img+n_p[l]) == 0)
							b++;
			}
			if(b > 4)					//if more than half of the pixel are black
					*img = 0;			//than make pixel black
			else if (b < 4)
					*img = 255;			//than make pixel white
			
			img++;						//last pixel in row
		}
		img++;
	}
}


void Image::binary_median(uInt8 *img_in, uInt8 *img_out)
{
	register int i,j,b,l;
	int static  n_p[8]={1,1-640,-640,-1-640,-1,640-1,640,640+1};

	//Median filter. Filters salt and pepper noise
	//This filter checks if the pixel is black; if so go to next
	//if not check neighbours (size) and decide the pixels value

	img_in+=480;
	img_out+=480;
	for(j=1; j < 480-1; j++)
	{
		img_in++;//first pixel in row
		img_out++;
		for (i=1; i < 640-1; i++)
		{
			b=0;
			for(l=0; l<8; l++)
			{
				if(*(img_in+n_p[l]) == 0)
					b++;
			}
			if(b > 5)					//if more than half of the pixel are black
					*img_out = 0;			//than make pixel black
			//else if (b < 4)
			//		*img_out = 255;			//than make pixel white
			else 
				*img_out=*img_in;
			
			img_in++;						//last pixel in row
			img_out++;
		}
		img_in++;
		img_out++;
	}
}


void Image::row_correction(uInt8 *image)
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
	for(j=0;j<480;j+=2)
	{
		for(i=0;i<640;i++)
		{
			val_e +=*img++;
		}
	}

	//odd rows
	val_o=0;
	img=image;
	for(j=1;j<480;j+=2)
	{
		for(i=0;i<640;i++)
		{
			val_o +=*img++;
		}
	}

	//averages intesities of the half images
	av_e=(double)val_e / (double)(480/2*640);
	av_o=(double)val_o / (double)(480/2*640);

	//correction of the even rows
	img=image;
	for(j=0;j<480;j+=2)
	{
		for(i=0;i<640;i++)
		{
			p = (int)((double)img[j*640+i]*av_o/av_e);
			img[j*640+i] = (p>255) ? 255 : p;
		}		
	}
}