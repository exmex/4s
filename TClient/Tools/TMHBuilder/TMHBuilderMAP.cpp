#include "StdAfx.h"

MAPDWORD CTMHBuilderMAP::m_mapTMAPINFO;

FLOAT CTMHBuilderMAP::m_fClimbBound = 0.5f;
FLOAT CTMHBuilderMAP::m_fMoveBound = 0.58f;
FLOAT CTMHBuilderMAP::m_fFallBound = 1.5f;
FLOAT CTMHBuilderMAP::m_fGravity = 10.0f;


CTMHBuilderMAP::CTMHBuilderMAP()
{
	m_mapTHEIGHTINFO.clear();
	m_mapTOBJ.clear();

	m_pMAP = NULL;
	m_wMapID = 0;
}

CTMHBuilderMAP::~CTMHBuilderMAP()
{
	ReleaseMAP();
}

BYTE CTMHBuilderMAP::LockMAP( CTachyonRes *pRES,
							  WORD wMapID)
{
	DWORD dwMapID = GetMapID(wMapID);
	ReleaseMAP();

	if( dwMapID == 0xFFFFFFFF )
		return FALSE;

	m_pMAP = (CTachyonHUGEMAP *) pRES->LockMAP(dwMapID);
	m_wMapID = wMapID;

	return TRUE;
}

void CTMHBuilderMAP::LoadUNIT( CD3DDevice *pDevice,
							   int nUnitID)
{
	static int vSIDE[9][2] = {
		{ -1, -1}, {  0, -1}, {  1, -1},
		{ -1,  0}, {  0,  0}, {  1,  0},
		{ -1,  1}, {  0,  1}, {  1,  1}};

	MAPTMHOBJ::iterator itTOBJ;
	MAPMAPOBJ::iterator itOBJ;
	ReleaseUNIT();

	int nSIZE = m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength;
	int nUnitX = nUnitID % m_pMAP->m_nUnitCountX;
	int nUnitZ = nUnitID / m_pMAP->m_nUnitCountX;

	for( int i=0; i<9; i++)
	{
		int nX = nUnitX + vSIDE[i][0];
		int nZ = nUnitZ + vSIDE[i][1];

		if( nX >= 0 && nX < m_pMAP->m_nUnitCountX &&
			nZ >= 0 && nZ < m_pMAP->m_nUnitCountZ )
		{
			int nUNIT = nZ * m_pMAP->m_nUnitCountX + nX;

			if(m_pMAP->m_pENABLE[nUNIT])
			{
				m_pMAP->LoadUNIT(
					pDevice->m_pDevice,
					&m_pMAP->m_UNIT[0],
					nUNIT);

				for( itOBJ = m_pMAP->m_UNIT[0].m_mapOBJ.begin(); itOBJ != m_pMAP->m_UNIT[0].m_mapOBJ.end(); itOBJ++)
				{
					CTMHBuilderOBJ *pTOBJ = new CTMHBuilderOBJ();

					pTOBJ->InitOBJ( pDevice, (*itOBJ).second);
					pTOBJ->m_bCollisionType = pTOBJ->GetAttrINT(ID_COLLISION_TYPE);
					pTOBJ->m_bCamCollision = pTOBJ->GetAttrINT(ID_CAM_COLLISION);
					pTOBJ->m_bType = pTOBJ->GetAttrINT(ID_OBJ_TYPE);

					if( pTOBJ->m_bCollisionType == TCOLLISION_NONE && pTOBJ->m_bCamCollision )
						pTOBJ->m_bCollisionType = TCOLLISION_POLY;

					switch(pTOBJ->m_bType)
					{
					case OT_SILHOUETTE	:
					case OT_FOG			: delete pTOBJ; break;
					case OT_WATER		:
						{
							pTOBJ->m_bCollisionType = TCOLLISION_POLY;

							pTOBJ->InitSIZE(
								FLOAT(nSIZE),
								0.0f,
								FLOAT(nSIZE),
								0.0f);

							m_mapTOBJ.insert( MAPTMHOBJ::value_type( (*itOBJ).first, pTOBJ));
						}

						break;

					default				:
						if( pTOBJ->m_bCollisionType != TCOLLISION_NONE )
						{
							pTOBJ->InitSIZE(
								(*itOBJ).second->m_vSCALE.x * pTOBJ->GetAttrFLOAT(ID_SIZE_X),
								(*itOBJ).second->m_vSCALE.y * pTOBJ->GetAttrFLOAT(ID_SIZE_Y),
								(*itOBJ).second->m_vSCALE.z * pTOBJ->GetAttrFLOAT(ID_SIZE_Z),
								(*itOBJ).second->m_vSCALE.y * pTOBJ->GetAttrFLOAT(ID_DEPTH));

							m_mapTOBJ.insert( MAPTMHOBJ::value_type( (*itOBJ).first, pTOBJ));
						}
						else
							delete pTOBJ;

						break;
					}
				}
			}
		}
	}

	nUnitX *= nSIZE;
	nUnitZ *= nSIZE;

	for( itTOBJ = m_mapTOBJ.begin(); itTOBJ != m_mapTOBJ.end(); itTOBJ++)
	{
		int nGRID = (*itTOBJ).second->m_bType == OT_WATER ? TWATERGRID_SIZE : TGRID_SIZE;

		int nStartX = INT((*itTOBJ).second->m_vPosition._41 - (*itTOBJ).second->m_fRadiusH) - nUnitX;
		int nStartZ = INT((*itTOBJ).second->m_vPosition._43 - (*itTOBJ).second->m_fRadiusH) - nUnitZ;
		int nEndX = INT((*itTOBJ).second->m_vPosition._41 + (*itTOBJ).second->m_fRadiusH) + 1 - nUnitX;
		int nEndZ = INT((*itTOBJ).second->m_vPosition._43 + (*itTOBJ).second->m_fRadiusH) + 1 - nUnitZ;

		nStartX = max( 0, nStartX / nGRID * nGRID);
		nStartZ = max( 0, nStartZ / nGRID * nGRID);
		nEndX = min( nSIZE, nEndX / nGRID * nGRID + nGRID);
		nEndZ = min( nSIZE, nEndZ / nGRID * nGRID + nGRID);

		for( int i = nStartX; i < nEndX; i += nGRID)
			for( int j = nStartZ; j < nEndZ; j += nGRID)
			{
				DWORD dwINDEX = MAKELONG(
					WORD(i / nGRID),
					WORD(j / nGRID));

				if((*itTOBJ).second->RectTest( FLOAT(nUnitX + i), FLOAT(nUnitZ + j), FLOAT(nGRID)))
				{
					MAPTHEIGHTINFO::iterator finder = m_mapTHEIGHTINFO.find(dwINDEX);
					LPVECTORDWORD pTINFO = NULL;

					if( finder == m_mapTHEIGHTINFO.end() )
					{
						pTINFO = new VECTORDWORD();
						pTINFO->clear();

						m_mapTHEIGHTINFO.insert( MAPTHEIGHTINFO::value_type( dwINDEX, pTINFO));
					}
					else
						pTINFO = (*finder).second;

					pTINFO->push_back((*itTOBJ).first);
				}
			}
	}
}

void CTMHBuilderMAP::ReleaseMAP()
{
	ReleaseUNIT();

	if(m_pMAP)
	{
		delete m_pMAP;
		m_pMAP = NULL;
	}
	m_wMapID = 0;
}

void CTMHBuilderMAP::ReleaseUNIT()
{
	MAPTHEIGHTINFO::iterator itINFO;
	MAPTMHOBJ::iterator itTOBJ;

	for( itINFO = m_mapTHEIGHTINFO.begin(); itINFO != m_mapTHEIGHTINFO.end(); itINFO++)
	{
		(*itINFO).second->clear();
		delete (*itINFO).second;
	}

	for( itTOBJ = m_mapTOBJ.begin(); itTOBJ != m_mapTOBJ.end(); itTOBJ++)
		delete (*itTOBJ).second;

	m_mapTHEIGHTINFO.clear();
	m_mapTOBJ.clear();
}

DWORD CTMHBuilderMAP::GetMapID( WORD wMapID)
{
	MAPDWORD::iterator finder = m_mapTMAPINFO.find(wMapID);

	if( finder != m_mapTMAPINFO.end() )
		return (*finder).second;

	return 0xFFFFFFFF;
}

void CTMHBuilderMAP::InitMAPINFO()
{
	CFile vTMAP( ".\\Tcd\\TMAP.tcd", CFile::modeRead|CFile::typeBinary);
	CArchive vTMAPCHART( &vTMAP, CArchive::load);
	WORD wCount;

	ReleaseMAPINFO();
	vTMAPCHART
		>> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		DWORD dwMapID;
		DWORD dwID;

		FLOAT fScaleX;
		FLOAT fScaleY;
		FLOAT fScaleZ;

		FLOAT fPosX;
		FLOAT fPosY;
		FLOAT fPosZ;

		FLOAT fRotX;
		FLOAT fRotY;
		FLOAT fRotZ;

		BYTE bDungeon;
		BYTE bNPCCALL;

		vTMAPCHART
			>> dwMapID
			>> dwID
			>> fScaleX
			>> fScaleY
			>> fScaleZ
			>> fPosX
			>> fPosY
			>> fPosZ
			>> fRotX
			>> fRotY
			>> fRotZ
			>> bDungeon
			>> bNPCCALL;

		m_mapTMAPINFO.insert( MAPDWORD::value_type( dwMapID, dwID));
	}
}

void CTMHBuilderMAP::ReleaseMAPINFO()
{
	m_mapTMAPINFO.clear();
}
