#include "stdafx.h"
#include "TExplorer.h"


CTExplorerMoveObj::CTExplorerMoveObj()
{
	m_bDrawShadow = TRUE;
	m_bDrawName = TRUE;
}

CTExplorerMoveObj::~CTExplorerMoveObj()
{
}

void CTExplorerMoveObj::ReleaseData()
{
	CTExplorerObjBase::ReleaseData();

	m_vJumpDIR = D3DXVECTOR2( 0.0f, 0.0f);
	m_vFallDIR = D3DXVECTOR2( 0.0f, 0.0f);

	m_fBreathHeight = 0.0f;
	m_fJumpHeight = 0.0f;
	m_fFallHeight = 0.0f;

	m_fMoveSpeed = 0.0f;
	m_fJumpSpeed = 0.0f;

	m_dwJumpTick = 0;
	m_dwFallTick = 0;

	m_nPITCH = 0;
	m_nDIR = 0;

	m_bMouseDIR = TKDIR_N;
	m_bKeyDIR = TKDIR_N;

	m_bBack = FALSE;
	m_bRun = TRUE;
}

void CTExplorerMoveObj::CalcDIR( DWORD dwTick,
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

	int nDIR = bKeyDIR / 3 < 2 ? (INT(bKeyDIR % 3) - 1) * INT(dwTick) : (1 - INT(bKeyDIR % 3)) * INT(dwTick);
	int nPITCH = m_nPITCH + INT(m_wPITCH);

	while(nPITCH < 0)
		nPITCH += 1800;
	m_wPITCH = nPITCH % 1800;

	nDIR += INT(m_wDIR) + m_nDIR;
	while(nDIR < 0)
		nDIR += 1800;
	nDIR %= 1800;

	int nMoveDIR = nMOVE[bMouseDIR] != MOVE_NONE ? (nDIR + nMOVE[bMouseDIR]) % 1800 : MOVE_NONE;
	CalcDIR( (WORD) nMoveDIR, (WORD) nDIR);

	m_bMouseDIR = bMouseDIR;
	m_bKeyDIR = bKeyDIR;

	m_nPITCH = 0;
	m_nDIR = 0;
}

void CTExplorerMoveObj::CalcDIR( WORD wMoveDIR, WORD wDIR)
{
	static DWORD dwPivotID[6][2] = {
		{ ID_PIVOT_WAIST, 1},
		{ ID_PIVOT_VALLY, 1},
		{ ID_PIVOT_CHEST, 1},
		{ ID_PIVOT_LARM, 3},
		{ ID_PIVOT_RARM, 3},
		{ ID_PIVOT_NECK, 4}};

	if( m_wMoveDIR == wMoveDIR && m_wDIR == wDIR && m_bSwim == m_bPrevSwim )
		return;
	BYTE bBack = FALSE;

	if( m_wMoveDIR != wMoveDIR )
		if( wMoveDIR != MOVE_NONE )
		{
			int nROT = INT(wDIR) - INT(wMoveDIR);

			while(nROT < 0)
				nROT += 1800;
			nROT %= 1800;

			if( nROT > 450 && nROT < 1350 )
			{
				bBack = TRUE;
				nROT -= 900;
			}

			FLOAT fRotUP = FLOAT(nROT > 900 ? nROT - 1800 : nROT) * D3DX_PI / 3600.0f;
			for( int i=0; i<6; i++)
				SetPIVOT( dwPivotID[i][0], 0.0f, fRotUP * FLOAT(dwPivotID[i][1]), 0.0f, 1.0f, 1.0f, 1.0f );
		}
		else
		{
			for( int i=0; i<6; i++)
				SetPIVOT( dwPivotID[i][0], 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f );
		}

	if( wMoveDIR == MOVE_NONE )
	{
		D3DXMatrixRotationY(
			&m_vWorld,
			FLOAT(wDIR) * D3DX_PI / 900.0f);

		if(m_bSwim)
			m_vWorld *= GetPITCH(wDIR);

		m_vWorld._41 = m_vPosition._41;
		m_vWorld._42 = m_vPosition._42;
		m_vWorld._43 = m_vPosition._43;
	}
	else if( m_wMoveDIR != MOVE_NONE )
	{
		D3DXVECTOR3 vMove(
			m_vPosition._41 - m_vWorld._41,
			m_vPosition._42 - m_vWorld._42,
			m_vPosition._43 - m_vWorld._43);

		D3DXMATRIX vINV = CTMath::Inverse(&m_vWorld);
		D3DXMATRIX vROT;

		int nRotY = bBack ? INT(wMoveDIR) - 900 : INT(wMoveDIR);
		vINV._41 = 0.0f;
		vINV._42 = 0.0f;
		vINV._43 = 0.0f;

		D3DXMatrixRotationY(
			&vROT,
			FLOAT(nRotY) * D3DX_PI / 900.0f);

		if(m_bSwim)
			vROT *= GetPITCH(wDIR);
		vROT *= vINV;

		CTMath::Transform(
			&vROT,
			&vMove);

		m_vPosition._41 -= vMove.x;
		m_vPosition._42 -= vMove.y;
		m_vPosition._43 -= vMove.z;

		m_vWorld = vROT * m_vPosition;
		m_vPosition._41 += vMove.x;
		m_vPosition._42 += vMove.y;
		m_vPosition._43 += vMove.z;
	}
	else
	{
		D3DXMatrixRotationY(
			&m_vWorld,
			FLOAT(bBack ? INT(wMoveDIR) - 900 : INT(wMoveDIR)) * D3DX_PI / 900.0f);

		if(m_bSwim)
			m_vWorld *= GetPITCH(wDIR);

		m_vWorld._41 = m_vPosition._41;
		m_vWorld._42 = m_vPosition._42;
		m_vWorld._43 = m_vPosition._43;
	}

	m_wMoveDIR = wMoveDIR;
	m_wDIR = wDIR;

	m_bPrevSwim = m_bSwim;
	m_bBack = bBack;
}

D3DXMATRIX CTExplorerMoveObj::GetPITCH( WORD wDIR)
{
	D3DXVECTOR3 vAxisX( -1.0f, 0.0f, 0.0f);
	D3DXMATRIX vPITCH;

	D3DXMatrixRotationY(
		&vPITCH,
		FLOAT(wDIR) * D3DX_PI / 900.0f);

	CTMath::Transform(
		&vPITCH,
		&vAxisX);

	D3DXMatrixRotationAxis(
		&vPITCH,
		&vAxisX,
		FLOAT(m_wPITCH) * D3DX_PI / 900.0f);

	return vPITCH;
}

BYTE CTExplorerMoveObj::Fall( LPD3DXVECTOR2 pFallDIR)
{
	if( IsFall() || CTChart::GetTACTION( TA_FALL )->m_bLevel < m_bACTLevel )
		return FALSE;

	TACTION vActionID = FindActionID(
		TA_FALL,
		GetWeaponID(m_bMode));
	SetTAction(TA_FALL);

	m_bACTLevel = CTChart::GetTACTION( TA_FALL )->m_bLevel;
	SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);

	if( pFallDIR->x != 0.0f || pFallDIR->y != 0.0f )
		(*pFallDIR) /= D3DXVec2Length(pFallDIR);

	m_fFallHeight = m_vPosition._42;
	m_dwFallTick = 0;
	m_vFallDIR = max( TMINFALL_SPEED, m_fMoveSpeed) * (*pFallDIR);

	return TRUE;
}

BYTE CTExplorerMoveObj::Jump()
{
	if( IsJump() || CTChart::GetTACTION( TA_JUMP )->m_bLevel < m_bACTLevel )
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

void CTExplorerMoveObj::CalcTick( LPD3DXVECTOR3 pPREV,
								  CD3DDevice *pDevice,
								  CTachyonRes *pRES,
								  DWORD dwTick)
{
	ResetAction(
		pDevice,
		pRES);

	if(IsFall())
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

	CTExplorerObjBase::CalcTick(
		pDevice->m_pDevice,
		dwTick);

	if(dwTick)
	{
		m_fMoveSpeed = D3DXVec2Length(&D3DXVECTOR2(
			m_vPosition._41 - pPREV->x,
			m_vPosition._43 - pPREV->z)) * 1000.0f / FLOAT(dwTick);
	}
}

void CTExplorerMoveObj::CalcHeight( LPD3DXVECTOR3 pPREV,
								    CTExplorerMAP *pMAP,
									DWORD dwTick)
{
	D3DXVECTOR2 vMOVE = D3DXVECTOR2(
		m_vPosition._41 - pPREV->x,
		m_vPosition._43 - pPREV->z);
	CTExplorerObjBase *pFLOOR = NULL;

	FLOAT fMove = m_fMoveSpeed * FLOAT(dwTick) / 1000.0f;
	BYTE bLAND = FALSE;

	FLOAT fWaterHeight = pMAP->GetWaterHeight(
		m_vPosition._41,
		m_vPosition._43) - m_fBreathHeight;

	if(IsFall())
	{
		FLOAT fTime = FLOAT(m_dwFallTick) / 1000.0f;
		FLOAT fFallHeight = m_fFallHeight - CTExplorerMAP::m_fGravity * fTime * fTime / 2.0f;

		if( !m_bSwim || fFallHeight > fWaterHeight )
		{
			m_vWorld._42 += fFallHeight - m_vPosition._42;
			m_vPosition._42 = fFallHeight;
		}
		else
			bLAND = TRUE;
	}
	else if(IsJump())
	{
		FLOAT fTime = FLOAT(m_dwJumpTick) / 1000.0f;
		FLOAT fJumpHeight = m_fJumpHeight + m_fJumpSpeed * fTime - CTExplorerMAP::m_fGravity * fTime * fTime / 2.0f;

		if( !m_bSwim || fTime < m_fJumpSpeed / CTExplorerMAP::m_fGravity || fJumpHeight > fWaterHeight )
		{
			m_vWorld._42 += fJumpHeight - m_vPosition._42;
			m_vPosition._42 = fJumpHeight;
		}
		else
			bLAND = TRUE;
	}

	FLOAT fLandHeight = pMAP->GetLandHeight(
		m_vPosition._41,
		max( pPREV->y, m_vPosition._42),
		m_vPosition._43);

	FLOAT fHeight = pMAP->GetHeight(
		&pMAP->m_vDRAWOBJ,
		this, &pFLOOR,
		&D3DXVECTOR3(
		m_vPosition._41,
		max( pPREV->y, m_vPosition._42) + 0.1f,
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
	m_bSwim = fHeight > (m_bSwim ? fWaterHeight + TWATER_ESCAPE : fWaterHeight) ? FALSE : TRUE;

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

	if( fHeight < m_vPosition._42 && !IsFall() && !IsJump() && (
		fMove == 0.0f || CTExplorerMAP::m_fFallBound < (m_vPosition._42 - fHeight) / fMove) )
		Fall(&vMOVE);

	if( !IsJump() && !IsFall() )
	{
		m_vWorld._42 += fHeight - m_vPosition._42;
		m_vPosition._42 = fHeight;
	}
	else if( dwTick && m_vPosition._42 <= pPREV->y && (m_vPosition._42 <= fHeight || bLAND) )
	{
		m_vWorld._42 += fHeight - m_vPosition._42;
		m_vPosition._42 = fHeight;

		if( IsJump() || IsFall() )
		{
			m_bAction = GetTAction();
			m_bACTLevel = CTChart::GetTACTION( m_bAction )->m_bLevel;
		}
		else
			m_bAction = GetTAction();
	}

	LPTREGIONINFO pTREGION = pMAP ? pMAP->GetRegionINFO(
		m_vPosition._41,
		m_vPosition._43) : NULL;

	if(pTREGION)
		m_pTREGION = pTREGION;
}

BYTE CTExplorerMoveObj::GetTAction()
{
	switch(m_bAction)
	{
	case TA_DEAD	:
	case TA_DIE		: return TA_DEAD;
	default			:
		if( m_bMouseDIR != TKDIR_N )
			return m_bRun ? TA_RUN : TA_WALK;

		break;
	}

	return TA_STAND;
}

BYTE CTExplorerMoveObj::GetTAction( BYTE bMouseDIR)
{
	switch(m_bAction)
	{
	case TA_DEAD		:
	case TA_DIE			: return TA_DEAD;
	default				:
		if( CTChart::GetTACTION( m_bAction )->m_bContinue)
			return m_bAction;
		else if( bMouseDIR != TKDIR_N )
			return m_bRun ? TA_RUN : TA_WALK;

		break;
	}

	return TA_STAND;
}

void CTExplorerMoveObj::CancelTAction()
{
	BYTE bAction = GetTAction(m_bMouseDIR);

	if( CTChart::GetTACTION( bAction )->m_bCancel)
		m_bAction = GetTAction();

	ClearTActionSFX(&m_vTACTIONSFX);
}

void CTExplorerMoveObj::RemoveTAction()
{
	BYTE bAction = GetTAction(m_bMouseDIR);

	if( CTChart::GetTACTION( bAction )->m_bContinue)
		m_bAction = GetTAction();

	ClearTActionSFX(&m_vTACTIONSFX);
}

void CTExplorerMoveObj::SetAction( DWORD dwActionID,
								   DWORD dwAniID)
{
	CTExplorerObjBase::SetAction(
		dwActionID,
		dwAniID);
}

void CTExplorerMoveObj::ResetAction( CD3DDevice *pDevice,
									 CTachyonRes *pRES)
{
	if( m_bACTLevel > CTChart::GetTACTION( m_bAction )->m_bLevel )
		return;
	BYTE bActionID = TA_COUNT;

	switch(m_bAction)
	{
	case TA_FOLLOW		: bActionID = m_bKeyDIR == TKDIR_N ? m_bSwim ? TA_SWIMSTAND : TA_STAND : m_bSwim ? TA_SWIMMOVE : TA_RUN; break;
	case TA_WALK		: bActionID = m_bBack ? m_bSwim ? TA_SWIMBACK : TA_BACK : m_bSwim ? TA_SWIMMOVE : TA_WALK; break;
	case TA_RUN			: bActionID = m_bBack ? m_bSwim ? TA_SWIMBACK : TA_BACK : m_bSwim ? TA_SWIMMOVE : TA_RUN; break;
	case TA_STAND		: bActionID = m_bSwim ? TA_SWIMSTAND : TA_STAND; break;
	case TA_DEAD		: bActionID = m_bAction; break;
	default				:
		if( CTChart::GetTACTION( m_bAction )->m_bContinue)
			bActionID = m_bAction;

		break;
	}

	if( bActionID != TA_COUNT )
	{
		TACTION vActionID = FindActionID(
			bActionID,
			GetWeaponID(m_bMode));

		if( vActionID.m_bEquipMode != m_bEquipMode )
			ResetWeaponPOS( pDevice, pRES, vActionID.m_bEquipMode);

		SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
	}
}

WORD CTExplorerMoveObj::GetTargetDIR( FLOAT fTargetX,
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
		return 450;
	else if( fTargetX < 0.0f )
		return 1350;

	return 1800;
}
