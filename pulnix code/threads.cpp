#include "stdafx.h"
#include "niimaq.h"
#include "functions.h"
#include "pulnix_ni.h"
#include "pulnix_niDlg.h"

//#define CANVASTOP  90     // top of the display area
//#define CANVASLEFT 25     // left of the display area

DWORD ImaqThread(ThreadCtrl_t *Data)
{
static Int32 nbFrame = 0;  
static Int32 t1, t2;
static char textBuf[32];
static uInt32 bufAddr,currBufNum;



	// the thread stop when stop_recording goes to TRUE
	while(!Data->stop_recording)
	{
		t2 = GetTickCount();
		// Hold the buffer whose index is BufNum. This is a cumulative buffer
		// index, if the buffer has not been acquired yet this function will block
		// until it is available. If the buffer has been overwritten this function
		// will return the last available buffer. Once the buffer is hold the buffer
		// won't be overwritten until it is released with imgSessionReleaseBuffer.
		imgSessionExamineBuffer (Sid, BufNum, &currBufNum, &bufAddr);
		
		// Display the index of the extracted buffer
		//sprintf(textBuf, "%d", currBufNum);
//		m_status=(char)currBufNum;
//		SetWindowText (HBufExtracted, textBuf);
		
		// Display the index of the required buffer
//		sprintf(textBuf, "%d", BufNum);
//		SetWindowText (HBufWanted, textBuf);
	
		// Computes and display the number of missed buffers
		if(currBufNum-BufNum)
		{
			MissedNbBuf = MissedNbBuf + currBufNum-BufNum;
			BufNum = currBufNum;
		}
//		sprintf(textBuf, "%d", MissedNbBuf);
//		SetWindowText (HNbBufMissed, textBuf);
	
		imgPlotDC ((GUIHNDL)Data->ImaqSmplDC->m_hDC, (void *)bufAddr, 0, 0,
				   Data->AcqWinWidth, Data->AcqWinHeight,
			       CANVASLEFT, CANVASTOP, FALSE);

//		RECT myrect={0,0,100,100};
//		HBRUSH mybrush=CreateSolidBrush(0x00ffffff);
//
//		FillRect(Data->ImaqSmplDC->m_hDC,&myrect,mybrush);

		// release the buffer back in the ring
		imgSessionReleaseBuffer (Sid);
			
		// Now we want to extract the next buffer
		BufNum++;
		
		// Calculate the number of frame per seconds every 10 frames
		nbFrame++;
		if (nbFrame>10)
		{
			sprintf(textBuf, "%.2f", 1000.0 * (double)nbFrame / (double)(t2-t1));
			//_ltoa(textBuf,buffer,10);
			Data->frame_rate->SetWindowText(textBuf);
			t1 = t2;
			nbFrame=0;
		}
	}
	return 0;
}



/*
CALLBACK OnTimer(UINT uID,	UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    static int error, status, i;  
	unsigned int currBufNum;
    char    buffer[32];
	
	if(uID == ImaqTimerId)
	{
		// Check the progress of the acquisition
		imgSessionStatus (Sid, &status, &currBufNum);
			
		
//		if(status)
//			SetWindowText (HStatus, "Acquiring...");
//		else
//			SetWindowText (HStatus, "Finished");

//		sprintf(buffer, "%d", currBufNum);
//		SetWindowText (HBufNum, buffer);
		
		if(currBufNum != 0xFFFFFFFF)
			imgPlot ((GUIHNDL)Data->ImaqSmplDC->m_hDC, 
			          (void *)ImaqBuffers[currBufNum], 0, 0, 
					  Data->AcqWinWidth, Data->AcqWinHeight, 
					  CANVASLEFT, CANVASTOP, FALSE);
    
		
		if(!status) // acquisition is finished
		{
			// Stop the timer
			timeKillEvent(ImaqTimerId);
        
			// Umlock memory
			if (Bid != 0)
				imgMemUnlock(Bid);

			// dispose of the buffers
			for (i = 0; i < NUM_SEQUENCE_BUFFERS; i++)
				if (ImaqBuffers[i] != NULL)
					imgDisposeBuffer(ImaqBuffers[i]);

			// close this buffer list
			if (Bid != 0)
				imgDisposeBufList(Bid, FALSE);
			
			// Close the interface and the session
			imgClose (Sid, TRUE);
			imgClose (Iid, TRUE);
		}
	}

    return;
}
*/