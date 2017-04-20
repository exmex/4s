// TExplorer.h : main header file for the TExplorer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CTExplorerApp:
// See TExplorer.cpp for the implementation of this class
//

class CTExplorerApp : public CTachyonApp
{
public:
	CTExplorerApp();

public:
	static CString m_vKEYField[TKEY_COUNT];
	static WORD m_vKEY[TKEY_COUNT];

	static DWORD m_dwAppTick;

public:
	ULONG_PTR m_pGDI;

// Overrides
public:
	virtual void LoadStdProfileSettings();
	virtual void SaveStdProfileSettings();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
public:
	DECLARE_MESSAGE_MAP()
};

extern CTExplorerApp theApp;