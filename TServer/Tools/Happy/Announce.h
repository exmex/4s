// 현승룡 Announce.h

#pragma once
#include "afxwin.h"

#define MAX_WORLD 10

// CAnnounce 대화 상자입니다.

class CAnnounce : public CDialog
{
	DECLARE_DYNAMIC(CAnnounce)

public:
	CAnnounce(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnnounce();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ANNOUNCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_comboWorld;
	CString		m_strAnnounce;
	DWORD		m_nID;

	CButton m_checkTimer;
	DWORD m_dwTimerMin;

	BOOL m_bTimer;

	void SetWorld();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedButtonTimerend();
	afx_msg void OnBnClickedButtonTimer();
};
