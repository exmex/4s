#if (!defined __TSTATUSCONTROLBAR_H)
#define __TSTATUSCONTROLBAR_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTStatusControlBar

class __TUI_API CTStatusControlBar : public CStatusBar
{
// Construction
public:
	DECLARE_DYNAMIC(CTStatusControlBar)
	CTStatusControlBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTStatusControlBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTStatusControlBar();
	void DrawBorders(
		CDC *pDC,
		CRect &rc);

	BOOL m_bOuterRectInFirstBand:TRUE;

protected:
	COLORREF m_clrStatusBk;

	CFrameWnd *_GetDockingFrameImpl();
	void _SyncStatusBarColors();
	void _ContextMenuTrack();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTStatusControlBar)
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if (!defined __TSTATUSCONTROLBAR_H)