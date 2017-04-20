// TMacroMachineDoc.h : CTMacroMachineDoc 클래스의 인터페이스
//

#include "MainFrm.h"

#pragma once

class CTMacroMachineDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CTMacroMachineDoc();
	DECLARE_DYNCREATE(CTMacroMachineDoc)

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
	virtual ~CTMacroMachineDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()

public:
	WSADATA wsaData; // socket

	HWND m_HWND;
    HWND GetSafeHwnd(); // HWND return (TachyonSession)

	CMainFrame * GetMain();
	
	BOOL OnSessionMsg(CTachyonSession *pSession, CPacket *pPacket);
	void OnConnect(CTachyonSession *pSession, int nErrorCode);
	void OnClose(CTachyonSession *pSession, int nErrorCode);

public:
	CRITICAL_SECTION m_cs;
	MAPTPROTOCOL m_mapProtocol;
	MAPTPROTOCOLTYPE m_mapType;

	// TMacroMachineFile.cpp

	BOOL LoadProtocol(CString strFile);
	DWORD SplitProtocol(CString strP);
	void SplitType(DWORD dwValue, CString strT);

	MAPTVALUE m_mapTVALUE;
	QTMACRO m_qTMACRO;
	MAPTPROTOCOLTEMP m_mapTProtocol;

	// TMacroMachineDoc.cpp

	BOOL LoadMacro(CString strFile);
	BOOL EmptyMacro();
	void ExcuteMacro();

	MAPTSESSION m_mapTSession;
	BOOL m_bRun;
	WORD m_wThread;
	WORD m_wSessionIndex;
	BYTE m_bRelayChatting;

	// TMacroMachineThread.cpp

	WORD GetSessionSize();
	WORD GetThreadSize();
	BOOL CreateSession(WORD wCnt,BYTE bConnectType = CON_LOGIN);
	CTachyonSession * CreateSession(BYTE bConnectType);
	BOOL ThreadCreate(CTachyonSession * pSession);
	void StartThread();
	void EndThread();
	void ConnetSession(CString strAddr, DWORD dwPort);

	static DWORD WINAPI _WorkThread( LPVOID lpParam);
	DWORD WorkThread();

	void SendMacroProtocol(CTachyonSession* pSession, TQPROTOCOL& tProtocol);
	void SendMacro(CTachyonSession * pSession);
	BOOL SendLoopProtocol(CTachyonSession* pSession, CPacket* pPacket);
	void Receive(CTachyonSession * pSession, CPacket * pPacket);

	BYTE GetByte(CString str);
	WORD GetWord(CString str);
	DWORD GetDword(CString str);
	FLOAT GetFloat(CString str);
	CString GetString(CString str);
	INT64 GetInt64(CString str);

	void InputValue(CTachyonSession * pSession, CPacket * pMsg, TPTVALUE tValue, DWORD dwProtocolID);

	CString RecvByte(BYTE bValue);
	CString RecvWord(WORD wValue);
	CString RecvDword(DWORD dwValue);
	CString RecvFloat(FLOAT fValue);
	CString RecvString(CString strValue);
	CString RecvInt64(INT64 iValue);

	BYTE OutputValue(CTachyonSession * pSession, CPacket * pMsg, TCHILD tChild, DWORD dwIndex);

	void PacketOutput(CTachyonSession * pSession, CPacket * pPacket, TCHILD tChild);
	void UpdateSessionCnt();
};


