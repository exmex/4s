#pragma once
#include "afxcmn.h"


// CBuildShadowDlg 대화 상자입니다.

class CBuildShadowDlg : public CDialog
{
	DECLARE_DYNAMIC(CBuildShadowDlg)

public:
	CBuildShadowDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBuildShadowDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BUILDSHADOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	BYTE m_bShadowPixel;
	virtual BOOL OnInitDialog();
	CSpinButtonCtrl m_spinPPT;
};
