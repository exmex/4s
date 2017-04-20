#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"


CTMonster::CTMonster()
{
	m_pNextSkill = NULL;
	m_bStatus = OS_DISAPPEAR;
	m_bType = OT_MON;
	m_bEssential = FALSE;
	m_dwMoney = 0;

	m_dwHostKEY = 0;
	m_dwHostID = 0;

	m_bKeeperType = 0;
	m_dwKeeperID = 0;
	m_dwInvenLock = 0;

	m_bCurPathID=0;
	m_bCurPathDir=1;

	m_pSPAWN = NULL;
	m_pMON = NULL;
	m_pATTR = NULL;

	m_fNextX = m_fStartX = 0;
	m_fNextY = m_fStartY = 0;
	m_fNextZ = m_fStartZ = 0;

	m_bTargetType = OT_PC;
	m_dwTargetID = 0;
	m_dwHelper = 0;
	m_bHelpCount = 0;
	m_bEquipSpecial = FALSE;
	m_bRemove = FALSE;
	m_wRegenDelSpawn = 0;

	m_mapGuildDamage.clear(); 
	m_mapDamage.clear();
	m_mapAggro.clear();
	m_mapTItemList.clear();
}

CTMonster::~CTMonster()
{
	MAPTITEM::iterator it;
	for(it=m_mapTItemList.begin(); it!=m_mapTItemList.end(); it++)
		delete (*it).second;
	m_mapTItemList.clear();
}

BYTE CTMonster::GetColor(DWORD dwCharID, DWORD dwPartyID, BYTE bCountry)
{
	BYTE bKeepType;
	DWORD dwKeepID;
	if(dwPartyID)
	{
		bKeepType = OWNER_PARTY;
		dwKeepID = dwPartyID;
	}
	else
	{
		bKeepType = OWNER_PRIVATE;
		dwKeepID = dwCharID;
	}

	if(GetWarCountry() == bCountry)
		return TNCOLOR_ALLI;
	else if(GetWarCountry() != TCONTRY_N)
		return TNCOLOR_ENEMY;
	else if(m_bMode == MT_GOHOME)
		return TNCOLOR_PIECE;
	if(m_dwKeeperID == dwKeepID && m_bKeeperType == bKeepType)
		return TNCOLOR_ENEMY;
	else if(m_dwKeeperID)
		return TNCOLOR_NO_REWARD;
	else if(m_bMode == MT_BATTLE)
		return TNCOLOR_ENEMY;
	else if(m_pMON->m_pAI->m_bAIType == MONAI_FIRSTATK)
		return TNCOLOR_ENEMY;
	else
		return TNCOLOR_PIECE;
}

BYTE CTMonster::LeaveAggro(DWORD dwHostID, DWORD dwRHId, BYTE bRHTType)
{
	MAPTAGGRO::iterator it = m_mapAggro.find(MAKEINT64(dwRHId, bRHTType));
	if(it!=m_mapAggro.end())
		m_mapAggro.erase(it);
	else if(dwRHId == m_dwTargetID && bRHTType == m_bTargetType)
	{
		OnEvent( AT_LEAVELB, 0, dwHostID, dwRHId, bRHTType);
		return FALSE;
	}

	DWORD dwAggro=0;
	TAGGRO aggro;

	for(it=m_mapAggro.begin(); it!=m_mapAggro.end(); it++)
		if(dwAggro < (*it).second.m_dwAggro &&
			(*it).second.m_bCountry != m_bCountry)
		{
			dwAggro = (*it).second.m_dwAggro;
			aggro.Copy((*it).second);
		}

	if(m_bMode != MT_GOHOME)
	{
		if(aggro.m_dwObjID)
		{
			CTObjBase * pTarget = m_pMAP->FindNeighbor(
				aggro.m_bObjType,
				aggro.m_dwObjID,
				m_fPosX,
				m_fPosZ);

			if( pTarget &&
				(aggro.m_bObjType != m_bTargetType ||
				aggro.m_dwObjID != m_dwTargetID))
			{
				OnEvent(AT_DEFEND, 0 , aggro.m_dwHostID, aggro.m_dwObjID, aggro.m_bObjType);
				return TRUE;
			}
			else if(!pTarget)
			{
				LeaveAggro(aggro.m_dwHostID, aggro.m_dwObjID, aggro.m_bObjType);
				return FALSE;
			}
		}
		else
		{
			OnEvent( AT_LEAVELB, 0, dwHostID, dwRHId, bRHTType);
			return FALSE;
		}
	}

	return FALSE;
}
BYTE CTMonster::SetAggro(DWORD dwHostID,
						 DWORD dwAttackID,
						 BYTE bAttackType,
						 BYTE bAttackCountry,
						 BYTE bAttackClass,
						 DWORD dwTarget,
						 BYTE bTargetType,
						 INT nAggro,
						 BYTE bActive)
{
	if( nAggro == 0 ||
		bAttackType == OT_MON ||
		!dwAttackID)
		return FALSE;

	if(m_bMode == MT_GOHOME && !bActive)
		return FALSE;

	if(bAttackCountry == GetWarCountry())
		return FALSE;

	if(bAttackClass == TCLASS_WARRIOR)
		nAggro = nAggro * 3 / 2;

	DWORD dwNew = 0;
	DWORD dwOld = 0;
	MAPTAGGRO::iterator it = m_mapAggro.find(MAKEINT64(m_dwTargetID, m_bTargetType));
	if(it!=m_mapAggro.end())
		dwOld = (*it).second.m_dwAggro;

	it=m_mapAggro.find(MAKEINT64(dwAttackID,bAttackType));
	if(it!=m_mapAggro.end())
	{
		INT nTemp = (*it).second.m_dwAggro + nAggro;
		(nTemp < 0) ? (*it).second.m_dwAggro = 0 : (*it).second.m_dwAggro += nAggro;
		dwNew = (*it).second.m_dwAggro;
	}
	else if(bActive)
	{
		TAGGRO aggro;
		aggro.m_dwHostID = dwHostID;
		aggro.m_dwObjID = dwAttackID;
		aggro.m_bObjType = bAttackType;
		aggro.m_dwAggro = nAggro;
		aggro.m_bCountry = bAttackCountry;
		m_mapAggro.insert(MAPTAGGRO::value_type(MAKEINT64(dwAttackID, bAttackType), aggro));
		dwNew = nAggro;
	}
	else
	{
		it = m_mapAggro.find(MAKEINT64(dwTarget, bTargetType));
		if(it!=m_mapAggro.end())
		{
			TAGGRO aggro;
			aggro.m_dwHostID = dwHostID;
			aggro.m_dwObjID = dwAttackID;
			aggro.m_bObjType = bAttackType;
			aggro.m_dwAggro = nAggro;
			aggro.m_bCountry = bAttackCountry;
			m_mapAggro.insert(MAPTAGGRO::value_type(MAKEINT64(dwAttackID, bAttackType), aggro));
			dwNew = nAggro;
		}
	}

	if(nAggro < 0 && m_bMode == MT_BATTLE)
	{
		// Aggro 감소
		DWORD dwAggro=0;
		TAGGRO aggro;

		for(it=m_mapAggro.begin(); it!=m_mapAggro.end(); it++)
		{
			if(dwAggro < (*it).second.m_dwAggro &&
				(*it).second.m_bCountry != m_bCountry)
			{
				dwAggro = (*it).second.m_dwAggro;
				aggro.Copy((*it).second);
			}
		}

		if( !m_mapAggro.empty() &&
			(aggro.m_bObjType != m_bTargetType || aggro.m_dwObjID != m_dwTargetID) &&
			(dwOld + dwOld * 0.1 < aggro.m_dwAggro))
			OnEvent(AT_DEFEND, 0, aggro.m_dwHostID, aggro.m_dwObjID, aggro.m_bObjType);

		return TRUE;
	}
	else
	{
		// Aggro 증가
		if((dwNew && m_bMode != MT_BATTLE) ||
			((m_bTargetType != bAttackType ||
			m_dwTargetID != dwAttackID ) &&
			dwOld + dwOld * 0.1 < dwNew))
		{
			OnEvent(AT_DEFEND, 0, dwHostID, dwAttackID, bAttackType);
			return TRUE;
		}
	}

	return FALSE;
}

DWORD CTMonster::FindAggro(DWORD dwID, BYTE bType)
{
	DWORD dwAggro = 0;

	MAPTAGGRO::iterator it = m_mapAggro.find(MAKEINT64(dwID, bType));
	if(it!=m_mapAggro.end())
		dwAggro = (*it).second.m_dwAggro;

	return dwAggro;
}

void CTMonster::SetEventToFollower()
{
	if(m_pSPAWN->m_dwLeader == m_dwID)
	{
		for(DWORD i=0; i<m_pSPAWN->m_vTMON.size(); i++)
		{
			CTMonster * pFollower = m_pMAP->FindMonster(m_pSPAWN->m_vTMON[i]->m_dwID);
			if(pFollower && !pFollower->m_dwTargetID)
				pFollower->OnEvent(AT_DEFEND, 0, m_dwHostID, m_dwTargetID, m_bTargetType);
		}
	}
	else if(m_pSPAWN->m_pSPAWN->m_wPartyID)
	{
		for(DWORD i=0; i<m_pSPAWN->m_pSPAWN->m_vPARTY.size(); i++)
		{
			CTMonSpawn * pSPAWN = m_pMAP->FindMonSpawn(m_pSPAWN->m_pSPAWN->m_vPARTY[i]);

			for(DWORD j=0; pSPAWN && j<pSPAWN->m_vTMON.size(); j++)
			{
				CTMonster * pParty = m_pMAP->FindMonster(pSPAWN->m_vTMON[j]->m_dwID);
				if(pParty && !pParty->m_dwTargetID)
					pParty->OnEvent(AT_DEFEND, 0, m_dwHostID, m_dwTargetID, m_bTargetType);
			}
		}
	}
}

void CTMonster::MoveFollower(FLOAT fPosX, FLOAT fPosY, FLOAT fPosZ)
{
	for(DWORD i=0; i<m_pSPAWN->m_vTMON.size(); i++)
	{
		CTMonster * pFollower = m_pSPAWN->m_vTMON[i];
		if( pFollower->m_dwID != m_dwID &&
			pFollower->m_bStatus == OS_WAKEUP &&
			pFollower->m_bMode == MT_NORMAL &&
			!pFollower->IsActionBlock() &&
			GetDistance(pFollower->m_fPosX, pFollower->m_fPosZ, fPosX, fPosZ) < MAX_ROAMRANGE)
		{
			CTPlayer * pFH = pFollower->FindHost(pFollower->m_dwHostID);
			if(pFH)
			{
				FLOAT fRad = FLOAT(rand() % 360) * FLOAT(M_PI) / 180.0f;
				FLOAT fX = fPosX + 3 * cosf(fRad);
				FLOAT fY = fPosY;
				FLOAT fZ = fPosZ + 3 * sinf(fRad);
				BYTE bAction;
				if(GetDistance(pFollower->m_fPosX, pFollower->m_fPosZ, m_fPosX, m_fPosZ) < MAX_WALKBOUND)
					bAction = TA_WALK;		
				else 
					bAction = TA_RUN;
				
				pFH->SendCS_MONACTION_ACK(
					pFollower->m_dwID,
					bAction,
					fX,
					fY,
					fZ);

				pFollower->m_fStartX = pFollower->m_fPosX;
				pFollower->m_fStartY = pFollower->m_fPosY;
				pFollower->m_fStartZ = pFollower->m_fPosZ;
				pFollower->m_bCurPathID = m_bCurPathID;
				pFollower->m_bCurPathDir = m_bCurPathDir;
			}
		}
	}
}

void CTMonster::AddAggro(DWORD dwHostID, DWORD dwTarget, BYTE bTargetType, BYTE bCountry, DWORD dwAggro)
{
	MAPTAGGRO::iterator it = m_mapAggro.find(MAKEINT64(dwTarget, bTargetType));
	if(it!=m_mapAggro.end())
		(*it).second.m_dwAggro += dwAggro;
	else
	{
		TAGGRO aggro;
		aggro.m_dwHostID = dwHostID;
		aggro.m_dwObjID = dwTarget;
		aggro.m_bObjType = bTargetType;
		aggro.m_dwAggro = dwAggro;
		aggro.m_bCountry = bCountry;
		m_mapAggro.insert(MAPTAGGRO::value_type(MAKEINT64(dwTarget, bTargetType), aggro));
	}
}

void CTMonster::DelAggro(DWORD dwTarget, BYTE bTargetType)
{
	MAPTAGGRO::iterator it = m_mapAggro.find(MAKEINT64(dwTarget, bTargetType));
	if(it!=m_mapAggro.end())
		m_mapAggro.erase(it);
}

DWORD CTMonster::OnDamage( DWORD dwHostID,
						   DWORD dwAttackID,
						   BYTE bAttackType,
						   BYTE bAttackCountry,
						   BYTE bAttackAidCountry,
						   BYTE bAtkCanSelect,
						   WORD wPartyID,
						   int nDamageHP,
						   int nDamageMP)
{
	CTObjBase::OnDamage(
		dwHostID,
		dwAttackID,
		bAttackType,
		bAttackCountry,
		bAttackAidCountry,
		bAtkCanSelect,
		wPartyID,
		nDamageHP,
		nDamageMP);

	if(nDamageHP <= 0)
		return 0;

	DWORD dwDamage = nDamageHP;

	if( m_pMON->m_bCall &&
		!m_bHelpCount &&
		m_dwHP < GetMaxHP() / 2 )
	{
		m_bGetaway = TRUE;
		OnEvent( AT_HELP, 0, 0, 0, 0);
	}

	if(bAttackType != OT_MON)
	{
		__int64 nKey = wPartyID ? MAKEINT64(wPartyID,0) : dwHostID;
		__int64 nValue = MAKEINT64(dwHostID,dwDamage);

		BYTE bType = wPartyID ? OWNER_PARTY : OWNER_PRIVATE;
		DWORD dwKeeper = wPartyID ? wPartyID : dwHostID;
		DWORD dwTotal = dwDamage;

		MAPINT64::iterator it = m_mapDamage.find(nKey);
		if(it!=m_mapDamage.end())
		{
			(*it).second += dwDamage;
			dwTotal = DWORD((*it).second);
			nValue = MAKEINT64(dwHostID,dwTotal);
			(*it).second = nValue;
		}
		else
			m_mapDamage.insert(MAPINT64::value_type(nKey, nValue));

		// 길드 데미지 저장
		DWORD dwGuildID = 0;		
		MAPDWORD::iterator itG;
		CTPlayer* pPlayer = _AtlModule.FindChar(dwHostID);
		if(pPlayer &&
			pPlayer->GetWarCountry() < TCONTRY_B &&
            (pPlayer->m_bCountry  < TCONTRY_B || pPlayer->m_dwTacticsID))
			dwGuildID = pPlayer->GetGuild();

		if(dwGuildID)
		{
			itG = m_mapGuildDamage.find(dwGuildID);
			if(itG != m_mapGuildDamage.end() )			
				(*itG).second += dwDamage;
			else
                m_mapGuildDamage.insert(MAPDWORD::value_type(dwGuildID,dwDamage) );
		}

		if(!((CTMonster *)this)->m_dwKeeperID &&
			dwTotal >= GetMaxHP()*MONKEEP_PER/100)
		{
			VPLAYER vPlayers;
			vPlayers.clear();

			m_pMAP->GetNeighbor(
				&vPlayers,
				m_fPosX,
				m_fPosZ);
			SetKeeper( bType, dwKeeper);

			for( DWORD i=0; i<vPlayers.size(); i++)
			{
				vPlayers[i]->SendCS_CHANGECOLOR_ACK(
					m_dwID,
					m_bType,
					GetColor( vPlayers[i]->m_dwID, vPlayers[i]->GetPartyID(), vPlayers[i]->GetWarCountry()),
					GetWarCountry());
			}

			vPlayers.clear();
		}
	}

	return dwDamage;
}

void CTMonster::OnEvent( AI_TRIGGER nEvent,
						 DWORD dwTriggerID,
						 DWORD dwEventHost,
						 DWORD dwRHId,
						 BYTE bRHType)
{
	if(m_bType != OT_MON)
		return;

	CTMonsterAI *pAI = m_pMON && m_pMON->m_pAI ? m_pMON->m_pAI : _AtlModule.FindTMonsterAI(DEFAULT_AI);
	MAPVTAICOMMAND::iterator itAI = pAI->m_mapVCOMMAND[nEvent].find(dwTriggerID);

	if( itAI != pAI->m_mapVCOMMAND[nEvent].end() )
	{
		LPVTAICOMMAND pVCMD = (*itAI).second;

		for( int i=0; i<INT(pVCMD->size()); i++)
			_AtlModule.DoAICMD( (*pVCMD)[i], this, dwEventHost, dwRHId, bRHType, dwTriggerID);
	}
}

void CTMonster::Initialize()
{
	CTMonsterAI *pAI = m_pMON && m_pMON->m_pAI ? m_pMON->m_pAI : _AtlModule.FindTMonsterAI(DEFAULT_AI);
	MAPVTAICOMMAND::iterator itAI = pAI->m_mapVCOMMAND[AT_DELETE].find(0);

	if( itAI != pAI->m_mapVCOMMAND[AT_DELETE].end() )
	{
		LPVTAICOMMAND pVCMD = (*itAI).second;

		for( int i=0; i<INT(pVCMD->size()); i++)
		{
			if((*pVCMD)[i]->CanRun( this, 0, 0, 0))
					(*pVCMD)[i]->ExecAI( this, 0, 0, 0);
		}
	}
}

CTPlayer *CTMonster::FindHost( DWORD dwHostID)
{
	CTPlayer *pResult = NULL;

	if(m_pMAP)
	{
		MAPTCELL::iterator it;
		MAPTCELL mapCELL;

		mapCELL.clear();

		m_pMAP->GetNeighbor(
			&mapCELL,
			m_fPosX,
			m_fPosZ);

		for( it = mapCELL.begin(); it != mapCELL.end(); it++)
		{
			MAPPLAYER::iterator finder = (*it).second->m_mapPLAYER.find(dwHostID);
  
			if( finder != (*it).second->m_mapPLAYER.end() )
			{
				pResult = (*finder).second;
				break;
			}
		}

		mapCELL.clear();
	}

	return pResult;
}

BYTE CTMonster::GetLevel()
{
	return m_pMON->m_bLevel;
}

BYTE CTMonster::GetClass()
{
	return m_pMON->m_bClass;
}

void CTMonster::OnDie( DWORD dwAttackID , BYTE bObjectType, WORD wTempMonID ) 
{
	if( m_bStatus == OS_DEAD || Transformation() )
		return;

	CTObjBase::OnDie(dwAttackID,bObjectType,wTempMonID);

	WORD wAddMagicProb = 0;
	int nAddItemDrop = 0;
	int nMaxLevel = 0;

	__int64 nKey = m_bKeeperType == OWNER_PARTY ? __int64(m_dwKeeperID) << 32 : m_dwKeeperID;
	MAPINT64::iterator it = m_mapDamage.find(nKey);
	DWORD dwDamage = 0;
	BYTE bCountry = TCONTRY_N;

	if( it != m_mapDamage.end() )
		dwDamage = DWORD((*it).second);
/*
#ifdef DEF_UDPLOG	//	08-02-04 Duke Kim
	_AtlModule.m_pUdpSocket->LogMonster(LOGMAP_MONDIE, NULL, this );						
#endif
*/
	if(dwDamage)
		switch(m_bKeeperType)
		{
		case OWNER_PARTY	:
			{
				WORD wTotalLevel = 0;

				VPLAYER vPlayer;
				VPLAYER vParty;

				vPlayer.clear();
				vParty.clear();

				m_pMAP->GetNeighbor(
					&vPlayer,
					m_fPosX,
					m_fPosZ);

				for( DWORD i=0; i<DWORD(vPlayer.size()); i++)
					if( m_dwKeeperID == vPlayer[i]->GetPartyID() )
					{
						bCountry = vPlayer[i]->GetWarCountry();
						wTotalLevel += vPlayer[i]->m_bLevel;
						vParty.push_back(vPlayer[i]);
						if(nMaxLevel < vPlayer[i]->m_bLevel)
							nMaxLevel = vPlayer[i]->m_bLevel;
					}

				if(wTotalLevel == 0)
					wTotalLevel = 100;

				WORD wTotalMember = WORD(vParty.size());
				DWORD dwTotalExp = DWORD(GetExp(dwDamage) * (1.0f + 0.01f * ((wTotalMember * wTotalMember) / 2.0f + wTotalMember - 1.5f)));
				BYTE bCheckQuest = FALSE;

				for(int i=0; i<DWORD(wTotalMember); i++)
				{
					DWORD dwSharedExp = dwTotalExp * vParty[i]->m_bLevel / wTotalLevel;
					DWORD dwEXP = DWORD(FLOAT(dwSharedExp) * vParty[i]->GetLevelRate(m_bLevel) + 0.99);

					if(vParty[i]->m_bMain)
					{
						if(vParty[i]->m_dwID == dwAttackID)
						{
							if(vParty[i]->m_bInPcBang & (PCBANG_REAL | PCBANG_PREMIUM1))
								wAddMagicProb = PCBANG_DROPMAGICPROB;

							wAddMagicProb += vParty[i]->GetMagicDropBuff();
							nAddItemDrop = vParty[i]->HaveItemDropBuff(m_pMON->m_bItemProb);
						}

						WORD wBonus = 0;
						vParty[i]->IsExpBenefit(wBonus);
						wBonus += vParty[i]->GetGainExpBuff();

						if(wBonus)
						{
							dwEXP += dwEXP * wBonus / 100;
						}

						if(dwEXP)
						{
							vParty[i]->CheckSoulEXP(vParty, dwEXP);
							vParty[i]->GainExp( dwEXP, TRUE);
						}
#ifdef DEF_UDPLOG
							_AtlModule.m_pUdpSocket->LogExpByMonster(LOGMAP_MONEXPPARTY, vParty[i], this, dwEXP);						
#endif

						if(!bCheckQuest || !m_pMON->m_bCheckPartyQuest)
						{
							_AtlModule.CheckQuest(
								vParty[i],
								m_dwID,
								m_fPosX,
								m_fPosY,
								m_fPosZ,
								m_pMON->m_wKind,
								QTT_HUNT,
								TT_KILLMON, 1);

							bCheckQuest = TRUE;
						}
					}
					else
					{
						_AtlModule.SendMW_MONSTERDIE_ACK(
							vParty[i]->m_dwID,
							vParty[i]->m_dwKEY,
							dwEXP,
							m_dwID,
							m_fPosX,
							m_fPosY,
							m_fPosZ,
							m_pMON->m_wKind,
							QTT_HUNT,
							TT_KILLMON, 1);
#ifdef DEF_UDPLOG
                        _AtlModule.m_pUdpSocket->LogExpByMonster(LOGMAP_MONEXPPARTY, vParty[i], this, dwEXP, TRUE);						
#endif
					}
				}

				vPlayer.clear();
				vParty.clear();
			}

			break;

		case OWNER_PRIVATE	:
			{
				CTPlayer * pOwner = FindHost(m_dwKeeperID);
				if(pOwner && pOwner->m_bMain)
				{
					nMaxLevel = pOwner->m_bLevel;
					DWORD dwEXP = DWORD(FLOAT(GetExp(dwDamage)) * pOwner->GetLevelRate(m_bLevel) + 0.99);
					bCountry = pOwner->GetWarCountry();

					if(pOwner->m_bMain)
					{
						if(pOwner->m_bInPcBang & (PCBANG_REAL | PCBANG_PREMIUM1))
							wAddMagicProb = PCBANG_DROPMAGICPROB;

						wAddMagicProb += pOwner->GetMagicDropBuff();
						nAddItemDrop = pOwner->HaveItemDropBuff(m_pMON->m_bItemProb);

						WORD wBonus = 0;
						pOwner->IsExpBenefit(wBonus);
						wBonus += pOwner->GetGainExpBuff();

						if(wBonus)
						{
							dwEXP += dwEXP * wBonus / 100;
						}

						if(dwEXP)
							pOwner->GainExp( dwEXP, TRUE);
#ifdef DEF_UDPLOG
						_AtlModule.m_pUdpSocket->LogExpByMonster(LOGMAP_MONEXPSOLO, pOwner, this, dwEXP);						
#endif

						_AtlModule.CheckQuest(
							pOwner,
							m_dwID,
							m_fPosX,
							m_fPosY,
							m_fPosZ,
							m_pMON->m_wKind,
							QTT_HUNT,
							TT_KILLMON, 1);
					}
					else
					{
						_AtlModule.SendMW_MONSTERDIE_ACK(
							pOwner->m_dwID,
							pOwner->m_dwKEY,
							dwEXP,
							m_dwID,
							m_fPosX,
							m_fPosY,
							m_fPosZ,
							m_pMON->m_wKind,
							QTT_HUNT,
							TT_KILLMON, 1);
#ifdef DEF_UDPLOG
						_AtlModule.m_pUdpSocket->LogExpByMonster(LOGMAP_MONEXPSOLO, pOwner, this, dwEXP, TRUE);						
#endif
					}
				}
			}
			break;
		}

	if( m_pSPAWN && m_pSPAWN->m_bStatus == MONSPAWN_SUSPEND )
		OnBattleZoneEvent(dwAttackID);
	 
	LPEVENTINFO pEventDrop = _AtlModule.GetEventInfo((BYTE)EVENT_ITEMDROP);
	if( pEventDrop && (pEventDrop->m_wMapID == m_pMAP->m_wMapID || pEventDrop->m_wMapID == 0xFF) )
		nAddItemDrop = nAddItemDrop + m_pMON->m_bItemProb * _AtlModule.m_wEventValue[EVENT_ITEMDROP] / 100;
	LPEVENTINFO pEventMagic = _AtlModule.GetEventInfo((BYTE)EVENT_ITEMMAGICDROP);
	if(pEventMagic && (pEventMagic->m_wMapID == m_pMAP->m_wMapID || pEventMagic->m_wMapID == 0xFF))
		wAddMagicProb = wAddMagicProb + _AtlModule.m_wEventValue[EVENT_ITEMMAGICDROP];

	if(nMaxLevel > m_bLevel)
	{
		if(nMaxLevel - m_bLevel < 25)
			AddItem(bCountry, wAddMagicProb, nAddItemDrop);
/*
		switch(_AtlModule.GetNation())
		{
		case NATION_KOREA:
			if(nMaxLevel - m_bLevel < 25)
				AddItem(bCountry, wAddMagicProb, nAddItemDrop);
			break;
		case NATION_JAPAN:
			AddItem(bCountry, wAddMagicProb, nAddItemDrop);
			break;
		case NATION_TAIWAN:
			if(nMaxLevel - m_bLevel < 15)
				AddItem(bCountry, wAddMagicProb, nAddItemDrop);
			break;
		default:
			if(nMaxLevel - m_bLevel < 10)
				AddItem(bCountry, wAddMagicProb, nAddItemDrop);
			break;
		}
*/
	}
	else
		AddItem(bCountry, wAddMagicProb, nAddItemDrop);

	OnEvent( AT_DEAD, 0, dwAttackID, 0, 0);
}

void CTMonster::Defend( CTSkillTemp * pTemp,
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
	if(	pTemp->IsNegative())
	{
		SetAggro(
			dwHostID,
			bCanSelect ? dwAttackID : dwHostID,
			bCanSelect ? bAttackType : OT_PC,
			GetAttackCountry(bAttackCountry, bAttackAidCountry),
			bAttackClass,
			0, 0,
			max(1,pTemp->GetAggro(bSkillLevel)),
			TRUE);
	}

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

	if(m_bType == OT_MON &&
		m_bStatus != OS_DEAD &&
		m_pMAP &&
		(m_dwHP != GetMaxHP() || !m_vMaintainSkill.empty()))
	{
		MAPTONTIMEMON::iterator itOTM = _AtlModule.m_mapTOnTimeMon.find(m_dwID);
		if(itOTM == _AtlModule.m_mapTOnTimeMon.end())
		{
			TONTIMEMON mon;
			mon.m_bChannel = m_bChannel;
			mon.m_dwID = m_dwID;
			mon.m_wMapID = m_pMAP->m_wMapID;
			mon.m_wPartyID = m_pMAP->m_wPartyID;
			_AtlModule.m_mapTOnTimeMon.insert(MAPTONTIMEMON::value_type(m_dwID, mon));
		}
	}
}

void CTMonster::SetKeeper(BYTE bType, DWORD dwKeeper)
{
	m_bKeeperType = bType;
	m_dwKeeperID = dwKeeper;
}
CTItem * CTMonster::SelectItem(BYTE bItemID)
{
	CTInven * pInven = FindTInven(INVEN_DEFAULT);
	if(!pInven) return NULL;

	pInven->FindTItem(bItemID);
	return NULL;
}
DWORD CTMonster::GetHunter()
{
	if(m_bKeeperType == OWNER_PARTY)
	{
		MAPTAGGRO::iterator it;
		DWORD dwAggro=0;
		TAGGRO aggro;

		for(it=m_mapAggro.begin(); it!=m_mapAggro.end(); it++)
			if(dwAggro < (*it).second.m_dwAggro)
			{
				dwAggro = (*it).second.m_dwAggro;
				aggro.Copy((*it).second);
			}

		if(dwAggro)
			return aggro.m_dwHostID;
	}

	return 0;
}

BYTE CTMonster::EraseItem(BYTE bItemID)
{
	CTInven * pInven = FindTInven(INVEN_DEFAULT);
	if(!pInven) return FALSE;

	MAPTITEM::iterator it = pInven->m_mapTITEM.find(bItemID);
	if(it != pInven->m_mapTITEM.end())
	{
		pInven->m_mapTITEM.erase(it);
		return TRUE;
	}
	return FALSE;
}
CTItem * CTMonster::GetItem(WORD wItemID)
{
	CTInven * pInven = FindTInven(INVEN_DEFAULT);
	if(!pInven) return NULL;

	MAPTITEM::iterator it;
	for(it=pInven->m_mapTITEM.begin(); it!=pInven->m_mapTITEM.end(); it++)
		if((*it).second->m_wItemID == wItemID)
			return (*it).second;

	return NULL;
}

void CTMonster::AddItem(CTItem * pItem)
{
	CTInven * pInven = FindTInven(INVEN_DEFAULT);
	if(!pInven)
	{
		delete pItem;
		return;
	}

	BYTE bItemID = pInven->GetBlankPos();
	if( bItemID == INVALID_SLOT )
	{
		delete pItem;
		return;
	}

	pItem->m_bItemID = bItemID;
	pItem->m_dlID = _AtlModule.GenItemID();
	_AtlModule.SetItemAttr(pItem, 0);
	pItem->SetGLevel(m_bLevel);
	pItem->SetDuration(TRUE);
	pInven->m_mapTITEM.insert( MAPTITEM::value_type( pItem->m_bItemID, pItem));
}
void CTMonster::AddItem(BYTE bCountry, WORD wAddMagicProb, INT nAddItemDrop)
{
	if(!m_pMON->m_dwMaxWeight)
		return;

	if( m_pMON->m_bMoneyProb > rand() % 100 &&
		m_pMON->m_dwMaxMoney > m_pMON->m_dwMinMoney)
			m_dwMoney = m_pMON->m_dwMinMoney + rand() % (m_pMON->m_dwMaxMoney - m_pMON->m_dwMinMoney);
	else
		m_dwMoney = 0;

	 
	LPEVENTINFO pEVENT = _AtlModule.GetEventInfo((BYTE)EVENT_MONEYDROP);
	if(pEVENT && (pEVENT->m_wMapID == m_pMAP->m_wMapID || pEVENT->m_wMapID == 0xFF))
        m_dwMoney += m_dwMoney * _AtlModule.m_wEventValue[EVENT_MONEYDROP]/100;

	BYTE bItemID=0;
	CTInven * pInven = FindTInven(INVEN_DEFAULT);
	if(!pInven) return;

	BYTE j = 0;

	for(DWORD i=0; !m_pMON->m_vMONITEM.empty() && i<m_pMON->m_bDropCount; i++)
	{
		if(m_pMON->m_bItemProb + nAddItemDrop > rand() % 100)
		{
			BYTE bBlank =  pInven->GetBlankPos();
			if(bBlank == INVALID_SLOT)
				return;

			DWORD dwChoice = TRand(m_pMON->m_dwMaxWeight);
			DWORD dwCur = 0;
			BYTE bSelected = 0;
			for(j = 0; j < m_pMON->m_vMONITEM.size(); j++)
			{
				dwCur += m_pMON->m_vMONITEM[j]->m_wWeight;
				if( dwChoice < dwCur )
				{
					bSelected = j;
					break;
				}
			}

			if(m_pMON->m_vMONITEM[bSelected]->m_bItemProb[MIP_NORMAL1] > rand() % 100 &&
				m_pMON->m_vMONITEM[bSelected]->m_bItemProb[MIP_NORMAL2] > rand() % 100 &&
				m_pMON->m_vMONITEM[bSelected]->m_bItemProb[MIP_NORMAL3] > rand() % 100 &&
				m_pMON->m_vMONITEM[bSelected]->m_bItemProb[MIP_NORMAL4] > rand() % 100)
			{
				CTItem * pItem = NULL;

				if(m_pMON->m_vMONITEM[bSelected]->m_bChartType)
				{
					LPTITEM pITEM = m_pMON->m_vMONITEM[bSelected]->m_pTITEM;

					LPTITEM pTemp = pITEM ? pITEM : _AtlModule.MonChoiceItem(m_pMON->m_vMONITEM[bSelected], bCountry);
					if(!pTemp)
						return;

					pItem = new CTItem();
					pItem->m_dlID = _AtlModule.GenItemID();
					pItem->m_bItemID = bBlank;
					pItem->m_wItemID = pTemp->m_wItemID;
					pItem->m_pTITEM = pTemp;
					pItem->SetDuration(TRUE);
					_AtlModule.SetItemAttr(pItem, 0);
					pItem->m_bCount = 1;
					if(!pItem->SetGLevel(m_bLevel))
						MakeSpecialItem(m_pMON->m_vMONITEM[bSelected], pItem, wAddMagicProb);
				}
				else
				{
					CTItem * pMagicItem = m_pMON->m_vMONITEM[bSelected]->m_pMagicItem ?
						m_pMON->m_vMONITEM[bSelected]->m_pMagicItem : _AtlModule.MonChoiceMagicItem(m_pMON->m_vMONITEM[bSelected], bCountry);

					if(!pMagicItem)
						return;

					pItem = new CTItem();
					pItem->Copy(pMagicItem, TRUE);
					pItem->m_bItemID = bBlank;
					pItem->SetEndTime(DWORD(pItem->m_dEndTime) * DAY_ONE);
				}

				if(pItem)
					pInven->m_mapTITEM.insert(MAPTITEM::value_type(pItem->m_bItemID, pItem));
			}
		}
	}
}

void CTMonster::AddItemList(CTItem * pItem)
{
	m_mapTItemList.insert(MAPTITEM::value_type(pItem->m_bItemID, pItem));
}

CTItem * CTMonster::EraseItemList(BYTE bItemID)
{
	CTItem * pItem = NULL;

	MAPTITEM::iterator find = m_mapTItemList.find(bItemID);
	if(find!=m_mapTItemList.end())
	{
		pItem = (*find).second;
		m_mapTItemList.erase(bItemID);
	}

	return pItem;
}

void CTMonster::MakeSpecialItem(LPTMONITEM pMonItem, CTItem * pItem, WORD wAddMagicProb)
{
	/*
	if(pMonItem->m_bItemProb[MIP_MAGIC] > rand() % 100)
	{
		if(pMonItem->m_bItemProb[MIP_SET] > rand() % 100)
		{
			if(pMonItem->m_bItemProb[MIP_RARE] > rand() % 100)
			{
				BYTE bCnt = MAX(BYTE(rand() % pMonItem->m_bItemRareOp)+1, 3);
				for(BYTE i=0; i<bCnt; i++)
					SetMagicOpt(pItem);
			}
			else
			{
				MakeSetItem(pItem);
			}
		}
		else
		{
			BYTE bCnt = MAX(BYTE(rand() % pMonItem->m_bItemMagicOpt)+1, 1);
			for(BYTE i=0; i<bCnt; i++)
				SetMagicOpt(pItem);
		}
	}
	*/
	if(!pItem || pItem->m_bLevel)
		return;

	MAPTMAGIC::iterator it;
	for(it=pItem->m_mapTMAGIC.begin(); it!=pItem->m_mapTMAGIC.end(); it++)
		delete (*it).second;
	pItem->m_mapTMAGIC.clear();

	WORD wMagicProb;

	if(_AtlModule.GetNation() == NATION_KOREA)
		wMagicProb = pMonItem->m_bItemProb[MIP_MAGIC];
	else
		wMagicProb = pMonItem->m_bItemProb[MIP_MAGIC] * 2 /3;

	wMagicProb = wAddMagicProb ? wMagicProb + wMagicProb * wAddMagicProb / 100 : wMagicProb;

	if(pItem->m_pTITEM->m_bCanMagic && wMagicProb > rand() % 100)
	{
		if(pItem->m_pTITEM->m_bCanRare &&
			pMonItem->m_bItemProb[MIP_RARE] > rand() % 100 &&
			pMonItem->m_bItemRareOpt)
		{
			BYTE bCnt = min(max(BYTE(rand() % pMonItem->m_bItemRareOpt)+1, 3), 5);
			for(BYTE i=0; i<bCnt; i++)
				SetMagicOpt(pItem, MIP_RARE);
		}
		else if(pMonItem->m_bItemMagicOpt)
		{
			BYTE bCnt = min(max(BYTE(rand() % pMonItem->m_bItemMagicOpt)+1, 1), 2);
			for(BYTE i=0; i<bCnt; i++)
				SetMagicOpt(pItem, MIP_MAGIC);
		}
	}
}
void CTMonster::MakeSetItem(CTItem * pItem)
{
	VTITEMSET vItemSet;
	vItemSet.clear();

	for(DWORD i=0; i<_AtlModule.m_vItemSet.size(); i++)
		if(_AtlModule.m_vItemSet[i]->m_wBaseID == pItem->m_pTITEM->m_wItemID)
			vItemSet.push_back(_AtlModule.m_vItemSet[i]);

	if(!vItemSet.empty())
	{
		BYTE bSel = BYTE(rand() % vItemSet.size());
		LPTITEMSET pSet = vItemSet[bSel];
		for(BYTE j=0; j<TMAGIC_MAX; j++)
		{
			if(pSet->m_pMagic[j] && pSet->m_bValue[j])
			{
				LPTMAGIC pMagic = new TMAGIC();
				pMagic->m_wValue = pSet->m_bMin[j] + rand() % (pSet->m_bValue[j] + 1);
				pMagic->m_pMagic = pSet->m_pMagic[j];
				pItem->m_mapTMAGIC.insert(MAPTMAGIC::value_type(pMagic->m_pMagic->m_bMagic, pMagic));
			}
		}
	}

	vItemSet.clear();
}
void CTMonster::SetMagicOpt(CTItem * pItem, BYTE bOptType)
{
	if(_AtlModule.m_vItemMagic[pItem->m_pTITEM->m_bKind].empty())
		return;
    
	MAPTMAGIC::iterator it;
	VTITEMMAGIC vItemMagic;
	vItemMagic.clear();
	
	for(BYTE i=0; i<_AtlModule.m_vItemMagic[pItem->m_pTITEM->m_bKind].size(); i++)
	{
		it = pItem->m_mapTMAGIC.find(_AtlModule.m_vItemMagic[pItem->m_pTITEM->m_bKind][i]->m_bMagic);
		if(it==pItem->m_mapTMAGIC.end())
		{
			if(_AtlModule.m_vItemMagic[pItem->m_pTITEM->m_bKind][i]->m_bMinLevel > m_bLevel)
				continue;
			else if( bOptType==MIP_MAGIC && !_AtlModule.m_vItemMagic[pItem->m_pTITEM->m_bKind][i]->m_bIsMagic)
				continue;
			else if( bOptType==MIP_RARE && !_AtlModule.m_vItemMagic[pItem->m_pTITEM->m_bKind][i]->m_bIsRare)
				continue;
				
			vItemMagic.push_back(_AtlModule.m_vItemMagic[pItem->m_pTITEM->m_bKind][i]);
		}
	}
	if(!vItemMagic.size())
		return;

	BYTE sel = 0;
	BYTE bSelected = 0;

	while(vItemMagic.size())
	{
		sel = BYTE(rand() % vItemMagic.size());
		bSelected = 1;

		if(vItemMagic[sel]->m_bExclIndex)
		{
			for(it=pItem->m_mapTMAGIC.begin(); it!=pItem->m_mapTMAGIC.end(); it++)
			{
				if((*it).second->m_pMagic->m_bExclIndex==vItemMagic[sel]->m_bExclIndex)
				{
					vItemMagic.erase(vItemMagic.begin() + sel);
					bSelected = 0;
					break;
				}
			}
		}

		if(bSelected)
			break;
	}

	if(!bSelected)
	{
		vItemMagic.clear();
		return;
	}

	// m_wValue 최종옵션비율
	LPTMAGIC pTMAGIC = new TMAGIC();
	pTMAGIC->m_pMagic = vItemMagic[sel];

	if((IsInPartyZone() && rand() % 2) || !(rand() % 3))
		pTMAGIC->m_wValue = MakeItemMagicValue(min(ITEMAGIC_BASELEVEL, m_bLevel), pTMAGIC->m_pMagic->m_wRareBound);
	else
		pTMAGIC->m_wValue = MakeItemMagicValue(m_bLevel, pTMAGIC->m_pMagic->m_wMaxBound);

	if(pTMAGIC->m_wValue)
		pItem->m_mapTMAGIC.insert(MAPTMAGIC::value_type(pTMAGIC->m_pMagic->m_bMagic, pTMAGIC));
	else
		delete pTMAGIC;

	vItemMagic.clear();
}

BYTE CTMonster::Lottery(CTItem * pItem)
{
	if(pItem->m_mapRoutingJoiner.empty())
	{
		pItem->m_mapRoutingJoiner.clear();
		pItem->m_bMaxRouting = 0;
		return FALSE;
	}

	BYTE bOwner = BYTE(rand() % pItem->m_mapRoutingJoiner.size());
	MAPDWORD::iterator it = pItem->m_mapRoutingJoiner.begin();
	int i=0;
	while(i++ < bOwner) it++;

	CTPlayer * pPlayer = _AtlModule.FindPlayer((*it).first, (*it).second);

	pItem->m_mapRoutingJoiner.clear();
	pItem->m_bMaxRouting = 0;

	if(!pPlayer)
		return FALSE;

	if(pPlayer->m_bMain)
	{
		BYTE bEraseItemID = pItem->m_bItemID;
		WORD wItemID = pItem->m_wItemID;
		CTItem logItem;
		logItem.Copy(pItem, FALSE);

		VTITEM vITEM;
		vITEM.clear();

		vITEM.push_back(pItem);

		if(pPlayer->CanPush(&vITEM, 0))
		{
#ifdef	DEF_UDPLOG
			_AtlModule.m_pUdpSocket->LogItemByMonster(LOGMAP_ITEMBYMONPARTY, pPlayer, this, &logItem);
#endif //	DEF_UDPLOG

			pPlayer->PushTItem(&vITEM);
			pPlayer->SendCS_MONITEMTAKE_ACK(MIT_SUCCESS);
			pPlayer->SendCS_GETITEM_ACK(&logItem);

			EraseItem(bEraseItemID);

			if(m_dwInvenLock)
				_AtlModule.ReviewMonItemList(m_dwInvenLock, this);

			_AtlModule.PartyMonItemTake(pPlayer, &logItem);

			vITEM.clear();

			return TRUE;
		}
		else
		{
			pPlayer->SendCS_MONITEMTAKE_ACK(MIT_FULLINVEN);
			if(m_dwInvenLock)
				_AtlModule.ReviewMonItemList(m_dwInvenLock, this);
				
			return FALSE;
		}
	}
	else
	{
		m_dwInvenLock = pPlayer->m_dwID;
		_AtlModule.SendMW_ADDITEM_ACK(
			(*it).first,
			(*it).second,
			_AtlModule.GetSvrID(),
			m_bChannel,
			m_pMAP->m_wMapID,
			m_dwID,
			0,INVALID_SLOT,
			pItem);

		EraseItem(pItem->m_bItemID);
		AddItemList(pItem);

		return TRUE;
	}
}
DWORD CTMonster::GetExp(DWORD dwDamage)
{
	DWORD dwExp = 0;

	if(_AtlModule.GetNation() == NATION_KOREA)
		dwExp = m_pMON->m_wExp;
	else
	{
		BYTE bRate = GetMonExpRate();
		dwExp = DWORD(m_pMON->m_wExp) * bRate / 100;
		if(IsInMissionMap())
			dwExp = dwExp * 120 / 100;
	}
	 
	dwExp += dwExp * _AtlModule.m_wEventValue[EVENT_EXPADD] / 100;

	return dwExp;
}
BYTE CTMonster::CanUseSkillNOW( CTSkill *pTSKILL)
{
	return pTSKILL &&
		pTSKILL->CanUse(_AtlModule.m_dwTick + m_dwAtkWaitTick) &&
		pTSKILL->GetRequiredHP(GetPureMaxHP()) <= m_dwHP &&
		pTSKILL->GetRequiredMP(GetPureMaxMP()) <= m_dwMP &&
		CheckPrevAct(pTSKILL->m_pTSKILL->m_wPrevActiveID) ? TRUE : FALSE;
}
WORD CTMonster::SelectSkill(CTPlayer * pHost, BYTE bNew)
{
	if(!pHost)
		return 0;

	WORD wPrevSkill = m_pNextSkill ? m_pNextSkill->m_pTSKILL->m_wID : 0;
	FLOAT fDist = GetTargetDistance();

	MAPTSKILL::iterator it;
	CTSkill *p2ND = NULL;
	m_pNextSkill = NULL;

	for( it = m_mapTSKILL.begin(); it != m_mapTSKILL.end(); it++)
	{
		CTSkill *pTSKILL = (*it).second;

		if( pTSKILL && CheckPrevAct(pTSKILL->m_pTSKILL->m_wPrevActiveID) )
		{
			if( fDist >= pTSKILL->m_pTSKILL->m_wMinRange &&
				fDist <= pTSKILL->m_pTSKILL->m_wMaxRange )
			{
				if(!m_pNextSkill)
					m_pNextSkill = pTSKILL;
				else if(CanUseSkillNOW(m_pNextSkill))
				{
					if( CanUseSkillNOW(pTSKILL) &&
						m_pNextSkill->m_pTSKILL->m_dwReuseDelay < pTSKILL->m_pTSKILL->m_dwReuseDelay )
						m_pNextSkill = pTSKILL;
				}
				else if(CanUseSkillNOW(pTSKILL))
					m_pNextSkill = pTSKILL;
				else if( m_pNextSkill->m_pTSKILL->m_wMinRange < pTSKILL->m_pTSKILL->m_wMinRange )
					m_pNextSkill = pTSKILL;
				else if( pTSKILL->m_pTSKILL->m_wMinRange == m_pNextSkill->m_pTSKILL->m_wMinRange &&
					pTSKILL->m_pTSKILL->m_dwReuseDelay < m_pNextSkill->m_pTSKILL->m_dwReuseDelay )
					m_pNextSkill = pTSKILL;
			}
			else if( pTSKILL->m_pTSKILL->m_wMinRange < fDist )
				if(!p2ND)
					p2ND = pTSKILL;
				else if(CanUseSkillNOW(p2ND))
				{
					if( CanUseSkillNOW(pTSKILL) && (
						p2ND->m_pTSKILL->m_dwReuseDelay < pTSKILL->m_pTSKILL->m_dwReuseDelay || (
						p2ND->m_pTSKILL->m_dwReuseDelay == pTSKILL->m_pTSKILL->m_dwReuseDelay &&
						p2ND->m_pTSKILL->m_wMaxRange < pTSKILL->m_pTSKILL->m_wMaxRange )))
						p2ND = pTSKILL;
				}
				else if(CanUseSkillNOW(pTSKILL))
					p2ND = pTSKILL;
				else if( p2ND->m_pTSKILL->m_wMinRange < pTSKILL->m_pTSKILL->m_wMinRange )
					p2ND = pTSKILL;
				else if( pTSKILL->m_pTSKILL->m_wMinRange == p2ND->m_pTSKILL->m_wMinRange &&
					pTSKILL->m_pTSKILL->m_dwReuseDelay < p2ND->m_pTSKILL->m_dwReuseDelay )
					p2ND = pTSKILL;
		}
	}

	if( m_pNextSkill && p2ND &&
		!CanUseSkillNOW(m_pNextSkill) &&
		CanUseSkillNOW(p2ND) &&
		m_pNextSkill->m_pTSKILL->m_wMinRange <= p2ND->m_pTSKILL->m_wMinRange )
		m_pNextSkill = p2ND;

	if(!m_pNextSkill)
		m_pNextSkill = p2ND;

	if(!m_pNextSkill)
		return 0;

	if( bNew || m_pNextSkill->m_pTSKILL->m_wID != wPrevSkill )
		pHost->SendCS_MONSETSKILL_ACK(
			m_dwID,
			m_pNextSkill->m_pTSKILL->m_wID,
			m_pNextSkill->m_pTSKILL->m_wMaxRange);

	return m_pNextSkill->m_pTSKILL->m_wID;
}
FLOAT CTMonster::GetTargetDistance(CTObjBase * pTarget)
{
	FLOAT fDis = FLOAT(0xFFFF);
	if(pTarget)
	{
		FLOAT fDistX = pTarget->m_fPosX - m_fPosX;
		FLOAT fDistZ = pTarget->m_fPosZ - m_fPosZ;
		FLOAT fSize = m_fSize + pTarget->m_fSize;
		fDis = sqrt(fDistX * fDistX + fDistZ * fDistZ);
		fDis = fDis > fSize ? fDis - fSize : 0;
	}

	return fDis;
}

FLOAT CTMonster::GetTargetDistance()
{
	FLOAT fDis = FLOAT(0xFFFF);

	CTPlayer * pHost = FindHost(m_dwHostID);
	CTObjBase * pTarget;

	if(!pHost)
		return fDis;

	switch(m_bTargetType)
	{
	case OT_PC:
		pTarget = pHost;
		break;
	case OT_RECALL:
		pTarget = pHost->FindRecallMon(m_dwTargetID);
		break;
	case OT_SELF:
		pTarget = pHost->FindSelfObj(m_dwTargetID);
		break;
	default:
		return fDis;
	}

	return GetTargetDistance(pTarget);
}

void CTMonster::ResetSize(WORD wMonTempID)
{
	if(!m_pMON)
		return;

	if(wMonTempID)
	{
		LPTMONSTER pMon = _AtlModule.FindTMonster(wMonTempID);
		if(pMon)
			m_fSize = pMon->m_fSize/2;
	}
	else
		m_fSize = m_pMON->m_fSize / 2;
}

DWORD CTMonster::GetMaxAP(BYTE bArrow)
{
	DWORD dwAP;
	if(bArrow)
	{
		dwAP = m_pATTR->m_wLAP;
		if(!HaveDisWeapon())
			dwAP += m_pATTR->m_wMaxWAP;

		CalcAbilityValue(dwAP, MTYPE_LAP);
	}
	else
	{
        dwAP = m_pATTR->m_wAP;
		if(!HaveDisWeapon())
			dwAP += m_pATTR->m_wMaxWAP;

		CalcAbilityValue(dwAP, MTYPE_PAP);
	}
	return dwAP;
}
DWORD CTMonster::GetMinAP(BYTE bArrow)
{
	DWORD dwAP;
	if(bArrow)
	{
		dwAP = m_pATTR->m_wLAP;
		if(!HaveDisWeapon())
			dwAP += m_pATTR->m_wMinWAP;

		CalcAbilityValue(dwAP, MTYPE_LAP);
	}
	else
	{
        dwAP = m_pATTR->m_wAP;
		if(!HaveDisWeapon())
			dwAP += m_pATTR->m_wMinWAP;

		CalcAbilityValue(dwAP, MTYPE_PAP);
	}
	return dwAP;
}
DWORD CTMonster::GetDefendPower()
{
	DWORD dwDP = m_pATTR->m_wDP;
	if(!HaveDisDefend())
		dwDP += m_pATTR->m_wWDP;

	CalcAbilityValue(dwDP, MTYPE_PDP);
	return dwDP;
}
WORD CTMonster::GetAttackLevel()
{
	DWORD dwAL = m_pATTR->m_wAL;
	CalcAbilityValue(dwAL, MTYPE_AL);
	return WORD(dwAL);
}
WORD CTMonster::GetDefendLevel()
{
	DWORD dwDL = m_pATTR->m_wDL;
	CalcAbilityValue(dwDL, MTYPE_DL);
	return WORD(dwDL);
}
WORD CTMonster::GetMagicAtkLevel()
{
	DWORD dwAL = m_pATTR->m_wMAL;
	CalcAbilityValue(dwAL, MTYPE_MAL);
	return WORD(dwAL);
}
WORD CTMonster::GetMagicDefLevel()
{
	DWORD dwDL = m_pATTR->m_wMDL;
	CalcAbilityValue(dwDL, MTYPE_MDL);
	return WORD(dwDL);
}
BYTE CTMonster::GetCriticalPysProb()
{
	DWORD dwCR = m_pATTR->m_bCriticalPP;
	CalcAbilityValue(dwCR, MTYPE_CR);
	return BYTE(dwCR);
}
BYTE CTMonster::GetCriticalMagicProb()
{
	DWORD dwCR = m_pATTR->m_bCriticalMP;
	CalcAbilityValue(dwCR, MTYPE_MCR);
	return BYTE(dwCR);
}
BYTE CTMonster::GetCritical(CTSkill * pSkill)
{
	BYTE bAttackType = pSkill->m_pTSKILL->GetAttackType();
	BYTE bCR;
	if(!m_pMON || !m_pMON->m_wID)
		bCR = 0xFF;
	else if(bAttackType == SAT_MAGIC)
		bCR = GetCriticalMagicProb();
	else
		bCR = GetCriticalPysProb();

	return bCR;
}
DWORD CTMonster::GetMaxHP()
{
	DWORD dwMaxHP = m_pATTR->m_dwMaxHP;
	CalcAbilityValue(dwMaxHP, MTYPE_MHP);
	if(dwMaxHP == 0) dwMaxHP = 1;

	return dwMaxHP;
}
DWORD CTMonster::GetMaxMP()
{
	DWORD dwMaxMP = m_pATTR->m_dwMaxMP;
	CalcAbilityValue(dwMaxMP, MTYPE_MMP);
	if(dwMaxMP == 0) dwMaxMP = 1;

	return dwMaxMP;
}
DWORD CTMonster::GetMinMagicAP()
{
	DWORD dwAP = m_pATTR->m_wMAP;
	if(!HaveDisWeapon())
		dwAP += m_pATTR->m_wMinWAP;

	CalcAbilityValue(dwAP, MTYPE_MAP);

	return dwAP;
}
DWORD CTMonster::GetMaxMagicAP()
{
	DWORD dwAP = m_pATTR->m_wMAP;
	if(!HaveDisWeapon())
		dwAP += m_pATTR->m_wMaxWAP;

	CalcAbilityValue(dwAP, MTYPE_MAP);

	return dwAP;
}
DWORD CTMonster::GetMagicDefPower()
{
	DWORD dwMDP = m_pATTR->m_wMDP;
	if(!HaveDisDefend())
		dwMDP += m_pATTR->m_wWDP;

	CalcAbilityValue(dwMDP, MTYPE_MDP);
	return dwMDP;
}
BYTE CTMonster::UseSkillItem(CTSkill * pSkill, BYTE bNeedAddtional)
{
	return TRUE;
}

BYTE CTMonster::MoveNext(FLOAT &fPosX, FLOAT &fPosY, FLOAT &fPosZ, BYTE &bAction, CTPlayer *pHOST)
{
	BYTE bGo = FALSE;

	if(m_pSPAWN->m_dwLeader &&
		m_pSPAWN->m_dwLeader != m_dwID )
	{
		CTMonster * pLeader = m_pMAP->FindMonster(m_pSPAWN->m_dwLeader);
		if(pLeader &&
			pLeader->m_bStatus == OS_WAKEUP &&
			m_bMode == MT_NORMAL &&
			GetDistance(m_fPosX, m_fPosZ, pLeader->m_fPosX, pLeader->m_fPosZ) < MAX_ROAMRANGE)
		{
			FLOAT fRad = FLOAT(rand() % 360) * FLOAT(M_PI) / 180.0f;
			fPosX = pLeader->m_fNextX + 3 * cosf(fRad);
			fPosY = pLeader->m_fNextY;
			fPosZ = pLeader->m_fNextZ + 3 * sinf(fRad);

			m_fStartX = m_fPosX;
			m_fStartY = m_fPosY;
			m_fStartZ = m_fPosZ;
			m_bCurPathID = pLeader->m_bCurPathID;
			m_bCurPathDir = pLeader->m_bCurPathDir;
			
			if(GetDistance(m_fPosX, m_fPosZ, pLeader->m_fPosX, pLeader->m_fPosZ) < MAX_WALKBOUND)
				bAction = TA_WALK;
			else
				bAction = TA_RUN;
			
			return TRUE;
		}
	}

	if(m_bMode != MT_GOHOME)
	{
		if( fabs(m_fPosY - pHOST->m_fPosY) > MAX_ROAMHEIGHT ||
			GetDistance( m_fPosX, m_fPosZ, pHOST->m_fPosX, pHOST->m_fPosZ) > MAX_ROAMBOUND )
			bAction = TA_STAND;
		else
			switch(m_pSPAWN->m_pSPAWN->m_bRoamType)
			{
			case 0:
				if( DWORD(rand() % 100) < m_pMON->m_bRoamProb )
				{
					switch(m_bAction)
					{
					case TA_STAND	: bAction = rand() % 2 ? TA_WALK : TA_RUN; break;
					case TA_RUN		: bAction = rand() % 2 ? TA_STAND : TA_WALK; break;
					case TA_WALK	:
						{
							bAction = m_bAction;
							if(rand() % 2)
								bAction = rand() % 2 ? TA_STAND : TA_RUN;
						}
						break;
					}
				}
				break;
			case 1:
				bAction = TA_WALK;
				break;
			case 2:
				bAction = TA_RUN;
				break;
			case 3:
				if(abs(m_fPosX - m_fStartX) > 1 || abs(m_fPosZ - m_fStartZ) > 1)
				{
					bAction = TA_WALK;
					bGo = TRUE;
				}
				else
					bAction = TA_STAND;
				break;
			}
	}
	else
		bAction = TA_RUN;

	if(m_bAction == TA_STAND
		&& bAction == TA_STAND)
		return FALSE;

	if(!m_pSPAWN->m_pSPAWN->m_vPathPoint.empty())
	{
		bGo = TRUE;

		if(GetDistance(m_pSPAWN->m_pSPAWN->m_vPathPoint[m_bCurPathID]->m_fPosX, m_pSPAWN->m_pSPAWN->m_vPathPoint[m_bCurPathID]->m_fPosZ, m_fPosX, m_fPosZ) <= ZONE_HOMESIZE)
		{
			if( DWORD(rand() % 100) > m_pSPAWN->m_pSPAWN->m_vPathPoint[m_bCurPathID]->m_bProb )
			{
				if(m_bCurPathDir)
				{
					if(m_bCurPathID == m_pSPAWN->m_pSPAWN->m_vPathPoint.size()-1)
					{
						m_bCurPathID--;
						m_bCurPathDir=0;
					}
					else
						m_bCurPathID++;
				}
				else
				{
					if(m_bCurPathID == 0)
					{
						m_bCurPathID++;
						m_bCurPathDir=1;
					}
					else
						m_bCurPathID--;
				}
			}
			else
				bGo = FALSE;
		}

		if(bGo && m_bMode == MT_NORMAL)
		{
			m_fStartX = m_fPosX;
			m_fStartY = m_fPosY;
			m_fStartZ = m_fPosZ;
		}
	}
	else
	{
		if( GetDistance(m_fStartX, m_fStartZ, m_fPosX, m_fPosZ) > m_pSPAWN->m_pSPAWN->m_bArea )
			bGo = TRUE;
	}

	if(bGo)
	{
		if(m_bMode == MT_NORMAL &&
			!m_pSPAWN->m_pSPAWN->m_vPathPoint.empty())
		{
			FLOAT fRad = FLOAT(rand() % 360) * FLOAT(M_PI) / 180.0f;
			fPosX = m_pSPAWN->m_pSPAWN->m_vPathPoint[m_bCurPathID]->m_fPosX + m_pSPAWN->m_pSPAWN->m_vPathPoint[m_bCurPathID]->m_fRadius * cosf(fRad);
			fPosY = m_pSPAWN->m_pSPAWN->m_vPathPoint[m_bCurPathID]->m_fPosY;
			fPosZ = m_pSPAWN->m_pSPAWN->m_vPathPoint[m_bCurPathID]->m_fPosZ + m_pSPAWN->m_pSPAWN->m_vPathPoint[m_bCurPathID]->m_fRadius * sinf(fRad);
		}
		else
		{
			fPosX = m_fStartX;
			fPosY = m_fStartY;
			fPosZ = m_fStartZ;
		}
	}
	else
	{
		FLOAT fRad = FLOAT(rand() % 360) * FLOAT(M_PI) / 180.0f;

		fPosX = m_fStartX + m_pSPAWN->m_pSPAWN->m_bArea * cosf(fRad);
		fPosY = m_fStartY;
		fPosZ = m_fStartZ + m_pSPAWN->m_pSPAWN->m_bArea * sinf(fRad);
	}

	return TRUE;
}

BYTE CTMonster::IsOutofRoamBound()
{
	if(m_bMode == MT_NORMAL)
	{
		if( GetDistance(m_fStartX, m_fStartZ, m_fPosX, m_fPosZ) > m_pSPAWN->m_pSPAWN->m_bArea )
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}

void CTMonster::Recover(DWORD dwTick)
{
	if(!CheckAttack() || m_bStatus == OS_DEAD)
		return;

	if(HaveStopRecover())
		return;

	BYTE bKeepChange = FALSE;
	BYTE bChange = FALSE;

	if(	m_bMode != MT_BATTLE &&
		dwTick >= m_dwRecoverHPTick + RECOVER_TIME )
	{
		DWORD dwMaxHP = GetMaxHP();

		if(m_dwHP < dwMaxHP)
		{
			bChange = TRUE;
			DWORD dwPrevHP = m_dwHP;
			DWORD dwInc = dwMaxHP / 4;
			CalcAbilityValue(dwInc, MTYPE_HPR);
			m_dwHP += dwInc;
			if(m_dwHP > dwMaxHP)
				m_dwHP = dwMaxHP;

			m_dwRecoverHPTick = dwTick;

			MAPINT64::iterator itDg;
			float fDiv = float(dwMaxHP - dwPrevHP);
			if(fDiv == 0) fDiv = 1;
			float fRatio = (m_dwHP - dwPrevHP) / fDiv;
			for(itDg=m_mapDamage.begin(); itDg!=m_mapDamage.end();)
			{
				DWORD dwDamage = DWORD((*itDg).second);
				(*itDg).second = DWORD(dwDamage - dwDamage*fRatio);
				if(!DWORD((*itDg).second))
					itDg = m_mapDamage.erase(itDg);
				else
					itDg++;
			}

			// 길드데미지
			MAPDWORD::iterator itGu;
			for(itGu = m_mapGuildDamage.begin(); itGu != m_mapGuildDamage.end(); )
			{
				DWORD dwGuildDamage = (*itGu).second;
				(*itGu).second = DWORD(dwGuildDamage - dwGuildDamage * fRatio);
				if( !(*itGu).second )
					itGu = m_mapGuildDamage.erase(itGu);
				else
					itGu++;
			}

			if(m_dwKeeperID &&
				m_dwHP >= dwMaxHP - dwMaxHP*MONKEEP_PER/100)
			{
				SetKeeper(0,0);
				bKeepChange = TRUE;
			}
		}
	}

	if(dwTick >= m_dwRecoverMPTick + RECOVER_TIME )
	{
		DWORD dwMaxMP = GetMaxMP();

		if(m_dwMP < dwMaxMP)
		{
			bChange = TRUE;
			DWORD dwInc = dwMaxMP/4;
			CalcAbilityValue(dwInc, MTYPE_MPR);
			m_dwMP += dwInc;
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
				pChar->SendCS_CHANGECOLOR_ACK(
					m_dwID,
					OT_MON,
					GetColor( pChar->m_dwID, pChar->GetPartyID(), pChar->GetWarCountry()),
					m_bCountry);
			}

			vPLAYERS.pop_back();
		}

		vPLAYERS.clear();
	}
}

BYTE CTMonster::OnBattleZoneEvent(DWORD dwAttackID)
{
	MAPTBATTLEZONE::iterator it = _AtlModule.m_mapTBATTLEZONE.find(m_pSPAWN->m_pSPAWN->m_wLocalID);
	if(it == _AtlModule.m_mapTBATTLEZONE.end())
		return FALSE;

	LPTLOCAL pLocal = _AtlModule.FindOccupationZone(m_pSPAWN->m_pSPAWN->m_wLocalID);
	LPTLOCAL pCastle = _AtlModule.FindCastleZone(m_pSPAWN->m_pSPAWN->m_wLocalID);
	LPTLOCAL pMission = _AtlModule.FindMissionZone(m_pSPAWN->m_pSPAWN->m_wLocalID);
	if(!pLocal && !pCastle && !pMission)
		return FALSE;

	DWORD dwSwitchID = 0;
	BYTE bEvent = BS_NORMAL;
	LPTBATTLEZONE pZone = (*it).second;
	if(pZone->m_wLGateKeeperSpawnID == m_pSPAWN->m_pSPAWN->m_wID)
	{
		dwSwitchID = pZone->m_dwLSwitchID;
		bEvent = BS_OPENLGATE;
	}
	else if(pZone->m_wRGateKeeperSpawnID == m_pSPAWN->m_pSPAWN->m_wID)
	{
		dwSwitchID = pZone->m_dwRSwitchID;
		bEvent = BS_OPENRGATE;
	}
	else if(pZone->m_wCGateKeeperSpawnID == m_pSPAWN->m_pSPAWN->m_wID)
	{
		dwSwitchID = pZone->m_dwCSwitchID;
		bEvent = BS_OPENRGATE;
	}

	if(pMission && bEvent)
	{
		if(pMission->m_bOccupied)
			return FALSE;

		pMission->m_bOccupied = TRUE;
		pMission->m_bStatus = BS_PEACE;
		BYTE bCountry = (m_bCountry == TCONTRY_D) ? TCONTRY_C : TCONTRY_D;

		_AtlModule.SendDM_MISSIONOCCUPY_REQ(
			pMission->m_wLocal,
			OCCUPY_ACCEPT,
			bCountry,
			dwAttackID);
	}

	if(dwSwitchID)
	{
		LPTSWITCH pSwitch = m_pMAP->FindSwitch(dwSwitchID);
		if( pSwitch && _AtlModule.ChangeSwitch( m_pMAP, dwSwitchID, SWC_OPEN) )
		{
			if(pLocal)
			{
				if(!pLocal->m_bGateOpened)
					_AtlModule.AddMonSpawn(m_pMAP, pLocal->m_pZone->m_wBossSpawnID, MONSPAWN_SUSPEND);

				pLocal->m_bGateOpened = TRUE;
				_AtlModule.NotifyLocalInfo(SM_BATTLE_OPENGATE, pLocal->m_wLocal, 0, pLocal->m_pZone->m_strName);
			}
		}
	}

	if(pLocal && pLocal->m_pZone->m_wBossSpawnID == m_pSPAWN->m_pSPAWN->m_wID)
	{
		DWORD dwGuildID = 0;
		DWORD dwMaxDamage = 0;
		DWORD dwGuildDamage = 0;
		MAPDWORD::iterator itG;
		for(itG = m_mapGuildDamage.begin(); itG != m_mapGuildDamage.end(); itG++)
		{
			dwGuildDamage = (*itG).second;
			if(dwMaxDamage < dwGuildDamage)
			{
				dwMaxDamage = dwGuildDamage;
				dwGuildID = (*itG).first;
			}
		}

		pLocal->m_bOccupied = TRUE;

		_AtlModule.SendDM_LOCALOCCUPY_REQ(
			pLocal->m_wLocal,
			OCCUPY_ACCEPT,
			dwGuildID,
			dwAttackID);

		_AtlModule.PvPEvent(PVPE_GODMONKILL, NULL, dwAttackID);
	}

	return TRUE;
}

BYTE CTMonster::IsTransformation()
{
	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		if(m_pMON->m_wTransSkillID == m_vMaintainSkill[i]->m_pTSKILL->m_wID)
			return TRUE;
	}

	return FALSE;
}

BYTE CTMonster::Transformation()
{
	if(!m_pMON->m_wTransSkillID)
		return FALSE;

	CTSkillTemp * pTemp = _AtlModule.FindTSkill(m_pMON->m_wTransSkillID);
	if(!pTemp || IsTransformation())
		return FALSE;
	else
	{
		CTSkill * pSkill = new CTSkill();
		pSkill->m_bLevel = 1;
		pSkill->m_pTSKILL = pTemp;

		CTSkill * pMaintain = MaintainSkill(
			pSkill,
			0,
			m_dwHostID,
			OT_PC,
			m_dwID,
			m_bType,
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

			for(DWORD ti=0; ti<pTemp->m_vData.size(); ti++)
			{
				if(pTemp->m_vData[ti]->m_bType == SDT_TRANS)
                    ResetSize(pTemp->m_vData[ti]->m_wValue);
			}

			m_dwHP = GetMaxHP();
			m_dwMP = GetMaxMP();

			VPLAYER vPlayers;
			vPlayers.clear();

			m_pMAP->GetNeighbor(
				&vPlayers,
				m_fPosX,
				m_fPosZ);

			while(!vPlayers.empty())
			{
				CTPlayer * pChar = vPlayers.back();

				pChar->SendCS_HPMP_ACK(
					m_dwID,
					m_bType,
					m_bLevel,
					m_dwHP,
					m_dwHP,
					m_dwMP,
					m_dwMP);

				pChar->SendCS_DEFEND_ACK(
					m_dwID,
					m_dwID,
					m_bType,
					m_bType,
					0, 0,
					0, 0,
					TRUE,
					0, 0,
					HT_NORMAL,
					1,
					m_bLevel,
					0,0,0,0,
					TRUE,
					FALSE,
					m_bCountry,
					m_bAidCountry,
					m_pMON->m_wTransSkillID,
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

void CTMonster::Blind()
{
	OnEvent( AT_HELP, 0, 0, 0, 0);
//	OnEvent( AT_LEAVELB, 0, m_dwHostID, m_dwTargetID, m_bTargetType);
//	OnEvent(AT_ATHOME, 0, m_dwHostID, 0, 0);
}

BYTE CTMonster::CanSilenceAttack(WORD wSkillID)
{
	CTSkillTemp * pTemp = _AtlModule.FindTSkill(wSkillID);
	if(!pTemp)
		return FALSE;

	for(DWORD i=0; i<m_vMaintainSkill.size(); i++)
	{
		CTSkill * pSkill = m_vMaintainSkill[i];
		for(DWORD j=0; j<pSkill->m_pTSKILL->m_vData.size(); j++)
		{
			LPTSKILLDATA pData = pSkill->m_pTSKILL->m_vData[j];
			if(pData->m_bType == SDT_STATUS &&
				pData->m_bExec == SDT_STATUS_SILENCE &&
				pTemp->m_bKind != MONSTER_BASE_SKILL_KIND)				
				return FALSE;
		}
	}

	return TRUE;
}

void CTMonster::NotifyHost(CTPlayer * pPrevHost)
{
	VPLAYER vPlayers;
	vPlayers.clear();

	m_pMAP->GetNeighbor(
		&vPlayers,
		m_fPosX,
		m_fPosZ);

	if(pPrevHost)
		pPrevHost->SendCS_MONHOST_ACK(m_dwID, FALSE);

	while(!vPlayers.empty())
	{
		CTPlayer * pChar = vPlayers.back();
		if(!pPrevHost || (pPrevHost && pPrevHost->m_dwID != pChar->m_dwID))
			pChar->SendCS_MONHOST_ACK( m_dwID, m_dwHostID == pChar->m_dwID ? TRUE : FALSE);

		vPlayers.pop_back();
	}
}

void CTMonster::ResetHost()
{
	CTPlayer * pHOST = _AtlModule.FindChar(m_dwHostID);

	m_bAction = TA_STAND;
	ChgMode(MT_NORMAL);

	m_dwHostID = 0;
	m_dwTargetID = 0;
	m_bTargetType = 0;
	m_dwHelper = 0;
	m_bHelpCount = 0;

	m_mapAggro.clear();
	m_mapDamage.clear();
	m_mapGuildDamage.clear(); 

	if(pHOST)
	{
		NotifyHost(pHOST);

		if(m_pMAP)
		{
			if(GetDistance(m_fStartX, m_fStartZ, m_fPosX, m_fPosZ) > CELL_SIZE/2)
				_AtlModule.SendSM_RESETHOST_REQ(
					m_pMAP->m_bChannel,
					m_pMAP->m_wMapID,
					m_pMAP->m_wPartyID,
					m_dwID);
		}
	}
}

BYTE CTMonster::IsInMissionMap()
{
	if(!m_pMAP) return FALSE;

	if(m_pMAP->m_wMapID >= 700 && m_pMAP->m_wMapID < 800)
		return TRUE;

	return FALSE;
}

BYTE CTMonster::IsInPartyZone()
{
	if(!m_pMAP) return FALSE;

	if(m_pMAP->m_wMapID >= 900 && m_pMAP->m_wMapID < 1000)
		return TRUE;

	return FALSE;
}

