#if (!defined __TTABCTRL_H)
#define __TTABCTRL_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class __TUI_API CTTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CTTabCtrl)

public:
	CTTabCtrl();
	virtual ~CTTabCtrl();

public:
	UINT m_nMenuID;

	CMenu m_Menu;
	CFont m_Font;
	int m_nPos;

public:
	virtual void UpdateFont();
	virtual void SetMenuID(
		UINT nMenuID,
		int nPos = 0);

	virtual UINT GetMenuID();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTTabCtrl)
	public:
	virtual BOOL Create(
		CWnd* pParentWnd,
		UINT nID = AFX_IDC_TAB_CONTROL,
		DWORD dwStyle = WS_VISIBLE|WS_CHILD|TCS_BOTTOM|TCS_TOOLTIPS|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		const CRect& rect=CRect( 0, 0, 0, 0));
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CTTabCtrl)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


inline void CTTabCtrl::SetMenuID( UINT nMenuID, int nPos)
{
	m_nMenuID = nMenuID;
	m_nPos = nPos;
}

inline UINT CTTabCtrl::GetMenuID()
{
	return m_nMenuID;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if (!defined __TTABCTRL_H)
