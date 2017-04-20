// Session.h : interface for the CSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SESSION_H__DDA198A6_A9AA_4CD6_BD4C_23C594C1306F__INCLUDED_)
#define AFX_SESSION_H__DDA198A6_A9AA_4CD6_BD4C_23C594C1306F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef queue<CPacket *>						QPACKET, *LPQPACKET;

#define SEND_BUF_SIZE							(1024 * 64)

#define RECV_CLI_SIZE							(1024 * 64)
#define RECV_SVR_SIZE							(1024 * 2000)

#define SESSION_CLIENT							1
#define SESSION_SERVER							2

#define KEY_COUNT								((BYTE) 7)

class CSession
{
private:
	DWORD m_dwRecvNumber;
	DWORD m_dwSendNumber;
	void Encrypt(CPacket * pPacket);
	BOOL Decrypt(CPacket * pPacket);

public:
	BYTE m_bUseCrypt;
	BYTE m_bSessionType;
	BYTE m_bCanRecv;
	BYTE m_bCanSend;
	BYTE m_bClosing;
	BOOL m_bValid;
	BYTE m_bBufFull;

	DWORD m_dwAcceptTick;
	DWORD m_dwCloseTick;

	CRITICAL_SECTION m_cs;

public:
	SOCKADDR_IN m_addr;			//Remote address
	CPacket m_Recv;				//Receive packet

	QPACKET m_qSEND;
	SOCKET m_sock;
	WSABUF m_vBUF;
	WSABUF m_vBUFSEND;
	DWORD m_dwSENT;

	BYTE  m_pBUF[SEND_BUF_SIZE];

	DWORD m_dwBufSize;
	DWORD m_dwQRead;

	COverlappedEx m_ovSEND;
	COverlappedEx m_ovRECV;
	COverlappedEx m_ovCONN;

public:
	static BOOL CreateSocket( SOCKET &socket);

	BOOL ConnectEx(LPCTSTR lpszAddr, WORD wPort, HANDLE CompletionPort, DWORD dwCompletionKey, DWORD dwNumberOfConcurrentThreads);
	BOOL Connect( LPCTSTR lpszAddr, WORD wPort, LPCTSTR lpszMyAddr=NULL);
	BOOL Listen( WORD wPort);
	BOOL Read( DWORD dwRead);

	virtual BOOL Say( CPacket *pPacket);

	BOOL Post();
	BOOL SendQueueFull();

	BOOL SendComplete( DWORD dwTRANS);
	BOOL OnInvalidSession();
	BOOL CloseSession();

	BOOL OnSendComplete( DWORD dwTRANS);
	BOOL OnConnect();
	BOOL WaitForMessage();

	int CheckMessage();

	void ClearBuffer();
	void Flush();

public:
	virtual BOOL Open( SOCKET sock, CPacket& init);
	virtual void Close();

public:
	CSession();
	virtual ~CSession();
};

#endif // !defined(AFX_SESSION_H__DDA198A6_A9AA_4CD6_BD4C_23C594C1306F__INCLUDED_)
