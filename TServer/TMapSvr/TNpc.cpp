#include "StdAfx.h"
#include ".\tnpc.h"
#include "TMapSvr.h" 
#include "TMapSvrModule.h"

CTNpc::CTNpc(void)
{
	m_pLocal = NULL;
	m_wSpawnPosID = 0;
	m_wItemID = 0;
	m_pPortal =	NULL;
	m_pArena = NULL;
    m_mapItem.clear();
	m_mapMagicItem.clear();
	m_mapSkill.clear();
	m_mapQuest.clear();
	m_mapMon.clear();
}

CTNpc::~CTNpc(void)
{
	MAPTMONSTERSHOP::iterator it;
	for(it=m_mapMon.begin(); it!=m_mapMon.end(); it++)
		delete (*it).second;
	m_mapMon.clear();

	
	MAPMAPTAUCTION::iterator itF;
	for( itF = m_mapTAuctionFind.begin(); itF != m_mapTAuctionFind.end(); itF++)
		delete (*itF).second;
	m_mapTAuctionFind.clear();

	MAPTAUCTION::iterator itA;
	for( itA = m_mapTAuction.begin(); itA != m_mapTAuction.end(); itA++)
		delete (*itA).second;
	m_mapTAuction.clear();
}

LPTITEM CTNpc::GetItem(WORD wID)
{
	MAPTITEMTEMP::iterator it = m_mapItem.find(wID);
	if(it!=m_mapItem.end())
		return (*it).second;
	else
		return NULL;
}

CTItem* CTNpc::GetMagicItem(DWORD dwID)
{
	MAPTSTORAGEITEM::iterator it = m_mapMagicItem.find(dwID);
	if(it!=m_mapMagicItem.end())
		return (*it).second;
	else
		return NULL;
}


CTSkillTemp * CTNpc::GetSkill(WORD wSkillID)
{
	MAPTSKILLTEMP::iterator it = m_mapSkill.find(wSkillID);
	if(it!=m_mapSkill.end())
		return (*it).second;
	else
		return NULL;
}

LPTPORTAL CTNpc::GetPortal()
{
	if(m_pPortal)
		return m_pPortal;
	else
		return NULL;
}

WORD CTNpc::GetSpawnPos()
{
	return m_wSpawnPosID;
}

BYTE CTNpc::CanTalk(BYTE bCountry, BYTE bAidCountry, BYTE bDisguise)
{
	if(m_bCountry != TCONTRY_N)
		return	(bCountry == m_bCountry || bAidCountry == m_bCountry) &&
				(!bDisguise || bDisguise == m_bCountry + SDT_TRANS_DISGUISE_D);
	else
	{
		BYTE bNpcCountry = m_pLocal ? m_pLocal->m_bCountry : m_bCountry;
		return  bNpcCountry == TCONTRY_N ||
				((bCountry == bNpcCountry || bAidCountry == bNpcCountry)
				  && (!bDisguise || bDisguise == bNpcCountry + SDT_TRANS_DISGUISE_D)) ||
				((bCountry != bNpcCountry && bAidCountry != bNpcCountry)
				  && bDisguise == bNpcCountry + SDT_TRANS_DISGUISE_D);
	}
}

LPTMONSTERSHOP CTNpc::GetMon(WORD wID)
{
	MAPTMONSTERSHOP::iterator it = m_mapMon.find(wID);
	if(it!=m_mapMon.end())
		return (*it).second;
	else
		return NULL;
}

BYTE CTNpc::AddAuction(DWORD dwAuctionID, LPTAUCTION pAuction)
{
	MAPTAUCTION::iterator itA = m_mapTAuction.find(pAuction->m_dwAuctionID);
	if(itA == m_mapTAuction.end())				
		m_mapTAuction.insert(MAPTAUCTION::value_type(pAuction->m_dwAuctionID, pAuction));
	else
		return FALSE;

	// 직업 세분화
	BYTE bType  = AUCTIONTREE1(pAuction->m_dwCode);
	BYTE bClass = AUCTIONTREE2(pAuction->m_dwCode);
	BYTE bKind  = AUCTIONTREE3(pAuction->m_dwCode);
	for(BYTE i = 0; i < TCLASS_COUNT; i++ )
	{
		DWORD dwFindCode = 0;
		if( (DWORD)bClass & BITSHIFTID(i) )		
			dwFindCode = MAKEAUCTIONCODE(bType,BITSHIFTID(i),bKind);
		else if( bClass == 0)
			dwFindCode = MAKEAUCTIONCODE(bType,0,bKind);
		
		if(dwFindCode == 0 )
			continue;

		LPMAPTAUCTION pmapAuction = NULL;
		MAPMAPTAUCTION::iterator itF = m_mapTAuctionFind.find(dwFindCode);
		if( itF == m_mapTAuctionFind.end() )
		{
			pmapAuction = new MAPTAUCTION();
			m_mapTAuctionFind.insert(MAPMAPTAUCTION::value_type(dwFindCode,pmapAuction));
		}
		else
		{
			pmapAuction = (*itF).second;
		}

		MAPTAUCTION::iterator itD = pmapAuction->find(dwAuctionID);
		if( itD != pmapAuction->end())			
			pmapAuction->erase(itD);		

		pmapAuction->insert(MAPTAUCTION::value_type(dwAuctionID,pAuction));

		if(bClass == 0)
			break;
	}
	return TRUE;
}

void CTNpc::DelAuction(DWORD dwAuctionID,DWORD dwCode)
{
	if(!dwAuctionID)
		return;

	// 직업 세분화
	BYTE bType  = AUCTIONTREE1(dwCode);
	BYTE bClass = AUCTIONTREE2(dwCode);
	BYTE bKind  = AUCTIONTREE3(dwCode);

	for(BYTE i = 0; i < TCLASS_COUNT; i++ )
	{
		DWORD dwFindCode = 0;

		if( (DWORD)bClass & BITSHIFTID(i))
			dwFindCode = MAKEAUCTIONCODE(bType,BITSHIFTID(i),bKind);
		else if(bClass == 0 )
			dwFindCode = MAKEAUCTIONCODE(bType,0,bKind);
		
		if(dwFindCode == 0)
			continue;

		MAPMAPTAUCTION::iterator itF = m_mapTAuctionFind.find(dwFindCode);
		if( itF == m_mapTAuctionFind.end() )
			continue;

		MAPTAUCTION::iterator itD = (*itF).second->find(dwAuctionID);
		if( itD != (*itF).second->end())
			(*itF).second->erase(itD);

		if(bClass == 0)
			break;
	}

	MAPTAUCTION::iterator itA = m_mapTAuction.find(dwAuctionID);
	if( itA != m_mapTAuction.end())
	{
		delete (*itA).second->m_pItem;
		delete (*itA).second;
		m_mapTAuction.erase(itA);
	}
}

BYTE CTNpc::AuctionCondition(LPTAUCTION pAuction,BYTE bMinWearLv,BYTE bMaxWearLv,BYTE bMinPowerLv,BYTE bMaxPowerLv,BYTE bOption,WORD wItemID)
{
	if(!pAuction)
		return FALSE;
	
	BYTE bWearLv = pAuction->m_pItem->GetEquipLevel();
	BYTE bPowerLv = pAuction->m_pItem->m_bLevel;

	if( (bMinWearLv > bWearLv || bWearLv > bMaxWearLv) ||
		(bMinPowerLv > bPowerLv || bPowerLv > bMaxPowerLv ) )
		return FALSE;

	if(bOption)
	{
		MAPTMAGIC::iterator itM = pAuction->m_pItem->m_mapTMAGIC.find(bOption);
		if( itM == pAuction->m_pItem->m_mapTMAGIC.end() )
			return FALSE;
	}

	if(wItemID && wItemID != pAuction->m_pItem->m_pTITEM->m_wItemID )
		return FALSE;

	return TRUE;
}

void CTNpc::SearchAuctions(LPMAPDWORD pmapDWORD, DWORD dwCode, BYTE bMinWearLv,BYTE bMaxWearLv,BYTE bMinPowerLv,BYTE bMaxPowerLv,BYTE bOption,WORD wItemID)
{	
	MAPMAPTAUCTION::iterator itF = m_mapTAuctionFind.find(dwCode);
	if( itF == m_mapTAuctionFind.end() )
		return;

	LPMAPTAUCTION pmapTAuction= (*itF).second;
	for(MAPTAUCTION::iterator itD = pmapTAuction->begin(); itD != pmapTAuction->end(); itD++)
	{
		LPTAUCTION pAuction = (*itD).second;
		if(!pAuction)
			return;

		if(!AuctionCondition(pAuction,bMinWearLv,bMaxWearLv,bMinPowerLv,bMaxPowerLv,bOption,wItemID) )
			continue;

		if(pAuction->m_dEnd > _AtlModule.m_timeCurrent)
			pAuction->m_dwRemainTime = DWORD(pAuction->m_dEnd - _AtlModule.m_timeCurrent);
		else
			pAuction->m_dwRemainTime = 0;
		
		MAPDWORD::iterator itMD = pmapDWORD->find(pAuction->m_dwAuctionID);
		if( itMD == pmapDWORD->end() )
			pmapDWORD->insert(MAPDWORD::value_type(pAuction->m_dwAuctionID,pAuction->m_dwAuctionID));
	}
}

void CTNpc::SearchAuctionAll(LPVTAUCTION pVTAUCTION,BYTE bMinWearLv,BYTE bMaxWearLv,BYTE bMinPowerLv,BYTE bMaxPowerLv,BYTE bOption,WORD wItemID)
{
	MAPTAUCTION::iterator it;
	for(it = m_mapTAuction.begin(); it != m_mapTAuction.end(); it++)
	{
		if(!AuctionCondition( (*it).second,bMinWearLv,bMaxWearLv,bMinPowerLv,bMaxPowerLv,bOption,wItemID))
			continue;

		if((*it).second->m_dEnd > _AtlModule.m_timeCurrent)
			(*it).second->m_dwRemainTime = DWORD((*it).second->m_dEnd - _AtlModule.m_timeCurrent);
		else
			(*it).second->m_dwRemainTime = 0;

		pVTAUCTION->push_back((*it).second);
	}
}

void CTNpc::AlignAuction(LPVTAUCTION pVTAUCTION, BYTE bAlignKind)
{
	switch(bAlignKind)
	{
	case AAK_AUCTIONIDASC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),AuctionIDASC());	 break;
	case AAK_AUCTIONIDDESC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),AuctionIDDESC());	 break;
	case AAK_ENDTIMEASC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),EndTimeASC());	 break;
	case AAK_ENDTIMEDESC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),EndTimeDESC());	 break;
	case AAK_REMAINTIMEASC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),RemainTimeASC());  break;
	case AAK_REMAINTIMEDESC: std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),RemainTimeDESC()); break;
	case AAK_BIDPRICEASC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),BidPriceASC());	 break;
	case AAK_BIDPRICEDESC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),BidPriceDESC());	 break;
	case AAK_DIRECTPRICEASC: std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),DirectPriceASC()); break;
	case AAK_DIRECTPRICEDESC:std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),DirectPriceDESC());break;
	case AAK_BIDCOUNTASC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),BidCountASC());	 break;
	case AAK_BIDCOUNTDESC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),BidCountDESC());	 break;
	case AAK_CLASSASC:		 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),ClassASC());		 break;
	case AAK_CLASSDESC:		 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),ClassDESC());		 break;
	case AAK_SELLERNAMEASC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),SellerNameASC());	 break;
	case AAK_SELLERNAMEDESC: std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),SellerNameDESC()); break;
	case AAK_REGASC:																			 break;
	case AAK_REGDESC:		 std::reverse(pVTAUCTION->begin(),pVTAUCTION->end());				 break;
	case AAK_WEARLEVELASC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),WearLevelASC());	 break;
	case AAK_WEARLEVELDESC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),WearLevelDESC());	 break;
	case AAK_ITEMLEVELASC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),ItemLevelASC());	 break;
	case AAK_ITEMLEVELDESC:	 std::sort(pVTAUCTION->begin(), pVTAUCTION->end(),ItemLevelDESC());	 break;
	}
}