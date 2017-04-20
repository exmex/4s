#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"


CTAICmdRegen::CTAICmdRegen()
{
}

CTAICmdRegen::~CTAICmdRegen()
{
}

DWORD CTAICmdRegen::GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	return pMON->m_pSPAWN->m_pSPAWN->m_dwDelay;
}

BYTE CTAICmdRegen::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bStatus != OS_DISAPPEAR ||
        !pMON->m_pSPAWN ||
		!pMON->m_pSPAWN->m_pSPAWN ||
		pMON->m_bRemove)
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdRegen::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if(!pMON->m_pSPAWN->m_pSPAWN->m_bEvent &&
		pMON->m_pSPAWN->m_pSPAWN->m_lpvGroup &&
		!pMON->m_pSPAWN->m_pSPAWN->m_lpvGroup->empty() )
	{
		BYTE bGroupOrder = BYTE(rand() % pMON->m_pSPAWN->m_pSPAWN->m_lpvGroup->size());
		MAPTMONSPAWN::iterator itSp = pMON->m_pMAP->m_mapTMONSPAWN.find(pMON->m_pSPAWN->m_pSPAWN->m_lpvGroup->at(bGroupOrder)->m_wID);
		if(itSp != pMON->m_pMAP->m_mapTMONSPAWN.end())
		{
			if(!(*itSp).second->m_vTMON.empty())
				pMON = (*itSp).second->m_vTMON.back();
		}
	}

	BYTE bRegen = BYTE(rand() % 100) < pMON->m_pSPAWN->m_pSPAWN->m_bProb ? TRUE : FALSE;

	if(bRegen)
	{
		if(!pMON->m_pMON)
		{
			DWORD dwRand = rand();
			DWORD dwTotal = 0;

			for( int i=0; i<INT(pMON->m_pSPAWN->m_pSPAWN->m_vMAPMON.size()); i++)
				if(!pMON->m_pSPAWN->m_pSPAWN->m_vMAPMON[i]->m_bEssential)
					dwTotal += pMON->m_pSPAWN->m_pSPAWN->m_vMAPMON[i]->m_bProb;

			dwRand = dwTotal ? dwRand % dwTotal : 0;
			dwTotal = 0;

			for(int i=0; i<INT(pMON->m_pSPAWN->m_pSPAWN->m_vMAPMON.size()); i++)
				if(!pMON->m_pSPAWN->m_pSPAWN->m_vMAPMON[i]->m_bEssential)
				{
					dwTotal += pMON->m_pSPAWN->m_pSPAWN->m_vMAPMON[i]->m_bProb;

					if( dwRand < dwTotal )
					{
						pMON->m_pMON = _AtlModule.FindTMonster(pMON->m_pSPAWN->m_pSPAWN->m_vMAPMON[i]->m_wMonID);

						if(!pMON->m_pMON)
							_AtlModule.LogEvent("%d Monster have no data in TMONSTERCHART", pMON->m_pSPAWN->m_pSPAWN->m_vMAPMON[i]->m_wMonID);

						pMON->m_pTLEVEL = _AtlModule.FindTLevel(pMON->m_pMON->m_bLevel);
						pMON->m_pTCLASS = _AtlModule.FindTClass(pMON->m_pMON->m_bClass);
						pMON->m_pTRACE = _AtlModule.FindTRace(pMON->m_pMON->m_bRace);
						pMON->m_pATTR = _AtlModule.FindMonAttr(MAKELONG(pMON->m_pMON->m_wMonAttr, pMON->m_pMON->m_bLevel));

						if(!pMON->m_pATTR)
						{
							_AtlModule.LogEvent("%d Monster have no ATTR data in TMONATTRCHART", pMON->m_pSPAWN->m_pSPAWN->m_vMAPMON[i]->m_wMonID);
							ATLTRACE(_T("REGEN ATTR NULL ID:%d ATTR ID:%d\n"), pMON->m_pMON->m_wID, MAKELONG(pMON->m_pMON->m_wMonAttr, pMON->m_pMON->m_bLevel));
							return FALSE;
						}

						pMON->m_bLevel = pMON->m_pMON->m_bLevel;

						for(DWORD ms=0; ms<pMON->m_pMON->m_vSKILL.size(); ms++)
						{
							CTSkillTemp * pTemp = _AtlModule.FindTSkill(pMON->m_pMON->m_vSKILL[ms]);
							if(pTemp)
							{
								CTSkill * pSkill = new CTSkill();
								pSkill->m_pTSKILL = pTemp;
								pSkill->m_bLevel = 1;
								pMON->m_mapTSKILL.insert(MAPTSKILL::value_type(pMON->m_pMON->m_vSKILL[ms], pSkill));
							}
						}

						break;
					}
				}
		}

		if(!pMON->m_pMON)
			return FALSE;

		CTInven * pInven = pMON->FindTInven(INVEN_DEFAULT);
		if(pInven)
		{
			MAPTITEM::iterator it;
			for(it=pInven->m_mapTITEM.begin(); it!=pInven->m_mapTITEM.end(); it++)
				delete (*it).second;
			pInven->m_mapTITEM.clear();
		}

		MAPTITEM::iterator itList;
		for(itList=pMON->m_mapTItemList.begin(); itList!=pMON->m_mapTItemList.end(); itList++)
			delete (*itList).second;
		pMON->m_mapTItemList.clear();

		pMON->m_bCurPathID=0;
		pMON->m_bCurPathDir=1;

		CTMonster* pLeader = NULL;
		if(pMON->m_pSPAWN->m_dwLeader)			
			pLeader = pMON->m_pMAP->FindMonster(pMON->m_pSPAWN->m_dwLeader);

		if(pLeader && pLeader->m_dwID != pMON->m_dwID && pLeader->m_bStatus == OS_WAKEUP)
		{
			FLOAT fRangeX = 0.0f;
			FLOAT fRangeZ = 0.0f;
		
			if(pMON->m_pSPAWN->m_pSPAWN->m_bRange)
			{
				FLOAT fLength = FLOAT(rand() % pMON->m_pSPAWN->m_pSPAWN->m_bRange);
				FLOAT fRad = FLOAT(rand() % 360) * FLOAT(M_PI) / 180.0f;
				fRangeX = fLength * cosf(fRad);
				fRangeZ = fLength * sinf(fRad);
			}

			pMON->m_fStartX = pMON->m_fNextX = pMON->m_fPosX = pLeader->m_fNextX + fRangeX;
			pMON->m_fStartZ = pMON->m_fNextZ = pMON->m_fPosZ = pLeader->m_fNextZ + fRangeZ;
			pMON->m_bCurPathID = pLeader->m_bCurPathID;
			pMON->m_bCurPathDir = pLeader->m_bCurPathDir;
		}
		else if(pMON->m_pSPAWN->m_pSPAWN->m_bRange)
		{
			FLOAT fLength = FLOAT(rand() % pMON->m_pSPAWN->m_pSPAWN->m_bRange);
			FLOAT fRad = FLOAT(rand() % 360) * FLOAT(M_PI) / 180.0f;

			pMON->m_fStartX = pMON->m_fNextX = pMON->m_fPosX = pMON->m_pSPAWN->m_pSPAWN->m_fPosX + fLength * cosf(fRad);
			pMON->m_fStartZ = pMON->m_fNextZ = pMON->m_fPosZ = pMON->m_pSPAWN->m_pSPAWN->m_fPosZ + fLength * sinf(fRad);
		}
		else
		{
			pMON->m_fStartX = pMON->m_fNextX = pMON->m_fPosX = pMON->m_pSPAWN->m_pSPAWN->m_fPosX;
			pMON->m_fStartZ = pMON->m_fNextZ = pMON->m_fPosZ = pMON->m_pSPAWN->m_pSPAWN->m_fPosZ;
		}

		pMON->SetKeeper(0,0);
		pMON->m_fStartY = pMON->m_fNextY = pMON->m_fPosY = pMON->m_pSPAWN->m_pSPAWN->m_fPosY;
		pMON->m_wDIR = pMON->m_pSPAWN->m_pSPAWN->m_wDir;
		pMON->m_fSize = pMON->m_pMON->m_fSize/2;
		pMON->m_dwAtkWaitTick = 0;
		pMON->m_bMode = MT_NORMAL;
		pMON->m_bCanAttack = pMON->m_pMON->m_bCanAttack;
		pMON->m_bIsSpecial = pMON->m_pMON->m_bIsSpecial;
		pMON->m_bLeaveCount = 0;
		pMON->m_dwMoney = 0;
		pMON->m_bRemove = FALSE;

		if(pMON->m_wRegenDelSpawn)
		{
			CTMonSpawn * pMS = pMON->m_pMAP->FindMonSpawn(pMON->m_wRegenDelSpawn);
			if(pMS)
			{
				for(DWORD i=0; i<pMS->m_vTMON.size(); i++)
					pMS->m_vTMON[i]->OnDie(0,OT_NONE,0);
			}
		}

		if(pMON->m_pSPAWN->m_pSPAWN->m_wLocalID)
		{
			LPTLOCAL pLocal = _AtlModule.FindOccupationZone(pMON->m_pSPAWN->m_pSPAWN->m_wLocalID);
			if(pLocal)
				pMON->m_bCountry = pLocal->m_bCountry;
		}

		pMON->m_pNextSkill = NULL;
		pMON->ResetHost();

		pMON->m_dwHP = pMON->GetMaxHP();
		pMON->m_dwMP = pMON->GetMaxMP();

		pMON->m_pMAP->EnterMAP(pMON);
	}

	return !bRegen && CTAICommand::ExecAI( pMON, dwEventHost, dwRHId, bRHType) ? TRUE : FALSE;
}
