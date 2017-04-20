// QuestGiveSkill.h: interface for the CQuestGiveSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTGIVESKILL_H__467AAFDD_0C80_4237_BEAE_91EDA9B380EE__INCLUDED_)
#define AFX_QUESTGIVESKILL_H__467AAFDD_0C80_4237_BEAE_91EDA9B380EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CQuestGiveSkill : public CQuest
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
	CQuestGiveSkill();
	virtual ~CQuestGiveSkill();
};

#endif // !defined(AFX_QUESTGIVESKILL_H__467AAFDD_0C80_4237_BEAE_91EDA9B380EE__INCLUDED_)
