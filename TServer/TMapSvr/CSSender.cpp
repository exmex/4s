#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

void CTPlayer::SendCT_SERVICEMONITOR_REQ( DWORD dwTick,
										  DWORD dwSession,
										  DWORD dwUser,
										  DWORD dwActiveUser)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CT_SERVICEMONITOR_REQ)
		<< dwTick
		<< dwSession
		<< dwUser
		<< dwActiveUser;

	Say(pMSG);
}

void CTPlayer::SendCT_MONSPAWNFIND_ACK(DWORD dwManager,
									   WORD wMapID,
									   WORD wSpawnID,
									   CTMonSpawn * pSPAWN)
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(CT_MONSPAWNFIND_ACK)
		<< dwManager
		<< wMapID
		<< wSpawnID
		<< (BYTE)pSPAWN->m_vTMON.size();

	for(DWORD i=0; i<pSPAWN->m_vTMON.size(); i++)
	{
		(*pMSG)
			<< pSPAWN->m_vTMON[i]->m_dwID
			<< pSPAWN->m_vTMON[i]->m_dwHostID
			<< pSPAWN->m_vTMON[i]->m_bStatus
			<< pSPAWN->m_vTMON[i]->m_fPosX
			<< pSPAWN->m_vTMON[i]->m_fPosY
			<< pSPAWN->m_vTMON[i]->m_fPosZ;
	}

	Say(pMSG);
}

void CTPlayer::SendCT_CASTLEINFO_ACK(DWORD dwManagerID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CT_CASTLEINFO_ACK)
		<< dwManagerID
		<< WORD(_AtlModule.m_vCastle.size());

	for(DWORD ll=0; ll<_AtlModule.m_vCastle.size(); ll++)
	{
		LPTLOCAL pLocal = _AtlModule.m_vCastle[ll];

		(*pMSG)
			<< pLocal->m_wLocal
			<< pLocal->m_pZone->m_strName
			<< pLocal->m_bStatus
			<< pLocal->m_dwDefGuildID
			<< pLocal->m_strDefName
			<< pLocal->m_bCountry						
			<< pLocal->m_dwAtkGuildID
			<< pLocal->m_strAtkName			
			<< pLocal->m_timeNextDefend;
	}

	Say(pMSG);
}

void CTPlayer::SendCS_CONNECT_ACK( BYTE bResult,
								  LPVBYTE vServerID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_CONNECT_ACK)
		<< bResult;

	if(vServerID)
	{
		(*pMSG)
			<< BYTE(vServerID->size());

		for(DWORD i=0; i<vServerID->size(); i++)
		{
			(*pMSG)
				<< (*vServerID)[i];
		}
	}
	else
		(*pMSG)
			<< BYTE(0);

	Say(pMSG);
}

void CTPlayer::SendCS_SHUTDOWN_ACK()
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(CS_SHUTDOWN_ACK);

	Say(pMSG);
}

void CTPlayer::SendCS_INVALIDCHAR_ACK()
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_INVALIDCHAR_ACK);
	Say(pMSG);
}

void CTPlayer::SendCS_CHARINFO_ACK(DWORD dwTick,
								   DWORD dwPrevExp,
								   DWORD dwNextExp)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_CHARINFO_ACK)
		<< m_dwID
		<< m_strNAME
		<< m_bStartAct
		<< m_bClass
		<< m_bRace
		<< m_bCountry
		<< m_bAidCountry
		<< m_bSex
		<< m_bHair
		<< m_bFace
		<< m_bBody
		<< m_bPants
		<< m_bHand
		<< m_bFoot
		<< m_bHelmetHide
		<< m_bLevel
		<< GetPartyID()
		<< m_dwGuildID
		<< m_dwFame
		<< m_dwFameColor
		<< m_bGuildDuty
		<< m_bGuildPeer
		<< m_strGuildName
		<< m_dwTacticsID
		<< m_strTacticsName
		<< m_dwGold
		<< m_dwSilver
		<< m_dwCooper
		<< dwPrevExp
		<< dwNextExp
		<< m_dwEXP
		<< GetMaxHP()
		<< m_dwHP
		<< GetMaxMP()
		<< m_dwMP
		<< GetPartyChiefID()
		<< GetCommanderID()
		<< m_dwRegion
		<< m_wMapID
		<< m_fPosX
		<< m_fPosY
		<< m_fPosZ
		<< m_wDIR
		<< m_wSkillPoint
		<< m_bLuckyNumber
		<< GetAidLeftTime();

	WORD arPoint[4];
	GetSkillKindPoint(arPoint);

	(*pMSG)
		<< arPoint[0]
		<< arPoint[1]
		<< arPoint[2]
		<< arPoint[3]
		<< BYTE(m_mapTINVEN.size());

	MAPTINVEN::iterator itTINVEN;

	for( itTINVEN = m_mapTINVEN.begin(); itTINVEN != m_mapTINVEN.end(); itTINVEN++)
	{
		CTInven *pTINVEN = (*itTINVEN).second;
		MAPTITEM::iterator itTITEM;

		(*pMSG)
			<< pTINVEN->m_bInvenID
			<< pTINVEN->m_wItemID
			<< pTINVEN->m_dEndTime
			<< BYTE(pTINVEN->m_mapTITEM.size());

		for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
		{
			CTItem *pTITEM = (*itTITEM).second;
			pTITEM->WrapPacketClient(pMSG, m_dwID);
		}
	}

	(*pMSG)	<< (BYTE)m_mapTSKILL.size();
	MAPTSKILL::iterator it;
	for(it=m_mapTSKILL.begin(); it!=m_mapTSKILL.end(); it++)
	{
		CTSkill * pSkill = (*it).second;
		(*pMSG) << pSkill->m_pTSKILL->m_wID
			<< pSkill->m_bLevel
			<< pSkill->GetReuseRemainTick(dwTick);
	}

	(*pMSG) << (BYTE)m_vMaintainSkill.size();
	for(BYTE is = 0; is<(BYTE)m_vMaintainSkill.size(); is++)
	{
		(*pMSG)
			<< m_vMaintainSkill[is]->GetID()
			<< m_vMaintainSkill[is]->m_bLevel
			<< m_vMaintainSkill[is]->GetRemainTick(dwTick)
			<< m_vMaintainSkill[is]->m_dwAttackID
			<< m_vMaintainSkill[is]->m_bAttackType
			<< m_vMaintainSkill[is]->m_dwHostID
			<< m_vMaintainSkill[is]->m_bHostType
			<< m_vMaintainSkill[is]->m_bHit
			<< m_vMaintainSkill[is]->m_wAttackLevel
			<< m_vMaintainSkill[is]->m_bAttackerLevel
			<< m_vMaintainSkill[is]->m_dwPysMinPower
			<< m_vMaintainSkill[is]->m_dwPysMaxPower
			<< m_vMaintainSkill[is]->m_dwMgMinPower
			<< m_vMaintainSkill[is]->m_dwMgMaxPower
			<< m_vMaintainSkill[is]->m_bCanSelect
			<< m_vMaintainSkill[is]->m_bAttackCountry
			<< m_vMaintainSkill[is]->m_fPosX
			<< m_vMaintainSkill[is]->m_fPosY
			<< m_vMaintainSkill[is]->m_fPosZ;
	}

	(*pMSG) << BYTE(m_mapHotkeyInven.size());
	MAPTHOTKEYINVEN::iterator itHotkey;
	for(itHotkey=m_mapHotkeyInven.begin(); itHotkey!=m_mapHotkeyInven.end(); itHotkey++)
	{
		(*pMSG) << BYTE((*itHotkey).first);
		for(BYTE i=0; i<MAX_HOTKEY_POS; i++)
		{
			(*pMSG)
				<< (*itHotkey).second->m_hotkey[i].m_bType
				<< (*itHotkey).second->m_hotkey[i].m_wID;
		}
	}

	(*pMSG) << BYTE(m_mapItemCoolTime.size());
	MAPDWORD::iterator itIU;
	for(itIU=m_mapItemCoolTime.begin(); itIU!=m_mapItemCoolTime.end(); itIU++)
	{
		(*pMSG)
			<< WORD((*itIU).first)
			<< ((*itIU).second > _AtlModule.m_dwTick ? (*itIU).second-_AtlModule.m_dwTick : 0);
	}

	(*pMSG)
		<< m_dwPvPTotalPoint
		<< m_dwPvPUseablePoint
		<< m_dwMonthPvPoint; 

	Say(pMSG);
}

void CTPlayer::SendCS_ENTER_ACK( CTPlayer *pPlayer,
								 BYTE bNewMember,
								 DWORD dwTick)
{
	CPacket *pMSG = new CPacket();

	BYTE bColor = !pPlayer->CanFight(m_bType, m_bCountry, m_bAidCountry) ? TNCOLOR_ALLI : TNCOLOR_ENEMY;

	pMSG->SetID(CS_ENTER_ACK)
		<< pPlayer->m_dwID
		<< pPlayer->m_strNAME
		<< (bColor == TNCOLOR_ALLI ? pPlayer->m_strComment : NAME_NULL)
		<< pPlayer->m_dwGuildID
		<< pPlayer->m_dwFame
		<< pPlayer->m_dwFameColor
		<< pPlayer->m_strGuildName
		<< pPlayer->m_bGuildPeer
		<< pPlayer->m_dwTacticsID
		<< pPlayer->m_strTacticsName
		<< pPlayer->m_bStore
		<< (pPlayer->m_bStore ? pPlayer->m_strStoreName : NAME_NULL)
		<< pPlayer->m_dwRiding
		<< pPlayer->m_bClass
		<< pPlayer->m_bRace
		<< pPlayer->m_bCountry
		<< pPlayer->m_bAidCountry
		<< pPlayer->m_bSex
		<< pPlayer->m_bHair
		<< pPlayer->m_bFace
		<< pPlayer->m_bBody
		<< pPlayer->m_bPants
		<< pPlayer->m_bHand
		<< pPlayer->m_bFoot
		<< pPlayer->m_bLevel
		<< pPlayer->m_bHelmetHide
		<< pPlayer->GetMaxHP()
		<< pPlayer->m_dwHP
		<< pPlayer->GetMaxMP()
		<< pPlayer->m_dwMP
		<< pPlayer->GetPartyChiefID()
		<< pPlayer->GetPartyID()
		<< pPlayer->GetCommanderID()
		<< pPlayer->m_fPosX
		<< pPlayer->m_fPosY
		<< pPlayer->m_fPosZ
		<< pPlayer->m_bAction
		<< pPlayer->m_bBlock
		<< pPlayer->m_bMode
		<< pPlayer->m_wPitch
		<< pPlayer->m_wDIR
		<< pPlayer->m_bMouseDIR
		<< pPlayer->m_bKeyDIR
		<< bColor
		<< pPlayer->m_dwRegion
		<< pPlayer->m_bInPcBang
		<< pPlayer->m_aftermath.m_bStep
		<< WORD(pPlayer->IsInCastle() ? m_wCastle : 0)
		<< BYTE(pPlayer->IsInCastle() ? pPlayer->m_bCamp : 0)
		<< WORD(pPlayer->IsInCastle() ? pPlayer->m_wGodBall : 0);

	(*pMSG) << (BYTE)pPlayer->m_vMaintainSkill.size();
	for(BYTE is = 0; is<(BYTE)pPlayer->m_vMaintainSkill.size(); is++)
	{
		(*pMSG)
			<< pPlayer->m_vMaintainSkill[is]->GetID()
			<< pPlayer->m_vMaintainSkill[is]->m_bLevel
			<< pPlayer->m_vMaintainSkill[is]->GetRemainTick(dwTick)
			<< pPlayer->m_vMaintainSkill[is]->m_dwAttackID
			<< pPlayer->m_vMaintainSkill[is]->m_bAttackType
			<< pPlayer->m_vMaintainSkill[is]->m_dwHostID
			<< pPlayer->m_vMaintainSkill[is]->m_bHostType
			<< pPlayer->m_vMaintainSkill[is]->m_bHit
			<< pPlayer->m_vMaintainSkill[is]->m_wAttackLevel
			<< pPlayer->m_vMaintainSkill[is]->m_bAttackerLevel
			<< pPlayer->m_vMaintainSkill[is]->m_dwPysMinPower
			<< pPlayer->m_vMaintainSkill[is]->m_dwPysMaxPower
			<< pPlayer->m_vMaintainSkill[is]->m_dwMgMinPower
			<< pPlayer->m_vMaintainSkill[is]->m_dwMgMaxPower
			<< pPlayer->m_vMaintainSkill[is]->m_bCanSelect
			<< pPlayer->m_vMaintainSkill[is]->m_bAttackCountry
			<< pPlayer->m_vMaintainSkill[is]->m_fPosX
			<< pPlayer->m_vMaintainSkill[is]->m_fPosY
			<< pPlayer->m_vMaintainSkill[is]->m_fPosZ;
	}

	CTInven *pTINVEN = pPlayer->FindTInven(INVEN_EQUIP);
	if(pTINVEN)
	{
		MAPTITEM::iterator itTITEM;

		(*pMSG)
			<< BYTE(pTINVEN->m_mapTITEM.size());

		for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
		{
			CTItem *pTITEM = (*itTITEM).second;
			pTITEM->WrapPacketClient(pMSG, pPlayer->m_dwID);
		}

		(*pMSG)
			<< bNewMember;
	}
	else
	{
		(*pMSG)
			<< BYTE(0)
			<< bNewMember;
	}


	Say(pMSG);
}

void CTPlayer::SendCS_OTHERSELF_ACK(CTPlayer * pPlayer,
									CTRecallMon * pMon,
									DWORD dwTick,
									BYTE bNewMember)
{
	CPacket *pMSG = new CPacket();

	BYTE bColor = !pPlayer->CanFight(m_bType, m_bCountry, m_bAidCountry) ? TNCOLOR_ALLI : TNCOLOR_ENEMY;

	pMSG->SetID(CS_OTHERSELF_ACK)
		<< pMon->m_dwID
		<< pPlayer->m_dwID
		<< pPlayer->m_strNAME
		<< pPlayer->m_dwGuildID
		<< pPlayer->m_dwFame
		<< pPlayer->m_dwFameColor
		<< pPlayer->m_strGuildName
		<< pPlayer->m_bGuildPeer
		<< pPlayer->m_dwTacticsID
		<< pPlayer->m_strTacticsName
		<< pPlayer->m_bClass
		<< pPlayer->m_bRace
		<< pPlayer->m_bCountry
		<< pPlayer->m_bAidCountry
		<< pPlayer->m_bSex
		<< pPlayer->m_bHair
		<< pPlayer->m_bFace
		<< pPlayer->m_bBody
		<< pPlayer->m_bPants
		<< pPlayer->m_bHand
		<< pPlayer->m_bFoot
		<< pPlayer->m_bHelmetHide
		<< pPlayer->m_bLevel
		<< pPlayer->GetPartyChiefID()
		<< pPlayer->GetPartyID()
		<< pPlayer->GetCommanderID();

	CTInven *pTINVEN = pPlayer->FindTInven(INVEN_EQUIP);
	if(pTINVEN)
	{
		MAPTITEM::iterator itTITEM;

		(*pMSG)
			<< BYTE(pTINVEN->m_mapTITEM.size());

		for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
		{
			CTItem *pTITEM = (*itTITEM).second;
			pTITEM->WrapPacketClient(pMSG, pPlayer->m_dwID);
		}
	}
	else
	{
		(*pMSG)
			<< BYTE(0);
	}

	Say(pMSG);
}

void CTPlayer::SendCS_LEAVE_ACK( DWORD dwCharID, BYTE bExitMap)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_LEAVE_ACK)
		<< dwCharID
		<< bExitMap;

	Say(pMSG);
}

void CTPlayer::SendCS_MOVE_ACK( DWORD dwCharID,
							    FLOAT fPosX,
								FLOAT fPosY,
								FLOAT fPosZ,
								WORD wPitch,
								WORD wDIR,
								BYTE bMouseDIR,
								BYTE bKeyDIR,
								BYTE bAction)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_MOVE_ACK)
		<< dwCharID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wPitch
		<< wDIR
		<< bMouseDIR
		<< bKeyDIR
		<< bAction;

	Say(pMSG);
}

void CTPlayer::SendCS_JUMP_ACK( DWORD dwObjID,
							    BYTE bObjType,
							    FLOAT fPosX,
								FLOAT fPosY,
								FLOAT fPosZ,
								WORD wPitch,
								WORD wDIR,
								BYTE bAction)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_JUMP_ACK)
		<< dwObjID
		<< bObjType
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wPitch
		<< wDIR
		<< bAction;

	Say(pMSG);
}

void CTPlayer::SendCS_BLOCK_ACK( DWORD dwObjID,
								 BYTE bObjType,
								 FLOAT fPosX,
								 FLOAT fPosY,
								 FLOAT fPosZ,
								 WORD wPitch,
								 WORD wDIR,
								 BYTE bAction,
								 BYTE bBlock)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_BLOCK_ACK)
		<< dwObjID
		<< bObjType
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wPitch
		<< wDIR
		<< bAction
		<< bBlock;

	Say(pMSG);
}

void CTPlayer::SendCS_MONHOST_ACK( DWORD dwMonID,
								   BYTE bSet)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_MONHOST_ACK)
		<< dwMonID
		<< bSet;

	Say(pMSG);
}

void CTPlayer::SendCS_ADDSELFOBJ_ACK(CTSelfObj * pSelf, BYTE bNewMember, DWORD dwTick)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_ADDSELFOBJ_ACK)
		<< pSelf->m_dwHostID
		<< pSelf->m_dwID
		<< pSelf->m_pMON->m_wID
		<< pSelf->m_bCountry
		<< pSelf->m_bAidCountry
		<< pSelf->GetColor( m_dwID, GetPartyID(), GetWarCountry())
		<< pSelf->m_bLevel
		<< pSelf->GetMaxHP()
		<< pSelf->m_dwHP
		<< pSelf->GetMaxMP()
		<< pSelf->m_dwMP
		<< pSelf->m_fPosX
		<< pSelf->m_fPosY
		<< pSelf->m_fPosZ
		<< pSelf->m_wPitch
		<< pSelf->m_wDIR
		<< pSelf->m_bAction
		<< pSelf->m_bMode
		<< bNewMember
		<< pSelf->m_dwRegion
		<< pSelf->m_pMON->m_bRecallType
		<< pSelf->m_bHit
		<< pSelf->m_bAtkSkillLevel
		<< pSelf->m_wAtkAL
		<< pSelf->m_bAtkLevel
		<< pSelf->m_dwPysMinPower
		<< pSelf->m_dwPysMaxPower
		<< pSelf->m_dwMgMinPower
		<< pSelf->m_dwMgMaxPower
		<< pSelf->GetLifeTick(_AtlModule.m_dwTick);

	(*pMSG) << (BYTE)pSelf->m_vMaintainSkill.size();
	for(int is = 0; is<(BYTE)pSelf->m_vMaintainSkill.size(); is++)
	{
		(*pMSG)
			<< pSelf->m_vMaintainSkill[is]->GetID()
			<< pSelf->m_vMaintainSkill[is]->m_bLevel
			<< pSelf->m_vMaintainSkill[is]->GetRemainTick(dwTick)
			<< pSelf->m_vMaintainSkill[is]->m_dwAttackID
			<< pSelf->m_vMaintainSkill[is]->m_bAttackType
			<< pSelf->m_vMaintainSkill[is]->m_dwHostID
			<< pSelf->m_vMaintainSkill[is]->m_bHostType
			<< pSelf->m_vMaintainSkill[is]->m_bHit
			<< pSelf->m_vMaintainSkill[is]->m_wAttackLevel
			<< pSelf->m_vMaintainSkill[is]->m_bAttackerLevel
			<< pSelf->m_vMaintainSkill[is]->m_dwPysMinPower
			<< pSelf->m_vMaintainSkill[is]->m_dwPysMaxPower
			<< pSelf->m_vMaintainSkill[is]->m_dwMgMinPower
			<< pSelf->m_vMaintainSkill[is]->m_dwMgMaxPower
			<< pSelf->m_vMaintainSkill[is]->m_bCanSelect
			<< pSelf->m_vMaintainSkill[is]->m_bAttackCountry
			<< pSelf->m_vMaintainSkill[is]->m_fPosX
			<< pSelf->m_vMaintainSkill[is]->m_fPosY
			<< pSelf->m_vMaintainSkill[is]->m_fPosZ;
	}

	Say(pMSG);
}

void CTPlayer::SendCS_ADDRECALLMON_ACK(CTRecallMon *pMON,
                                       BYTE bNewMember,
                                       DWORD dwTick)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_ADDRECALLMON_ACK)
		<< pMON->m_dwHostID
		<< pMON->m_dwID
		<< pMON->m_pMON->m_wID
		<< pMON->m_wPetID
		<< pMON->m_strName
		<< pMON->m_bCountry
		<< pMON->m_bAidCountry
		<< pMON->GetColor( m_dwID, GetPartyID(), GetWarCountry())
		<< pMON->m_bLevel
		<< pMON->GetMaxHP()
		<< pMON->m_dwHP
		<< pMON->GetMaxMP()
		<< pMON->m_dwMP
		<< pMON->m_fPosX
		<< pMON->m_fPosY
		<< pMON->m_fPosZ
		<< pMON->m_wPitch
		<< pMON->m_wDIR
		<< pMON->m_bMouseDIR
		<< pMON->m_bKeyDIR
		<< pMON->m_bAction
		<< pMON->m_bMode
		<< bNewMember
		<< pMON->m_dwRegion
		<< pMON->m_pMON->m_bRecallType
		<< pMON->m_bHit
		<< pMON->m_bAtkSkillLevel
		<< pMON->m_pATTR->m_wAL
		<< pMON->m_bAtkLevel
		<< pMON->GetMinAP(FALSE)
		<< pMON->GetMaxAP(FALSE)
		<< pMON->GetMinMagicAP()
		<< pMON->GetMaxMagicAP()
		<< pMON->GetLifeTick(_AtlModule.m_dwTick);

	(*pMSG) << (BYTE)pMON->m_vMaintainSkill.size();
	for(int is = 0; is<(BYTE)pMON->m_vMaintainSkill.size(); is++)
	{
		(*pMSG)
			<< pMON->m_vMaintainSkill[is]->GetID()
			<< pMON->m_vMaintainSkill[is]->m_bLevel
			<< pMON->m_vMaintainSkill[is]->GetRemainTick(dwTick)
			<< pMON->m_vMaintainSkill[is]->m_dwAttackID
			<< pMON->m_vMaintainSkill[is]->m_bAttackType
			<< pMON->m_vMaintainSkill[is]->m_dwHostID
			<< pMON->m_vMaintainSkill[is]->m_bHostType
			<< pMON->m_vMaintainSkill[is]->m_bHit
			<< pMON->m_vMaintainSkill[is]->m_wAttackLevel
			<< pMON->m_vMaintainSkill[is]->m_bAttackerLevel
			<< pMON->m_vMaintainSkill[is]->m_dwPysMinPower
			<< pMON->m_vMaintainSkill[is]->m_dwPysMaxPower
			<< pMON->m_vMaintainSkill[is]->m_dwMgMinPower
			<< pMON->m_vMaintainSkill[is]->m_dwMgMaxPower
			<< pMON->m_vMaintainSkill[is]->m_bCanSelect
			<< pMON->m_vMaintainSkill[is]->m_bAttackCountry
			<< pMON->m_vMaintainSkill[is]->m_fPosX
			<< pMON->m_vMaintainSkill[is]->m_fPosY
			<< pMON->m_vMaintainSkill[is]->m_fPosZ;
	}

	Say(pMSG);
}

void CTPlayer::SendCS_ADDMON_ACK( CTMonster *pMON,
								  BYTE bNewMember,
								  DWORD dwTick)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_ADDMON_ACK)
		<< pMON->m_dwID
		<< pMON->m_pMON->m_wID
		<< pMON->m_pMON->m_bLevel
		<< pMON->GetMaxHP()
		<< pMON->m_dwHP
		<< pMON->GetMaxMP()
		<< pMON->m_dwMP
		<< pMON->m_fPosX
		<< pMON->m_fPosY
		<< pMON->m_fPosZ
		<< pMON->m_wPitch
		<< pMON->m_wDIR
		<< pMON->m_bMouseDIR
		<< pMON->m_bKeyDIR
		<< pMON->m_bAction
		<< pMON->m_bMode
		<< bNewMember
		<< pMON->m_bCountry
		<< pMON->GetColor( m_dwID, GetPartyID(), GetWarCountry())
		<< pMON->m_dwRegion;

	(*pMSG) << (BYTE)pMON->m_vMaintainSkill.size();
	for(int is = 0; is<(BYTE)pMON->m_vMaintainSkill.size(); is++)
	{
		(*pMSG)
			<< pMON->m_vMaintainSkill[is]->GetID()
			<< pMON->m_vMaintainSkill[is]->m_bLevel
			<< pMON->m_vMaintainSkill[is]->GetRemainTick(dwTick)
			<< pMON->m_vMaintainSkill[is]->m_dwAttackID
			<< pMON->m_vMaintainSkill[is]->m_bAttackType
			<< pMON->m_vMaintainSkill[is]->m_dwHostID
			<< pMON->m_vMaintainSkill[is]->m_bHostType
			<< pMON->m_vMaintainSkill[is]->m_bHit
			<< pMON->m_vMaintainSkill[is]->m_wAttackLevel
			<< pMON->m_vMaintainSkill[is]->m_bAttackerLevel
			<< pMON->m_vMaintainSkill[is]->m_dwPysMinPower
			<< pMON->m_vMaintainSkill[is]->m_dwPysMaxPower
			<< pMON->m_vMaintainSkill[is]->m_dwMgMinPower
			<< pMON->m_vMaintainSkill[is]->m_dwMgMaxPower
			<< pMON->m_vMaintainSkill[is]->m_bCanSelect
			<< pMON->m_vMaintainSkill[is]->m_bAttackCountry
			<< pMON->m_vMaintainSkill[is]->m_fPosX
			<< pMON->m_vMaintainSkill[is]->m_fPosY
			<< pMON->m_vMaintainSkill[is]->m_fPosZ;
	}

	Say(pMSG);
}
void CTPlayer::SendCS_CHANGECOLOR_ACK(DWORD dwID,
									   BYTE bType,
									   BYTE bColor,
									   BYTE bCountry)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_CHANGECOLOR_ACK)
		<< bType
		<< dwID
		<< bColor
		<< bCountry;

	Say(pMSG);
}

void CTPlayer::SendCS_DELMON_ACK( DWORD dwMonID,
								  BYTE bExitMap)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_DELMON_ACK)
		<< dwMonID
		<< bExitMap;

	Say(pMSG);
}
void CTPlayer::SendCS_DELSELFOBJ_ACK( DWORD dwObjID, BYTE bExitMap )
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_DELSELFOBJ_ACK)
		<< dwObjID
		<< bExitMap;

	Say(pMSG);
}

void CTPlayer::SendCS_DELRECALLMON_ACK( DWORD dwHostID,
									   DWORD dwMonID,
										BYTE bExitMap)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_DELRECALLMON_ACK)
		<< dwHostID
		<< dwMonID
		<< bExitMap;

	Say(pMSG);
}

void CTPlayer::SendCS_MONACTION_ACK( DWORD dwMonID,
									 BYTE bAction,
									 FLOAT fPosX,
									 FLOAT fPosY,
									 FLOAT fPosZ,
									 DWORD dwTargetID,
									 BYTE bTargetType)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_MONACTION_ACK)
		<< dwMonID
		<< bAction
		<< fPosX
		<< fPosY
		<< fPosZ
		<< dwTargetID
		<< bTargetType;

	Say(pMSG);
}

void CTPlayer::SendCS_MONMOVE_ACK( DWORD dwMonID,
								   BYTE bType,
								   FLOAT fPosX,
								   FLOAT fPosY,
								   FLOAT fPosZ,
								   WORD wPitch,
								   WORD wDIR,
								   BYTE bMouseDIR,
								   BYTE bKeyDIR,
								   BYTE bAction)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_MONMOVE_ACK)
		<< WORD(1)
		<< dwMonID
		<< bType
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wPitch
		<< wDIR
		<< bMouseDIR
		<< bKeyDIR
		<< bAction;

	Say(pMSG);
}

void CTPlayer::SendCS_MONMOVE_ACK(LPMAPTMONSTER pMapMon)
{
	if(pMapMon->empty())
		return;

	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_MONMOVE_ACK)
		<< WORD(pMapMon->size());

	MAPTMONSTER::iterator it;
	for(it=pMapMon->begin(); it!=pMapMon->end(); it++)
	{
		CTMonster * pMon = (*it).second;
		(*pMSG)
			<< pMon->m_dwID
			<< pMon->m_bType
			<< pMon->m_fPosX
			<< pMon->m_fPosY
			<< pMon->m_fPosZ
			<< pMon->m_wPitch
			<< pMon->m_wDIR
			<< pMon->m_bMouseDIR
			<< pMon->m_bKeyDIR
			<< pMon->m_bAction;
	}

	Say(pMSG);	
}

void CTPlayer::SendCS_CHGMODE_ACK( DWORD dwID,
								   BYTE bType,
								   BYTE bMode)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_CHGMODE_ACK)
		<< dwID
		<< bType
		<< bMode;

	Say(pMSG);
}

void CTPlayer::SendCS_ACTION_ACK( BYTE bResult,
								  DWORD dwObjID,
								  BYTE bObjType,
								  BYTE bActionID,
								  DWORD dwActID,
								  DWORD dwAniID,
								  WORD wSkillID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_ACTION_ACK)
		<< bResult
		<< dwObjID
		<< bObjType
		<< bActionID
		<< dwActID
		<< dwAniID
		<< wSkillID;

	Say(pMSG);
}

void CTPlayer::SendCS_MONATTACK_ACK( DWORD dwAttackID,
									 DWORD dwTargetID,
									 BYTE bAttackType,
									 BYTE bTargetType,
									 WORD wSkillID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_MONATTACK_ACK)
		<< dwAttackID
		<< dwTargetID
		<< bAttackType
		<< bTargetType
		<< wSkillID;

	Say(pMSG);
}

void CTPlayer::SendCS_DEFEND_ACK( DWORD dwAttackID,
								  DWORD dwTargetID,
								  BYTE bAttackType,
								  BYTE bTargetType,
								  DWORD dwHostID,
								  BYTE bHostType,
								  DWORD dwActID,
								  DWORD dwAniID,
								  BYTE bIsMaintain,
								  DWORD dwMaintainTick,
								  BYTE bHit,
								  BYTE bAtkHit,
                                  WORD wAttackLevel,
								  BYTE bAttackerLevel,
                                  DWORD dwPysMinPower,
                                  DWORD dwPysMaxPower,
								  DWORD dwMgMinPower,
								  DWORD dwMgMaxPower,
								  BYTE bCanSelect,
								  BYTE bCancelCharge,
								  BYTE bAttackCountry,
								  BYTE bAttackAidCountry,
								  WORD wSkillID,
								  BYTE bSkillLevel,
								  WORD wBackSkillID,
								  BYTE bPerform,
                                  FLOAT fAtkPosX,
								  FLOAT fAtkPosY,
								  FLOAT fAtkPosZ,
                                  FLOAT fDefPosX,
								  FLOAT fDefPosY,
								  FLOAT fDefPosZ,
								  MAPWORD* mapSkillDamage)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_DEFEND_ACK)
		<< dwAttackID
		<< dwTargetID
		<< bAttackType
		<< bTargetType
		<< dwHostID
		<< bHostType
		<< dwActID
		<< dwAniID
		<< bIsMaintain
		<< dwMaintainTick
		<< bHit
		<< bAtkHit
		<< wAttackLevel
		<< bAttackerLevel
		<< dwPysMinPower
		<< dwPysMaxPower
		<< dwMgMinPower
		<< dwMgMaxPower
		<< bCanSelect
		<< bCancelCharge
		<< bAttackCountry
		<< bAttackAidCountry
		<< wSkillID
		<< bSkillLevel
		<< wBackSkillID
		<< bPerform
		<< fAtkPosX
		<< fAtkPosY
		<< fAtkPosZ
		<< fDefPosX
		<< fDefPosY
		<< fDefPosZ;

	if(mapSkillDamage)
	{
		(*pMSG)
			<< BYTE(mapSkillDamage->size());

		MAPWORD::iterator it;
		for(it=mapSkillDamage->begin(); it!=mapSkillDamage->end(); it++)
		{
			(*pMSG)
				<< BYTE((*it).first)
				<< DWORD((*it).second);
		}
	}
	else
		(*pMSG)	<< BYTE(0);

	Say(pMSG);
}

void CTPlayer::SendCS_HPMP_ACK( DWORD dwID,
							    BYTE bType,
								BYTE bLevel,
								DWORD dwMaxHP,
							    DWORD dwHP,
								DWORD dwMaxMP,
								DWORD dwMP)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_HPMP_ACK)
		<< dwID
		<< bType
		<< dwMaxHP
		<< dwHP
		<< dwMaxMP
		<< dwMP;
	Say(pMSG);

	if( GetPartyID() && dwID == m_dwID )
		_AtlModule.SendMW_PARTYMANSTAT_ACK( GetPartyID(), dwID, bType, bLevel, dwMaxHP, dwHP, dwMaxMP, dwMP);
}

void CTPlayer::SendCS_LEVEL_ACK( DWORD dwCharID,
								 BYTE bLevel)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_LEVEL_ACK)
		<< dwCharID
		<< bLevel;

	Say(pMSG);
}

void CTPlayer::SendCS_EXP_ACK()
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_EXP_ACK)
		<< m_dwEXP
		<< m_pTPREVLEVEL->m_dwEXP
		<< m_pTLEVEL->m_dwEXP;

	Say(pMSG);
}

void CTPlayer::SendCS_DIE_ACK( DWORD dwID,
							   BYTE bType)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_DIE_ACK)
		<< dwID
		<< bType;

	Say(pMSG);
}

void CTPlayer::SendCS_REVIVAL_ACK( DWORD dwCharID,
								   FLOAT fPosX,
								   FLOAT fPosY,
								   FLOAT fPosZ)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_REVIVAL_ACK)
		<< dwCharID
		<< fPosX
		<< fPosY
		<< fPosZ;

	Say(pMSG);
}

void CTPlayer::SendCS_MOVEITEM_ACK( BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_MOVEITEM_ACK)
		<< bResult;

	Say(pMSG);
}

void CTPlayer::SendCS_UPDATEITEM_ACK( CTItem *pTITEM,
									  BYTE bInvenID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_UPDATEITEM_ACK)
		<< bInvenID;

	pTITEM->WrapPacketClient(pMSG, m_dwID);

	Say(pMSG);
}

void CTPlayer::SendCS_ADDITEM_ACK( CTItem *pTITEM,
								   BYTE bInvenID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_ADDITEM_ACK)
		<< bInvenID;

	pTITEM->WrapPacketClient(pMSG, m_dwID);

	Say(pMSG);
}

void CTPlayer::SendCS_DELITEM_ACK( BYTE bInvenID,
								   BYTE bItemID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_DELITEM_ACK)
		<< bInvenID
		<< bItemID;

	Say(pMSG);
}

void CTPlayer::SendCS_EQUIP_ACK( CTPlayer *pPlayer)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_EQUIP_ACK)
		<< pPlayer->m_dwID;

	CTInven *pTINVEN = pPlayer->FindTInven(INVEN_EQUIP);
	if(pTINVEN)
	{
		MAPTITEM::iterator itTITEM;

		(*pMSG)
			<< BYTE(pTINVEN->m_mapTITEM.size());

		for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
		{
			CTItem *pTITEM = (*itTITEM).second;
			pTITEM->WrapPacketClient(pMSG, pPlayer->m_dwID);
		}
	}
	else
		(*pMSG) << BYTE(0);

	Say(pMSG);
}
void CTPlayer::SendCS_SKILLBUY_ACK(BYTE bRet, WORD wSkillID, BYTE bLevel)
{
	CPacket *pMsg = new CPacket();
	pMsg->SetID(CS_SKILLBUY_ACK)
		<< bRet
		<< wSkillID
		<< bLevel
		<< m_dwGold
		<< m_dwSilver
		<< m_dwCooper
		<< m_wSkillPoint;

	WORD arPoint[4];
	GetSkillKindPoint(arPoint);
	(*pMsg)
		<< arPoint[0]
		<< arPoint[1]
		<< arPoint[2]
		<< arPoint[3];

	Say(pMsg);
}

void CTPlayer::SendCS_SKILLUSE_ACK(BYTE bResult,
								   DWORD dwAttackID,
								   BYTE bAttackType,
								   WORD wSkillID,
								   BYTE bActionID,
								   DWORD dwActID,
								   DWORD dwAniID,
								   BYTE bSkillLevel,
								   WORD wBackSkill,
								   WORD	wAttackLevel,
								   BYTE bAttackerLevel,
                                   DWORD dwPysMinPower,
                                   DWORD dwPysMaxPower,
                                   DWORD dwMgMinPower,
                                   DWORD dwMgMaxPower,
								   WORD wTransHP,
								   WORD wTransMP,
								   BYTE bCurseProb,
                                   BYTE bEquipSpecial,
                                   BYTE bCanSelect,
								   BYTE bAttackCountry,
								   BYTE bAttackAidCountry,
								   BYTE bCP,
								   FLOAT fGndPosX,
								   FLOAT fGndPosY,
								   FLOAT fGndPosZ,
								   LPVDWORD vTarget,
								   LPVBYTE vTargetType)
{
	CPacket *pMsg = new CPacket();
	pMsg->SetID(CS_SKILLUSE_ACK)
		<< bResult
		<< dwAttackID
		<< bAttackType
		<< wSkillID
		<< wBackSkill
		<< bActionID
		<< dwActID
		<< dwAniID
		<< bSkillLevel
		<< wAttackLevel
		<< bAttackerLevel
		<< dwPysMinPower
		<< dwPysMaxPower
		<< dwMgMinPower
		<< dwMgMaxPower
		<< wTransHP
		<< wTransMP
		<< bCurseProb
		<< bEquipSpecial
		<< bCanSelect
		<< bAttackCountry
		<< bAttackAidCountry
		<< bCP
		<< fGndPosX
		<< fGndPosY
		<< fGndPosZ;

	if(vTarget)
	{
		(*pMsg)
			<< (BYTE)vTarget->size();

		for(int i=0; i<(int)vTarget->size(); i++)
		{
			(*pMsg)
				<< (*vTarget)[i]
				<< (*vTargetType)[i];
		}
	}
	else
	{
		(*pMsg)
			<< BYTE(0);
	}

	Say(pMsg);
}

void CTPlayer::SendCS_LOOPSKILL_ACK(BYTE bResult,
								   DWORD dwAttackID,
								   BYTE bAttackType,
								   WORD wSkillID,
								   BYTE bSkillLevel,
                                   WORD	wAttackLevel,
								   BYTE bAttackerLevel,
                                   DWORD dwPysMinPower,
                                   DWORD dwPysMaxPower,
                                   DWORD dwMgMinPower,
                                   DWORD dwMgMaxPower,
                                   BYTE bCanSelect,
								   BYTE bAttackCountry,
								   BYTE bAttackAidCountry,
								   BYTE bHit,
								   FLOAT fGndPosX,
								   FLOAT fGndPosY,
								   FLOAT fGndPosZ,
								   LPVDWORD vTarget,
								   LPVBYTE vTargetType)
{
	CPacket *pMsg = new CPacket();
	pMsg->SetID(CS_LOOPSKILL_ACK)
		<< bResult
		<< dwAttackID
		<< bAttackType
		<< wSkillID
		<< bSkillLevel
		<< wAttackLevel
		<< bAttackerLevel
		<< dwPysMinPower
		<< dwPysMaxPower
		<< dwMgMinPower
		<< dwMgMaxPower
		<< bCanSelect
		<< bAttackCountry
		<< bAttackAidCountry
		<< bHit
		<< fGndPosX
		<< fGndPosY
		<< fGndPosZ;

	if(vTarget)
	{
		(*pMsg)
			<< (BYTE)vTarget->size();

		for(int i=0; i<(int)vTarget->size(); i++)
		{
			(*pMsg)
				<< (*vTarget)[i]
				<< (*vTargetType)[i];
		}
	}
	else
	{
		(*pMsg)
			<< BYTE(0);
	}

	Say(pMsg);
}

void CTPlayer::SendCS_SKILLEND_ACK(DWORD dwObjID,
								   BYTE bObjType,
								   WORD wSkillID)
{
	CPacket *pMsg = new CPacket();
	pMsg->SetID(CS_SKILLEND_ACK)
		<< dwObjID
		<< bObjType
		<< wSkillID;

	Say(pMsg);
}
void CTPlayer::SendCS_CHGPARTYTYPE_ACK(BYTE bRet,
									   BYTE bPartyType)
{
	CPacket *pMsg = new CPacket();
	pMsg->SetID(CS_CHGPARTYTYPE_ACK)
		<< bRet
		<< bPartyType;

	Say(pMsg);
}
void CTPlayer::SendCS_PARTYADD_ACK( CString strRequestID,
								   CString strTargetID,
								   BYTE bReply)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_PARTYADD_ACK)
		<< strRequestID
		<< strTargetID
		<< bReply;

	Say(pMSG);
}
void CTPlayer::SendCS_PARTYJOINASK_ACK(CString strRequestID,
									   BYTE bObtainType)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_PARTYJOINASK_ACK)
		<< strRequestID
		<< bObtainType;

	Say(pMSG);
}
void CTPlayer::SendCS_PARTYJOIN_ACK(WORD wPartyID,
									CString strMemberID,
									DWORD dwMemberID,
									DWORD dwChiefID,
									WORD wCommanderID,
									CString strGuildName,
									BYTE bLevel,
									DWORD dwMaxHP,
									DWORD dwHP,
									DWORD dwMaxMP,
									DWORD dwMP,
									BYTE bRace,
									BYTE bSex,
									BYTE bFace,
									BYTE bHair,
									BYTE bPartyType,
									BYTE bClass)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_PARTYJOIN_ACK)
		<< wPartyID
		<< strMemberID
		<< dwMemberID
		<< dwChiefID
		<< wCommanderID
		<< strGuildName
		<< bLevel
		<< dwMaxHP
		<< dwHP
		<< dwMaxMP
		<< dwMP
		<< bRace
		<< bSex
		<< bFace
		<< bHair
		<< bPartyType
		<< bClass;

	Say(pMSG);
}

void CTPlayer::SendCS_PARTYDEL_ACK( DWORD dwMemberID,
									DWORD dwChiefID,
									WORD wCommanderID,
									WORD wPartyID,
									BYTE bKicked)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_PARTYDEL_ACK)
		<< dwMemberID
		<< dwChiefID
		<< wCommanderID
		<< wPartyID
		<< bKicked;

	Say(pMSG);
}

void CTPlayer::SendCS_PARTYMANSTAT_ACK( DWORD dwID,
										BYTE bType,
										BYTE bLevel,
										DWORD dwMaxHP,
										DWORD dwCurHP,
										DWORD dwMaxMP,
										DWORD dwCurMP)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_PARTYMANSTAT_ACK)
		<< dwID
		<< bType
		<< bLevel
		<< dwMaxHP
		<< dwCurHP
		<< dwMaxMP
		<< dwCurMP;
	Say(pMsg);
}
void CTPlayer::SendCS_QUESTADD_ACK( DWORD dwQuestID,
								   BYTE bType)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_QUESTADD_ACK)
		<< dwQuestID
		<< bType;

#ifdef DEF_UDPLOG
	_AtlModule.m_pUdpSocket->LogQuest(LOGMAP_QUESTSTART, this, dwQuestID);
#endif

	Say(pMSG);
}

void CTPlayer::SendCS_QUESTUPDATE_ACK( DWORD dwQuestID,
									  DWORD dwTermID,
									  BYTE bType,
									  BYTE bCount,
									  BYTE bStatus)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_QUESTUPDATE_ACK)
		<< dwQuestID
		<< dwTermID
		<< bType
		<< bCount
		<< bStatus;

	Say(pMSG);
}

void CTPlayer::SendCS_QUESTCOMPLETE_ACK( BYTE bResult,
									    DWORD dwQuestID,
										DWORD dwTermID,
										BYTE bType,
										DWORD dwDropID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_QUESTCOMPLETE_ACK)
		<< bResult
		<< dwQuestID
		<< dwTermID
		<< bType
		<< dwDropID;

	Say(pMSG);
}

void CTPlayer::SendCS_QUESTLIST_ACK( DWORD dwTick)
{
	MAPQUEST::iterator it;
	VQUEST vQUEST;
	vQUEST.clear();

	for( it = m_mapQUEST.begin(); it != m_mapQUEST.end(); it++)
	{
		if( (*it).second->m_bCompleteCount < (*it).second->m_bTriggerCount )
			vQUEST.push_back((*it).second);
	}

	BYTE bCount = (BYTE) vQUEST.size();
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_QUESTLIST_ACK)
		<< bCount;

	for( int i=0; i<bCount; i++)
	{
		BYTE bTermCount = (BYTE) vQUEST[i]->m_pQUEST->m_vTerm.size();

		(*pMSG)
			<< vQUEST[i]->m_pQUEST->m_dwQuestID
			<< vQUEST[i]->m_pQUEST->m_bType
			<< bTermCount;

		for( int j=0; j<bTermCount; j++)
		{
			LPQUESTTERM pTERM = vQUEST[i]->FindRunningTerm(
				vQUEST[i]->m_pQUEST->m_vTerm[j]->m_dwTermID,
				vQUEST[i]->m_pQUEST->m_vTerm[j]->m_bTermType);

			BYTE bStatus = vQUEST[i]->CheckComplete(
				this,
				dwTick,
				vQUEST[i]->m_pQUEST->m_vTerm[j]);

			(*pMSG)
				<< vQUEST[i]->m_pQUEST->m_vTerm[j]->m_dwTermID
				<< vQUEST[i]->m_pQUEST->m_vTerm[j]->m_bTermType
				<< vQUEST[i]->m_pQUEST->m_vTerm[j]->m_bCount
				<< (BYTE) (pTERM ? pTERM->m_bCount : GetTermCount(vQUEST[i]->m_pQUEST->m_vTerm[j]->m_dwTermID, vQUEST[i]->m_pQUEST->m_vTerm[j]->m_bTermType))
				<< bStatus;
		}
	}
	vQUEST.clear();

	Say(pMSG);
}

void CTPlayer::SendCS_QUESTLIST_COMPLETE_ACK()
{
	MAPQUEST::iterator it;
	VDWORD vQuestID;

	for( it = m_mapQUEST.begin(); it != m_mapQUEST.end(); ++it )
	{
		if( (*it).second->m_pQUEST->m_dwParentID == 0 )
		{
			DWORD dwQuestID = GetCompleteQuestID( (*it).second, (*it).second->m_pQUEST->m_dwQuestID );
			if( dwQuestID != 0 )
				vQuestID.push_back( dwQuestID );
		}
	}

	BYTE bCount = (BYTE) vQuestID.size();
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_QUESTLIST_COMPLETE_ACK)
		<< bCount;

	for( int i=0; i<bCount; ++i )
		(*pMSG) << vQuestID[i];

	vQuestID.clear();

	Say(pMSG);
}
void CTPlayer::SendCS_QUESTSTARTTIMER_ACK(DWORD dwQuestID,
                                          DWORD dwTick)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_QUESTSTARTTIMER_ACK)
		<< dwQuestID
		<< dwTick;

	Say(pMSG);
}
void CTPlayer::SendCS_QUESTLIST_POSSIBLE_ACK(LPMAPMAPDWORD mapNpcQuest)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_QUESTLIST_POSSIBLE_ACK)
		<< BYTE(mapNpcQuest->size());

	MAPMAPDWORD::iterator itN;
	MAPDWORD::iterator itQ;
	for(itN=mapNpcQuest->begin(); itN!=mapNpcQuest->end(); itN++)
	{
		CTNpc * pNpc = _AtlModule.FindTNpc((*itN).first);
		if(!pNpc)
			continue;

		(*pMSG)
			<< pNpc->m_wID
			<< BYTE(pNpc->m_pLocal ? pNpc->m_pLocal->m_bCountry : pNpc->m_bCountry)
			<< BYTE((*itN).second.size());

//		ATLTRACE2("NPC ID : %d, size %d",(*itN).first, (*itN).second.size());
		LPMAPDWORD mapQuest = &((*itN).second);
		for(itQ = mapQuest->begin(); itQ != mapQuest->end(); itQ++)
		{
			(*pMSG)
				<< (*itQ).first
				<< BYTE((*itQ).second);
//			ATLTRACE2(" QUEST ID %d, Type %d",(*itQ).first, (*itQ).second);
		}
//		ATLTRACE2("\n");
	}

	Say(pMSG);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 길드
void CTPlayer::SendCS_GUILDESTABLISH_ACK(BYTE bResult, DWORD dwID, CString strName)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDESTABLISH_ACK)
		<< bResult
		<< dwID
		<< strName;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDDISORGANIZATION_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDDISORGANIZATION_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDINVITE_ACK(BYTE bResult,
									  CString strName,
                                      DWORD dwInviter,
                                      CString strInviterName)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDINVITE_ACK)
		<< bResult
		<< strName
		<< dwInviter
		<< strInviterName;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDJOIN_ACK(BYTE bRet,
									DWORD dwGuildID,
                                    CString strGuildName,
									DWORD dwNewMemberID,
                                    CString strNewMemberName,
									BYTE bMaxGuildMember)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDJOIN_ACK)
		<< bRet
		<< dwGuildID
		<< strGuildName
		<< dwNewMemberID
		<< strNewMemberName
		<< bMaxGuildMember;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDDUTY_ACK(BYTE bResult,
									CString strTarget,
									BYTE bDuty)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDDUTY_ACK)
		<< bResult
		<< strTarget
		<< bDuty;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDPEER_ACK(BYTE bResult,
									CString strTarget,
									BYTE bPeer,
									BYTE bOldPeer)
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(CS_GUILDPEER_ACK)
		<< bResult
		<< strTarget
		<< bPeer
		<< bOldPeer;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDLEAVE_ACK(BYTE bResult,
									 CString strTarget, 
									 BYTE bReason)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDLEAVE_ACK)
		<< bResult
		<< strTarget
		<< bReason;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDMEMBERLIST_ACK(LPPACKETBUF pBUF)
{
	CPacket *pMSG = new CPacket();

	pMSG->CopyData(&pBUF->m_packet, sizeof(DWORD)*2);
	pMSG->SetID(CS_GUILDMEMBERLIST_ACK);

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDINFO_ACK(LPPACKETBUF pBUF)
{
	CPacket *pMSG = new CPacket();

	pMSG->CopyData(&pBUF->m_packet, sizeof(DWORD)*2);
	pMSG->SetID(CS_GUILDINFO_ACK);

	Say(pMSG);
}

void CTPlayer::SendCS_GUILDLOCALLIST_ACK()
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDLOCALLIST_ACK)
		<< WORD(_AtlModule.m_vCastle.size());

	for(DWORD ll=0; ll<_AtlModule.m_vCastle.size(); ll++)
	{
		LPTLOCAL pCastle = _AtlModule.m_vCastle[ll];
		WORD wDefGuildPoint = 0;
		WORD wAtkGuildPoint = 0;
		WORD wMyGuildPoint = 0;

		MAPDWORD::iterator itGP = pCastle->m_mapTPoint.find(pCastle->m_dwDefGuildID);
		if(itGP != pCastle->m_mapTPoint.end())
			wDefGuildPoint = WORD((*itGP).second);

		itGP = pCastle->m_mapTPoint.find(pCastle->m_dwAtkGuildID);
		if(itGP != pCastle->m_mapTPoint.end())
			wAtkGuildPoint = WORD((*itGP).second);

		itGP = pCastle->m_mapTPoint.find(GetGuild());
		if(itGP != pCastle->m_mapTPoint.end())
			wMyGuildPoint = WORD((*itGP).second);

		(*pMSG)
			<< pCastle->m_wLocal
			<< pCastle->m_pZone->m_strName
			<< BYTE(_AtlModule.CanApplyCastle(pCastle) ? FALSE : TRUE)
			<< pCastle->m_dwGuild
			<< pCastle->m_strGuild
			<< pCastle->m_bCountry
			<< pCastle->m_timeNextDefend
			<< pCastle->m_strHero
			<< pCastle->m_strDefName
			<< pCastle->m_strAtkName
			<< wDefGuildPoint
			<< pCastle->m_wDPoint
			<< pCastle->m_bDefCount
			<< wAtkGuildPoint
			<< pCastle->m_wCPoint
			<< pCastle->m_bAtkCount
			<< wMyGuildPoint
			<< BYTE(pCastle->m_vTop3[TCONTRY_D].size());

		for(BYTE t3=0; t3<pCastle->m_vTop3[TCONTRY_D].size(); t3++)
		{
			(*pMSG)
				<< pCastle->m_vTop3[TCONTRY_D][t3].m_strName
				<< pCastle->m_vTop3[TCONTRY_D][t3].m_wPoint;
		}

		(*pMSG)
			<< BYTE(pCastle->m_vTop3[TCONTRY_C].size());

		for(BYTE t3=0; t3<pCastle->m_vTop3[TCONTRY_C].size(); t3++)
		{
			(*pMSG)
				<< pCastle->m_vTop3[TCONTRY_C][t3].m_strName
				<< pCastle->m_vTop3[TCONTRY_C][t3].m_wPoint;
		}

		(*pMSG)
			<< WORD(pCastle->m_mapLocal.size());

		MAPTLOCAL::iterator itLOCAL;
		for(itLOCAL=pCastle->m_mapLocal.begin(); itLOCAL!=pCastle->m_mapLocal.end(); itLOCAL++)
		{
			(*pMSG)
				<< (*itLOCAL).second->m_wLocal
				<< (*itLOCAL).second->m_pZone->m_strName
				<< (*itLOCAL).second->m_dwGuild
				<< (*itLOCAL).second->m_strGuild
				<< (*itLOCAL).second->m_bCountry
				<< (*itLOCAL).second->m_timeNextDefend
				<< (*itLOCAL).second->m_strHero;
		}
	}

	(*pMSG)
		<< BYTE(_AtlModule.m_vMission.size());

	for(DWORD ms=0; ms<_AtlModule.m_vMission.size(); ms++)
	{
		LPTLOCAL pMission = _AtlModule.m_vMission[ms];
		(*pMSG)
			<< pMission->m_wLocal
			<< pMission->m_pZone->m_strName
			<< pMission->m_bCountry
			<< pMission->m_bStatus
			<< pMission->m_timeNextDefend;
	}

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDLOCALRETURN_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDLOCALRETURN_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDCABINETLIST_ACK(BYTE bMaxCabinet, VTITEM vItem)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDCABINETLIST_ACK)
		<< bMaxCabinet
		<< BYTE(vItem.size());

	while(!vItem.empty())
	{
		CTItem * pItem = vItem.back();
		vItem.pop_back();
		(*pMSG)
			<< pItem->m_dwStItemID;

		pItem->WrapPacketClient(pMSG, m_dwID, FALSE);

		delete pItem;
	}

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDCABINETPUTIN_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDCABINETPUTIN_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDCABINETTAKEOUT_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDCABINETTAKEOUT_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDCONTRIBUTION_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDCONTRIBUTION_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDARTICLELIST_ACK(LPPACKETBUF pBUF)
{
	CPacket *pMSG = new CPacket();

	pMSG->CopyData(&pBUF->m_packet, sizeof(DWORD)*2);
	pMSG->SetID(CS_GUILDARTICLELIST_ACK);

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDARTICLEADD_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDARTICLEADD_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDARTICLEDEL_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDARTICLEDEL_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDARTICLEUPDATE_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDARTICLEUPDATE_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDFAME_ACK(BYTE bResult, DWORD dwFame, DWORD dwFameColor)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDFAME_ACK)
		<< bResult
		<< dwFame
		<< dwFameColor;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDWANTEDADD_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDWANTEDADD_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDWANTEDDEL_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDWANTEDDEL_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDWANTEDLIST_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);

	pMsg->SetID(CS_GUILDWANTEDLIST_ACK);
	Say(pMsg);
}
void CTPlayer::SendCS_GUILDVOLUNTEERING_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDVOLUNTEERING_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDVOLUNTEERINGDEL_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDVOLUNTEERINGDEL_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDVOLUNTEERLIST_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);

	pMsg->SetID(CS_GUILDVOLUNTEERLIST_ACK);
	Say(pMsg);
}
void CTPlayer::SendCS_GUILDVOLUNTEERREPLY_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDVOLUNTEERREPLY_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDTACTICSWANTEDADD_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDTACTICSWANTEDADD_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDTACTICSWANTEDDEL_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDTACTICSWANTEDDEL_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDTACTICSWANTEDLIST_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);

	pMsg->SetID(CS_GUILDTACTICSWANTEDLIST_ACK);
	Say(pMsg);
}
void CTPlayer::SendCS_GUILDTACTICSVOLUNTEERING_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDTACTICSVOLUNTEERING_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDTACTICSVOLUNTEERINGDEL_ACK(BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDTACTICSVOLUNTEERINGDEL_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDTACTICSVOLUNTEERLIST_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);

	pMsg->SetID(CS_GUILDTACTICSVOLUNTEERLIST_ACK);
	Say(pMsg);
}
void CTPlayer::SendCS_GUILDTACTICSREPLY_ACK(BYTE bResult, DWORD dwTarget)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDTACTICSREPLY_ACK)
		<< bResult
		<< dwTarget;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDTACTICSKICKOUT_ACK(BYTE bResult, DWORD dwTarget)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDTACTICSKICKOUT_ACK)
		<< bResult
		<< dwTarget;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDTACTICSINVITE_ACK(CString strGuildName,
											 CString strName,
                                             BYTE bDay,
                                             DWORD dwPoint,
                                             DWORD dwGold,
                                             DWORD dwSilver,
                                             DWORD dwCooper)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDTACTICSINVITE_ACK)
		<< strGuildName
		<< strName
		<< bDay
		<< dwPoint
		<< dwGold
		<< dwSilver
		<< dwCooper;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDTACTICSANSWER_ACK(BYTE bResult,
											 DWORD dwMemberID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_GUILDTACTICSANSWER_ACK)
		<< bResult
		<< dwMemberID;

	Say(pMSG);
}
void CTPlayer::SendCS_GUILDTACTICSLIST_ACK(LPPACKETBUF pBUF)
{
	CPacket *pMSG = new CPacket();

	pMSG->CopyData(&pBUF->m_packet, sizeof(DWORD)*2);
	pMSG->SetID(CS_GUILDTACTICSLIST_ACK);

	Say(pMSG);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void CTPlayer::SendCS_CHAT_ACK(BYTE bGroup,
							   DWORD dwSenderID,
							   CString strName,
							   CString strTalk)
{
	// 차단목록 검사
	if(!CheckProtected(dwSenderID, PROTECTED_CHAT))
			return;

	CPacket * pMSG = new CPacket();

	pMSG->SetID(CS_CHAT_ACK)
		<< bGroup
		<< dwSenderID
		<< strName
		<< strTalk;

	Say(pMSG);
}

void CTPlayer::SendCS_CABINETLIST_ACK()
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_CABINETLIST_ACK)
		<< BYTE(m_mapCabinet.size());

	MAPTCABINET::iterator it;
	for(it=m_mapCabinet.begin(); it!=m_mapCabinet.end(); it++)
	{
		(*pMSG)
			<< (*it).second->m_bCabinetID
			<< (*it).second->m_bUse;
	}

	Say(pMSG);
}

void CTPlayer::SendCS_CABINETITEMLIST_ACK(BYTE bResult, LPTCABINET pTCABINET)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_CABINETITEMLIST_ACK)
		<< bResult;

	if(pTCABINET)
	{
		(*pMSG)
			<< pTCABINET->m_bCabinetID
			<< DWORD(pTCABINET->m_mapCabinetItem.size());

		MAPTSTORAGEITEM::iterator itITEM;

		for(itITEM=pTCABINET->m_mapCabinetItem.begin(); itITEM!=pTCABINET->m_mapCabinetItem.end(); itITEM++)
		{
			CTItem * pTITEM = (*itITEM).second;
			(*pMSG)
				<< pTITEM->m_dwStItemID;

			pTITEM->WrapPacketClient(pMSG, m_dwID, FALSE);
		}
	}

	Say(pMSG);
}

void CTPlayer::SendCS_CABINETOPEN_ACK(BYTE bResult, BYTE bCabinetID)
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(CS_CABINETOPEN_ACK)
		<< bResult
		<< bCabinetID;

	Say(pMSG);
}

void CTPlayer::SendCS_NPCITEMLIST_ACK(CTNpc * pNpc)
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(CS_NPCITEMLIST_ACK)
		<< pNpc->m_wID
		<< pNpc->m_bType;

	BYTE bDiscountRate = _AtlModule.GetDiscountRate(this,pNpc);	

	(*pMSG) << bDiscountRate;

	switch(pNpc->m_bType)
	{
	case TNPC_SKILL_RENT:
	case TNPC_SKILL_MASTER:
		{
			MAPTSKILLTEMP::iterator it;
			MAPTSKILLTEMP mapSKILL;
			mapSKILL.clear();
			for(it=pNpc->m_mapSkill.begin(); it!=pNpc->m_mapSkill.end(); it++)
			{
				CTSkillTemp * pTemp = (*it).second;
				CTSkill * pSkill = FindTSkill(pTemp->m_wID);
				if(!pSkill)
				{
					if(pTemp->m_dwClassID & BITSHIFTID(m_bClass) &&
						IsEnoughSkillPoint(pTemp) &&
						(!pTemp->m_wParentSkillID ||
						(pTemp->m_wParentSkillID && FindTSkill(pTemp->m_wParentSkillID))))
						mapSKILL.insert(MAPTSKILLTEMP::value_type(pTemp->m_wID, pTemp));
				}
				else
				{
					if(pSkill->m_pTSKILL->m_bMaxLevel > pSkill->m_bLevel &&
						m_bLevel >= pSkill->GetNextLevel() &&
						IsEnoughSkillPoint(pSkill->m_pTSKILL))
						mapSKILL.insert(MAPTSKILLTEMP::value_type(pTemp->m_wID, pTemp)); 
				}
			}
			(*pMSG)
				<< BYTE(mapSKILL.size());

			for(it=mapSKILL.begin(); it!=mapSKILL.end(); it++)
			{
				(*pMSG)
					<< (*it).second->m_wID;			

				CTSkill * pSkill = FindTSkill((*it).second->m_wID);
				if(pSkill)
					(*pMSG) << (*it).second->GetPrice(_AtlModule.FindTLevel(pSkill->GetNextLevel())->m_dwMoney);		
				else
					(*pMSG) << (*it).second->GetPrice(_AtlModule.FindTLevel((*it).second->m_bStartLevel)->m_dwMoney);			
			}
			mapSKILL.clear();
		}
		break;
	case TNPC_PVPOINT:
	case TNPC_ITEM:
		{
			MAPTITEMTEMP::iterator it;
			MAPTITEMTEMP mapITEM;
			mapITEM.clear();
			for(it=pNpc->m_mapItem.begin(); it!=pNpc->m_mapItem.end(); it++)
			{
				if((*it).second->m_dwClassID & BITSHIFTID(m_bClass) &&
					((*it).second->m_bItemCountry == TCONTRY_N ||
					pNpc->m_bDiscountCondition == DCC_ALLCOUNTRY ||
					(*it).second->m_bItemCountry == m_bCountry))
					mapITEM.insert(MAPTITEMTEMP::value_type((*it).first, (*it).second));
			}

			(*pMSG)
				<< BYTE(mapITEM.size());

			for(it=mapITEM.begin(); it!=mapITEM.end(); it++)
			{
				WORD wItemID = (*it).first;
				DWORD dwPrice = 0;
				if(pNpc->m_bType == TNPC_PVPOINT)
					dwPrice = _AtlModule.GetItemPvPrice((*it).second);
				else
					dwPrice = _AtlModule.GetItemPrice((*it).second);				

				(*pMSG) << wItemID
						<< dwPrice;
			}

			mapITEM.clear();
		}
		break;
	case TNPC_PORTAL:
		{
			MAPTDESTINATION::iterator itDest;
			MAPTDESTINATION mapDest;
			mapDest.clear();
			LPTPORTAL pPortal = pNpc->GetPortal();

			if( pPortal && CheckPortalCondition(pPortal, pPortal->m_bCondition, pPortal->m_dwConditionID))
			{
				for(itDest=pPortal->m_mapDestination.begin(); itDest!=pPortal->m_mapDestination.end(); itDest++)
				{
					BYTE bSuccess = TRUE;
					for(BYTE i = 0; i < PORTALCONDITION_COUNT; i++)
					{
						if(!CheckPortalCondition((*itDest).second->m_pPortal, (*itDest).second->m_bCondition[i], (*itDest).second->m_dwConditionID[i]))
						{
							bSuccess = FALSE;
							break;
						}						
					}
					if(bSuccess)
                        mapDest.insert(MAPTDESTINATION::value_type((*itDest).first, (*itDest).second));
				}
			}

			(*pMSG)
				<< BYTE(mapDest.size());

			for(itDest=mapDest.begin(); itDest!=mapDest.end(); itDest++)
			{
				(*pMSG) << (*itDest).first
						<< (*itDest).second->m_dwPrice;
			}

			mapDest.clear();
		}
		break;
	case TNPC_MONSTER:
		{
			(*pMSG)
                << BYTE(pNpc->m_mapMon.size());

			MAPTMONSTERSHOP::iterator it;
			for(it=pNpc->m_mapMon.begin(); it!=pNpc->m_mapMon.end(); it++)
			{
				LPTMONSTERSHOP pMon = (*it).second;
				(*pMSG)
					<< pMon->m_wID
					<< pMon->m_dwPrice;
			}
		}
		break;
	case TNPC_MAGICITEM:
		{
			MAPTSTORAGEITEM::iterator it;
			MAPTSTORAGEITEM mapMagicITEM;
			mapMagicITEM.clear();
			for(it=pNpc->m_mapMagicItem.begin(); it!=pNpc->m_mapMagicItem.end(); it++)
			{
				if((*it).second->m_pTITEM->m_dwClassID & BITSHIFTID(m_bClass) &&
					((*it).second->m_pTITEM->m_bItemCountry == TCONTRY_N ||
					pNpc->m_bDiscountCondition == DCC_ALLCOUNTRY ||
					(*it).second->m_pTITEM->m_bItemCountry == m_bCountry))
					mapMagicITEM.insert(MAPTSTORAGEITEM::value_type((*it).first, (*it).second));
			}

			(*pMSG)
				<< BYTE(mapMagicITEM.size());

			for(it=mapMagicITEM.begin(); it!=mapMagicITEM.end(); it++)
			{
				DWORD dwQItemID = (*it).first;
				DWORD dwPrice = (*it).second->m_dwMoney;
				__int64 dUseTime = (*it).second->m_dEndTime;

				(*pMSG) << dwQItemID
						<< dwPrice;

				CTime time(_AtlModule.m_timeCurrent);
				time += CTimeSpan(LONG((*it).second->m_dEndTime),0,59,0);
				(*it).second->m_dEndTime = time.GetTime();
				//(*it).second->SetEndTime(DWORD((*it).second->m_dEndTime) * DAY_ONE);
				(*it).second->WrapPacketClient(pMSG, m_dwID, FALSE);
				(*it).second->m_dEndTime = dUseTime;
			}

			mapMagicITEM.clear();
		}
		break;
	}

	Say(pMSG);
}
void CTPlayer::SendCS_ITEMBUY_ACK(BYTE bRet,
								  WORD wItemID)
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(CS_ITEMBUY_ACK)
		<< bRet
		<< wItemID
		<< m_dwGold
		<< m_dwSilver
		<< m_dwCooper;

	Say(pMSG);
}

void CTPlayer::SendCS_MAGICITEMBUY_ACK(BYTE bRet)
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(CS_MAGICITEMBUY_ACK)
		<< bRet
		<< m_dwGold
		<< m_dwSilver
		<< m_dwCooper;

	Say(pMSG);
}

void CTPlayer::SendCS_ITEMSELL_ACK(BYTE bResult)
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(CS_ITEMSELL_ACK)
		<< bResult
		<< m_dwGold
		<< m_dwSilver
		<< m_dwCooper;

	Say(pMSG);
}
void CTPlayer::SendCS_MONITEMTAKE_ACK(BYTE bResult)
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(CS_MONITEMTAKE_ACK)
		<< bResult;

	Say(pMSG);
}
void CTPlayer::SendCS_MONITEMLIST_ACK(BYTE bRet,
									  DWORD dwMonID,
									  DWORD dwMoney,
									  CTInven * pInven,
									  BYTE bUpdate)
{
	CPacket * pMSG = new CPacket();

	DWORD dwGold, dwSilver, dwCooper;
	CalcMoney(dwMoney, dwGold, dwSilver, dwCooper);

	pMSG->SetID(CS_MONITEMLIST_ACK)
		<< bRet
		<< bUpdate
		<< dwMonID
		<< dwGold
		<< dwSilver
		<< dwCooper;

	if(!bRet)
	{
		WORD wSize = pMSG->GetSize();
		BYTE bCount = 0;

		(*pMSG)
			<< bCount;

		MAPTITEM::iterator it;
		for(it=pInven->m_mapTITEM.begin(); it != pInven->m_mapTITEM.end();it++)
		{
			if((*it).second->m_bMaxRouting)
				continue;

			if((*it).second->m_dwOwnerID == 0 || (*it).second->m_dwOwnerID == m_dwID)
			{
				CTItem * pTITEM = (*it).second;
				pTITEM->WrapPacketClient(pMSG, m_dwID);
				bCount++;
			}
		}
		memcpy(pMSG->GetBuffer()+wSize, &bCount, sizeof(bCount));
	}

	Say(pMSG);
}
void CTPlayer::SendCS_MONITEMLOTTERY_ACK(DWORD dwMonID, CTItem *pItem)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_MONITEMLOTTERY_ACK)
		<< dwMonID;

	pItem->WrapPacketClient(pMsg, m_dwID, FALSE);

	Say(pMsg);
}
void CTPlayer::SendCS_FRIENDASK_ACK(CString strInviter)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_FRIENDASK_ACK)
		<< strInviter;

	Say(pMsg);
}
void CTPlayer::SendCS_FRIENDADD_ACK(BYTE bResult,
									DWORD dwFriendID,
									CString strName,
									BYTE bLevel,
									BYTE bGroup,
									BYTE bClass,
									DWORD dwRegion)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_FRIENDADD_ACK)
		<< bResult
		<< dwFriendID
		<< strName
		<< bLevel
		<< bGroup
		<< bClass
		<< dwRegion;

	Say(pMsg);
}
void CTPlayer::SendCS_FRIENDERASE_ACK(BYTE bResult,
                                      DWORD dwFriendID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_FRIENDERASE_ACK)
		<< bResult
		<< dwFriendID;

	Say(pMsg);
}
void CTPlayer::SendCS_FRIENDLIST_ACK(CPacket * pMsg)
{
	pMsg->SetID(CS_FRIENDLIST_ACK);
	Say(pMsg);
}
void CTPlayer::SendCS_FRIENDCONNECTION_ACK(BYTE bResult, 
										   CString strName, 
										   DWORD dwRegion)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_FRIENDCONNECTION_ACK)
		<< bResult
		<< strName
		<< dwRegion;

	Say(pMsg);
}
void CTPlayer::SendCS_FRIENDREGION_ACK(CString strName, 
									   DWORD dwRegion)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_FRIENDREGION_ACK)
		<< strName
		<< dwRegion;

	Say(pMsg);
}
void CTPlayer::SendCS_CHGPARTYCHIEF_ACK(BYTE bRET)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CHGPARTYCHIEF_ACK)
		<< bRET;

	Say(pMsg);
}
void CTPlayer::SendCS_CHGSQUADCHIEF_ACK(WORD wSquadID, 
										DWORD dwChiefID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CHGSQUADCHIEF_ACK)
		<< wSquadID
		<< dwChiefID;

	Say(pMsg);
}
void CTPlayer::SendCS_PARTYATTR_ACK(DWORD dwCharID,
									WORD wPartyID,
									DWORD dwChiefID,
									WORD wCommander)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_PARTYATTR_ACK)
		<< dwCharID
		<< wPartyID
		<< dwChiefID
		<< wCommander;

	Say(pMsg);
}
void CTPlayer::SendCS_GUILDATTR_ACK(DWORD dwCharID,
									DWORD dwGuildID,
									DWORD dwFame,
									DWORD dwFameColor,
									CString strName,
									BYTE bPeer,
									DWORD dwTacticsID, 
									CString strTacticsName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_GUILDATTR_ACK)
		<< dwCharID
		<< dwGuildID
		<< dwFame
		<< dwFameColor
		<< strName
		<< bPeer
		<< dwTacticsID
		<< strTacticsName;

	Say(pMsg);
}

void CTPlayer::SendCS_HOTKEYCHANGE_ACK(BYTE bInven, THOTKEY hotkey[], BYTE bCount)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_HOTKEYCHANGE_ACK)
		<< bInven
		<< bCount;

	for(BYTE i=0; i<bCount; i++)
	{
		(*pMsg)
			<< hotkey[i].m_bPos
			<< hotkey[i].m_bType
			<< hotkey[i].m_wID;
	}

	Say(pMsg);
}
void CTPlayer::SendCS_MONEY_ACK()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_MONEY_ACK)
		<< m_dwGold
		<< m_dwSilver
		<< m_dwCooper;

	Say(pMsg);
}
void CTPlayer::SendCS_NPCITEMLIST_ACK(WORD wID, LPVWORD pvItem)
{
	CPacket * pMsg = new CPacket();
	
	 	
	CTNpc* pNpc = _AtlModule.FindTNpc(wID);
	BYTE bDiscountRate = _AtlModule.GetDiscountRate(this,pNpc);

	pMsg->SetID(CS_NPCITEMLIST_ACK)
		<< wID
		<< BYTE(TNPC_BOX)
		<< bDiscountRate				
		<< BYTE(pvItem->size());

	for(DWORD i=0; i<pvItem->size(); i++)
	{
		(*pMsg) << (*pvItem)[i];
	}

	Say(pMsg);
}
void CTPlayer::SendCS_CHARSTATINFO_ACK(CTPlayer * pPlayer)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CHARSTATINFO_ACK)
		<< pPlayer->m_dwID
		<< (WORD)pPlayer->GetSTR()
		<< (WORD)pPlayer->GetDEX()
		<< (WORD)pPlayer->GetCON()
		<< (WORD)pPlayer->GetINT()
		<< (WORD)pPlayer->GetWIS()
		<< (WORD)pPlayer->GetMEN()
		<< pPlayer->GetMinAP(FALSE)
		<< pPlayer->GetMaxAP(FALSE)
		<< pPlayer->GetDefendPower()
		<< pPlayer->GetMinAP(TRUE)
		<< pPlayer->GetMaxAP(TRUE)
		<< pPlayer->GetAtkSpeed(TAD_PHYSICAL)
		<< pPlayer->GetAtkSpeed(TAD_LONG)
		<< pPlayer->GetAtkSpeed(TAD_MAGIC)
		<< pPlayer->GetAtkSpeedRate(TAD_PHYSICAL)
		<< pPlayer->GetAtkSpeedRate(TAD_LONG)
		<< pPlayer->GetAtkSpeedRate(TAD_MAGIC)
		<< pPlayer->GetAttackLevel()
		<< pPlayer->GetDefendLevel()
		<< pPlayer->GetCriticalPysProb()
		<< pPlayer->GetMinMagicAP()
		<< pPlayer->GetMaxMagicAP()
		<< pPlayer->GetMagicDefPower()
		<< pPlayer->GetMagicAtkLevel()
		<< pPlayer->GetMagicDefLevel()
		<< pPlayer->GetChargeSpeed()
		<< pPlayer->GetChargeProb()
		<< pPlayer->GetCriticalMagicProb()
		<< pPlayer->m_wSkillPoint
		<< pPlayer->m_aftermath.m_bStep;

	Say(pMsg);
}

void CTPlayer::SendCS_CHARSTATINFO_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD));

	pMsg->SetID(CS_CHARSTATINFO_ACK);
	Say(pMsg);
}

void CTPlayer::SendCS_CANCELACTION_ACK(DWORD dwObjID, BYTE bObjType)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CANCELACTION_ACK)
		<< dwObjID
		<< bObjType;

	Say(pMsg);
}

void CTPlayer::SendCS_LOCALENABLE_ACK(WORD wID, BYTE bStatus)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_LOCALENABLE_ACK)
		<< wID
		<< bStatus;

	Say(pMsg);
}

void CTPlayer::SendCS_LOCALOCCUPY_ACK(BYTE bType, WORD wID, BYTE bCountry, DWORD dwGuildID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_LOCALOCCUPY_ACK)
		<< bType
		<< wID
		<< bCountry
		<< dwGuildID;

	Say(pMsg);
}

void CTPlayer::SendCS_CASTLEOCCUPY_ACK(BYTE bType, WORD wID, DWORD dwGuildID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CASTLEOCCUPY_ACK)
		<< bType
		<< wID
		<< dwGuildID;

	Say(pMsg);
}

void CTPlayer::SendCS_ITEMUPGRADE_ACK(BYTE bResult, BYTE bTargetInven, BYTE bTargetItemID, BYTE bGrade, BYTE bGradeEffect, WORD wColor)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_ITEMUPGRADE_ACK)
		<< bResult
		<< bTargetInven
		<< bTargetItemID
		<< bGrade
		<< bGradeEffect
		<< wColor;

	Say(pMsg);
}

void CTPlayer::SendCS_ITEMMAGICGRADE_ACK(BYTE bResult, BYTE bTargetInven, CTItem * pTITEM)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_ITEMMAGICGRADE_ACK)
		<< bResult
		<< bTargetInven
		<< BYTE(pTITEM ? pTITEM->m_bItemID : 0)
		<< BYTE(pTITEM ? pTITEM->m_mapTMAGIC.size() : 0);

	if(pTITEM)
	{
		MAPTMAGIC::iterator itTMAGIC;
		for( itTMAGIC = pTITEM->m_mapTMAGIC.begin(); itTMAGIC != pTITEM->m_mapTMAGIC.end(); itTMAGIC++)
		{
			(*pMsg)
				<< (*itTMAGIC).first
				<< pTITEM->GetMagicValue((*itTMAGIC).second);
		}
	}

	Say(pMsg);
}

void CTPlayer::SendCS_CHGCORPSCOMMANDER_ACK(BYTE bRET)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CHGCORPSCOMMANDER_ACK)
		<< bRET;

	Say(pMsg);
}

void CTPlayer::SendCS_CORPSASK_ACK(CString strAskName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CORPSASK_ACK)
		<< strAskName;

	Say(pMsg);
}
void CTPlayer::SendCS_CORPSREPLY_ACK(BYTE bResult,
									 CString strAskName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CORPSREPLY_ACK)
		<< bResult
		<< strAskName;

	Say(pMsg);
}
void CTPlayer::SendCS_CORPSJOIN_ACK(WORD wCorpsID,
									WORD wCommanderID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CORPSJOIN_ACK)
		<< wCorpsID
		<< wCommanderID;

	Say(pMsg);
}
void CTPlayer::SendCS_ADDSQUAD_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);
	pMsg->SetID(CS_ADDSQUAD_ACK);

	Say(pMsg);
}
void CTPlayer::SendCS_DELSQUAD_ACK(WORD wSquad)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_DELSQUAD_ACK)
		<< wSquad;

	Say(pMsg);
}
void CTPlayer::SendCS_DELCORPSUNIT_ACK(WORD wSquad, DWORD dwTarget)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_DELCORPSUNIT_ACK)
		<< wSquad
		<< dwTarget;

	Say(pMsg);
}
void CTPlayer::SendCS_ADDCORPSUNIT_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);
	pMsg->SetID(CS_ADDCORPSUNIT_ACK);

	Say(pMsg);
}

void CTPlayer::SendCS_CORPSCMD_ACK(WORD wSquadID,
								   DWORD dwCharID,
								   WORD wMapID,
								   BYTE bCMD,
								   DWORD dwTargetID,
								   BYTE bTargetType,
								   WORD wPosX,
								   WORD wPosZ)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CORPSCMD_ACK)
		<< wSquadID
		<< dwCharID
		<< wMapID
		<< bCMD
		<< dwTargetID
		<< bTargetType
		<< wPosX
		<< wPosZ;

	Say(pMsg);
}

void CTPlayer::SendCS_CORPSENEMYLIST_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);
	pMsg->SetID(CS_CORPSENEMYLIST_ACK);

	Say(pMsg);
}

void CTPlayer::SendCS_MOVECORPSENEMY_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);
	pMsg->SetID(CS_MOVECORPSENEMY_ACK);

	Say(pMsg);
}

void CTPlayer::SendCS_MOVECORPSUNIT_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);
	pMsg->SetID(CS_MOVECORPSUNIT_ACK);

	Say(pMsg);
}

void CTPlayer::SendCS_ADDCORPSENEMY_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);
	pMsg->SetID(CS_ADDCORPSENEMY_ACK);

	Say(pMsg);
}

void CTPlayer::SendCS_DELCORPSENEMY_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);
	pMsg->SetID(CS_DELCORPSENEMY_ACK);

	Say(pMsg);
}

void CTPlayer::SendCS_CORPSHP_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);
	pMsg->SetID(CS_CORPSHP_ACK);

	Say(pMsg);
}

void CTPlayer::SendCS_REPORTENEMYLIST_ACK(WORD wSquadID, DWORD dwCharID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_REPORTENEMYLIST_ACK)
		>> wSquadID
		>> dwCharID;

	Say(pMsg);
}

void CTPlayer::SendCS_PARTYMOVE_ACK(BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_PARTYMOVE_ACK)
		<< bResult;

	Say(pMsg);
}

void CTPlayer::SendCS_PROTECTEDLIST_ACK()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_PROTECTEDLIST_ACK)
		<< BYTE(m_mapTPROTECTED.size());

	MAPTPROTECTED::iterator it;
	for(it=m_mapTPROTECTED.begin(); it!=m_mapTPROTECTED.end(); it++)
	{
		(*pMsg)
			<< (*it).first
			<< (*it).second->m_strName
			<< (*it).second->m_bOption;
	}

	Say(pMsg);
}

void CTPlayer::SendCS_PROTECTEDADD_ACK(BYTE bResult,
									   DWORD dwCharID,
									   CString strProtected,
									   BYTE bOption)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_PROTECTEDADD_ACK)
		<< bResult
		<< dwCharID
		<< strProtected
		<< bOption;

	Say(pMsg);
}
void CTPlayer::SendCS_PROTECTEDERASE_ACK(BYTE bResult, 
										 CString strProtected)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_PROTECTEDERASE_ACK)
		<< bResult
		<< strProtected;

	Say(pMsg);
}
void CTPlayer::SendCS_FRIENDGROUPMAKE_ACK(BYTE bRet,
										  BYTE bGroup,
										  CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_FRIENDGROUPMAKE_ACK)
		<< bRet
		<< bGroup
		<< strName;

	Say(pMsg);
}
void CTPlayer::SendCS_FRIENDGROUPDELETE_ACK(BYTE bRet,
											BYTE bGroup)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_FRIENDGROUPDELETE_ACK)
		<< bRet
		<< bGroup;

	Say(pMsg);
}
void CTPlayer::SendCS_FRIENDGROUPCHANGE_ACK(BYTE bRet,
											BYTE bGroup,
											DWORD dwFriend)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_FRIENDGROUPCHANGE_ACK)
		<< bRet
		<< bGroup
		<< dwFriend;

	Say(pMsg);
}
void CTPlayer::SendCS_FRIENDGROUPNAME_ACK(BYTE bRet,
										  BYTE bGroup,
										  CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_FRIENDGROUPNAME_ACK)
		<< bRet
		<< bGroup
		<< strName;

	Say(pMsg);
}
void CTPlayer::SendCS_TMSRECV_ACK(DWORD dwTMS, CString strSender, CString strMessage)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_TMSRECV_ACK)
		<< dwTMS
		<< strSender
		<< strMessage;

	Say(pMsg);
}
void CTPlayer::SendCS_TMSINVITE_ACK(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(DWORD)*2);
	pMsg->SetID(CS_TMSINVITE_ACK);

	Say(pMsg);
}
void CTPlayer::SendCS_TMSOUT_ACK(DWORD dwTMS, CString strMember)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_TMSOUT_ACK)
		<< dwTMS
		<< strMember;

	Say(pMsg);
}
void CTPlayer::SendCS_POSTSEND_ACK(BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_POSTSEND_ACK)
		<< bResult;

	Say(pMsg);
}
void CTPlayer::SendCS_POSTRECV_ACK(DWORD dwPostID,
								   BYTE bRead,
								   CString strSender,
								   CString strTitle,
								   BYTE bType,
								   __int64 ldwTime)
{
	if(IsTutorial())
		return;

	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_POSTRECV_ACK)
		<< dwPostID
		<< bRead
		<< bType
		<< strSender
		<< strTitle
		<< ldwTime;

	m_wPostTotal++;
	m_wPostRead++;

	Say(pMsg);
}
void CTPlayer::SendCS_POSTVIEW_ACK(DWORD dwPostID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_POSTVIEW_ACK)
		<< dwPostID;

	if(!m_pPost || m_pPost->m_dwPostID != dwPostID)
	{
		(*pMsg)
			<< BYTE(0)
			<< NAME_NULL
			<< DWORD(0)
			<< DWORD(0)
			<< DWORD(0)
			<< BYTE(0);
	}
	else
	{
		LPTPOST pPost = m_pPost;

		(*pMsg)
			<< pPost->m_bRead
			<< pPost->m_strMessage
			<< pPost->m_dwGold
			<< pPost->m_dwSilver
			<< pPost->m_dwCooper
			<< BYTE(pPost->m_vItem.size());

		CTItem * pItem;
		for(DWORD i=0; i<pPost->m_vItem.size(); i++)
		{
			pItem = pPost->m_vItem[i];
			pItem->WrapPacketClient(pMsg, m_dwID, FALSE);
		}
	}

	Say(pMsg);
}
void CTPlayer::SendCS_POSTDEL_ACK(DWORD dwPostID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_POSTDEL_ACK)
		<< dwPostID;
	Say(pMsg);
}

void CTPlayer::SendCS_POSTGETITEM_ACK(BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_POSTGETITEM_ACK)
		<< bResult;
	Say(pMsg);
}

void CTPlayer::SendCS_POSTRETURN_ACK(BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_POSTRETURN_ACK)
		<< bResult;
	Say(pMsg);
}

void CTPlayer::SendCS_POSTLIST_ACK(CPacket * pMsg)
{
	pMsg->SetID(CS_POSTLIST_ACK);
	Say(pMsg);
}

void CTPlayer::SendCS_CASTLEENABLE_ACK(WORD wCastle,
									   BYTE bStatus)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CASTLEENABLE_ACK)
		<< wCastle
		<< bStatus;

	Say(pMsg);
}
void CTPlayer::SendCS_CASTLEAPPLY_ACK(BYTE bResult,
									  WORD wCastle,
									  DWORD dwTarget,
									  BYTE bCamp)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CASTLEAPPLY_ACK)
		<< bResult
		<< wCastle
		<< dwTarget
		<< bCamp;

	Say(pMsg);
}
void CTPlayer::SendCS_ITEMUSE_ACK(BYTE bResult,
								  WORD wDelayGroupID,
								  BYTE bKind,
								  DWORD dwTick)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_ITEMUSE_ACK)
		<< bResult
		<< wDelayGroupID
		<< bKind
		<< dwTick;

	Say(pMsg);
}
void CTPlayer::SendCS_MONSETSKILL_ACK(DWORD dwMonID,
									  WORD wSkillID,
									  WORD wAtkRange)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_MONSETSKILL_ACK)
		<< dwMonID
		<< wSkillID
		<< wAtkRange;

	Say(pMsg);
}
void CTPlayer::SendCS_NPCTALK_ACK( DWORD dwQuestID,
								   WORD wNpcID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_NPCTALK_ACK)
		<< dwQuestID
		<< wNpcID;

	Say(pMsg);
}
void CTPlayer::SendCS_CHAPTERMSG_ACK( DWORD dwQuestID )
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CHAPTERMSG_ACK)
		<< dwQuestID;

	Say(pMsg);
}
/////////////////////////////////////////////////////////////////
// 현승룡 공지사항
void CTPlayer::SendCS_ANNOUNCEMENTCHAT_ACK(CString strAnnounce)
{
    CPacket *pMsg = new CPacket();
	pMsg->SetID(CS_ANNOUNCEMENTCHAT_ACK)
		<< strAnnounce;

	Say(pMsg);
}

void CTPlayer::SendCS_BEGINTELEPORT_ACK( BYTE bChannel, WORD wMapID)
{
    CPacket *pMsg = new CPacket();

	pMsg->SetID(CS_BEGINTELEPORT_ACK)
		<< bChannel
		<< wMapID;

	Say(pMsg);
}

void CTPlayer::SendCS_TELEPORT_ACK( BYTE bResult,
								    DWORD dwRange,
								    DWORD dwID,
									BYTE bType,
									WORD wMapID,
									FLOAT fPosX,
									FLOAT fPosY,
									FLOAT fPosZ)
{
    CPacket *pMsg = new CPacket();

	pMsg->SetID(CS_TELEPORT_ACK)
		<< bResult
		<< dwID
		<< bType
		<< dwRange
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ;

	Say(pMsg);
}

void CTPlayer::SendCS_INVENADD_ACK( BYTE bResult,
								    BYTE bInvenID,
									WORD wItemID,
									__time64_t dEndTime)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_INVENADD_ACK)
		<< bResult
		<< bInvenID
		<< wItemID
		<< dEndTime;

	Say(pMsg);
}

void CTPlayer::SendCS_INVENDEL_ACK( BYTE bResult,
								    BYTE bInvenID)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_INVENDEL_ACK)
		<< bResult
		<< bInvenID;

	Say(pMsg);
}

void CTPlayer::SendCS_INVENMOVE_ACK( BYTE bResult,
									 BYTE bSRCInvenID,
									 BYTE bDESTInvenID)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_INVENMOVE_ACK)
		<< bResult
		<< bSRCInvenID
		<< bDESTInvenID;

	Say(pMsg);
}
void CTPlayer::SendCS_GATEADD_ACK(DWORD dwGateID, BYTE bOpened)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_GATEADD_ACK)
		<< dwGateID
		<< bOpened;

	Say(pMsg);
}
void CTPlayer::SendCS_GATEDEL_ACK(DWORD dwGateID)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_GATEDEL_ACK)
		<< dwGateID;

	Say(pMsg);
}
void CTPlayer::SendCS_GATECHANGE_ACK(DWORD dwGateID, BYTE bOpened)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_GATECHANGE_ACK)
		<< dwGateID
		<< bOpened;

	Say(pMsg);
}
void CTPlayer::SendCS_SWITCHADD_ACK(DWORD dwSwitchID, BYTE bOpened)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_SWITCHADD_ACK)
		<< dwSwitchID
		<< bOpened;

	Say(pMsg);
}
void CTPlayer::SendCS_SWITCHDEL_ACK(DWORD dwSwitchID)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_SWITCHDEL_ACK)
		<< dwSwitchID;

	Say(pMsg);
}
void CTPlayer::SendCS_SWITCHCHANGE_ACK(BYTE bResult, DWORD dwSwitchID, BYTE bOpened)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_SWITCHCHANGE_ACK)
		<< bResult
		<< dwSwitchID
		<< bOpened;

	Say(pMsg);
}

void CTPlayer::SendCS_REVIVALASK_ACK(DWORD dwID,
									 BYTE bType,
									 WORD wSkillID,
									 BYTE bLevel)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_REVIVALASK_ACK)
		<< dwID
		<< bType
		<< wSkillID
		<< bLevel;

	Say(pMsg);
}

void CTPlayer::SendCS_REVIVALREPLY_ACK(BYTE bReply,
									   DWORD dwDefender)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_REVIVALREPLY_ACK)
		<< bReply
		<< dwDefender;

	Say(pMsg);
}

void CTPlayer::SendCS_DROPDAMAGE_ACK(DWORD dwID,
									 BYTE bType,
									 DWORD dwDamage)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DROPDAMAGE_ACK)
		<< dwID
		<< bType
		<< dwDamage;

	Say(pMsg);
}
void CTPlayer::SendCS_SETRETURNPOS_ACK(BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_SETRETURNPOS_ACK)
		<< bResult;

	Say(pMsg);
}
void CTPlayer::SendCS_DEALITEMEND_ACK(BYTE bResult, CString strTarget)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DEALITEMEND_ACK)
		<< bResult
		<< strTarget;

	Say(pMsg);
}

void CTPlayer::SendCS_DEALITEMASK_ACK(CString strInviter)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DEALITEMASK_ACK)
		<< strInviter;

	Say(pMsg);
}

void CTPlayer::SendCS_DEALITEMSTART_ACK(CString strTarget)
{
	m_dealItem.m_bStatus = DEAL_START;

	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DEALITEMSTART_ACK)
		<< strTarget;

	Say(pMsg);
}

void CTPlayer::SendCS_DEALITEMADD_ACK(DWORD dwGold,
									  DWORD dwSilver,
									  DWORD dwCooper,
									  LPVTITEM pvItem)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DEALITEMADD_ACK)
		<< dwGold
		<< dwSilver
		<< dwCooper
		<< BYTE(pvItem->size());

	for(DWORD i=0; i<pvItem->size(); i++)
	{
		CTItem * pItem = (*pvItem)[i];
		pItem->WrapPacketClient(pMsg, m_dwID, FALSE);
	}

	Say(pMsg);
}

void CTPlayer::SendCS_STOREOPEN_ACK(BYTE bResult, DWORD dwCharID, CString strName)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_STOREOPEN_ACK)
		<< bResult
		<< dwCharID
		<< strName;

	Say(pMsg);
}

void CTPlayer::SendCS_STORECLOSE_ACK(BYTE bResult, DWORD dwCharID)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_STORECLOSE_ACK)
		<< bResult
		<< dwCharID;

	Say(pMsg);
}

void CTPlayer::SendCS_STOREITEMLIST_ACK(DWORD dwID, CString strName, CTPlayer * pTarget)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_STOREITEMLIST_ACK)
		<< dwID
		<< strName;

	WORD wSize = pMsg->GetSize();
	BYTE bCount = 0;

	(*pMsg)
		<< bCount;

	MAPTSTOREITEM::iterator itSTORE;
	
	for(itSTORE=pTarget->m_mapStoreItem.begin(); itSTORE!=pTarget->m_mapStoreItem.end(); itSTORE++)
	{
		CTInven * pInven = pTarget->FindTInven((*itSTORE).second->m_bInvenID);
		if(pInven)
		{
			CTItem * pItem = pInven->FindTItem((*itSTORE).second->m_bItemID);
			if(pItem)
			{
				(*pMsg)
					<< (*itSTORE).first
					<< (*itSTORE).second->m_dwGold
					<< (*itSTORE).second->m_dwSilver
					<< (*itSTORE).second->m_dwCooper;

				pItem->WrapPacketClient(pMsg, m_dwID, FALSE);
				bCount++;

				if(bCount == 0xFF)
					break;
			}
		}
	}
	memcpy(pMsg->GetBuffer()+wSize, &bCount, sizeof(bCount));

	Say(pMsg);
}

void CTPlayer::SendCS_STOREITEMBUY_ACK(BYTE bResult, WORD wItemID, BYTE bCount)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_STOREITEMBUY_ACK)
		<< bResult
		<< wItemID
		<< bCount;

	Say(pMsg);
}

void CTPlayer::SendCS_STOREITEMSELL_ACK(BYTE bItem, BYTE bCount)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_STOREITEMSELL_ACK)
		<< bItem
		<< bCount;

	Say(pMsg);
}

void CTPlayer::SendCS_PETMAKE_ACK(BYTE bResult, WORD wPetID, CString strName, __int64 ldwTime)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_PETMAKE_ACK)
		<< bResult
		<< wPetID
		<< strName
		<< ldwTime;

	Say(pMsg);
}

void CTPlayer::SendCS_PETDEL_ACK(BYTE bResult, WORD wPetID)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_PETDEL_ACK)
		<< bResult
		<< wPetID;

	Say(pMsg);
}

void CTPlayer::SendCS_PETLIST_ACK()
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_PETLIST_ACK)
		<< BYTE(m_mapTPET.size());

	MAPTPET::iterator it;
	for(it=m_mapTPET.begin(); it!=m_mapTPET.end(); it++)
	{
		(*pMsg)
			<< (*it).second->m_wPetID
			<< (*it).second->m_strName
			<< (*it).second->m_ldwTime;
	}

	Say(pMsg);
}

void CTPlayer::SendCS_PETRECALL_ACK(BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_PETRECALL_ACK)
		<< bResult;

	Say(pMsg);
}

void CTPlayer::SendCS_PETRIDING_ACK(BYTE bResult, DWORD dwCharID, DWORD dwMonID, BYTE bAction)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_PETRIDING_ACK)
		<< bResult
		<< dwCharID
		<< dwMonID
		<< bAction;

	Say(pMsg);
}

void CTPlayer::SendCS_CHGCHANNEL_ACK(BYTE bResult, BYTE bChannel)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_CHGCHANNEL_ACK)
		<< bResult
		<< bChannel;

	Say(pMsg);
}

void CTPlayer::SendCS_TESTMSG_ACK()
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_TESTMSG_ACK)
		<< BYTE(1);

	Say(pMsg);
}

void CTPlayer::SendCS_PARTYITEMTAKE_ACK(DWORD dwCharID, CTItem *pItem)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_PARTYITEMTAKE_ACK)
		<< dwCharID;

	pItem->WrapPacketClient(pMsg, m_dwID, FALSE);

	Say(pMsg);
}

void CTPlayer::SendCS_GETITEM_ACK(CTItem *pTITEM)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_GETITEM_ACK);

	pTITEM->WrapPacketClient(pMsg, m_dwID, FALSE);

	Say(pMsg);
}

void CTPlayer::SendCS_DUELINVITE_ACK(DWORD dwInviter)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DUELINVITE_ACK)
		<< dwInviter;

	Say(pMsg);
}
void CTPlayer::SendCS_DUELSTART_ACK(BYTE bResult, DWORD dwInviter, DWORD dwTarget)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DUELSTART_ACK)
		<< bResult
		<< dwInviter
		<< dwTarget;

	Say(pMsg);
}
void CTPlayer::SendCS_DUELEND_ACK(DWORD dwLoser)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DUELEND_ACK)
		<< dwLoser;

	Say(pMsg);
}
void CTPlayer::SendCS_DUELSTANDBY_ACK(DWORD dwInviter, DWORD dwTarget, float fPosX, float fPosZ)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DUELSTANDBY_ACK)
		<< dwInviter
		<< dwTarget
		<< fPosX
		<< fPosZ;

	Say(pMsg);
}
void CTPlayer::SendCS_GETTARGETANS_ACK(DWORD dwCharID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_GETTARGETANS_ACK)
		<< dwCharID;

	Say(pMsg);
}
void CTPlayer::SendCS_GETTARGET_ACK(DWORD dwTargetID,
									BYTE bTargetType)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_GETTARGET_ACK)
		<< dwTargetID
		<< bTargetType;

	Say(pMsg);
}
void CTPlayer::SendCS_SMSSEND_ACK(BYTE bResult,
                                  BYTE bType,
								  CString strTarget)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_SMSSEND_ACK)
		<< bResult
		<< bType
		<< strTarget;

	Say(pMsg);
}
void CTPlayer::SendCS_SKILLINIT_ACK(BYTE bResult, 
									WORD wSkillID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_SKILLINIT_ACK)
		<< bResult
		<< wSkillID;

	Say(pMsg);
}
void CTPlayer::SendCS_SKILLLIST_ACK(DWORD dwTick)
{
	WORD arPoint[4];
	GetSkillKindPoint(arPoint);

	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_SKILLLIST_ACK)
		<< m_wSkillPoint
		<< arPoint[0]
		<< arPoint[1]
		<< arPoint[2]
		<< arPoint[3]
		<< BYTE(m_mapTSKILL.size());

	MAPTSKILL::iterator it;
	for(it=m_mapTSKILL.begin(); it!=m_mapTSKILL.end(); it++)
	{
		(*pMsg)
			<< (*it).second->m_pTSKILL->m_wID
			<< (*it).second->m_bLevel
			<< (*it).second->GetReuseRemainTick(dwTick);
	}

	Say(pMsg);
}
void CTPlayer::SendCS_SKILLINITPOSSIBLE_ACK(VWORD vSkill)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_SKILLINITPOSSIBLE_ACK)
		<< BYTE(vSkill.size());

	for(BYTE i=0; i<vSkill.size(); i++)
	{
		(*pMsg)
			<< vSkill[i];
	}

	Say(pMsg);
}
void CTPlayer::SendCS_HELMETHIDE_ACK(DWORD dwCharID, BYTE bHide)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_HELMETHIDE_ACK)
		<< dwCharID
		<< bHide;

	Say(pMsg);
}
void CTPlayer::SendCS_PARTYMEMBERRECALL_ACK(BYTE bResult, BYTE bType, CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_PARTYMEMBERRECALL_ACK)
		<< bResult
		<< bType
		<< strName;

	Say(pMsg);
}
void CTPlayer::SendCS_PARTYMEMBERRECALLANS_ACK(CString strName,
											   BYTE bInven,
											   BYTE bItem)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_PARTYMEMBERRECALLANS_ACK)
		<< strName
		<< bInven
		<< bItem;

	Say(pMsg);
}

void CTPlayer::SendCS_CASHITEMGET_ACK(BYTE bResult,
									  DWORD dwCashItemID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CASHITEMGET_ACK)
		<< bResult
		<< dwCashItemID;

	Say(pMsg);
}
void CTPlayer::SendCS_CASHSHOPITEMLIST_ACK(DWORD dwCash,
										   DWORD dwCashBonus,
										   WORD wItemID,
										   VTCASHCATEGORY & cash)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CASHSHOPITEMLIST_ACK)
		<< dwCash
		<< dwCashBonus
		<< BYTE(cash.size());

	for(BYTE cc=0; cc<cash.size(); cc++)
	{
		(*pMsg)
			<< cash[cc]->m_strName;

		WORD wSize = pMsg->GetSize();
		WORD wCount = 0;

		(*pMsg)
			<< wCount;

		for(WORD ci=0; ci<cash[cc]->m_vCashItem.size(); ci++)
		{
			LPTCASHITEM pCashItem = cash[cc]->m_vCashItem[ci];
			if(!pCashItem->m_pItem ||
				(!wItemID && !pCashItem->m_bCanSell) ||
				(pCashItem->m_pItem->m_pTITEM->m_bItemCountry != m_bCountry &&
				pCashItem->m_pItem->m_pTITEM->m_bItemCountry != TCONTRY_N) ||
				(wItemID && wItemID != pCashItem->m_pItem->m_wItemID))
				continue;

			(*pMsg)
				<< pCashItem->m_wID
				<< pCashItem->m_wInfoID
				<< pCashItem->m_dwMoney
				<< pCashItem->m_bKind;

			pCashItem->m_pItem->WrapPacketClient(pMsg, m_dwID, FALSE, pCashItem->m_wUseTime, TRUE);
			wCount++;
		}

		memcpy(pMsg->GetBuffer()+wSize, &wCount, sizeof(wCount));
	}

	Say(pMsg);
}
void CTPlayer::SendCS_CASHITEMBUY_ACK(BYTE bResult, DWORD dwCash, DWORD dwCashBonus)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CASHITEMBUY_ACK)
		<< bResult
		<< dwCash
		<< dwCashBonus;

	Say(pMsg);
}

void CTPlayer::SendCS_CASHITEMPRESENT_ACK(BYTE bResult, DWORD dwCash, DWORD dwCashBonus)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CASHITEMPRESENT_ACK)
		<< bResult
		<< dwCash
		<< dwCashBonus;

	Say(pMsg);
}

void CTPlayer::SendCS_RESETPCBANG_ACK(DWORD dwCharID,
									  BYTE bInPcBang)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_RESETPCBANG_ACK)
		<< dwCharID
		<< bInPcBang;

	Say(pMsg);
}

void CTPlayer::SendCS_SOULMATE_ACK()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_SOULMATE_ACK)
		<< m_dwSoulmate
		<< m_strSoulmate;

	Say(pMsg);
}

void CTPlayer::SendCS_SOULMATESEARCH_ACK(BYTE bResult,
										 DWORD dwTargetID,
										 CString strName,
										 DWORD dwRegion)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_SOULMATESEARCH_ACK)
		<< bResult
		<< dwTargetID
		<< strName
		<< dwRegion
		<< m_dwGold
		<< m_dwSilver
		<< m_dwCooper;

	Say(pMsg);
}

void CTPlayer::SendCS_SOULMATEREGREADY_ACK(BYTE bResult, CString strName, BYTE bNpcInven, BYTE bNpcItem )
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_SOULMATEREGREADY_ACK)
		<< bResult
		<< strName		
		<< bNpcInven
		<< bNpcItem;

	Say(pMsg);
}

void CTPlayer::SendCS_SOULMATEREG_ACK(BYTE bResult, DWORD dwCharID, CString strName, DWORD dwRegion)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_SOULMATEREG_ACK)
		<< bResult
		<< dwCharID
		<< strName
		<< dwRegion
		<< m_dwGold
		<< m_dwSilver
		<< m_dwCooper;

	Say(pMsg);
}

void CTPlayer::SendCS_SOULMATEEND_ACK(BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_SOULMATEEND_ACK)
		<< bResult;

	Say(pMsg);
}

void CTPlayer::SendCS_GAMBLECHECK_ACK(BYTE bResult,
									  BYTE bInven,
									  BYTE bItem,
									  DWORD dwMoney,
									  BYTE bDiscountRate)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_GAMBLECHECK_ACK)
		<< bResult
		<< bInven
		<< bItem
		<< dwMoney
		<< bDiscountRate;

	Say(pMsg);
}

void CTPlayer::SendCS_GAMBLEOPEN_ACK(BYTE bResult,
									 BYTE bInven,
									 BYTE bItem,
									 CTItem * pItem)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_GAMBLEOPEN_ACK)
		<< bResult
		<< m_dwGold
		<< m_dwSilver
		<< m_dwCooper
		<< bInven
		<< bItem;

	if(pItem)
		pItem->WrapPacketClient(pMsg, m_dwID, FALSE);

	Say(pMsg);
}

void CTPlayer::SendCS_ADDGODTOWER_ACK(LPTGODTOWER pGodTower)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_ADDGODTOWER_ACK)
		<< pGodTower->m_wID
		<< pGodTower->m_fPosX
		<< pGodTower->m_fPosY
		<< pGodTower->m_fPosZ;

	if(pGodTower->m_pGodBall)
	{
		(*pMsg)
			<< pGodTower->m_pGodBall->m_wID
			<< pGodTower->m_pGodBall->m_bCamp;
	}
	else
	{
		(*pMsg)
			<< WORD(0)
			<< BYTE(0);
	}

	Say(pMsg);
}

void CTPlayer::SendCS_DELGODTOWER_ACK(WORD wID)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DELGODTOWER_ACK)
		<< wID;

	Say(pMsg);
}

void CTPlayer::SendCS_ADDGODBALL_ACK(LPTGODBALL pGodBall)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_ADDGODBALL_ACK)
		<< pGodBall->m_wID
		<< pGodBall->m_bCamp
		<< pGodBall->m_bGround
		<< pGodBall->m_fPosX
		<< pGodBall->m_fPosY
		<< pGodBall->m_fPosZ;

	Say(pMsg);
}

void CTPlayer::SendCS_DELGODBALL_ACK(WORD wID)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DELGODBALL_ACK)
		<< wID;

	Say(pMsg);
}

void CTPlayer::SendCS_TAKEGODBALL_ACK(DWORD dwCharID,
                                      WORD wGodBallID)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_TAKEGODBALL_ACK)
		<< dwCharID
		<< wGodBallID;

	Say(pMsg);
}

void CTPlayer::SendCS_MOUNTGODBALL_ACK(WORD wGodTower,
									   WORD wGodBall,
									   BYTE bCamp,
                                       DWORD dwCharID)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_MOUNTGODBALL_ACK)
		<< wGodTower
		<< wGodBall
		<< bCamp
		<< dwCharID;

	Say(pMsg);
}
void CTPlayer::SendCS_DEMOUNTGODBALL_ACK(WORD wGodTower,
                                         DWORD dwCharID)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DEMOUNTGODBALL_ACK)
		<< wGodTower
		<< dwCharID;

	Say(pMsg);
}
void CTPlayer::SendCS_BALANCEOFPOWER_ACK(FLOAT fPower_Def,
										 DWORD dwLeftTick,
										 WORD wKillPoint_Atk,
										 WORD wKillPoint_Def,
										 CString strOwner1_Atk,
										 CString strOwner2_Atk,
										 CString strOwner1_Def,
										 CString strOwner2_Def,
										 WORD wKeep1_Atk,
										 WORD wKeep2_Atk,
										 WORD wKeep1_Def,
										 WORD wKeep2_Def)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_BALANCEOFPOWER_ACK)
		<< fPower_Def
		<< dwLeftTick
		<< wKillPoint_Atk
		<< wKillPoint_Def
		<< strOwner1_Atk
		<< strOwner2_Atk
		<< strOwner1_Def
		<< strOwner2_Def
		<< wKeep1_Atk
		<< wKeep2_Atk
		<< wKeep1_Def
		<< wKeep2_Def;

	Say(pMsg);
}
void CTPlayer::SendCS_REMOVEGODBALL_ACK(DWORD dwCharID)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_REMOVEGODBALL_ACK)
		<< dwCharID;

	Say(pMsg);
}

void CTPlayer::SendCS_DURATIONDEC_ACK(BYTE bInven, BYTE bItem, DWORD dwDuration)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DURATIONDEC_ACK)
		<< bInven
		<< bItem
		<< dwDuration;

	Say(pMsg);
}

void CTPlayer::SendCS_DURATIONEND_ACK(BYTE bInven, BYTE bItem, BYTE bDeleted)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DURATIONEND_ACK)
		<< bInven
		<< bItem
		<< bDeleted;

	Say(pMsg);
}
void CTPlayer::SendCS_DURATIONREPCOST_ACK(DWORD dwCost,BYTE bDiscountRate)  
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DURATIONREPCOST_ACK)
		<< dwCost
		<< bDiscountRate;

	Say(pMsg);
}
void CTPlayer::SendCS_DURATIONREP_ACK(BYTE bResult, VTITEM &vItem)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_DURATIONREP_ACK)
		<< bResult
		<< BYTE(vItem.size());

	while(!vItem.empty())
	{
		CTItem * pItem = vItem.back();
		(*pMsg)
			<< pItem->m_bInven
			<< pItem->m_bItemID
			<< pItem->m_dwDuraMax
			<< pItem->m_dwDuraCur;

		vItem.pop_back();
	}

	Say(pMsg);
}
void CTPlayer::SendCS_REFINECOST_ACK(DWORD dwCost,BYTE bDiscountRate)  
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_REFINECOST_ACK)
		<< dwCost
		<< bDiscountRate;

	Say(pMsg);
}

void CTPlayer::SendCS_REFINE_ACK(BYTE bResult, BYTE bInven, CTItem * pItem)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_REFINE_ACK)
		<< bResult
		<< bInven;

	if(pItem)
		pItem->WrapPacketClient(pMsg, m_dwID);

	Say(pMsg);
}

void CTPlayer::SendCS_RESETCOOLTIME_ACK(WORD wExceptSkill)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_RESETCOOLTIME_ACK)
		<< wExceptSkill;

	Say(pMsg);
}

void CTPlayer::SendCS_CASHCABINETBUY_ACK(BYTE bResult,
                                         __int64 ldwTime,
                                         DWORD dwCash,
                                         DWORD dwBonus)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_CASHCABINETBUY_ACK)
		<< bResult
		<< ldwTime
		<< dwCash
		<< dwBonus;

	Say(pMsg);
}

void CTPlayer::SendCS_CASHITEMPUTIN_ACK(BYTE bResult,
										DWORD dwNewID,
										CTItem * pItem,
										DWORD dwCash,
										DWORD dwBonus)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_CASHITEMPUTIN_ACK)
		<< bResult
		<< dwNewID;

	if(!bResult && pItem)
		pItem->WrapPacketClient(pMsg, m_dwID, FALSE);

	Say(pMsg);
}

void CTPlayer::SendCS_CASHCABINETBUYCASH_ACK(DWORD dwCash)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CASHCABINETBUYCASH_ACK)
		<< dwCash;

	Say(pMsg);
}

void CTPlayer::SendCS_CHANGECHARBASE_ACK(BYTE bResult,
										 DWORD dwCharID,
										 BYTE bType,
										 BYTE bValue,
										 CString strName,
										 DWORD dwSecond)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CHANGECHARBASE_ACK)
		<< bResult
		<< dwCharID
		<< bType
		<< bValue
		<< strName
		<< dwSecond;

	Say(pMsg);
}

void CTPlayer::SendCS_STOPTHECLOCK_ACK(BYTE bResult,
									   BYTE bInven,
									   BYTE bItem,
									   __time64_t dEndTime)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_STOPTHECLOCK_ACK)
		<< bResult
		<< bInven
		<< bItem
		<< dEndTime;

	Say(pMsg);
}

void CTPlayer::SendCS_HEROSELECT_ACK(BYTE bResult)   
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CS_HEROSELECT_ACK)
		<< bResult;

	Say(pMsg);
}

void CTPlayer::SendCS_HEROLIST_ACK()  
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(CS_HEROLIST_ACK)
		<< BYTE(_AtlModule.m_vOccupation.size());

	for(DWORD ll=0; ll<_AtlModule.m_vOccupation.size(); ll++)
	{
		LPTLOCAL pLocal = _AtlModule.m_vOccupation[ll];

		(*pMSG)
			<< pLocal->m_wLocal
			<< pLocal->m_strHero;
	}

	(*pMSG)
		<< BYTE(_AtlModule.m_vCastle.size());

	for(DWORD ll=0; ll<_AtlModule.m_vCastle.size(); ll++)
	{
		LPTLOCAL pLocal = _AtlModule.m_vCastle[ll];

		(*pMSG)
			<< pLocal->m_wLocal
			<< pLocal->m_strHero;
	}

	Say(pMSG);
}

void CTPlayer::SendCS_SYSTEMMSG_ACK(BYTE bType, CPacket * pPacket)
{
	if(SM_EVENT_LOTTERY == bType)
	{
		CPacket * pMsg = new CPacket();
		pMsg->SetID(CS_SYSTEMMSG_ACK)
			<< BYTE(SM_EVENT_LOTTERY);
		pMsg->AddData(pPacket);

		Say(pMsg);
	}
}

void CTPlayer::SendCS_SYSTEMMSG_ACK(BYTE bType,
									WORD wLocalID,
									DWORD dwSecond,
									CString strMsg1,
									CString strMsg2,
									BYTE bHour,
									BYTE bMinute,
									CString strPresent,
									BYTE bSelect,
									INT64 dlValue)
{
	if(bType == SM_BATTLE_OPENGATE &&
		(!m_pLocal || m_pLocal->m_wLocal != wLocalID))
		return;

	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_SYSTEMMSG_ACK)
		<< bType;

	switch(bType)
	{
	case SM_CASTLE_NORMAL:
	case SM_BATTLE_NORMAL:
	case SM_CASTLE_START:
	case SM_BATTLE_START:
		break;
	case SM_MISSION_NORMAL:
	case SM_MISSION_START:
		(*pMSG)
			<< wLocalID;	//홀수.짝수 미션
		break;
	case SM_BATTLE_BOSSDIE:
		(*pMSG)
			<< strMsg1
			<< strMsg2
			<< wLocalID; //점령 국가
		break;
	case SM_MISSION_TIMEOUT:
	case SM_MISSION_BOSSDIE:
		(*pMSG)
			<< strMsg1
			<< wLocalID //점령국가
			<< dwSecond; //맵아이디
		break;
	case SM_CASTLE_END:
		(*pMSG)
			<< strMsg1
			<< strMsg2;
		break;
	case SM_BATTLE_OPENGATE:
		(*pMSG)
			<< strMsg1;
		break;
	case SM_CASTLE_START_ALARM:
	case SM_BATTLE_START_ALARM:
	case SM_CASTLE_END_ALARM:
	case SM_BATTLE_END_ALARM:
	case SM_CASTLE_PEACE:
	case SM_BATTLE_PEACE:
		(*pMSG)
            << dwSecond;
		break;
	case SM_MISSION_START_ALARM:
	case SM_MISSION_END_ALARM:
	case SM_MISSION_PEACE:
		(*pMSG)
			<< wLocalID	//홀수.짝수 미션
            << dwSecond;
		break;
	case SM_DUAL_WIN:
		(*pMSG)
			<< strMsg1
			<< strMsg2;
		break;
	case SM_EVENT_QUARTER:
		(*pMSG)
			<< bHour
			<< bMinute
			<< strPresent
			<< bSelect;
		break;
	case SM_CHAT_BAN:
		(*pMSG)
			<< dwSecond;
		break;
	case SM_ITEM_EXPIRE:
		(*pMSG)
			<< dwSecond
			<< bHour
			<< bMinute;
		break;
	case SM_TOURNAMENT_STEP:
		(*pMSG)
			<< bHour			//Group
			<< BYTE(wLocalID)	// Step
			<< dwSecond
			<< dlValue;
		break;
	case SM_TOURNAMENT_COUNTDOWN:
		(*pMSG)
			<< BYTE(wLocalID)
			<< dwSecond;
		break;
	case SM_ARENA_COUNTDOWN:
		(*pMSG)
			<< wLocalID
			<< bHour
			<< bMinute
			<< dwSecond;
		break;
	default:
		delete pMSG;
		return;
	}

	Say(pMSG);
}

void CTPlayer::SendCS_OPENMONEY_ACK(DWORD dwMoney)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(CS_OPENMONEY_ACK)
		<< dwMoney;

	Say(pMSG);
}

void CTPlayer::SendCS_RELAYCONNECT_ACK(DWORD dwIP, WORD wPort)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_RELAYCONNECT_ACK)
		<< dwIP
		<< wPort;

	Say(pMsg);
}

void CTPlayer::SendCS_COMMENT_ACK(DWORD dwCharID,
                                  CString strComment)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_COMMENT_ACK)
		<< dwCharID
		<< strComment;

	Say(pMsg);
}

 
void CTPlayer::SendCS_PVPPOINT_ACK(DWORD dwTotal,
                                   DWORD dwUseable,
                                   BYTE bEvent,
								   DWORD dwMonthPvPoint)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_PVPPOINT_ACK)
		<< dwTotal
		<< dwUseable
		<< bEvent
		<< dwMonthPvPoint;

	Say(pMsg);
}


void CTPlayer::SendCS_GUILDPOINTLOG_ACK(LPPACKETBUF pBUF)
{
	CPacket * pMsg = new CPacket();
	pMsg->CopyData(&pBUF->m_packet, sizeof(DWORD)*2);
	pMsg->SetID(CS_GUILDPOINTLOG_ACK);

	Say(pMsg);
}
void CTPlayer::SendCS_GUILDPOINTREWARD_ACK(BYTE bRet,
										   DWORD dwRemain)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_GUILDPOINTREWARD_ACK)
		<< bRet
		<< dwRemain;

	Say(pMsg);
}
void CTPlayer::SendCS_GUILDPVPRECORD_ACK(LPPACKETBUF pBUF)
{
	CPacket *pMSG = new CPacket();

	pMSG->CopyData(&pBUF->m_packet, sizeof(DWORD)*2);
	pMSG->SetID(CS_GUILDPVPRECORD_ACK);

	Say(pMSG);
}
void CTPlayer::SendCS_PVPRECORD_ACK(BYTE bType)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_PVPRECORD_ACK)
		<< bType;

	if(bType)
	{
		(*pMsg)
			<< DWORD(0)
			<< BYTE(0);

		for(BYTE i=0; i<TCLASS_COUNT; i++)
		{
			(*pMsg)
				<< m_aDuelScore[i][1]
				<< m_aDuelScore[i][0];
		}

		(*pMsg)
			<< BYTE(m_vDuelRecordSet.size());

		for(BYTE r=0; r<m_vDuelRecordSet.size(); r++)
		{
			(*pMsg)
				<< m_vDuelRecordSet[r].m_strName
				<< m_vDuelRecordSet[r].m_bWin
				<< m_vDuelRecordSet[r].m_bClass
				<< m_vDuelRecordSet[r].m_bLevel
				<< m_vDuelRecordSet[r].m_dwPoint
				<< m_vDuelRecordSet[r].m_dTime;
		}
	}
	else
	{
		(*pMsg)
			<< m_dwPvPRankOrder
			<< m_bPvPRankPercent;

		for(BYTE i=0; i<TCLASS_COUNT; i++)
		{
			(*pMsg)
				<< m_aPvPRecord[i][1]
				<< m_aPvPRecord[i][0];
		}

		BYTE bCount = 0;
		WORD wTotal = (WORD)m_vPvPRecent.size();
		if( wTotal > PVP_RECENTRECORDCOUNT)
			bCount = PVP_RECENTRECORDCOUNT;
		else
			bCount = (BYTE)wTotal;

		(*pMsg)
			<< bCount;

		for(WORD r= wTotal - bCount; r < wTotal ; r++)
		{
			(*pMsg)
				<< m_vPvPRecent[r].m_strName
				<< m_vPvPRecent[r].m_bWin
				<< m_vPvPRecent[r].m_bClass
				<< m_vPvPRecent[r].m_bLevel
				<< m_vPvPRecent[r].m_dwPoint
				<< m_vPvPRecent[r].m_dTime;		
		}
		 
		(*pMsg)			
			<< m_dwMonthRankOrder
			<< m_bMonthRankPercent
			<< m_wMonthWin
			<< m_wMonthLose;
	}

	Say(pMsg);
}

void CTPlayer::SendCS_ENDWAR_ACK(BYTE bType,
                                 DWORD dwWinGuildID,
                                 DWORD dwDefTotal,
                                 DWORD dwAtkTotal,
								 CString strDefName,
								 DWORD dwDefPower,
                                 WORD wDefPoint,
                                 WORD wDefKillPoint,
								 CString strAtkName,
                                 DWORD dwAtkPower,
                                 WORD wAtkPoint,
                                 WORD wAtkKillPoint)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_ENDWAR_ACK)
		<< bType
		<< dwWinGuildID
		<< dwDefTotal
		<< dwAtkTotal
		<< strDefName
		<< dwDefPower
		<< wDefPoint
		<< wDefKillPoint
		<< strAtkName
		<< dwAtkPower
		<< wAtkPoint
		<< wAtkKillPoint;

	Say(pMsg);
}

void CTPlayer::SendCS_ENTERCASTLE_ACK(WORD wCastle,
									  BYTE bCamp,
									  CString strName_Atk,
									  CString strName_Def)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_ENTERCASTLE_ACK)
		<< wCastle
		<< bCamp
		<< strName_Atk
		<< strName_Def;

	Say(pMsg);
}

void CTPlayer::SendCS_LEAVECASTLE_ACK()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_LEAVECASTLE_ACK);

	Say(pMsg);
}

void CTPlayer::SendCS_WARP_ACK(DWORD dwObjID,
							   BYTE bObjType,
							   DWORD dwRange,
							   FLOAT fPosX,
							   FLOAT fPosY,
							   FLOAT fPosZ)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_WARP_ACK)
		<< dwObjID
		<< bObjType
		<< dwRange
		<< fPosX
		<< fPosY
		<< fPosZ;

	Say(pMsg);
}

void CTPlayer::SendCS_MONSTERBUY_ACK(BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_MONSTERBUY_ACK)
		<< bResult;

	Say(pMsg);
}

void CTPlayer::SendCS_ITEMCHANGE_ACK(BYTE bResult, WORD wNewID, BYTE bNewCount)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_ITEMCHANGE_ACK)
		<< bResult
		<< wNewID
		<< bNewCount;

	Say(pMsg);
}
void CTPlayer::SendCS_COUNTDOWN_ACK(DWORD dwCommand)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_COUNTDOWN_ACK)
		<< dwCommand;

	Say(pMsg);
}

void CTPlayer::SendCS_CHANGEITEMATTR_ACK(BYTE bInven, CTItem * pItem)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_CHANGEITEMATTR_ACK)
		<< bInven;

	if(pItem)
		pItem->WrapPacketClient(pMsg, m_dwID);

	Say(pMsg);
}

void CTPlayer::SendCS_WASTEREFINE_ACK(BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(CS_WASTEREFINE_ACK)
		<< bResult;

	Say(pMsg);
}

void CTPlayer::SendCS_CASHSHOPSTOP_ACK(BYTE bUse)  
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_CASHSHOPSTOP_ACK)
		<< bUse;

	Say(pMsg);
}

void CTPlayer::SendCS_APEXDATA_ACK(char * BufRecv, int nLen)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_APEXDATA_ACK)
		<< nLen;

	pMsg->AttachBinary((LPVOID)BufRecv, nLen);

	Say(pMsg);
}


void CTPlayer::SendCS_AUCTIONREG_ACK(BYTE bResult,DWORD dwAuctionID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_AUCTIONREG_ACK)
		<< bResult
		<< dwAuctionID;

	Say(pMsg);
}

void CTPlayer::SendCS_AUCTIONINTEREST_ACK(BYTE bResult,BYTE bType)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_AUCTIONINTEREST_ACK)
		<< bResult
		<< bType;

	Say(pMsg);
}

void CTPlayer::SendCS_AUCTIONINTERESTLIST_ACK(BYTE bResult,LPVTAUCTION vTAuction,WORD wPageNum,WORD wStartNum, WORD wEndNum)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_AUCTIONINTERESTLIST_ACK)
		<< bResult;

	if(vTAuction)
	{
		(*pMsg)
			<< (WORD)vTAuction->size()
			<< wPageNum
			<< (WORD)(wEndNum - wStartNum);

		for(WORD i = wStartNum; i < wEndNum; i++)
		{
			vTAuction->at(i)->WrapPacketAuction(pMsg);
			vTAuction->at(i)->m_pItem->WrapPacketClient(pMsg, m_dwID);
		}
	}


	Say(pMsg);
}

void CTPlayer::SendCS_AUCTIONBIDLIST_ACK(BYTE bResult,LPVTAUCTION vTAuction,WORD wPageNum,WORD wStartNum, WORD wEndNum)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_AUCTIONBIDLIST_ACK)
		<< bResult;

	if(vTAuction)
	{
		(*pMsg)
			<< (WORD)vTAuction->size()
			<< wPageNum
			<< (WORD)(wEndNum - wStartNum);

		for(WORD i = wStartNum; i < wEndNum; i++)
		{
			vTAuction->at(i)->WrapPacketAuction(pMsg);
			vTAuction->at(i)->m_pItem->WrapPacketClient(pMsg, m_dwID);
		}
	}


	Say(pMsg);
}

void CTPlayer::SendCS_AUCTIONREGCANCEL_ACK(BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_AUCTIONREGCANCEL_ACK)
		<< bResult;

	Say(pMsg);
}

void CTPlayer::SendCS_AUCTIONBID_ACK(BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_AUCTIONBID_ACK)
		<< bResult;

	Say(pMsg);
}

void CTPlayer::SendCS_AUCTIONBUYDIRECT_ACK(BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_AUCTIONBUYDIRECT_ACK)
		<< bResult;

	Say(pMsg);
}

void CTPlayer::SendCS_AUCTIONFIND_ACK(BYTE bResult,LPVTAUCTION vTAuction,WORD wPageNum,WORD wStartNum, WORD wEndNum)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_AUCTIONFIND_ACK)
		<< bResult;

	if(vTAuction)
	{
		(*pMsg)
			<< (WORD)vTAuction->size()
			<< wPageNum
			<< (WORD)(wEndNum - wStartNum);

		for(WORD i = wStartNum; i < wEndNum; i++)
		{
			vTAuction->at(i)->WrapPacketAuction(pMsg);
			vTAuction->at(i)->m_pItem->WrapPacketClient(pMsg, m_dwID);
		}
	}

	Say(pMsg);
}

void CTPlayer::SendCS_AUCTIONREGLIST_ACK(BYTE bResult,LPVTAUCTION vTAuction,WORD wPageNum,WORD wStartNum, WORD wEndNum)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_AUCTIONREGLIST_ACK)
		<< bResult;

	if(vTAuction)
	{
		(*pMsg)
			<< (WORD)vTAuction->size()
			<< wPageNum
			<< (WORD)(wEndNum - wStartNum);

		for(WORD i = wStartNum; i < wEndNum; i++)
		{
			vTAuction->at(i)->WrapPacketAuction(pMsg);
			vTAuction->at(i)->m_pItem->WrapPacketClient(pMsg, m_dwID);
		}
	}

	Say(pMsg);
}

void CTPlayer::SendCS_KICKOUTMAP_ACK()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_KICKOUTMAP_ACK)
		<< m_wMapID;

	Say(pMsg);
}

void CTPlayer::SendCS_REGGUILDCLOAK_ACK(BYTE bResult,
										BYTE bInven,
										BYTE bItemID,
										BYTE bReg)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_REGGUILDCLOAK_ACK)
		<< bResult
		<< bInven
		<< bItemID
		<< bReg;

	Say(pMsg);
}

void CTPlayer::SendCS_RESETCLOAK_ACK(DWORD dwCharID,
									 BYTE bInven,
									 BYTE bItemID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_RESETCLOAK_ACK)
		<< dwCharID
		<< bInven
		<< bItemID;

	Say(pMsg);
}
 
void CTPlayer::SendCS_FAMERANKLIST_ACK(BYTE bType, MONTHRANKER arFameRank[MONTHCOUNT][FAMERANKCOUNT],BYTE bMonth)
{
	CTime t = CTime::GetCurrentTime();
	WORD wYear = t.GetYear();
	BYTE bCurMonth = t.GetMonth();
	if(bMonth >= bCurMonth)
		wYear--;

	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_FAMERANKLIST_ACK)
		<< bType
		<< wYear
		<< bMonth
		<< BYTE(FAMERANKCOUNT);

	for(BYTE i = 0; i < FAMERANKCOUNT; i++)
		arFameRank[bMonth][i].WrapPacketIn(pMsg);

	Say(pMsg);
}

void CTPlayer::SendCS_UPDATEFAMERANKLIST_ACK(MONTHRANKER arFameRank[FRT_COUNT][MONTHCOUNT][FAMERANKCOUNT],BYTE bMonth)
{
	CTime t = CTime::GetCurrentTime();
	WORD wYear = t.GetYear();
	BYTE bCurMonth = t.GetMonth();
	if(bMonth > bCurMonth)
		wYear--;

	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_UPDATEFAMERANKLIST_ACK)
		<< wYear
		<< bMonth
		<< BYTE(FRT_COUNT);

	for(BYTE f=0; f<FRT_COUNT; f++)
	{
		(*pMsg)
			<< f
			<< BYTE(FAMERANKCOUNT);

		for(BYTE i = 0; i < FAMERANKCOUNT; i++)
			arFameRank[f][bMonth][i].WrapPacketIn(pMsg);
	}

	Say(pMsg);
}

void CTPlayer::SendCS_MONTHRANKLIST_ACK(BYTE bRankMonth,MONTHRANKER arMonthRank[COUNTRY_COUNT][MONTHRANKCOUNT])
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_MONTHRANKLIST_ACK)
		<< bRankMonth
		<< _AtlModule.m_bFirstGroupCount
		<< BYTE(FIRSTGRADEGROUPCOUNT);
	
	for(BYTE i = 0; i < COUNTRY_COUNT; i++)
	{
		(*pMsg)
			<< i;

		for(BYTE j = 0; j < FIRSTGRADEGROUPCOUNT; j++)
			arMonthRank[i][j].WrapPacketIn(pMsg);
	}

	Say(pMsg);
}

void CTPlayer::SendCS_FIRSTGRADEGROUP_ACK(BYTE bRankMonth,MONTHRANKER arMonthRank[COUNTRY_COUNT][FIRSTGRADEGROUPCOUNT])
{
	if(bRankMonth >= MONTHCOUNT || _AtlModule.m_bFirstGroupCount > FIRSTGRADEGROUPCOUNT)
		return;

	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_FIRSTGRADEGROUP_ACK)
		<< bRankMonth
		<< _AtlModule.m_bFirstGroupCount;
	
	for(BYTE i = 0; i < COUNTRY_COUNT; i++)
	{
		(*pMsg)
			<< i;

		for(BYTE j = 0; j < _AtlModule.m_bFirstGroupCount; j++)
			arMonthRank[i][j].WrapPacketIn(pMsg);
	}
	
	Say(pMsg);
}

void CTPlayer::SendCS_AFTERMATH_ACK(DWORD dwCharID, BYTE bStep)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_AFTERMATH_ACK)
		<< dwCharID
		<< bStep;

	Say(pMsg);
}

void CTPlayer::SendCS_CHATBAN_ACK(BYTE bRet)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CS_CHATBAN_ACK)
		<< bRet;

	Say(pMsg);
}

void CTPlayer::SendCS_POSTINFO_ACK()
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CS_POSTINFO_ACK)
		<< m_wPostTotal
		<< m_wPostRead;

	Say(pMsg);
}

void CTPlayer::SendCS_TOURNAMENT_ACK(CPacket * pPacket, WORD wProtocol)
{
	CPacket *pMSG = new CPacket();

	pMSG->CopyData(pPacket, sizeof(DWORD)*2 + sizeof(WORD));

	switch(wProtocol)
	{
	case MW_TOURNAMENTSCHEDULE_REQ:
		pMSG->SetID(CS_TOURNAMENTSCHEDULE_ACK);
		break;
	case MW_TOURNAMENTAPPLYINFO_REQ:
		pMSG->SetID(CS_TOURNAMENTAPPLYINFO_ACK);
		break;
	case MW_TOURNAMENTAPPLY_REQ:
		pMSG->SetID(CS_TOURNAMENTAPPLY_ACK);
		break;
	case MW_TOURNAMENTJOINLIST_REQ:
		pMSG->SetID(CS_TOURNAMENTJOINLIST_ACK);
		break;
	case MW_TOURNAMENTPARTYLIST_REQ:
		pMSG->SetID(CS_TOURNAMENTPARTYLIST_ACK);
		break;
	case MW_TOURNAMENTPARTYADD_REQ:
		pMSG->SetID(CS_TOURNAMENTPARTYADD_ACK);
		break;
	case MW_TOURNAMENTMATCHLIST_REQ:
		pMSG->SetID(CS_TOURNAMENTMATCHLIST_ACK);
		break;
	case MW_TOURNAMENTEVENTLIST_REQ:
		pMSG->SetID(CS_TOURNAMENTEVENTLIST_ACK);
		break;
	case MW_TOURNAMENTEVENTINFO_REQ:
		pMSG->SetID(CS_TOURNAMENTEVENTINFO_ACK);
		break;
	case MW_TOURNAMENTEVENTJOIN_REQ:
		pMSG->SetID(CS_TOURNAMENTEVENTJOIN_ACK);
		break;
	default:
		delete pMSG;
		return;
	}

	Say(pMSG);
}

void CTPlayer::SendCS_TOURNAMENTRESULT_ACK(BYTE bStep,
										   BYTE bRet,
										   DWORD dwWin,
										   DWORD dwLose,
										   DWORD dwBlueHideTick,
										   DWORD dwRedHideTick)
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(CS_TOURNAMENTRESULT_ACK)
		<< bStep
		<< bRet
		<< dwWin
		<< dwLose
		<< dwBlueHideTick
		<< dwRedHideTick;

	Say(pMSG);
}

void CTPlayer::SendCS_TOURNAMENTENTER_ACK(CTMap * pMap)
{
	if(!pMap)
		return;

	CPacket * pMSG = new CPacket();

	pMSG->SetID(CS_TOURNAMENTENTER_ACK)
		<< pMap->m_wDefKillPoint
		<< pMap->m_wAtkKillPoint
		<< BYTE(pMap->m_mapTNMTPlayer.size());

	MAPTOURNAMENTPLAYER::iterator it;
	for(it=pMap->m_mapTNMTPlayer.begin(); it!=pMap->m_mapTNMTPlayer.end(); it++)
	{
		LPTOURNAMENTPLAYER pPlayer = (*it).second;
		(*pMSG)
			<< pPlayer->m_bSlot
			<< pPlayer->m_dwCharID
			<< pPlayer->m_bCountry
			<< pPlayer->m_strName
			<< pPlayer->m_bLevel
			<< pPlayer->m_bClass
			<< pPlayer->m_dwChiefID;
	}

	Say(pMSG);
}

void CTPlayer::SendCS_TOURNAMENTSELECTWINNER_ACK(WORD wBlueValue, WORD wRedValue)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_TOURNAMENTSELECTWINNER_ACK)
		<< BYTE(wBlueValue)
		<< BYTE(wRedValue);

	Say(pMSG);
}

void CTPlayer::SendCS_TOURNAMENTCHEER_ACK(DWORD dwCharID, DWORD dwTargetID)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_TOURNAMENTCHEER_ACK)
		<< dwCharID
		<< dwTargetID;

	Say(pMSG);
}

void CTPlayer::SendCS_HELPMESSAGE_ACK(BYTE bID, CString strMessage)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_HELPMESSAGE_ACK)
		<< bID
		<< strMessage;

	Say(pMSG);
}

void CTPlayer::SendCS_ITEMLEVELREVISION_ACK(BYTE bLevel)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_ITEMLEVELREVISION_ACK)
		<< bLevel;

	Say(pMSG);
}

void CTPlayer::SendCS_RPSSTART_ACK(BYTE bResult)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_RPSSTART_ACK)
		<< bResult
		<< m_dwGold
		<< m_dwSilver
		<< m_dwCooper;

	Say(pMSG);
}
void CTPlayer::SendCS_RPSGAME_ACK(BYTE bWinCount, BYTE bPlayerRPS, BYTE bNpcRPS)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_RPSGAME_ACK)
		<< bWinCount
		<< bPlayerRPS
		<< bNpcRPS;

	Say(pMSG);
}

void CTPlayer::SendCS_WARCOUNTRYBALANCE_ACK(DWORD dwCountD, DWORD dwCountC)
{
	if(dwCountD + dwCountC <= WARBALANCE_BASECOUNT)
		dwCountD = dwCountC = WARBALANCE_BASECOUNT/2;

	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_WARCOUNTRYBALANCE_ACK)
		<< dwCountD
		<< dwCountC;

	Say(pMSG);
}

void CTPlayer::SendCS_MEETINGROOM_ACK(BYTE bType, BYTE bResult, CString strName)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_MEETINGROOM_ACK)
		<< bType
		<< bResult
		<< strName;

	Say(pMSG);
}

void CTPlayer::SendCS_ARENA_ACK(BYTE bResult)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_ARENA_ACK)
		<< bResult;

	Say(pMSG);
}

void CTPlayer::SendCS_ARENATEAM_ACK(WORD wID, BYTE bTeam, VDWORD & vFighter)
{
	CPacket * pMSG = new CPacket();

	BYTE bCount = BYTE(vFighter.size());

	pMSG->SetID(CS_ARENATEAM_ACK)
		<< wID
		<< bTeam
		<< bCount;

	for(BYTE i=0; i<bCount; i++)
		(*pMSG) << vFighter[i];

	Say(pMSG);
}

void CTPlayer::SendCS_CMGIFT_ACK(BYTE bRet)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_CMGIFT_ACK)
		<< bRet;

	Say(pMSG);
}

#ifdef __N_PROTECT

void CTPlayer::SendCS_NPROTECT_ACK(BYTE bServerID, DWORD dwTick)
{
	m_dwNPSendTime = dwTick;
	m_bSend = TRUE;

	CPacket * pMsg = new CPacket();
	pMsg->SetID(CS_NPROTECT_ACK)
		<< bServerID;

	pMsg->AttachBinary(&m_csNProtect->m_AuthQuery, sizeof(GG_AUTH_DATA));

	Say(pMsg);
}
#endif