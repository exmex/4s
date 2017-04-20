#include "StdAfx.h"


CTServer::CTServer()
{
	m_mapCHANNEL.clear();
	m_wID = 0;
	m_bCashSale = FALSE;  
}

CTServer::~CTServer()
{
	m_mapCHANNEL.clear();
}

void CTServer::WrapItem(CPacket * pPacket, LPTITEM pItem)
{
	(*pPacket)
		<< pItem->m_dlID
		<< pItem->m_bItemID
		<< pItem->m_wItemID
		<< pItem->m_bLevel
		<< pItem->m_bCount
		<< pItem->m_bGLevel
		<< pItem->m_dwDuraMax
		<< pItem->m_dwDuraCur
		<< pItem->m_bRefineCur
		<< pItem->m_dEndTime
		<< pItem->m_bGradeEffect
		<< pItem->m_dwExtValue[IEV_ELD]
		<< pItem->m_dwExtValue[IEV_WRAP]
		<< pItem->m_dwExtValue[IEV_COLOR]
		<< pItem->m_dwExtValue[IEV_GUILD]
		<< pItem->m_bMagicCount;

	for(BYTE i=0; i<pItem->m_bMagicCount; i++)
	{
		(*pPacket)
			<< pItem->m_bMagic[i]
			<< pItem->m_wValue[i];
	}
}
