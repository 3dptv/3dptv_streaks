#ifndef _IO_FUNCTIONS_H_INCLUDED_
#define _IO_FUNCTIONS_H_INCLUDED_

	int Read_image(char *, uInt8 *, int ,int);
	void Read_cam_param(Extrinsic_par *, Intrinsic_par *, char *);
	void Read_P_matrix(P_matrix *, char *);
	void Get_parameters(P_matrix ,Extrinsic_par *, Intrinsic_par *);
	void Get_parameters3(P_matrix ,Extrinsic_par *, Intrinsic_par *);
	void display(int, char [], CEdit *);
	void display_double(double, char [], CEdit *);
	int Rd_matched_points();
	int Write_3D_points(GlobalPoint*, int);
	int Read_3D_points(GlobalPoint*, char*);
    void Read_table(char*, Vec2*, CProgressCtrl*);

#endif /* _IO_FUNCTIONS_H_INCLUDED_ */