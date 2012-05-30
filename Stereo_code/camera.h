#ifndef _CAMERA_H_INCLUDED_
#define _CAMERA_H_INCLUDED_

	#include "global.h"

	typedef struct {
		double X_0;
		double Y_0;
		double Z_0;
		double omega;
		double phi;
		double kappa;
	} Extrinsic_par;

	typedef struct {
		double focus;
		double x_h;
		double y_h;
		double s_x;
		double theta;
		double k1;
		double k2;
		double p1;
		double p2;
	} Intrinsic_par;

	typedef struct {
		int    width;
		int    height;
		double pix_w;
		double pix_h;
	} CCD_par;


	typedef struct{
	double    a;
	double    b;
	double    c;
	} EpiPar;

	class Spline;

	class Camera{
		public:
			Camera();
			~Camera();
			bool			init;
			CCD_par			ccd;
			T_vec			t;
		    Matrix			R;
			Intrinsic_par	in;
	     	//the camera matrix of a ideal pin hole camera with f=1
			double			P[12];   
			void Set_position(T_vec t_new_origin);
			void Set_orientation(double phi_new, double omega_new, double kappa_new);
			void Trans_angles(T_vec direction);
			void Read_cam_par(char*);
			bool Read_ccd_par(char*);
			void Get_transf_matrix();
			void Get_epi(EpiPar *, CamPoint);
			bool Normalize_look_up(const Pixel p, CamPoint* hp);
			bool Normalize_look_up(const CamPoint p, CamPoint* hp);
			void Normalize_look_up(Path*);
			void Normalize_look_up(Path_seg*);
			bool Normalize(const CamPoint p, CamPoint *hp);
			bool Normalize(const Pixel p, CamPoint *hp);
			CamPoint Transform_nor2img(CamPoint p);
			void Transform_nor2img(CamPoint*, CamPoint*, int);
			CamPoint Project_point(GlobalPoint P);
			CamPoint Project_point_norm(GlobalPoint P);
			int Load_look_up(char *, CProgressCtrl *, int);
			void Get_essential(Matrix,T_vec);
			void Get_camera_matrix();
			void Plot_epi(CDC *, uInt8 [], double, double, double);
			static void Get_triangulation_matrix(Camera, Camera);
			static void Get_3D_point(GlobalPoint*, CamPoint*, CamPoint*, int);
			static bool Get_3D_point(GlobalPoint*, CamPoint*, CamPoint*, double);
			static bool Get_3D_point_hom(GlobalPoint*, double*, double*, CamPoint*, CamPoint*, double);
			static bool Get_3D_point_hom(GlobalPoint *Point, const double *P1, const double *P2, const CamPoint hp1, const CamPoint hp2);
			static bool Get_3D_point_non_lin(GlobalPoint *Point, const Camera c1, const Camera c2, CamPoint *hp1, CamPoint *hp2, double r_par);
			static bool Get_3D_point_non_lin(GlobalPoint *Point, const Camera c1, const Camera c2, const CamPoint hp1, const CamPoint hp2);
		private:
			Extrinsic_par	ex;
			//the essential matrix describing the projection from ..
			Matrix			E;
			//matrix used for the triangulation
			//it contains only extrinsic paprameters from BOTH cameras
			static double   K[24];  
			//the look-up table containing the corrected pixel coordinates 
			//the correction includes skew/stretching/lens distortion and principal point shift
			Vec2			*table;
	};


	
#endif /* _CAMERA_H_INCLUDED_ */