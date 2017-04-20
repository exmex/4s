/////////////////////////////////////////////////////////////////////////////
//	File Name	:	LogTool.h
//
//	Copyright	:	(c) Zemi Interactive, Inc 2002~2008
//
/////////////////////////////////////////////////////////////////////////////
#pragma once


#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLogToolApp:
// See LogTool.cpp for the implementation of this class
//

class CLogToolApp : public CWinApp
{
public:
	CLogToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLogToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////