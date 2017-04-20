#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTProgressDlg 대화 상자입니다.

class CTProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CTProgressDlg)

public:
	CTProgressDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTProgressDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROGRESS_DIALOG };

protected:
	static DWORD __WaitForCANCEL( DWORD_PTR lParam);
	void WaitForCANCEL();

public:
	LPTHREAD_START_ROUTINE m_pTHREAD;
	HANDLE m_hTHREAD;
	LPVOID m_lParam;

public:
	CString m_strCURRENT;
	CString m_strTOTAL;
	CString m_strACTION;

	CProgressCtrl m_cCURRENT;
	CProgressCtrl m_cTOTAL;
	CButton m_cCANCEL;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuUpdate();
	afx_msg void OnMenuClose();
	virtual void OnCancel();
};
