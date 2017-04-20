#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"


CTSelfObj::CTSelfObj()
{
	m_bType = OT_SELF;
	m_dwPysMinPower = 0;
	m_dwPysMaxPower = 0;
	m_dwMgMinPower = 0;
	m_dwMgMaxPower = 0;
	m_wAtkAL = 0;
	m_bAtkSkillLevel = 1;
}

CTSelfObj::~CTSelfObj()
{
	_AtlModule.ReleaseSelfMonID(m_dwID);
}

void CTSelfObj::OnDie( DWORD dwAttackID, BYTE bObjectType, WORD wTempMonID ) 
{
	if(!m_pMON)
		return;

	if(m_pMON->m_bRecallType == TRECALLTYPE_SKILL &&
		m_pMON->m_pAI->m_bAIType == MONAI_FIRSTATK)
		return;

	CTPlayer *pCreater = FindHost(m_dwHostID);

	if( m_pMAP && pCreater )
	{
		VTMONSTER vMONS;
		vMONS.clear();

		m_pMAP->GetNeighbor(
			&vMONS,
			m_fPosX,
			m_fPosZ);

		while(!vMONS.empty())
		{
			CTMonster *pAGGROMON = vMONS.back();

			MAPTAGGRO::iterator it = pAGGROMON->m_mapAggro.find(MAKEINT64( m_dwID, m_bType));
			if( it != pAGGROMON->m_mapAggro.end() )
			{
				pAGGROMON->AddAggro(m_dwHostID, m_dwHostID, OT_PC, pCreater->GetWarCountry(), (*it).second.m_dwAggro);
				pAGGROMON->LeaveAggro(m_dwHostID, m_dwID, m_bType);
			}

			vMONS.pop_back();
		}

		vMONS.clear();
	}
}

void CTSelfObj::Recover(DWORD dwTick)
{
}
