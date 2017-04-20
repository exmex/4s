// TClient.h : main header file for the TClient application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

// CTClientApp:
// See TClient.cpp for the implementation of this class
//

class CTClientApp : public CTachyonApp
{
public:
	CTClientApp();

public:
	static CString m_vKEYField[TKEY_COUNT];
	static WORD m_vKEY[TKEY_COUNT];
	static DWORD m_dwAppTick;
	static CTime m_dAppDate;
	static CTime m_dCurDate;

public:
	QTLOADCMD m_qTLOADCMD;
	ULONG_PTR m_pGDI;

	CRITICAL_SECTION m_cs;
	HANDLE m_hLoadEvent;
	HANDLE m_hLoadThread;

	BYTE m_bFirstRun;
	BYTE m_bLoadRun;
	BOOL m_bLoaded;

	BOOL m_bNeedWorldUnify; 
	BYTE m_bNeedSleep;
	BYTE m_bWait;
	BYTE m_bCancelLoad;
	BYTE m_bForceConnect;
	BYTE m_bHackMSG;

	DWORD m_dwUserID;
	DWORD m_dwPrmPCROOM;
	INT64 m_dlCheckFile;
#ifdef TEST_MODE
#else
	CTModuleProtector m_vModuleGuard;
#endif

	CNPGameLib*		m_pNpgl;			// 게임가드.
	CString			m_strNPMSG;			// 게임가드 에러메세지.

	CPacketSpyDlg* m_pPacketSpy;

	CString m_strHackMSG[3];

#ifdef KEYHOOK_CTRLALTDEL
	static HHOOK	m_hHook;
	static bool		m_bKeyDownCAD;
#endif

	CTClientNICEventSink	m_TNICEventSink;

public:
	void DeleteTLoadCMD( LPTLOADCMD pTCMD);
	void ClearTLoadCMD();

	void ExitLoadThread( BYTE bCancelCMD);
	BYTE CreateLoadThread();

#ifndef TEST_MODE
	static int __stdcall HS_CallbackProc ( long lCode, long lParamSize, void* pParam );
	BOOL HackShield_Update();
	BOOL HackShield_Init();
	BOOL HackShield_UnInit();
	BOOL HackShield_StartService();
	BOOL HackShield_StopService();
#endif

#ifdef KEYHOOK_CTRLALTDEL
public :
	static HRESULT CALLBACK KeyHookProc( int nCode, WPARAM wParam, LPARAM lParam );
#endif

// Overrides
public:
	virtual void LoadStdProfileSettings();
	virtual void SaveStdProfileSettings();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
	static DWORD WINAPI _LoadThread( LPVOID lpParam);

public:
	static void ToggleImeLocalMode();
	static BOOL IsImeLocalMode();

// Implementation
public:
	DECLARE_MESSAGE_MAP()
	virtual BYTE MainProc();
	virtual int Run();
};

extern CTClientApp theApp;