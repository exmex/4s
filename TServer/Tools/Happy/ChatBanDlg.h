//---------------------------------------
// 08-01-16
// JINUK
// 채팅 금지 다이얼로그
//---------------------------------------

#pragma once
#include "afxwin.h"

// CChatBanDlg 대화 상자입니다.

class CChatBanDlg : public CDialog
{
	DECLARE_DYNAMIC(CChatBanDlg)

public:
	CChatBanDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CChatBanDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CHATBAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void Init(); 

public:
	CEdit m_editChatBanName;
	CEdit m_editChatBanMin;
	CEdit m_ebBanReason;

	CString m_strChatBanName;	
	int m_iChatBanMin;

	afx_msg void OnBnClickedOk();
	
};
