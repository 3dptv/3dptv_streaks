#include "stdafx.h"
#include "niimaq.h"
#include "parameters.h"
#include "grabber.h"

extern SESSION_ID   Sid;
extern INTERFACE_ID Iid;
extern BUFLIST_ID   Bid;


Grabber::Grabber()
{
	armed			=FALSE;
	images_running	=FALSE;
	on				=FALSE;
}

Grabber::~Grabber()
{
	imgSessionTriggerClear(Sid);
}

void Grabber::drive_signal()
{
	PULSE_ID plsID=0;

	int signal_local;

	switch (signal){
		case 0:  signal_local=IMG_TRIG_DRIVE_FRAME_DONE;
			break;
		case 1:  signal_local=IMG_TRIG_DRIVE_FRAME_START;
			break;
		case 2:  signal_local=IMG_TRIG_DRIVE_HSYNC;
			break;
		case 3:  signal_local=IMG_TRIG_DRIVE_VSYNC;
			break;
	}
	switch (line){
		case 0:
			imgSessionTriggerDrive(Sid, IMG_EXT_RTSI0, IMG_TRIG_POLAR_ACTIVEH,signal_local);
			break;
		case 1:
			imgSessionTriggerDrive(Sid, IMG_EXT_RTSI1, IMG_TRIG_POLAR_ACTIVEH,signal_local);
			break;
		case 2:
			imgSessionTriggerDrive(Sid, IMG_EXT_RTSI2, IMG_TRIG_POLAR_ACTIVEH,signal_local);
			break;
 		case 3:
			imgSessionTriggerDrive(Sid, IMG_EXT_RTSI3, IMG_TRIG_POLAR_ACTIVEH,signal_local);
			break;
		case 4:
			imgSessionTriggerDrive(Sid, IMG_EXT_RTSI4, IMG_TRIG_POLAR_ACTIVEH,signal_local);
			break;
		case 5:
			imgSessionTriggerDrive(Sid, IMG_EXT_RTSI5, IMG_TRIG_POLAR_ACTIVEH,signal_local);
			break;
		case 6:
			imgSessionTriggerDrive(Sid, IMG_EXT_RTSI6, IMG_TRIG_POLAR_ACTIVEH,signal_local);
			break;
		case 7:
			imgSessionTriggerDrive(Sid, IMG_EXT_TRIG0, IMG_TRIG_POLAR_ACTIVEH,signal_local);
			break;
		case 8:
			imgSessionTriggerDrive(Sid, IMG_EXT_TRIG1, IMG_TRIG_POLAR_ACTIVEH,signal_local);
			break;
		case 9:
			imgSessionTriggerDrive(Sid, IMG_EXT_TRIG2, IMG_TRIG_POLAR_ACTIVEH,signal_local);
			break;
		case 10:
			imgSessionTriggerDrive(Sid, IMG_EXT_TRIG3, IMG_TRIG_POLAR_ACTIVEH,signal_local);
			break;
		default: ;
	}
//PULSE_TIMEBASE_PIXELCLK
								
//	imgPulseCreate(PULSE_TIMEBASE_50MHZ, 15, 15, IMG_IMMEDIATE,
//		                        IMG_TRIG_POLAR_ACTIVEH, IMG_EXT_RTSI6,
//								IMG_TRIG_POLAR_ACTIVEH,
//								PULSE_MODE_TRAIN, &plsID);
//
//	imgPulseStart(plsID, Sid);
}