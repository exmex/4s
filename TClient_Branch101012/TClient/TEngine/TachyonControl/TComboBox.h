#if (!defined __TCOMBO_BOX_H)
#define __TCOMBO_BOX_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CTComboEditCtrlHook

class CTComboBox;
class __TUI_API CTComboEditCtrlHook : public CEdit
{
	DECLARE_DYNCREATE(CTComboEditCtrlHook)

// Construction
public:
	CTComboEditCtrlHook();

// Attributes
public:

// Operations
public:
	CTComboBox *GetTComboBox();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTComboEditCtrlHook)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTComboEditCtrlHook();

// Generated message map functions
protected:
	//{{AFX_MSG(CTComboEditCtrlHook)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	friend class CTComboBox;
}; // class CTComboEditCtrlHook


/////////////////////////////////////////////////////////////////////////////
// CTComboBox

class __TUI_API CTComboBox : public CComboBox
{
	DECLARE_DYNCREATE(CTComboBox)

// Construction
public:
	CTComboBox();

// Attributes
public:
	BOOL m_bEnableAutoComplete:1;

protected:
	DWORD m_dwUpdatePeriod;
	DWORD m_dwUpdateTimer;

private:
	BOOL m_bAutoComplete:1;
	BOOL m_bDrawing:1;

	CTComboEditCtrlHook *m_pInnerEditHook;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTComboBox();
	virtual BOOL OnTrackComboContextMenu( HWND hWndSrc);

	// Generated message map functions
protected:
	//{{AFX_MSG(CTComboBox)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditCtrlUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void _OnPaintImpl(
		BOOL bPressed,
		BOOL bHover);

	void _OnDrawComboImpl(
		BOOL bPressed,
		BOOL bHover,
		CDC *pDC = NULL);

	friend class CTComboEditCtrlHook;
}; // class CTComboBox

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if (!defined __TCOMBO_BOX_H)