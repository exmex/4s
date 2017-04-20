// TMapViewer.h : main header file for the TMapViewer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CTMapViewerApp:
// See TMapViewer.cpp for the implementation of this class
//

class CTMapViewerApp : public CTachyonApp
{
public:
	CTMapViewerApp();

public:
	static CString m_vKEYField[TKEY_COUNT];
	static WORD m_vKEY[TKEY_COUNT];

	static DWORD m_dwAppTick;

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

extern CTMapViewerApp theApp;