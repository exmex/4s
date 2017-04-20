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
	void LogMoney(DWORD pAction, CTPlayer *pPlayer, CTMonster *pMon, __int64 pMoney,LPQUESTTEMP pQUEST=NULL, BYTE bIsWorld = 0);
	void LogMoneyTrade(DWORD pAction, CTPlayer *pPlayer, CString pTargetName, __int64 pMoney, int pCost=0);

	void LogItemByMonster(DWORD pAction, CTPlayer *pPlayer, CTMonster *pMon, CTItem *pItem, __int64 pMoney=0);
	void LogItemByMonster(DWORD pAction, CTPlayer *pPlayer, int pMonTempID, CTItem *pItem );

	void LogItemByNPC(DWORD pAction, CTPlayer *pPlayer, CTNpc * pNpc,	CTItem *pItem, int pMoney=0, LPQUESTTEMP pQUEST=NULL);
	void LogItemUpgrade(DWORD pAction, CTPlayer *pPlayer, CTItem *pItem, int nUpLevel=0);	

	void LogItemByStore(DWORD pAction, CTPlayer *pPlayer, CString pTargetName,	CTItem *pItem, __int64 pMoney=0, BYTE bCount = 0);

	void LogItemTrade(DWORD pAction, CTPlayer *pPlayer, CTItem *pItem, CString pTargetName);
	
	void LogExpByMonster(DWORD pAction, CTPlayer *pPlayer, CTMonster *pMon, DWORD dwGain, BYTE bIsWorld = 0);

	void LogSkillAct(DWORD pAction, CTPlayer *pPlayer, CTSkill *pSkill, __int64 pMoney = 0);

	void LogPet					(DWORD pAction, CTPlayer *pPlayer, WORD wPeID, CString strName = NULL, __int64 dwTime = 0);

	void LogLevelUp				(DWORD pAction, CTPlayer *pPlayer, BYTE m_bLevel);

	void LogMonster				(DWORD pAction, CTPlayer *pPlayer, CTMonster *pMon);
	
	void LogQuest				(DWORD pAction, CTPlayer *pPlayer, DWORD dwQuestID, DWORD dwGain = 0);

	void LogUserDie				(DWORD pAction, CTPlayer *pPlayer, BYTE m_bType);

	void LogEnterMap			(DWORD pAction, CTPlayer *pPlayer);

	void LogLocalOccupy			(DWORD pAction, BYTE bType, WORD wLocalID, BYTE bCountry, DWORD dwGuildID);
	void LogLocalEnable			(DWORD pAction, WORD wLocalID, BYTE bStatus);

	void LogCabinet				(DWORD pAction, CTPlayer *pPlayer, BYTE bCabinetID, CTItem *pItem, BYTE bCount, int pMoney = 0);

	void LogChat				(DWORD pAction, CTPlayer *pPlayer, CString strName, CString strTalk);

	void LogGuild				(DWORD pAction, CTPlayer *pPlayer,DWORD pTargetID,CString pTargetName, DWORD pGuildID, CString strGuildName, BYTE bRet );

	void LogGuildDisorganization(DWORD pAction, CTPlayer *pPlayer, BYTE pDisorg);

	void LogGuildDutyPeer		(DWORD pAction, CTPlayer *pPlayer,	DWORD pTargetID,CString pTargetName, DWORD pGuildID, CString strGuildName, int pData, BYTE bRet );

	void LogGuildExpMoeny		(DWORD pAction, CTPlayer *pPlayer, int pExp, int pGold, int pSilver, int pCooper, int pPvPoint, BYTE bRet);

	void LogGuildCabinet		(DWORD pAction, CTPlayer *pPlayer, DWORD pGuildID, CString strGuildName,BYTE bCabinetID, CTItem *pItem, BYTE bCount, DWORD dwMoney);

	void LogCashItem			(DWORD pAction, CTPlayer *pPlayer, DWORD pTargetID, CString pTargetName, CTItem *pItem,int pCashID, int pCash, int pBonus);

	void LogCashCabinetBuy		(DWORD pAction, CTPlayer *pPlayer, int pCashID, __time64_t pDate, int pCash, int pBonus,  int pResult);
    

	void LogTeleport			(DWORD pAction, CTPlayer *pPlayer, CTNpc * pNpc, int pPrice, int pPortalID);

	void LogPvPointChar			(DWORD pAction, CTPlayer *pPlayer,DWORD dwPoint,BYTE bEventType, BYTE bPointType, CString pTargetName);
	void LogPvPointGuild		(DWORD pAction, DWORD dwPoint,WORD wLocalID, BYTE bCountry, DWORD dwGuildID, BYTE bEventType,BYTE bPointType);
	
	void LogAuctionReg			(DWORD pAction, CTPlayer *pPlayer,DWORD dwAuctionID,__int64 ldwStartPrice,__int64 ldwDirectPrice, CTItem *pItem,DWORD dwCharID = 0,CString strNAME = _T(""));
	void LogAuctionBid			(DWORD pAction, CTPlayer *pPlayer,DWORD dwAuctionID,__int64 ldwPrice,CTItem *pItem,BYTE bCount,DWORD dwCharID = 0,CString strNAME = _T(""));

	void LogTournamentApply		(DWORD pAction, CTPlayer *pPlayer, CString pEventName, BYTE bParty, CString strTarget=_T(""));
	void LogTournamentWin		(DWORD pAction, LPTOURNAMENTPLAYER pWin, LPTOURNAMENTPLAYER pLos, CString pTitle, DWORD dwValue, BYTE bStep);
	void LogTournamentEvent		(DWORD pAction, CTPlayer *pPlayer, DWORD dwTargetID, CString pTitle, BYTE bType);
	void LogRPSGame				(DWORD pAction, CTPlayer *pPlayer, BYTE bType, BYTE bWin, DWORD dwMoney);
	void LogTactics				(DWORD pAction, CTPlayer *pPlayer, DWORD dwGuild, CString strName);
	void LogAidCountry			(DWORD pAction, CTPlayer *pPlayer, BYTE bNowAid, BYTE bPrevAid);
	void LogCountry				(DWORD pAction, CTPlayer *pPlayer, BYTE bNowCountry, BYTE bPrevCountry);
	void LogKickOut				(DWORD pAction, CTPlayer *pPlayer, BYTE bType);
	void LogCMGift				(DWORD pAction, CTPlayer *pPlayer, WORD wGiftID, DWORD dwGMID, BYTE bType, DWORD dwValue, BYTE bCount);
};

#endif	//	_UDPSOCKET_H_