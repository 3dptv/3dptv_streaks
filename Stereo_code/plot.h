#ifndef _PLOT_H_INCLUDED_
#define _PLOT_H_INCLUDED_

	class Plot{
		public:
			static void Points(CDC *,  uInt8[], CamPoint[], int, int, int, int);
			static void Points(CDC *,  uInt8[], Pixel[],    int, int, int, int);
			static void Pixels(CDC *,  uInt8[], CamPoint[], int, int, int, int);
			static void Pixels(CDC *,  uInt8[], Pixel[],    int, int, int, int);
			static void Circles(CDC *, uInt8[], CamPoint[], int, int, int, int);
			static void Circles(CDC *, uInt8[], Pixel[],    int, int, int, int);
			static void Line(CDC*, uInt8[], double, double, double);
			static void Line(CDC*, uInt8[], double, double, double,double, double, double);
			static void Line(CDC*, uInt8[], double, double);
			static void Image_col(CDC*, uInt8*, int);
			static void Image(CDC*, uInt8*, int);
			static void display(int    ,char[] ,CEdit *);
			static void display(int,int  ,char[] ,CEdit *);
			static void display(double ,char[] ,CEdit *);
			static void display(double , double, char[] ,CEdit *);
			static void display(double , double, double, char[] ,CEdit *);
			static void zoom(CDC*, uInt8 *,int,int, int);
			static void Number(CDC *, uInt8[] , int, int, int);
			static void Histogram(CDC*,double*,int,int,int,int);
			static void Get_message(int, CEdit *);
		private:
			static int width, height;
	};

	
#endif /* _PLOT_H_INCLUDED_ */