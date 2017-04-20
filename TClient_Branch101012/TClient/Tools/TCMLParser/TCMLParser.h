// TCMLParser.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CTCMLParserApp:
// See TCMLParser.cpp for the implementation of this class
//

class CTCMLParserApp : public CWinApp
{
public:
	CTCMLParserApp();

// Overrides
	public:
	virtual void LoadStdProfileSettings();
	virtual void SaveStdProfileSettings();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTCMLParserApp theApp;