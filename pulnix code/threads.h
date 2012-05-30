#if !defined(_THREADS_H_)
#define _THREADS_H_

#include <windows.h>
#include "niimaq.h"

// Data types
typedef struct ThreadCtrl {
	int    stop_recording;
	int    is_running;
	CEdit *frame_rate;
	Int32  AcqWinTop;
	Int32  AcqWinLeft;
    Int32  AcqWinWidth;
    Int32  AcqWinHeight;
	CDC   *ImaqSmplDC;
} ThreadCtrl_t;

// Callbacks
extern DWORD ImaqThread(ThreadCtrl_t *Data);
extern void CALLBACK OnTimer(UINT uID,	UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

#endif /* _THREADS_H_ */