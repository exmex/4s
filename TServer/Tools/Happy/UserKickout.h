// 현승룡 UserKickout.h

#pragma once


// CUserKickout 대화 상자입니다.

class CUserKickout : public CDialog
{
	DECLARE_DYNAMIC(CUserKickout)

public:
	CUserKickout(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserKickout();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_USERKICKOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUser;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
