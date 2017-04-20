#pragma once


// CAddFarImageDlg 대화 상자입니다.

class CAddFarImageDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddFarImageDlg)

public:
	CAddFarImageDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddFarImageDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ADD_FARIMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
