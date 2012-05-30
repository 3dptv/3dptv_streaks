// Stereo2Dlg.cpp : implementation file
//
#include "stdafx.h"
#include "Stereo2.h"
//#include <windows.h>
#include "Stereo2Dlg.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <imsl.h>
#include "niimaq.h"
#include "parameters.h"
#include "matrix_op.h"
#include "image.h"
#include "path.h"
#include "pathlist.h"
#include "camera.h"
#include "spline.h"
#include "splinelist.h"
#include "plot.h"
#include "write_out.h"
#include "global.h"
#include "vrml_generator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDC     *device;
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//***************                                                              *****************
//***************          Global Variables for the Code Stereo2               *****************
//***************          (Windows independent!)                              *****************
//**********************************************************************************************
//**********************************************************************************************
//**************************Variables for Program Control***************************************
bool	  CONTR=TRUE;		//to run the program faster all the CONTR orders can be deleted
							//they are actually just used for visual control in pixel space
bool      PROJECTIVE=FALSE; //for projective correct corespondence building set PROJECTIVE=TRUE
							//else set FALSE;
bool      SPLINE_ITERATION=FALSE; //spline refinement by intersections
bool	  ANALYSIS=FALSE;
bool	  CHG_COORD=FALSE;
int       img_name_zeros=9; //gives the zeros of the image name  xxx0000.raw   =9
							//									 xxx000.raw    =8

int		  analyze_img;		//variable to indicate which image is analyzed
int		  changed;			//variable to control functions to analyze the images
bool      running;
//***************used for getting the origin coordinates******************************************
int p_i[8]={1,0,0,0,0,0,0,0};
Pixel p_coord[8];
//**************************End Variables for Program Control***********************************
//**************************End Variables for Calibration Program Control***********************
int       T_S;                    //time step
//**********************Memory for images ******************************************************
//        uInt8 is defined in niimaq.h
uInt8     imagedatacam1[640*480];          //the actual image containing the streaks
uInt8     imagedatacam2[640*480];
uInt8     refimagedatacam1[640*480];       //reference image used to eliminate background
uInt8     refimagedatacam2[640*480];
uInt8     colimagedata1[3*640*480];        //used to display centroids, epi-lines, numbers etc. 
uInt8     colimagedata2[3*640*480];
//**********************End Memory for images***************************************************
//**********************The Look-up tables with the normalized pixel coordinates****************
Vec2	 table1[640*480], table2[640*480];
//**********************************************************************************************
Camera camera1;
Camera camera2;
//**********************************************************************************************
FILE *f_path1,   *f_path2;
FILE *f_spline1, *f_spline2;
FILE *f_global;
FILE *f_spline_global;
FILE  *fp1;
FILE  *fp2;
FILE  *fp3;

void test_spline();
double Get_r_max(int *r1, int *r2, int *t_s, int, int, double spl1_r_max, double spl2_r_max);
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//***************                                                              *****************
//***************         END Global Variables for the Code Stereo2            *****************
//***************          (Windows independent!)                              *****************
//**********************************************************************************************
//**********************************************************************************************
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
// CStereo2Dlg dialog

CStereo2Dlg::CStereo2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStereo2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStereo2Dlg)
	m_first_img = 0;
	m_last_img = 40;
	m_thres_ptv = 9;
	m_thres_ptv2 = 9;
	m_file_img  =     _T("G:\\Images\\Delta28_Mai\\Images\\cam1000.raw");
	m_file_img2 =     _T("G:\\Images\\Delta28_Mai\\Images\\cam2000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta28_Mai\\Images\\ref_cam1.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta28_Mai\\Images\\ref_cam2.raw");
	m_file_calib  =   _T("G:\\Images\\Delta28_Mai\\Calibration\\cam1_03.par");
	m_file_calib2 =   _T("G:\\Images\\Delta28_Mai\\Calibration\\cam2_03.par");
	m_img_nr = 0;
	m_thresh_analyze = 9;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
/*	
	//Data set Uniform_28_Oct_2002 Images3  hmm
	//optimal threshold 8 AND 4
	//search line 500/500
	m_file_img  =     _T("G:\\Images\\Uniform_28_Oct_2002\\Images3\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Uniform_28_Oct_2002\\Images3\\ref2_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Uniform_28_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Uniform_28_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Uniform_28_Oct_2002\\Images3\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Uniform_28_Oct_2002\\Images3\\ref2_cam2.raw");


	//Data set Uniform_28_Oct_2002 Images2  hmm
	//optimal threshold 8 AND 3
	//search line 500/500
	m_file_img  =     _T("G:\\Images\\Uniform_28_Oct_2002\\Images2\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Uniform_28_Oct_2002\\Images2\\ref2_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Uniform_28_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Uniform_28_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Uniform_28_Oct_2002\\Images2\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Uniform_28_Oct_2002\\Images2\\ref2_cam2.raw");

	//Data set Uniform_28_Oct_2002 Images1  hmm
	//optimal threshold x AND x
	//search line 550/550
	m_file_img  =     _T("G:\\Images\\Uniform_28_Oct_2002\\Images1\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Uniform_28_Oct_2002\\Images1\\ref2_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Uniform_28_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Uniform_28_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Uniform_28_Oct_2002\\Images1\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Uniform_28_Oct_2002\\Images1\\ref2_cam2.raw");



	m_file_img  =     _T("G:\\Images\\Plate_18_Oct_2002\\Images4\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Plate_18_Oct_2002\\Images4\\ref1_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Plate_18_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Plate_18_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Plate_18_Oct_2002\\Images4\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Plate_18_Oct_2002\\Images4\\ref2_cam2.raw");


	//Data set Plate_18_Oct_2002 Images3  acctually it is OK
	//optimal threshold 9 AND 5
	//search line 550/550
	m_file_img  =     _T("G:\\Images\\Plate_18_Oct_2002\\Images3\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Plate_18_Oct_2002\\Images3\\ref1_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Plate_18_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Plate_18_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Plate_18_Oct_2002\\Images3\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Plate_18_Oct_2002\\Images3\\ref2_cam2.raw");

	//Data set Plate_18_Oct_2002 Images2  better 
	//optimal threshold 9 AND 5
	//search line 580/580
	m_file_img  =     _T("G:\\Images\\Plate_18_Oct_2002\\Images2\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Plate_18_Oct_2002\\Images2\\ref1_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Plate_18_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Plate_18_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Plate_18_Oct_2002\\Images2\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Plate_18_Oct_2002\\Images2\\ref2_cam2.raw");

	//Data set Plate_18_Oct_2002 Images1  hmmm
	//optimal threshold 9 AND 5
	//search line 580/580
	m_file_img  =     _T("G:\\Images\\Plate_18_Oct_2002\\Images1\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Plate_18_Oct_2002\\Images1\\ref1_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Plate_18_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Plate_18_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Plate_18_Oct_2002\\Images1\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Plate_18_Oct_2002\\Images1\\ref1_cam2.raw");



	//Data set Plate_17_Oct_2002 Images2  too view pixels
	//optimal threshold 8 AND 5
	//search line 600/570
	m_file_img  =     _T("G:\\Images\\Plate_17_Oct_2002\\Images2\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Plate_17_Oct_2002\\Images2\\ref1_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Plate_17_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Plate_17_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Plate_17_Oct_2002\\Images2\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Plate_17_Oct_2002\\Images2\\ref1_cam2.raw");

	//Data set Plate_17_Oct_2002 Images1  too view pixels
	//optimal threshold 8 AND 5
	//search line 600/570
	m_file_img  =     _T("G:\\Images\\Plate_17_Oct_2002\\Images1\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Plate_17_Oct_2002\\Images1\\ref1_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Plate_17_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Plate_17_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Plate_17_Oct_2002\\Images1\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Plate_17_Oct_2002\\Images1\\ref1_cam2.raw");

	//Data set Plate_16_Oct_2002 Images3   is better that Images 1
	//optimal threshold 9 AND 6
	//search line 600/570
	m_file_img  =     _T("G:\\Images\\Plate_16_Oct_2002\\Images3\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Plate_16_Oct_2002\\Images3\\ref1_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Plate_16_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Plate_16_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Plate_16_Oct_2002\\Images3\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Plate_16_Oct_2002\\Images3\\ref1_cam2.raw");


	//Data set Plate_16_Oct_2002 Images2
	//optimal threshold 9 AND 6
	//search line 600/600
	m_file_img  =     _T("G:\\Images\\Plate_16_Oct_2002\\Images2\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Plate_16_Oct_2002\\Images2\\ref1_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Plate_16_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Plate_16_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Plate_16_Oct_2002\\Images2\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Plate_16_Oct_2002\\Images2\\ref1_cam2.raw");


	//Data set Plate_16_Oct_2002 Images1
	//optimal threshold 9 AND 6
	//search line 600/600
	m_file_img  =     _T("G:\\Images\\Plate_16_Oct_2002\\Images1\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Plate_16_Oct_2002\\Images1\\ref1_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Plate_16_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Plate_16_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Plate_16_Oct_2002\\Images1\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Plate_16_Oct_2002\\Images1\\ref1_cam2.raw");


	//Data set Uniform_11_Oct_2002 Images3  : Good, !!!!!!!
	//optimal threshold 8 AND 4
	m_file_img  =     _T("G:\\Images\\Uniform_11_Oct2002\\Images3\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Uniform_11_Oct2002\\Images3\\ref2_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Uniform_11_Oct2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Uniform_11_Oct2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Uniform_11_Oct2002\\Images3\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Uniform_11_Oct2002\\Images3\\ref1_cam2.raw");


	//Data set Uniform_11_Oct_2002 Images2  : Good, use ref2_cam2!!!
	//optimal threshold 8 and 6
	m_file_img  =     _T("G:\\Images\\Uniform_11_Oct2002\\Images2\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Uniform_11_Oct2002\\Images2\\ref1_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Uniform_11_Oct2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Uniform_11_Oct2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Uniform_11_Oct2002\\Images2\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Uniform_11_Oct2002\\Images2\\ref2_cam2.raw");


	m_file_img  =     _T("G:\\Images\\Uniform_11_Oct2002\\Images1\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Uniform_11_Oct2002\\Images1\\ref_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Uniform_11_Oct2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Uniform_11_Oct2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Uniform_11_Oct2002\\Images1\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Uniform_11_Oct2002\\Images1\\ref_cam2.raw");



	//Data set Delta_10_Oct_2002 Images2  ???
	//optimal threshold 10 and 8
	//find line 550 500
	m_file_img  =     _T("G:\\Images\\Delta_10_Oct_2002\\Images2\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta_10_Oct_2002\\Images2\\ref_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Delta_10_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Delta_10_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Delta_10_Oct_2002\\Images2\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta_10_Oct_2002\\Images2\\ref_cam2.raw");


	//Data set Delta_10_Oct_2002 :Not too nice seeding but it works
	//optimal threshold 10 and 8
	//find line 550 500
	m_file_img  =     _T("G:\\Images\\Delta_10_Oct_2002\\Images\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta_10_Oct_2002\\Images\\ref_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Delta_10_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Delta_10_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Delta_10_Oct_2002\\Images\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta_10_Oct_2002\\Images\\ref_cam2.raw");

 	//Data set Delta_2_Oct_2002 :EVEN NICER!!!!!!!!!!!!!!!
	//optimal threshold 10 and 8
	//find line 550 500
	m_file_img  =     _T("G:\\Images\\Delta_2_Oct_2002\\Images2\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta_2_Oct_2002\\Images2\\cam1_ref.raw");
	m_file_calib  =   _T("G:\\Images\\Delta_2_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Delta_2_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Delta_2_Oct_2002\\Images2\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta_2_Oct_2002\\Images2\\cam2_ref.raw");

  	//Data set Delta_2_Oct_2002 :NICE!!!!!!!!!!!!!!!
	//optimal threshold 8 and 8
	//find line 550 500
	m_file_img  =     _T("G:\\Images\\Delta_2_Oct_2002\\Images1\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta_2_Oct_2002\\Images1\\cam1_ref.raw");
	m_file_calib  =   _T("G:\\Images\\Delta_2_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Delta_2_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Delta_2_Oct_2002\\Images1\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta_2_Oct_2002\\Images1\\cam2_ref.raw");	
	  
		
	//Data set Delta_25Sept :seems to work, if nothing better take this
	//optimal threshold 9 and 9
	m_file_img  =     _T("G:\\Images\\Delta_2_Oct_2002\\Images1\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta_2_Oct_2002\\Images1\\cam1_ref.raw");
	m_file_calib  =   _T("G:\\Images\\Delta_2_Oct_2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Delta_2_Oct_2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Delta_2_Oct_2002\\Images1\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta_2_Oct_2002\\Images1\\cam2_ref.raw");


	//Data set Delta_25Sept :seems to work, if nothing better take this
	//optimal threshold 8 and 3
	//optimal  search_line1=550;
	//optimal  search_line2=550
	m_file_img  =     _T("G:\\Images\\Delta_25Sept2002\\Images1\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta_25Sept2002\\Images1\\cam1_ref.raw");
	m_file_calib  =   _T("G:\\Images\\Delta_25Sept2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Delta_25Sept2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Delta_25Sept2002\\Images1\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta_25Sept2002\\Images1\\cam2_ref.raw");
	//Data set Delta_24Sept :seems to work
	//optimal threshold 9 and 9 !!!!!!!!!!!!!!!!!! NOT LESSSSSSSSSSS!!
	//optimal  search_line1=550;
	//optimal  search_line2=550
	m_file_img  =     _T("G:\\Images\\Delta_24Sept2002\\Images1\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta_24Sept2002\\Images1\\cam1_ref.raw");
	m_file_calib  =   _T("G:\\Images\\Delta_24Sept2002\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("G:\\Images\\Delta_24Sept2002\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("G:\\Images\\Delta_24Sept2002\\Images1\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta_24Sept2002\\Images1\\cam2_ref.raw");


	//Data set Delta_23Sept :too many crossing paths
	//optimal threshold 10 and 4
	//optimal  search_line1=550;
	//optimal  search_line2=550
	m_file_img  =     _T("G:\\Images\\Delta_23Sept2002\\Images1\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta_23Sept2002\\Images1\\ref_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Delta_23Sept2002\\Calibration\\cam1_0123.par");
	m_file_calib2 =   _T("G:\\Images\\Delta_23Sept2002\\Calibration\\cam2_0123.par");
	m_file_img2 =     _T("G:\\Images\\Delta_23Sept2002\\Images1\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta_23Sept2002\\Images1\\ref_cam2.raw");


	m_file_img  =     _T("G:\\Images\\Uniform_2Aug2002\\Images\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Uniform_2Aug2002\\Images\\cam1_ref.raw");
	m_file_calib  =   _T("G:\\Images\\Uniform_2Aug2002\\Calibration\\cam1.par");
	m_file_calib2 =   _T("G:\\Images\\Uniform_2Aug2002\\Calibration\\cam2.par");
	m_file_img2 =     _T("G:\\Images\\Uniform_2Aug2002\\Images\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Uniform_2Aug2002\\Images\\cam2_ref.raw");


	//the image pair synchronisation did not work
	m_file_img  =     _T("G:\\Images\\Uniform_31Juli2002\\Images\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Uniform_31Juli2002\\Images\\cam1_ref.raw");
	m_file_calib  =   _T("G:\\Images\\Uniform_31Juli2002\\Calibration\\cam1.par");
	m_file_calib2 =   _T("G:\\Images\\Uniform_31Juli2002\\Calibration\\cam2.par");
	m_file_img2 =     _T("G:\\Images\\Uniform_31Juli2002\\Images\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Uniform_31Juli2002\\Images\\cam2_ref.raw");


	//Data set Uniform_30Juli2002
	//optimal threshold 10 and 6
	//optimal  search_line1=550;
	//optimal  search_line2=450;
	m_file_img  =     _T("G:\\Images\\Uniform_30Juli2002\\Images\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Uniform_30Juli2002\\Images\\cam1_ref.raw");
	m_file_calib  =   _T("G:\\Images\\Uniform_30Juli2002\\Calibration\\cam1.par");
	m_file_calib2 =   _T("G:\\Images\\Uniform_30Juli2002\\Calibration\\cam2.par");
	m_file_img2 =     _T("G:\\Images\\Uniform_30Juli2002\\Images\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Uniform_30Juli2002\\Images\\cam2_ref.raw");


	m_file_img  =     _T("G:\\Images\\CalStick29_Mai02\\Images\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\CalStick29_Mai02\\Images\\cam1_ref.raw");
	m_file_calib  =   _T("G:\\Images\\CalStick29_Mai02\\Calibration\\cam1.par");
	m_file_calib2 =   _T("G:\\Images\\CalStick29_Mai02\\Calibration\\cam2.par");
	m_file_img2 =     _T("G:\\Images\\CalStick29_Mai02\\Images\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\CalStick29_Mai02\\Images\\cam2_ref.raw");

	//Data set Delta3_Juni02 has nice long path lines
	//optimal threshold 14 and 6
	//optimal  search_line1=640;
	//optimal  search_line2=640;
	m_file_img  =     _T("G:\\Images\\Delta3_Juni02\\Images6\\cam10000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta3_Juni02\\Images6\\cam1_ref.raw");
	m_file_calib  =   _T("G:\\Images\\Delta3_Juni02\\Calibration\\cam1.par");
	m_file_calib2 =   _T("G:\\Images\\Delta3_Juni02\\Calibration\\cam2.par");
	m_file_img2 =     _T("G:\\Images\\Delta3_Juni02\\Images6\\cam20000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta3_Juni02\\Images6\\cam2_ref.raw");

	//Data set Delta19_Juni has nice long path lines
	//optimal threshold 12
	//optimal  search_line1=550;
	//optimal  search_line2=420;
	m_file_img  =     _T("G:\\Images\\Delta19_Juni\\Img_Delta_10ms\\cam10000.raw");
	m_file_img2 =     _T("G:\\Images\\Delta19_Juni\\Img_Delta_10ms\\cam20000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta19_Juni\\Img_Delta_10ms\\cam1_ref.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta19_Juni\\Img_Delta_10ms\\cam2_ref.raw");
	m_file_calib  =   _T("G:\\Images\\Delta19_Juni\\Calibration\\cam1.par");
	m_file_calib2 =   _T("G:\\Images\\Delta19_Juni\\Calibration\\cam2.par");


	//Data set Delta1_Juni has nice long path lines
	//optimal threshold 9,9
	//optimal  search_line1=550;
	//optimal  search_line2=390;
	m_file_img  =     _T("G:\\Images\\Delta1_Juni\\Images\\cam1000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta1_Juni\\Images\\ref_cam1.raw");
	m_file_calib  =   _T("G:\\Images\\Delta1_Juni\\Calibration\\cam1_03.par");
	m_file_calib2 =   _T("G:\\Images\\Delta1_Juni\\Calibration\\cam2_03.par");
	m_file_img2 =     _T("G:\\Images\\Delta1_Juni\\Images\\cam2000.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta1_Juni\\Images\\ref_cam2.raw");

	m_file_img  =     _T("F:\\Images\\NACA9_Oct\\Images3\\cam1000.raw");
	m_file_ref_img  = _T("F:\\Images\\NACA9_Oct\\Images3\\cam1_ref.raw");
	m_file_calib  =   _T("F:\\Images\\NACA9_Oct\\Calibration\\cam1_01234.par");
	m_file_calib2 =   _T("F:\\Images\\NACA9_Oct\\Calibration\\cam2_01234.par");
	m_file_img2 =     _T("F:\\Images\\NACA9_Oct\\Images3\\cam2000.raw");
	m_file_ref_img2 = _T("F:\\Images\\NACA9_Oct\\Images3\\cam1_ref.raw");

    //long, good visible path lines, straight and boring
	m_file_img  =     _T("G:\\Images\\Delta28_Mai\\Images\\cam1000.raw");
	m_file_img2 =     _T("G:\\Images\\Delta28_Mai\\Images\\cam2000.raw");
	m_file_ref_img  = _T("G:\\Images\\Delta28_Mai\\Images\\ref_cam1.raw");
	m_file_ref_img2 = _T("G:\\Images\\Delta28_Mai\\Images\\ref_cam2.raw");
	m_file_calib  =   _T("G:\\Images\\Delta28_Mai\\Calibration\\cam1_03.par");
	m_file_calib2 =   _T("G:\\Images\\Delta28_Mai\\Calibration\\cam2_03.par");

    m_file_img  =     _T("G:\\Images\\Plate1_July\\Images_6ms\\cam10000.raw");
	m_file_img2 =     _T("G:\\Images\\Plate1_July\\Images_6ms\\cam20000.raw");
	m_file_ref_img  = _T("G:\\Images\\Plate1_July\\Images_6ms\\ref_cam1.raw");
	m_file_ref_img2 = _T("G:\\Images\\Plate1_July\\Images_6ms\\ref_cam2.raw");
	m_file_calib  =   _T("G:\\Images\\Plate1_July\\Calibration\\cam1.par");
	m_file_calib2 =   _T("G:\\Images\\Plate1_July\\Calibration\\cam2.par");


    m_file_img  =     _T("F:\\Images\\Naca11_July\\NACA0012\\cam10000.raw");
	m_file_img2 =     _T("F:\\Images\\Naca11_July\\NACA0012\\cam20000.raw");
	m_file_ref_img  = _T("F:\\Images\\Naca11_July\\NACA0012\\ref_cam1.raw");
	m_file_ref_img2 = _T("F:\\Images\\Naca11_July\\NACA0012\\ref_cam2.raw");
	m_file_calib  =   _T("F:\\Images\\Naca11_July\\Calibration\\cam1.par");
	m_file_calib2 =   _T("F:\\Images\\Naca11_July\\Calibration\\cam2.par");
	
	//optimal thresh for this sequence 13/13
 	m_file_img  =     _T("G:\\Images\\Naca11_July\\Images_15ms\\cam10000.raw");
	m_file_img2 =     _T("G:\\Images\\Naca11_July\\Images_15ms\\cam20000.raw");
	m_file_ref_img  = _T("G:\\Images\\Naca11_July\\Images_15ms\\ref2_cam1.raw");
	m_file_ref_img2 = _T("G:\\Images\\Naca11_July\\Images_15ms\\ref2_cam2.raw");
	m_file_calib  =   _T("G:\\Images\\Naca11_July\\Calibration\\cam1.par");
	m_file_calib2 =   _T("G:\\Images\\Naca11_July\\Calibration\\cam2.par");

    m_file_img  =     _T("F:\\Images\\Naca11_July\\Images_23ms\\cam10000.raw");
	m_file_img2 =     _T("F:\\Images\\Naca11_July\\Images_23ms\\cam20000.raw");
	m_file_ref_img  = _T("F:\\Images\\Naca11_July\\Images_23ms\\ref2_cam1.raw");
	m_file_ref_img2 = _T("F:\\Images\\Naca11_July\\Images_23ms\\ref2_cam2.raw");
	m_file_calib  =   _T("F:\\Images\\Naca11_July\\Calibration\\cam1.par");
	m_file_calib2 =   _T("F:\\Images\\Naca11_July\\Calibration\\cam2.par");
*/

void CStereo2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStereo2Dlg)
	DDX_Control(pDX, IDC_ANALYZE_IMG1, m_analyze_img);
	DDX_Control(pDX, IDC_IMG_NR_SPIN, m_an_img_nr);
	DDX_Control(pDX, IDC_PROGRESS1, m_control_bar);
	DDX_Control(pDX, IDC_STATUS, m_status);
	DDX_Text(pDX, IDC_FIRST_IMG, m_first_img);
	DDV_MinMaxInt(pDX, m_first_img, 0, 4000);
	DDX_Text(pDX, IDC_LAST_IMG, m_last_img);
	DDV_MinMaxInt(pDX, m_last_img, 0, 5000);
	DDX_Text(pDX, IDC_THRES_PTV, m_thres_ptv);
	DDV_MinMaxInt(pDX, m_thres_ptv, 0, 255);
	DDX_Text(pDX, IDC_FILE_IMG, m_file_img);
	DDV_MaxChars(pDX, m_file_img, 255);
	DDX_Text(pDX, IDC_FILE_REF_IMG1, m_file_ref_img);
	DDV_MaxChars(pDX, m_file_ref_img, 255);
	DDX_Text(pDX, IDC_FILE_CALIB, m_file_calib);
	DDV_MaxChars(pDX, m_file_calib, 255);
	DDX_Text(pDX, IDC_FILE_CALIB2, m_file_calib2);
	DDV_MaxChars(pDX, m_file_calib2, 255);
	DDX_Text(pDX, IDC_FILE_IMG2, m_file_img2);
	DDV_MaxChars(pDX, m_file_img2, 255);
	DDX_Text(pDX, IDC_FILE_REF_IMG2, m_file_ref_img2);
	DDV_MaxChars(pDX, m_file_ref_img2, 255);
	DDX_Text(pDX, IDC_THRES_PTV2, m_thres_ptv2);
	DDV_MinMaxInt(pDX, m_thres_ptv2, 0, 255);
	DDX_Text(pDX, IDC_IMG_NR, m_img_nr);
	DDV_MinMaxInt(pDX, m_img_nr, 0, 5000);
	DDX_Text(pDX, IDC_THRESH_ANALYZE, m_thresh_analyze);
	DDV_MinMaxInt(pDX, m_thresh_analyze, 0, 255);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStereo2Dlg, CDialog)
	//{{AFX_MSG_MAP(CStereo2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RUN, OnRun)
	ON_BN_CLICKED(IDC_VMRL, OnVmrl)
	ON_BN_CLICKED(IDC_OPEN_IMG_FILE, OnOpenImgFile)
	ON_BN_CLICKED(IDC_OPEN_FILE_REF_IMG, OnOpenFileRefImg)
	ON_BN_CLICKED(IDC_OPEN_CAM_PAR, OnOpenCamPar)
	ON_EN_CHANGE(IDC_IMG_NR, OnAnalyze)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_ANALYZE_IMG2, OnAnlyseImg)
	ON_BN_CLICKED(IDC_CHG_COORD, OnChgCoord)
	ON_BN_CLICKED(IDC_OPEN_CAM_PAR2, OnOpenCamPar)
	ON_BN_CLICKED(IDC_OPEN_IMG_FILE2, OnOpenImgFile)
	ON_BN_CLICKED(IDC_OPEN_FILE_REF_IMG2, OnOpenFileRefImg)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStereo2Dlg message handlers

BOOL CStereo2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_1BOUTBOX must be in the system command range.
//	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
//	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
//			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	analyze_img=1;
	changed=0;
	running=0;
	m_analyze_img.SetCheck(1);
	UpdateData(TRUE);

	return TRUE;  
}

void CStereo2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
/*	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
	*/
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStereo2Dlg::OnPaint() 
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
HCURSOR CStereo2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}




void CStereo2Dlg::OnRun() 
{
	running=TRUE;
	int ans=0;
	int i, step;
     
    device=this->GetDC();


//****Reset the status window*********************************************
	m_status.SetSel(0,-1,FALSE);
	m_status.Clear();
//************************************************************************
	char file_path_result[255],file_result[255];
	char file_path1[255],file_path2[255],file1[255],file2[255];	
	char file_cam_par1[255],file_cam_par2[255];	
	char file_ccd_par1[255],file_ccd_par2[255]; 
	char file_lut_par1[255],file_lut_par2[255]; 
	char file_img1[255],file_img2[255];
	char file_ref_img1[255],file_ref_img2[255];
	GetDlgItemText(IDC_FILE_CALIB,file_path1,255);
	GetDlgItemText(IDC_FILE_CALIB2,file_path2,255);
	GetDlgItemText(IDC_FILE_IMG,file_img1,255);
	GetDlgItemText(IDC_FILE_IMG2,file_img2,255);
	GetDlgItemText(IDC_FILE_REF_IMG1,file_ref_img1,255);				
	GetDlgItemText(IDC_FILE_REF_IMG2,file_ref_img2,255);
	char *c=file_path1+strlen(file_path1);
	while(*c != '\\')
		*c--;
	strcpy(file1,++c);
	*c='\0';
	c=file1+strlen(file1);
	while(*c != '.')
		*c--;
	*c='\0';

	c=file_path2+strlen(file_path2);
	while(*c != '\\')
		*c--;
	strcpy(file2,++c);
	*c='\0';
	c=file2+strlen(file2);
	while(*c != '.')
		*c--;
	*c='\0';
//************************************************************************
	strcpy(file_path_result,file_path1);
	c=file_path_result+strlen(file_path_result)-2;
	while(*c != '\\')
		*c--;
	*c='\0';
	strcat(file_path_result,"\\Result\\");


	strcpy(&file_result[0],file_path_result);
	strcat(file_result,"path1.dat");
	f_path1=fopen(file_result,"wb");
	if(f_path1 == NULL || ferror(f_path1))  
	{	
		MessageBox(0,"Could not open Folder \"Result\"\nCreate Folder and press OK", 0);
		if(f_path1 == NULL || ferror(f_path1))  
		{	
			MessageBox(0,"Could not open Folder \"Result\"\n STOP", 0);
			return;
		}
	}
	strcpy(&file_result[0],file_path_result);
	strcat(file_result,"path2.dat");
	f_path2=fopen(file_result,"wb");
	strcpy(&file_result[0],file_path_result);
	strcat(file_result,"spline1.dat");
	f_spline1=fopen(file_result,"wb");
	strcpy(&file_result[0],file_path_result);
	strcat(file_result,"spline2.dat");
	f_spline2=fopen(file_result,"wb");
	strcpy(&file_result[0],file_path_result);
	strcat(file_result,"3Dpoints.dat");
	f_global=fopen(file_result,"wb");
	strcpy(&file_result[0],file_path_result);
	strcat(file_result,"3Dspline_points.dat");
	f_spline_global=fopen(file_result,"wb");
	strcpy(&file_result[0],file_path_result);
	strcat(file_result,"contour_dev.m");
	fp1 = fopen (file_result,"wb");
	strcpy(&file_result[0],file_path_result);
	strcat(file_result,"contour_x.m");
	fp2 = fopen (file_result,"wb");
	strcpy(&file_result[0],file_path_result);
	strcat(file_result,"contour_y.m");
	fp3 = fopen (file_result,"wb");



	strcpy(file_cam_par1,file_path1);
	strcpy(file_cam_par2,file_path2);
	strcpy(file_ccd_par1,file_path1);
	strcpy(file_ccd_par2,file_path2);
	strcpy(file_lut_par1,file_path1);
	strcpy(file_lut_par2,file_path2);
	strcat(file_ccd_par1,"ccd_par1.ccd");
	strcat(file_ccd_par2,"ccd_par2.ccd");
	strcat(file_cam_par1,file1);
	strcat(file_cam_par2,file2);
	strcat(file_lut_par1,file1);
	strcat(file_lut_par2,file2);
	strcat(file_cam_par1,".par");
	strcat(file_cam_par2,".par");
	strcat(file_lut_par1,".lut");
	strcat(file_lut_par2,".lut");

	char num_form1[9];
	strcpy(num_form1,"1%0");
	char a;
	itoa(img_name_zeros-5,&a,10);
	strcpy(&num_form1[3],&a);
	strcpy(&num_form1[4],"d.raw");
	char num_form2[9];
	strcpy(num_form2,"2%0");
	itoa(img_name_zeros-5,&a,10);
	strcpy(&num_form2[3],&a);
	strcpy(&num_form2[4],"d.raw");
//************************************************************************
	Plot::display(-99,"Welcome", &m_status);

 
	if(camera1.init == FALSE)
	{
		if(camera1.Load_look_up(file_lut_par1, &m_control_bar, 1))
		{
			Plot::display(-99,"Error, can not finde LUT 1", &m_status);
			return;
		}
		else
			Plot::display(-99,"First LUT loaded", &m_status);

		camera1.Read_cam_par(file_cam_par1);
		if(camera1.Read_ccd_par(file_ccd_par1)==FALSE)
		{
			MessageBox("Can not open CCD parameter 1 file");
			return;
		}
		camera1.Get_transf_matrix();
		camera1.Get_camera_matrix();
		camera1.init = TRUE;
	}

	if(camera2.init == FALSE)
	{
		if(camera2.Load_look_up(file_lut_par2, &m_control_bar, 2))
		{
			Plot::display(1,"Error, can not finde LUT 2", &m_status);
			return;
		}
		else
			Plot::display(-99,"Second LUT loaded", &m_status);

		camera2.Read_cam_par(file_cam_par2);
		if(camera2.Read_ccd_par(file_ccd_par2)==FALSE)
		{
			MessageBox("Can not open CCD parameter 2 file");
			return;
		}
		camera2.Get_transf_matrix();
		camera2.Get_camera_matrix();
		camera2.init = TRUE;
	}


	//this function gets the matrix K which describes the projection of 
	//world cooridinates into normalized coordinates
	Camera::Get_triangulation_matrix(camera1, camera2);
	//the essental matrix for the projection from view1 into view2
	camera1.Get_essential(camera2.R, camera2.t);
	//the essental matrix for the projection from view2 into view1
	camera2.Get_essential(camera1.R, camera1.t);
//*****initialize memory for images***************************************
	for(i=0; i<(3 * camera1.ccd.height*camera1.ccd.width); i++)
	{
		colimagedata1[i] = 255;
		colimagedata2[i] = 255;
	}
	for(i=0; i<(camera1.ccd.height*camera1.ccd.width); i++)
	{
		imagedatacam1[i] = 0;
		imagedatacam2[i] = 0;
		refimagedatacam1[i] = 0;
		refimagedatacam2[i] = 0;
	}
//************************************************************************
//*****Read reference image***********************************************
	if(Image::Read(file_ref_img1, refimagedatacam1))
	{
		Plot::display(1,"Error reading ref img: ", &m_status);
		return;
	}
	if(Image::Read(file_ref_img2, refimagedatacam2))
	{
		Plot::display(2,"Error reading ref img: ", &m_status);
		return;
	}
//************************************************************************
 
//************************************************************************
//**********   Define the search box for the first path segment  *********
	int search_area1_nr=480;
	int search_area2_nr=480;

	Pixel *search_area1 = new Pixel[search_area1_nr];
	Pixel *search_area2 = new Pixel[search_area2_nr];

	for(i=0;i<480 ; i++)
	{
		search_area1[i].x=500;
		search_area1[i].y=i;
	}
	for(i=0;i<480; i++)
	{
		search_area2[i].x=500;
		search_area2[i].y=i;
	}

	//plot search area
	Plot::Pixels(device, colimagedata1, search_area1, search_area1_nr, 20, 140, 70);
	Plot::Pixels(device, colimagedata2, search_area2, search_area2_nr, 20, 140, 70);
//************************************************************************

	T_S=0;
	Plot::display(-99,"Tracking...", &m_status);	
	UpdateData(TRUE);

	char img_file_cam[255]; 
	strcpy(img_file_cam , m_file_img);
	const int l_str = strlen(img_file_cam)-img_name_zeros;
	m_control_bar.SetRange(m_first_img, m_last_img+1);
	m_control_bar.SetStep(1);

	PathList pathlist1;
	PathList pathlist2;

	//Array of pointers to paths which are matched 
	//and are waiting to be used for 3D calcualation
	PathList pathlist_matched;

	SplineList splinelist1;
	SplineList splinelist2;

	int	nr_global=0;
	int path_nr1_tot=0, path_nr1=0;
	int path_nr2_tot=0, path_nr2=0;

	//1/2-1 the size of the search box to detect new path elements
	const int search_box1=5;
	const int search_box2=5;

	//parameter indicating from which path element the 3D reconstruction is started
	//idealy first_segment=0 is used. However if the first segment is partially
	//cover the result is not accurate
	const int first_segment=1;
	// same applies for the last element, see declaration of Dspline!!
	//parameter which describes the number of measurements points (nodes) generated (r)
	const double node_spacing=0.1;//0.05 is a good value
	//parameter which describes the interval at which the knots are generated (r)
	const double knots_spacing=0.8;//0.8 was good!
	//parameter which describes the initial interval at which the knots are generated (r) (1.5)
	const double init_knots_spacing=1.4;
	//parameter which describes the points generated for the 3-D calculation (r)
	const double pair_inkrement=0.1;
	//parameter controling the correspondence tolerance
	const double epi_tolerance=4*0.009/camera2.in.focus;
	//const double epi_tolerance=100*0.009/camera2.in.focus;
	//test_spline();

	for(step=m_first_img; step <= m_last_img; step++)
	{
		Plot::display(T_S,"Time step:", &m_status);
		m_control_bar.StepIt();
		//*****Read actual image**************************************************

		sprintf(img_file_cam+l_str, num_form1, step);
		if(Image::Read(img_file_cam, imagedatacam1))
		{
			Plot::display(1,"Error reading img: ", &m_status);	return;
		}
		sprintf(img_file_cam+l_str, num_form2, step);
		if(Image::Read(img_file_cam, imagedatacam2))
		{
			Plot::display(2,"Error reading img: ", &m_status);	return;
		}
		//******End Read actual image*********************************************

		Image::Process(device, imagedatacam1, refimagedatacam1, 1, m_thres_ptv);
		Image::Process(device, imagedatacam2, refimagedatacam2, 2, m_thres_ptv2);

		//********************Search for the next path_segments (VIEW 1) ***********************************
		//**************************************************************************************************
		for(int p_index=0; p_index < pathlist1.number; p_index++)
		{
			PathListElem *p_element=pathlist1[p_index];
			Path *p=p_element->element;

			int next_found=0,i=0;
			Path_seg *path_seg = p->path_seg + p->nr_seg-1;
			if(p->nr_seg > MAX_PATH_SEG)
			{
				next_found=0;
				Plot::display(1,"MAX_PATH_SEG in view: ", &m_status);
			}
			else
			{
				//the neighbourhood of all endpoints of the segment is search for new segments
				//until a segment is found. Except if the path is marked already finished
				//The search is done in a square area around the endpoints
				while(path_seg->end_point_index[i] != -1 && next_found==0 && p->finished==FALSE)
				{
					int index= path_seg->end_point_index[i];
					int i_s, j_s, i1, j1, i2, j2;
					if((i1 = path_seg->pixel[index].x-search_box1) < 0)     i1  = 0; 
					if((i2 = path_seg->pixel[index].x+search_box1) > 640-1) i2  = 640-1;
					if((j1 = path_seg->pixel[index].y-search_box1) < 0)     j1  = 0; 
					if((j2 = path_seg->pixel[index].y+search_box1) > 480-1) j2  = 480-1;
					for(i_s=i1; i_s<i2; i_s++)
					{
						for(j_s=j1; j_s<j2; j_s++)
						{
							if(*(imagedatacam1+i_s+640*j_s) == 255)
								next_found=p->Get_contour_point(path_seg+1, imagedatacam1, i_s,j_s);
							

							if(next_found>5)
							{
								p->nr_seg++;
								Plot::Pixels(device, colimagedata1, p->path_seg[p->nr_seg-1].pixel, p->path_seg[p->nr_seg-1].p_nr, 0, 10+p->nr_seg*40, 140-p->nr_seg*40);
								ans=p->Get_conture_curvature_a(path_seg+1);
								if(ans==-5){Plot::display(1," MAX_END_P in Get_cont.. ", &m_status); return;}
								//plot the leading endpoint
								Plot::Points(device, colimagedata1, &path_seg->pixel[path_seg->end_point_index[i]], 1, 0, 0, 0);
								//normalize the endpoints
								camera1.Normalize_look_up(path_seg+1);

								if(CONTR)
								{
									if(p->Get_property(path_seg+1))
									{
										Path_seg *ptset=path_seg+1;
										Plot::Circles(device, colimagedata1, &ptset->centroid, 1, 0, 0, 255);
									}
								}
								if(i>0)
								{
									//sort the endpoints of the old path segment
									int  dum = path_seg->end_point_index[0];
									path_seg->end_point_index[0]=path_seg->end_point_index[i];
									path_seg->end_point_index[1]=dum;
								}
							}
							if(next_found>0)	break;
						}
						if(next_found>0)	break;
					}
					i++;
				}
			}
			//*********Process Finished Paths****** (VIEW 1) **********************************************
			//*********************************************************************************************
			if(next_found==0 && p->finished == FALSE )  //change this line if CONTR==1&& p->matched
			{
				//Plot::display(T_S,"Finished path (view 1) proc in:", &m_status);
				//Plot::display(p->nr_seg,"   number of segments: ", &m_status);
				//this is done only if the path could be matched, otherwise this procedure is meaningless
				p->finished=TRUE;
				if(p->nr_seg>4)
				{
					//if the path is ready to be used for the 3D calculation put it the list***************
					if(p->matched)
						pathlist_matched.add(p);
					//*************************************************************************************
					//assign the path to the corresponding spline
					Spline *spline1 = splinelist1.create();
					p->cor_spline=spline1;

					//normalize the path's pixel
					camera1.Normalize_look_up(p);
					p->Get_hproperty();

					if(spline1->Init_hnode_all(p)==FALSE){
						Plot::display(-99,"Too many nots (2500)",&m_status); return;}
				
					if(spline1->Generate_knots(init_knots_spacing)==1){
						Plot::display(-99,"Too many control points (200)",&m_status); return;}
					spline1->Get_ctrl_points();
					//plot initial spline
					if(CONTR)
					{
						CamPoint P;
						
						for(double r=0.1; r <= spline1->Max_parameter(); r+=0.01)
						{
							spline1->Get_point(&P, r);
							P=camera1.Transform_nor2img(P);
							if(r+0.0000000001-floor(r+0.0000000001)<0.00001)
								Plot::Points(device, colimagedata1, &P, 1, 0, 0, 250);
							else
								Plot::Pixels(device, colimagedata1, &P, 1,  250, 0, 100);
						}
					}
					//make the B-spline approximation better 
					//1. calculate the slope at more parameter values
					if(SPLINE_ITERATION)
					{
						spline1->Get_derivative_coeff();
						//genetate new measurement data
						for(double r=0; r <= spline1->Max_parameter(); r+=node_spacing)
						{
							int err=spline1->Get_new_hnode(p, r, camera1.in.focus);
							if(err==2){
								Plot::display(-99,"Error view1, new_nodes>=2500",&m_status); return;}
						}

						if(spline1->Generate_knots(knots_spacing)==1){
								Plot::display(-99,"Too many control points (200)",&m_status); return;}
						spline1->Get_ctrl_points();
						//plot spline
						if(CONTR)
						{
							//spline1->Plot_node_points(colimagedata1);
							//spline1->Plot_ctrl_points(colimagedata1);
							for(double r=0; r <= spline1->Max_parameter(); r+=0.01)
							{
								CamPoint P;
								spline1->Get_point(&P, r);
								P=camera1.Transform_nor2img(P);
								Plot::Pixels(device, colimagedata1, &P, 1, 0, 0, 255);
							}
						}
					}//if(0)
				}
			}
			//*********End Process Finished Paths****** (VIEW 1) *******************************************
			//**********************************************************************************************
		}
		//************Second view***************************************************************************
		//**************************************************************************************************
		for(p_index=0; p_index < pathlist2.number; p_index++)
		{
			PathListElem *p_element=pathlist2[p_index];
			Path *p=p_element->element;

			int next_found=0,i=0;
			Path_seg *path_seg = p->path_seg + p->nr_seg-1;
			if(p->nr_seg > MAX_PATH_SEG)
			{
				next_found=0;
				Plot::display(2,"MAX_PATH_SEG in view: ", &m_status);
			}
			else
			{
				while(path_seg->end_point_index[i] != -1 && next_found==0 && p->finished==FALSE)
				{
					int index= path_seg->end_point_index[i];
					int i_s, j_s, i1, j1, i2, j2;
					if((i1 = path_seg->pixel[index].x-search_box2) < 0)     i1  = 0; 
					if((i2 = path_seg->pixel[index].x+search_box2) > 640-1) i2  = 640-1;
					if((j1 = path_seg->pixel[index].y-search_box2) < 0)     j1  = 0; 
					if((j2 = path_seg->pixel[index].y+search_box2) > 480-1) j2  = 480-1;
					for(i_s=i1; i_s<i2; i_s++)
					{
						for(j_s=j1; j_s<j2; j_s++)
						{
							if(*(imagedatacam2+i_s+640*j_s) == 255)
								next_found=p->Get_contour_point(path_seg+1, imagedatacam2, i_s,j_s);

							if(next_found>5)
							{
								p->nr_seg++;
								Plot::Pixels(device, colimagedata2, p->path_seg[p->nr_seg-1].pixel, p->path_seg[p->nr_seg-1].p_nr, 0, 10+p->nr_seg*40, 140-p->nr_seg*40);
								ans=p->Get_conture_curvature_a(path_seg+1);
								if(ans==-5){Plot::display(1," MAX_END_P in Get_cont.. ", &m_status); return;}
								//plot the leading endpoint
								Plot::Points(device, colimagedata2, &path_seg->pixel[path_seg->end_point_index[i]], 1, 0, 0, 0);
								//normalize the endpoints
								camera2.Normalize_look_up(path_seg+1);

								if(CONTR)
								{
									if(p->Get_property(path_seg+1))
									{
										Path_seg *ptset=path_seg+1;
										Plot::Circles(device, colimagedata2, &ptset->centroid, 2, 0, 0, 255);
									}
								}
								if(i>0)
								{
									//sort the endpoints of the old path segment
									int dum = path_seg->end_point_index[0];
									path_seg->end_point_index[0]=path_seg->end_point_index[i];
									path_seg->end_point_index[1]=dum;
								}
							}
							if(next_found>0)	break;
						}
						if(next_found>0)	break;
					}
					i++;
				}
			}
			//*********Process Finished Paths****(second view)*********************************************
			//*********************************************************************************************
			if(next_found==0 && p->finished == FALSE)
			{
				p->finished=TRUE;
				if(p->nr_seg>4)
				{
					double r;
					//assign the path to the corresponding spline
					Spline *spline2 = splinelist2.create();
					p->cor_spline=spline2;
					//normalize the path's pixel
					camera2.Normalize_look_up(p);
					p->Get_hproperty();
					
					spline2->Init_hnode_all(p);

					if(spline2->Generate_knots(init_knots_spacing)==1){
							Plot::display(-99,"Too many control points (200)",&m_status); return;}
					spline2->Get_ctrl_points();
					//plot initial spline
					if(CONTR)
					{
						CamPoint P;
						for(r=0.1; r <= spline2->Max_parameter(); r+=0.01)
						{
							spline2->Get_point(&P, r);
							P=camera2.Transform_nor2img(P);
							if(r+0.0000000001-floor(r+0.0000000001)<0.00001)
								Plot::Points(device, colimagedata2, &P, 1, 0, 0, 250);
							else
								Plot::Pixels(device, colimagedata2, &P, 1, 250, 0, 100);
						}
					}
					//make the B-spline approximation better
					//1. calculate the slope at more parameter values
					if(SPLINE_ITERATION)
					{
						spline2->Get_derivative_coeff();
						for(r=0; r <= spline2->Max_parameter(); r+=node_spacing)
						{
							int err=spline2->Get_new_hnode(p, r, camera2.in.focus);
							if(err==2){
								Plot::display(-99,"Error view2, new_nodes>=2500",&m_status); return;}
						}

						if(spline2->Generate_knots(knots_spacing)==1){
								Plot::display(-99,"Too many control points (200)",&m_status); return;}
						spline2->Get_ctrl_points();
						//plot spline
						if(CONTR)
						{
							//spline2->Plot_node_points(colimagedata2);
							//spline2->Plot_ctrl_points(colimagedata2);
							for(r=0; r <= spline2->Max_parameter(); r+=0.01)
							{
								CamPoint P;
								spline2->Get_point(&P, r);
								P=camera2.Transform_nor2img(P);
								Plot::Pixels(device, colimagedata2, &P, 1, 0, 0, 255);
							}
						}
					}//if(0)
					//***************                                                        ***************
				}
			}
			//*********End Process Finished Paths***********************************************************
			//**********************************************************************************************
		}
		//********************End Search for the next path_segments*****************************************
		//**************************************************************************************************

		//**************************************************************************************************
		//*********Find corresponding paths*****************************************************************
		//**************************************************************************************************
		//the search is done as soon as a path segment is found. If the end points are matched, the path
		//is declared as matched; otherwise matching of endpoints is tried for the next time steps.
		//As soon as one of the paths is declared as finished, matching not possible anymore
		//this is controlled by looping through the growing paths: first_grow1 until last_grow1
		//Set the matching tolerance to two pixel
		Path *p1, *p2;

		for(int p_index1=0; p_index1 < pathlist1.number; p_index1++)
		{
			PathListElem *p_element=pathlist1[p_index1];
			p1=p_element->element;	
			
			if(!p1->matched)
			{
				EpiPar epi;
				Path_seg *seg1;
				seg1=p1->path_seg+p1->nr_seg-1;

				//project (end-)point of path seg from view 1 into view 2
				camera1.Get_epi(&epi, seg1->hpixel[seg1->end_point_index[0]]);

				double nenner = sqrt(epi.a * epi.a + epi.b * epi.b);
				//search for possible correspondences in view two
				for(int p_index2=0; p_index2 < pathlist2.number; p_index2++)
				{
					PathListElem *p_element=pathlist2[p_index2];
					p2=p_element->element;

					if(!p2->matched)
					{
						Path_seg *seg2;
						seg2=p2->path_seg+p2->nr_seg-1;

						double distance  =fabs(epi.a * seg2->hpixel[seg2->end_point_index[0]].x + 
										       epi.b * seg2->hpixel[seg2->end_point_index[0]].y +
										       epi.c)  / nenner;

						camera2.Plot_epi(device,colimagedata2,epi.a,epi.b,epi.c);
						if(distance < epi_tolerance)
						{	
							for(i=0;i<4;i++)
							{
								if(p1->correspondence[i]==p2 || p1->correspondence[i]==0)
								{
									p1->correspondence[i]=p2;
									p1->i_match[i]++;

									if(p1->i_match[i] > 0)
									{
										Plot::display(T_S,"Path matched  :", &m_status);	
										//camera2.Plot_epi(device,colimagedata2,epi.a,epi.b,epi.c);
										int index1= seg1->end_point_index[0];
										int index2= seg2->end_point_index[0];
										Plot::Number(device,colimagedata1,seg1->pixel[index1].x,seg1->pixel[index1].y, p1->nr_seg-1);
										Plot::Number(device,colimagedata2,seg2->pixel[index2].x,seg2->pixel[index2].y, p2->nr_seg-1);

										p1->correspondence[0]=p2;
										p2->correspondence[0]=p1;
										p1->matched=1;
										p2->matched=1;
										//synchronize the two views; the path1 segment p1->nr_seg corresponds to p2->nr_seg.
										p1->r_corr=p1->nr_seg-1;
										p2->r_corr=p2->nr_seg-1;
										break;
									}
								}
							}
						}
					}
				}
			}
		}	
		//*********End Find corresponding paths*************************************************************
		//**************************************************************************************************


		//*********Project the points from spline 1 into view 2 to get corresponding points*****************
		//*********on spline********************************************************************************
		//*********Correspondence search must be done in normalized space***********************************
		//for that purpose both splines must be calculated 
		//This is done only when both splines are calculated -> both paths have to be marked as finished
		//matched paths are not nececeraly finished!
		//only paths with a certain number of elements are procesed
		//the "pathlist" contains all matched and finished paths with more than 4 segments from the first view
		if(pathlist_matched.first_path != 0)
		{
			for(int ip1=0; ip1<pathlist_matched.number; ip1++)
			{
				PathListElem  *pl_elem = pathlist_matched[ip1];
				p1 = pl_elem->element;
				Path *p2=p1->correspondence[0];
				if(p2->finished && p2->nr_seg>4)
				{
					Spline *spl1=p1->cor_spline;
					Spline *spl2=p2->cor_spline;
					int r1=p1->r_corr;
					int r2=p2->r_corr;
					int t_start;
					//get the maximal parameter common to both splines
					double r_max=Get_r_max(&r1, &r2, &t_start, p1->t_start, p2->t_start, spl1->Max_parameter(), spl2->Max_parameter())+1;
					EpiPar epi;
					CamPoint hp1, hp2;
					i=0;
					Global global_points;
					global_points.nr_p=0;
					GlobalPoint *gpoint = global_points.point;
					if(PROJECTIVE==TRUE)
					{
						//point pair in two views are build by projecting points from the 
						//first view into the second view. This epipolar lines are intersected
						//with the spline in the second view to build corresponding points.
						static bool first_sect;
						static double r_last;
						r_last=r2;
						first_sect=0;
						int ans_sect;
						for(double r=0; r <= r_max; r+=pair_inkrement)
						{
							if(i++>=MAX_H_POINT){
								Plot::display(-99,"MAX_H_POINT", &m_status); return;}
							//*****Define (normalized)Points on the path spline1  to build corresponding pairs*
							//*********************************************************************************
							hp1.w=1;
							p1->cor_spline->Get_point(&hp1, r+r1);
							//*****get the projection into view two********************************************
							camera1.Get_epi(&epi, hp1);
							//if((int)r%2 ==0)
							//	camera2.Plot_epi(device,colimagedata2, epi.a, epi.b, epi.c);
							//get the intersection with spline2
							//the intersection is done only segment-wise
							if(r_last < ((int)r+2*pair_inkrement))
								r_last=(int)r-2*pair_inkrement;
							ans_sect=spl2->Line_intersect(epi.a, epi.b, epi.c, &hp2, &r_last, r+r2+2*pair_inkrement);
							if(ans_sect==0)
							{
								first_sect=1;
								//*********  get the 3D coordinates   ***(just if a coresponding point was found)*
								//********************************************************************************
								//modf(r, &seg);
								if(Camera::Get_3D_point_hom(gpoint, camera1.P, camera2.P, &hp1, &hp2, r_last))
								{
									if(global_points.nr_p++ >= MAX_G_POINT) {Plot::display(-99,"MAX_G_POINT", &m_status); return;}
									gpoint++;
								}
							}
							else if(ans_sect==1)  //a intersection with an critical angle was found
							{
								first_sect=1;
							}
							else if(ans_sect==3)
							{
								//no intersection of the epi line with the path spline was found
								if(first_sect)		//do move the beginning of the intersection window
								{
									r_last+=(0.8*pair_inkrement);
								}
								else	//do not move the beginning of the intersection window	
								{

								}
							}
							//********************************************************************************
							//********************************************************************************
							//this is used to control the result (the result has to be first de-normalized)***
							CamPoint point[MAX_H_POINT];
							camera2.Transform_nor2img(point, &hp2, 1);
							Plot::Pixels(device, colimagedata2, point,1, 30,  140, 200);	
							//********************************************************************************
						}
					}
					else
					{
						for(double r=first_segment; r <= r_max; r+=pair_inkrement)
						{
							if(i++>=MAX_H_POINT){
								Plot::display(-99,"MAX_H_POINT", &m_status); return;}
							//*****Define (normalized)Points on the path spline1 and spline2 ***
							//*****to build corresponding pairs   ******************************
							p1->cor_spline->Get_point(&hp1, r+r1);
							p2->cor_spline->Get_point(&hp2, r+r2);
							//*********  get the 3D coordinates   ******************************
								
							if(Camera::Get_3D_point_hom(gpoint, camera1.P, camera2.P, &hp1, &hp2, r))
							{
								if(global_points.nr_p++ >= MAX_G_POINT) {Plot::display(-99,"MAX_G_POINT", &m_status); return;}
								gpoint++;
							}
						}
					}
					//p1->nr_p=--i;
					//**********************************************************************************
					//Get the 3D-spline*****************************************************************
					DSpline dspline;
					dspline.last_element=1;
					dspline.Get_measurement_p(global_points);
					if(dspline.Generate_knots(0.5)==1)
					{
							Plot::display(-99,"Too many 3D control points (200)",&m_status); 
							return;
					}
					else if(dspline.Generate_knots(0.5)==2)
					{
							Plot::display(-99,"not enough measuremnts points",&m_status); 							
					}
					else
					{
						dspline.Get_ctrl_points();
						dspline.Get_derivative_coeff();
						dspline.Get_dderivative_coeff();
						dspline.Get_ddderivative_coeff();
						//calculate spline and write points to file
						dspline.Get_spline(nr_global);
						nr_global++;
					}
					//**********************************************************************************
					//remove the path from the matched list 
					pathlist_matched.rem(ip1);
					//and delete the paths and splines from list 1 and 2
					PathListElem *p_p=pathlist1.first_path;
					for(i=0;i<pathlist1.number;i++)
					{
						if(p_p->element == p1)
							pathlist1.del(i);
						p_p++;
					}
					p_p=pathlist2.first_path;
					for(i=0;i<pathlist2.number;i++)
					{
						if(p_p->element == p1)
							pathlist2.del(i);
						p_p++;
					}
					SplineListElem *s_p=splinelist1.first_spline;
					for(i=0;i<splinelist1.number;i++)
					{
						if(s_p->element == spl1)
							splinelist1.del(i);
						s_p++;
					}
					s_p=splinelist2.first_spline;
					for(i=0;i<splinelist2.number;i++)
					{
						if(s_p->element == spl1)
							splinelist1.del(i);
						s_p++;
					}
					--ip1;
				}
			}//for
		}//if

		//*************search the image for new path********************************************************
		//**************************************************************************************************
		//search the region
		for(int j=0;j<search_area1_nr;j++)
		{
			Path path;
			if(*(imagedatacam1+search_area1[j].x+640*search_area1[j].y) == 255)
			{
				path_nr1=path.Get_contour_point(path.path_seg, imagedatacam1, search_area1[j].x, search_area1[j].y);
				//go to end
				while(*(imagedatacam1+search_area1[j].x+640*search_area1[j].y) && j< search_area1_nr-1)
					j++;

				if(path_nr1>5 && path_nr1<5000)
				{
					Path *new_path;
					new_path=pathlist1.create();
					new_path->nr_seg=1;
					//copy data from path to new_path (bzw to path list)
					new_path->path_seg[0].p_nr=path.path_seg[0].p_nr;
					for(i=0;i<path.path_seg[0].p_nr;i++)
					{
						new_path->path_seg[0].pixel[i].x=path.path_seg[0].pixel[i].x;
						new_path->path_seg[0].pixel[i].y=path.path_seg[0].pixel[i].y;
					}


					path_nr1_tot ++;
					new_path->t_start=T_S;

					Plot::Pixels(device, colimagedata1, new_path->path_seg->pixel, new_path->path_seg->p_nr, 0, 100, 150);
					ans=new_path->Get_conture_curvature_a(new_path->path_seg);
					if(ans==-5){Plot::display(1," MAX_END_P in Get_cont.. ", &m_status); return;}
					if(new_path->Get_property(new_path->path_seg))
						Plot::Circles(device, colimagedata1, &new_path->path_seg->centroid, 1, 0, 0, 255);
				
					//normalize the endpoints
					camera1.Normalize_look_up(new_path->path_seg);
				}
			}
		}
		//*************Second view**************************************************************************
		//**************************************************************************************************
		//search the region
		for(j=0;j<search_area2_nr;j++)
		{
			Path path;
			if(*(imagedatacam2+search_area2[j].x+640*search_area2[j].y) == 255)
			{
				path_nr2=path.Get_contour_point(path.path_seg, imagedatacam2, search_area2[j].x, search_area2[j].y);
				//go to end
				while(*(imagedatacam2+search_area1[j].x+640*search_area1[j].y) && j< search_area2_nr-1)
					j++;

				if(path_nr2>5 && path_nr2<5000)
				{
					Path *new_path;
					new_path=pathlist2.create();
					new_path->nr_seg=1;
					//copy data from path to new_path (bzw to path list)
					new_path->path_seg[0].p_nr=path.path_seg[0].p_nr;
					for(i=0;i<path.path_seg[0].p_nr;i++)
					{
						new_path->path_seg[0].pixel[i].x=path.path_seg[0].pixel[i].x;
						new_path->path_seg[0].pixel[i].y=path.path_seg[0].pixel[i].y;
					}

					path_nr2_tot ++;
					new_path->t_start=T_S;

					Plot::Pixels(device, colimagedata2, new_path->path_seg->pixel, new_path->path_seg->p_nr, 0, 100, 150);
					ans=new_path->Get_conture_curvature_a(new_path->path_seg);
					if(ans==-5){Plot::display(2," MAX_END_P in Get_cont.. ", &m_status); return;}
					if(new_path->Get_property(new_path->path_seg))
						Plot::Circles(device, colimagedata2, &new_path->path_seg->centroid, 1, 0, 0, 255);
				
					//normalize the endpoints
					camera1.Normalize_look_up(new_path->path_seg);
				}
			}
		}
		//**************************************************************************************************
		//*********End search the image for new path********************************************************
		Plot::Image_col(device, colimagedata1, 1);
		Plot::Image_col(device, colimagedata2, 2);
		T_S++;
	}
		

	pathlist_matched.destroy();
	pathlist1.destroy();
	pathlist2.destroy();
	splinelist1.destroy();
	splinelist2.destroy();
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(f_path1);
	fclose(f_path2);	
	fclose(f_spline1);
	fclose(f_spline2);
	fclose(f_global);
	running=FALSE;
	ANALYSIS=TRUE;
}

//**********************************************************************************************************
//**********************************************************************************************************
//**********************************************************************************************************
void CStereo2Dlg::OnVmrl() 
{
	char file[255];

	GetDlgItemText(IDC_FILE_CALIB,file,255);

	CDC     *device;
    device=this->GetDC();

	int res=VRML::vrml_points_gen(file, &m_control_bar, device, &m_status);
	if(res==0)
		Plot::display(-99,"Data to VRML converted ", &m_status);
	else if(res==1)
		Plot::display(-99,"No data file", &m_status);
}


void CStereo2Dlg::OnOK() 
{	
	CDialog::OnOK();
}


double Get_r_max(int *r1, int *r2, int *t_s, int p1_t_start, int p2_t_start, double spl1_r_max, double spl2_r_max)
{
	//get the maximal parameter common to both splines
	int delta;
	if(*r1<=*r2) 
	{ 
		*t_s=p1_t_start;
		*r2-=*r1; 
		*r1=0;
		delta = *r2;
		if(spl2_r_max - delta < spl1_r_max)
			return spl2_r_max-delta;
		else 
			return spl1_r_max;
	}
	else       
	{ 
		*t_s=p2_t_start;
		*r1-=*r2; 
		*r2=0;
		delta = *r1;
		if(spl1_r_max - delta < spl2_r_max)
			return spl1_r_max-delta;
		else 
			return spl2_r_max;
	}
}


void CStereo2Dlg::OnOpenImgFile() 
{
	int ans;

	CFileDialog dlg(TRUE,
		NULL,
		m_file_img,
		0,
		"Image files |*.raw|all files|*.*||", this);	

	switch (GetCurrentMessage()->wParam)
	{

		case IDC_OPEN_IMG_FILE :
			ans = dlg.DoModal();
			if(ans==IDOK)
			{
				m_file_img = dlg.GetPathName();
				UpdateData(FALSE);
			}
			break;
		case IDC_OPEN_IMG_FILE2 :
			ans = dlg.DoModal();
			if(ans==IDOK)
			{
				m_file_img2 = dlg.GetPathName();
				UpdateData(FALSE);
			}
			break;
	}
	return;
}



void CStereo2Dlg::OnOpenFileRefImg() 
{
	int ans;

	CFileDialog dlg(TRUE,
		NULL,
		m_file_ref_img,
		0,
		"Image files |*.raw|all files|*.*||", this);
	
	switch (GetCurrentMessage()->wParam)
	{
		case IDC_OPEN_FILE_REF_IMG :
			ans = dlg.DoModal();
			if(ans==IDOK)
			{
				m_file_ref_img = dlg.GetPathName();
				UpdateData(FALSE);
			}
			break;
		case IDC_OPEN_FILE_REF_IMG2 :
			ans=dlg.DoModal();
			if(ans==IDOK)
			{
				m_file_ref_img2 = dlg.GetPathName();
				UpdateData(FALSE);
			}
			break;
	}
	return;
	
}

void CStereo2Dlg::OnOpenCamPar() 
{
	int ans;

	CFileDialog dlg(TRUE,
		NULL,
		m_file_calib,
		0,
		"Camera parameter files |*.par|*.lut|*.ccd|all files|*.*||", this);	

	switch (GetCurrentMessage()->wParam)
	{
		case IDC_OPEN_CAM_PAR :	
			ans=dlg.DoModal();
			if(ans==IDOK)
			{
				m_file_calib = dlg.GetPathName();
				UpdateData(FALSE);
			}
			break;
		case IDC_OPEN_CAM_PAR2 :	
			ans=dlg.DoModal();
			if(ans==IDOK)
			{
				m_file_calib2 = dlg.GetPathName();
				UpdateData(FALSE);
			}
			break;
	}
	return;	
}	





void CStereo2Dlg::OnAnalyze() 
{
	static init=-1;
    device=this->GetDC();
	char file_img[255];
	char file_ref_img[255];
	static char img_file_cam[255];

	if(analyze_img==1)
	{
		GetDlgItemText(IDC_FILE_IMG,file_img,255);
		GetDlgItemText(IDC_FILE_REF_IMG1,file_ref_img,255);
	}
	else
	{
		GetDlgItemText(IDC_FILE_IMG2,file_img,255);
		GetDlgItemText(IDC_FILE_REF_IMG2,file_ref_img,255);
	}

	if(init == 0 || changed ==1)
	{
		if(Image::Read(file_ref_img, refimagedatacam1))
		{
			Plot::display(1,"Error reading ref img: ", &m_status);
			return;
		}
		strcpy(img_file_cam , file_img);
		int j = strlen(img_file_cam)-7;
		sprintf(img_file_cam+j, "%03d.raw", m_img_nr);
		if(Image::Read(img_file_cam, imagedatacam1))
		{
			Plot::display(1,"Error reading img: ", &m_status);	return;
		}
		Plot::Image(device, imagedatacam1, 1);
		Image::Process(device, imagedatacam1, refimagedatacam1, 1, m_thres_ptv);
		Plot::Image(device, imagedatacam1, 2);
		init=1;
		changed=0;
		m_an_img_nr.SetRange(0,3000);
	}
	else if(init ==1)
	{
		UpdateData(TRUE);
		m_thres_ptv=m_thresh_analyze;
		int j = strlen(img_file_cam)-7;
		sprintf(img_file_cam+j, "%03d.raw", m_img_nr);
		if(Image::Read(img_file_cam, imagedatacam1))
		{
			Plot::display(1,"Error reading img: ", &m_status);	
			MessageBox(0, "Error Reading Image 1", 0);
			return;
		}
		Plot::Image(device, imagedatacam1, 1);
		Image::Process(device, imagedatacam1, refimagedatacam1, 1, m_thres_ptv);
		Plot::Image(device, imagedatacam1, 2);
	}
	else
		init=0;
}




void CStereo2Dlg::OnAnlyseImg() 
{

	switch (GetCurrentMessage()->wParam)
	{
		case IDC_ANALYZE_IMG1 :	
			analyze_img=1;
			changed=1;
			break;
		case IDC_ANALYZE_IMG2 :	
			analyze_img=2;
			changed=1;
			break;
	}
	return;		
}

void CStereo2Dlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	int x=(int)point.x;
	int y=(int)point.y;


	if(ANALYSIS)
	{
		if(y<480+CANVASTOP1 && y>CANVASTOP1)
		{
			x-=CANVASLEFT1;
			y-=CANVASTOP1;
			Plot::zoom(device, colimagedata1, x, y, 1);
		}
		else if(y<480+CANVASTOP2 && y>CANVASTOP2)
		{
			x-=CANVASLEFT2;
			y-=CANVASTOP2;
			Plot::zoom(device, colimagedata2, x, y, 2);
		}
	}
	else if(CHG_COORD)
	{
		device=this->GetDC();

		for(int i=0;i<8;i++)
		{
			//here the index i indicates the point resp. coordinate
			//i=0: origin in image 1
			//i=1: origin in image 2
			//i=2: X in image 1
			//i=3: X in image 2
			//i=4: Y in image 1
			//i=5: Y in image 2
			//i=6: Z in image 1
			//i=7: Z in image 2
			if(p_i[i])
			{
				if(fmod((double)i,2) ==0)
				{
					//image 1
					if(x>= CANVASLEFT1 && x < CANVASLEFT1+640 && y>CANVASTOP1 && y<480+CANVASTOP1)
					{
						x-=CANVASLEFT1;
						y-=CANVASTOP1;
						p_coord[i].x=x;
						p_coord[i].y=y;	
						Plot::Circles(device, colimagedata1 , &p_coord[i], 1, 0, 0, 250);
						Plot::Image_col(device,  colimagedata1, 1);
						if(i==0)
							Plot::display(x,y,"O(x,y) image 1  ", &m_status);
						else if(i==2)
							Plot::display(x,y,"X(x,y) image 1  ", &m_status);
						else if(i==4)
							Plot::display(x,y,"Y(x,y) image 1  ", &m_status);
						else if(i==6)
							Plot::display(x,y,"Z(x,y) image 1  ", &m_status);
					}
					else 
					{
						MessageBox(0, "Invalid point, try again", 0);
						return;
					}
					/*
					switch (i){
						case 0: box_text="Indicate origin in image 2";	break;			
						case 2: box_text="Indicate x direction in image 2";	break;					
						case 4: box_text="Indicate y direction in image 2";	break;				
						case 6: box_text="Indicate z direction in image 2";	break;
					}
					AfxMessageBox(box_text, MB_YESNO, 0);
					*/
					break;
				}
				else
				{
					//image 2
					if(x>= CANVASLEFT2 && x < CANVASLEFT2+640 && y>CANVASTOP2 && y<480+CANVASTOP2)
					{
						x-=CANVASLEFT2;
						y-=CANVASTOP2;
						p_coord[i].x=x;
						p_coord[i].y=y;	
						Plot::Circles(device, colimagedata2 , &p_coord[i], 1, 0, 0, 250);
						Plot::Image_col(device,  colimagedata2, 2);
						if(i==1)
							Plot::display(x,y,"O(x,y) image 2  ", &m_status);
						else if(i==3)
							Plot::display(x,y,"X(x,y) image 2  ", &m_status);
						else if(i==5)
							Plot::display(x,y,"Y(x,y) image 2  ", &m_status);
						else if(i==7)
							Plot::display(x,y,"Z(x,y) image 2  ", &m_status);
					}
					else 
					{
						MessageBox(0, "Invalid point, try again", 0);
						return;
					}
					/*
					if(i<7)
					{
						switch (i){
							//case 1: Plot::display(-99,"First LUT loaded", &m_status);
							case 1: box_text="Indicate x direction in image 1";	break;
							case 3: box_text="Indicate y direction in image 1";	break;
							case 5: box_text="Indicate z direction in image 1";	break;
						}
						//image 2
						int ans=AfxMessageBox(box_text, MB_YESNO, 0);
						if(ans==IDNO)
						{
							i++;
							switch (i){
								case 3: box_text="Indicate y direction in image 1";	break;
								case 7: box_text="Indicate z direction in image 1";	break;
							}
							AfxMessageBox(box_text, MB_YESNO, 0);
						}
					}
					*/
					break;
				}
			}
		}
		if(i==7)
			CStereo2Dlg::OnChgCoord2();
		else
		{
			p_i[i]=0;
			p_i[i+1]=1;
		}
	}

	CDialog::OnRButtonDown(nFlags, point);
}


void CStereo2Dlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	Plot::Image_col(device, colimagedata1,1);
	Plot::Image_col(device, colimagedata2,2);
	CDialog::OnRButtonUp(nFlags, point);
}



void test_spline() 
{
//this function is used to test the spline fitting 
//code

	Spline test_spline;
	//FILE *fp=fopen("Control\\spline_data.dat","rb");
	FILE *fp=fopen("Control\\parabol_data2.dat","rb");

	//number of the measurement points
	int number;
	double r;
	fscanf(fp,"%i",&number);
	//test_spline.Num_measurements()=number;
	for(int i=0;i<number;i++)
		fscanf(fp,"%lf%lf%lf",&test_spline.parameter[i],&test_spline.node[i].x,&test_spline.node[i].y);

	fclose(fp);

	Imsl_d_spline *spx;
	Imsl_d_spline *spy;
	//Imsl_d_ppoly  *ppx;
	//Imsl_d_ppoly  *ppy;



	//test_spline.r_max=5;
	//test_spline.r_max=3;
	test_spline.Generate_knots(1);
	test_spline.Get_ctrl_points();


	double *x= new double[number];
	double *y= new double[number];
	double *parameter= new double[number];
	double *knots= new double[test_spline.g+8];

	for(i=0;i<number;i++)
	{
		x[i]=test_spline.node[i].x;
		y[i]=test_spline.node[i].y;
		parameter[i] = test_spline.parameter[i];
	}
	for(i=0;i<test_spline.g+8;i++)
	{
		knots[i]=test_spline.lambda[i];
	}

	//ppx=imsl_d_cub_spline_smooth (m, parameter, x, 0);
	//ppy=imsl_d_cub_spline_smooth (m, parameter, y, 0);
	spx=imsl_d_spline_least_squares (number, parameter, x, test_spline.g+8-4, IMSL_ORDER, 4, IMSL_KNOTS, knots, 0);
	spy=imsl_d_spline_least_squares (number, parameter, y, test_spline.g+8-4, IMSL_ORDER, 4, IMSL_KNOTS, knots, 0);
    //IMSL_KNOTS, float knots[]

	fp=fopen("Control\\spline.dat","wb");
	fp2=fopen("Control\\spline_imsl.dat","wb");
	CamPoint p;
	for(r=0;r<=test_spline.Max_parameter();r+=0.03)
	{
		test_spline.Get_point(&p,r);
		fprintf(fp,"%10.2f%10.2f\n",p.x,p.y);

		double x = imsl_d_spline_value(r, spx, 0);
		double y = imsl_d_spline_value(r, spy, 0);
		//p.x = imsl_d_cub_spline_value(r, ppx, 0);
		//p.y = imsl_d_cub_spline_value(r, ppy, 0);
		fprintf(fp2,"%10.2f%10.2f\n",x,y);
	}
	fclose(fp);
}


void CStereo2Dlg::OnChgCoord() 
{

	char *file_chg_coord1;
	CString file_name1, file_name2;;
	int ans;


	//open image 1
	CFileDialog dlg(TRUE, NULL, file_name1,0, "Image  |*.raw|all files|*.*||", this);	

	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		file_name1 = dlg.GetPathName();
		UpdateData(FALSE);
	}
	else 
		return;
	file_chg_coord1=file_name1.GetBuffer(5);
	if(Image::Read(file_chg_coord1, imagedatacam1))
	{
		Plot::display(1,"Error reading coord img: ", &m_status);
		return;
	}
	else
		Plot::Image(device, imagedatacam1, 1);


	//open image 2
	char *file_chg_coord2;
	CFileDialog dlg2(TRUE, NULL, file_name2,0, "Image  |*.raw|all files|*.*||", this);	

	ans=dlg2.DoModal();
	if(ans==IDOK)
	{
		file_name2 = dlg2.GetPathName();
		UpdateData(FALSE);
	}
	else 
		return;
	file_chg_coord2=file_name2.GetBuffer(5);
	if(Image::Read(file_chg_coord2, imagedatacam2))
	{
		Plot::display(2,"Error reading coord img: ", &m_status);
		return;
	}
	else
		Plot::Image(device, imagedatacam2, 2);



	for(int i=0;i<480*640;i++)
	{
		colimagedata1[3*i]	 =imagedatacam1[i];
		colimagedata1[3*i+1] =imagedatacam1[i];
		colimagedata1[3*i+2] =imagedatacam1[i];

		colimagedata2[3*i]	 =imagedatacam2[i];
		colimagedata2[3*i+1] =imagedatacam2[i];
		colimagedata2[3*i+2] =imagedatacam2[i];
	}

	ANALYSIS=FALSE;
	CHG_COORD=TRUE;
	MessageBox("Click on origin in image 1",NULL, MB_OK);
	//Get images coordinates
}


void CStereo2Dlg::OnButtonTest() 
{
		char file_cam_par1[255],file_cam_par2[255];	
		char file_ccd_par1[255],file_ccd_par2[255]; 
		char file_lut_par1[255],file_lut_par2[255]; 

		strcpy(file_cam_par1,"G:\\Images\\TestCal\\cam1.par");
		strcpy(file_cam_par2,"G:\\Images\\TestCal\\cam2.par");
		strcpy(file_ccd_par1,"G:\\Images\\TestCal\\ccd_par1.ccd");
		strcpy(file_ccd_par2,"G:\\Images\\TestCal\\ccd_par2.ccd");
		strcpy(file_lut_par1,"G:\\Images\\TestCal\\cam1.lut");
		strcpy(file_lut_par2,"G:\\Images\\TestCal\\cam2.lut");


		if(camera1.init == 0)
		{
			if(camera1.Load_look_up(file_lut_par1, &m_control_bar, 1))
			{
				Plot::display(-99,"Error, can not finde LUT 1", &m_status);
				return;
			}
			else
			{
				Plot::display(-99,"First LUT loaded", &m_status);
				camera1.init = 1;
			}
		}

		if(camera2.init == 0)
		{
			if(camera2.Load_look_up(file_lut_par2, &m_control_bar, 2))
			{
				Plot::display(1,"Error, can not finde LUT 2", &m_status);
				return;
			}
			else
			{
				Plot::display(-99,"Second LUT loaded", &m_status);
				camera1.init = 1;
			}
		}
		camera1.Read_cam_par(file_cam_par1);
		camera2.Read_cam_par(file_cam_par2);
		camera1.Read_ccd_par(file_ccd_par1);
		camera2.Read_ccd_par(file_ccd_par2);
		//this fills the t and R variables
		camera1.Get_transf_matrix();
		camera2.Get_transf_matrix();
		//this fills the P matrix [R; -Rt]
		camera1.Get_camera_matrix();
		camera2.Get_camera_matrix();


		//read 3D points
	    FILE *points3d= NULL;
		char file_name[255];
		strcpy(file_name,"G:\\Images\\TestCal\\points3d.dat");
		int nr;
		points3d=fopen(file_name,"r");
		fscanf(points3d,"%d",&nr);
		GlobalPoint *P = new GlobalPoint[nr];
		for (int i=0;i<nr;i++) fscanf (points3d," %lf %lf %lf",&P[i].X,&P[i].Y,&P[i].Z);
		fclose(points3d);


		CamPoint x_test1=camera1.Project_point(P[1]);
		CamPoint x_test2=camera2.Project_point(P[1]);
		x_test1  = camera1.Transform_nor2img(x_test1);
		x_test2  = camera2.Transform_nor2img(x_test2);


		GlobalPoint P_rec;
		CamPoint hp_O_1, hp_O_2;
		//camera1.Normalize_look_up(x_test1,  &hp_O_1);
		//camera2.Normalize_look_up(x_test2 , &hp_O_2);
		camera1.Normalize(x_test1,  &hp_O_1);
		camera2.Normalize(x_test2 , &hp_O_2);
		Camera::Get_3D_point_hom(&P_rec, camera1.P, camera2.P, hp_O_1, hp_O_2);
}





//global variables needed because of the IMSL function
void fct_coord_chg(int m, int n, double x[], double f[]);
T_vec x1_glob, y1_glob, z1_glob;
T_vec o1_glob, o2_glob;
double nx_glob, ny_glob, nz_glob;

void fct_rot_mat(int m, int n, double x[], double f[]);
Matrix R2_glob;

void  CStereo2Dlg::OnChgCoord2() 
{
		char file1[255],file2[255];	
		char file_path1[255],file_path2[255];
		char file_cam_par1[255],file_cam_par2[255];	
		char file_ccd_par1[255],file_ccd_par2[255]; 
		char file_lut_par1[255],file_lut_par2[255]; 
		GetDlgItemText(IDC_FILE_CALIB,file_path1,255);
		GetDlgItemText(IDC_FILE_CALIB2,file_path2,255);
		char *c=file_path1+strlen(file_path1);
		while(*c != '\\')
			*c--;
		strcpy(file1,++c);
		*c='\0';
		c=file1+strlen(file1);
		while(*c != '.')
			*c--;
		*c='\0';

		c=file_path2+strlen(file_path2);
		while(*c != '\\')
			*c--;
		strcpy(file2,++c);
		*c='\0';
		c=file2+strlen(file2);
		while(*c != '.')
			*c--;
		*c='\0';

		strcpy(file_cam_par1,file_path1);
		strcpy(file_cam_par2,file_path2);
		strcpy(file_ccd_par1,file_path1);
		strcpy(file_ccd_par2,file_path2);
		strcpy(file_lut_par1,file_path1);
		strcpy(file_lut_par2,file_path2);
		strcat(file_ccd_par1,"ccd_par1.ccd");
		strcat(file_ccd_par2,"ccd_par2.ccd");
		strcat(file_cam_par1,file1);
		strcat(file_cam_par2,file2);
		strcat(file_lut_par1,file1);
		strcat(file_lut_par2,file2);
		strcat(file_cam_par1,".par");
		strcat(file_cam_par2,".par");
		strcat(file_lut_par1,".lut");
		strcat(file_lut_par2,".lut");
		int NORM =1;
		
		if(camera1.init == 0)
		{
			if(camera1.Load_look_up(file_lut_par1, &m_control_bar, 1))
			{
				Plot::display(-99,"Error, can not finde LUT 1", &m_status);
				return;
			}
			else
				Plot::display(-99,"First LUT loaded", &m_status);
		}

		if(camera2.init == 0)
		{
			if(camera2.Load_look_up(file_lut_par2, &m_control_bar, 2))
			{
				Plot::display(1,"Error, can not finde LUT 2", &m_status);
				return;
			}
			else
				Plot::display(-99,"Second LUT loaded", &m_status);
		}
		
		camera1.Read_cam_par(file_cam_par1);
		camera2.Read_cam_par(file_cam_par2);
		camera1.Read_ccd_par(file_ccd_par1);
		camera2.Read_ccd_par(file_ccd_par2);
		//this fills the t and R variables
		camera1.Get_transf_matrix();
		camera2.Get_transf_matrix();
		//this fills the P matrix [R; -Rt]
		camera1.Get_camera_matrix();
		camera2.Get_camera_matrix();


		//calculate the 3D coordinates of the origin
		GlobalPoint P_origin;
		CamPoint hp_O_1, hp_O_2;
		if(NORM)
		{
			camera1.Normalize_look_up(p_coord[0],  &hp_O_1);
			camera2.Normalize_look_up(p_coord[1] , &hp_O_2);
		}
		else
		{
			camera1.Normalize(p_coord[0],  &hp_O_1);
			camera2.Normalize(p_coord[1] , &hp_O_2);
			//CamPoint o_test_img1  = camera1.Transform_nor2img(hp_O_1);
			//CamPoint o_test_img2  = camera2.Transform_nor2img(hp_O_2);
		}
		Camera::Get_3D_point_hom(&P_origin, camera1.P, camera2.P, hp_O_1, hp_O_2);
		//GlobalPoint P_origin_non_lin;
		//Camera::Get_3D_point_non_lin(&P_origin_non_lin, camera1, camera2, hp_O_1, hp_O_2);
		//Camera::Get_3D_point_non_lin(&P_origin, camera1, camera2, hp_O_1, hp_O_2);
		Plot::display(P_origin.X,P_origin.Y,P_origin.Z,"O", &m_status);

		//calculate the 3D coordinates of the x-direction indicator
		GlobalPoint P_X;
		CamPoint hpx1, hpx2;
		if(NORM)
		{
			camera1.Normalize_look_up(p_coord[2],  &hpx1);
			camera2.Normalize_look_up(p_coord[3] , &hpx2);
		}
		else
		{
			camera1.Normalize(p_coord[2],  &hpx1);
			camera2.Normalize(p_coord[3] , &hpx2);
		}
		Camera::Get_3D_point_hom(&P_X, camera1.P, camera2.P, hpx1, hpx2);
		//Camera::Get_3D_point_non_lin(&P_X, camera1, camera2, hpx1, hpx2);
		Plot::display(P_X.X,P_X.Y,P_X.Z,"X", &m_status);

		//calculate the 3D coordinates of the y-direction indicator
		GlobalPoint P_Y;
		CamPoint hpy1, hpy2;
		if(NORM)
		{
			camera1.Normalize_look_up(p_coord[4],  &hpy1);
			camera2.Normalize_look_up(p_coord[5] , &hpy2);
		}
		else
		{
			camera1.Normalize(p_coord[4],  &hpy1);
			camera2.Normalize(p_coord[5] , &hpy2);
		}
		Camera::Get_3D_point_hom(&P_Y, camera1.P, camera2.P, hpy1, hpy2);
		//Camera::Get_3D_point_non_lin(&P_Y, camera1, camera2, hpy1, hpy2);
		Plot::display(P_Y.X,P_Y.Y,P_Y.Z,"Y", &m_status);

		//calculate the 3D coordinates of the z-direction indicator
		GlobalPoint P_Z;
		CamPoint hpz1, hpz2;
		if(NORM)
		{
			camera1.Normalize_look_up(p_coord[6],  &hpz1);
			camera2.Normalize_look_up(p_coord[7] , &hpz2);
		}
		else
		{
			camera1.Normalize(p_coord[6],  &hpz1);
			camera2.Normalize(p_coord[7] , &hpz2);
		}
		Camera::Get_3D_point_hom(&P_Z, camera1.P, camera2.P, hpz1, hpz2);
		//Camera::Get_3D_point_non_lin(&P_Z, camera1, camera2, hpz1, hpz2);
		Plot::display(P_Z.X,P_Z.Y,P_Z.Z,"Z", &m_status);

		//test
		//double test = Y_dir.t1*Z_dir.t1  +  Y_dir.t2*Z_dir.t2  +  Y_dir.t3*Z_dir.t3;
	   

		CamPoint o_test1=camera1.Project_point(P_origin);
		CamPoint o_test2=camera2.Project_point(P_origin);
		CamPoint o_test_img1  = camera1.Transform_nor2img(o_test1);
		CamPoint o_test_img2  = camera2.Transform_nor2img(o_test2);

		GlobalPoint P_rec;
		CamPoint hp_oo_1, hp_oo_2;
		//camera1.Normalize_look_up(o_test1,  &hp_O_1);
		//camera2.Normalize_look_up(o_test2 , &hp_O_2);
		camera1.Normalize(o_test_img1,  &hp_oo_1);
		camera2.Normalize(o_test_img1 , &hp_oo_2);
		Camera::Get_3D_point_hom(&P_rec, camera1.P, camera2.P, hp_oo_1, hp_oo_2);


		CamPoint x_test1=camera1.Project_point(P_X);
		CamPoint x_test2=camera2.Project_point(P_X);
		CamPoint x_test_img1  = camera1.Transform_nor2img(x_test1);
		CamPoint x_test_img2  = camera2.Transform_nor2img(x_test2);

		CamPoint y_test1=camera1.Project_point(P_Y);
		CamPoint y_test2=camera2.Project_point(P_Y);
		CamPoint y_test_img1  = camera1.Transform_nor2img(y_test1);
		CamPoint y_test_img2  = camera2.Transform_nor2img(y_test2);

		CamPoint z_test1=camera1.Project_point(P_Z);
		CamPoint z_test2=camera2.Project_point(P_Z);
		CamPoint z_test_img1  = camera1.Transform_nor2img(z_test1);
		CamPoint z_test_img2  = camera2.Transform_nor2img(z_test2);

		device=this->GetDC();
		Plot::Circles(device, colimagedata1 , &o_test_img1, 1, 250, 250, 250);
		Plot::Circles(device, colimagedata2 , &o_test_img2, 1, 250, 250, 250);

		Plot::Circles(device, colimagedata1 , &x_test_img1, 1, 250, 250, 250);
		Plot::Circles(device, colimagedata2 , &x_test_img2, 1, 250, 250, 250);

		Plot::Circles(device, colimagedata1 , &y_test_img1, 1, 250, 250, 250);
		Plot::Circles(device, colimagedata2 , &y_test_img2, 1, 250, 250, 250);

		Plot::Circles(device, colimagedata1 , &z_test_img1, 1, 250, 250, 250);
		Plot::Circles(device, colimagedata2 , &z_test_img2, 1, 250, 250, 250);

		Plot::Image_col(device,  colimagedata1, 1);
		Plot::Image_col(device,  colimagedata2, 2);


		//get the X, Y, Z directions 
		T_vec X_dir;
		X_dir.t1=P_origin.X-P_X.X;
		X_dir.t2=P_origin.Y-P_X.Y;
		X_dir.t3=P_origin.Z-P_X.Z;
		T_vec Y_dir;
		Y_dir.t1=P_origin.X-P_Y.X;
		Y_dir.t2=P_origin.Y-P_Y.Y;
		Y_dir.t3=P_origin.Z-P_Y.Z;	
		T_vec Z_dir;
		Z_dir.t1=P_origin.X-P_Z.X;
		Z_dir.t2=P_origin.Y-P_Z.Y;
		Z_dir.t3=P_origin.Z-P_Z.Z;


		//get the lenght of the vectors
		double nx=sqrt((P_origin.X-P_X.X)*(P_origin.X-P_X.X)  +  (P_origin.Y-P_X.Y)*(P_origin.Y-P_X.Y)
						+(P_origin.Z-P_X.Z)*(P_origin.Z-P_X.Z));
		double ny=sqrt((P_origin.X-P_Y.X)*(P_origin.X-P_Y.X)  +  (P_origin.Y-P_Y.Y)*(P_origin.Y-P_Y.Y)
						+(P_origin.Z-P_Y.Z)*(P_origin.Z-P_Y.Z));
		double nz=sqrt((P_origin.X-P_Z.X)*(P_origin.X-P_Z.X)  +  (P_origin.Y-P_Z.Y)*(P_origin.Y-P_Z.Y)
						+(P_origin.Z-P_Z.Z)*(P_origin.Z-P_Z.Z));


		Plot::display(nx,"nx		", &m_status);
		Plot::display(ny,"ny		", &m_status);
		Plot::display(nz,"nz		", &m_status);
		Plot::display(-99,"--------------",		   &m_status);





		//copy the data to global (global in the code not in the coordinate system!!) variables
		o1_glob.t1=P_origin.X;
		o1_glob.t2=P_origin.Y;
		o1_glob.t3=P_origin.Z;
	
		y1_glob.t1=P_Y.X;
		y1_glob.t2=P_Y.Y;
		y1_glob.t3=P_Y.Z;
		
		z1_glob.t1=P_Z.X;
		z1_glob.t2=P_Z.Y;
		z1_glob.t3=P_Z.Z;

		nx_glob=nx;
		ny_glob=ny;
		nz_glob=nz;


		double xguess[3];
		double *result;
		int m=6, n=3;           //m: Number of Funktions   n: Number of Variables  n<=m
		xguess[0]=0.1;
		xguess[1]=3.0;
		xguess[2]=0.1;
		result = imsl_d_nonlin_least_squares(fct_coord_chg, m, n, IMSL_XGUESS, xguess, 0);
		double phi1_new  =*result++;
		double omega1_new=*result++;
		double kappa1_new=*result;

		//************R=-R2' R1************************************************************
		//*********** --> R2=-R' R1 *******************************************************
		//****** R= R2_old ****************************************************************
		Matrix R1;
		R1.a11 =  cos(phi1_new)*cos(kappa1_new);
		R1.a12 = -cos(phi1_new)*sin(kappa1_new);
		R1.a13 =  sin(phi1_new);
		R1.a21 =  cos(omega1_new)*sin(kappa1_new) + sin(omega1_new)*sin(phi1_new)*cos(kappa1_new);
		R1.a22 =  cos(omega1_new)*cos(kappa1_new) - sin(omega1_new)*sin(phi1_new)*sin(kappa1_new);
		R1.a23 = -sin(omega1_new)*cos(phi1_new);  
		R1.a31 =  sin(omega1_new)*sin(kappa1_new) - cos(omega1_new)*sin(phi1_new)*cos(kappa1_new);
		R1.a32 =  sin(omega1_new)*cos(kappa1_new) + cos(omega1_new)*sin(phi1_new)*sin(kappa1_new);
		R1.a33 =  cos(omega1_new)*cos(phi1_new);

		//Matrix R2=Neg(mat_mult(transpose(camera2.R),R1));
		Matrix R2=mat_mult(transpose(camera2.R),R1);

		//get the angles of camera 2
		//copy the data into the global variable
		R2_glob.a11  = R2.a11;
		R2_glob.a12  = R2.a12;
		R2_glob.a13  = R2.a13;
		R2_glob.a21  = R2.a21;
		R2_glob.a22  = R2.a22;
		R2_glob.a23  = R2.a23;  
		R2_glob.a31  = R2.a31;
		R2_glob.a32  = R2.a32;
		R2_glob.a33  = R2.a33;

		m=9, n=3;           //m: Number of Funktions   n: Number of Variables  n<=m
		xguess[0]=0.1;
		xguess[1]=0.1;
		xguess[2]=0.1;

		result = imsl_d_nonlin_least_squares(fct_rot_mat, m, n, IMSL_XGUESS, xguess, 0);
		double phi2_new   =	*result++;
		double omega2_new =	*result++;
		double kappa2_new =	*result;

		//calculate the new extrinsic camera parameters and
		//transfer this data into the camera1 class and camera2 class
		//t1=-R1 o'
		T_vec t1_new=mat_vec_mult(Neg(R1),o1_glob);
		//O2'=R1'(O2-t1)  
		T_vec t2_new=vec_add(mat_vec_mult(R1,camera2.t),t1_new);


		//test the result
		//Matrix R_test_old=mat_mult(transpose(camera2.R),camera1.R);
		Matrix R_test_new=mat_mult(transpose(R2),R1);
		//T_vec t_old = mat_vec_mult(transpose(camera2.R),vec_sub(camera1.t,camera2.t));
		T_vec t_new = mat_vec_mult(transpose(R2),vec_sub(t1_new,t2_new));

		camera1.Set_position(t1_new);
		camera2.Set_position(t2_new);

		camera1.Set_orientation(phi1_new, omega1_new, kappa1_new);
		camera2.Set_orientation(phi2_new, omega2_new, kappa2_new);

		camera1.Get_transf_matrix();
		camera1.Get_camera_matrix();

		camera2.Get_transf_matrix();
		camera2.Get_camera_matrix();

		Plot::display(t1_new.t1,"X1		", &m_status);
		Plot::display(t1_new.t2,"Y1		", &m_status);
		Plot::display(t1_new.t3,"Z1		", &m_status);
		Plot::display(-99,"--------------",		   &m_status);
		Plot::display(t2_new.t1,"X2		", &m_status);
		Plot::display(t2_new.t2,"Y2		", &m_status);
		Plot::display(t2_new.t3,"Z2		", &m_status);
		Plot::display(-99,"--------------",		   &m_status);
		Plot::display(phi1_new,		"phi1	",	&m_status);
		Plot::display(omega1_new,	"omega1	",	&m_status);
		Plot::display(kappa1_new,	"kappa1	",	&m_status);
		Plot::display(-99,"--------------",		   &m_status);
		Plot::display(phi2_new,		"phi2	",	&m_status);
		Plot::display(omega2_new,	"omega2	",	&m_status);
		Plot::display(kappa2_new,	"kappa2	",	&m_status);

		ANALYSIS=TRUE;
		CHG_COORD=FALSE;

		p_i[0]=1;
		p_coord[0].x=0;
		p_coord[0].y=0;
		for(int i=1;i<8;i++);
		{
			p_i[i]=0;
			p_coord[i].x=0;
			p_coord[i].y=0;
		}



/*		//optimize the points such that a orthogonal sytem results
		if(0)
		{
			int n_equ=3;
			int m_var=3;
			int ncon=1;
			double A[9]={1,0,0,0,1,0,0,0,1};
			double b[3]={Y_dir.t1, Y_dir.t2, Y_dir.t3};
			double xlb[] = {-10000.0, -10000.0, -10000.0};
			double xub[] = { 10000.0,  10000.0,  10000.0};
			int con_type[1]={0};
			double C[3]={Z_dir.t1, Z_dir.t2, Z_dir.t3};
			double bc[]={0};

			double *res = imsl_d_lin_lsq_lin_constraints (n_equ,m_var,ncon,A,b,C, bc, bc, con_type, xlb, xub, 0);
			Y_dir.t1=*res++; 
			Y_dir.t2;*res++; 
			Y_dir.t3;*res; 
		}
		else if(0)
		{
			Y_dir.t3 = -(Y_dir.t1*Z_dir.t1  +  Y_dir.t2*Z_dir.t2)/Z_dir.t3;
			P_Y.Z = P_origin.Z - Y_dir.t3;
		}
*/

}





void fct_coord_chg(int m, int n, double x[], double f[])
{

	double phi=x[0];
	double omega=x[1];
	double kappa=x[2];

	Matrix R;
	//fill the rotations matrix
	R.a11 =  cos(phi)*cos(kappa);
	R.a12 = -cos(phi)*sin(kappa);
	R.a13 =  sin(phi);
	R.a21 =  cos(omega)*sin(kappa) + sin(omega)*sin(phi)*cos(kappa);
	R.a22 =  cos(omega)*cos(kappa) - sin(omega)*sin(phi)*sin(kappa);
	R.a23 = -sin(omega)*cos(phi);  
	R.a31 =  sin(omega)*sin(kappa) - cos(omega)*sin(phi)*cos(kappa);
	R.a32 =  sin(omega)*cos(kappa) + cos(omega)*sin(phi)*sin(kappa);
	R.a33 =  cos(omega)*cos(phi);

	T_vec x_dir;
	x_dir.t1=nx_glob;
	x_dir.t2=0;
	x_dir.t3=0;

	T_vec y_dir;
	y_dir.t1=0;
	y_dir.t2=ny_glob;
	y_dir.t3=0;

	T_vec z_dir;
	z_dir.t1=0;
	z_dir.t2=0;
	z_dir.t3=nz_glob;

	R=transpose(R);
	//first criterion 
	//T_vec f1=vec_sub(mat_vec_mult(R,vec_add(x_dir,t_glob)),x1_glob);
	//f[0]=f1.t1;
	//f[1]=f1.t2;
	//f[2]=f1.t3;
	//second criterion 
	//x'=R_1' dir_x + org' - x'
	T_vec f2= vec_sub(vec_add(mat_vec_mult(R, y_dir),o1_glob), y1_glob);
	f[0]=f2.t1;
	f[1]=f2.t2;
	f[2]=f2.t3;
	//third criterion 
	T_vec f3= vec_sub(vec_add(mat_vec_mult(R, z_dir),o1_glob), z1_glob);
	f[3]=f3.t1;
	f[4]=f3.t2;
	f[5]=f3.t3;
}


void fct_rot_mat(int m, int n, double x[], double f[])
{
	double phi=x[0];
	double omega=x[1];
	double kappa=x[2];

	f[0]  = R2_glob.a11 -  (cos(phi)*cos(kappa));
	f[1]  = R2_glob.a12 -  (-cos(phi)*sin(kappa));
	f[2]  = R2_glob.a13 -  (sin(phi));
	f[3]  = R2_glob.a21 -  (cos(omega)*sin(kappa) + sin(omega)*sin(phi)*cos(kappa));
	f[4]  = R2_glob.a22 -  (cos(omega)*cos(kappa) - sin(omega)*sin(phi)*sin(kappa));
	f[5]  = R2_glob.a23 -  (-sin(omega)*cos(phi));  
	f[6]  = R2_glob.a31 -  (sin(omega)*sin(kappa) - cos(omega)*sin(phi)*cos(kappa));
	f[7]  = R2_glob.a32 -  (sin(omega)*cos(kappa) + cos(omega)*sin(phi)*sin(kappa));
	f[8]  = R2_glob.a33 -  (cos(omega)*cos(phi));
}

