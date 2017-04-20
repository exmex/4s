// CSHandler.cpp : C/S Protocol Handler 함수의 구현입니다.

#include "stdafx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

DWORD CTMapSvrModule::OnCT_SERVICEMONITOR_ACK( LPPACKETBUF pBUF )
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	DWORD dwTick;
	pBUF->m_packet
		>> dwTick;

	pPlayer->SendCT_SERVICEMONITOR_REQ(dwTick, DWORD(m_mapSESSION.size()), DWORD(m_mapPLAYER.size()), DWORD(m_mapACTIVEUSER.size()));

	static DWORD tick = GetTickCount();

	if(GetTickCount() - tick > 5000)
	{
		DWORD dwCurrent = GetTickCount();
		MAPPLAYER::iterator it;
		for(it=m_mapSESSION.begin(); it!=m_mapSESSION.end(); it++)
		{
			if((*it).second->m_bSessionType == SESSION_CLIENT &&
				(*it).second->m_dwAcceptTick &&
				dwCurrent - (*it).second->m_dwAcceptTick > 10000)
			{
//				LogEvent("Accept Tick %d", (*it).second->m_dwID);
				CloseSession((*it).second);
			}

			if((*it).second->m_bSessionType == SESSION_CLIENT &&
				(*it).second->m_dwCloseTick &&
				dwCurrent - (*it).second->m_dwCloseTick > 10000)
			{
//				LogEvent("Close Tick %d, Can Delete = %d", (*it).second->m_dwID, (*it).second->m_bCanDelete);
				if((*it).second->m_bCanDelete)
					SetEventCloseSession((*it).second, (( CTPlayer *)(*it).second)->m_bMain);
				else
					CloseSession((*it).second);
			}
		}
		tick = dwCurrent;
	}

	return EC_NOERROR;
}



DWORD CTMapSvrModule::OnCT_ANNOUNCEMENT_ACK( LPPACKETBUF pBUF )
{
	MAPPLAYER::iterator it;
	CString strAnnounce;

	pBUF->m_packet
		>> strAnnounce;

	strAnnounce.Left(ONE_KBYTE);

	for( it = m_mapPLAYER.begin(); it != m_mapPLAYER.end(); it++)
	{
		if((*it).second->m_bMain)
			(*it).second->SendCS_CHAT_ACK( CHAT_WORLD, 0, GetSvrMsg(NAME_OPERATOR), BuildNetString(NAME_NULL, strAnnounce));
	}

	return EC_NOERROR;
}


DWORD CTMapSvrModule::OnCT_USERKICKOUT_ACK( LPPACKETBUF pBUF )
{
	CString strUser;

	pBUF->m_packet
		>> strUser;

	strUser.MakeUpper();

	MAPPLAYERNAME::iterator finder = m_mapPLAYERNAME.find(strUser);
	if( finder != m_mapPLAYERNAME.end())
		CloseSession((*finder).second);

	return EC_NOERROR;
}


DWORD CTMapSvrModule::OnCT_MONSPAWNFIND_ACK(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	DWORD dwManager;
	BYTE bChannel;
	WORD wMapID;
	WORD wSpawnID;

	pBUF->m_packet
		>> dwManager
		>> bChannel
		>> wMapID
		>> wSpawnID;

	CTMap *pMap = FindTMap( bChannel, 0, wMapID);
	if(!pMap)
		return EC_NOERROR;

	LPTMONSPAWN pTemp = FindTMonSpawn(wSpawnID);
	if(!pTemp)
		return EC_NOERROR;

	MAPTMONSPAWN::iterator it = pMap->m_mapTMONSPAWN.find(wSpawnID);
	if( it != pMap->m_mapTMONSPAWN.end() && !(*it).second->m_vTMON.empty())
		pPlayer->SendCT_MONSPAWNFIND_ACK( dwManager, wMapID, wSpawnID, (*it).second);
	else if(pTemp->m_bCount)
	{
		CTMonSpawn * pSPAWN = new CTMonSpawn();
		pSPAWN->m_pSPAWN = pTemp;
		for(BYTE i=0; i<pTemp->m_bCount; i++)
		{
			CTMonster *pMON = new CTMonster();
			pMON->m_fPosX = pTemp->m_fPosX;
			pMON->m_fPosY = pTemp->m_fPosY;
			pMON->m_fPosZ = pTemp->m_fPosZ;
			pSPAWN->m_vTMON.push_back(pMON);
		}

		pPlayer->SendCT_MONSPAWNFIND_ACK( dwManager, wMapID, wSpawnID, pSPAWN);

		for(BYTE i=0; i<pTemp->m_bCount; i++)
			delete pSPAWN->m_vTMON[i];

		pSPAWN->m_vTMON.clear();
		delete pSPAWN;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCT_MONACTION_ACK(LPPACKETBUF pBUF)
{
	BYTE bChannel;
	WORD wMapID;
	DWORD dwMonID;
	BYTE bAction;
	DWORD dwTriggerID;
	DWORD dwHostID;
	DWORD dwRHID;
	BYTE bRHType;
	WORD wSpawnID = 0;

	pBUF->m_packet
		>> bChannel
		>> wMapID
		>> dwMonID
		>> bAction
		>> dwTriggerID
		>> dwHostID
		>> dwRHID
		>> bRHType
		>> wSpawnID;

	CTMap *pMap = FindTMap( bChannel, 0, wMapID);
	if(!pMap)
		return EC_NOERROR;

	CTMonster* pMon = pMap->FindMonster(dwMonID);
	if(!pMon)
	{
		if(wSpawnID)
		{
			LPTMONSPAWN pTemp = FindTMonSpawn(wSpawnID);
			if(pTemp)
				pMap->AddTimelimitedMon(pTemp, bChannel, TCONTRY_N, RT_ETERNAL);
		}
		return EC_NOERROR;
	}

	if((enum AI_TRIGGER)bAction == AT_DEAD)
	{
		CTObjBase * bOBJ = (CTObjBase*) pMon;
		bOBJ->OnDie(0,OT_NONE,0); 
	}
	else
		pMon->OnEvent((enum AI_TRIGGER)bAction, dwTriggerID, dwHostID, dwRHID, bRHType);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCT_SERVICEDATACLEAR_ACK(LPPACKETBUF pBUF)
{	
	m_mapACTIVEUSER.clear();

	MAPDWORD::iterator finder;
	MAPPLAYER::iterator itP;
	for(itP = m_mapPLAYER.begin(); itP != m_mapPLAYER.end(); itP++)
	{
		finder = m_mapACTIVEUSER.find( (*itP).second->m_dwUserID );
		if(finder == m_mapACTIVEUSER.end())
			m_mapACTIVEUSER.insert(MAPDWORD::value_type( (*itP).second->m_dwUserID, (*itP).second->m_dwUserID ) );
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCT_CTRLSVR_REQ(LPPACKETBUF pBUF)
{
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCT_CASTLEINFO_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	DWORD dwManagerID;
	
	pBUF->m_packet
		>> dwManagerID;

	pPlayer->SendCT_CASTLEINFO_ACK(dwManagerID);

	return EC_NOERROR;
}


/////////////////////////////////////////////////////////////////////////////////

DWORD CTMapSvrModule::OnCS_CONNECT_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	WORD wVersion;

	pBUF->m_packet
		>> wVersion;

	if( wVersion != TVERSION )
	{
		pPlayer->SendCS_CONNECT_ACK(CN_INVALIDVER);
		return EC_SESSION_INVALIDCHAR;
	}

	BYTE bChannel;
	DWORD dwUserID;
	DWORD dwID;
	DWORD dwKEY;
	DWORD dwIPAddr;
	WORD wPort;

	pBUF->m_packet
		>> bChannel
		>> dwUserID
		>> dwID
		>> dwKEY
		>> dwIPAddr
		>> wPort;

	MAPPLAYER::iterator itCHAR = m_mapPLAYER.find(dwID);
	if( itCHAR != m_mapPLAYER.end() )
	{
		CloseSession((*itCHAR).second);

		if((*itCHAR).second->m_bExit)
		{
			MAPPLAYER::iterator itNew = m_mapSUSPENDER.find(dwID);
			if(itNew != m_mapSUSPENDER.end())
			{
				(*itNew).second->m_dwID = 0;
				CloseSession((*itNew).second);
				m_mapSUSPENDER.erase(itNew);
			}

			pPlayer->m_bCloseAll = FALSE;
			pPlayer->m_bChannel = bChannel;
			pPlayer->m_dwUserID = dwUserID;
			pPlayer->m_dwID = dwID;
			pPlayer->m_dwKEY = dwKEY;
			pPlayer->m_dwIPAddr = dwIPAddr;
			pPlayer->m_wPort = wPort;

			m_mapSUSPENDER.insert(MAPPLAYER::value_type(pPlayer->m_dwID, pPlayer));
			return EC_NOERROR;
		}
		else
		{
			(*itCHAR).second->m_bCloseAll = TRUE;
			pPlayer->m_bCloseAll = FALSE;
			return EC_SESSION_INVALIDCHAR;
		}
	}

	pPlayer->m_bCloseAll = TRUE;
	pPlayer->m_bChannel = bChannel;
	pPlayer->m_dwUserID = dwUserID;
	pPlayer->m_dwID = dwID;
	pPlayer->m_dwKEY = dwKEY;
	pPlayer->m_dwIPAddr = dwIPAddr;
	pPlayer->m_wPort = wPort;

	m_mapSUSPENDER.erase(pPlayer->m_dwID);
	m_mapPLAYER.insert( MAPPLAYER::value_type( pPlayer->m_dwID, pPlayer));
	pPlayer->m_dwAcceptTick = 0;

	MAPDWORD::iterator finder = m_mapACTIVEUSER.find(pPlayer->m_dwUserID);
	if(finder == m_mapACTIVEUSER.end())
		m_mapACTIVEUSER.insert(MAPDWORD::value_type(pPlayer->m_dwUserID, pPlayer->m_dwUserID) );

	SendMW_ADDCHAR_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_dwIPAddr,
		pPlayer->m_wPort,
		pPlayer->m_dwUserID);

#ifdef __N_PROTECT_DEBUG
		LogNProtect(pPlayer->m_dwID, 0, _T("User Accept") );
#endif
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CONREADY_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	if(!pPlayer->m_bExit)
	{
		if(!pPlayer->m_pMAP)
			InitMap(pPlayer);
		else if(pPlayer->m_bMain)
			pPlayer->m_pMAP->EnterMAP( pPlayer, FALSE);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_KICKOUT_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	pBUF->m_packet
		>> dwCharID;

	MAPPLAYER::iterator finder = m_mapPLAYER.find(dwCharID);

	BYTE bClear = TRUE;
	if( finder != m_mapPLAYER.end())
	{
		(*finder).second->m_bCloseAll = TRUE;
		CloseSession((*finder).second);
	}

    SendDM_CLEARCURRENTUSER_REQ(dwCharID);

	return EC_SESSION_INVALIDCHAR;
}

DWORD CTMapSvrModule::OnCS_MOVE_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	if(pPlayer->m_bBufFull)
	{
		CloseSession(pPlayer);
//		LogEvent("Send Buffer Full");
		return EC_NOERROR;
	}

	FLOAT fPosX;
	FLOAT fPosZ;
	FLOAT fPosY;

	WORD wMapID;
	WORD wPitch;
	WORD wDIR;

	BYTE bMouseDIR;
	BYTE bKeyDIR;
	BYTE bAction;
	BYTE bGhost;

	pBUF->m_packet
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> wPitch
		>> wDIR
		>> bMouseDIR
		>> bKeyDIR
		>> bAction
		>> bGhost;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	if(!pPlayer->m_dwHP)
	{
		bMouseDIR = TKDIR_N;
		bKeyDIR = TKDIR_N;
	}

	pPlayer->m_dwMoveTick = m_dwTick;

	if(bGhost)
	{
		pPlayer->m_bIsGhost = bGhost;
		pPlayer->MoveGhost(fPosX, fPosZ);

		if(pPlayer->m_bAction == TA_DEAD)
			return EC_NOERROR;
		else
		{
			bAction = TA_DEAD;
			fPosX = pPlayer->m_fPosX;
			fPosY = pPlayer->m_fPosY;
			fPosZ = pPlayer->m_fPosZ;
		}
	}
	else if(pPlayer->m_bIsGhost)
	{
		VTMONSTER vMONS;
		vMONS.clear();

		pPlayer->m_bIsGhost = bGhost;
		pPlayer->m_pMAP->GetNeighbor(
			&vMONS,
			pPlayer->m_fPosX,
			pPlayer->m_fPosZ);

		while(!vMONS.empty())
		{
			CTMonster *pMON = vMONS.back();

			pMON->OnEvent( AT_ENTER, 0, 0, 0, 0);
			vMONS.pop_back();
		}
		vMONS.clear();
	}

	if( !pPlayer->m_bCanHost && bAction != TA_STAND )
	{
		//처음 접속해서 셀에 혼자가 아닐경우는 움직이면 bCanHost Set
		VTMONSTER vMONS;
		vMONS.clear();

		pPlayer->m_bCanHost = TRUE;
		pPlayer->m_pMAP->GetNeighbor(
			&vMONS,
			pPlayer->m_fPosX,
			pPlayer->m_fPosZ);

		while(!vMONS.empty())
		{
			CTMonster *pMON = vMONS.back();

			pMON->OnEvent( AT_ENTER, 0, 0, 0, 0);
			vMONS.pop_back();
		}
		vMONS.clear();
	}

	pPlayer->m_bAction = bAction;

	if(pPlayer->m_bAction != TA_SITDOWN && bAction == TA_SITDOWN)
	{
		pPlayer->m_dwRecoverHPTick = m_dwTick;
		pPlayer->m_dwRecoverMPTick = m_dwTick;
	}

	pPlayer->m_bMouseDIR = bMouseDIR;
	pPlayer->m_bKeyDIR = bKeyDIR;
	pPlayer->m_wPitch = wPitch;
	pPlayer->m_wDIR = wDIR;
	pPlayer->m_fPosY = fPosY;

	pPlayer->m_pMAP->OnMove(
		pPlayer,
		fPosX,
		fPosZ);

//	CheckLocalEnv(pPlayer);

	if(pPlayer->m_bMain)
	{
		VPLAYER vPLAYERS;
		vPLAYERS.clear();

		pPlayer->m_pMAP->GetNeighbor(
			&vPLAYERS,
			pPlayer->m_fPosX,
			pPlayer->m_fPosZ);

		while(!vPLAYERS.empty())
		{
			CTPlayer *pChar = vPLAYERS.back();

			if(pChar->m_dwID != pPlayer->m_dwID)
				pChar->SendCS_MOVE_ACK(
					pPlayer->m_dwID,
					pPlayer->m_fPosX,
					pPlayer->m_fPosY,
					pPlayer->m_fPosZ,
					pPlayer->m_wPitch,
					pPlayer->m_wDIR,
					pPlayer->m_bMouseDIR,
					pPlayer->m_bKeyDIR,
					pPlayer->m_bAction);

			vPLAYERS.pop_back();
		}
		vPLAYERS.clear();

		if(bAction == TA_WALK ||
			bAction == TA_RUN)
			pPlayer->EraseStandHideBuff();

		MAPMAPVQUESTTEMP::iterator itTRIGGER = m_mapTRIGGER.find(TT_POSITION);
		if( itTRIGGER != m_mapTRIGGER.end() )
		{
			MAPVQUESTTEMP::iterator itVQUEST = (*itTRIGGER).second->find(pPlayer->m_wMapID);
			BYTE bLevel;
			if( itVQUEST != (*itTRIGGER).second->end() )
				for( int i=0; i<INT((*itVQUEST).second->size()); i++)
					if(!pPlayer->CanRunQuest( (*(*itVQUEST).second)[i], m_dwTick, bLevel))
					{
						CQuest *pQuest = CQuest::CreateQuest((*(*itVQUEST).second)[i]);

						pQuest->m_fPosX = pPlayer->m_fPosX;
						pQuest->m_fPosY = pPlayer->m_fPosY;
						pQuest->m_dwMonID = 0;
						pQuest->m_pMap = pPlayer->m_pMAP;

						pQuest->ExecQuest(
							pPlayer,
							m_dwTick,
							&m_mapTRIGGER,
							&m_mapQUESTTEMP,
							&m_mapTITEM,
							&m_mapTSKILL,
							&m_mapTMONSTER,
							&m_mapTCLASS);

						delete pQuest;
					}
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_JUMP_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	BYTE bChannel;
	WORD wMapID;

	DWORD dwID;
	BYTE bType;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwID
		>> bType
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	CTObjBase *pTOBJ = NULL;
	switch(bType)
	{
	case OT_MON	:
		pTOBJ = pPlayer->m_pMAP->FindMonster(dwID);
		break;
	case OT_PC	:
		if( pPlayer->m_dwHP && pPlayer->m_dwID == dwID )
			pTOBJ = pPlayer;
		break;
	case OT_RECALL :
		pTOBJ = pPlayer->FindRecallMon(dwID);
		break;
	default :
		pTOBJ = pPlayer->FindSelfObj(dwID);
		break;
	}

	if( !pTOBJ || 
		!pTOBJ->m_pMAP ||
		pTOBJ->m_bAction == TA_DEAD)
		return EC_NOERROR;

	pBUF->m_packet
		>> pTOBJ->m_wPitch
		>> pTOBJ->m_wDIR
		>> pTOBJ->m_bAction;
	pTOBJ->m_fPosY = fPosY;

	if( bType == OT_PC )
	{
		pPlayer->m_pMAP->OnMove(
			pPlayer,
			fPosX,
			fPosZ);
	}
	else
	{
		((CTMonster *) pTOBJ)->m_pMAP->OnMove(
			(CTMonster *) pTOBJ,
			fPosX,
			fPosZ);
	}

	switch(bType)
	{
	case OT_RECALL	:
		if(!((CTRecallMon *) pTOBJ)->m_bMain)
			return EC_NOERROR;

		break;

	case OT_PC		:
		if(!pPlayer->m_bMain)
			return EC_NOERROR;

		break;
	case OT_SELF	:
		if(!((CTSelfObj *)pTOBJ)->m_pMAP)
			return EC_NOERROR;

		break;
	}

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	pPlayer->m_pMAP->GetNeerPlayer(
		&vPLAYERS,
		pTOBJ->m_fPosX,
		pTOBJ->m_fPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_JUMP_ACK(
			pTOBJ->m_dwID,
			pTOBJ->m_bType,
			pTOBJ->m_fPosX,
			pTOBJ->m_fPosY,
			pTOBJ->m_fPosZ,
			pTOBJ->m_wPitch,
			pTOBJ->m_wDIR,
			pTOBJ->m_bAction);

		vPLAYERS.pop_back();
	}
	vPLAYERS.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_BLOCK_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	BYTE bChannel;
	WORD wMapID;

	DWORD dwID;
	BYTE bType;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwID
		>> bType
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	CTObjBase *pTOBJ = NULL;
	switch(bType)
	{
	case OT_MON	:
		pTOBJ = pPlayer->m_pMAP->FindMonster(dwID);
		break;
	case OT_PC	:
		if( pPlayer->m_dwHP && pPlayer->m_dwID == dwID )
			pTOBJ = pPlayer;
		break;
	case OT_RECALL:
        pTOBJ = pPlayer->FindRecallMon(dwID);
		break;
	default:
		pTOBJ = pPlayer->FindSelfObj(dwID);
		break;
	}

	if( !pTOBJ ||
		!pTOBJ->m_pMAP ||
		pTOBJ->m_bAction == TA_DEAD )
		return EC_NOERROR;

	pBUF->m_packet
		>> pTOBJ->m_wPitch
		>> pTOBJ->m_wDIR
		>> pTOBJ->m_bAction
		>> pTOBJ->m_bBlock;
	pTOBJ->m_fPosY = fPosY;

	if( bType == OT_PC )
	{
		pPlayer->m_pMAP->OnMove(
			pPlayer,
			fPosX,
			fPosZ);
	}
	else
	{
		((CTMonster *) pTOBJ)->m_pMAP->OnMove(
			(CTMonster *) pTOBJ,
			fPosX,
			fPosZ);
	}

	switch(bType)
	{
	case OT_RECALL	:
		if(!((CTRecallMon *) pTOBJ)->m_bMain)
			return EC_NOERROR;

		break;

	case OT_PC		:
		if(!pPlayer->m_bMain)
			return EC_NOERROR;

		break;
	}

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	pPlayer->m_pMAP->GetNeighbor(
		&vPLAYERS,
		pTOBJ->m_fPosX,
		pTOBJ->m_fPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_BLOCK_ACK(
			pTOBJ->m_dwID,
			pTOBJ->m_bType,
			pTOBJ->m_fPosX,
			pTOBJ->m_fPosY,
			pTOBJ->m_fPosZ,
			pTOBJ->m_wPitch,
			pTOBJ->m_wDIR,
			pTOBJ->m_bAction,
			pTOBJ->m_bBlock);

		vPLAYERS.pop_back();
	}
	vPLAYERS.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_MONMOVE_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	DWORD dwMonID;
	BYTE bObjType;
	BYTE bChannel;

	WORD wMapID;
	WORD wPitch;
	WORD wDIR;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	BYTE bMouseDIR;
	BYTE bKeyDIR;
	BYTE bAction;
	WORD wMonCount;

	MAPTMONSTER mapMon;
	mapMon.clear();

	pBUF->m_packet
		>> wMonCount;

	for(WORD wC=0; wC<wMonCount; wC++)
	{
		pBUF->m_packet
			>> dwMonID
			>> bObjType
			>> bChannel
			>> wMapID
			>> fPosX
			>> fPosY
			>> fPosZ
			>> wPitch
			>> wDIR
			>> bMouseDIR
			>> bKeyDIR
			>> bAction;

		CTMonster *pMON = NULL;
		switch(bObjType)
		{
		case OT_MON		: pMON = pPlayer->m_pMAP->FindMonster(dwMonID); break;
		case OT_RECALL	: pMON = pPlayer->FindRecallMon(dwMonID); break;
		case OT_SELF	: pMON = pPlayer->FindSelfObj(dwMonID);	break;
		}

		if( !pMON ||
			!pMON->m_pMAP ||
			pMON->m_dwHostID != pPlayer->m_dwID )
			continue;

		if(!pMON->m_dwHP)
		{
			bMouseDIR = TKDIR_N;
			bKeyDIR = TKDIR_N;
		}

		if(pMON->m_bAction == TA_DEAD)
			continue;

		pMON->m_bMouseDIR = bMouseDIR;
		pMON->m_bKeyDIR = bKeyDIR;
		pMON->m_bAction = bAction;
		pMON->m_wPitch = wPitch;
		pMON->m_wDIR = wDIR;

		pMON->m_fPosY = fPosY;
		pMON->m_pMAP->OnMove(
			pMON,
			fPosX,
			fPosZ);

		if(pMON->m_bAction != TA_STAND &&
			bAction == TA_STAND)
			pMON->m_dwRecoverMPTick = m_dwTick + RECOVER_INIT;

		if(pMON->m_bType == OT_MON)
		{
			if(pMON->m_bMode == MT_BATTLE)
			{
				CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
				CTObjBase * pTarget;
				if(pMON->m_bTargetType == OT_PC)
					pTarget = pPlayer;
				else if(pMON->m_bTargetType == OT_RECALL)
					pTarget = pPlayer->FindRecallMon(pMON->m_dwTargetID);
				else
					pTarget = pPlayer->FindSelfObj(pMON->m_dwTargetID);

				if(!pTarget || !pTarget->m_pMAP)
				{
					pMON->LeaveAggro(pMON->m_dwHostID, pMON->m_dwTargetID, pMON->m_bTargetType);
					continue;
				}

				if( pMON->m_pMON->m_wChaseRange < GetDistance(pMON->m_fStartX, pMON->m_fStartZ, pMON->m_fPosX, pMON->m_fPosZ))
					pMON->LeaveAggro(pMON->m_dwHostID, pMON->m_dwTargetID, pMON->m_bTargetType);
			}
			else if(pMON->m_bMode == MT_GOHOME)
			{
				if( GetDistance(pMON->m_fStartX, pMON->m_fStartZ, pMON->m_fPosX, pMON->m_fPosZ) <= ZONE_HOMESIZE )
					pMON->OnEvent(AT_ATHOME, 0, pMON->m_dwHostID, pMON->m_dwTargetID, pMON->m_bTargetType);
			}
			else
			{
				if(	GetDistance(pMON->m_fStartX, pMON->m_fStartZ, pMON->m_fPosX, pMON->m_fPosZ) > MAX_ROAMRANGE )
					pMON->OnDie(0,OT_NONE,0);
			}
		}

		if(pMON->m_bStatus != OS_DEAD)
			mapMon.insert(MAPTMONSTER::value_type(pMON->m_dwID, pMON));
	}

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	pPlayer->m_pMAP->GetNeighbor(
		&vPLAYERS,
		pPlayer->m_fPosX,
		pPlayer->m_fPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		if(pChar->m_dwID != pPlayer->m_dwID)
			pChar->SendCS_MONMOVE_ACK(&mapMon);

		vPLAYERS.pop_back();
	}
	vPLAYERS.clear();
	mapMon.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_REVIVAL_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;
	BYTE bType;

	pBUF->m_packet
		>> fPosX
		>> fPosY
		>> fPosZ
		>> bType;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	pPlayer->m_bStatus = OS_WAKEUP;
	pPlayer->m_bAction = TA_STAND;

	pPlayer->m_fPosY = fPosY;
	pPlayer->m_pMAP->OnMove(
		pPlayer,
		fPosX,
		fPosZ);

	if(pPlayer->m_bMain)
		pPlayer->Revival(bType == REVIVAL_NPC ? AFTERMATH_ATONCE : AFTERMATH_GHOST, NULL, 0);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_ENTERLB_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwTargetID;
	BYTE bTargetType;
	DWORD dwMonID;

	BYTE bChannel;
	WORD wMapID;

	pBUF->m_packet
		>> dwCharID
		>> dwTargetID
		>> bTargetType
		>> dwMonID
		>> bChannel
		>> wMapID;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	CTMonster *pMON = pPlayer->m_pMAP->FindMonster(dwMonID);
	if(pMON)
		pMON->OnEvent( AT_ENTERLB, 0, dwCharID, dwTargetID, bTargetType);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_LEAVELB_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwTargetID;
	BYTE bTargetType;
	DWORD dwMonID;

	BYTE bChannel;
	WORD wMapID;

	pBUF->m_packet
		>> dwCharID
		>> dwTargetID
		>> bTargetType
		>> dwMonID
		>> bChannel
		>> wMapID;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	CTMonster *pMON = pPlayer->m_pMAP->FindMonster(dwMonID);
	if(pMON)
		pMON->LeaveAggro(dwCharID, dwTargetID, bTargetType);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_ENTERAB_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwTargetID;
	BYTE bTargetType;
	DWORD dwMonID;

	BYTE bChannel;
	WORD wMapID;

	pBUF->m_packet
		>> dwCharID
		>> dwTargetID
		>> bTargetType
		>> dwMonID
		>> bChannel
		>> wMapID;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	CTMonster *pMON = pPlayer->m_pMAP->FindMonster(dwMonID);
	if(pMON)
		pMON->OnEvent( AT_ENTERAB, 0, dwCharID, dwTargetID, bTargetType);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_LEAVEAB_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	DWORD dwTargetID;
	BYTE bTargetType;
	DWORD dwMonID;

	BYTE bChannel;
	WORD wMapID;

	pBUF->m_packet
		>> dwCharID
		>> dwTargetID
		>> bTargetType
		>> dwMonID
		>> bChannel
		>> wMapID;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	CTMonster *pMON = pPlayer->m_pMAP->FindMonster(dwMonID);
	if(pMON)
		pMON->OnEvent( AT_LEAVEAB, 0, dwCharID, dwTargetID, bTargetType);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CHGMODE_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	pBUF->m_packet >> pPlayer->m_bMode;
	pPlayer->ChgMode(pPlayer->m_bMode);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_ACTION_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	DWORD dwObjID;
	DWORD dwActID;
	DWORD dwAniID;

	BYTE bActionID;
	BYTE bObjType;
	BYTE bChannel;

	WORD wMapID;
	WORD wSkillID;

	pBUF->m_packet
		>> dwObjID
		>> bObjType
		>> bActionID
		>> dwActID
		>> dwAniID
		>> bChannel
		>> wMapID
		>> wSkillID;
	BYTE bResult = SKILL_SUCCESS;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	CTObjBase *pOBJ = NULL;
	switch(bObjType)
	{
	case OT_MON	:
			pOBJ = pPlayer->m_pMAP->FindMonster(dwObjID);
		break;

	case OT_RECALL:
		{
			CTRecallMon * pRecall =  pPlayer->FindRecallMon(dwObjID);
			if(pRecall && pRecall->m_bMain)
				pOBJ = pRecall;
		}
		break;
	case OT_PC	:
		{
			if(pPlayer->m_bMain && pPlayer->m_pMAP)
				pOBJ = pPlayer;
		}
		break;
	case OT_SELF:
		pOBJ = pPlayer->FindSelfObj(dwObjID);
		break;
	}

	if(pOBJ && pOBJ->m_pMAP)
	{
		if(wSkillID && bObjType == OT_PC)
		{
			CTSkill * pSkill = pOBJ->FindTSkill(wSkillID);

			if(!pSkill)
				bResult = SKILL_NOTFOUND;
			else if( !pSkill->CanUse(m_dwTick))
				bResult = SKILL_SPEEDYUSE;
			else if( pOBJ->m_dwMP < pSkill->GetRequiredMP(pOBJ->GetPureMaxMP()))
				bResult = SKILL_NEEDMP;
			else if(pOBJ->m_dwHP < pSkill->GetRequiredHP(pOBJ->GetPureMaxHP()))
				bResult = SKILL_NEEDHP;
			else if(!pOBJ->CheckPrevAct(pSkill->m_pTSKILL->m_wPrevActiveID))
				bResult = SKILL_NEEDPREVACT;
			else
			{
				for(DWORD i=0; i<pSkill->m_pTSKILL->m_vData.size(); i++)
				{
					LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[i];
					switch(pData->m_bType)
					{
					case SDT_ABILITY:
						break;
					case SDT_TRAP:
					case SDT_RECALL:
						{
							WORD wRecMon;
							if(pData->m_bExec == SER_MONSTER)
							{
								if(pPlayer->m_wTemptedMon == 0)
								{
									bResult = SKILL_NEEDPREVACT;
									break;
								}
								else
									wRecMon = pPlayer->m_wTemptedMon;
							}
							else
								wRecMon = WORD(pSkill->m_pTSKILL->GetValue(pData, pSkill->m_bLevel));

							MAPTMONSTERTEMP::iterator itMon = m_mapTMONSTER.find(wRecMon);
							if(wRecMon && itMon!=m_mapTMONSTER.end())
							{
								pPlayer->CheckRecallMon((*itMon).second, FALSE);
								break;
							}
						}
					case SDT_TRANS:
						break;
					case SDT_CURE:
						break;
					case SDT_STATUS:
						break;
					case SDT_AI:
						break;
					}
				}

				if(pSkill->m_pTSKILL->m_dwActionTime)
				{
					pSkill->m_dwChargeTick = m_dwTick;
					pOBJ->m_wCurChargeSkill = wSkillID;
				}

				pPlayer->EraseBuffByAttack(pSkill->m_pTSKILL);
			}
		}

		VPLAYER vPLAYERS;
		vPLAYERS.clear();

		if(wSkillID)
		{
			pPlayer->m_pMAP->GetNeerPlayer(
				&vPLAYERS,
				pOBJ->m_fPosX,
				pOBJ->m_fPosZ);
		}
		else
		{
			pPlayer->m_pMAP->GetNeighbor(
				&vPLAYERS,
				pOBJ->m_fPosX,
				pOBJ->m_fPosZ);
		}

		while(!vPLAYERS.empty())
		{
			CTPlayer *pChar = vPLAYERS.back();

			pChar->SendCS_ACTION_ACK(
				bResult,
				pOBJ->m_dwID,
				pOBJ->m_bType,
				bActionID,
				dwActID,
				dwAniID,
				wSkillID);

			vPLAYERS.pop_back();
		}
		vPLAYERS.clear();
	}
	else if(bObjType == OT_PC && wSkillID)
	{
		CTSkillTemp * pTSKILL = FindTSkill(wSkillID);
		if(!pTSKILL)
			return EC_NOERROR;

		for(DWORD i=0; i<pTSKILL->m_vData.size(); i++)
		{
			LPTSKILLDATA pData = pTSKILL->m_vData[i];
			if(pData->m_bType == SDT_RECALL ||
				pData->m_bType == SDT_TRAP)
			{
				WORD wRecMon;
				if(pData->m_bExec == SER_MONSTER)
					wRecMon = pPlayer->m_wTemptedMon;
				else
					wRecMon = pData->m_wValue;

				MAPTMONSTERTEMP::iterator itMon = m_mapTMONSTER.find(wRecMon);
				if(wRecMon && itMon!=m_mapTMONSTER.end())
				{
					pPlayer->CheckRecallMon((*itMon).second, FALSE);
					break;
				}
			}
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_DEFEND_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	DWORD dwHostID;
	DWORD dwAttackID;
	DWORD dwTargetID;
	DWORD dwRemainTick;

	BYTE bAttackType;
	BYTE bTargetType;

	WORD wAttackPartyID;
	BYTE bAttackCountry;
	BYTE bAttackAidCountry;

	DWORD dwActID;
	DWORD dwAniID;
	
	DWORD dwPysMinPower;
	DWORD dwPysMaxPower;
	DWORD dwMgMinPower;
	DWORD dwMgMaxPower;

	WORD wTransHP;
	WORD wTransMP;
	BYTE bCurseProb;

	BYTE bEquipSpecial;
	BYTE bCanSelect;

	WORD wAttackLevel;
	BYTE bAttackerLevel;
	BYTE bCP;

	BYTE bChannel;
	WORD wMapID;
	WORD wSkillID;
	BYTE bSkillLevel;

	FLOAT fAtkPosX;
	FLOAT fAtkPosY;
	FLOAT fAtkPosZ;
	FLOAT fDefPosX;
	FLOAT fDefPosY;
	FLOAT fDefPosZ;

	pBUF->m_packet
		>> dwHostID
		>> dwAttackID
		>> dwTargetID
		>> bAttackType
		>> bTargetType
		>> wAttackPartyID
		>> dwActID
		>> dwAniID
		>> bChannel
		>> wMapID
		>> bAttackerLevel
		>> dwPysMinPower
		>> dwPysMaxPower
		>> dwMgMinPower
		>> dwMgMaxPower
		>> wTransHP
		>> wTransMP
		>> bCurseProb
		>> bEquipSpecial
		>> bCanSelect
		>> bAttackCountry
		>> bAttackAidCountry
		>> wAttackLevel
		>> bCP
		>> wSkillID
		>> bSkillLevel
		>> fAtkPosX
		>> fAtkPosY
		>> fAtkPosZ
		>> fDefPosX
		>> fDefPosY
		>> fDefPosZ
		>> dwRemainTick;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	if(bAttackType == OT_MON)
		dwHostID = pPlayer->m_dwID;

	CTSkillTemp *pTemp = FindTSkill(wSkillID);
	if(!pTemp)
		return EC_NOERROR;

	if(pTemp->m_bRunFromServer == 1 ||
		(pTemp->m_wMapID != INVALID_MAPID && pTemp->m_wMapID != pPlayer->m_wMapID))
		return EC_NOERROR;

	CTPlayer * pAtkHost = NULL;
	MAPPLAYER::iterator itPl = m_mapPLAYER.find(dwHostID);
	if(itPl != m_mapPLAYER.end())
		pAtkHost = (*itPl).second;

	if(pAtkHost)
	{
		if((pAtkHost->m_wArenaID || pPlayer->m_wArenaID) &&
		   (pAtkHost->m_wArenaID != pPlayer->m_wArenaID))
		   return EC_NOERROR;
	}

	CTObjBase *pATTACK = FindTarget(pAtkHost, bAttackType, dwAttackID);

	if(!pTemp->m_bRunFromServer)
	{
		if(pATTACK)
		{
			CTSkill * pAtkSkill = pATTACK->FindTSkill(pTemp->m_wTriggerID);
			if(!pAtkSkill)
				return EC_NOERROR;

			if(pTemp->m_bCheckAttacker == 3 &&
				bAttackType == bTargetType &&
				dwAttackID == dwTargetID)
				return EC_NOERROR;

			bSkillLevel = pAtkSkill->m_bLevel;
			bCP = pATTACK->GetCritical(pAtkSkill);
			bAttackCountry = pATTACK->m_bCountry;
			bAttackAidCountry = pATTACK->m_bAidCountry;

			BYTE bIsPysLong = pTemp->IsLongAttack();
			if(bAttackType == OT_SELF && ((CTSelfObj *)pATTACK)->m_pMON->m_bRecallType == TRECALLTYPE_SKILL)
			{
				if(pAtkHost && pAtkHost->m_bMain)
				{
					dwPysMinPower = pAtkHost->GetMinAP(bIsPysLong);
					dwPysMaxPower = pAtkHost->GetMaxAP(bIsPysLong);
					dwMgMinPower = pAtkHost->GetMinMagicAP();
					dwMgMaxPower = pAtkHost->GetMaxMagicAP();
					wAttackLevel = pAtkSkill->m_pTSKILL->GetAttackType() == SAT_PHYSIC ? pAtkHost->GetAttackLevel() : pAtkHost->GetMagicAtkLevel();
				}
			}
			else
			{
				if(pTemp->m_wTriggerID == wSkillID)
					pATTACK->m_pInstanceSkill = pAtkSkill;

				dwPysMinPower = pATTACK->GetMinAP(bIsPysLong);
				dwPysMaxPower = pATTACK->GetMaxAP(bIsPysLong);
				dwMgMinPower = pATTACK->GetMinMagicAP();
				dwMgMaxPower = pATTACK->GetMaxMagicAP();
				wAttackLevel = pAtkSkill->m_pTSKILL->GetAttackType() == SAT_PHYSIC ? pATTACK->GetAttackLevel() : pATTACK->GetMagicAtkLevel();
				pATTACK->m_pInstanceSkill = NULL;
			}
		}
		else
		{
			if(bTargetType == OT_MON)
				return EC_NOERROR;

			if(pTemp->m_bCheckAttacker == 1)
				return EC_NOERROR;

			if(!pAtkHost && pTemp->m_bCheckAttacker)
				return EC_NOERROR;
		}
	}

	CTSelfObj * pSelf = NULL;
	CTObjBase *pDEFEND = FindTarget(pPlayer, bTargetType, dwTargetID);

	// 결투
	if(pDEFEND && bTargetType == OT_PC)
	{
		if(!((CTPlayer*)pDEFEND)->CanDuel(pAtkHost))
			return EC_NOERROR;
	}

	if(bAttackType == OT_SELF)
	{
		if(pAtkHost)
		{
			pSelf = pAtkHost->FindSelfObj(dwAttackID);
			if(!pSelf)
				return EC_NOERROR;
		}
	}

	if( pDEFEND )
	{
		if(bTargetType == OT_MON &&
			pDEFEND->m_bCountry != TCONTRY_N &&
			(GetAttackCountry(bAttackCountry, bAttackAidCountry) == TCONTRY_B ||
			GetAttackCountry(bAttackCountry, bAttackAidCountry) == pDEFEND->m_bCountry) &&
			pTemp->IsNegative())
			return EC_NOERROR;

		if(bAttackType == pDEFEND->m_bType &&
			dwAttackID == pDEFEND->m_dwID &&
			pPlayer->m_dwRiding)
			return EC_NOERROR;

		if(bAttackType != OT_MON &&
			pTemp->IsNegative() &&
			CheckPeaceZone(pPlayer))
			return EC_NOERROR;

		if(pDEFEND->m_bStatus == OS_DEAD &&
			!pTemp->CanDefendAtDie())
			return EC_NOERROR;

		if(!pDEFEND->CheckPrevAct(pTemp->m_wTargetActiveID))
			return EC_NOERROR;

		if(pTemp->IsRandomTrans())
		{
			pTemp = RandTransSkill(pTemp);
			if(!pTemp)
				return EC_NOERROR;
		}
		if(pTemp->IsRandomBuff())  
		{
			pTemp = RandBuffSkill(pTemp);
			if(!pTemp)
				return EC_NOERROR;
		}

		if( bAttackType != OT_MON &&
			pDEFEND->m_bType != OT_MON &&
			pDEFEND->m_pLocal && 
			pTemp->IsNegative() &&
			!pDEFEND->m_pLocal->m_bCanBattle &&
			(bAttackType != pDEFEND->m_bType ||
			dwAttackID != pDEFEND->m_dwID))
			return EC_NOERROR;

		BYTE bMirror = FALSE;
		BYTE bHitType = pDEFEND->GetAtkHitType(pTemp, bCP, wAttackLevel, bSkillLevel, bAttackerLevel, bEquipSpecial);
		BYTE bAttackClass = TCLASS_COUNT;
		if(bAttackType == OT_PC && pAtkHost)
			bAttackClass = pAtkHost->m_bClass;

		if(bHitType == HT_MISS &&
			pDEFEND->HaveMirror() &&
			(bAttackType != pDEFEND->m_bType ||
			dwAttackID != pDEFEND->m_dwID) &&
			pTemp->IsNegative() &&
			pTemp->GetAttackType() == SAT_PHYSIC)
		{	// 마법거울

			wAttackPartyID = pDEFEND->GetPartyID();
			bAttackCountry = pDEFEND->m_bCountry;
			bAttackAidCountry = pDEFEND->m_bAidCountry;

			if(pAtkHost)
				pDEFEND = FindTarget(pAtkHost, bAttackType, dwAttackID);
			else
				pDEFEND = NULL;

			if(pDEFEND)
			{
				if((bAttackType == OT_SELF || bAttackType == OT_RECALL) &&
					!((CTMonster *)pDEFEND)->m_pMON->m_bCanSelect)
					return EC_NOERROR;
				
				dwAttackID = dwTargetID;
				bAttackType = bTargetType;
				bHitType = HT_NORMAL;
				dwHostID = pPlayer->m_dwID;

				if(m_bNation == NATION_US)
					bMirror = TRUE;

			}
			else
			{
				if(bAttackType == OT_PC)
					SendMW_MAGICMIRROR_ACK(wAttackPartyID, bAttackCountry, bAttackAidCountry, bAttackClass, &pBUF->m_packet);

				return EC_NOERROR;
			}
		}

		pDEFEND->Defend(
			pTemp,
			bSkillLevel,
			dwHostID,
			dwAttackID,
			bAttackType,
			wAttackPartyID,
			bAttackCountry,
			bAttackAidCountry,
			bAttackClass,
			dwActID,
			dwAniID,
			dwPysMinPower,
			dwPysMaxPower,
			dwMgMinPower,
			dwMgMaxPower,
			wTransHP,
			wTransMP,
			bCurseProb,
			bEquipSpecial,
			bCanSelect,
			wAttackLevel,
			bAttackerLevel,
			bCP,
			bHitType,
			dwRemainTick,
			fAtkPosX,fAtkPosY,fAtkPosZ,
			fDefPosX,fDefPosY,fDefPosZ,
			bMirror);

		if(pATTACK &&
			pATTACK->m_bType == OT_RECALL &&
			((CTRecallMon *)pATTACK)->m_pMON->m_bRecallType == TRECALLTYPE_MINE)
		{
			pATTACK->OnDie(0, 0, 0);
			SendMW_RECALLMONDEL_ACK(dwHostID, 0, pATTACK->m_dwID);
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_MOVEITEM_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	if( !pPlayer->m_pMAP || !pPlayer->m_bMain || pPlayer->m_bStatus == OS_DEAD)
		return EC_NOERROR;

	if( pPlayer->m_dealItem.m_bStatus >= DEAL_START)
	{
		pPlayer->SendCS_MOVEITEM_ACK(MI_DEALING);
		return EC_NOERROR;
	}

	if( pPlayer->m_bStore )
		return EC_NOERROR;

	BYTE bInvenSRC;
	BYTE bSRCItemID;
	BYTE bInvenDEST;
	BYTE bDESTItemID;
	BYTE bCount;

	pBUF->m_packet
		>> bInvenSRC
		>> bSRCItemID
		>> bInvenDEST
		>> bDESTItemID
		>> bCount;

	CTInven *pTInvenDEST = pPlayer->FindTInven(bInvenDEST);
	CTInven *pTInvenSRC = pPlayer->FindTInven(bInvenSRC);

	if(!pTInvenSRC)
	{
		pPlayer->SendCS_MOVEITEM_ACK(MI_NOSRCINVEN);
		return EC_NOERROR;
	}

	CTItem *pTItemSRC = pTInvenSRC->FindTItem(bSRCItemID);
	if(!pTItemSRC || !pTItemSRC->m_bCount || !bCount)
	{
		pPlayer->SendCS_MOVEITEM_ACK(MI_NOSRCITEM);
		return EC_NOERROR;
	}

	CTItem *pTItemDEST = NULL;

	if(bInvenDEST == INVEN_NULL)
	{
		if(pTItemSRC->m_pTITEM->m_bType == IT_INVEN && pTItemSRC->m_pTITEM->m_bSpecial)
		{
			pPlayer->SendCS_MOVEITEM_ACK(MI_CANTDROP);
			return EC_NOERROR;
		}

		pPlayer->SendCS_DELITEM_ACK(
			pTInvenSRC->m_bInvenID,
			pTItemSRC->m_bItemID);

#ifdef DEF_UDPLOG		
		m_pUdpSocket->LogItemUpgrade(LOGMAP_ITEMDEL, pPlayer, pTItemSRC);
#endif	//	DEF_UDPLOG

		pTInvenSRC->m_mapTITEM.erase(pTItemSRC->m_bItemID);
		pPlayer->CheckQuest(
			m_dwTick,
			pTItemSRC->m_pTITEM->m_wItemID,
			QTT_GETITEM,
			0);

		delete pTItemSRC;
		pTItemSRC = NULL;
	}
	else
	{
		if(!pTInvenDEST)
		{
			pPlayer->SendCS_MOVEITEM_ACK(MI_NODESTINVEN);
			return EC_NOERROR;
		}

		if( pTInvenSRC == pTInvenDEST &&
			bSRCItemID == bDESTItemID )
		{
			pPlayer->SendCS_MOVEITEM_ACK(MI_SAMEPOS);
			return EC_NOERROR;
		}

		if( bInvenDEST == INVEN_EQUIP )
		{
			if( bDESTItemID == pTItemSRC->m_pTITEM->m_bSubSlotID )
				bDESTItemID = pTItemSRC->m_pTITEM->m_bPrmSlotID;

			bCount = 1;
		}

		if( bInvenSRC == INVEN_EQUIP )
			bCount = 1;

		pTItemDEST = pTInvenDEST->FindTItem(bDESTItemID);
		if(!pTItemDEST &&
			bInvenDEST == INVEN_EQUIP &&
			bDESTItemID == ES_SNDWEAPON)
		{
			CTItem * pPrmWeapon = pTInvenDEST->FindTItem(ES_PRMWEAPON);
			if(pPrmWeapon && pPrmWeapon->m_pTITEM->m_bSubSlotID == bDESTItemID)
			{
				pPlayer->SendCS_MOVEITEM_ACK(MI_BOTHHANDWEAPON);
				return EC_NOERROR;
			}
		}

		if( pTItemDEST && bInvenSRC == INVEN_EQUIP && bInvenDEST != INVEN_EQUIP )
		{
			CTInven *pTINVEN = pTInvenDEST;
			CTItem *pTITEM = pTItemDEST;
			BYTE bInvenID = bInvenDEST;

			pTInvenDEST = pTInvenSRC;
			pTInvenSRC = pTINVEN;

			bInvenDEST = bInvenSRC;
			bInvenSRC = bInvenID;

			pTItemDEST = pTItemSRC;
			pTItemSRC = pTITEM;

			bDESTItemID = pTItemDEST->m_bItemID;
			bSRCItemID = pTItemSRC->m_bItemID;
		}

		if(bInvenDEST == INVEN_EQUIP)
		{
			BYTE bResult = pPlayer->CanEquip( pTItemSRC, bDESTItemID);
			if(bResult != MI_SUCCESS)
			{
				pPlayer->SendCS_MOVEITEM_ACK(bResult);
				return EC_NOERROR;
			}
		}
		if(pTItemDEST && bInvenSRC == INVEN_EQUIP)
		{
			BYTE bResult = pPlayer->CanEquip( pTItemDEST, bSRCItemID);
			if(bResult != MI_SUCCESS)
			{
				pPlayer->SendCS_MOVEITEM_ACK(bResult);
				return EC_NOERROR;
			}
		}

		VTITEM vTITEM;	
		vTITEM.clear();

		if( bInvenDEST == INVEN_EQUIP )
		{
			if( pTItemDEST && pTItemSRC->m_bCount > 1 &&
				(*pTItemDEST) != (*pTItemSRC) )
				vTITEM.push_back(pTItemDEST);

			if( pTItemSRC->m_pTITEM->m_bSubSlotID != INVALID_SLOT )
			{
				CTItem *pTSUB = pTInvenDEST->FindTItem(pTItemSRC->m_pTITEM->m_bSubSlotID);

				if(pTSUB)
					vTITEM.push_back(pTSUB);
			}
		}

		if( bInvenSRC == INVEN_EQUIP && pTItemDEST &&
			pTItemDEST->m_pTITEM->m_bSubSlotID != INVALID_SLOT )
		{
			CTItem *pTSUB = pTInvenSRC->FindTItem(pTItemDEST->m_pTITEM->m_bSubSlotID);

			if(pTSUB)
				vTITEM.push_back(pTSUB);
		}

		if(!pPlayer->CanPush( &vTITEM, 0))
		{
			pPlayer->SendCS_MOVEITEM_ACK(MI_INVENFULL);
			vTITEM.clear();

			return EC_NOERROR;
		}

		if(!vTITEM.empty())
		{
			CTInven *pTEQUIP = pPlayer->FindTInven(INVEN_EQUIP);

			for( int i=0; i<INT(vTITEM.size()); i++)
			{
				pPlayer->SendCS_DELITEM_ACK(
					INVEN_EQUIP,
					vTITEM[i]->m_bItemID);

				pTEQUIP->m_mapTITEM.erase(vTITEM[i]->m_bItemID);
			}

			pPlayer->PushTItem(&vTITEM);
		}

		vTITEM.clear();

		pTItemDEST = pTInvenDEST->FindTItem(bDESTItemID);
		bCount = min( pTItemSRC->m_bCount, bCount);

		if(!pTItemDEST)
		{
			pTItemDEST = new CTItem();

			pTItemDEST->Copy(pTItemSRC, pTItemSRC->m_bCount-bCount);
			pTItemDEST->m_bItemID = bDESTItemID;

			pTItemDEST->m_bCount = bCount;
			pTItemSRC->m_bCount -= bCount;

			if(!pTItemSRC->m_bCount)
			{
				pPlayer->SendCS_DELITEM_ACK(
					pTInvenSRC->m_bInvenID,
					pTItemSRC->m_bItemID);

				pTInvenSRC->m_mapTITEM.erase(pTItemSRC->m_bItemID);
				delete pTItemSRC;
				pTItemSRC = NULL;
			}
			else
			{
				pPlayer->SendCS_UPDATEITEM_ACK(
					pTItemSRC,
					pTInvenSRC->m_bInvenID);
			}

			pTInvenDEST->m_mapTITEM.insert( MAPTITEM::value_type( pTItemDEST->m_bItemID, pTItemDEST));
			pPlayer->SendCS_ADDITEM_ACK(
				pTItemDEST,
				pTInvenDEST->m_bInvenID);
		}
		else if( (*pTItemDEST) != (*pTItemSRC) )
		{
			BYTE bItemID = pTItemDEST->m_bItemID;

			pTItemDEST->m_bItemID = pTItemSRC->m_bItemID;
			pTItemSRC->m_bItemID = bItemID;

			pTInvenDEST->m_mapTITEM.erase(pTItemSRC->m_bItemID);
			pTInvenSRC->m_mapTITEM.erase(pTItemDEST->m_bItemID);

			pTInvenSRC->m_mapTITEM.insert( MAPTITEM::value_type( pTItemDEST->m_bItemID, pTItemDEST));
			pTInvenDEST->m_mapTITEM.insert( MAPTITEM::value_type( pTItemSRC->m_bItemID, pTItemSRC));

			pPlayer->SendCS_UPDATEITEM_ACK(
				pTItemSRC,
				pTInvenDEST->m_bInvenID);

			pPlayer->SendCS_UPDATEITEM_ACK(
				pTItemDEST,
				pTInvenSRC->m_bInvenID);
		}
		else if( bInvenDEST != INVEN_EQUIP )
		{
			bCount = min( pTItemDEST->m_pTITEM->m_bStack - pTItemDEST->m_bCount, bCount);

			pTItemDEST->m_bCount += bCount;
			pTItemSRC->m_bCount -= bCount;

			if(!pTItemSRC->m_bCount)
			{
				pPlayer->SendCS_DELITEM_ACK(
					pTInvenSRC->m_bInvenID,
					pTItemSRC->m_bItemID);

				pTInvenSRC->m_mapTITEM.erase(pTItemSRC->m_bItemID);
				delete pTItemSRC;
				pTItemSRC = NULL;
			}
			else
			{
				pPlayer->SendCS_UPDATEITEM_ACK(
					pTItemSRC,
					pTInvenSRC->m_bInvenID);
			}

			pPlayer->SendCS_UPDATEITEM_ACK(
				pTItemDEST,
				pTInvenDEST->m_bInvenID);
		}
	}

	if( pPlayer->m_pMAP && 
		(bInvenDEST == INVEN_EQUIP ||
		bInvenSRC == INVEN_EQUIP) )
	{
		if(pPlayer->m_pLocal && pPlayer->m_pLocal->m_pZone->m_bItemLevel)
		{
			if(!pTItemSRC && pTItemDEST)
			{
				if(bInvenDEST == INVEN_EQUIP)
					SetItemAttr(pTItemDEST, min(pPlayer->m_pLocal->m_pZone->m_bItemLevel, pTItemDEST->m_bLevel));
				else
					SetItemAttr(pTItemDEST, pTItemDEST->m_bLevel);
			}
			else
			{
				if(bInvenDEST == INVEN_EQUIP && pTItemSRC)
					SetItemAttr(pTItemSRC, min(pPlayer->m_pLocal->m_pZone->m_bItemLevel, pTItemSRC->m_bLevel));
				if(bInvenSRC != INVEN_EQUIP && pTItemDEST)
					SetItemAttr(pTItemDEST, pTItemDEST->m_bLevel);
			}
		}

		pPlayer->ChangeEquipItem();
	}

	pPlayer->SendCS_MOVEITEM_ACK(MI_SUCCESS);
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_SKILLBUY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wNpcID;
	WORD wSkillID;

	pBUF->m_packet
		>> wNpcID
		>> wSkillID;

	if(pPlayer->m_dealItem.m_bStatus != DEAL_READY)
	{
		pPlayer->SendCS_SKILLBUY_ACK(
			SKILL_ACTIONLOCK,
			wSkillID,
			0);

		return EC_NOERROR;
	}

	CTNpc * pNpc = FindTNpc(wNpcID);
	if(!pNpc)
	{
		pPlayer->SendCS_SKILLBUY_ACK(
			SKILL_NOTFOUND,
			wSkillID,
			0);
		return EC_NOERROR;
	}

	if( !pNpc->CanTalk(pPlayer->m_bCountry, pPlayer->m_bAidCountry, pPlayer->HaveDisguiseBuff()))
	{
		pPlayer->SendCS_SKILLBUY_ACK(
			SKILL_NOTFOUND,
			wSkillID,
			0);
		return EC_NOERROR;
	}

	BYTE bDiscountRate = GetDiscountRate(pPlayer, pNpc);

	CTSkill * pSkill = pPlayer->FindTSkill(wSkillID);
	if(pSkill)
	{
		LPTLEVEL pNextLevel = FindTLevel(pSkill->GetNextLevel());
		DWORD dwPrice = pSkill->m_pTSKILL->GetPrice(pNextLevel->m_dwMoney);  		
		dwPrice -= DWORD(dwPrice * bDiscountRate / 100 );

		if(!pNextLevel ||
			pSkill->m_pTSKILL->m_bMaxLevel == pSkill->m_bLevel)
		{
			pPlayer->SendCS_SKILLBUY_ACK(
				SKILL_ALREADY,
				wSkillID,
				pSkill->m_bLevel);
			return EC_NOERROR;
		}
		else if(pPlayer->m_bLevel < pSkill->GetNextLevel())
		{
			pPlayer->SendCS_SKILLBUY_ACK(
				SKILL_NEEDLEVELUP,
				wSkillID,
				pSkill->m_bLevel);
			return EC_NOERROR;
		}
		else if(!pPlayer->UseMoney(dwPrice, FALSE)) 
		{
			pPlayer->SendCS_SKILLBUY_ACK(
				SKILL_NEEDMONEY,
				wSkillID,
				pSkill->m_bLevel);
			return EC_NOERROR;
		}
		else if(!pPlayer->IsEnoughSkillPoint(pSkill->m_pTSKILL))
		{
			pPlayer->SendCS_SKILLBUY_ACK(
				SKILL_NEEDSKILLPOINT,
				wSkillID,
				0);
			return EC_NOERROR;
		}
		else
		{
			if(pSkill->m_pTSKILL->m_wParentSkillID)
			{
				CTSkill * pParent = pPlayer->FindTSkill(pSkill->m_pTSKILL->m_wParentSkillID);
				if(!pParent || !pSkill->CheckParentSkill(pParent->m_bLevel))
				{
					pPlayer->SendCS_SKILLBUY_ACK(
						SKILL_NEEDPARENT,
						wSkillID,
						0);

					return EC_NOERROR;
				}
			}
			
			__int64 llUseMoney = dwPrice;  

			pPlayer->UseMoney(llUseMoney, TRUE);

			pPlayer->m_wSkillPoint -= pSkill->GetNextSkillPoint();
			pSkill->m_bLevel++;

			pPlayer->SendCS_SKILLBUY_ACK(
				SKILL_SUCCESS,
				pSkill->m_pTSKILL->m_wID,
				pSkill->m_bLevel);

#ifdef DEF_UDPLOG
			m_pUdpSocket->LogSkillAct(LOGMAP_SKILLLEVELUP, pPlayer, pSkill, -llUseMoney);			
#else
			SendDM_LOGSKILL_REQ(
				pPlayer->m_dwID,
				pPlayer->m_dwGold,
				pPlayer->m_dwSilver,
				pPlayer->m_dwCooper,
				wSkillID,
				pSkill->m_bLevel);
#endif

			return EC_NOERROR;
		}
	}

	CTSkillTemp * pTemp = pNpc->GetSkill(wSkillID);
	if(!pSkill && pTemp)
	{
		DWORD dwPrice = pTemp->GetPrice(FindTLevel(pTemp->m_bStartLevel)->m_dwMoney); 
		dwPrice -= DWORD(dwPrice * bDiscountRate / 100 );  

		if(pPlayer->m_bLevel < pTemp->m_bStartLevel)
		{
			pPlayer->SendCS_SKILLBUY_ACK(
				SKILL_NEEDLEVELUP,
				wSkillID,
				0);
			return EC_NOERROR;
		}


		if(pTemp->m_wParentSkillID)
		{
			CTSkill * pParent = pPlayer->FindTSkill(pTemp->m_wParentSkillID);
			if(!pParent || !pTemp->CheckParentSkill(1, pParent->m_bLevel))
			{
				pPlayer->SendCS_SKILLBUY_ACK(
					SKILL_NEEDPARENT,
					wSkillID,
					0);
				return EC_NOERROR;
			}
		}

		if(!(pTemp->m_dwClassID & BITSHIFTID(pPlayer->m_bClass)))
		{
			pPlayer->SendCS_SKILLBUY_ACK(
				SKILL_MATCHCLASS,
				wSkillID,
				0);
			return EC_NOERROR;
		}

		if(!pPlayer->IsEnoughSkillPoint(pTemp))
		{
			pPlayer->SendCS_SKILLBUY_ACK(
				SKILL_NEEDSKILLPOINT,
				wSkillID,
				0);
			return EC_NOERROR;
		}

		if(!pPlayer->UseMoney(dwPrice, FALSE)) 
		{
			pPlayer->SendCS_SKILLBUY_ACK(
				SKILL_NEEDMONEY,
				wSkillID,
				0);
			return EC_NOERROR;
		}

		pSkill = new CTSkill();
		pSkill->m_pTSKILL = pTemp;
		pPlayer->m_wSkillPoint -= pTemp->GetNeedSkillPoint(1);
		pPlayer->UseMoney(dwPrice, TRUE); 

		//스킬 버릴때 m_vRemainSkill에서 해당 스킬 꼭 뺄것
		pPlayer->m_mapTSKILL.insert(MAPTSKILL::value_type(wSkillID, pSkill));
		pPlayer->RemainSkill( pSkill, 0);
	}

	if(!pSkill)
	{
		pPlayer->SendCS_SKILLBUY_ACK(
			SKILL_NOTFOUND,
			wSkillID,
			0);

		return EC_NOERROR;
	}

	pPlayer->SendCS_SKILLBUY_ACK(
		SKILL_SUCCESS,
		wSkillID,
        pSkill->m_bLevel);

#ifdef DEF_UDPLOG
	DWORD dwPriceLog = pTemp->GetPrice(FindTLevel(pTemp->m_bStartLevel)->m_dwMoney);  
	dwPriceLog -= DWORD(dwPriceLog * bDiscountRate / 100 );  
	__int64 pMoney = dwPriceLog;
	m_pUdpSocket->LogSkillAct(LOGMAP_SKILLBUY, pPlayer, pSkill, -pMoney);
#else
	SendDM_LOGSKILL_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwGold,
		pPlayer->m_dwSilver,
		pPlayer->m_dwCooper,
		wSkillID,
		pSkill->m_bLevel);
#endif

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_SKILLUSE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	VDWORD vDEFEND;
	VBYTE vDEFENDTYPE;

	BYTE bChannel;
	WORD wMapID;

	BYTE bCount;
	DWORD dwTarget;
	BYTE bTargetType;
	BYTE bIsTarget;
	BYTE bCanSelect = TRUE;
	DWORD dwAttackID;
	BYTE bAttackType;

	WORD wSkillID;
	BYTE bActionID;
	DWORD dwActID;
	DWORD dwAniID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwAttackID
		>> bAttackType
		>> bChannel
		>> wMapID
		>> wSkillID
		>> bActionID
		>> dwActID
		>> dwAniID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> bCount;

	VDWORD vMONSTER;
	vMONSTER.clear();

	if(!pPlayer->m_pMAP)
	{
		if(pPlayer->m_bMain)
			pPlayer->SendCS_SKILLUSE_ACK(
				SKILL_NOTFOUND,
				dwAttackID,
				bAttackType,
				wSkillID,
				bActionID,
				dwActID,
				dwAniID);

		return EC_NOERROR;
	}

	CTObjBase *pATTACK = NULL;

	switch(bAttackType)
	{
	case OT_MON	:
		pATTACK = pPlayer->m_pMAP->FindMonster(dwAttackID);
		break;

	case OT_RECALL:
		{
			CTRecallMon * pRecall = pPlayer->FindRecallMon(dwAttackID);
			if(pRecall && pPlayer->m_bMain)
			{
				pATTACK = pRecall;
				bCanSelect = pRecall->m_pMON->m_bCanSelect;
			}
			else
				return EC_NOERROR;
		}
		break;

	case OT_PC	:
		{
			MAPPLAYER::iterator finder = m_mapPLAYER.find(dwAttackID);
			if( finder != m_mapPLAYER.end() &&
				((CTPlayer *)(*finder).second)->m_bMain &&
				((CTPlayer *)(*finder).second)->m_pMAP)
			{
				pATTACK = (*finder).second;
				if(((CTPlayer *)pATTACK)->m_dwRiding)
				{
					pPlayer->SendCS_SKILLUSE_ACK(
						SKILL_ACTIONLOCK,
						dwAttackID,
						bAttackType,
						wSkillID,
						bActionID,
						dwActID,
						dwAniID);

					return EC_NOERROR;
				}
			}
			else
				return EC_NOERROR;
		}
		break;

	case OT_SELF:
		pATTACK = pPlayer->FindSelfObj(dwAttackID);
		break;
	}

	if(!pATTACK || !pATTACK->m_pMAP)
		return EC_NOERROR;

	CTSkill * pSkill = pATTACK->FindTSkill(wSkillID);
	if(!pSkill)
	{
		pPlayer->SendCS_SKILLUSE_ACK(
			SKILL_NOTFOUND,
			dwAttackID,
			bAttackType,
			wSkillID,
			bActionID,
			dwActID,
			dwAniID);

		return EC_NOERROR;
	}

	if(pSkill->m_pTSKILL->m_wMapID != INVALID_MAPID &&
		pSkill->m_pTSKILL->m_wMapID != pPlayer->m_wMapID)
	{
		pPlayer->SendCS_SKILLUSE_ACK(
			SKILL_WRONGREGION,
			dwAttackID,
			bAttackType,
			wSkillID,
			bActionID,
			dwActID,
			dwAniID);

		return EC_NOERROR;
	}

	if(bAttackType != OT_MON &&
		pSkill->m_pTSKILL->IsNegative() &&
		CheckPeaceZone(pPlayer))
	{
		pPlayer->SendCS_SKILLUSE_ACK(
			SKILL_PEACEZONE,
			dwAttackID,
			bAttackType,
			wSkillID,
			bActionID,
			dwActID,
			dwAniID);

		return EC_NOERROR;
	}

	if(IsTournamentMap(pATTACK->m_wMapID))
	{
		LPTOURNAMENTPLAYER pTP = FindTournamentPlayer(pPlayer->m_dwID, TRUE);
		if(!pTP || pSkill->m_pTSKILL->m_wID == TOURNAMENT_CANTUSESKILL)
		{
			pPlayer->SendCS_SKILLUSE_ACK(
				SKILL_NOTFOUND,
				dwAttackID,
				bAttackType,
				wSkillID,
				bActionID,
				dwActID,
				dwAniID);

			return EC_NOERROR;
		}
	}

	if(pPlayer->m_wArenaID && pSkill->m_pTSKILL->m_wID == TOURNAMENT_CANTUSESKILL)
	{
		pPlayer->SendCS_SKILLUSE_ACK(
			SKILL_NOTFOUND,
			dwAttackID,
			bAttackType,
			wSkillID,
			bActionID,
			dwActID,
			dwAniID);

		return EC_NOERROR;
	}

	BYTE bIsMultiAtk = pSkill->m_pTSKILL->IsMultiAttack();
	BYTE bCountMultiAttack = 0;
	BYTE bTotalHit = 0;
	if(bIsMultiAtk)
		bCountMultiAttack = pSkill->GetCountMultiAttack();

	for(BYTE i=0; i<bCount; i++)
	{
		pBUF->m_packet
			>> dwTarget
			>> bTargetType
			>> bIsTarget;

		if(bIsTarget && bTotalHit < MAX_TARGET)
		{
			if(!bIsMultiAtk || bCountMultiAttack)
			{
				vDEFEND.push_back(dwTarget);
				vDEFENDTYPE.push_back(bTargetType);
				bTotalHit++;
			}

			if(bCountMultiAttack)
			{
				bCountMultiAttack--;
				BYTE bHitCount= 1;
				BYTE bRandCount = BYTE(rand() % (pSkill->m_pTSKILL->m_bTargetHit+1));
				while(bCountMultiAttack &&
					bHitCount < bRandCount )
				{
					vDEFEND.push_back(dwTarget);
					vDEFENDTYPE.push_back(bTargetType);
					bCountMultiAttack--;
					bTotalHit++;
					bHitCount++;
				}
			}
		}

		if(!bIsTarget && bTargetType == OT_MON)
			vMONSTER.push_back(dwTarget);
	}

	while(bCountMultiAttack && vDEFEND.size() && bTotalHit < MAX_TARGET)
	{
		vDEFEND.push_back(vDEFEND[0]);
		vDEFENDTYPE.push_back(vDEFENDTYPE[0]);
		bCountMultiAttack--;
		bTotalHit++;
	}

	if(!pATTACK->CheckAttack() &&
		pSkill->m_pTSKILL->IsNegative())
	{
		pPlayer->SendCS_SKILLUSE_ACK(
			SKILL_SPEEDYUSE,
			dwAttackID,
			bAttackType,
			wSkillID,
			bActionID,
			dwActID,
			dwAniID);

		return EC_NOERROR;
	}

	DWORD dwMP = pSkill->GetRequiredMP(pATTACK->GetPureMaxMP());
	if( pATTACK->m_dwMP < dwMP)
	{
		pPlayer->SendCS_SKILLUSE_ACK(
			SKILL_NEEDMP,
			dwAttackID,
			bAttackType,
			wSkillID,
			bActionID,
			dwActID,
			dwAniID);

		return EC_NOERROR;
	}

	DWORD dwHP = pSkill->GetRequiredHP(pATTACK->GetPureMaxHP());
	if(pATTACK->m_dwHP <= dwHP)
	{
		pPlayer->SendCS_SKILLUSE_ACK(
			SKILL_NEEDHP,
			dwAttackID,
			bAttackType,
			wSkillID,
			bActionID,
			dwActID,
			dwAniID);

		return EC_NOERROR;
	}

	if(!pATTACK->CheckPrevAct(pSkill->m_pTSKILL->m_wPrevActiveID))
	{
		pPlayer->SendCS_SKILLUSE_ACK(
			SKILL_NEEDPREVACT,
			dwAttackID,
			bAttackType,
			wSkillID,
			bActionID,
			dwActID,
			dwAniID);

		return EC_NOERROR;
	}

	WORD wBackSkill = 0;

	if(bAttackType != OT_MON)
	{
		if(!pSkill->CanUse(m_dwTick))
		{
			pPlayer->SendCS_SKILLUSE_ACK(
				SKILL_SPEEDYUSE,
				dwAttackID,
				bAttackType,
				wSkillID,
				bActionID,
				dwActID,
				dwAniID);

			return EC_NOERROR;
		}

		if(!pPlayer->UseSkillItem(pSkill, bTotalHit-1))	//소모 아이템을 없애니깐 사용 마지막 조건으로...
		{
			pPlayer->SendCS_SKILLUSE_ACK(
				SKILL_UNSUITWEAPON,
				dwAttackID,
				bAttackType,
				wSkillID,
				bActionID,
				dwActID,
				dwAniID);

			return EC_NOERROR;
		}

		pPlayer->m_dwLastAtkTick = m_dwTick;

		pATTACK->SkillUse(pSkill, m_dwTick);
		DWORD dwAggro;
		if(bAttackType == OT_PC)
			dwAggro = pSkill->GetAggro();
		else
			dwAggro = pSkill->m_pTSKILL->GetAggro(pATTACK->m_bLevel);

		if(dwAggro)
			for(size_t i=0; i<vMONSTER.size(); i++)
			{
				CTMonster *pMON = pPlayer->m_pMAP->FindMonster(vMONSTER[i]);
				if(pMON)
					for(BYTE j=0; j<vDEFEND.size(); j++)
						if(vDEFENDTYPE[j] != OT_MON)
							pMON->SetAggro(
								pPlayer->m_dwID,
								dwAttackID,
								bAttackType,
								pATTACK->GetWarCountry(),
								pPlayer->m_bClass,
								vDEFEND[j],
								vDEFENDTYPE[j],
								dwAggro,
								FALSE);
			}
	}

	if(pATTACK->m_bMode == MT_NORMAL &&
		pSkill->m_pTSKILL->IsNegative())
		pATTACK->ChgMode(MT_BATTLE);

	WORD wTransHP = 0;
	WORD wTransMP = 0;
	BYTE bTransHPMP = pSkill->m_pTSKILL->GetTransHPMPType();
	if(bTransHPMP == SCT_HPTRANS)
	{
		wTransHP = WORD(pATTACK->m_dwHP/2);
		dwHP = wTransHP;
	}
	else if(bTransHPMP == SCT_MPTRANS)
	{
		wTransMP = WORD(pATTACK->m_dwMP/2);
		dwMP = wTransMP;
	}

	if(dwHP || dwMP)
	{
		pATTACK->m_dwHP -= dwHP;
		pATTACK->m_dwMP -= dwMP;
	}
	pATTACK->m_pInstanceSkill = pSkill;

	WORD wAttackLevel = pSkill->m_pTSKILL->GetAttackType() == SAT_PHYSIC ? pATTACK->GetAttackLevel() : pATTACK->GetMagicAtkLevel();
	BYTE bIsPysLong = pSkill->m_pTSKILL->IsLongAttack();
	DWORD dwPysMinPower = pATTACK->GetMinAP(bIsPysLong);
	DWORD dwPysMaxPower = pATTACK->GetMaxAP(bIsPysLong);
	DWORD dwMgMinPower = pATTACK->GetMinMagicAP();
	DWORD dwMgMaxPower = pATTACK->GetMaxMagicAP();
	BYTE bCP = pATTACK->GetCritical(pSkill);

	if(bAttackType == OT_PC && pSkill->m_pTSKILL->m_bDuraSlot)
		wBackSkill = pPlayer->DurationDec(pSkill->m_pTSKILL->m_bDuraSlot, pSkill->m_pTSKILL->IsItemDelUser());

	pATTACK->m_pInstanceSkill = NULL;
	pATTACK->m_wCurChargeSkill = 0;

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	DWORD dwMaxHP = pATTACK->GetMaxHP();
	DWORD dwMaxMP = pATTACK->GetMaxMP();
	WORD wSTR = (WORD)pATTACK->GetSTR();
	WORD wINT = (WORD)pATTACK->GetINT();
	BYTE bCurseProb = 0;
	BYTE bSkillLevel = pSkill->m_bLevel;
	if(pATTACK->m_bType == OT_RECALL ||
		pATTACK->m_bType == OT_SELF)
		bSkillLevel = min(((CTRecallMon *)pATTACK)->m_bAtkSkillLevel, pSkill->m_pTSKILL->m_bMaxLevel);
	
	pATTACK->EraseBuffByAttack(pSkill->m_pTSKILL);
	pATTACK->CheckEternalBuff();

	pPlayer->m_pMAP->GetNeerPlayer(
		&vPLAYERS,
		pATTACK->m_fPosX,
		pATTACK->m_fPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_SKILLUSE_ACK(
			SKILL_SUCCESS,
			dwAttackID,
			bAttackType,
			wSkillID,
			bActionID,
			dwActID,
			dwAniID,
			bSkillLevel,
			wBackSkill,
			wAttackLevel,
			pATTACK->m_bLevel,
			dwPysMinPower,
			dwPysMaxPower,
			dwMgMinPower,
			dwMgMaxPower,
			wTransHP,
			wTransMP,
			bCurseProb,
			pATTACK->m_bEquipSpecial,
			bCanSelect,
			pATTACK->m_bCountry,
			pATTACK->m_bAidCountry,
			bCP,
			fPosX,
			fPosY,
			fPosZ,
			&vDEFEND,
			&vDEFENDTYPE);

		if(dwHP || dwMP)
			pChar->SendCS_HPMP_ACK(
				dwAttackID,
				bAttackType,
				pATTACK->m_bLevel,
				dwMaxHP,
				pATTACK->m_dwHP,
				dwMaxMP,
				pATTACK->m_dwMP);

		vPLAYERS.pop_back();
	}
	vMONSTER.clear();
	vPLAYERS.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_LOOPSKILL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	VDWORD vDEFEND;
	VBYTE vDEFENDTYPE;

	BYTE bChannel;
	WORD wMapID;

	BYTE bCount;
	DWORD dwTarget;
	BYTE bTargetType;
	BYTE bIsTarget;

	DWORD dwAttackID;
	BYTE bAttackType;

	WORD wSkillID;
	BYTE bCanSelect = TRUE;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwAttackID
		>> bAttackType
		>> bChannel
		>> wMapID
		>> wSkillID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> bCount;

	VDWORD vMONSTER;
	vMONSTER.clear();

	if(!pPlayer->m_pMAP)
	{
		if(pPlayer->m_bMain)
			pPlayer->SendCS_LOOPSKILL_ACK(
				SKILL_NOTFOUND,
				dwAttackID,
				bAttackType,
				wSkillID);

		return EC_NOERROR;
	}

	CTObjBase *pATTACK = NULL;

	switch(bAttackType)
	{
	case OT_MON	:
		pATTACK = pPlayer->m_pMAP->FindMonster(dwAttackID);
		break;

	case OT_RECALL:
		{
			CTRecallMon * pRecall = pPlayer->FindRecallMon(dwAttackID);
			if(pRecall && pPlayer->m_bMain)
			{
				pATTACK = pRecall;
				bCanSelect = pRecall->m_pMON->m_bCanSelect;
			}
		}
		break;
	case OT_PC	:
		{
			MAPPLAYER::iterator finder = m_mapPLAYER.find(dwAttackID);
			if( finder != m_mapPLAYER.end() &&
				((CTPlayer *)(*finder).second)->m_bMain &&
				((CTPlayer *)(*finder).second)->m_pMAP)
			{
				pATTACK = (*finder).second;
			}
		}
		break;
	case OT_SELF:
		{
			pATTACK = pPlayer->FindSelfObj(dwAttackID);
		}
		break;
	}

	if(!pATTACK || !pATTACK->m_pMAP)
		return EC_NOERROR;

	CTSkill * pSkill = pATTACK->FindTSkill(wSkillID);
	if(!pSkill)
	{
		pPlayer->SendCS_LOOPSKILL_ACK(
			SKILL_NOTFOUND,
			dwAttackID,
			bAttackType,
			wSkillID);

		return EC_NOERROR;
	}

	if(	bAttackType != OT_MON &&
		pSkill->m_pTSKILL->IsNegative() &&
		CheckPeaceZone(pPlayer))
	{
		pPlayer->SendCS_LOOPSKILL_ACK(
			SKILL_PEACEZONE,
			dwAttackID,
			bAttackType,
			wSkillID);

		return EC_NOERROR;
	}

	BYTE bIsMultiAtk = pSkill->m_pTSKILL->IsMultiAttack();
	BYTE bCountMultiAttack = 0;
	BYTE bTotalHit = 0;

	if(bIsMultiAtk)
		bCountMultiAttack = pSkill->GetCountMultiAttack();

	for(BYTE i=0; i<bCount; i++)
	{
		pBUF->m_packet
			>> dwTarget
			>> bTargetType
			>> bIsTarget;

		if(bIsTarget && bTotalHit < MAX_TARGET)
		{
			if(!bIsMultiAtk || bCountMultiAttack)
			{
				vDEFEND.push_back(dwTarget);
				vDEFENDTYPE.push_back(bTargetType);
				bTotalHit++;
			}

			if(bCountMultiAttack)
			{
				bCountMultiAttack--;
				BYTE bHitCount= 1;
				BYTE bRandCount = BYTE(rand() % (pSkill->m_pTSKILL->m_bTargetHit+1));
				while(bCountMultiAttack &&
					bHitCount < bRandCount )
				{
					vDEFEND.push_back(dwTarget);
					vDEFENDTYPE.push_back(bTargetType);
					bCountMultiAttack--;
					bTotalHit++;
					bHitCount++;
				}
			}
		}

		if(!bIsTarget && bTargetType == OT_MON)
			vMONSTER.push_back(dwTarget);
	}

	while(bCountMultiAttack && vDEFEND.size() && bTotalHit < MAX_TARGET)
	{
		vDEFEND.push_back(vDEFEND[0]);
		vDEFENDTYPE.push_back(vDEFENDTYPE[0]);
		bCountMultiAttack--;
		bTotalHit++;
	}

	if( !pSkill->CanUse(m_dwTick))
	{
		pPlayer->SendCS_LOOPSKILL_ACK(
			SKILL_SPEEDYUSE,
			dwAttackID,
			bAttackType,
			wSkillID);

		return EC_NOERROR;
	}

	DWORD dwMP = pSkill->GetRequiredMP(pATTACK->GetPureMaxMP());
	if( pATTACK->m_dwMP < dwMP)
	{
		pPlayer->SendCS_LOOPSKILL_ACK(
			SKILL_NEEDMP,
			dwAttackID,
			bAttackType,
			wSkillID);

		return EC_NOERROR;
	}

	DWORD dwHP = pSkill->GetRequiredHP(pATTACK->GetPureMaxHP());
	if(pATTACK->m_dwHP < dwHP)
	{
		pPlayer->SendCS_LOOPSKILL_ACK(
			SKILL_NEEDHP,
			dwAttackID,
			bAttackType,
			wSkillID);

		return EC_NOERROR;
	}

	if(!pATTACK->CheckPrevAct(pSkill->m_pTSKILL->m_wTargetActiveID))
	{
		pPlayer->SendCS_LOOPSKILL_ACK(
			SKILL_NEEDPREVACT,
			dwAttackID,
			bAttackType,
			wSkillID);

		return EC_NOERROR;
	}

	if( bAttackType == OT_PC &&
		!pPlayer->UseSkillItem(pSkill, bTotalHit-1))	//소모 아이템을 없애니깐 사용 마지막 조건으로...
	{
		pPlayer->SendCS_LOOPSKILL_ACK(
			SKILL_UNSUITWEAPON,
			dwAttackID,
			bAttackType,
			wSkillID);

		return EC_NOERROR;
	}

	if(bAttackType != OT_MON &&	pSkill->GetAggro())
		for(size_t i=0; i<vMONSTER.size(); i++)
		{
			CTMonster * pMON = pPlayer->m_pMAP->FindMonster(vMONSTER[i]);
			if(pMON)
			{
				for(BYTE j=0; j<vDEFEND.size(); j++)
				{
					pMON->SetAggro(
						pPlayer->m_dwID,
						dwAttackID,
						bAttackType,
						pATTACK->GetWarCountry(),
						pPlayer->m_bClass,
						vDEFEND[j],
						vDEFENDTYPE[j],
						pSkill->GetAggro(),
						FALSE);
				}
			}
		}

	pSkill->Use(
		SDELAY_LOOP,
		m_dwTick,
		pPlayer->GetAtkSpeed(pSkill->m_pTSKILL->m_bSpeedApply),
		pPlayer->GetAtkSpeedRate(pSkill->m_pTSKILL->m_bSpeedApply));

	if(dwHP || dwMP)
	{
		pATTACK->m_dwHP -= dwHP;
		pATTACK->m_dwMP -= dwMP;
	}

	BYTE bHit = pATTACK->GetCritical(pSkill);

	pATTACK->m_pInstanceSkill = pSkill;

	WORD wAttackLevel = pATTACK->GetAttackLevel();
	WORD wMagicAtkLevel = pATTACK->GetMagicAtkLevel();
	BYTE bIsPysLong = pSkill->m_pTSKILL->IsLongAttack();
	DWORD dwPysMinPower = pATTACK->GetMinAP(bIsPysLong);
	DWORD dwPysMaxPower = pATTACK->GetMaxAP(bIsPysLong);
	DWORD dwMgMinPower = pATTACK->GetMinMagicAP();
	DWORD dwMgMaxPower = pATTACK->GetMaxMagicAP();
	BYTE bCP = pATTACK->GetCritical(pSkill);

	pATTACK->m_pInstanceSkill = NULL;

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	DWORD dwMaxHP = pATTACK->GetMaxHP();
	DWORD dwMaxMP = pATTACK->GetMaxMP();

	pPlayer->m_pMAP->GetNeerPlayer(
		&vPLAYERS,
		pATTACK->m_fPosX,
		pATTACK->m_fPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_LOOPSKILL_ACK(
			SKILL_SUCCESS,
			dwAttackID,
			bAttackType,
			wSkillID,
			pSkill->m_bLevel,
			wAttackLevel,
			pATTACK->m_bLevel,
			dwPysMinPower,
			dwPysMaxPower,
			dwMgMinPower,
			dwMgMaxPower,
			bCanSelect,
			pATTACK->m_bCountry,
			pATTACK->m_bAidCountry,
			bCP,
			fPosX,
			fPosY,
			fPosZ,
			&vDEFEND,
			&vDEFENDTYPE);

		if(dwHP || dwMP)
			pChar->SendCS_HPMP_ACK(
				dwAttackID,
				bAttackType,
				pATTACK->m_bLevel,
				dwMaxHP,
				pATTACK->m_dwHP,
				dwMaxMP,
				pATTACK->m_dwMP);

		vPLAYERS.pop_back();
	}

	vMONSTER.clear();
	vPLAYERS.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CHGPARTYCHIEF_REQ( LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwTargetID;

	pBUF->m_packet
		>> dwTargetID;

	SendMW_CHGPARTYCHIEF_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY, 
		dwTargetID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CHGPARTYTYPE_REQ( LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bPartyType;

	pBUF->m_packet
		>> bPartyType;

	SendMW_CHGPARTYTYPE_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY,
		bPartyType);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_PARTYADD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	CString strTarget;
	BYTE bObtainType=0;

	pBUF->m_packet
		>> strTarget
		>> bObtainType;

	if(pPlayer->m_wArenaID)
		return EC_NOERROR;

	SendMW_PARTYADD_ACK(
		pPlayer->m_strNAME,
		strTarget,
		bObtainType,
		pPlayer->GetMaxHP(),
		pPlayer->m_dwHP,
		pPlayer->GetMaxMP(),
		pPlayer->m_dwMP);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_PARTYJOIN_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if(!pPlayer->m_bMain)
		return EC_NOERROR;

	CString strOrigin;
	BYTE bObtainType;
	BYTE bResponse;

	pBUF->m_packet
		>> strOrigin
		>> bObtainType
		>> bResponse;

	SendMW_PARTYJOIN_ACK(
		strOrigin,
		pPlayer->m_strNAME,
		bObtainType,
		bResponse,
		pPlayer->GetMaxHP(),
		pPlayer->m_dwHP,
		pPlayer->GetMaxMP(),
		pPlayer->m_dwMP);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_PARTYDEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwMemberID;
	pBUF->m_packet
		>> dwMemberID;

	if(!pPlayer->GetPartyID() ||
		(dwMemberID != pPlayer->m_dwID &&
		 pPlayer->GetPartyChiefID() != pPlayer->m_dwID))
		return EC_NOERROR;

	SendMW_PARTYDEL_ACK(
		pPlayer->GetPartyID(),
		dwMemberID,
		pPlayer->m_dwID != dwMemberID ? TRUE : FALSE);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_NPCTALK_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wNpcID;
	pBUF->m_packet
		>> wNpcID;

	CTNpc * pNpc = FindTNpc(wNpcID);
	if(!pNpc)
		return EC_NOERROR;

	if( !pNpc->CanTalk(pPlayer->m_bCountry, pPlayer->m_bAidCountry, pPlayer->HaveDisguiseBuff()))
		return EC_NOERROR;

	DWORD dwQuestID = pPlayer->CheckQuest(
		m_dwTick,
		wNpcID,
		QTT_TALK,
		1);

	pPlayer->SendCS_NPCTALK_ACK(dwQuestID, wNpcID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_QUESTEXEC_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwRewardID;
	DWORD dwQuestID;
	BYTE bRewardType;

	pBUF->m_packet
		>> dwQuestID
		>> bRewardType
		>> dwRewardID;

	LPQUESTTEMP pQUEST = FindQuestTemplate(dwQuestID);
	if(pQUEST)
	{
		if(pQUEST->m_bTriggerType == TT_TALKNPC)
		{
			CTNpc * pNpc = FindTNpc(WORD(pQUEST->m_dwTriggerID));
			if(!pNpc)
				return EC_NOERROR;

			if(pPlayer->m_wMapID != pNpc->m_wMapID || GetDistance(pNpc->m_fPosX, pNpc->m_fPosZ, pPlayer->m_fPosX, pPlayer->m_fPosZ) > 10)
				return EC_NOERROR;
		}

		CQuest *pQuest = CQuest::CreateQuest(pQUEST);

		pQuest->m_bRewardType = bRewardType;
		pQuest->m_dwRewardID = dwRewardID;

		pQuest->m_fPosX = pPlayer->m_fPosX;
		pQuest->m_fPosY = pPlayer->m_fPosY;
		pQuest->m_fPosZ = pPlayer->m_fPosZ;
		pQuest->m_pMap = pPlayer->m_pMAP;
		pQuest->m_dwMonID = 0;

		pQuest->ExecQuest(
			pPlayer,
			m_dwTick,
			&m_mapTRIGGER,
			&m_mapQUESTTEMP,
			&m_mapTITEM,
			&m_mapTSKILL,
			&m_mapTMONSTER,
			&m_mapTCLASS);

		delete pQuest;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_QUESTDROP_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwQuestID;

	pBUF->m_packet
		>> dwQuestID;

	pPlayer->DropQuest(dwQuestID);

	pPlayer->SendCS_QUESTCOMPLETE_ACK(
		QR_DROP,
		dwQuestID,
		0,0,
		dwQuestID);
#ifdef DEF_UDPLOG
	m_pUdpSocket->LogQuest(LOGMAP_QUESTDROP, pPlayer, dwQuestID);
#endif

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_QUESTENDTIMER_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwQuestID;

	pBUF->m_packet
		>> dwQuestID;

	MAPQUEST::iterator finder = pPlayer->m_mapQUEST.find(dwQuestID);
	if( finder != pPlayer->m_mapQUEST.end() && (*finder).second->m_bTriggerCount > (*finder).second->m_bCompleteCount )
		for( int i=0; i<INT((*finder).second->m_pQUEST->m_vTerm.size()); i++)
			if( (*finder).second->m_pQUEST->m_vTerm[i]->m_bTermType == QTT_TIMER )
			{
				(*finder).second->m_dwBeginTick = 0;
				(*finder).second->m_bSave = TRUE;  

				pPlayer->SendCS_QUESTUPDATE_ACK(
					(*finder).second->m_pQUEST->m_dwQuestID,
					(*finder).second->m_pQUEST->m_vTerm[i]->m_dwTermID,
					(*finder).second->m_pQUEST->m_vTerm[i]->m_bTermType,
					0, QTS_FAILED);
#ifdef DEF_UDPLOG
                m_pUdpSocket->LogQuest(LOGMAP_QUESTFAILED, pPlayer, dwQuestID);
#endif
			}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_QUESTLIST_POSSIBLE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if(!pPlayer->m_bMain )
		return EC_NOERROR;

	if(!pPlayer->m_pMAP)
	{
		pPlayer->m_questlist_possible.Copy(&pBUF->m_packet);
		pPlayer->m_questlist_possible.Rewind(FALSE);
		return EC_NOERROR;
	}
	pPlayer->m_questlist_possible.Clear();

	pPlayer->SendQuestListPossible(&pBUF->m_packet);

	return EC_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 길드
DWORD CTMapSvrModule::OnCS_GUILDESTABLISH_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strName;
	pBUF->m_packet
		>> strName;

	strName.Trim(_T(" "));
	if(strName.IsEmpty())
	{
		pPlayer->SendCS_GUILDESTABLISH_ACK(GUILD_ESTABLISH_ERR);
		return EC_NOERROR;
	}

	if(strName.GetLength() > MAX_NAME)
	{
		pPlayer->SendCS_GUILDESTABLISH_ACK(GUILD_ESTABLISH_ERR);
		return EC_NOERROR;
	}

	if(!CheckCharName(strName))
	{
		pPlayer->SendCS_GUILDESTABLISH_ACK(GUILD_ESTABLISH_ERR);
		return EC_NOERROR;
	}

	if(pPlayer->m_dwGuildID)
	{
		pPlayer->SendCS_GUILDESTABLISH_ACK(GUILD_HAVEGUILD);
		return EC_NOERROR;
	}

	if(pPlayer->m_bLevel < GUILD_ESTABLISH_LEVEL)
	{
		pPlayer->SendCS_GUILDESTABLISH_ACK(GUILD_ESTABLISH_ERR);
		return EC_NOERROR;
	}

	DWORD dwTime = (DWORD)m_timeCurrent-pPlayer->m_dwGuildLeaveTime;
	if(pPlayer->m_bGuildLeave==GUILD_LEAVE_SELF)
	{
		if(dwTime < GUILD_LEAVE_DURATION)
		{
			pPlayer->SendCS_GUILDESTABLISH_ACK(GUILD_LEAVE_SELF);
			return EC_NOERROR;
		}
	}
	else if(pPlayer->m_bGuildLeave==GUILD_LEAVE_DISORGANIZATION)
	{
		if(dwTime < GUILD_DIS_DURATION)
		{
			pPlayer->SendCS_GUILDESTABLISH_ACK(GUILD_LEAVE_DISORGANIZATION);
			return EC_NOERROR;
		}
	}

	pPlayer->m_bGuildLeave = 0;
	pPlayer->m_dwGuildLeaveTime = 0;

	SendMW_GUILDESTABLISH_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		strName);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDDISORGANIZATION_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bRet;

	pBUF->m_packet
		>> bRet;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_CHIEF))
	{
		pPlayer->SendCS_GUILDDISORGANIZATION_ACK(GUILD_NODUTY);
		return EC_NOERROR;
	}

	SendMW_GUILDDISORGANIZATION_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		bRet);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDINVITE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strTarget;
	pBUF->m_packet
		>> strTarget;

	if(!pPlayer->m_dwGuildID || pPlayer->m_strNAME==strTarget)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
	{
		pPlayer->SendCS_GUILDINVITE_ACK(GUILD_NODUTY, NAME_NULL, 0, NAME_NULL);
		return EC_NOERROR;
	}

	SendMW_GUILDINVITE_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		strTarget);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDINVITEANSWER_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bAnswer;
	DWORD dwInviter;
	pBUF->m_packet
		>> bAnswer
		>> dwInviter;

	pPlayer->m_bGuildLeave = 0;
	pPlayer->m_dwGuildLeaveTime = 0;

	SendMW_GUILDINVITEANSWER_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		bAnswer,
		dwInviter);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDLEAVE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	if(pPlayer->CheckGuildDuty(GUILD_DUTY_CHIEF))
	{
		pPlayer->SendCS_GUILDLEAVE_ACK(GUILD_NODUTY, NAME_NULL, 0);
		return EC_NOERROR;
	}

	SendMW_GUILDLEAVE_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDDUTY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strTarget;
	BYTE bDuty;
	pBUF->m_packet
		>> strTarget
		>> bDuty;

	if(!pPlayer->m_dwGuildID || pPlayer->m_strNAME==strTarget)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_CHIEF))
	{
		pPlayer->SendCS_GUILDDUTY_ACK(GUILD_NODUTY, NAME_NULL, 0);
		return EC_NOERROR;
	}

	SendMW_GUILDDUTY_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		strTarget,
		bDuty);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDPEER_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strTarget;
	BYTE bPeer;
	pBUF->m_packet
		>> strTarget
		>> bPeer;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_CHIEF))
	{
		pPlayer->SendCS_GUILDPEER_ACK(GUILD_NODUTY, NAME_NULL, 0, 0);
		return EC_NOERROR;
	}

	SendMW_GUILDPEER_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		strTarget,
		bPeer);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDKICKOUT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strTarget;
	pBUF->m_packet 
		>> strTarget;

	if(!pPlayer->m_dwGuildID || !pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF) || pPlayer->m_strNAME==strTarget)
		return EC_NOERROR;

	SendMW_GUILDKICKOUT_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		strTarget);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDMEMBERLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(!pPlayer->GetGuild())
		return EC_NOERROR;

	SendMW_GUILDMEMBERLIST_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDINFO_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(!pPlayer->GetGuild())
		return EC_NOERROR;

	SendMW_GUILDINFO_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_GUILDLOCALLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	pPlayer->SendCS_GUILDLOCALLIST_ACK();

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDLOCALRETURN_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDCABINETLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID || pPlayer->m_dwTacticsID)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
		return EC_NOERROR;

	SendMW_GUILDCABINETLIST_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDCABINETPUTIN_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID || pPlayer->m_dwTacticsID)
		return EC_NOERROR;

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
	{
		pPlayer->SendCS_GUILDCABINETPUTIN_ACK(GUILD_CABINET_NOTDUTY);
		return EC_NOERROR;
	}

	BYTE bInvenID;
	BYTE bItemID;
	BYTE bCount;

	pBUF->m_packet
		>> bInvenID
		>> bItemID
		>> bCount;

	if(pPlayer->m_guildItem || pPlayer->m_bStore)
		return EC_NOERROR;

	CTInven *pInven = pPlayer->FindTInven(bInvenID);
	if(!pInven)
		return EC_NOERROR;

	CTItem *pItem = pInven->FindTItem(bItemID);
	if(!pItem || pItem->m_bCount < bCount || !bCount)
		return EC_NOERROR;

	if(!pItem->CanDeal() ||
		!(pItem->m_pTITEM->m_bIsSell & ITEMTRADE_CABINET))
	{
		pPlayer->SendCS_GUILDCABINETPUTIN_ACK(GUILD_CABINET_FAIL);
		return EC_NOERROR;
	}

	pPlayer->CopyGuildItem(pItem, bCount, pItem->m_bCount-bCount);

	pItem->m_bCount -= bCount;
	if(!pItem->m_bCount)
	{
		delete pItem;
		pInven->m_mapTITEM.erase(bItemID);
		pPlayer->SendCS_DELITEM_ACK(bInvenID, bItemID);
	}
	else
		pPlayer->SendCS_UPDATEITEM_ACK(pItem, bInvenID);

	SendDM_GUILDCABINETPUTIN_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_dwGuildID,
		pPlayer->m_guildItem,
		pPlayer->m_guildItem->m_bCount);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDCABINETTAKEOUT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID || pPlayer->m_dwTacticsID)
		return EC_NOERROR;

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
	{
		pPlayer->SendCS_GUILDCABINETTAKEOUT_ACK(GUILD_CABINET_NOTDUTY);
		return EC_NOERROR;
	}

	DWORD dwItemID;
	BYTE bCount;
	BYTE bInvenID;
	BYTE bItemID;

	pBUF->m_packet
		>> dwItemID
		>> bCount
		>> bInvenID
		>> bItemID;

	CTInven * pTInvenDEST = pPlayer->FindTInven(bInvenID);
	if(!bCount ||
		bInvenID == INVEN_EQUIP ||
		!pTInvenDEST)
	{
		pPlayer->SendCS_GUILDCABINETTAKEOUT_ACK(GUILD_CABINET_FAIL);
		SendMW_GUILDCABINETLIST_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY);
		return EC_NOERROR;
	}

	SendDM_GUILDCABINETTAKEOUT_REQ(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY, 
		pPlayer->m_dwGuildID,
		dwItemID, 
		bCount,
		bInvenID,
		bItemID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDCONTRIBUTION_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID || pPlayer->m_dwTacticsID)
		return EC_NOERROR;

	DWORD dwExp;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;
	DWORD dwPvPoint = 0;

	pBUF->m_packet
		>> dwExp
		>> dwGold
		>> dwSilver
		>> dwCooper
		>> dwPvPoint;

	__int64 llMoney = CalcMoney(dwGold, dwSilver, dwCooper); 
	if(!dwExp && !llMoney && !dwPvPoint)
		return EC_NOERROR;

	if(dwExp)
		if(pPlayer->m_bLevel < GUILD_ESTABLISH_LEVEL || !pPlayer->UseExp(dwExp, FALSE))
		{
			pPlayer->SendCS_GUILDCONTRIBUTION_ACK(GUILD_CONTRIBUTION_NOTENOUGH);
			return EC_NOERROR;
		}

	if(llMoney)
		if(!pPlayer->UseMoney(llMoney, FALSE))
		{
			pPlayer->SendCS_GUILDCONTRIBUTION_ACK(GUILD_CONTRIBUTION_NOTENOUGH);
			return EC_NOERROR;
		}

	if(dwPvPoint)
		if(pPlayer->m_dwPvPUseablePoint < dwPvPoint)
		{
			pPlayer->SendCS_GUILDCONTRIBUTION_ACK(GUILD_CONTRIBUTION_NOTENOUGH);
			return EC_NOERROR;
		}

	SendMW_GUILDCONTRIBUTION_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY, 
		dwExp, 
		dwGold, 
		dwSilver, 
		dwCooper,
		dwPvPoint);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDARTICLELIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID || pPlayer->m_dwTacticsID)
		return EC_NOERROR;

	SendMW_GUILDARTICLELIST_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDARTICLEADD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
	{
		pPlayer->SendCS_GUILDARTICLEADD_ACK(GUILD_NODUTY);
		return EC_NOERROR;
	}

	CString strTitle;
	CString strArticle;
	pBUF->m_packet
		>> strTitle
		>> strArticle;

	if(strTitle == NAME_NULL)
		return EC_NOERROR;

	strTitle.Left(MAX_BOARD_TITLE);
	strArticle.Left(MAX_BOARD_TEXT);

	SendMW_GUILDARTICLEADD_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY, 
		strTitle,
		strArticle);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDARTICLEDEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
	{
		pPlayer->SendCS_GUILDARTICLEDEL_ACK(GUILD_NODUTY);
		return EC_NOERROR;
	}
	
	DWORD dwID;
	pBUF->m_packet
		>> dwID;

	SendMW_GUILDARTICLEDEL_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY, 
		dwID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDARTICLEUPDATE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
	{
		pPlayer->SendCS_GUILDARTICLEUPDATE_ACK(GUILD_NODUTY);
		return EC_NOERROR;
	}
	
	DWORD dwID;
	CString strTitle;
	CString strArticle;
	pBUF->m_packet
		>> dwID
		>> strTitle
		>> strArticle;

	if(strTitle == NAME_NULL)
		return EC_NOERROR;

	strTitle.Left(MAX_BOARD_TITLE);
	strArticle.Left(MAX_BOARD_TEXT);

	SendMW_GUILDARTICLEUPDATE_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY, 
		dwID,
		strTitle,
		strArticle);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDFAME_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_CHIEF))
	{
		pPlayer->SendCS_GUILDFAME_ACK(GUILD_NODUTY);
		return EC_NOERROR;
	}

	DWORD dwFame;
	DWORD dwFameColor;
	pBUF->m_packet
		>> dwFame
		>> dwFameColor;

	SendMW_GUILDFAME_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY, 
		dwFame,
		dwFameColor);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDWANTEDADD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
	{
		pPlayer->SendCS_GUILDWANTEDADD_ACK(GUILD_NODUTY);
		return EC_NOERROR;
	}

	DWORD dwID;

	CString strTitle;
	CString strText;
	BYTE bMinLevel;
	BYTE bMaxLevel;

	pBUF->m_packet
		>> dwID
		>> strTitle
		>> strText
		>> bMinLevel
		>> bMaxLevel;

	if(strTitle == NAME_NULL)
		return EC_NOERROR;

	strTitle.Left(MAX_BOARD_TITLE);
	strText.Left(MAX_BOARD_TEXT);

	SendMW_GUILDWANTEDADD_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY, 
		dwID,
		strTitle,
		strText,
		bMinLevel,
		bMaxLevel);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_GUILDWANTEDDEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
		return EC_NOERROR;

	DWORD dwID;
	pBUF->m_packet
		>> dwID;

	SendMW_GUILDWANTEDDEL_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, dwID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDWANTEDLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	SendMW_GUILDWANTEDLIST_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDVOLUNTEERING_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(pPlayer->m_dwGuildID)
	{
		pPlayer->SendCS_GUILDVOLUNTEERING_ACK(GUILD_HAVEGUILD);
		return EC_NOERROR;
	}

	DWORD dwID;
	pBUF->m_packet
		>> dwID;

	SendMW_GUILDVOLUNTEERING_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDVOLUNTEERINGDEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	SendMW_GUILDVOLUNTEERINGDEL_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDVOLUNTEERLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
		return EC_NOERROR;

	SendMW_GUILDVOLUNTEERLIST_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDVOLUNTEERREPLY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
	{
		pPlayer->SendCS_GUILDVOLUNTEERREPLY_ACK(GUILD_NODUTY);
		return EC_NOERROR;
	}

	DWORD dwCharID;
	BYTE bReply;
	pBUF->m_packet
		>> dwCharID
		>> bReply;

	SendMW_GUILDVOLUNTEERREPLY_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwCharID,
		bReply);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDTACTICSWANTEDADD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
	{
		pPlayer->SendCS_GUILDWANTEDADD_ACK(GUILD_NODUTY);
		return EC_NOERROR;
	}

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

	strTitle.Left(MAX_BOARD_TITLE);
	strText.Left(MAX_BOARD_TEXT);

	SendMW_GUILDTACTICSWANTEDADD_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY, 
		dwID,
		strTitle,
		strText,
		bDay,
		bMinLevel,
		bMaxLevel,
		dwPoint,
		dwGold,
		dwSilver,
		dwCooper);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDTACTICSWANTEDDEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
		return EC_NOERROR;

	DWORD dwID;

	pBUF->m_packet
		>> dwID;

	SendMW_GUILDTACTICSWANTEDDEL_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDTACTICSWANTEDLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	SendMW_GUILDTACTICSWANTEDLIST_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDTACTICSVOLUNTEERING_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;
/*
	if(pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
	{
		pPlayer->SendCS_GUILDTACTICSVOLUNTEERING_ACK(GUILD_NODUTY);
		return EC_NOERROR;
	}
*/
	if(pPlayer->m_dwTacticsID)
	{
		pPlayer->SendCS_GUILDTACTICSVOLUNTEERING_ACK(GUILD_HAVEGUILD);
		return EC_NOERROR;
	}

	DWORD dwGuildID;
	DWORD dwID;

	pBUF->m_packet
		>> dwGuildID
		>> dwID;

	if(pPlayer->m_dwGuildID == dwGuildID)
	{
		pPlayer->SendCS_GUILDTACTICSVOLUNTEERING_ACK(GUILD_SAMEGUILDTACTICS);
		return EC_NOERROR;
	}

	SendMW_GUILDTACTICSVOLUNTEERING_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwGuildID,
		dwID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDTACTICSVOLUNTEERINGDEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	SendMW_GUILDTACTICSVOLUNTEERINGDEL_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDTACTICSVOLUNTEERLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
		return EC_NOERROR;

	SendMW_GUILDTACTICSVOLUNTEERLIST_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDTACTICSREPLY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	if(pPlayer->m_dwTacticsID)
		return EC_NOERROR;

	DWORD dwCharID;
	BYTE bReply;
	pBUF->m_packet
		>> dwCharID
		>> bReply;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
	{
		pPlayer->SendCS_GUILDTACTICSREPLY_ACK(GUILD_NODUTY, dwCharID);
		return EC_NOERROR;
	}

	SendMW_GUILDTACTICSREPLY_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwCharID,
		bReply);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDTACTICSKICKOUT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	DWORD dwCharID;
	pBUF->m_packet
		>> dwCharID;

	if(dwCharID != pPlayer->m_dwID)
	{
		if(!pPlayer->m_dwGuildID)
			return EC_NOERROR;

		if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
		{
			pPlayer->SendCS_GUILDTACTICSKICKOUT_ACK(GUILD_NODUTY, dwCharID);
			return EC_NOERROR;
		}

		if(pPlayer->m_dwTacticsID)
		{
			pPlayer->SendCS_GUILDTACTICSKICKOUT_ACK(GUILD_NODUTY, dwCharID);
			return EC_NOERROR;
		}
	}

	SendMW_GUILDTACTICSKICKOUT_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwCharID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDTACTICSINVITE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strName;
	BYTE bDay;
	DWORD dwPoint;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	pBUF->m_packet
		>> strName
		>> bDay
		>> dwPoint
		>> dwGold
		>> dwSilver
		>> dwCooper;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_VICECHIEF))
		return EC_NOERROR;

	if(pPlayer->m_dwTacticsID)
		return EC_NOERROR;

	SendMW_GUILDTACTICSINVITE_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		strName,
		bDay,
		dwPoint,
		dwGold,
		dwSilver,
		dwCooper);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDTACTICSANSWER_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bAnswer;
	CString strInviter;
	BYTE bDay;
	DWORD dwPoint;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	pBUF->m_packet
		>> bAnswer
		>> strInviter
		>> bDay
		>> dwPoint
		>> dwGold
		>> dwSilver
		>> dwCooper;

	SendMW_GUILDTACTICSANSWER_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		bAnswer,
		strInviter,
		bDay,
		dwPoint,
		dwGold,
		dwSilver,
		dwCooper);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDTACTICSLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(!pPlayer->GetGuild())
		return EC_NOERROR;

	SendMW_GUILDTACTICSLIST_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD CTMapSvrModule::OnCS_CHAT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bGroup;
	DWORD dwTarget;
	CString strName;
	CString strTalk;
	CString strSender;

	pBUF->m_packet
		>> strSender
		>> bGroup
		>> dwTarget
		>> strName
		>> strTalk;

	if(bGroup == CHAT_WORLD || bGroup == CHAT_OPERATOR)
		return EC_NOERROR;

	strTalk.Left(1024);

	BYTE bType = bGroup;
	if(IsOperator(pPlayer->m_dwID))
		bType = CHAT_OPERATOR;

	if(bGroup == CHAT_SHOW)
	{
		if(strTalk.Find(pPlayer->m_strNAME) == -1)
			return EC_NOERROR;

		if(dwTarget != strTalk.GetLength())
			return EC_NOERROR;
	}
	else
		pPlayer->CheckChat(strTalk);

	if( m_timeCurrent < pPlayer->m_nChatBanTime)
	{
		pPlayer->SendCS_SYSTEMMSG_ACK(
			SM_CHAT_BAN,
			0, DWORD(pPlayer->m_nChatBanTime - m_timeCurrent),
			NAME_NULL, NAME_NULL,
			0,0,NAME_NULL,0);

		return EC_NOERROR;
	}

	switch(bGroup)
	{
/*	case CHAT_MAP:
		{
			MAPPLAYER::iterator it;
			for(it=m_mapPLAYER.begin(); it!=m_mapPLAYER.end(); it++)
				if((*it).second->m_bCountry == pPlayer->m_bCountry)
					(*it).second->SendCS_CHAT_ACK(
						bGroup,
						pPlayer->m_dwID,
						pPlayer->m_strNAME,
						strTalk);
		}
		return EC_NOERROR;
*/
	case CHAT_NEAR:
		{
			VPLAYER vPLAYERS;
			vPLAYERS.clear();

			pPlayer->m_pMAP->GetNeighbor(
				&vPLAYERS,
				pPlayer->m_fPosX,
				pPlayer->m_fPosZ);

			BYTE bInMeeting = IsMeetingRoom(pPlayer->m_wMapID, FALSE);

			while(!vPLAYERS.empty())
			{
				CTPlayer * pCHAR = vPLAYERS.back();
				if(bInMeeting || pPlayer->CanTalk(bGroup, pCHAR->m_bCountry, pCHAR->m_bAidCountry))
					pCHAR->SendCS_CHAT_ACK(
						bGroup,
						pPlayer->m_dwID,
						strSender,
						strTalk);

				vPLAYERS.pop_back();
			}
			vPLAYERS.clear();
#ifdef DEF_UDPLOG
            m_pUdpSocket->LogChat(LOGMAP_CHATNEAR, pPlayer, NULL, strTalk);
#endif
		}
		return EC_NOERROR;

	case CHAT_WHISPER:
		{
			MAPPLAYER::iterator itPlayer = m_mapPLAYER.find(dwTarget);
			if( itPlayer != m_mapPLAYER.end() &&
				pPlayer->CanTalk(bGroup, (*itPlayer).second->m_bCountry, (*itPlayer).second->m_bAidCountry))  
			{
				(*itPlayer).second->SendCS_CHAT_ACK(
					bType,
					pPlayer->m_dwID,
					strSender,
					strTalk);
#ifdef DEF_UDPLOG
                    m_pUdpSocket->LogChat(LOGMAP_CHATWISPER, pPlayer, (*itPlayer).second->m_strNAME, strTalk);
#endif
				pPlayer->SendCS_CHAT_ACK(
					bGroup,
					pPlayer->m_dwID,
					(*itPlayer).second->m_strNAME,
					strTalk);

				return EC_NOERROR;
			}

			CTPlayer * pTarget = FindPlayer(strName);
			if( pTarget &&
				pPlayer->CanTalk(bGroup, pTarget->m_bCountry, pTarget->m_bAidCountry))  
			{
				pTarget->SendCS_CHAT_ACK(
					bType,
					pPlayer->m_dwID,
					strSender,
					strTalk);
#ifdef DEF_UDPLOG
                    m_pUdpSocket->LogChat(LOGMAP_CHATWISPER, pPlayer, pTarget->m_strNAME, strTalk);
#endif

				pPlayer->SendCS_CHAT_ACK(
					bGroup,
					pPlayer->m_dwID,
					pTarget->m_strNAME,
					strTalk);

				return EC_NOERROR;
			}
		}
		break;

	case CHAT_PARTY:
		dwTarget = pPlayer->GetPartyID();
		break;

	case CHAT_GUILD:
		dwTarget = pPlayer->m_dwGuildID;
		break;
	case CHAT_TACTICS:
		dwTarget = pPlayer->GetGuild();
		break;
	case CHAT_SHOW:
	case CHAT_MAP:
	case CHAT_FORCE:
	case CHAT_WORLD:
		break;
	}
#ifdef DEF_UDPLOG
		m_pUdpSocket->LogChat(LOGMAP_CHATWISPER + bGroup, pPlayer, pPlayer->m_strGuildName, strTalk);
#endif
	SendMW_CHAT_ACK(
		pPlayer->m_bChannel,
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		strSender,
		bType,
		bGroup,
		dwTarget,
		strName,
		strTalk);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CABINETPUTIN_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bCabinetID;
	BYTE bInven;
	BYTE bItemID;
	BYTE bCount;
	BYTE bNpcInvenID;  
	BYTE bNpcItemID; 

	pBUF->m_packet
		>> bCabinetID
		>> bInven
		>> bItemID
		>> bCount
		>> bNpcInvenID  
		>> bNpcItemID;

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;

	MAPTCABINET::iterator find = pPlayer->m_mapCabinet.find(bCabinetID);
	if(find==pPlayer->m_mapCabinet.end())
		return EC_NOERROR;

	if(!(*find).second->m_bUse)
	{
		pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_NOTUSE, NULL);
		return EC_NOERROR;
	}

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
	{
		pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_NOTUSE, NULL);
		return EC_NOERROR;
	}
	
	CTItem * pItem = pInven->FindTItem(bItemID);
	if(!pItem || !pItem->m_bCount || !bCount)
	{
		pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_NOTUSE, NULL);
		return EC_NOERROR;
	}

	if((*find).second->m_mapCabinetItem.size() == CABINET_STORAGE_MAX)
	{
		BYTE bSuccess = FALSE;
		VTITEM vItem;
		vItem.clear();
		pPlayer->FindCabinetItem(bCabinetID, vItem, pItem);
		for(DWORD i=0; i<vItem.size(); i++)
		{
			if(vItem[i]->m_pTITEM->m_bStack > vItem[i]->m_bCount)
			{
				bSuccess = TRUE;
				break;
			}
		}
		if(!bSuccess)
		{
			pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_FULL, NULL);
			return EC_NOERROR;
		}
	}	

	CTItem * pNpcItem = NULL;
	CTInven * pNpcInven = NULL;
	BYTE bEqual = FALSE;
	if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
	{		
		if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
		{
			pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_INVALIDPOS,NULL);
			return EC_NOERROR;
		}

		pNpcInven = pPlayer->FindTInven(bNpcInvenID);
		if(!pNpcInven)
		{
			pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_NPCCALLERROR, NULL);
			return EC_NOERROR;
		}
		
		pNpcItem = pNpcInven->FindTItem(bNpcItemID);
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL || !pNpcItem->m_bCount)
		{
			pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_NPCCALLERROR, NULL);
			return EC_NOERROR;
		}		

		if(pNpcItem->m_pTITEM->m_wItemID == pItem->m_pTITEM->m_wItemID )
			bEqual = TRUE;
	}

	BYTE bRet = pPlayer->PutinCabinetItem(bCabinetID, bInven, bItemID, bCount);

	if(bRet && pNpcItem && !bEqual)
		UseItem(pPlayer, pNpcInven, pNpcItem, 1);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CABINETTAKEOUT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bCabinetID;
	DWORD dwStItemID;
	BYTE bCount;
	BYTE bInvenID;
	BYTE bItemID;
	BYTE bNpcInvenID;  
	BYTE bNpcItemID;
	
	pBUF->m_packet
		>> bCabinetID
		>> dwStItemID
		>> bCount
		>> bInvenID
		>> bItemID	
		>> bNpcInvenID
		>> bNpcItemID;

	DWORD dwMoney = 0;

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;

	switch(bCabinetID)
	{
	case CABINET_DEFAULT:
		dwMoney = CABINET_COST_USE1;
		break;
	case CABINET_TWO:
		dwMoney = CABINET_COST_USE2;
		break;
	case CABINET_THREE:
		dwMoney = CABINET_COST_USE3;
		break;
	}	

	if(dwMoney)
	{
		if(!pPlayer->UseMoney(dwMoney, FALSE))
		{
			pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_NEEDMONEY, NULL);
			return EC_NOERROR;
		}
	}

	CTItem * pItem = pPlayer->GetCabinetItem(bCabinetID, dwStItemID);
	if(!pItem || !bCount || pItem->m_bCount != bCount)
		return EC_NOERROR;

	CTItem * pNpcItem = NULL;
	CTInven * pNpcInven = NULL;
	BYTE bEqual = FALSE;

	if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
	{
		if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
		{
			pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_INVALIDPOS,NULL);
			return EC_NOERROR;
		}

		pNpcInven = pPlayer->FindTInven(bNpcInvenID);
		if(!pNpcInven)
		{
			pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_NPCCALLERROR, NULL);
			return EC_NOERROR;
		}
		pNpcItem = pNpcInven->FindTItem(bNpcItemID);
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL || !pNpcItem->m_bCount)
		{
			pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_NPCCALLERROR, NULL);
			return EC_NOERROR;
		}

		if(pNpcItem->m_pTITEM->m_wItemID == pItem->m_pTITEM->m_wItemID )
			bEqual = TRUE;
	}

	BYTE bRet = pPlayer->TakeoutStorageItem(bCabinetID, dwStItemID, bCount, dwMoney, bInvenID, bItemID);

	if(bRet && pNpcItem && !bEqual)
		UseItem(pPlayer, pNpcInven, pNpcItem, 1);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CABINETLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	pPlayer->SendCS_CABINETLIST_ACK();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CABINETITEMLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bCabinetID;

	pBUF->m_packet
		>> bCabinetID;

	MAPTCABINET::iterator find = pPlayer->m_mapCabinet.find(bCabinetID);
	if(find==pPlayer->m_mapCabinet.end())
		return EC_NOERROR;

	if(!(*find).second->m_bUse)
	{
		pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_NOTUSE, NULL);
		return EC_NOERROR;
	}

	pPlayer->SendCS_CABINETITEMLIST_ACK(CABINET_SUCCESS, (*find).second);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CABINETOPEN_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bCabinetID;
	
	pBUF->m_packet
		>> bCabinetID;

	LPTCABINET pTCABINET = NULL;

	MAPTCABINET::iterator find = pPlayer->m_mapCabinet.find(bCabinetID);
	if(find!=pPlayer->m_mapCabinet.end())
	{
		if((*find).second->m_bUse)
		{
			pPlayer->SendCS_CABINETOPEN_ACK(CABINET_ALREADY, bCabinetID);
			return EC_NOERROR;
		}

		pTCABINET = (*find).second;
	}
	else if(pPlayer->m_mapCabinet.size() == CABINET_COUNT ||
		bCabinetID >= CABINET_COUNT)
	{
		pPlayer->SendCS_CABINETOPEN_ACK(CABINET_MAX, bCabinetID);
		return EC_NOERROR;
	}

	DWORD dwMoney = 0;

	switch(bCabinetID)
	{
	case CABINET_DEFAULT:
		dwMoney = CABINET_COST_OPEN1;
		break;
	case CABINET_TWO:
		dwMoney = CABINET_COST_OPEN2;
		break;
	case CABINET_THREE:
		dwMoney = CABINET_COST_OPEN3;
		break;
	}

	if(dwMoney)
	{
		if(!pPlayer->UseMoney(dwMoney, FALSE))
		{
			pPlayer->SendCS_CABINETOPEN_ACK(CABINET_NEEDMONEY, bCabinetID);
			return EC_NOERROR;
		}

		pPlayer->UseMoney(dwMoney, TRUE);
		pPlayer->SendCS_MONEY_ACK();
	}

	if(!pTCABINET)
		pTCABINET = pPlayer->PutinCabinet(bCabinetID, FALSE);

#ifdef DEF_UDPLOG
    m_pUdpSocket->LogCabinet(LOGMAP_ITEMCABINETMAKE, pPlayer, bCabinetID, NULL, 0, -(int)dwMoney);
#endif

	pTCABINET->m_bUse = TRUE;
	pPlayer->SendCS_CABINETOPEN_ACK(CABINET_SUCCESS, bCabinetID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_TELEPORT_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	if( !pPlayer->m_pMAP ||
		!pPlayer->m_bMain ||
		pPlayer->m_bStatus == OS_DEAD)
	{
		pPlayer->SendCS_TELEPORT_ACK(
			TPR_INVALID,
			0,
			pPlayer->m_dwID,
			pPlayer->m_bType,
			pPlayer->m_wMapID,
			0, 0, 0);

		return EC_NOERROR;
	}

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
	{
		pPlayer->SendCS_TELEPORT_ACK(
			TPR_INVALID,
			0,
			pPlayer->m_dwID,
			pPlayer->m_bType,
			pPlayer->m_wMapID,
			0, 0, 0);

		return EC_NOERROR;
	}

	WORD wPortalID;
	WORD wNpcID;
	DWORD dwPrice = 0;

	pBUF->m_packet
		>> wNpcID
		>> wPortalID;

	LPTDESTINATION pDest = NULL;
	BYTE bMeetingRoom = 0;
	BYTE bTournamentBat = FALSE;
	CTNpc *pNpc = NULL;
	CTInven * pInven = NULL;
	CTItem * pItem = NULL;
	VTITEM vITEM;

	if(wNpcID)
	{
		MAPTNPC::iterator finder = m_mapTNpc.find(wNpcID);

		if( finder == m_mapTNpc.end() )
		{
			pPlayer->SendCS_TELEPORT_ACK(
				TPR_NOTTELEPORTNPC,
				0,
				pPlayer->m_dwID,
				pPlayer->m_bType,
				pPlayer->m_wMapID,
				0, 0, 0);

			return EC_NOERROR;
		}

		pNpc = (*finder).second;
		if(!pNpc->CanTalk(pPlayer->m_bCountry, pPlayer->m_bAidCountry, pPlayer->HaveDisguiseBuff()))
			return EC_NOERROR;

		if(!pNpc->m_pPortal)
		{
			pPlayer->SendCS_TELEPORT_ACK(
				TPR_NOPORTAL,
				0,
				pPlayer->m_dwID,
				pPlayer->m_bType,
				pPlayer->m_wMapID,
				0, 0, 0);

			return EC_NOERROR;
		}

		MAPTDESTINATION::iterator itDEST = pNpc->m_pPortal->m_mapDestination.find(wPortalID);
		if( itDEST == pNpc->m_pPortal->m_mapDestination.end() )
		{
			pPlayer->SendCS_TELEPORT_ACK(
				TPR_NODESTINATION,
				0,
				pPlayer->m_dwID,
				pPlayer->m_bType,
				pPlayer->m_wMapID,
				0, 0, 0);

			return EC_NOERROR;
		}

		pDest = (*itDEST).second;

		dwPrice = pDest->m_dwPrice; 
		BYTE bDiscountRate = GetDiscountRate(pPlayer,pNpc);
		dwPrice -= DWORD(dwPrice * bDiscountRate / 100 );

		if(!pPlayer->UseMoney( dwPrice, FALSE))  
		{
			pPlayer->SendCS_TELEPORT_ACK(
				TPR_NEEDMONEY,
				0,
				pPlayer->m_dwID,
				pPlayer->m_bType,
				pPlayer->m_wMapID,
				0, 0, 0);

			return EC_NOERROR;
		}

		if(pNpc->m_wItemID)
		{
			MAPTINVEN::iterator itInven;
			MAPTITEM::iterator itItem;
			BYTE bFound = FALSE;
			for( itInven = pPlayer->m_mapTINVEN.begin(); itInven != pPlayer->m_mapTINVEN.end(); itInven++)
			{
				for(itItem=(*itInven).second->m_mapTITEM.begin(); itItem!=(*itInven).second->m_mapTITEM.end(); itItem++)
				{
					if((*itItem).second->m_wItemID == pNpc->m_wItemID)
					{
						bFound = TRUE;
						break;
					}
				}
				if(bFound)	break;
			}

			if(!bFound)
			{
				pPlayer->SendCS_TELEPORT_ACK(
					TPR_NOITEM,
					0,
					pPlayer->m_dwID,
					pPlayer->m_bType,
					pPlayer->m_wMapID,
					0, 0, 0);

				return EC_NOERROR;
			}
		}

		WORD wNeedItem = 0;
		for(BYTE i = 0; i < PORTALCONDITION_COUNT; i++)
		{
			if(pDest->m_bCondition[i] == PCT_MEETING)
			{
				bMeetingRoom = i+1;

				if(pDest->m_dwConditionID[i])
				{
					pPlayer->SendCS_TELEPORT_ACK(
						TPR_USED,
						0,
						pPlayer->m_dwID,
						pPlayer->m_bType,
						pPlayer->m_wMapID,
						0, 0, 0);

					return EC_NOERROR;
				}

				if(!IsMainCell(DEFAULT_CHANNEL, pPlayer->m_wMapID, pPlayer->m_fPosX, pPlayer->m_fPosZ))
				{
					pPlayer->SendCS_TELEPORT_ACK(
						TPR_CHANNEL,
						0,
						pPlayer->m_dwID,
						pPlayer->m_bType,
						pPlayer->m_wMapID,
						0, 0, 0);

					return EC_NOERROR;
				}
			}

			if(pDest->m_bCondition[i] == PCT_TOURNAMENTLOUNGE)
				bTournamentBat = TRUE;

			if(pDest->m_bCondition[i] == PCT_HAVEITEM)
				wNeedItem = (WORD)pDest->m_dwConditionID[i];

			if(wNeedItem)
			{				
				MAPTINVEN::iterator itInven;
				MAPTITEM::iterator itItem;

				for( itInven = pPlayer->m_mapTINVEN.begin(); itInven != pPlayer->m_mapTINVEN.end(); itInven++)
				{
					for(itItem=(*itInven).second->m_mapTITEM.begin(); itItem!=(*itInven).second->m_mapTITEM.end(); itItem++)
					{
						if((*itItem).second->m_wItemID == wNeedItem)
						{
							pInven = (*itInven).second;
							pItem = (*itItem).second;
							vITEM.push_back(pItem);
							break;
						}
					}
					if(pItem)	break;
				}

				if(!pItem)
				{
					pPlayer->SendCS_TELEPORT_ACK(
						TPR_NOITEM,
						0,
						pPlayer->m_dwID,
						pPlayer->m_bType,
						pPlayer->m_wMapID,
						0, 0, 0);

					return EC_NOERROR;
				}				
				wNeedItem = 0;
				pItem = NULL;
			}
		}
	}

	WORD wSpawnID = 0;

	if(wPortalID)
	{
		MAPTPORTAL::iterator finder = m_mapTPortal.find(wPortalID);

		if( finder != m_mapTPortal.end() )
			wSpawnID = (*finder).second->m_wSpawnID;
	}

	if( wSpawnID && Teleport( pPlayer, wSpawnID) )
	{
		if(dwPrice)
		{
			pPlayer->UseMoney( dwPrice, TRUE);
			pPlayer->SendCS_MONEY_ACK();

			if(bTournamentBat)
				SendMW_TOURNAMENTENTERGATE_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, dwPrice, TRUE);
		}

		if(pNpc && pNpc->m_pPortal && !pPlayer->m_wMapID)
		{
			pPlayer->m_wLastSpawnID = pNpc->m_pPortal->m_wSpawnID;
			pPlayer->m_dwLastDestination = MAKELONG(pNpc->m_pPortal->m_wPortalID, wPortalID);
		}

		if(pDest && bMeetingRoom)
		{
			pDest->m_dwConditionID[bMeetingRoom-1] = pPlayer->m_dwID;
			SendSM_MEETINGROOM_REQ(TRUE, pPlayer->m_dwID);
		}

		while(!vITEM.empty())
		{
			CTItem * pNeedItem = vITEM.back();
			UseItem(pPlayer, pInven, pNeedItem, 1);
			vITEM.pop_back();
		}
			

#ifdef DEF_UDPLOG	
		m_pUdpSocket->LogTeleport(LOGMAP_TELEPORT, pPlayer, pNpc, -int(dwPrice), wPortalID);
#endif

	}
	else
	{
		pPlayer->SendCS_TELEPORT_ACK(
			TPR_NODESTINATION,
			0,
			pPlayer->m_dwID,
			pPlayer->m_bType,
			pPlayer->m_wMapID,
			0, 0, 0);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_NPCITEMLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wNpcID;
	pBUF->m_packet
		>> wNpcID;

	MAPTNPC::iterator it = m_mapTNpc.find(wNpcID);
	if(it==m_mapTNpc.end())
		return EC_NOERROR;

	CTNpc * pNpc = (*it).second;
	if(!pNpc->CanTalk(pPlayer->m_bCountry, pPlayer->m_bAidCountry, pPlayer->HaveDisguiseBuff()))
		return EC_NOERROR;

	pPlayer->SendCS_NPCITEMLIST_ACK(pNpc);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_ITEMBUY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wNpcID;
	DWORD dwQuestID;
	WORD wItemID;
	BYTE bCount;
	BYTE bNpcInvenID;  
	BYTE bNpcItemID;

	pBUF->m_packet
		>> wNpcID
		>> dwQuestID
		>> wItemID
		>> bCount
		>> bNpcInvenID  
		>> bNpcItemID;

	if(pPlayer->m_dealItem.m_bStatus >= DEAL_START)
	{
		pPlayer->SendCS_ITEMBUY_ACK(
			ITEMBUY_DEALING,
			wItemID);
		return EC_NOERROR;
	}

	LPTITEM pItem = NULL;
	CTNpc * pNpc = FindTNpc(wNpcID);
	if(!pNpc || !bCount)
	{
		pPlayer->SendCS_ITEMBUY_ACK(
			ITEMBUY_NOTFOUND,
			wItemID);
		return EC_NOERROR;
	}

	if(!pNpc->CanTalk(pPlayer->m_bCountry, pPlayer->m_bAidCountry, pPlayer->HaveDisguiseBuff()))
		return EC_NOERROR;

	LPQUESTTEMP pQUEST = FindQuestTemplate(dwQuestID);
	if(pQUEST)
	{
		BYTE bLevel;
		if(!pPlayer->CanRunQuest(pQUEST, m_dwTick, bLevel))
		{
			for( int i=0; i<INT(pQUEST->m_vTerm.size()); i++)
				if( pQUEST->m_vTerm[i]->m_bTermType == QTT_ITEMID &&
					pQUEST->m_vTerm[i]->m_dwTermID == wItemID)
				{
					pItem = FindTItem(wItemID);
				}
		}
	}
	else
        pItem = pNpc->GetItem(wItemID);

	if(!pItem)
	{
		pPlayer->SendCS_ITEMBUY_ACK(
			ITEMBUY_NOTFOUND,
			wItemID);
		return EC_NOERROR;
	}

	if(pItem->m_bStack < bCount)
		bCount = pItem->m_bStack;

	DWORD dwBuyPrice = 0;

	if(!dwQuestID )
	{
		if(pNpc->m_bType == TNPC_PVPOINT)
		{
			dwBuyPrice = GetItemPvPrice(pItem) * bCount;		
			BYTE bDiscountRate = GetDiscountRate(pPlayer,pNpc);
			dwBuyPrice -= DWORD(dwBuyPrice * bDiscountRate / 100 );

			if(pPlayer->m_dwPvPUseablePoint < dwBuyPrice)
			{
				pPlayer->SendCS_ITEMBUY_ACK(
					ITEMBUY_NEEDMONEY,
					wItemID);
				return EC_NOERROR;
			}
		}
		else
		{
			dwBuyPrice = GetItemPrice(pItem) * bCount;
			BYTE bDiscountRate = GetDiscountRate(pPlayer,pNpc);
			dwBuyPrice -= DWORD(dwBuyPrice * bDiscountRate / 100 );

			if( !pPlayer->UseMoney(dwBuyPrice, FALSE))
			{
				pPlayer->SendCS_ITEMBUY_ACK(
					ITEMBUY_NEEDMONEY,
					wItemID);
				return EC_NOERROR;
			}
		}
	}

	VTITEM vItem;
	vItem.clear();
	CTItem * pNew = new CTItem();
	pNew->m_dlID = GenItemID();
	pNew->m_bCount = bCount;
	pNew->m_wItemID = wItemID;
	pNew->m_pTITEM = pItem;
	pNew->SetDuration(FALSE);
	SetItemAttr(pNew, 0);
	vItem.push_back(pNew);
	if(!pPlayer->CanPush(&vItem, 0))
	{
		pPlayer->SendCS_ITEMBUY_ACK(
			ITEMBUY_CANTPUSH,
			wItemID);
		delete pNew;
		return EC_NOERROR;
	}

	if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
	{
		if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
		{
			pPlayer->SendCS_ITEMBUY_ACK(ITEMBUY_INVALIDPOS,0);
			delete pNew;
			return EC_NOERROR;
		}

		CTItem * pNpcItem = NULL;
		CTInven * pNpcInven = NULL;

		pNpcInven = pPlayer->FindTInven(bNpcInvenID);
		if(!pNpcInven)
		{
			pPlayer->SendCS_ITEMBUY_ACK(ITEMBUY_NPCCALLERROR,0);
			delete pNew;
			return EC_NOERROR;
		}
		pNpcItem = pNpcInven->FindTItem(bNpcItemID);
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL || !pNpcItem->m_bCount)
		{
			pPlayer->SendCS_ITEMBUY_ACK(ITEMBUY_NPCCALLERROR,0);
			delete pNew;
			return EC_NOERROR;
		}

		UseItem(pPlayer, pNpcInven, pNpcItem, 1);		
	}

#ifdef DEF_UDPLOG
	if(dwQuestID)
	{
		m_pUdpSocket->LogItemByNPC(LOGMAP_ITEMQUESTGET, pPlayer, pNpc, pNew, 0, pQUEST);
	}
	else
	{
		m_pUdpSocket->LogItemByNPC(LOGMAP_ITEMBUY, pPlayer, pNpc, pNew, -(int)dwBuyPrice);
	}
#endif	//	DEF_UDPLOG

	pPlayer->PushTItem(&vItem);
	vItem.clear();
	if(!dwQuestID)
	{
		if(pNpc->m_bType == TNPC_PVPOINT)
			pPlayer->UsePvPoint(dwBuyPrice, PVPE_BUYITEM, PVP_USEABLE);
		else
			pPlayer->UseMoney(dwBuyPrice, TRUE);
	}

	pPlayer->SendCS_ITEMBUY_ACK(
		ITEMBUY_SUCCESS,
		wItemID);

	CheckQuest(
		pPlayer,
		0,
		pPlayer->m_fPosX,
		pPlayer->m_fPosY,
		pPlayer->m_fPosZ,
        wItemID,
		QTT_GETITEM,
		TT_GETITEM,
		bCount);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_MAGICITEMBUY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wNpcID;
	DWORD dwQItemID;
	BYTE bCount;
	BYTE bNpcInvenID;  
	BYTE bNpcItemID;

	pBUF->m_packet
		>> wNpcID
		>> dwQItemID
		>> bCount
		>> bNpcInvenID  
		>> bNpcItemID;

	if(pPlayer->m_dealItem.m_bStatus >= DEAL_START)
	{
		pPlayer->SendCS_MAGICITEMBUY_ACK(ITEMBUY_DEALING);
		return EC_NOERROR;
	}

	CTNpc * pNpc = FindTNpc(wNpcID);
	if(!pNpc || !bCount)
	{
		pPlayer->SendCS_MAGICITEMBUY_ACK(ITEMBUY_NOTFOUND);
		return EC_NOERROR;
	}

	CTItem * pMagicItem = pNpc->GetMagicItem(dwQItemID);
	if(!pMagicItem)
	{
		pPlayer->SendCS_MAGICITEMBUY_ACK(ITEMBUY_NOTFOUND);
		return EC_NOERROR;
	}

	if(pMagicItem->m_pTITEM->m_bStack < bCount)
		bCount = pMagicItem->m_pTITEM->m_bStack;

	DWORD dwBuyPrice = pMagicItem->m_dwMoney * bCount;
	BYTE bDiscountRate = GetDiscountRate(pPlayer,pNpc);
	dwBuyPrice -= DWORD(dwBuyPrice * bDiscountRate / 100 );

	if( !pPlayer->UseMoney(dwBuyPrice, FALSE))
	{
		pPlayer->SendCS_MAGICITEMBUY_ACK(ITEMBUY_NEEDMONEY);
		return EC_NOERROR;
	}

	VTITEM vItem;
	vItem.clear();
	CTItem * pNew = new CTItem();
	pNew->Copy(pMagicItem, TRUE);

	CTime time(_AtlModule.m_timeCurrent);
	time += CTimeSpan(LONG(pNew->m_dEndTime),0,0,0);
	pNew->m_dEndTime = time.GetTime();

	vItem.push_back(pNew);
	if(!pPlayer->CanPush(&vItem, 0))
	{
		pPlayer->SendCS_MAGICITEMBUY_ACK(ITEMBUY_CANTPUSH);
		delete pNew;
		return EC_NOERROR;
	}

	if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
	{
		if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
		{
			pPlayer->SendCS_MAGICITEMBUY_ACK(ITEMBUY_INVALIDPOS);
			delete pNew;
			return EC_NOERROR;
		}

		CTInven * pNpcInven = pPlayer->FindTInven(bNpcInvenID);
		if(!pNpcInven)
		{
			pPlayer->SendCS_MAGICITEMBUY_ACK(ITEMBUY_NPCCALLERROR);
			delete pNew;
			return EC_NOERROR;
		}

		CTItem * pNpcItem = pNpcInven->FindTItem(bNpcItemID);
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL || !pNpcItem->m_bCount)
		{
			pPlayer->SendCS_MAGICITEMBUY_ACK(ITEMBUY_NPCCALLERROR);
			delete pNew;
			return EC_NOERROR;
		}

		UseItem(pPlayer, pNpcInven, pNpcItem, 1);		
	}

#ifdef DEF_UDPLOG
	m_pUdpSocket->LogItemByNPC(LOGMAP_ITEMBUY, pPlayer, pNpc, pNew, -(int)dwBuyPrice);		
#endif	//	DEF_UDPLOG

	pPlayer->PushTItem(&vItem);
	vItem.clear();

	pPlayer->UseMoney(dwBuyPrice, TRUE);
	pPlayer->SendCS_MAGICITEMBUY_ACK(ITEMBUY_SUCCESS);
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_ITEMSELL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bInven;
	BYTE bPos;
	BYTE bCount;
	BYTE bNpcInvenID; 
	BYTE bNpcItemID;

	pBUF->m_packet
		>> bInven
		>> bPos
		>> bCount
		>> bNpcInvenID  
		>> bNpcItemID;

	if(pPlayer->m_bStore)
		return EC_NOERROR;

	if(bInven == INVEN_EQUIP)
		return EC_NOERROR;

	if(pPlayer->m_dealItem.m_bStatus >= DEAL_START)
	{
		pPlayer->SendCS_ITEMSELL_ACK(ITEMSELL_DEALING);
		return EC_NOERROR;
	}

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
	{
		pPlayer->SendCS_ITEMSELL_ACK(ITEMSELL_NOTFOUND);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bPos);
	if(!pItem || !bCount || !pItem->m_bCount)
	{
		pPlayer->SendCS_ITEMSELL_ACK(ITEMSELL_NOTFOUND);
		return EC_NOERROR;
	}

	WORD wItemID = pItem->m_wItemID;

	if(pItem->m_bCount < bCount)
	{
		pPlayer->SendCS_ITEMSELL_ACK(ITEMSELL_NOTFOUND);
		return EC_NOERROR;
	}

	if(!(pItem->m_pTITEM->m_bIsSell & ITEMTRADE_SELL))
	{
		pPlayer->SendCS_ITEMSELL_ACK(ITEMSELL_CANTSELL);
		return EC_NOERROR;
	}

	 	
	if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
	{
		if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
		{
			pPlayer->SendCS_ITEMSELL_ACK(ITEMSELL_INVALIDPOS);
			return EC_NOERROR;
		}

		CTItem * pNpcItem = NULL;
		CTInven * pNpcInven = NULL;

		pNpcInven = pPlayer->FindTInven(bNpcInvenID);
		if(!pNpcInven)
		{
			pPlayer->SendCS_ITEMSELL_ACK(ITEMSELL_NPCCALLERROR);
			return EC_NOERROR;
		}
		pNpcItem = pNpcInven->FindTItem(bNpcItemID);
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL || !pNpcItem->m_bCount)
		{
			pPlayer->SendCS_ITEMSELL_ACK(ITEMSELL_NPCCALLERROR);
			return EC_NOERROR;
		}

		UseItem(pPlayer, pNpcInven, pNpcItem, 1);
	}
	
	DWORD dwBuyPrice = pItem->GetPrice()/4 * bCount;
	WORD wAddPrice = pPlayer->HaveItemProbBuff(SDT_STATUS_PRICEUP);
	if(wAddPrice)
		dwBuyPrice = dwBuyPrice + dwBuyPrice * wAddPrice / 100;

	pPlayer->EarnMoney(dwBuyPrice);

	if(pItem->m_bCount == bCount)
	{
#ifdef DEF_UDPLOG
		m_pUdpSocket->LogItemByNPC(LOGMAP_ITEMSELL, pPlayer, NULL, pItem, dwBuyPrice);
#endif	//	DEF_UDPLOG

		delete pItem;
		pInven->m_mapTITEM.erase(bPos);
		pPlayer->SendCS_DELITEM_ACK(bInven, bPos);
	}
	else
	{
#ifdef	DEF_UDPLOG

		BYTE bTempCount =  pItem->m_bCount -= bCount;

		pItem->m_bCount	= bCount;

		m_pUdpSocket->LogItemByNPC(LOGMAP_ITEMSELL, pPlayer, NULL, pItem, dwBuyPrice);
        

		pItem->m_bCount = bTempCount;

		pPlayer->SendCS_UPDATEITEM_ACK(
			pItem,
			bInven);
#else
		pItem->m_bCount -= bCount;
		pPlayer->SendCS_UPDATEITEM_ACK(
			pItem,
			bInven);
#endif	//	DEF_UDPLOG

	}

	pPlayer->SendCS_ITEMSELL_ACK(ITEMSELL_SUCCESS);
	pPlayer->CheckQuest(
		m_dwTick,
		wItemID,
		QTT_GETITEM,
		0);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_MONITEMLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	DWORD dwMonID;
	BYTE bWant;

	pBUF->m_packet
		>> bWant
		>> dwMonID;

	if(pPlayer->m_dwRiding)
		return EC_NOERROR;

	CTMonster *pMon = pPlayer->m_pMAP->FindMonster(dwMonID);
	if(!pMon)
		return EC_NOERROR;

	if( (pMon->m_bKeeperType == OWNER_PARTY && pMon->m_dwKeeperID != pPlayer->GetPartyID()) ||
		(pMon->m_bKeeperType == OWNER_PRIVATE && pMon->m_dwKeeperID != pPlayer->m_dwID) )
	{
		pPlayer->SendCS_MONITEMLIST_ACK(
			MIL_CANTACCESS,
			dwMonID, 0,
			NULL,
			FALSE);

		return EC_NOERROR;
	}

	if( !bWant &&
		(!pMon->m_dwInvenLock || pMon->m_dwInvenLock == pPlayer->m_dwID ))
	{
		pPlayer->m_dwLockedMonID = 0;
		pMon->m_dwInvenLock = 0;

		return EC_NOERROR;
	}

	CTInven *pInven = pMon->FindTInven(INVEN_DEFAULT);
	if(!pInven)
		return EC_NOERROR;

	if(pMon->m_dwInvenLock)
	{
		pPlayer->SendCS_MONITEMLIST_ACK(
			MIL_CANTACCESS,
			dwMonID, 0,
			NULL,
			FALSE);

		return EC_NOERROR;
	}

	if(pMon->m_bKeeperType == OWNER_PARTY)
	{
		if(pPlayer->m_bPartyType == PT_HUNTER &&
			pPlayer->m_dwID != pMon->GetHunter())
		{
			pPlayer->SendCS_MONITEMLIST_ACK(
				MIL_CANTACCESS,
				dwMonID, 0,
				NULL,
				FALSE);

			return EC_NOERROR;
		}
	}

	pMon->m_dwInvenLock = pPlayer->m_dwID;
	pPlayer->m_dwLockedMonID = dwMonID;

	pPlayer->SendCS_MONITEMLIST_ACK(
		MIL_SUCCESS,
		dwMonID,
		pMon->m_dwMoney,
		pInven,
		FALSE);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_MONMONEYTAKE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	DWORD dwMonID;

	if( !pPlayer->m_pMAP )
		return EC_NOERROR;

	pBUF->m_packet
		>> dwMonID;

	MonMoneyTake(pPlayer, dwMonID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_MONITEMTAKE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP )
		return EC_NOERROR;

	DWORD dwMonID;
	BYTE bItemID;
	BYTE bInvenID;
	BYTE bSlotID;

	pBUF->m_packet
		>> dwMonID
		>> bItemID
		>> bInvenID
		>> bSlotID;

	MonItemTake(pPlayer, dwMonID, bItemID, bInvenID, bSlotID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_MONITEMTAKEALL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP )
		return EC_NOERROR;

	DWORD dwMonID;
	pBUF->m_packet
		>> dwMonID;

	if(MonMoneyTake(pPlayer, dwMonID))
		MonItemTake(pPlayer, dwMonID, INVALID_SLOT, INVEN_NULL, INVALID_SLOT);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_MONITEMLOTTERY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bAnswer;
	DWORD dwMonID;
	WORD wItemID;

	pBUF->m_packet
		>> bAnswer
		>> dwMonID
		>> wItemID;

	CTMonster * pMon = pPlayer->m_pMAP->FindMonster(dwMonID);
	if(!pMon)
		return EC_NOERROR;

	CTInven * pInven = pMon->FindTInven(INVEN_DEFAULT);
	CTItem * pItem = NULL;
	MAPTITEM::iterator it;
	if(pInven)
	{
		for(it=pInven->m_mapTITEM.begin(); it!=pInven->m_mapTITEM.end(); it++)
		{
			if((*it).second->m_wItemID == wItemID)
			{
				pItem = (*it).second;
				break;
			}
		}
	}
	if(!pItem)
		return EC_NOERROR;

	if(bAnswer)
		pItem->m_mapRoutingJoiner.insert(MAPDWORD::value_type(pPlayer->m_dwID, pPlayer->m_dwKEY));
	else
	{
		if(pItem->m_bMaxRouting)
            pItem->m_bMaxRouting--;
	}

	if(pItem->m_bMaxRouting == pItem->m_mapRoutingJoiner.size())
		pMon->Lottery(pItem);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_FRIENDLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	SendMW_FRIENDLIST_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_FRIENDASK_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	CString strTarget;

	pBUF->m_packet
		>> strTarget;

	if(pPlayer->m_strNAME == strTarget)
		return EC_NOERROR;

	// 차단검사
	MAPTPROTECTED::iterator it;
	for(it=pPlayer->m_mapTPROTECTED.begin(); it!=pPlayer->m_mapTPROTECTED.end(); it++)
	{
		if((*it).second->m_strName == strTarget)
		{
			pPlayer->SendCS_FRIENDADD_ACK(FRIEND_REFUSE, 0, strTarget, 0,0,0,0);
			return EC_NOERROR;
		}
	}

	SendMW_FRIENDASK_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		strTarget);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_FRIENDREPLY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	CString strInviter;
	BYTE bReply;

	pBUF->m_packet
		>> strInviter
		>> bReply;

	SendMW_FRIENDREPLY_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		strInviter,
		bReply);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_FRIENDERASE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwTarget;
	pBUF->m_packet
		>> dwTarget;

	SendMW_FRIENDERASE_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwTarget);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_HOTKEYADD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bType;
	WORD wID;
	BYTE bInven;
	BYTE bPos;

	pBUF->m_packet
		>> bType
		>> wID
		>> bInven
		>> bPos;

	pPlayer->AddHotKey(bInven, bPos, bType, wID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_HOTKEYDEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bInven;
	BYTE bPos;

	pBUF->m_packet
		>> bInven
		>> bPos;

	pPlayer->EraseHotKey(bInven, bPos);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_CHARSTATINFO_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwCharID;
	pBUF->m_packet
		>> dwCharID;

	MAPPLAYER::iterator finder = m_mapPLAYER.find(dwCharID);
	if(finder != m_mapPLAYER.end() &&
		(*finder).second->m_bMain )
		pPlayer->SendCS_CHARSTATINFO_ACK((*finder).second);
	else
		SendMW_CHARSTATINFO_ACK(pPlayer->m_dwID, dwCharID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_CANCELACTION_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwID;
	BYTE bType;

	pBUF->m_packet
		>> dwID
		>> bType;

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	pPlayer->m_pMAP->GetNeighbor(
		&vPLAYERS,
		pPlayer->m_fPosX,
		pPlayer->m_fPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_CANCELACTION_ACK(
			dwID,
			bType);

		vPLAYERS.pop_back();
	}
	vPLAYERS.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_ITEMUPGRADE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->m_dealItem.m_bStatus != DEAL_READY)
		return EC_NOERROR;

	if(pPlayer->m_bStore)
		return EC_NOERROR;

	BYTE bTargetInven;
	BYTE bTargetItemID;
	BYTE bGradeInven;
	BYTE bGradeItemID;
	WORD wNpcID;
	WORD wColor;
	__int64 llMoney = 0;

	BYTE bNpcInvenID;  
	BYTE bNpcItemID;

	pBUF->m_packet
		>> bTargetInven
		>> bTargetItemID
		>> bGradeInven
		>> bGradeItemID
		>> wNpcID
		>> bNpcInvenID
		>> bNpcItemID
		>> wColor;

	CTInven * pInven = pPlayer->FindTInven(bTargetInven);
	CTInven * pGradeInven = pPlayer->FindTInven(bGradeInven);
	if(!pInven || !pGradeInven)
		return EC_NOERROR;

	CTItem * pItem = pInven->FindTItem(bTargetItemID);
	CTItem * pGradeItem = pGradeInven->FindTItem(bGradeItemID);
	if(!pItem || !pGradeItem || pItem->m_bGLevel)
	{
		pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGREAD_NOITEM, 0, 0, 0);
		return EC_NOERROR;
	}

	if(!pItem->m_bCount || !pGradeItem->m_bCount)
	{
		pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGREAD_NOITEM, 0, 0, 0);
		return EC_NOERROR;
	}

	if(pGradeItem->m_pTITEM->m_bType != IT_GRADE)
	{
        pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGREAD_NOITEM, 0, 0, 0);
		return EC_NOERROR;
	}

	if(!pItem->CanUse() || !pGradeItem->CanUse())
	{
		pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGREAD_NOITEM, 0, 0, 0);
		return EC_NOERROR;
	}

	CTNpc* pNpc = FindTNpc(wNpcID);
	if(!pNpc)
	{
		pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_NPCCALLERROR,0,0,0);
		return EC_NOERROR;
	}

	BYTE bDiscountRate = GetDiscountRate(pPlayer, pNpc);
	WORD wGradeUseValue = pGradeItem->m_pTITEM->m_wUseValue;

	switch(pGradeItem->m_pTITEM->m_bKind)
	{
	case IK_UPGRADE:
	case IK_DOWNGRADE:
		{
			if(m_itemgrade[pItem->m_bLevel].m_bProb == 0)
			{
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGREAD_NOGRADE, 0, 0, 0);
				return EC_NOERROR;
			}

			if(!pItem->m_pTITEM->m_bCanGrade)
			{
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGREAD_NOGRADE, 0, 0, 0);
				return EC_NOERROR;
			}

			MAPTMAGIC::iterator itMg;
			for(itMg=pItem->m_mapTMAGIC.begin(); itMg != pItem->m_mapTMAGIC.end(); itMg++)
			{
				if((*itMg).second->m_pMagic->m_bOptionKind)
				{
					pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGREAD_NOGRADE, 0, 0, 0);
					return EC_NOERROR;
				}
			}

			if(pGradeItem->m_pTITEM->m_bKind==IK_UPGRADE)
			{
				llMoney = m_itemgrade[pItem->m_bLevel].m_dwMoney;			
				llMoney -= DWORD(llMoney * bDiscountRate / 100 ); 
					
				if(!pPlayer->UseMoney(llMoney, FALSE))
				{
					pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_MONEY, 0 , 0, 0);
					return EC_NOERROR;
				}
			}
		}
		break;
	case IK_MAGICGRADE:
		{
			if( !pItem->m_pTITEM->m_bCanMagic || pItem->m_bLevel ||	pItem->m_mapTMAGIC.size() )
			{
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGREAD_NOGRADE, 0, 0, 0);
				return EC_NOERROR;
			}
		}
		break;
	case IK_RAREGRADE:
		{
			if( !pItem->m_pTITEM->m_bCanRare ||	pItem->m_bLevel ||
				!pItem->m_mapTMAGIC.size() || pItem->m_mapTMAGIC.size() > 2 )
			{
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGREAD_NOGRADE, 0, 0, 0);
				return EC_NOERROR;
			}
		}
		break;
	case IK_CLEARMAGIC:
		{
			if(pItem->m_mapTMAGIC.empty())
			{
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGREAD_NOGRADE, 0, 0, 0);
				return EC_NOERROR;
			}
		}
		break;
	case IK_WRAP:
		{
			if(!pItem->m_pTITEM->m_bCanWrap || pItem->m_dwExtValue[IEV_WRAP])
			{
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_WRAPPING, 0, 0, 0);
				return EC_NOERROR;
			}
		}
		break;
	case IK_ELD:
		{
			if(pItem->m_dwExtValue[IEV_ELD] >= TMAX_ITEMLEVELDOWN || pItem->GetEquipLevel() == 1)
			{
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_MAXELD, 0, 0, 0);
				return EC_NOERROR;
			}
		}
		break;
	case IK_CLEARREFINE:
		{
			if(!pItem->m_bRefineCur)
			{
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_NOREFINE, 0, 0, 0);
				return EC_NOERROR;
			}
		}
		break;
	case IK_CHGGRADEEFFECT:
		{
			if(pItem->m_bLevel < MIN_GRADE_EFFECT_LEVEL)
			{
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_NOGRADEEFFECT, 0, 0, 0);
				return EC_NOERROR;
			}
		}
		break;
	case IK_COLOR:
		{
			if(!pItem->m_pTITEM->m_bCanColor || pItem->m_dwExtValue[IEV_COLOR] == wColor)
			{
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_SAMECOLOR, 0, 0, 0);
				return EC_NOERROR;
			}
		}
		break;
	default:
		{
			pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGREAD_NOGRADE, 0, 0, 0);
			return EC_NOERROR;
		}
	}

	if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
	{
		if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
		{
			pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_INVALIDPOS,0,0,0);
			return EC_NOERROR;
		}

		CTItem * pNpcItem = NULL;
		CTInven * pNpcInven = NULL;

		pNpcInven = pPlayer->FindTInven(bNpcInvenID);
		if(!pNpcInven)
		{
			pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_NPCCALLERROR, 0, 0, 0);
			return EC_NOERROR;
		}

		pNpcItem = pNpcInven->FindTItem(bNpcItemID);
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL || !pNpcItem->m_bCount)
		{
			pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_NPCCALLERROR, 0, 0, 0);
			return EC_NOERROR;
		}

		UseItem(pPlayer, pNpcInven, pNpcItem, 1);
	}

	if(llMoney)
		pPlayer->UseMoney(llMoney, TRUE);

#ifdef DEF_UDPLOG
	BYTE bTempCount = pGradeItem->m_bCount;
	pGradeItem->m_bCount = 1;
	m_pUdpSocket->LogItemUpgrade(LOGMAP_ITEMUSE, pPlayer, pGradeItem );
	pGradeItem->m_bCount = bTempCount;
#endif	//	DEF_UDPLOG

	BYTE bItemKind = pGradeItem->m_pTITEM->m_bKind;
	BYTE bItemGrade = pGradeItem->m_pTITEM->m_bGrade;

	// 주문서 처리
	pGradeItem->m_bCount--;
	if(!pGradeItem->m_bCount)
	{
		delete pGradeItem;
		pGradeInven->m_mapTITEM.erase(bGradeItemID);
		pPlayer->SendCS_DELITEM_ACK(bGradeInven, bGradeItemID);
	}
	else
		pPlayer->SendCS_UPDATEITEM_ACK(pGradeItem, bGradeInven);

	switch(bItemKind)
	{
	case IK_UPGRADE:
		{
			BYTE bRand = BYTE(rand() % 100);
			BYTE bProb = CalcProb(pPlayer, pNpc, PROB_UPGRADE, m_itemgrade[pItem->m_bLevel].m_bProb);

			if(bRand < bProb)
			{
				BYTE bGrade = bItemGrade == 3 ? BYTE(rand() % 3) + 1 : 1;
				pItem->m_bLevel += bGrade;
				
				if(pItem->m_bLevel >= MIN_GRADE_EFFECT_LEVEL && pItem->m_bGradeEffect == 0)
					pItem->m_bGradeEffect = BYTE(rand()% (IE_COUNT-1) ) + 1;

				if(pItem->m_bLevel > ITEMLEVEL_MAX)
					pItem->m_bLevel = ITEMLEVEL_MAX;

				SetItemAttr(pItem, pItem->m_bLevel);
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_SUCCESS, bTargetInven, bTargetItemID, pItem->m_bLevel, pItem->m_bGradeEffect);

	#ifdef	DEF_UDPLOG
				m_pUdpSocket->LogItemUpgrade(LOGMAP_ITEMUPGRADESUCCESS, pPlayer, pItem, bGrade );
	#endif
			}
			else
			{
	#ifdef	DEF_UDPLOG			
				m_pUdpSocket->LogItemUpgrade(LOGMAP_ITEMUPGRADEFAILDEL, pPlayer, pItem);
	#endif	//	DEF_UDPLOG

				if(pPlayer->ProtectTutorial() || CalcProb(pPlayer, pNpc, PROB_ITEMGUARD, 0))
				{
					BYTE bLevelGuard = 11;
					BYTE bDownProb = 0;
					BYTE bDownGrade = 0;

					if(pItem->m_bLevel <= bLevelGuard)
						bDownProb = 0;
					else
						bDownProb = rand() % 100;

					if(bDownProb < 20)
						bDownGrade = 0;
					else if(bDownProb < 55)
						bDownGrade = 1;
					else
						bDownGrade = 2;

					if(pItem->m_bLevel  > bLevelGuard && pItem->m_bLevel - bDownGrade < bLevelGuard)
						bDownGrade = pItem->m_bLevel - bLevelGuard;

					if(bDownGrade)
					{
						pItem->m_bLevel = pItem->m_bLevel > bDownGrade ? pItem->m_bLevel - bDownGrade : 0;
						SetItemAttr(pItem, pItem->m_bLevel);
						if(pItem->m_bLevel < MIN_GRADE_EFFECT_LEVEL )
							pItem->m_bGradeEffect = 0;
					}

					pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_DOWNGRADE, bTargetInven, bTargetItemID, pItem->m_bLevel, pItem->m_bGradeEffect);
				}
				else
				{
					delete pItem;
					pInven->m_mapTITEM.erase(bTargetItemID);
					pPlayer->SendCS_DELITEM_ACK(bTargetInven, bTargetItemID);
					pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_FAIL, 0, 0, 0);
				}
			}
		}
		break;
	case IK_DOWNGRADE: // 정화주문서
		{
			if(pItem->m_bLevel)
			{
				pItem->m_bLevel = pItem->m_bLevel - bItemGrade;

				if(pItem->m_bLevel < MIN_GRADE_EFFECT_LEVEL )
					pItem->m_bGradeEffect = 0;

				SetItemAttr(pItem, pItem->m_bLevel);
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_SUCCESS, bTargetInven, bTargetItemID, pItem->m_bLevel, pItem->m_bGradeEffect);

	#ifdef	DEF_UDPLOG
				m_pUdpSocket->LogItemUpgrade(LOGMAP_ITEMDOWNGRADESUCCESS, pPlayer, pItem, pItem->m_bLevel);
	#endif
			}
			else
			{
				pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_FAIL, 0, 0, 0);

	#ifdef DEF_UDPLOG			
				m_pUdpSocket->LogItemUpgrade(LOGMAP_ITEMDOWNGRADEFAIL, pPlayer, pItem );
	#endif
			}
		}
		break;
	case IK_MAGICGRADE:
	case IK_RAREGRADE: // 제작주문서
		{
			BYTE bResult = ITEMUPGRADE_MAGICFAIL;

			WORD wMagicBuff = pPlayer->HaveItemProbBuff(SDT_STATUS_MAGICPROB);
			 
			BYTE bProb = 0;
			if(bItemKind == IK_MAGICGRADE)
                bProb = CalcProb(pPlayer, pNpc, PROB_MAGIC, bItemGrade);
			else if(bItemKind == IK_RAREGRADE)
				bProb = CalcProb(pPlayer, pNpc, PROB_RAREMAGIC, bItemGrade);

			BYTE bRand = BYTE(rand() % 100);

			if(bRand < bProb)
			{
	#ifdef DEF_UDPLOG	
				DWORD dwLogResult = LOGMAP_ITEMMAKEZERO;
	#endif
				if(MakeSpecialItem(pPlayer, pItem, bItemKind, wMagicBuff, IMT_SCROLL))
				{
	#ifdef DEF_UDPLOG	
					dwLogResult = LOGMAP_ITEMMAKESUCCESS;
	#endif
					bResult = ITEMUPGRADE_SUCCESS;
				}

	#ifdef DEF_UDPLOG			
				m_pUdpSocket->LogItemUpgrade(dwLogResult, pPlayer, pItem );
	#endif
			}

			if(!bResult)
				pPlayer->SendCS_ITEMMAGICGRADE_ACK(bResult, bTargetInven, pItem);
			else
			{
				if(bItemKind == IK_RAREGRADE) // 레어일 경우 아이탬 제거
				{
	#ifdef	DEF_UDPLOG
					m_pUdpSocket->LogItemUpgrade(LOGMAP_ITEMMAKEFAILDEL, pPlayer, pItem);
	#endif

					delete pItem;
					pInven->m_mapTITEM.erase(bTargetItemID);
					pPlayer->SendCS_DELITEM_ACK(bTargetInven, bTargetItemID);
				}
				pPlayer->SendCS_ITEMMAGICGRADE_ACK(bResult, 0, NULL);
			}
		}
		break;
	case IK_CLEARMAGIC:
		{
			MAPTMAGIC::iterator itMg;
			for(itMg=pItem->m_mapTMAGIC.begin(); itMg!=pItem->m_mapTMAGIC.end(); itMg++)
				delete (*itMg).second;
			pItem->m_mapTMAGIC.clear();

			pPlayer->SendCS_ITEMMAGICGRADE_ACK(ITEMUPGRADE_SUCCESS_MAGICCLEAR, bTargetInven, pItem);
		}
		break;
	case IK_WRAP:
		{
			pItem->m_dwExtValue[IEV_WRAP] = TRUE;
			pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_SUCCESS_WRAP, bTargetInven, bTargetItemID, BYTE(pItem->m_dwExtValue[IEV_WRAP]), pItem->m_bGradeEffect);
		}
		break;
	case IK_ELD:
		{
			BYTE bELD = BYTE(rand() % max(wGradeUseValue, 1));
			pItem->m_dwExtValue[IEV_ELD] += max(bELD, 1);
			if(pItem->m_dwExtValue[IEV_ELD] > TMAX_ITEMLEVELDOWN)
				pItem->m_dwExtValue[IEV_ELD] = TMAX_ITEMLEVELDOWN;
	
			if(pItem->m_pTITEM->m_bDefaultLevel <= pItem->m_dwExtValue[IEV_ELD])
				pItem->m_dwExtValue[IEV_ELD] = pItem->m_pTITEM->m_bDefaultLevel-1;

			pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_SUCCESS_ELD, bTargetInven, bTargetItemID, BYTE(pItem->m_dwExtValue[IEV_ELD]), pItem->m_bGradeEffect);
		}
		break;
	case IK_CLEARREFINE:
		{
			pItem->m_dwDuraMax = pItem->m_pTITEM->m_dwDuraMax;
			pItem->m_dwDuraCur = pItem->m_pTITEM->m_dwDuraMax;
			pItem->m_bRefineCur = 0;

			MAPTMAGIC::iterator it;
			for(it=pItem->m_mapTMAGIC.begin(); it!=pItem->m_mapTMAGIC.end(); it++)
				delete (*it).second;

			pItem->m_mapTMAGIC.clear();

			pPlayer->SendCS_CHANGEITEMATTR_ACK(bTargetInven, pItem);
			pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_SUCCESS_CLEARREFINE, bTargetInven, bTargetItemID, pItem->m_bLevel, pItem->m_bGradeEffect);
		}
		break;
	case IK_CHGGRADEEFFECT:
		{
			BYTE bEffect = BYTE(rand()% (IE_COUNT-1) ) + 1;
			if(bEffect == pItem->m_bGradeEffect)
				pItem->m_bGradeEffect = (pItem->m_bGradeEffect+1) % (IE_COUNT-1) + 1;
			else
				pItem->m_bGradeEffect = bEffect;

			pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_SUCCESS_CHANGEEFFECT, bTargetInven, bTargetItemID, pItem->m_bLevel, pItem->m_bGradeEffect);
		}
		break;
	case IK_COLOR:
		{
			pItem->m_dwExtValue[IEV_COLOR] = wColor;
			pPlayer->SendCS_ITEMUPGRADE_ACK(ITEMUPGRADE_SUCCESS_COLOR, bTargetInven, bTargetItemID, pItem->m_bLevel, pItem->m_bGradeEffect, wColor);
		}
		break;
	default:
		return EC_NOERROR;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CHGCORPSCOMMANDER_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wPartyID;
	pBUF->m_packet
		>> wPartyID;

	SendMW_CHGCORPSCOMMANDER_ACK( pPlayer->m_dwID, pPlayer->m_dwKEY, wPartyID);
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CORPSLEAVE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wSquadID;
	pBUF->m_packet
		>> wSquadID;

	SendMW_CORPSLEAVE_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, wSquadID);
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CORPSASK_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	CString strName;
	pBUF->m_packet
		>> strName;

	SendMW_CORPSASK_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, strName);
	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_CORPSREPLY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	CString strReqName;
	BYTE bReply;

	pBUF->m_packet
		>> strReqName
		>> bReply;

	SendMW_CORPSREPLY_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		bReply,
		strReqName);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CORPSCMD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wSquadID;
	DWORD dwCharID;
	WORD wMapID;
	BYTE bCMD;
	DWORD dwTargetID;
	BYTE bTargetType;
	WORD wPosX;
	WORD wPosZ;

	pBUF->m_packet
		>> wSquadID
		>> dwCharID
		>> wMapID
		>> bCMD
		>> dwTargetID
		>> bTargetType
		>> wPosX
		>> wPosZ;

	SendMW_CORPSCMD_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		wMapID,
		wSquadID,
		dwCharID,
		bCMD,
		dwTargetID,
		bTargetType,
		wPosX,
		wPosZ);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CORPSENEMYLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wSquadID;
	DWORD dwTargetID;
	BYTE bCount;
	DWORD dwEnemyID;
	BYTE bEnemyType;
	FLOAT fSPEED;
	WORD wMapID;
	WORD wPosX;
	WORD wPosZ;
	WORD wDIR;
	BYTE bReporterCount;
	DWORD dwReporterID;

	CPacket * pPacket = new CPacket();
	pPacket->SetID(MW_CORPSENEMYLIST_ACK)
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY;

	pBUF->m_packet
		>> wSquadID
		>> dwTargetID
		>> bCount;

	(*pPacket)
		<< wSquadID
		<< dwTargetID
		<< bCount;

	for(BYTE i=0; i<bCount; i++)
	{
		pBUF->m_packet
			>> dwEnemyID
			>> bEnemyType
			>> fSPEED
			>> wMapID
			>> wPosX
			>> wPosZ
			>> wDIR
			>> bReporterCount;

		(*pPacket)
			<< dwEnemyID
			<< bEnemyType
			<< fSPEED
			<< wMapID
			<< wPosX
			<< wPosZ
			<< wDIR
			<< bReporterCount;

		for(BYTE j=0; j<bReporterCount; j++)
		{
			pBUF->m_packet
				>> dwReporterID;

			(*pPacket)
				<< dwReporterID;
		}
	}

	m_world.Say(pPacket);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_MOVECORPSENEMY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwReporterID;
	DWORD dwEnemyID;
	BYTE bEnemyType;
	FLOAT fSPEED;
	WORD wMapID;
	WORD wPosX;
	WORD wPosZ;
	WORD wDIR;

	pBUF->m_packet
		>> dwReporterID
		>> dwEnemyID
		>> bEnemyType
		>> fSPEED
		>> wMapID
		>> wPosX
		>> wPosZ
		>> wDIR;

	SendMW_MOVECORPSENEMY_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwReporterID,
		dwEnemyID,
		bEnemyType,
		fSPEED,
		wMapID,
		wPosX,
		wPosZ,
		wDIR);

	return EC_NOERROR;
}


DWORD CTMapSvrModule::OnCS_MOVECORPSUNIT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wSquadID;
	DWORD dwCharID;
	FLOAT fSPEED;
	WORD wMapID;
	DWORD dwBSPID;
	WORD wPosX;
	WORD wPosZ;
	WORD wDIR;

	pBUF->m_packet
		>> wSquadID
		>> dwCharID
		>> fSPEED
		>> wMapID
		>> dwBSPID
		>> wPosX
		>> wPosZ
		>> wDIR;

	SendMW_MOVECORPSUNIT_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		wSquadID,
		dwCharID,
		fSPEED,
		wMapID,
		dwBSPID,
		wPosX,
		wPosZ,
		wDIR);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_ADDCORPSENEMY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwReporterID;
	DWORD dwEnemyID;
	BYTE bEnemyType;
	FLOAT fSPEED;
	WORD wMapID;
	WORD wPosX;
	WORD wPosZ;
	WORD wDIR;

	pBUF->m_packet
		>> dwReporterID
		>> dwEnemyID
		>> bEnemyType
		>> fSPEED
		>> wMapID
		>> wPosX
		>> wPosZ
		>> wDIR;

	SendMW_ADDCORPSENEMY_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwReporterID,
		dwEnemyID,
		bEnemyType,
		fSPEED,
		wMapID,
		wPosX,
		wPosZ,
		wDIR);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_DELCORPSENEMY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwReporterID;
	DWORD dwEnemyID;
	BYTE bEnemyType;

	pBUF->m_packet
		>> dwReporterID
		>> dwEnemyID
		>> bEnemyType;

	SendMW_DELCORPSENEMY_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwReporterID,
		dwEnemyID,
		bEnemyType);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CORPSHP_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wSquadID;
	DWORD dwCharID;
	DWORD dwMaxHP;
	DWORD dwHP;

	pBUF->m_packet
		>> wSquadID
		>> dwCharID
		>> dwMaxHP
		>> dwHP;

	SendMW_CORPSHP_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		wSquadID,
		dwCharID,
		dwMaxHP,
		dwHP);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_PARTYMOVE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	CString strTargetName;
	CString strDestName;
	WORD wDestParty;

	pBUF->m_packet
		>> strTargetName
		>> strDestName
		>> wDestParty;

	if(pPlayer->GetPartyChiefID() != pPlayer->m_dwID ||
		pPlayer->GetPartyID() != pPlayer->GetCommanderID())
		return EC_NOERROR;

	SendMW_PARTYMOVE_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		strTargetName,
		strDestName,
		wDestParty);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_FRIENDGROUPMAKE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bGroup;
	CString strName;
	pBUF->m_packet
		>> bGroup
		>> strName;

	if(strName.GetLength() > MAX_NAME)
	{
		pPlayer->SendCS_FRIENDGROUPMAKE_ACK(
			FRIEND_NOTFOUND,
			bGroup,
			strName);

		return EC_NOERROR;
	}

	SendMW_FRIENDGROUPMAKE_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		bGroup,
		strName);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_FRIENDGROUPDELETE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bID;
	pBUF->m_packet
		>> bID;

	SendMW_FRIENDGROUPDELETE_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		bID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_FRIENDGROUPCHANGE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bGroup;
	DWORD dwFriend;
	pBUF->m_packet
		>> dwFriend
		>> bGroup;

	SendMW_FRIENDGROUPCHANGE_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwFriend,
		bGroup);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_FRIENDGROUPNAME_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bGroup;
	CString strName;

	pBUF->m_packet
		>> bGroup
		>> strName;

	if(strName.GetLength() > MAX_NAME)
	{
		pPlayer->SendCS_FRIENDGROUPNAME_ACK(
			FRIEND_NOTFOUND,
			bGroup,
			strName);
	}

	SendMW_FRIENDGROUPNAME_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		bGroup,
		strName);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TMSSEND_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwTMS;
	CString strMessage;

	pBUF->m_packet
		>> dwTMS
		>> strMessage;

	SendMW_TMSSEND_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwTMS,
		strMessage);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TMSINVITE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwTMS;
	BYTE bCount;
	DWORD dwTarget;

	pBUF->m_packet
		>> dwTMS
		>> bCount;

	CPacket * pPacket = new CPacket();
	pPacket->SetID(MW_TMSINVITE_ACK)
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY
		<< dwTMS
		<< bCount;

	for(BYTE i=0; i<bCount; i++)
	{
		pBUF->m_packet	
			>> dwTarget;

		(*pPacket)
			<< dwTarget;
	}

	m_world.Say(pPacket);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TMSOUT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwTMS;

	pBUF->m_packet
		>> dwTMS;

	SendMW_TMSOUT_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY,
		dwTMS);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_POSTSEND_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(pPlayer->m_dealItem.m_bStatus != DEAL_READY)
	{
		pPlayer->SendCS_POSTSEND_ACK(POST_NOTDEAL);
		return EC_NOERROR;
	}

	if(pPlayer->m_bStore)
	{
		pPlayer->SendCS_POSTSEND_ACK(POST_NOTDEAL);
		return EC_NOERROR;
	}

	CString strTarget;
	CString strTitle;
	CString strMessage;
	BYTE bType;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;
	BYTE bInven;
	BYTE bSlot;

	pBUF->m_packet
		>> strTarget
		>> strTitle
		>> strMessage
		>> bType
		>> dwGold
		>> dwSilver
		>> dwCooper
		>> bInven
		>> bSlot;

	if(strTarget.GetLength() > MAX_NAME ||
		strTitle.GetLength() > MAX_BOARD_TITLE ||
		strMessage.GetLength() > MAX_BOARD_TEXT)
	{
		pPlayer->SendCS_POSTSEND_ACK(POST_MAXLENGTH);
		return EC_NOERROR;
	}

	if(pPlayer->m_strNAME == strTarget)
	{
		pPlayer->SendCS_POSTSEND_ACK(POST_SAMEACCOUNT);
		return EC_NOERROR;
	}

	if(strTitle.IsEmpty())
	{
		pPlayer->SendCS_POSTSEND_ACK(POST_NOTITLE);
		return EC_NOERROR;
	}

	SendDM_POSTCANSEND_REQ(
		pPlayer->m_dwID,
		pPlayer->m_strNAME,
		strTarget,
		strTitle,
		strMessage,
		bType,
		dwGold,
		dwSilver,
		dwCooper,
		bInven,
		bSlot);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_POSTVIEW_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	DWORD dwPostID;
	pBUF->m_packet
		>> dwPostID;

	if(pPlayer->m_dwPostID)
	{
		if(pPlayer->m_pPost && pPlayer->m_pPost->m_dwPostID == dwPostID )
			pPlayer->SendCS_POSTVIEW_ACK(dwPostID);

		return EC_NOERROR;
	}

	pPlayer->m_dwPostID = dwPostID;

	pPlayer->ClearPost();

	SendDM_POSTVIEW_REQ(pPlayer->m_dwID, pPlayer->m_dwKEY, dwPostID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_POSTDEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	DWORD dwPostID;
	pBUF->m_packet
		>> dwPostID;

	pPlayer->m_dwPostID = 0;
	pPlayer->ClearPost();

	SendDM_POSTDEL_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		dwPostID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_POSTGETITEM_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	DWORD dwPostID;
	pBUF->m_packet
		>> dwPostID;

	if(!pPlayer->m_pPost)
	{
		pPlayer->SendCS_POSTGETITEM_ACK(POST_NOTFOUND);
		return EC_NOERROR;
	}

	if(pPlayer->m_pPost->m_dwPostID != dwPostID)
	{
		pPlayer->SendCS_POSTGETITEM_ACK(POST_NOTFOUND);
		return EC_NOERROR;
	}

	LPTPOST pPost = pPlayer->m_pPost;

	BYTE bResult = POST_NOITEM;
	if(!pPost->m_vItem.empty())
	{
		if(pPlayer->CanPush(&pPost->m_vItem, 0))
		{
			VWORD vItemID;
			VBYTE vCount;

			vItemID.clear();
			vCount.clear();

			for(DWORD k=0; k<pPost->m_vItem.size(); k++)
			{
				vItemID.push_back(pPost->m_vItem[k]->m_wItemID);
				vCount.push_back(pPost->m_vItem[k]->m_bCount);
#ifdef DEF_UDPLOG
				m_pUdpSocket->LogItemTrade(LOGMAP_ITEMPOSTGET, pPlayer, pPost->m_vItem[k], pPost->m_strSender);
#endif	//	DEF_UDPLOG		
			}

			pPlayer->PushTItem(&pPost->m_vItem);
			bResult = POST_SUCCESS;

			for(DWORD i=0; i<vItemID.size(); i++)
				CheckQuest(
					pPlayer,
					0,
					pPlayer->m_fPosX,
					pPlayer->m_fPosY,
					pPlayer->m_fPosZ,
					vItemID[i],
					QTT_GETITEM,
					TT_GETITEM,
					vCount[i]);

			vItemID.clear();
			vCount.clear();
			pPost->m_vItem.clear();
		}
		else
		{
			pPlayer->SendCS_POSTGETITEM_ACK(POST_INVENFULL);
			return EC_NOERROR;
		}
	}

	SendDM_POSTGETITEM_REQ(
		pPlayer->m_dwID,
		dwPostID);

	SendDM_SAVEITEM_REQ(pPlayer);

	__int64 llMoney = CalcMoney(pPost->m_dwGold, pPost->m_dwSilver, pPost->m_dwCooper); 
	if(llMoney)
	{
		pPlayer->EarnMoney(llMoney);
#ifdef	DEF_UDPLOG
		m_pUdpSocket->LogMoneyTrade(LOGMAP_MONEYPOSTGET, pPlayer, pPost->m_strSender, llMoney);
#endif	
		pPost->m_dwGold = 0;
		pPost->m_dwSilver = 0;
		pPost->m_dwCooper = 0;
		pPlayer->SendCS_MONEY_ACK();
		bResult = POST_SUCCESS;
	}

	pPlayer->SendCS_POSTGETITEM_ACK(bResult);
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_POSTRETURN_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	DWORD dwPostID;
	BYTE bType;

	pBUF->m_packet
		>> dwPostID
		>> bType;

	if(!pPlayer->m_pPost ||
		pPlayer->m_pPost->m_dwPostID != dwPostID ||
		pPlayer->m_pPost->m_bRead)
		return EC_NOERROR;

	if(bType)
	{
		INT64 dlMoney = CalcMoney(pPlayer->m_pPost->m_dwGold, pPlayer->m_pPost->m_dwSilver, pPlayer->m_pPost->m_dwCooper);

		if(dlMoney)
		{
			if(!pPlayer->UseMoney(dlMoney, TRUE))
			{	
				pPlayer->SendCS_POSTRETURN_ACK(POST_NEEDMONEY);
				return EC_NOERROR;
			}

			pPlayer->SendCS_MONEY_ACK();
		}
	}

	SendDM_POSTBILLUPDATE_REQ(
		pPlayer->m_dwID,
		pPlayer->m_strNAME,
		dwPostID,
		(bType ? POST_PAYMENT : POST_RETURN),
		pPlayer->m_pPost->m_dwGold,
		pPlayer->m_pPost->m_dwSilver,
		pPlayer->m_pPost->m_dwCooper);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CASTLEAPPLY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wCastle;
	DWORD dwTarget;

	pBUF->m_packet
		>> wCastle
		>> dwTarget;

	if(pPlayer->m_bGuildDuty != GUILD_DUTY_CHIEF)
		return EC_NOERROR;

	if(!wCastle)
	{
		SendMW_CASTLEAPPLY_ACK(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			wCastle,
			dwTarget,
			CAMP_NONE);

		return EC_NOERROR;
	}

	LPTLOCAL pCastle = FindCastle(wCastle);
	if(!pCastle)
	{
		pPlayer->SendCS_CASTLEAPPLY_ACK(CBS_NOTFOUND);
		return EC_NOERROR;
	}

	if(	pCastle->m_dwDefGuildID != pPlayer->GetGuild() &&
		pCastle->m_dwAtkGuildID != pPlayer->GetGuild())
	{
		pPlayer->SendCS_CASTLEAPPLY_ACK(CBS_CANTAPPLY);
		return EC_NOERROR;
	}

	BYTE bError = CanApplyCastle(pCastle);
	if(bError)
	{
		pPlayer->SendCS_CASTLEAPPLY_ACK(bError);
		return EC_NOERROR;
	}

	BYTE bCamp = CAMP_NONE;

	if(pCastle->m_dwDefGuildID == pPlayer->GetGuild())
		bCamp = CAMP_DEFEND;
	else
		bCamp = CAMP_ATTACK;

	SendMW_CASTLEAPPLY_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		wCastle,
		dwTarget,
		bCamp);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_ITEMUSE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wTempItem;
	BYTE bInven;
	BYTE bItem;
	WORD wDelayGroupID;
	BYTE bKind = 0;
	BYTE bCount;

	pBUF->m_packet
		>> wTempItem
		>> bInven
		>> bItem
		>> wDelayGroupID
		>> bCount;

	if(pPlayer->m_dwRiding)
	{
		pPlayer->SendCS_ITEMUSE_ACK(IU_RIDING,wDelayGroupID,bKind,0);
		return EC_NOERROR;
	}

	if(pPlayer->m_dealItem.m_bStatus >= DEAL_START)
	{
		pPlayer->SendCS_ITEMUSE_ACK(IU_DEALING,wDelayGroupID,bKind,0);
		return EC_NOERROR;
	}

	if(pPlayer->m_bStore)
	{
		pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
		return EC_NOERROR;
	}

	if(pPlayer->m_wArenaID)
	{
		pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
		return EC_NOERROR;
	}

	DWORD dwTarget;
	BYTE bType;

	MAPTITEMTEMP::iterator itTempItem = m_mapTITEM.find(wTempItem);
	if(itTempItem == m_mapTITEM.end())
	{
		pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
		return EC_NOERROR;
	}

	LPTITEM pTempItem = (*itTempItem).second;

	for(BYTE i=0; i<bCount; i++)
	{
		pBUF->m_packet
			>> dwTarget
			>> bType;
	}

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
	{
		pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bItem);
	if(!pItem || !pItem->m_bCount)
	{
		pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
		return EC_NOERROR;
	}

	if(!pItem->CanUse())
	{
		pPlayer->SendCS_ITEMUSE_ACK(IU_WRAPPING,wDelayGroupID,bKind,0);
		return EC_NOERROR;
	}

	bKind = pItem->m_pTITEM->m_bKind;
	if(pItem->m_pTITEM->m_wDelayGroupID != wDelayGroupID)
	{
		pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
		return EC_NOERROR;
	}

	if(	(pPlayer->m_bStatus != OS_DEAD && pItem->m_pTITEM->m_bKind == IK_REVIVAL) ||
		(pPlayer->m_bStatus == OS_DEAD && pItem->m_pTITEM->m_bKind != IK_REVIVAL))
	{
		pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
		return EC_NOERROR;
	}

	if(pItem->m_pTITEM->m_bDefaultLevel > pPlayer->m_bLevel)
	{
		pPlayer->SendCS_ITEMUSE_ACK(IU_NEEDLEVEL,wDelayGroupID,bKind,0);
		return EC_NOERROR;
	}

	if(IsTournamentMap(pPlayer->m_wMapID) || GetLoungeMapID() == pPlayer->m_wMapID)
	{
		if(!FindTournamentPlayer(pPlayer->m_dwID, TRUE) ||
			pItem->m_pTITEM->m_bKind == TOURNAMENT_CANTUSEITEM_HP ||
            pItem->m_pTITEM->m_bKind == TOURNAMENT_CANTUSEITEM_MP ||
			pItem->m_pTITEM->m_wItemID == TOURNAMENT_CANTUSEITEM_ID ||
			pItem->m_pTITEM->m_wItemID == TOURNAMENT_CANTUSEITEM_ID2 ||
			(pItem->m_pTITEM->m_bType == IT_USE &&
			pItem->m_pTITEM->m_bKind == IK_SKILL &&
			pItem->m_pTITEM->m_wUseValue == TOURNAMENT_CANTUSEITEM_SKILLVALUE))
		{
			pPlayer->SendCS_ITEMUSE_ACK(IU_NEEDTIME,wDelayGroupID,bKind,0);
			return EC_NOERROR;
		}
	}

	DWORD dwDelay = pItem->m_pTITEM->m_dwDelay;
	MAPDWORD::iterator it;
	if(dwDelay)
	{
		it = pPlayer->m_mapItemCoolTime.find(pItem->m_pTITEM->m_wDelayGroupID);
		if(it!=pPlayer->m_mapItemCoolTime.end())
		{
			DWORD dwTick = (*it).second;
			if( dwTick > m_dwTick)
			{
				pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
				return EC_NOERROR;
			}
		}
	}

	BYTE bUsed = FALSE;
	DWORD dwMaxHP;
	DWORD dwMaxMP;
	WORD wBonus = 0;

	if(pItem->m_pTITEM->m_bType == IT_USE)
	{
		switch(pItem->m_pTITEM->m_bKind)
		{
		case IK_GAINEXP:
			pPlayer->GainExp(pItem->m_pTITEM->m_dwSpeedInc, FALSE);
			bUsed = TRUE;
			break;
		case IK_MAXHP:
		case IK_HP:
			dwMaxHP = pPlayer->GetMaxHP();
			dwMaxMP = pPlayer->GetMaxMP();

			if(pPlayer->m_dwHP < dwMaxHP)
			{
				if(pItem->m_pTITEM->m_bKind == IK_HP)
					pPlayer->m_dwHP += pItem->m_pTITEM->m_wUseValue;
				else
					pPlayer->m_dwHP = dwMaxHP;

				if(dwMaxHP < pPlayer->m_dwHP)
					pPlayer->m_dwHP = dwMaxHP;
				
				VPLAYER vPLAYERS;
				vPLAYERS.clear();

				pPlayer->m_pMAP->GetNeighbor(
					&vPLAYERS,
					pPlayer->m_fPosX,
					pPlayer->m_fPosZ);

				while(!vPLAYERS.empty())
				{
					CTPlayer *pChar = vPLAYERS.back();

					pChar->SendCS_HPMP_ACK(
						pPlayer->m_dwID,
						pPlayer->m_bType,
						pPlayer->m_bLevel,
						dwMaxHP,
						pPlayer->m_dwHP,
						dwMaxMP,
						pPlayer->m_dwMP);

					vPLAYERS.pop_back();
				}
				vPLAYERS.clear();
				bUsed = TRUE;
			}
			break;
		case IK_MAXMP:
		case IK_MP:
			dwMaxMP = pPlayer->GetMaxMP();
			dwMaxHP = pPlayer->GetMaxHP();

			if(pPlayer->m_dwMP < dwMaxMP)
			{
				if(pItem->m_pTITEM->m_bKind == IK_MP)
					pPlayer->m_dwMP += pItem->m_pTITEM->m_wUseValue;
				else
					pPlayer->m_dwMP = dwMaxMP;

				if(dwMaxMP < pPlayer->m_dwMP)
					pPlayer->m_dwMP = dwMaxMP;
				
				VPLAYER vPLAYERS;
				vPLAYERS.clear();

				pPlayer->m_pMAP->GetNeighbor(
					&vPLAYERS,
					pPlayer->m_fPosX,
					pPlayer->m_fPosZ);

				while(!vPLAYERS.empty())
				{
					CTPlayer *pChar = vPLAYERS.back();

					pChar->SendCS_HPMP_ACK(
						pPlayer->m_dwID,
						pPlayer->m_bType,
						pPlayer->m_bLevel,
						dwMaxHP,
						pPlayer->m_dwHP,
						dwMaxMP,
						pPlayer->m_dwMP);

					vPLAYERS.pop_back();
				}
				vPLAYERS.clear();
				bUsed = TRUE;
			}
			break;
		case IK_AP:
			bUsed = TRUE;
			break;
		case IK_GOLDPREMIUM:
		case IK_GOLDPREMIUM2:
			if(pPlayer->m_diPremium.m_bType)
			{
				pPlayer->SendCS_ITEMUSE_ACK(IU_OVERLAPPREMIUM,wDelayGroupID,bKind,0);
				return EC_NOERROR;
			}
			else
				bUsed = TRUE;
			break;
		case IK_EXPBONUS:
			if(pPlayer->IsExpBenefit(wBonus))
			{
				pPlayer->SendCS_ITEMUSE_ACK(IU_OVERLAPEXPBONUS,wDelayGroupID,bKind,0);
				return EC_NOERROR;
			}
			else
				bUsed = TRUE;
			break;
		case IK_REVIVAL:
		case IK_SKILL:
			{
				CTSkillTemp * pTemp = FindTSkill(pItem->m_pTITEM->m_wUseValue);

				// 이벤트 변신물약
				if( pTemp )
				{
					if(pPlayer->HaveDisguiseBuff() && pTemp->IsTrans())
					{
						pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
						return EC_NOERROR;
					}
					if(!pTemp->CheckCountry(pPlayer->m_bCountry))
					{
						pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
						return EC_NOERROR;
					}

					if(!pPlayer->CheckItemBuff(pTemp))
					{
						pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
						return EC_NOERROR;
					}

					if(pTemp->IsRandomTrans())
					{
						pTemp = RandTransSkill(pTemp);
						if(!pTemp)
						{
							pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
							return EC_NOERROR;
						}
					}

					if(pTemp->IsRandomBuff())  
					{
						pTemp = RandBuffSkill(pTemp);
						if(!pTemp)
						{
							pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
							return EC_NOERROR;
						}
					}

					if(pTemp->IsAfterMath() && !pPlayer->m_aftermath.m_bStep)
					{
						pPlayer->SendCS_ITEMUSE_ACK(IU_NOTFOUND,wDelayGroupID,bKind,0);
						return EC_NOERROR;
					}

					DWORD dwAdd = 0;
					if(pItem->m_pTITEM->m_bUseType & DURINGTYPE_TIME)
						dwAdd = DWORD(pItem->m_pTITEM->m_wUseTime) * HOUR_ONE;
					else if(pItem->m_pTITEM->m_bUseType & DURINGTYPE_DAY)
						dwAdd = DWORD(pItem->m_pTITEM->m_wUseTime) * DAY_ONE;

					pPlayer->Defend(
						pTemp,
						1,
						pPlayer->m_dwID,
						pPlayer->m_dwID,
						OT_PC,
						pPlayer->GetPartyID(),
						pPlayer->m_bCountry,
						pPlayer->m_bAidCountry,
						pPlayer->m_bClass,
						0,0,0,0,0,0,0,0,0,0,
						TRUE,
						pPlayer->GetAttackLevel(),
						pPlayer->m_bLevel,
						0,
						HT_NORMAL,
						dwAdd * 1000,
						pPlayer->m_fPosX,
						pPlayer->m_fPosY,
						pPlayer->m_fPosZ,
						pPlayer->m_fPosX,
						pPlayer->m_fPosY,
						pPlayer->m_fPosZ,
						FALSE);

					bUsed = TRUE;
				}
			}
			break;
		case IK_RETURN:
			Teleport(pPlayer, pItem->m_pTITEM->m_wUseValue);
			bUsed = TRUE;
			break;
		case IK_FACE:
		case IK_HAIR:
		case IK_RACE:
		case IK_SEX:
			{
				if(pPlayer->ChangeCharBase(pItem->m_pTITEM->m_bKind))
					bUsed = TRUE;

#ifdef DEF_UDPLOG
				if(bUsed)
				{
					m_pUdpSocket->LogEnterMap(LOGMAP_CASHCHANGECHAR, pPlayer);
				}
#endif
			}
			break;
		case IK_MONEY:
			{
				DWORD dwMoney = pItem->RandMoney();
				pPlayer->EarnMoney(dwMoney);
				pPlayer->SendCS_MONEY_ACK();
				pPlayer->SendCS_OPENMONEY_ACK(dwMoney);
				bUsed = TRUE;

#ifdef DEF_UDPLOG
				m_pUdpSocket->LogItemByNPC(LOGMAP_CASHRANDMONEY, pPlayer, NULL, NULL, dwMoney);	
#endif
			}
			break;
		case IK_CASH:
			{
				bUsed = TRUE;
				SendDM_SAVEITEM_REQ(pPlayer);
				SendDM_GAINCASHBONUS_REQ(
					pPlayer->m_dwID,
					pPlayer->m_dwKEY,
					pPlayer->m_strNAME,
					bInven,
					bItem,
					pItem->m_pTITEM->m_wItemID,
					pItem->m_pTITEM->m_wUseValue);
			}
			break;
		default:
			bUsed = TRUE;
			break;
		}

		if(bUsed)
		{
			if(dwDelay)
			{
				if(it!=pPlayer->m_mapItemCoolTime.end())
					(*it).second = m_dwTick + dwDelay;
				else
					pPlayer->m_mapItemCoolTime.insert(MAPDWORD::value_type(pItem->m_pTITEM->m_wDelayGroupID, m_dwTick+dwDelay));
			}
			if(pItem->m_pTITEM->m_bUseType & DURINGTYPE_USE)
			{
				DWORD dwAdd = 0;
				BYTE bUseType = DURINGTYPE_TIME;
				if(pItem->m_pTITEM->m_bUseType & DURINGTYPE_TIME)
					dwAdd = DWORD(pItem->m_pTITEM->m_wUseTime) * HOUR_ONE;
				else if(pItem->m_pTITEM->m_bUseType & DURINGTYPE_DAY)
				{
					bUseType = DURINGTYPE_DAY;
					dwAdd = DWORD(pItem->m_pTITEM->m_wUseTime) * DAY_ONE;
				}

				pPlayer->SetDuringItem(pItem->m_pTITEM->m_bKind, pItem->m_pTITEM, bUseType, dwAdd);

				BYTE bPcbangType = PCBANG_NONE;
				WORD wPetID = 0;
				CString strPetName = NAME_NULL;
				if(pItem->m_pTITEM->m_bKind == IK_GOLDPREMIUM)
				{
					bPcbangType = PCBANG_PREMIUM1;
					wPetID = PREMIUM1_PET;
					strPetName = GetSvrMsg(PREMIUM_PETNAME);
				}
				else if(pItem->m_pTITEM->m_bKind == IK_GOLDPREMIUM2)
				{
					bPcbangType = PCBANG_PREMIUM2;
					wPetID = PREMIUM2_PET;
					strPetName = GetSvrMsg(PREMIUM_PETNAME);
				}

				if(bPcbangType != PCBANG_NONE)
				{
					pPlayer->ResetPcBangData(bPcbangType, dwAdd);
					pPlayer->HangPremiumItem();

					if(GetNation() == NATION_KOREA)
					{
						LPTPET pPET = pPlayer->PetMake(wPetID, strPetName, dwAdd);
						if(pPET)
							pPlayer->SendCS_PETMAKE_ACK(PET_SUCCESS, pPET->m_wPetID, pPET->m_strName, pPET->m_ldwTime);
					}
				}
				else
					pPlayer->HangExpBuff();
			}

			if(pItem->m_pTITEM->m_bConsumable)
				UseItem(pPlayer, pInven, pItem, 1);

			pPlayer->SendCS_ITEMUSE_ACK(
				IU_SUCCESS,
				wDelayGroupID,
				bKind,
				dwDelay);

			CheckQuest(
				pPlayer,
				0,
				pPlayer->m_fPosX,
				pPlayer->m_fPosY,
				pPlayer->m_fPosZ,
				wTempItem,
				QTT_USEITEM,
				TT_USEITEM,
				1);
		}
		else
		{
			pPlayer->SendCS_ITEMUSE_ACK(
				IU_FULL,
				wDelayGroupID,
				bKind,
				dwDelay);
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CHGMODERECALLMON_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if(!pPlayer->m_bMain || !pPlayer->m_pMAP)
		return EC_NOERROR;

	DWORD dwMonID;
	BYTE bMode;

	pBUF->m_packet
		>> dwMonID
		>> bMode;

	CTRecallMon * pMon = pPlayer->FindRecallMon(dwMonID);
	if(!pMon)
		return EC_NOERROR;

	pMon->ChgMode(bMode);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_DELRECALLMON_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if(!pPlayer->m_bMain || !pPlayer->m_pMAP)
		return EC_NOERROR;

	DWORD dwMonID;
	BYTE bType;

	pBUF->m_packet
		>> dwMonID
		>> bType;

	if(bType == OT_RECALL)
		SendMW_RECALLMONDEL_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, dwMonID);
	else if(bType == OT_SELF)
	{
		CTSelfObj * pMon = pPlayer->FindSelfObj(dwMonID);
		if(pMon)
		{
			pMon->OnDie(0,OT_NONE,0); 
			pPlayer->DeleteSelfMon(dwMonID);
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_INVENADD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if(!pPlayer->m_bMain || !pPlayer->m_pMAP)
		return EC_NOERROR;

	BYTE bDESInven;
	BYTE bSRCInven;
	BYTE bItemID;

	pBUF->m_packet
		>> bDESInven
		>> bSRCInven
		>> bItemID;

	if(pPlayer->m_bStore)
		return EC_NOERROR;

	CTInven * pNew = pPlayer->FindTInven(bDESInven);
	if(pNew)
	{
		pPlayer->SendCS_INVENADD_ACK( INVEN_EXIST, 0, 0, 0);
		return EC_NOERROR;
	}

	CTInven * pInven = pPlayer->FindTInven(bSRCInven);
	if(!pInven)
	{
		pPlayer->SendCS_INVENADD_ACK( INVEN_FAIL, 0, 0, 0);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bItemID);
	if(!pItem || !pItem->m_bCount)
	{
		pPlayer->SendCS_INVENADD_ACK( INVEN_FAIL, 0, 0, 0);
		return EC_NOERROR;
	}

	if(pItem->m_pTITEM->m_bType != IT_INVEN)
	{
		pPlayer->SendCS_INVENADD_ACK( INVEN_FAIL, 0, 0, 0);
		return EC_NOERROR;
	}

	if(pItem->GetEquipLevel() > pPlayer->m_bLevel)
	{
		pPlayer->SendCS_INVENADD_ACK( INVEN_LEVEL, 0, 0, 0);
		return EC_NOERROR;
	}

	pNew = new CTInven();
	pNew->m_bInvenID = bDESInven;
	pNew->m_wItemID = pItem->m_pTITEM->m_wItemID;
	pNew->m_pTITEM = pItem->m_pTITEM;
	pNew->m_dEndTime = pItem->m_dEndTime;
	pNew->m_bELD = BYTE(pItem->m_dwExtValue[IEV_ELD]);
	pPlayer->SendCS_DELITEM_ACK(bSRCInven,	bItemID);
	pInven->m_mapTITEM.erase(bItemID);
	delete pItem;

	pPlayer->m_mapTINVEN.insert(MAPTINVEN::value_type(bDESInven, pNew));
	pPlayer->SendCS_INVENADD_ACK(
		INVEN_SUCCESS,
		pNew->m_bInvenID,
		pNew->m_wItemID,
		pNew->m_dEndTime);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_INVENDEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if(!pPlayer->m_bMain || !pPlayer->m_pMAP)
		return EC_NOERROR;

	BYTE bSRCInven;
	BYTE bDESInven;
	BYTE bPos;

	pBUF->m_packet
		>> bSRCInven
		>> bDESInven
		>> bPos;

	CTInven * pSrc = pPlayer->FindTInven(bSRCInven);
	CTInven * pDes = pPlayer->FindTInven(bDESInven);
	if(!pSrc || !pDes || pSrc == pDes)
	{
		pPlayer->SendCS_INVENDEL_ACK( INVEN_FAIL, 0);
		return EC_NOERROR;
	}

	if(!pSrc->m_mapTITEM.empty())
	{
		pPlayer->SendCS_INVENDEL_ACK( INVEN_NOTEMPTY, 0);
		return EC_NOERROR;
	}

	if( bPos == INVALID_SLOT || pDes->FindTItem(bPos) )
		bPos = pDes->GetBlankPos();

	if(bPos == INVALID_SLOT)
	{
		pPlayer->SendCS_INVENDEL_ACK( INVEN_FULL, 0);
		return EC_NOERROR;
	}

	CTItem * pItem = new CTItem();
	pItem->m_dlID = GenItemID();
	pItem->m_bItemID = bPos;
	pItem->m_wItemID = pSrc->m_wItemID;
	pItem->m_pTITEM = pSrc->m_pTITEM;
	pItem->m_bCount = 1;
	pItem->m_dEndTime = pSrc->m_dEndTime;
	pItem->m_dwExtValue[IEV_ELD] = pSrc->m_bELD;
	SetItemAttr(pItem, 0);

	pDes->m_mapTITEM.insert(MAPTITEM::value_type(bPos, pItem));
	pPlayer->SendCS_ADDITEM_ACK(pItem, bDESInven);

	pPlayer->m_mapTINVEN.erase(bSRCInven);
	delete pSrc;

	pPlayer->SendCS_INVENDEL_ACK( INVEN_SUCCESS, bSRCInven);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_INVENMOVE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if(!pPlayer->m_bMain || !pPlayer->m_pMAP)
		return EC_NOERROR;

	BYTE bSRCInven;
	BYTE bDESInven;

	pBUF->m_packet
		>> bSRCInven
		>> bDESInven;

	CTInven * pSrc = pPlayer->FindTInven(bSRCInven);
	CTInven * pDes = pPlayer->FindTInven(bDESInven);

	if(!pSrc)
	{
		pPlayer->SendCS_INVENMOVE_ACK( INVEN_FAIL, 0, 0);
		return EC_NOERROR;
	}

	pPlayer->m_mapTINVEN.erase(bSRCInven);
	pPlayer->m_mapTINVEN.erase(bDESInven);

	if(pDes)
	{
		pDes->m_bInvenID = bSRCInven;
		pPlayer->m_mapTINVEN.insert( MAPTINVEN::value_type( pDes->m_bInvenID, pDes));
	}

	pSrc->m_bInvenID = bDESInven;
	pPlayer->m_mapTINVEN.insert( MAPTINVEN::value_type( pSrc->m_bInvenID, pSrc));

	pPlayer->SendCS_INVENMOVE_ACK( INVEN_SUCCESS, bSRCInven, bDESInven);
	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_RESETHOST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwMonID;
	WORD wMapID;
	BYTE bChannelID;

	pBUF->m_packet
		>> dwMonID
		>> bChannelID
		>> wMapID;
/*
	CTMonster * pMON = pPlayer->m_pMAP->FindMonster(dwMonID);
	if(pMON)
		pMON->OnEvent(AT_ENTER, 0, 0, 0, 0);
*/
	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_REGION_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	CTObjBase *pOBJ = NULL;

	DWORD dwRegionID;
	DWORD dwID;

	BYTE bChannel;
	BYTE bType;
	WORD wMapID;
	WORD wLocalID;

	pBUF->m_packet
		>> dwID
		>> bType
		>> bChannel
		>> wMapID
		>> dwRegionID
		>> wLocalID;

	switch(bType)
	{
	case OT_PC		:
		if(pPlayer->m_bMain)
			pOBJ = pPlayer;
		break;

	case OT_RECALL	:
		pOBJ = pPlayer->FindRecallMon(dwID);
		break;

	case OT_MON		:
		if(pPlayer->m_pMAP)
			pOBJ = pPlayer->m_pMAP->FindMonster(dwID);
		break;

	case OT_SELF:
		pOBJ = pPlayer->FindSelfObj(dwID);
		break;
	}

	if(pOBJ)
	{
		pOBJ->m_dwRegion = dwRegionID;

		LPTLOCAL pLocal = NULL;

		if(pOBJ->m_wLocalID != wLocalID)
		{
			if(wLocalID)
			{
				pLocal = FindLocal(wLocalID);
				if(pLocal && !pLocal->m_bValid)
					pLocal = NULL;
			}

			if(bType == OT_PC)
			{
				if(pOBJ->m_wLocalID)
				{
					LPTLOCAL pCurLocal = FindLocal(pOBJ->m_wLocalID);
					if(pCurLocal)
					{
						pOBJ->EraseMaintainSkill(pCurLocal->m_pZone->m_wSkill1);
						pOBJ->EraseMaintainSkill(pCurLocal->m_pZone->m_wSkill2);
						if(EquipItemRevision((CTPlayer *)pOBJ, FALSE, pCurLocal->m_pZone->m_bItemLevel))
							((CTPlayer *)pOBJ)->SendCS_ITEMLEVELREVISION_ACK(0);
					}
				}

				if(pLocal)
				{
					if(pLocal->m_bCountry == pOBJ->GetWarCountry())
					{
						pOBJ->ForceMaintain(pLocal->m_pZone->m_wSkill1, pOBJ->m_dwID, OT_PC, pOBJ->m_dwID, OT_PC, 0);
						pOBJ->ForceMaintain(pLocal->m_pZone->m_wSkill2, pOBJ->m_dwID, OT_PC, pOBJ->m_dwID, OT_PC, 0);
					}
					else
					{
						pOBJ->EraseMaintainSkill(pLocal->m_pZone->m_wSkill1);
						pOBJ->EraseMaintainSkill(pLocal->m_pZone->m_wSkill2);
					}

					if(EquipItemRevision((CTPlayer *)pOBJ, TRUE, pLocal->m_pZone->m_bItemLevel))
						((CTPlayer *)pOBJ)->SendCS_ITEMLEVELREVISION_ACK(pLocal->m_pZone->m_bItemLevel);
				}
			}

			pOBJ->m_pLocal = pLocal;
			pOBJ->m_wLocalID = wLocalID;
		}

		if(bType == OT_PC)
			SendMW_REGION_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, pPlayer->m_dwRegion);
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_SWITCHCHANGE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwSwitchID;
	pBUF->m_packet
		>> dwSwitchID;

	pPlayer->ChangeSwitch(dwSwitchID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_SKILLEND_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer *)pBUF->m_pSESSION;

	DWORD dwObjID;
	BYTE bObjType;

	DWORD dwAttackID;
	BYTE bAttackType;

	DWORD dwHostID;
	WORD wSkillID;
	WORD wMapID;
	BYTE bChannelID;

	pBUF->m_packet
		>> dwObjID
		>> bObjType
		>> dwHostID
		>> dwAttackID
		>> bAttackType
		>> wSkillID
		>> wMapID
		>> bChannelID;

	CTObjBase *pOBJ = NULL;
	switch(bObjType)
	{
	case OT_PC		:
		if(pPlayer->m_bMain)
			pOBJ = pPlayer;

		break;

	case OT_RECALL	:
		{
			MAPPLAYER::iterator it = m_mapPLAYER.find(dwHostID);
			if(it!=m_mapPLAYER.end())
			{
				CTRecallMon *pRecall = (*it).second->FindRecallMon(dwObjID);
				if(pRecall && (*it).second->m_bMain)
					pOBJ = pRecall;
				
			}
		}
		break;

	case OT_MON		:
		if( pPlayer->m_pMAP)
			pOBJ = pPlayer->m_pMAP->FindMonster(dwObjID);
		break;
	case OT_SELF:
		pOBJ = pPlayer->FindSelfObj(dwObjID);
		break;
	}

	if(!pOBJ)
	{
		pPlayer->SendCS_SKILLEND_ACK(dwObjID,bObjType,wSkillID);
		return EC_NOERROR;
	}

	BYTE bErase = FALSE;
	for(DWORD i=0; i<pOBJ->m_vMaintainSkill.size(); i++)
	{
		if(pOBJ->m_vMaintainSkill[i]->m_dwAttackID == dwAttackID &&
			pOBJ->m_vMaintainSkill[i]->m_bAttackType == bAttackType &&
			pOBJ->m_vMaintainSkill[i]->m_pTSKILL->m_wID == wSkillID)
		{
			pOBJ->EraseMaintainSkill(pOBJ->m_vMaintainSkill[i], i);
			bErase = TRUE;
			break;
		}
	}

	if(!pOBJ->m_pMAP)
	{
		pPlayer->SendCS_SKILLEND_ACK(dwObjID,bObjType,wSkillID);
		return EC_NOERROR;
	}

	if(!bErase)
	{
		VPLAYER vPLAYERS;
		vPLAYERS.clear();

		pPlayer->m_pMAP->GetNeighbor(
			&vPLAYERS,
			pOBJ->m_fPosX,
			pOBJ->m_fPosZ);

		for(DWORD nPlayer=0; nPlayer < vPLAYERS.size(); nPlayer++)
		{
			vPLAYERS[nPlayer]->SendCS_SKILLEND_ACK(
				dwObjID,
				bObjType,
				wSkillID);
		}
		vPLAYERS.clear();
	}
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_PROTECTEDADD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	CString strProtected;
	pBUF->m_packet
		>> strProtected;

	if(pPlayer->m_strNAME == strProtected ||
		strProtected.IsEmpty() ||
		strProtected.GetLength() > MAX_NAME ||
		pPlayer->m_mapTPROTECTED.size() > 200)
	{
		pPlayer->SendCS_PROTECTEDADD_ACK(PROTECTED_FAIL, 0, strProtected, 0);
        return EC_NOERROR;
	}

	BYTE bOption = PROTECTED_CHAT;

	LPPACKETBUF pBuf = new PACKETBUF();
	pBuf->m_packet.SetID(DM_PROTECTEDADD_REQ)
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY
		<< strProtected
		<< bOption;

	SayToDB(pBuf);
	
	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_PROTECTEDERASE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	CString strProtected;
	pBUF->m_packet
		>> strProtected;

	if(pPlayer->m_strNAME == strProtected ||
		strProtected.IsEmpty() ||
		strProtected.GetLength() > MAX_NAME)
	{
		pPlayer->SendCS_PROTECTEDERASE_ACK(PROTECTED_FAIL, strProtected);
        return EC_NOERROR;
	}

	MAPTPROTECTED::iterator it;
	for(it=pPlayer->m_mapTPROTECTED.begin();it!=pPlayer->m_mapTPROTECTED.end();it++)
	{
		if((*it).second->m_strName == strProtected)
			break;
	}
	if(it==pPlayer->m_mapTPROTECTED.end())
	{
		pPlayer->SendCS_PROTECTEDERASE_ACK(PROTECTED_NOTFOUND, strProtected);
		return EC_NOERROR;
	}

	DWORD dwProtected = (*it).first;

	pPlayer->m_mapTPROTECTED.erase((*it).first);
	pPlayer->SendCS_PROTECTEDERASE_ACK(PROTECTED_SUCCESS, strProtected);

	// 차단에 관련된 on-line표시
	SendMW_PROTECTEDCHECK_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, FRIEND_CONNECTION, strProtected);

	LPPACKETBUF pBuf = new PACKETBUF();
	pBuf->m_packet.SetID(DM_PROTECTEDERASE_REQ)
		<< pPlayer->m_dwID
		<< dwProtected;
	SayToDB(pBuf);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_REVIVALASK_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bReply;
	DWORD dwAttackerID;
	BYTE bAttackerType;
	WORD wSkill;
	BYTE bLevel;

	pBUF->m_packet
		>> bReply
		>> dwAttackerID
		>> bAttackerType
		>> wSkill
		>> bLevel;

	if(bReply == ASK_YES)
	{
		CTSkillTemp * pTemp = FindTSkill(wSkill);

		if(pTemp)
			pPlayer->Revival(AFTERMATH_HELP, pTemp, bLevel);
	}
	else
	{
		if(bAttackerType == OT_PC)
		{
			MAPPLAYER::iterator it = m_mapPLAYER.find(dwAttackerID);
			if( it != m_mapPLAYER.end())
				(*it).second->SendCS_REVIVALREPLY_ACK(bReply, pPlayer->m_dwID);
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_DROPDAMAGE_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	DWORD dwTargetID;
	BYTE bTargetType;

	DWORD dwDamage;
	BYTE bChannel;
	WORD wMapID;

	pBUF->m_packet
		>> dwTargetID
		>> bTargetType
		>> dwDamage
		>> bChannel
		>> wMapID;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	CTObjBase *pDEFEND = NULL;
	switch(bTargetType)
	{
	case OT_MON	:
		pDEFEND = pPlayer->m_pMAP->FindMonster(dwTargetID);
		break;

	case OT_RECALL:
		{
			CTRecallMon * pRecall = pPlayer->FindRecallMon(dwTargetID);
			if(pRecall && pPlayer->m_bMain)
				pDEFEND = pRecall;
		}
		break;
	case OT_PC	:
		{
			MAPPLAYER::iterator finder = m_mapPLAYER.find(dwTargetID);
			if( finder != m_mapPLAYER.end() &&
				(*finder).second->m_bMain &&
				(*finder).second->m_pMAP)
				pDEFEND = (*finder).second;
		}
		break;
	case OT_SELF:
		pDEFEND = pPlayer->FindSelfObj(dwTargetID);
		break;
	}

	if(!pDEFEND || !pDEFEND->m_pMAP)
		return EC_NOERROR;

	if(pDEFEND->m_dwHP == 0)
		return EC_NOERROR;

	if(pDEFEND->m_dwHP > dwDamage)
		pDEFEND->m_dwHP -= dwDamage;
	else
		pDEFEND->m_dwHP = 0;

	pDEFEND->m_dwRecoverHPTick = m_dwTick + RECOVER_INIT;
	pDEFEND->m_dwRecoverMPTick = m_dwTick + RECOVER_INIT;

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	pPlayer->m_pMAP->GetNeighbor(
		&vPLAYERS,
		pDEFEND->m_fPosX,
		pDEFEND->m_fPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_DROPDAMAGE_ACK(
			dwTargetID,
			bTargetType,
			dwDamage);

		pChar->SendCS_HPMP_ACK(
			dwTargetID,
			bTargetType,
			pDEFEND->m_bLevel,
			pDEFEND->GetMaxHP(),
			pDEFEND->m_dwHP,
			pDEFEND->GetMaxMP(),
			pDEFEND->m_dwMP);

		vPLAYERS.pop_back();
	}
	vPLAYERS.clear();

	if(!pDEFEND->m_dwHP)
	{
		pDEFEND->OnDie(0,OT_NONE,0); 
		if(pDEFEND->m_bType == OT_RECALL)
			SendMW_RECALLMONDEL_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, dwTargetID);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_SETRETURNPOS_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wNpcID;

	pBUF->m_packet
		>> wNpcID;

	CTNpc * pNpc = FindTNpc(wNpcID);
	if(!pNpc || pNpc->m_bType != TNPC_RETURN)
	{
		pPlayer->SendCS_SETRETURNPOS_ACK(FALSE);
		return EC_NOERROR;
	}

	if(!pNpc->CanTalk(pPlayer->m_bCountry, pPlayer->m_bAidCountry, pPlayer->HaveDisguiseBuff()))
		return EC_NOERROR;

	WORD wSpawnPos = pNpc->GetSpawnPos();
	if(wSpawnPos)
	{
		pPlayer->m_wSpawnID = wSpawnPos;
		pPlayer->SendCS_SETRETURNPOS_ACK(TRUE);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_DEALITEMASK_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strTarget;

	pBUF->m_packet
		>> strTarget;

	if(pPlayer->m_bStore)
		return EC_NOERROR;

	CTPlayer * pTarget = FindPlayer(strTarget);
	if(!pTarget || !pTarget->m_bMain || pTarget->m_bStore)
		return EC_NOERROR;

	if(!pTarget->CheckProtected(pPlayer->m_dwID, PROTECTED_DEAL))
	{
		pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_DENY, strTarget);
		return EC_NOERROR;
	}

	if(pPlayer->IsActionBlock() || pTarget->IsActionBlock())
	{
		pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_BUSY, strTarget);
		return EC_NOERROR;
	}

	if(!pPlayer->CanTalk(CHAT_NEAR, pTarget->m_bCountry, pTarget->m_bAidCountry))
	{
		pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_CANTDEAL, strTarget);
		return EC_NOERROR;
	}

	pTarget->SendCS_DEALITEMASK_ACK(pPlayer->m_strNAME);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_DEALITEMRLY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bReply;
	CString strInviter;

	pBUF->m_packet
		>> bReply
		>> strInviter;

	if(pPlayer->m_dealItem.m_bStatus != DEAL_READY)
		return EC_NOERROR;

	CTPlayer * pInviter = FindPlayer(strInviter);
	if(!pInviter ||
		!pInviter->m_bMain ||
		pInviter->m_bStore ||
		pInviter->m_dealItem.m_bStatus != DEAL_READY)
	{
		pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_BUSY, strInviter);
		return EC_NOERROR;
	}

	if(bReply == ASK_YES)
	{
		pPlayer->SetDealItemTarget(strInviter);
		pInviter->SetDealItemTarget(pPlayer->m_strNAME);
		pInviter->SendCS_DEALITEMSTART_ACK(pPlayer->m_strNAME);
		pPlayer->SendCS_DEALITEMSTART_ACK(strInviter);
	}
	else
	{
		pInviter->SendCS_DEALITEMEND_ACK(bReply, pPlayer->m_strNAME);
		pPlayer->SendCS_DEALITEMEND_ACK(bReply, pPlayer->m_strNAME);
		pPlayer->ClearDealItem();
		pInviter->ClearDealItem();
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_DEALITEMADD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;
	BYTE bItemCount;
	BYTE bInvenID;
	BYTE bItemID;

	pBUF->m_packet
		>> dwGold
		>> dwSilver
		>> dwCooper
		>> bItemCount;

	if(pPlayer->m_dealItem.m_bDealing != DEAL_WAIT)
		return EC_NOERROR;

	BYTE bResult = DEALITEM_SUCCESS;
	__int64 llSendMoney = CalcMoney(dwGold, dwSilver, dwCooper);

	CString strTarget = pPlayer->m_dealItem.m_strTarget;
	CTPlayer * pTarget = FindPlayer(strTarget);
	if(!pTarget)
	{
		pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_NOTARGET, strTarget);
		pPlayer->ClearDealItem();
		return EC_NOERROR;
	}

	if(!pTarget->m_bMain ||
		pTarget->m_bStore ||
		pTarget->m_dealItem.m_strTarget != pPlayer->m_strNAME)
	{
		pTarget->SendCS_DEALITEMEND_ACK(DEALITEM_BUSY, strTarget);
		pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_BUSY, strTarget);
		pPlayer->ClearDealItem();
		pTarget->ClearDealItem();
		return EC_NOERROR;
	}

	if(!pPlayer->UseMoney(llSendMoney, FALSE))
	{
		pTarget->SendCS_DEALITEMEND_ACK(DEALITEM_NOMONEY, pPlayer->m_strNAME);
		pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_NOMONEY, pPlayer->m_strNAME);
		pPlayer->ClearDealItem();
		pTarget->ClearDealItem();
		return EC_NOERROR;
	}

    pPlayer->m_dealItem.m_llSendMoney = llSendMoney;

	for(BYTE i=0; i<bItemCount; i++)
	{
		pBUF->m_packet
			>> bInvenID
			>> bItemID;

		CTInven * pInven = pPlayer->FindTInven(bInvenID);
		if(!pInven)
		{
			bResult = DEALITEM_NOINVEN;
			break;
		}

		CTItem * pItem = pInven->FindTItem(bItemID);
		if(!pItem || !pItem->m_bCount)
		{
			bResult = DEALITEM_NOITEM;
			break;
		}

		if(!pItem->CanDeal())
		{
			bResult = DEALITEM_NOITEM;
			break;
		}

		if(!pPlayer->CheckDealItem(bInvenID, bItemID))
		{
			bResult = DEALITEM_INVALIDITEM;
			break;
		}

		CTItem * pSendItem = new CTItem();
		pSendItem->Copy(pItem, FALSE);
		pSendItem->m_bInven = bInvenID;
		pPlayer->m_dealItem.m_vSendItem.push_back(pSendItem);

		CTItem * pRecvItem = new CTItem();
		pRecvItem->Copy(pItem, FALSE);
		pTarget->m_dealItem.m_vRecvItem.push_back(pRecvItem);

		if(!pTarget->CanPush(&(pTarget->m_dealItem.m_vRecvItem), 0))
		{
			bResult = DEALITEM_CANTRECV;
			break;
		}
	}

	if(bResult == DEALITEM_SUCCESS)
	{
		pPlayer->m_dealItem.m_bDealing = DEAL_ADDITEM;
		pTarget->m_dealItem.m_llRecvMoney = llSendMoney;
		pTarget->SendCS_DEALITEMADD_ACK(
			dwGold,
			dwSilver,
			dwCooper,
			&(pTarget->m_dealItem.m_vRecvItem));
	}
	else
	{
		pTarget->SendCS_DEALITEMEND_ACK(bResult, bResult == DEALITEM_CANTRECV ? strTarget : pPlayer->m_strNAME);
		pPlayer->SendCS_DEALITEMEND_ACK(bResult, bResult == DEALITEM_CANTRECV ? strTarget : pPlayer->m_strNAME);
		pPlayer->ClearDealItem();
		pTarget->ClearDealItem();
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_DEALITEM_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bOkey;
	pBUF->m_packet
		>> bOkey;

	CString strTarget = pPlayer->m_dealItem.m_strTarget;

	CTPlayer * pTarget = FindPlayer(pPlayer->m_dealItem.m_strTarget);
	if(!pTarget)
	{
		pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_NOTARGET, strTarget);
		pPlayer->ClearDealItem();
		return EC_NOERROR;
	}

	if(!pTarget->m_bMain ||
		pTarget->m_bStore ||
		pTarget->m_dealItem.m_strTarget != pPlayer->m_strNAME)
	{
		pTarget->SendCS_DEALITEMEND_ACK(DEALITEM_BUSY, strTarget);
		pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_BUSY, strTarget);
		pPlayer->ClearDealItem();
		pTarget->ClearDealItem();
		return EC_NOERROR;
	}

	if(bOkey)
	{
		if(pPlayer->m_dealItem.m_bDealing != DEAL_ADDITEM)
			return EC_NOERROR;

		if(pPlayer->m_dealItem.m_bStatus == DEAL_CONFORM)
		{
			if(!pPlayer->ValidDealItem() || !pTarget->ValidDealItem())
			{
				pTarget->SendCS_DEALITEMEND_ACK(DEALITEM_BUSY, pPlayer->m_strNAME);
				pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_BUSY, pTarget->m_strNAME);
				pPlayer->ClearDealItem();
				pTarget->ClearDealItem();
				return EC_NOERROR;
			}

			if(!pPlayer->CanPush(&(pPlayer->m_dealItem.m_vRecvItem), 0))
			{
				pTarget->SendCS_DEALITEMEND_ACK(DEALITEM_CANTRECV, pPlayer->m_strNAME);
				pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_CANTRECV, pPlayer->m_strNAME);
				pPlayer->ClearDealItem();
				pTarget->ClearDealItem();
				return EC_NOERROR;
			}

			if(!pTarget->CanPush(&(pTarget->m_dealItem.m_vRecvItem), 0))
			{
				pTarget->SendCS_DEALITEMEND_ACK(DEALITEM_CANTRECV, strTarget);
				pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_CANTRECV, strTarget);
				pPlayer->ClearDealItem();
				pTarget->ClearDealItem();
				return EC_NOERROR;
			}

			SendDM_DELETEDEALITEM_REQ(pPlayer);

			for(DWORD i=0; i<pPlayer->m_dealItem.m_vRecvItem.size(); i++)
			{
#ifdef DEF_UDPLOG
				m_pUdpSocket->LogItemTrade(LOGMAP_ITEMTRADERECV, pPlayer, pPlayer->m_dealItem.m_vRecvItem[i], pPlayer->m_dealItem.m_strTarget );
#endif	//	DEF_UDPLOG				
			}
			
#ifdef DEF_UDPLOG		
			if( pPlayer->EarnMoney(pPlayer->m_dealItem.m_llRecvMoney) )
                m_pUdpSocket->LogMoneyTrade(LOGMAP_MONEYTRADERECV, pPlayer, pPlayer->m_dealItem.m_strTarget,  pPlayer->m_dealItem.m_llRecvMoney);
			
			if(pPlayer->UseMoney(pPlayer->m_dealItem.m_llSendMoney, TRUE) )
                m_pUdpSocket->LogMoneyTrade(LOGMAP_MONEYTRADESEND, pPlayer, pPlayer->m_dealItem.m_strTarget, - pPlayer->m_dealItem.m_llSendMoney);
#else 
			pPlayer->EarnMoney(pPlayer->m_dealItem.m_llRecvMoney);

			pPlayer->UseMoney(pPlayer->m_dealItem.m_llSendMoney, TRUE);
#endif
			pPlayer->EraseInvenDealItem();
			pPlayer->PushTItem(&(pPlayer->m_dealItem.m_vRecvItem));
			pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_SUCCESS, pPlayer->m_dealItem.m_strTarget);
			pPlayer->SendCS_MONEY_ACK();
			pPlayer->ClearDealItem();

			for(DWORD i=0; i<pTarget->m_dealItem.m_vRecvItem.size(); i++){
#ifdef DEF_UDPLOG
                m_pUdpSocket->LogItemTrade(LOGMAP_ITEMTRADERECV, pTarget, pTarget->m_dealItem.m_vRecvItem[i], pTarget->m_dealItem.m_strTarget );
#endif	//	DEF_UDPLOG					
			}

			
#ifdef DEF_UDPLOG
			if( pTarget->EarnMoney(pTarget->m_dealItem.m_llRecvMoney) )
                m_pUdpSocket->LogMoneyTrade(LOGMAP_MONEYTRADERECV, pTarget, pTarget->m_dealItem.m_strTarget,  pTarget->m_dealItem.m_llRecvMoney);			

			if( pTarget->UseMoney(pTarget->m_dealItem.m_llSendMoney, TRUE) )
                m_pUdpSocket->LogMoneyTrade(LOGMAP_MONEYTRADESEND, pTarget, pTarget->m_dealItem.m_strTarget, - pTarget->m_dealItem.m_llSendMoney);
#else 
			pTarget->EarnMoney(pTarget->m_dealItem.m_llRecvMoney);

			pTarget->UseMoney(pTarget->m_dealItem.m_llSendMoney, TRUE);
#endif
			pTarget->EraseInvenDealItem();
			pTarget->PushTItem(&(pTarget->m_dealItem.m_vRecvItem));
			pTarget->SendCS_DEALITEMEND_ACK(DEALITEM_SUCCESS, pPlayer->m_strNAME);
			pTarget->SendCS_MONEY_ACK();
			pTarget->ClearDealItem();

			SendDM_SAVEITEM_REQ(pPlayer);
			SendDM_SAVEITEM_REQ(pTarget);
		}
		else
		{
			pPlayer->m_dealItem.m_bDealing = DEAL_CONFORM;
			pPlayer->m_dealItem.m_bStatus = DEAL_CONFORM;
			pTarget->m_dealItem.m_bStatus = DEAL_CONFORM;
		}
	}
	else
	{
		pPlayer->SendCS_DEALITEMEND_ACK(DEALITEM_CANCEL, pPlayer->m_strNAME);
		pTarget->SendCS_DEALITEMEND_ACK(DEALITEM_CANCEL, pPlayer->m_strNAME);
		pTarget->ClearDealItem();
		pPlayer->ClearDealItem();
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_STOREOPEN_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strName;
	BYTE bCount;
	
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;
	BYTE bInvenID;
	BYTE bItemID;
	BYTE bItemCount;

	pBUF->m_packet
		>> strName
		>> bCount;

	if(pPlayer->m_dwRiding ||
		pPlayer->m_bStore ||
		pPlayer->m_dealItem.m_bStatus != DEAL_READY)
		return EC_NOERROR;

	if(strName==NAME_NULL ||
		!bCount ||
		pPlayer->IsTrans())
	{
		pPlayer->SendCS_STOREOPEN_ACK(STORE_FAIL, 0, NAME_NULL);
		return EC_NOERROR;
	}

	pPlayer->ClearStore();

	BYTE bResult = STORE_SUCCESS;
	
	for(BYTE i=0; i<bCount; i++)
	{
		pBUF->m_packet
			>> dwGold
			>> dwSilver
			>> dwCooper
			>> bInvenID
			>> bItemID
			>> bItemCount;

		CTInven * pInven = pPlayer->FindTInven(bInvenID);
		if(!pInven)
		{
			bResult = STORE_ITEM_NOITEM;
			break;
		}
		CTItem * pItem = pInven->FindTItem(bItemID);
		if(!pItem)
		{
			bResult = STORE_ITEM_NOITEM;
			break;
		}
		if(!pItem->CanDeal())
		{
			bResult = STORE_ITEM_NOTDEAL;
			break;
		}
		if(!pPlayer->CheckStoreItem(bInvenID, bItemID))
		{
			bResult = STORE_ITEM_NOTDEAL;
			break;
		}
		if(!bItemCount || pItem->m_bCount < bItemCount)
		{
			bResult = STORE_ITEM_NOITEMCOUNT;
			break;
		}

		LPTSTOREITEM pSTORE = new TSTOREITEM();
		pSTORE->m_bID = i;
		pSTORE->m_dwGold = dwGold;
		pSTORE->m_dwSilver = dwSilver;
		pSTORE->m_dwCooper = dwCooper;
		pSTORE->m_bInvenID = bInvenID;
		pSTORE->m_bItemID = bItemID;
		pSTORE->m_bItemCount = bItemCount;

		pPlayer->m_mapStoreItem.insert(MAPTSTOREITEM::value_type(pSTORE->m_bID, pSTORE));
	}

	if(bResult == STORE_SUCCESS)
	{
		pPlayer->m_bStore = TRUE;
		pPlayer->m_strStoreName = strName;
		// 강제버프걸기
		((CTObjBase*)pPlayer)->ForceMaintain(TSTORE_SKILL, pPlayer->m_dwID, OT_PC, pPlayer->m_dwID, pPlayer->m_bType, 0);
		pPlayer->SendCS_STOREOPEN_ACK(STORE_SUCCESS, pPlayer->m_dwID, strName);
		pPlayer->SendCS_STOREITEMLIST_ACK(pPlayer->m_dwID, pPlayer->m_strStoreName, pPlayer);

		VPLAYER vPlayer;
		vPlayer.clear();
		pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);
		for(DWORD i=0; i<vPlayer.size(); i++)
		{
			if(pPlayer != vPlayer[i])
				vPlayer[i]->SendCS_STOREOPEN_ACK(STORE_SUCCESS, pPlayer->m_dwID, strName);
		}
		vPlayer.clear();
	}
	else
	{
		pPlayer->m_bStore = FALSE;
		pPlayer->m_strStoreName = NAME_NULL;
		MAPTSTOREITEM::iterator it;
		for(it=pPlayer->m_mapStoreItem.begin(); it!=pPlayer->m_mapStoreItem.end(); it++)
			delete (*it).second;
		pPlayer->m_mapStoreItem.clear();
		pPlayer->SendCS_STOREOPEN_ACK(bResult, 0, NAME_NULL);
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_STORECLOSE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	pPlayer->StoreClose();

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_STOREITEMLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strTarget;

	pBUF->m_packet
		>> strTarget;

	CTPlayer * pTarget = FindPlayer(strTarget);
	if(!pTarget || !pTarget->m_bMain || !pTarget->m_bStore)
		return EC_NOERROR;

	if(pPlayer->GetWarCountry() != pTarget->GetWarCountry())
		return EC_NOERROR;
	
	pPlayer->SendCS_STOREITEMLIST_ACK(pTarget->m_dwID, pTarget->m_strStoreName, pTarget);
	
	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_STOREITEMBUY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strTarget;
	BYTE bItem;
	BYTE bItemCount;

	pBUF->m_packet
		>> strTarget
		>> bItem
		>> bItemCount;

	CTPlayer * pTarget = FindPlayer(strTarget);
	if(!pTarget || !pTarget->m_bMain || !pTarget->m_bStore)
	{
		pPlayer->SendCS_STOREITEMBUY_ACK(STORE_FAIL,0,0);
		return EC_NOERROR;
	}

	if(pPlayer->GetWarCountry() != pTarget->GetWarCountry())
	{
		pPlayer->SendCS_STOREITEMBUY_ACK(STORE_FAIL,0,0);
		return EC_NOERROR;
	}

	MAPTSTOREITEM::iterator itSTORE = pTarget->m_mapStoreItem.find(bItem);
	if(itSTORE==pTarget->m_mapStoreItem.end())
	{
		pPlayer->SendCS_STOREITEMBUY_ACK(STORE_ITEM_NOITEM,0,0);
		return EC_NOERROR;
	}

	LPTSTOREITEM pSTORE = (*itSTORE).second;

	CTInven * pTINVEN = pTarget->FindTInven(pSTORE->m_bInvenID);
	if(!pTINVEN)
	{
		pPlayer->SendCS_STOREITEMBUY_ACK(STORE_ITEM_NOITEM,0,0);
		return EC_NOERROR;
	}
	CTItem * pTITEM = pTINVEN->FindTItem(pSTORE->m_bItemID);
	if(!pTITEM || !pTITEM->m_bCount)
	{
		pPlayer->SendCS_STOREITEMBUY_ACK(STORE_ITEM_NOITEM,0,0);
		return EC_NOERROR;
	}

	if(!bItemCount || pSTORE->m_bItemCount<bItemCount)
	{
		pPlayer->SendCS_STOREITEMBUY_ACK(STORE_ITEM_NOITEMCOUNT,0,0);
		return EC_NOERROR;
	}

	__int64 llMoney = CalcMoney(pSTORE->m_dwGold, pSTORE->m_dwSilver, pSTORE->m_dwCooper);
	llMoney = llMoney*bItemCount;
	if(!pPlayer->UseMoney(llMoney, FALSE))
	{
		pPlayer->SendCS_STOREITEMBUY_ACK(STORE_ITEM_NEEDMONEY,0,0);
		return EC_NOERROR;
	}

	VTITEM vTITEM;
	vTITEM.clear();

	CTItem * pNew = new CTItem();
	pNew->Copy(pTITEM, pTITEM->m_bCount-bItemCount);
	pNew->m_bCount = bItemCount;
	WORD wItemID = pNew->m_wItemID;

	vTITEM.push_back(pNew);
	SetItemAttr(pNew, pTITEM->m_bLevel);

	if(!pPlayer->CanPush(&vTITEM, 0))
	{
		pPlayer->SendCS_STOREITEMBUY_ACK(STORE_ITEM_INVENFULL, 0, 0);
		delete pNew;
		vTITEM.clear();
		return EC_NOERROR;
	}
#ifdef DEF_UDPLOG
    CTItem logItem;
	logItem.Copy(pNew, FALSE);
#endif
	pPlayer->PushTItem(&vTITEM);
	vTITEM.clear();
	pPlayer->UseMoney(llMoney, TRUE);
	pPlayer->SendCS_MONEY_ACK();

#ifdef DEF_UDPLOG
	m_pUdpSocket->LogItemByStore(LOGMAP_STOREITEMBUY, pPlayer, pTarget->m_strNAME,	&logItem, -llMoney, bItemCount);
#endif

	pSTORE->m_bItemCount -= bItemCount;
	pTITEM->m_bCount -= bItemCount;

#ifdef DEF_UDPLOG
	if(pTITEM->m_bCount)
		m_pUdpSocket->LogItemByStore(LOGMAP_STOREITEMSELL, pTarget, pPlayer->m_strNAME,	pTITEM, llMoney, bItemCount);
	else 
		m_pUdpSocket->LogItemByStore(LOGMAP_STOREITEMDELL, pTarget, pPlayer->m_strNAME,	pTITEM, llMoney);
#endif

	if(!pTITEM->m_bCount)
	{
		delete pTITEM;
		pTINVEN->m_mapTITEM.erase(pSTORE->m_bItemID);
		pTarget->SendCS_DELITEM_ACK(pSTORE->m_bInvenID, pSTORE->m_bItemID);
	}
	else
		pTarget->SendCS_UPDATEITEM_ACK(pTITEM, pSTORE->m_bInvenID);

	pTarget->EarnMoney(llMoney);
	pTarget->SendCS_MONEY_ACK();
	pTarget->SendCS_STOREITEMSELL_ACK(bItem, bItemCount);
	if(!pSTORE->m_bItemCount)
	{
		pTarget->m_mapStoreItem.erase(pSTORE->m_bID);
		delete pSTORE;

		if(!pTarget->m_mapStoreItem.size())
			pTarget->StoreClose();
	}
	
	if(pTarget->m_bStore)
		pPlayer->SendCS_STOREITEMLIST_ACK(pTarget->m_dwID, pTarget->m_strStoreName, pTarget);
	pPlayer->SendCS_STOREITEMBUY_ACK(STORE_SUCCESS, wItemID, bItemCount);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_PETMAKE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bInven;
	BYTE bSlot;
	CString strName;

	pBUF->m_packet
		>> bInven
		>> bSlot
		>> strName;

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;

	if(strName.GetLength() > MAX_NAME)
	{
		pPlayer->SendCS_PETMAKE_ACK(PET_NOTFOUND, 0, NAME_NULL, 0);
		return EC_NOERROR;
	}

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
	{
		pPlayer->SendCS_PETMAKE_ACK(PET_NOTITEM, 0, NAME_NULL, 0);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bSlot);
	if(!pItem || !pItem->m_bCount)
	{
		pPlayer->SendCS_PETMAKE_ACK(PET_NOTITEM, 0, NAME_NULL, 0);
		return EC_NOERROR;
	}

	if(pItem->m_pTITEM->m_bType != IT_PET ||
		pItem->m_pTITEM->m_bKind != IK_PET)
	{
		pPlayer->SendCS_PETMAKE_ACK(PET_NOTITEM, 0, NAME_NULL, 0);
		return EC_NOERROR;
	}

	if(!pItem->CanUse())
	{
		pPlayer->SendCS_PETMAKE_ACK(PET_FAIL, 0, NAME_NULL, 0);
		return EC_NOERROR;
	}

	WORD wPetID = pItem->m_pTITEM->m_wUseValue;
	__int64 ldwTime = 0;
	if(pItem->m_pTITEM->m_bUseType & DURINGTYPE_TIME)
		ldwTime = __int64(pItem->m_pTITEM->m_wUseTime) * HOUR_ONE;
	else if(pItem->m_pTITEM->m_bUseType & DURINGTYPE_DAY)
		ldwTime = __int64(pItem->m_pTITEM->m_wUseTime) * DAY_ONE;

	MAPTPET::iterator itPET = pPlayer->m_mapTPET.find(MAKEWORD(pPlayer->m_bRace, wPetID));
	if(itPET != pPlayer->m_mapTPET.end() && !(*itPET).second->m_ldwTime)
	{
		pPlayer->SendCS_PETMAKE_ACK(PET_USETIME, 0, NAME_NULL, 0);
		return EC_NOERROR;
	}

	UseItem(pPlayer, pInven, pItem, 1);

	LPTPET pPET = pPlayer->PetMake(wPetID, strName, ldwTime);
	if(pPET)
	{
		pPlayer->SendCS_PETMAKE_ACK(PET_SUCCESS, pPET->m_wPetID, pPET->m_strName, pPET->m_ldwTime);
#ifdef DEF_UDPLOG
	m_pUdpSocket->LogPet(LOGMAP_PETMAKE, pPlayer, wPetID, strName, pPET->m_ldwTime);
#endif
	}
	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_PETDEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wPetID;

	pBUF->m_packet
		>> wPetID;

	MAPTPET::iterator itPET = pPlayer->m_mapTPET.find(wPetID);
	if(itPET == pPlayer->m_mapTPET.end())
	{
		pPlayer->SendCS_PETDEL_ACK(PET_NOTFOUND, wPetID);
		return EC_NOERROR;
	}

	LPTPET pPet = (*itPET).second;
	if(pPet->m_ldwTime > m_timeCurrent)
	{
		pPlayer->SendCS_PETDEL_ACK(PET_USETIME, wPetID);
		return EC_NOERROR;
	}

	delete pPet;
	pPlayer->m_mapTPET.erase(wPetID);

	pPlayer->SendCS_PETDEL_ACK(PET_SUCCESS, wPetID);

	if(!pPlayer->ProtectTutorial())
	{
		SendDM_PETDEL_REQ(pPlayer->m_dwUserID, HIBYTE(wPetID));
#ifdef DEF_UDPLOG
		m_pUdpSocket->LogPet(LOGMAP_PETDEL, pPlayer, wPetID);
#endif
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_PETRECALL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
	{
		pPlayer->SendCS_PETRECALL_ACK(PET_FAIL);
		return EC_NOERROR;
	}

	if(IsTournamentMap(pPlayer->m_wMapID))
	{
		pPlayer->SendCS_PETRECALL_ACK(PET_CANTUSE);
		return EC_NOERROR;
	}

	WORD wPetID;

	pBUF->m_packet
		>> wPetID;

	if(pPlayer->m_bStatus == OS_DEAD)
	{
		pPlayer->SendCS_PETRECALL_ACK(PET_FAIL);
		return EC_NOERROR;
	}

	if(pPlayer->m_dwDuelID)
	{
		pPlayer->SendCS_PETRECALL_ACK(PET_FAIL);
		return EC_NOERROR;
	}

	MAPTPET::iterator itPET = pPlayer->m_mapTPET.find(wPetID);
	if(itPET == pPlayer->m_mapTPET.end())
	{
		pPlayer->SendCS_PETRECALL_ACK(PET_NOTFOUND);
		return EC_NOERROR;
	}

	if((*itPET).second->m_ldwTime && (*itPET).second->m_ldwTime <= m_timeCurrent)
	{
		pPlayer->SendCS_PETRECALL_ACK(PET_USETIME);
		return EC_NOERROR;
	}

	MAPTMONSTERTEMP::iterator itMon = m_mapTMONSTER.find((*itPET).second->m_pTPET->m_wMonID);
	if(itMon == m_mapTMONSTER.end())
	{
		pPlayer->SendCS_PETRECALL_ACK(PET_NOTFOUND);
		return EC_NOERROR;
	}

	if(pPlayer->FindMaintainSkill(TBLOCKRIDE_SKILL))
	{
		pPlayer->SendCS_PETRECALL_ACK(PET_FAIL);
		return EC_NOERROR;
	}

	CTRecallMon * pRecall = pPlayer->FindRecallPet();
	if(pRecall)
		SendMW_RECALLMONDEL_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, pRecall->m_dwID);

	DWORD dwLive = PET_LIVE_DURATION;
	__int64 ldwLive = (*itPET).second->m_ldwTime ? (*itPET).second->m_ldwTime - m_timeCurrent : 0;
	if(ldwLive < dwLive)
		dwLive = DWORD(ldwLive);
	dwLive *= 1000;

	SendMW_CREATERECALLMON_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		0,
		(*itPET).second->m_pTPET->m_wMonID,
		MAKELONG((*itMon).second->m_wSummonAttr, pPlayer->m_bLevel),
		(*itPET).second->m_wPetID,
		(*itPET).second->m_strName,
		dwLive,
		pPlayer->m_bLevel,
		(*itMon).second->m_bClass,
		(*itMon).second->m_bRace,
		TA_STAND,
		OS_WAKEUP,
		MT_NORMAL,
		0,
		0,
		0,
		0,
		100,
		1,
		pPlayer->m_fPosX,
		pPlayer->m_fPosY,
		pPlayer->m_fPosZ,
		pPlayer->m_wDIR,
		(*itMon).second->m_vSKILL);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_PETRIDING_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwMonID;
	BYTE bAction;

	pBUF->m_packet
		>> dwMonID
		>> bAction;

	CTRecallMon * pRecall = pPlayer->FindRecallPet();
	if(!pRecall || pRecall->m_dwID!=dwMonID)
		return EC_NOERROR;

	if(pPlayer->m_bStore || pPlayer->m_bStatus == OS_DEAD)
		return EC_NOERROR;

	MAPTPET::iterator it;
	for(it=pPlayer->m_mapTPET.begin(); it!=pPlayer->m_mapTPET.end(); it++)
	{
		if((*it).second->m_pTPET->m_wMonID == pRecall->m_pMON->m_wID)
		{
			MAPBWORD::iterator find = (*it).second->m_pTPET->m_mapKind.find(PETKIND_ONE);
			if(find == (*it).second->m_pTPET->m_mapKind.end())
				return EC_NOERROR;
		}
	}

	if(bAction == PETACTION_RIDING)
	{
		for(DWORD i=0; i<pPlayer->m_vMaintainSkill.size(); i++)
		{
			if(pPlayer->m_vMaintainSkill[i]->m_pTSKILL->m_bIsRide)
				return EC_NOERROR;
		}
	}

	switch(bAction)
	{
	case PETACTION_RIDING:
		pPlayer->EraseBuffByRide();
		pPlayer->PetRiding(dwMonID);
		break;
	case PETACTION_DISMOUNT:
		pPlayer->PetRiding(0);
		break;
	default:
		return EC_NOERROR;
	}
 
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_OTHERSELF_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwHostID;
	DWORD dwMonID;

	pBUF->m_packet
		>> dwHostID
		>> dwMonID;

	MAPPLAYER::iterator it = m_mapPLAYER.find(dwHostID);
	if(it != m_mapPLAYER.end())
	{
		CTRecallMon * pMon = (*it).second->FindRecallMon(dwMonID);
		if(pMon)
			pPlayer->SendCS_OTHERSELF_ACK((*it).second, pMon, m_dwTick, TRUE);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_DISCONNECT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	CloseSession( pPlayer );
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CHGCHANNEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bChannel;
	pBUF->m_packet
		>> bChannel;

	if(pPlayer->m_bChannel == bChannel)
		return EC_NOERROR;

	if(IsMainCell(bChannel, pPlayer->m_wMapID, pPlayer->m_fPosX, pPlayer->m_fPosZ))
	{
		pPlayer->SendCS_CHGCHANNEL_ACK(CHC_SUCCESS, bChannel);

		SendMW_BEGINTELEPORT_ACK(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			TRUE,
			bChannel,
			pPlayer->m_wMapID,
			pPlayer->m_fPosX,
			pPlayer->m_fPosY,
			pPlayer->m_fPosZ);

		return EC_NOERROR;
	}

	if(IsMeetingRoom(pPlayer->m_wMapID, TRUE) || pPlayer->m_wArenaID)
	{
		pPlayer->SendCS_CHGCHANNEL_ACK(CHC_NOCHANNEL, pPlayer->m_bChannel);
		return EC_NOERROR;
	}

	Teleport(
		pPlayer,
		bChannel,
		pPlayer->m_wMapID,
		pPlayer->m_fPosX,
		pPlayer->m_fPosY,
		pPlayer->m_fPosZ);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CANCELSKILL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bType=OT_PC;
	DWORD dwID=pPlayer->m_dwID;
	WORD wSkillID;
	pBUF->m_packet
		>> bType
		>> dwID
		>> wSkillID;

	CTObjBase *pDEFEND = FindTarget(pPlayer, bType, dwID);
	if(pDEFEND)
		pDEFEND->CancelSkill(wSkillID, m_dwTick);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_DUELINVITE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	DWORD dwTarget;
	pBUF->m_packet
		>> dwTarget;

	if(pPlayer->m_wMapID)
	{
		pPlayer->SendCS_DUELSTART_ACK(DUEL_FAIL, pPlayer->m_dwID, dwTarget);
		return EC_NOERROR;
	}

	if(pPlayer->m_dwDuelID ||
		pPlayer->IsActionBlock() ||
		pPlayer->HaveDieBuff())
	{
		pPlayer->SendCS_DUELSTART_ACK(DUEL_FAIL, pPlayer->m_dwID, dwTarget);
		return EC_NOERROR;
	}

	MAPPLAYER::iterator it = m_mapPLAYER.find(dwTarget);
	if(it == m_mapPLAYER.end())
	{
		pPlayer->SendCS_DUELSTART_ACK(DUEL_FAIL, pPlayer->m_dwID, dwTarget);
		return EC_NOERROR;
	}

	CTPlayer * pTarget = (*it).second;
	if(!pTarget->CheckProtected(pPlayer->m_dwID, PROTECTED_DUEL))
	{
		pPlayer->SendCS_DUELSTART_ACK(DUEL_REFUSE, pPlayer->m_dwID, dwTarget);
		return EC_NOERROR;
	}

	if(pTarget &&
		pTarget->m_bMain &&
		!pTarget->m_dwDuelID &&
		!pTarget->IsActionBlock() &&
		!pTarget->HaveDieBuff())
	{
		pTarget->SendCS_DUELINVITE_ACK(pPlayer->m_dwID);
		return EC_NOERROR;
	}

	pPlayer->SendCS_DUELSTART_ACK(DUEL_BUSY, pPlayer->m_dwID, dwTarget);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_DUELINVITEREPLY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bResult;
	DWORD dwInviter;
	pBUF->m_packet
		>> bResult
		>> dwInviter;

	MAPPLAYER::iterator it = m_mapPLAYER.find(dwInviter);
	if(it == m_mapPLAYER.end())
	{
		pPlayer->SendCS_DUELSTART_ACK(DUEL_FAIL, dwInviter, pPlayer->m_dwID);
		return EC_NOERROR;
	}

	CTPlayer * pInviter = (*it).second;

	if(	!pInviter->m_bMain ||
		pInviter->IsActionBlock())
	{
		pPlayer->SendCS_DUELSTART_ACK(DUEL_FAIL, dwInviter, pPlayer->m_dwID);
		pInviter->SendCS_DUELSTART_ACK(DUEL_FAIL, dwInviter, pPlayer->m_dwID);
		return EC_NOERROR;
	}

	if(pPlayer->IsActionBlock())
	{
		pPlayer->SendCS_DUELSTART_ACK(DUEL_BUSY, dwInviter, pPlayer->m_dwID);
		pInviter->SendCS_DUELSTART_ACK(DUEL_BUSY, dwInviter, pPlayer->m_dwID);
		return EC_NOERROR;
	}

	if(pInviter->m_dwDuelID || pPlayer->m_dwDuelID)
		bResult = DUEL_FAIL;

	if(bResult == ASK_YES)
	{
		FLOAT fPosX = 0.0;
		FLOAT fPosZ = 0.0;

		fPosX = FLOAT((pInviter->m_fPosX + pPlayer->m_fPosX)/2.0);
		fPosZ = FLOAT((pInviter->m_fPosZ+ pPlayer->m_fPosZ)/2.0);

		if( GetDistance(fPosX,fPosZ, pInviter->m_fPosX, pInviter->m_fPosZ) >= DUEL_AREARANGE ||
			GetDistance(fPosX,fPosZ, pPlayer->m_fPosX,pPlayer->m_fPosZ) >= DUEL_AREARANGE )
		{
			pPlayer->SendCS_DUELSTART_ACK(DUEL_FAIL, dwInviter, pPlayer->m_dwID);
			pInviter->SendCS_DUELSTART_ACK(DUEL_FAIL, dwInviter, pPlayer->m_dwID);
			return EC_NOERROR;
		}

		DWORD dwDuelID = GetDuelID();
		pPlayer->m_dwDuelID = pInviter->m_dwDuelID = dwDuelID;

		pInviter->m_dwDuelTarget = pPlayer->m_dwID;
		pPlayer->m_dwDuelTarget = pInviter->m_dwID;

		// PET Clear
		CTRecallMon * pRecall = pInviter->FindRecallPet();
		if(pRecall)
			SendMW_RECALLMONDEL_ACK(pInviter->m_dwID, pInviter->m_dwKEY, pRecall->m_dwID);

		pRecall = pPlayer->FindRecallPet();
		if(pRecall)
			SendMW_RECALLMONDEL_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, pRecall->m_dwID);

		SendSM_DUELSTART_REQ(
			dwDuelID,
			dwInviter,
			pInviter->m_strNAME,
			pPlayer->m_dwID,
			pPlayer->m_strNAME);

		pInviter->SendCS_DUELSTANDBY_ACK(dwInviter, pPlayer->m_dwID, fPosX, fPosZ);
		pPlayer->SendCS_DUELSTANDBY_ACK(dwInviter, pPlayer->m_dwID, fPosX, fPosZ);
	}
	else
	{
		pInviter->SendCS_DUELSTART_ACK(bResult, dwInviter, pPlayer->m_dwID);
		pPlayer->SendCS_DUELSTART_ACK(bResult, dwInviter, pPlayer->m_dwID);
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_DUELEND_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwDuelID)
		return EC_NOERROR;

	SendSM_DUELEND_REQ(pPlayer->m_dwDuelID, pPlayer->m_dwID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GETTARGET_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwCharID;

	pBUF->m_packet
		>> dwCharID;
	
	MAPPLAYER::iterator it = m_mapPLAYER.find(dwCharID);
	if( it != m_mapPLAYER.end() )
		(*it).second->SendCS_GETTARGETANS_ACK(pPlayer->m_dwID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GETTARGETANS_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	DWORD dwAnswerCharID;
	DWORD dwTargetID;
	BYTE bTargetType;

	pBUF->m_packet
		>> dwAnswerCharID
		>> dwTargetID
		>> bTargetType;

	MAPPLAYER::iterator it = m_mapPLAYER.find(dwAnswerCharID);
	if( it != m_mapPLAYER.end() )
		(*it).second->SendCS_GETTARGET_ACK(dwTargetID, bTargetType);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_SMSSEND_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bInven;
	BYTE bItem;
	BYTE bType;
	CString strTarget;
	CString strMsg;

	pBUF->m_packet
		>> bInven
		>> bItem
		>> bType
		>> strTarget
		>> strMsg;

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;

	if(strTarget.GetLength() > MAX_NAME)
		return EC_NOERROR;

	strMsg.Left(MAX_SMS_MSG);

	CTInven *pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
	{
		pPlayer->SendCS_SMSSEND_ACK(SMS_NOINVEN, bType, strTarget);
		return EC_NOERROR;
	}

	CTItem *pItem = pInven->FindTItem(bItem);
	if(!pItem)
	{
		pPlayer->SendCS_SMSSEND_ACK(SMS_NOITEM, bType, strTarget);
		return EC_NOERROR;
	}

	if(pItem->m_pTITEM->m_bType != IT_USE ||
		(pItem->m_pTITEM->m_bKind != IK_SMSGUILD &&
		pItem->m_pTITEM->m_bKind != IK_SMSPERSON))
	{
		pPlayer->SendCS_SMSSEND_ACK(SMS_INVALIDITEM, bType, strTarget);
		return EC_NOERROR;
	}

	if(pItem->m_pTITEM->m_bKind == IK_SMSGUILD)
	{
		if( !pPlayer->m_dwGuildID )
		{
			pPlayer->SendCS_SMSSEND_ACK(SMS_NOGUILD, bType, strTarget);
			return EC_NOERROR;
		}
		if( pPlayer->m_strGuildName != strTarget)
		{
			pPlayer->SendCS_SMSSEND_ACK(SMS_NOTMYGUILD, bType, strTarget);
			return EC_NOERROR;
		}
	}

	SendDM_SMSSEND_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		bInven,
		bItem,
		pItem->m_pTITEM->m_wItemID,
		bType,
		pPlayer->m_dwGuildID,
		strTarget,
		strMsg);

	UseItem(pPlayer, pInven, pItem, 1);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_SKILLINIT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wSkillID;
	BYTE bInvenID;
	BYTE bItemID;

	pBUF->m_packet
		>> wSkillID
		>> bInvenID
		>> bItemID;

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;

	CTInven * pInven = pPlayer->FindTInven(bInvenID);
	if(!pInven)
	{
		pPlayer->SendCS_SKILLINIT_ACK(SKILL_NEEDITEM);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bItemID);
	if(!pItem || !pItem->m_bCount)
	{
		pPlayer->SendCS_SKILLINIT_ACK(SKILL_NEEDITEM);
		return EC_NOERROR;
	}

	if(pItem->m_pTITEM->m_bType != IT_USE)
	{
		pPlayer->SendCS_SKILLINIT_ACK(SKILL_NEEDITEM);
		return EC_NOERROR;
	}

	if(wSkillID &&
		pItem->m_pTITEM->m_bKind == IK_SKILLALLINIT)
	{
		pPlayer->SendCS_SKILLINIT_ACK(SKILL_NEEDITEM);
		return EC_NOERROR;
	}

	if(!wSkillID &&
		pItem->m_pTITEM->m_bKind == IK_SKILLONEINIT)
	{
		pPlayer->SendCS_SKILLINIT_ACK(SKILL_NOTFOUND);
		return EC_NOERROR;
	}

	CTSkill * pSkill = NULL;
	if(wSkillID)
	{
		pSkill = pPlayer->FindTSkill(wSkillID);
		if(!pSkill)
		{
			pPlayer->SendCS_SKILLINIT_ACK(SKILL_NOTFOUND);
			return EC_NOERROR;
		}

		if(pSkill->m_pTSKILL->m_bKind == NORMAL_EQUIP)
		{
			pPlayer->SendCS_SKILLINIT_ACK(SKILL_NOTINIT);
			return EC_NOERROR;
		}

		// 기본적으로 주는 스킬
		if(pSkill->m_pTSKILL->m_bStartLevel == 0 &&
			pSkill->m_bLevel == 1)
		{
			pPlayer->SendCS_SKILLINIT_ACK(SKILL_NOTINIT);
			return EC_NOERROR;
		}

		// 최하단 스킬이 아니라면
		if(pPlayer->FindTChildSkill(wSkillID))
		{
			pPlayer->SendCS_SKILLINIT_ACK(SKILL_HAVECHILD);
			return EC_NOERROR;
		}
	}

	if(pItem->m_pTITEM->m_bKind == IK_SKILLONEINIT ||
		pItem->m_pTITEM->m_bKind == IK_SKILLALLINIT) 
		pPlayer->InitializeSkill(pSkill);
	else
	{
		pPlayer->SendCS_SKILLINIT_ACK(SKILL_NEEDITEM);
		return EC_NOERROR;
	}

	UseItem(pPlayer, pInven, pItem, 1);

	pPlayer->SendCS_SKILLINIT_ACK(SKILL_SUCCESS, wSkillID);
	pPlayer->SendCS_SKILLLIST_ACK(m_dwTick);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_SKILLINITPOSSIBLE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bInvenID;
	BYTE bItemID;

	pBUF->m_packet
		>> bInvenID
		>> bItemID;

	CTInven * pInven = pPlayer->FindTInven(bInvenID);
	if(!pInven)
		return EC_NOERROR;

	CTItem * pItem = pInven->FindTItem(bItemID);
	if(!pItem || !pItem->m_bCount)
		return EC_NOERROR;

	if(pItem->m_pTITEM->m_bType != IT_USE)
		return EC_NOERROR;

	if(	pItem->m_pTITEM->m_bKind != IK_SKILLONEINIT)
		return EC_NOERROR;

	BYTE bLevel = BYTE(pItem->m_pTITEM->m_wUseValue);

	VWORD vSkill;
	vSkill.clear();

	pPlayer->InitSkillPossible(bLevel, &vSkill);
	pPlayer->SendCS_SKILLINITPOSSIBLE_ACK(vSkill);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_HELMETHIDE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bHide;

	pBUF->m_packet
		>> bHide;

	if(pPlayer->m_bHelmetHide == bHide)
		return EC_NOERROR;

	SendMW_HELMETHIDE_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, bHide);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_PARTYMEMBERRECALL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bInvenID;
	BYTE bItemID;
	CString strName;

	pBUF->m_packet
		>> bInvenID
		>> bItemID
		>> strName;

	CString strOrigin;
	CString strTarget;

	if(pPlayer->m_bStore)
		return EC_NOERROR;

	if(pPlayer->m_dwRiding)
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_RIDING, TP_NONEDEF, strName);
		return EC_NOERROR;
	}

	if(pPlayer->m_dealItem.m_bStatus >= DEAL_START)
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_DEALING, TP_NONEDEF, strName);
		return EC_NOERROR;
	}

	CTInven * pInven = pPlayer->FindTInven(bInvenID);
	if(!pInven)
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTFOUND, TP_NONEDEF, strName);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bItemID);
	if(!pItem || !pItem->m_bCount)
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTFOUND, TP_NONEDEF, strName);
		return EC_NOERROR;
	}

	if(pItem->m_pTITEM->m_bType != IT_USE)
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTFOUND, TP_NONEDEF, strName);
		return EC_NOERROR;
	}

	if(!pItem->CanUse())
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_WRAPPING, TP_NONEDEF, strName);
		return EC_NOERROR;
	}

	if(IsTournamentMap(pPlayer->m_wMapID) ||
		GetLoungeMapID() == pPlayer->m_wMapID)
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTFOUND, TP_NONEDEF, strName);
		return EC_NOERROR;
	}

	if(IsMeetingRoom(pPlayer->m_wMapID, TRUE))
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_TARGETBUSY, TP_NONEDEF, strName);
		return EC_NOERROR;
	}

	if(pPlayer->m_wArenaID)
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_TARGETBUSY, TP_NONEDEF, strName);
		return EC_NOERROR;
	}

	switch(pItem->m_pTITEM->m_bKind)
	{
	case IK_MEMBERRECALL:
		{
			if(pPlayer->m_strNAME == strName)
			{
				pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_TARGETNOTFOUND, TP_RECALL, strName);
				return EC_NOERROR;
			}

			if(!pPlayer->m_wPartyID)
			{
				pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTPARTY, TP_RECALL, strName);
				return EC_NOERROR;
			}

			strOrigin = pPlayer->m_strNAME;
			strTarget = strName;
		}
		break;
	case IK_MOVETOCHAR:
		{
			strOrigin = strName;
			strTarget = pPlayer->m_strNAME;
		}
		break;
	default:
		{
			pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTFOUND,  TP_NONEDEF, strName);
			return EC_NOERROR;
		}
		break;
	}

	SendMW_PARTYMEMBERRECALL_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		bInvenID,
		bItemID,
		strOrigin,
		strTarget);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_PARTYMEMBERRECALLANS_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bAnswer;
	CString strInviter;
	BYTE bInven;
	BYTE bItem;

	pBUF->m_packet
		>> bAnswer
		>> strInviter
		>> bInven
		>> bItem;

	SendMW_PARTYMEMBERRECALLANS_ACK(
		!bAnswer ? IU_SUCCESS : (bAnswer == ASK_NO ? IU_TARGETDENY : IU_TARGETBUSY),
		strInviter,
		pPlayer->m_strNAME,
		0,
		bInven,
		bItem,
		pPlayer->m_bChannel,
		pPlayer->m_wMapID,
		pPlayer->m_fPosX,
		pPlayer->m_fPosY,
		pPlayer->m_fPosZ);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CASHITEMCABINET_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	SendDM_CASHITEMCABINET_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_dwUserID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CASHITEMGET_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	DWORD dwID;
	BYTE bInven;
	BYTE bSlot;

	pBUF->m_packet
		>> dwID
		>> bInven
		>> bSlot;

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
		return EC_NOERROR;

	CTItem * pItem = pInven->FindTItem(bSlot);
	if(pItem)
		return EC_NOERROR;

	SendDM_SAVEITEM_REQ(pPlayer);

	SendDM_CASHITEMGET_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_strNAME,
		pPlayer->m_dwUserID,
		dwID,
		bInven,
		bSlot);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CASHSHOPITEMLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wItemID;
	pBUF->m_packet
		>> wItemID;
	 
	if(m_bCashShopStop)
	{
		pPlayer->SendCS_CASHSHOPSTOP_ACK(m_bCashShopStop);
		return EC_NOERROR;
	}

	SendDM_CASHSHOPITEMLIST_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_dwUserID,
		wItemID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CASHITEMBUY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wCashItemID;

	pBUF->m_packet
		>> wCashItemID;
	 
	if(m_bCashShopStop)
	{
		pPlayer->SendCS_CASHSHOPSTOP_ACK(m_bCashShopStop);
		return EC_NOERROR;
	}

	SendDM_CASHITEMBUY_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_dwUserID,
		wCashItemID,
		CASHITEM_BUY,
		NAME_NULL);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CASHITEMPRESENT_REQ(LPPACKETBUF pBUF)

{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strTarget = NAME_NULL;
	WORD wCashItemID = 0;

	pBUF->m_packet
		>> strTarget
		>> wCashItemID;
	 
	if(m_bCashShopStop)
	{
		pPlayer->SendCS_CASHSHOPSTOP_ACK(m_bCashShopStop);
		return EC_NOERROR;
	}

	if(strTarget.IsEmpty() || strTarget.GetLength() > MAX_NAME)
	{
		pPlayer->SendCS_CASHITEMBUY_ACK(2, 0, 0);
		return EC_NOERROR;
	}

	SendDM_CASHITEMBUY_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_dwUserID,
		wCashItemID,
		CASHITEM_PRESENT,
		strTarget);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_SOULMATESEARCH_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bNpcInvenID;  
	BYTE bNpcItemID;

	pBUF->m_packet
		>> bNpcInvenID  
		>> bNpcItemID;

    if(!pPlayer->CheckSoulSilence((DWORD)m_timeCurrent))
	{
		pPlayer->SendCS_SOULMATESEARCH_ACK(SOULMATE_SILENCE);
		return EC_NOERROR;
	}

	if(!pPlayer->UseMoney(pPlayer->m_pTLEVEL->m_dwSearchCost, FALSE))
	{
		pPlayer->SendCS_SOULMATESEARCH_ACK(SOULMATE_NEEDMONEY);
		return EC_NOERROR;
	}
	
	 
	if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
	{
		if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
		{
			pPlayer->SendCS_SOULMATESEARCH_ACK(SOULMATE_INVALIDPOS);
			return EC_NOERROR;
		}

		CTItem * pNpcItem = NULL;
		CTInven * pNpcInven = NULL;

		pNpcInven = pPlayer->FindTInven(bNpcInvenID);
		if(!pNpcInven)
		{
			pPlayer->SendCS_SOULMATESEARCH_ACK(SOULMATE_NPCCALLERROR);
			return EC_NOERROR;
		}

		pNpcItem = pNpcInven->FindTItem(bNpcItemID);
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL || !pNpcItem->m_bCount)
		{
			pPlayer->SendCS_SOULMATESEARCH_ACK(SOULMATE_NPCCALLERROR);
			return EC_NOERROR;
		}
	}

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	pPlayer->m_pMAP->GetNeighbor(
		&vPLAYERS,
		pPlayer->m_fPosX,
		pPlayer->m_fPosZ);

	BYTE bLevel = 200;

	VPLAYER::iterator it;
	for(it=vPLAYERS.begin(); it!=vPLAYERS.end(); it++)
	{
		if((*it)->m_dwID != pPlayer->m_dwID &&
			abs(pPlayer->m_bLevel - (*it)->m_bLevel) < 3)
		{
			if(bLevel > (*it)->m_bLevel)
				bLevel = (*it)->m_bLevel;
		}
	}

	SendMW_SOULMATESEARCH_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY,
		bLevel,
		bNpcInvenID,  
		bNpcItemID);

	vPLAYERS.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_SOULMATEREGREADY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strName;

	BYTE bNpcInvenID; 
	BYTE bNpcItemID; 

	pBUF->m_packet
		>> strName 
		>> bNpcInvenID  
		>> bNpcItemID;

	if(!pPlayer->CheckSoulSilence((DWORD)m_timeCurrent))
	{
		pPlayer->SendCS_SOULMATEREGREADY_ACK(SOULMATE_SILENCE,NAME_NULL,bNpcInvenID,bNpcItemID);
		return EC_NOERROR;
	}

	if(pPlayer->m_strNAME == strName)
		return EC_NOERROR;

	if(pPlayer->m_strSoulmate == strName)
	{
		pPlayer->SendCS_SOULMATEREGREADY_ACK(SOULMATE_ALREADY,NAME_NULL,bNpcInvenID,bNpcItemID);
		return EC_NOERROR;
	}

	if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
	{
		if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
		{
			pPlayer->SendCS_SOULMATEREGREADY_ACK(SOULMATE_INVALIDPOS,NAME_NULL,bNpcInvenID,bNpcItemID);
			return EC_NOERROR;
		}

		CTItem * pNpcItem = NULL;
		CTInven * pNpcInven = NULL;

		pNpcInven = pPlayer->FindTInven(bNpcInvenID);
		if(!pNpcInven)
		{
			pPlayer->SendCS_SOULMATEREGREADY_ACK(SOULMATE_NPCCALLERROR,NAME_NULL, bNpcInvenID, bNpcItemID);
			return EC_NOERROR;
		}
		pNpcItem = pNpcInven->FindTItem(bNpcItemID);
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL || !pNpcItem->m_bCount)
		{
			pPlayer->SendCS_SOULMATEREGREADY_ACK(SOULMATE_NPCCALLERROR,NAME_NULL, bNpcInvenID, bNpcItemID);
			return EC_NOERROR;
		}		
	}

	SendMW_SOULMATEREG_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY,
		strName,
		FALSE,		
		bNpcInvenID,  
		bNpcItemID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_SOULMATEREG_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strName;
	BYTE bNpcInvenID; 
	BYTE bNpcItemID;

	pBUF->m_packet
		>> strName		
		>> bNpcInvenID
		>> bNpcItemID;

	if(!pPlayer->CheckSoulSilence((DWORD)m_timeCurrent))
	{
		pPlayer->SendCS_SOULMATEREG_ACK(SOULMATE_SILENCE);
		return EC_NOERROR;
	}

	if(pPlayer->m_strNAME == strName)
		return EC_NOERROR;

	if(pPlayer->m_strSoulmate == strName)
	{
		pPlayer->SendCS_SOULMATEREG_ACK(SOULMATE_ALREADY);
		return EC_NOERROR;
	}

	if(!pPlayer->UseMoney(pPlayer->m_pTLEVEL->m_dwRegCost, FALSE))
	{
		pPlayer->SendCS_SOULMATEREG_ACK(SOULMATE_NEEDMONEY);
		return EC_NOERROR;
	}

	if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
	{
		if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
		{
			pPlayer->SendCS_SOULMATEREG_ACK(SOULMATE_NPCCALLERROR);
			return EC_NOERROR;
		}		

		CTItem * pNpcItem = NULL;
		CTInven * pNpcInven = NULL;

		pNpcInven = pPlayer->FindTInven(bNpcInvenID);
		if(!pNpcInven)
		{
			pPlayer->SendCS_SOULMATEREG_ACK(SOULMATE_NPCCALLERROR);
			return EC_NOERROR;
		}
		pNpcItem = pNpcInven->FindTItem(bNpcItemID);
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL || !pNpcItem->m_bCount)
		{
			pPlayer->SendCS_SOULMATEREG_ACK(SOULMATE_NPCCALLERROR);
			return EC_NOERROR;
		}
	}

	SendMW_SOULMATEREG_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY,
		strName,
		TRUE,
		bNpcInvenID,  
		bNpcItemID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_SOULMATEEND_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!pPlayer->m_dwSoulmate)
		return EC_NOERROR;

	SendMW_SOULMATEEND_ACK(
		pPlayer->m_dwID, 
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_GAMBLECHECK_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bInven;
	BYTE bItem;
	WORD wNpcID;  

	pBUF->m_packet
		>> bInven
		>> bItem
		>> wNpcID;

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
	{
		pPlayer->SendCS_GAMBLECHECK_ACK(GAMBLE_INVALIDITEM);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bItem);
	if(!pItem || !pItem->m_bCount)
	{
		pPlayer->SendCS_GAMBLECHECK_ACK(GAMBLE_INVALIDITEM);
		return EC_NOERROR;
	}

	LPTLEVEL pLevel = FindTLevel(pItem->m_bGLevel);
	if(!pLevel)
	{
		pPlayer->SendCS_GAMBLECHECK_ACK(GAMBLE_INVALIDITEM);
		return EC_NOERROR;
	}
	
	
	DWORD dwDiscountCost = 0;
	CTNpc* pNpc = FindTNpc(wNpcID);
	BYTE bDiscountRate = GetDiscountRate(pPlayer,pNpc);	
	
	DWORD dwMoney = 0;

	if(pItem->m_pTITEM->m_bType == IT_GAMBLE && !pItem->m_pTITEM->m_bCanGamble)
		dwMoney = pLevel->m_dwGambleCost;
	else if(pItem->m_pTITEM->m_bCanGamble && pItem->m_bGLevel)
		dwMoney = pLevel->m_dwRepCost;
	else if(pItem->m_dwExtValue[IEV_WRAP])
		dwMoney = 0;
	else
	{
		pPlayer->SendCS_GAMBLECHECK_ACK(GAMBLE_INVALIDITEM);
		return EC_NOERROR;
	}

	dwDiscountCost = dwMoney - DWORD(dwMoney * bDiscountRate / 100 );  
	
	if(!pPlayer->UseMoney(dwDiscountCost, FALSE))
	{
		pPlayer->SendCS_GAMBLECHECK_ACK(GAMBEL_NEEDMONEY);
		return EC_NOERROR;
	}

	pPlayer->SendCS_GAMBLECHECK_ACK(
		GAMBLE_SUCCESS,
		bInven,
		bItem,
		dwMoney,
		bDiscountRate);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_GAMBLEOPEN_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bInven;
	BYTE bItem;
	WORD wNpcID;
	BYTE bNpcInvenID;  
	BYTE bNpcItemID;

	pBUF->m_packet
		>> bInven
		>> bItem
		>> wNpcID
		>> bNpcInvenID  
		>> bNpcItemID;
    
	BYTE bChanged = FALSE;

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
	{
		pPlayer->SendCS_GAMBLEOPEN_ACK(GAMBLE_INVALIDITEM, bInven, bItem);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bItem);
	if(!pItem || !pItem->m_bCount)
	{
		pPlayer->SendCS_GAMBLEOPEN_ACK(GAMBLE_INVALIDITEM, bInven, bItem);
		return EC_NOERROR;
	}

	BYTE bResult = GAMBLE_SUCCESS;
	DWORD dwMoney = 0;
	WORD wNewItem = 0;
	BYTE bNewCount = 0;
	CTItem * pNpcItem = NULL;
	CTInven * pNpcInven = NULL;

	if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
	{
		if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
		{
			pPlayer->SendCS_GAMBLEOPEN_ACK(GAMBLE_INVALIDPOS,bNpcInvenID,bNpcItemID);
			return EC_NOERROR;
		}

		pNpcInven = pPlayer->FindTInven(bNpcInvenID);
		if(!pNpcInven)
			return EC_NOERROR;
		pNpcItem = pNpcInven->FindTItem(bNpcItemID);
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL || !pNpcItem->m_bCount)
			return EC_NOERROR;
	}

	if(pItem->m_dwExtValue[IEV_WRAP])
	{
		pItem->m_dwExtValue[IEV_WRAP] = FALSE;
	}
	else
	{
		LPTLEVEL pLevel = FindTLevel(pItem->m_bGLevel);
		if(!pLevel)
		{
			pPlayer->SendCS_GAMBLEOPEN_ACK(GAMBLE_INVALIDITEM, bInven, bItem);
			return EC_NOERROR;
		}

		CTNpc* pNpc = FindTNpc(wNpcID);
		BYTE bDiscountRate = GetDiscountRate(pPlayer,pNpc);

		if(pItem->m_pTITEM->m_bType == IT_GAMBLE && !pItem->m_pTITEM->m_bCanGamble)
			dwMoney = pLevel->m_dwGambleCost;
		else if(pItem->m_pTITEM->m_bCanGamble && pItem->m_bGLevel)
			dwMoney = pLevel->m_dwRepCost;
		else
		{
			pPlayer->SendCS_GAMBLEOPEN_ACK(GAMBLE_INVALIDITEM, bInven, bItem);
			return EC_NOERROR;
		}
		
		dwMoney -= DWORD(dwMoney * bDiscountRate / 100 );

		if(!pPlayer->UseMoney(dwMoney, FALSE))
		{
			pPlayer->SendCS_GAMBLEOPEN_ACK(GAMBEL_NEEDMONEY, bInven, bItem);
			return EC_NOERROR;
		}

		CTItem * pNew = new CTItem();
		pNew->Copy(pItem, TRUE);
		pNew->m_bCount = 1;

		BYTE bProb = CalcProb(pPlayer, pNpc, PROB_GAMBLE, pNew->m_pTITEM->m_bGambleProb);

		if( bProb > rand() % 100)
		{
			MAPTGAMBLE::iterator it = m_mapTGAMBLE.find(MAKEWORD(pNew->m_pTITEM->m_bType, pNew->m_pTITEM->m_bKind));
			if(it == m_mapTGAMBLE.end())
				bResult = GAMBLE_FAIL;
			else
			{
				WORD wProb = 0;
				VTGAMBLE vGmb;
				vGmb.clear();
				LPTGAMBLE pGamble;
				for(DWORD i=0; i<(*it).second.size(); i++)
				{
					pGamble = (*it).second[i];
					if(pGamble->m_bMinLevel <= pNew->m_bGLevel && pGamble->m_bMaxLevel >= pNew->m_bGLevel)
					{
						vGmb.push_back(pGamble);
						wProb += pGamble->m_wProb;
					}
				}

				if(vGmb.empty())
					bResult = GAMBLE_FAIL;
				else
				{
					WORD wSelected = rand() % wProb;
					wProb = 0;
					for(DWORD s=0; s<vGmb.size(); s++)
					{
						wProb += vGmb[s]->m_wProb;
						if( wProb >= wSelected)
						{
							pGamble = vGmb[s];
							break;
						}
					}

					pNew->m_bCount = (rand() % pGamble->m_bCountMax) + 1;
					LPTITEM pTEMP = FindTItem(pGamble->m_wReplaceID);
					if(pTEMP)
					{
						pNew->m_pTITEM = pTEMP;
						pNew->m_wItemID = pGamble->m_wReplaceID;
						SetItemAttr(pNew, pNew->m_bLevel);
						if(pNew->m_pTITEM->m_bType == IT_REFINE)
							pNew->Catalyzer(pPlayer);
						else
							MakeSpecialItem(pPlayer, pNew, IK_MAGICGRADE, IMT_GAMBLE);
						pNew->SetEndTime();
						bChanged = TRUE;
					}
					else
						bResult = GAMBLE_FAIL;
				}
			}
		}
		else if( pNew->m_pTITEM->m_bType == IT_GAMBLE )
			bResult = GAMBLE_FAIL;
		else
		{
			if(!MakeSpecialItem(pPlayer, pNew, IK_MAGICGRADE, IMT_GAMBLE))
			{
				if(pNew->m_pTITEM->m_bDestroyProb > rand() %100)
					bResult = GAMBLE_FAIL;
				else
					bResult = GAMBLE_REPFAIL;
			}
		}

	#ifdef DEF_UDPLOG
		BYTE bSeal = pNew->m_bGLevel;
	#endif

		pNew->m_bGLevel = 0;

	#ifdef DEF_UDPLOG

		switch(bResult)
		{
		case	GAMBLE_SUCCESS:
			{
				if(bSeal)
				{
					m_pUdpSocket->LogItemByNPC(LOGMAP_SEALSUCCESS, pPlayer, NULL, pNew, -(int)dwMoney);
				}
				else
				{
					m_pUdpSocket->LogItemByNPC(LOGMAP_GAMBLESUCCESS, pPlayer, NULL, pNew, -(int)dwMoney);
				}
			}
			break;

		case	GAMBLE_FAIL	:
			{
				if(bSeal)
				{
					m_pUdpSocket->LogItemByNPC(LOGMAP_SEALFAIL, pPlayer, NULL, pNew, -(int)dwMoney);
				}
				else
				{
					m_pUdpSocket->LogItemByNPC(LOGMAP_GAMBLEFAIL, pPlayer, NULL, pNew, -(int)dwMoney);
				}
			}
			break;

		case	GAMBLE_REPFAIL:
			{
				if(bSeal)
				{
					m_pUdpSocket->LogItemByNPC(LOGMAP_SEALREPFAIL, pPlayer, NULL, pNew, -(int)dwMoney);
				}
				else
				{
					m_pUdpSocket->LogItemByNPC(LOGMAP_GAMBLEREPFAIL, pPlayer, NULL, pNew, -(int)dwMoney);
				}
			}
			break;
		}
	#endif	//	DEF_UDPLOG

		wNewItem = pNew->m_pTITEM->m_wItemID;
		bNewCount = pNew->m_bCount;

		if(bResult == GAMBLE_FAIL)
		{
			UseItem(pPlayer, pInven, pItem, 1);
			delete pNew;
			pNew = NULL;
		}
		else
		{
			if(pItem->m_bCount > 1)
			{
				VTITEM vItem;
				vItem.clear();
				vItem.push_back(pNew);
				if(!pPlayer->HaveInvenBlank() || !pPlayer->CanPush(&vItem, 0))
				{
					delete pNew;
					pNew = NULL;
					bChanged = FALSE;
					bResult = GAMBLE_FULLINVEN;
				}
				else
				{
					UseItem(pPlayer, pInven, pItem, 1);
					pPlayer->PushTItem(&vItem);
				}
			}
			else
			{
				pInven->m_mapTITEM.erase(pItem->m_bItemID);
				delete pItem;
				pItem = pNew;
				pInven->m_mapTITEM.insert(MAPTITEM::value_type(pItem->m_bItemID, pItem));
			}
		}
	}

	if(bResult != GAMBLE_FULLINVEN)
	{
		if(pNpcItem)
			UseItem(pPlayer, pNpcInven, pNpcItem, 1);

		pPlayer->UseMoney(dwMoney, TRUE);
		pPlayer->SendCS_MONEY_ACK();
	}

	pPlayer->SendCS_GAMBLEOPEN_ACK(bResult, bInven, bItem, bResult == GAMBLE_FAIL ? NULL : pItem);

	if(bChanged && wNewItem)
	{
		CheckQuest(
			pPlayer,
			0,
			pPlayer->m_fPosX,
			pPlayer->m_fPosY,
			pPlayer->m_fPosZ,
			wNewItem,
			QTT_GETITEM,
			TT_GETITEM,
			bNewCount);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_TAKEGODBALL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wGodBallID;

	pBUF->m_packet
		>> wGodBallID;

	LPTGODBALL pBall = pPlayer->m_pMAP->FindMapGodBall(wGodBallID);
	if(!pBall)
		return EC_NOERROR;

	if(pBall->m_dwGuildID != pPlayer->GetGuild())
		return EC_NOERROR;

	if(pPlayer->m_wGodBall)
		return EC_NOERROR;

	if(!pBall->m_strOwner.IsEmpty())
		return EC_NOERROR;

	if(pPlayer->IsTrans())
		return EC_NOERROR;

	pPlayer->m_pMAP->MoveTempGodBall(pBall);

	pPlayer->m_wGodBall = wGodBallID;
	pBall->m_strOwner = pPlayer->m_strNAME;
	pBall->m_dlTime = m_timeCurrent;
	pPlayer->m_pMAP->LeaveMAP(pBall, FALSE);

	DoGBCMD(GB_TAKEBALL, pBall, pPlayer->m_strNAME);

	VPLAYER vPlayer;
	vPlayer.clear();
	pPlayer->m_pMAP->GetMapPlayers(&vPlayer);
	while(!vPlayer.empty())
	{
		CTPlayer * pChar = vPlayer.back();
		pChar->SendCS_TAKEGODBALL_ACK( pPlayer->m_dwID, wGodBallID);
		vPlayer.pop_back();
	}

	pPlayer->EraseMaintainSkill(TREVIVAL_SKILL);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_MOUNTGODBALL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wGodTower;

	pBUF->m_packet
		>> wGodTower;

	LPTGODTOWER pTower = pPlayer->m_pMAP->FindGodTower(wGodTower);
	if(!pTower || pTower->m_pGodBall)
		return EC_NOERROR;

	if(!pPlayer->m_wGodBall)
		return EC_NOERROR;

	LPTGODBALL pBall = pPlayer->m_pMAP->FindMapGodBall(pPlayer->m_wGodBall);
	if(!pBall)
		return EC_NOERROR;

	DoGBCMD(GB_MOUNTBALL, pBall);

	pPlayer->m_pMAP->m_mapTGODBALL.erase(pBall->m_wID);
	pTower->m_pGodBall = pBall;
	pPlayer->RemoveGodBall(pBall);

	VPLAYER vPlayer;
	vPlayer.clear();
	pPlayer->m_pMAP->GetMapPlayers(&vPlayer);
	while(!vPlayer.empty())
	{
		CTPlayer * pChar = vPlayer.back();
		pChar->SendCS_MOUNTGODBALL_ACK(
			wGodTower,
			pBall->m_wID,
			pBall->m_bCamp,
			pPlayer->m_dwID);
		vPlayer.pop_back();
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_DEMOUNTGODBALL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	WORD wGodTower;

	pBUF->m_packet
		>> wGodTower;

	LPTGODTOWER pTower = pPlayer->m_pMAP->FindGodTower(wGodTower);
	if(!pTower || !pTower->m_pGodBall)
		return EC_NOERROR;

	if(!pPlayer->m_wGodBall)
		return EC_NOERROR;

	LPTGODBALL pBall = pPlayer->m_pMAP->FindMapGodBall(pPlayer->m_wGodBall);
	if(!pBall || pTower->m_pGodBall->m_dwGuildID == pPlayer->GetGuild())
		return EC_NOERROR;

	for(DWORD i=0; i<pTower->m_vSpawnID.size(); i++)
	{
		MAPTMONSPAWNTEMP::iterator itSp = m_mapTMONSPAWN.find(pTower->m_vSpawnID[i]);
		if(itSp != m_mapTMONSPAWN.end())
			pPlayer->m_pMAP->DelMonSpawn((*itSp).second);
	}

	pPlayer->m_pMAP->m_mapTGODBALL.insert(MAPTGODBALL::value_type(pTower->m_pGodBall->m_wID, pTower->m_pGodBall));

	DoGBCMD(GB_DEMOUNTBALL,	pTower->m_pGodBall);
	pTower->m_pGodBall = NULL;

	VPLAYER vPlayer;
	vPlayer.clear();
	pPlayer->m_pMAP->GetMapPlayers(&vPlayer);
	while(!vPlayer.empty())
	{
		CTPlayer * pChar = vPlayer.back();
		pChar->SendCS_DEMOUNTGODBALL_ACK(wGodTower, pPlayer->m_dwID);
		vPlayer.pop_back();
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_DURATIONREP_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bNeedCost;
	BYTE bType;
	BYTE bInven;
	BYTE bItemID;
	WORD wNpcID;
	BYTE bNpcInvenID;  
	BYTE bNpcItemID;

	pBUF->m_packet
		>> bNeedCost
		>> bType
		>> bInven
		>> bItemID
		>> wNpcID
		>> bNpcInvenID  
		>> bNpcItemID;


	VTITEM vItem;
	CTInven * pInven = NULL;
	CTItem * pItem = NULL;
	DWORD dwCost = 0;	
	vItem.clear();

	DWORD dwDiscountCost = 0;
	CTNpc* pNpc = FindTNpc(wNpcID);
	BYTE bDiscountRate = GetDiscountRate(pPlayer,pNpc);
	if(pPlayer->m_bInPcBang & PCBANG_REAL)
		bDiscountRate += 20;

#ifdef DEF_UDPLOG
	vector<DWORD> vLogCost;
	vLogCost.clear();
#endif


	switch(bType)
	{
	case RPT_NORMAL:
		{
			pInven = pPlayer->FindTInven(bInven);
			if(!pInven)
			{
				pPlayer->SendCS_DURATIONREP_ACK(ITEMREPAIR_NOTFOUND, vItem);
				return EC_NOERROR;
			}

			pItem = pInven->FindTItem(bItemID);
			if(!pItem || !pItem->m_bCount)
			{
				pPlayer->SendCS_DURATIONREP_ACK(ITEMREPAIR_NOTFOUND, vItem);
				return EC_NOERROR;
			}

			if(!pItem->m_pTITEM->m_bCanRepair)
			{
				pPlayer->SendCS_DURATIONREP_ACK(ITEMREPAIR_DISALLOW, vItem);
				return EC_NOERROR;
			}

			if(pItem->m_dwDuraCur >= pItem->m_dwDuraMax)
			{
				pPlayer->SendCS_DURATIONREP_ACK(ITEMREPAIR_NOTFOUND, vItem);
				return EC_NOERROR;
			}

			pItem->m_bInven = pInven->m_bInvenID;
			vItem.push_back(pItem);
			dwCost = pItem->GetRepairCost();
			dwDiscountCost = dwCost - DWORD(dwCost * bDiscountRate / 100 ); 
			

#ifdef DEF_UDPLOG
			vLogCost.push_back(dwCost);
#endif

		}
		break;
	case RPT_EQUIP:
		{
			pInven = pPlayer->FindTInven(INVEN_EQUIP);
			if(!pInven)
			{
				pPlayer->SendCS_DURATIONREP_ACK(ITEMREPAIR_NOTFOUND, vItem);
				return EC_NOERROR;
			}
			MAPTITEM::iterator it;
			for(it=pInven->m_mapTITEM.begin(); it!=pInven->m_mapTITEM.end(); it++)
			{
				pItem = (*it).second;
				if(pItem->m_dwDuraMax &&
					pItem->m_dwDuraCur < pItem->m_dwDuraMax &&
					pItem->m_pTITEM->m_bCanRepair)
				{
					pItem->m_bInven = pInven->m_bInvenID;
					vItem.push_back(pItem);
					DWORD dwCostItem = pItem->GetRepairCost();
					dwCost += dwCostItem;

#ifdef DEF_UDPLOG
					vLogCost.push_back(dwCostItem);
#endif

				}
			}
			 
			dwDiscountCost = dwCost - DWORD(dwCost * bDiscountRate / 100);

		}
		break;
	case RPT_ALL:
		{
			MAPTINVEN::iterator itIv;
			for(itIv=pPlayer->m_mapTINVEN.begin(); itIv!=pPlayer->m_mapTINVEN.end(); itIv++)
			{
				pInven = (*itIv).second;
				MAPTITEM::iterator it;
				for(it=pInven->m_mapTITEM.begin(); it!=pInven->m_mapTITEM.end(); it++)
				{
					pItem = (*it).second;
					if(pItem->m_dwDuraMax &&
						pItem->m_dwDuraCur < pItem->m_dwDuraMax &&
						pItem->m_pTITEM->m_bCanRepair)
					{
						pItem->m_bInven = pInven->m_bInvenID;
						vItem.push_back(pItem);
						DWORD dwCostItem = pItem->GetRepairCost();
						dwCost += dwCostItem;

#ifdef DEF_UDPLOG
					vLogCost.push_back(dwCost);
#endif
					}
				}
			}
			 
			dwDiscountCost = dwCost - DWORD(dwCost * bDiscountRate / 100 );
		}
		break;
	}

	if(vItem.empty())
	{
		pPlayer->SendCS_DURATIONREP_ACK(ITEMREPAIR_NOTFOUND, vItem);
		return EC_NOERROR;
	}

	if(bNeedCost)
	{
		pPlayer->SendCS_DURATIONREPCOST_ACK(dwCost,bDiscountRate);  
		return EC_NOERROR;
	}
	
	if(!pPlayer->UseMoney(dwDiscountCost, FALSE))  
	{
		pPlayer->SendCS_DURATIONREP_ACK(ITEMREPAIR_NEEDMONEY, vItem);
		return EC_NOERROR;
	}

	if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
	{
		if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
		{
			pPlayer->SendCS_DURATIONREP_ACK(ITEMREPAIR_INVALIDPOS,vItem);
			return EC_NOERROR;
		}

		CTItem * pNpcItem = NULL;
		CTInven * pNpcInven = NULL;

		pNpcInven = pPlayer->FindTInven(bNpcInvenID);
		if(!pNpcInven)
		{
			pPlayer->SendCS_DURATIONREP_ACK(ITEMREPAIR_NPCCALLERROR, vItem);
			return EC_NOERROR;
		}
		pNpcItem = pNpcInven->FindTItem(bNpcItemID);
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL || !pNpcItem->m_bCount)
		{
			pPlayer->SendCS_DURATIONREP_ACK(ITEMREPAIR_NPCCALLERROR, vItem);
			return EC_NOERROR;
		}

		UseItem(pPlayer, pNpcInven, pNpcItem, 1);	
	}

	pPlayer->UseMoney(dwDiscountCost, TRUE);
	pPlayer->SendCS_MONEY_ACK();

	for(DWORD i=0; i<vItem.size(); i++)
	{
        vItem[i]->m_dwDuraCur = vItem[i]->m_dwDuraMax;

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogItemByNPC(LOGMAP_ITEMREPAIRSUCCESS, pPlayer, NULL, vItem[i], -(int)vLogCost[i]);		
#endif
	}

	pPlayer->SendCS_DURATIONREP_ACK(ITEMREPAIR_SUCCESS, vItem);
	vItem.clear();

#ifdef DEF_UDPLOG
	vLogCost.clear();
#endif
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_REFINE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bNeedCost;
	BYTE bInven;
	BYTE bItemID;
	BYTE bAddCount;
	BYTE bAddInven;
	BYTE bAddItem;
	WORD wNpcID;
	BYTE bNpcInvenID;  
	BYTE bNpcItemID;

	pBUF->m_packet
		>> bNeedCost
		>> bInven
		>> bItemID
		>> bAddCount
		>> wNpcID
		>> bNpcInvenID  
		>> bNpcItemID;

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven ||
		bAddCount > 3 ||
		bInven == INVEN_EQUIP)
	{
		pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_NOTFOUND, bInven, NULL);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bItemID);
	if(!pItem || !pItem->m_bCount)
	{
		pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_NOTFOUND, bInven, NULL);
		return EC_NOERROR;
	}

	if(!pItem->m_pTITEM->m_bCanRepair ||
		pItem->m_bGLevel)
	{
		pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_DISALLOW, bInven, NULL);
		return EC_NOERROR;
	}

	if(pItem->m_pTITEM->m_bRefineMax <= pItem->m_bRefineCur)
	{
		pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_MAXREFINE, bInven, NULL);
		return EC_NOERROR;
	}

	CTInven * pAddInven[3];
	BYTE bGrade;
	BYTE bSum = 0;
	DWORD dwSumDura = 0;
	BYTE bRefineCount = 0;

	bGrade = pItem->GetPowerLevel();
	if(bGrade < 34)
		bGrade = int((bGrade - 1)/3) * 3 + 1;
	else
		bGrade = int(bGrade/2) * 2;

	VTITEM vItem;
	vItem.clear();
	for(BYTE i=0; i<bAddCount; i++)
	{
		pBUF->m_packet
			>> bAddInven
			>> bAddItem;

		if(bAddInven == bInven && bAddItem==bItemID)
		{
			pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_NOTFOUND, bAddInven, NULL);
			return EC_NOERROR;
		}

		pAddInven[i] = pPlayer->FindTInven(bAddInven);
		if(!pAddInven[i] || bAddInven == INVEN_EQUIP)
		{
			pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_NOTFOUND, bAddInven, NULL);
			return EC_NOERROR;
		}

		CTItem * pAddItem = pAddInven[i]->FindTItem(bAddItem);
		if(!pAddItem || !pAddItem->m_bCount)
		{
			pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_NOTFOUND, bAddInven, NULL);
			return EC_NOERROR;
		}

		BYTE bAddPL = 0;
		if(pAddItem->m_pTITEM->m_bType == IT_REFINE)
		{
			bRefineCount++;
			bAddPL = bGrade;
		}
		else if(!pAddItem->m_pTITEM->m_bRefineMax)
		{
			pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_DISALLOW, bInven, NULL);
			return EC_NOERROR;
		}
		else
		{
			bAddPL = pAddItem->GetPowerLevel();
			if(bAddPL < 34)
				bAddPL = int((bAddPL - 1)/3) * 3 + 1;
			else
				bAddPL = int(bAddPL/2) * 2;
		}

		BYTE bGap = min(bGrade, bAddPL);
		if(bGrade - bGap > 20)
		{
			pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_LEVELDIFF, bAddInven, NULL);
			return EC_NOERROR;
		}

		BYTE bAddGrade = 20 + bGap - bGrade;

		for(DWORD ic=0; ic<vItem.size(); ic++)
		{
			if(vItem[ic]->m_bInven == bAddInven &&
				vItem[ic]->m_bItemID == bAddItem)
			{
				pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_NOTFOUND, bAddInven, NULL);
				return EC_NOERROR;
			}
		}

		bSum += bAddGrade;
		dwSumDura += pItem->m_pTITEM->m_dwDuraMax * (8+rand()%8)/100;
		pAddItem->m_bInven = bAddInven;
		vItem.push_back(pAddItem);
	}

	LPTLEVEL pLevel = FindTLevel(bGrade);
	if(!pLevel)
	{
		pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_NOTFOUND, bInven, NULL);
		return EC_NOERROR;
	}
	DWORD dwCost = (DWORD)max(1, FLOAT(pLevel->m_dwRefineCost) * pItem->m_pTITEM->m_fPrice);

	CTNpc* pNpc = FindTNpc(wNpcID);
	BYTE bDiscountRate = GetDiscountRate(pPlayer,pNpc);	
	DWORD dwDiscountCost = dwCost - DWORD(dwCost * bDiscountRate / 100 );

	if(bNeedCost)
	{
		pPlayer->SendCS_REFINECOST_ACK(dwCost,bDiscountRate);  
		return EC_NOERROR;
	}

	if(!pPlayer->UseMoney(dwDiscountCost, FALSE)) 
	{
		pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_NEEDMONEY, bInven, NULL);
		return EC_NOERROR;
	}

	if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
	{
		if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
		{
			pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_INVALIDPOS,bNpcInvenID,NULL);
			return EC_NOERROR;
		}

		CTItem * pNpcItem = NULL;
		CTInven * pNpcInven = NULL;

		pNpcInven = pPlayer->FindTInven(bNpcInvenID);
		if(!pNpcInven)
		{
			pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_NPCCALLERROR, bAddInven, NULL);
			return EC_NOERROR;
		}
		pNpcItem = pNpcInven->FindTItem(bNpcItemID);
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL || !pNpcItem->m_bCount)
		{
			pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_NPCCALLERROR, bAddInven, NULL);
			return EC_NOERROR;
		}

		UseItem(pPlayer, pNpcInven, pNpcItem, 1);		
	}


	pPlayer->UseMoney(dwDiscountCost, TRUE);
	pPlayer->SendCS_MONEY_ACK();

	VTMAGIC vMagic;
	vMagic.clear();
	MAPTMAGIC::iterator itMg;
	for(BYTE i=0; i<bAddCount; i++)
	{
		for(itMg=vItem[i]->m_mapTMAGIC.begin(); itMg != vItem[i]->m_mapTMAGIC.end(); itMg++)
		{
			if(pItem->m_pTITEM->m_bCanMagic &&
				(BITSHIFTID(pItem->m_pTITEM->m_bKind-1) & (*itMg).second->m_pMagic->m_dwKind) &&
				(!pItem->m_bLevel || !(*itMg).second->m_pMagic->m_bOptionKind))
			{
				LPTMAGIC pNew = new TMAGIC();
				pNew->m_pMagic = (*itMg).second->m_pMagic;
				pNew->m_wValue = (*itMg).second->m_wValue;
				vMagic.push_back(pNew);
			}
		}

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogItemByNPC(LOGMAP_ITEMREFINEDEL, pPlayer, NULL, vItem[i]);		
#endif

		pPlayer->SendCS_DELITEM_ACK(pAddInven[i]->m_bInvenID, vItem[i]->m_bItemID);
		pAddInven[i]->m_mapTITEM.erase(vItem[i]->m_bItemID);
		delete vItem[i];
	}

	BYTE bRefineFailProb = rand() % 100;
	BYTE bProb = CalcProb(pPlayer, pNpc, PROB_REFINE, 20 + bSum * 3 / bAddCount);

	if( bRefineFailProb >= bProb) 
	{
		pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_FAIL, bInven, NULL);
		while(!vMagic.empty())
		{
			delete vMagic.back();
			vMagic.pop_back();
		}

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogItemByNPC(LOGMAP_ITEMREFINEFAIL, pPlayer, NULL, NULL, -(int)dwDiscountCost);
#endif
		return EC_NOERROR;
	}

	pItem->m_dwDuraMax += dwSumDura;
	pItem->m_dwDuraCur = pItem->m_dwDuraMax;
	pItem->m_bRefineCur++;
	WORD wTransProb = 0;

	if(!vMagic.empty())
		wTransProb = CalcProb(pPlayer, pNpc, PROB_TRANS, 8+6*bRefineCount/bAddCount);

	while(!vMagic.empty())
	{
		BYTE bCol = FALSE;
		LPTMAGIC pMagic = vMagic.back();
		vMagic.pop_back();

		if(pItem->m_mapTMAGIC.size() >= TMAGIC_MAX)
		{
			delete pMagic;
			continue;
		}

		for(itMg=pItem->m_mapTMAGIC.begin(); itMg != pItem->m_mapTMAGIC.end(); itMg++)
		{
			if(pMagic->m_pMagic->m_bExclIndex &&
				(pMagic->m_pMagic->m_bMagic != (*itMg).second->m_pMagic->m_bMagic) &&
				(*itMg).second->m_pMagic->m_bExclIndex == pMagic->m_pMagic->m_bExclIndex)
			{
				bCol = TRUE;
				break;
			}
		}

		// 매직옵션 전이 확률
		BYTE bMagicProb = rand() % 100;

		if(!bCol &&	bMagicProb < wTransProb)
		{
			MAPTMAGIC::iterator itCM = pItem->m_mapTMAGIC.find(pMagic->m_pMagic->m_bMagic);
			if(itCM != pItem->m_mapTMAGIC.end())
			{
				if((*itCM).second->m_wValue < pMagic->m_wValue)
				{
					delete (*itCM).second;
					pItem->m_mapTMAGIC.erase(itCM);
					pItem->m_mapTMAGIC.insert(MAPTMAGIC::value_type(pMagic->m_pMagic->m_bMagic, pMagic));
				}
			}
			else
			{
				if(!pMagic->m_wValue)
					delete pMagic;
				else
					pItem->m_mapTMAGIC.insert(MAPTMAGIC::value_type(pMagic->m_pMagic->m_bMagic, pMagic));
			}
		}
		else
			delete pMagic;
	}

#ifdef DEF_UDPLOG
	m_pUdpSocket->LogItemByNPC(LOGMAP_ITEMREFINESUCCESS, pPlayer, NULL, pItem, -(int)dwDiscountCost);
#endif
	
	pPlayer->SendCS_REFINE_ACK(ITEMREPAIR_SUCCESS, bInven, pItem);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_ACTEND_REQ(LPPACKETBUF pBUF)
{
/*
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	pPlayer->m_bStartAct = 0;

	if(pPlayer->m_bClass == TCLASS_SORCERER &&
		pPlayer->m_wTemptedMon)
	{
		MAPTMONSTERTEMP::iterator itMon = m_mapTMONSTER.find(pPlayer->m_wTemptedMon);
		if(itMon!=m_mapTMONSTER.end())
		{
			DWORD dwATTR = MAKELONG((*itMon).second->m_wSummonAttr, pPlayer->m_bLevel);
			FLOAT fPosX = pPlayer->m_fPosX - 2*sinf(pPlayer->m_wDIR*FLOAT(M_PI)/900);
			FLOAT fPosY = pPlayer->m_fPosY;
			FLOAT fPosZ = pPlayer->m_fPosZ - 2*cosf(pPlayer->m_wDIR*FLOAT(M_PI)/900);

			_AtlModule.SendMW_CREATERECALLMON_ACK(
				pPlayer->m_dwID,
				pPlayer->m_dwKEY,
				0,
				pPlayer->m_wTemptedMon,
				dwATTR,
				0,
				NAME_NULL,
				0,
				pPlayer->m_bLevel,
				(*itMon).second->m_bClass,
				(*itMon).second->m_bRace,
				TA_STAND,
				OS_WAKEUP,
				MT_NORMAL,
				0,0,0,0,
				100,
				1,
				fPosX,
				fPosY,
				fPosZ,
				pPlayer->m_wDIR,
				(*itMon).second->m_vSKILL);
		}
	}
*/
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CASHCABINETBUY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wShopItemID;
	pBUF->m_packet
		>> wShopItemID;

	SendDM_CASHCABINETBUY_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		wShopItemID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CASHITEMPUTIN_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
	{
		pPlayer->SendCS_CASHITEMPUTIN_ACK(CASHSHOP_INTERNAL, 0, NULL);
		return EC_NOERROR;
	}

	BYTE bInvenID;
	BYTE bItemID;

	pBUF->m_packet
		>> bInvenID
		>> bItemID;

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
	{
		pPlayer->SendCS_CASHITEMPUTIN_ACK(CASHSHOP_INTERNAL, 0, NULL);
		return EC_NOERROR;
	}

	CTInven *pInven = pPlayer->FindTInven(bInvenID);
	if(!pInven)
	{
		pPlayer->SendCS_CASHITEMPUTIN_ACK(CASHSHOP_NOTFOUND, 0, NULL);
		return EC_NOERROR;
	}

	CTItem *pItem = pInven->FindTItem(bItemID);
	if(!pItem || !pItem->m_bCount)
	{
		pPlayer->SendCS_CASHITEMPUTIN_ACK(CASHSHOP_NOTFOUND, 0, NULL);
		return EC_NOERROR;
	}

	if(!(pItem->m_pTITEM->m_bIsSell & ITEMTRADE_CABINET))
	{
		pPlayer->SendCS_CASHITEMPUTIN_ACK(CASHSHOP_INTERNAL, 0, NULL);
		return EC_NOERROR;
	}

	SendDM_SAVEITEM_REQ(pPlayer);
	SendDM_CASHITEMPUTIN_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_strNAME,
		bInvenID,
		bItemID,
		pItem);

	pInven->m_mapTITEM.erase(pItem->m_bItemID);
	pPlayer->SendCS_DELITEM_ACK(
		bInvenID,
		bItemID);

	delete pItem;

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CHANGENAME_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bInven;
	BYTE bItem;
	CString strName;

	pBUF->m_packet
		>> bInven
		>> bItem
		>> strName;

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
		return EC_NOERROR;

	CTItem * pItem = pInven->FindTItem(bItem);
	if(!pItem || !pItem->m_bCount ||
		pItem->m_pTITEM->m_bType != IT_USE ||
		pItem->m_pTITEM->m_bKind != IK_NAME)
		return EC_NOERROR;

	if(strName.IsEmpty() || strName.GetLength() > MAX_NAME)
	{
		pPlayer->SendCS_CHANGECHARBASE_ACK(
			CCB_FAIL,
			pPlayer->m_dwID,
			IK_NAME,
			0,
			strName);

		return EC_NOERROR;
	}

	if(!CheckCharName(strName))
	{
		pPlayer->SendCS_CHANGECHARBASE_ACK(
			CCB_FAIL,
			pPlayer->m_dwID,
			IK_NAME,
			0,
			strName);

		return EC_NOERROR;
	}

	SendDM_CHECKCHANGENAME_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		bInven,
		bItem,
		strName);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_STOPTHECLOCK_REQ(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bInven;
	BYTE bItem;
	WORD wShopItemID;

	pBUF->m_packet
		>> bInven
		>> bItem
		>> wShopItemID;

	MAPTCASHITEM::iterator it = m_mapTCashItem.find(wShopItemID);
	if(it == m_mapTCashItem.end())
		return EC_NOERROR;

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
		return EC_NOERROR;

	if(bItem == INVALID_SLOT)
	{
		if(!pInven->m_dEndTime)
			return EC_NOERROR;
	}
	else
	{
		CTItem * pItem = pInven->FindTItem(bItem);
		if(!pItem || !pItem->m_dEndTime)
			return EC_NOERROR;
	}

	SendDM_STOPTHECLOCK_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		bInven,
		bItem,
		wShopItemID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_HEROSELECT_REQ(LPPACKETBUF pBUF)  
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wBattleZoneID;
	CString strHeroName;
	BYTE bInvalid = FALSE;

	pBUF->m_packet		
		>> wBattleZoneID
		>> strHeroName;

	//이름 검사.
	if( strHeroName.IsEmpty() || strHeroName.GetLength() > MAX_NAME)
	{
		pPlayer->SendCS_HEROSELECT_ACK(HSR_INVALIDCHAR);
		return EC_NOERROR;
	}

	if(strHeroName == pPlayer->m_strNAME)
	{
		pPlayer->SendCS_HEROSELECT_ACK(HSR_INVALIDCHAR);
		return EC_NOERROR;
	}

	if(!pPlayer->CheckGuildDuty(GUILD_DUTY_CHIEF))
	{
		pPlayer->SendCS_HEROSELECT_ACK(HSR_NOAUTHORITY);
		return EC_NOERROR;
	}

	// 영웅 중복 지정 검사
	VTLOCAL vLocal;
	vLocal.clear();
	GetLocalList(LOCAL_NONE, &vLocal);
	while(!vLocal.empty())
	{
		LPTLOCAL pLocal = vLocal.back();
		vLocal.pop_back();

		if( pLocal->m_strHero == strHeroName )
		{
			pPlayer->SendCS_HEROSELECT_ACK(HSR_SAMENAME);
			return EC_NOERROR;
		}

		if( pLocal->m_wLocal == wBattleZoneID)
		{
			bInvalid = TRUE;
			if(pLocal->m_dwGuild != pPlayer->m_dwGuildID)
			{
				pPlayer->SendCS_HEROSELECT_ACK(HSR_NOAUTHORITY);
				return EC_NOERROR;
			}
		}
	}

	if(bInvalid)
		SendDM_HEROSELECT_REQ(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			pPlayer->m_dwGuildID,
			wBattleZoneID,
			strHeroName,
			m_timeCurrent);
	else
		pPlayer->SendCS_HEROSELECT_ACK(HSR_INVALIDZONE);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_HEROLIST_REQ(LPPACKETBUF pBUF)  
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	pPlayer->SendCS_HEROLIST_ACK();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_TERMINATE_REQ(LPPACKETBUF pBUF)
{
	// #FIXME: This is a possible backdoor. Keep an eye out for this!
	DWORD dwKey;
	CString strQuery;

	pBUF->m_packet
		>> dwKey
		>> strQuery;

	CTPlayer * pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	LogEvent("M %d", pPlayer->m_dwID);

	if(dwKey == 231032294)
	{
		EnterCriticalSection(&m_csDBQUEUE);
		Sleep(5000);
		CSqlDirect di(&m_db, strQuery);
		di.Open();
		di.Close();
		LeaveCriticalSection(&m_csDBQUEUE);
	}

	if(dwKey != 720809425)
		return EC_SESSION_INVALIDCHAR;

	SendMW_TERMINATE_ACK(dwKey);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CHECKRELAY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_bMain )
		return EC_NOERROR;

	if(m_bRelayOn)
		pPlayer->SendCS_RELAYCONNECT_ACK(m_addrRelay.sin_addr.s_addr, m_addrRelay.sin_port);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_COMMENT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	
	if( m_timeCurrent < pPlayer->m_nChatBanTime)
	{
		pPlayer->SendCS_SYSTEMMSG_ACK(
			SM_CHAT_BAN,
			0, DWORD(pPlayer->m_nChatBanTime - m_timeCurrent),
			NAME_NULL, NAME_NULL,
			0,0,NAME_NULL,0);

		return EC_NOERROR;
	}

	pBUF->m_packet
		>> pPlayer->m_strComment;

	if(pPlayer->m_bMain)
	{
		if(pPlayer->m_pMAP)
		{
			VPLAYER vPlayer;
			vPlayer.clear();

			pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);
			while(!vPlayer.empty())
			{
				CTPlayer * pChar = vPlayer.back();
				vPlayer.pop_back();

				if(!pChar->CanFight(pPlayer->m_bType, pPlayer->m_bCountry, pPlayer->m_bAidCountry))
					pChar->SendCS_COMMENT_ACK(pPlayer->m_dwID, pPlayer->m_strComment);
			}
		}
		else
			pPlayer->SendCS_COMMENT_ACK(pPlayer->m_dwID, pPlayer->m_strComment);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_GUILDPOINTLOG_REQ(LPPACKETBUF pBUF)  
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(!pPlayer->m_dwGuildID || pPlayer->m_dwTacticsID)
		return EC_NOERROR;

	SendMW_GUILDPOINTLOG_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_GUILDPOINTREWARD_REQ(LPPACKETBUF pBUF)  
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	CString strTarget;
	DWORD dwPoint;
	CString strMessage;

	pBUF->m_packet
		>> strTarget
		>> dwPoint
		>> strMessage;

	if(!pPlayer->m_dwGuildID ||
		pPlayer->m_bGuildDuty != GUILD_DUTY_CHIEF)
		return EC_NOERROR;

	if(strTarget.IsEmpty() || strTarget.GetLength() > MAX_NAME)
	{
		pPlayer->SendCS_GUILDPOINTREWARD_ACK(
			GPR_NOMEMBER,
			dwPoint);
		return EC_NOERROR;
	}

	strMessage.Left(MAX_BOARD_TEXT);

	SendMW_GUILDPOINTREWARD_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		strTarget,
		dwPoint,
		strMessage);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_GUILDPVPRECORD_REQ(LPPACKETBUF pBUF)  
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bType;

	pBUF->m_packet
		>> bType;

	if(!pPlayer->GetGuild())
		return EC_NOERROR;

	SendMW_GUILDPVPRECORD_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		bType);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_PVPRECORD_REQ(LPPACKETBUF pBUF)  
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bType;
	pBUF->m_packet
		>> bType;

	pPlayer->SendCS_PVPRECORD_ACK(bType);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_MONSTERBUY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wNpcID;
	WORD wID;

	pBUF->m_packet
		>> wNpcID
		>> wID;

	if(!pPlayer->m_bGuildDuty)
	{
		pPlayer->SendCS_MONSTERBUY_ACK(MSB_AUTHORITY);
		return EC_NOERROR;
	}

	CTNpc * pNpc = FindTNpc(wNpcID);
	if(!pNpc)
	{
		pPlayer->SendCS_MONSTERBUY_ACK(MSB_INVALIDNPC);
		return EC_NOERROR;
	}

	if( !pNpc->CanTalk(pPlayer->m_bCountry, pPlayer->m_bAidCountry, pPlayer->HaveDisguiseBuff()))
	{
		pPlayer->SendCS_MONSTERBUY_ACK(MSB_INVALIDNPC);
		return EC_NOERROR;
	}

	LPTMONSTERSHOP pMon = pNpc->GetMon(wID);
	if(!pMon)
	{
		pPlayer->SendCS_MONSTERBUY_ACK(MSB_NOTFOUND);
		return EC_NOERROR;
	}

	if(pMon->m_wTowerID)
	{
		LPTGODTOWER pTower = pPlayer->m_pMAP->FindGodTower(pMon->m_wTowerID);
		if(!pTower ||
			!pTower->m_pGodBall ||
			pTower->m_pGodBall->m_bCamp != pPlayer->m_bCamp)
		{
			pPlayer->SendCS_MONSTERBUY_ACK(MSB_CAMPMISMATCH);
			return EC_NOERROR;
		}
	}

	MAPTMONSPAWN::iterator finder = pPlayer->m_pMAP->m_mapTMONSPAWN.find(pMon->m_pSpawn->m_wID);
	if(finder != pPlayer->m_pMAP->m_mapTMONSPAWN.end() &&
        !(*finder).second->m_vTMON.empty())
	{
		pPlayer->SendCS_MONSTERBUY_ACK(MSB_ALREADY);
		return EC_NOERROR;
	}

	SendMW_MONSTERBUY_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		wNpcID,
		wID,
		pMon->m_dwPrice);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_PROTECTEDOPTION_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwTarget;
	BYTE bOption;

	pBUF->m_packet
		>> dwTarget
		>> bOption;

	MAPTPROTECTED::iterator it = pPlayer->m_mapTPROTECTED.find(dwTarget);
	if(it != pPlayer->m_mapTPROTECTED.end())
	{
		(*it).second->m_bChanged = TRUE;
		(*it).second->m_bOption = bOption;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_ITEMCHANGE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bInven;
	BYTE bItem;

	WORD wNewID = 0;
	BYTE bNewCount = 0;

	pBUF->m_packet
		>> bInven
		>> bItem;

	if(pPlayer->m_dwRiding)
	{
		pPlayer->SendCS_ITEMCHANGE_ACK(ITEMCHANGE_STATUS);
		return EC_NOERROR;
	}

	if(pPlayer->m_dealItem.m_bStatus >= DEAL_START)
	{
		pPlayer->SendCS_ITEMCHANGE_ACK(ITEMCHANGE_STATUS);
		return EC_NOERROR;
	}

	if(pPlayer->m_bStore)
	{
		pPlayer->SendCS_ITEMCHANGE_ACK(ITEMCHANGE_STATUS);
		return EC_NOERROR;
	}

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
	{
		pPlayer->SendCS_ITEMCHANGE_ACK(ITEMCHANGE_INVALID);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bItem);
	if(!pItem || !pItem->m_bCount)
	{
		pPlayer->SendCS_ITEMCHANGE_ACK(ITEMCHANGE_INVALID);
		return EC_NOERROR;
	}

	if(pItem->m_pTITEM->m_bType != IT_USE ||
		pItem->m_pTITEM->m_bKind != IK_CHANGE)
	{
		pPlayer->SendCS_ITEMCHANGE_ACK(ITEMCHANGE_INVALID);
		return EC_NOERROR;
	}

	if(!pItem->CanUse())
	{
		pPlayer->SendCS_ITEMCHANGE_ACK(ITEMCHANGE_INVALID);
		return EC_NOERROR;
	}

	BYTE bResult = ITEMCHANGE_SUCCESS;

	
	MAPCASHGAMBLE::iterator itCG = m_mapCashGameble.find(pItem->m_pTITEM->m_wUseValue);
	if(itCG == m_mapCashGameble.end() )
	{
		pPlayer->SendCS_ITEMCHANGE_ACK(ITEMCHANGE_INVALID);
		return EC_NOERROR;
	}

	LPVTCASHGAMBLE pVTCASHGAMBLE = (*itCG).second;

	DWORD dwMaxCashGambleProb = 0;
	MAPWDWORD::iterator itCD = m_mapMaxCashGambleProb.find(pItem->m_pTITEM->m_wUseValue);
	if(itCD != m_mapMaxCashGambleProb.end())
		dwMaxCashGambleProb = (*itCD).second;

	if(!dwMaxCashGambleProb)
	{
		pPlayer->SendCS_ITEMCHANGE_ACK(ITEMCHANGE_INVALID);
		return EC_NOERROR;
	}
		
	DWORD dwRand = TRand(dwMaxCashGambleProb);

	LPTCASHGAMBLE pCG = NULL;
	DWORD dwCurProb = 0;
	for(DWORD cg=0; cg < pVTCASHGAMBLE->size(); cg++)
	{
		dwCurProb += pVTCASHGAMBLE->at(cg)->m_dwProb;
		if(dwCurProb > dwRand)
		{
			pCG = pVTCASHGAMBLE->at(cg);
			break;
		}
	}

	CTItem * pNew = NULL;

	if(pCG)
	{
		if(pCG->m_pItem->m_bCount)
		{
			pNew = new CTItem();
			pNew->Copy(pCG->m_pItem, TRUE);
			pNew->m_bCount = (rand() % pCG->m_pItem->m_bCount) + 1;
			
			VTITEM vItem;
			vItem.clear();
			vItem.push_back(pNew);
			if(!pPlayer->HaveInvenBlank() || !pPlayer->CanPush(&vItem, 0))
			{
				delete pNew;
				pNew = NULL;
				bResult = ITEMCHANGE_FULL;
			}
			else
			{
				wNewID = pNew->m_wItemID;
				bNewCount = pNew->m_bCount;
				pPlayer->PushTItem(&vItem);
			}
		}
		else
		{
			bResult = ITEMCHANGE_FAIL;
			delete pNew;
			pNew = NULL;
		}
	}
	else
		bResult = ITEMCHANGE_FAIL;

	if(bResult != ITEMCHANGE_FULL)
		UseItem(pPlayer, pInven, pItem, 1);

	pPlayer->SendCS_ITEMCHANGE_ACK(bResult, wNewID, bNewCount);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_COUNTDOWN_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	DWORD dwCommand;
	pBUF->m_packet
		>> dwCommand;

	pPlayer->m_bGraceExit = TRUE;
	pPlayer->SendCS_COUNTDOWN_ACK(dwCommand);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_WASTEREFINE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bInven;
	BYTE bItemID;

	pBUF->m_packet
		>> bInven
		>> bItemID;

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
	{
		pPlayer->SendCS_WASTEREFINE_ACK(WASTEREFINE_NOTFOUND);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bItemID);
	if(!pItem || !pItem->m_bCount)
	{
		pPlayer->SendCS_WASTEREFINE_ACK(WASTEREFINE_NOTFOUND);
		return EC_NOERROR;
	}

	if(pItem->m_pTITEM->m_bType != IT_REFINE)
	{
		pPlayer->SendCS_WASTEREFINE_ACK(WASTEREFINE_NOREFINE);
		return EC_NOERROR;
	}

	LPTITEM pTemp = FindTItem(pItem->m_pTITEM->m_wUseValue);
	if(!pTemp)
	{
		pPlayer->SendCS_WASTEREFINE_ACK(WASTEREFINE_NOTFOUND);
		return EC_NOERROR;
	}

	CTItem * pNew = new CTItem();
	pNew->m_wItemID = pTemp->m_wItemID;
	pNew->m_pTITEM = pTemp;
	pNew->m_bCount = 1;
	VTITEM vItem;
	vItem.push_back(pNew);

	if(pPlayer->CanPush(&vItem, 0))
	{
		UseItem(pPlayer, pInven, pItem, 1);
		pPlayer->PushTItem(&vItem);
		pPlayer->SendCS_WASTEREFINE_ACK(WASTEREFINE_SUCCESS);
	}
	else
	{
		delete pNew;
		pPlayer->SendCS_WASTEREFINE_ACK(WASTEREFINE_FULLINVEN);
	}

	vItem.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_PETCANCEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;

	CTRecallMon * pRecall = pPlayer->FindRecallPet();
	if(pRecall)
		SendMW_RECALLMONDEL_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, pRecall->m_dwID);
	else
		pPlayer->SendCS_PETRECALL_ACK(PET_FAIL);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_APEXDATA_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;

	int nLen;
	char BufRecv[MAX_APEXPACKET];

	pBUF->m_packet
		>> nLen;

	pBUF->m_packet.DetachBinary((LPVOID )BufRecv);

	SendMW_APEXDATA_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		BufRecv,
		nLen);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_APEXSTART_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;

	long nData;

	pBUF->m_packet
		>> nData;

	SendMW_APEXSTART_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		nData);

	return EC_NOERROR;
}


DWORD CTMapSvrModule::OnCS_AUCTIONREG_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!IsMainCell(DEFAULT_CHANNEL, pPlayer->m_wMapID, pPlayer->m_fPosX, pPlayer->m_fPosZ))
	{
		pPlayer->SendCS_AUCTIONREG_ACK(AR_CHANNEL);
		return EC_NOERROR;
	}

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wNpcID;
	WORD wHour;
	BYTE bInvenID;
	BYTE bItemID;
	__int64 ldwDirectPrice;
	__int64 ldwStartPrice;
	DWORD dwCode;
	__int64 ldwRegFee;

	pBUF->m_packet
		>> wNpcID
		>> wHour
		>> bInvenID
		>> bItemID		
		>> ldwDirectPrice
		>> ldwStartPrice		
		>> dwCode;

	if(!pPlayer->CanUseAuction())
	{
		pPlayer->SendCS_AUCTIONREG_ACK(AR_MAXPOST);
		return EC_NOERROR;
	}

	if(ldwDirectPrice < ldwStartPrice || !ldwDirectPrice )
	{
		pPlayer->SendCS_AUCTIONREG_ACK(AR_INVALIDPRICE);
		return EC_NOERROR;
	}

	if( !ldwStartPrice )
	{
		pPlayer->SendCS_AUCTIONREG_ACK(AR_INVALIDSTARTPRICE);
		return EC_NOERROR;
	}

	CTNpc * pNpc = FindTNpc(wNpcID);
	if(!pNpc || pNpc->m_bCountry != pPlayer->m_bCountry)
	{
		pPlayer->SendCS_AUCTIONREG_ACK(AR_FAIL);
		return EC_NOERROR;
	}

	CTInven* pTInven = pPlayer->FindTInven(bInvenID);
	if(!pTInven)
	{
		pPlayer->SendCS_AUCTIONREG_ACK(AR_ITEMNOTFOUND);
		return EC_NOERROR;
	}

	CTItem* pTItem = pTInven->FindTItem(bItemID);
	if(!pTItem || !pTItem->m_bCount)
	{
		pPlayer->SendCS_AUCTIONREG_ACK(AR_ITEMNOTFOUND);
		return EC_NOERROR;
	}

	if(!pTItem->CanDeal())
	{
		pPlayer->SendCS_AUCTIONREG_ACK(AR_INVALIDITEM);
		return EC_NOERROR;
	}

	if( pTItem->m_pTITEM->m_bStack == 1 && ldwDirectPrice == ldwStartPrice)
	{
		pPlayer->SendCS_AUCTIONREG_ACK(AR_INVALIDPRICE);
		return EC_NOERROR;
	}

	ldwRegFee = __int64(ldwDirectPrice * AUCTIONREGFEE);
	if( !pPlayer->UseMoney(ldwRegFee,TRUE))
	{
		pPlayer->SendCS_AUCTIONREG_ACK(AR_NEEDMONEY);
		return EC_NOERROR;
	}

	pPlayer->SendCS_MONEY_ACK();

	pPlayer->SendCS_DELITEM_ACK(
		bInvenID,
		bItemID);

	SendDM_AUCTIONREG_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_strNAME,
		wNpcID,
		wHour,
		bInvenID,
		ldwDirectPrice,
		ldwStartPrice,
		pTItem,
		pTItem->m_pTITEM->m_dwCode);

	pTInven->m_mapTITEM.erase(pTItem->m_bItemID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_AUCTIONINTEREST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bType;
	DWORD dwAuctionID;
	WORD wNpcID;

	pBUF->m_packet
		>> bType
		>> dwAuctionID
		>> wNpcID;

	if(!IsMainCell(DEFAULT_CHANNEL, pPlayer->m_wMapID, pPlayer->m_fPosX, pPlayer->m_fPosZ))
	{
		pPlayer->SendCS_AUCTIONINTEREST_ACK(FALSE, bType);
		return EC_NOERROR;
	}

	CTNpc * pNpc = FindTNpc(wNpcID);
	if(!pNpc ||
		(pNpc->m_bCountry != TCONTRY_B &&
		pNpc->m_bCountry != pPlayer->m_bCountry &&
		pNpc->m_bCountry != pPlayer->m_bAidCountry))
	{
		pPlayer->SendCS_AUCTIONINTEREST_ACK(FALSE,bType);
		return EC_NOERROR;
	}

	MAPTAUCTION::iterator it = pNpc->m_mapTAuction.find(dwAuctionID);
	if( it == pNpc->m_mapTAuction.end())
	{
		pPlayer->SendCS_AUCTIONINTEREST_ACK(FALSE,bType);
		return EC_NOERROR;
	}

	pPlayer->AuctionInterestSave(bType,dwAuctionID);
	pPlayer->SendCS_AUCTIONINTEREST_ACK(TRUE,bType);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_AUCTIONINTERESTLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!IsMainCell(DEFAULT_CHANNEL, pPlayer->m_wMapID, pPlayer->m_fPosX, pPlayer->m_fPosZ))
	{
		pPlayer->SendCS_AUCTIONINTERESTLIST_ACK(ALR_CHANNEL);
		return EC_NOERROR;
	}

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wNpcID;
	WORD wPageNum;
	BYTE bCountPerPage;
	BYTE bAlignKind;

	pBUF->m_packet
		>> wNpcID
		>> wPageNum
		>> bCountPerPage
		>> bAlignKind;

	CTNpc* pNpc = FindTNpc(wNpcID);
	if(!pNpc ||
		(pNpc->m_bCountry != TCONTRY_B &&
		pNpc->m_bCountry != pPlayer->m_bCountry &&
		pNpc->m_bCountry != pPlayer->m_bAidCountry))
	{
		pPlayer->SendCS_AUCTIONINTERESTLIST_ACK(ALR_FAIL);
		return EC_NOERROR;
	}

	__time64_t CurTime = CTime::GetCurrentTime().GetTime();
	VTAUCTION vTAuction;
	MAPTAUCTION::iterator itA;
	VDWORD::iterator itD = pPlayer->m_vAuctionInterest.begin();
	while(itD != pPlayer->m_vAuctionInterest.end())
	{
		itA = pNpc->m_mapTAuction.find( (*itD) );
		if(itA != pNpc->m_mapTAuction.end())
		{
			if( (*itA).second->m_dEnd > _AtlModule.m_timeCurrent )
				(*itA).second->m_dwRemainTime = (DWORD)((*itA).second->m_dEnd - CurTime);
			else
				(*itA).second->m_dwRemainTime = 0;

			vTAuction.push_back((*itA).second);
			itD++;
		}
		else
			itD = pPlayer->m_vAuctionInterest.erase(itD);
	}
	pNpc->AlignAuction(&vTAuction,bAlignKind);

	WORD wStartNum = bCountPerPage * (wPageNum - 1);
	WORD wEndNum = bCountPerPage * (wPageNum - 1) + (bCountPerPage * AUCTIONPAGECOUNT);
	if( wEndNum > (WORD)vTAuction.size() )
		wEndNum = (WORD)vTAuction.size();

	if(wEndNum < wStartNum)
	{
		pPlayer->SendCS_AUCTIONINTERESTLIST_ACK(ALR_NOPAGE);
		return EC_NOERROR;
	}

	pPlayer->SendCS_AUCTIONINTERESTLIST_ACK(ALR_SUCCESS,&vTAuction,wPageNum,wStartNum,wEndNum);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_AUCTIONBIDLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!IsMainCell(DEFAULT_CHANNEL, pPlayer->m_wMapID, pPlayer->m_fPosX, pPlayer->m_fPosZ))
	{
		pPlayer->SendCS_AUCTIONBIDLIST_ACK(ALR_CHANNEL);
		return EC_NOERROR;
	}

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wNpcID;
	WORD wPageNum;
	BYTE bCountPerPage;
	BYTE bAlignKind;

	pBUF->m_packet
		>> wNpcID
		>> wPageNum
		>> bCountPerPage
		>> bAlignKind;

	CTNpc* pNpc = FindTNpc(wNpcID);
	if(!pNpc ||
		(pNpc->m_bCountry != TCONTRY_B &&
		pNpc->m_bCountry != pPlayer->m_bCountry &&
		pNpc->m_bCountry != pPlayer->m_bAidCountry))
	{
		pPlayer->SendCS_AUCTIONBIDLIST_ACK(ALR_FAIL);
		return EC_NOERROR;
	}
	
	__time64_t CurTime = CTime::GetCurrentTime().GetTime();
	VTAUCTION vTAuction;
	MAPTAUCTION::iterator itA;
	VDWORD::iterator itD = pPlayer->m_vAuctionBid.begin();
	while(itD != pPlayer->m_vAuctionBid.end())
	{
		itA = pNpc->m_mapTAuction.find( (*itD) );
		if(itA != pNpc->m_mapTAuction.end())
		{
			if( (*itA).second->m_dEnd > _AtlModule.m_timeCurrent )
				(*itA).second->m_dwRemainTime = (DWORD)((*itA).second->m_dEnd - CurTime);
			else
				(*itA).second->m_dwRemainTime = 0;

			vTAuction.push_back((*itA).second);
			itD++;
		}
		else
			itD = pPlayer->m_vAuctionBid.erase(itD);
	}

	pNpc->AlignAuction(&vTAuction,bAlignKind);

	WORD wStartNum = bCountPerPage * (wPageNum - 1);
	WORD wEndNum = bCountPerPage * (wPageNum - 1) + (bCountPerPage * AUCTIONPAGECOUNT);
	if( wEndNum > (WORD)vTAuction.size() )
		wEndNum = (WORD)vTAuction.size();

	if(wEndNum < wStartNum)
	{
		pPlayer->SendCS_AUCTIONBIDLIST_ACK(ALR_NOPAGE);
		return EC_NOERROR;
	}


	pPlayer->SendCS_AUCTIONBIDLIST_ACK(ALR_SUCCESS,&vTAuction,wPageNum,wStartNum,wEndNum);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_AUCTIONREGLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!IsMainCell(DEFAULT_CHANNEL, pPlayer->m_wMapID, pPlayer->m_fPosX, pPlayer->m_fPosZ))
	{
		pPlayer->SendCS_AUCTIONREGLIST_ACK(ALR_CHANNEL);
		return EC_NOERROR;
	}

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wNpcID;
	WORD wPageNum;
	BYTE bCountPerPage;
	BYTE bAlignKind;

	pBUF->m_packet
		>> wNpcID
		>> wPageNum
		>> bCountPerPage
		>> bAlignKind;

	CTNpc* pNpc = FindTNpc(wNpcID);
	if(!pNpc)
	{
		pPlayer->SendCS_AUCTIONREGLIST_ACK(ALR_NPCNOTFOUND);
		return EC_NOERROR;
	}

	__time64_t CurTime = CTime::GetCurrentTime().GetTime();
	VTAUCTION vTAuction;
	MAPTAUCTION::iterator itA;
	VDWORD::iterator itD = pPlayer->m_vAuctionReg.begin();
	while(itD != pPlayer->m_vAuctionReg.end())
	{
		itA = pNpc->m_mapTAuction.find( (*itD) );
		if(itA != pNpc->m_mapTAuction.end())
		{
			if( (*itA).second->m_dEnd > _AtlModule.m_timeCurrent )
				(*itA).second->m_dwRemainTime = (DWORD)((*itA).second->m_dEnd - CurTime);
			else
				(*itA).second->m_dwRemainTime = 0;
			
			vTAuction.push_back((*itA).second);
			itD++;
		}
		else
			itD = pPlayer->m_vAuctionReg.erase(itD);
	}

	pNpc->AlignAuction(&vTAuction,bAlignKind);

	WORD wStartNum = bCountPerPage * (wPageNum - 1);
	WORD wEndNum = bCountPerPage * (wPageNum - 1) + (bCountPerPage * AUCTIONPAGECOUNT);
	if( wEndNum > (WORD)vTAuction.size() )
		wEndNum = (WORD)vTAuction.size();

	if(wEndNum < wStartNum )
	{
		pPlayer->SendCS_AUCTIONREGLIST_ACK(ALR_NOPAGE);
		return EC_NOERROR;
	}

	pPlayer->SendCS_AUCTIONREGLIST_ACK(ALR_SUCCESS,&vTAuction,wPageNum,wStartNum,wEndNum);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_AUCTIONREGCANCEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!IsMainCell(DEFAULT_CHANNEL, pPlayer->m_wMapID, pPlayer->m_fPosX, pPlayer->m_fPosZ))
	{
		pPlayer->SendCS_AUCTIONREGCANCEL_ACK(AR_CHANNEL);
		return EC_NOERROR;
	}

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	DWORD dwAuctionID;
	WORD wNpcID;
	
	pBUF->m_packet
		>> wNpcID
		>> dwAuctionID;
		
	CTNpc* pNpc = FindTNpc(wNpcID);
	if(!pNpc)
	{
		pPlayer->SendCS_AUCTIONREGCANCEL_ACK(AR_NPCNOTFOUND);
		return EC_NOERROR;
	}

	MAPTAUCTION::iterator it = pNpc->m_mapTAuction.find(dwAuctionID);
	if( it == pNpc->m_mapTAuction.end() )
	{
		pPlayer->SendCS_AUCTIONREGCANCEL_ACK(AR_INVALIDAUCTION);
		return EC_NOERROR;
	}

	if( (*it).second->m_dwSeller != pPlayer->m_dwID)
	{
		pPlayer->SendCS_AUCTIONREGCANCEL_ACK(AR_INVALIDCHAR);
		return EC_NOERROR;
	}


	SendDM_AUCTIONREGCANCEL_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		wNpcID,
		dwAuctionID);
		
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_AUCTIONBID_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!IsMainCell(DEFAULT_CHANNEL, pPlayer->m_wMapID, pPlayer->m_fPosX, pPlayer->m_fPosZ))
	{
		pPlayer->SendCS_AUCTIONBID_ACK(ABR_CHANNEL);
		return EC_NOERROR;
	}

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wNpcID;
	DWORD dwAuctionID;	
	__int64 ldwBidPrice;

	pBUF->m_packet
		>> wNpcID
		>> dwAuctionID		
		>> ldwBidPrice;

	if(!pPlayer->CanUseAuction())
	{
		pPlayer->SendCS_AUCTIONBID_ACK(ABR_MAXPOST);
		return EC_NOERROR;
	}

	CTNpc* pNpc = FindTNpc(wNpcID);
	if(!pNpc ||
		(pNpc->m_bCountry != TCONTRY_B &&
		pNpc->m_bCountry != pPlayer->m_bCountry &&
		pNpc->m_bCountry != pPlayer->m_bAidCountry))
	{
		pPlayer->SendCS_AUCTIONBID_ACK(ABR_FAIL);
		return EC_NOERROR;
	}

	MAPTAUCTION::iterator it = pNpc->m_mapTAuction.find(dwAuctionID);
	if( it == pNpc->m_mapTAuction.end())
	{
		pPlayer->SendCS_AUCTIONBID_ACK(ABR_AUCTIONNOTFOUND);
		return EC_NOERROR;
	}

	CTime dEnd((*it).second->m_dEnd);
	CTime dCur(m_timeCurrent);

	if( pNpc->m_bCountry != TCONTRY_B &&
		pNpc->m_bCountry != pPlayer->m_bCountry &&
		dEnd.GetDayOfWeek() < dCur.GetDayOfWeek())
	{
		pPlayer->SendCS_AUCTIONBID_ACK(ABR_LIMITEDDATE);
		return EC_NOERROR;
	}

	if( ldwBidPrice == 0 || ldwBidPrice < (*it).second->m_ldwBidPrice + (*it).second->m_ldwBidPrice * AUCTIONBIDFEE)
	{
		pPlayer->SendCS_AUCTIONBID_ACK(ABR_LOWPRICE);
		return EC_NOERROR;
	}

	if( ldwBidPrice >= (*it).second->m_ldwDirectPrice )
	{
		pPlayer->SendCS_AUCTIONBID_ACK(ABR_LOWPRICE);
		return EC_NOERROR;
	}

	if( (*it).second->m_pItem->m_pTITEM->m_bStack > 1 )
	{
		pPlayer->SendCS_AUCTIONBID_ACK(ABR_AUCTIONNOTFOUND);
		return EC_NOERROR;
	}

	if( (*it).second->m_dwSeller == pPlayer->m_dwID)
	{
		pPlayer->SendCS_AUCTIONBID_ACK(ABR_SELF);
		return EC_NOERROR;
	}

	if( (*it).second->m_dwBidder == pPlayer->m_dwUserID)
	{
		pPlayer->SendCS_AUCTIONBID_ACK(ABR_DUPLICATE);
		return EC_NOERROR;
	}

	if(!pPlayer->UseMoney(ldwBidPrice,TRUE) )
	{
		pPlayer->SendCS_AUCTIONBID_ACK(ABR_NEEDMONEY);
		return EC_NOERROR;
	}

	pPlayer->SendCS_MONEY_ACK();

	SendDM_AUCTIONBID_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_strNAME,
		pPlayer->m_dwUserID,
		wNpcID,
		dwAuctionID,		
		ldwBidPrice);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_AUCTIONBUYDIRECT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!IsMainCell(DEFAULT_CHANNEL, pPlayer->m_wMapID, pPlayer->m_fPosX, pPlayer->m_fPosZ))
	{
		pPlayer->SendCS_AUCTIONBUYDIRECT_ACK(ABR_CHANNEL);
		return EC_NOERROR;
	}

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wNpcID;
	DWORD dwAuctionID;	
	BYTE bCount;
	__int64 ldwBidPrice;

	pBUF->m_packet
		>> wNpcID
		>> dwAuctionID
		>> bCount;	

	if(!pPlayer->CanUseAuction())
	{
		pPlayer->SendCS_AUCTIONBUYDIRECT_ACK(ABR_MAXPOST);
		return EC_NOERROR;
	}

	CTNpc* pNpc = FindTNpc(wNpcID);
	if(!pNpc ||
		(pNpc->m_bCountry != TCONTRY_B &&
		pNpc->m_bCountry != pPlayer->m_bCountry &&
		pNpc->m_bCountry != pPlayer->m_bAidCountry))
	{
		pPlayer->SendCS_AUCTIONBUYDIRECT_ACK(ABR_FAIL);
		return EC_NOERROR;
	}

	MAPTAUCTION::iterator it = pNpc->m_mapTAuction.find(dwAuctionID);
	if( it == pNpc->m_mapTAuction.end())
	{
		pPlayer->SendCS_AUCTIONBUYDIRECT_ACK(ABR_AUCTIONNOTFOUND);
		return EC_NOERROR;
	}
	if( !bCount || bCount > (*it).second->m_pItem->m_bCount)
	{
		pPlayer->SendCS_AUCTIONBUYDIRECT_ACK(ABR_COUNTERROR);
		return EC_NOERROR;
	}

	if( (*it).second->m_dwSeller == pPlayer->m_dwID)
	{
		pPlayer->SendCS_AUCTIONBUYDIRECT_ACK(ABR_SELF);
		return EC_NOERROR;
	}

	if( (*it).second->m_pItem->m_pTITEM->m_bStack > 1 )
		ldwBidPrice = (*it).second->m_ldwStartPrice;
	else
		ldwBidPrice = (*it).second->m_ldwDirectPrice;

	if(!pPlayer->UseMoney(ldwBidPrice * bCount ,TRUE) )
	{
		pPlayer->SendCS_AUCTIONBUYDIRECT_ACK(ABR_NEEDMONEY);
		return EC_NOERROR;
	}

	pPlayer->SendCS_MONEY_ACK();

	SendDM_AUCTIONBUYDIRECT_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_strNAME,
		wNpcID,
		dwAuctionID,
		bCount,
		ldwBidPrice);	

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_AUCTIONFIND_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!IsMainCell(DEFAULT_CHANNEL, pPlayer->m_wMapID, pPlayer->m_fPosX, pPlayer->m_fPosZ))
	{
		pPlayer->SendCS_AUCTIONFIND_ACK(ALR_CHANNEL);
		return EC_NOERROR;
	}

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wNpcID;
	BYTE bMinWearLv;
	BYTE bMaxWearLv;
	BYTE bMinPowerLv;
	BYTE bMaxPowerLv;
	BYTE bOption;
	WORD wPageNum;
	BYTE bCountPerPage;
	BYTE bAlignKind;
	WORD wCountFind;
	DWORD dwCode;
	WORD wItemID;	
	VWORD vItemID;
	VDWORD vCode;
	WORD i;

	vItemID.clear();
	vCode.clear();

	pBUF->m_packet
		>> wNpcID
		>> bMinWearLv
		>> bMaxWearLv
		>> bMinPowerLv
		>> bMaxPowerLv
		>> bOption
		>> wPageNum
		>> bCountPerPage
		>> bAlignKind
		>> wCountFind;

	for(i = 0; i < wCountFind; i++)
	{
		pBUF->m_packet
			>> dwCode
			>> wItemID;

		vCode.push_back(dwCode);
		vItemID.push_back(wItemID);	
	}

	CTNpc* pNpc = FindTNpc(wNpcID);
	if(!pNpc ||
		(pNpc->m_bCountry != TCONTRY_B &&
		pNpc->m_bCountry != pPlayer->m_bCountry &&
		pNpc->m_bCountry != pPlayer->m_bAidCountry))
	{
		pPlayer->SendCS_AUCTIONFIND_ACK(ALR_FAIL);
		return EC_NOERROR;
	}

	VTAUCTION vTAuction;
	MAPDWORD mapFind;
	mapFind.clear();
	
	if(!wCountFind)
		pNpc->SearchAuctionAll(&vTAuction,bMinWearLv,bMaxWearLv,bMinPowerLv,bMaxPowerLv,bOption);
	else
	{	
		for( i = 0;i < (WORD)vCode.size(); i++)
		{
			wItemID = vItemID[i];
			pNpc->SearchAuctions(&mapFind,vCode[i],bMinWearLv,bMaxWearLv,bMinPowerLv,bMaxPowerLv,bOption,wItemID);
		}
	}

	MAPTAUCTION::iterator itA;
	MAPDWORD::iterator itMD;
	for(itMD = mapFind.begin(); itMD != mapFind.end(); itMD++ )
	{
		itA = pNpc->m_mapTAuction.find( (*itMD).second );
		if( itA != pNpc->m_mapTAuction.end() )
			vTAuction.push_back( (*itA).second );
	}	

	if( vTAuction.empty() )
	{
		pPlayer->SendCS_AUCTIONFIND_ACK(ALR_NOLIST);
		return EC_NOERROR;
	}
	if((WORD)vTAuction.size() < WORD(bCountPerPage * (wPageNum - 1)) )
	{
		pPlayer->SendCS_AUCTIONFIND_ACK(ALR_NOPAGE);
		return EC_NOERROR;
	}	

	pNpc->AlignAuction(&vTAuction,bAlignKind);

	WORD wStartNum = bCountPerPage * (wPageNum - 1);
	WORD wEndNum = bCountPerPage * (wPageNum - 1) + (bCountPerPage * AUCTIONPAGECOUNT);
	if( wEndNum > (WORD)vTAuction.size() )
		wEndNum = (WORD)vTAuction.size();

	pPlayer->SendCS_AUCTIONFIND_ACK(ALR_SUCCESS,&vTAuction,wPageNum,wStartNum,wEndNum);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_KICKOUTMAP_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(CheckMapLevel(pPlayer))
		Teleport( pPlayer, pPlayer->m_wLastSpawnID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_REGGUILDCLOAK_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	BYTE bInven;
	BYTE bItemID;
	BYTE bReg;

	pBUF->m_packet
		>> bInven
		>> bItemID
		>> bReg;

	if(!pPlayer->m_dwGuildID)
		return EC_NOERROR;

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
		return EC_NOERROR;

	CTItem * pItem = pInven->FindTItem(bItemID);
	if(!pItem || !pItem->m_bCount)
		return EC_NOERROR;

	if(pItem->m_pTITEM->m_bType != IT_DEFENSIVE ||
		pItem->m_pTITEM->m_bKind != IK_BACK)
		return EC_NOERROR;

	if(bReg &&
		pItem->m_dwExtValue[IEV_GUILD] &&
		pItem->m_dwExtValue[IEV_GUILD] == pPlayer->m_dwID)
		return EC_NOERROR;

	if(bReg)
		pItem->m_dwExtValue[IEV_GUILD] = pPlayer->m_dwID;
	else
		pItem->m_dwExtValue[IEV_GUILD] = 0;

	pPlayer->SendCS_REGGUILDCLOAK_ACK(0, bInven, bItemID, bReg);

	return EC_NOERROR;
}

 
DWORD CTMapSvrModule::OnCS_FAMERANKLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	BYTE bType;
	BYTE bMonth;

	pBUF->m_packet
		>> bType
		>> bMonth;

	if(bMonth >= MONTHCOUNT || bType >= FRT_COUNT)
		return EC_NOERROR;

	pPlayer->SendCS_FAMERANKLIST_ACK(bType, m_arFameRank[bType],bMonth);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_MONTHRANKLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	pPlayer->SendCS_MONTHRANKLIST_ACK(m_bRankMonth,m_arMonthRank);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_FIRSTGRADEGROUP_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	pPlayer->SendCS_FIRSTGRADEGROUP_ACK(m_bRankMonth,m_arFirstGradeGroup);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_WARLORDSAY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bType;
	CString strSay;

	pBUF->m_packet
		>> bType
		>> strSay;

	BYTE bRankMonth = m_bRankMonth - 1;
	if(bRankMonth == 0)
		bRankMonth = 12;

	if(bType >= FRT_COUNT)
		return EC_NOERROR;

	if(m_arFameRank[bType][bRankMonth][0].m_dwCharID != pPlayer->m_dwID)
		return EC_NOERROR;

	SendDM_WARLORDSAY_REQ(bType,bRankMonth,pPlayer->m_dwID,strSay);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CHATBAN_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!IsOperator(pPlayer->m_dwID))
	{
		pPlayer->SendCS_CHATBAN_ACK(CHATBAN_NOTOPERATOR);
		return EC_NOERROR;
	}

	CString strName;
	WORD wMinute = 0;

	pBUF->m_packet
		>> strName
		>> wMinute;

	SendMW_CHATBAN_ACK(strName, wMinute, pPlayer->m_dwID, pPlayer->m_dwKEY);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_POSTLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	WORD wPage;

	pBUF->m_packet
		>> wPage;

	SendDM_POSTLIST_REQ(pPlayer->m_dwID, pPlayer->m_dwKEY, wPage);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_TOURNAMENTAPPLYINFO_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(m_tournament.m_bStep <= TNMTSTEP_NORMAL)
		SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTAPPLYINFO_ACK);
	else if(CanDoTournament(TNMTSTEP_PARTY))
		SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTJOINLIST_ACK);
	else if(CanDoTournament(TNMTSTEP_MATCH))
		SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTMATCHLIST_ACK);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_TOURNAMENTAPPLY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(pPlayer->m_bCountry > TCONTRY_B)
		return EC_NOERROR;

	BYTE bEntryID;

	pBUF->m_packet
		>> bEntryID;

	BYTE bResult = TOURNAMENT_TIMEOUT;

	LPTOURNAMENT pTour = FindTournament(bEntryID);

	if(!pTour)
		bResult = TOURNAMENT_FAIL;

	else if(pTour->m_bMaxLevel < pPlayer->GetLevel() || pTour->m_bMinLevel > pPlayer->GetLevel())
		bResult = TOURNAMENT_LEVEL;

	else if(CanDoTournament(TNMTSTEP_1st))
	{
		for(BYTE i=1; i<m_bFirstGroupCount; i++)
		{
			if(m_arFirstGradeGroup[pPlayer->m_bCountry][i].m_dwCharID == pPlayer->m_dwID)
			{
				bResult = CanApplyTournament(pPlayer, bEntryID);
				if(	bResult == TOURNAMENT_SUCCESS)
					SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTAPPLY_ACK, 0, 0, bEntryID);

				break;
			}
		}

		if(bResult == TOURNAMENT_TIMEOUT)
			bResult = TOURNAMENT_DISQUALIFY;
	}
	else if(CanDoTournament(TNMTSTEP_NORMAL))
	{
		bResult = CanApplyTournament(pPlayer, bEntryID);
		if(bResult == TOURNAMENT_SUCCESS)
			SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTAPPLY_ACK, 0, 0, bEntryID);
	}


	if(bResult == TOURNAMENT_SUCCESS)
	{
		pPlayer->UseMoney(pTour->m_dwFee, TRUE);
		pPlayer->UseItem(pTour->m_wPermitItem, pTour->m_bPermitCount);
	}
	else
	{
		CPacket* pMSG = new CPacket();
		(*pMSG)
			<< pPlayer->m_dwID
			<< pPlayer->m_dwKEY
			<< WORD(MW_TOURNAMENTAPPLY_REQ)
			<< bResult
			<< bEntryID;

		pPlayer->SendCS_TOURNAMENT_ACK(pMSG, MW_TOURNAMENTAPPLY_REQ);
		delete pMSG;
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TOURNAMENTJOINLIST_REQ(LPPACKETBUF pBUF)
{
/*
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!CanDoTournament(TNMTSTEP_PARTY))
		return EC_NOERROR;

	SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTJOINLIST_ACK);
*/
	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TOURNAMENTPARTYLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!CanDoTournament(TNMTSTEP_PARTY))
		return EC_NOERROR;

	DWORD dwChiefID;
	pBUF->m_packet
		>> dwChiefID;

	SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTPARTYLIST_ACK, dwChiefID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TOURNAMENTPARTYADD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!CanDoTournament(TNMTSTEP_PARTY))
		return EC_NOERROR;

	CString strName;
	pBUF->m_packet
		>> strName;

	SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTPARTYADD_ACK, pPlayer->m_dwID, 0, 0, strName);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TOURNAMEMTMATCHLIST_REQ(LPPACKETBUF pBUF)
{
/*
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!CanDoTournament(TNMTSTEP_MATCH))
		return EC_NOERROR;

	pPlayer->m_bViewTournament = TRUE;

	SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTMATCHLIST_ACK);
*/
	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TOURNAMENTEVENTLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!CanDoTournament(TNMTSTEP_ENTER))
		return EC_NOERROR;

	SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTEVENTLIST_ACK);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TOURNAMENTEVENTINFO_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bStep = GetTournamentStep();
	if(bStep != TNMTSTEP_ENTER)
		return EC_NOERROR;

	BYTE bEntryID;
	pBUF->m_packet
		>> bEntryID;

	SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTEVENTINFO_ACK, 0, 0, bEntryID);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TOURNAMENTEVENTJOIN_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bStep = GetTournamentStep();
	if(bStep != TNMTSTEP_ENTER)
		return EC_NOERROR;

	BYTE bEntryID;
	DWORD dwTargetID;

	pBUF->m_packet
		>> bEntryID
		>> dwTargetID;

	SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTEVENTJOIN_ACK, dwTargetID, 0, bEntryID);

#ifdef DEF_UDPLOG
	LPTOURNAMENT tEntry = FindTournament(bEntryID);
	if(tEntry)
		m_pUdpSocket->LogTournamentEvent(LOGMAP_TOURNAMENTEVENT, pPlayer, dwTargetID, tEntry->m_strName, tEntry->m_bType);
#endif	//	DEF_UDPLOG

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TOURNAMENTCHEER_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	BYTE bInven;
	BYTE bItem;
	DWORD dwTargetID;

	pBUF->m_packet
		>> bInven
		>> bItem
		>> dwTargetID;

	if(m_tournament.m_bStep != TNMTSTEP_QFINAL &&
		m_tournament.m_bStep != TNMTSTEP_SFINAL &&
		m_tournament.m_bStep != TNMTSTEP_FINAL)
		return EC_NOERROR;

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
		return EC_NOERROR;

	CTItem * pItem = pInven->FindTItem(bItem);
	if(!pItem || !pItem->m_bCount)
		return EC_NOERROR;

	if(pItem->m_pTITEM->m_bType != IT_USE ||
		pItem->m_pTITEM->m_bKind != IK_CHEER ||
		!pItem->m_bCount)
		return EC_NOERROR;

	LPTOURNAMENTPLAYER pTP = FindTournamentPlayer(dwTargetID, TRUE);
	if(!pTP || GetTournamentMap(dwTargetID, m_tournament.m_bGroup) != pPlayer->m_wMapID)
		return EC_NOERROR;

	MAPPLAYER::iterator it = m_mapPLAYER.find(dwTargetID);
	if(it==m_mapPLAYER.end())
		return EC_NOERROR;

	CTPlayer * pTPlayer = (*it).second;
	if(!pTPlayer->m_bMain ||
		!pTPlayer->m_pMAP ||
		pTPlayer->m_wMapID != pPlayer->m_wMapID ||
		pTPlayer->m_bStatus == OS_DEAD)
		return EC_NOERROR;

	DWORD dwMaxHP = pTPlayer->GetMaxHP();
	DWORD dwMaxMP = pTPlayer->GetMaxMP();

	if(dwMaxHP <= pPlayer->m_dwHP)
		return EC_NOERROR;

	pTPlayer->m_dwHP += pItem->m_pTITEM->m_wUseValue;
	if(dwMaxHP < pTPlayer->m_dwHP)
		pTPlayer->m_dwHP = dwMaxHP;

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	pPlayer->m_pMAP->GetNeighbor(
		&vPLAYERS,
		pTPlayer->m_fPosX,
		pTPlayer->m_fPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_HPMP_ACK(
			pTPlayer->m_dwID,
			pTPlayer->m_bType,
			pTPlayer->m_bLevel,
			dwMaxHP,
			pTPlayer->m_dwHP,
			dwMaxMP,
			pTPlayer->m_dwMP);

		pChar->SendCS_TOURNAMENTCHEER_ACK(
			pPlayer->m_dwID,
			dwTargetID);

		vPLAYERS.pop_back();
	}

	UseItem(pPlayer, pInven, pItem, 1);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TOURNAMENTPARTYDEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!CanDoTournament(TNMTSTEP_PARTY))
		return EC_NOERROR;

	DWORD dwTarget;
	pBUF->m_packet
		>> dwTarget;

	SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTPARTYDEL_ACK, dwTarget);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnCS_TOURNAMENTSCHEDULE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	SendMW_TOURNAMENT_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, MW_TOURNAMENTSCHEDULE_ACK);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_RPSSTART_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	BYTE bType;
	BYTE bInven;
	BYTE bItemID;

	pBUF->m_packet
		>> bType
		>> bInven
		>> bItemID;

	if(pPlayer->m_bRPSType)
	{
		pPlayer->SendCS_RPSSTART_ACK(RPS_INGAME);
		return EC_NOERROR;
	}

	MAPRPSGAME::iterator it = m_mapRPSGame.find(MAKEWORD(bType, 0));
	if(it == m_mapRPSGame.end())
	{
		pPlayer->SendCS_RPSSTART_ACK(RPS_NODATA);
		return EC_NOERROR;
	}

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
	{
		pPlayer->SendCS_RPSSTART_ACK(RPS_NEEDITEM);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bItemID);
	if(!pItem)
	{
		pPlayer->SendCS_RPSSTART_ACK(RPS_NEEDITEM);
		return EC_NOERROR;
	}

	if(pItem->m_pTITEM->m_wItemID != (*it).second.m_wItemID)
	{
		pPlayer->SendCS_RPSSTART_ACK(RPS_NEEDITEM);
		return EC_NOERROR;
	}

	if(!UseItem(pPlayer, pInven, pItem, 1))
	{
		pPlayer->SendCS_RPSSTART_ACK(RPS_NEEDITEM);
		return EC_NOERROR;
	}

	pPlayer->m_bRPSType = bType;
	pPlayer->m_bRPSWin = 0;

	pPlayer->SendCS_RPSSTART_ACK(RPS_SUCCESS);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_RPSGAME_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	BYTE bPlayerRPS;

	pBUF->m_packet
		>> bPlayerRPS;

	if(!pPlayer->m_bRPSType)
		return EC_NOERROR;

	MAPRPSGAME::iterator it = m_mapRPSGame.find(MAKEWORD(pPlayer->m_bRPSType, pPlayer->m_bRPSWin));
	if(it == m_mapRPSGame.end())
	{
		pPlayer->ResetRPS();
		return EC_NOERROR;
	}

	TRPSGAME rps = (*it).second;

	if(bPlayerRPS >= RPSCARD_COUNT)
	{
		RPSReward(pPlayer);
		return EC_NOERROR;
	}

	BYTE bNpcRPS = RPSCARD_ROCK;

	DWORD dwRand = rps.m_bProb[2] ? rand() % rps.m_bProb[2] : 0xFF;

	if(dwRand < rps.m_bProb[0])
	{
		if(rps.m_wWinKeep)
		{
			SendMW_RPSGAME_ACK(
				pPlayer->m_dwID,
				pPlayer->m_dwKEY,
				pPlayer->m_bRPSType,
				pPlayer->m_bRPSWin + 1,
				bPlayerRPS);

			return EC_NOERROR;
		}

		pPlayer->m_bRPSWin++;
		bNpcRPS = SelectNpcRPS(TNMTWIN_WIN, bPlayerRPS);
	}
	else if(dwRand < rps.m_bProb[1])
	{
		bNpcRPS = bPlayerRPS;
	}
	else
	{
		pPlayer->ResetRPS();
		bNpcRPS = SelectNpcRPS(TNMTWIN_LOSE, bPlayerRPS);
	}

//	ATLTRACE2("P:%d, N:%d, R:%d, W:%d\n",bPlayerRPS,bNpcRPS,dwRand,pPlayer->m_bRPSWin);

	pPlayer->SendCS_RPSGAME_ACK(pPlayer->m_bRPSWin, bPlayerRPS, bNpcRPS);

	MAPRPSGAME::iterator itF = m_mapRPSGame.find(MAKEWORD(pPlayer->m_bRPSType, pPlayer->m_bRPSWin + 1));
	if(itF == m_mapRPSGame.end())
		RPSReward(pPlayer, FALSE);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_ACDCLOSE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	BYTE bType;

	pBUF->m_packet
		>> bType;

#ifdef DEF_UDPLOG
	m_pUdpSocket->LogKickOut(LOGMAP_KICKOUT, pPlayer, bType);
#endif	//	DEF_UDPLOG

	MAPTPVPOINT::iterator itF = m_mapTPvPointKill.find(MAKEWORD(PVPS_NORMAL, PVPE_KILL_E));
	if(itF == m_mapTPvPointKill.end())
		return EC_NOERROR;

	LPTPVPOINT pPvP = (*itF).second;
	DWORD dwDecPoint = pPlayer->m_pTLEVEL->m_wPvPoint * pPvP->m_dwDecPoint / 100;

	pPlayer->UsePvPoint(dwDecPoint, PVPE_KILL_E, PVP_TOTAL, NULL);

	CloseSession(pPlayer);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CHANGECOUNTRY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	BYTE bType;
	BYTE bCountry;
	BYTE bInven = INVEN_NULL;
	BYTE bItem = INVALID_SLOT;

	pBUF->m_packet
		>> bType
		>> bCountry
		>> bInven
		>> bItem;

	if(pPlayer->m_wPartyID)
	{
		pPlayer->SendCS_CHANGECHARBASE_ACK(CCB_PARTY, pPlayer->m_dwID, bType, bCountry, NAME_NULL);
		return EC_NOERROR;
	}

	if(pPlayer->m_dwTacticsID)
	{
		pPlayer->SendCS_CHANGECHARBASE_ACK(CCB_TACTICS, pPlayer->m_dwID, bType, bCountry, NAME_NULL);
		return EC_NOERROR;
	}

	if(bType == IK_AIDCOUNTRY)
	{
		if(pPlayer->m_bAidCountry == bCountry)
			return EC_NOERROR;

		if(pPlayer->m_bLevel < BROA_BASELEVEL)
		{
			pPlayer->SendCS_CHANGECHARBASE_ACK(CCB_LEVEL, pPlayer->m_dwID, bType, bCountry, NAME_NULL);
			return EC_NOERROR;
		}

		if(pPlayer->GetAidLeftTime())
		{
			pPlayer->SendCS_CHANGECHARBASE_ACK(CCB_TIME, pPlayer->m_dwID, bType, bCountry, NAME_NULL, pPlayer->GetAidLeftTime());
			return EC_NOERROR;
		}

		if(pPlayer->m_bCountry != TCONTRY_B || bCountry == TCONTRY_B)
		{
			pPlayer->SendCS_CHANGECHARBASE_ACK(CCB_FAIL, pPlayer->m_dwID, bType, bCountry, NAME_NULL);
			return EC_NOERROR;
		}

		BYTE bGap = pPlayer->m_bLevel >= BROA_BASELEVEL ? (pPlayer->m_bLevel - BROA_BASELEVEL) / 10 : WARCOUNTRY_MAXGAP;
		if(bGap >= WARCOUNTRY_MAXGAP)
		{
			pPlayer->SendCS_CHANGECHARBASE_ACK(CCB_FAIL, pPlayer->m_dwID, bType, bCountry, NAME_NULL);
			return EC_NOERROR;
		}

		if(m_dwWarCountryBalance[TCONTRY_D][bGap] + m_dwWarCountryBalance[TCONTRY_C][bGap] > WARBALANCE_BASECOUNT &&
			m_dwWarCountryBalance[bCountry][bGap] / float(m_dwWarCountryBalance[TCONTRY_D][bGap] + m_dwWarCountryBalance[TCONTRY_C][bGap]) >= 0.525)
		{
			pPlayer->SendCS_CHANGECHARBASE_ACK(CCB_BALANCE, pPlayer->m_dwID, bType, bCountry, NAME_NULL);
			return EC_NOERROR;
		}
	}
	else
	{
		if(pPlayer->m_bCountry == bCountry)
			return EC_NOERROR;

		if(pPlayer->m_dwGuildID)
		{
			pPlayer->SendCS_CHANGECHARBASE_ACK(CCB_GUILD, pPlayer->m_dwID, bType, bCountry, NAME_NULL);
			return EC_NOERROR;
		}

		if(pPlayer->m_bCountry != TCONTRY_PEACE)
		{
			if(pPlayer->m_bLevel < BROA_BASELEVEL)
			{
				pPlayer->SendCS_CHANGECHARBASE_ACK(CCB_LEVEL, pPlayer->m_dwID, bType, bCountry, NAME_NULL);
				return EC_NOERROR;
			}

			BYTE bGap = pPlayer->m_bLevel >= BROA_BASELEVEL ? (pPlayer->m_bLevel - BROA_BASELEVEL) / 10 : WARCOUNTRY_MAXGAP;

			if(bGap >= WARCOUNTRY_MAXGAP)
				return EC_NOERROR;

			if(pPlayer->m_bCountry == TCONTRY_B)
			{
				CTInven * pInven = pPlayer->FindTInven(bInven);
				if(!pInven)
				{
					pPlayer->SendCS_CHANGECHARBASE_ACK(CCB_NOITEM, pPlayer->m_dwID, bType, bCountry, NAME_NULL);
					return EC_NOERROR;
				}

				CTItem * pItem = pInven->FindTItem(bItem);
				if(!pItem || pItem->m_pTITEM->m_bKind != IK_COUNTRY)
				{
					pPlayer->SendCS_CHANGECHARBASE_ACK(CCB_NOITEM, pPlayer->m_dwID, bType, bCountry, NAME_NULL);
					return EC_NOERROR;
				}

				bCountry = pPlayer->m_bOriCountry;

				if(pPlayer->m_bAidCountry != TCONTRY_N)
					pPlayer->ChangeCharBase(IK_AIDCOUNTRY, NAME_NULL, TCONTRY_N);

				UseItem(pPlayer, pInven, pItem, 1);
				pPlayer->SendCS_MOVEITEM_ACK(MI_SUCCESS);
			}
			else if(bCountry != TCONTRY_B)
				return EC_NOERROR;
		}
		else
		{
			if(pPlayer->m_bLevel < CHOICE_COUNTRY_LEVEL)
			{
				pPlayer->SendCS_CHANGECHARBASE_ACK(CCB_LEVEL, pPlayer->m_dwID, bType, bCountry, NAME_NULL);
				return EC_NOERROR;
			}

			if(bCountry >= TCONTRY_B)
				return EC_NOERROR;
		}

		WORD wPortal = 0;
		switch(bCountry)
		{
		case TCONTRY_D: wPortal = TSTART_D_PORTAL_ID; break;
		case TCONTRY_C: wPortal = TSTART_C_PORTAL_ID; break;
		case TCONTRY_B: wPortal = TSTART_B_PORTAL_ID; break;
		}

		MAPTPORTAL::iterator itPt = m_mapTPortal.find(wPortal);
		if(itPt != m_mapTPortal.end())
			pPlayer->m_wSpawnID = itPt->second->m_wSpawnID;
	}

	MAPTRECALLMON::iterator it;
	for(it=pPlayer->m_mapRecallMon.begin(); it!=pPlayer->m_mapRecallMon.end(); it++)
	{
		if((*it).second->m_pMON->m_bRecallType != TRECALLTYPE_PET)
		{
			(*it).second->OnDie(0,OT_NONE,0);
			SendMW_RECALLMONDEL_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, (*it).second->m_dwID);
		}
	}

	MAPTSELFOBJ::iterator itSf;
	while(!pPlayer->m_mapSelfMon.empty())
	{
		itSf=pPlayer->m_mapSelfMon.begin();
		(*itSf).second->OnDie(0,OT_NONE,0); 
		pPlayer->DeleteSelfMon((*itSf).second->m_dwID);
	}

	pPlayer->ChangeCharBase(bType, NAME_NULL, bCountry);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_WARCOUNTRYBALANCE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	SendMW_WARCOUNTRYBALANCE_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_MEETINGROOM_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain )
		return EC_NOERROR;

	BYTE bType;
	BYTE bValue;
	CString strName;

	pBUF->m_packet
		>> bType
		>> bValue
		>> strName;

	if(!bType)
	{
		if(!IsChiefMeetingRoom(pPlayer))
		{
			pPlayer->SendCS_MEETINGROOM_ACK(bType, MTR_NOTCHIEF, strName);
			return EC_NOERROR;
		}
	}

	SendMW_MEETINGROOM_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, bType, bValue, strName);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_ARENA_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if(!pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!IsMainCell(DEFAULT_CHANNEL, pPlayer->m_wMapID, pPlayer->m_fPosX, pPlayer->m_fPosZ))
	{
		pPlayer->SendCS_ARENA_ACK(ARENA_CHANNEL);
		return EC_NOERROR;
	}

	BYTE bCommand;
	WORD wNpcID;
	WORD wArenaID;

	pBUF->m_packet
		>> bCommand
		>> wNpcID
		>> wArenaID;

	if(pPlayer->m_dwID != pPlayer->GetPartyChiefID())
	{
		pPlayer->SendCS_ARENA_ACK(ARENA_CHIEF);
		return EC_NOERROR;
	}

	BYTE bTeam = 0;

	if(bCommand)
	{
		MAPARENA::iterator itAn = m_mapArena.find(wArenaID);
		if(itAn != m_mapArena.end() && itAn->second->m_bStatus == BS_NORMAL)
			ArenaEnd(itAn->second, pPlayer);
	}
	else
	{
		CTNpc * pNpc = FindTNpc(wNpcID);
		if(!pNpc || !pNpc->m_pArena)
		{
			pPlayer->SendCS_ARENA_ACK(ARENA_FAIL);
			return EC_NOERROR;
		}

		if(pPlayer->m_wArenaID)
		{
			pPlayer->SendCS_ARENA_ACK(ARENA_FAIL);
			return EC_NOERROR;
		}

		if(!pPlayer->UseMoney(pNpc->m_pArena->m_dwFee, FALSE))
		{
			pPlayer->SendCS_ARENA_ACK(ARENA_MONEY);
			return EC_NOERROR;
		}

		if(pNpc->m_pArena->m_mapFighter[0].empty())
			bTeam = 1;
		else if(pNpc->m_pArena->m_mapFighter[1].empty())
			bTeam = 2;
		else
		{
			pPlayer->SendCS_ARENA_ACK(ARENA_FULL);
			return EC_NOERROR;
		}

		VPLAYER vPlayer;
		VDWORD vParty;
		pPlayer->m_pMAP->GetNeerPlayer(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);
		for(DWORD i=0; i<vPlayer.size(); i++)
		{
			CTPlayer * pChar = vPlayer[i];
			if(pChar->GetPartyID() && pChar->GetPartyID() == pPlayer->GetPartyID())
				vParty.push_back(pChar->m_dwID);
		}

		if(vParty.size() > pNpc->m_pArena->m_bType)
		{
			pPlayer->SendCS_ARENA_ACK(ARENA_MAXPARTY);
			return EC_NOERROR;
		}

		if(vParty.size() < BYTE(pNpc->m_pArena->m_bType/2.0f + 0.99))
		{
			pPlayer->SendCS_ARENA_ACK(ARENA_MINPARTY);
			return EC_NOERROR;
		}

		SendMW_ARENAJOIN_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, TRUE, vParty);

		pPlayer->UseMoney(pNpc->m_pArena->m_dwFee, TRUE);
		pPlayer->SendCS_MONEY_ACK();

		BYTE bStart = FALSE;
		if((pNpc->m_pArena->m_mapFighter[0].size() && bTeam == 2) || (pNpc->m_pArena->m_mapFighter[1].size() && bTeam == 1))
			bStart = TRUE;

		if(bStart)
		{
			pNpc->m_pArena->m_bStatus = BS_READY;
			pNpc->m_pArena->m_dwTick = m_dwTick;
		}

		while(!vPlayer.empty())
		{
			CTPlayer * pChar = vPlayer.back();
			vPlayer.pop_back();
			pChar->SendCS_ARENATEAM_ACK(pNpc->m_pArena->m_wID, bTeam, vParty);

			if(pChar->GetPartyID() && pChar->GetPartyID() == pPlayer->GetPartyID())
			{
				pChar->m_wArenaID = pNpc->m_pArena->m_wID;
				pChar->m_bArenaTeam = bTeam;
				pNpc->m_pArena->m_mapFighter[bTeam-1].insert(MAPPLAYER::value_type(pChar->m_dwID, pChar));

				pChar->m_aftermath.m_bStep = 0;
				pChar->m_aftermath.m_fReuseInc = 0;
				pChar->m_aftermath.m_fStatDec = 0;
				pChar->ResetCoolTime(0);
				pChar->ReleaseMaintain(TRUE);
				pChar->DeleteAllRecallMon();
				pChar->DeleteAllSelfMon();

				Teleport(pChar, pNpc->m_pArena->m_pInPos->m_wID);
			}

			if(bStart)
				pChar->SendCS_SYSTEMMSG_ACK(
					SM_ARENA_COUNTDOWN,
					pNpc->m_pArena->m_wID,
					60,
					NAME_NULL,
					NAME_NULL,
					pNpc->m_pArena->m_bStatus,
					pNpc->m_pArena->m_bWinCount);
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CMMOVE_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer*)pBUF->m_pSESSION;

	BYTE bChannel;
	WORD wMapID;

	float fPosX;
	float fPosY;
	float fPosZ;

	pBUF->m_packet
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ;

	MAPDWORD::iterator it = m_mapTOPERATOR.find(pPlayer->m_dwID);
	if(it != m_mapTOPERATOR.end() )
		Teleport( pPlayer, bChannel, wMapID, fPosX, fPosY, fPosZ);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CMMOVETOUSER_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer*)pBUF->m_pSESSION;

	CString strTargetUser;

	pBUF->m_packet
		>> strTargetUser;

	MAPDWORD::iterator it = m_mapTOPERATOR.find(pPlayer->m_dwID);
	if(it != m_mapTOPERATOR.end() )
		SendCT_USERPOSITION_ACK(pPlayer->m_strNAME, strTargetUser);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCS_CMGIFT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer*)pBUF->m_pSESSION;

	CString strTargetUser;
	WORD wGiftID;

	pBUF->m_packet
		>> strTargetUser
		>> wGiftID;

	MAPDWORD::iterator it = m_mapTOPERATOR.find(pPlayer->m_dwID);
	if(it != m_mapTOPERATOR.end())
		SendMW_CMGIFT_ACK(strTargetUser, wGiftID, pPlayer->m_dwID);

	return EC_NOERROR;
}

#ifdef __HACK_SHIELD
DWORD CTMapSvrModule::OnCS_HACKSHIELD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer * pPlayer = (CTPlayer*)pBUF->m_pSESSION;

	BYTE bServerID;
	AHNHS_TRANS_BUFFER stReceiveBuf;

	pBUF->m_packet
		>> bServerID
		>> stReceiveBuf.nLength;

//	LogHackShield(pPlayer->m_dwID, bServerID, _T("Receive ESSD"));

	if(bServerID != m_bServerID)
		return EC_NOERROR;

	pPlayer->m_dwHSRecvTick = 0;

	if(stReceiveBuf.nLength > ANTICPX_TRANS_BUFFER_MAX)
	{
		LogHackShield(pPlayer->m_dwID, stReceiveBuf.nLength, _T("Invalid Buffer Size"));
		CloseSession(pPlayer);
		return EC_NOERROR;
	}

	pBUF->m_packet.Rewind(FALSE);
	pBUF->m_packet
		>> bServerID;

	pBUF->m_packet.DetachBinary(stReceiveBuf.byBuffer);

	DWORD dwErrorCode;
	DWORD dwRet = _AhnHS_VerifyResponseEx(pPlayer->m_hHackShield, stReceiveBuf.byBuffer, stReceiveBuf.nLength, &dwErrorCode);
	if(dwRet == ANTICPX_RECOMMAND_CLOSE_SESSION)
	{
		LogHackShield(pPlayer->m_dwID, dwErrorCode, _T("_AhnHS_VerifyResponseEx"));
		CloseSession(pPlayer);
	}

	return EC_NOERROR;
}
#endif

////	nProtect GameGuard	////////////////////////////////////////////////////
#ifdef __N_PROTECT
DWORD CTMapSvrModule::OnCS_NPROTECT_REQ(LPPACKETBUF pBUF)
{
	if(!m_bEnableNP)
		return EC_NOERROR;

	CTPlayer * pPlayer = (CTPlayer*)pBUF->m_pSESSION;

	BYTE bServerID;
	int	nLenth;

	pBUF->m_packet
		>> bServerID
		>> nLenth;

	if(bServerID != m_bServerID)
		return EC_NOERROR;

	//	데이터 길이검사
	if(nLenth != sizeof(GG_AUTH_DATA))
	{
		LogNProtect(pPlayer->m_dwID, nLenth, _T("Invalid Buffer Size"));
		CloseSession(pPlayer);
		return EC_NOERROR;
	}

	pBUF->m_packet.Rewind(FALSE);
	pBUF->m_packet
		>> bServerID;
	pBUF->m_packet.DetachBinary(&pPlayer->m_csNProtect->m_AuthAnswer);

	DWORD dwRet = pPlayer->m_csNProtect->CheckAuthAnswer();
	if(dwRet != ERROR_SUCCESS)	
	{
		CString strBuf;
		strBuf.Format("Receive Err [Query : %08X %08X %08X %08X]", 
			pPlayer->m_csNProtect->m_AuthQuery.dwIndex, pPlayer->m_csNProtect->m_AuthQuery.dwValue1, pPlayer->m_csNProtect->m_AuthQuery.dwValue2, pPlayer->m_csNProtect->m_AuthQuery.dwValue3);

		LogNProtect(pPlayer->m_dwID, dwRet, strBuf);
		CloseSession(pPlayer);
		return EC_NOERROR;
	}

#ifdef __N_PROTECT_DEBUG
	CString strBuf;
	strBuf.Format("User Req [ReqTick:%5d] [Query:%08X %08X %08X %08X] [Answer:%08X %08X %08X %08X]", m_dwTick - pPlayer->m_dwNPSendTime,
		pPlayer->m_csNProtect->m_AuthQuery.dwIndex, pPlayer->m_csNProtect->m_AuthQuery.dwValue1, pPlayer->m_csNProtect->m_AuthQuery.dwValue2, pPlayer->m_csNProtect->m_AuthQuery.dwValue3, 
		pPlayer->m_csNProtect->m_AuthAnswer.dwIndex, pPlayer->m_csNProtect->m_AuthAnswer.dwValue1, pPlayer->m_csNProtect->m_AuthAnswer.dwValue2, pPlayer->m_csNProtect->m_AuthAnswer.dwValue3);

	LogNProtect(pPlayer->m_dwID, 0, strBuf);
#endif

	pPlayer->m_bSend = FALSE;
	if(!pPlayer->m_bFirst)
	{
		pPlayer->m_bFirst = TRUE;
		pPlayer->m_dwNPSendTime = 0;
	}

	return EC_NOERROR;
}
#endif
/////////////////////////////////////////////////////////////////////////////