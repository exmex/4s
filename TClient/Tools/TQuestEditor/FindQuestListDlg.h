#pragma once
#include "afxcmn.h"


// CFindQuestListDlg 대화 상자입니다.

typedef vector<HTREEITEM> VECHITEM;
typedef map< int, HTREEITEM> MAPITEM;

class CFindQuestListDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindQuestListDlg)

public:
	CFindQuestListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFindQuestListDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LIST_FIND_QUEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CListCtrl m_cFindQuestList;
	void InitListData(CTreeCtrl *pTree, VECHITEM* vHItem);
private:
	MAPITEM		m_mapITEMLIST;
	CTreeCtrl*	m_pTree;
	VECHITEM*	m_pHItem;
	HTREEITEM	m_pSelectedItem;
public:
	afx_msg void OnSelectedItem();
	afx_msg void OnSelectedItem(NMHDR *pNMHDR, LRESULT *pResult);
	void GetSelectedItem(HTREEITEM* pSelectedItem);
};
