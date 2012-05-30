#if !defined(_SERIAL_PORT_H_)
#define _SERIAL_PORT_H_

	class Port{
	public:
		static int open();
		static void close();
		static bool set();
		static bool set_timeout();
		static int  write(char*, int);
		static char read();
		static int  port_open;
		static HANDLE hPort;
		static char lpszPortName[];
	};


#endif /* _SERIAL_PORT_ */