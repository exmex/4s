#include "stdafx.h"
#include "TMapViewerWnd.h"
#include "TErrorCode.h"
#include "Resource.h"


DWORD CTMapViewerGame::OnTCMD_EXIT( CString strParam)
{
	PostQuitMessage(0);
	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_LOADCHAR( CString strParam)
{
	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_LOADMAP( CString strParam)
{
	DWORD dwMapID = 0;

	if(strParam.IsEmpty())
		return TERROR_LOADMAP;
	m_pMainWnd->m_bResetTick = TRUE;

	TRY
	{
		sscanf( LPCSTR(strParam), "%d", &dwMapID);
		m_vMAP.ReleaseMAP();

		m_vMAP.LoadMAP(
			m_pDevice,
			m_pCamera,
			m_pRESMAP,
			WORD(dwMapID));

		m_vMAP.m_pRESCHAR = m_pRES;

		if(!m_vMAP.m_pMAP)
			return TERROR_LOADMAP_NOTFOUND;

		ResetTSKY();
	}
	CATCH_ALL(e)
	{
		return TERROR_LOADMAP;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_MOVE( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_MOVE;

	TRY
	{
		sscanf( LPCSTR(strParam), "%f %f", &m_pMainChar->m_vWorld._41, &m_pMainChar->m_vWorld._43);
		m_pMainChar->m_vWorld._42 = m_vMAP.GetHeight(
			&m_vMAP.m_vDRAWOBJ,
			m_pMainChar,
			&D3DXVECTOR3(
			m_pMainChar->m_vWorld._41,
			m_pMainChar->m_vWorld._42,
			m_pMainChar->m_vWorld._43),
			0.0f, FALSE);
		ResetTSKY();
	}
	CATCH_ALL(e)
	{
		return TERROR_MOVE;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_FPS( CString strParam)
{
	m_bFPS = !m_bFPS;
	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_MINIMAP( CString strParam)
{
	m_pTMinimap->ShowComponent(!m_pTMinimap->IsVisible());
	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_CLIMB(CString strParam)
{
	static FLOAT fClimb = 0.5;
	if(!m_bClimb)
		m_vMAP.m_fClimbBound = 0.0f;
	else
		m_vMAP.m_fClimbBound = fClimb;

	CString strMSG;
	strMSG.Format("Climb 체크가 %s 되었습니다.", m_bClimb ? "활성화" : "비활성화");

	m_bClimb = !m_bClimb;
	
	ChatMSG("상태","C",strMSG, 0xFFFF0000);
	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_SPEED( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_SPEED;

	TRY
	{
		sscanf( LPCSTR(strParam), "%f", &m_pMainChar->m_fSpeedFactor);
	}
	CATCH_ALL(e)
	{
		return TERROR_SPEED;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_POS( CString strParam)
{
	m_bPOS = !m_bPOS;
	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_HELP( CString strParam)
{
	CString strTYPE;
	CString strTMSG;

	strTYPE.LoadString(IDS_TEXT_HELP);
	strTMSG.LoadString(IDS_MSG_HELP);
	ChatMSG( strTYPE, m_strTCMD[TCMD_HELP], strTMSG, 0xFFFFFFFF);

	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_LIGHT( CString strParam)
{
	m_bLIGHT = !m_bLIGHT;
	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_SHADOW( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_SHADOW;

	TRY
	{
		FLOAT fSHADOW;

		sscanf( LPCSTR(strParam), _T("%f"), &fSHADOW);
		m_vMAP.m_bSHADOW = BYTE(fSHADOW * 0xFF);

		if(m_vMAP.m_pMAP)
			m_vMAP.m_pMAP->m_bSHADOW = m_vMAP.m_bSHADOW;
	}
	CATCH_ALL(e)
	{
		return TERROR_SHADOW;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_MATERIALA( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_MATERIALA;

	TRY
	{
		sscanf( LPCSTR(strParam), _T("%f %f %f"), &m_vShadow.Ambient.r, &m_vShadow.Ambient.g, &m_vShadow.Ambient.b);
	}
	CATCH_ALL(e)
	{
		return TERROR_MATERIALA;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_MATERIALD( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_MATERIALD;

	TRY
	{
		sscanf( LPCSTR(strParam), _T("%f %f %f"), &m_vShadow.Diffuse.r, &m_vShadow.Diffuse.g, &m_vShadow.Diffuse.b);
	}
	CATCH_ALL(e)
	{
		return TERROR_MATERIALD;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_MAPCOLOR( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_MAPCOLOR;

	TRY
	{
		FLOAT fRValue = 0.0f;
		FLOAT fGValue = 0.0f;
		FLOAT fBValue = 0.0f;

		sscanf( LPCSTR(strParam), _T("%f %f %f"), &fRValue, &fGValue, &fBValue);
		m_vMAP.m_dwTMapCOLOR = D3DCOLOR_ARGB( 0xFF, BYTE(fRValue * 0xFF), BYTE(fGValue * 0xFF), BYTE(fBValue * 0xFF));
	}
	CATCH_ALL(e)
	{
		return TERROR_MAPCOLOR;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_LANDCOLLISION( CString strParam)
{
	m_bLandCollision = !m_bLandCollision;
	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_LIGHTMODE( CString strParam)
{
	m_bTLIGHTMode = !m_bTLIGHTMode;
	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_LIGHTA( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_AMBIENT;

	TRY
	{
		FLOAT fRValue = 0.0f;
		FLOAT fGValue = 0.0f;
		FLOAT fBValue = 0.0f;
		DWORD dwIndex = 0;

		sscanf( LPCSTR(strParam), _T("%d %f %f %f"), &dwIndex, &fRValue, &fGValue, &fBValue);
		if( dwIndex < TLIGHT_COUNT )
		{
			FLOAT fUnitLength = FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
			BYTE bUnitX = BYTE(m_pCamera->m_vTarget.x / fUnitLength);
			BYTE bUnitZ = BYTE(m_pCamera->m_vTarget.z / fUnitLength);

			LPTLIGHT pTLIGHT = NULL;

			pTLIGHT = FindTLIGHT(
				m_vMAP.m_pMAP->m_nUnitCountX,
				TLIGHTSET_MAIN, dwIndex,
				m_vMAP.m_wMapID,
				bUnitX,
				bUnitZ);

			m_vLIGHT[dwIndex].m_Light.Ambient.r = fRValue;
			m_vLIGHT[dwIndex].m_Light.Ambient.g = fGValue;
			m_vLIGHT[dwIndex].m_Light.Ambient.b = fBValue;

			if(pTLIGHT != NULL)
			{
				pTLIGHT->m_fAmbientR = fRValue;
				pTLIGHT->m_fAmbientG = fGValue;
				pTLIGHT->m_fAmbientB = fBValue;

				SetTLIGHT(
					m_vMAP.m_pMAP->m_nUnitCountX,
					TLIGHTSET_MAIN, dwIndex,
					m_vMAP.m_wMapID,
					bUnitX,
					bUnitZ,
					pTLIGHT);
			}

			m_vLIGHT[dwIndex].SetLight(&m_vLIGHT[dwIndex].m_Light);
		}
	}
	CATCH_ALL(e)
	{
		return TERROR_AMBIENT;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_LIGHTD( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_DIFFUSE;

	TRY
	{
		FLOAT fRValue = 0.0f;
		FLOAT fGValue = 0.0f;
		FLOAT fBValue = 0.0f;
		DWORD dwIndex = 0;

		sscanf( LPCSTR(strParam), _T("%d %f %f %f"), &dwIndex, &fRValue, &fGValue, &fBValue);
		if( dwIndex < TLIGHT_COUNT )
		{
			FLOAT fUnitLength = FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
			BYTE bUnitX = BYTE(m_pCamera->m_vTarget.x / fUnitLength);
			BYTE bUnitZ = BYTE(m_pCamera->m_vTarget.z / fUnitLength);

			LPTLIGHT pTLIGHT = NULL;

			pTLIGHT = FindTLIGHT(
				m_vMAP.m_pMAP->m_nUnitCountX,
				TLIGHTSET_MAPCOLOR, dwIndex,
				m_vMAP.m_wMapID,
				bUnitX,
				bUnitZ);

			m_vLIGHT[dwIndex].m_Light.Diffuse.r = fRValue;
			m_vLIGHT[dwIndex].m_Light.Diffuse.g = fGValue;
			m_vLIGHT[dwIndex].m_Light.Diffuse.b = fBValue;

			if(pTLIGHT != NULL)
			{
				pTLIGHT->m_fDiffuseR = fRValue;
				pTLIGHT->m_fDiffuseG = fGValue;
				pTLIGHT->m_fDiffuseB = fBValue;

				SetTLIGHT(
					m_vMAP.m_pMAP->m_nUnitCountX,
					TLIGHTSET_MAPCOLOR, dwIndex,
					m_vMAP.m_wMapID,
					bUnitX,
					bUnitZ,
					pTLIGHT);
			}

			m_vLIGHT[dwIndex].SetLight(&m_vLIGHT[dwIndex].m_Light);
		}
	}
	CATCH_ALL(e)
	{
		return TERROR_DIFFUSE;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_LIGHTDIR( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_LIGHTDIR;

	TRY
	{
		D3DXVECTOR3 vDIR( 0.0f, 0.0f, 0.0f);
		DWORD dwIndex = 0;

		sscanf( LPCSTR(strParam), _T("%d %f %f %f"), &dwIndex, &vDIR.x, &vDIR.y, &vDIR.z);
		if( dwIndex < TLIGHT_COUNT )
		{
			FLOAT fUnitLength = FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
			BYTE bUnitX = BYTE(m_pCamera->m_vTarget.x / fUnitLength);
			BYTE bUnitZ = BYTE(m_pCamera->m_vTarget.z / fUnitLength);

			LPTLIGHT pTLIGHT = NULL;

			pTLIGHT = FindTLIGHT(
				m_vMAP.m_pMAP->m_nUnitCountX,
				TLIGHTSET_MAPCOLOR, dwIndex,
				m_vMAP.m_wMapID,
				bUnitX,
				bUnitZ);

			m_vLIGHT[dwIndex].m_Light.Direction.x = vDIR.x;
			m_vLIGHT[dwIndex].m_Light.Direction.y = vDIR.y;
			m_vLIGHT[dwIndex].m_Light.Direction.z = vDIR.z;

			if(pTLIGHT != NULL)
			{
				pTLIGHT->m_vDIR = m_vLIGHT[dwIndex].m_Light.Direction;

				SetTLIGHT(
					m_vMAP.m_pMAP->m_nUnitCountX,
					TLIGHTSET_MAPCOLOR, dwIndex,
					m_vMAP.m_wMapID,
					bUnitX,
					bUnitZ,
					pTLIGHT);
			}

			m_vLIGHT[dwIndex].SetLight(&m_vLIGHT[dwIndex].m_Light);
		}
	}
	CATCH_ALL(e)
	{
		return TERROR_LIGHTDIR;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_FOGCOLOR( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_FOGCOLOR;

	TRY
	{
		FLOAT fRValue = 0.0f;
		FLOAT fGValue = 0.0f;
		FLOAT fBValue = 0.0f;

		sscanf( LPCSTR(strParam), _T("%f %f %f"), &fRValue, &fGValue, &fBValue);

		FLOAT fUnitLength = FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
		BYTE bUnitX = BYTE(m_pCamera->m_vTarget.x / fUnitLength);
		BYTE bUnitZ = BYTE(m_pCamera->m_vTarget.z / fUnitLength);

		LPTUNITFOG pTFOG = FindTFOG(
			m_vMAP.m_wMapID,
			bUnitX,
			bUnitZ);

		pTFOG->m_pTGLOBAL->m_bRValue = BYTE(INT(fRValue * 255) % 255);
		pTFOG->m_pTGLOBAL->m_bGValue = BYTE(INT(fGValue * 255) % 255);
		pTFOG->m_pTGLOBAL->m_bBValue = BYTE(INT(fBValue * 255) % 255);

		SetTFOG(pTFOG, MAKELONG(MAKEWORD( bUnitX, bUnitZ), m_vMAP.m_wMapID));
	}
	CATCH_ALL(e)
	{
		return TERROR_FOGCOLOR;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}
DWORD CTMapViewerGame::OnTCMD_FOGRANGE( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_FOGRANGE;

	TRY
	{
		FLOAT fSTART = 0.0f;
		FLOAT fEND = 0.0f;

		sscanf( LPCSTR(strParam), _T("%f %f"), &fSTART, &fEND);
		
		FLOAT fUnitLength = FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
		BYTE bUnitX = BYTE(m_pCamera->m_vTarget.x / fUnitLength);
		BYTE bUnitZ = BYTE(m_pCamera->m_vTarget.z / fUnitLength);

		LPTUNITFOG pTFOG = FindTFOG(
			m_vMAP.m_wMapID,
			bUnitX,
			bUnitZ);

		pTFOG->m_pTGLOBAL->m_fSTART = fSTART;
		pTFOG->m_pTGLOBAL->m_fEND	= fEND;
		
		SetTFOG(pTFOG, MAKELONG(MAKEWORD( bUnitX, bUnitZ), m_vMAP.m_wMapID));
	}
	CATCH_ALL(e)
	{
		return TERROR_FOGRANGE;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTMapViewerGame::OnTCMD_FOGDENSITY( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_FOGDENSITY;

	TRY
	{
		FLOAT fDENSITY = 0.0f;

		sscanf( LPCSTR(strParam), _T("%f"), &fDENSITY);

		FLOAT fUnitLength = FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
		BYTE bUnitX = BYTE(m_pCamera->m_vTarget.x / fUnitLength);
		BYTE bUnitZ = BYTE(m_pCamera->m_vTarget.z / fUnitLength);

		LPTUNITFOG pTFOG = FindTFOG(
			m_vMAP.m_wMapID,
			bUnitX,
			bUnitZ);

		pTFOG->m_pTGLOBAL->m_fDENSITY = fDENSITY;
		
		SetTFOG(pTFOG, MAKELONG(MAKEWORD( bUnitX, bUnitZ), m_vMAP.m_wMapID));
	}
	CATCH_ALL(e)
	{
		return TERROR_FOGDENSITY;
	}
	END_CATCH_ALL

		return TERROR_NONE;
}

