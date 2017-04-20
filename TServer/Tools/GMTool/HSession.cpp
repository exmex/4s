// HSession.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "HSession.h"
#include ".\hsession.h"
#include "GMToolDoc.h"

// CHSession

CHSession::CHSession()
{
	m_pOwner = NULL;
	m_bValid = FALSE;
	m_nWroteBytes = m_nSendBufferSize = 0;
	InitializeCriticalSection(&m_cs);
}

CHSession::~CHSession()
{
	End();
	DeleteCriticalSection(&m_cs);
}


// CHSession 멤버 함수입니다.
void CHSession::SetOwner(CGMToolDoc *pOwner)
{
	m_pOwner = pOwner;
}
BOOL CHSession::Start(LPCTSTR strAddr, DWORD dwPort)
{
	End();

	if(!Create())
		return FALSE;

	int nSIZE = 1024 * 1000;

	SetSockOpt(SO_RCVBUF, (const char *) &nSIZE, sizeof(int));

	if(!Connect(strAddr, dwPort))
	{
		int err = GetLastError();
		if( err != WSAEWOULDBLOCK )
		{
			End();
			return FALSE;
		}
	}
	return TRUE;
}
void CHSession::End()
{
	m_bValid = FALSE;

	while(!m_qSEND.empty())
	{
		delete m_qSEND.front();
		m_qSEND.pop();
	}

	Close();	
}
void CHSession::Say(CPacket *pPacket)
{
	if(!IsValid())
	{
		delete pPacket;
		return;
	}

	m_qSEND.push(pPacket);

	EnterCriticalSection(&m_cs);
	SendPacket(	m_qSEND.front());
	LeaveCriticalSection(&m_cs);
}
BOOL CHSession::SendPacket(CPacket *pPacket)
{//
	if(!pPacket)
		return FALSE;
	
	m_nSendBufferSize = pPacket->GetSize();	
	
	while( m_nWroteBytes < m_nSendBufferSize )
	{
		int nWrote = Send(pPacket->GetBuffer() + m_nWroteBytes, m_nSendBufferSize - m_nWroteBytes);
		if(nWrote != SOCKET_ERROR)
			m_nWroteBytes += nWrote;
		else
		{
			int err = GetLastError();
			if(err == WSAEWOULDBLOCK)
				return FALSE;
			else
			{
				return FALSE;
			}
		}
	}

	m_qSEND.pop();
	delete pPacket;
	m_nWroteBytes = m_nSendBufferSize = 0;

	return TRUE;
}

BOOL CHSession::IsValid()
{
	return m_bValid;
}
void CHSession::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CAsyncSocket::OnAccept(nErrorCode);
}

void CHSession::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_bValid = FALSE;
	m_pOwner->OnClose(nErrorCode);
	End();
	CAsyncSocket::OnClose(nErrorCode);
}

void CHSession::OnConnect(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(!m_pOwner) return;

	if( nErrorCode == 0)
	{
		m_bValid = TRUE;
	}
	m_pOwner->OnConnect(nErrorCode);

	CAsyncSocket::OnConnect(nErrorCode);
}

void CHSession::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(nErrorCode == 0)
	{
		if(m_packet.IsReadBufferFull()) m_packet.ExpandIoBuffer();

		int nRead = Receive( 
			m_packet.GetBuffer() + m_packet.GetReadBytes(), 
			m_packet.m_dwBufferSize - m_packet.GetReadBytes());	

		if( !m_packet.ReadBytes( nRead )) return;

		DWORD rb = m_packet.GetReadBytes();
		WORD s = m_packet.GetSize();

		while(0 != m_packet.GetReadBytes() && 
			m_packet.GetReadBytes() >= m_packet.GetSize() )
		{
			m_pOwner->OnReceive(&m_packet);
			m_packet.Flush();
		}
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void CHSession::OnSend(int nErrorCode)
{
	EnterCriticalSection(&m_cs);
	while(!m_qSEND.empty())
	{
		if(!SendPacket(m_qSEND.front()))
			break;
	}
	LeaveCriticalSection(&m_cs);

	CAsyncSocket::OnSend(nErrorCode);
}
