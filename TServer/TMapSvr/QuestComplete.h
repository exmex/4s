// QuestComplete.h: interface for the CQuestComplete class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTCOMPLETE_H__13C0C66D_4208_4F3C_AEE3_A5F12165891B__INCLUDED_)
#define AFX_QUESTCOMPLETE_H__13C0C66D_4208_4F3C_AEE3_A5F12165891B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CQuestComplete : public CQuest
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
	CQuestComplete();
	virtual ~CQuestComplete();
};

#endif // !defined(AFX_QUESTCOMPLETE_H__13C0C66D_4208_4F3C_AEE3_A5F12165891B__INCLUDED_)
