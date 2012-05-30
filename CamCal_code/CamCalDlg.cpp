// CamCalDlg.cpp : implementation file
//

#include "stdafx.h"
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <fstream.h>
#include "CamCal.h"
#include "CamCalDlg.h"
#include "Get_3D_DialogDlg.h"
#include "ccd_parDlg.h"
#include "niimaq.h"
#include "parameters.h"
#include "structures.h"
#include "plot_functions.h"
#include "io_functions.h"
#include "img_proc.h"
#include "lin_calibration.h"
#include "calibration.h"
#include "dist_look_up.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//**************************Variables for mouse pointer position**********************
int x_pick, y_pick;
//**************************Array inicating which Parameter to optimize***************
int       var[15];
//**************************Variables for Program Flow Control************************
int       plane_index=0;          //Number of the planes currently used
int       match_nr = -99;         //Used to save the match number of the last plane
int       CAL_MODUS=0;            //Marking points modus
int       INS_MODUS=0;			  //Image inspection modus
static int first=1;               //Initial Parameters are just read from the dialog if 
                                  // first=1
//**************************End Variables for Program Flow Control********************
//**********************Variables for Camera Calibration******************************
CamPoint        centr[MAX];
//**********************End Variables for Camera Calibration**************************
//**********************Arrays to store the detected control points*******************
Extrinsic_par ex_init;
Intrinsic_par in_init;
CCD_par		  ccd_init;
P_matrix	  P;
Plane_par     plane_init;
float         rms; 
//**********************End Camera Parameters used for Calibration********************
//**********************Variables used by calibration.cpp and lin_calibration.cpp*****
int number;
int first_plane_nr;                              //number of detected points in first plane
double x_gem[MAX_MATCH], y_gem[MAX_MATCH];
double X[MAX_MATCH], Y[MAX_MATCH], Z[MAX_MATCH];
//**********************End Variables used by calibration.cpp and lin_calibration.cpp*
int        mem_alloc=0;
uInt8*     image_data=NULL;		//image memory
uInt8*     color_image_data=NULL;	//image memory
//**********************file and path names******************************************
char file_global[255];        //file name of the global files 
char file_cal_img[255];       //file name of the calibration images 
char path[255];				  //path of the directory containing match.dat, un_sort...
//************************************************************************************
int min_pix=3, max_pix=100;
int GAUSS,ROW_CORR;
CDC     *device;

  

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
// CCamCalDlg dialog

CCamCalDlg::CCamCalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCamCalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCamCalDlg)
	m_file_cal_img = _T("");
	m_file_global = _T("");
	m_dir_name = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCamCalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamCalDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_bar);
	DDX_Control(pDX, IDC_LIN_CAL, m_lin_cal);
	DDX_Control(pDX, IDC_CTR_FOCUS, m_ctr_focus);
	DDX_Control(pDX, IDC_CTR_Z0, m_ctr_z0);
	DDX_Control(pDX, IDC_CTR_YH, m_ctr_yh);
	DDX_Control(pDX, IDC_CTR_Y0, m_ctr_y0);
	DDX_Control(pDX, IDC_CTR_XH, m_ctr_xh);
	DDX_Control(pDX, IDC_CTR_X0, m_ctr_x0);
	DDX_Control(pDX, IDC_CTR_THETA, m_ctr_theta);
	DDX_Control(pDX, IDC_CTR_SX, m_ctr_sx);
	DDX_Control(pDX, IDC_CTR_PHI, m_ctr_phi);
	DDX_Control(pDX, IDC_CTR_P2, m_ctr_p2);
	DDX_Control(pDX, IDC_CTR_P1, m_ctr_p1);
	DDX_Control(pDX, IDC_CTR_OMEGA, m_ctr_omega);
	DDX_Control(pDX, IDC_CTR_KAPPA, m_ctr_kappa);
	DDX_Control(pDX, IDC_CTR_K2, m_ctr_k2);
	DDX_Control(pDX, IDC_CTR_K1, m_ctr_k1);
	DDX_Control(pDX, IDC_RMS, m_rms);
	DDX_Control(pDX, IDC_REF_POINT, m_ref_point);
	DDX_Control(pDX, IDC_CAL_THRES, m_cal_thres);
	DDX_Control(pDX, IDC_STATUS, m_status);
	DDX_Control(pDX, IDC_K2, m_k2);
	DDX_Control(pDX, IDC_THETA, m_theta);
	DDX_Control(pDX, IDC_Y_H, m_y_h);
	DDX_Control(pDX, IDC_X_H, m_x_h);
	DDX_Control(pDX, IDC_S_X, m_s_x);
	DDX_Control(pDX, IDC_Z_0, m_z_0);
	DDX_Control(pDX, IDC_Y_0, m_y_0);
	DDX_Control(pDX, IDC_X_0, m_x_0);
	DDX_Control(pDX, IDC_FOCUS, m_focus);
	DDX_Control(pDX, IDC_OMEGA, m_omega);
	DDX_Control(pDX, IDC_KAPPA, m_kappa);
	DDX_Control(pDX, IDC_K1, m_k1);
	DDX_Control(pDX, IDC_PHI, m_phi);
	DDX_Control(pDX, IDC_P2, m_p2);
	DDX_Control(pDX, IDC_P1, m_p1);
	DDX_Text(pDX, IDC_FILE_CAL_IMG, m_file_cal_img);
	DDX_Text(pDX, IDC_FILE_GLOBAL, m_file_global);
	DDX_Text(pDX, IDC_DIR_PATH, m_dir_name);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCamCalDlg, CDialog)
	//{{AFX_MSG_MAP(CCamCalDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_CAL_CAM, OnCalCam)
	ON_BN_CLICKED(IDC_PLANE1, OnPlane)
	ON_BN_CLICKED(IDC_DETECT, OnDetect)
	ON_BN_CLICKED(IDC_NEW_SET, OnNewSet)
	ON_BN_CLICKED(IDC_RESET_PARA, OnResetPara)
	ON_BN_CLICKED(IDC_OPEN_CAL_IMAGE, OnOpenCalImage)
	ON_BN_CLICKED(IDC_OPEN_GLOBAL_FILE, OnOpenGlobalFile)
	ON_BN_CLICKED(IDC_LIN_CAL, OnLinCal)
	ON_BN_CLICKED(IDC_PATH, OnPath)
	ON_BN_CLICKED(IDC_GET_3D, OnGet3d)
	ON_BN_CLICKED(IDC_GET_LOOK_UP, OnGetLookUp)
	ON_COMMAND(ID_FILE_SAVENONLINCAMPAR, OnFileSavenonlin)
	ON_COMMAND(ID_FILE_SAVELINEARCAMPAR, OnFileSavelin)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_CCDPARAMETERS_SET, OnCCDparSet)
	ON_BN_CLICKED(IDC_PLANE2, OnPlane)
	ON_BN_CLICKED(IDC_PLANE3, OnPlane)
	ON_BN_CLICKED(IDC_PLANE5, OnPlane)
	ON_BN_CLICKED(IDC_PLANE6, OnPlane)
	ON_BN_CLICKED(IDC_PLANE7, OnPlane)
	ON_BN_CLICKED(IDC_PLANE8, OnPlane)
	ON_BN_CLICKED(IDC_PLANE9, OnPlane)
	ON_BN_CLICKED(IDC_PLANE10, OnPlane)
	ON_BN_CLICKED(IDC_PLANE4, OnPlane)
	ON_BN_CLICKED(IDC_PLANE0, OnPlane)
	ON_COMMAND(IDD_GAUSS, OnImageProc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamCalDlg message handlers

BOOL CCamCalDlg::OnInitDialog()
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


	m_x_0.SetWindowText("120");
	m_y_0.SetWindowText("75");
	m_z_0.SetWindowText("-1200");
	m_omega.SetWindowText("3.0");
	m_phi.SetWindowText("0.0");
	m_kappa.SetWindowText("0.0");
	m_focus.SetWindowText("8.0");
	m_x_h.SetWindowText("0.0");
	m_y_h.SetWindowText("0.0");
	m_s_x.SetWindowText("1.0");
	m_theta.SetWindowText("0.0");
	m_k1.SetWindowText("0.0");
	m_k2.SetWindowText("0.0");
	m_p1.SetWindowText("0.0");
	m_p2.SetWindowText("0.0");

	m_ref_point.SetWindowText("126");  //220	
	m_cal_thres.SetWindowText("140");

	m_ctr_x0.SetCheck(1);
	m_ctr_y0.SetCheck(1);
	m_ctr_z0.SetCheck(1);
	m_ctr_omega.SetCheck(1);
	m_ctr_phi.SetCheck(1);
	m_ctr_kappa.SetCheck(1);
	m_ctr_focus.SetCheck(1);
	m_ctr_xh.SetCheck(0);
	m_ctr_yh.SetCheck(0);
	m_ctr_sx.SetCheck(0);
	m_ctr_theta.SetCheck(0);
	m_ctr_p1.SetCheck(0);
	m_ctr_p2.SetCheck(0);
	m_ctr_k1.SetCheck(0);
	m_ctr_k2.SetCheck(0);

	ccd_init.height=480;
	ccd_init.width =640;
	ccd_init.pix_h =0.009;
	ccd_init.pix_w =0.009;

	GAUSS=1;
	ROW_CORR=1;


	display(-99,"Camera Calibration by Matthias Machacek ETH/IFD", &m_status);
	display(-99," ", &m_status);
	display(-99,"You need the following file with the sorted global and local points:", &m_status);
	display(-99,"  working directory\"match.dat\"", &m_status);
	display(-99,"This file can be created. You need:", &m_status);
	display(-99,"  the calibration images with the name extensions  \"....#.raw\"", &m_status);
	display(-99,"  the images have to be in raw format, unsigned and 8bit", &m_status);
	display(-99,"  the files containing the global coordinates for each plane  \"....#.dat\"", &m_status);



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCamCalDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCamCalDlg::OnPaint() 
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
HCURSOR CCamCalDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void CCamCalDlg::OnPlane() 
{
	char buffer[255];
	int i;
	int do_sort=0,step_max=1;
	static int j, nr_planes;
	double hist[256];

	image_data =     (uInt8 *)calloc((  ccd_init.width*ccd_init.height),sizeof(uInt8)); //image memory
	color_image_data = (uInt8 *)malloc(3*ccd_init.width*ccd_init.height*sizeof(uInt8));
	mem_alloc=1;


    device=this->GetDC();


	//check first if user specified a path for the working directory
	if(file_cal_img[0] == '\0')
	{
		MessageBox("Specifiy the calibration image!",0, MB_ICONEXCLAMATION );
		return;
	}

	CAL_MODUS=0;
	INS_MODUS=1;


//*****initialize memory for images***************************************
	for(i=0; i<(3 * ccd_init.height*ccd_init.width); i++)
	{
		color_image_data[i] = 0;
	}
	for(i=0; i<(ccd_init.height*ccd_init.width); i++)
	{
		image_data[i] = 0;
	}
//************************************************************************

	switch (GetCurrentMessage()->wParam)
	{
		case IDC_PLANE0 :  
			plane_index = 0; break;
		case IDC_PLANE1 :  
			plane_index = 1; break;
		case IDC_PLANE2 :
			plane_index = 2; break;
		case IDC_PLANE3 : 
			plane_index = 3; break;
		case IDC_PLANE4 : 
			plane_index = 4; break;
		case IDC_PLANE5 : 
			plane_index = 5; break;
		case IDC_PLANE6 : 
			plane_index = 6; break;
		case IDC_PLANE7 : 
			plane_index = 7; break;
		case IDC_PLANE8 : 
			plane_index = 8; break;
		case IDC_PLANE9 : 
			plane_index = 9; break;
		case IDC_PLANE10 : 
			plane_index = 10; break;
	}

	display(plane_index,"  Plane: ", &m_status);
	j = strlen(file_cal_img)-5;
	strcpy(buffer,file_cal_img);
	sprintf(buffer+j, "%d.raw", plane_index);
	if(Read_image(buffer, image_data, ccd_init.height, ccd_init.width))
	{
		display(plane_index,"   Trouble reading image", &m_status);
		CAL_MODUS = 0;
		return;
	}


	//temporary image memory
	uInt8 * cal_img=new uInt8[640*480];

	//do some image enhancement
	if(ROW_CORR)
		Image_row_cor(image_data);
	//low-passs filtering
	//Box_filter(image_data, cal_img);
	if(GAUSS)
	{
		Gauss_filter(image_data, cal_img);
		Image_subtract(image_data, cal_img);
	}
	//Hist_equalization(image_data);
	//Background_leveling_min(image_data);

	//Plot_image(device, cal_img ,ccd_init.height, ccd_init.width, 1, 0);
	Plot_image(device, image_data ,ccd_init.height, ccd_init.width, 1, 0);
	histo(&image_data[0], hist);
	Plot_histo(device, hist,  1, 0, 0, 255);

	delete cal_img;
}



void CCamCalDlg::OnDetect() 
{
	int i,j,cal_thres, ref_point,s_nr_max;
	char file_name[255],buffer[255];
	FILE *data;

    device=this->GetDC();

	//*********check first if user specified a path for the working directory***************
	if(path[0] == '\0')
	{
		MessageBox("Specifiy the working path!",0, MB_ICONEXCLAMATION );
		return;
	}
	//**************************************************************************************

	CAL_MODUS=1;
	INS_MODUS=0;

	m_cal_thres.GetWindowText(buffer, 30);
	cal_thres = atoi(buffer);

	s_nr_max = Extract_streaks_roes (centr, image_data, cal_thres, min_pix, max_pix);
	if(s_nr_max==-1)
	{
		display(-99,"  Error", &m_status);
		return;
	}
	display(s_nr_max,"  # of contr points :", &m_status);

	Plot_points(device, color_image_data, centr, ccd_init.height, ccd_init.width, s_nr_max, 0, 255, 0,  1);

	//****************************************************************************************
	//takes the reference marker points and writes them into the file "local_us_plane1.dat"
	//Create_file(ref_markers1_par,  s_nr_max1, plane_index); 
	sprintf(file_name,path);
	strcat(file_name,"local_us_plane");
	j=strlen(file_name);
	sprintf(file_name+j, "%d.dat", plane_index);

	data = (FILE *)fopen(file_name,"w");
	fprintf(data,"%d\n", s_nr_max);

	for(i=0; i < s_nr_max; i++)
		fprintf(data,"%d %f %f\n", i, centr[i].x, centr[i].y);
		
	fclose(data);
	//****************************************************************************************


	m_ref_point.GetWindowText(buffer, 30);
	ref_point = atoi(buffer);
	display(-99,"   Delete point with right mouse button and", &m_status);
	display(ref_point,"   Select reference point with left mouse button: ", &m_status);

	x_pick=-1;
	y_pick=-1;
}





void CCamCalDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(CAL_MODUS)
	{

	 //*******************check first if user specified a path for the working directory*******
		if(file_global[0] == '\0')
		{
			MessageBox("Specifiy global files!",0, MB_ICONEXCLAMATION );
			return;
		}
		//check first if user specified a path for the working directory
		if(path[0] == '\0')
		{
			MessageBox("Specifiy the working path!",0, MB_ICONEXCLAMATION );
			return;
		}
	 //*******************end check ***********************************************************


		char buffer[256];
	    char file_name[255]; 
		int  ref_point, first_plane, dum;
		static int  match_nr_old, nr;
	//***********************Variables to plot control numbers*********************************
		FILE *dat= NULL;
		int	   i, j, dummy1;
		float  dummy2;
		int   *index;
		float *x, *y, *z, *xd, *yd;
		device=this->GetDC();
	//***********************End Variables to plot control numbers****************************

		x_pick = point.x - CANVASTOP1;
		y_pick = point.y - CANVASLEFT1;

		if(x_pick >= 0 && x_pick < ccd_init.width && y_pick >= 0 && y_pick < ccd_init.height )
		{
			m_ref_point.GetWindowText(buffer, 30);
			ref_point = atoi(buffer)-1;

		//**********************reads the points in file "local_us_plane.dat and*****************
		//**********************writes the sorted one into match.dat*****************************
			if(match_nr == -99)
			{
				match_nr_old = 0;
				first_plane = 1;
			}
			else 
			{
         		first_plane = 0;    
				match_nr_old += match_nr;
			}

			j=strlen(file_global)-5;

			sprintf(file_global+j, "%d.dat", plane_index);
			match_nr = Sort_points(ref_point, x_pick, y_pick, file_global, first_plane); 
			if(match_nr==-1){
				display(-99,"  Can not open file with global data", &m_status);	
				return;
			}
			if(match_nr==-2){
				display(-99,"  Can not open file \"local_us_plane\"", &m_status);	
				return;
			}
			if(match_nr!=0)
			{
				display(match_nr,"  Number of matches: ", &m_status);	
				if(first) first_plane_nr=match_nr;
			    //**********************Plot Control Number**************************************
				sprintf(file_name,path);
				strcat(file_name,"match.dat");
				dat= (FILE *) fopen (file_name,"r");
				if(dat==NULL)
				{
					display(-99,"  Can not open file \"match.dat\"", &m_status);	
					return;
				}

				fscanf (dat, "%d",&dum);
				for(i=0; i<match_nr_old; i++)
					fscanf (dat,"%d %d %f %f %f %f %f\n",&dummy1,&dummy1,
								  &dummy2,&dummy2,&dummy2,&dummy2,&dummy2);

				x= (float *) calloc (match_nr,sizeof(float));
				y= (float *) calloc (match_nr,sizeof(float));
				z= (float *) calloc (match_nr,sizeof(float));
				xd=(float *) calloc (match_nr,sizeof(float));
				yd=(float *) calloc (match_nr,sizeof(float));
				index=(int *) calloc (match_nr,sizeof(int));

				for (i=0;i<match_nr;i++) fscanf (dat,"%d %d %f %f %f %f %f\n",&index[i],&nr,
													&x[i],&y[i],&z[i],&xd[i],&yd[i]);
				fclose (dat);

				for(i=0; i < match_nr; i++)
					Plot_number(device, color_image_data, ccd_init.height, ccd_init.width, index[i],(int)xd[i],(int)yd[i], 1);	
				
				free(x); free(y); free(z); free(xd); free(yd); free(index);
		//**********************End Plot Control Number*******************************************
				CAL_MODUS=0;
			}
			else	display(-99,"  invalid point! Try again", &m_status);	
		}
//		else	display(0,"  invalid point! Try again", &m_status);	
	}


	if(INS_MODUS)
	{
		char buffer[256];
		int value;

		x_pick = point.x - CANVASTOP1;
		y_pick = point.y - CANVASLEFT1;

		if(x_pick >= 0 && x_pick < ccd_init.width && y_pick >= 0 && y_pick < ccd_init.height)
		{
			value=*(image_data+(y_pick*ccd_init.width+x_pick));

			sprintf(buffer, "Pixel position: (%d,%d) and value: %d", x_pick, y_pick, value);
			display(-99,buffer, &m_status);

		}
//		else	display(0,"  invalid point!", &m_status);	
	}
	CDialog::OnLButtonDown(nFlags, point);
}




void CCamCalDlg::OnNewSet() 
{
	match_nr = -99;
}





void CCamCalDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(CAL_MODUS){
		FILE *data;
		device=this->GetDC();
		int i, j, nr, found,i_del;

		int *index;
		float *x, *y;
		char file_name[255];

		x_pick = point.x - CANVASTOP1;
		y_pick = point.y - CANVASLEFT1;

		//check first if user specified a path for the working directory
		if(path[0] == '\0')
		{
			MessageBox("Specifiy the path of the working directory!",0, MB_ICONEXCLAMATION );
			return;
		}
		sprintf(file_name,path);
		strcat(file_name,"local_us_plane");
		j=strlen(file_name);
		sprintf(file_name+j, "%d.dat", plane_index);

		data = (FILE *)fopen(file_name,"r+");
		fscanf(data,"%d", &nr);

		index = (int *) calloc (nr, sizeof(int));
		x= (float *) calloc (nr,sizeof(float));
		y= (float *) calloc (nr,sizeof(float));

		for(i=0; i < nr; i++)   fscanf(data," %d %f %f\n", &index[i], &x[i], &y[i]);

		for(i=0; i < nr; i++){
			if(fabs(x[i] - x_pick) < 5 && fabs(y[i] - y_pick) < 5)
			{
				found=1;
				i_del=i;
				--nr;
				break;
			}
		}
		if(found==1)  // Delete the point from the list
		{
			fclose(data);
			data = (FILE *)fopen(file_name,"w");
			display(index[i],"   Deleted Point # : ", &m_status);
			//mark the deleted point with a red cross
			Plot_error_cross(device, color_image_data, ccd_init.height, ccd_init.width, (int)x[i_del],(int)y[i_del]);
			for (j=i; j< nr; j++)
				x[j]=x[j+1], y[j]=y[j+1], index[j]=index[j+1];

			int red=fprintf(data,"%d\n", nr);
			for(i=0; i < nr; i++)
				fprintf(data,"%d %f %f\n", index[i], x[i], y[i]);
		}
		else 
			display(-99,"  No Point found ", &m_status);	
		fclose(data);

		free(x);
		free(y);
	}
	CDialog::OnRButtonDown(nFlags, point);
}


void CCamCalDlg::OnLinCal() 
{
	int status;
	char buffer[255];
    device=this->GetDC();

	if(mem_alloc==0)
	{
		image_data =     (uInt8 *)calloc((  ccd_init.width*ccd_init.height),sizeof(uInt8)); //image memory
		color_image_data = (uInt8 *)malloc(3*ccd_init.width*ccd_init.height*sizeof(uInt8));
	}

	if(status = Rd_matched_points())
	{
		if(status == -1)
			display(-99,"  Could not open file \"match.dat\" ", &m_status);	
		else if(status == 1)
			display(-99,"  MAX_MATCH too small ", &m_status);	
		return;
	}

	Lin_calibrate3(device);

	gcvt(ex_init.X_0, 8, buffer);
	m_x_0.SetWindowText(buffer);
	gcvt(ex_init.Y_0, 8, buffer);
	m_y_0.SetWindowText(buffer);
	gcvt(ex_init.Z_0,8, buffer);
	m_z_0.SetWindowText(buffer);

	gcvt(ex_init.omega , 8, buffer);
	m_omega.SetWindowText(buffer);
	gcvt(ex_init.phi , 8, buffer);
	m_phi.SetWindowText(buffer);
	gcvt(ex_init.kappa , 8, buffer);
	m_kappa.SetWindowText(buffer );

	gcvt(in_init.focus , 8, buffer);
	m_focus.SetWindowText(buffer );	
	gcvt(in_init.x_h , 8, buffer);
	m_x_h.SetWindowText(buffer );
	gcvt(in_init.y_h , 8, buffer); 
	m_y_h.SetWindowText(buffer );
	gcvt(in_init.s_x , 8, buffer);
	m_s_x.SetWindowText(buffer );

	gcvt(rms , 5, buffer);
	m_rms.SetWindowText(buffer );

	display(-99,"Calibration done", &m_status);
	display(-99,"Save parameters?", &m_status);
}


void CCamCalDlg::OnCalCam() 
{
	int i,status;
	char buffer[255];
    device=this->GetDC();

	if(mem_alloc==0)
	{
		image_data =     (uInt8 *)calloc((  ccd_init.width*ccd_init.height),sizeof(uInt8)); //image memory
		color_image_data = (uInt8 *)malloc(3*ccd_init.width*ccd_init.height*sizeof(uInt8));
	}



	if(first){
		//this parameters are the initials values for the nonlin alg.
		//they are read when the code is started and when the button
		//"Reset and Reread Initials parameters"is pressed". In this case
		// routine "OnResetPara" is called
		m_x_0.GetWindowText(buffer, 30);
		ex_init.X_0 = atof(buffer);
		m_y_0.GetWindowText(buffer, 30);
		ex_init.Y_0 = atof(buffer);
		m_z_0.GetWindowText(buffer, 30);
		ex_init.Z_0 = atof(buffer);

		m_omega.GetWindowText(buffer, 30);
		ex_init.omega = atof(buffer);
		m_phi.GetWindowText(buffer, 30);
		ex_init.phi = atof(buffer);
		m_kappa.GetWindowText(buffer, 30);
		ex_init.kappa = atof(buffer);

		m_focus.GetWindowText(buffer, 30);
		in_init.focus = atof(buffer);
		m_x_h.GetWindowText(buffer, 30);
		in_init.x_h = atof(buffer);
		m_y_h.GetWindowText(buffer, 30);
		in_init.y_h = atof(buffer); 
		m_s_x.GetWindowText(buffer, 30);
		in_init.s_x = atof(buffer);
		m_theta.GetWindowText(buffer, 30);
		in_init.theta = atof(buffer);

		m_k1.GetWindowText(buffer, 30);
		in_init.k1 = atof(buffer);
		m_k2.GetWindowText(buffer, 30);
		in_init.k2 = atof(buffer);
		m_p1.GetWindowText(buffer, 30);
		in_init.p1 = atof(buffer);
		m_p2.GetWindowText(buffer, 30);
		in_init.p2 = atof(buffer);


		first=0;
	}

	display(-99,"  Calibrating...", &m_status);

	for(i=0;i<17;i++)
		var[i]=0;
	if(m_ctr_x0.GetCheck())		var[0]=1;
	if(m_ctr_y0.GetCheck())		var[1]=1;
	if(m_ctr_z0.GetCheck())		var[2]=1;
	if(m_ctr_omega.GetCheck())	var[3]=1;
	if(m_ctr_phi.GetCheck())	var[4]=1;
	if(m_ctr_kappa.GetCheck())	var[5]=1;
	if(m_ctr_focus.GetCheck())	var[6]=1;
	if(m_ctr_xh.GetCheck())		var[7]=1;
	if(m_ctr_yh.GetCheck())		var[8]=1;
	if(m_ctr_sx.GetCheck())		var[9]=1;
	if(m_ctr_theta.GetCheck())	var[10]=1;
	if(m_ctr_k1.GetCheck())		var[11]=1;
	if(m_ctr_k2.GetCheck())		var[12]=1;
	if(m_ctr_p1.GetCheck())		var[13]=1;
	if(m_ctr_p2.GetCheck())		var[14]=1;

	if(status = Rd_matched_points())
	{
		if(status == -1)
			display(-99,"  Could not open file \"match.dat\" ", &m_status);	
		else if(status == 1)
			display(-99,"  MAX_MATCH too small ", &m_status);	
	}
	status = Calibrate(device);

	gcvt(ex_init.X_0, 8, buffer);
	m_x_0.SetWindowText(buffer);
	gcvt(ex_init.Y_0, 8, buffer);
	m_y_0.SetWindowText(buffer);
	gcvt(ex_init.Z_0,8, buffer);
	m_z_0.SetWindowText(buffer);

	gcvt(ex_init.omega , 8, buffer);
	m_omega.SetWindowText(buffer);
	gcvt(ex_init.phi , 8, buffer);
	m_phi.SetWindowText(buffer);
	gcvt(ex_init.kappa , 8, buffer);
	m_kappa.SetWindowText(buffer );

	gcvt(in_init.focus , 8, buffer);
	m_focus.SetWindowText(buffer );

	gcvt(in_init.x_h , 8, buffer);
	m_x_h.SetWindowText(buffer );
	gcvt(in_init.y_h , 8, buffer); 
	m_y_h.SetWindowText(buffer );

	gcvt(in_init.s_x , 8, buffer);
	m_s_x.SetWindowText(buffer );
	gcvt(in_init.theta , 8, buffer);
	m_theta.SetWindowText(buffer );

	gcvt(in_init.k1 , 8, buffer);
	m_k1.SetWindowText(buffer );
	gcvt(in_init.k2 , 8, buffer);
	m_k2.SetWindowText(buffer );
	gcvt(in_init.p1 , 8, buffer);
	m_p1.SetWindowText(buffer );
	gcvt(in_init.p2 , 8, buffer);
	m_p2.SetWindowText(buffer );

	gcvt(rms , 5, buffer);
	m_rms.SetWindowText(buffer );

	//IMSL_MIN_AT_LOWERBOUND           =   8004,
	//IMSL_MIN_AT_UPPERBOUND           =   8005,
	//IMSL_STEP_TOLERANCE              =   8006,
	//IMSL_LINEAR_CONSTRAINT_VALUE     =   8007,
	//IMSL_NEQ_CANNOT_BE_GT_NCON       =   8008,
	//IMSL_MATRIX_ORDER_NOT_POSITIVE   =   2029,
	//MSL_LITTLE_FCN_CHANGE            =   8064
	//IMSL_TOO_MANY_FCN_EVAL           =   8029

	if(status == 1)
	{
		display(-99,"  ERROR: array too small", &m_status);
		display(-99,"  in int Calibrate()", &m_status);
		return;
	}
	else if(status == -1)
	{
		display(-99,"  ERROR: could not open \"match.dat\"", &m_status);
		return;
	}	
	else if(status == 8004)
	{
		display(status,"  WARNING :", &m_status);
		display(-99,"  IMSL_MIN_AT_LOWERBOUND", &m_status);
	}
	else if(status == 8005)
	{
		display(status,"  WARNING :", &m_status);
		display(-99,"  IMSL_MIN_AT_UPPERBOUND", &m_status);
	}
	else if(status == 8006)
	{
		display(status,"  WARNING :", &m_status);
		display(-99,"  IMSL_STEP_TOLERANCE", &m_status);
	}
	else if(status == 8007)
	{
		display(status,"  WARNING :", &m_status);
		display(-99,"  IMSL_LINEAR_CONSTRAINT_VALUE", &m_status);
	}
	else if(status == 8008)
	{
		display(status,"  WARNING :", &m_status);
		display(-99,"  IMSL_NEQ_CANNOT_BE_GT_NCON", &m_status);
	}
	else if(status == 2029)
	{
		display(status,"  WARNING :", &m_status);
		display(-99,"  IMSL_MATRIX_ORDER_NOT_POSITIVE", &m_status);
	}
	else if(status == 8064)
	{
		display(status,"  WARNING :", &m_status);
		display(-99,"  IMSL_MATRIX_ORDER_NOT_POSITIVE", &m_status);
	}
	else if(status == 8029 )
	{
		display(status,"  WARNING :", &m_status);
		display(-99,"  IMSL_TOO_MANY_FCN_EVAL ", &m_status);
	}
	else if(status > 0 )
	{
		display(status,"  WARNING :", &m_status);
	}

	display(-99,"Calibration done", &m_status);
	display(-99,"Save parameters?", &m_status);
}


void CCamCalDlg::OnResetPara() 
{
	first=1;

	m_x_0.SetWindowText("120");
	m_y_0.SetWindowText("75");
	m_z_0.SetWindowText("-1200");
	m_omega.SetWindowText("3.0");
	m_phi.SetWindowText("0.0");
	m_kappa.SetWindowText("0.0");
	m_focus.SetWindowText("8.0");
	m_x_h.SetWindowText("0.0");
	m_y_h.SetWindowText("0.0");
	m_s_x.SetWindowText("1.0");
	m_theta.SetWindowText("0.0");
	m_k1.SetWindowText("0.0");
	m_k2.SetWindowText("0.0");
	m_p1.SetWindowText("0.0");
	m_p2.SetWindowText("0.0");
}



void CCamCalDlg::OnOpenCalImage() 
{
	int ans;

	CFileDialog dlg(TRUE,
		NULL,
		NULL, 0,
		"Images in raw format |*.raw||", this);

	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		SetDlgItemText(IDC_FILE_CAL_IMG,dlg.GetPathName());
		GetDlgItemText(IDC_FILE_CAL_IMG,file_cal_img,100);
		m_file_cal_img = dlg.GetFileTitle();

		// copy output data back into dialog...
		UpdateData(FALSE);
	}
	return;
}


void CCamCalDlg::OnOpenGlobalFile() 
{
	int ans;

	CFileDialog dlg(TRUE,
		NULL,
		NULL, 0,
		"Files containing global data |*.dat|all files|*.*||", this);

	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		SetDlgItemText(IDC_FILE_GLOBAL,dlg.GetPathName());
		GetDlgItemText(IDC_FILE_GLOBAL,file_global,100);
		m_file_global =  dlg.GetFileTitle();

		// copy output data back into dialog...	
		UpdateData(FALSE);
	}
	return;
}



void CCamCalDlg::OnPath() 
{
	int ans,j;


	CFileDialog dlg(TRUE,
		NULL,
		NULL, 0,
		"", this);

	ans=dlg.DoModal();

	if(ans==IDOK)
	{
		SetDlgItemText(IDC_DIR_PATH,dlg.GetPathName());
		GetDlgItemText(IDC_DIR_PATH,path,100);

		j=strlen(path);

		while(path[j] != 92)
			j--;
		path[++j]='\0';
		m_dir_name = dlg.GetPathName();
		//SetDlgItemText(IDC_DIR_PATH,path);
	}
	return;	
}


void CCamCalDlg::OnGet3d() 
{

	Get_3D_DialogDlg dlg(this);

	dlg.DoModal();

	return;			


}


void CCamCalDlg::OnGetLookUp() 
{

	int ans;
	char  *file_cam;
	CString dummy1,dummy2;

	display(-99,"Specify Camera File", &m_status);	

	CFileDialog dlg(TRUE,
		NULL,
		NULL, 0,
		"Camera Parameters|*.par|all files|*.*||", this);

	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		dummy2 = dlg.GetPathName();
		file_cam=dummy2.GetBuffer(1);

		Read_cam_param(&ex_init, &in_init, file_cam);

		display(-99,"Calculating Look Up Table", &m_status);	
		Get_table(file_cam, &m_bar);
		display(-99,"Look Up Table Done", &m_status);
	}

	return;
}



void CCamCalDlg::OnFileSavenonlin() 
{
	int ans;
	char file_name[255];
	ofstream outdat;

	CFileDialog dlg(FALSE,
		NULL,
		NULL, 0,
		NULL, this);

	// show it to the user
	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		strcpy(file_name, dlg.GetPathName());	
		outdat.open (file_name);

		char tmpbuf[255];
		_strdate(tmpbuf);
 
		//Output into file
		outdat << "Extrinsic Parameters of Camera" << endl;
		outdat << "X,Y,Z [mm] omega,phi,kappa [rad]" << endl;
		outdat << ex_init.X_0   << endl;
		outdat << ex_init.Y_0   << endl;
		outdat << ex_init.Z_0   << endl;
		outdat << ex_init.omega << endl;
		outdat << ex_init.phi   << endl;
		outdat << ex_init.kappa << endl;
		outdat << "Intrinsic Parameters of Camera" << endl;
		outdat << "focus [mm] u_0[mm] v_0[mm] s_x[mm] phi[rad] k1, k2, p1, p2" << endl; 
		outdat << in_init.focus << endl;
		outdat << in_init.x_h   << endl;
		outdat << in_init.y_h   << endl;
		outdat << in_init.s_x   << endl;
		outdat << in_init.theta << endl;
		outdat << in_init.k1    << endl;
		outdat << in_init.k2    << endl;
		outdat << in_init.p1    << endl;
		outdat << in_init.p2    << endl;
		outdat << "RMS[mm]  " << rms    << endl;
		outdat << endl;
		outdat << "Cal. images used:		" << file_cal_img   << endl;
		outdat << "Number of planes used:   " << plane_index+1  << endl;
		outdat << "Number of markers:       " << number         << endl;
		outdat << "Data created:            " << tmpbuf		    << endl;
		outdat.close();		
		
		display(-99,"Camera Pararameter saved", &m_status);	
	}
	else display(-99,"Camera Pararameter not saved", &m_status);	
}

void CCamCalDlg::OnFileSavelin() 
{
	int ans;
	char file_name[255];
	ofstream outdat;

	CFileDialog dlg(FALSE,
		NULL,
		NULL, 0,
		NULL, this);

	// show it to the user
	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		strcpy(file_name, dlg.GetPathName());	
		outdat.open (file_name);

		//Output into file
		outdat << "P-Matrix 3 rows and 4 columns" << endl;
		outdat << P.p11 << endl;
		outdat << P.p12 << endl;
		outdat << P.p13 << endl;
		outdat << P.p14 << endl;
		outdat << P.p21 << endl;
		outdat << P.p22 << endl;
		outdat << P.p23 << endl;
		outdat << P.p24 << endl;
		outdat << P.p31 << endl; 
		outdat << P.p32 << endl;
		outdat << P.p33 << endl;
		outdat << P.p34 << endl;
		outdat.close();		
		
		display(-99,"P Matrix saved", &m_status);	
	}
	else display(-99,"P Matrix not saved", &m_status);
	

}

void CCamCalDlg::OnHelpAbout() 
{
	CDialog dlg(IDHELP, this);
	dlg.DoModal();
	return;	
}

void CCamCalDlg::OnCCDparSet() 
{
	ccd_parDlg dlg(this);

	dlg.DoModal();

	ccd_init.height = dlg.m_image_height;
	ccd_init.width  = dlg.m_image_width;
	ccd_init.pix_h  = dlg.m_pixle_height;
	ccd_init.pix_w  = dlg.m_pixle_width;
	return;	
}

void CCamCalDlg::OnImageProc() 
{
	CMenu *menu;
	menu = GetMenu();
	if(menu->GetMenuState(IDD_GAUSS, MF_UNCHECKED) == MF_CHECKED)
	{
		menu->CheckMenuItem(IDD_GAUSS, MF_UNCHECKED);
		GAUSS=0;
	}
	else
	{
		menu->CheckMenuItem(IDD_GAUSS, MF_CHECKED);
		GAUSS=1;
	}
}
