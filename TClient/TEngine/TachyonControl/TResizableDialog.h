#if (!defined __TRESIZABLEDIALOG_H)
#define __TRESIZABLEDIALOG_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if (!defined __TTEMPL_H)
	#include <TTempl.h>
#endif // __TTEMPL_H

#if (!defined __TBUTTON_H)
	#include <TButton.h>
#endif // __TBUTTON_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTResizableDialog

#if _MFC_VER < 0x700
	#define __DLGBASE		CDialog
#else
	#define __DLGBASE		CTADLG<CDialog>
#endif


class __TUI_API CTResizableDialog : public CTWA<CTWS<__DLGBASE> >
{
	BOOL m_bEnabledControlBarUpdate:TRUE;
	BOOL m_bInConrolBarUpdate:TRUE;

// Construction
public:
	DECLARE_DYNAMIC(CTResizableDialog)

	CTResizableDialog();
	CTResizableDialog(
		UINT nIDTemplate,
		CWnd *pParentWnd = NULL);

	CTResizableDialog(
		LPCTSTR lpszTemplateName,
		CWnd *pParentWnd = NULL);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTResizableDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:

// Generated message map functions
protected:
	//{{AFX_MSG(CTResizableDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	virtual void OnCancel();
}; // class CTResizableDialog


/////////////////////////////////////////////////////////////////////////////
// CTResizablePropertyPage

#if _MFC_VER < 0x700
	#define __PAGEBASE		CPropertyPage
#else
	#define __PAGEBASE		CTAPPG<CPropertyPage>
#endif

class __TUI_API CTResizablePropertyPage : public CTWA< CTWS<__PAGEBASE> >
{
	BOOL m_bEnabledControlBarUpdate:TRUE;
	BOOL m_bInConrolBarUpdate:TRUE;

// Construction
public:
	DECLARE_DYNAMIC(CTResizablePropertyPage)

	CTResizablePropertyPage();
	CTResizablePropertyPage(
		UINT nIDTemplate,
		UINT nIDCaption = 0);

	CTResizablePropertyPage(
		LPCTSTR lpszTemplateName,
		UINT nIDCaption = 0);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTResizablePropertyPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:

// Generated message map functions
protected:
	//{{AFX_MSG(CTResizablePropertyPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	virtual void OnCancel();
}; // class CTResizablePropertyPage


/////////////////////////////////////////////////////////////////////////////
// CTResizablePropertySheet

#if _MFC_VER < 0x700
	#define __SHEETBASE			CPropertySheet
#else
	#define __SHEETBASE			CTAPSH<CPropertySheet>
#endif


class __TUI_API CTResizablePropertySheet : public CTWA< CTWS<__SHEETBASE> >
{
	BOOL m_bSheetInitialized:TRUE;
	void _SyncActivePage();

	class __TUI_API CButtonInSheetHook : public CTButton
	{
	public:
		virtual void PostNcDestroy()
		{
			CTButton::PostNcDestroy();
			delete this;
		}
	}; // class CButtonInSheetHook

// Construction
public:
	DECLARE_DYNAMIC(CTResizablePropertySheet)
	CTResizablePropertySheet();

	CTResizablePropertySheet(
		UINT nIDCaption,
		CWnd *pParentWnd = NULL,
		UINT nSelectPage = 0);

	CTResizablePropertySheet(
		LPCTSTR pszCaption,
		CWnd *pParentWnd = NULL,
		UINT nSelectPage = 0);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTResizablePropertySheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void OnResizablePropertySheetInitialized();

// Generated message map functions
protected:
	//{{AFX_MSG(CTResizablePropertySheet)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
}; // class CTResizablePropertySheet


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if (!defined __TRESIZABLEDIALOG_H)