#include "StdAfx.h"


CTachyonMAP::CTachyonMAP()
{
	m_bMAPType = MT_EXT;

	m_mapOBJ.clear();
	m_mapSFX.clear();
	m_mapSND.clear();
}

CTachyonMAP::~CTachyonMAP()
{
	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;

	for( itOBJ = m_mapOBJ.begin(); itOBJ != m_mapOBJ.end(); itOBJ++)
		delete (*itOBJ).second;

	for( itSFX = m_mapSFX.begin(); itSFX != m_mapSFX.end(); itSFX++)
		delete (*itSFX).second;

	for( itSND = m_mapSND.begin(); itSND != m_mapSND.end(); itSND++)
		delete (*itSND).second;

	m_mapOBJ.clear();
	m_mapSFX.clear();
	m_mapSND.clear();
}

FLOAT CTachyonMAP::GetHeight( FLOAT fPosX,
							  FLOAT fPosY,
							  FLOAT fPosZ)
{
	return 0.0f;
}
