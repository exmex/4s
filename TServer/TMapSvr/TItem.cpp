#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

CTItem::CTItem()
{
	m_mapTMAGIC.clear();
	m_pTITEM = NULL;
	m_pTITEMATTR = NULL;

	m_bInven = INVEN_NULL;
	m_dlID = 0;
	m_bItemID = 0;
	m_wItemID = 0;
	m_bLevel = 0;
	m_bCount = 0;
	m_bGLevel = 0;
	m_dwDuraMax = 0;
	m_dwDuraCur = 0;
	m_bRefineCur = 0;
	m_dEndTime = 0;
	m_dAlarmTime = 0;
	m_bGradeEffect = 0;
	m_bMagicDropLevel = 0;
	m_bStType = STORAGE_INVEN;
	m_dwStItemID = 0;
	m_dwOwnerID = 0;
	m_dwMoney = 0;

	m_bMaxRouting = 0;
	m_mapRoutingJoiner.clear();
	memset(m_dwExtValue, 0, sizeof(DWORD)*TMAGIC_MAX);
}

CTItem::~CTItem()
{
	MAPTMAGIC::iterator itTMAGIC;

	for( itTMAGIC = m_mapTMAGIC.begin(); itTMAGIC != m_mapTMAGIC.end(); itTMAGIC++)
		delete (*itTMAGIC).second;

	m_mapTMAGIC.clear();
}

BYTE CTItem::operator == ( CTItem& param)
{
	if( m_wItemID != param.m_wItemID ||
		m_mapTMAGIC.size() != param.m_mapTMAGIC.size() ||
		m_bLevel != param.m_bLevel || 
		m_pTITEMATTR != param.m_pTITEMATTR ||
		m_bGLevel != param.m_bGLevel ||
		m_dwDuraMax != param.m_dwDuraMax ||
		m_dwDuraCur != param.m_dwDuraCur ||
		m_bRefineCur != param.m_bRefineCur ||
		m_dEndTime != param.m_dEndTime)
		return FALSE;

	for(BYTE ev=0; ev<TMAGIC_MAX; ev++)
		if(m_dwExtValue[ev] != param.m_dwExtValue[ev])
			return FALSE;

	MAPTMAGIC::iterator itTMAGIC;

	for( itTMAGIC = m_mapTMAGIC.begin(); itTMAGIC != m_mapTMAGIC.end(); itTMAGIC++)
	{
		MAPTMAGIC::iterator finder = param.m_mapTMAGIC.find((*itTMAGIC).first);

		if( finder == param.m_mapTMAGIC.end() )
			return FALSE;

		if( (*itTMAGIC).second->m_wValue != (*finder).second->m_wValue )
			return FALSE;
	}

	return TRUE;
}

BYTE CTItem::operator != ( CTItem& param)
{
	return (*this) == param ? FALSE : TRUE;
}

void CTItem::Copy(CTItem * pItem, BYTE bGenID)
{
	if(!pItem)
		return;

	MAPTMAGIC::iterator itTMAGIC;

	for( itTMAGIC = m_mapTMAGIC.begin(); itTMAGIC != m_mapTMAGIC.end(); itTMAGIC++)
		delete (*itTMAGIC).second;
	m_mapTMAGIC.clear();

	if(bGenID)
		m_dlID = _AtlModule.GenItemID();
	else
		m_dlID = pItem->m_dlID;

	m_bItemID = pItem->m_bItemID;
	m_wItemID = pItem->m_wItemID;
	m_bLevel = pItem->m_bLevel;
	m_pTITEM = pItem->m_pTITEM;

	m_bCount = pItem->m_bCount;
	m_bGLevel = pItem->m_bGLevel;
	m_dwDuraMax = pItem->m_dwDuraMax;
	m_dwDuraCur = pItem->m_dwDuraCur;
	m_bRefineCur = pItem->m_bRefineCur;
	m_dEndTime = pItem->m_dEndTime;
	m_bGradeEffect = pItem->m_bGradeEffect;
	memcpy(m_dwExtValue, pItem->m_dwExtValue, sizeof(DWORD)*TMAGIC_MAX);

	for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
	{
		LPTMAGIC pTMAGIC = new TMAGIC();

		pTMAGIC->m_wValue = (*itTMAGIC).second->m_wValue;
		pTMAGIC->m_pMagic = (*itTMAGIC).second->m_pMagic;
		m_mapTMAGIC.insert( MAPTMAGIC::value_type( (*itTMAGIC).first, pTMAGIC));
	}

	_AtlModule.SetItemAttr(this, m_bLevel);
}

BYTE CTItem::SetGLevel(BYTE bLevel)
{
	if(m_pTITEM->m_bType == IT_GAMBLE ||
		( m_pTITEM->m_bCanGamble &&	rand() % 100 < m_pTITEM->m_wItemProb_G))
	{
		m_bGLevel = bLevel;
		return TRUE;
	}
	else
		return FALSE;
}

void CTItem::SetDuration(BYTE bMon)
{
	if(bMon)
		m_dwDuraMax = m_pTITEM->m_dwDuraMax * ((rand() % 90) + 11) / 100;
	else
		m_dwDuraMax = m_pTITEM->m_dwDuraMax;

	m_dwDuraCur = m_dwDuraMax;
	SetEndTime();
}

DWORD CTItem::DurationDec(BYTE bDel)
{
	DWORD dwDec = rand() % 3;
	if(bDel || m_dwDuraCur <= dwDec)
		m_dwDuraCur = 0;
	else
		m_dwDuraCur -= dwDec;

	return m_dwDuraCur;
}

DWORD CTItem::GetRepairCost()
{
	LPTLEVEL pLevel = _AtlModule.FindTLevel(GetPowerLevel());
	if(!pLevel)
		return 0;
	else
	{
		DWORD dwCost = (DWORD)max(1, pLevel->m_dwRepairCost * (m_dwDuraMax - m_dwDuraCur) * m_pTITEM->m_fPrice / m_dwDuraMax);
		MAPTMAGIC::iterator it = m_mapTMAGIC.find(MTYPE_REPCOST);
		if(it!=m_mapTMAGIC.end())
			dwCost = dwCost * (*it).second->m_wValue / 100;

		return dwCost;
	}
}

BYTE CTItem::GetWeaponPowerLevel()
{
	static FLOAT vPowerRate[IK_HP] = {
		0.0f,	// IK_NONE
		0.75f,	// IK_1HAND
		0.2f,	// IK_LHAND
		1.0f,	// IK_2HAND
		1.0f,	// IK_CAKRAM
		1.1f,	// IK_AX
		0.8f,	// IK_BOW
		1.0f,	// IK_ARBALEST
		0.8f,	// IK_MBAR
		1.0f,	// IK_MSTICK
		0.0f,	// IK_VAJRA
		0.0f,	// IK_MULTIVAJRA
		0.0f,	// IK_SHIELD
		0.0f,	// IK_CLOTH
		0.0f,	// IK_UNIFORM
		0.0f,	// IK_LLEATHER
		0.0f,	// IK_HLEATHER
		0.0f,	// IK_LARMOR
		0.0f,	// IK_HARMOR
		0.0f,	// IK_BACK
		0.0f,	// IK_NECK
		0.0f,	// IK_FINGER
		0.0f,	// IK_BALL
		0.0f,	// IK_PET
		0.0f,	// IK_ARROW
		0.0f};	// IK_SARROW

	if( m_pTITEM->m_bKind >= IK_HP || vPowerRate[m_pTITEM->m_bKind] == 0.0f )
		return BYTE(1);

	FLOAT fLevelConst = 1.0f / logf(_AtlModule.GetFormulaRateX(FTYPE_1ST));
	FLOAT fItemMinRate = _AtlModule.GetFormulaRateX(FTYPE_WEAPONPOWER);
	FLOAT fItemMaxRate = _AtlModule.GetFormulaRateY(FTYPE_WEAPONPOWER);

	DWORD dwMinAP = m_pTITEM->m_bType == IT_LONG ? GetMinLAP() : GetMinAP();
	DWORD dwMaxAP = m_pTITEM->m_bType == IT_LONG ? GetMaxLAP() : GetMaxAP();

	dwMinAP += GetMinMagicAP();
	dwMaxAP += GetMaxMagicAP();

	BYTE bMinLevel = BYTE(logf(FLOAT(dwMinAP) / (vPowerRate[m_pTITEM->m_bKind] * fItemMinRate)) * fLevelConst) + 1;
	BYTE bMaxLevel = BYTE(logf(FLOAT(dwMaxAP) / (vPowerRate[m_pTITEM->m_bKind] * fItemMaxRate)) * fLevelConst) + 1;

	return BYTE(max( (bMinLevel + bMaxLevel) / 2, 1));
}

BYTE CTItem::GetShieldPowerLevel()
{
	static FLOAT vPowerRate[IK_HP] = {
		0.0f,	// IK_NONE
		0.0f,	// IK_1HAND
		0.0f,	// IK_LHAND
		0.0f,	// IK_2HAND
		0.0f,	// IK_CAKRAM
		0.0f,	// IK_AX
		0.0f,	// IK_BOW
		0.0f,	// IK_ARBALEST
		0.0f,	// IK_MBAR
		0.0f,	// IK_MSTICK
		0.0f,	// IK_VAJRA
		0.8f,	// IK_MULTIVAJRA
		1.0f,	// IK_SHIELD
		1.3f,	// IK_CLOTH
		1.3f,	// IK_UNIFORM
		1.3f,	// IK_LLEATHER
		1.3f,	// IK_HLEATHER
		1.3f,	// IK_LARMOR
		1.3f,	// IK_HARMOR
		0.0f,	// IK_BACK
		0.0f,	// IK_NECK
		0.0f,	// IK_FINGER
		0.0f,	// IK_BALL
		0.0f,	// IK_PET
		0.0f,	// IK_ARROW
		0.0f};	// IK_SARROW

	static FLOAT vPartsPower[ES_COUNT] = {
		0.0f,	// ES_PRMWEAPON
		0.3f,	// ES_SNDWEAPON
		0.0f,	// ES_LONGWEAPON
		0.2f,	// ES_HEAD
		0.0f,	// ES_BACK
		0.3f,	// ES_BODY
		0.25f,	// ES_PANTS
		0.15f,	// ES_FOOT
		0.1f,	// ES_HAND
		0.0f,	// ES_NECK
		0.0f,	// ES_LFINGER
		0.0f,	// ES_RFINGER
		0.0f,	// ES_LEAR
		0.0f,	// ES_REAR
		0.0f};	// ES_FACE

	if( m_pTITEM->m_bPrmSlotID >= ES_COUNT ||
		vPartsPower[m_pTITEM->m_bPrmSlotID] == 0.0f ||
		m_pTITEM->m_bKind >= IK_HP ||
		vPowerRate[m_pTITEM->m_bKind] == 0.0f )
		return BYTE(1);

	FLOAT fLevelConst = 1.0f / logf(_AtlModule.GetFormulaRateX(FTYPE_1ST));
	FLOAT fItemRate = _AtlModule.GetFormulaRateX(FTYPE_ITEMPOWER);
	DWORD dwDP = GetDefendPower() + GetMagicDefPower();

	if( m_pTITEM->m_bType == IT_SHIELD )
		dwDP += m_pTITEMATTR->m_wDP + m_pTITEMATTR->m_wMDP;

	fItemRate = FLOAT(dwDP) / (vPowerRate[m_pTITEM->m_bKind] * vPartsPower[m_pTITEM->m_bPrmSlotID] * fItemRate);
	fItemRate = logf(fItemRate) * fLevelConst + 1;

	return max( BYTE(fItemRate), 1);
}

BYTE CTItem::GetPowerLevel()
{
	if( !m_pTITEM || !m_pTITEMATTR )
		return BYTE(1);

	if( GetMagicValue(MTYPE_PAP) ||
		GetMagicValue(MTYPE_LAP) ||
		GetMagicValue(MTYPE_PDP) ||
		GetMagicValue(MTYPE_MAP) ||
		GetMagicValue(MTYPE_MDP) )
	{
		switch(m_pTITEM->m_bType)
		{
		case IT_WEAPON		:
		case IT_LONG		: return GetWeaponPowerLevel();
		case IT_DEFENSIVE	:
		case IT_SHIELD		: return GetShieldPowerLevel();
		}
	}
	else
		return m_pTITEMATTR->m_bGrade;

	return BYTE(1);
}

DWORD CTItem::GetMaxAP()
{
	if(!m_pTITEM || !m_pTITEMATTR)
		return 0;

	DWORD dwValue=0;

	dwValue += GetMagicValue(MTYPE_PAP)
		+ GetMagicValue(MTYPE_PMAXAP);

	if(m_pTITEM->m_bType == IT_WEAPON)
		dwValue += m_pTITEMATTR->m_wMaxAP;

	return dwValue;
}
DWORD CTItem::GetMaxLAP()
{
	if(!m_pTITEM || !m_pTITEMATTR)
		return 0;

	DWORD dwValue=0;

	dwValue += GetMagicValue(MTYPE_LAP)
		+ GetMagicValue(MTYPE_LMAXAP);

	if(m_pTITEM->m_bType == IT_LONG)
		dwValue += m_pTITEMATTR->m_wMaxAP;

	return dwValue;
}
DWORD CTItem::GetMinAP()
{
	if(!m_pTITEM || !m_pTITEMATTR)
		return 0;

	DWORD dwValue=0;

	dwValue += GetMagicValue(MTYPE_PAP)
		+ GetMagicValue(MTYPE_PMINAP);

	if(m_pTITEM->m_bType == IT_WEAPON)
		dwValue += m_pTITEMATTR->m_wMinAP;

	return dwValue;
}
DWORD CTItem::GetMinLAP()
{
	if(!m_pTITEM || !m_pTITEMATTR)
		return 0;

	DWORD dwValue=0;

	dwValue += GetMagicValue(MTYPE_LAP)
		+ GetMagicValue(MTYPE_LMINAP);

	if(m_pTITEM->m_bType == IT_LONG)
		dwValue += m_pTITEMATTR->m_wMinAP;

	return dwValue;
}
DWORD CTItem::GetMaxMagicAP()
{
	if(!m_pTITEM || !m_pTITEMATTR)
		return 0;

	DWORD dwValue=0;

	dwValue += GetMagicValue(MTYPE_MAP)
		+ GetMagicValue(MTYPE_MPMAXAP);

	if(m_pTITEM->m_bType == IT_WEAPON)
		dwValue += m_pTITEMATTR->m_wMaxMAP;

	return dwValue;
}

DWORD CTItem::GetMinMagicAP()
{
	if(!m_pTITEM || !m_pTITEMATTR)
		return 0;

	DWORD dwValue=0;

	dwValue += GetMagicValue(MTYPE_MAP)
		+ GetMagicValue(MTYPE_MPMINAP);

	if(m_pTITEM->m_bType == IT_WEAPON)
		dwValue += m_pTITEMATTR->m_wMinMAP;

	return dwValue;
}

DWORD CTItem::GetDefendPower()
{
	if(!m_pTITEM || !m_pTITEMATTR)
		return 0;

	DWORD dwValue=0;

	dwValue += GetMagicValue(MTYPE_PDP);

	if(m_pTITEM->m_bType != IT_SHIELD)
		dwValue += m_pTITEMATTR->m_wDP;

	return dwValue;
}
DWORD CTItem::GetMagicDefPower()
{
	if(!m_pTITEM || !m_pTITEMATTR)
		return 0;

	DWORD dwValue=0;

	dwValue += GetMagicValue(MTYPE_MDP);

	if(m_pTITEM->m_bType != IT_SHIELD)
		dwValue += m_pTITEMATTR->m_wMDP;

	return dwValue;
}

WORD CTItem::GetMagicValue( BYTE bMagicID)
{
	MAPTMAGIC::iterator finder = m_mapTMAGIC.find(bMagicID);
	if( finder != m_mapTMAGIC.end() )
		return GetMagicValue((*finder).second);

	return 0;
}

WORD CTItem::GetMagicValue( LPTMAGIC pTMAGIC)
{
	LPTITEMMAGIC pMagic = pTMAGIC->m_pMagic;

	FLOAT fRevision = FLOAT(pMagic->m_bRvType ? m_pTITEM->m_fRevision[pMagic->m_bRvType-1] : 1.0);
	WORD wValue = WORD(fRevision * pTMAGIC->m_wValue * pMagic->m_wMaxValue) / 100;
	return max(wValue, 1);
}

void CTItem::SetRevision(LPTMAGIC pTMAGIC)
{
	LPTITEMMAGIC pMagic = pTMAGIC->m_pMagic;
	FLOAT fRevision = FLOAT(pMagic->m_bRvType ? m_pTITEM->m_fRevision[pMagic->m_bRvType-1] : 1.0);
	FLOAT fDiv = pMagic->m_wMaxValue * fRevision > 0 ? pMagic->m_wMaxValue * fRevision : 100;
	pTMAGIC->m_wValue = WORD(100/fDiv + 0.999f);
}

void CTItem::WrapPacket(CPacket * pPacket)
{
	(*pPacket)
		<< m_dlID
		<< m_bItemID
		<< m_wItemID
		<< m_bLevel
		<< m_bCount
		<< m_bGLevel
		<< m_dwDuraMax
		<< m_dwDuraCur
		<< m_bRefineCur
		<< m_dEndTime
		<< m_bGradeEffect
		<< m_dwExtValue[IEV_ELD]
		<< m_dwExtValue[IEV_WRAP]
		<< m_dwExtValue[IEV_COLOR]
		<< m_dwExtValue[IEV_GUILD]
		<< BYTE(m_mapTMAGIC.size());

	MAPTMAGIC::iterator itTMAGIC;
	for( itTMAGIC = m_mapTMAGIC.begin(); itTMAGIC != m_mapTMAGIC.end(); itTMAGIC++)
	{
		(*pPacket)
			<< (*itTMAGIC).first
			<< (*itTMAGIC).second->m_wValue;
	}
}

void CTItem::WrapPacketClient(CPacket * pPacket, DWORD dwCharID, BYTE bAddItemID, WORD wUseTime, BYTE bCashItem)
{
	BYTE bRegGuild = FALSE;

	if(	m_dwExtValue[IEV_GUILD] &&
		m_dwExtValue[IEV_GUILD] == dwCharID)
		bRegGuild = TRUE;

	if(bAddItemID)
		(*pPacket) << m_bItemID;

	(*pPacket)
		<< m_wItemID
		<< m_bLevel
		<< m_bCount
		<< m_dwDuraMax
		<< m_dwDuraCur
		<< m_pTITEM->m_bRefineMax
		<< m_bRefineCur
		<< m_bGLevel;

	if(bCashItem)
		(*pPacket) << wUseTime;
	else
		(*pPacket) << m_dEndTime;

	(*pPacket)
		<< m_bGradeEffect
		<< BYTE(m_dwExtValue[IEV_ELD])
		<< BYTE(m_dwExtValue[IEV_WRAP])
		<< WORD(m_dwExtValue[IEV_COLOR])
		<< bRegGuild
		<< BYTE(m_mapTMAGIC.size());

	MAPTMAGIC::iterator itTMAGIC;
	for( itTMAGIC = m_mapTMAGIC.begin(); itTMAGIC != m_mapTMAGIC.end(); itTMAGIC++)
	{
		(*pPacket)
			<< (*itTMAGIC).first
			<< GetMagicValue((*itTMAGIC).second);
	}
}

BYTE CTItem::CanUse()
{
	return !m_dwExtValue[IEV_WRAP];
}

BYTE CTItem::HavePower()
{
	return (m_dwDuraMax && m_dwDuraCur) || !m_dwDuraMax;
}

DWORD CTItem::GetPrice()
{
	if(!m_pTITEM || !m_pTITEMATTR)
		return 0;

	BYTE bGrade = 0;
	if(m_pTITEM->m_wAttrID)
		bGrade = GetPowerLevel();
	else
		bGrade = m_pTITEM->m_bDefaultLevel;

	return DWORD(_AtlModule.FindTLevel(bGrade)->m_dwMoney * m_pTITEM->m_fPrice + 0.99);
}

DWORD CTItem::GetPvPoint()
{
	if(!m_pTITEM || !m_pTITEMATTR)
		return 0;

	BYTE bGrade = 0;
	if(m_pTITEM->m_wAttrID)
		bGrade = GetPowerLevel();
	else
		bGrade = m_pTITEM->m_bDefaultLevel;

	return DWORD(_AtlModule.FindTLevel(bGrade)->m_dwPvPMoney * m_pTITEM->m_fPrice + 0.99);
}

void CTItem::SetEndTime(DWORD dwUseTime)
{
	DWORD dwAdd = 0;

	if(dwUseTime)
		dwAdd = dwUseTime;
	else if(m_pTITEM->m_bUseType & DURINGTYPE_TAKE)
	{
		if(m_pTITEM->m_bUseType & DURINGTYPE_TIME)
			dwAdd = DWORD(m_pTITEM->m_wUseTime) * HOUR_ONE;
		else if(m_pTITEM->m_bUseType & DURINGTYPE_DAY)
			dwAdd = DWORD(m_pTITEM->m_wUseTime) * DAY_ONE;
	}

	if(dwAdd)
	{
		CTime time(_AtlModule.m_timeCurrent);
		dwAdd -= time.GetSecond();
		dwAdd -= time.GetMinute() * 60;
		dwAdd += (24 - time.GetHour()) * HOUR_ONE;
		m_dEndTime = time.GetTime() + dwAdd;
	}
	else
		m_dEndTime = 0;
}

WORD CTItem::GetAutoSkill()
{
	MAPTMAGIC::iterator it;
	for(it=m_mapTMAGIC.begin(); it!=m_mapTMAGIC.end(); it++)
	{
		if((*it).second->m_pMagic->m_wAutoSkill)
		{
			if((*it).second->m_wValue > rand() % 100)
				return (*it).second->m_pMagic->m_wAutoSkill;
		}
	}

	return 0;
}

BYTE CTItem::GetEquipLevel()
{
	BYTE bLevel = m_pTITEM->m_bDefaultLevel;

	if(bLevel > m_dwExtValue[IEV_ELD])
		bLevel -= BYTE(m_dwExtValue[IEV_ELD]);
/*
	MAPTMAGIC::iterator it = m_mapTMAGIC.find(MTYPE_EQLEVEL);
	if(it!=m_mapTMAGIC.end())
		bLevel -= min(GetMagicValue((*it).second), bLevel);
*/
	return bLevel;
}

void CTItem::SetGradeEffect(BYTE bEffect) //08-0415
{
	m_bGradeEffect = bEffect;
}

DWORD CTItem::RandMoney()
{
	DWORD dwMoney = 0;

	for( BYTE i=0; i<5; i++)
	{
		float fAdd = FLOAT(rand() % 100) / 100.0f;
		dwMoney += DWORD(m_pTITEM->m_fPrice * 1000 * fAdd * fAdd);

		if( fAdd < 0.95f )
			break;
	}

	return max( DWORD(m_pTITEM->m_wUseValue) * 1000, dwMoney);
}

void CTItem::Catalyzer(CTPlayer * pPlayer)
{
	static int opprob[6] = { 100, 50, 20, 10, 5, 1};

	MAPTITEMMAGIC::iterator it;

	VTITEMMAGIC vMagic;
	vMagic.clear();

	for(it=_AtlModule.m_mapTItemMagic.begin(); it!=_AtlModule.m_mapTItemMagic.end(); it++)
	{
		if((*it).first && (*it).second->m_bRefine)
			vMagic.push_back((*it).second);
	}

	for( int i=0; i<6; i++)
	{
		if( rand() % 100 < opprob[i] )
		{
			BYTE bSelect = BYTE(rand() % vMagic.size());
			LPTMAGIC pTMAGIC = new TMAGIC();
			pTMAGIC->m_pMagic = vMagic[bSelect];
			pTMAGIC->m_wValue = pPlayer->MakeItemMagicValue(ITEMAGIC_BASELEVEL, pTMAGIC->m_pMagic->m_wRareBound);

			if(pTMAGIC->m_wValue)
			{
				m_mapTMAGIC.insert(MAPTMAGIC::value_type(pTMAGIC->m_pMagic->m_bMagic, pTMAGIC));
				vMagic.erase(vMagic.begin() + bSelect);
			}
			else
				delete pTMAGIC;
		}
	}
}

BYTE CTItem::CanDeal()
{
	return m_dwExtValue[IEV_WRAP] || (m_pTITEM->m_bIsSell & ITEMTRADE_DEAL);
}