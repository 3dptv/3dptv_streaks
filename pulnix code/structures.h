#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_
// Data types
typedef struct ThreadCtrl {
	int    stop_recording;
	int    is_running;
	CEdit *frame_rate;
	CEdit *status;
	CEdit *nr_of_img;
	Int32  AcqWinTop;
	Int32  AcqWinLeft;
    Int32  AcqWinWidth;
    Int32  AcqWinHeight;
	CDC   *ImaqSmplDC;
} ThreadCtrl_t;

struct Frame{
	Int32		 AcqWinTop;
	Int32        AcqWinLeft;
	Int32        AcqWinWidth;
	Int32        AcqWinHeight;
};

#endif