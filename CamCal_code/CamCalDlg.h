// CamCalDlg.h : header file
//

#if !defined(AFX_CAMCALDLG_H__72B04AE9_8E0F_11D4_AF3F_005004D8666E__INCLUDED_)
#define AFX_CAMCALDLG_H__72B04AE9_8E0F_11D4_AF3F_005004D8666E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCamCalDlg dialog

class CCamCalDlg : public CDialog
{
// Construction
public:
	CCamCalDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCamCalDlg)
	enum { IDD = IDD_CAMCAL_DIALOG };
	CProgressCtrl	m_bar;
	CButton	m_lin_cal;
	CButton	m_ctr_focus;
	CButton	m_ctr_z0;
	CButton	m_ctr_yh;
	CButton	m_ctr_y0;
	CButton	m_ctr_xh;
	CButton	m_ctr_x0;
	CButton	m_ctr_theta;
	CButton	m_ctr_sx;
	CButton	m_ctr_phi;
	CButton	m_ctr_p2;
	CButton	m_ctr_p1;
	CButton	m_ctr_omega;
	CButton	m_ctr_kappa;
	CButton	m_ctr_k2;
	CButton	m_ctr_k1;
	CEdit	m_rms;
	CEdit	m_ref_point;
	CEdit	m_cal_thres;
	CEdit	m_status;
	CEdit	m_k2;
	CEdit	m_theta;
	CEdit	m_y_h;
	CEdit	m_x_h;
	CEdit	m_s_x;
	CEdit	m_z_0;
	CEdit	m_y_0;
	CEdit	m_x_0;
	CEdit	m_focus;
	CEdit	m_omega;
	CEdit	m_kappa;
	CEdit	m_k1;
	CEdit	m_phi;
	CEdit	m_p2;
	CEdit	m_p1;
	CString	m_file_cal_img;
	CString	m_file_global;
	CString	m_dir_name;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamCalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCamCalDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCalCam();
	afx_msg void OnPlane();
	afx_msg void OnDetect();
	afx_msg void OnNewSet();
	afx_msg void OnResetPara();
	afx_msg void OnOpenCalImage();
	afx_msg void OnOpenGlobalFile();
	afx_msg void OnLinCal();
	afx_msg void OnPath();
	afx_msg void OnGet3d();
	afx_msg void OnGetLookUp();
	afx_msg void OnFileSavenonlincampar();
	afx_msg void OnFileSavelinearcampar();
	afx_msg void OnFileSavenonlin();
	afx_msg void OnFileSavelin();
	afx_msg void OnHelpAbout();
	afx_msg void OnCCDparSet();
	afx_msg void OnImageProc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMCALDLG_H__72B04AE9_8E0F_11D4_AF3F_005004D8666E__INCLUDED_)
