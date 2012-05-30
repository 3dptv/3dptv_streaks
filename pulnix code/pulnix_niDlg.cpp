// pulnix_niDlg.cpp : implementation file
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "stdafx.h"
#include "pulnix_ni.h"
#include "pulnix_niDlg.h"
#include "niimaq.h"
#include "nidaq.h"
#include "nidaqcns.h"
#include "parameters.h"
#include "timer.h"
#include "functions.h"
#include "serial_port.h"
#include "grabber.h"


// Imaq globals
SESSION_ID   Sid = 0;
BUFLIST_ID   Bid = 0;
INTERFACE_ID Iid = 0;
PULSE_ID     plsID=0;

Int8         **ImagesRingBuffer;
Int8         **ImagesSeqBuffer;
Int8         **ImageBuffer;
Int8          *ImageCalcBuffer;
int           *ImageCalcBuffer2;
//unsigned int  *skippedBuffers; 
int            num_seq_buffer=NUM_SEQUENCE_BUFFERS, num_seq_buffer_new;
Frame          frame;
uInt32	  	   BufNum;
UINT           ImaqTimerId;
ThreadCtrl_t   ThreadData;
HANDLE         HThread; 
int			   last, changed=0;   
int            sub_ref=0, integ=0, avrg=0, thresh=0, inv=0;
int            thresh_value;
int			   line1_used,line2_used;
//**************************Variables to display image sequence****************
int			   DISP_SEQ=0;
//*****************************************************************************
Timer		   timer;
Grabber		   grabber1,grabber2;
//*****************************************************************************
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPulnix_niDlg dialog

CPulnix_niDlg::CPulnix_niDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPulnix_niDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPulnix_niDlg)
	m_file_save = _T("");
	m_set_frame_rate = 0;
	m_exp_time = 0.0;
	m_nr_of_buffers = 0;
	m_two_cameras = FALSE;
	m_master = FALSE;
	m_external_trigger = FALSE;
	m_async = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPulnix_niDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPulnix_niDlg)
	DDX_Control(pDX, IDC_STATUS_NR_IMAGES, m_nr_of_images);
	DDX_Control(pDX, IDC_TRIGGER2_SIGNAL_OUT, m_trigger2_signal_out);
	DDX_Control(pDX, IDC_TRIGGER2_OUT, m_trigger2_line_out);
	DDX_Control(pDX, IDC_TRIGGER1_OUT, m_trigger1_line_out);
	DDX_Control(pDX, IDC_TRIGGER1_SIGNAL_OUT, m_trigger1_signal_out);
	DDX_Control(pDX, IDC_IMG_NR, m_img_nr);
	DDX_Control(pDX, IDC_INV, m_inv);
	DDX_Control(pDX, IDC_THRESH_VALUE, m_thresh_value);
	DDX_Control(pDX, IDC_THRESH, m_thresh);
	DDX_Control(pDX, IDC_INTEG, m_integ);
	DDX_Control(pDX, IDC_SUB_REF, m_sub_ref);
	DDX_Control(pDX, IDC_FRAME_RATE_APPLY, m_frame_rate_apply);
	DDX_Control(pDX, IDC_SAVE_ONE_SHOT, m_save_one_shot_apply);
	DDX_Control(pDX, IDC_SEQUENCE, m_sequence);
	DDX_Control(pDX, IDC_ONE_SHOT, m_one_shot);
	DDX_Control(pDX, IDC_STOP, m_stop);
	DDX_Control(pDX, IDC_RING, m_ring);
	DDX_Control(pDX, IDC_FRAME_RATE, m_frame_rate);
	DDX_Control(pDX, IDC_STATUS, m_status);
	DDX_Text(pDX, IDC_NAME_ONE_SHOT, m_file_save);
	DDX_Text(pDX, IDC_SET_FRAME_RATE, m_set_frame_rate);
	DDV_MinMaxInt(pDX, m_set_frame_rate, 1, 120);
	DDX_Text(pDX, IDC_EXP_TIME_SET, m_exp_time);
	DDV_MinMaxDouble(pDX, m_exp_time, 0.1, 1000.);
	DDX_Text(pDX, IDC_NR_OF_BUFFERS, m_nr_of_buffers);
	DDV_MinMaxInt(pDX, m_nr_of_buffers, 1, 3300);
	DDX_Check(pDX, IDC_TWO_CAMERAS, m_two_cameras);
	DDX_Check(pDX, IDC_MASTER, m_master);
	DDX_Check(pDX, IDC_EXTERNAL_TRIGGER, m_external_trigger);
	DDX_Check(pDX, IDC_ASYNC, m_async);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPulnix_niDlg, CDialog)
	//{{AFX_MSG_MAP(CPulnix_niDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RING, OnRing)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_ONE_SHOT, OnOneShot)
	ON_BN_CLICKED(IDC_SEQUENCE, OnSequence)
	ON_BN_CLICKED(IDC_SAVE_ONE_SHOT, OnSaveOneShot)
	ON_BN_CLICKED(IDC_NR_OF_BUFFERS_APPLY, OnNrOfBuffersApply)
	ON_BN_CLICKED(IDC_SUB_REF, OnSubRef)
	ON_BN_CLICKED(IDC_INTEG, OnInteg)
	ON_BN_CLICKED(IDC_THRESH, OnThresh)
	ON_BN_CLICKED(IDC_CLEAR_INT, OnClearInt)
	ON_BN_CLICKED(IDC_SAVE_IMG_PROC, OnSaveImgProc)
	ON_EN_CHANGE(IDC_STATUS_NR_IMAGES, OnChangeStatusNrImages)
	ON_BN_CLICKED(IDC_LASER_TRIG_ON, OnLaserTrigOn)
	ON_BN_CLICKED(IDC_ASYNC, OnAsync)
	ON_BN_CLICKED(IDC_FRAME_RATE_APPLY, OnFrameRateApply)
	ON_BN_CLICKED(IDC_MAX_EXP, OnMaxExp)
	ON_BN_CLICKED(IDC_EXTERNAL_TRIGGER, OnExternalTrigger)
	ON_BN_CLICKED(IDC_SIGNAL1, OnGrabberSignal1)
	ON_BN_CLICKED(IDC_SIGNAL2, OnGrabberSignal2)
	ON_BN_CLICKED(IDC_SIGNAL1_OFF, OnSignal1Off)
	ON_BN_CLICKED(IDC_INV, OnThresh)
	ON_BN_CLICKED(IDC_LASER_TRIG_OFF, OnLaserTrigOn)
	ON_BN_CLICKED(IDC_SIGNAL2_OFF, OnSignal2Off)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPulnix_niDlg message handlers

BOOL CPulnix_niDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	display(-99,"Welcome", &m_status);
	//***************Check if this computer has a NI timer card************************************
	unsigned long infoValue;
	int status = Get_DAQ_Device_Info (DEVICE_NUMBER, ND_COUNTER_TYPE, &infoValue);
	if(status == -10401)
	{
		display(-99,"No timer", &m_status);

		m_frame_rate_apply.EnableWindow(FALSE);
		m_master=0;
		((CButton*)GetDlgItem(IDC_MASTER))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_LASER_TRIGGER))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_LASER_TRIG_ON))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_LASER_TRIG_OFF))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_FRAME_RATE_APPLY))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EXTERNAL_TRIGGER))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_MAX_EXP))->EnableWindow(FALSE);
		timer.exist=0;
	}

	else if(infoValue == ND_NI_TIO)
	{
		display(-99,"Timer detected", &m_status);
		m_master=1;
		((CButton*)GetDlgItem(IDC_LASER_TRIGGER))->SetWindowText("1");
		((CButton*)GetDlgItem(IDC_LASER_TRIG_OFF))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_ASYNC))->SetCheck(0);
		//disable async settings
		((CButton*)GetDlgItem(IDC_LASER_TRIGGER))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_LASER_TRIG_ON))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_LASER_TRIG_OFF))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_FRAME_RATE_APPLY))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_MAX_EXP))->EnableWindow(FALSE);
		timer.exist=1;
	}
	//***************End Check if this computer has a NI timer card**********************************


	ThreadData.is_running=0;
	m_stop.EnableWindow(FALSE);	
	m_save_one_shot_apply.EnableWindow(FALSE);	
	m_sub_ref.EnableWindow(FALSE);
	m_thresh_value.SetWindowText("50");
	m_two_cameras=0;
	line1_used=-1;
	line2_used=-1;
	((CButton*)GetDlgItem(IDC_SIGNAL1_OFF))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_SIGNAL2_OFF))->SetCheck(1);
	m_trigger1_signal_out.SetCurSel(2);
	m_trigger1_line_out.SetCurSel(7);
	m_trigger2_signal_out.SetCurSel(3);
	m_trigger2_line_out.SetCurSel(8);

	m_set_frame_rate=120;
	m_exp_time=8.3;
	m_nr_of_buffers=10;
	UpdateData(FALSE);
	UpdateData(TRUE);

	grabber1.line=7;
	grabber1.signal=0;
	grabber1.locked_line=8;
	grabber1.locked_signal=1;
	grabber2.line=8;
	grabber2.signal=1;
	grabber2.locked_line=7;
	grabber2.locked_signal=0;

	//Allocate memory for sequence
	ImagesSeqBuffer=new Int8*[num_seq_buffer];
	//Allocate memory for calculation image
	ImageCalcBuffer=new Int8[480*640];
 	ImageCalcBuffer2=new int[480*640];

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPulnix_niDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPulnix_niDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}





// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPulnix_niDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//*********************************************************************************************************
//*********************************************************************************************************
//**************************************    RING   ********************************************************
//*********************************************************************************************************
//*********************************************************************************************************
void CPulnix_niDlg::OnRing() 
{
	DWORD			dwThreadId;	

	display(-99, " ", &m_status);
	clear_memory(last);
	display(last, "memory cleared", &m_status);
	last=CTR_RING;

	DISP_SEQ=0;

	ImagesRingBuffer=new Int8*[NUM_RING_BUFFERS];

	if(ThreadData.is_running==0){
		
		ThreadData.is_running=1;
		m_ring.EnableWindow(FALSE);
		m_stop.EnableWindow(TRUE);
		m_one_shot.EnableWindow(FALSE);
		m_sequence.EnableWindow(FALSE);
	    UpdateData(TRUE);

		// Open interface and a session
		imgInterfaceOpen ("img0", &Iid);
		imgSessionOpen (Iid, &Sid);
		display(-99, "interface img0 open", &m_status);


		init_window(&frame);

		//this functions sets the timer board to output the HD/VD signal
		if(timer.exist){
			timer.initialize();
			display(-99, "  VInit running on 120Hz", &m_status);
			//CPulnix_niDlg::OnFrameRateApply();
		}
		if(m_async)
			CPulnix_niDlg::OnFrameRateApply();

		//if the mode is set to two cameras the accisitions is initialized 
		//to wait for a trigger signal from the timer board to start the aquisition
		if(m_two_cameras || m_external_trigger)                   
			int err=imgSessionTriggerConfigure(Sid, IMG_EXT_RTSI6, 
			                           IMG_TRIG_POLAR_ACTIVEH, 60000,
								       IMG_TRIG_ACTION_CAPTURE);

		set_memory(CTR_RING, frame);
		display(-99, "  memory set", &m_status);        

		BufNum = 0;

		ThreadData.frame_rate=&m_frame_rate;
		ThreadData.status=&m_status;
		ThreadData.nr_of_img=&m_nr_of_images;
		ThreadData.stop_recording=0;
		ThreadData.AcqWinHeight=frame.AcqWinHeight;
		ThreadData.AcqWinWidth=frame.AcqWinWidth;
		ThreadData.ImaqSmplDC=this->GetDC();

		imgSessionAcquire(Sid, 0, NULL);

		if(grabber1.armed)
			grabber1.drive_signal();
		if(grabber2.armed)
			grabber2.drive_signal();
		grabber1.images_running=1;
		grabber2.images_running=1;

		if(timer.exist && m_two_cameras && m_master && !m_external_trigger)  //two cameras and master
			timer.trigger();					//triggers the aquisition with one pulse over the RTSI6 line
		else if(timer.do_trigger_on_trigger)	//set timer board to wait for external trigger
			timer.trigger_on_trigger();
	

		// Start the acquisition thread
		HThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ImaqThread, 
				  (void *)&ThreadData, 0, &dwThreadId);    
	}
}




void CPulnix_niDlg::OnStop() 
{
	unsigned long	 bufNum;

    ThreadData.stop_recording=1;
	ThreadData.is_running=0;

	m_two_cameras=0;
	m_stop.EnableWindow(FALSE);
	m_ring.EnableWindow(TRUE);
	m_one_shot.EnableWindow(TRUE);
	m_sequence.EnableWindow(TRUE);
    timer.stop();
	grabber1.images_running=0;
	grabber2.images_running=0;	

    imgSessionAbort(Sid, &bufNum);
	CloseHandle (HThread);

	imgSessionTriggerClear(Sid);

	//stop the output from the frame grabber
	CPulnix_niDlg::OnSignal1Off();
	CPulnix_niDlg::OnSignal2Off();
	((CButton*)GetDlgItem(IDC_SIGNAL1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_SIGNAL2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_SIGNAL1_OFF))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_SIGNAL2_OFF))->SetCheck(1);

	if (Bid != 0)
		imgMemUnlock(Bid);
}
//*********************************************************************************************************
//*********************************************************************************************************
//**************************************    END RING   ****************************************************
//*********************************************************************************************************
//*********************************************************************************************************


void CPulnix_niDlg::OnOneShot() 
{
	uInt32 status, currBufNum;
	int err;

	display(-99, " ", &m_status);
	clear_memory(last);
	display(last, " memory cleared", &m_status);
	last=CTR_ONE_SHOT;

	m_ring.EnableWindow(FALSE);
	m_stop.EnableWindow(TRUE);
	m_one_shot.EnableWindow(FALSE);	
	m_sequence.EnableWindow(FALSE);
	UpdateData(TRUE);

	// Open an interface and a session
	imgInterfaceOpen ("img0", &Iid);
	imgSessionOpen (Iid, &Sid);
	display(-99, "interface img0 open", &m_status);

	if(m_two_cameras)  //Two Cameras	
		imgSessionTriggerConfigure(Sid, IMG_EXT_RTSI6, 
		                       IMG_TRIG_POLAR_ACTIVEH, 60000,
							   IMG_TRIG_ACTION_BUFLIST);


	init_window(&frame);
    set_memory(CTR_ONE_SHOT, frame);
	display(-99, "  memory set", &m_status);

	// start the acquisition
	imgSessionAcquire(Sid, 0, NULL);       //async deleted

	if(timer.exist && m_two_cameras && m_master)  //two cameras and master
		timer.trigger();

	err=imgSessionStatus (Sid, &status, &currBufNum);
	while(status)
	{
		err=imgSessionStatus (Sid, &status, &currBufNum);
	}

	imgPlotDC ((GUIHNDL)this->GetDC()->m_hDC, (void *)ImageBuffer, 0, 0,
				  frame.AcqWinWidth, frame.AcqWinHeight,
			      CANVASLEFT, CANVASTOP, FALSE);

	if (Bid != 0)
		imgMemUnlock(Bid);

	display(-99, "One Shot done", &m_status);

	m_ring.EnableWindow(TRUE);
	m_stop.EnableWindow(FALSE);
	m_one_shot.EnableWindow(TRUE);
	m_sequence.EnableWindow(TRUE);
	m_save_one_shot_apply.EnableWindow(TRUE);
	m_sub_ref.EnableWindow(TRUE);

}



void CPulnix_niDlg::OnFrameRateApply() 
{
	//this routine sets the VDInit frequency 
	//and the exposure time 
	//the camera must be set to asynchronous mode befor this call

	UpdateData(TRUE);
	int rate=m_set_frame_rate;
	double exp_time = m_exp_time;
	int max_exp=0;
	timer.change(rate, &exp_time, max_exp);
	m_exp_time= exp_time;
	display(-99, "	VInit frequency changed as spezified", &m_status);
	UpdateData(FALSE);
	
}

void CPulnix_niDlg::OnMaxExp() 
{
	//this routine calculates sets the VDInit frequency 
	//of the camera for a given frequence VInit
	//the camera must be set to asynchronous mode befor this call
	double  exp_time;

	UpdateData(TRUE);
	int rate=m_set_frame_rate;
	int max_exp=1;
	timer.change(rate, &exp_time, max_exp);
	m_exp_time= exp_time;
	display(-99, "	VInit frequency changed as spezified", &m_status);
	UpdateData(FALSE);
 
}


void CPulnix_niDlg::OnNrOfBuffersApply() 
{
	UpdateData(TRUE);
	num_seq_buffer_new=m_nr_of_buffers;
	if(num_seq_buffer_new>3300)
		num_seq_buffer_new =3300;
	else if(num_seq_buffer_new < 1)
		num_seq_buffer_new =1;
	display(num_seq_buffer_new, " # of buffer changed", &m_status);
	changed=1;
	m_nr_of_buffers=num_seq_buffer_new;
	UpdateData(FALSE);
}


//*********************************************************************************************************
//*********************************************************************************************************
//**************************************    SEQUENCE   ****************************************************
//*********************************************************************************************************
//*********************************************************************************************************
void CPulnix_niDlg::OnSequence() 
{
    int   i;  
	uInt32 status, currBufNum;
	int err=0;

	DISP_SEQ=0;
	m_ring.EnableWindow(FALSE);
	m_stop.EnableWindow(TRUE);
	m_one_shot.EnableWindow(FALSE);	
	m_sequence.EnableWindow(FALSE);
	UpdateData(TRUE);

	display(-99, " ", &m_status);
	clear_memory(last);
	display(-99, "last memory cleared", &m_status);

	if(changed)
	{
		delete [] ImagesSeqBuffer;
		ImagesSeqBuffer=new Int8*[num_seq_buffer_new];		      
		num_seq_buffer=num_seq_buffer_new;
		changed=0;
		display(-99, "memory updated", &m_status);
	}
	last=CTR_SEQUENCE;
	
	// Open an interface and a session
	imgInterfaceOpen ("img0", &Iid);
	imgSessionOpen (Iid, &Sid);
 	display(-99, "Interface img0 open", &m_status);

    init_window(&frame);
	
	if(timer.exist)
	{
		timer.initialize();
		display(-99, "    VD running on 120Hz", &m_status);
	}
	if(m_async)  //do not change here, except you know the devil
	{
		CPulnix_niDlg::OnFrameRateApply();
		display(-99, "    Changed to VInit, running on 120Hz", &m_status);
	}

	//if there are two cameras indicated then the frame grabber 
	//is set in wait modus for a trigger signal from the timer 
	//board. This signal is generated by the routine trigger();
	if(m_two_cameras || m_external_trigger)                  
		int err=imgSessionTriggerConfigure(Sid, IMG_EXT_RTSI6, 
			                       IMG_TRIG_POLAR_ACTIVEH, 60000,
								   IMG_TRIG_ACTION_BUFLIST); //CAPTURE

	if((err=set_memory(CTR_SEQUENCE, frame))!=0)
	{
		//imgShowError(err,buffer);
		display(err, "  Fehler", &m_status);
	}
	else
	{
		display(-99, "  memory set", &m_status);
		display(-99, "  start acquisition", &m_status);

		if(grabber1.armed)
		{
			grabber1.drive_signal();
			grabber1.armed=0;
		}
		if(grabber2.armed)
		{
			grabber2.drive_signal();
			grabber2.armed=0;
		}

		// start the acquisition
		imgSessionAcquire(Sid, 1, NULL);

		//********************************************************************************************
		//*******************  TRIGGER SECTION  ******************************************************
		//********************************************************************************************
		//two cameras and master:
		//a signal is send from the timer card over the RTSI bus to trigger
		//both frame grabber
		if(timer.exist && m_two_cameras && m_master && !m_external_trigger)  
			timer.trigger();
		//set timer board to wait for external trigger arriving at the timers gate
		else if(timer.do_trigger_on_trigger)   
			timer.trigger_on_trigger();
		//********************************************************************************************
		//******************* END TRIGGER SECTION  ***************************************************
		//********************************************************************************************

		status=1;
		i=0;
		int done =0;
		char buffer[255];
		while(status)
		{	
			err=imgSessionStatus (Sid, &status, &currBufNum);
			if(currBufNum <= 0 || currBufNum >10000)
			{
				if(done==0)
				{
					display(-99, "  waiting ....", &m_status);
					done=1;
				}
			}
			else
			{
				if(done==1)
				{
					display(-99, "  recording ....", &m_status);
					done=2;
				}
				itoa(currBufNum,buffer,10);
				m_nr_of_images.SetWindowText(buffer);
				UpdateData(TRUE);
				i++;
			}
		}
		display(-99, "Sequence done", &m_status);

		if (Bid != 0)
			imgMemUnlock(Bid);
	}

	//if(timer.do_trigger_on_trigger) 
		timer.reset_trigger_on_trigger();

	imgSessionTriggerClear(Sid);

	unsigned long	 bufNum;
    imgSessionAbort(Sid, &bufNum);

	m_ring.EnableWindow(TRUE);
	m_stop.EnableWindow(FALSE);
	m_one_shot.EnableWindow(TRUE);
	m_sequence.EnableWindow(TRUE);	
	m_save_one_shot_apply.EnableWindow(TRUE);
	m_nr_of_images.SetFocus();
	m_two_cameras=0;
	m_img_nr.SetRange(0,num_seq_buffer-1);
	DISP_SEQ=1;
	if(timer.exist)
		timer.stop();

	//stop the output (FrameStart, FrameDone, etc.. from the frame grabber
	CPulnix_niDlg::OnSignal1Off();
	CPulnix_niDlg::OnSignal2Off();
	((CButton*)GetDlgItem(IDC_SIGNAL1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_SIGNAL2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_SIGNAL1_OFF))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_SIGNAL2_OFF))->SetCheck(1);
	UpdateData(FALSE);
}
//*********************************************************************************************************
//*********************************************************************************************************
//**************************************    END SEQUENCE   ************************************************
//*********************************************************************************************************
//*********************************************************************************************************


void CPulnix_niDlg::OnSaveOneShot() 
{
	char buffer[256];
	int i,j;
	int ans,sel;


	CFileDialog dlg(FALSE,
		NULL,
		"image", 
		OFN_OVERWRITEPROMPT,
		"raw image files|*.raw|bitmap files|*.bmp|tif files|*.tif|png files|*.png||",this);



	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		sel=dlg.m_ofn.nFilterIndex;
		
		strcpy(buffer,dlg.GetPathName());
		j=strlen(buffer);

		m_file_save = dlg.GetFileTitle();

		if(last==CTR_ONE_SHOT)
		{
			if (ImageBuffer != NULL)
			{
				if(sel==3)
				{
					sprintf(buffer + j,".tif");
					if(!imgSessionSaveBufferEx(Sid, ImageBuffer, buffer))
						display(-99, "Image saved to disk", &m_status);
					else
						display(-99, "Error saving file", &m_status);
				}
				if(sel==2)
				{
					sprintf(buffer + j,".bmp");
					if(!imgSessionSaveBufferEx(Sid, ImageBuffer, buffer))
						display(-99, "Image saved to disk", &m_status);
					else
						display(-99, "Error saving file", &m_status);
				}
				if(sel==4)
				{
					sprintf(buffer + j,".png");
					if(!imgSessionSaveBufferEx(Sid, ImageBuffer, buffer))
						display(-99, "Image saved to disk", &m_status);
					else
						display(-99, "Error saving file", &m_status);
				}
				if(sel==1)
				{
					sprintf(buffer + j,".raw");

					FILE *fp= fopen(buffer,"wb");
					fwrite((char *)ImageBuffer, sizeof(char), 640*480, fp);
					fclose(fp);

					display(-99, "Image saved to disk", &m_status);
				}
			}

		}
		else if(last==CTR_SEQUENCE)
		{
			for(i = 0; i < num_seq_buffer; i++)
			{
				if (ImagesSeqBuffer[i] != NULL)
				{
					if(sel==3){
						sprintf(buffer + j,"%04d.tif",i);
						imgSessionSaveBufferEx(Sid, ImagesSeqBuffer[i], buffer);
					}
					if(sel==2){
						sprintf(buffer + j,"%04d.bmp",i);
						imgSessionSaveBufferEx(Sid, ImagesSeqBuffer[i], buffer);
					}
					if(sel==4){
						sprintf(buffer + j,"%04d.png",i);
						imgSessionSaveBufferEx(Sid, ImagesSeqBuffer[i], buffer);
					}
					if(sel==1)
					{
						sprintf(buffer + j,"%04d.raw",i);

						FILE *fp= fopen(buffer,"wb");
						fwrite((char *)ImagesSeqBuffer[i], sizeof(char), 640*480, fp);
						fclose(fp);

						//ofstream outdat;
						//outdat.open(buffer);
						//outdat.write((char *)ImagesSeqBuffer[i],640*480);
						//outdat.close();
					}
				}
			}
		display(i, " Images saved to disk", &m_status);
		}

		UpdateData(FALSE);
	}
}



void CPulnix_niDlg::OnOK() 
{
	clear_memory(last);

	delete [] ImagesSeqBuffer;
	delete [] ImagesRingBuffer;
	delete ImageCalcBuffer;

	//stop the timer card
	timer.laser_trigger_off();
	timer.stop();

	CDialog::OnOK();
}


void CPulnix_niDlg::OnSubRef() 
{
	if(m_sub_ref.GetCheck())
		sub_ref=1;
	else sub_ref=0;	
}



void CPulnix_niDlg::OnInteg() 
{
	int i;
	if(m_integ.GetCheck())
	{
		integ=1;
		//inizialize calculation buffer
		for(i=0;i<480*640;i++) *ImageCalcBuffer = 0;
	}
	else integ=0;	
}




void CPulnix_niDlg::OnThresh() 
{
	char buffer[256];
	if(m_thresh.GetCheck())
	{
		m_thresh_value.GetWindowText(buffer,8);
		thresh_value=atoi(buffer);
		thresh=1;
	}
	else thresh=0;
	
	if(m_inv.GetCheck())
		inv=1;
	else
		inv=0;


}


//*************This rotine sends the HD or VD or frame start or frame done**********
//*************to the specified line of the frame grabber***************************
void CPulnix_niDlg::OnGrabberSignal1() 
{
	m_trigger1_signal_out.EnableWindow(FALSE);
	m_trigger1_line_out.EnableWindow(FALSE);

	UpdateData(TRUE);
	int signal=m_trigger1_signal_out.GetCurSel();
	if(signal != grabber1.locked_signal || grabber2.on == FALSE )
	{
		grabber1.signal=signal;
		grabber2.locked_signal=signal;
	}
	else
	{
		display(-99, " This signal is locked!", &m_status);
		return;
	}

	int line=m_trigger1_line_out.GetCurSel();
	if(line != grabber1.locked_line && line !=6 )
	{
		grabber1.line=line;
		grabber2.locked_line=line;
	}
	else
	{
		display(-99, " This line is locked!", &m_status);
		return;
	}

	grabber1.armed	=TRUE;
	grabber1.on		=TRUE;
	if(grabber1.images_running==TRUE)
		grabber1.drive_signal();
}


void CPulnix_niDlg::OnSignal1Off() 
{
	//Both lines are set zero!
	((CButton*)GetDlgItem(IDC_SIGNAL1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_SIGNAL2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_SIGNAL1_OFF))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_SIGNAL2_OFF))->SetCheck(1);
	m_trigger1_signal_out.EnableWindow(TRUE);
	m_trigger1_line_out.EnableWindow(TRUE);	
	m_trigger2_signal_out.EnableWindow(TRUE);
	m_trigger2_line_out.EnableWindow(TRUE);	
	grabber1.on=FALSE;
	grabber2.on=FALSE;

	imgSessionTriggerClear(Sid);
}



void CPulnix_niDlg::OnGrabberSignal2() 
{
	m_trigger2_signal_out.EnableWindow(FALSE);
	m_trigger2_line_out.EnableWindow(FALSE);

	UpdateData(TRUE);
	int signal=m_trigger2_signal_out.GetCurSel();
	if(signal != grabber2.locked_signal || grabber1.on == FALSE)
	{
		grabber2.signal=signal;
		grabber1.locked_signal=signal;
	}
	else
	{
		display(-99, " This signal is locked!", &m_status);
		return;
	}

	int line=m_trigger2_line_out.GetCurSel();
	if(line != grabber2.locked_line && line !=6)
	{
		grabber2.line=line;
		grabber1.locked_line=line;
	}
	else
	{
		display(-99, " This line is locked!", &m_status);
		return;
	}

	grabber2.armed	= TRUE;
	grabber2.on		= TRUE;
	if(grabber2.images_running==TRUE)
		grabber2.drive_signal();
}



void CPulnix_niDlg::OnSignal2Off() 
{
	//Both lines are set zero!
	((CButton*)GetDlgItem(IDC_SIGNAL1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_SIGNAL2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_SIGNAL1_OFF))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_SIGNAL2_OFF))->SetCheck(1);	
	m_trigger1_signal_out.EnableWindow(TRUE);
	m_trigger1_line_out.EnableWindow(TRUE);	
	m_trigger2_signal_out.EnableWindow(TRUE);
	m_trigger2_line_out.EnableWindow(TRUE);	
	grabber1.on=FALSE;
	grabber2.on=FALSE;

	imgSessionTriggerClear(Sid);
}



void CPulnix_niDlg::OnClearInt() 
{
	int i;

	//inizialize calculation buffer
	for(i=0;i<480*640;i++) *ImageCalcBuffer = 0;
}



void CPulnix_niDlg::OnSaveImgProc() 
{
	char buffer[255];
	int j;

	if(ImageCalcBuffer != NULL)
	{
		CFileDialog dlg(FALSE,
			NULL,
			NULL, OFN_OVERWRITEPROMPT,
			"Bitmap files|*.bmp|all files|*.*||", this);	

		// show it to the user
		dlg.DoModal();
		//dlg.GetFileExt();
		strcpy(buffer,dlg.GetPathName());
		j=strlen(buffer);
		sprintf(buffer + j,".bmp");
		if(!imgSessionSaveBufferEx(Sid, ImageCalcBuffer, buffer))
				display(-99, "Image saved to disk", &m_status);
	}
	else 
		display(-99, "No Image saved to disk", &m_status);

	return;	
}


void CPulnix_niDlg::OnChangeStatusNrImages() 
{
	char buffer[255];
	int display_nr,err,i;
	unsigned char *ptr1,*ptr2;
  
	if(DISP_SEQ)
	{
		GetDlgItemText(IDC_STATUS_NR_IMAGES,buffer,8);
		display_nr=atoi(buffer);


		if(display_nr >= num_seq_buffer || display_nr < 0)
		{
			display_nr=num_seq_buffer-1;
			SetDlgItemText(IDC_STATUS_NR_IMAGES,"0");
		}
		//do some simple image processing
		if(thresh)
		{	
			ptr1 = (unsigned char*)ImagesSeqBuffer[display_nr];
			ptr2 = (unsigned char*)ImageCalcBuffer;
			for(i=0; i< 640*480; i++)
				*ptr2++ = (*ptr1++ > thresh_value) ? 255 : 0;

			err=imgPlotDC((GUIHNDL)this->GetDC()->m_hDC, (void *)ImageCalcBuffer, 0, 0, 
						frame.AcqWinWidth, frame.AcqWinHeight, 
						CANVASLEFT, CANVASTOP, FALSE);


		}
		//no some simple image processing
		else 
			err=imgPlotDC((GUIHNDL)this->GetDC()->m_hDC, (void *)ImagesSeqBuffer[display_nr], 0, 0, 
						frame.AcqWinWidth, frame.AcqWinHeight, 
						CANVASLEFT, CANVASTOP, FALSE);


	}
}




void CPulnix_niDlg::OnLaserTrigOn() 
{
	int delta,err;
	char buffer[26];

	switch(GetCurrentMessage()->wParam)
	{
		case IDC_LASER_TRIG_ON : 
			GetDlgItemText(IDC_LASER_TRIGGER,buffer,8);
			delta=atoi(buffer);
			err=timer.laser_trigger_on(delta);
			break;
		case IDC_LASER_TRIG_OFF: 
			err=timer.laser_trigger_off();
			break;
	}
}



void CPulnix_niDlg::OnAsync() 
{
	//COM1  port is used
	int ans=Port::open();
	if(ans)
	{
		if(Port::set() == FALSE)
		{
			display(-99, "Error, in port set", &m_status);
			return;
		}
		if(Port::set_timeout()==FALSE)
		{
			display(-99, "Error, in port set", &m_status);
			return;
		}
		Port::port_open=1;
		if(((CButton*)GetDlgItem(IDC_ASYNC))->GetCheck())
		{
			if(timer.exist)
			{
				((CButton*)GetDlgItem(IDC_LASER_TRIGGER))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(IDC_LASER_TRIG_ON))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(IDC_LASER_TRIG_OFF))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(IDC_FRAME_RATE_APPLY))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(IDC_MAX_EXP))->EnableWindow(TRUE);
			}
			//set the camera to asynchronous modus
			char c[255], *p=c;
			//set Shutter mode to Async  (SA0)
			int j=sprintf(p,"\x3ASA0\x0D");
			Port::write(p, j);
			char a=Port::read();
			//set Shutter time to P.W.C  (SA9)
			j=sprintf(p,"\x3ASA9\x0D");
			Port::write(p, j);
			a=Port::read();
			//set Async Sync out to One Shot  (O1)
			j=sprintf(p,"\x3AO1\x0D");
			Port::write(p, j);
			a=Port::read();
			display(-99, "Camera is set on asyncronous modus", &m_status);
		}
		else
		{	
			if(timer.exist)
			{	
				((CButton*)GetDlgItem(IDC_LASER_TRIGGER))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_LASER_TRIG_ON))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_LASER_TRIG_OFF))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_FRAME_RATE_APPLY))->EnableWindow(FALSE);
				((CButton*)GetDlgItem(IDC_MAX_EXP))->EnableWindow(FALSE);
			}
			//set the camera to normal modus
			char c[255], *p=c;
			//set Async Sync out to Continuous  (O0)
			int j=sprintf(p,"\x3AO0\x0D");
			Port::write(p, j);
			char a=Port::read();
			//set Shutter time "Normal"and shutter  mode to "Manual"  (SM9)
			j=sprintf(p,"\x3ASM9\x0D");
			Port::write(p, j);
			a=Port::read();
			display(-99, "Camera is set on normal modus", &m_status);
		}
		//close the port
		Port::close();
		Port::port_open=0;
	}
	else 
	{
		display(-99, "Error, COM1 port is already open!", &m_status);
	}
}


void CPulnix_niDlg::OnExternalTrigger() 
{
	if(timer.do_trigger_on_trigger == FALSE)
		timer.do_trigger_on_trigger = TRUE;
	else
		timer.do_trigger_on_trigger = FALSE;

	UpdateData(TRUE);
}


