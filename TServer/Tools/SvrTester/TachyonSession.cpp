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
	memset( &m_source, 0x00, sizeof(SOCKADDR_IN));
	memset( &m_real, 0x00, sizeof(SOCKADDR_IN));
	memset( &m_addr, 0x00, sizeof(SOCKADDR_IN));

	while(!m_qSEND.empty())
	{
		delete m_qSEND.front();
		m_qSEND.pop();
	}

	m_bLocalIP = FALSE;
	m_bCanSend = TRUE;
	m_bValid = FALSE;
	m_pOwner = NULL;

	m_sock = INVALID_SOCKET;
	m_bType = SOCK_STREAM;

	m_dwBufSize = 0;
	m_dwQRead = 0;

	/////////////////////////////////////////////////////////////
	m_SessionID = 0; // Session ID
	m_hEvent = 0;
	m_hThread = 0;
    m_dwThreadID = 0;

	// Message Queue
	while(!m_qMessage.empty())	
		m_qMessage.pop();

	InitializeCriticalSection(&g_cs); // Critical Section

	ListClear(); // List Deque Clear

	InitCharInfo(); // Char Info Init

	// CS_LOGIN_ACK
	m_bLogin = FALSE;
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	// Macro Info
	ClearMacroInfo();

	m_vParam.clear();
	/////////////////////////////////////////////////////////////
}

CTachyonSession::~CTachyonSession()
{
	/////////////////////////////////////////////////////////////
	DeleteCriticalSection(&g_cs); // Critical Section

	ListClear(); // List Deque Clear
	InitCharInfo(); // Char Info Init

	m_bLogin = FALSE;
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	// Macro Info
	ClearMacroInfo();

	m_vParam.clear();
	/////////////////////////////////////////////////////////////

	End();
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

void CTachyonSession::SetOwner( CTSessionDoc *pOwner)
{
	m_pOwner = pOwner;
}

BOOL CTachyonSession::OnReceive( int nErrorCode)
{
	if( m_sock == INVALID_SOCKET || nErrorCode )
		return FALSE;

	switch(m_bType)
	{
	case SOCK_STREAM	:
		{
			if(m_packet.IsReadBufferFull())
				m_packet.ExpandIoBuffer(m_packet.GetSize());

			int nRead = recv(
				m_sock,
				(char *) m_packet.GetBuffer() + m_packet.GetReadBytes(),
				m_packet.m_dwBufferSize - m_packet.GetReadBytes(), 0);

			if(!Read(nRead))
				return FALSE;

			while( CheckMSG() == PACKET_COMPLETE )
			{
				m_pOwner->OnSessionMsg( this, &m_packet);
				Flush();
			}
		}

		break;

	case SOCK_DGRAM		:
		{
			int nSIZE = sizeof(SOCKADDR_IN);

			memset( &m_source, 0x00, sizeof(SOCKADDR_IN));
			int nRead = recvfrom(
				m_sock,
				(char *) m_packet.GetBuffer(),
				m_packet.m_dwBufferSize, 0,
				(SOCKADDR *) &m_source,
				&nSIZE);

			if( nRead <= 0 )
				return FALSE;

			if(!Read(nRead))
				return FALSE;

			while( CheckMSG() == PACKET_COMPLETE )
			{
				m_pOwner->OnSessionMsg( this, &m_packet);
				Flush();
			}
		}

		break;
	}

	return TRUE;
}

BYTE CTachyonSession::OnSendComplete()
{
	DWORD dwTRANS = 0;
	DWORD dwFlags = 0;

	if(m_bCanSend)
		return TRUE;

	if(!WSAGetOverlappedResult(
		m_sock,
		(LPWSAOVERLAPPED) &m_ov,
		&dwTRANS,
		TRUE,
		&dwFlags))
		return TRUE;

	if( dwTRANS > 0 )
	{
		m_dwBufSize -= dwTRANS;
		if(m_dwBufSize)
			memcpy( m_pBUF, m_pBUF + dwTRANS, m_dwBufSize);
	}
	m_bCanSend = TRUE;

	return Post();
}

BYTE CTachyonSession::Post()
{
	if( INVALID_SOCKET == m_sock )	
		return FALSE;	

	if( m_qSEND.empty() && m_dwBufSize == 0 )
		return TRUE;

	if( !m_bCanSend || m_dwBufSize >= SESSIONBUF_SIZE )
		return TRUE;

	while( !m_qSEND.empty() && m_dwBufSize < SESSIONBUF_SIZE )
	{
		CPacket *pPacket = m_qSEND.front();

		LPBYTE pSRC = pPacket->GetBuffer() + m_dwQRead;
		LPBYTE pDEST = m_pBUF + m_dwBufSize;

		DWORD dwSRC = pPacket->GetSize() - m_dwQRead;
		DWORD dwDEST = SESSIONBUF_SIZE - m_dwBufSize;
		DWORD dwTRANS = min( dwSRC, dwDEST);

		memcpy( pDEST, pSRC, dwTRANS);
		m_dwBufSize += dwTRANS;
		m_dwQRead += dwTRANS;

		if( m_dwQRead >= pPacket->GetSize() )
		{
			m_qSEND.pop();
			m_dwQRead = 0;

			delete pPacket;
		}
	}

	DWORD dwSENT = 0;
	WSABUF vBUF;

	vBUF.buf = (char *) m_pBUF;
	vBUF.len = m_dwBufSize;

	m_ov.m_bTYPE = TOV_SSN_SEND;
	m_ov.m_pOBJ = this;

	if( WSASend( m_sock, &vBUF, 1, &dwSENT,
		0, (LPOVERLAPPED) &m_ov, NULL) == SOCKET_ERROR )
	{		
		int nERROR = WSAGetLastError();

		if( nERROR != WSA_IO_PENDING )		
			return FALSE;		

		m_bCanSend = FALSE;
	}
	else
	{
		m_bCanSend = FALSE;
		return OnSendComplete();
	}

	return TRUE;
}

void CTachyonSession::OnSend( int nErrorCode)
{
	if( m_sock != INVALID_SOCKET && !nErrorCode )
		OnSendComplete();
}

void CTachyonSession::OnConnect( int nErrorCode)
{	
	m_bValid = m_sock != INVALID_SOCKET && !nErrorCode;

	if(m_pOwner)
		m_pOwner->OnConnect( this, nErrorCode);
}

void CTachyonSession::OnClose( int nErrorCode)
{
	if(m_pOwner)
		m_pOwner->OnClose( this, nErrorCode);
	End();
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

	char pHostName[32];
	gethostname( pHostName, 32);
	hostent *pHostent = gethostbyname(pHostName);

	memset( &m_addr, 0x00, sizeof(SOCKADDR_IN));
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = ((in_addr *) *pHostent->h_addr_list)[0].s_addr;
	m_addr.sin_port = htons((u_short) dwPort);
	memcpy( &m_real, &m_addr, sizeof(SOCKADDR_IN));

	m_mapSESSION.insert( MAPSESSION::value_type(
		m_sock,
		this));
	m_bType = bType;

	switch(bType)
	{
	case SOCK_STREAM	:
		{
			pHostent = gethostbyname(strAddr);
			CString strIP = inet_ntoa(*((in_addr *) *pHostent->h_addr_list));

			memset( &m_target, 0x00, sizeof(SOCKADDR_IN));
			m_target.sin_family = AF_INET;
			m_target.sin_addr.s_addr = inet_addr(strIP);
			m_target.sin_port = htons((u_short) dwPort);

			if( WSAAsyncSelect( m_sock, m_pOwner->GetSafeHwnd(), WM_SESSION_MSG, FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE|FD_OOB) == SOCKET_ERROR || (
				connect( m_sock, (SOCKADDR *) &m_target, sizeof(SOCKADDR_IN)) && GetLastError() != WSAEWOULDBLOCK ))
			{
				m_mapSESSION.erase(m_sock);
				closesocket(m_sock);
				m_sock = INVALID_SOCKET;				

				return FALSE;
			}
		}

		break;

	case SOCK_DGRAM		:
		{
			if( bind( m_sock, (SOCKADDR *) &m_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR ||
				WSAAsyncSelect( m_sock, m_pOwner->GetSafeHwnd(), WM_SESSION_MSG, FD_READ|FD_OOB) == SOCKET_ERROR )
			{
				m_mapSESSION.erase(m_sock);
				closesocket(m_sock);
				m_sock = INVALID_SOCKET;

				return FALSE;
			}

			m_bValid = TRUE;
		}

		break;
	}	

	return TRUE;
}

void CTachyonSession::End()
{
	MAPSESSION::iterator finder = m_mapSESSION.find(m_sock);

	if( m_sock != INVALID_SOCKET )
		closesocket(m_sock);

	if( finder != m_mapSESSION.end() )
		m_mapSESSION.erase(finder);

	memset( &m_target, 0x00, sizeof(SOCKADDR_IN));
	memset( &m_real, 0x00, sizeof(SOCKADDR_IN));
	memset( &m_addr, 0x00, sizeof(SOCKADDR_IN));

	while(!m_qSEND.empty())
	{
		delete m_qSEND.front();
		m_qSEND.pop();
	}

	m_sock = INVALID_SOCKET;
	m_bType = SOCK_STREAM;

	m_dwBufSize = 0;
	m_dwQRead = 0;
	m_dwSendNumber = 0;

	m_bLocalIP = FALSE;
	m_bCanSend = TRUE;
	m_bValid = FALSE;

	m_packet.Clear();
}

void CTachyonSession::Encrypt( CPacket *pPacket)
{
	pPacket->m_pHeader->m_dwNumber = ++m_dwSendNumber;
	INT64 key = g_4skey[pPacket->m_pHeader->m_dwNumber % KEY_COUNT];
	pPacket->Encrypt(key);
}

BYTE CTachyonSession::Say( CPacket *pPacket, SOCKADDR_IN *pADDR)
{
	if( INVALID_SOCKET == m_sock )
	{
		delete pPacket;
		pPacket = NULL;
		return FALSE;
	}

	///Encrypt(pPacket);

	switch(m_bType)
	{
	case SOCK_DGRAM		:
		{
			sendto( m_sock, (const char *) pPacket->GetBuffer(), pPacket->GetSize(), 0, (SOCKADDR *) pADDR, sizeof(SOCKADDR_IN));
			delete pPacket;
			pPacket = NULL;
		}

		break;

	case SOCK_STREAM	:
		{
			m_qSEND.push(pPacket);
			return Post();
		}

		break;
	}	

	return FALSE;
}

BYTE CTachyonSession::Say( CPacket *pPacket)
{
	return Say( pPacket, &m_target);
}

void CTachyonSession::SetUDPTarget( SOCKADDR_IN *pADDR)
{
	memcpy( &m_target, pADDR, sizeof(SOCKADDR_IN));
}

int CTachyonSession::CheckMSG()
{
	if( m_packet.GetReadBytes() < PACKET_HEADER_SIZE ||
		m_packet.GetReadBytes() < m_packet.GetSize() )
		return PACKET_INCOMPLETE;

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

/////////////////////////////////////////////////////////////
// Session ID / Event
void CTachyonSession::SetID(DWORD SessionID)
{
	m_SessionID = SessionID;	
}
void CTachyonSession::SetHandle(HANDLE hEvent)
{	
	m_hEvent = hEvent;
}

// Message Queue Push / Front / Pop / Empty / Clear
void CTachyonSession::PushMessage(STRUCTTHREAD sThread)
{	
    EnterCriticalSection(&g_cs);
	m_qMessage.push(sThread);
	SetEvent(m_hEvent);
	LeaveCriticalSection(&g_cs);
}
STRUCTTHREAD CTachyonSession::FrontMessage()
{	
	EnterCriticalSection(&g_cs);
	STRUCTTHREAD sThread = m_qMessage.front();
	LeaveCriticalSection(&g_cs);
	return sThread;
}
void CTachyonSession::PopMessage()
{
	EnterCriticalSection(&g_cs);	
	m_qMessage.pop();
	LeaveCriticalSection(&g_cs);
}
BOOL CTachyonSession::EmptyMessage()
{	
	return m_qMessage.empty();
}
void CTachyonSession::ClearMessage()
{
	while( !m_qMessage.empty() )
	{
		STRUCTTHREAD sThread = m_qMessage.front();
		if( sThread.type == THREAD_TYPE_SEND )
			delete sThread.packet;
		m_qMessage.pop();
	}
}

// List Deque Push / Pop / Clear
void CTachyonSession::PushList(CString strProtocol, CString strValue)
{
	STRUCTSTRING temp;
	temp.strProtocol = strProtocol;
	temp.strValue = strValue;

	if( m_dqList.size() == LIST_MAX )
		m_dqList.pop_front();

	m_dqList.push_back(temp);
}
STRUCTSTRING CTachyonSession::PopList(int i)
{
	return m_dqList[i];
}
void CTachyonSession::ListClear()
{
    m_dqList.clear();
}

// UserID / CharID / Key / GroupID / Channel / SvrID Init
void CTachyonSession::InitCharInfo()
{
	m_sCharInfo.UserID = _T("");
	m_sCharInfo.CharID = _T("");
	m_sCharInfo.KEY = _T("");
	m_sCharInfo.GroupID = _T("");
	m_sCharInfo.Channel = _T("");
	m_sCharInfo.SvrID = _T("");
}
// UserID / CharID / Key / GroupID / Channel / SvrID Set
void CTachyonSession::SetCharInfo(CString sProtocol, CString sValue)
{
	CString strProtocol = sProtocol;
	CString strValue = sValue;

	if( strProtocol == _T("CS_LOGIN_ACK") )
	{	
		int pos1, pos2;

		pos1 = strValue.Find(_T("("));
		pos2 = strValue.Find(_T(")"));
		CString temp = strValue.Mid(pos1, pos2-pos1);
		temp.Trim(_T("() "));
		if( temp != _T("0") )
		{
			InitCharInfo();
			return;
		}

		pos1 = strValue.Find(_T("DWORD"));

		strValue = strValue.Mid(pos1);
		pos1 = strValue.Find(_T("("));
		pos2 = strValue.Find(_T(")"));
		m_sCharInfo.UserID = strValue.Mid(pos1, pos2-pos1);
		m_sCharInfo.UserID.Trim(_T("() "));

        strValue = strValue.Mid(pos2+1);
		pos1 = strValue.Find(_T("("));
		pos2 = strValue.Find(_T(")"));
		m_sCharInfo.CharID = strValue.Mid(pos1, pos2-pos1);
		m_sCharInfo.CharID.Trim(_T("() "));

		strValue = strValue.Mid(pos2+1);
		pos1 = strValue.Find(_T("("));
		pos2 = strValue.Find(_T(")"));
		m_sCharInfo.KEY = strValue.Mid(pos1, pos2-pos1);
		m_sCharInfo.KEY.Trim(_T("() "));
	}	
	if( strProtocol == _T("CS_START_ACK") )
	{
		int pos1, pos2;		
		CString temp;

		m_bLogin = TRUE;

		pos1 = strValue.Find(_T("BYTE"));
		strValue = strValue.Mid(pos1+1);
		pos1 = strValue.Find(_T("("));
		pos2 = strValue.Find(_T(")"));
		temp = strValue.Mid(pos1, pos2-pos1);
		temp.Trim(_T("() "));
		if( atoi(temp) != 0 )
			m_bLogin = FALSE;

		pos1 = strValue.Find(_T("DWORD"));
		strValue = strValue.Mid(pos1+1);
		pos1 = strValue.Find(_T("("));
		pos2 = strValue.Find(_T(")"));
		temp = strValue.Mid(pos1, pos2-pos1);
		temp.Trim(_T("() "));		

		SOCKADDR_IN vADDR;
		vADDR.sin_addr.S_un.S_addr = atoi(temp);
		m_sMapIP = inet_ntoa(vADDR.sin_addr);

		strValue = strValue.Mid(pos2+1);		
		pos1 = strValue.Find(_T("("));
		pos2 = strValue.Find(_T(")"));
		temp = strValue.Mid(pos1, pos2-pos1);
		temp.Trim(_T("() "));
		m_wMapPort = atoi(temp);

		pos1 = strValue.Find(_T("BYTE"));
		strValue = strValue.Mid(pos1);		

		pos1 = strValue.Find(_T("("));
		pos2 = strValue.Find(_T(")"));
		m_sCharInfo.SvrID = strValue.Mid(pos1, pos2-pos1);
		m_sCharInfo.SvrID.Trim(_T("() "));		
	}
}
void CTachyonSession::SetCharInfo(int i, CString sParam)
{
	CString strParam = sParam;

	switch(i)
	{
	case 1:		
		m_sCharInfo.GroupID = strParam;		
		break;
	case 2:
		int pos;

		pos = strParam.Find(_T(" "));
		m_sCharInfo.GroupID = strParam.Left(pos);
		m_sCharInfo.GroupID.Trim(_T(" "));

		strParam = strParam.Mid(pos+1);
        pos = strParam.Find(_T(" "));		
		m_sCharInfo.Channel = strParam.Left(pos);
		m_sCharInfo.Channel.Trim(_T(" "));
		
		m_sCharInfo.CharID = strParam.Mid(pos+1);
		m_sCharInfo.CharID.Trim(_T(" "));
		break;
	}	
}
// UserID / CharID / Key / GroupID / Channel / SvrID Get
CString CTachyonSession::GetUserID()
{
	return m_sCharInfo.UserID;
}
CString CTachyonSession::GetCharID()
{
	return m_sCharInfo.CharID;
}
CString CTachyonSession::GetKEY()
{
	return m_sCharInfo.KEY;
}
CString CTachyonSession::GetGroupID()
{
	return m_sCharInfo.GroupID;
}
CString CTachyonSession::GetChannel()
{
	return m_sCharInfo.Channel;
}
CString CTachyonSession::GetSvrID()
{
	return m_sCharInfo.SvrID;
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// Macro Info
void CTachyonSession::SetMacroInfo(QUEUEDWORD qID, MAPMACRO mMacro)
{
	m_mMacro.clear();

	m_qID = qID;
	m_mMacro = mMacro;

	SetSelfVector();
}
void CTachyonSession::ClearMacroInfo()
{
	while( !m_qID.empty() )
		m_qID.pop();
	m_mMacro.clear();

	InitSendCnt();

	ClearSelfVector();
}
BOOL CTachyonSession::IsEmptyMacro()
{
	BOOL flag = m_qID.empty();

	return flag;
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// Self Vector Set / Clear / Check
void CTachyonSession::SetSelfVector()
{
	m_vSelf.clear();

	MAPMACRO::iterator it;
	for(it = m_mMacro.begin(); it != m_mMacro.end(); it++)
	{
		VECTORSELFINFO vSInfo = (*it).second->GetSelfInfo();
		
		for(int i = 0; i < (int)vSInfo.size(); i++)
		{
			STRUCTSELFINFO sInfo = vSInfo[i];
			m_vSelf.push_back(sInfo);
		}
	}	
}
void CTachyonSession::ClearSelfVector()
{
	m_vSelf.clear();
}
BOOL CTachyonSession::IsSelfCheck(DWORD pID, CString sData)
{
	for(int i = 0; i < (int)m_vSelf.size(); i++)
	{
		if( m_vSelf[i].dwPID == pID )
			break;
	}

	if( i == m_vSelf.size() )
		return TRUE;

	i = m_vSelf[i].pos;
    CString data = sData;
	int pos;
	
	while( i-- )
	{
		pos = data.Find(_T(")"));
		data = data.Mid(pos+1);
	}

	pos = data.Find(_T("("));
	data = data.Mid(pos);
	pos = data.Find(_T(")"));
	data = data.Left(pos);
	data.Trim(_T("()"));

    if( GetCharID() != data)
		return FALSE;

	return TRUE;
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// Macro Param Set / Get
void CTachyonSession::SetMacroParam()
{
	m_vParam.clear();

	DWORD mID = m_qID.front();
	MAPMACRO::iterator finder = m_mMacro.find(mID);

	m_vParam = (*finder).second->GetMacroParam();
}
VECTORPARAM CTachyonSession::GetMacroParam()
{	
	return m_vParam;	
}
void CTachyonSession::UpdateMacroParam(CString sName, CString sType, CString sData)
{
	for(int i = 0; i < (int)m_vParam.size(); i++)
	{
		if( m_vParam[i].strName == sName &&
			m_vParam[i].strType == sType )
		{
			m_vParam[i].strData = sData;
			break;
		}
	}
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// Macro Send Message
STRUCTPINFO CTachyonSession::GetSendMsg()
{
    STRUCTPINFO pInfo;

    DWORD mID = m_qID.front();
	MAPMACRO::iterator finder = m_mMacro.find(mID);
	CTMacro* pMacro = (*finder).second;	
	
	if( pMacro->IsSendLast(m_SendCnt) )
	{
		InitSendCnt();
		if( !pMacro->GetMacroLoop() )		
			m_qID.pop();
		
		if( !m_qID.empty() )
		{
			mID = m_qID.front();
			SetMacroParam();

			MAPMACRO::iterator find = m_mMacro.find(mID);
			pMacro = (*find).second;
			pInfo = pMacro->GetCurSendInfo(m_SendCnt++);
		}
	}
	else
		pInfo = pMacro->GetCurSendInfo(m_SendCnt++);
	
	return pInfo;	
}

// Send Count
void CTachyonSession::InitSendCnt()
{
	m_SendCnt = 0;
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// Set Param Data
BOOL CTachyonSession::SetParamData(DWORD pID, CString sData)
{
	DWORD mID = m_qID.front();
	MAPMACRO::iterator finder = m_mMacro.find(mID);
	CTMacro* pMacro = (*finder).second;

	VECTORPARAM vParam;
	VECTORSRINFO rInfo = pMacro->GetRecvInfo();	
	for(int i = 0; i < (int)rInfo.size(); i++)
	{
		if( rInfo[i].dwPID == pID )
		{
			vParam = rInfo[i].vParam;
			break;
		}
	}

	if( vParam.empty() )
		return FALSE;

	VECTORSTRING vData;
	vData = RecvParamData(vParam, sData);	

	for(i = 0; i < (int)vParam.size(); i++)
	{	
		CString strName = vParam[i].strName;
		CString strType = vParam[i].strType;

		if( strName == _T("Default") )
			continue;

		if( vData[i] != _T("") )
			UpdateMacroParam(strName, strType, vData[i]);		
	}

	return TRUE;
}
// Param Data Loop / Get
VECTORSTRING CTachyonSession::RecvParamData(VECTORPARAM vParam, CString sData)
{	
	VECTORSTRING vData;
	CString sPData = sData;
	
	for(int i = 0; i < (int)vParam.size(); i++)
	{
		// Loop Start
		if( vParam[i].strType == _T("LOOPSTART") )
		{
			int loop = atoi(vData.back());
			vData.push_back(_T("loop start"));
			i = RecvParamLoop(&vData, &sPData, i+1, loop, vParam);
		}		
		else
		{
            int pos = sPData.Find(_T("("));
			sPData = sPData.Mid(pos);
			pos = sPData.Find(_T(")"));
			CString data = sPData.Left(pos);
			data.Trim(_T("()"));
			sPData = sPData.Mid(pos+1);

			vData.push_back(data);
		}		
	}

	return vData;
}
int CTachyonSession::RecvParamLoop(VECTORSTRING* vData, CString* sPData, int i, int LoopCnt, VECTORPARAM vParam)
{
	int start = i;
	int last;

	if( LoopCnt == 0 )
	{
		int cnt = 1;
		vData->push_back(_T(""));
		while(cnt)
		{
			i++;
			if(vParam[i].strType == _T("LOOPEND"))
			{
				vData->push_back(_T("loop end"));
				cnt--;
			}
			else if(vParam[i].strType == _T("LOOPSTART"))
			{
				vData->push_back(_T("loop start"));
				cnt++;
			}
			else 
				vData->push_back(_T(""));
		}
		last = i;
		return last;
	}

	for(int loop = 0; loop < LoopCnt; loop++)
	{
		while(1)
		{        
			// Loop 처리 - Loop 속에 또 Loop가 있을수 있다
			if( vParam[i].strType == _T("LOOPSTART") )
			{
				int lp = atoi(vData->back());
				vData->push_back(_T("loop start"));
				i = RecvParamLoop(vData, sPData, i+1, lp, vParam);
			}
			else if( vParam[i].strType == _T("LOOPEND") )
			{
				if(loop == 0)
					vData->push_back(_T("loop end"));
				last = i;
				i = start;
				break;
			}
			else
			{
				int pos = sPData->Find(_T("("));
				*sPData = sPData->Mid(pos);
				pos = sPData->Find(_T(")"));
				CString data = sPData->Left(pos);
				data.Trim(_T("()"));
				*sPData = sPData->Mid(pos+1);

				if( loop == 0 )
					vData->push_back(data);
			}
			i++;
		}
	}

	return last;
}
/////////////////////////////////////////////////////////////