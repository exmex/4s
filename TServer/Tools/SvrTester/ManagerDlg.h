#pragma once


// CManagerDlg 대화 상자입니다.

class CManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CManagerDlg)

public:
	CManagerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManagerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANAGERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSerchProtocol();
	afx_msg void OnBnClickedOk();

public:
	CString m_strIP;
	CString m_strPort;
	CString m_strFile;
	CString m_strUser;
	CString m_strPass;
	virtual BOOL OnInitDialog();
};
