#if (!defined __TTABCTRLBAR_H)
#define __TTABCTRLBAR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


typedef struct tagTCB_ITEM
{
	CString m_strToolTipLabel;
	CString m_strTabLabel;

    UINT m_nToolTipId;
	CWnd *m_pWnd;
} TCB_ITEM;

typedef CList< TCB_ITEM *, TCB_ITEM *> CTabViews;


class __TUI_API CTTabControlBar : public CTControlBar
{
	DECLARE_DYNAMIC(CTTabControlBar)

public:
	CTTabControlBar();
	virtual ~CTTabControlBar();

protected:
	int m_nNextToolTipId;
	int m_nActiveTab;
	int m_nPointTab;

	CTabViews m_viewList;
	CTTabCtrl m_TabCtrl;
	CView* m_pActiveView;

	CMapStringToPtr m_ClassNameToImageIndexMap;
	BOOL m_bToolTipsEnabled;

public:
	virtual BOOL AddView(
		LPCTSTR lpszLabel,
		CRuntimeClass *pViewClass,
		CDocument *pDoc = NULL,
		UINT nIndex = -1,
		CCreateContext *pContext = NULL);

	virtual void SetActiveView( CRuntimeClass *pViewClass);
	virtual void SetActiveView( int nView);
	virtual void RemoveView( int nView);

	virtual CView* GetView( CRuntimeClass *pViewClass);
	virtual CView* GetView( int nView);
	virtual CView* GetActiveView();

	virtual CImageList* SetTabImageList( CImageList *pImageList);
	virtual BOOL ModifyTabStyle(
		DWORD dwRemove,
		DWORD dwAdd,
		UINT nFlags = 0);

	virtual LPCTSTR GetViewName( CRuntimeClass *pViewClass);
	virtual LPCTSTR GetViewName( int nView);

    virtual void DeleteView( CRuntimeClass *pViewClass);
	virtual void DeleteView( int nView);

	virtual void SetViewToolTip( CRuntimeClass *pViewClass, LPCTSTR lpszLabel);
    virtual void SetViewToolTip( int nView, LPCTSTR lpszLabel);
	virtual void SetTabMenuID(
		UINT nMenuID,
		int nPos = 0);

	virtual void SetActiveView( CView* pUserView);
	virtual void AssociateImageIndexWithRuntimeClass(
		int nIndex,
		CRuntimeClass* pRuntimeClass);

	void AssociateImageIndexWithRuntimeClass(
		int nIndex,
		LPCTSTR lpszRuntimeClassName);

	virtual BOOL RemoveAllTabs();
	virtual int GetTabFromPoint( CPoint point = CPoint(0,0));

protected:
	virtual short GetImageIndexFromRuntimeClass( CRuntimeClass* pRuntimeClass);
	virtual void RemoveListItem( POSITION pos);
	virtual void SizeTabView(CWnd* pWnd);
	virtual CToolTipCtrl* GetToolTip();

	virtual void AddToolTip( TCB_ITEM* pMember, LPCTSTR lpszLabel);
	virtual void ResetToolTips( CToolTipCtrl* pToolTip);
	virtual TCB_ITEM* CreateTabListItem( LPCTSTR lpszLabel);

	virtual BOOL CreateTabView(
		CRuntimeClass *pViewClass,
		CDocument *pDocument,
		CCreateContext *pContext);

	virtual BOOL EnableToolTips( BOOL bEnable);

// Overrides
public:
    // ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTTabCtrlBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CTTabCtrlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


inline void CTTabControlBar::SetTabMenuID( UINT nMenuID, int nPos)
{
	m_TabCtrl.SetMenuID(
		nMenuID,
		nPos);
};

inline CView* CTTabControlBar::GetActiveView()
{
	return m_pActiveView;
}

inline CImageList* CTTabControlBar::SetTabImageList( CImageList *pImageList)
{
	return m_TabCtrl.SetImageList(pImageList);
}

inline BOOL CTTabControlBar::ModifyTabStyle( DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	return m_TabCtrl.ModifyStyle(
		dwRemove,
		dwAdd,
		nFlags);
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if (!defined __TTABCTRLBAR_H)
