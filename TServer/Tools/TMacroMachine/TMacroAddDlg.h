#pragma once
#include "afxwin.h"


// CTMacroAddDlg 대화 상자입니다.

class CTMacroAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CTMacroAddDlg)

public:
	CTMacroAddDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTMacroAddDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TMACROADDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbLoop;
	CString m_strName;
	BYTE m_bLoop;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
