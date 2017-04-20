#include "stdafx.h"
#include "TExplorerWnd.h"
#include "TErrorCode.h"
#include "Resource.h"
#include ".\texplorergame.h"

LPDIRECT3DVERTEXBUFFER9 CTExplorerGame::m_pTRECTVB = NULL;

CString CTExplorerGame::m_strTCMD[TCMD_COUNT] = {
	CString("EXIT"),			// TCMD_EXIT
	CString("CH"),		// TCMD_LOADCHAR
	CString("L"),				// TCMD_LOADMAP
	CString("M"),				// TCMD_MOVE
	CString("FPS"),				// TCMD_FPS
	CString("MINIMAP"),			// TCMD_MINIMAP
	CString("S"),				// TCMD_SPEED
	CString("POS"),				// TCMD_POS
	CString("HELP"),			// TCMD_HELP
	CString("BUILD"),			// TCMD_BUILD
	CString("FP"),				// TCMD_FINDPATH
	CString("PATH"),			// TCMD_PATH
	CString("LIGHT"),			// TCMD_LIGHT
	CString("LAND"),			// TCMD_MAPCOLOR
	CString("SHADOW"),			// TCMD_SHADOW
	CString("MA"),				// TCMD_MATERIALA
	CString("MD"),				// TCMD_MATERIALD
	CString("LC"),				// TCMD_LANDCOLLISION
	CString("LM"),				// TCMD_LIGHTMODE
	CString("A"),				// TCMD_LIGHTA
	CString("D"),				// TCMD_LIGHTD
	CString("DIR"),				// TCMD_LIGHTDIR
	CString("PM"),				// TCMD_PATHMODE
	CString("DR"),				// TCMD_DELRANGE
	CString("IMPOSTER"),		// TCMD_IMPOSTER
	CString("SAVEIMPOSTER"),	// TCMD_SAVEIMPOSTER
	CString("FOGON"),			// TCMD_FOGON
	CString("FOGOFF"),			// TCMD_FOGOFF
};	

struct binary_draw : public binary_function< CTExplorerObjBase *, CTExplorerObjBase *, bool>
{
	bool operator () ( const CTExplorerObjBase * &_Left, const CTExplorerObjBase * &_Right) const
	{
		if( _Left->m_OBJ.m_pOBJ->m_bFileID > _Right->m_OBJ.m_pOBJ->m_bFileID )
			return true;
		else if( _Left->m_OBJ.m_pOBJ->m_bFileID < _Right->m_OBJ.m_pOBJ->m_bFileID )
			return false;

		return _Left->m_fZValue > _Right->m_fZValue ? true : false;
	};
};


CTExplorerGame::CTExplorerGame()
{
	m_pTCHATEDIT = NULL;
	m_pTCHAT = NULL;
	m_bChatON = FALSE;
	m_bUseImposter = FALSE;

	m_bPATHFLAG = TRUE;
	m_bLIGHT = TRUE;
	m_bFPS = TRUE;
	m_bPOS = TRUE;
	m_bFogRender = TRUE;

	m_pDevice = NULL;
	m_pCamera = NULL;

	m_pWorldmap = NULL;
	m_pTMinimap = NULL;
	m_pMainChar = NULL;
	m_pMainWnd = NULL;
	m_pRES = NULL;

	m_bMoveType = TMAINMOVE_NORMAL;
	m_bAutoRun = FALSE;
	m_bCamFIX = FALSE;

	m_bLandCollision = TRUE;
	m_bTLIGHTMode = TRUE;
	m_bShowUI = TRUE;

	m_fGlowRange = DEF_GLOW_RANGE;
	m_nCamZOOM = 0;
	m_wCamDIR = 0;

	m_dwActiveVolume = TBGM_VOLUME;
	m_bStopBGM = FALSE;
	m_dwBGM = ID_BGM_INTRO;

	m_bLButtonDown = FALSE;
	m_bRButtonDown = FALSE;

	m_dwLButtonTick = 0;
	m_dwRButtonTick = 0;
	m_dwTSHADOW = 0;

	m_dwTSKY = 0;
	m_wTSKY = 0;
	m_bTSKY = 0;

	ZeroMemory( &m_vNormal, sizeof(D3DMATERIAL9));
	ZeroMemory( &m_vShadow, sizeof(D3DMATERIAL9));

	m_vNormal.Ambient.a = 1.0f;
	m_vNormal.Ambient.r = 1.0f;
	m_vNormal.Ambient.g = 1.0f;
	m_vNormal.Ambient.b = 1.0f;
	m_vNormal.Diffuse.a = 1.0f;
	m_vNormal.Diffuse.r = 1.0f;
	m_vNormal.Diffuse.g = 1.0f;
	m_vNormal.Diffuse.b = 1.0f;

	m_vShadow.Ambient.a = 1.0f;
	m_vShadow.Ambient.r = 0.7f;
	m_vShadow.Ambient.g = 0.7f;
	m_vShadow.Ambient.b = 0.7f;
	m_vShadow.Diffuse.a = 1.0f;
	m_vShadow.Diffuse.r = 0.7f;
	m_vShadow.Diffuse.g = 0.7f;
	m_vShadow.Diffuse.b = 0.7f;
	m_fDeleteRange = 2.0f;

	m_vFARIMAGE[!m_bTSKY].m_dwSHADOWTick = TMAXSHADOW_TICK;
	m_vFARIMAGE[m_bTSKY].m_dwSHADOWTick = TMAXSHADOW_TICK;

	m_vFARIMAGE[!m_bTSKY].m_bZWrite = FALSE;
	m_vFARIMAGE[m_bTSKY].m_bZWrite = FALSE;

	m_vSKY[!m_bTSKY].m_dwSHADOWTick = TMAXSHADOW_TICK;
	m_vSKY[m_bTSKY].m_dwSHADOWTick = TMAXSHADOW_TICK;

	m_vSKY[!m_bTSKY].m_bZWrite = FALSE;
	m_vSKY[m_bTSKY].m_bZWrite = FALSE;
	m_vMAP.ReleaseMAP();
}

CTExplorerGame::~CTExplorerGame()
{
	ReleaseGame();
}

void CTExplorerGame::CalcSCENE( DWORD dwTick)
{
	D3DXVECTOR3 vPOS(
		m_pMainChar->m_vPosition._41,
		m_pMainChar->m_vPosition._42,
		m_pMainChar->m_vPosition._43);

	m_vMAP.CalcMAPOBJ(
		m_pDevice,
		m_pCamera,
		dwTick);

	switch(m_bMoveType)
	{
	case TMAINMOVE_CLICK	: CalcClickMove(dwTick); break;
	default					: CalcMainChar(dwTick); break;
	}

	m_pMainChar->CalcTick(
		&vPOS,
		m_pDevice,
		m_pRES,
		dwTick);

	if(m_bLandCollision)
	{
		D3DXVECTOR3 vWORLD(
			m_pMainChar->m_vWorld._41,
			m_pMainChar->m_vWorld._42,
			m_pMainChar->m_vWorld._43);

		switch(m_vMAP.CheckCollision( m_pMainChar, &vPOS))
		{
		case TCR_BACK		:
			{
				m_pMainChar->m_vWorld._41 = vWORLD.x - m_pMainChar->m_vPosition._41 + vPOS.x;
				m_pMainChar->m_vWorld._43 = vWORLD.z - m_pMainChar->m_vPosition._43 + vPOS.z;
			}
		case TCR_CORRECT	: m_pMainChar->CalcTick( &vPOS, m_pDevice, m_pRES, 0); break;
		}
	}

	LPTREGIONINFO pTREGION = m_pMainChar->m_pTREGION;
	m_pMainChar->CalcHeight(
		&vPOS,
		&m_vMAP,
		dwTick);
	if( m_pMainChar->m_pTREGION != pTREGION )
		OnRegionChanged();

	CTachyonSFX::CalcBUF(
		m_pDevice->m_pDevice,
		dwTick);
}

HRESULT CTExplorerGame::Render( DWORD dwTickCount)
{
	if(m_vMAP.m_pMAP)
	{
		DWORD dwTotal = dwTickCount;

		if( m_vKEY.m_vSTATE[TKEY_ROT] && m_vKEY.m_vTICK[TKEY_ROT] + CLICK_DELAY < m_vKEY.m_dwTick )
			m_pMainChar->m_nPITCH = INT(m_pCamera->m_wCamPITCH) - INT(m_pMainChar->m_wPITCH) - 150;

		m_vMAP.m_bHouseMesh = m_pMainChar->m_bHouseMesh;
		m_vMAP.m_dwHouseID = m_pMainChar->m_dwHouseID;

		while(dwTotal)
		{
			DWORD dwTick = min( dwTotal, TTICK_INTERVAL);

			CalcSCENE(dwTick);
			dwTotal -= dwTick;
		}
		FLOAT fWaterHeight = CalcCamera(dwTickCount);

		ResetVisibleOBJ(dwTickCount);
		CalcGlobalTEX(dwTickCount);
		CalcBGM(dwTickCount);

		CalcTSKY( fWaterHeight, dwTickCount);
		m_pDevice->BeginGLOWScene();

		m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF);
		DisableLIGHT();
		ResetSCENEConstant();

		CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
		m_vSKY[m_bTSKY].Render( m_pDevice, m_pCamera);
		m_vFARIMAGE[m_bTSKY].Render( m_pDevice, m_pCamera);
		if(!m_vSKY[!m_bTSKY].m_bHide)
		{
			SetMaterial(&m_vSKY[!m_bTSKY]);
			m_vSKY[!m_bTSKY].Render( m_pDevice, m_pCamera);
			m_vFARIMAGE[!m_bTSKY].Render( m_pDevice, m_pCamera);
			SetMaterial(NULL);
		}
		CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);

		ResetTFOG(fWaterHeight > m_pCamera->m_vPosition.y ? TRUE : FALSE);

		m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);

		BYTE bIsEXTVisible = m_vMAP.IsEXTVisible();
		if(bIsEXTVisible)
		{
			EnableLIGHT();
			RenderTSILHOUETTE();
		}

		DisableLIGHT();
		m_vMAP.RenderBSP(m_pCamera);
		DisableLIGHT();

		if(bIsEXTVisible)
			m_vMAP.Render(m_pDevice);

		SetAmbient( 0, 0, 0);
		EnableLIGHT();

		for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
			if(!m_vMAP.m_vDRAWBSP[i]->m_vTDRAW.empty())
			{
				DisableLIGHT();
				EnableLIGHT(m_vMAP.m_vDRAWBSP[i]);

				RenderTOBJ(&m_vMAP.m_vDRAWBSP[i]->m_vTDRAW);
			}

		DisableLIGHT();
		EnableLIGHT();

		RenderTOBJ(&m_vMAP.m_vTDRAW);

		m_pDevice->EndGLOWScene( m_pCamera, m_fGlowRange);

		for( i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
			RenderTTEXT(&m_vMAP.m_vDRAWBSP[i]->m_vTDRAW);
		RenderTTEXT(&m_vMAP.m_vTDRAW);

		if(m_bPATHFLAG)
			RenderTPATH();

		CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
		m_vMAP.RenderWATER(
			m_pDevice,
			m_pCamera);
		CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);

		CTachyonSFX::RenderBUF(
			m_pDevice,
			m_pCamera,
			CTExplorerObjBase::m_fCamDist);

		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);
	}

	if(m_bShowUI)
		return TFrame::Render(dwTickCount);
	else
		return S_OK;
}

void CTExplorerGame::ResetTSKY()
{
	FLOAT fUnitLength = FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
	BYTE bUnitX = BYTE(m_pMainChar->m_vPosition._41 / fUnitLength);
	BYTE bUnitZ = BYTE(m_pMainChar->m_vPosition._43 / fUnitLength);

	LPTSKYBOX pTFAR = CTChart::FindTSKYBOX(
		m_vMAP.m_pMAP->m_nUnitCountX,
		TSKYOBJ_FAR,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	LPTSKYBOX pTSKY = CTChart::FindTSKYBOX(
		m_vMAP.m_pMAP->m_nUnitCountX,
		TSKYOBJ_SKY,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	m_wTSKY = MAKEWORD( bUnitX, bUnitZ);
	m_bTSKY = 0;
	m_dwTSKY = 0;

	m_vFARIMAGE[m_bTSKY].Release();
	m_vSKY[m_bTSKY].Release();

	if(pTFAR)
	{
		m_vFARIMAGE[m_bTSKY].InitOBJ(m_pRES->GetOBJ(pTFAR->m_dwOBJ));
		m_vFARIMAGE[m_bTSKY].m_vScale._11 = TSKY_SCALE;
		m_vFARIMAGE[m_bTSKY].m_vScale._22 = TSKY_SCALE;
		m_vFARIMAGE[m_bTSKY].m_vScale._33 = TSKY_SCALE;
		m_vFARIMAGE[m_bTSKY].SetCloth(
			m_pDevice,
			pTFAR->m_dwCLK,
			pTFAR->m_dwCL,
			pTFAR->m_dwMESH);
		m_vFARIMAGE[m_bTSKY].ResetOBJPart(m_pDevice);

		m_vFARIMAGE[m_bTSKY].SetAction(
			pTFAR->m_dwACT,
			pTFAR->m_dwANI);

		m_vFARIMAGE[m_bTSKY].m_bZWrite = FALSE;
		m_vFARIMAGE[m_bTSKY].m_bHide = FALSE;
		m_vFARIMAGE[m_bTSKY].m_bAlpha = ALPHA_MAX;
	}

	if(pTSKY)
	{
		m_vSKY[m_bTSKY].InitOBJ(m_pRES->GetOBJ(pTSKY->m_dwOBJ));
		m_vSKY[m_bTSKY].m_vScale._11 = TSKY_SCALE;
		m_vSKY[m_bTSKY].m_vScale._22 = TSKY_SCALE;
		m_vSKY[m_bTSKY].m_vScale._33 = TSKY_SCALE;
		m_vSKY[m_bTSKY].SetCloth(
			m_pDevice,
			pTSKY->m_dwCLK,
			pTSKY->m_dwCL,
			pTSKY->m_dwMESH);
		m_vSKY[m_bTSKY].ResetOBJPart(m_pDevice);

		m_vSKY[m_bTSKY].SetAction(
			pTSKY->m_dwACT,
			pTSKY->m_dwANI);

		m_vSKY[m_bTSKY].m_bAlpha = ALPHA_MAX;
		m_vSKY[m_bTSKY].m_bHide = FALSE;
	}

	m_vFARIMAGE[!m_bTSKY].m_bHide = TRUE;
	m_vSKY[!m_bTSKY].m_bHide = TRUE;
}

void CTExplorerGame::ResetTFOG( BYTE bWATER)
{
	FLOAT fUnitLength = FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
	BYTE bUnitX = BYTE(m_pCamera->m_vTarget.x / fUnitLength);
	BYTE bUnitZ = BYTE(m_pCamera->m_vTarget.z / fUnitLength);

	FLOAT fPosX = m_pCamera->m_vTarget.x - FLOAT(bUnitX) * fUnitLength;
	FLOAT fPosZ = m_pCamera->m_vTarget.z - FLOAT(bUnitZ) * fUnitLength;

	TLIGHT vTLIGHT[TLIGHT_COUNT];
	TLIGHT vTSHADOW;
	TLIGHT vTMTRL;
	TLIGHT vTMAPCOLOR;
	TFOG vTFOG;

	LPTUNITFOG pTFOG = CTChart::FindTFOG(
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);
	LPTLIGHT pTLIGHT = NULL;

	if(pTFOG)
		memcpy( &vTFOG, pTFOG->m_pTGLOBAL, sizeof(TFOG));
	else
		memset( &vTFOG, 0, sizeof(TFOG));

	pTLIGHT = CTChart::FindTLIGHT(
		m_vMAP.m_pMAP->m_nUnitCountX,
		TLIGHTSET_MAPCOLOR, 0,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	if(pTLIGHT)
		memcpy( &vTMAPCOLOR, pTLIGHT, sizeof(TLIGHT));
	else
		memset( &vTMAPCOLOR, 0, sizeof(TLIGHT));

	pTLIGHT = CTChart::FindTLIGHT(
		m_vMAP.m_pMAP->m_nUnitCountX,
		TLIGHTSET_SHADOW, 0,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	if(pTLIGHT)
		memcpy( &vTSHADOW, pTLIGHT, sizeof(TLIGHT));
	else
		memset( &vTSHADOW, 0, sizeof(TLIGHT));

	pTLIGHT = CTChart::FindTLIGHT(
		m_vMAP.m_pMAP->m_nUnitCountX,
		TLIGHTSET_MTRL, 0,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	if(pTLIGHT)
		memcpy( &vTMTRL, pTLIGHT, sizeof(TLIGHT));
	else
		memset( &vTMTRL, 0, sizeof(TLIGHT));

	for( BYTE i=0; i<TLIGHT_COUNT; i++)
	{
		pTLIGHT = CTChart::FindTLIGHT(
			m_vMAP.m_pMAP->m_nUnitCountX,
			TLIGHTSET_MAIN, i,
			m_vMAP.m_wMapID,
			bUnitX,
			bUnitZ);

		if(pTLIGHT)
			memcpy( &vTLIGHT[i], pTLIGHT, sizeof(TLIGHT));
		else
			memset( &vTLIGHT[i], 0, sizeof(TLIGHT));
	}

	if( fPosX < TUNITLERP_LENGTH && bUnitX > 0 )
	{
		FLOAT fTIME = (TUNITLERP_LENGTH - fPosX) / (2.0f * TUNITLERP_LENGTH);

		for( i=0; i<TLIGHT_COUNT; i++)
			LerpTLIGHT( &vTLIGHT[i], TLIGHTSET_MAIN, i, bUnitX - 1, bUnitZ, fTIME);

		LerpTLIGHT( &vTMAPCOLOR, TLIGHTSET_MAPCOLOR, 0, bUnitX - 1, bUnitZ, fTIME);
		LerpTLIGHT( &vTSHADOW, TLIGHTSET_SHADOW, 0, bUnitX - 1, bUnitZ, fTIME);
		LerpTLIGHT( &vTMTRL, TLIGHTSET_MTRL, 0, bUnitX - 1, bUnitZ, fTIME);

		LerpTFOG( &vTFOG, bUnitX - 1, bUnitZ, fTIME);
	}
	else if( fPosX > fUnitLength - TUNITLERP_LENGTH && bUnitX < m_vMAP.m_pMAP->m_nUnitCountX - 1 )
	{
		FLOAT fTIME = (fPosX - fUnitLength + TUNITLERP_LENGTH) / (2.0f * TUNITLERP_LENGTH);

		for( i=0; i<TLIGHT_COUNT; i++)
			LerpTLIGHT( &vTLIGHT[i], TLIGHTSET_MAIN, i, bUnitX + 1, bUnitZ, fTIME);

		LerpTLIGHT( &vTMAPCOLOR, TLIGHTSET_MAPCOLOR, 0, bUnitX + 1, bUnitZ, fTIME);
		LerpTLIGHT( &vTSHADOW, TLIGHTSET_SHADOW, 0, bUnitX + 1, bUnitZ, fTIME);
		LerpTLIGHT( &vTMTRL, TLIGHTSET_MTRL, 0, bUnitX + 1, bUnitZ, fTIME);

		LerpTFOG( &vTFOG, bUnitX + 1, bUnitZ, fTIME);
	}

	if( fPosZ < TUNITLERP_LENGTH && bUnitZ > 0 )
	{
		FLOAT fTIME = (TUNITLERP_LENGTH - fPosZ) / (2.0f * TUNITLERP_LENGTH);

		for( i=0; i<TLIGHT_COUNT; i++)
			LerpTLIGHT( &vTLIGHT[i], TLIGHTSET_MAIN, i, bUnitX, bUnitZ - 1, fTIME);

		LerpTLIGHT( &vTMAPCOLOR, TLIGHTSET_MAPCOLOR, 0, bUnitX, bUnitZ - 1, fTIME);
		LerpTLIGHT( &vTSHADOW, TLIGHTSET_SHADOW, 0, bUnitX, bUnitZ - 1, fTIME);
		LerpTLIGHT( &vTMTRL, TLIGHTSET_MTRL, 0, bUnitX, bUnitZ - 1, fTIME);

		LerpTFOG( &vTFOG, bUnitX, bUnitZ - 1, fTIME);
	}
	else if( fPosZ > fUnitLength - TUNITLERP_LENGTH && bUnitZ < m_vMAP.m_pMAP->m_nUnitCountZ - 1 )
	{
		FLOAT fTIME = (fPosZ - fUnitLength + TUNITLERP_LENGTH) / (2.0f * TUNITLERP_LENGTH);

		for( i=0; i<TLIGHT_COUNT; i++)
			LerpTLIGHT( &vTLIGHT[i], TLIGHTSET_MAIN, i, bUnitX, bUnitZ + 1, fTIME);

		LerpTLIGHT( &vTMAPCOLOR, TLIGHTSET_MAPCOLOR, 0, bUnitX, bUnitZ + 1, fTIME);
		LerpTLIGHT( &vTSHADOW, TLIGHTSET_SHADOW, 0, bUnitX, bUnitZ + 1, fTIME);
		LerpTLIGHT( &vTMTRL, TLIGHTSET_MTRL, 0, bUnitX, bUnitZ + 1, fTIME);

		LerpTFOG( &vTFOG, bUnitX, bUnitZ + 1, fTIME);
	}

	if(pTFOG)
		for( i=0; i<BYTE(pTFOG->m_vTLOCAL.size()); i++)
		{
			FLOAT fLocalX = pTFOG->m_vTLOCAL[i]->m_fPosX - FLOAT(bUnitX) * fUnitLength;
			FLOAT fLocalZ = pTFOG->m_vTLOCAL[i]->m_fPosZ - FLOAT(bUnitZ) * fUnitLength;

			FLOAT fLength = D3DXVec2Length(&D3DXVECTOR2(
				fLocalX - fPosX,
				fLocalZ - fPosZ));

			if( fLength < pTFOG->m_vTLOCAL[i]->m_fRADIUS )
			{
				memcpy( &vTFOG, pTFOG->m_vTLOCAL[i], sizeof(TFOG));
				break;
			}
			else if( fLength < pTFOG->m_vTLOCAL[i]->m_fRANGE )
			{
				FLOAT fTIME = (pTFOG->m_vTLOCAL[i]->m_fRANGE - fLength) / (pTFOG->m_vTLOCAL[i]->m_fRANGE - pTFOG->m_vTLOCAL[i]->m_fRADIUS);
				LerpTFOG( &vTFOG, pTFOG->m_vTLOCAL[i], fTIME);
			}
		}

	if(bWATER)
	{
		pTFOG = CTChart::FindTFOG( 0xFFFF, 0, 0);

		if(pTFOG)
		{
			vTFOG.m_bRValue = pTFOG->m_pTGLOBAL->m_bRValue;
			vTFOG.m_bGValue = pTFOG->m_pTGLOBAL->m_bGValue;
			vTFOG.m_bBValue = pTFOG->m_pTGLOBAL->m_bBValue;

			vTFOG.m_fDENSITY = max( vTFOG.m_fDENSITY, pTFOG->m_pTGLOBAL->m_fDENSITY);
			vTFOG.m_fSTART = min( vTFOG.m_fSTART, pTFOG->m_pTGLOBAL->m_fSTART);
			vTFOG.m_fEND = min( vTFOG.m_fEND, pTFOG->m_pTGLOBAL->m_fEND);
		}
	}

	for( i=0; i<TLIGHT_COUNT; i++)
	{
		if(m_bTLIGHTMode)
		{
			memset( &m_vLIGHT[i].m_Light, 0, sizeof(D3DLIGHT9));

			m_vLIGHT[i].m_Light.Type = D3DLIGHT_DIRECTIONAL;
			m_vLIGHT[i].m_Light.Direction.x = vTLIGHT[i].m_vDIR.x;
			m_vLIGHT[i].m_Light.Direction.y = vTLIGHT[i].m_vDIR.y;
			m_vLIGHT[i].m_Light.Direction.z = vTLIGHT[i].m_vDIR.z;

			m_vLIGHT[i].m_Light.Ambient.r = vTLIGHT[i].m_fAmbientR;
			m_vLIGHT[i].m_Light.Ambient.g = vTLIGHT[i].m_fAmbientG;
			m_vLIGHT[i].m_Light.Ambient.b = vTLIGHT[i].m_fAmbientB;
			m_vLIGHT[i].m_Light.Ambient.a = 1.0f;

			m_vLIGHT[i].m_Light.Diffuse.r = vTLIGHT[i].m_fDiffuseR;
			m_vLIGHT[i].m_Light.Diffuse.g = vTLIGHT[i].m_fDiffuseG;
			m_vLIGHT[i].m_Light.Diffuse.b = vTLIGHT[i].m_fDiffuseB;
			m_vLIGHT[i].m_Light.Diffuse.a = 1.0f;
			m_vLIGHT[i].SetLight(&m_vLIGHT[i].m_Light);
		}

		m_vLIGHT[i].EnableLight();
	}

	if( m_bTLIGHTMode )
	{
		m_vMAP.m_dwTMapCOLOR = D3DCOLOR_ARGB(
			0xFF,
			BYTE(vTMAPCOLOR.m_fAmbientR * 0xFF),
			BYTE(vTMAPCOLOR.m_fAmbientG * 0xFF),
			BYTE(vTMAPCOLOR.m_fAmbientB * 0xFF));
		m_dwTMapCOLOR = m_vMAP.m_dwTMapCOLOR;
	}
	else
	{
		m_vMAP.m_dwTMapCOLOR = m_dwTMapCOLOR;
	}

	if( m_bTLIGHTMode )
	{
		m_vMAP.m_bSHADOW = BYTE(vTSHADOW.m_fAmbientR * 0xFF);
	}

	if( m_bTLIGHTMode )
	{
		m_vShadow.Ambient.r = vTMTRL.m_fAmbientR;
		m_vShadow.Ambient.g = vTMTRL.m_fAmbientG;
		m_vShadow.Ambient.b = vTMTRL.m_fAmbientB;
		m_vShadow.Diffuse.r = vTMTRL.m_fDiffuseR;
		m_vShadow.Diffuse.g = vTMTRL.m_fDiffuseG;
		m_vShadow.Diffuse.b = vTMTRL.m_fDiffuseB;
	}

	if( !m_bFogRender )
		return ;

	if(m_pDevice->m_vCAPS.RasterCaps & D3DPRASTERCAPS_WFOG)
	{
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGTABLEMODE, vTFOG.m_bType);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	}
	else
	{
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGVERTEXMODE, vTFOG.m_bType);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_NONE);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_RANGEFOGENABLE, FALSE);
	}

	m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGCOLOR, D3DCOLOR_ARGB( 0xFF, vTFOG.m_bRValue, vTFOG.m_bGValue, vTFOG.m_bBValue));
	m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, TRUE);

	switch(vTFOG.m_bType)
	{
	case D3DFOG_LINEAR	:
		{
			m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGSTART, *(LPDWORD) (&vTFOG.m_fSTART));
			m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGEND, *(LPDWORD) (&vTFOG.m_fEND));
		}

		break;

	default				: m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGDENSITY, *(LPDWORD) (&vTFOG.m_fDENSITY)); break;
	}
}

void CTExplorerGame::LerpTLIGHT( LPTLIGHT pTLIGHT,
								 BYTE bSetID,
								 BYTE bIndex,
								 BYTE bUnitX,
								 BYTE bUnitZ,
								 FLOAT fTIME)
{
	LPTLIGHT pTBLEND = CTChart::FindTLIGHT(
		m_vMAP.m_pMAP->m_nUnitCountX,
		bSetID,
		bIndex,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	if(pTBLEND)
	{
		pTLIGHT->m_fAmbientR = FLOAT(pTLIGHT->m_fAmbientR) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fAmbientR) * fTIME;
		pTLIGHT->m_fAmbientG = FLOAT(pTLIGHT->m_fAmbientG) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fAmbientG) * fTIME;
		pTLIGHT->m_fAmbientB = FLOAT(pTLIGHT->m_fAmbientB) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fAmbientB) * fTIME;

		pTLIGHT->m_fDiffuseR = FLOAT(pTLIGHT->m_fDiffuseR) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fDiffuseR) * fTIME;
		pTLIGHT->m_fDiffuseG = FLOAT(pTLIGHT->m_fDiffuseG) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fDiffuseG) * fTIME;
		pTLIGHT->m_fDiffuseB = FLOAT(pTLIGHT->m_fDiffuseB) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fDiffuseB) * fTIME;
	}
}

void CTExplorerGame::LerpTFOG( LPTFOG pTFOG,
							   BYTE bUnitX,
							   BYTE bUnitZ,
							   FLOAT fTIME)
{
	if( !m_bFogRender )
		return ;

	LPTUNITFOG pTBLEND = CTChart::FindTFOG(
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	if(pTBLEND)
		LerpTFOG( pTFOG, pTBLEND->m_pTGLOBAL, fTIME);
}

void CTExplorerGame::LerpTFOG( LPTFOG pTFOG,
							   LPTFOG pTBLEND,
							   FLOAT fTIME)
{
	pTFOG->m_bRValue = BYTE(FLOAT(pTFOG->m_bRValue) * (1.0f - fTIME) + FLOAT(pTBLEND->m_bRValue) * fTIME);
	pTFOG->m_bGValue = BYTE(FLOAT(pTFOG->m_bGValue) * (1.0f - fTIME) + FLOAT(pTBLEND->m_bGValue) * fTIME);
	pTFOG->m_bBValue = BYTE(FLOAT(pTFOG->m_bBValue) * (1.0f - fTIME) + FLOAT(pTBLEND->m_bBValue) * fTIME);

	pTFOG->m_fDENSITY = FLOAT(pTFOG->m_fDENSITY) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fDENSITY) * fTIME;
	pTFOG->m_fSTART = FLOAT(pTFOG->m_fSTART) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fSTART) * fTIME;
	pTFOG->m_fEND = FLOAT(pTFOG->m_fEND) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fEND) * fTIME;
}

void CTExplorerGame::ResetVisibleOBJ( DWORD dwTick)
{
	m_vMAP.ResetTMAP(
		m_pDevice,
		m_pCamera,
		dwTick);

	for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
		m_vMAP.m_vDRAWBSP[i]->m_vTDRAW.clear();
	m_vMAP.m_vTDRAW.clear();

	for( i=0; i<INT(m_vMAP.m_vDRAWOBJ.size()); i++)
		if(m_vMAP.m_vDRAWOBJ[i]->m_bVisible)
			PushDrawOBJ(m_vMAP.m_vDRAWOBJ[i]);
		else
			m_vMAP.m_vDRAWOBJ[i]->CalcOBJSFX(m_pDevice);

	m_pMainChar->ResetVisible( m_pCamera, dwTick);
	PushDrawOBJ(m_pMainChar);

	for( i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
		m_vMAP.m_vDRAWBSP[i]->m_vTDRAW.sort(binary_draw());
	m_vMAP.m_vTDRAW.sort(binary_draw());
}

void CTExplorerGame::PushDrawOBJ( CTExplorerObjBase *pTOBJ)
{
	if( pTOBJ->m_bType != OT_BSP )
	{
		BYTE bNodeOBJ = FALSE;

		for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
			if( pTOBJ->m_dwHouseID == m_vMAP.m_vDRAWBSP[i]->m_dwHouseID )
			{
				DWORD dwNodeID = m_vMAP.m_vDRAWBSP[i]->GetNodeID(D3DXVECTOR3(
					pTOBJ->m_vPosition._41,
					pTOBJ->m_vPosition._42,
					pTOBJ->m_vPosition._43));

				if( dwNodeID != NODEID_NULL )
				{
					if((m_vMAP.m_dwBspID == NODEID_NULL || m_vMAP.m_dwBspID == m_vMAP.m_vDRAWBSP[i]->m_dwBspID ||
						m_vMAP.m_dwHouseID == m_vMAP.m_vDRAWBSP[i]->m_dwHouseID) &&
						m_vMAP.m_vDRAWBSP[i]->m_mapPVS.find(dwNodeID) != m_vMAP.m_vDRAWBSP[i]->m_mapPVS.end() )
					{
						m_vMAP.m_vDRAWBSP[i]->m_vTDRAW.push_back(pTOBJ);
						return;
					}

					bNodeOBJ = TRUE;
				}
			}

		if(bNodeOBJ)
			return;
	}

	m_vMAP.m_vTDRAW.push_back(pTOBJ);
}

void CTExplorerGame::RenderTOBJ( LPLISTTOBJBASE pLIST)
{
	LISTTOBJBASE::iterator itTOBJ;

	CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
	for( itTOBJ = pLIST->begin(); itTOBJ != pLIST->end(); itTOBJ++)
		if(!(*itTOBJ)->m_bDrawShadow)
			RenderTOBJ(*itTOBJ);
	CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);

	CTExplorerObjBase::BeginDrawSquare(m_pDevice);
	SetMaterial(NULL);
	CTExplorerObjBase::m_pTSHADOW->m_dwCurTick = m_dwTSHADOW;

	UVKEY vKEY = CTExplorerObjBase::m_pTSHADOW->GetUV();
	FLOAT fDepthBias = TOBJSHADOW_BIAS;
	D3DXMATRIX vUV;

	m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
	m_pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xC0000000);
	m_pDevice->m_pDevice->SetTexture( 0, CTExplorerObjBase::m_pTSHADOW->GetTexture());
	CTMath::GetUVMatrix( &vUV, &vKEY);

	for( itTOBJ = pLIST->begin(); itTOBJ != pLIST->end(); itTOBJ++)
		RenderTSQUARE( (*itTOBJ), &vUV, NULL, TOBJSHADOW_SCALE);

	CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
	for( itTOBJ = pLIST->begin(); itTOBJ != pLIST->end(); itTOBJ++)
		if((*itTOBJ)->m_bDrawShadow)
			RenderTOBJ(*itTOBJ);
	CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);
	fDepthBias = EXTBIAS_BASE;

	m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
	SetMaterial(NULL);
}

void CTExplorerGame::RenderTOBJ( CTExplorerObjBase *pTOBJ)
{
	pTOBJ->m_bInSHADOW = m_vMAP.PtInSHADOW(
		pTOBJ->m_vPosition._41,
		pTOBJ->m_vPosition._43) ? TRUE : FALSE;
	
	SetMaterial(pTOBJ);

	// 임시 imposter test
	if( m_bUseImposter && 
		(pTOBJ->m_bType == OT_NPC && pTOBJ->m_fCamDIST > 15.0f) || 
		(pTOBJ->m_bType == OT_NONE && pTOBJ->m_fCamDIST > 25.0f && pTOBJ->m_fRadius < 2.0f && pTOBJ->m_fSizeY < 2.0f) )
	{
		DWORD dwAmbient = D3DCOLOR_ARGB(
			(DWORD)(m_vNormal.Ambient.a * 255.0f),
			(DWORD)(m_vNormal.Ambient.r * 255.0f),
			(DWORD)(m_vNormal.Ambient.g * 255.0f),
			(DWORD)(m_vNormal.Ambient.b * 255.0f));
	}
	else
	{
		if( pTOBJ->m_bAlpha < 0xFF )
		{
			m_pDevice->m_pDevice->SetRenderState( D3DRS_COLORWRITEENABLE, 0);
			pTOBJ->Render( m_pDevice, m_pCamera);
			pTOBJ->m_bZWrite = FALSE;

			m_pDevice->m_pDevice->SetRenderState(
				D3DRS_COLORWRITEENABLE,
				D3DCOLORWRITEENABLE_ALPHA|
				D3DCOLORWRITEENABLE_RED|
				D3DCOLORWRITEENABLE_GREEN|
				D3DCOLORWRITEENABLE_BLUE);
		}

		pTOBJ->Render( m_pDevice, m_pCamera);
		pTOBJ->m_bZWrite = TRUE;
	}
}

void CTExplorerGame::RenderTPATH()
{
	for( int i=0; i<INT(m_vMAP.m_vTPATH.size()); i++)
		m_vMAP.m_vTPATH[i]->Render(m_pDevice);
}

void CTExplorerGame::RenderTSILHOUETTE()
{
	CTExplorerCAM vTCAM;

	vTCAM.InitCamera(
		m_pDevice->m_pDevice,
		m_pCamera->m_fFarPlane - m_pCamera->m_fFarPlane / 5.0f,
		2.0f * m_pCamera->m_fFarPlane,
		m_pCamera->m_fFOV,
		INT(m_pCamera->m_fWidth),
		INT(m_pCamera->m_fHeight));

	vTCAM.SetPosition(
		m_pCamera->m_vPosition,
		m_pCamera->m_vTarget,
		m_pCamera->m_vUp,
		FALSE);
	vTCAM.Activate(TRUE);

	CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
	for( BYTE i=0; i<9; i++)
		if(m_vMAP.m_pMAP->m_pUNIT[i])
			for( WORD j=0; j<m_vMAP.m_vTSILHOUETTE[m_vMAP.m_pMAP->m_bUNIT[i]].m_wCount; j++)
				if(m_vMAP.m_vTSILHOUETTE[m_vMAP.m_pMAP->m_bUNIT[i]].m_vTOBJ[j])
					m_vMAP.m_vTSILHOUETTE[m_vMAP.m_pMAP->m_bUNIT[i]].m_vTOBJ[j]->Render( m_pDevice, &vTCAM);

	CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);
	m_pCamera->Activate(TRUE);
}

void CTExplorerGame::RenderTTEXT( LPLISTTOBJBASE pLIST)
{
	LISTTOBJBASE::iterator itTOBJ;
	D3DXMATRIX vWORLD;

	D3DXMatrixRotationY(
		&vWORLD,
		atan2f( m_pMainChar->m_vPosition._41 - m_pCamera->m_vPosition.x, m_pMainChar->m_vPosition._43 - m_pCamera->m_vPosition.z));

	for( itTOBJ = pLIST->begin(); itTOBJ != pLIST->end(); itTOBJ++)
		if((*itTOBJ)->m_bDrawName)
			RenderTTEXT( &vWORLD, *itTOBJ);
}

void CTExplorerGame::RenderTTEXT( LPD3DXMATRIX pWORLD, CTExplorerObjBase *pTOBJ)
{
	FLOAT fLength = D3DXVec2Length(&D3DXVECTOR2(
		pTOBJ->m_vPosition._41 - m_pCamera->m_vPosition.x,
		pTOBJ->m_vPosition._43 - m_pCamera->m_vPosition.z));
	FLOAT fPosY = pTOBJ->m_vPosition._42 + pTOBJ->m_fSizeY;

	D3DXVECTOR3 vDIR = m_pCamera->m_vPosition - m_pCamera->m_vTarget;
	D3DXMATRIX vWORLD;
	D3DXPLANE vPLANE;

	D3DXMatrixRotationX(
		&vWORLD,
		atan2f( m_pCamera->m_vPosition.y - fPosY, fLength));
	vWORLD *= (*pWORLD);

	vWORLD._41 = pTOBJ->m_vPosition._41;
	vWORLD._42 = fPosY;
	vWORLD._43 = pTOBJ->m_vPosition._43;
	vDIR /= D3DXVec3Length(&vDIR);

	D3DXPlaneFromPointNormal(
		&vPLANE,
		&D3DXVECTOR3(
		vWORLD._41,
		vWORLD._42,
		vWORLD._43),
		&vDIR);

	fLength = 2.0f * tanf(m_pCamera->m_fFOV / 2.0f) * D3DXPlaneDotCoord(
		&vPLANE,
		&m_pCamera->m_vPosition) / FLOAT(m_pDevice->m_option.m_dwScreenY);
	vWORLD._42 += fLength * TTEXT_HEIGHT;

	if(pTOBJ->m_bDrawName)
	{
		CString strNAME = pTOBJ->GetName();

		pTOBJ->m_vNAME.m_dwLineColor = 0xFF000000;
		pTOBJ->m_vNAME.m_dwColor = 0xFFFFFF00;

		pTOBJ->m_vNAME.TextOut(
			m_pDevice->m_pDevice,
			vWORLD,
			strNAME,
			FLOAT(pTOBJ->m_vNAME.GetHeight()) * fLength,
			DT_CENTER);
		vWORLD._42 += fLength * (FLOAT(pTOBJ->m_vNAME.GetHeight()) + TTEXT_HEIGHT);
	}
}

void CTExplorerGame::RenderTSQUARE( CTExplorerObjBase *pTOBJ,
								    LPD3DXMATRIX pUV,
									LPD3DXVECTOR2 pDIR,
									FLOAT fSCALE)
{
	if(!pTOBJ->m_bDrawShadow)
		return;

	D3DXMATRIX vMAT(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	if( pTOBJ->m_bLand && m_vMAP.m_pMAP )
	{
		FLOAT fPosX = 2.0f * FLOAT(INT(pTOBJ->m_vPosition._41) / 2) - m_vMAP.m_pMAP->m_nTileLength;
		FLOAT fPosZ = 2.0f * FLOAT(INT(pTOBJ->m_vPosition._43) / 2) - m_vMAP.m_pMAP->m_nTileLength;

		m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vMAT);

		vMAT._11 = FLOAT(3 * m_vMAP.m_pMAP->m_nTileLength) / (fSCALE * pTOBJ->m_fSizeX);
		vMAT._22 = FLOAT(3 * m_vMAP.m_pMAP->m_nTileLength) / (fSCALE * pTOBJ->m_fSizeZ);
		vMAT._31 = vMAT._11 * (fPosX - pTOBJ->m_vPosition._41) / FLOAT(3 * m_vMAP.m_pMAP->m_nTileLength) + 0.5f;
		vMAT._32 = vMAT._22 * (fPosZ - pTOBJ->m_vPosition._43) / FLOAT(3 * m_vMAP.m_pMAP->m_nTileLength) + 0.5f;
		vMAT = (*pUV) * vMAT;

		if(pDIR)
		{
			D3DXMATRIX vROT;

			D3DXMatrixTransformation2D(
				&vROT,
				NULL,
				0.0f,
				NULL,
				&D3DXVECTOR2( 0.5f, 0.5f),
				atan2f( pDIR->x, pDIR->y) + D3DX_PI,
				NULL);

			vROT._31 = vROT._41;
			vROT._32 = vROT._42;
			vROT._41 = 0.0f;
			vROT._42 = 0.0f;

			m_pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &(vMAT * vROT));
		}
		else
			m_pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &vMAT);

		for( int i=0; i<3; i++)
		{
			fPosX = 2.0f * FLOAT(INT(pTOBJ->m_vPosition._41) / 2) - m_vMAP.m_pMAP->m_nTileLength;

			for( int j=0; j<3; j++)
			{
				PVERTEX vDATA[4] = {
					{								 fPosX, m_vMAP.m_pMAP->GetHeight(								 fPosX, pTOBJ->m_vPosition._42,								   fPosZ),								  fPosZ,	 FLOAT(j) / 3.0f,	  FLOAT(i) / 3.0f},
					{								 fPosX, m_vMAP.m_pMAP->GetHeight(								 fPosX, pTOBJ->m_vPosition._42, fPosZ + m_vMAP.m_pMAP->m_nTileLength), fPosZ + m_vMAP.m_pMAP->m_nTileLength,	 FLOAT(j) / 3.0f, FLOAT(i + 1) / 3.0f},
					{ fPosX + m_vMAP.m_pMAP->m_nTileLength, m_vMAP.m_pMAP->GetHeight( fPosX + m_vMAP.m_pMAP->m_nTileLength, pTOBJ->m_vPosition._42,								   fPosZ),								  fPosZ, FLOAT(j + 1) / 3.0f,	  FLOAT(i) / 3.0f},
					{ fPosX + m_vMAP.m_pMAP->m_nTileLength, m_vMAP.m_pMAP->GetHeight( fPosX + m_vMAP.m_pMAP->m_nTileLength, pTOBJ->m_vPosition._42, fPosZ + m_vMAP.m_pMAP->m_nTileLength), fPosZ + m_vMAP.m_pMAP->m_nTileLength, FLOAT(j + 1) / 3.0f, FLOAT(i + 1) / 3.0f}};

				m_pDevice->m_pDevice->DrawPrimitiveUP(
					D3DPT_TRIANGLESTRIP, 2,
					vDATA,
					sizeof(PVERTEX));
				fPosX += m_vMAP.m_pMAP->m_nTileLength;
			}

			fPosZ += m_vMAP.m_pMAP->m_nTileLength;
		}
	}
	else
	{
		m_pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, pUV);

		D3DXMatrixScaling(
			&vMAT,
			fSCALE * pTOBJ->m_fSizeX,
			1.0f,
			fSCALE * pTOBJ->m_fSizeZ);

		if(pDIR)
		{
			D3DXMATRIX vROT;

			D3DXMatrixRotationY(
				&vROT,
				atan2f( pDIR->x, pDIR->y) + D3DX_PI);
			vMAT *= vROT;
		}

		vMAT._41 = pTOBJ->m_vPosition._41;
		vMAT._42 = pTOBJ->m_fSquareHeight;
		vMAT._43 = pTOBJ->m_vPosition._43;

		m_pDevice->m_pDevice->SetStreamSource( 0, CTExplorerObjBase::m_pTSQUARE, 0, sizeof(PVERTEX));
		m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vMAT);
		m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

void CTExplorerGame::CalcClickMove( DWORD dwTick)
{
	D3DXVECTOR3 vPOS(
		m_pMainChar->m_vPosition._41,
		m_pMainChar->m_vPosition._42,
		m_pMainChar->m_vPosition._43);
	m_pMainChar->CancelTAction();

	if( D3DXPlaneDotCoord( &m_vBOUND, &vPOS) < 0.0f )
	{
		m_bMoveType = TMAINMOVE_NORMAL;
		CalcMainChar(dwTick);
	}
}

void CTExplorerGame::CalcMainChar( DWORD dwTick)
{
	static DWORD dwPrevCell = 0;
	static DWORD dwSYNC = 0;

	static int nPITCH = 0;
	static int nDIR = 0;

	int nDirX = 1;
	int nDirZ = 1;

	if( m_vKEY.m_vSTATE[TKEY_FORWARD] || m_bAutoRun )
		nDirZ--;

	if(m_vKEY.m_vSTATE[TKEY_BACK])
		nDirZ++;

	if(m_vKEY.m_vSTATE[TKEY_RIGHT])
		nDirX++;

	if(m_vKEY.m_vSTATE[TKEY_LEFT])
		nDirX--;

	nPITCH += m_pMainChar->m_nPITCH;
	nDIR += m_pMainChar->m_nDIR;
	dwSYNC += dwTick;

	BYTE bMouseDIR = nDirZ * 3 + (m_vKEY.m_vSTATE[TKEY_ROT] ? nDirX : 1);
	BYTE bKeyDIR = nDirZ * 3 + (m_vKEY.m_vSTATE[TKEY_ROT] ? 1 : nDirX);

	if( bMouseDIR != TKDIR_N )
		m_pMainChar->CancelTAction();

	BYTE bAction = m_pMainChar->GetTAction(bMouseDIR);
	DWORD dwCell = MAKELONG(
		WORD(m_pMainChar->m_vPosition._41 / CELL_SIZE),
		WORD(m_pMainChar->m_vPosition._43 / CELL_SIZE));

	if( bMouseDIR != m_pMainChar->m_bMouseDIR || bKeyDIR != m_pMainChar->m_bKeyDIR ||
		dwPrevCell != dwCell || abs(nDIR) > DIR_RANGE || abs(nPITCH) > DIR_RANGE ||
		dwSYNC > SYNC_TICK || bAction != m_pMainChar->m_bAction )
	{
		int nNextPITCH = INT(m_pMainChar->m_wPITCH) + m_pMainChar->m_nPITCH;
		int nNextDIR = INT(m_pMainChar->m_wDIR) + m_pMainChar->m_nDIR;

		if( bAction != m_pMainChar->m_bAction )
			m_pMainChar->CancelTAction();

		while(nNextPITCH < 0)
			nNextPITCH += 1800;
		nNextPITCH %= 1800;

		while(nNextDIR < 0)
			nNextDIR += 1800;
		nNextDIR %= 1800;

		m_pMainChar->SetTAction(bAction);
		dwPrevCell = dwCell;
		dwSYNC = 0;

		nPITCH = 0;
		nDIR = 0;
	}

	m_pMainChar->CalcDIR(
		dwTick,
		bMouseDIR,
		bKeyDIR);
}

FLOAT CTExplorerGame::CalcCamera( DWORD dwTick)
{
	int nCamRotX = 0;
	int nCamRotY = 0;
	int nCamZoom = 0;

	if(m_vKEY.m_vSTATE[TKEY_CAM_RIGHT])
		nCamRotY--;

	if(m_vKEY.m_vSTATE[TKEY_CAM_LEFT])
		nCamRotY++;

	if(m_vKEY.m_vSTATE[TKEY_CAM_DOWN])
		nCamRotX--;

	if(m_vKEY.m_vSTATE[TKEY_CAM_UP])
		nCamRotX++;

	if(m_vKEY.m_vSTATE[TKEY_ZOOM_IN])
		nCamZoom--;

	if(m_vKEY.m_vSTATE[TKEY_ZOOM_OUT])
		nCamZoom++;

	RotCameraX(nCamRotX * INT(dwTick * m_pCamera->m_bCamRotSPEED));
	RotCameraY(nCamRotY * INT(dwTick * m_pCamera->m_bCamRotSPEED));

	if( !nCamZoom && m_nCamZOOM )
	{
		DWORD dwZoomTick = abs(m_nCamZOOM);

		if( dwZoomTick > dwTick )
			dwZoomTick -= dwTick;

		nCamZoom = m_nCamZOOM < 0 ? -1 : 1;
		m_pCamera->ZoomCamera(
			dwZoomTick,
			nCamZoom);

		m_nCamZOOM -= nCamZoom * INT(dwZoomTick);
	}
	else
	{
		m_pCamera->ZoomCamera(
			dwTick,
			nCamZoom);

		m_nCamZOOM = 0;
	}

	if( !nCamRotX && !nCamRotY && !m_vKEY.m_vSTATE[TKEY_CAM_ROT] )
		FixCamera(dwTick);
	m_bCamFIX = m_pMainChar->m_wDIR == m_wCamDIR ? TRUE : FALSE;

	if( m_pCamera->m_fPrevDIST < m_pCamera->m_fCamDIST )
		m_pCamera->m_fPrevDIST = min( m_pCamera->m_fPrevDIST + TCAMOUT_SPEED * FLOAT(dwTick), m_pCamera->m_fCamDIST);

	m_pCamera->SetPosition(
		D3DXVECTOR3( 0.0f, 0.0f, m_pCamera->m_fPrevDIST),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f));

	m_pCamera->Rotate( FLOAT(m_wCamDIR) * D3DX_PI / 900.0f, 0.0f, 0.0f, TRUE);
	m_pCamera->Rotate( 0.0f, -FLOAT(m_pCamera->m_wCamPITCH) * D3DX_PI / 900.0f, 0.0f);
	m_pCamera->Move(
		m_pMainChar->m_vPosition._41 - m_pCamera->m_vTarget.x,
		m_pMainChar->m_vPosition._42 - m_pCamera->m_vTarget.y + m_pCamera->m_fCamHEIGHT,
		m_pMainChar->m_vPosition._43 - m_pCamera->m_vTarget.z, TRUE);
	FLOAT fWaterHeight = CheckCamPos();

	CD3DSound::ResetLISTENER(
		&m_pCamera->m_vPosition,
		&(m_pCamera->m_vTarget - m_pCamera->m_vPosition),
		&m_pCamera->m_vUp);

	return fWaterHeight;
}

BYTE CTExplorerGame::FixCamera( DWORD dwTick)
{
	BYTE bResult = m_wCamDIR == m_pMainChar->m_wDIR && !m_vKEY.m_vSTATE[TKEY_ROT] ? FALSE : TRUE;

	if( m_pMainChar->m_wMoveDIR == MOVE_NONE &&
		!m_vKEY.m_vSTATE[TKEY_ROT] )
		return FALSE;

	if( m_bMoveType != TMAINMOVE_NORMAL || m_vKEY.m_vTICK[TKEY_ROT] + CLICK_DELAY > m_vKEY.m_dwTick )
		return FALSE;

	if( m_vKEY.m_vSTATE[TKEY_ROT] && m_bCamFIX )
	{
		m_wCamDIR = m_pMainChar->m_wDIR;
		return bResult;
	}
	int nMOVE = INT(m_pMainChar->m_wDIR) - INT(m_wCamDIR);

	while(nMOVE < 0)
		nMOVE += 1800;
	nMOVE %= 1800;

	if( nMOVE > 900 )
		nMOVE -= 1800;

	int nROT = abs(nMOVE);
	if( nROT > dwTick )
		nROT = dwTick;

	nMOVE = INT(m_wCamDIR) + (nMOVE < 0 ? -nROT : nROT);
	while(nMOVE < 0)
		nMOVE += 1800;
	m_wCamDIR = WORD(nMOVE % 1800);

	return bResult;
}

FLOAT CTExplorerGame::CheckCamPos()
{
	D3DXVECTOR3 vTARGET = m_pCamera->m_vTarget;
	D3DXVECTOR3 vPOS = m_pCamera->m_vPosition;

	FLOAT fDIST = 0.0f;
	FLOAT fLEN = 0.0f;

	if(m_vMAP.HitTest( &fDIST, &vTARGET, &vPOS, 2.0f * m_pMainChar->m_fRadius, 2.0f * m_pMainChar->m_fRadius))
	{
		D3DXVECTOR3 vDIR = vTARGET - vPOS;

		fLEN = D3DXVec3Length(&vDIR);
		vPOS += (fLEN - max( m_pMainChar->m_fRadius, fDIST)) * vDIR / fLEN;
	}
	m_pCamera->m_fPrevDIST = D3DXVec3Length(&(vTARGET - vPOS));

	FLOAT fWaterHeight = FLOAT(INT(100.0f * m_vMAP.GetWaterHeight(
		vPOS.x,
		vPOS.z))) / 100.0f;

	FLOAT fLandHeight = FLOAT(INT(100.0f * m_vMAP.GetLandHeight(
		vPOS.x,
		vPOS.y,
		vPOS.z))) / 100.0f;

	if( fWaterHeight > fLandHeight && fabs(vPOS.y - fWaterHeight) < TCAM_SIZE / 2.0f )
		if( vPOS.y > fWaterHeight || fWaterHeight < fLandHeight + TCAM_SIZE )
			vPOS.y = fWaterHeight + TCAM_SIZE / 2.0f;
		else
			vPOS.y = fWaterHeight - TCAM_SIZE / 2.0f;

	D3DXVECTOR3 vDIR = vTARGET - vPOS;
	D3DXVECTOR3 vUP;

	vDIR /= D3DXVec3Length(&vDIR);
	D3DXVec3Cross(
		&vUP,
		&vDIR,
		&m_pCamera->m_vAxisX);
	vUP /= D3DXVec3Length(&vUP);

	m_pCamera->SetPosition(
		vPOS,
		vTARGET,
		vUP);
	m_pCamera->Activate(TRUE);

	vTARGET = D3DXVECTOR3(
		0.0f,
		m_pCamera->m_fCamHEIGHT,
		0.0f);

	CTMath::Transform(
		&m_pMainChar->m_vPosition,
		&vTARGET);

	if( D3DXVec3Length(&(vTARGET - vPOS)) < m_pCamera->m_fZoomMIN + THEIGHT_ZERO )
		m_pMainChar->m_bFPView = TRUE;
	else
		m_pMainChar->m_bFPView = FALSE;

	return fWaterHeight;
}

void CTExplorerGame::RotCameraX( int nCamRotX)
{
	nCamRotX += INT(m_pCamera->m_wCamPITCH);

	while(nCamRotX < 0)
		nCamRotX += 1800;

	m_pCamera->m_wCamPITCH = WORD(nCamRotX % 1800);
	if( m_pCamera->m_wCamPITCH > 900 )
		m_pCamera->m_wCamPITCH = max( TPITCH_MAX, m_pCamera->m_wCamPITCH);
	else
		m_pCamera->m_wCamPITCH = min( TPITCH_MIN, m_pCamera->m_wCamPITCH);
}

void CTExplorerGame::RotCameraY( int nCamRotY)
{
	nCamRotY += INT(m_wCamDIR);

	while(nCamRotY < 0)
		nCamRotY += 1800;

	m_wCamDIR = WORD(nCamRotY % 1800);
}

void CTExplorerGame::OnCommand( TCOMMAND cmd)
{
	int nERROR = FALSE;

	switch(cmd)
	{
	ON_GM_COMMAND(GM_MINIMAP_ZOOMOUT)
	ON_GM_COMMAND(GM_MINIMAP_ZOOMIN)

	ON_GM_COMMAND(GM_OPEN_WORLDMAP)
	ON_GM_COMMAND(GM_SELECT_WORLDMAP);
	ON_GM_COMMAND(GM_CLOSE_UI)
	}
}

void CTExplorerGame::OnCommand( DWORD dwCMD, CString strParam)
{
	DWORD dwERROR = TERROR_UNKNOWN_CMD;

	strParam.TrimLeft();
	switch(dwCMD)
	{
	ON_EXPLORER_CMD(TCMD_MATERIALA)
	ON_EXPLORER_CMD(TCMD_MATERIALD)
	ON_EXPLORER_CMD(TCMD_LOADCHAR)
	ON_EXPLORER_CMD(TCMD_MAPCOLOR)
	ON_EXPLORER_CMD(TCMD_FINDPATH)
	ON_EXPLORER_CMD(TCMD_LOADMAP)
	ON_EXPLORER_CMD(TCMD_MINIMAP)
	ON_EXPLORER_CMD(TCMD_SHADOW)
	ON_EXPLORER_CMD(TCMD_SPEED)
	ON_EXPLORER_CMD(TCMD_BUILD)
	ON_EXPLORER_CMD(TCMD_LIGHT)
	ON_EXPLORER_CMD(TCMD_PATH)
	ON_EXPLORER_CMD(TCMD_MOVE)
	ON_EXPLORER_CMD(TCMD_EXIT)
	ON_EXPLORER_CMD(TCMD_HELP)
	ON_EXPLORER_CMD(TCMD_FPS)
	ON_EXPLORER_CMD(TCMD_POS)

	ON_EXPLORER_CMD(TCMD_LANDCOLLISION)
	ON_EXPLORER_CMD(TCMD_LIGHTMODE)
	ON_EXPLORER_CMD(TCMD_LIGHTDIR)
	ON_EXPLORER_CMD(TCMD_LIGHTA)
	ON_EXPLORER_CMD(TCMD_LIGHTD)

	ON_EXPLORER_CMD(TCMD_PATHMODE)
	ON_EXPLORER_CMD(TCMD_DELRANGE)

	ON_EXPLORER_CMD(TCMD_IMPOSTER)
	ON_EXPLORER_CMD(TCMD_SAVEIMPOSTER)

	ON_EXPLORER_CMD(TCMD_FOGON)
	ON_EXPLORER_CMD(TCMD_FOGOFF)
	}

	if(dwERROR)
		OnERROR(dwERROR);
}

void CTExplorerGame::OnERROR( DWORD dwERROR)
{
	CString strTCMD;
	CString strTYPE;
	CString strTMSG;

	strTYPE.LoadString(IDS_TEXT_ERROR);
	switch(dwERROR)
	{
	case TERROR_LOADMAP_NOTFOUND	: strTCMD = m_strTCMD[TCMD_LOADMAP]; strTMSG.LoadString(IDS_ERROR_LOADMAP_NOTFOUND); break;
	case TERROR_UNKNOWN_CMD			: strTCMD.LoadString(IDS_TCMD_UNKNOWN); strTMSG.LoadString(IDS_ERROR_UNKNOWN_CMD); break;
	case TERROR_BUILD_NOMAP			: strTCMD = m_strTCMD[TCMD_BUILD]; strTMSG.LoadString(IDS_ERROR_BUILD_NOMAP); break;
	case TERROR_FINDPATH_NOMAP		: strTCMD = m_strTCMD[TCMD_FINDPATH]; strTMSG.LoadString(IDS_ERROR_FINDPATH_NOMAP); break;
	case TERROR_LOADMAP				: strTCMD = m_strTCMD[TCMD_LOADMAP]; strTMSG.LoadString(IDS_ERROR_LOADMAP); break;
	case TERROR_MOVE				: strTCMD = m_strTCMD[TCMD_MOVE]; strTMSG.LoadString(IDS_ERROR_MOVE); break;
	case TERROR_SPEED				: strTCMD = m_strTCMD[TCMD_SPEED]; strTMSG.LoadString(IDS_ERROR_SPEED); break;
	case TERROR_DELRANGE			: strTCMD = m_strTCMD[TCMD_DELRANGE]; strTMSG.LoadString(IDS_ERROR_DELRANGE); break;
	case TERROR_MAPCOLOR			: strTCMD = m_strTCMD[TCMD_MAPCOLOR]; strTMSG.LoadString(IDS_ERROR_MAPCOLOR); break;
	case TERROR_SHADOW				: strTCMD = m_strTCMD[TCMD_SHADOW]; strTMSG.LoadString(IDS_ERROR_SHADOW); break;
	case TERROR_AMBIENT				: strTCMD = m_strTCMD[TCMD_LIGHTA]; strTMSG.LoadString(IDS_ERROR_AMBIENT); break;
	case TERROR_DIFFUSE				: strTCMD = m_strTCMD[TCMD_LIGHTD]; strTMSG.LoadString(IDS_ERROR_DIFFUSE); break;
	case TERROR_LIGHTDIR			: strTCMD = m_strTCMD[TCMD_LIGHTDIR]; strTMSG.LoadString(IDS_ERROR_LIGHTDIR); break;
	case TERROR_MATERIALA			: strTCMD = m_strTCMD[TCMD_MATERIALA]; strTMSG.LoadString(IDS_ERROR_MATERIALA); break;
	case TERROR_MATERIALD			: strTCMD = m_strTCMD[TCMD_MATERIALD]; strTMSG.LoadString(IDS_ERROR_MATERIALD); break;
	}

	ChatMSG( strTYPE, strTCMD, strTMSG, 0xFFFFFFF0);
}

void CTExplorerGame::ReleaseGame()
{
	m_bLButtonDown = FALSE;
	m_bRButtonDown = FALSE;

	m_dwLButtonTick = 0;
	m_dwRButtonTick = 0;

	m_bAutoRun = FALSE;
	m_bCamFIX = FALSE;

	m_bLandCollision = TRUE;
	m_bTLIGHTMode = TRUE;

	m_nCamZOOM = 0;
	m_wCamDIR = 0;

	m_bMoveType = TMAINMOVE_NORMAL;
	m_bChatON = FALSE;
	m_dwTSHADOW = 0;

	m_dwTSKY = 0;
	m_wTSKY = 0;
	m_bTSKY = 0;

	m_pMainChar->m_bMode = MT_NORMAL;
	m_vKEY.ResetSTATE();
	m_vMAP.ReleaseMAP();

	CTExplorerBSP::ReleaseTBSPMAP();
}

void CTExplorerGame::OnActivateKEY( BYTE bSCAN)
{
	WORD wKEY = m_vKEY.GetTKEY(MAKEWORD( bSCAN, m_vKEY.GetCTRL()));

	if(!m_vKEY.m_vSTATE[wKEY])
	{
		m_vKEY.m_vTICK[wKEY] = m_vKEY.m_dwTick;
		m_vKEY.m_vSTATE[wKEY] = TRUE;
	}

	switch(wKEY)
	{
	case TKEY_CAM_LEFT			:
	case TKEY_CAM_RIGHT			:
	case TKEY_CAM_UP			:
	case TKEY_CAM_DOWN			:
	case TKEY_CAM_ROT			:
	case TKEY_ZOOM_IN			:
	case TKEY_ZOOM_OUT			: break;
	case TKEY_ROT				: m_bMoveType = TMAINMOVE_NORMAL; break;
	case TKEY_AUTORUN			:
		if(!m_bPATHMODE)
		{
			m_bMoveType = TMAINMOVE_NORMAL;
			m_bAutoRun = !m_bAutoRun;
		}
		else
			DeleteFlag();

		break;

	case TKEY_JUMP				:
		{
			m_pMainChar->CancelTAction();
			m_pMainChar->Jump();
		}

		break;

	case TKEY_FINDPATH			: OnTCMD_FINDPATH(_T("")); break;
	case TKEY_ADDFLAG			: AddFlag(); break;
	case TKEY_RUN				: m_pMainChar->m_bRun = !m_pMainChar->m_bRun; break;
	case TKEY_FORWARD			:
	case TKEY_BACK				: m_bAutoRun = FALSE; break;
	case TKEY_SHOW_UI			: m_bShowUI = !m_bShowUI;
	case TKEY_GETPOS				: WritePosToLogFile(); break;
	default						: m_bMoveType = TMAINMOVE_NORMAL; break;
	}
}

void CTExplorerGame::OnReleaseKEY( BYTE bSCAN)
{
	WORD wKEY = m_vKEY.GetTKEY(MAKEWORD( bSCAN, 0x00));

	if(m_vKEY.m_vSTATE[wKEY])
	{
		m_vKEY.m_vSTATE[wKEY] = FALSE;
		m_vKEY.m_vTICK[wKEY] = 0;
	}

	wKEY = m_vKEY.GetTKEY(MAKEWORD( bSCAN, 0x01));
	if(m_vKEY.m_vSTATE[wKEY])
	{
		m_vKEY.m_vSTATE[wKEY] = FALSE;
		m_vKEY.m_vTICK[wKEY] = 0;
	}
}

void CTExplorerGame::OnRegionChanged()
{
	m_dwActiveVolume = TBGM_VOLUME;
	m_bStopBGM = TRUE;
}

void CTExplorerGame::OnImeComposition( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CFrameGroup::OnImeComposition( hWnd, wParam, lParam);
}

void CTExplorerGame::OnChar( UINT nChar, int nRepCnt, UINT nFlags)
{
	WORD wKEY = m_vKEY.GetTKEY(MapVirtualKey( LOBYTE(nFlags), 1));

	if( wKEY == TKEY_CHAT )
		DoCHAT();

	CFrameGroup::OnChar( nChar, nRepCnt, nFlags);
}

void CTExplorerGame::OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags)
{
	if(!m_bChatON)
		OnActivateKEY(MapVirtualKey( LOBYTE(nFlags), 1));

	CFrameGroup::OnKeyDown( nChar, nRepCnt, nFlags);
}

void CTExplorerGame::OnKeyUp( UINT nChar, int nRepCnt, UINT nFlags)
{
	if(!m_bChatON)
		OnReleaseKEY(MapVirtualKey( LOBYTE(nFlags), 1));

	CFrameGroup::OnKeyUp( nChar, nRepCnt, nFlags);
}

void CTExplorerGame::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	m_nCamZOOM -= zDelta;
	CFrameGroup::DoMouseWheel( nFlags, zDelta, pt);
}

void CTExplorerGame::OnMouseMove( UINT nFlags, CPoint pt)
{
	CPoint point(
		m_pDevice->m_option.m_dwScreenX / 2,
		m_pDevice->m_option.m_dwScreenY / 2);
	BYTE bMOVE = FALSE;

	if( m_vKEY.m_vSTATE[TKEY_CAM_ROT] && m_vKEY.m_vTICK[TKEY_CAM_ROT] + CLICK_DELAY < m_vKEY.m_dwTick )
	{
		RotCameraX(pt.y - m_vKEY.m_point.y);
		RotCameraY(pt.x - m_vKEY.m_point.x);
		bMOVE = TRUE;
	}

	if( m_vKEY.m_vSTATE[TKEY_ROT] && m_vKEY.m_vTICK[TKEY_ROT] + CLICK_DELAY < m_vKEY.m_dwTick )
	{
		m_pMainChar->m_nDIR = pt.x - m_vKEY.m_point.x;
		RotCameraX(pt.y - m_vKEY.m_point.y);

		m_bMoveType = TMAINMOVE_NORMAL;
		bMOVE = TRUE;
	}

	if(bMOVE)
	{
		pt = point;
		ClientToScreen( m_pMainWnd->GetSafeHwnd(), &point);
		SetCursorPos( point.x, point.y);
	}
	m_vKEY.m_point = pt;

	CFrameGroup::OnMouseMove( nFlags, pt);
}

void CTExplorerGame::OnLButtonDown( UINT nFlags, CPoint pt)
{
	m_dwLButtonTick = m_vKEY.m_dwTick;
	m_bLButtonDown = TRUE;

	if(!HitTest(pt))
		OnActivateKEY(VK_LBUTTON);
	m_vKEY.m_point = pt;

	CFrameGroup::OnLButtonDown( nFlags, pt);
}

void CTExplorerGame::OnLButtonDblClk( UINT nFlags, CPoint pt)
{
	CFrameGroup::OnLButtonDblClk( nFlags, pt);
}

void CTExplorerGame::OnRButtonDown( UINT nFlags, CPoint pt)
{
	m_dwRButtonTick = m_vKEY.m_dwTick;
	m_bRButtonDown = TRUE;

	if(!HitTest(pt))
		OnActivateKEY(VK_RBUTTON);
	m_vKEY.m_point = pt;

	CFrameGroup::OnRButtonDown( nFlags, pt);
}

void CTExplorerGame::OnLButtonUp( UINT nFlags, CPoint pt)
{
	if( m_bLButtonDown && m_dwLButtonTick + CLICK_DELAY > m_vKEY.m_dwTick )
		OnClick( nFlags, pt);

	m_bLButtonDown = FALSE;
	m_dwLButtonTick = 0;
	OnReleaseKEY(VK_LBUTTON);

	CFrameGroup::OnLButtonUp( nFlags, pt);
}

void CTExplorerGame::OnRButtonUp( UINT nFlags, CPoint pt)
{
	if( m_bRButtonDown && m_dwRButtonTick + CLICK_DELAY > m_vKEY.m_dwTick )
		OnRClick( nFlags, pt);

	m_bRButtonDown = FALSE;
	m_dwRButtonTick = 0;
	OnReleaseKEY(VK_RBUTTON);

	CFrameGroup::OnRButtonUp( nFlags, pt);
}

void CTExplorerGame::OnClick( UINT nFlags, CPoint pt)
{
	if(!HitTest(pt))
	{
		D3DXVECTOR3 vTARGET = m_pCamera->m_vPosition + m_pCamera->GetRayDirection( pt.x, pt.y);
		D3DXVECTOR3 vSTART = m_pCamera->m_vPosition;
		D3DXVECTOR3 vDIR;
		D3DXPLANE vPLANE;

		D3DXPlaneFromPointNormal(
			&vPLANE,
			&D3DXVECTOR3( 0.0f, m_pMainChar->m_vPosition._42, 0.0f),
			&D3DXVECTOR3( 0.0f, 1.0f, 0.0f));

		if( CTMath::PlaneIntersectLine(
			&vDIR,
			&vSTART,
			&vTARGET,
			&vPLANE))
		{
			D3DXVECTOR3 vPOS(
				m_pMainChar->m_vPosition._41,
				m_pMainChar->m_vPosition._42,
				m_pMainChar->m_vPosition._43);

			if( D3DXVec3Dot( &(vTARGET - vSTART), &(vDIR - vSTART)) < 0.0f )
			{
				vDIR -= vPOS;
				vDIR = -vDIR;
			}
			else
				vDIR -= vPOS;

			WORD wDIR = m_pMainChar->GetTargetDIR(
				vDIR.x,
				vDIR.z);

			if( wDIR != 1800 )
			{
				D3DXMATRIX vROT;

				vPOS = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);
				D3DXMatrixRotationY(
					&vROT,
					FLOAT(wDIR) * D3DX_PI / 900);

				CTMath::Transform(
					&vROT,
					&vPOS);

				D3DXPlaneFromPoints(
					&m_vBOUND,
					&vSTART,
					&vTARGET,
					&(vTARGET + vPOS));

				m_pMainChar->m_bMouseDIR = TKDIR_F;
				m_pMainChar->m_bKeyDIR = TKDIR_F;
				m_pMainChar->SetTAction(m_pMainChar->GetTAction());
				m_pMainChar->CalcDIR( wDIR, wDIR);

				m_bMoveType = TMAINMOVE_CLICK;
				m_bAutoRun = FALSE;
			}
		}
	}
}

void CTExplorerGame::OnRClick( UINT nFlags, CPoint pt)
{
}

void CTExplorerGame::SetMaterial( CTExplorerObjBase *pTOBJ)
{
	static D3DMATERIAL9 vMAT = {
		{ 0.0f, 0.0f, 0.0f, 0.0f},
		{ 0.0f, 0.0f, 0.0f, 0.0f},
		{ 0.0f, 0.0f, 0.0f, 0.0f},
		{ 0.0f, 0.0f, 0.0f, 0.0f},
		0.0f};

	if(pTOBJ)
	{
		FLOAT fValue = FLOAT(pTOBJ->m_dwSHADOWTick) / FLOAT(TMAXSHADOW_TICK);

		vMAT.Ambient.r = m_vShadow.Ambient.r + fValue * (m_vNormal.Ambient.r - m_vShadow.Ambient.r);
		vMAT.Ambient.g = m_vShadow.Ambient.g + fValue * (m_vNormal.Ambient.g - m_vShadow.Ambient.g);
		vMAT.Ambient.b = m_vShadow.Ambient.b + fValue * (m_vNormal.Ambient.b - m_vShadow.Ambient.b);
		vMAT.Ambient.a = m_vShadow.Ambient.a + fValue * (m_vNormal.Ambient.a - m_vShadow.Ambient.a);

		vMAT.Diffuse.r = m_vShadow.Diffuse.r + fValue * (m_vNormal.Diffuse.r - m_vShadow.Diffuse.r);
		vMAT.Diffuse.g = m_vShadow.Diffuse.g + fValue * (m_vNormal.Diffuse.g - m_vShadow.Diffuse.g);
		vMAT.Diffuse.b = m_vShadow.Diffuse.b + fValue * (m_vNormal.Diffuse.b - m_vShadow.Diffuse.b);
		vMAT.Diffuse.a = FLOAT(pTOBJ->m_bAlpha) / FLOAT(ALPHA_MAX);
	}
	else
	{
		vMAT.Ambient.r = m_vNormal.Ambient.r;
		vMAT.Ambient.g = m_vNormal.Ambient.g;
		vMAT.Ambient.b = m_vNormal.Ambient.b;
		vMAT.Ambient.a = m_vNormal.Ambient.a;

		vMAT.Diffuse.r = m_vNormal.Diffuse.r;
		vMAT.Diffuse.g = m_vNormal.Diffuse.g;
		vMAT.Diffuse.b = m_vNormal.Diffuse.b;
		vMAT.Diffuse.a = m_vNormal.Diffuse.a;
	}

	if(m_pDevice->m_bEnableSHADER)
	{
		m_pDevice->m_pDevice->SetVertexShaderConstantF(
			m_pDevice->m_vConstantVS[VC_MTRLAMBIENT],
			(FLOAT *) &D3DXVECTOR4(
			vMAT.Ambient.r,
			vMAT.Ambient.g,
			vMAT.Ambient.b,
			vMAT.Ambient.a), 1);

		m_pDevice->m_pDevice->SetVertexShaderConstantF(
			m_pDevice->m_vConstantVS[VC_MTRLDIFFUSE],
			(FLOAT *) &D3DXVECTOR4(
			vMAT.Diffuse.r,
			vMAT.Diffuse.g,
			vMAT.Diffuse.b,
			vMAT.Diffuse.a), 1);
	}

	m_pDevice->m_pDevice->SetMaterial(&vMAT);
}

void CTExplorerGame::SetAmbient( BYTE bRValue,
								 BYTE bGValue,
								 BYTE bBValue)
{
	m_pDevice->m_pDevice->SetRenderState(
		D3DRS_AMBIENT,
		D3DCOLOR_ARGB(
		0xFF,
		bRValue,
		bGValue,
		bBValue));

	if(!m_pDevice->m_bEnableSHADER)
		return;

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_AMBIENT],
		(FLOAT *) &D3DXVECTOR4(
		FLOAT(bRValue) / 255.0f,
		FLOAT(bGValue) / 255.0f,
		FLOAT(bBValue) / 255.0f,
		1.0f), 1);
}

void CTExplorerGame::EnableLIGHT( CTExplorerBSP *pTBSP)
{
	MAPD3DLIGHT::iterator itLIGHT;
	VTLIGHT vTLIGHT;

	pTBSP->ResetTLIGHT();
	vTLIGHT.clear();

	for( itLIGHT = pTBSP->m_pMAP->m_mapLIGHT.begin(); itLIGHT != pTBSP->m_pMAP->m_mapLIGHT.end(); itLIGHT++)
	{
		BYTE bPUSH = TRUE;

		if( (*itLIGHT).second->m_Light.Type == D3DLIGHT_POINT )
		{
			DWORD dwNodeID = pTBSP->GetNodeID(D3DXVECTOR3(
				(*itLIGHT).second->m_Light.Position.x,
				(*itLIGHT).second->m_Light.Position.y,
				(*itLIGHT).second->m_Light.Position.z));

			if( dwNodeID != NODEID_NULL && pTBSP->m_mapPVS.find(dwNodeID) == pTBSP->m_mapPVS.end() )
				bPUSH = FALSE;
			else
				CTachyonMesh::m_bSoftwareVP = TRUE;
		}

		if(bPUSH)
		{
			(*itLIGHT).second->EnableLight(TRUE);
			vTLIGHT.push_back((*itLIGHT).second);
		}
	}

	if(!m_pDevice->m_bEnableSHADER)
	{
		vTLIGHT.clear();
		return;
	}

	int vLightCount[4] = { INT(vTLIGHT.size()), 0, 1, 0};
	FLOAT vCONST[1024];
	FLOAT *pCONST = vCONST;

	m_pDevice->m_pDevice->SetVertexShaderConstantI(
		m_pDevice->m_vConstantVS[VC_LIGHTCOUNT],
		vLightCount, 1);

	for( int i=0; i<vLightCount[0]; i++)
	{
		memcpy( pCONST, vTLIGHT[i]->m_Light.Type == D3DLIGHT_POINT ?
			&D3DXVECTOR4(
			1.0f / (vTLIGHT[i]->m_Light.Range * TLIGHT_SCALE * vTLIGHT[i]->m_Light.Range * TLIGHT_SCALE),
			0.0f,
			0.0f,
			0.0f) : &D3DXVECTOR4(
			vTLIGHT[i]->m_Light.Ambient.r,
			vTLIGHT[i]->m_Light.Ambient.g,
			vTLIGHT[i]->m_Light.Ambient.b,
			vTLIGHT[i]->m_Light.Ambient.a),
			4 * sizeof(FLOAT));
		pCONST += 4;
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTAMBIENT],
		vCONST, vLightCount[0]);
	pCONST = vCONST;

	for( i=0; i<vLightCount[0]; i++)
	{
		memcpy( pCONST, &D3DXVECTOR4(
			vTLIGHT[i]->m_Light.Diffuse.r,
			vTLIGHT[i]->m_Light.Diffuse.g,
			vTLIGHT[i]->m_Light.Diffuse.b,
			vTLIGHT[i]->m_Light.Diffuse.a),
			4 * sizeof(FLOAT));
		pCONST += 4;
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTDIFFUSE],
		vCONST, vLightCount[0]);
	pCONST = vCONST;

	for( i=0; i<vLightCount[0]; i++)
	{
		memcpy( pCONST, vTLIGHT[i]->m_Light.Type == D3DLIGHT_POINT ?
			&D3DXVECTOR4(
			vTLIGHT[i]->m_Light.Position.x,
			vTLIGHT[i]->m_Light.Position.y,
			vTLIGHT[i]->m_Light.Position.z,
			-1.0f) : &D3DXVECTOR4(
			vTLIGHT[i]->m_Light.Direction.x,
			vTLIGHT[i]->m_Light.Direction.y,
			vTLIGHT[i]->m_Light.Direction.z,
			1.0f),
			4 * sizeof(FLOAT));
		pCONST += 4;
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTDIR],
		vCONST, vLightCount[0]);
	vTLIGHT.clear();
}

void CTExplorerGame::EnableLIGHT()
{
	static int vLightCount[4] = { TLIGHT_COUNT, 0, 1, 0};

	for( int i=0; i<TLIGHT_COUNT; i++)
		m_vLIGHT[i].EnableLight();

	if(!m_pDevice->m_bEnableSHADER)
		return;

	FLOAT vCONST[16];
	FLOAT *pCONST = vCONST;

	m_pDevice->m_pDevice->SetVertexShaderConstantI(
		m_pDevice->m_vConstantVS[VC_LIGHTCOUNT],
		vLightCount, 1);

	for( i=0; i<TLIGHT_COUNT; i++)
	{
		memcpy( &vCONST[i * 4], &D3DXVECTOR4(
			m_vLIGHT[i].m_Light.Ambient.r,
			m_vLIGHT[i].m_Light.Ambient.g,
			m_vLIGHT[i].m_Light.Ambient.b,
			m_vLIGHT[i].m_Light.Ambient.a),
			4 * sizeof(FLOAT));
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTAMBIENT],
		vCONST, TLIGHT_COUNT);

	for( i=0; i<TLIGHT_COUNT; i++)
	{
		memcpy( &vCONST[i * 4], &D3DXVECTOR4(
			m_vLIGHT[i].m_Light.Diffuse.r,
			m_vLIGHT[i].m_Light.Diffuse.g,
			m_vLIGHT[i].m_Light.Diffuse.b,
			m_vLIGHT[i].m_Light.Diffuse.a),
			4 * sizeof(FLOAT));
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTDIFFUSE],
		vCONST, TLIGHT_COUNT);

	for( i=0; i<TLIGHT_COUNT; i++)
	{
		memcpy( &vCONST[i * 4], &D3DXVECTOR4(
			m_vLIGHT[i].m_Light.Direction.x,
			m_vLIGHT[i].m_Light.Direction.y,
			m_vLIGHT[i].m_Light.Direction.z,
			1.0f),
			4 * sizeof(FLOAT));
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTDIR],
		vCONST, TLIGHT_COUNT);
}

void CTExplorerGame::DisableLIGHT()
{
	static int vLightCount[4] = { 0, 0, 1, 0};

	for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
		m_vMAP.m_vDRAWBSP[i]->m_pMAP->DisableLight();

	for( i=0; i<TLIGHT_COUNT; i++)
		m_vLIGHT[i].EnableLight(FALSE);

	if(!m_pDevice->m_bEnableSHADER)
		return;

	m_pDevice->m_pDevice->SetVertexShaderConstantI(
		m_pDevice->m_vConstantVS[VC_LIGHTCOUNT],
		vLightCount, 1);
}

void CTExplorerGame::ResetSCENEConstant()
{
	m_pDevice->m_pDevice->SetMaterial(&m_vNormal);
	if(!m_pDevice->m_bEnableSHADER)
		return;

	FLOAT vCONST[16] = {
		1.0f,
		1.0f,
		m_vMAP.GetIntensity(),
		0.0f};

	m_pDevice->m_pDevice->SetPixelShaderConstantF(
		m_pDevice->m_vConstantPS[PC_ARG],
		vCONST, 1);

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_MTRLAMBIENT],
		(FLOAT *) &D3DXVECTOR4(
		m_vNormal.Ambient.r,
		m_vNormal.Ambient.g,
		m_vNormal.Ambient.b,
		m_vNormal.Ambient.a), 1);

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_MTRLDIFFUSE],
		(FLOAT *) &D3DXVECTOR4(
		m_vNormal.Diffuse.r,
		m_vNormal.Diffuse.g,
		m_vNormal.Diffuse.b,
		m_vNormal.Diffuse.a), 1);

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_AMBIENT],
		(FLOAT *) &D3DXVECTOR4(
		m_vMAP.GetSpecRange(),
		m_vMAP.GetSpecMax(),
		0.0f,
		0.0f), 1);

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_CAMPOS],
		(FLOAT *) &D3DXVECTOR4(
		m_pCamera->m_vPosition.x,
		m_pCamera->m_vPosition.y,
		m_pCamera->m_vPosition.z,
		0.0f), 1);

	D3DXMatrixTranspose( (LPD3DXMATRIX) vCONST, &(m_pCamera->m_matView * m_pCamera->m_matProjection));
	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_PROJ],
		vCONST, 4);
}

LRESULT CTExplorerGame::OnOBJMsg( LPOBJMSG pMSG)
{
	switch(pMSG->m_dwMSG)
	{
	case OM_TSUBACTENDING	:
		{
			CTExplorerObjBase *pTOBJ = (CTExplorerObjBase *) pMSG->m_pOBJ;
			pTOBJ->SetSubAction( 0, 0);
		}

		break;

	case OM_ACTENDING		:
		{
			CTExplorerObjBase *pTOBJ = (CTExplorerObjBase *) pMSG->m_pOBJ;

			if( pTOBJ == m_pMainChar )
				pTOBJ->m_bSTEPSND = TRUE;

			if( pTOBJ->m_bACTLevel != CTChart::GetTACTION( pTOBJ->m_bAction )->m_bLevel )
			{
				pTOBJ->m_bACTLevel = CTChart::GetTACTION( pTOBJ->m_bAction )->m_bLevel;
				return 1;
			}
		}

		break;
	}

	return 0;
}

void CTExplorerGame::SwitchFocus( TComponent *pCandidate)
{
	if( pCandidate != m_pWorldmap && m_pTCHATEDIT->IsVisible() )
		pCandidate = m_pTCHATEDIT;
	CFrameGroup::SwitchFocus(pCandidate);

	RemoveKid(m_pWorldmap);
	AddKid(m_pWorldmap);
}

void CTExplorerGame::EnableChat( BYTE bEnable)
{
	m_pTCHATEDIT->ShowComponent(bEnable);
	m_pTCHAT->ShowComponent(bEnable);
	SwitchFocus(bEnable ? m_pTCHATEDIT : NULL);

	m_pTCHATEDIT->ClearText();
	m_bChatON = bEnable;
}

void CTExplorerGame::DoCHAT()
{
	m_pTCHATEDIT->m_strText.TrimRight();
	m_pTCHATEDIT->m_strText.TrimLeft();

	if( !m_bChatON || m_pTCHATEDIT->m_strText.IsEmpty() )
		EnableChat(!m_bChatON);
	else
	{
		ParseCMD(m_pTCHATEDIT->m_strText);
		m_pTCHATEDIT->ClearText();
	}
}

CString CTExplorerGame::ChatMSG( CString strTYPE,
								 CString strNAME,
								 CString strMSG,
								 DWORD dwCOLOR)
{
	VECTORSTRING vTEXT;

	CString strRESULT;
	CString strFORMAT;
	CString strTEXT;

	CSize szITEM = m_pTCHAT->GetItemSize(1);
	CSize szTEXT;

	int nPOS = 0;

	szITEM.cx -= TCHAT_MARGINE;
	strRESULT.Empty();

	strTEXT = strMSG.Tokenize( "\n", nPOS);
	while(!strTEXT.IsEmpty())
	{
		strTEXT.Remove('\r');

		while(!strTEXT.IsEmpty())
		{
			m_pTCHAT->GetTextExtentPoint( LPCTSTR(strTEXT), szTEXT);
			CString strMBCS = BuildMBCSInfo(strTEXT);
			strFORMAT = strTEXT;

			while( szTEXT.cx > szITEM.cx )
			{
				int nLength = strFORMAT.GetLength() - 1;

				strFORMAT.Delete(nLength);
				nLength--;

				if( nLength >= 0 && IS_MBCS_LEAD(strMBCS.GetAt(nLength)) )
					strFORMAT.Delete(nLength);

				m_pTCHAT->GetTextExtentPoint( LPCTSTR(strFORMAT), szTEXT);
			}

			int nIndex = strTEXT.GetLength() - strFORMAT.GetLength();
			if( nIndex > 0 )
				strTEXT = strTEXT.Right(nIndex);
			else
				strTEXT.Empty();
			strFORMAT.TrimLeft(' ');

			if(!strRESULT.IsEmpty())
				strRESULT = strRESULT + CString("\n");
			strRESULT += strFORMAT;

			vTEXT.push_back(strFORMAT);
			strFORMAT.Empty();
		}

		strTEXT = strMSG.Tokenize( "\n", nPOS);
	}

	strTEXT.Format(
		IDS_FMT_CHAT_NAME,
		strTYPE,
		strNAME);

	m_pTCHAT->ShowComponent(TRUE);
	strFORMAT = strTEXT;

	for( BYTE i=0; i<INT(vTEXT.size()); i++)
	{
		int nIndex = m_pTCHAT->AddString(LPCTSTR(strFORMAT));

		m_pTCHAT->SetItemString( nIndex, 1, vTEXT[i]);
		m_pTCHAT->SetUserColor( nIndex, 0, dwCOLOR);
		m_pTCHAT->SetUserColor( nIndex, 1, dwCOLOR);
		strFORMAT.Empty();
	}

	while(m_pTCHAT->GetItemCount() > TCHAT_SIZE)
		m_pTCHAT->DelItem(0);

	m_pTCHAT->SetCurSelItem(max( m_pTCHAT->GetItemCount() - 1, 0));
	vTEXT.clear();

	return strRESULT;
}

void CTExplorerGame::ErrorMSG( DWORD dwTypeID,
							   DWORD dwTitleID,
							   DWORD dwMsgID,
							   DWORD dwCOLOR)
{
	CString strTITLE;
	CString strTYPE;
	CString strMSG;

	strTITLE.LoadString(dwTitleID);
	strTYPE.LoadString(dwTypeID);
	strMSG.LoadString(dwMsgID);

	ChatMSG( strTYPE, strTITLE, strMSG, dwCOLOR);
}

void CTExplorerGame::CalcBGM( DWORD dwTick)
{
	static DWORD dwBGMTick = 0;

	if(m_bStopBGM)
	{
		CT3DMusic *pMUSIC = (CT3DMusic *) CTachyonRes::m_MEDIA.GetData(
			MEDIA_TMUSIC,
			m_dwBGM);

		m_dwActiveVolume -= min( m_dwActiveVolume, dwTick);
		if(m_dwActiveVolume)
		{
			BYTE bFadeVolume = BYTE(VOLUME_MAX * m_dwActiveVolume / TBGM_VOLUME);

			if(pMUSIC)
			{
				pMUSIC->m_bFadeVolume = bFadeVolume;
				pMUSIC->ResetVolume();
			}
		}
		else
		{
			if(pMUSIC)
				pMUSIC->m_bFadeVolume = VOLUME_MAX;

			CTachyonRes::m_MEDIA.Stop(
				MEDIA_TMUSIC,
				m_dwBGM, 0);
			m_bStopBGM = FALSE;
		}
	}
	else if(!CTachyonRes::m_MEDIA.IsPlay( MEDIA_TMUSIC, m_dwBGM, 0))
	{
		dwBGMTick += dwTick;

		if( dwBGMTick > TBGM_TIMER )
		{
			if( m_pMainChar->m_pTREGION &&
				m_pMainChar->m_pTREGION->m_pVTBGM &&
				!m_pMainChar->m_pTREGION->m_pVTBGM->empty() &&
				!(rand() % TBGM_RATE) )
			{
				CTachyonRes::m_MEDIA.Stop(
					MEDIA_TMUSIC,
					m_dwBGM, 0);
					
				VECTORDWORD& vTBGM = *(m_pMainChar->m_pTREGION->m_pVTBGM);
				m_dwBGM = vTBGM[rand() % INT(vTBGM.size())];

				m_dwActiveVolume = TBGM_VOLUME;
				m_bStopBGM = FALSE;

				CTachyonRes::m_MEDIA.Play(
					MEDIA_TMUSIC,
					m_dwBGM);
			}

			dwBGMTick = 0;
		}
	}
}

void CTExplorerGame::CalcGlobalTEX( DWORD dwTick)
{
	if(CTExplorerObjBase::m_pTSHADOW)
	{
		m_dwTSHADOW += dwTick;
		m_dwTSHADOW %= CTExplorerObjBase::m_pTSHADOW->m_dwTotalTick;
	}
}

void CTExplorerGame::CalcTSKY( FLOAT fWaterHeight,
							   DWORD dwTick)
{
	m_vFARIMAGE[m_bTSKY].m_vWorld._41 = m_pMainChar->m_vPosition._41;
	m_vFARIMAGE[m_bTSKY].m_vWorld._42 = max( fWaterHeight, m_pMainChar->m_vPosition._42);
	m_vFARIMAGE[m_bTSKY].m_vWorld._43 = m_pMainChar->m_vPosition._43;

	m_vSKY[m_bTSKY].m_vWorld._41 = m_vFARIMAGE[m_bTSKY].m_vWorld._41;
	m_vSKY[m_bTSKY].m_vWorld._42 = m_vFARIMAGE[m_bTSKY].m_vWorld._42;
	m_vSKY[m_bTSKY].m_vWorld._43 = m_vFARIMAGE[m_bTSKY].m_vWorld._43;

	m_vFARIMAGE[m_bTSKY].CalcTick( m_pDevice->m_pDevice, dwTick);
	m_vSKY[m_bTSKY].CalcTick( m_pDevice->m_pDevice, dwTick);

	if(!m_vSKY[!m_bTSKY].m_bHide)
	{
		m_vFARIMAGE[!m_bTSKY].m_vWorld._41 = m_vFARIMAGE[m_bTSKY].m_vWorld._41;
		m_vFARIMAGE[!m_bTSKY].m_vWorld._42 = m_vFARIMAGE[m_bTSKY].m_vWorld._42;
		m_vFARIMAGE[!m_bTSKY].m_vWorld._43 = m_vFARIMAGE[m_bTSKY].m_vWorld._43;

		m_vSKY[!m_bTSKY].m_vWorld._41 = m_vSKY[m_bTSKY].m_vWorld._41;
		m_vSKY[!m_bTSKY].m_vWorld._42 = m_vSKY[m_bTSKY].m_vWorld._42;
		m_vSKY[!m_bTSKY].m_vWorld._43 = m_vSKY[m_bTSKY].m_vWorld._43;

		m_vFARIMAGE[!m_bTSKY].CalcTick( m_pDevice->m_pDevice, dwTick);
		m_vSKY[!m_bTSKY].CalcTick( m_pDevice->m_pDevice, dwTick);
	}

	FLOAT fUnitLength = FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
	BYTE bUnitX = BYTE(m_pMainChar->m_vPosition._41 / fUnitLength);
	BYTE bUnitZ = BYTE(m_pMainChar->m_vPosition._43 / fUnitLength);
	WORD wTSKY = MAKEWORD( bUnitX, bUnitZ);

	if( wTSKY != m_wTSKY )
		if(m_vSKY[!m_bTSKY].m_bHide)
		{
			LPTSKYBOX pTFAR = CTChart::FindTSKYBOX(
				m_vMAP.m_pMAP->m_nUnitCountX,
				TSKYOBJ_FAR,
				m_vMAP.m_wMapID,
				bUnitX,
				bUnitZ);

			LPTSKYBOX pTSKY = CTChart::FindTSKYBOX(
				m_vMAP.m_pMAP->m_nUnitCountX,
				TSKYOBJ_SKY,
				m_vMAP.m_wMapID,
				bUnitX,
				bUnitZ);

			m_vFARIMAGE[!m_bTSKY].Release();
			m_vSKY[!m_bTSKY].Release();

			if( pTFAR && pTSKY )
			{
				m_vFARIMAGE[!m_bTSKY].InitOBJ(m_pRES->GetOBJ(pTFAR->m_dwOBJ));
				m_vFARIMAGE[!m_bTSKY].m_vScale._11 = TSKY_SCALE;
				m_vFARIMAGE[!m_bTSKY].m_vScale._22 = TSKY_SCALE;
				m_vFARIMAGE[!m_bTSKY].m_vScale._33 = TSKY_SCALE;
				m_vFARIMAGE[!m_bTSKY].SetCloth(
					m_pDevice,
					pTFAR->m_dwCLK,
					pTFAR->m_dwCL,
					pTFAR->m_dwMESH);
				m_vFARIMAGE[!m_bTSKY].ResetOBJPart(m_pDevice);

				m_vFARIMAGE[!m_bTSKY].SetAction(
					pTFAR->m_dwACT,
					pTFAR->m_dwANI);

				m_vFARIMAGE[!m_bTSKY].m_bZWrite = FALSE;
				m_vFARIMAGE[!m_bTSKY].m_bHide = FALSE;
				m_vFARIMAGE[!m_bTSKY].m_bAlpha = 0;

				m_vSKY[!m_bTSKY].InitOBJ(m_pRES->GetOBJ(pTSKY->m_dwOBJ));
				m_vSKY[!m_bTSKY].m_vScale._11 = TSKY_SCALE;
				m_vSKY[!m_bTSKY].m_vScale._22 = TSKY_SCALE;
				m_vSKY[!m_bTSKY].m_vScale._33 = TSKY_SCALE;
				m_vSKY[!m_bTSKY].SetCloth(
					m_pDevice,
					pTSKY->m_dwCLK,
					pTSKY->m_dwCL,
					pTSKY->m_dwMESH);
				m_vSKY[!m_bTSKY].ResetOBJPart(m_pDevice);

				m_vSKY[!m_bTSKY].SetAction(
					pTSKY->m_dwACT,
					pTSKY->m_dwANI);

				m_vSKY[!m_bTSKY].m_bHide = FALSE;
				m_vSKY[!m_bTSKY].m_bAlpha = 0;

				m_vFARIMAGE[!m_bTSKY].CalcTick( m_pDevice->m_pDevice, 0);
				m_vSKY[!m_bTSKY].CalcTick( m_pDevice->m_pDevice, 0);
				m_dwTSKY = 0;
			}
			else
				m_wTSKY = wTSKY;
		}
		else
		{
			m_dwTSKY = min( m_dwTSKY + dwTick, TMAXSKY_TICK);

			m_vFARIMAGE[!m_bTSKY].m_bAlpha = BYTE(ALPHA_MAX * m_dwTSKY / TMAXSKY_TICK);
			m_vSKY[!m_bTSKY].m_bAlpha = m_vFARIMAGE[!m_bTSKY].m_bAlpha;

			if( m_vSKY[!m_bTSKY].m_bAlpha == ALPHA_MAX )
			{
				m_vFARIMAGE[m_bTSKY].m_bHide = TRUE;
				m_vFARIMAGE[m_bTSKY].m_bAlpha = 0;

				m_vSKY[m_bTSKY].m_bHide = TRUE;
				m_vSKY[m_bTSKY].m_bAlpha = 0;

				m_bTSKY = !m_bTSKY;
				m_wTSKY = wTSKY;
				m_dwTSKY = 0;
			}
		}
	else if(!m_vSKY[!m_bTSKY].m_bHide)
	{
		m_dwTSKY -= min( m_dwTSKY, dwTick);

		m_vFARIMAGE[!m_bTSKY].m_bAlpha = BYTE(ALPHA_MAX * m_dwTSKY / TMAXSKY_TICK);
		m_vSKY[!m_bTSKY].m_bAlpha = m_vFARIMAGE[!m_bTSKY].m_bAlpha;

		if(!m_vSKY[!m_bTSKY].m_bAlpha)
		{
			m_vFARIMAGE[!m_bTSKY].m_bHide = TRUE;
			m_vSKY[!m_bTSKY].m_bHide = TRUE;
		}
	}
}

void CTExplorerGame::ParseCMD( CString strCMD)
{
	int nPOS = 0;

	CString strID = strCMD.Tokenize( " ", nPOS);
	if(!strID.IsEmpty())
	{
		strID.MakeUpper();

		for( DWORD i=0; i<TCMD_COUNT; i++)
			if( strID == m_strTCMD[i] )
			{
				OnCommand( i, strCMD.Mid(strID.GetLength()));
				return;
			}

		OnCommand( TCMD_COUNT, CString(""));
	}
}

void CTExplorerGame::DeleteFlag()
{
	if( !m_vMAP.m_pMAP || !m_vMAP.m_pMAP->m_pUNIT[4] )
		return;

	BYTE bUnitX = BYTE(INT(m_pMainChar->m_vPosition._41) / (m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength));
	BYTE bUnitZ = BYTE(INT(m_pMainChar->m_vPosition._43) / (m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength));

	if(m_vMAP.m_pMAP->m_pENABLE[bUnitZ * m_vMAP.m_pMAP->m_nUnitCountX + bUnitX])
	{
		MAPTPATHFLAG::iterator itTPATH = CTExplorerFlag::m_mapTPATHFLAG.begin();

		while(itTPATH != CTExplorerFlag::m_mapTPATHFLAG.end())
		{
			MAPTPATHFLAG::iterator itNEXT = itTPATH;
			itNEXT++;

			if( HIWORD((*itTPATH).second->m_dwID) == MAKEWORD( bUnitX, bUnitZ) &&
				fabs((*itTPATH).second->m_vPOS.y - m_pMainChar->m_vPosition._42) < 1.0f &&
				D3DXVec2Length(&D3DXVECTOR2(
				(*itTPATH).second->m_vPOS.x - m_pMainChar->m_vPosition._41,
				(*itTPATH).second->m_vPOS.z - m_pMainChar->m_vPosition._43)) < m_fDeleteRange )
				delete (*itTPATH).second;

			itTPATH = itNEXT;
		}
	}
}

void CTExplorerGame::AddFlag()
{
	if( !m_vMAP.m_pMAP || !m_vMAP.m_pMAP->m_pUNIT[4] )
		return;

	BYTE bUnitX = BYTE(INT(m_pMainChar->m_vPosition._41) / (m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength));
	BYTE bUnitZ = BYTE(INT(m_pMainChar->m_vPosition._43) / (m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength));

	if(m_vMAP.m_pMAP->m_pENABLE[bUnitZ * m_vMAP.m_pMAP->m_nUnitCountX + bUnitX])
	{
		CTExplorerFlag *pTFLAG = new CTExplorerFlag();

		pTFLAG->m_vNAME.SetFont(CTExplorerObjBase::m_pFONT);
		pTFLAG->m_vPOS.x = m_pMainChar->m_vPosition._41;
		pTFLAG->m_vPOS.y = m_pMainChar->m_vPosition._42 + TPATH_HEIGHT;
		pTFLAG->m_vPOS.z = m_pMainChar->m_vPosition._43;

		CTExplorerFlag::RegTPATH(
			pTFLAG,
			bUnitX,
			bUnitZ);
	}
}
//==================================================
// 현재 캐릭터 위치의 x, y, z값을 파일에 쓴다.
//==================================================
void CTExplorerGame::WritePosToLogFile(void)
{
	float fPos_x = m_pMainChar->m_vPosition._41;
	float fPos_y = m_pMainChar->m_vPosition._42; 
	float fPos_z = m_pMainChar->m_vPosition._43;

	FILE *fp;
	CTime Time = CTime::GetCurrentTime();
	CString FileName;
	FileName.Format("PosLog%d_%d_%d.txt", Time.GetYear(), Time.GetMonth(), Time.GetDay());

	fp = fopen(FileName,"a");
	
	fprintf(fp, "%f,%f,%f,%d시%d분%d초\n", fPos_x, fPos_y, fPos_z, Time.GetHour(), Time.GetMinute(), Time.GetSecond());
	
	fclose(fp);
}
