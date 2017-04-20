// TQuestEditor.h : main header file for the TQUESTEDITOR application
//

#if !defined(AFX_TQUESTEDITOR_H__52C59856_569F_4902_9747_905991806B87__INCLUDED_)
#define AFX_TQUESTEDITOR_H__52C59856_569F_4902_9747_905991806B87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorApp:
// See TQuestEditor.cpp for the implementation of this class
//

class CTQuestEditorApp : public CWinApp
{
public:
	CTQuestEditorApp();

public:
	CMenu m_popup;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTQuestEditorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTQuestEditorApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TQUESTEDITOR_H__52C59856_569F_4902_9747_905991806B87__INCLUDED_)
