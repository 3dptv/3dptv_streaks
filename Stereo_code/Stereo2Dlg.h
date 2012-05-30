// Stereo2Dlg.h : header file
//

#if !defined(AFX_STEREO2DLG_H__70CFDE67_FE63_11D3_AF21_0050DA26B74C__INCLUDED_)
#define AFX_STEREO2DLG_H__70CFDE67_FE63_11D3_AF21_0050DA26B74C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStereo2Dlg dialog

class CStereo2Dlg : public CDialog
{
// Construction
public:
	CStereo2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CStereo2Dlg)
	enum { IDD = IDD_STEREO2_DIALOG };
	CButton	m_analyze_img;
	CSpinButtonCtrl	m_an_img_nr;
	CProgressCtrl	m_control_bar;
	CEdit	m_status;
	int		m_first_img;
	int		m_last_img;
	int		m_thres_ptv;
	CString	m_file_img;
	CString	m_file_ref_img;
	CString	m_file_calib;
	CString	m_file_calib2;
	CString	m_file_img2;
	CString	m_file_ref_img2;
	int		m_thres_ptv2;
	int		m_img_nr;
	int		m_thresh_analyze;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStereo2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
	void OnChgCoord2();
	// Generated message map functions
	//{{AFX_MSG(CStereo2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRun();
	virtual void OnOK();
	afx_msg void OnVmrl();
	afx_msg void OnOpenImgFile();
	afx_msg void OnOpenFileRefImg();
	afx_msg void OnOpenCamPar();
	afx_msg void OnAnalyze();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAnlyseImg();
	afx_msg void OnBmedian();
	afx_msg void OnChgCoord();
	afx_msg void OnButtonTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STEREO2DLG_H__70CFDE67_FE63_11D3_AF21_0050DA26B74C__INCLUDED_)
