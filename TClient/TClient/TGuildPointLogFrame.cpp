#include "stdafx.h"
#include "TGuildPointLogFrame.h"

CTGuildPointLogFrame::CTGuildPointLogFrame( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc)
{
	m_pList = static_cast<TList*>( FindKid( ID_CTRLINST_LIST ) );
}

CTGuildPointLogFrame::~CTGuildPointLogFrame()
{
}