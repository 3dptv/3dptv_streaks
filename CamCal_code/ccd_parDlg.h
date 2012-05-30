#if !defined(AFX_CCD_PARDLG_H__F7B2CF23_358E_11D5_AF80_0050DA26BBDE__INCLUDED_)
#define AFX_CCD_PARDLG_H__F7B2CF23_358E_11D5_AF80_0050DA26BBDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ccd_parDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ccd_parDlg dialog

class ccd_parDlg : public CDialog
{
// Construction
public:
	ccd_parDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ccd_parDlg)
	enum { IDD = IDD_CCD_PAR_DIALOG };
	int		m_image_height;
	int		m_image_width;
	double	m_pixle_height;
	double	m_pixle_width;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ccd_parDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ccd_parDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCD_PARDLG_H__F7B2CF23_358E_11D5_AF80_0050DA26BBDE__INCLUDED_)
