#include "StdAfx.h"
#include "TClientGame.h"

BYTE CTClientSkill::m_bDiscountRate = 0;

DWORD CTClientSkill::m_vTSKILLLEVEL[TSKILLSLOT_COUNT] = {
	ID_CTRLINST_SKILLLEVEL_1,
	ID_CTRLINST_SKILLLEVEL_2,
	ID_CTRLINST_SKILLLEVEL_3,
	ID_CTRLINST_SKILLLEVEL_4,
	ID_CTRLINST_SKILLLEVEL_5,
	ID_CTRLINST_SKILLLEVEL_6,
	ID_CTRLINST_SKILLLEVEL_7,
	ID_CTRLINST_SKILLLEVEL_8,
	ID_CTRLINST_SKILLLEVEL_9,
	ID_CTRLINST_SKILLLEVEL_10,
	ID_CTRLINST_SKILLLEVEL_11,
	ID_CTRLINST_SKILLLEVEL_12,
	ID_CTRLINST_SKILLLEVEL_13,
	ID_CTRLINST_SKILLLEVEL_14};

DWORD CTClientSkill::m_vTSKILLSLOT[TSKILLSLOT_COUNT] = {
	ID_CTRLINST_SKILL_1,
	ID_CTRLINST_SKILL_2,
	ID_CTRLINST_SKILL_3,
	ID_CTRLINST_SKILL_4,
	ID_CTRLINST_SKILL_5,
	ID_CTRLINST_SKILL_6,
	ID_CTRLINST_SKILL_7,
	ID_CTRLINST_SKILL_8,
	ID_CTRLINST_SKILL_9,
	ID_CTRLINST_SKILL_10,
	ID_CTRLINST_SKILL_11,
	ID_CTRLINST_SKILL_12,
	ID_CTRLINST_SKILL_13,
	ID_CTRLINST_SKILL_14};

DWORD CTClientSkill::m_vTSKILLADD[TSKILLSLOT_COUNT] = {
	ID_CTRLINST_ADD_1,
	ID_CTRLINST_ADD_2,
	ID_CTRLINST_ADD_3,
	ID_CTRLINST_ADD_4,
	ID_CTRLINST_ADD_5,
	ID_CTRLINST_ADD_6,
	ID_CTRLINST_ADD_7,
	ID_CTRLINST_ADD_8,
	ID_CTRLINST_ADD_9,
	ID_CTRLINST_ADD_10,
	ID_CTRLINST_ADD_11,
	ID_CTRLINST_ADD_12,
	ID_CTRLINST_ADD_13,
	ID_CTRLINST_ADD_14};


CTClientSkill::CTClientSkill()
{
	m_bTimerON = FALSE;
	m_bLoopON = FALSE;
	m_pTSKILL = NULL;

	m_dwExceptTick = 0;
	m_dwReuseTick = 0;
	m_dwTick = 0;
	m_bLevel = 0;

	m_vTSFX.clear();
}

CTClientSkill::~CTClientSkill()
{
	RemoveTSFX();
}

LPTSKILL CTClientSkill::FindTIMMER( LPTSKILL pTSKILL)
{
	for( int i=0; i<INT(pTSKILL->m_vTFUNCTION.size()); i++)
		if( pTSKILL->m_vTFUNCTION[i]->m_bType == SDT_STATUS && pTSKILL->m_vTFUNCTION[i]->m_bFunctionID == SDT_STATUS_TIMMER )
			return CTChart::FindTSKILLTEMP(pTSKILL->m_vTFUNCTION[i]->m_wValue);

	return NULL;
}

int CTClientSkill::CalcTSKILLDATA( LPTSKILL pTSKILL,
								   BYTE bINDEX,
								   BYTE bLevel)
{
	int nRESULT = 0;

	if( bINDEX >= INT(pTSKILL->m_vTFUNCTION.size()) ||
		!pTSKILL->m_vTFUNCTION[bINDEX] )
		return nRESULT;

	switch(pTSKILL->m_vTFUNCTION[bINDEX]->m_bCalc)
	{
	case TSKILLDATAOP_CONSTANT	: nRESULT = INT(pTSKILL->m_vTFUNCTION[bINDEX]->m_wValue); break;
	case TSKILLDATAOP_LINE		: nRESULT = INT(pTSKILL->m_vTFUNCTION[bINDEX]->m_wValue) + (INT(bLevel) - 1) * INT(pTSKILL->m_vTFUNCTION[bINDEX]->m_wInc); break;
	case TSKILLDATAOP_DEC		: nRESULT = INT(pTSKILL->m_vTFUNCTION[bINDEX]->m_wValue) - (INT(bLevel) - 1) * INT(pTSKILL->m_vTFUNCTION[bINDEX]->m_wInc); break;
	case TSKILLDATAOP_EXP		: nRESULT = INT(CTChart::m_vLEVELRATE[INT(pTSKILL->m_bLevel) + (INT(bLevel) - 1) * INT(pTSKILL->m_bLevelInc)] * INT(pTSKILL->m_vTFUNCTION[bINDEX]->m_wValue) / 100); break;
	}

	return nRESULT;
}

BYTE CTClientSkill::IsAttackSKILL( LPTSKILL pTSKILL)
{
	return pTSKILL && !(pTSKILL->m_bPositive % 2) ? TRUE : FALSE;
}

BYTE CTClientSkill::IsFlySKILL( LPTSKILL pTSKILL)
{
	if(!pTSKILL)
		return FALSE;

	for( int i=0; i<INT(pTSKILL->m_vTFUNCTION.size()); i++)
		switch(pTSKILL->m_vTFUNCTION[i]->m_bType)
		{
		case SDT_TRANS		:
			{
				LPTMONTEMP pTMON = CTChart::FindTMONTEMP( pTSKILL->m_vTFUNCTION[i]->m_wValue );

				if( pTMON && pTMON->m_bCanFly )
					return TRUE;
			}
		}

	return FALSE;
}

DWORD CTClientSkill::GetChargeTick( BYTE bChargeSpeed)
{
	if( !m_pTSKILL )
		return 0;

	if( !bChargeSpeed )
		return m_pTSKILL->m_dwSpellTick;

	return m_pTSKILL->m_dwSpellTick * 100 / bChargeSpeed;
}

DWORD CTClientSkill::GetReuseTick( CTClientObjBase* pHOST)
{
	if(!m_pTSKILL)
		return 0;
	DWORD dwResult = m_pTSKILL->m_dwDelay + INT(max( m_bLevel, 1) - 1) * m_pTSKILL->m_nDelayInc;
	if( pHOST && m_pTSKILL->m_bDelayType )
	{
		DWORD dwAD = pHOST->m_dwAttackDelay[ m_pTSKILL->m_bDelayType ];
		DWORD dwADR = pHOST->m_dwAttackDelayRate[ m_pTSKILL->m_bDelayType ];

		return (dwResult + dwAD) * dwADR / 100;
	}

	return dwResult;
}

void CTClientSkill::RemoveTSFX()
{
	while(!m_vTSFX.empty())
	{
		LPOBJSFX pItem = m_vTSFX.back();
		CTachyonSFX *pTSFX = CTachyonSFX::GetSFX(pItem->m_dwID);

		if(pTSFX)
		{
			pTSFX->m_bHasHost = FALSE;
			pTSFX->m_bDeleted = TRUE;
		}

		m_vTSFX.pop_back();
		delete pItem;
	}
}
