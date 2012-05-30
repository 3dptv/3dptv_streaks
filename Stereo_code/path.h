#ifndef _PATH_H_INCLUDED_
#define _PATH_H_INCLUDED_

	#include "niimaq.h"

	const int MAX_PATHS=200;
	const int MAX_PATH_SEG=40;
	const int MAX_SEG_PIX=300;
	const int MAX_H_POINT=300;
	const int MAX_END_P=10;


	class Path_seg{
		public:
			Path_seg();
			~Path_seg();
			Pixel*        operator=(const Pixel&);
			CamPoint*     operator=(const CamPoint&);
			int           p_nr;
			bool		  circular;
			Pixel         pixel[MAX_SEG_PIX];
			CamPoint	  centroid;
			int           end_point_index[MAX_END_P];
			CamPoint      hpixel[MAX_SEG_PIX];
	} ;

	class Spline;
	class PathList;

	class Path{
		public:
			Path();
			~Path();
			//Path::operator=(const CamPoint*);
			bool	  finished;
			bool	  matched;
			int		  t_start;
			int		  cor_indicator;
			int		  nr_seg;
			int		  r_corr;
			int		  i_match[4];
			Path	 *correspondence[4];
			Spline	 *cor_spline;
			Path_seg  path_seg[MAX_PATH_SEG];	
			CamPoint  hpoint[MAX_H_POINT];
			int       nr_p;
			void Init();
			void Get_r(int, int);
			int  Get_contour(Path_seg[], uInt8 *);
			int  Get_contour_frame(Path_seg[], uInt8 *);
			int  Get_contour_line(PathList, uInt8 *, int);
			int  Get_contour_point(Path_seg*, uInt8 *, int, int);
			//int  Get_conture_curvature(Path_seg*, const int);
			int  Get_conture_curvature_a(Path_seg *path_seg);
			int  Get_property(Path_seg*);
			int  Get_hproperty();
	};

	
#endif /* _PATH_H_INCLUDED_ */