// TachyonApp.cpp: implementation of the CTachyonApp class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTachyonApp

BEGIN_MESSAGE_MAP(CTachyonApp, CWinApp)
	//{{AFX_MSG_MAP(CTachyonApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTachyonApp construction

CTachyonApp::CTachyonApp()
{
	m_pTachyonWnd = NULL;
	m_dwSLEEP = 0;
}

CTachyonApp::~CTachyonApp()
{
}

void CTachyonApp::LoadStdProfileSettings()
{
	if(!m_pTachyonWnd)
		return;

	CString strValue;
	DWORD dwValue;

	CWinApp::LoadStdProfileSettings();
	strValue = GetProfileString(
		_T("Settings"),
		_T("WindowedMode"),
		_T("NOT FOUND"));

	if( strValue == _T("NOT FOUND") )
	{
		WriteProfileString(
			_T("Settings"),
			_T("WindowedMode"),
#ifdef _DEBUG
			_T("TRUE"));
		m_pTachyonWnd->m_Device.m_option.m_bWindowedMode = TRUE;
	}
	else
		m_pTachyonWnd->m_Device.m_option.m_bWindowedMode = TRUE;
#else
			m_pTachyonWnd->m_Device.m_option.m_bWindowedMode ? _T("TRUE") : _T("FALSE"));
	}
	else
		m_pTachyonWnd->m_Device.m_option.m_bWindowedMode = strValue == _T("TRUE");
#endif

	strValue = GetProfileString(
		_T("Settings"),
		_T("UseShader"),
		_T("NOT FOUND"));

	if( strValue == _T("NOT FOUND") )
	{
		WriteProfileString(
			_T("Settings"),
			_T("UseShader"),
			m_pTachyonWnd->m_Device.m_option.m_bUseSHADER ? _T("TRUE") : _T("FALSE"));
	}
	else
		m_pTachyonWnd->m_Device.m_option.m_bUseSHADER = strValue == _T("TRUE");

	dwValue = GetProfileInt(
		_T("Settings"),
		_T("SwapEffect"), 0);

	if(!dwValue)
	{
		WriteProfileInt(
			_T("Settings"),
			_T("SwapEffect"),
			m_pTachyonWnd->m_Device.m_option.m_dwSwapEffect);
	}
	else
		m_pTachyonWnd->m_Device.m_option.m_dwSwapEffect = dwValue;

	dwValue = GetProfileInt(
		_T("Settings"),
		_T("PresentationInterval"), 0);

	if(!dwValue)
	{
		WriteProfileInt(
			_T("Settings"),
			_T("PresentationInterval"),
			m_pTachyonWnd->m_Device.m_option.m_dwPresentationInterval);
	}
	else
		m_pTachyonWnd->m_Device.m_option.m_dwPresentationInterval = dwValue;

	dwValue = GetProfileInt(
		_T("Settings"),
		_T("Behavior"), 0);

	if(!dwValue)
	{
		WriteProfileInt(
			_T("Settings"),
			_T("Behavior"),
			m_pTachyonWnd->m_Device.m_option.m_dwBehavior);
	}
	else
		m_pTachyonWnd->m_Device.m_option.m_dwBehavior = dwValue;

	dwValue = GetProfileInt(
		_T("Settings"),
		_T("ScreenX"), 0);

	if(!dwValue)
	{
		WriteProfileInt(
			_T("Settings"),
			_T("ScreenX"),
			m_pTachyonWnd->m_Device.m_option.m_dwScreenX);
	}
	else
		m_pTachyonWnd->m_Device.m_option.m_dwScreenX = dwValue;

	dwValue = GetProfileInt(
		_T("Settings"),
		_T("ScreenY"), 0);

	if(!dwValue)
	{
		WriteProfileInt(
			_T("Settings"),
			_T("ScreenY"),
			m_pTachyonWnd->m_Device.m_option.m_dwScreenY);
	}
	else
		m_pTachyonWnd->m_Device.m_option.m_dwScreenY = dwValue;

	dwValue = GetProfileInt(
		_T("Settings"),
		_T("TextureDetail"), TEXTURE_DETAIL_COUNT);

	if(dwValue == TEXTURE_DETAIL_COUNT)
	{
		WriteProfileInt(
			_T("Settings"),
			_T("TextureDetail"),
			m_pTachyonWnd->m_Device.m_option.m_nTextureDetail);
	}
	else
		m_pTachyonWnd->m_Device.m_option.m_nTextureDetail = (TEXTURE_DETAIL_LEVEL) dwValue;
}

void CTachyonApp::SaveStdProfileSettings()
{
	WriteProfileString(
		_T("Settings"),
		_T("WindowedMode"),
		m_pTachyonWnd->m_Device.m_option.m_bWindowedMode ? _T("TRUE") : _T("FALSE"));

	WriteProfileString(
		_T("Settings"),
		_T("UseShader"),
		m_pTachyonWnd->m_Device.m_option.m_bUseSHADER ? _T("TRUE") : _T("FALSE"));

	WriteProfileInt(
		_T("Settings"),
		_T("SwapEffect"),
		m_pTachyonWnd->m_Device.m_option.m_dwSwapEffect);

	WriteProfileInt(
		_T("Settings"),
		_T("PresentationInterval"),
		m_pTachyonWnd->m_Device.m_option.m_dwPresentationInterval);

	WriteProfileInt(
		_T("Settings"),
		_T("Behavior"),
		m_pTachyonWnd->m_Device.m_option.m_dwBehavior);

	WriteProfileInt(
		_T("Settings"),
		_T("ScreenX"),
		m_pTachyonWnd->m_Device.m_option.m_dwScreenX);

	WriteProfileInt(
		_T("Settings"),
		_T("ScreenY"),
		m_pTachyonWnd->m_Device.m_option.m_dwScreenY);

	WriteProfileInt(
		_T("Settings"),
		_T("TextureDetail"),
		m_pTachyonWnd->m_Device.m_option.m_nTextureDetail);
}

int CTachyonApp::Run()
{
	while(m_pTachyonWnd)
	{
		if(m_dwSLEEP)
		{
			Sleep(m_dwSLEEP);
			m_dwSLEEP = 0;
		}

		if(!MainProc())
			break;
	}

	return FALSE;
}

BYTE CTachyonApp::MainProc()
{
	MSG msg;

	if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	{
		if( WM_QUIT == msg.message )
		{
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
				if( msg.hwnd == m_pTachyonWnd->GetSafeHwnd() )
				{
					UINT nFlag = HIWORD(msg.lParam);
					UINT nRep = LOWORD(msg.lParam);

					m_pTachyonWnd->OnChar( msg.wParam, nRep, nFlag);
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				break;

			case WM_IME_COMPOSITION	:
				{
					m_pTachyonWnd->OnImeComposition(
						m_pTachyonWnd->GetSafeHwnd(),
						msg.wParam,
						msg.lParam);
				}

				break;

			default							:
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				break;
			}
		}
	}
	else if(!m_pTachyonWnd->m_bActivate)
	{
		WaitMessage();
	
#ifndef TEST_MODE
		if( CWnd::GetForegroundWindow() != m_pTachyonWnd )
			m_dwSLEEP = 100;
#endif
	}
	else if(!m_pTachyonWnd->m_bOnMovie)
	{
#ifndef TEST_MODE
		if( CWnd::GetForegroundWindow() != m_pTachyonWnd )
			m_dwSLEEP = 10;
#endif

		m_pTachyonWnd->Render();

#ifdef TEST_MODE
		CTProfileManager::Increment_Frame_Counter();
#endif
	}

	while(!m_pTachyonWnd->m_vCOMMAND.empty())
	{
		m_pTachyonWnd->OnCommand(*(m_pTachyonWnd->m_vCOMMAND.begin()));
		m_pTachyonWnd->m_vCOMMAND.erase(m_pTachyonWnd->m_vCOMMAND.begin());
	}

	return TRUE;
}
