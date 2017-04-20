#if (!defined __TCOLORDLG_H)
#define __TCOLORDLG_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __TCOLORCTRL_H)
	#include <TColorCtrl.h>
#endif

#if (!defined __TBUTTON_H)
	#include <TButton.h>
#endif

#if (!defined __TCOMBO_BOX_H)
	#include <TComboBox.h>
#endif

#if (!defined __TRESIZABLE_DIALOG_H)
	#include <TResizableDialog.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CTColorDlg dialog

class __TUI_API CTColorDlg : public CTResizableDialog
{
public:
	CTColorDlg(
		COLORREF clrInit = RGB( 255, 255, 255),
		COLORREF clrNew = RGB( 255, 255, 255),
		CWnd *pParent = NULL);

	COLORREF m_clrInit;
	COLORREF m_clrNew;
	CString m_strCaption;

// Dialog Data
	//{{AFX_DATA(CTColorDlg)
	CSpinButtonCtrl m_SpinC;
	CSpinButtonCtrl m_SpinM;
	CSpinButtonCtrl m_SpinY;
	CSpinButtonCtrl m_SpinS;
	CSpinButtonCtrl m_SpinR;
	CSpinButtonCtrl m_SpinL;
	CSpinButtonCtrl m_SpinH;
	CSpinButtonCtrl m_SpinG;
	CSpinButtonCtrl m_SpinB;
	CEdit m_EditC;
	CEdit m_EditM;
	CEdit m_EditY;
	CEdit m_EditS;
	CEdit m_EditR;
	CEdit m_EditL;
	CEdit m_EditH;
	CEdit m_EditG;
	CEdit m_EditB;

	CTComboBox m_ComboType;
	CTButton m_BtnCancel;
	CTButton m_BtnOK;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bDialogInitColplete;
	BOOL m_bSyncingValues;
	HICON m_hIcon;

	CTColorCtrl m_wndColorCtrl;
	CTColorCtrl m_wndColorDiff;

	// Generated message map functions
	//{{AFX_MSG(CTColorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSelendokComboSelType();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void SyncFromEditRGB();
	afx_msg void SyncFromEditCMY();
	afx_msg void SyncFromEditHSL();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	LRESULT OnColorChanged(WPARAM wParam,LPARAM lParam);

private:
	void SyncToEditRGB(
		COLORREF clr,
		BOOL bSyncRGB,
		BOOL bSyncCMY,
		BOOL bSyncHLS);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if (!defined __TCOLORDLG_H)