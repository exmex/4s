#include "StdAfx.h"
#include ".\tskilltemp.h"

CTSkillTemp::CTSkillTemp(void)
{
	m_vData.clear();
	m_mapTSkillPoint.clear();
}

CTSkillTemp::~CTSkillTemp(void)
{
	while(!m_vData.empty())
	{
		delete m_vData.back();
		m_vData.pop_back();
	}

	m_vData.clear();

	MAPTSKILLPOINT::iterator it;
	for(it=m_mapTSkillPoint.begin(); it!=m_mapTSkillPoint.end(); it++)
		delete (*it).second;
	m_mapTSkillPoint.clear();
}

BYTE CTSkillTemp::IsPositive()
{
	return m_bPositive == SPT_POSITIVE;
}

BYTE CTSkillTemp::IsNegative()
{
	return !(m_bPositive % 2);
}

BYTE CTSkillTemp::HaveSkillData(LPTSKILLDATA pData)
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == pData->m_bType &&
			m_vData[i]->m_bAttr == pData->m_bAttr &&
			m_vData[i]->m_bExec &&
			m_vData[i]->m_bExec == pData->m_bExec)
			return TRUE;
	}

	return FALSE;
}
int CTSkillTemp::CalcValue(BYTE bLevel, BYTE bType, BYTE bExec, DWORD dwValue)
{
	int nInc=0;
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == bType &&
			m_vData[i]->m_bExec == bExec)
			nInc += Calculate(bLevel, i, dwValue);
	}

	return nInc;
}

int CTSkillTemp::GetValue(LPTSKILLDATA pData, BYTE bLevel)
{
	switch(pData->m_bCalc)
	{
	case 0:
		return pData->m_wValue;
	case 1:
        return pData->m_wValue + (bLevel-1) * pData->m_wValueInc;
	case 2:
		return int((pData->m_wValue * pow(m_f1stRateX, m_bStartLevel+(bLevel-1)*m_bNextLevel))/100);
	case 3:
		return INT(pData->m_wValue) - (INT(bLevel)-1) * INT(pData->m_wValueInc);
	}
	return 0;
}
int CTSkillTemp::Calculate(BYTE bLevel, DWORD index, DWORD dwValue)
{
	int nCalcValue = GetValue(m_vData[index], bLevel);
	int nValue = 0;

	switch(m_vData[index]->m_bInc)
	{
	case SVI_INCREASE:
		nValue = nCalcValue;
		break;
	case SVI_DECREASE:
		nValue = -nCalcValue;
		break;
	case SVI_MULTIPLY:
		nValue = dwValue * nCalcValue - dwValue;
		break;
	case SVI_DIVIDE:
		if(nCalcValue <= 0) nCalcValue = 1;
		nValue = dwValue / nCalcValue - dwValue;
		break;
	case SVI_PRECENT:
		nValue = int(dwValue * nCalcValue / 100.0) - dwValue;
		break;
	default:
		break;
	}

	return nValue;
}
BYTE CTSkillTemp::IsMirrorType()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_STATUS &&
			m_vData[i]->m_bExec == SDT_STATUS_MAGICMIRROR)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsDamageType()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_ABILITY &&
			m_vData[i]->m_bAttr &&
		   (m_vData[i]->m_bExec == MTYPE_DAMAGE ||
			m_vData[i]->m_bExec == MTYPE_MDAMAGE ||
		    m_vData[i]->m_bExec == MTYPE_HP ||
			m_vData[i]->m_bExec == MTYPE_MP ))
		  return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsRemainType()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bAction == SA_CONTINUE)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsPassiveType()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bAction == SA_PASSIVE &&
			m_vData[i]->m_bType == SDT_ABILITY)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsPosture()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_STATUS &&
			(m_vData[i]->m_bExec == SDT_STATUS_CRAZEMODE ||
			 m_vData[i]->m_bExec == SDT_STATUS_DEFENDMODE ||
			 m_vData[i]->m_bExec == SDT_STATUS_ATKMODE))

			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::GetAttackType()
{
	BYTE bType = SAT_NONE;
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		switch(m_vData[i]->m_bAttr)
		{
		case SATT_PHYSIC:
		case SATT_LONG:
			return SAT_PHYSIC;
		case SATT_MAGICNO:
			return SAT_MAGIC;
		case SATT_MAGICSR:
		case SATT_MAGICDR:
		case SATT_MAGICER:
		case SATT_MAGICFR:
		case SATT_MAGICPR:
		case SATT_MAGICIR:
			bType = SAT_MAGIC;
			break;
		}
	}

	return bType;
}
BYTE CTSkillTemp::IsLongAttack()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_ABILITY &&
			(m_vData[i]->m_bExec == MTYPE_LAP ||
			 m_vData[i]->m_bAttr == SATT_LONG))
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::CanEquip(BYTE bKind)
{
	for(DWORD i=0; i<(DWORD)m_vData.size(); i++)
	{
		BYTE bType = m_vData[i]->m_bType;
		BYTE bExec = m_vData[i]->m_bExec;
		if(bType == SDT_EQUIP && bExec == bKind)
			return TRUE;
	}

	return FALSE;
}
WORD CTSkillTemp::GetID()
{
	return m_wID;
}
BYTE CTSkillTemp::IsMaintainType()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bAction == SA_BUFF)
			return TRUE;
	}

	return FALSE;
}
BYTE CTSkillTemp::IsSilence()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_STATUS &&
			m_vData[i]->m_bExec == SDT_STATUS_SILENCE)
			return TRUE;
	}

	return FALSE;
}
BYTE CTSkillTemp::IsBuffType()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bAction == SA_BUFF)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsDotType()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bAction == SA_DOT)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsRevivalType()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_CURE &&
			m_vData[i]->m_bExec == SCT_REVIVAL)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsBlockType()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_STATUS &&
			( m_vData[i]->m_bExec == SDT_STATUS_BLOCK ||
			  m_vData[i]->m_bExec == SDT_STATUS_HOLD ||
			  m_vData[i]->m_bExec == SDT_STATUS_HYPNOSIS))
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsHideBuff()
{
	return m_bIsHideSkill;
}

BYTE CTSkillTemp::IsCancelMine()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_STATUS &&
			m_vData[i]->m_bExec == SDT_STATUS_CANCELMINE)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsTrans()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_TRANS)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsItemDelTarget()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_ITEM &&
			m_vData[i]->m_bExec == SDT_ITEM_DELTARGET)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsItemDelUser()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_ITEM &&
			m_vData[i]->m_bExec == SDT_ITEM_DELUSER)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsDie()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_STATUS &&
			m_vData[i]->m_bExec == SDT_STATUS_DIE)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsMultiAttack()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_ABILITY &&
			m_vData[i]->m_bExec == MTYPE_EFC)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::GetTransHPMPType()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_CURE)
		{
			if(m_vData[i]->m_bExec == SCT_HPTRANS)
				return SCT_HPTRANS;
			else if(m_vData[i]->m_bExec == SCT_MPTRANS)
				return SCT_MPTRANS;
		}
	}

	return 0;
}
BYTE CTSkillTemp::CheckCountry(BYTE bCountry)
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_TRANS &&
			((m_vData[i]->m_bExec == SDT_TRANS_DISGUISE_D && bCountry == TCONTRY_D) ||
			(m_vData[i]->m_bExec == SDT_TRANS_DISGUISE_C && bCountry == TCONTRY_C) ||
			(m_vData[i]->m_bExec == SDT_TRANS_DISGUISE_B && bCountry == TCONTRY_B)))
			return FALSE;
	}

	return TRUE;
}

DWORD CTSkillTemp::GetPrice(DWORD dwMoney)
{
	return DWORD(dwMoney * m_fPrice);
}

BYTE CTSkillTemp::GetNeedSkillPoint(BYTE bLevel)
{
	MAPTSKILLPOINT::iterator it = m_mapTSkillPoint.find(bLevel);
	if(it != m_mapTSkillPoint.end())
		return (*it).second->m_bSkillPoint;
	else
		return 0;
}

BYTE CTSkillTemp::GetNeedKindPoint(BYTE bLevel)
{
	MAPTSKILLPOINT::iterator it = m_mapTSkillPoint.find(bLevel);
	if(it != m_mapTSkillPoint.end())
		return (*it).second->m_bGroupPoint;
	else
		return 0;
}

BYTE CTSkillTemp::CheckParentSkill(BYTE bLearnLevel, BYTE bParentLevel)
{
	MAPTSKILLPOINT::iterator it = m_mapTSkillPoint.find(bLearnLevel);
	if(it != m_mapTSkillPoint.end() &&	(*it).second->m_bParentLevel <= bParentLevel)
		return TRUE;
	else
		return FALSE;
}

DWORD CTSkillTemp::GetAggro(BYTE bLevel)
{
	if(!m_dwAggro)
		return 0;

	DWORD dwAggro = DWORD(m_dwAggro * pow(m_f1stRateX, m_bStartLevel+(bLevel-1)*m_bNextLevel)) / 100;
	return dwAggro;
}

WORD CTSkillTemp::GetItemProbBuff(BYTE bType)  
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if( m_vData[i]->m_bType == SDT_STATUS &&
			m_vData[i]->m_bExec == bType)
			return m_vData[i]->m_wValue;
	}
	
	return 0;
}

BYTE CTSkillTemp::IsRandomTrans()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(	m_vData[i]->m_bType == SDT_TRANS &&
			m_vData[i]->m_bExec == SDT_TRANS_RANDOM)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsAfterMath()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(	m_vData[i]->m_bType == SDT_CURE &&
			m_vData[i]->m_bExec == SCT_AFTERMATH)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsRandomBuff()  
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(	m_vData[i]->m_bType == SDT_STATUS &&
			m_vData[i]->m_bExec == SDT_STATUS_RANDOM)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::CanDefendAtDie()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if((m_vData[i]->m_bType == SDT_CURE &&
			m_vData[i]->m_bExec == SCT_REVIVAL) ||
			(m_vData[i]->m_bType == SDT_AI &&
			 m_vData[i]->m_bExec == SDT_TEMPT))
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsMonRecall()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_RECALL &&
			m_vData[i]->m_bExec == SER_MONSTER)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsRecall()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_RECALL &&
			m_vData[i]->m_bExec != SER_SKILL)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsLuckyPotion()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_STATUS &&
			m_vData[i]->m_bExec == SDT_STATUS_ITEMUPGRADE)
			return TRUE;
	}

	return FALSE;
}

BYTE CTSkillTemp::IsExpPotion()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		if(m_vData[i]->m_bType == SDT_STATUS &&
			m_vData[i]->m_bExec == SDT_STATUS_AUTOEXP)
			return TRUE;
	}

	return FALSE;
}

WORD CTSkillTemp::GetAutoExp()
{
	for(DWORD i=0; i<m_vData.size(); i++)
	{
		LPTSKILLDATA pData = m_vData[i];
		if(pData->m_bType == SDT_STATUS &&
			pData->m_bExec == SDT_STATUS_AUTOEXP)
			return pData->m_wValue;
	}

	return 0;
}
