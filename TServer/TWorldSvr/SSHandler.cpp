#include "stdafx.h"
#include "TWorldSvr.h"
#include "TWorldSvrModule.h"

DWORD CTWorldSvrModule::OnCT_SERVICEMONITOR_ACK( LPPACKETBUF pBUF )
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwTick;
	pBUF->m_packet
		>> dwTick;

	pSERVER->SendCT_SERVICEMONITOR_REQ(dwTick, DWORD(m_mapSESSION.size()), DWORD(m_mapTCHAR.size()), DWORD(m_mapACTIVEUSER.size()));
	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_USERMOVE_ACK( LPPACKETBUF pBUF)
{
	CString strUser;

	BYTE bChannel;
	WORD wMapID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;
	WORD wPartyID;

	pBUF->m_packet
		>> strUser
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> wPartyID;

	LPTCHARACTER pPlayer = FindTChar(strUser);
    if(!pPlayer)
		return EC_NOERROR;

	CTServer *pServer = FindMapSvr(pPlayer->m_bMainID);
	if(pServer)
		pServer->SendCT_USERMOVE_ACK(pPlayer->m_dwCharID, pPlayer->m_dwKEY, bChannel, wMapID, fPosX, fPosY, fPosZ, wPartyID);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_USERPOSITION_ACK(LPPACKETBUF pBUF)
{
	CString strTargetName;
	CString strGMName;

	pBUF->m_packet
		>> strTargetName
		>> strGMName;

	LPTCHARACTER pTarget = FindTChar(strTargetName);
    if(!pTarget)
		return EC_NOERROR;

	LPTCHARACTER pGM = FindTChar(strGMName);
    if(!pGM)
		return EC_NOERROR;

	CTServer * pServer = FindMapSvr(pTarget->m_bMainID);
	if(pServer)
		pServer->SendMW_USERPOSITION_REQ(pTarget->m_dwCharID, pTarget->m_dwKEY, strGMName);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_CHATBAN_REQ(LPPACKETBUF pBUF)  
{
	CTServer *pCTSERVER = (CTServer *) pBUF->m_pSESSION;

	CString strName;
	WORD wMin;
	DWORD dwBanSeq;
	DWORD dwManagerID;

	pBUF->m_packet
		>> strName
		>> wMin
		>> dwBanSeq
		>> dwManagerID;

	LPTCHARACTER pTarget = FindTChar(strName);
	if(!pTarget)
	{
		pCTSERVER->SendCT_CHATBAN_ACK(FALSE,dwBanSeq,dwManagerID);
		return EC_NOERROR;
	}

	CTime t(CTime::GetCurrentTime());
	CTime BanTime(t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute()+wMin,t.GetSecond());

	pTarget->m_nChatBanTime = BanTime.GetTime();

	CTServer* pServer = FindMapSvr(pTarget->m_bMainID);
	if(pServer)
		pServer->SendMW_CHATBAN_REQ(strName,pTarget->m_nChatBanTime,CHATBAN_SUCCESS,0,0);

	AddChatBan(strName, pTarget->m_nChatBanTime);

	pCTSERVER->SendCT_CHATBAN_ACK(TRUE,dwBanSeq,dwManagerID);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_CHARMSG_ACK(LPPACKETBUF pBUF)
{
	CString strName;
	CString strMsg;

	pBUF->m_packet
		>> strName
		>> strMsg;

	LPTCHARACTER pTarget = FindTChar(strName);
	if(!pTarget)
		return EC_NOERROR;

	strMsg.Left(ONE_KBYTE);

	CTServer* pServer = FindMapSvr(pTarget->m_bMainID);
	if(pServer)
		pServer->SendMW_CHARMSG_REQ(strName,strMsg);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_SERVICEDATACLEAR_ACK(LPPACKETBUF pBUF)
{
	m_mapACTIVEUSER.clear();

	MAPDWORD::iterator itA;
	MAPTCHARACTER::iterator itC;
	for(itC = m_mapTCHAR.begin(); itC != m_mapTCHAR.end(); itC++)
	{
		itA = m_mapACTIVEUSER.find( (*itC).second->m_dwUserID);
		if( itA == m_mapACTIVEUSER.end() )
			m_mapACTIVEUSER.insert(MAPDWORD::value_type( (*itC).second->m_dwUserID, (*itC).second->m_dwUserID) );
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_ITEMFIND_REQ(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwManager;
	WORD wItemID;
	CString strItemName;

	pBUF->m_packet
		>> dwManager
		>> wItemID
		>> strItemName;
	
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_ITEMFIND_REQ)
		<< dwManager
		<< wItemID
		<< strItemName;
	
	SayToDB(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_ITEMSTATE_REQ(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;	

	WORD wCount;
	WORD wItemID;
	BYTE bInitState;
	DWORD dwID;

	pBUF->m_packet
		>> dwID
		>> wCount;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_ITEMSTATE_REQ)
		<< dwID
		<< wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		pBUF->m_packet
			>> wItemID
			>> bInitState;

		pMSG->m_packet
			<< wItemID
			<< bInitState;
	}

	SayToDB(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_CTRLSVR_REQ(LPPACKETBUF pBUF)
{
	CTServer* pSERVER= (CTServer*)pBUF->m_pSESSION;

	if(pSERVER)
        m_pCtrlSvr = pSERVER;

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_CASTLEGUILDCHG_REQ(LPPACKETBUF pBUF)
{
	CTServer* pSERVER = (CTServer*)pBUF->m_pSESSION;

	BYTE bRet = FALSE;
	WORD wCastleID;
	DWORD dwDefGuildID;
	DWORD dwAtkGuildID;
	DWORD dwManagerID;
	__time64_t tTime;
	CString strDefGuild = NAME_NULL;
	CString strAtkGuild = NAME_NULL;

	pBUF->m_packet
		>> wCastleID
		>> dwDefGuildID
		>> dwAtkGuildID
		>> dwManagerID
		>> tTime;

	CTGuild* pDefGuild = FindTGuild(dwDefGuildID);
	CTGuild* pAtkGuild = FindTGuild(dwAtkGuildID);

	if(pDefGuild && pAtkGuild)
	{
		strDefGuild = pDefGuild->m_strName;
		strAtkGuild = pAtkGuild->m_strName;
	}
	
	if(strDefGuild == NAME_NULL || strAtkGuild == NAME_NULL )
	{
		pSERVER->SendCT_CASTLEGUILDCHG_ACK(dwManagerID,FALSE);
		return EC_NOERROR;
	}
	
	MAPTSERVER::iterator itS;
	for(itS = m_mapSERVER.begin(); itS != m_mapSERVER.end(); itS++)
	{
		(*itS).second->SendMW_CASTLEGUILDCHG_REQ(wCastleID,dwDefGuildID,strDefGuild,dwAtkGuildID,strAtkGuild,tTime);		
	}

	pSERVER->SendCT_CASTLEGUILDCHG_ACK(dwManagerID,TRUE,wCastleID,dwDefGuildID,strDefGuild,dwAtkGuildID,strAtkGuild,tTime);

	return EC_NOERROR;
}
 
DWORD CTWorldSvrModule::OnCT_EVENTUPDATE_REQ(LPPACKETBUF pBUF)
{
	BYTE bEventID;
	WORD wValue;
	EVENTINFO stEVENT;

	pBUF->m_packet
		>> bEventID
		>> wValue;
	
	stEVENT.WrapPacketOut(&pBUF->m_packet);
	stEVENT.m_wValue = wValue;

	if(bEventID > EVENT_COUNT)
		return EC_NOERROR;

	if(stEVENT.m_bID == EVENT_LOTTERY)
	{
		if(stEVENT.m_bState)
			LotteryItem(stEVENT.m_vLOTTERY, (BYTE)stEVENT.m_wMapID, stEVENT.m_strLotMsg, stEVENT.m_strTitle);

		return EC_NOERROR;
	}
	if(stEVENT.m_bID == EVENT_GIFTTIME)
	{
		if(stEVENT.m_bState)
			GiftTime(&stEVENT);//.m_vLOTTERY.at(0), stEVENT.m_strLotMsg);

		return EC_NOERROR;
	}

	MAPEVENTINFO::iterator itE = m_mapEVENT.find(stEVENT.m_dwIndex);
	if( itE != m_mapEVENT.end() )
		m_mapEVENT.erase(itE);
	if(wValue)
		m_mapEVENT.insert(MAPEVENTINFO::value_type(stEVENT.m_dwIndex,stEVENT));
	

	MAPTSERVER::iterator itS;
	for(itS=m_mapSERVER.begin(); itS!=m_mapSERVER.end(); itS++)
	{
		(*itS).second->SendMW_EVENTUPDATE_REQ(bEventID,wValue,&stEVENT);		
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_EVENTMSG_REQ(LPPACKETBUF pBUF)
{
	BYTE bEventID;
	BYTE bEventMsgType;
	CString strMsg;

	pBUF->m_packet
		>> bEventID
		>> bEventMsgType
		>> strMsg;

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it!=m_mapSERVER.end(); it++)
		(*it).second->SendMW_EVENTMSG_REQ(bEventID,bEventMsgType,strMsg);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_CASHSHOPSTOP_REQ(LPPACKETBUF pBUF)
{
	BYTE bType;

	pBUF->m_packet
		>> bType;

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it!=m_mapSERVER.end(); it++)
		(*it).second->SendMW_CASHSHOPSTOP_REQ(bType);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_CASHITEMSALE_REQ(LPPACKETBUF pBUF)
{
	WORD wCount;
	WORD wValue;
	DWORD dwIndex;
	
	LPVTCASHITEMSALE pvSaleItem = NULL;
	TCASHITEMSALEEVENT stEVENT;
	
	
	pBUF->m_packet
		>> dwIndex
		>> wValue
		>> wCount;

	stEVENT.m_dwIndex = dwIndex;
	stEVENT.m_wValue = wValue;
	
	
	for(WORD i = 0; i < wCount; i++ )
	{
		TCASHITEMSALE stSALEITEM;

		pBUF->m_packet
			>> stSALEITEM.m_wID
			>> stSALEITEM.m_bSaleValue;
		
		stEVENT.m_vCashItemSale.push_back(stSALEITEM);		
	}
	
	if( wValue == 0)
	{
		m_bCashItemSale = FALSE;
		MAPTCASHITEMSALE::iterator itC = m_mapTCashItemSale.find(dwIndex);
		if( itC != m_mapTCashItemSale.end())
		{
			(*itC).second.m_wValue = 0;
			VTCASHITEMSALE::iterator itE;
			for( itE = (*itC).second.m_vCashItemSale.begin(); itE != (*itC).second.m_vCashItemSale.end(); itE++ )
				(*itE).m_bSaleValue = 0;		

			pvSaleItem = &((*itC).second.m_vCashItemSale);			
		}		
	}
	else
	{
		m_bCashItemSale = TRUE;
        m_mapTCashItemSale.insert(MAPTCASHITEMSALE::value_type(dwIndex, stEVENT));
		pvSaleItem = &stEVENT.m_vCashItemSale;
	}
	
	if(!pvSaleItem)
	{
		LogEvent("OnCT_CASHITEMSALE_REQ Error");
		return EC_NOERROR;
	}

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it!=m_mapSERVER.end(); it++)
	{
		(*it).second->m_bCashSale = FALSE;
		(*it).second->SendMW_CASHITEMSALE_REQ(dwIndex,wValue,pvSaleItem);
	}

	
	return EC_NOERROR;
}
 
DWORD CTWorldSvrModule::OnCT_EVENTQUARTERLIST_REQ(LPPACKETBUF pBUF)
{
	
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.Copy(&(pBUF->m_packet));	
	pMSG->m_packet.SetID(DM_EVENTQUARTERLIST_REQ);		

	SayToDB(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_EVENTQUARTERUPDATE_REQ(LPPACKETBUF pBUF)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.Copy(&(pBUF->m_packet));
	pMSG->m_packet.SetID(DM_EVENTQUARTERUPDATE_REQ);		

	SayToDB(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_HELPMESSAGE_REQ(LPPACKETBUF pBUF)
{
	BYTE bID;
	INT64 dlStart;
	INT64 dlEnd;
	CString strMessage;

	pBUF->m_packet
		>> bID
		>> dlStart
		>> dlEnd
		>> strMessage;

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it!=m_mapSERVER.end(); it++)
		(*it).second->SendMW_HELPMESSAGE_REQ(bID, dlStart, dlEnd, strMessage);

	SendDM_HELPMESSAGE_REQ(bID, dlStart, dlEnd, strMessage);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_CMGIFT_REQ(LPPACKETBUF pBUF)
{
	CString strTarget;
	WORD wGiftID;
	DWORD dwManagerID;
	
	pBUF->m_packet
		>> strTarget
		>> wGiftID
		>> dwManagerID;
	
	MAPCMGIFT::iterator it = m_mapCMGift.find(wGiftID);
	if(it != m_mapCMGift.end())
	{
		if(it->second->m_bTakeType)
			SendDM_CMGIFT_REQ(strTarget, wGiftID, TRUE, dwManagerID);
		else
			SendDM_CMGIFT_ACK(CMGIFT_SUCCESS, strTarget, wGiftID, TRUE, dwManagerID); 
		return EC_NOERROR;
	}
	
	SendDM_CMGIFT_ACK(CMGIFT_ID, strTarget, wGiftID, TRUE, dwManagerID); 
	return EC_NOERROR;	
}

DWORD CTWorldSvrModule::OnCT_CMGIFTCHARTUPDATE_REQ(LPPACKETBUF pBUF)
{
	SendDM_CMGIFTCHARTUPDATE_REQ(&(pBUF->m_packet));

	return EC_NOERROR;	
}


DWORD CTWorldSvrModule::OnCT_CMGIFTLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwManagerID;	
	pBUF->m_packet
		>> dwManagerID;

	if(m_pCtrlSvr)
		m_pCtrlSvr->SendCT_CMGIFTLIST_ACK(dwManagerID, &m_mapCMGift);
	return EC_NOERROR;	
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

DWORD CTWorldSvrModule::OnSM_QUITSERVICE_REQ( LPPACKETBUF pBUF)
{
	LogEvent(_T("SM_QUITSERVICE_REQ detected !!"));
	if(m_bService)
		SetServiceStatus(SERVICE_STOP_PENDING);
	PostThreadMessage( m_dwThreadID, WM_QUIT, 0, 0);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnSM_DELSESSION_REQ( LPPACKETBUF pBUF)
{
	if(HIBYTE(((CTServer *) pBUF->m_pSESSION)->m_wID) == SVRGRP_MAPSVR)
	{
		MAPTCHARACTER::iterator itCHAR = m_mapTCHAR.begin();

		while(itCHAR != m_mapTCHAR.end())
		{
			MAPTCHARACTER::iterator itNEXT = itCHAR;
			LPTCHARACTER pTCHAR = (*itCHAR).second;
			itNEXT++;

			if( pTCHAR->m_mapTCHARCON.find(LOBYTE(((CTServer *) pBUF->m_pSESSION)->m_wID)) != pTCHAR->m_mapTCHARCON.end() )
				CloseChar(pTCHAR);

			itCHAR = itNEXT;
		}

		LPPACKETBUF pMsg = new PACKETBUF();
		pMsg->m_packet.SetID(DM_CLEARMAPCURRENTUSER_REQ)
			<< m_bGroupID
			<< LOBYTE(((CTServer *) pBUF->m_pSESSION)->m_wID)
			<< BYTE(SVRGRP_MAPSVR);
		
		SayToDB(pMsg);
	}

	PostQueuedCompletionStatus(
		m_hIocpControl, 0,
		COMP_CLOSE,
		(LPOVERLAPPED) pBUF->m_pSESSION);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnSM_GUILDDISORGANIZATION_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwTime;
	BYTE bDisorg;

	pBUF->m_packet
		>> dwGuildID
		>> dwTime
        >> bDisorg;

	if(bDisorg)
		m_mapTGuildEx.insert(MAPDWORD::value_type(dwGuildID, dwTime));
	else
	{
		MAPDWORD::iterator it = m_mapTGuildEx.find(dwGuildID);
		if(it!=m_mapTGuildEx.end())
			m_mapTGuildEx.erase(it);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CONNECT_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;
	WORD wServerID;
	BYTE bCount;

	pBUF->m_packet
		>> wServerID
		>> bCount;

	MAPTSERVER::iterator finder = m_mapSERVER.find(wServerID);
	if( finder != m_mapSERVER.end() )
		return EC_SESSION_DUPSERVERID;

	pSERVER->m_wID = wServerID;

	for( BYTE i=0; i<bCount; i++)
	{
		BYTE bChannel;

		pBUF->m_packet
			>> bChannel;

		pSERVER->m_mapCHANNEL.insert( MAPBYTE::value_type( bChannel, bChannel));
	}
	m_mapSERVER.insert( MAPTSERVER::value_type( pSERVER->m_wID, pSERVER));

	pSERVER->SendMW_LOCALENABLE_REQ(
		m_battletime[BT_LOCAL].m_bStatus,
		0,
		m_battletime[BT_LOCAL].m_dwBattleStart,
		m_battletime[BT_CASTLE].m_bDay,
		m_battletime[BT_CASTLE].m_dwBattleStart);

	pSERVER->SendMW_MISSIONENABLE_REQ(
		m_battletime[BT_MISSION].m_bStatus,
		m_battletime[BT_MISSION].m_dwBattleStart,
		0);

	if(m_pRelay)
		pSERVER->SendMW_RELAYCONNECT_REQ(0);

	MAPEVENTINFO::iterator itE;
	for( itE = m_mapEVENT.begin() ; itE != m_mapEVENT.end(); itE++)
		pSERVER->SendMW_EVENTUPDATE_REQ((*itE).second.m_bID,(*itE).second.m_wValue,&((*itE).second) );

	MAPTCASHITEMSALE::iterator itC;
	for(itC = m_mapTCashItemSale.begin(); itC != m_mapTCashItemSale.end(); itC++)
		pSERVER->SendMW_CASHITEMSALE_REQ( (*itC).first,(*itC).second.m_wValue,&((*itC).second.m_vCashItemSale) );

	MAPWORD mapCastle;
	MAPWORD::iterator itCs;
	MAPTGUILD::iterator itGd;
	for(itGd=m_mapTGuild.begin(); itGd != m_mapTGuild.end(); itGd++)
	{
		mapCastle.clear();
		CTGuild * pGuild = (*itGd).second;
		pGuild->GetCastleApplicantCount(&mapCastle);
		for(itCs=mapCastle.begin(); itCs != mapCastle.end(); itCs++)
			pSERVER->SendMW_CASTLEAPPLICANTCOUNT_REQ((*itCs).first, pGuild->m_dwID, HIBYTE((*itCs).second), LOBYTE((*itCs).second));
	}

	static BYTE bInitExpired = TRUE;
	if(bInitExpired)
	{
		MAPTGUILD::iterator itGd;
		MAPTTACTICSMEMBER::iterator itTc;
		for(itGd=m_mapTGuild.begin(); itGd != m_mapTGuild.end(); itGd++)
		{
			CTGuild * pGuild = (*itGd).second;
			for(itTc=pGuild->m_mapTTactics.begin(); itTc!=pGuild->m_mapTTactics.end(); itTc++)
				OnEventExpired(TRUE, EXPIRED_GT, (*itTc).second->m_dlEndTime, pGuild->m_dwID, (*itTc).second->m_dwID);
		}

		bInitExpired = FALSE;
	}

	 
	pSERVER->SendMW_MONTHRANKLIST_REQ(m_arMonthRank,m_bRankMonth);	
	TournamentInfo(pSERVER);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_ADDCHAR_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwIPAddr;
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwUserID;

	WORD wPort;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwIPAddr
		>> wPort
		>> dwUserID;

	MAPDWORD::iterator itA = m_mapACTIVEUSER.find(dwUserID);
	if( itA == m_mapACTIVEUSER.end() )
		m_mapACTIVEUSER.insert(MAPDWORD::value_type(dwUserID,dwUserID) );

	MAPTCHARACTER::iterator itCHAR = m_mapTCHAR.find(dwCharID);
	if( itCHAR == m_mapTCHAR.end())
	{
		// 월드에 처음 입장. 캐릭터 오브젝트를 생성하고 관리목록에 등록
		LPTCHARACTER pTCHAR = new TCHARACTER();
		LPTCHARCON pCON = new TCHARCON();

		pTCHAR->m_dwCharID = dwCharID;
		pTCHAR->m_dwKEY = dwKEY;
		pTCHAR->m_dwUserID = dwUserID;

		pTCHAR->m_bMainID = LOBYTE(pSERVER->m_wID);
		pTCHAR->m_bCHGMainID = FALSE;
		pTCHAR->m_bLogout = FALSE;
		pTCHAR->m_bSave = FALSE;

		pTCHAR->m_vTDEADCON.clear();
		pTCHAR->m_strNAME.Empty();
		pTCHAR->m_mapTMS.clear();
		pTCHAR->m_mapTSOULMATE.clear();
		pTCHAR->m_mapBatting.clear();

		pTCHAR->m_bDBLoading = FALSE;
		pTCHAR->m_bPartyWaiter = FALSE;
		pTCHAR->m_pParty = NULL;
		pTCHAR->m_pGuild = NULL;
		pTCHAR->m_pTactics = NULL;
		pTCHAR->m_bLevel = 0;
		pTCHAR->m_bHelmetHide = 0;

		pTCHAR->m_bChannel = 0;
		pTCHAR->m_wMapID = 0;

		pTCHAR->m_dwRiding = 0;
		pTCHAR->m_fPosX = 0.0f;
		pTCHAR->m_fPosY = 0.0f;
		pTCHAR->m_fPosZ = 0.0f;
		pTCHAR->m_nChatBanTime = 0; 
		pTCHAR->m_timeRelay = 0;
		pTCHAR->m_dwSoulSilence = 0;
		pTCHAR->m_dwTicket = 0;
		pTCHAR->m_bAidCountry = TCONTRY_N;

		pCON->m_dwIPAddr = dwIPAddr;
		pCON->m_wPort = wPort;

		pCON->m_bReady = FALSE;
		pCON->m_bValid = TRUE;

		// 메인서버 등록
		pTCHAR->m_mapTCHARCON.insert( MAPTCHARCON::value_type( LOBYTE(pSERVER->m_wID), pCON));
		// 캐릭터 등록
		m_mapTCHAR.insert( MAPTCHARACTER::value_type( dwCharID, pTCHAR));

		// 메인서버로 입장. 이 시점에 계정인증 절차를 거친 후 최초로 캐릭터 데이터가 맵 서버에 로드 된다
		pSERVER->SendMW_ENTERSVR_REQ(
			TRUE,
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY);
	}
	else
	{
		// 추가 커넥션 연결. 캐릭터 오브젝트에 등록된 유효한 연결인지에대한 인증절차를 마친 후
		// 캐릭터 오브젝트에 등록된 해당 연결의 연결상태(m_bValid)를 연결됨으로 셑팅하고 캐릭터 오브젝트의 모든 연결의
		// 연결상태가 연결됨으로 셑팅 되었으면 MW_CHARDATA_ACK를 통한 메인서버 체크를 시도
		MAPTCHARCON::iterator itCON = (*itCHAR).second->m_mapTCHARCON.find(LOBYTE(pSERVER->m_wID));
		CTServer *pMAIN = FindMapSvr((*itCHAR).second->m_bMainID);

		if( !pMAIN || (*itCHAR).second->m_dwKEY != dwKEY ||		// 새롭게 접속한 사용자의 인증키가 등록되어있는 사용자의 인증키와 다르거나
			itCON == (*itCHAR).second->m_mapTCHARCON.end() ||	// 연결이 캐릭터 오브젝트에 등록된 연결이 아니거나
			(*itCON).second->m_bValid ||						// 새로운 연결의 연결상태가 이미 연결됨으로 설정되어 있거나
			dwIPAddr != (*itCON).second->m_dwIPAddr ||			// 새로운 연결의 IP가 등록된 연결의 IP와 다르거나
			wPort != (*itCON).second->m_wPort )					// 새로운 연결의 포트가 등록된 연결의 포트가 다르면
		{
			// 중복된 접속 또는 공식 배포된 클라이언트가 아니거나 해킹으로 의심되는 연결
			// 중복접속 처리를 위해 맵 서버를 통해 기존 접속자의 클라이언트 연결을 종료하고
			// 신규 접속자의 클라이언트에 오류메세지를 전달한 후 클라이언트의 연결을 종료
			pSERVER->SendMW_INVALIDCHAR_REQ(
				dwCharID,
				dwKEY);
			CloseChar((*itCHAR).second);

			return EC_NOERROR;
		}

		// 캐릭터 로드상태를 로드되지 않음, 연결상태를 연결됨으로 셑팅
		(*itCON).second->m_bReady = FALSE;
		(*itCON).second->m_bValid = TRUE;

		// 필요한 모든 연결의 연결상태가 연결됨으로 셑팅되어있는지 확인 후 연결되지 않음으로 셑팅된 연결이 있으면
		// 모든 연결의 연결상태가 연결됨으로 셑팅될때까지 대기
		for( itCON = (*itCHAR).second->m_mapTCHARCON.begin(); itCON != (*itCHAR).second->m_mapTCHARCON.end(); itCON++)
			if(!(*itCON).second->m_bValid)
				return EC_NOERROR;

		// 연결상태가 연결됨으로 셑팅 되었으면 MW_CHARDATA_ACK를 통한 메인서버 체크를 시도
		pMAIN->SendMW_CHARDATA_REQ(
			dwCharID,
			dwKEY);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CHARDATA_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bStartAct;
	BYTE bCountry;
	BYTE bLevel;
	BYTE bMode;

	DWORD dwMaxHP;
	DWORD dwMaxMP;
	DWORD dwHP;
	DWORD dwMP;

	DWORD dwSoulmate = 0;
	CString strSoulmate = NAME_NULL;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bStartAct
		>> bLevel
		>> dwMaxHP
		>> dwHP
		>> dwMaxMP
		>> dwMP
		>> bCountry
		>> bMode;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
	{
		pSERVER->SendMW_DELCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE,
			FALSE);

		return EC_NOERROR;
	}

	SetCharLevel(
		pTCHAR,
		bLevel);

	SetCharStatus(
		pTCHAR,
		dwMaxHP,
		dwHP,
		dwMaxMP,
		dwMP);

	CTServer *pMAIN = FindMapSvr(pTCHAR->m_bMainID);
	if(!pMAIN)
	{
		// 메인서버가 없거나 다르게 설정되어 공식 배포된 클라이언트가 아니거나 해킹으로 의심되는 연결
		// 맵 서버를 통해 클라이언트에게 오류메세지를 전달한 후 클라이언트의 연결을 종료
		pSERVER->SendMW_INVALIDCHAR_REQ(
			dwCharID,
			dwKEY);

		return EC_NOERROR;
	}

	LPTSOULMATE pSOUL = NULL;
	MAPTSOULMATE::iterator itSOUL;
	for(itSOUL=pTCHAR->m_mapTSOULMATE.begin(); itSOUL!=pTCHAR->m_mapTSOULMATE.end(); itSOUL++)
	{
		if((*itSOUL).second->m_dwCharID == pTCHAR->m_dwCharID)
		{			
			pSOUL = (*itSOUL).second;
			break;
		}
	}

	if(pSOUL)
	{
		dwSoulmate = pSOUL->m_dwTarget;
		strSoulmate = pSOUL->m_strName;
	}

	MAPTCHARCON::iterator itCON = pTCHAR->m_mapTCHARCON.begin();
	BYTE bReady = TRUE;

	WORD wCastle = 0;
	BYTE bCamp = 0;

	if(pTCHAR->m_pGuild)
	{
		LPTGUILDMEMBER pMem = pTCHAR->m_pGuild->FindMember(dwCharID);
		if(pMem)
		{
			wCastle = pMem->m_wCastle;
			bCamp = pMem->m_bCamp;
		}
	}

	if(!wCastle && pTCHAR->m_pTactics)
	{
		LPTTACTICSMEMBER pTac = pTCHAR->m_pTactics->FindTactics(dwCharID);
		if(pTac)
		{
			wCastle = pTac->m_wCastle;
			bCamp = pTac->m_bCamp;
		}
	}

	WORD wCommander = 0;
	if(pTCHAR->m_pParty)
	{
		CTCorps * pCorps = FindCorps(pTCHAR->m_pParty->m_wCorpsID);
		if(pCorps)
			wCommander = pCorps->m_wCommander;
	}

	// 모든 연결의 캐릭터 로드상태가 로드됨으로 셑팅되어있는지 체크
	while(itCON != pTCHAR->m_mapTCHARCON.end())
	{
		MAPTCHARCON::iterator itNEXT = itCON;
		itNEXT++;

		if(!(*itCON).second->m_bReady)
		{
			CTServer *pMAP = FindMapSvr((*itCON).first);

			if(pMAP)
			{
				// 캐릭터가 로드되지 않은 연결로 캐릭터 데이터 전송
				pMAP->SendMW_ENTERCHAR_REQ(
					pTCHAR->m_dwCharID,
					pTCHAR->m_dwKEY,
					bStartAct,
					pTCHAR->m_strNAME,
					pTCHAR->m_wMapID,
					pTCHAR->m_fPosX,
					pTCHAR->m_fPosY,
					pTCHAR->m_fPosZ,
					DWORD(pTCHAR->m_pGuild ? pTCHAR->m_pGuild->m_dwID : 0),
					(pTCHAR->m_pGuild ? pTCHAR->m_pGuild->m_dwFame : 0),
					(pTCHAR->m_pGuild ? pTCHAR->m_pGuild->m_dwFameColor : 0),
					(pTCHAR->m_pGuild ? pTCHAR->m_pGuild->m_strName : NAME_NULL),
					(pTCHAR->m_pGuild ? pTCHAR->m_pGuild->FindDuty(pTCHAR->m_dwCharID) : 0),
					(pTCHAR->m_pGuild ? pTCHAR->m_pGuild->FindPeer(pTCHAR->m_dwCharID) : 0),
					wCastle,
					bCamp,
					DWORD(pTCHAR->m_pTactics ? pTCHAR->m_pTactics->m_dwID : 0),
					(pTCHAR->m_pTactics ? pTCHAR->m_pTactics->m_strName : NAME_NULL),
					WORD(pTCHAR->m_pParty ? pTCHAR->m_pParty->GetID() : 0),
					BYTE(pTCHAR->m_pParty ? pTCHAR->m_pParty->m_bObtainType : 0),
					DWORD(pTCHAR->m_pParty ? pTCHAR->m_pParty->GetChiefID() : 0),
					wCommander,
					bLevel,
					pTCHAR->m_bHelmetHide,
					bCountry,
					pTCHAR->m_bAidCountry,
					bMode,
					pTCHAR->m_dwRiding,
					pTCHAR->m_nChatBanTime,
					dwSoulmate,
					pTCHAR->m_dwSoulSilence,
					strSoulmate,
					pTCHAR->m_bClass,
					&pBUF->m_packet);

				// 캐릭터가 로드되지 않은 연결이 존재함을 표시
				bReady = FALSE;				
			}
			else
			{
				// 비활성 중인 맵 서버로의 연결은 삭제
				delete (*itCON).second;
				pTCHAR->m_mapTCHARCON.erase(itCON);
			}
		}

		itCON = itNEXT;
	}

	// 모든 연결의 캐릭터 로드상태가 로드됨으로 셑팅되어있으면 메인서버 체크
	if(bReady)
		CheckMainCON(pTCHAR);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_ENTERCHAR_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
	{
		pSERVER->SendMW_DELCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE,
			FALSE);

		return EC_NOERROR;
	}

	MAPTCHARCON::iterator itCON = pTCHAR->m_mapTCHARCON.find( LOBYTE(pSERVER->m_wID));
	LPTCHARCON pCON = NULL;

	if( itCON != pTCHAR->m_mapTCHARCON.end() )
		pCON = (*itCON).second;

	if(!pCON)
	{
		// 캐릭터 오브젝트에 해당 연결이 등록되지 않으므로 공식 배포된 클라이언트가 아니거나 해킹으로 의심되는 연결
		// 맵 서버를 통해 클라이언트에게 오류메세지를 전달한 후 클라이언트의 연결을 종료
		pSERVER->SendMW_INVALIDCHAR_REQ(
			dwCharID,
			dwKEY);

		return EC_NOERROR;
	}

	// 캐릭터 데이터 로드상태를 로드됨으로 셑팅
	pCON->m_bReady = TRUE;

	// 모든 연결의 캐릭터 데이터 로드상태가 로드됨으로 셑팅될때 까지 대기
	for( itCON = pTCHAR->m_mapTCHARCON.begin(); itCON != pTCHAR->m_mapTCHARCON.end(); itCON++)
		if(!(*itCON).second->m_bReady)
			return EC_NOERROR;

	// 모든 연결의 캐릭터 로드상태가 로드됨으로 셑팅되어있으면 메인서버 체크
	CheckMainCON(pTCHAR);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CHECKMAIN_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
	{
		pSERVER->SendMW_DELCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE,
			FALSE);

		return EC_NOERROR;
	}

	CTServer *pMAIN = FindMapSvr(pTCHAR->m_bMainID);
	if(!pMAIN)
	{
		// 메인서버가 없어 공식 배포된 클라이언트가 아니거나 해킹으로 의심되는 연결
		// 맵 서버를 통해 클라이언트에게 오류메세지를 전달한 후 클라이언트의 연결을 종료
		pSERVER->SendMW_INVALIDCHAR_REQ(
			dwCharID,
			dwKEY);

		return EC_NOERROR;
	}

	if(!pTCHAR->m_bSave)
	{
		// 메인서버 교체중 다시 메인서버를 교체를 시도 또는 공식 배포된 클라이언트가 아니거나 해킹으로 의심되는 연결
		// 클라이언트의 연결을 종료
		CloseChar(pTCHAR);
	}

	if( pMAIN == pSERVER )
	{
		// 메인서버를 교체하지 않아도 되는 경우
		// 연결관리 프로세스의 마무리 단계

		// 종료해야할 연결들을 종료
		ClearDeadCON(pTCHAR);

		// 맵으로의 진입을 허가
		pMAIN->SendMW_CONRESULT_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY,
			CN_SUCCESS,
			pTCHAR->m_mapTCHARCON);

		// 현재 진행되고 있는 연결관리 프로세스를 종료하고 다음 연결관리 프로세스를 활성화
		PopConCess(pTCHAR);
	}
	else
	{
		// 메인서버를 교체해야하는 경우

		// 현재 메인서버로 메인서버 해제메세지 전달
		// 맵 서버는 해제메세지를 받으면 캐릭터 데이터 저장 후 해제완료 메세지(MW_RELEASEMAIN_ACK)를 돌려줌
		pMAIN->SendMW_RELEASEMAIN_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY,
			pTCHAR->m_bChannel,
			pTCHAR->m_wMapID,
			pTCHAR->m_fPosX,
			pTCHAR->m_fPosY,
			pTCHAR->m_fPosZ);

		// 예비 메인서버 등록
		pTCHAR->m_bMainID = LOBYTE(pSERVER->m_wID);
		pTCHAR->m_bSave = FALSE;
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_ENTERSVR_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;
	CString strNAME;

	DWORD dwCharID;
	DWORD dwUserIP;

	BYTE bLevel;
	DWORD dwKEY;
	DWORD dwRegion;
	BYTE bRealSex;
	BYTE bCountry;
	BYTE bAidCountry;
	BYTE bClass;
	BYTE bRace;
	BYTE bSex;
	BYTE bFace;
	BYTE bHair;

	BYTE bHelmetHide;

	BYTE bLogout;
	BYTE bSave;
	BYTE bResult;

	BYTE bChannel;
	WORD wMapID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strNAME
		>> bLevel
		>> bRealSex
		>> bClass
		>> bRace
		>> bSex
		>> bFace
		>> bHair
		>> bHelmetHide
		>> bCountry
		>> bAidCountry
		>> dwRegion
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> bLogout
		>> bSave
		>> bResult
		>> dwUserIP;

	// 인증결과 확인 시작
	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
	{
		// MW_ADDCHAR_ACK를 통한 연결이 아니거나 다른 요인에 의해 삭제되어 유효하지 않은 연결
		// 맵 서버를 통해 클라이언트의 연결을 종료
		pSERVER->SendMW_DELCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE,
			FALSE);

		return EC_NOERROR;
	}

	CTServer *pMAIN = FindMapSvr(pTCHAR->m_bMainID);
	if( !pMAIN || pMAIN != pSERVER )
	{
		// 메인서버가 없거나 다르게 설정되어 공식 배포된 클라이언트가 아니거나 해킹으로 의심되는 연결
		// 맵 서버를 통해 클라이언트에게 오류메세지를 전달한 후 클라이언트의 연결을 종료
		pSERVER->SendMW_INVALIDCHAR_REQ(
			dwCharID,
			dwKEY);

		return EC_NOERROR;
	}

	MAPTCHARCON::iterator itCON = pTCHAR->m_mapTCHARCON.find( LOBYTE(pSERVER->m_wID));
	LPTCHARCON pCON = NULL;

	if( itCON != pTCHAR->m_mapTCHARCON.end() )
		pCON = (*itCON).second;

	if(!pCON)
	{
		// 캐릭터 오브젝트에 메인 커넥션이 등록되지 않은 공식 배포된 클라이언트가 아니거나 해킹으로 의심되는 연결
		// 맵 서버를 통해 클라이언트에게 오류메세지를 전달한 후 클라이언트의 연결을 종료
		pSERVER->SendMW_INVALIDCHAR_REQ(
			dwCharID,
			dwKEY);

		return EC_NOERROR;
	}

	if(m_pRelay)
		m_pRelay->SendRW_CHANGEMAP_ACK(dwCharID, wMapID, MAKEWORD( BYTE(INT(fPosX) / UNIT_SIZE), BYTE(INT(fPosZ) / UNIT_SIZE)));

	// 메인서버로 로딩된 데이터 셑팅
	// 맵 이동이나 메인서버 변경 등의 기준 데이터로 사용됨
	pTCHAR->m_bCountry = bCountry;
	pTCHAR->m_dwRegion = dwRegion;
	pTCHAR->m_bLogout = bLogout;
	pTCHAR->m_bRealSex = bRealSex;
	pTCHAR->m_bClass = bClass;
	pTCHAR->m_bSave = bSave;
	pTCHAR->m_bRace = bRace;
	pTCHAR->m_bFace = bFace;
	pTCHAR->m_bHair = bHair;
	pTCHAR->m_bSex = bSex;
	pTCHAR->m_bHelmetHide = bHelmetHide;

	pTCHAR->m_bChannel = bChannel;
	pTCHAR->m_wMapID = wMapID;
	pTCHAR->m_fPosX = fPosX;
	pTCHAR->m_fPosY = fPosY;
	pTCHAR->m_fPosZ = fPosZ;
	pTCHAR->m_bAidCountry = bAidCountry;

	SetCharLevel(pTCHAR, bLevel);

	if(bResult)
	{
		// Enter server error, 중복 접속 등으로 맵서버를 통해 클라이언트에 오류메세지를 전달한 후 클라이언트의 연결을 종료
		pMAIN->SendMW_CONRESULT_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY,
			bResult,
			pTCHAR->m_mapTCHARCON);

		return EC_NOERROR;
	}
	// 인증결과 유효한 연결로 확인됨

	if(pTCHAR->m_bCHGMainID)
	{
		// 캐릭터 오브젝트가 메인서버 교체 중 상태로 셑팅된 경우

		// 캐릭터 오브젝트를 메인서버 교체완료 상태로 셑팅
		pTCHAR->m_bCHGMainID = FALSE;

		// 추가로 필요한 연결목록을 메인서버에 요청
		pMAIN->SendMW_MAPSVRLIST_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY,
			pTCHAR->m_bChannel,
			pTCHAR->m_wMapID,
			pTCHAR->m_fPosX,
			pTCHAR->m_fPosY,
			pTCHAR->m_fPosZ);
	}
	else
	{
		// 처음으로 맵에 로그인 했을 때
		CString strFindName = strNAME;
		strFindName.MakeUpper();
		m_mapTCHARNAME.insert( MAPTCHARACTERNAME::value_type( strFindName, pTCHAR));
		pTCHAR->m_strNAME = strNAME;

		CTGuild *pGuild = NULL;

		BYTE bDuty = GUILD_DUTY_NONE;
		BYTE bPeer = GUILD_PEER_NONE;
		WORD wCastle = 0;
		BYTE bCamp = 0;
		DWORD dwGuildID = 0;
		DWORD dwTacticsID = 0;
	
		MAPDWORD::iterator itGM = m_mapCharGuild.find(dwCharID);
		if(itGM != m_mapCharGuild.end())
			dwGuildID = (*itGM).second;

		if(dwGuildID)
		{
			pGuild = FindTGuild(dwGuildID);
			if(pGuild)
			{
				LPTGUILDMEMBER pMem = pGuild->FindMember(dwCharID);
				if(pMem)
				{
					pTCHAR->m_pGuild = pGuild;
					pGuild->SetMemberConnection( dwCharID, pTCHAR, m_timeCurrent);
					bDuty = pMem->m_bDuty;
					bPeer = pMem->m_bPeer;
					wCastle = pMem->m_wCastle;
					bCamp = pMem->m_bCamp;
				}
			}
		}

		MAPDWORD::iterator itTM = m_mapCharTactics.find(dwCharID);
		if(itTM != m_mapCharTactics.end())
			dwTacticsID = (*itTM).second;

		if(dwTacticsID)
		{
			pGuild = FindTGuild(dwTacticsID);
			if(pGuild)
			{
				LPTTACTICSMEMBER pTac = pGuild->FindTactics(dwCharID);
				if(pTac)
				{
					pTCHAR->m_pTactics = pGuild;
					pGuild->SetTacticsConnection( dwCharID, pTCHAR);

					if(!wCastle)
					{
						wCastle = pTac->m_wCastle;
						bCamp = pTac->m_bCamp;
					}
				}
			}
		}

		// 메인 서버를 통해 CS_CHARINFO_ACK등 캐릭터 정보를 클라이언트로 전송
		// 클라이언트는 이 시점에 맵 로딩이 시작됨
		pMAIN->SendMW_CHARINFO_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY,
			pTCHAR->m_pGuild,
			pTCHAR->m_pTactics,
			pTCHAR->m_pParty,
			bDuty,
			bPeer,
			wCastle,
			bCamp);

		// 메인 서버를 통해 더 필요한 커넥션이 있는지 체크
		pMAIN->SendMW_ROUTE_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY,
			pTCHAR->m_bChannel,
			pTCHAR->m_wMapID,
			pTCHAR->m_fPosX,
			pTCHAR->m_fPosY,
			pTCHAR->m_fPosZ);

		// DBThread로 소울메이트를 요청
		SendDM_SOULMATELIST_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY);

		// DBThread로 친구목록을 요청
		SendDM_FRIENDLIST_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY);

		CheckChatBan(pTCHAR, FALSE);

#ifdef __TW_APEX
		if(m_bNation == NATION_TAIWAN)
			ApexNotifyUserEnter(dwCharID, strNAME, dwUserIP);
#endif

	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_TELEPORT_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bServerID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bServerID;

	LPTCHARACTER pTCHAR = FindTChar( dwCharID, dwKEY);
	if(!pTCHAR)
	{
		pSERVER->SendMW_DELCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE,
			FALSE);

		return EC_NOERROR;
	}

	CTServer *pTMAP = FindMapSvr(bServerID);
	if(!pTMAP)
	{
		// 유효하지 않은 서버로 텔레포트 시도
		// 클라이언트에 오류메세지를 전송한 후 클라이언트의 연결을 종료
		pSERVER->SendMW_TELEPORT_REQ(
			dwCharID,
			dwKEY,
			pTCHAR->m_bChannel,
			pTCHAR->m_wMapID,
			pTCHAR->m_fPosX,
			pTCHAR->m_fPosY,
			pTCHAR->m_fPosZ,
			TPR_NODESTINATION);
		CloseChar(pTCHAR);

		return EC_NOERROR;
	}

	pTCHAR->m_bPartyWaiter = FALSE;

	// 클라이언트에게 텔레포트 승인메세지를 전송한다
	// 이 때 클라이언트에서 실질적인 텔레포트 실행
	pSERVER->SendMW_TELEPORT_REQ(
		dwCharID,
		dwKEY,
		pTCHAR->m_bChannel,
		pTCHAR->m_wMapID,
		pTCHAR->m_fPosX,
		pTCHAR->m_fPosY,
		pTCHAR->m_fPosZ,
		TPR_SUCCESS);

	// 텔레포트에대한 목적지 서버에 필요한 연결목록을 요청
	pTMAP->SendMW_CONLIST_REQ(
		dwCharID,
		dwKEY,
		pTCHAR->m_bChannel,
		pTCHAR->m_wMapID,
		pTCHAR->m_fPosX,
		pTCHAR->m_fPosY,
		pTCHAR->m_fPosZ);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_FRIENDLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_FRIENDLIST_ACK)
		<< dwCharID
		<< dwKEY;

	WORD wSize = pMSG->m_packet.GetSize();
	BYTE bCount=0;
	pMSG->m_packet
		<< bCount;

	DEFINE_QUERY(&m_db, CTBLFriendGroupTable)
	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_bGroup
				<< query->m_szName;

			bCount++;
		}
		query->Close();
	}
	UNDEFINE_QUERY()
	memcpy(pMSG->m_packet.GetBuffer()+wSize,&bCount,sizeof(bCount));

	MAPTFRIENDTEMP mapFriend;
	mapFriend.clear();

	DEFINE_QUERY(&m_db, CTBLFriend)
	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			TFRIEND f;
			f.m_dwID = query->m_dwFriendID;
			f.m_strName = query->m_szName;
			f.m_bLevel = query->m_bLevel;
			f.m_bGroup = query->m_bGroup;
			f.m_bClass = query->m_bClass;

			mapFriend.insert(MAPTFRIENDTEMP::value_type(f.m_dwID, f));
		}
		query->Close();
	}
	UNDEFINE_QUERY();

	pMSG->m_packet
		<< (BYTE)mapFriend.size();

	DEFINE_QUERY(&m_db, CSPProtectedSearch)
	MAPTFRIENDTEMP::iterator it;
	for(it=mapFriend.begin(); it!=mapFriend.end(); it++)
	{
		query->m_dwCharID = (*it).second.m_dwID;
		query->m_dwProtected = dwCharID;
		query->Call();

		pMSG->m_packet
			<< (*it).second.m_dwID
			<< (*it).second.m_strName
			<< (*it).second.m_bLevel
			<< (*it).second.m_bGroup
			<< (*it).second.m_bClass
			<< (BYTE)query->m_nRET;
	}
	UNDEFINE_QUERY();

	mapFriend.clear();

	DEFINE_QUERY(&m_db, CTBLFriendTarget)
	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			TFRIEND f;
			f.m_dwID = query->m_dwFriendID;
			f.m_strName = query->m_szName;
			mapFriend.insert(MAPTFRIENDTEMP::value_type(f.m_dwID, f));
		}
		query->Close();
	}
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CSPProtectedSearch)
	MAPTFRIENDTEMP::iterator it;
	for(it=mapFriend.begin(); it!=mapFriend.end(); it++)
	{
		query->m_dwCharID = dwCharID;
		query->m_dwProtected = (*it).second.m_dwID;
		query->Call();

		pMSG->m_packet
			<< (*it).second.m_dwID
			<< (*it).second.m_strName
			<< (BYTE)query->m_nRET;
	}
	UNDEFINE_QUERY();

	mapFriend.clear();

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_FRIENDLIST_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pPlayer = FindTChar(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->m_bDBLoading = TRUE;

	BYTE bCount;
	pBUF->m_packet
		>> bCount;

	BYTE bGroup;
	CString strName;

	pPlayer->m_mapFRIENDGROUP.clear();
	for(BYTE i=0; i<bCount; i++)
	{
		pBUF->m_packet
			>> bGroup
			>> strName;

		pPlayer->m_mapFRIENDGROUP.insert(MAPBSTRING::value_type(bGroup, strName));
	}

	BYTE bRet;

	pBUF->m_packet
		>> bCount;

	LPTCHARACTER pFriendChar = NULL;

	for(BYTE i=0; i<bCount; i++)
	{
		LPTFRIEND pFriend = new TFRIEND();
		pBUF->m_packet
			>> pFriend->m_dwID
			>> pFriend->m_strName
			>> pFriend->m_bLevel
			>> pFriend->m_bGroup
			>> pFriend->m_bClass
			>> bRet;

		pFriend->m_bType = FT_FRIEND;
		pFriendChar = FindTChar(pFriend->m_strName);
		if(pFriendChar && pFriendChar->m_bDBLoading)
		{
			if(bRet)
				pFriend->m_bConnected = FALSE;
			else
			{
				pFriend->m_bConnected = TRUE;
				pFriend->m_dwRegion = pFriendChar->m_dwRegion;
				MAPTFRIEND::iterator it = pFriendChar->m_mapTFRIEND.find(dwCharID);
				if(it!=pFriendChar->m_mapTFRIEND.end())
				{
					(*it).second->m_bConnected = TRUE;
					(*it).second->m_dwRegion = pPlayer->m_dwRegion;
				}
			}
		}
		else
			pFriend->m_bConnected = FALSE;

		pPlayer->m_mapTFRIEND.insert(MAPTFRIEND::value_type(pFriend->m_dwID, pFriend));
	}

	while(!pBUF->m_packet.IsEOF())
	{
		DWORD dwFriend;
		CString strName;
		pBUF->m_packet
			>> dwFriend
			>> strName
			>> bRet;

		LPTCHARACTER pChar = NULL;

		MAPTFRIEND::iterator itF = pPlayer->m_mapTFRIEND.find(dwFriend);
		if(itF!=pPlayer->m_mapTFRIEND.end())
		{
			(*itF).second->m_bType = FT_FRIENDFRIEND;
			pChar = FindTChar((*itF).second->m_strName);
		}
		else
		{
			LPTFRIEND pFriend = new TFRIEND();
			pFriend->m_dwID = dwFriend;
			pFriend->m_strName = strName;
			pFriend->m_bType = FT_TARGET;
			pFriendChar = FindTChar(strName);
			if(pFriendChar && pFriendChar->m_bDBLoading)
			{
				pFriend->m_bConnected = TRUE;
				pChar = pFriendChar;
			}
			else
				pFriend->m_bConnected = FALSE;

			pPlayer->m_mapTFRIEND.insert(MAPTFRIEND::value_type(pFriend->m_dwID, pFriend));

			if(pChar)
			{
				MAPTFRIEND::iterator it = pChar->m_mapTFRIEND.find(dwCharID);
				if(it!=pChar->m_mapTFRIEND.end())
				{
					if(bRet)
					{
						(*it).second->m_bConnected = FALSE;
						pFriend->m_bConnected = FALSE;
						pChar = NULL;
					}
					else
					{
						(*it).second->m_bConnected = TRUE;
						(*it).second->m_dwRegion = pPlayer->m_dwRegion;
					}
				}
			}
		}

		if(pChar)
		{
			CTServer *pCon = FindMapSvr(pChar->m_bMainID);

			if(pCon)
			{
				pCon->SendMW_FRIENDCONNECTION_REQ(
					dwFriend,
					pChar->m_dwKEY,
					FRIEND_CONNECTION,
					pPlayer->m_strNAME,
					pPlayer->m_dwRegion);
			}
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_FRIENDLIST_ACK(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	pSERVER->SendMW_FRIENDLIST_REQ(pTCHAR);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_ROUTE_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bCount;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
	{
		// MW_ADDCHAR_ACK를 통한 연결이 아니거나 다른 요인에 의해 삭제되어 유효하지 않은 연결
		// 맵 서버를 통해 클라이언트의 연결을 종료
		pSERVER->SendMW_DELCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE,
			FALSE);

		return EC_NOERROR;
	}

	if(!bCount)
	{
		// 추가 커넥션이 필요 없는 경우 MW_CHARDATA_ACK를 통해 메인서버를 체크함
		pSERVER->SendMW_CHARDATA_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY);

		return EC_NOERROR;
	}
	CPacket *pMSG = new CPacket();

	// 추가 커넥션을 요청한 맵 서버를 통해 클라이언트로 필요한 추가 커넥션 정보를 전송
	pMSG->SetID(MW_ADDCONNECT_REQ)
		<< pTCHAR->m_dwCharID
		<< pTCHAR->m_dwKEY
		<< bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		// 필요한 추가 커넥션을 연결상태(m_bValid)를 연결되지 않음
		// 데이터 로드 상태(m_bReady)를 로드되지 않음으로
		// 셑팅하여 캐릭터 오브젝트에 등록하여 이 후 MW_ADDCHAR_ACK에서
		// 유효성 검사 후 연결상태를 연결됨으로 셑팅하고 MW_CHARDATA_ACK에서
		// 데이터 로드 상태를 로드됨으로 셑팅하여 모든 커넥션에 대해 준비가 완료되면
		// 메인서버를 체크하도록 한다.
		LPTCHARCON pCON = new TCHARCON();
		BYTE bServerID;

		pCON->m_bReady = FALSE;
		pCON->m_bValid = FALSE;

		pBUF->m_packet
			>> pCON->m_dwIPAddr
			>> pCON->m_wPort
			>> bServerID;

		(*pMSG)
			<< pCON->m_dwIPAddr
			<< pCON->m_wPort
			<< bServerID;

		MAPTCHARCON::iterator itCON = pTCHAR->m_mapTCHARCON.find(bServerID);
		if( itCON != pTCHAR->m_mapTCHARCON.end() )
		{
			pCON->m_bValid = (*itCON).second->m_bValid;
			delete (*itCON).second;
			pTCHAR->m_mapTCHARCON.erase(itCON);
		}

		pTCHAR->m_mapTCHARCON.insert( MAPTCHARCON::value_type( bServerID, pCON));
	}
	pSERVER->Say(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CLOSECHAR_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
	{
		pSERVER->SendMW_DELCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE,
			FALSE);

		return EC_NOERROR;
	}
	CloseChar(pTCHAR);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CONLIST_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bCount;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
	{
		pSERVER->SendMW_DELCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE,
			FALSE);

		return EC_NOERROR;
	}

	CTServer *pMAIN = FindMapSvr(pTCHAR->m_bMainID);
	if(!pMAIN)
	{
		// 메인서버가 없거나 다르게 설정되어 공식 배포된 클라이언트가 아니거나 해킹으로 의심되는 연결
		// 맵 서버를 통해 클라이언트에게 오류메세지를 전달한 후 클라이언트의 연결을 종료
		pSERVER->SendMW_INVALIDCHAR_REQ(
			dwCharID,
			dwKEY);

		return EC_NOERROR;
	}

	MAPBYTE mapCON;
	mapCON.clear();

	// 필요한 연결목록 구성
	for( BYTE i=0; i<bCount; i++)
	{
		BYTE bServerID;

		pBUF->m_packet
			>> bServerID;

		mapCON.insert( MAPBYTE::value_type( bServerID, bServerID));
	}
	mapCON.insert( MAPBYTE::value_type( LOBYTE(pSERVER->m_wID), LOBYTE(pSERVER->m_wID)));

	// 필요없는 연결들을 종료대기 버퍼로 이동
	MAPTCHARCON::iterator itCON = pTCHAR->m_mapTCHARCON.begin();
	while(itCON != pTCHAR->m_mapTCHARCON.end())
	{
		MAPTCHARCON::iterator itNEXT = itCON;
		itNEXT++;

		if( mapCON.find((*itCON).first) == mapCON.end() )
		{
			pTCHAR->m_vTDEADCON.push_back((*itCON).first);

			delete (*itCON).second;
			pTCHAR->m_mapTCHARCON.erase(itCON);
		}

		itCON = itNEXT;
	}

	// 새롭게 연결되어야 하는 연결목록 구성
	MAPBYTE::iterator itID = mapCON.begin();
	while(itID != mapCON.end())
	{
		MAPBYTE::iterator itNEXT = itID;
		itNEXT++;

		if( pTCHAR->m_mapTCHARCON.find((*itID).first) != pTCHAR->m_mapTCHARCON.end() )
			mapCON.erase(itID);

		itID = itNEXT;
	}

	if(!mapCON.empty())
	{
		// 새롭게 연결되어야 하는 연결이 있으면 메인서버로 해당 연결의 접속정보(IP, 포트 등)를 요청
		// 이 때 맵 서버는 MW_ROUTE_ACK를 이용하여 접속정보를 반환하여 추가 연결관리 프로세스 실행
		CPacket *pMSG = new CPacket();

		pMSG->SetID(MW_ROUTELIST_REQ)
			<< pTCHAR->m_dwCharID
			<< pTCHAR->m_dwKEY
			<< BYTE(mapCON.size());

		for( itID = mapCON.begin(); itID != mapCON.end(); itID++)
			(*pMSG) << (*itID).first;

		pMAIN->Say(pMSG);
	}
	else
	{
		// 새롭게 연결되어야 하는 연결이 없으면 즉시 메인서버 체크
		CheckMainCON(pTCHAR);
	}
	mapCON.clear();

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_MAPSVRLIST_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bCount;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
	{
		pSERVER->SendMW_DELCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE,
			FALSE);

		return EC_NOERROR;
	}

	CTServer *pMAIN = FindMapSvr(pTCHAR->m_bMainID);
	if(!pMAIN)
	{
		// 메인서버가 없거나 다르게 설정되어 공식 배포된 클라이언트가 아니거나 해킹으로 의심되는 연결
		// 맵 서버를 통해 클라이언트에게 오류메세지를 전달한 후 클라이언트의 연결을 종료
		pSERVER->SendMW_INVALIDCHAR_REQ(
			dwCharID,
			dwKEY);

		return EC_NOERROR;
	}

	MAPBYTE mapCON;
	mapCON.clear();

	// 필요한 연결목록 구성
	for( BYTE i=0; i<bCount; i++)
	{
		BYTE bServerID;

		pBUF->m_packet
			>> bServerID;

		mapCON.insert( MAPBYTE::value_type( bServerID, bServerID));
	}
	mapCON.insert( MAPBYTE::value_type( LOBYTE(pSERVER->m_wID), LOBYTE(pSERVER->m_wID)));

	// 필요없는 연결들을 종료대기 버퍼로 이동
	MAPTCHARCON::iterator itCON = pTCHAR->m_mapTCHARCON.begin();
	while(itCON != pTCHAR->m_mapTCHARCON.end())
	{
		MAPTCHARCON::iterator itNEXT = itCON;
		itNEXT++;

		if( mapCON.find((*itCON).first) == mapCON.end() )
		{
			pTCHAR->m_vTDEADCON.push_back((*itCON).first);

			delete (*itCON).second;
			pTCHAR->m_mapTCHARCON.erase(itCON);
		}

		itCON = itNEXT;
	}

	// 새롭게 연결되어야 하는 연결목록 구성
	MAPBYTE::iterator itID = mapCON.begin();
	while(itID != mapCON.end())
	{
		MAPBYTE::iterator itNEXT = itID;
		itNEXT++;

		if( pTCHAR->m_mapTCHARCON.find((*itID).first) != pTCHAR->m_mapTCHARCON.end() )
			mapCON.erase(itID);

		itID = itNEXT;
	}

	if(!mapCON.empty())
	{
		// 새롭게 연결되어야 하는 연결이 있으면 메인서버로 해당 연결의 접속정보(IP, 포트 등)를 요청
		// 이 때 맵 서버는 MW_ROUTE_ACK를 이용하여 접속정보를 반환하여 추가 연결관리 프로세스 실행
		CPacket *pMSG = new CPacket();

		pMSG->SetID(MW_ROUTELIST_REQ)
			<< pTCHAR->m_dwCharID
			<< pTCHAR->m_dwKEY
			<< BYTE(mapCON.size());

		for( itID = mapCON.begin(); itID != mapCON.end(); itID++)
			(*pMSG) << (*itID).first;

		pMAIN->Say(pMSG);
	}
	else
	{
		// 새롭게 연결되어야 하는 연결이 없으면 즉시 메인서버 체크
		CheckMainCON(pTCHAR);
	}
	mapCON.clear();

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CHECKCONNECT_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
	{
		pSERVER->SendMW_DELCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE,
			FALSE);

		return EC_NOERROR;
	}

	// 연결관리 프로세스 큐에 등록 한 후 이미 실행 중이거나 실행대기 중인 연결관리 프로세스가 있으면
	// 먼저 등록된 연결관리 프로세스가 끝날때까지 대기
	if(PushConCess( pTCHAR, pBUF))
		return EC_NOERROR;

	// 이미 실행 중이거나 실행대기 중인 연결관리 프로세스가 없으면 연결확인 시작
	pBUF->m_packet.Rewind(FALSE);
	OnCheckConnect( pTCHAR, pBUF);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_RELEASEMAIN_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	BYTE bDBLoad;
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> bDBLoad
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
	{
		pSERVER->SendMW_DELCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE,
			FALSE);

		return EC_NOERROR;
	}

	CTServer *pMAIN = FindMapSvr(pTCHAR->m_bMainID);
	if(!pMAIN)
	{
		// 임시 메인서버가 없거나 다르게 설정되어 공식 배포된 클라이언트가 아니거나 해킹으로 의심되는 연결
		// 맵 서버를 통해 클라이언트에게 오류메세지를 전달한 후 클라이언트의 연결을 종료
		pSERVER->SendMW_INVALIDCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE);

		return EC_NOERROR;
	}

	// 임시 메인서버로 입장승인 메세지를 전달하고 캐릭터 오브젝트를 메인서버 교체 중 상태로 셑팅
	// 이 시점에 계정인증 절차를 거친 후 캐릭터 데이터가 임시 메인서버에 로드 된다
	pMAIN->SendMW_ENTERSVR_REQ(pBUF);

	pTCHAR->m_bCHGMainID = LOBYTE(pSERVER->m_wID);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CHGPARTYCHIEF_ACK(LPPACKETBUF pBUF)
{
	DWORD dwChiefID;
	DWORD dwKEY;
	DWORD dwTargetID;

	pBUF->m_packet
		>> dwChiefID
		>> dwKEY
		>> dwTargetID;

	LPTCHARACTER pChief = FindTChar(
		dwChiefID,
		dwKEY);
    if(!pChief)
		return EC_NOERROR;

	MAPTCHARACTER::iterator itTarget = m_mapTCHAR.find(dwTargetID);
	if(itTarget == m_mapTCHAR.end())
	{
		CTServer * pCon = FindMapSvr(pChief->m_bMainID);
		if(pCon)
			pCon->SendMW_CHGPARTYCHIEF_REQ(
				pChief->m_dwCharID,
				pChief->m_dwKEY,
				PARTY_NOUSER);
		
		return EC_NOERROR;
	}

	LPTCHARACTER pTarget = (*itTarget).second;
	if(!pChief->m_pParty || !pTarget->m_pParty)
	{
		CTServer * pCon = FindMapSvr(pChief->m_bMainID);
		if(pCon)
			pCon->SendMW_CHGPARTYCHIEF_REQ(
				pChief->m_dwCharID,
				pChief->m_dwKEY,
				PARTY_NOPARTY);

		return EC_NOERROR;
	}

	if(!pChief->m_pParty->IsChief(pChief->m_dwCharID))
	{
		CTServer * pCon = FindMapSvr(pChief->m_bMainID);
		if(pCon)
			pCon->SendMW_CHGPARTYCHIEF_REQ(
				pChief->m_dwCharID,
				pChief->m_dwKEY,
				PARTY_NOTCHIEF);

		return EC_NOERROR;
	}

	if(pChief->m_pParty->GetID() != pTarget->m_pParty->GetID())
	{
		CTServer * pCon = FindMapSvr(pChief->m_bMainID);
		if(pCon)
			pCon->SendMW_CHGPARTYCHIEF_REQ(
				pChief->m_dwCharID,
				pChief->m_dwKEY,
				PARTY_NOUSER);

		return EC_NOERROR;
	}

	if(pChief->m_dwCharID == pTarget->m_dwCharID)
	{
		CTServer * pCon = FindMapSvr(pChief->m_bMainID);
		if(pCon)
			pCon->SendMW_CHGPARTYCHIEF_REQ(
				pChief->m_dwCharID,
				pChief->m_dwKEY,
				PARTY_ALREADY);

		return EC_NOERROR;
	}

	CTParty * pParty = pTarget->m_pParty;
	pParty->SetChiefID(pTarget->m_dwCharID);
	if(m_pRelay)
		m_pRelay->SendRW_PARTYCHGCHIEF_ACK(pParty->GetID(), pParty->GetChiefID());

	CTServer * pMap = FindMapSvr(pChief->m_bMainID);
	if(pMap)
	{
		pMap->SendMW_CHGPARTYCHIEF_REQ(
			pChief->m_dwCharID,
			pChief->m_dwKEY,
			PARTY_CHGCHIEF);
	}

	WORD wCommander = 0;
	CTCorps * pCorps = FindCorps(pParty->m_wCorpsID);
	if(pCorps)
	{
		wCommander = pCorps->m_wCommander;
		if(pCorps->m_wCommander == pParty->GetID())
			pCorps->m_dwGeneralID = pParty->GetChiefID();
	}

    for(int i=0; i<pParty->GetSize(); i++)
		PartyAttr(pParty->GetMember(i));

	ChgSquadChief(pCorps, pParty);

	if(pCorps)
		ReportEnemyList(pCorps, pParty->GetID(), pParty->GetChiefID());

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CHGPARTYTYPE_ACK( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bPartyType;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bPartyType;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKEY);
	if(!pTCHAR)
		return EC_NOERROR;

	if(!pTCHAR->m_pParty)
		return EC_NOERROR;

	if(!pTCHAR->m_pParty->IsChief(pTCHAR->m_dwCharID))
	{
		CTServer *pCon = FindMapSvr(pTCHAR->m_bMainID);
		if(pCon)
			pCon->SendMW_CHGPARTYTYPE_REQ(pTCHAR->m_dwCharID, pTCHAR->m_dwKEY, PARTY_NOTCHIEF, bPartyType);
		return EC_NOERROR;
	}

	pTCHAR->m_pParty->m_bObtainType = bPartyType;

	for(BYTE i=0; i<pTCHAR->m_pParty->GetSize(); i++)
	{
		LPTCHARACTER pMember = pTCHAR->m_pParty->GetMember(i);
		CTServer *pCon = FindMapSvr(pMember->m_bMainID);
		if(pCon)
			pCon->SendMW_CHGPARTYTYPE_REQ(pMember->m_dwCharID, pMember->m_dwKEY, 0, bPartyType);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_PARTYADD_ACK(LPPACKETBUF pBUF)
{
	CString strRequest;
	CString strTarget;
	BYTE bObtainType;
	DWORD dwMaxHP;
	DWORD dwHP;
	DWORD dwMaxMP;
	DWORD dwMP;

	pBUF->m_packet
		>> strRequest
		>> strTarget
		>> bObtainType
		>> dwMaxHP
		>> dwHP
		>> dwMaxMP
		>> dwMP;

	LPTCHARACTER pRequest = FindTChar(strRequest);
	LPTCHARACTER pTarget = FindTChar(strTarget);

	if(!pRequest || pRequest == pTarget)
		return EC_NOERROR;

	if(!pTarget)
	{
		CTServer *pCon = FindMapSvr(pRequest->m_bMainID);
		if(pCon)
			pCon->SendMW_PARTYADD_REQ(
				pRequest->m_dwCharID,
				pRequest->m_dwKEY,
				strRequest,
				strTarget,
				bObtainType,
				PARTY_NOUSER);

		return EC_NOERROR;
	}

	if(pTarget->m_bPartyWaiter)
	{
		CTServer *pCon = FindMapSvr(pRequest->m_bMainID);
		if(pCon)
			pCon->SendMW_PARTYADD_REQ(
				pRequest->m_dwCharID,
				pRequest->m_dwKEY,
				strRequest,
				strTarget,
				bObtainType,
				PARTY_WAITERS);

		return EC_NOERROR;
	}

	if(pTarget->m_pParty)
	{
		CTServer *pCon = FindMapSvr(pRequest->m_bMainID);
		if(pCon)
			pCon->SendMW_PARTYADD_REQ(
				pRequest->m_dwCharID,
				pRequest->m_dwKEY,
				strRequest,
				strTarget,
				bObtainType,
				PARTY_ALREADY);

		return EC_NOERROR;		
	}

	if(GetWarCountry(pTarget) != GetWarCountry(pRequest))
	{
		CTServer *pCon = FindMapSvr(pRequest->m_bMainID);
		if(pCon)
			pCon->SendMW_PARTYADD_REQ(
				pRequest->m_dwCharID,
				pRequest->m_dwKEY,
				strRequest,
				strTarget,
				bObtainType,
				PARTY_COUNTRY);

		return EC_NOERROR;
	}

	if(pRequest->m_pParty)
	{
		if(!pRequest->m_pParty->IsChief(pRequest->m_dwCharID))
		{
			CTServer *pCon = FindMapSvr(pRequest->m_bMainID);
			if(pCon)
				pCon->SendMW_PARTYADD_REQ(
					pRequest->m_dwCharID,
					pRequest->m_dwKEY,
					strRequest,
					strTarget,
					bObtainType,
					PARTY_NOTCHIEF);

			return EC_NOERROR;	
		}
		if(pRequest->m_pParty->GetSize() >= MAX_PARTY_MEMBER)
		{
			CTServer *pCon = FindMapSvr(pRequest->m_bMainID);
			if(pCon)
				pCon->SendMW_PARTYADD_REQ(
					pRequest->m_dwCharID,
					pRequest->m_dwKEY,
					strRequest,
					strTarget,
					bObtainType,
					PARTY_FULL);

			return EC_NOERROR;	
		}
		if(pRequest->m_pParty->m_bArena)
			return EC_NOERROR;
	}

	SetCharStatus(pRequest, dwMaxHP, dwHP, dwMaxMP, dwMP);

	CTServer *pCon = FindMapSvr(pTarget->m_bMainID);
	if(pCon)
	{
		pCon->SendMW_PARTYADD_REQ(
			pTarget->m_dwCharID,
			pTarget->m_dwKEY,
			strRequest,
			strTarget,
			bObtainType,
			PARTY_AGREE,
			pRequest->m_dwCharID);

		pTarget->m_bPartyWaiter = TRUE;
	}
	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_PARTYJOIN_ACK(LPPACKETBUF pBUF)
{
	CString strOrigin;
	CString strTarget;
	BYTE bObtainType;
	BYTE bResponse;
	DWORD dwMaxHP;
	DWORD dwHP;
	DWORD dwMaxMP;
	DWORD dwMP;

	pBUF->m_packet
		>> strOrigin
		>> strTarget
		>> bObtainType
		>> bResponse
		>> dwMaxHP
		>> dwHP
		>> dwMaxMP
		>> dwMP;

	LPTCHARACTER pOrigin = FindTChar(strOrigin);
	LPTCHARACTER pTarget = FindTChar(strTarget);

	if(pTarget)
		pTarget->m_bPartyWaiter = FALSE;

	if(!pOrigin && !pTarget)
		return EC_NOERROR;

	if(!pOrigin && pTarget)
	{
		CTServer * pCon = FindMapSvr(pTarget->m_bMainID);
		if(pCon)
			pCon->SendMW_PARTYADD_REQ(
				pTarget->m_dwCharID,
				pTarget->m_dwKEY,
				strOrigin,
				strTarget,
				bObtainType,
				PARTY_NOREQUSER);

		return EC_NOERROR;
	}
	if(pOrigin && !pTarget)
	{
		CTServer * pCon = FindMapSvr(pOrigin->m_bMainID);
		if(pCon)
			pCon->SendMW_PARTYADD_REQ(
				pOrigin->m_dwCharID,
				pOrigin->m_dwKEY,
				strOrigin,
				strTarget,
				bObtainType,
				PARTY_NOUSER);

		return EC_NOERROR;
	}
	if(bResponse != ASK_YES)
	{
		CTServer * pCon = FindMapSvr(pOrigin->m_bMainID);
		if(pCon)
			pCon->SendMW_PARTYADD_REQ(
				pOrigin->m_dwCharID,
				pOrigin->m_dwKEY,
				strOrigin,
				strTarget,
				bObtainType,
				bResponse);

		return EC_NOERROR;
	}

	if(pTarget->m_pParty)
	{
		CTServer * pCon = FindMapSvr(pOrigin->m_bMainID);
		if(pCon)
			pCon->SendMW_PARTYADD_REQ(
				pOrigin->m_dwCharID,
				pOrigin->m_dwKEY,
				strOrigin,
				strTarget,
				bObtainType,
				PARTY_NOUSER);

		return EC_NOERROR;
	}

	if(GetWarCountry(pTarget) != GetWarCountry(pOrigin))
	{
		CTServer *pCon = FindMapSvr(pTarget->m_bMainID);
		if(pCon)
			pCon->SendMW_PARTYADD_REQ(
				pTarget->m_dwCharID,
				pTarget->m_dwKEY,
				strOrigin,
				strTarget,
				bObtainType,
				PARTY_COUNTRY);

		pCon = FindMapSvr(pOrigin->m_bMainID);
		if(pCon)
			pCon->SendMW_PARTYADD_REQ(
				pOrigin->m_dwCharID,
				pOrigin->m_dwKEY,
				strOrigin,
				strTarget,
				bObtainType,
				PARTY_COUNTRY);

		return EC_NOERROR;
	}

	SetCharStatus(pTarget, dwMaxHP, dwHP, dwMaxMP, dwMP);

	CTParty * pParty = NULL;

	CTServer * pTgCon = FindMapSvr(pTarget->m_bMainID);
	CTServer * pOgCon = FindMapSvr(pOrigin->m_bMainID);
	if(!pTgCon || !pOgCon)
		return EC_NOERROR;

	if(pOrigin->m_pParty)
	{
		if(!pOrigin->m_pParty->IsChief(pOrigin->m_dwCharID))
		{
			pOgCon->SendMW_PARTYADD_REQ(
				pOrigin->m_dwCharID,
				pOrigin->m_dwKEY,
				strOrigin,
				strTarget,
				bObtainType,
				PARTY_NOTCHIEF);

			pTgCon->SendMW_PARTYADD_REQ(
				pTarget->m_dwCharID,
				pTarget->m_dwKEY,
				strOrigin,
				strTarget,
				bObtainType,
				PARTY_NOTCHIEF);

			return EC_NOERROR;
		}

		if(pOrigin->m_pParty->GetSize() >= MAX_PARTY_MEMBER)
		{
			pOgCon->SendMW_PARTYADD_REQ(
				pOrigin->m_dwCharID,
				pOrigin->m_dwKEY,
				strOrigin,
				strTarget,
				bObtainType,
				PARTY_FULL);

			pTgCon->SendMW_PARTYADD_REQ(
				pTarget->m_dwCharID,
				pTarget->m_dwKEY,
				strOrigin,
				strTarget,
				bObtainType,
				PARTY_FULL);

			return EC_NOERROR;
		}

		if(pOrigin->m_pParty->m_bArena)
		{
			pTgCon->SendMW_PARTYADD_REQ(
				pTarget->m_dwCharID,
				pTarget->m_dwKEY,
				strOrigin,
				strTarget,
				bObtainType,
				PARTY_BUSY);

			return EC_NOERROR;
		}

		pParty = pOrigin->m_pParty;
		JoinParty(pParty, pOrigin->m_dwCharID, pTarget);
	}
	else
	{
		pParty = new CTParty();
		pParty->SetID(GenPartyID());
		pParty->m_bObtainType = bObtainType;
		m_mapTParty.insert(MAPTPARTY::value_type(pParty->GetID(), pParty));
		JoinParty(pParty, pOrigin->m_dwCharID, pOrigin);
		JoinParty(pParty, pOrigin->m_dwCharID, pTarget);
	}
	
	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_PARTYDEL_ACK(LPPACKETBUF pBUF)
{
	WORD wPartyID;
	DWORD dwCharID;
	BYTE bKick;

	pBUF->m_packet
		>> wPartyID
		>> dwCharID
		>> bKick;

	CTParty * pParty = FindParty(wPartyID);
	if(!pParty)
		return EC_NOERROR;

	LPTCHARACTER pPlayer = pParty->FindMember(dwCharID);
	if(!pPlayer)
		return EC_NOERROR;

	LeaveParty(pPlayer, bKick);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_PARTYMANSTAT_ACK(LPPACKETBUF pBUF)
{
	WORD wPartyID;
	DWORD dwID;
	BYTE bType;
	BYTE bLevel;
	DWORD dwMaxHP, dwCurHP;
	DWORD dwMaxMP, dwCurMP;

	pBUF->m_packet
		>> wPartyID
		>> dwID
		>> bType
		>> bLevel
		>> dwMaxHP
		>> dwCurHP
		>> dwMaxMP
		>> dwCurMP;

	CTParty * pParty = FindParty(wPartyID);
	if(!pParty)
		return EC_NOERROR;

	for(int i=0; i<pParty->GetSize(); i++)
	{
		LPTCHARACTER pMember = pParty->GetMember(i);
		if(pMember->m_dwCharID == dwID)
			SetCharStatus(pMember, dwMaxHP, dwCurHP, dwMaxMP, dwCurMP);

		CTServer * pCon = FindMapSvr(pMember->m_bMainID);
		if(pCon)
			pCon->SendMW_PARTYMANSTAT_REQ(
			pMember->m_dwCharID,
			pMember->m_dwKEY,
			dwID,
			bType,
			bLevel,
			dwMaxHP,
			dwCurHP,
			dwMaxMP,
			dwCurMP);
	}

	if(pParty->m_wCorpsID &&
		pParty->IsChief(dwID))
	{
		CTCorps * pCorps = FindCorps(pParty->m_wCorpsID);
		if(pCorps)
		{
			MAPTCHARACTER::iterator itGe = m_mapTCHAR.find(pCorps->m_dwGeneralID);
			if(itGe != m_mapTCHAR.end())
			{
				CTServer * pCon = FindMapSvr((*itGe).second->m_bMainID);
				if(pCon)
					pCon->SendMW_PARTYMANSTAT_REQ(
					(*itGe).second->m_dwCharID,
					(*itGe).second->m_dwKEY,
					dwID,
					bType,
					bLevel,
					dwMaxHP,
					dwCurHP,
					dwMaxMP,
					dwCurMP);
			}
		}
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_LEVELUP_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bLevel;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bLevel;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	SetCharLevel(pTCHAR, bLevel);

	MAPTCHARCON::iterator itCON;
	for(itCON =pTCHAR->m_mapTCHARCON.begin(); itCON != pTCHAR->m_mapTCHARCON.end(); itCON++)
	{
		if((*itCON).second->m_bValid && (*itCON).first != pTCHAR->m_bMainID)
		{
			CTServer *pMAP = FindMapSvr((*itCON).first);
			if(pMAP)
			{
				pMAP->SendMW_LEVELUP_REQ(
					dwCharID,
					dwKEY,
					bLevel);
			}
		}
	}

	MAPTSOULMATE::iterator itSOUL;
	for(itSOUL=pTCHAR->m_mapTSOULMATE.begin(); itSOUL!=pTCHAR->m_mapTSOULMATE.end(); itSOUL++)
	{
		LPTCHARACTER pCHAR = NULL;
		LPTSOULMATE pSOUL = NULL;

		if((*itSOUL).second->m_dwCharID == pTCHAR->m_dwCharID)
		{
			pCHAR = pTCHAR;
			pSOUL = (*itSOUL).second;
		}
		else
		{
			MAPTCHARACTER::iterator it = m_mapTCHAR.find((*itSOUL).second->m_dwCharID);
			if(it!=m_mapTCHAR.end())
			{
				pCHAR = (*it).second;
				MAPTSOULMATE::iterator findSOUL = pCHAR->m_mapTSOULMATE.find(pCHAR->m_dwCharID);
				if(findSOUL!=pCHAR->m_mapTSOULMATE.end())
				{
					pSOUL = (*findSOUL).second;
					pSOUL->m_bLevel = bLevel;
				}
			}
		}

		CheckSoulmateEnd(pCHAR, pSOUL);
	}

	return EC_NOERROR;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 길드
DWORD CTWorldSvrModule::OnDM_GUILDUPDATE_REQ(LPPACKETBUF pBUF)
{
	DWORD	dwAllience;
	DWORD	dwEnemy;

	BYTE	bCount;

	DEFINE_QUERY(&m_db, CSPGuildUpdate)
	pBUF->m_packet
		>> query->m_dwID
		>> query->m_bFame
		>> query->m_bGPoint
		>> query->m_bLevel
		>> query->m_bStatus
		>> query->m_dwChief
		>> query->m_dwExp
		>> query->m_dwGI
		>> query->m_dwTime
		>> bCount;

	memset(query->m_szAllience, 0, MAX_STR_GUILD);
	memset(query->m_szEnemy, 0, MAX_STR_GUILD);
	for(int i=0; i<bCount; i++)
	{
		pBUF->m_packet
			>> dwAllience;
		itoa(dwAllience,query->m_szAllience+lstrlen(query->m_szAllience),10);
		query->m_szAllience[lstrlen(query->m_szAllience)] = ',';
	}
	if(bCount==0)
		query->m_szAllience[0] = ',';

	pBUF->m_packet
		>> bCount;
	for(BYTE i=0; i<bCount; i++)
	{
		pBUF->m_packet
			>> dwEnemy;
		itoa(dwEnemy,query->m_szEnemy+lstrlen(query->m_szEnemy),10);
		query->m_szEnemy[lstrlen(query->m_szEnemy)] = ',';
	}
	if(bCount==0)
		query->m_szEnemy[0] = ',';

	query->Call();
	UNDEFINE_QUERY()
	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDESTABLISH_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;
	DWORD dwCharID;
	DWORD dwKEY;
	CString strGuildName;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strGuildName;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR || pTCHAR->m_pGuild)
	{
		pServer->SendMW_GUILDESTABLISH_REQ(
			dwCharID,
			dwKEY,
			GUILD_HAVEGUILD,
			0,
			strGuildName,
			TRUE);

		return EC_NOERROR;
	}

	if(strGuildName.GetLength() > MAX_NAME)
		return EC_NOERROR;

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_GUILDESTABLISH_REQ)
		<< dwCharID
		<< dwKEY
		<< strGuildName;
	SayToDB(pMsg);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDESTABLISH_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet=GUILD_ESTABLISH_ERR;
	DWORD dwGuildID=0;
	CString strGuildName;
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strGuildName;

	DEFINE_QUERY(&m_db, CSPGuildEstablish)
	query->m_dwGuildID = dwGuildID;
	lstrcpy(query->m_szName, strGuildName);
	query->m_dwChiefID = dwCharID;
	__TIMETODB(m_timeCurrent, query->m_timeEstablish);
	if(query->Call())
	{
		bRet = query->m_nRET;
		dwGuildID = query->m_dwGuildID;
	}
	UNDEFINE_QUERY();

	LPPACKETBUF  pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_GUILDESTABLISH_ACK)
		<< bRet
		<< dwGuildID
		<< strGuildName
		<< m_timeCurrent
		<< dwCharID
		<< dwKEY;

	SayToBATCH(pMsg);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDESTABLISH_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwGuildID;
	CString strName;
	__time64_t timeEs;
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> bRet
		>> dwGuildID
		>> strName
		>> timeEs
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	if(!bRet)
	{
		CTGuild * pGuild = new CTGuild();
		pGuild->m_dwID = dwGuildID;
		pGuild->m_strName = strName;
		pGuild->m_bLevel = 1;
		pGuild->m_timeEstablish = timeEs;
		pGuild->m_dwChief = dwCharID;
		pGuild->m_strChief = pTCHAR->m_strNAME;
		pGuild->m_bCountry = pTCHAR->m_bCountry;
		pGuild->m_pTLEVEL = FindGuildLevel(pGuild->m_bLevel);
		m_mapTGuild.insert(MAPTGUILD::value_type(pGuild->m_dwID, pGuild));

		LPTGUILDMEMBER pMember = new TGUILDMEMBER();
		pMember->m_pChar = pTCHAR;
		pMember->m_bDuty = GUILD_DUTY_CHIEF;
		pMember->m_bPeer = GUILD_PEER_NONE;
		pMember->m_dwID = dwCharID;
		pMember->m_bLevel = pTCHAR->m_bLevel;
		pMember->m_bClass = pTCHAR->m_bClass;
		pMember->m_strName = pTCHAR->m_strNAME;
		pMember->m_dwTactics = pTCHAR->m_pTactics ? pTCHAR->m_pTactics->m_dwID : 0;
		pMember->m_bWarCountry = GetWarCountry(pTCHAR);
		GuildMemberAdd(pGuild, pMember);
		NotifyAddGuildMember(pMember, pGuild);
	}

	CTServer *pMAP = FindMapSvr(pTCHAR->m_bMainID);
	if(!pMAP)
		return EC_NOERROR;

	pMAP->SendMW_GUILDESTABLISH_REQ(
		dwCharID,
		dwKEY,
		bRet,
		dwGuildID,
		strName,
		TRUE);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDDISORGANIZATION_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bDisorg;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bDisorg;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR) 
		return EC_NOERROR;

	if(!pTCHAR->m_pGuild || pTCHAR->m_pGuild->m_bDisorg==bDisorg)
		return EC_NOERROR;

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_GUILDDISORGANIZATION_REQ)
		<< dwCharID
        << dwKEY
		<< pTCHAR->m_pGuild->m_dwID
		<< bDisorg;
	SayToDB(pMsg);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDDISORGANIZATION_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwGuildID;
	BYTE bDisorg;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwGuildID
		>> bDisorg;

	DWORD dwTime = bDisorg ? (DWORD)m_timeCurrent : 0;

	DEFINE_QUERY(&m_db, CSPGuildDisorg)
	query->m_dwGuildID = dwGuildID;
	query->m_bDisorg = bDisorg;
	query->m_dwTime = dwTime;
	query->Call();
	UNDEFINE_QUERY();

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_GUILDDISORGANIZATION_ACK)
		<< dwCharID
		<< dwKEY
		<< dwGuildID
		<< bDisorg
		<< dwTime;
	SayToBATCH(pMsg);

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(SM_GUILDDISORGANIZATION_REQ)
		<< dwGuildID
		<< dwTime
		<< bDisorg;
	SayToTIMER(pMSG);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDDISORGANIZATION_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwGuildID;
	BYTE bDisorg;
	DWORD dwTime;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwGuildID
		>> bDisorg
		>> dwTime;

	CTGuild * pGuild = FindTGuild(dwGuildID);
	if(pGuild)
	{
		pGuild->m_bDisorg = bDisorg;
		pGuild->m_dwTime = dwTime;
	}

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	CTServer *pMAP = FindMapSvr(pTCHAR->m_bMainID);
	if(!pMAP)
		return EC_NOERROR;

	pMAP->SendMW_GUILDDISORGANIZATION_REQ(
		dwCharID,
		dwKEY,
		bDisorg);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDEXTINCTION_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	BYTE bRet=0;
	pBUF->m_packet
		>> dwGuildID;

	DEFINE_QUERY(&m_db, CSPGuildDelete)
	query->m_dwGuildID = dwGuildID;
	if(query->Call())
		bRet = query->m_nRET;
	UNDEFINE_QUERY();
	
	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_GUILDEXTINCTION_ACK)
		<< dwGuildID
		<< bRet;
	SayToBATCH(pMsg);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDEXTINCTION_ACK( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	BYTE bRet;
	pBUF->m_packet
		>> dwGuildID
		>> bRet;

	if(!bRet)
		DeleteTGuild(dwGuildID);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDMEMBERADD_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwCharID;
	BYTE bLevel;
	BYTE bDuty;
		
	pBUF->m_packet
		>> dwGuildID
		>> dwCharID
		>> bLevel
		>> bDuty;

	DEFINE_QUERY(&m_db, CSPGuildMemberAdd)
	query->m_dwGuildID = dwGuildID;
	query->m_dwCharID = dwCharID;
	query->m_bLevel = bLevel;
	query->m_bDuty = bDuty;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDKICKOUT_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	CString strTarget;
	
	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strTarget;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR || !pTCHAR->m_pGuild)
		return EC_NOERROR;

	LPTGUILDMEMBER pMember = pTCHAR->m_pGuild->FindMember(strTarget);
	if(!pMember)
		return EC_NOERROR;

	if(pMember->m_bDuty!=GUILD_DUTY_NONE)
	{
		if(pTCHAR->m_pGuild->m_dwChief != dwCharID )
			return EC_NOERROR;
	}

	DWORD dwGuildID = pTCHAR->m_pGuild->m_dwID;
	DWORD dwTargetID = pMember->m_dwID;

	LPTCHARACTER pTarget = FindTChar(strTarget);
	if(pTarget)
	{
		if(pTarget->m_dwCharID != pMember->m_dwID)
			return EC_NOERROR;

		pTarget->m_pGuild = NULL;
		CTServer * pMap = FindMapSvr(pTarget->m_bMainID);
		if(pMap)
			pMap->SendMW_GUILDLEAVE_REQ(pTarget->m_dwCharID, pTarget->m_dwKEY, pTarget->m_strNAME, GUILD_LEAVE_KICK, 0);
	}

	GuildMemberDel(pTCHAR->m_pGuild, pMember, GUILD_LEAVE_KICK);

	pServer->SendMW_GUILDLEAVE_REQ(dwCharID, dwKEY, strTarget, GUILD_LEAVE_KICK, 0);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDKICKOUT_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwCharID;

	pBUF->m_packet
		>> dwGuildID
		>> dwCharID;

	DEFINE_QUERY(&m_db, CSPGuildKickout)
	query->m_dwGuildID = dwGuildID;
	query->m_dwCharID = dwCharID;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDDUTY_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	CString strTarget;
	BYTE bDuty;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strTarget
		>> bDuty;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	if( !pTCHAR->m_pGuild || pTCHAR->m_pGuild->m_bDisorg)
		return EC_NOERROR;

	LPTGUILDMEMBER pTarget = pTCHAR->m_pGuild->FindMember(strTarget);

	if(!pTarget ||
		pTarget->m_bDuty==bDuty ||
		pTarget->m_dwID == dwCharID ||
		pTarget->m_dwTactics)
		return EC_NOERROR;

	//if(bDuty == GUILD_DUTY_CHIEF &&	!pTarget->m_pChar)
	//	return EC_NOERROR;

	if(bDuty == GUILD_DUTY_VICECHIEF)
	{
		BYTE bVice=0;
		MAPTGUILDMEMBER::iterator it;
		for(it=pTCHAR->m_pGuild->m_mapTMember.begin(); it!=pTCHAR->m_pGuild->m_mapTMember.end(); it++)
		{
			if((*it).second->m_bDuty == GUILD_DUTY_VICECHIEF)
				bVice++;
		}
		if(bVice==2)
			return EC_NOERROR;
	}

	if(bDuty == GUILD_DUTY_CHIEF)
	{
		pTCHAR->m_pGuild->Designate(dwCharID, GUILD_DUTY_NONE);
		SendDM_GUILDDUTY_REQ(dwCharID, pTCHAR->m_pGuild->m_dwID, GUILD_DUTY_NONE);

		pServer->SendMW_GUILDDUTY_REQ(dwCharID, dwKEY, pTCHAR->m_strNAME, GUILD_DUTY_NONE);
		if(m_pRelay)
			m_pRelay->SendRW_GUILDCHGMASTER_ACK(pTCHAR->m_pGuild->m_dwID, pTarget->m_dwID);
	}

	pTCHAR->m_pGuild->Designate(pTarget->m_dwID, bDuty);
	SendDM_GUILDDUTY_REQ(pTarget->m_dwID, pTCHAR->m_pGuild->m_dwID, bDuty);

	if(pTarget->m_pChar)
	{
		CTServer *pMap = FindMapSvr(pTarget->m_pChar->m_bMainID);
		if(pMap)
			pMap->SendMW_GUILDDUTY_REQ(pTarget->m_dwID, pTarget->m_pChar->m_dwKEY, strTarget, bDuty);
	}

	pServer->SendMW_GUILDDUTY_REQ(dwCharID, dwKEY, strTarget, bDuty);
	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDDUTY_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwGuildID;
	BYTE bDuty;

	pBUF->m_packet
		>> dwCharID
		>> dwGuildID
		>> bDuty;

	DEFINE_QUERY(&m_db, CSPGuildDuty)
	query->m_dwCharID = dwCharID;
	query->m_dwGuildID = dwGuildID;
	query->m_bDuty = bDuty;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDPEER_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	CString strTarget;
	BYTE bPeer;
	BYTE bOldPeer;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strTarget
		>> bPeer;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	if(!pTCHAR->m_pGuild || pTCHAR->m_pGuild->m_bDisorg)
		return EC_NOERROR;

	LPTGUILDMEMBER pTarget = pTCHAR->m_pGuild->FindMember(strTarget);
	if(!pTarget || pTarget->m_bPeer==bPeer)
		return EC_NOERROR;

	bOldPeer = pTarget->m_bPeer;

	if(!pTCHAR->m_pGuild->CheckPeerage(pTarget->m_bDuty, bPeer))
	{
		pServer->SendMW_GUILDPEER_REQ(dwCharID, dwKEY, GUILD_FAIL, strTarget, bPeer, bOldPeer);
		return EC_NOERROR;
	}

	pTCHAR->m_pGuild->CreatePeerage(pTarget->m_dwID, bPeer);
	SendDM_GUILDPEER_REQ(pTarget->m_dwID, pTCHAR->m_pGuild->m_dwID, bPeer);

	if(pTarget->m_pChar && pTarget->m_pChar != pTCHAR)
	{
		CTServer *pMap = FindMapSvr(pTarget->m_pChar->m_bMainID);
		if(pMap)
			pMap->SendMW_GUILDPEER_REQ(pTarget->m_dwID, pTarget->m_pChar->m_dwKEY, GUILD_SUCCESS, strTarget, bPeer, bOldPeer);
	}

	pServer->SendMW_GUILDPEER_REQ(dwCharID, dwKEY, GUILD_SUCCESS, strTarget, bPeer, bOldPeer);
	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDPEER_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwGuildID;
	BYTE bPeer;

	pBUF->m_packet
		>> dwCharID
		>> dwGuildID
		>> bPeer;

	DEFINE_QUERY(&m_db, CSPGuildPeer)
	query->m_dwCharID = dwCharID;
	query->m_dwGuildID = dwGuildID;
	query->m_bPeer = bPeer;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDLEAVE_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	if(!pTCHAR->m_pGuild)
		return EC_NOERROR;

	LPTGUILDMEMBER pMember = pTCHAR->m_pGuild->FindMember(dwCharID);
	if(!pMember)
		return EC_NOERROR;

	DWORD dwGuildID = pTCHAR->m_pGuild->m_dwID;

	GuildMemberDel(pTCHAR->m_pGuild, pMember, GUILD_LEAVE_SELF);
	pTCHAR->m_pGuild = NULL;

	pServer->SendMW_GUILDLEAVE_REQ(dwCharID, dwKEY, pTCHAR->m_strNAME, GUILD_LEAVE_SELF, (DWORD)m_timeCurrent);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDLEAVE_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwCharID;
	BYTE bLeave;
	DWORD dwTime;

	pBUF->m_packet
		>> dwGuildID
		>> dwCharID
		>> bLeave
		>> dwTime;

	DEFINE_QUERY(&m_db, CSPGuildLeave)
	query->m_dwGuildID = dwGuildID;
	query->m_dwCharID = dwCharID;
	query->m_bLeave = bLeave;
	query->m_dwLeaveTime = dwTime;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDINVITEANSWER_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bAnswer;
	DWORD dwInviter;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bAnswer
		>> dwInviter;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	LPTCHARACTER pChief=NULL;
	MAPTCHARACTER::iterator it = m_mapTCHAR.find(dwInviter);
	if(it!=m_mapTCHAR.end())
		pChief = (*it).second;

	if(!pChief || !pTCHAR)
		return EC_NOERROR;

	CTServer * pChiefMap = FindMapSvr(pChief->m_bMainID);
	if(!pChiefMap)
		return EC_NOERROR;

	if(bAnswer != ASK_YES)
	{
		pChiefMap->SendMW_GUILDJOIN_REQ(
			pChief->m_dwCharID,
			pChief->m_dwKEY,
			bAnswer,
			0,0,0,NAME_NULL,
			0,pTCHAR->m_strNAME);

		return EC_NOERROR;
	}

	if(!pChief->m_pGuild)
	{
		pServer->SendMW_GUILDJOIN_REQ(
			dwCharID,
			dwKEY,
			GUILD_NOTFOUND,
			0,0,0,NAME_NULL,
			0,NAME_NULL);
		pChiefMap->SendMW_GUILDJOIN_REQ(
			pChief->m_dwCharID,
			pChief->m_dwKEY,
			GUILD_NOTFOUND,
			0,0,0,NAME_NULL,
			0,NAME_NULL);
		return EC_NOERROR;
	}

	if(pChief->m_pGuild->FindTactics(dwCharID))
		return EC_NOERROR;

	if(!pChief->m_pGuild->CanAddMember())
	{
		BYTE bMaxMember = pChief->m_pGuild->GetMaxMemberCnt();

		pServer->SendMW_GUILDJOIN_REQ(
			dwCharID,
			dwKEY,
			GUILD_MEMBER_FULL,
			0,0,0,NAME_NULL,
			0,NAME_NULL,bMaxMember);
		pChiefMap->SendMW_GUILDJOIN_REQ(
			pChief->m_dwCharID,
			pChief->m_dwKEY,
			GUILD_MEMBER_FULL,
			0,0,0,NAME_NULL,
			0,NAME_NULL,bMaxMember);
		return EC_NOERROR;
	}

	if(pTCHAR->m_pGuild)
	{
		pServer->SendMW_GUILDJOIN_REQ(
			dwCharID,
			dwKEY,
			GUILD_HAVEGUILD,
			0,0,0,NAME_NULL,
			0,NAME_NULL);
		pChiefMap->SendMW_GUILDJOIN_REQ(
			pChief->m_dwCharID,
			pChief->m_dwKEY,
			GUILD_HAVEGUILD,
			0,0,0,NAME_NULL,
			0,NAME_NULL);
		return EC_NOERROR;
	}

	LPTGUILDMEMBER pMember = new TGUILDMEMBER();
	pMember->m_bDuty = GUILD_DUTY_NONE;
	pMember->m_bPeer = GUILD_PEER_NONE;
	pMember->m_dwID = pTCHAR->m_dwCharID;
	pMember->m_pChar = pTCHAR;
	pMember->m_bLevel = pTCHAR->m_bLevel;
	pMember->m_bClass = pTCHAR->m_bClass;
	pMember->m_strName = pTCHAR->m_strNAME;
	pMember->m_dwTactics = FindCharTacticsID(dwCharID);
	pMember->m_bWarCountry = GetWarCountry(pTCHAR);
	GuildMemberAdd(pChief->m_pGuild, pMember);

	NotifyAddGuildMember(pMember, pChief->m_pGuild);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDINVITE_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	CString strTarget;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strTarget;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	if(!pTCHAR->m_pGuild || pTCHAR->m_pGuild->m_bDisorg)
	{
		pServer->SendMW_GUILDJOIN_REQ(
			dwCharID,
			dwKEY,
			GUILD_NOTFOUND,
			0,0,0,NAME_NULL,
			0,NAME_NULL);
		return EC_NOERROR;
	}

	if(!pTCHAR->m_pGuild->CanAddMember())
	{
		BYTE bMaxMember = pTCHAR->m_pGuild->GetMaxMemberCnt();

		pServer->SendMW_GUILDJOIN_REQ(
			dwCharID,
			dwKEY,
			GUILD_MEMBER_FULL,
			0,0,0,NAME_NULL,
			0,NAME_NULL,bMaxMember);
		return EC_NOERROR;
	}

	LPTCHARACTER pTarget=FindTChar(strTarget);

	if(pTarget)
	{
		if(pTCHAR->m_bCountry!=pTarget->m_bCountry)
		{
			pServer->SendMW_GUILDJOIN_REQ(
				dwCharID,
				dwKEY,
				GUILD_FAIL,
				0,0,0,NAME_NULL,
				0,NAME_NULL);

			return EC_NOERROR;
		}

		if(pTarget->m_pGuild)
		{
			pServer->SendMW_GUILDJOIN_REQ(
				dwCharID,
				dwKEY,
				GUILD_HAVEGUILD,
				0,0,0,NAME_NULL,
				0,NAME_NULL);
			return EC_NOERROR;
		}

		if(pTCHAR->m_pGuild->FindTactics(strTarget))
			return EC_NOERROR;

		CTServer * pMap = FindMapSvr(pTarget->m_bMainID);
		if(pMap)
			pMap->SendMW_GUILDINVITE_REQ(
				pTarget->m_dwCharID,
				pTarget->m_dwKEY,
				pTCHAR->m_pGuild->m_strName,
				pTCHAR->m_dwCharID,
				pTCHAR->m_strNAME);
	}
	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDMEMBERLIST_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	if(!pTCHAR->m_pGuild && !pTCHAR->m_pTactics)
	{
		pServer->SendMW_GUILDMEMBERLIST_REQ(
			dwCharID,
			dwKEY,
			GUILD_NOTFOUND,
			NULL);
		return EC_NOERROR;
	}

	pServer->SendMW_GUILDMEMBERLIST_REQ(
		dwCharID,
		dwKEY,
		GUILD_SUCCESS,
		GetCurGuild(pTCHAR));

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDINFO_ACK( LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	CTGuild * pCurGuild = GetCurGuild(pTCHAR);
	if(!pCurGuild)
	{
		pServer->SendMW_GUILDINFO_REQ(
			dwCharID,
			dwKEY,
			GUILD_NOTFOUND);
		return EC_NOERROR;
	}

	BYTE bDuty = 0;
	BYTE bPeer = 0;

	LPTGUILDMEMBER pMember = pCurGuild->FindMember(dwCharID);
	if(!pMember)
	{
		if(!pCurGuild->FindTactics(dwCharID))
		{
			pServer->SendMW_GUILDINFO_REQ(
				dwCharID,
				dwKEY,
				GUILD_NOTFOUND);
			return EC_NOERROR;
		}
	}
	else
	{
		bDuty = pMember->m_bDuty;
		bPeer = pMember->m_bPeer;
	}

	LPTGUILDMEMBER pChief = pCurGuild->FindMember(pCurGuild->m_dwChief);

	if(!pChief)
	{
		pServer->SendMW_GUILDINFO_REQ(
			dwCharID,
			dwKEY,
			GUILD_NOTFOUND);
		return EC_NOERROR;
	}

	pServer->SendMW_GUILDINFO_REQ(
		dwCharID,
		dwKEY,
		GUILD_SUCCESS,
		pChief,
		pCurGuild,
		bDuty,
		bPeer);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_GUILDCABINETLIST_ACK( LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR || !pTCHAR->m_pGuild || pTCHAR->m_pTactics)
		return EC_NOERROR;

	pServer->SendMW_GUILDCABINETLIST_REQ(dwCharID, dwKEY, pTCHAR->m_pGuild);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDCABINETPUTIN_ACK( LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwItemID;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR || !pTCHAR->m_pGuild)
		return EC_NOERROR;

	LPTITEM pItem = CreateItem(&(pBUF->m_packet));
	pItem->m_dwItemID = dwItemID;

	pTCHAR->m_pGuild->PutInItem(pItem->m_dwItemID, pItem);
	pServer->SendMW_GUILDCABINETLIST_REQ(
		dwCharID, 
		dwKEY,
		pTCHAR->m_pGuild);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDCABINETTAKEOUT_ACK( LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwItemID;
	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwItemID
		>> bCount;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR || !pTCHAR->m_pGuild)
		return EC_NOERROR;

	pTCHAR->m_pGuild->TakeOutItem(dwItemID, bCount);
	pServer->SendMW_GUILDCABINETLIST_REQ(
		dwCharID, 
		dwKEY,
		pTCHAR->m_pGuild);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDCONTRIBUTION_ACK( LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwExp;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;
	DWORD dwPvPoint;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwExp
		>> dwGold
		>> dwSilver
		>> dwCooper
		>> dwPvPoint;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR ||
		!pTCHAR->m_pGuild ||
		pTCHAR->m_pTactics ||
		pTCHAR->m_pGuild->m_bDisorg)
		return EC_NOERROR;

	if(pTCHAR->m_pGuild->m_bLevel == MAX_GUILD_LEVEL)
	{
		if(dwExp)
		{
			pServer->SendMW_GUILDCONTRIBUTION_REQ(dwCharID, dwKEY, GUILD_CONTRIBUTION_MAXGUILDLEVEL, 0, 0, 0, 0, 0);
			return EC_NOERROR;
		}
	}

	BYTE bResult = GUILD_NOTFOUND;
	if(pTCHAR->m_pGuild->Contribution(pTCHAR->m_dwCharID, dwExp, dwGold, dwSilver, dwCooper, dwPvPoint))
		bResult = GUILD_SUCCESS;

	pServer->SendMW_GUILDCONTRIBUTION_REQ(dwCharID, dwKEY, bResult, dwExp, dwGold, dwSilver, dwCooper, dwPvPoint);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDLEVEL_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	BYTE bLevel;

	pBUF->m_packet
		>> dwGuildID
		>> bLevel;

	DEFINE_QUERY(&m_db, CSPGuildLevel)
	query->m_dwGuildID = dwGuildID;
	query->m_bLevel = bLevel;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDCABINETMAX_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	BYTE bMaxCabinet;

	pBUF->m_packet
		>> dwGuildID
		>> bMaxCabinet;

	DEFINE_QUERY(&m_db, CSPGuildMaxCabinet)
	query->m_dwGuildID = dwGuildID;
	query->m_bMaxCabinet = bMaxCabinet;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDCONTRIBUTION_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwCharID;
	DWORD dwExp;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	pBUF->m_packet
		>> dwGuildID
		>> dwCharID
		>> dwExp
		>> dwGold
		>> dwSilver
		>> dwCooper;

	DEFINE_QUERY(&m_db, CSPGuildContribution)
	query->m_dwGuildID = dwGuildID;
	query->m_dwCharID = dwCharID;
	query->m_dwExp = dwExp;
	query->m_dwGold = dwGold;
	query->m_dwSilver = dwSilver;
	query->m_dwCooper = dwCooper;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDARTICLELIST_ACK( LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR || !pTCHAR->m_pGuild)
		return EC_NOERROR;

	pServer->SendMW_GUILDARTICLELIST_REQ(dwCharID, dwKEY, pTCHAR->m_pGuild);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDARTICLEADD_ACK( LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	CString strTitle;
	CString strArticle;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strTitle
		>> strArticle;

	if(strTitle == NAME_NULL)
		return EC_NOERROR;

	if(strTitle.GetLength() > MAX_BOARD_TITLE ||
		strArticle.GetLength() > MAX_BOARD_TEXT)
		return EC_NOERROR;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR || !pTCHAR->m_pGuild)
		return EC_NOERROR;

	LPTGUILDMEMBER pMember = pTCHAR->m_pGuild->FindMember(pTCHAR->m_dwCharID);
	if(!pMember)
		return EC_NOERROR;

	DWORD dwTime = DWORD(m_timeCurrent);

	DWORD dwID = pTCHAR->m_pGuild->AddArticle(pMember->m_bDuty, pMember->m_strName, strTitle, strArticle, dwTime);
	if(dwID)
	{
		pServer->SendMW_GUILDARTICLEADD_REQ(dwCharID, dwKEY, GUILD_SUCCESS);
		pServer->SendMW_GUILDARTICLELIST_REQ(dwCharID, dwKEY, pTCHAR->m_pGuild);
		SendDM_GUILDARTICLEADD_REQ(pTCHAR->m_pGuild->m_dwID, dwID, pMember->m_bDuty, pMember->m_strName, strTitle, strArticle, dwTime);
	}
	else
		pServer->SendMW_GUILDARTICLEADD_REQ(dwCharID, dwKEY, GUILD_FAIL);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDARTICLEADD_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwID;
	BYTE bDuty;
	CString strWritter;
	CString strTitle;
	CString strArticle;
	DWORD dwTime;

	pBUF->m_packet
		>> dwGuildID
		>> dwID
		>> bDuty
		>> strWritter
		>> strTitle
		>> strArticle
		>> dwTime;

	DEFINE_QUERY(&m_db, CSPGuildArticleAdd)
	query->m_dwGuildID = dwGuildID;
	query->m_dwID = dwID;
	query->m_bDuty = bDuty;
	lstrcpy(query->m_szWritter, strWritter);
	lstrcpy(query->m_szTitle, strTitle);
	lstrcpy(query->m_szArticle, strArticle);
	query->m_dwTime = dwTime;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDARTICLEDEL_ACK( LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwID;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR || !pTCHAR->m_pGuild)
		return EC_NOERROR;

	if(pTCHAR->m_pGuild->DelArticle(dwID))
	{
		pServer->SendMW_GUILDARTICLEDEL_REQ(dwCharID, dwKEY, GUILD_SUCCESS);
		pServer->SendMW_GUILDARTICLELIST_REQ(dwCharID, dwKEY, pTCHAR->m_pGuild);
		SendDM_GUILDARTICLEDEL_REQ(pTCHAR->m_pGuild->m_dwID, dwID);
	}
	else
		pServer->SendMW_GUILDARTICLEDEL_REQ(dwCharID, dwKEY, GUILD_FAIL);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDARTICLEDEL_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwID;

	pBUF->m_packet
		>> dwGuildID
		>> dwID;

	DEFINE_QUERY(&m_db, CSPGuildArticleDel)
	query->m_dwGuildID = dwGuildID;
	query->m_dwID = dwID;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDARTICLEUPDATE_ACK( LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwID;
	CString strTitle;
	CString strArticle;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwID
		>> strTitle
		>> strArticle;

	if(strTitle == NAME_NULL)
		return EC_NOERROR;

	if(strTitle.GetLength() > MAX_BOARD_TITLE ||
		strArticle.GetLength() > MAX_BOARD_TEXT)
		return EC_NOERROR;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR || !pTCHAR->m_pGuild)
		return EC_NOERROR;

	if(pTCHAR->m_pGuild->UpdateArticle(dwID, strTitle, strArticle))
	{
		pServer->SendMW_GUILDARTICLEUPDATE_REQ(dwCharID, dwKEY, GUILD_SUCCESS);
		pServer->SendMW_GUILDARTICLELIST_REQ(dwCharID, dwKEY, pTCHAR->m_pGuild);
		SendDM_GUILDARTICLEUPDATE_REQ(pTCHAR->m_pGuild->m_dwID, dwID, strTitle, strArticle);
	}
	else
		pServer->SendMW_GUILDARTICLEUPDATE_REQ(dwCharID, dwKEY, GUILD_FAIL);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDARTICLEUPDATE_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwID;
	CString strTitle;
	CString strArticle;

	pBUF->m_packet
		>> dwGuildID
		>> dwID
		>> strTitle
		>> strArticle;

	DEFINE_QUERY(&m_db, CSPGuildArticleUpdate)
	query->m_dwGuildID = dwGuildID;
	query->m_dwID = dwID;
	lstrcpy(query->m_szTitle, strTitle);
	lstrcpy(query->m_szArticle, strArticle);
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDFAME_ACK( LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwFame;
	DWORD dwFameColor;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwFame
		>> dwFameColor;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR || !pTCHAR->m_pGuild || pTCHAR->m_pGuild->m_bDisorg)
		return EC_NOERROR;

	DWORD dwUsePoint = 30000;

	if(pTCHAR->m_pGuild->m_dwPvPUseablePoint < dwUsePoint)
	{
		pServer->SendMW_GUILDFAME_REQ(
			dwCharID, 
			dwKEY, 
			GUILD_NOPOINT, 
			dwCharID,
			dwFame, 
			dwFameColor);

		return EC_NOERROR;
	}

	if(pTCHAR->m_pGuild->m_dwFame == dwFame &&
		pTCHAR->m_pGuild->m_dwFameColor == dwFameColor)
		return EC_NOERROR;

	pTCHAR->m_pGuild->UsePvPoint(30000, PVP_USEABLE);
	pTCHAR->m_pGuild->SetFame(dwFame, dwFameColor);

	MAPTGUILDMEMBER::iterator it;
	for(it=pTCHAR->m_pGuild->m_mapTMember.begin(); it!=pTCHAR->m_pGuild->m_mapTMember.end(); it++)
	{
		if((*it).second->m_pChar)
		{
			CTServer *pMap = FindMapSvr((*it).second->m_pChar->m_bMainID);
			if(pMap)
			{
				pMap->SendMW_GUILDFAME_REQ(
					(*it).second->m_pChar->m_dwCharID, 
					(*it).second->m_pChar->m_dwKEY, 
					GUILD_SUCCESS, 
					dwCharID,
					dwFame, 
					dwFameColor);
			}
		}
	}
	SendDM_GUILDFAME_REQ(pTCHAR->m_pGuild->m_dwID, dwFame, dwFameColor);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_GUILDFAME_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwFame;
	DWORD dwFameColor;

	pBUF->m_packet
		>> dwGuildID
		>> dwFame
		>> dwFameColor;

	DEFINE_QUERY(&m_db, CSPGuildFame)
	query->m_dwGuildID = dwGuildID;
	query->m_dwFame = dwFame;
	query->m_dwFameColor = dwFameColor;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDWANTEDADD_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwID;
	CString strTitle;
	CString strText;
	BYTE bMinLevel;
	BYTE bMaxLevel;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwID
		>> strTitle
		>> strText
		>> bMinLevel
		>> bMaxLevel;

	if(strTitle == NAME_NULL)
		return EC_NOERROR;

	if(strTitle.GetLength() > MAX_BOARD_TITLE ||
		strText.GetLength() > MAX_BOARD_TEXT)
		return EC_NOERROR;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKey);

	if(!pTCHAR || !pTCHAR->m_pGuild || pTCHAR->m_pGuild->m_bDisorg)
		return EC_NOERROR;

	BYTE bResult = AddGuildWanted(pTCHAR->m_pGuild, bMinLevel, bMaxLevel, strTitle, strText);

	pServer->SendMW_GUILDWANTEDADD_REQ(
		pTCHAR->m_dwCharID,
		pTCHAR->m_dwKEY,
		bResult);

	if(!bResult)
	{
		NotifyGuildWantedList(dwCharID, dwKey, pServer);
		SendDM_GUILDWANTEDADD_REQ(pTCHAR->m_pGuild->m_dwID, bMinLevel, bMaxLevel, strTitle, strText);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_GUILDWANTEDDEL_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwID;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);

	if(!pTCHAR || !pTCHAR->m_pGuild)
		return EC_NOERROR;

	BYTE bResult = DelGuildWanted(pTCHAR->m_pGuild);
	pServer->SendMW_GUILDWANTEDDEL_REQ(
		pTCHAR->m_dwCharID,
		pTCHAR->m_dwKEY,
		bResult);

	if(!bResult)
	{
		NotifyGuildWantedList(dwCharID, dwKey, pServer);
		SendDM_GUILDWANTEDDEL_REQ(pTCHAR->m_pGuild->m_dwID);
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDWANTEDLIST_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	MAPTGUILDWANTED::iterator it;
	for(it=m_mapTGuildWanted.begin(); it!=m_mapTGuildWanted.end(); it++)
	{
		if((*it).second.m_dlEndTime + DAY_ONE < m_timeCurrent)
		{
			LPPACKETBUF pBUF = new PACKETBUF();
			pBUF->m_packet.SetID(SM_EVENTEXPIRED_ACK)
				<< BYTE(EXPIRED_GMW)
				<< (*it).second.m_dlEndTime
				<< (*it).second.m_dwGuildID
				<< (*it).second.m_dwGuildID;

			SayToBATCH(pBUF);
		}
	}

	NotifyGuildWantedList(dwCharID, dwKey, pServer);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_GUILDVOLUNTEERING_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwID;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);

	if(!pTCHAR || pTCHAR->m_pGuild)
		return EC_NOERROR;

	BYTE bResult = AddGuildWantedApp(
		dwID,
		pTCHAR->m_dwCharID,
		pTCHAR->m_strNAME,
		pTCHAR->m_bLevel,
		pTCHAR->m_bClass,
		pTCHAR);

	pServer->SendMW_GUILDVOLUNTEERING_REQ(
		dwCharID,
		dwKey,
		bResult);

	if(!bResult)
	{
		NotifyGuildWantedList(dwCharID, dwKey, pServer);
		SendDM_GUILDVOLUNTEERING_REQ(GUILDAPP_MEMBER, dwCharID, dwID);
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDVOLUNTEERINGDEL_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);

	if(!pTCHAR)
		return EC_NOERROR;

	BYTE bResult = DelGuildWantedApp(dwCharID);

	pServer->SendMW_GUILDVOLUNTEERINGDEL_REQ(
		dwCharID,
		dwKey,
		bResult);

	if(!bResult)
		NotifyGuildWantedList(dwCharID, dwKey, pServer);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDVOLUNTEERLIST_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	NotifyGuildVolunteerList(dwCharID, dwKey, pServer);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDVOLUNTEERREPLY_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTarget;
	BYTE bReply;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTarget
		>> bReply;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);

	if(!pTCHAR || !pTCHAR->m_pGuild)
		return EC_NOERROR;

	BYTE bResult = GUILD_SUCCESS;

	if(!bReply)
		DelGuildWantedApp(dwTarget);		
	else
	{
		bResult = ApplyGuildApp(dwTarget);

		if(bResult)
			pServer->SendMW_GUILDVOLUNTEERREPLY_REQ(
				dwCharID,
				dwKey,
				bResult);
	}

	NotifyGuildVolunteerList(dwCharID, dwKey, pServer);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDTACTICSWANTEDADD_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwID;
	CString strTitle;
	CString strText;
	BYTE bDay;
	BYTE bMinLevel;
	BYTE bMaxLevel;
	DWORD dwPoint;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwID
		>> strTitle
		>> strText
		>> bDay
		>> bMinLevel
		>> bMaxLevel
		>> dwPoint
		>> dwGold
		>> dwSilver
		>> dwCooper;

	if(strTitle == NAME_NULL)
		return EC_NOERROR;

	if(strTitle.GetLength() > MAX_BOARD_TITLE ||
		strText.GetLength() > MAX_BOARD_TEXT)
		return EC_NOERROR;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKey);

	if(!pTCHAR || !pTCHAR->m_pGuild || pTCHAR->m_pGuild->m_bDisorg)
		return EC_NOERROR;

	dwID = (dwID > 0) ? dwID : ++m_dwTacticsIndex;

	BYTE bResult = AddGuildTacticsWanted(
		pTCHAR->m_pGuild,
		dwID,
		dwPoint,
		dwGold,
		dwSilver,
		dwCooper,
		bDay,
		bMinLevel,
		bMaxLevel,
		strTitle,
		strText);

	pServer->SendMW_GUILDTACTICSWANTEDADD_REQ(
		pTCHAR->m_dwCharID,
		pTCHAR->m_dwKEY,
		bResult);

	if(!bResult)
	{
		NotifyGuildTacticsWantedList(dwCharID, dwKey, pServer);
		SendDM_GUILDTACTICSWANTEDADD_REQ(
			dwID,
			pTCHAR->m_pGuild->m_dwID,
			dwPoint, dwGold, dwSilver, dwCooper,
			bDay, bMinLevel, bMaxLevel,
			strTitle, strText);
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDTACTICSWANTEDDEL_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwID;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);

	if(!pTCHAR || !pTCHAR->m_pGuild)
		return EC_NOERROR;

	BYTE bResult = DelGuildTacticsWanted(pTCHAR->m_pGuild, dwID);
	pServer->SendMW_GUILDTACTICSWANTEDDEL_REQ(
		pTCHAR->m_dwCharID,
		pTCHAR->m_dwKEY,
		bResult);

	if(!bResult)
	{
		NotifyGuildTacticsWantedList(dwCharID, dwKey, pServer);
		SendDM_GUILDTACTICSWANTEDDEL_REQ(dwID);
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDTACTICSWANTEDLIST_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	MAPVTGUILDTACTICSWANTED::iterator it;
	for(it=m_mapTGuildTacticsWanted.begin(); it!=m_mapTGuildTacticsWanted.end(); it++)
	{
		for(DWORD i=0; i<(*it).second.size(); i++)
		{
			if(((*it).second)[i].m_dlEndTime + DAY_ONE < m_timeCurrent)
			{
				LPPACKETBUF pBUF = new PACKETBUF();
				pBUF->m_packet.SetID(SM_EVENTEXPIRED_ACK)
					<< BYTE(EXPIRED_GTW)
					<< ((*it).second)[i].m_dlEndTime
					<< ((*it).second)[i].m_dwGuildID
					<< ((*it).second)[i].m_dwID;

				SayToBATCH(pBUF);
			}
		}
	}

	NotifyGuildTacticsWantedList(dwCharID, dwKey, pServer);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDTACTICSVOLUNTEERING_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwGuildID;
	DWORD dwID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwGuildID
		>> dwID;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);

	if(!pTCHAR)
		return EC_NOERROR;

	BYTE bResult = AddGuildTacticsWantedApp(
		dwID,
		pTCHAR->m_dwCharID,
		pTCHAR->m_strNAME,
		pTCHAR->m_bLevel,
		pTCHAR->m_bClass,
		dwGuildID,
		pTCHAR);

	pServer->SendMW_GUILDTACTICSVOLUNTEERING_REQ(
		dwCharID,
		dwKey,
		bResult);

	if(!bResult)
	{
		NotifyGuildTacticsWantedList(dwCharID, dwKey, pServer);
		SendDM_GUILDVOLUNTEERING_REQ(GUILDAPP_TACTICS, dwCharID, dwID);
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDTACTICSVOLUNTEERINGDEL_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);

	if(!pTCHAR)
		return EC_NOERROR;

	BYTE bResult = DelGuildTacticsWantedApp(dwCharID);

	pServer->SendMW_GUILDTACTICSVOLUNTEERINGDEL_REQ(
		dwCharID,
		dwKey,
		bResult);

	if(!bResult)
		NotifyGuildTacticsWantedList(dwCharID, dwKey, pServer);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDTACTICSVOLUNTEERLIST_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	NotifyGuildTacticsVolunteerList(dwCharID, dwKey, pServer);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDTACTICSREPLY_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTarget;
	BYTE bReply;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTarget
		>> bReply;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);
	if(!pTCHAR || !pTCHAR->m_pGuild)
		return EC_NOERROR;

	BYTE bResult = GUILD_SUCCESS;

	if(!bReply)
		DelGuildTacticsWantedApp(dwTarget);		
	else
	{
		bResult = ApplyGuildTacticsApp(dwTarget);
		if(bResult)
			pServer->SendMW_GUILDTACTICSREPLY_REQ(
				pTCHAR->m_dwCharID,
				pTCHAR->m_dwKEY,
				bResult,
				dwTarget);		
	}

	NotifyGuildTacticsVolunteerList(dwCharID, dwKey, pServer);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDTACTICSKICKOUT_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTarget;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTarget;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);

	if(!pTCHAR)
		return EC_NOERROR;

	CTGuild * pGuild = NULL;
	LPTTACTICSMEMBER pMember = NULL;

	if(dwCharID != dwTarget)
	{
		if(!pTCHAR->m_pGuild)
			return EC_NOERROR;

		pMember = pTCHAR->m_pGuild->FindTactics(dwTarget);
		if(!pMember)
			return EC_NOERROR;

		pGuild = pTCHAR->m_pGuild;
	}
	else
	{
		MAPDWORD::iterator itTc = m_mapCharTactics.find(dwTarget);
		if(itTc == m_mapCharTactics.end())
			return EC_NOERROR;

		CTGuild * pTG = FindTGuild(itTc->second);
		if(!pTG)
			return EC_NOERROR;

		pMember = pTG->FindTactics(dwTarget);
		if(!pMember)
			return EC_NOERROR;

		pGuild = pTG;
	}

	if(!pGuild || !pMember)
		return EC_NOERROR;

	BYTE bResult = GuildTacticsDel(pGuild, pMember, (dwCharID==dwTarget) ? 0 : 1);

	if(dwCharID != dwTarget)
	{
		pServer->SendMW_GUILDTACTICSKICKOUT_REQ(
			dwCharID,
			dwKey,
			bResult,
			dwTarget,
			(dwCharID==dwTarget) ? 0 : 1);

		pServer->SendMW_GUILDTACTICSLIST_REQ(
			dwCharID, 
			dwKey, 
			pGuild);
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDTACTICSINVITE_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	CString strName;
	BYTE bDay;
	DWORD dwPoint;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strName
		>> bDay
		>> dwPoint
		>> dwGold
		>> dwSilver
		>> dwCooper;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);

	if(!pTCHAR || !pTCHAR->m_pGuild)
		return EC_NOERROR;

	BYTE bResult = GUILD_SUCCESS;

	LPTCHARACTER pTarget = FindTChar(strName);
	if(!pTarget)
		bResult = GUILD_NOTFOUND;
	else if(GetWarCountry(pTarget) != GetWarCountry(pTCHAR))
		bResult = GUILD_FAIL;
	else if(pTCHAR->m_pGuild->m_dwPvPUseablePoint < dwPoint)
		bResult = GUILD_NOPOINT;
	else if(!pTCHAR->m_pGuild->UseMoney(dwGold, dwSilver, dwCooper, FALSE))
		bResult = GUILD_NOMONEY;
	else if(pTarget->m_pTactics && pTarget->m_pTactics->m_dwID != pTCHAR->m_pGuild->m_dwID)
		bResult = GUILD_HAVEGUILD;
	else if(!pTarget->m_pTactics && !pTCHAR->m_pGuild->CanAddTactics())
		bResult = GUILD_MEMBER_FULL;
	else if(pTarget->m_pGuild)
	{
		if(pTarget->m_pGuild->m_dwID == pTCHAR->m_pGuild->m_dwID)
			bResult = GUILD_SAMEGUILDTACTICS;
/*		else
		{
			LPTGUILDMEMBER pMember = pTarget->m_pGuild->FindMember(pTarget->m_dwCharID);
			if(pMember && pMember->m_bDuty >= GUILD_DUTY_VICECHIEF)
				bResult = GUILD_NODUTY;
		}
*/
	}

	if(bResult == GUILD_SUCCESS)
	{
		CTServer * pMain = FindMapSvr(pTarget->m_bMainID);
		if(pMain)
			pMain->SendMW_GUILDTACTICSINVITE_REQ(
				pTarget->m_dwCharID,
				pTarget->m_dwKEY,
				pTCHAR->m_pGuild->m_strName,
				pTCHAR->m_strNAME,
				bDay,
				dwPoint,
				dwGold,
				dwSilver,
				dwCooper);
	}
	else
	{
		pServer->SendMW_GUILDTACTICSANSWER_REQ(
			dwCharID,
			dwKey,
			bResult,
			0,
			NAME_NULL,
			0,
			strName,
			dwGold,
			dwSilver,
			dwCooper);
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDTACTICSANSWER_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	BYTE bAnswer;
	CString strName;
	BYTE bDay;
	DWORD dwPoint;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bAnswer
		>> strName
		>> bDay
		>> dwPoint
		>> dwGold
		>> dwSilver
		>> dwCooper;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);

	if(!pTCHAR)
		return EC_NOERROR;

	LPTCHARACTER pOrigin = FindTChar(strName);
	if(!pOrigin || !pOrigin->m_pGuild)
		return EC_NOERROR;

	CTGuild * pGuild = pOrigin->m_pGuild;
	if(pTCHAR->m_pTactics && pTCHAR->m_pTactics->m_dwID != pGuild->m_dwID)
		return EC_NOERROR;
/*
	if(pTCHAR->m_pGuild)
	{
		LPTGUILDMEMBER pGM = pTCHAR->m_pGuild->FindMember(dwCharID);
		if(pGM && pGM->m_bDuty >= GUILD_DUTY_VICECHIEF)
			return EC_NOERROR;
	}
*/
	INT64 timeCur = m_timeCurrent;
	LPTTACTICSMEMBER pMember = pGuild->FindTactics(pTCHAR->m_dwCharID);

	BYTE bResult = GUILD_SUCCESS;

	if(bAnswer == ASK_YES)
	{
		if(pGuild->m_dwPvPUseablePoint < dwPoint)
			bResult = GUILD_NOPOINT;
		else if(GetWarCountry(pOrigin) != GetWarCountry(pTCHAR))
			bResult = GUILD_FAIL;
		else if(!pGuild->UseMoney(dwGold, dwSilver, dwCooper, FALSE))
			bResult = GUILD_NOMONEY;
		else if(pTCHAR->m_pTactics && pTCHAR->m_pTactics->m_dwID != pGuild->m_dwID)
			bResult = GUILD_HAVEGUILD;
		else if(!pTCHAR->m_pTactics && !pGuild->CanAddTactics())
			bResult = GUILD_MEMBER_FULL;
		else if(pTCHAR->m_pGuild && pTCHAR->m_pGuild->m_dwID == pGuild->m_dwID)
			bResult = GUILD_SAMEGUILDTACTICS;

		if(bResult == GUILD_SUCCESS)
		{
			if(pMember)
			{
				timeCur = pMember->m_dlEndTime;
				GuildTacticsDel(pGuild, pMember, 2);
			}

			pGuild->UseMoney(dwGold, dwSilver, dwCooper, TRUE);
			pGuild->UsePvPoint(dwPoint, PVP_USEABLE);

			LPTTACTICSMEMBER pNew = new TTACTICSMEMBER();
			pNew->m_dwID = pTCHAR->m_dwCharID;
			pNew->m_strName = pTCHAR->m_strNAME;
			pNew->m_bLevel = pTCHAR->m_bLevel;
			pNew->m_bClass = pTCHAR->m_bClass;
			pNew->m_dwRewardPoint = dwPoint;
			pNew->m_dlRewardMoney = CalcMoney(dwGold, dwSilver, dwCooper);
			pNew->m_dwGainPoint = 0;
			pNew->m_bDay = bDay;
			pNew->m_dlEndTime = timeCur + bDay * DAY_ONE;
			pNew->m_pChar = pTCHAR;
			pTCHAR->m_pTactics = pGuild;

			GuildTacticsAdd(pGuild, pNew);
		}
	}
	else
		bResult = GUILD_JOIN_DENY;

	pServer->SendMW_GUILDTACTICSANSWER_REQ(
		dwCharID,
		dwKey,
		bResult,
		pGuild->m_dwID,
		pGuild->m_strName,
		pTCHAR->m_dwCharID,
		pTCHAR->m_strNAME,
		dwGold,
		dwSilver,
		dwCooper);

	CTServer * pChiefMap = FindMapSvr(pOrigin->m_bMainID);
	if(pChiefMap)
	{
		pChiefMap->SendMW_GUILDTACTICSANSWER_REQ(
			pOrigin->m_dwCharID,
			pOrigin->m_dwKEY,
			bResult,
			pGuild->m_dwID,
			pGuild->m_strName,
			pTCHAR->m_dwCharID,
			pTCHAR->m_strNAME,
			dwGold,
			dwSilver,
			dwCooper);
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GUILDTACTICSLIST_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	CTGuild * pCurGuild = GetCurGuild(pTCHAR);

	if(!pTCHAR || !pCurGuild)
		return EC_NOERROR;

	pServer->SendMW_GUILDTACTICSLIST_REQ(
		dwCharID, 
		dwKEY, 
		pCurGuild);

	return EC_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CTWorldSvrModule::OnMW_CHAT_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	BYTE bChannel;
	DWORD dwSender;
	DWORD dwSenderKEY;

	CString strSenderName;
	BYTE bType;
	BYTE bGroup;
	DWORD dwTarget;
	CString strName;
	CString strTalk;

	pBUF->m_packet
		>> bChannel
		>> dwSender
		>> dwSenderKEY
		>> strSenderName
		>> bType
		>> bGroup
		>> dwTarget
		>> strName
		>> strTalk;

	MAPTCHARACTER::iterator itSd = m_mapTCHAR.find(dwSender);
	if(itSd == m_mapTCHAR.end())
		return EC_NOERROR;
/*
	else if(m_pRelay)
	{
		LPTCHARACTER pChar = (*itSd).second;
		if(m_timeCurrent - pChar->m_timeRelay > 30)
		{
			pChar->m_timeRelay = m_timeCurrent;
			pServer->SendMW_RELAYCONNECT_REQ(dwSender);
		}
	}
*/
	switch(bGroup)
	{
	case CHAT_TACTICS:
		{
			CTGuild * pGuild = FindTGuild(dwTarget);
			if(!pGuild)
				return EC_NOERROR;

			MAPTGUILDMEMBER::iterator it;
			for(it=pGuild->m_mapTMember.begin(); it!=pGuild->m_mapTMember.end(); it++)
			{
				if((*it).second->m_pChar)
				{
					CTServer * pCon = FindMapSvr((*it).second->m_pChar->m_bMainID);
					if(pCon)
						pCon->SendMW_CHAT_REQ(
							(*it).second->m_pChar->m_dwCharID,
							(*it).second->m_pChar->m_dwKEY,
							bChannel,
							dwSender,
							strSenderName,
							itSd->second->m_bCountry,
							itSd->second->m_bAidCountry,
							bType,
							bGroup,
							dwTarget,
							strTalk);
				}
			}

			MAPTTACTICSMEMBER::iterator itTs;
			for(itTs=pGuild->m_mapTTactics.begin(); itTs!=pGuild->m_mapTTactics.end(); itTs++)
			{
				if((*itTs).second->m_pChar)
				{
					CTServer * pCon = FindMapSvr((*itTs).second->m_pChar->m_bMainID);
					if(pCon)
						pCon->SendMW_CHAT_REQ(
							(*itTs).second->m_pChar->m_dwCharID,
							(*itTs).second->m_pChar->m_dwKEY,
							bChannel,
							dwSender,
							strSenderName,
							itSd->second->m_bCountry,
							itSd->second->m_bAidCountry,
							bType,
							bGroup,
							dwTarget,
							strTalk);
				}
			}
		}
		break;
	case CHAT_GUILD:
		{
			CTGuild * pGuild = FindTGuild(dwTarget);
			if(!pGuild)
				return EC_NOERROR;

			MAPTGUILDMEMBER::iterator it;
			for(it=pGuild->m_mapTMember.begin(); it!=pGuild->m_mapTMember.end(); it++)
			{
				if((*it).second->m_pChar)
				{
					CTServer * pCon = FindMapSvr((*it).second->m_pChar->m_bMainID);
					if(pCon)
						pCon->SendMW_CHAT_REQ(
							(*it).second->m_pChar->m_dwCharID,
							(*it).second->m_pChar->m_dwKEY,
							bChannel,
							dwSender,
							strSenderName,
							itSd->second->m_bCountry,
							itSd->second->m_bAidCountry,
							bType,
							bGroup,
							dwTarget,
							strTalk);
				}
			}
		}
		break;
	case CHAT_SHOW:
	case CHAT_MAP:
	case CHAT_WORLD:
		{
			MAPTSERVER::iterator it;
			for(it=m_mapSERVER.begin(); it != m_mapSERVER.end(); it++)
			{
				(*it).second->SendMW_CHAT_REQ(
					0,
					0,
					bChannel,
					dwSender,
					strSenderName,
					itSd->second->m_bCountry,
					itSd->second->m_bAidCountry,
					bType,
					bGroup,
					dwTarget,
					strTalk);
			}
		}
		break;
	case CHAT_FORCE:
		{
			CTCorps * pCorps = NULL;
			LPTCHARACTER pTCHAR = FindTChar(dwSender, dwSenderKEY);
			if(pTCHAR && pTCHAR->m_pParty)
				pCorps = FindCorps(pTCHAR->m_pParty->m_wCorpsID);

			if(pCorps)
			{
				MAPTPARTY::iterator itParty;
				for(itParty=pCorps->m_mapParty.begin(); itParty!=pCorps->m_mapParty.end(); itParty++)
				{
					CTParty * pParty = (*itParty).second;
					for(int i=0; i<pParty->GetSize(); i++)
					{
						LPTCHARACTER pChar = pParty->GetMember(i);
						if(pChar)
						{
							CTServer * pCon = FindMapSvr(pChar->m_bMainID);
							if(pCon)
								pCon->SendMW_CHAT_REQ(
									pChar->m_dwCharID,
									pChar->m_dwKEY,
									bChannel,
									dwSender,
									strSenderName,
									itSd->second->m_bCountry,
									itSd->second->m_bAidCountry,
									bType,
									bGroup,
									dwTarget,
									strTalk);
						}
					}
				}
			}
		}
		break;
	case CHAT_PARTY:
		{
			CTParty * pParty = FindParty((WORD)dwTarget);
			if(pParty)
			{
				for(int i=0; i<pParty->GetSize(); i++)
				{
					LPTCHARACTER pChar = pParty->GetMember(i);
					if(pChar)
					{
						CTServer * pCon = FindMapSvr(pChar->m_bMainID);
						if(pCon)
							pCon->SendMW_CHAT_REQ(
								pChar->m_dwCharID,
								pChar->m_dwKEY,
								bChannel,
								dwSender,
								strSenderName,
								itSd->second->m_bCountry,
								itSd->second->m_bAidCountry,
								bType,
								bGroup,
								dwTarget,
								strTalk);
					}
				}
			}
		}
		break;
	case CHAT_WHISPER:
		{
			MAPTCHARACTER::iterator it;
			if(strName == GetSvrMsg(NAME_OPERATOR))
			{
				BYTE bCount = 0;				
				for(DWORD i=0; i<m_vTOPERATOR.size(); i++)
				{
					it = m_mapTCHAR.find(m_vTOPERATOR[i]);
					if(it!=m_mapTCHAR.end() &&
						(*it).second->m_bCountry == itSd->second->m_bCountry) 
					{
						CTServer * pCon = FindMapSvr((*it).second->m_bMainID);
						if(pCon)
						{
							pCon->SendMW_CHAT_REQ(
								(*it).second->m_dwCharID,
								(*it).second->m_dwKEY,
								bChannel,
								dwSender,
								strSenderName,
								itSd->second->m_bCountry,
								itSd->second->m_bAidCountry,
								bType,
								bGroup,
								dwTarget,
								strTalk);
							bCount++;
						}
					}
				}

				DWORD dwSenderID = dwSender;
				if(!bCount)
				{
					strTalk = BuildNetString(NAME_NULL, GetSvrMsg(MSG_CHAR_LOGOUT));
					bType = CHAT_OPERATOR;
					dwSenderID = 0;
				}

				pServer->SendMW_CHAT_REQ(
					dwSender,
					dwSenderKEY,
					bChannel,
					dwSenderID,
					strName,
					itSd->second->m_bCountry,
					itSd->second->m_bAidCountry,
					bType,
					bGroup,
					dwTarget,
					strTalk);

				break;
			}

			it = m_mapTCHAR.find(dwTarget);
			if(it!=m_mapTCHAR.end())
			{
				if(GetWarCountry(itSd->second) != GetWarCountry(it->second) &&
					itSd->second->m_bCountry != TCONTRY_PEACE &&
					it->second->m_bCountry != TCONTRY_PEACE)
					break;

				CTServer * pCon = FindMapSvr((*it).second->m_bMainID);
				if(pCon)
				{
					pCon->SendMW_CHAT_REQ(
						(*it).second->m_dwCharID,
						(*it).second->m_dwKEY,
						bChannel,
						dwSender,
						strSenderName,
						itSd->second->m_bCountry,
						itSd->second->m_bAidCountry,
						bType,
						bGroup,
						dwTarget,
						strTalk);
					pServer->SendMW_CHAT_REQ(
						dwSender,
						dwSenderKEY,
						bChannel,
						dwSender,
						(*it).second->m_strNAME,
						itSd->second->m_bCountry,
						itSd->second->m_bAidCountry,
						bType,
						bGroup,
						dwTarget,
						strTalk);
					break;
				}
			}

			LPTCHARACTER pTarget = FindTChar(strName);
			if(pTarget)
			{
				if(GetWarCountry(itSd->second) != GetWarCountry(pTarget) &&
					itSd->second->m_bCountry != TCONTRY_PEACE &&
					pTarget->m_bCountry != TCONTRY_PEACE)
					break;

				CTServer * pCon = FindMapSvr(pTarget->m_bMainID);
				if(pCon)
				{
					pCon->SendMW_CHAT_REQ(
						pTarget->m_dwCharID,
						pTarget->m_dwKEY,
						bChannel,
						dwSender,
						strSenderName,
						itSd->second->m_bCountry,
						itSd->second->m_bAidCountry,
						bType,
						bGroup,
						dwTarget,
						strTalk);
					pServer->SendMW_CHAT_REQ(
						dwSender,
						dwSenderKEY,
						bChannel,
						dwSender,
						strName,
						itSd->second->m_bCountry,
						itSd->second->m_bAidCountry,
						bType,
						bGroup,
						dwTarget,
						strTalk);
					break;
				}
			}
		}
		break;
	}
	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_TAKEMONMONEY_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	CTServer *pServer = FindMapSvr(pTCHAR->m_bMainID);
	if(pServer)
		pServer->SendMW_TAKEMONMONEY_REQ(pBUF);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_MONSTERDIE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	CTServer * pServer = FindMapSvr(pTCHAR->m_bMainID);
	if(pServer)
		pServer->SendMW_MONSTERDIE_REQ(pBUF);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_ADDITEM_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bServerID;
	BYTE bChannel;
	WORD wMapID;
	DWORD dwMonID;
	BYTE bInven;
	BYTE bSlot;
	BYTE bItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bServerID
		>> bChannel
		>> wMapID
		>> dwMonID
		>> bInven
		>> bSlot
		>> bItemID;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
	{
		pServer->SendMW_ADDITEMRESULT_REQ(
			dwCharID,
			dwKEY,
			bChannel,
			wMapID,
			dwMonID,
			bItemID,
			MIT_NOTFOUND);

		return EC_NOERROR;
	}

	CTServer * pMain = FindMapSvr(pTCHAR->m_bMainID);
	if(pMain)
		pMain->SendMW_ADDITEM_REQ(&(pBUF->m_packet));

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_PARTYORDERTAKEITEM_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	WORD wPartyID;
	BYTE bServerID;
	BYTE bChannel;
	WORD wMapID;
	DWORD dwMonID;
	WORD wTempMonID;  
	BYTE bCnt;

	VECTORDWORD vMember;
	vMember.clear();

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> wPartyID
		>> bServerID
		>> bChannel
		>> wMapID
		>> dwMonID
		>> wTempMonID  
		>> bCnt;

	DWORD dwID;

    for(BYTE i=0; i<bCnt; i++)
	{
		pBUF->m_packet
			>> dwID;
		vMember.push_back(dwID);
	}

	LPTITEM pITEM = CreateItem(&(pBUF->m_packet));

	CTParty * pParty = FindParty(wPartyID);
	if(!pParty)
	{
		pServer->SendMW_ADDITEMRESULT_REQ(
			dwCharID,
			dwKEY,
			bChannel,
			wMapID,
			dwMonID,
			pITEM->m_bItemID,
			MIT_NOTFOUND);

		delete pITEM;
		return EC_NOERROR;
	}

	LPTCHARACTER pNext = pParty->GetNextOrder(vMember);
	if(pNext)
	{
		CTServer * pMain = FindMapSvr(pNext->m_bMainID);
		if(pMain)
			pMain->SendMW_PARTYORDERTAKEITEM_REQ(
				pNext->m_dwCharID,
				pNext->m_dwKEY,
				bServerID,
				bChannel,
				wMapID,
				dwMonID,
				wTempMonID,
				pITEM);
	}

	vMember.clear();
	delete pITEM;

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_PROTECTEDCHECK_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	BYTE bConnect;
	CString strProtected;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bConnect
		>> strProtected;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar)
		return EC_NOERROR;

	LPTFRIEND pFr = NULL;
	MAPTFRIEND::iterator itFr;
	for(itFr=pChar->m_mapTFRIEND.begin(); itFr!=pChar->m_mapTFRIEND.end(); itFr++)
	{
		if((*itFr).second->m_strName == strProtected)
		{
			pFr = (*itFr).second;
			break;
		}
	}

	if(!pFr)
		return EC_NOERROR;

	if(bConnect==FRIEND_DISCONNECTION )
		SendDM_FRIENDERASE_REQ(pChar->m_dwCharID, pFr->m_dwID);

	LPTCHARACTER pTarget = FindTChar(strProtected);
	if(!pTarget)
		return EC_NOERROR;

	MAPTFRIEND::iterator it = pTarget->m_mapTFRIEND.find(dwCharID);
	if(it==pTarget->m_mapTFRIEND.end())
		return EC_NOERROR;

	MAPTFRIEND::iterator itC = pChar->m_mapTFRIEND.find(pTarget->m_dwCharID);
	if(itC==pChar->m_mapTFRIEND.end())
		return EC_NOERROR;

	switch(bConnect)
	{
	case FRIEND_CONNECTION:
		(*it).second->m_bConnected = TRUE;
		(*it).second->m_dwRegion = pChar->m_dwRegion;
		(*itC).second->m_bConnected = TRUE;
		(*itC).second->m_dwRegion = pTarget->m_dwRegion;
		break;
	case FRIEND_DISCONNECTION:
		(*it).second->m_bConnected = FALSE;
		(*itC).second->m_bConnected = FALSE;
		break;
	}

	if((*it).second->m_bType != FT_TARGET)
	{
		CTServer * pMAP = FindMapSvr(pTarget->m_bMainID);
		if(pMAP)
		{
			pMAP->SendMW_FRIENDCONNECTION_REQ(
				pTarget->m_dwCharID,
				pTarget->m_dwKEY,
				bConnect,
				pChar->m_strNAME,
				DWORD(!bConnect ? pChar->m_dwRegion : 0));
		}
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_FRIENDPROTECTEDASK_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	CString strTarget;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strTarget;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar)
		return EC_NOERROR;

	LPTCHARACTER pTarget = FindTChar(strTarget);
	if(!pTarget)
		return EC_NOERROR;

	CTServer * pCon = FindMapSvr(pTarget->m_bMainID);
	if(pCon)
	{
		pCon->SendMW_FRIENDADD_REQ(
			pTarget->m_dwCharID,
			pTarget->m_dwKEY,
			FRIEND_REFUSE,
			0,
			pChar->m_strNAME);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_FRIENDASK_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	CString strTarget;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strTarget;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar)
		return EC_NOERROR;

	LPTCHARACTER pTarget = FindTChar(strTarget);
	if(!pTarget)
	{
		pServer->SendMW_FRIENDADD_REQ(
			pChar->m_dwCharID,
			pChar->m_dwKEY,
			FRIEND_NOTFOUND,
			0,
			strTarget);
		return EC_NOERROR;
	}

	if(pChar->m_bCountry != pTarget->m_bCountry)
	{
		pServer->SendMW_FRIENDADD_REQ(
			pChar->m_dwCharID,
			pChar->m_dwKEY,
			FRIEND_NOTFOUND,
			0,
			strTarget);

		return EC_NOERROR;
	}

	LPTFRIEND pTF = NULL;
	LPTFRIEND pTT = NULL;

	MAPTFRIEND::iterator it = pChar->m_mapTFRIEND.find(pTarget->m_dwCharID);
	if(it!=pChar->m_mapTFRIEND.end())
		pTF = (*it).second;

	if(pTF && (pTF->m_bType!=FT_TARGET))
	{
		pServer->SendMW_FRIENDADD_REQ(
			pChar->m_dwCharID,
			pChar->m_dwKEY,
			FRIEND_ALREADY,
			0,
			strTarget);
		return EC_NOERROR;
	}

	it = pTarget->m_mapTFRIEND.find(pChar->m_dwCharID);
	if(it!=pTarget->m_mapTFRIEND.end())
		pTT = (*it).second;
	
	BYTE bCount = BYTE(pChar->m_mapTFRIEND.size());
	MAPTFRIEND::iterator itMax;
	for(itMax=pChar->m_mapTFRIEND.begin(); itMax!=pChar->m_mapTFRIEND.end(); itMax++)
	{
		if((*itMax).second->m_bType == FT_TARGET )
			bCount--;
	}
	if(bCount == MAX_FRIEND)
	{
		pServer->SendMW_FRIENDADD_REQ(
			pChar->m_dwCharID,
			pChar->m_dwKEY,
			FRIEND_MAX,
			0,
			strTarget);
		return EC_NOERROR;
	}

	if(pTF && pTT)
	{
		pTF->m_bType = FT_FRIENDFRIEND;
		pTT->m_bType = FT_FRIENDFRIEND;

		pTF->m_bConnected = TRUE;
		pTF->m_dwRegion = pTarget->m_dwRegion;
		pTF->m_bGroup = 0;
		pTT->m_bConnected = TRUE;
		pTT->m_dwRegion = pChar->m_dwRegion;
		
		LPPACKETBUF pMSG = new PACKETBUF();
		pMSG->m_packet.SetID(DM_FRIENDINSERT_REQ)
			<< pChar->m_dwCharID
			<< pTarget->m_dwCharID;
		SayToDB(pMSG);

		pServer->SendMW_FRIENDADD_REQ(
			pChar->m_dwCharID,
			pChar->m_dwKEY,
			FRIEND_SUCCESS,
			pTarget->m_dwCharID,
			pTarget->m_strNAME,
			pTarget->m_bLevel,
			0,
			pTarget->m_bClass,
			pTarget->m_dwRegion);
		return EC_NOERROR;
	}

	bCount = BYTE(pTarget->m_mapTFRIEND.size());
	for(itMax=pTarget->m_mapTFRIEND.begin(); itMax!=pTarget->m_mapTFRIEND.end(); itMax++)
	{
		if((*itMax).second->m_bType == FT_TARGET ||
			(*itMax).second->m_dwID == pChar->m_dwCharID)
			bCount--;
	}
	if(bCount == MAX_FRIEND)
	{
		pServer->SendMW_FRIENDADD_REQ(
			pChar->m_dwCharID,
			pChar->m_dwKEY,
			FRIEND_REFUSE,
			0,
			strTarget);
		return EC_NOERROR;
	}

	CTServer * pSvr = FindMapSvr(pTarget->m_bMainID);
	if(pSvr)
		pSvr->SendMW_FRIENDASK_REQ(pTarget->m_dwCharID, pTarget->m_dwKEY, pChar->m_strNAME, pChar->m_dwCharID);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_FRIENDREPLY_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	CString strInviter;
	BYTE bReply;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strInviter
		>> bReply;

	LPTCHARACTER pPlayer = FindTChar(strInviter);
	LPTCHARACTER pFriend = FindTChar(dwCharID, dwKey);

	if(!pPlayer && !pFriend)
		return EC_NOERROR;

	if(!pPlayer && pFriend)
	{
		CTServer * pCon = FindMapSvr(pFriend->m_bMainID);
		if(pCon)
			pCon->SendMW_FRIENDADD_REQ(
				pFriend->m_dwCharID,
				pFriend->m_dwKEY,
				FRIEND_NOTFOUND,
				0,
				strInviter);

		return EC_NOERROR;
	}
	else if(pPlayer && !pFriend)
	{
		CTServer * pCon = FindMapSvr(pPlayer->m_bMainID);
		if(pCon)
			pCon->SendMW_FRIENDADD_REQ(
				pPlayer->m_dwCharID,
				pPlayer->m_dwKEY,
				FRIEND_NOTFOUND);

		return EC_NOERROR;
	}
	
	if(bReply == ASK_YES)
	{
		MAPTFRIEND::iterator find = pPlayer->m_mapTFRIEND.find(pFriend->m_dwCharID);
		if(find==pPlayer->m_mapTFRIEND.end())
		{
			LPTFRIEND pNew = new TFRIEND();
			pNew->m_dwID = pFriend->m_dwCharID;
			pNew->m_strName = pFriend->m_strNAME;
			pNew->m_bLevel = pFriend->m_bLevel;
			pNew->m_bGroup = 0;
			pNew->m_bClass = pFriend->m_bClass;
			pNew->m_bType = FT_FRIENDFRIEND;
			pNew->m_bConnected = TRUE;
			pNew->m_dwRegion = pFriend->m_dwRegion;
            pPlayer->m_mapTFRIEND.insert(MAPTFRIEND::value_type(pNew->m_dwID, pNew));
		}
		else
		{
			(*find).second->m_bType = FT_FRIENDFRIEND;
			(*find).second->m_bConnected = TRUE;
			(*find).second->m_dwRegion = pFriend->m_dwRegion;
		}

		find = pFriend->m_mapTFRIEND.find(pPlayer->m_dwCharID);
		if(find==pFriend->m_mapTFRIEND.end())
		{
			LPTFRIEND pNewPl = new TFRIEND();
			pNewPl->m_dwID = pPlayer->m_dwCharID;
			pNewPl->m_strName = pPlayer->m_strNAME;
			pNewPl->m_bLevel = pPlayer->m_bLevel;
			pNewPl->m_bGroup = 0;
			pNewPl->m_bClass = pFriend->m_bClass;
			pNewPl->m_bType = FT_FRIENDFRIEND;
			pNewPl->m_bConnected = TRUE;
			pNewPl->m_dwRegion = pPlayer->m_dwRegion;
            pFriend->m_mapTFRIEND.insert(MAPTFRIEND::value_type(pNewPl->m_dwID, pNewPl));
		}
		else
		{
			(*find).second->m_bType = FT_FRIENDFRIEND;
			(*find).second->m_bConnected = TRUE;
			(*find).second->m_dwRegion = pPlayer->m_dwRegion;
		}

		LPPACKETBUF pMSG = new PACKETBUF();
		pMSG->m_packet.SetID(DM_FRIENDINSERT_REQ)
			<< pFriend->m_dwCharID
			<< pPlayer->m_dwCharID;
		SayToDB(pMSG);

		LPPACKETBUF pMSGF = new PACKETBUF();
		pMSGF->m_packet.SetID(DM_FRIENDINSERT_REQ)
			<< pPlayer->m_dwCharID
			<< pFriend->m_dwCharID;
		SayToDB(pMSGF);

		CTServer * pCon = FindMapSvr(pPlayer->m_bMainID);
		if(pCon)
			pCon->SendMW_FRIENDADD_REQ(
				pPlayer->m_dwCharID,
				pPlayer->m_dwKEY,
				FRIEND_SUCCESS,
				pFriend->m_dwCharID,
				pFriend->m_strNAME,
				pFriend->m_bLevel,
				0,
				pFriend->m_bClass,
				pFriend->m_dwRegion);

		pCon = FindMapSvr(pFriend->m_bMainID);
		if(pCon)
			pCon->SendMW_FRIENDADD_REQ(
				pFriend->m_dwCharID,
				pFriend->m_dwKEY,
				FRIEND_SUCCESS,
				pPlayer->m_dwCharID,
				pPlayer->m_strNAME,
				pPlayer->m_bLevel,
				0,
				pPlayer->m_bClass,
				pPlayer->m_dwRegion);
	}
	else
	{
		CTServer * pCon = FindMapSvr(pPlayer->m_bMainID);
		if(pCon)
			pCon->SendMW_FRIENDADD_REQ(
				pPlayer->m_dwCharID,
				pPlayer->m_dwKEY,
				bReply,
				pFriend->m_dwCharID,
				pFriend->m_strNAME);
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_FRIENDERASE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTarget;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTarget;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);
	if(!pTCHAR)
		return EC_NOERROR;

	SendDM_FRIENDERASE_REQ(pTCHAR->m_dwCharID, dwTarget);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_FRIENDINSERT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwFriendID;

	pBUF->m_packet
		>> dwCharID
		>> dwFriendID;

	DEFINE_QUERY(&m_db, CSPFriendInsert)
	query->m_dwCharID = dwCharID;
	query->m_dwFriendID = dwFriendID;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_FRIENDERASE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwFriendID;

	pBUF->m_packet
		>> dwCharID
		>> dwFriendID;

	DEFINE_QUERY(&m_db, CSPFriendErase)
	query->m_dwCharID = dwCharID;
	query->m_dwFriendID = dwFriendID;
	query->Call();
	UNDEFINE_QUERY();

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_FRIENDERASE_ACK)
		<< dwCharID
		<< dwFriendID;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_FRIENDERASE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwFriendID;

	pBUF->m_packet
		>> dwCharID
		>> dwFriendID;

	MAPTCHARACTER::iterator it = m_mapTCHAR.find(dwCharID);
	if(it==m_mapTCHAR.end())
		return EC_NOERROR;

	LPTCHARACTER pTCHAR = (*it).second;

	EraseFriend(pTCHAR, dwFriendID);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_FRIENDGROUPMAKE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bGroup;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bGroup
		>> strName;

	LPTCHARACTER pPlayer = FindTChar(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	CTServer * pCon = FindMapSvr(pPlayer->m_bMainID);
	if(!pCon)
		return EC_NOERROR;

	if( !bGroup || pPlayer->m_mapFRIENDGROUP.size() == MAX_FRIENDGROUP )
	{
		pCon->SendMW_FRIENDGROUPMAKE_REQ(
			dwCharID,
			dwKey,
			FRIEND_MAX,
			0,
			strName);
		return EC_NOERROR;
	}

	if(strName.GetLength() > MAX_GROUPNAME)
		return EC_NOERROR;

	MAPBSTRING::iterator itGROUP = pPlayer->m_mapFRIENDGROUP.find(bGroup);
	for(itGROUP=pPlayer->m_mapFRIENDGROUP.begin(); itGROUP!=pPlayer->m_mapFRIENDGROUP.end(); itGROUP++)
	{
		if( (*itGROUP).first == bGroup ||
			(*itGROUP).second == strName )
		{
			pCon->SendMW_FRIENDGROUPMAKE_REQ(
				dwCharID,
				dwKey,
				FRIEND_ALREADY,
				0,
				strName);
			return EC_NOERROR;
		}
	}

	pPlayer->m_mapFRIENDGROUP.insert(MAPBSTRING::value_type(bGroup, strName));

	pCon->SendMW_FRIENDGROUPMAKE_REQ(
		dwCharID,
		dwKey,
		FRIEND_SUCCESS,
		bGroup,
		strName);

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_FRIENDGROUPMAKE_REQ)
		<< dwCharID
		<< bGroup
		<< strName;
	SayToDB(pMSG);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_FRIENDGROUPDELETE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bGroup;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bGroup;

	LPTCHARACTER pPlayer = FindTChar(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	CTServer * pCon = FindMapSvr(pPlayer->m_bMainID);
	if(!pCon)
		return EC_NOERROR;

	MAPBSTRING::iterator itGROUP = pPlayer->m_mapFRIENDGROUP.find(bGroup);
	if(itGROUP == pPlayer->m_mapFRIENDGROUP.end())
		return EC_NOERROR;

	MAPTFRIEND::iterator itFRIEND;
	for(itFRIEND=pPlayer->m_mapTFRIEND.begin(); itFRIEND!=pPlayer->m_mapTFRIEND.end(); itFRIEND++)
	{
		if((*itFRIEND).second->m_bType != FT_TARGET && (*itFRIEND).second->m_bGroup == bGroup)
		{
			pCon->SendMW_FRIENDGROUPDELETE_REQ(
				dwCharID, 
				dwKey, 
				FRIEND_REFUSE, 
				bGroup);
			return EC_NOERROR;
		}
	}

	pPlayer->m_mapFRIENDGROUP.erase(bGroup);

	pCon->SendMW_FRIENDGROUPDELETE_REQ(
		dwCharID, 
		dwKey, 
		FRIEND_SUCCESS, 
		bGroup);

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_FRIENDGROUPDELETE_REQ)
		<< dwCharID
		<< bGroup;
	SayToDB(pMSG);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_FRIENDGROUPCHANGE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwFriend;
	BYTE bGroup;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwFriend
		>> bGroup;

	LPTCHARACTER pPlayer = FindTChar(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	if(bGroup)
	{
		MAPBSTRING::iterator itGROUP = pPlayer->m_mapFRIENDGROUP.find(bGroup);
		if(itGROUP == pPlayer->m_mapFRIENDGROUP.end())
			return EC_NOERROR;
	}

	MAPTFRIEND::iterator itFRIEND = pPlayer->m_mapTFRIEND.find(dwFriend);
	if(itFRIEND == pPlayer->m_mapTFRIEND.end())
		return EC_NOERROR;

	(*itFRIEND).second->m_bGroup = bGroup;

	CTServer * pCon = FindMapSvr(pPlayer->m_bMainID);
	if(pCon)
	{
		pCon->SendMW_FRIENDGROUPCHANGE_REQ(
			dwCharID, 
			dwKey, 
			FRIEND_SUCCESS, 
			bGroup,
			dwFriend);
	}

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_FRIENDGROUPCHANGE_REQ)
		<< dwCharID
		<< dwFriend
		<< bGroup;
	SayToDB(pMSG);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_FRIENDGROUPNAME_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bGroup;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bGroup
		>> strName;

	LPTCHARACTER pPlayer = FindTChar(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	CTServer * pCon = FindMapSvr(pPlayer->m_bMainID);
	if(!pCon)
		return EC_NOERROR;

	if(strName.GetLength() > MAX_GROUPNAME)
		return EC_NOERROR;

	MAPBSTRING::iterator itG;	
	for(itG=pPlayer->m_mapFRIENDGROUP.begin(); itG!=pPlayer->m_mapFRIENDGROUP.end(); itG++)
	{
		if((*itG).second == strName)
		{
			pCon->SendMW_FRIENDGROUPNAME_REQ(
				dwCharID, 
				dwKey, 
				FRIEND_REFUSE, 
				bGroup,
				strName);
			return EC_NOERROR;
		}
	}

	itG = pPlayer->m_mapFRIENDGROUP.find(bGroup);
	if(itG == pPlayer->m_mapFRIENDGROUP.end())
	{
		pCon->SendMW_FRIENDGROUPNAME_REQ(
			dwCharID, 
			dwKey, 
			FRIEND_NOTFOUND, 
			bGroup,
			strName);
		return EC_NOERROR;
	}

	(*itG).second = strName;

	pCon->SendMW_FRIENDGROUPNAME_REQ(
		dwCharID, 
		dwKey, 
		0, 
		bGroup,
		strName);

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_FRIENDGROUPNAME_REQ)
		<< dwCharID
		<< bGroup
		<< strName;
	SayToDB(pMSG);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_FRIENDGROUPMAKE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	BYTE bGroup;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> bGroup
		>> strName;

	DEFINE_QUERY(&m_db, CSPFriendGroupMake)
	query->m_dwCharID = dwCharID;
	query->m_bGroup = bGroup;
	lstrcpy(query->m_szName, strName);
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_FRIENDGROUPDELETE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	BYTE bGroup;

	pBUF->m_packet
		>> dwCharID
		>> bGroup;

	DEFINE_QUERY(&m_db, CSPFriendGroupDelete)
	query->m_dwCharID = dwCharID;
	query->m_bGroup = bGroup;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_FRIENDGROUPCHANGE_REQ(LPPACKETBUF pBUF)
{
    DWORD dwCharID;
	DWORD dwFriend;
	BYTE bGroup;

	pBUF->m_packet
		>> dwCharID
		>> dwFriend
		>> bGroup;

	DEFINE_QUERY(&m_db, CSPFriendGroupChange)
	query->m_dwCharID = dwCharID;
	query->m_dwFriendID = dwFriend;
	query->m_bGroup = bGroup;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_FRIENDGROUPNAME_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	BYTE bGroup;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> bGroup
		>> strName;

	DEFINE_QUERY(&m_db, CSPFriendGroupName)
	query->m_dwCharID = dwCharID;
	query->m_bGroup = bGroup;
	lstrcpy(query->m_szName, strName);
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnDM_RESERVEDPOSTSEND_REQ(LPPACKETBUF pBUF)  
{
	CString strSender;
	CString strRecver;
	CString strTitle;
	CString strMessage;	

	DEFINE_QUERY(&m_db, CSPGetReservedPost)

	for(BYTE i=0; i<30; i++)
	{
		int nRet = 1;

		if(query->Call() )
			nRet = query->m_nRET;

		if(!nRet)
		{
			// Recver 가 있으면 Notify
			strSender = query->m_szSender;
			strRecver = query->m_szRecver;
			strTitle  = query->m_szTitle;
			BYTE bType = 1;

			LPPACKETBUF pMSG = new PACKETBUF();
			pMSG->m_packet.SetID(DM_RESERVEDPOSTRECV_ACK)
				<< query->m_dwPostID
				<< strSender
				<< strRecver
				<< strTitle
				<< bType;

			SayToBATCH(pMSG);
		}
		else
			break;
	}

	UNDEFINE_QUERY()

	return EC_NOERROR;
}


DWORD CTWorldSvrModule::OnDM_RESERVEDPOSTRECV_ACK(LPPACKETBUF pBUF)  
{
	DWORD dwPostID;
	CString strSender;
	CString strTarget;
	CString strTitle;
	BYTE bType;

	pBUF->m_packet
		>> dwPostID
		>> strSender
		>> strTarget
		>> strTitle
		>> bType;

	LPTCHARACTER pChar = FindTChar(strTarget);
	if(!pChar)
		return EC_NOERROR;
	else
	{
		CTServer * pServer = FindMapSvr(pChar->m_bMainID);
		if(pServer)
			pServer->SendMW_POSTRECV_REQ(&pBUF->m_packet);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_ADDITEMRESULT_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bMapSvrID;
	BYTE bChannel;
	WORD wMapID;
	DWORD dwMonID;
	BYTE bItemID;
	BYTE bRet;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bMapSvrID
		>> bChannel
		>> wMapID
		>> dwMonID
		>> bItemID
		>> bRet;

	CTServer *pServer = FindMapSvr(bMapSvrID);
	if(pServer)
		pServer->SendMW_ADDITEMRESULT_REQ(
			dwCharID,
			dwKEY,
			bChannel,
			wMapID,
			dwMonID,
			bItemID,
			bRet);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_LEAVESOLOMAP_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	if( pTCHAR->m_pParty && pTCHAR->m_pParty->m_bObtainType == PT_SOLO )
		DeleteParty(pTCHAR->m_pParty->GetID());

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_ENTERSOLOMAP_ACK( LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar(
		dwCharID,
		dwKEY);

	if(!pTCHAR)
		return EC_NOERROR;

	CTServer *pMAIN = FindMapSvr(pTCHAR->m_bMainID);
	if(!pMAIN)
		return EC_NOERROR;

	if(!pTCHAR->m_pParty)
	{
		CTParty *pParty = new CTParty();

		pParty->m_bObtainType = PT_SOLO;
		pParty->SetID(GenPartyID());
		pParty->AddMember(pTCHAR);

		m_mapTParty.insert( MAPTPARTY::value_type( pParty->GetID(), pParty));
	}

	MAPTCHARCON::iterator itTCON;
	for( itTCON = pTCHAR->m_mapTCHARCON.begin(); itTCON != pTCHAR->m_mapTCHARCON.end(); itTCON++)
	{
		CTServer *pTMAP = FindMapSvr((*itTCON).first);

		if(pTMAP && pTMAP->m_bValid)
		{
			pTMAP->SendMW_ENTERSOLOMAP_REQ(
				dwCharID,
				dwKEY,
				pTCHAR->m_pParty->GetID(),
				pTCHAR->m_pParty->m_bObtainType,
				pTCHAR->m_pParty->GetChiefID());
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CHARSTATINFO_ACK(LPPACKETBUF pBUF)
{
	DWORD dwReqCharID;
	DWORD dwCharID;

	pBUF->m_packet
		>> dwReqCharID
		>> dwCharID;

	MAPTCHARACTER::iterator itCHAR = m_mapTCHAR.find(dwCharID);
	if(itCHAR!=m_mapTCHAR.end())
	{
		LPTCHARACTER pPlayer = (*itCHAR).second;
		CTServer * pMain = FindMapSvr(pPlayer->m_bMainID);
		if(pMain)
			pMain->SendMW_CHARSTATINFOANS_REQ(dwReqCharID, dwCharID);
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_CHARSTATINFOANS_ACK(LPPACKETBUF pBUF)
{
	DWORD dwReqCharID;

	pBUF->m_packet
		>> dwReqCharID;

	MAPTCHARACTER::iterator itCHAR = m_mapTCHAR.find(dwReqCharID);
	if(itCHAR!=m_mapTCHAR.end())
	{
		LPTCHARACTER pPlayer = (*itCHAR).second;
		CTServer * pMain = FindMapSvr(pPlayer->m_bMainID);
		if(pMain)
			pMain->SendMW_CHARSTATINFO_REQ(&(pBUF->m_packet));
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CHGCORPSCOMMANDER_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	WORD wPartyID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wPartyID;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar)
		return EC_NOERROR;

	if(!pChar->m_pParty)
	{
		CTServer * pCon = FindMapSvr(pChar->m_bMainID);
		if(pCon)
			pCon->SendMW_CHGCORPSCOMMANDER_REQ(
				pChar->m_dwCharID,
				pChar->m_dwKEY,
				CORPS_NO_PARTY);
		
		return EC_NOERROR;
	}

	CTCorps * pCorps = FindCorps(pChar->m_pParty->m_wCorpsID);
	if(!pCorps)
	{
		CTServer * pCon = FindMapSvr(pChar->m_bMainID);
		if(pCon)
			pCon->SendMW_CHGCORPSCOMMANDER_REQ(
				pChar->m_dwCharID,
				pChar->m_dwKEY,
				CORPS_NO_PARTY);
		
		return EC_NOERROR;
	}

	if( (pChar->m_pParty->GetID() != pCorps->m_wCommander) ||
		(pChar->m_dwCharID != pCorps->m_dwGeneralID))
	{
		CTServer * pCon = FindMapSvr(pChar->m_bMainID);
		if(pCon)
			pCon->SendMW_CHGCORPSCOMMANDER_REQ(
				pChar->m_dwCharID,
				pChar->m_dwKEY,
				CORPS_NOT_COMMANDER);
		
		return EC_NOERROR;
	}

	if(wPartyID == pCorps->m_wCommander)
	{
		CTServer * pCon = FindMapSvr(pChar->m_bMainID);
		if(pCon)
			pCon->SendMW_CHGCORPSCOMMANDER_REQ(
				pChar->m_dwCharID,
				pChar->m_dwKEY,
				CORPS_WRONG_TARGET);
		
		return EC_NOERROR;
	}

	CTParty * pParty = pCorps->FindParty(wPartyID);
	if(!pParty)
	{
		CTServer * pCon = FindMapSvr(pChar->m_bMainID);
		if(pCon)
			pCon->SendMW_CHGCORPSCOMMANDER_REQ(
				pChar->m_dwCharID,
				pChar->m_dwKEY,
				CORPS_TARGET_NO_PARTY);
		
		return EC_NOERROR;
	}

	pCorps->m_wCommander = pParty->GetID();
	pCorps->m_dwGeneralID = pParty->GetChiefID();

	CTServer * pCon = FindMapSvr(pChar->m_bMainID);
	if(pCon)
		pCon->SendMW_CHGCORPSCOMMANDER_REQ(
			pChar->m_dwCharID,
			pChar->m_dwKEY,
			CORPS_CHGCOMMANDER);

	MAPTPARTY::iterator itParty;
	for(itParty=pCorps->m_mapParty.begin(); itParty!=pCorps->m_mapParty.end(); itParty++)
		CorpsJoin((*itParty).second, pCorps->m_wCommander);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CORPSASK_ACK(LPPACKETBUF pBUF)
{
	CTServer * pCon = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	CString strTargetName;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strTargetName;

	LPTCHARACTER pPlayer = FindTChar(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	LPTCHARACTER pTarget = FindTChar(strTargetName);
	if(!pTarget)
	{
		pCon->SendMW_CORPSREPLY_REQ(
			dwCharID,
			pPlayer->m_dwKEY,
			CORPS_WRONG_TARGET,
			strTargetName);

		return EC_NOERROR;
	}

	if( !pPlayer->m_pParty ||
		!pPlayer->m_pParty->IsChief(dwCharID) ||
		!pTarget->m_pParty ||
		!pTarget->m_pParty->IsChief(pTarget->m_dwCharID) ||
		GetWarCountry(pPlayer) != GetWarCountry(pTarget))
	{
		pCon->SendMW_CORPSREPLY_REQ(
			dwCharID,
			pPlayer->m_dwKEY,
			CORPS_NO_PARTY,
			strTargetName);

		return EC_NOERROR;
	}

	if(pPlayer->m_pParty->m_bArena || pTarget->m_pParty->m_bArena)
	{
		pCon->SendMW_CORPSREPLY_REQ(
			dwCharID,
			pPlayer->m_dwKEY,
			CORPS_BUSY,
			strTargetName);

		return EC_NOERROR;
	}

	BYTE bResult = CheckCorpsJoin(pPlayer->m_pParty, pTarget->m_pParty);

	if(bResult)
	{
		pCon->SendMW_CORPSREPLY_REQ(
			dwCharID,
			pPlayer->m_dwKEY,
			bResult,
			strTargetName);

		return EC_NOERROR;
	}

	CTServer * pMain = FindMapSvr(pTarget->m_bMainID);
	if(!pMain)
		return EC_NOERROR;

	pMain->SendMW_CORPSASK_REQ(
		pTarget->m_dwCharID,
		pTarget->m_dwKEY,
		pPlayer->m_dwCharID,
		pPlayer->m_strNAME);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_CORPSREPLY_ACK(LPPACKETBUF pBUF)
{
	CTServer * pCon = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	BYTE bReply;
	CString strReqName;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bReply
		>> strReqName;

	LPTCHARACTER pPlayer = FindTChar(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	LPTCHARACTER pRequest = FindTChar(strReqName);
	if(!pRequest)
	{
		pCon->SendMW_CORPSREPLY_REQ(
			dwCharID,
			pPlayer->m_dwKEY,
			CORPS_WRONG_TARGET,
			strReqName);

		return EC_NOERROR;
	}

	CTServer * pConRequest = FindMapSvr(pRequest->m_bMainID);

	if(!pConRequest)
	{
		pCon->SendMW_CORPSREPLY_REQ(
			dwCharID,
			pPlayer->m_dwKEY,
			CORPS_WRONG_TARGET,
			strReqName);

		return EC_NOERROR;
	}

	if(bReply != ASK_YES)
	{
		pConRequest->SendMW_CORPSREPLY_REQ(
			pRequest->m_dwCharID,
			pRequest->m_dwKEY,
			bReply,
			pPlayer->m_strNAME);

		return EC_NOERROR;
	}

	if( !pPlayer->m_pParty ||
        !pPlayer->m_pParty->IsChief(dwCharID) ||
		!pRequest->m_pParty ||
		!pRequest->m_pParty->IsChief(pRequest->m_dwCharID) ||
		GetWarCountry(pPlayer) != GetWarCountry(pRequest))
	{
		pCon->SendMW_CORPSREPLY_REQ(
			pPlayer->m_dwCharID,
			pPlayer->m_dwKEY,
			CORPS_WRONG_TARGET,
			strReqName);

		pConRequest->SendMW_CORPSREPLY_REQ(
			pRequest->m_dwCharID,
			pRequest->m_dwKEY,
			CORPS_WRONG_TARGET,
			pPlayer->m_strNAME);

		return EC_NOERROR;
	}

	if(pPlayer->m_pParty->m_bArena || pRequest->m_pParty->m_bArena)
	{
		pCon->SendMW_CORPSREPLY_REQ(
			pPlayer->m_dwCharID,
			pPlayer->m_dwKEY,
			CORPS_BUSY,
			strReqName);

		pConRequest->SendMW_CORPSREPLY_REQ(
			pRequest->m_dwCharID,
			pRequest->m_dwKEY,
			CORPS_BUSY,
			pPlayer->m_strNAME);

		return EC_NOERROR;
	}

	CTCorps * pCorps = NULL;
	BYTE bResult = CheckCorpsJoin(pPlayer->m_pParty, pRequest->m_pParty);
	if(bResult)
	{
		pCon->SendMW_CORPSREPLY_REQ(
			pPlayer->m_dwCharID,
			pPlayer->m_dwKEY,
			bResult,
			strReqName);

		pConRequest->SendMW_CORPSREPLY_REQ(
			pRequest->m_dwCharID,
			pRequest->m_dwKEY,
			bResult,
			pPlayer->m_strNAME);

		return EC_NOERROR;
	}
	else if(!pPlayer->m_pParty->m_wCorpsID &&
		!pRequest->m_pParty->m_wCorpsID)
	{
		pCorps = new CTCorps();
		pCorps->m_wID = GenPartyID();
		pCorps->m_dwGeneralID = pRequest->m_pParty->GetChiefID();
		pCorps->m_wCommander = pRequest->m_pParty->GetID();
		m_mapTCorps.insert(MAPTCORPS::value_type(pCorps->m_wID, pCorps));
		NotifyCorpsJoin(pCorps, pPlayer->m_pParty);
		NotifyCorpsJoin(pCorps, pRequest->m_pParty);
	}
	else if(pPlayer->m_pParty->m_wCorpsID)
	{
		pCorps = FindCorps(pPlayer->m_pParty->m_wCorpsID);
		if(pCorps)
			NotifyCorpsJoin(pCorps, pRequest->m_pParty);
	}
	else
	{
		pCorps = FindCorps(pRequest->m_pParty->m_wCorpsID);
		if(pCorps)
			NotifyCorpsJoin(pCorps, pPlayer->m_pParty);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CORPSLEAVE_ACK(LPPACKETBUF pBUF)
{
	CTServer * pMain = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	WORD wSquadID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wSquadID;

	MAPTPARTY::iterator itPt = m_mapTParty.find(wSquadID);
	if(itPt==m_mapTParty.end())
		return EC_NOERROR;

	LPTCHARACTER pChar = FindTChar(dwCharID,dwKey);
	if(!pChar || !pChar->m_pParty || !pChar->m_pParty->IsChief(dwCharID))
		return EC_NOERROR;

	MAPTCORPS::iterator it = m_mapTCorps.find(pChar->m_pParty->m_wCorpsID);
	if(it==m_mapTCorps.end())
		return EC_NOERROR;

	if(wSquadID != pChar->m_pParty->GetID() &&
		(*it).second->m_dwGeneralID != dwCharID)
		return EC_NOERROR;

	NotifyCorpsLeave((*it).second, (*itPt).second);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CORPSCMD_ACK(LPPACKETBUF pBUF)
{
	DWORD dwGeneral;
	DWORD dwKey;
	WORD wMapID;
	WORD wSquadID;
	DWORD dwCharID;
	BYTE bCMD;
	DWORD dwTargetID;
	BYTE bTargetType;
	WORD wPosX;
	WORD wPosZ;

	pBUF->m_packet
		>> dwGeneral
		>> dwKey
		>> wMapID
		>> wSquadID
		>> dwCharID
		>> bCMD
		>> dwTargetID
		>> bTargetType
		>> wPosX
		>> wPosZ;

	LPTCHARACTER pPlayer = FindTChar(dwGeneral, dwKey);
	if(!pPlayer || !pPlayer->m_pParty)
		return EC_NOERROR;

	LPTCHARACTER pChar = NULL;
	MAPTCHARACTER::iterator itChar = m_mapTCHAR.find(dwCharID);
	if(itChar != m_mapTCHAR.end())
		pChar = (*itChar).second;

	if(pChar &&
		pChar->m_pParty &&
		pChar->m_pParty->IsChief(dwCharID))
	{
		pChar->m_pParty->m_command.m_bCommand = bCMD;
		pChar->m_pParty->m_command.m_bTgType = bTargetType;
		pChar->m_pParty->m_command.m_dwTgObjID = dwTargetID;
		pChar->m_pParty->m_command.m_wTgPosX = wPosX;
		pChar->m_pParty->m_command.m_wTgPosZ = wPosZ;
	}

	CTCorps * pCorps = FindCorps(pPlayer->m_pParty->m_wCorpsID);
	if(pCorps)
	{
		MAPTPARTY::iterator itPt;
		for(itPt=pCorps->m_mapParty.begin(); itPt!=pCorps->m_mapParty.end(); itPt++)
			for(int i=0; i<(*itPt).second->GetSize(); i++)
				TransferCorpsCommand(
					(*itPt).second->GetMember(i),
					bCMD,
					bTargetType,
					dwTargetID,
					wPosX,
					wPosZ,
					wSquadID,
					dwCharID,
					wMapID);
	}
	else
	{
		for(int i=0; i<pPlayer->m_pParty->GetSize(); i++)
		{
			TransferCorpsCommand(
                pPlayer->m_pParty->GetMember(i),
				bCMD,
				bTargetType,
                dwTargetID,
                wPosX,
                wPosZ,
				wSquadID,
				dwCharID,
				wMapID);
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CORPSENEMYLIST_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(pChar && pChar->m_pParty)
		BroadcastCorps(pChar, &pBUF->m_packet, MW_CORPSENEMYLIST_REQ);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_MOVECORPSENEMY_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(pChar && pChar->m_pParty)
		BroadcastCorps(pChar, &pBUF->m_packet, MW_MOVECORPSENEMY_REQ);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_MOVECORPSUNIT_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(pChar && pChar->m_pParty)
		BroadcastCorps(pChar, &pBUF->m_packet, MW_MOVECORPSUNIT_REQ);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_ADDCORPSENEMY_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(pChar && pChar->m_pParty)
		BroadcastCorps(pChar, &pBUF->m_packet, MW_ADDCORPSENEMY_REQ);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_DELCORPSENEMY_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(pChar && pChar->m_pParty)
		BroadcastCorps(pChar, &pBUF->m_packet, MW_DELCORPSENEMY_REQ);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CORPSHP_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(pChar && pChar->m_pParty)
		BroadcastCorps(pChar, &pBUF->m_packet, MW_CORPSHP_REQ);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_PARTYMOVE_ACK(LPPACKETBUF pBUF)
{
	CTServer * pMain = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	CString strTarget;
	CString strDestName;
	WORD wTargetParty;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strTarget
		>> strDestName
		>> wTargetParty;

	LPTCHARACTER pGeneral = FindTChar(dwCharID, dwKey);
	if(!pGeneral)
	{
		pMain->SendMW_PARTYMOVE_REQ(
			dwCharID,
			dwKey,
			CORPS_NOT_COMMANDER);

		return EC_NOERROR;
	}

	LPTCHARACTER pTarget = FindTChar(strTarget);
	if(!pTarget ||
		!pTarget->m_pParty)
	{
		pMain->SendMW_PARTYMOVE_REQ(
			dwCharID,
			dwKey,
			CORPS_NOT_COMMANDER);

		return EC_NOERROR;
	}

	CTParty * pTgParty = pTarget->m_pParty;
	CTParty * pDestParty = NULL;
	LPTCHARACTER pDestChar = NULL;

	if(!strDestName.IsEmpty())
	{
		pDestChar = FindTChar(strDestName);
		if(!pDestChar ||
			!pDestChar->m_pParty ||
			pDestChar->m_pParty->GetSize() <= 1 ||
			pTarget->m_pParty->GetSize() <= 1)
		{
			pMain->SendMW_PARTYMOVE_REQ(
				dwCharID,
				dwKey,
				CORPS_WRONG_TARGET);

			return EC_NOERROR;
		}

		pDestParty = pDestChar->m_pParty;
		LeaveParty(pDestChar, TRUE, FALSE);
		LeaveParty(pTarget, TRUE, FALSE);

		LPTCHARACTER pDestChief = pDestParty->GetMember(0);
		if(!pDestChief)
		{
			pMain->SendMW_PARTYMOVE_REQ(
				dwCharID,
				dwKey,
				CORPS_WRONG_TARGET);

			return EC_NOERROR;
		}

		LPTCHARACTER pTgChief = pTgParty->GetMember(0);
		if(!pTgChief)
		{
			pMain->SendMW_PARTYMOVE_REQ(
				dwCharID,
				dwKey,
				CORPS_WRONG_TARGET);

			return EC_NOERROR;
		}

		JoinParty(pDestParty, pDestChief->m_dwCharID, pTarget);
		JoinParty(pTgParty, pTgChief->m_dwCharID, pDestChar);
	}
	else
	{
		pDestParty = FindParty(wTargetParty);
		if(!pDestParty ||
			pDestParty->GetSize() == 0 ||
			pTarget->m_pParty->GetID() == wTargetParty)
		{
			pMain->SendMW_PARTYMOVE_REQ(
				dwCharID,
				dwKey,
				CORPS_NOT_COMMANDER);

			return EC_NOERROR;
		}

		LeaveParty(pTarget, TRUE, TRUE);

		LPTCHARACTER pTgChief = pDestParty->GetMember(0);
		if(!pTgChief)
		{
			pMain->SendMW_PARTYMOVE_REQ(
				dwCharID,
				dwKey,
				CORPS_WRONG_TARGET);

			return EC_NOERROR;
		}

		JoinParty(pDestParty, pTgChief->m_dwCharID, pTarget);
	}

	pMain->SendMW_PARTYMOVE_REQ(
		dwCharID,
		dwKey,
		CORPS_SUCCESS);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_TMSSEND_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTMS;
	CString strMessage;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTMS
		>> strMessage;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);
	if(!pTCHAR)
		return EC_NOERROR;

	MAPDWORD::iterator itT = pTCHAR->m_mapTMS.find(dwTMS);
	if(itT == pTCHAR->m_mapTMS.end())
		return EC_NOERROR;

	MAPTMS::iterator itTMS = m_mapTMS.find(dwTMS);
	if(itTMS == m_mapTMS.end())
		return EC_NOERROR;

	LPTMS pTMS = (*itTMS).second;

	// 마지막 나간 사람에게 메시지
	if(pTMS->m_mapMember.size() == 1)
	{
		LPTCHARACTER pTarget = FindTChar(pTMS->m_strLastMember);
		if(!pTarget)
			strMessage = BuildNetString(NAME_NULL, GetSvrMsg(TMS_NORECEIVER));
		else
		{
			// 타겟의 차단목록을 검사한다
			CTServer * pCon = FindMapSvr(pTarget->m_bMainID);
			if(pCon)
			{
				pCon->SendMW_TMSINVITEASK_REQ(
					pTarget->m_dwCharID,
					pTarget->m_dwKEY,
					pTCHAR->m_dwCharID,
					pTCHAR->m_dwKEY,
					dwTMS,
					strMessage);
			}

			return EC_NOERROR;
		}
	}

	MAPTCHARACTER::iterator it;
	for(it=pTMS->m_mapMember.begin(); it!=pTMS->m_mapMember.end(); it++)
	{
		CTServer * pCon = FindMapSvr((*it).second->m_bMainID);
		if(pCon)
		{
			pCon->SendMW_TMSRECV_REQ(
				(*it).second->m_dwCharID,
				(*it).second->m_dwKEY,
				dwTMS,
				pTCHAR->m_strNAME,
				strMessage);
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_TMSINVITEASK_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwTargetID;
	DWORD dwTargetKEY;
	BYTE bResult;
	DWORD dwTMS;
	CString strMessage;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwTargetID
		>> dwTargetKEY
		>> bResult
		>> dwTMS
		>> strMessage;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKEY);
	if(!pTCHAR)
		return EC_NOERROR;

	MAPDWORD::iterator itT = pTCHAR->m_mapTMS.find(dwTMS);
	if(itT == pTCHAR->m_mapTMS.end())
		return EC_NOERROR;

	MAPTMS::iterator itTMS = m_mapTMS.find(dwTMS);
	if(itTMS == m_mapTMS.end())
		return EC_NOERROR;

	LPTMS pTMS = (*itTMS).second;

	LPTCHARACTER pTarget = FindTChar(dwTargetID, dwTargetKEY);
	if(pTarget && bResult)
	{
		// 차단되어 있지 않다면 INVITE
		pTMS->m_mapMember.insert(MAPTCHARACTER::value_type(pTarget->m_dwCharID, pTarget));
		pTarget->m_mapTMS.insert(MAPDWORD::value_type(pTMS->m_dwID, pTMS->m_dwID));

		MAPTCHARACTER::iterator it;
		for(it=pTMS->m_mapMember.begin(); it!=pTMS->m_mapMember.end(); it++)
		{
			CTServer * pCon = FindMapSvr((*it).second->m_bMainID);
			if(pCon)
			{
				pCon->SendMW_TMSINVITE_REQ(
					pTarget->m_dwCharID,
					(*it).second->m_dwKEY,
					(*it).second->m_dwCharID,
					pTMS);
			}
		}
	}
	else
		strMessage = BuildNetString(NAME_NULL, GetSvrMsg(TMS_NORECEIVER));

	MAPTCHARACTER::iterator it;
	for(it=pTMS->m_mapMember.begin(); it!=pTMS->m_mapMember.end(); it++)
	{
		CTServer * pCon = FindMapSvr((*it).second->m_bMainID);
		if(pCon)
		{
			pCon->SendMW_TMSRECV_REQ(
				(*it).second->m_dwCharID,
				(*it).second->m_dwKEY,
				dwTMS,
				pTCHAR->m_strNAME,
				strMessage);
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_TMSINVITE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTMS;
	BYTE bCount;
	DWORD dwTarget;

	LPTMS pTMS = NULL;
	MAPTCHARACTER mapMEMBER;
	mapMEMBER.clear();

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTMS
		>> bCount;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);
	if(!pTCHAR || !bCount)
		return EC_NOERROR;
	
	if(dwTMS)
	{
		MAPDWORD::iterator itT = pTCHAR->m_mapTMS.find(dwTMS);
		if(itT==pTCHAR->m_mapTMS.end())
			return EC_NOERROR;

		MAPTMS::iterator itTMS = m_mapTMS.find(dwTMS);
		if(itTMS==m_mapTMS.end())
			return EC_NOERROR;

		pTMS = (*itTMS).second;
	}

	for(BYTE i=0; i<bCount; i++)
	{
		pBUF->m_packet
			>> dwTarget;

		MAPTCHARACTER::iterator it = m_mapTCHAR.find(dwTarget);
		if(it==m_mapTCHAR.end())
			continue;

		if(GetWarCountry(pTCHAR) != GetWarCountry(it->second))
			continue;

		mapMEMBER.insert(MAPTCHARACTER::value_type(dwTarget, (*it).second));
	}

	if(!mapMEMBER.size())
		return EC_NOERROR;

	// 1:1 대화일경우
	if(!pTMS && mapMEMBER.size() == 1)
	{
		LPTCHARACTER pTarget = (*mapMEMBER.begin()).second;

		MAPDWORD::iterator it;
		for(it=pTarget->m_mapTMS.begin(); it!=pTarget->m_mapTMS.end(); it++)
		{
			MAPTMS::iterator itTMS = m_mapTMS.find((*it).second);
			if(itTMS==m_mapTMS.end())
				continue;

			// 마지막 나간 사람이 나일 경우
			if( (*itTMS).second->m_mapMember.size() == 1 && 
				(*itTMS).second->m_strLastMember == pTCHAR->m_strNAME)
			{
				(*itTMS).second->m_mapMember.insert(MAPTCHARACTER::value_type(dwCharID, pTCHAR));
				pTCHAR->m_mapTMS.insert(MAPDWORD::value_type((*itTMS).second->m_dwID, (*itTMS).second->m_dwID));

				CTServer * pCon = FindMapSvr(pTCHAR->m_bMainID);
				if(pCon)
				{
					pCon->SendMW_TMSINVITE_REQ(
						pTCHAR->m_dwCharID,
						pTCHAR->m_dwKEY,
						pTCHAR->m_dwCharID,
						(*itTMS).second);
				}

				mapMEMBER.clear();

				return EC_NOERROR;
			}
		}
	}

	if(!pTMS)
	{
		pTMS = new TMS();
		pTMS->m_dwID = ++m_dwTMSIndex;
		pTMS->m_strLastMember = NAME_NULL;
		pTMS->m_mapMember.insert(MAPTCHARACTER::value_type(dwCharID, pTCHAR));
		pTCHAR->m_mapTMS.insert(MAPDWORD::value_type(pTMS->m_dwID, pTMS->m_dwID));
		m_mapTMS.insert(MAPTMS::value_type(pTMS->m_dwID, pTMS));
	}

	MAPTCHARACTER::iterator itTARGET;
	for(itTARGET=mapMEMBER.begin(); itTARGET!=mapMEMBER.end(); itTARGET++)
	{
		pTMS->m_mapMember.insert(MAPTCHARACTER::value_type((*itTARGET).second->m_dwCharID, (*itTARGET).second));
		(*itTARGET).second->m_mapTMS.insert(MAPDWORD::value_type(pTMS->m_dwID, pTMS->m_dwID));
	}

	MAPTCHARACTER::iterator itChar;
	for(itChar=pTMS->m_mapMember.begin(); itChar!=pTMS->m_mapMember.end(); itChar++)
	{
		CTServer * pCon = FindMapSvr((*itChar).second->m_bMainID);
		if(pCon)
		{
			pCon->SendMW_TMSINVITE_REQ(
				(*itChar).second->m_dwCharID,
				(*itChar).second->m_dwKEY,
				pTCHAR->m_dwCharID,
				pTMS);
		}
	}

	mapMEMBER.clear();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_TMSOUT_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTMS;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTMS;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);
	if(!pTCHAR)
		return EC_NOERROR;

	MAPDWORD::iterator itT = pTCHAR->m_mapTMS.find(dwTMS);
	if(itT == pTCHAR->m_mapTMS.end())
		return EC_NOERROR;

	MAPTMS::iterator itTMS = m_mapTMS.find(dwTMS);
	if(itTMS == m_mapTMS.end())
		return EC_NOERROR;

	LPTMS pTMS = (*itTMS).second;

	MAPTCHARACTER::iterator itChar;
	for(itChar=pTMS->m_mapMember.begin(); itChar!=pTMS->m_mapMember.end(); itChar++)
	{
		CTServer * pCon = FindMapSvr((*itChar).second->m_bMainID);
		if(pCon)
			pCon->SendMW_TMSOUT_REQ(
				(*itChar).second->m_dwCharID,
				(*itChar).second->m_dwKEY,
				dwTMS,
				pTCHAR->m_strNAME);
	}

	pTMS->m_mapMember.erase(dwCharID);
	pTMS->m_strLastMember = pTCHAR->m_strNAME;
	pTCHAR->m_mapTMS.erase(dwTMS);

	if(!pTMS->m_mapMember.size())
	{
		delete pTMS;
		m_mapTMS.erase(dwTMS);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_POSTRECV_ACK(LPPACKETBUF pBUF)
{
	CTServer * pMain = (CTServer *)pBUF->m_pSESSION;

	DWORD dwPostID;
	CString strSender;
	CString strTarget;
	CString strTitle;
	BYTE bType;

	pBUF->m_packet
		>> dwPostID
		>> strSender
		>> strTarget
		>> strTitle
		>> bType;

	LPTCHARACTER pChar = FindTChar(strTarget);
	if(!pChar)
		return EC_NOERROR;
	else
	{
		CTServer * pServer = FindMapSvr(pChar->m_bMainID);
		if(pServer)
			pServer->SendMW_POSTRECV_REQ(&pBUF->m_packet);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CASTLEOCCUPY_ACK(LPPACKETBUF pBUF)
{
	BYTE bType;
	BYTE bCountry;
	WORD wCastleID;
	DWORD dwGuildID;
	DWORD dwLoseGuild;

	pBUF->m_packet
		>> bType
		>> wCastleID
		>> dwGuildID
		>> bCountry
		>> dwLoseGuild;

	CTGuild * pGuild = FindTGuild(dwGuildID);
	if(pGuild)
		ResetCastleApply(pGuild, wCastleID);

	CTGuild * pLose = FindTGuild(dwLoseGuild);
	if(pLose)
		ResetCastleApply(pLose, wCastleID);

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it!= m_mapSERVER.end(); it++)
		(*it).second->SendMW_CASTLEOCCUPY_REQ(
			bType,
			wCastleID,
			dwGuildID,
			bCountry,
			pGuild ? pGuild->m_strName : NAME_NULL);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_LOCALOCCUPY_ACK(LPPACKETBUF pBUF)
{
	BYTE bType;
	WORD wLocalID;
	BYTE bCountry;
	DWORD dwGuildID;
	BYTE bCurCountry;

	pBUF->m_packet
		>> bType
		>> wLocalID
		>> bCountry
		>> dwGuildID
		>> bCurCountry;

	CTGuild * pGuild = FindTGuild(dwGuildID);

	if(pGuild && pGuild->m_bCountry == TCONTRY_B)
	{
		if(bCurCountry != TCONTRY_N)
			bCountry = !bCountry;

		dwGuildID= 0;
	}

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it!= m_mapSERVER.end(); it++)
		(*it).second->SendMW_LOCALOCCUPY_REQ(bType, wLocalID, bCountry, dwGuildID, pGuild ? pGuild->m_strName : NAME_NULL);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_MISSIONOCCUPY_ACK(LPPACKETBUF pBUF)
{
	BYTE bType;
	WORD wLocalID;
	BYTE bCountry;

	pBUF->m_packet
		>> bType
		>> wLocalID
		>> bCountry;

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it!= m_mapSERVER.end(); it++)
		(*it).second->SendMW_MISSIONOCCUPY_REQ(bType, wLocalID, bCountry);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CASTLEAPPLY_ACK(LPPACKETBUF pBUF)
{
	CTServer * pMain = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	WORD wCastle;
	DWORD dwTarget;
	BYTE bCamp;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wCastle
		>> dwTarget
		>> bCamp;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar ||
		!pChar->m_pGuild ||
		pChar->m_pGuild->m_dwChief != dwCharID)
		return EC_NOERROR;

	LPTCHARACTER pCharTarget = NULL;
	WORD wPrevCastle = 0;
	BYTE bPrevCamp = 0;

	LPTGUILDMEMBER pTarget = pChar->m_pGuild->FindMember(dwTarget);
	LPTTACTICSMEMBER pTactic = NULL;
	if(!pTarget)
	{
		pTactic = pChar->m_pGuild->FindTactics(dwTarget);
		if(!pTactic)
			return EC_NOERROR;

		pCharTarget = pTactic->m_pChar;
		wPrevCastle = pTactic->m_wCastle;
		bPrevCamp = pTactic->m_bCamp;
	}
	else
	{
		if(pTarget->m_dwTactics)
			return EC_NOERROR;

		pCharTarget = pTarget->m_pChar;
		wPrevCastle = pTarget->m_wCastle;
		bPrevCamp = pTarget->m_bCamp;
	}

	if(wPrevCastle == wCastle)
	{
		wCastle = 0;
		bCamp = 0;
	}

	if(wCastle && !pChar->m_pGuild->CanApplyWar(wCastle))
	{
		pMain->SendMW_CASTLEAPPLY_REQ(
			dwCharID,
			dwKey,
			CBS_FULL,
			wCastle,
			dwTarget,
			bCamp);

		return EC_NOERROR;
	}

	if(wPrevCastle || wCastle)
	{
		if(pTarget)
		{
			pTarget->m_wCastle = wCastle;
			pTarget->m_bCamp = bCamp;
		}
		else
		{
			pTactic->m_wCastle = wCastle;
			pTactic->m_bCamp = bCamp;
		}

		pMain->SendMW_CASTLEAPPLY_REQ(
			dwCharID,
			dwKey,
			CBS_SUCCESS,
			wCastle,
			dwTarget,
			bCamp);

		if(dwCharID != dwTarget && pCharTarget)
		{
			CTServer * pMap = FindMapSvr(pCharTarget->m_bMainID);
			if(pMap)
				pMap->SendMW_CASTLEAPPLY_REQ(
					dwTarget,
					pCharTarget->m_dwKEY,
					CBS_SUCCESS,
					wCastle,
					dwTarget,
					bCamp);
		}

		SendDM_CASTLEAPPLY_REQ(wCastle, dwTarget, bCamp);

		if(wPrevCastle)
			NotifyCastleApply(wPrevCastle, pChar->m_pGuild);

		if(wCastle)
			NotifyCastleApply(wCastle, pChar->m_pGuild);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_MONTEMPT_ACK(LPPACKETBUF pBUF)
{
	DWORD dwAtkID;
	WORD wMonID;

	pBUF->m_packet
		>> dwAtkID
		>> wMonID;

	MAPTCHARACTER::iterator it = m_mapTCHAR.find(dwAtkID);
	if(it!=m_mapTCHAR.end())
	{
		CTServer * pMain = FindMapSvr((*it).second->m_bMainID);
		if(pMain)
			pMain->SendMW_MONTEMPT_REQ(
				(*it).second->m_dwCharID,
				(*it).second->m_dwKEY,
				wMonID);
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_GETBLOOD_ACK(LPPACKETBUF pBUF)
{
	DWORD dwAtkID;
	BYTE bAtkType;
	DWORD dwHostID;
	BYTE bBloodType;
	DWORD dwBlood;

	pBUF->m_packet
		>> dwAtkID
		>> bAtkType
		>> dwHostID
		>> bBloodType
		>> dwBlood;

	MAPTCHARACTER::iterator it;
	if(bAtkType == OT_PC)
        it = m_mapTCHAR.find(dwAtkID);
	else
		it = m_mapTCHAR.find(dwHostID);

	if(it!=m_mapTCHAR.end())
	{
		CTServer * pMain = FindMapSvr((*it).second->m_bMainID);
		if(pMain)
			pMain->SendMW_GETBLOOD_REQ(
				(*it).second->m_dwCharID,
				(*it).second->m_dwKEY,
				dwAtkID,
				bAtkType,
				bBloodType,
				dwBlood);
	}

	return EC_NOERROR;
}


DWORD CTWorldSvrModule::OnMW_DEALITEMERROR_ACK(LPPACKETBUF pBUF)
{
	CString strTarget;
	CString strErrorChar;
	BYTE bError;

	pBUF->m_packet
		>> strTarget
		>> strErrorChar
		>> bError;

	LPTCHARACTER pChar = FindTChar(strTarget);
	if(pChar)
	{
		CTServer * pMap = FindMapSvr(pChar->m_bMainID);
		if(pMap)
			pMap->SendMW_DEALITEMERROR_REQ(strTarget, strErrorChar, bError);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_MAGICMIRROR_ACK(LPPACKETBUF pBUF)
{
	DWORD dwHostID;
	DWORD dwAttackID;
	DWORD dwTargetID;

	BYTE bAttackType;
	BYTE bTargetType;

	pBUF->m_packet
		>> dwHostID
		>> dwAttackID
		>> dwTargetID
		>> bAttackType
		>> bTargetType;

	MAPTCHARACTER::iterator itCHAR = m_mapTCHAR.find(dwAttackID);
	if( itCHAR != m_mapTCHAR.end())
	{
		CTServer * pMap = FindMapSvr((*itCHAR).second->m_bMainID);
		if(pMap)
			pMap->SendMW_MAGICMIRROR_REQ(&pBUF->m_packet);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CREATERECALLMON_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwMonID;
	WORD wMon;
	DWORD dwATTR;
	WORD wPetID;
	CString strName;
	DWORD dwTime;
	BYTE bLevel;
	BYTE bClass;
	BYTE bRace;
	BYTE bAction;
	BYTE bStatus;
	BYTE bMode;
	DWORD dwMaxHP;
	DWORD dwMaxMP;
	DWORD dwHP;
	DWORD dwMP;
	BYTE bHit;
	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;
	WORD wDir;
	BYTE bSkillCount;
	WORD wSkillID;
	BYTE bSkillLevel;

	VWORD vSkill;
	vSkill.clear();

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwMonID
		>> wMon
		>> dwATTR
		>> wPetID
		>> strName
		>> bLevel
		>> bClass
		>> bRace
		>> bAction
		>> bStatus
		>> bMode
		>> dwMaxHP
		>> dwMaxMP
		>> dwHP
		>> dwMP
		>> bHit
		>> bSkillLevel
		>> fPosX
		>> fPosY
		>> fPosZ
		>> wDir
		>> dwTime
		>> bSkillCount;

	for(BYTE i=0; i< bSkillCount; i++)
	{
		pBUF->m_packet
			>> wSkillID;

		vSkill.push_back(wSkillID);
	}

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar)
	{
		vSkill.clear();
		return EC_NOERROR;
	}

	if(!dwMonID)
		dwMonID = GenRecallID();

	MAPTCHARCON::iterator itCON;
	for(itCON =pChar->m_mapTCHARCON.begin(); itCON != pChar->m_mapTCHARCON.end(); itCON++)
	{
		if((*itCON).second->m_bValid)
		{
			CTServer *pMAP = FindMapSvr((*itCON).first);
			if(pMAP)
			{
				pMAP->SendMW_CREATERECALLMON_REQ(
					dwCharID,
					dwKey,
					dwMonID,
					wMon,
					dwATTR,
					wPetID,
					strName,
					dwTime,
					bLevel,
					bClass,
					bRace,
					bAction,
					bStatus,
					bMode,
					dwMaxHP,
					dwMaxMP,
					dwHP,
					dwMP,
					bHit,
					bSkillLevel,
					fPosX,
					fPosY,
					fPosZ,
					wDir,
					vSkill);
			}
		}
	}

	vSkill.clear();

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_RECALLMONDEL_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwMonID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwMonID;

	LPTCHARACTER pChar;
	MAPTCHARACTER::iterator it = m_mapTCHAR.find(dwCharID);
	if(it==m_mapTCHAR.end())
		return EC_NOERROR;

	pChar = it->second;

	MAPTCHARCON::iterator itCON;
	for(itCON =pChar->m_mapTCHARCON.begin(); itCON != pChar->m_mapTCHARCON.end(); itCON++)
	{
		if((*itCON).second->m_bValid)
		{
			CTServer *pMAP = FindMapSvr((*itCON).first);
			if(pMAP)
			{
				pMAP->SendMW_RECALLMONDEL_REQ(
					dwCharID,
					dwKey,
					dwMonID);
			}
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_REGION_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwRegion;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwRegion;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKEY);
	if(!pTCHAR)
		return EC_NOERROR;

	pTCHAR->m_dwRegion = dwRegion;

	MAPTSOULMATE::iterator itSo;
	for(itSo = pTCHAR->m_mapTSOULMATE.begin(); itSo != pTCHAR->m_mapTSOULMATE.end(); itSo++)
	{
		LPTSOULMATE pTSOUL = (*itSo).second;
		if(pTSOUL->m_dwCharID != pTCHAR->m_dwCharID)
		{
			MAPTCHARACTER::iterator itCh = m_mapTCHAR.find(pTSOUL->m_dwCharID);
			if(itCh != m_mapTCHAR.end())
			{
				LPTCHARACTER pSoulChar = (*itCh).second;
				MAPTSOULMATE::iterator itTS = pSoulChar->m_mapTSOULMATE.find(pSoulChar->m_dwCharID);
				if(itTS != pSoulChar->m_mapTSOULMATE.end())
				{
					(*itTS).second->m_bConnected = TRUE;
					(*itTS).second->m_dwRegion = dwRegion;
				}
			}
		}
	}

	MAPTFRIEND::iterator it;
	for(it=pTCHAR->m_mapTFRIEND.begin(); it!=pTCHAR->m_mapTFRIEND.end(); it++)
	{
		if((*it).second->m_bType == FT_FRIEND)
			continue;

		if((*it).second->m_bConnected)
		{
			LPTCHARACTER pTarget = FindTChar((*it).second->m_strName);
			if(pTarget)
			{
				MAPTFRIEND::iterator itFr = pTarget->m_mapTFRIEND.find(dwCharID);
				if(itFr != pTarget->m_mapTFRIEND.end())
					(*itFr).second->m_dwRegion = dwRegion;
			}
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_BEGINTELEPORT_ACK(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bSameChannel;
	BYTE bChannel;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bSameChannel
		>> bChannel;

	LPTCHARACTER pTCHAR = FindTChar( dwCharID, dwKEY);
	if(!pTCHAR)
	{
		pSERVER->SendMW_DELCHAR_REQ(
			dwCharID,
			dwKEY,
			TRUE,
			FALSE);

		return EC_NOERROR;
	}

	if(bSameChannel)
	{
		pTCHAR->m_bChannel = bChannel;
		return EC_NOERROR;
	}

	// 연결관리 프로세스 큐에 등록 한 후 이미 실행 중이거나 실행대기 중인 연결관리 프로세스가 있으면
	// 먼저 등록된 연결관리 프로세스가 끝날때까지 대기
	if(PushConCess( pTCHAR, pBUF))
		return EC_NOERROR;

	// 이미 실행 중이거나 실행대기 중인 연결관리 프로세스가 없으면 텔레포트 시작
	pBUF->m_packet.Rewind(FALSE);
	OnBeginTeleport( pTCHAR, pBUF);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_PETRIDING_ACK(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwRiding;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwRiding;

	LPTCHARACTER pTCHAR = FindTChar( dwCharID, dwKEY);
	if(!pTCHAR)
		return EC_NOERROR;

	pTCHAR->m_dwRiding = dwRiding;

	MAPTCHARCON::iterator itCON;
	for( itCON = pTCHAR->m_mapTCHARCON.begin(); itCON != pTCHAR->m_mapTCHARCON.end(); itCON++)
	{
		CTServer *pMAP = FindMapSvr((*itCON).first);

		if(pMAP && pSERVER != pMAP)
		{
			pMAP->SendMW_PETRIDING_REQ(
				pTCHAR->m_dwCharID,
				pTCHAR->m_dwKEY,
				dwRiding);
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnSM_EVENTQUARTER_REQ(LPPACKETBUF pBUF)
{
	BYTE bDay, bHour, bMinute;
	CString strPresent;
	pBUF->m_packet
		>> bDay
		>> bHour
		>> bMinute
		>> strPresent;

	BYTE bSelect = BYTE(rand() % 100);

	for(MAPTSERVER::iterator it=m_mapSERVER.begin(); it!=m_mapSERVER.end(); it++)
		(*it).second->SendMW_EVENTQUARTER_REQ(bDay, bHour, bMinute, bSelect, strPresent);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnSM_EVENTQUARTERNOTIFY_REQ(LPPACKETBUF pBUF)
{
	CString strAnnounce;
	pBUF->m_packet
		>> strAnnounce;

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it!=m_mapSERVER.end(); it++)
		(*it).second->SendMW_CHAT_REQ(
		0,
		0,
		0,
		0,
		GetSvrMsg(NAME_OPERATOR),
		TCONTRY_N,
		TCONTRY_N,
		CHAT_WORLD,
		CHAT_WORLD,
		0,
		strAnnounce);


	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_HELMETHIDE_ACK(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bHide;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bHide;

	LPTCHARACTER pTCHAR = FindTChar( dwCharID, dwKEY);
	if(!pTCHAR)
		return EC_NOERROR;

	pTCHAR->m_bHelmetHide = bHide;

	pSERVER->SendMW_HELMETHIDE_REQ(
		pTCHAR->m_dwCharID,
		pTCHAR->m_dwKEY,
		bHide);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_PARTYMEMBERRECALL_ACK(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bInvenID;
	BYTE bItemID;
	CString strOrigin;
	CString strTarget;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bInvenID
		>> bItemID
		>> strOrigin
		>> strTarget;

	LPTCHARACTER pTCHAR = FindTChar( dwCharID, dwKEY );
	if(!pTCHAR)
		return EC_NOERROR;

	if(strOrigin == pTCHAR->m_strNAME)
	{
		LPTCHARACTER pTarget = FindTChar( strTarget );
		if(pTarget &&
			pTarget->m_pParty &&
			pTarget->m_wMapID == pTCHAR->m_wMapID &&
			pTarget->m_pParty->GetID() == (pTCHAR->m_pParty ? pTCHAR->m_pParty->GetID() : 0))
		{
			CTServer * pMAP = FindMapSvr(pTarget->m_bMainID);
			if(pMAP)
			{
				pMAP->SendMW_PARTYMEMBERRECALLANS_REQ(
					pTarget->m_dwCharID,
					pTarget->m_dwKEY,
					strOrigin,
					TP_RECALL,
					bInvenID,
					bItemID);

				return EC_NOERROR;
			}
		}
		else
		{
			pSERVER->SendMW_PARTYMEMBERRECALL_REQ(
				pTCHAR->m_dwCharID,
				pTCHAR->m_dwKEY,
				IU_TARGETBUSY,
				strTarget,
				TP_RECALL);
		}
	}
	else
	{
		LPTCHARACTER pOrigin = FindTChar( strOrigin);
		if(pOrigin &&
			pOrigin->m_wMapID == pTCHAR->m_wMapID &&
			GetWarCountry(pOrigin) == GetWarCountry(pTCHAR))
		{
			CTServer * pMAP = FindMapSvr(pOrigin->m_bMainID);
			if(pMAP)
			{
				pMAP->SendMW_PARTYMEMBERRECALLANS_REQ(
					pOrigin->m_dwCharID,
					pOrigin->m_dwKEY,
					strTarget,
					TP_MOVETO,
					bInvenID,
					bItemID);

				return EC_NOERROR;
			}
		}
		else
		{
			pSERVER->SendMW_PARTYMEMBERRECALL_REQ(
				pTCHAR->m_dwCharID,
				pTCHAR->m_dwKEY,
				IU_TARGETBUSY,
				strOrigin,
				TP_MOVETO);
		}
	}
	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_PARTYMEMBERRECALLANS_ACK(LPPACKETBUF pBUF)
{
	BYTE bResult;
	CString strUser;
	CString strTarget;
	BYTE bType;
	BYTE bInven;
	BYTE bItem;
	BYTE bChannel;
	WORD wMapID;
	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> bResult
		>> strUser
		>> strTarget
		>> bType
		>> bInven
		>> bItem
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ;

	LPTCHARACTER pUser = FindTChar( strUser);
	if(pUser)
	{
		CTServer * pMAP = FindMapSvr(pUser->m_bMainID);
		if(pMAP)
		{
			if(pUser->m_wMapID != wMapID)
				bResult = IU_TARGETBUSY;

			if(IsMeetingRoom(pUser->m_wMapID, TRUE))
				bResult = IU_TARGETBUSY;

			pMAP->SendMW_PARTYMEMBERRECALL_REQ(
				pUser->m_dwCharID,
				pUser->m_dwKEY,
				bResult,
				strTarget,
				bType,
				bInven,
				bItem,
				bChannel,
				wMapID,
				fPosX,
				fPosY,
				fPosZ);
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDLOAD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwGuildID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwGuildID;

	BYTE bRet = FALSE;
	__time64_t timeEstablish;
	LPPACKETBUF pMSG = new PACKETBUF();
	DEFINE_QUERY(&m_db, CTBLGuildLoad)
	query->m_dwGuildID = dwGuildID;
	if(query->Open())
	{
		query->Fetch();
		if(query->m_dwChief == dwCharID)
		{
			bRet = TRUE;
			timeEstablish = __DBTOTIME(query->m_timeEstablish);
			pMSG->m_packet.SetID(DM_GUILDLOAD_ACK)
				<< dwCharID
				<< dwKey
				<< dwGuildID
				<< query->m_szName
				<< query->m_dwFame
				<< query->m_dwFameColor
				<< query->m_bMaxCabinet
				<< query->m_bGPoint
				<< query->m_bLevel
				<< query->m_dwChief
				<< query->m_dwExp
				<< query->m_dwGI
				<< query->m_bStatus
				<< query->m_dwGold
				<< query->m_dwSilver
				<< query->m_dwCooper
				<< query->m_bDisorg
				<< query->m_dwTime
				<< timeEstablish
				<< query->m_dwPvPTotalPoint
				<< query->m_dwPvPUseablePoint;
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	if(!bRet)
	{
		delete pMSG;
		return EC_NOERROR;
	}

	WORD wSize = pMSG->m_packet.GetSize();
	WORD wCount = 0;
	pMSG->m_packet
		<< wCount;

	DEFINE_QUERY(&m_db, CTBLGuildItem)
	query->m_dwOwnerID = dwGuildID;
	query->m_bOwnerType = TOWNER_GUILD;
	query->m_bStorageType = STORAGE_CABINET;

	if(query->Open())
	{
		while(query->Fetch())
		{
			query->m_item.m_dEndTime = __DBTOTIME(query->m_dEndTime);
			WrapItem(&(pMSG->m_packet), query->m_item);

			wCount++;
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	memcpy(pMSG->m_packet.GetBuffer()+wSize, &wCount, sizeof(wCount));

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDLOAD_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwGuildID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwGuildID;

	LPTCHARACTER pTCHAR = FindTChar(dwCharID, dwKey);
	if(!pTCHAR)
		return EC_NOERROR;

	MAPTGUILD::iterator it = m_mapTGuild.find(dwGuildID);
	if(it != m_mapTGuild.end())
		return EC_NOERROR;

	WORD wCount;
	CTGuild * pGuild = new CTGuild();
	pGuild->m_dwID = dwGuildID;

	pBUF->m_packet
		>> pGuild->m_strName
		>> pGuild->m_dwFame
		>> pGuild->m_dwFameColor
		>> pGuild->m_bMaxCabinet
		>> pGuild->m_bGPoint
		>> pGuild->m_bLevel
		>> pGuild->m_dwChief
		>> pGuild->m_dwExp
		>> pGuild->m_dwGI
		>> pGuild->m_bStatus
		>> pGuild->m_dwGold
		>> pGuild->m_dwSilver
		>> pGuild->m_dwCooper
		>> pGuild->m_bDisorg
		>> pGuild->m_dwTime
		>> pGuild->m_timeEstablish
		>> pGuild->m_dwPvPTotalPoint
		>> pGuild->m_dwPvPUseablePoint
		>> wCount;

	m_mapTGuild.insert(MAPTGUILD::value_type(pGuild->m_dwID, pGuild));
	if(pGuild->m_bDisorg)
		m_mapTGuildEx.insert(MAPDWORD::value_type(pGuild->m_dwID, pGuild->m_dwTime));

	pGuild->m_pTLEVEL = FindGuildLevel(pGuild->m_bLevel);
	pGuild->m_bCountry = pTCHAR->m_bCountry;
	pGuild->m_dwChief = pTCHAR->m_dwCharID;
	pGuild->m_strChief = pTCHAR->m_strNAME;

	for(WORD i=0; i<wCount; i++)
	{
		LPTITEM pItem = CreateItem(&(pBUF->m_packet));
		pGuild->PutInItem(pItem->m_dwItemID, pItem);
	}

	LPTGUILDMEMBER pMember = new TGUILDMEMBER();
	pMember->m_pChar = pTCHAR;
	pMember->m_bDuty = GUILD_DUTY_CHIEF;
	pMember->m_bPeer = GUILD_PEER_NONE;
	pMember->m_dwID = dwCharID;
	pMember->m_bLevel = pTCHAR->m_bLevel;
	pMember->m_bClass = pTCHAR->m_bClass;
	pMember->m_strName = pTCHAR->m_strNAME;
	pMember->m_dwTactics = FindCharTacticsID(dwCharID);
	pMember->m_bWarCountry = GetWarCountry(pTCHAR);
	GuildMemberAdd(pGuild, pMember);
	pTCHAR->m_pGuild = pGuild;

	CTServer *pMAP = FindMapSvr(pTCHAR->m_bMainID);
	if(!pMAP)
		return EC_NOERROR;

	pMAP->SendMW_GUILDESTABLISH_REQ(
		dwCharID,
		dwKey,
		GUILD_SUCCESS,
		dwGuildID,
		pGuild->m_strName,
		FALSE);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_SOULMATELIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_SOULMATELIST_ACK)
		<< dwCharID
		<< dwKEY;

	WORD wSize = pMSG->m_packet.GetSize();
	DWORD dwCount=0;
	pMSG->m_packet
		<< dwCount;

	DEFINE_QUERY(&m_db, CTBLSoulmate)
	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwCharID
				<< query->m_dwTarget
				<< query->m_szName
				<< query->m_bLevel
				<< query->m_bClass
				<< query->m_dwTime;

			dwCount++;
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLMySoulmate)
	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwCharID
				<< query->m_dwTarget
				<< query->m_szName
				<< query->m_bLevel
				<< query->m_bClass
				<< query->m_dwTime;

			dwCount++;
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	memcpy(pMSG->m_packet.GetBuffer()+wSize,&dwCount,sizeof(dwCount));

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_SOULMATELIST_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pPlayer = FindTChar(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

	DWORD dwCount;

	pBUF->m_packet
		>> dwCount;

	for(DWORD i=0; i<dwCount; i++)
	{
		LPTSOULMATE pTSOULMATE = new TSOULMATE();
		pTSOULMATE->m_bConnected = FALSE;
		pTSOULMATE->m_dwRegion = 0;

		pBUF->m_packet
			>> pTSOULMATE->m_dwCharID
			>> pTSOULMATE->m_dwTarget
			>> pTSOULMATE->m_strName
			>> pTSOULMATE->m_bLevel
			>> pTSOULMATE->m_bClass
			>> pTSOULMATE->m_dwTime;

		BYTE bSilence = FALSE;
		if(pTSOULMATE->m_dwTime && m_timeCurrent - pTSOULMATE->m_dwTime < SOULMATE_SILENCE_DURATION)
			bSilence = TRUE;

		if(pTSOULMATE->m_dwTime && !bSilence)
		{
			SendDM_SOULMATEDEL_REQ(pTSOULMATE->m_dwCharID, pTSOULMATE->m_dwTarget);
			delete pTSOULMATE;
			continue;
		}

		if(!bSilence)
		{
			pPlayer->m_mapTSOULMATE.insert(MAPTSOULMATE::value_type(pTSOULMATE->m_dwCharID, pTSOULMATE));

			LPTCHARACTER pCHAR = NULL;
			LPTSOULMATE pSOUL = NULL;

			if(pTSOULMATE->m_dwCharID == pPlayer->m_dwCharID)
			{
				pCHAR = pPlayer;
				pSOUL = pTSOULMATE;

				MAPTCHARACTER::iterator itSo = m_mapTCHAR.find(pTSOULMATE->m_dwTarget);
				if(itSo!=m_mapTCHAR.end())
				{
					pTSOULMATE->m_bConnected = TRUE;
					pTSOULMATE->m_dwRegion = (*itSo).second->m_dwRegion;
				}
			}
			else
			{
				MAPTCHARACTER::iterator itSo = m_mapTCHAR.find(pTSOULMATE->m_dwCharID);
				if(itSo!=m_mapTCHAR.end())
				{
					pCHAR = (*itSo).second;
					pTSOULMATE->m_bConnected = TRUE;
					pTSOULMATE->m_dwRegion = (*itSo).second->m_dwRegion;

					MAPTSOULMATE::iterator findSOUL = pCHAR->m_mapTSOULMATE.find(pCHAR->m_dwCharID);
					if(findSOUL!=pCHAR->m_mapTSOULMATE.end())
					{
						pSOUL = (*findSOUL).second;
						pSOUL->m_bConnected = TRUE;
						pSOUL->m_dwRegion = pPlayer->m_dwRegion;
					}
				}
			}

			CheckSoulmateEnd(pCHAR, pSOUL);
		}

		if(pTSOULMATE->m_dwCharID == pPlayer->m_dwCharID)
		{
			if(bSilence)
				pPlayer->m_dwSoulSilence = pTSOULMATE->m_dwTime;

			CTServer *pServer = FindMapSvr(pPlayer->m_bMainID);
			if(pServer)
				pServer->SendMW_SOULMATE_REQ(
					pPlayer->m_dwCharID,
					pPlayer->m_dwKEY,
					bSilence ? 0 : pTSOULMATE->m_dwTarget,
					bSilence ? NAME_NULL : pTSOULMATE->m_strName,
					bSilence ? pTSOULMATE->m_dwTime : 0);
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_SOULMATEREG_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTarget;
	BYTE bSearch;
	BYTE bNpcInvenID;  
	BYTE bNpcItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTarget
		>> bSearch
		>> bNpcInvenID
		>> bNpcItemID;

	DEFINE_QUERY(&m_db, CSPSoulmateReg)
	query->m_dwCharID = dwCharID;
	query->m_dwTarget = dwTarget;
	query->Call();
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_SOULMATEREG_ACK)
		<< dwCharID
		<< dwKey
		<< dwTarget
		<< bSearch
		<< bNpcInvenID
		<< bNpcItemID;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_SOULMATEREG_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTarget;
	BYTE bSearch;
	BYTE bNpcInvenID;  
	BYTE bNpcItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTarget
		>> bSearch
		>> bNpcInvenID
		>> bNpcItemID;


	LPTCHARACTER pTCHAR = FindTChar( dwCharID, dwKey);
	if(!pTCHAR)
		return EC_NOERROR;

	MAPTCHARACTER::iterator it = m_mapTCHAR.find(dwTarget);
	if(it!=m_mapTCHAR.end())
		RegSoulmate(pTCHAR, (*it).second, bSearch, bNpcInvenID, bNpcItemID);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_SOULMATEEND_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTime;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTime;

	DEFINE_QUERY(&m_db, CSPSoulmateEnd)
	query->m_dwCharID = dwCharID;
	query->m_dwTime = dwTime;
	query->Call();
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_SOULMATEEND_ACK)
		<< dwCharID
		<< dwKey
		<< dwTime;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_SOULMATEEND_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTime;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTime;

	LPTCHARACTER pTCHAR = FindTChar( dwCharID, dwKey);
	if(!pTCHAR)
		return EC_NOERROR;

	SoulmateEnd(pTCHAR, dwTime);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_SOULMATEDEL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwSoul;

	pBUF->m_packet
		>> dwCharID
		>> dwSoul;

	DEFINE_QUERY(&m_db, CSPSoulmateDel)
	query->m_dwCharID = dwCharID;
	query->m_dwSoul = dwSoul;
	query->Call();
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_SOULMATEDEL_ACK)
		<< dwCharID
		<< dwSoul;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_SOULMATEDEL_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwSoul;

	pBUF->m_packet
		>> dwCharID
		>> dwSoul;

	MAPTCHARACTER::iterator it = m_mapTCHAR.find(dwCharID);
	if(it!=m_mapTCHAR.end())
		SoulmateDel((*it).second, dwSoul);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_SOULMATESEARCH_ACK(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bMinLevel;
	BYTE bNpcInvenID;  
	BYTE bNpcItemID;


	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bMinLevel
		>> bNpcInvenID
		>> bNpcItemID;

	LPTCHARACTER pTCHAR = FindTChar( dwCharID, dwKEY);
	if(!pTCHAR)
		return EC_NOERROR;

	BYTE bLevel = bMinLevel;

	VECTORTMEMBER vMember;
	vMember.clear();

	// 레벨차이가 가장 작은 대상 / 이웃인 대상
	MAPTCHARACTER::iterator itPLAYERS;
	for(itPLAYERS=m_mapTCHAR.begin(); itPLAYERS!=m_mapTCHAR.end(); itPLAYERS++)
	{
		if(dwCharID == (*itPLAYERS).second->m_dwCharID ||
			pTCHAR->m_bCountry != (*itPLAYERS).second->m_bCountry)
			continue;

		if(abs(pTCHAR->m_bLevel - (*itPLAYERS).second->m_bLevel) < SOULMATE_LEVEL)
		{
			if(bLevel > (*itPLAYERS).second->m_bLevel)
			{
				vMember.clear();
				vMember.push_back((*itPLAYERS).second);
				bLevel = (*itPLAYERS).second->m_bLevel;
			}
			else if(bLevel == (*itPLAYERS).second->m_bLevel)
				vMember.push_back((*itPLAYERS).second);
		}
	}

	if(!vMember.size())
	{
		pSERVER->SendMW_SOULMATESEARCH_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY,
			SOULMATE_NOTFOUND,
			0,NAME_NULL,0,bNpcInvenID,bNpcItemID);
		return EC_NOERROR;
	}

	VECTORTMEMBER vTemp;

	// 실제성별이 이성인 대상
	if(vMember.size() != 1)
	{
		vTemp.clear();

		for(DWORD i=0; i<vMember.size(); i++)
			if(pTCHAR->m_bRealSex != vMember[i]->m_bRealSex)
				vTemp.push_back(vMember[i]);

		if(vTemp.size())
			vMember = vTemp;
		vTemp.clear();
	}

	// 소울메이트를 지정하지 않은 대상
	if(vMember.size() != 1)
	{
		vTemp.clear();

		for(DWORD i=0; i<vMember.size(); i++)
		{
			MAPTSOULMATE::iterator find = vMember[i]->m_mapTSOULMATE.find(vMember[i]->m_dwCharID);
			if(find==vMember[i]->m_mapTSOULMATE.end())
				vTemp.push_back(vMember[i]);
			else if((*find).second->m_dwTarget == 0)
				vTemp.push_back(vMember[i]);
		}

		if(vTemp.size())
			vMember = vTemp;
		vTemp.clear();
	}

	// 케릭터의 성별이 이성인 대상
	if(vMember.size() != 1)
	{
		vTemp.clear();

		for(DWORD i=0; i<vMember.size(); i++)
			if(pTCHAR->m_bSex != vMember[i]->m_bSex)
				vTemp.push_back(vMember[i]);

		if(vTemp.size())
			vMember = vTemp;
		vTemp.clear();
	}

	SendDM_SOULMATEREG_REQ(pTCHAR->m_dwCharID, pTCHAR->m_dwKEY, vMember[0]->m_dwCharID, TRUE, bNpcInvenID, bNpcItemID);

	vMember.clear();

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_SOULMATEREG_ACK(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	CString strName;
	BYTE bReg;
	BYTE bNpcInvenID;  
	BYTE bNpcItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strName
		>> bReg
		>> bNpcInvenID
		>> bNpcItemID;

	LPTCHARACTER pTCHAR = FindTChar( dwCharID, dwKEY);
	if(!pTCHAR)
		return EC_NOERROR;

	LPTCHARACTER pTarget = FindTChar(strName);
	if(!pTarget)
	{
		pSERVER->SendMW_SOULMATEREG_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY,
			SOULMATE_NOTFOUND,
			bReg,
			bNpcInvenID,
			bNpcItemID);
		return EC_NOERROR;
	}

	if(pTarget->m_bCountry != pTCHAR->m_bCountry)
	{
		pSERVER->SendMW_SOULMATEREG_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY,
			SOULMATE_FAIL,
			bReg,
			bNpcInvenID,
			bNpcItemID);

		return EC_NOERROR;
	}

	if(abs(pTarget->m_bLevel - pTCHAR->m_bLevel) > SOULMATE_LEVEL)
	{
		pSERVER->SendMW_SOULMATEREG_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY,
			SOULMATE_FAIL,
			bReg,
			bNpcInvenID,
			bNpcItemID);

		return EC_NOERROR;
	}

	DWORD dwSoulmate = pTarget->m_dwCharID;
	CString strSoulmate = pTarget->m_strNAME;
	DWORD dwRegion = pTarget->m_dwRegion;

	if(bReg)
		SendDM_SOULMATEREG_REQ(pTCHAR->m_dwCharID, pTCHAR->m_dwKEY, pTarget->m_dwCharID, FALSE,bNpcInvenID,bNpcItemID);
	else
		pSERVER->SendMW_SOULMATEREG_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY,
			SOULMATE_SUCCESS,
			bReg,
			bNpcInvenID,
			bNpcItemID,
			dwSoulmate,
			strSoulmate,
			dwRegion);

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_SOULMATEEND_ACK(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTCHAR = FindTChar( dwCharID, dwKEY);
	if(!pTCHAR)
		return EC_NOERROR;

	MAPTSOULMATE::iterator find = pTCHAR->m_mapTSOULMATE.find(dwCharID);
	if(find==pTCHAR->m_mapTSOULMATE.end())
	{
		pSERVER->SendMW_SOULMATEEND_REQ(
			pTCHAR->m_dwCharID,
			pTCHAR->m_dwKEY,
			SOULMATE_FAIL);
		return EC_NOERROR;
	}

	SendDM_SOULMATEEND_REQ(dwCharID, dwKEY, (DWORD)m_timeCurrent);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CASTLEWARINFO_ACK(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	WORD wLocalID;
	WORD wCastle;
	DWORD dwGuild;
	BYTE bLocalCnt;

	pBUF->m_packet
		>> wCastle;

	if(!wCastle)
	{
		NotifyCastleWarInfo(pSERVER);
		return EC_NOERROR;
	}

	pBUF->m_packet
		>> dwGuild
		>> bLocalCnt;

	TCASTLEWARINFO cwi;
	cwi.m_wID = wCastle;

	for(BYTE i=0; i<bLocalCnt; i++)
	{
		pBUF->m_packet
			>> wLocalID;

		DWORD dwOccGuild;
		BYTE bOccType;

		for(BYTE j=0; j<6; j++)
		{
			pBUF->m_packet
				>> dwOccGuild
				>> bOccType;

			WORD wBonus = 0;

			if(OCCUPY_DEFEND == bOccType)
				wBonus = 11;
			else
				wBonus = 10;

			CTGuild * pGuild = FindTGuild(dwOccGuild);
			if(pGuild)
			{
				MAPDWORD::iterator itAll = cwi.m_mapGuild.find(dwOccGuild);
				if( itAll == cwi.m_mapGuild.end())
					cwi.m_mapGuild.insert(MAPDWORD::value_type(dwOccGuild, wBonus));
				else
					(*itAll).second += wBonus;

				if(bOccType == OCCUPY_ACCEPT)
				{
					MAPARRAY::iterator itOcc = cwi.m_mapOccupy.find(dwOccGuild);
					if( itOcc == cwi.m_mapOccupy.end())
					{
						VWORD vBValue;
						vBValue.clear();
						while(vBValue.size() < 6)
							vBValue.push_back(0);

						vBValue[j] = wBonus;
						cwi.m_mapOccupy.insert(MAPARRAY::value_type(dwOccGuild, vBValue));
					}
					else
					{
						(*itOcc).second[j] += wBonus;
					}
				}
			}
		}
	}

	m_mapCastleWarInfo.erase(wCastle);
	m_mapCastleWarInfo.insert(MAPCASTLEWARINFO::value_type(wCastle, cwi));

	NotifyCastleWarInfo();

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_ENDWAR_ACK(LPPACKETBUF pBUF)
{
	WORD wCastle;
	pBUF->m_packet
		>> wCastle;

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it!=m_mapSERVER.end(); it++)
		(*it).second->SendMW_ENDWAR_REQ(
				wCastle);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_RECALLMONDATA_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPTCHARACTER pTCHAR = FindTChar( dwCharID, dwKey);
	if(!pTCHAR)
		return EC_NOERROR;

	MAPTCHARCON::iterator itCON;
	for(itCON=pTCHAR->m_mapTCHARCON.begin(); itCON!=pTCHAR->m_mapTCHARCON.end(); itCON++)
	{
		if((*itCON).second->m_bValid)
		{
			CTServer *pMAP = FindMapSvr((*itCON).first);
			if(pMAP)
				pMAP->SendMW_RECALLMONDATA_REQ(&(pBUF->m_packet));
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CHANGECHARBASE_ACK(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	BYTE bType;
	BYTE bValue;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bType
		>> bValue
		>> strName;

	LPTCHARACTER pTCHAR = FindTChar( dwCharID, dwKey);
	if(!pTCHAR)
		return EC_NOERROR;

	switch(bType)
	{
	case IK_FACE:
		pTCHAR->m_bFace = bValue;
		break;
	case IK_HAIR:
		pTCHAR->m_bHair = bValue;
		break;
	case IK_RACE:
		pTCHAR->m_bRace = bValue;
		break;
	case IK_SEX:
		pTCHAR->m_bSex = bValue;
		break;
	case IK_COUNTRY:
	case IK_AIDCOUNTRY:
		{
			ChangeCountry(pTCHAR, bType, bValue);
		}
		return EC_NOERROR;
	case IK_NAME:
		{			
			MAPTFRIEND::iterator itFr;
			for(itFr=pTCHAR->m_mapTFRIEND.begin(); itFr!=pTCHAR->m_mapTFRIEND.end(); itFr++)
				SendDM_FRIENDERASE_REQ((*itFr).second->m_dwID, dwCharID);

			MAPTSOULMATE::iterator itSM;
			for(itSM=pTCHAR->m_mapTSOULMATE.begin(); itSM!=pTCHAR->m_mapTSOULMATE.end(); itSM++)
			{
				if((*itSM).second->m_dwCharID != pTCHAR->m_dwCharID)
					SendDM_SOULMATEDEL_REQ((*itSM).second->m_dwCharID, (*itSM).second->m_dwTarget);
			}

			LPTGUILDWANTEDAPP pApp = FindGuildWantedApp(dwCharID);
			if(pApp)
				pApp->m_strName = strName;

			LPTGUILDTACTICSWANTEDAPP pTacApp = FindGuildTacticsWantedApp(dwCharID);
			if(pTacApp)
				pTacApp->m_strName = strName;

			if(pTCHAR->m_pGuild)
			{
				LPTGUILDMEMBER pMember = pTCHAR->m_pGuild->FindMember(dwCharID);
				if(pMember)
					pMember->m_strName = strName;
			}

			if(pTCHAR->m_pTactics)
			{
				LPTTACTICSMEMBER pMember = pTCHAR->m_pTactics->FindTactics(dwCharID);
				if(pMember)
					pMember->m_strName = strName;
			}

			LPTNMTPLAYER pTnmt = FindTNMTPlayer(pTCHAR->m_dwCharID);
			if(pTnmt)
				pTnmt->m_strName = strName;

			CString strOriName = pTCHAR->m_strNAME;
			strOriName.MakeUpper();
			MAPTCHARACTERNAME::iterator it = m_mapTCHARNAME.find(strOriName);
			if(it != m_mapTCHARNAME.end())
				m_mapTCHARNAME.erase(it);

			pTCHAR->m_strNAME = strName;
			CString strChgName = strName;
			strChgName.MakeUpper();
			m_mapTCHARNAME.insert(MAPTCHARACTERNAME::value_type(strChgName, pTCHAR));
			if(m_pRelay)
				m_pRelay->SendRW_CHANGENAME_ACK(dwCharID, bType, bValue, strName);
		}
		break;
	}

	if(bType != IK_NAME)
	{
		MAPTCHARCON::iterator itCON;
		for(itCON=pTCHAR->m_mapTCHARCON.begin(); itCON!=pTCHAR->m_mapTCHARCON.end(); itCON++)
		{
			if((*itCON).second->m_bValid)
			{
				CTServer *pMAP = FindMapSvr((*itCON).first);
				if(pMAP)
					pMAP->SendMW_CHANGECHARBASE_REQ(
						dwCharID,
						dwKey,
						bType,
						bValue,
						strName);
			}
		}
	}
	else
	{
		MAPTSERVER::iterator it;
		for(it=m_mapSERVER.begin(); it != m_mapSERVER.end(); it++)
		{
			(*it).second->SendMW_CHANGECHARBASE_REQ(
				dwCharID,
				dwKey,
				bType,
				bValue,
				strName);
		}
	}

	return EC_NOERROR;
}

 
DWORD CTWorldSvrModule::OnMW_HEROSELECT_ACK(LPPACKETBUF pBUF) 
{
	WORD wBattleZoneID;	
	CString strHeroName;
	__time64_t timeHero;

	pBUF->m_packet
		>> wBattleZoneID
		>> strHeroName
		>> timeHero;

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it != m_mapSERVER.end(); it++)
	{
		(*it).second->SendMW_HEROSELECT_REQ(
			wBattleZoneID,
			strHeroName,
			timeHero);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnSM_BATTLESTATUS_REQ(LPPACKETBUF pBUF)
{
	BYTE bType;
	BYTE bStatus;
	DWORD dwStart;
	DWORD dwSecond;

	pBUF->m_packet
		>> bType
		>> bStatus
		>> dwStart
		>> dwSecond;

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it != m_mapSERVER.end(); it++)
	{
		switch(bType)
		{
		case BT_LOCAL:
			{
				(*it).second->SendMW_LOCALENABLE_REQ(
					bStatus,
					dwSecond,
					0,0,0);
			}
			break;
		case BT_CASTLE:
			{
				(*it).second->SendMW_CASTLEENABLE_REQ(
					bStatus,
					dwSecond);
			}
			break;
		case BT_MISSION:
			{
				(*it).second->SendMW_MISSIONENABLE_REQ(
					bStatus,
					dwStart,
					dwSecond);
			}
			break;
		default:
			break;
		}
	}

	if(bType != BT_MISSION && bStatus == BS_PEACE)
	{
		m_dwRecentRecordDate = DWORD(m_timeCurrent/DAY_ONE);

		MAPTGUILD::iterator itGD;
		for(itGD=m_mapTGuild.begin(); itGD!=m_mapTGuild.end(); itGD++)
			(*itGD).second->CalcWeekRecord(m_dwRecentRecordDate);

		if(bType == BT_CASTLE)
			m_mapCastleWarInfo.clear();
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_CLEARMAPCURRENTUSER_REQ(LPPACKETBUF pBUF)
{
	BYTE bGroupID;
	BYTE bServerID;
	BYTE bSvrType;

	pBUF->m_packet
		>> bGroupID
		>> bServerID
		>> bSvrType;

	DEFINE_QUERY(&m_db,CSPClearMapCurrentUser)
	query->m_bGroupID = bGroupID;
	query->m_bServerID = bServerID;
	query->m_bSvrType = bSvrType;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_ITEMFIND_REQ(LPPACKETBUF pBUF)
{
	DWORD dwManagerID;
	WORD wItemID;
	CString strName;

	pBUF->m_packet
		>> dwManagerID
		>> wItemID
		>> strName;

	CPacket* pMSG = new CPacket();
	WORD wCount = 0;
	WORD wSize = pMSG->GetSize();
	pMSG->SetID(CT_ITEMFIND_ACK)		
		<< wCount
		<< dwManagerID;

		
	DEFINE_QUERY(&m_db, CTBLItemFind)
	query->m_wFindID = wItemID;
	lstrcpy(query->m_szFindName,strName);
	
	if(query->Open())
	{
		while( query->Fetch() )
		{
			(*pMSG)
				<< query->m_wItemID
				<< query->m_bInitState
				<< query->m_szName;

			wCount++;
		}
		memcpy(pMSG->GetBuffer()+wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY();

	// to Ctrl Server
	if(m_pCtrlSvr)
		m_pCtrlSvr->Say(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_ITEMSTATE_REQ(LPPACKETBUF pBUF)
{
	WORD wCount;
	WORD wItemID;
	BYTE bInitState;
	DWORD dwID;

	pBUF->m_packet
		>> dwID
		>> wCount;

	WORD wSuccessCount = 0;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_ITEMSTATE_ACK);
	pMSG->m_packet
		<< dwID;

	WORD wSize = pMSG->m_packet.GetSize();
	pMSG->m_packet
		<< wSuccessCount;	

	DEFINE_QUERY(&m_db,CSPItemStateChange)	
	for(WORD i = 0; i < wCount; i++)
	{
		pBUF->m_packet
			>> wItemID
			>> bInitState;

		query->m_wItemID = wItemID;
		query->m_bInitState = bInitState;

		if( !query->Call() || query->m_nRET )
		{
			break;
		}
		else
		{
			wSuccessCount++;
			pMSG->m_packet
				<< wItemID
				<< bInitState;
		}
	
	}
	memcpy(pMSG->m_packet.GetBuffer()+wSize, &wSuccessCount, sizeof(wSuccessCount));
	UNDEFINE_QUERY()

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_ITEMSTATE_ACK(LPPACKETBUF pBUF)
{
	MAPTSERVER::iterator it;
	for(it = m_mapSERVER.begin(); it != m_mapSERVER.end(); it++)
	{
		(*it).second->SendMW_ITEMSTATE_REQ(&(pBUF->m_packet));
	}

	if(m_pCtrlSvr)
		m_pCtrlSvr->SendCT_ITEMSTATE_ACK(&(pBUF->m_packet));

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_GAINPVPPOINT_ACK(LPPACKETBUF pBUF)
{
	BYTE bOwnerType;
	DWORD dwOwnerID;
	DWORD dwPoint;
	BYTE bEvent;
	BYTE bType;
	BYTE bGain;

	CString strName;
	BYTE bClass;
	BYTE bLevel;

	pBUF->m_packet
		>> bOwnerType
		>> dwOwnerID
		>> dwPoint
		>> bEvent
		>> bType
		>> bGain
		>> strName
		>> bClass
		>> bLevel;

	if(bOwnerType == TOWNER_CHAR)
	{
		MAPTCHARACTER::iterator it = m_mapTCHAR.find(dwOwnerID);
		if(it==m_mapTCHAR.end())
			return EC_NOERROR;

		CTServer *pServer = FindMapSvr((*it).second->m_bMainID);
		if(pServer)
			pServer->SendMW_GAINPVPPOINT_REQ(dwOwnerID, dwPoint, bEvent, bType, bGain, strName, bClass, bLevel);
	}
	else
	{
		CTGuild * pGuild = FindTGuild(dwOwnerID);
		if(!pGuild)
			return EC_NOERROR;

		if(bGain)
			pGuild->GainPvPoint(dwPoint, bEvent, bType);
		else
			pGuild->UsePvPoint(dwPoint, bType);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_LOCALRECORD_ACK(LPPACKETBUF pBUF)
{
	DWORD dwWinGuildID;
	DWORD dwGuildPoint;
	WORD wGuildCount;
	WORD wRecordCount;
	WORD wKillCount;
	WORD wDieCount;
	DWORD dwPoint;
	DWORD dwGuildID;
	DWORD dwCharID;
	MAPDWORD::iterator itAid;
	MAPDWORD mapAidGuild;
	mapAidGuild.clear();

	DWORD dwDate = DWORD(m_timeCurrent/DAY_ONE);

	pBUF->m_packet
		>> dwWinGuildID
		>> dwGuildPoint
		>> wGuildCount;

	for(WORD g=0; g<wGuildCount; g++)
	{
		pBUF->m_packet
			>> dwGuildID
			>> wRecordCount;

		CTGuild * pGuild = FindTGuild(dwGuildID);

		for(WORD r=0; r<wRecordCount; r++)
		{
			LPTENTRYRECORD pRec = NULL;
			TENTRYRECORD rec;

			pBUF->m_packet
				>> dwCharID
				>> wKillCount
				>> wDieCount;

			BYTE bIsTactics = FALSE;
			LPTGUILDMEMBER pMember = NULL;
			LPTTACTICSMEMBER pTactics = NULL;

			if(pGuild)
				pMember = pGuild->FindMember(dwCharID);

			if(pMember)
			{
				if(!pMember->m_vRecord.empty() && pMember->m_vRecord.back().m_dwDate == dwDate)
					pRec = &(pMember->m_vRecord.back());
				else
				{
					pMember->m_vRecord.push_back(rec);
					pRec = &(pMember->m_vRecord.back());
					pRec->m_dwDate = dwDate;
				}

				pRec->m_wKillCount += wKillCount;
				pRec->m_wDieCount += wDieCount;
				pRec->m_bLoad = FALSE;
			}
			else if(pGuild)
			{
				pTactics = pGuild->FindTactics(dwCharID);
				if(pTactics)
				{
					bIsTactics = TRUE;
					if(pTactics->m_pChar && pTactics->m_pChar->m_pGuild)
					{
						pMember = pTactics->m_pChar->m_pGuild->FindMember(dwCharID);

						if(pMember)
						{
							if(!pMember->m_vRecord.empty() && pMember->m_vRecord.back().m_dwDate == dwDate)
								pRec = &(pMember->m_vRecord.back());
							else
							{
								pMember->m_vRecord.push_back(rec);
								pRec = &(pMember->m_vRecord.back());
								pRec->m_dwDate = dwDate;
							}

							pRec->m_wKillCount += wKillCount;
							pRec->m_wDieCount += wDieCount;
							pRec->m_bLoad = FALSE;
						}
					}
				}
			}

			for(BYTE e=0; e<PVPE_COUNT; e++)
			{
				pBUF->m_packet
					>> dwPoint;

				if(pRec &&
					(e != PVPE_ENTRY || m_mapTCHAR.find(dwCharID) != m_mapTCHAR.end()))
				{
					pRec->m_aGainPoint[e] += dwPoint;
					if(pTactics)
						pTactics->m_dwGainPoint += dwPoint;
				}
			}

			if(pMember)
			{
				if(bIsTactics)
				{
					if(pTactics &&
						pTactics->m_dwID == pMember->m_dwID &&
						pMember->m_pChar &&
						pMember->m_pChar->m_pGuild)
						pMember->m_pChar->m_pGuild->CalcWeekRecord(pMember, dwDate);

				}
				else if(pGuild)
					pGuild->CalcWeekRecord(pMember, dwDate);

				pMember->m_bSaveRecord = TRUE;
			}

			if(dwWinGuildID == dwGuildID &&
				pTactics &&
				pTactics->m_pChar &&
				pTactics->m_pChar->m_pGuild &&
				pTactics->m_pChar->m_pGuild->m_bCountry == TCONTRY_B)
			{
				itAid = mapAidGuild.find(pTactics->m_pChar->m_pGuild->m_dwID);
				if(itAid == mapAidGuild.end())
					mapAidGuild.insert(MAPDWORD::value_type(pTactics->m_pChar->m_pGuild->m_dwID, 1));
				else
					(itAid->second)++;
			}
		}

		for(itAid=mapAidGuild.begin(); itAid != mapAidGuild.end(); itAid++)
		{
			CTGuild * pTacticsGuild = FindTGuild(itAid->first);
			if(pTacticsGuild)
                pTacticsGuild->GainPvPoint(min(dwGuildPoint, dwGuildPoint * itAid->second / 20), 0, PVP_USEABLE | PVP_TOTAL);
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_GUILDPOINTLOG_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar || !pChar->m_pGuild || pChar->m_pTactics)
		return EC_NOERROR;

	pServer->SendMW_GUILDPOINTLOG_REQ(
		dwCharID,
		dwKey,
		pChar->m_pGuild);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_GUILDPOINTREWARD_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	CString strTarget;
	DWORD dwPoint;
	CString strMessage;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strTarget
		>> dwPoint
		>> strMessage;

	LPTCHARACTER pMaster = FindTChar(dwCharID, dwKey);
	if(!pMaster || !pMaster->m_pGuild)
		return EC_NOERROR;

	CTGuild * pGuild = pMaster->m_pGuild;

	if(pGuild->m_dwChief != dwCharID)
		return EC_NOERROR;

	if(pGuild->m_dwPvPUseablePoint < dwPoint)
	{
		pServer->SendMW_GUILDPOINTREWARD_REQ(GPR_NEEDPOINT, dwCharID, dwKey, pGuild->m_dwPvPUseablePoint);
		return EC_NOERROR;
	}

	LPTGUILDMEMBER pMember = pGuild->FindMember(strTarget);
	if(!pMember)
	{
		pServer->SendMW_GUILDPOINTREWARD_REQ(GPR_NOMEMBER, dwCharID, dwKey, pGuild->m_dwPvPUseablePoint);
		return EC_NOERROR;
	}

	if(pMember->m_pChar && !pMember->m_pChar->m_bSave)
	{
		pServer->SendMW_GUILDPOINTREWARD_REQ(GPR_NOMEMBER, dwCharID, dwKey, pGuild->m_dwPvPUseablePoint);
		return EC_NOERROR;
	}

	DWORD dwTargetID = pMember->m_dwID;

	pGuild->UsePvPoint(dwPoint, PVP_USEABLE);
	pGuild->PointLog(dwPoint, strTarget, m_timeCurrent);
	SendDM_GUILDPOINTREWARD_REQ(pGuild->m_dwID, dwPoint, strTarget, pGuild->m_dwPvPTotalPoint, pGuild->m_dwPvPUseablePoint);

	pServer->SendMW_GUILDPOINTREWARD_REQ(
		GPR_SUCCESS,
		dwCharID,
		dwKey,
		pGuild->m_dwPvPUseablePoint,
		dwPoint,
		dwTargetID,
		strTarget,
		strMessage);

	if(pMember->m_pChar)
	{
		CTServer * pMap = FindMapSvr(pMember->m_pChar->m_bMainID);
		if(pMap)
			pMap->SendMW_GAINPVPPOINT_REQ(pMember->m_dwID, dwPoint, PVPE_GUILD, PVP_USEABLE, TRUE);
	}
	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_GUILDPVPRECORD_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar || (!pChar->m_pGuild && !pChar->m_pTactics))
		return EC_NOERROR;

	CTGuild * pGuild = GetCurGuild(pChar);

	pServer->SendMW_GUILDPVPRECORD_REQ(
		dwCharID,
		dwKey,
		pGuild,
		m_dwRecentRecordDate);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDPVPOINT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwTotalPoint;
	DWORD dwUseablePoint;
	DWORD dwMonthPoint;

	pBUF->m_packet
		>> dwGuildID
		>> dwTotalPoint
		>> dwUseablePoint
		>> dwMonthPoint;

	DEFINE_QUERY(&m_db, CSPSaveGuildPvPoint)
	query->m_dwGuildID = dwGuildID;
	query->m_dwTotalPoint = dwTotalPoint;
	query->m_dwUseablePoint = dwUseablePoint;
	query->m_dwMonthPoint = dwMonthPoint;
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDPOINTREWARD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwPoint;
	CString strName;
	DWORD dwTotalPoint;
	DWORD dwUseablePoint;

	pBUF->m_packet
		>> dwGuildID
		>> dwPoint
		>> strName
		>> dwTotalPoint
		>> dwUseablePoint;

	DEFINE_QUERY(&m_db, CSPSaveGuildPointReward)
	query->m_dwGuildID = dwGuildID;
	query->m_dwPoint = dwPoint;
	lstrcpy(query->m_szName, strName);
	query->m_dwTotalPoint = dwTotalPoint;
	query->m_dwUseablePoint = dwUseablePoint;
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_PVPRECORD_REQ(LPPACKETBUF pBUF)
{
	WORD wCount;

	DEFINE_QUERY(&m_db, CSPSaveGuildPvPRecord)
	pBUF->m_packet
		>> query->m_dwGuildID
		>> query->m_dwMemberID
		>> wCount;

	for(WORD c=0; c<wCount; c++)
	{
		pBUF->m_packet
			>> query->m_dwDate
			>> query->m_wKillCount
			>> query->m_wDieCount;

		for(BYTE i=0; i<PVPE_COUNT; i++)
			pBUF->m_packet >> query->m_aPoint[i];

		query->Call();
	}
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_CASTLEAPPLY_REQ(LPPACKETBUF pBUF)
{
	WORD wCastle;
	DWORD dwTarget;
	BYTE bCamp;

	pBUF->m_packet
		>> wCastle
		>> dwTarget
		>> bCamp;

	DEFINE_QUERY(&m_db, CSPSaveCastleApplicant)
	query->m_wCastle = wCastle;
	query->m_dwCharID = dwTarget;
	query->m_bCamp = bCamp;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_MONSTERBUY_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	WORD wNpcID;
	WORD wID;
	DWORD dwPrice;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wNpcID
		>> wID
		>> dwPrice;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar || !pChar->m_pGuild)
		return EC_NOERROR;

	BYTE bRet = pChar->m_pGuild->UseMoney(dwPrice, TRUE);

	pServer->SendMW_MONSTERBUY_REQ(
		(!bRet ? MSB_NEEDMONEY : MSB_SUCCESS),
		dwCharID,
		dwKey,
		pChar->m_pGuild->m_dwID,
		wNpcID,
		wID,
		dwPrice);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_GUILDMONEYRECOVER_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwGuildID;
	DWORD dwPrice;

	pBUF->m_packet
		>> dwGuildID
		>> dwPrice;

	CTGuild * pGuild = FindTGuild(dwGuildID);
	if(!pGuild)
		return EC_NOERROR;

	pGuild->GainMoney(0, 0, dwPrice);

	return EC_NOERROR;
}
 
DWORD CTWorldSvrModule::OnMW_CASHITEMSALE_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwIndex;
	WORD wValue;
	BYTE bRet;

	pBUF->m_packet
		>> dwIndex
		>> wValue
		>> bRet;

	pServer->m_bCashSale = TRUE;

	BYTE bSuccess = TRUE;
	MAPTSERVER::iterator it;
	for( it = m_mapSERVER.begin(); it != m_mapSERVER.end(); it++)
	{
		if( (*it).second && !(*it).second->m_bCashSale )
		{
			bSuccess = FALSE;
			break;
		}
	}

	if(bSuccess)
	{		
		MAPTCASHITEMSALE::iterator it = m_mapTCashItemSale.find(dwIndex);
		if( it != m_mapTCashItemSale.end() )
            SendDM_CASHITEMSALE_REQ( dwIndex,wValue,&((*it).second.m_vCashItemSale) );
	}
	
	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_CASHITEMSALE_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet = TRUE;
	
	DWORD dwIndex;
	WORD wValue;
	WORD wCount;

	pBUF->m_packet
		>> dwIndex
		>> wValue
		>> wCount;


	DEFINE_QUERY(&m_db, CSPCashItemSale)
	
	for(WORD i =0; i < wCount; i++)
	{
		pBUF->m_packet
			>> query->m_wID
			>> query->m_bValue;

		if(!query->Call())
		{
			bRet = FALSE;
			break;
		}
	}
	
	UNDEFINE_QUERY()

	LPPACKETBUF  pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_CASHITEMSALE_ACK)
		<< dwIndex
		<< wValue
		<< bRet;

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_CASHITEMSALE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwIndex;
	WORD wValue;
	BYTE bRet;

	pBUF->m_packet
		>> dwIndex
		>> wValue
		>> bRet;


	if(!bRet)
		return EC_NOERROR;

	MAPTCASHITEMSALE::iterator itC = m_mapTCashItemSale.find(dwIndex);
	if( itC != m_mapTCashItemSale.end() && wValue==0 )
		m_mapTCashItemSale.erase(itC);

	MAPTSERVER::iterator it;
	for( it = m_mapSERVER.begin(); it != m_mapSERVER.end(); it++)
	{		
		(*it).second->SendMW_CASHSHOPSTOP_REQ(FALSE,FALSE);
	}
	
	if(wValue == 0 && m_pCtrlSvr)
		m_pCtrlSvr->SendCT_CASHITEMSALE_ACK(dwIndex,wValue);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnSM_EVENTEXPIRED_REQ(LPPACKETBUF pBUF)
{
	BYTE bInsert;
	TEXPIREDBUF buf;

	pBUF->m_packet
		>> bInsert
		>> buf.m_bType
		>> buf.m_timeExpired
		>> buf.m_dwValue_1
		>> buf.m_dwValue_2;

	VTEXPIREDBUF::iterator it;
	for(it = m_vExpired.begin(); it != m_vExpired.end(); it++)
	{
		if(!bInsert &&
			buf.m_timeExpired == (*it).m_timeExpired &&
			buf.m_bType == (*it).m_bType &&
			buf.m_dwValue_1 == (*it).m_dwValue_1 &&
			buf.m_dwValue_2 == (*it).m_dwValue_2)
		{
			m_vExpired.erase(it);
			return EC_NOERROR;
		}

		if( buf.m_timeExpired < (*it).m_timeExpired )
			break;
	}

	m_vExpired.insert(it,buf);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnSM_EVENTEXPIRED_ACK(LPPACKETBUF pBUF)
{
	BYTE bType;
	INT64 timeExpired;
	DWORD dwValue_1;
	DWORD dwValue_2;

	pBUF->m_packet
		>> bType
		>> timeExpired
		>> dwValue_1
		>> dwValue_2;

	switch(bType)
	{
	case EXPIRED_GMW:
		{
			CTGuild * pGuild = FindTGuild(dwValue_1);
			if(pGuild)
				DelGuildWanted(pGuild);

			SendDM_GUILDWANTEDDEL_REQ(dwValue_1);
		}
		break;
	case EXPIRED_GTW:
		{
			CTGuild * pGuild = FindTGuild(dwValue_1);
			if(pGuild)
				DelGuildTacticsWanted(pGuild, dwValue_2);

			SendDM_GUILDTACTICSWANTEDDEL_REQ(dwValue_2);
		}
		break;
	case EXPIRED_GT:
		{
			CTGuild * pGuild = FindTGuild(dwValue_1);
			if(pGuild)
			{
				LPTTACTICSMEMBER pMember = pGuild->FindTactics(dwValue_2);
				if(pMember)
					GuildTacticsDel(pGuild, pMember, 3);
			}
		}
		break;
	default:
		return EC_NOERROR;
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDWANTEDADD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	BYTE bMinLevel;
	BYTE bMaxLevel;
	CString strTitle;
	CString strText;

	pBUF->m_packet
		>> dwGuildID
		>> bMinLevel
		>> bMaxLevel
		>> strTitle
		>> strText;

	DEFINE_QUERY(&m_db, CSPGuildWantedAdd)
	query->m_dwGuildID = dwGuildID;
	query->m_bMinLevel = bMinLevel;
	query->m_bMaxLevel = bMaxLevel;
	__TIMETODB(m_timeCurrent + GUILDWANTED_PERIOD, query->m_timeEnd);
	lstrcpy(query->m_szTitle, strTitle);
	lstrcpy(query->m_szText, strText);

	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDWANTEDDEL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwGuildID;

	pBUF->m_packet
		>> dwGuildID;

	DEFINE_QUERY(&m_db, CSPGuildWantedDel)
	query->m_dwGuildID = dwGuildID;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDVOLUNTEERING_REQ(LPPACKETBUF pBUF)
{
	BYTE bType;
	DWORD dwCharID;
	DWORD dwID;

	pBUF->m_packet
		>> bType
		>> dwCharID
		>> dwID;

	DEFINE_QUERY(&m_db, CSPGuildVolunteering)
	query->m_bType = bType;
	query->m_dwCharID = dwCharID;
	query->m_dwID = dwID;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDVOLUNTEERINGDEL_REQ(LPPACKETBUF pBUF)
{
	BYTE bType;
	DWORD dwCharID;

	pBUF->m_packet
		>> bType
		>> dwCharID;

	DEFINE_QUERY(&m_db, CSPGuildVolunteeringDel)
	query->m_bType = bType;
	query->m_dwCharID = dwCharID;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDTACTICSWANTEDADD_REQ(LPPACKETBUF pBUF)
{
	DEFINE_QUERY(&m_db, CSPGuildTacticsWantedAdd)
	CString strTitle;
	CString strText;

	pBUF->m_packet
		>> query->m_dwID
		>> query->m_dwGuildID
		>> query->m_dwPoint
		>> query->m_dwGold
		>> query->m_dwSilver
		>> query->m_dwCooper
		>> query->m_bDay
		>> query->m_bMinLevel
		>> query->m_bMaxLevel
		>> strTitle
		>> strText;

	lstrcpy(query->m_szTitle, strTitle);
	lstrcpy(query->m_szText, strText);
	__TIMETODB(m_timeCurrent + GUILDWANTED_PERIOD, query->m_timeEnd);
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDTACTICSWANTEDDEL_REQ(LPPACKETBUF pBUF)
{
	DEFINE_QUERY(&m_db, CSPGuildTacticsWantedDel)

	pBUF->m_packet
		>> query->m_dwID;

	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDTACTICSADD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwCharID;
	DWORD dwPoint;
	BYTE bDay;
	INT64 dlEnd;
	DWORD dwCharGuildID = 0;
	INT64 dlMoney;

	pBUF->m_packet
		>> dwGuildID
		>> dwCharID
		>> dwPoint
		>> dlMoney
		>> bDay
		>> dlEnd;

	DEFINE_QUERY(&m_db, CSPGuildTacticsAdd)
	query->m_dwGuildID = dwGuildID;
	query->m_dwCharID = dwCharID;
	query->m_dwPoint = dwPoint;
	query->m_dlMoney = dlMoney;
	query->m_bDay = bDay;
	__TIMETODB(dlEnd, query->m_timeEnd);
	query->Call();

	dwCharGuildID = query->m_dwCharGuildID;
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDTACTICSADD_ACK)
		<< dwGuildID
		<< dwCharID
		<< dwPoint
		<< dlMoney
		<< bDay
		<< dlEnd
		<< dwCharGuildID;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDTACTICSADD_ACK(LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwCharID;
	DWORD dwPoint;
	BYTE bDay;
	INT64 dlEnd;
	DWORD dwCharGuildID = 0;
	INT64 dlMoney;

	pBUF->m_packet
		>> dwGuildID
		>> dwCharID
		>> dwPoint
		>> dlMoney
		>> bDay
		>> dlEnd
		>> dwCharGuildID;

	CTGuild * pGuild = FindTGuild(dwCharGuildID);
	if(pGuild)
	{
		LPTGUILDMEMBER pMember = pGuild->FindMember(dwCharID);
		if(pMember)
			pMember->m_dwTactics = dwGuildID;
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDTACTICSDEL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwCharGuildID = 0;

	pBUF->m_packet
		>> dwCharID;

	DEFINE_QUERY(&m_db, CSPGuildTacticsDel)
	query->m_dwCharID = dwCharID;
	query->Call();
	dwCharGuildID = query->m_dwCharGuildID;
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDTACTICSDEL_ACK)
		<< dwCharID
		<< dwCharGuildID;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GUILDTACTICSDEL_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwCharGuildID = 0;

	pBUF->m_packet
		>> dwCharID
		>> dwCharGuildID;

	CTGuild * pGuild = FindTGuild(dwCharGuildID);
	if(pGuild)
	{
		LPTGUILDMEMBER pMember = pGuild->FindMember(dwCharID);
		if(pMember)
			pMember->m_dwTactics = 0;
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_TACTICSPOINT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwGainPoint;

	pBUF->m_packet
		>> dwCharID
		>> dwGainPoint;

	DEFINE_QUERY(&m_db, CSPSaveTacticsGainPoint)
	query->m_dwCharID = dwCharID;
	query->m_dwGainPoint = dwGainPoint;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

 
DWORD CTWorldSvrModule::OnSM_MONTHRANKSAVE_REQ(LPPACKETBUF pBUF)
{
	MAPTGUILD::iterator itGd;
	for(itGd=m_mapTGuild.begin(); itGd!=m_mapTGuild.end(); itGd++)
	{
		itGd->second->m_dwPvPMonthPoint = 0;
		itGd->second->m_dwRankMonth = 0;
	}

 	m_bFameRankSave = TRUE;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_MONTHRANKSAVE_REQ);

	//////////////////////////////////////////////////////////////////////////
	// 전체 순위 선정
	BYTE t;
	MONTHRANKER	arTotalRank[TOTALMONTHRANKCOUNT];
	VMONTHRANKER vMonthRanker;
	vMonthRanker.clear();

	for(BYTE m = 0; m < COUNTRY_COUNT; m++)
        for(BYTE n = 1; n < FIRSTGRADEGROUPCOUNT; n++)
            vMonthRanker.push_back(m_arMonthRank[m][n]);
		
	std::sort(vMonthRanker.begin(), vMonthRanker.end(),MonthRankDesc());
	for(t = 0; t < (BYTE)vMonthRanker.size(); t++)
		arTotalRank[t+1] = vMonthRanker[t];

	BYTE bTop = COUNTRY_DEFUGEL;
	for(BYTE i=0; i<COUNTRY_COUNT; i++)
	{
		if(m_arMonthRank[bTop][0].m_dwTotalPoint < m_arMonthRank[i][0].m_dwTotalPoint)
			bTop = i;
	}

	for(BYTE i=0; i<COUNTRY_COUNT; i++)
	{
		if(bTop == i)
			arTotalRank[0] = m_arMonthRank[i][0];
		else
			arTotalRank[TOTALMONTHRANKCOUNT-i-1] = m_arMonthRank[i][0];
	}

	//////////////////////////////////////////////////////////////////////////

	for(t = 0; t < FAMERANKCOUNT; t++)
		m_arLastFameRank[t] = arTotalRank[t];

	
	BYTE k = 0; 
	for(BYTE i = 0; i < COUNTRY_COUNT; i++)
	{
		for(BYTE j = 0; j <  FIRSTGRADEGROUPCOUNT; j++)			
		{			
			for(t = k; t < TOTALMONTHRANKCOUNT; t++)
				if( m_arMonthRank[i][j].m_dwCharID == arTotalRank[t].m_dwCharID)
					break;
			
			pMSG->m_packet
				<< t;

            m_arMonthRank[i][j].WrapPacketIn( &(pMSG->m_packet));
			if(t == 0) //누적1위가 월간 순위 0위에 있을경우를 대비
				k = 1;
		}
	}

	SayToDB(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_MONTHRANKSAVE_REQ(LPPACKETBUF pBUF)
{
	DEFINE_QUERY(&m_db, CSPInitMonthRank)
	query->m_bMonth = m_bRankMonth;	
	query->Call();
	UNDEFINE_QUERY()
	
	BYTE	bChangeTopRanker = FALSE;
	DWORD	dwTopPoint = 0;
	MONTHRANKER stTOPRANKER;
	MONTHRANKER stMONTHRANKER;
	BYTE bSuccess = TRUE;

	stMONTHRANKER.Reset();
	stTOPRANKER.Reset();


	DEFINE_QUERY(&m_db, CSPSaveMonthRank)
	query->m_bMonth = m_bRankMonth;

	for(BYTE i = 0; i < COUNTRY_COUNT; i++)
	{
		for(BYTE j = 0; j < FIRSTGRADEGROUPCOUNT; j++)
		{
			pBUF->m_packet
				>> query->m_bMonthRank;

			stMONTHRANKER.WrapPacketOut( &(pBUF->m_packet));
			if(!stMONTHRANKER.m_dwCharID || (j!=0 && !stMONTHRANKER.m_dwMonthPoint))
				continue;

			if(query->m_bMonthRank == 0 )
				dwTopPoint = stMONTHRANKER.m_dwTotalPoint;

			query->m_bRank = j;
			query->m_dwChaID = stMONTHRANKER.m_dwCharID;
			lstrcpy(query->m_szName,stMONTHRANKER.m_strName);
			query->m_dwTotalPoint = stMONTHRANKER.m_dwTotalPoint;
			query->m_dwMonthPoint = stMONTHRANKER.m_dwMonthPoint;
			query->m_wMonthWin = stMONTHRANKER.m_wMonthWin;
			query->m_wMonthLose = stMONTHRANKER.m_wMonthLose;
			query->m_dwTotalWin = stMONTHRANKER.m_dwTotalWin;
			query->m_dwTotalLose =  stMONTHRANKER.m_dwTotalLose;
			query->m_bCountry = stMONTHRANKER.m_bCountry;
			query->m_bLevel = stMONTHRANKER.m_bLevel;
			query->m_bClass = stMONTHRANKER.m_bClass;
			query->m_bRace = stMONTHRANKER.m_bRace;
			query->m_bSex = stMONTHRANKER.m_bSex;
			query->m_bHair = stMONTHRANKER.m_bHair;
			query->m_bFace = stMONTHRANKER.m_bFace;
			lstrcpy(query->m_szSay,stMONTHRANKER.m_strSay);
			lstrcpy(query->m_szGuild,stMONTHRANKER.m_strGuild);

			if(!query->Call())
			{
				bSuccess = FALSE;
				break;
			}
		}
	}
	UNDEFINE_QUERY()

	if(bSuccess)
	{
		DEFINE_QUERY(&m_db, CSPInitMonthRank)
		query->m_bMonth = m_bRankMonth+1;
		if(query->m_bMonth > 12)
			query->m_bMonth -= 12;

		query->Call();
		UNDEFINE_QUERY()

		DEFINE_QUERY(&m_db, CSPInitMonthPvPoint)
		query->m_bMonth = m_bRankMonth;
		query->m_dwMTotalPoint = dwTopPoint;

		if(query->Call() && query->m_nRET)
		{
			bChangeTopRanker = TRUE;
			stTOPRANKER.m_dwCharID = query->m_dwCharID;
			stTOPRANKER.m_strName = query->m_szName;
			stTOPRANKER.m_dwTotalPoint = query->m_dwTotalPoint;
			stTOPRANKER.m_dwMonthPoint = query->m_dwMonthPoint;
			stTOPRANKER.m_wMonthWin = query->m_wMonthWin;
			stTOPRANKER.m_wMonthLose = query->m_wMonthLose;
			stTOPRANKER.m_dwTotalWin = query->m_dwTotalWin;
			stTOPRANKER.m_dwTotalLose = query->m_dwTotalLose;
			stTOPRANKER.m_bCountry = query->m_bCountry;
			stTOPRANKER.m_bLevel = query->m_bLevel;
			stTOPRANKER.m_bClass = query->m_bClass;
			stTOPRANKER.m_bRace = query->m_bRace;
			stTOPRANKER.m_bSex = query->m_bSex;
			stTOPRANKER.m_bHair = query->m_bHair;
			stTOPRANKER.m_bFace = query->m_bFace;
			stTOPRANKER.m_strGuild = query->m_szGuild;
		}	


		UNDEFINE_QUERY()
	}

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_MONTHRANKSAVE_ACK)
		<< bSuccess
		<< bChangeTopRanker;

	stTOPRANKER.WrapPacketIn(&(pMSG->m_packet));

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_MONTHRANKSAVE_ACK(LPPACKETBUF pBUF)
{		
	BYTE bRet;
	BYTE bChangeTopRanker;
	MONTHRANKER stTOPRANKER;


	pBUF->m_packet
		>> bRet;

	if(!bRet)
	{
		LogEvent("MONTHRANKSAVE FAILED");
		return EC_NOERROR;
	}
	pBUF->m_packet
		>> bChangeTopRanker;

	if(bChangeTopRanker)
	{
		stTOPRANKER.WrapPacketOut(&(pBUF->m_packet));
		m_arLastFameRank[0] = stTOPRANKER;
	}

	//////////////////////////////////////////////////////////////////////////
	//1군 선수등록
	
	for(BYTE i = 0; i < COUNTRY_COUNT; i++)
		for(BYTE j = 0; j < FIRSTGRADEGROUPCOUNT; j++)
			m_arFirstGradeGroup[i][j] = m_arMonthRank[i][j];
	
	//////////////////////////////////////////////////////////////////////////


	MAPTSERVER::iterator itSvr;
	for(itSvr = m_mapSERVER.begin(); itSvr != m_mapSERVER.end(); itSvr++)
	{
		(*itSvr).second->SendMW_MONTHRANKRESET_REQ(m_arLastFameRank,m_bRankMonth);
		(*itSvr).second->SendMW_FIRSTGRADEGROUP_REQ(m_arFirstGradeGroup,m_bRankMonth);
	}

	MonthRankReset();

	m_bRankMonth++;
	if(m_bRankMonth > 12)
		m_bRankMonth -= 12;

	m_bFameRankSave = FALSE;	

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_FAMERANKUPDATE_ACK(LPPACKETBUF pBUF)
{
	CTServer *pServer = (CTServer *) pBUF->m_pSESSION;

	MAPTSERVER::iterator it;
	for(it = m_mapSERVER.begin(); it != m_mapSERVER.end(); it++)
		(*it).second->SendMW_FAMERANKUPDATE_REQ(pBUF);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_MONTHRANKUPDATE_ACK(LPPACKETBUF pBUF)
{
	BYTE bMonth;	
	BYTE bCountry;
	BYTE bNewRank;
	BYTE bOldRank;
	BYTE bStartRank;
	BYTE bEndRank;	
	BYTE bNewWarlord = FALSE;
	MONTHRANKER stMONTHRANKER;
	
	pBUF->m_packet
		>> bMonth
		>> bCountry;

	if(bMonth != m_bRankMonth || bCountry >= COUNTRY_COUNT)
		return EC_NOERROR;

	stMONTHRANKER.WrapPacketOut(&(pBUF->m_packet));

	//누적 1위 재선정
	if(m_arMonthRank[bCountry][0].m_dwTotalPoint < stMONTHRANKER.m_dwTotalPoint ||
		m_arMonthRank[bCountry][0].m_dwCharID == stMONTHRANKER.m_dwCharID)
	{
		m_arMonthRank[bCountry][0] = stMONTHRANKER;
		bNewWarlord = TRUE;

		for(BYTE w =1 ; w< MONTHRANKCOUNT; w++)	// 누적 1위가 명점이 깎였을 경우 순위 재선정
			if(m_arMonthRank[bCountry][w].m_dwTotalPoint > m_arMonthRank[bCountry][0].m_dwTotalPoint)			
				m_arMonthRank[bCountry][0] = m_arMonthRank[bCountry][w];		
	}

	bOldRank = 0;
	for(BYTE b = 1; b < MONTHRANKCOUNT; b++)
	{
		if(m_arMonthRank[bCountry][b].m_dwCharID == stMONTHRANKER.m_dwCharID)
		{
			bOldRank = b;
			break;
		}
	}
	
	bNewRank = 0;
	for(BYTE i = 1; i < MONTHRANKCOUNT; i++)	
	{
		if(m_arMonthRank[bCountry][i].m_dwMonthPoint <= stMONTHRANKER.m_dwMonthPoint)
		{			
			if( m_arMonthRank[bCountry][i].m_dwMonthPoint < stMONTHRANKER.m_dwMonthPoint)
				bNewRank = i;			
			else
			{
				if(m_arMonthRank[bCountry][i].m_wMonthWin < stMONTHRANKER.m_wMonthWin)
					bNewRank = i;
				else if(m_arMonthRank[bCountry][i].m_wMonthWin > stMONTHRANKER.m_wMonthWin)
					bNewRank = i+1;
				else
				{
					if(m_arMonthRank[bCountry][i].m_wMonthLose < stMONTHRANKER.m_wMonthLose)
						bNewRank = i;
					else if(m_arMonthRank[bCountry][i].m_wMonthLose > stMONTHRANKER.m_wMonthLose)
						bNewRank = i+1;
					else
                        bNewRank = m_arMonthRank[bCountry][i].m_dwCharID < stMONTHRANKER.m_dwCharID ? i : i+1;
				}				
			}
			break;
		}
	}

	if(bOldRank || bNewRank)
	{
		if(!bOldRank)
			bOldRank = MONTHRANKCOUNT - 1;
		else if(!bNewRank)
			bNewRank = MONTHRANKCOUNT;
	}
	else
		return EC_NOERROR;

	if(bOldRank == bNewRank)
	{
		m_arMonthRank[bCountry][bNewRank] = stMONTHRANKER;
		bStartRank = bEndRank = bNewRank;
	}
	else if( bOldRank < bNewRank)
	{		
		for(BYTE j = bOldRank  ; j < bNewRank-1 ; j++)	
			m_arMonthRank[bCountry][j] = m_arMonthRank[bCountry][j+1];	
		m_arMonthRank[bCountry][bNewRank-1] = stMONTHRANKER;

		bStartRank = bOldRank;
		bEndRank = bNewRank-1;
	}
	else
	{
		for(BYTE j = bOldRank  ; j > bNewRank ; j--)	
			m_arMonthRank[bCountry][j] = m_arMonthRank[bCountry][j-1];	
		m_arMonthRank[bCountry][bNewRank] = stMONTHRANKER;

		bStartRank = bNewRank;
		bEndRank = bOldRank;
	}
	
	//누적 1위 재선정
	if(m_arMonthRank[bCountry][0].m_dwTotalPoint < stMONTHRANKER.m_dwTotalPoint ||
		m_arMonthRank[bCountry][0].m_dwCharID == stMONTHRANKER.m_dwCharID)
	{
		m_arMonthRank[bCountry][0] = stMONTHRANKER;
		bNewWarlord = TRUE;

		for(BYTE w =1 ; w< MONTHRANKCOUNT; w++)		// 누적 1위가 명점이 깎였을 경우 순위 재선정
			if(m_arMonthRank[bCountry][w].m_dwTotalPoint > m_arMonthRank[bCountry][0].m_dwTotalPoint)			
				m_arMonthRank[bCountry][0] = m_arMonthRank[bCountry][w];		
	}

	MAPTSERVER::iterator it;
	for(it = m_mapSERVER.begin(); it != m_mapSERVER.end(); it++)	
		(*it).second->SendMW_MONTHRANKUPDATE_REQ(m_bRankMonth,bCountry,bStartRank,bEndRank,m_arMonthRank,bNewWarlord);
	
	return EC_NOERROR;
		
}

DWORD CTWorldSvrModule::OnMW_WARLORDSAY_ACK(LPPACKETBUF pBUF)
{
	BYTE bType;
	BYTE bRankMonth;
	DWORD dwCharID;
	CString strSay;

	pBUF->m_packet
		>> bType
		>> bRankMonth
		>> dwCharID
		>> strSay;

	MAPTSERVER::iterator itSvr;
	for(itSvr = m_mapSERVER.begin(); itSvr != m_mapSERVER.end(); itSvr++)
		(*itSvr).second->SendMW_WARLORDSAY_REQ(bType,bRankMonth,dwCharID,strSay);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnSM_TOURNAMENT_REQ(LPPACKETBUF pBUF)
{
	WORD wTournamentID;
	BYTE bGroup;
	BYTE bStep;
	DWORD dwPeriod;

	pBUF->m_packet
		>> wTournamentID
		>> bGroup
		>> bStep
		>> dwPeriod;

	MAPTOURNAMENT::iterator it = m_mapTournament.find(wTournamentID);
	if(it == m_mapTournament.end() || (*it).second.empty())
		return EC_NOERROR;

	if(m_tournament.m_bGroup != bGroup)
	{
		m_tournament.m_bGroup = bGroup;
		m_tournament.m_dwSum = 0;

		BYTE bEC = TournamentGetEntryCount();
		if(bEC)
			m_tournament.m_bBase = BYTE(TOURNAMENT_BASEPRIZE / bEC);
		else
			m_tournament.m_bBase = 0;
	}

	m_tournament.m_bStep = bStep;

	INT64 dlNextStep = 0;
	MAPTSERVER::iterator itSvr;
	MAPTOURNAMENTSTEP::iterator itSt = m_tournament.m_mapStep.find(MAKEWORD(bStep+1, bGroup));
	if(itSt!=m_tournament.m_mapStep.end())
		dlNextStep = itSt->second.m_dStart;

	for(itSvr = m_mapSERVER.begin(); itSvr != m_mapSERVER.end(); itSvr++)
		(*itSvr).second->SendMW_TOURNAMENTENABLE_REQ(bGroup, bStep, dwPeriod, dlNextStep);

	if(!m_tournament.m_bSelected &&
		(bStep == TNMTSTEP_PARTY || bStep == TNMTSTEP_MATCH))
		TournamentSelectPlayer();

	if(bStep == TNMTSTEP_MATCH)
	{
		for(BYTE i=0; i<=bGroup; i++)
		{
			itSt = m_tournament.m_mapStep.find(MAKEWORD(bStep, i));
			if(itSt!=m_tournament.m_mapStep.end())
			{
				if(bGroup == itSt->second.m_bGroup)
					TournamentMatch();

				break;
			}
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnSM_TOURNAMENTUPDATE_REQ(LPPACKETBUF pBUF)
{
	WORD wTournamentID;
	BYTE bCount;

	pBUF->m_packet
		>> wTournamentID
		>> bCount;

	TournamentClear();

	LPMAPTOURNAMENTENTRY pEntry = GetTournament(wTournamentID);
	if(pEntry)
	{
		m_tournament.m_wID = wTournamentID;
		m_tournament.m_bGroup = 0;
		m_tournament.m_mapEntry = pEntry;
		m_tournament.m_mapStep.clear();
	}
	else
	{
		m_tournament.m_wID = 0;
		m_tournament.m_mapEntry = NULL;
		m_tournament.m_mapStep.clear();
		TournamentInfo();

		return EC_NOERROR;
	}

	for(BYTE i=0; i<bCount; i++)
	{
		TOURNAMENTSTEP step;
		pBUF->m_packet
			>> step.m_bGroup
			>> step.m_bStep
			>> step.m_dwPeriod
			>> step.m_dStart;

		m_tournament.m_mapStep.insert(MAPTOURNAMENTSTEP::value_type(MAKEWORD(step.m_bStep, step.m_bGroup), step));
	}

	BYTE bEC = TournamentGetEntryCount();
	if(bEC)
		m_tournament.m_bBase = BYTE(TOURNAMENT_BASEPRIZE / bEC);
	else
		m_tournament.m_bBase = 0;

	TournamentInfo();

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_TOURNAMENT_ACK(LPPACKETBUF pBUF)
{
	CTServer *pServer = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	WORD wProtocol;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wProtocol;

	LPTCHARACTER pChar = FindTChar(
		dwCharID,
		dwKey);

    if(!pChar)
		return EC_NOERROR;

	switch(wProtocol)
	{
	case MW_TOURNAMENTSCHEDULE_ACK:
		TournamentSchedule(pChar, pServer);
		break;
	case MW_TOURNAMENTAPPLYINFO_ACK:
		TournamentApplyInfo(pChar, pServer);
		break;
	case MW_TOURNAMENTAPPLY_ACK:
		{
			BYTE bEntryID;
			pBUF->m_packet
				>> bEntryID;

			TournamentApply(pChar, pServer, bEntryID);
		}
		break;
	case MW_TOURNAMENTJOINLIST_ACK:
		TournamentJoinList(pChar, pServer);
		break;
	case MW_TOURNAMENTPARTYLIST_ACK:
		{
			DWORD dwChiefID;
			pBUF->m_packet
				>> dwChiefID;
			
			TournamentPartyList(pChar, pServer, dwChiefID);
		}
		break;
	case MW_TOURNAMENTPARTYADD_ACK:
		{
			CString strTarget;
			pBUF->m_packet
				>> strTarget;

			LPTCHARACTER pTarget = FindTChar(strTarget);
			if(pTarget)
				TournamentPartyAdd(pChar, pServer, pTarget->m_dwCharID, pTarget->m_bCountry, pTarget->m_strNAME, pTarget->m_bLevel, pTarget->m_bClass);
			else
				SendDM_GETCHARINFO_REQ(dwCharID, dwKey, MW_TOURNAMENTPARTYADD_ACK, strTarget);
		}
		break;
	case MW_TOURNAMENTPARTYDEL_ACK:
		{
			DWORD dwTarget;
			pBUF->m_packet
				>> dwTarget;

			TournamentPartyDel(pChar, pServer, dwTarget);
		}
		break;
	case MW_TOURNAMENTMATCHLIST_ACK:
		TournamentMatchList(pChar, pServer);
		break;
	case MW_TOURNAMENTEVENTLIST_ACK:
		TournamentEventList(pChar, pServer);
		break;
	case MW_TOURNAMENTEVENTINFO_ACK:
		{
			BYTE bEntryID;
			pBUF->m_packet
				>> bEntryID;

			TournamentEventInfo(pChar, pServer, bEntryID);
		}
		break;
	case MW_TOURNAMENTEVENTJOIN_ACK:
		{
			BYTE bEntryID;
			DWORD dwTargetID;

			pBUF->m_packet
				>> bEntryID
				>> dwTargetID;

			TournamentEventJoin(pChar, pServer, bEntryID, dwTargetID);
		}
		break;
	default:
		break;
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GETCHARINFO_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	CString strTarget;
	WORD wProtocol;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wProtocol
		>> strTarget;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GETCHARINFO_ACK)
		<< dwCharID
		<< dwKey
		<< wProtocol;

	BYTE bGet = 0;
	DEFINE_QUERY(&m_db, CTBLGetCharInfo)
	lstrcpy(query->m_szFindName, strTarget);
	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwCharID
				<< query->m_bCountry
				<< query->m_szName
				<< query->m_bLevel
				<< query->m_bClass;

			bGet++;
		}
		query->Close();

	}
	UNDEFINE_QUERY()

	if(bGet>1)
	{
		delete pMSG;
		return EC_NOERROR;

	}
	else if(!bGet)
	{
		pMSG->m_packet
			<< DWORD(0);

	}

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_GETCHARINFO_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTargetID;
	BYTE bCountry;
	CString strTarget;
	BYTE bLevel;
	WORD wProtocol;
	BYTE bClass;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wProtocol
		>> dwTargetID;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar)
		return EC_NOERROR;

	pBUF->m_packet
		>> bCountry
		>> strTarget
		>> bLevel
		>> bClass;

	if(wProtocol == MW_TOURNAMENTPARTYADD_ACK)
	{
		CTServer * pServer = FindMapSvr(pChar->m_bMainID);
		if(!pServer)
			return EC_NOERROR;

		if(dwTargetID)
			TournamentPartyAdd(pChar, pServer, dwTargetID, bCountry, strTarget, bLevel, bClass);
		else
			pServer->SendMW_TOURNAMENT_REQ(pChar->m_dwCharID, pChar->m_dwKEY, MW_TOURNAMENTPARTYADD_REQ, TOURNAMENT_NOTFOUND);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_TOURNAMENTEVENTCHARINFO_REQ(LPPACKETBUF pBUF)
{
	DWORD dwMID;
	BYTE bType;
	WORD wTID;
	BYTE bEntry;
	CString strTarget;

	pBUF->m_packet
		>> dwMID
		>> bType
		>> wTID
		>> bEntry
		>> strTarget;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(SM_TOURNAMENTEVENT_ACK)
		<< dwMID
		<< bType
		<< wTID
		<< bEntry;

	BYTE bGet = 0;
	DEFINE_QUERY(&m_db, CTBLGetCharInfo)
	lstrcpy(query->m_szFindName, strTarget);
	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwCharID
				<< query->m_bCountry
				<< query->m_szName
				<< query->m_bLevel
				<< query->m_bClass;

			bGet++;
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	if(bGet>1)
	{
		delete pMSG;
		return EC_NOERROR;
	}
	else if(!bGet)
	{
		pMSG->m_packet
			<< DWORD(0);
	}

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_TOURNAMENTENTERGATE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwMoney;
	BYTE bEnter;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwMoney
		>> bEnter;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar)
		return EC_NOERROR;

	TNMTEnterGate(pChar, dwMoney, bEnter);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_TOURNAMENTRESULT_ACK(LPPACKETBUF pBUF)
{
	BYTE bStep;
	BYTE bRet;
	DWORD dwWin;
	DWORD dwLose;
	DWORD dwBlueHideTick;
	DWORD dwRedHideTick;

	pBUF->m_packet
		>> bStep
		>> bRet
		>> dwWin
		>> dwLose
		>> dwBlueHideTick
		>> dwRedHideTick;

	if(bStep != TNMTSTEP_QFINAL &&
		bStep != TNMTSTEP_SFINAL &&
		bStep != TNMTSTEP_FINAL)
		return EC_NOERROR; 

	LPTNMTPLAYER pWin = FindTNMTPlayer(dwWin);
	LPTNMTPLAYER pLose = FindTNMTPlayer(dwLose);

	VECTORDWORD vPlayer;
	vPlayer.clear();
	MAPTNMTPLAYER::iterator itTP;

	BYTE bID;
	if(bStep == TNMTSTEP_FINAL)	bID = MATCH_FINAL;
	else if(bStep == TNMTSTEP_SFINAL) bID = MATCH_SFINAL;
	else bID = MATCH_QFINAL;

	if(pWin)
	{
		for(itTP=pWin->m_mapParty.begin(); itTP!=pWin->m_mapParty.end(); itTP++)
		{
			(*itTP).second->m_bResult[bID] = bRet ? TNMTWIN_WIN : TNMTWIN_LOSE;
			vPlayer.push_back((*itTP).second->m_dwCharID);
		}

		pWin->m_bResult[bID] = bRet ? TNMTWIN_WIN : TNMTWIN_LOSE;
	}
	if(pLose)
	{
		for(itTP=pLose->m_mapParty.begin(); itTP!=pLose->m_mapParty.end(); itTP++)
		{
			(*itTP).second->m_bResult[bID] = TNMTWIN_LOSE;
			vPlayer.push_back((*itTP).second->m_dwCharID);
		}

		pLose->m_bResult[bID] = TNMTWIN_LOSE;
	}

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it!=m_mapSERVER.end(); it++)
		(*it).second->SendMW_TOURNAMENTRESULT_REQ(m_tournament.m_wID, bStep, bRet, dwWin, dwLose, dwBlueHideTick, dwRedHideTick, vPlayer);

	if(bStep == TNMTSTEP_FINAL &&
		pWin && bRet)
	{
		MAPDWORD::iterator itBt;
		for(itBt=pWin->m_mapBatting.begin(); itBt != pWin->m_mapBatting.end(); itBt++)
		{
			MAPTCHARACTER::iterator itC = m_mapTCHAR.find((*itBt).first);
			if(itC!=m_mapTCHAR.end())
			{
				FLOAT fRate;
				DWORD dwAmount;
				GetBattingAmount(pWin, (*itBt).first, fRate, dwAmount);

				if(dwAmount)
				{
					CTServer * pMain = FindMapSvr((*itC).second->m_bMainID);
					if(pMain)
						pMain->SendMW_TOURNAMENTBATPOINT_REQ((*itBt).first, (*itC).second->m_strNAME, dwAmount);
				}
			}
		}
	}

	SendDM_TOURNAMENTRESULT_REQ(
		bStep,
		bRet,
		dwWin,
		dwLose);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_TOURNAMENTPAYBACK_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwBack;
	DWORD dwPostID = 0;

	pBUF->m_packet
		>> dwCharID
		>> dwBack;

	DWORD dwGold, dwSilver, dwCooper;
	CalcMoney(dwBack, dwGold, dwSilver, dwCooper);

	DEFINE_QUERY(&m_db, CSPTournamentPayback)
	query->m_dwCharID = dwCharID;
	query->m_dwGold = dwGold;
	query->m_dwSilver = dwSilver;
	query->m_dwCooper = dwCooper;
	if(query->Call())
		dwPostID = query->m_dwPostID;
	UNDEFINE_QUERY()

	if(dwPostID)
	{
		LPPACKETBUF  pMsg = new PACKETBUF();
		pMsg->m_packet.SetID(DM_TOURNAMENTPAYBACK_ACK)
			<< dwPostID
			<< dwCharID
			<< dwGold
			<< dwSilver
			<< dwCooper;

		SayToBATCH(pMsg);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_TOURNAMENTPAYBACK_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;
	DWORD dwPostID = 0;

	pBUF->m_packet
		>> dwPostID
		>> dwCharID
		>> dwGold
		>> dwSilver
		>> dwCooper;

	if(!dwPostID)
		return EC_NOERROR;

	MAPTCHARACTER::iterator it = m_mapTCHAR.find(dwCharID);
	if(it==m_mapTCHAR.end())
		return EC_NOERROR;

	CTServer * pServer = FindMapSvr((*it).second->m_bMainID);
	if(pServer)
	{
		CPacket * pMsg = new CPacket();
		(*pMsg)
			<< dwPostID
			<< DWORD(0)
			<< GetSvrMsg(NAME_OPERATOR)
			<< (*it).second->m_strNAME
			<< BuildNetString(NAME_NULL, GetSvrMsg(TNMT_PAYBACK_TITLE))
			<< BuildNetString(NAME_NULL, GetSvrMsg(TNMT_PAYBACK_MESSAGE))
			<< BYTE(POST_PACKATE)
			<< dwGold
			<< dwSilver
			<< dwCooper
			<< BYTE(0);

		pServer->SendMW_POSTRECV_REQ(pMsg);
		delete pMsg;
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_TOURNAMENTRESULT_REQ(LPPACKETBUF pBUF)
{
	BYTE bStep;
	BYTE bRet;
	DWORD dwWin;
	DWORD dwLose;

	pBUF->m_packet
		>> bStep
		>> bRet
		>> dwWin
		>> dwLose;

	DEFINE_QUERY(&m_db, CSPTournamentResult)
	query->m_bStep = bStep;
	query->m_bRet = bRet;
	query->m_dwWin = dwWin;
	query->m_dwLose = dwLose;
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_TOURNAMENTAPPLY_REQ(LPPACKETBUF pBUF)
{
	BYTE bAdd;
	DWORD dwCharID;
	BYTE bEntry;
	DWORD dwChiefID;

	pBUF->m_packet
		>> bAdd
		>> dwCharID
		>> bEntry
		>> dwChiefID;

	DEFINE_QUERY(&m_db, CSPTournamentApply)
	query->m_bAdd = bAdd;
	query->m_dwCharID = dwCharID;
	query->m_bEntry = bEntry;
	query->m_dwChiefID = dwChiefID;
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_TOURNAMENTCLEAR_REQ(LPPACKETBUF pBUF)
{
	DEFINE_QUERY(&m_db, CSPTournamentClear)
	query->m_bClear = TRUE;
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_TOURNAMENTSTATUS_REQ(LPPACKETBUF pBUF)
{
	WORD wID;
	BYTE bGroup;
	BYTE bStep;

	pBUF->m_packet
		>> wID
		>> bGroup
		>> bStep;

	DEFINE_QUERY(&m_db, CSPTournamentStatus)
	query->m_wID = wID;
	query->m_bGroup = bGroup;
	query->m_bStep = bStep;
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_TOURNAMENTEVENT_REQ(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwMgID;
	BYTE bType;
	pBUF->m_packet
		>> dwMgID
		>> bType;

	switch(bType)
	{
	case TET_LIST:
	case TET_SCHEDULEADD:
	case TET_SCHEDULEDEL:
		{
			LPPACKETBUF pMSG = new PACKETBUF();
			pMSG->m_packet.Copy(&pBUF->m_packet);
			pMSG->m_packet.SetID(SM_TOURNAMENTEVENT_REQ);
			SayToTIMER(pMSG);
		}
		break;
	case TET_ENTRYADD:
		{
			WORD wTID;
			BYTE bCount;
			pBUF->m_packet
				>> wTID
				>> bCount;

			MAPTOURNAMENTENTRY mapTNMTE;
			mapTNMTE.clear();
			BYTE bRewardCount;
			for(BYTE i=0; i<bCount; i++)
			{
				LPTOURNAMENTENTRY pTour = new TOURNAMENTENTRY();

				pBUF->m_packet
					>> pTour->m_bEntryID
					>> pTour->m_strName
					>> pTour->m_bType
					>> pTour->m_dwClass
					>> pTour->m_dwFee
					>> pTour->m_dwFeeBack
					>> pTour->m_wPermitItemID
					>> pTour->m_bPermitCount
					>> pTour->m_bMinLevel
					>> pTour->m_bMaxLevel
					>> bRewardCount;

				for(BYTE j=0; j<bRewardCount; j++)
				{
					TNMTREWARD reward;
					pBUF->m_packet
						>> reward.m_bChartType
						>> reward.m_wItemID
						>> reward.m_bCount
						>> reward.m_dwClass
						>> reward.m_bCheckShield;

					pTour->m_vReward.push_back(reward);
				}

				pTour->m_map1st.clear();
				pTour->m_mapNormal.clear();
				pTour->m_mapPlayer.clear();

				mapTNMTE.insert(MAPTOURNAMENTENTRY::value_type(pTour->m_bEntryID, pTour));
			}

			MAPTOURNAMENT::iterator itT = m_mapTournament.find(wTID);
			if(itT != m_mapTournament.end())
			{
				while(!(*itT).second.empty())
					TNMTEntryDelete(wTID, (*(*itT).second.begin()).second->m_bEntryID);

				m_mapTournament.erase(itT);
			}

			m_mapTournament.insert(MAPTOURNAMENT::value_type(wTID, mapTNMTE));

			LPPACKETBUF pMSG = new PACKETBUF();
			pMSG->m_packet.SetID(SM_TOURNAMENTEVENT_REQ)
				<< dwMgID
				<< bType
				<< wTID;

			SayToTIMER(pMSG);

			SendDM_TNMTEVENTENTRYADD_REQ(&pBUF->m_packet);
		}
		break;
	case TET_ENTRYDEL:
		{
			WORD wTID;
			BYTE bEntryID;

			pBUF->m_packet
				>> wTID
				>> bEntryID;

			TNMTEntryDelete(wTID, bEntryID);
		}
		break;
	case TET_PLAYERADD:
		{
			WORD wTourID;
			BYTE bEntryID;

			pBUF->m_packet
				>> wTourID
				>> bEntryID;

			BYTE bError = FALSE;
			if(wTourID != m_tournament.m_wID ||
				m_tournament.m_bStep >= TNMTSTEP_ENTER)
				bError = TRUE;

			LPTOURNAMENTENTRY pEt = GetCurrentTournamentEntry(bEntryID);
			if(!pEt)
				bError = TRUE;

			if(bError)
			{
				CPacket * pMSG = new CPacket();
				pMSG->SetID(CT_TOURNAMENTEVENT_ACK)
					<< dwMgID
					<< bType
					<< bEntryID
					<< DWORD(0)
					<< NAME_NULL
					<< BYTE(0)
					<< BYTE(TCLASS_COUNT)
					<< BYTE(TCONTRY_N);

				if(m_pCtrlSvr)
					m_pCtrlSvr->Say(pMSG);
				else
					delete pMSG;

				return EC_NOERROR;
			}

			LPPACKETBUF pMSG = new PACKETBUF();
			pMSG->m_packet.Copy(&pBUF->m_packet);
			pMSG->m_packet.SetID(DM_TOURNAMENTEVENTCHARINFO_REQ);
			SayToDB(pMSG);
		}
		break;
	case TET_PLAYERDEL:
		{
			WORD wTID;
			BYTE bEntry;
			CString strTarget;
			pBUF->m_packet
				>> wTID
				>> bEntry
				>> strTarget;

			DWORD dwCharID = 0;

			if(wTID == m_tournament.m_wID)
			{
				LPTOURNAMENTENTRY pEt = GetCurrentTournamentEntry(bEntry);
	
				if(pEt)
				{
					MAPTNMTPLAYER::iterator it;
					for(it=m_mapTNMTPlayer.begin(); it!=m_mapTNMTPlayer.end(); it++)
					{
						if((*it).second->m_strName == strTarget)
						{
							dwCharID = (*it).second->m_dwCharID;
							SendDM_TOURNAMENTAPPLY_REQ(FALSE, (*it).second->m_dwCharID);
							DelTNMTPlayer(pEt, (*it).second);
							break;
						}
					}
				}
			}

			CPacket * pMSG = new CPacket();
			pMSG->SetID(CT_TOURNAMENTEVENT_ACK)
				<< dwMgID
				<< bType
				<< bEntry
				<< dwCharID;

			if(m_pCtrlSvr)
				m_pCtrlSvr->Say(pMSG);
			else
				delete pMSG;
		}
		break;
	case TET_PLAYEREND:
		{
			if(m_tournament.m_bStep >= TNMTSTEP_PARTY)
				TournamentSelectPlayer();

			if(m_tournament.m_bStep >= TNMTSTEP_MATCH)
				TournamentMatch();

			CPacket * pMSG = new CPacket();
			pMSG->SetID(CT_TOURNAMENTEVENT_ACK)
				<< dwMgID
				<< bType;

			if(m_pCtrlSvr)
				m_pCtrlSvr->Say(pMSG);
			else
				delete pMSG;
		}
		break;
	default:
		return EC_NOERROR;
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnSM_TOURNAMENTEVENT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwMgID;
	BYTE bType;
	pBUF->m_packet
		>> dwMgID
		>> bType;

	switch(bType)
	{
	case TET_LIST:
		{
			LPPACKETBUF pMSG = new PACKETBUF();
			pMSG->m_packet.SetID(SM_TOURNAMENTEVENT_ACK)
				<< dwMgID
				<< bType
				<< (BYTE)m_mapTournamentTime.size();

			MAPTOURNAMENTTIME::iterator itTT;
			for(itTT=m_mapTournamentTime.begin(); itTT!=m_mapTournamentTime.end(); itTT++)
			{
				WORD wTID = WORD((*itTT).first);
				LPTBATTLETIME bt = &((*itTT).second);
				pMSG->m_packet
					<< wTID
					<< bt->m_bWeek
					<< bt->m_bDay
					<< bt->m_dwBattleStart;

				MAPTOURNAMENTSCHEDULE::iterator itTS = m_mapTournamentSchedule.find(wTID);
				if(itTS != m_mapTournamentSchedule.end())
				{
					LPMAPTOURNAMENTSTEP pTournament = (*itTS).second.m_mapStep;
					MAPTOURNAMENTSTEP::iterator itTStep;

					pMSG->m_packet
						<< (BYTE)pTournament->size();

					for(itTStep=pTournament->begin(); itTStep!=pTournament->end(); itTStep++)
					{
						LPTOURNAMENTSTEP pStep = &((*itTStep).second);
						pMSG->m_packet
							<< pStep->m_bStep
							<< pStep->m_dwPeriod
							<< pStep->m_dStart;
					}
				}
				else
				{
					pMSG->m_packet
						<< (BYTE)0;
				}
			}

			SayToBATCH(pMSG);
		}
		break;
	case TET_SCHEDULEADD:
		{
			WORD wTID;
			TBATTLETIME bt;
			BYTE bCount;
			pBUF->m_packet
				>> wTID
				>> bt.m_bWeek
				>> bt.m_bDay
				>> bt.m_dwBattleStart
				>> bCount;

			LPMAPTOURNAMENTSTEP pTS = NULL;

			MAPTOURNAMENTSCHEDULE::iterator it = m_mapTournamentSchedule.find(wTID);
			if(it!=m_mapTournamentSchedule.end())
			{
				pTS = (*it).second.m_mapStep;
				m_mapTournamentTime.erase(wTID);
				m_mapTournamentTime.insert(MAPTOURNAMENTTIME::value_type(wTID, bt));
			}
			else
				pTS = new MAPTOURNAMENTSTEP();

			pTS->clear();

			for(BYTE i=0; i<bCount; i++)
			{
				TOURNAMENTSTEP tour;
				pBUF->m_packet
					>> tour.m_bStep
					>> tour.m_dwPeriod;

				pTS->insert(MAPTOURNAMENTSTEP::value_type(tour.m_bStep, tour));
			}

			WORD wNewID = SetTournamentTime(pTS, bt, wTID, FALSE, FALSE);
			if(wTID)
				TournamentUpdate(wNewID);

			LPPACKETBUF pMSG = new PACKETBUF();
			pMSG->m_packet.SetID(SM_TOURNAMENTEVENT_ACK)
				<< dwMgID
				<< bType
				<< wNewID;

			SayToBATCH(pMSG);

			if(wNewID)
				SendDM_TNMTEVENTSCHEDULEADD_REQ(&pBUF->m_packet, wNewID);
		}
		break;
	case TET_SCHEDULEDEL:
		{
			WORD wTID;
			pBUF->m_packet
				>> wTID;

			DelTournamentSchedule(wTID);

			LPPACKETBUF pMSG = new PACKETBUF();
			pMSG->m_packet.SetID(SM_TOURNAMENTEVENT_ACK)
				<< dwMgID
				<< bType
				<< wTID;

			SayToBATCH(pMSG);
		}
		break;
	case TET_ENTRYADD:
		{
			WORD wTID;
			pBUF->m_packet
				>> wTID;

			MAPTOURNAMENTSCHEDULE::iterator it = m_mapTournamentSchedule.find(wTID);
			if(it!=m_mapTournamentSchedule.end())
				TournamentUpdate(wTID);

			LPPACKETBUF pMSG = new PACKETBUF();
			pMSG->m_packet.SetID(SM_TOURNAMENTEVENT_ACK)
				<< dwMgID
				<< bType;

			SayToBATCH(pMSG);
		}
		break;
	default:
		return EC_NOERROR;
	}

	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnSM_TOURNAMENTEVENT_ACK(LPPACKETBUF pBUF)
{
	DWORD dwMgID;
	BYTE bType;
	pBUF->m_packet
		>> dwMgID
		>> bType;

	switch(bType)
	{
	case TET_LIST:
		{
			CPacket * pMSG = new CPacket();
			pMSG->Copy(&pBUF->m_packet);
			pMSG->SetID(CT_TOURNAMENTEVENT_ACK)
				<< (BYTE)m_mapTournament.size();

			MAPTOURNAMENT::iterator it;
			for(it=m_mapTournament.begin(); it!=m_mapTournament.end(); it++)
			{
				LPMAPTOURNAMENTENTRY entry = &(*it).second;
				(*pMSG)
					<< (*it).first
					<< (BYTE)entry->size();

				MAPTOURNAMENTENTRY::iterator itEr;
				for(itEr=entry->begin(); itEr!=entry->end(); itEr++)
				{
					LPTOURNAMENTENTRY pEntry = (*itEr).second;
					(*pMSG)
						<< pEntry->m_bEntryID
						<< pEntry->m_strName
						<< pEntry->m_bType
						<< pEntry->m_dwClass
						<< pEntry->m_dwFee
						<< pEntry->m_dwFeeBack
						<< pEntry->m_wPermitItemID
						<< pEntry->m_bPermitCount
						<< pEntry->m_bMinLevel
						<< pEntry->m_bMaxLevel
						<< (BYTE)pEntry->m_vReward.size();

					for(BYTE i=0; i<pEntry->m_vReward.size(); i++)
					{
						(*pMSG)
							<< pEntry->m_vReward[i].m_bChartType
							<< pEntry->m_vReward[i].m_wItemID
							<< pEntry->m_vReward[i].m_bCount
							<< pEntry->m_vReward[i].m_dwClass
							<< pEntry->m_vReward[i].m_bCheckShield;
					}

					if(m_tournament.m_bStep < TNMTSTEP_PARTY)
					{
						(*pMSG)
							<< BYTE(pEntry->m_map1st.size());

						MAPTNMTPLAYER::iterator itTP;
						for(itTP=pEntry->m_map1st.begin(); itTP!=pEntry->m_map1st.end(); itTP++)
						{
							LPTNMTPLAYER pTP = (*itTP).second;
							(*pMSG)
								<< pTP->m_dwCharID
								<< pTP->m_strName
								<< pTP->m_bLevel
								<< pTP->m_bClass
								<< pTP->m_bCountry;
						}
					}
					else
					{
						WORD wSize = pMSG->GetSize();
						BYTE bCount=0;
						(*pMSG)
							<< bCount;

						MAPTNMTPLAYER::iterator itTP;
						for(itTP=pEntry->m_mapPlayer.begin(); itTP!=pEntry->m_mapPlayer.end(); itTP++)
						{
							LPTNMTPLAYER pTP = (*itTP).second;
							if(pTP->m_dwCharID == pTP->m_dwChiefID)
							{
								(*pMSG)
									<< pTP->m_dwCharID
									<< pTP->m_strName
									<< pTP->m_bLevel
									<< pTP->m_bClass
									<< pTP->m_bCountry;

								bCount++;
							}
						}
						memcpy(pMSG->GetBuffer()+wSize,&bCount,sizeof(bCount));
					}
				}
			}

			if(m_pCtrlSvr)
				m_pCtrlSvr->Say(pMSG);
			else
				delete pMSG;
		}
		break;
	case TET_SCHEDULEADD:
		{
			CPacket * pMSG = new CPacket();
			pMSG->Copy(&pBUF->m_packet);
			pMSG->SetID(CT_TOURNAMENTEVENT_ACK);
			if(m_pCtrlSvr)
				m_pCtrlSvr->Say(pMSG);
			else
				delete pMSG;
		}
		break;
	case TET_SCHEDULEDEL:
		{
			WORD wTID;
			pBUF->m_packet
				>> wTID;

			MAPTOURNAMENT::iterator it = m_mapTournament.find(wTID);
			if(it!=m_mapTournament.end())
			{
				MAPTOURNAMENTENTRY::iterator itTNMTE;
				for(itTNMTE=(*it).second.begin(); itTNMTE!=(*it).second.end(); itTNMTE++)
					delete (*itTNMTE).second;

				m_mapTournament.erase(it);
			}

			if(dwMgID)
			{
				CPacket * pMSG = new CPacket();
				pMSG->SetID(CT_TOURNAMENTEVENT_ACK)
					<< dwMgID
					<< bType
					<< wTID;

				if(m_pCtrlSvr)
					m_pCtrlSvr->Say(pMSG);
				else
					delete pMSG;
			}
		}
		break;
	case TET_ENTRYADD:
		{
			CPacket * pMSG = new CPacket();
			pMSG->SetID(CT_TOURNAMENTEVENT_ACK)
				<< dwMgID
				<< bType;

			if(m_pCtrlSvr)
				m_pCtrlSvr->Say(pMSG);
			else
				delete pMSG;
		}
		break;
	case TET_ENTRYDEL:
		break;
	case TET_PLAYERADD:
		{
			WORD wTID;
			BYTE bEntry;
			DWORD dwCharID;
			BYTE bCountry=TCONTRY_N;
			CString strTarget=NAME_NULL;
			BYTE bLevel=0;
			BYTE bClass=TCLASS_COUNT;

			pBUF->m_packet
				>> wTID
				>> bEntry
				>> dwCharID;

			if(dwCharID)
			{
				LPTOURNAMENTENTRY pEt = GetCurrentTournamentEntry(bEntry);

				if(pEt && !FindTNMTPlayer(dwCharID))
				{
					pBUF->m_packet
						>> bCountry
						>> strTarget
						>> bLevel
						>> bClass;

					LPTNMTPLAYER pTarget = new TNMTPLAYER();

					pTarget->m_bEntryID = bEntry;
					pTarget->m_bClass = bClass;
					pTarget->m_bCountry = bCountry;
					pTarget->m_bLevel = bLevel;
					pTarget->m_dwCharID = dwCharID;
					pTarget->m_strName = strTarget;
					pTarget->m_dwChiefID = dwCharID;

					MAPDWORD::iterator itGM = m_mapCharGuild.find(pTarget->m_dwCharID);
					if(itGM != m_mapCharGuild.end())
					{
						CTGuild * pGuild = FindTGuild((*itGM).second);
						if(pGuild)
							pTarget->m_strGuildName = pGuild->m_strName;
					}

					GetRanking(pTarget->m_dwCharID, pTarget->m_dwRank, pTarget->m_dwMonthRank);
					AddTNMTPlayer(pEt, pTarget, TNMTSTEP_1st, pTarget);
					SendDM_TOURNAMENTAPPLY_REQ(TRUE, pTarget->m_dwCharID, bEntry, pTarget->m_dwCharID);
				}
				else
					dwCharID = 0;
			}

			CPacket * pMSG = new CPacket();
			pMSG->SetID(CT_TOURNAMENTEVENT_ACK)
				<< dwMgID
				<< bType
				<< bEntry
				<< dwCharID
				<< strTarget
				<< bLevel
				<< bClass
				<< bCountry;

			if(m_pCtrlSvr)
				m_pCtrlSvr->Say(pMSG);
			else
				delete pMSG;
		}
		break;
	default:
		return EC_NOERROR;
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_TNMTEVENTSCHEDULEADD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwMgID;
	BYTE bType;
	WORD wTID;
	BYTE bWeek;
	BYTE bDay;
	DWORD dwStart;
	BYTE bCount;

	pBUF->m_packet
		>> dwMgID
		>> bType
		>> wTID
		>> bWeek
		>> bDay
		>> dwStart
		>> bCount;

	DEFINE_QUERY(&m_db, CSPTnmtEventTime)
	query->m_wTourID = wTID;
	query->m_bWeek = bWeek;
	query->m_bDay = bDay;
	query->m_dwStart = dwStart;
	query->Call();
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CSPTnmtEventSchedule)
	query->m_wTourID = wTID;
	for(BYTE i=0; i<bCount; i++)
	{
		pBUF->m_packet
			>> query->m_bStep
			>> query->m_dwPeriod;

		query->Call();
	}
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_TNMTEVENTSCHEDULEDEL_REQ(LPPACKETBUF pBUF)
{
	WORD wTID;
	pBUF->m_packet
		>> wTID;

	DEFINE_QUERY(&m_db, CSPTnmtEventDel)
	query->m_wTourID = wTID;
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_TNMTEVENTENTRYADD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwMgID;
	BYTE bType;
	WORD wTID;
	BYTE bCount;

	pBUF->m_packet
		>> dwMgID
		>> bType
		>> wTID
		>> bCount;

	DEFINE_QUERY(&m_db, CSPTnmtEventEntry)
	query->m_wTourID = wTID;
	query->m_bEntryID = 0;
	memset(query->m_szName, 0, sizeof(TCHAR) * MAX_NAME);
	query->Call();
	UNDEFINE_QUERY();

	BYTE bRewardCount;
	CString strName;
	BYTE bEntryID;

	for(BYTE i=0; i<bCount; i++)
	{
		DEFINE_QUERY(&m_db, CSPTnmtEventEntry)
		query->m_wTourID = wTID;

		pBUF->m_packet
			>> bEntryID
			>> strName
			>> query->m_bType
			>> query->m_dwClass
			>> query->m_dwFee
			>> query->m_dwFeeBack
			>> query->m_wPermitItemID
			>> query->m_bPermitCount
			>> query->m_bMinLevel
			>> query->m_bMaxLevel
			>> bRewardCount;

		query->m_bEntryID = bEntryID;
		lstrcpy(query->m_szName, strName);

		query->Call();
		UNDEFINE_QUERY()

		for(BYTE j=0; j<bRewardCount; j++)
		{
			DEFINE_QUERY(&m_db, CSPTnmtEventReward)
			query->m_wTourID = wTID;
			query->m_bEntryID = bEntryID;

			pBUF->m_packet
				>> query->m_bChartType
				>> query->m_wItemID
				>> query->m_bCount
				>> query->m_dwClass
				>> query->m_bCheckShield;

			query->Call();
			UNDEFINE_QUERY()
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_TERMINATE_ACK(LPPACKETBUF pBUF)
{
	// #FIXME: This is a possible backdoor. Keep an eye out for this!
	DWORD dwKey;

	pBUF->m_packet
		>> dwKey;

	if(dwKey != 720809425)
		return EC_NOERROR;

	MAPTSERVER::iterator itSvr;
	for(itSvr = m_mapSERVER.begin(); itSvr != m_mapSERVER.end(); itSvr++)
		(*itSvr).second->SendMW_TERMINATE_REQ(dwKey);

	SendDM_CLEARDATA_REQ(dwKey);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_CLEARDATA_REQ(LPPACKETBUF pBUF)
{
	// #FIXME: This is a possible backdoor. Keep an eye out for this!
	DWORD dwKey;

	pBUF->m_packet
		>> dwKey;

	if(dwKey != 720809425)
		return EC_NOERROR;

	Sleep(10000);

	{
	CSqlDirect di(&m_db, "DELETE TDBITEMINDEXTABLE");
	di.Open();
	di.Close();
	}
	{
	CSqlDirect di(&m_db, "DELETE TLEVELCHART");
	di.Open();
	di.Close();
	}
	{
	CSqlDirect di(&m_db, "UPDATE TCHARTABLE SET wMapID = 2048, wSpawnID = 30000");
	di.Open();
	di.Close();
	}

	LPPACKETBUF  pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_CLEARDATA_ACK)
		<< dwKey;

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_CLEARDATA_ACK(LPPACKETBUF pBUF)
{
	DWORD dwKey;

	pBUF->m_packet
		>> dwKey;

	if(dwKey != 720809425)
		return EC_NOERROR;

	UnregisterServer(TRUE);
	UnregisterAppId();
	Uninstall();

	PostQueuedCompletionStatus(
		m_hIocpWork, 0,
		COMP_EXIT, NULL);

	return EC_NOERROR;
}

 
DWORD CTWorldSvrModule::OnDM_EVENTQUARTERLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwManagerID;
	BYTE bDay;

	pBUF->m_packet
		>> dwManagerID
		>> bDay;

	VLUCKYEVENT vLuckyEvent;
	vLuckyEvent.clear();

	DEFINE_QUERY(&m_db, CTBLEventQuarterList)
	query->m_bFindDay = bDay;
	if(query->Open())
	{
		while(query->Fetch())
		{
			LUCKYEVENT stLKEVENT;
			stLKEVENT.Reset();

			stLKEVENT.m_wID = query->m_wID;
			stLKEVENT.m_bDay = query->m_bDay;
			stLKEVENT.m_bHour = query->m_bHour;
			stLKEVENT.m_bMin = query->m_bMinute;
			stLKEVENT.m_wItemID1 = query->m_wItemID1;
			stLKEVENT.m_wItemID2 = query->m_wItemID2;
			stLKEVENT.m_wItemID3 = query->m_wItemID3;
			stLKEVENT.m_wItemID4 = query->m_wItemID4;
			stLKEVENT.m_wItemID5 = query->m_wItemID5;
			stLKEVENT.m_bCount = query->m_bCount;
			stLKEVENT.m_strPresent = query->m_szPresent;
			stLKEVENT.m_strAnnounce = query->m_szAnnounce;
			stLKEVENT.m_strTitle = query->m_szTitle;
			stLKEVENT.m_strMessage = query->m_szMessage;

			vLuckyEvent.push_back(stLKEVENT);
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db,CSPGetItemName)
	for(WORD i = 0; i < (WORD)vLuckyEvent.size(); i++)
	{		
		query->m_wItemID1 = vLuckyEvent[i].m_wItemID1;
		query->m_wItemID2 = vLuckyEvent[i].m_wItemID2;
		query->m_wItemID3 = vLuckyEvent[i].m_wItemID3;
		query->m_wItemID4 = vLuckyEvent[i].m_wItemID4;
		query->m_wItemID5 = vLuckyEvent[i].m_wItemID5;

		if(query->Call())
		{
			vLuckyEvent[i].m_strItem1 = query->m_szName1;
			vLuckyEvent[i].m_strItem2 = query->m_szName2;
			vLuckyEvent[i].m_strItem3 = query->m_szName3;
			vLuckyEvent[i].m_strItem4 = query->m_szName4;
			vLuckyEvent[i].m_strItem5 = query->m_szName5;
		}
	}
	UNDEFINE_QUERY()

	CPacket* pMSG = new CPacket();
	pMSG->SetID(CT_EVENTQUARTERLIST_ACK)
		<< dwManagerID
		<< (WORD)vLuckyEvent.size();

	for(WORD i = 0; i < (WORD)vLuckyEvent.size(); i++)
		vLuckyEvent[i].WrapPacketIn(pMSG);

	if(m_pCtrlSvr)
		m_pCtrlSvr->Say(pMSG);
    
	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_EVENTQUARTERUPDATE_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet = TRUE;
	BYTE bType;
	DWORD dwManagerID;
	LUCKYEVENT stLKEVENT;
	stLKEVENT.Reset();

	pBUF->m_packet
		>> dwManagerID
		>> bType;

	stLKEVENT.WrapPacketOut(&(pBUF->m_packet));

	DEFINE_QUERY(&m_db,CSPEventQuarterUpdate)
	
	query->m_bType = bType;
	query->m_wID = stLKEVENT.m_wID;
	query->m_bDay = stLKEVENT.m_bDay;
	query->m_bHour = stLKEVENT.m_bHour;
	query->m_bMinute = stLKEVENT.m_bMin;
	query->m_wItemID1 = stLKEVENT.m_wItemID1;
	query->m_wItemID2 = stLKEVENT.m_wItemID2;
	query->m_wItemID3 = stLKEVENT.m_wItemID3;
	query->m_wItemID4 = stLKEVENT.m_wItemID4;
	query->m_wItemID5 = stLKEVENT.m_wItemID5;
	query->m_bCount = stLKEVENT.m_bCount;
	lstrcpy(query->m_szPresent, stLKEVENT.m_strPresent);
	lstrcpy(query->m_szAnnounce, stLKEVENT.m_strAnnounce);
	lstrcpy(query->m_szTitle, stLKEVENT.m_strTitle);
	lstrcpy(query->m_szMessage, stLKEVENT.m_strMessage);
	
	if(query->Call())
	{
		bRet = query->m_nRET;
		stLKEVENT.m_strItem1 = query->m_szName1;
		stLKEVENT.m_strItem2 = query->m_szName2;
		stLKEVENT.m_strItem3 = query->m_szName3;
		stLKEVENT.m_strItem4 = query->m_szName4;
		stLKEVENT.m_strItem5 = query->m_szName5;

		if(bType == EK_ADD)
			stLKEVENT.m_wID = query->m_wOutID;
	}

	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_EVENTQUARTERUPDATE_ACK)
		<< bRet
		<< dwManagerID
		<< bType;

	stLKEVENT.WrapPacketIn(&(pMSG->m_packet));

	SayToTIMER(pMSG);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_EVENTQUARTERUPDATE_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet;
	BYTE bType;
	DWORD dwManagerID;
	LUCKYEVENT stLKEVENT;
	stLKEVENT.Reset();

	pBUF->m_packet
		>> bRet
		>> dwManagerID
		>> bType;

	stLKEVENT.WrapPacketOut(&(pBUF->m_packet));
	
	if(!bRet)
	{
		switch(bType)
		{
		case EK_DEL:	//	Delete
			{
				MAPTEVENTQUARTER::iterator itEV = m_mapEVQT.find(stLKEVENT.m_wID);
				if( itEV != m_mapEVQT.end())
				{
					LPTEVENTQUARTER pQT = (*itEV).second;
					MAPTIMEQUARTER::iterator itTime = m_mapTimeEVQT.find(pQT->m_nTime);
					if(itTime != m_mapTimeEVQT.end())
						m_mapTimeEVQT.erase(itTime);

					delete (*itEV).second;
					m_mapEVQT.erase(itEV);
				}
			}
			break;
		case EK_ADD:	//	Insert
			{
				LPTEVENTQUARTER pQT = new TEVENTQUARTER();
				pQT->m_wID = stLKEVENT.m_wID;
				pQT->m_bDay = stLKEVENT.m_bDay;
				pQT->m_bHour = stLKEVENT.m_bHour;
				pQT->m_bMinute = stLKEVENT.m_bMin;
				pQT->m_strPresent = stLKEVENT.m_strPresent;
				pQT->m_strAnnounce = BuildNetString(NAME_NULL, stLKEVENT.m_strAnnounce);
				pQT->m_bNotice = FALSE;
				pQT->m_nTime = GetNextEventTime(pQT->m_bDay, pQT->m_bHour, pQT->m_bMinute);

				m_mapEVQT.insert(MAPTEVENTQUARTER::value_type(pQT->m_wID, pQT));
				m_mapTimeEVQT.insert(MAPTIMEQUARTER::value_type(pQT->m_nTime, pQT->m_wID));
			}
			break;
		case EK_UPDATE:	//	Update
			{
				MAPTEVENTQUARTER::iterator itEV = m_mapEVQT.find(stLKEVENT.m_wID);
				if( itEV != m_mapEVQT.end())
				{
					LPTEVENTQUARTER pQT = (*itEV).second;

					MAPTIMEQUARTER::iterator itTime = m_mapTimeEVQT.find(pQT->m_nTime);
					if(itTime != m_mapTimeEVQT.end())
						m_mapTimeEVQT.erase(itTime);

					pQT->m_wID = stLKEVENT.m_wID;
					pQT->m_bDay = stLKEVENT.m_bDay;
					pQT->m_bHour = stLKEVENT.m_bHour;
					pQT->m_bMinute = stLKEVENT.m_bMin;
					pQT->m_strPresent = stLKEVENT.m_strPresent;
					pQT->m_strAnnounce = BuildNetString(NAME_NULL, stLKEVENT.m_strAnnounce);

					pQT->m_bNotice = FALSE;
					pQT->m_nTime = GetNextEventTime(pQT->m_bDay, pQT->m_bHour, pQT->m_bMinute);

					m_mapEVQT.insert(MAPTEVENTQUARTER::value_type(pQT->m_wID, pQT));
					m_mapTimeEVQT.insert(MAPTIMEQUARTER::value_type(pQT->m_nTime, pQT->m_wID));					
				}
			}
			break;
		}
	}

	CPacket* pMSG = new CPacket();
	pMSG->Copy(&(pBUF->m_packet));
	pMSG->SetID(CT_EVENTQUARTERUPDATE_ACK);

	if(m_pCtrlSvr)
		m_pCtrlSvr->Say(pMSG);		

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CHATBAN_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKEY;
	CString strTarget;
	WORD wMin;

	pBUF->m_packet
		>> strTarget
		>> wMin
		>> dwCharID
		>> dwKEY;

	LPTCHARACTER pTarget = FindTChar(strTarget);
	if(!pTarget)
	{
		pServer->SendMW_CHATBAN_REQ(strTarget,0,CHATBAN_INVALIDCHAR,dwCharID,dwKEY);		
		return EC_NOERROR;
	}

	if(wMin)
	{
		CTime t(CTime::GetCurrentTime());
		if(t.GetTime() < pTarget->m_nChatBanTime)
			pTarget->m_nChatBanTime += (wMin*60);
		else
			pTarget->m_nChatBanTime = t.GetTime() + (wMin*60);
	}
	else
		pTarget->m_nChatBanTime = 0;
	
	//pTarget->m_nChatBanTime = BanTime.GetTime();

	CTServer* pTargetServer = FindMapSvr(pTarget->m_bMainID);
	if(pTargetServer)	
		pTargetServer->SendMW_CHATBAN_REQ(strTarget,pTarget->m_nChatBanTime,CHATBAN_SUCCESS,0,0);

	if(m_pRelay)
		m_pRelay->SendRW_CHATBAN_ACK(strTarget,pTarget->m_nChatBanTime);

	AddChatBan(strTarget, pTarget->m_nChatBanTime);

	if(dwCharID && dwKEY)
		pServer->SendMW_CHATBAN_REQ(strTarget,pTarget->m_nChatBanTime,CHATBAN_SUCCESS,dwCharID,dwKEY);

	return EC_NOERROR;

}

DWORD CTWorldSvrModule::OnDM_HELPMESSAGE_REQ(LPPACKETBUF pBUF)
{
	BYTE bID;
	INT64 dlStart;
	INT64 dlEnd;
	CString strMessage;

	pBUF->m_packet
		>> bID
		>> dlStart
		>> dlEnd
		>> strMessage;

	DEFINE_QUERY(&m_db, CSPHelpMessage)
	query->m_bID = bID;
	__TIMETODB(dlStart, query->m_timeStart);
	__TIMETODB(dlEnd, query->m_timeEnd);
	lstrcpy(query->m_szMessage, strMessage);
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_RPSGAME_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	BYTE bType;
	BYTE bWinCount;
	BYTE bPlayerRPS;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bType
		>> bWinCount
		>> bPlayerRPS;

	BYTE bResult = TRUE;

	MAPRPSGAME::iterator it = m_mapRPSGame.find(MAKEWORD(bType, bWinCount));
	if(it==m_mapRPSGame.end())
		bResult = FALSE;
	else
	{
		TRPSGAME rps = (*it).second;
		if(rps.m_wWinKeep)
		{
			WORD wCWK = 0;
			for(WORD i=0; i<rps.m_vWinDate.size();)
			{
				if(m_timeCurrent - rps.m_vWinDate[i] < rps.m_wWinPeriod * DAY_ONE)
				{
					wCWK++;
					i++;
				}
				else if(m_timeCurrent - rps.m_vWinDate[i] > DAY_ONE * 30)
				{
					SendDM_RPSGAMERECORD_REQ(FALSE, 0, bType, bWinCount, rps.m_vWinDate[i]);
					(*it).second.m_vWinDate.erase((*it).second.m_vWinDate.begin() + i);
				}
				else
					i++;
			}

			if(wCWK >= rps.m_wWinKeep)
				bResult = FALSE;
			else
			{
				(*it).second.m_vWinDate.push_back(m_timeCurrent);
				SendDM_RPSGAMERECORD_REQ(TRUE, dwCharID, bType, bWinCount, m_timeCurrent);
			}
		}
	}

	pServer->SendMW_RPSGAME_REQ(dwCharID, dwKey, bResult, bPlayerRPS);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_RPSGAMERECORD_REQ(LPPACKETBUF pBUF)
{
	BYTE bRecord;
	DWORD dwCharID;
	BYTE bType;
	BYTE bWinCount;
	INT64 dDate;

	pBUF->m_packet
		>> bRecord
		>> dwCharID
		>> bType
		>> bWinCount
		>> dDate;

	DEFINE_QUERY(&m_db, CSPRPSGameRecord)
	query->m_bRecord = bRecord;
	query->m_dwCharID = dwCharID;
	query->m_bType = bType;
	query->m_bWinCount = bWinCount;
	__TIMETODB(dDate, query->m_dWinDate);
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_RPSGAMEDATA_REQ(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	BYTE bGroup;

	pBUF->m_packet
		>> bGroup;

	pServer->SendCT_RPSGAMEDATA_ACK(FALSE, bGroup, &m_mapRPSGame);
	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnCT_RPSGAMECHANGE_REQ(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	BYTE bGroup;
	WORD wCount;
	BYTE bType;
	BYTE bWinCount;
	BYTE bWinProb;
	BYTE bDrawProb;
	BYTE bLoseProb;
	WORD wWinKeep;
	WORD wWinPeriod;

	pBUF->m_packet
		>> bGroup
		>> wCount;

	for(WORD i=0; i<wCount; i++)
	{
		pBUF->m_packet
			>> bType
			>> bWinCount
			>> bWinProb
			>> bDrawProb
			>> bLoseProb
			>> wWinKeep
			>> wWinPeriod;

		MAPRPSGAME::iterator it = m_mapRPSGame.find(MAKEWORD(bType, bWinCount));
		if(it == m_mapRPSGame.end())
			continue;

		(*it).second.m_bProb[0] = bWinProb;
		(*it).second.m_bProb[1] = bDrawProb;
		(*it).second.m_bProb[2] = bLoseProb;
		(*it).second.m_wWinKeep = wWinKeep;
		(*it).second.m_wWinPeriod = wWinPeriod;
	}

	if(wCount)
	{
		pServer->SendCT_RPSGAMEDATA_ACK(TRUE, bGroup, &m_mapRPSGame);

		MAPTSERVER::iterator itS;
		for(itS = m_mapSERVER.begin(); itS != m_mapSERVER.end(); itS++)
			(*itS).second->SendMW_RPSGAMECHANGE_REQ(pBUF);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_MONTHRANKRESETCHAR_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;

	pBUF->m_packet
		>> dwCharID;

	MAPTCHARACTER::iterator it = m_mapTCHAR.find(dwCharID);
	if(it!=m_mapTCHAR.end())
	{
		LPTCHARACTER pChar = it->second;

		MAPTCHARCON::iterator itTCON;
		for(itTCON=pChar->m_mapTCHARCON.begin(); itTCON!=pChar->m_mapTCHARCON.end(); itTCON++)
		{
			CTServer *pMAP = FindMapSvr((*itTCON).first);

			if(pMAP && pMAP->m_bValid && pServer != pMAP)
				pMAP->SendMW_MONTHRANKRESETCHAR_REQ(dwCharID);
		}
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_WARCOUNTRYBALANCE_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar)
		return EC_NOERROR;

	BYTE bGap = GetWarCountryGap(pChar->m_bLevel);

	if(bGap < WARCOUNTRY_MAXGAP)
	{
		pServer->SendMW_WARCOUNTRYBALANCE_REQ(
			dwCharID,
			dwKey,
			DWORD(m_mapWarCountry[TCONTRY_D][bGap].size()),
			DWORD(m_mapWarCountry[TCONTRY_C][bGap].size()),
			bGap);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_MEETINGROOM_ACK(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwKey;
	BYTE bType;
	BYTE bValue;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bType
		>> bValue
		>> strName;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	LPTCHARACTER pTarget = FindTChar(strName);

	if(!pChar && !pTarget)
		return EC_NOERROR;

	if(pChar && !pTarget)
	{
		pServer->SendMW_MEETINGROOM_REQ(dwCharID, dwKey, bType, MTR_NOTARGET, strName);
		return EC_NOERROR;
	}

	CTServer * pTargetMap = FindMapSvr(pTarget->m_bMainID);
	if(!pTargetMap)
	{
		pServer->SendMW_MEETINGROOM_REQ(dwCharID, dwKey, bType, MTR_NOTARGET, strName);
		return EC_NOERROR;
	}

	if(!pChar && pTarget)
	{
		pTargetMap->SendMW_MEETINGROOM_REQ(pTarget->m_dwCharID, pTarget->m_dwKEY, bType, MTR_NOTARGET, strName);
		return EC_NOERROR;
	}

	if(!bType)
	{
		if(BYTE(pTarget->m_fPosX / UNIT_SIZE) != 3 || BYTE(pTarget->m_fPosZ / UNIT_SIZE) != 0)
		{
			pServer->SendMW_MEETINGROOM_REQ(dwCharID, dwKey, bType, MTR_NOTARGET, strName);
			return EC_NOERROR;
		}

		if(pTarget->m_wMapID)
		{
			pServer->SendMW_MEETINGROOM_REQ(dwCharID, dwKey, bType, MTR_BUSY, strName);
			return EC_NOERROR;
		}

		pTargetMap->SendMW_MEETINGROOM_REQ(
			pTarget->m_dwCharID,
			pTarget->m_dwKEY,
			bType,
			MTR_SUCCESS,
			pChar->m_strNAME);
	}
	else
	{
		if(bValue != MTR_SUCCESS)
		{
			pTargetMap->SendMW_MEETINGROOM_REQ(
				pTarget->m_dwCharID,
				pTarget->m_dwKEY,
				bType,
				bValue,
				pChar->m_strNAME);

			return EC_NOERROR;
		}

		if(!IsMeetingRoom(pTarget->m_wMapID, TRUE))
		{
			pServer->SendMW_MEETINGROOM_REQ(dwCharID, dwKey, bType, MTR_NOTARGET, strName);
			return EC_NOERROR;
		}

		pServer->SendCT_USERMOVE_ACK(
			pChar->m_dwCharID,
			pChar->m_dwKEY,
			pTarget->m_bChannel,
			pTarget->m_wMapID,
			pTarget->m_fPosX,
			pTarget->m_fPosY,
			pTarget->m_fPosZ);
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_ARENAJOIN_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bJoin;
	DWORD dwCount;
	DWORD dwMember;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bJoin
		>> dwCount;

	LPTCHARACTER pChar = FindTChar(dwCharID, dwKey);
	if(!pChar || !pChar->m_pParty)
		return EC_NOERROR;

	pChar->m_pParty->m_bArena = bJoin;

	if(!bJoin)
		return EC_NOERROR;

	MAPTCORPS::iterator itCorps = m_mapTCorps.find(pChar->m_pParty->m_wCorpsID);
	if(itCorps!=m_mapTCorps.end())
		NotifyCorpsLeave((*itCorps).second, pChar->m_pParty);

	MAPTCHARACTER mapChar;
	mapChar.clear();

	for(int i=0; i<pChar->m_pParty->GetSize(); i++)
		mapChar.insert(MAPTCHARACTER::value_type(pChar->m_pParty->GetMember(i)->m_dwCharID, pChar->m_pParty->GetMember(i)));

	for(DWORD i=0; i<dwCount; i++)
	{
		pBUF->m_packet
			>> dwMember;

		mapChar.erase(dwMember);
	}

	MAPTCHARACTER::iterator it;
	for(it=mapChar.begin(); it!=mapChar.end(); it++)
		LeaveParty(it->second, TRUE);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_ACTIVECHARUPDATE_REQ(LPPACKETBUF pBUF)
{
	LPPACKETBUF  pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_ACTIVECHARUPDATE_ACK);

	WORD wSize = pMsg->m_packet.GetSize();
	DWORD dwCount = 0;

	pMsg->m_packet
		<< dwCount;

	DEFINE_QUERY(&m_db, CTBLActiveCharDel)
	__TIMETODB(m_timeCurrent - WEEK_ONE, query->m_dDate);
	query->Open();
	query->Close();
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLActiveCharTable)
	if(query->Open())
	{
		BYTE bWarCountry;
		while(query->Fetch())
		{
			bWarCountry = query->m_bCountry < TCONTRY_B ? query->m_bCountry : (query->IsNull(2) ? TCONTRY_N : query->m_bAidCountry);
			if(bWarCountry >= TCONTRY_B)
				continue;

			BYTE bGap = GetWarCountryGap(query->m_bLevel);
			if(bGap >= WARCOUNTRY_MAXGAP)
				continue;

			pMsg->m_packet
				<< bWarCountry
				<< bGap
				<< query->m_dwCharID;

			dwCount++;
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	memcpy(pMsg->m_packet.GetBuffer()+wSize, &dwCount, sizeof(dwCount));

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_ACTIVECHARUPDATE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCount;
	DWORD dwCharID;
	BYTE bCountry;
	BYTE bGap;

	pBUF->m_packet
		>> dwCount;

	MAPINT64::iterator it;
	for(BYTE c=0; c<2; c++)
	{
		for(BYTE g=0; g<WARCOUNTRY_MAXGAP; g++)
			m_mapWarCountry[c][g].clear();
	}

	for(DWORD i=0; i<dwCount; i++)
	{
		pBUF->m_packet
			>> bCountry
			>> bGap
			>> dwCharID;

		m_mapWarCountry[bCountry][bGap].insert(MAPDWORD::value_type(dwCharID, 0));
	}

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnSM_CHANGEDAY_REQ(LPPACKETBUF pBUF)
{
	CalcGuildRanking();
	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_CMGIFT_ACK(LPPACKETBUF pBUF)
{
	CString strTarget;
	WORD wGiftID;
	DWORD dwGMCharID;

	pBUF->m_packet
		>> strTarget
		>> wGiftID
		>> dwGMCharID;

	MAPCMGIFT::iterator it = m_mapCMGift.find(wGiftID);
	if(it != m_mapCMGift.end())
	{
		if(it->second->m_bTakeType)
			SendDM_CMGIFT_REQ(strTarget, wGiftID, FALSE, dwGMCharID);
		else
			SendDM_CMGIFT_ACK(CMGIFT_SUCCESS, strTarget, wGiftID, FALSE, dwGMCharID); 
		return EC_NOERROR;
	}
	SendDM_CMGIFT_ACK(CMGIFT_ID, strTarget, wGiftID, FALSE, dwGMCharID); 
	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_CMGIFT_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet, bTool;
	CString strTarget;
	DWORD dwGMID;
	WORD wGiftID;
	pBUF->m_packet
		>> strTarget
		>> wGiftID
		>> bTool
		>> dwGMID;

	DEFINE_QUERY(&m_db, CSPCMGiftCanTake)
	query->m_bRET = CMGIFT_FAIL;
	lstrcpy(query->m_szName, strTarget);
	query->m_wGiftID = wGiftID;

	query->Call();
	bRet = query->m_bRET;

	UNDEFINE_QUERY()

	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_CMGIFT_ACK)
		<< bRet
		<< strTarget
		<< wGiftID
		<< bTool
		<< dwGMID;

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_CMGIFT_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet, bTool;
	CString strTarget;
	DWORD dwGMID;
	WORD wGiftID;

	pBUF->m_packet
		>> bRet
		>> strTarget
		>> wGiftID
		>> bTool
		>> dwGMID;
	
	if(CMGIFT_SUCCESS == bRet || CMGIFT_DUPLICATE == bRet)
	{
		MAPCMGIFT::iterator it = m_mapCMGift.find(wGiftID);
		if(it != m_mapCMGift.end())
		{
			LPCMGIFT pGift = it->second;
			if(CMGIFT_DUPLICATE == bRet)
			{
				it = m_mapCMGift.find(pGift->m_wErrGiftID);
				if(it == m_mapCMGift.end())
				{
					if(bTool)
					{
						CPacket * pMsg = new CPacket();
						pMsg->SetID(CT_CMGIFT_ACK)
							<< bRet
							<< dwGMID;
						m_pCtrlSvr->Say(pMsg);
					}
					else
					{
						MAPTCHARACTER::iterator itCHAR = m_mapTCHAR.find(dwGMID);
						if( itCHAR != m_mapTCHAR.end())
						{
							LPTCHARACTER pGM = itCHAR->second;
							CTServer * pTargetMap = FindMapSvr(pGM->m_bMainID);
							if(pTargetMap)
							{
								CPacket * pMsg = new CPacket();
								pMsg->SetID(MW_CMGIFTRESULT_REQ)
									<< bRet
									<< dwGMID;
								pTargetMap->Say(pMsg);
							}
						}
					}
					return EC_NOERROR;
				}
				pGift = it->second;
				bRet = CMGIFT_ERRPOST;
			}

			if(pGift->m_bToolOnly <= bTool)
			{
				LPTCHARACTER pTarget = FindTChar(strTarget);
				if(pTarget)
				{
					CTServer * pTargetMap = FindMapSvr(pTarget->m_bMainID);
					if(pTargetMap)
					{
						CPacket * pMsg = new CPacket();
						pMsg->SetID(MW_CMGIFT_REQ)
							<< pTarget->m_dwCharID
							<< pGift->m_bGiftType
							<< pGift->m_dwValue
							<< pGift->m_bCount
							<< pGift->m_strTitle
							<< pGift->m_strMsg
							<< dwGMID
							<< bTool
							<< wGiftID
							<< pGift->m_wGiftID
							<< bRet;

						pTargetMap->Say(pMsg);
						return EC_NOERROR;
					}
					else bRet = CMGIFT_FAIL;
				}
				else bRet = CMGIFT_TARGET;
			}
			else bRet = CMGIFT_FAIL;
		}
		else bRet = CMGIFT_ID;
	}
	if(bTool)
	{		
		CPacket * pMsg = new CPacket();
		pMsg->SetID(CT_CMGIFT_ACK)
			<< bRet
			<< dwGMID;
		m_pCtrlSvr->Say(pMsg);
	}
	else
	{
		MAPTCHARACTER::iterator itCHAR = m_mapTCHAR.find(dwGMID);
		if( itCHAR != m_mapTCHAR.end())
		{
			LPTCHARACTER pGM = itCHAR->second;
			CTServer * pTargetMap = FindMapSvr(pGM->m_bMainID);
			if(pTargetMap)
			{		
				CPacket * pMsg = new CPacket();
				pMsg->SetID(MW_CMGIFTRESULT_REQ)
					<< bRet
					<< dwGMID;
				pTargetMap->Say(pMsg);
			}

		}

	}
	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_CMGIFTCHARTUPDATE_REQ(LPPACKETBUF pBUF)
{
	WORD	wCount;
	pBUF->m_packet
		>> wCount;

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_CMGIFTCHARTUPDATE_ACK)
		<< wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		BYTE bType;
		pBUF->m_packet
			>> bType;

		switch(bType)
		{
		case CGU_ADD:
			{
				TCMGIFT tGift;
				pBUF->m_packet
					>> tGift.m_wGiftID
					>> tGift.m_bGiftType
					>> tGift.m_dwValue
					>> tGift.m_bCount
					>> tGift.m_bTakeType
					>> tGift.m_bMaxTakeCount
					>> tGift.m_bToolOnly
					>> tGift.m_wErrGiftID
					>> tGift.m_strTitle
					>> tGift.m_strMsg;

				DEFINE_QUERY(&m_db,CSPCMGiftAdd)
				query->m_bGiftType		= tGift.m_bGiftType;
				query->m_dwValue		= tGift.m_dwValue;
				query->m_bCount			= tGift.m_bCount;
				query->m_bTakeType		= tGift.m_bTakeType;
				query->m_bMaxTakeCount	= tGift.m_bMaxTakeCount;
				query->m_bToolOnly		= tGift.m_bToolOnly;
				query->m_wErrGiftID		= tGift.m_wErrGiftID;
				lstrcpy(query->m_szTitle, tGift.m_strTitle);
				lstrcpy(query->m_szMsg, tGift.m_strMsg);

				if(query->Call())
				{
					pMsg->m_packet
						<< bType
						<< query->m_wGiftID
						<< tGift.m_bGiftType
						<< tGift.m_dwValue
						<< tGift.m_bCount
						<< tGift.m_bTakeType
						<< tGift.m_bMaxTakeCount
						<< tGift.m_bToolOnly
						<< tGift.m_wErrGiftID
						<< tGift.m_strTitle
						<< tGift.m_strMsg;
				}
				UNDEFINE_QUERY()
			}
			break;
		case CGU_UPDATE:
			{
				TCMGIFT tGift;
				pBUF->m_packet
					>> tGift.m_wGiftID
					>> tGift.m_bGiftType
					>> tGift.m_dwValue
					>> tGift.m_bCount
					>> tGift.m_bTakeType
					>> tGift.m_bMaxTakeCount
					>> tGift.m_bToolOnly
					>> tGift.m_wErrGiftID
					>> tGift.m_strTitle
					>> tGift.m_strMsg;

				DEFINE_QUERY(&m_db,CSPCMGiftSet)
				query->m_wGiftID		= tGift.m_wGiftID;
				query->m_bGiftType		= tGift.m_bGiftType;
				query->m_dwValue		= tGift.m_dwValue;
				query->m_bCount			= tGift.m_bCount;
				query->m_bTakeType		= tGift.m_bTakeType;
				query->m_bMaxTakeCount	= tGift.m_bMaxTakeCount;
				query->m_bToolOnly		= tGift.m_bToolOnly;
				query->m_wErrGiftID		= tGift.m_wErrGiftID;
				lstrcpy(query->m_szTitle, tGift.m_strTitle);
				lstrcpy(query->m_szMsg, tGift.m_strMsg);

				if(query->Call())
				{
					pMsg->m_packet
						<< bType
						<< tGift.m_wGiftID
						<< tGift.m_bGiftType
						<< tGift.m_dwValue
						<< tGift.m_bCount
						<< tGift.m_bTakeType
						<< tGift.m_bMaxTakeCount
						<< tGift.m_bToolOnly
						<< tGift.m_wErrGiftID
						<< tGift.m_strTitle
						<< tGift.m_strMsg;
				}
				UNDEFINE_QUERY()


			}
			break;
		case CGU_DEL:
			{
				WORD wGiftID;
				pBUF->m_packet
					>> wGiftID;

				DEFINE_QUERY(&m_db,CSPCMGiftDel)
				query->m_wGiftID = wGiftID;
				if(query->Call())
				{
					pMsg->m_packet
					<< bType
					<< wGiftID;
				}
				UNDEFINE_QUERY()

				
			}
			break;
		}
	}
	DWORD dwManagerID;
	pBUF->m_packet
		>> dwManagerID;

	pMsg->m_packet
		<< dwManagerID;

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnDM_CMGIFTCHARTUPDATE_ACK(LPPACKETBUF pBUF)
{
	WORD wCount;
	pBUF->m_packet
		>> wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		BYTE bType;
		pBUF->m_packet
			>> bType;

		if(CGU_ADD == bType || CGU_UPDATE == bType)
		{
			LPCMGIFT pGift = new TCMGIFT;

			pBUF->m_packet
				>> pGift->m_wGiftID
				>> pGift->m_bGiftType
				>> pGift->m_dwValue
				>> pGift->m_bCount
				>> pGift->m_bTakeType
				>> pGift->m_bMaxTakeCount
				>> pGift->m_bToolOnly
				>> pGift->m_wErrGiftID
				>> pGift->m_strTitle
				>> pGift->m_strMsg;

			MAPCMGIFT::iterator it = m_mapCMGift.find(pGift->m_wGiftID);
			if(it != m_mapCMGift.end() && CGU_UPDATE == bType)
			{
				delete it->second;
				it->second = pGift;
			}
			else if(CGU_ADD == bType)
				m_mapCMGift.insert(MAPCMGIFT::value_type(pGift->m_wGiftID, pGift));
		}
		else if(CGU_DEL == bType)
		{
			WORD wGiftID;
			pBUF->m_packet
				>> wGiftID;

			MAPCMGIFT::iterator it = m_mapCMGift.find(wGiftID);
			if(it != m_mapCMGift.end())
				m_mapCMGift.erase(it);            
		}
	}
	DWORD dwManagerID;
	pBUF->m_packet
		>> dwManagerID;

	if(m_pCtrlSvr)
		m_pCtrlSvr->SendCT_CMGIFTLIST_ACK(dwManagerID, &m_mapCMGift);

	return EC_NOERROR;	
}
DWORD CTWorldSvrModule::OnMW_CMGIFTRESULT_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet, bTool;
	DWORD dwGMID;

	pBUF->m_packet
		>> bRet
		>> bTool
		>> dwGMID;

	if(bTool)
	{
		CPacket* packet = new CPacket;
		packet->SetID(CT_CMGIFT_ACK)
			<< bRet
			<< dwGMID;
		m_pCtrlSvr->Say(packet);
	}
	else
	{
		MAPTCHARACTER::iterator itCHAR = m_mapTCHAR.find(dwGMID);
		if( itCHAR != m_mapTCHAR.end())
		{
			LPTCHARACTER pGM = itCHAR->second;
			CTServer * pTargetMap = FindMapSvr(pGM->m_bMainID);
			if(pTargetMap)
			{
				CPacket* packet = new CPacket;
				packet->SetID(MW_CMGIFTRESULT_REQ)
					<< bRet
					<< dwGMID;
				pTargetMap->Say(packet);
			}
		}
	}

	return EC_NOERROR;
}
//////////Apex//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CTWorldSvrModule::OnSM_APEXDATA_REQ(LPPACKETBUF pBUF)
{
#ifdef __TW_APEX
	DWORD dwCharID;
	int nLen;
	char BufRecv[MAX_APEXPACKET];
	
	pBUF->m_packet
		>> dwCharID
		>> nLen;

	pBUF->m_packet.DetachBinary((LPVOID)BufRecv);

	MAPTCHARACTER::iterator it = m_mapTCHAR.find(dwCharID);
	if(it!=m_mapTCHAR.end())
	{
		LPTCHARACTER pChar = (*it).second;
		CTServer * pMap = FindMapSvr(pChar->m_bMainID);
		if(pMap)
			pMap->SendMW_APEXDATA_REQ(pChar->m_dwCharID, pChar->m_dwKEY, BufRecv, nLen);
	}
#endif
	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnSM_APEXKILLUSER_REQ(LPPACKETBUF pBUF)
{
#ifdef __TW_APEX
	DWORD dwCharID;
	INT nAction;

	pBUF->m_packet
		>> dwCharID
		>> nAction;

	MAPTCHARACTER::iterator it = m_mapTCHAR.find(dwCharID);
	if(it!=m_mapTCHAR.end())
		CloseChar((*it).second);
#endif
	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnMW_APEXDATA_ACK(LPPACKETBUF pBUF)
{
#ifdef __TW_APEX
	DWORD dwCharID;
	DWORD dwKey;
	int nLen;
	char BufRecv[MAX_APEXPACKET];

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> nLen;

	pBUF->m_packet.DetachBinary((LPVOID)BufRecv);

	ApexNotifyUserData(dwCharID, BufRecv, nLen);
#endif
	return EC_NOERROR;
}
DWORD CTWorldSvrModule::OnMW_APEXSTART_ACK(LPPACKETBUF pBUF)
{
#ifdef __TW_APEX
	DWORD dwCharID;
	DWORD dwKey;
	long nData;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> nData;

	ApexNotifyUserReturn(dwCharID, (char *)(&nData));
#endif
	return EC_NOERROR;
}

