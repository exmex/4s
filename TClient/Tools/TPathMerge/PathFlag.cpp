#include "StdAfx.h"
#include "PathFlag.h"

MAPTPATHFLAG CPathFlag::m_mapTPATHFLAG;
WORD CPathFlag::m_wNewID = 1;


CPathFlag::CPathFlag()
{
	m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_dwID = TPATHFLAG_NULL;
}

CPathFlag::~CPathFlag()
{
	MAPTPATHFLAG::iterator finder = m_mapTPATHFLAG.find(m_dwID);

	if( finder != m_mapTPATHFLAG.end() )
		m_mapTPATHFLAG.erase(finder);
}

void CPathFlag::InitTPATH()
{
	m_mapTPATHFLAG.clear();
	m_wNewID = 1;
}

void CPathFlag::ReleaseTPATH()
{
	MAPTPATHFLAG::iterator itTPATH = m_mapTPATHFLAG.begin();

	while(itTPATH != m_mapTPATHFLAG.end())
	{
		MAPTPATHFLAG::iterator itNEXT = itTPATH;
		itNEXT++;

		delete (*itTPATH).second;
		itTPATH = itNEXT;
	}

	m_mapTPATHFLAG.clear();
	m_wNewID = 1;
}

void CPathFlag::RegTPATH( CPathFlag *pTPATH,
						  BYTE bUnitX,
						  BYTE bUnitZ)
{
	WORD wUnitID = MAKEWORD( bUnitX, bUnitZ);

	while( m_wNewID == TPATHFLAG_NULL || m_mapTPATHFLAG.find(MAKELONG( m_wNewID, wUnitID)) != m_mapTPATHFLAG.end() )
		m_wNewID++;

	pTPATH->m_dwID = MAKELONG( m_wNewID, wUnitID);
	m_mapTPATHFLAG.insert( MAPTPATHFLAG::value_type( pTPATH->m_dwID, pTPATH));
}
