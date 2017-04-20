#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTLoginDlg 대화 상자입니다.

class CTLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CTLoginDlg)

public:
	CIPAddressCtrl m_cIPAddr;
	CString m_strIPAddr;

	CString m_strID;
	CString m_strPW;
	DWORD m_dwPort;
	BYTE m_bPWEncrypt;
	

public:
	CTLoginDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTLoginDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	
	CButton m_chkPWEncrypt;
};
