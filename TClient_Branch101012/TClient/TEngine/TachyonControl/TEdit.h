#if (!defined __TEDIT_H)
#define __TEDIT_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CTEdit

class __TUI_API CTEdit : public CEdit
{
	void _PostRedraw();
	void _DrawEditImpl(
		CRect rectClient,
		CDC *pDC = NULL);

	BOOL m_bMouseOver;

public:
	CTEdit();
	void SetIndent( int margin);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTEdit();

// Generated message map functions
protected:
	//{{AFX_MSG(CTEdit)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if (!defined __TEDIT_H)