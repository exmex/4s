#pragma once
#include "afxwin.h"


// CAnnounceDlg 대화 상자입니다.

class CAnnounceDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnnounceDlg)

public:
	CAnnounceDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnnounceDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ANNOUNCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CButton m_chkTimer;
	DWORD m_dwTimerMin;
	CComboBox m_comboWorld;

	CString m_strAnnounce;
	DWORD	m_nID;
	BOOL	m_bTimer;

	void	SetWorld();

	afx_msg void OnBnClickedBtnTimeSelect();
	afx_msg void OnBnClickedBtnTimerEnd();	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
};
