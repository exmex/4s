#pragma once

#define HP_RECEIVE(x)			case x: On##x(pPacket); break;
typedef queue<CPacket *>		QPACKET, *LPQPACKET;

// CHSession 명령 대상입니다.
class CHappyDoc;

class CHSession : public CAsyncSocket
{
public:
	CRITICAL_SECTION m_cs;
	QPACKET m_qSEND;
	CPacket m_packet;
	CPacket * m_pSending;

	BOOL m_bValid;
	int					m_nWroteBytes;
	int					m_nSendBufferSize;
	CHappyDoc *			m_pOwner;

public:
	void SetOwner(CHappyDoc * pOwner);
	BOOL SendPacket(CPacket * pPacket);
	BOOL IsValid();
	void Say(CPacket *pPacket);
	void End();
	BOOL Start(LPCTSTR strAddr, DWORD dwPort);

public:
	CHSession();
	virtual ~CHSession();
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
};
