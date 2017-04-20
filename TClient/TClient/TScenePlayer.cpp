#include "stdafx.h"
#include "TScenePlayer.h"
#include "TClientGame.h"
#include "TClientWnd.h"

#define TINSTALL_ALPHA( dwColor, bAlpha)			DWORD(((DWORD(bAlpha) << 24) & 0xFF000000) | (DWORD(dwColor) & 0x00FFFFFF))

static DWORD _dwTEXT_[] =
{
	TSTR_NONE,
	TSTR_FCEVENT_C1,
	TSTR_FCEVENT_C2,
	TSTR_FCEVENT_D1,
	TSTR_FCEVENT_D2,
	TSTR_FCEVENT_D_TO_B1,
	TSTR_FCEVENT_D_TO_B2,
	TSTR_FCEVENT_D_TO_B3,
	TSTR_FCEVENT_C_TO_B1,
	TSTR_FCEVENT_C_TO_B2,
	TSTR_FCEVENT_C_TO_B3,
	TSTR_FCEVENT_P1,
	TSTR_FCEVENT_P2,
	TSTR_FCEVENT_P3
};

static DWORD _dwTEXTURE_[] =
{
	ID_TEX_SCENE00,
	ID_TEX_SCENE01,
	ID_TEX_SCENE02,
	ID_TEX_SCENE03,
	ID_TEX_SCENE04
};

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
	m_nAct = 255;
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
				D3DXVECTOR3 vDIR = GetCameraDIR( pTCHAR->m_bContryID );

				pCAM->m_wCamPITCH = WORD(CTMath::GetPITCH(&vDIR) * 5);
				pCAM->m_fPrevDIST = D3DXVec3Length(&vDIR);
				pCAM->m_fCamDIST = pCAM->m_fPrevDIST;

				pTGAME->m_wCamDIR = WORD(CTMath::GetYAW(&vDIR) * 5);
				vDIR = pTCHAR->GetPosition();
				vDIR.y += pCAM->m_fCamHEIGHT;

				switch( pTCHAR->m_bContryID )
				{
				case TCONTRY_D:
					pCAM->m_vTOFFSET = TSTART_D_CAM_TARGET - vDIR;
					break;
				case TCONTRY_C:
					pCAM->m_vTOFFSET = TSTART_C_CAM_TARGET - vDIR;
					break;
				case TCONTRY_B:
					pCAM->m_vTOFFSET = TSTART_B_CAM_TARGET - vDIR;
					break;
				case TCONTRY_PEACE:
					pCAM->m_vTOFFSET = TSTART_P_CAM_TARGET - vDIR;
					break;
				}
			}
		}

		break;

	case TSCENE_PLAYER_CODE_CALC_CAMERA			:
		{
			CTClientChar *pTCHAR = pTGAME->GetMainChar();
			CTClientCAM *pCAM = pTGAME->GetMainCam();

			if( pTCHAR && pCAM )
			{
				D3DXVECTOR3 vDIR = GetCameraDIR(pTCHAR->m_bContryID);
				FLOAT fTIME = 1.0f - FLOAT(m_dwTick) / FLOAT(m_dwTotal);

				pTGAME->m_wCamDIR = WORD(CTMath::GetYAW(&vDIR) * 5) + 1800 - WORD(TSTART_CAM_YAW * fTIME);
				pCAM->m_wCamPITCH = WORD(CTMath::GetPITCH(&vDIR) * 5) + WORD(TSTART_CAM_PITCH * fTIME);
				pCAM->m_fPrevDIST = TSTART_CAM_DIST * fTIME + D3DXVec3Length(&vDIR);
				pCAM->m_fCamDIST = pCAM->m_fPrevDIST;

				pTGAME->m_wCamDIR %= 1800;
				pCAM->m_wCamPITCH %= 1800;

				vDIR = pTCHAR->GetPosition();
				vDIR.y += pCAM->m_fCamHEIGHT;

				switch( pTCHAR->m_bContryID )
				{
				case TCONTRY_D:
					vDIR = TSTART_D_CAM_TARGET - vDIR;
					break;
				case TCONTRY_C:
					vDIR = TSTART_C_CAM_TARGET - vDIR;
					break;
				case TCONTRY_B:
					vDIR = TSTART_B_CAM_TARGET - vDIR;
					break;
				case TCONTRY_PEACE:
					vDIR = TSTART_P_CAM_TARGET - vDIR;
					break;
				}

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
				DWORD dwNPC = 0;
				switch( pTCHAR->m_bContryID )
				{
				case TCONTRY_D:
					dwNPC = TSTART_EVENT_NPCID_D;
					break;
				case TCONTRY_C:
					dwNPC = TSTART_EVENT_NPCID_C;
					break;
				case TCONTRY_B:
					//dwNPC = TSTART_EVENT_NPCID_B;
					break;
				case TCONTRY_PEACE:
					dwNPC = TSTART_EVENT_NPCID_P;
					break;
				}

				if( 0 != dwNPC )
				{
					pTGAME->ResetTargetOBJ( pMAP->FindTNPC( dwNPC ) );
					pTGAME->DoDEFACTION();
				}
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
			{
				CString strMSG;
				DWORD dwID = 0;

				if( pTGAME->m_vTSCENE_PLAYER.m_nCurScene == TSCENE_SCRIPT_BROIR_ENTER &&
					pTGAME->m_vTSCENE_PLAYER.m_bPrevCountry == TCONTRY_C &&
					m_bTEXT != 0 )
				{
					dwID = _dwTEXT_[ m_bTEXT+3 ];
				}
				else if( (pTGAME->m_vTSCENE_PLAYER.m_nCurScene == TSCENE_SCRIPT_DEFUGEL_ENTER ||
						pTGAME->m_vTSCENE_PLAYER.m_nCurScene == TSCENE_SCRIPT_CRAXION_ENTER) &&
						(pTGAME->m_vTSCENE_PLAYER.m_bPrevCountry == (BYTE)(-1)) &&
						( m_bTEXT == 1 || m_bTEXT == 3) )
				{
					dwID = 0;
				}
				else
					dwID = _dwTEXT_[ m_bTEXT ];

				if( dwID != 0 )
					strMSG = CTChart::LoadString( (TSTRING) dwID );
				else
					strMSG.Empty();

				pTCINEMATIC->ResetTEXT( &strMSG );
			}
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

	case TSCENE_PLAYER_CODE_ACTION:
		{
			CTClientChar* pMainChar = pTGAME->GetMainChar();
			if( pMainChar )
			{
				TACTION vActionID = pMainChar->FindActionID(
					this->m_nAct,
					pMainChar->GetWeaponID(pMainChar->m_bMode));

				pTGAME->GetSession()->SendCS_CHGMODE_REQ(
					MT_NORMAL);

				pTGAME->SendCS_ACTION_REQ(
					pMainChar->m_dwID,
					pMainChar->m_bType,
					this->m_nAct,
					vActionID.m_dwActID,
					vActionID.m_dwAniID,
					pTGAME->GetMainWnd()->m_bChannel,
					pTGAME->GetClientMap()->m_wMapID,
					0);
			}
		}
		break;

	case TSCENE_PLAYER_CODE_CHECK_QUEST:
		{
			pTGAME->CheckQuestOBJ();
		}
		break;

	case TSCENE_PLAYER_CODE_RESUME_TICK			: pTGAME->m_bEnableNPCTick = TRUE; break;
	case TSCENE_PLAYER_CODE_PAUSE_TICK			: pTGAME->m_bEnableNPCTick = FALSE; break;
	case TSCENE_PLAYER_CODE_TELEPORT			: pTGAME->ExitTutorial(); break;
	default										: break;
	}

	return m_dwTick ? FALSE : TRUE;
}

D3DXVECTOR3 CTSpCode::GetCameraDIR(
	BYTE bCountry )
{
	switch( bCountry )
	{
	case TCONTRY_D:
		return (TSTART_D_CAM_TARGET - TSTART_D_CAM_POSITION);
	case TCONTRY_C:
		return (TSTART_C_CAM_TARGET - TSTART_C_CAM_POSITION);
	case TCONTRY_B:
		return (TSTART_B_CAM_TARGET - TSTART_B_CAM_POSITION);
	case TCONTRY_PEACE:
		return (TSTART_P_CAM_TARGET - TSTART_P_CAM_POSITION);
	}
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void CTSpCode::Render( CTClientGame *pTGAME)
{
	switch(m_eCode)
	{
	case TSCENE_PLAYER_CODE_DRAW_COLOR_FADEIN	:
		if(m_dwTotal)
		{
			BYTE bALPHA = BYTE(0xFF - 0xFF * m_dwTick / m_dwTotal);
			Fill(
				pTGAME,
				NULL,
				TINSTALL_ALPHA( m_dwCOLOR, bALPHA),
				m_dwUV );
		}

		break;

	case TSCENE_PLAYER_CODE_FADE_OUT_TEXTURE	:
		if( m_dwTotal && m_pTEX[0] )
		{
			BYTE bALPHA = BYTE(0xFF * m_dwTick / m_dwTotal);

			m_pTEX[0]->m_dwCurTick = m_dwTexTick[0];
			if(GETVALUE_A(m_dwCOLOR))
				Fill(
					pTGAME,
					m_pTEX[0]->GetTexture(),
					NULL,
					TINSTALL_ALPHA( 0x00FFFFFF, bALPHA),
					m_dwCOLOR,
					m_dwUV,
					m_dwUV2);
			else
				Fill(
					pTGAME,
					m_pTEX[0]->GetTexture(),
					TINSTALL_ALPHA( m_dwCOLOR, bALPHA),
					m_dwUV );
		}

		break;

	case TSCENE_PLAYER_CODE_FADE_IN_TEXTURE		:
		if( m_dwTotal && m_pTEX[0] )
		{
			BYTE bALPHA = BYTE(0xFF - 0xFF * m_dwTick / m_dwTotal);

			m_pTEX[0]->m_dwCurTick = m_dwTexTick[0];
			if(GETVALUE_A(m_dwCOLOR))
			{
				Fill(
					pTGAME,
					m_pTEX[0]->GetTexture(),
					NULL,
					TINSTALL_ALPHA( 0x00FFFFFF, bALPHA),
					m_dwCOLOR,
					m_dwUV,
					m_dwUV2 );
			}
			else
			{
				Fill(
					pTGAME,
					m_pTEX[0]->GetTexture(),
					TINSTALL_ALPHA( m_dwCOLOR, bALPHA),
					m_dwUV );
			}
		}

		break;

	case TSCENE_PLAYER_CODE_FADE_TEXTURE		:
		if( m_dwTotal && m_pTEX[0] && m_pTEX[1] )
		{
			BYTE bALPHA = BYTE(0xFF * m_dwTick / m_dwTotal);

			m_pTEX[0]->m_dwCurTick = 0;
			m_pTEX[1]->m_dwCurTick = 0;

			FillFade(
				pTGAME,
				m_pTEX[0]->GetTexture(),
				m_pTEX[1]->GetTexture(),
				bALPHA,
				m_dwUV,
				m_dwUV2 );
		}

		break;

	case TSCENE_PLAYER_CODE_DRAW_TEXTURE		:
		if( m_dwTotal && m_pTEX[0] )
		{
			m_pTEX[0]->m_dwCurTick = m_dwTexTick[0];
			{
				Fill(
					pTGAME,
					m_pTEX[0]->GetTexture(),
					0xFFFFFFFF,
					m_dwUV );
			}
		}

		break;

	case TSCENE_PLAYER_CODE_DRAW_COLOR			:
		if(m_dwTotal)
		{
			Fill(
				pTGAME,
				NULL,
				m_dwCOLOR,
				m_dwUV );
		}

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

void CTSpCode::Fill(
	CTClientGame *pTGAME,
	LPDIRECT3DTEXTURE9 pTEX,
	DWORD dwCOLOR,
	DWORD dwUV )
{
	if(!pTGAME)
		return;

	CTClientChar *pTCHAR = pTGAME->GetMainChar();
	CD3DDevice *pDevice = pTGAME->GetDevice();

	if( !pDevice || !pTCHAR )
		return;

	FLOAT fU = 0.5f * (1.0f - 0.5f * FLOAT(pDevice->m_option.m_dwScreenX) / FLOAT(pDevice->m_option.m_dwScreenY));
	FLOAT fV = dwUV ? 0.5f : 0.0f;

	TNLVERTEX vTRECT[4] = {
		{ 0.0f, 0.0f, 0.5f, 1.0f, dwCOLOR, fU, fV},
		{ FLOAT(pDevice->m_option.m_dwScreenX), 0.0f, 0.5f, 1.0f, dwCOLOR, 1.0f - fU, fV},
		{ 0.0f, FLOAT(pDevice->m_option.m_dwScreenY), 0.5f, 1.0f, dwCOLOR, fU, 0.5f + fV},
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

void CTSpCode::Fill(
	CTClientGame *pTGAME,
	LPDIRECT3DTEXTURE9 pTEX1,
	LPDIRECT3DTEXTURE9 pTEX2,
	DWORD dwCOLOR1,
	DWORD dwCOLOR2,
	DWORD dwUV1,
	DWORD dwUV2 )
{
	if(!pTGAME)
		return;

	CTClientChar *pTCHAR = pTGAME->GetMainChar();
	CD3DDevice *pDevice = pTGAME->GetDevice();

	if( !pDevice || !pTCHAR )
		return;

	FLOAT fU = 0.5f * (1.0f - 0.5f * FLOAT(pDevice->m_option.m_dwScreenX) / FLOAT(pDevice->m_option.m_dwScreenY));
	FLOAT fV = dwUV1 ?  0.5f : 0.0f;

	TNLVERTEX vTRECT[4] = {
		{ 0.0f, 0.0f, 0.5f, 1.0f, dwCOLOR1, fU, fV},
		{ FLOAT(pDevice->m_option.m_dwScreenX), 0.0f, 0.5f, 1.0f, dwCOLOR1, 1.0f - fU, fV},
		{ 0.0f, FLOAT(pDevice->m_option.m_dwScreenY), 0.5f, 1.0f, dwCOLOR1, fU, 0.5f + fV},
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

void CTSpCode::FillFade(
	CTClientGame *pTGAME,
	LPDIRECT3DTEXTURE9 pTEX1,
	LPDIRECT3DTEXTURE9 pTEX2,
	BYTE bAlpha,
	DWORD dwUV1,
	DWORD dwUV2 )
{
	if(!pTGAME)
		return;

	CTClientChar *pTCHAR = pTGAME->GetMainChar();
	CD3DDevice *pDevice = pTGAME->GetDevice();

	if( !pDevice || !pTCHAR )
		return;

	FLOAT fU = 0.5f * (1.0f - 0.5f * FLOAT(pDevice->m_option.m_dwScreenX) / FLOAT(pDevice->m_option.m_dwScreenY));
	FLOAT fV = dwUV2 ? 0.5f : 0.0f;

	DWORD dwColor = D3DCOLOR_ARGB(bAlpha,0,0,0);

	TNLVERTEX vTRECT[4] = {
		{ 0.0f, 0.0f, 0.5f, 1.0f, dwColor, fU, fV},
		{ FLOAT(pDevice->m_option.m_dwScreenX), 0.0f, 0.5f, 1.0f, dwColor, 1.0f - fU, fV},
		{ 0.0f, FLOAT(pDevice->m_option.m_dwScreenY), 0.5f, 1.0f, dwColor, fU, 0.5f + fV},
		{ FLOAT(pDevice->m_option.m_dwScreenX), FLOAT(pDevice->m_option.m_dwScreenY), 0.5f, 1.0f, dwColor, 1.0f - fU, 0.5f + fV}};

	pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0 );
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->m_pDevice->SetFVF(T3DFVF_TNLVERTEX);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	pDevice->m_pDevice->SetTexture( 0, pTEX2);

	pDevice->m_pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2, vTRECT, sizeof(TNLVERTEX));

	fV = dwUV1 ? 0.5f : 0.0f;
	vTRECT[0].m_fV = fV;
	vTRECT[1].m_fV = fV;
	vTRECT[2].m_fV = 0.5f + fV;
	vTRECT[3].m_fV = 0.5f + fV;

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->m_pDevice->SetTexture( 0, pTEX1);
	pDevice->m_pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2, vTRECT, sizeof(TNLVERTEX));
}

CTScenePlayer::CTScenePlayer()
: m_nCurScene(0),
m_bPrevCountry(255)
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
					MAPRES::iterator finder = pRES->m_mapTEX.find(_dwTEXTURE_[nIndex]);

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

				strTEXT = strLINE.Tokenize( strTSEP, nPOS );
				if(!strTEXT.IsEmpty())
				{
					int nIndex = atoi(strTEXT);
					pTCODE->m_dwUV = nIndex;
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

				strTEXT = strLINE.Tokenize( strTSEP, nPOS );
				if(!strTEXT.IsEmpty())
					sscanf( strTEXT, _T("%d"), &pTCODE->m_dwUV);
               
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

				strTEXT = strLINE.Tokenize( strTSEP, nPOS );
				if(!strTEXT.IsEmpty())
					sscanf( strTEXT, _T("%d"), &pTCODE->m_dwUV);

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

				strTEXT = strLINE.Tokenize( strTSEP, nPOS );
				if(!strTEXT.IsEmpty())
				{
					int nIndex = atoi(strTEXT);
					pTCODE->m_dwUV = nIndex;
				}
				
				strTEXT = strLINE.Tokenize( strTSEP, nPOS );
				if(!strTEXT.IsEmpty())
				{
					int nIndex = atoi(strTEXT);
					pTCODE->m_dwUV2 = nIndex;
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
			else if( strTEXT == _T("action") )
			{
				pTCODE = new CTSpCode();
				pTCODE->m_eCode = TSCENE_PLAYER_CODE_ACTION;

				strTEXT = strLINE.Tokenize( strTSEP, nPOS);
				if(!strTEXT.IsEmpty())
					pTCODE->m_nAct = (int) atoi(strTEXT);				
			}
			else if( strTEXT == _T("check_quest"))
			{
				pTCODE = new CTSpCode();
				pTCODE->m_eCode = TSCENE_PLAYER_CODE_CHECK_QUEST;				
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
	// 5 TSCENE_SCRIPT_LEAVE_TUTORIAL
	static CString vTSCENE_SCRIPT_LEAVE_TUTORIAL[] = {
		CString(_T("hide_skip")),
		CString(_T("hide_ui")),
		CString(_T("draw_color_fadein 3000 0xFFFFFFFF")),
		CString(_T("teleport")),
		CString(_T("draw_color 3600000 0xFFFFFFFF"))};

	// 20 TSCENE_SCRIPT_DEFUGEL_ENTER
	static CString vTSCENE_SCRIPT_DEFUGEL_ENTER[] = {
		CString(_T("check_quest")),
		CString(_T("load 0")),
		CString(_T("hide_ui")),
		CString(_T("show_skip")),
		CString(_T("pause_tick")),
		CString(_T("start_camera")),
		CString(_T("draw_color 100 0xFFFFFFFF")),
		CString(_T("show_text 3")),
		CString(_T("fadein_texture 2500 0 0xFFFFFFFF 0")),
		CString(_T("fade_texture 3500 0 0 0 1")),
		CString(_T("show_text 4")),
		CString(_T("draw_texture 6000 0 1 ")),
		CString(_T("hide_skip")),
		CString(_T("show_text 0")),
		CString(_T("fadeout_texture 3500 0 0x00FFFFFF 1")),
		CString(_T("resume_tick")),
		CString(_T("wait 800")),
		CString(_T("calc_camera 3500")),
		CString(_T("show_ui")),
		CString(_T("wait 1000")),
		CString(_T("show_dialog"))};

	// 20 TSCENE_SCRIPT_CRAXION_ENTER
	static CString vTSCENE_SCRIPT_CRAXION_ENTER[] = {
		CString(_T("check_quest")),
		CString(_T("load 1")),
		CString(_T("hide_ui")),
		CString(_T("show_skip")),
		CString(_T("pause_tick")),
		CString(_T("start_camera")),
		CString(_T("draw_color 100 0xFFFFFFFF")),
		CString(_T("show_text 1")),
		CString(_T("fadein_texture 2500 1 0xFFFFFFFF 0")),
		CString(_T("fade_texture 3500 1 1 0 1")),
		CString(_T("show_text 2")),
		CString(_T("draw_texture 6000 1 1 ")),
		CString(_T("hide_skip")),
		CString(_T("show_text 0")),
		CString(_T("fadeout_texture 3500 1 0x00FFFFFF 1")),
		CString(_T("resume_tick")),
		CString(_T("wait 800")),
		CString(_T("calc_camera 3500")),
		CString(_T("show_ui")),
		CString(_T("wait 1000")),
		CString(_T("show_dialog"))};

	// 21 TSCENE_SCRIPT_BROIR_ENTER
	static CString vTSCENE_SCRIPT_BROIR_ENTER[] = {
		CString(_T("check_quest")),
		CString(_T("load 2")),
		CString(_T("hide_ui")),
		CString(_T("show_skip")),
		CString(_T("pause_tick")),
		CString(_T("start_camera")),
		CString(_T("draw_color 100 0xFFFFFFFF")),
		CString(_T("show_text 5")),
		CString(_T("fadein_texture 5500 2 0xFFFFFFFF 0")),
		CString(_T("show_text 6")),
		CString(_T("fade_texture 5500 2 2 0 1")),
		CString(_T("show_text 7")),
		CString(_T("draw_texture 5500 2 1 ")),
		CString(_T("hide_skip")),
		CString(_T("show_text 0")),
		CString(_T("fadeout_texture 2500 2 0x00FFFFFF 1")),
		CString(_T("resume_tick")),
		CString(_T("wait 800")),
		CString(_T("calc_camera 3500")),
		CString(_T("action 36")),
		CString(_T("show_ui")),
		CString(_T("wait 1000")) };

	// 21 TSCENE_SCRIPT_PEACE_ENTER
	static CString vTSCENE_SCRIPT_PEACE_ENTER[] = {
		CString(_T("check_quest")),
		CString(_T("load 4")),
		CString(_T("hide_ui")),
		CString(_T("show_skip")),
		CString(_T("pause_tick")),
		CString(_T("start_camera")),
		CString(_T("draw_color 100 0xFFFFFFFF")),
		CString(_T("show_text 11")),
		CString(_T("fadein_texture 7000 4 0xFFFFFFFF 0")),
		CString(_T("show_text 12")),
		CString(_T("fade_texture 7000 4 4 0 1")),
		CString(_T("show_text 13")),
		CString(_T("draw_texture 7000 4 1 ")),
		CString(_T("hide_skip")),
		CString(_T("show_text 0")),
		CString(_T("fadeout_texture 1500 4 0x00FFFFFF 1")),
		CString(_T("resume_tick")),
		CString(_T("wait 800")),
		CString(_T("calc_camera 3500")),
		CString(_T("show_ui")),
		CString(_T("wait 1000")),
		CString(_T("show_dialog"))};

	// 14 TSCENE_SCRIPT_DEFUGEL_SKIP
	static CString vTSCENE_SCRIPT_DEFUGEL_SKIP[] = {
		CString(_T("check_quest")),
		CString(_T("load 0")),
		CString(_T("hide_ui")),
		CString(_T("hide_skip")),
		CString(_T("pause_tick")),
		CString(_T("start_camera")),
		CString(_T("show_text 0")),
		CString(_T("draw_texture 1000 0 1")),
		CString(_T("fadeout_texture 3500 0 0x00FFFFFF 1")),
		CString(_T("resume_tick")),
		CString(_T("wait 800")),
		CString(_T("calc_camera 3500")),
		CString(_T("show_ui")),
		CString(_T("wait 1000")),
		CString(_T("show_dialog"))};

	// 14 TSCENE_SCRIPT_CRAXION_SKIP
	static CString vTSCENE_SCRIPT_CRAXION_SKIP[] = {
		CString(_T("check_quest")),
		CString(_T("load 1")),
		CString(_T("hide_ui")),
		CString(_T("hide_skip")),
		CString(_T("pause_tick")),
		CString(_T("start_camera")),
		CString(_T("show_text 0")),
		CString(_T("draw_texture 1000 1 1")),
		CString(_T("fadeout_texture 3500 1 0x00FFFFFF 1")),
		CString(_T("resume_tick")),
		CString(_T("wait 800")),
		CString(_T("calc_camera 3500")),
		CString(_T("show_ui")),
		CString(_T("wait 1000")),
		CString(_T("show_dialog"))};

	// 14 TSCENE_SCRIPT_BROIR_SKIP
	static CString vTSCENE_SCRIPT_BROIR_SKIP[] = {
		CString(_T("check_quest")),
		CString(_T("load 2")),
		CString(_T("hide_ui")),
		CString(_T("hide_skip")),
		CString(_T("pause_tick")),
		CString(_T("start_camera")),
		CString(_T("show_text 0")),
		CString(_T("draw_texture 1000 2 1")),
		CString(_T("fadeout_texture 3500 2 0x00FFFFFF 1")),
		CString(_T("resume_tick")),
		CString(_T("wait 800")),
		CString(_T("calc_camera 3500")),
		CString(_T("show_ui")),
		CString(_T("wait 1000")),
		CString(_T("show_dialog"))};

	// 14 vTSCENE_SCRIPT_PEACE_SKIP
	static CString vTSCENE_SCRIPT_PEACE_SKIP[] = {
		CString(_T("check_quest")),
		CString(_T("load 4")),
		CString(_T("hide_ui")),
		CString(_T("hide_skip")),
		CString(_T("pause_tick")),
		CString(_T("start_camera")),
		CString(_T("show_text 0")),
		CString(_T("draw_texture 1000 4 1")),
		CString(_T("fadeout_texture 3500 4 0x00FFFFFF 1")),
		CString(_T("resume_tick")),
		CString(_T("wait 800")),
		CString(_T("calc_camera 3500")),
		CString(_T("show_ui")),
		CString(_T("wait 1000")),
		CString(_T("show_dialog"))};

	static CString* vTSCRIPT[TSCENE_SCRIPT_COUNT] = {
		vTSCENE_SCRIPT_LEAVE_TUTORIAL,
		vTSCENE_SCRIPT_DEFUGEL_ENTER,
		vTSCENE_SCRIPT_CRAXION_ENTER,
		vTSCENE_SCRIPT_BROIR_ENTER,
		vTSCENE_SCRIPT_PEACE_ENTER,
		vTSCENE_SCRIPT_DEFUGEL_SKIP,
		vTSCENE_SCRIPT_CRAXION_SKIP,
		vTSCENE_SCRIPT_BROIR_SKIP,
		vTSCENE_SCRIPT_PEACE_SKIP
	};

	static DWORD vTCOUNT[TSCENE_SCRIPT_COUNT] =
	{
		5, //TSCENE_SCRIPT_LEAVE_TUTORIAL
		21, //TSCENE_SCRIPT_DEFUGEL_ENTER
		21, //TSCENE_SCRIPT_CRAXION_ENTER
		22, //TSCENE_SCRIPT_BROIR_ENTER
		22, //TSCENE_SCRIPT_PEACE_ENTER
		15, //TSCENE_SCRIPT_DEFUGEL_SKIP
		15, //TSCENE_SCRIPT_CRAXION_SKIP
		15, //TSCENE_SCRIPT_BROIR_SKIP
		15, //TSCENE_SCRIPT_PEACE_SKIP
	};

	if( nTSCENE < TSCENE_SCRIPT_COUNT )
	{
		m_nCurScene = nTSCENE;
		LoadScript( pDevice, pRES, vTSCRIPT[m_nCurScene], vTCOUNT[m_nCurScene]);
	}
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