#include "stdafx.h"
#include "niimaq.h"
#include "parameters.h"
#include "functions.h"
#include "pulnix_ni.h"
#include "pulnix_niDlg.h"


extern SESSION_ID   Sid;
extern INTERFACE_ID Iid;
extern BUFLIST_ID   Bid;

extern int          num_seq_buffer;
extern Int8         **ImagesRingBuffer;
extern Int8         **ImagesSeqBuffer;
extern Int8         **ImageBuffer;
extern Int8          *ImageCalcBuffer;
extern int           *ImageCalcBuffer2;
extern UINT           ImaqTimerId;
extern int			  last,sub_ref,integ,avrg,thresh,inv;
extern int            thresh_value;


void init_window(Frame *frame){

		imgSetAttribute (Sid, IMG_ATTR_ACQWINDOW_TOP,   3);
		imgSetAttribute (Sid, IMG_ATTR_ACQWINDOW_LEFT,  6);
		imgSetAttribute (Sid, IMG_ATTR_ACQWINDOW_WIDTH, 647);
		imgSetAttribute (Sid, IMG_ATTR_ACQWINDOW_HEIGHT,485);

		frame->AcqWinHeight=480;
		frame->AcqWinWidth= 640;
		frame->AcqWinTop=0;
		frame->AcqWinLeft=0;

		imgSetAttribute (Sid, IMG_ATTR_ROI_TOP,   frame->AcqWinTop);
		imgSetAttribute (Sid, IMG_ATTR_ROI_LEFT,  frame->AcqWinLeft);
		imgSetAttribute (Sid, IMG_ATTR_ROI_WIDTH, frame->AcqWinWidth);
		imgSetAttribute (Sid, IMG_ATTR_ROI_HEIGHT,frame->AcqWinHeight);

		imgSetAttribute (Sid, IMG_ATTR_ROWPIXELS, frame->AcqWinWidth); 
};


int set_memory(int mode, Frame frame){
	int             i,bufSize;
    unsigned int    bufCmd;
	int err;
    unsigned int skippedBuffers[3300] = {0}; 
	bufSize = frame.AcqWinWidth * frame.AcqWinHeight;
	
	
	if(mode==CTR_ONE_SHOT)                  //one shot
	{								        
		imgCreateBufList(1, &Bid);

//		imgCreateBuffer(Sid, FALSE, bufSize, &ImagesSeqBuffer[0]);
//		imgSetBufferElement(Bid, 0, IMG_BUFF_ADDRESS, (uInt32)ImagesSeqBuffer[0]);
		imgCreateBuffer(Sid, FALSE, bufSize, &ImageBuffer);
		imgSetBufferElement(Bid, 0, IMG_BUFF_ADDRESS, (uInt32)ImageBuffer);

		imgSetBufferElement(Bid, 0, IMG_BUFF_SIZE, bufSize);
		imgSetBufferElement(Bid, 0, IMG_BUFF_COMMAND, IMG_CMD_STOP);
	}

	else if(mode==CTR_SEQUENCE)				//sequence
	{                  		
		if((err=imgCreateBufList(num_seq_buffer, &Bid)) !=0)
			return err;

		for (i = 0; i < num_seq_buffer; i++)
		{
			ImagesSeqBuffer[i]= NULL;
			if((err=imgCreateBuffer(Sid, IMG_HOST_FRAME, bufSize, &ImagesSeqBuffer[i]))!=0)
				return err;
			if (ImagesSeqBuffer[i]==NULL) return err;
			if((err=imgSetBufferElement(Bid, i, IMG_BUFF_ADDRESS, (uInt32)ImagesSeqBuffer[i]))!=0)
				return err;
			if((err=imgSetBufferElement(Bid, i, IMG_BUFF_SIZE, bufSize))!=0)
				return err;
			bufCmd = (i == (num_seq_buffer - 1)) ? IMG_CMD_STOP : IMG_CMD_NEXT;
			if((err=imgSetBufferElement(Bid, i, IMG_BUFF_COMMAND, bufCmd))!=0)
				return err;
			if((err=imgSetBufferElement(Bid, i, IMG_BUFF_SKIPCOUNT, skippedBuffers[i]))!=0)
				return err;
		}
	}

	else if(mode==CTR_RING)				     //ring
	{                       

		imgCreateBufList(NUM_RING_BUFFERS, &Bid);
		
		for (i = 0; i < NUM_RING_BUFFERS; i++)
		{
		    imgCreateBuffer(Sid, FALSE, bufSize, &ImagesRingBuffer[i]);
			imgSetBufferElement(Bid, i, IMG_BUFF_ADDRESS, (uInt32)ImagesRingBuffer[i]);
			imgSetBufferElement(Bid, i, IMG_BUFF_SIZE, bufSize);
			bufCmd = (i == (NUM_RING_BUFFERS - 1)) ? IMG_CMD_LOOP : IMG_CMD_NEXT;
			imgSetBufferElement(Bid, i, IMG_BUFF_COMMAND, bufCmd);
		}
	}

	if(err=imgMemLock(Bid)!=0)
		return err;
	if(err=imgSessionConfigure(Sid, Bid)!=0)
		return err;	
	return 0;
}

void clear_memory(int mode){

	int i,err=0; 

	if(mode==CTR_ONE_SHOT)                  //one shot
	{
/*		if (ImageBuffer != NULL)
			imgDisposeBuffer(ImageBuffer);

		if (Bid != 0){
			imgDisposeBufList(Bid, FALSE);
			Bid=0;
		}
		if(Sid != 0){
			imgClose (Sid, TRUE);
			Sid=0;
		}
		if(Iid != 0){
			imgClose (Iid, TRUE);
			Iid=0;
		}*/
	}


	else if(mode==CTR_SEQUENCE)				//sequence
	{
		for(i = 0; i < num_seq_buffer; i++)
		{
			if (ImagesSeqBuffer[i] != NULL)
				imgDisposeBuffer(ImagesSeqBuffer[i]);
		}
		if (Bid != 0){
			imgDisposeBufList(Bid, FALSE);
			Bid=0;
		}
	    if(Sid != 0){					
			imgClose (Sid, TRUE);
			Sid=0;
		}
		if(Iid != 0){
			imgClose (Iid, TRUE);
			Iid=0;
		}
	}


	else if(mode==CTR_RING)				     //ring
	{  
		for (i = 0; i < NUM_RING_BUFFERS; i++)
			if (ImagesRingBuffer[i] != NULL)
				err=imgDisposeBuffer(ImagesRingBuffer[i]);

		if (Bid != 0){
			err=imgDisposeBufList(Bid, FALSE);
			Bid=0;
		}
		if(Sid != 0){
			imgClose (Sid, TRUE);
			Sid=0;
		}
		if(Iid != 0){
			imgClose (Iid, TRUE);
			Iid=0;
		}
	}
}


DWORD ImaqThread(ThreadCtrl_t *Data)
{
static Int32 nbFrame = 0;  
static Int32 t1, t2;
static char textBuf[32];
static uInt32 bufAddr, bufAddr_new, currBufNum;
register int i;
unsigned char *ptr1, *ptr2, *ptr3; 
int *ptr4;
int counter=1;


	int max_i=640*480;

	// the thread stops when stop_recording goes to TRUE
	while(!Data->stop_recording)
	{
		t2 = GetTickCount();


		imgSessionExamineBuffer (Sid, BufNum, &currBufNum, &bufAddr);
		
		// Display the index of the extracted buffer
		sprintf(textBuf, "%d", currBufNum);
		Data->nr_of_img->SetWindowText(textBuf);

		ptr1= (unsigned char*)bufAddr;

		if(sub_ref)        //First a image has to be aquired with "OneShot"
		{
			ptr3= (unsigned char*)ImageBuffer;
	
			for(i=0; i< max_i; i++)
				*ptr1++ = abs(*ptr1 - *ptr3++);
		}

		if(thresh)
		{
			ptr1= (unsigned char*)bufAddr;
			//while(ptr1 < (unsigned char*)bufAddr+640*480)
			for(i=0;i<max_i;i++)
				*ptr1++ = (*ptr1 > thresh_value) ? 255 : 0;
		}


	    if(inv)
		{
			ptr1= (unsigned char*)bufAddr;

			for(i=0; i< max_i; i++)
				*ptr1 ++ = 255 - *ptr1;	
		}

//***************itegration of a binary image*****************************************
		if(integ && thresh)
		{
			ptr1= (unsigned char*)bufAddr;
			ptr2= (unsigned char*)ImageCalcBuffer;

			if(counter==1)
			{
				for(i=0; i< max_i; i++)
					*ptr2++=*ptr1++;
				counter=2;
			}
			else
			{
				for(i=0; i< max_i; i++)
				{		
					*ptr2++=(*ptr1 == 255 || *ptr2 == 255) ? 255 : *ptr1;
					ptr1++;
				}		
				bufAddr=(uInt32)ImageCalcBuffer;
			}	
		}
//************************************************************************************
//***************itegration of 255 image**********************************************
		if(integ && !thresh)
		{
			ptr1= (unsigned char*)bufAddr;
			ptr2= (unsigned char*)ImageCalcBuffer;
			ptr4= (int*)ImageCalcBuffer2;


			if(counter==1)
			{
				for(i=0; i< max_i; i++)
					*ptr4++=*ptr1++;
				
				counter=2;
				ptr1= (unsigned char*)bufAddr;
				ptr4= (int*)ImageCalcBuffer2;
			}

			for(i=0; i< max_i; i++)	
			{
				*ptr4 = *ptr4 + *ptr1;
				*ptr2++ = *ptr4/(counter);
				ptr1++;ptr4++;
			}
			counter++;
			bufAddr=(uInt32)ImageCalcBuffer;	
		}
//************************************************************************************

		if(0){
			ptr1= (unsigned char*)bufAddr;
			ptr2= (unsigned char*)ImageCalcBuffer;
			if(counter==1)
				for(i=0; i< max_i; i++)
				{
					*ptr2=*ptr1;
					ptr1++;
					ptr2++;
				}


			if(counter>1){
				for(i=0; i< max_i; i++)
				{
					*ptr2=(*ptr2 + *ptr1)/counter;
					ptr1++;
					ptr2++;
				}
				bufAddr=(uInt32)ImageCalcBuffer;
			}
			counter++;
		}

	
		int err=imgPlotDC ((GUIHNDL)Data->ImaqSmplDC->m_hDC, (void *)bufAddr, 0, 0,
				   Data->AcqWinWidth, Data->AcqWinHeight,
			       CANVASLEFT, CANVASTOP, FALSE);

//		CorImageFree(&image_out);

		// release the buffer back in the ring
		imgSessionReleaseBuffer (Sid);
			
		BufNum++;
		
		nbFrame++;
		if (nbFrame>100)
		{
			sprintf(textBuf, "%.2f", 1000.0 * (double)nbFrame / (double)(t2-t1));
			Data->frame_rate->SetWindowText(textBuf);
			t1 = t2;
			nbFrame=0;
		}
	}
	return 0;
}


void display(int number, char text[], CEdit *status)
{
	char buffer[256];
	
	if(number!= -99){
		itoa(number,buffer,10);
		status->ReplaceSel(buffer, FALSE);
	}

	status->ReplaceSel(text,FALSE);
	status->ReplaceSel("\r\n",FALSE);


}