#include "StdAfx.h"


CTClientMaintain::CTClientMaintain()
{
	m_dwAttackID = 0;
	m_dwHostID = 0;
	m_dwTIMMER = 0;
	m_dwEndTick = 0;

	m_bUnlimitedType = FALSE;
	m_bAttackType = OT_NONE;
	m_bHostType = OT_NONE;
	m_pTIMMER = NULL;
}

CTClientMaintain::~CTClientMaintain()
{
}

DWORD CTClientMaintain::GetLeftTick( DWORD dwTick)
{
	return m_dwEndTick > dwTick ? m_dwEndTick - dwTick : 0;
}
