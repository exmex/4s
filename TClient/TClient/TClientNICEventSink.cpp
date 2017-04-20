#include "stdafx.h"
#include "TClientNICEventSink.h"
#include "TClientWnd.h"

CTClientNICEventSink::CTClientNICEventSink()
:	m_pMainWnd( NULL ),
m_nStatus( -1 ),
m_bInit( false )
{
}

CTClientNICEventSink::~CTClientNICEventSink()
{
}

BOOL CTClientNICEventSink::InitTEVENT( CTClientWnd* pWnd, CString& strIP, int nTimer )
{
	m_pMainWnd = pWnd;
	m_strIP = strIP;
	m_nTimer = nTimer;

	return OpenService_Winmgmt();
}

void CTClientNICEventSink::Close()
{
	CloseServiceHandle( m_hService );
	CloseServiceHandle( m_hServiceMgr );
}

void CTClientNICEventSink::ReleaseTEVENT()
{
	Close();

	CTNICEventSink::ReleaseTEVENT();
}

BOOL CTClientNICEventSink::OpenService_Winmgmt()
{
	m_hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	if( m_hServiceMgr == NULL )
	{
		CloseServiceHandle( m_hServiceMgr );
		return FALSE;
	}

	m_hService = OpenService( m_hServiceMgr, "winmgmt", SERVICE_ALL_ACCESS );
	if( m_hService == NULL )
	{
		Close();
		return FALSE;
	}

	DWORD dwStartType = GetStartType();
	if( dwStartType == SERVICE_DISABLED )
	{
		if( !SC_Change_to_AutoStart() )
		{
			Close();
			return FALSE;
		}
	}

	m_bInit = true;

	return TRUE;
}

BOOL CTClientNICEventSink::UpdateService()
{
	if( !m_bInit || !m_pMainWnd )
		return FALSE;

	static int sStatus = -1;
	GetCurrentStatus();

	if( sStatus != m_nStatus )	// 상태가 변함.
	{
		switch( m_nStatus )
		{
		case SERVICE_STOPPED :
			SC_Start();
			break;
		case SERVICE_PAUSED :
			SC_Continue();
			break;
		case SERVICE_RUNNING :
			CTNICEventSink::InitTEVENT( m_strIP, m_nTimer );
			break;
		}

		sStatus = m_nStatus;
	}

	return TRUE;
}

int CTClientNICEventSink::GetCurrentStatus()
{
	SERVICE_STATUS scStatus;
	ControlService( m_hService, SERVICE_CONTROL_INTERROGATE, &scStatus );

	return m_nStatus = scStatus.dwCurrentState;
}

void CTClientNICEventSink::OnMediaDisconnect( IWbemClassObject* pObj )
{
	if( m_pMainWnd )
	{
		// 게임에 접속중 일때만 처리.
		if( m_pMainWnd->GetMainFrame() == &m_pMainWnd->m_MainGame )
		{
			m_pMainWnd->m_bNetCableDisconnected = TRUE;
			m_pMainWnd->m_vCOMMAND.push_back( GM_NETCABLE_DISCONNECTED );
		}
	}
}


DWORD CTClientNICEventSink::GetStartType()
{
	QUERY_SERVICE_CONFIG sc_config;
	DWORD dwBytesNeeded;
	if( !QueryServiceConfig( m_hService, &sc_config, 0, &dwBytesNeeded ) )
		return -1;

	return sc_config.dwStartType;
}

BOOL CTClientNICEventSink::SC_Start()
{
	ChangeServiceConfig( 
		m_hService,        // handle of service 
		SERVICE_NO_CHANGE, // service type: no change 
		SERVICE_AUTO_START,       // change service start type 
		SERVICE_NO_CHANGE, // error control: no change 
		NULL,              // binary path: no change 
		NULL,              // load order group: no change 
		NULL,              // tag ID: no change 
		NULL,              // dependencies: no change 
		NULL,              // account name: no change 
		NULL,              // password: no change 
		NULL);            // display name: no change

	return StartService( m_hService, NULL, NULL );
}

BOOL CTClientNICEventSink::SC_Continue()
{
	SERVICE_STATUS nStatus;

	return ControlService( m_hService, SERVICE_CONTROL_CONTINUE, &nStatus );
}

BOOL CTClientNICEventSink::SC_Change_to_AutoStart()
{
	if( !ChangeServiceConfig( m_hService,
		SERVICE_NO_CHANGE,
		SERVICE_AUTO_START,
		SERVICE_NO_CHANGE,
		NULL, NULL, NULL,
		NULL, NULL, NULL, NULL ) )
		return FALSE;

	return TRUE;
}