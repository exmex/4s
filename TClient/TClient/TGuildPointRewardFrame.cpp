#include "stdafx.h"
#include "TGuildPointRewardFrame.h"

CTGuildPointRewardFrame::CTGuildPointRewardFrame( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc)
{
	TComponent* pMsgTplt = FindKid(ID_CTRLINST_EDIT_MESSAGE);
	m_pMessage = new TMultiLineEdit(this,pMsgTplt->m_pDESC);

	RemoveKid(pMsgTplt);
	AddKid(m_pMessage);

	m_pMessage->m_id = pMsgTplt->m_id;
	delete pMsgTplt;

	m_pTargetEdit = (TEdit*) FindKid( ID_CTRLINST_EDIT_TARGET );
	m_pPointEdit = (TEdit*) FindKid( ID_CTRLINST_EDIT_POINT );
	m_pGuildPoint = FindKid( ID_CTRLINST_GUILDPOINT );
}

CTGuildPointRewardFrame::~CTGuildPointRewardFrame()
{
}

TEdit* CTGuildPointRewardFrame::GetCurEdit()
{
	if( !IsVisible() )
		return NULL;

	if( m_pTargetEdit->IsVisible() && m_pTargetEdit->GetFocus() )
		return m_pTargetEdit;

	if( m_pPointEdit->IsVisible() && m_pPointEdit->GetFocus() )
		return m_pPointEdit;

	if( m_pMessage->IsVisible() && m_pMessage->GetFocus() )
		return m_pMessage;

	return NULL;
}

void CTGuildPointRewardFrame::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	if(!CanProcess()) 
		return;

	if( nChar == VK_RETURN && m_pMessage->GetFocus() )
	{
		m_pMessage->InsertReturnChar();
		m_pMessage->MoveScrollToCaret(FALSE);
		return ;
	}

	CTClientUIBase::OnKeyDown(nChar,nRepCnt,nFlags);
}