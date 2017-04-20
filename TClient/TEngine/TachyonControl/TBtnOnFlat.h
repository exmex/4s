#if (!defined __TBTNONFLAT_H)
#define __TBTNONFLAT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#define FE_MOUSEOVER		1
#define FE_MOUSEOUT			2


/////////////////////////////////////////////////////////////////////////////
// CTBtnOnFlat window

class __TUI_API CTBtnOnFlat : public CButton
{
	DECLARE_DYNCREATE(CTBtnOnFlat);

// Construction
public:
	CTBtnOnFlat();

// Attributes
public:

private:
	BOOL m_bCurrentlyIsFlat;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTBtnOnFlat)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTBtnOnFlat();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTBtnOnFlat)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__TBTNONFLAT_H)