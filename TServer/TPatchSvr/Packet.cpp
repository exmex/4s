// Packet.cpp: implementation of the CPacket class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

//COverlappedEx
COverlappedEx::COverlappedEx()
{
	memset( &m_ov, 0, sizeof(OVERLAPPED));

	m_bTYPE = TOV_SSN_RECV;
	m_pOBJ = NULL;
}

COverlappedEx::~COverlappedEx()
{
}


//CPacket
CPacket::CPacket()
{
	m_dwBufferSize = 0;
	m_pHeader = NULL;
	m_pBuf = NULL;
	m_bType = PACKET_SEND;

	ExpandIoBuffer(-1);
	Clear();
}

CPacket::~CPacket()
{
	if( m_pBuf && m_dwBufferSize )
		delete[] m_pBuf;
}

DWORD CPacket::ExpandIoBuffer( DWORD dwNewSize)
{
	DWORD dwPrevOffset = PACKET_HEADER_SIZE;
	DWORD dwExpandSize;
	DWORD dwAddSize =0;

	LPBYTE pExpand = NULL;

	if( dwNewSize != -1 )
		dwExpandSize = dwNewSize;
	else
		dwExpandSize = m_dwBufferSize + DEF_PACKET_SIZE;

	if( dwExpandSize <= m_dwBufferSize )
		return dwAddSize;

	pExpand = new BYTE[dwExpandSize];
	memset(pExpand, 0, sizeof(BYTE) * dwExpandSize);

	if(m_pBuf)
	{
		memcpy( pExpand, m_pBuf, m_dwBufferSize);
		dwPrevOffset = DWORD(m_ptrOffset - m_pBuf);

		delete[] m_pBuf;
	}

	m_dwBufferSize = dwExpandSize;
	m_pBuf = pExpand;

	m_pHeader = (LPPACKETHEADER) pExpand;
	m_ptrOffset = m_pBuf + dwPrevOffset;

	return dwAddSize;
}

void CPacket::Clear()
{
	memset( m_pBuf, 0, m_dwBufferSize);
	m_dwReadBytes = 0;

	if(m_bType == PACKET_SEND)
		m_pHeader->m_wSize = PACKET_HEADER_SIZE;

	m_ptrOffset = m_pBuf + PACKET_HEADER_SIZE;
}

LPBYTE CPacket::GetBuffer()
{
	return m_pBuf;
}

void CPacket::Flush()
{
	DWORD dwTrashSize = GetSize();

	if( m_dwReadBytes < dwTrashSize )
	{
		Clear();
		return;
	}

	m_dwReadBytes -= dwTrashSize;
	m_ptrOffset = m_pBuf + PACKET_HEADER_SIZE;

	memcpy( m_pBuf, m_pBuf + dwTrashSize, m_dwReadBytes);
}

BOOL CPacket::IsReadBufferFull()
{
	return m_dwReadBytes >= PACKET_HEADER_SIZE && GetSize() > m_dwBufferSize;
}

BOOL CPacket::IsEOF()
{
	return !CanRead(1);
}

BOOL CPacket::CanRead( DWORD length)
{
	DWORD dwReadBytes = DWORD(m_ptrOffset - m_pBuf) + length;
	WORD wPacketSize = GetSize();

	return m_dwBufferSize >= dwReadBytes && wPacketSize >= dwReadBytes;
}

BOOL CPacket::CanWrite( DWORD length)
{
	return m_dwBufferSize >= GetSize() + length;
}

WORD CPacket::GetID()
{
	return m_pHeader ? m_pHeader->m_wID : 0xFFFF;
}

CPacket& CPacket::SetID( WORD wID)
{
	if(m_pHeader)
		m_pHeader->m_wID = wID;

	return *this;
}

WORD CPacket::GetSize()
{//Packet size, it does not mead the size of valid data or buffer
	return m_pHeader ? m_pHeader->m_wSize : 0xFFFF;
}

void CPacket::Read( LPVOID param, int nLength)
{
	if(CanRead(nLength))
	{
		memcpy( param, m_ptrOffset, nLength);
		m_ptrOffset += nLength;
	}
}

void CPacket::Write( LPVOID param, int nLength)
{
	if(m_pHeader->m_wSize >= MAX_PACKET_SIZE)
		return;

	if(!CanWrite(nLength))
		ExpandIoBuffer(m_dwBufferSize + max(nLength, DEF_PACKET_SIZE));

	memcpy( m_ptrOffset, param, nLength);
	m_ptrOffset += nLength;

	if(m_pHeader->m_wSize + nLength > MAX_PACKET_SIZE)
		m_pHeader->m_wSize = MAX_PACKET_SIZE;
	else
		m_pHeader->m_wSize += nLength;
}

void CPacket::Copy( CPacket *pMsg)
{
	if(!pMsg)
		return;

	DWORD dwPacketSize = pMsg->GetSize();
	if( dwPacketSize >= MAX_PACKET_SIZE )
		return;

	if( m_dwBufferSize < dwPacketSize )
		ExpandIoBuffer(dwPacketSize);

	Clear();

	memcpy( m_pBuf, pMsg->m_pBuf, dwPacketSize);
	m_ptrOffset = m_pBuf + dwPacketSize;
}

void CPacket::CopyData(CPacket * pMsg, WORD wDeleteSize)
{
	if(!pMsg || pMsg->GetSize() < wDeleteSize + PACKET_HEADER_SIZE)
		return;

	WORD wAddSize = pMsg->GetSize() - PACKET_HEADER_SIZE - wDeleteSize;

	if( m_dwBufferSize < DWORD(GetSize() + wAddSize) )
		ExpandIoBuffer(GetSize() + wAddSize);

	memcpy( m_ptrOffset, pMsg->m_pBuf+PACKET_HEADER_SIZE+wDeleteSize, wAddSize);
	m_pHeader->m_wSize += wAddSize;
	m_ptrOffset += wAddSize;
}

void CPacket::AddData(CPacket * pMsg)
{
	if(!pMsg)
		return;

	WORD wDataSize = pMsg->GetSize() - PACKET_HEADER_SIZE;
	if( wDataSize+GetSize() >= MAX_PACKET_SIZE )
		return;

	if( m_dwBufferSize < DWORD(wDataSize + GetSize())  )
		ExpandIoBuffer(wDataSize + GetSize());

	memcpy( m_ptrOffset, pMsg->m_pBuf+PACKET_HEADER_SIZE, wDataSize);
	m_pHeader->m_wSize += wDataSize;
	m_ptrOffset += wDataSize;
}

int CPacket::DetachBinary( LPVOID *ptr)
{
	int nLength;

	Read( &nLength, sizeof(DWORD));
	if( nLength <= 0 || !CanRead(nLength) )
	{
		*ptr = NULL;
		return 0;
	}

	*ptr = new BYTE[nLength];
	Read( *ptr, nLength);

	return nLength;
}

void CPacket::AttachBinary( LPVOID param, int nLength)
{
	Write( &nLength, sizeof(int));
	Write( param, nLength);
}

DWORD CPacket::GetReadBytes()
{
	return m_dwReadBytes;
}

BOOL CPacket::ReadBytes( DWORD dwReadBytes)
{
	DWORD dwTotalBytes = m_dwReadBytes + dwReadBytes;

	if( dwTotalBytes > m_dwBufferSize)
		return FALSE;

	m_dwReadBytes += dwReadBytes;
	return TRUE;
}

CPacket& CPacket::operator << ( DWORD param)
{
	Write( (LPVOID) &param, sizeof(DWORD));
	return *this;
}

CPacket& CPacket::operator << ( WORD param)
{
	Write( (LPVOID) &param, sizeof(WORD));
	return *this;
}

CPacket& CPacket::operator << ( LPCTSTR param)
{
	int nLength = lstrlen(param);

	Write( (LPVOID) &nLength, sizeof(int));
	Write( (LPVOID) param, nLength);

	return *this;
}

CPacket& CPacket::operator << ( short param)
{
	Write( (LPVOID) &param, sizeof(short));
	return *this;
}

CPacket& CPacket::operator << ( long param)
{
	Write( (LPVOID) &param, sizeof(long));
	return *this;
}

CPacket& CPacket::operator << ( int param)
{
	Write( (LPVOID) &param, sizeof(int));
	return *this;
}

CPacket& CPacket::operator << ( char param)
{
	Write( (LPVOID) &param, sizeof(char));
	return *this;
}

CPacket& CPacket::operator << ( BYTE param)
{
	Write( (LPVOID) &param, sizeof(BYTE));
	return *this;
}

CPacket& CPacket::operator << ( float param)
{
	Write( (LPVOID)&param, sizeof(float));
	return *this;
}

CPacket& CPacket::operator << ( __int64 param)
{
	Write( (LPVOID) &param, sizeof(__int64));
	return *this;
}

CPacket& CPacket::operator >> ( DWORD& param)
{
	Read( (LPVOID) &param, sizeof(DWORD));
	return *this;
}

CPacket& CPacket::operator >> ( WORD& param)
{
	Read( (LPVOID) &param, sizeof(WORD));
	return *this;
}

CPacket& CPacket::operator >> ( CString& param)
{
	int nLength;

	Read( (LPVOID) &nLength, sizeof(int));
	if(CanRead(nLength))
	{
		LPTSTR buff = new char[nLength + 1];

		memset( buff, '\0', nLength + 1);
		Read( buff, nLength);
		param = buff;

		delete[] buff;
	}

	return *this;
}

CPacket& CPacket::operator >> ( short& param)
{
	Read( (LPVOID) &param, sizeof(short));
	return *this;
}

CPacket& CPacket::operator >> ( long& param)
{
	Read( (LPVOID) &param, sizeof(long));
	return *this;
}

CPacket& CPacket::operator >> ( int& param)
{
	Read( (LPVOID) &param, sizeof(int));
	return *this;
}

CPacket& CPacket::operator >> ( char& param)
{
	Read( (LPVOID) &param, sizeof(char));
	return *this;
}

CPacket& CPacket::operator >> ( BYTE& param)
{
	Read( (LPVOID) &param, sizeof(BYTE));
	return *this;
}

CPacket& CPacket::operator >> ( float& param)
{
	Read( (LPVOID) &param, sizeof(float));
	return *this;
}

CPacket& CPacket::operator >> ( __int64& param)
{
	Read( (LPVOID) &param, sizeof(__int64));
	return *this;
}

void CPacket::Rewind(BOOL bWrite)
{
	if(bWrite)
		m_pHeader->m_wSize = PACKET_HEADER_SIZE;
	m_ptrOffset = m_pBuf + PACKET_HEADER_SIZE;
}
