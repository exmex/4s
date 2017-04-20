#include "StdAfx.h"
#include "TGameDB.h"
#include "TNpcposSet.h"
#include "TMonposSet.h"


CTGameDB::CTGameDB()
{
	m_mapTMONIDINDEX.clear();
	m_mapTKINDINDEX.clear();
	m_mapTNDATA.clear();
	m_vTMDATA.clear();
}

CTGameDB::~CTGameDB()
{
	ReleaseDATA();
}

void CTGameDB::ReleaseDATA()
{
	MAPTNDATA::iterator itTNDATA;

	for( itTNDATA = m_mapTNDATA.begin(); itTNDATA != m_mapTNDATA.end(); itTNDATA++)
		delete (*itTNDATA).second;
	m_mapTNDATA.clear();

	while(!m_vTMDATA.empty())
	{
		delete m_vTMDATA.back();
		m_vTMDATA.pop_back();
	}

	ClearINDEX(&m_mapTMONIDINDEX);
	ClearINDEX(&m_mapTKINDINDEX);
}

BYTE CTGameDB::LoadDATA()
{
	CDatabase vDB;
	ReleaseDATA();

	if(!vDB.Open(_T("TGAME")))
		return FALSE;

	CTNpcposSet vTNPC(&vDB);
	if(!vTNPC.Open())
		return FALSE;

	if(!vTNPC.IsEOF())
		vTNPC.MoveFirst();

	while(!vTNPC.IsEOF())
	{
		LPTNDATA pTNDATA = FindTNDATA(vTNPC.m_wID);

		if(!pTNDATA)
		{
			pTNDATA = new TNDATA();
			m_mapTNDATA.insert(MAPTNDATA::value_type( vTNPC.m_wID, pTNDATA));
		}

		pTNDATA->m_strTITLE = vTNPC.m_szName2;
		pTNDATA->m_strNAME = vTNPC.m_szName;
		pTNDATA->m_wID = vTNPC.m_wID;

		pTNDATA->m_wMapID = vTNPC.m_wMapID;
		pTNDATA->m_fPosX = vTNPC.m_fPosX;
		pTNDATA->m_fPosY = vTNPC.m_fPosY;
		pTNDATA->m_fPosZ = vTNPC.m_fPosZ;

		vTNPC.MoveNext();
	}
	vTNPC.Close();

	CTMonposSet vTMON(&vDB);
	if(!vTMON.Open())
	{
		ReleaseDATA();
		return FALSE;
	}

	if(!vTMON.IsEOF())
		vTMON.MoveFirst();

	while(!vTMON.IsEOF())
	{
		LPTMDATA pTMDATA = new TMDATA();

		pTMDATA->m_strTITLE = vTMON.m_szName2;
		pTMDATA->m_strNAME = vTMON.m_szName;
		pTMDATA->m_wMonID = vTMON.m_wMonID;
		pTMDATA->m_wKind = vTMON.m_wKind;

		pTMDATA->m_wMapID = vTMON.m_wMapID;
		pTMDATA->m_fPosX = vTMON.m_fPosX;
		pTMDATA->m_fPosY = vTMON.m_fPosY;
		pTMDATA->m_fPosZ = vTMON.m_fPosZ;

		AddToINDEX(
			&m_mapTMONIDINDEX,
			pTMDATA->m_wMonID,
			pTMDATA);

		AddToINDEX(
			&m_mapTKINDINDEX,
			pTMDATA->m_wKind,
			pTMDATA);

		m_vTMDATA.push_back(pTMDATA);
		vTMON.MoveNext();
	}
	vTMON.Close();

	return TRUE;
}

void CTGameDB::AddToINDEX( LPMAPMAPTMDATA pINDEX,
						   DWORD dwID,
						   LPTMDATA pTMDATA)
{
	LPMAPTMDATA pMAPTMDATA = FindMAPTMDATA( pINDEX, dwID);

	if(!pMAPTMDATA)
	{
		pMAPTMDATA = new MAPTMDATA();
		pMAPTMDATA->clear();

		pINDEX->insert(MAPMAPTMDATA::value_type( dwID, pMAPTMDATA));
	}

	LPVTMDATA pVTMDATA = FindVTMDATA(
		pMAPTMDATA,
		pTMDATA->m_wMapID);

	if(!pVTMDATA)
	{
		pVTMDATA = new VTMDATA();
		pVTMDATA->clear();

		pMAPTMDATA->insert(MAPTMDATA::value_type( pTMDATA->m_wMapID, pVTMDATA));
	}

	pVTMDATA->push_back(pTMDATA);
}

void CTGameDB::ClearINDEX( LPMAPMAPTMDATA pINDEX)
{
	MAPMAPTMDATA::iterator itINDEX;

	for( itINDEX = pINDEX->begin(); itINDEX != pINDEX->end(); itINDEX++)
	{
		LPMAPTMDATA pTMDATA = (*itINDEX).second;
		MAPTMDATA::iterator it;

		for( it = pTMDATA->begin(); it != pTMDATA->end(); it++)
		{
			(*it).second->clear();
			delete (*it).second;
		}

		pTMDATA->clear();
		delete pTMDATA;
	}

	pINDEX->clear();
}

LPVTMDATA CTGameDB::FindVTMDATA( LPMAPTMDATA pMAPTMDATA,
								 DWORD dwMapID)
{
	MAPTMDATA::iterator finder = pMAPTMDATA->find(dwMapID);

	if( finder != pMAPTMDATA->end() )
		return (*finder).second;

	return NULL;
}

LPMAPTMDATA CTGameDB::FindMAPTMDATA( LPMAPMAPTMDATA pINDEX,
									 DWORD dwID)
{
	MAPMAPTMDATA::iterator finder = pINDEX->find(dwID);

	if( finder != pINDEX->end() )
		return (*finder).second;

	return NULL;
}

LPTNDATA CTGameDB::FindTNDATA( DWORD dwID)
{
	MAPTNDATA::iterator finder = m_mapTNDATA.find(dwID);

	if( finder != m_mapTNDATA.end() )
		return (*finder).second;

	return NULL;
}
