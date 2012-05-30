// CamCal.h : main header file for the CAMCAL application
//

#if !defined(AFX_CAMCAL_H__72B04AE7_8E0F_11D4_AF3F_005004D8666E__INCLUDED_)
#define AFX_CAMCAL_H__72B04AE7_8E0F_11D4_AF3F_005004D8666E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCamCalApp:
// See CamCal.cpp for the implementation of this class
//

class CCamCalApp : public CWinApp
{
public:
	CCamCalApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamCalApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCamCalApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMCAL_H__72B04AE7_8E0F_11D4_AF3F_005004D8666E__INCLUDED_)
