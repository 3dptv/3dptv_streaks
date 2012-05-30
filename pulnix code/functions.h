#if !defined(_FUNCTIONS_H_)
#define _FUNCTIONS_H_

#include <windows.h>
#include "niimaq.h"
#include <mmsystem.h>
#include "structures.h"

// Callbacks
extern DWORD ImaqThread(ThreadCtrl_t *Data);

void init_window(Frame *);
int set_memory(int, Frame);
void clear_memory(int mode);
void display(int number, char text[], CEdit *status); 

void image_tresh(uInt32 img_data_addr, uInt32 *img_data_addr_new);

#endif /* _FUNCTIONS_H_ */




