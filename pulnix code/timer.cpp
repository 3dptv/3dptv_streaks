#include "stdafx.h"
#include "nidaq.h"
#include "nidaqcns.h"
#include "timer.h"

short dev_num_code;

//********************************The cables:********************************************
//PFI_32 (ND_COUNTER_1) is connected with the HD in signal of the camera
//PFI_28 (ND_COUNTER_2) 
//PFI_24 (ND_COUNTER_3) is connected with the VInit resp. VD in signal of the camera
//PFI_20 (ND_COUNTER_0) is used for as trigger out line to the frame grabbers
//***************************************************************************************
//ND_COUNT_1 is for the low part of the pulse
//ND_COUNT_2 is for the high part of the pulse
//5E-8s * 656 * 254 = 8.331 ms
//***************************************************************************************
//ND_PFI_26 is gate of counter 3
//ND_PFI_30 is gate of counter 2
//ND_PFI_34 is gate of counter 1
//ND_PFI_38 is gate of counter 0
//***************************************************************************************

Timer::Timer()
{
	initialized=FALSE;
	do_trigger_on_trigger=FALSE;
}


void Timer::initialize(){
	int err;
	int rate1;
//Signal supplied by the timer board (time base=20MHz; 5E-8s)
//***HD signal 30,488 kHz ****************************************************************
//***Channel 1  (PFI_32,  Pin 5)  Green Cable
	rate1=656;
	err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_1, ND_RESET); 
	err=GPCTR_Set_Application (DEVICE_NUMBER, ND_COUNTER_1, ND_PULSE_TRAIN_GNR);
    err=GPCTR_Change_Parameter(DEVICE_NUMBER, ND_COUNTER_1, ND_COUNT_1, rate1-500); 
    err=GPCTR_Change_Parameter(DEVICE_NUMBER, ND_COUNTER_1, ND_COUNT_2,		  500);
	err=Select_Signal         (DEVICE_NUMBER, ND_PFI_32   , ND_GPCTR1_OUTPUT, ND_LOW_TO_HIGH);
	err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_1, ND_PREPARE);
//***VD signal 120 Hz ************************************************************************
//***Channel 3  (PFI_24,  Pin 29)  Pink Cable
	err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_3, ND_RESET); 
	err=GPCTR_Set_Application (DEVICE_NUMBER, ND_COUNTER_3, ND_PULSE_TRAIN_GNR);
    err=GPCTR_Change_Parameter(DEVICE_NUMBER, ND_COUNTER_3, ND_COUNT_1, rate1*254-100000); 
    err=GPCTR_Change_Parameter(DEVICE_NUMBER, ND_COUNTER_3, ND_COUNT_2,           100000);  
	err=Select_Signal         (DEVICE_NUMBER, ND_PFI_24,    ND_GPCTR3_OUTPUT, ND_HIGH_TO_LOW);
	err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_3, ND_PREPARE);
//***  Start  ************************************************************************
	err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_1, ND_ARM);
	err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_3, ND_ARM);

	initialized=TRUE;
}

void Timer::run(int counter)
{
	if(counter ==0)
		GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_0, ND_ARM);
	if(counter ==1)
		GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_1, ND_ARM);
	if(counter ==2)
		GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_2, ND_ARM);
	if(counter ==3)
		GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_3, ND_ARM);
}
	

void Timer::stop()
{
	int err=GPCTR_Control			(DEVICE_NUMBER, ND_COUNTER_0,   ND_RESET);
		err=GPCTR_Control			(DEVICE_NUMBER, ND_COUNTER_1,	ND_RESET);
		err=GPCTR_Control			(DEVICE_NUMBER, ND_COUNTER_3,   ND_RESET);
		initialized=FALSE;
}


void Timer::change(int rate, double *exp_time, int max_exp){


	int high, low;
	int low_max, high_min;
	int rate1=656;


	//calc max exposure time 
	if(max_exp)
	{
		high = 660;
		low  = (int)(1.0/(float)rate/5e-8) - high;
		*exp_time=1000.0*(double)low*5e-8;
	}
	else 
	{ 
		high_min  = 660;
		low_max  = (int)(1.0/(float)rate/5e-8) - high_min;
		low = (int)(*exp_time/1000.0 / 5e-8);
		if(low_max < low)
		{
			low = low_max;
			*exp_time=1000.0*(double)low*5e-8;
		}
		high= int(1.0/float(rate) / 5e-8) - low;
		if(high <= 660)
			high = 660;
	}

	if(initialized==TRUE)
	{	
		//VDinit signal
		int err=Select_Signal         (DEVICE_NUMBER, ND_PFI_24,    ND_GPCTR3_OUTPUT, ND_HIGH_TO_LOW);
			err=GPCTR_Change_Parameter(DEVICE_NUMBER, ND_COUNTER_3, ND_COUNT_1, low); 
			err=GPCTR_Change_Parameter(DEVICE_NUMBER, ND_COUNTER_3, ND_COUNT_2,high); 
			err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_3, ND_SWITCH_CYCLE);
	}
}


int Timer::laser_trigger_on(int delta)
{
//***Trigger signal 120Hz but with a delay delta *******************************************
//***Channel 2  (PFI_28,  Pin 32)  White Cable
	int rate1=656*2;
//convert ms in base units of timer card
	int delay=(int)((float)delta/0.00005);

	int err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_2, ND_RESET); 
		err=GPCTR_Set_Application (DEVICE_NUMBER, ND_COUNTER_2, ND_PULSE_TRAIN_GNR);
		err=GPCTR_Change_Parameter(DEVICE_NUMBER, ND_COUNTER_2, ND_COUNT_1, rate1*254-rate1*254+delay);
		err=GPCTR_Change_Parameter(DEVICE_NUMBER, ND_COUNTER_2, ND_COUNT_2,			  rate1*254-delay);
		err=Select_Signal         (DEVICE_NUMBER, ND_RTSI_5   , ND_GPCTR2_OUTPUT, ND_DONT_CARE);
		err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_2, ND_PROGRAM);

	return err;
}

int Timer::laser_trigger_off()
{
	int err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_2, ND_RESET);
	return err;
}

void Timer::trigger(){
	int err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_0, ND_RESET); 
		err=GPCTR_Set_Application (DEVICE_NUMBER, ND_COUNTER_0, ND_SINGLE_PULSE_GNR);
		err=GPCTR_Change_Parameter(DEVICE_NUMBER, ND_COUNTER_0, ND_COUNT_1, 500); 
		err=GPCTR_Change_Parameter(DEVICE_NUMBER, ND_COUNTER_0, ND_COUNT_2, 500);
		err=Select_Signal         (DEVICE_NUMBER, ND_RTSI_6   , ND_GPCTR0_OUTPUT, ND_DONT_CARE);
		err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_0, ND_PROGRAM);
}




void Timer::trigger_on_trigger()
{
	//ND_RETRIG_PULSE_GNR: Generation of a retriggerable delayed pulse after
	//EACH transition on the GATE INPUT.
	//For Counter 0 the gate to be used is ND_PFI_38

	int err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_0, ND_RESET); 
		//err=GPCTR_Set_Application (DEVICE_NUMBER, ND_COUNTER_0, ND_RETRIG_PULSE_GNR);
		err=GPCTR_Set_Application (DEVICE_NUMBER, ND_COUNTER_0, ND_SINGLE_TRIG_PULSE_GNR);
		//Set the shape of the trigger signal
		err=GPCTR_Change_Parameter(DEVICE_NUMBER, ND_COUNTER_0, ND_COUNT_1, 500); 
		err=GPCTR_Change_Parameter(DEVICE_NUMBER, ND_COUNTER_0, ND_COUNT_2, 2000);
		//select the output port
		err=Select_Signal         (DEVICE_NUMBER, ND_RTSI_6   , ND_GPCTR0_OUTPUT, ND_LOW_TO_HIGH);
		err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_0, ND_PROGRAM);
}


void Timer::reset_trigger_on_trigger()
{
	int err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_0, ND_RESET);
		err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_1, ND_RESET);
		err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_2, ND_RESET);
		err=GPCTR_Control         (DEVICE_NUMBER, ND_COUNTER_3, ND_RESET);	
}









         