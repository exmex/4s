/////////////////////////////////////////////////////////////////////////////
//	File Name	:	DetailItemDlg.h
//
//	Copyright	:	(c) Zemi Interactive, inc 2002~2009
//
/////////////////////////////////////////////////////////////////////////////
#pragma once


/////////////////////////////////////////////////////////////////////////////
// CDetailItemDlg dialog

class CDetailItemDlg : public CDialog
{
// Construction
public:
	CDetailItemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetailItemDlg)
	enum { IDD = IDD_DIALOG_DETAIL };
	CListBox	m_ctrlDetail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetailItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetailItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL InsertList(int idx,LPCTSTR str);
	void ClearAllItems();
};
