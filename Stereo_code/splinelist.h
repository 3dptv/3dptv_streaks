#ifndef _SPLINE_LIST_H_INCLUDED_
#define _SPLINE_LIST_H_INCLUDED_

	//this as a class to operate a list of pointers (element)
	//the pointer list is connected by pointers to the next 
	//element (next)

	class Spline;

	class SplineListElem{
		public:
			Spline			*element;
			SplineListElem	*next;
	};

	//this class can create new elements     create  (memory is allocated)
	//and can delete them                    del     (memory is freed)
	//it can also add existing elements      add     (no memory is allocated)
	//or remove them                         rem     (no memory is freed)
	class SplineList{
		public:
			SplineList() :  number(0), first_spline(0), last_spline(0){}
			~SplineList()   {destroy();}
			SplineListElem  *operator[](int);
			SplineListElem	*first_spline;
			SplineListElem	*last_spline;
			Spline*			create();
			bool			del(int);
			void			add(Spline*);
			bool			rem(int);
			void			destroy();
			int				number;
//		private:
//			int				number;
	};

#endif /* _PATH_H_INCLUDED_ */