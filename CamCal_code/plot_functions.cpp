#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include "niimaq.h"
#include "parameters.h"
#include "structures.h"
#include "plot_functions.h"

extern Intrinsic_par    in2;
extern CCD_par	 ccd1,ccd2;

void Plot_image(CDC *device, uInt8 *ImgAddr, int h, int w, int pos, int overlay)
{	
	if(pos==1)
	{
		int err=imgPlotDC ((GUIHNDL)device->m_hDC, (void *)ImgAddr, 0, 0,
				        w, h,
			            CANVASLEFT1, CANVASTOP1, FALSE);
	}
	if(pos==2)
	{
		int err=imgPlotDC ((GUIHNDL)device->m_hDC, (void *)ImgAddr, 0, 0,
				        w, h,
			            CANVASLEFT2, CANVASTOP2, FALSE);
	}

	if(pos==3)
	{
		int err=imgPlotDC ((GUIHNDL)device->m_hDC, (void *)ImgAddr, 0, 0,
				        w, h,
			            CANVASLEFT3, CANVASTOP3, FALSE);
	}
}




void Plot_pixels(CDC *device, uInt8 colimagedata[], CamPoint point[], int h, int w, 
				                 int nr, int r, int g, int b, int place)
{
	int i;

	for(i=1; i <= nr;i++){
		if((int)point[i].x == -1 || (int)point[i].x == -1 ||
		   (int)point[i].y == -1 || (int)point[i].y == -1   )
		{
			//do not plot this point
		}
		else 
		{
			colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))]  = (uInt8)r;
			colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))-1]= (uInt8)g;
			colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))-2]= (uInt8)b;
		}
	}

	if(place==1)
		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)colimagedata, 0, 0,
				     w, h,
			         CANVASLEFT1, CANVASTOP1,  IMGPLOT_COLOR_RGB24);
	else 
		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)colimagedata, 0, 0,
				     w, h,
			         CANVASLEFT2, CANVASTOP2,  IMGPLOT_COLOR_RGB24);
}


void Plot_histo(CDC *device, double* point, int nr, int r, int g, int b)
{
	int origin,i,j;
	uInt8 imagedata[3*256*110];
	uInt8 *image=imagedata;

	for(i=0;i<3*256*110;i++)
	{
		*image=255;
		image++;
	}

	image=imagedata;
	for(j=0;j<100;j++)
	{
		image=imagedata+(j*3*256);
		for(i=0;i<(3*256);i+=60)
		{
			*image=0;
			*(image+1)=0;
			*(image+2)=0;
			image+=60;
		}
	}

	image=imagedata+3*100*256;
	for(i=0;i<(3*256);i++)
	{
		*image=0;
		image++;
	}

	image=imagedata;
	origin =3*99*256;
	for(i=0;i<128;i++)
	{
		*(image+origin-3*((int)(*point)*256-2*i  )+1)=0;
		*(image+origin-3*((int)(*point)*256-2*i+1)+1)=0;
		point++;     
	}

    //Print Numbers  0
	image=imagedata+3*103*256+7;
	*image=0; image+=3;*image=0;image+=3;*image=0;image+=3;*image=0;
	image=imagedata+3*104*256+7; *image=0; image+=9; *image=0;
	image=imagedata+3*105*256+7; *image=0; image+=9; *image=0;
	image=imagedata+3*106*256+7; *image=0; image+=9; *image=0;
	image=imagedata+3*107*256+7; *image=0; image+=9; *image=0;
	image=imagedata+3*108*256+7; 
	*image=0; image+=3;*image=0;image+=3;*image=0;image+=3;*image=0;


    //Print Numbers  100
	image=imagedata+3*103*256+292;
	*image=0; 
	image=imagedata+3*104*256+292; *image=0; 
	image=imagedata+3*105*256+292; *image=0;
	image=imagedata+3*106*256+292; *image=0;
	image=imagedata+3*107*256+292; *image=0; 
	image=imagedata+3*108*256+292; *image=0;


	image=imagedata+3*103*256+298;
	*image=0; image+=3;*image=0;image+=3;*image=0;image+=3;*image=0;
	image=imagedata+3*104*256+298; *image=0; image+=9; *image=0;
	image=imagedata+3*105*256+298; *image=0; image+=9; *image=0;
	image=imagedata+3*106*256+298; *image=0; image+=9; *image=0;
	image=imagedata+3*107*256+298; *image=0; image+=9; *image=0;
	image=imagedata+3*108*256+298; 
	*image=0; image+=3;*image=0;image+=3;*image=0;image+=3;*image=0;

	image=imagedata+3*103*256+316;
	*image=0; image+=3;*image=0;image+=3;*image=0;image+=3;*image=0;
	image=imagedata+3*104*256+316; *image=0; image+=9; *image=0;
	image=imagedata+3*105*256+316; *image=0; image+=9; *image=0;
	image=imagedata+3*106*256+316; *image=0; image+=9; *image=0;
	image=imagedata+3*107*256+316; *image=0; image+=9; *image=0;
	image=imagedata+3*108*256+316; 
	*image=0; image+=3;*image=0;image+=3;*image=0;image+=3;*image=0;

    //Print Numbers  200
	image=imagedata+3*103*256+580;
 	*image=0; image+=3;*image=0;image+=3;*image=0;image+=3;*image=0;
	image=imagedata+3*104*256+589; *image=0; 
	image=imagedata+3*105*256+586; *image=0;
	image=imagedata+3*106*256+583; *image=0;
	image=imagedata+3*107*256+580; *image=0; 
	image=imagedata+3*108*256+580; //*image=0;
	*image=0; image+=3;*image=0;image+=3;*image=0;image+=3;*image=0;

	image=imagedata+3*103*256+598;
	*image=0; image+=3;*image=0;image+=3;*image=0;image+=3;*image=0;
	image=imagedata+3*104*256+598; *image=0; image+=9; *image=0;
	image=imagedata+3*105*256+598; *image=0; image+=9; *image=0;
	image=imagedata+3*106*256+598; *image=0; image+=9; *image=0;
	image=imagedata+3*107*256+598; *image=0; image+=9; *image=0;
	image=imagedata+3*108*256+598; 
	*image=0; image+=3;*image=0;image+=3;*image=0;image+=3;*image=0;

	image=imagedata+3*103*256+616;
	*image=0; image+=3;*image=0;image+=3;*image=0;image+=3;*image=0;
	image=imagedata+3*104*256+616; *image=0; image+=9; *image=0;
	image=imagedata+3*105*256+616; *image=0; image+=9; *image=0;
	image=imagedata+3*106*256+616; *image=0; image+=9; *image=0;
	image=imagedata+3*107*256+616; *image=0; image+=9; *image=0;
	image=imagedata+3*108*256+616; 
	*image=0; image+=3;*image=0;image+=3;*image=0;image+=3;*image=0;


	imgPlotDC ((GUIHNDL)device->m_hDC, (void *)imagedata, 0, 0,
				    256, 110,
			        395, 565,  IMGPLOT_COLOR_RGB24);
}




void Plot_points(CDC *device, uInt8 colimagedata[], CamPoint point[], int h, int w, int nr, int r, int g, int b, int place){

	int i;

	for(i=0; i < nr;i++){
		if((int)point[i].x == -1 || (int)point[i].x == -1 ||
		   (int)point[i].y == -1 || (int)point[i].y == -1   )
		{
			//do not plot this point
		}
		else if((int)point[i].x <= 1 || (int)point[i].x >= (w-2) ||
		   (int)point[i].y <= 1 || (int)point[i].y >= (h-2)   )
		{
			//colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))]  = (uInt8)b;
			//colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))-1]= (uInt8)r;
			//colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))-2]= (uInt8)g;
		}
		else 
		{
			colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))]  = (uInt8)b;
			colimagedata[3*((int)point[i].x +1 +(w*(int)point[i].y    ))]  = (uInt8)b;
			colimagedata[3*((int)point[i].x -1 +(w*(int)point[i].y    ))]  = (uInt8)b;
			colimagedata[3*((int)point[i].x    +(w*((int)point[i].y+1)))]  = (uInt8)b;
			colimagedata[3*((int)point[i].x    +(w*((int)point[i].y-1)))]  = (uInt8)b;

			colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))-1]= (uInt8)r;
			colimagedata[3*((int)point[i].x +1 +(w*(int)point[i].y    ))-1]= (uInt8)r;
			colimagedata[3*((int)point[i].x -1 +(w*(int)point[i].y    ))-1]= (uInt8)r;
			colimagedata[3*((int)point[i].x    +(w*((int)point[i].y+1)))-1]= (uInt8)r;
			colimagedata[3*((int)point[i].x    +(w*((int)point[i].y-1)))-1]= (uInt8)r;

			colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))-2]= (uInt8)g;
			colimagedata[3*((int)point[i].x +1 +(w*(int)point[i].y    ))-2]= (uInt8)g;
			colimagedata[3*((int)point[i].x -1 +(w*(int)point[i].y    ))-2]= (uInt8)g;
			colimagedata[3*((int)point[i].x    +(w*((int)point[i].y+1)))-2]= (uInt8)g;
			colimagedata[3*((int)point[i].x    +(w*((int)point[i].y-1)))-2]= (uInt8)g;
		}
	}

	if(place==1){
		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)colimagedata, 0, 0,
			w, h,  CANVASLEFT1, CANVASTOP1,  IMGPLOT_COLOR_RGB24);
	}
	else if(place==2){
		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)colimagedata, 0, 0,
			w, h,  CANVASLEFT2, CANVASTOP2,  IMGPLOT_COLOR_RGB24);
	}
	else if(place==3){
		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)colimagedata, 0, 0,
			w, h,  CANVASLEFT3, CANVASTOP3,  IMGPLOT_COLOR_RGB24);
	}

}


void Plot_points3D(CDC *device, uInt8 colimagedata[], CamPoint point[], int h, int w, int nr, int r, int g, int b, int place){

	int i;

	for(i=0; i <= nr;i++){
		if((int)point[i].x == -1 || (int)point[i].x == -1 ||
		   (int)point[i].y == -1 || (int)point[i].y == -1   )
		{
			//do not plot this point
		}
		else if((int)point[i].x <= 1 || (int)point[i].x >=638 ||
		   (int)point[i].y <= 1 || (int)point[i].y >=478   )
		{
			colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))]  = (uInt8)b;
			colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))-1]= (uInt8)r;
			colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))-2]= (uInt8)g;
		}
		else 
		{
			colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))]  = (uInt8)b;
			colimagedata[3*((int)point[i].x +1 +(w*(int)point[i].y    ))]  = (uInt8)b;
			colimagedata[3*((int)point[i].x -1 +(w*(int)point[i].y    ))]  = (uInt8)b;
			colimagedata[3*((int)point[i].x    +(w*((int)point[i].y+1)))]  = (uInt8)b;
			colimagedata[3*((int)point[i].x    +(w*((int)point[i].y-1)))]  = (uInt8)b;

			colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))-1]= (uInt8)r;
			colimagedata[3*((int)point[i].x +1 +(w*(int)point[i].y    ))-1]= (uInt8)r;
			colimagedata[3*((int)point[i].x -1 +(w*(int)point[i].y    ))-1]= (uInt8)r;
			colimagedata[3*((int)point[i].x    +(w*((int)point[i].y+1)))-1]= (uInt8)r;
			colimagedata[3*((int)point[i].x    +(w*((int)point[i].y-1)))-1]= (uInt8)r;

			colimagedata[3*((int)point[i].x    +(w*(int)point[i].y    ))-2]= (uInt8)g;
			colimagedata[3*((int)point[i].x +1 +(w*(int)point[i].y    ))-2]= (uInt8)g;
			colimagedata[3*((int)point[i].x -1 +(w*(int)point[i].y    ))-2]= (uInt8)g;
			colimagedata[3*((int)point[i].x    +(w*((int)point[i].y+1)))-2]= (uInt8)g;
			colimagedata[3*((int)point[i].x    +(w*((int)point[i].y-1)))-2]= (uInt8)g;
		}
	}

	if(place==1){
		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)colimagedata, 0, 0,
			w, h,  CANVASLEFT1, CANVASTOP1,  IMGPLOT_COLOR_RGB24);
	}
	else if(place==2){
		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)colimagedata, 0, 0,
			w, h,  CANVASLEFT2, CANVASTOP2,  IMGPLOT_COLOR_RGB24);
	}
	else if(place==3){
		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)colimagedata, 0, 0,
			w, h,  CANVASLEFT3, CANVASTOP3,  IMGPLOT_COLOR_RGB24);
	}

}



void Plot_error_cross(CDC *device, uInt8 colimagedata[], int h, int w, int x, int y)
{

		colimagedata[3*(x +		w*(y  ))]  = 255;

		//right-lower arm
		if(x < w-4 && y < h-3)
		{
			colimagedata[3*(x +	1 +	w*(y+1))-1]  = 255;
			colimagedata[3*(x +	2 +	w*(y+2))-1]  = 255;
			colimagedata[3*(x +	3 +	w*(y+3))-1]  = 255;
	//		colimagedata[3*(x +	4 +	w*(y+4))-1]  = 255;
			colimagedata[3*(x +	2 +	w*(y+1))-1]  = 255;
			colimagedata[3*(x +	3 +	w*(y+2))-1]  = 255;
			colimagedata[3*(x +	4 +	w*(y+3))-1]  = 255;
	//		colimagedata[3*(x +	5 +	w*(y+4))-1]  = 255;
		}
		//right-upper arm
		if(x < w-2 && y > 3)
		{
			colimagedata[3*(x +	1 +	w*(y-1))-1]  = 255;
			colimagedata[3*(x +	2 +	w*(y-2))-1]  = 255;
			colimagedata[3*(x +	3 +	w*(y-3))-1]  = 255;
	//		colimagedata[3*(x +	4 +	w*(y-4))-1]  = 255;
			colimagedata[3*(x +	2 +	w*(y-1))-1]  = 255;
			colimagedata[3*(x +	3 +	w*(y-2))-1]  = 255;
			colimagedata[3*(x +	4 +	w*(y-3))-1]  = 255;
	//		colimagedata[3*(x +	5 +	w*(y-4))-1]  = 255;
		}
		//left-lower arm
		if(x > 4 && y < h-3)
		{
			colimagedata[3*(x -	1 +	w*(y+1))-1]  = 255;
			colimagedata[3*(x -	2 +	w*(y+2))-1]  = 255;
			colimagedata[3*(x -	3 +	w*(y+3))-1]  = 255;
	//		colimagedata[3*(x -	4 +	w*(y+4))-1]  = 255;
			colimagedata[3*(x -	2 +	w*(y+1))-1]  = 255;
			colimagedata[3*(x -	3 +	w*(y+2))-1]  = 255;
			colimagedata[3*(x -	4 +	w*(y+3))-1]  = 255;
	//		colimagedata[3*(x -	5 +	w*(y+4))-1]  = 255;
		}
		//left-upper arm
		if(x > 4 && y > 3)
		{
			colimagedata[3*(x -	1 +	w*(y-1))-1]  = 255;
			colimagedata[3*(x -	2 +	w*(y-2))-1]  = 255;
			colimagedata[3*(x -	3 +	w*(y-3))-1]  = 255;
	//		colimagedata[3*(x -	4 +	w*(y-4))-1]  = 255;
			colimagedata[3*(x -	2 +	w*(y-1))-1]  = 255;
			colimagedata[3*(x -	3 +	w*(y-2))-1]  = 255;
			colimagedata[3*(x -	4 +	w*(y-3))-1]  = 255;
	//		colimagedata[3*(x -	5 +	w*(y-4))-1]  = 255;
		}

		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)colimagedata, 0, 0,
				     w, h,
			         CANVASLEFT1, CANVASTOP1,  IMGPLOT_COLOR_RGB24);
}


void Plot_circle(CDC *device, uInt8 colimagedata[], double r)
{

	int i,zeile,spalte;
	double phi,pi=3.14159;

	for(i=0;i<360;i++)
	{
		phi=(double)i/180.0*pi;
		spalte=ccd2.width/2  + (int)(r*cos(phi));
		zeile= ccd2.height/2 + (int)(r*sin(phi));

		if(spalte>=0 && spalte < ccd2.width && zeile >=0 && zeile < ccd2.height)
			colimagedata[3*(int)(640*zeile+spalte)-1]= (char)255;
	}


	int err=imgPlotDC ((GUIHNDL)device->m_hDC, (void *)colimagedata, 0, 0,
				        640, 480,
			            CANVASLEFT2, CANVASTOP2, IMGPLOT_COLOR_RGB24);

}


void Plot_number(CDC *device, uInt8 colimagedata[], int h, int w, int number, int x, int y, int place)
{
	char buffer[2];
			itoa(number, buffer, 10);
	int pos=0;

	if(x<15)
	{
		if(y<15)
		{
			x=10;
			y=15;
		}
		if(y>480)
		{
			x=10;
			y=470;
		}
		else
		{
			x=10;
			y=y-10;
		}
	}
	else if(x>615)
	{
		if(y<15)
		{
			x=615;
			y=15;
		}
		if(y>480)
		{
			x=615;
			y=470;
		}
		else
		{
			x=x-20;
			y=y-10;
		}
	}
	else
	{
		if(y<15)
		{
			x=x+2;
			y=15;
		}
		if(y>480)
		{
			x=x+2;
			y=470;
		}
		else
		{
			x=x+2;
			y=y-10;
		}
	}




/*
	if(x<=615 && y>=15)      
	{
		x=x+2;
		y=y-10;
	}
	else if(x<=615 && y<15)
	{
		x=x+2;
		y=y;
	}
	else if(x>615 && y>=15)
	{
		x=x-20;
		y=y-10;
	}
	else if(x>615 && y>=15)      
	{
		x=x-20;
		y=y;
	}
*/

	do{
		switch(buffer[pos]){
			case '0':
				colimagedata[3*(x +		w*(y  ))]  = 255;
				colimagedata[3*(x +		w*(y+1))]  = 255;
				colimagedata[3*(x +		w*(y+2))]  = 255;
				colimagedata[3*(x +		w*(y+3))]  = 255;
				colimagedata[3*(x +		w*(y+4))]  = 255;
				colimagedata[3*(x +		w*(y+5))]  = 255;
				colimagedata[3*(x +		w*(y+6))]  = 255;
				colimagedata[3*(x +		w*(y+7))]  = 255;
				colimagedata[3*(x +		w*(y+8))]  = 255;
				colimagedata[3*(x +		w*(y+9))]  = 255;

				colimagedata[3*(x + 1 + w*(y  ))]  = 255;
				colimagedata[3*(x + 2 + w*(y  ))]  = 255;
				colimagedata[3*(x + 3 + w*(y  ))]  = 255;
				colimagedata[3*(x + 4 + w*(y  ))]  = 255;
				colimagedata[3*(x + 5 + w*(y  ))]  = 255;

				colimagedata[3*(x + 1 + w*(y+9))]  = 255;
				colimagedata[3*(x + 2 + w*(y+9))]  = 255;
				colimagedata[3*(x + 3 + w*(y+9))]  = 255;
				colimagedata[3*(x + 4 + w*(y+9))]  = 255;
				colimagedata[3*(x + 5 + w*(y+9))]  = 255;


				colimagedata[3*(x + 5 + w*(y+1))]  = 255;
				colimagedata[3*(x + 5 + w*(y+2))]  = 255;
				colimagedata[3*(x + 5 + w*(y+3))]  = 255;
				colimagedata[3*(x + 5 + w*(y+4))]  = 255;
				colimagedata[3*(x + 5 + w*(y+5))]  = 255;
				colimagedata[3*(x + 5 + w*(y+6))]  = 255;
				colimagedata[3*(x + 5 + w*(y+7))]  = 255;
				colimagedata[3*(x + 5 + w*(y+8))]  = 255;
				colimagedata[3*(x + 5 + w*(y+9))]  = 255;
				break;
			case '1':
				colimagedata[3*(x + 2 + w*(y  ))]  = 255;
				colimagedata[3*(x + 2 + w*(y+1))]  = 255;
				colimagedata[3*(x + 2 + w*(y+2))]  = 255;
				colimagedata[3*(x + 2 + w*(y+3))]  = 255;
				colimagedata[3*(x + 2 + w*(y+4))]  = 255;
				colimagedata[3*(x + 2 + w*(y+5))]  = 255;
				colimagedata[3*(x + 2 + w*(y+6))]  = 255;
				colimagedata[3*(x + 2 + w*(y+7))]  = 255;
				colimagedata[3*(x + 2 + w*(y+8))]  = 255;
				colimagedata[3*(x + 2 + w*(y+9))]  = 255;
				break;
			case '2':
				colimagedata[3*(x +		w*(y  ))]  = 255;
				colimagedata[3*(x +		w*(y+1))]  = 255;
				colimagedata[3*(x +		w*(y+2))]  = 255;

				colimagedata[3*(x + 1 + w*(y  ))]  = 255;
				colimagedata[3*(x + 2 + w*(y  ))]  = 255;
				colimagedata[3*(x + 3 + w*(y  ))]  = 255;
				colimagedata[3*(x + 4 + w*(y  ))]  = 255;
				colimagedata[3*(x + 5 + w*(y  ))]  = 255;

				colimagedata[3*(x + 5 + w*(y+1))]  = 255;
				colimagedata[3*(x + 5 + w*(y+2))]  = 255;
				colimagedata[3*(x + 5 + w*(y+3))]  = 255;
				colimagedata[3*(x + 4 + w*(y+4))]  = 255;
				colimagedata[3*(x + 3 + w*(y+5))]  = 255;
				colimagedata[3*(x + 2 + w*(y+6))]  = 255;
				colimagedata[3*(x + 1 + w*(y+7))]  = 255;
				colimagedata[3*(x + 0 + w*(y+8))]  = 255;
				colimagedata[3*(x + 0 + w*(y+9))]  = 255;

				colimagedata[3*(x + 1 + w*(y+9))]  = 255;
				colimagedata[3*(x + 2 + w*(y+9))]  = 255;
				colimagedata[3*(x + 3 + w*(y+9))]  = 255;
				colimagedata[3*(x + 4 + w*(y+9))]  = 255;
				colimagedata[3*(x + 5 + w*(y+9))]  = 255;
				break;
			case '3':
				colimagedata[3*(x +		w*(y  ))]  = 255;
				colimagedata[3*(x +		w*(y+9))]  = 255;

				colimagedata[3*(x + 1 + w*(y  ))]  = 255;
				colimagedata[3*(x + 2 + w*(y  ))]  = 255;
				colimagedata[3*(x + 3 + w*(y  ))]  = 255;
				colimagedata[3*(x + 4 + w*(y  ))]  = 255;
				colimagedata[3*(x + 5 + w*(y  ))]  = 255;

				colimagedata[3*(x + 1 + w*(y+4))]  = 255;
				colimagedata[3*(x + 2 + w*(y+4))]  = 255;
				colimagedata[3*(x + 3 + w*(y+4))]  = 255;
				colimagedata[3*(x + 4 + w*(y+4))]  = 255;
				colimagedata[3*(x + 5 + w*(y+4))]  = 255;

				colimagedata[3*(x + 5 + w*(y+1))]  = 255;
				colimagedata[3*(x + 5 + w*(y+2))]  = 255;
				colimagedata[3*(x + 5 + w*(y+3))]  = 255;
				colimagedata[3*(x + 5 + w*(y+4))]  = 255;
				colimagedata[3*(x + 5 + w*(y+5))]  = 255;
				colimagedata[3*(x + 5 + w*(y+6))]  = 255;
				colimagedata[3*(x + 5 + w*(y+7))]  = 255;
				colimagedata[3*(x + 5 + w*(y+8))]  = 255;
				colimagedata[3*(x + 5 + w*(y+9))]  = 255;

				colimagedata[3*(x + 1 + w*(y+9))]  = 255;
				colimagedata[3*(x + 2 + w*(y+9))]  = 255;
				colimagedata[3*(x + 3 + w*(y+9))]  = 255;
				colimagedata[3*(x + 4 + w*(y+9))]  = 255;
				colimagedata[3*(x + 5 + w*(y+9))]  = 255;

				break;
			case  '4':
				colimagedata[3*(x +		w*(y  ))]  = 255;
				colimagedata[3*(x +		w*(y+1))]  = 255;
				colimagedata[3*(x +		w*(y+2))]  = 255;
				colimagedata[3*(x +		w*(y+3))]  = 255;
				colimagedata[3*(x +		w*(y+4))]  = 255;
				colimagedata[3*(x +		w*(y+5))]  = 255;

				colimagedata[3*(x +	4 +	w*(y+5))]  = 255;
				colimagedata[3*(x +	4 + w*(y+6))]  = 255;
				colimagedata[3*(x +	4 +	w*(y+7))]  = 255;
				colimagedata[3*(x +	4 +	w*(y+8))]  = 255;
				colimagedata[3*(x +	4 +	w*(y+9))]  = 255;

				colimagedata[3*(x + 1 + w*(y+6))]  = 255;
				colimagedata[3*(x + 2 + w*(y+6))]  = 255;
				colimagedata[3*(x + 3 + w*(y+6))]  = 255;
				colimagedata[3*(x + 4 + w*(y+6))]  = 255;
				colimagedata[3*(x + 5 + w*(y+6))]  = 255;
				break;
			case '5':
				colimagedata[3*(x +		w*(y  ))]  = 255;
				colimagedata[3*(x +		w*(y+1))]  = 255;
				colimagedata[3*(x +		w*(y+2))]  = 255;
				colimagedata[3*(x +		w*(y+3))]  = 255;
				colimagedata[3*(x +		w*(y+4))]  = 255;
				colimagedata[3*(x +		w*(y+5))]  = 255;
				colimagedata[3*(x +		w*(y+9))]  = 255;

				colimagedata[3*(x + 1 + w*(y  ))]  = 255;
				colimagedata[3*(x + 2 + w*(y  ))]  = 255;
				colimagedata[3*(x + 3 + w*(y  ))]  = 255;
				colimagedata[3*(x + 4 + w*(y  ))]  = 255;
				colimagedata[3*(x + 5 + w*(y  ))]  = 255;

				colimagedata[3*(x + 1 + w*(y+5))]  = 255;
				colimagedata[3*(x + 2 + w*(y+5))]  = 255;
				colimagedata[3*(x + 3 + w*(y+5))]  = 255;
				colimagedata[3*(x + 4 + w*(y+5))]  = 255;
				colimagedata[3*(x + 5 + w*(y+5))]  = 255;

				colimagedata[3*(x + 5 + w*(y+1))]  = 255;

				colimagedata[3*(x + 5 + w*(y+5))]  = 255;
				colimagedata[3*(x + 5 + w*(y+6))]  = 255;
				colimagedata[3*(x + 5 + w*(y+7))]  = 255;
				colimagedata[3*(x + 5 + w*(y+8))]  = 255;
				colimagedata[3*(x + 5 + w*(y+9))]  = 255;

				colimagedata[3*(x + 1 + w*(y+9))]  = 255;
				colimagedata[3*(x + 2 + w*(y+9))]  = 255;
				colimagedata[3*(x + 3 + w*(y+9))]  = 255;
				colimagedata[3*(x + 4 + w*(y+9))]  = 255;
				colimagedata[3*(x + 5 + w*(y+9))]  = 255;
				break;
			case '6':
				colimagedata[3*(x +	3+	w*(y  ))]  = 255;
				colimagedata[3*(x +	2+	w*(y+1))]  = 255;
				colimagedata[3*(x +	1+	w*(y+2))]  = 255;
				colimagedata[3*(x +		w*(y+3))]  = 255;
				colimagedata[3*(x +		w*(y+4))]  = 255;
				colimagedata[3*(x +		w*(y+5))]  = 255;
				colimagedata[3*(x +		w*(y+6))]  = 255;
				colimagedata[3*(x +		w*(y+7))]  = 255;
				colimagedata[3*(x +		w*(y+8))]  = 255;
				colimagedata[3*(x +		w*(y+9))]  = 255;

				colimagedata[3*(x + 1 + w*(y+4))]  = 255;
				colimagedata[3*(x + 2 + w*(y+4))]  = 255;
				colimagedata[3*(x + 3 + w*(y+4))]  = 255;
				colimagedata[3*(x + 4 + w*(y+4))]  = 255;
				colimagedata[3*(x + 5 + w*(y+4))]  = 255;

				colimagedata[3*(x + 5 + w*(y+4))]  = 255;
				colimagedata[3*(x + 5 + w*(y+5))]  = 255;
				colimagedata[3*(x + 5 + w*(y+6))]  = 255;
				colimagedata[3*(x + 5 + w*(y+7))]  = 255;
				colimagedata[3*(x + 5 + w*(y+8))]  = 255;
				colimagedata[3*(x + 5 + w*(y+9))]  = 255;

				colimagedata[3*(x + 1 + w*(y+9))]  = 255;
				colimagedata[3*(x + 2 + w*(y+9))]  = 255;
				colimagedata[3*(x + 3 + w*(y+9))]  = 255;
				colimagedata[3*(x + 4 + w*(y+9))]  = 255;
				colimagedata[3*(x + 5 + w*(y+9))]  = 255;
				break;
			case '7':
				colimagedata[3*(x +		w*(y  ))]  = 255;
				colimagedata[3*(x +		w*(y+1))]  = 255;
				colimagedata[3*(x +		w*(y+2))]  = 255;

				colimagedata[3*(x + 1 + w*(y  ))]  = 255;
				colimagedata[3*(x + 2 + w*(y  ))]  = 255;
				colimagedata[3*(x + 3 + w*(y  ))]  = 255;
				colimagedata[3*(x + 4 + w*(y  ))]  = 255;
				colimagedata[3*(x + 5 + w*(y  ))]  = 255;

				colimagedata[3*(x + 5 + w*(y+1))]  = 255;
				colimagedata[3*(x + 5 + w*(y+2))]  = 255;
				colimagedata[3*(x + 5 + w*(y+3))]  = 255;
				colimagedata[3*(x + 4 + w*(y+4))]  = 255;
				colimagedata[3*(x + 3 + w*(y+5))]  = 255;
				colimagedata[3*(x + 2 + w*(y+6))]  = 255;
				colimagedata[3*(x + 1 + w*(y+7))]  = 255;
				colimagedata[3*(x + 0 + w*(y+8))]  = 255;
				colimagedata[3*(x + 0 + w*(y+9))]  = 255;
				break;

			case '8':
				colimagedata[3*(x +		w*(y  ))]  = 255;
				colimagedata[3*(x +		w*(y+1))]  = 255;
				colimagedata[3*(x +		w*(y+2))]  = 255;
				colimagedata[3*(x +		w*(y+3))]  = 255;
				colimagedata[3*(x +		w*(y+4))]  = 255;
				colimagedata[3*(x +		w*(y+5))]  = 255;
				colimagedata[3*(x +		w*(y+6))]  = 255;
				colimagedata[3*(x +		w*(y+7))]  = 255;
				colimagedata[3*(x +		w*(y+8))]  = 255;
				colimagedata[3*(x +		w*(y+9))]  = 255;

				colimagedata[3*(x + 1 + w*(y  ))]  = 255;
				colimagedata[3*(x + 2 + w*(y  ))]  = 255;
				colimagedata[3*(x + 3 + w*(y  ))]  = 255;
				colimagedata[3*(x + 4 + w*(y  ))]  = 255;
				colimagedata[3*(x + 5 + w*(y  ))]  = 255;

				colimagedata[3*(x + 1 + w*(y+4))]  = 255;
				colimagedata[3*(x + 2 + w*(y+4))]  = 255;
				colimagedata[3*(x + 3 + w*(y+4))]  = 255;
				colimagedata[3*(x + 4 + w*(y+4))]  = 255;
				colimagedata[3*(x + 5 + w*(y+4))]  = 255;

				colimagedata[3*(x + 5 + w*(y+1))]  = 255;
				colimagedata[3*(x + 5 + w*(y+2))]  = 255;
				colimagedata[3*(x + 5 + w*(y+3))]  = 255;
				colimagedata[3*(x + 5 + w*(y+4))]  = 255;
				colimagedata[3*(x + 5 + w*(y+5))]  = 255;
				colimagedata[3*(x + 5 + w*(y+6))]  = 255;
				colimagedata[3*(x + 5 + w*(y+7))]  = 255;
				colimagedata[3*(x + 5 + w*(y+8))]  = 255;
				colimagedata[3*(x + 5 + w*(y+9))]  = 255;

				colimagedata[3*(x + 1 + w*(y+9))]  = 255;
				colimagedata[3*(x + 2 + w*(y+9))]  = 255;
				colimagedata[3*(x + 3 + w*(y+9))]  = 255;
				colimagedata[3*(x + 4 + w*(y+9))]  = 255;
				colimagedata[3*(x + 5 + w*(y+9))]  = 255;
				break;
				case '9':
				colimagedata[3*(x +		w*(y  ))]  = 255;
				colimagedata[3*(x +		w*(y+1))]  = 255;
				colimagedata[3*(x +		w*(y+2))]  = 255;
				colimagedata[3*(x +		w*(y+3))]  = 255;
				colimagedata[3*(x +		w*(y+4))]  = 255;

				colimagedata[3*(x + 1 + w*(y  ))]  = 255;
				colimagedata[3*(x + 2 + w*(y  ))]  = 255;
				colimagedata[3*(x + 3 + w*(y  ))]  = 255;
				colimagedata[3*(x + 4 + w*(y  ))]  = 255;
				colimagedata[3*(x + 5 + w*(y  ))]  = 255;

				colimagedata[3*(x + 1 + w*(y+4))]  = 255;
				colimagedata[3*(x + 2 + w*(y+4))]  = 255;
				colimagedata[3*(x + 3 + w*(y+4))]  = 255;
				colimagedata[3*(x + 4 + w*(y+4))]  = 255;
				colimagedata[3*(x + 5 + w*(y+4))]  = 255;

				colimagedata[3*(x + 5 + w*(y+1))]  = 255;
				colimagedata[3*(x + 5 + w*(y+2))]  = 255;
				colimagedata[3*(x + 5 + w*(y+3))]  = 255;
				colimagedata[3*(x + 5 + w*(y+4))]  = 255;
				colimagedata[3*(x + 5 + w*(y+5))]  = 255;
				colimagedata[3*(x + 5 + w*(y+6))]  = 255;
				colimagedata[3*(x + 5 + w*(y+7))]  = 255;
				colimagedata[3*(x + 5 + w*(y+8))]  = 255;
				colimagedata[3*(x + 5 + w*(y+9))]  = 255;
				break;
				}//switch
			x += 7;	
		}
		while(buffer[++pos] != '\0');
		

	if(place==1)
		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)colimagedata, 0, 0,
				    w, h,
			        CANVASLEFT1, CANVASTOP1,  IMGPLOT_COLOR_RGB24);

	else if(place==3)
		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)colimagedata, 0, 0,
				    w, h,
			        CANVASLEFT3, CANVASTOP3,  IMGPLOT_COLOR_RGB24);
}



void Plot_epi(CDC *device, uInt8 imagedata[], EpiPar epipar[], int h, int w, int num, 
			  CamPoint lim1[], CamPoint lim2[], int red)
{
	int j;
	double a,b, a_res, b_res, dummy;
	int x,y;

	for(j=1;j <= num; j++)
	{
		//***************rescale epi parameters from normalized metric to pixel************************
		a = -epipar[j].a / epipar[j].b;
		b = -epipar[j].c / epipar[j].b;

		a_res= -a *ccd2.pix_w/ccd2.pix_h;
		b_res= (double)ccd2.height/2.0 - in2.y_h/ccd2.pix_h + in2.focus*b/ccd2.pix_h +
							(double)ccd2.width/2.0* a *ccd2.pix_w/ccd2.pix_h + a*in2.x_h /ccd2.pix_h;

		//depending on the slope (a_res) the depending variable is changed 
		if(fabs(a_res)<1)
		{
			if(lim1[j].x > lim2[j].x)
			{
				dummy = lim1[j].x;
				lim1[j].x = lim2[j].x;
				lim2[j].x = dummy;
			}

			if(lim1[j].x <= 0)
				lim1[j].x = 1;
			if(lim2[j].x <= 0)
				lim2[j].x = 1;		
			if(lim2[j].x > 640)
				lim2[j].x = 640;
			if(lim1[j].x > 640)
				lim1[j].x = 640;

			x=0;
			y=(int)b_res;
			while(y<0 && y>h && x < w)
			{
				x++;
				y=(int)(a_res*x+b_res);
			}
			if(x<w && x>=0)
			{
				for(x=(int)lim1[j].x; x<=(int)lim2[j].x; x++)
				{
					y=(int)(a_res*x+b_res);
					if(y >=0 && y< h && x < w)
						imagedata[3*(y*w+x)+red]= (char)255;
				}
			}
		}
		else 
		{		
			if(lim1[j].y > lim2[j].y)
			{
				dummy = lim1[j].y;
				lim1[j].y = lim2[j].y;
				lim2[j].y = dummy;
			}

			if(lim1[j].y <= 0)
				lim1[j].y = 1;
			if(lim2[j].y <= 0)
				lim2[j].y = 1;		
			if(lim2[j].y > h)
				lim2[j].y = h;
			if(lim1[j].y > h)
				lim1[j].y = h;
		
			b_res=-b_res/a_res;
			a_res=1.0/a_res;

			y=0;
			x=(int)b_res;
			while(x<0 && x>w && y < h)
			{
				y++;
				x=(int)(a_res*y+b_res);
			}
			if(y<h && y>=0)
			{
				for(y=(int)lim1[j].y; y<=(int)lim2[j].y; y++)
				{
					x=(int)(a_res*y+b_res);
					if(x >=0 && x< w && y < h)
						imagedata[3*(y*w+x)+red]= (char)255;
				}
			}		
		}
	}
//***************Plot epi lines*****************************************************************************
//	Plot_image(device, imagedata,  2, 0);
	imgPlotDC ((GUIHNDL)device->m_hDC, (void *)imagedata, 0, 0,
				 640, 480,
			     CANVASLEFT2, CANVASTOP2,  IMGPLOT_COLOR_RGB24);
//***************End Plot epi lines*************************************************************************
}


void Plot_epi_no_lim(CDC *device, uInt8 imagedata[], EpiPar epipar[], int h, int w, int num, int red)
{
	int j;
	double a,b, a_res, b_res;
	int x,y;

	for(j=0;j < num; j++)
	{
		//***************rescale epi parameters from normalized metric to pixel************************
		a = -epipar[j].a / epipar[j].b;
		b = -epipar[j].c / epipar[j].b;

		a_res= -a *ccd2.pix_w/ccd2.pix_h;
		b_res= (double)ccd2.height/2.0 - in2.y_h/ccd2.pix_h + in2.focus*b/ccd2.pix_h +
							(double)ccd2.width/2.0* a *ccd2.pix_w/ccd2.pix_h + a*in2.x_h /ccd2.pix_h;

		//depending on the slope (a_res) the depending variable is changed 
		if(fabs(a_res)<1)
		{
			x=0;
			y=(int)b_res;
			while(y<0 && y>h && x < w)
			{
				x++;
				y=(int)(a_res*x+b_res);
			}
			if(x<w && x>=0)
			{
				for(x= 0; x< 640; x++)
				{
					y=(int)(a_res*x+b_res);
					if(y >=0 && y< h && x < w)
						imagedata[3*(y*w+x)+red]= (char)255;
				}
			}
		}
		else 
		{		
			b_res=-b_res/a_res;
			a_res=1.0/a_res;

			y=0;
			x=(int)b_res;
			while(x<0 && x>w && y < h)
			{
				y++;
				x=(int)(a_res*y+b_res);
			}
			if(y<h && y>=0)
			{
				for(y=0; y < 480; y++)
				{
					x=(int)(a_res*y+b_res);
					if(x >=0 && x< w && y < h)
						imagedata[3*(y*w+x)+red]= (char)255;
				}
			}		
		}
	}
//***************Plot epi lines*****************************************************************************
//	Plot_image(device, imagedata,  2, 0);
	imgPlotDC ((GUIHNDL)device->m_hDC, (void *)imagedata, 0, 0,
				 640, 480,
			     CANVASLEFT2, CANVASTOP2,  IMGPLOT_COLOR_RGB24);
//***************End Plot epi lines*************************************************************************
}