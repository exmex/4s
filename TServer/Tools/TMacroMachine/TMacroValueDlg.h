#pragma once
#include "afxwin.h"
#include "md5.h"

// CTMacroValueDlg 대화 상자입니다.

class CTMacroValueDlg : public CDialog
{
	DECLARE_DYNAMIC(CTMacroValueDlg)

public:
	CTMacroValueDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTMacroValueDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TMACROVALUEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbValueType;
	CComboBox m_cbType;

	BYTE m_bValueType;
	BYTE m_bType;
	CString m_strName;
	CString m_strValue;

	void SetValue(BYTE bValueType, BYTE bType, CString strName, CString strValue);

public:
	CString EncryptPW(CString& strSOURCE);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCombo2();

};
