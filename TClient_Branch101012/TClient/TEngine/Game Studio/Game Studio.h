// Game Studio.h : main header file for the Game Studio application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CGameStudioApp:
// See Game Studio.cpp for the implementation of this class
//

class CGameStudioApp : public CWinApp
{
public:
	CMultiDocTemplate* m_pAnimationTemplate;
	CMultiDocTemplate* m_pTextureTemplate;
	CMultiDocTemplate* m_pOBJTypeTemplate;
	CMultiDocTemplate* m_pEXTMAPTemplate;
	CMultiDocTemplate* m_pBSPMAPTemplate;
	CMultiDocTemplate* m_pImageTemplate;
	CMultiDocTemplate* m_pMediaTemplate;
	CMultiDocTemplate* m_pFRAMETemplate;
	CMultiDocTemplate* m_pCOMPTemplate;
	CMultiDocTemplate* m_pMeshTemplate;
	CMultiDocTemplate* m_pOBJTemplate;
	CMultiDocTemplate* m_pSFXTemplate;
	CMenu m_menuPopup;
	ULONG_PTR m_pGDI;

public:
	void ClearFolder( CString strPATH);

public:
	CGameStudioApp();

// Overrides
public:
	virtual void LoadStdProfileSettings();
	virtual void SaveStdProfileSettings();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	virtual BOOL SaveAllModified();
	virtual int Run();
};

extern CGameStudioApp theApp;