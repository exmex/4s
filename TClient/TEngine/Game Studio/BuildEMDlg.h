#pragma once


// CBuildEMDlg 대화 상자입니다.

class CBuildEMDlg : public CDialog
{
	DECLARE_DYNAMIC(CBuildEMDlg)

public:
	CBuildEMDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBuildEMDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BUILDEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CString m_strTitle;
	int m_nType;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
};
