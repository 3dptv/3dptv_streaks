#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include "niimaq.h"
#include "parameters.h"
#include "matrix_op.h"
#include "plot.h"
 
int Plot::width=640;
int Plot::height=480;

void Plot::Points(CDC *device, uInt8 colimagedata[], CamPoint point[], int nr, int r, int g, int b)
{

	for(int i=0; i < nr;i++){
		if(point[i].x >= 1 && point[i].x < width-1 && point[i].y >= 1 && point[i].y < height-1)
		{
			colimagedata[3*((int)point[i].x    +(640*(int)point[i].y    ))]  = (uInt8)r;
			colimagedata[3*((int)point[i].x +1 +(640*(int)point[i].y    ))]  = (uInt8)r;
			colimagedata[3*((int)point[i].x -1 +(640*(int)point[i].y    ))]  = (uInt8)r;
			colimagedata[3*((int)point[i].x    +(640*((int)point[i].y+1)))]  = (uInt8)r;
			colimagedata[3*((int)point[i].x    +(640*((int)point[i].y-1)))]  = (uInt8)r;

			colimagedata[3*((int)point[i].x    +(640*(int)point[i].y    ))+1]= (uInt8)g;
			colimagedata[3*((int)point[i].x +1 +(640*(int)point[i].y    ))+1]= (uInt8)g;
			colimagedata[3*((int)point[i].x -1 +(640*(int)point[i].y    ))+1]= (uInt8)g;
			colimagedata[3*((int)point[i].x    +(640*((int)point[i].y+1)))+1]= (uInt8)g;
			colimagedata[3*((int)point[i].x    +(640*((int)point[i].y-1)))+1]= (uInt8)g;

			colimagedata[3*((int)point[i].x    +(640*(int)point[i].y    ))+2]= (uInt8)b;
			colimagedata[3*((int)point[i].x +1 +(640*(int)point[i].y    ))+2]= (uInt8)b;
			colimagedata[3*((int)point[i].x -1 +(640*(int)point[i].y    ))+2]= (uInt8)b;
			colimagedata[3*((int)point[i].x    +(640*((int)point[i].y+1)))+2]= (uInt8)b;
			colimagedata[3*((int)point[i].x    +(640*((int)point[i].y-1)))+2]= (uInt8)b;
		}
	}
}

void Plot::Points(CDC *device, uInt8 colimagedata[], Pixel point[], int nr, int r, int g, int b)
{

	for(int i=0; i < nr;i++){
		if(point[i].x >= 1 && point[i].x < width-1 && point[i].y >= 1 && point[i].y < height-1)
		{
			colimagedata[3*(point[i].x    +(640*point[i].y    ))]  = (uInt8)r;
			colimagedata[3*(point[i].x +1 +(640*point[i].y    ))]  = (uInt8)r;
			colimagedata[3*(point[i].x -1 +(640*point[i].y    ))]  = (uInt8)r;
			colimagedata[3*(point[i].x    +(640*(point[i].y+1)))]  = (uInt8)r;
			colimagedata[3*(point[i].x    +(640*(point[i].y-1)))]  = (uInt8)r;

			colimagedata[3*(point[i].x    +(640*point[i].y    ))+1]= (uInt8)g;
			colimagedata[3*(point[i].x +1 +(640*point[i].y    ))+1]= (uInt8)g;
			colimagedata[3*(point[i].x -1 +(640*point[i].y    ))+1]= (uInt8)g;
			colimagedata[3*(point[i].x    +(640*(point[i].y+1)))+1]= (uInt8)g;
			colimagedata[3*(point[i].x    +(640*(point[i].y-1)))+1]= (uInt8)g;

			colimagedata[3*(point[i].x    +(640*point[i].y    ))+2]= (uInt8)b;
			colimagedata[3*(point[i].x +1 +(640*point[i].y    ))+2]= (uInt8)b;
			colimagedata[3*(point[i].x -1 +(640*point[i].y    ))+2]= (uInt8)b;
			colimagedata[3*(point[i].x    +(640*(point[i].y+1)))+2]= (uInt8)b;
			colimagedata[3*(point[i].x    +(640*(point[i].y-1)))+2]= (uInt8)b;
		}
	}
}


void Plot::Pixels(CDC *device, uInt8 colimagedata[], Pixel point[], int nr, int r, int g, int b)
{

	for(int i=0; i < nr;i++){
		if(point[i].x >= 0 && point[i].x < width && point[i].y >= 0 && point[i].y < height)
		{
			colimagedata[3*(point[i].x    +(640*point[i].y    ))]  = (uInt8)r;
			colimagedata[3*(point[i].x    +(640*point[i].y    ))+1]= (uInt8)g;
			colimagedata[3*(point[i].x    +(640*point[i].y    ))+2]= (uInt8)b;
		}
	}
}


void Plot::Pixels(CDC *device, uInt8 colimagedata[], CamPoint point[], int nr, int r, int g, int b)
{

	for(int i=0; i < nr;i++){
		if(point[i].x >= 0 && point[i].x < width && point[i].y >= 0 && point[i].y < height)
		{
			colimagedata[3*((int)point[i].x    +(640*(int)point[i].y    ))  ]= (uInt8)r;
			colimagedata[3*((int)point[i].x    +(640*(int)point[i].y    ))+1]= (uInt8)g;
			colimagedata[3*((int)point[i].x    +(640*(int)point[i].y    ))+2]= (uInt8)b;
		}
	}
}



void Plot::Circles(CDC *device, uInt8 colimagedata[], CamPoint point[], int nr, int r, int g, int b)
{
	char col[3]={(uInt8)r,(uInt8)g,(uInt8)b};

	for(int i=0; i < nr;i++){
		if(point[i].x >= 4 && point[i].x < width-4 && point[i].y >= 4 && point[i].y < height-4)
		{
			for(int c=0;c<3;c++)
			{
				colimagedata[3*((int)point[i].x +2 +640*((int)point[i].y +3   ))+c]  = col[c];
				colimagedata[3*((int)point[i].x +3 +640*((int)point[i].y +2   ))+c]  = col[c];

				colimagedata[3*((int)point[i].x -2 +640*((int)point[i].y +3   ))+c]  = col[c];
				colimagedata[3*((int)point[i].x -3 +640*((int)point[i].y +2   ))+c]  = col[c];

				colimagedata[3*((int)point[i].x -2 +640*((int)point[i].y -3   ))+c]  = col[c];
				colimagedata[3*((int)point[i].x -3 +640*((int)point[i].y -2   ))+c]  = col[c];

				colimagedata[3*((int)point[i].x +2 +640*((int)point[i].y -3   ))+c]  = col[c];
				colimagedata[3*((int)point[i].x +3 +640*((int)point[i].y -2   ))+c]  = col[c];

				colimagedata[3*((int)point[i].x -1 +640*((int)point[i].y -4   ))+c]  = col[c];
				colimagedata[3*((int)point[i].x    +640*((int)point[i].y -4   ))+c]  = col[c];
				colimagedata[3*((int)point[i].x +1 +640*((int)point[i].y -4   ))+c]  = col[c];

				colimagedata[3*((int)point[i].x -1 +640*((int)point[i].y +4   ))+c]  = col[c];
				colimagedata[3*((int)point[i].x    +640*((int)point[i].y +4   ))+c]  = col[c];
				colimagedata[3*((int)point[i].x +1 +640*((int)point[i].y +4   ))+c]  = col[c];

				colimagedata[3*((int)point[i].x -4 +640*((int)point[i].y -1   ))+c]  = col[c];
				colimagedata[3*((int)point[i].x -4 +640*((int)point[i].y      ))+c]  = col[c];
				colimagedata[3*((int)point[i].x -4 +640*((int)point[i].y +1   ))+c]  = col[c];

				colimagedata[3*((int)point[i].x +4 +640*((int)point[i].y -1   ))+c]  = col[c];
				colimagedata[3*((int)point[i].x +4 +640*((int)point[i].y      ))+c]  = col[c];
				colimagedata[3*((int)point[i].x +4 +640*((int)point[i].y +1   ))+c]  = col[c];
			}
		}
	}
}


void Plot::Circles(CDC *device, uInt8 colimagedata[], Pixel point[], int nr, int r, int g, int b)
{
	char col[3]={(uInt8)r,(uInt8)g,(uInt8)b};

	for(int i=0; i < nr;i++){
		if(point[i].x >= 4 && point[i].x < width-4 && point[i].y >= 4 && point[i].y < height-4)
		{
			for(int c=0;c<3;c++)
			{
				colimagedata[3*(point[i].x +2 +640*(point[i].y +3   ))+c]  = col[c];
				colimagedata[3*(point[i].x +3 +640*(point[i].y +2   ))+c]  = col[c];

				colimagedata[3*(point[i].x -2 +640*(point[i].y +3   ))+c]  = col[c];
				colimagedata[3*(point[i].x -3 +640*(point[i].y +2   ))+c]  = col[c];

				colimagedata[3*(point[i].x -2 +640*(point[i].y -3   ))+c]  = col[c];
				colimagedata[3*(point[i].x -3 +640*(point[i].y -2   ))+c]  = col[c];

				colimagedata[3*(point[i].x +2 +640*(point[i].y -3   ))+c]  = col[c];
				colimagedata[3*(point[i].x +3 +640*(point[i].y -2   ))+c]  = col[c];

				colimagedata[3*(point[i].x -1 +640*(point[i].y -4   ))+c]  = col[c];
				colimagedata[3*(point[i].x    +640*(point[i].y -4   ))+c]  = col[c];
				colimagedata[3*(point[i].x +1 +640*(point[i].y -4   ))+c]  = col[c];

				colimagedata[3*(point[i].x -1 +640*(point[i].y +4   ))+c]  = col[c];
				colimagedata[3*(point[i].x    +640*(point[i].y +4   ))+c]  = col[c];
				colimagedata[3*(point[i].x +1 +640*(point[i].y +4   ))+c]  = col[c];

				colimagedata[3*(point[i].x -4 +640*(point[i].y -1   ))+c]  = col[c];
				colimagedata[3*(point[i].x -4 +640*(point[i].y      ))+c]  = col[c];
				colimagedata[3*(point[i].x -4 +640*(point[i].y +1   ))+c]  = col[c];

				colimagedata[3*(point[i].x +4 +640*(point[i].y -1   ))+c]  = col[c];
				colimagedata[3*(point[i].x +4 +640*(point[i].y      ))+c]  = col[c];
				colimagedata[3*(point[i].x +4 +640*(point[i].y +1   ))+c]  = col[c];
			}
		}
	}
}





void Plot::Line(CDC *device, uInt8 imagedata[], double a, double b, double c)
{
	//  !!
	//this codes need a check of the slope, otherwise the loops can be very long!!
	//convert from ax+by+c=0 into y=ax+b
	
	int x,y;
	int h=480,w=640;

	
	if(b != 0)	
	{
		a=-a/b;
		b=-c/b;

		//depending on the slope a the depending variable is changed 
		if(fabs(a)<1)
		{
			x=0;
			y=(int)b;
			while(y<0 || y>=h && x <= w)
				y=(int)(a* ++x +b);

			while(x<w && x>=0 && y>=0 && y<h)
			{
				x++;
				y=(int)(a*x+b);
				imagedata[3*(y*w+x)+1]= (char)20;
			}
		}
		else 
		{		
			b=-b/a;
			a=1.0/a;

			y=0;
			x=(int)b;
			while(x<0 || x>=w && y <= h)
				x=(int)(a* ++y +b);

			while(y<h && y>=0 && x >=0 && x<w)
			{
				y++;
				x=(int)(a*y+b);
				imagedata[3*(y*w+x)+1]= (char)20;
			}		
		}
	}

	else
	{
		b=-c/a;

		if(b>=0 && b<w)
		{
			y=0;
			x=(int)b;
			while(y++ < h)
			{
				imagedata[3*(y*w+x)+1]= (char)20;
			}	
		}
	}
}


void Plot::Line(CDC *device, uInt8 imagedata[], double a, double b, double c, double px, double py, double l)
{
	//with limiter
	int x,y;
	int h=480,w=640;

	
	if(b != 0)	
	{
		a=-a/b;
		b=-c/b;

		double phi=atan(a);
		double dx=l*sin(phi);
		//depending on the slope a the depending variable is changed 
		if(fabs(a)<1)
		{
			x=0;
			y=(int)b;
			while(y<0 || y>=h && x <= w || x< px-dx )
				y=(int)(a* ++x +b);

			while(x<w && x>=0 && y>=0 && y<h && x < px+dx)
			{
				x++;
				y=(int)(a*x+b);
				imagedata[3*(y*w+x)+1]= (char)20;
			}
		}
		else 
		{		
			b=-b/a;
			a=1.0/a;

			double phi=atan(a);
			double dy=l*cos(phi);
			y=0;
			x=(int)b;
			while(x<0 || x>=w && y <= h || y < py-dy)
				x=(int)(a* ++y +b);

			while(y<h && y>=0 && x >=0 && x<w && y < py+dy)
			{
				y++;
				x=(int)(a*y+b);
				imagedata[3*(y*w+x)+1]= (char)20;
			}		
		}
	}

	else
	{
		b=-c/a;

		if(b>=0 && b<w)
		{
			y=0;
			x=(int)b;
			while(y++ < h)
			{
				imagedata[3*(y*w+x)+1]= (char)20;
			}	
		}
	}
}


void Plot::Line(CDC *device, uInt8 imagedata[], double a, double b)
{
	int x,y;
	int h=480,w=640;

	//depending on the slope a the depending variable is changed 
	if(fabs(a)<1)
	{
		x=0;
		y=(int)b;
		while(y<0 && y>h && x < w)
			y=(int)(a* ++x +b);

		while(x<w-1 && x>=0 && y>=0 && y<h)
		{
			x++;
			y=(int)(a*x+b);
			imagedata[3*(y*w+x)+1]= (char)20;
		}
	}
	else 
	{		
		b=-b/a;
		a=1.0/a;

		y=0;
		x=(int)b;
		while((x<0 || x>w) && y < h)
			x=(int)(a* ++y +b);

		while(y<h && y>=0 && x >=0 && x<w)
		{
			y++;
			x=(int)(a*y+b);
			imagedata[3*(y*w+x)+1]= (char)20;
		}		
	}
}




void Plot::Image(CDC *device, uInt8 *ImgAddr, int pos)
{

	
	if(pos==1)
	{
		int err=imgPlotDC ((GUIHNDL)device->m_hDC, (void *)ImgAddr, 0, 0,
				        640, 480,
			            CANVASLEFT1, CANVASTOP1, FALSE);
	}
	if(pos==2)
	{
		int err=imgPlotDC ((GUIHNDL)device->m_hDC, (void *)ImgAddr, 0, 0,
				        640, 480,
			            CANVASLEFT2, CANVASTOP2, FALSE);
	}
}


void Plot::Image_col(CDC *device, uInt8 *ImgAddr, int pos)
{

	
	if(pos==1)
		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)ImgAddr, 0, 0,
				     640, 480,
			         CANVASLEFT1, CANVASTOP1,  IMGPLOT_COLOR_RGB24);

	else 
		imgPlotDC ((GUIHNDL)device->m_hDC, (void *)ImgAddr, 0, 0,
				     640, 480,
			         CANVASLEFT2, CANVASTOP2,  IMGPLOT_COLOR_RGB24);

}



void Plot::display(int number, char text[], CEdit *status)
{
	char buffer[256];
	
	status->ReplaceSel(text,FALSE);

	if(number!=-99){
		itoa(number,buffer,10);
		status->ReplaceSel(buffer, FALSE);
	}
	status->ReplaceSel("\r\n",FALSE);

}



void Plot::display(int number, int number2, char text[], CEdit *status)
{
	char buffer[256];
	
	status->ReplaceSel(text,FALSE);

	if(number!=-99){
		itoa(number,buffer,10);
		status->ReplaceSel(buffer, FALSE);
	}
	status->ReplaceSel("  ",FALSE);
	if(number!=-99){
		itoa(number2,buffer,10);
		status->ReplaceSel(buffer, FALSE);
	}
	status->ReplaceSel("\r\n",FALSE);

}

void Plot::display(double number, char text[], CEdit *status)
{
	char buffer[50];
	
	status->ReplaceSel(text,FALSE);

	if(number!=-99){
		gcvt(number, 7, buffer);
		status->ReplaceSel(buffer, FALSE);
	}
	status->ReplaceSel("\r\n",FALSE);
}

void Plot::display(double number,double number2, char text[], CEdit *status)
{
	char buffer[50];
	
	status->ReplaceSel(text,FALSE);

	status->ReplaceSel("(",FALSE);
	if(number!=-99){
		gcvt(number, 7, buffer);
		status->ReplaceSel(buffer, FALSE);
	}
	status->ReplaceSel(",",FALSE);
	if(number!=-99){
		gcvt(number2, 7, buffer);
		status->ReplaceSel(buffer, FALSE);
	}
	status->ReplaceSel(")",FALSE);
	status->ReplaceSel("\r\n",FALSE);
}


void Plot::display(double number,double number2, double number3, char text[], CEdit *status)
{
	char buffer[50];
	
	status->ReplaceSel(text,FALSE);

	status->ReplaceSel("(",FALSE);
	if(number!=-99){
		gcvt(number, 7, buffer);
		status->ReplaceSel(buffer, FALSE);
	}
	status->ReplaceSel(",",FALSE);
	if(number!=-99){
		gcvt(number2, 7, buffer);
		status->ReplaceSel(buffer, FALSE);
	}
	status->ReplaceSel(",",FALSE);
	if(number!=-99){
		gcvt(number3, 7, buffer);
		status->ReplaceSel(buffer, FALSE);
	}
	status->ReplaceSel(")",FALSE);
	status->ReplaceSel("\r\n",FALSE);
}


void Plot::Histogram(CDC *device, double* point, int nr, int r, int g, int b)
{
	int origin,i,j;
	uInt8 imagedata[3*256*110];
	uInt8 *image=imagedata;

	for(i=0;i<3*256*110;i++)
		*image++=255;

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





void Plot::Number(CDC *device, uInt8 colimagedata[],int x, int y,int number)
{

	int h=480;
	int w=640;
	int i;
	const int col[3]={0,0,0};


	char buffer[2];
			itoa(number, buffer, 10);
	int pos=0;

	if(x<15)
	{
		if(y<15)
			{x=10;	y=15;}
		if(y>480)
			{x=10;  y=470;}
		else
			{x=10;  y=y-10;}
	}
	else if(x>615)
	{
		if(y<15)
		{x=615;	y=15;}
		if(y>480)
		{x=615;	y=470;
		}
		else
		{x=x-20;y=y-10;}
	}
	else
	{
		if(y<15)
		{x=x+2;y=15;}
		if(y>480)
		{x=x+2;y=470;}
		else
		{x=x+2;y=y-10;}
	}

	do{
		switch(buffer[pos]){
			case '0':
				for(i=0;i<3;i++)
				{
				colimagedata[3*(x +		w*(y  ))+i]  = col[i];
				colimagedata[3*(x +		w*(y+1))+i]  = col[i];
				colimagedata[3*(x +		w*(y+2))+i]  = col[i];
				colimagedata[3*(x +		w*(y+3))+i]  = col[i];
				colimagedata[3*(x +		w*(y+4))+i]  = col[i];
				colimagedata[3*(x +		w*(y+5))+i]  = col[i];
				colimagedata[3*(x +		w*(y+6))+i]  = col[i];
				colimagedata[3*(x +		w*(y+7))+i]  = col[i];
				colimagedata[3*(x +		w*(y+8))+i]  = col[i];
				colimagedata[3*(x +		w*(y+9))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y  ))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+9))+i]  = col[i];


				colimagedata[3*(x + 5 + w*(y+1))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+2))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+3))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+7))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+8))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+9))+i]  = col[i];
				}
				break;
			case '1':
				for(i=0;i<3;i++)
				{
				colimagedata[3*(x + 2 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+1))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+2))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+3))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+7))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+8))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+9))+i]  = col[i];
				}
				break;
			case '2':
				for(i=0;i<3;i++)
				{
				colimagedata[3*(x +		w*(y  ))+i]  = col[i];
				colimagedata[3*(x +		w*(y+1))+i]  = col[i];
				colimagedata[3*(x +		w*(y+2))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y  ))+i]  = col[i];

				colimagedata[3*(x + 5 + w*(y+1))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+2))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+3))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 1 + w*(y+7))+i]  = col[i];
				colimagedata[3*(x + 0 + w*(y+8))+i]  = col[i];
				colimagedata[3*(x + 0 + w*(y+9))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+9))+i]  = col[i];
				}
				break;
			case '3':
				for(i=0;i<3;i++)
				{
				colimagedata[3*(x +		w*(y  ))+i]  = col[i];
				colimagedata[3*(x +		w*(y+9))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y  ))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+4))+i]  = col[i];

				colimagedata[3*(x + 5 + w*(y+1))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+2))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+3))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+7))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+8))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+9))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+9))+i]  = col[i];
				}
				break;
			case  '4':
				for(i=0;i<3;i++)
				{
				colimagedata[3*(x +		w*(y  ))+i]  = col[i];
				colimagedata[3*(x +		w*(y+1))+i]  = col[i];
				colimagedata[3*(x +		w*(y+2))+i]  = col[i];
				colimagedata[3*(x +		w*(y+3))+i]  = col[i];
				colimagedata[3*(x +		w*(y+4))+i]  = col[i];
				colimagedata[3*(x +		w*(y+5))+i]  = col[i];

				colimagedata[3*(x +	4 +	w*(y+5))+i]  = col[i];
				colimagedata[3*(x +	4 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x +	4 +	w*(y+7))+i]  = col[i];
				colimagedata[3*(x +	4 +	w*(y+8))+i]  = col[i];
				colimagedata[3*(x +	4 +	w*(y+9))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+6))+i]  = col[i];
				}
				break;
			case '5':
				for(i=0;i<3;i++)
				{
				colimagedata[3*(x +		w*(y  ))+i]  = col[i];
				colimagedata[3*(x +		w*(y+1))+i]  = col[i];
				colimagedata[3*(x +		w*(y+2))+i]  = col[i];
				colimagedata[3*(x +		w*(y+3))+i]  = col[i];
				colimagedata[3*(x +		w*(y+4))+i]  = col[i];
				colimagedata[3*(x +		w*(y+5))+i]  = col[i];
				colimagedata[3*(x +		w*(y+9))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y  ))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+5))+i]  = col[i];

				colimagedata[3*(x + 5 + w*(y+1))+i]  = col[i];

				colimagedata[3*(x + 5 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+7))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+8))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+9))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+9))+i]  = col[i];

				colimagedata[3*(x +		w*(y  ))+i]  = col[i];
				colimagedata[3*(x +		w*(y+1))+i]  = col[i];
				colimagedata[3*(x +		w*(y+2))+i]  = col[i];
				colimagedata[3*(x +		w*(y+3))+i]  = col[i];
				colimagedata[3*(x +		w*(y+4))+i]  = col[i];
				colimagedata[3*(x +		w*(y+5))+i]  = col[i];
				colimagedata[3*(x +		w*(y+9))+i]  = col[i];
				}
				break;
			case '6':
				for(i=0;i<3;i++)
				{
				colimagedata[3*(x +	3+	w*(y  ))+i]  = col[i];
				colimagedata[3*(x +	2+	w*(y+1))+i]  = col[i];
				colimagedata[3*(x +	1+	w*(y+2))+i]  = col[i];
				colimagedata[3*(x +		w*(y+3))+i]  = col[i];
				colimagedata[3*(x +		w*(y+4))+i]  = col[i];
				colimagedata[3*(x +		w*(y+5))+i]  = col[i];
				colimagedata[3*(x +		w*(y+6))+i]  = col[i];
				colimagedata[3*(x +		w*(y+7))+i]  = col[i];
				colimagedata[3*(x +		w*(y+8))+i]  = col[i];
				colimagedata[3*(x +		w*(y+9))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+4))+i]  = col[i];

				colimagedata[3*(x + 5 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+7))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+8))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+9))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+9))+i]  = col[i];
				}
				break;
			case '7':
				for(i=0;i<3;i++)
				{
				colimagedata[3*(x +		w*(y  ))+i]  = col[i];
				colimagedata[3*(x +		w*(y+1))+i]  = col[i];
				colimagedata[3*(x +		w*(y+2))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y  ))+i]  = col[i];

				colimagedata[3*(x + 5 + w*(y+1))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+2))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+3))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 1 + w*(y+7))+i]  = col[i];
				colimagedata[3*(x + 0 + w*(y+8))+i]  = col[i];
				colimagedata[3*(x + 0 + w*(y+9))+i]  = col[i];
				}
				break;

			case '8':
				for(i=0;i<3;i++)
				{
				colimagedata[3*(x +		w*(y  ))+i]  = col[i];
				colimagedata[3*(x +		w*(y+1))+i]  = col[i];
				colimagedata[3*(x +		w*(y+2))+i]  = col[i];
				colimagedata[3*(x +		w*(y+3))+i]  = col[i];
				colimagedata[3*(x +		w*(y+4))+i]  = col[i];
				colimagedata[3*(x +		w*(y+5))+i]  = col[i];
				colimagedata[3*(x +		w*(y+6))+i]  = col[i];
				colimagedata[3*(x +		w*(y+7))+i]  = col[i];
				colimagedata[3*(x +		w*(y+8))+i]  = col[i];
				colimagedata[3*(x +		w*(y+9))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y  ))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+4))+i]  = col[i];

				colimagedata[3*(x + 5 + w*(y+1))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+2))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+3))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+7))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+8))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+9))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+9))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+9))+i]  = col[i];
				}
				break;
				case '9':
				for(i=0;i<3;i++)
				{
				colimagedata[3*(x +		w*(y  ))+i]  = col[i];
				colimagedata[3*(x +		w*(y+1))+i]  = col[i];
				colimagedata[3*(x +		w*(y+2))+i]  = col[i];
				colimagedata[3*(x +		w*(y+3))+i]  = col[i];
				colimagedata[3*(x +		w*(y+4))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y  ))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y  ))+i]  = col[i];

				colimagedata[3*(x + 1 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 2 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 3 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 4 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+4))+i]  = col[i];

				colimagedata[3*(x + 5 + w*(y+1))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+2))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+3))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+4))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+5))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+6))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+7))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+8))+i]  = col[i];
				colimagedata[3*(x + 5 + w*(y+9))+i]  = col[i];
				}
				break;
				}//switch
			x += 7;	
		}
		while(buffer[++pos] != '\0');
}


void Plot::zoom(CDC *device, uInt8 *image,int x_pick,int y_pick, int pos)
{
	int i,j,i1,i2,j1,j2,ij,n,m,nm;
	uInt8 *imagezoom =new  uInt8[3*640*480]; 	
	int factor=4;

	i1=x_pick-width /(2*factor)+1;   if(i1<0)        i1=0;
	i2=x_pick+width /(2*factor)-1;   if(i2>width-1)  i2=width-1;
	j1=y_pick-height/(2*factor)+1;  if(j1<0)        j1=0;
	j2=y_pick+height/(2*factor)-1;  if(j2>height-1) j2=height-1;

	n=0;
	for(j=j1;j<j2;j++)
	{
		n++;
		m=0;
		for(i=i1;i<i2;i++)
		{
			m++;
			ij= j*width+i;
			nm=	n*width+m;
			for(int fh=0;fh<factor;fh++)
			{
				for(int fw=0;fw<factor;fw++)
				{
					imagezoom[  3*fw +factor*3*nm+3*fh*width]=image[   3*ij];
					imagezoom[1+3*fw +factor*3*nm+3*fh*width]=image[1 +3*ij];
					imagezoom[2+3*fw +factor*3*nm+3*fh*width]=image[2 +3*ij];
				}
			}
		}
	}
	Plot::Image_col(device, imagezoom, pos);
	delete []imagezoom;
}
