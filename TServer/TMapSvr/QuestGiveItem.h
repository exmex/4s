// QuestGiveItem.h: interface for the CQuestGiveItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTGIVEITEM_H__2F03FD57_B3DE_4693_8661_0DD5EEBDDD13__INCLUDED_)
#define AFX_QUESTGIVEITEM_H__2F03FD57_B3DE_4693_8661_0DD5EEBDDD13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CQuestGiveItem : public CQuest
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
	CQuestGiveItem();
	virtual ~CQuestGiveItem();
};

#endif // !defined(AFX_QUESTGIVEITEM_H__2F03FD57_B3DE_4693_8661_0DD5EEBDDD13__INCLUDED_)
