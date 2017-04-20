// 4Story.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"

// CStoryApp
CStoryApp theApp;

BEGIN_MESSAGE_MAP(CStoryApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CStoryApp::CStoryApp()
{
}

BOOL CStoryApp::InitInstance()
{
	InitCommonControls();
	CWinApp::InitInstance();
	AfxEnableControlContainer();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성한 응용 프로그램"));

	CStoryDlg dlg;
	
#ifdef _MODULPROTECTE
	if(!dlg.BeginProtect())
	{
		AfxMessageBox(IDS_ERROR_LOAD_FILE);
		return FALSE;
	}
#endif

	m_pMainWnd = &dlg;
	dlg.DoModal();
	//INT_PTR nResponse = dlg.DoModal();
	//if (nResponse == IDOK)
	//{
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//}

	return FALSE;
}

void CStoryApp::InsertResolution(DWORD dwWidth,DWORD dwHeight,CString strMode)
{
	SCREENMODE SCRMODE;
	SCRMODE.dwWidth = dwWidth;
	SCRMODE.dwHeight = dwHeight;
	SCRMODE.strMode = strMode;

	m_vScreenMode.push_back(SCRMODE);
}