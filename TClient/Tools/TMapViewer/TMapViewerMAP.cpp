#include "StdAfx.h"
#include "TMapViewerGame.h"
#include "Resource.h"


MAPTREGIONINFO CTMapViewerMAP::m_mapTREGIONINFO;
MAPTMAPINFO CTMapViewerMAP::m_mapTMAPINFO;
MAPTMINIMAP CTMapViewerMAP::m_mapTMINIMAP;
LPDIRECT3DTEXTURE9 CTMapViewerMAP::m_pTPOS = NULL;

FLOAT CTMapViewerMAP::m_fClimbBound = 0.5f;
FLOAT CTMapViewerMAP::m_fMoveBound = 0.58f;
FLOAT CTMapViewerMAP::m_fFallBound = 1.5f;
FLOAT CTMapViewerMAP::m_fGravity = 10.0f;


CTMapViewerMAP::CTMapViewerMAP()
{
	m_dwTMapCOLOR = 0xFFFFFFFF;
	m_dwBspID = NODEID_NULL;
	m_wMapID = 0;
	m_bSHADOW = 0x60;
	m_fTSCALE = 5.0f;

	m_nTCellX = -(TDRAWCELL_WIDTH / 2 + 1);
	m_nTCellZ = -(TDRAWCELL_WIDTH / 2 + 1);
	m_pMAP = NULL;
	m_pRESCHAR = NULL;

	for( WORD i=0; i<9; i++)
	{
		for( WORD j=0; j<TOBJCELL_COUNT; j++)
		{
			m_vTMAPOBJ[i][j].clear();
			m_vTMAPSFX[i][j].clear();
			m_vTMAPSND[i][j].clear();
		}

		m_vTSILHOUETTE[i].m_vTOBJ.clear();
		m_vBIGOBJ[i].m_vTOBJ.clear();
		m_vTBSP[i].m_vTBSP.clear();

		m_vTSILHOUETTE[i].m_wCount = 0;
		m_vBIGOBJ[i].m_wCount = 0;
		m_vTBSP[i].m_wCount = 0;
	}

	for( i=0; i<TDRAWCELL_COUNT; i++)
	{
		m_vTOBJ[i].m_vTOBJ.clear();
		m_vTSFX[i].m_vTSFX.clear();
		m_vTSND[i].m_vTSND.clear();

		m_vTOBJ[i].m_wCount = 0;
		m_vTSND[i].m_wCount = 0;

		m_vTCELL[i] = 0xFFFF;
		m_vLOCK[i] = FALSE;
	}

	m_vDRAWOBJ.clear();
	m_vDRAWBSP.clear();
	m_vTPATH.clear();
	m_vWATER.clear();
	m_vTDRAW.clear();
}

CTMapViewerMAP::~CTMapViewerMAP()
{
	ReleaseMAP();

	for( WORD i=0; i<9; i++)
	{
		while(!m_vTSILHOUETTE[i].m_vTOBJ.empty())
		{
			delete m_vTSILHOUETTE[i].m_vTOBJ.back();
			m_vTSILHOUETTE[i].m_vTOBJ.pop_back();
		}

		while(!m_vBIGOBJ[i].m_vTOBJ.empty())
		{
			delete m_vBIGOBJ[i].m_vTOBJ.back();
			m_vBIGOBJ[i].m_vTOBJ.pop_back();
		}

		while(!m_vTBSP[i].m_vTBSP.empty())
		{
			delete m_vTBSP[i].m_vTBSP.back();
			m_vTBSP[i].m_vTBSP.pop_back();
		}
	}

	for( i=0; i<TDRAWCELL_COUNT; i++)
	{
		while(!m_vTOBJ[i].m_vTOBJ.empty())
		{
			delete m_vTOBJ[i].m_vTOBJ.back();
			m_vTOBJ[i].m_vTOBJ.pop_back();
		}

		while(!m_vTSND[i].m_vTSND.empty())
		{
			delete m_vTSND[i].m_vTSND.back();
			m_vTSND[i].m_vTSND.pop_back();
		}

		while(!m_vTSFX[i].m_vTSFX.empty())
		{
			delete m_vTSFX[i].m_vTSFX.back();
			m_vTSFX[i].m_vTSFX.pop_back();
		}
	}
}

void CTMapViewerMAP::ReleaseMAP()
{
	for( WORD i=0; i<9; i++)
	{
		ClearTSILHOUETTE(BYTE(i));
		ClearBIGOBJ(BYTE(i));
		ClearTBSP(BYTE(i));

		for( WORD j=0; j<TOBJCELL_COUNT; j++)
		{
			m_vTMAPOBJ[i][j].clear();
			m_vTMAPSFX[i][j].clear();
			m_vTMAPSND[i][j].clear();
		}
	}

	for( i=0; i<TDRAWCELL_COUNT; i++)
	{
		ClearMAPOBJ(
			&m_vTOBJ[i],
			&m_vTSFX[i],
			&m_vTSND[i]);

		m_vTCELL[i] = 0xFFFF;
		m_vLOCK[i] = FALSE;
	}

	if(m_pMAP)
	{
		delete m_pMAP;
		m_pMAP = NULL;
	}

	m_vDRAWOBJ.clear();
	m_vDRAWBSP.clear();
	m_vTPATH.clear();
	m_vWATER.clear();
	m_vTDRAW.clear();

	m_mapTBSP.clear();

	m_nTCellX = -(TDRAWCELL_WIDTH / 2 + 1);
	m_nTCellZ = -(TDRAWCELL_WIDTH / 2 + 1);

	m_dwBspID = NODEID_NULL;
	m_wMapID = 0;
}

FLOAT CTMapViewerMAP::GetAttrFLOAT( LPMAPOBJ pTOBJ,
								   DWORD dwID)
{
	MAPATTRIBUTE::iterator finder = pTOBJ->m_mapATTR.find(dwID);

	if( finder == pTOBJ->m_mapATTR.end() )
		return 0;

	return *((FLOAT *) (*finder).second->m_pValue);
}

int CTMapViewerMAP::GetAttrINT( LPMAPOBJ pTOBJ,
							   DWORD dwID)
{
	MAPATTRIBUTE::iterator finder = pTOBJ->m_mapATTR.find(dwID);

	if( finder == pTOBJ->m_mapATTR.end() )
		return 0;

	return *((int *) (*finder).second->m_pValue);
}

void CTMapViewerMAP::LoadMAP( CD3DDevice *pDevice,
							 CTMapViewerCAM *pCamera,
							 CTachyonRes *pRES,
							 WORD wMapID)
{
	LPTMAPINFO pTMAPINFO = GetMapINFO(wMapID);

	m_pMAP = (CTachyonHUGEMAP *) pRES->LockMAP(pTMAPINFO->m_dwMapID);
	m_wMapID = wMapID;

	for( int i=0; i<9; i++)
	{
		m_pMAP->m_bINDEX[i] = 0xFF;
		m_pMAP->m_bUNIT[i] = 0xFF;
	}

	m_pMAP->m_bSHADOW = m_bSHADOW;
	m_pMAP->m_nUnitX = -2;
	m_pMAP->m_nUnitZ = -2;
	m_pMAP->m_nCellX = -2;
	m_pMAP->m_nCellZ = -2;

	ResetTMAP( pDevice, pCamera, 0);
}

void CTMapViewerMAP::LoadMAPOBJ( CD3DDevice *pDevice, LPD3DXVECTOR3 pPOS)
{
	for( BYTE i=0; i<9; i++)
		if(m_pMAP->m_bUPDATE[i])
		{
			BYTE bUnitX = m_pMAP->m_nUnitX + i % 3;
			BYTE bUnitZ = m_pMAP->m_nUnitZ + i / 3;

			FLOAT fUnitLength = FLOAT(m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);
			FLOAT fUnitX = FLOAT(bUnitX) * fUnitLength;
			FLOAT fUnitZ = FLOAT(bUnitZ) * fUnitLength;

			MAPMAPOBJ::iterator itOBJ;
			MAPMAPSFX::iterator itSFX;
			MAPMAPSND::iterator itSND;

			for( WORD j=0; j<TOBJCELL_COUNT; j++)
			{
				m_vTMAPOBJ[m_pMAP->m_bUNIT[i]][j].clear();
				m_vTMAPSFX[m_pMAP->m_bUNIT[i]][j].clear();
				m_vTMAPSND[m_pMAP->m_bUNIT[i]][j].clear();
			}

			ClearTSILHOUETTE(m_pMAP->m_bUNIT[i]);
			ClearBIGOBJ(m_pMAP->m_bUNIT[i]);
			ClearTBSP(m_pMAP->m_bUNIT[i]);

			ClearTFLAG(i);
			LoadTFLAG(i);

			for( itOBJ = m_pMAP->m_pUNIT[i]->m_mapOBJ.begin(); itOBJ != m_pMAP->m_pUNIT[i]->m_mapOBJ.end(); itOBJ++)
			{
				BYTE bOBJType = GetAttrINT( (*itOBJ).second, ID_OBJ_TYPE);

				switch(bOBJType)
				{
				case OT_SILHOUETTE	:
					{
						if( m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_wCount >= WORD(m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ.size()) )
							m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ.push_back(new CTMapViewerObject());
						CTMapViewerObject *pINST = (CTMapViewerObject *) m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ[m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_wCount];

						m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_wCount++;
						pINST->ReleaseData();
						pINST->m_bZWrite = FALSE;

						pINST->InitOBJ( pDevice, (*itOBJ).second);
						pINST->m_bCollisionType = pINST->GetAttrINT(ID_COLLISION_TYPE);
						pINST->m_bCamCollision = pINST->GetAttrINT(ID_CAM_COLLISION);
						pINST->m_dwID = pINST->GetAttrINT(ID_MAP_ID);
						pINST->m_bType = pINST->GetAttrINT(ID_OBJ_TYPE);

						pINST->InitSIZE(
							(*itOBJ).second->m_vSCALE.x * pINST->GetAttrFLOAT(ID_SIZE_X),
							(*itOBJ).second->m_vSCALE.y * pINST->GetAttrFLOAT(ID_SIZE_Y),
							(*itOBJ).second->m_vSCALE.z * pINST->GetAttrFLOAT(ID_SIZE_Z),
							(*itOBJ).second->m_vSCALE.y * pINST->GetAttrFLOAT(ID_DEPTH));
					}

					break;

				case OT_FOG			: break;
				case OT_BSP			:
					{
						if( m_vTBSP[m_pMAP->m_bUNIT[i]].m_wCount >= WORD(m_vTBSP[m_pMAP->m_bUNIT[i]].m_vTBSP.size()) )
							m_vTBSP[m_pMAP->m_bUNIT[i]].m_vTBSP.push_back(new CTMapViewerBSP());
						CTMapViewerBSP *pTBSP = m_vTBSP[m_pMAP->m_bUNIT[i]].m_vTBSP[m_vTBSP[m_pMAP->m_bUNIT[i]].m_wCount];

						m_vTBSP[m_pMAP->m_bUNIT[i]].m_wCount++;
						pTBSP->m_dwBspID = GetAttrINT(
							(*itOBJ).second,
							ID_MAP_ID);
						pTBSP->m_pINFO = GetMapINFO(pTBSP->m_dwBspID);

						pTBSP->m_pMAP = (CTachyonBSPMAP *) m_pMAP->m_pRES->LockMAP(pTBSP->m_pINFO->m_dwMapID);
						pTBSP->InitTBSP((*itOBJ).second);

						m_mapTBSP.insert( MAPTBSP::value_type( pTBSP->m_dwBspID, pTBSP));
					}
				default				:
					{
						FLOAT fSizeX = GetAttrFLOAT( (*itOBJ).second, ID_SIZE_X);
						FLOAT fSizeZ = GetAttrFLOAT( (*itOBJ).second, ID_SIZE_Z);

						FLOAT fLocalX = (*itOBJ).second->m_vPOS.x - fUnitX;
						FLOAT fLocalZ = (*itOBJ).second->m_vPOS.z - fUnitZ;

						if( bOBJType == OT_WATER || (bOBJType == OT_COLLISION || bOBJType == OT_PATHWALL ? 1.0f : 2.0f) * TOBJCELL_LENGTH < max( fSizeX, fSizeZ) )
						{
							if( m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_wCount >= WORD(m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_vTOBJ.size()) )
								m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_vTOBJ.push_back(new CTMapViewerObject());
							CTMapViewerObject *pINST = (CTMapViewerObject *) m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_vTOBJ[m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_wCount];

							m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_wCount++;
							pINST->ReleaseData();
							pINST->m_bZWrite = bOBJType == OT_WATER ? FALSE : TRUE;

							pINST->InitOBJ( pDevice, (*itOBJ).second);
							pINST->m_bCollisionType = pINST->GetAttrINT(ID_COLLISION_TYPE);
							pINST->m_bCamCollision = pINST->GetAttrINT(ID_CAM_COLLISION);
							pINST->m_dwID = pINST->GetAttrINT(ID_MAP_ID);
							pINST->m_bType = pINST->GetAttrINT(ID_OBJ_TYPE);

							pINST->InitSIZE(
								(*itOBJ).second->m_vSCALE.x * pINST->GetAttrFLOAT(ID_SIZE_X),
								(*itOBJ).second->m_vSCALE.y * pINST->GetAttrFLOAT(ID_SIZE_Y),
								(*itOBJ).second->m_vSCALE.z * pINST->GetAttrFLOAT(ID_SIZE_Z),
								(*itOBJ).second->m_vSCALE.y * pINST->GetAttrFLOAT(ID_DEPTH));
						}
						else if( 0.0f <= fLocalX && fLocalX < fUnitLength && 0.0f <= fLocalZ && fLocalZ < fUnitLength )
						{
							int nLocalX = INT(fLocalX / TOBJCELL_LENGTH);
							int nLocalZ = INT(fLocalZ / TOBJCELL_LENGTH);
							int nIndex = nLocalZ * TOBJCELL_WIDTH + nLocalX;

							m_vTMAPOBJ[m_pMAP->m_bUNIT[i]][nIndex].push_back((*itOBJ).second);
						}
					}

					break;
				}
			}

			for( itSFX = m_pMAP->m_pUNIT[i]->m_mapSFX.begin(); itSFX != m_pMAP->m_pUNIT[i]->m_mapSFX.end(); itSFX++)
			{
				FLOAT fLocalX = (*itSFX).second->m_vPOS.x - fUnitX;
				FLOAT fLocalZ = (*itSFX).second->m_vPOS.z - fUnitZ;

				if( 0.0f <= fLocalX && fLocalX < fUnitLength &&
					0.0f <= fLocalZ && fLocalZ < fUnitLength )
				{
					int nLocalX = INT(fLocalX / TOBJCELL_LENGTH);
					int nLocalZ = INT(fLocalZ / TOBJCELL_LENGTH);
					int nIndex = nLocalZ * TOBJCELL_WIDTH + nLocalX;

					m_vTMAPSFX[m_pMAP->m_bUNIT[i]][nIndex].push_back((*itSFX).second);
				}
			}

			for( itSND = m_pMAP->m_pUNIT[i]->m_mapSND.begin(); itSND != m_pMAP->m_pUNIT[i]->m_mapSND.end(); itSND++)
			{
				FLOAT fLocalX = (*itSND).second->m_vPOS.x - fUnitX;
				FLOAT fLocalZ = (*itSND).second->m_vPOS.z - fUnitZ;

				if( 0.0f <= fLocalX && fLocalX < fUnitLength &&
					0.0f <= fLocalZ && fLocalZ < fUnitLength )
				{
					int nLocalX = INT(fLocalX / TOBJCELL_LENGTH);
					int nLocalZ = INT(fLocalZ / TOBJCELL_LENGTH);
					int nIndex = nLocalZ * TOBJCELL_WIDTH + nLocalX;

					m_vTMAPSND[m_pMAP->m_bUNIT[i]][nIndex].push_back((*itSND).second);
				}
			}
		}
}

void CTMapViewerMAP::LoadMAPOBJ( CD3DDevice *pDevice,
								LPVTMAPOBJ pTMAPOBJ,
								LPVTMAPSFX pTMAPSFX,
								LPVTMAPSND pTMAPSND,
								LPTOBJBLOCK pTOBJ,
								LPTSFXBLOCK pTSFX,
								LPTSNDBLOCK pTSND)
{
	for( int i=0; i<INT(pTMAPOBJ->size()); i++)
	{
		if( pTOBJ->m_wCount >= WORD(pTOBJ->m_vTOBJ.size()) )
			pTOBJ->m_vTOBJ.push_back(new CTMapViewerObject());
		CTMapViewerObject *pINST = (CTMapViewerObject *) pTOBJ->m_vTOBJ[pTOBJ->m_wCount];

		pINST->ReleaseData();
		pTOBJ->m_wCount++;
		pINST->InitOBJ( pDevice, (*pTMAPOBJ)[i]);

		pINST->m_bCollisionType = pINST->GetAttrINT(ID_COLLISION_TYPE);
		pINST->m_bCamCollision = pINST->GetAttrINT(ID_CAM_COLLISION);
		pINST->m_dwID = pINST->GetAttrINT(ID_MAP_ID);
		pINST->m_bType = pINST->GetAttrINT(ID_OBJ_TYPE);

		pINST->InitSIZE(
			(*pTMAPOBJ)[i]->m_vSCALE.x * pINST->GetAttrFLOAT(ID_SIZE_X),
			(*pTMAPOBJ)[i]->m_vSCALE.y * pINST->GetAttrFLOAT(ID_SIZE_Y),
			(*pTMAPOBJ)[i]->m_vSCALE.z * pINST->GetAttrFLOAT(ID_SIZE_Z),
			(*pTMAPOBJ)[i]->m_vSCALE.y * pINST->GetAttrFLOAT(ID_DEPTH));
	}

	for( i=0; i<INT(pTMAPSFX->size()); i++)
	{
		if( pTSFX->m_wCount >= WORD(pTSFX->m_vTSFX.size()) )
			pTSFX->m_vTSFX.push_back(new CTMapViewerSFX());
		CTMapViewerSFX *pINST = (CTMapViewerSFX *) pTSFX->m_vTSFX[pTSFX->m_wCount];
		D3DXMATRIX vRotX;
		D3DXMATRIX vRotY;
		D3DXMATRIX vRotZ;

		pTSFX->m_wCount++;
		pINST->InitSFX(
			pDevice,
			(*pTMAPSFX)[i]->m_pSFX);

		pINST->m_bHasHost = TRUE;
		D3DXMatrixRotationX(
			&vRotX,
			(*pTMAPSFX)[i]->m_vROT.x);

		D3DXMatrixRotationY(
			&vRotY,
			(*pTMAPSFX)[i]->m_vROT.y);

		D3DXMatrixRotationZ(
			&vRotZ,
			(*pTMAPSFX)[i]->m_vROT.z);

		pINST->m_vGlobal = vRotX * vRotY * vRotZ;
		pINST->m_vGlobal._41 = (*pTMAPSFX)[i]->m_vPOS.x;
		pINST->m_vGlobal._42 = (*pTMAPSFX)[i]->m_vPOS.y;
		pINST->m_vGlobal._43 = (*pTMAPSFX)[i]->m_vPOS.z;
		pINST->CalcTick( pDevice->m_pDevice, 0);

		pINST->m_dwID = CTachyonSFX::RegSFX( pINST, (*pTMAPSFX)[i]->m_bBias);
	}

	for( i=0; i<INT(pTMAPSND->size()); i++)
	{
		if( pTSND->m_wCount >= WORD(pTSND->m_vTSND.size()) )
			pTSND->m_vTSND.push_back(new SNDIDX());
		LPSNDIDX pIDX = pTSND->m_vTSND[pTSND->m_wCount];
		pTSND->m_wCount++;

		pIDX->m_pSND = (CD3DSound *) CTachyonRes::m_MEDIA.GetData(
			MEDIA_DSOUND,
			(*pTMAPSND)[i]->m_dwMediaID);
		pIDX->m_nIndex = pIDX->m_pSND->Lock();

		if( pIDX->m_nIndex >= 0 )
		{
			pIDX->m_pSND->GetDS3D(pIDX->m_nIndex)->SetPosition(
				(*pTMAPSND)[i]->m_vPOS.x,
				(*pTMAPSND)[i]->m_vPOS.y,
				(*pTMAPSND)[i]->m_vPOS.z,
				DS3D_IMMEDIATE);
		}
		else
		{
			pIDX->m_nIndex = -1;
			pIDX->m_pSND = NULL;

			pTSND->m_wCount--;
		}
	}
}

void CTMapViewerMAP::ClearMAPOBJ( LPTOBJBLOCK pTOBJ,
								 LPTSFXBLOCK pTSFX,
								 LPTSNDBLOCK pTSND)
{
	for( WORD i=0; i<pTOBJ->m_wCount; i++)
		pTOBJ->m_vTOBJ[i]->Release();

	
	for( i=0; i<pTSFX->m_wCount; i++)
	{
		CTMapViewerSFX *pINST = (CTMapViewerSFX *) pTSFX->m_vTSFX[i];
		MAPTSFX::iterator finder = CTachyonSFX::m_mapINST.find(pINST->m_dwID);

		if( finder != CTachyonSFX::m_mapINST.end() )
			CTachyonSFX::m_mapINST.erase(finder);

		pINST->m_dwID = 0;
		pINST->Release();
	}

	for( i=0; i<pTSND->m_wCount; i++)
	{
		if(pTSND->m_vTSND[i]->m_pSND)
			pTSND->m_vTSND[i]->m_pSND->Unlock(pTSND->m_vTSND[i]->m_nIndex);

		pTSND->m_vTSND[i]->m_dwMediaID = 0;
		pTSND->m_vTSND[i]->m_nIndex = -1;
		pTSND->m_vTSND[i]->m_pSND = NULL;
	}

	pTOBJ->m_wCount = 0;
	pTSFX->m_wCount = 0;
	pTSND->m_wCount = 0;
}

void CTMapViewerMAP::CheckMAPOBJ( CD3DDevice *pDevice,
								 LPD3DXVECTOR3 pPOS)
{
	int nTCellX = INT(pPOS->x / TOBJCELL_LENGTH) - TDRAWCELL_WIDTH / 2;
	int nTCellZ = INT(pPOS->z / TOBJCELL_LENGTH) - TDRAWCELL_WIDTH / 2;

	if( nTCellX != m_nTCellX ||
		nTCellZ != m_nTCellZ )
	{
		FLOAT fUnitLength = FLOAT(m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);
		WORD vTCELL[TDRAWCELL_COUNT];

		for( WORD i=0; i<TDRAWCELL_COUNT; i++)
		{
			vTCELL[i] = m_vTCELL[i];
			m_vTCELL[i] = 0xFFFF;
			m_vLOCK[i] = FALSE;
		}

		for( i=0; i<TDRAWCELL_COUNT; i++)
			if( vTCELL[i] != 0xFFFF )
			{
				int nLocalX = m_nTCellX + i % TDRAWCELL_WIDTH - nTCellX;
				int nLocalZ = m_nTCellZ + i / TDRAWCELL_WIDTH - nTCellZ;

				if( nLocalX < 0 || nLocalX > TDRAWCELL_WIDTH - 1 ||
					nLocalZ < 0 || nLocalZ > TDRAWCELL_WIDTH - 1 )
				{
					ClearMAPOBJ(
						&m_vTOBJ[vTCELL[i]],
						&m_vTSFX[vTCELL[i]],
						&m_vTSND[vTCELL[i]]);
				}
				else
				{
					WORD wIndex = nLocalZ * TDRAWCELL_WIDTH + nLocalX;

					m_vTCELL[wIndex] = vTCELL[i];
					m_vLOCK[vTCELL[i]] = TRUE;
				}
			}

		for( i=0; i<TDRAWCELL_COUNT; i++)
			if( m_vTCELL[i] == 0xFFFF )
			{
				FLOAT fPosX = FLOAT(nTCellX + i % TDRAWCELL_WIDTH) * TOBJCELL_LENGTH;
				FLOAT fPosZ = FLOAT(nTCellZ + i / TDRAWCELL_WIDTH) * TOBJCELL_LENGTH;

				int nUnitX = INT(fPosX / fUnitLength) - m_pMAP->m_nUnitX;
				int nUnitZ = INT(fPosZ / fUnitLength) - m_pMAP->m_nUnitZ;

				if( nUnitX >= 0 && nUnitX < 3 &&
					nUnitZ >= 0 && nUnitZ < 3 )
				{
					BYTE bIndex = nUnitZ * 3 + nUnitX;

					if(m_pMAP->m_pUNIT[bIndex])
					{
						FLOAT fLocalX = fPosX - FLOAT(nUnitX + m_pMAP->m_nUnitX) * fUnitLength;
						FLOAT fLocalZ = fPosZ - FLOAT(nUnitZ + m_pMAP->m_nUnitZ) * fUnitLength;

						int nLocalX = INT(fLocalX / TOBJCELL_LENGTH);
						int nLocalZ = INT(fLocalZ / TOBJCELL_LENGTH);

						if( 0 <= nLocalX && nLocalX < TOBJCELL_WIDTH &&
							0 <= nLocalZ && nLocalZ < TOBJCELL_WIDTH )
						{
							int nIndex = nLocalZ * TOBJCELL_WIDTH + nLocalX;

							for( WORD j=0; j<TDRAWCELL_COUNT; j++)
								if(!m_vLOCK[j])
								{
									m_vLOCK[j] = TRUE;
									m_vTCELL[i] = j;

									LoadMAPOBJ(
										pDevice,
										&m_vTMAPOBJ[m_pMAP->m_bUNIT[bIndex]][nIndex],
										&m_vTMAPSFX[m_pMAP->m_bUNIT[bIndex]][nIndex],
										&m_vTMAPSND[m_pMAP->m_bUNIT[bIndex]][nIndex],
										&m_vTOBJ[j],
										&m_vTSFX[j],
										&m_vTSND[j]);

									break;
								}
						}
					}
				}
			}

		m_nTCellX = nTCellX;
		m_nTCellZ = nTCellZ;
	}
}

FLOAT CTMapViewerMAP::GetIntensity()
{
	return m_pMAP->m_fIntensity;
}

FLOAT CTMapViewerMAP::GetSpecRange()
{
	return m_pMAP->m_fSpecRange;
}

FLOAT CTMapViewerMAP::GetSpecMax()
{
	return m_pMAP->m_fSpecMax;
}

FLOAT CTMapViewerMAP::GetHeight( LPVTOBJBASE pTMAPOBJ,
							    CTMapViewerObjBase *pOBJ,
							    LPD3DXVECTOR3 pPOINT,
								FLOAT fMove,
								BYTE bCheckBound)
{
	FLOAT fResult = m_pMAP->GetHeight(
		pPOINT->x,
		pPOINT->y,
		pPOINT->z);

	FLOAT fPrevH = pPOINT->y + fMove / CTMapViewerMAP::m_fMoveBound;
	FLOAT fLandH = fResult;

	if( pPOINT->y + pOBJ->m_fSizeY < fResult ||
		GetRegionID( pPOINT->x, pPOINT->z) == BLANK_REGION )
		fResult = TMIN_HEIGHT;

	for( int i=0; i<INT(pTMAPOBJ->size()); i++)
	{
		FLOAT fLength = D3DXVec2Length(&D3DXVECTOR2(
			(*pTMAPOBJ)[i]->m_vPosition._41 - pPOINT->x,
			(*pTMAPOBJ)[i]->m_vPosition._43 - pPOINT->z));

		if( (*pTMAPOBJ)[i]->m_bCollisionType != TCOLLISION_NONE && (*pTMAPOBJ)[i] != pOBJ &&
			fLength < pOBJ->m_fRadius + (*pTMAPOBJ)[i]->m_fRadius )
		{
			FLOAT fHeight = (*pTMAPOBJ)[i]->GetHeight(
				pOBJ,
				&D3DXVECTOR3(
				pPOINT->x,
				fPrevH,
				pPOINT->z));

			if( fResult < fHeight && fHeight <= fPrevH )
				fResult = fHeight;
		}
	}

	if( fResult <= TMIN_HEIGHT )
		fResult = fLandH;

	return fResult + THEIGHT_ZERO;
}

FLOAT CTMapViewerMAP::GetLandHeight( FLOAT fPosX,
								    FLOAT fPosY,
									FLOAT fPosZ)
{
	return m_pMAP ? m_pMAP->GetHeight(
		fPosX,
		fPosY,
		fPosZ) + THEIGHT_ZERO : 0.0f;
}

FLOAT CTMapViewerMAP::GetWaterHeight( FLOAT fPosX,
									 FLOAT fPosZ)
{
	for( int i=0; i<INT(m_vWATER.size()); i++)
	{
		CTMapViewerObject *pOBJ = (CTMapViewerObject *) m_vWATER[i];

		if(pOBJ)
		{
			MAPCLKINST::iterator it;

			for( it = pOBJ->m_OBJ.m_mapCLK.begin(); it != pOBJ->m_OBJ.m_mapCLK.end(); it++)
			{
				LPCLKINST pCI = (*it).second;

				if( pCI && pCI->m_pMESH && pCI->m_pMESH->m_pMESH &&
					pCI->m_pMESH->m_pMESH->m_pDATA &&
					pCI->m_pMESH->m_pMESH->m_pDATA->m_dwCount )
				{
					LPMESHVERTEX pDATA = (LPMESHVERTEX) pCI->m_pMESH->m_pMESH->m_pDATA->m_pVB;
					CTachyonMesh *pMESH = pCI->m_pMESH->m_pMESH;

					for( int j=0; j<pMESH->m_dwMeshCount; j++)
						for( int k=0; k<pMESH->m_pMESH[j][0]->m_dwCount / 3; k++)
						{
							D3DXVECTOR3 vPoint[3];

							FLOAT fDist;
							FLOAT fU;
							FLOAT fV;

							for( int l=0; l<3; l++)
							{
								vPoint[l] = D3DXVECTOR3(
									pDATA[pMESH->m_pMESH[j][0]->m_pIB[k * 3 + l]].m_fPosX,
									pDATA[pMESH->m_pMESH[j][0]->m_pIB[k * 3 + l]].m_fPosY,
									pDATA[pMESH->m_pMESH[j][0]->m_pIB[k * 3 + l]].m_fPosZ);

								CTMath::Transform(
									&pOBJ->m_vPosition,
									&vPoint[l]);
							}

							if( D3DXIntersectTri(
								&vPoint[0],
								&vPoint[1],
								&vPoint[2],
								&D3DXVECTOR3(
								fPosX,
								0.0f,
								fPosZ),
								&D3DXVECTOR3(
								0.0f,
								1.0f,
								0.0f),
								&fU, &fV,
								&fDist))
								return fDist;
						}
				}
			}
		}
	}

	return -100.0f;
}

D3DXPLANE CTMapViewerMAP::GetBoundPlane( LPD3DXVECTOR3 pPREV,
									    LPD3DXVECTOR3 pNEXT)
{
	CTachyonEXTMAP *pMAP = NULL;
	D3DXPLANE vResult;

	int nUnitX = 0;
	int nUnitZ = 0;

	CPoint point(
		(INT) (pNEXT->x / FLOAT(m_pMAP->m_nTileLength)),
		(INT) (pNEXT->z / FLOAT(m_pMAP->m_nTileLength)));

	for( int i=0; i<9; i++)
		if(m_pMAP->m_pUNIT[i])
		{
			nUnitX = m_pMAP->m_nUnitLength * (m_pMAP->m_nUnitX + i % 3);
			nUnitZ = m_pMAP->m_nUnitLength * (m_pMAP->m_nUnitZ + i / 3);

			CRect rect(
				0, 0,
				m_pMAP->m_nUnitLength,
				m_pMAP->m_nUnitLength);

			rect.OffsetRect(
				nUnitX,
				nUnitZ);

			if(rect.PtInRect(point))
			{
				pMAP = m_pMAP->m_pUNIT[i];
				break;
			}
		}

	if( pMAP && pMAP->m_pDATA )
	{
		FLOAT fUnitX = pNEXT->x / FLOAT(m_pMAP->m_nTileLength) - nUnitX;
		FLOAT fUnitZ = pNEXT->z / FLOAT(m_pMAP->m_nTileLength) - nUnitZ;

		int nPosX = INT(fUnitX);
		int nPosZ = INT(fUnitZ);

		FLOAT fResX = fUnitX - nPosX;
		FLOAT fResZ = fUnitZ - nPosZ;

		if( fResX + fResZ < 1.0f )
		{
			D3DXVECTOR3 vPoint[3] = {
				D3DXVECTOR3( 0.0f, pMAP->m_pDATA[(nPosZ + 1) * (m_pMAP->m_nUnitLength + 1) + nPosX], 1.0f),
				D3DXVECTOR3( 1.0f, pMAP->m_pDATA[nPosZ * (m_pMAP->m_nUnitLength + 1) + nPosX + 1], 0.0f),
				D3DXVECTOR3( 0.0f, pMAP->m_pDATA[nPosZ * (m_pMAP->m_nUnitLength + 1) + nPosX], 0.0f)};

			D3DXPlaneFromPoints(
				&vResult,
				&vPoint[0],
				&vPoint[1],
				&vPoint[2]);
		}
		else
		{
			D3DXVECTOR3 vPoint[3] = {
				D3DXVECTOR3( 1.0f, pMAP->m_pDATA[(nPosZ + 1) * (m_pMAP->m_nUnitLength + 1) + nPosX + 1], 1.0f),
				D3DXVECTOR3( 1.0f, pMAP->m_pDATA[nPosZ * (m_pMAP->m_nUnitLength + 1) + nPosX + 1], 0.0f),
				D3DXVECTOR3( 0.0f, pMAP->m_pDATA[(nPosZ + 1) * (m_pMAP->m_nUnitLength + 1) + nPosX], 1.0f)};

			D3DXPlaneFromPoints(
				&vResult,
				&vPoint[0],
				&vPoint[1],
				&vPoint[2]);
		}

		D3DXPlaneNormalize(
			&vResult,
			&vResult);

		D3DXVECTOR3 vNormal(
			vResult.a,
			vResult.b,
			vResult.c);
		vNormal /= D3DXVec3Length(&vNormal);

		if( D3DXVec3Dot( &vNormal, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f)) < m_fClimbBound )
		{
			vNormal = D3DXVECTOR3(
				vResult.a,
				0.0f,
				vResult.c);
			vNormal /= D3DXVec3Length(&vNormal);

			D3DXPlaneFromPointNormal(
				&vResult,
				pPREV,
				&vNormal);
		}
		else
		{
			D3DXPlaneFromPointNormal(
				&vResult,
				&D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
				&D3DXVECTOR3( 0.0f, 1.0f, 0.0f));
		}
	}
	else
	{
		D3DXPlaneFromPointNormal(
			&vResult,
			&D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
			&D3DXVECTOR3( 0.0f, 1.0f, 0.0f));
	}

	return vResult;
}

LPTREGIONINFO CTMapViewerMAP::GetRegionINFO( BYTE bContryID,
										    FLOAT fPosX,
											FLOAT fPosZ)
{
	BYTE bRegionID = GetRegionID( fPosX, fPosZ);

	BYTE bUnitX = INT(fPosX) / (m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);
	BYTE bUnitZ = INT(fPosZ) / (m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);
	BYTE bUnitID = bUnitZ * m_pMAP->m_nUnitCountX + bUnitX;

	return GetRegionINFO(MAKELONG( MAKEWORD( bRegionID, bUnitID), MAKEWORD( bContryID, BYTE(m_wMapID))));
}

LPTREGIONINFO CTMapViewerMAP::GetRegionINFO( DWORD dwRegionID)
{
	MAPTREGIONINFO::iterator finder = m_mapTREGIONINFO.find(dwRegionID);

	if( finder != m_mapTREGIONINFO.end() )
		return (*finder).second;

	return NULL;
}

LPTMAPINFO CTMapViewerMAP::GetMapINFO( DWORD dwMapID)
{
	MAPTMAPINFO::iterator finder = m_mapTMAPINFO.find(dwMapID);

	if( finder != m_mapTMAPINFO.end() )
		return (*finder).second;

	return NULL;
}

BYTE CTMapViewerMAP::GetRegionID( FLOAT fPosX,
								 FLOAT fPosZ)
{
	int nTileX = INT(fPosX) / m_pMAP->m_nTileLength;
	int nTileZ = INT(fPosZ) / m_pMAP->m_nTileLength;

	return m_pMAP->GetRegionID( nTileX, nTileZ);
}

BYTE CTMapViewerMAP::PtInSHADOW( FLOAT fPosX,
							    FLOAT fPosZ)
{
	return m_pMAP->PtInSHADOW( fPosX, fPosZ);
}

void CTMapViewerMAP::ResetTMINIMAP( LPDIRECT3DTEXTURE9 pTARGET,
								   CTMapViewerChar *pHost,
								   CD3DDevice *pDevice,
								   CTMapViewerCAM *pCamera)
{
	LPDIRECT3DSURFACE9 pTMINIMAP = NULL;
	LPDIRECT3DSURFACE9 pBACK = NULL;
	LPDIRECT3DSURFACE9 pZBUF = NULL;
	CD3DCamera vCAMERA;
	FLOAT fMIP = 0.0f;

	D3DVIEWPORT9 vNEW = {
		0, 0,
		TMINIMAPTEX_SIZE,
		TMINIMAPTEX_SIZE,
		0.0f,
		1.0f};
	D3DVIEWPORT9 vOLD;

	pDevice->m_pDevice->GetDepthStencilSurface(&pZBUF);
	pDevice->m_pDevice->GetRenderTarget( 0, &pBACK);
	pDevice->m_pDevice->GetViewport(&vOLD);

	pDevice->m_pDevice->SetDepthStencilSurface(NULL);
	pDevice->m_pDevice->SetViewport(&vNEW);

	vCAMERA.InitOrthoCamera(
		pDevice->m_pDevice,
		-1.0f,
		1.0f,
		1.0f,
		1.0f);

	vCAMERA.SetPosition(
		D3DXVECTOR3( 0.0f, 0.5f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
		FALSE);
	vCAMERA.Activate(TRUE);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x000000E0);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);

	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

	pDevice->m_pDevice->SetStreamSource( 0, CTMapViewerGame::m_pTRECTVB, 0, sizeof(PVERTEX));
	pDevice->m_pDevice->SetFVF(T3DFVF_PVERTEX);

	pTARGET->GetSurfaceLevel( 0, &pTMINIMAP);
	pDevice->m_pDevice->SetRenderTarget( 0, pTMINIMAP);

	pDevice->m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(
		0, 0, 0, 0),
		1.0f, 0);

	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &fMIP));
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	if( m_dwBspID == NODEID_NULL || !ResetTMINIMAP( pDevice, WORD(m_dwBspID), pHost->m_vPosition._41, pHost->m_vPosition._43, m_fTSCALE) )
		ResetTMINIMAP( pDevice, m_wMapID, pHost->m_vPosition._41, pHost->m_vPosition._43, m_fTSCALE);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	D3DXMATRIX vSCALE;
	D3DXMATRIX vROT;
	D3DXMATRIX vPOS;

	D3DXMatrixScaling(
		&vSCALE,
		TPOS_SCALE,
		TPOS_SCALE,
		TPOS_SCALE);

	D3DXMatrixRotationY(
		&vROT,
		D3DX_PI + FLOAT(pHost->m_wDIR) * D3DX_PI / 900.0f);

	D3DXMatrixTranslation(
		&vPOS,
		-0.5f,
		0.0f,
		0.5f);
	vPOS *= vSCALE * vROT;

	pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vPOS);
	pDevice->m_pDevice->SetTexture( 0, m_pTPOS);

	pDevice->m_pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0, 2);

	pDevice->m_pDevice->SetRenderTarget( 0, pBACK);
	pDevice->m_pDevice->SetDepthStencilSurface(pZBUF);
	pDevice->m_pDevice->SetViewport(&vOLD);

	pTMINIMAP->Release();
	pBACK->Release();
	pZBUF->Release();

	pCamera->Activate(TRUE);
}

BYTE CTMapViewerMAP::ResetTMINIMAP( CD3DDevice *pDevice,
								   WORD wMapID,
								   FLOAT fCenterX,
								   FLOAT fCenterZ,
								   FLOAT fSCALE)
{
	FLOAT fLength = ((FLOAT) m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);
	BYTE bResult = FALSE;

	D3DXVECTOR2 vCENTER(
		fCenterX / fLength,
		fCenterZ / fLength);

	for( int i=0; i<9; i++)
	{
		int nUnitX = INT(vCENTER.x) - 1 + i % 3;
		int nUnitZ = INT(vCENTER.y) - 1 + i / 3;

		MAPTMINIMAP::iterator finder = m_mapTMINIMAP.find(MAKELONG( MAKEWORD(
			nUnitX,
			nUnitZ), wMapID));

		if( finder != m_mapTMINIMAP.end() && (*finder).second->m_pTEX )
		{
			D3DXVECTOR2 vPOS = D3DXVECTOR2(
				(*finder).second->m_fPosX,
				(*finder).second->m_fPosZ) / fLength - vCENTER;
			FLOAT fRATIO = fSCALE * (*finder).second->m_fSCALE;

			D3DXMATRIX vMAT(
				fSCALE, 0.0f, 0.0f, 0.0f,
				0.0f, fSCALE, 0.0f, 0.0f,
				0.0f, 0.0f, fSCALE, 0.0f,
				fRATIO * vPOS.x, 0.0f,
				fRATIO * vPOS.y, 1.0f);

			pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vMAT);
			pDevice->m_pDevice->SetTexture( 0, (*finder).second->m_pTEX);

			pDevice->m_pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				0, 2);
			bResult = TRUE;
		}
	}

	return bResult;
}

void CTMapViewerMAP::Render( CD3DDevice *pDevice)
{
	pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, m_dwTMapCOLOR);
	m_pMAP->Render(pDevice);
	pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);

	for( int i=0; i<9; i++)
		if(m_pMAP->m_pUNIT[i])
		{
			MAPSNDIDX::iterator itSND;

			for( itSND = m_pMAP->m_mapMAPSND[m_pMAP->m_bUNIT[i]].begin(); itSND != m_pMAP->m_mapMAPSND[m_pMAP->m_bUNIT[i]].end(); itSND++)
				if( (*itSND).second && (*itSND).second->m_pSND && !(*itSND).second->m_pSND->IsPlay((*itSND).second->m_nIndex) )
					(*itSND).second->m_pSND->Play((*itSND).second->m_nIndex);
		}
}

void CTMapViewerMAP::ResetTMAP( CD3DDevice *pDevice,
							   CTMapViewerCAM *pCamera,
							   DWORD dwTick)
{
	if(IsEXTVisible())
	{
		D3DXVECTOR3 vDIR = pCamera->m_vTarget - pCamera->m_vPosition;
		D3DXVECTOR3 vPOS = pCamera->m_vPosition;
		vDIR.y = 0.0f;

		vDIR *= (pCamera->m_fZoomMAX + TCAMFIX_DIST) / D3DXVec3Length(&vDIR);
		pCamera->m_vPosition = pCamera->m_vTarget - vDIR;

		m_pMAP->ResetVB(
			pDevice,
			pCamera,
			TRUE,
			FALSE);
		pCamera->m_vPosition = vPOS;
	}

	LoadMAPOBJ( pDevice, &pCamera->m_vTarget);
	CheckMAPOBJ(
		pDevice,
		&pCamera->m_vTarget);
	ResetVisibleOBJ( pDevice, pCamera, dwTick);
}

void CTMapViewerMAP::ResetVisibleOBJ( CD3DDevice *pDevice,
									 CTMapViewerCAM *pCamera,
									 DWORD dwTick)
{
	MAPTPATHFLAG::iterator itTPATH;

	m_vDRAWOBJ.clear();
	m_vDRAWBSP.clear();
	m_vTPATH.clear();
	m_vWATER.clear();

	for( WORD i=0; i<TDRAWCELL_COUNT; i++)
		if(m_vLOCK[i])
		{
			for( WORD j=0; j<m_vTOBJ[i].m_wCount; j++)
			{
				ResetVisibleOBJ(
					pDevice,
					m_vTOBJ[i].m_vTOBJ[j],
					pCamera,
					dwTick);
			}
		}

	for( i=0; i<9; i++)
		if(m_pMAP->m_pUNIT[i])
		{
			for( WORD j=0; j<m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_wCount; j++)
				if(m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ[j])
				{
					m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ[j]->ResetVisible(
						pCamera,
						dwTick);
				}
		}

	for( i=0; i<9; i++)
		if(m_pMAP->m_pUNIT[i])
		{
			for( WORD j=0; j<m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_wCount; j++)
			{
				ResetVisibleOBJ(
					pDevice,
					m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_vTOBJ[j],
					pCamera,
					dwTick);
			}
		}
}

void CTMapViewerMAP::ResetVisibleOBJ( CD3DDevice *pDevice,
									 CTMapViewerObjBase *pTOBJ,
									 CTMapViewerCAM *pCamera,
									 DWORD dwTick)
{
	if(!pTOBJ)
		return;

	pTOBJ->ResetVisible(
		pCamera,
		dwTick);

	if( pTOBJ->m_bType != OT_WATER )
	{
		if( pTOBJ->m_bType == OT_BSP )
		{
			MAPTBSP::iterator finder = m_mapTBSP.find(pTOBJ->m_dwID);

			if( finder != m_mapTBSP.end() )
				m_vDRAWBSP.push_back((*finder).second);
		}

		if( pTOBJ->m_fCamDIST - pTOBJ->m_fRadiusH < CTMapViewerObjBase::m_fCamDist )
			m_vDRAWOBJ.push_back(pTOBJ);
		else
			pTOBJ->CalcOBJSFX(pDevice);
	}
	else
		m_vWATER.push_back(pTOBJ);
}

BYTE CTMapViewerMAP::CheckCollision( CTMapViewerObjBase *pOBJ, LPD3DXVECTOR3 pPREV)
{
	BYTE bResult = TCR_NONE;

	D3DXVECTOR3 vPOS(
		pOBJ->m_vPosition._41,
		pOBJ->m_vPosition._42,
		pOBJ->m_vPosition._43);

	D3DXPLANE vPLANE = GetBoundPlane(
		pPREV,
		&vPOS);

	D3DXVECTOR3 vNormal(
		vPLANE.a,
		vPLANE.b,
		vPLANE.c);
	vNormal /= D3DXVec3Length(&vNormal);

	if( pOBJ->m_bLand && !pOBJ->m_bSwim && D3DXVec3Dot( &D3DXVECTOR3( 0.0f, 1.0f, 0.0f), &vNormal) < TMapViewer_ZERO )
	{
		FLOAT fDist = D3DXPlaneDotCoord( &vPLANE, &vPOS);

		if( fDist < TMapViewer_ZERO )
		{
			D3DXVECTOR3 vMOVE = fDist * vNormal;

			vPOS.x -= vMOVE.x;
			vPOS.z -= vMOVE.z;

			vPLANE = GetBoundPlane(
				pPREV,
				&vPOS);

			D3DXVECTOR3 vNormalN(
				vPLANE.a,
				vPLANE.b,
				vPLANE.c);
			vNormalN /= D3DXVec3Length(&vNormalN);

			if( D3DXVec3Dot( &D3DXVECTOR3( 0.0f, 1.0f, 0.0f), &vNormalN) < TMapViewer_ZERO )
			{
				D3DXVECTOR3 vGRAVITY = 0.02f * vNormal;
				D3DXVECTOR3 vGPOS = vPOS + vGRAVITY;

				vPLANE = GetBoundPlane(
					pPREV,
					&vGPOS);

				D3DXVECTOR3 vNormalG(
					vPLANE.a,
					vPLANE.b,
					vPLANE.c);
				vNormalG /= D3DXVec3Length(&vNormalG);

				if( D3DXVec3Dot( &D3DXVECTOR3( 0.0f, 1.0f, 0.0f), &vNormalG) < TMapViewer_ZERO )
				{
					vMOVE -= vGRAVITY;
					vPOS = vGPOS;
				}
			}

			if( D3DXVec3Dot( &D3DXVECTOR3( 0.0f, 1.0f, 0.0f), &vNormalN) >= TMapViewer_ZERO ||
				D3DXVec3Dot( &vNormal, &vNormalN) > 0.99f ||
				GetHeight( &m_vDRAWOBJ, pOBJ, &vPOS, 0.0f, FALSE) <= pPREV->y )
			{
				pOBJ->m_vWorld._41 -= vMOVE.x;
				pOBJ->m_vWorld._43 -= vMOVE.z;

				bResult = TCR_CORRECT;
			}
			else
				return TCR_BACK;
		}
	}

	for( int i=0; i<INT(m_vDRAWOBJ.size()); i++)
	{
		CTMapViewerObject *pMAPOBJ = (CTMapViewerObject *) m_vDRAWOBJ[i];

		if( pMAPOBJ && D3DXVec2Length(&D3DXVECTOR2(
			pMAPOBJ->m_vPosition._41 - pOBJ->m_vPosition._41,
			pMAPOBJ->m_vPosition._43 - pOBJ->m_vPosition._43)) < pMAPOBJ->m_fRadius + pOBJ->m_fRadius &&
			pOBJ->CheckCollision(pMAPOBJ) )
			switch(bResult)
			{
			case TCR_NONE		: bResult = TCR_CORRECT; break;
			case TCR_CORRECT	: return TCR_BACK;
			}
	}

	return bResult;
}

void CTMapViewerMAP::CalcMAPOBJ( CD3DDevice *pDevice,
							    CTMapViewerCAM *pCamera,
								DWORD dwTick)
{
	for( WORD i=0; i<TDRAWCELL_COUNT; i++)
		if(m_vLOCK[i])
			for( WORD j=0; j<m_vTOBJ[i].m_wCount; j++)
				m_vTOBJ[i].m_vTOBJ[j]->CalcTick( pDevice->m_pDevice, dwTick);

	for( i=0; i<9; i++)
		if(m_pMAP->m_pUNIT[i])
			for( WORD j=0; j<m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_wCount; j++)
				m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ[j]->CalcTick( pDevice->m_pDevice, dwTick);

	for( i=0; i<9; i++)
		if(m_pMAP->m_pUNIT[i])
			for( WORD j=0; j<m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_wCount; j++)
				m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_vTOBJ[j]->CalcTick( pDevice->m_pDevice, dwTick);
}

void CTMapViewerMAP::RenderWATER( CD3DDevice *pDevice, CTMapViewerCAM *pCamera)
{
	for( int i=0; i<INT(m_vWATER.size()); i++)
	{
		CTMapViewerObject *pOBJ = (CTMapViewerObject *) m_vWATER[i];

		if(pOBJ)
			pOBJ->Render( pDevice, pCamera);
	}
}

void CTMapViewerMAP::RenderBSP( CTMapViewerCAM *pCamera)
{
	for( int i=0; i<INT(m_vDRAWBSP.size()); i++)
	{
		DWORD dwNodeID = m_vDRAWBSP[i]->GetNodeID(pCamera->m_vTarget);

		if( dwNodeID != NODEID_NULL && m_dwBspID != m_vDRAWBSP[i]->m_dwBspID )
		{
			MAPD3DXVECTOR3::iterator itTNODE;
			BYTE bENTER = FALSE;

			for( itTNODE = m_vDRAWBSP[i]->m_pINFO->m_mapTNODE.begin(); itTNODE != m_vDRAWBSP[i]->m_pINFO->m_mapTNODE.end(); itTNODE++)
				if( m_vDRAWBSP[i]->m_mapPVS.find((*itTNODE).first) != m_vDRAWBSP[i]->m_mapPVS.end() )
				{
					bENTER = TRUE;
					break;
				}

			if(bENTER)
				m_dwBspID = m_vDRAWBSP[i]->m_dwBspID;
		}
		else if( dwNodeID == NODEID_NULL && m_dwBspID == m_vDRAWBSP[i]->m_dwBspID )
			m_dwBspID = NODEID_NULL;

		if( m_dwBspID == NODEID_NULL || m_dwBspID == m_vDRAWBSP[i]->m_dwBspID )
		{
			if( dwNodeID == NODEID_NULL )
			{
				MAPD3DXVECTOR3::iterator itTNODE;
				FLOAT fDIST = 0.0f;

				for( itTNODE = m_vDRAWBSP[i]->m_pINFO->m_mapTNODE.begin(); itTNODE != m_vDRAWBSP[i]->m_pINFO->m_mapTNODE.end(); itTNODE++)
				{
					FLOAT fLocal = D3DXVec3Length(&(pCamera->m_vTarget - (*(*itTNODE).second)));

					if( dwNodeID == NODEID_NULL || fLocal < fDIST )
					{
						dwNodeID = (*itTNODE).first;
						fDIST = fLocal;
					}
				}
			}

			m_vDRAWBSP[i]->Render(dwNodeID);
		}
	}
}

BYTE CTMapViewerMAP::CanMove( VTOBJBASE& vOBJ,
							 CTMapViewerObjBase *pOBJ,
							 LPD3DXVECTOR3 pSTART,
							 LPD3DXVECTOR3 pTARGET)
{
	FLOAT fRange = D3DXVec2Length(&D3DXVECTOR2(
		pTARGET->x - pSTART->x,
		pTARGET->z - pSTART->z));

	D3DXVECTOR3 vTARGET = (*pTARGET);
	D3DXVECTOR3 vSTART = (*pSTART);

	vTARGET.y += pOBJ->m_fSizeY / 2.0f;
	vSTART.y += pOBJ->m_fSizeY / 2.0f;

	if( fRange > 0.01f )
		for( int i=0; i<INT(vOBJ.size()); i++)
		{
			FLOAT fDIST = D3DXVec3Length(&(D3DXVECTOR3(
				vOBJ[i]->m_vPosition._41,
				vOBJ[i]->m_vPosition._42,
				vOBJ[i]->m_vPosition._43) - (*pSTART))) - vOBJ[i]->m_fRadiusH - pOBJ->m_fRadius;

			if( fDIST < fRange && vOBJ[i]->HitTest( &vSTART, &vTARGET, pOBJ->m_fRadius, pOBJ->m_fSizeY) )
				return FALSE;
		}

	return TRUE;
}

void CTMapViewerMAP::ClearTFLAG( BYTE bINDEX)
{
	MAPTPATHFLAG::iterator itTPATH = CTMapViewerFlag::m_mapTPATHFLAG.begin();
	BYTE bUnitX = m_pMAP->m_nUnitX + bINDEX % 3;
	BYTE bUnitZ = m_pMAP->m_nUnitZ + bINDEX / 3;

	WORD wUnitID = MAKEWORD(
		bUnitX,
		bUnitZ);

	while(itTPATH != CTMapViewerFlag::m_mapTPATHFLAG.end())
	{
		MAPTPATHFLAG::iterator itNEXT = itTPATH;
		itNEXT++;

		if( HIWORD((*itTPATH).second->m_dwID) == wUnitID )
			delete (*itTPATH).second;

		itTPATH = itNEXT;
	}
}

void CTMapViewerMAP::ClearTBSP( BYTE bINDEX)
{
	for( WORD i=0; i<m_vTBSP[bINDEX].m_wCount; i++)
	{
		MAPTBSP::iterator finder = m_mapTBSP.find(m_vTBSP[bINDEX].m_vTBSP[i]->m_dwBspID);

		if( finder != m_mapTBSP.end() )
			m_mapTBSP.erase(finder);

		m_vTBSP[bINDEX].m_vTBSP[i]->Release();
	}

	m_vTBSP[bINDEX].m_wCount = 0;
}

void CTMapViewerMAP::ClearTSILHOUETTE( BYTE bINDEX)
{
	for( WORD i=0; i<m_vTSILHOUETTE[bINDEX].m_wCount; i++)
		m_vTSILHOUETTE[bINDEX].m_vTOBJ[i]->Release();

	m_vTSILHOUETTE[bINDEX].m_wCount = 0;
}

void CTMapViewerMAP::ClearBIGOBJ( BYTE bINDEX)
{
	for( WORD i=0; i<m_vBIGOBJ[bINDEX].m_wCount; i++)
		m_vBIGOBJ[bINDEX].m_vTOBJ[i]->Release();

	m_vBIGOBJ[bINDEX].m_wCount = 0;
}

void CTMapViewerMAP::LoadTFLAG( BYTE bINDEX)
{
	BYTE bUnitX = m_pMAP->m_nUnitX + bINDEX % 3;
	BYTE bUnitZ = m_pMAP->m_nUnitZ + bINDEX / 3;

	TRY
	{
		CString strFile;
		strFile.Format( IDS_FMT_PATHFILE, MAKELONG( MAKEWORD( bUnitX, bUnitZ), m_wMapID));

		CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
		CArchive ar( &file, CArchive::load);

		DWORD dwCount;
		ar >> dwCount;

		for( DWORD i=0; i<dwCount; i++)
		{
			CTMapViewerFlag *pTFLAG = new CTMapViewerFlag();
			DWORD dwTFLAG;

			ar	>> pTFLAG->m_dwID
				>> pTFLAG->m_vPOS.x
				>> pTFLAG->m_vPOS.y
				>> pTFLAG->m_vPOS.z
				>> dwTFLAG;

			for( DWORD j=0; j<dwTFLAG; j++)
			{
				DWORD dwNODE;
				ar >> dwNODE;

				pTFLAG->m_vPATH.push_back(dwNODE);
			}

			CTMapViewerFlag::m_mapTPATHFLAG.insert( MAPTPATHFLAG::value_type( pTFLAG->m_dwID, pTFLAG));
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL
}

BYTE CTMapViewerMAP::HitTest( FLOAT *pDIST,
							 LPD3DXVECTOR3 pSTART,
							 LPD3DXVECTOR3 pEND,
							 FLOAT fLineH,
							 FLOAT fLineV)
{
	FLOAT fLength = D3DXVec3Length(&((*pEND) - (*pSTART)));

	if(!m_pMAP)
		return FALSE;
	BYTE bResult = HitTestHeight( pDIST, pSTART, pEND, fLineH, fLineV);

	for( int i=0; i<INT(m_vDRAWOBJ.size()); i++)
		if(m_vDRAWOBJ[i]->m_bCamCollision)
		{
			FLOAT fDist[2] = {
				D3DXVec3Length(&D3DXVECTOR3(
				pEND->x - m_vDRAWOBJ[i]->m_vPosition._41,
				pEND->y - m_vDRAWOBJ[i]->m_vPosition._42,
				pEND->z - m_vDRAWOBJ[i]->m_vPosition._43)),
				D3DXVec3Length(&D3DXVECTOR3(
				pSTART->x - m_vDRAWOBJ[i]->m_vPosition._41,
				pSTART->y - m_vDRAWOBJ[i]->m_vPosition._42,
				pSTART->z - m_vDRAWOBJ[i]->m_vPosition._43))};
			FLOAT fLocal = 0.0f;

			if((fLength + m_vDRAWOBJ[i]->m_fRadiusH > fDist[0] ||
				fLength + m_vDRAWOBJ[i]->m_fRadiusH > fDist[1]) &&
				m_vDRAWOBJ[i]->HitPoly(
				&fLocal,
				pSTART,
				pEND,
				fLineH,
				fLineV) && (!bResult || fLocal < (*pDIST) ))
			{
				(*pDIST) = fLocal;
				bResult = TRUE;
			}
		}

	return bResult;
}

BYTE CTMapViewerMAP::HitTestHeight( FLOAT *pDIST,
								   LPD3DXVECTOR3 pSTART,
								   LPD3DXVECTOR3 pEND,
								   FLOAT fLineH,
								   FLOAT fLineV)
{
	FLOAT fStartX = FLOAT(INT(min( pSTART->x, pEND->x) / m_pMAP->m_nTileLength));
	FLOAT fStartZ = FLOAT(INT(min( pSTART->z, pEND->z) / m_pMAP->m_nTileLength));
	FLOAT fEndX = FLOAT(INT(max( pSTART->x, pEND->x) / m_pMAP->m_nTileLength) + 1);
	FLOAT fEndZ = FLOAT(INT(max( pSTART->z, pEND->z) / m_pMAP->m_nTileLength) + 1);

	BYTE bCountX = BYTE(fEndX - fStartX);
	BYTE bCountZ = BYTE(fEndZ - fStartZ);
	BYTE bResult = FALSE;

	D3DXVECTOR3 vDIR = (*pEND) - (*pSTART);
	FLOAT fLength = D3DXVec3Length(&vDIR);

	if( fLength == 0.0f )
		return bResult;

	fStartX *= m_pMAP->m_nTileLength;
	fStartZ *= m_pMAP->m_nTileLength;
	vDIR /= fLength;

	D3DXVECTOR3 vRECT[4] = {
		D3DXVECTOR3( -fLineV / 2.0f, 0.0f,  fLineH / 2.0f),
		D3DXVECTOR3( -fLineV / 2.0f, 0.0f, -fLineH / 2.0f),
		D3DXVECTOR3(  fLineV / 2.0f, 0.0f, -fLineH / 2.0f),
		D3DXVECTOR3(  fLineV / 2.0f, 0.0f,  fLineH / 2.0f)};

	D3DXPLANE vBOUND[4];
	D3DXMATRIX vBASIS;

	CTMath::GetBasisMatrix(
		&vBASIS,
		pSTART,
		&vDIR);

	if( fLineV != 0.0f && fLineH != 0.0f )
		for( BYTE i=0; i<4; i++)
		{
			D3DXPlaneFromPoints(
				&vBOUND[i],
				&vRECT[i],
				&D3DXVECTOR3(
				vRECT[i].x,
				vRECT[i].y + 1.0f,
				vRECT[i].z),
				&vRECT[(i + 1) % 4]);
		}

	for( BYTE i=0; i<bCountZ; i++)
	{
		FLOAT fPosZ = fStartZ + FLOAT(i * m_pMAP->m_nTileLength);

		for( BYTE j=0; j<bCountX; j++)
		{
			static int nIndex[2][3] = {
				{ 0, 1, 2},
				{ 2, 1, 3}};
			FLOAT fPosX = fStartX + FLOAT(j * m_pMAP->m_nTileLength);

			D3DXVECTOR3 vPOINT[4] = {
				D3DXVECTOR3( fPosX, 0.0f, fPosZ),
				D3DXVECTOR3( fPosX, 0.0f, fPosZ + m_pMAP->m_nTileLength),
				D3DXVECTOR3( fPosX + m_pMAP->m_nTileLength, 0.0f, fPosZ),
				D3DXVECTOR3( fPosX + m_pMAP->m_nTileLength, 0.0f, fPosZ + m_pMAP->m_nTileLength)};

			for( BYTE k=0; k<4; k++)
			{
				vPOINT[k].y = m_pMAP->GetHeight(
					vPOINT[k].x,
					vPOINT[k].y,
					vPOINT[k].z);

				CTMath::Transform(
					&vBASIS,
					&vPOINT[k]);
			}

			for( k=0; k<2; k++)
			{
				D3DXVECTOR3 vPOLY[3] = {
					vPOINT[nIndex[k][0]],
					vPOINT[nIndex[k][1]],
					vPOINT[nIndex[k][2]]};
				FLOAT fLocal = 0.0f;

				if( CTMath::HitPoly(
					vPOLY,
					fLineV != 0.0f && fLineH != 0.0f ? vRECT : NULL,
					fLineV != 0.0f && fLineH != 0.0f ? vBOUND : NULL,
					&fLocal) && fLocal < fLength && (
					!bResult || fLocal < (*pDIST) ))
				{
					(*pDIST) = fLocal;
					bResult = TRUE;
				}
			}
		}
	}

	return bResult;
}

DWORD CTMapViewerMAP::GetTStepSND( FLOAT fPosX,
								  FLOAT fPosY,
								  FLOAT fPosZ)
{
	int nTileX = INT(fPosX / m_pMAP->m_nTileLength);
	int nTileZ = INT(fPosZ / m_pMAP->m_nTileLength);

	BYTE bTileID = m_pMAP->FindDetailID(
		nTileX,
		nTileZ);

	if( bTileID != TILE_NULL )
	{
		MAPDWORD::iterator finder = CTMapViewerObjBase::m_mapTSTEPSND.find(MAKEWORD( bTileID, BYTE(m_wMapID)));

		if( finder != CTMapViewerObjBase::m_mapTSTEPSND.end() )
			return (*finder).second;
	}

	return TDEFSTEP_SND;
}

BYTE CTMapViewerMAP::IsEXTVisible()
{
	BYTE bRESULT = FALSE;

	if( m_dwBspID != NODEID_NULL )
	{
		MAPTBSP::iterator finder = m_mapTBSP.find(m_dwBspID);

		if( finder != m_mapTBSP.end() )
		{
			MAPD3DXVECTOR3::iterator itTNODE;

			for( itTNODE = (*finder).second->m_pINFO->m_mapTNODE.begin(); itTNODE != (*finder).second->m_pINFO->m_mapTNODE.end(); itTNODE++)
				if( (*finder).second->m_mapPVS.find((*itTNODE).first) != (*finder).second->m_mapPVS.end() )
					return TRUE;
		}
	}
	else
		bRESULT = TRUE;

	return bRESULT;
}
