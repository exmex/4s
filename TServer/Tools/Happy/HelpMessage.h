#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CHelpMessage 대화 상자입니다.

class CHelpMessage : public CDialog
{
	DECLARE_DYNAMIC(CHelpMessage)

public:
	CHelpMessage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHelpMessage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_HELPINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_cbWorld;
	CComboBox m_cbType;
	CDateTimeCtrl m_dtStart;
	CDateTimeCtrl m_dtEnd;

public:
	void	Init();
	void	SetShow(BYTE bShow);
	BYTE	GetWorld(BYTE& p_bOut);	

	afx_msg void OnBnClickedBtnHsend();
	afx_msg void OnCbnSelchangeCbHtype();
};
