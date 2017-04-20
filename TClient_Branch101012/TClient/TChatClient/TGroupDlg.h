#pragma once
#include "afxcmn.h"

// CTGroupDlg 대화 상자입니다.

class CTGroupDlg : public CDialog
{
	DECLARE_DYNAMIC(CTGroupDlg)

public:
	CTGroupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTGroupDlg();

public:
	CListCtrl m_cTGROUP;
	VTGROUP m_vTGROUP;
	LPTGROUP m_pTGROUP;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_GROUP };

public:
	void ClearTGROUP();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
