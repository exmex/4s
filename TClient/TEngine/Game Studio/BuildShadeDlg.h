#pragma once
#include "afxcmn.h"


// CBuildShadeDlg 대화 상자입니다.

class CBuildShadeDlg : public CDialog
{
	DECLARE_DYNAMIC(CBuildShadeDlg)

public:
	CBuildShadeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBuildShadeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BUILDSHADE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CSpinButtonCtrl m_spinLX;
	CSpinButtonCtrl m_spinLY;
	CSpinButtonCtrl m_spinLZ;

	CString m_strLightX;
	CString m_strLightY;
	CString m_strLightZ;

	int m_nType;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
