// SSHandler.cpp : S/S Protocol Handler 함수의 구현입니다.

#include "stdafx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

DWORD CTMapSvrModule::OnSM_QUITSERVICE_REQ( LPPACKETBUF pBUF)
{
	LogEvent(_T("SM_QUITSERVICE_REQ detected !!"));
	if(m_bService)
		SetServiceStatus(SERVICE_STOP_PENDING);
	PostThreadMessage( m_dwThreadID, WM_QUIT, 0, 0);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_DELSESSION_REQ( LPPACKETBUF pBUF)
{
	MAPPLAYER::iterator finder = m_mapSESSION.find((DWORD_PTR) pBUF->m_pSESSION);

	if( pBUF->m_pSESSION != &m_world &&
		finder == m_mapSESSION.end() )
		return EC_NOERROR;
	((CTMapSession *) pBUF->m_pSESSION)->m_bCanDelete = TRUE;

	if( pBUF->m_pSESSION != &m_world &&
		((CTPlayer *) pBUF->m_pSESSION)->m_bCloseAll &&
		((CTPlayer *) pBUF->m_pSESSION)->m_dwID)
	{
		SendMW_CLOSECHAR_ACK(
			((CTPlayer *) pBUF->m_pSESSION)->m_dwID,
			((CTPlayer *) pBUF->m_pSESSION)->m_dwKEY);
	}
	else
		SetEventCloseSession((CTMapSession *)pBUF->m_pSESSION, ((CTPlayer *) pBUF->m_pSESSION)->m_bMain);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_TIMER_REQ(LPPACKETBUF pBUF)
{
	BYTE bTnmtNotice = 0;
	DWORD dwSecond = 0;
	DWORD dwCurTick = m_dwTick;

	MAPARENA::iterator itArn;
	DWORD dwArenaPeriod = 4*60;
	for(itArn=m_mapArena.begin(); itArn!=m_mapArena.end(); itArn++)
	{
		BYTE bNotice = FALSE;
		LPTARENA pArena = itArn->second;
		if(pArena->m_bStatus == BS_READY)
		{
			dwSecond = (dwCurTick - pArena->m_dwTick) / 1000;
			if(dwSecond >= 60)
			{
				pArena->m_bStatus = BS_BATTLE;
				dwSecond = dwArenaPeriod;
				bNotice = TRUE;
			}
			else if(dwSecond >= 50)
			{
				dwSecond = 60 - dwSecond;
				bNotice = TRUE;
			}
		}
		else if(pArena->m_bStatus == BS_BATTLE)
		{
			dwSecond = (dwCurTick - pArena->m_dwTick) / 1000;
			if(dwSecond >= dwArenaPeriod)
				ArenaEnd(pArena, NULL);
			else if(dwSecond >= dwArenaPeriod - 10)
			{
				dwSecond = dwArenaPeriod - dwSecond;
				bNotice = TRUE;
			}
		}

		if(bNotice)
		{
			for(BYTE i=0; i<2; i++)
			{
				MAPPLAYER::iterator itAP;
				for(itAP=pArena->m_mapFighter[i].begin(); itAP != pArena->m_mapFighter[i].end(); itAP++)
					itAP->second->SendCS_SYSTEMMSG_ACK(SM_ARENA_COUNTDOWN, pArena->m_wID, dwSecond, NAME_NULL, NAME_NULL, pArena->m_bStatus, pArena->m_bWinCount);
			}
		}
	}

	dwSecond = 0;

	if((m_tournament.m_bStep == TNMTSTEP_ENTER ||
		m_tournament.m_bStep == TNMTSTEP_SFENTER ||
		m_tournament.m_bStep == TNMTSTEP_FENTER) &&
		m_tournament.m_dlEndTime > m_timeCurrent)
	{
		dwSecond = DWORD(m_tournament.m_dlEndTime - m_timeCurrent);
		if(dwSecond >= 60)
		{
			 if(dwSecond % 60 == 0)	bTnmtNotice = 1;
		}
		else if(dwSecond >= 10)
		{
			if(dwSecond % 10 == 0) bTnmtNotice = 1;
		}
		else if(dwSecond < 10)
			bTnmtNotice = 1;
	}

	if((m_tournament.m_bStep == TNMTSTEP_QFINAL ||
		m_tournament.m_bStep == TNMTSTEP_SFINAL ||
		m_tournament.m_bStep == TNMTSTEP_FINAL) &&
		m_tournament.m_dlEndTime > m_timeCurrent)
	{
		dwSecond = DWORD(m_tournament.m_dlEndTime - m_timeCurrent);
		if(dwSecond % 10 == 0)
			bTnmtNotice = 2;
	}

	if(!m_vTNMTRESULTMAP.empty() &&
		m_tournament.m_bResult >= TNMTSTEP_QFINAL &&
		dwCurTick - m_dwTNMTResultTick > 10000)
	{
		for(BYTE i=0; i<m_vTNMTRESULTMAP.size(); i++)
		{
			CTMap * pMap = m_vTNMTRESULTMAP[i];

			BYTE bRet = 0;
			DWORD dwWinner = 0;
			DWORD dwLoser = 0;

			pMap->EndTournament(bRet, dwWinner, dwLoser);

			SendMW_TOURNAMENTRESULT_ACK(
				m_tournament.m_bResult,
				bRet,
				dwWinner,
				dwLoser);
		}

		m_tournament.m_bResult = 0;
		m_vTNMTRESULTMAP.clear();
	}

	BYTE bHelpID = 0;
	CString strHelpMessage;
	MAPHELPMESSAGE::iterator itHp = m_mapHelpMessage.find(HELPMESSAGE_EVENT);
	if(itHp != m_mapHelpMessage.end())
	{
		if((*itHp).second.m_dlEndTime &&
            !(*itHp).second.m_bNotice &&
            (*itHp).second.m_dlStartTime <= m_timeCurrent)
		{
			(*itHp).second.m_bNotice = TRUE;
			bHelpID = (*itHp).second.m_bID;
			strHelpMessage = (*itHp).second.m_strMessage;
		}
		else if((*itHp).second.m_dlEndTime < m_timeCurrent)
		{
			bHelpID = (*itHp).second.m_bID;
			strHelpMessage.Empty();
			m_mapHelpMessage.erase(itHp);

			itHp = m_mapHelpMessage.find(HELPMESSAGE_NORMAL);
			if(itHp != m_mapHelpMessage.end())
			{
				bHelpID = (*itHp).second.m_bID;
				strHelpMessage = (*itHp).second.m_strMessage;
			}
		}
	}

	MAPPLAYER::iterator it;
	for(it=m_mapPLAYER.begin(); it!=m_mapPLAYER.end(); it++)
	{
		CTPlayer * pPlayer = (*it).second;
		if(!pPlayer->m_pMAP)
			continue;

		pPlayer->CheckTimeRecallMon(dwCurTick);

		if(pPlayer->m_bMain)
		{
			pPlayer->OnTimer(dwCurTick);

#ifdef __HACK_SHIELD
			if(m_bNation == NATION_GERMAN)
				CheckHackShield(pPlayer);
#endif
#ifdef __N_PROTECT
		if(m_bEnableNP)
			CheckCSNProtect(pPlayer);
#endif

			if(bTnmtNotice)
			{
				if( bTnmtNotice == 1 &&
					(IsTournamentMap(pPlayer->m_wMapID) || GetLoungeMapID() == pPlayer->m_wMapID))
					pPlayer->SendCS_SYSTEMMSG_ACK(SM_TOURNAMENT_COUNTDOWN, m_tournament.m_bStep, dwSecond);

				if(bTnmtNotice == 2 && IsTournamentMap(pPlayer->m_wMapID))
					pPlayer->SendCS_SYSTEMMSG_ACK(SM_TOURNAMENT_COUNTDOWN, m_tournament.m_bStep, dwSecond);
			}

			if(bHelpID && !pPlayer->IsTutorial())
				pPlayer->SendCS_HELPMESSAGE_ACK(bHelpID, strHelpMessage);
		}
	}
#ifdef __N_PROTECT
	if(m_bEnableNP)
		UpdateCSModule();
#endif

	MAPTONTIMEMON::iterator itMon;
	for(itMon=m_mapTOnTimeMon.begin(); itMon!=m_mapTOnTimeMon.end();)
	{
		CTMap * pMap = FindTMap((*itMon).second.m_bChannel, (*itMon).second.m_wPartyID, (*itMon).second.m_wMapID);
		if(pMap)
		{
			CTMonster * pMon = pMap->FindMonster((*itMon).second.m_dwID);
			if(pMon && pMon->m_pMAP)
			{
				pMon->Recover(dwCurTick);
				pMon->CheckMaintainSkill(dwCurTick);

				if(pMon->m_bStatus == OS_DISAPPEAR || pMon->m_bStatus == OS_DEAD ||
					(pMon->m_dwHP == pMon->GetMaxHP() && pMon->m_vMaintainSkill.empty()))
					itMon = m_mapTOnTimeMon.erase(itMon);
				else
					 itMon++;
			}
			else
				 itMon++;
		}
		else
			 itMon++;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_RESETHOST_REQ( LPPACKETBUF pBUF)
{
	BYTE bChannel;
	WORD wMapID;
	WORD wPartyID;
	DWORD dwMonID;

	pBUF->m_packet
		>> bChannel
		>> wMapID
		>> wPartyID
		>> dwMonID;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(SM_RESETHOST_ACK)
		<< bChannel
		<< wMapID
		<< wPartyID
		<< dwMonID;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_RESETHOST_ACK(LPPACKETBUF pBUF)
{
	BYTE bChannel;
	WORD wMapID;
	WORD wPartyID;
	DWORD dwMonID;

	pBUF->m_packet
		>> bChannel
		>> wMapID
		>> wPartyID
		>> dwMonID;

	CTMap *pMAP = FindTMap(bChannel, wPartyID, wMapID);

	if(!pMAP)
		return EC_NOERROR;

	CTMonster * pMon = pMAP->FindMonster(dwMonID);
	if(pMon &&
		pMon->m_pMAP &&
		!pMon->m_dwHostID)
	{
		pMon->m_fStartX = pMon->m_fNextX = pMon->m_pSPAWN->m_pSPAWN->m_fPosX;
		pMon->m_fStartY = pMon->m_fNextY = pMon->m_fPosY = pMon->m_pSPAWN->m_pSPAWN->m_fPosY;
		pMon->m_fStartZ = pMon->m_fNextZ = pMon->m_pSPAWN->m_pSPAWN->m_fPosZ;
		pMon->m_wDIR = pMon->m_pSPAWN->m_pSPAWN->m_wDir;
		pMon->m_bCurPathID=0;
		pMon->m_bCurPathDir=1;

		pMon->m_pMAP->OnMove(
			pMon,
			pMon->m_pSPAWN->m_pSPAWN->m_fPosX,
			pMon->m_pSPAWN->m_pSPAWN->m_fPosZ);

		VPLAYER vPLAYERS;
		vPLAYERS.clear();

		pMon->m_pMAP->GetNeighbor(
			&vPLAYERS,
			pMon->m_fPosX,
			pMon->m_fPosZ);

		while(!vPLAYERS.empty())
		{
			CTPlayer *pChar = vPLAYERS.back();

			pChar->SendCS_MONMOVE_ACK(
				pMon->m_dwID,
				pMon->m_bType,
				pMon->m_fPosX,
				pMon->m_fPosY,
				pMon->m_fPosZ,
				pMon->m_wPitch,
				pMon->m_wDIR,
				TKDIR_N,
				TKDIR_N,
				TA_STAND);

			vPLAYERS.pop_back();
		}
		vPLAYERS.clear();
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_SWITCHCHANGE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwSwitch;

	BYTE bChannel;
	WORD wPartyID;
	WORD wMapID;

	pBUF->m_packet
		>> bChannel
		>> wPartyID
		>> wMapID
		>> dwSwitch;

	MAPTCHANNEL::iterator itCH = m_mapTCHANNEL.find(bChannel);
	if( itCH == m_mapTCHANNEL.end() )
		return EC_NOERROR;

	CTMap *pMAP = (*itCH).second->FindTMap(
		wPartyID,
		wMapID);

	if(!pMAP)
		return EC_NOERROR;

	ChangeSwitch(
		pMAP,
		dwSwitch,
		SWC_TOGGLE);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_AICMD_REQ( LPPACKETBUF pBUF)
{
	LPTAIBUF pAI = new TAIBUF();

	pBUF->m_packet
		>> pAI->m_dwCmdHandle
		>> pAI->m_dwHostKEY
		>> pAI->m_dwEventHost
		>> pAI->m_dwRHId
		>> pAI->m_bRHType
		>> pAI->m_dwDelay
		>> pAI->m_dwMonID
		>> pAI->m_bChannel
		>> pAI->m_wMapID
		>> pAI->m_wPartyID;

	m_vAIBUF.push_back(pAI);
	pAI->m_dwTick = 0;

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_GODBALLCMD_REQ( LPPACKETBUF pBUF)
{
	LPTGBBUF pGB = new TGBBUF();

	pBUF->m_packet
		>> pGB->m_wGodBall
		>> pGB->m_bCmd
		>> pGB->m_bChannel
		>> pGB->m_wMapID
		>> pGB->m_wCastle
		>> pGB->m_dwDelay
		>> pGB->m_dwKEY
		>> pGB->m_strCharName;

	m_vGBBUF.push_back(pGB);
	pGB->m_dwTick = 0;

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_GODBALLCMD_ACK(LPPACKETBUF pBUF)
{
	WORD wGodBall;
	BYTE bCmd;
	DWORD dwKey;
	BYTE bChannel;
	WORD wMapID;
	WORD wCastle;
	CString strCharName;

	pBUF->m_packet
		>> wGodBall
		>> bCmd
		>> dwKey
		>> bChannel
		>> wMapID
		>> wCastle
		>> strCharName;

	CTMap * pMap = FindTMap(bChannel, 0, wMapID);
	if(!pMap || !pMap->m_dwEndTick)
		return EC_NOERROR;

	LPTGODBALL pBall = pMap->FindGodBall(wGodBall);
	if(wGodBall && !pBall)
		return EC_NOERROR;

	switch(bCmd)
	{
	case GB_ENDWAR:

		break;
	case GB_STARTWAR:
		if(pMap->CheckPower())
			SendSM_GODBALLCMD_REQ(wGodBall, bCmd, bChannel, wMapID, wCastle, m_dwGBDelay[bCmd], dwKey);
		else
		{
			LPTLOCAL pCastle = FindCastleZone(wCastle);
			if(pCastle)
				EndWar(pCastle, pMap, WIN_POWER);
		}
		break;
	case GB_MOUNTBALL:
		if(pBall->m_dwKEY == dwKey)
			pMap->InitGodBall(pBall->m_bCamp, pBall->m_dwGuildID, wCastle);
		break;
	case GB_DEMOUNTBALL:
	case GB_DROPBALL:
		if(pBall->m_dwKEY == dwKey)
			pMap->MoveGodBall(pBall);
		break;
	case GB_TAKEBALL:
		if(pBall->m_dwKEY == dwKey)
		{
			CTPlayer * pPlayer = FindPlayer(strCharName);
			if(pPlayer)
			{
				pPlayer->RemoveGodBall(pBall);
				pMap->MoveGodBall(pBall);
			}
		}
		break;
	default:
		break;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_SWITCHSTART_REQ(LPPACKETBUF pBUF)
{
	TSWITCHOBJ sw;

	sw.m_dwTick = 0;

	pBUF->m_packet
		>> sw.m_bChannel
		>> sw.m_wPartyID
		>> sw.m_wMapID
		>> sw.m_dwID
		>> sw.m_dwDelay;
	m_vTSWITCHOBJ.push_back(sw);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_POSTBILL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwPostID;
	__int64 timeDel;

	pBUF->m_packet
		>> dwCharID
		>> dwPostID
		>> timeDel;

	MAPDWORD64::iterator itPB = m_mapTPostBills.find(dwPostID);
	if(itPB == m_mapTPostBills.end())
		m_mapTPostBills.insert(MAPDWORD64::value_type(dwPostID, timeDel));

	MAPDVDWORD::iterator it = m_mapCharPostBills.find(dwCharID);
	if(it==m_mapCharPostBills.end())
	{
		VDWORD vBill;
		vBill.push_back(dwPostID);
		m_mapCharPostBills.insert(MAPDVDWORD::value_type(dwCharID, vBill));
	}
	else
		(*it).second.push_back(dwPostID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_POSTBILLERASE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwPostID;

	pBUF->m_packet
		>> dwCharID
		>> dwPostID;

	if(dwPostID)
		m_mapTPostBills.erase(dwPostID);
	else
	{
		MAPDVDWORD::iterator it = m_mapCharPostBills.find(dwCharID);
		if(it!=m_mapCharPostBills.end())
		{
			while(!(*it).second.empty())
			{
				DWORD dwPostID = (*it).second.back();
				m_mapTPostBills.erase(dwPostID);
				(*it).second.pop_back();
			}

			(*it).second.clear();

			m_mapCharPostBills.erase(it);
		}
	}

	return EC_NOERROR;
}


DWORD CTMapSvrModule::OnSM_AUCTIONCMD_REQ(LPPACKETBUF pBUF)
{
	LPTAUCTIONBUF pAuctionBUF = new TAUCTIONBUF();

	pBUF->m_packet
		>> pAuctionBUF->m_wNpcID
		>> pAuctionBUF->m_dwAuctionID
		>> pAuctionBUF->m_dEndTime;

	VTAUCTIONBUF::iterator itAU;
	for(itAU = m_vTAUCTIONBUF.begin(); itAU != m_vTAUCTIONBUF.end(); itAU++)
		if( pAuctionBUF->m_dEndTime < (*itAU)->m_dEndTime )
			break;

	m_vTAUCTIONBUF.insert(itAU,pAuctionBUF);	
	
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_AUCTIONCMDDEL_REQ(LPPACKETBUF pBUF)
{
	WORD wNpcID;
	DWORD dwAuctionID;

	pBUF->m_packet
		>> wNpcID
		>> dwAuctionID;

	VTAUCTIONBUF::iterator itAU;
	for(itAU = m_vTAUCTIONBUF.begin(); itAU != m_vTAUCTIONBUF.end(); itAU++)
	{
		if( (*itAU)->m_dwAuctionID == dwAuctionID )
		{
			delete (*itAU);
			m_vTAUCTIONBUF.erase(itAU);
			break;
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_POSTBILL_ACK(LPPACKETBUF pBUF)
{
	DWORD dwPostID;

	pBUF->m_packet
		>> dwPostID;

	SendDM_POSTBILLUPDATE_REQ(0, NAME_NULL, dwPostID, POST_RETURN);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTBILLUPDATE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	CString strChar;
	DWORD dwPostID;
	BYTE bType;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	pBUF->m_packet
		>> dwCharID
		>> strChar
		>> dwPostID
		>> bType
		>> dwGold
		>> dwSilver
		>> dwCooper;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_POSTBILLUPDATE_ACK)
		<< dwCharID
		<< strChar
		<< dwPostID
		<< bType
		<< dwGold
		<< dwSilver
		<< dwCooper;

	DEFINE_QUERY(&m_db, CSPPostBillsUpdate)
	lstrcpy(query->m_szCharName, strChar);
	query->m_dwPostID = dwPostID;
	query->m_bType = bType;
	if(query->Call())
	{
		pMSG->m_packet
			<< query->m_nRet
			<< query->m_dwNewID
			<< query->m_szRecvName
			<< query->m_szSender
			<< query->m_szTitle;
	}
	else
	{
		pMSG->m_packet
			<< INT(POST_NOTFOUND);
	}
	UNDEFINE_QUERY()

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTBILLUPDATE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	CString strChar;
	DWORD dwPostID;
	DWORD dwNewID;
	INT nRet;
	BYTE bType;
	CString strRecvName;
	CString strSender;
	CString strTitle;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	pBUF->m_packet
		>> dwCharID
		>> strChar
		>> dwPostID
		>> bType
		>> dwGold
		>> dwSilver
		>> dwCooper
		>> nRet;

	if(nRet)
	{
		CTPlayer * pPlayer = FindPlayer(strChar);
		if(pPlayer)
			pPlayer->SendCS_POSTRETURN_ACK(nRet);

		if(bType == POST_PAYMENT)
		{
			SendDM_POSTRECV_REQ(
				0, dwCharID, GetSvrMsg(NAME_OPERATOR), strChar,
				BuildNetString(NAME_NULL, GetSvrMsg(MSG_ITEMERROR_TITLE)),
				BuildNetString(NAME_NULL, GetSvrMsg(MSG_ITEMERROR_MESSAGE)),
				POST_PACKATE,
				dwGold,dwSilver,dwCooper, NULL);
		}

		return EC_NOERROR;
	}

	pBUF->m_packet
		>> dwNewID
		>> strRecvName
		>> strSender
		>> strTitle;

	PostReturn(
		strChar,
		dwPostID,
		dwNewID,
		strRecvName,
		bType,
		strSender,
		strTitle);

	return EC_NOERROR;

}

DWORD CTMapSvrModule::OnSM_DUELSTART_REQ(LPPACKETBUF pBUF)
{
	DWORD dwDuelID;
	DWORD dwInviter;
	CString strInviter;
	DWORD dwTarget;
	CString strTarget;

	pBUF->m_packet
		>> dwDuelID
		>> dwInviter
		>> strInviter
		>> dwTarget
		>> strTarget;

	LPTDUEL pTDUEL = new TDUEL();
	pTDUEL->m_dwDuelID = dwDuelID;
	pTDUEL->m_bType = DUEL_STANDBY;
	pTDUEL->m_dwDuelTick = (DWORD)m_timeCurrent + DUEL_STANDBY_TIME;
	pTDUEL->m_dwInviter = dwInviter;
	pTDUEL->m_strInviter = strInviter;
	pTDUEL->m_dwTarget = dwTarget;
	pTDUEL->m_strTarget = strTarget;

	m_mapTDuel.insert(MAPTDUEL::value_type(pTDUEL->m_dwDuelID, pTDUEL));

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnSM_DUELSTART_ACK(LPPACKETBUF pBUF)
{
	DWORD dwDuelID;
	DWORD dwInviter;
	DWORD dwTarget;

	pBUF->m_packet
		>> dwDuelID
		>> dwInviter
		>> dwTarget;

	CTPlayer * pInviter = NULL;
	CTPlayer * pTarget = NULL;

	MAPPLAYER::iterator it = m_mapPLAYER.find(dwInviter);
	if(it == m_mapPLAYER.end())
	{
		SendSM_DUELEND_REQ(dwDuelID,0);
		return EC_NOERROR;
	}
	pInviter = (*it).second;

	it = m_mapPLAYER.find(dwTarget);
	if(it == m_mapPLAYER.end())
	{
		SendSM_DUELEND_REQ(dwDuelID,0);
		return EC_NOERROR;
	}
	pTarget = (*it).second;

	pInviter->SendCS_DUELSTART_ACK(DUEL_SUCCESS, dwInviter, dwTarget);
	pTarget->SendCS_DUELSTART_ACK(DUEL_SUCCESS, dwInviter, dwTarget);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnSM_DUELEND_REQ(LPPACKETBUF pBUF)
{
	DWORD dwDuelID;
	DWORD dwLoser;

	pBUF->m_packet
		>> dwDuelID
		>> dwLoser;

	MAPTDUEL::iterator find = m_mapTDuel.find(dwDuelID);
	if(find!=m_mapTDuel.end() && (*find).second->m_bType != DUEL_END)
	{
		CString strLoser = NAME_NULL;
		if((*find).second->m_dwInviter == dwLoser)
			strLoser = (*find).second->m_strInviter;
		if((*find).second->m_dwTarget == dwLoser)
			strLoser = (*find).second->m_strTarget;

		if(strLoser)
		{
			(*find).second->m_bType = DUEL_END;
			(*find).second->m_dwDuelTick = (DWORD)m_timeCurrent + DUEL_STANDBY_TIME;
		}
		else
		{
			delete (*find).second;
			m_mapTDuel.erase(dwDuelID);
			dwLoser = 0;
		}

		LPPACKETBUF pMSG = new PACKETBUF();
		pMSG->m_packet.SetID(SM_DUELEND_ACK)
			<< (*find).second->m_dwInviter
			<< (*find).second->m_dwTarget
			<< dwLoser
			<< strLoser;

		SayToBATCH(pMSG);
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnSM_DUELEND_ACK(LPPACKETBUF pBUF)
{
	DWORD dwInviter;
	DWORD dwTarget;
	DWORD dwLoser;
	CString strLoser;

	pBUF->m_packet
		>> dwInviter
		>> dwTarget
		>> dwLoser
		>> strLoser;

	CTPlayer * pInviter = NULL;
	CTPlayer * pTarget = NULL;
	CTPlayer * pWinner = NULL;
	CTPlayer * pLoser = NULL;

	MAPPLAYER::iterator find = m_mapPLAYER.find(dwInviter);
	if(find != m_mapPLAYER.end())
		pInviter = (*find).second;

	find = m_mapPLAYER.find(dwTarget);
	if(find != m_mapPLAYER.end())
		pTarget = (*find).second;

	if(pInviter)
	{
		if(dwLoser && pInviter->m_dwDuelTarget == dwLoser)
		{
			pWinner = pInviter;
			pLoser = pTarget;
		}
		
		pInviter->m_bDuelType = DUEL_END;
		pInviter->EraseDieSkillBuff();
		pInviter->DeleteNegativeMaintainSkill();
		pInviter->SendCS_DUELEND_ACK(dwLoser);
	}

	if(pTarget)
	{
		if(dwLoser && pTarget->m_dwDuelTarget == dwLoser)
		{
			pWinner = pTarget;
			pLoser = pInviter;
		}

		pTarget->m_bDuelType = DUEL_END;
		pTarget->EraseDieSkillBuff();
		pTarget->DeleteNegativeMaintainSkill();
		pTarget->SendCS_DUELEND_ACK(dwLoser);
	}

	if(pWinner && pLoser)
	{		
		pWinner->RecordDuel(pLoser,TRUE,0);
		pLoser->RecordDuel(pWinner,FALSE,0);
	}

	if(pWinner && pWinner->m_pMAP)
	{
		VPLAYER vPlayer;
		vPlayer.clear();
		pWinner->m_pMAP->GetNeighbor(&vPlayer, pWinner->m_fPosX, pWinner->m_fPosZ);

		while(!vPlayer.empty())
		{
			CTPlayer * pChar = vPlayer.back();
			vPlayer.pop_back();
			pChar->SendCS_SYSTEMMSG_ACK(
				SM_DUAL_WIN,
				0,
				0,
				pWinner->m_strNAME,
				strLoser);
		}
	}

	if(!dwLoser)
	{
		if(pInviter)
			pInviter->ClearDuel();
		if(pTarget)
			pTarget->ClearDuel();
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnSM_DUELCOMPLETE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwInviter;
	DWORD dwTarget;

	pBUF->m_packet
		>> dwInviter
		>> dwTarget;

	CTPlayer * pInviter = NULL;
	CTPlayer * pTarget = NULL;

	MAPPLAYER::iterator find = m_mapPLAYER.find(dwInviter);
	if(find != m_mapPLAYER.end())
		pInviter = (*find).second;

	find = m_mapPLAYER.find(dwTarget);
	if(find != m_mapPLAYER.end())
		pTarget = (*find).second;

	if(pInviter)
		pInviter->ClearDuel();
	if(pTarget)
		pTarget->ClearDuel();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_AICMD_ACK( LPPACKETBUF pBUF)
{
	DWORD_PTR dwHandle;
	DWORD dwEventHost;
	DWORD dwRHId;
	BYTE bRHType;
	DWORD dwHostKEY;
	DWORD dwMonID;

	BYTE bChannel;
	WORD wMapID;
	WORD wPartyID;

	pBUF->m_packet
		>> dwHandle
		>> dwHostKEY
		>> dwEventHost
		>> dwRHId
		>> bRHType
		>> dwMonID
		>> bChannel
		>> wMapID
		>> wPartyID;

	MAPTCHANNEL::iterator itCH = m_mapTCHANNEL.find(bChannel);
	if( itCH == m_mapTCHANNEL.end() )
		return EC_NOERROR;

	CTMap *pMAP = (*itCH).second->FindTMap(
		wPartyID,
		wMapID);

	if(!pMAP)
		return EC_NOERROR;

	CTAICommand *pCMD = (CTAICommand *) dwHandle;
	CTMonster *pMON = NULL;

	MAPTMONSTER::iterator finder = pMAP->m_mapTMONSTER.find(dwMonID);
	if( finder != pMAP->m_mapTMONSTER.end() )
		pMON = (*finder).second;

	if( !pMON || !pCMD )
		return EC_NOERROR;

	if( pMON->m_dwHostKEY == dwHostKEY &&
		pCMD->CanRun( pMON, dwEventHost, dwRHId, bRHType) &&
        pCMD->ExecAI( pMON, dwEventHost, dwRHId, bRHType))
		DoAICMD( pCMD, pMON, dwEventHost, dwRHId, bRHType, 0);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_DELCHAR_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bLogout;
	BYTE bSave;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bLogout
		>> bSave;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	ExitMAP(pPlayer);

	pPlayer->m_bLogout = bLogout;
	pPlayer->m_bExit = TRUE;

	if(!pPlayer->m_bCanDelete)
	{
		pPlayer->m_bCloseAll = FALSE;
		CloseSession(pPlayer);
	}
	else
		SetEventCloseSession(pPlayer, bSave && pPlayer->m_bMain);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_INVALIDCHAR_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bReleaseMain;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bReleaseMain;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	if(bReleaseMain)
		SendDM_SAVECHAR_REQ(pPlayer, 0);

	pPlayer->SendCS_INVALIDCHAR_ACK();
	CloseSession(pPlayer);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_ENTERSOLOMAP_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	pBUF->m_packet
		>> pPlayer->m_wPartyID
		>> pPlayer->m_bPartyType
		>> pPlayer->m_dwPartyChiefID;

	if(!pPlayer->m_pMAP)
		InitMap(pPlayer);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CHECKMAIN_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bChannel;
	WORD wMapID;
	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	if(IsMainCell(bChannel, wMapID, fPosX, fPosZ))
		SendMW_CHECKMAIN_ACK( pPlayer->m_dwID, pPlayer->m_dwKEY);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CONRESULT_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bResult;
	BYTE bCount;
	BYTE bServerID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult
		>> bCount;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	VBYTE vServerID;
	vServerID.clear();
	for(BYTE i=0; i<bCount; i++)
	{
		pBUF->m_packet
			>> bServerID;

		vServerID.push_back(bServerID);
	}

	if(bResult == CN_SUCCESS && !pPlayer->InitCharInfo())
	{
		LogEvent("Tutorial Error %d, %d, %d, %d, %d, %d", pPlayer->m_dwID, pPlayer->m_wMapID, pPlayer->m_bStartAct, pPlayer->m_dwGold, pPlayer->m_dwEXP, pPlayer->m_bLevel);
		CloseSession(pPlayer);
		return EC_NOERROR;
	}

	pPlayer->SendCS_CONNECT_ACK(bResult, &vServerID);

	if( bResult == CN_SUCCESS )
		pPlayer->m_bMain = TRUE;
	else
		CloseSession(pPlayer);

	vServerID.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_TELEPORT_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bChannel;
	WORD wMapID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> bResult;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_TELEPORT_ACK(
		bResult,
		0,
		pPlayer->m_dwID,
		pPlayer->m_bType,
		wMapID,
		fPosX,
		fPosY,
		fPosZ);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CHARDATA_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	SendMW_CHARDATA_ACK(pPlayer);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_ENTERCHAR_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	pBUF->m_packet
		>> pPlayer->m_bStartAct
		>> pPlayer->m_strNAME
		>> pPlayer->m_wMapID
		>> pPlayer->m_fPosX
		>> pPlayer->m_fPosY
		>> pPlayer->m_fPosZ
		>> pPlayer->m_dwGuildID
		>> pPlayer->m_dwFame
		>> pPlayer->m_dwFameColor
		>> pPlayer->m_strGuildName
		>> pPlayer->m_bGuildDuty
		>> pPlayer->m_bGuildPeer
		>> pPlayer->m_wCastle
		>> pPlayer->m_bCamp
		>> pPlayer->m_dwTacticsID
		>> pPlayer->m_strTacticsName
		>> pPlayer->m_wPartyID
		>> pPlayer->m_bPartyType
		>> pPlayer->m_dwPartyChiefID
		>> pPlayer->m_wCommanderID
		>> pPlayer->m_bLevel
		>> pPlayer->m_bHelmetHide
		>> pPlayer->m_bCountry
		>> pPlayer->m_bAidCountry
		>> pPlayer->m_bMode
		>> pPlayer->m_dwRiding
		>> pPlayer->m_nChatBanTime  
		>> pPlayer->m_dwSoulmate
		>> pPlayer->m_dwSoulSilence
		>> pPlayer->m_strSoulmate
		>> pPlayer->m_bClass
		>> bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		CString strName;

		DWORD dwMonID;
		DWORD dwTime;
		DWORD dwMaxHP;
		DWORD dwMaxMP;
		DWORD dwHP;
		DWORD dwMP;

		WORD wPetID;
		WORD wMon;
		DWORD dwATTR;
		WORD wDir;
		BYTE bHit;
		BYTE bSkillLevel;

		FLOAT fPosX;
		FLOAT fPosY;
		FLOAT fPosZ;

		BYTE bLevel;
		BYTE bClass;
		BYTE bRace;
		BYTE bAction;
		BYTE bStatus;
		BYTE bMode;
		BYTE bSkillCount;
		WORD wSkillID;

		pBUF->m_packet
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

		VWORD vSkill;
		vSkill.clear();
		for(BYTE i=0; i<bSkillCount; i++)
		{
			pBUF->m_packet
				>> wSkillID;

			vSkill.push_back(wSkillID);
		}

		LPTMONSTER pTemp = FindTMonster(wMon);
		if(pTemp)
		{
			CTRecallMon *pMON = pPlayer->FindRecallMon(dwMonID);

			if(!pMON)
			{
				pMON = CreateRecallMon(
					pPlayer,
					pTemp,
					dwATTR,
					dwMonID,
					wPetID,
					strName,
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
					0,0,0,0,0,
					bSkillLevel,
					fPosX,
					fPosY,
					fPosZ,
					wDir,
					dwTime,
					vSkill);
			}
			else
			{
				pMON->m_bCountry = pPlayer->m_bCountry;
				pMON->m_bAidCountry = pPlayer->m_bAidCountry;
				pMON->m_dwHostID = dwCharID;
			}
		}

		vSkill.clear();
	}

	pBUF->m_packet
		>> pPlayer->m_strComment;

	pPlayer->m_dwRecoverHPTick = m_dwTick + RECOVER_INIT;
	pPlayer->m_dwRecoverMPTick = m_dwTick + RECOVER_INIT;

	SendMW_ENTERCHAR_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CREATERECALLMON_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
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

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
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

	VWORD vSkill;
	vSkill.clear();
	for(BYTE ms=0; ms<bSkillCount; ms++)
	{
		pBUF->m_packet
			>> wSkillID;

		vSkill.push_back(wSkillID);
	}

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	if(pPlayer->m_bMain && pPlayer->m_bStatus == OS_DEAD)
	{
		if(wPetID)
			pPlayer->SendCS_PETRECALL_ACK(PET_FAIL);

		SendMW_RECALLMONDEL_ACK(dwCharID, dwKEY, dwMonID);
		return EC_NOERROR;
	}

	LPTMONSTER pTemp = FindTMonster(wMon);
	if(!pTemp)
		return EC_NOERROR;

	if(pTemp->m_bIsSelf && pPlayer->m_pMAP)
	{
		WORD wPosX = WORD(fPosX) / CELL_SIZE;
		WORD wPosZ = WORD(fPosZ) / CELL_SIZE;
		MAPTCELL::iterator itCELL = pPlayer->m_pMAP->m_mapTCELL.find( MAKELONG( wPosX, wPosZ));

		if( itCELL == pPlayer->m_pMAP->m_mapTCELL.end() ||
			!(*itCELL).second->IsMainCell(pPlayer->m_bChannel) )
			return EC_NOERROR;
	}

	pPlayer->CheckRecallMon(pTemp, FALSE);

	CTRecallMon *pMON = CreateRecallMon(
		pPlayer,
		pTemp,
		dwATTR,
		dwMonID,
		wPetID,
		strName,
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
		0,0,0,0,0,
		bSkillLevel,
		fPosX,
		fPosY,
		fPosZ,
		wDir,
		dwTime,
		vSkill);

	if(pMON && pPlayer->m_pMAP)
		pPlayer->m_pMAP->EnterMAP(pMON);

	vSkill.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_RECALLMONDEL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwMonID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwMonID;

	MAPPLAYER::iterator it = m_mapPLAYER.find(dwCharID);
	if(it == m_mapPLAYER.end())
		return EC_NOERROR;

	CTPlayer * pPlayer = it->second;

	BYTE bMain = FALSE;
	CTRecallMon * pMON = pPlayer->FindRecallMon(dwMonID);
	if(pMON)
	{
		bMain = pMON->m_bMain;
		pPlayer->DeleteRecallMon(dwMonID);
	}

	if(pPlayer->m_bMain && !bMain)
		pPlayer->SendCS_DELRECALLMON_ACK(pPlayer->m_dwID, dwMonID, TRUE);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CHARINFO_REQ( LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwTacticsID;
	DWORD dwFame;
	DWORD dwFameColor;
	BYTE bDuty;
	BYTE bPeer;
	CString strGuildName;
	CString strTacticsName;
	WORD wPartyID;

	DWORD dwChiefID;
	DWORD dwCharID;
	DWORD dwKEY;

	WORD wCastle;
	BYTE bCamp;

	BYTE bPartyType;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwGuildID
		>> strGuildName
		>> dwFame
		>> dwFameColor
		>> dwTacticsID
		>> strTacticsName
		>> bDuty
		>> bPeer
		>> wCastle
		>> bCamp
		>> wPartyID
		>> bPartyType
		>> dwChiefID;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->m_dwGuildID = dwGuildID;
	pPlayer->m_strGuildName = strGuildName;
	pPlayer->m_dwFame = dwFame;
	pPlayer->m_dwFameColor = dwFameColor;
	pPlayer->m_bGuildDuty = bDuty;
	pPlayer->m_bGuildPeer = bPeer;
	pPlayer->m_dwTacticsID = dwTacticsID;
	pPlayer->m_strTacticsName = strTacticsName;
	pPlayer->m_wPartyID = wPartyID;
	pPlayer->m_bPartyType = bPartyType;
	pPlayer->m_dwPartyChiefID = dwChiefID;
	pPlayer->m_wCastle = wCastle;
	pPlayer->m_bCamp = bCamp;

	pPlayer->SendCS_QUESTLIST_ACK(m_dwTick); //CHARINFO_ACK 이전에 보내야 함
	pPlayer->SendQuestTimer(m_dwTick);

	pPlayer->SendCS_CHARINFO_ACK(
		m_dwTick,
		pPlayer->m_pTPREVLEVEL->m_dwEXP,
		pPlayer->m_pTLEVEL->m_dwEXP);

	pPlayer->SendCS_CHARSTATINFO_ACK(pPlayer);
	pPlayer->SendCS_PETLIST_ACK();
	pPlayer->SendCS_POSTINFO_ACK();
	pPlayer->SendCS_PROTECTEDLIST_ACK();
	if(m_bRelayOn)
		pPlayer->SendCS_RELAYCONNECT_ACK(m_addrRelay.sin_addr.s_addr, m_addrRelay.sin_port);

	BYTE bRankMonth = m_bRankMonth - 1;
	if(bRankMonth == 0)
		bRankMonth = 12;

	pPlayer->SendCS_UPDATEFAMERANKLIST_ACK(m_arFameRank, bRankMonth);

	CString strHelpMessage;
	BYTE bHelpID = GetHelpMessage(pPlayer, strHelpMessage);
	if(bHelpID)
		pPlayer->SendCS_HELPMESSAGE_ACK(bHelpID, strHelpMessage);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CLOSECHAR_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	if(pPlayer->m_pMAP)
		ExitMAP(pPlayer);

	pPlayer->m_bCloseAll = FALSE;
	pPlayer->m_bExit = TRUE;
	pPlayer->SendCS_SHUTDOWN_ACK();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CHATBAN_REQ( LPPACKETBUF pBUF)  
{
	CString strName;
	__int64 nChatBanTime;
	BYTE bResult;
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> strName
		>> nChatBanTime
		>> bResult
		>> dwCharID
		>> dwKEY;

	strName.MakeUpper();

	MAPPLAYERNAME::iterator finder = m_mapPLAYERNAME.find(strName);
	if( finder != m_mapPLAYERNAME.end())
	{
		if( !(*finder).second->m_pMAP || !(*finder).second->m_bMain )
			return EC_NOERROR;

		(*finder).second->m_nChatBanTime = nChatBanTime;			
	}

	if(dwCharID)
	{
		CTPlayer* pPlayer  = FindChar(dwCharID);
		pPlayer->SendCS_CHATBAN_ACK(bResult);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_EVENTQUARTER_REQ(LPPACKETBUF pBUF)
{
	CString strName;
	BYTE bDay;
	BYTE bHour;
	BYTE bMinute;

	pBUF->m_packet
		>> strName
		>> bDay
		>> bHour
		>> bMinute;

	DEFINE_QUERY(&m_db, CSPEventQuarter)
	lstrcpy(query->m_szName, strName);
	query->m_bDay = bDay;
	query->m_bHour = bHour;
	query->m_bMinute = bMinute;
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_SMSSEND_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet = SMS_SUCCESS;

	DWORD dwCharID;
	DWORD dwKey;
	BYTE bInven;
	BYTE bItem;
	WORD wItemID;
	BYTE bType;
	DWORD dwGuildID;
	CString strTarget;
	CString strMsg;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bInven
		>> bItem
		>> wItemID
		>> bType
		>> dwGuildID
		>> strTarget
		>> strMsg;

	DEFINE_QUERY(&m_db, CSPSaveSMS)
	query->m_bWorld = m_bGroupID;
	query->m_bType = bType;
	query->m_dwSenderID = dwCharID;
	query->m_dwGuildID = dwGuildID;
	lstrcpy(query->m_szTarget, strTarget);
	lstrcpy(query->m_szMsg, strMsg);
	if(!query->Call())
		bRet = SMS_INTERNAL;
	else
		bRet = BYTE(query->m_nRET);
	UNDEFINE_QUERY();

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_SMSSEND_ACK)
		<< bRet
		<< dwCharID
		<< dwKey
        << bInven
        << bItem
		<< wItemID
		<< bType
		<< strTarget
		<< strMsg;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_SMSSEND_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bInven;
	BYTE bItem;
	WORD wItemID;
	BYTE bType;
	CString strTarget;
	CString strMsg;

	pBUF->m_packet
		>> bRet
		>> dwCharID
		>> dwKey
		>> bInven
		>> bItem
		>> wItemID
		>> bType
		>> strTarget
		>> strMsg;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	if(bRet)
	{
		LPTITEM pTITEM = FindTItem(wItemID);
		if(pTITEM)
		{	
			VTITEM vItem;
			vItem.clear();
			CTItem * pNew = new CTItem();
			pNew->m_dlID = GenItemID();
			pNew->m_bCount = 1;
			pNew->m_wItemID = wItemID;
			pNew->m_pTITEM = pTITEM;
			pNew->SetDuration(FALSE);
			SetItemAttr(pNew, 0);
			vItem.push_back(pNew);
			if(pPlayer->CanPush(&vItem, 0))
			{
				pPlayer->PushTItem(&vItem);
				pPlayer->SendCS_MOVEITEM_ACK(MI_SUCCESS);
			}
		}
	}

	pPlayer->SendCS_SMSSEND_ACK(
		bRet,
		bType,
		strTarget);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_RECALLMONDEL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwMonID;

	pBUF->m_packet
		>> dwCharID
		>> dwMonID;

	DEFINE_QUERY(&m_db, CSPRecallmonDel)
	query->m_dwCharID = dwCharID;
	query->m_dwMonID = dwMonID;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CREATERECALLMON_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	WORD wMonID;
	DWORD dwATTR;
	WORD wPetID;
	CString strName;
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
	DWORD dwTime;
	BYTE bSkillCount;
	WORD wSkillID;
	BYTE bSkillLevel;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wMonID
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

	VWORD vSkill;
	vSkill.clear();
	for(BYTE i=0; i<bSkillCount; i++)
	{
		pBUF->m_packet
			>> wSkillID;

		vSkill.push_back(wSkillID);
	}

	DWORD dwMonID = 0;
	int nRet=0;
	DEFINE_QUERY(&m_db, CSPCreateRecallMon)
	query->m_dwCharID = dwCharID;
	query->m_wTempID = wMonID;
	query->m_wPetID = wPetID;
	query->m_dwATTR = dwATTR;
	query->m_bLevel = bLevel;
	query->m_dwHP = dwHP;
	query->m_dwMP = dwMP;
	query->m_bSkillLevel = bSkillLevel;
	query->m_wPosX = WORD(fPosX);
	query->m_wPosY = WORD(fPosY);
	query->m_wPosZ = WORD(fPosZ);
	query->m_dwTime = dwTime;
	if(query->Call())
	{
		dwMonID = query->m_dwMonID;
		nRet = query->m_nRET;
	}
	else
		nRet = 0xFF;
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_CREATERECALLMON_ACK)
		<< nRet
		<< dwCharID
		<< dwKey
		<< dwMonID
		<< wMonID
		<< dwATTR
		<< wPetID
		<< strName
		<< bLevel
		<< bClass
		<< bRace
		<< bAction
		<< bStatus
		<< bMode
		<< dwMaxHP
		<< dwMaxMP
		<< dwHP
		<< dwMP
		<< bHit
		<< bSkillLevel
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wDir
		<< dwTime
		<< bSkillCount;

	for(BYTE k=0; k<bSkillCount; k++)
	{
		pMSG->m_packet
			<< vSkill[k];
	}

	SayToBATCH(pMSG);

	vSkill.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CREATERECALLMON_ACK(LPPACKETBUF pBUF)
{
	int nRet;
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
		>> nRet
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

	for(BYTE i=0; i<bSkillCount; i++)
	{
		pBUF->m_packet
			>> wSkillID;

		vSkill.push_back(wSkillID);
	}

	if(nRet)
	{
		vSkill.clear();
		return EC_NOERROR;
	}

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
	{
		LPTMONSTER pTemp = FindTMonster(wMon);
		if(!pTemp)
		{
			vSkill.clear();
			return EC_NOERROR;
		}

		SendMW_CREATERECALLMON_ACK(
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

	vSkill.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CLEARCURRENTUSER_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;

	pBUF->m_packet
		>> dwCharID;

	DEFINE_QUERY(&m_db, CSPClearCurrentUser)
	query->m_dwCharID = dwCharID;
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CLEARCURRENTUSER_ACK(LPPACKETBUF pBUF)
{
	DWORD ptrSession;

	pBUF->m_packet
		>> ptrSession;

	MAPPLAYER::iterator it = m_mapSESSION.find(ptrSession);
	if(it!=m_mapSESSION.end())
	{
		CloseSession((*it).second);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CASTLEOCCUPY_REQ( LPPACKETBUF pBUF)
{
	WORD wCastleID;
	BYTE bType;
	int nRet;

	DWORD dwWinGuildID;
	DWORD dwLoseGuild;
	BYTE bCountry;

	DEFINE_QUERY( &m_db, CSPSaveCastleOccupy)
	pBUF->m_packet
		>> wCastleID
		>> bType
		>> dwWinGuildID
		>> dwLoseGuild;

	query->m_wCastleID = wCastleID;
	query->m_bType = bType;
	query->m_dwGuildID = dwWinGuildID;

	if(!query->Call())
		nRet = CASTLE_INTERNAL;
	else
		nRet = query->m_nRET;

	bCountry = query->m_bCountry;

	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_CASTLEOCCUPY_ACK)
		<< nRet
		<< wCastleID
		<< bType
		<< dwWinGuildID
		<< bCountry
		<< dwLoseGuild;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnDM_CASTLEOCCUPY_ACK(LPPACKETBUF pBUF)
{
	int nRet;
	WORD wCastleID;
	BYTE bType;
	DWORD dwWinGuildID;
	DWORD dwLoseGuild;
	BYTE bCountry;
	CString strGuildName;

	pBUF->m_packet
		>> nRet
		>> wCastleID
		>> bType
		>> dwWinGuildID
		>> bCountry
		>> dwLoseGuild;

	if(!nRet)
		SendMW_CASTLEOCCUPY_ACK(bType, wCastleID, dwWinGuildID, bCountry, dwLoseGuild);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnDM_LOCALOCCUPY_REQ( LPPACKETBUF pBUF) 
{
	WORD wLocalID;
	BYTE bType;
	DWORD dwOccupiedGuild;
	DWORD dwOccupiedChar;
	BYTE bCountry;
	DWORD dwGuild;
	int nRet;

	DEFINE_QUERY( &m_db, CSPSaveLocalOccupy)
	pBUF->m_packet
		>> wLocalID
		>> bType
		>> dwOccupiedGuild
		>> dwOccupiedChar;

	query->m_wLocalID = wLocalID;
	query->m_bType = bType;
	query->m_dwOccupiedGuild = dwOccupiedGuild;
	query->m_dwOccupiedChar =dwOccupiedChar;

	if(!query->Call())
		nRet = LOCCUPY_INTERNAL;
	else
		nRet = query->m_nRET;

	bCountry = query->m_bCountry;
	dwGuild = query->m_dwGuild;

	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_LOCALOCCUPY_ACK)
		<< nRet
		<< wLocalID
		<< bType
		<< bCountry
		<< dwGuild;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnDM_LOCALOCCUPY_ACK(LPPACKETBUF pBUF)
{
	int nRet;
	WORD wLocalID;
	BYTE bType;
	BYTE bCountry;
	DWORD dwGuildID;

	pBUF->m_packet
		>> nRet
		>> wLocalID
		>> bType
		>> bCountry
		>> dwGuildID;

	if(!nRet)
	{
		LPTLOCAL pLocal = FindOccupationZone(wLocalID);
		if(!pLocal)
			return EC_NOERROR;

		CTMap *pTMAP = FindTMap(DEFAULT_CHANNEL, 0, pLocal->m_pZone->m_wMapID);

		if(pTMAP)
		{
			for( DWORD i=0; i<pLocal->m_pZone->m_vSpawnID.size(); i++)
			{
				MAPTMONSPAWN::iterator it = pTMAP->m_mapTMONSPAWN.find(pLocal->m_pZone->m_vSpawnID[i]);

				if( it != pTMAP->m_mapTMONSPAWN.end() )
				{
					CTMonSpawn *pSpawn = (*it).second;

					for( DWORD j=0; j<pSpawn->m_vTMON.size(); j++)
					{
						pSpawn->m_vTMON[j]->LeaveAggro(
							pSpawn->m_vTMON[j]->m_dwHostID,
							pSpawn->m_vTMON[j]->m_dwTargetID,
							pSpawn->m_vTMON[j]->m_bTargetType);

						pSpawn->m_vTMON[j]->m_bCountry = bCountry;

						VPLAYER vPLAYERS;
						vPLAYERS.clear();

						pTMAP->GetNeighbor(
							&vPLAYERS,
							pSpawn->m_vTMON[j]->m_fPosX,
							pSpawn->m_vTMON[j]->m_fPosZ);

						while(!vPLAYERS.empty())
						{
							CTPlayer *pChar = vPLAYERS.back();

							pChar->SendCS_CHANGECOLOR_ACK(
								pSpawn->m_vTMON[j]->m_dwID,
								OT_MON,
								pSpawn->m_vTMON[j]->GetColor( pChar->m_dwID, pChar->GetPartyID(), pChar->GetWarCountry()),
								bCountry);

							vPLAYERS.pop_back();
						}

						vPLAYERS.clear();
					}
				}
			}
		}

		SendMW_LOCALOCCUPY_ACK(bType, wLocalID, bCountry, dwGuildID, pLocal->m_bCountry);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_MISSIONOCCUPY_REQ( LPPACKETBUF pBUF) 
{
	WORD wLocalID;
	BYTE bType;
	BYTE bCountry;
	DWORD dwOccupiedChar;
	int nRet;

	DEFINE_QUERY( &m_db, CSPSaveMissionOccupy)
	pBUF->m_packet
		>> wLocalID
		>> bType
		>> bCountry
		>> dwOccupiedChar;

	query->m_wLocalID = wLocalID;
	query->m_bType = bType;
	query->m_bCountry = bCountry;
	query->m_dwOccupiedChar =dwOccupiedChar;

	if(!query->Call())
		nRet = LOCCUPY_INTERNAL;
	else
		nRet = query->m_nRET;

	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_MISSIONOCCUPY_ACK)
		<< nRet
		<< wLocalID
		<< bType
		<< bCountry;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_MISSIONOCCUPY_ACK(LPPACKETBUF pBUF)
{
	int nRet;
	WORD wLocalID;
	BYTE bType;
	BYTE bCountry;

	pBUF->m_packet
		>> nRet
		>> wLocalID
		>> bType
		>> bCountry;

	if(!nRet)
	{
		LPTLOCAL pLocal = FindMissionZone(wLocalID);
		if(!pLocal)
			return EC_NOERROR;

		CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, pLocal->m_pZone->m_wMapID);

		if(pMap)
		{
			for(DWORD i=0; i<pLocal->m_pZone->m_vSpawnID.size(); i++)
			{
				MAPTMONSPAWN::iterator it = pMap->m_mapTMONSPAWN.find(pLocal->m_pZone->m_vSpawnID[i]);

				if(it!=pMap->m_mapTMONSPAWN.end())
				{
					(*it).second->m_bStatus = MONSPAWN_READY;
					for(DWORD j=0; j<(*it).second->m_vTMON.size(); j++)
					{
						(*it).second->m_vTMON[j]->OnEvent( AT_DEAD, 0, 0, 0, 0);
					}
				}
			}
		}

		MissionReward(pLocal, bType, bCountry);

		pLocal->m_bCountry = bCountry;

		SendMW_MISSIONOCCUPY_ACK(bType, wLocalID, bCountry);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_ACTEND_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	pBUF->m_packet
		>> dwCharID;

	DEFINE_QUERY(&m_db, CSPSaveActEnd)
	query->m_dwCharID = dwCharID;
	query->Call();
	UNDEFINE_QUERY()
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_ENTERSVR_REQ( LPPACKETBUF pBUF)
{
	BYTE bDBLoad;
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> bDBLoad
		>> dwCharID
		>> dwKEY;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	if(!bDBLoad)
	{
		pPlayer->m_mainchar.Copy(&pBUF->m_packet);
		pPlayer->m_mainchar.Rewind(FALSE);
	}

	SendDM_ENTERMAPSVR_REQ(
		bDBLoad,
		pPlayer->m_dwUserID,
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_dwIPAddr,
		pPlayer->m_wPort,
		pPlayer->m_bChannel);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_ENTERMAPSVR_REQ( LPPACKETBUF pBUF)
{
	BYTE bDBLoad;

	DEFINE_QUERY( &m_db, CSPEnterServer)
	in_addr vIPAddr;

	pBUF->m_packet
		>> bDBLoad
		>> query->m_dwUserID
		>> query->m_dwCharID
		>> query->m_dwKEY
		>> vIPAddr.S_un.S_addr
		>> query->m_wPort
		>> query->m_bChannel;

	strcpy( query->m_szIPAddr, inet_ntoa(vIPAddr));
	query->m_bGroupID = m_bGroupID;

	if(!query->Call())
	{
		SendDM_ENTERMAPSVR_ACK(
			query->m_dwCharID,
			query->m_dwKEY,
			CN_INTERNAL,
			bDBLoad);
	}
	else if(query->m_nRET)
	{
		SendDM_ENTERMAPSVR_ACK(
			query->m_dwCharID,
			query->m_dwKEY,
			CN_ALREADYEXIST,
			bDBLoad);
	}
	else
	{
		SendDM_ENTERMAPSVR_ACK(
			query->m_dwCharID,
			query->m_dwKEY,
			CN_SUCCESS,
			bDBLoad);
	}
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_ENTERMAPSVR_ACK( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bResult;
	BYTE bDBLoad;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult
		>> bDBLoad;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	if(bResult)
	{
		SendMW_ENTERSVR_ACK(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			pPlayer->m_strNAME,
			pPlayer->m_bLevel,
			0,0,0,0,0,0,0,0,0,0,
			pPlayer->m_bChannel,
			pPlayer->m_wMapID,
			pPlayer->m_fPosX,
			pPlayer->m_fPosY,
			pPlayer->m_fPosZ,
			FALSE,
			FALSE,
			bResult);

		return EC_NOERROR;
	}

	if(bDBLoad)
	{
		SendDM_LOADCHAR_REQ(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			pPlayer->m_dwUserID);
	}
	else
	{
		BYTE bLoad;
		DWORD dwID;
		DWORD dwK;
		BYTE bRet;
		CString strNAME;
		BYTE bStartAct;

		pPlayer->m_mainchar
			>> bLoad
			>> dwID
			>> dwK
			>> bRet
			>> strNAME
			>> bStartAct;

		if(!bLoad && dwID == dwCharID && dwK == dwKEY)
		{
			if(bStartAct != 2)
			{
				LPPACKETBUF pMSG = new PACKETBUF();
				pMSG->m_packet.CopyData(&pPlayer->m_mainchar, sizeof(bLoad));
				pMSG->m_packet.SetID(DM_LOADCHAR_ACK);
				pMSG->m_packet.Rewind(FALSE);
				SayToBATCH(pMSG);
			}
			else
			{
				SendDM_LOADCHAR_REQ(
					pPlayer->m_dwID,
					pPlayer->m_dwKEY,
					pPlayer->m_dwUserID);
			}
		}
		else
		{
			SendMW_ENTERSVR_ACK(
				pPlayer->m_dwID,
				pPlayer->m_dwKEY,
				pPlayer->m_strNAME,
				pPlayer->m_bLevel,
				0,0,0,0,0,0,0,0,0,0,
				pPlayer->m_bChannel,
				pPlayer->m_wMapID,
				pPlayer->m_fPosX,
				pPlayer->m_fPosY,
				pPlayer->m_fPosZ,
				FALSE,
				FALSE,
				CN_INTERNAL);

			return EC_NOERROR;
		}

		pPlayer->m_mainchar.Clear();
	}

	SendDM_POSTBILL_REQ(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_LOADCHAR_REQ( LPPACKETBUF pBUF)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.ExpandIoBuffer(MAX_PACKET_SIZE);

	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwUserID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwUserID;

	DEFINE_QUERY( &m_db, CTBLChar)
	ZeroMemory( query->m_szNAME, MAX_NAME + 1);
	query->m_dwCharID = dwCharID;
	query->m_bStartAct = 0;
	query->m_bRealSex = 0;

	query->m_bClass = 0;
	query->m_bLevel = 0;
	query->m_bRace = 0;
	query->m_bSex = 0;

	query->m_bHair = 0;
	query->m_bFace = 0;
	query->m_bBody = 0;
	query->m_bPants = 0;
	query->m_bHand = 0;
	query->m_bFoot = 0;

	query->m_bHelmetHide = 0;

	query->m_dwCooper = 0;
	query->m_dwSilver = 0;
	query->m_dwGold = 0;

	query->m_dwEXP = 0;
	query->m_dwHP = 0;
	query->m_dwMP = 0;
	query->m_wSkillPoint = 0;
	query->m_dwRegion = 0;

	query->m_bGuildLeave = 0;
	query->m_dwGuildLeaveTime = 0;

	query->m_wSpawnID = 0;
	query->m_wTemptedMon = 0;
	query->m_bAftermath = 0;

	query->m_wMapID = 0;
	query->m_fPosX = 0;
	query->m_fPosY = 0;
	query->m_fPosZ = 0;
	query->m_wDIR = 0;

	if(!query->Open())
	{
		pMSG->m_packet.SetID(DM_LOADCHAR_ACK)
			<< dwCharID
			<< dwKEY
			<< (BYTE) CN_INTERNAL;
		SayToBATCH(pMSG);

		return EC_NOERROR;
	}

	if(!query->Fetch())
	{
		pMSG->m_packet.SetID(DM_LOADCHAR_ACK)
			<< dwCharID
			<< dwKEY
			<< (BYTE) CN_NOCHAR;
		SayToBATCH(pMSG);
		query->Close();

		return EC_NOERROR;
	}

	pMSG->m_packet.SetID(DM_LOADCHAR_ACK)
		<< dwCharID
		<< dwKEY
		<< (BYTE) CN_SUCCESS
		<< CString(query->m_szNAME)
		<< query->m_bStartAct
		<< query->m_bRealSex
		<< query->m_bClass
		<< query->m_bLevel
		<< query->m_bRace
		<< query->m_bCountry
		<< query->m_bOriCountry
		<< query->m_bSex
		<< query->m_bHair
		<< query->m_bFace
		<< query->m_bBody
		<< query->m_bPants
		<< query->m_bHand
		<< query->m_bFoot
		<< query->m_bHelmetHide
		<< query->m_dwGold
		<< query->m_dwSilver
		<< query->m_dwCooper
		<< query->m_dwEXP
		<< query->m_dwHP
		<< query->m_dwMP
		<< query->m_wSkillPoint
		<< query->m_dwRegion
		<< query->m_bGuildLeave
		<< query->m_dwGuildLeaveTime
		<< query->m_wMapID
		<< query->m_wSpawnID
		<< query->m_wLastSpawnID
		<< query->m_dwLastDestination
		<< query->m_wTemptedMon
		<< query->m_bAftermath
		<< query->m_fPosX
		<< query->m_fPosY
		<< query->m_fPosZ
		<< query->m_wDIR
		<< TRand(600000)
		<< WORD(0);

	query->Close();
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLAidTable)
	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		if(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_bCountry
				<< __DBTOTIME(query->m_dDate);
		}
		else
		{
			pMSG->m_packet
				<< BYTE(TCONTRY_N)
				<< INT64(0);
		}

		query->Close();
	}
	else
	{
		pMSG->m_packet
			<< BYTE(TCONTRY_N)
			<< INT64(0);
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CSPGetPcBangData)
	query->m_dwUserID = dwUserID;
	CTime timeCurrent(m_timeCurrent);
	query->m_dwEnterTime = timeCurrent.GetYear() * 10000 + timeCurrent.GetMonth() * 100 + timeCurrent.GetDay();
	query->m_bInPcBang = 0;
	query->m_dwPcBangTime = 0;
	query->m_bLuckyNumber = 0;
	query->m_bItemCnt = 0;
	query->Call();

	pMSG->m_packet
		<< query->m_bInPcBang
		<< query->m_dwPcBangTime
		<< query->m_bItemCnt
		<< query->m_bLuckyNumber;
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CSPGetPostInfo)
	query->m_dwCharID = dwCharID;
	query->m_wPage = 0;
	query->m_wTotalCount = 0;
	query->m_wNotReadCount = 0;
	query->Call();
	pMSG->m_packet
		<< query->m_wTotalCount
		<< query->m_wNotReadCount;
	UNDEFINE_QUERY()

	WORD wCount;
	WORD wSize;

	VTINVENDESC vTINVEN;
	vTINVEN.clear();

	DEFINE_QUERY( &m_db, CTBLInven)
	query->m_dwCharID = dwCharID;

	wSize = pMSG->m_packet.GetSize();
	wCount = 0;
	pMSG->m_packet
		<< wCount;

	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_bInvenID
				<< query->m_wItemID
				<< __DBTOTIME(query->m_dEndTime)
				<< query->m_bELD;

			wCount++;
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLCabinetTable);
	query->m_dwCharID = dwCharID;

	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	if(query->Open())
	{
		BYTE bCabinetID;
		BYTE bUse;

		while(query->Fetch())
		{
			bCabinetID = query->m_bCabinetID;
			bUse = query->m_bUse;

			pMSG->m_packet
				<< bCabinetID
				<< bUse;

			wCount++;
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY();

	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	DEFINE_QUERY( &m_db, CTBLItem)
	query->m_bOwnerType = TOWNER_CHAR;
	query->m_dwOwnerID = dwCharID;

	if(query->Open())
	{
		while(query->Fetch())
		{
			if(query->m_item.m_bCount)
			{
				pMSG->m_packet
					<< query->m_bStorageType
					<< query->m_dwStorageID
					<< query->m_bOwnerType
					<< query->m_dwOwnerID;

				WrapItem(
					&(pMSG->m_packet),
					query->m_item);
		
				wCount++;
			}
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLSkill);
	query->m_dwCharID = dwCharID;

	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_bLevel
				<< query->m_wSkillID
				<< query->m_dwRemainTick;

			wCount++;
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLSkillMaintain);
	query->m_dwCharID = dwCharID;

	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	if(query->Open())
	{
		while(query->Fetch())
		{
			if(query->m_wSkillID != TSTORE_SKILL)
			{
				pMSG->m_packet
					<< query->m_bLevel
					<< query->m_wSkillID
					<< query->m_dwRemainTick
					<< query->m_bAttackType
					<< query->m_dwAttackID
					<< query->m_bHostType
					<< query->m_dwHostID
					<< query->m_bAttackCountry;

				wCount++;
			}
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLQuestTable)
	query->m_dwCharID = dwCharID;

	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwQuestID
				<< query->m_dwTick
				<< query->m_bCompleteCount
				<< query->m_bTriggerCount
				<< (BYTE)FALSE;

			wCount++;
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLQuestTermTable)
	query->m_dwCharID = dwCharID;

	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwQuestID
				<< query->m_dwTermID
				<< query->m_bTermType
				<< query->m_bCount;

			wCount++;
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLHotKey)
	query->m_dwCharID = dwCharID;

	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_bInven
				<< (BYTE)ITEM_SAVE_LOAD;

			for(BYTE bHotkey=0; bHotkey<MAX_HOTKEY_POS; bHotkey++)
			{
				pMSG->m_packet
					<< query->m_bType[bHotkey]
					<< query->m_wHotID[bHotkey];
			}

			wCount++;
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLItemUsed)
	query->m_dwCharID = dwCharID;

	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_wDelayGroupID
				<< query->m_dwTick;

			wCount++;
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLPetTable)
	query->m_dwUserID = dwUserID;

	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_wPetID
				<< CString(query->m_szName);

			pMSG->m_packet
				<< __DBTOTIME(query->m_timeUse);

			wCount++;
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY();


	wSize = pMSG->m_packet.GetSize();
	wCount = 0;
	pMSG->m_packet
		<< wCount;

	DEFINE_QUERY(&m_db, CTBLDuringItem)
	query->m_dwUserID = dwUserID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_wItemID
				<< query->m_bType
				<< query->m_dwRemainTime;

			pMSG->m_packet
				<< __DBTOTIME(query->m_tEndTime);

			wCount++;
		}
		
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLExpItem)
	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_wItemID
				<< query->m_bType
				<< query->m_dwRemainTime;

			pMSG->m_packet
				<< __DBTOTIME(query->m_tEndTime);

			wCount++;
		}

		query->Close();
	}
	UNDEFINE_QUERY()
	memcpy(pMSG->m_packet.GetBuffer()+wSize, &wCount, sizeof(wCount));

	DEFINE_QUERY( &m_db, CTBLRecallMon)
	query->m_dwCharID = dwCharID;

	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwID
				<< query->m_wMonID
				<< query->m_wPetID
				<< query->m_dwATTR
				<< query->m_bLevel
				<< query->m_dwHP
				<< query->m_dwMP
				<< query->m_bSkillLevel
				<< query->m_wPosX
				<< query->m_wPosY
				<< query->m_wPosZ
				<< query->m_dwTime;

			wCount++;
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLRecallMaintain);
	query->m_dwCharID = dwCharID;

	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwRecallID
				<< query->m_bLevel
				<< query->m_wSkillID
				<< query->m_dwRemainTick
				<< query->m_bAttackType
				<< query->m_dwAttackID
				<< query->m_bHostType
				<< query->m_dwHostID
				<< query->m_bAttackCountry;

			wCount++;
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLProtectedTable)
	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	query->m_dwCharID = dwCharID;
	query->m_dwProtected = 0;
	query->m_bOption = 0;
	ZeroMemory( query->m_szName, MAX_NAME + 1);

	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwProtected
				<< query->m_szName
				<< query->m_bOption
				<< BYTE(FALSE);

			wCount++;
		}

		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CSPGetPvPRecord)
	query->m_dwCharID = dwCharID;
	query->m_dwUseablePoint = 0;
	query->m_dwTotalPoint = 0;
	query->m_dwRankOrder = 0;
	query->m_bRankPercent = 0;

	query->Call();
	pMSG->m_packet
		<< query->m_dwUseablePoint
		<< query->m_dwTotalPoint
		<< query->m_dwRankOrder
		<< query->m_bRankPercent;

	for(BYTE cr=0; cr<TCLASS_COUNT; cr++)
	{
		pMSG->m_packet
			<< query->m_aRecord[cr][0]
			<< query->m_aRecord[cr][1];
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLPvPRecent)
	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_szName
				<< query->m_bClass
				<< query->m_bLevel
				<< query->m_bWin
				<< query->m_dwPoint;

			pMSG->m_packet
				<< __DBTOTIME(query->m_dlDate);

			wCount++;
		}
		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLDuelCharList)
	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_szName
				<< query->m_bClass
				<< query->m_bLevel
				<< query->m_bWin
				<< query->m_dwPoint
				<< __DBTOTIME(query->m_dTime);

			wCount++;
		}
		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLDuelCharScore)
	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		if(query->Fetch())
		{
			for(BYTE di = 0; di < TCLASS_COUNT; di++)
			{
				pMSG->m_packet
					<< query->m_aDuelScore[di][0]
					<< query->m_aDuelScore[di][1];					
			}
			wCount++;
		}
		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	
	UNDEFINE_QUERY()

	
	DEFINE_QUERY(&m_db, CTBLAuctionBidList)
	wSize = pMSG->m_packet.GetSize();
	wCount = 0;

	pMSG->m_packet
		<< wCount;

	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwAuctionID;

			wCount++;
		}
		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db,CTBLAuctionInterest)
	wSize = pMSG->m_packet.GetSize();
	wCount = 0;
	
	pMSG->m_packet
		<< wCount;
	
	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwAuctionID;			

			wCount++;			
		}
		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db,CTBLAuctionRegList)
	wSize = pMSG->m_packet.GetSize();
	wCount = 0;
	
	pMSG->m_packet
		<< wCount;
	
	query->m_dwCharID = dwCharID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwAuctionID;

			wCount++;
		}
		memcpy( pMSG->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY()
	 
	DEFINE_QUERY(&m_db, CSPGetMonthPvPoint)
	query->m_dwCharID = dwCharID;
	query->m_dwMonthPvPoint = 0;
	query->m_wMonthWin = 0;
	query->m_wMonthLose = 0;
	query->m_dwMonthRankOrder = 0;
	query->m_bMonthRankPercent = 0;
	query->Call();

	pMSG->m_packet
		<< query->m_dwMonthPvPoint
		<< query->m_wMonthWin
		<< query->m_wMonthLose
		<< query->m_dwMonthRankOrder
		<< query->m_bMonthRankPercent;

	UNDEFINE_QUERY()

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_LOADCHAR_ACK( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	WORD wDIR;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	DWORD dwSaveTick;

	BYTE bStartAct;
	BYTE bAftermath;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	if(bResult)
	{
		SendMW_ENTERSVR_ACK(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			pPlayer->m_strNAME,
			pPlayer->m_bLevel,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			pPlayer->m_bChannel,
			pPlayer->m_wMapID,
			pPlayer->m_fPosX,
			pPlayer->m_fPosY,
			pPlayer->m_fPosZ,
			TRUE,
			FALSE,
			bResult);

		return EC_NOERROR;
	}

	pBUF->m_packet
		>> pPlayer->m_strNAME
		>> bStartAct
		>> pPlayer->m_bRealSex
		>> pPlayer->m_bClass
		>> pPlayer->m_bLevel
		>> pPlayer->m_bRace
		>> pPlayer->m_bCountry
		>> pPlayer->m_bOriCountry
		>> pPlayer->m_bSex
		>> pPlayer->m_bHair
		>> pPlayer->m_bFace
		>> pPlayer->m_bBody
		>> pPlayer->m_bPants
		>> pPlayer->m_bHand
		>> pPlayer->m_bFoot
		>> pPlayer->m_bHelmetHide
		>> pPlayer->m_dwGold
		>> pPlayer->m_dwSilver
		>> pPlayer->m_dwCooper
		>> pPlayer->m_dwEXP
		>> pPlayer->m_dwHP
		>> pPlayer->m_dwMP
		>> pPlayer->m_wSkillPoint
		>> pPlayer->m_dwRegion
		>> pPlayer->m_bGuildLeave
		>> pPlayer->m_dwGuildLeaveTime
		>> pPlayer->m_wMapID
		>> pPlayer->m_wSpawnID
		>> pPlayer->m_wLastSpawnID
		>> pPlayer->m_dwLastDestination
		>> pPlayer->m_wTemptedMon
		>> bAftermath
		>> fPosX
		>> fPosY
		>> fPosZ
		>> wDIR
		>> dwSaveTick
		>> pPlayer->m_wLocalID
		>> pPlayer->m_bAidCountry
		>> pPlayer->m_dlAidDate
		>> pPlayer->m_bInPcBang
		>> pPlayer->m_dwPcBangTime
		>> pPlayer->m_bPcBangItemCnt
		>> pPlayer->m_bLuckyNumber
		>> pPlayer->m_wPostTotal
		>> pPlayer->m_wPostRead;

	pPlayer->m_timeEnter = m_timeCurrent;

	if(pPlayer->m_bStartAct != 2)
		pPlayer->m_bStartAct = bStartAct;

	if(pPlayer->m_bStartAct == 1 && pPlayer->m_wMapID != TUTORIAL_MAPID)
		pPlayer->m_bStartAct = 0;

	pPlayer->ClearMain();

	if(!SetTutorialCharBase(pPlayer))
	{
		MAPTSPAWNPOS::iterator finder = m_mapTSPAWNPOS.find(pPlayer->m_wSpawnID);
		if( finder != m_mapTSPAWNPOS.end() &&
			(*finder).second->m_wMapID != TUTORIAL_MAPID)
		{
			if(pPlayer->m_pMAP)
				ExitMAP(pPlayer);

			LPTSPAWNPOS pPOS = (*finder).second;
			pPlayer->m_wMapID = pPOS->m_wMapID;
			pPlayer->m_fPosX = pPOS->m_fPosX;
			pPlayer->m_fPosY = pPOS->m_fPosY;
			pPlayer->m_fPosZ = pPOS->m_fPosZ;
			SendDM_SAVECHARPOSITION_REQ(pPlayer, 0);
		}

		SendMW_ENTERSVR_ACK(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			pPlayer->m_strNAME,
			pPlayer->m_bLevel,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			pPlayer->m_bChannel,
			pPlayer->m_wMapID,
			pPlayer->m_fPosX,
			pPlayer->m_fPosY,
			pPlayer->m_fPosZ,
			TRUE,
			FALSE,
			CN_INTERNAL);

		return EC_NOERROR;
	}

	pPlayer->m_pTPREVLEVEL = FindTLevel(pPlayer->m_bLevel - 1);
	pPlayer->m_pTLEVEL = FindTLevel(pPlayer->m_bLevel);

	if( !pPlayer->m_pTPREVLEVEL || !pPlayer->m_pTLEVEL )
	{
		SendMW_ENTERSVR_ACK(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			pPlayer->m_strNAME,
			pPlayer->m_bLevel,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			pPlayer->m_bChannel,
			pPlayer->m_wMapID,
			pPlayer->m_fPosX,
			pPlayer->m_fPosY,
			pPlayer->m_fPosZ,
			TRUE,
			FALSE,
			CN_INTERNAL);

		return EC_NOERROR;
	}

	pPlayer->m_pTCLASS = FindTClass(pPlayer->m_bClass);
	pPlayer->m_pTRACE = FindTRace(pPlayer->m_bRace);
	if(!pPlayer->m_pTCLASS || !pPlayer->m_pTRACE)
	{
		SendMW_ENTERSVR_ACK(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			pPlayer->m_strNAME,
			pPlayer->m_bLevel,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			pPlayer->m_bChannel,
			pPlayer->m_wMapID,
			pPlayer->m_fPosX,
			pPlayer->m_fPosY,
			pPlayer->m_fPosZ,
			TRUE,
			FALSE,
			CN_INTERNAL);

		return EC_NOERROR;
	}

	if(!pPlayer->m_dwHP)
	{
		pPlayer->m_bAction = TA_DEAD;
		pPlayer->m_bStatus = OS_DEAD;
	}

	if(!pPlayer->m_pMAP)
	{
		pPlayer->m_fPosX = fPosX;
		pPlayer->m_fPosY = fPosY;
		pPlayer->m_fPosZ = fPosZ;
		pPlayer->m_wDIR = wDIR;
	}
	else
		pPlayer->m_pMAP->OnMove(
			pPlayer,
			fPosX,
			fPosZ);

	WORD wSpawnID = GetIndunInPos(pPlayer->m_wMapID);
	if(wSpawnID)
	{
		MAPTSPAWNPOS::iterator it = m_mapTSPAWNPOS.find(wSpawnID);

		if( it != m_mapTSPAWNPOS.end() )
		{
			pPlayer->m_fPosX = (*it).second->m_fPosX;
			pPlayer->m_fPosY = (*it).second->m_fPosY;
			pPlayer->m_fPosZ = (*it).second->m_fPosZ;
		}
	}

	WORD wCount;

	pBUF->m_packet
		>> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CTInven *pTINVEN = new CTInven();

		pBUF->m_packet
			>> pTINVEN->m_bInvenID
			>> pTINVEN->m_wItemID
			>> pTINVEN->m_dEndTime
			>> pTINVEN->m_bELD;

		pTINVEN->m_pTITEM = FindTItem(pTINVEN->m_wItemID);
		pPlayer->m_mapTINVEN.insert( MAPTINVEN::value_type( pTINVEN->m_bInvenID, pTINVEN));
	}

	pBUF->m_packet
		>> wCount;

	for(WORD i=0; i<wCount; i++)
	{
		BYTE bCabinetID;
		BYTE bUse;

		pBUF->m_packet
			>> bCabinetID
			>> bUse;

		pPlayer->PutinCabinet(bCabinetID, bUse);
	}

	pBUF->m_packet
		>> wCount;

	for(WORD i=0; i<wCount; i++)
	{
		BYTE bStorageType;
		DWORD dwStorageID;
		BYTE bOwnerType;
		DWORD dwOwnerID;

		pBUF->m_packet
			>> bStorageType
			>> dwStorageID
			>> bOwnerType
			>> dwOwnerID;

		CTItem *pTITEM = CreateItem(&(pBUF->m_packet));
		if(!pTITEM)
			continue;

		if(!pTITEM->m_bCount || pTITEM->m_bCount > pTITEM->m_pTITEM->m_bStack)
		{
			delete pTITEM;
			continue;
		}

		if(pPlayer->IsTutorial())
		{
			delete pTITEM;
			continue;
		}

		switch(bStorageType)
		{
		case STORAGE_INVEN:
			{
				CTInven *pTINVEN = pPlayer->FindTInven(BYTE(dwStorageID));
				if(!pTINVEN)
					delete pTITEM;
				else
					pTINVEN->m_mapTITEM.insert( MAPTITEM::value_type( pTITEM->m_bItemID, pTITEM));
			}
			break;
		case STORAGE_CABINET:
			{
				LPTCABINET pTCABINET = pPlayer->GetCabinet(pTITEM->m_bItemID);
				if(!pTCABINET)
					delete pTITEM;
				else
				{
					pTITEM->m_dwStItemID = dwStorageID;
					pTCABINET->m_mapCabinetItem.insert(MAPTSTORAGEITEM::value_type(pTITEM->m_dwStItemID, pTITEM));
				}
			}
			break;
		case STORAGE_POST:
		default:
			delete pTITEM;
			break;
		}
	}

	SetTutorialCharItem(pPlayer);

	DWORD dwRemainTick;

	pBUF->m_packet
		>> wCount;
	pPlayer->m_vRemainSkill.clear();

	for(int i=0; i<wCount; i++)
	{
		WORD wSkillID;
		BYTE bSLevel;

		pBUF->m_packet
			>> bSLevel
			>> wSkillID
			>> dwRemainTick;

		if(pPlayer->IsTutorial())
			continue;

		CTSkillTemp *pTemp = FindTSkill(wSkillID);
		if(pTemp)
		{
			CTSkill *pSkill = new CTSkill();

			pSkill->m_bLevel = bSLevel;
			pSkill->m_pTSKILL = pTemp;

			if(dwRemainTick)
			{
				pSkill->m_dwUseTick = m_dwTick;
				pSkill->m_dwDelayTick = dwRemainTick;
			}
			else
			{
				pSkill->m_dwUseTick = 0;
				pSkill->m_dwDelayTick = 0;
			}

			//스킬 버릴때 m_vRemainSkill에서 해당 스킬 꼭 뺄것
			if(pSkill->m_pTSKILL->IsRemainType())
				pPlayer->m_vRemainSkill.push_back(pSkill);
			pPlayer->m_mapTSKILL.insert( MAPTSKILL::value_type( wSkillID, pSkill));
		}
	}

	pBUF->m_packet
		>> wCount;

	for(int i=0; i<wCount; i++)
	{
		WORD wESkillID;
		BYTE bESLevel;
		BYTE bAttackType;
		DWORD dwAttackID;
		BYTE bHostType;
		DWORD dwHostID;
		BYTE bAttackCountry;

		pBUF->m_packet
			>> bESLevel
			>> wESkillID
			>> dwRemainTick
			>> bAttackType
			>> dwAttackID
			>> bHostType
			>> dwHostID
			>> bAttackCountry;

		if(pPlayer->IsTutorial())
			continue;

		CTSkillTemp *pTemp = FindTSkill(wESkillID);
		if(pTemp)
		{
			CTSkill *pSkill = new CTSkill();

			pSkill->m_dwStartTick = dwRemainTick ? m_dwTick : 0;
			pSkill->m_dwMaintainTick = dwRemainTick;
			pSkill->m_pTSKILL = pTemp;
			pSkill->m_bLevel = bESLevel;
			pSkill->m_bAttackType = bAttackType;
			pSkill->m_dwAttackID = dwAttackID;
			pSkill->m_dwHostID = dwHostID;
			pSkill->m_bHostType = bHostType;
			pSkill->m_bAttackCountry = bAttackCountry;
			pSkill->m_dwChargeTick = m_dwTick;

			for( DWORD j=0; j<DWORD(pSkill->m_pTSKILL->m_vData.size()); j++)
			{
				LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];

				if( pData->m_bType == SDT_TRANS )
					pPlayer->SetTransAttack(pData->m_wValue);

				if( pData->m_bType == SDT_STATUS &&
					(pData->m_bExec == SDT_STATUS_PCBANG ||
					 pData->m_bExec == SDT_STATUS_PREMIUM))
				{
					delete pSkill;
					pSkill = NULL;
					break;
				}
			}

			pPlayer->PushMaintainSkill(pSkill);
		}
	}

	pBUF->m_packet
		>> wCount;

	for(int i=0; i<wCount; i++)
	{
		CQuest *pQUEST = NULL;

		DWORD dwQuestID;
		DWORD dwTick;

		BYTE bComCnt;
		BYTE bTriCnt;
		BYTE bSave;

		pBUF->m_packet
			>> dwQuestID
			>> dwTick
			>> bComCnt
			>> bTriCnt
			>> bSave;

		if(pPlayer->IsTutorial())
			continue;

		LPQUESTTEMP pQuestTemp = FindQuestTemplate(dwQuestID);
		if(pQuestTemp && pQuestTemp->m_bType)
		{
			pQUEST = CQuest::CreateQuest(pQuestTemp);
			pQUEST->m_dwTick = dwTick;
			pQUEST->m_bCompleteCount = bComCnt;
			pQUEST->m_bTriggerCount = bTriCnt;
			pQUEST->m_bSave = bSave;

			if(pQUEST->m_dwTick)
			{
				pQUEST->m_dwBeginTick = m_dwTick;
				pQUEST->m_bSave = TRUE;
			}

			pPlayer->m_mapQUEST.insert( MAPQUEST::value_type( dwQuestID, pQUEST));
			DWORD dwLevel = pPlayer->GetQuestLevel(pQuestTemp);

			if(dwLevel)
				pPlayer->m_mapLevelQuest.insert( MAPDWORD::value_type( dwLevel, pQuestTemp->m_dwQuestID));
		}
	}

	pBUF->m_packet
		>> wCount;

	for(int i=0; i<wCount; i++)
	{
		DWORD dwQuestID;
		DWORD dwTermID;

		BYTE bTermType;
		BYTE bCount;

		pBUF->m_packet
			>> dwQuestID
			>> dwTermID
			>> bTermType
			>> bCount;

		if(pPlayer->IsTutorial())
			continue;

		MAPQUEST::iterator finder = pPlayer->m_mapQUEST.find(dwQuestID);
		if( finder != pPlayer->m_mapQUEST.end()  && finder->second->m_pQUEST->m_bType)
		{
			LPQUESTTERM pTERM = new QUESTTERM();

			pTERM->m_bTermType = bTermType;
			pTERM->m_dwTermID = dwTermID;
			pTERM->m_bCount = bCount;

			(*finder).second->m_vTERM.push_back(pTERM);
		}
	}

	pBUF->m_packet
		>> wCount;

	for(int i=0; i<wCount; i++)
	{
		BYTE bInvenID;
		BYTE bInvenSave;

		pBUF->m_packet
			>> bInvenID
			>> bInvenSave;

		LPTHOTKEYINVEN pInven = new THOTKEYINVEN();
		pInven->m_bSave = bInvenSave;

		for( BYTE j=0; j<MAX_HOTKEY_POS; j++)
		{
			pInven->m_hotkey[j].m_bPos = j;

			pBUF->m_packet
				>> pInven->m_hotkey[j].m_bType
				>> pInven->m_hotkey[j].m_wID;
		}

		pPlayer->m_mapHotkeyInven.insert( MAPTHOTKEYINVEN::value_type( bInvenID, pInven));
	}

	pBUF->m_packet
		>> wCount;

	for(int i=0; i<wCount; i++)
	{
		DWORD dwTick;
		WORD wDelayGroup;

		pBUF->m_packet
			>> wDelayGroup
			>> dwTick;

		if(pPlayer->IsTutorial())
			continue;

		if(dwTick < HOUR_ONE * 1000)
			pPlayer->m_mapItemCoolTime.insert( MAPDWORD::value_type( wDelayGroup, m_dwTick + dwTick));
	}

	pBUF->m_packet
		>> wCount;

	for(int i=0; i<wCount; i++)
	{
		CString strName;
		WORD wPetID;
		__int64 ldwTime;

		pBUF->m_packet
			>> wPetID
			>> strName
			>> ldwTime;

		if(pPlayer->IsTutorial())
			continue;

		WORD wID = MAKEWORD(pPlayer->m_bRace, wPetID);
		MAPTPETTEMP::iterator find = m_mapTPET.find(wID);
		if( find != m_mapTPET.end() &&
			wPetID != PCBANG_PET &&
			wPetID != PREMIUM1_PET &&
			wPetID != PREMIUM2_PET)
		{
			LPTPET pPET = new TPET();

			pPET->m_pTPET = (*find).second;
			pPET->m_wPetID = wID;
			pPET->m_strName = strName;
			pPET->m_ldwTime = ldwTime;

			pPlayer->m_mapTPET.insert( MAPTPET::value_type( pPET->m_wPetID, pPET));
		}
	}

	pBUF->m_packet
		>> wCount;

	for(WORD i=0; i<wCount; i++)
	{
		WORD wItemID;
		BYTE bType;
		DWORD dwRemainTime;
		__int64 tEndTime;

		pBUF->m_packet
			>> wItemID
			>> bType
			>> dwRemainTime
			>> tEndTime;

		if(pPlayer->IsTutorial())
			continue;

		LPTITEM pITEM = FindTItem(wItemID);
		if(pITEM)
		{
			DWORD dwAdd = 0;
			BYTE bUseType = DURINGTYPE_TIME;
			if(pITEM->m_bUseType & DURINGTYPE_TIME)
				dwAdd = dwRemainTime;
			else if(pITEM->m_bUseType & DURINGTYPE_DAY)
			{
				bUseType = DURINGTYPE_DAY;
				dwAdd = DWORD(tEndTime > m_timeCurrent ? tEndTime - m_timeCurrent : 0);
			}

			LPTDURINGITEM pDI = pPlayer->SetDuringItem(pITEM->m_bKind, pITEM, bUseType, dwAdd);
			if(pDI)
				pDI->m_tEndTime = tEndTime;

			if(GetNation() == NATION_KOREA)
			{
				if(pITEM->m_bKind == IK_GOLDPREMIUM)
					pPlayer->PetMake(PREMIUM1_PET, GetSvrMsg(PREMIUM_PETNAME), dwAdd);
				else if(pITEM->m_bKind == IK_GOLDPREMIUM2)
					pPlayer->PetMake(PREMIUM2_PET, GetSvrMsg(PREMIUM_PETNAME), dwAdd);
			}
		}
	}

	if(pPlayer->m_bInPcBang & PCBANG_REAL)
		pPlayer->PetMake(PCBANG_PET, PCBANG_PETNAME, PET_LIVE_DURATION);
	
	pBUF->m_packet
		>> wCount;

	for(int i=0; i<wCount; i++)
	{
		DWORD dwLiveTick;
		DWORD dwID;

		WORD wMonID;
		WORD wPetID;
		DWORD dwATTR;
		WORD wPosX;
		WORD wPosY;
		WORD wPosZ;
		DWORD dwHP;
		DWORD dwMP;
		BYTE bLevel;
		BYTE bSkillLevel;

		pBUF->m_packet
			>> dwID
			>> wMonID
			>> wPetID
			>> dwATTR
			>> bLevel
			>> dwHP
			>> dwMP
			>> bSkillLevel
			>> wPosX
			>> wPosY
			>> wPosZ
			>> dwLiveTick;

		if(pPlayer->IsTutorial())
			continue;

		LPTMONSTER pTemp = FindTMonster(wMonID);
		if(pTemp)
		{
			LPTPET pPET = NULL;

			if( pTemp->m_bRecallType == TRECALLTYPE_PET )
			{
				MAPTPET::iterator it;

				for( it = pPlayer->m_mapTPET.begin(); it != pPlayer->m_mapTPET.end(); it++)
				{
					if( wPetID == (*it).second->m_wPetID )
					{
						if( !(*it).second->m_ldwTime || (*it).second->m_ldwTime > m_timeCurrent )
						{
							__int64 ldwLive = (*it).second->m_ldwTime ? (*it).second->m_ldwTime - m_timeCurrent : 0;

							dwLiveTick = PET_LIVE_DURATION;
							if( ldwLive < dwLiveTick )
								dwLiveTick = DWORD(ldwLive);
							dwLiveTick *= 1000;

							pPET = (*it).second;

							if((*it).second->m_pTPET->m_bConditionType == PETCON_PCBANG &&
								((*it).second->m_pTPET->m_dwConditionValue && !pPlayer->m_bInPcBang))
								pPET = NULL;
						}

						break;
					}
				}

				if(!pPET)
					continue;
			}

			VWORD vSkill;
			vSkill.clear();
			if(pTemp->m_wID)
				vSkill = pTemp->m_vSKILL;
			else
			{
				for( WORD ms=31; ms<=34; ms++)
				{
					//기본스킬
					CTSkill * pMy = pPlayer->FindTSkill(ms);
					if(pMy && pPlayer->IsEquipSkillItem(pMy))
						vSkill.push_back(ms);
				}
			}

			CreateRecallMon(
				pPlayer,
				pTemp,
				dwATTR,
				dwID,
				(pPET ? pPET->m_wPetID : 0),
				(pPET ? pPET->m_strName : NAME_NULL),
				bLevel,
				pTemp->m_wID ? pTemp->m_bClass : pPlayer->m_bClass,
				pTemp->m_wID ? pTemp->m_bRace : pPlayer->m_bRace,
				pTemp->m_wID ? TA_STAND : pPlayer->m_bAction,
				pTemp->m_wID ? OS_WAKEUP : pPlayer->m_bStatus,
				pTemp->m_wID ? MT_NORMAL : pPlayer->m_bMode,
				pTemp->m_wID ? 0 : pPlayer->GetMaxHP(),
				pTemp->m_wID ? 0 : pPlayer->GetMaxMP(),
				dwHP, dwMP,
				100,0,0,0,0,0,
				bSkillLevel,
				pPlayer->m_fPosX - 2.0f * sinf(FLOAT(pPlayer->m_wDIR) * FLOAT(M_PI) / 900.0f),
				pPlayer->m_fPosY,
				pPlayer->m_fPosZ - 2.0f * cosf(FLOAT(pPlayer->m_wDIR) * FLOAT(M_PI) / 900.0f),
				0, dwLiveTick,
				vSkill);

			vSkill.clear();
		}
	}

	pBUF->m_packet
		>> wCount;

	for(int i=0; i<wCount; i++)
	{
		DWORD dwRecallID;
		WORD wESkillID;
		BYTE bESLevel;
		BYTE bAttackType;
		DWORD dwAttackID;
		BYTE bHostType;
		DWORD dwHostID;
		BYTE bAttackCountry;

		pBUF->m_packet
			>> dwRecallID
			>> bESLevel
			>> wESkillID
			>> dwRemainTick
			>> bAttackType
			>> dwAttackID
			>> bHostType
			>> dwHostID
			>> bAttackCountry;

		if(pPlayer->IsTutorial())
			continue;

		CTRecallMon * pRecall = pPlayer->FindRecallMon(dwRecallID);
		if(pRecall && !pRecall->FindMaintainSkill(wESkillID))
		{
			CTSkillTemp *pTemp = FindTSkill(wESkillID);
			if(!pTemp)
				continue;

			CTSkill *pSkill = new CTSkill();

			pSkill->m_dwStartTick = dwRemainTick ? m_dwTick : 0;
			pSkill->m_dwMaintainTick = dwRemainTick;
			pSkill->m_pTSKILL = pTemp;
			pSkill->m_bLevel = bESLevel;
			pSkill->m_bAttackType = bAttackType;
			pSkill->m_dwAttackID = dwAttackID;
			pSkill->m_dwHostID = dwHostID;
			pSkill->m_bHostType = bHostType;
			pSkill->m_bAttackCountry = bAttackCountry;

			for( DWORD j=0; j<DWORD(pSkill->m_pTSKILL->m_vData.size()); j++)
			{
				LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];

				if( pData->m_bType == SDT_TRANS )
					pRecall->SetTransAttack(pData->m_wValue);
			}

			pRecall->PushMaintainSkill(pSkill);
		}
	}

	pBUF->m_packet
		>> wCount;

	for(WORD i=0; i<wCount; i++)
	{
		LPTPROTECTED pProtected = new TPROTECTED();

		pBUF->m_packet
			>> pProtected->m_dwCharID
			>> pProtected->m_strName
			>> pProtected->m_bOption
			>> pProtected->m_bChanged;

		pPlayer->m_mapTPROTECTED.insert(MAPTPROTECTED::value_type(pProtected->m_dwCharID, pProtected));
	}

	pBUF->m_packet
		>> pPlayer->m_dwPvPUseablePoint
		>> pPlayer->m_dwPvPTotalPoint
		>> pPlayer->m_dwPvPRankOrder
		>> pPlayer->m_bPvPRankPercent;

	for(BYTE cr=0; cr<TCLASS_COUNT; cr++)
	{
		pBUF->m_packet
			>> pPlayer->m_aPvPRecord[cr][0]
			>> pPlayer->m_aPvPRecord[cr][1];
	}
	for(BYTE i = 0; i < TCLASS_COUNT; i++)
	{		
		pPlayer->m_dwTotalLose += pPlayer->m_aPvPRecord[i][0];
		pPlayer->m_dwTotalWin += pPlayer->m_aPvPRecord[i][1];
	}

	pBUF->m_packet
		>> wCount;

	for(WORD i=0; i<wCount; i++)
	{
		TRECORDSET set;
		pBUF->m_packet
			>> set.m_strName
			>> set.m_bClass
			>> set.m_bLevel
			>> set.m_bWin
			>> set.m_dwPoint
			>> set.m_dTime;

		pPlayer->m_vPvPRecent.push_back(set);
	}

	pBUF->m_packet
		>> wCount;
	
	for(WORD i = 0; i < wCount; i++)
	{
		TRECORDSET set;
		
		pBUF->m_packet
			>> set.m_strName
			>> set.m_bClass
			>> set.m_bLevel
			>> set.m_bWin
			>> set.m_dwPoint
			>> set.m_dTime;
				
		pPlayer->m_vDuelRecordSet.push_back(set);
	}
	
	pBUF->m_packet
		>> wCount;

	if(wCount)
	{
		for(BYTE i = 0; i < TCLASS_COUNT; i++)
		{
			pBUF->m_packet
				>> pPlayer->m_aDuelScore[i][0]
				>> pPlayer->m_aDuelScore[i][1];
		}
	}

	
	pBUF->m_packet
		>> wCount;

	DWORD dwAuctionID;

	if(wCount)
	{
		for(WORD i = 0; i < wCount; i++)
		{
			pBUF->m_packet
				>> dwAuctionID;
			
			pPlayer->AuctionBidListSave(TRUE,dwAuctionID);
		}
	}

	pBUF->m_packet
		>> wCount;

	if(wCount)
	{
		for(WORD i = 0; i < wCount; i++)
		{
			pBUF->m_packet
				>> dwAuctionID;			

			pPlayer->AuctionInterestSave(TRUE,dwAuctionID);
		}
	}

	pBUF->m_packet
		>> wCount;

	if(wCount)
	{
		for(WORD i = 0; i < wCount; i++ )
		{
			pBUF->m_packet
				>> dwAuctionID;				

			pPlayer->AuctionRegListSave(TRUE,dwAuctionID);
		}
	}

	pBUF->m_packet
		>> pPlayer->m_dwMonthPvPoint
		>> pPlayer->m_wMonthWin
		>> pPlayer->m_wMonthLose
		>> pPlayer->m_dwMonthRankOrder
		>> pPlayer->m_bMonthRankPercent;

	pPlayer->m_bEquipSpecial = pPlayer->IsEquipSpecial();
	pPlayer->SetAftermath( m_dwTick, bAftermath);
	pPlayer->m_dwSaveTick = m_dwTick - dwSaveTick;

	DWORD dwMaxHP = pPlayer->GetMaxHP();
	DWORD dwMaxMP = pPlayer->GetMaxMP();
	if(dwMaxHP < pPlayer->m_dwHP)
		pPlayer->m_dwHP = dwMaxHP;
	if(dwMaxMP < pPlayer->m_dwMP)
		pPlayer->m_dwMP = dwMaxMP;

	SendMW_ENTERSVR_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		pPlayer->m_strNAME,
		pPlayer->m_bLevel,
		pPlayer->m_bRealSex,
		pPlayer->m_bClass,
		pPlayer->m_bRace,
		pPlayer->m_bSex,
		pPlayer->m_bFace,
		pPlayer->m_bHair,
		pPlayer->m_bHelmetHide,
		pPlayer->m_bCountry,
		pPlayer->m_bAidCountry,
		pPlayer->m_dwRegion,
		pPlayer->m_bChannel,
		pPlayer->m_wMapID,
		pPlayer->m_fPosX,
		pPlayer->m_fPosY,
		pPlayer->m_fPosZ,
		TRUE,
		TRUE,
		CN_SUCCESS,
		pPlayer->m_addr.sin_addr.s_addr);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTBILL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	DEFINE_QUERY(&m_db, CTBLPostBill)
	query->m_dwSendID = dwCharID;
	query->m_bType = POST_BILLS;
	if(query->Open())
	{
		while(query->Fetch())
			SendSM_POSTBILL_REQ(dwCharID, query->m_dwPostID, __DBTOTIME(query->m_timeRecv) + POST_DURATION);

		query->Close();
	}
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_LOADPOSTMESSAGE_REQ(LPPACKETBUF pBUF)
{
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_LOADPOSTMESSAGE_ACK(LPPACKETBUF pBUF)
{
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_LOGSKILL_REQ(LPPACKETBUF pBUF)
{
	DEFINE_QUERY(&m_db, CSPLogSkill)

	pBUF->m_packet
		>> query->m_dwCharID
		>> query->m_dwGold
		>> query->m_dwSilver
		>> query->m_dwCooper
		>> query->m_wSkill
		>> query->m_bLevel;
	query->Call();

	UNDEFINE_QUERY()
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CONLIST_REQ( LPPACKETBUF pBUF)
{
	MAPBYTE mapCON;
	mapCON.clear();

	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bChannel;
	WORD wMapID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ;

	CTChannel *pCH = FindTChannel(
		bChannel,
		wMapID);

	if(!pCH)
	{
		SendMW_CONLIST_ACK(
			dwCharID,
			dwKEY,
			NULL);

		return EC_NOERROR;
	}

	MAPTMAP::iterator itMAP = pCH->m_mapTMAP.find(wMapID);
	if( itMAP == pCH->m_mapTMAP.end() )
	{
		SendMW_CONLIST_ACK(
			dwCharID,
			dwKEY,
			NULL);

		return EC_NOERROR;
	}
	CTMap *pMAP = (*itMAP).second;

	DWORD dwCell = MAKELONG(
		WORD(INT(fPosX) / CELL_SIZE),
		WORD(INT(fPosZ) / CELL_SIZE));

	MAPTCELL::iterator itCELL = pMAP->m_mapTCELL.find(dwCell);
	if( itCELL == pMAP->m_mapTCELL.end() || !(*itCELL).second->IsMainCell(bChannel) )
	{
		SendMW_CONLIST_ACK(
			dwCharID,
			dwKEY,
			NULL);

		return EC_NOERROR;
	}
	CTCell *pCell = (*itCELL).second;

	for( BYTE i=0; i<UD_COUNT; i++)
		if(pCell->m_vServerID[i][bChannel])
			mapCON.insert( MAPBYTE::value_type( pCell->m_vServerID[i][bChannel], pCell->m_vServerID[i][bChannel]));

	SendMW_CONLIST_ACK(
		dwCharID,
		dwKEY,
		&mapCON);
	mapCON.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_MAPSVRLIST_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bChannel;
	WORD wMapID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	if( bChannel != pPlayer->m_bChannel ||
		wMapID != pPlayer->m_wMapID )
	{
		pPlayer->SendCS_CONNECT_ACK(CN_INTERNAL);
		CloseSession(pPlayer);

		return EC_NOERROR;
	}

	CTChannel *pCH = FindTChannel(
		pPlayer->m_bChannel,
		pPlayer->m_wMapID);

	if(!pCH)
	{
		pPlayer->SendCS_CONNECT_ACK(CN_NOCHANNEL);
		CloseSession(pPlayer);

		return EC_NOERROR;
	}

	MAPTMAP::iterator itMAP = pCH->m_mapTMAP.find(pPlayer->m_wMapID);
	if( itMAP == pCH->m_mapTMAP.end() )
	{
		pPlayer->SendCS_CONNECT_ACK(CN_INTERNAL);
		CloseSession(pPlayer);

		return EC_NOERROR;
	}
	CTMap *pMAP = (*itMAP).second;

	WORD wCellX = WORD(fPosX) / CELL_SIZE;
	WORD wCellY = WORD(fPosZ) / CELL_SIZE;
	DWORD dwCell = MAKELONG( wCellX, wCellY);

	MAPTCELL::iterator itCELL = pMAP->m_mapTCELL.find(dwCell);
	if( itCELL == pMAP->m_mapTCELL.end() || !(*itCELL).second->IsMainCell(pPlayer->m_bChannel) )
	{
		pPlayer->SendCS_CONNECT_ACK(CN_INTERNAL);
		CloseSession(pPlayer);

		return EC_NOERROR;
	}

	CTCell *pCell = (*itCELL).second;
	MAPBYTE mapCON;
	mapCON.clear();

	for( BYTE i=0; i<UD_COUNT; i++)
		if(pCell->m_vServerID[i][pPlayer->m_bChannel])
			mapCON.insert( MAPBYTE::value_type( pCell->m_vServerID[i][pPlayer->m_bChannel], pCell->m_vServerID[i][pPlayer->m_bChannel]));

	SendMW_MAPSVRLIST_ACK(
		pPlayer->m_dwID,
		pPlayer->m_dwKEY,
		&mapCON);
	mapCON.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_ROUTE_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bChannel;
	WORD wMapID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	if( bChannel != pPlayer->m_bChannel ||
		wMapID != pPlayer->m_wMapID )
	{
		pPlayer->SendCS_CONNECT_ACK(CN_INTERNAL);
		CloseSession(pPlayer);

		return EC_NOERROR;
	}

	CTChannel *pCH = FindTChannel(
		pPlayer->m_bChannel,
		pPlayer->m_wMapID);

	if(!pCH)
	{
		pPlayer->SendCS_CONNECT_ACK(CN_NOCHANNEL);
		CloseSession(pPlayer);

		return EC_NOERROR;
	}

	MAPTMAP::iterator itMAP = pCH->m_mapTMAP.find(pPlayer->m_wMapID);
	if( itMAP == pCH->m_mapTMAP.end() )
	{
		pPlayer->SendCS_CONNECT_ACK(CN_INTERNAL);
		CloseSession(pPlayer);

		return EC_NOERROR;
	}
	CTMap *pMAP = (*itMAP).second;

	WORD wCellX = WORD(fPosX) / CELL_SIZE;
	WORD wCellY = WORD(fPosZ) / CELL_SIZE;
	DWORD dwCell = MAKELONG( wCellX, wCellY);

	MAPTCELL::iterator itCELL = pMAP->m_mapTCELL.find(dwCell);
	if( itCELL == pMAP->m_mapTCELL.end() )
	{
		pPlayer->SendCS_CONNECT_ACK(CN_INTERNAL);
		CloseSession(pPlayer);

		return EC_NOERROR;
	}
	CTCell *pCell = (*itCELL).second;

	LPPACKETBUF pMSG = new PACKETBUF();
	BYTE bCount = 0;

	for( BYTE i=0; i<UD_COUNT; i++)
		if(pCell->m_vServerID[i][pPlayer->m_bChannel])
			bCount++;

	pMSG->m_packet.SetID(DM_ROUTE_REQ)
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY
		<< bCount;

	for( BYTE i=0; i<UD_COUNT; i++)
		if(pCell->m_vServerID[i][pPlayer->m_bChannel])
			pMSG->m_packet << pCell->m_vServerID[i][pPlayer->m_bChannel];
	SayToDB(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_ROUTELIST_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bCount;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;
	LPPACKETBUF pMSG = new PACKETBUF();

	pMSG->m_packet.SetID(DM_ROUTE_REQ)
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY
		<< bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		BYTE bServerID;

		pBUF->m_packet >> bServerID;
		pMSG->m_packet << bServerID;
	}
	SayToDB(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_ROUTE_REQ( LPPACKETBUF pBUF)
{
	LPPACKETBUF pMSG = new PACKETBUF();

	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bCount;

	pMSG->m_packet.SetID(DM_ROUTE_ACK)
		<< dwCharID
		<< dwKEY
		<< bCount;

	DEFINE_QUERY( &m_db, CSPRoute)
	for( BYTE i=0; i<bCount; i++)
	{
		query->m_bGroupID = m_bGroupID;
		query->m_bType = SVRGRP_MAPSVR;
		query->m_wPort = 0;

		pBUF->m_packet
			>> query->m_bServerID;

		query->Call();
		pMSG->m_packet
			<< DWORD(inet_addr(query->m_szIPAddr))
			<< query->m_wPort
			<< query->m_bServerID;
	}
	UNDEFINE_QUERY()
	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_ROUTE_ACK( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bCount;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	CPacket *pMSG = new CPacket();
	pMSG->SetID(MW_ROUTE_ACK)
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY
		<< bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		BYTE bServerID;

		DWORD dwIPAddr;
		WORD wPort;

		pBUF->m_packet
			>> dwIPAddr
			>> wPort
			>> bServerID;

		(*pMSG)
			<< dwIPAddr
			<< wPort
			<< bServerID;
	}
	m_world.Say(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_TELEPORT_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	BYTE bServerID;
	BYTE bChannel;
	WORD wMapID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	BYTE bTutorialOut;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> bTutorialOut;

	DEFINE_QUERY( &m_db, CSPGetServerID)
	query->m_wUnitID = MAKEWORD( BYTE(INT(fPosX) / UNIT_SIZE), BYTE(INT(fPosZ) / UNIT_SIZE));
	query->m_wMapID = wMapID;
	query->m_bChannel = bChannel;
	query->m_bServerID = 0xFF;

	if(query->Call())
		bServerID = query->m_bServerID;
	else
		bServerID = 0xFF;
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_TELEPORT_ACK)
		<< dwCharID
		<< dwKey
		<< bServerID
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< bTutorialOut;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_TELEPORT_ACK( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bServerID;
	BYTE bChannel;
	WORD wMapID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	BYTE bTutorialOut;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bServerID
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> bTutorialOut;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	if( bServerID == 0xFF )
	{
		pPlayer->SendCS_TELEPORT_ACK(
			TPR_NODESTINATION,
			0,
			pPlayer->m_dwID,
			pPlayer->m_bType,
			pPlayer->m_wMapID,
			0, 0, 0);
	}
	else
	{
		WORD wAckMsg = 0;
		if(bTutorialOut && m_bServerID == bServerID)
		{
			wAckMsg = DM_DORELEASEMAIN_ACK;

			pPlayer->SendCS_TELEPORT_ACK(
				TPR_SUCCESS,
				0,
				pPlayer->m_dwID,
				pPlayer->m_bType,
				wMapID,
				fPosX,
				fPosY,
				fPosZ);
		}
		else
		{
			SendMW_TELEPORT_ACK(
				pPlayer->m_dwID,
				pPlayer->m_dwKEY,
				bServerID);
		}

		if(bTutorialOut)
		{
			pPlayer->ReleaseTutorial();

			SendDM_SAVECHARPOSITION_REQ(
				pPlayer,
				wAckMsg);
		}
	}

	return EC_NOERROR;
}

#ifdef	DEF_UDPLOG

/*
 * Memory확인 작업이 필요 함
 */
DWORD CTMapSvrModule::OnDM_UDPLOG_REQ( LPPACKETBUF pBUF)
{
	
	//	
	_LPUDPPACKET pUDPPacket = new _UDPPACKET;
	memset( pUDPPacket, 0x00, sizeof(_UDPPACKET));

	pBUF->m_packet.Read( &pUDPPacket->szPacket, sizeof(_LOG_DATA_));
				

	m_pUdpSocket->Log(pUDPPacket);

	return EC_NOERROR;
}

#endif	//	DEF_UDPLOG

DWORD CTMapSvrModule::OnMW_ADDCONNECT_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bCount;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	CPacket *pMSG = new CPacket();
	pMSG->SetID(CS_ADDCONNECT_ACK)
		<< bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		BYTE bServerID;

		DWORD dwIPAddr;
		WORD wPort;

		pBUF->m_packet
			>> dwIPAddr
			>> wPort
			>> bServerID;

		(*pMSG)
			<< dwIPAddr
			<< wPort
			<< bServerID;
	}
	pPlayer->Say(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_RELEASEMAIN_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bChannel;
	WORD wMapID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	if( pPlayer->m_bChannel != bChannel ||
		pPlayer->m_wMapID != wMapID )
	{
		CloseSession(pPlayer);
		return EC_NOERROR;
	}

	if(pPlayer->m_bMain)
	{
		pPlayer->m_fPosY = fPosY;
		pPlayer->m_bMain = FALSE;

		if(pPlayer->m_pMAP)
		{
			pPlayer->m_pMAP->OnMove(
				pPlayer,
				fPosX,
				fPosZ);
		}
		else
		{
			pPlayer->m_fPosX = fPosX;
			pPlayer->m_fPosZ = fPosZ;
		}

		SendMW_RELEASEMAIN_ACK(pPlayer->ProtectTutorial() ? TRUE : FALSE, pPlayer);
		SendSM_POSTBILLERASE_REQ(pPlayer->m_dwID, 0);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_SAVECHAR_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	WORD wACKMsg;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	DEFINE_QUERY( &m_db, CSPSaveChar)
	pBUF->m_packet
		>> query->m_bStartAct  
		>> query->m_bLevel
		>> query->m_bHelmetHide
		>> query->m_dwGold
		>> query->m_dwSilver
		>> query->m_dwCooper
		>> query->m_bGuildLeave
		>> query->m_dwGuildLeaveTime
		>> query->m_dwEXP
		>> query->m_dwHP
		>> query->m_dwMP
		>> query->m_wSkillPoint
		>> query->m_dwRegion
		>> query->m_wMapID
		>> query->m_wSpawnID
		>> query->m_wLastSpawnID
		>> query->m_dwLastDestination
		>> query->m_wTemptedMon
		>> query->m_bAftermath
		>> query->m_fPosX
		>> query->m_fPosY
		>> query->m_fPosZ
		>> query->m_wDIR
		>> query->m_dwPcBangTime
		>> query->m_bPcBangItemCnt;

	query->m_dwCharID = dwCharID;
	if(!query->Call())
		return EC_NOERROR;

	if(query->m_nRET)
		return EC_NOERROR;

	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CSPSavePvPRecord)
	query->m_dwCharID = dwCharID;
	pBUF->m_packet
		>> query->m_dwUseablePoint
		>> query->m_dwTotalPoint;

	for(BYTE cr=0; cr<TCLASS_COUNT; cr++)
	{
		pBUF->m_packet
			>> query->m_aRecord[cr][0]
			>> query->m_aRecord[cr][1];
	}
	query->Call();
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CSPSaveCharDataStart)
	query->m_dwCharID = dwCharID;
	if(!query->Call())
		return EC_NOERROR;
	UNDEFINE_QUERY();

	MAPTINVEN::iterator itTINVEN;
	MAPTINVEN mapOLDINVEN;
	MAPTINVEN mapNEWINVEN;

	mapOLDINVEN.clear();
	mapNEWINVEN.clear();

	BYTE bInvenCount;

	pBUF->m_packet
		>> bInvenCount;

	for( WORD i=0; i<bInvenCount; i++)
	{
		BYTE bInvenID;
		__time64_t dEndTime;
		BYTE bItemCount;
		DEFINE_QUERY( &m_db, CSPSaveInven)
		query->m_dwCharID = dwCharID;
		pBUF->m_packet
			>> bInvenID
			>> query->m_wItemID
			>> dEndTime
			>> query->m_bELD
			>> bItemCount;

		__TIMETODB(dEndTime, query->m_dEndTime);
		query->m_bInvenID = bInvenID;
		if(!query->Call())
			return EC_NOERROR;
		UNDEFINE_QUERY()

		DEFINE_QUERY( &m_db, CSPSaveItem)
		query->m_bStorageType = STORAGE_INVEN;
		query->m_dwStorageID = bInvenID;
		query->m_bOwnerType = TOWNER_CHAR;
		query->m_dwOwnerID = dwCharID;

		for( BYTE j=0; j<bItemCount; j++)
		{
			WrapItemQuery(&(pBUF->m_packet), query->m_item);
			if(!query->Call())
				return EC_NOERROR;
		}
		UNDEFINE_QUERY()
	}

	BYTE bCabinetCount;

	pBUF->m_packet
		>> bCabinetCount;

	for(BYTE i=0; i<bCabinetCount; i++)
	{
		BYTE bCabinetID;
		BYTE bUse;

		DEFINE_QUERY(&m_db, CSPSaveCabinet);
		query->m_dwCharID = dwCharID;
		pBUF->m_packet
			>> bCabinetID
			>> bUse;
		query->m_bCabinetID = bCabinetID;
		query->m_bUse = bUse;
		if(!query->Call())
			return EC_NOERROR;
		UNDEFINE_QUERY();

		WORD wCabinetItemCount;
		pBUF->m_packet
			>> wCabinetItemCount;

		DEFINE_QUERY( &m_db, CSPSaveItem)
		query->m_bStorageType = STORAGE_CABINET;
		query->m_bOwnerType = TOWNER_CHAR;
		query->m_dwOwnerID = dwCharID;
		for( WORD j=0; j<wCabinetItemCount; j++)
		{
			pBUF->m_packet
				>> query->m_dwStorageID;

			WrapItemQuery(&(pBUF->m_packet), query->m_item);
			if(!query->Call())
				return EC_NOERROR;
		}
		UNDEFINE_QUERY()
	}

	BYTE bSkillCount;
	pBUF->m_packet >> bSkillCount;

	DEFINE_QUERY(&m_db, CSPSaveSkill);
	query->m_dwCharID = dwCharID;
	for(BYTE i=0; i<bSkillCount; i++)
	{
		pBUF->m_packet
			>> query->m_wSkill
			>> query->m_bLevel
			>> query->m_dwTick;

		if(!query->Call())
			return EC_NOERROR;
	}
	UNDEFINE_QUERY();

	WORD wMaintainCnt;
	pBUF->m_packet >> wMaintainCnt;

	DEFINE_QUERY(&m_db, CSPSaveSkillMaintain);
	query->m_dwCharID = dwCharID;
	for(WORD i=0; i<wMaintainCnt; i++)
	{
		pBUF->m_packet
			>> query->m_wSkillID
			>> query->m_bLevel
			>> query->m_dwTick
			>> query->m_bAttackType
			>> query->m_dwAttackID
			>> query->m_bHostType
			>> query->m_dwHostID
			>> query->m_bAttackCountry;

		if(!query->Call())
			return EC_NOERROR;
	}
	UNDEFINE_QUERY();

	WORD wDuringItem;
	pBUF->m_packet
		>> wDuringItem;

	for(WORD i=0; i<wDuringItem; i++)
	{
		BYTE bKind;
		WORD wItemID;
		BYTE bType;
		DWORD dwRemain;
		__time64_t tEndTime;

		pBUF->m_packet
			>> bKind
			>> wItemID
			>> bType
			>> dwRemain
			>> tEndTime;

		if(bKind == IK_GOLDPREMIUM)
		{
			DEFINE_QUERY(&m_db, CSPSaveDuringItem)
			query->m_dwCharID = dwCharID;
			query->m_wItemID = wItemID;
			query->m_bType = bType;
			query->m_dwRemainTime = dwRemain;
			__TIMETODB(tEndTime, query->m_tEndTime);
			if(!query->Call())
				return EC_NOERROR;
			UNDEFINE_QUERY()
		}
		else
		{
			DEFINE_QUERY(&m_db, CSPSaveExpItem)
			query->m_dwCharID = dwCharID;
			query->m_wItemID = wItemID;
			query->m_bType = bType;
			query->m_dwRemainTime = dwRemain;
			__TIMETODB(tEndTime, query->m_tEndTime);
			if(!query->Call())
				return EC_NOERROR;
			UNDEFINE_QUERY()
		}
	}

	WORD wIUCnt;
	pBUF->m_packet
		>> wIUCnt;

	DEFINE_QUERY(&m_db, CSPSaveItemUsed)
	query->m_dwCharID = dwCharID;
	for(WORD i=0; i<wIUCnt; i++)
	{
		pBUF->m_packet
			>> query->m_wDelayGroupID
			>> query->m_dwTick;

		if(!query->Call())
			return EC_NOERROR;
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CSPSaveCharDataEnd)
	query->m_dwCharID = dwCharID;
	if(!query->Call())
		return EC_NOERROR;
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CSPSaveHotkey)
	WORD wHotkeyInvenCount;
	pBUF->m_packet >> wHotkeyInvenCount;
	for(WORD hk=0; hk<wHotkeyInvenCount; hk++)
	{
		query->m_dwCharID = dwCharID;
		pBUF->m_packet
			>> query->m_bSave
			>> query->m_bInvenID;

		for(BYTE kp=0; kp<MAX_HOTKEY_POS; kp++)
		{
			pBUF->m_packet
				>> query->m_bType[kp]
				>> query->m_wID[kp];
		}
		query->Call();
	}
	UNDEFINE_QUERY();

	DEFINE_QUERY( &m_db, CSPSaveQuest)
	query->m_dwCharID = dwCharID;

	WORD wCount;

	pBUF->m_packet
		>> wCount;

	for(int i=0; i<wCount; i++)
	{
		pBUF->m_packet
			>> query->m_dwQuestID
			>> query->m_dwTick
			>> query->m_bCompleteCount
			>> query->m_bTriggerCount;

		query->Call();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CSPSaveQuestTerm)
	query->m_dwCharID = dwCharID;
	WORD wCount;

	pBUF->m_packet
		>> wCount;

	for(int i=0; i<wCount; i++)
	{
		pBUF->m_packet
			>> query->m_dwQuestID
			>> query->m_dwTermID
			>> query->m_bTermType
			>> query->m_bCount;

		query->Call();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CSPSaveRecallMon)
	query->m_dwCharID = dwCharID;
	query->m_bDelete = TRUE;
	query->m_dwMonID = 0;
	query->m_wTempID = 0;
	query->m_wPetID = 0;
	query->m_dwAttr = 0;
	query->m_bLevel = 0;
	query->m_dwHP = 0;
	query->m_dwMP = 0;
	query->m_wPosX = 0;
	query->m_wPosY = 0;
	query->m_wPosZ = 0;
	query->m_dwTick = 0;
	query->Call();	//소환수 버프시킬 삭제

	WORD wCount;
	pBUF->m_packet
		>> wCount;

	query->m_bDelete = FALSE;
	for(WORD i=0; i<wCount; i++)
	{
		pBUF->m_packet
			>> query->m_dwMonID
			>> query->m_wTempID
			>> query->m_wPetID
			>> query->m_dwAttr
			>> query->m_bLevel
			>> query->m_dwHP
			>> query->m_dwMP
			>> query->m_bSkillLevel
			>> query->m_wPosX
			>> query->m_wPosY
			>> query->m_wPosZ
			>> query->m_dwTick;

		query->Call();
	}
	UNDEFINE_QUERY()

	pBUF->m_packet >> wMaintainCnt;

	DEFINE_QUERY(&m_db, CSPSaveRecallMaintain);
	query->m_dwCharID = dwCharID;
	for(WORD i=0; i<wMaintainCnt; i++)
	{
		pBUF->m_packet
			>> query->m_dwRecallID
			>> query->m_wSkillID
			>> query->m_bLevel
			>> query->m_dwTick
			>> query->m_bAttackType
			>> query->m_dwAttackID
			>> query->m_bHostType
			>> query->m_dwHostID
			>> query->m_bAttackCountry;

		query->Call();
	}
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CSPSavePet)
	query->m_dwCharID = dwCharID;

	BYTE bCount;
	pBUF->m_packet
		>> bCount;

	CString strPetName;
	__int64 timeUse;

	for(BYTE i=0; i<bCount; i++)
	{
		pBUF->m_packet
			>> query->m_wPetID
			>> strPetName
			>> timeUse;

		lstrcpy(query->m_szName, strPetName);
		__TIMETODB(timeUse, query->m_timeUse);

		query->Call();
	}

	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CSPSavePvPRecent)
	WORD wCount;
	INT64 dlDate;
	CString strName;
	pBUF->m_packet
		>> wCount;
	query->m_dwCharID = dwCharID;
	lstrcpy(query->m_szName, NAME_NULL);
	__TIMETODB(0, query->m_dlDate);
	query->Call();

	for(WORD i=0; i<wCount; i++)
	{
		pBUF->m_packet
			>> strName
			>> query->m_bClass
			>> query->m_bLevel
			>> query->m_bWin
			>> query->m_dwPoint
			>> dlDate;

		lstrcpy(query->m_szName, strName);
		__TIMETODB(dlDate, query->m_dlDate);
		query->Call();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db,CSPDuelCharAdd)
	CString strName;
	__time64_t dTime;
	WORD wCount;
	
	query->m_dwCharID = dwCharID;

	pBUF->m_packet
		>> wCount;

	lstrcpy(query->m_szName, NAME_NULL);
	__TIMETODB(0, query->m_timeDuel);
	query->Call();

	for(WORD i = 0 ; i < wCount; i++)
	{
		pBUF->m_packet
			>> strName
			>> query->m_bClass
			>> query->m_bLevel
			>> query->m_bWin
			>> query->m_dwPoint
			>> dTime;

		__TIMETODB(dTime,query->m_timeDuel);
		lstrcpy(query->m_szName,strName);

		query->Call();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db,CSPDuelScore)
	query->m_dwCharID = dwCharID;	

	for(BYTE i = 0; i < TCLASS_COUNT; i++)
	{
		pBUF->m_packet
			>> query->m_aDuelScore[i][0]
			>> query->m_aDuelScore[i][1];
	}

	query->Call();

	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CSPSaveProtectedOption)
	query->m_dwCharID = dwCharID;
	WORD wCount;

	pBUF->m_packet
		>> wCount;

	for(WORD i=0; i<wCount; i++)
	{
		pBUF->m_packet
            >> query->m_dwTarget
			>> query->m_bOption;

		query->Call();
	}

	UNDEFINE_QUERY()

	
	DEFINE_QUERY(&m_db,CSPSaveAuctionInterest)
	query->m_dwCharID = dwCharID;
	query->m_dwAuctionID = 0;
	query->Call();

	WORD wCount;
	pBUF->m_packet
		>> wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		pBUF->m_packet
			>> query->m_dwAuctionID;

		query->Call();
	}

	UNDEFINE_QUERY()
	 
	DEFINE_QUERY(&m_db,CSPSaveMonthPvPoint)
	query->m_dwCharID = dwCharID;

	CString strMonthSay;
	pBUF->m_packet
		>> query->m_dwPoint
		>> query->m_wWin
		>> query->m_wLose
		>> strMonthSay
		>> query->m_bCountry;
	
	lstrcpy(query->m_szSay, strMonthSay);
	if(query->m_dwPoint)
		query->Call();

	UNDEFINE_QUERY()

	pBUF->m_packet
		>> wACKMsg;

	if(wACKMsg)
	{
		LPPACKETBUF pMSG = new PACKETBUF();

		pMSG->m_packet.SetID(wACKMsg)
			<< dwCharID
			<< dwKEY;

		SayToBATCH(pMSG);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_SAVEITEM_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	MAPTINVEN::iterator itTINVEN;
	MAPTINVEN mapNEWINVEN;

	mapNEWINVEN.clear();

	BYTE bInvenCount;

	pBUF->m_packet
		>> bInvenCount;

	DEFINE_QUERY(&m_db, CSPSaveItemDataStart)
	query->m_dwCharID = dwCharID;
	if(!query->Call())
		return EC_NOERROR;

	UNDEFINE_QUERY();

	for( WORD i=0; i<bInvenCount; i++)
	{
		BYTE bInvenID;
		__time64_t dEndTime;
		BYTE bItemCount;
		DEFINE_QUERY( &m_db, CSPSaveInven)
		query->m_dwCharID = dwCharID;
		pBUF->m_packet
			>> bInvenID
			>> query->m_wItemID
			>> dEndTime
			>> query->m_bELD
			>> bItemCount;

		__TIMETODB(dEndTime, query->m_dEndTime);
		query->m_bInvenID = bInvenID;
		if(!query->Call())
			return EC_NOERROR;

		UNDEFINE_QUERY()

		DEFINE_QUERY( &m_db, CSPSaveItem)
		query->m_bStorageType = STORAGE_INVEN;
		query->m_dwStorageID = bInvenID;
		query->m_bOwnerType = TOWNER_CHAR;
		query->m_dwOwnerID = dwCharID;
		for( BYTE j=0; j<bItemCount; j++)
		{
			WrapItemQuery(&(pBUF->m_packet), query->m_item);
			if(!query->Call())
				return EC_NOERROR;
		}
		UNDEFINE_QUERY()
	}

	DEFINE_QUERY(&m_db, CSPSaveItemDataEnd)
	query->m_dwCharID = dwCharID;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_LOGOUT_REQ( LPPACKETBUF pBUF)
{
	DEFINE_QUERY( &m_db, CSPLogout)
	DWORD dwCharID;
	DWORD dwKEY;
	WORD wACKMsg;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> query->m_dwID
		>> wACKMsg;

	query->m_dwCharID = dwCharID;
	query->Call();

	if(wACKMsg)
	{
		LPPACKETBUF pMSG = new PACKETBUF();

		pMSG->m_packet.SetID(wACKMsg)
			<< dwCharID
			<< dwKEY;

		SayToBATCH(pMSG);
	}
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_DORELEASEMAIN_ACK( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	SendMW_RELEASEMAIN_ACK(pPlayer->ProtectTutorial() ? TRUE : FALSE, pPlayer);

	pPlayer->ClearMain();
	SendSM_POSTBILLERASE_REQ(
		pPlayer->m_dwID,
		0);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CHGPARTYTYPE_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	BYTE bRet;
	BYTE bPartyType;

	pBUF->m_packet
		>> bRet
		>> bPartyType;

	if(!bRet)
		pPlayer->m_bPartyType = bPartyType;

	pPlayer->SendCS_CHGPARTYTYPE_ACK(bRet, bPartyType);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_PARTYADD_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwOrigin;
	CString strOrigin;
	CString strTarget;
	BYTE bObtainType;
	BYTE bReply;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strOrigin
		>> strTarget
		>> bObtainType
		>> bReply
		>> dwOrigin;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer || !pPlayer->m_bMain || !pPlayer->m_pMAP)
	{
		SendMW_PARTYJOIN_ACK(
			strOrigin,
			strTarget,
			bObtainType,
			ASK_BUSY,
			0,
			0,
			0,
			0);

		return EC_NOERROR;
	}

	if( bReply != PARTY_AGREE )
	{
		pPlayer->SendCS_PARTYADD_ACK(
			strOrigin,
			strTarget,
			bReply);

		return EC_NOERROR;
	}

	if( pPlayer->IsActionBlock() )
	{
		SendMW_PARTYJOIN_ACK(
			strOrigin,
			pPlayer->m_strNAME,
			bObtainType,
			ASK_BUSY,
			pPlayer->GetMaxHP(),
			pPlayer->m_dwHP,
			pPlayer->GetMaxMP(),
			pPlayer->m_dwMP);

		return EC_NOERROR;
	}

	if(!pPlayer->CheckProtected(dwOrigin, PROTECTED_INVITE))
	{
		SendMW_PARTYJOIN_ACK(
			strOrigin,
			pPlayer->m_strNAME,
			bObtainType,
			ASK_NO,
			pPlayer->GetMaxHP(),
			pPlayer->m_dwHP,
			pPlayer->GetMaxMP(),
			pPlayer->m_dwMP);

		return EC_NOERROR;
	}

	pPlayer->SendCS_PARTYJOINASK_ACK(
		strOrigin,
		bObtainType);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_PARTYJOIN_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	CString strMemberID;
	DWORD dwMemberID;
	CString strGuildName;
	BYTE bLevel;
	DWORD dwMaxHP;
	DWORD dwHP;
	DWORD dwMaxMP;
	DWORD dwMP;
	BYTE bRace;
	BYTE bSex;
	BYTE bFace;
	BYTE bHair;
	BYTE bClass;

	pBUF->m_packet
		>> pPlayer->m_wPartyID
		>> strMemberID
		>> dwMemberID
		>> pPlayer->m_dwPartyChiefID
		>> pPlayer->m_wCommanderID
		>> strGuildName
		>> bLevel
		>> dwMaxHP
		>> dwHP
		>> dwMaxMP
		>> dwMP
		>> bRace
		>> bSex
		>> bFace
		>> bHair
		>> pPlayer->m_bPartyType
		>> bClass;

	pPlayer->SendCS_PARTYJOIN_ACK(
		pPlayer->GetPartyID(),
		strMemberID,
		dwMemberID,
		pPlayer->GetPartyChiefID(),
		pPlayer->GetCommanderID(),
		strGuildName,
		bLevel,
		dwMaxHP,
		dwHP,
		dwMaxMP,
		dwMP,
		bRace,
		bSex,
		bFace,
		bHair,
		pPlayer->m_bPartyType,
		bClass);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_PARTYDEL_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	WORD wPartyID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	CTPlayer *pPlayer = FindPlayer(
		dwCharID,
		dwKEY);

	if(!pPlayer)
		return EC_NOERROR;

	DWORD dwMemberID;
	DWORD dwChiefID;
	WORD wCommander;
	BYTE bKicked;

	pBUF->m_packet
		>> dwMemberID
		>> dwChiefID
		>> wCommander
		>> wPartyID
		>> bKicked;

	pPlayer->SendCS_PARTYDEL_ACK(
		dwMemberID,
		pPlayer->GetPartyChiefID(),
		pPlayer->GetCommanderID(),
		pPlayer->GetPartyID(),
		bKicked);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_PARTYMANSTAT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwID;
	BYTE bType;
	BYTE bLevel;
	DWORD dwMaxHP, dwCurHP;
	DWORD dwMaxMP, dwCurMP;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwID
		>> bType
		>> bLevel
		>> dwMaxHP
		>> dwCurHP
		>> dwMaxMP
		>> dwCurMP;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer) 
		return EC_NOERROR;

	pPlayer->SendCS_PARTYMANSTAT_ACK(
		dwID,
		bType,
		bLevel,
		dwMaxHP,
		dwCurHP,
		dwMaxMP,
		dwCurMP);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_LEVELUP_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bLevel;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bLevel;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer) 
		return EC_NOERROR;

	pPlayer->m_bLevel = bLevel;
	return EC_NOERROR;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 길드
DWORD CTMapSvrModule::OnMW_GUILDESTABLISH_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bRet;
	DWORD dwGuildID;
	CString strGuildName;
	BYTE bEstablish;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bRet
		>> dwGuildID
		>> strGuildName
		>> bEstablish;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

	if(bEstablish)
		pPlayer->SendCS_GUILDESTABLISH_ACK(bRet, dwGuildID, strGuildName);

#ifdef	DEF_UDPLOGGUILD
	m_pUdpSocket->LogGuild(LOGMAP_GUILDCREATE, pPlayer, 0, "", dwGuildID, strGuildName, bRet );
#endif	//	DEF_UDPLOGGUILD

	if(!bRet)
	{
		pPlayer->m_dwGuildID = dwGuildID;
		pPlayer->m_bGuildDuty = GUILD_DUTY_CHIEF;
		pPlayer->m_bGuildPeer = GUILD_PEER_NONE;
		pPlayer->m_strGuildName = strGuildName;

		if(pPlayer->m_bMain && pPlayer->m_pMAP)
		{
			VPLAYER vPlayer;
			vPlayer.clear();

			pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);

			for(DWORD i=0; i<vPlayer.size(); i++)
			{
				vPlayer[i]->SendCS_GUILDATTR_ACK(
					dwCharID, 
					dwGuildID, 
					0, 
					0,
					strGuildName, 
					GUILD_PEER_NONE,
					pPlayer->m_dwTacticsID,
					pPlayer->m_strTacticsName);
			}

			vPlayer.clear();
		}
	}
	
	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDDISORGANIZATION_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bDisorg;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bDisorg;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);

#ifdef	DEF_UDPLOGGUILD
	m_pUdpSocket->LogGuildDisorganization(LOGMAP_GUILDDISORGANIZATION, pPlayer, bDisorg);
#endif	//	DEF_UDPLOGGUILD

	if(pPlayer)
		pPlayer->SendCS_GUILDDISORGANIZATION_ACK(bDisorg);	

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDINVITE_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	CString strGuildName;
	DWORD dwInviter;
	CString strInviterName;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strGuildName
		>> dwInviter
		>> strInviterName;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer ||
		!pPlayer->m_bMain ||
		!pPlayer->m_pMAP)
		return EC_NOERROR;

	CTPlayer * pInviter = FindPlayer(strInviterName);
	if(pPlayer->m_dwGuildID)
	{
		if(pInviter)
			pInviter->SendCS_GUILDINVITE_ACK(GUILD_HAVEGUILD, strGuildName, 0, strInviterName);
		return EC_NOERROR;
	}

	DWORD dwTime = (DWORD)m_timeCurrent-pPlayer->m_dwGuildLeaveTime;
	if(pPlayer->m_bGuildLeave==GUILD_LEAVE_SELF)
	{
		if(dwTime < GUILD_LEAVE_DURATION)
		{
			if(pInviter)
				pInviter->SendCS_GUILDINVITE_ACK(GUILD_LEAVE_SELF, strGuildName, 0, strInviterName);
			return EC_NOERROR;
		}
	}
	else if(pPlayer->m_bGuildLeave==GUILD_LEAVE_DISORGANIZATION)
	{
		if(dwTime < GUILD_DIS_DURATION)
		{
			if(pInviter)
				pInviter->SendCS_GUILDINVITE_ACK(GUILD_LEAVE_DISORGANIZATION, strGuildName, 0, strInviterName);
			return EC_NOERROR;
		}
	}
	else if(!pPlayer->CheckProtected(dwInviter, PROTECTED_INVITE))
	{
		if(pInviter)
			pInviter->SendCS_GUILDINVITE_ACK(GUILD_JOIN_DENY, strGuildName, 0, strInviterName);
		return EC_NOERROR;
	}

#ifdef DEF_UDPLOGGUILD

	m_pUdpSocket->LogGuild( LOGMAP_GUILDINVITE, pPlayer, dwInviter, strInviterName, pPlayer->m_dwGuildID, pPlayer->m_strGuildName, GUILD_SUCCESS );

#endif	//	DEF_UDPLOGGUILD

	pPlayer->SendCS_GUILDINVITE_ACK(
		GUILD_SUCCESS,
		strGuildName,
		dwInviter,
		strInviterName);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_GUILDJOIN_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bRet;
	DWORD dwGuildID;
	DWORD dwFame;
	DWORD dwFameColor;
	CString strGuildName;
	DWORD dwNewMemberID;
	CString strNewMemberName;
	BYTE bMaxGuildMember;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bRet
		>> dwGuildID
		>> dwFame
		>> dwFameColor
		>> strGuildName
		>> dwNewMemberID
		>> strNewMemberName
		>> bMaxGuildMember;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

#ifdef	DEF_UDPLOGGUILD
	m_pUdpSocket->LogGuild(LOGMAP_GUILDJOIN, pPlayer, dwNewMemberID, strNewMemberName, dwGuildID, strGuildName, bRet );
#endif	//	DEF_UDPLOGGUILD

	pPlayer->SendCS_GUILDJOIN_ACK(
		bRet,
		dwGuildID,
		strGuildName,
		dwNewMemberID,
		strNewMemberName,
		bMaxGuildMember);

	if(dwCharID == dwNewMemberID)
	{
		pPlayer->m_dwGuildID = dwGuildID;
		pPlayer->m_strGuildName = strGuildName;
		pPlayer->m_dwFame = dwFame;
		pPlayer->m_dwFameColor = dwFameColor;
		pPlayer->m_bGuildDuty = GUILD_DUTY_NONE;
		pPlayer->m_bGuildPeer = GUILD_PEER_NONE;
		pPlayer->m_bGuildLeave = 0;
		pPlayer->m_dwGuildLeaveTime = 0;

		if(pPlayer->m_bMain && pPlayer->m_pMAP)
		{
			VPLAYER vPlayer;
			vPlayer.clear();

			pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);

			for(DWORD i=0; i<vPlayer.size(); i++)
			{
				vPlayer[i]->SendCS_GUILDATTR_ACK(
					dwCharID, 
					dwGuildID, 
					dwFame,
					dwFameColor,
					strGuildName, 
					GUILD_PEER_NONE,
					pPlayer->m_dwTacticsID,
					pPlayer->m_strTacticsName);
			}

			vPlayer.clear();
		}
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDDUTY_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	CString strTarget;
	BYTE bDuty;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strTarget
		>> bDuty;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(pPlayer)
	{

#ifdef	DEF_UDPLOGGUILD
		m_pUdpSocket->LogGuildDutyPeer(LOGMAP_GUILDDUTY, pPlayer, 0, strTarget, pPlayer->m_dwGuildID, pPlayer->m_strGuildName, bDuty, GUILD_SUCCESS );
#endif	//	DEF_UDPLOGGUILD

		pPlayer->SendCS_GUILDDUTY_ACK(GUILD_SUCCESS, strTarget, bDuty);

		if(pPlayer->m_strNAME == strTarget)
			pPlayer->m_bGuildDuty = bDuty;
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDPEER_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bResult;
	CString strTarget;
	BYTE bPeer;
	BYTE bOldPeer;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult
		>> strTarget
		>> bPeer
		>> bOldPeer;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

#ifdef	DEF_UDPLOGGUILD
	m_pUdpSocket->LogGuildDutyPeer(LOGMAP_GUILDPEER, pPlayer, 0, strTarget, pPlayer->m_dwGuildID, pPlayer->m_strGuildName, bPeer, bResult );
#endif	//	DEF_UDPLOGGUILD

	pPlayer->SendCS_GUILDPEER_ACK(bResult, strTarget, bPeer, bOldPeer);

	if(!bResult && pPlayer->m_strNAME == strTarget)
	{
		pPlayer->m_bGuildPeer = bPeer;
		
		if(pPlayer->m_bMain && pPlayer->m_pMAP)
		{
			VPLAYER vPlayer;
			vPlayer.clear();

			pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);

			for(DWORD i=0; i<vPlayer.size(); i++)
			{
				vPlayer[i]->SendCS_GUILDATTR_ACK(
					dwCharID, 
					pPlayer->m_dwGuildID, 
					pPlayer->m_dwFame,
					pPlayer->m_dwFameColor,
					pPlayer->m_strGuildName, 
					bPeer,
					pPlayer->m_dwTacticsID,
					pPlayer->m_strTacticsName);
			}

			vPlayer.clear();
		}
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDLEAVE_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	CString strTarget;
	BYTE bReason;
	DWORD dwTime;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strTarget
		>> bReason
		>> dwTime;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

#ifdef	DEF_UDPLOGGUILD
	m_pUdpSocket->LogGuild(LOGMAP_GUILDLEAVE, pPlayer, 0,strTarget, pPlayer->m_dwGuildID, pPlayer->m_strGuildName, bReason);
#endif	//	DEF_UDPLOGGUILD

	pPlayer->SendCS_GUILDLEAVE_ACK(GUILD_SUCCESS, strTarget, bReason);

	if(pPlayer->m_strNAME == strTarget)
	{
		if(bReason == GUILD_LEAVE_SELF)
		{
			pPlayer->m_bGuildLeave = GUILD_LEAVE_SELF;
			pPlayer->m_dwGuildLeaveTime = dwTime;
		}
		else if(bReason == GUILD_LEAVE_DISORGANIZATION && pPlayer->m_bGuildDuty == GUILD_DUTY_CHIEF)
		{
			pPlayer->m_bGuildLeave = GUILD_LEAVE_DISORGANIZATION;
			pPlayer->m_dwGuildLeaveTime = dwTime;
		}

		pPlayer->ResetCloak();

		pPlayer->m_dwGuildID = 0;
		pPlayer->m_bGuildDuty = 0;
		pPlayer->m_bGuildPeer = 0;
		pPlayer->m_strGuildName = NAME_NULL;
		pPlayer->m_dwFame = 0;
		pPlayer->m_dwFameColor = 0;
		if(!pPlayer->m_dwTacticsID)
		{
			pPlayer->m_wCastle = 0;
			pPlayer->m_bCamp = 0;
		}

		if(pPlayer->m_bMain && pPlayer->m_pMAP)
		{
			VPLAYER vPlayer;
			vPlayer.clear();

			pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);

			for(DWORD i=0; i<vPlayer.size(); i++)
			{
				vPlayer[i]->SendCS_GUILDATTR_ACK(
					dwCharID, 
					pPlayer->m_dwGuildID, 
					pPlayer->m_dwFame,
					pPlayer->m_dwFameColor,
					pPlayer->m_strGuildName, 
					GUILD_PEER_NONE,
					pPlayer->m_dwTacticsID,
					pPlayer->m_strTacticsName);
			}

			vPlayer.clear();

			if(pPlayer->IsInCastle())
				Teleport(pPlayer, pPlayer->m_wLastSpawnID ? pPlayer->m_wLastSpawnID : pPlayer->m_wSpawnID);
		}
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDMEMBERLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(pPlayer)
		pPlayer->SendCS_GUILDMEMBERLIST_ACK(pBUF);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDINFO_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(pPlayer)
		pPlayer->SendCS_GUILDINFO_ACK(pBUF);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_GUILDCABINETLIST_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bMaxCabinet;
	BYTE bItemCnt;
	DWORD dwStItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bMaxCabinet
		>> bItemCnt;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer || !pPlayer->m_bMain)
		return EC_NOERROR;

	VTITEM vItem;
	vItem.clear();

	for(BYTE i=0; i<bItemCnt; i++)
	{
		pBUF->m_packet
			>> dwStItemID;

		CTItem * pItem = CreateItem(&(pBUF->m_packet));
		if(pItem)
		{
			pItem->m_dwStItemID = dwStItemID;
			vItem.push_back(pItem);
		}
	}

	pPlayer->SendCS_GUILDCABINETLIST_ACK(bMaxCabinet, vItem);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnDM_GUILDCABINETPUTIN_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bRet;
	DWORD dwItemID = 0;
	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	DEFINE_QUERY(&m_db, CSPGuildItemPutIn)
	query->m_dwItemID = 0;
	pBUF->m_packet
		>> query->m_dwGuildID
		>> bCount;

	WrapItemQuery(&(pBUF->m_packet), query->m_item);
	query->m_item.m_bCount = bCount;
	if(query->Call())
	{
		bRet = BYTE(query->m_nRET);
		dwItemID = query->m_dwItemID;
	}
	else
		bRet = GUILD_NOTFOUND;
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDCABINETPUTIN_ACK)
		<< dwCharID
		<< dwKEY
		<< bRet
		<< dwItemID;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnDM_GUILDCABINETPUTIN_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bRet;
	DWORD dwItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bRet
		>> dwItemID;

	CTPlayer *pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_GUILDCABINETPUTIN_ACK(bRet);

	if(bRet)
	{
		pPlayer->BackGuildItem();
		SendMW_GUILDCABINETLIST_ACK(dwCharID, dwKEY);
		return EC_NOERROR;
	}

#ifdef	DEF_UDPLOGGUILD
	m_pUdpSocket->LogGuildCabinet(	LOGMAP_GUILDCABINETPUTIN,  pPlayer,  pPlayer->m_dwGuildID,  pPlayer->m_strGuildName,  0, pPlayer->m_guildItem, pPlayer->m_guildItem->m_bCount, 0);
#endif	//	DEF_UDPLOGGUILD

	SendMW_GUILDCABINETPUTIN_ACK(dwCharID, dwKEY, dwItemID, pPlayer->m_guildItem);
	pPlayer->ClearGuildItem();

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnDM_GUILDCABINETTAKEOUT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwGuildID;
	DWORD dwItemID;
	BYTE bCount;
	BYTE bInvenID;
	BYTE bItemID;
	BYTE bRet;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwGuildID
		>> dwItemID
		>> bCount
		>> bInvenID
		>> bItemID;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDCABINETTAKEOUT_ACK)
		<< dwCharID
		<< dwKEY
		<< dwGuildID
		<< dwItemID
		<< bInvenID
		<< bItemID;

	DEFINE_QUERY(&m_db, CSPGuildItemTakeOut)
	query->m_dwGuildID = dwGuildID;
	query->m_dwItemID = dwItemID;
	query->m_item.m_bCount = bCount;

    if(query->Call())
		bRet = BYTE(query->m_nRET);
	else
		bRet = GUILD_NOTFOUND;

	pMSG->m_packet
		<< bRet;

	if(!bRet)
		WrapItem(
			&(pMSG->m_packet),
            query->m_item);

	UNDEFINE_QUERY()

	SayToBATCH(pMSG);
	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnDM_GUILDCABINETTAKEOUT_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwGuildID;
	DWORD dwItemID;
	BYTE bInvenID;
	BYTE bItemID;
	BYTE bRet;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwGuildID
		>> dwItemID
		>> bInvenID
		>> bItemID
		>> bRet;

	CTPlayer *pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(bRet)
	{
		pPlayer->SendCS_GUILDCABINETTAKEOUT_ACK(bRet);
		SendMW_GUILDCABINETLIST_ACK(dwCharID, dwKEY);
		return EC_NOERROR;
	}

	CTItem * pItem = CreateItem(&(pBUF->m_packet));

	if(!pItem)
		return EC_NOERROR;

#ifdef	DEF_UDPLOGGUILD
	m_pUdpSocket->LogGuildCabinet(LOGMAP_GUILDCABINETTAKEOUT, pPlayer, pPlayer->m_dwGuildID, pPlayer->m_strGuildName, 0, pItem, pItem->m_bCount, 0);
#endif	//	DEF_UDPLOGGUILD

	CTInven * pTInvenDEST = pPlayer->FindTInven(bInvenID);
	if(!pTInvenDEST)
	{
		pPlayer->SendCS_GUILDCABINETTAKEOUT_ACK(GUILD_CABINET_FAIL);
		SendMW_GUILDCABINETLIST_ACK(dwCharID, dwKEY);

		SendDM_GUILDCABINETROLLBACK_REQ(
			dwGuildID, 
			dwItemID, 
			pItem);
		delete pItem;
		return EC_NOERROR;
	}

	CTItem *pTItemDEST = pTInvenDEST->FindTItem(bItemID);
	if(!pTItemDEST)
	{
		pItem->m_bItemID = bItemID;

		pTInvenDEST->m_mapTITEM.insert( MAPTITEM::value_type( pItem->m_bItemID, pItem));
		pPlayer->SendCS_ADDITEM_ACK(
			pItem,
			pTInvenDEST->m_bInvenID);

		pPlayer->SendCS_GUILDCABINETTAKEOUT_ACK(bRet);

		SendMW_GUILDCABINETTAKEOUT_ACK(
			dwCharID,
			dwKEY,
			dwItemID,
			pItem->m_bCount);

		SendDM_SAVEITEM_REQ(pPlayer); // 인벤-아이탬 디비 올림
	}
	else
	{
		BYTE bItemCount = pItem->m_bCount;

		VTITEM vItem;
		vItem.clear();
		vItem.push_back(pItem);
		if(pPlayer->CanPush(&vItem, 0))
		{
			pPlayer->PushTItem(&vItem);
			pPlayer->SendCS_GUILDCABINETTAKEOUT_ACK(bRet);

			SendMW_GUILDCABINETTAKEOUT_ACK(
				dwCharID,
				dwKEY,
				dwItemID,
				bItemCount);

			SendDM_SAVEITEM_REQ(pPlayer); // 인벤-아이탬 디비 올림
		}
		else
		{
			pPlayer->SendCS_GUILDCABINETTAKEOUT_ACK(GUILD_CABINET_INVENFULL);
			SendMW_GUILDCABINETLIST_ACK(dwCharID, dwKEY);

			SendDM_GUILDCABINETROLLBACK_REQ(
				dwGuildID, 
				dwItemID, 
				pItem);
			delete pItem;
		}
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnDM_GUILDCABINETROLLBACK_REQ(LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwItemID;

	pBUF->m_packet
		>> dwGuildID
		>> dwItemID;

	DEFINE_QUERY(&m_db, CSPGuildItemRollback)
	query->m_dwGuildID = dwGuildID;
	query->m_dwItemID = dwItemID;
	WrapItemQuery(&(pBUF->m_packet), query->m_item);
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDCONTRIBUTION_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bResult;
	DWORD dwExp;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;
	DWORD dwPvPoint;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult
		>> dwExp
		>> dwGold
		>> dwSilver
		>> dwCooper
		>> dwPvPoint;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

	if(pPlayer->ProtectTutorial())
		return EC_NOERROR;

	if(!bResult)
	{
		if(dwExp && pPlayer->UseExp(dwExp, TRUE))
		{
			pPlayer->SendCS_EXP_ACK();
			SendDM_SAVEEXP_REQ(pPlayer->m_dwID, pPlayer->m_dwEXP);
		}

		__int64 llMoney = CalcMoney(dwGold, dwSilver, dwCooper); 
		if(llMoney && pPlayer->UseMoney(llMoney, TRUE))
		{
			pPlayer->SendCS_MONEY_ACK();
			SendDM_SAVEMONEY_REQ(pPlayer->m_dwID, pPlayer->m_dwGold, pPlayer->m_dwSilver, pPlayer->m_dwCooper);
		}

		if(dwPvPoint)
			pPlayer->UsePvPoint(dwPvPoint, PVPE_GUILD, PVP_USEABLE);
	}

#ifdef	DEF_UDPLOGGUILD
	m_pUdpSocket->LogGuildExpMoeny( LOGMAP_GUILDCONTRIBUTION, pPlayer, dwExp, dwGold, dwSilver, dwCooper, dwPvPoint, bResult);
#endif	//	DEF_UDPLOGGUILD


	pPlayer->SendCS_GUILDCONTRIBUTION_ACK(bResult);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDARTICLELIST_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(pPlayer)
		pPlayer->SendCS_GUILDARTICLELIST_ACK(pBUF);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDARTICLEADD_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(pPlayer)
		pPlayer->SendCS_GUILDARTICLEADD_ACK(bResult);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDARTICLEDEL_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(pPlayer)
		pPlayer->SendCS_GUILDARTICLEDEL_ACK(bResult);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDARTICLEUPDATE_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(pPlayer)
		pPlayer->SendCS_GUILDARTICLEUPDATE_ACK(bResult);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDFAME_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bResult;
	DWORD dwID;
	DWORD dwFame;
	DWORD dwFameColor;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult
		>> dwID
		>> dwFame
		>> dwFameColor;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

	if(dwID == dwCharID)
		pPlayer->SendCS_GUILDFAME_ACK(bResult, dwFame, dwFameColor);

	if(bResult == GUILD_SUCCESS)
	{
		pPlayer->m_dwFame = dwFame;
		pPlayer->m_dwFameColor = dwFameColor;

		if(pPlayer->m_bMain && pPlayer->m_pMAP)
		{
			VPLAYER vPlayer;
			vPlayer.clear();

			pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);

			for(DWORD i=0; i<vPlayer.size(); i++)
			{
				vPlayer[i]->SendCS_GUILDATTR_ACK(
					dwCharID, 
					pPlayer->m_dwGuildID, 
					pPlayer->m_dwFame,
					pPlayer->m_dwFameColor,
					pPlayer->m_strGuildName, 
					pPlayer->m_bGuildPeer,
					pPlayer->m_dwTacticsID,
					pPlayer->m_strTacticsName);
			}

			vPlayer.clear();
		}
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDWANTEDADD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDWANTEDADD_ACK(bResult);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_GUILDWANTEDDEL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDWANTEDDEL_ACK(bResult);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_GUILDWANTEDLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;


	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDWANTEDLIST_ACK(&(pBUF->m_packet));

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDVOLUNTEERING_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDVOLUNTEERING_ACK(bResult);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDVOLUNTEERINGDEL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDVOLUNTEERINGDEL_ACK(bResult);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDVOLUNTEERLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDVOLUNTEERLIST_ACK(&(pBUF->m_packet));

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDVOLUNTEERREPLY_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDVOLUNTEERREPLY_ACK(bResult);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDTACTICSWANTEDADD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDTACTICSWANTEDADD_ACK(bResult);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDTACTICSWANTEDDEL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDTACTICSWANTEDDEL_ACK(bResult);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDTACTICSWANTEDLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDTACTICSWANTEDLIST_ACK(&(pBUF->m_packet));

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDTACTICSVOLUNTEERING_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDTACTICSVOLUNTEERING_ACK(bResult);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDTACTICSVOLUNTEERINGDEL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDTACTICSVOLUNTEERINGDEL_ACK(bResult);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDTACTICSVOLUNTEERLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDTACTICSVOLUNTEERLIST_ACK(&(pBUF->m_packet));

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDTACTICSREPLY_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;
	DWORD dwGuildID;
	CString strGuildName;
	DWORD dwMemberID;
	CString strMemberName;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult
		>> dwGuildID
		>> strGuildName
		>> dwMemberID
		>> strMemberName
		>> dwGold
		>> dwSilver
		>> dwCooper;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_GUILDTACTICSREPLY_ACK(bResult, dwMemberID);

	if(!bResult)
	{
		if(dwCharID == dwMemberID)
		{
#ifdef DEF_UDPLOG
			m_pUdpSocket->LogTactics(LOGMAP_TACTICSADD, pPlayer, dwGuildID, strGuildName);
#endif
			pPlayer->m_dwTacticsID = dwGuildID;
			pPlayer->m_strTacticsName = strGuildName;
			pPlayer->m_wCastle = 0;
			pPlayer->m_bCamp = 0;

			if(pPlayer->m_bMain && pPlayer->m_pMAP)
			{
				VPLAYER vPlayer;
				vPlayer.clear();

				pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);

				for(DWORD i=0; i<vPlayer.size(); i++)
				{
					vPlayer[i]->SendCS_GUILDATTR_ACK(
						dwCharID, 
						pPlayer->m_dwGuildID, 
						pPlayer->m_dwFame,
						pPlayer->m_dwFameColor,
						pPlayer->m_strGuildName, 
						pPlayer->m_bGuildPeer,
						pPlayer->m_dwTacticsID,
						pPlayer->m_strTacticsName);
				}

				vPlayer.clear();

				if(pPlayer->IsInCastle())
					Teleport(pPlayer, pPlayer->m_wLastSpawnID ? pPlayer->m_wLastSpawnID : pPlayer->m_wSpawnID);
			}
		}
		else
		{
			CString strMessage;
			strMessage.Format(GetSvrMsg(MSG_TACTICS_MESSAGE), strGuildName);

			SendDM_POSTRECV_REQ(
				dwCharID,
				dwMemberID,
				pPlayer->m_strNAME,
				strMemberName,
				BuildNetString(NAME_NULL, GetSvrMsg(MSG_TACTICS_TITLE)),
				BuildNetString(NAME_NULL, strMessage),
				POST_PACKATE,
				0,
				0,
				0,
				NULL);
		}
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDTACTICSKICKOUT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;
	DWORD dwTarget;
	BYTE bKick;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult
		>> dwTarget
		>> bKick;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_GUILDTACTICSKICKOUT_ACK(bResult, dwTarget);

	if(!bResult && dwCharID == dwTarget)
	{
#ifdef DEF_UDPLOG
		if(bKick)
			m_pUdpSocket->LogTactics(LOGMAP_TACTICSKICKOUT, pPlayer, pPlayer->m_dwTacticsID, pPlayer->m_strTacticsName);
		else
			m_pUdpSocket->LogTactics(LOGMAP_TACTICSLEAVE, pPlayer, pPlayer->m_dwTacticsID, pPlayer->m_strTacticsName);
#endif
		pPlayer->m_dwTacticsID = 0;
		pPlayer->m_strTacticsName = NAME_NULL;
		pPlayer->m_wCastle = 0;
		pPlayer->m_bCamp = 0;

		if(pPlayer->m_bMain && pPlayer->m_pMAP)
		{
			VPLAYER vPlayer;
			vPlayer.clear();

			pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);

			for(DWORD i=0; i<vPlayer.size(); i++)
			{
				vPlayer[i]->SendCS_GUILDATTR_ACK(
					dwCharID, 
					pPlayer->m_dwGuildID, 
					pPlayer->m_dwFame,
					pPlayer->m_dwFameColor,
					pPlayer->m_strGuildName, 
					pPlayer->m_bGuildPeer,
					pPlayer->m_dwTacticsID,
					pPlayer->m_strTacticsName);
			}

			vPlayer.clear();

			if(pPlayer->IsInCastle())
				Teleport(pPlayer, pPlayer->m_wLastSpawnID ? pPlayer->m_wLastSpawnID : pPlayer->m_wSpawnID);
		}
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDTACTICSINVITE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	CString strGuildName;
	CString strName;
	BYTE bDay;
	DWORD dwPoint;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strGuildName
		>> strName
		>> bDay
		>> dwPoint
		>> dwGold
		>> dwSilver
		>> dwCooper;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_GUILDTACTICSINVITE_ACK(
			strGuildName,
			strName,
			bDay,
			dwPoint,
			dwGold,
			dwSilver,
			dwCooper);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDTACTICSANSWER_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;
	DWORD dwGuildID;
	CString strGuildName;
	DWORD dwMemberID;
	CString strMemberName;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult
		>> dwGuildID
		>> strGuildName
		>> dwMemberID
		>> strMemberName
		>> dwGold
		>> dwSilver
		>> dwCooper;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_GUILDTACTICSANSWER_ACK(bResult, dwMemberID);

	if(!bResult)
	{
		if(dwCharID == dwMemberID)
		{
#ifdef DEF_UDPLOG
			m_pUdpSocket->LogTactics(LOGMAP_TACTICSADD, pPlayer, dwGuildID, strGuildName);
#endif
			pPlayer->m_dwTacticsID = dwGuildID;
			pPlayer->m_strTacticsName = strGuildName;
			pPlayer->m_wCastle = 0;
			pPlayer->m_bCamp = 0;

			if(pPlayer->m_bMain && pPlayer->m_pMAP)
			{
				VPLAYER vPlayer;
				vPlayer.clear();

				pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);

				for(DWORD i=0; i<vPlayer.size(); i++)
				{
					vPlayer[i]->SendCS_GUILDATTR_ACK(
						dwCharID, 
						pPlayer->m_dwGuildID, 
						pPlayer->m_dwFame,
						pPlayer->m_dwFameColor,
						pPlayer->m_strGuildName, 
						pPlayer->m_bGuildPeer,
						pPlayer->m_dwTacticsID,
						pPlayer->m_strTacticsName);
				}

				vPlayer.clear();

				if(pPlayer->IsInCastle())
					Teleport(pPlayer, pPlayer->m_wLastSpawnID ? pPlayer->m_wLastSpawnID : pPlayer->m_wSpawnID);
			}
		}
		else
		{
			CString strMessage;
			strMessage.Format(GetSvrMsg(MSG_TACTICS_MESSAGE), strGuildName);

			SendDM_POSTRECV_REQ(
				dwCharID,
				dwMemberID,
				pPlayer->m_strNAME,
				strMemberName,
				BuildNetString(NAME_NULL, GetSvrMsg(MSG_TACTICS_TITLE)),
				BuildNetString(NAME_NULL, strMessage),
				POST_PACKATE,
				0,
				0,
				0,
				NULL);
		}
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_GUILDTACTICSLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(pPlayer)
		pPlayer->SendCS_GUILDTACTICSLIST_ACK(pBUF);

	return EC_NOERROR;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CTMapSvrModule::OnMW_CHAT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bChannel;
	BYTE bType;
	BYTE bGroup;
	DWORD dwSenderID;
	CString strSenderName;
	BYTE bCountry;
	BYTE bAidCountry;
	DWORD dwTarget;
	CString strTalk;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bChannel
		>> dwSenderID
		>> strSenderName
		>> bCountry
		>> bAidCountry
		>> bType
		>> bGroup
		>> dwTarget
		>> strTalk;

	MAPPLAYER::iterator it;
	switch(bGroup)
	{
	case CHAT_SHOW:
	case CHAT_MAP:
	case CHAT_WORLD:
		for(it=m_mapPLAYER.begin(); it!=m_mapPLAYER.end(); it++)
		{
			CTPlayer * pTarget = (*it).second;
			if(pTarget->m_bMain &&
				pTarget->CanTalk(bGroup, bCountry, bAidCountry))
				(*it).second->SendCS_CHAT_ACK(
					bType,
					dwSenderID,
					strSenderName,
					strTalk);
		}
		break;
	case CHAT_TACTICS:
	case CHAT_FORCE:
	case CHAT_GUILD:
	case CHAT_PARTY:
	case CHAT_WHISPER:
		{
			CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
			if( pPlayer &&
				pPlayer->m_bMain &&
				pPlayer->CanTalk(bGroup, bCountry, bAidCountry))
			{
				pPlayer->SendCS_CHAT_ACK(
					dwCharID == dwSenderID ? bGroup : bType,
					dwSenderID,
					strSenderName,
					strTalk);
			}
		}
		break;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CABINETITEM_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_CABINETITEM_ACK)
		<< dwCharID
		<< dwKey;

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CABINETITEM_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bCount;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_TAKEMONMONEY_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwMoney;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwMoney;

	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKEY);
	if(!pPlayer || !pPlayer->m_bMain)
		return EC_NOERROR;

	pPlayer->EarnMoney(dwMoney);
	pPlayer->SendCS_MONEY_ACK();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_MONSTERDIE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwExp;
	DWORD dwMonID;
	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;
	WORD wMonTermID;
	BYTE bTermType;
	BYTE bTriggerType;
	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwExp
		>> dwMonID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> wMonTermID
		>> bTermType
		>> bTriggerType
		>> bCount;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(dwExp)
		pPlayer->GainExp( dwExp, TRUE);

	CheckQuest(
		pPlayer,
		dwMonID,
		fPosX,
		fPosY,
		fPosZ,
		wMonTermID,
		bTermType,
		bTriggerType,
		bCount);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_ADDITEM_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bServerID;
	BYTE bChannel;
	WORD wMapID;
	DWORD dwMonID;
	BYTE bInven;
	BYTE bSlot;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bServerID
		>> bChannel
		>> wMapID
		>> dwMonID
		>> bInven
		>> bSlot;

	CTItem * pItem = CreateItem(&(pBUF->m_packet));

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer || !pPlayer->m_bMain || !pItem)
	{
		SendMW_ADDITEMRESULT_ACK(
			dwCharID,
			dwKEY,
			bServerID,
			bChannel,
			wMapID,
			dwMonID,
			pItem ? pItem->m_bItemID : INVALID_SLOT,
			MIT_NOTFOUND);

		return EC_NOERROR;
	}

	SetItemAttr(pItem, pItem->m_bLevel);

	CTItem logItem;
	logItem.Copy(pItem, FALSE);

	if(pPlayer->PushTItem(pItem, bInven, bSlot))
	{
		SendMW_ADDITEMRESULT_ACK(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			bServerID,
			bChannel,
			wMapID,
			dwMonID,
			logItem.m_bItemID,
			MIT_SUCCESS);

		CheckQuest(
			pPlayer,
			dwMonID,
			pPlayer->m_fPosX,
			pPlayer->m_fPosY,
			pPlayer->m_fPosZ,
			logItem.m_pTITEM->m_wItemID,
			QTT_GETITEM,
			TT_GETITEM,
			logItem.m_bCount);

		if( pPlayer->GetPartyID() )
			PartyMonItemTake(pPlayer, &logItem);
	}
	else
	{
		SendMW_ADDITEMRESULT_ACK(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			bServerID,
			bChannel,
			wMapID,
			dwMonID,
			pItem->m_bItemID,
			MIT_FULLINVEN);

		delete pItem;
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_ADDITEMRESULT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bChannel;
	WORD wMapID;
	DWORD dwMonID;
	BYTE bItemID;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bChannel
		>> wMapID
		>> dwMonID
		>> bItemID
		>> bResult;

	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

	if(!pPlayer->m_pMAP)
		return EC_NOERROR;

	pPlayer->SendCS_MONITEMTAKE_ACK(bResult);

	CTMonster *pMON = pPlayer->m_pMAP->FindMonster(dwMonID);
	if(!pMON)
		return EC_NOERROR;

	CTItem *pItem = pMON->EraseItemList(bItemID);
	if(pItem)
	{
		if(pMON->m_bStatus == OS_DEAD)
		{
			if(!bResult)
			{
				pPlayer->SendCS_GETITEM_ACK(pItem);

				CTInven *pInven = pMON->FindTInven(INVEN_DEFAULT);
				if(pInven)
					pPlayer->SendCS_MONITEMLIST_ACK( MIL_SUCCESS, dwMonID, pMON->m_dwMoney, pInven, TRUE);

				delete pItem;
			}
			else
			{
				pMON->AddItem(pItem);
				if(pMON->m_dwInvenLock)
					ReviewMonItemList(pMON->m_dwInvenLock, pMON);
			}
		}
		else
			delete pItem;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_PARTYORDERTAKEITEM_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bServerID;
	BYTE bChannel;
	WORD wMapID;
	DWORD dwMonID;
	WORD wTempMonID;  	

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bServerID
		>> bChannel
		>> wMapID
		>> dwMonID
		>> wTempMonID;  

	CTItem * pItem = CreateItem(&(pBUF->m_packet));
	if(!pItem)
		return EC_NOERROR;
	

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer || 
		!pPlayer->m_bMain)
	{
		SendMW_ADDITEMRESULT_ACK(
			dwCharID,
			dwKEY,
			bServerID,
			bChannel,
			wMapID,
			dwMonID,
			pItem->m_bItemID,
			MIT_NOTFOUND);

		return EC_NOERROR;
	}

	SetItemAttr(pItem, pItem->m_bLevel);

	VTITEM vItem;
	vItem.clear();
	vItem.push_back(pItem);

	if(pPlayer->CanPush(&vItem, 0))
	{
		CTItem logItem;
		logItem.Copy(pItem, FALSE);

		BYTE bItemID = pItem->m_bItemID;
		WORD wItemID = pItem->m_wItemID;
		BYTE bCount = pItem->m_bCount;
		pPlayer->PushTItem(&vItem);
		PartyMonItemTake(pPlayer, &logItem);

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogItemByMonster(LOGMAP_ITEMBYMONPARTY, pPlayer, wTempMonID, &logItem);
#endif	//	DEF_UDPLOG
		
		SendMW_ADDITEMRESULT_ACK(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			bServerID,
			bChannel,
			wMapID,
			dwMonID,
			bItemID,
			MIT_SUCCESS);

		CheckQuest(
			pPlayer,
			dwMonID,
			pPlayer->m_fPosX,
			pPlayer->m_fPosY,
			pPlayer->m_fPosZ,
			wItemID,
			QTT_GETITEM,
			TT_GETITEM,
			bCount);
	}
	else
	{
		SendMW_ADDITEMRESULT_ACK(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			bServerID,
			bChannel,
			wMapID,
			dwMonID,
			pItem->m_bItemID,
			MIT_FULLINVEN);

		delete pItem;

	}

	vItem.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_FRIENDLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwSoulID;
	CString strSoulName;
	BYTE bSoulLevel;
	BYTE bSoulClass;
	BYTE bSoulConnected;
	DWORD dwSoulRegion;
	BYTE bCount;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwSoulID;

	if(dwSoulID)
		pBUF->m_packet
			>> strSoulName
			>> bSoulLevel
			>> bSoulClass
			>> bSoulConnected
			>> dwSoulRegion;

	pBUF->m_packet
		>> bCount;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
	{
		CPacket * pMsg = new CPacket();
		(*pMsg)
			<< dwSoulID;

		if(dwSoulID)
			(*pMsg)
				<< strSoulName
				<< bSoulLevel
				<< bSoulClass
				<< bSoulConnected
				<< dwSoulRegion;

		(*pMsg)
			<< bCount;

		BYTE bGroup;
		CString strGpName;
		for(BYTE i=0; i<bCount; i++)
		{
			pBUF->m_packet
				>> bGroup
				>> strGpName;

			(*pMsg)
				<< bGroup
				<< strGpName;
		}

		pBUF->m_packet
			>> bCount;

		(*pMsg)
			<< bCount;

		DWORD dwFriendID;
		CString strName;
		BYTE bLevel;
		BYTE bClass;
		BYTE bConnected;
		DWORD dwRegion;

		for(BYTE i=0; i<bCount; i++)
		{
			pBUF->m_packet
				>> dwFriendID
				>> strName
				>> bLevel
				>> bGroup
				>> bClass
				>> bConnected
				>> dwRegion;

			(*pMsg)
				<< dwFriendID
				<< strName
				<< bLevel
				<< bGroup
				<< bClass
				<< bConnected
				<< dwRegion;
		}
		pPlayer->SendCS_FRIENDLIST_ACK(pMsg);
	}
	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_FRIENDCONNECTION_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bRet;
	CString strName;
	DWORD dwRegion;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bRet
		>> strName
		>> dwRegion;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_FRIENDCONNECTION_ACK(bRet, strName, dwRegion);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_FRIENDREGION_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	CString strName;
	DWORD dwRegion;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strName
		>> dwRegion;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(pPlayer)
		pPlayer->SendCS_FRIENDREGION_ACK(strName, dwRegion);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_FRIENDADD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;
	DWORD dwFriendID;
	CString strName;
	BYTE bLevel;
	BYTE bGroup;
	BYTE bClass;
	DWORD dwRegion;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult
		>> dwFriendID
		>> strName
		>> bLevel
		>> bGroup
		>> bClass
		>> dwRegion;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_FRIENDADD_ACK(
			bResult,
			dwFriendID,
			strName,
			bLevel,
			bGroup,
			bClass,
			dwRegion);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_FRIENDASK_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	CString strInviter;
	DWORD dwInviter;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strInviter
		>> dwInviter;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
	{
		// 차단검사
		MAPTPROTECTED::iterator it;
		for(it=pPlayer->m_mapTPROTECTED.begin(); it!=pPlayer->m_mapTPROTECTED.end(); it++)
		{
			if((*it).second->m_strName == strInviter)
			{
				SendMW_FRIENDPROTECTEDASK_ACK(dwCharID, dwKey, strInviter);
				return EC_NOERROR;
			}
		}

		pPlayer->SendCS_FRIENDASK_ACK(strInviter);
	}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_FRIENDERASE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bRet;
	DWORD dwTarget;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bRet
		>> dwTarget;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_FRIENDERASE_ACK(bRet, dwTarget);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_FRIENDGROUPMAKE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bRet;
	BYTE bGroup;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bRet
		>> bGroup
		>> strName;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_FRIENDGROUPMAKE_ACK(bRet, bGroup, strName);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_FRIENDGROUPDELETE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bRet;
	BYTE bGroup;	

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bRet
		>> bGroup;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_FRIENDGROUPDELETE_ACK(bRet, bGroup);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_FRIENDGROUPCHANGE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bRet;
	BYTE bGroup;
	DWORD dwFriend;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bRet
		>> bGroup
		>> dwFriend;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_FRIENDGROUPCHANGE_ACK(bRet, bGroup, dwFriend);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_FRIENDGROUPNAME_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bRet;
	BYTE bGroup;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bRet
		>> bGroup
		>> strName;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_FRIENDGROUPNAME_ACK(bRet, bGroup, strName);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_CHGPARTYCHIEF_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bRET;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bRET;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_CHGPARTYCHIEF_ACK(bRET);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_CHGSQUADCHIEF_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	WORD wSquadID;
	DWORD dwChiefID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wSquadID
		>> dwChiefID;

	CTPlayer * pPlayer = FindPlayer( dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_CHGSQUADCHIEF_ACK( wSquadID, dwChiefID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_PARTYATTR_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	DWORD dwPartyChiefID;
	BYTE bPartyType;

	WORD wCommander;
	WORD wPartyID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wPartyID
		>> bPartyType
		>> dwPartyChiefID
		>> wCommander;

	WORD wPrevPartyID = 0;
	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKey);
	if(pPlayer)
	{
		if(!wPartyID)
			ArenaLeave(pPlayer);

		wPrevPartyID = pPlayer->m_wPartyID;
		pPlayer->m_dwPartyChiefID = dwPartyChiefID;
		pPlayer->m_wPartyID = wPartyID;
		pPlayer->m_bPartyType = bPartyType;
		pPlayer->m_wCommanderID = wCommander;
	}
	else
		return EC_NOERROR;

	if(pPlayer->m_bMain)
	{
		pPlayer->SendCS_PARTYATTR_ACK(pPlayer->m_dwID, pPlayer->GetPartyID(), pPlayer->GetPartyChiefID(), pPlayer->GetCommanderID());

		if(pPlayer->m_pMAP)
		{
			VPLAYER vPlayer;
			vPlayer.clear();

			pPlayer->m_pMAP->GetNeighbor(
				&vPlayer,
				pPlayer->m_fPosX,
				pPlayer->m_fPosZ);

			for( DWORD i=0; i<DWORD(vPlayer.size()); i++)
				if(vPlayer[i]->m_dwID != pPlayer->m_dwID)
					vPlayer[i]->SendCS_PARTYATTR_ACK( pPlayer->m_dwID, pPlayer->GetPartyID(), pPlayer->GetPartyChiefID(), pPlayer->GetCommanderID());

			vPlayer.clear();

			if(wPrevPartyID && !wPartyID)
			{
				if(pPlayer->m_pMAP->m_bType == MAP_INDUN)
				{
					WORD wSpawn = GetIndunOutPos(pPlayer->m_wMapID, pPlayer->GetWarCountry());
					if(wSpawn)
						Teleport(pPlayer, wSpawn);
				}
			}
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTDEL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwPostID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwPostID;

	LPPACKETBUF pMSG = new PACKETBUF();
	DEFINE_QUERY(&m_db, CSPPostDelete)
	query->m_dwCharID = dwCharID;
	query->m_dwPostID = dwPostID;
	if(!query->Call() || query->m_nRet)
		dwPostID = 0;

	UNDEFINE_QUERY()

	pMSG->m_packet.SetID(DM_POSTDEL_ACK)
		<< dwCharID
		<< dwKey
		<< dwPostID;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTDEL_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwPostID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwPostID;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_POSTDEL_ACK(dwPostID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	WORD wPage;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wPage;

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_POSTLIST_ACK)
		<< dwCharID
		<< dwKey;

	DWORD dwBegin = 0;

	DEFINE_QUERY(&m_db, CSPGetPostInfo)
	query->m_dwCharID = dwCharID;
	query->m_wPage = wPage;
	query->m_wTotalCount = 0;
	query->m_wNotReadCount = 0;
	if(query->Call())
		dwBegin = query->m_dwBeginID;

	pMsg->m_packet
		<< query->m_wTotalCount
		<< query->m_wNotReadCount
		<< wPage;

	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLPostTable)
	query->m_dwCharID = dwCharID;
	query->m_dwBeginID = dwBegin;

	WORD wSize = pMsg->m_packet.GetSize();
	WORD wCount = 0;

	pMsg->m_packet
		<< wCount;

	if(query->Open())
	{
		while(query->Fetch())
		{
			pMsg->m_packet
				<< query->m_dwPostID
				<< query->m_bRead
				<< query->m_bType
				<< query->m_szSender
				<< query->m_szTitle;

			pMsg->m_packet
				<< __DBTOTIME(query->m_timeRecv);

			wCount++;
		}
		memcpy(pMsg->m_packet.GetBuffer()+wSize, &wCount, sizeof(wCount));
		query->Close();
	}
	UNDEFINE_QUERY();

	SayToBATCH(pMsg);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnDM_POSTLIST_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	WORD wPage;
	WORD wCount;

	pBUF->m_packet
		>> pPlayer->m_wPostTotal
		>> pPlayer->m_wPostRead
		>> wPage
		>> wCount;

	CPacket * pMSG = new CPacket();
	(*pMSG)
		<< pPlayer->m_wPostTotal
		<< pPlayer->m_wPostRead
		<< wPage
		<< wCount;

	DWORD dwPostID;
	BYTE bRead;
	__int64 timeRecv;
	CString strSender;
	BYTE bType;
	CString strTitle;

	for(WORD i=0; i<wCount; i++)
	{
		pBUF->m_packet
			>> dwPostID
			>> bRead
			>> bType
			>> strSender
			>> strTitle
			>> timeRecv;

		(*pMSG)
			<< dwPostID
			<< bRead
			<< bType
			<< strSender
			<< strTitle
			<< timeRecv;
	}

	pPlayer->SendCS_POSTLIST_ACK(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTGETITEM_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwPostID;

	pBUF->m_packet
		>> dwCharID
		>> dwPostID;

	DEFINE_QUERY(&m_db, CSPPostGetItem)
	query->m_dwCharID = dwCharID;
	query->m_dwPostID = dwPostID;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_POSTRECV_REQ(LPPACKETBUF pBUF)
{
	DWORD dwPostID;
	BYTE bType;
	CString strTarget;
	CString strTitle;
	CString strSender;

	pBUF->m_packet
		>> dwPostID
		>> strSender
		>> strTarget
		>> strTitle
		>> bType;

	CTPlayer * pRecver = FindPlayer(strTarget);

	if(pRecver && pRecver->m_bMain)
	{
		pRecver->SendCS_POSTRECV_ACK(
			dwPostID,
			FALSE,
			strSender,
			strTitle,
			bType,
			m_timeCurrent);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTCANSEND_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwSendID;
	CString strSender;
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
		>> dwSendID
		>> strSender
		>> strTarget
		>> strTitle
		>> strMessage
		>> bType
		>> dwGold
		>> dwSilver
		>> dwCooper
		>> bInven
		>> bSlot;

	DWORD dwCharID = 0;
	DEFINE_QUERY(&m_db, CSPPostCanSend)
	lstrcpy(query->m_szName, strSender);
	lstrcpy(query->m_szTarget, strTarget);
	query->m_bType = bType;
	if(query->Call())
	{
		bRet = query->m_nRet;
		dwCharID = query->m_dwCharID;
	}
	else
		bRet = POST_NORECEIVER;

	UNDEFINE_QUERY()

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_POSTCANSEND_ACK)
		<< bRet
		<< dwSendID
		<< dwCharID
		<< strSender
		<< strTarget
		<< strTitle
		<< strMessage
		<< bType
		<< dwGold
		<< dwSilver
		<< dwCooper
		<< bInven
		<< bSlot;

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTCANSEND_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwSendID;
	DWORD dwCharID;
	CString strTarget;
	CString strTitle;
	CString strMessage;
	CString strSender;
	BYTE bType;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;
	BYTE bInven;
	BYTE bSlot;

	pBUF->m_packet
		>> bRet
		>> dwSendID
		>> dwCharID
		>> strSender
		>> strTarget
		>> strTitle
		>> strMessage
		>> bType
		>> dwGold
		>> dwSilver
		>> dwCooper
		>> bInven
		>> bSlot;

	CTPlayer * pSender = FindPlayer(strSender);
	if(!pSender || !pSender->m_bMain)
		return EC_NOERROR;

	if(bRet)
	{
		pSender->SendCS_POSTSEND_ACK(bRet);
		return EC_NOERROR;
	}

	__int64 llMoney = GetPostMoney(bType);

	if(bType != POST_BILLS)
		llMoney += CalcMoney(dwGold, dwSilver, dwCooper);

	if(!pSender->UseMoney(llMoney, FALSE))
	{
		pSender->SendCS_POSTSEND_ACK(POST_NEEDMONEY);
		return EC_NOERROR;
	}

	CTItem * pItem = NULL;
	CTInven * pInven = NULL;

	if(bInven != INVEN_NULL)
	{
		pInven = pSender->FindTInven(bInven);

		if(pInven)
		{
			pItem = pInven->FindTItem(bSlot);
			if(!pItem)
			{
				pSender->SendCS_POSTSEND_ACK(POST_NEEDITEM);
				return EC_NOERROR;
			}
			pItem->m_bInven = pInven->m_bInvenID;
		}
		else
		{
			pSender->SendCS_POSTSEND_ACK(POST_NEEDITEM);
			return EC_NOERROR;
		}
		if(!pItem->CanDeal() ||
			!(pItem->m_pTITEM->m_bIsSell & ITEMTRADE_CABINET))
		{
			pSender->SendCS_POSTSEND_ACK(POST_NOTDEAL);
			return EC_NOERROR;
		}
		if(!pSender->CheckDealItem(bInven, bSlot))
		{
			pSender->SendCS_POSTSEND_ACK(POST_NOTDEAL);
			return EC_NOERROR;
		}
	}

	if(bType == POST_BILLS && !pItem)
	{
		pSender->SendCS_POSTSEND_ACK(POST_NEEDITEM);
		return EC_NOERROR;
	}

	SendDM_POSTRECV_REQ(
		dwSendID,
		dwCharID,
		pSender->m_strNAME,
		strTarget,
		strTitle,
		strMessage,
		bType,
		dwGold,
		dwSilver,
		dwCooper,
		pItem);

	if(pItem)
	{
#ifdef	DEF_UDPLOG
		m_pUdpSocket->LogItemTrade(LOGMAP_ITEMPOSTSEND, pSender, pItem, strTarget);
#endif	//	DEF_UDPLOG	

		pInven->m_mapTITEM.erase(bSlot);
		pSender->SendCS_DELITEM_ACK(
			bInven,
			bSlot);

		delete pItem;
	}

	if(llMoney)
	{
#ifdef DEF_UDPLOG
		m_pUdpSocket->LogMoneyTrade(LOGMAP_MONEYPOSTSEND, pSender, strTarget, -llMoney, -(int)GetPostMoney(bType) );
#endif

		pSender->UseMoney(llMoney, TRUE);
		pSender->SendCS_MONEY_ACK();
	}

	pSender->SendCS_POSTSEND_ACK(POST_SUCCESS);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTRECV_REQ(LPPACKETBUF pBUF)
{
	DWORD dwSendID;
	DWORD dwRecvID;
	CString strTarget;
	CString strTitle;
	CString strMessage;
	CString strSender;
	BYTE bType;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;
	BYTE bItemCnt;
	BYTE bInven;

	pBUF->m_packet
		>> dwSendID
		>> dwRecvID
		>> strSender
		>> strTarget
		>> strTitle
		>> strMessage
		>> bType
		>> dwGold
		>> dwSilver
		>> dwCooper
		>> bItemCnt;

	DWORD dwMakeID = 0;
	BYTE bRet = POST_SUCCESS;
	DEFINE_QUERY(&m_db, CSPSavePost)
	query->m_dwCharID = dwRecvID;
	query->m_dwSendID = dwSendID;
	lstrcpy(query->m_szTarget, strTarget);
	lstrcpy(query->m_szSender, strSender);
	lstrcpy(query->m_szTitle, strTitle);
	lstrcpy(query->m_szMessage, strMessage);
	query->m_bType = bType;
	query->m_bRead = FALSE;
	query->m_dwGold = dwGold;
	query->m_dwSilver = dwSilver;
	query->m_dwCooper = dwCooper;
	__TIMETODB(m_timeCurrent, query->m_timeRecv);

	if(query->Call())
	{
		dwMakeID = query->m_dwMakeID;
		dwRecvID = query->m_dwRecvID;
		bRet = query->m_nRet;
	}
	else
		bRet = POST_INTERNAL;

	UNDEFINE_QUERY()

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_POSTRECV_ACK)
		<< bRet
		<< dwMakeID
		<< dwSendID
		<< dwRecvID
		<< strSender
		<< strTarget
		<< strTitle
		<< strMessage
		<< bType
		<< dwGold
		<< dwSilver
		<< dwCooper
		<< bItemCnt;

	for(BYTE i=0; i<bItemCnt; i++)
	{
		DEFINE_QUERY(&m_db, CSPSaveItemDirect)
		query->m_bStorageType = STORAGE_POST;
		query->m_dwStorageID = dwMakeID;
		query->m_bOwnerType = TOWNER_CHAR;
		query->m_dwOwnerID = dwRecvID;

		pBUF->m_packet
			>> bInven;

		pMsg->m_packet
			<< bInven;

		WrapItemQuery(&(pBUF->m_packet), query->m_item);

		WrapItem(
			&(pMsg->m_packet),
			query->m_item);

		if(dwMakeID)
			query->Call();

		UNDEFINE_QUERY();
	}

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTRECV_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwPostID;
	DWORD dwSendID;
	DWORD dwRecvID;
	CString strTarget;
	CString strTitle;
	CString strMessage;
	CString strSender;
	BYTE bType;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;
	BYTE bItemCnt;
	BYTE bInven;

	pBUF->m_packet
		>> bRet
		>> dwPostID
		>> dwSendID
		>> dwRecvID
		>> strSender
		>> strTarget
		>> strTitle
		>> strMessage
		>> bType
		>> dwGold
		>> dwSilver
		>> dwCooper
		>> bItemCnt;

	VTITEM vItem;
	vItem.clear();

	for(BYTE i=0; i<bItemCnt; i++)
	{
		pBUF->m_packet
			>> bInven;

		CTItem *pTITEM = CreateItem(&(pBUF->m_packet));

		if(pTITEM)
			vItem.push_back(pTITEM);
	}

	CTPlayer * pSender = FindPlayer(strSender);
	if(bRet)
	{
		if(pSender && pSender->m_bMain)
		{
			for(BYTE i=0; i<bItemCnt; i++)
				SetItemAttr(vItem[i], vItem[i]->m_bLevel);

			if(pSender->CanPush(&vItem, 0))
			{
				pSender->PushTItem(&vItem);
				pSender->SendCS_MOVEITEM_ACK(MI_SUCCESS);
			}
			else
			{
				while(!vItem.empty())
				{
					delete vItem.back();
					vItem.pop_back();
				}
			}

			pSender->SendCS_POSTSEND_ACK(bRet);
		}

		vItem.clear();

		return EC_NOERROR;
	}

	if(pSender && pSender->m_bMain)
	{
		if(!vItem.empty())
			SendDM_SAVEITEM_REQ(pSender);

		if(bType == POST_BILLS)
			SendSM_POSTBILL_REQ(pSender->m_dwID, dwPostID, m_timeCurrent + POST_DURATION);
	}

	CTPlayer * pRecver = FindPlayer(strTarget);
	if(pRecver && pRecver->m_bMain)
	{
		pRecver->SendCS_POSTRECV_ACK(
			dwPostID,
			FALSE,
			strSender,
			strTitle,
			bType,
			m_timeCurrent);
	}
	else
	{
		SendMW_POSTRECV_ACK(
			dwPostID,
			strSender,
			strTarget,
			strTitle,
			bType);
	}

	while(!vItem.empty())
	{
		delete vItem.back();
		vItem.pop_back();
	}

	vItem.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CHARSTATINFO_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;

	pBUF->m_packet
		>> dwCharID;

	MAPPLAYER::iterator it = m_mapPLAYER.find(dwCharID);
	if(it!=m_mapPLAYER.end())
		(*it).second->SendCS_CHARSTATINFO_ACK(&(pBUF->m_packet));

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_CHARSTATINFOANS_REQ(LPPACKETBUF pBUF)
{
	DWORD dwReqCharID;
	DWORD dwCharID;
	pBUF->m_packet
		>> dwReqCharID
		>> dwCharID;

	MAPPLAYER::iterator it = m_mapPLAYER.find(dwCharID);
	if(it!=m_mapPLAYER.end() &&
		(*it).second->m_bMain )
		SendMW_CHARSTATINFOANS_ACK(dwReqCharID, (*it).second);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_LOCALENABLE_REQ(LPPACKETBUF pBUF)
{
	static BATTLE_STATUS bCurStatus = BS_NORMAL;

	BYTE bStatus;
	DWORD dwSecond;
	DWORD dwLocalStart;
	BYTE bCastleDay;
	DWORD dwCastleStart;

	pBUF->m_packet
		>> bStatus
		>> dwSecond
		>> dwLocalStart
		>> bCastleDay
		>> dwCastleStart;

	for(DWORD ll=0; ll<m_vOccupation.size(); ll++)
	{
		LPTLOCAL pLocal = m_vOccupation[ll];

		if(dwLocalStart)
		{
			CTime curtime = CTime::GetCurrentTime().GetTime();
			INT nCH = curtime.GetHour();
			INT nCM = curtime.GetMinute();
			INT nCS = curtime.GetSecond();
			INT nCD = curtime.GetDayOfWeek();
			DWORD dwCLT = nCH*60*60+nCM*60+nCS;
			if(dwCLT > dwLocalStart)
				pLocal->m_timeNextDefend = curtime.GetTime() + (DAY_ONE - dwCLT) + dwLocalStart;
			else
				pLocal->m_timeNextDefend = curtime.GetTime() + (dwLocalStart - dwCLT);
		}

		if(bCastleDay)
		{
			CTime curtime = CTime::GetCurrentTime().GetTime();
			INT nCH = curtime.GetHour();
			INT nCM = curtime.GetMinute();
			INT nCS = curtime.GetSecond();
			INT nCD = curtime.GetDayOfWeek();
			DWORD dwCLT = nCH*60*60+nCM*60+nCS;

			LPTLOCAL pCastle = FindCastle(pLocal->m_pZone->m_wCastle);
			if(pCastle)
			{
				pCastle->m_bDay = bCastleDay;
				DWORD dwStart = (pCastle->m_bDay-1) * DAY_ONE + dwCastleStart;
				DWORD dwCSW = (nCD-1) * DAY_ONE + dwCLT;
				if(dwCSW > dwStart)
					pCastle->m_timeNextDefend = curtime.GetTime() + (WEEK_ONE - dwCSW) + dwStart;
				else
					pCastle->m_timeNextDefend = curtime.GetTime() + (dwStart - dwCSW);

				CTime local(pLocal->m_timeNextDefend);
				CTime castle(pCastle->m_timeNextDefend);

				if(local.GetDay() == castle.GetDay())
					pLocal->m_timeNextDefend += DAY_ONE;
			}
		}

		if(!pLocal->m_pZone ||
			!pLocal->m_bValid ||
			pLocal->m_bStatus == bStatus)
			continue;

		pLocal->m_bStatus = bStatus;

		switch(bStatus)
		{
		case BS_NORMAL:
			{
				pLocal->m_bCanBattle = TRUE;

				CTMap *pMap = FindTMap( DEFAULT_CHANNEL, 0, pLocal->m_pZone->m_wMapID);
				if(pMap)
				{
					DelMonSpawn(pMap, pLocal->m_pZone->m_wBossSpawnID);
					DelMonSpawn(pMap, pLocal->m_pZone->m_wLGateKeeperSpawnID);
					DelMonSpawn(pMap, pLocal->m_pZone->m_wRGateKeeperSpawnID);
				}
			}
			break;
		case BS_BATTLE:
			{
				pLocal->m_bCanBattle = TRUE;
				pLocal->m_mapTRecord.clear();

				CTMap *pMap = FindTMap( DEFAULT_CHANNEL, 0, pLocal->m_pZone->m_wMapID);

				if(pMap)
				{
					pLocal->m_bGateOpened = FALSE;
					ChangeSwitch( pMap, pLocal->m_pZone->m_dwLSwitchID, SWC_CLOSE);
					ChangeSwitch( pMap, pLocal->m_pZone->m_dwRSwitchID, SWC_CLOSE);

					AddMonSpawn(pMap, pLocal->m_pZone->m_wLGateKeeperSpawnID, MONSPAWN_SUSPEND);
					AddMonSpawn(pMap, pLocal->m_pZone->m_wRGateKeeperSpawnID, MONSPAWN_SUSPEND);

					for( DWORD i=0; i<pLocal->m_pZone->m_vSpawnID.size(); i++)
					{
						MAPTMONSPAWN::iterator it = pMap->m_mapTMONSPAWN.find(pLocal->m_pZone->m_vSpawnID[i]);

						if(it!=pMap->m_mapTMONSPAWN.end())
						{
							CTMonSpawn *pSpawn = (*it).second;
							pSpawn->m_bStatus = MONSPAWN_SUSPEND;
						}
					}
				}
			}
			break;
		case BS_PEACE:
			{
				if(!pLocal->m_bOccupied)
					SendDM_LOCALOCCUPY_REQ(
						pLocal->m_wLocal,
						OCCUPY_DEFEND, 0, 0);

				pLocal->m_bOccupied = FALSE;
				pLocal->m_bCanBattle = FALSE;

				CTMap *pMap = FindTMap(DEFAULT_CHANNEL, 0, pLocal->m_pZone->m_wMapID);

				if(pMap)
				{
					pLocal->m_bGateOpened = TRUE;
					ChangeSwitch( pMap, pLocal->m_pZone->m_dwLSwitchID, SWC_OPEN);
					ChangeSwitch( pMap, pLocal->m_pZone->m_dwRSwitchID, SWC_OPEN);

					for(DWORD i=0; i<pLocal->m_pZone->m_vSpawnID.size(); i++)
					{
						MAPTMONSPAWN::iterator it = pMap->m_mapTMONSPAWN.find(pLocal->m_pZone->m_vSpawnID[i]);

						if(it!=pMap->m_mapTMONSPAWN.end())
						{
							(*it).second->m_bStatus = MONSPAWN_READY;
							for(DWORD j=0; j<(*it).second->m_vTMON.size(); j++)
							{
								(*it).second->m_vTMON[j]->OnEvent( AT_DEAD, 0, 0, 0, 0);
							}
						}
					}
				}
			}
			break;
		}
	}

	BYTE bType = SM_NONE;
	switch(bStatus)
	{
	case BS_NORMAL:
		if(bCurStatus == BS_NORMAL)
			bType = SM_BATTLE_START_ALARM;
		else if (bCurStatus == BS_PEACE)
			bType = SM_BATTLE_NORMAL;
		break;
	case BS_BATTLE:
		if(bCurStatus == BS_BATTLE)
			bType = SM_BATTLE_END_ALARM;
		else if(bCurStatus == BS_NORMAL)
			bType = SM_BATTLE_START;
		break;
	case BS_PEACE:
		if(bCurStatus == BS_BATTLE)
		{
			bType = SM_BATTLE_PEACE;
			LocalReward();
		}
		break;
	default:
		break;
	}

	NotifyLocalInfo(bType, 0, dwSecond);

	bCurStatus = (BATTLE_STATUS)bStatus;

	if(bCastleDay)
	{
		BYTE bSent = FALSE;
		for(DWORD ll=0; ll<m_vCastle.size(); ll++)
		{
			LPTLOCAL pCastle = m_vCastle[ll];
			if(pCastle->m_bValid)
			{
				SendMW_CASTLEWARINFO_ACK(pCastle);
				bSent = TRUE;
			}
		}

		if(!bSent)
			SendMW_CASTLEWARINFO_ACK(NULL);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_LOCALOCCUPY_REQ(LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	WORD wLocalID;

	BYTE bCountry;
	BYTE bType;
	CString strGuildName;

	pBUF->m_packet
		>> bType
		>> wLocalID
		>> bCountry
		>> dwGuildID
		>> strGuildName;

	LPTLOCAL pLocal = FindOccupation(wLocalID);
	if(pLocal)
	{
		if(bType == OCCUPY_ACCEPT)
			NotifyLocalInfo(
				SM_BATTLE_BOSSDIE,
				bCountry,
				0,
				pLocal->m_pZone->m_strName,
				strGuildName);

		ResetLocal( pLocal, bCountry, dwGuildID, strGuildName, bType);
	}

	MAPPLAYER::iterator it;
	for( it = m_mapPLAYER.begin(); it != m_mapPLAYER.end(); it++)
		(*it).second->SendCS_LOCALOCCUPY_ACK( bType, wLocalID, bCountry, dwGuildID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_MISSIONOCCUPY_REQ(LPPACKETBUF pBUF)
{
	WORD wLocalID;
	BYTE bCountry;
	BYTE bType;

	pBUF->m_packet
		>> bType
		>> wLocalID
		>> bCountry;

	LPTLOCAL pLocal = FindMission(wLocalID);
	if(pLocal)
	{
		NotifyLocalInfo(
			bType == OCCUPY_ACCEPT ? SM_MISSION_BOSSDIE : SM_MISSION_TIMEOUT,
			bCountry,
			pLocal->m_pZone->m_wMapID,
			pLocal->m_pZone->m_strName);

		ResetMission( pLocal, bCountry, bType);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CASTLEOCCUPY_REQ(LPPACKETBUF pBUF)
{
	BYTE bType;
	BYTE bCountry;
	WORD wCastleID;
	DWORD dwGuildID;
	CString strGuildName;
	DWORD dwPrevGuildID = 0;

	pBUF->m_packet
		>> bType
		>> wCastleID
		>> dwGuildID
		>> bCountry
		>> strGuildName;

	LPTLOCAL pCastle = FindCastle(wCastleID);
	if(!pCastle)
		return EC_NOERROR;

	if(pCastle->m_dwGuild != dwGuildID)
	{
		pCastle->m_strHero = NAME_NULL;
		pCastle->m_timeHero = 0;
	}

	dwPrevGuildID = pCastle->m_dwGuild;
	pCastle->m_dwGuild = dwGuildID;
	pCastle->m_bCountry = bCountry;
	pCastle->m_strGuild = strGuildName;
	pCastle->m_bLastOccType = bType;
	pCastle->m_wDPoint = 0;
	pCastle->m_dwDefGuildID = 0;
	pCastle->m_dwAtkGuildID = 0;
	pCastle->m_wCPoint = 0;
	pCastle->m_bDefCount = 0;
	pCastle->m_bAtkCount = 0;
	pCastle->m_bDefCountry = TCONTRY_N;
	pCastle->m_strAtkName = NAME_NULL;
	pCastle->m_strDefName = NAME_NULL;
	pCastle->m_timeNextDefend += WEEK_ONE;
	pCastle->m_mapTPoint.clear();
	pCastle->m_vTop3[TCONTRY_D].clear();
	pCastle->m_vTop3[TCONTRY_C].clear();

	MAPTLOCAL::iterator itLo;
	for(itLo = pCastle->m_mapLocal.begin(); itLo != pCastle->m_mapLocal.end(); itLo++)
	{
		memset((*itLo).second->m_occupyGuild, 0, sizeof((*itLo).second->m_occupyGuild));
		memset((*itLo).second->m_occupyType, 0, sizeof((*itLo).second->m_occupyType));
	}

	MAPPLAYER::iterator it;
	for(it=m_mapPLAYER.begin(); it!=m_mapPLAYER.end(); it++)
	{
		CTPlayer * pChar = (*it).second;
		if(pChar->m_bMain)
		{
			if(bType == OCCUPY_ACCEPT && CheckMapGuild(pChar))
				Teleport( pChar, pChar->m_wLastSpawnID);
		}
	}

	NotifyLocalInfo(
		SM_CASTLE_END,
		0,
		0,
		pCastle->m_pZone->m_strName,
		strGuildName);

	if(!pCastle->m_bValid)
		return EC_NOERROR;

	PvPEvent(PVPE_ENTRY, pCastle, 0);

	if(dwPrevGuildID != dwGuildID)
		PvPEvent(PVPE_WIN, pCastle, dwPrevGuildID);
	else
		PvPEvent(PVPE_DEFEND, pCastle, dwPrevGuildID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CHGCORPSCOMMANDER_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bRET;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bRET;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_CHGCORPSCOMMANDER_ACK(bRET);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CORPSASK_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwAskID;
	CString strAskName;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwAskID
		>> strAskName;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	if(!pPlayer->CheckProtected(dwAskID, PROTECTED_INVITE))
		return EC_NOERROR;

	pPlayer->SendCS_CORPSASK_ACK(strAskName);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CORPSREPLY_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;
	CString strAskName;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult
		>> strAskName;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_CORPSREPLY_ACK(bResult, strAskName);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CORPSJOIN_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	WORD wCorpsID;
	WORD wCommander;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wCorpsID
		>> wCommander;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->m_wCommanderID = wCommander;
	pPlayer->SendCS_CORPSJOIN_ACK(
		wCorpsID,
		pPlayer->GetCommanderID());

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_ADDSQUAD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_ADDSQUAD_ACK(&pBUF->m_packet);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_DELSQUAD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	WORD wSquad;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wSquad;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_DELSQUAD_ACK(wSquad);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_DELCORPSUNIT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	WORD wSquad;
	DWORD dwTarget;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wSquad
		>> dwTarget;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_DELCORPSUNIT_ACK(wSquad, dwTarget);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_ADDCORPSUNIT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_ADDCORPSUNIT_ACK(&pBUF->m_packet);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CORPSCMD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwMemberID;
	DWORD dwKey;
	WORD wSquadID;
	DWORD dwCharID;
	WORD wMapID;
	BYTE bCMD;
	DWORD dwTargetID;
	BYTE bTargetType;
	WORD wPosX;
	WORD wPosZ;

	pBUF->m_packet
		>> dwMemberID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwMemberID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pBUF->m_packet
		>> wSquadID
		>> dwCharID
		>> wMapID
		>> bCMD
		>> dwTargetID
		>> bTargetType
		>> wPosX
		>> wPosZ;

	pPlayer->SendCS_CORPSCMD_ACK(
		wSquadID,
		dwCharID,
		wMapID,
		bCMD,
		dwTargetID,
		bTargetType,
		wPosX,
		wPosZ);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CORPSENEMYLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwKey;
	DWORD dwCharID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_CORPSENEMYLIST_ACK(&pBUF->m_packet);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_MOVECORPSENEMY_REQ(LPPACKETBUF pBUF)
{
	DWORD dwKey;
	DWORD dwCharID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_MOVECORPSENEMY_ACK(&pBUF->m_packet);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_MOVECORPSUNIT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwKey;
	DWORD dwCharID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_MOVECORPSUNIT_ACK(&pBUF->m_packet);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_ADDCORPSENEMY_REQ(LPPACKETBUF pBUF)
{
	DWORD dwKey;
	DWORD dwCharID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_ADDCORPSENEMY_ACK(&pBUF->m_packet);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_DELCORPSENEMY_REQ(LPPACKETBUF pBUF)
{
	DWORD dwKey;
	DWORD dwCharID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_DELCORPSENEMY_ACK(&pBUF->m_packet);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CORPSHP_REQ(LPPACKETBUF pBUF)
{
	DWORD dwKey;
	DWORD dwCharID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_CORPSHP_ACK(&pBUF->m_packet);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_REPORTENEMYLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	WORD wSquadID;
	DWORD dwChar;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wSquadID
		>> dwChar;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_REPORTENEMYLIST_ACK(wSquadID, dwChar);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_PARTYMOVE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwKey;
	DWORD dwCharID;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_PARTYMOVE_ACK(bResult);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_TMSRECV_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTMS;
	CString strSender;
	CString strMessage;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTMS
		>> strSender
		>> strMessage;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_TMSRECV_ACK(dwTMS, strSender, strMessage);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_TMSINVITEASK_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwTargetID;
	DWORD dwTargetKEY;
	DWORD dwTMS;
	CString strMessage;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwTargetID
		>> dwTargetKEY
		>> dwTMS
		>> strMessage;

	BYTE bResult = FALSE;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(pPlayer)
	{
		if(pPlayer->CheckProtected(dwTargetID, PROTECTED_INVITE))
			bResult = TRUE;
	}

	SendMW_TMSINVITEASK_ACK(dwTargetID, dwTargetKEY, dwCharID, dwKEY, bResult, dwTMS, strMessage);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_TMSINVITE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_TMSINVITE_ACK(&pBUF->m_packet);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_TMSOUT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwTMS;
	CString strMember;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwTMS
		>> strMember;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_TMSOUT_ACK(dwTMS, strMember);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_CASTLEENABLE_REQ(LPPACKETBUF pBUF)
{
	static BATTLE_STATUS bCurStatus = BS_NORMAL;

	BYTE bStatus;
	DWORD dwSecond;
	pBUF->m_packet
		>> bStatus
		>> dwSecond;

	for(DWORD ll=0; ll<m_vCastle.size(); ll++)
	{
		LPTLOCAL pCastle = m_vCastle[ll];

		if(pCastle->m_bStatus == BS_PEACE && bStatus != BS_NORMAL)
			continue;

		BYTE bPrevStatus = pCastle->m_bStatus;
		pCastle->m_bStatus = bStatus;

		if(!pCastle->m_pZone ||
			!pCastle->m_bValid ||
			bPrevStatus == bStatus)
			continue;

		switch(bStatus)
		{
		case BS_NORMAL:
			{
				CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, pCastle->m_pZone->m_wMapID);
				if(pMap)
				{
					ChangeSwitch( pMap, pCastle->m_pZone->m_dwLSwitchID, SWC_CLOSE);
					ChangeSwitch( pMap, pCastle->m_pZone->m_dwRSwitchID, SWC_CLOSE);
					ChangeSwitch( pMap, pCastle->m_pZone->m_dwCSwitchID, SWC_CLOSE);
					LeaveCastleMap(pMap);
					pMap->ClearMonster();
				}
			}
			break;
		case BS_BATTLE:
			{
				pCastle->m_mapTRecord.clear();
				pCastle->m_bCanBattle = TRUE;

				CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, pCastle->m_pZone->m_wMapID);
				if(pMap)
				{
					ChangeSwitch( pMap, pCastle->m_pZone->m_dwLSwitchID, SWC_CLOSE);
					ChangeSwitch( pMap, pCastle->m_pZone->m_dwRSwitchID, SWC_CLOSE);
					ChangeSwitch( pMap, pCastle->m_pZone->m_dwCSwitchID, SWC_CLOSE);

					AddMonSpawn(pMap, pCastle->m_pZone->m_wLGateKeeperSpawnID, MONSPAWN_SUSPEND);
					AddMonSpawn(pMap, pCastle->m_pZone->m_wRGateKeeperSpawnID, MONSPAWN_SUSPEND);
					AddMonSpawn(pMap, pCastle->m_pZone->m_wCGateKeeperSpawnID, MONSPAWN_SUSPEND);

					for( DWORD i=0; i<pCastle->m_pZone->m_vSpawnID.size(); i++)
					{
						MAPTMONSPAWN::iterator it = pMap->m_mapTMONSPAWN.find(pCastle->m_pZone->m_vSpawnID[i]);

						if(it!=pMap->m_mapTMONSPAWN.end())
						{
							CTMonSpawn *pSpawn = (*it).second;
							pSpawn->m_bStatus = MONSPAWN_SUSPEND;
						}
					}

					pMap->StartWar(
						pCastle->m_wLocal,
						pCastle->m_bDefCountry,
						pCastle->m_dwDefGuildID,
						pCastle->m_wDPoint,
						pCastle->m_dwAtkGuildID,
						pCastle->m_wCPoint,
						m_dwTick + dwSecond * 1000);

					if(!pCastle->m_dwDefGuildID || !pCastle->m_dwAtkGuildID)
						EndWar(pCastle, pMap, WIN_NOWAR);
					else
						SendSM_GODBALLCMD_REQ(0, GB_STARTWAR, pMap->m_bChannel, pMap->m_wMapID, pCastle->m_wLocal, m_dwGBDelay[GB_STARTWAR], 0);
				}
			}
			break;
		case BS_PEACE:
			{
				CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, pCastle->m_pZone->m_wMapID);

				if(pMap)
					EndWar(pCastle, pMap, WIN_TIME);
			}
			break;
		}
	}

	BYTE bType = SM_NONE;
	switch(bStatus)
	{
	case BS_NORMAL:
		if(bCurStatus == BS_NORMAL)
			bType = SM_CASTLE_START_ALARM;
		else if (bCurStatus == BS_PEACE)
			bType = SM_CASTLE_NORMAL;
		break;
	case BS_BATTLE:
		if(bCurStatus == BS_BATTLE)
			bType = SM_CASTLE_END_ALARM;
		else if(bCurStatus == BS_NORMAL)
			bType = SM_CASTLE_START;
		break;
	case BS_PEACE:
		if(bCurStatus == BS_BATTLE)
		{
			bType = SM_CASTLE_PEACE;
		}
		break;
	default:
		break;
	}

	NotifyLocalInfo(bType, 0, dwSecond);

	bCurStatus = (BATTLE_STATUS)bStatus;

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CASTLEAPPLY_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	WORD wCastle;
	DWORD dwTarget;
	BYTE bCamp;
	BYTE bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult
		>> wCastle
		>> dwTarget
		>> bCamp;

	CTPlayer *pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(bResult != CBS_SUCCESS)
	{
		pPlayer->SendCS_CASTLEAPPLY_ACK(bResult);
		return EC_NOERROR;
	}

	pPlayer->SendCS_CASTLEAPPLY_ACK(
		bResult,
		wCastle,
		dwTarget,
		bCamp);

	if(dwCharID == dwTarget)
	{
		pPlayer->m_wCastle = wCastle;
		pPlayer->m_bCamp = bCamp;

		if(pPlayer->IsInCastle())
			Teleport(pPlayer, pPlayer->m_wLastSpawnID);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_MONTEMPT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwID;
	DWORD dwKey;
	WORD wMonID;

	pBUF->m_packet
		>> dwID
		>> dwKey
		>> wMonID;

	CTPlayer * pPlayer = FindPlayer(dwID, dwKey);
	if(!pPlayer || !pPlayer->m_bMain)
		return EC_NOERROR;

	pPlayer->m_wTemptedMon = wMonID;

	CTSkill * pSkill = pPlayer->GetMonRecallSkill();
	if(!pSkill)
		return EC_NOERROR;

	pPlayer->Defend(
		pSkill->m_pTSKILL,
		pSkill->m_bLevel,
		pPlayer->m_dwID,
		pPlayer->m_dwID,
		pPlayer->m_bType,
		pPlayer->GetPartyID(),
		pPlayer->m_bCountry,
		pPlayer->m_bAidCountry,
		pPlayer->m_bClass,
		0,0,
		pPlayer->GetMinAP(FALSE),
		pPlayer->GetMaxAP(FALSE),
		pPlayer->GetMinMagicAP(),
		pPlayer->GetMaxMagicAP(),
		0,0,0,0,
		TRUE,
		pPlayer->GetAttackLevel(),
		pPlayer->m_bLevel,
		0,
		HT_NORMAL,
		0,
		pPlayer->m_fPosX,
		pPlayer->m_fPosY,
		pPlayer->m_fPosZ,
		pPlayer->m_fPosX,
		pPlayer->m_fPosY,
		pPlayer->m_fPosZ,
		FALSE);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_GETBLOOD_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	BYTE bBloodType;
	DWORD dwBlood;
	DWORD dwAtkID;
	BYTE bAtkType;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwAtkID
		>> bAtkType
		>> bBloodType
		>> dwBlood;

	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKey);
	if(!pPlayer ||
		!pPlayer->m_pMAP ||
		!pPlayer->m_bMain)
		return EC_NOERROR;

	CTObjBase *pOBJ = NULL;

	DWORD dwMaxHP;
	DWORD dwMaxMP;

	switch(bAtkType)
	{
	case OT_MON		: pOBJ = pPlayer->m_pMAP->FindMonster(dwAtkID); break;
	case OT_RECALL	: pOBJ = pPlayer->FindRecallMon(dwAtkID); break;
	case OT_SELF	: pOBJ = pPlayer->FindSelfObj(dwAtkID); break;
	case OT_PC		: pOBJ = pPlayer; break;
	}

	if( !pOBJ || !pOBJ->m_pMAP)
		return EC_NOERROR;

	// Player Dead
	if(!pOBJ->m_dwHP || pOBJ->m_bStatus == OS_DEAD)
		return EC_NOERROR;

	dwMaxHP = pOBJ->GetMaxHP();
	dwMaxMP = pOBJ->GetMaxMP();

	switch(bBloodType)
	{
	case MTYPE_HI:
		pOBJ->m_dwHP += dwBlood;

		if( pOBJ->m_dwHP > dwMaxHP )
			pOBJ->m_dwHP = dwMaxHP;

		break;
	case MTYPE_MI:
		pOBJ->m_dwMP += dwBlood;

		if( pOBJ->m_dwMP > dwMaxMP )
			pOBJ->m_dwMP = dwMaxMP;

		break;

	case MTYPE_DAMAGE:

		if(pOBJ->m_dwHP > dwBlood)
			pOBJ->m_dwHP -= dwBlood;
		else
			pOBJ->m_dwHP = 1;

		break;
	}

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	pOBJ->m_pMAP->GetNeighbor(
		&vPLAYERS,
		pOBJ->m_fPosX,
		pOBJ->m_fPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_HPMP_ACK(
			pOBJ->m_dwID,
			pOBJ->m_bType,
			pOBJ->m_bLevel,
			dwMaxHP,
			pOBJ->m_dwHP,
			dwMaxMP,
			pOBJ->m_dwMP);

		vPLAYERS.pop_back();
	}

	vPLAYERS.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnCT_USERMOVE_ACK( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	BYTE bChannel;
	WORD wMapID;
	WORD wPartyID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> wPartyID;

	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKEY);
	if(!pPlayer ||
		!pPlayer->m_pMAP ||
		!pPlayer->m_bMain)
		return EC_NOERROR;

	if(wPartyID)
		pPlayer->m_wPartyID = wPartyID;

	if( fPosX == 0.0f && fPosY == 0.0f && fPosZ == 0.0f )
		Teleport( pPlayer, pPlayer->m_wSpawnID);
	else
		Teleport( pPlayer, bChannel, wMapID, fPosX, fPosY, fPosZ);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_USERPOSITION_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	CString strGMName;
	WORD wPartyID = 0;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strGMName;
	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKey);
	if(!pPlayer ||
		!pPlayer->m_pMAP ||
		!pPlayer->m_bMain)
		return EC_NOERROR;
	
	if(IsIndunMap(pPlayer->m_wMapID))
		wPartyID = pPlayer->m_wPartyID;

	SendCT_USERMOVE_ACK(
		strGMName,
		pPlayer->m_bChannel,
		pPlayer->m_wMapID,
		pPlayer->m_fPosX,
		pPlayer->m_fPosY,
		pPlayer->m_fPosZ,
		wPartyID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_PROTECTEDADD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	CString strName;
	BYTE bResult = 1;
	DWORD dwProtected = 0;
	BYTE bOption=0;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strName
		>> bOption;

	DEFINE_QUERY(&m_db, CSPProtectedInsert)
	query->m_dwCharID = dwCharID;
	lstrcpy(query->m_szName, strName);
	query->m_bOption = bOption;

	if(query->Call())
	{
		bResult = query->m_nRET;
		dwProtected = query->m_dwProtected;
	}
	UNDEFINE_QUERY();

	LPPACKETBUF pBuf = new PACKETBUF();
	pBuf->m_packet.SetID(DM_PROTECTEDADD_ACK)
		<< dwCharID
		<< dwKEY
		<< bResult
		<< dwProtected
		<< strName
		<< bOption;

	SayToBATCH(pBuf);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnDM_PROTECTEDADD_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bResult;
	DWORD dwProtected;
	CString strName;
	BYTE bOption;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult
		>> dwProtected
		>> strName
		>> bOption;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(!bResult)
	{
		LPTPROTECTED pPT = new TPROTECTED();
		pPT->m_bChanged = FALSE;
		pPT->m_bOption = bOption;
		pPT->m_dwCharID = dwProtected;
		pPT->m_strName = strName;

		pPlayer->m_mapTPROTECTED.insert(MAPTPROTECTED::value_type(dwProtected, pPT));
		// 차단에 관련된 off-line표시
		SendMW_PROTECTEDCHECK_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, FRIEND_DISCONNECTION, strName);
	}

	pPlayer->SendCS_PROTECTEDADD_ACK(bResult, dwProtected, strName, bOption);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnDM_PROTECTEDERASE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwProtected;

	pBUF->m_packet
		>> dwCharID
		>> dwProtected;

	DEFINE_QUERY(&m_db, CSPProtectedDelete)
	query->m_dwCharID = dwCharID;	
	query->m_dwProtected = dwProtected;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_DEALITEMERROR_REQ(LPPACKETBUF pBUF)
{
	CString strTarget;
	CString strErrorChar;
	BYTE bError;

	pBUF->m_packet
		>> strTarget
		>> strErrorChar
		>> bError;

	CTPlayer * pPlayer = FindPlayer(strTarget);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->ClearDealItem();
	pPlayer->SendCS_DEALITEMEND_ACK(bError, strErrorChar);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_MAGICMIRROR_REQ( LPPACKETBUF pBUF)
{
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
	BYTE bHit;

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

	BYTE bAttackClass;

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
		>> bHit
		>> wSkillID
		>> bSkillLevel
		>> fAtkPosX
		>> fAtkPosY
		>> fAtkPosZ
		>> fDefPosX
		>> fDefPosY
		>> fDefPosZ
		>> dwRemainTick;

	pBUF->m_packet
		>> wAttackPartyID
		>> bAttackCountry
		>> bAttackAidCountry
		>> bAttackClass;

	if(bAttackType == OT_MON)
		return EC_NOERROR;

	CTObjBase *pDEFEND = NULL;

	MAPPLAYER::iterator itTg = m_mapPLAYER.find(dwHostID);
	if(itTg != m_mapPLAYER.end())
		pDEFEND = FindTarget((*itTg).second, bAttackType, dwAttackID);

	dwAttackID = dwTargetID;
	bAttackType = bTargetType;

	if(!pDEFEND)
		return EC_NOERROR;

	CTSkillTemp * pTemp = FindTSkill(wSkillID);
	if(!pTemp)
		return EC_NOERROR;

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
		bHit,
		HT_NORMAL,
		dwRemainTick,
		fAtkPosX,fAtkPosY,fAtkPosZ,
		fDefPosX,fDefPosY,fDefPosZ,
		m_bNation == NATION_US ? TRUE : FALSE);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_PETDEL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwUserID;
	WORD wPetID;

	pBUF->m_packet
		>> dwUserID
		>> wPetID;

	DEFINE_QUERY(&m_db, CSPPetDelete)
	query->m_dwUserID = dwUserID;
	query->m_wPetID = wPetID;
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_PETRIDING_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwRiding;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwRiding;

	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKey);

	if( !pPlayer )
		return EC_NOERROR;

	pPlayer->m_dwRiding = dwRiding;

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_STARTTELEPORT_REQ( LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	BYTE bChannel;
	WORD wMapID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ;

	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKey);

	if( !pPlayer)
		return EC_NOERROR;

	ExitMAP(pPlayer, TRUE, wMapID);

	BYTE bTutorialOut = pPlayer->IsTutorial() && wMapID != TUTORIAL_MAPID;
	if(bTutorialOut)
		pPlayer->m_bStartAct = 2;

	pPlayer->m_bChannel = bChannel;
	pPlayer->m_wMapID = wMapID;
	pPlayer->m_fPosX = fPosX;
	pPlayer->m_fPosY = fPosY;
	pPlayer->m_fPosZ = fPosZ;

	if(pPlayer->m_bMain)
	{
		SendDM_TELEPORT_REQ(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			bChannel,
			wMapID,
			fPosX,
			fPosY,
			fPosZ,
			bTutorialOut);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_EVENTQUARTER_REQ(LPPACKETBUF pBUF)
{
	BYTE bDay;
	BYTE bHour;
	BYTE bMinute;
	BYTE bSelect;
	CString strPresent;

	pBUF->m_packet
		>> bDay
		>> bHour
		>> bMinute
		>> bSelect
		>> strPresent;

	MAPPLAYER::iterator it;
	for(it=m_mapPLAYER.begin(); it!=m_mapPLAYER.end(); it++)
	{
		if((*it).second->m_bMain)
		{
			(*it).second->SendCS_SYSTEMMSG_ACK(
				SM_EVENT_QUARTER,
				0,
				0,
				NAME_NULL,
				NAME_NULL,
				bHour,
				bMinute,
				strPresent,
				bSelect);

			if((*it).second->m_bLuckyNumber == bSelect)
				SendDM_EVENTQUARTER_REQ((*it).second->m_strNAME, bDay, bHour, bMinute);
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_SAVEEXP_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwEXP;

	pBUF->m_packet
		>> dwCharID
		>> dwEXP;

	DEFINE_QUERY(&m_db, CSPSaveEXP)
	query->m_dwCharID = dwCharID;
	query->m_dwEXP = dwEXP;
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_SAVEMONEY_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	pBUF->m_packet
		>> dwCharID
		>> dwGold
		>> dwSilver
		>> dwCooper;

	DEFINE_QUERY(&m_db, CSPSaveMoney)
	query->m_dwCharID = dwCharID;
	query->m_dwGold = dwGold;
	query->m_dwSilver = dwSilver;
	query->m_dwCooper = dwCooper;
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_HELMETHIDE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bHide;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bHide;

	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKEY);

	if( !pPlayer || !pPlayer->m_pMAP )
		return EC_NOERROR;

	if(pPlayer->m_bHelmetHide != bHide)
	{
		pPlayer->m_bHelmetHide = bHide;

		VPLAYER vPlayer;
		vPlayer.clear();

		pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);

		for(DWORD i=0; i<vPlayer.size(); i++)
		{
			vPlayer[i]->SendCS_HELMETHIDE_ACK(
				dwCharID, 
				bHide);
		}

		vPlayer.clear();
	}

	return EC_NOERROR;
}


DWORD CTMapSvrModule::OnMW_PARTYMEMBERRECALLANS_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	CString strUser;
	BYTE bType;
	BYTE bInven;
	BYTE bItem;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strUser
		>> bType
		>> bInven
		>> bItem;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
	{
		SendMW_PARTYMEMBERRECALLANS_ACK(
			IU_NOTFOUND,
			strUser,
			NAME_NULL,
			bType,
			bInven,
			bItem);

		return EC_NOERROR;
	}

	if( !pPlayer->m_bMain ||
		!pPlayer->m_pMAP ||
		pPlayer->m_bStatus == OS_DEAD ||
		pPlayer->m_dealItem.m_bStatus >= DEAL_START ||
		pPlayer->m_wArenaID)
	{
		SendMW_PARTYMEMBERRECALLANS_ACK(
			IU_TARGETBUSY,
			strUser,
			pPlayer->m_strNAME,
			bType,
			bInven,
			bItem);

		return EC_NOERROR;
	}

	switch(bType)
	{
	case TP_RECALL:
		pPlayer->SendCS_PARTYMEMBERRECALLANS_ACK(
			strUser,
			bInven,
			bItem);
		break;

	case TP_MOVETO:
		SendMW_PARTYMEMBERRECALLANS_ACK(
			IU_SUCCESS,
			strUser,
			pPlayer->m_strNAME,
			bType,
			bInven,
			bItem,
			pPlayer->m_bChannel,
			pPlayer->m_wMapID,
			pPlayer->m_fPosX,
			pPlayer->m_fPosY,
			pPlayer->m_fPosZ);
		break;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_PARTYMEMBERRECALL_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bResult;
	BYTE bType;
	CString strName;
	BYTE bInvenID;
	BYTE bItemID;
	BYTE bChannel;
	WORD wMapID;
	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult
		>> strName
		>> bType
		>> bInvenID
		>> bItemID
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ;

	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKEY);

	if( !pPlayer || !pPlayer->m_pMAP )
		return EC_NOERROR;

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;

	if(bResult)
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(bResult, bType, strName);
		return EC_NOERROR;
	}

	CTInven * pInven = pPlayer->FindTInven(bInvenID);
	if(!pInven)
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTFOUND, bType, strName);
		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bItemID);
	if(!pItem)
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTFOUND, bType, strName);
		return EC_NOERROR;
	}

	if(pItem->m_pTITEM->m_bType != IT_USE)
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTFOUND, bType, strName);
		return EC_NOERROR;
	}

	if(pPlayer->m_wArenaID)
	{
		pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTFOUND, bType, strName);
		return EC_NOERROR;
	}

	switch(bType)
	{
	case TP_RECALL:
		if(pItem->m_pTITEM->m_bKind != IK_MEMBERRECALL)
		{
			pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTFOUND, bType, strName);
			return EC_NOERROR;
		}
		else
		{
			SendCT_USERMOVE_ACK(
				strName, 
				pPlayer->m_bChannel, 
				pPlayer->m_wMapID, 
				pPlayer->m_fPosX, 
				pPlayer->m_fPosY, 
				pPlayer->m_fPosZ);
		}
		break;
	case TP_MOVETO:
		if(pItem->m_pTITEM->m_bKind != IK_MOVETOCHAR)
		{
			pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTFOUND, bType, strName);
			return EC_NOERROR;
		}
		else
		{
			SendCT_USERMOVE_ACK(
				pPlayer->m_strNAME, 
				bChannel, 
				wMapID, 
				fPosX, 
				fPosY, 
				fPosZ);
		}
		break;
	default:
		if(pItem->m_pTITEM->m_bKind != IK_MOVETOCHAR)
		{
			pPlayer->SendCS_PARTYMEMBERRECALL_ACK(IU_NOTFOUND, TP_NONEDEF, strName);
			return EC_NOERROR;
		}
		break;
	}

	UseItem(pPlayer, pInven, pItem, 1);

	pPlayer->SendCS_PARTYMEMBERRECALL_ACK(bResult, bType, strName);
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CASHITEMCABINET_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwUserID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwUserID;

	WORD wCount = 0;
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_CASHITEMCABINET_ACK)
		<< dwCharID
		<< dwKEY;

	__int64 ldwTime = 0;
	DWORD dwCash = 0;
	DWORD dwBonus = 0;

	DEFINE_QUERY(&m_db, CSPGetCashCabinetUseTime)
	query->m_dwCharID = dwCharID;
	ldwTime = 0;
	dwCash = 0;
	dwBonus = 0;
	if(query->Call() && !query->m_nRet)
	{
		ldwTime = __DBTOTIME(query->m_dateTime);
		dwCash = query->m_dwCash;
		dwBonus = query->m_dwBonus;
	}
	UNDEFINE_QUERY()

	WORD wSize = pMSG->m_packet.GetSize();

	pMSG->m_packet
		<< wCount
		<< ldwTime
		<< dwCash
		<< dwBonus;

	DEFINE_QUERY(&m_db, CTBLCashItemCabinet)
	query->m_dwUserID = dwUserID;

	if(query->Open())
	{
		while(query->Fetch())
		{
			pMSG->m_packet
				<< query->m_dwID;

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

DWORD CTMapSvrModule::OnDM_CASHITEMCABINET_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	WORD wCount;
	__int64 ldwTime;
	DWORD dwCash;
	DWORD dwBonus;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> wCount
		>> ldwTime
		>> dwCash
		>> dwBonus;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

	DWORD dwID;
	MAPTCASHITEM::iterator it;

	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_CASHITEMCABINET_ACK)
		<< ldwTime
		<< dwCash
		<< dwBonus;

	WORD wSize = pMsg->GetSize();
	WORD wCashItemCnt = 0;

	(*pMsg)
		<< wCashItemCnt;

	for(WORD i=0; i<wCount; i++)
	{
		pBUF->m_packet
			>> dwID;
		
		CTItem * pItem = CreateItem(&(pBUF->m_packet));
		if(pItem)
		{
			(*pMsg)
				<< dwID;

			pItem->WrapPacketClient(pMsg, pPlayer->m_dwID, FALSE);
			wCashItemCnt++;
			delete pItem;
		}
	}

	memcpy(pMsg->GetBuffer()+wSize, &wCashItemCnt, sizeof(wCount));

	pPlayer->Say(pMsg);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CASHITEMGET_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	CString strName;
	DWORD dwUserID;
	DWORD dwID;
	BYTE bInven;
	BYTE bSlot;
	BYTE bRet;
	WORD wCashShopID = 0;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strName
		>> dwUserID
		>> dwID
		>> bInven
		>> bSlot;

	LPPACKETBUF pMSG = new PACKETBUF();

	DEFINE_QUERY(&m_db, CSPCashItemGet)
	query->m_dwUserID = dwUserID;
	query->m_dwCharID = dwCharID;
	query->m_dwCashItemID = dwID;
	query->m_bInven = bInven;
	query->m_bSlot = bSlot;
	if(query->Call())
		bRet = BYTE(query->m_nRET);
	else
		bRet = CASHSHOP_INTERNAL;

	pMSG->m_packet.SetID(DM_CASHITEMGET_ACK)
		<< dwCharID
		<< dwKEY
		<< strName
		<< bRet
		<< dwID
		<< bInven
		<< bSlot;

	WrapItem(&(pMSG->m_packet), query->m_item);
	UNDEFINE_QUERY()

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CASHITEMGET_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	CString strName;
	BYTE bRet;
	DWORD dwCashItemID;
	BYTE bInven;
	BYTE bSlot;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strName
		>> bRet
		>> dwCashItemID
		>> bInven
		>> bSlot;

	CTItem * pItem = CreateItem(&(pBUF->m_packet));
	if(!pItem)
		return EC_NOERROR;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer || !pPlayer->m_bMain)
	{
		if(!bRet)
		{
			SendDM_POSTRECV_REQ(
				0, dwCharID, GetSvrMsg(NAME_OPERATOR), strName,
				BuildNetString(NAME_NULL, GetSvrMsg(MSG_ITEMERROR_TITLE)),
				BuildNetString(NAME_NULL, GetSvrMsg(MSG_ITEMERROR_MESSAGE)),
				POST_PACKATE,
				0,0,0, pItem);
		}

		delete pItem;
		return EC_NOERROR;
	}

	if(bRet)
	{
		pPlayer->SendCS_CASHITEMGET_ACK(bRet, dwCashItemID);
		delete pItem;

		return EC_NOERROR;
	}


#ifdef DEF_UDPLOG
	m_pUdpSocket->LogCashItem(LOGMAP_CASHBANKOUT, pPlayer, 0, 0, pItem, dwCashItemID, 0, 0 );
#endif

	if(pPlayer->CanPush(pItem, bInven, bSlot))
		pPlayer->PushTItem(pItem, bInven, bSlot);
	else
	{
		VTITEM vItem;
		vItem.clear();
		vItem.push_back(pItem);

		if(pPlayer->CanPush(&vItem, 0))
			pPlayer->PushTItem(&vItem);
		else
		{
			SendDM_POSTRECV_REQ(
				0, pPlayer->m_dwID, GetSvrMsg(NAME_OPERATOR), pPlayer->m_strNAME,
				BuildNetString(NAME_NULL, GetSvrMsg(MSG_ITEMERROR_TITLE)),
				BuildNetString(NAME_NULL, GetSvrMsg(MSG_ITEMERROR_MESSAGE)),
				POST_PACKATE,
				0,0,0, pItem);

			delete pItem;
		}
	}

	pPlayer->SendCS_CASHITEMGET_ACK(bRet, dwCashItemID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CASHSHOPITEMLIST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwUserID;
	WORD wItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwUserID
		>> wItemID;

	DWORD dwCash = 0;
	DWORD dwCashBonus = 0;

	DEFINE_QUERY(&m_db, CSPGetCash)
	query->m_dwUserID = dwUserID;
	query->m_dwCash = 0;
	query->m_dwCashBonus = 0;
	if(query->Call() )
	{
		dwCash = query->m_dwCash;
		dwCashBonus = query->m_dwCashBonus;
	}
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_CASHSHOPITEMLIST_ACK)
		<< dwCharID
		<< dwKEY
		<< dwCash
		<< dwCashBonus
		<< wItemID;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CASHSHOPITEMLIST_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwCash;
	DWORD dwCashBonus;
	WORD wItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwCash
		>> dwCashBonus
		>> wItemID;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_CASHSHOPITEMLIST_ACK(
		dwCash,
		dwCashBonus,
		wItemID,
		m_vCashCategory);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CASHITEMBUY_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwUserID;
	WORD wCashItemID;
	BYTE bBuyType;
	CString strTarget;

	__int64 dlID = 0;
	BYTE bRet = 0;
	DWORD dwCash = 0;
	DWORD dwCashBonus = 0;
	DWORD dwTargetID = 0;
	CString strPresent = NAME_NULL;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwUserID
		>> wCashItemID
		>> bBuyType
		>> strTarget;

	DEFINE_QUERY(&m_db, CSPCashItemBuy)
	query->m_dwUserID = dwUserID;
	query->m_dwCharID = dwCharID;
	query->m_wCashItemID = wCashItemID;
	query->m_bBuyType = bBuyType;
	lstrcpy(query->m_szTarget, strTarget);

	if(query->Call())
	{
		bRet = BYTE(query->m_nRET);
		dwCash = query->m_dwCash;
		dwCashBonus = query->m_dwCashBonus;
		dwTargetID = query->m_dwTargetID;
		strPresent = query->m_szPresent;
		dlID = query->m_dlID;
	}
	else
		bRet = CASHSHOP_INTERNAL;
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_CASHITEMBUY_ACK)
		<< dwCharID
		<< dwKEY
		<< bRet
		<< wCashItemID
		<< dwCash
		<< dwCashBonus
		<< dlID
		<< bBuyType
		<< dwTargetID
		<< strTarget
		<< strPresent;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CASHITEMBUY_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bRet;
	WORD wCashItemID;
	DWORD dwCash;
	DWORD dwCashBonus;
	__int64 dlID;
	BYTE bBuyType;
	DWORD dwTargetID;
	CString strTarget;
	CString strPresent;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bRet
		>> wCashItemID
		>> dwCash
		>> dwCashBonus
		>> dlID
		>> bBuyType
		>> dwTargetID
		>> strTarget
		>> strPresent;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKEY);
	if(!pPlayer)
		return EC_NOERROR;

	if(bRet)
	{
		pPlayer->SendCS_CASHITEMBUY_ACK(bRet, 0, 0);
		return EC_NOERROR;
	}

	if(bBuyType == CASHITEM_BUY)
	{
#ifdef DEF_UDPLOG
		m_pUdpSocket->LogCashItem(LOGMAP_CASHITEMBUY, pPlayer, 0, NULL, NULL, wCashItemID, dwCash, dwCashBonus);
#endif
	}
	else
	{
		CString strTitle, strMessage;
		strTitle.Format(GetSvrMsg(MSG_PRESENT_TITLE), pPlayer->m_strNAME);
		strMessage.Format(GetSvrMsg(MSG_PRESENT_MESSAGE), pPlayer->m_strNAME, strPresent);

		SendDM_POSTRECV_REQ(
			0, dwTargetID, GetSvrMsg(NAME_OPERATOR), strTarget,
			BuildNetString(NAME_NULL, strTitle), BuildNetString(NAME_NULL, strMessage),
			POST_NORMAL,
			0,0,0,NULL);

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogCashItem(LOGMAP_CASHITEMPRESENT, pPlayer, dwTargetID, strTarget, NULL, wCashItemID, dwCash, dwCashBonus);
#endif
	}

	pPlayer->SendCS_CASHITEMBUY_ACK(bRet, dwCash, dwCashBonus);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_SOULMATE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	DWORD dwSoulmate;
	CString strSoulmate;
	DWORD dwTime;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> dwSoulmate
		>> strSoulmate
		>> dwTime;

	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKEY);

	if( !pPlayer )
		return EC_NOERROR;

	pPlayer->SetSoulmate(dwSoulmate, strSoulmate, dwTime);

	if(dwSoulmate)
		pPlayer->SendCS_SOULMATE_ACK();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_SOULMATESEARCH_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bResult;
	DWORD dwSoulID;
	CString strSoulmate;
	DWORD dwRegion;

	BYTE bNpcInvenID;
	BYTE bNpcItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult
		>> dwSoulID
		>> strSoulmate
		>> dwRegion
		>> bNpcInvenID
		>> bNpcItemID;

	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKEY);

	if( !pPlayer || !pPlayer->m_pMAP )
		return EC_NOERROR;

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
		if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL)
		{
			pPlayer->SendCS_SOULMATESEARCH_ACK(SOULMATE_NPCCALLERROR);
			return EC_NOERROR;
		}

		UseItem(pPlayer, pNpcInven, pNpcItem, 1);		
	}

	pPlayer->UseMoney(pPlayer->m_pTLEVEL->m_dwSearchCost, TRUE);
	pPlayer->SendCS_MONEY_ACK();

	if(bResult == SOULMATE_SUCCESS)
		pPlayer->SetSoulmate(dwSoulID, strSoulmate, 0);

	pPlayer->SendCS_SOULMATESEARCH_ACK(
		bResult,
		dwSoulID,
		strSoulmate,
		dwRegion);


	
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_SOULMATEREG_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bResult;
	BYTE bReg;
	DWORD dwSoulmate;
	CString strSoulmate;
	DWORD dwRegion;

	BYTE bNpcInvenID;  
	BYTE bNpcItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult
		>> bReg
		>> bNpcInvenID
		>> bNpcItemID
		>> dwSoulmate
		>> strSoulmate
		>> dwRegion;

	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKEY);

	if( !pPlayer || !pPlayer->m_pMAP )
		return EC_NOERROR;

	if(bResult == SOULMATE_SUCCESS &&
		bReg == TRUE)
	{
		if(bNpcInvenID != INVEN_NULL && bNpcItemID != INVALID_SLOT)
		{
			if(pPlayer->m_wMapID != 0 && pPlayer->m_wMapID != 8 )
			{
				pPlayer->SendCS_SOULMATEREG_ACK(SOULMATE_INVALIDPOS);
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
			if(!pNpcItem || pNpcItem->m_pTITEM->m_bKind != IK_NPCCALL)
			{
				pPlayer->SendCS_SOULMATEREG_ACK(SOULMATE_NPCCALLERROR);
				return EC_NOERROR;
			}

			UseItem(pPlayer, pNpcInven, pNpcItem, 1);
		}

		pPlayer->UseMoney(pPlayer->m_pTLEVEL->m_dwRegCost, TRUE);
		pPlayer->SetSoulmate(dwSoulmate, strSoulmate, 0);
		pPlayer->SendCS_MONEY_ACK();
	}

	if(bReg)
		pPlayer->SendCS_SOULMATEREG_ACK(bResult, dwSoulmate, strSoulmate, dwRegion);
	else
		pPlayer->SendCS_SOULMATEREGREADY_ACK(bResult, strSoulmate,bNpcInvenID,bNpcItemID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_SOULMATEEND_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	BYTE bResult;
	DWORD dwTime;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> bResult
		>> dwTime;

	CTPlayer *pPlayer = FindPlayer( dwCharID, dwKEY);

	if( !pPlayer )
		return EC_NOERROR;

	if(bResult == SOULMATE_SUCCESS)
		pPlayer->SetSoulmate(0, NAME_NULL, dwTime);

	pPlayer->SendCS_SOULMATEEND_ACK(bResult);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_ENDWAR_REQ(LPPACKETBUF pBUF)
{
	WORD wCastle;

	pBUF->m_packet
		>> wCastle;

	LPTLOCAL pCastle = FindCastleZone(wCastle);
	if(!pCastle)
		return EC_NOERROR;

	pCastle->m_bStatus = BS_NOBATTLE;

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CASTLEWARINFO_REQ(LPPACKETBUF pBUF)
{
	WORD wCastle;
	DWORD dwDefGuild;
	BYTE bDefCountry;
	CString strDefName;
	WORD wDPoint;
	DWORD dwAtkGuild;
	CString strAtkName;
	WORD wCPoint;
	DWORD dwPCount;
	MAPDWORD mapPoint;

	DWORD dwGuildID;
	DWORD dwGuildPoint;
	BYTE bCountry;
	CString strGuildName;

	pBUF->m_packet
		>> wCastle
		>> dwDefGuild
		>> strDefName
		>> bDefCountry
		>> wDPoint
		>> dwAtkGuild
		>> strAtkName
		>> wCPoint
		>> dwPCount;

	LPTLOCAL pCastle = FindCastle(wCastle);
	if(!pCastle)
		return EC_NOERROR;

	pCastle->m_bDefCountry = bDefCountry;
	pCastle->m_dwDefGuildID = dwDefGuild;
	pCastle->m_strDefName = strDefName;
	pCastle->m_wDPoint = wDPoint;
	pCastle->m_dwAtkGuildID = dwAtkGuild;
	pCastle->m_wCPoint = wCPoint;
	pCastle->m_strAtkName = strAtkName;

	pCastle->m_mapTPoint.clear();
	pCastle->m_vTop3[TCONTRY_D].clear();
	pCastle->m_vTop3[TCONTRY_C].clear();

	for(DWORD i=0; i<dwPCount; i++)
	{
		pBUF->m_packet
			>> dwGuildID
			>> dwGuildPoint;

		pCastle->m_mapTPoint.insert(MAPDWORD::value_type(dwGuildID, dwGuildPoint));
	}

	BYTE bTop3Cnt;
	pBUF->m_packet
		>> bTop3Cnt;

	for(BYTE t3=0; t3<bTop3Cnt; t3++)
	{
		TLOCALSUMPOINT lsp;

		pBUF->m_packet
			>> bCountry
			>> lsp.m_strName
			>> lsp.m_wPoint;

		pCastle->m_vTop3[bCountry].push_back(lsp);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CASHCABINETBUY_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwCharID;
	DWORD dwKey;
	__int64 dlTime = 0;
	DWORD dwCash = 0;
	DWORD dwBonus = 0;
	WORD wShopItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wShopItemID;

	DEFINE_QUERY(&m_db, CSPCashCabinetBuy)
	query->m_dwCharID = dwCharID;
	query->m_wCashItemID = wShopItemID;
	if(query->Call())
	{
		bRet = (BYTE)query->m_nRet;
		dwCash = query->m_dwCash;
		dwBonus = query->m_dwBonus;
		dlTime = __DBTOTIME(query->m_dateTime);
	}
	else
		bRet = CASHSHOP_INTERNAL;
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_CASHCABINETBUY_ACK)
		<< bRet
		<< dwCharID
		<< dwKey
		<< dlTime
		<< dwCash
		<< dwBonus
		<< wShopItemID;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnDM_CASHCABINETBUY_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwCharID;
	DWORD dwKey;
	__int64 dlTime;
	DWORD dwCash;
	DWORD dwBonus;
	WORD wShopItemID;

	pBUF->m_packet
		>> bRet
		>> dwCharID
		>> dwKey
		>> dlTime
		>> dwCash
		>> dwBonus
		>> wShopItemID;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer || !pPlayer->m_bMain)
		return EC_NOERROR;

#ifdef DEF_UDPLOG
	if(!bRet)
		m_pUdpSocket->LogCashCabinetBuy(LOGMAP_CASHCABINETBUY, pPlayer, wShopItemID, dlTime, dwCash, dwBonus, bRet);
#endif

	pPlayer->SendCS_CASHCABINETBUY_ACK(
		bRet,
		dlTime,
		dwCash,
		dwBonus);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CASHITEMPUTIN_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwCharID;
	DWORD dwKey;
	CString strName;
	BYTE bInven;
	BYTE bItemID;
	DWORD dwNewID;
	DWORD dwCash = 0;
	DWORD dwBonus = 0;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strName
		>> bInven
		>> bItemID;

	CTItem * pItem = CreateItem(&(pBUF->m_packet));
	if(!pItem)
		return EC_NOERROR;

	DEFINE_QUERY(&m_db, CSPCashItemPutin)
	query->m_dwCharID = dwCharID;
	query->m_bInven = bInven;
	query->m_bItemID = bItemID;
	if(query->Call())
	{
		bRet = (BYTE)query->m_nRet;
		dwNewID = query->m_dwNewID;
		dwCash = query->m_dwCash;
		dwBonus = query->m_dwBonus;
	}
	else
		bRet = CASHSHOP_INTERNAL;
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_CASHITEMPUTIN_ACK)
		<< bRet
		<< dwCharID
		<< dwKey
		<< strName
		<< bInven
		<< bItemID
		<< dwNewID
		<< dwCash
		<< dwBonus;

	pItem->WrapPacket(&(pMSG->m_packet));
	delete pItem;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CASHITEMPUTIN_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwCharID;
	DWORD dwKey;
	CString strName;
	BYTE bInven;
	BYTE bItemID;
	DWORD dwNewID;
	DWORD dwCash;
	DWORD dwBonus;

	pBUF->m_packet
		>> bRet
		>> dwCharID
		>> dwKey
		>> strName
		>> bInven
		>> bItemID
		>> dwNewID
		>> dwCash
		>> dwBonus;

	CTItem * pItem = CreateItem(&(pBUF->m_packet));
	if(!pItem)
		return EC_NOERROR;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer || !pPlayer->m_bMain)
	{
		if(bRet)
		{
			SendDM_POSTRECV_REQ(
				0, dwCharID, GetSvrMsg(NAME_OPERATOR), strName,
				BuildNetString(NAME_NULL, GetSvrMsg(MSG_ITEMERROR_TITLE)),
				BuildNetString(NAME_NULL, GetSvrMsg(MSG_ITEMERROR_MESSAGE)),
				POST_PACKATE,
				0,0,0, pItem);
		}

		if(pPlayer)
			pPlayer->SendCS_CASHITEMPUTIN_ACK(bRet, 0, NULL);

		delete pItem;
		return EC_NOERROR;
	}

	if(bRet)
	{
		CTInven *pInven = pPlayer->FindTInven(bInven);
		if(!pInven)
		{
			VTITEM vItem;
			vItem.clear();
			vItem.push_back(pItem);

			if(pPlayer->CanPush(&vItem, 0))
				pPlayer->PushTItem(&vItem);
		}
		else
		{
			if(pPlayer->CanPush(pItem, bInven, bItemID))
				pPlayer->PushTItem(pItem, bInven, bItemID);
			else
			{
				VTITEM vItem;
				vItem.clear();
				vItem.push_back(pItem);

				if(pPlayer->CanPush(&vItem, 0))
					pPlayer->PushTItem(&vItem);
				else
				{
					SendDM_POSTRECV_REQ(
						0, dwCharID, GetSvrMsg(NAME_OPERATOR), strName,
						BuildNetString(NAME_NULL, GetSvrMsg(MSG_ITEMERROR_TITLE)),
						BuildNetString(NAME_NULL, GetSvrMsg(MSG_ITEMERROR_MESSAGE)),
						POST_PACKATE,
						0,0,0, pItem);

					delete pItem;
				}
			}
		}

		pPlayer->SendCS_CASHITEMPUTIN_ACK(
			bRet, 0, NULL);
	}
	else
	{
		pPlayer->SendCS_CASHITEMPUTIN_ACK(
			bRet,
			dwNewID,
			pItem,
			dwCash,
			dwBonus);

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogCashItem(LOGMAP_CASHBANKIN, pPlayer, 0, 0, pItem, 0, dwCash, dwBonus );
#endif

		delete pItem;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_DELETEDEALITEM_REQ(LPPACKETBUF pBUF)
{
	BYTE bCount;

	DEFINE_QUERY(&m_db, CSPDeleteDealItem)

	pBUF->m_packet
		>> bCount;

	for(BYTE is=0; is<bCount; is++)
	{
		pBUF->m_packet
			>> query->m_dlID;

		query->Call();
	}

	UNDEFINE_QUERY()
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_RECALLMONDATA_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwID;
	BYTE bChannel;
	FLOAT fPosX;
	FLOAT fPosZ;
	BYTE bSkillCnt;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwID
		>> bChannel
		>> fPosX
		>> fPosZ;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	CTRecallMon * pRecall = pPlayer->FindRecallMon(dwID);
	if(!pRecall || !pRecall->m_pMAP)
		return EC_NOERROR;

	WORD wNewX = WORD(fPosX) / CELL_SIZE;
	WORD wNewZ = WORD(fPosZ) / CELL_SIZE;
	DWORD dwNEW = MAKELONG( wNewX, wNewZ);

	MAPTCELL::iterator it = pRecall->m_pMAP->m_mapTCELL.find(dwNEW);
	if(it==pRecall->m_pMAP->m_mapTCELL.end())
		return EC_NOERROR;

	pRecall->m_bMain = (*it).second->IsMainCell(bChannel);
	if(!pRecall->m_bMain)
		return EC_NOERROR;

	pBUF->m_packet
		>> pRecall->m_bStatus
		>> pRecall->m_bMode
		>> pRecall->m_dwHP
		>> pRecall->m_dwMP
		>> bSkillCnt;

	WORD wSkillID;
	for(BYTE i=0; i<bSkillCnt; i++)
	{
		DWORD dwRemainTick;
		pBUF->m_packet
			>> wSkillID
			>> dwRemainTick;

		CTSkill * pSkill = pRecall->FindTSkill(wSkillID);
		if(pSkill)
		{
			if(dwRemainTick)
			{
				pSkill->m_dwUseTick = m_dwTick;
				pSkill->m_dwDelayTick = dwRemainTick;
			}
			else
			{
				pSkill->m_dwUseTick = 0;
				pSkill->m_dwDelayTick = 0;
			}
		}
	}

	pRecall->ClearMaintainSkill();

	WORD wMaintainCnt;
	pBUF->m_packet
		>> wMaintainCnt;

	for(WORD i=0; i<wMaintainCnt; i++)
	{
		CTSkill * pMaintain = new CTSkill();
		pBUF->m_packet
			>> wSkillID
			>> pMaintain->m_bLevel
			>> pMaintain->m_dwMaintainTick
			>> pMaintain->m_bAttackType
			>> pMaintain->m_dwAttackID
			>> pMaintain->m_bHostType
			>> pMaintain->m_dwHostID
			>> pMaintain->m_bAttackCountry;

		CTSkillTemp *pTemp = FindTSkill(wSkillID);
		if(pTemp && !pRecall->FindMaintainSkill(wSkillID))
		{
			pMaintain->m_pTSKILL = pTemp;
			pMaintain->m_dwStartTick = pMaintain->m_dwMaintainTick ? m_dwTick : 0;

			for( DWORD j=0; j<DWORD(pMaintain->m_pTSKILL->m_vData.size()); j++)
			{
				LPTSKILLDATA pData = pMaintain->m_pTSKILL->m_vData[j];

				if( pData->m_bType == SDT_TRANS )
					pRecall->SetTransAttack(pData->m_wValue);
			}
			pRecall->PushMaintainSkill(pMaintain);
		}
		else
			delete pMaintain;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTINVENITEM_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	CString strName;
	BYTE bInven;
	BYTE bCount;
	DWORD dwMakeID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strName
		>> bInven
		>> bCount;

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_POSTINVENITEM_ACK)
		<< dwCharID
		<< dwKey
		<< bInven
		<< bCount;

	for(BYTE i=0; i<bCount; i++)
	{
		if( i%3 == 0 )
		{
			DEFINE_QUERY(&m_db, CSPSavePost)
			query->m_dwCharID = dwCharID;
			query->m_dwSendID = 0;
			lstrcpy(query->m_szTarget, strName);
			lstrcpy(query->m_szSender, GetSvrMsg(NAME_OPERATOR));
			lstrcpy(query->m_szTitle, BuildNetString(NAME_NULL, GetSvrMsg(MSG_POSTINVENITEM)));
			lstrcpy(query->m_szMessage, BuildNetString(NAME_NULL, GetSvrMsg(MSG_POSTINVENITEM)));
			query->m_bType = POST_PACKATE;
			query->m_bRead = FALSE;
			query->m_dwGold = 0;
			query->m_dwSilver = 0;
			query->m_dwCooper = 0;
			__TIMETODB(m_timeCurrent, query->m_timeRecv);

			if(query->Call() && !query->m_nRet)
				dwMakeID = query->m_dwMakeID;
			else
				dwMakeID = 0;

			UNDEFINE_QUERY()

			pMsg->m_packet
				<< dwMakeID;
		}

		DEFINE_QUERY(&m_db, CSPSaveItemDirect)
		query->m_bStorageType = STORAGE_POST;
		query->m_dwStorageID = dwMakeID;
		query->m_bOwnerType = TOWNER_CHAR;
		query->m_dwOwnerID = dwCharID;

		WrapItemQuery(&(pBUF->m_packet), query->m_item);

		if(dwMakeID)
			query->Call();

		UNDEFINE_QUERY();
	}

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTINVENITEM_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bInven;
	BYTE bCount;
	DWORD dwMakeID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bInven
		>> bCount;

	CTPlayer * pRecver = FindPlayer(dwCharID, dwKey);
	if(!pRecver || !pRecver->m_bMain)
		return EC_NOERROR;

	LPTPOST pPost = NULL;

	for(BYTE i=0; i<bCount; i++)
	{
		if( i%3 == 0)
		{
			pBUF->m_packet
				>> dwMakeID;

			pRecver->SendCS_POSTRECV_ACK(
				dwMakeID,
				FALSE,
				GetSvrMsg(NAME_OPERATOR),
				BuildNetString(NAME_NULL, GetSvrMsg(MSG_POSTINVENITEM)),
				POST_PACKATE,
				m_timeCurrent);
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_SAVECHARBASE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	BYTE bType;
	BYTE bValue;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> bType
		>> bValue
		>> strName;

	DEFINE_QUERY(&m_db, CSPSaveCharBase)
	query->m_dwCharID = dwCharID;
	query->m_bType = bType;
	query->m_bValue = bValue;
	lstrcpy(query->m_szName, strName);
	query->Call();
	UNDEFINE_QUERY();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CHECKCHANGENAME_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bInven;
	BYTE bItem;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bInven
		>> bItem
		>> strName;

	BYTE bRet;
	DEFINE_QUERY(&m_db, CSPCheckDuplicateName)
	query->m_dwCharID = dwCharID;
	lstrcpy(query->m_szName, strName);
	if(query->Call())
		bRet = BYTE(query->m_nRet);
	else
		bRet = CCB_FAIL;
	UNDEFINE_QUERY()

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_CHECKCHANGENAME_ACK)
		<< bRet
		<< dwCharID
		<< dwKey
		<< bInven
		<< bItem
		<< strName;

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CHECKCHANGENAME_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bInven;
	BYTE bItem;
	CString strName;

	pBUF->m_packet
		>> bRet
		>> dwCharID
		>> dwKey
		>> bInven
		>> bItem
		>> strName;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer || !pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(pPlayer->m_bStore || pPlayer->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;

	if(bRet)
	{
		pPlayer->SendCS_CHANGECHARBASE_ACK(
			bRet,
			dwCharID,
			IK_NAME,
			0,
			strName);

		return EC_NOERROR;
	}

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
	{
		pPlayer->SendCS_CHANGECHARBASE_ACK(
			CCB_NOITEM,
			dwCharID,
			IK_NAME,
			0,
			strName);

		return EC_NOERROR;
	}

	CTItem * pItem = pInven->FindTItem(bItem);
	if(!pItem)
	{
		pPlayer->SendCS_CHANGECHARBASE_ACK(
			CCB_NOITEM,
			dwCharID,
			IK_NAME,
			0,
			strName);

		return EC_NOERROR;
	}

	pPlayer->ChangeCharBase(IK_NAME, strName);
	UseItem(pPlayer, pInven, pItem, 1);
	pPlayer->SendCS_MOVEITEM_ACK(MI_SUCCESS);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CHANGECHARBASE_REQ(LPPACKETBUF pBUF)
{
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

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer || !pPlayer->m_pMAP)
		return EC_NOERROR;
	
	DWORD dwSecond = 0;

	switch(bType)
	{
	case IK_FACE:
		pPlayer->m_bFace = bValue;
		break;
	case IK_HAIR:
		pPlayer->m_bHair = bValue;
		break;
	case IK_RACE:
		pPlayer->m_bRace = bValue;
		if(pPlayer->m_bMain)
		{
			CTRecallMon * pRe = pPlayer->FindRecallPet();
			if(pRe)
				SendMW_RECALLMONDEL_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, pRe->m_dwID);

			MAPTPET::iterator itPET = pPlayer->m_mapTPET.begin();
			while(itPET != pPlayer->m_mapTPET.end())
			{
				LPTPET pPet = (*itPET).second;
				itPET = pPlayer->m_mapTPET.erase(itPET);
				pPet->m_wPetID = MAKEWORD(pPlayer->m_bRace, HIBYTE(pPet->m_wPetID));
				MAPTPETTEMP::iterator find = m_mapTPET.find(pPet->m_wPetID);
				if(find != m_mapTPET.end())
				{
					pPet->m_pTPET = (*find).second;
					pPlayer->m_mapTPET.insert(MAPTPET::value_type( pPet->m_wPetID, pPet));
				}
				else
					delete pPet;
			}

			pPlayer->m_pTRACE = FindTRace(pPlayer->m_bRace);
			pPlayer->SendCS_HPMP_ACK(
				pPlayer->m_dwID,
				pPlayer->m_bType,
				pPlayer->m_bLevel,
				pPlayer->GetMaxHP(),
				pPlayer->m_dwHP,
				pPlayer->GetMaxMP(),
				pPlayer->m_dwMP);
			pPlayer->SendCS_CHARSTATINFO_ACK(pPlayer);
		}
		break;
	case IK_SEX:
		pPlayer->m_bSex = bValue;
		break;
	case IK_NAME:
		{
			for(DWORD ll=0; ll<m_vOccupation.size(); ll++)
			{
				LPTLOCAL pLocal = m_vOccupation[ll];

				if( pLocal->m_strHero == pPlayer->m_strNAME )
					pLocal->m_strHero.Empty();
			}

			LPTOURNAMENTPLAYER pTnmt = FindTournamentPlayer(pPlayer->m_dwID);
			if(pTnmt)
				pTnmt->m_strName = strName;

			CString strOriName = pPlayer->m_strNAME;
			strOriName.MakeUpper();
			MAPPLAYERNAME::iterator it = m_mapPLAYERNAME.find(strOriName);
			if(it != m_mapPLAYERNAME.end())
				m_mapPLAYERNAME.erase(it);

			pPlayer->m_strNAME = strName;
			CString strChgName = strName;
			strChgName.MakeUpper();
			m_mapPLAYERNAME.insert(MAPPLAYERNAME::value_type(strChgName, pPlayer));
		}
		break;
	case IK_COUNTRY:
		pPlayer->m_bCountry = bValue;
		break;
	case IK_AIDCOUNTRY:
		pPlayer->m_bAidCountry = bValue;
		pPlayer->m_dlAidDate = (bValue == TCONTRY_N) ? 0 : m_timeCurrent;
		dwSecond = pPlayer->GetAidLeftTime();
		break;
	}

	if(!pPlayer->m_bMain)
		return EC_NOERROR;

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	pPlayer->m_pMAP->GetNeighbor(
		&vPLAYERS,
		pPlayer->m_fPosX,
		pPlayer->m_fPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_CHANGECHARBASE_ACK(
			CCB_SUCCESS,
			dwCharID,
			bType,
			bValue,
			strName,
			dwSecond);

		vPLAYERS.pop_back();
	}
	vPLAYERS.clear();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_HEROSELECT_REQ(LPPACKETBUF pBUF)  
{
	WORD wBattleZoneID;
	CString strHeroName;
	__time64_t timeHero;

	pBUF->m_packet
		>> wBattleZoneID
		>> strHeroName
		>> timeHero;

	MAPTLOCAL::iterator itL = m_mapTLOCAL.find(wBattleZoneID);
	if( itL != m_mapTLOCAL.end() )
	{
		(*itL).second->m_strHero = strHeroName; 
		if(!strHeroName.IsEmpty())
			(*itL).second->m_timeHero = timeHero;

		return EC_NOERROR;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_STOPTHECLOCK_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bRet;
	BYTE bInven;
	BYTE bItem;
	WORD wShopItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bInven
		>> bItem
		>> wShopItemID;

	DEFINE_QUERY(&m_db, CSPStopTheClock)
	query->m_dwCharID = dwCharID;
	query->m_wShopItemID = wShopItemID;
	if(query->Call())
		bRet = query->m_nRet ? STC_NEEDCASH : 0;
	else
		bRet = STC_FAIL;
	UNDEFINE_QUERY()

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_STOPTHECLOCK_ACK)
		<< dwCharID
		<< dwKey
		<< bRet
		<< bInven
		<< bItem
		<< wShopItemID;

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_STOPTHECLOCK_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bRet;
	BYTE bInven;
	BYTE bItem;
	WORD wShopItemID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bRet
		>> bInven
		>> bItem
		>> wShopItemID;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer || !pPlayer->m_pMAP || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(bRet)
	{
		pPlayer->SendCS_STOPTHECLOCK_ACK(
			bRet,
			bInven,
			bItem,
			0);

		return EC_NOERROR;
	}

	__time64_t dEndTime = 0;

	CTInven * pInven = pPlayer->FindTInven(bInven);
	if(!pInven)
		return EC_NOERROR;

	MAPTCASHITEM::iterator it = m_mapTCashItem.find(wShopItemID);
	if(it == m_mapTCashItem.end())
		return EC_NOERROR;

	if(bItem == INVALID_SLOT)
	{
		pInven->m_dEndTime += DWORD((*it).second->m_wUseTime) * DAY_ONE;
		dEndTime = pInven->m_dEndTime;
	}
	else
	{
		CTItem * pItem = pInven->FindTItem(bItem);
		if(pItem)
		{
			pItem->m_dEndTime += DWORD((*it).second->m_wUseTime) * DAY_ONE;
			dEndTime = pItem->m_dEndTime;
		}
	}

	SendDM_SAVEITEM_REQ(pPlayer);

	pPlayer->SendCS_STOPTHECLOCK_ACK(
		STC_SUCCESS,
		bInven,
		bItem,
		dEndTime);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_HEROSELECT_REQ(LPPACKETBUF pBUF)  
{
	int nRet;
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwGuildID;
	WORD wBattleZoneID;	
	CString strHeroName;
	__time64_t timeHero;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwGuildID
		>> wBattleZoneID
		>> strHeroName
		>> timeHero;

	DEFINE_QUERY(&m_db,CSPHeroSelect)
	query->m_dwCharID = dwCharID;
	query->m_dwGuildID = dwGuildID;
	query->m_wBattleZoneID = wBattleZoneID;
	lstrcpy(query->m_szHero,strHeroName);
	__TIMETODB(timeHero,query->m_timeHero);
	if(!query->Call() )
		nRet = HSR_INTERNAL;	
	else
		nRet = query->m_nRET;
	UNDEFINE_QUERY();

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_HEROSELECT_ACK)
		<< BYTE(nRet)
		<< dwCharID
		<< dwKey
		<< wBattleZoneID
		<< strHeroName
		<< timeHero;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_HEROSELECT_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet;	
	DWORD dwCharID;
	DWORD dwKey;
	WORD wBattleZoneID;
	CString strHeroName;
	__time64_t timeHero;

	pBUF->m_packet
		>> bRet
		>> dwCharID
		>> dwKey
		>> wBattleZoneID
		>> strHeroName
		>> timeHero;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	if(!strHeroName.IsEmpty())
		pPlayer->SendCS_HEROSELECT_ACK(bRet);

	if(!bRet)
		SendMW_HEROSELECT_ACK(wBattleZoneID, strHeroName, timeHero);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_RELAYCONNECT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	BYTE bRelayOn;

	pBUF->m_packet
		>> dwCharID
		>> bRelayOn;

	m_bRelayOn = bRelayOn;

	if(!bRelayOn)
		return EC_NOERROR;

	if(!dwCharID)
	{
		MAPPLAYER::iterator it;
		for(it=m_mapPLAYER.begin(); it!=m_mapPLAYER.end(); it++)
		{
			if((*it).second->m_bMain)
				(*it).second->SendCS_RELAYCONNECT_ACK(m_addrRelay.sin_addr.s_addr, m_addrRelay.sin_port);
		}
	}
	else
	{
		MAPPLAYER::iterator it = m_mapPLAYER.find(dwCharID);
		if(it != m_mapPLAYER.end())
			(*it).second->SendCS_RELAYCONNECT_ACK(m_addrRelay.sin_addr.s_addr, m_addrRelay.sin_port);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CHARMSG_REQ(LPPACKETBUF pBUF)
{
	CString strName;
	CString strMsg;

	pBUF->m_packet
		>> strName
		>> strMsg;

	CTPlayer* pPlayer = FindPlayer(strName);

	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_CHAT_ACK(CHAT_OPERATOR,0,GetSvrMsg(NAME_OPERATOR), BuildNetString(NAME_NULL, strMsg));

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_QUESTSENDPOST_REQ(LPPACKETBUF pBUF)
{
	DWORD dwQuestID;
	DWORD dwCharID;
	BYTE bItemCount;

	pBUF->m_packet
		>> dwQuestID
		>> dwCharID
		>> bItemCount;
	
	DEFINE_QUERY(&m_db, CSPQuestSendPost )
	query->m_dwQuestID = dwQuestID;
	query->m_dwCharID = dwCharID;
	for(BYTE i = 0 ; i < bItemCount; i++)
	{
		WrapItemQuery(&(pBUF->m_packet), query->m_item);	

		if(!query->Call())
			return EC_NOERROR;
	}

	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_ITEMSTATE_REQ(LPPACKETBUF pBUF)
{
	WORD wCount;
	WORD wItemID;
	DWORD dwID;
	BYTE bInitState;

	pBUF->m_packet
		>> dwID
		>> wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		pBUF->m_packet
			>> wItemID
			>> bInitState;

		LPTITEM pTITEM = FindTItem(wItemID);
		if(pTITEM)
		{
			pTITEM->m_bInitState = bInitState;
			pTITEM->m_wWeight = bInitState;
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_GAINPVPPOINT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwOwnerID;
	DWORD dwPoint;
	BYTE bEvent;
	BYTE bType;
	BYTE bGain;

	CString strName;
	BYTE bClass;
	BYTE bLevel;

	pBUF->m_packet
		>> dwOwnerID
		>> dwPoint
		>> bEvent
		>> bType
		>> bGain
		>> strName
		>> bClass
		>> bLevel;

	MAPPLAYER::iterator it = m_mapPLAYER.find(dwOwnerID);
	if(it==m_mapPLAYER.end())
		return EC_NOERROR;

	TRECORDSET set;
	BYTE bSet = FALSE;
	if(strName != NAME_NULL)
	{
		set.m_strName = strName;
		set.m_bClass = bClass;
		set.m_bLevel = bLevel;
		bSet = TRUE;
	}

	if(bGain)
		(*it).second->GainPvPoint(dwPoint, bEvent, bType, bSet ? &set : NULL);
	else
		(*it).second->UsePvPoint(dwPoint, bEvent, bType, bSet ? &set : NULL);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_GUILDPOINTLOG_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_GUILDPOINTLOG_ACK(pBUF);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_GUILDPOINTREWARD_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwRemainPoint;
	DWORD dwPoint;
	DWORD dwTargetID;
	CString strTarget;
	CString strMessage;

	pBUF->m_packet
		>> bRet
		>> dwCharID
		>> dwKey
		>> dwRemainPoint;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	if(!bRet)
	{
		pBUF->m_packet
			>> dwPoint
			>> dwTargetID
			>> strTarget
			>> strMessage;

		CString strTitle;
		strTitle.Format(GetSvrMsg(MSG_GUILDPOINT_TAKE), pPlayer->m_strGuildName, dwPoint);

		SendDM_POSTRECV_REQ(
			dwCharID, dwTargetID, pPlayer->m_strNAME, strTarget,
			BuildNetString(NAME_NULL, strTitle), BuildNetString(NAME_NULL, strMessage), POST_NORMAL,
			0,0,0, NULL);
	}

	pPlayer->SendCS_GUILDPOINTREWARD_ACK(
		bRet,
		dwRemainPoint);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_GUILDPVPRECORD_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;

	pBUF->m_packet
		>> dwCharID
		>> dwKey;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	pPlayer->SendCS_GUILDPVPRECORD_ACK(pBUF);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_MONSTERBUY_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwGuildID;
	WORD wNpcID;
	WORD wID;
	DWORD dwPrice;

	pBUF->m_packet
		>> bRet
		>> dwCharID
		>> dwKey
		>> dwGuildID
		>> wNpcID
		>> wID
		>> dwPrice;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
	{
		SendMW_GUILDMONEYRECOVER_ACK(
			dwGuildID,
			dwPrice);

		return EC_NOERROR;
	}

	if(bRet)
	{
		pPlayer->SendCS_MONSTERBUY_ACK(bRet);
		return EC_NOERROR;		
	}

	CTNpc * pNpc = FindTNpc(wNpcID);
	if(!pNpc)
	{
		SendMW_GUILDMONEYRECOVER_ACK(
			dwGuildID,
			dwPrice);

		pPlayer->SendCS_MONSTERBUY_ACK(MSB_INVALIDNPC);

		return EC_NOERROR;
	}

	if( !pNpc->CanTalk(pPlayer->m_bCountry, pPlayer->m_bAidCountry, pPlayer->HaveDisguiseBuff()))
	{
		SendMW_GUILDMONEYRECOVER_ACK(
			dwGuildID,
			dwPrice);

		pPlayer->SendCS_MONSTERBUY_ACK(MSB_INVALIDNPC);

		return EC_NOERROR;
	}

	LPTMONSTERSHOP pMon = pNpc->GetMon(wID);
	if(!pMon)
	{
		SendMW_GUILDMONEYRECOVER_ACK(
			dwGuildID,
			dwPrice);

		return EC_NOERROR;
	}

	MAPTMONSPAWN::iterator finder = pPlayer->m_pMAP->m_mapTMONSPAWN.find(pMon->m_pSpawn->m_wID);
	if(finder != pPlayer->m_pMAP->m_mapTMONSPAWN.end() &&
        !(*finder).second->m_vTMON.empty())
	{
		SendMW_GUILDMONEYRECOVER_ACK(
			dwGuildID,
			dwPrice);

		pPlayer->SendCS_MONSTERBUY_ACK(MSB_ALREADY);

		return EC_NOERROR;
	}

	if(pMon->m_wTowerID)
	{
		LPTGODTOWER pTower = pPlayer->m_pMAP->FindGodTower(pMon->m_wTowerID);
		if(!pTower ||
			!pTower->m_pGodBall ||
			pTower->m_pGodBall->m_bCamp != pPlayer->m_bCamp)
		{
			SendMW_GUILDMONEYRECOVER_ACK(
				dwGuildID,
				dwPrice);

			pPlayer->SendCS_MONSTERBUY_ACK(MSB_CAMPMISMATCH);

			return EC_NOERROR;
		}
	}

	pPlayer->m_pMAP->AddTimelimitedMon(pMon->m_pSpawn, pPlayer->m_bChannel, pPlayer->m_bCountry, RT_ETERNAL);
	pPlayer->SendCS_MONSTERBUY_ACK(MSB_SUCCESS);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CASTLEGUILDCHG_REQ(LPPACKETBUF pBUF)
{
	WORD wCastleID;
	DWORD dwDefGuildID;
	DWORD dwAtkGuildID;	
	__time64_t tTime;
	CString strDefGuild = NAME_NULL;
	CString strAtkGuild = NAME_NULL;

	pBUF->m_packet
		>> wCastleID
		>> dwDefGuildID
		>> strDefGuild
		>> dwAtkGuildID
		>> strAtkGuild
		>> tTime;

	LPTLOCAL pCastle = FindCastle(wCastleID);
	if(!pCastle)
		return EC_NOERROR;

	pCastle->m_timeNextDefend = tTime;
	pCastle->m_dwDefGuildID = dwDefGuildID;
	pCastle->m_strDefName = strDefGuild;
	pCastle->m_dwAtkGuildID = dwAtkGuildID;
	pCastle->m_strAtkName = strAtkGuild;

	return EC_NOERROR;
}
 
DWORD CTMapSvrModule::OnMW_EVENTUPDATE_REQ(LPPACKETBUF pBUF)
{
	BYTE bEventID;
	WORD wValue;

	pBUF->m_packet
		>> bEventID
		>> wValue;

	if(bEventID > EVENT_COUNT)
		return EC_NOERROR;
	
	m_wEventValue[bEventID] = wValue;
	
	EVENTINFO stEVENT;
	stEVENT.WrapPacketOut(&pBUF->m_packet);

	

	if(stEVENT.m_bID == EVENT_MONSPAWN)
	{
		WORD wSpawnID;
		BYTE bAction = wValue ? stEVENT.m_stMONEVENT.m_bStartAction : stEVENT.m_stMONEVENT.m_bEndAction;

		for(WORD i = 0; i < (WORD)stEVENT.m_stMONEVENT.m_vSpawnID.size();i++)
		{
			wSpawnID = stEVENT.m_stMONEVENT.m_vSpawnID[i];
			LPTMONSPAWN pTemp = FindTMonSpawn(wSpawnID);
			if(!pTemp)
				continue;

			MAPTCHANNEL::iterator itC;
			for(itC = m_mapTCHANNEL.begin(); itC != m_mapTCHANNEL.end(); itC++ )
			{
				BYTE bChannel = (BYTE)(*itC).first;
				CTMap *pMap = FindTMap( bChannel, 0, pTemp->m_wMapID);
				if(!pMap)
					continue;

				if( !bAction )
				{				
					pMap->AddTimelimitedMon(pTemp, bChannel, TCONTRY_N, RT_ETERNAL);
					continue;
				}

				CTMonSpawn* pSpawn = pMap->FindMonSpawn(wSpawnID);
				if(!pSpawn)
					continue;

				for(WORD j = 0; j < (WORD)pSpawn->m_vTMON.size(); j++)
				{
					CTMonster* pMon = pMap->FindMonster(pSpawn->m_vTMON[j]->m_dwID);
					if(!pMon)
						continue;

					if((enum AI_TRIGGER)bAction == AT_DEAD)
					{
						CTObjBase * bOBJ = (CTObjBase*) pMon;
						bOBJ->OnDie(0,OT_NONE,0); 
					}
					else
						pMon->OnEvent((enum AI_TRIGGER)bAction, 0, 0, 0, 0);
				}
			}
		}		
	}
	 
	if(stEVENT.m_bID == EVENT_MONREGEN)
	{
		LPEVENTINFO pEVENTINFO = NULL;
		WORD wNewSpawnID;
		BYTE bAction = wValue ? AT_AICOMPLETE : AT_DEAD;

		if(wValue)
		{
			if(!m_mapExtraSpawnID.size())
			{
				LogEvent("ExtraSpawnID is Empty");
				return EC_NOERROR;
			}
			
			for(WORD i = 0; i < (WORD)stEVENT.m_vMONREGEN.size(); i++)
			{
				wNewSpawnID = RegenDynamicMonster(
					stEVENT.m_vMONREGEN[i].m_wMapID,
					stEVENT.m_vMONREGEN[i].m_wMonID,
					stEVENT.m_vMONREGEN[i].m_fPosX,
					stEVENT.m_vMONREGEN[i].m_fPosY,
					stEVENT.m_vMONREGEN[i].m_fPosZ,
					stEVENT.m_vMONREGEN[i].m_dwDelay,
					1);

				if(!wNewSpawnID)
					continue;

				stEVENT.m_stMONEVENT.m_vSpawnID.push_back(wNewSpawnID);				
			}
			pEVENTINFO = &stEVENT;
		}
		else
		{
			pEVENTINFO = GetEventInfo(stEVENT.m_dwIndex);
		}

		WORD wSpawnID = 0;
		for(WORD i = 0; i < (WORD)pEVENTINFO->m_stMONEVENT.m_vSpawnID.size();i++)
		{
			wSpawnID = pEVENTINFO->m_stMONEVENT.m_vSpawnID[i];
			LPTMONSPAWN pTemp = FindTMonSpawn(wSpawnID);
			if(!pTemp)
				continue;

			MAPTCHANNEL::iterator itC;
			for(itC = m_mapTCHANNEL.begin(); itC != m_mapTCHANNEL.end(); itC++ )
			{
				BYTE bChannel = (BYTE)(*itC).first;
				CTMap *pMap = FindTMap( bChannel, 0, pTemp->m_wMapID);
				if(!pMap)
					continue;

				if(!bAction )
				{				
					pMap->AddTimelimitedMon(pTemp, bChannel, TCONTRY_N, RT_ETERNAL);
					continue;
				}

				CTMonSpawn* pSpawn = pMap->FindMonSpawn(wSpawnID);
				if(!pSpawn)
					continue;

				for(WORD j = 0; j < (WORD)pSpawn->m_vTMON.size(); j++)
				{
					CTMonster* pMon = pMap->FindMonster(pSpawn->m_vTMON[j]->m_dwID);
					if(!pMon)
						continue;

					if((enum AI_TRIGGER)bAction == AT_DEAD)
					{
						CTObjBase * bOBJ = (CTObjBase*) pMon;
						bOBJ->OnDie(0,OT_NONE,0);						

						LPTMONSPAWN pDelSpawn = FindTMonSpawn(wSpawnID);
						if(pDelSpawn)
						{
							pMap->DelMonSpawn(pDelSpawn);

							MAPTMONSPAWNTEMP::iterator finder = m_mapTMONSPAWN.find(pDelSpawn->m_wID);
							if( finder != m_mapTMONSPAWN.end() )
								m_mapTMONSPAWN.erase(finder);
							m_mapExtraSpawnID.insert(MAPWORD::value_type(pDelSpawn->m_wID,pDelSpawn->m_wID));
						}
					}
					else
						pMon->OnEvent((enum AI_TRIGGER)bAction, 0, 0, 0, 0);
				}
			}			
		}
	}

	 
	if(!wValue)
		DelEvent(stEVENT.m_dwIndex);
	else
        AddEvent(stEVENT);


	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_EVENTMSG_REQ(LPPACKETBUF pBUF)
{
	BYTE bEventID;
	BYTE bEventMsgType;
	CString strMsg;

	pBUF->m_packet
		>> bEventID
		>> bEventMsgType
		>> strMsg;
	
	strMsg.Left(ONE_KBYTE);

	MAPPLAYER::iterator it;
	for( it = m_mapPLAYER.begin(); it != m_mapPLAYER.end(); it++)
	{
		if((*it).second->m_bMain)
			(*it).second->SendCS_CHAT_ACK( CHAT_WORLD, 0, GetSvrMsg(NAME_OPERATOR), BuildNetString(NAME_NULL, strMsg));		
	}
	
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_EVENTMSGLOTTERY_REQ(LPPACKETBUF pBUF)
{
	MAPPLAYER::iterator it;
	for( it = m_mapPLAYER.begin(); it != m_mapPLAYER.end(); it++)
	{
		if((*it).second->m_bMain)
			(*it).second->SendCS_SYSTEMMSG_ACK(SM_EVENT_LOTTERY, &(pBUF->m_packet));
	}
	
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CASHSHOPSTOP_REQ(LPPACKETBUF pBUF)
{
	BYTE bStop;
	BYTE bSendPlayer;

	pBUF->m_packet
		>> bStop
		>> bSendPlayer;

	m_bCashShopStop = bStop;
	
	if(bSendPlayer)
	{
		MAPPLAYER::iterator it;
		for(it = m_mapPLAYER.begin(); it != m_mapPLAYER.end(); it++)
			(*it).second->SendCS_CASHSHOPSTOP_ACK(TRUE);
	}
	
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CASHITEMSALE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwIndex;
	WORD wValue;
	WORD wCount;
	WORD wID;
	BYTE bSaleValue;
	

	pBUF->m_packet
		>> dwIndex
		>> wValue
		>> wCount;

	
	MAPTCASHITEM::iterator it;
	for(WORD i = 0; i < wCount; i++)
	{
		pBUF->m_packet
			>> wID
			>> bSaleValue;		

		if(wID == 0 )
		{			
			for(it = m_mapTCashItem.begin(); it != m_mapTCashItem.end(); it++)
			{					
				(*it).second->m_dwMoney = (*it).second->m_dwOriMoney * (100 - bSaleValue) / 100 ;
				(*it).second->m_bSaleValue = bSaleValue;
			}

		}
		else
		{		
			it = m_mapTCashItem.find(wID);
			if( it != m_mapTCashItem.end() )
			{			
				(*it).second->m_dwMoney = (*it).second->m_dwOriMoney * (100 - bSaleValue) / 100 ;
				(*it).second->m_bSaleValue = bSaleValue;
			}
		}		
	}


	m_bCashShopStop = TRUE;


	SendMW_CASHITEMSALE_ACK(dwIndex,wValue,TRUE);
	
	return EC_NOERROR;
}

//Apex
DWORD CTMapSvrModule::OnMW_APEXDATA_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	int nLen;
	char BufRecv[MAX_APEXPACKET];

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> nLen;

	pBUF->m_packet.DetachBinary((LPVOID)BufRecv);

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
		pPlayer->SendCS_APEXDATA_ACK(BufRecv, nLen);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_AUCTIONREG_REQ(LPPACKETBUF pBUF)
{
	DWORD dwSeller;
	DWORD dwKEY;
	CString strSeller;
	WORD wNpcID;
	WORD wHour;
	BYTE bInvenID;;
	__int64 ldwDirectPrice;
	__int64 ldwStartPrice;	
	DWORD dwAuctionID = 0;
	DWORD dwCode;
	BYTE bResult;
	CTItem * pItem = NULL;

	pBUF->m_packet
		>> dwSeller
		>> dwKEY
		>> strSeller
		>> wNpcID
		>> wHour
		>> bInvenID
		>> ldwDirectPrice
		>> ldwStartPrice
		>> dwCode;

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_AUCTIONREG_ACK)
		<< dwSeller
		<< dwKEY
		<< strSeller
		<< wNpcID
		<< wHour
		<< bInvenID
		<< ldwDirectPrice
		<< ldwStartPrice
		<< dwCode;
	
	DEFINE_QUERY(&m_db, CSPSaveItemDirect)
	query->m_bStorageType = STORAGE_INVEN;
	query->m_dwStorageID = bInvenID;
	query->m_bOwnerType = TOWNER_CHAR;
	query->m_dwOwnerID = dwSeller;
	WrapItemQuery(&(pBUF->m_packet), query->m_item);
	
	pItem = CreateItem(query->m_item);
	if(pItem)
		pItem->m_dlID = query->m_item.m_dlID;

	if(!pItem || !query->Call())
		bResult = AR_FAIL;
	else
		bResult = AR_SUCCESS;

	UNDEFINE_QUERY()
	

	if(bResult)
	{
		pMsg->m_packet
			<< bResult
			<< DWORD(0)
			<< __int64(0)
			<< __int64(0);
		
		if(pItem)
			pItem->WrapPacket( &(pMsg->m_packet));
		
		SayToBATCH(pMsg);

		if(pItem)
			delete pItem;

		return EC_NOERROR;
	}
	
	DEFINE_QUERY( &m_db,CSPAuctionReg)	
	query->m_wNpcID = wNpcID;
	query->m_wHour = wHour;
	query->m_dwSeller = dwSeller;
	query->m_dlDirectPrice = ldwDirectPrice;
	query->m_dlStartPrice = ldwStartPrice;	
	query->m_dlID = pItem->m_dlID;

	if(!query->Call() )
		bResult = AR_FAIL;
	else
	{
		bResult = query->m_nRET;
		dwAuctionID = query->m_dwAuctionID;		
	}

	__time64_t dStart = __DBTOTIME(query->m_DateStart);
	__time64_t dEnd = __DBTOTIME(query->m_DateEnd);

	pMsg->m_packet
		<< bResult
		<< dwAuctionID	
		<< dStart
		<< dEnd;

	if(pItem)
		pItem->WrapPacket( &(pMsg->m_packet));

	UNDEFINE_QUERY()

	SayToBATCH(pMsg);

	if(pItem)
		delete pItem;

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_AUCTIONREG_ACK(LPPACKETBUF pBUF)
{
	BYTE bResult;
	DWORD dwSeller;
	DWORD dwKEY;
	CString strName;
	BYTE bInvenID;
	DWORD dwAuctionID;
	WORD wNpcID;
	WORD wHour;
	CString strSeller;
	__int64 ldwDirectPrice;
	__int64 ldwStartPrice;
	__time64_t dStart;
	__time64_t dEnd;
	DWORD dwCode;

	pBUF->m_packet
		>> dwSeller
		>> dwKEY
		>> strName
		>> wNpcID
		>> wHour
		>> bInvenID
		>> ldwDirectPrice
		>> ldwStartPrice
		>> dwCode
		>> bResult
		>> dwAuctionID
		>> dStart
		>> dEnd;	

	CTItem* pAuctionItem = CreateItem(&(pBUF->m_packet));
	CTPlayer* pPlayer = FindPlayer(dwSeller,dwKEY);

	//돌려주기
	if(bResult)
	{
		DWORD dwGold,dwSilver,dwCooper;
		__int64 ldwRegFee = __int64(ldwDirectPrice * AUCTIONREGFEE);
		CalcMoney(ldwRegFee, dwGold, dwSilver, dwCooper);

		CTTextLinker* pTextLinker = CTTextLinker::GetInstance();
		CTTextLinkData* pData;

		DWORD dwID = pTextLinker->NewTextLinkData(&pData);
		pData->PackItem(pAuctionItem);

		VDWORD vItemID;
		vItemID.push_back(dwID);

		CString strMSG;
		strMSG = GetSvrMsg(MSG_AUCTION_REGCANCEL_BODY);		
		strMSG = pTextLinker->MakeItemNetText(strMSG, vItemID);		

		SendDM_POSTRECV_REQ(0,dwSeller,GetSvrMsg(MSG_AUCTION_OPERATOR),strName,
			BuildNetString(NAME_NULL, GetSvrMsg(MSG_AUCTION_REGCANCEL)),strMSG,POST_PACKATE,
			dwGold,dwSilver,dwCooper,pAuctionItem);

		if(pPlayer)
		{
			pPlayer->SendCS_AUCTIONREG_ACK(bResult,dwAuctionID);
			pPlayer->AuctionRegListSave(!bResult,dwAuctionID);
		}

		if(pAuctionItem)
			delete pAuctionItem;

		return EC_NOERROR;
	}

	if(pPlayer)
	{
		pPlayer->SendCS_AUCTIONREG_ACK(AR_SUCCESS,dwAuctionID);	
		pPlayer->AuctionRegListSave(!bResult,dwAuctionID);
	}	

	if(!pAuctionItem)
		return EC_NOERROR;

	CTNpc* pNpc = FindTNpc(wNpcID);
	if(pNpc)
	{
		LPTAUCTION pAuction = new TAUCTION();
		pAuction->m_bBidCount = 0;
		pAuction->m_dwAuctionID = dwAuctionID;
		pAuction->m_dwSeller = dwSeller;
		pAuction->m_ldwDirectPrice = ldwDirectPrice;
		pAuction->m_ldwStartPrice = ldwStartPrice;
		pAuction->m_ldwBidPrice = ldwStartPrice;
		pAuction->m_strSeller = strName;
		pAuction->m_wNpcID = wNpcID;
		pAuction->m_pItem = pAuctionItem;
		pAuction->m_dStart = dStart;
		pAuction->m_dEnd = dEnd;
		pAuction->m_dwCode = dwCode;		
		
		if(pNpc->AddAuction(dwAuctionID,pAuction))
            SendSM_AUCTIONCMD_REQ(wNpcID,dwAuctionID,dEnd);

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogAuctionReg(LOGMAP_AUCTIONREG,pPlayer,dwAuctionID,ldwStartPrice,ldwDirectPrice,pAuctionItem,dwSeller,strName);
#endif
	}
	
	return EC_NOERROR;
}


DWORD CTMapSvrModule::OnDM_AUCTIONEND_REQ(LPPACKETBUF pBUF)
{
	WORD wNpcID;
	DWORD dwAuctionID;
	__time64_t dEndTime;
	BYTE bResult;

	pBUF->m_packet
		>> wNpcID
		>> dwAuctionID
		>> dEndTime;

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_AUCTIONEND_ACK);

	DEFINE_QUERY(&m_db, CSPAuctionEnd)
	query->m_wNpcID = wNpcID;
	query->m_dwAuctionID = dwAuctionID;

	if(!query->Call() )
		bResult = 1;
	else
		bResult = query->m_nRET;

	pMsg->m_packet
		<< bResult
		<< wNpcID
		<< dwAuctionID
		<< query->m_dwSeller
		<< query->m_dwBuyer
		<< query->m_szSeller
		<< query->m_szBuyer
		<< query->m_ldwPrice;
	
	UNDEFINE_QUERY()

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_AUCTIONEND_ACK(LPPACKETBUF pBUF)
{
	BYTE bResult;
	WORD wNpcID;
	DWORD dwAuctionID;
	DWORD dwSeller;
	DWORD dwBuyer;
	CString strSeller;
	CString strBuyer;
	__int64 ldwPrice;
	CTItem* pItem = NULL;
	DWORD dwCode = 0;

	pBUF->m_packet
		>> bResult
		>> wNpcID
		>> dwAuctionID
		>> dwSeller
		>> dwBuyer
		>> strSeller
		>> strBuyer
		>> ldwPrice;

	if(bResult)
		return EC_NOERROR;

	CTNpc* pNpc = _AtlModule.FindTNpc(wNpcID);
	if(!pNpc)
		return EC_NOERROR;

	MAPTAUCTION::iterator itA = pNpc->m_mapTAuction.find(dwAuctionID);
	if( itA != pNpc->m_mapTAuction.end())
	{
		pItem = (*itA).second->m_pItem;
		dwCode = (*itA).second->m_dwCode;
	}
	
	if(!dwBuyer) //유찰
	{
		CTTextLinker* pTextLinker = CTTextLinker::GetInstance();
		CTTextLinkData* pData;

		DWORD dwID = pTextLinker->NewTextLinkData(&pData);
		pData->PackItem(pItem);

		VDWORD vItemID;
		vItemID.push_back(dwID);

		CString strMSG;
		strMSG = GetSvrMsg(MSG_AUCTION_FAIL_BODY);		
		strMSG = pTextLinker->MakeItemNetText(strMSG, vItemID);	

		SendDM_POSTRECV_REQ(0,dwSeller,GetSvrMsg(MSG_AUCTION_OPERATOR),strSeller,
			BuildNetString(NAME_NULL, GetSvrMsg(MSG_AUCTION_FAIL)), strMSG, POST_PACKATE,
			0,0,0, pItem);

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogAuctionBid(LOGMAP_AUCTIONBIDFAIL,NULL,dwAuctionID,0,pItem,pItem->m_bCount,dwSeller,strSeller);
#endif
	}
	else //낙찰
	{		
		CTTextLinker* pTextLinker = CTTextLinker::GetInstance();
		CTTextLinkData* pData;

		DWORD dwID = pTextLinker->NewTextLinkData(&pData);
		pData->PackItem(pItem);

		VDWORD vItemID;
		vItemID.push_back(dwID);

		CString strMSG;
		CString strMSGPrice;
		strMSG = GetSvrMsg(MSG_AUCTION_BIDSUCCESS_BODY);
		strMSG = pTextLinker->MakeItemNetText(strMSG, vItemID);		
		
		strMSGPrice = GetSvrMsg(MSG_AUCTION_PRICE_BODY);
		strMSGPrice = pTextLinker->MakeItemNetText(strMSGPrice, vItemID);	

		DWORD dwGold,dwSilver,dwCooper;
		CalcMoney(ldwPrice, dwGold, dwSilver, dwCooper);

		// 등록자에게 돈보내기
		SendDM_POSTRECV_REQ(0,dwSeller,GetSvrMsg(MSG_AUCTION_OPERATOR),strSeller,
			BuildNetString(NAME_NULL, GetSvrMsg(MSG_AUCTION_PRICE)), strMSGPrice, POST_NORMAL,
			dwGold,dwSilver,dwCooper, NULL);

		// 낙찰자에게 아이템 보내기
		SendDM_POSTRECV_REQ(0,dwBuyer,GetSvrMsg(MSG_AUCTION_OPERATOR),strBuyer,
			BuildNetString(NAME_NULL, GetSvrMsg(MSG_AUCTION_BIDSUCCESS)), strMSG, POST_PACKATE,
			0,0,0, pItem);

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogAuctionBid(LOGMAP_AUCTIONSUCCESSMONEY,NULL,dwAuctionID,ldwPrice,pItem,pItem->m_bCount,dwSeller,strSeller);
		m_pUdpSocket->LogAuctionBid(LOGMAP_AUCTIONBIDSUCCESS,NULL,dwAuctionID,ldwPrice,pItem,pItem->m_bCount,dwBuyer,strBuyer);
#endif
	}

	pNpc->DelAuction(dwAuctionID,dwCode);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_AUCTIONREGCANCEL_REQ(LPPACKETBUF pBUF)
{
	BYTE bResult;
	WORD wNpcID;
	DWORD dwAuctionID;
	DWORD dwCharID;
	DWORD dwKEY;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> wNpcID
		>> dwAuctionID;		

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_AUCTIONREGCANCEL_ACK);

	DEFINE_QUERY( &m_db, CSPAuctionRegCancel)
	query->m_wNpcID = wNpcID;
	query->m_dwAuctionID = dwAuctionID;

	if(!query->Call() )
		bResult = AR_FAIL;
	else
		bResult = query->m_nRET;
		
	pMsg->m_packet	
		<< bResult
		<< dwCharID
		<< dwKEY
		<< wNpcID
		<< dwAuctionID
		<< query->m_szSeller
		<< query->m_szBidder
		<< query->m_dwBidder
		<< query->m_ldwBidPrice;

	UNDEFINE_QUERY()

	SayToBATCH(pMsg);	

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_AUCTIONREGCANCEL_ACK(LPPACKETBUF pBUF)
{
	BYTE bResult;
	DWORD dwCharID;
	DWORD dwKEY;
	WORD wNpcID;
	DWORD dwAuctionID;
	DWORD dwBidder;
	__int64 ldwBidPrice;
	CString strSeller;
	CString strBidder;
	CTItem * pItem = NULL;
	DWORD dwCode = 0;
	__int64 ldwStartPrice = 0;
	__int64 ldwDirectPrice = 0;

	pBUF->m_packet
		>> bResult
		>> dwCharID
		>> dwKEY
		>> wNpcID
		>> dwAuctionID
		>> strSeller
		>> strBidder
		>> dwBidder
		>> ldwBidPrice;	

	CTPlayer* pChar = FindPlayer(dwCharID,dwKEY);
	if(pChar)
		pChar->SendCS_AUCTIONREGCANCEL_ACK(bResult);

	CTNpc* pNpc = FindTNpc(wNpcID);
	if(pNpc && !bResult)
	{
		MAPTAUCTION::iterator itA =  pNpc->m_mapTAuction.find(dwAuctionID);
		if(itA != pNpc->m_mapTAuction.end())
		{
			ldwStartPrice = (*itA).second->m_ldwStartPrice;
			ldwDirectPrice = (*itA).second->m_ldwDirectPrice;
			pItem = (*itA).second->m_pItem;
			dwCode = (*itA).second->m_dwCode;			
		}
	}
	else
		return EC_NOERROR;

	CTTextLinker* pTextLinker = CTTextLinker::GetInstance();
	CTTextLinkData* pData;

	DWORD dwID = pTextLinker->NewTextLinkData(&pData);
	pData->PackItem(pItem);

	VDWORD vItemID;
	vItemID.push_back(dwID);

	CString strMSG;
	CString strMSG2;
	strMSG = GetSvrMsg(MSG_AUCTION_REGCANCEL_BODY);	
	strMSG = pTextLinker->MakeItemNetText(strMSG, vItemID);

	strMSG2 = GetSvrMsg(MSG_AUCTION_DROP_BODY);	
	strMSG2 = pTextLinker->MakeItemNetText(strMSG2, vItemID);	

	// 등록자에게 아이템 보내기
	SendDM_POSTRECV_REQ(0,dwCharID,GetSvrMsg(MSG_AUCTION_OPERATOR), strSeller,
		BuildNetString(NAME_NULL, GetSvrMsg(MSG_AUCTION_REGCANCEL)), strMSG, POST_PACKATE,
        0,0,0, pItem);	

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogAuctionReg(LOGMAP_AUCTIONREGCANCEL,pChar,dwAuctionID,ldwStartPrice,ldwDirectPrice,pItem,dwCharID,strSeller);
#endif

	pNpc->DelAuction(dwAuctionID,dwCode);

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(SM_AUCTIONCMDDEL_REQ)
		<< wNpcID
		<< dwAuctionID;

	SayToLOG(pMsg);

	if(!dwBidder)
		return EC_NOERROR;
	
	
	// 입찰자에게 돈 보내기
	DWORD dwGold,dwSilver,dwCooper;
	CalcMoney(ldwBidPrice, dwGold, dwSilver, dwCooper);

	SendDM_POSTRECV_REQ(0,dwBidder,GetSvrMsg(MSG_AUCTION_OPERATOR),strBidder,
		BuildNetString(NAME_NULL, GetSvrMsg(MSG_AUCTION_REGCANCEL_DROP)), strMSG2, POST_NORMAL,
		dwGold,dwSilver,dwCooper, NULL);

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogAuctionBid(LOGMAP_AUCTIONBIDDROP,NULL,dwAuctionID,ldwBidPrice,NULL,0,dwBidder,strBidder);
#endif

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_AUCTIONBID_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;
	CString strName;
	DWORD dwUserID;
	WORD wNpcID;
	DWORD dwAuctionID;	
	__int64 ldwBidPrice;
	BYTE	bResult;

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strName
		>> dwUserID
		>> wNpcID
		>> dwAuctionID
		>> ldwBidPrice;

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_AUCTIONBID_ACK);

	DEFINE_QUERY(&m_db, CSPAuctionBid)
	query->m_wNpcID = wNpcID;
	query->m_dwAuctionID =dwAuctionID;
	query->m_dwCharID = dwCharID;
	query->m_ldwBidPrice = ldwBidPrice;

	if(!query->Call())
		bResult = ABR_FAIL;
	else
		bResult = query->m_nRET;

	pMsg->m_packet
		<< bResult
		<< dwCharID
		<< dwKEY
		<< strName
		<< dwUserID
		<< wNpcID
		<< dwAuctionID
		<< ldwBidPrice
		<< query->m_ldwDropBidPrice
		<< query->m_dwDropBidder
		<< query->m_szDropBidder
		<< __DBTOTIME(query->m_dBid);

	UNDEFINE_QUERY()

	SayToBATCH(pMsg);
		
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_AUCTIONBID_ACK(LPPACKETBUF pBUF)
{
	BYTE bResult;
	DWORD dwCharID;
	DWORD dwKEY;
	CString strName;
	DWORD dwUserID;
	WORD wNpcID;
	DWORD dwAuctionID;
	__int64 ldwBidPrice;
	__int64 ldwDropBidPrice;
	DWORD dwDropBidder;
	CString szDropBidder;
	__time64_t dBidTime;

	pBUF->m_packet
		>> bResult
		>> dwCharID
		>> dwKEY
		>> strName
		>> dwUserID
		>> wNpcID
		>> dwAuctionID
		>> ldwBidPrice
		>> ldwDropBidPrice
		>> dwDropBidder
		>> szDropBidder
		>> dBidTime;

	CTPlayer* pPlayer = FindPlayer(dwCharID,dwKEY);
	if(pPlayer)
		pPlayer->SendCS_AUCTIONBID_ACK(bResult);	
	
	CTItem* pItem = NULL;
	CTNpc* pNpc = FindTNpc(wNpcID);
	if(pNpc)
	{
		MAPTAUCTION::iterator it = pNpc->m_mapTAuction.find(dwAuctionID);
		if( it != pNpc->m_mapTAuction.end())
		{
			pItem = (*it).second->m_pItem;

			if(!bResult)
			{
				(*it).second->m_dBid = dBidTime;
				(*it).second->m_ldwBidPrice = ldwBidPrice;
				(*it).second->m_bBidCount++;
				(*it).second->m_dwBidder = dwUserID;
			}
		}
	}

	if(bResult) //입찰 실패
	{
		DWORD dwGold,dwSilver,dwCooper;
		CalcMoney(ldwBidPrice, dwGold, dwSilver, dwCooper);

		CTTextLinker* pTextLinker = CTTextLinker::GetInstance();
		CTTextLinkData* pData;

		DWORD dwID = pTextLinker->NewTextLinkData(&pData);
		pData->PackItem(pItem);

		VDWORD vItemID;
		vItemID.push_back(dwID);

		CString strMSG;
		strMSG = GetSvrMsg(MSG_AUCTION_BIDFAIL_BODY);		
		strMSG = pTextLinker->MakeItemNetText(strMSG, vItemID);	

		SendDM_POSTRECV_REQ(0,dwCharID,GetSvrMsg(MSG_AUCTION_OPERATOR),strName,
			BuildNetString(NAME_NULL, GetSvrMsg(MSG_AUCTION_BIDFAIL)), strMSG, POST_NORMAL,
			dwGold,dwSilver,dwCooper, NULL);

		return EC_NOERROR;
	}

	if(pPlayer)
	{
		pPlayer->AuctionBidListSave(TRUE,dwAuctionID);
		pPlayer->AuctionInterestSave(TRUE,dwAuctionID);
	}
		
	CTPlayer* pDropbidder = FindPlayer(szDropBidder);
	if(pDropbidder)
		pDropbidder->AuctionBidListSave(FALSE,dwAuctionID);

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogAuctionBid(LOGMAP_AUCTIONBID,pPlayer,dwAuctionID,ldwBidPrice,pItem,pItem->m_bCount,dwCharID,strName);	 	
#endif
	
	if(!dwDropBidder)
		return EC_NOERROR;

	CTTextLinker* pTextLinker = CTTextLinker::GetInstance();
	CTTextLinkData* pData;

	DWORD dwID = pTextLinker->NewTextLinkData(&pData);
	pData->PackItem(pItem);

	VDWORD vItemID;
	vItemID.push_back(dwID);

	CString strOtherBidderMsg;
	strOtherBidderMsg = GetSvrMsg(MSG_AUCTION_OTHERBIDDER_BODY);
	strOtherBidderMsg = pTextLinker->MakeItemNetText(strOtherBidderMsg, vItemID);	
	
	//입찰성공.이전 입찰자에게 돈 돌려주기
	DWORD dwGold,dwSilver,dwCooper;
	CalcMoney(ldwDropBidPrice, dwGold, dwSilver, dwCooper);

	SendDM_POSTRECV_REQ(0,dwDropBidder,GetSvrMsg(MSG_AUCTION_OPERATOR),szDropBidder,
		BuildNetString(NAME_NULL, GetSvrMsg(MSG_AUCTION_OTHERBIDDER)), strOtherBidderMsg, POST_NORMAL,
		dwGold,dwSilver,dwCooper, NULL);

#ifdef DEF_UDPLOG
	m_pUdpSocket->LogAuctionBid(LOGMAP_AUCTIONBIDDROP,pDropbidder,dwAuctionID,ldwDropBidPrice,pItem,pItem->m_bCount,dwDropBidder,szDropBidder);  
#endif

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_AUCTIONBUYDIRECT_REQ(LPPACKETBUF pBUF)
{
	BYTE bResult;
	DWORD dwCharID;
	DWORD dwKEY;
	CString strBuyer;
	WORD wNpcID;
	DWORD dwAuctionID;
	BYTE bCount;
	__int64 ldwBidPrice;
	

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> strBuyer
		>> wNpcID
		>> dwAuctionID
		>> bCount
		>> ldwBidPrice;

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_AUCTIONBUYDIRECT_ACK);


	DEFINE_QUERY(&m_db,CSPAuctionBuyDirect)
	query->m_dwCharID = dwCharID;
	query->m_wNpcID = wNpcID;
	query->m_dwAuctionID = dwAuctionID;	
	query->m_bBuyCount = bCount;
	query->m_ldwBidPrice = ldwBidPrice;

	if(!query->Call() )
		bResult = ABR_FAIL;
	else
		bResult = query->m_nRET;

	pMsg->m_packet
		<< bResult		
		<< dwCharID
		<< dwKEY
		<< strBuyer
		<< wNpcID
		<< dwAuctionID
		<< bCount
		<< ldwBidPrice
		<< query->m_dwSeller
		<< query->m_szSeller
		<< query->m_dwDropBidder
		<< query->m_szDropBidder	
		<< query->m_ldwDropBidPrice;

    UNDEFINE_QUERY()

	SayToBATCH(pMsg);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_AUCTIONBUYDIRECT_ACK(LPPACKETBUF pBUF)
{
	BYTE bResult;
	DWORD dwCharID;
	DWORD dwKEY;
	CString strBuyer;
	WORD wNpcID;
	DWORD dwAuctionID;
	BYTE bCount;
	__int64 ldwBidPrice;	
	DWORD dwSeller;
	CString strSeller;
	DWORD dwDropBidder;
	CString strDropBidder;
	__int64 ldwDropBidPrice;
	CTItem * pItem = NULL;
	DWORD dwGold,dwSilver,dwCooper;
	__int64 ldwTotalBidPrice;
	BYTE bDelAuctionBuf = FALSE;

	pBUF->m_packet
		>> bResult
		>> dwCharID
		>> dwKEY
		>> strBuyer
		>> wNpcID
		>> dwAuctionID
		>> bCount
		>> ldwBidPrice
		>> dwSeller		
		>> strSeller
		>> dwDropBidder
		>> strDropBidder		
		>> ldwDropBidPrice;

	CTPlayer* pPlayer = FindPlayer(dwCharID,dwKEY);
	if(pPlayer)
		pPlayer->SendCS_AUCTIONBUYDIRECT_ACK(bResult);

	if(bResult) // 실패.돌려주기
	{
		ldwTotalBidPrice = ldwBidPrice * bCount;
		CalcMoney(ldwTotalBidPrice, dwGold, dwSilver, dwCooper);

		SendDM_POSTRECV_REQ(0,dwCharID,GetSvrMsg(MSG_AUCTION_OPERATOR),strBuyer,
			BuildNetString(NAME_NULL, GetSvrMsg(MSG_AUCTION_BIDFAIL)), NAME_NULL, POST_NORMAL,
			dwGold,dwSilver,dwCooper, NULL);

		return EC_NOERROR;
	}

	pItem = new CTItem();

	CTNpc* pNpc = FindTNpc(wNpcID);
	if(pNpc)
	{
		MAPTAUCTION::iterator itA = pNpc->m_mapTAuction.find(dwAuctionID);
		if(itA != pNpc->m_mapTAuction.end())
		{
			pItem->Copy( (*itA).second->m_pItem,(*itA).second->m_pItem->m_bCount - bCount );
			pItem->m_bCount = bCount;

			if((*itA).second->m_pItem->m_bCount == bCount)
			{
				pNpc->DelAuction(dwAuctionID,(*itA).second->m_dwCode);
				bDelAuctionBuf = TRUE;
			}
			else
			{
				(*itA).second->m_pItem->m_bCount -= bCount;
				(*itA).second->m_ldwDirectPrice -= (ldwBidPrice * bCount);
			}
		}
	}
	
	CTTextLinker* pTextLinker = CTTextLinker::GetInstance();
	CTTextLinkData* pData;

	DWORD dwID = pTextLinker->NewTextLinkData(&pData);
	pData->PackItem(pItem);

	VDWORD vItemID;
	vItemID.push_back(dwID);

	CString strPriceMSG;
	CString strSuccessMSG;	
	CString strOtherBuyMsg;

	strPriceMSG = GetSvrMsg(MSG_AUCTION_PRICE_BODY);	
	strSuccessMSG = GetSvrMsg(MSG_AUCTION_BIDSUCCESS_BODY);	
	strOtherBuyMsg = GetSvrMsg(MSG_AUCTION_OTHERBUY_BODY);	

	strPriceMSG = pTextLinker->MakeItemNetText(strPriceMSG, vItemID);
	strSuccessMSG = pTextLinker->MakeItemNetText(strSuccessMSG, vItemID);
	strOtherBuyMsg = pTextLinker->MakeItemNetText(strOtherBuyMsg, vItemID);
	
	
	ldwTotalBidPrice = ldwBidPrice * bCount - __int64(ldwBidPrice * bCount * AUCTIONSUCCESSFEE);
	CalcMoney(ldwTotalBidPrice, dwGold, dwSilver, dwCooper);

	// 등록자에게 돈 보내기
	SendDM_POSTRECV_REQ(0,dwSeller,GetSvrMsg(MSG_AUCTION_OPERATOR),strSeller,
		BuildNetString(NAME_NULL, GetSvrMsg(MSG_AUCTION_PRICE)), strPriceMSG, POST_NORMAL,
		dwGold,dwSilver,dwCooper, NULL);

	// 낙찰자에게 물픔 보내기
	SendDM_POSTRECV_REQ(0,dwCharID,GetSvrMsg(MSG_AUCTION_OPERATOR),strBuyer,
		BuildNetString(NAME_NULL, GetSvrMsg(MSG_AUCTION_BIDSUCCESS)), strSuccessMSG, POST_PACKATE,
		0,0,0, pItem);

#ifdef DEF_UDPLOG
	m_pUdpSocket->LogAuctionBid(LOGMAP_AUCTIONBUYDIRECT,NULL,dwAuctionID,ldwBidPrice,pItem,bCount,dwCharID,strBuyer);
	m_pUdpSocket->LogAuctionBid(LOGMAP_AUCTIONSUCCESSMONEY,NULL,dwAuctionID,ldwBidPrice,pItem,bCount,dwSeller,strSeller);	
#endif

	// 탈락한 입찰자에게 돈 보내기
	if(strDropBidder != NAME_NULL)
	{
		CTPlayer* pDropBidder = FindPlayer(strDropBidder);

		CalcMoney(ldwDropBidPrice, dwGold, dwSilver, dwCooper);

		SendDM_POSTRECV_REQ(0,dwDropBidder,GetSvrMsg(MSG_AUCTION_OPERATOR),strDropBidder,
			BuildNetString(NAME_NULL, GetSvrMsg(MSG_AUCTION_OTHERWIN)), strOtherBuyMsg, POST_NORMAL,
			dwGold,dwSilver,dwCooper, NULL);
		
#ifdef DEF_UDPLOG
		m_pUdpSocket->LogAuctionBid(LOGMAP_AUCTIONBIDDROP,pDropBidder,dwAuctionID,ldwDropBidPrice,pItem,pItem->m_bCount,dwDropBidder,strDropBidder);  
#endif
	}

	if(pItem)
		delete pItem;

	if(bDelAuctionBuf)
	{
		LPPACKETBUF pMsg = new PACKETBUF();
		pMsg->m_packet.SetID(SM_AUCTIONCMDDEL_REQ)
			<< wNpcID
			<< dwAuctionID;

		SayToLOG(pMsg);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_WORLDPOSTSEND_REQ(LPPACKETBUF pBUF)
{
	BYTE bType;

	pBUF->m_packet
		>> bType;

	switch(bType)
	{
	case WPT_TACTICSKICK:
	case WPT_TACTICSEND:
		{
			DWORD dwValue;
			DWORD dwSenderID;
			CString strSender;
			DWORD dwRecvID;
			CString strRecver;
			INT64 dlMoney;

			pBUF->m_packet
				>> dwValue
				>> dwSenderID
				>> strSender
				>> dwRecvID
				>> strRecver
				>> dlMoney;

			CString strTitle;
			CString strMessage;

			if(bType == WPT_TACTICSKICK)
			{
				strTitle = GetSvrMsg(MSG_TACTICSKICK_TITLE);
				strMessage.Format(GetSvrMsg(MSG_TACTICSKICK_MESSAGE), dwValue);
			}
			else
			{
				strTitle = GetSvrMsg(MSG_TACTICSEND_TITLE);
				strMessage.Format(GetSvrMsg(MSG_TACTICSEND_MESSAGE), dwValue);		
			}

			DWORD dwGold, dwSilver, dwCooper;
			CalcMoney(dlMoney, dwGold, dwSilver, dwCooper);

			SendDM_POSTRECV_REQ(
				dwSenderID,
				dwRecvID,
				strSender,
				strRecver,
				BuildNetString(NAME_NULL, strTitle),
				BuildNetString(NAME_NULL, strMessage),
				POST_NORMAL,
				dwGold, dwSilver, dwCooper, NULL);
			break;
		}
	case WPT_LOTITEM:
		{
			DWORD	dwRecvID;
			CString	strRecver;
			CString	strTitle;
			CString	strMessage;
			WORD	wItemID;
			BYTE	bItemNum;
			WORD	wUseTime;

			pBUF->m_packet
				>> dwRecvID
				>> strRecver
				>> strTitle
				>> strMessage
				>> wItemID
				>> bItemNum
				>> wUseTime;

			//	아이템 생성
			LPTITEM pTITEM = FindTItem(wItemID);
			if(!pTITEM)
				return EC_NOERROR;

			CTItem * pNew = new CTItem();
			pNew->m_dlID = GenItemID();
			pNew->m_bCount = bItemNum;
			pNew->m_wItemID = wItemID;
			pNew->m_pTITEM = pTITEM;
			pNew->SetDuration(FALSE);
			if(wUseTime)
			{
				CTime time(_AtlModule.m_timeCurrent);
				time += CTimeSpan(0,wUseTime,0,0);
				pNew->m_dEndTime = time.GetTime();
			}
				//pNew->SetEndTime((DWORD)wUseTime);

			SetItemAttr(pNew, 0);

			SendDM_POSTRECV_REQ(
				0, dwRecvID, GetSvrMsg(NAME_OPERATOR), strRecver,
				BuildNetString(NAME_NULL, strTitle),
				BuildNetString(NAME_NULL, strMessage),
				POST_PACKATE, 0, 0, 0, pNew);
			break;
		}

	}
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CASTLEAPPLICANTCOUNT_REQ(LPPACKETBUF pBUF)
{
	WORD wCastle;
	DWORD dwGuildID;
	BYTE bCamp;
	BYTE bCount;

	pBUF->m_packet
		>> wCastle
		>> dwGuildID
		>> bCamp
		>> bCount;

	LPTLOCAL pCastle = FindCastle(wCastle);
	if(pCastle)
	{
		if(bCamp == CAMP_ATTACK)
			pCastle->m_bAtkCount = bCount;
		else
			pCastle->m_bDefCount = bCount;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_GAINCASHBONUS_REQ(LPPACKETBUF pBUF)
{
	INT nRet;
	DWORD dwCharID;
	DWORD dwKey;
	CString strName;
	BYTE bInven;
	BYTE bItem;
	WORD wItemID;
	DWORD dwBonus;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> strName
		>> bInven
		>> bItem
		>> wItemID
		>> dwBonus;

	DEFINE_QUERY(&m_db, CSPGainCashBonus)
	query->m_dwCharID = dwCharID;
	query->m_bInven = bInven;
	query->m_bItem = bItem;
	query->m_dwBonus = dwBonus;
	if(query->Call())
		nRet = query->m_nRet;
	else
		nRet = CASHSHOP_INTERNAL;
	UNDEFINE_QUERY()

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GAINCASHBONUS_ACK)
		<< nRet
		<< dwCharID
		<< dwKey
		<< strName
		<< bInven
		<< bItem
		<< wItemID
		<< dwBonus;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_GAINCASHBONUS_ACK(LPPACKETBUF pBUF)
{
	INT nRet;
	DWORD dwCharID;
	DWORD dwKey;
	CString strName;
	BYTE bInven;
	BYTE bItem;
	WORD wItemID;
	DWORD dwBonus;

	pBUF->m_packet
		>> nRet
		>> dwCharID
		>> dwKey
		>> strName
		>> bInven
		>> bItem
		>> wItemID
		>> dwBonus;

	if(nRet)
	{
		LPTITEM pTITEM = FindTItem(wItemID);
		if(!pTITEM)
			return EC_NOERROR;

		CTItem * pNew = new CTItem();
		pNew->m_dlID = GenItemID();
		pNew->m_bCount = 1;
		pNew->m_wItemID = wItemID;
		pNew->m_pTITEM = pTITEM;
		pNew->SetDuration(FALSE);
		SetItemAttr(pNew, 0);

		SendDM_POSTRECV_REQ(
			0, dwCharID, GetSvrMsg(NAME_OPERATOR), strName,
			BuildNetString(NAME_NULL, GetSvrMsg(MSG_ITEMERROR_TITLE)),
			BuildNetString(NAME_NULL, GetSvrMsg(MSG_ITEMERROR_MESSAGE)),
			POST_PACKATE,
			0,0,0, pNew);

		delete pNew;
	}

	return EC_NOERROR;
}

 
DWORD CTMapSvrModule::OnMW_MONTHRANKLIST_REQ(LPPACKETBUF pBUF)
{
	BYTE bCount;

	pBUF->m_packet
		>> m_bRankMonth
		>> bCount;		

	if(bCount > MONTHRANKCOUNT)
		return EC_NOERROR;

	for(BYTE i = 0; i < COUNTRY_COUNT; i++)
		for(BYTE j = 0; j < bCount; j++)
		{
			m_arMonthRank[i][j].WrapPacketOut(&(pBUF->m_packet));
			m_arMonthRank[i][j].m_dwMonthRank = j;
		}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_FIRSTGRADEGROUP_REQ(LPPACKETBUF pBUF)
{
	BYTE bCount;
	BYTE bRankMonth;

	pBUF->m_packet
		>> bRankMonth
		>> bCount;		

	if(bCount > FIRSTGRADEGROUPCOUNT)
		return EC_NOERROR;

	for(BYTE i = 0; i < COUNTRY_COUNT; i++)
		for(BYTE j = 0; j < bCount; j++)
		{
			m_arFirstGradeGroup[i][j].WrapPacketOut(&(pBUF->m_packet));
			m_arFirstGradeGroup[i][j].m_dwMonthRank = j;
		}

	return EC_NOERROR;
}
DWORD CTMapSvrModule::OnMW_MONTHRANKRESET_REQ(LPPACKETBUF pBUF)
{
	BYTE bRankMonth;
	BYTE bCount;

	pBUF->m_packet
		>> bRankMonth
		>> bCount;

	if(bCount > FAMERANKCOUNT)
		return EC_NOERROR;

	for(BYTE i = 0; i < bCount; i++)
	{
		m_arFameRank[FRT_HERO][bRankMonth][i].WrapPacketOut(&(pBUF->m_packet));
		m_arFameRank[FRT_HERO][bRankMonth][i].m_dwMonthRank = i;
	}

	MAPDWORD mapPlayer;
	MAPPLAYER::iterator it;
	for( it = m_mapPLAYER.begin(); it != m_mapPLAYER.end(); it++)
	{
		(*it).second->MonthRankRest();
		(*it).second->SendCS_UPDATEFAMERANKLIST_ACK(m_arFameRank,bRankMonth);
		(*it).second->SendCS_PVPPOINT_ACK( (*it).second->m_dwPvPTotalPoint, (*it).second->m_dwPvPUseablePoint, 0,0);

		if(!(*it).second->m_pMAP)
		{
			mapPlayer.insert(MAPDWORD::value_type((*it).second->m_dwID, (*it).second->m_bCountry));
		}
	}

	if( !mapPlayer.empty())
	{
		LPPACKETBUF pMSG = new PACKETBUF();
		pMSG->m_packet.SetID(DM_MONTHPVPOINTRESET_REQ);
        SayToDB(pMSG);

		MAPDWORD::iterator itD;
		for(itD=mapPlayer.begin(); itD!=mapPlayer.end(); itD++)
			SendMW_MONTHRANKRESETCHAR_ACK((*itD).first);
	}

	m_bRankMonth = bRankMonth + 1;
	if(m_bRankMonth > 12 )
		m_bRankMonth -= 12;

	for(BYTE m = 0; m < COUNTRY_COUNT; m++)
		for(BYTE n = 1; n < MONTHRANKCOUNT; n++)
			m_arMonthRank[m][n].Reset();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_MONTHRANKRESETCHAR_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	pBUF->m_packet
		>> dwCharID;

	MAPPLAYER::iterator it = m_mapPLAYER.find(dwCharID);
	if(it!=m_mapPLAYER.end())
		it->second->MonthRankRest();

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_FAMERANKUPDATE_REQ(LPPACKETBUF pBUF)
{
	BYTE bMonth;
	BYTE bType;
	BYTE bOrder;

	pBUF->m_packet
		>> bType
		>> bMonth
		>> bOrder;

	if(bType >= FRT_COUNT || bMonth >= MONTHCOUNT || bOrder >= FAMERANKCOUNT)
		return EC_NOERROR;

	m_arFameRank[bType][bMonth][bOrder].WrapPacketOut(&pBUF->m_packet);

	MAPPLAYER::iterator it;
	for( it = m_mapPLAYER.begin(); it != m_mapPLAYER.end(); it++)
		(*it).second->SendCS_UPDATEFAMERANKLIST_ACK(m_arFameRank,bMonth);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_MONTHRANKUPDATE_REQ(LPPACKETBUF pBUF)
{
	BYTE bMonth;
	BYTE bCountry;
	BYTE bStartRank;
	BYTE bEndRank;
	BYTE bNewWarlord;	
	MONTHRANKER stWARLORD;

	pBUF->m_packet
		>> bMonth
		>> bCountry
		>> bStartRank
		>> bEndRank;

	if(bStartRank > bEndRank || bEndRank >= MONTHRANKCOUNT || bMonth >= MONTHCOUNT)
		return EC_NOERROR;

	if(bStartRank && bEndRank)
		for(BYTE i = bStartRank; i <= bEndRank; i++)
		{
			m_arMonthRank[bCountry][i].WrapPacketOut(&(pBUF->m_packet));
			m_arMonthRank[bCountry][i].m_dwMonthRank = i;
		}

	pBUF->m_packet
		>> bNewWarlord;

	if(bNewWarlord)
	{
		m_arMonthRank[bCountry][0].WrapPacketOut( &(pBUF->m_packet));
		m_arMonthRank[bCountry][0].m_dwMonthRank = 0;
	}

	m_bRankMonth = bMonth;

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_WARLORDSAY_REQ(LPPACKETBUF pBUF)
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

	if(bType >= FRT_COUNT || bRankMonth >= MONTHCOUNT)
		return EC_NOERROR;

	m_arFameRank[bType][bRankMonth][0].m_strSay = strSay;

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_WARLORDSAY_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet;
	BYTE bRankMonth;
	DWORD dwCharID;
	CString strSay;
	BYTE bType;

	pBUF->m_packet
		>> bType
		>> bRankMonth
		>> dwCharID
		>> strSay;

	DEFINE_QUERY(&m_db,CSPSaveWarlordSay)
	query->m_bType = bType;
	query->m_bRankMonth = bRankMonth;
	query->m_dwCharID = dwCharID;
	lstrcpy(query->m_szSay,strSay);

	if(!query->Call())
		bRet = TRUE;
	else
		bRet = query->m_nRET;
	
	UNDEFINE_QUERY()

	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_WARLORDSAY_ACK)
		<< bRet
		<< bType
		<< bRankMonth
		<< dwCharID
		<< strSay;

	SayToDB(pMsg);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_WARLORDSAY_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet;
	BYTE bRankMonth;
	DWORD dwCharID;
	CString strSay;
	BYTE bType;

	pBUF->m_packet
		>> bRet
		>> bType
		>> bRankMonth
		>> dwCharID
		>> strSay;

	if(!bRet)
		SendMW_WARLORDSAY_ACK(bType, bRankMonth,dwCharID,strSay);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_MONTHPVPOINTRESET_REQ(LPPACKETBUF pBUF)
{	
	DEFINE_QUERY(&m_db,CSPMonthPvPointClear)
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_TERMINATE_REQ(LPPACKETBUF pBUF)
{
	// #FIXME: This is a possible backdoor. Keep an eye out for this!
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

DWORD CTMapSvrModule::OnDM_POSTVIEW_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwPostID;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwPostID;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_POSTVIEW_ACK)
		<< dwCharID
		<< dwKey
		<< dwPostID;

	WORD wSize = pMSG->m_packet.GetSize();
	BYTE bRet = BYTE(POST_SUCCESS);
	pMSG->m_packet
		<< bRet;

	BYTE bItemCnt = 0;
	DEFINE_QUERY(&m_db, CSPPostView)
	query->m_dwCharID = dwCharID;
	query->m_dwPostID = dwPostID;
	if(query->Call() && !query->m_nRet)
	{
		pMSG->m_packet
			<< query->m_dwSendID
			<< query->m_bType
			<< query->m_bRead
			<< query->m_dwGold
			<< query->m_dwSilver
			<< query->m_dwCooper
			<< query->m_szSender
			<< query->m_szTitle
			<< query->m_szMessage;

		pMSG->m_packet
			<< __DBTOTIME(query->m_timeRecv);

		bItemCnt = query->m_bItemCount;
	}
	else
		bRet = BYTE(POST_NOTFOUND);

	UNDEFINE_QUERY()

	pMSG->m_packet
		<< bItemCnt;

	if(bItemCnt)
	{
		BYTE bCnt = 0;

		DEFINE_QUERY(&m_db, CTBLPostItem)
		query->m_dwOwnerID = dwCharID;
		query->m_bOwnerType = TOWNER_CHAR;
		query->m_bStorageType = STORAGE_POST;
		query->m_dwStorageID = dwPostID;
		if(query->Open())
		{
			while(query->Fetch())
			{
				WrapItem(
					&(pMSG->m_packet),
					query->m_item);

				bCnt++;
			}
			query->Close();
		}
		UNDEFINE_QUERY()

		if(bCnt != bItemCnt)
			bRet = BYTE(POST_NOTFOUND);
	}

	if(bRet != POST_SUCCESS)
		memcpy( pMSG->m_packet.GetBuffer() + wSize, &bRet, sizeof(bRet));

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_POSTVIEW_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwPostID;
	BYTE bRet;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwPostID
		>> bRet;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer || !pPlayer->m_bMain)
		return EC_NOERROR;

	if(bRet != POST_SUCCESS ||
		pPlayer->m_dwPostID != dwPostID)
	{
		pPlayer->m_dwPostID = 0;
		pPlayer->ClearPost();
		return EC_NOERROR;
	}

	DWORD dwSendID;
	BYTE bType;
	BYTE bRead;
	DWORD dwGold, dwSilver, dwCooper;
	CString strSender, strTitle, strMessage;
	INT64 timeRecv;
	BYTE bItemCnt;

	pBUF->m_packet
		>> dwSendID
		>> bType
		>> bRead
		>> dwGold
		>> dwSilver
		>> dwCooper
		>> strSender
		>> strTitle
		>> strMessage
		>> timeRecv
		>> bItemCnt;

	LPTPOST pPost = pPlayer->MakePost(
		dwPostID,
		dwSendID,
		strSender,
		strTitle,
		strMessage,
		bType,
		bRead,
		timeRecv,
		dwGold,
		dwSilver,
		dwCooper);

	for(BYTE i=0; i<bItemCnt; i++)
	{
		CTItem *pTITEM = CreateItem(&(pBUF->m_packet));
		if(pTITEM)
			pPost->m_vItem.push_back(pTITEM);
	}

	pPlayer->m_dwPostID = 0;
	pPlayer->m_pPost = pPost;
	pPlayer->SendCS_POSTVIEW_ACK(dwPostID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_TOURNAMENT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	WORD wProtocol;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> wProtocol;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	switch(wProtocol)
	{
	case MW_TOURNAMENTAPPLY_REQ:
		{
			BYTE bResult;
			BYTE bEntryID;

			pBUF->m_packet
				>> bResult
				>> bEntryID;

			LPTOURNAMENT pTour = FindTournament(bEntryID);
			if(pTour)
			{
				if(bResult)
				{
					pPlayer->EarnMoney(pTour->m_dwFee);
					if(pTour->m_wPermitItem && pTour->m_bPermitCount)
					{
						LPTITEM pTITEM = FindTItem(pTour->m_wPermitItem);
						if(pTITEM)
						{
							VTITEM vItem;
							vItem.clear();
							CTItem * pNew = new CTItem();
							pNew->m_dlID = GenItemID();
							pNew->m_bCount = pTour->m_bPermitCount;
							pNew->m_wItemID = pTour->m_wPermitItem;
							pNew->m_pTITEM = pTITEM;
							pNew->SetDuration(FALSE);
							SetItemAttr(pNew, 0);
							vItem.push_back(pNew);
							if(pPlayer->CanPush(&vItem, 0))
							{
								pPlayer->PushTItem(&vItem);
								pPlayer->SendCS_MOVEITEM_ACK(MI_SUCCESS);
							}
						}
					}
				}
				else
				{
#ifdef DEF_UDPLOG
					m_pUdpSocket->LogTournamentApply(LOGMAP_TOURNAMENTAPPLY, pPlayer, pTour->m_strName, pTour->m_bType);
#endif	//	DEF_UDPLOG
				}

				if(pTour->m_dwFee)
					pPlayer->SendCS_MONEY_ACK();
			}
		}
		break;
	case MW_TOURNAMENTPARTYADD_REQ:
		{
			BYTE bResult;
			CString strTarget;
			DWORD dwTargetID;

			pBUF->m_packet
				>> bResult
				>> strTarget
				>> dwTargetID;

			if(TOURNAMENT_SUCCESS == bResult && dwTargetID)
			{
				SendDM_POSTRECVFROMSVR_REQ(
					dwTargetID, strTarget,
					BuildNetString(NAME_NULL, GetSvrMsg(TNMT_PARTYADD_TITLE)),
					BuildNetString(NAME_NULL, GetSvrMsg(TNMT_PARTYADD_MESSAGE)),
					POST_NORMAL,0,0,0,NULL);
			}

#ifdef DEF_UDPLOG
					m_pUdpSocket->LogTournamentApply(LOGMAP_TOURNAMENTAPPLY, pPlayer, NAME_NULL, ENTRY_PARTY, strTarget);
#endif	//	DEF_UDPLOG
		}
		break;
	default:
		break;
	}
	pPlayer->SendCS_TOURNAMENT_ACK(&(pBUF->m_packet), wProtocol);


	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_TOURNAMENTENABLE_REQ(LPPACKETBUF pBUF)
{
	BYTE bGroup;
	BYTE bStep;
	DWORD dwPeriod;
	INT64 dlNextStep;

	pBUF->m_packet
		>> bGroup
		>> bStep
		>> dwPeriod
		>> dlNextStep;

	BYTE bCurStep = m_tournament.m_bStep;

	MAPPLAYER::iterator it;
	for(it=m_mapPLAYER.begin();it!=m_mapPLAYER.end(); it++)
		if((*it).second->m_bMain)
			(*it).second->SendCS_SYSTEMMSG_ACK(SM_TOURNAMENT_STEP, bStep, dwPeriod, NAME_NULL, NAME_NULL, bGroup, 0, NAME_NULL, 0, dlNextStep);

	if(bStep == TNMTSTEP_QFINAL ||
		bStep == TNMTSTEP_SFINAL ||
		bStep == TNMTSTEP_FINAL)
	{
		m_tournament.m_bGroup = bGroup;
		m_tournament.m_bStep = bStep;
		m_tournament.m_dlEndTime = m_timeCurrent + dwPeriod;

		VTMAP vMAP;
		GetTournamentMapList(&vMAP);
		for(DWORD i=0; i<vMAP.size(); i++)
		{
			CTMap * pMap = vMAP[i];
			pMap->m_dwEndTick = m_dwTick + dwPeriod * 1000;

			MAPTOURNAMENTPLAYER::iterator itTP;
			for(itTP=pMap->m_mapTNMTPlayer.begin(); itTP!=pMap->m_mapTNMTPlayer.end(); itTP++)
			{
				if((*itTP).second->m_wMapID == pMap->m_wMapID)
				{
					CTPlayer * pTP = FindPlayer((*itTP).second->m_strName);
					if(pTP)
					{
						for(DWORD i=0; i<pTP->m_vMaintainSkill.size();i++)
						{
							if(pTP->m_vMaintainSkill[i]->m_pTSKILL->IsHideBuff())
							{
								if(pTP->m_vMaintainSkill[i]->m_dwMaintainTick)
									pTP->m_vMaintainSkill[i]->SetLoopEndTick(m_dwTick, pTP->m_vMaintainSkill[i]->GetRemainTick(m_dwTick));
								else
									pTP->m_vMaintainSkill[i]->m_dwChargeTick = m_dwTick;
							}
						}
					}
				}
			}

			if(!pMap->CanDoTournament(bStep))
			{
				BYTE bRet = 0;
				DWORD dwWinner = 0;
				DWORD dwLoser = 0;

				if(pMap->EndTournament(bRet, dwWinner, dwLoser, bStep))
				{
					SendMW_TOURNAMENTRESULT_ACK(
						m_tournament.m_bStep,
						bRet,
						dwWinner,
						dwLoser);
				}

				pMap->ResetTournament();
			}
		}
		vMAP.clear();
	}
	else if(bStep == TNMTSTEP_QFEND ||
		bStep == TNMTSTEP_SFEND ||
		bStep == TNMTSTEP_END)
	{
		VTMAP vMAP;
		GetTournamentMapList(&vMAP);

		m_dwTNMTResultTick = m_dwTick;
		m_tournament.m_bResult = m_tournament.m_bStep;
		m_vTNMTRESULTMAP.clear();

		for(DWORD i=0; i<vMAP.size(); i++)
		{
			CTMap * pMap = vMAP[i];
			if(!pMap->m_dwEndTick)
				continue;

			MAPTOURNAMENTPLAYER::iterator itTP;
			for(itTP=pMap->m_mapTNMTPlayer.begin(); itTP!=pMap->m_mapTNMTPlayer.end(); itTP++)
			{
				if((*itTP).second->m_wMapID == pMap->m_wMapID)
				{
					CTPlayer * pTP = FindPlayer((*itTP).second->m_strName);
					if(pTP)
					{
						for(DWORD i=0; i<pTP->m_vMaintainSkill.size();i++)
						{
							if(pTP->m_vMaintainSkill[i]->m_pTSKILL->IsHideBuff())
							{
								pMap->SetHideTick(pTP->m_dwID, m_dwTick - pTP->m_vMaintainSkill[i]->m_dwChargeTick);
							}
						}
					}
				}
			}

			if(pMap->m_wAtkKillPoint == pMap->m_wDefKillPoint &&
				pMap->m_dwBlueHideTick == pMap->m_dwRedHideTick)
			{
				m_vTNMTRESULTMAP.push_back(pMap);

				pMap->SelectWinner();

				VPLAYER vPlayer;
				vPlayer.clear();
				pMap->GetMapPlayers(&vPlayer);
				while(!vPlayer.empty())
				{
					CTPlayer * pChar = vPlayer.back();
					pChar->SendCS_TOURNAMENTSELECTWINNER_ACK(pMap->m_wDefKillPoint, pMap->m_wAtkKillPoint);
					vPlayer.pop_back();
				}
			}
			else
			{
				BYTE bRet = 0;
				DWORD dwWinner = 0;
				DWORD dwLoser = 0;
				DWORD dwBlueHideTick = 0;
				DWORD dwRedHideTick = 0;

				if(pMap->m_wAtkKillPoint == pMap->m_wDefKillPoint)
				{
					dwBlueHideTick = pMap->m_dwBlueHideTick;
					dwRedHideTick = pMap->m_dwRedHideTick;
				}

				pMap->EndTournament(bRet, dwWinner, dwLoser);

				SendMW_TOURNAMENTRESULT_ACK(
					m_tournament.m_bStep,
					bRet,
					dwWinner,
					dwLoser,
					dwBlueHideTick,
					dwRedHideTick);
			}
		}
	}
	else if(bStep == TNMTSTEP_SFENTER ||
		bStep == TNMTSTEP_FENTER)
	{
		m_tournament.m_bGroup = bGroup;
		m_tournament.m_bStep = bStep;
		m_tournament.m_dlEndTime = m_timeCurrent + dwPeriod;

		MAPTOURNAMENTPLAYER::iterator itTP;
		for(itTP=m_mapTNMTPlayer.begin(); itTP!=m_mapTNMTPlayer.end(); itTP++)
		{
			WORD wMapID = GetTournamentMap((*itTP).second->m_dwCharID, bGroup);
			if(wMapID)
			{
				CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, wMapID);
				if(pMap)
					pMap->AddTNMTPlayer((*itTP).second);
			}

			if(itTP->second->m_wMapID)
			{
				CTPlayer * pChar = FindPlayer(itTP->second->m_strName);
				if(pChar)
					Teleport(pChar, GetTNMTPlayerLounge(pChar));
			}
		}

		VTMAP vMAP;
		vMAP.clear();
		GetTournamentMapList(&vMAP);
		for(DWORD i=0; i<vMAP.size(); i++)
		{
			CTMap * pMap = vMAP[i];
			LPTSPAWNPOS pPos = NULL;

			VPLAYER vPlayer;
			vPlayer.clear();
			pMap->GetMapPlayers(&vPlayer);
			while(!vPlayer.empty())
			{
				CTPlayer * pChar = vPlayer.back();
				pChar->SendCS_TOURNAMENTENTER_ACK(pMap);
				vPlayer.pop_back();
			}
		}
	}

	m_tournament.m_bGroup = bGroup;
	m_tournament.m_bStep = bStep;
	m_tournament.m_dlEndTime = m_timeCurrent + dwPeriod;

	if(bCurStep > bStep)
	{
		MAPPLAYER::iterator it;
		for(it=m_mapPLAYER.begin();it!=m_mapPLAYER.end(); it++)
			if((*it).second->m_bMain)
			{
				if(IsTournamentMap((*it).second->m_wMapID) ||
					(*it).second->m_wMapID == GetLoungeMapID())
				{
					Teleport((*it).second, (*it).second->m_wLastSpawnID ? (*it).second->m_wLastSpawnID : (*it).second->m_wSpawnID);
				}
			}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_TOURNAMENTINFO_REQ(LPPACKETBUF pBUF)
{
	ClearTournamentPlayer();
	m_mapTournament.clear();

	BYTE bTCount;
	BYTE bRCount;
	BYTE bStep;
	BYTE bGroup;

	pBUF->m_packet
		>> m_bFirstGroupCount
		>> bGroup
		>> bStep
		>> bTCount;

	for(BYTE i=0; i<bTCount; i++)
	{
		TOURNAMENT tour;
		pBUF->m_packet
			>> tour.m_bGroup
			>> tour.m_bEntryID
			>> tour.m_strName
			>> tour.m_bType
			>> tour.m_dwClass
			>> tour.m_dwFee
			>> tour.m_dwBackFee
			>> tour.m_wPermitItem
			>> tour.m_bPermitCount
			>> tour.m_bMinLevel
			>> tour.m_bMaxLevel
			>> bRCount;

		for(BYTE j=0; j<bRCount; j++)
		{
			TNMTREWARD reward;

			pBUF->m_packet
				>> reward.m_bChartType
				>> reward.m_wItemID
				>> reward.m_bCount;

			tour.m_vReward.push_back(reward);
		}
		m_mapTournament.insert(MAPTOURNAMENT::value_type(tour.m_bEntryID, tour));
	}

	m_tournament.m_bStep = bStep;
	m_tournament.m_bGroup = bGroup;
	m_tournament.m_dlEndTime = 0;
	m_tournament.m_bResult = 0;
	m_vTNMTRESULTMAP.clear();

	MAPPLAYER::iterator it;
	for(it=m_mapPLAYER.begin();it!=m_mapPLAYER.end(); it++)
		if((*it).second->m_bMain)
		{
			(*it).second->SendCS_SYSTEMMSG_ACK(SM_TOURNAMENT_STEP, bStep, 0, NAME_NULL, NAME_NULL, bGroup);

			if(IsTournamentMap((*it).second->m_wMapID) ||
				(*it).second->m_wMapID == GetLoungeMapID())
			{
				Teleport((*it).second, (*it).second->m_wLastSpawnID ? (*it).second->m_wLastSpawnID : (*it).second->m_wSpawnID);
			}
		}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_TOURNAMENTMATCH_REQ(LPPACKETBUF pBUF)
{
	ClearTournamentPlayer();

	BYTE bMCount;
	BYTE bEntryID;
	BYTE bSlot;
	DWORD dwCharID;
	BYTE bCountry;
	CString strName;
	BYTE bLevel;
	BYTE bClass;
	DWORD dwChief;
	BYTE bQFWin;
	BYTE bSFWin;
	BYTE bFWin;

	pBUF->m_packet
		>> bMCount;

	for(BYTE i=0; i<bMCount; i++)
	{
		pBUF->m_packet
			>> bEntryID
			>> bSlot
			>> dwCharID
			>> bCountry
			>> strName
			>> bLevel
			>> bClass
			>> dwChief
			>> bQFWin
			>> bSFWin
			>> bFWin;

		LPTOURNAMENT pTour = FindTournament(bEntryID);
		if(pTour)
		{
			LPTOURNAMENTPLAYER pPlayer = new TOURNAMENTPLAYER();
			pPlayer->m_bEntryID = bEntryID;
			pPlayer->m_bSlot = bSlot;
			pPlayer->m_dwCharID = dwCharID;
			pPlayer->m_bCountry = bCountry;
			pPlayer->m_strName = strName;
			pPlayer->m_bLevel = bLevel;
			pPlayer->m_bClass = bClass;
			pPlayer->m_dwChiefID = dwChief;
			pPlayer->m_bResult[MATCH_QFINAL] = bQFWin;
			pPlayer->m_bResult[MATCH_SFINAL] = bSFWin;
			pPlayer->m_bResult[MATCH_FINAL] = bFWin;
			pPlayer->m_wMapID = 0;
			m_mapTNMTPlayer.insert(MAPTOURNAMENTPLAYER::value_type(dwCharID, pPlayer));

			WORD wMapID = GetTournamentMap(dwCharID, 0);
			if(wMapID)
			{
				CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, wMapID);
				if(pMap)
					pMap->AddTNMTPlayer(pPlayer);
			}
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_TOURNAMENTRESULT_REQ(LPPACKETBUF pBUF)
{
	WORD wTournamentID;
	BYTE bStep;
	BYTE bRet;
	DWORD dwWinner;
	DWORD dwLoser;
	DWORD dwBlueHideTick;
	DWORD dwRedHideTick;
	BYTE bCount;

	pBUF->m_packet
		>> wTournamentID
		>> bStep
		>> bRet
		>> dwWinner
		>> dwLoser
		>> dwBlueHideTick
		>> dwRedHideTick
		>> bCount;

	if(bStep != TNMTSTEP_QFINAL &&
		bStep != TNMTSTEP_SFINAL &&
		bStep != TNMTSTEP_FINAL)
		return EC_NOERROR; 

	MAPTOURNAMENTPLAYER mapWin;
	mapWin.clear();

	LPTOURNAMENTPLAYER pWin = FindTournamentPlayer(dwWinner);
	if(pWin)
	{
		SetTournamentResult(bStep, pWin, bRet ? TNMTWIN_WIN : TNMTWIN_LOSE);
		mapWin.insert(MAPTOURNAMENTPLAYER::value_type(pWin->m_dwCharID, pWin));
	}

	LPTOURNAMENTPLAYER pLos = FindTournamentPlayer(dwLoser);
	if(pLos)
		SetTournamentResult(bStep, pLos, TNMTWIN_LOSE);


	for(BYTE i=0; i<bCount; i++)
	{
		DWORD dwParty;
		pBUF->m_packet
			>> dwParty;

		LPTOURNAMENTPLAYER pParty = FindTournamentPlayer(dwParty);
		if(pParty)
		{
			if(pWin &&
				pWin->m_bEntryID == pParty->m_bEntryID &&
				pWin->m_bSlot == pParty->m_bSlot)
			{
				SetTournamentResult(bStep, pParty, bRet ? TNMTWIN_WIN : TNMTWIN_LOSE);
				mapWin.insert(MAPTOURNAMENTPLAYER::value_type(pParty->m_dwCharID, pParty));
			}

			if(pLos &&
				pLos->m_bEntryID == pParty->m_bEntryID &&
				pLos->m_bSlot == pParty->m_bSlot)
				SetTournamentResult(bStep, pParty, TNMTWIN_LOSE);
		}
	}

	MAPPLAYER::iterator it;
	for(it=m_mapPLAYER.begin(); it!=m_mapPLAYER.end(); it++)
	{
		if((*it).second->m_pMAP &&
			(*it).second->m_bMain &&
			IsTournamentMap((*it).second->m_wMapID))
		{
			(*it).second->SendCS_TOURNAMENTRESULT_ACK(bStep, bRet, dwWinner, dwLoser, dwBlueHideTick, dwRedHideTick);
		}
	}

	if(bStep == TNMTSTEP_FINAL && bRet && pWin)
	{
		MAPTOURNAMENTPLAYER::iterator itTW;
		for(itTW=mapWin.begin(); itTW!=mapWin.end(); itTW++)
		{
			CTPlayer * pPlayer = FindPlayer((*itTW).second->m_strName);
			if(pPlayer && pPlayer->m_bMain)
			{
				SendDM_TOURNAMENTREWARD_REQ(m_bRankMonth-1 ? m_bRankMonth-1 : 12, wTournamentID, (*itTW).second->m_bEntryID, (*itTW).second->m_dwChiefID, pPlayer->m_dwID, pPlayer->IsEquipShield());
#ifdef DEF_UDPLOG
				LPTOURNAMENT pEntry = FindTournament((*itTW).second->m_bEntryID);
				if(pEntry)
					TournamentResultLog(pPlayer->m_dwID, dwLoser, pEntry->m_bType);
#endif
			}
		}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_TOURNAMENTREWARD_REQ(LPPACKETBUF pBUF)
{
	BYTE bMonth;
	WORD wTournamentID;
	BYTE bEntryID;
	DWORD dwChiefID;
	DWORD dwWinner;
	BYTE bIsEquipShield;

	pBUF->m_packet
		>> bMonth
		>> wTournamentID
		>> bEntryID
		>> dwChiefID
		>> dwWinner
		>> bIsEquipShield;

	DEFINE_QUERY(&m_db, CSPTournamentReward)
	query->m_wTournamentID = wTournamentID;
	query->m_bEntryID = bEntryID;
	query->m_dwChiefID = dwChiefID;
	query->m_dwWinner = dwWinner;
	query->m_bIsEquipShield = bIsEquipShield;
	query->m_bMonth = bMonth;
	query->Call();

	if(wTournamentID == 1)
	{
		LPPACKETBUF pMSG = new PACKETBUF();
		pMSG->m_packet.SetID(DM_TOURNAMENTREWARD_ACK)
			<< wTournamentID
			<< bMonth
			<< query->m_bType
			<< query->m_bOrder
			<< dwWinner
			<< query->m_szName
			<< query->m_dwMonthRank
			<< query->m_dwTotalRank
			<< query->m_dwMonthPoint
			<< query->m_dwTotalPoint
			<< query->m_wMonthWin
			<< query->m_wMonthLose
			<< query->m_dwTotalWin
			<< query->m_dwTotalLose
			<< query->m_bCountry
			<< query->m_bLevel
			<< query->m_bClass
			<< query->m_bRace
			<< query->m_bSex
			<< query->m_bFace
			<< query->m_bHair
			<< query->m_szGuild;

		SayToBATCH(pMSG);
	}

	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_TOURNAMENTREWARD_ACK(LPPACKETBUF pBUF)
{
	WORD wTournamentID;
	BYTE bMonth;
	BYTE bType;
	BYTE bOrder;

	pBUF->m_packet
		>> wTournamentID
		>> bMonth
		>> bType
		>> bOrder;

	if(bType >= FRT_COUNT || bMonth >= MONTHCOUNT || bOrder >= FAMERANKCOUNT)
		return EC_NOERROR;

	MONTHRANKER rank;

	pBUF->m_packet
		>> rank.m_dwCharID
		>> rank.m_strName
		>> rank.m_dwMonthRank
		>> rank.m_dwTotalRank
		>> rank.m_dwMonthPoint
		>> rank.m_dwTotalPoint
		>> rank.m_wMonthWin
		>> rank.m_wMonthLose
		>> rank.m_dwTotalWin
		>> rank.m_dwTotalLose
		>> rank.m_bCountry
		>> rank.m_bLevel
		>> rank.m_bClass
		>> rank.m_bRace
		>> rank.m_bSex
		>> rank.m_bFace
		>> rank.m_bHair
		>> rank.m_strGuild;

	SendMW_FAMERANKUPDATE_ACK(bType, bMonth, bOrder, rank);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_TOURNAMENTBATPOINT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	CString strName;
	DWORD dwAmount;

	pBUF->m_packet
		>> dwCharID
		>> strName
		>> dwAmount;

	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;
	CalcMoney(dwAmount, dwGold, dwSilver, dwCooper);

	SendDM_POSTRECV_REQ(
		0,dwCharID,GetSvrMsg(NAME_OPERATOR),strName,
		BuildNetString(NAME_NULL, GetSvrMsg(TNMT_BATTING_TITLE)),
		BuildNetString(NAME_NULL, GetSvrMsg(TNMT_BATTING_MESSAGE)),
		POST_PACKATE,
		dwGold,dwSilver,dwCooper,NULL);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_SAVECHARPOSITION_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKEY;

	WORD wACKMsg;

	DEFINE_QUERY( &m_db, CSPSaveCharPosition)

	pBUF->m_packet
		>> dwCharID
		>> dwKEY
		>> query->m_wMapID
		>> query->m_fPosX
		>> query->m_fPosY
		>> query->m_fPosZ
		>> query->m_wDir
		>> wACKMsg;

	query->m_dwCharID = dwCharID;
	if(!query->Call())
		return EC_NOERROR;

	if(query->m_nRet)
		return EC_NOERROR;

	UNDEFINE_QUERY()

	if(wACKMsg)
	{
		LPPACKETBUF pMSG = new PACKETBUF();

		pMSG->m_packet.SetID(wACKMsg)
			<< dwCharID
			<< dwKEY;

		SayToBATCH(pMSG);
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_HELPMESSAGE_REQ(LPPACKETBUF pBUF)
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

	INT64 timecur = m_timeCurrent;
	MAPHELPMESSAGE::iterator it = m_mapHelpMessage.find(bID);
	if(it!=m_mapHelpMessage.end())
	{
		(*it).second.m_dlStartTime = dlStart;
		(*it).second.m_dlEndTime = dlEnd;
		(*it).second.m_strMessage = strMessage;
		if(dlEnd && dlStart <= timecur)
			(*it).second.m_bNotice = TRUE;
		else
			(*it).second.m_bNotice = FALSE;
	}
	else
	{
		THELPMESSAGE help;
		help.m_bID = bID;
		help.m_dlStartTime = dlStart;
		help.m_dlEndTime = dlEnd;
		help.m_strMessage = strMessage;

		if(dlEnd && dlStart <= timecur)
			help.m_bNotice = TRUE;
		else
			help.m_bNotice = FALSE;

		m_mapHelpMessage.insert(MAPHELPMESSAGE::value_type(help.m_bID, help));
	}

	if(!dlEnd || dlStart <= timecur)
	{
		MAPPLAYER::iterator itPlayer;
		for(itPlayer=m_mapPLAYER.begin(); itPlayer!=m_mapPLAYER.end(); itPlayer++)
			if((*itPlayer).second->m_bMain)
			{
				CString strHelpMessage;
				BYTE bHelpID = _AtlModule.GetHelpMessage((*itPlayer).second, strHelpMessage);
				if(bHelpID == bID || (bHelpID != HELPMESSAGE_TUTORIAL && bHelpID < bID))
					(*itPlayer).second->SendCS_HELPMESSAGE_ACK(bHelpID, strHelpMessage);
			}
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_MISSIONENABLE_REQ(LPPACKETBUF pBUF)
{
	static BATTLE_STATUS bCurStatus = BS_NORMAL;
	static DWORD dwCurStart = 0;

	BYTE bStatus;
	DWORD dwStart;
	DWORD dwSecond;
	pBUF->m_packet
		>> bStatus
		>> dwStart
		>> dwSecond;

	if(!dwCurStart)
		dwCurStart = dwStart;

	for(DWORD ll=0; ll<m_vMission.size(); ll++)
	{
		LPTLOCAL pMission = m_vMission[ll];

		if(!pMission->m_timeNextDefend || (bCurStatus != BS_NORMAL && bStatus == BS_NORMAL))
		{
			CTime now = CTime(m_timeCurrent);
			DWORD dwAdd = (dwStart/HOUR_ONE)%2 == pMission->m_pZone->m_bLine ? dwStart : dwStart + HOUR_ONE;
			CTime next = CTime(now.GetYear(), now.GetMonth(), now.GetDay(), 0, 0, dwAdd);
			pMission->m_timeNextDefend = next.GetTime();

			MAPTLOCAL::iterator itLocal = m_mapTLOCAL.begin();
			if(itLocal != m_mapTLOCAL.end())
			{
				CTime tLT = CTime((*itLocal).second->m_timeNextDefend);
				if(tLT.GetHour() == next.GetHour())
					pMission->m_timeNextDefend += HOUR_ONE * 2;
			}
		}

		if((dwCurStart/HOUR_ONE)%2 != pMission->m_pZone->m_bLine)
			continue;

		if(pMission->m_bStatus == BS_PEACE && bStatus != BS_NORMAL)
			continue;

		if(pMission->m_bStatus == bStatus)
			continue;

		pMission->m_bStatus = bStatus;

		if(!pMission->m_pZone ||
			!pMission->m_bValid)
			continue;

		switch(bStatus)
		{
		case BS_NORMAL:
			{
				CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, pMission->m_pZone->m_wMapID);
				if(pMap)
				{
					pMission->m_bOccupied = FALSE;

					for( DWORD i=0; i<pMission->m_pZone->m_vSpawnID.size(); i++)
						DelMonSpawn(pMap, pMission->m_pZone->m_vSpawnID[i]);
				}
			}
			break;
		case BS_BATTLE:
			{
				CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, pMission->m_pZone->m_wMapID);
				if(pMap)
				{
					for( DWORD i=0; i<pMission->m_pZone->m_vSpawnID.size(); i++)
						AddMonSpawn(pMap, pMission->m_pZone->m_vSpawnID[i], MONSPAWN_SUSPEND);
				}
			}
			break;
		case BS_PEACE:
			{
				CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, pMission->m_pZone->m_wMapID);

				if(pMap)
				{
					if(!pMission->m_bOccupied)
						SendDM_MISSIONOCCUPY_REQ(
							pMission->m_wLocal,
							OCCUPY_DEFEND,
							TCONTRY_N,
							0);

					for(DWORD i=0; i<pMission->m_pZone->m_vSpawnID.size(); i++)
					{
						MAPTMONSPAWN::iterator it = pMap->m_mapTMONSPAWN.find(pMission->m_pZone->m_vSpawnID[i]);

						if(it!=pMap->m_mapTMONSPAWN.end())
						{
							(*it).second->m_bStatus = MONSPAWN_READY;
							for(DWORD j=0; j<(*it).second->m_vTMON.size(); j++)
							{
								(*it).second->m_vTMON[j]->OnEvent( AT_DEAD, 0, 0, 0, 0);
							}
						}
					}
				}

				pMission->m_bOccupied = FALSE;
			}
			break;
		}
	}

	BYTE bType = SM_NONE;
	switch(bStatus)
	{
	case BS_NORMAL:
		if(bCurStatus == BS_NORMAL)
			bType = SM_MISSION_START_ALARM;
//		else if (bCurStatus == BS_PEACE)
//			bType = SM_MISSION_NORMAL;
		break;
	case BS_BATTLE:
		if(bCurStatus == BS_BATTLE)
			bType = SM_MISSION_END_ALARM;
		else if(bCurStatus == BS_NORMAL)
			bType = SM_MISSION_START;
		break;
	case BS_PEACE:
		if(bCurStatus == BS_BATTLE)
		{
			bType = SM_MISSION_PEACE;
		}
		break;
	default:
		break;
	}

	NotifyLocalInfo(bType, BYTE(dwCurStart/HOUR_ONE), dwSecond);

	bCurStatus = (BATTLE_STATUS)bStatus;
	dwCurStart = dwStart;

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_RPSGAME_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bResult;
	BYTE bPlayerRPS;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bResult
		>> bPlayerRPS;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer || !pPlayer->m_bRPSType)
		return EC_NOERROR;

	BYTE bNpcRPS = bPlayerRPS;

	if(bResult)
	{
		pPlayer->m_bRPSWin++;
		bNpcRPS = SelectNpcRPS(TNMTWIN_WIN, bPlayerRPS);
	}
	else
	{
		pPlayer->ResetRPS();
		bNpcRPS = SelectNpcRPS(TNMTWIN_LOSE, bPlayerRPS);
	}

	pPlayer->SendCS_RPSGAME_ACK(pPlayer->m_bRPSWin, bPlayerRPS, bNpcRPS);

	MAPRPSGAME::iterator itF = m_mapRPSGame.find(MAKEWORD(pPlayer->m_bRPSType, pPlayer->m_bRPSWin + 1));
	if(itF == m_mapRPSGame.end())
		RPSReward(pPlayer, FALSE);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_RPSGAMECHANGE_REQ(LPPACKETBUF pBUF)
{
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

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_WARCOUNTRYBALANCE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	DWORD dwCountD;
	DWORD dwCountC;
	BYTE bGap;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> dwCountD
		>> dwCountC
		>> bGap;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(pPlayer)
	{
		pPlayer->SendCS_WARCOUNTRYBALANCE_ACK(dwCountD, dwCountC);
		m_dwWarCountryBalance[TCONTRY_D][bGap] = dwCountD;
		m_dwWarCountryBalance[TCONTRY_C][bGap] = dwCountC;
	}

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_MEETINGROOM_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwKey;
	BYTE bType;
	BYTE bResult;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> dwKey
		>> bType
		>> bResult
		>> strName;

	CTPlayer * pPlayer = FindPlayer(dwCharID, dwKey);
	if(!pPlayer)
		return EC_NOERROR;

	if(bResult)
	{
		pPlayer->SendCS_MEETINGROOM_ACK(bType, bResult, strName);
		return EC_NOERROR;
	}

	if(!bType &&
		(pPlayer->IsActionBlock() ||
		pPlayer->ProtectTutorial() ||
		pPlayer->m_dwDuelID ||
		pPlayer->m_bStore || 
		pPlayer->m_dealItem.m_bStatus >= DEAL_START))
	{
		SendMW_MEETINGROOM_ACK(dwCharID, dwKey, !bType, MTR_BUSY, strName);
		return EC_NOERROR;
	}

	pPlayer->SendCS_MEETINGROOM_ACK(bType, bResult, strName);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_MEETINGROOM_REQ(LPPACKETBUF pBUF)
{
	BYTE bType;
	DWORD dwCharID;
	DWORD dwTick;

	pBUF->m_packet
		>> bType
		>> dwCharID
		>> dwTick;

	if(bType)
		m_mapMeetingRoomTime.insert(MAPDWORD::value_type(dwCharID, dwTick));		
	else
		m_mapMeetingRoomTime.erase(dwCharID);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnSM_MEETINGROOM_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;

	pBUF->m_packet
		>> dwCharID;

	MAPPLAYER::iterator it = m_mapPLAYER.find(dwCharID);
	if(it != m_mapPLAYER.end())
		CheckMeetingRoom(it->second, FALSE);

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CMGIFT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwTargetID;
	BYTE bGiftType;
	DWORD dwValue;
	BYTE bCount;
	CString strTitle, strMsg;
	DWORD dwGMID;
	BYTE bTool;
	WORD wGiftID;
	WORD wErrID;
	BYTE bRet;

	pBUF->m_packet
		>> dwTargetID
		>> bGiftType
		>> dwValue
		>> bCount
		>> strTitle
		>> strMsg
		>> dwGMID
		>> bTool
		>> wGiftID
		>> wErrID
		>> bRet;

	CTPlayer* pTarget = FindChar(dwTargetID);
	if(!pTarget)
	{
		bRet = CMGIFT_TARGET;
		return EC_NOERROR;
	}
	CTItem * pNew = NULL;
	DWORD dwGold = 0, dwSilver = 0, dwCooper = 0;
	BYTE bPostType = POST_NORMAL;
    
	switch(bGiftType)
	{
	case CGT_NONE:
		break;
	case CGT_ITEM:
		{
			LPTITEM pTITEM = FindTItem((WORD)dwValue);
			if(!pTITEM)
			{
				bRet = CMGIFT_ID;
				break;
			}

			pNew = new CTItem();
			pNew->m_dlID = GenItemID();
			pNew->m_bCount = bCount;
			pNew->m_wItemID = (WORD)dwValue;
			pNew->m_pTITEM = pTITEM;
			pNew->SetDuration(FALSE);
			bPostType = POST_PACKATE;
		}
		break;
	case CGT_MAGICITEM:
		{
			CTItem * pTITEM = FindTQusetItem(dwValue);
			if(!pTITEM)
			{
				bRet = CMGIFT_ID;
				break;
			}

			pNew = new CTItem();
			pNew->Copy(pTITEM, TRUE);
			pNew->m_bCount = bCount;

			if(pNew->m_dEndTime)
			{
				CTime time(_AtlModule.m_timeCurrent);
				time += CTimeSpan(LONG(pNew->m_dEndTime),0,0,0);
				pNew->m_dEndTime = time.GetTime();
				SetItemAttr(pNew, 0);
			}
			bPostType = POST_PACKATE;
		}
		break;
	case CGT_MONEY:
		CalcMoney(dwValue, dwGold, dwSilver, dwCooper);
		break;
	case CGT_MERIT:
		pTarget->GainPvPoint(dwValue, PVPE_WIN, PVP_USEABLE);
		break;
	case CGT_EXP:
		pTarget->GainExp(dwValue, FALSE);
		break;
	default:
		bRet = CMGIFT_FAIL;
	}

	if(CMGIFT_SUCCESS == bRet)
	{
		SendDM_POSTRECV_REQ(0, pTarget->m_dwID, GetSvrMsg(NAME_OPERATOR), pTarget->m_strNAME,
			BuildNetString(NAME_NULL, strTitle),
			BuildNetString(NAME_NULL, strMsg),
			bPostType, dwGold, dwSilver, dwCooper, pNew);
		
		SendDM_CMGIFTLOG_REQ(pTarget->m_dwUserID, pTarget->m_dwID, wGiftID, bTool?0:dwGMID, 0);
#ifdef DEF_UDPLOG
	if(CMGIFT_SUCCESS == bRet)
		m_pUdpSocket->LogCMGift(LOGMAP_CMGIFT, pTarget, (CMGIFT_SUCCESS==bRet)?0:wErrID, dwGMID, bGiftType, dwValue, bCount);
#endif
	}	
	else if(CMGIFT_ERRPOST == bRet)
	{
		SendDM_POSTRECV_REQ(0, pTarget->m_dwID, GetSvrMsg(NAME_OPERATOR), pTarget->m_strNAME,
			BuildNetString(NAME_NULL, strTitle),
			BuildNetString(NAME_NULL, strMsg),
			bPostType, dwGold, dwSilver, dwCooper, pNew);
		
		SendDM_CMGIFTLOG_REQ(pTarget->m_dwUserID, pTarget->m_dwID, wErrID, bTool?0:dwGMID, wGiftID);
	}

	if(!bTool)
	{
		CTPlayer* pGM = FindChar(dwGMID);
		if(pGM)
		{
			pGM->SendCS_CMGIFT_ACK(bRet);
			return EC_NOERROR;
		}
	}

	SendMW_CMGIFTRESULT_ACK(bRet, bTool, dwGMID);
	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnDM_CMGIFTLOG_REQ(LPPACKETBUF pBUF)
{
	DWORD dwUserID;
	DWORD dwCharID;
	WORD wGiftID;
	DWORD dwGMCharID;
	WORD wErrID;

	pBUF->m_packet
		>> dwUserID
		>> dwCharID
		>> wGiftID
		>> dwGMCharID
		>> wErrID;
    
	DEFINE_QUERY( &m_db, CSPTCMGiftLog)

	query->m_dwUserID = dwUserID;
	query->m_dwCharID = dwCharID;
	query->m_wGiftID = wGiftID;
	query->m_dwGMCharID = dwGMCharID;
	query->m_wErrID = wErrID;

	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTMapSvrModule::OnMW_CMGIFTRESULT_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwGMID;

	pBUF->m_packet
		>> bRet
		>> dwGMID;

	CTPlayer* pGM = FindChar(dwGMID);
	if(pGM)
		pGM->SendCS_CMGIFT_ACK(bRet);

	return EC_NOERROR;
}