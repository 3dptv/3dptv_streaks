#if !defined(AFX_HELP_H__B4A10703_1886_11D5_AF7D_0050DA26BBDE__INCLUDED_)
#define AFX_HELP_H__B4A10703_1886_11D5_AF7D_0050DA26BBDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// help.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// help dialog

class help : public CDialog
{
// Construction
public:
	help(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(help)
	enum { IDD = IDHELP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(help)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(help)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELP_H__B4A10703_1886_11D5_AF7D_0050DA26BBDE__INCLUDED_)
