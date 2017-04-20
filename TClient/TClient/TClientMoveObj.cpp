#include "stdafx.h"
#include "TClient.h"
#include "TClientGame.h"

CTClientMoveObj::CTClientMoveObj()
: m_vSyncLastPosition(0.0f, 0.0f, 0.0f)
{
	m_bCanSelected = TRUE;
	m_bDynamicOBJ = TRUE;
	m_bDrawName = TRUE;
	m_bDive = FALSE;

	D3DXMatrixIdentity(&m_vScaleRACE);
	D3DXMatrixIdentity(&m_vScaleSFX);
}

CTClientMoveObj::~CTClientMoveObj()
{
}

void CTClientMoveObj::ReleaseData()
{
	CTClientObjBase::ReleaseData();

	m_nPITCH = 0;
	m_nDIR = 0;

	m_bMouseDIR = TKDIR_N;
	m_bKeyDIR = TKDIR_N;

	m_bBack = FALSE;
	m_bDive = FALSE;
	m_bRun = TRUE;
	m_bBlock = FALSE;

	D3DXMatrixIdentity(&m_vScaleRACE);
	D3DXMatrixIdentity(&m_vScaleSFX);

	D3DXMatrixIdentity(&m_vScale);
	D3DXMatrixIdentity(&m_vPreScale);
}

void CTClientMoveObj::CalcDIR( DWORD dwTick,
							   BYTE bMouseDIR,
							   BYTE bKeyDIR)
{
	static int nMOVE[TKDIR_COUNT] = {
		1575,		// TKDIR_LF
		0,			// TKDIR_F
		225,		// TKDIR_RF
		1350,		// TKDIR_L
		MOVE_NONE,	// TKDIR_N
		450,		// TKDIR_R
		1125,		// TKDIR_LB
		900,		// TKDIR_B
		675};		// TKDIR_RB

	int nDIR = bKeyDIR / 3 < 2 ? (INT(bKeyDIR % 3) - 1) * INT(dwTick) / 2 : (1 - INT(bKeyDIR % 3)) * INT(dwTick) / 2;
	int nPITCH = m_nPITCH + INT(m_wPITCH);

	while(nPITCH < 0)
		nPITCH += 1800;
	nPITCH %= 1800;

	nDIR += INT(m_wDIR) + m_nDIR;
	while(nDIR < 0)
		nDIR += 1800;
	nDIR %= 1800;

	int nMoveDIR = nMOVE[bMouseDIR] != MOVE_NONE ? (nDIR + nMOVE[bMouseDIR]) % 1800 : MOVE_NONE;
	CalcDIR( (WORD) nMoveDIR, (WORD) nDIR, (WORD) nPITCH);

	m_bMouseDIR = bMouseDIR;
	m_bKeyDIR = bKeyDIR;

	m_nPITCH = 0;
	m_nDIR = 0;
}

void CTClientMoveObj::CalcDIR( WORD wMoveDIR,
							   WORD wDIR,
							   WORD wPITCH)
{
	if( m_wMoveDIR == wMoveDIR && m_wDIR == wDIR && m_wPITCH == wPITCH && m_bPrevFLY == IsFlyOBJ() )
		return;

	int nROT = INT(wDIR) - INT(wMoveDIR);
	BYTE bBack = FALSE;

	while(nROT < 0)
		nROT += 1800;
	nROT %= 1800;

	if( nROT > 450 && nROT < 1350 )
	{
		bBack = TRUE;
		nROT -= 900;
	}

	if( m_wMoveDIR != wMoveDIR )
		SetHeadDIR(wMoveDIR != MOVE_NONE ? FLOAT(nROT > 900 ? nROT - 1800 : nROT) * D3DX_PI / 900.0f : 0.0f);

	if( wMoveDIR == MOVE_NONE )
	{
		FLOAT fWorldX = m_vWorld._41;
		FLOAT fWorldY = m_vWorld._42;
		FLOAT fWorldZ = m_vWorld._43;

		D3DXMatrixRotationY(
			&m_vWorld,
			FLOAT(wDIR) * D3DX_PI / 900.0f);

		if(IsFlyOBJ())
			m_vWorld = GetPITCH(wPITCH) * m_vWorld;

		m_vWorld._41 = fWorldX;
		m_vWorld._42 = fWorldY;
		m_vWorld._43 = fWorldZ;
	}
	else if( m_wMoveDIR != MOVE_NONE )
	{
		D3DXVECTOR3 vMove(
			m_vWorld._41 - m_vPosition._41,
			m_vWorld._42 - m_vPosition._42,
			m_vWorld._43 - m_vPosition._43);

		D3DXMATRIX vPrevX;
		D3DXMATRIX vPrevY;
		D3DXMATRIX vNextX;
		D3DXMATRIX vNextY;

		int nPrevY = m_bBack ? INT(m_wMoveDIR) - 900 : INT(m_wMoveDIR);
		int nNextY = bBack ? INT(wMoveDIR) - 900 : INT(wMoveDIR);

		D3DXMatrixRotationY(
			&vPrevY,
			-FLOAT(nPrevY) * D3DX_PI / 900.0f);

		D3DXMatrixRotationY(
			&vNextY,
			FLOAT(nNextY) * D3DX_PI / 900.0f);

		if(m_bPrevFLY)
			vPrevX = GetPITCH(WORD(1800 - INT(m_wPITCH)));
		else
			D3DXMatrixIdentity(&vPrevX);

		if(IsFlyOBJ())
			vNextX = GetPITCH(wPITCH);
		else
			D3DXMatrixIdentity(&vNextX);

		CTMath::Transform(
			&vPrevY,
			&vMove);

		CTMath::Transform(
			&vPrevX,
			&vMove);

		CTMath::Transform(
			&vNextX,
			&vMove);

		CTMath::Transform(
			&vNextY,
			&vMove);

		m_vWorld = vNextX * vNextY;
		m_vWorld._41 = m_vPosition._41 + vMove.x;
		m_vWorld._42 = m_vPosition._42 + vMove.y;
		m_vWorld._43 = m_vPosition._43 + vMove.z;
	}
	else
	{
		FLOAT fWorldX = m_vWorld._41;
		FLOAT fWorldY = m_vWorld._42;
		FLOAT fWorldZ = m_vWorld._43;

		D3DXMatrixRotationY(
			&m_vWorld,
			FLOAT(bBack ? INT(wMoveDIR) - 900 : INT(wMoveDIR)) * D3DX_PI / 900.0f);

		if(IsFlyOBJ())
			m_vWorld = GetPITCH(wPITCH) * m_vWorld;

		m_vWorld._41 = fWorldX;
		m_vWorld._42 = fWorldY;
		m_vWorld._43 = fWorldZ;
	}

	m_wMoveDIR = wMoveDIR;
	m_wDIR = wDIR;
	m_wPITCH = wPITCH;

	m_bPrevFLY = IsFlyOBJ();
	m_bBack = bBack;
}

void CTClientMoveObj::SetHeadDIR( FLOAT fDIR)
{
	static DWORD dwPivotID[6][2] = {
		{ ID_PIVOT_WAIST, 1},
		{ ID_PIVOT_VALLY, 1},
		{ ID_PIVOT_CHEST, 1},
		{ ID_PIVOT_LARM, 3},
		{ ID_PIVOT_RARM, 3},
		{ ID_PIVOT_NECK, 4}};
	fDIR /= 4.0f;

	for( int i=0; i<6; i++)
		SetPIVOT( dwPivotID[i][0], 0.0f, fDIR * FLOAT(dwPivotID[i][1]), 0.0f, 1.0f, 1.0f, 1.0f );
}

D3DXMATRIX CTClientMoveObj::GetPITCH( WORD wPITCH)
{
	D3DXMATRIX vPITCH;

	D3DXMatrixRotationX(
		&vPITCH,
		-FLOAT(wPITCH) * D3DX_PI / 900.0f);

	return vPITCH;
}

void CTClientMoveObj::DoSLIDE( LPD3DXVECTOR3 pATTACK,
							   LPD3DXVECTOR3 pDEFEND,
							   BYTE bForceRUN)
{
	m_bSlide = FALSE;

	if( bForceRUN || (!IsDead() && !IsBlock() && !IsDown()) )
	{
		m_vSlideDIR = D3DXVECTOR2(
			pDEFEND->x - pATTACK->x,
			pDEFEND->z - pATTACK->z);
		FLOAT fLength = D3DXVec2Length(&m_vSlideDIR);

		if( fLength > 0.0f )
		{
			m_vSlideDIR *= TSLIDE_DIST / fLength;
			m_dwSlideTick = TSLIDE_TICK;
			m_bSlide = TRUE;
		}
	}

	if(!m_bSlide)
		EndSLIDE();
}

BYTE CTClientMoveObj::CheckFall( CTClientMAP *pMAP,
								 LPD3DXVECTOR2 pFallDIR)
{
	if( !pMAP || !pMAP->m_pMAP || IsFlyOBJ() )
		return FALSE;

	CTachyonHUGEMAP *pTMAP = pMAP->m_pMAP;
	D3DXVECTOR3 vTPOS = GetPosition();

	int nSIZE = pTMAP->m_nUnitLength * pTMAP->m_nTileLength;
	int nUnitX = INT(vTPOS.x) / nSIZE - pTMAP->m_nUnitX;
	int nUnitZ = INT(vTPOS.z) / nSIZE - pTMAP->m_nUnitZ;

	if( nUnitX >= 0 && nUnitX < 3 &&
		nUnitZ >= 0 && nUnitZ < 3 )
	{
		BYTE bINDEX = BYTE(nUnitZ * 3 + nUnitX);

		if( pTMAP->m_pUNIT[bINDEX] && pTMAP->m_pUNIT[bINDEX]->m_pDATA )
		{
			FLOAT *pDATA = pTMAP->m_pUNIT[bINDEX]->m_pDATA;
			D3DXVECTOR3 vTNORMAL;

			FLOAT fLEFT = FLOAT(INT(vTPOS.x / pTMAP->m_nTileLength) * pTMAP->m_nTileLength);
			FLOAT fTOP = FLOAT(INT(vTPOS.z / pTMAP->m_nTileLength) * pTMAP->m_nTileLength);
			FLOAT fRIGHT = fLEFT + pTMAP->m_nTileLength;
			FLOAT fBOTTOM = fTOP + pTMAP->m_nTileLength;

			FLOAT fResX = vTPOS.x - fLEFT;
			FLOAT fResZ = vTPOS.z - fTOP;

			int nPosX = INT(vTPOS.x) % nSIZE;
			int nPosZ = INT(vTPOS.z) % nSIZE;

			fResX /= FLOAT(pTMAP->m_nTileLength);
			fResZ /= FLOAT(pTMAP->m_nTileLength);

			nPosX /= pTMAP->m_nTileLength;
			nPosZ /= pTMAP->m_nTileLength;

			if( fResX + fResZ < 1.0f )
			{
				D3DXVECTOR3 vTPOLY[3] = {
					D3DXVECTOR3( fLEFT, pDATA[(nPosZ + 1) * (pTMAP->m_nUnitLength + 1) + nPosX], fBOTTOM),
					D3DXVECTOR3( fRIGHT, pDATA[nPosZ * (pTMAP->m_nUnitLength + 1) + nPosX + 1], fTOP),
					D3DXVECTOR3( fLEFT, pDATA[nPosZ * (pTMAP->m_nUnitLength + 1) + nPosX], fTOP)};

				D3DXVec3Cross(
					&vTNORMAL,
					&(vTPOLY[1] - vTPOLY[0]),
					&(vTPOLY[2] - vTPOLY[0]));
			}
			else
			{
				D3DXVECTOR3 vTPOLY[3] = {
					D3DXVECTOR3( fRIGHT, pDATA[(nPosZ + 1) * (pTMAP->m_nUnitLength + 1) + nPosX + 1], fBOTTOM),
					D3DXVECTOR3( fRIGHT, pDATA[nPosZ * (pTMAP->m_nUnitLength + 1) + nPosX + 1], fTOP),
					D3DXVECTOR3( fLEFT, pDATA[(nPosZ + 1) * (pTMAP->m_nUnitLength + 1) + nPosX], fBOTTOM)};

				D3DXVec3Cross(
					&vTNORMAL,
					&(vTPOLY[1] - vTPOLY[0]),
					&(vTPOLY[2] - vTPOLY[0]));
			}

			pFallDIR->x = vTNORMAL.x;
			pFallDIR->y = vTNORMAL.z;
			fResX = D3DXVec2Length(pFallDIR);

			if( fResX > 0.0f && fabs(vTNORMAL.y) / fResX <= 2.0f * CTClientMAP::m_fMoveBound )
			{
				(*pFallDIR) /= fResX;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BYTE CTClientMoveObj::Fall( LPD3DXVECTOR2 pFallDIR)
{
	if( IsRide() || IsFall() )
		return FALSE;

	if( CTChart::GetTACTION(TA_FALL)->m_bLevel >= m_bACTLevel || m_bIsHolded || m_bIsStuned )
	{
		TACTION vActionID = FindActionID(
			TA_FALL,
			GetWeaponID(m_bMode));

		m_bACTLevel = CTChart::GetTACTION( TA_FALL )->m_bLevel;
		m_bAction = TA_FALL;

		SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
	}

	if( pFallDIR->x != 0.0f || pFallDIR->y != 0.0f )
		(*pFallDIR) /= D3DXVec2Length(pFallDIR);

	m_fFallHeight = m_vPosition._42;
	m_dwFallTick = 0;
	m_vFallDIR = max( TMINFALL_SPEED, m_fMoveSpeed) * (*pFallDIR);

	return TRUE;
}

BYTE CTClientMoveObj::Jump()
{
	if( IsRide() || IsJump() || IsFall() || IsPush() || m_bStandHide ||
		CTChart::GetTACTION( TA_JUMP )->m_bLevel < m_bACTLevel )
		return FALSE;

	TACTION vActionID = FindActionID(
		TA_JUMP,
		GetWeaponID(m_bMode));
	SetTAction(TA_FLY);

	m_bACTLevel = CTChart::GetTACTION( TA_JUMP )->m_bLevel;
	SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);

	m_fJumpHeight = m_vPosition._42;
	m_dwJumpTick = 0;

	if( m_wMoveDIR != MOVE_NONE )
	{
		D3DXVECTOR3 vDIR( 0.0f, 0.0f, -m_fMoveSpeed);
		D3DXMATRIX vROT;

		D3DXMatrixRotationY(
			&vROT,
			FLOAT(m_wMoveDIR) * D3DX_PI / 900.0f);

		CTMath::Transform(
			&vROT,
			&vDIR);

		m_vJumpDIR.x = vDIR.x;
		m_vJumpDIR.y = vDIR.z;
	}
	else
		m_vJumpDIR = D3DXVECTOR2( 0.0f, 0.0f);

	return TRUE;
}

void CTClientMoveObj::CalcTick( LPD3DXVECTOR3 pPREV,
							    CD3DDevice *pDevice,
								CTachyonRes *pRES,
								DWORD dwTick)
{
	ResetAction(
		pDevice,
		pRES);

	if(IsPush())
	{
		FLOAT fMove = FLOAT(dwTick) / 1000.0f;

		m_vWorld._41 += fMove * m_vPushDIR.x;
		m_vWorld._43 += fMove * m_vPushDIR.y;
		m_dwPushTick += dwTick;

		EndSLIDE();
	}
	else if(IsFall())
	{
		FLOAT fMove = FLOAT(dwTick) / 1000.0f;

		m_vWorld._41 += fMove * m_vFallDIR.x;
		m_vWorld._43 += fMove * m_vFallDIR.y;
		m_dwFallTick += dwTick;
	}
	else if(IsJump())
	{
		FLOAT fMove = FLOAT(dwTick) / 1000.0f;

		m_vWorld._41 += fMove * m_vJumpDIR.x;
		m_vWorld._43 += fMove * m_vJumpDIR.y;
		m_dwJumpTick += dwTick;
	}

	if(m_bSlide)
	{
		DWORD dwSLIDE = min( m_dwSlideTick, dwTick);

		m_vWorld._41 += FLOAT(dwSLIDE) * m_vSlideDIR.x / FLOAT(TSLIDE_TICK);
		m_vWorld._43 += FLOAT(dwSLIDE) * m_vSlideDIR.y / FLOAT(TSLIDE_TICK);
		m_dwSlideTick -= dwSLIDE;

		if(!m_dwSlideTick)
			EndSLIDE();
	}

	CTClientObjBase::CalcTick(
		pDevice->m_pDevice,
		dwTick);

	if(dwTick)
	{
		m_fMoveSpeed = D3DXVec2Length(&D3DXVECTOR2(
			m_vPosition._41 - pPREV->x,
			m_vPosition._43 - pPREV->z)) * 1000.0f / FLOAT(dwTick);
	}
}

void CTClientMoveObj::CalcHeight( LPD3DXVECTOR3 pPREV,
								  CTClientMAP *pMAP,
								  DWORD dwTick)
{
	D3DXVECTOR2 vMOVE = D3DXVECTOR2(
		m_vPosition._41 - pPREV->x,
		m_vPosition._43 - pPREV->z);
	CTClientObjBase *pFLOOR = NULL;

	FLOAT fMove = m_fMoveSpeed * FLOAT(dwTick) / 1000.0f;
	FLOAT fRevH = 0.0f;

	BYTE bFALL = FALSE;
	BYTE bLAND = FALSE;

	FLOAT fWaterHeight = TMIN_HEIGHT;
	FLOAT fHeight = TMIN_HEIGHT;

	if(ForecastHeight( pMAP, &fHeight, &fWaterHeight, dwTick))
	{
		m_vWorld._42 += fHeight - m_vPosition._42;
		m_vPosition._42 = fHeight;
	}
	else if( IsPush() || IsFall() || IsJump() )
		bLAND = TRUE;

	fHeight = pMAP->GetHeight(
		this, &pFLOOR,
		&D3DXVECTOR3(
		m_vPosition._41,
		max( pPREV->y, m_vPosition._42) + 0.2f,
		m_vPosition._43),
		fMove,
		TRUE);

	if(pFLOOR)
	{
		m_bHouseMesh = pFLOOR->m_bHouseMesh;
		m_dwHouseID = pFLOOR->m_dwHouseID;
		m_bLand = FALSE;
	}
	else
	{
		m_bHouseMesh = FALSE;
		m_dwHouseID = 0;
		m_bLand = TRUE;
	}
	m_fSquareHeight = fHeight;

	if( !CanDIVE() && fHeight < fWaterHeight + m_fBreathHeight )
		fHeight = fWaterHeight + m_fBreathHeight;

	if(CanFLY())
	{
		FLOAT fMinSKY = max( fHeight, fWaterHeight);
		FLOAT fMaxSKY = fMinSKY + GetFlyMAX(
			pMAP,
			m_vPosition._41,
			m_vPosition._43);

		fHeight = max( fMinSKY, m_vPosition._42);
		fHeight = min( fMaxSKY, fHeight);
		fRevH = fHeight - m_vPosition._42;

		m_vPosition._42 += fRevH;
		m_vWorld._42 += fRevH;
	}
	m_bSwim = fHeight > (m_bSwim ?  fWaterHeight + TWATER_ESCAPE : fWaterHeight) ? FALSE : TRUE;

	if( m_dwANISNDFuncID && m_bSTEPSND )
	{
		if( fWaterHeight + m_fBreathHeight > fHeight )
			m_dwANISNDFuncID = TWATERSTEP_SND;
		else if(m_bLand)
			m_dwANISNDFuncID = pMAP->GetTStepSND( m_vPosition._41, m_vPosition._42, m_vPosition._43);
		else
			m_dwANISNDFuncID = TDEFSTEP_SND;

		m_bSTEPSND = FALSE;
	}

	if(m_bSwim)
		fHeight = max( fHeight, min( fWaterHeight, m_vPosition._42));

	if( (fHeight + THEIGHT_ZERO) < m_vPosition._42 && !CanFLY() && !IsFall() && !IsJump() && !IsPush() &&
		fMove <= CTClientMAP::m_fMoveBound * (m_vPosition._42 - fHeight) )
		bFALL = Fall(&vMOVE);

	if( !IsJump() && !IsFall() && !bFALL && !IsPush() )
	{
		D3DXVECTOR2 vTDIR;

		m_vWorld._42 += fHeight - m_vPosition._42;
		m_vPosition._42 = fHeight;

		if( !pFLOOR && CheckFall( pMAP, &vTDIR) )
		{
			m_fMoveSpeed = TMINFALL_SPEED;
			Fall(&vTDIR);
		}
	}
	else if( dwTick && m_vPosition._42 <= pPREV->y + fRevH && (m_vPosition._42 <= fHeight || bLAND) )
	{
		m_vWorld._42 += fHeight - m_vPosition._42;
		m_vPosition._42 = fHeight;

		if( !m_dwHP && !m_bGhost )
		{
			m_bAction = TA_DEAD;
			m_bACTLevel = CTChart::GetTACTION( TA_DEAD )->m_bLevel;
		}
		else if( IsJump() || IsFall() )
		{
			if(IsJump())
				m_dwDropDamage = CalcJumpDamage();
			else
				m_dwDropDamage = CalcFallDamage();

			m_bAction = GetTAction();
			m_bACTLevel = CTChart::GetTACTION( m_bAction )->m_bLevel;
		}
		else if(IsPush())
		{
			TACTION vActionID = FindActionID(
				TA_DOWN,
				GetWeaponID(m_bMode));

			m_dwDropDamage = CalcPushDamage();
			m_bAction = GetTAction();

			if( m_bACTLevel <= CTChart::GetTACTION( TA_DOWN )->m_bLevel )
				if( vActionID.m_dwActID && vActionID.m_dwAniID )
				{
					SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);

					m_bACTLevel = CTChart::GetTACTION( TA_DOWN )->m_bLevel;
					m_bDown = TRUE;
				}
				else
					m_bACTLevel = CTChart::GetTACTION( m_bAction )->m_bLevel;
		}
		else
			m_bAction = GetTAction();
	}
	else if( dwTick && m_vPosition._42 <= fHeight )
	{
		FLOAT fMoveH = fHeight - m_vPosition._42;

		m_vPosition._42 = fHeight;
		m_vWorld._42 += fMoveH;

		if(IsJump())
			m_fJumpHeight += fMoveH;

		if(IsFall())
			m_fFallHeight += fMoveH;

		if(IsPush())
			m_fPushHeight += fMoveH;
	}

	LPTREGIONINFO pTREGION = pMAP ? pMAP->GetRegionINFO(
		m_vPosition._41,
		m_vPosition._43) : NULL;

	if(pTREGION)
		m_pTREGION = pTREGION;

	m_bDive = !IsDead() && !m_bGhost && !m_bMermaid && fWaterHeight > fHeight + TWATER_ESCAPE ? TRUE : FALSE;
}

DWORD CTClientMoveObj::CalcJumpDamage()
{
	return CalcDropDamage(
		m_fJumpSpeed,
		CTClientMAP::m_fGravity,
		FLOAT(m_dwJumpTick) / 1000.0f);
}

DWORD CTClientMoveObj::CalcFallDamage()
{
	return CalcDropDamage(
		0.0f, CTClientMAP::m_fGravity,
		FLOAT(m_dwFallTick) / 1000.0f);
}

DWORD CTClientMoveObj::CalcPushDamage()
{
	return CalcDropDamage(
		m_fPushSpeed,
		TPUSH_BUST * CTClientMAP::m_fGravity,
		FLOAT(m_dwPushTick) / 1000.0f);
}

DWORD CTClientMoveObj::CalcDropDamage( FLOAT fSpeed,
									   FLOAT fGravity,
									   FLOAT fTime)
{
	FLOAT fDropTime = fSpeed / fGravity;

	if( m_bParashute || m_bSwim || fTime < fDropTime )
		return 0;
	fTime -= fDropTime;

	FLOAT fHeight = fGravity * fTime * fTime / 2.0f;
	FLOAT fVelocity = sqrtf(2.0f * fHeight * CTClientMAP::m_fGravity);
	FLOAT fDropVelocity = sqrtf(TDROPDAMAGE_HEIGHT * CTClientMAP::m_fGravity);
	FLOAT fDropDamage = 20.0f * max( fVelocity - fDropVelocity, 0.0f);
	FLOAT fRatio = 6.0f * max( fVelocity - fDropVelocity, 0.0f);

	return DWORD(fRatio * FLOAT(m_dwMaxHP) / 100.0f + fDropDamage);
}

FLOAT CTClientMoveObj::GetFlyMAX( CTClientMAP *pMAP,
								  FLOAT fPosX,
								  FLOAT fPosZ)
{
	if( !pMAP || !pMAP->m_pMAP )
		return TFLY_MIN;

	if( CTClientGame::GetInstance()->IsMiniGameBattle() || CTClientGame::GetInstance()->m_bTournamentWatching )
		return 7.0f;

	int nUnitLength = pMAP->m_pMAP->m_nUnitLength * pMAP->m_pMAP->m_nTileLength;
	CRect rect(
		0, 0,
		nUnitLength,
		nUnitLength);

	rect.OffsetRect(
		INT(fPosX) / nUnitLength * nUnitLength,
		INT(fPosZ) / nUnitLength * nUnitLength);

	rect.DeflateRect(
		TUNIT_BOUND,
		TUNIT_BOUND);

	FLOAT fDELTA = fPosX - FLOAT(rect.left);
	FLOAT fVALUE = FLOAT(rect.right) - fPosX;

	fDELTA = min( fDELTA, fVALUE);
	fVALUE = FLOAT(rect.bottom) - fPosZ;
	fDELTA = min( fDELTA, fVALUE);
	fVALUE = fPosZ - FLOAT(rect.top);
	fDELTA = min( fDELTA, fVALUE);

	if( fDELTA > FLOAT(2 * TUNIT_BOUND) )
		return TFLY_MAX;

	if( fDELTA < 0.0f )
		return TFLY_MIN;

	return TFLY_MIN + (TFLY_MAX - TFLY_MIN) * fDELTA / FLOAT(2 * TUNIT_BOUND);
}

BYTE CTClientMoveObj::GetTAction()
{
	switch(m_bAction)
	{
	case TA_DEAD	:
	case TA_DIE		: return TA_DEAD;
	default			:
		if( !m_bIsHolded && !m_bIsStuned &&
			m_bMouseDIR != TKDIR_N )
			return m_bRun ? TA_RUN : TA_WALK;

		break;
	}

	return CTClientObjBase::GetTAction();
}

BYTE CTClientMoveObj::GetTAction( BYTE bMouseDIR)
{
	switch(m_bAction)
	{
	case TA_DEAD		:
	case TA_DIE			: return TA_DEAD;
	default				:
		if(CTChart::GetTACTION( m_bAction )->m_bContinue)
			return m_bAction;
		else if( !m_bIsHolded && !m_bIsStuned && bMouseDIR != TKDIR_N )
			return m_bRun ? TA_RUN : TA_WALK;

		break;
	}

	return CTClientObjBase::GetTAction();
}

void CTClientMoveObj::CancelTAction()
{
	BYTE bAction = GetTAction(m_bMouseDIR);

	if(CTChart::GetTACTION( bAction )->m_bCancel)
	{
		m_bAction = GetTAction();
		m_bACTLevel = CTChart::GetTACTION( m_bAction )->m_bLevel;
	}

	ClearTActionSFX(&m_vTACTIONSFX);
}

void CTClientMoveObj::RemoveTAction()
{
	BYTE bAction = GetTAction(m_bMouseDIR);

	if(CTChart::GetTACTION( bAction )->m_bContinue)
		m_bAction = GetTAction();

	ClearTActionSFX(&m_vTACTIONSFX);
}

void CTClientMoveObj::SetAction( DWORD dwActionID,
								 DWORD dwAniID)
{
	if(CheckAction( dwActionID, dwAniID))
	{
		CTClientObjBase::SetAction(
			dwActionID,
			dwAniID);
	}
}

void CTClientMoveObj::ResetAction( CD3DDevice *pDevice,
								   CTachyonRes *pRES)
{
	if( m_bACTLevel > CTChart::GetTACTION(m_bAction)->m_bLevel )
		return;
	BYTE bActionID = TA_COUNT;

	switch(m_bAction)
	{
	case TA_FOLLOW		: bActionID = m_bKeyDIR == TKDIR_N ? m_bSwim ? TA_SWIMSTAND : TA_STAND : m_bSwim ? TA_SWIMMOVE : TA_RUN; break;
	case TA_WALK		: bActionID = m_bBack ? m_bSwim ? TA_SWIMBACK : TA_BACK : m_bSwim ? TA_SWIMMOVE : TA_WALK; break;
	case TA_RUN			: bActionID = m_bBack ? m_bSwim ? TA_SWIMBACK : TA_BACK : m_bSwim ? TA_SWIMMOVE : TA_RUN; break;
	case TA_DEAD		: bActionID = m_bAction; break;
	case TA_STAND		:
		if(!m_bSwim)
			switch(m_bKeyDIR)
			{
			case TKDIR_L	:
				{
					SetHeadDIR(-D3DX_PI / 4.0f);
					bActionID = TA_TURN_LEFT;
				}

				break;

			case TKDIR_R	:
				{
					SetHeadDIR(D3DX_PI / 4.0f);
					bActionID = TA_TURN_RIGHT;
				}

				break;

			default			:
				{
					SetHeadDIR(0.0f);
					bActionID = TA_STAND;
				}

				break;
			}
		else
			bActionID = TA_SWIMSTAND;

		break;

	default				:
		if(CTChart::GetTACTION(m_bAction)->m_bContinue)
			bActionID = m_bAction;

		break;
	}

	if( bActionID != TA_COUNT )
	{
		TACTION vActionID = FindActionID(
			bActionID,
			GetWeaponID(m_bMode));
		BYTE bHideOnCapeMode = CTChart::GetTACTION(bActionID)->m_bHideOnCapeMode;

		if( m_bHideOnCapeMode != bHideOnCapeMode ||
			vActionID.m_bEquipMode != m_bEquipMode )
		{
			m_bEquipMode = vActionID.m_bEquipMode;
			m_bHideOnCapeMode = bHideOnCapeMode;

			ResetEQUIP(
				pDevice,
				pRES);
		}

		SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
	}
}

WORD CTClientMoveObj::GetTargetPITCH( FLOAT fTargetX,
									  FLOAT fTargetY,
									  FLOAT fTargetZ)
{
	if(CanDIVE())
	{
		FLOAT fLength = D3DXVec2Length(&D3DXVECTOR2(
			fTargetX,
			fTargetZ));

		if( fLength > 0.1f )
		{
			int nPITCH = -INT(atan2f( fTargetY, fLength) * 900.0f / D3DX_PI);

			while(nPITCH < 0)
				nPITCH += 1800;

			return WORD(nPITCH % 1800);
		}
	}

	return 0;
}

WORD CTClientMoveObj::GetTargetDIR( FLOAT fTargetX,
									FLOAT fTargetZ)
{
	if( fTargetZ != 0.0f )
	{
		int nDIR = 900 + INT(atan2f( fTargetX, fTargetZ) * 900.0f / D3DX_PI);

		while(nDIR < 0)
			nDIR += 1800;

		return WORD(nDIR % 1800);
	}
	else if( fTargetX > 0.0f )
		return 1350;
	else if( fTargetX < 0.0f )
		return 450;

	return m_wDIR;
}

void CTClientMoveObj::Push( FLOAT fPushDist,
							LPD3DXVECTOR2 pDIR)
{
	FLOAT fLength = D3DXVec2Length(pDIR);

	if( IsPush() ||
		CTChart::GetTACTION( TA_PUSH )->m_bLevel < m_bACTLevel ||
		fPushDist == 0.0f ||
		fLength == 0.0f ||
		(m_bType == OT_PC && ((CTClientChar*)this)->GetRidingPet()) )
		return;

	m_fPushSpeed = TPUSH_BUST * CTClientMAP::m_fGravity * fPushDist / (4.0f * TPUSH_SPEED);
	m_fPushHeight = m_vPosition._42;

	m_vPushDIR = (*pDIR) * 3.0f * TPUSH_SPEED / fLength;
	m_dwPushTick = 0;

	TACTION vActionID = FindActionID(
		TA_PUSHSTART,
		GetWeaponID(m_bMode));
	SetTAction(TA_PUSH);

	m_bACTLevel = CTChart::GetTACTION( TA_PUSHSTART )->m_bLevel;
	SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
}

void CTClientMoveObj::DoTRANS( CD3DDevice *pDevice,
							   CTachyonRes *pRES,
							   WORD wMonID)
{
	if(!m_bCanSelected)
		return;

	LPTMONTEMP pTMON = CTChart::FindTMONTEMP( wMonID );
	if(pTMON)
	{
		MAPTINVEN::iterator finder = m_mapTINVEN.find(INVEN_TRANS);
		CTClientInven *pTINVEN = NULL;

		if( finder == m_mapTINVEN.end() )
		{
			pTINVEN = new CTClientInven();

			pTINVEN->m_bInvenID = INVEN_TRANS;
			pTINVEN->m_wItemID = 0;

			m_mapTINVEN.insert( MAPTINVEN::value_type( INVEN_TRANS, pTINVEN));
		}
		else
			pTINVEN = (*finder).second;

		pTINVEN->ClearInven();

		for( BYTE i=0; i<ES_COUNT; i++)
		{
			if(pTMON->m_pTITEM[i])
			{
				CTClientItem *pTITEM = new CTClientItem();

				pTITEM->SetItemSlot(i);
				pTITEM->SetTITEM(pTMON->m_pTITEM[i]);
				pTITEM->SetCount(1);

				pTINVEN->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
			}
		}

		InitOBJ(pRES->GetOBJ(pTMON->m_dwOBJ));
		m_pTRANS = pTMON;
		ResetEQUIP( pDevice, pRES);

		m_fBreathHeight = GetAttrFLOAT(ID_BREATH_HEIGHT);
		m_bCanSelected = pTMON->m_bCanSelected;

		InitSIZE(
			pTMON->m_fSize,
			pTMON->m_fScaleY * GetAttrFLOAT(ID_SIZE_Y),
			pTMON->m_fSize,
			0.0f, 0.0f);

		// 08.10.31 ±Ç¼ºÇõ
		m_vPreScale = m_vScale;

		D3DXMatrixScaling(
			&m_vScale,
			pTMON->m_fScaleX,
			pTMON->m_fScaleY,
			pTMON->m_fScaleZ);

		m_vScaleSFX = CTMath::Inverse(&m_vScale);
		m_vPosition = m_vScale * m_vWorld;
		//...

		ResetRootID(ID_PIVOT_WAIST);

		CTClientObjBase::DoTRANS(
			pDevice,
			pRES,
			wMonID);
	}
}

void CTClientMoveObj::DoRETRANS(
					   CD3DDevice *pDevice,
					   CTachyonRes *pRES)
{
	m_vScale = m_vPreScale;

	CTClientObjBase::DoRETRANS(
		pDevice,
		pRES);
}

void CTClientMoveObj::OnDIE( CD3DDevice *pDevice,
							 CTachyonRes *pRES)
{
	m_bMouseDIR = TKDIR_N;
	m_bKeyDIR = TKDIR_N;

	CTClientObjBase::OnDIE( pDevice, pRES);
}

void CTClientMoveObj::CalcSFX( CD3DDevice *pDevice,
							   LPVECTOROBJSFX pSFX)
{
	VECTOROBJSFX::iterator it = pSFX->begin();

	while(it != pSFX->end())
	{
		LPOBJSFX pItem = (*it);
		CTachyonSFX *pTSFX = CTachyonSFX::GetSFX(pItem->m_dwID);

		if( !pTSFX->m_pSFX || !pTSFX->IsDead() )
		{
			D3DXMATRIX vRotX;
			D3DXMATRIX vRotY;
			D3DXMATRIX vRotZ;

			D3DXMatrixRotationX(
				&vRotX,
				pItem->m_pSFX->m_vROT.x);

			D3DXMatrixRotationY(
				&vRotY,
				pItem->m_pSFX->m_vROT.y);

			D3DXMatrixRotationZ(
				&vRotZ,
				pItem->m_pSFX->m_vROT.z);
			pTSFX->m_vGlobal = vRotX * vRotY * vRotZ;

			pTSFX->m_vGlobal._41 = pItem->m_pSFX->m_vPOS.x;
			pTSFX->m_vGlobal._42 = pItem->m_pSFX->m_vPOS.y;
			pTSFX->m_vGlobal._43 = pItem->m_pSFX->m_vPOS.z;

			if(pItem->m_pSFX->m_nPivotID)
				pTSFX->m_vGlobal *= m_pBone[pItem->m_pSFX->m_nPivotID];
			else if(pItem->m_bUseOBJScale)
				pTSFX->m_vGlobal = m_vScaleRACE * pTSFX->m_vGlobal * m_vScaleSFX * GetPositionMat();
			else
				pTSFX->m_vGlobal *= GetPositionMat();

			if(!pTSFX->m_pSFX)
			{
				DWORD dwInitTick = pTSFX->m_dwInitTick;

				pTSFX->InitSFX(
					pDevice,
					pItem->m_pSFX->m_pSFX);
				pTSFX->m_dwInitTick = dwInitTick;
			}

			it++;
		}
		else
		{
			CTachyonSFX::Release(pItem->m_dwID);
			delete pItem;

			pSFX->erase(it);
		}
	}
}
