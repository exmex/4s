#include "StdAfx.h"
#include ".\questtalk.h"

CQuestTalk::CQuestTalk(void)
{
}

CQuestTalk::~CQuestTalk(void)
{
}

void CQuestTalk::ExecQuest(CTPlayer *pPlayer,
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
		pPlayer->SendCS_NPCTALK_ACK(m_pQUEST->m_dwQuestID, WORD(m_dwMonID));
	else
		pPlayer->SendCS_NPCTALK_ACK(0, WORD(m_dwMonID));
}
