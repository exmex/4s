#include "StdAfx.h"

CTSkill::CTSkill()
{
	m_dwChargeTick = 0;
	m_dwUseTick = 0;
	m_dwDelayTick = 0;
	m_dwStartTick = 0;
	m_dwMaintainTick = 0;
	m_pTSKILL = NULL;
	m_bLevel = 1;
	m_dwAttackID = 0;
	m_bAttackType = OT_PC;
	m_dwHostID = 0;
	m_bHostType = OT_PC;
	m_bHit = HT_NORMAL;
	m_wAttackLevel = 1;
	m_bAttackerLevel = 1;
	m_dwPysMinPower = 0;
	m_dwPysMaxPower = 0;
	m_dwMgMinPower = 0;
	m_dwMgMaxPower = 0;
	m_bCanSelect = TRUE;
	m_wSilencedSkill = 0;

	m_bAttackCountry = TCONTRY_N;
	m_fPosX = 0;
	m_fPosY = 0;
	m_fPosZ = 0;
}
CTSkill::~CTSkill()
{
}
void CTSkill::SetMaintain(DWORD dwAtkID,
                          BYTE bAtkType,
                          DWORD dwHostID,
                          BYTE bHostType,
						  BYTE bHit,
                          WORD wAtkLevel,
						  BYTE bAttackerLevel,
                          DWORD dwPysMinPower,
                          DWORD dwPysMaxPower,
						  DWORD dwMgMinPower,
						  DWORD dwMgMaxPower,
						  BYTE bCanSelect,
						  BYTE bAttackCountry,
                          FLOAT fPosX,
                          FLOAT fPosY,
                          FLOAT fPosZ)
{
	m_dwAttackID = dwAtkID;
	m_bAttackType = bAtkType;
	m_dwHostID = dwHostID;
	m_bHostType = bHostType;
	m_bHit = bHit;
	m_wAttackLevel = wAtkLevel;
	m_bAttackerLevel = bAttackerLevel;
	m_dwPysMinPower = dwPysMinPower;
	m_dwPysMaxPower = dwPysMaxPower;
	m_dwMgMinPower = dwMgMinPower;
	m_dwMgMaxPower = dwMgMaxPower;
	m_bCanSelect = bCanSelect;
	m_bAttackCountry = bAttackCountry;
	m_fPosX = fPosX;
	m_fPosY = fPosY;
	m_fPosZ = fPosZ;
}

void CTSkill::Use(BYTE bType, DWORD dwTick, DWORD dwAttackSpeed, DWORD dwRate)
{
	DWORD dwDelay = GetReuseDelay(bType, dwAttackSpeed, dwRate);
	if(GetReuseRemainTick(dwTick) < dwDelay)
	{
		m_dwUseTick = dwTick;
		m_dwDelayTick = dwDelay;
	}
}

DWORD CTSkill::GetReuseDelay(BYTE bType, DWORD dwAttackSpeed, DWORD dwRate)
{
	DWORD dwBaseDelay;
	switch(bType)
	{
	case SDELAY_SKILL:
		dwBaseDelay = m_pTSKILL->m_dwReuseDelay + (m_bLevel - 1) * m_pTSKILL->m_nReuseDelayInc;
		break;
	case SDELAY_LOOP:
		dwBaseDelay = m_pTSKILL->m_dwLoopDelay;
		break;
	case SDELAY_KIND:
		return dwAttackSpeed;
	default:
		break;
	}

	return (dwBaseDelay + dwAttackSpeed) * dwRate / 100;
}

DWORD CTSkill::GetRemainTick(DWORD dwTick)
{
	if(!m_dwStartTick || (dwTick - m_dwStartTick) >= m_dwMaintainTick)
		return 0;
	else
		return m_dwMaintainTick - (dwTick - m_dwStartTick);
}

DWORD CTSkill::GetReuseRemainTick(DWORD dwTick)
{
	if(!m_dwUseTick || (dwTick - m_dwUseTick) >= m_dwDelayTick)
		return 0;
	else
		return m_dwDelayTick - (dwTick - m_dwUseTick);
}
BYTE CTSkill::GetProb(LPTSKILLDATA pData)
{
	return TRUE;
}
DWORD CTSkill::GetValue(LPTSKILLDATA pData)
{
	return m_pTSKILL->GetValue(pData, m_bLevel);
}
DWORD CTSkill::GetValue(BYTE bType, BYTE bExec)
{
	for(DWORD i=0; i<m_pTSKILL->m_vData.size(); i++)
	{
		if(m_pTSKILL->m_vData[i]->m_bType == bType &&
			m_pTSKILL->m_vData[i]->m_bExec == bExec)
			return GetValue(m_pTSKILL->m_vData[i]);
	}
	return 0;
}
void CTSkill::SetEndTick(DWORD dwTick)
{
	DWORD dwRemainTick = GetMaintainTick();
	m_dwStartTick = dwRemainTick ? max(dwTick, 1) : 0;
	m_dwMaintainTick = dwRemainTick;
	m_dwChargeTick = dwTick;
}
void CTSkill::SetLoopEndTick(DWORD dwTick, DWORD dwRemain)
{
	m_dwStartTick = max(dwTick, 1);
	m_dwMaintainTick = dwRemain;
	m_dwChargeTick = dwTick;
}

DWORD CTSkill::GetMaintainTick()
{
	return m_pTSKILL->m_dwDuration + m_pTSKILL->m_dwDurationInc * (m_bLevel-1);
}
int CTSkill::CalcAbilityValue(BYTE bAction, DWORD dwValue, BYTE bExec)
{
	int nIncreaseValue = 0;
	for(DWORD i=0; i<m_pTSKILL->m_vData.size(); i++)
	{
		if( m_pTSKILL->m_vData[i]->m_bAction == bAction &&
			m_pTSKILL->m_vData[i]->m_bType == SDT_ABILITY &&
			m_pTSKILL->m_vData[i]->m_bExec == bExec)
			nIncreaseValue += m_pTSKILL->Calculate(m_bLevel, i, dwValue);
	}
	return nIncreaseValue;
}
int CTSkill::CalcCure(CTSkill * pSkill, DWORD dwValue, BYTE bExec)
{
	if(!pSkill)
		return 0;

	int nIncreaseValue = 0;
	for(DWORD i=0; i<m_pTSKILL->m_vData.size(); i++)
	{
		if(	m_pTSKILL->m_vData[i]->m_bType == SDT_CURE &&
			m_pTSKILL->m_vData[i]->m_bExec == bExec)
		{
			for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
			{
				if(	m_pTSKILL->m_vData[i]->m_bAttr == pSkill->m_pTSKILL->m_vData[j]->m_bAttr)
					nIncreaseValue += m_pTSKILL->Calculate(m_bLevel, i, dwValue);
			}
		}
	}

	return nIncreaseValue;
}

DWORD CTSkill::GetRequiredHP(DWORD dwMaxHP)
{
	DWORD dwHP = 0;
	FLOAT fRate = pow(m_pTSKILL->m_f1stRateX, int(m_pTSKILL->m_bStartLevel)+(m_bLevel-1)*m_pTSKILL->m_bNextLevel)/100;
	switch(m_pTSKILL->m_bUseHPType)
	{
	case 1:
		dwHP = DWORD(m_pTSKILL->m_dwUseHP * fRate);
		break;
	case 2:
		dwHP = dwMaxHP * m_pTSKILL->m_dwUseHP / 100;
		break;
	}

	return dwHP;
}

DWORD CTSkill::GetRequiredMP(DWORD dwMaxMP)
{
	DWORD dwMP = 0;
	FLOAT fRate = pow(m_pTSKILL->m_f1stRateX, int(m_pTSKILL->m_bStartLevel)+(m_bLevel-1)*m_pTSKILL->m_bNextLevel)/100;
	switch(m_pTSKILL->m_bUseMPType)
	{
	case 1:
		dwMP = DWORD(m_pTSKILL->m_dwUseMP * fRate);
		break;
	case 2:
		dwMP = dwMaxMP * m_pTSKILL->m_dwUseMP / 100;
		break;
	}

	return dwMP;
}

WORD CTSkill::GetID()
{
	return m_pTSKILL->GetID();
}

BYTE CTSkill::CanUse(DWORD dwTick)
{
	return !GetReuseRemainTick(dwTick);
}

BYTE CTSkill::GetNextLevel()
{
	return m_pTSKILL->m_bStartLevel + m_bLevel * m_pTSKILL->m_bNextLevel;
}
BYTE CTSkill::IsEnd(DWORD dwTick)
{
	if(m_dwStartTick && dwTick - m_dwStartTick >= m_dwMaintainTick)
		return TRUE;
	else
		return FALSE;
}

DWORD CTSkill::GetBlood(DWORD dwValue, BYTE bExec)
{
	for(DWORD i=0; i<m_pTSKILL->m_vData.size(); i++)
	{
		if(m_pTSKILL->m_vData[i]->m_bType == SDT_ABILITY &&
			m_pTSKILL->m_vData[i]->m_bExec == bExec)
			return (DWORD)max(m_pTSKILL->Calculate(m_bLevel, i, dwValue), 0);
	}
	return 0;
}
DWORD CTSkill::GetAggro()
{
	if(!m_pTSKILL->m_dwAggro)
		return 0;

	DWORD dwAggro = DWORD(m_pTSKILL->m_dwAggro * pow(m_pTSKILL->m_f1stRateX, m_pTSKILL->m_bStartLevel+(m_bLevel-1)*m_pTSKILL->m_bNextLevel)) / 100;
	return dwAggro;
}
BYTE CTSkill::GetCountMultiAttack()
{
	return CalcAbilityValue(SA_ONCE, 1, MTYPE_EFC);
}

BYTE CTSkill::GetNextSkillPoint()
{
	return m_pTSKILL->GetNeedSkillPoint(m_bLevel + 1);
}

WORD CTSkill::GetUsedSkillPoint()
{
	WORD wTotal = 0;
	for(BYTE i=0; i<m_bLevel; i++)
		wTotal += m_pTSKILL->GetNeedSkillPoint(i+1);

	return wTotal;
}

BYTE CTSkill::CheckParentSkill(BYTE bParentLevel)
{
	return m_pTSKILL->CheckParentSkill(m_bLevel + 1, bParentLevel);
}

