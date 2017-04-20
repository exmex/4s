// Quest.cpp: implementation of the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuest::CQuest()
{
	m_pQUEST = NULL;
	m_pMap = NULL;
	m_vTERM.clear();

	m_bCompleteCount = 0;
	m_bTriggerCount = 0;

	m_dwBeginTick = 0;
	m_dwTick = 0;

	m_bRewardType = 0;
	m_dwRewardID = 0;

	m_dwMonID = 0;
	m_fPosX = 0;
	m_fPosY = 0;
	m_fPosZ = 0;
	m_bSave = FALSE;  
}

CQuest::~CQuest()
{
	while(!m_vTERM.empty())
	{
		delete m_vTERM.back();
		m_vTERM.pop_back();
	}
	m_vTERM.clear();
}

CQuest *CQuest::CreateQuest( LPQUESTTEMP pQUEST)
{
	CQuest *pResult = NULL;

	switch(pQUEST->m_bType)
	{
	case QT_DEFTALK		: pResult = new CQuestDefaultTalk(); break;
	case QT_GIVESKILL	: pResult = new CQuestGiveSkill(); break;
	case QT_GIVEITEM	: pResult = new CQuestGiveItem(); break;
	case QT_DROPITEM	: pResult = new CQuestDropItem(); break;
	case QT_SPAWNMON	: pResult = new CQuestSpawnMon(); break;
	case QT_TELEPORT	: pResult = new CQuestTeleport(); break;
	case QT_COMPLETE	: pResult = new CQuestComplete(); break;
	case QT_MISSION		: pResult = new CQuestMission(); break;
	case QT_ROUTING		: pResult = new CQuestRouting(); break;
	case QT_NPCTALK		: pResult = new CQuestTalk(); break;
	case QT_DROPQUEST	: pResult = new CQuestDrop(); break;
	case QT_CHAPTERMSG	: pResult = new CQuestChapterMsg(); break;
	case QT_SWITCH		: pResult = new CQuestSwitch(); break;
	case QT_DIEMON		: pResult = new CQuestDieMon(); break;
	case QT_DEFENDSKILL	: pResult = new CQuestDefendSkill(); break;
	case QT_DELETEITEM  : pResult = new CQuestDeleteItem(); break;
	case QT_SENDPOST	: pResult = new CQuestSendPost();	break;
	case QT_CRAFT		: pResult = new CQuestCraft(); break;  
	case QT_REGEN		: pResult = new CQuestRegen(); break;
	default				: pResult = new CQuest(); break;
	}
	pResult->m_pQUEST = pQUEST;

	return pResult;
}

void CQuest::ExecQuest( CTPlayer *pPlayer,
					    DWORD dwTick,
					    LPMAPMAPVQUESTTEMP pTRIGGER,
						LPMAPQUESTTEMP pQUESTTEMP,
						LPMAPTITEMTEMP pITEMTEMP,
						LPMAPTSKILLTEMP pSKILLTEMP,
						LPMAPTMONSTERTEMP pMONTEMP,
						LPMAPCLASS pCLASS)
{
	MAPMAPVQUESTTEMP::iterator itTRIGGER = pTRIGGER->find(TT_EXECQUEST);

	if( itTRIGGER != pTRIGGER->end() )
	{
		MAPVQUESTTEMP::iterator itVQUEST = (*itTRIGGER).second->find(m_pQUEST->m_dwQuestID);
		BYTE bLevel;
		if( itVQUEST != (*itTRIGGER).second->end() )
			for( int i=0; i<INT((*itVQUEST).second->size()); i++)
				if(!pPlayer->CanRunQuest( (*(*itVQUEST).second)[i], dwTick, bLevel))
				{
					CQuest *pQuest = CreateQuest((*(*itVQUEST).second)[i]);

					pQuest->ExecQuest(
						pPlayer,
						dwTick,
						pTRIGGER,
						pQUESTTEMP,
						pITEMTEMP,
						pSKILLTEMP,
						pMONTEMP,
						pCLASS);

					delete pQuest;
				}
	}
}

LPQUESTTERM CQuest::CheckComplete( CTPlayer *pPlayer, DWORD dwTick)
{
	for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
		if( CheckComplete( pPlayer, dwTick, m_pQUEST->m_vTerm[i]) != QTS_SUCCESS )
			return m_pQUEST->m_vTerm[i];

	return NULL;
}

BYTE CQuest::CheckComplete( CTPlayer *pPlayer,
						    DWORD dwTick,
							LPQUESTTERM pTERM)
{
	switch(pTERM->m_bTermType)
	{
	case QTT_GETITEM	:
		{
			MAPTINVEN::iterator itInven;
			MAPTITEM::iterator itItem;
			WORD wCount = 0;

			for( itInven = pPlayer->m_mapTINVEN.begin(); itInven != pPlayer->m_mapTINVEN.end(); itInven++)
			{
				for(itItem=(*itInven).second->m_mapTITEM.begin(); itItem!=(*itInven).second->m_mapTITEM.end(); itItem++)
				{
					if((*itItem).second->m_wItemID == pTERM->m_dwTermID)
						wCount += (*itItem).second->m_bCount;
				}
			}

			if( wCount < pTERM->m_bCount || !pTERM->m_bCount)
				return QTS_RUN;
		}

		break;

	case QTT_TALK		:
	case QTT_HUNT		:
	case QTT_USEITEM	:
		{
			LPQUESTTERM pTerm = FindRunningTerm(
				pTERM->m_dwTermID,
				pTERM->m_bTermType);

			if( !pTerm || pTerm->m_bCount < pTERM->m_bCount )
				return QTS_RUN;
		}

		break;

	case QTT_TIMER		:
		if( m_dwBeginTick && dwTick - m_dwBeginTick > m_dwTick )
			return QTS_FAILED;

		break;
	case QTT_LEFT:
		if(pPlayer->m_fPosX < pTERM->m_dwTermID)
			return QTS_RUN;

		break;
	case QTT_TOP:
		if(pPlayer->m_fPosZ > pTERM->m_dwTermID)
			return QTS_RUN;

		break;
	case QTT_RIGHT:
		if(pPlayer->m_fPosX > pTERM->m_dwTermID)
			return QTS_RUN;

		break;
	case QTT_BOTTOM:
		if(pPlayer->m_fPosZ < pTERM->m_dwTermID)
			return QTS_RUN;

		break;

	case QTT_QUESTCOMPLETED:
		{
			CQuest * pCompQ = pPlayer->FindQuest(pTERM->m_dwTermID);
			if(!pCompQ || pCompQ->m_bCompleteCount < pTERM->m_bCount)
				return QTS_RUN;
		}
		break;
	default:
		break;
	}

	return QTS_SUCCESS;
}

LPQUESTTERM CQuest::FindRunningTerm( DWORD dwTermID, BYTE bType)
{
	for( int i=0; i<INT(m_vTERM.size()); i++)
		if( m_vTERM[i]->m_dwTermID == dwTermID &&
			m_vTERM[i]->m_bTermType == bType )
			return m_vTERM[i];

	LPQUESTTERM pTERM = NULL;
	switch(bType)
	{
	case QTT_HUNT	:
	case QTT_TALK	:
	case QTT_USEITEM:
		{
			pTERM = new QUESTTERM();

			pTERM->m_dwTermID = dwTermID;
			pTERM->m_bTermType = bType;
			pTERM->m_bCount = 0;

			m_vTERM.push_back(pTERM);
		}
		break;
	}

	return pTERM;
}

LPQUESTTERM CQuest::FindTerm( DWORD dwTermID, BYTE bType)
{
	for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
		if( m_pQUEST->m_vTerm[i]->m_dwTermID == dwTermID &&
			m_pQUEST->m_vTerm[i]->m_bTermType == bType )
			return m_pQUEST->m_vTerm[i];

	return NULL;
}
