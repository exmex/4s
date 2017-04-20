#include "StdAfx.h"
#include "TExplorer.h"


CTExplorerKEY::CTExplorerKEY()
{
	m_point = CPoint( 0, 0);
	m_dwTick = 0;

	ResetSTATE();
	InitKEY();
}

CTExplorerKEY::~CTExplorerKEY()
{
	m_mapKEY.clear();
}

void CTExplorerKEY::ResetSTATE()
{
	memset( m_vSTATE, 0, TKEY_COUNT * sizeof(BYTE));
	memset( m_vTICK, 0, TKEY_COUNT * sizeof(DWORD));
}

void CTExplorerKEY::InitKEY()
{
	m_mapKEY.clear();
	for( WORD i=1; i<TKEY_COUNT; i++)
		m_mapKEY.insert( MAPWORD::value_type( CTExplorerApp::m_vKEY[i], i));
}

WORD CTExplorerKEY::GetTKEY( WORD wKEY)
{
	MAPWORD::iterator finder = m_mapKEY.find(wKEY);

	if( finder == m_mapKEY.end() )
		return TKEY_NONE;

	return (*finder).second;
}

BYTE CTExplorerKEY::GetCTRL()
{
	return GetKeyState(VK_CONTROL) < 0 ? 0x01 : 0x00;
}
