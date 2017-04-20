#pragma once
#include "afxwin.h"


// CAddMediaDlg 대화 상자입니다.

class CAddMediaDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddMediaDlg)

public:
	CAddMediaDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddMediaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ADDMEDIA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cLoadType;

	CButton m_cFolder;
	CButton m_cFile;

	CStatic m_cFolderTitle;
	CStatic m_cFileTitle;

	CEdit m_cFolderEdit;
	CEdit m_cFileEdit;

	CString m_strFolderName;
	CString m_strFileName;
	int m_nLoadType;

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboLoadtype();
	afx_msg void OnBnClickedButtonFolder();
	afx_msg void OnBnClickedButtonMediafile();
protected:
	virtual void OnOK();
};
