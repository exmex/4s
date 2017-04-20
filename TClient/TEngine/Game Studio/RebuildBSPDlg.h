#pragma once
#include "afxwin.h"


// CRebuildBSPDlg 대화 상자입니다.

class CRebuildBSPDlg : public CDialog
{
	DECLARE_DYNAMIC(CRebuildBSPDlg)

public:
	CRebuildBSPDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRebuildBSPDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BSPREBUILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CString m_strFile;
	CButton m_cFile;

	BOOL m_bLIGHT;
	BOOL m_bPVS;
	BOOL m_bTEX;
	BOOL m_bOBJ;
	BOOL m_bSFX;
	BOOL m_bSND;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonMeshfile();
	virtual void OnOK();
};
