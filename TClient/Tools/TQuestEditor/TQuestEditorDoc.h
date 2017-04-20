// TQuestEditorDoc.h : interface of the CTQuestEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TQUESTEDITORDOC_H__6D0B9C0C_6701_44E4_9060_6A1142D72B59__INCLUDED_)
#define AFX_TQUESTEDITORDOC_H__6D0B9C0C_6701_44E4_9060_6A1142D72B59__INCLUDED_

#include "ClassTitleDlg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTQuestView;

typedef vector<HTREEITEM> VECHITEM;

class CTQuestEditorDoc : public CDocument
{
protected: // create from serialization only
	CTQuestEditorDoc();
	DECLARE_DYNCREATE(CTQuestEditorDoc)

// Attributes
public:
	MAPTQCLASS m_mapTQCLASS;
	CDatabase m_db;

public:
	void UpdateData( LPTQUEST pQUEST);
	void DeleteData(
		CTreeCtrl *pTree,
		HTREEITEM itemPos);

	void ResetTQClassData();
	void ReleaseTQClass();
	void LoadTQClass();

	void LoadData(
		CTreeCtrl *pTree,
		HTREEITEM itemPos);

	void SaveMPQ(
		CFile *pFile,
		HTREEITEM itemPos,
		CTreeCtrl *pTree);

	void GetQuestTitle(VECHITEM* pVECHITEM, HTREEITEM hItem);
	void ParentExpand(HTREEITEM  hItem);
	void ChildExpand(HTREEITEM  hItem);
	void ChildCollapse(HTREEITEM hItem);

	void MoveSubTree(
		CTreeCtrl *pTree,
		HTREEITEM itemCUR,
		HTREEITEM itemPARENT);

	HTREEITEM FindNode(
		HTREEITEM itemPos,
		CTreeCtrl *pTree,
		DWORD dwQuestID);
    
	BOOL GetClassMain(DWORD dwClassID);
	void SetClassMain(BYTE bMain);

	LPTQUEST AddQuest( LPTQUEST pSourceQUEST );
	void AddTerm( LPTQUEST pQUEST, LPTQUESTTERM pSourceTERM );
	void AddCond( LPTQUEST pQUEST, LPTCONDITION pSourceCOND );
	void AddReward( LPTQUEST pQUEST, LPTQUESTREWARD pSourceREWARD );
    
// Operations
public:
	CTQuestView* m_pTQuestView;
	CTreeCtrl* GetTreeCtrl();

	TQCLASS m_vNULLClass;
	CClassTitleDlg* m_pClassTitleDlg;

	void _Recursive_CollectQuestByType(
		CTreeCtrl* pTree,
		HTREEITEM itemPos,
		BYTE bQuestType_Filter,
		std::vector<LPTQUEST>& vOutput );
	BYTE GetMonsterLevelByKind( DWORD dwKind );
	//LPCTSTR GetTriggerTypeString( BYTE bTriggerType );

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTQuestEditorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTQuestEditorDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
public:
	void LoadDataProc();
	//{{AFX_MSG(CTQuestEditorDoc)
	afx_msg void OnDatabaseOpen();
	afx_msg void OnUpdateDatabaseOpen(CCmdUI* pCmdUI);
	afx_msg void OnDatabaseClose();
	afx_msg void OnUpdateDatabaseClose(CCmdUI* pCmdUI);
	afx_msg void OnQuestAdd();
	afx_msg void OnUpdateQuestAdd(CCmdUI* pCmdUI);
	afx_msg void OnQuestDelete();
	afx_msg void OnUpdateQuestDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnEditClass();
	afx_msg void OnUpdateEditClass(CCmdUI *pCmdUI);
	afx_msg void OnQuestFind();
	afx_msg void OnFind();
	afx_msg void OnQuestChgparent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnQuestPrintSummary();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TQUESTEDITORDOC_H__6D0B9C0C_6701_44E4_9060_6A1142D72B59__INCLUDED_)
