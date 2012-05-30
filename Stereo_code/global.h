#ifndef _GLOBAL_H_INCLUDED_
#define _GLOBAL_H_INCLUDED_


	#include "path.h"

	const int MAX_G_POINT=MAX_H_POINT;

	typedef struct{
		double X;
		double Y;
		double Z;
		double r;
	} GlobalPoint;

	class Global{
		public:
			Global(){nr_p=0;}
			int				nr_p;
			GlobalPoint		point[MAX_G_POINT];
	};


#endif /* _GLOBAL_H_INCLUDED_ */