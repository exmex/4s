#pragma once
#include "quest.h"

class CQuestDieMon :
	public CQuest
{
public:
	CQuestDieMon(void);
	~CQuestDieMon(void);
	virtual void ExecQuest(
		CTPlayer *pPlayer,
		DWORD dwTick,
		LPMAPMAPVQUESTTEMP pTRIGGER,
		LPMAPQUESTTEMP pQUESTTEMP,
		LPMAPTITEMTEMP pITEMTEMP,
		LPMAPTSKILLTEMP pSKILLTEMP,
		LPMAPTMONSTERTEMP pMONTEMP,
		LPMAPCLASS pCLASS);
};
