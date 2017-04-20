/*
 *	UdpSocket.h
 *
 *	For Send to LogServer
 */
#ifndef _UDPSOCKET_H_
#define _UDPSOCKET_H_

 
#include <LogPacket.h>


/*
 *	CUdpSocket
 */
class CUdpSocket 
{
public:
	CUdpSocket();
	~CUdpSocket();

private: 
	SOCKET				m_SendSock;
	SOCKADDR_IN			m_LogAddr;
	CRITICAL_SECTION	m_LogLock; 

	CHAR*	m_lpLogBuf;
	INT		m_nSendByte;

	VOID CLOSESOCKET( SOCKET &x )	{	if(x != INVALID_SOCKET) closesocket(x), x = INVALID_SOCKET; }

public:

	//	Base Function
	BOOL Initialize(char *pIPAddr=NULL,int pPort=2000);

	//void Log(_LOG_DATA_ pLogData);
	void Log(_LPUDPPACKET);
	void SendToLogPacket(_LPUDPPACKET);	

	void SendDMLog(_LOG_DATA_ pLogData);

	//	Log Function
	void LogMoneyTrade(DWORD pAction, CTPlayer *pPlayer, CString pTargetName, __int64 pMoney, int pCost=0);

	
	void LogPet					(DWORD pAction, CTPlayer *pPlayer, WORD wPeID, CString strName = NULL, __int64 dwTime = 0);

	void LogLevelUp				(DWORD pAction, CTPlayer *pPlayer, BYTE m_bLevel);

	
	void LogQuest				(DWORD pAction, CTPlayer *pPlayer, DWORD dwQuestID, DWORD dwGain = 0);

	void LogUserDie				(DWORD pAction, CTPlayer *pPlayer, BYTE m_bType);

	void LogEnterMap			(DWORD pAction, CTPlayer *pPlayer);

	void LogLocalOccupy			(DWORD pAction, BYTE bType, WORD wLocalID, BYTE bCountry, DWORD dwGuildID);
	void LogLocalEnable			(DWORD pAction, WORD wLocalID, BYTE bStatus);

	void LogChat				(DWORD pAction, CTPlayer *pPlayer, CString strName, CString strTalk);

	void LogGuild				(DWORD pAction, CTPlayer *pPlayer,DWORD pTargetID,CString pTargetName, DWORD pGuildID, CString strGuildName, BYTE bRet );

	void LogGuildDisorganization(DWORD pAction, CTPlayer *pPlayer, BYTE pDisorg);

	void LogGuildDutyPeer		(DWORD pAction, CTPlayer *pPlayer,	DWORD pTargetID,CString pTargetName, DWORD pGuildID, CString strGuildName, int pData, BYTE bRet );

	void LogGuildExpMoeny		(DWORD pAction, CTPlayer *pPlayer, int pExp, int pGold, int pSilver, int pCooper, BYTE bRet);


	void LogCashCabinetBuy		(DWORD pAction, CTPlayer *pPlayer, int pCashID, __time64_t pDate, int pCash, int pBonus,  int pResult);
    
};

#endif	//	_UDPSOCKET_H_