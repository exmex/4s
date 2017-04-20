#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"


CTObjBase::CTObjBase()
{
	m_pMAP = NULL;
	m_dwID = 0;

	m_pTCLASS = NULL;
	m_pTRACE = NULL;
	m_pTLEVEL = NULL;
	m_pTPREVLEVEL = NULL;

	m_dwHP = 0;
	m_dwMP = 0;
	m_dwRegion = 0;

	m_dwRecoverHPTick = 0;
	m_dwRecoverMPTick = 0;

	m_wTemptedMon = 0;
	m_bLevel = 0;
	m_wMapID = 0;
	m_pLocal = NULL;
	m_fPosX = 0.0f;
	m_fPosY = 0.0f;
	m_fPosZ = 0.0f;
	m_fSize = OBJECT_SIZE/2;

	m_bCountry = TCONTRY_N;
	m_bAidCountry = TCONTRY_N;
	m_bOriCountry = TCONTRY_N;
	m_bBlock = FALSE;
	m_pInstanceSkill = NULL;
	m_dlAidDate = 0;

	m_bMouseDIR = TKDIR_N;
	m_bKeyDIR = TKDIR_N;
	m_wPitch = 0;
	m_wDIR = 0;
	m_wCurChargeSkill = 0;
	m_wLocalID = 0;

	m_bAction = TA_STAND;
	m_bMode = MT_NORMAL;

	m_bChannel = 0;
	m_bType = 0;
	m_bCanAttack = TRUE;
	m_bIsSpecial = FALSE;
	m_bExit = FALSE;

	m_wPartyID = 0;
	m_bPartyType = PT_FREE;
	m_dwGuildID = 0;
	m_bGuildDuty = 0;
	m_bGuildPeer = 0;
	m_strGuildName = NAME_NULL;
	m_bGetaway = FALSE;

	m_dwTacticsID = 0;
	m_strTacticsName = NAME_NULL;

	m_dwFame = 0;
	m_dwFameColor = 0;
	memset(&m_aftermath, 0, sizeof(TAFTERMATH));

	m_mapTINVEN.clear();
	m_mapTSKILL.clear();
	m_vMaintainSkill.clear();
	m_vRemainSkill.clear();
}

CTObjBase::~CTObjBase()
{
	ClearInven();
	ClearSkill();
	ClearMaintainSkill();
}

void CTObjBase::ClearInven()
{
	MAPTINVEN::iterator itINVEN;

	for( itINVEN = m_mapTINVEN.begin(); itINVEN != m_mapTINVEN.end(); itINVEN++)
		delete (*itINVEN).second;

	m_mapTINVEN.clear();
}

void CTObjBase::ClearSkill()
{
	MAPTSKILL::iterator itTSKILL;

	for( itTSKILL = m_mapTSKILL.begin(); itTSKILL != m_mapTSKILL.end(); itTSKILL++)
		delete (*itTSKILL).second;

	m_vRemainSkill.clear();
	m_mapTSKILL.clear();
}

void CTObjBase::ClearMaintainSkill()
{
	while(m_vMaintainSkill.size())
	{
		delete m_vMaintainSkill.back();
		m_vMaintainSkill.pop_back();
	}

	m_vMaintainSkill.clear();
}
CTInven *CTObjBase::FindTInven( BYTE bInvenID)
{
	MAPTINVEN::iterator finder = m_mapTINVEN.find(bInvenID);

	if( finder != m_mapTINVEN.end() )
		return (*finder).second;

	return NULL;
}

BYTE CTObjBase::CanEquip( CTItem *pTItem, BYTE bItemID)
{
	BYTE bResult = MI_SUCCESS;
	BYTE bHaveSkill = FALSE;

	if(!pTItem->CanUse())
		return MI_WRAP;

	if(!pTItem->m_pTITEM->m_bEquipSkill)
	{
		bHaveSkill = TRUE;
		bResult = MI_SUCCESS;
	}
	else
	{
		MAPTSKILL::iterator it;
		for(it=m_mapTSKILL.begin(); it != m_mapTSKILL.end(); it++)
		{
			CTSkill * pSkill = (*it).second;
			if(pSkill->m_pTSKILL->CanEquip(pTItem->m_pTITEM->m_bKind))
			{
				bHaveSkill = TRUE;
				bResult = MI_SUCCESS;
				break;
			}
		}
	}

	if(!bHaveSkill)
		bResult = MI_NOSKILL;
	else if(!(BITSHIFTID(bItemID) & pTItem->m_pTITEM->m_dwSlotID))
		bResult = MI_CANNOTEQUIP;
	else if(!(BITSHIFTID(GetClass()) & pTItem->m_pTITEM->m_dwClassID))
		bResult = MI_NOMATCHCLASS;
	else if(GetLevel() < pTItem->GetEquipLevel())
		bResult = MI_LOWLEVEL;

	return bResult;
}
BYTE CTObjBase::CanPush(CTItem * pTItem, BYTE bInven, BYTE bSlot)
{
	if(bSlot == INVALID_SLOT)
	{
		VTITEM vITEM;
		vITEM.clear();

		vITEM.push_back(pTItem);
		BYTE bRet = CanPush(&vITEM, 0);

		vITEM.clear();

		return bRet;
	}

	CTInven * pInven = FindTInven(bInven);
	if(!pInven)
		return FALSE;

	CTItem * pItem = pInven->FindTItem(bSlot);
	if(!pItem)
		return TRUE;

	if((*pItem) == (*pTItem) &&
		pItem->m_bCount < pItem->m_pTITEM->m_bStack)
	{
		if(pItem->m_bCount + pTItem->m_bCount <= pItem->m_pTITEM->m_bStack)
			return TRUE;
	}

	return FALSE;
}

BYTE CTObjBase::CanPush( LPVTITEM pTITEM, BYTE bIndex)
{
	if( bIndex < BYTE(pTITEM->size()) )
	{
		CTInven *pTINVEN = FindTInven(INVEN_DEFAULT);
		CTItem *pTItem = (*pTITEM)[bIndex];

		BYTE bItemID = pTINVEN ? pTINVEN->GetEasePos(pTItem) : INVALID_SLOT;
		BYTE bResult = FALSE;
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

		if( bItemID == INVALID_SLOT || !pTINVEN )
			return bResult;

		CTItem *pDESTItem = pTINVEN->FindTItem(bItemID);
		if(pDESTItem)
		{
			bCount = pDESTItem->m_pTITEM->m_bStack - pDESTItem->m_bCount;
			bCount = min( pTItem->m_bCount, bCount);

			pDESTItem->m_bCount += bCount;
			pTItem->m_bCount -= bCount;

			bResult = CanPush( pTITEM, pTItem->m_bCount ? bIndex : bIndex + 1);
			pDESTItem->m_bCount -= bCount;
			pTItem->m_bCount += bCount;
		}
		else
		{
			pDESTItem = new CTItem();

			pDESTItem->Copy(pTItem, FALSE);
			pDESTItem->m_bItemID = bItemID;

			bCount = min( pTItem->m_pTITEM->m_bStack, pTItem->m_bCount);
			pDESTItem->m_bCount = bCount;
			pTItem->m_bCount -= bCount;

			pTINVEN->m_mapTITEM.insert( MAPTITEM::value_type( pDESTItem->m_bItemID, pDESTItem));
			bResult = CanPush( pTITEM, pTItem->m_bCount ? bIndex : bIndex + 1);

			pTINVEN->m_mapTITEM.erase(pDESTItem->m_bItemID);
			delete pDESTItem;
			pDESTItem = NULL;
			pTItem->m_bCount += bCount;
		}

		return bResult;
	}

	return TRUE;
}

void CTObjBase::ChgMode( BYTE bMode)
{
	if(!m_pMAP)
		return;

	DWORD dwTick = _AtlModule.m_dwTick;

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	m_pMAP->GetNeighbor(
		&vPLAYERS,
		m_fPosX,
		m_fPosZ);

	m_bMode = bMode;
	m_dwLastAtkTick = dwTick;

	if(bMode == MT_BATTLE)
	{
		m_dwRecoverHPTick = dwTick + RECOVER_INIT;
		m_dwRecoverMPTick = dwTick + RECOVER_INIT;
	}

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_CHGMODE_ACK(
			m_dwID,
			m_bType,
			m_bMode);

		if(m_bType == OT_MON)
		{
			CTMonster * pMon = (CTMonster *)this;
			pChar->SendCS_CHANGECOLOR_ACK(
				m_dwID,
				m_bType,
				pMon->GetColor( pChar->m_dwID, pChar->GetPartyID(), pChar->GetWarCountry()),
				pMon->m_bCountry);
		}

		vPLAYERS.pop_back();
	}

	vPLAYERS.clear();
}

void CTObjBase::CalcDamage( DWORD dwAttackID,
						    BYTE bAttackType,
							MAPWORD& mapDamage,
							int & nDamageHP,
							int & nDamageMP,
							DWORD dwPhyMin,
							DWORD dwPhyMax,
							DWORD dwMagicMin,
							DWORD dwMagicMax,
							BYTE & bHit,
							CTSkill * pSkill,
							BYTE bDamageType)
{
	DWORD dwDamage = 0;
	DWORD dwDefendPower = 0;
	DWORD dwShildDefend = 0;
	int nHP = m_dwHP;
	int nMP = m_dwMP;
	DWORD dwMaxHP = GetMaxHP();
	DWORD dwMaxMP = GetMaxMP();

	for(DWORD i=0; i<pSkill->m_pTSKILL->m_vData.size(); i++)
	{
		DWORD dwValue=0;

		LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[i];
		if(pData->m_bType != bDamageType)
			continue;

		switch(pData->m_bAttr)
		{
		case SATT_NONE:
			break;
		case SATT_PHYSIC:
		case SATT_LONG:
			{
				if((dwAttackID != m_dwID ||
					bAttackType != m_bType) &&
					HaveExcptPhysic())
					break;

				switch(pData->m_bExec)
				{
				case MTYPE_DAMAGE:
					{
						dwShildDefend = GetShieldDP();
						dwDefendPower = dwShildDefend + GetDefendPower();
						DWORD dwA = max(int(dwPhyMin - dwDefendPower), 5);
						DWORD dwB = max(int(dwPhyMax - dwDefendPower), 7);
						if(bHit == HT_NORMAL)
							dwValue = dwA + rand() % max(int(dwB - dwA), 1);
						else
							dwValue = dwB + DWORD(dwB * (GetFormulaRateX(FTYPE_PCD) + rand() % max(GetFormulaInit(FTYPE_PCD),1))/100);

						CalcAbilityValue(dwValue, MTYPE_DAMAGE);
						dwValue -= DistributeSkill(dwValue);

						if(nHP > (int)dwValue)
							nHP = nHP - dwValue;
						else
						{
							dwValue = nHP;
							nHP = 0;
						}

						nDamageHP += dwValue;
					}
					break;
				case MTYPE_MDAMAGE:
					{
						dwShildDefend = GetShieldDP();
						dwDefendPower = dwShildDefend + GetDefendPower();
						DWORD dwA = max(int(dwPhyMin - dwDefendPower), 5);
						DWORD dwB = max(int(dwPhyMax - dwDefendPower), 7);
						if(bHit == HT_NORMAL)
							dwValue = dwA + rand() % max(int(dwB - dwA), 1);
						else
							dwValue = dwA + DWORD(dwA * (GetFormulaRateX(FTYPE_PCD) + rand() % max(GetFormulaInit(FTYPE_PCD),1))/100);

						CalcAbilityValue(dwValue, MTYPE_MDAMAGE);
						if(nMP > (int)dwValue)
							nMP = nMP - dwValue;
						else
						{
							dwValue = nMP;
							nMP = 0;
						}

						nDamageMP += dwValue;
					}
					break;
				case MTYPE_HP:
					{
						dwValue = dwMaxHP;
						int nInc = CalcAbilityValue(dwValue, MTYPE_HP);
						if(nInc < 0)
						{
							nInc += DistributeSkill(abs(nInc));

							if(nHP > abs(nInc))
							{
								nHP = nHP + nInc;
								dwValue = abs(nInc);
							}
							else
							{
								dwValue = nHP;
								nHP = 0;
							}

							nDamageHP += dwValue;
						}
						else
						{
							if(nHP + nInc > (int)dwMaxHP)
							{
								nDamageHP -= dwMaxHP - nHP;
								nHP = dwMaxHP;
							}
							else
							{
								nDamageHP -= nInc;
								nHP += nInc;
							}
							dwValue = 0;
						}
					}
					break;
				case MTYPE_MP:
					{
						dwValue = dwMaxMP;
						int nInc = CalcAbilityValue(dwValue, MTYPE_MP);
						if(nInc < 0)
						{
							if(nMP > abs(nInc))
							{
								nMP = nMP + nInc;
								dwValue = abs(nInc);
							}
							else
							{
								dwValue = nMP;
								nMP = 0;
							}
							nDamageMP += dwValue;
						}
						else
						{
							if(nMP + nInc > (int)dwMaxMP)
							{
								nDamageMP -= dwMaxMP - nMP;
								nMP = dwMaxMP;
							}
							else
							{
								nDamageMP -= nInc;
								nMP += nInc;
							}
							dwValue = 0;
						}
					}
					break;
				}

				if(dwValue)
				{
					MAPWORD::iterator itDg = mapDamage.find(pData->m_bAttr);
					if(itDg == mapDamage.end())
                        mapDamage.insert(MAPWORD::value_type(pData->m_bExec, (WORD)dwValue));
					else
						(*itDg).second += (WORD)dwValue;
				}
			}
			break;
		default:
			{
				if(	(dwAttackID != m_dwID ||
					bAttackType != m_bType) &&
					HaveExcptMagic())
					break;

				switch(pData->m_bExec)
				{
				case MTYPE_DAMAGE:
					{
						dwShildDefend = GetShieldMDP();
						dwDefendPower = dwShildDefend + GetMagicDefPower();
						DWORD dwA = max(int(dwMagicMin - dwDefendPower), 5);
						DWORD dwB = max(int(dwMagicMax - dwDefendPower), 7);
						if(bHit == HT_NORMAL)
							dwValue = dwA + rand() % max(int(dwB - dwA), 1);
						else
							dwValue = dwA + DWORD(dwA * (GetFormulaRateX(FTYPE_MCD) + rand() % max(GetFormulaInit(FTYPE_MCD),1))/100);

						CalcAbilityValue(dwValue, MTYPE_DAMAGE);
						dwValue -= DistributeSkill(dwValue);

						if(nHP > (int)dwValue)
							nHP = nHP - dwValue;
						else
						{
							dwValue = nHP;
							nHP = 0;
						}

						nDamageHP += dwValue;
					}
					break;
				case MTYPE_MDAMAGE:
					{
						dwShildDefend = GetShieldMDP();
						dwDefendPower = dwShildDefend + GetMagicDefPower();
						DWORD dwA = max(int(dwMagicMin - dwDefendPower), 5);
						DWORD dwB = max(int(dwMagicMax - dwDefendPower), 7);
						if(bHit == HT_NORMAL)
							dwValue = dwA + rand() % max(int(dwB - dwA), 1);
						else
							dwValue = dwA + DWORD(dwA * (GetFormulaRateX(FTYPE_MCD) + rand() % max(GetFormulaInit(FTYPE_MCD),1))/100);

						CalcAbilityValue(dwValue, MTYPE_MDAMAGE);
						if(nMP > (int)dwValue)
							nMP = nMP - dwValue;
						else
						{
							dwValue = nMP;
							nMP = 0;
						}
						nDamageMP += dwValue;
					}
					break;
				case MTYPE_HP:
					{
						dwValue = dwMaxHP;
						int nInc = CalcAbilityValue(dwValue, MTYPE_HP);
						if(nInc < 0)
						{
							nInc += DistributeSkill(abs(nInc));

							if(nHP > abs(nInc))
							{
								nHP = nHP + nInc;
								dwValue = abs(nInc);
							}
							else
							{
								dwValue = nHP;
								nHP = 0;
							}

							nDamageHP += dwValue;
						}
						else
						{
							if(nHP + nInc > (int)dwMaxHP)
							{
								nDamageHP -= dwMaxHP - nHP;
								nHP = dwMaxHP;
							}
							else
							{
								nDamageHP -= nInc;
								nHP += nInc;
							}
							dwValue = 0;
						}
					}
					break;
				case MTYPE_MP:
					{
						dwValue = dwMaxMP;
						int nInc = CalcAbilityValue(dwMaxMP, MTYPE_MP);
						if(nInc < 0)
						{
							if(nMP > abs(nInc))
							{
								nMP = nMP + nInc;
								dwValue = abs(nInc);
							}
							else
							{
								dwValue = nMP;
								nMP = 0;
							}
							nDamageMP += dwValue;
						}
						else
						{
							if(nMP + nInc > (int)dwMaxMP)
							{
								nDamageMP -= dwMaxMP - nMP;
								nMP = dwMaxMP;
							}
							else
							{
								nDamageMP -= nInc;
								nMP += nInc;
							}
							dwValue = 0;
						}
					}
					break;
				}

				if(dwValue)
				{
					MAPWORD::iterator itDg = mapDamage.find(pData->m_bAttr);
					if(itDg == mapDamage.end())
                        mapDamage.insert(MAPWORD::value_type(pData->m_bExec, (WORD)dwValue));
					else
						(*itDg).second += (WORD)dwValue;
				}
			}
			break;
		}
	}

	if(dwShildDefend)
		bHit = HT_BLOCK;
}

DWORD CTObjBase::OnDamage(DWORD dwHostID,
						  DWORD dwAttackID,
						  BYTE bAttackType,
						  BYTE bAttackCountry,
						  BYTE bAttackAidCountry,
						  BYTE bAtkCanSelect,
						  WORD wPartyID,
						  int nDamageHP,
						  int nDamageMP)
{
	if(!m_pMAP || m_bStatus == OS_DEAD)
		return 0;

	DWORD dwHP = m_dwHP;

	if(nDamageHP > 0)
		m_dwHP = (int)m_dwHP > nDamageHP ? m_dwHP - nDamageHP : 0;
	else
	{
		DWORD dwMaxHP = GetMaxHP(); 
		m_dwHP = dwMaxHP > m_dwHP - nDamageHP ? m_dwHP - nDamageHP : dwMaxHP;
	}
	if(nDamageMP > 0)
		m_dwMP = (int)m_dwMP > nDamageMP ? m_dwMP - nDamageMP : 0;
	else
	{
		DWORD dwMaxMP = GetMaxMP(); 
		m_dwMP = dwMaxMP > m_dwMP - nDamageMP ? m_dwMP - nDamageMP : dwMaxMP;
	}

	if(m_bType == OT_PC && nDamageHP > 0 && m_dwHP )
	{
		CTPlayer * pMe = (CTPlayer*)this;
		pMe->CheckSoulHP();
		if(bAttackType != OT_MON && dwHostID != m_dwID && CanFight(bAttackType, bAttackCountry, bAttackAidCountry))
		{
			pMe->m_dwLastAttackCharID = dwHostID;
			pMe->m_dwLastAttackTick = _AtlModule.m_dwTick;
		}
	}

	return dwHP > m_dwHP ? dwHP - m_dwHP : 0;
}

void CTObjBase::Defend( CTSkillTemp * pTemp,
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
						WORD wAL,
						BYTE bAttackerLevel,
						BYTE bCP,
						BYTE bAtkHit,
						DWORD dwRemainTick,
						FLOAT fAtkPosX,
						FLOAT fAtkPosY,
						FLOAT fAtkPosZ,
						FLOAT fDefPosX,
						FLOAT fDefPosY,
						FLOAT fDefPosZ,
						BYTE bMirror)
{
	if(pTemp->IsNegative() && FindMaintainSkill(TREVIVAL_SKILL))
		bAtkHit = HT_MISS;

	DWORD dwMaxHP = GetMaxHP();
	DWORD dwMaxMP = GetMaxMP();
	DWORD dwHP = m_dwHP;
	DWORD dwMP = m_dwMP;

	m_pInstanceSkill = new CTSkill();
	m_pInstanceSkill->m_pTSKILL = pTemp;
	m_pInstanceSkill->m_bLevel = bSkillLevel;

	MAPWORD mapDamage;
	mapDamage.clear();

	BYTE bHPMP = FALSE;
	BYTE bCancelCharge = FALSE;

	CTRecallMon * pRecallMon = NULL;
	CTSkill * pMaintain = NULL;
	CTSkill * pRemain = NULL;
	int nDamageHP = 0;
	int nDamageMP = 0;
	WORD wAutoSkill = 0;

	if(bAtkHit != HT_MISS &&
		m_pInstanceSkill->m_pTSKILL->IsDamageType())
	{
		CalcDamage(
			dwAttackID,
			bAttackType,
			mapDamage,
			nDamageHP,
			nDamageMP,
			dwPysMinPower,
			dwPysMaxPower,
			dwMgMinPower,
			dwMgMaxPower,
			bAtkHit,
			m_pInstanceSkill,
			SDT_ABILITY);

		if(m_bType == OT_PC)
		{
			if(bAtkHit == HT_BLOCK)
				wAutoSkill = ((CTPlayer *)this)->DurationDec(4, m_pInstanceSkill->m_pTSKILL->IsItemDelTarget());
			else if(nDamageHP > 0)
				wAutoSkill = ((CTPlayer *)this)->DurationDec(3, m_pInstanceSkill->m_pTSKILL->IsItemDelTarget());

			if(!CanFight(bAttackType, bAttackCountry, bAttackAidCountry))
				wAutoSkill = 0;
		}

		if(wAutoSkill)
		{
			CTSkillTemp * pAuto = _AtlModule.FindTSkill(wAutoSkill);
			if(pAuto)
			{
				if(nDamageHP > 0)
				{
					if(pAuto->IsDamageType())
					{
						nDamageHP += pAuto->CalcValue(bSkillLevel, SDT_ABILITY, MTYPE_DAMAGE, nDamageHP);
						nDamageHP += pAuto->CalcValue(bSkillLevel, SDT_ABILITY, MTYPE_HP, nDamageHP);
						wAutoSkill = 0;
					}
					else if(pAuto->IsMirrorType())
					{
						_AtlModule.SendMW_GETBLOOD_ACK(dwAttackID, bAttackType, dwHostID, MTYPE_DAMAGE, nDamageHP);
						nDamageHP = 0;
						wAutoSkill = 0;
					}
				}
			}
		}

		OnDamage(
			dwHostID,
			dwAttackID,
			bAttackType,
			bAttackCountry,
			bAttackAidCountry,
			bCanSelect,
			wAttackPartyID,
			nDamageHP,
			nDamageMP);

		CTSkill *pCurCharge = FindTSkill(m_wCurChargeSkill);
		if(pCurCharge &&
			pCurCharge->m_dwChargeTick &&
			pCurCharge->m_pTSKILL->m_dwActionTime + pCurCharge->m_dwChargeTick > _AtlModule.m_dwTick)
		{
			if(rand() % 100 > GetChargeProb()) 
				bCancelCharge = TRUE;
		}
	}

	BYTE bPerform = PERFORM_SUCCESS;
	if(bAtkHit != HT_MISS && !bMirror)
	{
		pMaintain = MaintainSkill(
			m_pInstanceSkill,
			dwRemainTick,
			dwHostID,
			OT_PC,
			dwAttackID,
			bAttackType,
			dwPysMinPower,
			dwPysMaxPower,
			dwMgMinPower,
			dwMgMaxPower,
			bCurseProb,
			bCanSelect,
			GetAttackCountry(bAttackCountry, bAttackAidCountry),
			bCP,
			wAL,
			bAttackerLevel,
			fAtkPosX,
			fAtkPosY,
			fAtkPosZ);

		pRemain = RemainSkill(m_pInstanceSkill, dwAttackID);

		if((!pMaintain && m_pInstanceSkill->m_pTSKILL->IsMaintainType()) ||
			(!pRemain && m_pInstanceSkill->m_pTSKILL->IsRemainType()))
			bPerform = PERFORM_MISS;
		else
			bPerform = PerformSkill(
				dwAttackID,
				bAttackType,
				bAttackCountry,
				bAttackAidCountry,
				bAttackClass,
				dwHostID,
				pTemp,
				bSkillLevel,
				bCP,
				dwPysMinPower,
				dwPysMaxPower,
				dwMgMinPower,
				dwMgMaxPower,
				wTransHP,
				wTransMP,
				bCurseProb,
				wAL,
				bAttackerLevel,
				nDamageHP,
				nDamageMP,
				fAtkPosX,
				fAtkPosY,
				fAtkPosZ,
				&pRecallMon);
	}
	else
		bPerform = PERFORM_MISS;

	DWORD dwNewMaxHP = GetMaxHP();
	DWORD dwNewMaxMP = GetMaxMP();
	if(dwNewMaxHP < m_dwHP)
		m_dwHP = dwNewMaxHP;
	if(dwNewMaxMP < m_dwMP)
		m_dwMP = dwNewMaxMP;

	bHPMP = (m_dwHP == dwHP && m_dwMP == dwMP && dwNewMaxHP == dwMaxHP && dwNewMaxMP == dwMaxMP) ? FALSE : TRUE;

	if(m_bType == OT_PC)
	{
		CTPlayer * pPlayer = (CTPlayer *)this;

		if(pTemp->m_bIsDismount && bAtkHit != HT_MISS)
		{
			pPlayer->StoreClose();
			if(pPlayer->m_dwRiding)
			{
				pPlayer->PetRiding(0);
				pPlayer->ForceMaintain(
					TBLOCKRIDE_SKILL,
					m_dwID,
					m_bType,
					m_dwID,
					m_bType,
					0);
			}
		}
		else if(pTemp->IsNegative())
		{
			pPlayer->StoreClose();
			if(pPlayer->m_dwRiding && bAtkHit == HT_CRITICAL)
			{
				pPlayer->PetRiding(0);
				pPlayer->ForceMaintain(
					TBLOCKRIDE_SKILL,
					m_dwID,
					m_bType,
					m_dwID,
					m_bType,
					0);
			}
		}
	}

	if((m_bType != bAttackType || m_dwID != dwAttackID) &&
		m_pInstanceSkill->m_pTSKILL->IsNegative())
		EraseBuffByDefend(pTemp);

	CheckEternalBuff();

	if(pRemain)
		m_vRemainSkill.push_back(pRemain);

	BYTE bIsMaintain = FALSE;
	DWORD dwMaintainTick = 0;
	if(PushMaintainSkill(pMaintain))
	{
		bIsMaintain = TRUE;
		dwMaintainTick = dwRemainTick ? dwRemainTick : pMaintain->GetMaintainTick();
	}

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	if(bIsMaintain && 
		pMaintain &&
		pMaintain->m_pTSKILL->m_bORadius &&
		(m_dwID != dwAttackID || m_bType != bAttackType))
	{
		CTPlayer *pChar = NULL;
		if(m_bType == OT_PC)
			pChar = (CTPlayer *)this;
		else
		{
			CTMonster * pMon = (CTMonster *)this;
			pChar = pMon->FindHost(pMon->m_dwHostID);
		}

		if(pChar)
			vPLAYERS.push_back(pChar);
	}
	else
	{
		if(bIsMaintain)
			m_pMAP->GetNeighbor(
				&vPLAYERS,
				m_fPosX,
				m_fPosZ);
		else
			m_pMAP->GetNeerPlayer(
				&vPLAYERS,
				m_fPosX,
				m_fPosZ);
	}

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_DEFEND_ACK(
			dwAttackID,
			pRecallMon ? pRecallMon->m_dwID : m_dwID,
			bAttackType,
			pRecallMon ? pRecallMon->m_bType : m_bType,
			dwHostID,
			OT_PC,
			dwActID,
			dwAniID,
			bIsMaintain,
			dwMaintainTick,
			bCP,
			(m_dwHP ? bAtkHit : HT_LASTHIT),
			wAL,
			bAttackerLevel,
			dwPysMinPower,
			dwPysMaxPower,
			dwMgMinPower,
			dwMgMaxPower,
			bCanSelect,
			bCancelCharge,
			bAttackCountry,
			bAttackAidCountry,
			m_pInstanceSkill->m_pTSKILL->m_wID,
			m_pInstanceSkill->m_bLevel,
			wAutoSkill,
			(bPerform == PERFORM_SUCCESS ? TRUE : FALSE),
			fAtkPosX,fAtkPosY,fAtkPosZ,
			fDefPosX,fDefPosY,fDefPosZ,
			&mapDamage);

		if(bHPMP)
			pChar->SendCS_HPMP_ACK(
				m_dwID,
				m_bType,
				m_bLevel,
				dwNewMaxHP,
				m_dwHP,
				dwNewMaxMP,
				(m_dwHP ? m_dwMP : 0));

		vPLAYERS.pop_back();
	}

	vPLAYERS.clear();

	delete m_pInstanceSkill;
	m_pInstanceSkill = NULL;

	if( bAttackType != OT_MON &&
		bAtkHit != HT_MISS &&
		bHPMP)
	{
		VTMONSTER vMONS;
		vMONS.clear();

		m_pMAP->GetNeighbor(&vMONS, m_fPosX, m_fPosZ);

		while(!vMONS.empty())
		{
			CTMonster * pAGGROMON = vMONS.back();
			vMONS.pop_back();

			if(GetDistance(fDefPosX, fDefPosZ, pAGGROMON->m_fPosX, pAGGROMON->m_fPosZ) > 30)
				continue;

			DWORD dwAggro = 0;
			BYTE bForce;
			if(m_bType == OT_MON && m_dwID == pAGGROMON->m_dwID)
			{
				dwAggro = nDamageHP + nDamageMP;
				bForce = TRUE;
			}
			else if(!pTemp->m_bRadius)
			{
				dwAggro = (nDamageHP + nDamageMP) / 10;
				bForce = FALSE;
			}
			else
				continue;

			pAGGROMON->SetAggro(
				dwHostID,
				bCanSelect ? dwAttackID : dwHostID,
				bCanSelect ? bAttackType : OT_PC,
				GetAttackCountry(bAttackCountry, bAttackAidCountry),
				bAttackClass,
				m_dwID,
				m_bType,
				dwAggro,
				bForce);
		}
	}

	if(m_bType == OT_PC && (pMaintain || pRemain ))
		((CTPlayer *)this)->SendCS_CHARSTATINFO_ACK((CTPlayer *)this);

	if(bAtkHit == HT_MISS)
		ForceMaintain(TAVOID_SKILL, dwHostID, OT_PC, dwAttackID, bAttackType, 0);
	else if(bAtkHit == HT_BLOCK)
		ForceMaintain(TBLOCK_SKILL, dwHostID, OT_PC, dwAttackID, bAttackType, 0);

	if(!m_dwHP && m_bStatus != OS_DEAD)
	{
		BYTE bDuelResult = 0;
		if( m_bType == OT_PC && ((CTPlayer *)this)->m_dwDuelID)
			bDuelResult = ((CTPlayer *)this)->DuelLose(dwHostID);
		
		if(!bDuelResult)
		{
			WORD wTempMonID = 0;

			switch(bAttackType)
			{			
			case OT_MON:
				{				
				CTMonster* pMon = m_pMAP->FindMonster(dwAttackID);				
				if(pMon)
                    wTempMonID = pMon->m_pMON->m_wID;
                
				OnDie(dwHostID,bAttackType, wTempMonID);
				}
				break;
			default:
				OnDie(dwHostID,bAttackType,wTempMonID);
			}
		}

		if(m_bType == OT_RECALL)
		{
			CTRecallMon * pRecall = (CTRecallMon *)this;
			CTPlayer * pOwner = pRecall->FindHost(pRecall->m_dwHostID);
			if(pOwner)
				_AtlModule.SendMW_RECALLMONDEL_ACK(pOwner->m_dwID, pOwner->m_dwKEY, pRecall->m_dwID);
		}
		else if(m_bType == OT_SELF)
		{
			CTSelfObj * pSelf = (CTSelfObj *)this;
			CTPlayer * pOwner = pSelf->FindHost(pSelf->m_dwHostID);
			if(pOwner)
				pOwner->DeleteSelfMon(m_dwID);
		}
	}
	mapDamage.clear();
}

DWORD CTObjBase::GetFormulaInit(BYTE bFormula)
{
	return _AtlModule.GetFormulaInit(bFormula);
}

FLOAT CTObjBase::GetFormulaRateX(BYTE bFormula)
{
	return _AtlModule.GetFormulaRateX(bFormula);
}

FLOAT CTObjBase::GetFormulaRateY(BYTE bFormula)
{
	return _AtlModule.GetFormulaRateY(bFormula);
}

void CTObjBase::Recover( DWORD dwTick)
{
	if(!m_pMAP || m_bStatus == OS_DEAD)
		return;

	if(HaveStopRecover())
		return;

	BYTE bChange = FALSE;
	BYTE bKeepChange = FALSE;

	if(	m_bMode == MT_NORMAL &&
		dwTick >= m_dwRecoverHPTick + RECOVER_TIME )
	{
		DWORD dwMaxHP = GetMaxHP();

		if(m_dwHP < dwMaxHP)
		{
			bChange = TRUE;
			m_dwHP += GetHPR();

			if(m_dwHP > dwMaxHP)
				m_dwHP = dwMaxHP;

			m_dwRecoverHPTick = dwTick;
		}
	}

	if(dwTick >= m_dwRecoverMPTick + RECOVER_TIME)
	{
		DWORD dwMaxMP = GetMaxMP();

		if(m_dwMP < dwMaxMP)
		{
			bChange = TRUE;
			m_dwMP += GetMPR();

			if(m_dwMP > dwMaxMP)
				m_dwMP = dwMaxMP;

			m_dwRecoverMPTick = dwTick;
		}
	}

	if(bChange)
	{
		DWORD dwMaxHP = GetMaxHP();
		DWORD dwMaxMP = GetMaxMP();

		VPLAYER vPLAYERS;
		vPLAYERS.clear();

		m_pMAP->GetNeighbor(
			&vPLAYERS,
			m_fPosX,
			m_fPosZ);

		while(!vPLAYERS.empty())
		{
			CTPlayer *pChar = vPLAYERS.back();
			pChar->SendCS_HPMP_ACK(
				m_dwID,
				m_bType,
				m_bLevel,
				dwMaxHP,
				m_dwHP,
				dwMaxMP,
				m_dwMP);

			if(bKeepChange)
			{
				CTMonster * pMon = (CTMonster *)this;
				pChar->SendCS_CHANGECOLOR_ACK(
					m_dwID,
					OT_MON,
					pMon->GetColor( pChar->m_dwID, pChar->GetPartyID(), pChar->GetWarCountry()),
					pMon->GetWarCountry());
			}

			vPLAYERS.pop_back();
		}

		vPLAYERS.clear();
	}
}

int CTObjBase::CalcAbilityValue( DWORD &dwValue, BYTE bExec, CTSkill * pInstanceSkill)
{
	int nIncreaseValue = 0;
	int nInstInc = 0;
	int nCureInc = 0;

	if(pInstanceSkill)
		nInstInc += pInstanceSkill->m_pTSKILL->CalcValue(pInstanceSkill->m_bLevel, SDT_ABILITY, bExec, dwValue);
	else if(m_pInstanceSkill)
		nInstInc += m_pInstanceSkill->m_pTSKILL->CalcValue(m_pInstanceSkill->m_bLevel, SDT_ABILITY, bExec, dwValue);

	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		nIncreaseValue += m_vMaintainSkill[i]->CalcAbilityValue(SA_BUFF, dwValue, bExec);
		nCureInc += m_vMaintainSkill[i]->CalcCure(m_pInstanceSkill, dwValue, bExec);
	}

	if((nInstInc >= 0 && nCureInc + nInstInc < 0) ||
		(nInstInc <= 0 && nCureInc + nInstInc > 0))
		nInstInc = 0;
	else
		nInstInc += nCureInc;

	nIncreaseValue += nInstInc;
	DWORD dwCalcValue = max(0, int(dwValue) + nIncreaseValue);

	if(nIncreaseValue)
		nIncreaseValue += ApplyEffectionBuff(dwCalcValue); //마법효과 증폭

	int nInc = 0;
	for(size_t i=0; i < m_vRemainSkill.size(); i++)
	{
		nInc += m_vRemainSkill[i]->CalcAbilityValue(SA_CONTINUE, dwValue, bExec);
		nInc += m_vRemainSkill[i]->CalcAbilityValue(SA_PASSIVE, dwValue, bExec);
	}

	dwValue = max(0, int(dwCalcValue) + nInc);
	return nIncreaseValue + nInc;
}

FLOAT CTObjBase::GetBaseStatValue( BYTE bType, WORD wBaseMIN)
{
	WORD wBase = BASE_STAT;

	switch(bType)
	{
	case MTYPE_STR: wBase += m_pTRACE->m_wSTR + m_pTCLASS->m_wSTR; break;
	case MTYPE_DEX: wBase += m_pTRACE->m_wDEX + m_pTCLASS->m_wDEX; break;
	case MTYPE_CON: wBase += m_pTRACE->m_wCON + m_pTCLASS->m_wCON; break;
	case MTYPE_INT: wBase += m_pTRACE->m_wINT + m_pTCLASS->m_wINT; break;
	case MTYPE_WIS: wBase += m_pTRACE->m_wWIS + m_pTCLASS->m_wWIS; break;
	case MTYPE_MEN: wBase += m_pTRACE->m_wMEN + m_pTCLASS->m_wMEN; break;
	}
	FLOAT fValue = max( wBaseMIN, wBase) * pow(GetFormulaRateX(FTYPE_1ST), m_bLevel-1);

	return fValue;
}

DWORD CTObjBase::GetAbility(BYTE bType)
{
	switch(bType)
	{
	case MTYPE_STR:
		return DWORD(GetSTR());
	case MTYPE_DEX:
		return DWORD(GetDEX());
	case MTYPE_CON:
		return DWORD(GetCON());
	case MTYPE_INT:
		return DWORD(GetINT());
	case MTYPE_WIS:
		return DWORD(GetWIS());
	case MTYPE_MEN:
		return DWORD(GetMEN());
	case MTYPE_PAP:
		return max(GetMaxAP(TRUE), GetMaxAP(FALSE));
	case MTYPE_PDP:
		return GetDefendPower();
	case MTYPE_LAP:
		return max(GetMaxAP(TRUE), GetMaxAP(FALSE));
	case MTYPE_AL:
		return GetAttackLevel();
	case MTYPE_DL:
		return GetDefendLevel();
	case MTYPE_CR:
		return GetCriticalPysProb();
	case MTYPE_MDP:
		return GetMagicDefPower();
	case MTYPE_MAP:
		return GetMaxMagicAP();
	case MTYPE_MCS:
		return GetChargeSpeed();
	case MTYPE_CMP:
		return GetChargeProb();
	case MTYPE_MCR:
		return GetCriticalMagicProb();
	case MTYPE_AVP:
		return GetAvoidProb();
	case MTYPE_HPR:
		return GetHPR();
	case MTYPE_MPR:
		return GetMPR();
	case MTYPE_MHP:
		return GetMaxHP();
	case MTYPE_MMP:
		return GetMaxMP();
	case MTYPE_PAS:
		return GetAtkSpeedRate(TAD_PHYSICAL);
	case MTYPE_LAS:
		return GetAtkSpeedRate(TAD_LONG);
	case MTYPE_MPAS:
		return GetAtkSpeedRate(TAD_MAGIC);
	case MTYPE_SPDPOW:
		return GetShieldDP();
	case MTYPE_SMDPOW:
		return GetShieldMDP();
	case MTYPE_PMINAP:
		return GetMinAP(FALSE);
	case MTYPE_PMAXAP:
		return GetMaxAP(FALSE);
	case MTYPE_LMINAP:
		return GetMinAP(TRUE);
	case MTYPE_LMAXAP:
		return GetMaxAP(TRUE);
	case MTYPE_MPMINAP:
		return GetMinMagicAP();
	case MTYPE_MPMAXAP:
		return GetMaxMagicAP();
	case MTYPE_MAL:
		return GetMagicAtkLevel();
	case MTYPE_MDL:
		return GetMagicDefLevel();
	default:
		return 100;
	}
}

DWORD CTObjBase::Calc2ndAbility(BYTE bFormula)
{
	DWORD dwValue = 0;
	DWORD dwInit = GetFormulaInit(bFormula);
	FLOAT fRateX = GetFormulaRateX(bFormula);
	FLOAT fRateY = GetFormulaRateY(bFormula);
	switch(bFormula)
	{
	case FTYPE_PDP:
		dwValue = dwInit + DWORD(pow(fRateY,m_bLevel) * fRateX);
		break;
	case FTYPE_PAP:
		dwValue = dwInit + DWORD(GetSTR() * fRateX);
		break;
	case FTYPE_LAP:
	case FTYPE_AL:
		dwValue = dwInit + DWORD(GetDEX() * fRateX);
		break;
	case FTYPE_DL:
		dwValue = DWORD(GetDEX( FALSE, WORD(fRateY)) * fRateX);
		break;
	case FTYPE_NAS:
		dwValue = DWORD(min(dwInit, max(fRateY - (BASE_STAT + m_pTRACE->m_wDEX + m_pTCLASS->m_wDEX) * fRateX, 0)));
		break;
	case FTYPE_PCR:
		dwValue = dwInit + DWORD((BASE_STAT + m_pTRACE->m_wDEX + m_pTCLASS->m_wDEX) * fRateX);
		break;
	case FTYPE_HP:
	case FTYPE_HPR:
		dwValue = dwInit + DWORD(GetCON() * fRateX);
		break;
	case FTYPE_MDP:
		dwValue = dwInit + DWORD(pow(fRateY,m_bLevel) * fRateX);
		break;
	case FTYPE_MAP:
		dwValue = dwInit + DWORD(GetINT() * fRateX);
		break;
	case FTYPE_MNAS:
		dwValue = DWORD(min(dwInit, max(fRateY - (BASE_STAT + m_pTRACE->m_wWIS + m_pTCLASS->m_wWIS) * fRateX, 0)));
		break;
	case FTYPE_MCR:
		dwValue = dwInit + DWORD((BASE_STAT + m_pTRACE->m_wWIS + m_pTCLASS->m_wWIS) * fRateX);
		break;
	case FTYPE_MAL:
		dwValue = dwInit + DWORD(GetWIS() * fRateX);
		break;
	case FTYPE_MDL:
		dwValue = DWORD(GetWIS( FALSE, WORD(fRateY)) * fRateX);
		break;
	case FTYPE_MP:
	case FTYPE_MPR:
		dwValue = dwInit + DWORD(GetMEN() * fRateX);
		break;
	case FTYPE_MSP:
		dwValue = DWORD((BASE_STAT + m_pTRACE->m_wMEN + m_pTCLASS->m_wMEN) * fRateX - fRateY);
		break;
	case FTYPE_MCS:
		dwValue = dwInit;
		break;
	}
	return dwValue;
}

int CTObjBase::CalcItemAbility(BYTE bType)
{
	int nValue = 0;

	if(HaveDisguiseBuff())
		return nValue;

	CTInven *pTEQUIP = FindTInven(INVEN_EQUIP);
	if(pTEQUIP)
	{
		MAPTITEM::iterator itTITEM;

		for( itTITEM = pTEQUIP->m_mapTITEM.begin(); itTITEM != pTEQUIP->m_mapTITEM.end(); itTITEM++)
		{
			if(!(*itTITEM).second->HavePower())
				continue;

			if(HaveDisWeapon() && (*itTITEM).second->m_pTITEM->m_bType != IT_SHIELD &&
				((*itTITEM).second->m_bItemID == ES_PRMWEAPON ||
				(*itTITEM).second->m_bItemID == ES_SNDWEAPON ||
				(*itTITEM).second->m_bItemID == ES_LONGWEAPON))
				continue;

			if(HaveDisDefend() &&
				((*itTITEM).second->m_bItemID == ES_HEAD ||
				(*itTITEM).second->m_bItemID == ES_BODY ||
				(*itTITEM).second->m_bItemID == ES_PANTS ||
				(*itTITEM).second->m_bItemID == ES_FOOT ||
				(*itTITEM).second->m_bItemID == ES_HAND))
				continue;

			switch(bType)
			{
			case ABILITY_STR:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_STR);
				break;
			case ABILITY_DEX:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_DEX);
				break;
			case ABILITY_CON:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_CON);
				break;
			case ABILITY_INT:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_INT);
				break;
			case ABILITY_WIS:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_WIS);
				break;
			case ABILITY_MEN:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_MEN);
				break;
			case ABILITY_MAXAP :
				nValue += (*itTITEM).second->GetMaxAP();
				break;
			case ABILITY_MAXLAP:
				nValue += (*itTITEM).second->GetMaxLAP();
				break;
			case ABILITY_MINAP:
				nValue += (*itTITEM).second->GetMinAP();
				break;
			case ABILITY_MINLAP:
				nValue += (*itTITEM).second->GetMinLAP();
				break;
			case ABILITY_MAXMAP:
				nValue += (*itTITEM).second->GetMaxMagicAP();
				break;
			case ABILITY_MINMAP:
				nValue += (*itTITEM).second->GetMinMagicAP();
				break;
			case ABILITY_MAXHP:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_MHP);
				break;
			case ABILITY_MAXMP:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_MMP);
				break;
			case ABILITY_PDP:
				nValue += (*itTITEM).second->GetDefendPower();
				break;
			case ABILITY_MDP:
				nValue += (*itTITEM).second->GetMagicDefPower();
				break;
			case ABILITY_AL:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_AL);
				break;
			case ABILITY_DL:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_DL);
				break;
			case ABILITY_MAL:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_MAL);
				break;
			case ABILITY_MDL:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_MDL);
				break;
			case ABILITY_PCR:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_CR);
				break;
			case ABILITY_MCR:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_MCR);
				break;
			case ABILITY_CMP:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_CMP);
				break;
			case ABILITY_SDR:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_SDR);
				if((*itTITEM).second->m_pTITEM->m_bType == IT_SHIELD &&
					(*itTITEM).second->m_pTITEM->m_bKind == IK_SHIELD)
					nValue += (*itTITEM).second->m_pTITEMATTR->m_bBlockProb;
				break;
			case ABILITY_SDP:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_SPDPOW);
				if((*itTITEM).second->m_pTITEM->m_bType == IT_SHIELD &&
					(*itTITEM).second->m_pTITEM->m_bKind == IK_SHIELD)
					nValue += (*itTITEM).second->m_pTITEMATTR->m_wDP;
				break;
			case ABILITY_SMDR:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_SMDR);
				if((*itTITEM).second->m_pTITEM->m_bType == IT_SHIELD &&
					(*itTITEM).second->m_pTITEM->m_bKind == IK_MULTIVAJRA)
					nValue += (*itTITEM).second->m_pTITEMATTR->m_bBlockProb;
				break;
			case ABILITY_SMDP:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_SMDPOW);
				if((*itTITEM).second->m_pTITEM->m_bType == IT_SHIELD &&
					(*itTITEM).second->m_pTITEM->m_bKind == IK_MULTIVAJRA)
					nValue += (*itTITEM).second->m_pTITEMATTR->m_wMDP;
				break;
			case ABILITY_MCS:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_MCS);
				break;
			case ABILITY_HPR:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_HPR);
				break;
			case ABILITY_MPR:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_MPR);
				break;
			case ABILITY_AVP:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_AVP);
				break;
			case ABILITY_ATKSPEED_S:
				if((*itTITEM).second->m_bItemID == ES_PRMWEAPON ||
					(*itTITEM).second->m_bItemID == ES_SNDWEAPON)
					nValue += (*itTITEM).second->m_pTITEM->m_dwSpeedInc;
				break;
			case ABILITY_ATKSPEED_L:
				if((*itTITEM).second->m_bItemID == ES_LONGWEAPON)
                    nValue += (*itTITEM).second->m_pTITEM->m_dwSpeedInc;
				break;
			case ABILITY_ATKSPEED_SR:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_PAS);
				break;
			case ABILITY_ATKSPEED_LR:
                nValue += (*itTITEM).second->GetMagicValue(MTYPE_LAS);
				break;
			case ABILITY_ATKSPEED_MR:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_MPAS);
				break;
			case ABILITY_AFTERMATH:
				nValue += (*itTITEM).second->GetMagicValue(MTYPE_AFTERMATH);
				break;
			}
		}
	}

	return nValue;
}

FLOAT CTObjBase::GetSTR( BYTE bPure, WORD wBaseMIN)
{
	FLOAT fSTR = GetBaseStatValue( MTYPE_STR, wBaseMIN);
	if(!bPure)
	{
		CalcAfterMath(fSTR);
		fSTR += CalcItemAbility(ABILITY_STR);
		DWORD dwSTR = (DWORD)fSTR;
		fSTR += CalcAbilityValue(dwSTR, MTYPE_STR);
	}
	return fSTR;
}

FLOAT CTObjBase::GetDEX( BYTE bPure, WORD wBaseMIN)
{
	FLOAT fDEX = GetBaseStatValue( MTYPE_DEX, wBaseMIN);
	if(!bPure)
	{
		CalcAfterMath(fDEX);
		fDEX += CalcItemAbility(ABILITY_DEX);
		DWORD dwDEX = (DWORD)fDEX;
		fDEX += CalcAbilityValue(dwDEX, MTYPE_DEX);
	}
	return fDEX;
}

FLOAT CTObjBase::GetCON( BYTE bPure, WORD wBaseMIN)
{
	FLOAT fCON = GetBaseStatValue( MTYPE_CON, wBaseMIN);
	if(!bPure)
	{
		CalcAfterMath(fCON);
		fCON += CalcItemAbility(ABILITY_CON);
		DWORD dwCON = (DWORD)fCON;
		fCON += CalcAbilityValue(dwCON, MTYPE_CON);
	}
	return fCON;
}

FLOAT CTObjBase::GetINT( BYTE bPure, WORD wBaseMIN)
{
	FLOAT fINT = GetBaseStatValue( MTYPE_INT, wBaseMIN);
	if(!bPure)
	{
		CalcAfterMath(fINT);
		fINT += CalcItemAbility(ABILITY_INT);
		DWORD dwINT = (DWORD)fINT;
		fINT += CalcAbilityValue(dwINT, MTYPE_INT);
	}
	return fINT;
}

FLOAT CTObjBase::GetWIS( BYTE bPure, WORD wBaseMIN)
{
	FLOAT fWIS = GetBaseStatValue( MTYPE_WIS, wBaseMIN);
	if(!bPure)
	{
		CalcAfterMath(fWIS);
		fWIS += CalcItemAbility(ABILITY_WIS);
		DWORD dwWIS = (DWORD)fWIS;
		fWIS += CalcAbilityValue(dwWIS, MTYPE_WIS);
	}
	return fWIS;
}

FLOAT CTObjBase::GetMEN( BYTE bPure, WORD wBaseMIN)
{
	FLOAT fMEN = GetBaseStatValue( MTYPE_MEN, wBaseMIN);
	if(!bPure)
	{
		CalcAfterMath(fMEN);
		fMEN += CalcItemAbility(ABILITY_MEN);
		DWORD dwMEN = (DWORD)fMEN;
		fMEN += CalcAbilityValue(dwMEN, MTYPE_MEN);
	}
	return fMEN;
}

DWORD CTObjBase::GetAtkSpeed(BYTE bType)
{
	int nAS = 0;

	switch(bType)
	{
	case TAD_PHYSICAL:
		nAS = Calc2ndAbility(FTYPE_NAS);
		nAS += CalcItemAbility(ABILITY_ATKSPEED_S);
		break;
	case TAD_LONG:
		nAS = Calc2ndAbility(FTYPE_NAS);
		nAS += CalcItemAbility(ABILITY_ATKSPEED_L);
		break;
	case TAD_MAGIC:
		nAS = Calc2ndAbility(FTYPE_MNAS);
		nAS += CalcItemAbility(ABILITY_ATKSPEED_S);
		break;
	default:
		break;
	}

	return DWORD(max(nAS, 0));
}

DWORD CTObjBase::GetAtkSpeedRate( BYTE bType)
{
	DWORD dwAS = TMAGIC_OPT_SPEED_RATE;
	DWORD dwIas = 0;
	switch(bType)
	{
	case TAD_PHYSICAL:
		CalcAbilityValue(dwAS, MTYPE_PAS);
		dwIas = min(CalcItemAbility(ABILITY_ATKSPEED_SR),100);
		if(dwIas)
			dwAS = dwAS * (100-dwIas) / 100;
		break;
	case TAD_LONG:
		CalcAbilityValue(dwAS, MTYPE_LAS);
		dwIas = min(CalcItemAbility(ABILITY_ATKSPEED_LR),100);
		if(dwIas)
			dwAS = dwAS * (100-dwIas) / 100;
		break;
	case TAD_MAGIC:
		CalcAbilityValue(dwAS, MTYPE_MPAS);
		dwIas = min(CalcItemAbility(ABILITY_ATKSPEED_MR),100);
		if(dwIas)
			dwAS = dwAS * (100-dwIas) / 100;
		break;
	default:
		break;
	}

	return DWORD(max(dwAS, 0));
}

DWORD CTObjBase::GetMaxAP(BYTE bArrow)
{
	DWORD dwMaxAP;

	if(bArrow)
	{
		dwMaxAP = Calc2ndAbility(FTYPE_LAP);
		dwMaxAP += CalcItemAbility(ABILITY_MAXLAP);
		CalcAbilityValue(dwMaxAP, MTYPE_LAP);
	}
	else
	{
		dwMaxAP = Calc2ndAbility(FTYPE_PAP);
		dwMaxAP += CalcItemAbility(ABILITY_MAXAP);
		CalcAbilityValue(dwMaxAP, MTYPE_PAP);
	}

	return dwMaxAP;
}

DWORD CTObjBase::GetMinAP(BYTE bArrow)
{
	DWORD dwMinAP;

	if(bArrow)
	{
		dwMinAP = Calc2ndAbility(FTYPE_LAP);
		dwMinAP += CalcItemAbility(ABILITY_MINLAP);
		CalcAbilityValue(dwMinAP, MTYPE_LAP);
	}
	else
	{
		dwMinAP = Calc2ndAbility(FTYPE_PAP);
		dwMinAP += CalcItemAbility(ABILITY_MINAP);
		CalcAbilityValue(dwMinAP, MTYPE_PAP);
	}

	DWORD dwMaxAP = GetMaxAP(bArrow);
	if(dwMinAP > dwMaxAP)
		dwMinAP = dwMaxAP;

	return dwMinAP;
}

DWORD CTObjBase::GetMaxMagicAP()
{
	DWORD dwMaxAP;
	dwMaxAP = Calc2ndAbility(FTYPE_MAP);
	dwMaxAP += CalcItemAbility(ABILITY_MAXMAP);
	CalcAbilityValue(dwMaxAP, MTYPE_MAP);
	return dwMaxAP;
}

DWORD CTObjBase::GetMinMagicAP()
{
	DWORD dwMinAP;
	dwMinAP = Calc2ndAbility(FTYPE_MAP);
	dwMinAP += CalcItemAbility(ABILITY_MINMAP);
	CalcAbilityValue(dwMinAP, MTYPE_MAP);

	DWORD dwMaxAP = GetMaxMagicAP();
	if(dwMaxAP < dwMinAP)
		dwMinAP = dwMaxAP;

	return dwMinAP;
}

DWORD CTObjBase::GetPureMaxHP()
{
	DWORD dwValue = 0;
	DWORD dwInit = GetFormulaInit(FTYPE_HP);
	FLOAT fRateX = GetFormulaRateX(FTYPE_HP);
	FLOAT fRateY = GetFormulaRateY(FTYPE_HP);
	dwValue = dwInit + DWORD(GetCON(TRUE) * fRateX);
	return dwValue;
}

DWORD CTObjBase::GetPureMaxMP()
{
	DWORD dwValue = 0;
	DWORD dwInit = GetFormulaInit(FTYPE_MP);
	FLOAT fRateX = GetFormulaRateX(FTYPE_MP);
	FLOAT fRateY = GetFormulaRateY(FTYPE_MP);
	dwValue = dwInit + DWORD(GetMEN(TRUE) * fRateX);
	return dwValue;
}

DWORD CTObjBase::GetMaxHP()
{
	DWORD dwMaxHP = Calc2ndAbility(FTYPE_HP);
	dwMaxHP += CalcItemAbility(ABILITY_MAXHP);
	CalcAbilityValue(dwMaxHP, MTYPE_MHP);
	return dwMaxHP;
}

DWORD CTObjBase::GetMaxMP()
{
	DWORD dwMaxMP = Calc2ndAbility(FTYPE_MP);
	dwMaxMP += CalcItemAbility(ABILITY_MAXMP);
	CalcAbilityValue(dwMaxMP, MTYPE_MMP);
	return dwMaxMP;
}

DWORD CTObjBase::GetDefendPower()
{
	DWORD dwDP = Calc2ndAbility(FTYPE_PDP);
	dwDP += CalcItemAbility(ABILITY_PDP);
	CalcAbilityValue(dwDP, MTYPE_PDP);
	return dwDP;
}

DWORD CTObjBase::GetMagicDefPower()
{
	DWORD dwMDP = Calc2ndAbility(FTYPE_MDP);
	dwMDP += CalcItemAbility(ABILITY_MDP);
	CalcAbilityValue(dwMDP, MTYPE_MDP);
	return dwMDP;
}
WORD CTObjBase::GetAttackLevel()
{
	DWORD dwAL = Calc2ndAbility(FTYPE_AL);
	dwAL += CalcItemAbility(ABILITY_AL);
	CalcAbilityValue(dwAL, MTYPE_AL);
	return WORD(dwAL);
}

WORD CTObjBase::GetDefendLevel()
{
	DWORD dwDL = Calc2ndAbility(FTYPE_DL);
	dwDL += CalcItemAbility(ABILITY_DL);
	CalcAbilityValue(dwDL, MTYPE_DL);
	return WORD(dwDL);
}
WORD CTObjBase::GetMagicAtkLevel()
{
	DWORD dwAL = Calc2ndAbility(FTYPE_MAL);
	dwAL += CalcItemAbility(ABILITY_MAL);
	CalcAbilityValue(dwAL, MTYPE_MAL);
	return WORD(dwAL);
}
WORD CTObjBase::GetMagicDefLevel()
{
	DWORD dwDL = Calc2ndAbility(FTYPE_MDL);
	dwDL += CalcItemAbility(ABILITY_MDL);
	CalcAbilityValue(dwDL, MTYPE_MDL);
	return WORD(dwDL);
}
BYTE CTObjBase::GetCriticalPysProb()
{
	DWORD dwCR = Calc2ndAbility(FTYPE_PCR);
	dwCR += CalcItemAbility(ABILITY_PCR);
	CalcAbilityValue(dwCR, MTYPE_CR);
	return BYTE(dwCR);
}
BYTE CTObjBase::GetCriticalMagicProb()
{
	DWORD dwCR = Calc2ndAbility(FTYPE_MCR);
	dwCR += CalcItemAbility(ABILITY_MCR);
	CalcAbilityValue(dwCR, MTYPE_MCR);
	return BYTE(dwCR);
}
BYTE CTObjBase::GetChargeProb()
{
	DWORD dwCP = Calc2ndAbility(FTYPE_MSP);
	dwCP += CalcItemAbility(ABILITY_CMP);
    CalcAbilityValue(dwCP, MTYPE_CMP);
	return BYTE(dwCP);
}
DWORD CTObjBase::GetShieldDP()
{
	DWORD dwDP = 0;
	DWORD dwR = CalcItemAbility(ABILITY_SDR);
	CalcAbilityValue(dwR, MTYPE_SDR);
	if(dwR > DWORD(rand() % 100))
	{
		dwDP = CalcItemAbility(ABILITY_SDP);
		CalcAbilityValue(dwDP, MTYPE_SPDPOW);
	}

	return dwDP;
}
DWORD CTObjBase::GetShieldMDP()
{
	DWORD dwDP = 0;
	DWORD dwR = CalcItemAbility(ABILITY_SMDR);
	CalcAbilityValue(dwR, MTYPE_SMDR);
	if(dwR > DWORD(rand() % 100))
	{
		dwDP = CalcItemAbility(ABILITY_SMDP);
		CalcAbilityValue(dwDP, MTYPE_SMDPOW);
	}

	return dwDP;
}

BYTE CTObjBase::IsEquipSpecial()
{
	CTInven *pTEQUIP = FindTInven(INVEN_EQUIP);

	if(pTEQUIP)
	{
		MAPTITEM::iterator itTITEM;

		for( itTITEM = pTEQUIP->m_mapTITEM.begin(); itTITEM != pTEQUIP->m_mapTITEM.end(); itTITEM++)
			if((*itTITEM).second->m_pTITEM->m_bSpecial)
				return TRUE;
	}

	return FALSE;
}
BYTE CTObjBase::GetChargeSpeed()
{
	DWORD dwCS = Calc2ndAbility(FTYPE_MCS);
	dwCS += CalcItemAbility(ABILITY_MCS);
	CalcAbilityValue(dwCS, MTYPE_MCS);
	return BYTE(dwCS);
}
DWORD CTObjBase::GetAvoidProb()
{
	DWORD dwAvoid = CalcItemAbility(ABILITY_AVP);
	CalcAbilityValue(dwAvoid, MTYPE_AVP);
	return dwAvoid;
}
DWORD CTObjBase::GetHPR()
{
	DWORD dwInc = Calc2ndAbility(FTYPE_HPR);
	dwInc += CalcItemAbility(ABILITY_HPR);
	CalcAbilityValue(dwInc, MTYPE_HPR);
	return dwInc;
}
DWORD CTObjBase::GetMPR()
{
	DWORD dwInc = Calc2ndAbility(FTYPE_MPR);
	dwInc += CalcItemAbility(ABILITY_MPR);
	CalcAbilityValue(dwInc, MTYPE_MPR);
	return dwInc;
}
WORD CTObjBase::GetPartyID()
{
	return m_bPartyType == PT_SOLO ? 0 : m_wPartyID;
}

BYTE CTObjBase::GetLevel()
{
	return m_bLevel;
}

BYTE CTObjBase::GetClass()
{
	return 0;
}

BYTE CTObjBase::GetCritical(CTSkill * pSkill)
{
	BYTE bAttackType = pSkill->m_pTSKILL->GetAttackType();
	BYTE bCR;
	if(bAttackType == SAT_MAGIC)
		bCR = GetCriticalMagicProb();
	else
		bCR = GetCriticalPysProb();

	return bCR;
}
BYTE CTObjBase::GetAtkHitType(CTSkillTemp * pSkill,
							  BYTE bCR,
							  WORD wAL,
							  BYTE bSkillLevel,
							  BYTE bAttackerLevel,
							  BYTE bEquipSpecial)
{
	if(bCR == 0xFF || 	//분신소환수 공격할때 무조건 MISS
		m_bMode == MT_GOHOME)
		return HT_MISS;

	if(m_bIsSpecial && !bEquipSpecial)
		return HT_MISS;

	DWORD dwAtk = 0;
	BYTE bRand = 0;

	if(pSkill->m_bHitTest == 1)
	{
		bRand = rand() % 100;
		if(bRand < pSkill->m_bHitInit + (bSkillLevel-1)*pSkill->m_bHitInc)
			dwAtk = 100;
		else
			dwAtk = 0;
	}
	else
	{
		int nL = int(m_bLevel)-bAttackerLevel;
		if(!wAL) wAL = 1;

		if(pSkill->GetAttackType() == SAT_PHYSIC)
		{
			DWORD dwInit = GetFormulaInit(FTYPE_PAR);
			dwAtk = (DWORD)min( dwInit, max((GetFormulaRateY(FTYPE_PAR) - pow(GetFormulaRateX(FTYPE_PAR), nL) * GetDefendLevel()/wAL),0)*100);
		}
		else
		{
			DWORD dwInit = GetFormulaInit(FTYPE_MAR);
			dwAtk = (DWORD)min(dwInit, max((GetFormulaRateY(FTYPE_MAR) - pow(GetFormulaRateX(FTYPE_MAR), nL) * GetMagicDefLevel()/wAL),0)*100);
		}
	}

	DWORD dwAVP = 0;
	if(pSkill->IsNegative())
		dwAVP = GetAvoidProb();

	dwAtk = dwAtk > dwAVP ? max(dwAtk - dwAVP, 20) : 20;
	BYTE bRand2 = rand() % 100;
	BYTE bHit = bRand2 <  dwAtk ? (BYTE(rand() % 100) < bCR ? HT_CRITICAL : HT_NORMAL) : HT_MISS;
	return bHit;
}

WORD CTObjBase::GetPysResist()
{
	return 0;
}
WORD CTObjBase::GetMagicResist()
{
	return 0;
}
WORD CTObjBase::GetFR()
{
	return 0;
}
WORD CTObjBase::GetIR()
{
	return 0;
}
WORD CTObjBase::GetER()
{
	return 0;
}
WORD CTObjBase::GetSR()
{
	return 0;
}
WORD CTObjBase::GetDR()
{
	return 0;
}

void CTObjBase::OnDie( DWORD dwAttackID, BYTE bObjectType, WORD wTempMonID) 
{
	m_bAction = TA_DEAD;
	m_bStatus = OS_DEAD;

	m_dwHP = 0;
	m_dwMP = 0;

	SetTransAttack(0);

	ReleaseMaintain();

	if(!m_pMAP)
		return;

	VPLAYER vPlayer;
	vPlayer.clear();

	m_pMAP->GetNeighbor(&vPlayer, m_fPosX, m_fPosZ);

	while(!vPlayer.empty())
	{
		CTPlayer * pChar = vPlayer.back();
		pChar->SendCS_DIE_ACK(m_dwID, m_bType);
		vPlayer.pop_back();		
	}

	vPlayer.clear();
}

CTSkill * CTObjBase::FindTSkill(WORD wSkill)
{
	MAPTSKILL::iterator it = m_mapTSKILL.find(wSkill);
	if(it==m_mapTSKILL.end())
		return NULL;
	else
		return (*it).second;
}

BYTE CTObjBase::UpdateSkill(CTSkillTemp * pTemp, WORD wSkillID, BYTE bLevel)
{
	if(!FindTSkill(wSkillID))
	{
		CTSkill * pSkill = new CTSkill();
		pSkill->m_pTSKILL = pTemp;
		pSkill->m_bLevel = bLevel;

		m_mapTSKILL.insert(MAPTSKILL::value_type(pTemp->m_wID, pSkill));
		if(m_bType == OT_PC)
		{
			((CTPlayer *)this)->SendCS_SKILLBUY_ACK(
				SKILL_SUCCESS,
				pTemp->m_wID,
				bLevel);

			//스킬 버릴때 m_vRemainSkill에서 해당 스킬 꼭 뺄것
			RemainSkill( pSkill, 0);
		}

		return TRUE;
	}

	return FALSE;
}

void CTObjBase::CheckMaintainSkill(DWORD dwTick)
{
	WORD wPcbangSkill = 0;

	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
		if(m_vMaintainSkill[i]->IsEnd(dwTick))
		{
			if(m_vMaintainSkill[i]->m_pTSKILL->m_wID == TPCBANG_SKILL)
				wPcbangSkill = TPCBANG_SKILL;

			EraseMaintainSkill(m_vMaintainSkill[i], i);
		}
		else
		{
			if(m_bType == OT_PC)
			{
				WORD wKillCount = m_vMaintainSkill[i]->m_pTSKILL->GetAutoExp();
				if(wKillCount)
				{
					if(dwTick - m_vMaintainSkill[i]->m_dwChargeTick >= 10000)
					{
						DWORD dwAddTick = dwTick - m_vMaintainSkill[i]->m_dwChargeTick >= 20000 ? dwTick - m_vMaintainSkill[i]->m_dwChargeTick : 10000;
						m_vMaintainSkill[i]->m_dwChargeTick += dwAddTick;
						DWORD dwGain = wKillCount * DWORD( (m_bLevel > 79 ? 1.85f : 1.0f) * INT(30.0f * log10( double(m_bLevel + 1)) + m_bLevel * 2) );
						BYTE bRate = GetMonExpRate();
						dwGain = dwGain * bRate / 100;
						if(dwGain)
							((CTPlayer *)this)->GainExp(dwGain, FALSE);
					}
				}
			}

			i++;
		}
	}

	if(m_bType == OT_PC && wPcbangSkill)
		((CTPlayer *)this)->HangExpBuff();
}

void CTObjBase::CheckEternalBuff()
{
	if(m_bType != OT_PC || m_dwMP)
		return;

	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
		if(	m_vMaintainSkill[i]->m_pTSKILL->m_bIsuse &&
			!m_vMaintainSkill[i]->m_pTSKILL->m_dwDuration)
			EraseMaintainSkill(m_vMaintainSkill[i], i);
		else
			i++;
	}
}

void CTObjBase::EraseMaintainSkill(WORD wID)
{
	if(!wID)
		return;

	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
		if(m_vMaintainSkill[i]->m_pTSKILL->m_wID == wID)
			EraseMaintainSkill(m_vMaintainSkill[i], i);
		else
			i++;
	}

	if(m_bType == OT_PC && wID == TPCBANG_SKILL)
		((CTPlayer *)this)->HangExpBuff();
}

BYTE CTObjBase::CheckAttack()
{
	if(m_bGetaway || !m_bCanAttack)
		return FALSE;

	for(DWORD i=0; i<m_vMaintainSkill.size();i++)
	{
		CTSkillTemp * pSkill = m_vMaintainSkill[i]->m_pTSKILL;

		for(DWORD dwData=0; dwData<pSkill->m_vData.size(); dwData++)
		{
			LPTSKILLDATA pData = pSkill->m_vData[dwData];
			if(pData->m_bType == SDT_STATUS &&
				( pData->m_bExec == SDT_STATUS_BLOCK ||
				pData->m_bExec == SDT_STATUS_HOLD ||
				pData->m_bExec == SDT_STATUS_HYPNOSIS ||
				(m_bType != OT_PC && (	pData->m_bExec == SDT_STATUS_FLASH || pData->m_bExec == SDT_STATUS_DARK))))
				return FALSE;

			if(pData->m_bType == SDT_AI && pData->m_bExec == SDT_RUNAWAY)
				return FALSE;
		}
	}

	return TRUE;
}

void CTObjBase::EraseMaintainSkill(CTSkill * pSkill, DWORD index)
{
	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	DWORD dwMaxHP = GetMaxHP();
	DWORD dwMaxMP = GetMaxMP();
	if(pSkill->m_pTSKILL->m_bIsHideSkill)
		EraseOtherSelf();

	if(m_pMAP)
	{
		if(pSkill->m_pTSKILL->m_bORadius &&
			(pSkill->m_dwAttackID != m_dwID || pSkill->m_bAttackType != m_bType))
		{
			CTPlayer * pChar = NULL;
			if(m_bType == OT_PC)
				pChar = (CTPlayer *)this;
			else
			{
				CTMonster * pMon = (CTMonster *)this;
				pChar = pMon->FindHost(pMon->m_dwHostID);
			}

			if(pChar)
				vPLAYERS.push_back(pChar);
		}
		else
			m_pMAP->GetNeighbor(
				&vPLAYERS,
				m_fPosX,
				m_fPosZ);

		for(DWORD nPlayer=0; nPlayer < vPLAYERS.size(); nPlayer++)
		{
			vPLAYERS[nPlayer]->SendCS_SKILLEND_ACK(
				m_dwID,
				m_bType,
				pSkill->m_pTSKILL->m_wID);
		}

		if(m_bType == OT_PC &&
			_AtlModule.IsTournamentMap(m_pMAP->m_wMapID) &&
			(_AtlModule.m_tournament.m_bStep == TNMTSTEP_QFINAL ||
			_AtlModule.m_tournament.m_bStep == TNMTSTEP_SFINAL ||
			_AtlModule.m_tournament.m_bStep == TNMTSTEP_FINAL) &&
			pSkill->m_pTSKILL->IsHideBuff())
		{
			m_pMAP->SetHideTick(m_dwID, pSkill->m_dwMaintainTick ? pSkill->m_dwMaintainTick : _AtlModule.m_dwTick - pSkill->m_dwChargeTick);
		}
	}

	BYTE bCanAttack = CheckAttack();
	WORD wSkillID = pSkill->m_pTSKILL->m_wID;
	m_vMaintainSkill.erase(m_vMaintainSkill.begin()+index);

	if(pSkill->m_pTSKILL->IsTrans())
		SetTransAttack(0);

	if(m_bType == OT_MON)
	{
		if(!bCanAttack && CheckAttack())
		{
			// 도망 스킬 해제
			CTMonster * pMon = (CTMonster *)this;

			if(pMon->m_dwTargetID)
				pMon->OnEvent(AT_DEFEND, 0, pMon->m_dwHostID, pMon->m_dwTargetID, pMon->m_bTargetType);
			else
			{
				MAPTAGGRO::iterator it;
				DWORD dwMaxAggro=0;
				TAGGRO aggro;
				for(it=pMon->m_mapAggro.begin(); it!=pMon->m_mapAggro.end(); it++)
				{
					if(dwMaxAggro < (*it).second.m_dwAggro)
					{
						dwMaxAggro = (*it).second.m_dwAggro;
						aggro.Copy((*it).second);
					}
				}
				if(aggro.m_dwHostID &&
					aggro.m_bCountry != pMon->m_bCountry)
					pMon->SetAggro(aggro.m_dwHostID, aggro.m_dwObjID, aggro.m_bObjType, aggro.m_bCountry, TCLASS_COUNT, 0, 0, 1, TRUE);
			}
		}
	}

	BYTE bDie = pSkill->m_pTSKILL->IsDie();
	if(bDie)
	{
		m_dwHP = 0;
		m_dwMP = 0;
	}

	delete pSkill;

	DWORD dwMCHP = GetMaxHP();
	DWORD dwMCMP = GetMaxMP();

	BYTE bHPMP = (dwMaxHP == dwMCHP && dwMaxMP == dwMCMP) ? FALSE : TRUE;
	if(bHPMP || bDie)
	{
		if(dwMCHP < m_dwHP)	m_dwHP = dwMCHP;
		if(dwMCMP < m_dwMP) m_dwMP = dwMCMP;

		for(DWORD nPlayer=0; nPlayer < vPLAYERS.size(); nPlayer++)
		{
			vPLAYERS[nPlayer]->SendCS_HPMP_ACK(
				m_dwID,
				m_bType,
				m_bLevel,
				dwMCHP,
				m_dwHP,
				dwMCMP,
				m_dwMP);
		}
	}
	vPLAYERS.clear();

	if(m_bType == OT_PC)
	{
		CTPlayer * pPlayer = (CTPlayer *)this;
		if(wSkillID == TREVIVAL_SKILL)
			pPlayer->RecallRevivalErase();

		pPlayer->SendCS_CHARSTATINFO_ACK(pPlayer);
	}
	else if(m_bType == OT_RECALL)
	{
		if(wSkillID == TREVIVAL_SKILL)
		{
			CTRecallMon * pMon = (CTRecallMon *)this;
			CTPlayer * pOwner = pMon->FindHost(pMon->m_dwHostID);
			if(pOwner && pOwner->m_bMain && pOwner->m_pMAP)
				pOwner->EraseMaintainSkill(TREVIVAL_SKILL);
		}
	}

	if(bDie)
		OnDie(0,OT_NONE,0); 
}

CTSkill * CTObjBase::FindMaintainSkill(WORD wSkill)
{
	for(DWORD i=0; i<(DWORD)m_vMaintainSkill.size(); i++)
	{
		if(m_vMaintainSkill[i]->GetID() == wSkill)
			return m_vMaintainSkill[i];
	}

	return NULL;
}

BYTE CTObjBase::IsEquipSkillItem(CTSkill * pSkill)
{
	if(!pSkill->m_pTSKILL->m_dwWeapon)
		return TRUE;

	CTInven * pTEquip = FindTInven(INVEN_EQUIP);

	if(pTEquip)
	{
		MAPTITEM::iterator it;
		for(it=pTEquip->m_mapTITEM.begin(); it!=pTEquip->m_mapTITEM.end(); it++)
		{
			CTItem * pItem = (*it).second;
			if(BITSHIFTID(pItem->m_pTITEM->m_bKind-1) & pSkill->m_pTSKILL->m_dwWeapon)
				return TRUE;
		}
	}
	return FALSE;
}

BYTE CTObjBase::UseSkillItem(CTSkill * pSkill, BYTE bAddtional)
{
	if(m_bType == OT_PC)
	{
		CTPlayer * pMe = (CTPlayer *)this;
        if(pMe->m_bStore || pMe->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;
	}

	if(!DeleteSkillItem(pSkill->m_pTSKILL->m_wUseItem))
		return FALSE;

	if(!pSkill->m_pTSKILL->m_dwWeapon)
		return TRUE;

	CTInven * pTEquip = FindTInven(INVEN_EQUIP);

	if(pTEquip)
	{
		MAPTITEM::iterator it;
		for(it=pTEquip->m_mapTITEM.begin(); it!=pTEquip->m_mapTITEM.end(); it++)
		{
			CTItem * pItem = (*it).second;
			if(BITSHIFTID(pItem->m_pTITEM->m_bKind-1) & pSkill->m_pTSKILL->m_dwWeapon)
			{
				if(!pItem->HavePower())
					return FALSE;

				if(!pItem->m_pTITEM->m_bUseItemKind)
					return TRUE;
				else
				{
					BYTE bNeedCount = pItem->m_pTITEM->m_bUseItemCount + bAddtional;
					MAPTINVEN::iterator itInv;
					for(itInv=m_mapTINVEN.begin(); itInv!=m_mapTINVEN.end(); itInv++)
					{
						BYTE bCount=0;

						MAPTITEM mapUItem;
						mapUItem.clear();

						MAPTITEM::iterator itUI;

						CTInven * pInven = (*itInv).second;
						if(pInven)
						{
							for(itUI=pInven->m_mapTITEM.begin(); itUI != pInven->m_mapTITEM.end(); itUI++)
							{
								if(pItem->m_pTITEM->m_bUseItemKind == (*itUI).second->m_pTITEM->m_bKind)
								{
									mapUItem.insert(MAPTITEM::value_type((*itUI).first, (*itUI).second));
									bCount += (*itUI).second->m_bCount;
									if(bCount >= bNeedCount)
										break;
								}
							}
						}
						if(bCount >= bNeedCount)
						{
							CTPlayer * pPlayer = (CTPlayer *)this;
							for(itUI=mapUItem.begin(); itUI!=mapUItem.end(); itUI++)
							{
								BYTE bHaveCnt = (*itUI).second->m_bCount;
								_AtlModule.UseItem(
									pPlayer,
									pInven,
									(*itUI).second,
									bNeedCount > bHaveCnt ? bHaveCnt : bNeedCount);

								bNeedCount -= bNeedCount > bHaveCnt ? bHaveCnt : bNeedCount;
								if(!bNeedCount)
									break;
							}

							pPlayer->SendCS_MOVEITEM_ACK(MI_SUCCESS);
							return TRUE;
						}

						mapUItem.clear();
					}
				}
			}
		}
	}
	return FALSE;
}

BYTE CTObjBase::CheckPrevAct(WORD wActID)
{
	if(!wActID)
		return TRUE;

	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		if(pSkill->m_pTSKILL->m_wID == wActID)
			return TRUE;
	}

	return FALSE;
}
BYTE CTObjBase::UpdateBuffSkill(CTSkill * pTemp, DWORD dwAttacker, BYTE bAttackType)
{
	if(IsTrans() && pTemp->m_pTSKILL->IsTrans())
		return FALSE;

	if(pTemp->m_pTSKILL->IsNegative())
	{
		for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
		{
			if(m_vMaintainSkill[i]->m_pTSKILL->m_wID == pTemp->m_pTSKILL->m_wID)
			{
				delete m_vMaintainSkill[i];
				m_vMaintainSkill.erase(m_vMaintainSkill.begin()+i);
			}
		}

		return TRUE;
	}

	BYTE bCount = BYTE(m_vMaintainSkill.size());
	BYTE bRet = FALSE;
	BYTE bCollision = FALSE;

	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
		BYTE bErase = FALSE;
		CTSkill * pSkill = m_vMaintainSkill[i];
		if(pSkill->m_pTSKILL->IsNegative())
		{
			bErase = FALSE;
		}
		else if(pTemp->m_pTSKILL->IsPosture())
		{
			if(pSkill->m_pTSKILL->IsPosture() ||
				(pSkill->m_pTSKILL->m_wPosture && 
				pSkill->m_pTSKILL->m_wPosture != pTemp->m_pTSKILL->m_wID &&
				pSkill->m_bAttackType == m_bType &&
				pSkill->m_dwAttackID == m_dwID))
				bErase = TRUE;
		}
		else if(pSkill->m_pTSKILL->IsPosture())
		{
			bErase = FALSE;
		}
		else
		{
			for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
			{
				LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
				if(pData->m_bAction == SA_BUFF && pTemp->m_pTSKILL->HaveSkillData(pData))
				{
					DWORD dwValue1=0;
					DWORD dwValue2=0;
					if(pData->m_bType == SDT_ABILITY)
					{
						dwValue1 = dwValue2 = GetAbility(pData->m_bExec);
						CalcAbilityValue(dwValue1, pData->m_bExec, pSkill);
						CalcAbilityValue(dwValue2, pData->m_bExec, pTemp);
					}
					else
					{
						dwValue1 = pSkill->GetValue(pData->m_bType, pData->m_bExec);
						dwValue2 = pTemp->GetValue(pData->m_bType, pData->m_bExec);
					}

					if(pSkill->m_pTSKILL->m_bPriority < pTemp->m_pTSKILL->m_bPriority)
					{
						bErase = TRUE;
						break;
					}
					else if(pSkill->m_pTSKILL->m_bPriority > pTemp->m_pTSKILL->m_bPriority)
					{
						bCollision = TRUE;
						bErase = FALSE;
						break;
					}
					else if(dwValue1 > dwValue2)
					{
						bCollision = TRUE;
						bErase = FALSE;
					}
					else if(dwValue1 < dwValue2)
					{
						bErase = TRUE;
						break;
					}
					else if((pSkill->m_bAttackType == m_bType && pSkill->m_dwAttackID == m_dwID) &&
                        (bAttackType != m_bType || dwAttacker != m_dwID))
					{
						bCollision = TRUE;
						bErase = FALSE;
					}
					else if((pSkill->m_bAttackType != m_bType || pSkill->m_dwAttackID != m_dwID) &&
						(bAttackType == m_bType && dwAttacker == m_dwID))
					{
						bErase = TRUE;
						break;
					}
					else if(pSkill->m_pTSKILL->m_bORadius)
					{
						bCollision = TRUE;
						bErase = FALSE;
					}
					else
					{
						bErase = TRUE;
						break;
					}
				}
			}
		}

		if(bErase)
		{
			EraseMaintainSkill(m_vMaintainSkill[i], i);
			bRet = TRUE;
		}
		else
			i++;
	}

	return !bCount || bRet || !bCollision;
}

BYTE CTObjBase::PerformSkill(DWORD dwAttack,
							 BYTE bAtkerType,
							 BYTE bAttackCountry,
							 BYTE bAttackAidCountry,
							 BYTE bAttackClass,
							 DWORD dwHostID,
							 CTSkillTemp * pTemp,
							 BYTE bSkillLevel,
							 BYTE bHit,
							 DWORD dwPysMinPower,
							 DWORD dwPysMaxPower,
							 DWORD dwMgMinPower,
							 DWORD dwMgMaxPower,
							 WORD wTransHP,
							 WORD wTransMP,
							 BYTE bCurseProb,
							 WORD wAtkLevel,
							 BYTE bAttackerLevel,
							 int nDamageHP,
							 int nDamageMP,
                             FLOAT fAtkPosX,
                             FLOAT fAtkPosY,
                             FLOAT fAtkPosZ,
							 CTRecallMon ** pRecallMon)
{
	if(!m_pMAP)
		return PERFORM_MISS;

	BYTE bResult = PERFORM_SUCCESS;

	for(DWORD i=0; i<pTemp->m_vData.size(); i++)
	{
		LPTSKILLDATA pData = pTemp->m_vData[i];

		switch(pData->m_bType)
		{
		case SDT_ABILITY:
			switch(pData->m_bExec)
			{
			case MTYPE_HI:
				if(nDamageHP > 0)
				{
					DWORD dwBlood = (DWORD)max(pTemp->Calculate(bSkillLevel, i, nDamageHP), 0);
					if(dwBlood)
						_AtlModule.SendMW_GETBLOOD_ACK(dwAttack, bAtkerType, dwHostID, MTYPE_HI, dwBlood);
				}
				break;
			case MTYPE_MI:
				if(nDamageMP > 0)
				{
					DWORD dwBlood = (DWORD)max(pTemp->Calculate(bSkillLevel, i, nDamageMP), 0);
					if(dwBlood)
						_AtlModule.SendMW_GETBLOOD_ACK(dwAttack, bAtkerType, dwHostID, MTYPE_MI, dwBlood);
				}
				break;
			}
			break;
		case SDT_TRAP:
		case SDT_RECALL:
			{
				if( m_bType != OT_PC )
					return PERFORM_FAIL;

				CTPlayer * pMain = (CTPlayer *)this;

				WORD wRecMon;
				BYTE bTempted = FALSE;
				if(pData->m_bExec == SER_MONSTER)
				{
					if(m_wTemptedMon == 0)
						return PERFORM_FAIL;
					else
					{
						wRecMon = m_wTemptedMon;
						bTempted = TRUE;
					}
				}
				else
					wRecMon = WORD(pTemp->GetValue(pData, bSkillLevel));

				MAPTMONSTERTEMP::iterator itMon = _AtlModule.m_mapTMONSTER.find(wRecMon);
				if(itMon!=_AtlModule.m_mapTMONSTER.end())
				{
					DWORD dwATTR = MAKELONG((*itMon).second->m_wSummonAttr, min(bAttackerLevel, 90));

					pMain->CheckRecallMon((*itMon).second, TRUE);

					FLOAT fPosX;
					FLOAT fPosY;
					FLOAT fPosZ;

					if(pTemp->m_bTargetRange == TSKILLRANGE_POINT)
					{
						fPosX = fAtkPosX;
						fPosY = fAtkPosY;
						fPosZ = fAtkPosZ;
					}
					else
					{
						fPosX = m_fPosX - 2*sinf(m_wDIR*FLOAT(M_PI)/900);
						fPosY = m_fPosY;
						fPosZ = m_fPosZ - 2*cosf(m_wDIR*FLOAT(M_PI)/900);
					}

					if( pMain->m_mapRecallMon.size() >= MAX_RECALLMON )
						return PERFORM_FAIL;

					if((*itMon).second->m_bIsSelf)
					{
						WORD wPosX = WORD(fPosX) / CELL_SIZE;
						WORD wPosZ = WORD(fPosZ) / CELL_SIZE;

						MAPTCELL::iterator itCELL = m_pMAP->m_mapTCELL.find( MAKELONG( wPosX, wPosZ));
						if( itCELL != m_pMAP->m_mapTCELL.end() &&
							(*itCELL).second->IsMainCell(m_bChannel) )
						{
							*pRecallMon = _AtlModule.CreateRecallMon(
								pMain,
								(*itMon).second,
								dwATTR,
								0, 0,
								NAME_NULL,
								pMain->m_bLevel,
								(*itMon).second->m_bClass,
								(*itMon).second->m_bRace,
								TA_STAND,
								OS_WAKEUP,
								MT_NORMAL,
								0,0,0,0,
								bHit,
								dwPysMinPower,
								dwPysMaxPower,
								dwMgMinPower,
								dwMgMaxPower,
								wAtkLevel,
								bSkillLevel,
								fPosX,
								fPosY,
								fPosZ,
								m_wDIR,
								m_pInstanceSkill->GetMaintainTick() + pMain->GetRecallLifeTime(),
								(*itMon).second->m_vSKILL);

							if(*pRecallMon)
								m_pMAP->EnterMAP(*pRecallMon);
						}
						else
						{
							_AtlModule.SendMW_CREATERECALLMON_ACK(
								pMain->m_dwID,
								pMain->m_dwKEY,
								0, wRecMon,
								dwATTR, 0,
								NAME_NULL,
								m_pInstanceSkill->GetMaintainTick() + pMain->GetRecallLifeTime(),
								pMain->m_bLevel,
								(*itMon).second->m_bClass,
								(*itMon).second->m_bRace,
								TA_STAND,
								OS_WAKEUP,
								MT_NORMAL,
								0,0,0,0,
								bHit,
								bSkillLevel,
								fPosX,
								fPosY,
								fPosZ,
								m_wDIR,
								(*itMon).second->m_vSKILL);
						}
					}
					else
					{
						VWORD vSkill;
						vSkill.clear();

						if((*itMon).second->m_wID)
							vSkill = (*itMon).second->m_vSKILL;
						else
						{
							for( WORD ms=31; ms<=34; ms++)
							{
								//기본스킬
								CTSkill * pMy = FindTSkill(ms);
								if(pMy && IsEquipSkillItem(pMy))
									vSkill.push_back(ms);
							}
						}

						_AtlModule.SendMW_CREATERECALLMON_ACK(
							pMain->m_dwID,
							pMain->m_dwKEY,
							0,
							wRecMon,
							dwATTR,
							0,
							NAME_NULL,
							m_pInstanceSkill->GetMaintainTick() + pMain->GetRecallLifeTime(),
							pMain->m_bLevel,
							(*itMon).second->m_wID ? (*itMon).second->m_bClass : pMain->m_bClass,
							(*itMon).second->m_wID ? (*itMon).second->m_bRace : pMain->m_bRace,
							(*itMon).second->m_wID ? TA_STAND : pMain->m_bAction,
							(*itMon).second->m_wID ? OS_WAKEUP : pMain->m_bStatus,
							(*itMon).second->m_wID ? MT_NORMAL : pMain->m_bMode,
							(*itMon).second->m_wID ? 0 : pMain->GetMaxHP(),
							(*itMon).second->m_wID ? 0 : pMain->GetMaxMP(),
							(*itMon).second->m_wID ? 0 : pMain->GetMaxHP(),
							(*itMon).second->m_wID ? 0 : pMain->GetMaxMP(),
							bHit,
							bSkillLevel,
							fPosX,
							fPosY,
							fPosZ,
							m_wDIR,
							vSkill);

/*						_AtlModule.SendDM_CREATERECALLMON_REQ(
							pMain,
							wRecMon,
							dwATTR,
							0, 
							NAME_NULL,
							pMain->m_bLevel,
							(*itMon).second->m_wID ? (*itMon).second->m_bClass : pMain->m_bClass,
							(*itMon).second->m_wID ? (*itMon).second->m_bRace : pMain->m_bRace,
							(*itMon).second->m_wID ? TA_STAND : pMain->m_bAction,
							(*itMon).second->m_wID ? OS_WAKEUP : pMain->m_bStatus,
							(*itMon).second->m_wID ? MT_NORMAL : pMain->m_bMode,
							(*itMon).second->m_wID ? 0 : pMain->GetMaxHP(),
							(*itMon).second->m_wID ? 0 : pMain->GetMaxMP(),
							(*itMon).second->m_wID ? 0 : pMain->GetMaxHP(),
							(*itMon).second->m_wID ? 0 : pMain->GetMaxMP(),
							bHit,
							bSkillLevel,
							fPosX,
							fPosY,
							fPosZ,
							m_wDIR,
							m_pInstanceSkill->GetMaintainTick() + pMain->GetRecallLifeTime(),
							vSkill);
*/
						vSkill.clear();
					}
				}
				else
					return PERFORM_MISS;
			}
			break;
		case SDT_TRANS:
			{
				if(IsTrans())
					return PERFORM_FAIL;

				switch(pData->m_bExec)
				{
					case SDT_TRANS_DISGUISE_D:
					case SDT_TRANS_DISGUISE_C:
					case SDT_TRANS_DISGUISE_B:
						{
							CTPlayer *pPlayer = NULL;
							if(m_bType == OT_PC)
								pPlayer = (CTPlayer *) this;
							else
								return PERFORM_FAIL;

							pPlayer->DeletePositiveMaintainSkill();
							pPlayer->DeleteAllRecallMon();
							pPlayer->DeleteAllSelfMon();
						}
						break;
					case SDT_TRANS_CHIPMUNK:
						if(m_bType == OT_MON)
							((CTMonster *)this)->OnEvent( AT_HELP, 0, 0, 0, 0);
						break;
				}

				SetTransAttack(pData->m_wValue);

				if(m_bType == OT_PC)
					((CTPlayer *)this)->DropGodBall();
			}
			break;
		case SDT_CURE:
			switch(pData->m_bExec)
			{
			case SCT_AFTERMATH:
				if(m_bType == OT_PC)
				{
					CTPlayer * pMain = (CTPlayer *)this;
					if(pMain->m_aftermath.m_bStep > pTemp->GetValue(pData, bSkillLevel))
						pMain->m_aftermath.m_bStep -= BYTE(pTemp->GetValue(pData, bSkillLevel));
					else
						pMain->m_aftermath.m_bStep = 0;

					pMain->m_aftermath.m_fReuseInc = FLOAT(pMain->m_aftermath.m_bStep*0.5);
					pMain->m_aftermath.m_fStatDec = FLOAT(pMain->m_aftermath.m_bStep*0.3);

					pMain->SendCS_CHARSTATINFO_ACK(pMain);

					if(pMain->m_pMAP)
					{
						VPLAYER vPLAYERS;
						vPLAYERS.clear();

						pMain->m_pMAP->GetNeighbor(
							&vPLAYERS,
							pMain->m_fPosX,
							pMain->m_fPosZ);

						while(!vPLAYERS.empty())
						{
							CTPlayer *pChar = vPLAYERS.back();

							pChar->SendCS_AFTERMATH_ACK(
								pMain->m_dwID,
								pMain->m_aftermath.m_bStep);

							vPLAYERS.pop_back();
						}
						vPLAYERS.clear();
					}
				}
				else
					return PERFORM_FAIL;
				break;
			case SCT_REVIVAL:
				if(m_bType == OT_PC)
				{
					if(m_bStatus != OS_DEAD)
						return PERFORM_MISS;
					else if(dwAttack != m_dwID)
						((CTPlayer *)this)->SendCS_REVIVALASK_ACK(dwAttack, bAtkerType, pTemp->m_wID, bSkillLevel);
					else
						((CTPlayer *)this)->Revival(AFTERMATH_NONE, pTemp, 0);
				}
				else
					return PERFORM_FAIL;
				break;
			case SCT_POSREMOVE:
				DeletePositiveMaintainSkill();
				break;
			case SCT_NEGREMOVE:
				DeleteNegativeMaintainSkill();
				break;
			case SCT_CANCELRECALL:
				{
					CTRecallMon * pRecall = NULL;
					if(m_bType == OT_PC)
						pRecall = ((CTPlayer *)this)->FindMainRecall();
					else
						pRecall = (CTRecallMon *)this;

					if(!pRecall)
						return PERFORM_FAIL;

					pRecall->m_dwDurationTick = 1;
				}
				break;
			case SCT_DIE:
				EraseDieSkillBuff();
				break;
			case SCT_GETRECALLHP:
				if(m_bType == OT_RECALL)
				{
					CTRecallMon * pMon = (CTRecallMon *)this;
					CTPlayer * pOwner = pMon->FindHost(dwHostID);
					if(pOwner &&
						pOwner->m_bMain &&
						pOwner->m_pMAP &&
						pOwner->m_bStatus != OS_DEAD)
					{
						DWORD dwIns = pMon->m_dwHP / 2;
						pOwner->m_dwHP += pTemp->Calculate(bSkillLevel, i, dwIns);
						DWORD dwMaxHP = pOwner->GetMaxHP();
						if(dwMaxHP < pOwner->m_dwHP)
							pOwner->m_dwHP = dwMaxHP;

						pMon->m_dwHP -= dwIns;

						VPLAYER vPlayer;
						vPlayer.clear();

						m_pMAP->GetNeighbor(&vPlayer, pMon->m_fPosX, pMon->m_fPosZ);

						while(!vPlayer.empty())
						{
							CTPlayer * pChar = vPlayer.back();
							vPlayer.pop_back();
							pChar->SendCS_HPMP_ACK(
								pOwner->m_dwID,
								pOwner->m_bType,
								pOwner->m_bLevel,
								dwMaxHP,
								pOwner->m_dwHP,
								pOwner->GetMaxMP(),
								pOwner->m_dwMP);
						}

						vPlayer.clear();
					}
				}
				break;
			case SCT_GETRECALLMP:
				if(m_bType == OT_RECALL)
				{
					CTRecallMon * pMon = (CTRecallMon *)this;
					CTPlayer * pOwner = pMon->FindHost(dwHostID);
					if(pOwner &&
						pOwner->m_bMain &&
						pOwner->m_pMAP)
					{
						DWORD dwIns = pMon->GetLifeTick(_AtlModule.m_dwTick) * pMon->m_dwHP / 180000;
						pOwner->m_dwMP += dwIns + pTemp->Calculate(bSkillLevel, i, dwIns);
						DWORD dwMaxMP = pOwner->GetMaxMP();
						if(dwMaxMP < pOwner->m_dwMP)
							pOwner->m_dwMP = dwMaxMP;

						pMon->m_dwRecallTick -= pMon->m_dwDurationTick;

						VPLAYER vPlayer;
						vPlayer.clear();

						m_pMAP->GetNeighbor(&vPlayer, pMon->m_fPosX, pMon->m_fPosZ);

						while(!vPlayer.empty())
						{
							CTPlayer * pChar = vPlayer.back();
							vPlayer.pop_back();
							pChar->SendCS_HPMP_ACK(
								pOwner->m_dwID,
								pOwner->m_bType,
								pOwner->m_bLevel,
								pOwner->GetMaxHP(),
								pOwner->m_dwHP,
								dwMaxMP,
								pOwner->m_dwMP);
						}

						vPlayer.clear();
					}
				}
				break;
			case SCT_MCPOWER:
			case SCT_POISON:
			case SCT_WOUND:
			case SCT_DISEASE:
				break;
			case SCT_HP:
				{
					int nValue =  pTemp->Calculate(bSkillLevel, i, GetMaxHP());
					m_dwHP +=  nValue + nValue * (rand()%16) / 100;
				}
				break;
			case SCT_MP:
				{
					int nValue =  pTemp->Calculate(bSkillLevel, i, GetMaxMP());
					m_dwMP +=  nValue + nValue * (rand()%16) / 100;
				}
				break;
			case SCT_CANCEL:
				{
					if(m_bType == OT_MON && IsTrans())
						return PERFORM_MISS;

                    CancelBlockBuff();
				}
				break;
			case SCT_RESETCOOLTIME:
				{
					if(m_bType != OT_PC)
						return PERFORM_MISS;

					CTPlayer * pChar = (CTPlayer *)this;
					pChar->ResetCoolTime(pTemp->m_wID);
				}
				break;
			case SCT_HPTRANS:
					m_dwHP += pTemp->Calculate(bSkillLevel, i, wTransHP);
				break;
			case SCT_MPTRANS:
					m_dwMP += pTemp->Calculate(bSkillLevel, i, wTransMP);
				break;
			}
			break;
		case SDT_STATUS:
			switch(pData->m_bExec)
			{
			case SDT_STATUS_RETURN:
				{
					if(m_bType == OT_PC)
					{
						CTPlayer * pTarget = (CTPlayer *)this;
						_AtlModule.Teleport( pTarget, pTarget->m_wSpawnID);
					}
				}
				break;
			case SDT_STATUS_RETURNPOS:
				{
					if(m_bType == OT_PC)
					{
						CTPlayer * pTarget = (CTPlayer *)this;
						_AtlModule.Teleport( pTarget, pData->m_wValue);
					}
				}
				break;
			case SDT_STATUS_DARK:
			case SDT_STATUS_FLASH:
				{
					if(m_bType == OT_MON)
					{
						CTMonster * pMon = (CTMonster *)this;
						pMon->Blind();
					}
				}
				break;
			case SDT_STATUS_TELEPORT:
				{
					if(m_bType == OT_PC)
					{
						CTPlayer * pTarget = (CTPlayer *)this;
						pTarget->PetRiding(0);
						pTarget->SendCS_WARP_ACK(
							m_dwID,
							m_bType,
							0,
							fAtkPosX,
							fAtkPosY,
							fAtkPosZ);
					}
				}
				break;
			case SDT_STATUS_WARP:
				{
					CTPlayer *pPlayer = NULL;
					if(m_bType == OT_PC)
					{
						pPlayer = (CTPlayer *) this;
						pPlayer->PetRiding(0);
					}
					else
						pPlayer = ((CTMonster *) this)->FindHost(((CTMonster *)this)->m_dwHostID);

					DWORD dwLength = pTemp->GetValue(pData, bSkillLevel);

					if(pPlayer)
						pPlayer->SendCS_WARP_ACK(
							m_dwID,
							m_bType,
							dwLength,
							fAtkPosX,
							fAtkPosY,
							fAtkPosZ);
					else
						return PERFORM_FAIL;
				}
				break;
			case SDT_STATUS_HPMPCHANGE:
				if(m_dwMP == 0)
					return PERFORM_FAIL;
				else
				{
					DWORD dwMaxHP = GetMaxHP();
					DWORD dwMaxMP = GetMaxMP();
					DWORD dwHP = m_dwHP;
					m_dwHP = dwMaxHP > m_dwMP ? m_dwMP : dwMaxHP;
					m_dwMP = dwMaxMP > dwHP ? dwHP : dwMaxMP;
				}
				break;
			case SDT_STATUS_HPTOMP:
				if(m_dwHP == 0)
					return PERFORM_FAIL;
				else
				{
					DWORD dwDec = m_dwHP / 2;
					m_dwMP += pTemp->Calculate(bSkillLevel, i, dwDec);
					m_dwHP -= dwDec;
				}
				break;
			}
			break;
		case SDT_AI:
			{
				switch(pData->m_bExec)
				{
				case SDT_AGGROGNEER:
					{
						VTMONSTER vMONS;
						vMONS.clear();

						m_pMAP->GetNeighbor(&vMONS, m_fPosX, m_fPosZ);

						while(!vMONS.empty())
						{
							CTMonster * pAGGROMON = vMONS.back();
							vMONS.pop_back();

							if(GetDistance(m_fPosX, m_fPosZ, pAGGROMON->m_fPosX, pAGGROMON->m_fPosZ) > 30)
								continue;

							MAPWORD mapDamage;
							mapDamage.clear();
							INT nAggroHP = 0;
							INT nAggroMP = 0;
							BYTE bAggroHit = bHit;

							pAGGROMON->CalcDamage(
								dwAttack,
								bAtkerType,
								mapDamage,
								nAggroHP,
								nAggroMP,
								dwPysMinPower,
								dwPysMaxPower,
								dwMgMinPower,
								dwMgMaxPower,
								bAggroHit,
								m_pInstanceSkill,
								SDT_AI);

							DWORD dwAggroValue = max(nAggroHP, nAggroMP);
							dwAggroValue += pTemp->Calculate(bSkillLevel, i, dwAggroValue);
							pAGGROMON->SetAggro(
								dwHostID,
								dwAttack,
								bAtkerType,
								GetAttackCountry(bAttackCountry, bAttackAidCountry),
								bAttackClass,
								0, 0,
								dwAggroValue,
								FALSE);
						}
					}
					break;
				case SDT_AGGRODAMAGE:
					if(m_bType == OT_MON)
					{
						CTMonster * pMon = (CTMonster *)this;

						MAPWORD mapDamage;
                        mapDamage.clear();
						INT nAggroHP = 0;
						INT nAggroMP = 0;
						BYTE bAggroHit = bHit;

						pMon->CalcDamage(
                            dwAttack,
                            bAtkerType,
                            mapDamage,
                            nAggroHP,
                            nAggroMP,
                            dwPysMinPower,
                            dwPysMaxPower,
                            dwMgMinPower,
                            dwMgMaxPower,
                            bAggroHit,
                            m_pInstanceSkill,
							SDT_AI);

						DWORD dwAggroValue = max(nAggroHP, nAggroMP);
						dwAggroValue += pTemp->Calculate(bSkillLevel, i, dwAggroValue);

						pMon->SetAggro(
							dwHostID,
							dwAttack,
							bAtkerType,
							GetAttackCountry(bAttackCountry, bAttackAidCountry),
							bAttackClass,
							0, 0,
							dwAggroValue,
							TRUE);
					}
					break;
				case SDT_RUNAWAY: // 도망
					if(m_bType == OT_MON)
					{
						CTMonster * pMon = (CTMonster *)this;
						pMon->OnEvent( AT_HELP, 0, 0, 0, 0);
					}
					break;
				case SDT_CHANGETARGET: // 타겟변경
					{
						if(m_bType != OT_MON)
							return PERFORM_FAIL;

						CTMonster * pMon = (CTMonster *)this;
						MAPTAGGRO::iterator it;
						DWORD dwMaxAggro = 0;
						TAGGRO aggro;
						for(it=pMon->m_mapAggro.begin(); it!=pMon->m_mapAggro.end(); it++)
						{
							if(dwMaxAggro < (*it).second.m_dwAggro)
							{
								dwMaxAggro = (*it).second.m_dwAggro;
								aggro.Copy((*it).second);
							}
						}
						if(aggro.m_dwHostID &&
							aggro.m_bCountry != pMon->GetWarCountry())
							pMon->OnEvent(AT_DEFEND, 0, aggro.m_dwHostID, aggro.m_dwObjID, aggro.m_bObjType);
					}
					break;
				case SDT_LEAD: // 유인
					{
						if(m_bType != OT_MON)
							return PERFORM_FAIL;

						CTMonster * pMon = (CTMonster *)this;
						if(bAtkerType == OT_PC &&
							CanFight(bAtkerType, bAttackCountry, bAttackAidCountry))
							pMon->OnEvent(AT_DEFEND, 0, dwAttack, dwAttack, OT_PC);
					}
					break;
				case SDT_TEMPT: // 소환유혹
					{
						if(m_bType != OT_MON)
							return PERFORM_FAIL;

						CTMonster * pMon = (CTMonster *)this;
						if(pMon->m_pMON->m_bTame && pMon->m_bLevel <= bAttackerLevel )
							_AtlModule.SendMW_MONTEMPT_ACK(dwAttack, pMon->m_pMON->m_wID);
						else
							return PERFORM_FAIL;
					}
					break;
				case SDT_AGGRO:
					{
						if(m_bType != OT_MON)
							return PERFORM_FAIL;

						CTMonster * pMon = (CTMonster *)this;
						DWORD dwAggro = pMon->FindAggro(dwAttack, bAtkerType);
						pMon->SetAggro(
							dwHostID,
							dwAttack,
							bAtkerType,
							GetAttackCountry(bAttackCountry, bAttackAidCountry),
							bAttackClass,
							0, 0,
							pTemp->Calculate(bSkillLevel, i, dwAggro),
							TRUE);
					}
					break;
				case SDT_RESETAGGRO:
					{
						if(m_bType != OT_MON)
							return PERFORM_FAIL;

						CTMonster * pMon = (CTMonster *)this;
						pMon->LeaveAggro(dwHostID, dwAttack, bAtkerType);
					}
					break;
				case SDT_AGGROTRANS:
					{
						if(m_bType != OT_MON)
							return PERFORM_FAIL;

						CTMonster * pMon = (CTMonster *)this;

						if(bAtkerType != OT_PC)
							break;

						DWORD dwAggro = pMon->FindAggro(dwAttack, bAtkerType);
						if(dwAggro)
						{
							CTPlayer * pHost = pMon->FindHost(dwAttack);
							if(	!pHost->m_pMAP ||
								!pHost->m_bMain)
								break;

							CTRecallMon * pRecall = pHost->FindMainRecall();
							if(!pRecall)
								break;

							pMon->AddAggro(dwHostID, pRecall->m_dwID, pRecall->m_bType, pRecall->GetWarCountry(), dwAggro);
							pMon->LeaveAggro(dwHostID, dwAttack, bAtkerType);
						}
					}
				}
			}
			break;
		case SDT_ITEM:
			break;
		}
	}

	return bResult;
}

CTSkill * CTObjBase::FindRemainSkill(WORD wID)
{
	for(DWORD i=0; i<m_vRemainSkill.size(); i++)
		if(m_vRemainSkill[i]->m_pTSKILL->m_wID == wID)
			return m_vRemainSkill[i];
	return NULL;
}

CTSkill * CTObjBase::RemainSkill(CTSkill * pTemp, DWORD dwAtkID)
{
	if(!pTemp->m_pTSKILL->IsRemainType())
		return NULL;

	CTSkill * pSkill = FindRemainSkill(pTemp->m_pTSKILL->m_wID);
	if(pSkill)
		return NULL;
	else
		pSkill = FindTSkill(pTemp->m_pTSKILL->m_wID);

	if(!pSkill)
		return NULL;

	for(DWORD i=0; i<m_vRemainSkill.size(); i++)
	{
		if(m_vRemainSkill[i]->m_pTSKILL->m_bPriority == pTemp->m_pTSKILL->m_bPriority)
		{
			m_vRemainSkill.erase(m_vRemainSkill.begin()+i);
			break;
		}
	}

	return pSkill;
}

CTSkill * CTObjBase::MaintainSkill(CTSkill * pTemp,
                              DWORD dwRemainTick,
                              DWORD dwHostID,
                              BYTE bHostType,
                              DWORD dwAttackID,
                              BYTE bAttackType,								   
                              DWORD dwPsyMinPower,
                              DWORD dwPsyMaxPower,
                              DWORD dwMgMinPower,
                              DWORD dwMgMaxPower,
                              BYTE bCurseProb,
                              BYTE bCanSelect,
                              BYTE bAttackCountry,
                              BYTE bHit,
                              WORD wAttackLevel,
                              BYTE bAttackerLevel,
                              FLOAT fPosX,
                              FLOAT fPosY,
                              FLOAT fPosZ)
{
	if(!pTemp->m_pTSKILL->IsMaintainType())
		return NULL;

	if(m_bType == OT_SELF && 
		(bAttackType != m_bType || dwAttackID != m_dwID))
		return NULL;

	CTSkill * pMaintainSkill;

	BYTE bRet = UpdateBuffSkill(pTemp, dwAttackID, bAttackType);
	if(bRet)
	{
		pMaintainSkill = new CTSkill();
		pMaintainSkill->m_pTSKILL = pTemp->m_pTSKILL;
		pMaintainSkill->m_bLevel = pTemp->m_bLevel;

		pMaintainSkill->SetMaintain(
			dwAttackID,
			bAttackType,
			dwHostID,
			bHostType,
			bHit,
			wAttackLevel,
			bAttackerLevel,
			dwPsyMinPower,
			dwPsyMaxPower,
			dwMgMinPower,
			dwMgMaxPower,
			bCanSelect,
			bAttackCountry,
			fPosX,
			fPosY,
			fPosZ);

		if(dwRemainTick)
			pMaintainSkill->SetLoopEndTick(_AtlModule.m_dwTick, dwRemainTick);
		else
			pMaintainSkill->SetEndTick(_AtlModule.m_dwTick);
	}
	else
		pMaintainSkill = NULL;

	if(m_bType == OT_MON &&
		pMaintainSkill &&
		pMaintainSkill->m_pTSKILL->IsSilence())
	{
		CTMonster * pMon = (CTMonster *)this;
		if(pMon->m_pNextSkill)
			pMaintainSkill->m_wSilencedSkill = pMon->m_pNextSkill->m_pTSKILL->m_wID;
	}

	return pMaintainSkill;
}

BYTE CTObjBase::HaveDisWeapon()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_DISWEAPON)
				return TRUE;
		}
	}

	return FALSE;
}

BYTE CTObjBase::HaveDisDefend()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_DISDEFEND)
				return TRUE;
		}
	}

	return FALSE;
}

BYTE CTObjBase::HaveMirror()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		if(pSkill->m_pTSKILL->IsMirrorType())
			return TRUE;
	}

	return FALSE;
}

BYTE CTObjBase::HaveStopRecover()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_STOPRECOVER)
				return TRUE;
		}
	}

	return FALSE;
}
BYTE CTObjBase::HaveMask()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_MASK)
				return TRUE;
		}
	}

	return FALSE;
}
///////////////////////////////////////////////////////////////////////
// 변장
BYTE CTObjBase::HaveDisguiseBuff()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_TRANS &&
				(pData->m_bExec == SDT_TRANS_DISGUISE_D ||
				pData->m_bExec == SDT_TRANS_DISGUISE_C ||
				pData->m_bExec == SDT_TRANS_DISGUISE_B))
				return pData->m_bExec;
		}
	}

	return 0;
}
BYTE CTObjBase::GetCountry()
{
	if(HaveDisguiseBuff())
	{
		for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
		{
			CTSkill * pSkill = m_vMaintainSkill[i];
			for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
			{
				LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
				if(pData->m_bType == SDT_TRANS &&
					pData->m_bExec == SDT_TRANS_DISGUISE_D)
					return TCONTRY_D;

				if(pData->m_bType == SDT_TRANS &&
					pData->m_bExec == SDT_TRANS_DISGUISE_C)
					return TCONTRY_C;

				if(pData->m_bType == SDT_TRANS &&
					pData->m_bExec == SDT_TRANS_DISGUISE_B)
					return TCONTRY_B;
			}
		}
	}

	return m_bCountry;
}
///////////////////////////////////////////////////////////////////////
DWORD CTObjBase::DistributeSkill(DWORD dwDamageHP)
{
	if(m_bType != OT_PC || !m_pMAP)
		return 0;

	CTPlayer * pPlayer = (CTPlayer *)this;
	CTRecallMon * pRecall = pPlayer->FindMainRecall();
	if(!pRecall)
		return 0;

	CTSkill * pDistribute = NULL;
	for(DWORD i=0; i<pRecall->m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = pRecall->m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_DISTRIBUTE)
				pDistribute = pSkill;
		}
	}

	DWORD dwMonDg = 0;
	if(pDistribute)
	{
		dwMonDg = dwDamageHP * pDistribute->GetValue(SDT_STATUS, SDT_STATUS_DISTRIBUTE) / 100;
		if(dwMonDg)
		{
			dwMonDg = pRecall->OnDamage(
				pPlayer->m_dwID,
				pPlayer->m_dwID,
				pPlayer->m_bType,
				pPlayer->m_bCountry,
				pPlayer->m_bAidCountry,
				TRUE,
				pPlayer->m_wPartyID,
				dwMonDg,
				0);

			DWORD dwMaxHP = pRecall->GetMaxHP();
			DWORD dwMaxMP = pRecall->GetMaxMP();

			VPLAYER vPLAYERS;
			vPLAYERS.clear();

			m_pMAP->GetNeighbor(
				&vPLAYERS,
				m_fPosX,
				m_fPosZ);

			while(!vPLAYERS.empty())
			{
				CTPlayer *pChar = vPLAYERS.back();
				pChar->SendCS_HPMP_ACK(
					pRecall->m_dwID,
					pRecall->m_bType,
					pRecall->m_bLevel,
					dwMaxHP,
					pRecall->m_dwHP,
					dwMaxMP,
					pRecall->m_dwMP);

				vPLAYERS.pop_back();
			}

			vPLAYERS.clear();

			if(pRecall->m_dwHP == 0)
				_AtlModule.SendMW_RECALLMONDEL_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, pRecall->m_dwID);
		}
	}

	return dwMonDg;
}

int CTObjBase::ApplyEffectionBuff(DWORD &dwValue)
{
	int nInc = 0;
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_MAGIC)
			{
				if(pData->m_bInc == SVI_INCREASE)
					nInc += dwValue * pSkill->GetValue(pData) / 100;
				else
					nInc -= dwValue * pSkill->GetValue(pData) / 100;
			}
		}
	}

	dwValue += nInc;
	return nInc;
}
void CTObjBase::EraseHideBuff()
{
	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
		BYTE bErase = FALSE;
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bInc == SVI_INCREASE &&
				(pData->m_bExec == SDT_STATUS_HIDE ||
				pData->m_bExec == SDT_STATUS_CLARITY ||
				pData->m_bExec == SDT_STATUS_STANDHIDE))
			{
				EraseMaintainSkill(pSkill, i);
				bErase = TRUE;
				break;
			}
		}

		if(!bErase)	i++;
	}
}

void CTObjBase::EraseStandHideBuff()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_STANDHIDE)
			{
				EraseMaintainSkill(pSkill, i);
				return;
			}
		}
	}
}

void CTObjBase::EraseOtherSelf()
{
	if(m_bType != OT_PC)
		return;

	CTPlayer * pPlayer = (CTPlayer *)this;
	CTRecallMon * pSelf = pPlayer->FindMainRecall();
	if(pSelf && !pSelf->m_pMON->m_wID)
		_AtlModule.SendMW_RECALLMONDEL_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, pSelf->m_dwID);
}

void CTObjBase::CancelBlockBuff()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		if(pSkill->m_pTSKILL->IsBlockType())
		{
			EraseMaintainSkill(pSkill, i);
			break;
		}
	}
}

void CTObjBase::EraseItemUpgradeBuff()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_ITEMUPGRADE)
			{
				EraseMaintainSkill(pSkill, i);
				return;
			}
		}
	}
}

BYTE CTObjBase::SetTransAttack(WORD wMonID)
{
	if(wMonID)
	{
		MAPTMONSTERTEMP::iterator itMon = _AtlModule.m_mapTMONSTER.find(wMonID);
		if(itMon!=_AtlModule.m_mapTMONSTER.end())
		{
			m_bCanAttack = (*itMon).second->m_bCanAttack;
			if(m_bType == OT_MON && !m_bCanAttack)
				((CTMonster *)this)->Blind();

			ResetSize(wMonID);
		}
	}
	else
	{
		switch(m_bType)
		{
		case OT_PC:
			m_bCanAttack = TRUE;
			break;
		case OT_MON:
		case OT_RECALL:
		case OT_SELF:
			m_bCanAttack = ((CTMonster *)this)->m_pMON->m_bCanAttack;
			break;
		default:
			break;
		}

		ResetSize(0);
	}

	return TRUE;
}

BYTE CTObjBase::ForceMaintain(WORD wSkillID,
							  DWORD dwHostID,
							  BYTE bHostType,
							  DWORD dwAttack,
							  BYTE bAtkType,
							  DWORD dwRemainTick)
{
	if(!wSkillID || !m_pMAP)
		return FALSE;

	CTSkillTemp * pTemp = _AtlModule.FindTSkill(wSkillID);
	if(!pTemp)
		return FALSE;
	else
	{
		CTSkill * pSkill = new CTSkill();
		pSkill->m_bLevel = 1;
		pSkill->m_pTSKILL = pTemp;

		CTSkill * pMaintain = MaintainSkill(
			pSkill,
			dwRemainTick,
			dwHostID,
			bHostType,
			dwAttack,
			bAtkType,
			0,0,0,0,0,
			TRUE,
			GetWarCountry(),
			0,0,0,
			m_fPosX,
			m_fPosY,
			m_fPosZ);

		if(pMaintain)
		{
			if(!PushMaintainSkill(pMaintain))
			{
				delete pSkill;
				return FALSE;
			}

			VPLAYER vPlayers;
			vPlayers.clear();

			m_pMAP->GetNeighbor(
				&vPlayers,
				m_fPosX,
				m_fPosZ);

			while(!vPlayers.empty())
			{
				CTPlayer * pChar = vPlayers.back();

				pChar->SendCS_DEFEND_ACK(
					dwAttack,
					m_dwID,
					bAtkType,
					m_bType,
					0, 0,
					0, 0,
					TRUE,
					dwRemainTick ? dwRemainTick : pSkill->GetMaintainTick(),
					0,
					HT_NORMAL,
					1,
					m_bLevel,
					0,0,0,0,
					TRUE,
					FALSE,
					m_bCountry,
					m_bAidCountry,
					wSkillID,
					1,
					0,
					TRUE,
					m_fPosX,
					m_fPosY,
					m_fPosZ,
					m_fPosX,
					m_fPosY,
					m_fPosZ,
					NULL);

				vPlayers.pop_back();
			}

			vPlayers.clear();
		}

		delete pSkill;
	}

	return TRUE;
}

BYTE CTObjBase::HaveExcptMagic()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_EXCEPTMAGIC)
				return TRUE;
		}
	}

	return FALSE;
}

BYTE CTObjBase::HaveExcptPhysic()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_EXCEPTPHYSIC)
				return TRUE;
		}
	}

	return FALSE;
}

void CTObjBase::ResetSize(WORD wMonTempID)
{
}

BYTE CTObjBase::IsTrans()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		if(pSkill->m_pTSKILL->IsTrans())
			return TRUE;
	}

	return FALSE;
}

BYTE CTObjBase::IsActionBlock()
{
	if(m_bStatus == OS_DEAD)
		return TRUE;

	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD dwData=0; dwData<pSkill->m_pTSKILL->m_vData.size(); dwData++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[dwData];
			if(pData->m_bType == SDT_TRANS)
				return TRUE;
			else if(pData->m_bType == SDT_STATUS &&
                ( pData->m_bExec == SDT_STATUS_BLOCK ||
                pData->m_bExec == SDT_STATUS_HOLD))
                return TRUE;
		}
	}

	return FALSE;
}

void CTObjBase::SkillUse(CTSkill * pSkill, DWORD dwTick)
{
	DWORD dwSR = GetAtkSpeedRate(pSkill->m_pTSKILL->m_bSpeedApply);
	pSkill->Use(
		SDELAY_SKILL,
		dwTick,
		GetAtkSpeed(pSkill->m_pTSKILL->m_bSpeedApply),
		dwSR);

	if(pSkill->m_pTSKILL->m_dwKindDelay)
	{
		MAPTSKILL::iterator it;
		for(it=m_mapTSKILL.begin(); it!=m_mapTSKILL.end(); it++)
		{
			if(pSkill->m_pTSKILL->m_bKind == (*it).second->m_pTSKILL->m_bKind)
				(*it).second->Use(SDELAY_KIND, dwTick, pSkill->m_pTSKILL->m_dwKindDelay, dwSR);
		}
	}
}

void CTObjBase::CancelSkill(WORD wSkillID, DWORD dwTick)
{
	CTSkill * pSkill = FindTSkill(wSkillID);
	if(pSkill)
	{
		MAPTSKILL::iterator it;
		for(it=m_mapTSKILL.begin(); it!=m_mapTSKILL.end(); it++)
		{
			if(pSkill->m_pTSKILL->m_bKind == (*it).second->m_pTSKILL->m_bKind &&
				(*it).second->GetReuseRemainTick(dwTick) <= pSkill->m_pTSKILL->m_dwKindDelay)
			{
				(*it).second->m_dwUseTick = 0;
				(*it).second->m_dwDelayTick = 0;
			}
		}

		pSkill->m_dwUseTick = 0;
		pSkill->m_dwDelayTick = 0;
	}
}

void CTObjBase::CalcAfterMath(FLOAT &fValue)
{
	fValue -= fValue*m_aftermath.m_fStatDec/100;
}

BYTE CTObjBase::PushMaintainSkill(CTSkill * pSkill)
{
	if(!pSkill)
		return FALSE;

	if(!pSkill->m_pTSKILL->m_bStatic && m_bStatus == OS_DEAD)
		return FALSE;

	m_vMaintainSkill.push_back(pSkill);
	return TRUE;
}

void CTObjBase::EraseBuffByDefend(CTSkillTemp * pCurSkill)
{
	if(pCurSkill->IsPosture())
		return;

	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];

		if((pSkill->m_pTSKILL->m_bEraseAct & BEA_DEFEND) &&
			(!pSkill->m_pTSKILL->m_bIsHideSkill ||
				(pSkill->m_pTSKILL->m_bIsHideSkill &&
				(pCurSkill->m_bEraseHide & BEA_DEFEND))))
			EraseMaintainSkill(pSkill, i);
		else
			i++;
/*
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if((pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_HYPNOSIS) ||
				(pData->m_bType == SDT_TRANS &&
				pData->m_bExec == SDT_TRANS_CHIPMUNK &&
				pSkill->m_pTSKILL->m_wID+1 != wExcept))
			{
				EraseMaintainSkill(pSkill, i);
				bErase = TRUE;
				break;
			}
		}

		if(!bErase)	i++;
*/
	}
}

void CTObjBase::EraseBuffByAttack(CTSkillTemp * pCurSkill)
{
	if(pCurSkill->IsPosture())
		return;

	if(pCurSkill->IsRecall())
		return;

	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];

		if((pSkill->m_pTSKILL->m_bEraseAct & BEA_ATTACK) &&
			(!pSkill->m_pTSKILL->m_bIsHideSkill ||
				(pSkill->m_pTSKILL->m_bIsHideSkill &&
				(pCurSkill->m_bEraseHide & BEA_ATTACK))))
			EraseMaintainSkill(pSkill, i);
		else
			i++;
/*
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_NOTSELECT)
			{
				EraseMaintainSkill(pSkill, i);
				bErase = TRUE;
				break;
			}
		}

		if(!bErase)	i++;
*/
	}
}

void CTObjBase::EraseBuffByRide()
{
	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
		BYTE bErase = FALSE;
		CTSkill * pSkill = m_vMaintainSkill[i];
		if(pSkill->m_pTSKILL->m_bEraseAct & BEA_RIDE)
			EraseMaintainSkill(pSkill, i);
		else
			i++;
	}
}

BYTE CTObjBase::DeleteSkillItem(WORD wItemID)
{
	if(!wItemID)
		return TRUE;

	if(m_bType == OT_PC)
	{
		CTPlayer * pMe = (CTPlayer *)this;
		if(pMe->m_bStore || pMe->m_dealItem.m_bStatus >= DEAL_START)
		return EC_NOERROR;
	}

	MAPTINVEN::iterator it;
	MAPTITEM::iterator itIT;
	for(it=m_mapTINVEN.begin(); it!=m_mapTINVEN.end(); it++)
	{
		for(itIT=(*it).second->m_mapTITEM.begin(); itIT!=(*it).second->m_mapTITEM.end(); itIT++)
		{
			if((*itIT).second->m_pTITEM->m_wItemID == wItemID)
			{
				_AtlModule.UseItem(
					(CTPlayer *)this,
					(*it).second,
					(*itIT).second,
					1);

				((CTPlayer *)this)->SendCS_MOVEITEM_ACK(MI_SUCCESS);
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CTObjBase::EraseDieSkillBuff()
{
	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
        if(m_vMaintainSkill[i]->m_pTSKILL->IsDie())
		{
			if(m_pMAP)
			{
				VPLAYER vPLAYERS;
				vPLAYERS.clear();
				m_pMAP->GetNeighbor(
					&vPLAYERS,
					m_fPosX,
					m_fPosZ);

				for(DWORD nPlayer=0; nPlayer < vPLAYERS.size(); nPlayer++)
				{
					vPLAYERS[nPlayer]->SendCS_SKILLEND_ACK(
						m_dwID,
						m_bType,
						m_vMaintainSkill[i]->m_pTSKILL->m_wID);
				}
			}

			delete m_vMaintainSkill[i];
			m_vMaintainSkill.erase(m_vMaintainSkill.begin()+i);
		}
		else
			i++;
	}
}

BYTE CTObjBase::HaveDieBuff()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
		if(m_vMaintainSkill[i]->m_pTSKILL->IsDie())
			return TRUE;

	return FALSE;
}

WORD CTObjBase::HaveItemProbBuff(BYTE bType)  
{
	for(DWORD i=0; i < m_vMaintainSkill.size(); i++)
	{
		CTSkill* pSkill = m_vMaintainSkill[i];
		WORD wProb = pSkill->m_pTSKILL->GetItemProbBuff(bType);
		if(wProb)
			return wProb;
	}

	return 0;
}

void CTObjBase::EraseItemProbBuff(BYTE bType)  
{
	for(DWORD i=0; i < m_vMaintainSkill.size(); i++)
	{
		CTSkill* pSkill = m_vMaintainSkill[i];
		for(DWORD j = 0; j < pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == bType)
			{
				EraseMaintainSkill(pSkill,i);
				return;
			}
		}
	}	
}

DWORD CTObjBase::GetGuild()
{
	return m_dwTacticsID ? m_dwTacticsID : m_dwGuildID;
}

void CTObjBase::DeletePositiveMaintainSkill()
{
	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
		if(m_vMaintainSkill[i]->m_pTSKILL->IsPositive())
			EraseMaintainSkill(m_vMaintainSkill[i], i);
		else
			i++;
	}
}

void CTObjBase::DeleteNegativeMaintainSkill()
{
	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
		if(m_vMaintainSkill[i]->m_pTSKILL->m_bPositive == SPT_NEGATIVE)
			EraseMaintainSkill(m_vMaintainSkill[i], i);
		else
			i++;
	}
}

INT CTObjBase::HaveItemDropBuff(BYTE bItemProb)  
{
	for(DWORD i=0; i < m_vMaintainSkill.size(); i++)
	{
		CTSkill* pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS && pData->m_bExec == SDT_STATUS_ITEMDROP)
				return pSkill->m_pTSKILL->Calculate(pSkill->m_bLevel, j, bItemProb);
		}
	}

	return 0;
}

WORD CTObjBase::GetMagicDropBuff()
{
	for(DWORD i=0; i < m_vMaintainSkill.size(); i++)
	{
		CTSkill* pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS && pData->m_bExec == SDT_STATUS_MAGICDROP)
				return pData->m_wValue;
		}
	}

	return 0;
}

WORD CTObjBase::GetGainExpBuff()
{
	for(DWORD i=0; i < m_vMaintainSkill.size(); i++)
	{
		CTSkill* pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS && pData->m_bExec == SDT_STATUS_GAINEXP)
				return pData->m_wValue;
		}
	}

	return 0;
}

BYTE CTObjBase::IsInCastle()
{
	if(!m_pMAP || !m_pMAP->m_pCastle)
		return FALSE;

	return TRUE;
}

WORD CTObjBase::MakeItemMagicValue(INT nBase, WORD wBound)
{
	if(!wBound)
		return 0;

	BYTE bLevel_Const = 10;
	BYTE bRate_Const = 3;
	INT nRand1 = rand();
	INT nRand2 = rand();
	INT nRand3 = rand();

	FLOAT fRateX = GetFormulaRateX(FTYPE_1ST);
	INT nOpLv = min( 80, max( 1, nBase - nRand1 % bLevel_Const) + max( 0, (nRand2 % 1000) - 989));
	WORD wValue = 1 + (nRand3 % (WORD)( wBound * ((100 - bRate_Const) * pow( fRateX, nOpLv) / pow( fRateX, 80) + bRate_Const) / 100));
	return wValue;
}

void CTObjBase::ReleaseMaintain(BYTE bNotify)
{
	for(DWORD i=0; i<m_vMaintainSkill.size();)
	{
		if(m_vMaintainSkill[i]->m_pTSKILL->m_bStatic)
			i++;
		else
		{
			if(bNotify)
				EraseMaintainSkill(m_vMaintainSkill[i], i);
			else
			{
				delete m_vMaintainSkill[i];
				m_vMaintainSkill.erase(m_vMaintainSkill.begin() + i);
			}
		}
	}
}

BYTE CTObjBase::GetMonExpRate()
{
	BYTE bRate = 100;

	if(_AtlModule.GetNation() != NATION_KOREA)
	{
		BYTE bRaise = 39;
		BYTE bMax = 64;
		BYTE bMin = 55;
		bRate = bMax - min( bRaise, m_bLevel) * WORD(bMax - bMin) / bRaise;
	}

	return bRate;
}

BYTE CTObjBase::GetWarCountry()
{
	BYTE bCountry = m_bCountry;

	if(m_bAidCountry != TCONTRY_N)
		bCountry = m_bAidCountry;

	return bCountry;
}

BYTE CTObjBase::CanFight(BYTE bType, BYTE bCountry, BYTE bAidCountry)
{
	if(GetAttackCountry(bCountry, bAidCountry) == GetWarCountry())
		return FALSE;

	if(m_bType != OT_MON && bType != OT_MON)
	{
		if(m_bCountry == TCONTRY_PEACE || bCountry == TCONTRY_PEACE)
			return FALSE;
	}

	return TRUE;
}

