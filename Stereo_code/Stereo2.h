// Stereo2.h : main header file for the STEREO2 application
//

#if !defined(AFX_STEREO2_H__70CFDE65_FE63_11D3_AF21_0050DA26B74C__INCLUDED_)
#define AFX_STEREO2_H__70CFDE65_FE63_11D3_AF21_0050DA26B74C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CStereo2App:
// See Stereo2.cpp for the implementation of this class
//

class CStereo2App : public CWinApp
{
public:
	CStereo2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStereo2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CStereo2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STEREO2_H__70CFDE65_FE63_11D3_AF21_0050DA26B74C__INCLUDED_)
