#include "StdAfx.h"
#include "TMapViewer.h"


CTMapViewerKEY::CTMapViewerKEY()
{
	m_point = CPoint( 0, 0);
	m_dwTick = 0;

	ResetSTATE();
	InitKEY();
}

CTMapViewerKEY::~CTMapViewerKEY()
{
	m_mapKEY.clear();
}

void CTMapViewerKEY::ResetSTATE()
{
	memset( m_vSTATE, 0, TKEY_COUNT * sizeof(BYTE));
	memset( m_vTICK, 0, TKEY_COUNT * sizeof(DWORD));
}

void CTMapViewerKEY::InitKEY()
{
	m_mapKEY.clear();
	for( WORD i=1; i<TKEY_COUNT; i++)
		m_mapKEY.insert( MAPWORD::value_type( CTMapViewerApp::m_vKEY[i], i));
}

WORD CTMapViewerKEY::GetTKEY( WORD wKEY)
{
	MAPWORD::iterator finder = m_mapKEY.find(wKEY);

	if( finder == m_mapKEY.end() )
		return TKEY_NONE;

	return (*finder).second;
}

BYTE CTMapViewerKEY::GetCTRL()
{
	return GetKeyState(VK_CONTROL) < 0 ? 0x01 : 0x00;
}
