#pragma once
#include "afxcmn.h"

// CTCharDlg 대화 상자입니다.

class CTCharDlg : public CDialog
{
	DECLARE_DYNAMIC(CTCharDlg)

public:
	CTCharDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTCharDlg();

public:
	LPTCHARACTER m_pTCHAR;
	VTCHARACTER m_vTCHAR;
	CListCtrl m_cTCHAR;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CHAR };

public:
	void ClearTCHAR();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
