#if !defined(AFX_TLISTCTRL_H__D631848E_FD41_4825_8795_78FC08D7C0E3__INCLUDED_)
#define AFX_TLISTCTRL_H__D631848E_FD41_4825_8795_78FC08D7C0E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTListCtrl window

class CTListCtrl : public CListCtrl
{
// Construction
public:
	CTListCtrl();

// Attributes
public:
	void SetCurSel( int nIndex);
	int GetCurSel();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTListCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TLISTCTRL_H__D631848E_FD41_4825_8795_78FC08D7C0E3__INCLUDED_)
