#pragma once
#include "afxwin.h"


// CDlgServiceLog 대화 상자입니다.

class CDlgServiceLog : public CDialog
{
	DECLARE_DYNAMIC(CDlgServiceLog)

public:
	CDlgServiceLog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgServiceLog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLGSERVICELOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();

public:
	CComboBox m_comboTime;

	BYTE m_bStat;
	BYTE m_bMin;
};
