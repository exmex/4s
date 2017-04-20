#include "stdafx.h"
#include "TTournamentResult.h"
#include "TClientGame.h"


CTTournamentResult::CTTournamentResult( TComponent *pParent, CD3DDevice *pDevice, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc )
{
	m_pKILL1 = FindKid(ID_CTRLINST_KILL1);
	m_pKILL2 = FindKid(ID_CTRLINST_KILL2);
	m_pNAME1 = FindKid(ID_CTRLINST_NAME1);
	m_pNAME2 = FindKid(ID_CTRLINST_NAME2);
	m_pCAMERA_LEFT = FindKid(ID_CTRLINST_CAMERA_LEFT);
	m_pCAMERA_RIGHT = FindKid(ID_CTRLINST_CAMERA_RIGHT);
	m_pWL1 = FindKid(ID_CTRLINST_WL1);
	m_pWL2 = FindKid(ID_CTRLINST_WL2);
	m_pDRAW = FindKid(ID_CTRLINST_DRAW);
	m_pDRAW->GetComponentPos(&m_ptDRAW);
	RemoveKid(m_pDRAW);
	m_pWINNER = FindKid(ID_CTRLINST_WINNER);
	RemoveKid(m_pWINNER);
	m_pWINNER_LEFT = FindKid(ID_CTRLINST_WINNER_LEFT);
	m_pWINNER_RIGHT = FindKid(ID_CTRLINST_WINNER_RIGHT);
	m_pTEXT = FindKid(ID_CTRLINST_TEXT);
	m_pTEXT->GetComponentPos(&m_ptTEXT);
	RemoveKid(m_pTEXT);
	m_pTICK1 = FindKid(ID_CTRLINST_TICK1);
	m_pTICK2 = FindKid(ID_CTRLINST_TICK2);
	m_pTXT_HIDE = FindKid(ID_CTRLINST_TXT_HIDE);
	m_pDevice = pDevice;

	m_fDist = 1.3f;
	m_fTheta = 90.0f * D3DX_PI / 180.0f; // 수직회전
	m_fPhi = 0.0f; // 수평회전
	m_vPosOffset.x = 0.0f;
	m_vPosOffset.y = 0.0f;
	m_vPosOffset.z = 0.0f;
	m_bDrag = FALSE;

	m_dwLeftCharID = 0;
	m_dwRightCharID = 0;

	m_bBeginDice = FALSE;
	m_dwTickDice = 0;
	m_dwPrevTickDice = 0;
	m_bDiceLeftTeam = 0;
	m_bDiceRightTeam = 0;
}

CTTournamentResult::~CTTournamentResult()
{
	SAFE_DELETE(m_pWINNER);
	SAFE_DELETE(m_pDRAW);
	SAFE_DELETE(m_pTEXT);
}

void CTTournamentResult::SetInfo(
	DWORD dwLeftCharID,
	DWORD dwRightCharID,
	CString strLeft,
	CString strRight,
	DWORD dwLeftKillCount,
	DWORD dwRightKillCount,
	DWORD dwLeftHideTick,
	DWORD dwRightHideTick,
	BOOL bResult,
	BOOL bTrueIsLeftWinButRightWin )
{
	m_pNAME1->m_strText = strLeft;
	m_pNAME2->m_strText = strRight;
	m_pKILL1->m_strText.Format( "%d", dwLeftKillCount );
	m_pKILL2->m_strText.Format( "%d", dwRightKillCount );
	m_dwLeftCharID = dwLeftCharID;
	m_dwRightCharID = dwRightCharID;
	m_bResult = bResult;
	m_bTrueIsLeftWinButRightWin = bTrueIsLeftWinButRightWin;
	m_pTEXT->m_strText.Empty();
	m_bBeginDice = FALSE;
	m_dwTickDice = 0;
	m_dwPrevTickDice = 0;
	m_bDiceLeftTeam = 0;
	m_bDiceRightTeam = 0;

	if( dwLeftHideTick != 0 || dwRightHideTick != 0 )
	{
		m_pTICK1->m_strText = CTChart::Format(TSTR_TOURNAMENT_HIDETICK,
			(dwLeftHideTick/1000), (INT)(dwLeftHideTick%1000)/10 );

		m_pTICK2->m_strText = CTChart::Format(TSTR_TOURNAMENT_HIDETICK,
			(dwRightHideTick/1000), (INT)(dwRightHideTick%1000)/10 );

		m_pTXT_HIDE->m_strText = CTChart::LoadString( TSTR_TOURNAMENT_HIDETIME );
	}
	else
	{
		m_pTICK1->m_strText.Empty();
		m_pTICK2->m_strText.Empty();
		m_pTXT_HIDE->m_strText.Empty();
	}

	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
	CTClientChar* pLeftChar = &(CTClientGame::GetInstance()->m_LeftChar);
	CTClientChar* pRightChar = &(CTClientGame::GetInstance()->m_RightChar);

	SetCharacter(
		(pMainChar->m_dwID == m_dwLeftCharID) ? pMainChar : CTClientGame::GetInstance()->FindPlayer(m_dwLeftCharID),
		pLeftChar,
		m_bResult,
		m_bTrueIsLeftWinButRightWin);
    
	SetCharacter(
		(pMainChar->m_dwID == m_dwRightCharID) ? pMainChar : CTClientGame::GetInstance()->FindPlayer(m_dwRightCharID),
		pRightChar,
		m_bResult,
		!m_bTrueIsLeftWinButRightWin);
}

void CTTournamentResult::BeginSelectWinner(
	BYTE bLeftValue,
	BYTE bRightValue)
{
	LPTTOURNAMENTTEAM pLeftTeam = CTClientGame::GetInstance()->m_pTLeftTeam;
	LPTTOURNAMENTTEAM pRightTeam = CTClientGame::GetInstance()->m_pTRightTeam;

	if( pLeftTeam && !(pLeftTeam->m_vMembers.empty()) )
	{
		m_dwLeftCharID = pLeftTeam->m_vMembers[0]->m_dwCharID;
		m_pNAME1->m_strText = pLeftTeam->m_vMembers[0]->m_strName;
	}
	else
	{
		m_pNAME1->m_strText.Empty();
		m_dwLeftCharID = 0;
	}

	m_pKILL1->m_strText.Empty();
	m_pWL1->m_strText.Empty();
	m_pTICK1->m_strText.Empty();

	if( pRightTeam && !(pRightTeam->m_vMembers.empty()) )
	{
		m_dwRightCharID = pRightTeam->m_vMembers[0]->m_dwCharID;
		m_pNAME2->m_strText = pRightTeam->m_vMembers[0]->m_strName;
	}
	else
	{
		m_pNAME2->m_strText.Empty();
		m_dwRightCharID = 0;
	}

	m_pKILL2->m_strText.Empty();
	m_pWL2->m_strText.Empty();
	m_pTICK2->m_strText.Empty();

	m_bResult = 0;
	m_bTrueIsLeftWinButRightWin = 0;
	m_pTEXT->m_strText = CTChart::LoadString(TSTR_TOURNAMENT_DRAW_NOTICE);
	m_pTXT_HIDE->m_strText.Empty();
	m_bBeginDice = TRUE;
	m_dwTickDice = 0;
	m_dwPrevTickDice = 0;
	m_bDiceLeftTeam = bLeftValue;
	m_bDiceRightTeam = bRightValue;

	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
	CTClientChar* pLeftChar = &(CTClientGame::GetInstance()->m_LeftChar);
	CTClientChar* pRightChar = &(CTClientGame::GetInstance()->m_RightChar);

	SetCharacter(
		(pMainChar->m_dwID == m_dwLeftCharID) ? pMainChar : CTClientGame::GetInstance()->FindPlayer(m_dwLeftCharID),
		pLeftChar,
		0,
		m_bTrueIsLeftWinButRightWin);
    
	SetCharacter(
		(pMainChar->m_dwID == m_dwRightCharID) ? pMainChar : CTClientGame::GetInstance()->FindPlayer(m_dwRightCharID),
		pRightChar,
		0,
		!m_bTrueIsLeftWinButRightWin);
}

HRESULT CTTournamentResult::Render( DWORD dwTickCount )
{
	//Dist, Theta, Phi, vPosOffset.x, y, z
	static FLOAT DrawCamera[6][6] =
	{
		{ 1.300000f, 1.570796f, 0.000000f, -0.200000f, 0.000000f, 0.000000f }, //무승부 인간 남자
		{ 1.300000f, 1.570796f, 0.000000f, -0.110000f, 0.000000f, 0.000000f }, //무승부 인간 여자
		{ 1.900000f, 1.570796f, 0.000000f, -0.360000f, 0.000000f, 0.000000f }, //무승부 수인 남자
		{ 1.300000f, 1.570796f, 0.000000f, -0.190000f, 0.000000f, 0.000000f }, //무승부 수인 여자
		{ 1.200000f, 1.570796f, 0.000000f, -0.090000f, 0.150000f, 0.000000f }, //무승부 요정 남자
		{ 1.200000f, 1.570796f, 0.000000f, -0.090000f, 0.150000f, 0.000000f } //무승부 요정 여자
	};

	static FLOAT LeftWinCamera[6][6] =
	{
		{ 1.000000f, 1.710796f, -0.560000f, -0.200000f, 0.050000f, 0.000000f },//승자 인간 남자
		{ 0.900000f, 1.670797f, -0.460000f, -0.110000f, 0.060000f, 0.000000f },//승자 인간 여자
		{ 1.600000f, 1.790796f, -0.580000f, -0.370000f, 0.120000f, 0.000000f },//승자 수인 남자
		{ 1.000000f, 1.790796f, -0.640000f, -0.190000f, 0.000000f, 0.000000f },//승자 수인 여자
		{ 0.900000f, 1.910796f, -0.200000f, -0.090000f, 0.150000f, 0.000000f },//승자 요정 남자
		{ 0.900000f, 1.910796f, -0.200000f, -0.090000f, 0.150000f, 0.000000f }//승자 요정 여자
	};

	static FLOAT RightWinCamera[6][6] =
	{
		{ 1.100000f, 1.670796f, 0.360000f, -0.140000f, 0.050000f, 0.000000f }, //승자 인간 남자
		{ 0.900000f, 1.790796f, 0.380000f, -0.050000f, 0.060000f, 0.000000f }, //승자 인간 여자
		{ 1.600000f, 1.650796f, 0.100000f, -0.370000f, 0.140000f, 0.000000f }, //승자 수인 남자
		{ 1.000000f, 1.790796f, 0.140000f, -0.190000f, 0.080000f, 0.000000f }, //승자 수인 여자
		{ 0.900000f, 1.830796f, 0.360000f, -0.090000f, 0.150000f, 0.000000f }, //승자 요정 남자
		{ 0.900000f, 1.830796f, 0.360000f, -0.090000f, 0.150000f, 0.000000f } //승자 요정 여자
	};

	static FLOAT LeftLoseCamera[6][6] =
	{
		{ 1.100000f, 1.270796f, -0.600000f, -0.210000f, 0.110000f, 0.000000f }, //패자 인간 남자
		{ 1.000000f, 1.410797f, -0.420000f, -0.140000f, 0.130000f, 0.000000f }, //패자 인간 여자
		{ 1.800000f, 1.450796f, -0.500000f, -0.370000f, 0.190000f, 0.000000f }, //패자 수인 남자
		{ 1.200000f, 1.330796f, -0.500000f, -0.190000f, 0.140000f, 0.000000f }, //패자 수인 여자
		{ 1.200000f, 1.610796f, -0.440000f, -0.090000f, 0.260000f, 0.000000f }, //패자 요정 남자
		{ 1.200000f, 1.610796f, -0.440000f, -0.090000f, 0.260000f, 0.000000f } //패자 요정 여자
	};

	static FLOAT RightLoseCamera[6][6] = 
	{
		{ 1.200000f, 1.230796f, 0.120000f, -0.210000f, 0.110000f, 0.000000f }, //패자 인간 남자
		{ 1.000000f, 1.430797f, 0.120000f, -0.090000f, 0.130000f, 0.000000f }, //패자 인간 여자
		{ 1.800000f, 1.470796f, 0.100000f, -0.370000f, 0.190000f, 0.000000f }, //패자 수인 남자
		{ 1.200000f, 1.350796f, 0.080000f, -0.190000f, 0.140000f, 0.000000f }, //패자 수인 여자
		{ 1.200000f, 1.650796f, 0.380000f, -0.090000f, 0.260000f, 0.000000f }, //패자 요정 남자
		{ 1.200000f, 1.650796f, 0.380000f, -0.090000f, 0.260000f, 0.000000f } //패자 요정 여자
	};

	if( m_bBeginDice )
	{
		m_dwTickDice += dwTickCount;
		if( m_dwTickDice < 4000 )
		{
			if( m_dwPrevTickDice+100 < m_dwTickDice )
			{
				m_pWL1->m_strText.Format("%d", 1+(rand()%6) );
				m_dwPrevTickDice = m_dwTickDice;
			}
		}
		else if( 4000 <= m_dwTickDice && m_dwTickDice < 8000 )
		{
			m_pWL1->m_strText.Format("%d", m_bDiceLeftTeam);

			if( m_dwPrevTickDice+100 < m_dwTickDice )
			{
				m_pWL2->m_strText.Format("%d", 1+(rand()%6) );
				m_dwPrevTickDice = m_dwTickDice;
			}
		}
		else
		{
			m_bBeginDice = FALSE;
			m_pWL1->m_strText.Format("%d", m_bDiceLeftTeam);
			m_pWL2->m_strText.Format("%d", m_bDiceRightTeam);
		}
	}

	if(!m_bVisible)
		return S_OK;
	CTClientUIBase::Render(dwTickCount);

	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientChar* pMainChar = pGame->GetMainChar();
	CTClientChar* pLeftChar = &(pGame->m_LeftChar);
	CTClientChar* pRightChar = &(pGame->m_RightChar);

	if( pLeftChar->m_OBJ.m_pOBJ != NULL )
	{
		BYTE w = pLeftChar->GetRaceID() * 2 + pLeftChar->m_bSex;

		if( m_bResult && m_bTrueIsLeftWinButRightWin )
		{
			RenderCharacter(
				pLeftChar,
				m_pCAMERA_LEFT,
				LeftWinCamera[w][0],
				LeftWinCamera[w][1],
				LeftWinCamera[w][2],
				LeftWinCamera[w][3],
				LeftWinCamera[w][4],
				LeftWinCamera[w][5]);
		}
		else if( m_bResult && !m_bTrueIsLeftWinButRightWin )
		{
			RenderCharacter(
				pLeftChar,
				m_pCAMERA_LEFT,
				LeftLoseCamera[w][0],
				LeftLoseCamera[w][1],
				LeftLoseCamera[w][2],
				LeftLoseCamera[w][3],
				LeftLoseCamera[w][4],
				LeftLoseCamera[w][5]);
		}
		else
		{
			RenderCharacter(
				pLeftChar,
				m_pCAMERA_LEFT,
				DrawCamera[w][0],
				DrawCamera[w][1],
				DrawCamera[w][2],
				DrawCamera[w][3],
				DrawCamera[w][4],
				DrawCamera[w][5]);
		}
	}

	if( pRightChar->m_OBJ.m_pOBJ != NULL )
	{	
		BYTE w = pRightChar->GetRaceID() * 2 + pRightChar->m_bSex;

		if( m_bResult && m_bTrueIsLeftWinButRightWin )
		{
			RenderCharacter(
				pRightChar,
				m_pCAMERA_RIGHT,
				RightLoseCamera[w][0],
				RightLoseCamera[w][1],
				RightLoseCamera[w][2],
				RightLoseCamera[w][3],
				RightLoseCamera[w][4],
				RightLoseCamera[w][5]);
		}
		else if( m_bResult && !m_bTrueIsLeftWinButRightWin )
		{
			RenderCharacter(
				pRightChar,
				m_pCAMERA_RIGHT,
				RightWinCamera[w][0],
				RightWinCamera[w][1],
				RightWinCamera[w][2],
				RightWinCamera[w][3],
				RightWinCamera[w][4],
				RightWinCamera[w][5]);
		}
		else
		{
			RenderCharacter(
				pRightChar,
				m_pCAMERA_RIGHT,
				DrawCamera[w][0],
				DrawCamera[w][1],
				DrawCamera[w][2],
				DrawCamera[w][3],
				DrawCamera[w][4],
				DrawCamera[w][5]);
		}
	}

	if( m_bResult == TRUE )
	{
		CPoint pt;

		if( m_bTrueIsLeftWinButRightWin )
		{
			m_pWINNER_LEFT->GetComponentPos(&pt);
			m_pWINNER_LEFT->ComponentToScreen(&pt);
		}
		else
		{
			m_pWINNER_RIGHT->GetComponentPos(&pt);
			m_pWINNER_RIGHT->ComponentToScreen(&pt);
		}

		m_pWINNER->MoveComponent(pt);
		m_pWINNER->Render(dwTickCount);
	}
	else if( m_bResult == FALSE )
	{
		CPoint pt;
		GetComponentPos(&pt);
		
		CPoint ptDRAW(pt);
		CPoint ptTEXT(pt);
		
		ptDRAW += m_ptDRAW;
		ptTEXT += m_ptTEXT;

		m_pDRAW->MoveComponent(ptDRAW);
		m_pTEXT->MoveComponent(ptTEXT);
		
		m_pDRAW->Render(dwTickCount);
		m_pTEXT->Render(dwTickCount);
	}

	return S_OK;
}

void CTTournamentResult::RenderCharacter(
	CTClientChar* pTCHAR,
	TComponent* pTargetComp,
	FLOAT fDist,
	FLOAT fTheta,
	FLOAT fPhi,
	FLOAT vOffsetX,
	FLOAT vOffsetY,
	FLOAT vOffsetZ)
{
	D3DXVECTOR3 vWaist(pTCHAR->GetPosition());

	pTCHAR->ApplyMatrix(m_pDevice);

	MAPDWORD::iterator itID = pTCHAR->m_OBJ.m_pOBJ->m_mapPIVOT.find(ID_PIVOT_WAIST);
	if( itID != pTCHAR->m_OBJ.m_pOBJ->m_mapPIVOT.end() )
	{
		vWaist.x = pTCHAR->m_pBone[(*itID).second]._41;
		vWaist.y = pTCHAR->m_pBone[(*itID).second]._42;
		vWaist.z = pTCHAR->m_pBone[(*itID).second]._43;
	}

	D3DXVECTOR3 vOffset(vOffsetX, vOffsetY, vOffsetZ);
	D3DVIEWPORT9 vOLD;
	D3DVIEWPORT9 vNEW;

	CRect rect;
	pTargetComp->GetComponentRect(&rect);
	pTargetComp->ComponentToScreen(&rect);

	m_pDevice->m_pDevice->GetViewport(&vOLD);
	vNEW.Height = rect.Height() + min( 0, rect.top);
	vNEW.Width = rect.Width() + min( 0, rect.left);
	vNEW.MinZ = 0.0f;
	vNEW.MaxZ = 1.0f;
	vNEW.X = max( 0, rect.left);
	vNEW.Y = max( 0, rect.top);
	m_pDevice->m_pDevice->SetViewport(&vNEW);

	m_vCamera.InitCamera(
		m_pDevice->m_pDevice,
		0.1f,
		1000.0f,
		45.0f * D3DX_PI / 180.0f,
		vNEW.Width,
		vNEW.Height);

	D3DXVECTOR3 vCamTarget(vWaist);
	vCamTarget.y += vOffset.y;
	D3DXVECTOR3 vCamPos;
	FLOAT phi = fPhi + -(FLOAT(pTCHAR->m_wDIR) * D3DX_PI / 900.0f) - D3DX_PI/2.0f;
	vCamPos.x = fDist * sinf(fTheta) * cosf(phi);
	vCamPos.y = fDist * cosf(fTheta);
	vCamPos.z = fDist * sinf(fTheta) * sinf(phi);
	vCamPos += vCamTarget;

	D3DXVECTOR3 vCamDir = vCamTarget - vCamPos;
	D3DXVec3Normalize(&vCamDir, &vCamDir);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 vCross;
	D3DXVec3Cross(&vCross, &vCamDir, &vUp);
	vCross *= vOffset.x;

	vCamPos += vCross;
	vCamTarget += vCross;

	m_vCamera.SetPosition(
		vCamPos,
		vCamTarget,
		D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	m_vCamera.Activate(TRUE);

	m_pDevice->m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_ZBUFFER,
		0, 1.0f, 0);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);

	pTCHAR->Render(
		m_pDevice,
		&m_vCamera);

	pTCHAR->RenderWeaponEffect(
		m_pDevice,
		&m_vCamera );

	CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);

	m_pDevice->m_pDevice->SetViewport(&vOLD);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

void CTTournamentResult::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonDown(nFlags, pt);
}

void CTTournamentResult::OnLButtonUp(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonUp(nFlags, pt);
}

void CTTournamentResult::OnMouseMove(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnMouseMove(nFlags, pt);
}

void CTTournamentResult::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	CTClientUIBase::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CTTournamentResult::DoMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return CTClientUIBase::DoMouseWheel(nFlags, zDelta, pt);
}

void CTTournamentResult::ShowComponent(BOOL bVisible)
{
	if( bVisible )
	{
		SIZE size;
		GetComponentSize(&size);
		CRect rtScreen = CTClientUIBase::GetScreenRect();

		CPoint pt;
		pt.x = rtScreen.right/2 - size.cx/2;
		pt.y = rtScreen.bottom/2 - size.cy/2;
		
		MoveComponent(pt);
	}
	else
	{
		CTClientChar* pLeftChar = &(CTClientGame::GetInstance()->m_LeftChar);
		CTClientChar* pRightChar = &(CTClientGame::GetInstance()->m_RightChar);
		pLeftChar->Release();
		pRightChar->Release();
		m_pWL1->m_strText.Empty();
		m_pWL2->m_strText.Empty();
	}

	CTClientUIBase::ShowComponent(bVisible);
}

void CTTournamentResult::SetCharacter(
	CTClientChar* pTCHAR,
	CTClientChar* pTARGET,
	BYTE bResult,
	BOOL bWin)
{
	pTARGET->Release();

	if( pTCHAR == NULL || pTARGET == NULL )
		return ;

	pTARGET->m_pHost = (CWnd*) CTClientGame::GetInstance()->GetMainWnd();
	pTARGET->m_bRaceID_ = pTCHAR->m_bRaceID_;
	pTARGET->m_bSex = pTCHAR->m_bSex;

	pTARGET->m_bPants = pTCHAR->m_bPants;
	pTARGET->m_bHair = pTCHAR->m_bHair;
	pTARGET->m_bFace = pTCHAR->m_bFace;
	pTARGET->m_bBody = pTCHAR->m_bBody;
	pTARGET->m_bHand = pTCHAR->m_bHand;
	pTARGET->m_bFoot = pTCHAR->m_bFoot;
	pTARGET->m_bHelmetHide = pTCHAR->m_bHelmetHide;

	pTARGET->InitOBJ(CTClientGame::GetInstance()->GetResource()->GetOBJ( CTChart::m_vTRACE[pTCHAR->GetRaceID()][pTCHAR->m_bSex]));
	pTARGET->m_fBreathHeight = pTCHAR->GetAttrFLOAT(ID_BREATH_HEIGHT);
	pTARGET->m_fJumpSpeed = pTCHAR->GetAttrFLOAT(ID_JUMP_SPEED);
	pTARGET->InitSIZE(
		pTCHAR->GetAttrFLOAT(ID_SIZE_X),
		pTCHAR->GetAttrFLOAT(ID_SIZE_Y),
		pTCHAR->GetAttrFLOAT(ID_SIZE_Z),
		0.0f, 0.0f);

	pTARGET->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pTARGET->m_wMoveDIR = MOVE_NONE;
	pTARGET->m_wPITCH = 0;
	pTARGET->m_wDIR = 0;
	pTARGET->m_nPITCH = 0;
	pTARGET->m_nDIR = 0;

	D3DXMatrixScaling(
		&pTARGET->m_vScaleRACE,
		CTChart::m_vTRACESCALE[pTCHAR->GetRaceID()][pTCHAR->m_bSex],
		CTChart::m_vTRACESCALE[pTCHAR->GetRaceID()][pTCHAR->m_bSex],
		CTChart::m_vTRACESCALE[pTCHAR->GetRaceID()][pTCHAR->m_bSex]);

	MAPTINVEN::iterator finder = pTCHAR->m_mapTINVEN.find(INVEN_EQUIP);
	if( finder != pTCHAR->m_mapTINVEN.end() )
	{
		pTARGET->m_mapTINVEN.insert( MAPTINVEN::value_type( INVEN_EQUIP, (*finder).second));
		pTARGET->ClearEquip();
		pTARGET->ResetEQUIP( m_pDevice, CTClientGame::GetInstance()->GetResource());
		pTARGET->m_mapTINVEN.clear();
	}

	pTARGET->m_bActionLock = FALSE;

	if( bResult )
	{
		int i = bWin ? 109 : 110;
		TACTION vActionID = pTARGET->FindActionID( i, WT_NORMAL);
		pTARGET->ClearAnimationID();
		pTARGET->SetAction( vActionID.m_dwActID, vActionID.m_dwAniID );
		pTARGET->m_bACTLevel = CTChart::GetTACTION( i )->m_bLevel + 1;
	}
	else
	{
		TACTION vActionID = pTARGET->FindActionID( 0, WT_NORMAL);
		pTARGET->SetAction( vActionID.m_dwActID, vActionID.m_dwAniID );
	}
}
