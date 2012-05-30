#include "stdafx.h"
#include "serial_port.h"


HANDLE Port::hPort;
char Port::lpszPortName[]="COM1";
int Port::port_open=0;

int Port::open(){
    // Open the serial port.
    hPort = CreateFile (lpszPortName, // Pointer to the name of the port
                      GENERIC_READ | GENERIC_WRITE,  // Access (read-write) mode
                      0,            // Share mode
                      NULL,         // Pointer to the security attribute
                      OPEN_EXISTING,// How to open the serial port
                      0,            // Port attributes
                      NULL);        // Handle to port with attribute
                                    // to copy
	if(hPort == INVALID_HANDLE_VALUE)
		return 0;
	else 
		return 1;
}



void Port::close()
{
	CloseHandle(hPort);
}
 



bool Port::set(){
    DCB PortDCB;
	DWORD dwError;

	// Initialize the DCBlength member. 
	PortDCB.DCBlength = sizeof (DCB); 

	// Get the default port setting information.
	GetCommState (hPort, &PortDCB);

	// Change the DCB structure settings.
	PortDCB.BaudRate = 9600;					// Current baud 
//	PortDCB.fBinary = TRUE;						// Binary mode; no EOF check 
//	PortDCB.fParity = TRUE;						// Enable parity checking 
//	PortDCB.fOutxCtsFlow = FALSE;				// No CTS output flow control 
//	PortDCB.fOutxDsrFlow = FALSE;				// No DSR output flow control 
//	PortDCB.fDtrControl = DTR_CONTROL_DISABLE;	// DTR flow control type 
								  
//	PortDCB.fDsrSensitivity = FALSE;			// DSR sensitivity 
//	PortDCB.fTXContinueOnXoff = TRUE;			// XOFF continues Tx 
//	PortDCB.fOutX = TRUE;						// No XON/XOFF out flow control 
//	PortDCB.fInX = TRUE;						// No XON/XOFF in flow control 
//	PortDCB.fErrorChar = FALSE;					// Disable error replacement 
//	PortDCB.fNull = FALSE;						// Disable null stripping 
//	PortDCB.fRtsControl = RTS_CONTROL_DISABLE;  // RTS flow control 
//	PortDCB.fAbortOnError = FALSE;				// Do not abort reads/writes on error
	PortDCB.ByteSize = 8;						// Number of bits/byte, 4-8 
	PortDCB.Parity = NOPARITY;					// 0-4=no,odd,even,mark,space 
	PortDCB.StopBits = ONESTOPBIT;				// 0,1,2 = 1, 1.5, 2 Defaul ONESTOPBIT

	if (!SetCommState (hPort, &PortDCB))
	{
	  dwError = GetLastError ();
	  return FALSE;
	}
	return TRUE;
}

bool Port::set_timeout(){
	COMMTIMEOUTS CommTimeouts;
	DWORD dwError;

	// Retrieve the time-out parameters for all read and write operations 
	GetCommTimeouts (hPort, &CommTimeouts);
	// Change the COMMTIMEOUTS structure settings; units in ms
	CommTimeouts.ReadIntervalTimeout = -1;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;  
	CommTimeouts.ReadTotalTimeoutConstant = 0;      
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;  
	CommTimeouts.WriteTotalTimeoutConstant =   5000;    
 
	if (!SetCommTimeouts (hPort, &CommTimeouts))
	{  
	  // Could not create the read thread.
	  dwError = GetLastError ();
	  return FALSE;
	}
	return TRUE;
}



int Port::write(char* c, int nr)
{
	DWORD dwNumBytesWritten;

	BOOL res=WriteFile (hPort,   // Port handle
           c,				     // Pointer to the data to write 
           nr,                   // Number of bytes to write
           &dwNumBytesWritten,   // Pointer to the number of bytes written                               // 
           NULL                  // Must be NULL for Windows CE
	);
	return dwNumBytesWritten;
}



char Port::read(){
 

	DWORD dwCommEvent;
	DWORD dwRead;
	char  chRead=0;

	if (!SetCommMask(hPort, EV_RXCHAR||EV_TXEMPTY||EV_CTS||EV_DSR||EV_RLSD||EV_ERR||EV_RING))
	{// Error setting communications event mask
	} 

	while(chRead != 0x0D)
	{
	   if (WaitCommEvent(hPort, &dwCommEvent, NULL)) {
		  do {
			 if (ReadFile(hPort, &chRead, 1, &dwRead, NULL))
			 {

			 }
			 else
			 {	
				break;
			 }
		  } while (dwRead);
	   }
	   else
		  // Error in WaitCommEvent
		  break;
	}
	return chRead;
}
