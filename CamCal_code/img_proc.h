#ifndef _IMG_PROC_H_INCLUDED_
#define _IMG_PROC_H_INCLUDED_

	//void display_img(uInt8 *image);
	void histo(uInt8 *, double *);
	void Image_row_cor(uInt8 *);
	void Background_leveling(uInt8 *);
	void Background_leveling_min(uInt8 *);
	void Box_filter(uInt8 *image_in, uInt8 *image_out);
	void Gauss_filter(uInt8 *, uInt8 *);
	void Hist_equalization(uInt8 *image);
	void Image_subtract(uInt8 *image1, uInt8 *image2);
	int Extract_streaks_roes (CamPoint[], uInt8 *, int, int min_pix, int max_pix);
	int Extract_streaks_roes3D (CalPoint[], CamPoint[], uInt8 *, int);

#endif /* _IMG_PROC_H_INCLUDED_ */