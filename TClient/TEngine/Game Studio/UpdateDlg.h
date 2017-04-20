#pragma once
#include "afxwin.h"


// CUpdateDlg 대화 상자입니다.

class CUpdateDlg : public CDialog
{
	DECLARE_DYNAMIC(CUpdateDlg)

public:
	CUpdateDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUpdateDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_UPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFileName;
	CString m_strFilter;
	CString m_strTitle;
	CString m_strEXT;

	CButton m_cFile;

	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButtonFile();
};
