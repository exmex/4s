// Game StudioView.h : interface of the CGameStudioView class
//

#pragma once
#include "Game StudioDoc.h"


#define GSCT_ITEM			((BYTE) 0x00)
#define GSCT_GSLIST			((BYTE) 0x01)
#define GSCT_TEX			((BYTE) 0x02)


typedef struct tagGSCDATA
{
	BYTE m_bType;
	LPVOID m_pData;
} GSCDATA, *LPGSCDATA;


class CGameStudioView : public CTreeView
{
protected: // create from serialization only
	CGameStudioView();
	DECLARE_DYNCREATE(CGameStudioView)

protected:
	BOOL CanDrop(
		CItemBase *pItemSRC,
		CItemBase *pItemDEST);

	void MoveTree(
		CItemBase *pItemSRC,
		CItemBase *pItemDEST);

	COleDropTarget m_DropTarget;
	CImageList m_listTreeImages;
	HTREEITEM m_hCutItem;
	
	BOOL m_bCut;
	BOOL m_bCopy;

// Attributes
public:
	CGameStudioDoc* GetDocument() const;

// Operations
public:
	static CImageList* m_pDragImage;
	static CLIPFORMAT m_cfGS;

	static void InitDragResource();

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CGameStudioView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void TrackPopupMenu(
		CPoint& point,
		CMenu *pMenu);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual void OnDragLeave();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEditPaste();
	afx_msg void OnEditCopy();
	afx_msg void OnEditUndo();
	afx_msg void OnEditCut();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // debug version in Game StudioView.cpp
inline CGameStudioDoc* CGameStudioView::GetDocument() const
   { return reinterpret_cast<CGameStudioDoc*>(m_pDocument); }

#endif
