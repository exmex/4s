#include "stdafx.h"
#include "TExplorerWnd.h"
#include "TErrorCode.h"
#include "Resource.h"


int CTExplorerGame::OnGM_MINIMAP_ZOOMOUT()
{
	FLOAT *pTSCALE = NULL;
	FLOAT fMIN = 1.0f;

	if(m_pWorldmap->IsVisible())
	{
		if( m_pWorldmap->m_fTSCALE == TWORLDMAP_MINSCALE )
		{
			FLOAT fPOSX = m_pMainChar->m_vPosition._41;
			FLOAT fPOSZ = m_pMainChar->m_vPosition._43;

			if( m_pWorldmap->GetWorldmapLevel(fPOSX,fPOSZ) == TWORLDMAP_LEVEL_ALL )
				m_pWorldmap->m_bTALLWORLDMODE = TRUE;
		}
		else
		{
			pTSCALE = &(m_pWorldmap->m_fTSCALE);
			fMIN = TWORLDMAP_MINSCALE;
		}
	}

	//if(m_pWorldmap->IsVisible())
	//{
	//	pTSCALE = &m_pWorldmap->m_fTSCALE;
	//	fMIN = TWORLDMAP_MINSCALE;
	//}
	//else
	//{
	//	pTSCALE = &m_vMAP.m_fTSCALE;
	//	fMIN = TMINIMAP_INIT_SCALE / powf( TMINIMAP_SCALE_FACTOR, TMINIMAP_SCALE_COUNT);
	//}

	if(pTSCALE)
		(*pTSCALE) = max( (*pTSCALE) / TMINIMAP_SCALE_FACTOR, fMIN);

	return TERROR_NONE;
}

int CTExplorerGame::OnGM_MINIMAP_ZOOMIN()
{
	FLOAT *pTSCALE = NULL;
	FLOAT fMAX = 1.0f;

	if(m_pWorldmap->IsVisible())
	{
		if( !m_pWorldmap->m_bTALLWORLDMODE )
		{
			pTSCALE = &(m_pWorldmap->m_fTSCALE);
			fMAX = powf( TMINIMAP_SCALE_FACTOR, TMINIMAP_SCALE_COUNT);
		}
		else
			m_pWorldmap->m_bTALLWORLDMODE = FALSE;
	}

	//if(m_pWorldmap->IsVisible())
	//{
	//	pTSCALE = &m_pWorldmap->m_fTSCALE;
	//	fMAX = powf( TMINIMAP_SCALE_FACTOR, TMINIMAP_SCALE_COUNT);
	//}
	//else
	//{
	//	pTSCALE = &m_vMAP.m_fTSCALE;
	//	fMAX = TMINIMAP_INIT_SCALE;
	//}

	if(pTSCALE)
		(*pTSCALE) = min( (*pTSCALE) * TMINIMAP_SCALE_FACTOR, fMAX);

	return TERROR_NONE;
}

int CTExplorerGame::OnGM_OPEN_WORLDMAP()
{
	//if(m_vMAP.m_wMapID)
	//{
	//	CString strTITLE;
	//	CString strTYPE;
	//	CString strMSG;

	//	strMSG.LoadString(IDS_ERROR_CANNOT_OPEN_WORLDMAP);
	//	strTITLE.LoadString(IDS_OPEN_WORLDMAP);
	//	strTYPE.LoadString(IDS_TEXT_ERROR);
	//	ChatMSG( strTYPE, strTITLE, strMSG, 0xFFFFFFF0);

	//	return TERROR_NONE;
	//}

	//m_pWorldmap->m_vTCENTER.x = m_pMainChar->m_vPosition._41;
	//m_pWorldmap->m_vTCENTER.y = m_pMainChar->m_vPosition._43;

	//m_pWorldmap->ShowComponent(TRUE);
	//SwitchFocus(m_pWorldmap);

	//return TERROR_NONE;

	FLOAT fPOSX = m_pMainChar->m_vPosition._41; 
	FLOAT fPOSZ = m_pMainChar->m_vPosition._43;

	if( m_pWorldmap->GetWorldmapLevel(fPOSX,fPOSZ) == TWORLDMAP_LEVEL_NONE )
	{
		CString strTYPE;
		CString strMSG;

		strMSG.LoadString(IDS_ERROR_CANNOT_OPEN_WORLDMAP);
		strTYPE.LoadString(IDS_TEXT_ERROR);
		ChatMSG( strTYPE, m_pMainChar->GetName(), strMSG, 0xFFFFFFF0);

		return TERROR_NONE;
	}

	m_pWorldmap->m_bTALLWORLDMODE = FALSE;
	m_pWorldmap->m_vTCENTER.x = fPOSX;
	m_pWorldmap->m_vTCENTER.y = fPOSZ;

	m_pWorldmap->ShowComponent(TRUE);
	SwitchFocus(m_pWorldmap);

	return TERROR_NONE;
}

int CTExplorerGame::OnGM_SELECT_WORLDMAP()
{
	if( m_pWorldmap->m_pTLASTWBTN )
	{
		MAPDWORD::iterator itr = CTExplorerMAP::m_mapTBTNID2UNIT.find(m_pWorldmap->m_pTLASTWBTN->m_id);
		if( itr != CTExplorerMAP::m_mapTBTNID2UNIT.end() )
		{
			WORD wID = LOWORD(itr->second);
			BYTE bUX = LOBYTE(wID);
			BYTE bUZ = HIBYTE(wID);

			FLOAT fLEN = (FLOAT)(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
			FLOAT fHLEN = fLEN * 0.5f;

			m_pWorldmap->m_vTCENTER.x = FLOAT(bUX) * fLEN + fHLEN;
			m_pWorldmap->m_vTCENTER.y = FLOAT(bUZ) * fLEN + fHLEN;

			m_pWorldmap->m_bTALLWORLDMODE = FALSE;
			m_pWorldmap->m_pTLASTWBTN = NULL;
		}
	}

	return TERROR_NONE;
}

int CTExplorerGame::OnGM_CLOSE_UI()
{
	if( m_pFocus == m_pWorldmap )
		m_pWorldmap->ShowComponent(FALSE);

	return TERROR_NONE;
}
