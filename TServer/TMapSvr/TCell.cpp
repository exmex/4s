#include "StdAfx.h"


CTCell::CTCell()
{
	for( int i=0; i<MAX_CHANNEL; i++)
	{
		for( int j=0; j<UD_COUNT; j++)
			m_vServerID[j][i] = 0;

		m_vExtCell[i] = FALSE;
		m_vEnable[i] = FALSE;
	}

	m_mapRECALLMON.clear();
	m_mapSELFOBJ.clear();

	m_mapMONSTER.clear();
	m_mapPLAYER.clear();

	m_mapSWITCH.clear();
	m_mapGATE.clear();
	
	m_dwID = 0;
}

CTCell::~CTCell()
{
	m_mapRECALLMON.clear();
	m_mapSELFOBJ.clear();

	m_mapMONSTER.clear();
	m_mapPLAYER.clear();

	m_mapSWITCH.clear();
	m_mapGATE.clear();
}

BYTE CTCell::IsMainCell( BYTE bChannel)
{
	return bChannel == MAX_CHANNEL || (m_vEnable[bChannel] && !m_vExtCell[bChannel]) ? TRUE : FALSE;
}

BYTE CTCell::IsEnable( BYTE bChannel)
{
	return bChannel == MAX_CHANNEL || m_vEnable[bChannel] ? TRUE : FALSE;
}

void CTCell::EnterPlayer( CTPlayer *pPlayer,
						  CTCell *pCELL,
						  BYTE bNewMember,
						  DWORD dwTick)
{
	MAPTRECALLMON::iterator itREC;
	MAPTSELFOBJ::iterator itSELF;

	MAPTMONSTER::iterator itMON;
	MAPPLAYER::iterator itCHAR;

	MAPTSWITCH::iterator itSW;
	MAPTGATE::iterator itGT;

	for( itREC = m_mapRECALLMON.begin(); itREC != m_mapRECALLMON.end(); itREC++)
		if( (*itREC).second->m_bMain && (IsMainCell(pPlayer->m_bChannel) ||
			pPlayer->m_bChannel == (*itREC).second->m_bChannel) )
			pPlayer->SendCS_ADDRECALLMON_ACK( (*itREC).second, FALSE, dwTick);

	for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
	{
		CTPlayer *pChar = (*itCHAR).second;

		if( pPlayer != pChar )
		{
			if( pChar->m_bMain && (IsMainCell(pPlayer->m_bChannel) ||
				pChar->m_bChannel == pPlayer->m_bChannel) )
				pPlayer->SendCS_ENTER_ACK( pChar, FALSE, dwTick);

			if( pPlayer->m_bMain && pCELL->IsMainCell(pChar->m_bChannel) )
				pChar->SendCS_ENTER_ACK( pPlayer, bNewMember, dwTick);
		}
	}

	if(IsMainCell(pPlayer->m_bChannel))
	{
		for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
		{
			pPlayer->SendCS_ADDMON_ACK( (*itMON).second, FALSE, dwTick);
			(*itMON).second->OnEvent( AT_ENTER, 0, pPlayer->m_dwID, pPlayer->m_dwID, OT_PC);
		}

		for( itSW = m_mapSWITCH.begin(); itSW != m_mapSWITCH.end(); itSW++)
			pPlayer->SendCS_SWITCHADD_ACK( (*itSW).second->m_dwSwitchID, (*itSW).second->m_bOpened);

		for( itGT = m_mapGATE.begin(); itGT != m_mapGATE.end(); itGT++)
			pPlayer->SendCS_GATEADD_ACK( (*itGT).second->m_dwGateID, (*itGT).second->m_bOpened);

		for( itSELF = m_mapSELFOBJ.begin(); itSELF != m_mapSELFOBJ.end(); itSELF++)
			pPlayer->SendCS_ADDSELFOBJ_ACK( (*itSELF).second, FALSE, dwTick);
	}
}

void CTCell::EnterMonster( CTMonster *pMON,
						   CTCell *pCELL,
						   BYTE bNewMember,
						   DWORD dwTick)
{
	MAPPLAYER::iterator itCHAR;

	switch(pMON->m_bType)
	{
	case OT_RECALL	:
		if(((CTRecallMon *) pMON)->m_bMain)
			for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
				if(pCELL->IsMainCell((*itCHAR).second->m_bChannel))
					(*itCHAR).second->SendCS_ADDRECALLMON_ACK( (CTRecallMon *) pMON, bNewMember, dwTick);

		break;

	case OT_SELF	:
		for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
			if(pCELL->IsMainCell((*itCHAR).second->m_bChannel))
				(*itCHAR).second->SendCS_ADDSELFOBJ_ACK( (CTSelfObj *) pMON, bNewMember, dwTick);

		break;

	default			:
		for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
			if(pCELL->IsMainCell((*itCHAR).second->m_bChannel))
				(*itCHAR).second->SendCS_ADDMON_ACK( pMON, bNewMember, dwTick);

		break;
	}
}

void CTCell::AddSwitch( LPTSWITCH pSwitch)
{
	if( m_mapSWITCH.find(pSwitch->m_dwSwitchID) == m_mapSWITCH.end() )
		m_mapSWITCH.insert( MAPTSWITCH::value_type( pSwitch->m_dwSwitchID, pSwitch));
}

void CTCell::AddGate( LPTGATE pGate)
{
	if( m_mapGATE.find(pGate->m_dwGateID) == m_mapGATE.end() )
		m_mapGATE.insert( MAPTGATE::value_type( pGate->m_dwGateID, pGate));
}

void CTCell::AddPlayer( CTPlayer *pPlayer)
{
	if( m_mapPLAYER.find(pPlayer->m_dwID) == m_mapPLAYER.end() )
		m_mapPLAYER.insert( MAPPLAYER::value_type( pPlayer->m_dwID, pPlayer));
}

void CTCell::AddMonster( CTMonster *pMON)
{
	switch(pMON->m_bType)
	{
	case OT_RECALL	:
		if( m_mapRECALLMON.find(pMON->m_dwID) == m_mapRECALLMON.end() )
			m_mapRECALLMON.insert( MAPTRECALLMON::value_type( pMON->m_dwID, (CTRecallMon *) pMON));

		break;

	case OT_SELF	:
		if( m_mapSELFOBJ.find(pMON->m_dwID) == m_mapSELFOBJ.end() )
			m_mapSELFOBJ.insert( MAPTSELFOBJ::value_type( pMON->m_dwID, (CTSelfObj *) pMON));

		break;

	default			:
		if( m_mapMONSTER.find(pMON->m_dwID) == m_mapMONSTER.end() )
			m_mapMONSTER.insert( MAPTMONSTER::value_type( pMON->m_dwID, pMON));

		break;
	}
}

void CTCell::LeaveChannel( CTPlayer *pPlayer)
{
	MAPTSELFOBJ::iterator itSELF;
	MAPTMONSTER::iterator itMON;

	MAPTSWITCH::iterator itSW;
	MAPTGATE::iterator itGT;

	MAPTGODTOWER::iterator itGTW;
	MAPTGODBALL::iterator  itGB;

	if(IsMainCell(pPlayer->m_bChannel))
		return;

	for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
	{
		if( (*itMON).second->m_dwInvenLock == pPlayer->m_dwID )
		{
			pPlayer->m_dwLockedMonID = 0;
			(*itMON).second->m_dwInvenLock = 0;
		}

		pPlayer->SendCS_DELMON_ACK( (*itMON).second->m_dwID, FALSE);

		(*itMON).second->LeaveAggro(
			pPlayer->m_dwID,
			pPlayer->m_dwID,
			OT_PC);

		(*itMON).second->OnEvent(
			AT_LEAVE, 0,
			pPlayer->m_dwID,
			pPlayer->m_dwID,
			OT_PC);
	}

	for( itSW = m_mapSWITCH.begin(); itSW != m_mapSWITCH.end(); itSW++)
		pPlayer->SendCS_SWITCHDEL_ACK( (*itSW).second->m_dwSwitchID);

	for( itGT = m_mapGATE.begin(); itGT != m_mapGATE.end(); itGT++)
		pPlayer->SendCS_GATEDEL_ACK( (*itGT).second->m_dwGateID);

	for( itSELF = m_mapSELFOBJ.begin(); itSELF != m_mapSELFOBJ.end(); itSELF++)
		pPlayer->SendCS_DELSELFOBJ_ACK( (*itSELF).second->m_dwID, FALSE);
}

void CTCell::LeaveChannel( CTPlayer *pPlayer,
						   BYTE bChannel,
						   BYTE bExitMap)
{
	MAPTRECALLMON::iterator itREC;
	MAPPLAYER::iterator itCHAR;

	for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
	{
		CTPlayer *pChar = (*itCHAR).second;

		if( pChar->m_bChannel != pPlayer->m_bChannel && pChar->m_bChannel == bChannel )
		{
			pChar->SendCS_LEAVE_ACK( pPlayer->m_dwID, bExitMap);
			pPlayer->SendCS_LEAVE_ACK( pChar->m_dwID, FALSE);
		}
	}

	for( itREC = m_mapRECALLMON.begin(); itREC != m_mapRECALLMON.end(); itREC++)
		if( (*itREC).second->m_bMain &&
			(*itREC).second->m_bChannel != pPlayer->m_bChannel &&
			(*itREC).second->m_bChannel == bChannel )
			pPlayer->SendCS_DELRECALLMON_ACK( (*itREC).second->m_dwHostID, (*itREC).second->m_dwID, FALSE);
}

void CTCell::LeaveChannel( CTPlayer *pPlayer,
						   BYTE bExitMap)
{
	MAPTRECALLMON::iterator itREC;
	MAPTSELFOBJ::iterator itSELF;

	MAPTMONSTER::iterator itMON;
	MAPPLAYER::iterator itCHAR;

	MAPTSWITCH::iterator itSW;
	MAPTGATE::iterator itGT;

	for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
	{
		CTPlayer *pChar = (*itCHAR).second;

		if( pPlayer->m_bChannel != pChar->m_bChannel )
		{
			pChar->SendCS_LEAVE_ACK( pPlayer->m_dwID, bExitMap);

			if(!IsMainCell(pPlayer->m_bChannel))
				pPlayer->SendCS_LEAVE_ACK( pChar->m_dwID, FALSE);
		}
	}

	if(!IsMainCell(pPlayer->m_bChannel))
	{
		for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
		{
			if( (*itMON).second->m_dwInvenLock == pPlayer->m_dwID )
			{
				pPlayer->m_dwLockedMonID = 0;
				(*itMON).second->m_dwInvenLock = 0;
			}

			pPlayer->SendCS_DELMON_ACK( (*itMON).second->m_dwID, FALSE);

			(*itMON).second->LeaveAggro(
				pPlayer->m_dwID,
				pPlayer->m_dwID,
				OT_PC);

			(*itMON).second->OnEvent(
				AT_LEAVE, 0,
				pPlayer->m_dwID,
				pPlayer->m_dwID,
				OT_PC);
		}

		for( itREC = m_mapRECALLMON.begin(); itREC != m_mapRECALLMON.end(); itREC++)
			if( (*itREC).second->m_bMain && (*itREC).second->m_bChannel != pPlayer->m_bChannel )
				pPlayer->SendCS_DELRECALLMON_ACK( (*itREC).second->m_dwHostID, (*itREC).second->m_dwID, FALSE);

		for( itSW = m_mapSWITCH.begin(); itSW != m_mapSWITCH.end(); itSW++)
			pPlayer->SendCS_SWITCHDEL_ACK( (*itSW).second->m_dwSwitchID);

		for( itGT = m_mapGATE.begin(); itGT != m_mapGATE.end(); itGT++)
			pPlayer->SendCS_GATEDEL_ACK( (*itGT).second->m_dwGateID);

		for( itSELF = m_mapSELFOBJ.begin(); itSELF != m_mapSELFOBJ.end(); itSELF++)
			pPlayer->SendCS_DELSELFOBJ_ACK( (*itSELF).second->m_dwID, FALSE);
	}
}

void CTCell::LeavePlayer( CTPlayer *pPlayer, BYTE bExitMap)
{
	MAPTRECALLMON::iterator itREC;
	MAPTSELFOBJ::iterator itSELF;

	MAPTMONSTER::iterator itMON;
	MAPPLAYER::iterator itCHAR;

	MAPTSWITCH::iterator itSW;
	MAPTGATE::iterator itGT;

	for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
	{
		CTPlayer *pChar = (*itCHAR).second;

		if(pChar->m_bMain)
			pPlayer->SendCS_LEAVE_ACK( pChar->m_dwID, FALSE);

		if(pPlayer->m_bMain)
			pChar->SendCS_LEAVE_ACK( pPlayer->m_dwID, bExitMap);
	}

	for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
	{
		CTMonster * pMon = (*itMON).second;

		if( pMon->m_dwInvenLock == pPlayer->m_dwID )
		{
			pPlayer->m_dwLockedMonID = 0;
			pMon->m_dwInvenLock = 0;
		}

		pPlayer->SendCS_DELMON_ACK( pMon->m_dwID, FALSE);

		pMon->LeaveAggro(
			pPlayer->m_dwID,
			pPlayer->m_dwID,
			OT_PC);

		pMon->OnEvent(
			AT_LEAVE, 0,
			pPlayer->m_dwID,
			pPlayer->m_dwID,
			OT_PC);
	}

	for( itREC = m_mapRECALLMON.begin(); itREC != m_mapRECALLMON.end(); itREC++)
		if((*itREC).second->m_bMain)
			pPlayer->SendCS_DELRECALLMON_ACK( (*itREC).second->m_dwHostID, (*itREC).second->m_dwID, FALSE);

	for( itSW = m_mapSWITCH.begin(); itSW != m_mapSWITCH.end(); itSW++)
		pPlayer->SendCS_SWITCHDEL_ACK( (*itSW).second->m_dwSwitchID);

	for( itGT = m_mapGATE.begin(); itGT != m_mapGATE.end(); itGT++)
		pPlayer->SendCS_GATEDEL_ACK( (*itGT).second->m_dwGateID);

	for( itSELF = m_mapSELFOBJ.begin(); itSELF != m_mapSELFOBJ.end(); itSELF++)
		pPlayer->SendCS_DELSELFOBJ_ACK( (*itSELF).second->m_dwID, FALSE);
}

void CTCell::LeaveChannel( CTMonster *pMON,
						   BYTE bChannel,
						   BYTE bExitMap)
{
	MAPTMONSTER::iterator itMON;
	MAPPLAYER::iterator itCHAR;

	switch(pMON->m_bType)
	{
	case OT_RECALL	:
		{
			for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
				if( (*itCHAR).second->m_bChannel != pMON->m_bChannel &&
					(*itCHAR).second->m_bChannel == bChannel )
					(*itCHAR).second->SendCS_DELRECALLMON_ACK( pMON->m_dwHostID, pMON->m_dwID, bExitMap);

			for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
			{
				(*itMON).second->LeaveAggro( pMON->m_dwHostID, pMON->m_dwID, pMON->m_bType);
				(*itMON).second->OnEvent( AT_LEAVE, 0, pMON->m_dwHostID, pMON->m_dwID, pMON->m_bType);
			}
		}

		break;

	case OT_SELF	:
		{
			for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
				if( (*itCHAR).second->m_bChannel != pMON->m_bChannel &&
					(*itCHAR).second->m_bChannel == bChannel )
					(*itCHAR).second->SendCS_DELSELFOBJ_ACK( pMON->m_dwID, bExitMap);

			for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
			{
				(*itMON).second->LeaveAggro( pMON->m_dwHostID, pMON->m_dwID, pMON->m_bType);
				(*itMON).second->OnEvent( AT_LEAVE, 0, pMON->m_dwHostID, pMON->m_dwID, pMON->m_bType);
			}
		}

		break;

	default			:
		for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
			if( (*itCHAR).second->m_bChannel != pMON->m_bChannel &&
				(*itCHAR).second->m_bChannel == bChannel )
				(*itCHAR).second->SendCS_DELMON_ACK( pMON->m_dwID, bExitMap);

		break;
	}
}

void CTCell::LeaveChannel( CTMonster *pMON,
						   BYTE bExitMap)
{
	MAPTMONSTER::iterator itMON;
	MAPPLAYER::iterator itCHAR;

	switch(pMON->m_bType)
	{
	case OT_RECALL	:
		{
			for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
				if( (*itCHAR).second->m_bChannel != pMON->m_bChannel )
					(*itCHAR).second->SendCS_DELRECALLMON_ACK( pMON->m_dwHostID, pMON->m_dwID, bExitMap);

			for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
			{
				(*itMON).second->LeaveAggro( pMON->m_dwHostID, pMON->m_dwID, pMON->m_bType);
				(*itMON).second->OnEvent( AT_LEAVE, 0, pMON->m_dwHostID, pMON->m_dwID, pMON->m_bType);
			}
		}

		break;

	case OT_SELF	:
		{
			for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
				if( (*itCHAR).second->m_bChannel != pMON->m_bChannel )
					(*itCHAR).second->SendCS_DELSELFOBJ_ACK( pMON->m_dwID, bExitMap);

			for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
			{
				(*itMON).second->LeaveAggro( pMON->m_dwHostID, pMON->m_dwID, pMON->m_bType);
				(*itMON).second->OnEvent( AT_LEAVE, 0, pMON->m_dwHostID, pMON->m_dwID, pMON->m_bType);
			}
		}

		break;

	default			:
		for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
			if( (*itCHAR).second->m_bChannel != pMON->m_bChannel )
				(*itCHAR).second->SendCS_DELMON_ACK( pMON->m_dwID, bExitMap);

		break;
	}
}

void CTCell::LeaveMonster( CTMonster *pMON, BYTE bExitMap)
{
	MAPTMONSTER::iterator itMON;
	MAPPLAYER::iterator itCHAR;

	switch(pMON->m_bType)
	{
	case OT_RECALL	:
		if(((CTRecallMon *) pMON)->m_bMain)
		{
			for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
				(*itCHAR).second->SendCS_DELRECALLMON_ACK( pMON->m_dwHostID, pMON->m_dwID, bExitMap);

			for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
			{
				(*itMON).second->LeaveAggro( pMON->m_dwHostID, pMON->m_dwID, pMON->m_bType);
				(*itMON).second->OnEvent( AT_LEAVE, 0, pMON->m_dwHostID, pMON->m_dwID, pMON->m_bType);
			}
		}

		break;

	case OT_SELF	:
		{
			for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
				(*itCHAR).second->SendCS_DELSELFOBJ_ACK( pMON->m_dwID, bExitMap);

			for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
			{
				(*itMON).second->LeaveAggro( pMON->m_dwHostID, pMON->m_dwID, pMON->m_bType);
				(*itMON).second->OnEvent( AT_LEAVE, 0, pMON->m_dwHostID, pMON->m_dwID, pMON->m_bType);
			}
		}

		break;

	default			:
		for( itCHAR = m_mapPLAYER.begin(); itCHAR != m_mapPLAYER.end(); itCHAR++)
			(*itCHAR).second->SendCS_DELMON_ACK( pMON->m_dwID, bExitMap);

		break;
	}
}

void CTCell::DelPlayer( DWORD dwID)
{
	MAPPLAYER::iterator finder = m_mapPLAYER.find(dwID);

	if( finder != m_mapPLAYER.end() )
		m_mapPLAYER.erase(finder);
}

void CTCell::DelMonster( CTMonster *pMON)
{
	switch(pMON->m_bType)
	{
	case OT_RECALL	:
		{
			MAPTRECALLMON::iterator finder = m_mapRECALLMON.find(pMON->m_dwID);

			if( finder != m_mapRECALLMON.end() )
				m_mapRECALLMON.erase(finder);
		}

		break;

	case OT_SELF	:
		{
			MAPTSELFOBJ::iterator finder = m_mapSELFOBJ.find(pMON->m_dwID);

			if( finder != m_mapSELFOBJ.end() )
				m_mapSELFOBJ.erase(finder);
		}

		break;

	default			:
		{
			MAPTMONSTER::iterator finder = m_mapMONSTER.find(pMON->m_dwID);

			if( finder != m_mapMONSTER.end() )
				m_mapMONSTER.erase(finder);
		}

		break;
	}
}
