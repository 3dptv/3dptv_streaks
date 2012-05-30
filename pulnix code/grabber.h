#ifndef _GRABBER_H_
#define _GRABBER_H_

	class Grabber{
		public:
			Grabber();
			~Grabber();
			BOOL armed;
			BOOL on;
			BOOL images_running;
			int line;
			int signal;
			int locked_line;
			int locked_signal;
			void drive_signal();
	};



#endif /* _GRABBER_H_ */