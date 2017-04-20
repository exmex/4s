// QuestDropItem.h: interface for the CQuestDropItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTDROPITEM_H__D4BC71B5_DC7C_4F85_9EA1_8F58A37C7DF2__INCLUDED_)
#define AFX_QUESTDROPITEM_H__D4BC71B5_DC7C_4F85_9EA1_8F58A37C7DF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CQuestDropItem : public CQuest
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
	CQuestDropItem();
	virtual ~CQuestDropItem();
};

#endif // !defined(AFX_QUESTDROPITEM_H__D4BC71B5_DC7C_4F85_9EA1_8F58A37C7DF2__INCLUDED_)
