#include "StdAfx.h"
#include "TWorldSvr.h"
#include "TWorldSvrModule.h"

CTGuild::CTGuild(void)
{
	m_bLevel = 0;
	m_dwFame = 0;
	m_dwFameColor = 0;
	m_dwGI = 0;
	m_dwExp = 0;
	m_bGPoint = 0;
	m_bStatus = 0;
	m_dwGold = 0;
	m_dwSilver = 0;
	m_dwCooper = 0;
	m_bDisorg = 0;
	m_dwTime = 0;
	m_bCountry = TCONTRY_N;
	m_dwArticleIndex = 0;
	m_timeEstablish = 0;
	m_strArticleTitle = NAME_NULL;
	m_dwPvPUseablePoint = 0;
	m_dwPvPTotalPoint = 0;
	m_dwPvPMonthPoint = 0;
	m_dwRankTotal = 0;
	m_dwRankMonth = 0;

	m_bMaxCabinet = 0;

	m_pTLEVEL = NULL;

	m_mapTMember.clear();
	m_mapTTactics.clear();
	m_mapTArticle.clear();
	m_mapTCabinet.clear();
	m_vAllience.clear();
	m_vEnemy.clear();
	m_vPointReward.clear();
}

CTGuild::~CTGuild(void)
{
	MAPTGUILDARTICLE::iterator itARTICLE;
	MAPTITEM::iterator itITEM;
	MAPTTACTICSMEMBER::iterator itTACTICS;
	MAPTGUILDMEMBER::iterator itMEMBER;

	for(itARTICLE=m_mapTArticle.begin(); itARTICLE!=m_mapTArticle.end(); itARTICLE++)
		delete (*itARTICLE).second;

	for(itITEM=m_mapTCabinet.begin(); itITEM!=m_mapTCabinet.end(); itITEM++)
		delete (*itITEM).second;

	for(itMEMBER=m_mapTMember.begin(); itMEMBER!=m_mapTMember.end(); itMEMBER++)
		delete (*itMEMBER).second;
	
	for(itTACTICS=m_mapTTactics.begin(); itTACTICS!=m_mapTTactics.end(); itTACTICS++)
		delete (*itTACTICS).second;
	
	m_mapTArticle.end();
	m_mapTCabinet.clear();
	m_mapTMember.clear();
	m_mapTTactics.clear();
}

BYTE CTGuild::AddMember(LPTGUILDMEMBER pMember)
{
	MAPTGUILDMEMBER::iterator it = m_mapTMember.find(pMember->m_dwID);
	if(it==m_mapTMember.end())
	{
		pMember->m_dwGuildID = m_dwID;
		m_mapTMember.insert(MAPTGUILDMEMBER::value_type(pMember->m_dwID, pMember));

		UpdateLevel();

		return GUILD_SUCCESS;
	}
	else
		return GUILD_ALREADYMEMBER;
}

void CTGuild::DelMember(LPTGUILDMEMBER pMember)
{
	m_mapTMember.erase(pMember->m_dwID);
	delete pMember;

	UpdateLevel();
}

LPTCHARACTER CTGuild::Designate(DWORD dwID, BYTE bDuty)
{
	LPTCHARACTER pChar = NULL;
	MAPTGUILDMEMBER::iterator it = m_mapTMember.find(dwID);
	if(it!=m_mapTMember.end())
	{
		(*it).second->m_bDuty = bDuty;
		if(bDuty == GUILD_DUTY_CHIEF)
		{
			m_dwChief = dwID;
			m_strChief = (*it).second->m_strName;
		}

		if((*it).second->m_pChar)
			pChar = (*it).second->m_pChar;
	}
	return pChar;
}

LPTCHARACTER CTGuild::CreatePeerage(DWORD dwID, BYTE bPeer)
{
	LPTCHARACTER pChar=NULL;
	MAPTGUILDMEMBER::iterator it = m_mapTMember.find(dwID);
	if(it!=m_mapTMember.end())
	{
		(*it).second->m_bPeer = bPeer;
		pChar = (*it).second->m_pChar;
	}
	return pChar;
}

LPTGUILDMEMBER CTGuild::FindMember(DWORD dwID)
{
	MAPTGUILDMEMBER::iterator it = m_mapTMember.find(dwID);
	if(it!=m_mapTMember.end())
		return (*it).second;

	return NULL;
}

LPTGUILDMEMBER CTGuild::FindMember(CString strName)
{
	MAPTGUILDMEMBER::iterator it;
	for(it=m_mapTMember.begin(); it!=m_mapTMember.end(); it++)
	{
		if((*it).second->m_strName == strName)
			return (*it).second;
	}

	return NULL;
}

LPTGUILDMEMBER CTGuild::FindDesignate(BYTE bDuty)
{
	MAPTGUILDMEMBER::iterator it;
	for(it=m_mapTMember.begin(); it!=m_mapTMember.end(); it++)
	{
		if((*it).second->m_bDuty == bDuty)
			return (*it).second;
	}
	return NULL;
}

LPTGUILDMEMBER CTGuild::FindChief()
{
	MAPTGUILDMEMBER::iterator it = m_mapTMember.find(m_dwChief);

	if( it != m_mapTMember.end() )
		return (*it).second;

	return NULL;
}

BYTE CTGuild::FindDuty(DWORD dwID)
{
	MAPTGUILDMEMBER::iterator it = m_mapTMember.find(dwID);
	if(it!=m_mapTMember.end())
		return (*it).second->m_bDuty;

	return 0;
}

BYTE CTGuild::FindPeer( DWORD dwID)
{
	MAPTGUILDMEMBER::iterator it = m_mapTMember.find(dwID);

	if( it != m_mapTMember.end() )
		return (*it).second->m_bPeer;

	return 0;
}

void CTGuild::SetMemberConnection( DWORD dwCharID, LPTCHARACTER pChar, INT64 dlTime)
{
	MAPTGUILDMEMBER::iterator it = m_mapTMember.find(dwCharID);
	if( it != m_mapTMember.end() )
	{
		(*it).second->m_dlConnectedDate = dlTime;
		(*it).second->m_pChar = pChar;
	}
}

void CTGuild::SetMemberLevel( DWORD dwCharID, BYTE bLevel)
{
	MAPTGUILDMEMBER::iterator it = m_mapTMember.find(dwCharID);

	if( it != m_mapTMember.end() )
		(*it).second->m_bLevel = bLevel;
}

BYTE CTGuild::CheckPeerage(BYTE bDuty, BYTE bPeer)
{
	if(!bPeer)
		return TRUE;

	if(bPeer > MAX_GUILD_PEER_COUNT)
		return FALSE;

	BYTE bCnt=0;
	MAPTGUILDMEMBER::iterator it;
	for(it=m_mapTMember.begin(); it!=m_mapTMember.end(); it++)
	{
		if((*it).second->m_bPeer == bPeer)
			bCnt++;
	}

	if(bCnt >= m_pTLEVEL->m_bPeer[bPeer-1])
		return FALSE;

	switch(m_pTLEVEL->m_bLevel)
	{
	case 1:
	case 2:
		break;
	case 3:
	case 4:
		if(bDuty != GUILD_DUTY_CHIEF &&
			bPeer == GUILD_PEER_BARON)
			return FALSE;
		break;
	case 5:
	case 6:
		if(bDuty != GUILD_DUTY_CHIEF &&
			bPeer == GUILD_PEER_VISCOUNT)
			return FALSE;
		break;
	case 7:
	case 8:
		if(bDuty != GUILD_DUTY_CHIEF &&
			bPeer == GUILD_PEER_COUNT)
			return FALSE;
		break;
	case 9:
		if(bDuty != GUILD_DUTY_CHIEF &&
			bPeer == GUILD_PEER_MARQUIS)
			return FALSE;
		break;
	case 10:
		if(bDuty != GUILD_DUTY_CHIEF &&
			bPeer == GUILD_PEER_DUKE)
			return FALSE;
		break;
	}

	return TRUE;
}

BYTE CTGuild::GetMaxMemberCnt()
{
	return m_pTLEVEL->m_bMaxCnt;
}

BYTE CTGuild::GetMemberSize()
{
	return (BYTE)m_mapTMember.size();
}

BYTE CTGuild::CanAddMember()
{
	if(GetMemberSize() >= m_pTLEVEL->m_bMaxCnt)
		return FALSE;

	return TRUE;
}
BYTE CTGuild::CanAddTactics()
{
	if(m_mapTTactics.size() >= m_pTLEVEL->m_bTacticsCnt)
		return FALSE;

	return TRUE;
}
void CTGuild::UpdateLevel()
{
	BYTE bPrevLevel = m_bLevel;
	BYTE bMinCnt = m_pTLEVEL->m_bMinCnt;
	BYTE bMemberCnt = GetMemberSize();

	if(bMinCnt > bMemberCnt)
		m_bLevel = (m_bLevel-1) ? m_bLevel-1 : 1;
	else
	{
		BYTE bLevel = (m_bLevel+1) > MAX_GUILD_LEVEL ? MAX_GUILD_LEVEL : m_bLevel+1;
		LPTGUILDLEVEL pNextLevel = _AtlModule.FindGuildLevel(bLevel);
		if(pNextLevel)
		{
			if(m_dwExp >= m_pTLEVEL->m_dwEXP &&
				bMemberCnt >= pNextLevel->m_bMinCnt)
				m_bLevel++;
		}
	}

	if(bPrevLevel != m_bLevel)
	{
		LPTGUILDLEVEL pLevel = _AtlModule.FindGuildLevel(m_bLevel);
		if(pLevel)
		{
			m_pTLEVEL = pLevel;
			_AtlModule.SendDM_GUILDLEVEL_REQ(m_dwID, m_bLevel);
		}
		else
			m_bLevel = bPrevLevel;
	}

	if( m_bMaxCabinet < m_pTLEVEL->m_bCabinetCnt)
	{
		m_bMaxCabinet = m_pTLEVEL->m_bCabinetCnt;
		_AtlModule.SendDM_GUILDCABINETMAX_REQ(m_dwID, m_bMaxCabinet);
	}
}
BYTE CTGuild::GainEXP(LPTGUILDMEMBER pMember, DWORD dwExp)
{
	if(dwExp)
	{
		m_dwExp += dwExp;

		_AtlModule.SendDM_GUILDCONTRIBUTION_REQ(m_dwID, pMember->m_dwID, m_dwExp, m_dwGold, m_dwSilver, m_dwCooper);

		UpdateLevel();
	}

	return TRUE;
}
BYTE CTGuild::GainMoney(DWORD dwGold, DWORD dwSilver, DWORD dwCooper)
{
	__int64 llMoney = CalcMoney(dwGold, dwSilver, dwCooper); 
	if(llMoney)
	{
		__int64 llGMoney = llMoney + CalcMoney(m_dwGold, m_dwSilver, m_dwCooper);
		CalcMoney(llGMoney, m_dwGold, m_dwSilver, m_dwCooper);

		_AtlModule.SendDM_GUILDCONTRIBUTION_REQ(m_dwID, 0, m_dwExp, m_dwGold, m_dwSilver, m_dwCooper);
	}

	return TRUE;
}

BYTE CTGuild::UseMoney(INT64 dlMoney, BYTE bUse)
{
	__int64 dlMyMoney = CalcMoney(m_dwGold, m_dwSilver, m_dwCooper);

	if(dlMyMoney < dlMoney)
		return FALSE;

	if(bUse && dlMoney)
	{
		dlMyMoney -= dlMoney;
		CalcMoney(dlMyMoney, m_dwGold, m_dwSilver, m_dwCooper);

		_AtlModule.SendDM_GUILDCONTRIBUTION_REQ(m_dwID, 0, m_dwExp, m_dwGold, m_dwSilver, m_dwCooper);
	}

	return TRUE;
}

BYTE CTGuild::UseMoney(DWORD dwGold, DWORD dwSilver, DWORD dwCooper, BYTE bUse)
{
	__int64 llMoney = CalcMoney(dwGold, dwSilver, dwCooper); 
	return UseMoney(llMoney, bUse);
}

BYTE CTGuild::Contribution(DWORD dwCharID, DWORD dwExp, DWORD dwGold, DWORD dwSilver, DWORD dwCooper, DWORD dwPvPoint)
{
	LPTGUILDMEMBER pMember = FindMember(dwCharID);
	if(!pMember)
		return FALSE;

	INT64 llMoney = CalcMoney(dwGold, dwSilver, dwCooper);
	if(llMoney)
	{
		if(llMoney > MAX_GUILD_CONTRIBUTION || llMoney < MIN_GUILD_CONTRIBUTION )
			return FALSE;

		if(!GainMoney(dwGold, dwSilver, dwCooper))
			return FALSE;
	}

	if(!GainEXP(pMember, dwExp))
		return FALSE;

	if(dwPvPoint)
		GainPvPoint(dwPvPoint, PVPE_GUILD, PVP_USEABLE);

	return TRUE;
}
BYTE CTGuild::GetArticleSize()
{
	BYTE bSize = BYTE(m_mapTArticle.size());

	return bSize;
}
void CTGuild::AddArticle(DWORD dwID, BYTE bDuty, CString strWritter, CString strTitle, CString strArticle, DWORD dwTime)
{
	if(dwID >= m_dwArticleIndex)
		m_dwArticleIndex = dwID;

	LPTGUILDARTICLE pArticle = new TGUILDARTICLE();
	pArticle->m_dwID = m_dwArticleIndex;
	pArticle->m_bDuty = bDuty;
	pArticle->m_strWritter = strWritter;
	pArticle->m_strTitle = strTitle;
	pArticle->m_strArticle = strArticle;
	CTime time(dwTime);
	pArticle->m_strDate = time.Format(_T("%Y-%m-%d"));

	m_strArticleTitle = pArticle->m_strTitle;

	m_mapTArticle.insert(MAPTGUILDARTICLE::value_type(pArticle->m_dwID, pArticle));
}
DWORD CTGuild::AddArticle(BYTE bDuty, CString strWritter, CString strTitle, CString strArticle, DWORD dwTime)
{
	if( BYTE(m_mapTArticle.size()) >= MAX_GUILD_ARTICLE_COUNT )
		return 0;

	m_dwArticleIndex++;

	LPTGUILDARTICLE pArticle = new TGUILDARTICLE();
	pArticle->m_dwID = m_dwArticleIndex;
	pArticle->m_bDuty = bDuty;
	pArticle->m_strWritter = strWritter;
	pArticle->m_strTitle = strTitle;
	pArticle->m_strArticle = strArticle;
	CTime time(dwTime);
	pArticle->m_strDate = time.Format(_T("%Y-%m-%d"));

	m_strArticleTitle = pArticle->m_strTitle;

	m_mapTArticle.insert(MAPTGUILDARTICLE::value_type(pArticle->m_dwID, pArticle));

	return m_dwArticleIndex;
}
BYTE CTGuild::DelArticle(DWORD dwID)
{
	MAPTGUILDARTICLE::iterator find = m_mapTArticle.find(dwID);
	if(find != m_mapTArticle.end())
	{
		CString strTitle = (*find).second->m_strTitle;

		delete (*find).second;
		m_mapTArticle.erase(dwID);

		if(m_strArticleTitle == strTitle)
		{
			DWORD dwMax = 0;

			MAPTGUILDARTICLE::iterator it;
			for(it=m_mapTArticle.begin(); it!=m_mapTArticle.end(); it++)
			{
				if(dwMax < (*it).second->m_dwID)
				{
					dwMax = (*it).second->m_dwID;
					m_strArticleTitle = (*it).second->m_strTitle;
				}
			}
		}

		return TRUE;
	}

	return FALSE;
}
BYTE CTGuild::UpdateArticle(DWORD dwID, CString strTitle, CString strArticle)
{
	MAPTGUILDARTICLE::iterator find = m_mapTArticle.find(dwID);
	if(find != m_mapTArticle.end())
	{
		if(m_strArticleTitle == (*find).second->m_strTitle)
			m_strArticleTitle = strTitle;

		(*find).second->m_strTitle = strTitle;
		(*find).second->m_strArticle = strArticle;
		return TRUE;
	}

	return FALSE;
}
void CTGuild::SetFame(DWORD dwFame, DWORD dwFameColor)
{
	m_dwFame = dwFame;
	m_dwFameColor = dwFameColor;
}
BYTE CTGuild::GetCabinetSize()
{
	BYTE bSize = BYTE(m_mapTCabinet.size());

	return bSize;
}
void CTGuild::PutInItem(DWORD dwItemID, LPTITEM pItem)
{
	MAPTITEM::iterator find = m_mapTCabinet.find(dwItemID);
	if(find!=m_mapTCabinet.end())
	{
		(*find).second->m_bCount += pItem->m_bCount;
		delete pItem;
		pItem = NULL;
	}
	else
		m_mapTCabinet.insert(MAPTITEM::value_type(pItem->m_dwItemID, pItem));
}
LPTITEM	CTGuild::FindCabinetItem(DWORD dwItemID)
{
	LPTITEM pItem = NULL;

	MAPTITEM::iterator find = m_mapTCabinet.find(dwItemID);
	if(find!=m_mapTCabinet.end())
		pItem = (*find).second;

	return pItem;
}
BYTE CTGuild::TakeOutItem(DWORD dwItemID, BYTE bCount)
{
	MAPTITEM::iterator find = m_mapTCabinet.find(dwItemID);
	if(find==m_mapTCabinet.end())
		return FALSE;

	LPTITEM pItem = (*find).second;

	pItem->m_bCount -= bCount;
	if(!pItem->m_bCount)
	{
		delete pItem;
		m_mapTCabinet.erase(dwItemID);
	}

	return TRUE;
}

LPTTACTICSMEMBER CTGuild::FindTactics(DWORD dwID)
{
	MAPTTACTICSMEMBER::iterator find = m_mapTTactics.find(dwID);
	if(find!=m_mapTTactics.end())
		return (*find).second;

	return NULL;
}
LPTTACTICSMEMBER CTGuild::FindTactics(CString strName)
{
	MAPTTACTICSMEMBER::iterator it;
	for(it=m_mapTTactics.begin(); it!=m_mapTTactics.end(); it++)
		if((*it).second->m_strName == strName)
			return (*it).second;

	return NULL;
}
void CTGuild::SetTacticsConnection(DWORD dwCharID, LPTCHARACTER pChar)
{
	MAPTTACTICSMEMBER::iterator it = m_mapTTactics.find(dwCharID);
	if( it != m_mapTTactics.end() )
		(*it).second->m_pChar = pChar;
}
void CTGuild::GainPvPoint(DWORD dwPoint, BYTE bEvent, BYTE bType)
{
	if(!dwPoint)
		return;

	if(bType & PVP_TOTAL)
	{
		m_dwPvPTotalPoint += dwPoint;
		m_dwPvPMonthPoint += dwPoint;
	}

	if(bType & PVP_USEABLE)
		m_dwPvPUseablePoint += dwPoint;

	_AtlModule.SendDM_GUILDPVPOINT_REQ(
		m_dwID,
		m_dwPvPTotalPoint,
		m_dwPvPUseablePoint,
		m_dwPvPMonthPoint);
}

void CTGuild::UsePvPoint(DWORD dwPoint, BYTE bType)
{
	if(!dwPoint)
		return;

	if(bType & PVP_TOTAL)
		m_dwPvPTotalPoint = m_dwPvPTotalPoint > dwPoint ? m_dwPvPTotalPoint-dwPoint : 0;

	if(bType & PVP_USEABLE)
		m_dwPvPUseablePoint = m_dwPvPUseablePoint > dwPoint ? m_dwPvPUseablePoint-dwPoint : 0;

	_AtlModule.SendDM_GUILDPVPOINT_REQ(
		m_dwID,
		m_dwPvPTotalPoint,
		m_dwPvPUseablePoint,
		m_dwPvPMonthPoint);
}

void CTGuild::PointLog(DWORD dwPoint, CString strTarget, INT64 dlDate)
{
	TGUILDPOINTREWARD log;
	log.m_strName = strTarget;
	log.m_dwPoint = dwPoint;
	log.m_dlDate = dlDate;

	m_vPointReward.insert(m_vPointReward.begin(), log);
	if(m_vPointReward.size() > 50)
		m_vPointReward.pop_back();
}

void CTGuild::CalcWeekRecord(DWORD dwDate)
{
	MAPTGUILDMEMBER::iterator it;
	for(it=m_mapTMember.begin(); it!=m_mapTMember.end(); it++)
		CalcWeekRecord((*it).second, dwDate);
}

void CTGuild::CalcWeekRecord(LPTGUILDMEMBER pMember, DWORD dwDate)
{
	memset(&(pMember->m_weekrecord), 0, sizeof(TENTRYRECORD));

	for(WORD w=0; w<pMember->m_vRecord.size();)
	{
		if(pMember->m_vRecord[w].m_dwDate + 7 <= dwDate)
			pMember->m_vRecord.erase(pMember->m_vRecord.begin() + w);
		else
		{
			pMember->m_weekrecord.m_wKillCount += pMember->m_vRecord[w].m_wKillCount;
			pMember->m_weekrecord.m_wDieCount += pMember->m_vRecord[w].m_wDieCount;

			for(BYTE e=0; e<PVPE_COUNT; e++)
				pMember->m_weekrecord.m_aGainPoint[e] += pMember->m_vRecord[w].m_aGainPoint[e];

			w++;
		}
	}
}

BYTE CTGuild::CanApplyWar(WORD wCastle)
{
	BYTE bCount = 0;
	MAPTGUILDMEMBER::iterator it;
	for(it=m_mapTMember.begin(); it!=m_mapTMember.end(); it++)
	{
		if((*it).second->m_wCastle == wCastle)
			bCount++;
	}

	MAPTTACTICSMEMBER::iterator find;
	for(find=m_mapTTactics.begin(); find != m_mapTTactics.end(); find++)
	{
		if((*find).second->m_wCastle == wCastle)
			bCount++;
	}

	if(bCount >= 49)
		return FALSE;

	return TRUE;
}

BYTE CTGuild::AddTactics(LPTTACTICSMEMBER pMember)
{
	MAPTTACTICSMEMBER::iterator find = m_mapTTactics.find(pMember->m_dwID);
	if(find == m_mapTTactics.end())
	{
		_AtlModule.OnEventExpired(TRUE, EXPIRED_GT, pMember->m_dlEndTime, m_dwID, pMember->m_dwID);
		m_mapTTactics.insert(MAPTTACTICSMEMBER::value_type(pMember->m_dwID, pMember));
	}
	else
		return FALSE;

	return TRUE;
}

BYTE CTGuild::DelTactics(LPTTACTICSMEMBER pMember, BYTE bKick)
{
	if(bKick < 3)
		_AtlModule.OnEventExpired(FALSE, EXPIRED_GT, pMember->m_dlEndTime, m_dwID, pMember->m_dwID);

	if(!bKick)
	{
		GainPvPoint(pMember->m_dwRewardPoint, 0, PVP_USEABLE);

		DWORD dwGold, dwSilver, dwCooper;
		CalcMoney(pMember->m_dlRewardMoney, dwGold, dwSilver, dwCooper);
		GainMoney(dwGold, dwSilver, dwCooper);
	}

	m_mapTTactics.erase(pMember->m_dwID);
	delete pMember;

	return GUILD_SUCCESS;
}

WORD CTGuild::GetCastleApplicantCount(WORD wCastle)
{
	BYTE bCount = 0;
	BYTE bCamp = 0;
	MAPTGUILDMEMBER::iterator it;
	for(it=m_mapTMember.begin(); it!=m_mapTMember.end(); it++)
	{
		if((*it).second->m_wCastle == wCastle)
		{
			bCount++;
			bCamp = (*it).second->m_bCamp;
		}
	}

	MAPTTACTICSMEMBER::iterator find;
	for(find=m_mapTTactics.begin(); find != m_mapTTactics.end(); find++)
	{
		if((*find).second->m_wCastle == wCastle)
		{
			bCount++;
			bCamp = (*find).second->m_bCamp;
		}
	}

	return MAKEWORD(bCount, bCamp);
}

void CTGuild::GetCastleApplicantCount(MAPWORD * mapCastle)
{
	MAPTGUILDMEMBER::iterator itM;
	for(itM=m_mapTMember.begin(); itM!=m_mapTMember.end(); itM++)
	{
		WORD wCastle = (*itM).second->m_wCastle;
		if(wCastle)
		{
			MAPWORD::iterator itC = mapCastle->find(wCastle);
			if(itC == mapCastle->end())
				mapCastle->insert(MAPWORD::value_type(wCastle, MAKEWORD(1, (*itM).second->m_bCamp)));
			else
				(*itC).second += 1;
		}
	}

	MAPTTACTICSMEMBER::iterator itT;
	for(itT=m_mapTTactics.begin(); itT != m_mapTTactics.end(); itT++)
	{
		WORD wCastle = (*itT).second->m_wCastle;
		if(wCastle)
		{
			MAPWORD::iterator itC = mapCastle->find(wCastle);
			if(itC == mapCastle->end())
				mapCastle->insert(MAPWORD::value_type(wCastle, MAKEWORD(1, (*itT).second->m_bCamp)));
			else
				(*itC).second += 1;
		}
	}
}
