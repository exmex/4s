#pragma once


// CSnapDlg 대화 상자입니다.

class CSnapDlg : public CDialog
{
	DECLARE_DYNAMIC(CSnapDlg)

public:
	CSnapDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSnapDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETSNAP };

public:
	int m_nSNAP;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
