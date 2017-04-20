////////////////////////////////////////////////////////////////////////////////
//	File Name	:	LogToolDlg.h
//
//	Copyright	:	(c) Zemi Interactive, Inc.	2002-2009
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Defines.h"
#include "ODBCDatabase.h"
#include "DetailItemDlg.h"
#include "SearchDlg.h"


#include <vector>
#include <map>


using std::vector;
using std::map;
using std::pair;

#define GETINSTANCE() (CLogToolDlg::GetInstance())
/////////////////////////////////////////////////////////////////////////////
// CLogToolDlg dialog

class CLogToolDlg : public CDialog
{
// Construction
public:
	CLogToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLogToolDlg)
	enum { IDD = IDD_LOGTOOL_DIALOG };

	CStatic	m_ctrlServerType;
	CButton	m_ctrlSearch;
	CButton	m_ctrlNext;
	CButton	m_ctrlPrev;
	CStatic		m_ctrlMsg;
	CListCtrl	m_ctrlView;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogToolDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	typedef vector<RECORDSET>			RESULTSET, *LPRESULTSET;
	typedef RESULTSET::iterator			RESULTSETITER;

	typedef map<int, LPRESULTSET>		RESULTLIST;
	typedef RESULTLIST::iterator		RESULTLISTITER;
	typedef pair<int, LPRESULTSET>		RESULTLISTPAIR;

	typedef map<int , CString>			BASEITEMINFO;
	typedef BASEITEMINFO::iterator		BASEITEMITER;
	
	typedef map<int, CString>			RAREITEMINFO;
	typedef RAREITEMINFO::iterator		RAREITEMITER;

	typedef map<int, CString>			SKILLINFO;
	typedef SKILLINFO::iterator			SKILLITER;

	typedef map<int, CString>			LOCATIONINFO;
	typedef LOCATIONINFO::iterator		LOCATIONITER;

	typedef map<int, CString>			QUESTINFO;
	typedef QUESTINFO::iterator			QUESTITER;

	typedef map<int, CString>			SETINFO;
	typedef SETINFO::iterator			SETITER;

	typedef vector<CString>				VECTOR_STRING;
	typedef VECTOR_STRING::iterator		VECTOR_ITER;
	typedef VECTOR_STRING::value_type	VECTOR_VALUE;

	HICON m_hIcon;
	CToolBar m_ToolBar;

	BOOL	OnConnect();
	BOOL	OnSave();
	void	OnSearch();
	BOOL	OnView(int nTabIndex = 0);
	BOOL	OnViewAll();
	void	OnQuit();

	void	ClearResultMap();

	// Generated message map functions
	//{{AFX_MSG(CLogToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSearch();
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuQuit();
	afx_msg void OnMenuSave();
	afx_msg void OnMenuSearch();
	afx_msg void OnToolbarConnect();
	afx_msg void OnToolbarSave();
	afx_msg void OnToolbarSearch();
	afx_msg void OnMenuConnect();
	afx_msg void OnInfo();
	afx_msg void OnPopupSearch();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOk();
	afx_msg void OnButtonPrev();
	afx_msg void OnButtonNext();

	afx_msg void OnSelectAll();
	afx_msg void OnFileSave();
	afx_msg void OnAccelConnect();
	afx_msg void OnAccelQuit();
	afx_msg void OnAccelSearch();
	afx_msg void OnAccelSave();
	afx_msg void OnAccelSelectAll();
	afx_msg void OnAccelCopy();
	afx_msg void OnButtonAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	

	BOOL	Search(CTime From, CTime To, CString strConditions);
//	BOOL	GetItemName(RECORDSET& recordset);
//	BOOL    GetItemDetailInfo(RECORDSET& Recordset, VECTOR_STRING& outVar);

	static CLogToolDlg* GetInstance() {return s_pThis;}

	LPSERVERINFO		m_pServList;

public:
	BASEITEMINFO		m_BaseItem;
	RAREITEMINFO		m_RareItem;
	SKILLINFO			m_SkillInfo;
	LOCATIONINFO		m_LocationInfo;
	QUESTINFO			m_QuestScroll;
	SETINFO				m_SetItem;
	
private:
	CODBCDatabase	m_LogConnection;
	CODBCDatabase	m_GameConnection;

	CDetailItemDlg* m_pDlg;
	CSearchDlg*		m_pSearchDlg;

	RESULTSET		m_AllResults;
	RESULTLIST		m_ResultList;
	int				m_nTab;
	int				m_nCurrentTab;
	int				m_nCurrentConnect;
	CString			m_strAccID;
	CString			m_strAccPWD;
	
	CPoint			m_PopupPt;

	HACCEL			m_hAccelTable;

	static CLogToolDlg* s_pThis;
public:
	afx_msg void OnMenuSavelogcolumn();
};