#pragma once
#include "GSListCtrl.h"
#include "afxcmn.h"


// CEditMenuDlg 대화 상자입니다.

class CEditMenuDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditMenuDlg)

public:
	CEditMenuDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditMenuDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MENU };

protected:
	void DeleteMENU( int nIndex);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CGSListCtrl m_cMENU;
	CString m_strSEL;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemActivateListMenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditListMenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeleditListMenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
protected:
	virtual void OnOK();
};
