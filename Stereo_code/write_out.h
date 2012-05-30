#ifndef _WRITE_OUT_H_INCLUDED_
#define _WRITE_OUT_H_INCLUDED_

	class Path;

	class Write_out{
		public:
			static void Pixels(FILE*,CamPoint[], int);
			static void Pixels(FILE*,Pixel[],int);
			static void W_path(FILE*,Path*);
	};

#endif /* _WRITE_OUT_H_INCLUDED_ */