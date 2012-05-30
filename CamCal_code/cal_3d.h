#ifndef _CAL_3D_H
#define _CAL_3D_H

	void DrawScreen(CDC *);
	void show_img(CDC *);
	void get_points(CDC *, int);
	void Get_stereo_matrix();
	int get_3d(CDC *,int,int, double, CProgressCtrl*);
	int get_3d_lin(CDC *, int,int,double);
	void Normalize_coord(CamPoint [], CamPoint [],Intrinsic_par,CCD_par,int);
	void Normalize_look_up(CalPoint[], double, int, Vec2*);
	void Normalize_coord_lin(CamPoint [], CamPoint [],Intrinsic_par,CCD_par, int);
	void Get_centroid(CalPoint[], CamPoint[], int);
	void Get_epi(EpiPar [], CamPoint [], int, int);
	void Get_epi_pal(EpiPar [], EpiPar [], double, int);
	void Reduce_epi_z(CamPoint [], CamPoint [], CamPoint [], CamPoint [], CamPoint [], 
		int, int, int);
	void Set_epi_z(CamPoint [], CamPoint [], CamPoint [], CamPoint [], CamPoint [], 
		int, int, int);
	int  Match_points_epi(CamPoint [], CamPoint [], CamPoint *, CamPoint *, 
					 CamPoint [], CamPoint [], EpiPar [],  int, int, double);
	int Match_points_epi_no_lim(CamPoint *point1, CamPoint *point2, 
					 CamPoint hpoint1[], CamPoint hpoint2[], EpiPar epi_par[], 
					 int max1, int max2, double tolerance);
	void Solve_3D(GlobalPoint*, int);
	void fcn(int , int , double[] , double[]);
	void Solve_3D_lin(GlobalPoint*, P_matrix, P_matrix, CamPoint *, CamPoint *,int);
	void Solve_3D(GlobalPoint*, CamPoint *, CamPoint *,int);
	void Get_3D_point_tria(GlobalPoint *global_point, CamPoint *hp1, CamPoint *hp2, int point_nr);
	void Get_3D_point_norm(GlobalPoint *global_point, CamPoint *hp1, CamPoint *hp2, int point_nr);
	void Get_transf_mat();
	void Get_cam_transf_matrix();
	int Get_accuracy(double[], double[], double[], double[], int[]);
	double Rot_mat_elem(double omega, double phi, double kappa, int row, int col);

#endif /* CAL_3D_H */