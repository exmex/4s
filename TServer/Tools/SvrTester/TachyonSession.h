#pragma once

#include "Packet.h"
#include "TSessionDoc.h" // Document°¡ Owner

#include "STType.h"

#define ON_TRECEIVE(x)							case x	: nERROR = On##x( pSession, pPacket); break;
#define SESSIONBUF_SIZE							(1024 * 64)
#define KEY_COUNT								((BYTE) 7)

///////////////////////////////////////////////////////////////////
// Thread struct
typedef struct tagSTHREAD STRUCTTHREAD;
struct tagSTHREAD
{
	BYTE type;
    LPARAM lParam;
	CPacket *packet;
	int nError;
};
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// CharInfo struct
typedef struct tagCHARINFO STRUCTCHARINFO;
struct tagCHARINFO
{
	CString UserID;
    CString CharID;
	CString KEY;
	CString GroupID;
	CString Channel;
	CString SvrID;
};
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Thread Type
#define THREAD_TYPE_SEND		1
#define THREAD_TYPE_RECV		2
#define THREAD_TYPE_MAPCONNECT	3
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// List Max
#define LIST_MAX			40
///////////////////////////////////////////////////////////////////

class CTachyonSession
{
public:
	static MAPSESSION m_mapSESSION;

	static CTachyonSession *GetSession( SOCKET sock);
	static void ReleaseTachyonSession();
	static void InitTachyonSession();

public:
	CPacket m_packet;
	QPACKET m_qSEND;

	SOCKADDR_IN m_target;
	SOCKADDR_IN m_source;
	SOCKADDR_IN m_real;
	SOCKADDR_IN m_addr;
	SOCKET m_sock;

	CTSessionDoc *m_pOwner; // Document Owner
	COverlappedEx m_ov;

	DWORD m_dwBufSize;
	DWORD m_dwQRead;
	DWORD m_dwSendNumber;

	BYTE m_pBUF[SESSIONBUF_SIZE];

	BYTE m_bLocalIP;
	BYTE m_bCanSend;
	BYTE m_bValid;
	BYTE m_bType;

public:
	void SetOwner( CTSessionDoc *pOwner);

	BYTE Start( LPCTSTR strAddr, DWORD dwPort, BYTE bType = SOCK_STREAM);
	BYTE SendPacket( CPacket *pPacket);
	BYTE Read( DWORD dwRead);
	BYTE IsValid();

	void SetUDPTarget( SOCKADDR_IN *pADDR);
	void Flush();
	void End();

	void Encrypt( CPacket *pPacket);
	BYTE Say( CPacket *pPacket, SOCKADDR_IN *pADDR);
	BYTE Say( CPacket *pPacket);

	BYTE OnSendComplete();
	BYTE Post();

	int CheckMSG();

	////////////////////////////////////////
	DWORD m_SessionID; // Session ID 
	HANDLE m_hEvent;   // Event Handle
	HANDLE m_hThread;
	DWORD m_dwThreadID;

	void SetID(DWORD SessionID); // Session ID Set
	void SetHandle(HANDLE hEvent); // Event Handle Set

	CRITICAL_SECTION g_cs; // Critical Section

	// Message Queue
	queue<STRUCTTHREAD> m_qMessage; 

	void PushMessage(STRUCTTHREAD sThread); // Message Queue Push
	STRUCTTHREAD FrontMessage(); // Message Queue Front
	void PopMessage(); // Message Queue Pop
	BOOL EmptyMessage(); // Message Queue Empty
	void ClearMessage(); // Message Queue Clear

	// List Deque
	DEQUELIST m_dqList;

    void PushList(CString strProtocol, CString strValue); // List Deque Push
	STRUCTSTRING PopList(int i); // List Deque Pop
	void ListClear(); // List Deque Clear

	// Char Info
	STRUCTCHARINFO m_sCharInfo;

	// UserID / CharID / Key / GroupID / Channel / SvrID Init
	void InitCharInfo();
	// UserID / CharID / Key / GroupID / Channel / SvrID Set	
	void SetCharInfo(CString sProtocol, CString sValue);
	void SetCharInfo(int i, CString sParam);
	// UserID / CharID / Key / GroupID / Channel / SvrID Get
	CString GetUserID();
    CString GetCharID();
	CString GetKEY();
	CString GetGroupID();
	CString GetChannel();
	CString GetSvrID();

	// Map Server Connect
	BOOL m_bLogin;

	CString m_sMapIP;
	WORD m_wMapPort;
	///////////////////////////////////////

	///////////////////////////////////////
	// Macro Info
	QUEUEDWORD m_qID;
	MAPMACRO m_mMacro;

    void SetMacroInfo(QUEUEDWORD qID, MAPMACRO mMacro);
	void ClearMacroInfo();
	BOOL IsEmptyMacro();
	///////////////////////////////////////

	///////////////////////////////////////
	// Set Macro Self Vector
	VECTORSELFINFO m_vSelf;

	void SetSelfVector();
	void ClearSelfVector();
	BOOL IsSelfCheck(DWORD pID, CString sData);	
	///////////////////////////////////////

	///////////////////////////////////////
	// Macro Paramater
	VECTORPARAM m_vParam;

	void SetMacroParam();
	VECTORPARAM GetMacroParam();	
	void UpdateMacroParam(CString sName, CString sType, CString sData);
	///////////////////////////////////////

	///////////////////////////////////////
	// Macro Send Message
	STRUCTPINFO GetSendMsg();

	// Send Count
	int m_SendCnt;

	void InitSendCnt();
	///////////////////////////////////////

	///////////////////////////////////////
	// Set Param Data
	BOOL SetParamData(DWORD pID, CString sData);

	// Param Data
	VECTORSTRING RecvParamData(VECTORPARAM vParam, CString sData);
	int RecvParamLoop(VECTORSTRING* vData, CString* sPData, int i, int LoopCnt, VECTORPARAM vParam);	
	///////////////////////////////////////
public:
	virtual BOOL OnReceive( int nErrorCode);
	virtual void OnConnect( int nErrorCode);
	virtual void OnSend( int nErrorCode);
	virtual void OnClose( int nErrorCode);

public:
	CTachyonSession();
	~CTachyonSession();
};
