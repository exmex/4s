#include "StdAfx.h"
#include ".\trecallmon.h"

CTRecallMon::CTRecallMon(void)
{
	m_wPetID = 0;
	m_strName = NAME_NULL;
	m_bMain = TRUE;
	m_bType = OT_RECALL;
	m_dwRecallTick = 0;
	m_dwDurationTick = 0;

	m_bAtkLevel = 1;
	m_bAtkSkillLevel = 1;
}

CTRecallMon::~CTRecallMon(void)
{
}

DWORD CTRecallMon::GetLifeTick(DWORD dwTick)
{
	if(dwTick - m_dwRecallTick >= m_dwDurationTick)
		return 0;
	else
        return m_dwDurationTick - (dwTick - m_dwRecallTick);
}

void CTRecallMon::Recover(DWORD dwTick)
{
	CTObjBase::Recover(dwTick);
}

DWORD CTRecallMon::OnDamage(DWORD dwHostID,
							DWORD dwAttackID,
							BYTE bAttackType,
							BYTE bAttackCountry,
							BYTE bAttackAidCountry,
							BYTE bAtkCanSelect,
							WORD wPartyID,
							int nDamageHP,
							int nDamageMP)
{
	if(m_bMode != MT_BATTLE)
		ChgMode(MT_BATTLE);

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

void CTRecallMon::OnDie( DWORD dwAttackID, BYTE bObjectType, WORD wTempMonID) 
{
	CTPlayer *pHOST = FindHost(m_dwHostID);
	if(m_pMAP && pHOST)
	{
		// 말타기 해제
		if(pHOST->m_dwRiding == m_dwID)
			pHOST->PetRiding(0);

		VTMONSTER vMONS;
		vMONS.clear();

		m_pMAP->GetNeighbor(
			&vMONS,
			m_fPosX,
			m_fPosZ);

		while(!vMONS.empty())
		{
			CTMonster *pAGGROMON = vMONS.back();

			MAPTAGGRO::iterator it = pAGGROMON->m_mapAggro.find(MAKEINT64(m_dwID, m_bType));
			if(it!=pAGGROMON->m_mapAggro.end())
			{
				pAGGROMON->AddAggro(m_dwHostID, m_dwHostID, OT_PC, pHOST->GetWarCountry(), (*it).second.m_dwAggro);
				pAGGROMON->LeaveAggro(m_dwHostID, m_dwID, m_bType);
			}

			vMONS.pop_back();
		}

		vMONS.clear();
	}

	CTObjBase::OnDie(dwAttackID,bObjectType,wTempMonID); 
}

void CTRecallMon::Defend(
		CTSkillTemp * pTemp,
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

DWORD CTRecallMon::GetMaxHP()
{
	DWORD dwMaxHP = m_pMON->m_wID ? m_pATTR->m_dwMaxHP : m_dwMaxHP;
	CalcAbilityValue(dwMaxHP, MTYPE_MHP);
	return dwMaxHP;
}
DWORD CTRecallMon::GetMaxMP()
{
	DWORD dwMaxMP = m_pMON->m_wID ? m_pATTR->m_dwMaxMP : m_dwMaxMP;
	CalcAbilityValue(dwMaxMP, MTYPE_MMP);
	return dwMaxMP;
}
DWORD CTRecallMon::GetMaxAP(BYTE bArrow)
{
	CTPlayer * pPlayer = FindHost(m_dwHostID);

	DWORD dwAP;
	if(bArrow)
	{
		dwAP = m_pATTR->m_wLAP;
		if(!HaveDisWeapon())
		{
			dwAP += m_pATTR->m_wMaxWAP;
			if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
				dwAP += DWORD(max(pPlayer->CalcItemAbility(ABILITY_MAXLAP), pPlayer->CalcItemAbility(ABILITY_MAXMAP)) * RECALL_ITEMABILITY_RATE);
		}

		CalcAbilityValue(dwAP, MTYPE_LAP);
	}
	else
	{
        dwAP = m_pATTR->m_wAP;
		if(!HaveDisWeapon())
		{
			dwAP += m_pATTR->m_wMaxWAP;
			if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
				dwAP += DWORD(max(pPlayer->CalcItemAbility(ABILITY_MAXAP), pPlayer->CalcItemAbility(ABILITY_MAXMAP)) * RECALL_ITEMABILITY_RATE);
		}
		CalcAbilityValue(dwAP, MTYPE_PAP);
	}
	return dwAP;
}
DWORD CTRecallMon::GetMinAP(BYTE bArrow)
{
	CTPlayer * pPlayer = FindHost(m_dwHostID);

	DWORD dwAP;
	if(bArrow)
	{
		dwAP = m_pATTR->m_wLAP;
		if(!HaveDisWeapon())
		{
			dwAP += m_pATTR->m_wMinWAP;
			if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
				dwAP += DWORD(max(pPlayer->CalcItemAbility(ABILITY_MINLAP), pPlayer->CalcItemAbility(ABILITY_MINMAP)) * RECALL_ITEMABILITY_RATE);
		}

		CalcAbilityValue(dwAP, MTYPE_LAP);
	}
	else
	{
        dwAP = m_pATTR->m_wAP;
		if(!HaveDisWeapon())
		{
			dwAP += m_pATTR->m_wMinWAP;
			if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
				dwAP += DWORD(max(pPlayer->CalcItemAbility(ABILITY_MINAP), pPlayer->CalcItemAbility(ABILITY_MINMAP)) * RECALL_ITEMABILITY_RATE);
		}

		CalcAbilityValue(dwAP, MTYPE_PAP);
	}
	return dwAP;
}
DWORD CTRecallMon::GetMinMagicAP()
{
	CTPlayer * pPlayer = FindHost(m_dwHostID);

	DWORD dwAP = m_pATTR->m_wMAP;
	if(!HaveDisWeapon())
	{
		dwAP += m_pATTR->m_wMinWAP;
		if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
			dwAP += DWORD(max(pPlayer->CalcItemAbility(ABILITY_MINMAP), pPlayer->CalcItemAbility(ABILITY_MINAP)) * RECALL_ITEMABILITY_RATE);
	}
	CalcAbilityValue(dwAP, MTYPE_MAP);

	return dwAP;
}
DWORD CTRecallMon::GetMaxMagicAP()
{
	CTPlayer * pPlayer = FindHost(m_dwHostID);

	DWORD dwAP = m_pATTR->m_wMAP;
	if(!HaveDisWeapon())
	{
		dwAP += m_pATTR->m_wMaxWAP;
		if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
			dwAP += DWORD(max(pPlayer->CalcItemAbility(ABILITY_MAXMAP), pPlayer->CalcItemAbility(ABILITY_MAXAP)) * RECALL_ITEMABILITY_RATE);
	}

	CalcAbilityValue(dwAP, MTYPE_MAP);

	return dwAP;
}

DWORD CTRecallMon::GetDefendPower()
{
	CTPlayer * pPlayer = FindHost(m_dwHostID);

	DWORD dwDP = m_pATTR->m_wDP;
	if(!HaveDisDefend())
	{
		dwDP += m_pATTR->m_wWDP;
		if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
			dwDP += DWORD(max(pPlayer->CalcItemAbility(ABILITY_PDP), pPlayer->CalcItemAbility(ABILITY_MDP)) * RECALL_ITEMABILITY_RATE);
	}
	CalcAbilityValue(dwDP, MTYPE_PDP);
	return dwDP;
}
DWORD CTRecallMon::GetMagicDefPower()
{
	CTPlayer * pPlayer = FindHost(m_dwHostID);

	DWORD dwMDP = m_pATTR->m_wMDP;
	if(!HaveDisDefend())
	{
		dwMDP += m_pATTR->m_wWDP;
		if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
			dwMDP += DWORD(max(pPlayer->CalcItemAbility(ABILITY_MDP), pPlayer->CalcItemAbility(ABILITY_PDP)) * RECALL_ITEMABILITY_RATE);
	}
	CalcAbilityValue(dwMDP, MTYPE_MDP);
	return dwMDP;
}
WORD CTRecallMon::GetAttackLevel()
{
	CTPlayer * pPlayer = FindHost(m_dwHostID);

	DWORD dwAL = m_pATTR->m_wAL;
	if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
		dwAL += pPlayer->CalcItemAbility(ABILITY_AL);

	CalcAbilityValue(dwAL, MTYPE_AL);
	return WORD(dwAL);
}
WORD CTRecallMon::GetDefendLevel()
{
	CTPlayer * pPlayer = FindHost(m_dwHostID);

	DWORD dwDL = m_pATTR->m_wDL;
	if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
		dwDL += pPlayer->CalcItemAbility(ABILITY_DL);

	CalcAbilityValue(dwDL, MTYPE_DL);
	return WORD(dwDL);
}
WORD CTRecallMon::GetMagicAtkLevel()
{
	CTPlayer * pPlayer = FindHost(m_dwHostID);

	DWORD dwAL = m_pATTR->m_wMAL;
	if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
		dwAL += pPlayer->CalcItemAbility(ABILITY_MAL);

	CalcAbilityValue(dwAL, MTYPE_MAL);
	return WORD(dwAL);
}
WORD CTRecallMon::GetMagicDefLevel()
{
	CTPlayer * pPlayer = FindHost(m_dwHostID);

	DWORD dwDL = m_pATTR->m_wMDL;
	if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
		dwDL += pPlayer->CalcItemAbility(ABILITY_MDL);

	CalcAbilityValue(dwDL, MTYPE_MDL);
	return WORD(dwDL);
}
BYTE CTRecallMon::GetCriticalPysProb()
{
	CTPlayer * pPlayer = FindHost(m_dwHostID);

	DWORD dwCR = m_pATTR->m_bCriticalPP;
	if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
		dwCR += pPlayer->CalcItemAbility(ABILITY_PCR);

	CalcAbilityValue(dwCR, MTYPE_CR);
	return BYTE(dwCR);
}
BYTE CTRecallMon::GetCriticalMagicProb()
{
	CTPlayer * pPlayer = FindHost(m_dwHostID);

	DWORD dwCR = m_pATTR->m_bCriticalMP;
	if(pPlayer && pPlayer->m_pMAP && pPlayer->m_bMain)
		dwCR += CalcItemAbility(ABILITY_MCR);

	CalcAbilityValue(dwCR, MTYPE_MCR);
	return BYTE(dwCR);
}
