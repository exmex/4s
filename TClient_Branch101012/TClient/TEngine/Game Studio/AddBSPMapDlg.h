#pragma once
#include "afxwin.h"


// CAddBSPMapDlg 대화 상자입니다.

class CAddBSPMapDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddBSPMapDlg)

public:
	CAddBSPMapDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddBSPMapDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BSPMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CString m_strFile;
	CButton m_cFile;
	BOOL m_bPVS;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonMeshfile();
};
