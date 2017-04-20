// Session.cpp: implementation of the CSession class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

INT64 g_4skey[KEY_COUNT] = {
	0x5cf173275b82ea90,
	0x4d250f8dd7a79ac1,
	0x336c3aebf71a8b08,
	0x99c1df70b1cdd70f,
	0x4f3965cd539f3bdb,
	0x0969b18cb9330712,
	0xab97253c5fbb8b06};

CSession::CSession()
{
	memset( &m_addr, 0, sizeof(SOCKADDR_IN));
	InitializeCriticalSectionAndSpinCount(&m_cs, 4000);

	m_sock = INVALID_SOCKET;
	m_bValid = FALSE;
	m_dwRecvNumber = 0;
	m_dwSendNumber = 0;

	while(!m_qSEND.empty())
	{
		delete m_qSEND.front();
		m_qSEND.pop();
	}

	m_bUseCrypt = FALSE;
	m_bClosing = FALSE;
	m_bCanRecv = TRUE;
	m_bCanSend = TRUE;
	m_bSessionType = SESSION_SERVER;
	m_dwBufSize = 0;
	m_dwQRead = 0;
	m_bBufFull = FALSE;
	m_dwAcceptTick = 0;
	m_dwCloseTick = 0;

	m_Recv.m_bType = PACKET_RECV;

	m_ovRECV.m_bTYPE = TOV_SSN_RECV;
	m_ovRECV.m_pOBJ = this;

	m_ovSEND.m_bTYPE = TOV_SSN_SEND;
	m_ovSEND.m_pOBJ = this;

	m_ovCONN.m_bTYPE = TOV_SSN_CONN;
	m_ovCONN.m_pOBJ = this;
}

CSession::~CSession()
{
	Close();
	DeleteCriticalSection(&m_cs);
}

void CSession::Encrypt(CPacket * pPacket)
{
	if(!m_bUseCrypt)
		return;

	pPacket->m_pHeader->m_dwNumber = ++m_dwSendNumber;
	INT64 key = g_4skey[pPacket->m_pHeader->m_dwNumber % KEY_COUNT];
	pPacket->Encrypt(key);
	pPacket->EncryptHeader(key);
}

BOOL CSession::Decrypt(CPacket * pPacket)
{
	if(!m_bUseCrypt)
		return TRUE;

	m_dwRecvNumber++;
	INT64 key = g_4skey[m_dwRecvNumber % KEY_COUNT];

	pPacket->DecryptHeader(key);

	if(pPacket->m_pHeader->m_dwNumber != m_dwRecvNumber)
		return FALSE;

	return pPacket->Decrypt(key);
}

void CSession::Close()
{
	EnterCriticalSection(&m_cs);
	m_bValid = FALSE;

	if( INVALID_SOCKET != m_sock )
		closesocket(m_sock);
	m_sock = INVALID_SOCKET;

	memset( &m_addr, 0, sizeof(SOCKADDR_IN));
	m_Recv.Clear();

	while(!m_qSEND.empty())
	{
		delete m_qSEND.front();
		m_qSEND.pop();
	}
	m_bCanSend = TRUE;

	m_dwBufSize = 0;
	m_dwQRead = 0;
	LeaveCriticalSection(&m_cs);
}

BOOL CSession::CreateSocket( SOCKET &socket)
{
	if( socket != INVALID_SOCKET )
		closesocket(socket);

	socket = WSASocket(
		AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP,
		NULL, 0,
		WSA_FLAG_OVERLAPPED);

	if( socket == INVALID_SOCKET )
		return FALSE;
	BOOL bAsync = TRUE;

	int nERROR = ioctlsocket( socket, FIONBIO, (unsigned long *) &bAsync);
	int nSIZE = 1024*64; // Socket buffer size

	if( nERROR == SOCKET_ERROR )
	{
		closesocket(socket);
		socket = INVALID_SOCKET;

		return FALSE;
	}

	nERROR = setsockopt( socket, SOL_SOCKET, SO_RCVBUF, (const char *) &nSIZE, sizeof(int));
	if( nERROR == SOCKET_ERROR )
	{
		closesocket(socket);
		socket = INVALID_SOCKET;

		return FALSE;
	}

	nSIZE = 0;
	nERROR = setsockopt( socket, SOL_SOCKET, SO_SNDBUF, (const char *) &nSIZE, sizeof(int));
	if( nERROR == SOCKET_ERROR )
	{
		closesocket(socket);
		socket = INVALID_SOCKET;

		return FALSE;
	}

	return TRUE;
}

BOOL CSession::Listen( WORD wPort)
{
	SOCKADDR_IN addr;

	if(!CreateSocket(m_sock))
		return FALSE;

	memset( &addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(wPort);

	int nERROR = bind( m_sock, (sockaddr *) &addr, sizeof(SOCKADDR_IN));
	if( nERROR == SOCKET_ERROR )
	{
		Close();
		return FALSE;
	}

	nERROR = listen( m_sock, 5);
	if( nERROR == SOCKET_ERROR )
	{
		Close();
		return FALSE;
	}

	return TRUE;
}

BOOL CSession::Open( SOCKET sock, CPacket &init)
{
	SOCKADDR_IN *pAddr = (SOCKADDR_IN *) (init.GetBuffer() + 38);

	memcpy( &m_addr, pAddr, sizeof(SOCKADDR_IN));
	m_bClosing = FALSE;
	m_bCanRecv = TRUE;
	m_bCanSend = TRUE;
	m_bValid = TRUE;
	m_sock = sock;

	return TRUE;
}

BOOL CSession::SendQueueFull()
{
	WORD wMaxQ;

	if(m_bSessionType == SESSION_CLIENT)
		wMaxQ = 4096;
	else
		return FALSE;

	if(m_qSEND.size() > wMaxQ)
	{
		m_bBufFull = TRUE;
		return TRUE;
	}

	return FALSE;
}

BOOL CSession::OnInvalidSession()
{
	SMART_LOCKCS(&m_cs)
	if(m_bCanSend)
	{
		Close();
		return TRUE;
	}
	m_bValid = FALSE;

	return FALSE;
}

BOOL CSession::CloseSession()
{
	SMART_LOCKCS(&m_cs)
	if( m_sock == INVALID_SOCKET )
		return TRUE;

	closesocket(m_sock);
	m_sock = INVALID_SOCKET;
	m_bCanRecv = FALSE;

	return FALSE;
}

BOOL CSession::SendComplete( DWORD dwTRANS)
{
	SMART_LOCKCS(&m_cs)

	OnSendComplete(dwTRANS);

	if( !m_bValid && m_bCanSend )
	{
		Close();
		return TRUE;
	}

	if( !m_bCanRecv && m_bCanSend )
	{
		if( m_sock != INVALID_SOCKET )
			closesocket(m_sock);
		m_sock = INVALID_SOCKET;
	}

	return FALSE;
}

BOOL CSession::OnSendComplete( DWORD dwTRANS)
{
	SMART_LOCKCS(&m_cs)

	if(m_bCanSend)
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

BOOL CSession::Say( CPacket *pPacket)
{
	SMART_LOCKCS(&m_cs)

	if( !m_bCanRecv ||
		!m_bValid ||
		INVALID_SOCKET == m_sock ||
		SendQueueFull())
	{
		delete pPacket;
		return FALSE;
	}

	Encrypt(pPacket);
	m_qSEND.push(pPacket);

	return Post();
}

BOOL CSession::Post()
{
	if( INVALID_SOCKET == m_sock )
		return FALSE;

	if( m_qSEND.empty() && m_dwBufSize == 0 )
		return TRUE;

	if( !m_bCanSend || !m_bCanRecv)
		return TRUE;

	while( !m_qSEND.empty() && m_dwBufSize < SEND_BUF_SIZE )
	{
		CPacket *pPacket = m_qSEND.front();

		LPBYTE pSRC = pPacket->GetBuffer() + m_dwQRead;
		LPBYTE pDEST = m_pBUF + m_dwBufSize;

		DWORD dwSRC = pPacket->GetSize() - m_dwQRead;
		DWORD dwDEST = SEND_BUF_SIZE - m_dwBufSize;
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
		else
			break;
	}

	m_vBUFSEND.buf = (char *) m_pBUF;
	m_vBUFSEND.len = m_dwBufSize;

	m_bCanSend = FALSE;

	if( WSASend( m_sock, &m_vBUFSEND, 1, &m_dwSENT,
		0, (LPOVERLAPPED) &m_ovSEND, NULL) == SOCKET_ERROR )
	{
		int nERROR = WSAGetLastError();

		if( nERROR != WSA_IO_PENDING)
		{
			m_bCanSend = TRUE;
			m_bBufFull = TRUE;
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CSession::WaitForMessage()
{
	SMART_LOCKCS(&m_cs)
	if( INVALID_SOCKET == m_sock || !m_bValid || !m_bCanRecv )
		return FALSE;

	DWORD dwFlags = 0;
	DWORD dwRecv = 0;

	if(m_Recv.IsReadBufferFull())
		m_Recv.ExpandIoBuffer(m_Recv.GetSize());

	memset( &m_vBUF, 0, sizeof(WSABUF));
	m_vBUF.buf = (char *) (m_Recv.GetBuffer() + m_Recv.GetReadBytes());
	m_vBUF.len = m_Recv.m_dwBufferSize - m_Recv.GetReadBytes();

	if( WSARecv(
		m_sock,
		&m_vBUF, 1,
		&dwRecv,
		&dwFlags,
		(LPOVERLAPPED) &m_ovRECV,
		NULL))
	{
		DWORD dwERROR = WSAGetLastError();

		if( dwERROR != ERROR_IO_PENDING )
			return FALSE;
	}

	return TRUE;
}

BOOL CSession::Read( DWORD dwRead)
{
	SMART_LOCKCS(&m_cs)
	if( INVALID_SOCKET == m_sock || !m_bValid || !m_bCanRecv )
		return FALSE;

	if( dwRead == 0 )
		return FALSE;

	return m_Recv.ReadBytes(dwRead);
}

int CSession::CheckMessage()
{
	SMART_LOCKCS(&m_cs)
	if( m_sock == INVALID_SOCKET || !m_bValid || !m_bCanRecv )
		return PACKET_INVALID;

	if( m_Recv.GetReadBytes() < PACKET_HEADER_SIZE ||
		m_Recv.GetReadBytes() < m_Recv.GetSize() )
		return PACKET_INCOMPLETE;

	if( m_Recv.GetSize() < PACKET_HEADER_SIZE ||
		m_Recv.GetSize() == MAX_PACKET_SIZE)
		return PACKET_INVALID;

	if(m_bSessionType == SESSION_CLIENT &&
		!Decrypt(&m_Recv))
		return PACKET_INVALID;

	return PACKET_COMPLETE;
}

void CSession::Flush()
{
	SMART_LOCKCS(&m_cs)
	m_Recv.Flush();
}

BOOL CSession::Connect( LPCTSTR lpszAddr, WORD wPort, LPCTSTR lpszMyAddr)
{
	if(!CreateSocket(m_sock))
		return FALSE;

	if(lstrlen(lpszMyAddr))
	{
		SOCKADDR_IN addr;
		memset( &addr, 0, sizeof(SOCKADDR_IN));
		addr.sin_family = AF_INET;
		addr.sin_port = 0;
		addr.sin_addr.s_addr = inet_addr(lpszMyAddr);

		int nError = bind( m_sock, (sockaddr *) &addr, sizeof(SOCKADDR_IN));
		if(nError == SOCKET_ERROR )
		{
			Close();
			return FALSE;
		}
	}

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = inet_addr(lpszAddr);
	m_addr.sin_port = htons((u_short) wPort);

	int nERROR = connect( m_sock, (sockaddr *) &m_addr, sizeof(SOCKADDR_IN));
	if( SOCKET_ERROR == nERROR )
	{
		if( WSAGetLastError() == WSAEWOULDBLOCK )
		{
			fd_set fds;
			timeval timeout = { 2, 0};

			FD_ZERO(&fds);
			FD_SET( m_sock, &fds);

			nERROR = select( 0, NULL, &fds, NULL, &timeout);
			if( nERROR <= 0 )
			{
				Close();
				return FALSE;
			}
		}
		else
		{
			Close();
			return FALSE;
		}
	}

	m_bClosing = FALSE;
	m_bCanRecv = TRUE;
	m_bCanSend = TRUE;
	m_bValid = TRUE;

	return TRUE;
}

BOOL CSession::ConnectEx(LPCTSTR lpszAddr, WORD wPort, HANDLE CompletionPort, DWORD dwCompletionKey, DWORD dwNumberOfConcurrentThreads)
{
	if(!CreateSocket(m_sock))
		return FALSE;

	SOCKADDR_IN addr;
	memset( &addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_addr.s_addr = INADDR_ANY;

	int nError = bind( m_sock, (sockaddr *) &addr, sizeof(SOCKADDR_IN));
	if(nError == SOCKET_ERROR )
	{
		Close();
		return FALSE;
	}

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = inet_addr(lpszAddr);
	m_addr.sin_port = htons((u_short) wPort);

	DWORD dwBytes = 0;
	LPFN_CONNECTEX lpfnConnectEx = NULL; 
	GUID GuidConnectEx = WSAID_CONNECTEX;
	
	nError = WSAIoctl(
		m_sock,
        SIO_GET_EXTENSION_FUNCTION_POINTER,
        &GuidConnectEx,
        sizeof(GuidConnectEx),
        (LPVOID)&lpfnConnectEx,
		sizeof(lpfnConnectEx),
        &dwBytes,
        NULL,
        NULL);

	if(nError == SOCKET_ERROR)
	{
		Close();
		return FALSE;
	}

	HANDLE hIOCP = CreateIoCompletionPort(
		(HANDLE)m_sock,
		CompletionPort,
		dwCompletionKey,
		dwNumberOfConcurrentThreads);

	if(!hIOCP)
	{
		Close();
		return FALSE;
	}

	if(!lpfnConnectEx(m_sock, (sockaddr *)&m_addr, sizeof(SOCKADDR_IN), NULL, 0, NULL, (LPOVERLAPPED)&m_ovCONN))
	{
		nError = WSAGetLastError();
		if(nError != WSA_IO_PENDING)
		{
			Close();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CSession::OnConnect()
{
	if(!setsockopt(m_sock, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0 ))
	{
		m_bClosing = FALSE;
		m_bCanRecv = TRUE;
		m_bCanSend = TRUE;
		m_bValid = TRUE;
	}
	else
		return FALSE;

	return TRUE;
}
