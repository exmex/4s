#include "stdafx.h"
#include "TScenePlayer.h"
#include "TClientGame.h"

#define TINSTALL_ALPHA( dwColor, bAlpha)			DWORD(((DWORD(bAlpha) << 24) & 0xFF000000) | (DWORD(dwColor) & 0x00FFFFFF))


CTSpCode::CTSpCode()
{
	m_eCode = TSCENE_PLAYER_CODE_NONE;

	m_pTEX[0] = NULL;
	m_pTEX[1] = NULL;

	m_dwTexTick[0] = 0;
	m_dwTexTick[1] = 0;

	m_dwCOLOR = 0;
	m_dwTotal = 0;
	m_dwTick = 0;
	m_bTEXT = 0;
}

CTSpCode::~CTSpCode()
{
}

BYTE CTSpCode::CalcTick( CTClientGame *pTGAME, DWORD dwTick)
{
	m_dwTick -= min( m_dwTick, dwTick);

	switch(m_eCode)
	{
	case TSCENE_PLAYER_CODE_FADE_OUT_TEXTURE	:
	case TSCENE_PLAYER_CODE_FADE_IN_TEXTURE		:
	case TSCENE_PLAYER_CODE_DRAW_TEXTURE		: CalcTexTick( 0, dwTick); break;
	case TSCENE_PLAYER_CODE_FADE_TEXTURE		:
		for( BYTE i=0; i<2; i++)
			CalcTexTick( i, dwTick);

		break;

	case TSCENE_PLAYER_CODE_START_CAMERA		:
		{
			CTClientChar *pTCHAR = pTGAME->GetMainChar();
			CTClientCAM *pCAM = pTGAME->GetMainCam();

			if( pTCHAR && pCAM )
			{
				D3DXVECTOR3 vDIR = pTCHAR->m_bContryID == COUNTRY_CRAXION ? TSTART_C_CAM_TARGET - TSTART_C_CAM_POSITION : TSTART_D_CAM_TARGET - TSTART_D_CAM_POSITION;

				pCAM->m_wCamPITCH = WORD(CTMath::GetPITCH(&vDIR) * 5);
				pCAM->m_fPrevDIST = D3DXVec3Length(&vDIR);
				pCAM->m_fCamDIST = pCAM->m_fPrevDIST;

				pTGAME->m_wCamDIR = WORD(CTMath::GetYAW(&vDIR) * 5);
				vDIR = pTCHAR->GetPosition();
				vDIR.y += pCAM->m_fCamHEIGHT;

				pCAM->m_vTOFFSET = (pTCHAR->m_bContryID == COUNTRY_CRAXION ? TSTART_C_CAM_TARGET : TSTART_D_CAM_TARGET) - vDIR;
			}
		}

		break;

	case TSCENE_PLAYER_CODE_CALC_CAMERA			:
		{
			CTClientChar *pTCHAR = pTGAME->GetMainChar();
			CTClientCAM *pCAM = pTGAME->GetMainCam();

			if( pTCHAR && pCAM )
			{
				D3DXVECTOR3 vDIR = pTCHAR->m_bContryID == COUNTRY_CRAXION ? TSTART_C_CAM_TARGET - TSTART_C_CAM_POSITION : TSTART_D_CAM_TARGET - TSTART_D_CAM_POSITION;
				FLOAT fTIME = 1.0f - FLOAT(m_dwTick) / FLOAT(m_dwTotal);

				pTGAME->m_wCamDIR = WORD(CTMath::GetYAW(&vDIR) * 5) + 1800 - WORD(TSTART_CAM_YAW * fTIME);
				pCAM->m_wCamPITCH = WORD(CTMath::GetPITCH(&vDIR) * 5) + WORD(TSTART_CAM_PITCH * fTIME);
				pCAM->m_fPrevDIST = TSTART_CAM_DIST * fTIME + D3DXVec3Length(&vDIR);
				pCAM->m_fCamDIST = pCAM->m_fPrevDIST;

				pTGAME->m_wCamDIR %= 1800;
				pCAM->m_wCamPITCH %= 1800;

				vDIR = pTCHAR->GetPosition();
				vDIR.y += pCAM->m_fCamHEIGHT;

				vDIR = (pTCHAR->m_bContryID == COUNTRY_CRAXION ? TSTART_C_CAM_TARGET : TSTART_D_CAM_TARGET) - vDIR;
				D3DXVec3Lerp(
					&pCAM->m_vTOFFSET,
					&vDIR,
					&D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
					fTIME);
			}
		}

		break;

	case TSCENE_PLAYER_CODE_SHOW_DIALOG			:
		{
			CTClientChar *pTCHAR = pTGAME->GetMainChar();
			CTClientMAP *pMAP = pTGAME->GetClientMap();

			if( pMAP && pTCHAR )
			{
				pTGAME->ResetTargetOBJ(pMAP->FindTNPC(pTCHAR->m_bContryID == COUNTRY_CRAXION ? TSTART_EVENT_NPCID_C : TSTART_EVENT_NPCID_D));
				pTGAME->DoDEFACTION();
			}
		}

		break;

	case TSCENE_PLAYER_CODE_SHOW_UI				: pTGAME->DisableUI(TFRAME_CINEMATIC); break;
	case TSCENE_PLAYER_CODE_HIDE_UI				: pTGAME->EnableUI(TFRAME_CINEMATIC); break;
	case TSCENE_PLAYER_CODE_SHOW_TEXT			:
		{
			CTCinematicDlg *pTCINEMATIC = (CTCinematicDlg *) pTGAME->GetFrame(TFRAME_CINEMATIC);
			CTClientChar *pTCHAR = pTGAME->GetMainChar();

			if( pTCINEMATIC && pTCHAR )
				pTCINEMATIC->ResetTEXT(m_bTEXT ? &CTChart::LoadString(pTCHAR->m_bContryID == COUNTRY_CRAXION ? m_bTEXT == 1 ? TSTR_FCEVENT_C : TSTR_FCEVENT_C2 : m_bTEXT == 1 ? TSTR_FCEVENT_D : TSTR_FCEVENT_D2) : NULL);
		}

		break;

	case TSCENE_PLAYER_CODE_SHOW_SKIP			:
		{
			CTCinematicDlg *pTCINEMATIC = (CTCinematicDlg *) pTGAME->GetFrame(TFRAME_CINEMATIC);

			if(pTCINEMATIC)
				pTCINEMATIC->ShowTSKIP(TRUE);
		}

		break;

	case TSCENE_PLAYER_CODE_HIDE_SKIP			:
		{
			CTCinematicDlg *pTCINEMATIC = (CTCinematicDlg *) pTGAME->GetFrame(TFRAME_CINEMATIC);

			if(pTCINEMATIC)
				pTCINEMATIC->ShowTSKIP(FALSE);
		}

		break;

	case TSCENE_PLAYER_CODE_RESUME_TICK			: pTGAME->m_bEnableNPCTick = TRUE; break;
	case TSCENE_PLAYER_CODE_PAUSE_TICK			: pTGAME->m_bEnableNPCTick = FALSE; break;
	case TSCENE_PLAYER_CODE_TELEPORT			: pTGAME->ExitTutorial(); break;
	default										: break;
	}

	return m_dwTick ? FALSE : TRUE;
}

void CTSpCode::Render( CTClientGame *pTGAME)
{
	switch(m_eCode)
	{
	case TSCENE_PLAYER_CODE_DRAW_COLOR_FADEIN	:
		if(m_dwTotal)
		{
			BYTE bALPHA = BYTE(0xFF - 0xFF * m_dwTick / m_dwTotal);
			Fill( pTGAME, NULL, TINSTALL_ALPHA( m_dwCOLOR, bALPHA));
		}

		break;

	case TSCENE_PLAYER_CODE_FADE_OUT_TEXTURE	:
		if( m_dwTotal && m_pTEX[0] )
		{
			BYTE bALPHA = BYTE(0xFF * m_dwTick / m_dwTotal);

			m_pTEX[0]->m_dwCurTick = m_dwTexTick[0];
			if(GETVALUE_A(m_dwCOLOR))
				Fill( pTGAME, m_pTEX[0]->GetTexture(), NULL, TINSTALL_ALPHA( 0x00FFFFFF, bALPHA), m_dwCOLOR);
			else
				Fill( pTGAME, m_pTEX[0]->GetTexture(), TINSTALL_ALPHA( m_dwCOLOR, bALPHA));
		}

		break;

	case TSCENE_PLAYER_CODE_FADE_IN_TEXTURE		:
		if( m_dwTotal && m_pTEX[0] )
		{
			BYTE bALPHA = BYTE(0xFF - 0xFF * m_dwTick / m_dwTotal);

			m_pTEX[0]->m_dwCurTick = m_dwTexTick[0];
			if(GETVALUE_A(m_dwCOLOR))
				Fill( pTGAME, m_pTEX[0]->GetTexture(), NULL, TINSTALL_ALPHA( 0x00FFFFFF, bALPHA), m_dwCOLOR);
			else
				Fill( pTGAME, m_pTEX[0]->GetTexture(), TINSTALL_ALPHA( m_dwCOLOR, bALPHA));
		}

		break;

	case TSCENE_PLAYER_CODE_FADE_TEXTURE		:
		if( m_dwTotal && m_pTEX[0] && m_pTEX[1] )
		{
			BYTE bALPHA = BYTE(0xFF * m_dwTick / m_dwTotal);

			m_pTEX[0]->m_dwCurTick = m_dwTexTick[0];
			m_pTEX[1]->m_dwCurTick = m_dwTexTick[1];

			Fill( pTGAME, m_pTEX[0]->GetTexture(), m_pTEX[1]->GetTexture(), TINSTALL_ALPHA( 0x00FFFFFF, bALPHA), 0xFFFFFFFF);
		}

		break;

	case TSCENE_PLAYER_CODE_DRAW_TEXTURE		:
		if( m_dwTotal && m_pTEX[0] )
		{
			m_pTEX[0]->m_dwCurTick = m_dwTexTick[0];
			Fill( pTGAME, m_pTEX[0]->GetTexture(), 0xFFFFFFFF);
		}

		break;

	case TSCENE_PLAYER_CODE_DRAW_COLOR			:
		if(m_dwTotal)
			Fill( pTGAME, NULL, m_dwCOLOR);

		break;
	}
}

DWORD CTSpCode::CalcTexTick( BYTE bINDEX, DWORD dwTick)
{
	if( m_pTEX[bINDEX] && m_pTEX[bINDEX]->m_dwTotalTick )
	{
		m_dwTexTick[bINDEX] += dwTick;
		m_dwTexTick[bINDEX] %= m_pTEX[bINDEX]->m_dwTotalTick;
	}
	else
		m_dwTexTick[bINDEX] = 0;

	return m_dwTexTick[bINDEX];
}

void CTSpCode::Fill( CTClientGame *pTGAME,
					 LPDIRECT3DTEXTURE9 pTEX,
					 DWORD dwCOLOR)
{
	if(!pTGAME)
		return;

	CTClientChar *pTCHAR = pTGAME->GetMainChar();
	CD3DDevice *pDevice = pTGAME->GetDevice();

	if( !pDevice || !pTCHAR )
		return;

	FLOAT fU = 0.5f * (1.0f - 0.5f * FLOAT(pDevice->m_option.m_dwScreenX) / FLOAT(pDevice->m_option.m_dwScreenY));
	FLOAT fV = pTCHAR->m_bContryID == COUNTRY_CRAXION ? 0.5f : 0.0f;

	TNLVERTEX vTRECT[4] = {
		{								  0.0f,									0.0f, 0.5f, 1.0f, dwCOLOR,		  fU,		 fV},
		{ FLOAT(pDevice->m_option.m_dwScreenX),									0.0f, 0.5f, 1.0f, dwCOLOR, 1.0f - fU,		 fV},
		{								  0.0f, FLOAT(pDevice->m_option.m_dwScreenY), 0.5f, 1.0f, dwCOLOR,		  fU, 0.5f + fV},
		{ FLOAT(pDevice->m_option.m_dwScreenX), FLOAT(pDevice->m_option.m_dwScreenY), 0.5f, 1.0f, dwCOLOR, 1.0f - fU, 0.5f + fV}};

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, pTEX ? D3DTOP_MODULATE : D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE);

	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

	pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);

	pDevice->m_pDevice->SetTexture( 0, pTEX);
	pDevice->m_pDevice->SetFVF(T3DFVF_TNLVERTEX);

	pDevice->m_pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2, vTRECT, sizeof(TNLVERTEX));
}

void CTSpCode::Fill( CTClientGame *pTGAME,
					 LPDIRECT3DTEXTURE9 pTEX1,
					 LPDIRECT3DTEXTURE9 pTEX2,
					 DWORD dwCOLOR1,
					 DWORD dwCOLOR2)
{
	if(!pTGAME)
		return;

	CTClientChar *pTCHAR = pTGAME->GetMainChar();
	CD3DDevice *pDevice = pTGAME->GetDevice();

	if( !pDevice || !pTCHAR )
		return;

	FLOAT fU = 0.5f * (1.0f - 0.5f * FLOAT(pDevice->m_option.m_dwScreenX) / FLOAT(pDevice->m_option.m_dwScreenY));
	FLOAT fV = pTCHAR->m_bContryID == COUNTRY_CRAXION ? 0.5f : 0.0f;

	TNLVERTEX vTRECT[4] = {
		{								  0.0f,									0.0f, 0.5f, 1.0f, dwCOLOR1,		   fU,		  fV},
		{ FLOAT(pDevice->m_option.m_dwScreenX),									0.0f, 0.5f, 1.0f, dwCOLOR1, 1.0f - fU,		  fV},
		{								  0.0f, FLOAT(pDevice->m_option.m_dwScreenY), 0.5f, 1.0f, dwCOLOR1,		   fU, 0.5f + fV},
		{ FLOAT(pDevice->m_option.m_dwScreenX), FLOAT(pDevice->m_option.m_dwScreenY), 0.5f, 1.0f, dwCOLOR1, 1.0f - fU, 0.5f + fV}};

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, pTEX1 ? D3DTOP_MODULATE : D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_BLENDDIFFUSEALPHA);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE);

	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, pTEX2 ? D3DTA_TEXTURE : D3DTA_TFACTOR);

	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwCOLOR2);

	pDevice->m_pDevice->SetTexture( 0, pTEX1);
	pDevice->m_pDevice->SetTexture( 1, pTEX2);
	pDevice->m_pDevice->SetFVF(T3DFVF_TNLVERTEX);

	pDevice->m_pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2, vTRECT, sizeof(TNLVERTEX));
}


CTScenePlayer::CTScenePlayer()
{
	Release();
}

CTScenePlayer::~CTScenePlayer()
{
	Release();
}

void CTScenePlayer::Release()
{
	while(!m_vTSCENE.empty())
	{
		delete m_vTSCENE.back();
		m_vTSCENE.pop_back();
	}

	m_mapTEX.clear();
	m_bINDEX = 0;
}

void CTScenePlayer::LoadScript( CD3DDevice *pDevice,
							    CTachyonRes *pRES,
							    CString *pScript,
								int nCount)
{
	static DWORD dwTSceneTex[] = {
		ID_TEX_SCENE00,
		ID_TEX_SCENE01,
		ID_TEX_SCENE02};
	Release();

	for( int i=0; i<nCount; i++)
	{
		CString strLINE = pScript[i];
		strLINE.Trim();

		if( !strLINE.IsEmpty() && strLINE.GetAt(0) != ';' )
		{
			CString strTSEP(_T(" "));
			int nPOS = 0;

			CString strTEXT = strLINE.Tokenize( strTSEP, nPOS);
			CTSpCode *pTCODE = NULL;

			if( strTEXT == _T("load") )
			{
				strTEXT = strLINE.Tokenize( strTSEP, nPOS);

				if(!strTEXT.IsEmpty())
				{
					int nIndex = atoi(strTEXT);
					MAPRES::iterator finder = pRES->m_mapTEX.find(dwTSceneTex[nIndex]);

					if( finder != pRES->m_mapTEX.end() && (*finder).second )
						m_mapTEX.insert( MAPTEXTURESET::value_type( nIndex, (LPTEXTURESET) (*finder).second));
				}
			}
			else if( strTEXT == _T("draw_color") )
			{
				pTCODE = new CTSpCode();

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					pTCODE->m_dwTotal = atoi(strTEXT);

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					sscanf( strTEXT, _T("%x"), &pTCODE->m_dwCOLOR);

				pTCODE->m_eCode = TSCENE_PLAYER_CODE_DRAW_COLOR;
			}
			else if( strTEXT == _T("draw_texture") )
			{
				pTCODE = new CTSpCode();

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					pTCODE->m_dwTotal = atoi(strTEXT);

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
				{
					int nIndex = atoi(strTEXT);
					MAPTEXTURESET::iterator finder = m_mapTEX.find(nIndex);

					if( finder != m_mapTEX.end() && (*finder).second )
						pTCODE->m_pTEX[0] = (*finder).second;
				}

				pTCODE->m_eCode = TSCENE_PLAYER_CODE_DRAW_TEXTURE;
			}
			else if( strTEXT == _T("fadein_texture") )
			{
				pTCODE = new CTSpCode();

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					pTCODE->m_dwTotal = atoi(strTEXT);

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
				{
					int nIndex = atoi(strTEXT);
					MAPTEXTURESET::iterator finder = m_mapTEX.find(nIndex);

					if( finder != m_mapTEX.end() && (*finder).second )
						pTCODE->m_pTEX[0] = (*finder).second;
				}

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					sscanf( strTEXT, _T("%x"), &pTCODE->m_dwCOLOR);

				pTCODE->m_eCode = TSCENE_PLAYER_CODE_FADE_IN_TEXTURE;
			}
			else if( strTEXT == _T("fadeout_texture") )
			{
				pTCODE = new CTSpCode();

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					pTCODE->m_dwTotal = atoi(strTEXT);

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
				{
					int nIndex = atoi(strTEXT);
					MAPTEXTURESET::iterator finder = m_mapTEX.find(nIndex);

					if( finder != m_mapTEX.end() && (*finder).second )
						pTCODE->m_pTEX[0] = (*finder).second;
				}

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					sscanf( strTEXT, _T("%x"), &pTCODE->m_dwCOLOR);

				pTCODE->m_eCode = TSCENE_PLAYER_CODE_FADE_OUT_TEXTURE;
			}
			else if( strTEXT == _T("fade_texture") )
			{
				pTCODE = new CTSpCode();

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					pTCODE->m_dwTotal = atoi(strTEXT);

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
				{
					int nIndex = atoi(strTEXT);
					MAPTEXTURESET::iterator finder = m_mapTEX.find(nIndex);

					if( finder != m_mapTEX.end() && (*finder).second )
						pTCODE->m_pTEX[0] = (*finder).second;
				}

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
				{
					int nIndex = atoi(strTEXT);
					MAPTEXTURESET::iterator finder = m_mapTEX.find(nIndex);

					if( finder != m_mapTEX.end() && (*finder).second )
						pTCODE->m_pTEX[1] = (*finder).second;
				}

				pTCODE->m_eCode = TSCENE_PLAYER_CODE_FADE_TEXTURE;
			}
			else if( strTEXT == _T("show_ui") )
			{
				pTCODE = new CTSpCode();
				pTCODE->m_eCode = TSCENE_PLAYER_CODE_SHOW_UI;
			}
			else if( strTEXT == _T("hide_ui") )
			{
				pTCODE = new CTSpCode();
				pTCODE->m_eCode = TSCENE_PLAYER_CODE_HIDE_UI;
			}
			else if( strTEXT == _T("show_dialog") )
			{
				pTCODE = new CTSpCode();
				pTCODE->m_eCode = TSCENE_PLAYER_CODE_SHOW_DIALOG;
			}
			else if( strTEXT == _T("show_text") )
			{
				pTCODE = new CTSpCode();

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					pTCODE->m_bTEXT = atoi(strTEXT);

				pTCODE->m_eCode = TSCENE_PLAYER_CODE_SHOW_TEXT;
			}
			else if( strTEXT == _T("start_camera") )
			{
				pTCODE = new CTSpCode();
				pTCODE->m_eCode = TSCENE_PLAYER_CODE_START_CAMERA;
			}
			else if( strTEXT == _T("calc_camera") )
			{
				pTCODE = new CTSpCode();

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					pTCODE->m_dwTotal = atoi(strTEXT);

				pTCODE->m_eCode = TSCENE_PLAYER_CODE_CALC_CAMERA;
			}
			else if( strTEXT == _T("wait") )
			{
				pTCODE = new CTSpCode();

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					pTCODE->m_dwTotal = atoi(strTEXT);

				pTCODE->m_eCode = TSCENE_PLAYER_CODE_WAIT;
			}
			else if( strTEXT == _T("draw_color_fadein") )
			{
				pTCODE = new CTSpCode();

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					pTCODE->m_dwTotal = atoi(strTEXT);

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					sscanf( strTEXT, _T("%x"), &pTCODE->m_dwCOLOR);

				pTCODE->m_eCode = TSCENE_PLAYER_CODE_DRAW_COLOR_FADEIN;
			}
			else if( strTEXT == _T("teleport") )
			{
				pTCODE = new CTSpCode();
				pTCODE->m_eCode = TSCENE_PLAYER_CODE_TELEPORT;
			}
			else if( strTEXT == _T("pause_tick") )
			{
				pTCODE = new CTSpCode();
				pTCODE->m_eCode = TSCENE_PLAYER_CODE_PAUSE_TICK;
			}
			else if( strTEXT == _T("resume_tick") )
			{
				pTCODE = new CTSpCode();
				pTCODE->m_eCode = TSCENE_PLAYER_CODE_RESUME_TICK;
			}
			else if( strTEXT == _T("show_skip") )
			{
				pTCODE = new CTSpCode();
				pTCODE->m_eCode = TSCENE_PLAYER_CODE_SHOW_SKIP;
			}
			else if( strTEXT == _T("hide_skip") )
			{
				pTCODE = new CTSpCode();
				pTCODE->m_eCode = TSCENE_PLAYER_CODE_HIDE_SKIP;
			}

			if(pTCODE)
			{
				pTCODE->m_dwTick = pTCODE->m_dwTotal;
				m_vTSCENE.push_back(pTCODE);
			}
		}
	}
}

void CTScenePlayer::StartScene( CD3DDevice *pDevice,
							    CTachyonRes *pRES,
								int nTSCENE)
{
	static CString vTSCENE_SCRIPT_LEAVE_TUTORIAL[] = {
		CString(_T("hide_skip")),
		CString(_T("hide_ui")),
		CString(_T("draw_color_fadein 3000 0xFFFFFFFF")),
		CString(_T("teleport")),
		CString(_T("draw_color 3600000 0xFFFFFFFF"))};

	static CString vTSCENE_SCRIPT_ENTER_WORLD[] = {
		CString(_T("load 0")),
		CString(_T("load 1")),
		CString(_T("hide_ui")),
		CString(_T("show_skip")),
		CString(_T("pause_tick")),
		CString(_T("start_camera")),
		CString(_T("draw_color 100 0xFFFFFFFF")),
		CString(_T("show_text 1")),
		CString(_T("fadein_texture 2500 0 0xFFFFFFFF")),
		CString(_T("fade_texture 3500 0 1")),
		CString(_T("show_text 2")),
		CString(_T("draw_texture 3500 1")),
		CString(_T("hide_skip")),
		CString(_T("show_text 0")),
		CString(_T("fadeout_texture 3500 1 0x00FFFFFF")),
		CString(_T("resume_tick")),
		CString(_T("wait 800")),
		CString(_T("calc_camera 3500")),
		CString(_T("show_ui")),
		CString(_T("wait 1000")),
		CString(_T("show_dialog"))};

	static CString vTSCENE_SCRIPT_SKIP[] = {
		CString(_T("load 0")),
		CString(_T("load 1")),
		CString(_T("hide_ui")),
		CString(_T("hide_skip")),
		CString(_T("pause_tick")),
		CString(_T("start_camera")),
		CString(_T("show_text 0")),
		CString(_T("draw_texture 1000 1")),
		CString(_T("fadeout_texture 3500 1 0x00FFFFFF")),
		CString(_T("resume_tick")),
		CString(_T("wait 800")),
		CString(_T("calc_camera 3500")),
		CString(_T("show_ui")),
		CString(_T("wait 1000")),
		CString(_T("show_dialog"))};

	static CString* vTSCRIPT[TSCENE_SCRIPT_COUNT] = {
		vTSCENE_SCRIPT_LEAVE_TUTORIAL,
		vTSCENE_SCRIPT_ENTER_WORLD,
		vTSCENE_SCRIPT_SKIP};
	static DWORD vTCOUNT[TSCENE_SCRIPT_COUNT] = { 5, 21, 15};

	if( nTSCENE < TSCENE_SCRIPT_COUNT )
		LoadScript( pDevice, pRES, vTSCRIPT[nTSCENE], vTCOUNT[nTSCENE]);
}

BYTE CTScenePlayer::CalcTick( CTClientGame *pTGAME, DWORD dwTick)
{
	if(m_vTSCENE.empty())
		return FALSE;

	while( m_bINDEX < INT(m_vTSCENE.size()) && m_vTSCENE[m_bINDEX] && m_vTSCENE[m_bINDEX]->CalcTick( pTGAME, dwTick) )
	{
		m_bINDEX++;
		dwTick = 0;
	}

	if( m_bINDEX >= INT(m_vTSCENE.size()) || !m_vTSCENE[m_bINDEX] )
	{
		Release();
		return FALSE;
	}

	return TRUE;
}

void CTScenePlayer::Render( CTClientGame *pTGAME)
{
	if( m_bINDEX < INT(m_vTSCENE.size()) && m_vTSCENE[m_bINDEX] )
		m_vTSCENE[m_bINDEX]->Render(pTGAME);
}

BYTE CTScenePlayer::IsRunning()
{
	return m_vTSCENE.empty() ? FALSE : TRUE;
}
