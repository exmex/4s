// QuestTeleport.cpp: implementation of the CQuestTeleport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestTeleport::CQuestTeleport()
{
}

CQuestTeleport::~CQuestTeleport()
{
}

void CQuestTeleport::ExecQuest( CTPlayer *pPlayer,
							    DWORD dwTick,
							    LPMAPMAPVQUESTTEMP pTRIGGER,
								LPMAPQUESTTEMP pQUESTTEMP,
								LPMAPTITEMTEMP pITEMTEMP,
								LPMAPTSKILLTEMP pSKILLTEMP,
								LPMAPTMONSTERTEMP pMONTEMP,
								LPMAPCLASS pCLASS)
{
	BYTE bLevel;
	if(!pPlayer->CanRunQuest( m_pQUEST, dwTick, bLevel))
	{
		FLOAT fPosX = 0;
		FLOAT fPosY = 0;
		FLOAT fPosZ = 0;
		WORD wMapID = 0;

		for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
			switch(m_pQUEST->m_vTerm[i]->m_bTermType)
			{
			case QTT_MAPID	: wMapID = (WORD) m_pQUEST->m_vTerm[i]->m_dwTermID; break;
			case QTT_LEFT	: fPosX = (FLOAT) m_pQUEST->m_vTerm[i]->m_dwTermID; break;
			case QTT_HEIGHT	: fPosY = (FLOAT) m_pQUEST->m_vTerm[i]->m_dwTermID; break;
			case QTT_TOP	: fPosZ = (FLOAT) m_pQUEST->m_vTerm[i]->m_dwTermID; break;
			}

		_AtlModule.Teleport(
			pPlayer,
			pPlayer->m_bChannel,
			wMapID,
			fPosX,
			fPosY,
			fPosZ);

		CQuest::ExecQuest(
			pPlayer,
			dwTick,
			pTRIGGER,
			pQUESTTEMP,
			pITEMTEMP,
			pSKILLTEMP,
			pMONTEMP,
			pCLASS);
	}
}
