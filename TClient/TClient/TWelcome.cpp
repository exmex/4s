#include "stdafx.h"
#include "TWelcome.h"
#include "THelpFrame.h"
#include "TClientGame.h"

CTWelcome::CTWelcome( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase(pParent, pDesc)
{
	m_pID_CTRLINST_BACK = (TImageList*) FindKid( ID_CTRLINST_BACK );
	m_pID_CTRLINST_TEXT1 = FindKid( ID_CTRLINST_TEXT1 );
	m_pID_CTRLINST_TEXT2 = FindKid( ID_CTRLINST_TEXT2 );
	m_pID_CTRLINST_BTN_START = FindKid( ID_CTRLINST_BTN_START );
	m_pID_CTRLINST_BTN_SKIP = FindKid( ID_CTRLINST_BTN_SKIP );
	m_bMode = 0;
}

CTWelcome::~CTWelcome()
{
}

void CTWelcome::ShowComponent(BOOL bVisible)
{
	CTClientUIBase::ShowComponent(bVisible);

	if(bVisible)
	{
		CTClientUIBase::m_vBasis[TBASISPOINT_CENTER_MIDDLE];
		SIZE size;
		GetComponentSize(&size);
		size.cx /= 2;
		size.cy /= 2;

		MoveComponent( CPoint(
			CTClientUIBase::m_vBasis[TBASISPOINT_CENTER_MIDDLE].x - size.cx,
			CTClientUIBase::m_vBasis[TBASISPOINT_CENTER_MIDDLE].y - size.cy));

		if( m_pID_CTRLINST_BACK ) m_pID_CTRLINST_BACK->SetCurImage( m_bMode );
		if( m_pID_CTRLINST_TEXT1 ) m_pID_CTRLINST_TEXT1->ShowComponent(m_bMode != 0);
		if( m_pID_CTRLINST_TEXT2 ) m_pID_CTRLINST_TEXT2->ShowComponent(m_bMode != 0);
		if( m_pID_CTRLINST_BTN_START ) m_pID_CTRLINST_BTN_START->ShowComponent( m_bMode != 0 );
		if( m_pID_CTRLINST_BTN_SKIP ) m_pID_CTRLINST_BTN_SKIP->ShowComponent( m_bMode != 0 );
	}
	else if( CTClientGame::GetInstance()->GetClientMap()->m_wMapID == TUTORIAL_MAPID  )
	{
		static_cast<CTHelpFrame*>( CTClientGame::GetInstance()->GetFrame(TFRAME_HELP) )->SetHelp(17);
		CTClientGame::GetInstance()->EnableUI(TFRAME_HELP);
		m_bMode = 0;
	}
}

void CTWelcome::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if( m_pID_CTRLINST_BTN_START && m_pID_CTRLINST_BTN_START->HitTest(pt) )
	{
		m_pID_CTRLINST_BTN_START->OnLButtonUp(nFlags, pt);
		return ;
	}

	if( m_pID_CTRLINST_BTN_SKIP && m_pID_CTRLINST_BTN_SKIP->HitTest(pt) )
	{
		m_pID_CTRLINST_BTN_SKIP->OnLButtonUp(nFlags, pt);
		return ;
	}

	if( HitTest(pt) )
	{
		if( m_bMode == 0 )
		{
			m_bMode = 1;
			ShowComponent(TRUE);
		}
		else
			ShowComponent(FALSE);
	}
}