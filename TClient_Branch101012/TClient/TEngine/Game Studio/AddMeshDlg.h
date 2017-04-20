#pragma once
#include "afxwin.h"


// CAddMeshDlg 대화 상자입니다.

class CAddMeshDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddMeshDlg)

public:
	CAddMeshDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddMeshDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ADDMESH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CComboBox m_cLoadType;

	CStatic m_cFolderTitle;
	CStatic m_cFileTitle;

	CEdit m_cFolderEdit;
	CEdit m_cFileEdit;

	CButton m_cFolder;
	CButton m_cFile;

	CString m_strFolderName;
	CString m_strFileName;

	int m_nLoadType;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboLoadtype();
	afx_msg void OnBnClickedButtonFolder();
	afx_msg void OnBnClickedButtonMeshfile();

protected:
	virtual void OnOK();
};
