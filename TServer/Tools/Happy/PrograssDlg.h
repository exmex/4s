#pragma once
#include "afxcmn.h"


// CPrograssDlg 대화 상자입니다.

class CPrograssDlg : public CDialog
{
	DECLARE_DYNAMIC(CPrograssDlg)

public:
	CPrograssDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPrograssDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_INSTALL };
protected:
	DWORD m_dwTotal;
	DWORD m_dwCurrent;
	CProgressCtrl m_hProgressBar;

public:
	void SetTitle(CString strTitle)		{	SetWindowText(strTitle);}
	void SetText(CString strText)		{	SetDlgItemText(IDC_STATIC_NAME, strText);}
	void ShowComplete(BYTE bShow = SW_SHOW){GetDlgItem(IDC_BT_COMPLITE)->ShowWindow(bShow);}

	void SetProgressRange(DWORD dwMax);
	void SetProgressVal(DWORD dwVal);
	void AddProgressVal(DWORD dwAdd)	{	SetProgressVal(m_dwCurrent + dwAdd);}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtComplite(){ShowWindow(SW_HIDE);}
};
