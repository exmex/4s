#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

CTPlayer::CTPlayer()
{
	m_bSessionType = SESSION_CLIENT;
	m_strStoreName.Empty();
	m_strNAME.Empty();
	m_strComment.Empty();

	m_dwCloseTick = 0;
	m_dwAcceptTick = 0;
	m_dwIPAddr = 0;
	m_wPort = 0;

	m_dwUserID = 0;
	m_dwKEY = 0;

	m_bRealSex = 0;

	m_bStartAct = 0;
	m_bClass = 0;
	m_bRace = 0;
	m_bSex = 0;

	m_bPants = 0;
	m_bHair = 0;
	m_bFace = 0;
	m_bBody = 0;
	m_bHand = 0;
	m_bFoot = 0;
	m_bRPSWin = 0;
	m_bRPSType = 0;

	m_bCanHost = FALSE;
	m_bIsGhost = FALSE;

	m_dwCooper = 0;
	m_dwSilver = 0;
	m_dwGold = 0;
	m_dwEXP = 0;
	m_wSpawnID = 0;
	m_wSkillPoint = 0;
	m_dwPartyChiefID = 0;
	m_wCommanderID = 0;
	m_dwLockedMonID = 0;
	m_dwLastAtkTick = 0;
	m_dwPvPUseablePoint = 0;
	m_dwPvPTotalPoint = 0;
	m_dwPvPRankOrder = 0;
	m_bPvPRankPercent = 0;
	m_dwTotalWin = 0;
	m_dwTotalLose = 0;
	 
	m_dwMonthPvPoint = 0;
	m_dwMonthRankOrder = 0;
	m_bMonthRankPercent = 0;
	m_wMonthWin = 0;
	m_wMonthLose = 0;
	m_strMonthSay = NAME_NULL;

	m_dwPcBangTime = 0;
	m_bInPcBang = FALSE;
	m_timeEnter = 0;
	m_bPcBangItemCnt = 0;

	m_fGhostX = 0;
	m_fGhostZ = 0;
	m_fVital = 0;

	m_bCloseAll = TRUE;
	m_bLogout = FALSE;
	m_bMain = FALSE;

	m_bStatus = OS_WAKEUP;
	m_bType = OT_PC;

	m_bEquipSpecial = FALSE;
	m_bIsSpecial = FALSE;

	m_dwGuildLeaveTime = 0;
	m_bGuildLeave = 0;

	m_dwRiding = 0;
	m_bHelmetHide = 0;
	m_bStore = 0;

	m_dwDuelID = 0;
	m_bDuelType = 0;
	m_dwDuelTarget = 0;
	m_guildItem = NULL;
	m_bLuckyNumber = 0;
	m_pPost = NULL;

	m_dwSoulmate = 0;
	m_strSoulmate = NAME_NULL;
	m_dwSoulSilence = 0;
	m_wGodBall = 0;
	m_wLastSpawnID = 0;
	m_dwLastDestination = 0;
	m_nChatBanTime = 0;

	m_wCastle = 0;
	m_bCamp = 0;
	m_bGraceExit = FALSE;
	m_dwLastAttackCharID = 0;
	m_dwLastAttackTick = 0;
	m_dwPostID = 0;
	m_wPostTotal = 0;
	m_wPostRead = 0;
	m_wArenaID = 0;
	m_bArenaTeam = 0;

	m_questlist_possible.Clear();
	m_mapLevelQuest.clear();
	m_mapQUEST.clear();
	m_mapCabinet.clear();
	m_mapHotkeyInven.clear();
	m_mapItemCoolTime.clear();
	m_mapRecallMon.clear();
	m_mapSelfMon.clear();
	m_mapStoreItem.clear();
	m_mapTPROTECTED.clear();
	m_mapTPET.clear();

	ClearDealItem();
	ClearGuildItem();
	m_vPvPRecent.clear();
	memset(m_aPvPRecord, 0, sizeof(m_aPvPRecord));

	m_vDuelRecordSet.clear();
	memset(m_aDuelScore, 0, sizeof(m_aDuelScore));

	ClearAuctionList();
}

CTPlayer::~CTPlayer()
{
	ClearRecallMon(FALSE);
	ClearSelfMon(FALSE);
	ClearMain();
}

void CTPlayer::ClearMain()
{
	ClearInven();
	ClearSkill();
	ClearMaintainSkill();
	ClearQuest();
	ClearStorageItem();
	ClearHotkey();
	ClearDealItem();
	ClearPost();
	ClearStore();
	ClearPet();
	ClearDuringItem();
	ClearProtected();
	ClearAuctionList();

	if(m_dwDuelID)
		_AtlModule.SendSM_DUELEND_REQ(m_dwDuelID, 0);
	ClearDuel();
	ClearGuildItem();

	m_vDuelRecordSet.clear();
	m_vPvPRecent.clear();
	m_mapItemCoolTime.clear();

	m_questlist_possible.Clear();
}

BYTE CTPlayer::HangPremiumItem()
{
	HangExpBuff();

	if(!(m_bInPcBang & (PCBANG_PREMIUM1 | PCBANG_PREMIUM2)))
		return FALSE;

	if(m_diPremium.m_bType)
	{
		DWORD dwRemainTick = min(m_diPremium.m_dwRemainTime, DAY_ONE * 31) * 1000;
		ForceMaintain(
			m_diPremium.m_pTITEM->m_wUseValue,
			m_dwID,
			OT_PC,
			m_dwID,
			OT_PC,
			dwRemainTick);

		return TRUE;
	}

	return FALSE;
}
void CTPlayer::CheckDuringItem(__time64_t tCurrent, DWORD dwPassTime)
{
	if(m_diPremium.m_bType)
	{
		if((m_diPremium.m_bType == DURINGTYPE_DAY && m_diPremium.m_tEndTime <= tCurrent) ||
			(m_diPremium.m_bType == DURINGTYPE_TIME && m_diPremium.m_dwRemainTime == 0))
		{
			WORD wSkillID = m_diPremium.m_pTITEM->m_wUseValue;
			SetDuringItem(IK_GOLDPREMIUM, NULL, 0, 0);
			EraseMaintainSkill(wSkillID);
			ResetPcBangData(m_bInPcBang & PCBANG_REAL, PCBANG_TIME<m_dwPcBangTime ? 0 : m_dwPcBangTime-PCBANG_TIME);
			HangExpBuff();
		}
		else
		{
			m_diPremium.m_dwRemainTime = m_diPremium.m_dwRemainTime > dwPassTime ? m_diPremium.m_dwRemainTime - dwPassTime : 0;
		}
	}

	WORD wBonus = 0;
	if(IsExpBenefit(wBonus) == IK_EXPBONUS)
	{
		if((m_diExp.m_bType == DURINGTYPE_DAY && m_diExp.m_tEndTime <= tCurrent) ||
			(m_diExp.m_bType == DURINGTYPE_TIME && m_diExp.m_dwRemainTime == 0))
		{
			SetDuringItem(IK_EXPBONUS, NULL, 0, 0);
			EraseMaintainSkill(TPCBANG_SKILL);
			return;
		}	
		else
		{
			m_diExp.m_dwRemainTime = m_diExp.m_dwRemainTime > dwPassTime ? m_diExp.m_dwRemainTime - dwPassTime : 0;
		}
	}
}

LPTDURINGITEM CTPlayer::SetDuringItem(BYTE bKind, LPTITEM pITEM, BYTE bType, DWORD dwAdd)
{
	LPTDURINGITEM pDI = NULL;
	if(!dwAdd)
		bType = 0;

	switch(bKind)
	{
	case IK_GOLDPREMIUM:
	case IK_GOLDPREMIUM2:
		m_diPremium.m_pTITEM = pITEM;
		m_diPremium.m_bType = bType;
		m_diPremium.m_dwRemainTime = dwAdd;
		m_diPremium.m_tEndTime = _AtlModule.m_timeCurrent + dwAdd;
		pDI = &m_diPremium;
		break;
	case IK_EXPBONUS:
		m_diExp.m_pTITEM = pITEM;
		m_diExp.m_bType = bType;
		m_diExp.m_dwRemainTime = dwAdd;
		m_diExp.m_tEndTime = _AtlModule.m_timeCurrent + dwAdd;
		pDI = &m_diExp;
		break;
	default:
		break;
	}

	return pDI;
}

void CTPlayer::ResetPcBangData(BYTE bInPcBang, DWORD dwRemainTime)
{
	if(!m_pMAP)
		return;

	BYTE bPrevPc = m_bInPcBang;
	m_bInPcBang = (m_bInPcBang & PCBANG_REAL) | bInPcBang;

	if(bInPcBang != PCBANG_REAL)
	{
		m_dwPcBangTime = min(PCBANG_TIME < dwRemainTime ? 0 : PCBANG_TIME-dwRemainTime, m_dwPcBangTime);
		m_bPcBangItemCnt = 0;
		m_timeEnter = _AtlModule.m_timeCurrent;
		EraseMaintainSkill(TPCBANG_SKILL);
	}

	if(m_bInPcBang != bPrevPc)
	{
		VPLAYER vPLAYERS;
		vPLAYERS.clear();

		m_pMAP->GetNeighbor(
			&vPLAYERS,
			m_fPosX,
			m_fPosZ);

		while(!vPLAYERS.empty())
		{
			CTPlayer *pChar = vPLAYERS.back();
			pChar->SendCS_RESETPCBANG_ACK(m_dwID, m_bInPcBang);
			vPLAYERS.pop_back();
		}
	}
}

void CTPlayer::HangExpBuff()
{
	WORD wBonus = 0;
	BYTE bExp = IsExpBenefit(wBonus);
	if(bExp == IK_GOLDPREMIUM)
	{
		DWORD dwRemainTick = 0xFFFFFFFF;
		if(m_diPremium.m_bType)
			dwRemainTick = min(m_diPremium.m_dwRemainTime, DAY_ONE * 31) * 1000;

		ForceMaintain(TPCBANG_SKILL, m_dwID, OT_PC, m_dwID, OT_PC, min(dwRemainTick, (PCBANG_TIME - m_dwPcBangTime) * 1000));
	}
	else if(bExp == IK_EXPBONUS)
		ForceMaintain(TPCBANG_SKILL, m_dwID, OT_PC, m_dwID, OT_PC, m_diExp.m_dwRemainTime * 1000);
}

LPTPET CTPlayer::PetMake(WORD wPetID, CString strName, __int64 ldwTime)
{
	WORD wID = MAKEWORD(m_bRace, wPetID);

	LPTPET pPET = NULL;
	MAPTPET::iterator itPET = m_mapTPET.find(wID);
	if(itPET != m_mapTPET.end())
	{
		pPET = (*itPET).second;
		pPET->m_strName = strName;
		if(pPET->m_ldwTime < _AtlModule.m_timeCurrent ||
			(wPetID == PCBANG_PET || wPetID == PREMIUM2_PET))
			pPET->m_ldwTime = _AtlModule.m_timeCurrent + ldwTime;
		else
            pPET->m_ldwTime += ldwTime;

		if(!ldwTime)
			pPET->m_ldwTime = 0;
	}
	else
	{
		if(m_mapTPET.size() == MAX_PET_COUNT)
		{
			SendCS_PETMAKE_ACK(PET_FULL, 0, NAME_NULL, 0);
			return NULL;
		}
		
		MAPTPETTEMP::iterator find = _AtlModule.m_mapTPET.find(wID);
		if(find == _AtlModule.m_mapTPET.end())
		{
			SendCS_PETMAKE_ACK(PET_NOTFOUND, 0, NAME_NULL, 0);
			return NULL;
		}

		pPET = new TPET();
		pPET->m_wPetID = wID;
		pPET->m_strName = strName;
		pPET->m_ldwTime = ldwTime ? _AtlModule.m_timeCurrent + ldwTime : 0;
		pPET->m_pTPET = (*find).second;
		m_mapTPET.insert(MAPTPET::value_type(pPET->m_wPetID, pPET));
	}

	return pPET;
}

void CTPlayer::ResetSize(WORD wMonTempID)
{
	if(wMonTempID)
	{
		LPTMONSTER pMon = _AtlModule.FindTMonster(wMonTempID);
		if(pMon)
			m_fSize = pMon->m_fSize/2;
	}
	else
		m_fSize = OBJECT_SIZE / 2;
}

BYTE CTPlayer::SetDealItemTarget(CString strTarget)
{
	if(m_dealItem.m_bStatus != DEAL_READY)
		return FALSE;

	m_dealItem.m_strTarget = strTarget;

	DeleteDealItem();

	m_dealItem.m_bStatus = DEAL_START;
	m_dealItem.m_bDealing = DEAL_WAIT;

	return TRUE;
}

void CTPlayer::ClearProtected()
{
	MAPTPROTECTED::iterator it;
	for(it=m_mapTPROTECTED.begin(); it!=m_mapTPROTECTED.end(); it++)
		delete (*it).second;

	m_mapTPROTECTED.clear();
}

void CTPlayer::ClearDuringItem()
{
	SetDuringItem(IK_GOLDPREMIUM, NULL, 0, 0);
	SetDuringItem(IK_EXPBONUS, NULL, 0, 0);
}

void CTPlayer::ClearPost()
{
	if(m_pPost)
	{
		while(!m_pPost->m_vItem.empty())
		{
			delete m_pPost->m_vItem.back();
			m_pPost->m_vItem.pop_back();
		}

		delete m_pPost;
		m_pPost = NULL;
	}
/*
	MAPTPOST::iterator itPOST;
	for( itPOST = m_mapTPost.begin(); itPOST != m_mapTPost.end(); itPOST++)
	{
		while(!(*itPOST).second->m_vItem.empty())
		{
			delete (*itPOST).second->m_vItem.back();
			(*itPOST).second->m_vItem.pop_back();
		}

		delete (*itPOST).second;
	}

	m_mapTPost.clear();
*/
}
void CTPlayer::ClearStore()
{
	m_bStore = 0;
	m_strStoreName = NAME_NULL;
	MAPTSTOREITEM::iterator it;
	for(it=m_mapStoreItem.begin(); it!=m_mapStoreItem.end(); it++)
		delete (*it).second;

	m_mapStoreItem.clear();
}
void CTPlayer::ClearPet()
{
	MAPTPET::iterator it;
	for(it=m_mapTPET.begin(); it!=m_mapTPET.end(); it++)
		delete (*it).second;

	m_mapTPET.clear();
}
void CTPlayer::ClearDealItem()
{
	m_dealItem.m_bStatus = DEAL_READY;
	m_dealItem.m_bDealing = DEAL_READY;
	m_dealItem.m_strTarget.Empty();

	DeleteDealItem();
}

void CTPlayer::DeleteRecvDealItem()
{
	m_dealItem.m_bDealing = DEAL_READY;
	m_dealItem.m_llRecvMoney = 0;

	while(!m_dealItem.m_vRecvItem.empty())
	{
		delete m_dealItem.m_vRecvItem.back();
		m_dealItem.m_vRecvItem.pop_back();
	}

	m_dealItem.m_vRecvItem.clear();
}

void CTPlayer::DeleteSendDealItem()
{
	m_dealItem.m_llSendMoney = 0;

	while(!m_dealItem.m_vSendItem.empty())
	{
		delete m_dealItem.m_vSendItem.back();
		m_dealItem.m_vSendItem.pop_back();
	}

	m_dealItem.m_vSendItem.clear();
}

void CTPlayer::EraseInvenDealItem()
{
	while(!m_dealItem.m_vSendItem.empty())
	{
		CTItem * pItem = m_dealItem.m_vSendItem.back();
		m_dealItem.m_vSendItem.pop_back();

		CTInven * pInven = FindTInven(pItem->m_bInven);
		if(pInven)
		{
			CTItem * pDelItem = pInven->FindTItem(pItem->m_bItemID);
			if(pDelItem)
			{
#ifdef DEF_UDPLOG
				_AtlModule.m_pUdpSocket->LogItemTrade(LOGMAP_ITEMTRADESEND, this, pDelItem, m_dealItem.m_strTarget );
#endif	//	DEF_UDPLOG				

				SendCS_DELITEM_ACK(pItem->m_bInven, pItem->m_bItemID);
				pInven->m_mapTITEM.erase(pItem->m_bItemID);
				delete pDelItem;
			}
		}
	}
}

void CTPlayer::DeleteDealItem()
{
	DeleteRecvDealItem();
	DeleteSendDealItem();
}

DWORD CTPlayer::OnDamage( DWORD dwHostID,
						  DWORD dwAttackID,
						  BYTE bAttackType,
						  BYTE bAttackCountry,
						  BYTE bAttackAidCountry,
						  BYTE bAtkCanSelect,
						  WORD wPartyID,
						  int nDamageHP,
						  int nDamageMP)
{
	if(!m_pMAP)
		return 0;

	return CTObjBase::OnDamage(
		dwHostID,
		dwAttackID,
		bAttackType,
		bAttackCountry,
		bAttackAidCountry,
		bAtkCanSelect,
		wPartyID,
		nDamageHP,
		nDamageMP);
}

BYTE CTPlayer::CheckDealItem(BYTE bInven, BYTE bItemID)
{
	for(DWORD i=0; i<m_dealItem.m_vSendItem.size(); i++)
	{
		if(m_dealItem.m_vSendItem[i]->m_bInven == bInven &&
			m_dealItem.m_vSendItem[i]->m_bItemID == bItemID)
			return FALSE;
	}
	return TRUE;
}

BYTE CTPlayer::CheckStoreItem(BYTE bInven, BYTE bItemID)
{
	MAPTSTOREITEM::iterator it;

	for(it=m_mapStoreItem.begin(); it!=m_mapStoreItem.end(); it++)
	{
		if((*it).second->m_bInvenID == bInven &&
			(*it).second->m_bItemID == bItemID)
			return FALSE;
	}
	return TRUE;
}

BYTE CTPlayer::ValidDealItem()
{
	for(DWORD i=0; i<m_dealItem.m_vSendItem.size(); i++)
	{
		CTInven * pInven = FindTInven(m_dealItem.m_vSendItem[i]->m_bInven);
		if(!pInven)
			return FALSE;

		CTItem * pItem = pInven->FindTItem(m_dealItem.m_vSendItem[i]->m_bItemID);
		if(!pItem)
			return FALSE;

		if((*m_dealItem.m_vSendItem[i]) != (*pItem) ||
			m_dealItem.m_vSendItem[i]->m_bCount != pItem->m_bCount)
			return FALSE;
	}

	if(!UseMoney(m_dealItem.m_llSendMoney, FALSE))
		return FALSE;

	return TRUE;
}

void CTPlayer::ClearRecallMon( BYTE bExitMap)
{
	MAPTRECALLMON::iterator itMON;

	for( itMON = m_mapRecallMon.begin(); itMON != m_mapRecallMon.end(); itMON++)
	{
		if((*itMON).second->m_pMAP)
			(*itMON).second->m_pMAP->LeaveMAP((*itMON).second, bExitMap);

		delete (*itMON).second;
	}

	m_mapRecallMon.clear();
}

void CTPlayer::ClearSelfMon( BYTE bExitMap)
{
	MAPTSELFOBJ::iterator itMON;

	for( itMON = m_mapSelfMon.begin(); itMON != m_mapSelfMon.end(); itMON++)
	{
		if((*itMON).second->m_pMAP)
			(*itMON).second->m_pMAP->LeaveMAP((*itMON).second, bExitMap);

		delete (*itMON).second;
	}

	m_mapSelfMon.clear();
}

void CTPlayer::LeaveAllRecallMon()
{
	MAPTRECALLMON::iterator itMON;

	for( itMON = m_mapRecallMon.begin(); itMON != m_mapRecallMon.end(); itMON++)
		if((*itMON).second->m_pMAP)
		{
			(*itMON).second->m_pMAP->LeaveMAP((*itMON).second, FALSE);
			(*itMON).second->m_pMAP = NULL;
		}
}

void CTPlayer::ClearQuest()
{
	MAPQUEST::iterator itQUEST;

	for( itQUEST = m_mapQUEST.begin(); itQUEST != m_mapQUEST.end(); itQUEST++)
		delete (*itQUEST).second;

	m_mapLevelQuest.clear();
	m_mapQUEST.clear();
}

void CTPlayer::ClearStorageItem()
{
	MAPTSTORAGEITEM::iterator itITEM;
	MAPTCABINET::iterator itCABINET;
	for( itCABINET = m_mapCabinet.begin(); itCABINET != m_mapCabinet.end(); itCABINET++)
	{
		LPTCABINET pTCABINET = (*itCABINET).second;
		for( itITEM = pTCABINET->m_mapCabinetItem.begin(); itITEM != pTCABINET->m_mapCabinetItem.end(); itITEM++)
			delete (*itITEM).second;

		delete (*itCABINET).second;
	}

	m_mapCabinet.clear();
}

void CTPlayer::ClearHotkey()
{
	MAPTHOTKEYINVEN::iterator itTKEY;

	for( itTKEY = m_mapHotkeyInven.begin(); itTKEY != m_mapHotkeyInven.end(); itTKEY++)
		delete (*itTKEY).second;

	m_mapHotkeyInven.clear();
}

void CTPlayer::AddHotKey( BYTE bInven, BYTE bPos, BYTE bType, WORD wID)
{
	if(bPos >= MAX_HOTKEY_POS)
		return;

	THOTKEY keyChanged[2];
	BYTE bCount = 0;

	MAPTHOTKEYINVEN::iterator it = m_mapHotkeyInven.find(bInven);
	if(it!=m_mapHotkeyInven.end())
	{
		for(BYTE i=0; i<MAX_HOTKEY_POS; i++)
		{
			if(((*it).second->m_hotkey[i].m_bType == bType &&
				(*it).second->m_hotkey[i].m_wID == wID) ||
				(bPos == i && (*it).second->m_hotkey[i].m_bType != HOTKEY_NONE))
			{
				EraseHotKey(bInven, i);
			}
		}
		(*it).second->m_bSave |= ITEM_SAVE_UPDATE;
		keyChanged[bCount].m_bPos = bPos;
		keyChanged[bCount].m_bType = (*it).second->m_hotkey[bPos].m_bType = bType;
		keyChanged[bCount].m_wID = (*it).second->m_hotkey[bPos].m_wID = wID;
		bCount++;
	}
	else
	{
		LPTHOTKEYINVEN pInven = new THOTKEYINVEN();
		pInven->m_bSave = ITEM_SAVE_INSERT;
		keyChanged[bCount].m_bPos = bPos;
		keyChanged[bCount].m_bType = pInven->m_hotkey[bPos].m_bType = bType;
		keyChanged[bCount].m_wID = pInven->m_hotkey[bPos].m_wID = wID;
		m_mapHotkeyInven.insert(MAPTHOTKEYINVEN::value_type(bInven, pInven));
		bCount++;
	}

	if(bType == HOTKEY_SKILL)
	{
		CTSkill * pSkill = FindTSkill(wID);
		if(!pSkill)
		{
			CTSkillTemp * pTSKILL = _AtlModule.FindTSkill(wID);
			if(pTSKILL && pTSKILL->m_bGlobal)
			{
				pSkill = new CTSkill();
				pSkill->m_pTSKILL = pTSKILL;
				m_mapTSKILL.insert(MAPTSKILL::value_type(wID, pSkill));
				SendCS_SKILLBUY_ACK(
					SKILL_SUCCESS,
					wID,
					pSkill->m_bLevel);
			}
		}
	}

	SendCS_HOTKEYCHANGE_ACK(bInven, keyChanged, bCount);
}

void CTPlayer::EraseHotKey(BYTE bInven, BYTE bPos)
{
	if(bPos >= MAX_HOTKEY_POS)
		return;

	BYTE bCount = 0;
	THOTKEY keyChanged[1];

	MAPTHOTKEYINVEN::iterator it = m_mapHotkeyInven.find(bInven);
	if(it!=m_mapHotkeyInven.end())
	{
		if((*it).second->m_hotkey[bPos].m_bType == HOTKEY_SKILL)
		{
			CTSkill * pSkill = FindTSkill((*it).second->m_hotkey[bPos].m_wID);
			if(pSkill && pSkill->m_pTSKILL->m_bGlobal)
			{
				m_mapTSKILL.erase(pSkill->m_pTSKILL->m_wID);
				delete pSkill;
			}
		}

		(*it).second->m_hotkey[bPos].m_bType = HOTKEY_NONE;
		(*it).second->m_hotkey[bPos].m_wID = 0;
		(*it).second->m_bSave |= ITEM_SAVE_UPDATE;

		keyChanged[0].m_bPos = bPos;
		keyChanged[0].m_bType = 0;
		keyChanged[0].m_wID = 0;
		bCount++;
	}

	SendCS_HOTKEYCHANGE_ACK(bInven, keyChanged, bCount);
}

BYTE CTPlayer::IsEmptyHotkey(LPTHOTKEYINVEN pInven)
{
	for(BYTE i=0; i<MAX_HOTKEY_POS; i++)
	{
		if(pInven->m_hotkey[i].m_bType != HOTKEY_NONE)
			return FALSE;
	}
	return TRUE;
}

BYTE CTPlayer::UseMoney(__int64 llMoney, BYTE bUsed)
{
	if(!llMoney)
		return TRUE;

	__int64 llMyMoney = CalcMoney(m_dwGold, m_dwSilver, m_dwCooper);

	if(llMyMoney < llMoney)
		return FALSE;

	if(bUsed)
	{
		llMyMoney -= llMoney;
		CalcMoney(llMyMoney, m_dwGold, m_dwSilver, m_dwCooper);
	}

	return TRUE;
}

BYTE CTPlayer::EarnMoney(__int64 llMoney)
{
	if(!llMoney)
		return FALSE;

	__int64 llMyMoney = llMoney + CalcMoney(m_dwGold, m_dwSilver, m_dwCooper);

	CalcMoney(llMyMoney, m_dwGold, m_dwSilver, m_dwCooper);

	return TRUE;
}

void CTPlayer::OnTimer(DWORD dwTick)
{
	CheckMaintainSkill(dwTick);
	Recover(dwTick);
	ResetAftermath(dwTick);
	if(m_bMode == MT_BATTLE && m_dwLastAtkTick + RECOVER_INIT < dwTick)
		ChgMode(MT_NORMAL);

	CTime timeEnter(m_timeEnter);
	CTime timeCurrent(_AtlModule.m_timeCurrent);
	if(m_bInPcBang)
	{
		if(timeCurrent.GetDay() != timeEnter.GetDay())
		{
			m_dwPcBangTime = 0;
			m_bPcBangItemCnt = 0;
		
			HangPremiumItem();
		}
		else
			m_dwPcBangTime += DWORD(timeCurrent.GetTime() - m_timeEnter);
	}
	CheckDuringItem(timeCurrent.GetTime(), min(DWORD(timeCurrent.GetTime() - m_timeEnter), 10));
	CheckItemEndTime(_AtlModule.m_timeCurrent);

	m_timeEnter = timeCurrent.GetTime();

	if(dwTick - m_dwSaveTick > CHAR_SAVE_TICK)
	{
		m_dwSaveTick = dwTick;
		if(m_bMain)
			_AtlModule.SendDM_SAVECHAR_REQ(this, 0);
	}
}

void CTPlayer::GainExp( DWORD dwGain, BYTE bIsMon)
{
	if(!m_bMain ||
		!dwGain ||
		m_bStatus == OS_DEAD)
		return;

	DWORD dwBonus = 0;
	if(bIsMon)
	{
		DWORD dwVital = GetExpVital();
		dwBonus = min(dwVital, dwGain);
		DWORD dwDiv = m_pTLEVEL->m_dwEXP;
		if(dwDiv == 0) dwDiv = 1;
		m_fVital -= dwBonus*200/dwDiv;
	}

	DWORD dwNewExp = m_dwEXP + dwGain+dwBonus;

	if(m_bLevel >= _AtlModule.m_bMaxLevel)
		return;

	m_dwEXP = dwNewExp;

	BYTE bLevelUp = FALSE;

	if(m_pTLEVEL->m_dwEXP <= m_dwEXP)
	{
		LPTLEVEL pNext = _AtlModule.FindTLevel(m_bLevel+1);
		if(!pNext)
			return;

		if(pNext->m_bLevel >= _AtlModule.m_bMaxLevel)
			m_dwEXP = m_pTLEVEL->m_dwEXP;

		m_bLevel++;
		bLevelUp = TRUE;
		m_pTPREVLEVEL = m_pTLEVEL;
		m_pTLEVEL = pNext;
		m_wSkillPoint += m_pTLEVEL->m_bSkillPoint;
		m_dwHP = GetMaxHP();
		m_dwMP = GetMaxMP();
//		AutoEquipSkill();

		_AtlModule.SendMW_LEVELUP_ACK(
			m_dwID,
			m_dwKEY,
			m_bLevel);

#ifdef DEF_UDPLOG
		_AtlModule.m_pUdpSocket->LogLevelUp(LOGMAP_LEVELUP, this, m_bLevel);
#endif
		if( GetPartyID() )
			_AtlModule.SendMW_PARTYMANSTAT_ACK( GetPartyID(), m_dwID, m_bType, m_bLevel, GetMaxHP(), m_dwHP, GetMaxMP(), m_dwMP);

		SendCS_CHARSTATINFO_ACK(this);
		if(m_pMAP)
		{
			VPLAYER vPlayers;
			vPlayers.clear();

			m_pMAP->GetNeighbor(
				&vPlayers,
				m_fPosX,
				m_fPosZ);

			while(!vPlayers.empty())
			{
				CTPlayer * pChar = vPlayers.back();
				pChar->SendCS_LEVEL_ACK(
					m_dwID,
					m_bLevel);

				pChar->SendCS_HPMP_ACK(
					m_dwID,
					m_bType,
					m_bLevel,
					GetMaxHP(),
					m_dwHP,
					GetMaxMP(),
					m_dwMP);

				vPlayers.pop_back();
			}

			vPlayers.clear();
		}

		_AtlModule.CheckQuest(
			this,
			0,
			m_fPosX,
			m_fPosY,
			m_fPosZ,
			m_bLevel,
			0,
			TT_LEVELUP,
			1);
	}

	if(bLevelUp && _AtlModule.CheckMapLevel(this))
		SendCS_KICKOUTMAP_ACK();

	SendCS_EXP_ACK();
}

BYTE CTPlayer::UseExp(DWORD dwExp, BYTE bUsed)
{
	if(!dwExp)
		return FALSE;

	if(!m_pTPREVLEVEL)
		return FALSE;

	DWORD dwRemain = m_dwEXP - m_pTPREVLEVEL->m_dwEXP;
	if(dwExp >= dwRemain)
		return FALSE;

	if(bUsed)
		m_dwEXP -= dwExp;

	return TRUE;
}

void CTPlayer::AutoEquipSkill()
{
	MAPTSKILLTEMP::iterator it;

	for(it=_AtlModule.m_mapTEQUIPSKILL.begin(); it!=_AtlModule.m_mapTEQUIPSKILL.end(); it++)
	{
		CTSkillTemp * pTemp = (*it).second;
		if(!FindTSkill(pTemp->m_wID) &&
			m_bLevel == pTemp->m_bStartLevel &&
			pTemp->m_bCanLearn &&
			(pTemp->m_dwClassID & BITSHIFTID(m_bClass)))
		{
			CTSkill * pSkill = new CTSkill();
			pSkill->m_pTSKILL = pTemp;
			m_mapTSKILL.insert(MAPTSKILL::value_type(pTemp->m_wID, pSkill));
			SendCS_SKILLBUY_ACK(
				SKILL_SUCCESS,
				pTemp->m_wID, 1);
		}
	}
}

WORD CTPlayer::GetCommanderID()
{
	return m_bPartyType == PT_SOLO ? 0 : m_wCommanderID;
}

DWORD CTPlayer::GetPartyChiefID()
{
	return m_bPartyType == PT_SOLO ? 0 : m_dwPartyChiefID;
}

BYTE CTPlayer::GetLevel()
{
	return m_bLevel;
}

BYTE CTPlayer::GetClass()
{
	return m_bClass;
}

LPTCABINET CTPlayer::GetCabinet(BYTE bCabinetID)
{
	MAPTCABINET::iterator find = m_mapCabinet.find(bCabinetID);
	if(find!=m_mapCabinet.end())
		return (*find).second;

	return NULL;
}

DWORD CTPlayer::GetCabinetItemIndex(LPTCABINET pCabinet)
{
	if(pCabinet->m_mapCabinetItem.size())
	{
		MAPTSTORAGEITEM::iterator it = pCabinet->m_mapCabinetItem.end();
		it--;
		return (*it).second->m_dwStItemID + 1;
	}
	else
		return 1;
}

CTItem* CTPlayer::GetCabinetItem(BYTE bCabinetID, DWORD dwStItemID)
{
	MAPTCABINET::iterator find = m_mapCabinet.find(bCabinetID);
	if(find==m_mapCabinet.end())
		return NULL;

	MAPTSTORAGEITEM::iterator itITEM = (*find).second->m_mapCabinetItem.find(dwStItemID);
	if(itITEM!=(*find).second->m_mapCabinetItem.end())
		return (*itITEM).second;

	return NULL;
}

void CTPlayer::FindCabinetItem(BYTE bCabinetID, VTITEM &vItem, CTItem * pItem)
{
	MAPTCABINET::iterator find = m_mapCabinet.find(bCabinetID);
	if(find==m_mapCabinet.end())
		return;

	LPTCABINET pTCABINET = (*find).second;

	MAPTSTORAGEITEM::iterator it;
	for(it=pTCABINET->m_mapCabinetItem.begin(); it!=pTCABINET->m_mapCabinetItem.end(); it++)
	{
		if(*((*it).second) == *pItem)
			vItem.push_back((*it).second);
	}
}

void CTPlayer::EraseCabinetItem(BYTE bCabinetID, DWORD dwStItemID)
{
	MAPTCABINET::iterator find = m_mapCabinet.find(bCabinetID);
	if(find!=m_mapCabinet.end())
	{
		MAPTSTORAGEITEM::iterator itITEM = (*find).second->m_mapCabinetItem.find(dwStItemID);
		if(itITEM!=(*find).second->m_mapCabinetItem.end())
		{
			delete (*itITEM).second;
			(*find).second->m_mapCabinetItem.erase(itITEM);
		}
	}
}

LPTCABINET CTPlayer::PutinCabinet(BYTE bCabinetID, BYTE bUse)
{
	LPTCABINET pTCABINET = new TCABINET;
	pTCABINET->m_bCabinetID = bCabinetID;
	pTCABINET->m_bUse = bUse;
	m_mapCabinet.insert(MAPTCABINET::value_type(bCabinetID, pTCABINET));

	return pTCABINET;
}

BYTE CTPlayer::PutinCabinetItem(BYTE bCabinetID, BYTE bInven, BYTE bItemID, BYTE bCount)
{
	MAPTCABINET::iterator find = m_mapCabinet.find(bCabinetID);
	if(find==m_mapCabinet.end())
		return FALSE;

	LPTCABINET pTCABINET = (*find).second;

	if(bInven == INVEN_EQUIP)
		return FALSE;

	VTITEM vItem;
	vItem.clear();

	CTInven * pInven = FindTInven(bInven);
	CTItem * pItem=NULL;
	if(pInven)
		pItem = pInven->FindTItem(bItemID);

	if(!pItem || pItem->m_bCount < bCount)
		return FALSE;

	if(!(pItem->m_pTITEM->m_bIsSell & ITEMTRADE_CABINET))
	{
		SendCS_CABINETITEMLIST_ACK(CABINET_NOTUSE, pTCABINET);
		return FALSE;
	}

#ifdef DEF_UDPLOG
	_AtlModule.m_pUdpSocket->LogCabinet(LOGMAP_ITEMCABINETIN, this, bCabinetID, pItem, bCount);
#endif

	BYTE bTotalCount = 0;

	FindCabinetItem(bCabinetID, vItem, pItem);
	for(DWORD i=0; i<vItem.size(); i++)
	{
		BYTE bAddCount = vItem[i]->m_pTITEM->m_bStack - vItem[i]->m_bCount;
		bAddCount = min( bCount, bAddCount);

		if(bAddCount > 0)
		{
			vItem[i]->m_bCount += bAddCount;
			pItem->m_bCount -= bAddCount;
			bTotalCount += bAddCount;

			if(bTotalCount == bCount)
				break;
		}
	}

	if(bTotalCount < bCount && pTCABINET->m_mapCabinetItem.size() < CABINET_STORAGE_MAX)
	{
		CTItem * pNew = new CTItem();
		BYTE bAddCount = min( pItem->m_pTITEM->m_bStack, pItem->m_bCount);
		pNew->Copy(pItem, pItem->m_bCount-bAddCount);

		pNew->m_bCount = bAddCount;
		pNew->m_dwStItemID = GetCabinetItemIndex(pTCABINET);
		pNew->m_bItemID = bCabinetID;
		pItem->m_bCount -= bAddCount;
		pTCABINET->m_mapCabinetItem.insert(MAPTSTORAGEITEM::value_type(pNew->m_dwStItemID, pNew));
	}

	if(pItem->m_bCount == 0)
	{
		SendCS_DELITEM_ACK(
			bInven,
			bItemID);

		pInven->m_mapTITEM.erase(bItemID);
		delete pItem;
	}
	else
	{
		SendCS_UPDATEITEM_ACK(
			pItem,
			bInven);
	}

	SendCS_CABINETITEMLIST_ACK(CABINET_SUCCESS, pTCABINET);

	vItem.clear();

	return TRUE;
}

BYTE CTPlayer::TakeoutStorageItem(BYTE bCabinetID, DWORD dwStItemID, BYTE bCount, DWORD dwMoney, BYTE bInvenID, BYTE bItemID)
{
	MAPTCABINET::iterator find = m_mapCabinet.find(bCabinetID);
	if(find==m_mapCabinet.end())
		return FALSE;

	LPTCABINET pTCABINET = (*find).second;

	if(!pTCABINET->m_bUse)
	{
		SendCS_CABINETITEMLIST_ACK(CABINET_NOTUSE, NULL);
		return EC_NOERROR;
	}

	if(bInvenID == INVEN_EQUIP)
		return FALSE;

	CTInven * pTInvenDEST = FindTInven(bInvenID);
	if(!pTInvenDEST)
		return FALSE;
	
	CTItem * pItem = GetCabinetItem(bCabinetID, dwStItemID);
	if(!pItem || pItem->m_bCount != bCount)
		return FALSE;

	BYTE bResult = FALSE;
	CTItem logitem;
	logitem.Copy(pItem, FALSE);

	CTItem *pTItemDEST = pTInvenDEST->FindTItem(bItemID);
	if(!pTItemDEST)
	{
		pTItemDEST = new CTItem();

		pTItemDEST->Copy(pItem, FALSE);
		pTItemDEST->m_bItemID = bItemID;

		pTItemDEST->m_bCount = bCount;

		pTInvenDEST->m_mapTITEM.insert( MAPTITEM::value_type( pTItemDEST->m_bItemID, pTItemDEST));
		SendCS_ADDITEM_ACK(
			pTItemDEST,
			pTInvenDEST->m_bInvenID);

		bResult = TRUE;
	}
	else
	{
		VTITEM vITEM;
		vITEM.clear();

		CTItem * pNew;
		pNew = new CTItem();
		pNew->Copy(pItem, FALSE);

		vITEM.push_back(pNew);
		if(CTObjBase::CanPush(&vITEM, 0))
		{
			PushTItem(&vITEM);
			vITEM.clear();
			bResult = TRUE;
		}
		else
		{
			delete pNew;
			vITEM.clear();
		}
	}

	if(bResult)
	{
#ifdef DEF_UDPLOG
		_AtlModule.m_pUdpSocket->LogCabinet(LOGMAP_ITEMCABINETOUT, this, bCabinetID, &logitem, bCount, -(int)dwMoney);
#endif

		EraseCabinetItem(bCabinetID, dwStItemID);
		SendCS_CABINETITEMLIST_ACK(CABINET_SUCCESS, pTCABINET);

		if(dwMoney)
		{
			UseMoney(dwMoney, TRUE);
			SendCS_MONEY_ACK();
		}
	}

	return bResult;
}


BYTE CTPlayer::PushTItem( CTItem * pTItem, BYTE bInvenID, BYTE bSlotID)
{
	if(bSlotID == INVALID_SLOT)
	{
		VTITEM vITEM;
		vITEM.clear();
		vITEM.push_back(pTItem);
		if(CanPush(&vITEM, 0))
		{
			PushTItem(&vITEM);
			vITEM.clear();
			return TRUE;
		}
		else
		{
			vITEM.clear();
			return FALSE;
		}
	}

	CTInven * pInven = FindTInven(bInvenID);
	if(!pInven)
		return FALSE;

	CTItem * pItem = pInven->FindTItem(bSlotID);
	if(!pItem)
	{
		pTItem->m_bItemID = bSlotID;
		pTItem->m_bInven = bInvenID;

		if(!pTItem->m_dlID)
			pTItem->m_dlID = _AtlModule.GenItemID();

		pInven->m_mapTITEM.insert(MAPTITEM::value_type( pTItem->m_bItemID, pTItem));
		SendCS_ADDITEM_ACK(
			pTItem,
			pInven->m_bInvenID);

		return TRUE;
	}

	if((*pItem) == (*pTItem) &&
		pItem->m_bCount < pItem->m_pTITEM->m_bStack)
	{
		if(pItem->m_bCount + pTItem->m_bCount <= pItem->m_pTITEM->m_bStack)
		{
			pItem->m_bCount += pTItem->m_bCount;
			SendCS_UPDATEITEM_ACK(
				pItem,
				pInven->m_bInvenID);
			delete pTItem;
			return TRUE;
		}
	}

	return FALSE;
}
void CTPlayer::PushTItem( LPVTITEM pTITEM)
{
	int nIndex = 0;

	while(nIndex < INT(pTITEM->size()))
	{
		CTInven *pTINVEN = FindTInven(INVEN_DEFAULT);
		CTItem *pTItem = (*pTITEM)[nIndex];

		BYTE bItemID = pTINVEN ? pTINVEN->GetEasePos(pTItem) : INVALID_SLOT;
		BYTE bCount = 0;

		if( bItemID == INVALID_SLOT )
		{
			MAPTINVEN::iterator itTINVEN;

			for( itTINVEN = m_mapTINVEN.begin(); itTINVEN != m_mapTINVEN.end(); itTINVEN++)
				if( (*itTINVEN).second->m_bInvenID != INVEN_DEFAULT &&
					(*itTINVEN).second->m_bInvenID != INVEN_EQUIP )
				{
					bItemID = (*itTINVEN).second->GetEasePos(pTItem);

					if( bItemID != INVALID_SLOT )
					{
						pTINVEN = (*itTINVEN).second;
						break;
					}
				}
		}

		if( bItemID == INVALID_SLOT )
		{
			pTINVEN = FindTInven(INVEN_DEFAULT);
			bItemID = pTINVEN ? pTINVEN->GetBlankPos() : INVALID_SLOT;
		}

		if( bItemID == INVALID_SLOT )
		{
			MAPTINVEN::iterator itTINVEN;

			for( itTINVEN = m_mapTINVEN.begin(); itTINVEN != m_mapTINVEN.end(); itTINVEN++)
				if( (*itTINVEN).second->m_bInvenID != INVEN_DEFAULT &&
					(*itTINVEN).second->m_bInvenID != INVEN_EQUIP )
				{
					bItemID = (*itTINVEN).second->GetBlankPos();

					if( bItemID != INVALID_SLOT )
					{
						pTINVEN = (*itTINVEN).second;
						break;
					}
				}
		}

		CTItem *pDESTItem = pTINVEN->FindTItem(bItemID);
		if(pDESTItem)
		{
			bCount = pDESTItem->m_pTITEM->m_bStack - pDESTItem->m_bCount;
			bCount = min( pTItem->m_bCount, bCount);

			pDESTItem->m_bCount += bCount;
			pTItem->m_bCount -= bCount;

			SendCS_UPDATEITEM_ACK(
				pDESTItem,
				pTINVEN->m_bInvenID);
		}
		else
		{
			pDESTItem = new CTItem();

			bCount = min( pTItem->m_pTITEM->m_bStack, pTItem->m_bCount);
			pDESTItem->Copy(pTItem, !pTItem->m_dlID || pTItem->m_bCount > bCount);
			pDESTItem->m_bItemID = bItemID;
			pDESTItem->m_bCount = bCount;
			pTItem->m_bCount -= bCount;
			pTINVEN->m_mapTITEM.insert( MAPTITEM::value_type( pDESTItem->m_bItemID, pDESTItem));
			SendCS_ADDITEM_ACK(
				pDESTItem,
				pTINVEN->m_bInvenID);
		}
		pDESTItem->m_bInven = pTINVEN->m_bInvenID;

		if(!pTItem->m_bCount)
		{
			delete pTItem;
			pTItem = NULL;

			nIndex++;
		}
	}

	pTITEM->clear();
}

BYTE CTPlayer::HaveInvenBlank()
{
	MAPTINVEN::iterator itTINVEN;

	for( itTINVEN = m_mapTINVEN.begin(); itTINVEN != m_mapTINVEN.end(); itTINVEN++)
		if( (*itTINVEN).second->m_bInvenID != INVEN_EQUIP &&
			(*itTINVEN).second->GetBlankPos() != INVALID_SLOT)
		{
			return TRUE;
		}

	return FALSE;
}

void CTPlayer::Defend( CTSkillTemp * pTemp,
					    BYTE bSkillLevel,
						DWORD dwHostID,
					    DWORD dwAttackID,
					    BYTE bAttackType,
						WORD wAttackPartyID,
						BYTE bAttackCountry,
						BYTE bAttackAidCountry,
						BYTE bAttackClass,
						DWORD dwActID,
						DWORD dwAniID,
						DWORD dwPysMinPower,
						DWORD dwPysMaxPower,
						DWORD dwMgMinPower,
						DWORD dwMgMaxPower,
						WORD wTransHP,
						WORD wTransMP,
						BYTE bCurseProb,
						BYTE bEquipSpecial,
						BYTE bCanSelect,
						WORD wAttackLevel,
						BYTE bAttackerLevel,
						BYTE bCP,
						BYTE bHitType,
						DWORD dwRemainTick,
						FLOAT fAtkPosX,
						FLOAT fAtkPosY,
						FLOAT fAtkPosZ,
						FLOAT fDefPosX,
						FLOAT fDefPosY,
						FLOAT fDefPosZ,
						BYTE bMirror)
{
	if(m_bMode != MT_BATTLE &&
		pTemp->IsNegative())
		ChgMode(MT_BATTLE);

	m_dwLastAtkTick = _AtlModule.m_dwTick;

	CTObjBase::Defend(
		pTemp,
		bSkillLevel,
		dwHostID,
		dwAttackID,
		bAttackType,
		wAttackPartyID,
		bAttackCountry,
		bAttackAidCountry,
		bAttackClass,
		dwActID,
		dwAniID,
		dwPysMinPower,
		dwPysMaxPower,
		dwMgMinPower,
		dwMgMaxPower,
		wTransHP,
		wTransMP,
		bCurseProb,
		bEquipSpecial,
		bCanSelect,
		wAttackLevel,
		bAttackerLevel,
		bCP,
		bHitType,
		dwRemainTick,
		fAtkPosX,fAtkPosY,fAtkPosZ,
		fDefPosX,fDefPosY,fDefPosZ,
		bMirror);
}

void CTPlayer::OnDie( DWORD dwAttackID, BYTE bObjectType, WORD wTempMonID )
{
#ifdef	DEF_UDPLOG
	_AtlModule.m_pUdpSocket->LogUserDie(LOGMAP_USERDIE, this, m_bType);
#endif

	DropGodBall();
	MapKillPoint();

	MAPTRECALLMON::iterator it;
	for(it=m_mapRecallMon.begin(); it!=m_mapRecallMon.end(); it++)
	{
		(*it).second->OnDie(0,OT_NONE,0);
		_AtlModule.SendMW_RECALLMONDEL_ACK(m_dwID, m_dwKEY, (*it).second->m_dwID);
	}

	MAPTSELFOBJ::iterator itSf;
	while(!m_mapSelfMon.empty())
	{
		itSf=m_mapSelfMon.begin();
		(*itSf).second->OnDie(0,OT_NONE,0); 
		DeleteSelfMon((*itSf).second->m_dwID);
	}

	CTObjBase::OnDie(dwAttackID,bObjectType,wTempMonID); 

	if(m_wArenaID && m_bArenaTeam)
	{
		MAPARENA::iterator itArn = _AtlModule.m_mapArena.find(m_wArenaID);
		if(itArn != _AtlModule.m_mapArena.end())
		{
			BYTE bLose = TRUE;
			LPTARENA pArena = itArn->second;
			MAPPLAYER::iterator itP;
			for(itP=pArena->m_mapFighter[m_bArenaTeam-1].begin(); itP != pArena->m_mapFighter[m_bArenaTeam-1].end(); itP++)
			{
				if(itP->second->m_bStatus != OS_DEAD)
				{
					bLose = FALSE;
					break;
				}
			}

			if(bLose)
				_AtlModule.ArenaEnd(pArena, NULL);
		}

		return;
	}

	if(bObjectType != OT_MON && dwAttackID && dwAttackID != m_dwID)
		_AtlModule.PvPEvent(PVPE_KILL_E, this, dwAttackID);
}

void CTPlayer::MapKillPoint()
{
	if(!m_pMAP)
		return;

	LPTLOCAL pCastle = m_pMAP->m_pCastle;
	if(pCastle && pCastle->m_bStatus == BS_BATTLE)
	{
		if(pCastle->m_dwDefGuildID == GetGuild())
			m_pMAP->m_wAtkKillPoint++;
		else
			m_pMAP->m_wDefKillPoint++;
	}
	else if(m_pMAP->m_mapTNMTPlayer.size())
	{
		MAPTOURNAMENTPLAYER::iterator it = m_pMAP->m_mapTNMTPlayer.find(m_dwID);
		if(it!=m_pMAP->m_mapTNMTPlayer.end())
		{
			if(m_pMAP->m_bBlueTeam == (*it).second->m_bSlot)
				m_pMAP->m_wAtkKillPoint++;
			else
				m_pMAP->m_wDefKillPoint++;
		}
	}
}

void CTPlayer::DropGodBall()
{
	if(m_wGodBall && m_pMAP)
	{
		LPTGODBALL pBall = m_pMAP->FindMapGodBall(m_wGodBall);
		if(pBall)
		{
			pBall->m_fPosX = m_fPosX;
			pBall->m_fPosY = m_fPosY;
			pBall->m_fPosZ = m_fPosZ;
			RemoveGodBall(pBall);

			m_pMAP->EnterMAP(pBall);
			_AtlModule.DoGBCMD(GB_DROPBALL, pBall);
		}
	}
}

BYTE CTPlayer::CheckLevelCondition(LPQUESTTEMP pQUEST, DWORD dwTick, BYTE& bLevel)
{
	BYTE bResult = QCT_NONE;
	for( int i=0; i<INT(pQUEST->m_vCondition.size()); i++)
	{
		bResult = pQUEST->m_vCondition[i]->m_bConditionType;

		switch(pQUEST->m_vCondition[i]->m_bConditionType)
		{
		case QCT_UPPERLEVEL		: 
			bResult = m_bLevel >= pQUEST->m_vCondition[i]->m_dwConditionID ? QCT_NONE : QCT_UPPERLEVEL;
			bLevel = BYTE(pQUEST->m_vCondition[i]->m_dwConditionID);
			break;
		case QCT_LOWERLEVEL		: bResult = m_bLevel <= pQUEST->m_vCondition[i]->m_dwConditionID ? QCT_NONE : QCT_LOWERLEVEL; 
			break;
		default:
			bResult = QCT_NONE; break;
		}

		if(bResult)
			return bResult;
	}

	return bResult;
}


BYTE CTPlayer::CheckQuestCondition( LPQUESTTEMP pQUEST, DWORD dwTick, BYTE& bLevel)
{
	BYTE bLevelCondition = CheckLevelCondition(pQUEST,dwTick,bLevel);
	BYTE bResult = QCT_NONE;
	for( int i=0; i<INT(pQUEST->m_vCondition.size()); i++)
	{
		bResult = pQUEST->m_vCondition[i]->m_bConditionType;

		switch(pQUEST->m_vCondition[i]->m_bConditionType)
		{
		case QCT_UPPERLEVEL		:
		case QCT_LOWERLEVEL		:
				bResult = QCT_NONE;
				break;
		case QCT_MAPID			: bResult = m_wMapID == pQUEST->m_vCondition[i]->m_dwConditionID ? QCT_NONE : QCT_MAPID; break;
		case QCT_LEFT			: bResult = m_fPosX >= pQUEST->m_vCondition[i]->m_dwConditionID ? QCT_NONE : QCT_LEFT; break;
		case QCT_TOP			: bResult = m_fPosZ <= pQUEST->m_vCondition[i]->m_dwConditionID ? QCT_NONE : QCT_TOP; break;
		case QCT_RIGHT			: bResult = m_fPosX <= pQUEST->m_vCondition[i]->m_dwConditionID ? QCT_NONE : QCT_RIGHT; break;
		case QCT_BOTTOM			: bResult = m_fPosZ >= pQUEST->m_vCondition[i]->m_dwConditionID ? QCT_NONE : QCT_BOTTOM; break;
		case QCT_PROB			: bResult = (DWORD)(rand() % 100) < pQUEST->m_vCondition[i]->m_dwConditionID ? QCT_NONE : QCT_PROB; break;
		case QCT_CLASS			: bResult = pQUEST->m_vCondition[i]->m_dwConditionID & BITSHIFTID(m_bClass) ? QCT_NONE : QCT_CLASS; break;
		case QCT_HAVEQUEST		:
			{
				MAPQUEST::iterator finder = m_mapQUEST.find(pQUEST->m_vCondition[i]->m_dwConditionID);
				CQuest * pQParent = NULL;
				if( finder != m_mapQUEST.end() )
					pQParent = (*finder).second;

				if( pQParent && pQParent->m_bCompleteCount < pQParent->m_bTriggerCount )
				{
					if(pQUEST->m_bType == QT_DROPITEM)
					{
						bResult = QCT_NONE;
						for(DWORD i=0; i<pQParent->m_pQUEST->m_vTerm.size(); i++)
							for(DWORD k=0; k<pQUEST->m_vTerm.size(); k++)
								if(pQParent->m_pQUEST->m_vTerm[i]->m_dwTermID == pQUEST->m_vTerm[k]->m_dwTermID &&
								   pQParent->CheckComplete(this, dwTick, pQParent->m_pQUEST->m_vTerm[i]) == QTS_SUCCESS)
								{
									bResult = QCT_HAVEQUEST;
									break;
								}
					}
					else
					{
						LPQUESTTERM pTERM = pQParent->CheckComplete( this, dwTick);
						bResult = pQUEST->m_bType == QT_COMPLETE || (pTERM && pTERM->m_bTermType != QTT_TIMER) ? QCT_NONE : QCT_HAVEQUEST;
					}
				}
			}

			break;

		case QCT_HAVEITEM		:
			{
				MAPTITEM::iterator it;
				MAPTINVEN::iterator itInv;
				for(itInv=m_mapTINVEN.begin(); itInv!=m_mapTINVEN.end(); itInv++)
				{
					CTInven * pInven = (*itInv).second;
					for( it = pInven->m_mapTITEM.begin(); it != pInven->m_mapTITEM.end(); it++)
					{
						if( (*it).second->m_pTITEM->m_wItemID == pQUEST->m_vCondition[i]->m_dwConditionID )
						{
							bResult = QCT_NONE;
							break;
						}
					}

					if(bResult == QCT_NONE)
						break;
				}
			}

			break;

		case QCT_HAVENOITEM:
			{
				MAPTITEM::iterator it;

				bResult = QCT_NONE;
				MAPTINVEN::iterator itInv;
				for(itInv=m_mapTINVEN.begin(); itInv!=m_mapTINVEN.end(); itInv++)
				{
					CTInven * pInven = (*itInv).second;
					for( it = pInven->m_mapTITEM.begin(); it != pInven->m_mapTITEM.end(); it++)
					{
						if( (*it).second->m_pTITEM->m_wItemID == pQUEST->m_vCondition[i]->m_dwConditionID )
						{
							bResult = QCT_HAVENOITEM;
							break;
						}
					}

					if(bResult != QCT_NONE)
						break;
				}
			}
			break;
		case QCT_COUNTRY:
			{
				if(pQUEST->m_vCondition[i]->m_dwConditionID == GetWarCountry())
					bResult = QCT_NONE;
				else
					bResult = QCT_COUNTRY;
			}
			break;
		case QCT_AFTERQUESTCOMPLETE:
			{
				bResult = QCT_NONE;
				MAPQUEST::iterator finder = m_mapQUEST.find(pQUEST->m_vCondition[i]->m_dwConditionID);
				if(finder == m_mapQUEST.end())
					bResult = QCT_AFTERQUESTCOMPLETE;
				else
				{
					if(!(*finder).second->m_bCompleteCount ||
						(*finder).second->m_bCompleteCount < pQUEST->m_vCondition[i]->m_bCount)
						bResult = QCT_AFTERQUESTCOMPLETE;
				}
			}
			break;
		case QCT_SAMELEVEL:
			{
				MAPDWORD::iterator finder = m_mapLevelQuest.find(pQUEST->m_vCondition[i]->m_dwConditionID);
				if(finder == m_mapLevelQuest.end())
					bResult = QCT_NONE;
			}
			break;
		case QCT_SEX:
			{
				if(pQUEST->m_vCondition[i]->m_dwConditionID == m_bSex)
					bResult = QCT_NONE;
			}
			break;
		case QCT_BEFOREQUESTCOMPLETE:
			{
				MAPQUEST::iterator finder = m_mapQUEST.find(pQUEST->m_vCondition[i]->m_dwConditionID);
				if(finder == m_mapQUEST.end())
				{
					bResult = QCT_NONE;
					break;
				}

				if((*finder).second->m_bCompleteCount < (*finder).second->m_bTriggerCount )
				{
					bResult = QCT_NONE;
					break;
				}
			}
			break;
		case QCT_MAINTAINSKILL:
			{
				if(FindMaintainSkill(WORD(pQUEST->m_vCondition[i]->m_dwConditionID)))
					bResult = QCT_NONE;
			}
			break;
		case QCT_SWITCH:
			{
				LPTSWITCH pSwitch = m_pMAP->FindSwitch(pQUEST->m_vCondition[i]->m_dwConditionID);
				if(pSwitch && pSwitch->m_bOpened == pQUEST->m_vCondition[i]->m_bCount)
					bResult = QCT_NONE;
			}
			break;
		}

		if( bResult && !pQUEST->m_bConditionCheck )
			return bResult;

		if( !bResult && pQUEST->m_bConditionCheck && !bLevelCondition)
			return QCT_NONE;
	}

	return bResult ? bResult : bLevelCondition;
}
DWORD CTPlayer::GetQuestLevel(LPQUESTTEMP pQUEST)
{
	for( int i=0; i<INT(pQUEST->m_vCondition.size()); i++)
	{
		if(pQUEST->m_vCondition[i]->m_bConditionType == QCT_SAMELEVEL)
			return pQUEST->m_vCondition[i]->m_dwConditionID;
	}
	return FALSE;
}

void CTPlayer::DropQuest(DWORD dwQuestID)
{
	MAPQUEST::iterator finder = m_mapQUEST.find(dwQuestID);
	if( finder != m_mapQUEST.end())
	{
		DWORD dwLevel = GetQuestLevel((*finder).second->m_pQUEST);
		if(dwLevel)
			m_mapLevelQuest.erase(dwLevel);

		if((*finder).second->m_bTriggerCount > (*finder).second->m_bCompleteCount )
		{
			(*finder).second->m_bTriggerCount--;
			(*finder).second->m_dwBeginTick = 0;
			(*finder).second->m_dwTick = 0;
			(*finder).second->m_bSave = 1;

			for(DWORD i=0; i < (*finder).second->m_vTERM.size(); i++)
			{
				if((*finder).second->m_vTERM[i]->m_bTermType == QTT_HUNT ||
					(*finder).second->m_vTERM[i]->m_bTermType == QTT_TALK ||
					(*finder).second->m_vTERM[i]->m_bTermType == QTT_USEITEM)
					(*finder).second->m_vTERM[i]->m_bCount = 0;
			}
		}
	}
}

CQuest * CTPlayer::FindQuest( DWORD dwQuestID)
{
	MAPQUEST::iterator finder = m_mapQUEST.find(dwQuestID);

	if( finder == m_mapQUEST.end() )
		return NULL;
	else
		return (*finder).second;
}

BYTE CTPlayer::IsRunningQuest( DWORD dwQuestID)
{
	MAPQUEST::iterator finder = m_mapQUEST.find(dwQuestID);

	if( finder == m_mapQUEST.end() )
		return FALSE;

	return (*finder).second->m_bCompleteCount < (*finder).second->m_bTriggerCount ? TRUE : FALSE;
}
BYTE CTPlayer::CanRunQuest( LPQUESTTEMP pQUEST, DWORD dwTick, BYTE& bLevel)
{
	MAPQUEST::iterator finder = m_mapQUEST.find(pQUEST->m_dwQuestID);
	if( finder != m_mapQUEST.end())
	{
		if(pQUEST->m_bCountMax)
		{
			if((*finder).second->m_bTriggerCount >= pQUEST->m_bCountMax)
				return QCT_COUNTMAX;
		}
		else
		{
			if((*finder).second->m_bTriggerCount > (*finder).second->m_bCompleteCount)
				return QCT_COUNTMAX;
		}
	}

	if(pQUEST->m_dwParentID)
	{
		finder = m_mapQUEST.find(pQUEST->m_dwParentID);

		if( finder == m_mapQUEST.end() )
			return QCT_NOPARENT;

		if( pQUEST->m_bType == QT_COMPLETE && (
			!IsRunningQuest(pQUEST->m_dwParentID) ||
			(*finder).second->m_pQUEST->m_bCountMax &&
			(*finder).second->m_bCompleteCount >= (*finder).second->m_pQUEST->m_bCountMax))
			return QCT_COUNTMAX;

		if( pQUEST->m_bType == QT_MISSION && (
			(*finder).second->m_bTriggerCount == 0 ||
			(*finder).second->m_bTriggerCount > (*finder).second->m_bCompleteCount ))
			return QCT_COUNTMAX;
	}

	BYTE bResult  = CheckQuestCondition( pQUEST, dwTick, bLevel);
	if(bResult)
		return bResult;

	return IsRunningQuest(pQUEST->m_dwQuestID) ? QCT_COUNTMAX : QCT_NONE;
}

BYTE CTPlayer::OnQuestComplete( LPMAPTSKILLTEMP pSKILLTEMP,
							   LPMAPTITEMTEMP pITEMTEMP,
							   LPMAPCLASS pCLASS,
							   LPQUESTTEMP pQUEST,
							   BYTE bRewardType,
							   DWORD dwRewardID)
{
	VTITEM vITEM;
	vITEM.clear();

	BYTE bRandTake = BYTE(rand() % 100);
	BYTE bRandSum = 0;

	for( int i=0; i<INT(pQUEST->m_vReward.size()); i++)
		if( pQUEST->m_vReward[i]->m_bRewardType == RT_ITEM ||
			pQUEST->m_vReward[i]->m_bRewardType == RT_MAGICITEM )
		{
			BYTE bTake = FALSE;

			switch(pQUEST->m_vReward[i]->m_bTakeMethod)
			{
			case RM_SELECT	: bTake = pQUEST->m_vReward[i]->m_bRewardType == bRewardType && pQUEST->m_vReward[i]->m_dwRewardID == dwRewardID ? TRUE : FALSE; break;
			case RM_PROB	: bTake = (rand() % 100) < pQUEST->m_vReward[i]->m_bTakeData ? TRUE : FALSE; break;
			case RM_RANDOM	:
				bRandSum += pQUEST->m_vReward[i]->m_bTakeData;
				if(bRandSum > bRandTake) bTake = TRUE;
				break;
			case RM_DEFAULT	: bTake = TRUE; break;
			}

			if(bTake)
			{
				WORD wNewItemID = 0;
				if(pQUEST->m_vReward[i]->m_bRewardType == RT_MAGICITEM)
				{
					if(pQUEST->m_vReward[i]->m_pMagicItem)
						wNewItemID = pQUEST->m_vReward[i]->m_pMagicItem->m_pTITEM->m_wItemID;
				}
				else
					wNewItemID = (WORD)pQUEST->m_vReward[i]->m_dwRewardID;

				MAPTITEMTEMP::iterator finder = pITEMTEMP->find(wNewItemID);

				if( finder != pITEMTEMP->end() &&
					((*finder).second->m_dwClassID & BITSHIFTID(m_bClass)))
				{
					int nCount = pQUEST->m_vReward[i]->m_bCount;

					while( nCount > 0 )
					{
						CTItem *pITEM = new CTItem();
						BYTE bTemp = nCount > (*finder).second->m_bStack ? (*finder).second->m_bStack : nCount;

						if(pQUEST->m_vReward[i]->m_bRewardType == RT_MAGICITEM)
						{
							if(pQUEST->m_vReward[i]->m_pMagicItem)
							{
								pITEM->Copy(pQUEST->m_vReward[i]->m_pMagicItem, TRUE);
								pITEM->SetEndTime(DWORD(pITEM->m_dEndTime) * DAY_ONE);
								pITEM->m_bCount = bTemp;
							}
						}
						else
						{
							pITEM->m_dlID = _AtlModule.GenItemID();
							pITEM->m_pTITEM = (*finder).second;
							_AtlModule.SetItemAttr(pITEM, 0);
							pITEM->SetDuration(FALSE);
							pITEM->m_wItemID = pITEM->m_pTITEM->m_wItemID;
							pITEM->m_bCount = bTemp;
						}

						vITEM.push_back(pITEM);
						nCount -= bTemp;
					}
				}

				if(bRandSum) break;
			}
		}

	if(!CanPush(&vITEM, 0))
	{
		while(!vITEM.empty())
		{
			delete vITEM.back();
			vITEM.pop_back();
		}

		vITEM.clear();

		return QR_INVENTORYFULL;
	}

	if(!vITEM.empty())
	{
		VWORD vItemID;
		VBYTE vCount;

		vItemID.clear();
		vCount.clear();

		for(DWORD i=0; i<vITEM.size(); i++)
		{
			vItemID.push_back(vITEM[i]->m_wItemID);
			vCount.push_back(vITEM[i]->m_bCount);
#ifdef	DEF_UDPLOG
			_AtlModule.m_pUdpSocket->LogItemByNPC(LOGMAP_ITEMQUESTGET, this, NULL, vITEM[i], 0, pQUEST);
#endif	//	DEF_UDPLOST			
		}

		PushTItem(&vITEM);
		vITEM.clear();

		for(size_t i=0; i<vItemID.size(); i++)
			_AtlModule.CheckQuest(
				this,
				0,
				m_fPosX,
				m_fPosY,
				m_fPosZ,
				vItemID[i],
				QTT_GETITEM,
				TT_GETITEM,
				vCount[i]);

		vItemID.clear();
		vCount.clear();
	}

	for(int i=0; i<INT(pQUEST->m_vReward.size()); i++)
		if( pQUEST->m_vReward[i]->m_bRewardType != RT_ITEM &&
			pQUEST->m_vReward[i]->m_bRewardType != RT_MAGICITEM )
		{
			BYTE bTake = FALSE;

			switch(pQUEST->m_vReward[i]->m_bTakeMethod)
			{
			case RM_SELECT	: bTake = pQUEST->m_vReward[i]->m_bRewardType == bRewardType && pQUEST->m_vReward[i]->m_dwRewardID == dwRewardID ? TRUE : FALSE; break;
			case RM_PROB	: bTake = (rand() % 100) < pQUEST->m_vReward[i]->m_bTakeData ? TRUE : FALSE; break;
			case RM_DEFAULT	: bTake = TRUE; break;
			}

			if(bTake)
				switch(pQUEST->m_vReward[i]->m_bRewardType)
				{
				case RT_GOLD		:
					EarnMoney(pQUEST->m_vReward[i]->m_dwRewardID);
					SendCS_MONEY_ACK();
#ifdef	DEF_UDPLOG
					_AtlModule.m_pUdpSocket->LogMoney(LOGMAP_QUESTMONEYGET, this, NULL, pQUEST->m_vReward[i]->m_dwRewardID, pQUEST);
#endif	
					break;
				case RT_SKILL		:
					{
						MAPTSKILLTEMP::iterator itTEMP = pSKILLTEMP->find((WORD)pQUEST->m_vReward[i]->m_dwRewardID);
						if( itTEMP != pSKILLTEMP->end() &&
							((*itTEMP).second->m_dwClassID & BITSHIFTID(m_bClass)) &&
							!FindTSkill((*itTEMP).second->m_wID))
						{
							CTSkill * pSkill = new CTSkill();
							pSkill->m_pTSKILL = (*itTEMP).second;

							//스킬 버릴때 m_vRemainSkill에서 해당 스킬 꼭 뺄것
							m_mapTSKILL.insert(MAPTSKILL::value_type((*itTEMP).second->m_wID, pSkill));
							RemainSkill( pSkill, 0);
							SendCS_SKILLBUY_ACK(
								SKILL_SUCCESS,
								pSkill->m_pTSKILL->m_wID,
								pSkill->m_bLevel);
						}
					}
					break;
				case RT_SKILLUP		:
					{
						CTSkill* pSKILL = FindTSkill((WORD)pQUEST->m_vReward[i]->m_dwRewardID);
						if(pSKILL)
						{
							pSKILL->m_bLevel++;
							SendCS_SKILLBUY_ACK(
								SKILL_SUCCESS,
								pSKILL->m_pTSKILL->m_wID,
								pSKILL->m_bLevel);
						}
					}

					break;
				case RT_CHGCLASS	:
					break;
				case RT_EXP			:
					{
						WORD wAddExp = HaveItemProbBuff(SDT_STATUS_QUESTEXP);
						DWORD dwExp = pQUEST->m_vReward[i]->m_dwRewardID + pQUEST->m_vReward[i]->m_dwRewardID * wAddExp / 100;
						GainExp(dwExp, FALSE);
#ifdef DEF_UDPLOG
						_AtlModule.m_pUdpSocket->LogQuest( LOGMAP_QUESTEXPGET, this, pQUEST->m_dwQuestID, dwExp);
#endif
					}
					break;
				}
		}

	for(int i=0; i<INT(pQUEST->m_vTerm.size()); i++)
		if( pQUEST->m_vTerm[i]->m_bTermType == QTT_GETITEM )
		{
			BYTE bRemain = pQUEST->m_vTerm[i]->m_bCount;

			MAPTINVEN::iterator itInv;
			for(itInv=m_mapTINVEN.begin(); itInv!=m_mapTINVEN.end(); itInv++)
			{
				CTInven * pInven = (*itInv).second;

				MAPTITEM::iterator itITEM = pInven->m_mapTITEM.begin();
				while(itITEM != pInven->m_mapTITEM.end())
				{
					MAPTITEM::iterator itNEXT = itITEM;
					itNEXT++;

					if( (*itITEM).second->m_pTITEM->m_wItemID == pQUEST->m_vTerm[i]->m_dwTermID )
					{
						if( (*itITEM).second->m_bCount > bRemain )
						{
							(*itITEM).second->m_bCount -= bRemain;
							bRemain = 0;
						}
						else
						{
							bRemain -= (*itITEM).second->m_bCount;
							(*itITEM).second->m_bCount = 0;
						}

						if(!(*itITEM).second->m_bCount)
						{
#ifdef DEF_UDPLOG							
							_AtlModule.m_pUdpSocket->LogItemByNPC(LOGMAP_ITEMQUESTDEL, this, NULL, (*itITEM).second, 0, pQUEST);
#endif	//	DEF_UDPLOG

							SendCS_DELITEM_ACK(pInven->m_bInvenID, (*itITEM).second->m_bItemID);

							delete (*itITEM).second;
							pInven->m_mapTITEM.erase(itITEM);

							if( m_pMAP && pInven->m_bInvenID == INVEN_EQUIP )
							{
								VPLAYER vPLAYERS;
								vPLAYERS.clear();

								m_pMAP->GetNeighbor(
									&vPLAYERS,
									m_fPosX,
									m_fPosZ);

								while(!vPLAYERS.empty())
								{
									CTPlayer *pChar = vPLAYERS.back();

									pChar->SendCS_EQUIP_ACK(this);
									vPLAYERS.pop_back();
								}

								vPLAYERS.clear();
							}
						}
						else
						{
							SendCS_UPDATEITEM_ACK(
								(*itITEM).second,
								pInven->m_bInvenID);
						}

						if(!bRemain)
							break;
					}

					if(bRemain)
						itITEM = itNEXT;
					else
						break;
				}
			}
		}
#ifdef DEF_UDPLOG
        _AtlModule.m_pUdpSocket->LogQuest( LOGMAP_QUESTSUCCESS, this, pQUEST->m_dwQuestID);
#endif

	return QR_SUCCESS;
}

DWORD CTPlayer::CheckQuest( DWORD dwTick,
						  DWORD dwTermID,
						  BYTE bTermType,
						  int nCount)
{
	MAPQUEST::iterator it;
	DWORD dwQuestID = 0;

	for( it = m_mapQUEST.begin(); it != m_mapQUEST.end(); it++)
		if(IsRunningQuest((*it).second->m_pQUEST->m_dwQuestID))
		{
			LPQUESTTERM pTEMP = (*it).second->FindTerm( dwTermID, bTermType);

			if(pTEMP)
			{
				LPQUESTTERM pTERM = (*it).second->FindRunningTerm( dwTermID, bTermType);
				dwQuestID = (*it).second->m_pQUEST->m_dwQuestID;

				BYTE bPrevCount=0;
				if(pTERM)
				{
					bPrevCount = pTERM->m_bCount;
					pTERM->m_bCount = min( max( 0, INT(pTERM->m_bCount) + nCount), pTEMP->m_bCount);
				}

				if(!pTERM ||
					(pTERM && pTERM->m_bCount != bPrevCount))
				{
					SendCS_QUESTUPDATE_ACK(
						(*it).second->m_pQUEST->m_dwQuestID,
						pTEMP->m_dwTermID,
						pTEMP->m_bTermType,
						pTERM ? pTERM->m_bCount : GetTermCount(dwTermID, bTermType),
						(*it).second->CheckComplete( this, dwTick, pTEMP));
					
					(*it).second->m_bSave = TRUE;  
				}
			}
		}

	return dwQuestID;
}

BYTE CTPlayer::GetTermCount(DWORD dwTermID, BYTE bTermType)
{
	WORD wCount = 0;
	switch(bTermType)
	{
	case QTT_GETITEM	:
		{
			MAPTINVEN::iterator itInven;
			MAPTITEM::iterator itItem;

			for( itInven = m_mapTINVEN.begin(); itInven != m_mapTINVEN.end(); itInven++)
			{
				for(itItem=(*itInven).second->m_mapTITEM.begin(); itItem!=(*itInven).second->m_mapTITEM.end(); itItem++)
				{
					if((*itItem).second->m_wItemID == dwTermID)
						wCount += (*itItem).second->m_bCount;
				}
			}
		}
		break;
	case QTT_QUESTCOMPLETED:
		{
			CQuest * pCompQ = FindQuest(dwTermID);
			if(pCompQ)
				wCount = pCompQ->m_bCompleteCount;
		}
		break;

	default:
		break;
	}

	return (BYTE)min(wCount, 0xFF);
}

DWORD CTPlayer::GetCompleteQuestID( CQuest * pQuest, DWORD dwQuestID )
{
	if( ( pQuest->m_bTriggerCount != 0 )
		&& ( pQuest->m_bCompleteCount == pQuest->m_bTriggerCount ) )
	{
		MAPQUESTTEMP::iterator it;

		for( it = (pQuest->m_pQUEST)->m_mapCHILD.begin();
			it != (pQuest->m_pQUEST)->m_mapCHILD.end(); ++it )
		{
			if( (*it).second->m_bType == QT_MISSION )
			{
				MAPQUEST::iterator finder = m_mapQUEST.find( (*it).second->m_dwQuestID );

				if( finder == m_mapQUEST.end() )
					return pQuest->m_pQUEST->m_dwQuestID;
				else
					return GetCompleteQuestID( (*finder).second, dwQuestID );
			}
		}

		return dwQuestID;
	}
	
	return 0;
}
DWORD CTPlayer::GetPossibleQuestID( CQuest * pQuest )
{
	if( pQuest->m_bCompleteCount == pQuest->m_bTriggerCount )
	{
		MAPQUESTTEMP::iterator it;

		for(it=pQuest->m_pQUEST->m_mapCHILD.begin(); it!=pQuest->m_pQUEST->m_mapCHILD.end(); it++)
		{
			if( (*it).second->m_bType == QT_MISSION )
			{
				MAPQUEST::iterator finder = m_mapQUEST.find( (*it).second->m_dwQuestID );

				if( finder == m_mapQUEST.end() )
					return (*it).second->m_dwQuestID;				// 하위 퀘스트 수행 가능
				else
					return GetPossibleQuestID( (*finder).second );	// 하위 퀘스트 확인
			}
		}
	}

	return 0;	// 퀘스트 진행중
}
BYTE CTPlayer::IsEnoughSkillPoint(CTSkillTemp * pSkill)
{
	BYTE bPoint;
	CTSkill * pMy = FindTSkill(pSkill->m_wID);
	BYTE bNextLevel = !pMy ? 1 : pMy->m_bLevel + 1;
	bPoint = pSkill->GetNeedSkillPoint(bNextLevel);

	if(m_wSkillPoint < bPoint)
		return FALSE;

	WORD wTotalPoint=0;
	MAPTSKILL::iterator it;
	for(it=m_mapTSKILL.begin(); it!=m_mapTSKILL.end(); it++)
	{
		if((*it).second->m_pTSKILL->m_bKind == pSkill->m_bKind)
			wTotalPoint += (*it).second->GetUsedSkillPoint();
	}

	if(wTotalPoint < pSkill->GetNeedKindPoint(bNextLevel))
		return FALSE;

	return TRUE;
}

void CTPlayer::GetSkillKindPoint(WORD wPoint[])
{
	memset(wPoint, 0, 4*sizeof(WORD));

	MAPTSKILL::iterator it;
	for(it=m_mapTSKILL.begin(); it!=m_mapTSKILL.end(); it++)
	{
		CTSkill * pSkill = (*it).second;
		if(pSkill->m_pTSKILL->m_bKind == NORMAL_EQUIP)
		{
			wPoint[pSkill->m_pTSKILL->m_bKind] += pSkill->GetUsedSkillPoint();
			continue;
		}

		BYTE bKind;
		switch(m_bClass)
		{
		case TCLASS_WARRIOR:
			bKind = pSkill->m_pTSKILL->m_bKind;
			break;
		case TCLASS_RANGER:
			bKind = pSkill->m_pTSKILL->m_bKind-3;
			break;
		case TCLASS_ARCHER:
			bKind = pSkill->m_pTSKILL->m_bKind-6;
			break;
		case TCLASS_WIZARD:
			bKind = pSkill->m_pTSKILL->m_bKind-9;
			break;
		case TCLASS_PRIEST:
			bKind = pSkill->m_pTSKILL->m_bKind-12;
			break;
		case TCLASS_SORCERER:
			bKind = pSkill->m_pTSKILL->m_bKind-15;
			break;
		default:
			break;
		}

		if(bKind < 4)
			wPoint[bKind] += pSkill->GetUsedSkillPoint();
	}
}

CTRecallMon * CTPlayer::FindRecallMon(DWORD dwMonID)
{
	MAPTRECALLMON::iterator it = m_mapRecallMon.find(dwMonID);
	if(it!=m_mapRecallMon.end())
		return (*it).second;

	return NULL;
}

CTSelfObj * CTPlayer::FindSelfObj(DWORD dwMonID)
{
	MAPTSELFOBJ::iterator it = m_mapSelfMon.find(dwMonID);
	if(it!=m_mapSelfMon.end())
		return (*it).second;

	return NULL;
}

CTRecallMon * CTPlayer::FindMainRecall()
{
	MAPTRECALLMON::iterator itMon;
	for(itMon=m_mapRecallMon.begin(); itMon!=m_mapRecallMon.end(); itMon++)
	{
		CTRecallMon * pMON = (*itMon).second;
		if(pMON->m_pMON->m_bRecallType == TRECALLTYPE_MAIN)
			return pMON;
	}

	return NULL;
}

void CTPlayer::DeleteRecallMon(DWORD dwID)
{
	CTRecallMon * pMON = FindRecallMon(dwID);
	if(pMON)
	{
		if(pMON->m_pMAP)
		{
			pMON->OnDie(0,OT_NONE,0);
			pMON->m_pMAP->LeaveMAP(pMON, TRUE);
		}

		m_mapRecallMon.erase(dwID);
		delete pMON;
	}
}

void CTPlayer::DeleteSelfMon(DWORD dwID)
{
	CTSelfObj * pMON = FindSelfObj(dwID);
	if(pMON)
	{
		if(pMON->m_pMAP)
			pMON->m_pMAP->LeaveMAP(pMON, TRUE);

		m_mapSelfMon.erase(dwID);
		delete pMON;
	}
}

BYTE CTPlayer::CheckPortalCondition(LPTPORTAL pPortal, BYTE bCondition, DWORD dwConditionID)
{
	switch(bCondition)
	{
	case PCT_GUILD:
		{
			DWORD dwPortalGuild = pPortal->m_pLocal ? pPortal->m_pLocal->m_dwGuild : 0;
			return dwPortalGuild && (m_dwGuildID == dwPortalGuild) ? TRUE : FALSE;
		}
		break;
	case PCT_COUNTRY:
		{
			BYTE bDisguise = HaveDisguiseBuff();
			if(pPortal->m_bCountry != TCONTRY_N)
				return	(m_bCountry == pPortal->m_bCountry || m_bAidCountry == pPortal->m_bCountry) &&
						(!bDisguise || bDisguise == pPortal->m_bCountry + SDT_TRANS_DISGUISE_D);
			else
			{
				BYTE bPortalCountry = pPortal->m_pLocal ? pPortal->m_pLocal->m_bCountry : pPortal->m_bCountry;
				return  bPortalCountry == TCONTRY_N ||
						((m_bCountry == bPortalCountry || m_bAidCountry == bPortalCountry)
						  && (!bDisguise || bDisguise == bPortalCountry + SDT_TRANS_DISGUISE_D)) ||
						((m_bCountry != bPortalCountry && m_bAidCountry != bPortalCountry)
						  && bDisguise == bPortalCountry + SDT_TRANS_DISGUISE_D);
			}
		}
		break;
	case PCT_HAVEITEM:
		{
			MAPTINVEN::iterator itInven;
			MAPTITEM::iterator itItem;

			for( itInven = m_mapTINVEN.begin(); itInven != m_mapTINVEN.end(); itInven++)
			{
				for(itItem=(*itInven).second->m_mapTITEM.begin(); itItem!=(*itInven).second->m_mapTITEM.end(); itItem++)
				{
					if((*itItem).second->m_wItemID == WORD(dwConditionID))
						return TRUE;
				}
			}
		}
		break;
	case PCT_ATTACKPOS:
		{
			LPTLOCAL pCastle = _AtlModule.FindCastle(pPortal->m_wLocalID);
			if(pCastle &&
				m_bCamp == CAMP_ATTACK &&
				pCastle->m_wLocal == m_wCastle &&
				_AtlModule.CanEnterCastle(pCastle))
				return TRUE;
		}
		break;
	case PCT_DEFENDPOS:
		{
			LPTLOCAL pCastle = _AtlModule.FindCastle(pPortal->m_wLocalID);
			if(pCastle &&
				m_bCamp == CAMP_DEFEND &&
				pCastle->m_wLocal == m_wCastle &&
				_AtlModule.CanEnterCastle(pCastle))
				return TRUE;
		}
		break;
	case PCT_DOWNLEVEL:
		{
			if(m_bLevel >= dwConditionID)
				return TRUE;
		}
		break;
	case PCT_UPLEVEL:
		{
			if(m_bLevel <= dwConditionID)
				return TRUE;
		}
		break;
	case PCT_UPDOWNLEVEL:
		{
			if(m_bLevel >= LOWORD(dwConditionID) && m_bLevel <= HIWORD(dwConditionID))
				return TRUE;
		}
		break;
	case PCT_TOURNAMENTPLAYER:
		{
			if(dwConditionID == _AtlModule.GetTournamentMap(m_dwID, _AtlModule.m_tournament.m_bGroup))
				return TRUE;
		}
		break;
	case PCT_TOURNAMENTLOUNGE:
		{
			if(_AtlModule.CanDoTournament(TNMTSTEP_ENTER))
			{
				if(!dwConditionID)
					return TRUE;
				else if(dwConditionID == 1 && _AtlModule.FindTournamentPlayer(m_dwID, TRUE))
					return TRUE;
				else if(dwConditionID == 2 && !_AtlModule.FindTournamentPlayer(m_dwID, TRUE))
					return TRUE;
			}
		}
		break;
	case PCT_TOURNAMENTGUEST:
		{
			if(_AtlModule.IsActiveTournamentMap(WORD(dwConditionID)))
				return TRUE;
		}
		break;
	case PCT_MEETING:
		{
			if(!dwConditionID)
				return TRUE;
		}
		break;
	default:
		return TRUE;
	}

	return FALSE;
}
DWORD CTPlayer::GetExpVital()
{
	return DWORD(m_fVital*m_pTLEVEL->m_dwEXP/200);
}
FLOAT CTPlayer::GetLevelRate(BYTE bLevel)
{
	return FLOAT(1-min(max(int(m_bLevel)-bLevel,0), 10)*0.1);
}
BYTE CTPlayer::SetAftermath(DWORD dwTick, BYTE bStep)
{
	if(m_bLevel < 10)
		return FALSE;

	BYTE bPrevStep = m_aftermath.m_bStep;

	m_aftermath.m_bStep += bStep;
	if(m_aftermath.m_bStep > 100)
		m_aftermath.m_bStep = 100;

	if(bPrevStep != m_aftermath.m_bStep)
	{
		m_aftermath.m_fReuseInc=FLOAT(m_aftermath.m_bStep*0.5);
		m_aftermath.m_fStatDec=FLOAT(m_aftermath.m_bStep*0.3);
		m_aftermath.m_dwTick = dwTick + DWORD(27+m_aftermath.m_bStep*0.3)*1000;

		DWORD dwMaxHP = GetMaxHP();
		DWORD dwMaxMP = GetMaxMP();
		if(m_dwHP > dwMaxHP)
			m_dwHP = dwMaxHP;
		if(m_dwMP > dwMaxMP)
			m_dwMP = dwMaxMP;

		if(m_pMAP)
		{
			VPLAYER vPLAYERS;
			vPLAYERS.clear();

			m_pMAP->GetNeighbor(
				&vPLAYERS,
				m_fPosX,
				m_fPosZ);

			while(!vPLAYERS.empty())
			{
				CTPlayer *pChar = vPLAYERS.back();

				pChar->SendCS_AFTERMATH_ACK(
					m_dwID,
					m_aftermath.m_bStep);

				vPLAYERS.pop_back();
			}

			vPLAYERS.clear();
		}

		return TRUE;
	}

	return FALSE;
}
void CTPlayer::ResetAftermath(DWORD dwTick)
{
	if(m_aftermath.m_bStep &&
		m_aftermath.m_dwTick < dwTick)
	{
		BYTE bDec = 1 + CalcItemAbility(ABILITY_AFTERMATH);
		m_aftermath.m_bStep -= bDec;
		m_aftermath.m_fReuseInc=FLOAT(m_aftermath.m_bStep*0.5);
		m_aftermath.m_fStatDec=FLOAT(m_aftermath.m_bStep*0.3);
		m_aftermath.m_dwTick += DWORD(27+m_aftermath.m_bStep*0.3)*1000;

		DWORD dwMaxHP = GetMaxHP();
		DWORD dwMaxMP = GetMaxMP();

		if(m_pMAP)
		{
			VPLAYER vPLAYERS;
			vPLAYERS.clear();

			m_pMAP->GetNeighbor(
				&vPLAYERS,
				m_fPosX,
				m_fPosZ);

			while(!vPLAYERS.empty())
			{
				CTPlayer *pChar = vPLAYERS.back();

				pChar->SendCS_AFTERMATH_ACK(
					m_dwID,
					m_aftermath.m_bStep);

				pChar->SendCS_HPMP_ACK(
					m_dwID,
					m_bType,
					m_bLevel,
					dwMaxHP,
					m_dwHP,
					dwMaxMP,
					m_dwMP);

				vPLAYERS.pop_back();
			}

			vPLAYERS.clear();
		}

		SendCS_CHARSTATINFO_ACK(this);
	}
}
void CTPlayer::Recover(DWORD dwTick)
{
	CTObjBase::Recover(dwTick);
}
void CTPlayer::CheckRecallMon(LPTMONSTER pMON, BYTE bForce)
{
	MAPTSELFOBJ::iterator itS, itSDel;
	MAPTRECALLMON::iterator it;

	if(pMON->m_bRecallType == TRECALLTYPE_MAIN ||
		pMON->m_bRecallType == TRECALLTYPE_MINE ||
		pMON->m_bRecallType == TRECALLTYPE_AUTOAI)
	{
		for(it=m_mapRecallMon.begin(); it!=m_mapRecallMon.end();it++)
		{
			if(((*it).second->m_pMON->m_bRecallType == TRECALLTYPE_MAIN ||
				(*it).second->m_pMON->m_bRecallType == TRECALLTYPE_MINE) &&
				(*it).second->m_bMain)
			{
				_AtlModule.SendMW_RECALLMONDEL_ACK(m_dwID, m_dwKEY, (*it).second->m_dwID);
			}
		}

		for(itS=m_mapSelfMon.begin(); itS!=m_mapSelfMon.end();)
		{
			itSDel = itS;
			itS++;
			if((*itSDel).second->m_pMON->m_bRecallType == TRECALLTYPE_AUTOAI)
			{
				(*itSDel).second->OnDie(0,OT_NONE,0); 
				DeleteSelfMon((*itSDel).second->m_dwID);
			}
		}
	}
	else if(pMON->m_bRecallType == TRECALLTYPE_MAINTAIN)
	{
		MAPTSELFOBJ::reverse_iterator itRS, itRSDel;
		VDWORD vDELSELF;
		vDELSELF.clear();

		BYTE bCurrent = 0;
		BYTE bCnt = max(GetRecallCount(pMON->m_bRecallType), 1);
		for(itRS=m_mapSelfMon.rbegin(); itRS != m_mapSelfMon.rend(); itRS++)
		{
			if((*itRS).second->m_pMON->m_bRecallType == TRECALLTYPE_MAINTAIN)
			{
				bCurrent++;
				if(bCnt <= bCurrent)
				{
					(*itRS).second->OnDie(0,OT_NONE,0); 
					vDELSELF.push_back((*itRS).second->m_dwID);
				}
			}
		}
		
		for(DWORD ds=0; ds < vDELSELF.size(); ds++)
			DeleteSelfMon(vDELSELF[ds]);

		vDELSELF.clear();
	}
}

void CTPlayer::Revival(BYTE bType, CTSkillTemp * pTemp, BYTE bLevel)
{
	if(_AtlModule.IsTournamentMap(m_wMapID))
		bType = AFTERMATH_ATONCE;
	else
	{
		if(SetAftermath(_AtlModule.m_dwTick, bType))
			SendCS_CHARSTATINFO_ACK(this);
	}

	DWORD dwMaxHP = GetMaxHP();
	DWORD dwMaxMP = GetMaxMP();

	m_bStatus = OS_WAKEUP;
	m_bAction = TA_STAND;

	ChgMode(MT_NORMAL);

	switch(bType)
	{
	case AFTERMATH_ATONCE:
		m_dwHP = DWORD(dwMaxHP * 0.3);
		m_dwMP = DWORD(dwMaxMP * 0.3);
		break;
	case AFTERMATH_GHOST:
		m_dwHP = DWORD(dwMaxHP * 0.4);
		m_dwMP = DWORD(dwMaxMP * 0.4);
		break;
	case AFTERMATH_HELP:
		if(pTemp)
		{
			m_dwHP = dwMaxHP + pTemp->CalcValue(bLevel, SDT_ABILITY, MTYPE_HP, dwMaxHP);
			m_dwMP = dwMaxMP + pTemp->CalcValue(bLevel, SDT_ABILITY, MTYPE_MP, dwMaxMP);
		}
		break;
	case AFTERMATH_NONE:
		m_dwHP = dwMaxHP;
		m_dwMP = dwMaxMP;
		break;
	}

	if(!m_dwHP)
		m_dwHP = 1;

	if(m_pMAP)
	{
		VPLAYER vPLAYERS;
		vPLAYERS.clear();

		m_pMAP->GetNeighbor(
			&vPLAYERS,
			m_fPosX,
			m_fPosZ);

		while(!vPLAYERS.empty())
		{
			CTPlayer *pChar = vPLAYERS.back();

			pChar->SendCS_REVIVAL_ACK(
				m_dwID,
				m_fPosX,
				m_fPosY,
				m_fPosZ);

			pChar->SendCS_HPMP_ACK(
				m_dwID,
				OT_PC,
				m_bLevel,
				dwMaxHP,
				m_dwHP,
				dwMaxMP,
				m_dwMP);

			vPLAYERS.pop_back();
		}

		vPLAYERS.clear();

		ForceMaintain(TREVIVAL_SKILL, m_dwID, m_bType, m_dwID, m_bType, 0);

//		HangPremiumItem();
	}
}

BYTE CTPlayer::ChangeSwitch( DWORD dwSwitchID)
{
	if(!m_pMAP)
		return FALSE;

	LPTSWITCH pSwitch = m_pMAP->FindSwitch(dwSwitchID);
	if(!pSwitch)
		return FALSE;

	if(pSwitch->m_bLockOnClose && pSwitch->m_bOpened == FALSE)
		return FALSE;

	if(pSwitch->m_bLockOnOpen && pSwitch->m_bOpened == TRUE)
		return FALSE;

	if(_AtlModule.m_dwTick - pSwitch->m_dwStartTime < pSwitch->m_dwDuration)
		return FALSE;

	if(pSwitch->m_dwDuration)
	{
		_AtlModule.SendSM_SWITCHSTART_REQ(
			pSwitch->m_pMAP->m_bChannel,
			pSwitch->m_pMAP->m_wPartyID,
			pSwitch->m_pMAP->m_wMapID,
			dwSwitchID,
			pSwitch->m_dwDuration);
	}

	pSwitch->m_dwStartTime = _AtlModule.m_dwTick;
	pSwitch->m_bOpened = !pSwitch->m_bOpened;

	_AtlModule.CheckQuest(
		this,
		0,
		m_fPosX,
		m_fPosY,
		m_fPosZ,
		WORD(dwSwitchID),
		0,
		TT_RUNSWITCH,
		1);

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	m_pMAP->GetNeighbor(
		&vPLAYERS,
		pSwitch->m_wPosX,
		pSwitch->m_wPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_SWITCHCHANGE_ACK(
			SWITCH_SUCCESS,
			dwSwitchID,
			pSwitch->m_bOpened);
		vPLAYERS.pop_back();
	}

	vPLAYERS.clear();

	for(DWORD i=0; i<pSwitch->m_vGate.size(); i++)
	{
		LPTGATE pGate = pSwitch->m_vGate[i];
		BYTE bGateChange = TRUE;
		if(pGate->m_bType == GT_MULTISWITCH)
		{
			for(DWORD k=0; k<pGate->m_vSwitch.size(); k++)
			{
				if(pGate->m_vSwitch[k]->m_bOpened != pSwitch->m_bOpened)
				{
					bGateChange = FALSE;
					break;
				}
			}
		}

		if(!bGateChange)
			continue;

		pGate->m_bOpened = !pGate->m_bOpened;
		_AtlModule.CheckQuest(
			this,
			0,
			m_fPosX,
			m_fPosY,
			m_fPosZ,
			WORD(pGate->m_dwGateID),
			0,
			TT_RUNGATE,
			1);

		m_pMAP->GetNeighbor(
			&vPLAYERS,
			pGate->m_wPosX,
			pGate->m_wPosZ);

		while(!vPLAYERS.empty())
		{
			CTPlayer *pChar = vPLAYERS.back();

			pChar->SendCS_GATECHANGE_ACK(pGate->m_dwGateID, pGate->m_bOpened);
			vPLAYERS.pop_back();
		}

		vPLAYERS.clear();
	}

	return TRUE;
}

void CTPlayer::SendQuestTimer(DWORD dwTick)
{
	MAPQUEST::iterator it;
	for(it=m_mapQUEST.begin(); it!=m_mapQUEST.end(); it++)
	{
		CQuest * pQuest = (*it).second;
		if(pQuest->m_dwTick)
			SendCS_QUESTSTARTTIMER_ACK(
				pQuest->m_pQUEST->m_dwQuestID,
				pQuest->m_dwTick);
	}
}

void CTPlayer::CheckEquipSkill()
{
	WORD wPosture = 0;
	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
		if(m_vMaintainSkill[i]->m_bAttackType == OT_PC &&
			m_vMaintainSkill[i]->m_dwAttackID == m_dwID &&
			!IsEquipSkillItem(m_vMaintainSkill[i]))
		{
			if(m_vMaintainSkill[i]->m_pTSKILL->IsPosture())
				wPosture = m_vMaintainSkill[i]->m_pTSKILL->m_wID;

			EraseMaintainSkill(m_vMaintainSkill[i], i);
		}
		else
			i++;
	}

	if(wPosture)
	{
		CTSkill * pSkill;
		for(DWORD i=0; i<m_vMaintainSkill.size();)
		{
			pSkill = m_vMaintainSkill[i];
			if(pSkill->m_bAttackType == OT_PC &&
				pSkill->m_dwAttackID == m_dwID &&
				pSkill->m_pTSKILL->m_wPosture && 
				pSkill->m_pTSKILL->m_wPosture == wPosture)
			{
				EraseMaintainSkill(m_vMaintainSkill[i], i);
			}
			else
				i++;
		}
	}
}

void CTPlayer::MoveGhost(FLOAT fPosX, FLOAT fPosZ)
{
	if(!m_pMAP)
		return;

	m_fGhostX = fPosX;
	m_fGhostZ = fPosZ;

	if(!m_bCanHost &&
		GetDistance(m_fPosX, m_fPosZ, m_fGhostX, m_fGhostZ) < CELL_SIZE/2)
	{
		m_bCanHost = TRUE;

		VTMONSTER vMONS;
		vMONS.clear();

		m_pMAP->GetNeighbor(&vMONS, m_fPosX, m_fPosZ);
		while(!vMONS.empty())
		{
			CTMonster * pMON = vMONS.back();
			if(GetDistance(m_fPosX, m_fPosZ, pMON->m_fPosX, pMON->m_fPosZ) < CELL_SIZE/2)
				pMON->OnEvent(AT_ENTER, 0, 0, 0, 0);
			vMONS.pop_back();
		}

		vMONS.clear();
	}
	else if(m_bCanHost &&
		GetDistance(m_fPosX, m_fPosZ, m_fGhostX, m_fGhostZ) >= CELL_SIZE/2)
	{
		m_bCanHost = FALSE;

		VTMONSTER vMONS;
		vMONS.clear();

		m_pMAP->GetNeighbor(&vMONS, m_fPosX, m_fPosZ);
		while(!vMONS.empty())
		{
			CTMonster * pMON = vMONS.back();
			if(pMON->m_dwHostID == m_dwID)
				pMON->OnEvent( AT_LEAVE, 0, m_dwID, m_dwID, OT_PC);

			vMONS.pop_back();
		}

		vMONS.clear();
	}
}

BYTE CTPlayer::CanHost(FLOAT fPosX, FLOAT fPosZ)
{
	if(m_bIsGhost)
	{
		if(GetDistance(m_fPosX, m_fPosZ, m_fGhostX, m_fGhostZ) < CELL_SIZE/2 &&
			GetDistance(m_fPosX, m_fPosZ, fPosX, fPosZ) < CELL_SIZE/2)
			return TRUE;
		else
			return FALSE;
	}
	else
		return m_bCanHost;
}

LPTPOST CTPlayer::MakePost(DWORD dwPostID,
						   DWORD dwSendID,
						   CString strSender,
						   CString strTitle,
						   CString strMessage,
						   BYTE bType,
						   BYTE bRead,
						   __int64 timeRecv,
						   DWORD dwGold,
						   DWORD dwSilver,
						   DWORD dwCooper)
{
	if(!dwPostID)
		return NULL;

	LPTPOST pPost = new TPOST();

	pPost->m_bSave = POST_SAVE_NO;
	pPost->m_dwPostID = dwPostID;
	pPost->m_dwSendID = dwSendID;
	pPost->m_bRead = bRead;
	pPost->m_strSender = strSender;
	pPost->m_strTitle = strTitle;
	pPost->m_strMessage = strMessage;
	pPost->m_bType = bType;
	pPost->m_timeRecv = timeRecv;
	pPost->m_dwGold = dwGold;
	pPost->m_dwSilver = dwSilver;
	pPost->m_dwCooper = dwCooper;

	return pPost;
}

BYTE CTPlayer::HavePostBill()
{
/*
	MAPTPOST::iterator it;
	for(it=m_mapTPost.begin(); it!=m_mapTPost.end(); it++)
	{
		if((*it).second->m_bType == POST_BILLS &&
			!(*it).second->m_bRead)
			return TRUE;
	}
*/
	return FALSE;
}

BYTE CTPlayer::GetRecallCount(BYTE bRecallType)
{
	if(bRecallType == TRECALLTYPE_MAINTAIN)
		for(DWORD i=0; i<m_vRemainSkill.size(); i++)
			for(DWORD j=0; j<m_vRemainSkill[i]->m_pTSKILL->m_vData.size(); j++)
				if(m_vRemainSkill[i]->m_pTSKILL->m_vData[j]->m_bType == SDT_ABILITY &&
					m_vRemainSkill[i]->m_pTSKILL->m_vData[j]->m_bExec == MTYPE_RMC)
					return BYTE(m_vRemainSkill[i]->GetValue(m_vRemainSkill[i]->m_pTSKILL->m_vData[j]));
	return 1;
}

DWORD CTPlayer::GetRecallLifeTime()
{
	for(DWORD i=0; i<m_vRemainSkill.size(); i++)
		for(DWORD j=0; j<m_vRemainSkill[i]->m_pTSKILL->m_vData.size(); j++)
			if(m_vRemainSkill[i]->m_pTSKILL->m_vData[j]->m_bType == SDT_CURE &&
				m_vRemainSkill[i]->m_pTSKILL->m_vData[j]->m_bExec == SCT_INCLIFTTIME)
				return m_vRemainSkill[i]->GetValue(m_vRemainSkill[i]->m_pTSKILL->m_vData[j]);

	return 0;
}

void CTPlayer::StoreClose()
{
	if(m_bStore)
	{
		CTObjBase * pOBJ = this;
		for(DWORD i=0; i<pOBJ->m_vMaintainSkill.size(); i++)
		{
			if(pOBJ->m_vMaintainSkill[i]->m_pTSKILL->m_wID == TSTORE_SKILL)
			{
				pOBJ->EraseMaintainSkill(pOBJ->m_vMaintainSkill[i], i);
				break;
			}
		}

		m_bStore = 0;
		m_strStoreName = NAME_NULL;
		MAPTSTOREITEM::iterator itSTORE;
		for(itSTORE=m_mapStoreItem.begin(); itSTORE!=m_mapStoreItem.end(); itSTORE++)
			delete (*itSTORE).second;
		m_mapStoreItem.clear();

		if(m_pMAP)
		{
			VPLAYER vPlayer;
			vPlayer.clear();

			m_pMAP->GetNeighbor(&vPlayer, m_fPosX, m_fPosZ);
			for(DWORD i=0; i<vPlayer.size(); i++)
				vPlayer[i]->SendCS_STORECLOSE_ACK(STORE_SUCCESS, m_dwID);

			vPlayer.clear();
		}
	}
}

CTRecallMon * CTPlayer::FindRecallPet()
{
	MAPTRECALLMON::iterator itRecall;
	for(itRecall=m_mapRecallMon.begin(); itRecall!=m_mapRecallMon.end(); itRecall++)
	{
		if((*itRecall).second->m_pMON->m_bRecallType == TRECALLTYPE_PET)
			return (*itRecall).second;
	}

	return NULL;
}

void CTPlayer::CheckTimeRecallMon(DWORD dwTick)
{
	MAPTRECALLMON::iterator it;
	for(it=m_mapRecallMon.begin(); it!=m_mapRecallMon.end();it++)
	{
		CTRecallMon * pRM = (*it).second;
		if(pRM->m_bMain)
		{
			pRM->Recover(dwTick);
			pRM->CheckMaintainSkill(dwTick);
		}

		if((pRM->m_dwDurationTick && !pRM->GetLifeTick(dwTick)) ||
			!pRM->FindHost(pRM->m_dwHostID))
		{
			if(pRM->m_bMain)
				_AtlModule.SendMW_RECALLMONDEL_ACK(m_dwID, m_dwKEY, pRM->m_dwID);
		}
	}

	MAPTSELFOBJ::iterator itS, itSDel;
	for(itS=m_mapSelfMon.begin(); itS!=m_mapSelfMon.end();)
	{
		itSDel = itS;
		itS++;
		CTSelfObj * pSM = (*itSDel).second;
		pSM->Recover(dwTick);
		pSM->CheckMaintainSkill(dwTick);

		if((pSM->m_dwDurationTick && !pSM->GetLifeTick(dwTick)) ||
			!pSM->FindHost(pSM->m_dwHostID))
		{
			pSM->OnDie(0,OT_NONE,0); 
			DeleteSelfMon(pSM->m_dwID);
		}
	}
}

void CTPlayer::PetRiding(DWORD dwRiding)
{
	if(m_bMain &&
		dwRiding != m_dwRiding)
	{
		DWORD dwMonID = dwRiding ? dwRiding : m_dwRiding;
		m_dwRiding = dwRiding;

		_AtlModule.SendMW_PETRIDING_ACK(
			m_dwID,
			m_dwKEY,
			dwRiding);

		if(m_pMAP)
		{
			VPLAYER vPlayer;
			vPlayer.clear();
			m_pMAP->GetNeighbor(&vPlayer, m_fPosX, m_fPosZ);
			for(DWORD i=0; i<vPlayer.size(); i++)
				vPlayer[i]->SendCS_PETRIDING_ACK(
					PET_SUCCESS,
					m_dwID,
					dwMonID,
					dwRiding ? PETACTION_RIDING : PETACTION_DISMOUNT);

			vPlayer.clear();
		}
	}
}

void CTPlayer::ClearDuel()
{
	m_dwDuelID = 0;
	m_bDuelType = 0;
	m_dwDuelTarget = 0;
}
BYTE CTPlayer::CanDuel(CTPlayer * pAttacker)
{
	if(pAttacker)
	{ 
		if(m_dwDuelID &&
			m_dwID != pAttacker->m_dwID)
		{
			if(GetWarCountry() == pAttacker->GetWarCountry() ||
				m_bCountry == TCONTRY_PEACE ||
				pAttacker->m_bCountry == TCONTRY_PEACE)
			{
				if(m_dwDuelTarget != pAttacker->m_dwID)
					return FALSE;

				if(m_dwDuelTarget == pAttacker->m_dwID &&
					(m_bDuelType == DUEL_END || m_bDuelType == DUEL_STANDBY) )
					return FALSE;
			}
			else
				_AtlModule.SendSM_DUELEND_REQ(m_dwDuelID, 0);
		}
	}

	return TRUE;
}
BYTE CTPlayer::DuelLose(DWORD dwAttackerID)
{
	if(m_dwDuelTarget != dwAttackerID)
	{
		_AtlModule.SendSM_DUELEND_REQ(m_dwID, 0);
		return FALSE;
	}

	EraseDieSkillBuff();
	DeleteNegativeMaintainSkill();
	DeleteAllRecallMon();
	DeleteAllSelfMon();

	m_dwHP = 1;
	m_bDuelType = DUEL_END;

	SendCS_DUELEND_ACK(m_dwID);

	if(m_pMAP)
	{
		VPLAYER vPlayer;
		vPlayer.clear();
		m_pMAP->GetNeighbor(&vPlayer, m_fPosX, m_fPosZ);
		for(DWORD i=0; i<vPlayer.size(); i++)
		{
			vPlayer[i]->SendCS_HPMP_ACK(
				m_dwID,
				m_bType,
				m_bLevel,
				GetMaxHP(),
				m_dwHP,
				GetMaxMP(),
				m_dwMP);
		}

		vPlayer.clear();
	}

	_AtlModule.SendSM_DUELEND_REQ(m_dwDuelID, m_dwID);
	return TRUE;
}
void CTPlayer::DeleteAllRecallMon()
{
	MAPTRECALLMON::iterator it;
	for(it=m_mapRecallMon.begin(); it!=m_mapRecallMon.end();it++)
		_AtlModule.SendMW_RECALLMONDEL_ACK(m_dwID, m_dwKEY, (*it).second->m_dwID);
}
void CTPlayer::DeleteAllSelfMon()
{
	MAPTSELFOBJ::iterator itS, itSDel;
	for(itS=m_mapSelfMon.begin(); itS!=m_mapSelfMon.end();)
	{
		itSDel = itS;
		itS++;
		
		(*itSDel).second->OnDie(0,OT_NONE,0); 
		DeleteSelfMon((*itSDel).second->m_dwID);
	}
}
void CTPlayer::DeleteAllMaintainSkill()
{
	while(m_vMaintainSkill.size())
		EraseMaintainSkill(m_vMaintainSkill[0], 0);

	HangPremiumItem();
}

BYTE CTPlayer::IsExpBenefit(WORD &wBonus)
{
	if(!m_bMain)
		return FALSE;

	if(m_bInPcBang && m_dwPcBangTime < PCBANG_TIME)
	{
		if(_AtlModule.GetNation() == NATION_GERMAN)
			wBonus = 25;
		else
			wBonus = 20;

		return IK_GOLDPREMIUM;
	}
	else if(m_diExp.m_bType)
	{
		wBonus = m_diExp.m_pTITEM->m_wUseValue;
		return IK_EXPBONUS;
	}

	return FALSE;
}

void CTPlayer::ClearGuildItem()
{
	if(m_guildItem)
		delete m_guildItem;
	m_guildItem = NULL;
}

void CTPlayer::CopyGuildItem(CTItem * pItem, BYTE bCount, BYTE bGenID)
{
	m_guildItem = new CTItem();
	m_guildItem->Copy(pItem, bGenID);
	m_guildItem->m_bCount = bCount;
}

void CTPlayer::BackGuildItem()
{
	VTITEM vItem;
	vItem.clear();
	vItem.push_back(m_guildItem);

	if(CanPush(&vItem, 0))
		PushTItem(&vItem);
	else
		ClearGuildItem();

	m_guildItem = NULL;
}

BYTE CTPlayer::CheckGuildDuty(BYTE bDuty)
{
	if(m_bGuildDuty >= bDuty)
		return TRUE;

	return FALSE;
}

WORD CTPlayer::MagicBackSkill(BYTE bMagic)
{
	MAPTITEM::iterator it;
	CTInven * pInven = FindTInven(INVEN_EQUIP);
	MAPTMAGIC::iterator itTMAGIC;
	WORD wSkill = 0;
/*	if(pInven)
	{
		CTItem * pItem;
		if(bMagic == MTYPE_DESKILL)
			pItem = pInven->FindTItem(ES_BODY);
		else
			pItem = pInven->FindTItem(ES_PRMWEAPON);

		if(!pItem)
			return 0;

		for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
		{
			if((*itTMAGIC).second->m_pMagic->m_bMagic == bMagic)
			{
				MAPTSKILLTEMP::iterator itSk = _AtlModule.m_mapTSKILL.find((*itTMAGIC).second->m_wValue);
				if(itSk != _AtlModule.m_mapTSKILL.end())
				{
					wSkill = (*itSk).second->m_wID;
					BYTE bProb = rand() % 100;
					for(DWORD i=0; i<(*itSk).second->m_vData.size(); i++)
					{
						if((*itSk).second->m_vData[i]->m_bProb < bProb)
							wSkill = 0;
					}
				}
			}
		}
	}
*/
	return wSkill;
}
CTSkill * CTPlayer::FindTChildSkill(WORD wParentID)
{
	MAPTSKILL::iterator it;
	for(it=m_mapTSKILL.begin(); it!=m_mapTSKILL.end(); it++)
	{
		if((*it).second->m_pTSKILL->m_wParentSkillID == wParentID)
			return (*it).second;
	}

	return NULL;
}
void CTPlayer::InitializeSkill(CTSkill * pSkill)
{
	WORD wSkillPoint = 0;

	if(pSkill)
	{
		wSkillPoint = pSkill->m_pTSKILL->GetNeedSkillPoint(pSkill->m_bLevel);
		pSkill->m_bLevel--;

		if(!pSkill->m_bLevel)
		{
			for(DWORD i=0; i<m_vRemainSkill.size(); i++)
			{
				if(m_vRemainSkill[i]->m_pTSKILL->m_wID == pSkill->m_pTSKILL->m_wID)
				{
					m_vRemainSkill.erase(m_vRemainSkill.begin()+i);
					break;
				}
			}

			EraseHotkey(pSkill->m_pTSKILL->m_wID);

			m_mapTSKILL.erase(pSkill->m_pTSKILL->m_wID);
			delete pSkill;
		}
	}
	else
	{
		DWORD dwPayback = 0;
		VWORD vSkillID;
		vSkillID.clear();

		MAPTSKILL::iterator it;
		for(it=m_mapTSKILL.begin(); it!=m_mapTSKILL.end(); it++)
		{
			pSkill = (*it).second;

			if(pSkill->m_pTSKILL->m_bKind == NORMAL_EQUIP)
				continue;

			MAPTSKILLPOINT::iterator itSP = pSkill->m_pTSKILL->m_mapTSkillPoint.find(pSkill->m_bLevel);
			if(itSP != pSkill->m_pTSKILL->m_mapTSkillPoint.end())
				dwPayback += (*itSP).second->m_dwPayback;

			while(pSkill->m_bLevel)
			{
				// 기본적으로 주는 스킬
				if((*it).second->m_pTSKILL->m_bStartLevel == 0 &&
					(*it).second->m_bLevel == 1)
					break;

				wSkillPoint += pSkill->m_pTSKILL->GetNeedSkillPoint(pSkill->m_bLevel);
				pSkill->m_bLevel--;
			}

			if(!pSkill->m_bLevel)
				vSkillID.push_back(pSkill->m_pTSKILL->m_wID);
		}

		for(BYTE i=0; i<vSkillID.size(); i++)
		{
			CTSkill * pSkillTemp = NULL;

			MAPTSKILL::iterator find = m_mapTSKILL.find(vSkillID[i]);
			if(find!=m_mapTSKILL.end())
			{
				pSkillTemp = (*find).second;

				for(DWORD j=0; j<m_vRemainSkill.size(); j++)
				{
					if(m_vRemainSkill[j]->m_pTSKILL->m_wID == vSkillID[i])
					{
						m_vRemainSkill.erase(m_vRemainSkill.begin()+j);
						break;
					}
				}

				EraseHotkey(vSkillID[i]);

				m_mapTSKILL.erase(vSkillID[i]);
				delete pSkillTemp;
			}
		}

		if(dwPayback)
		{
			EarnMoney(dwPayback);
			SendCS_MONEY_ACK();
		}

		vSkillID.clear();
	}

	m_wSkillPoint += wSkillPoint;
}

void CTPlayer::InitSkillPossible(BYTE bLevel, LPVWORD vSkill)
{
	MAPTSKILL::iterator it;
	for(it=m_mapTSKILL.begin(); it!=m_mapTSKILL.end(); it++)
	{
		CTSkill * pSkill = (*it).second;
		if(pSkill->m_pTSKILL->m_bKind == NORMAL_EQUIP)
			continue;
		if(FindTChildSkill(pSkill->m_pTSKILL->m_wID))
			continue;
		if(pSkill->m_pTSKILL->m_bStartLevel == 0 &&
			pSkill->m_bLevel == 1)
			continue;
		if((pSkill->m_pTSKILL->m_bStartLevel + pSkill->m_bLevel) > bLevel)
			continue;

		vSkill->push_back(pSkill->m_pTSKILL->m_wID);
	}
}

void CTPlayer::EraseHotkey(WORD wSkillID)
{
	MAPTHOTKEYINVEN::iterator itHotkey;
	for(itHotkey=m_mapHotkeyInven.begin(); itHotkey!=m_mapHotkeyInven.end(); itHotkey++)
	{
		for(BYTE i=0; i<MAX_HOTKEY_POS; i++)
		{
			if((*itHotkey).second->m_hotkey[i].m_wID == wSkillID)
			{
				(*itHotkey).second->m_hotkey[i].m_bType = HOTKEY_NONE;
				(*itHotkey).second->m_hotkey[i].m_wID = 0;
				(*itHotkey).second->m_bSave |= ITEM_SAVE_UPDATE;
				break;
			}
		}
	}
}

void CTPlayer::ClearSoulmate()
{
	SetSoulmate(0, NAME_NULL, 0);
}

void CTPlayer::SetSoulmate(DWORD dwSoulmate, CString strSoulmate, DWORD dwSilence)
{
	m_dwSoulmate = dwSoulmate;
	m_strSoulmate = strSoulmate;
	m_dwSoulSilence = dwSilence;
}

BYTE CTPlayer::CheckSoulSilence(DWORD dwTime)
{
	if(!m_dwSoulSilence)
		return TRUE;

	DWORD dwSilence = dwTime - m_dwSoulSilence;
	if(dwSilence < SOULMATE_SILENCE_DURATION)
		return FALSE;

	return TRUE;
}

void CTPlayer::CheckSoulEXP(VPLAYER &vParty, DWORD dwEXP)
{
	if(!m_dwSoulmate || m_dwSoulSilence)
		return;

	for(BYTE i=0; i<vParty.size(); i++)
	{
		if(vParty[i]->m_bMain &&
			vParty[i]->m_dwID == m_dwSoulmate &&
			vParty[i]->m_bStatus != OS_DEAD)
		{
			DWORD dwGain = DWORD(dwEXP * 0.1);
			vParty[i]->GainExp(dwGain, FALSE);
			vParty[i]->SendCS_EXP_ACK();
			return;
		}
	}
}

void CTPlayer::CheckSoulHP()
{
	if(m_dwDuelID)
		return;

	if(_AtlModule.IsTournamentMap(m_wMapID))
		return;

	if(!CheckSoulTakeHP())
		return;

	if(m_pMAP)
	{
		VPLAYER vPLAYERS;
		vPLAYERS.clear();

		m_pMAP->GetNeighbor(
			&vPLAYERS,
			m_fPosX,
			m_fPosZ);

		while(!vPLAYERS.empty())
		{
			CTPlayer *pChar = vPLAYERS.back();

			// 피전송
			if(pChar->m_bMain &&
				pChar->m_dwSoulmate == m_dwID &&
				!pChar->m_dwSoulSilence &&
				pChar->GetPartyID() &&
				pChar->GetPartyID() == GetPartyID())
				pChar->TransSoulHP(this);

			if(!CheckSoulTakeHP())
				break;

			vPLAYERS.pop_back();
		}

		vPLAYERS.clear();
	}
}

BYTE CTPlayer::CheckSoulTakeHP()
{
	if(m_bStatus == OS_DEAD)
		return FALSE;

	DWORD dwHP = DWORD(GetMaxHP() * 0.15);
	if(dwHP <= m_dwHP)
		return FALSE;

	return TRUE;
}

void CTPlayer::TransSoulHP(CTPlayer * pPlayer)
{
	if(!m_bMain ||
		m_bStatus == OS_DEAD ||
		!m_pMAP)
		return;
	
	DWORD dwHP = DWORD(GetMaxHP() * 0.65);
	if(m_dwHP <= dwHP)
		return;

	DWORD dwTransHP = m_dwHP - dwHP;
	m_dwHP -= dwTransHP;

	DWORD dwMaxHP = pPlayer->GetMaxHP();
	if(dwMaxHP < (pPlayer->m_dwHP+dwTransHP))
		pPlayer->m_dwHP = dwMaxHP;
	else
		pPlayer->m_dwHP += dwTransHP;

	VPLAYER vPlayer;
	vPlayer.clear();
	m_pMAP->GetNeighbor(&vPlayer, m_fPosX, m_fPosZ);
	for(DWORD i=0; i<vPlayer.size(); i++)
	{
		CTPlayer * pChar = vPlayer[i];
		pChar->SendCS_HPMP_ACK(
			m_dwID,
			m_bType,
			m_bLevel,
			GetMaxHP(),
			m_dwHP,
			GetMaxMP(),
			m_dwMP);
	}
}

void CTPlayer::RemoveGodBall(LPTGODBALL pBall)
{
	m_wGodBall = 0;

	if(pBall)
	{
		pBall->m_strOwner = NAME_NULL;
		pBall->m_dlTime = 0;
	}

	if(!m_pMAP)
		return;

	VPLAYER vPlayer;
	vPlayer.clear();
	m_pMAP->GetMapPlayers(&vPlayer);
	while(!vPlayer.empty())
	{
		CTPlayer * pChar = vPlayer.back();
		pChar->SendCS_REMOVEGODBALL_ACK(m_dwID);
		vPlayer.pop_back();
	}
}

WORD CTPlayer::DurationDec(BYTE bSlot, BYTE bDel)
{
	CTItem * pItem1 = NULL;
	CTItem * pItem2 = NULL;
	BYTE bDef = 0;
	WORD wBackSkill = 0;

	CTInven * pInven = FindTInven(INVEN_EQUIP);
	switch(bSlot)
	{
	case 1://원거리무기
		pItem1 = pInven->FindTItem(ES_LONGWEAPON);
		break;
	case 2://방패 제외 주.보조무기
		pItem1 = pInven->FindTItem(ES_PRMWEAPON);
		if(pItem1 && pItem1->m_pTITEM->m_bType == IT_SHIELD)
			pItem1 = NULL;
		pItem2 = pInven->FindTItem(ES_SNDWEAPON);
		if(pItem2 && pItem2->m_pTITEM->m_bType == IT_SHIELD)
			pItem2 = NULL;
		break;
	case 3://방어구
		bDef = rand() % 6;
		switch(bDef)
		{
		case 0:
			pItem1 = pInven->FindTItem(ES_HEAD);
			break;
		case 1:
			pItem1 = pInven->FindTItem(ES_BODY);
			break;
		case 2:
			pItem1 = pInven->FindTItem(ES_PANTS);
			break;
		case 3:
			pItem1 = pInven->FindTItem(ES_HAND);
			break;
		case 4:
			pItem1 = pInven->FindTItem(ES_FOOT);
			break;
		case 5:
			pItem1 = pInven->FindTItem(ES_BACK);
			break;
		}
		break;
	case 4://방패
		pItem1 = pInven->FindTItem(ES_SNDWEAPON);
		if(pItem1 && pItem1->m_pTITEM->m_bType != IT_SHIELD)
			pItem1 = NULL;
		break;
	}

	if(pItem1)
	{
		wBackSkill = pItem1->GetAutoSkill();
		if(!pItem1->DurationDec(bDel))
		{
			SendCS_DURATIONEND_ACK(INVEN_EQUIP, pItem1->m_bItemID, !pItem1->m_pTITEM->m_bCanRepair);
			if(!pItem1->m_pTITEM->m_bCanRepair)
			{
				SendCS_DELITEM_ACK(INVEN_EQUIP,	pItem1->m_bItemID);
				pInven->m_mapTITEM.erase(pItem1->m_bItemID);
#ifdef DEF_UDPLOG
				_AtlModule.m_pUdpSocket->LogItemUpgrade(LOGMAP_ITEMDURATIONEND, this, pItem1);
#endif
				delete pItem1;
				ChangeEquipItem();
			}
		}
		else
			SendCS_DURATIONDEC_ACK(INVEN_EQUIP, pItem1->m_bItemID, pItem1->m_dwDuraCur);
	}

	if(pItem2)
	{
		if(!wBackSkill)
			wBackSkill = pItem2->GetAutoSkill();

		if(!pItem2->DurationDec(bDel))
		{
			SendCS_DURATIONEND_ACK(INVEN_EQUIP, pItem2->m_bItemID, !pItem2->m_pTITEM->m_bCanRepair);
			if(!pItem2->m_pTITEM->m_bCanRepair)
			{
				SendCS_DELITEM_ACK(INVEN_EQUIP,	pItem2->m_bItemID);
				pInven->m_mapTITEM.erase(pItem2->m_bItemID);
#ifdef DEF_UDPLOG
				_AtlModule.m_pUdpSocket->LogItemUpgrade(LOGMAP_ITEMDURATIONEND, this, pItem2);
#endif
				delete pItem2;
				ChangeEquipItem();
			}
		}
		else
			SendCS_DURATIONDEC_ACK(INVEN_EQUIP, pItem2->m_bItemID, pItem2->m_dwDuraCur);
	}

	return wBackSkill;
}

void CTPlayer::ChangeEquipItem()
{
	if(!m_pMAP)
		return;

	m_bEquipSpecial = IsEquipSpecial();

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	m_pMAP->GetNeighbor(
		&vPLAYERS,
		m_fPosX,
		m_fPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_EQUIP_ACK(this);
		vPLAYERS.pop_back();
	}
	vPLAYERS.clear();

	SendCS_MOVEITEM_ACK(MI_SUCCESS);
	SendCS_CHARSTATINFO_ACK(this);

	DWORD dwMaxHP = GetMaxHP();
	DWORD dwMaxMP = GetMaxMP();
	if(dwMaxHP < m_dwHP)
		m_dwHP = dwMaxHP;
	if(dwMaxMP < m_dwMP)
		m_dwMP = dwMaxMP;

	SendCS_HPMP_ACK(
		m_dwID,
		m_bType,
		m_bLevel,
		dwMaxHP,
		m_dwHP,
		dwMaxMP,
		m_dwMP);

	CheckEquipSkill();
}

void CTPlayer::ResetCoolTime(WORD wExceptSkill)
{
	MAPTSKILL::iterator it;
	for(it=m_mapTSKILL.begin(); it!=m_mapTSKILL.end(); it++)
	{
		if((*it).second->m_pTSKILL->m_wID != wExceptSkill)
		{
			(*it).second->m_dwUseTick = 0;
			(*it).second->m_dwDelayTick = 0;
		}
	}

	m_mapItemCoolTime.clear();

	SendCS_RESETCOOLTIME_ACK(wExceptSkill);
}

void CTPlayer::CheckItemEndTime(__time64_t dCurrentTime)
{
	if(m_bStore)
		return;

	BYTE bDel = FALSE;
	MAPTINVEN::iterator itInv;
	for(itInv=m_mapTINVEN.begin(); itInv!=m_mapTINVEN.end();)
	{
		CTInven * pInven = (*itInv).second;
		MAPTITEM::iterator itIT, itITDel;
		for(itIT = pInven->m_mapTITEM.begin(); itIT != pInven->m_mapTITEM.end();)
		{
			CTItem * pItem = (*itIT).second;
			if(pItem->m_dEndTime && 
				pItem->m_dAlarmTime &&
				pItem->m_dAlarmTime <= _AtlModule.m_timeCurrent)
			{
				DWORD dwSecond = DWORD(pItem->m_dEndTime - pItem->m_dAlarmTime);
				SendCS_SYSTEMMSG_ACK(
					SM_ITEM_EXPIRE,
					0,
					dwSecond,
					NAME_NULL,
					NAME_NULL,
					(*itInv).second->m_bInvenID,
					pItem->m_bItemID);

				_AtlModule.SetAlarmTime(pItem);
			}

			if(pItem->m_dEndTime && pItem->m_dEndTime <= dCurrentTime)
			{
				SendCS_DELITEM_ACK(pInven->m_bInvenID, pItem->m_bItemID);
				 itIT = pInven->m_mapTITEM.erase(itIT);
				delete pItem;

				if(pInven->m_bInvenID == INVEN_EQUIP)
					ChangeEquipItem();

				bDel = TRUE;
			}
			else
				itIT++;
		}

		if(pInven->m_dEndTime && pInven->m_dEndTime <= dCurrentTime)
		{
			_AtlModule.SendDM_POSTINVENITEM_REQ(
				m_dwID,
				m_dwKEY,
				m_strNAME,
				pInven);

			SendCS_INVENDEL_ACK(
				INVEN_SUCCESS,
				pInven->m_bInvenID);

			itInv = m_mapTINVEN.erase(itInv);
			delete pInven;

			_AtlModule.SendDM_SAVEITEM_REQ(this);
		}
		else
			 itInv++;
	}

	if(bDel)
		SendCS_MOVEITEM_ACK(MI_SUCCESS);
}

BYTE CTPlayer::ChangeCharBase(BYTE bType, CString strName, BYTE bValue)
{
	if(!m_pMAP)
		return FALSE;

	switch(bType)
	{
	case IK_FACE:
		bValue = rand() % 4;
		if(m_bFace == bValue)
			bValue = (m_bFace + 1) % 4;
		break;
	case IK_HAIR:
		bValue = rand() % 7;
		if(m_bHair == bValue)
			bValue = (m_bHair + 1) % 7;
		break;
	case IK_RACE:
		bValue = rand() % 3;
		if(m_bRace == bValue)
			bValue = (m_bRace + 1) % 3;
		break;
	case IK_SEX:
		bValue = !m_bSex;
		break;
	case IK_NAME:
		break;
	case IK_AIDCOUNTRY:
#ifdef DEF_UDPLOG
		_AtlModule.m_pUdpSocket->LogAidCountry(LOGMAP_AIDCOUNTRYCHANGE, this, bValue, m_bAidCountry);
#endif
		break;
	case IK_COUNTRY:
		_AtlModule.CheckMonthRank(this, m_bCountry,0,0);
		_AtlModule.CheckMonthRank(this, bValue,m_dwMonthPvPoint,m_dwPvPTotalPoint);
#ifdef DEF_UDPLOG
		_AtlModule.m_pUdpSocket->LogCountry(LOGMAP_COUNTRYCHANGE, this, bValue, m_bCountry);
#endif
		break;
	default:
		break;
	}

	_AtlModule.SendMW_CHANGECHARBASE_ACK(
		m_dwID,
		m_dwKEY,
		bType,
		bValue,
		strName);

	_AtlModule.SendDM_SAVECHARBASE_REQ(
		m_dwID,
		bType,
		bValue,
		strName);

	return TRUE;
}

void CTPlayer::SendQuestListPossible(CPacket * pPacket)
{
	BYTE bCount;
	WORD wNpcID;
	(*pPacket)
		>> bCount;

	MAPQUESTTEMP::iterator itTemp;
	MAPDWORD mapCanRunQuest;
	MAPMAPDWORD mapNpcQuest;
	MAPWORD mapArena;
	mapCanRunQuest.clear();
	mapNpcQuest.clear();
	mapArena.clear();

	for(BYTE i=0; i<bCount; i++)
	{
		(*pPacket)
			>> wNpcID;

		CTNpc * pNpc = _AtlModule.FindTNpc(wNpcID);
		if(!pNpc)
			continue;

		BYTE bNpcCountry = pNpc->m_pLocal ? pNpc->m_pLocal->m_bCountry : pNpc->m_bCountry;
		if(bNpcCountry != TCONTRY_N && bNpcCountry != TCONTRY_B && bNpcCountry != m_bCountry)
			continue;

		if(pNpc->m_bType == TNPC_ARENA && pNpc->m_pArena)
		{
			if(mapArena.find(pNpc->m_pArena->m_wID) == mapArena.end())
			{
				VDWORD vParty;
				MAPPLAYER::iterator itP;
				
				if(pNpc->m_pArena->m_mapFighter[0].size())
				{
					vParty.clear();
					for(itP=pNpc->m_pArena->m_mapFighter[0].begin(); itP != pNpc->m_pArena->m_mapFighter[0].end(); itP++)
						vParty.push_back(itP->second->m_dwID);

					SendCS_ARENATEAM_ACK(pNpc->m_pArena->m_wID, 1, vParty);
				}

				if(pNpc->m_pArena->m_mapFighter[1].size())
				{
					vParty.clear();
					for(itP=pNpc->m_pArena->m_mapFighter[1].begin(); itP != pNpc->m_pArena->m_mapFighter[1].end(); itP++)
						vParty.push_back(itP->second->m_dwID);

					SendCS_ARENATEAM_ACK(pNpc->m_pArena->m_wID, 2, vParty);
				}

				mapArena.insert(MAPWORD::value_type(pNpc->m_pArena->m_wID, pNpc->m_pArena->m_wID));
			}
		}

		mapCanRunQuest.clear();
		BYTE bMinLevel = 0xFF;
		for(itTemp=pNpc->m_mapQuest.begin(); itTemp!=pNpc->m_mapQuest.end(); itTemp++)
		{
			LPQUESTTEMP pQuestTemp = (*itTemp).second;
			BYTE bLevel = 0;
			BYTE bCanRun = CanRunQuest(pQuestTemp, _AtlModule.m_dwTick, bLevel);
			if(bCanRun == QCT_NONE || bCanRun == QCT_UPPERLEVEL)
			{
				if(bCanRun == QCT_UPPERLEVEL && bMinLevel > bLevel)
					bMinLevel = bLevel;

				mapCanRunQuest.insert(MAPDWORD::value_type(pQuestTemp->m_dwQuestID, MAKEWORD(bCanRun, bLevel)));
			}
		}

		MAPDWORD::iterator it;
		for(it=mapCanRunQuest.begin(); it!=mapCanRunQuest.end();)
		{
			if(LOBYTE((*it).second) == QCT_UPPERLEVEL && HIBYTE((*it).second) != bMinLevel)
				it = mapCanRunQuest.erase(it);
			else
				it++;
		}

		mapNpcQuest.insert(MAPMAPDWORD::value_type(wNpcID, mapCanRunQuest));
	}

	SendCS_QUESTLIST_POSSIBLE_ACK(&mapNpcQuest);
}

BYTE CTPlayer::CheckItemBuff(CTSkillTemp * pSkill)
{
	BYTE bIsLucky = pSkill->IsLuckyPotion();
	BYTE bIsExp = pSkill->IsExpPotion();

	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		if(m_vMaintainSkill[i]->m_pTSKILL->m_wID == pSkill->m_wID)
			return FALSE;

		if(bIsLucky && m_vMaintainSkill[i]->m_pTSKILL->IsLuckyPotion())
			return FALSE;

		if(bIsExp && m_vMaintainSkill[i]->m_pTSKILL->IsExpPotion())
			return FALSE;
	}

	return TRUE;
}

void CTPlayer::DeleteItem(WORD wItemID)
{	
	MAPTINVEN::iterator itInven;
	for(itInven = m_mapTINVEN.begin(); itInven != m_mapTINVEN.end(); itInven++)
	{
		BYTE bInvenID = (*itInven).first;
		CTInven* pInven = (*itInven).second;
		if(!pInven)
			continue;

		MAPTITEM::iterator itItem;
		for(itItem = pInven->m_mapTITEM.begin();itItem != pInven->m_mapTITEM.end(); )
		{
			BYTE bItemID = (*itItem).first;
			CTItem* pItem = (*itItem).second;
			if(!pItem)
				continue;
			
			if(wItemID == pItem->m_wItemID )
			{
				pInven->m_mapTITEM.erase(itItem++);
				delete pItem;

				SendCS_DELITEM_ACK(bInvenID,bItemID);				
			}
			else
				itItem++;
		}
	}
}

BYTE CTPlayer::GetPvPStatus()
{
	if(m_pLocal && m_pLocal->m_bStatus == BS_BATTLE)
		return PVPS_LOCAL;

	if(_AtlModule.IsTournamentMap(m_wMapID))
		return PVPS_LOCAL;

	return PVPS_NORMAL;
}

void CTPlayer::GainPvPoint(DWORD dwPoint, BYTE bEvent, BYTE bType, LPTRECORDSET pRec)
{
	if(pRec)
	{
		BYTE bSame=0;
		for(DWORD i=0; i<m_vPvPRecent.size(); i++)
		{
			if(_AtlModule.m_timeCurrent - m_vPvPRecent[i].m_dTime < 600 &&
				m_vPvPRecent[i].m_bWin &&
				m_vPvPRecent[i].m_strName == pRec->m_strName)
				bSame++;
		}

		if(bSame >= 3)
			dwPoint = 0;

		pRec->m_bWin = TRUE;
		pRec->m_dwPoint = dwPoint;
		RecordPvP(pRec);
	}

	if(!dwPoint)
		return;

	if(bType & PVP_TOTAL)
	{
		m_dwPvPTotalPoint += dwPoint;
		m_dwMonthPvPoint += dwPoint;
	}

	if(bType & PVP_USEABLE)
		m_dwPvPUseablePoint += dwPoint;
	 
	SendCS_PVPPOINT_ACK(m_dwPvPTotalPoint, m_dwPvPUseablePoint, bEvent,m_dwMonthPvPoint); 

#ifdef DEF_UDPLOG
	_AtlModule.m_pUdpSocket->LogPvPointChar(LOGMAP_PVPOINTGAINCHAR,this,dwPoint,bEvent,bType,pRec ? pRec->m_strName : NAME_NULL);
	if(bEvent == PVPE_GUILD)
		_AtlModule.m_pUdpSocket->LogPvPointGuild(LOGMAP_PVPOINTUSEGUILD,dwPoint,0,m_bCountry,m_dwGuildID,bEvent,bType);
#endif

	 
	if(bType & PVP_TOTAL)
		_AtlModule.CheckMonthRank(this, m_bCountry, m_dwMonthPvPoint, m_dwPvPTotalPoint);
}

void CTPlayer::UsePvPoint(DWORD dwPoint, BYTE bEvent, BYTE bType, LPTRECORDSET pRec)
{
	if(pRec)
	{
		pRec->m_bWin = FALSE;
		pRec->m_dwPoint = dwPoint;
		RecordPvP(pRec);
	}

	if(!dwPoint)
		return;

	if(bType & PVP_TOTAL)
	{
		m_dwPvPTotalPoint = m_dwPvPTotalPoint > dwPoint ? m_dwPvPTotalPoint-dwPoint : 0;
		m_dwMonthPvPoint = m_dwMonthPvPoint > dwPoint ? m_dwMonthPvPoint-dwPoint : 0;
	}

	if(bType & PVP_USEABLE)
		m_dwPvPUseablePoint = m_dwPvPUseablePoint > dwPoint ? m_dwPvPUseablePoint-dwPoint : 0;
	 
	SendCS_PVPPOINT_ACK(m_dwPvPTotalPoint, m_dwPvPUseablePoint, bEvent,m_dwMonthPvPoint); 

#ifdef DEF_UDPLOG
	_AtlModule.m_pUdpSocket->LogPvPointChar(LOGMAP_PVPOINTUSECHAR,this,dwPoint,bEvent,bType,pRec ? pRec->m_strName : NAME_NULL);
#endif

	 
	if( bType & PVP_TOTAL )
		_AtlModule.CheckMonthRank(this, m_bCountry, m_dwMonthPvPoint, m_dwPvPTotalPoint);
}

void CTPlayer::RecordPvP(LPTRECORDSET pRec)
{
	pRec->m_dTime = _AtlModule.m_timeCurrent;
	m_vPvPRecent.push_back(*pRec);

	VTRECORDSET::iterator it;
	for(it = m_vPvPRecent.begin(); it != m_vPvPRecent.end();)
	{
		if(m_vPvPRecent.size() <= PVP_RECENTRECORDCOUNT)
			break;

		if( _AtlModule.m_timeCurrent - (*it).m_dTime > PVP_SAVETIME)			
			m_vPvPRecent.erase(it++);
		else
			it++;
	}

	m_aPvPRecord[pRec->m_bClass][pRec->m_bWin]++;

	if(!pRec->m_bWin)
	{
		m_wMonthLose++;
		m_dwTotalLose++;
	}
	else
	{
		m_wMonthWin++;
		m_dwTotalWin++;
	}
}

void CTPlayer::DuelScoreReset()
{
	for(BYTE i = 0; i < TCLASS_COUNT; i++)
	{
		m_aDuelScore[i][0] = 0;
		m_aDuelScore[i][1] = 0;
	}
}

void CTPlayer::RecordDuel(CTPlayer* pChar, BYTE bWin, DWORD dwPoint)
{
	TRECORDSET set;
	set.m_bClass = pChar->m_bClass;
	set.m_bLevel = pChar->m_bLevel;
	set.m_bWin = bWin;
	set.m_dwPoint = dwPoint;
	set.m_dTime = _AtlModule.m_timeCurrent;
	set.m_strName = pChar->m_strNAME;	
	m_vDuelRecordSet.push_back(set);

	if(m_vDuelRecordSet.size() > RECENTRECORD )
		m_vDuelRecordSet.erase(m_vDuelRecordSet.begin());

	m_aDuelScore[pChar->m_bClass][bWin]++;	
}

CTSkill * CTPlayer::GetMonRecallSkill()
{
	MAPTSKILL::iterator it;
	for(it=m_mapTSKILL.begin(); it!=m_mapTSKILL.end(); it++)
	{
		CTSkill * pSkill = (*it).second;
		if(pSkill->m_pTSKILL->IsMonRecall())
			return pSkill;
	}

	return NULL;
}

BYTE CTPlayer::CheckProtected(DWORD dwTarget, BYTE bOption)
{
	MAPTPROTECTED::iterator it = m_mapTPROTECTED.find(dwTarget);
	if(it!=m_mapTPROTECTED.end() &&
		((*it).second->m_bOption & bOption))
		return FALSE;

	return TRUE;
}

void CTPlayer::RecallRevival()
{
	MAPTRECALLMON::iterator itTMON;

	for( itTMON = m_mapRecallMon.begin(); itTMON != m_mapRecallMon.end(); itTMON++)
		(*itTMON).second->ForceMaintain(TREVIVAL_SKILL, m_dwID, m_bType, (*itTMON).second->m_dwID, (*itTMON).second->m_bType, 0);
}

void CTPlayer::RecallRevivalErase()
{
	MAPTRECALLMON::iterator itTMON;

	for( itTMON = m_mapRecallMon.begin(); itTMON != m_mapRecallMon.end(); itTMON++)
		(*itTMON).second->EraseMaintainSkill(TREVIVAL_SKILL);
}

void CTPlayer::ClearAuctionList()
{
	m_vAuctionInterest.clear();
	m_vAuctionBid.clear();
	m_vAuctionReg.clear();
}

void CTPlayer::AuctionInterestSave(BYTE bType, DWORD dwAuctionID)
{	
	VDWORD::iterator itD;
	for( itD = m_vAuctionInterest.begin(); itD != m_vAuctionInterest.end(); itD++)
	{
		if( (*itD) == dwAuctionID )
		{
			m_vAuctionInterest.erase(itD);
			break;
		}
	}

	if(bType)
		m_vAuctionInterest.push_back(dwAuctionID);

}


void CTPlayer::AuctionBidListSave(BYTE bType,DWORD dwAuctionID)
{
	VDWORD::iterator itD;
	for(itD = m_vAuctionBid.begin(); itD != m_vAuctionBid.end(); itD++)
	{
		if( (*itD) == dwAuctionID )
		{
			m_vAuctionBid.erase(itD);
			break;
		}
	}

	if(bType)
		m_vAuctionBid.push_back(dwAuctionID);
}

void CTPlayer::AuctionRegListSave( BYTE bType, DWORD dwAuctionID )
{
	VDWORD::iterator itD;
	for( itD = m_vAuctionReg.begin(); itD != m_vAuctionReg.end(); itD++)
	{
		if( (*itD) == dwAuctionID )
		{
			m_vAuctionReg.erase(itD);
			break;
		}
	}

	if(bType)
		m_vAuctionReg.push_back(dwAuctionID);
}

void CTPlayer::ResetCloak()
{
	MAPTITEM::iterator it;
	MAPTINVEN::iterator itInv;
	for(itInv=m_mapTINVEN.begin(); itInv!=m_mapTINVEN.end(); itInv++)
	{
		CTInven * pInven = (*itInv).second;
		for( it = pInven->m_mapTITEM.begin(); it != pInven->m_mapTITEM.end(); it++)
		{
			CTItem * pItem = (*it).second;
			if( pItem->m_dwExtValue[IEV_GUILD] )
			{
				pItem->m_dwExtValue[IEV_GUILD] = 0;
				if(m_pMAP && pInven->m_bInvenID == INVEN_EQUIP)
				{
					VPLAYER vPLAYERS;
					vPLAYERS.clear();

					m_pMAP->GetNeighbor(
						&vPLAYERS,
						m_fPosX,
						m_fPosZ);

					while(!vPLAYERS.empty())
					{
						CTPlayer *pChar = vPLAYERS.back();

						pChar->SendCS_RESETCLOAK_ACK(
							m_dwID,
							pInven->m_bInvenID,
							pItem->m_bItemID);

						vPLAYERS.pop_back();
					}

					vPLAYERS.clear();

				}
				else
					SendCS_RESETCLOAK_ACK(
						m_dwID,
						pInven->m_bInvenID,
						pItem->m_bItemID);
			}
		}
	}
}
 
void CTPlayer::MonthRankRest()
{
	m_dwMonthPvPoint = 0;
	m_dwMonthRankOrder = 0;
	m_bMonthRankPercent = 0;
	m_wMonthWin = 0;
	m_wMonthLose = 0;
	m_strMonthSay = NAME_NULL;
}

BYTE CTPlayer::CanUseAuction()
{
	DWORD dwSum = DWORD(m_wPostTotal + m_vAuctionReg.size() + m_vAuctionBid.size());
	if(dwSum >= MAX_POST)
		return FALSE;

	return TRUE;
}

BYTE CTPlayer::HaveItem(WORD wItemID, BYTE bCount)
{
	BYTE bHave = 0;

	MAPTINVEN::iterator itInv;
	for(itInv=m_mapTINVEN.begin(); itInv!=m_mapTINVEN.end(); itInv++)
	{
		CTInven * pInven = (*itInv).second;
		MAPTITEM::iterator it;
		for(it=pInven->m_mapTITEM.begin(); it!=pInven->m_mapTITEM.end(); it++)
		{
			if((*it).second->m_pTITEM->m_wItemID == wItemID)
				bHave += (*it).second->m_bCount;

			if(bHave >= bCount)
				return TRUE;
		}
	}

	return FALSE;
}

void CTPlayer::UseItem(WORD wItemID, BYTE bCount)
{
	MAPTINVEN::iterator itInven;
	for(itInven = m_mapTINVEN.begin(); itInven != m_mapTINVEN.end(); itInven++)
	{
		BYTE bInvenID = (*itInven).first;
		CTInven* pInven = (*itInven).second;
		if(!pInven)
			continue;

		MAPTITEM::iterator itItem;
		for(itItem = pInven->m_mapTITEM.begin();itItem != pInven->m_mapTITEM.end(); )
		{
			BYTE bItemID = (*itItem).first;
			CTItem* pItem = (*itItem).second;
			if(!bCount)
				return;

			if(wItemID == pItem->m_wItemID )
			{
				if(pItem->m_bCount > bCount)
				{
					pItem->m_bCount -= bCount;
					SendCS_UPDATEITEM_ACK(
						pItem,
						bInvenID);

					return;
				}
				else
				{
					bCount -= pItem->m_bCount;
					pInven->m_mapTITEM.erase(itItem++);
					delete pItem;

					SendCS_DELITEM_ACK(bInvenID, bItemID);				
				}
			}
			else
				itItem++;
		}
	}
}

BYTE CTPlayer::IsEquipShield()
{
	CTInven *pTEQUIP = FindTInven(INVEN_EQUIP);
	CTItem * pItem = pTEQUIP->FindTItem(ES_SNDWEAPON);

	if(pItem && pItem->m_pTITEM->m_bType == IT_SHIELD)
		return TRUE;

	return FALSE;
}

BYTE CTPlayer::IsTutorial()
{
	return m_bStartAct == 1 && m_wMapID == TUTORIAL_MAPID;
}

BYTE CTPlayer::ProtectTutorial()
{
	return IsTutorial() || m_bStartAct == 2;
}

void CTPlayer::ReleaseTutorial()
{
	m_dwGold = m_dwSilver = m_dwCooper = 0;
//	m_dwEXP = 0;
//	m_bLevel = 1;
	m_wSpawnID = 0;
	m_bStartAct = 2;

	MAPTINVEN::iterator itInven;
	for(itInven = m_mapTINVEN.begin(); itInven != m_mapTINVEN.end(); itInven++)
	{
		BYTE bInvenID = (*itInven).first;
		CTInven* pInven = (*itInven).second;

		MAPTITEM::iterator itItem;
		for(itItem = pInven->m_mapTITEM.begin();itItem != pInven->m_mapTITEM.end(); itItem++)
		{
			delete (*itItem).second;
			SendCS_DELITEM_ACK(bInvenID, (*itItem).first);				
		}

		pInven->m_mapTITEM.clear();
	}

	MAPTSKILL::iterator itSk;
	for(itSk=m_mapTSKILL.begin(); itSk!=m_mapTSKILL.end(); itSk++)
	{
		CTSkill * pSkill = (*itSk).second;
		EraseHotkey(pSkill->m_pTSKILL->m_wID);
		delete pSkill;
	}
	m_vRemainSkill.clear();
	m_mapTSKILL.clear();

	MAPTHOTKEYINVEN::iterator itHotkey;
	for(itHotkey=m_mapHotkeyInven.begin(); itHotkey!=m_mapHotkeyInven.end(); itHotkey++)
		SendCS_HOTKEYCHANGE_ACK(BYTE((*itHotkey).first), (*itHotkey).second->m_hotkey, MAX_HOTKEY_POS);

	while(m_vMaintainSkill.size())
	{
		SendCS_SKILLEND_ACK(m_dwID,m_bType,m_vMaintainSkill.back()->m_pTSKILL->m_wID);
		delete m_vMaintainSkill.back();
		m_vMaintainSkill.pop_back();
	}
	m_vMaintainSkill.clear();

	ResetCoolTime(0);
	ClearRecallMon(TRUE);

	MAPTPET::iterator itPET;
	for(itPET = m_mapTPET.begin(); itPET != m_mapTPET.end(); itPET++)
	{
		SendCS_PETDEL_ACK(PET_SUCCESS, (*itPET).first);
		delete (*itPET).second;
	}
	m_mapTPET.clear();

	MAPQUEST::iterator itQt;
	for(itQt = m_mapQUEST.begin(); itQt != m_mapQUEST.end(); itQt++)
	{
		if((*itQt).second->m_bTriggerCount > (*itQt).second->m_bCompleteCount )
		{
			DWORD dwQuestID = itQt->second->m_pQUEST->m_dwQuestID;

			SendCS_QUESTCOMPLETE_ACK(
				QR_DROP,
				dwQuestID,
				0,0,
				dwQuestID);
		}
	}
	ClearQuest();

	SendCS_SKILLLIST_ACK(_AtlModule.m_dwTick);
	SendCS_MONEY_ACK();
	SendCS_EXP_ACK();
}

BYTE CTPlayer::InitCharInfo()
{
	if(m_bStartAct != 2)
		return TRUE;

	if(m_dwGold || m_dwSilver || m_dwCooper)
		return FALSE;

	m_bStartAct = 0;

	DWORD dwMaxHP = m_dwHP = GetMaxHP();
	DWORD dwMaxMP = m_dwMP = GetMaxMP();

	MAPTINVEN::iterator itInven;
	for(itInven = m_mapTINVEN.begin(); itInven != m_mapTINVEN.end(); itInven++)
	{
		BYTE bInvenID = (*itInven).first;
		CTInven* pInven = (*itInven).second;

		MAPTITEM::iterator itItem;
		for(itItem = pInven->m_mapTITEM.begin();itItem != pInven->m_mapTITEM.end(); itItem++)
		{
			BYTE bItemID = (*itItem).first;
			CTItem* pItem = (*itItem).second;

			SendCS_ADDITEM_ACK(pItem, bInvenID);				
		}
	}
	SendCS_MOVEITEM_ACK(MI_SUCCESS);

	MAPTSKILL::iterator itSk;
	for(itSk=m_mapTSKILL.begin(); itSk!=m_mapTSKILL.end(); itSk++)
	{
		CTSkill * pSkill = (*itSk).second;
		SendCS_SKILLBUY_ACK(
			SKILL_SUCCESS,
			pSkill->m_pTSKILL->m_wID,
			pSkill->m_bLevel);
	}

	MAPTHOTKEYINVEN::iterator itHotkey;
	for(itHotkey=m_mapHotkeyInven.begin(); itHotkey!=m_mapHotkeyInven.end(); itHotkey++)
		SendCS_HOTKEYCHANGE_ACK(BYTE((*itHotkey).first), (*itHotkey).second->m_hotkey, MAX_HOTKEY_POS);

	_AtlModule.SendMW_LEVELUP_ACK(
		m_dwID,
		m_dwKEY,
		m_bLevel);
	SendCS_LEVEL_ACK(m_dwID, m_bLevel);
	SendCS_HPMP_ACK(m_dwID, m_bType, m_bLevel, dwMaxHP, m_dwHP, dwMaxMP, m_dwMP);
	SendCS_EQUIP_ACK(this);
	SendCS_EXP_ACK();
	SendCS_CHARSTATINFO_ACK(this);
	SendCS_PETLIST_ACK();
	SendCS_POSTINFO_ACK();

	if(m_bClass == TCLASS_SORCERER &&
		m_wTemptedMon)
	{
		MAPTMONSTERTEMP::iterator itMon = _AtlModule.m_mapTMONSTER.find(m_wTemptedMon);
		if(itMon!=_AtlModule.m_mapTMONSTER.end())
		{
			DWORD dwATTR = MAKELONG((*itMon).second->m_wSummonAttr, m_bLevel);
			FLOAT fPosX = m_fPosX - 2*sinf(m_wDIR*FLOAT(M_PI)/900);
			FLOAT fPosY = m_fPosY;
			FLOAT fPosZ = m_fPosZ - 2*cosf(m_wDIR*FLOAT(M_PI)/900);

			_AtlModule.SendMW_CREATERECALLMON_ACK(
				m_dwID,
				m_dwKEY,
				0,
				m_wTemptedMon,
				dwATTR,
				0,
				NAME_NULL,
				0,
				m_bLevel,
				(*itMon).second->m_bClass,
				(*itMon).second->m_bRace,
				TA_STAND,
				OS_WAKEUP,
				MT_NORMAL,
				0,0,0,0,
				100,
				1,
				fPosX,
				fPosY,
				fPosZ,
				m_wDIR,
				(*itMon).second->m_vSKILL);
		}
	}

	CString strHelpMessage;
	BYTE bHelpID = _AtlModule.GetHelpMessage(this, strHelpMessage);
	if(bHelpID)
		SendCS_HELPMESSAGE_ACK(bHelpID, strHelpMessage);

	return TRUE;
}

void CTPlayer::ResetRPS()
{
	m_bRPSType = 0;
	m_bRPSWin = 0;
}

BYTE CTPlayer::CanTalk(BYTE bChatType, BYTE bCountry, BYTE bAidCountry)
{
	if(m_bCountry == TCONTRY_PEACE ||
        bCountry == TCONTRY_PEACE ||
		bCountry == TCONTRY_N)
		return TRUE;

	switch(bChatType)
	{
	case CHAT_NEAR:
		{
			if((m_bCountry == TCONTRY_B ||
				bCountry == TCONTRY_B) &&
				MAKEWORD( BYTE(INT(m_fPosX) / UNIT_SIZE), BYTE(INT(m_fPosZ) / UNIT_SIZE)) == 3 )
				return TRUE;

			if(GetWarCountry() == GetAttackCountry(bCountry, bAidCountry))
				return TRUE;
		}
		break;
	case CHAT_WHISPER:
	case CHAT_SHOW:
	case CHAT_MAP:
	case CHAT_WORLD:
		{
			if(GetWarCountry() == GetAttackCountry(bCountry, bAidCountry))
				return TRUE;
		}
		break;
	case CHAT_TACTICS:
	case CHAT_FORCE:
	case CHAT_GUILD:
	case CHAT_PARTY:
		{
			return TRUE;
		}
		break;
	}

	return FALSE;
}

BYTE CTPlayer::CheckChat(CString strTalk)
{
	DWORD dwCODE = 0;
	sscanf( LPCSTR(strTalk.Left(8)), _T("%X"), &dwCODE);
	WORD wNewSize = LOWORD(dwCODE);

	DWORD dwTick = GetTickCount();

	if(m_chatlog.m_dwRecent && dwTick - m_chatlog.m_dwRecent < 10000)
	{
		m_chatlog.m_dwSize += wNewSize;
		if(100 < m_chatlog.m_dwSize)
		{
			//CTime t(CTime::GetCurrentTime());
			//if(t.GetTime() < m_nChatBanTime)
			//	m_nChatBanTime += 300;
			//else
			//	m_nChatBanTime = t.GetTime() + 300;

			_AtlModule.SendMW_CHATBAN_ACK(m_strNAME,5,0,0);

			m_chatlog.m_dwRecent = dwTick;
			m_chatlog.m_dwSize = 0;

			return FALSE;
		}
	}
	else
	{
		m_chatlog.m_dwRecent = dwTick;
		m_chatlog.m_dwSize = 0;
	}

	TCHATLOG log;
	log.m_dwRecent = dwTick;
	log.m_dwSize = wNewSize;
	m_vChatLog.push_back(log);

	int nCount = int(m_vChatLog.size());
	int nInterval = 0;
	for(int cl=0; cl<nCount-1; cl++)
	{
		int nItv = m_vChatLog[cl+1].m_dwRecent - m_vChatLog[cl].m_dwRecent;
		if(!nInterval)
			nInterval = nItv;
		else
		{
			if(abs(nInterval-nItv) > 1000)
			{
				m_vChatLog.clear();
				break;
			}
			else
				nInterval = nItv;
		}
	}

	if(m_vChatLog.size() > 8)
	{
		//CTime t(CTime::GetCurrentTime());
		//if(t.GetTime() < m_nChatBanTime)
		//	m_nChatBanTime += 300;
		//else
		//	m_nChatBanTime = t.GetTime() + 300;
		_AtlModule.SendMW_CHATBAN_ACK(m_strNAME,5,0,0);

		m_vChatLog.clear();

		return FALSE;
	}

	return TRUE;
}

DWORD CTPlayer::GetAidLeftTime()
{
	if(m_bAidCountry >= TCONTRY_B)
		return 0;

	if(_AtlModule.m_timeCurrent >= WEEK_ONE + m_dlAidDate)
		return 0;

	return DWORD(m_dlAidDate + WEEK_ONE - _AtlModule.m_timeCurrent);
}
