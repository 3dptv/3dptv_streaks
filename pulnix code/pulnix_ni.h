// pulnix_ni.h : main header file for the PULNIX_NI application
//

#if !defined(AFX_PULNIX_NI_H__720169B5_E9C5_11D3_AF0C_0050DA26B74C__INCLUDED_)
#define AFX_PULNIX_NI_H__720169B5_E9C5_11D3_AF0C_0050DA26B74C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPulnix_niApp:
// See pulnix_ni.cpp for the implementation of this class
//

class CPulnix_niApp : public CWinApp
{
public:
	CPulnix_niApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPulnix_niApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPulnix_niApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PULNIX_NI_H__720169B5_E9C5_11D3_AF0C_0050DA26B74C__INCLUDED_)
