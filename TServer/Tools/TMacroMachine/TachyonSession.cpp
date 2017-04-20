#include "stdafx.h"

INT64 g_4skey[KEY_COUNT] = {
	0xFDFF1FF4F1FF00FA,
	0xFAFF0F1F2FF4F0FF,
	0xA3FF12FF41FF12EB,
	0xBE41F1230A1F5678,
	0xFF178384FAFBF458,
	0xFFA1F2FEFF4620FF,
	0x4602567FE124123C};

MAPSESSION CTachyonSession::m_mapSESSION;

CTachyonSession::CTachyonSession()
{
	memset( &m_target, 0x00, sizeof(SOCKADDR_IN));
	m_sock = INVALID_SOCKET;

	m_packet.ExpandIoBuffer(MAX_PACKET_SIZE);
	m_bValid = FALSE;
	m_pOwner = NULL;

	InitializeCriticalSection(&g_cs);

	m_bMacroExcute = FALSE;
	m_bConnectType = CON_LOGIN;
	m_bRelayEnable = FALSE;
	m_bSendMsg = 0;

	m_dwRecvNumber = 0;
	m_dwSendNumber = 0;
	m_bUseCrypt = TRUE;

	ClearQueue();
	ClearMapData();
}

CTachyonSession::~CTachyonSession()
{
	m_bMacroExcute = FALSE;

	End();
	DeleteCriticalSection(&g_cs);

	ClearQueue();
	ClearMapData();
}

CTachyonSession *CTachyonSession::GetSession( SOCKET sock)
{
	MAPSESSION::iterator finder = m_mapSESSION.find(sock);

	if( finder != m_mapSESSION.end() )
		return (*finder).second;

	return NULL;
}

void CTachyonSession::InitTachyonSession()
{
	m_mapSESSION.clear();
}

void CTachyonSession::ReleaseTachyonSession()
{
	m_mapSESSION.clear();
}

void CTachyonSession::SetOwner( CTMacroMachineDoc *pOwner)
{
	m_pOwner = pOwner;
}

BOOL CTachyonSession::OnReceive( int nErrorCode)
{
	if( m_sock == INVALID_SOCKET || nErrorCode )
		return FALSE;

	if(m_packet.IsReadBufferFull())
		m_packet.ExpandIoBuffer(m_packet.GetSize());

	int nRead = recv(
		m_sock,
		(char *) m_packet.GetBuffer() + m_packet.GetReadBytes(),
		m_packet.m_dwBufferSize - m_packet.GetReadBytes(), 0);

	if( nRead == SOCKET_ERROR )
		return TRUE;

	if(!Read(nRead))
		return FALSE;

	while( CheckMSG() == PACKET_COMPLETE )
	{
		m_pOwner->OnSessionMsg( this, &m_packet);
		Flush();
	}

	return TRUE;
}

void CTachyonSession::OnConnect( int nErrorCode)
{
	m_bValid = m_sock != INVALID_SOCKET && !nErrorCode;

	if(m_pOwner)
		m_pOwner->OnConnect( this, nErrorCode);
}

void CTachyonSession::OnClose( int nErrorCode)
{
	End();
	if(m_pOwner)
		m_pOwner->OnClose( this, nErrorCode);
}

BYTE CTachyonSession::Start( LPCTSTR strAddr, DWORD dwPort, BYTE bType)
{
	BOOL bAsync = TRUE;
	End();

	if(!m_pOwner)
		return FALSE;

	m_sock = WSASocket(
		AF_INET,
		bType,
		0, NULL,
		0, 0);

	if( m_sock == INVALID_SOCKET )
		return FALSE;

	if( ioctlsocket( m_sock, FIONBIO, (unsigned long *) &bAsync) == SOCKET_ERROR )
	{
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;

		return FALSE;
	}

	hostent *pHostent = gethostbyname(strAddr);
	CString strIP = inet_ntoa(*((in_addr *) *pHostent->h_addr_list));

	memset( &m_target, 0x00, sizeof(SOCKADDR_IN));
	m_target.sin_family = AF_INET;
	m_target.sin_addr.s_addr = inet_addr(strIP);
	m_target.sin_port = htons((u_short) dwPort);

	if( WSAAsyncSelect( m_sock, m_pOwner->GetSafeHwnd(), WM_SESSION_MSG, FD_CONNECT | FD_READ | FD_CLOSE) == SOCKET_ERROR || (
		connect( m_sock, (SOCKADDR *) &m_target, sizeof(SOCKADDR_IN)) && GetLastError() != WSAEWOULDBLOCK ))
	{
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;

		return FALSE;
	}

	m_mapSESSION.insert( MAPSESSION::value_type(
		m_sock,
		this));

	return TRUE;
}

void CTachyonSession::End()
{
	MAPSESSION::iterator finder = m_mapSESSION.find(m_sock);

	if( m_sock != INVALID_SOCKET )
		closesocket(m_sock);

	if( finder != m_mapSESSION.end() )
		m_mapSESSION.erase(finder);

	m_sock = INVALID_SOCKET;
	m_bValid = FALSE;
	m_dwSendNumber = 0;
	m_dwRecvNumber = 0;

	memset( &m_target, 0x00, sizeof(SOCKADDR_IN));
	m_packet.Clear();
}

void CTachyonSession::Encrypt( CPacket *pPacket)
{
	if(!m_bUseCrypt)
		return;

	pPacket->m_pHeader->m_dwNumber = ++m_dwSendNumber;
	INT64 key = g_4skey[pPacket->m_pHeader->m_dwNumber % KEY_COUNT];
	pPacket->Encrypt(key);
	pPacket->EncryptHeader(key);	
}

BOOL CTachyonSession::Decrypt(CPacket * pPacket)
{
	if(!m_bUseCrypt)
		return TRUE;

	m_dwRecvNumber++;
	INT64 key = g_4skey[m_dwRecvNumber % KEY_COUNT];

	pPacket->DecryptHeader(key);

	if(pPacket->m_pHeader->m_dwNumber != m_dwRecvNumber)
		return FALSE;

	return TRUE;
	//return pPacket->Decrypt(key);
}

BOOL CTachyonSession::Say( CPacket *pPacket)
{
	if( !IsValid() || !pPacket )
		return FALSE;

	Encrypt(pPacket);

	LPBYTE pBUF = pPacket->GetBuffer();
	int nSEND = 0;
	int nSIZE = INT(pPacket->GetSize());

	while(nSEND < nSIZE)
	{
		int nLOCAL = send( m_sock, (const char *) pBUF, nSIZE - nSEND, 0);

		if( nLOCAL != SOCKET_ERROR )
		{
			nSEND += nLOCAL;
			pBUF += nLOCAL;
		}
		else
		{
			int nERROR = WSAGetLastError();

			if( nERROR != WSAEWOULDBLOCK )
			{
				/*
				if(m_pOwner)
					m_pOwner->GetMain()->PostMessage( WM_SESSION_MSG, m_sock, MAKELONG( FD_CLOSE, WORD(nERROR)));
					*/

				TRACE("Send Error : %d\n",nERROR);
				return FALSE;
			}
		}
	}

	return TRUE;
}

int CTachyonSession::CheckMSG()
{
	if( m_packet.GetReadBytes() < PACKET_HEADER_SIZE ||
		m_packet.GetReadBytes() < m_packet.GetSize() )
		return PACKET_INCOMPLETE;

	if(!Decrypt(&m_packet))
	{
		ATLTRACE("Decrypt Error");		
		return PACKET_INVALID;
	}

	return PACKET_COMPLETE;
}

void CTachyonSession::Flush()
{
	m_packet.Flush();
}

BYTE CTachyonSession::IsValid()
{
	return m_bValid;
}

BYTE CTachyonSession::Read( DWORD dwRead)
{
	if( dwRead == 0 )
		return FALSE;

	return m_packet.ReadBytes(dwRead);
}

void CTachyonSession::ExcuteMacro()
{
	m_bMacroExcute = TRUE;
}

BOOL CTachyonSession::IsExcuteMacro()
{
	return m_bMacroExcute;
}

BOOL CTachyonSession::EmptyTType()
{
	EnterCriticalSection(&g_cs);
	BOOL bEmpty = m_qType.empty();
	LeaveCriticalSection(&g_cs);

	return bEmpty;
}

void CTachyonSession::PushTType(int nError, BYTE bType)
{
	if(!m_bValid)
		return;

	TTYPE tType;
	tType.m_nErrorCode = nError;
	tType.m_bType = bType;

	EnterCriticalSection(&g_cs);
	m_qType.push(tType);
	LeaveCriticalSection(&g_cs);
	SetEvent(m_hEvent);
}

TTYPE CTachyonSession::FrontTType()
{
	EnterCriticalSection(&g_cs);
	TTYPE tType = m_qType.front();
	LeaveCriticalSection(&g_cs);

	return tType;
}

void CTachyonSession::PopTType()
{
	EnterCriticalSection(&g_cs);
	TTYPE tType = m_qType.front();
	m_qType.pop();
	LeaveCriticalSection(&g_cs);
}

void CTachyonSession::ClearQueue()
{
	m_mapTValue.clear();

	while(m_qTMacro.size())
		m_qTMacro.pop();
}

void CTachyonSession::SetQueue(QTMACRO qMacro, MAPTVALUE mapTValue)
{
	m_qTMacro = qMacro;
	m_mapTValue = mapTValue;
	m_tMacro = m_qTMacro.front();
}

CString CTachyonSession::GetValue(DWORD dwID)
{
	CString str = _T("NULL");
	MAPTVALUE::iterator find = m_mapTValue.find(dwID);
	if(find!=m_mapTValue.end())
		str = (*find).second.m_strValue;

	return str;
}

BOOL CTachyonSession::EmptyMacro()
{
	if(m_qTMacro.empty())
		return TRUE;

	return FALSE;
}

TMACRO CTachyonSession::FrontMacro()
{
	TMACRO qMacro = m_qTMacro.front();

	return qMacro;
}

void CTachyonSession::PopMacro()
{
	// 매크로가 MapSvr 와 RelaySvr 중 어느 서버로 가는지 구분할 수 없으므로 주석처리
	//if(m_qTMacro.front().m_bLoop)
	//	m_qTMacro.push(m_tMacro);

	m_qTMacro.pop();

	if(!m_qTMacro.empty())
		m_tMacro = m_qTMacro.front();

	TRACE(_T("POP MACRO\n") );
}

BOOL CTachyonSession::EmptyProtocol()
{
	TMACRO qMacro = m_qTMacro.front();
	QTPROTOCOL qProtocol = qMacro.m_qProtocol;

	if(qProtocol.empty())
		return TRUE;

	return FALSE;
}

BYTE CTachyonSession::GetProtocolCount()
{
	TMACRO qMacro = m_qTMacro.front();
	QTPROTOCOL qProtocol = qMacro.m_qProtocol;

	return (BYTE)qProtocol.size();
}

TQPROTOCOL CTachyonSession::FronProtocol()
{
	TMACRO qMacro = m_qTMacro.front();
	QTPROTOCOL qProtocol = qMacro.m_qProtocol;
	TQPROTOCOL tProtocol = qProtocol.front();
	
	return tProtocol;
}

void CTachyonSession::PopProtocol()
{
	while(m_qTMacro.front().m_qProtocol.size() != 0 )
	{		
		TQPROTOCOL TmpProtocol = FronProtocol();
		MAPTPROTOCOL::iterator itF = m_pOwner->m_mapProtocol.find(TmpProtocol.m_dwID);

		if(itF != m_pOwner->m_mapProtocol.end() )
			TRACE(_T("POP PROTOCOL: %s Session: %d\n"),(*itF).second,m_wID);

		if(TmpProtocol.m_dwID == CS_CHAT_REQ)
			int a = 0;

		m_qTMacro.front().m_qProtocol.pop();
		
		if(m_qTMacro.front().m_qProtocol.size() == 0  )
			break;

		TQPROTOCOL tProtocol = FronProtocol();
		
		if(CheckProtocolType(tProtocol.m_dwID))
			break;
	}
}

BYTE CTachyonSession::CheckProtocolType(DWORD dwProtocolID)
{	
	if(m_bConnectType != CON_MAP && m_bConnectType != CON_RELAY)
		return TRUE;

	BYTE bType = CON_MAP;
	switch(dwProtocolID)
	{
	case CS_CHAT_REQ:		 
		if(!m_bRelayEnable)		
			bType = CON_MAP;		
		else
			bType = CON_RELAY;

		break;

	case CS_RELAYCHARDATA_REQ:
	case CS_CORPSCMD_REQ:
	case CS_CORPSENEMYLIST_REQ:
	case CS_MOVECORPSENEMY_REQ:
	case CS_MOVECORPSUNIT_REQ:
	case CS_ADDCORPSENEMY_REQ:
	case CS_DELCORPSENEMY_REQ:
	case CS_CORPSHP_REQ:
	case CS_RELAYPROTECTEDLIST_REQ:
	case CS_RELAYPROTECTEDADD_REQ:
	case CS_RELAYPROTECTEDDEL_REQ:
		{
			bType = CON_RELAY;
			break;
		}
	default:
		break;
	}

	if(m_bConnectType != bType )
		return FALSE;

	return TRUE;
}

TCHILD CTachyonSession::GetChild()
{
	TCHILD tChild;
	TMACRO qMacro = m_qTMacro.front();
	QTPROTOCOL qProtocol = qMacro.m_qProtocol;
	if(qProtocol.size() == 0 )
		return tChild;

	TQPROTOCOL tProtocol = qProtocol.front();	
	
	if( tProtocol.m_vTChild.size() > 0 )
	{
		for(DWORD dwC = 0; dwC < tProtocol.m_vTChild.size(); dwC++)
		{
			if( !tProtocol.m_vTChild[dwC].m_bRecvAck)
			{
				tChild = tProtocol.m_vTChild[dwC];
				break;
			}
		}
	}

	return tChild;
}

BOOL CTachyonSession::IsLoopProtocol()
{
	TMACRO qMacro = m_qTMacro.front();
	QTPROTOCOL qProtocol = qMacro.m_qProtocol;
	TQPROTOCOL tProtocol = qProtocol.front();

	return (BOOL)tProtocol.m_bLoop;
}

void CTachyonSession::UpdateValue(DWORD dwID, CString str)
{
	MAPTVALUE::iterator find = m_mapTValue.find(dwID);
	if(find!=m_mapTValue.end())
		(*find).second.m_strValue = str;
}

BOOL CTachyonSession::CheckSelf(DWORD dwID, CString str)
{
	MAPTVALUE::iterator find = m_mapTValue.find(dwID);
	if(find==m_mapTValue.end())
		return FALSE;

	if((*find).second.m_strName == STR_SELF_NAME)
	{
		CString strValue = (*find).second.m_strValue;

		TRACE(_T("SELF VALUE : %s COMPARE VALUE : %s\n"), strValue, str);

		MAPTVALUE::iterator it;
		for(it=m_mapTValue.begin(); it!=m_mapTValue.end(); it++)
		{
			if( (*it).second.m_strName == strValue &&
				(*it).second.m_strValue == str )
					return TRUE;
		}

		return FALSE;
	}

	return TRUE;
}

BOOL CTachyonSession::CheckBreak(DWORD dwID)
{
	MAPTVALUE::iterator find = m_mapTValue.find(dwID);
	if(find==m_mapTValue.end())
		return TRUE;

	if((*find).second.m_strName == STR_END_NAME)
		return TRUE;

	return FALSE;
}

void CTachyonSession::ClearMapData()
{
	m_strMapIP = _T("");
	m_wMapPort = 0;
	m_bMapConnect = FALSE;
}

BOOL CTachyonSession::IsMapConnect()
{
	return m_bMapConnect;
}

BYTE CTachyonSession::CheckMapConnect(DWORD dwID)
{
	if(dwID == CS_START_ACK)
	{
		m_bMapConnect = TRUE;
		TRACE(_T("CHECK MAP CONNECT : %d\n"), m_bMapConnect);
		return TRUE;
	}

	return FALSE;
}

void CTachyonSession::CheckMapIP(DWORD dwID, DWORD dwIP)
{
	if(dwID == CS_START_ACK)
	{
		SOCKADDR_IN vADDR;
		vADDR.sin_addr.S_un.S_addr = dwIP;
		m_strMapIP = inet_ntoa(vADDR.sin_addr);
		TRACE(_T("MAP IP : %s\n"), m_strMapIP);
	}
}

void CTachyonSession::CheckRelayIP(DWORD dwID, DWORD dwIP)
{
	if(dwID == CS_RELAYCONNECT_ACK)
	{
		SOCKADDR_IN vADDR;
		vADDR.sin_addr.S_un.S_addr = dwIP;
		m_strRelayIP = inet_ntoa(vADDR.sin_addr);
		TRACE(_T("RELAY IP : %s \n"),m_strRelayIP);
	}
}

void CTachyonSession::CheckMapPort(DWORD dwID, WORD wPort)
{
	if(dwID == CS_START_ACK)
	{
		m_wMapPort = wPort;
		TRACE(_T("MAP PORT : %d\n"), m_wMapPort);
	}
}

void CTachyonSession::CheckRelayPort(DWORD dwID, WORD wPort)
{
	if(dwID == CS_RELAYCONNECT_ACK)
	{
		m_wRelayPort = wPort;
		TRACE(_T("Relay PORT : %d\n"), m_wRelayPort);
	}
}



BYTE CTachyonSession::CheckRelayConnect(DWORD dwID)
{
	if(dwID == CS_RELAYCONNECT_ACK)	
		return TRUE;
	
	return FALSE;
}

BYTE CTachyonSession::GetConnectType()
{
	return m_bConnectType;
}

void CTachyonSession::SetConnectType(BYTE bConnectType)
{
	m_bConnectType = bConnectType;
}

void CTachyonSession::PushLoopProtocol(TQPROTOCOL Protocol)
{	
	m_mapLoopProtocol.insert( MAPTQPROTOCOL::value_type(Protocol.m_dwID, Protocol) );

	MAPTPROTOCOL::iterator find = m_pOwner->m_mapProtocol.find(Protocol.m_dwID);
	if(find != m_pOwner->m_mapProtocol.end())
		TRACE("Push Loop Protocol : %s Session : %d \n",(*find).second, m_wID);	
}

BOOL CTachyonSession::EmptyLoopProtocol()
{
	if( m_mapLoopProtocol.empty() )
		return TRUE;

	return FALSE;
}

void CTachyonSession::SetRecvAck(DWORD dwID, BOOL bRecvAck)
{
	TMACRO qMacro = m_qTMacro.front();
	QTPROTOCOL qProtocol = qMacro.m_qProtocol;
	TQPROTOCOL tProtocol = qProtocol.front();
	TCHILD tChild;

	if( tProtocol.m_vTChild.size() > 0 )
	{
		for(DWORD dwC = 0; dwC < tProtocol.m_vTChild.size(); dwC++)
		{
			if( tProtocol.m_vTChild[dwC].m_dwID == dwID)
			{
				m_qTMacro.front().m_qProtocol.front().m_vTChild[dwC].m_bRecvAck = bRecvAck;
				break;
			}
		}
	}
}
