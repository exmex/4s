// TPathTime.h : main header file for the TPathTime application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CTPathTimeApp:
// See TPathTime.cpp for the implementation of this class
//

class CTPathTimeApp : public CWinApp
{
public:
	CTPathTimeApp();

public:
	CString m_strPATH;
	CSize m_vTJOINT;

	float m_fTIME;
	int m_nSNAP;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual void LoadStdProfileSettings();
	virtual void SaveStdProfileSettings();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CTPathTimeApp theApp;