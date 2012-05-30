#ifndef _IMAGE_H_INCLUDED_
#define _IMAGE_PROC_H_INCLUDED_
	
	class Image{
		public:
			static int Read(char *, uInt8 *);
			static int Write(char *, uInt8 *);
			static void Process(CDC *, uInt8 *, uInt8 *, int, int);
			static void binary_median_sym(uInt8 *);
			static void binary_median_asym(uInt8 *);
			static void binary_median(uInt8 *, uInt8 *);
			static void row_correction(uInt8 *image);
		private:
	};


#endif /* _IMAGE_H_INCLUDED_ */