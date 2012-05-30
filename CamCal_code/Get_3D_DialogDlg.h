#if !defined(AFX_GET_3D_DIALOGDLG_H__9FF24466_2CC2_11D5_AF7E_0050DA26BBDE__INCLUDED_)
#define AFX_GET_3D_DIALOGDLG_H__9FF24466_2CC2_11D5_AF7E_0050DA26BBDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Get_3D_DialogDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Get_3D_DialogDlg dialog

class Get_3D_DialogDlg : public CDialog
{
// Construction
public:
	Get_3D_DialogDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Get_3D_DialogDlg)
	enum { IDD = IDD_GET_3D_DIALOG };
	CButton	m_limits_off;
	CProgressCtrl	m_bar;
	CEdit	m_status;
	int		m_zmin;
	int		m_zmax;
	int		m_thresh;
	double	m_tolerance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Get_3D_DialogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Get_3D_DialogDlg)
	afx_msg void OnDetectPoints();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowImages();
	afx_msg void OnGetEpi3d();
	afx_msg void OnFileStereoPos();
	afx_msg void OnFileStereoAnalyse();
	afx_msg void OnFileStereoExpPos();
	virtual void OnOK();
	afx_msg void OnFileStereoImg1();
	afx_msg void OnFileStereoImg2();
	afx_msg void OnFileCam1Par();
	afx_msg void OnFileCam2Par();
	afx_msg void OnCcdPar();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFileCam1refimg();
	afx_msg void OnFileCam2refimg();
	afx_msg void OnImgSub();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GET_3D_DIALOGDLG_H__9FF24466_2CC2_11D5_AF7E_0050DA26BBDE__INCLUDED_)
