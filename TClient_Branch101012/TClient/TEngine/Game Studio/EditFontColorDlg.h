#pragma once

#include "BarContent.h"
#include "afxwin.h"
#include "afxcmn.h"

// CEditFontColorDlg 대화 상자입니다.

class CEditFontColorDlg : public CBarContent
{
	DECLARE_DYNAMIC(CEditFontColorDlg)

public:
	CEditFontColorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditFontColorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FONT_EDIT_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	CStatic m_cControlBox;
	CColorBox m_cColorBox;
	CTColorCtrl m_cColorCtrl;
	
	CSpinButtonCtrl m_spinRValue;
	CSpinButtonCtrl m_spinGValue;
	CSpinButtonCtrl m_spinBValue;

	DWORD* m_dwFontColor;
	DWORD m_dwRValue;
	DWORD m_dwGValue;
	DWORD m_dwBValue;
	DWORD m_dwFONTCOLORHEX;

	CString m_strFontColorHex;

	BOOL m_bInit;

public:
	void InitColor(DWORD *dwColor);

public:
    afx_msg void OnEnChangeEditColor();
	afx_msg LRESULT OnColorSelected( WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnBnClickedOk();
};