#include "Stdafx.h"
#include "TMsgChatMemberList.h"
#include "TMessengerChat.h"
#include "Resource.h"

/*// ======================================================================
CTMsgChatMemberList::CTMsgChatMemberList(TComponent *pParent, LP_FRAMEDESC pDesc)
	: ITInnerFrame(pParent,pDesc,NULL)
{
	m_pScroll = static_cast<TScroll*>(FindKid(ID_CTRLINST_SCROLL));
	m_pScroll->SetScrollPos(0,0);
	
	m_vLine[0] = FindKid(ID_CTRLINST_LISTITEM_1);
	m_vLine[0]->m_strText.Empty();

	m_vLine[1] = FindKid(ID_CTRLINST_LISTITEM_2);
	m_vLine[1]->m_strText.Empty();

	m_vLine[2] = FindKid(ID_CTRLINST_LISTITEM_3);
	m_vLine[2]->m_strText.Empty();
}
// ----------------------------------------------------------------------
CTMsgChatMemberList::~CTMsgChatMemberList()
{
}
// ======================================================================

// ======================================================================
void CTMsgChatMemberList::Add(CTMessengerChat* pChat, BOOL bNewMsg)
{
	Item item;
	item.bNewMsg = bNewMsg;
	item.pChat = pChat;

	m_vItems.push_back(item);

	INT nCnt = GetCount();
	if( nCnt > MAX_LINE )
	{
		INT nScrPos, nScrRng;
		m_pScroll->GetScrollPos(nScrPos, nScrRng);

		nScrPos = nScrRng = nCnt - MAX_LINE;
		m_pScroll->SetScrollPos(nScrPos, nScrRng);
	}
}
// ----------------------------------------------------------------------
void CTMsgChatMemberList::NotifyNewMsg(INT nIndex)
{
	m_vItems[nIndex].bNewMsg = TRUE;
}
// ======================================================================
void CTMsgChatMemberList::Remove(INT nIndex)
{
	ItemArray::iterator itr;
	
	itr = m_vItems.begin() + nIndex;
	m_vItems.erase(itr);

	INT nScrPos, nScrRng;
	m_pScroll->GetScrollPos(nScrPos, nScrRng);

	INT nCnt = GetCount();
	if( nCnt > nScrRng+MAX_LINE )
	{
		nScrRng = nCnt - MAX_LINE;
		if( nScrRng < 0 )
			nScrRng = 0;

		if( nScrRng < nScrPos )
			nScrPos = nScrRng;

		m_pScroll->SetScrollPos(nScrPos, nScrRng);
	}
}
// ----------------------------------------------------------------------
void CTMsgChatMemberList::Clear()
{
	m_vItems.clear();
	m_pScroll->SetScrollPos(0, 0);
}
// ======================================================================
BOOL CTMsgChatMemberList::IsEmpty() const
{
	return m_vItems.empty();
}
// ----------------------------------------------------------------------
INT CTMsgChatMemberList::GetCount() const
{
	return (INT)m_vItems.size();
}
// ======================================================================
CTMessengerChat* CTMsgChatMemberList::GetChat(INT nIndex) const
{
	return const_cast<CTMessengerChat*>( m_vItems[nIndex].pChat );
}
// ======================================================================
INT CTMsgChatMemberList::FindByChat(CTMessengerChat* pChat)
{
	for(size_t i=0; i<m_vItems.size(); ++i)
	{
		if( m_vItems[i].pChat == pChat )
			return (INT)i;
	}

	return T_INVALID;
}
// ----------------------------------------------------------------------
INT CTMsgChatMemberList::FindByTargetName(const CString& strTarget)
{
	for(size_t i=0; i<m_vItems.size(); ++i)
	{
		if( m_vItems[i].pChat->FindTargetByName(strTarget) )
			return (INT)i;
	}

	return T_INVALID;
}
// ======================================================================
void CTMsgChatMemberList::Update(DWORD dwTickCount)
{
	static DWORD dwTotalTick = 0;
	dwTotalTick += dwTickCount;

	INT nScrRng,nScrPos;
	m_pScroll->GetScrollPos(nScrRng,nScrPos);

	INT nMemberCnt = GetCount();
	if( nMemberCnt > MAX_LINE )
		nScrRng = nMemberCnt - MAX_LINE;

	m_pScroll->SetScrollPos(nScrRng,nScrPos);

	for(INT i=0; i<MAX_LINE; ++i)
	{
		m_vLine[i]->m_strText.Empty();

		INT nIndex = i + nScrPos;
		if( nMemberCnt > nIndex )
		{
			if( m_vItems[i].bNewMsg )
			{
				if( dwTotalTick%1000 > 500 )
					continue;
			}

			CTMessengerChat* pChat = m_vItems[i].pChat;
			INT nTCnt = pChat->GetTargetCount();

			CString strTxt;
			if( nTCnt >= 1 )
			{
				const CString& strTarget = pChat->GetTarget(0)->strName;
				
				if( nTCnt == 1 )
					m_vLine[i]->m_strText = strTarget;
				else
					m_vLine[i]->m_strText = CTChart::Format( TSTR_FMT_MSGHAT_TITLE, (LPCSTR)strTarget, nTCnt);
			}
		}
	}
}
// ======================================================================

// ======================================================================
HRESULT CTMsgChatMemberList::Render(DWORD dwTickCount)
{
	Update(dwTickCount);

	return ITInnerFrame::Render(dwTickCount);
}
// ======================================================================
void CTMsgChatMemberList::OnLButtonDown(UINT nFlags, CPoint pt)
{
	INT nScrRng,nScrPos;
	m_pScroll->GetScrollPos(nScrRng,nScrPos);

	for(INT i=0; i<MAX_LINE; ++i)
	{
		TComponent* pComp = m_vLine[i];
		if( pComp->HitTest(pt) )
		{
			INT nSelIdx = nScrPos+i;
			if( (INT)m_vItems.size() > nSelIdx )
			{
				m_vItems[i].bNewMsg = FALSE;
				m_vItems[i].pChat->ShowComponent(TRUE);

				TComponent* pParent = m_vItems[i].pChat->GetParent();
				pParent->RemoveKid(m_vItems[i].pChat);
				pParent->AddKid(m_vItems[i].pChat);
			}
			
			break;
		}
	}
}
// ======================================================================*/