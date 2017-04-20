/////////////////////////////////////////////////////////////////////////////
//	File Name	:	ConnectDlg.h : header file
//
//	Copytight	:	(c) Zemi Interactive, Inc 2002~2009
//
/////////////////////////////////////////////////////////////////////////////
#pragma once



#include "LogToolDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CConnectDlg dialog

class CConnectDlg : public CDialog
{
// Construction
public:
	CConnectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConnectDlg)
	enum { IDD = IDD_DIALOG_CONNECT };
	CEdit	m_ctrlPassword;
	CEdit	m_ctrlAccount;
	CComboBox	m_ctrlConnect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConnectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_nConnectIdx;
	CString m_strDBName;
	CString m_strAccID;
	CString m_strPWD;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

