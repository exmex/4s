#include "StdAfx.h"
#include "Resource.h"

MAPTPATHFLAG CTClientFlag::m_mapTPATHFLAG;


CTClientFlag::CTClientFlag()
{
	m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vPATH.clear();

	m_dwID = TPATHFLAG_NULL;
}

CTClientFlag::~CTClientFlag()
{
	MAPTPATHFLAG::iterator finder = m_mapTPATHFLAG.find(m_dwID);

	if( finder != m_mapTPATHFLAG.end() )
		m_mapTPATHFLAG.erase(finder);

	m_vPATH.clear();
}

void CTClientFlag::InitTPATH()
{
	m_mapTPATHFLAG.clear();
}

void CTClientFlag::ReleaseTPATH()
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
}

CTClientFlag *CTClientFlag::FindTFLAG( DWORD dwID)
{
	MAPTPATHFLAG::iterator finder = m_mapTPATHFLAG.find(dwID);

	if( finder != m_mapTPATHFLAG.end() )
		return (*finder).second;

	return NULL;
}
