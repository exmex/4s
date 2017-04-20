#pragma once
#include "afxcmn.h"


// CIPChangeDlg 대화 상자입니다.

class CIPChangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CIPChangeDlg)

public:
	CIPChangeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CIPChangeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_ctrIPAddr;
	afx_msg void OnBnClickedOk();
};
