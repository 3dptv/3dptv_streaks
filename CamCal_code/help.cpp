// help.cpp : implementation file
//

#include "stdafx.h"
#include "CamCal.h"
#include "help.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// help dialog


help::help(CWnd* pParent /*=NULL*/)
	: CDialog(help::IDD, pParent)
{
	//{{AFX_DATA_INIT(help)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void help::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(help)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(help, CDialog)
	//{{AFX_MSG_MAP(help)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// help message handlers
