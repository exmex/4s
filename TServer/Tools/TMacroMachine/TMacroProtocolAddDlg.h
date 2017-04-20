#pragma once
#include "afxwin.h"


// CTMacroProtocolAddDlg 대화 상자입니다.

class CTMacroProtocolAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CTMacroProtocolAddDlg)

public:
	CTMacroProtocolAddDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTMacroProtocolAddDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TMACROPROTOCOLADDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_dwID;
	CString m_strName;
	BYTE m_bLoop;
	BYTE m_bSelf;
	WORD m_wSleep;
	BYTE m_bCheck;
	DWORD m_dwSelectIndex;

	MAPTPROTOCOL m_mapProtocol;

	void SetProtocol(MAPTPROTOCOL mapProtocol);
	void SetProtocolAttr();
	
public:
	CComboBox m_cbProtocol;
	CComboBox m_cbSelf;
	CComboBox m_cbLoop;
	CButton m_ckRoot;

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnFindProtocol();
};
