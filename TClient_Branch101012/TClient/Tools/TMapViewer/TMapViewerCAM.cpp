#include "stdafx.h"


CTMapViewerCAM::CTMapViewerCAM()
{
	m_fZoomSPEED = 0.0f;
	m_fZoomMIN = 0.0f;
	m_fZoomMAX = 0.0f;

	m_fCamHEIGHT = 0.0f;
	m_fCamDIST = 0.0f;
	m_fPrevDIST = 0.0f;

	m_bCamRotSPEED = 0;
	m_wCamPITCH = 0;
}

CTMapViewerCAM::~CTMapViewerCAM()
{
}

void CTMapViewerCAM::ZoomCamera( DWORD dwTick, int nCamZoom)
{
	while(dwTick)
	{
		DWORD dwZoom = dwTick < 10 ? dwTick : 10;

		m_fCamDIST += FLOAT(nCamZoom) * FLOAT(dwZoom) * m_fCamDIST * m_fZoomSPEED;
		dwTick -= dwZoom;

		if( m_fCamDIST < m_fZoomMIN )
		{
			m_fCamDIST = m_fZoomMIN;
			break;
		}

		if( m_fCamDIST > m_fZoomMAX )
		{
			m_fCamDIST = m_fZoomMAX;
			break;
		}
	}

	if(nCamZoom)
		m_fPrevDIST = m_fCamDIST;
}
