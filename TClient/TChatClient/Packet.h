// Packet.h: interface for the CPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKET_H__1395175B_A586_48E3_AE3D_93286AA78DE2__INCLUDED_)
#define AFX_PACKET_H__1395175B_A586_48E3_AE3D_93286AA78DE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//PACKET_HEADER
//  WORD	wSize
//	WORD	wMsgID
//	DWORD	dwChkSUM

typedef struct _tagPACKETHEADER							PACKETHEADER, *LPPACKETHEADER;


#define PACKET_HEADER_SIZE								sizeof(PACKETHEADER)
#define DEF_PACKET_SIZE									(1024)
#define MAX_PACKET_SIZE									(1024 * 64)

#define	TOV_SSN_RECV									((BYTE)0x01)
#define TOV_SSN_SEND									((BYTE)0x02)

#define PACKET_COMPLETE									(0)
#define PACKET_INCOMPLETE								(-1)
#define PACKET_INVALID									(1)


struct _tagPACKETHEADER
{
	WORD m_wSize;
	WORD m_wID;

	DWORD m_dwNumber;
	INT64 m_llChkSUM;
};


class COverlappedEx
{
public:
	OVERLAPPED m_ov;

	LPVOID m_pOBJ;
	BYTE m_bTYPE;

	COverlappedEx();
	virtual ~COverlappedEx();
};


class CPacket
{
public:
	LPPACKETHEADER m_pHeader;

	LPBYTE m_ptrOffset;
	LPBYTE m_pBuf;

	DWORD m_dwBufferSize;
	DWORD m_dwReadBytes;

public:
	CPacket();
	virtual ~CPacket();

public:
	void Encrypt(INT64 key);
	BOOL Decrypt(INT64 key);

	void EncryptHeader(INT64 key);
	BYTE DecryptHeader(INT64 key);

	void ExpandIoBuffer( DWORD dwNewSize = -1);
	void Rewind( BOOL bWrite);

	void Clear();
	void Flush();

	CPacket& SetID( WORD wID);
	LPBYTE GetBuffer();

	BOOL Receive( HANDLE sock, LPOVERLAPPED lpovRecv);
	BOOL ReadBytes( DWORD dwReadBytes);
	BOOL CanWrite( DWORD length);
	BOOL CanRead( DWORD length);
	BOOL IsReadBufferFull();
	BOOL IsEOF();

	DWORD GetReadBytes();
	WORD GetSize();
	WORD GetID();

	void Write( LPVOID param, int nLength);
	CPacket& operator << ( LPCTSTR param);
	CPacket& operator << ( short param);
	CPacket& operator << ( long param);
	CPacket& operator << ( DWORD param);
	CPacket& operator << ( WORD param);
	CPacket& operator << ( BYTE param);
	CPacket& operator << ( char param);
	CPacket& operator << ( int param);
	CPacket& operator << ( float param);
	CPacket& operator << ( __int64 param);

	void Read( LPVOID param, int nLength);
#ifdef _AFX
	CPacket& operator >> ( CString& param);
#else
	CPacket& operator >> ( LPCTSTR& param);
#endif
	CPacket& operator >> ( short& param);
	CPacket& operator >> ( long& param);
	CPacket& operator >> ( DWORD& param);
	CPacket& operator >> ( WORD& param);
	CPacket& operator >> ( BYTE& param);
	CPacket& operator >> ( char& param);
	CPacket& operator >> ( int& param);
	CPacket& operator >> ( float& param);
	CPacket& operator >> ( __int64& param);

	void Copy( CPacket *pMsg);
	void AttachBinary( LPVOID param, int nLength);
	int DetachBinary( LPVOID *ptr); //Caller should delete the ptr
	int DetachBinary( LPVOID ptr); //Caller should delete the ptr
};


#endif // !defined(AFX_PACKET_H__1395175B_A586_48E3_AE3D_93286AA78DE2__INCLUDED_)
