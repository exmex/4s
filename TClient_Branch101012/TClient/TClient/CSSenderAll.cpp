#include "stdafx.h"
#include "TClientWnd.h"


void CTClientGame::SendCS_CONREADY_REQ(BYTE bSvrID)
{
	CPacket packet;

	packet.SetID(CS_CONREADY_REQ);
	SayToSvr(packet, bSvrID);
}

void CTClientGame::SendCS_REVIVAL_REQ( FLOAT fPosX,
									   FLOAT fPosY,
									   FLOAT fPosZ,
									   BYTE bType)
{
	CPacket packet;

	packet.SetID(CS_REVIVAL_REQ)
		<< fPosX
		<< fPosY
		<< fPosZ
		<< bType;

	SayToAll(packet);
}

void CTClientGame::SendCS_BLOCK_REQ( DWORD dwObjID,
									 BYTE bObjType,
									 BYTE bChannel,
									 WORD wMapID,
									 FLOAT fPosX,
									 FLOAT fPosY,
									 FLOAT fPosZ,
									 WORD wPITCH,
									 WORD wDIR,
									 BYTE bAction,
									 BYTE bBlock)
{
	CPacket packet;

	packet.SetID(CS_BLOCK_REQ)
		<< dwObjID
		<< bObjType
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wPITCH
		<< wDIR
		<< bAction
		<< bBlock;

	SayToAll(packet);
}

void CTClientGame::SendCS_MOVE_REQ( WORD wMapID,
								    FLOAT fPosX,
								    FLOAT fPosY,
									FLOAT fPosZ,
									WORD wPITCH,
									WORD wDIR,
									BYTE bMouseDIR,
									BYTE bKeyDIR,
									BYTE bAction,
									BYTE bGhost)
{
	CPacket packet;

	packet.SetID(CS_MOVE_REQ)
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wPITCH
		<< wDIR
		<< bMouseDIR
		<< bKeyDIR
		<< bAction
		<< bGhost;

	SayToAll(packet);
}

void CTClientGame::SendCS_MONMOVE_REQ( CTClientObjBase *pTMON,
									   BYTE bChannel,
									   WORD wMapID,
									   FLOAT fPosX,
									   FLOAT fPosY,
									   FLOAT fPosZ,
									   WORD wPITCH,
									   WORD wDIR,
									   BYTE bMouseDIR,
									   BYTE bKeyDIR,
									   BYTE bAction)
{
	CPacket packet;

	if( GetPartyID(m_pMainChar) && IsHostingEnemy(pTMON) )
		pTMON->m_bTCORPSMOVE = TCORPSMOVE_ENEMY;

	packet.SetID(CS_MONMOVE_REQ)
		<< WORD(1)
		<< pTMON->m_dwID
		<< pTMON->m_bType
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wPITCH
		<< wDIR
		<< bMouseDIR
		<< bKeyDIR
		<< bAction;

	SayToAll(packet);
}

void CTClientGame::SendCS_MONMOVE_REQ( LPVTMONMOVE pTMONMOVE)
{
	BYTE bPARTY = GetPartyID(m_pMainChar) ? TRUE : FALSE;
	CPacket packet;

	packet.SetID(CS_MONMOVE_REQ)
		<< WORD(pTMONMOVE->size());

	for( WORD i=0; i<WORD(pTMONMOVE->size()); i++)
	{
		if( bPARTY && IsHostingEnemy((*pTMONMOVE)[i]->m_pTMON) )
			(*pTMONMOVE)[i]->m_pTMON->m_bTCORPSMOVE = TCORPSMOVE_ENEMY;

		packet
			<< (*pTMONMOVE)[i]->m_pTMON->m_dwID
			<< (*pTMONMOVE)[i]->m_pTMON->m_bType
			<< (*pTMONMOVE)[i]->m_bChannel
			<< (*pTMONMOVE)[i]->m_wMapID
			<< (*pTMONMOVE)[i]->m_fPosX
			<< (*pTMONMOVE)[i]->m_fPosY
			<< (*pTMONMOVE)[i]->m_fPosZ
			<< (*pTMONMOVE)[i]->m_wPITCH
			<< (*pTMONMOVE)[i]->m_wDIR
			<< (*pTMONMOVE)[i]->m_bMouseDIR
			<< (*pTMONMOVE)[i]->m_bKeyDIR
			<< (*pTMONMOVE)[i]->m_bAction;
	}

	SayToAll(packet);
}

void CTClientGame::SendCS_ENTERLB_REQ( DWORD dwHostID,
									   DWORD dwTargetID,
									   BYTE bTargetType,
									   DWORD dwMonID,
									   BYTE bChannel,
									   WORD wMapID)
{
	CPacket packet;

	packet.SetID(CS_ENTERLB_REQ)
		<< dwHostID
		<< dwTargetID
		<< bTargetType
		<< dwMonID
		<< bChannel
		<< wMapID;

	SayToAll(packet);
}

void CTClientGame::SendCS_ENTERAB_REQ( DWORD dwHostID,
									   DWORD dwTargetID,
									   BYTE bTargetType,
									   DWORD dwMonID,
									   BYTE bChannel,
									   WORD wMapID)
{
	CPacket packet;

	packet.SetID(CS_ENTERAB_REQ)
		<< dwHostID
		<< dwTargetID
		<< bTargetType
		<< dwMonID
		<< bChannel
		<< wMapID;

	SayToAll(packet);
}

void CTClientGame::SendCS_LEAVELB_REQ( DWORD dwHostID,
									   DWORD dwTargetID,
									   BYTE bTargetType,
									   DWORD dwMonID,
									   BYTE bChannel,
									   WORD wMapID)
{
	CPacket packet;

	packet.SetID(CS_LEAVELB_REQ)
		<< dwHostID
		<< dwTargetID
		<< bTargetType
		<< dwMonID
		<< bChannel
		<< wMapID;

	SayToAll(packet);
}

void CTClientGame::SendCS_LEAVEAB_REQ( DWORD dwHostID,
									   DWORD dwTargetID,
									   BYTE bTargetType,
									   DWORD dwMonID,
									   BYTE bChannel,
									   WORD wMapID)
{
	CPacket packet;

	packet.SetID(CS_LEAVEAB_REQ)
		<< dwHostID
		<< dwTargetID
		<< bTargetType
		<< dwMonID
		<< bChannel
		<< wMapID;

	SayToAll(packet);
}

void CTClientGame::SendCS_DEFEND_REQ( DWORD dwHostID,
									  DWORD dwAttackID,
									  DWORD dwTargetID,
									  BYTE bAttackType,
									  BYTE bTargetType,
									  WORD wPartyID,
									  DWORD dwActID,
									  DWORD dwAniID,
									  BYTE bChannel,
									  WORD wMapID,
									  BYTE bAtkLevel,
									  DWORD dwMinAP,
									  DWORD dwMaxAP,
									  DWORD dwMinMP,
									  DWORD dwMaxMP,
									  WORD wSTR,
									  WORD wINT,
									  BYTE bCURSE,
									  BYTE bSpecial,
									  BYTE bAglow,
									  BYTE bCountryID,
									  WORD wAL,
									  BYTE bHIT,
									  WORD wSkillID,
									  BYTE bLevel,
									  FLOAT fAtkPosX,
									  FLOAT fAtkPosY,
									  FLOAT fAtkPosZ,
									  FLOAT fDefPosX,
									  FLOAT fDefPosY,
									  FLOAT fDefPosZ,
									  DWORD dwTick)
{
	CPacket packet;

	packet.SetID(CS_DEFEND_REQ)
		<< dwHostID
		<< dwAttackID
		<< dwTargetID
		<< bAttackType
		<< bTargetType
		<< wPartyID
		<< dwActID
		<< dwAniID
		<< bChannel
		<< wMapID
		<< bAtkLevel
		<< dwMinAP
		<< dwMaxAP
		<< dwMinMP
		<< dwMaxMP
		<< wSTR
		<< wINT
		<< bCURSE
		<< bSpecial
		<< bAglow
		<< bCountryID
		<< wAL
		<< bHIT
		<< wSkillID
		<< bLevel
		<< fAtkPosX
		<< fAtkPosY
		<< fAtkPosZ
		<< fDefPosX
		<< fDefPosY
		<< fDefPosZ
		<< dwTick;

	SayToAll(packet);
}

void CTClientGame::SendCS_ACTION_REQ( DWORD dwObjID,
									  BYTE bObjType,
									  BYTE bActionID,
									  DWORD dwActID,
									  DWORD dwAniID,
									  BYTE bChannel,
									  WORD wMapID,
									  WORD wSkillID)
{
	CPacket packet;

	packet.SetID(CS_ACTION_REQ)
		<< dwObjID
		<< bObjType
		<< bActionID
		<< dwActID
		<< dwAniID
		<< bChannel
		<< wMapID
		<< wSkillID;

	SayToAll(packet);
}

void CTClientGame::SendCS_MONATTACK_REQ( DWORD dwAttackID,
										 DWORD dwTargetID,
										 BYTE bAttackType,
										 BYTE bTargetType,
										 DWORD dwActID,
										 DWORD dwAniID,
										 BYTE bChannel,
										 WORD wMapID)
{
	CPacket packet;

	packet.SetID(CS_MONATTACK_REQ)
		<< dwAttackID
		<< dwTargetID
		<< bAttackType
		<< bTargetType
		<< dwActID
		<< dwAniID
		<< bChannel
		<< wMapID;

	SayToAll(packet);
}

void CTClientGame::SendCS_JUMP_REQ( DWORD dwObjID,
								    BYTE bObjType,
									BYTE bChannel,
									WORD wMapID,
								    FLOAT fPosX,
								    FLOAT fPosY,
									FLOAT fPosZ,
									WORD wPITCH,
									WORD wDIR,
									BYTE bAction)
{
	CPacket packet;

	packet.SetID(CS_JUMP_REQ)
		<< dwObjID
		<< bObjType
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wPITCH
		<< wDIR
		<< bAction;

	SayToAll(packet);
}

void CTClientGame::SendCS_MONITEMLIST_REQ( BYTE bOpen,
										   DWORD dwMonID)
{
	CPacket packet;

	packet.SetID(CS_MONITEMLIST_REQ)
		<< bOpen
		<< dwMonID;

	SayToAll(packet);
}

void CTClientGame::SendCS_MONMONEYTAKE_REQ( CTClientObjBase* pTARGET)
{
	if( !pTARGET )
		return ;

	CPacket packet;

	packet.SetID(CS_MONMONEYTAKE_REQ)
		<< pTARGET->m_dwID;

	SayToAll(packet);
}

void CTClientGame::SendCS_MONITEMTAKE_REQ( DWORD dwMonID,
										   BYTE bMonSlotID,
										   BYTE bInvenID,
										   BYTE bSlotID)
{
	CPacket packet;

	packet.SetID(CS_MONITEMTAKE_REQ)
		<< dwMonID
		<< bMonSlotID
		<< bInvenID
		<< bSlotID;

	SayToAll(packet);
}

void CTClientGame::SendCS_MONITEMLOTTERY_REQ( BOOL bJoinLottery,
											  DWORD dwMonID,
											  WORD wItemID)
{
	BYTE bAnswer = bJoinLottery? 1: 0;

	CPacket packet;

	packet.SetID(CS_MONITEMLOTTERY_REQ)
		<< bAnswer
		<< dwMonID
		<< wItemID;

	SayToAll(packet);
}

void CTClientGame::SendCS_MONITEMTAKEALL_REQ( CTClientObjBase* pTARGET )
{
	if( !pTARGET )
		return ;

	CPacket packet;

	packet.SetID(CS_MONITEMTAKEALL_REQ)
		<< pTARGET->m_dwID;

	SayToAll(packet);
}

void CTClientGame::SendCS_SKILLUSE_REQ( CTClientObjBase *pATTACK,
									    CTClientObjBase *pDEFEND,
										LPD3DXVECTOR3 pGROUND,
										LPTSKILL pTSKILL,
										BOOL bTimerON,
										LPTSKILLTARGET_PARAM pPARAM)
{
	D3DXVECTOR3 vTGROUND = pATTACK->GetPosition();
	CPacket packet;

	if(pTSKILL->m_wItemID[pATTACK->GetSkillItemType(pTSKILL)])
		pATTACK->m_bShoot = TRUE;
	else
		pATTACK->m_bShoot = FALSE;

	BYTE bCount = min( 0xFF, INT(pPARAM->m_vTARGET.size()) + INT(pPARAM->m_vTBOUND.size()));
	BYTE bActionID = pATTACK->GetSkillActivateID(pTSKILL);

	TACTION vActionID = pATTACK->FindActionID(
		bActionID,
		pATTACK->GetWeaponID(MT_BATTLE));

	switch(pTSKILL->m_bRangeType)
	{
	case TSKILLRANGE_MFORWARD	:
		{
			D3DXMATRIX& vMAT = pATTACK->GetPositionMat();
			D3DXVECTOR3 vDIR( 0.0f, 0.0f, -1.0f);

			CTMath::Transform(
				&vMAT,
				&vDIR);

			vDIR -= vTGROUND;
			vDIR.y = 0.0f;

			vTGROUND = GetWarpTARGET(
				pATTACK,
				&vDIR,
				pTSKILL->m_fMoveDist);
		}

		break;

	case TSKILLRANGE_TBACKWARD	:
		if(pDEFEND)
		{
			D3DXVECTOR3 vDIR = pDEFEND->GetPosition() - vTGROUND;
			vDIR.y = 0.0f;

			vTGROUND = GetWarpTARGET(
				pDEFEND,
				&vDIR,
				pTSKILL->m_fMoveDist);
		}

		break;

	case TSKILLRANGE_TNEAR		:
		if(pDEFEND)
		{
			D3DXVECTOR3 vTARGET = pDEFEND->GetPosition();
			D3DXVECTOR3 vDIR = vTGROUND - vTARGET;
			vDIR.y = 0.0f;

			FLOAT fLength = D3DXVec3Length(&vDIR);
			if( fLength != 0.0f )
			{
				vTGROUND = vTARGET + (pATTACK->m_fRadius + pDEFEND->m_fRadius) / fLength * vDIR;
				vTARGET = vTGROUND;
				vTARGET.y += pATTACK->m_fSizeY;

				vTGROUND.y = m_vMAP.GetHeight(
					pATTACK,
					NULL,
					&vTARGET,
					0.0f,
					FALSE);
			}
		}

		break;
	}

	if( !pPARAM->m_bRANGE && !pPARAM->m_bPOINT )
		pGROUND = &vTGROUND;

	packet.SetID(CS_SKILLUSE_REQ)
		<< pATTACK->m_dwID
		<< pATTACK->m_bType
		<< m_pMainWnd->m_bChannel
		<< m_vMAP.m_wMapID
		<< pTSKILL->m_wSkillID
		<< bActionID
		<< vActionID.m_dwActID
		<< vActionID.m_dwAniID
		<< pGROUND->x
		<< pGROUND->y
		<< pGROUND->z
		<< bCount;

	for( BYTE i=0; i<bCount && i<BYTE(pPARAM->m_vTARGET.size()); i++)
	{
		packet
			<< pPARAM->m_vTARGET[i]->m_dwID
			<< pPARAM->m_vTARGET[i]->m_bType
			<< BYTE(TRUE);
	}

	bCount = bCount > BYTE(pPARAM->m_vTARGET.size()) ? bCount - BYTE(pPARAM->m_vTARGET.size()) : 0;
	for( i=0; i<bCount; i++)
	{
		packet
			<< pPARAM->m_vTBOUND[i]->m_dwID
			<< pPARAM->m_vTBOUND[i]->m_bType
			<< BYTE(FALSE);
	}

	if(bTimerON)
	{
		CTClientSkill *pTCSKILL = pATTACK->FindTSkill(pTSKILL->m_wSkillID);

		if(pTCSKILL)
			EnableSkillTimer( pATTACK, pTCSKILL, TRUE);
	}

	SayToAll(packet);
}

void CTClientGame::SendCS_LOOPSKILL_REQ( CTClientObjBase *pATTACK,
										 CTClientObjBase *pDEFEND,
										 LPD3DXVECTOR3 pGROUND,
										 LPTSKILL pTSKILL)
{
	BYTE bRANGE = pTSKILL->m_bRangeType == TSKILLRANGE_CIRCLE || pTSKILL->m_bRangeType == TSKILLRANGE_MCIRCLE ? TRUE : FALSE;
	BYTE bPOINT = pTSKILL->m_bRangeType == TSKILLRANGE_POINT ? TRUE : FALSE;

	if( pTSKILL->m_bRangeType == TSKILLRANGE_MCIRCLE && pGROUND )
	{
		(*pGROUND) = D3DXVECTOR3(
			pATTACK->GetPositionX(),
			pATTACK->GetPositionY(),
			pATTACK->GetPositionZ());
	}

	if((!bRANGE && !pDEFEND) || ((bRANGE || bPOINT) && !pGROUND))
	{
		if(IsHostingOBJ( pATTACK->m_dwID, pATTACK->m_bType))
		{
			SendCS_ACTION_REQ(
				pATTACK->m_dwID,
				pATTACK->m_bType,
				0, 0, 0,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID, 0);
		}

		if( pATTACK == m_pMainChar )
		{
			SkillMSG( m_pMainChar, SKILL_NOTARGET);
			if(m_bCHARGE)
				CancelCharge(TRUE);
		}

		return;
	}

	if(!pATTACK->CheckTransMode(pTSKILL))
	{
		if(IsHostingOBJ( pATTACK->m_dwID, pATTACK->m_bType))
		{
			SendCS_ACTION_REQ(
				pATTACK->m_dwID,
				pATTACK->m_bType,
				0, 0, 0,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID, 0);
		}

		if( pATTACK == m_pMainChar )
		{
			SkillMSG( m_pMainChar, SKILL_TRANS);
			if(m_bCHARGE)
				CancelCharge(TRUE);
		}

		return;
	}

	D3DXVECTOR2 vTPOS = bRANGE || bPOINT ? D3DXVECTOR2(
		pGROUND->x,
		pGROUND->z) : D3DXVECTOR2(
		pDEFEND->GetPositionX(),
		pDEFEND->GetPositionZ());
	CPacket packet;

	MAPMONSTER::iterator itMON;
	MAPPLAYER::iterator itPC;
	MAPRECALL::iterator itRECALL;

	VTOBJBASE vTBOUND;
	VTOBJBASE vTARGET;

	vTBOUND.clear();
	vTARGET.clear();

	if(pDEFEND)
	{
		BYTE bKIND = GetTargetKind(
			pATTACK,
			pDEFEND,
			pDEFEND,
			pTSKILL,
			&vTPOS);

		switch(bKIND)
		{
		case TARGETKIND_TARGET	: vTARGET.push_back(pDEFEND); break;
		case TARGETKIND_BOUND	: vTBOUND.push_back(pDEFEND); break;
		}
	}

	for( itRECALL = m_mapFIXRECALL.begin(); itRECALL != m_mapFIXRECALL.end(); itRECALL++)
		if( pDEFEND != (*itRECALL).second )
		{
			BYTE bKIND = GetTargetKind(
				pATTACK,
				pDEFEND,
				(*itRECALL).second,
				pTSKILL,
				&vTPOS);

			switch(bKIND)
			{
			case TARGETKIND_TARGET	: vTARGET.push_back((*itRECALL).second); break;
			case TARGETKIND_BOUND	: vTBOUND.push_back((*itRECALL).second); break;
			}
		}

	for( itRECALL = m_mapRECALL.begin(); itRECALL != m_mapRECALL.end(); itRECALL++)
		if( pDEFEND != (*itRECALL).second )
		{
			BYTE bKIND = GetTargetKind(
				pATTACK,
				pDEFEND,
				(*itRECALL).second,
				pTSKILL,
				&vTPOS);

			switch(bKIND)
			{
			case TARGETKIND_TARGET	: vTARGET.push_back((*itRECALL).second); break;
			case TARGETKIND_BOUND	: vTBOUND.push_back((*itRECALL).second); break;
			}
		}

	for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
		if( pDEFEND != (*itMON).second )
		{
			BYTE bKIND = GetTargetKind(
				pATTACK,
				pDEFEND,
				(*itMON).second,
				pTSKILL,
				&vTPOS);

			switch(bKIND)
			{
			case TARGETKIND_TARGET	: vTARGET.push_back((*itMON).second); break;
			case TARGETKIND_BOUND	: vTBOUND.push_back((*itMON).second); break;
			}
		}

	for( itPC = m_mapPLAYER.begin(); itPC != m_mapPLAYER.end(); itPC++)
		if( pDEFEND != (*itPC).second )
		{
			BYTE bKIND = GetTargetKind(
				pATTACK,
				pDEFEND,
				(*itPC).second,
				pTSKILL,
				&vTPOS);

			switch(bKIND)
			{
			case TARGETKIND_TARGET	: vTARGET.push_back((*itPC).second); break;
			case TARGETKIND_BOUND	: vTBOUND.push_back((*itPC).second); break;
			}
		}

	if( pDEFEND != m_pMainChar )
	{
		BYTE bKIND = GetTargetKind(
			pATTACK,
			pDEFEND,
			m_pMainChar,
			pTSKILL,
			&vTPOS);

		switch(bKIND)
		{
		case TARGETKIND_TARGET	: vTARGET.push_back(m_pMainChar); break;
		case TARGETKIND_BOUND	: vTBOUND.push_back(m_pMainChar); break;
		}
	}

	if( vTARGET.empty() && (!bRANGE || pTSKILL->m_bRangeType == TSKILLRANGE_MCIRCLE) )
	{
		if(IsHostingOBJ( pATTACK->m_dwID, pATTACK->m_bType))
		{
			SendCS_ACTION_REQ(
				pATTACK->m_dwID,
				pATTACK->m_bType,
				0, 0, 0,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID, 0);
		}

		if( pATTACK == m_pMainChar )
		{
			SkillMSG( m_pMainChar, SKILL_NOTARGET);
			if(m_bCHARGE)
				CancelCharge(TRUE);
		}

		return;
	}

	if(pTSKILL->m_wItemID[pATTACK->GetSkillItemType(pTSKILL)])
		pATTACK->m_bShoot = TRUE;
	else
		pATTACK->m_bShoot = FALSE;
	BYTE bCount = min( 0xFF, INT(vTARGET.size()) + INT(vTBOUND.size()));

	packet.SetID(CS_LOOPSKILL_REQ)
		<< pATTACK->m_dwID
		<< pATTACK->m_bType
		<< m_pMainWnd->m_bChannel
		<< m_vMAP.m_wMapID
		<< pTSKILL->m_wSkillID
		<< FLOAT(bRANGE || bPOINT ? pGROUND->x : 0.0f)
		<< FLOAT(bRANGE || bPOINT ? pGROUND->y : 0.0f)
		<< FLOAT(bRANGE || bPOINT ? pGROUND->z : 0.0f)
		<< bCount;

	for( BYTE i=0; i<bCount && i<BYTE(vTARGET.size()); i++)
	{
		packet
			<< vTARGET[i]->m_dwID
			<< vTARGET[i]->m_bType
			<< BYTE(TRUE);
	}

	bCount = bCount > BYTE(vTARGET.size()) ? bCount - BYTE(vTARGET.size()) : 0;
	for( i=0; i<bCount; i++)
	{
		packet
			<< vTBOUND[i]->m_dwID
			<< vTBOUND[i]->m_bType
			<< BYTE(FALSE);
	}

	vTARGET.clear();
	vTBOUND.clear();

	SayToAll(packet);
}

void CTClientGame::SendCS_ITEMUSE_REQ( LPD3DXVECTOR2 pPOS,
									   WORD wTempID,
									   BYTE bInvenID,
									   BYTE bSlotID,
									   WORD wDelayGroupID)
{
	MAPMONSTER::iterator itMON;
	MAPPLAYER::iterator itPC;
	MAPRECALL::iterator itRECALL;

	VTOBJBASE vTBOUND;
	vTBOUND.clear();

	CPacket packet;

	for( itRECALL = m_mapFIXRECALL.begin(); itRECALL != m_mapFIXRECALL.end(); itRECALL++)
	{
		FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
			(*itRECALL).second->GetPositionX() - pPOS->x,
			(*itRECALL).second->GetPositionZ() - pPOS->y));

		if( fDist < TARGET_BOUND )
			vTBOUND.push_back((*itRECALL).second);
	}

	for( itRECALL = m_mapRECALL.begin(); itRECALL != m_mapRECALL.end(); itRECALL++)
	{
		FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
			(*itRECALL).second->GetPositionX() - pPOS->x,
			(*itRECALL).second->GetPositionZ() - pPOS->y));

		if( fDist < TARGET_BOUND )
			vTBOUND.push_back((*itRECALL).second);
	}

	for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
	{
		FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
			(*itMON).second->GetPositionX() - pPOS->x,
			(*itMON).second->GetPositionZ() - pPOS->y));

		if( fDist < TARGET_BOUND )
			vTBOUND.push_back((*itMON).second);
	}

	for( itPC = m_mapPLAYER.begin(); itPC != m_mapPLAYER.end(); itPC++)
	{
		FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
			(*itPC).second->GetPositionX() - pPOS->x,
			(*itPC).second->GetPositionZ() - pPOS->y));

		if( fDist < TARGET_BOUND )
			vTBOUND.push_back((*itPC).second);
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pMainChar->GetPositionX() - pPOS->x,
		m_pMainChar->GetPositionZ() - pPOS->y));

	if( fDist < TARGET_BOUND )
		vTBOUND.push_back(m_pMainChar);

	BYTE bCount = min( 0xFF, INT(vTBOUND.size()));
	packet.SetID(CS_ITEMUSE_REQ)
		<< wTempID
		<< bInvenID
		<< bSlotID
		<< wDelayGroupID
		<< bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		packet
			<< vTBOUND[i]->m_dwID
			<< vTBOUND[i]->m_bType;
	}
	vTBOUND.clear();

	SayToAll(packet);
}

void CTClientGame::SendCS_REGION_REQ( DWORD dwID,
									  BYTE bType,
									  BYTE bChannel,
									  WORD wMapID,
									  DWORD dwRegionID,
									  WORD wLocalID)
{
	CPacket packet;

	packet.SetID(CS_REGION_REQ)
		<< dwID
		<< bType
		<< bChannel
		<< wMapID
		<< dwRegionID
		<< wLocalID;

	SayToAll(packet);
}

void CTClientGame::SendCS_SKILLEND_REQ( DWORD dwObjID,
									    BYTE bObjType,
										DWORD dwHostID,
										DWORD dwAttackID,
										BYTE bAttackType,
										WORD wSkillID,
										WORD wMapID,
										BYTE bChannel)
{
	CPacket packet;

	packet.SetID(CS_SKILLEND_REQ)
		<< dwObjID
		<< bObjType
		<< dwHostID
		<< dwAttackID
		<< bAttackType
		<< wSkillID
		<< wMapID
		<< bChannel;

	SayToAll(packet);
}

void CTClientGame::SendCS_REVIVALASK_REQ( BYTE bReply,
										  DWORD dwAttackID,
										  BYTE bAttackType,
										  WORD wSkillID,
										  BYTE bLevel)
{
	CPacket packet;

	packet.SetID(CS_REVIVALASK_REQ)
		<< bReply
		<< dwAttackID
		<< bAttackType
		<< wSkillID
		<< bLevel;

	SayToAll(packet);
}

void CTClientGame::SendCS_DROPDAMAGE_REQ( DWORD dwID,
										  BYTE bType,
										  DWORD dwDamage,
										  BYTE bChannel,
										  WORD wMapID)
{
	CPacket packet;

	packet.SetID(CS_DROPDAMAGE_REQ)
		<< dwID
		<< bType
		<< dwDamage
		<< bChannel
		<< wMapID;

	SayToAll(packet);
}

void CTClientGame::SendCS_COMMENT_REQ( CString& strComment )
{
	CPacket vMSG;
	vMSG.SetID(CS_COMMENT_REQ);
	
	vMSG
		<< strComment;

	SayToAll( vMSG );
}