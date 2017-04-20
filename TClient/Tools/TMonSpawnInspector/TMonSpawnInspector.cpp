// TMonSpawnInspector.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "TMonSpawnInspector.h"
#include "TMonSpawnInspectorMAP.h"
#include "TMonSpawnInspectorObjBase.h"
#include "TMonSpawnInspectorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTMonSpawnInspectorApp

BEGIN_MESSAGE_MAP(CTMonSpawnInspectorApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTMonSpawnInspectorApp 생성

CTMonSpawnInspectorApp::CTMonSpawnInspectorApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CTMonSpawnInspectorApp 개체입니다.

CTMonSpawnInspectorApp theApp;


// CTMonSpawnInspectorApp 초기화

BOOL CTMonSpawnInspectorApp::InitInstance()
{
	CWinApp::InitInstance();

	SetRegistryKey(_T("TMonSpawnInspector"));

	m_pMainWnd = &GetMainDlg();

	INT_PTR nResponse = GetMainDlg().DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}
