#pragma once
#include "afxwin.h"

// CSelectFormatDlg 대화 상자입니다.

class CSelectFormatDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectFormatDlg)

public:
	CSelectFormatDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSelectFormatDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SELECT_FORMAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cFormat;
	int m_nSelectedFormat;
	int m_nType;
};
