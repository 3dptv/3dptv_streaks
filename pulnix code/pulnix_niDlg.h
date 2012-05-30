// pulnix_niDlg.h : header file
//

#if !defined(AFX_PULNIX_NIDLG_H__720169B7_E9C5_11D3_AF0C_0050DA26B74C__INCLUDED_)
#define AFX_PULNIX_NIDLG_H__720169B7_E9C5_11D3_AF0C_0050DA26B74C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPulnix_niDlg dialog

class CPulnix_niDlg : public CDialog
{
// Construction
public:
	CPulnix_niDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPulnix_niDlg)
	enum { IDD = IDD_PULNIX_NI_DIALOG };
	CEdit	m_nr_of_images;
	CComboBox	m_trigger2_signal_out;
	CComboBox	m_trigger2_line_out;
	CComboBox	m_trigger1_line_out;
	CComboBox	m_trigger1_signal_out;
	CSpinButtonCtrl	m_img_nr;
	CButton	m_inv;
	CButton	m_trigger_off;
	CButton	m_trigger_on;
	CEdit	m_thresh_value;
	CButton	m_thresh;
	CButton	m_integ;
	CButton	m_sub_ref;
	CButton	m_frame_rate_apply;
	CButton	m_mode;
	CEdit	m_status_nr_images;
	CEdit	m_status_3;
	CEdit	m_status_2;
	CComboBox	m_rec_mode;
	CButton	m_save_one_shot_apply;
	CButton	m_sequence;
	CButton	m_one_shot;
	CButton	m_stop;
	CButton	m_ring;
	CEdit	m_frame_rate;
	CEdit	m_frame_width;
	CEdit	m_frame_height;
	CEdit	m_status;
	int		m_test;
	CString	m_file_save;
	int		m_set_frame_rate;
	double	m_exp_time;
	int		m_nr_of_buffers;
	BOOL	m_two_cameras;
	BOOL	m_master;
	BOOL	m_external_trigger;
	BOOL	m_async;
	//}}AFX_DATA


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPulnix_niDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPulnix_niDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRing();
	afx_msg void OnStop();
	afx_msg void OnOneShot();
	afx_msg void OnSequence();
	afx_msg void OnSaveOneShot();
	afx_msg void OnNrOfBuffersApply();
	virtual void OnOK();
	afx_msg void OnNrCameras();
	afx_msg void OnSubRef();
	afx_msg void OnInteg();
	afx_msg void OnThresh();
	afx_msg void OnTriggerOff();
	afx_msg void OnClearInt();
	afx_msg void OnSaveImgProc();
	afx_msg void OnChangeStatusNrImages();
	afx_msg void OnLaserTrigOn();
	afx_msg void OnOnFrameRateApply();
	afx_msg void OnAsync();
	afx_msg void OnFrameRateApply();
	afx_msg void OnMaxExp();
	afx_msg void OnExternalTrigger();
	afx_msg void OnGrabberSignal1();
	afx_msg void OnGrabberSignal2();
	afx_msg void OnSignal1Off();
	afx_msg void OnSignal2Off();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#include "niimaq.h"

extern SESSION_ID Sid;
extern uInt32	  BufNum;
extern Int32	  MissedNbBuf;

#endif // !defined(AFX_PULNIX_NIDLG_H__720169B7_E9C5_11D3_AF0C_0050DA26B74C__INCLUDED_)
