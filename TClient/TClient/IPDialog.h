#pragma once
#include "afxcmn.h"


// CIPDialog 대화 상자입니다.

class CIPDialog : public CDialog
{
	DECLARE_DYNAMIC(CIPDialog)

public:
	CIPDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CIPDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_ctrlIP;
	CString m_strIP;
	CString m_strPort;
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
