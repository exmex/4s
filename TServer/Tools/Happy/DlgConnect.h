#pragma once
#include "afxwin.h"
#include "afxcmn.h"

typedef struct tagACCOUNT
{
	CString m_str[5];
	tagACCOUNT()
	{
		for(BYTE i = 0; i < 4; i++)
			m_str[i] = _T("");
	}
} ACCOUNT;

// CDlgConnect 대화 상자입니다.

class CDlgConnect : public CDialog
{
	DECLARE_DYNAMIC(CDlgConnect)

public:
	CDlgConnect(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgConnect();

public:

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SERVERCONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwIP;
	DWORD m_dwPort;
	CString m_strUserID;
	CString m_strPassword;
	map<CString,ACCOUNT> m_mapACCOUNT;	
	CComboBox m_cbAccount;


public:
	void Init();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeCombo1();
	virtual BOOL OnInitDialog();
};
