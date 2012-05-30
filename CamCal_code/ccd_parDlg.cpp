// ccd_parDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CamCal.h"
#include "ccd_parDlg.h"
#include "Get_3D_DialogDlg.h"
#include "niimaq.h"
#include "parameters.h"
#include "structures.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCD_par ccd1;
extern CCD_par ccd2;

/////////////////////////////////////////////////////////////////////////////
// ccd_parDlg dialog


ccd_parDlg::ccd_parDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ccd_parDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ccd_parDlg)
	m_image_height = 480;
	m_image_width = 640;
	m_pixle_height = 0.009;
	m_pixle_width = 0.009;
	//}}AFX_DATA_INIT
}


void ccd_parDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ccd_parDlg)
	DDX_Text(pDX, IDC_IMAGE_HEIGHT, m_image_height);
	DDV_MinMaxInt(pDX, m_image_height, 1, 5000);
	DDX_Text(pDX, IDC_IMAGE_WIDTH, m_image_width);
	DDV_MinMaxInt(pDX, m_image_width, 1, 5000);
	DDX_Text(pDX, IDC_PIXEL_HEIGHT, m_pixle_height);
	DDV_MinMaxDouble(pDX, m_pixle_height, 1.e-004, 1.);
	DDX_Text(pDX, IDC_PIXEL_WIDTH, m_pixle_width);
	DDV_MinMaxDouble(pDX, m_pixle_width, 1.e-004, 1.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ccd_parDlg, CDialog)
	//{{AFX_MSG_MAP(ccd_parDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ccd_parDlg message handlers

BOOL ccd_parDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();




//	SetDlgItemText(IDC_IMAGE_HEIGHT,);
//	SetDlgItemText(IDC_IMAGE_WIDTH, );
//	SetDlgItemText(IDC_PIXEL_HEIGHT,);
//	SetDlgItemText(IDC_PIXEL_WIDTH, );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
