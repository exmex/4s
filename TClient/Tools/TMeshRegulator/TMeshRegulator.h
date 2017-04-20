// TMeshRegulator.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CTMeshRegulatorApp:
// See TMeshRegulator.cpp for the implementation of this class
//

class CTMeshRegulatorApp : public CWinApp
{
public:
	CTMeshRegulatorApp();

// Overrides
	public:
	virtual void LoadStdProfileSettings();
	virtual void SaveStdProfileSettings();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTMeshRegulatorApp theApp;