// 4StoryDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Unzip.h"

#ifdef _KOR
#include <XTrapArg.h>
#include <XTrap4Launcher.h>
#endif

#define TMP_TIMER						(100)

// CStoryDlg 대화 상자
ULONGLONG CStoryDlg::m_lVIDEOMEM = 0;
ULONGLONG CStoryDlg::m_lSYSMEM = 0;


CStoryDlg::CStoryDlg(CWnd* pParent /*=NULL*/)
: CDialog(CStoryDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strHompageURLForLuancher = HOMEPAGE_NAME;
	m_strNoticeURLForLuancher = LAUNCHERWEB_NAME;

	m_bHACK = TMP_HACK_NONE;
	m_bRUN = FALSE;
	m_hWND = NULL;
	m_hMP = NULL;
	sFlag = NULL;

	m_bCancel = FALSE;
	m_blPatchFileUpdate = FALSE;
	m_bDownloading = FALSE;
	m_bIsFtpConnecting = FALSE;
	m_strAppName = APP_NAME;
	m_hDownload = NULL;
	m_bProgressingColor = FALSE;
	m_dwMinBetaVer = 0;
	m_bFirstPrePatch = FALSE;
	m_bSecond	= 0;
	m_bFlash = 0;
	m_Ui.Default();
}
CStoryDlg::~CStoryDlg() 
{
	if( sFlag )
	{
		m_bSkin.Detach();
		//::DeleteObject(hBmp);
	}

	// 버튼 깜박임
	if(m_bFlash)
	{
		m_bFlash = FALSE;
		KillTimer(1);
	}
	if(m_bSecond)
	{
		m_bSecond = 0;
		KillTimer(99);
	}
}
void CStoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_CURRENT, m_progressCurrent);
	DDX_Control(pDX, IDC_PROGRESS_TOTAL, m_progressTotal);
	DDX_Control(pDX, IDC_BUTTON_HOMEPAGE, m_bHome);
	DDX_Control(pDX, IDC_BUTTON_SETTING, m_bSetting);
	DDX_Control(pDX, IDC_CHK_PREPATCH, m_chkPrePatch);
	DDX_Control(pDX, IDOK, m_bOK);
	DDX_Control(pDX, IDCANCEL, m_bCANCEL);
}

BEGIN_MESSAGE_MAP(CStoryDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_HOMEPAGE, OnBnClickedButtonHomepage)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, OnBnClickedButtonSetting)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHK_PREPATCH, OnBnClickedChkPrepatch)
END_MESSAGE_MAP()


// CStoryDlg 메시지 처리기
BOOL CStoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_bCancel = FALSE;
	m_blPatchFileUpdate = FALSE;
	m_bDownloading = FALSE;
	m_bIsFtpConnecting = FALSE;
	m_strAppName = APP_NAME;
	m_hDownload = NULL;

	HMODULE hModule = ::GetModuleHandle(NULL);
	if(hModule == 0)
	{			
		OnCancel();
		return FALSE;
	}
	::GetModuleFileName(hModule, m_szPatchFileName, _MAX_PATH);

	char GAME_TITLE[256]={0,};	
	CString strNew = m_szPatchFileName;	
	int n = strNew.Find(APP_EXT);
	if(n > 0)
	{
		strNew.Delete(n,lstrlen(APP_EXT));
		n = strNew.ReverseFind('\\');
		if(n >= 0)
			strNew.Delete(0, n+1);
	}
	strcpy(GAME_TITLE,strNew);

	HANDLE hMutex = NULL;
	if( !OpenMutex(MUTEX_ALL_ACCESS,false,GAME_TITLE) )
		hMutex = CreateMutex(NULL,TRUE,GAME_TITLE);
	else
	{	
		memset(GAME_TITLE,0,sizeof(GAME_TITLE));
		AfxMessageBox(IDS_ERROR_RUNING);
		OnCancel();
		return FALSE;
	}

	WSADATA wsaDATA;
	WORD wVersionRequested = MAKEWORD( 2, 2);
	int nERROR = WSAStartup( wVersionRequested, &wsaDATA);

#ifdef _GAMEFORGE
	ReadURLFile();
	ReadTextFile();
#endif

#ifdef _MODULPROTECTE	
	SetHWND(GetSafeHwnd());
#endif
/*
#ifdef _KOR
	XTrap_L_Patch((char*)xArg, NULL, 60000);
	m_bSecond = 1;
	SetTimer(99,1000,NULL);
#endif
*/
	if(!ReadRegistry())
	{
		if( AfxMessageBox(IDS_ERROR_REG,MB_OK ) == IDOK)
		{
			OnCancel();
			return FALSE;
		}
	}
#ifdef _DIS
	if ( !m_bDisclaimer )
	{
		ReadDisclaimerFile();
		if( AfxMessageBox( m_strDisclaimer, MB_OKCANCEL ) == IDCANCEL )
		{
			SetDisclaimer(FALSE);
			AfxPostQuitMessage(0);
			return TRUE;
		}
		SetDisclaimer(TRUE);
	}
#endif

	InfoFileRead();
	m_session.SetOwner(this);
	SessionStart(m_strIP, m_wPort);

	m_progressCurrent.SetRange( 0, 100 );
	m_progressTotal.SetRange( 0, 100 );

	CButton * pOK = (CButton *)GetDlgItem(IDOK);
	if(pOK)
		pOK->EnableWindow(FALSE);
#ifndef _KOR
	// [환경설정] 버튼 비활성화 JINUK
	CButton * pSet = (CButton *)GetDlgItem(IDC_BUTTON_SETTING);
	if(pSet)
		pSet->EnableWindow(FALSE);
#endif
	m_bFlash = FALSE; // 버튼 깜박임
//
//	////////////////////////////////////////////////////////////////
//	m_chkPrePatch.SetSkin(IDB_BIT_CHECK_NORMAL,IDB_BIT_CHECK_DOWN);
//	//m_bOK.LoadBitmap(IDB_BITMAP_SN, IDB_BITMAP_SP, IDB_BITMAP_SH, IDB_BITMAP_SD);
//	m_bOK.SetSkin(IDB_BITMAP_SN, IDB_BITMAP_SP, IDB_BITMAP_SH, IDB_BITMAP_SD);
//	m_bCANCEL.SetSkin(IDB_BITMAP_EN, IDB_BITMAP_EP, IDB_BITMAP_EH);
//	m_bHome.SetSkin(IDB_BITMAP_HN, IDB_BITMAP_HP, IDB_BITMAP_HH, 0);
//#ifndef _KOR
//	m_bSetting.SetSkin(IDB_BITMAP_ON, IDB_BITMAP_OP, IDB_BITMAP_OH, IDB_BITMAP_OD);
//#else
//	m_bSetting.SetSkin(IDB_BITMAP_ON, IDB_BITMAP_OP, IDB_BITMAP_OH, 0);
//#endif

	m_progressCurrent.SetBkColor(TRANSPARENT);
	m_progressTotal.SetBkColor(TRANSPARENT);

	UXSetting();
	////////////////////////////////////////////////////////////////
	SetWindowText(m_strAppName);

	m_chkPrePatch.SetCheck(CheakStartRegistry());

#ifndef _GAMEFORGE
	if( CheckIsFirst() )
	{
		ShowSettingFrame();

		int SCREEN_X = GetSystemMetrics( SM_CXSCREEN );
		int SCREEN_Y = GetSystemMetrics( SM_CYSCREEN );

		CString strResolution;
		strResolution.Format( _T("%d X %d"), SCREEN_X, SCREEN_Y );

		CGameSetting*	pGameSetting = CGameSetting::GetInstance();
		pGameSetting->SetResolutionCtrl( strResolution );
		pGameSetting->SetResolution( strResolution );
	}
#endif
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CStoryDlg::OnPaint() 
{
	if( sFlag )
	{	
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);	
		m_bSkin.BitBlt(dc.m_hDC, 0,0);
	}

	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
		CDialog::OnPaint();
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CStoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CStoryDlg::DestroyWindow()
{
	DownloadEnd();
	return CDialog::DestroyWindow();
}

LRESULT CStoryDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch(message)
	{
	case WM_RESTART_PATCH:
		{
			OnCancel();

			m_strDownload.ReleaseBuffer();
			CString strNew=m_szPatchFileName;

			STARTUPINFO StartupInfo;
			PROCESS_INFORMATION ProcessInfo;

			ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
			ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION)); 

			StartupInfo.cb = sizeof(STARTUPINFO);
			StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
			StartupInfo.wShowWindow = SW_SHOWNORMAL;

			int n = strNew.ReverseFind('.');
			strNew.Insert(n,BACKUP_EXT);

			if(!CreateProcess(strNew, NULL, 
				NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
				AfxMessageBox(IDS_DOWNLOAD_FAIL);
		}
		break;
	case WM_STOP_DOWNLOAD:
		DestroyWindow();
		break;

	case WM_CLOSE_SESSION:
		DownloadEnd();
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CStoryDlg::InitWeb()
{
	CString strUrl = m_strNoticeURLForLuancher;
	if(strUrl == _T(""))
		strUrl = LAUNCHERWEB_NAME;

	m_webCtrl.Create(_T("WebControl"), WS_CHILD|WS_VISIBLE, m_Ui.m_rtView, (CWnd*)this, 1111 );
	m_webCtrl.Navigate( strUrl, NULL, NULL, NULL, NULL );
#ifdef _KOR	
	::ShowWindow(m_webCtrl.GetSafeHwnd(),SW_HIDE);
#endif
}

BYTE CStoryDlg::ReadRegistry()
{
	CString strDisclaimer;
	BYTE bCurrent = FALSE;

	HKEY hKeyRet;	
	HKEY hKey = HKEY_LOCAL_MACHINE;

	CString strSubkey;
	strSubkey = m_strSubkey;
	if(strSubkey == _T(""))
		strSubkey = m_strAppName + REG_COUNTRY;

	strSubkey += _T("\\PB");	

	m_strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

	int err = RegOpenKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
	{
		RegCloseKey(hKeyRet);
		hKey = HKEY_CURRENT_USER;

		bCurrent = TRUE;
	}

	err = RegCreateKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
		return FALSE;

	BYTE	data[1024];
	DWORD   type;
	DWORD   cbdata =1024;

	//version
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_VERSION, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	m_dwVersion = *((LPDWORD)data);
	m_dwNextVersion = m_dwVersion;

	//local path 
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_LOCAL, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_SZ)
		return FALSE;

	m_strLocal = m_strDownload = data;
	m_strDownload += _T("_download");

	if(!SetCurrentDirectory(m_strLocal))
	{
		AfxMessageBox(IDS_ERROR_LOAD_FILE);
		return FALSE;
	}

	//exe file
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_EXE, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_SZ)
		return FALSE;

	m_strGame = data;

	//patch svr address
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_PATCHSVR, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_SZ)
		return FALSE;

	m_strIP = data;

	//patch svr port
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_PATCHPORT, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	m_wPort = *((LPWORD)data);

#ifdef _DIS
	// Disclaimer
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_DISCLAIMER, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_SZ)
		m_bDisclaimer = FALSE;
	else
	{
		strDisclaimer = data;
		strDisclaimer.MakeUpper();

		if ( strDisclaimer == CString(_T("TRUE")) )
			m_bDisclaimer = TRUE;
		else
			m_bDisclaimer = FALSE;
	}
#endif

	//	PrePatch 처음 설치 체크
	if(RegQueryValueEx(hKeyRet, _T("PrePatchFirst"), NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
	{
		m_bFirstPrePatch = TRUE;

		//version
		err = RegSetValueEx(hKeyRet, _T("PrePatchFirst"), 0, REG_NONE, NULL, NULL);
		if( ERROR_SUCCESS != err)
			return FALSE;
	}

	RegCloseKey(hKeyRet);

	if(bCurrent)
	{
		if(!CopyRegistry())
			return FALSE;
	}

	return TRUE;
}

void CStoryDlg::ReadURLFile()
{
	CStdioFile file;
	if( file.Open( "url.txt", CFile::modeRead | CFile::typeText ) )
	{
		file.ReadString( m_strHompageURLForLuancher );

		CString strLinkForGame;
		file.ReadString( strLinkForGame );

		CString strBtnNameForGame;
		file.ReadString( strBtnNameForGame );

		file.ReadString( m_strNoticeURLForLuancher );

		file.Close();
	}
}

void CStoryDlg::ReadTextFile()
{
	CString str;
	m_strSubkey = _T("");

	CStdioFile file;
#ifndef _GOA
	if( file.Open( "4story.txt", CFile::modeRead | CFile::typeText ) )
#else
	if( file.Open( "GOA.txt", CFile::modeRead | CFile::typeText ) )
#endif
	{
		file.ReadString( m_strSubkey );

		file.Close();
	}
}

BYTE CStoryDlg::CopyRegistry()
{
	HKEY hKeyRet;
	HKEY hKey = HKEY_LOCAL_MACHINE;

	CString strSubkey;
	strSubkey = m_strSubkey;
	if(strSubkey == _T(""))
		strSubkey = m_strAppName + REG_COUNTRY;
	strSubkey += _T("\\PB");

	m_strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

	int err = RegCreateKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
		return FALSE;

	BYTE	data[1024];
	DWORD   type = REG_DWORD;
	DWORD   cbData = 4;

	memcpy(data, &m_dwVersion, sizeof(DWORD));
	err = RegSetValueEx(hKeyRet, REG_VALUE_VERSION, 0, REG_DWORD, data, cbData);
	if(ERROR_SUCCESS != err)
		return FALSE;

	LPCSTR strLocal = m_strLocal;
	err = RegSetValueEx(
		hKeyRet,
		REG_VALUE_LOCAL, 
		0, 
		REG_SZ, 
		(LPBYTE)strLocal, 
		(DWORD) (lstrlen(strLocal)+1)*sizeof(TCHAR));
	if(ERROR_SUCCESS != err)
		return FALSE;

	LPCSTR strGame = m_strGame;
	err = RegSetValueEx(
		hKeyRet,
		REG_VALUE_EXE, 
		0, 
		REG_SZ, 
		(LPBYTE)strGame, 
		(DWORD) (lstrlen(strGame)+1)*sizeof(TCHAR));
	if(ERROR_SUCCESS != err)
		return FALSE;

	LPCSTR strIP = m_strIP;
	err = RegSetValueEx(
		hKeyRet,
		REG_VALUE_PATCHSVR, 
		0, 
		REG_SZ, 
		(LPBYTE)strIP, 
		(DWORD) (lstrlen(strIP)+1)*sizeof(TCHAR));
	if(ERROR_SUCCESS != err)
		return FALSE;

	DWORD dwPort = m_wPort;
	memcpy(data, &dwPort, sizeof(DWORD));
	err = RegSetValueEx(hKeyRet, REG_VALUE_PATCHPORT, 0, REG_DWORD, data, cbData);
	if(ERROR_SUCCESS != err)
		return FALSE;

	RegCloseKey(hKeyRet);
	return TRUE;
}
void CStoryDlg::LoadSkin()
{
	m_bSkin.LoadFromResource(AfxGetInstanceHandle(),IDB_BITMAP_BASE);
	sFlag = !m_bSkin.IsNull();

	if(sFlag)
		SetWindowPos( NULL, 0, 0, m_bSkin.GetWidth(), m_bSkin.GetHeight(), SWP_NOZORDER );

	//	m_bSkin.LoadBitmap(IDB_BITMAP_BASE);
	//if( sFlag )
	//{
	//	BITMAP bmp;
	//	m_bSkin.GetBitmap(&bmp);

	//	SetWindowPos( NULL, 0, 0, bmp.bmWidth, bmp.bmHeight, SWP_NOZORDER );
	//}
}
void CStoryDlg::SetControlPos()
{
	m_progressCurrent.MoveWindow( &m_Ui.m_rtCurrent);//.left,m_Ui.m_rtCurrent.top, m_Ui.m_rtCurrent.right, m_Ui.m_rtCurrent.bottom);
	m_progressTotal.MoveWindow( &m_Ui.m_rtTotal);

	RECT rt;
	m_bHome.GetClientRect(&rt);
	m_bHome.MoveWindow( m_Ui.m_Home.x, m_Ui.m_Home.y, rt.right, rt.bottom );
	m_bSetting.GetClientRect(&rt);
	m_bSetting.MoveWindow( m_Ui.m_Set.x, m_Ui.m_Set.y, rt.right, rt.bottom );
	m_bOK.GetClientRect(&rt);
	m_bOK.MoveWindow( m_Ui.m_Connect.x, m_Ui.m_Connect.y , rt.right, rt.bottom );
	m_bCANCEL.GetClientRect(&rt);
	m_bCANCEL.MoveWindow( m_Ui.m_Close.x, m_Ui.m_Close.y, rt.right, rt.bottom );
	m_chkPrePatch.GetClientRect(&rt);
	m_chkPrePatch.MoveWindow(m_Ui.m_PrePatch.x, m_Ui.m_PrePatch.y, rt.right, rt.bottom );
	InitWeb();

	//m_progressCurrent.MoveWindow( 326, 431, 434, 8 );
	//m_progressTotal.MoveWindow( 326, 447, 434, 8 );
//
//	RECT rt;
//	m_bHome.GetClientRect(&rt);
//	m_bHome.MoveWindow( 229, 476, rt.right, rt.bottom );
//	m_bSetting.GetClientRect(&rt);
//	m_bSetting.MoveWindow( 367, 476, rt.right, rt.bottom );
//	m_bOK.GetClientRect(&rt);
//	m_bOK.MoveWindow( 505, 476, rt.right, rt.bottom );
//	m_bCANCEL.GetClientRect(&rt);
//	m_bCANCEL.MoveWindow( 643, 476, rt.right, rt.bottom );
//	m_chkPrePatch.GetClientRect(&rt);
//	m_chkPrePatch.MoveWindow(749, 512, rt.right, rt.bottom );
//#ifndef _KOR	
//	InitWeb();
//#endif
}
void CStoryDlg::OnConnect(int nErrorCode)
{
	if(nErrorCode)
	{
		AfxMessageBox(IDS_CONNECT_FAIL);
		return;
	}
	else
		SendCT_NEWPATCH_REQ();
}
void CStoryDlg::OnClose(int nErrorCode)
{
	if(nErrorCode)
	{
		DownloadEnd();
		CString str;
		str.Format(_T("Error Code %d"), nErrorCode);
		AfxMessageBox(str);
	}
}
DWORD CStoryDlg::OnReceive(CPacket * pPacket)
{
	switch(pPacket->GetID())
	{
		HP_RECEIVE(CT_NEWPATCH_ACK)
	default:
		return 1;
	}
}
void CStoryDlg::SessionStart(CString strIp, DWORD dwPort)
{
	if(!m_session.Start(strIp, dwPort))
		AfxMessageBox(IDS_CONNECT_FAIL);
}
void CStoryDlg::Say(CPacket * pPacket)
{
	m_session.Say(pPacket);
}

DWORD WINAPI CStoryDlg::_Download(LPVOID lpParam)
{
	return ((CStoryDlg*)lpParam)->Download();
}

DWORD CStoryDlg::Download()
{
	m_bDownloading = TRUE;
	////////////////////////////////////////////////
	// 현승룡 웹 뜨기전까지 쓰레드 실행 막음
	while( m_webCtrl.GetBusy() )
		Sleep(500);
	//	프리패치 종료 전까지 쓰레드 실행 막음
	while(FindWindow(NULL, PREPATCH_NAME))
		Sleep(100);
	////////////////////////////////////////////////	
	CFile		fileNew;
	CStdioFile* remotefile	= NULL;
	__int64		nTotalRead	= 0;

	while(m_bDownloading)
	{
		LPPATCHFILE pPatchFile = NULL;
		VPATCHFILE::iterator PrePatchItr;

		if(m_vPatchFile.size())
			pPatchFile = (LPPATCHFILE)m_vPatchFile.front();

		if(!pPatchFile)
			break;

		//	PrePatch 검사
		BYTE bPatchType = PrePatchCheak(pPatchFile, PrePatchItr);

		//	디렉토리 설정
		if(!SetPath(pPatchFile, bPatchType))
		{
			m_bDownloading = FALSE;
			AfxMessageBox(IDS_ERROR_LOAD_FILE);
			break;
		}

		//	파일 다운로드
		if(bPatchType != DOWN_PREPATCH)
		{
			DWORD		dwRead = 0;

			TRY
			{
				//	URL 접속, 파일연결
				CString strURL = m_strFtpSvr + _T("/");
				if(!pPatchFile->m_strPath.IsEmpty())
					strURL += pPatchFile->m_strPath + _T("/");
				strURL += pPatchFile->m_strName;

				CInternetSession mysession;
				remotefile = mysession.OpenURL(strURL,1,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD);
				if(!remotefile)
				{
					m_bDownloading = FALSE;
					break;
				}
				//	이어받기인가
				if(DownloadRead(pPatchFile) || bPatchType == DOWN_INCOMPRE)
				{
					if(fileNew.Open(pPatchFile->m_strName, CFile::modeNoTruncate | CFile::modeWrite))
					{
						dwRead = (DWORD)fileNew.SeekToEnd();					
						remotefile->Seek(dwRead, CFile::begin);
						if(dwRead)
						{
							nTotalRead += dwRead;
							Progress(pPatchFile->m_strName, pPatchFile->m_dwSize, dwRead, nTotalRead,PROGRESS_TYPE_DOWNLOAD);
						}
					}
				}
				else
				{
					if(fileNew.Open(pPatchFile->m_strName, CFile::modeCreate | CFile::modeWrite))
					{
						dwRead = (DWORD)fileNew.SeekToEnd();					
						remotefile->Seek(dwRead, CFile::begin);
						DownloadWrite(pPatchFile);
					}
				}

				if(fileNew.m_hFile == CFile::hFileNull)
				{
					int error = GetLastError();
					m_bDownloading = FALSE;
					break;
				}

				//	파일 다운
				char	lpBuffer[1024+1];
				DWORD	dwNumberOfBytesRead;
				while (dwNumberOfBytesRead = remotefile->Read(lpBuffer, 1024))
				{
					if(!m_bDownloading)
					{
						if(!m_bCancel)
							AfxMessageBox(IDS_FTP_FAIL);
						break;
					}

					fileNew.Write(lpBuffer, dwNumberOfBytesRead);
					dwRead += dwNumberOfBytesRead;
					nTotalRead += dwNumberOfBytesRead;
					Progress(pPatchFile->m_strName, pPatchFile->m_dwSize, dwRead, nTotalRead,PROGRESS_TYPE_DOWNLOAD);
				}

				//	파일닫기
				fileNew.Close();
				remotefile->Close();
				delete remotefile;
				remotefile = NULL;
			}
			CATCH_ALL(error)
			{
				TCHAR	szCause[255];
				CString Cause;
				CString CauseTemp;
				error->GetErrorMessage(szCause,254,NULL);

				if(remotefile)
				{
					remotefile->Close();
					delete remotefile;
					remotefile = NULL;
				}

				if(fileNew.m_hFile != CFile::hFileNull)
					fileNew.Close();
#ifndef _RU
				CauseTemp.LoadString(IDS_DOWNLOAD_RETRY);
#else
				CauseTemp = IDS_DOWNLOAD_RETRY;
#endif
				Cause.Format(CauseTemp, szCause);
				if(AfxMessageBox(Cause, MB_YESNO) == IDYES)
					continue;
				else
					m_bDownloading = FALSE;

			}
			END_CATCH_ALL;
		}

		//	압축해제
		if(m_bDownloading)
		{
			if(bPatchType == DOWN_PREPATCH)
			{
				nTotalRead += pPatchFile->m_dwSize;
				Progress(pPatchFile->m_strName, 1, 1, nTotalRead,PROGRESS_TYPE_DOWNLOAD);
			}

			CString strSource, strTarget;
			strTarget.Empty();

			if(bPatchType > DOWN_NONE)
				strSource = m_strLocal + _T("_BetaPatch\\");
			else
				strSource = m_strDownload + _T("\\");

			if(!pPatchFile->m_strPath.IsEmpty())
			{
				strSource += pPatchFile->m_strPath + _T("\\");
				strTarget = pPatchFile->m_strPath + _T("\\");
			}
			strSource += pPatchFile->m_strName;

			if( UNZ_OK != Unzip( strSource, strTarget))
			{
				m_bDownloading = FALSE;
				AfxMessageBox(IDS_DOWNLOAD_FAIL);
				remove(strSource);
				break;
			}

			////	패치 완료
			if(!m_blPatchFileUpdate)
				SetVersion(pPatchFile->m_dwVersion);

			if(pPatchFile)
			{
				delete (*m_vPatchFile.begin());
				m_vPatchFile.erase(m_vPatchFile.begin());				
				pPatchFile = NULL;
			}

			if(bPatchType > DOWN_NONE)
			{
				delete (*PrePatchItr);
				m_vLocal.erase(PrePatchItr);
				InfoFileWrite();
			}
			else
				DownloadWrite();

			remove(strSource);

			if(m_blPatchFileUpdate)
				break;
		}
	}

	if(fileNew.m_hFile != CFile::hFileNull)
		fileNew.Close();

	if(remotefile)
	{
		remotefile->Close();
		delete remotefile;
		remotefile = NULL;
	}

	DownloadFinish();
	m_bDownloading = FALSE;

	return 0;
}

BYTE CStoryDlg::PrePatchCheak(LPPATCHFILE& pPatchFile, VPATCHFILE::iterator& itr)
{
	//	PrePatch 확인
	if(pPatchFile->m_dwBetaVer != 0)
	{
		for(VPATCHFILE::iterator it = m_vLocal.begin(); it != m_vLocal.end(); it++)
		{
			if((*it)->m_dwBetaVer == pPatchFile->m_dwBetaVer)
			{
				(*it)->m_dwVersion = pPatchFile->m_dwVersion;

				BYTE bRet = DOWN_PREPATCH;

				//	이어받아야 하는가?					
				if(0 == (*it)->m_dwSize)
				{
					bRet = DOWN_INCOMPRE;
					(*it)->m_dwSize	= pPatchFile->m_dwSize;
				}
				pPatchFile = (*it);
				itr = it;
				return bRet;
			}
		}
	}
	return DOWN_NONE;
}

BYTE CStoryDlg::SetPath(LPPATCHFILE pPatchFile, BYTE bPatchType)
{
	CString strPathName;

	if(bPatchType > DOWN_NONE)
		strPathName = m_strLocal + _T("_BetaPatch");
	else
		strPathName = m_strDownload;

	if(!pPatchFile->m_strPath.IsEmpty())
	{
		strPathName += _T("\\") + pPatchFile->m_strPath;
		if(!CreateDirectoryEx(strPathName))
			return FALSE;
	}
	if(!SetCurrentDirectory(strPathName))
		return FALSE;

	return TRUE;
}

void CStoryDlg::DownloadFinish()
{
	Progress(_T(""), 0, 100, 100, PROGRESS_TYPE_DEFAULT);
	if(m_bDownloading)
		CheckPatch();
	else
	{
		if(m_bCancel)
			PostMessage(WM_STOP_DOWNLOAD);
		else
		{
			AfxMessageBox(IDS_FTP_FAIL);
			RemoveDownloadDirectory();
		}
	}

	PostMessage(WM_CLOSE_SESSION);
}

void CStoryDlg::DownloadEnd()
{
	m_bDownloading = FALSE;

	if( m_hDownload )
	{
		WaitForSingleObject(m_hDownload,10000);
		CloseHandle(m_hDownload);
		m_hDownload = NULL;
	}

	for(DWORD i = 0; i < m_vPatchFile.size(); i++)
		delete m_vPatchFile[i];
	m_vPatchFile.clear();

	for(DWORD i = 0; i < m_vLocal.size(); i++)
		delete m_vLocal[i];
	m_vLocal.clear();

	if(m_session.m_hSocket != INVALID_SOCKET)
		m_session.End();
}
void CStoryDlg::RemoveDownloadDirectory()
{
	DeleteDirectory(m_strDownload);

	if(SetCurrentDirectory(m_strLocal))
		_rmdir(m_strDownload);

	if(m_vLocal.size() < 1 || !m_dwMinBetaVer)
	{
		CString strPrePath =  m_strLocal + "_BetaPatch\\";
		DeleteDirectory(strPrePath);

		if(SetCurrentDirectory(m_strLocal))
			_rmdir(strPrePath);
	}

}

void CStoryDlg::RemoveOldPrePatch()
{
	if(!m_vLocal.size())
		return;

	BYTE bDel = FALSE;

	VPATCHFILE::iterator it = m_vLocal.begin();
	while(it < m_vLocal.end())
	{
		if((*it)->m_dwBetaVer < m_dwMinBetaVer)
		{
			remove(m_strLocal + _T("_BetaPatch\\") + (*it)->m_strPath + (*it)->m_strName);
			m_vLocal.erase(it);
			it = m_vLocal.begin();
			bDel = TRUE;
		}
		else
			it++;
	}
	if(bDel)
		InfoFileWrite();
}

void CStoryDlg::DeleteDirectory(LPCTSTR strPath)
{
	WIN32_FIND_DATA FileData;
	if(SetCurrentDirectory(strPath))
	{
		HANDLE hSearch = FindFirstFile(_T("*"), &FileData);
		if( hSearch == INVALID_HANDLE_VALUE) return;

		BYTE bContinue = TRUE;
		while(bContinue)
		{
			if( FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if( 0!=strcmp(FileData.cFileName, ".") && 
					0!=strcmp(FileData.cFileName, "..") )
				{
					CString strFullPath = strPath;

					strFullPath+= _T("\\");
					strFullPath += FileData.cFileName;

					DeleteDirectory(strFullPath);
					if(SetCurrentDirectory(strPath))
						_rmdir(strFullPath);
				}
			}
			else
				DeleteFile(FileData.cFileName);

			if(!FindNextFile(hSearch, &FileData)) bContinue = FALSE;
		}

		FindClose(hSearch);
	}
}

void CStoryDlg::CheckPatch()
{
	if(m_blPatchFileUpdate)
		PostMessage(WM_RESTART_PATCH);
	else
	{
		m_strDownload.ReleaseBuffer();
		CString strNew=m_szPatchFileName;

		// 게임 시작 버튼 활성화
		CButton * pOK = (CButton *)GetDlgItem(IDOK);
		if(pOK)
		{
			m_bFlash = TRUE; // 버튼 깜박임
			pOK->EnableWindow(TRUE);
			SetTimer(1, 500, NULL);
		}

		// 환경설정 버튼 활성화
		CButton * pSet = (CButton *)GetDlgItem(IDC_BUTTON_SETTING);
		if(pSet)
			pSet->EnableWindow(TRUE);

		int n = strNew.Find(BACKUP_EXT);
		if(n > 0)
		{
			strNew.Delete(n,lstrlen(BACKUP_EXT));
			n = strNew.ReverseFind('\\');
			if(n >= 0)
				strNew.Delete(0, n+1);

			CopyFile(m_szPatchFileName, strNew, FALSE);
		}
		//	사용하지 않는 이전버전 PrePatch 지우기
		if(m_dwMinBetaVer)
			RemoveOldPrePatch();

		RemoveDownloadDirectory();

		//	Prepatch 열기
		if(m_chkPrePatch.GetCheck())
		{			
			CWnd* pWnd = FindWindow(NULL, PREPATCH_NAME);
			if(!pWnd)
				ShellExecute(NULL, _T("open"), m_strLocal + _T("PrePatch.exe"), NULL, NULL, NULL);
		}
	}	
}
int CStoryDlg::Unzip(LPCTSTR strZip, LPCTSTR strDirectory)
{
	int err = UNZ_OK;
	unz_global_info zipinfo;

	unzFile zip = unzOpen( strZip );
	if( NULL == zip )
		return -1;

	err = unzGetGlobalInfo( zip, &zipinfo );
	if( UNZ_OK != err)
	{
		unzClose( zip );
		return err;
	}

	DWORD totalcnt = zipinfo.number_entry;

	err =unzGoToFirstFile( zip );
	if( UNZ_OK != err)
	{
		unzClose( zip );
		return err;
	}

	for( DWORD i = 0; i < totalcnt && err == UNZ_OK; i++)
	{
		if( UNZ_OK == err || UNZ_END_OF_LIST_OF_FILE == err )
		{
			TCHAR szFileName[256];
			unz_file_info file_info;

			err = unzGetCurrentFileInfo( zip, &file_info, szFileName, 256, NULL, 0, NULL, 0);
			if( UNZ_OK != err )
			{
				unzClose( zip );
				return err;
			}

			CString strParse = szFileName;
			int nCurPos = 0;
			strParse.Tokenize(_T("."), nCurPos);
			if(nCurPos >= strParse.GetLength())
			{
				err = unzGoToNextFile( zip );
				continue;
			}

			VerifyDirectory(m_strLocal, strDirectory, FALSE);

			CString strTargetDir = m_strLocal + strDirectory;
			if( !SetCurrentDirectory( strTargetDir ))
			{
				unzClose( zip );
				return -1;
			}

			CString strOld, strNew;
			char szPatchFileName[_MAX_PATH], szFileExt[_MAX_PATH];
			_splitpath(m_szPatchFileName, NULL, NULL, szPatchFileName, szFileExt);

			strOld = strTargetDir+szFileName;
			if(!lstrcmpi(strOld, m_szPatchFileName))
			{
				CString strNew = szPatchFileName;
				strNew += BACKUP_EXT;
				strNew += szFileExt;
				lstrcpy(szFileName, strNew);
				strParse = szFileName;
				m_blPatchFileUpdate = TRUE;
			}

			if(strParse.Find(APP_EXT))
				ProssesKillr(szFileName);

			VerifyDirectory(strTargetDir, szFileName, TRUE);

			CFile file;
			if(!file.Open(szFileName, CFile::modeCreate|CFile::shareExclusive|CFile::modeWrite))
			{
				SetFileAttributes( szFileName, FILE_ATTRIBUTE_NORMAL );
				if( !file.Open(szFileName, CFile::modeCreate|CFile::shareExclusive|CFile::modeWrite) )
				{
					unzClose( zip );
					return -1; // 수정
				}
			}

			err = unzOpenCurrentFile( zip );
			if( UNZ_OK != err)
			{
				unzClose( zip);
				file.Close();

				return err;
			}

			DWORD dwSize = file_info.uncompressed_size;
			LPVOID data =  malloc(dwSize);

			if(! data )
			{
				unzClose( zip);
				file.Close();
				return -1;
			}

			err = unzReadCurrentFile( zip, data, dwSize );
			if( err < 0)
			{
				unzCloseCurrentFile( zip );
				unzClose( zip );
				file.Close();
				return err;
			}

			file.Write(data, dwSize);
			file.Flush();

			free(data);

			if( UNZ_CRCERROR == unzCloseCurrentFile( zip ))
			{
				unzClose( zip);
				file.Flush();
				file.Close();
				return UNZ_CRCERROR;
			}
			file.Close();

			if(1 < totalcnt)
				Progress(szFileName, totalcnt, i+1, 0, PROGRESS_TYPE_UNZIP);
		}
		err = unzGoToNextFile( zip );
	}

	return unzClose( zip );
}
BYTE CStoryDlg::SetVersion(DWORD dwVer)
{
	HKEY hKeyRet;
	HKEY hKey = HKEY_LOCAL_MACHINE;

	CString strSubkey;
	strSubkey = m_strSubkey;
	if(strSubkey == _T(""))
		strSubkey = m_strAppName + REG_COUNTRY;
	strSubkey += _T("\\PB");

	m_strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

	int err = RegCreateKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
		return FALSE;

	BYTE	data[1024];
	DWORD   type = REG_DWORD;
	DWORD   cbData = 4;

	memcpy(data, &dwVer, sizeof(DWORD));

	err = RegSetValueEx(hKeyRet, REG_VALUE_VERSION, 0, REG_DWORD, data, cbData);
	if(ERROR_SUCCESS != err)
		return FALSE;

	RegCloseKey(hKeyRet);
	return TRUE;
}
void CStoryDlg::VerifyDirectory(LPCTSTR target, LPCTSTR path, BYTE bHaveFile)
{
	TCHAR parse[256];
	char * tok = NULL;
	CString strDirectory;

	lstrcpy( parse, path );
	tok = strtok((char *)parse, PATH_DELIMETERS);
	if( !tok )
		return;

	strDirectory = target;

	if( bHaveFile )
	{
		do
		{
			if(lstrcmp(tok, _T(".")))
			{
				CreateDirectory(strDirectory, NULL);
				strDirectory += '\\';
				strDirectory += tok;
			}
		}while(NULL!= (tok = strtok(NULL, PATH_DELIMETERS)));
	}
	else
	{	
		CreateDirectory(strDirectory, NULL);										
		do
		{
			if(lstrcmp(tok, _T(".")))
			{	
				strDirectory += '\\';
				strDirectory += tok;
				CreateDirectory(strDirectory, NULL);										
			}
		}while(NULL!= (tok = strtok(NULL, PATH_DELIMETERS)));
	}
}
BYTE CStoryDlg::CreateDirectoryEx(CString strPath)
{
	if( strPath.IsEmpty() ) return TRUE;

	if( !CreateDirectory( strPath, NULL ))
	{
		DWORD dwErr = GetLastError();
		if( dwErr == ERROR_ALREADY_EXISTS ) return TRUE;

		if( dwErr == ERROR_PATH_NOT_FOUND )
		{
			int nPos = 0;
			int nNext = strPath.Find(_T("\\"), 0);
			while(nNext >=0 )
			{
				nPos = nNext;
				nNext = strPath.Find(_T("\\"), nPos+1 );
			}

			CString strCreate = strPath.Left( nPos );
			if( CreateDirectoryEx( strCreate) )
				return CreateDirectory(strPath, NULL );
		}
		return FALSE;
	}

	return TRUE;
}
void CStoryDlg::Progress(CString strName, DWORD dwSize, DWORD dwCurrent, __int64 nTotal, BYTE bType)
{
	if(!m_bDownloading)
		return;

	switch(bType)
	{
	case PROGRESS_TYPE_DEFAULT:
		{
			m_progressCurrent.SetPos( (int)dwCurrent );
			m_progressTotal.SetPos( (int)nTotal );
		}
		break;
	case PROGRESS_TYPE_DOWNLOAD:
		{
			if(m_bProgressingColor)
			{
				::SendMessage(m_progressCurrent, PBM_SETBARCOLOR, 0, (LPARAM)m_Ui.m_rgbCurrent);
				m_bProgressingColor = FALSE;
			}
			int nPos = int(dwSize ? __int64(100) * dwCurrent / dwSize : 0);
			m_progressCurrent.SetPos( nPos );

			nPos = int(100 * nTotal / m_nPatchTotalSize);
			m_progressTotal.SetPos( nPos );
		}
		break;
	case PROGRESS_TYPE_UNZIP:
		{
			if(!m_bProgressingColor)
			{
				::SendMessage(m_progressCurrent, PBM_SETBARCOLOR, 0, (LPARAM)m_Ui.m_rgbUnzip);
				m_bProgressingColor = TRUE;
			}
			int nPos = int(dwSize ? __int64(100) * dwCurrent / dwSize : 0);
			m_progressCurrent.SetPos( nPos );
		}
		break;
	default:
		break;
	}
}

void CStoryDlg::StartGame()
{
	if(SetCurrentDirectory( m_strLocal ))
	{
		STARTUPINFO StartupInfo;
		PROCESS_INFORMATION ProcessInfo;

		ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
		ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION)); 

		StartupInfo.cb = sizeof(STARTUPINFO);
		StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
		StartupInfo.wShowWindow = SW_SHOWNORMAL;

		CString strCommandLine;
		strCommandLine.Format(_T("%s %s %d"), m_strGame, m_strGameSvr, m_wGamePort);
///////////////////
	char strFile[MAX_PATH];
	sprintf( strFile, "%sLogin_kr.txt", m_strLocal);
	FILE* f = fopen( strFile ,"wt");
	fprintf(f,"%s %s", m_strGame, strCommandLine);
	fclose(f);
///////////////////////////////
		if(!CreateProcess(m_strGame, strCommandLine.LockBuffer(), 
			NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
		{
			strCommandLine.ReleaseBuffer();
			return;
		}

		strCommandLine.ReleaseBuffer();
	}
}

DWORD CStoryDlg::OnCT_NEWPATCH_ACK(CPacket * pPacket)
{
	m_nPatchTotalSize = 0;

	SOCKADDR_IN addr;
	WORD wCount;

	(*pPacket)
		>> m_strFtpSvr
		>> addr.sin_addr.s_addr
		>> m_wGamePort
		>> m_dwMinBetaVer
		>> wCount;

	m_strGameSvr = inet_ntoa(addr.sin_addr);

	for(WORD i=0; i<wCount; i++)
	{
		LPPATCHFILE pFile = new PATCHFILE();

		(*pPacket)
			>> pFile->m_dwVersion
			>> pFile->m_strPath
			>> pFile->m_strName
			>> pFile->m_dwSize
			>> pFile->m_dwBetaVer;

		m_nPatchTotalSize += pFile->m_dwSize;
		m_vPatchFile.push_back(pFile);
	}

	SendCT_PATCHSTART_REQ();

	if(m_bFirstPrePatch)
	{
		if( AfxMessageBox(IDS_PREPATCH, MB_YESNO ) == IDYES )
			WriteStartRegistry(TRUE);
		m_chkPrePatch.SetCheck(CheakStartRegistry());
	}

	if(wCount)
	{
		PrePatchQuit();
		Progress(_T(""), 0, 0, 0, PROGRESS_TYPE_DEFAULT);

		DWORD dwThreadID;
		CreateDirectory( m_strDownload , NULL);
		m_hDownload = CreateThread(NULL, 0, _Download, (LPVOID)this, CREATE_SUSPENDED, &dwThreadID);
		if( !m_hDownload )
		{
			AfxMessageBox(IDS_THREAD_FAIL);
			return 1;
		}
		ResumeThread(m_hDownload);
	}
	else
	{
		m_progressCurrent.SetPos(100);
		m_progressTotal.SetPos(100);
		CheckPatch();
		PrePatchQuit(TRUE);
		return 1;
	}
	return 0;
}
void CStoryDlg::SendCT_NEWPATCH_REQ()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_NEWPATCH_REQ)
		<< m_dwVersion;
	Say(pMsg);
}

void CStoryDlg::SendCT_PATCHSTART_REQ()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_PATCHSTART_REQ);
	Say(pMsg);
}

void CStoryDlg::OnBnClickedButtonHomepage()
{
	CString strUrl = m_strHompageURLForLuancher;
	if(strUrl == _T(""))
		strUrl = HOMEPAGE_NAME;

	ShellExecute(NULL, _T("open"), _T("iexplore"), (LPCTSTR)strUrl, NULL, SW_SHOWNORMAL);
}

void CStoryDlg::OnBnClickedButtonSetting()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowSettingFrame();
}

BYTE CStoryDlg::WriteRegistry()
{
	HKEY hKeyRet;
	HKEY hKey = HKEY_CURRENT_USER; //HKEY_LOCAL_MACHINE;

	CString strSubkey;
	strSubkey = m_strSubkey;
	if(strSubkey == _T(""))
		strSubkey = m_strAppName;// + REG_COUNTRY;
	strSubkey += _T("\\Settings"); //_T("\\PB");

	m_strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

	int err = RegCreateKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
		return FALSE;

	char	strBuf[1024];
	BYTE	data[1024];
	DWORD   type = REG_DWORD;
	DWORD   cbData = 4;

	// window mode
	//memcpy(data, &m_dwWindowMode, sizeof(DWORD));
	//err = RegSetValueEx(hKeyRet, REG_VALUE_WINDOW, 0, REG_DWORD, data, cbData);

	if(m_dwWindowMode == 1)
		strcpy(strBuf,"TRUE");
	else
		strcpy(strBuf,"FALSE");

	err = RegSetValueEx(hKeyRet, REG_WINDOW, 0, REG_SZ, (BYTE*)strBuf, (DWORD)strlen(strBuf));

	if(ERROR_SUCCESS != err)
		return FALSE;

	// shader mode
	//memcpy(data, &m_dwShaderMode, sizeof(DWORD));

	if(m_dwShaderMode == 1)
		strcpy(strBuf,"TRUE");
	else
		strcpy(strBuf,"FALSE");

	err = RegSetValueEx(hKeyRet, REG_SHADER, 0, REG_SZ, (BYTE*)strBuf, (DWORD)strlen(strBuf));
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	// character mode // 캐릭터 품질
	memcpy(data, &m_dwCharMode, sizeof(DWORD));
	err = RegSetValueEx(hKeyRet, REG_CHARACTER, 0, REG_DWORD, data, cbData);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	// paper matrix mode // 지형 품질
	memcpy(data, &m_dwPaperMode, sizeof(DWORD));
	err = RegSetValueEx(hKeyRet, REG_MAPDETAIL, 0, REG_DWORD, data, cbData);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	// background mode // 텍스쳐 품질
	memcpy(data, &m_dwBackMode, sizeof(DWORD));
	err = RegSetValueEx(hKeyRet, REG_TEXDETAIL, 0, REG_DWORD, data, cbData);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	RegCloseKey(hKeyRet);

	return TRUE;
}

BYTE CStoryDlg::CheakStartRegistry()
{
	HKEY hKeyRet;	

	int err = RegOpenKey(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\run"), &hKeyRet);
	if(ERROR_SUCCESS != err)
	{
		RegCloseKey(hKeyRet);
		return FALSE;
	}
	err = RegQueryValueEx(hKeyRet, _T("4StoryPrePatch"), NULL, NULL, NULL, NULL);
	if( ERROR_SUCCESS != err)
	{
		RegCloseKey(hKeyRet);
		return FALSE;
	}
	RegCloseKey(hKeyRet);
	return TRUE;
}

BYTE CStoryDlg::WriteStartRegistry(BYTE bCheak)
{
	HKEY hKeyRet;

	int err = RegOpenKey(HKEY_LOCAL_MACHINE,_T("Software\\Microsoft\\Windows\\CurrentVersion\\run"), &hKeyRet);
	if(ERROR_SUCCESS != err)
	{
		RegCloseKey(hKeyRet);
		return FALSE;
	}
	if(bCheak)
	{
		CString strPath = m_strLocal + _T("PrePatch.exe");
		err = RegSetValueEx(hKeyRet, _T("4StoryPrePatch"), 0, REG_SZ, (LPBYTE)strPath.GetBuffer(), (DWORD) (lstrlen(strPath)+1)*sizeof(TCHAR));
		if(ERROR_SUCCESS != err)
		{
			RegCloseKey(hKeyRet);
			return FALSE;
		}
		if(!m_bDownloading && m_chkPrePatch.GetCheck())
		{
			CWnd* pWnd = FindWindow(NULL, PREPATCH_NAME);
			if(!pWnd)
				ShellExecute(NULL, _T("open"), m_strLocal + _T("PrePatch.exe"), NULL, NULL, NULL);
		}
	}
	else
		RegDeleteValue(hKeyRet, _T("4StoryPrePatch"));

	RegCloseKey(hKeyRet);
	return TRUE;
}

void CStoryDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_bFlash)
	{
		m_bFlash = FALSE;
		KillTimer(1);
	}

#ifdef _MODULPROTECTE	
	EndThread();
	m_bHACK = LaunchTClient();
#else
	StartGame();
#endif

	OnOK();
}

void CStoryDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_bFlash)
	{
		m_bFlash = FALSE;
		KillTimer(1);
	}

	if(m_bDownloading)
	{
		m_bCancel = TRUE;
		DownloadEnd();
	}
	else
		DestroyWindow();
}

BOOL CStoryDlg::OnEraseBkgnd(CDC* pDC){return TRUE;}

void CStoryDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CStoryDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		m_bOK.FlashButton();
		m_bFlash++;
		break;
#ifdef _KOR
	case 99:
		m_bSecond++;
		break;
#endif
	}

	if(m_bFlash == 10)
		KillTimer(1);
#ifdef _KOR
	if(m_bSecond == 4)
	{			
		KillTimer(99);
		m_bSecond = 0;
		if(m_webCtrl)			
			::ShowWindow(m_webCtrl.GetSafeHwnd(),SW_SHOW);
	}
#endif
	CDialog::OnTimer(nIDEvent);
}

void CStoryDlg::OnDestroy()
{
#ifdef _MODULPROTECTE
	SetHWND(NULL);
	EndThread();
	HackMSG();	
#endif
	CDialog::OnDestroy();
}

void CStoryDlg::InitCAPS()
{
	IDxDiagProvider *pProvider = NULL;
	DXDIAG_INIT_PARAMS vPARAM;

	if( FAILED(CoCreateInstance(
		CLSID_DxDiagProvider,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDxDiagProvider,
		(LPVOID *) &pProvider)))
		return;

	ZeroMemory( &vPARAM, sizeof(DXDIAG_INIT_PARAMS));
	vPARAM.dwDxDiagHeaderVersion = DXDIAG_DX9_SDK_VERSION;
	vPARAM.bAllowWHQLChecks = TRUE;
	vPARAM.pReserved = NULL;
	vPARAM.dwSize = sizeof(DXDIAG_INIT_PARAMS);

	if(SUCCEEDED(pProvider->Initialize(&vPARAM)))
	{
		IDxDiagContainer *pROOT = NULL;

		if(SUCCEEDED(pProvider->GetRootContainer(&pROOT)))
		{
			IDxDiagContainer *pContainer = NULL;

			if(SUCCEEDED(pROOT->GetChildContainer( L"DxDiag_SystemInfo", &pContainer)))
			{
				VARIANT vVALUE;
				VariantInit(&vVALUE);

				if(SUCCEEDED(pContainer->GetProp( L"ullPhysicalMemory", &vVALUE)))
					m_lSYSMEM = _wtoi64(vVALUE.bstrVal);

				VariantClear(&vVALUE);
				pContainer->Release();
			}

			if(SUCCEEDED(pROOT->GetChildContainer( L"DxDiag_DisplayDevices", &pContainer)))
			{
				DWORD dwCount = 0;

				if( SUCCEEDED(pContainer->GetNumberOfChildContainers(&dwCount)) && dwCount > 0 )
				{
					WCHAR szNAME[MAX_PATH];

					if(SUCCEEDED(pContainer->EnumChildContainerNames( 0, szNAME, MAX_PATH)))
					{
						IDxDiagContainer *pDisplay = NULL;

						if(SUCCEEDED(pContainer->GetChildContainer( szNAME, &pDisplay)))
						{
							VARIANT vVALUE;
							VariantInit(&vVALUE);

							if(SUCCEEDED(pDisplay->GetProp( L"szDisplayMemoryEnglish", &vVALUE)))
								m_lVIDEOMEM = _wtoi64(vVALUE.bstrVal);

							VariantClear(&vVALUE);
							pDisplay->Release();
						}
					}
				}

				pContainer->Release();
			}

			pROOT->Release();
		}
	}

	pProvider->Release();
}

BYTE CStoryDlg::InitDevice()
{
	m_Device.m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if(!m_Device.m_pD3D)
	{
		AfxMessageBox(IDS_ERROR_DXINIT);	
		return FALSE;
	}

	D3DDISPLAYMODE d3ddm;
	BYTE bWindowMode = FALSE;
	if(bWindowMode)
	{
		HRESULT hr = m_Device.m_pD3D->GetAdapterDisplayMode(
			D3DADAPTER_DEFAULT,
			&d3ddm);

		if(FAILED(hr))
		{
			m_Device.m_pD3D->Release();
			m_Device.m_pD3D = NULL;

			return FALSE;
		}
	}
	else
		d3ddm.Format = D3DFMT_X8R8G8B8;

	ZeroMemory( &m_Device.m_vPRESENT, sizeof(m_Device.m_vPRESENT));
	m_Device.m_vPRESENT.BackBufferFormat = d3ddm.Format;

	return TRUE;
}


void CStoryDlg::CollectResolution()
{
	DEVMODE devMode;
	::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
	INT nCurrentFrequently = devMode.dmDisplayFrequency;

	InitDevice();

	if(!m_Device.m_pD3D)
	{
		CString strFMT;
		strFMT.Format("%d X %d",DEFAULT_SCREEN_X,DEFAULT_SCREEN_Y);
		strFMT.Trim(_T(" "));
		theApp.InsertResolution(DEFAULT_SCREEN_X,DEFAULT_SCREEN_Y,strFMT);
		return;
	}

	UINT uCount = m_Device.m_pD3D->GetAdapterModeCount(
		D3DADAPTER_DEFAULT,
		m_Device.m_vPRESENT.BackBufferFormat);

	for( UINT i=0 ; i < uCount ; ++i )
	{
		D3DDISPLAYMODE Mode;

		if( D3D_OK  == m_Device.m_pD3D->EnumAdapterModes(
			D3DADAPTER_DEFAULT,
			m_Device.m_vPRESENT.BackBufferFormat,
			i,
			&Mode) )
		{
			if( Mode.Width >= DEFAULT_SCREEN_X &&
				Mode.Height >= DEFAULT_SCREEN_Y &&
				Mode.Format == m_Device.m_vPRESENT.BackBufferFormat &&
				Mode.RefreshRate == nCurrentFrequently )
			{
				CString strFMT;
				strFMT.Format( "%d X %d ", Mode.Width,Mode.Height );				
				theApp.InsertResolution(Mode.Width,Mode.Height,strFMT);
			}
		}
	}

}

BYTE CStoryDlg::LaunchTClient()
{
	CTModuleProtector vLauncher;
	CString strCommandLine;
	SetCurrentDirectory( m_strLocal );
	strCommandLine.Format(_T("%s %s %d"), m_strGame ,m_strGameSvr, m_wGamePort);

	if(m_bHACK)
		return m_bHACK;

	if(!m_vModuleGuard.CheckValid())
		return m_vModuleGuard.HackDetected() ? TMP_HACK_FOUND : TMP_HACK_TIMEOUT;

	if(!vLauncher.InitProtector(CString(_T(".\\TClientMP.mpc"))))
		return TMP_HACK_INVALID_PE;

	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;

	ZeroMemory( &ProcessInfo, sizeof(PROCESS_INFORMATION)); 
	ZeroMemory( &StartupInfo, sizeof(STARTUPINFO));

	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_SHOWNORMAL;
///////////////////
	char strFile[MAX_PATH];
	sprintf( strFile, "%sLogin.txt", m_strLocal);
	FILE* f = fopen( strFile ,"wt");
	fprintf(f,"%s %s", m_strGame, strCommandLine);
	fclose(f);
///////////////////////////////

	if(!CreateProcess( m_strGame, strCommandLine.LockBuffer(), 
		NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
	{
		strCommandLine.ReleaseBuffer();
		return TMP_HACK_INVALID_PE;
	}
	strCommandLine.ReleaseBuffer();

	return TMP_HACK_NONE;
//	if(m_bHACK)
//		return m_bHACK;
//
//	if(!m_vModuleGuard.CheckValid())
//		return m_vModuleGuard.HackDetected() ? TMP_HACK_FOUND : TMP_HACK_TIMEOUT;
//
//	if(!vLauncher.InitProtector(CString(_T(".\\TClientMP.mpc"))))
//		return TMP_HACK_INVALID_PE;
//#ifdef _KOR
//	vLauncher.UseDynamicIB(FALSE);
//#endif
//
//	return vLauncher.ExecPE( &CString(_T(".\\TClient.exe")), &strCommandLine) ? TMP_HACK_NONE : TMP_HACK_INVALID_PE;
	
}

void CStoryDlg::ReadDisclaimerFile()
{
	CStdioFile file;
	CString strBuf;

	if( file.Open( "disclaimer.txt", CFile::modeRead | CFile::typeText ) )
	{
		m_strDisclaimer = "";

		while(file.ReadString(strBuf))
			m_strDisclaimer += strBuf + _T("\r\n");

		file.Close();
	}
}

BYTE CStoryDlg::SetDisclaimer(BYTE bDisclaimer)
{
	HKEY hKeyRet;
	HKEY hKey = HKEY_LOCAL_MACHINE;

	CString strSubkey;
	strSubkey = m_strSubkey;
	if(strSubkey == _T(""))
		strSubkey = m_strAppName + REG_COUNTRY;
	strSubkey += _T("\\PB");

	m_strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

	int err = RegCreateKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
		return FALSE;

	char	strBuf[1024];
	DWORD   type = REG_DWORD;
	DWORD   cbData = 4;

	if(bDisclaimer == TRUE)
		strcpy(strBuf,"TRUE");
	else
		strcpy(strBuf,"FALSE");

	err = RegSetValueEx(hKeyRet, REG_VALUE_DISCLAIMER, 0, REG_SZ, (BYTE*)strBuf, (DWORD)strlen(strBuf));
	if( ERROR_SUCCESS != err )
		return FALSE;

	RegCloseKey(hKeyRet);
	return TRUE;
}

void CStoryDlg::InfoFileWrite()
{
	char strFile[MAX_PATH];

	sprintf( strFile, "%s_BetaPatch\\PrePatch.btp", m_strLocal);

	FILE* f = fopen( strFile ,"wt");
	if( !f ) return;

	VPATCHFILE::iterator it;
	for(it = m_vLocal.begin(); it != m_vLocal.end(); it++)
	{	
		fprintf(f,"%d\t%s\t%d\t%s\n", (*it)->m_dwBetaVer, (*it)->m_strName, (*it)->m_dwSize, (*it)->m_strPath);
	}
	fclose(f);
}

void CStoryDlg::InfoFileRead()
{
	char strFile[MAX_PATH];
	char strLine[MAX_PATH];

	VPATCHFILE::iterator it;
	for(it = m_vLocal.begin(); it != m_vLocal.end(); it++)
		delete (LPPATCHFILE)*it;

	m_vLocal.clear();

	sprintf( strFile, "%s_BetaPatch\\PrePatch.btp", m_strLocal);

	FILE* f = fopen( strFile ,"rt");
	if( !f ) return;

	while(fgets(strLine, MAX_PATH, f))
	{
		strLine[strlen(strLine)-1] = NULL;
		LPPATCHFILE PatchFile = new PATCHFILE;
		int nIndex = 0;

		PatchFile->m_dwBetaVer	= (DWORD)atoi(strtok(strLine,"\t"));
		PatchFile->m_strName	= strtok(NULL,"\t");
		PatchFile->m_dwSize		= (DWORD)atoi(strtok(NULL,"\t"));
		PatchFile->m_strPath	= strtok(NULL,"\t");
		PatchFile->m_dwVersion	= 0;

		CString strPath = m_strLocal + "_BetaPatch\\" + PatchFile->m_strPath +'\\'+ PatchFile->m_strName;
		if(FindPatchFile(strPath))
			m_vLocal.push_back(PatchFile);
	}
	fclose(f);
}

BYTE CStoryDlg::FindPatchFile(CString strPathName)
{ 
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(strPathName, &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose (hFind);
		return TRUE;
	}
	return FALSE;
}

void CStoryDlg::PrePatchQuit(BYTE bCOM)
{
	CWnd* pWnd = FindWindow(NULL, PREPATCH_NAME);
	if(pWnd)
	{
		if(bCOM)
			::SendMessage(pWnd->m_hWnd, WM_PREPATCH_RRESET, 0, 0);
		else
			::SendMessage(pWnd->m_hWnd, WM_PREPATCH_RQUIT, 0, 0);
	}
}

void CStoryDlg::ShowSettingFrame()
{
	static bInit = TRUE;

	// 게임설정 대화상자 모달리스로 띄우기
	CPlaySetting*	pPlaySetting = CPlaySetting::GetInstance();
	CGameSetting*	pGameSetting = CGameSetting::GetInstance();

	if( bInit )
	{
		bInit = FALSE;

		InitCAPS();

		if( m_lSYSMEM >= 1000000000 && m_lVIDEOMEM >= 256 )
			theApp.m_bOptionLevel = OPTION_HI; // High
		else if( m_lSYSMEM < 1000000000 && m_lVIDEOMEM < 256 )
			theApp.m_bOptionLevel = OPTION_LOW; // Low
		else
			theApp.m_bOptionLevel = OPTION_MID; // Med

		CollectResolution();

		pPlaySetting->Create(IDD_PLAYSET);
		pGameSetting->Create(IDD_SETTING);
	}

	pPlaySetting->ShowWindow(SW_HIDE);
	pGameSetting->ShowWindow(SW_SHOW);
}

BYTE CStoryDlg::CheckIsFirst()
{
	CString strAppName = APP_NAME;
	CString strSubkey;
	CString strRegSubKey;

	strSubkey = strAppName;// + REG_COUNTRY;
	strSubkey += _T("\\Settings");
	strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

	HKEY hKeyRet;
	if( RegOpenKey( HKEY_CURRENT_USER, strRegSubKey, &hKeyRet ) != ERROR_SUCCESS )
	{
		RegCloseKey(hKeyRet);
		return FALSE;
	}

	BYTE	data[1024];
	DWORD   type;
	DWORD   cbdata =1024;
	BYTE	bReturn = TRUE;

	memset(data, 0, 1024);
	int err = RegQueryValueEx(hKeyRet, _T("IsFirst") , NULL, &type, data, &cbdata);
	if( err == ERROR_SUCCESS)
		bReturn = *((LPBYTE)data);

	if( bReturn == FALSE )
	{
		RegCloseKey( hKeyRet );
		return bReturn;
	}

	BOOL	bSetData = FALSE;
	cbdata = sizeof( BOOL );
	memset( data, 0, 1024 );
	memcpy( data, &bSetData, sizeof( BOOL ) );
	err = RegSetValueEx(hKeyRet, "IsFirst", 0, REG_DWORD, data, cbdata);
	if( err != ERROR_SUCCESS  )
	{
		RegCloseKey( hKeyRet );
		return bReturn;
	}

	RegCloseKey(hKeyRet);

	return bReturn;
}

void CStoryDlg::DownloadWrite(LPPATCHFILE pFile)
{
	char strFile[MAX_PATH];

	sprintf( strFile, "%s\\Download.btp", m_strDownload);

	FILE* f = fopen( strFile ,"wt");
	if( !f ) return;

	if(pFile)
		fprintf(f,"%d\t%s\t%s", pFile->m_dwVersion, pFile->m_strName, pFile->m_strPath);
	fclose(f);
}

BYTE CStoryDlg::DownloadRead(LPPATCHFILE pFile)
{
	char strFile[MAX_PATH];
	char strLine[MAX_PATH];
	BYTE bRet = false;

	sprintf( strFile, "%s\\Download.btp", m_strDownload);

	FILE* f = fopen( strFile ,"rt");
	if( !f ) return false;

	if(fgets(strLine, MAX_PATH, f))
	{
		DWORD dwVersion = (DWORD)atoi(strtok(strLine,"\t"));
		CString strName = strtok(NULL,"\t");
		CString strPath = strtok(NULL,"\t");

		if(pFile->m_dwVersion == dwVersion &&
			pFile->m_strName == strName &&
			pFile->m_strPath == strPath)
			bRet = true;
	}
	fclose(f);
	return bRet;
}

////	ModulProtectr

void CStoryDlg::HackMSG()
{
	switch(m_bHACK)
	{
	case TMP_HACK_INVALID_PE	:
		AfxMessageBox(IDS_ERROR_LOAD_FILE);
		break;
	case TMP_HACK_TIMEOUT	:
		AfxMessageBox(IDS_ERROR_TIMEOUT);
		break;

	case TMP_HACK_FOUND		:
		AfxMessageBox(IDS_ERROR_HACK_DETECTED);
		break;
	}
}

BYTE CStoryDlg::BeginProtect()
{
	DWORD dwThreadID;

	InitializeCriticalSection(&m_cs);
	SetRUN(TRUE);

	if( !m_vModuleGuard.InitProtector( MAKEINTRESOURCE(IDR_MPCFILE1), _T("MPCFILE")) ||
		!m_vModuleGuard.BeginWatch() )
	{
		EndProtect();
		m_bRUN = FALSE;

		return FALSE;
	}

	m_hMP = ::CreateThread(
		NULL, 0,
		_MPThread,
		(LPVOID) this,
		0, &dwThreadID);

	if(!m_hMP)
	{
		EndProtect();
		m_bRUN = FALSE;

		return FALSE;
	}

	return TRUE;
}

void CStoryDlg::EndThread()
{
	if(m_hMP)
	{
		SetRUN(FALSE);
		WaitForSingleObject( m_hMP, INFINITE);

		m_hMP = NULL;
	}
}

void CStoryDlg::EndProtect()
{
	EndThread();

	DeleteCriticalSection(&m_cs);
	m_vModuleGuard.ClearModule();
}

DWORD WINAPI CStoryDlg::_MPThread( LPVOID lpParam)
{
	((CStoryDlg *) lpParam)->MPThread();
	return 0;
}

void CStoryDlg::MPThread()
{
	while(GetRUN())
	{
		if(!m_vModuleGuard.CheckValid())
			PostQUIT();

		Sleep(TMP_TIMER);
	}
}

void CStoryDlg::SetRUN( BYTE bRUN)
{
	SMART_LOCKCS(&m_cs);
	m_bRUN = bRUN;
}

BYTE CStoryDlg::GetRUN()
{
	SMART_LOCKCS(&m_cs);
	return m_bRUN ? TRUE : FALSE;
}

void CStoryDlg::SetHWND( HWND hWND)
{
	SMART_LOCKCS(&m_cs);
	m_hWND = hWND;
}

void CStoryDlg::PostQUIT()
{
	SMART_LOCKCS(&m_cs);

	if(!m_bHACK)
		m_bHACK = m_vModuleGuard.HackDetected() ? TMP_HACK_FOUND : TMP_HACK_TIMEOUT;

	if(m_hWND)
		::PostMessage( m_hWND, WM_COMMAND, IDCANCEL, 0);
}

void CStoryDlg::ProssesKillr(CString strProssName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	strProssName.MakeUpper();

	if(hSnapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		BYTE bContinue = Process32First(hSnapshot, &pe32);

		while(bContinue)
		{
			CString strTemp = pe32.szExeFile;
			strTemp.MakeUpper();

			if(!lstrcmpi(strTemp, strProssName))
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pe32.th32ProcessID);
				if(hProcess)
				{
					//DWORD dwCode;
					//GetExitCodeProcess(hProcess, &dwCode);
					TerminateProcess(hProcess, 0);
					WaitForSingleObject(hProcess,INFINITE);
					CloseHandle(hProcess);
				}
			}
			bContinue = Process32Next(hSnapshot, &pe32);
		}
	}
	CloseHandle(hSnapshot);
}

BYTE CStoryDlg::LoadUXInfo()
{	
	char strFile[MAX_PATH];
	char strLine[MAX_PATH];

	sprintf( strFile, "%sIntro\\Launcherui.ini", m_strLocal);

	FILE* f = fopen( strFile ,"rt");
	if( !f ) return false;

	BYTE menu = 0;

	while(fgets(strLine, MAX_PATH, f))
	{
		BYTE offset = 0;
		UIButton* pBt = NULL;
		CString strTemp = strLine;
		strTemp.Remove('\n');	// 리턴제거
		// 주석처리
		int n = strTemp.Find("//");
		if(n > 0)
			strTemp.Delete(n,strTemp.GetLength()-n);
		if(!n || !strTemp.GetLength()) continue;

		//	소문자전환
		strTemp.MakeLower();
		//	공백제거
		strTemp.Trim();

		if(!strTemp.Find('['))
		{
			if(!strTemp.Find("[background]"))
				menu = 1;
			else if(!strTemp.Find("[homepage]"))
				menu = 2;
			else if(!strTemp.Find("[setting]"))
				menu = 3;
			else if(!strTemp.Find("[connect]"))
				menu = 4;
			else if(!strTemp.Find("[close]"))
				menu = 5;
			else if(!strTemp.Find("[prepatch]"))
				menu = 6;
			else if(!strTemp.Find("[currentprogress]"))
				menu = 7;
			else if(!strTemp.Find("[totalprogress]"))
				menu = 8;
			else if(!strTemp.Find("[pageview]"))
				menu = 9;
			else
				menu = 0;
			continue;
		}

		//	선택된 항목이 있는경우
		switch(menu)
		{
		case 1:
			{
				if(!(n = strTemp.Find("img")))
				{
					int iStart = 0;
					strTemp.Tokenize("\"", iStart);
					m_Ui.m_strBG = m_strLocal + "intro\\" + strTemp.Tokenize("\"", iStart);//strtok(NULL,"\"");
					m_Ui.m_Offset = m_Ui.m_Offset | 0x01;
				}
				if(!(n = strTemp.Find("alpha")))
				{
					int iStart = 0;
					strTemp.Delete(0, strTemp.Find('=')+1);
					if(!strTemp.GetLength())
						continue;
					m_Ui.m_rgbAlpha = RGB(
						(WORD)atoi(strTemp.Tokenize(",", iStart)),
						(WORD)atoi(strTemp.Tokenize(",", iStart)),
						(WORD)atoi(strTemp.Tokenize(",", iStart)));
					m_Ui.m_Offset = m_Ui.m_Offset | 0x40;
				}
			}
			break;
		case 2:	pBt = &m_Ui.m_Home;		offset = 0x02; break;
		case 3:	pBt = &m_Ui.m_Set;		offset = 0x04; break;
		case 4:	pBt = &m_Ui.m_Connect;	offset = 0x08; break;
		case 5:	pBt = &m_Ui.m_Close;	offset = 0x10; break;
		case 6:	pBt = &m_Ui.m_PrePatch; offset = 0x20; break;
		case 7:
			{
				if(!strTemp.Find("rect"))
				{
					int iStart = 0;
					strTemp.Delete(0, strTemp.Find('=')+1);
					if(!strTemp.GetLength())
						continue;
					m_Ui.m_rtCurrent.left	= (LONG)atoi(strTemp.Tokenize(",", iStart));
					m_Ui.m_rtCurrent.top	= (LONG)atoi(strTemp.Tokenize(",", iStart));
					m_Ui.m_rtCurrent.right	= (LONG)atoi(strTemp.Tokenize(",", iStart));
					m_Ui.m_rtCurrent.bottom	= (LONG)atoi(strTemp.Tokenize(",", iStart));
				}
				else if(!strTemp.Find("unzipcolor"))
				{
					int iStart = 0;
					strTemp.Delete(0, strTemp.Find('=')+1);
					if(!strTemp.GetLength())
						continue;
					m_Ui.m_rgbUnzip = RGB(
						(WORD)atoi(strTemp.Tokenize(",", iStart)),
						(WORD)atoi(strTemp.Tokenize(",", iStart)),
						(WORD)atoi(strTemp.Tokenize(",", iStart)));
				}
				else if(!strTemp.Find("color"))
				{
					int iStart = 0;
					strTemp.Delete(0, strTemp.Find('=')+1);
					if(!strTemp.GetLength())
						continue;
					m_Ui.m_rgbCurrent = RGB(
						(WORD)atoi(strTemp.Tokenize(",", iStart)),
						(WORD)atoi(strTemp.Tokenize(",", iStart)),
						(WORD)atoi(strTemp.Tokenize(",", iStart)));
				}
			}
			break;
		case 8:
			{
				if(!strTemp.Find("rect"))
				{
					int iStart = 0;
					strTemp.Delete(0, strTemp.Find('=')+1);
					if(!strTemp.GetLength())
						continue;
					m_Ui.m_rtTotal.left	= (LONG)atoi(strTemp.Tokenize(",", iStart));
					m_Ui.m_rtTotal.top	= (LONG)atoi(strTemp.Tokenize(",", iStart));
					m_Ui.m_rtTotal.right	= (LONG)atoi(strTemp.Tokenize(",", iStart));
					m_Ui.m_rtTotal.bottom	= (LONG)atoi(strTemp.Tokenize(",", iStart));
				}
				else if(!strTemp.Find("color"))
				{
					int iStart = 0;
					strTemp.Delete(0, strTemp.Find('=')+1);
					if(!strTemp.GetLength())
						continue;
					m_Ui.m_rgbTotal = RGB(
						(WORD)atoi(strTemp.Tokenize(",", iStart)),
						(WORD)atoi(strTemp.Tokenize(",", iStart)),
						(WORD)atoi(strTemp.Tokenize(",", iStart)));
				}
			}
			break;
		case 9:
			{
				if(!strTemp.Find("rect"))
				{
					int iStart = 0;
					strTemp.Delete(0, strTemp.Find('=')+1);
					if(!strTemp.GetLength())
						continue;
					m_Ui.m_rtView.left	= (LONG)atoi(strTemp.Tokenize(",", iStart));
					m_Ui.m_rtView.top	= (LONG)atoi(strTemp.Tokenize(",", iStart));
					m_Ui.m_rtView.right	= (LONG)atoi(strTemp.Tokenize(",", iStart));
					m_Ui.m_rtView.bottom	= (LONG)atoi(strTemp.Tokenize(",", iStart));
				}
			}
			break;
		}
		//	버튼인경우.
		if(pBt)
		{
			if(!strTemp.Find("normal img"))
			{
				int iStart = 0;
				strTemp.Tokenize("\"", iStart);
				pBt->m_strNormal  = m_strLocal + "intro\\" + strTemp.Tokenize("\"", iStart);
				m_Ui.m_Offset += offset;
			}
			else if(!strTemp.Find("down img"))
			{
				int iStart = 0;
				strTemp.Tokenize("\"", iStart);
				pBt->m_strDown  = m_strLocal + "intro\\" + strTemp.Tokenize("\"", iStart);
			}
			else if(!strTemp.Find("over img"))
			{
				int iStart = 0;
				strTemp.Tokenize("\"", iStart);
				pBt->m_strOver  = m_strLocal + "intro\\" + strTemp.Tokenize("\"", iStart);
			}
			else if(!strTemp.Find("disable img"))
			{
				int iStart = 0;
				strTemp.Tokenize("\"", iStart);
				pBt->m_strDisable  = m_strLocal + "intro\\" + strTemp.Tokenize("\"", iStart);
			}
			else if(!strTemp.Find("pos"))
			{
				int iStart = 0;
				strTemp.Delete(0, strTemp.Find('=')+1);
				if(!strTemp.GetLength())
					continue;
				pBt->x = (int)atoi(strTemp.Tokenize(",", iStart));
				pBt->y = (int)atoi(strTemp.Tokenize(",", iStart));
			}
			else if(!strTemp.Find("tooltip"))
			{
				strtok(strLine,"\"");
				pBt->m_strToolTip = strtok(NULL,"\"");
			}
		}
	}
	fclose(f);
	return true;
}
void CStoryDlg::UXSetting()
{
	if(LoadUXInfo())
	{
		if(m_Ui.m_Offset & 0x01)
			m_bSkin.Load(m_Ui.m_strBG);
		if(m_bSkin.IsNull())
			m_bSkin.LoadFromResource(AfxGetInstanceHandle(),IDB_BITMAP_BASE);
		else if(m_Ui.m_Offset & 0x40)
		{
			SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
			SetLayeredWindowAttributes(m_Ui.m_rgbAlpha, 0, LWA_COLORKEY); 
		}

		sFlag = !m_bSkin.IsNull();

		if(m_Ui.m_Offset & 0x02)
		{
			if(!m_bHome.SetSkin(m_Ui.m_Home.m_strNormal, m_Ui.m_Home.m_strDown, m_Ui.m_Home.m_strOver, m_Ui.m_Home.m_strDisable))
				m_bHome.SetSkin(IDB_BITMAP_HN, IDB_BITMAP_HP, IDB_BITMAP_HH);
		}
		else m_bHome.SetSkin(IDB_BITMAP_HN, IDB_BITMAP_HP, IDB_BITMAP_HH);

		if(m_Ui.m_Offset & 0x04)
		{
			if(!m_bSetting.SetSkin(m_Ui.m_Set.m_strNormal, m_Ui.m_Set.m_strDown, m_Ui.m_Set.m_strOver, m_Ui.m_Set.m_strDisable))
				m_bSetting.SetSkin(IDB_BITMAP_ON, IDB_BITMAP_OP, IDB_BITMAP_OH, IDB_BITMAP_OD);
		}
		else m_bSetting.SetSkin(IDB_BITMAP_ON, IDB_BITMAP_OP, IDB_BITMAP_OH, IDB_BITMAP_OD);

		if(m_Ui.m_Offset & 0x08)
		{
			if(!m_bOK.SetSkin(m_Ui.m_Connect.m_strNormal, m_Ui.m_Connect.m_strDown, m_Ui.m_Connect.m_strOver, m_Ui.m_Connect.m_strDisable))
				m_bOK.SetSkin(IDB_BITMAP_SN, IDB_BITMAP_SP, IDB_BITMAP_SH, IDB_BITMAP_SD);
		}
		else m_bOK.SetSkin(IDB_BITMAP_SN, IDB_BITMAP_SP, IDB_BITMAP_SH, IDB_BITMAP_SD);

		if(m_Ui.m_Offset & 0x10)
		{
			if(!m_bCANCEL.SetSkin(m_Ui.m_Close.m_strNormal, m_Ui.m_Close.m_strDown, m_Ui.m_Close.m_strOver, m_Ui.m_Close.m_strDisable))
				m_bCANCEL.SetSkin(IDB_BITMAP_EN, IDB_BITMAP_EP, IDB_BITMAP_EH);
		}
		else m_bCANCEL.SetSkin(IDB_BITMAP_EN, IDB_BITMAP_EP, IDB_BITMAP_EH);

		if(m_Ui.m_Offset & 0x20)
		{
			if(!m_chkPrePatch.SetSkin(m_Ui.m_PrePatch.m_strNormal, m_Ui.m_PrePatch.m_strDown, m_Ui.m_PrePatch.m_strOver, m_Ui.m_PrePatch.m_strDisable))
				m_chkPrePatch.SetSkin(IDB_BIT_CHECK_NORMAL,IDB_BIT_CHECK_DOWN);
		}
		else m_chkPrePatch.SetSkin(IDB_BIT_CHECK_NORMAL,IDB_BIT_CHECK_DOWN);
	}
	else
	{
		m_bSkin.LoadFromResource(AfxGetInstanceHandle(),IDB_BITMAP_BASE);
		sFlag = !m_bSkin.IsNull();

		m_bOK.SetSkin(IDB_BITMAP_SN, IDB_BITMAP_SP, IDB_BITMAP_SH, IDB_BITMAP_SD);
		m_bCANCEL.SetSkin(IDB_BITMAP_EN, IDB_BITMAP_EP, IDB_BITMAP_EH);
		m_bHome.SetSkin(IDB_BITMAP_HN, IDB_BITMAP_HP, IDB_BITMAP_HH, 0);
		m_bSetting.SetSkin(IDB_BITMAP_ON, IDB_BITMAP_OP, IDB_BITMAP_OH, IDB_BITMAP_OD);
		m_chkPrePatch.SetSkin(IDB_BIT_CHECK_NORMAL,IDB_BIT_CHECK_DOWN);
	}

	if(sFlag)
		SetWindowPos( NULL, 0, 0, m_bSkin.GetWidth(), m_bSkin.GetHeight(), SWP_NOZORDER );

	SetControlPos();

	//	ToolTip
	m_bOK.SetToolTipText(m_Ui.m_Connect.m_strToolTip);
	m_bCANCEL.SetToolTipText(m_Ui.m_Close.m_strToolTip );
	m_bHome.SetToolTipText(m_Ui.m_Home.m_strToolTip);
	m_bSetting.SetToolTipText(m_Ui.m_Set.m_strToolTip);
	m_chkPrePatch.SetToolTipText(m_Ui.m_PrePatch.m_strToolTip);

	// progress bar Color
	::SendMessage(m_progressCurrent, PBM_SETBARCOLOR, 0, (LPARAM)m_Ui.m_rgbCurrent);
	::SendMessage(m_progressTotal, PBM_SETBARCOLOR, 0, (LPARAM)m_Ui.m_rgbTotal);


}