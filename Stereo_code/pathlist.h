#ifndef _PATH_LIST_H_INCLUDED_
#define _PATH_LIST_H_INCLUDED_

	//this as a class to operate a list of pointers (element)
	//the pointer list is connected by pointers to the next 
	//element (next)

	class Path;

	class PathListElem{
		public:
			Path			*element;
			PathListElem	*next;
	};

	//this class can create new elements     create  (memory is allocated)
	//and can delete them                    del     (memory is freed)
	//it can also add existing elements      add     (no memory is allocated)
	//or remove them                         rem     (no memory is freed)
	class PathList{
		public:
			PathList() : number(0), first_path(0), last_path(0){}
			~PathList()   {destroy();}
			PathListElem    *operator[](int);
			PathListElem	*first_path;
			PathListElem	*last_path;
			Path*			create();
			bool			del(int);
			void			add(Path*);
			bool			rem(int);
			void			destroy();
			int				number;
//		private:
//			int				number;
	};

#endif /* _PATH_H_INCLUDED_ */