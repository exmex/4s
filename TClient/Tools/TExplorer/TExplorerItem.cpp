#include "StdAfx.h"

CTExplorerItem::CTExplorerItem()
{
	m_pTITEM = NULL;

	m_bItemID = 0;
	m_wItemID = 0;
}

CTExplorerItem::~CTExplorerItem()
{
}

LPTITEMVISUAL CTExplorerItem::GetDefaultVisual( LPTITEM pTITEM)
{
	return CTChart::FindTITEMVISUAL( pTITEM->m_wVisual[0]);
}

LPTITEMVISUAL CTExplorerItem::GetVisual()
{
	return CTChart::FindTITEMVISUAL( m_pTITEM->m_wVisual[0]);
}