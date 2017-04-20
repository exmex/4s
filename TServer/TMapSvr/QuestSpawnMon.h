// QuestSpawnMon.h: interface for the CQuestSpawnMon class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "quest.h"

class CQuestSpawnMon : public CQuest
{
public:
	virtual void ExecQuest(
		CTPlayer *pPlayer,
		DWORD dwTick,
		LPMAPMAPVQUESTTEMP pTRIGGER,
		LPMAPQUESTTEMP pQUESTTEMP,
		LPMAPTITEMTEMP pITEMTEMP,
		LPMAPTSKILLTEMP pSKILLTEMP,
		LPMAPTMONSTERTEMP pMONTEMP,
		LPMAPCLASS pCLASS);

public:
	CQuestSpawnMon();
	virtual ~CQuestSpawnMon();
};
