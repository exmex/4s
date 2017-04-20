#pragma once
#include "afxwin.h"


// CAddAnimationDlg 대화 상자입니다.

class CAddAnimationDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddAnimationDlg)

public:
	CAddAnimationDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddAnimationDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ADDANIMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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

	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnOK();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboLoadtype();
	afx_msg void OnBnClickedButtonFolder();
	afx_msg void OnBnClickedButtonAnimationfile();
	int m_nLoadType;
};
