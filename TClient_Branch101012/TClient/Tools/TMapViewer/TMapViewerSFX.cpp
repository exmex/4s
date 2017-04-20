#include "stdafx.h"
#include "TMapViewer.h"

MAPTSFXTEMP CTMapViewerSFX::m_mapTSFXTEMP;


CTMapViewerSFX::CTMapViewerSFX()
{
	m_dwID = 0;
}

CTMapViewerSFX::~CTMapViewerSFX()
{
}

LPTSFXTEMP CTMapViewerSFX::GetTSFXTEMP( DWORD dwFuncID)
{
	MAPTSFXTEMP::iterator finder = m_mapTSFXTEMP.find(dwFuncID);

	if( finder != m_mapTSFXTEMP.end() )
		return (*finder).second;

	return NULL;
}
