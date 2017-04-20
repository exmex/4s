#pragma once


// CUserKickOutDlg 대화 상자입니다.

class CUserKickOutDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserKickOutDlg)

public:
	CUserKickOutDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserKickOutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_USERKICKOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUser;
public:

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
