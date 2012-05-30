// Get_3D_DialogDlg.cpp : implementation file
//
#include "stdafx.h"
#include <malloc.h>
#include "CamCal.h"
#include "Get_3D_DialogDlg.h"
#include "niimaq.h"
#include "parameters.h"
#include "structures.h"
#include "plot_functions.h"
#include "io_functions.h"
#include "cal_3d.h"
#include "ccd_parDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//**********************file and path names******************************************
char file_image1[255];			//file name of stereo image pair 
char file_cam1_par[255];		//file name of the camera parameter file
char file_image2[255];			//file name of stereo image pair 
char file_cam2_par[255];		//file name of the camera parameter file
char file_global_coord[255];	//file name of the global coordinates
char file_global_exp_coord[255];//file name of the measured global coordinates
char file_ref_image1[255];		//file name of stereo reference image pair 
char file_ref_image2[255];		//file name of stereo reference image pair 
//************************************************************************************

CDC			 *device2;
CCD_par		  ccd1,ccd2;

int	limits_off;
int limits[7];
int TABLE_1_LOADED;
int TABLE_2_LOADED;
double av_dist_h[6],av_dist_p[6], av_tdist_p;
int m[6];
double E_res=0;
int E_res_nr=0;
bool REF;

Vec2     table_data1[640*480], table_data2[640*480];
Vec2     *table1=table_data1, *table2=table_data2;

/////////////////////////////////////////////////////////////////////////////
// Get_3D_DialogDlg dialog


Get_3D_DialogDlg::Get_3D_DialogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Get_3D_DialogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Get_3D_DialogDlg)
	m_zmin = 0;
	m_zmax = 0;
	m_thresh = 0;
	m_tolerance = 0.0;
	//}}AFX_DATA_INIT
}


void Get_3D_DialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Get_3D_DialogDlg)
	DDX_Control(pDX, IDC_LIMITS, m_limits_off);
	DDX_Control(pDX, IDC_PROGRESS1, m_bar);
	DDX_Control(pDX, IDC_STATUS, m_status);
	DDX_Text(pDX, IDC_ZMIN, m_zmin);
	DDX_Text(pDX, IDC_ZMAX, m_zmax);
	DDX_Text(pDX, IDC_THRESHOLD, m_thresh);
	DDX_Text(pDX, IDC_TOLERANCE, m_tolerance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Get_3D_DialogDlg, CDialog)
	//{{AFX_MSG_MAP(Get_3D_DialogDlg)
	ON_BN_CLICKED(IDC_DETECT_POINTS, OnDetectPoints)
	ON_BN_CLICKED(IDC_SHOW_IMAGES, OnShowImages)
	ON_BN_CLICKED(IDC_GET_EPI_3D, OnGetEpi3d)
	ON_BN_CLICKED(IDC_FILE_STEREO_POS, OnFileStereoPos)
	ON_BN_CLICKED(IDC_FILE_STEREO_ANALYSE, OnFileStereoAnalyse)
	ON_BN_CLICKED(IDC_FILE_STEREO_EXP_POS, OnFileStereoExpPos)
	ON_COMMAND(ID_FILE_CAMERA1IMAGE, OnFileStereoImg1)
	ON_COMMAND(ID_FILE_CAMERA2IMAGE, OnFileStereoImg2)
	ON_COMMAND(ID_FILE_CAMERA1PARAMETER, OnFileCam1Par)
	ON_COMMAND(ID_FILE_CAMERA2PARAMETER, OnFileCam2Par)
	ON_COMMAND(ID_CCD_CCD1PARAMETER, OnCcdPar)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_CAMERA1REFIMAGE, OnFileCam1refimg)
	ON_COMMAND(ID_FILE_CAMERA2REFIMAGE, OnFileCam2refimg)
	ON_BN_CLICKED(IDC_GET_EPI_3D_LIN, OnGetEpi3d)
	ON_COMMAND(ID_CCD_CCD2PARAMETER, OnCcdPar)
	ON_COMMAND(ID_IMAGEPROCESSING_SUBTRACTREFIMAGE, OnImgSub)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Get_3D_DialogDlg message handlers


BOOL Get_3D_DialogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	table1=(Vec2*)calloc((640*480),sizeof(Vec2));
//	table2=(Vec2*)calloc((640*480),sizeof(Vec2));

    m_thresh=120;
	m_zmin	=30;
	m_zmax	=30;
	m_tolerance = 0.001;
	
	UpdateData(FALSE);

	ccd1.height=480;
	ccd1.width =640;
	ccd1.pix_h =0.009;
	ccd1.pix_w =0.009;

	ccd2.height=480;
	ccd2.width =640;
	ccd2.pix_h =0.009;
	ccd2.pix_w =0.009;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}





void Get_3D_DialogDlg::OnFileStereoImg1() 
{
	int ans;

	CFileDialog dlg(TRUE,
		NULL,
		NULL, 0,
		"Images in raw format |*.raw||", this);

	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		CString filestring = dlg.GetPathName();
		strcpy(file_image1,filestring.GetBuffer(100));
	}
	return;	
}


void Get_3D_DialogDlg::OnFileStereoImg2() 
{
	int ans;

	CFileDialog dlg(TRUE,
		NULL,
		NULL, 0,
		"Images in raw format |*.raw||", this);

	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		CString filestring = dlg.GetPathName();
		strcpy(file_image2,filestring.GetBuffer(100));
	}
	return;	
	

}


void Get_3D_DialogDlg::OnFileCam1Par() 
{
	int ans;

	CFileDialog dlg(TRUE,
		NULL,
		NULL, 0,
		"Camera parameters |*.par|Linear Camera parameters|*.p|all files|*.*||", this);

	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		CString filestring = dlg.GetPathName();
		strcpy(file_cam1_par,filestring.GetBuffer(100));
		TABLE_1_LOADED=0;
	}
	return;		
}


void Get_3D_DialogDlg::OnFileCam2Par() 
{
	int ans;

	CFileDialog dlg(TRUE,
		NULL,
		NULL, 0,
		"Camera parameters |*.par|Linear Camera parameters|*.p|all files|*.*||", this);

	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		CString filestring = dlg.GetPathName();
		strcpy(file_cam2_par,filestring.GetBuffer(100));
		TABLE_2_LOADED=0;
	}
	return;	
}



void Get_3D_DialogDlg::OnShowImages() 
{
	device2=this->GetDC();
		
	if(file_image1[0] == '\0' || file_image2[0] == '\0')
	{
		MessageBox("Specifiy the Images!",0, MB_ICONEXCLAMATION );
		return;
	}
	else 
		show_img(device2);	
}



void Get_3D_DialogDlg::OnDetectPoints() 
{
    device2=this->GetDC();
	UpdateData(TRUE);
	get_points(device2, m_thresh);
}



void Get_3D_DialogDlg::OnGetEpi3d() 
{
	int i;
	device2=this->GetDC();

	UpdateData(TRUE);
	limits_off=m_limits_off.GetCheck();

	//initialize the variables for the calibration quality control
	for(i=0;i<6;i++)
	{
		av_dist_h[i]=0;
		av_dist_p[i]=0;
		m[i]=0;
	}
	E_res=0;
	E_res_nr=0;

	switch(GetCurrentMessage()->wParam)
	{
		case IDC_GET_EPI_3D_LIN:

			if(file_cam1_par[0] == '\0' || file_cam2_par[0] == '\0')
			{
				MessageBox("Specifiy the LINEAR Camera Parameter Files!",0, MB_ICONEXCLAMATION );
				return;
			}
			else
			{
				UpdateData(TRUE);
				int nr=get_3d_lin(device2, m_zmin, m_zmax, m_tolerance);
				display(nr,"  Number of matched points", &m_status);
			}
			break;
		case IDC_GET_EPI_3D:

			if(file_cam1_par[0] == '\0' || file_cam2_par[0] == '\0')
			{
				MessageBox("Specifiy the NON-LINEAR Camera Parameter Files!",0, MB_ICONEXCLAMATION );
				return;
			}
			else 
			{
				UpdateData(TRUE);
				CMenu *menu;
				menu = GetMenu();
				if(menu->GetMenuState(ID_IMAGEPROCESSING_SUBTRACTREFIMAGE, MF_UNCHECKED) == MF_CHECKED)
				{
					REF=TRUE;
				}
				else 
					REF=FALSE;

				int nr=get_3d(device2, m_zmin, m_zmax, m_tolerance, &m_bar);
				display(nr,"  Number of matched points", &m_status);

				if(E_res_nr !=0)
				{
					display(-99,"  Average essent.matrix res", &m_status);
					E_res=E_res/E_res_nr;
					display_double(E_res,      "  Res=  ", &m_status);
				}
				display_double(-99,"  averaged distance", &m_status);
				display_double(-99,"  point_epi", &m_status);
				for(i=0;i<6;i++)
				{
					if(m[i] != 0)
					{
						av_dist_h[i]=av_dist_h[i]/(double)m[i];
						av_dist_p[i]=av_dist_p[i]/(double)m[i]/ccd2.pix_h;
					}
				}
				display_double(m[0],      "  samples0=  ", &m_status);
				display_double(av_dist_h[0],"  av_dist_h=  ", &m_status);
				display_double(av_dist_p[0],"  av_dist_p=  ", &m_status);
				display_double(m[1],      "  samples1=  ", &m_status);
				display_double(av_dist_h[1],"  av_dist_h=  ", &m_status);
				display_double(av_dist_p[1],"  av_dist_p=  ", &m_status);
				display_double(m[2],      "  samples2=  ", &m_status);
				display_double(av_dist_h[2],"  av_dist_h=  ", &m_status);
				display_double(av_dist_p[2],"  av_dist_p=  ", &m_status);
				display_double(m[3],      "  samples3=  ", &m_status);
				display_double(av_dist_h[3],"  av_dist_h=  ", &m_status);
				display_double(av_dist_p[3],"  av_dist_p=  ", &m_status);
				display_double(m[4],      "  samples4=  ", &m_status);
				display_double(av_dist_h[4],"  av_dist_h=  ", &m_status);
				display_double(av_dist_p[4],"  av_dist_p=  ", &m_status);
				display_double(m[5],      "  samples5=  ", &m_status);
				display_double(av_dist_h[5],"  av_dist_h=  ", &m_status);
				display_double(av_dist_p[5],"  av_dist_p=  ", &m_status);

				char file[255];
				strcpy(file,file_cam1_par);
				char *c=file+strlen(file);
				while(*c != '\\')
					*c--;
				*c='\0';
				strcat(file,"\\accuracy.dat");
				FILE *fp=fopen(file,"wb");
				for(i=0;i<6;i++)
				{
					fprintf(fp,"Samples: %i\n",m[i]);
					fprintf(fp,"Average distance: %8.4lf [hom],  %8.4lf [pixel]\n",av_dist_h[0],av_dist_p[i]);
				}
				fprintf(fp,"The essential matrix residual x2^T E x1\n");
				fprintf(fp,"%8.4lf\n",E_res);
				char tmpbuf[255];
				_strdate(tmpbuf);
				fprintf(fp, "Data created: %s",tmpbuf);
			}
			break;			
	}
}


void Get_3D_DialogDlg::OnCcdPar() 
{
	ccd_parDlg dlg(this);

	dlg.DoModal();

	switch(GetCurrentMessage()->wParam)
	{
		case IDC_CCD1 : 
			ccd1.height = dlg.m_image_height;
			ccd1.width  = dlg.m_image_width;
			ccd1.pix_h  = dlg.m_pixle_height;
			ccd1.pix_w  = dlg.m_pixle_width;
			break;
		case IDC_CCD2: 
			ccd2.height = dlg.m_image_height;
			ccd2.width  = dlg.m_image_width;
			ccd2.pix_h  = dlg.m_pixle_height;
			ccd2.pix_w  = dlg.m_pixle_width;
			break;
	}
	return;	
}


void Get_3D_DialogDlg::OnFileStereoPos() 
{

	int ans,i=0;
	char *p;
	CString s;

	CFileDialog dlg(TRUE,
		NULL,
		NULL, 0,
		"Global Coordinates |*.dat|all files|*.*||", this);

	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		s = dlg.GetPathName();
		p = s.GetBuffer(3);
		while(*p)
		{
			file_global_coord[i]=*p;
			p++;
			i++;
		}
		file_global_coord[i]='\0';
	}
	return;		
}

void Get_3D_DialogDlg::OnFileStereoExpPos() 
{
	int ans,i=0;
	char *p;
	CString s;

	CFileDialog dlg(TRUE,
		NULL,
		NULL, 0,
		"Measured Global Coordinates |*.3D|all files|*.*||", this);

	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		s = dlg.GetPathName();
		p = s.GetBuffer(3);
		while(*p)
		{
			file_global_exp_coord[i]=*p;
			p++;
			i++;
		}
		file_global_exp_coord[i]='\0';
	}
	return;		
}



void Get_3D_DialogDlg::OnFileStereoAnalyse() 
{
	int tot_samp;
	double x_diff[10],y_diff[10],z_diff[10],diff[10];
	int s_nr[10];

	tot_samp = Get_accuracy(x_diff, y_diff, z_diff, diff, s_nr);

	if(tot_samp==-1)
			display(-99,"Error reading global file", &m_status);
	else if(tot_samp==-2)
			display(-99,"Error reading exp global file", &m_status);
	else 
	{
		display_double(tot_samp,      "Total # of samples=  ", &m_status);

		display_double(s_nr[0],      "  samples=  ", &m_status);
		display_double(x_diff[0],"  x_diff=  ", &m_status);
		display_double(y_diff[0],"  y_diff=  ", &m_status);
		display_double(z_diff[0],"  z_diff=  ", &m_status);
		display_double(diff[0],"      diff=  ", &m_status);
	}
}


void Get_3D_DialogDlg::OnOK() 
{

//	free(table1);
//	free(table2);
	
	CDialog::OnOK();
}


void Get_3D_DialogDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	device2=this->GetDC();
	DrawScreen(device2);

	CDialog::OnMouseMove(nFlags, point);
}



void Get_3D_DialogDlg::OnFileCam1refimg() 
{
	int ans;

	CFileDialog dlg(TRUE,
		NULL,
		NULL, 0,
		"Images in raw format |*.raw||", this);

	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		CString filestring = dlg.GetPathName();
		strcpy(file_ref_image1,filestring.GetBuffer(100));
	}
	return;		
}

void Get_3D_DialogDlg::OnFileCam2refimg() 
{
	int ans;

	CFileDialog dlg(TRUE,
		NULL,
		NULL, 0,
		"Images in raw format |*.raw||", this);

	ans=dlg.DoModal();
	if(ans==IDOK)
	{
		CString filestring = dlg.GetPathName();
		strcpy(file_ref_image2,filestring.GetBuffer(100));
	}
	return;	
	
}

void Get_3D_DialogDlg::OnImgSub() 
{
	CMenu *menu;
	menu = GetMenu();
	if(menu->GetMenuState(ID_IMAGEPROCESSING_SUBTRACTREFIMAGE, MF_UNCHECKED) == MF_CHECKED)
	{
		menu->CheckMenuItem(ID_IMAGEPROCESSING_SUBTRACTREFIMAGE, MF_UNCHECKED);
		REF=0;
	}
	else
	{
		menu->CheckMenuItem(ID_IMAGEPROCESSING_SUBTRACTREFIMAGE, MF_CHECKED);
		REF=1;
	}
	
}
