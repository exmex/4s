#pragma once


// CCharMsgDlg 대화 상자입니다.

class CCharMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CCharMsgDlg)

public:
	CCharMsgDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCharMsgDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CHARMSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
