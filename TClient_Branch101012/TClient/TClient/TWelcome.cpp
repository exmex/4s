#include "stdafx.h"
#include "TWelcome.h"
#include "THelpFrame.h"
#include "TClientGame.h"

CTWelcome::CTWelcome( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase(pParent, pDesc)
{
}

CTWelcome::~CTWelcome()
{
}

void CTWelcome::ShowComponent(BOOL bVisible)
{
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
	}
	else if( CTClientGame::GetInstance()->GetClientMap()->m_wMapID == TUTORIAL_MAPID  )
	{
		static_cast<CTHelpFrame*>( CTClientGame::GetInstance()->GetFrame(TFRAME_HELP) )->SetHelp(17);
		CTClientGame::GetInstance()->EnableUI(TFRAME_HELP);
	}

	CTClientUIBase::ShowComponent(bVisible);
}

void CTWelcome::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if( HitTest(pt) )
		ShowComponent(FALSE);
}