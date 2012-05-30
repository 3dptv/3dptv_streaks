#ifndef _TIMER_H_
#define _TIMER_H_

#define DEVICE_NUMBER 1

	class Timer{
		public:
			Timer();
			void initialize();
			void change(int, double*, int);
			void run(int counter);
			void stop();
			void trigger();
			void trigger_on_trigger();
			int  laser_trigger_on(int);
			int  laser_trigger_off();
			int  exist;
			bool do_trigger_on_trigger;
			void reset_trigger_on_trigger();
		private:
			bool initialized;
			int  running;
	};

#endif  /*_TIMER_H_*/