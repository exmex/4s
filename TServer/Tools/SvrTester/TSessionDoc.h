// TSessionDoc.h : CTSessionDoc 클래스의 인터페이스
//


#pragma once

#include "TachyonSession.h"

#include "STType.h"

#define ON_RECEIVE(x)							case x	: On##x(pPacket); break;

class CTSessionDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CTSessionDoc();
	DECLARE_DYNCREATE(CTSessionDoc)

// 특성
public:

// 작업
public:

// 재정의
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현
public:
	virtual ~CTSessionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()

public:
	CTachyonSession* m_pSession; // Session
	WSADATA wsaData; // socket

	///////////////////////////////////////////////////////////////////
	// Manager Svr
	CString m_strUser;
	CString m_strPass;
	
	// CTControlSvr Connect / DisConnect
	void CTSvrConnect(CString strIP, DWORD wPort, CString strUser, CString strPass); 
	void CTSvrDisConnect(); 

	// Manager Svr Message
	void SendCT_STLOGIN_REQ(CString strUser, CString strPass);
	void OnCT_STLOGIN_ACK(CPacket * pPacket);

	BYTE m_bAuthority; // Manager Authority
	BYTE m_bManagerConnect; // Manager Connect Flag
	///////////////////////////////////////////////////////////////////

	BYTE m_bRecvMessage; // Receive Message Flag

public:
	///////////////////////////////////////////////////////////////////
	HWND m_HWND;
    HWND GetSafeHwnd(); // HWND return (TachyonSession)

	// TachyonSession Message
	BOOL OnSessionMsg(CTachyonSession *pSession, CPacket *pPacket);
	void OnSessionClose(DWORD dwID);
	void OnConnect(CTachyonSession *pSession, int nErrorCode);
	void OnSend(CTachyonSession *pSession, int nErrorCode);
	void OnClose(CTachyonSession *pSession, int nErrorCode);
	BYTE Say(CPacket * pPacket); // Manager Say
	void SayToOne(CPacket * pPacket, int nID, DWORD pID, CString strParam); // Protocol One Say
	void SayToAll(CPacket * pPacket, DWORD pID, CString strParam); // Protocol All Say	
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	// Sever Connect / DisConnect
	void Connect(CString strIP, CString strPort, CString strThread);
	void DisConnect();

	// Protocol / Type Map
	MAPPROTOCOL m_mapProtocol;
	MAPPROTOCOLTYPE m_mapType;

	// String Type Cast
	BYTE strTobyte(CString str);
	WORD strToword(CString str);
	DWORD strTodword(CString str);
	float strTofloat(CString str);
	//__int64 strToint64(CString str);
	SOCKADDR_IN strToSOCKADDR(CString str);

	// Receive Message Value
	CString ReceiveByte(VECTORTYPE* vLoop, BYTE value);
	CString ReceiveWord(VECTORTYPE* vLoop, WORD value);
	CString ReceiveDword(VECTORTYPE* vLoop, DWORD value);
	CString ReceiveFloat(float value);
	CString ReceiveString(CString value);
	CString ReceiveInt64(__int64 value);
	CString ReceiveSOCKADDR(DWORD value);	

	// Protocol Send
	void Send(DWORD pID, CString strParam, BOOL bAllFlag);
	void SendPacket(CPacket *pMsg, VECTORTYPE v, TCHAR* token);

	// Protocol Receive
	void Receive(CTachyonSession* pSession, DWORD pID, CPacket* pPacket);
	void ReceivePacket(VECTORTYPE vType, CPacket *pPacket, CString* data);
	int ReceivePacketLoop(VECTORTYPE vType, VECTORTYPE* vLoop, CPacket *pPacket, int pos, CString* data);	

    // Push Receive Data List
    void PostPushList(CTachyonSession* pSession, CString strP, CString data);
	// CharInfo Show
	void PostCharinfoView();
	void ShowCharInfo();
	// Current Session List Show
	void PostSessionlistView();
	void ShowCurSessionList();	
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	// Session Create / Start / Close
	void SessionCreate();	
	void SessionStart(CString strIP, DWORD dwPort);		
	void SessionClose();
	void PostSessionClose(CTachyonSession * pSession);
	BYTE m_bRecvCnt;
	BYTE IsRecvAll();

	// Thread
    DWORD m_dwThreadNum; // Thread Num
	
	MAPPSESSION m_mapSession; // Session Map	
	MAPTHREADSESSION m_mapThreadSession; // Thread Map

    BOOL m_bRun; // Thread Run Flag

	// Thread Create / Start / End
	void ThreadCreate();
	void ThreadStart();	
	void ThreadEnd();

	// Thread Run
	static DWORD WINAPI _SessionThread( LPVOID lpParam); 
	DWORD SessionThread();
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	// Macro Flag
	BOOL m_bMacro;	

	void SetMacroFlag(BOOL flag);
	BOOL GetMacroFlag();	
	///////////////////////////////////////////////////////////////////	

	///////////////////////////////////////////////////////////////////	
	// Session Macro Map
	MAPMACRO m_mapSMacro;

	void InsertSMacroMap(DWORD mID, CTMacro* pMacro);
	void ClearSMacroMap();

	// Session Macro Queue
	QUEUEDWORD m_qMID;

	void InsertSMacroQueue(DWORD mID);
	void ClearSMacroQueue();
	///////////////////////////////////////////////////////////////////	

	///////////////////////////////////////////////////////////////////
	// Set Session Macro Info
	void SetSMacroInfo(CTachyonSession* pSession);
	///////////////////////////////////////////////////////////////////	

	///////////////////////////////////////////////////////////////////	
	// Macro Send
	BOOL MacroSend(CTachyonSession* pSession);
	void Send(CTachyonSession* pSession, DWORD pID, CString strParam);	
	///////////////////////////////////////////////////////////////////		
};


