#pragma once
#include "afxcmn.h"


class CProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);
	virtual ~CProgressDlg();

	enum { IDD = IDD_DIALOG_PROGRESS};

protected:
	virtual void DoDataExchange( CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	AFX_THREADPROC m_pThreadFunc;
	CWinThread* m_pThread;
	LPVOID m_lParam;

	CProgressCtrl m_cProgressBar;
	CString m_strStatus;
	CString m_strCancel;
	CString m_strTitle;
	BYTE m_bPercent;

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnProgressUpdate();
	afx_msg void OnProgressExit();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
};
