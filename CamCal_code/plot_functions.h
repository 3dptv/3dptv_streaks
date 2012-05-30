#ifndef _PLOT_FUNCTIONS_H_INCLUDED_
#define _PLOT_FUNCTIONS_H_INCLUDED_

	void Plot_image(CDC *, uInt8 *, int, int, int, int);
	void Plot_histo(CDC *, double* ,int, int, int, int);
	void Plot_circle(CDC *, uInt8[], double);
	void Plot_pixels(CDC *, uInt8[], CamPoint[], int, int, int, int, int ,int, int);
	void Plot_points(CDC *, uInt8[], CamPoint[], int, int, int, int, int, int, int);
	void Plot_points3D(CDC *, uInt8[], CamPoint[], int, int, int, int, int, int, int);
	void Plot_error_cross(CDC *, uInt8[], int, int, int , int);
	void Plot_number(CDC *, uInt8[], int, int, int , int , int ,int);
	void Plot_epi(CDC *, uInt8[], EpiPar[], int, int, int, CamPoint[], CamPoint[], int);
	void Plot_epi_no_lim(CDC *, uInt8[], EpiPar[], int, int, int, int);

#endif /* _PLOT_FUNCTIONS_H_INCLUDED_ */