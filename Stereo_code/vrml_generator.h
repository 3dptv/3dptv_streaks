#ifndef _VRML_GENERATOR_H_INCLUDED_
#define _VRML_GENERATOR_H_INCLUDED_

	class VRML{
		public:
			static int vrml_points_gen(char *, CProgressCtrl*, CDC* device, CEdit *m_status);
			static void Draw_Sphere(FILE *file,double radius, double x, double y, double z, double r, double g, double b);
			static void Draw_Cylinder(FILE *file, double radius, double x1, double y1, double z1, 
						                double x2, double y2, double z2, double r, double g, double b);
			static void Draw_Cylinder(FILE *file, double radius, double x1, double y1, double z1, 
						                double x2, double y2, double z2, double length, double r, double g, double b);
			static void Draw_Cylinder_dir(FILE *file, double radius, double x1, double y1, double z1, 
						                double dir_x, double dir_y, double dir_z, double length, 
										double r, double g, double b, double transp);
			static void Draw_Plane(FILE *file, double px, double py, double pz, double dir_x, double dir_y, double dir_z);
	};

#endif /* _VRML_GENERATOR_H_INCLUDED_ */