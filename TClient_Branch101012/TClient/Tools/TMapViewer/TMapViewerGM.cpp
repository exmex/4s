#include "stdafx.h"
#include "TMapViewerWnd.h"
#include "TErrorCode.h"
#include "Resource.h"


int CTMapViewerGame::OnGM_MINIMAP_ZOOMOUT()
{
	FLOAT *pTSCALE = NULL;
	FLOAT fMIN = 1.0f;

	if(m_pWorldmap->IsVisible())
	{
		pTSCALE = &m_pWorldmap->m_fTSCALE;
		fMIN = TWORLDMAP_MINSCALE;
	}
	else
	{
		pTSCALE = &m_vMAP.m_fTSCALE;
		fMIN = TMINIMAP_INIT_SCALE / powf( TMINIMAP_SCALE_FACTOR, TMINIMAP_SCALE_COUNT);
	}

	if(pTSCALE)
		(*pTSCALE) = max( (*pTSCALE) / TMINIMAP_SCALE_FACTOR, fMIN);

	return TERROR_NONE;
}

int CTMapViewerGame::OnGM_MINIMAP_ZOOMIN()
{
	FLOAT *pTSCALE = NULL;
	FLOAT fMAX = 1.0f;

	if(m_pWorldmap->IsVisible())
	{
		pTSCALE = &m_pWorldmap->m_fTSCALE;
		fMAX = powf( TMINIMAP_SCALE_FACTOR, TMINIMAP_SCALE_COUNT);
	}
	else
	{
		pTSCALE = &m_vMAP.m_fTSCALE;
		fMAX = TMINIMAP_INIT_SCALE;
	}

	if(pTSCALE)
		(*pTSCALE) = min( (*pTSCALE) * TMINIMAP_SCALE_FACTOR, fMAX);

	return TERROR_NONE;
}

int CTMapViewerGame::OnGM_OPEN_WORLDMAP()
{
	/*if(m_vMAP.m_wMapID)
	{
		CString strTITLE;
		CString strTYPE;
		CString strMSG;

		strMSG.LoadString(IDS_ERROR_CANNOT_OPEN_WORLDMAP);
		strTITLE.LoadString(IDS_OPEN_WORLDMAP);
		strTYPE.LoadString(IDS_TEXT_ERROR);
		ChatMSG( strTYPE, strTITLE, strMSG, 0xFFFFFFF0);

		return TERROR_NONE;
	}

	m_pWorldmap->m_vTCENTER.x = m_pMainChar->m_vPosition._41;
	m_pWorldmap->m_vTCENTER.y = m_pMainChar->m_vPosition._43;

	m_pWorldmap->ShowComponent(TRUE);
	SwitchFocus(m_pWorldmap);*/

	return TERROR_NONE;
}

int CTMapViewerGame::OnGM_CLOSE_UI()
{
	if( m_pFocus == m_pWorldmap )
		m_pWorldmap->ShowComponent(FALSE);

	return TERROR_NONE;
}
