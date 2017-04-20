// TClient.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TClient.h"
#include "TClientWnd.h"
#include "PacketSpyDlg.h"
#include "PacketSpyAcks.h"
#include "TMiniDump.h"

#ifndef TEST_MODE
#include <XTrapArg.h>
#include <HShield.h>
#include <HSUpChk.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD CTClientApp::m_dwAppTick = 0;
CTime CTClientApp::m_dAppDate = CTime(0);
CTime CTClientApp::m_dCurDate = CTime(0);

#ifdef KEYHOOK_CTRLALTDEL
HHOOK CTClientApp::m_hHook = NULL;
bool CTClientApp::m_bKeyDownCAD = FALSE;
#endif

typedef enum TMP_HACK_DETECT_TYPE
{
	TMP_HACK_NONE = 0,
	TMP_HACK_FOUND,
	TMP_HACK_TIMEOUT
} *LPTMP_HACK_DETECT_TYPE;


// 게임가드.
BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
	theApp.m_strNPMSG.Empty();

	CString strTitle = _T("nProtect GameGuard");

	switch (dwMsg)
	{
	case NPGAMEMON_COMM_ERROR:
	case NPGAMEMON_COMM_CLOSE:
		return FALSE;
	case NPGAMEMON_INIT_ERROR:
		theApp.GetMainWnd()->PostMessage( WM_QUIT );
		theApp.m_strNPMSG.Format( CTChart::Format( TSTR_NPGAMEMON_INIT_ERROR ), dwArg );
		MessageBox( NULL, theApp.m_strNPMSG, strTitle, MB_OK );
		return FALSE;
	case NPGAMEMON_SPEEDHACK:
		theApp.GetMainWnd()->PostMessage( WM_QUIT );
		theApp.m_strNPMSG.Format( CTChart::Format( TSTR_NPGAMEMON_SPEEDHACK ) );
		MessageBox( NULL, theApp.m_strNPMSG, strTitle, MB_OK );
		return FALSE;
	case NPGAMEMON_GAMEHACK_KILLED:
		theApp.GetMainWnd()->PostMessage( WM_QUIT );
		theApp.m_strNPMSG.Format( CTChart::Format( TSTR_NPGAMEMON_GAMEHACK ) );
		MessageBox( NULL, theApp.m_strNPMSG, strTitle, MB_OK );
		return FALSE;
	case NPGAMEMON_GAMEHACK_DETECT:
		theApp.GetMainWnd()->PostMessage( WM_QUIT );
		theApp.m_strNPMSG.Format( CTChart::Format( TSTR_NPGAMEMON_GAMEHACK ) );
		MessageBox( NULL, theApp.m_strNPMSG, strTitle, MB_OK );
		return FALSE;
	case NPGAMEMON_GAMEHACK_DOUBT:
		theApp.GetMainWnd()->PostMessage( WM_QUIT );
		theApp.m_strNPMSG.Format( CTChart::Format( TSTR_NPGAMEMON_GAMEHACK_DOUBT ) );
		MessageBox( NULL, theApp.m_strNPMSG, strTitle, MB_OK );
		return FALSE;
	case NPGAMEMON_CHECK_CSAUTH2:
		{
			CTClientGame* pGame = CTClientGame::GetInstance();
			if( !pGame )
				return FALSE;

			memcpy( &pGame->m_NPAuthData, (PVOID)dwArg, sizeof(GG_AUTH_DATA) );

			CPacket vMSG;
			vMSG.SetID(CS_NPROTECT_REQ)
				<< (BYTE)pGame->m_bNPServerID;

			vMSG.AttachBinary( &pGame->m_NPAuthData, sizeof(GG_AUTH_DATA) );

			pGame->SayToAll(vMSG);
		}
		return TRUE;
	}

	return TRUE;
}


#ifdef KEYHOOK_CTRLALTDEL

LRESULT CTClientApp::KeyHookProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if( nCode < 0 )
		CallNextHookEx( m_hHook, nCode, wParam, lParam );

	if( wParam == WM_KEYDOWN )
	{
		PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT) lParam;

		if( pKey->vkCode == VK_DELETE )
		{
			bool bCtrlDown = ( GetAsyncKeyState( VK_LCONTROL ) & 0x8000 || GetAsyncKeyState( VK_RCONTROL ) & 0x8000 );
			bool bAltDown = ( GetAsyncKeyState( VK_LMENU ) & 0x8000 || GetAsyncKeyState( VK_RMENU ) & 0x8000 );
			if( bCtrlDown && bAltDown )
			{
				CTClientGame* pGame = CTClientGame::GetInstance();
				CTClientSession* pSession = pGame->GetSession();
				if( pSession )
				{
					pSession->SendCS_ACDCLOSE_REQ();
					m_bKeyDownCAD = TRUE;
				}
			}
		}
	}

	return CallNextHookEx( m_hHook, nCode, wParam, lParam );
}

#endif


// CTClientApp
BEGIN_MESSAGE_MAP(CTClientApp, CWinApp)
END_MESSAGE_MAP()


// CTClientApp construction

CTClientApp::CTClientApp()
{
	m_hLoadThread = NULL;
	m_hLoadEvent = NULL;
	m_pGDI = NULL;

	m_bFirstRun = TRUE;
	m_bLoadRun = TRUE;
	m_bLoaded = FALSE;

	m_bNeedWorldUnify = FALSE;
	m_bNeedSleep = TRUE;
	m_bWait = TRUE;
	m_bCancelLoad = FALSE;
	m_bForceConnect = FALSE;
	m_pPacketSpy = NULL;
	m_bHackMSG = FALSE;

	m_dwUserID = 0;
	m_dwPrmPCROOM = 0;

	m_strHackMSG[0].Empty();
	m_strHackMSG[1].Empty();
	m_strHackMSG[2].Empty();

	CTMiniDump::SetOption(MiniDumpNormal);
	CTMiniDump::SetSocket(INVALID_SOCKET);

	ClearTLoadCMD();
}

CTClientApp theApp;

BOOL CTClientApp::InitInstance()
{
	static DWORD vVS[VS_COUNT] = {
		IDR_VS_WMESH,		// VS_WMESHVERTEX
		IDR_VS_MESH,		// VS_MESHVERTEX
		IDR_VS_LVERTEX};	// VS_LVERTEX

	static WORD vConstantVS[VC_COUNT] = {
		0,		// VC_COMMON
		397,	// VC_WORLD
		7,		// VC_PROJ
		3,		// VC_TEXTRAN0
		5,		// VC_TEXTRAN1
		139,	// VC_MTRLAMBIENT
		140,	// VC_MTRLDIFFUSE
		2,		// VC_AMBIENT
		1,		// VC_CAMPOS
		0,		// VC_LIGHTCOUNT
		141,	// VC_LIGHTAMBIENT
		269,	// VC_LIGHTDIFFUSE
		11,		// VC_LIGHTDIR
		0};		// VC_SKINNING

	static DWORD vPS[PS_COUNT] = {
		IDR_PS_STAGE1,
		IDR_PS_MODULATE,
		IDR_PS_MODULATE2X,
		IDR_PS_MODULATE4X,
		IDR_PS_ADD,
		IDR_PS_ADDSIGNED,
		IDR_PS_ADDSIGNED2X,
		IDR_PS_SUBTRACT,
		IDR_PS_ADDSMOOTH,
		IDR_PS_BLENDDIFFUSEALPHA,
		IDR_PS_BLENDTEXTUREALPHA,
		IDR_PS_BLENDFACTORALPHA,
		IDR_PS_BLENDTEXTUREALPHAPM,
		IDR_PS_BLENDCURRENTALPHA,
		IDR_PS_MODULATEALPHA_ADDCOLOR,
		IDR_PS_MODULATECOLOR_ADDALPHA,
		IDR_PS_MODULATEINVALPHA_ADDCOLOR,
		IDR_PS_MODULATEINVCOLOR_ADDALPHA,
		IDR_PS_DOTPRODUCT3,
		IDR_PS_MULTIPLYADD,
		IDR_PS_LERP,
		IDR_PS_SHADER,
		IDR_PS_DETAILMAP,
		IDR_PS_MAP};

	static WORD vConstantPS[PC_COUNT] = {
		2,		// PC_COMMON
		1,		// PC_ARG
		0};		// PC_TFACTOR

	GdiplusStartupInput vGDIInput;
	SYSTEM_INFO vINFO;

#ifdef KEYHOOK_CTRLALTDEL
	m_hHook = SetWindowsHookEx( 
		WH_KEYBOARD_LL,
		reinterpret_cast<HOOKPROC>( KeyHookProc ),
		AfxGetInstanceHandle(), NULL );
#endif

	if( !CTChart::InitTString( _T(".\\Tcd\\TString.tcd") ))
	{
		MessageBox( NULL, "TString.tcd not found", CTNationOption::m_strAppTitle, MB_OK);
		return FALSE;
	}

	// Load Chat Filter file.
	if( !CTStrFilter::LoadTStringCF() )
	{
		MessageBox( NULL, _T("TStringCF.tcd not found"), CTNationOption::m_strAppTitle, MB_OK );
		return FALSE;
	}
	
	// Init Chat Filter.
	CTStrFilter::m_sys.SetFilterFile( &CTStrFilter::m_file );

	if( !CTNationOption::SetNation( CTChart::m_strNationCode ) )
	{
		MessageBox( NULL, "Invalid Nation,", CTNationOption::m_strAppTitle, MB_OK);
		return FALSE;
	}

	if( CTNationOption::GERMANY )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_GERMAN, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	// Arab Ver.
	else if ( CTNationOption::EGYPT )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_ARABIC, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if ( CTNationOption::GREECE )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_GREEK, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if ( CTNationOption::CZECH )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_CZECH, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if ( CTNationOption::ITALY )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_ITALIAN, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if ( CTNationOption::SPAIN )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_SPANISH, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if ( CTNationOption::UNITEDKINGDOM || CTNationOption::UNITEDSTATE )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if( CTNationOption::ENGLISH )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if( CTNationOption::JAPAN )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
		TComponent::m_bUseWordBreak = FALSE;
	}
	else if( CTNationOption::POLAND )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_POLISH, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if ( CTNationOption::FRANCE )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_FRENCH, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if ( CTNationOption::TAIWAN )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_CHINESE, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
		TComponent::m_bUseWordBreak = FALSE;
	}
	else if ( CTNationOption::RUSSIA )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if ( CTNationOption::ROMANIA )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_ROMANIAN, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if ( CTNationOption::HUNGARY )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_HUNGARIAN, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if ( CTNationOption::TURKEY )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_TURKISH, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else if ( CTNationOption::PORTUGAL )
	{
		SetThreadLocale( MAKELCID( MAKELANGID(LANG_PORTUGUESE, SUBLANG_DEFAULT) , SORT_DEFAULT ) );
	}
	else	// 국내. 단어단위로 자르다보니 긴 단어 일 때 너무 많이 잘려서 아름답지 않음.
	{
		TComponent::m_bUseWordBreak = FALSE;
	}

#ifdef TEST_MODE
#else
	if( CTNationOption::INSTALL_XTRAP )
	{
		XTrap_C_Start(xArg, ".\\XTrap");
	}
	else if ( CTNationOption::INSTALL_HACKSHIELD )
	{
		if( !HackShield_Update() )
		{
			return FALSE;
		}

		if( !HackShield_Init() )
		{
			HackShield_UnInit();
			return FALSE;
		}
		if ( !HackShield_StartService() )
		{
			HackShield_StopService();
			HackShield_UnInit();
			return FALSE;
		}
	}
#endif

	// 게임가드.
	m_pNpgl = NULL;
	if( CTNationOption::RUSSIA )
	{
		m_pNpgl = new CNPGameLib( "4StoryRU" );
		DWORD dwResult = m_pNpgl->Init();

		if( dwResult != NPGAMEMON_SUCCESS )
		{
			CString strMsg;

			switch (dwResult)
			{
			case NPGAMEMON_ERROR_EXIST:
				strMsg = CTChart::Format( TSTR_NPGAMEMON_ERROR_EXIST );
				break;
			case NPGAMEMON_ERROR_GAME_EXIST:
				strMsg = CTChart::Format( TSTR_NPGAMEMON_ERROR_GAME_EXIST );
				break;
			case NPGAMEMON_ERROR_INIT:
				strMsg = CTChart::Format( TSTR_NPGAMEMON_ERROR_INIT );
				break;
			case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
			case NPGAMEMON_ERROR_NFOUND_GG:
			case NPGAMEMON_ERROR_AUTH_INI:
			case NPGAMEMON_ERROR_NFOUND_INI:
				strMsg = CTChart::Format( TSTR_NPGAMEMON_ERROR_NFOUND );
				break;
			case NPGAMEMON_ERROR_CRYPTOAPI:
				strMsg = CTChart::Format( TSTR_NPGAMEMON_ERROR_CRYPTOAPI );
				break;
			case NPGAMEMON_ERROR_EXECUTE:
				strMsg = CTChart::Format( TSTR_NPGAMEMON_ERROR_EXECUTE );
				break;
			case NPGAMEMON_ERROR_ILLEGAL_PRG:
				strMsg = CTChart::Format( TSTR_NPGAMEMON_ERROR_ILLEGAL_PRG );
				break;
			case NPGMUP_ERROR_ABORT:
				strMsg = CTChart::Format( TSTR_NPGMUP_ERROR_ABORT );
				break;
			case NPGMUP_ERROR_CONNECT:
				strMsg = CTChart::Format( TSTR_NPGMUP_ERROR_CONNECT );
				break;
			case NPGAMEMON_ERROR_GAMEGUARD:
				strMsg = CTChart::Format( TSTR_NPGAMEMON_ERROR_GAMEGUARD );
				break;
			case NPGMUP_ERROR_PARAM:
				strMsg = CTChart::Format( TSTR_NPGMUP_ERROR_PARAM );
				break;
			case NPGMUP_ERROR_INIT:
				strMsg = CTChart::Format( TSTR_NPGMUP_ERROR_INIT );
				break;
			case NPGMUP_ERROR_DOWNCFG:
				strMsg = CTChart::Format( TSTR_NPGMUP_ERROR_DOWNCFG );
				break;
			case NPGMUP_ERROR_AUTH:
				strMsg = CTChart::Format( TSTR_NPGMUP_ERROR_AUTH );
				break;
			case NPGAMEMON_ERROR_NPSCAN:
				strMsg = CTChart::Format( TSTR_NPGAMEMON_ERROR_NPSCAN );
				break;
			case NPGG_ERROR_COLLISION:
				strMsg = CTChart::Format( TSTR_NPGG_ERROR_COLLISION );
				break;
			default:
				// 적절한 종료 메시지 출력
				strMsg = CTChart::Format( TSTR_NPGG_ERROR );
					break;
			}

			CString strCaption;
			strCaption.Format( CTChart::Format( TSTR_NPGG_ERROR_TITLE ), dwResult );
			MessageBox(NULL, strMsg, strCaption, MB_OK);

			return FALSE;
		}
	}

	AfxEnableControlContainer();
	CoInitialize(NULL);

	if( GdiplusStartup( &m_pGDI, &vGDIInput, NULL) != Ok )
		return FALSE;

	if(!AfxSocketInit())
		return FALSE;

#ifdef TEST_MODE
#else
	if ( !CTNationOption::INSTALL_HACKSHIELD && !CTNationOption::INSTALL_GAMEGUARD )
	{
		if( !m_vModuleGuard.InitProtector(CString(_T(".\\TClientMP.mpc"))) ||
			!m_vModuleGuard.CheckImageBase(_T(".\\TClient.exe")) ||
			!m_vModuleGuard.BeginWatch() )
		{
			CString strMSG = CTChart::LoadString(TSTR_ERROR_LOADMPC);

			MessageBox( NULL, strMSG, CTNationOption::m_strAppTitle, MB_OK);

			return FALSE;
		}
	}
#endif

	HANDLE hFile = CreateFile(
		_T(".\\TClient.exe"),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL |
		FILE_FLAG_RANDOM_ACCESS,
		NULL);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		INT64 dlValue = 0;
		DWORD dwRead = 0;
		BYTE bResult = ReadFile(hFile, (LPVOID)&dlValue, sizeof(INT64), &dwRead, NULL);
		while(bResult && dwRead)
		{
			m_dlCheckFile ^= dlValue;
			bResult = ReadFile(hFile, (LPVOID)&dlValue, sizeof(INT64), &dwRead, NULL);
		}
		CloseHandle(hFile);
	}

	m_strHackMSG[0] = CTChart::LoadString(TSTR_ERROR_HACKTOOL_TIMEOUT);
	m_strHackMSG[1] = CTChart::LoadString(TSTR_ERROR_HACKTOOL_FOUND);
	m_strHackMSG[2] = CTChart::LoadString(TSTR_ERROR_HACKTOOL_WARN);

	if( CTNationOption::GERMANY || CTNationOption::FRANCE || CTNationOption::POLAND ||
		CTNationOption::CZECH || CTNationOption::ITALY || CTNationOption::SPAIN ||
		CTNationOption::UNITEDKINGDOM || CTNationOption::GREECE || CTNationOption::ROMANIA ||
		CTNationOption::UNITEDSTATE || CTNationOption::HUNGARY || CTNationOption::TURKEY ||
		CTNationOption::PORTUGAL ||
		// Arab Ver.
		CTNationOption::EGYPT )
		CD3DFont::m_nRenderingHint = TextRenderingHintAntiAlias;

	CTachyonSession::InitTachyonSession();
	CTDynamicBillboard::ClearTACTIVE();
	InitializeCriticalSection(&m_cs);
	GetSystemInfo(&vINFO);

	m_bNeedSleep = vINFO.dwNumberOfProcessors > 1 ? FALSE : TRUE;
	m_hLoadEvent = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);

	SetRegistryKey(_T("Zemi Interactive"));
	m_hAccel = LoadAccelerators(
		m_hInstance,
		MAKEINTRESOURCE(IDR_MAINFRAME));

	m_pTachyonWnd = new CTClientWnd();
	m_pMainWnd = m_pTachyonWnd;
	m_dwAppTick = GetTickCount();

	LoadStdProfileSettings();

	if(!m_pTachyonWnd->Create())
		return FALSE;

#ifndef TEST_MODE
	if( CTNationOption::INSTALL_XTRAP )
		XTrap_C_KeepAlive();
#endif

	m_pTachyonWnd->SetWindowText( CTNationOption::m_strAppTitle );
	m_pTachyonWnd->SetIcon( LoadIcon(IDR_MAINFRAME), TRUE);

#ifdef USE_PACKETSPY
	if( !m_pPacketSpy )
	{
		AfxInitRichEdit2();

		m_pPacketSpy = new CPacketSpyDlg(m_pTachyonWnd);
		CPacketSpyAcks::RegAllAcks();
	}
#endif

	memcpy( m_pTachyonWnd->m_Device.m_vConstantVS, vConstantVS, VC_COUNT * sizeof(WORD));
	memcpy( m_pTachyonWnd->m_Device.m_dwVertexShader, vVS, VS_COUNT * sizeof(DWORD));

	memcpy( m_pTachyonWnd->m_Device.m_vConstantPS, vConstantPS, PC_COUNT * sizeof(WORD));
	memcpy( m_pTachyonWnd->m_Device.m_dwPixelShader, vPS, PS_COUNT * sizeof(DWORD));

	m_pTachyonWnd->m_Device.m_strResourceType = "SHADER";
	if(!m_pTachyonWnd->InitDevices())
	{
		CString strTITLE;
		CString strMSG;

		strMSG = CTChart::LoadString( TSTR_ERR_FAILINITDEVICE);
		m_pMainWnd->GetWindowText(strTITLE);
		m_pMainWnd->ShowWindow(SW_HIDE);

		MessageBox( NULL, strMSG, strTITLE, MB_OK);
		return FALSE;
	}

	// 게임가드.
	if( CTNationOption::RUSSIA )
	{
		if( CheckD3DDevice( (LPVOID)m_pTachyonWnd->m_Device.m_pDevice, "d3d9.dll" ) )
		{
			DWORD dwErrorCode = GetLastError();
			CString strMsg;
			CString strTitle;

			strMsg.Format( "Error Code : %d", dwErrorCode );
			strTitle = "Error";

			MessageBox( NULL, strMsg, strTitle, MB_OK );
			
			return FALSE;
		}
	}

	m_pTachyonWnd->AdjustWindowSize(
		m_pTachyonWnd->m_Device.m_option.m_dwScreenX,
		m_pTachyonWnd->m_Device.m_option.m_dwScreenY );

	CTMiniDump::SetHWND(AfxGetMainWnd()->GetSafeHwnd());

	if(m_bFirstRun)
		((CTClientWnd *) m_pTachyonWnd)->ResetTOption();

	CString strCmdForceCon;
	strCmdForceCon = CTChart::LoadString( TSTR_STARTCMD_FORCECON);

	CString strCmdLine(m_lpCmdLine);
	if( strCmdLine.Replace(strCmdForceCon, "") > 0 )
		m_bForceConnect = TRUE;

	m_pTachyonWnd->m_strIPAddr = m_lpCmdLine;
	if( m_pTachyonWnd->m_strIPAddr != _T("") )
	{
		if( CTNationOption::MODIFY_DIRECTLOGIN )
		{
			int n = 0;

			m_pTachyonWnd->m_strIPAddr = strCmdLine.Tokenize( _T(" "), n );

			if( n == -1 )
				m_pTachyonWnd->m_dwPort = (DWORD)(-1);
			else
			{
				CString strPort = strCmdLine.Tokenize( _T(" "), n );
				m_pTachyonWnd->m_dwPort = atoi( strPort );
			}

			if( n == -1 )
				m_pTachyonWnd->m_strID.Empty();
			else
				m_pTachyonWnd->m_strID = strCmdLine.Tokenize( _T(" "), n );

			if( n == -1 )
				m_pTachyonWnd->m_strPW.Empty();
			else
				m_pTachyonWnd->m_strPW = strCmdLine.Tokenize( _T(" "), n );

			if( n == -1 )
				m_pTachyonWnd->m_bSiteCode = BYTE(-1);
			else
			{
				CString strSiteCode = strCmdLine.Tokenize( _T(" "), n );
				m_pTachyonWnd->m_bSiteCode = atoi( strSiteCode );
			}

			m_pTachyonWnd->m_bDirectLogin =
				!m_pTachyonWnd->m_strID.IsEmpty() &&
				!m_pTachyonWnd->m_strPW.IsEmpty() &&
				m_pTachyonWnd->m_bSiteCode != (BYTE)(-1);

			((CTClientWnd *) m_pTachyonWnd)->m_bChannel = BYTE(1);
		}
		else
		{
			DWORD dwChannel = 1;

			sscanf( m_lpCmdLine, "%s %d %d",
				LPCTSTR(m_pTachyonWnd->m_strIPAddr),
				&m_pTachyonWnd->m_dwPort,
				&dwChannel);
			((CTClientWnd *) m_pTachyonWnd)->m_bChannel = BYTE(dwChannel);
		}
	}

	//if( CTNationOption::KOR )
	//	CCurse::Load(NULL, IDR_TEXT_CURSE, "TEXT");
	//else if( CTNationOption::ENGLISH )
	//	CCurse::Load(NULL, IDR_TEXT_CURSE_GSP, "TEXT");

	EnterCriticalSection(&m_cs);
	if(!CreateLoadThread())
	{
		LeaveCriticalSection(&m_cs);
		return FALSE;
	}
	LeaveCriticalSection(&m_cs);

	TRY
	{
		LPTLOADCMD pTCMD = new TLOADCMD;

		pTCMD->m_dwTCMD = TLOADCMD_RES;
		pTCMD->m_pTBUF = NULL;

		SMART_LOCKCS(&m_cs);
		m_pTachyonWnd->m_bActivate = FALSE;
		m_qTLOADCMD.push(pTCMD);
		SetEvent(m_hLoadEvent);
	}
	CATCH(CFileException, fe)
	{
		EnterCriticalSection(&m_cs);
		ExitLoadThread(TRUE);
		LeaveCriticalSection(&m_cs);

		CString msg;
		msg = CTChart::Format( TSTR_ERROR_FILE, fe->m_strFileName);
		::MessageBox(NULL, msg, "ERROR", MB_OK|MB_ICONERROR);
		::PostQuitMessage(0);

		return FALSE;
	}
	AND_CATCH(CMemoryException, me)
	{
		EnterCriticalSection(&m_cs);
		ExitLoadThread(TRUE);
		LeaveCriticalSection(&m_cs);

		CString msg;
		msg = CTChart::LoadString( TSTR_ERROR_MEMORY);
		::MessageBox( NULL, msg, "ERROR", MB_OK|MB_ICONERROR);
		::PostQuitMessage(0);

		return FALSE;
	}
	END_CATCH
	
	if( m_pTachyonWnd->m_Device.m_option.m_bWindowedMode)
		m_pTachyonWnd->ShowWindow(SW_SHOW);
	else
		m_pTachyonWnd->ShowWindow(SW_MAXIMIZE);

	m_pTachyonWnd->UpdateWindow();
	m_pTachyonWnd->SetFocus();

	return TRUE;
}

int CTClientApp::ExitInstance()
{
#ifdef KEYHOOK_CTRLALTDEL
	UnhookWindowsHookEx( m_hHook );
#endif

	m_TNICEventSink.ReleaseTEVENT();

	if(m_pPacketSpy)
		delete m_pPacketSpy;

	if(m_bLoaded)
		CTachyonSession::ReleaseTachyonSession();

	DeleteCriticalSection(&m_cs);
	CloseHandle(m_hLoadEvent);

	GdiplusShutdown(m_pGDI);
	CoUninitialize();

	CString strURL;
	if( m_bNeedWorldUnify )
		strURL = CTChart::Format( TSTR_WORLDUNIFY_URL);
	else if( m_dwPrmPCROOM && m_dwUserID )
		strURL = CTChart::Format( TSTR_EXE_PRMPC_URL, m_dwPrmPCROOM, m_dwUserID);

	if( !strURL.IsEmpty() )
	{
		FILE* pFile = ::fopen(".\\_event.url", "wt");
		if( pFile )
		{
			::fprintf(pFile, strURL);
			::fclose(pFile);

			::ShellExecute(NULL, "open", ".\\_event.url", NULL, NULL, SW_MAXIMIZE);
			::DeleteFile(".\\_event.url");
		}
	}

	TRY
	{
		CStdioFile file;
		if( file.Open( _T(".\\Data\\Event.loc"), CFile::modeRead|CFile::typeText) )
		{
			CString strCmd;
			strCmd.Empty();

			file.ReadString(strCmd);

			strCmd.TrimRight();
			strCmd.TrimLeft();

			if(!strCmd.IsEmpty())
			{
				PROCESS_INFORMATION vPROCESS;
				STARTUPINFO vSTART;

				ZeroMemory( &vPROCESS, sizeof(PROCESS_INFORMATION)); 
				ZeroMemory( &vSTART, sizeof(STARTUPINFO));

				vSTART.cb = sizeof(STARTUPINFO);
				vSTART.dwFlags = STARTF_USESHOWWINDOW;
				vSTART.wShowWindow = SW_SHOWMAXIMIZED;

				CreateProcess(
					NULL,
					(LPSTR) LPCTSTR(strCmd),
					NULL,
					NULL,
					FALSE,
					0, NULL,
					NULL,
					&vSTART,
					&vPROCESS);
			}
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL
#ifdef TEST_MODE
#else
	m_vModuleGuard.ClearModule();

	if ( CTNationOption::INSTALL_HACKSHIELD )
	{
		CTClientApp* pTAPP = (CTClientApp*) AfxGetApp();
		pTAPP->HackShield_StopService();
		pTAPP->HackShield_UnInit();
	}

#endif
	CTChart::ReleaseTString();

	// 게임가드.
	if( CTNationOption::RUSSIA && m_pNpgl )
	{
		delete m_pNpgl;
		m_pNpgl = NULL;
	}

	return CWinApp::ExitInstance();
}

BYTE CTClientApp::CreateLoadThread()
{
	DWORD dwThread = 0;

	if(m_hLoadThread)
		return FALSE;

	m_hLoadThread = ::CreateThread(
		NULL, 0,
		_LoadThread,
		(LPVOID) m_pTachyonWnd,
		CREATE_SUSPENDED,
		&dwThread);

	if(!m_hLoadThread)
		return FALSE;

	if(!::SetThreadPriority( m_hLoadThread, THREAD_PRIORITY_BELOW_NORMAL))
		return FALSE;

	if( ::ResumeThread(m_hLoadThread) < 0 )
		return FALSE;

	LeaveCriticalSection(&m_cs);
	EnterCriticalSection(&m_cs);

	return TRUE;
}

void CTClientApp::ExitLoadThread( BYTE bCancelCMD)
{
	if(!m_hLoadThread)
		return;

	if(bCancelCMD)
		ClearTLoadCMD();

	m_bCancelLoad = bCancelCMD;
	m_bLoadRun = FALSE;
	LeaveCriticalSection(&m_cs);

	::SetThreadPriority( m_hLoadThread, THREAD_PRIORITY_NORMAL);
	SetEvent(m_hLoadEvent);
	WaitForSingleObject(
		m_hLoadThread,
		INFINITE);

	EnterCriticalSection(&m_cs);
	m_hLoadThread = NULL;

	m_bCancelLoad = FALSE;
	m_bLoadRun = TRUE;
}

#define REG_SETTINGS_RESET_IDENTITY		(0x00000001)		// 기존 값을 다른 값으로 세팅하면 디폴트로 리셋된다. ( 0을 제외한 값 )

void CTClientApp::LoadStdProfileSettings()
{
	CTachyonApp::LoadStdProfileSettings();

	m_pTachyonWnd->m_Device.m_option.m_dwBehavior |= D3DCREATE_MULTITHREADED;
	m_pTachyonWnd->m_Device.m_option.m_bUseSHADER = FALSE;

	CTKeySetting::GetInstance()->LoadRegistry(TREGISTRY_KEY);

	((CTClientWnd *) m_pTachyonWnd)->m_MainGame.m_vKEY.ResetSTATE();

	INT nReset = GetProfileInt(
		_T("Settings"),
		_T("Reset"),
		0);

	if(nReset != REG_SETTINGS_RESET_IDENTITY)
	{
		WriteProfileInt(
			_T("Settings"),
			_T("Reset"),
			REG_SETTINGS_RESET_IDENTITY);

		return ;
	}

	CTClientGame::m_vTOPTION.m_bMapDETAIL = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("MapDETAIL"),
		CTClientGame::m_vTOPTION.m_bMapDETAIL);

	CTClientGame::m_vTOPTION.m_bObjDETAIL = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("ObjDETAIL"),
		CTClientGame::m_vTOPTION.m_bObjDETAIL);

	CTClientGame::m_vTOPTION.m_bMapSHADOW = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("MapSHADOW"),
		CTClientGame::m_vTOPTION.m_bMapSHADOW);

	CTClientGame::m_vTOPTION.m_bObjSHADOW = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("ObjSHADOW"),
		CTClientGame::m_vTOPTION.m_bObjSHADOW);

	CTClientGame::m_vTOPTION.m_bMapSFX = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("MapSFX"),
		CTClientGame::m_vTOPTION.m_bMapSFX);

	CTachyonSFX::m_bOptionLodLevel = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("SfxDETAIL"),
		CTachyonSFX::m_bOptionLodLevel);

	CTClientGame::m_vTOPTION.m_bNpcNAME = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("NpcNAME"),
		CTClientGame::m_vTOPTION.m_bNpcNAME);

	CTClientGame::m_vTOPTION.m_bMonNAME = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("MonNAME"),
		CTClientGame::m_vTOPTION.m_bMonNAME);

	CTClientGame::m_vTOPTION.m_bPcNAME = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("PcNAME"),
		CTClientGame::m_vTOPTION.m_bPcNAME);

	CTachyonMedia::m_bMasterVolume = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("MainVolume"),
		CTachyonMedia::m_bMasterVolume);

	CT3DMusic::m_bMasterVolume = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("BGMVolume"),
		CT3DMusic::m_bMasterVolume);

	CD3DSound::m_bMasterVolume = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("SFXVolume"),
		CD3DSound::m_bMasterVolume);

	CTClientGame::m_vTOPTION.m_bDungeonLIGHTMAP = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("DLIGHTMAP"),
		CTClientGame::m_vTOPTION.m_bDungeonLIGHTMAP);

	CTClientGame::m_vTOPTION.m_bFieldLIGHTMAP = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("FLIGHTMAP"),
		CTClientGame::m_vTOPTION.m_bFieldLIGHTMAP);

	CTClientGame::m_vTOPTION.m_bFarIMAGE = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("FarIMAGE"),
		CTClientGame::m_vTOPTION.m_bFarIMAGE);

	CTClientGame::m_vTOPTION.m_bAutoHelp = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("AUTOHELP"),
		CTClientGame::m_vTOPTION.m_bAutoHelp);

	CTClientGame::m_vTOPTION.m_bTalkBox = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("TALKBOX"),
		CTClientGame::m_vTOPTION.m_bTalkBox);

	CTClientGame::m_vTOPTION.m_bDenyWhisper = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("DENYWHI"),
		CTClientGame::m_vTOPTION.m_bDenyWhisper);

	CTClientGame::m_vTOPTION.m_bDenyCommunity = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("DENYCOM"),
		CTClientGame::m_vTOPTION.m_bDenyCommunity);

	CTClientGame::m_vTOPTION.m_bContinualChat = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("CONCHAT"),
		CTClientGame::m_vTOPTION.m_bContinualChat);

	CTClientGame::m_vTOPTION.m_bMouseClickMove = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("MOUSECLICKMOVE"),
		CTClientGame::m_vTOPTION.m_bMouseClickMove);

	CTClientGame::m_vTOPTION.m_bAutoTargeting = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("AUTOTARGETING"),
		CTClientGame::m_vTOPTION.m_bAutoTargeting);

	CTClientGame::m_vTOPTION.m_bShowCaution15 = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("SHOW_CAUTION_15"),
		CTClientGame::m_vTOPTION.m_bShowCaution15);

	CTClientGame::m_vTOPTION.m_bHUD = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("HUD"),
		CTClientGame::m_vTOPTION.m_bHUD);

	CTachyonMedia::m_bON = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("MASTER"),
		CTachyonMedia::m_bON);

	CD3DSound::m_bON = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("SOUND"),
		CD3DSound::m_bON);

	CT3DMusic::m_bON = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("BGM"),
		CT3DMusic::m_bON);

	CTClientGame::m_vTOPTION.m_fOBJRange = FLOAT(atof(GetProfileString(
		_T("Settings"),
		_T("OBJRange"),
		_T("1.0"))));

	CString strFont = GetProfileString(
		_T("Settings"),
		_T("Font"),
		_T(""));
	if( !strFont.IsEmpty() )
	{
		SetForceTachyonFont(strFont);
		// New Arab Ver.
		CTachyonWnd::m_strFont = strFont;
	}
	
	INT nFontSizeRate = GetProfileInt(
		_T("Settings"),
		_T("FontSize"),
		100);
	SetTachyonFontSizeRate(nFontSizeRate);

	m_bFirstRun = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("FIRST"),
		TRUE);

	BYTE bFontOutline = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("FontOutline"),
		TRUE);

	SetTachyonFontOutline( bFontOutline );

	BYTE bFontShadow = (BYTE) GetProfileInt(
		_T("Settings"),
		_T("FontShadow"),
		TRUE);

	SetTachyonFontShadow( bFontShadow );

	if( CTNationOption::GERMANY || CTNationOption::FRANCE || CTNationOption::POLAND || CTNationOption::CZECH || 
		CTNationOption::ITALY || CTNationOption::SPAIN || CTNationOption::UNITEDKINGDOM ||
		CTNationOption::GREECE || CTNationOption::RUSSIA || CTNationOption::ROMANIA || CTNationOption::EGYPT ||
		CTNationOption::UNITEDSTATE || CTNationOption::HUNGARY || CTNationOption::TURKEY ||
		CTNationOption::PORTUGAL )
	{
		INT nFontRender = GetProfileInt(
			_T("Settings"),
			_T("FontRender"),
			4);
		SetTachyonFontRender(nFontRender);
		CD3DFont::m_nRenderingHint = nFontRender;
	}
	else
	{
		CString strFontQuality = GetProfileString(
			_T("Settings"),
			_T("FontQuality"),
			_T("ANTIALIASED_QUALITY") );
		SetTachyonFontQuality( strFontQuality );
	}

	if(GetProfileInt( _T("Settings"), _T("NullRes"), 0))
		CTachyonRes::m_bUseNullTex = TRUE;
}

void CTClientApp::SaveStdProfileSettings()
{
	CTachyonApp::SaveStdProfileSettings();

	WriteProfileInt(
		_T("Settings"),
		_T("MapDETAIL"),
		CTClientGame::m_vTOPTION.m_bMapDETAIL);

	WriteProfileInt(
		_T("Settings"),
		_T("ObjDETAIL"),
		CTClientGame::m_vTOPTION.m_bObjDETAIL);

	WriteProfileInt(
		_T("Settings"),
		_T("MapSHADOW"),
		CTClientGame::m_vTOPTION.m_bMapSHADOW);

	WriteProfileInt(
		_T("Settings"),
		_T("ObjSHADOW"),
		CTClientGame::m_vTOPTION.m_bObjSHADOW);

	WriteProfileInt(
		_T("Settings"),
		_T("MapSFX"),
		CTClientGame::m_vTOPTION.m_bMapSFX);

	WriteProfileInt(
		_T("Settings"),
		_T("SfxDETAIL"),
		CTachyonSFX::m_bOptionLodLevel);

	WriteProfileInt(
		_T("Settings"),
		_T("NpcNAME"),
		CTClientGame::m_vTOPTION.m_bNpcNAME);

	WriteProfileInt(
		_T("Settings"),
		_T("MonNAME"),
		CTClientGame::m_vTOPTION.m_bMonNAME);

	WriteProfileInt(
		_T("Settings"),
		_T("PcNAME"),
		CTClientGame::m_vTOPTION.m_bPcNAME);

	WriteProfileInt(
		_T("Settings"),
		_T("MainVolume"),
		CTachyonMedia::m_bMasterVolume);

	WriteProfileInt(
		_T("Settings"),
		_T("BGMVolume"),
		CT3DMusic::m_bMasterVolume);

	WriteProfileInt(
		_T("Settings"),
		_T("SFXVolume"),
		CD3DSound::m_bMasterVolume);

	WriteProfileInt(
		_T("Settings"),
		_T("DLIGHTMAP"),
		CTClientGame::m_vTOPTION.m_bDungeonLIGHTMAP);

	WriteProfileInt(
		_T("Settings"),
		_T("FLIGHTMAP"),
		CTClientGame::m_vTOPTION.m_bFieldLIGHTMAP);

	WriteProfileInt(
		_T("Settings"),
		_T("FarIMAGE"),
		CTClientGame::m_vTOPTION.m_bFarIMAGE);

	WriteProfileInt(
		_T("Settings"),
		_T("AUTOHELP"),
		CTClientGame::m_vTOPTION.m_bAutoHelp);

	WriteProfileInt(
		_T("Settings"),
		_T("TALKBOX"),
		CTClientGame::m_vTOPTION.m_bTalkBox);

	WriteProfileInt(
		_T("Settings"),
		_T("DENYWHI"),
		CTClientGame::m_vTOPTION.m_bDenyWhisper);

	WriteProfileInt(
		_T("Settings"),
		_T("DENYCOM"),
		CTClientGame::m_vTOPTION.m_bDenyCommunity);

	WriteProfileInt(
		_T("Settings"),
		_T("CONCHAT"),
		CTClientGame::m_vTOPTION.m_bContinualChat);

	WriteProfileInt(
		_T("Settings"),
		_T("MOUSECLICKMOVE"),
		CTClientGame::m_vTOPTION.m_bMouseClickMove);

	WriteProfileInt(
		_T("Settings"),
		_T("AUTOTARGETING"),
		CTClientGame::m_vTOPTION.m_bAutoTargeting);
	
	WriteProfileInt(
		_T("Settings"),
		_T("SHOW_CAUTION_15"),
		CTClientGame::m_vTOPTION.m_bShowCaution15);

	WriteProfileInt(
		_T("Settings"),
		_T("HUD"),
		CTClientGame::m_vTOPTION.m_bHUD);

	WriteProfileInt(
		_T("Settings"),
		_T("MASTER"),
		CTachyonMedia::m_bON);

	WriteProfileInt(
		_T("Settings"),
		_T("SOUND"),
		CD3DSound::m_bON);

	WriteProfileInt(
		_T("Settings"),
		_T("BGM"),
		CT3DMusic::m_bON);

	WriteProfileInt(
		_T("Settings"),
		_T("FIRST"),
		FALSE);

	CString strValue;
	strValue.Format( _T("%f"), CTClientGame::m_vTOPTION.m_fOBJRange);

	WriteProfileString(
		_T("Settings"),
		_T("OBJRange"),
		strValue);
}

void CTClientApp::ToggleImeLocalMode()
{
	HWND hWnd = ::AfxGetMainWnd()->GetSafeHwnd();
	if(!hWnd)
		return;

	HIMC hIMC = ::ImmGetContext(hWnd);
    DWORD dwConv;
	DWORD dwSent;
	DWORD dwTemp;

	::ImmGetConversionStatus(hIMC, &dwConv, &dwSent);
	if(dwConv & IME_CMODE_NATIVE)
	{
		dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
	    dwConv = dwTemp;
	}
	else
	{
		dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
		dwTemp |= IME_CMODE_NATIVE;
		dwConv = dwTemp;
	}

	::ImmSetConversionStatus( hIMC, dwConv, dwSent);
	::ImmReleaseContext( hWnd, hIMC);
}

BOOL CTClientApp::IsImeLocalMode()
{
	HWND hWnd = ::AfxGetMainWnd()->GetSafeHwnd();
	if(!hWnd)
		return FALSE;

	HIMC hIMC = ::ImmGetContext(hWnd);
    DWORD dwConv;
	DWORD dwSent;

	::ImmGetConversionStatus( hIMC, &dwConv, &dwSent);
	BOOL bResult = ( dwConv & IME_CMODE_NATIVE);
	::ImmReleaseContext(hWnd, hIMC);

	return bResult;
}

DWORD WINAPI CTClientApp::_LoadThread( LPVOID lpParam)
{
	CTClientWnd *pWnd = (CTClientWnd *) lpParam;
	return pWnd->LoadThread();
}

BYTE CTClientApp::MainProc()
{
	static DWORD dwServiceTick = ((CTClientWnd *) m_pTachyonWnd)->GetTClientTickCount();
	DWORD dwServiceCurTick = ((CTClientWnd *) m_pTachyonWnd)->GetTClientTickCount();
	if( dwServiceCurTick - dwServiceTick > TSERVICECHECK_INTERVAL )
	{
		m_TNICEventSink.UpdateService();

		dwServiceTick = ((CTClientWnd *) m_pTachyonWnd)->GetTClientTickCount();
	}

	static DWORD dwMSGTick = 0;
	MSG msg;

	if( dwMSGTick < TTICK_INTERVAL && PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE) )
	{
		DWORD dwPREV = ((CTClientWnd *) m_pTachyonWnd)->GetTClientTickCount();

		if( WM_QUIT == msg.message )
		{
			switch(m_bHackMSG)
			{
			case TMP_HACK_TIMEOUT	:
				{
					m_pTachyonWnd->ShowWindow(SW_HIDE);
					m_pTachyonWnd->MessageBox( m_strHackMSG[0], CTNationOption::m_strAppTitle, MB_OK);
					ExitLoadThread(TRUE);
				}

				break;

			case TMP_HACK_FOUND		:
				{
					m_pTachyonWnd->ShowWindow(SW_HIDE);
					m_pTachyonWnd->MessageBox( m_strHackMSG[1], CTNationOption::m_strAppTitle, MB_OK);
					m_pTachyonWnd->MessageBox( m_strHackMSG[2], CTNationOption::m_strAppTitle, MB_OK);
					ExitLoadThread(TRUE);
				}

				break;
			}

			SaveStdProfileSettings();
			m_pTachyonWnd->DestroyWindow();

			delete m_pTachyonWnd;
			m_pTachyonWnd = NULL;

			return FALSE;
		}

		int nHotCommand = TranslateAccelerator(
			m_pTachyonWnd->GetSafeHwnd(),
			m_hAccel,
			&msg);

		if(nHotCommand)
			m_pTachyonWnd->ProcessHotCommand(msg.wParam);
		else
		{
			switch(msg.message)
			{
			case WM_SESSION_MSG		:
				{
					CTachyonSession *pSession = CTachyonSession::GetSession(msg.wParam);

					if(pSession)
					{
						int nError = WSAGETSELECTERROR(msg.lParam);

						switch(WSAGETSELECTEVENT(msg.lParam))
						{
						case FD_CONNECT	: pSession->OnConnect(nError); break;
						case FD_READ	: pSession->OnReceive(nError); break;
						case FD_CLOSE	: pSession->OnClose(nError); break;
						}
					}
				}

				break;

			case WM_CHAR			:
				if( !msg.hwnd || msg.hwnd == m_pTachyonWnd->GetSafeHwnd() )
				{
					UINT nFlag = HIWORD(msg.lParam);
					UINT nRep = LOWORD(msg.lParam);

					m_pTachyonWnd->OnChar( msg.wParam, nRep, nFlag);
				}
				else if(THttpCtrl::IsFocusedHTML(msg.hwnd))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				break;

			case WM_IME_COMPOSITION			:
				if( !msg.hwnd || msg.hwnd == m_pTachyonWnd->GetSafeHwnd() )
					if( CTNationOption::JAPAN_IME || CTNationOption::TAIWAN_IME )
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					else
					{
						m_pTachyonWnd->OnImeComposition(
							m_pTachyonWnd->GetSafeHwnd(),
							msg.wParam,
							msg.lParam);
					}

				break;

			case WM_SYSKEYDOWN		:
			case WM_SYSKEYUP		:
			case WM_KEYDOWN			:
			case WM_KEYUP			:
				if( msg.hwnd && msg.hwnd != m_pTachyonWnd->GetSafeHwnd() )
					if(THttpCtrl::IsFocusedHTML(msg.hwnd))
					{
						WORD wKEY = CTKeySetting::GetInstance()->GetCurKeySet(
							MapVirtualKey( LOBYTE(HIWORD(msg.lParam)), 1),
							CTClientKEY::GetCurMOD());

						if( wKEY == TKEY_CLOSE_UI )
							msg.hwnd = m_pTachyonWnd->GetSafeHwnd();
					}
					else
						msg.hwnd = m_pTachyonWnd->GetSafeHwnd();

			default					:
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				break;
			}
		}

		if( msg.message != WM_SYSCOMMAND || msg.hwnd != m_pTachyonWnd->GetSafeHwnd() )
		{
			DWORD dwTICK = ((CTClientWnd *) m_pTachyonWnd)->GetTClientTickCount();

			if( dwTICK > dwPREV )
				dwMSGTick += dwTICK - dwPREV;
		}
		else
			dwMSGTick = 0;
	}
	else
	{
		dwMSGTick = 0;

		if(!m_pTachyonWnd->m_bActivate)
		{
			LeaveCriticalSection(&m_cs);
			WaitMessage();
			EnterCriticalSection(&m_cs);

#ifndef TEST_MODE
			if( CWnd::GetForegroundWindow() != m_pTachyonWnd )
				m_dwSLEEP = 100;
			else
				m_dwSLEEP = 1;
#endif
		}
		else if(!m_pTachyonWnd->m_bOnMovie)
		{
#ifndef TEST_MODE
			if( CWnd::GetForegroundWindow() != m_pTachyonWnd )
				m_dwSLEEP = 10;
			else
				m_dwSLEEP = 1;
#endif

			m_pTachyonWnd->Render();

#ifdef TEST_MODE
			CTProfileManager::Increment_Frame_Counter();
#endif
		}
	}

	while(!m_pTachyonWnd->m_vCOMMAND.empty())
	{
		m_pTachyonWnd->OnCommand(*(m_pTachyonWnd->m_vCOMMAND.begin()));
		m_pTachyonWnd->m_vCOMMAND.erase(m_pTachyonWnd->m_vCOMMAND.begin());
	}

	return TRUE;
}

int CTClientApp::Run()
{
	BYTE bHackPass = 0;

	while(m_pTachyonWnd)
	{
		if(m_dwSLEEP)
		{
			Sleep(m_dwSLEEP);
			m_dwSLEEP = 0;
		}
#ifdef TEST_MODE
#else
		if ( !CTNationOption::INSTALL_HACKSHIELD && !CTNationOption::INSTALL_GAMEGUARD )
		{
			if( !bHackPass && !m_vModuleGuard.CheckValid() )
			{
				m_bHackMSG = m_vModuleGuard.HackDetected() ? TMP_HACK_FOUND : TMP_HACK_TIMEOUT;
				m_pTachyonWnd->PostMessage(WM_QUIT);
			}
			bHackPass = (bHackPass + 1) % 5;
		}
#endif
		SMART_LOCKCS(&m_cs);
		if(!MainProc())
			break;

		if( m_bNeedSleep && (!m_qTLOADCMD.empty() || !m_bWait) )
		{
			static DWORD dwCount = 0;

			if(!(dwCount % 3))
				m_dwSLEEP = max( m_dwSLEEP, 30);

			dwCount++;
		}

		if(((CTClientWnd *) m_pTachyonWnd)->m_bResetDevice)
			if(m_bLoaded)
				((CTClientWnd *) m_pTachyonWnd)->RestoreDevice();
			else if(m_pTachyonWnd->m_Device.Reset())
			{
				SetEvent(((CTClientWnd *) m_pTachyonWnd)->m_hResetEvent);
				((CTClientWnd *) m_pTachyonWnd)->m_bResetDevice = FALSE;
			}
	}
	ExitInstance();

	return FALSE;
}

void CTClientApp::ClearTLoadCMD()
{
	while(!m_qTLOADCMD.empty())
	{
		DeleteTLoadCMD(m_qTLOADCMD.front());
		m_qTLOADCMD.pop();
	}
}

void CTClientApp::DeleteTLoadCMD( LPTLOADCMD pTCMD)
{
	if(!pTCMD)
		return;

	switch(pTCMD->m_dwTCMD)
	{
	case TLOADCMD_OBJ	:
		if(pTCMD->m_pTBUF)
		{
			delete (LPTLOADOBJ) pTCMD->m_pTBUF;
			pTCMD->m_pTBUF = NULL;
		}

		break;
	}

	delete pTCMD;
}

#ifndef TEST_MODE

BOOL CTClientApp::HackShield_Update()
{
	DWORD dwRet = 0;
	TCHAR szFullFilePath[MAX_PATH];

	::GetCurrentDirectory( MAX_PATH, szFullFilePath );
	_tcscat ( szFullFilePath, _T( "\\HShield\\" ) );

	AHNHS_EXT_ERRORINFO HsExtError;
	ZeroMemory( &HsExtError, sizeof(AHNHS_EXT_ERRORINFO) );

	dwRet = _AhnHS_HSUpdateEx( szFullFilePath, 
		1000 * 600,
		5847,
		AHNHSUPDATE_CHKOPT_HOSTFILE | AHNHSUPDATE_CHKOPT_GAMECODE,
		HsExtError,
		1000 * 20 ); 
	
	if ( dwRet != ERROR_SUCCESS) { // 에러 처리 
		switch ( dwRet ) { 
			case HSERROR_ENVFILE_NOTREAD:
				return FALSE;
				break;
			case HSERROR_ENVFILE_NOTWRITE:
				return FALSE;
				break; 
			case HSERROR_NETWORK_CONNECT_FAIL:
				return FALSE;
				break; 
			case HSERROR_HSUPDATE_TIMEOUT:
				return FALSE; 
				break;
			default:
				return FALSE;
				break;
		}
	}



	return TRUE;
}


BOOL CTClientApp::HackShield_Init()
{
	int nRet = HS_ERR_OK;
	CString szMsg;
	DWORD dwOption = 0;

	dwOption = AHNHS_CHKOPT_ALL | AHNHS_CHKOPT_STANDALONE | AHNHS_DISPLAY_HACKSHIELD_TRAYICON
		| AHNHS_CHKOPT_UPDATED_FILE_CHECK | AHNHS_CHKOPT_LOCAL_MEMORY_PROTECTION | AHNHS_CHKOPT_SELF_DESTRUCTION | AHNHS_CHKOPT_PROTECT_D3DX;


	nRet = _AhnHS_Initialize( CString(_T(".\\HShield\\EhSvc.dll")), 
		HS_CallbackProc, 
		5847, 
		"9ADF2105564B2B134A1C716C",
		dwOption,
		AHNHS_SPEEDHACK_SENSING_RATIO_NORMAL );

	if( nRet != HS_ERR_OK )
	{
		switch( nRet )
		{
		case HS_ERR_COMPATIBILITY_MODE_RUNNING:
		case HS_ERR_NEED_ADMIN_RIGHTS:
		case HS_ERR_INVALID_FILES:
		case HS_ERR_INIT_DRV_FAILED:
		case HS_ERR_DEBUGGER_DETECT:
		case HS_ERR_NOT_INITIALIZED:
		default:
			szMsg = CTChart::Format(TSTR_HS_ERR_FUNCTION, nRet);
			break;
		}
		AfxMessageBox( szMsg, MB_OK );
		return FALSE;
	}

	return TRUE;

}

BOOL CTClientApp::HackShield_UnInit()
{
	int nRet = HS_ERR_OK; 

	nRet = _AhnHS_Uninitialize();

	if ( nRet != HS_ERR_OK )
	{ 
		return FALSE;
	}

	return TRUE;
}

BOOL CTClientApp::HackShield_StartService()
{
	int nRet = HS_ERR_OK;
	CString szMsg;

	nRet = _AhnHS_StartService(); 

	if ( nRet != HS_ERR_OK )
	{
		switch ( nRet )
		{
		case HS_ERR_START_ENGINE_FAILED:
		case HS_ERR_DRV_FILE_CREATE_FAILED:
		case HS_ERR_REG_DRV_FILE_FAILED:
		case HS_ERR_START_DRV_FAILED:
		default: 
			szMsg = CTChart::Format(TSTR_HS_ERR_FUNCTION, nRet);
			break; 
		} 
		MessageBox( NULL, szMsg, CTNationOption::m_strAppTitle, MB_OK ); 
		return FALSE; 
	} 

	return TRUE;
}

BOOL CTClientApp::HackShield_StopService()
{
	int nRet = HS_ERR_OK;

	nRet = _AhnHS_StopService(); 

	if ( nRet != HS_ERR_OK )
	{ 
		return FALSE; 
	} 

	return TRUE;
}

int __stdcall CTClientApp::HS_CallbackProc ( long lCode, long lParamSize, void* pParam )
{
	BYTE bFoundHack = FALSE;
	CString szMsg;

	CWnd* pWnd = AfxGetApp()->GetMainWnd();

	switch ( lCode )
	{
	case AHNHS_ENGINE_DETECT_GAME_HACK:
		szMsg = CTChart::Format( TSTR_HS_ERR_DETECT_GAMEHACK, lCode, (LPSTR)pParam);
		bFoundHack = TRUE;
		break;
	case AHNHS_ACTAPC_DETECT_AUTOMACRO:
		szMsg = CTChart::Format( TSTR_HS_ERR_DETECT_AUTOMACRO, lCode );
		bFoundHack = TRUE;
		break;
	case AHNHS_ACTAPC_DETECT_AUTOMOUSE:
	case AHNHS_ACTAPC_DETECT_ALREADYHOOKED:
		bFoundHack = TRUE;
		break;
	case AHNHS_ACTAPC_DETECT_SPEEDHACK:
		szMsg = CTChart::Format( TSTR_HS_ERR_DETECT_SPEEDHACK, lCode );
		bFoundHack = TRUE;
		break;
	case AHNHS_ACTAPC_DETECT_KDTRACE:
	case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
		szMsg = CTChart::Format( TSTR_HS_ERR_DETECT_KDTRACE, lCode );
		bFoundHack = TRUE;
		break;
	case AHNHS_ACTAPC_DETECT_DRIVERFAILED:
	case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
	case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:
	case AHNHS_ACTAPC_DETECT_LMP_FAILED:
	case AHNHS_ACTAPC_DETECT_MEM_MODIFY_FROM_LMP:
	case AHNHS_ACTAPC_DETECT_ENGINEFAILED:
	case AHNHS_ACTAPC_DETECT_CODEMISMATCH:
	case AHNHS_ACTAPC_DETECT_ANTIFREESERVER:
	case AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS:
	case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:
	case AHNHS_ACTAPC_DETECT_ABNORMAL_FUNCTION_CALL:
		szMsg = CTChart::Format( TSTR_HS_ERR_DETECT_OTHER, lCode );
		bFoundHack = TRUE;
		break;
	}

	if( bFoundHack )
	{
		pWnd->PostMessage(WM_QUIT);
		AfxMessageBox(szMsg, MB_OK);
	}

	return 1;
}

#endif