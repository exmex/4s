// TQuestEditorView.h : interface of the CTQuestEditorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TQUESTEDITORVIEW_H__041E9E1E_17C3_4618_9DA4_24E5AFEA317F__INCLUDED_)
#define AFX_TQUESTEDITORVIEW_H__041E9E1E_17C3_4618_9DA4_24E5AFEA317F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTQuestEditorView : public CTreeView
{
protected: // create from serialization only
	CTQuestEditorView();
	DECLARE_DYNCREATE(CTQuestEditorView)

// Attributes
public:
	CTQuestEditorDoc* GetDocument();
	CImageList m_listTreeImage;
	LPTQUEST m_pCopySourceQUEST;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTQuestEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTQuestEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTQuestEditorView)
	afx_msg void OnDestroy();
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChildTreeState();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TQuestEditorView.cpp
inline CTQuestEditorDoc* CTQuestEditorView::GetDocument()
   { return (CTQuestEditorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TQUESTEDITORVIEW_H__041E9E1E_17C3_4618_9DA4_24E5AFEA317F__INCLUDED_)
