// TIDHBuilder.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CTIDHBuilderApp:
// See TIDHBuilder.cpp for the implementation of this class
//

class CTIDHBuilderApp : public CWinApp
{
public:
	CTIDHBuilderApp();

// Overrides
	public:
	virtual void LoadStdProfileSettings();
	virtual void SaveStdProfileSettings();
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTIDHBuilderApp theApp;