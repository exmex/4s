#pragma once
#include "afxwin.h"
#include "RetDlg.h"

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
	CString m_strChatBanName;
	CEdit m_editChatBanName;
	CEdit m_ebBanReason;

	int m_iChatBanMin;

	CRetDlg*	m_pdlgRet;

public:
	void Init();
	void ShowDlgRet(BYTE bShow, BYTE bRet);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();	
	
	CEdit m_ebBanTime;
};
