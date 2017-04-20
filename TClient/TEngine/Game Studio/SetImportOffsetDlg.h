#pragma once
#include "afxwin.h"


// CSetImportOffsetDlg 대화 상자입니다.

class CSetImportOffsetDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetImportOffsetDlg)

public:
	CSetImportOffsetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetImportOffsetDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_IMPORT_OFFSET };

public:
	int m_nFromLeft;
	int m_nFromTop;
	int m_nFromWidth;
	int m_nFromHeight;

	int m_nToLeft;
	int m_nToTop;
	float m_fToHeight;

	CString m_strFromLeft;
	CString m_strFromTop;
	CString m_strFromWidth;
	CString m_strFromHeight;

	CString m_strToLeft;
	CString m_strToTop;
	CString m_strToHeight;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
