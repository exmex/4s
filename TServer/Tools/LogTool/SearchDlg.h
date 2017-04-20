////////////////////////////////////////////////////////////////////////////////
//	File Name	:	SearchDlg.h
//
//	Copyright	:	(c) Zemi Interactive, Inc.	2002-2009
//
////////////////////////////////////////////////////////////////////////////////
#pragma once


////////////////////////////////////////////////////////////////////////////////
//	CSearchDlg dialog
////////////////////////////////////////////////////////////////////////////////
class CSearchDlg : public CDialog
{
// Construction
public:
	CSearchDlg(CWnd* pParent = NULL);   // standard constructor

protected:
	CTime	m_DateFrom;
	CTime	m_DateTo;
	TCHAR	m_strConditions[512];

public:


// Dialog Data
	//{{AFX_DATA(CSearchDlg)
	enum { IDD = IDD_DIALOG_SEARCH };

	CDateTimeCtrl	m_ctrlDateInput;
	CDateTimeCtrl	m_ctrlDateInput2;


	CComboBox	m_ctrlActionInput;
	CButton	m_ctrlAction;

	CButton m_ctrlIP;				//	IP
	CButton m_ctrlMapID;			//	Map ID

	CButton m_ctrlDK1;				//	DK 1
	CButton m_ctrlDK2;				//	DK 2
	CButton m_ctrlDK3;				//	DK 3
	CButton m_ctrlDK4;				//	DK 4
	CButton m_ctrlDK5;				//	DK 5
	CButton m_ctrlDK6;				//	DK 6
	CButton m_ctrlDK7;				//	DK 7
	CButton m_ctrlDK8;				//	DK 8
	CButton m_ctrlDK9;				//	DK 9

	CButton m_ctrlKey1;				// key 1
	CButton m_ctrlKey2;				// key 2
	CButton m_ctrlKey3;				// key 3
	CButton m_ctrlKey4;				// key 4
	CButton m_ctrlKey5;				// key 5
	CButton m_ctrlKey6;				// key 6
	CButton m_ctrlKey7;				// key 7

	CButton m_ctrlLog;				// Log

	CEdit	m_ctrlIPInput;			// IP
	CEdit	m_ctrlMapIDInput;		// Map ID

	CEdit	m_ctrlDKInput1;			// DK Input 1
	CEdit	m_ctrlDKInput2;			// DK Input 2
	CEdit	m_ctrlDKInput3;			// DK Input 3
	CEdit	m_ctrlDKInput4;			// DK Input 4
	CEdit	m_ctrlDKInput5;			// DK Input 5
	CEdit	m_ctrlDKInput6;			// DK Input 6
	CEdit	m_ctrlDKInput7;			// DK Input 7
	CEdit	m_ctrlDKInput8;			// DK Input 8
	CEdit	m_ctrlDKInput9;			// DK Input 9

	CEdit	m_ctrlKeyInput1;		//	Key Input 1
	CEdit	m_ctrlKeyInput2;		//	Key Input 2
	CEdit	m_ctrlKeyInput3;		//	Key Input 3
	CEdit	m_ctrlKeyInput4;		//	Key Input 4
	CEdit	m_ctrlKeyInput5;		//	Key Input 5
	CEdit	m_ctrlKeyInput6;		//	Key Input 6
	CEdit	m_ctrlKeyInput7;		//	Key Input 7

	CEdit	m_ctrlLogInput;			//	Log Input

	CButton		m_ctrlActionInsert;
	CButton		m_ctrlActionDelete;
	CListBox	m_ListAcionBox;

	CButton		m_ctrlCharInsert;
	CButton		m_ctrlCharDelete;
	CListBox	m_ListCharBox;

	CButton	m_ctrlDate;
	BOOL	m_Date;
	BOOL	m_CharacterFrom;
	BOOL	m_IDFrom;
	BOOL	m_Action;
	BOOL	m_CharacterTo;
	BOOL	m_IDTo;
	BOOL	m_Item;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CSearchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckDate();
//	afx_msg void OnCheckFrom();

	afx_msg void OnCheckIP();		//	IP
	afx_msg void OnCheckMapID();	//	Map ID

	afx_msg void OnCheckDK1();		//	dwKey1
	afx_msg void OnCheckDK2();		//	dwKey2
	afx_msg void OnCheckDK3();		//	dwKey3
	afx_msg void OnCheckDK4();		//	dwKey4
	afx_msg void OnCheckDK5();		//	dwKey5
	afx_msg void OnCheckDK6();		//	dwKey6
	afx_msg void OnCheckDK7();		//	dwKey7
	afx_msg void OnCheckDK8();		//	dwKey8
	afx_msg void OnCheckDK9();		//	dwKey9

	afx_msg void OnCheckKey1();		// key 1
	afx_msg void OnCheckKey2();		// key 2
	afx_msg void OnCheckKey3();		// key 3
	afx_msg void OnCheckKey4();		// key 4
	afx_msg void OnCheckKey5();		// key 5
	afx_msg void OnCheckKey6();		// key 6
	afx_msg void OnCheckKey7();		// key 7
	
	afx_msg void OnCheckLog();		// Log

	afx_msg void OnCheckAction();

	afx_msg void OnActionInsert();
	afx_msg void OnActionDelete();

	afx_msg void OnCharInsert();
	afx_msg void OnCharDelete();

	afx_msg void OnOk();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};