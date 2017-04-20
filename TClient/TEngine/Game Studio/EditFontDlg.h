#pragma once
#include "GSListCtrl.h"
#include "EditFontColorDlg.h"
#include "afxcmn.h"


// CEditFontDlg 대화 상자입니다.

class CEditFontDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditFontDlg)

public:
	CEditFontDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditFontDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FONT };

protected:
	void DeleteFONT( int nIndex);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CGSListCtrl m_cFONT;
	CString m_strSEL;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemActivateListFont(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditListFont(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeleditListFont(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonEdit();
protected:
	virtual void OnOK();
public:
	afx_msg void OnHdnBegintrackListFont(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnTrackListFont(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonEditColor();
};
