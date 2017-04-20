#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTMacroLoadDlg 대화 상자입니다.

class CTMacroLoadDlg : public CDialog
{
	DECLARE_DYNAMIC(CTMacroLoadDlg)

public:
	CTMacroLoadDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTMacroLoadDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TMACROLOADDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:	
	virtual BOOL OnInitDialog();

	///////////////////////////////////////////////
	// Macro ID Vector
	VECTORDWORD vMID;
	///////////////////////////////////////////////

	///////////////////////////////////////////////
    // Tree / ListBox
	CTreeCtrl m_tree;
	CImageList *m_pImageList;
	CListBox m_listbox;
	///////////////////////////////////////////////

	///////////////////////////////////////////////
	// Macro Insert / Delete
	void InsertList();
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);

	void DeleteList();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnLbnDblclkList1();
	///////////////////////////////////////////////

	///////////////////////////////////////////////
	// IDOK
	afx_msg void OnBnClickedOk();
	///////////////////////////////////////////////	
};
