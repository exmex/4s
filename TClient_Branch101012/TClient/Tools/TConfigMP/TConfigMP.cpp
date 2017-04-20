// TConfigMP.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "TConfigMP.h"
#include "TConfigMPDlg.h"
#include ".\tconfigmp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTConfigMPApp

BEGIN_MESSAGE_MAP(CTConfigMPApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTConfigMPApp 생성

CTConfigMPApp::CTConfigMPApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CTConfigMPApp 개체입니다.

CTConfigMPApp theApp;


// CTConfigMPApp 초기화

BOOL CTConfigMPApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다. 
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("Tachyon Engine"));

	CTConfigMPDlg dlg;
	m_pMainWnd = &dlg;

	dlg.m_strMAINPEFOLDER.Empty();
	dlg.m_strDESTFOLDER.Empty();
	dlg.m_strSRCFOLDER.Empty();
	LoadStdProfileSettings();

	INT_PTR nResponse = dlg.DoModal();
	m_pMainWnd = &dlg;
	SaveStdProfileSettings();
	m_pMainWnd = NULL;

	if (nResponse == IDOK)
	{
		// TODO: 여기에 대화 상자가 확인을 눌러 없어지는 경우 처리할
		// 코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 대화 상자가 취소를 눌러 없어지는 경우 처리할
		// 코드를 배치합니다.
	}

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고
	// 응용 프로그램을 끝낼 수 있도록 FALSE를 반환합니다.
	return FALSE;
}

int CTConfigMPApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

void CTConfigMPApp::LoadStdProfileSettings()
{
	CWinApp::LoadStdProfileSettings();

	((CTConfigMPDlg *) m_pMainWnd)->m_strMAINPEFOLDER = GetProfileString(
		_T("Config"),
		_T("MAINPE Folder"),
		_T(""));

	((CTConfigMPDlg *) m_pMainWnd)->m_strDESTFOLDER = GetProfileString(
		_T("Config"),
		_T("DEST Folder"),
		_T(""));

	((CTConfigMPDlg *) m_pMainWnd)->m_strSRCFOLDER = GetProfileString(
		_T("Config"),
		_T("SRC Folder"),
		_T(""));
}

void CTConfigMPApp::SaveStdProfileSettings()
{
	CWinApp::SaveStdProfileSettings();

	WriteProfileString(
		_T("Config"),
		_T("MAINPE Folder"),
		LPCSTR(((CTConfigMPDlg *) m_pMainWnd)->m_strMAINPEFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("DEST Folder"),
		LPCSTR(((CTConfigMPDlg *) m_pMainWnd)->m_strDESTFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("SRC Folder"),
		LPCSTR(((CTConfigMPDlg *) m_pMainWnd)->m_strSRCFOLDER));
}
