#include "StdAfx.h"
#include "TClient.h"
#include "TClientGame.h"
#include "TClientWnd.h"
#include "TSiegeManager.h"
#include "Resource.h"

LPTEXTURESET CTClientMAP::m_pTPURSUITDIR = NULL;
LPTEXTURESET CTClientMAP::m_pTDEADDIR = NULL;
LPTEXTURESET CTClientMAP::m_pTDEAD = NULL;
LPTEXTURESET CTClientMAP::m_pTPOS = NULL;
LPTEXTURESET CTClientMAP::m_pTPARTY = NULL;
LPTEXTURESET CTClientMAP::m_pTPARTYDIR = NULL;

LPTEXTURESET CTClientMAP::m_pTQUESTRECV_O = NULL;
LPTEXTURESET CTClientMAP::m_pTQUESTCOMP_O = NULL;
LPTEXTURESET CTClientMAP::m_pTQUESTRECV_X = NULL;
LPTEXTURESET CTClientMAP::m_pTQUESTCOMP_X = NULL;

LPTEXTURESET CTClientMAP::m_pTCOMMANDER = NULL;
LPTEXTURESET CTClientMAP::m_pTSQUAD = NULL;
LPTEXTURESET CTClientMAP::m_pTUNIT = NULL;
LPTEXTURESET CTClientMAP::m_pTMON = NULL;
LPTEXTURESET CTClientMAP::m_pTPC = NULL;

LPTEXTURESET CTClientMAP::m_pTCMDATTACK = NULL;
LPTEXTURESET CTClientMAP::m_pTCMDMOVE = NULL;
LPTEXTURESET CTClientMAP::m_pTCMDPROTECT = NULL;
LPTEXTURESET CTClientMAP::m_pTCMDALARM = NULL;

LPTEXTURESET CTClientMAP::m_pTSiegeGodBall = NULL;
LPTEXTURESET CTClientMAP::m_pTGodTower[3] = {NULL};

FLOAT CTClientMAP::m_fMoveBound = 0.58f;
FLOAT CTClientMAP::m_fGravity = 20.0f;


CTClientMAP::CTClientMAP()
{
	m_dwTMapCOLOR = 0xFFFFFFFF;
	m_dwBspID = NODEID_NULL;
	m_wMapID = 0;
	m_bSHADOW = 0x60;

	m_fTSCALE = TMINIMAP_INIT_SCALE / powf( TMINIMAP_SCALE_FACTOR, TMINIMAP_SCALE_COUNT);
	m_nTCellX = -(TDRAWCELL_WIDTH / 2 + 1);
	m_nTCellZ = -(TDRAWCELL_WIDTH / 2 + 1);

	m_pTLOADJOINT = NULL;
	m_pMAP = NULL;

	m_nUnitCellX = -2;
	m_nUnitCellZ = -2;

	m_bHouseMesh = FALSE;
	m_dwHouseID = 0;

	for( WORD i=0; i<9; i++)
	{
		for( WORD j=0; j<TOBJCELL_COUNT; j++)
		{
			m_mapTMAPOBJ[i][j].clear();
			m_vTMAPNPC[i][j].clear();
			m_vTMAPSFX[i][j].clear();
			m_vTMAPSND[i][j].clear();
		}

		m_vTSILHOUETTE[i].m_vTOBJ.clear();
		m_vBIGOBJ[i].m_vTOBJ.clear();

		m_mapTHEIGHTINFO[i].clear();
		m_vDRAWOBJ[i].clear();
		m_vTBSP[i].clear();

		m_vTSILHOUETTE[i].m_wCount = 0;
		m_vBIGOBJ[i].m_wCount = 0;
		m_vUNIT[i] = FALSE;
	}

	for( i=0; i<TDRAWCELL_COUNT; i++)
	{
		m_vTOBJ[i].m_vTOBJ.clear();
		m_vTNPC[i].m_vTOBJ.clear();
		m_vTSFX[i].m_vTSFX.clear();
		m_vTSND[i].m_vTSND.clear();

		m_vTOBJ[i].m_wCount = 0;
		m_vTNPC[i].m_wCount = 0;
		m_vTSND[i].m_wCount = 0;

		m_vTCELL[i] = 0xFFFF;
		m_vLOCK[i] = FALSE;
	}

	m_vNEWQUESTOBJ.clear();
	m_vDRAWBSP.clear();
	m_mapTPortal.clear();
	m_mapTNpc.clear();
	m_mapTGate.clear();
	m_vTPATH.clear();
	m_vTDRAW.clear();

	m_mapTCOLLOBJ.clear();
	m_mapTWATER.clear();
	m_vTDRAWGATE.clear();
	m_vTCOLLNPC.clear();
}

CTClientMAP::~CTClientMAP()
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

		while(!m_vTBSP[i].empty())
		{
			delete m_vTBSP[i].back();
			m_vTBSP[i].pop_back();
		}

		m_vUNIT[i] = FALSE;
	}

	for( i=0; i<TDRAWCELL_COUNT; i++)
	{
		while(!m_vTOBJ[i].m_vTOBJ.empty())
		{
			delete m_vTOBJ[i].m_vTOBJ.back();
			m_vTOBJ[i].m_vTOBJ.pop_back();
		}

		while(!m_vTNPC[i].m_vTOBJ.empty())
		{
			delete m_vTNPC[i].m_vTOBJ.back();
			m_vTNPC[i].m_vTOBJ.pop_back();
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

void CTClientMAP::ReleaseMAP()
{
	CTClientApp *pTAPP = (CTClientApp *) AfxGetApp();
	MAPTGATE::iterator itTGATE;

	if( pTAPP->m_hLoadThread && pTAPP->m_hLoadEvent )
	{
		pTAPP->ExitLoadThread(TRUE);
		pTAPP->CreateLoadThread();
	}

	for( WORD i=0; i<9; i++)
	{
		ClearTHEIGHTINFO(BYTE(i));
		ClearTSILHOUETTE(BYTE(i));
		ClearBIGOBJ(BYTE(i));
		ClearTBSP(BYTE(i));

		for( WORD j=0; j<TOBJCELL_COUNT; j++)
		{
			ClearTNPC(&m_vTMAPNPC[i][j]);
			m_mapTMAPOBJ[i][j].clear();
			m_vTMAPSFX[i][j].clear();
			m_vTMAPSND[i][j].clear();
		}

		m_vDRAWOBJ[i].clear();
	}

	for( i=0; i<TDRAWCELL_COUNT; i++)
	{
		ClearMAPOBJ(
			&m_vTOBJ[i],
			&m_vTNPC[i],
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
	m_pTLOADJOINT = NULL;

	for( itTGATE = m_mapTGate.begin(); itTGATE != m_mapTGate.end(); itTGATE++)
		delete (*itTGATE).second;

	m_vNEWQUESTOBJ.clear();
	m_vDRAWBSP.clear();
	m_vTPATH.clear();
	m_vTDRAW.clear();

	m_mapTCOLLOBJ.clear();
	m_mapTWATER.clear();
	m_vTDRAWGATE.clear();
	m_vTCOLLNPC.clear();

	m_mapTPortal.clear();
	m_mapTGate.clear();
	m_mapTNpc.clear();
	m_mapTBsp.clear();

	m_nTCellX = -(TDRAWCELL_WIDTH / 2 + 1);
	m_nTCellZ = -(TDRAWCELL_WIDTH / 2 + 1);

	m_nUnitCellX = -2;
	m_nUnitCellZ = -2;

	m_bHouseMesh = FALSE;
	m_dwHouseID = 0;

	m_dwBspID = NODEID_NULL;
	m_wMapID = 0;

	CTClientObjBase::ClearGARBAGE();
	CTClientFlag::ReleaseTPATH();
}

FLOAT CTClientMAP::GetAttrFLOAT( LPMAPOBJ pTOBJ,
								 DWORD dwID)
{
	MAPATTRIBUTE::iterator finder = pTOBJ->m_mapATTR.find(dwID);

	if( finder == pTOBJ->m_mapATTR.end() )
		return 0;

	return *((FLOAT *) (*finder).second->m_pValue);
}

int CTClientMAP::GetAttrINT( LPMAPOBJ pTOBJ,
							 DWORD dwID)
{
	MAPATTRIBUTE::iterator finder = pTOBJ->m_mapATTR.find(dwID);

	if( finder == pTOBJ->m_mapATTR.end() )
		return 0;

	return *((int *) (*finder).second->m_pValue);
}

void CTClientMAP::GetLineCollisionOBJ( LPVTOBJBASE pTCOLLISION,
									   LPD3DXVECTOR3 pSTART,
									   LPD3DXVECTOR3 pEND,
									   FLOAT fWIDTH)
{
	fWIDTH /= 2.0f;
	fWIDTH += FLOAT(TGRID_SIZE);

	FLOAT fMaxX = max( pSTART->x, pEND->x) + fWIDTH;
	FLOAT fMinX = min( pSTART->x, pEND->x) - fWIDTH;

	FLOAT fMaxZ = max( pSTART->z, pEND->z) + fWIDTH;
	FLOAT fMinZ = min( pSTART->z, pEND->z) - fWIDTH;

	D3DXVECTOR2 vO( pSTART->x, pSTART->z);
	D3DXVECTOR2 vA( pEND->x, pEND->z);

	if( !m_pMAP || vA == vO )
		return;

	int nSIZE = m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength;
	MAPDWORD mapTINDEX;

	mapTINDEX.clear();
	vA -= vO;
	fWIDTH *= fWIDTH;

	for( FLOAT fPosX = fMinX; fPosX < fMaxX; fPosX += FLOAT(TGRID_SIZE))
		for( FLOAT fPosZ = fMinZ; fPosZ < fMaxZ; fPosZ += FLOAT(TGRID_SIZE))
		{
			D3DXVECTOR2 vP(
				FLOAT(INT(fPosX)) + FLOAT(TGRID_SIZE) / 2.0f - vO.x,
				FLOAT(INT(fPosZ)) + FLOAT(TGRID_SIZE) / 2.0f - vO.y);

			vP -= D3DXVec2Dot( &vA, &vP) / D3DXVec2Dot( &vA, &vA) * vA;
			if( D3DXVec2Dot( &vP, &vP) < fWIDTH )
			{
				int nUnitX = INT(fPosX) / nSIZE - m_pMAP->m_nUnitX;
				int nUnitZ = INT(fPosZ) / nSIZE - m_pMAP->m_nUnitZ;

				if( nUnitX >= 0 && nUnitX < 3 &&
					nUnitZ >= 0 && nUnitZ < 3 )
				{
					BYTE bINDEX = BYTE(nUnitZ * 3 + nUnitX);

					if( m_pMAP->m_bUNIT[bINDEX] >= 0 &&
						m_pMAP->m_bUNIT[bINDEX] < 9 )
					{
						int nPosX = INT(fPosX) % nSIZE;
						int nPosZ = INT(fPosZ) % nSIZE;

						nPosX /= TGRID_SIZE;
						nPosZ /= TGRID_SIZE;

						GetCollisionOBJ(
							pTCOLLISION,
							&m_mapTCOLLOBJ,
							&mapTINDEX,
							bINDEX,
							MAKELONG(
							WORD(nPosX),
							WORD(nPosZ)));
					}
				}
			}
		}
	mapTINDEX.clear();

	fWIDTH = D3DXVec2Length(&vA) + sqrt(fWIDTH);
	GetCollisionGATE(
		pTCOLLISION,
		pSTART->x,
		pSTART->z,
		fWIDTH);

	GetCollisionNPC(
		pTCOLLISION,
		pSTART->x,
		pSTART->z,
		fWIDTH);
}

void CTClientMAP::GetCollisionOBJ( LPVTOBJBASE pTCOLLISION,
								   FLOAT fPosX,
								   FLOAT fPosZ,
								   FLOAT fRadius)
{
	MAPDWORD mapTINDEX;
	mapTINDEX.clear();

	if(!m_pMAP)
		return;

	for( BYTE i=0; i<9; i++)
		if( m_pMAP->m_bUNIT[i] != 0xFF )
		{
			GetCollisionOBJ(
				pTCOLLISION,
				&m_mapTCOLLOBJ,
				&mapTINDEX, i,
				fPosX,
				fPosZ,
				fRadius,
				TGRID_SIZE);
		}
	mapTINDEX.clear();

	GetCollisionGATE(
		pTCOLLISION,
		fPosX,
		fPosZ,
		fRadius);

	GetCollisionNPC(
		pTCOLLISION,
		fPosX,
		fPosZ,
		fRadius);
}

BOOL CTClientMAP::CheckCollisionOBJ( FLOAT fPosX,
								   FLOAT fPosZ,
								   FLOAT fRadius)
{
	if(!m_pMAP)
		return FALSE;

	for( BYTE i=0; i<9; i++)
		if( m_pMAP->m_bUNIT[i] != 0xFF )
		{
			if( CheckCollisionOBJ(
				i,
				fPosX,
				fPosZ,
				fRadius,
				TGRID_SIZE) )
			{
				return TRUE;
			}
		}

	if( CheckCollisionGATE(
		fPosX,
		fPosZ,
		fRadius) )
		return TRUE;

	if( CheckCollisionNPC(
		fPosX,
		fPosZ,
		fRadius) )
		return TRUE;

	return FALSE;
}

void CTClientMAP::GetCollisionOBJ( LPVTOBJBASE pTCOLLISION,
								   LPMAPOBJECT pTINST,
								   LPMAPDWORD pTINDEX,
								   BYTE bINDEX,
								   FLOAT fPosX,
								   FLOAT fPosZ,
								   FLOAT fRadius,
								   int nGRID)
{
	int nSIZE = m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength;
	int nUnitX = nSIZE * (m_pMAP->m_nUnitX + bINDEX % 3);
	int nUnitZ = nSIZE * (m_pMAP->m_nUnitZ + bINDEX / 3);

	int nStartX = INT(fPosX - fRadius) - nUnitX;
	int nStartZ = INT(fPosZ - fRadius) - nUnitZ;
	int nEndX = INT(fPosX + fRadius) + 1 - nUnitX;
	int nEndZ = INT(fPosZ + fRadius) + 1 - nUnitZ;

	nStartX = max( 0, nStartX / nGRID * nGRID);
	nStartZ = max( 0, nStartZ / nGRID * nGRID);
	nEndX = min( nSIZE, nEndX / nGRID * nGRID + nGRID);
	nEndZ = min( nSIZE, nEndZ / nGRID * nGRID + nGRID);

	for( int i = nStartX; i < nEndX; i += nGRID)
		for( int j = nStartZ; j < nEndZ; j += nGRID)
		{
			GetCollisionOBJ(
				pTCOLLISION,
				pTINST,
				pTINDEX,
				bINDEX,
				MAKELONG(
				WORD(i / nGRID),
				WORD(j / nGRID)));
		}
}

BOOL CTClientMAP::CheckCollisionOBJ( BYTE bINDEX,
								   FLOAT fPosX,
								   FLOAT fPosZ,
								   FLOAT fRadius,
								   int nGRID)
{
	int nSIZE = m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength;
	int nUnitX = nSIZE * (m_pMAP->m_nUnitX + bINDEX % 3);
	int nUnitZ = nSIZE * (m_pMAP->m_nUnitZ + bINDEX / 3);

	int nStartX = INT(fPosX - fRadius) - nUnitX;
	int nStartZ = INT(fPosZ - fRadius) - nUnitZ;
	int nEndX = INT(fPosX + fRadius) + 1 - nUnitX;
	int nEndZ = INT(fPosZ + fRadius) + 1 - nUnitZ;

	nStartX = max( 0, nStartX / nGRID * nGRID);
	nStartZ = max( 0, nStartZ / nGRID * nGRID);
	nEndX = min( nSIZE, nEndX / nGRID * nGRID + nGRID);
	nEndZ = min( nSIZE, nEndZ / nGRID * nGRID + nGRID);

	for( int i = nStartX; i < nEndX; i += nGRID)
		for( int j = nStartZ; j < nEndZ; j += nGRID)
		{
			if( CheckCollisionOBJ(
				bINDEX,
				MAKELONG(
				WORD(i / nGRID),
				WORD(j / nGRID))) )
				return TRUE;
		}

	return FALSE;
}

void CTClientMAP::GetCollisionOBJ( LPVTOBJBASE pTCOLLISION,
								   LPMAPOBJECT pTINST,
								   LPMAPDWORD pTINDEX,
								   BYTE bINDEX,
								   DWORD dwINDEX)
{
	MAPTHEIGHTINFO::iterator itTHEIGHT = m_mapTHEIGHTINFO[m_pMAP->m_bUNIT[bINDEX]].find(dwINDEX);

	if( itTHEIGHT != m_mapTHEIGHTINFO[m_pMAP->m_bUNIT[bINDEX]].end() )
	{
		int nCount = INT((*itTHEIGHT).second->size());

		for( int i=0; i<nCount; i++)
		{
			DWORD dwID = (*itTHEIGHT).second->at(i);

			if( pTINDEX->find(dwID) == pTINDEX->end() )
			{
				MAPOBJECT::iterator itTOBJ = pTINST->find(dwID);

				if( itTOBJ != pTINST->end() )
				{
					pTCOLLISION->push_back((CTClientObjBase *) (*itTOBJ).second);
					pTINDEX->insert( MAPDWORD::value_type( dwID, dwID));
				}
			}
		}
	}
}

BOOL CTClientMAP::CheckCollisionOBJ( BYTE bINDEX,
								   DWORD dwINDEX)
{
	MAPTHEIGHTINFO::iterator itTHEIGHT = m_mapTHEIGHTINFO[m_pMAP->m_bUNIT[bINDEX]].find(dwINDEX);

	if( itTHEIGHT != m_mapTHEIGHTINFO[m_pMAP->m_bUNIT[bINDEX]].end() )
	{
		int nCount = INT((*itTHEIGHT).second->size());

		if( nCount > 0 )
			return TRUE;
	}

	return FALSE;
}

void CTClientMAP::GetCollisionNPC( LPVTOBJBASE pTCOLLISION,
								   FLOAT fPosX,
								   FLOAT fPosZ,
								   FLOAT fRadius)
{
	for( int i=0; i<INT(m_vTCOLLNPC.size()); i++)
	{
		CTClientObjBase *pTNPC = m_vTCOLLNPC[i];
		D3DXVECTOR2 vDIST(
			pTNPC->GetPositionX() - fPosX,
			pTNPC->GetPositionZ() - fPosZ);
		FLOAT fDIST = pTNPC->m_fRadius + fRadius;

		if( D3DXVec2Dot( &vDIST, &vDIST) < fDIST * fDIST )
			pTCOLLISION->push_back(pTNPC);
	}
}

BOOL CTClientMAP::CheckCollisionNPC( 
								FLOAT fPosX,
								FLOAT fPosZ,
								FLOAT fRadius)
{
	for( int i=0; i<INT(m_vTCOLLNPC.size()); i++)
	{
		CTClientObjBase *pTNPC = m_vTCOLLNPC[i];
		D3DXVECTOR2 vDIST(
			pTNPC->GetPositionX() - fPosX,
			pTNPC->GetPositionZ() - fPosZ);
		FLOAT fDIST = pTNPC->m_fRadius + fRadius;

		if( D3DXVec2Dot( &vDIST, &vDIST) < fDIST * fDIST )
			return TRUE;
	}

	return FALSE;
}

void CTClientMAP::GetCollisionGATE( LPVTOBJBASE pTCOLLISION,
								    FLOAT fPosX,
									FLOAT fPosZ,
									FLOAT fRadius)
{
	for( int i=0; i<INT(m_vTDRAWGATE.size()); i++)
	{
		CTClientObjBase *pTGATE = m_vTDRAWGATE[i];
		D3DXVECTOR2 vDIST(
			pTGATE->GetPositionX() - fPosX,
			pTGATE->GetPositionZ() - fPosZ);
		FLOAT fDIST = pTGATE->m_fRadius + fRadius;

		if( D3DXVec2Dot( &vDIST, &vDIST) < fDIST * fDIST )
			pTCOLLISION->push_back(pTGATE);
	}
}

BOOL CTClientMAP::CheckCollisionGATE(
								    FLOAT fPosX,
									FLOAT fPosZ,
									FLOAT fRadius)
{
	for( int i=0; i<INT(m_vTDRAWGATE.size()); i++)
	{
		CTClientObjBase *pTGATE = m_vTDRAWGATE[i];
		D3DXVECTOR2 vDIST(
			pTGATE->GetPositionX() - fPosX,
			pTGATE->GetPositionZ() - fPosZ);
		FLOAT fDIST = pTGATE->m_fRadius + fRadius;

		if( D3DXVec2Dot( &vDIST, &vDIST) < fDIST * fDIST )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CTClientMAP::GetWaterOBJ( LPVTOBJBASE pTWATER,
							   FLOAT fPosX,
							   FLOAT fPosZ)
{
	MAPDWORD mapTINDEX;
	mapTINDEX.clear();

	if(!m_pMAP)
		return;

	for( BYTE i=0; i<9; i++)
		if( m_pMAP->m_bUNIT[i] != 0xFF )
		{
			GetCollisionOBJ(
				pTWATER,
				&m_mapTWATER,
				&mapTINDEX, i,
				fPosX,
				fPosZ,
				0.0f,
				TWATERGRID_SIZE);
		}
	mapTINDEX.clear();
}

void CTClientMAP::LoadMAP( CD3DDevice *pDevice,
						   CTClientCAM *pCamera,
						   CTachyonRes *pRES,
						   WORD wMapID)
{
	CTClientWnd *pMainWnd = CTClientWnd::GetInstance();
	if(pMainWnd->IsLoading())
	{
		pMainWnd->m_strLOADMSG = CTChart::LoadString(TSTR_LOADMSG_LOADDATA);
		pMainWnd->Render();
	}

	CTClientApp *pTAPP = (CTClientApp *) AfxGetApp();

	LPTMAPINFO pTMAPINFO = CTChart::GetTMAPINFO(wMapID);
	CTClientFlag::InitTPATH();

	pTAPP->ExitLoadThread(TRUE);
	pTAPP->CreateLoadThread();

	m_pMAP = (CTachyonHUGEMAP *) pRES->LockMAP(pTMAPINFO->m_dwMapID);
	m_wMapID = wMapID;

	for( int i=0; i<9; i++)
	{
		m_pMAP->m_bINDEX[i] = 0xFF;
		m_pMAP->m_bUNIT[i] = 0xFF;
	}
	m_pTLOADJOINT = NULL;

	m_pMAP->m_bSHADOW = m_bSHADOW;
	m_pMAP->m_nUnitX = -2;
	m_pMAP->m_nUnitZ = -2;
	m_pMAP->m_nCellX = -2;
	m_pMAP->m_nCellZ = -2;

	m_nUnitCellX = -2;
	m_nUnitCellZ = -2;

	ResetTMAP( pDevice, pCamera, 0, TRUE);
}

void CTClientMAP::LoadMAPOBJ( CD3DDevice *pDevice,
							  BYTE bLoadNOW)
{
	CTClientApp *pTAPP = (CTClientApp *) AfxGetApp();
	LPTLOADOBJ pTLOAD = NULL;

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
				m_mapTMAPOBJ[m_pMAP->m_bUNIT[i]][j].clear();
				m_vTMAPSFX[m_pMAP->m_bUNIT[i]][j].clear();
				m_vTMAPSND[m_pMAP->m_bUNIT[i]][j].clear();
			}

			ClearTSILHOUETTE(m_pMAP->m_bUNIT[i]);
			ClearBIGOBJ(m_pMAP->m_bUNIT[i]);

			if(!pTLOAD)
				pTLOAD = new TLOADOBJ();

			pTLOAD->m_vUnitID.push_back( MAKELONG( MAKEWORD(
				bUnitX,
				bUnitZ),
				WORD(m_pMAP->m_bUNIT[i])));

			for( itOBJ = m_pMAP->m_pUNIT[i]->m_mapOBJ.begin(); itOBJ != m_pMAP->m_pUNIT[i]->m_mapOBJ.end(); itOBJ++)
			{
				BYTE bOBJType = GetAttrINT( (*itOBJ).second, ID_OBJ_TYPE);

				switch(bOBJType)
				{
				case OT_SILHOUETTE	:
					{
						if( m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_wCount >= WORD(m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ.size()) )
							m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ.push_back(new CTClientObject());
						CTClientObject *pINST = (CTClientObject *) m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ[m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_wCount];

						m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_wCount++;
						pINST->ReleaseData();

						pINST->m_bZEnable = FALSE;
						pINST->m_bZWrite = FALSE;

						pINST->InitOBJ( pDevice, (*itOBJ).second);
						pINST->m_bCollisionType = pINST->GetAttrINT(ID_COLLISION_TYPE);
						pINST->m_bCamCollision = pINST->GetAttrINT(ID_CAM_COLLISION);
						pINST->m_dwID = pINST->GetAttrINT(ID_MAP_ID);
						pINST->m_bType = pINST->GetAttrINT(ID_OBJ_TYPE);
						pINST->m_bLastDraw = BYTE(pINST->GetAttrINT(ID_LAST_DRAW));

						pINST->InitSIZE(
							(*itOBJ).second->m_vSCALE.x * pINST->GetAttrFLOAT(ID_SIZE_X),
							(*itOBJ).second->m_vSCALE.y * pINST->GetAttrFLOAT(ID_SIZE_Y),
							(*itOBJ).second->m_vSCALE.z * pINST->GetAttrFLOAT(ID_SIZE_Z),
							(*itOBJ).second->m_vSCALE.y * pINST->GetAttrFLOAT(ID_DEPTH),
							max( (*itOBJ).second->m_vSCALE.x, max(
							(*itOBJ).second->m_vSCALE.y,
							(*itOBJ).second->m_vSCALE.z)) * pINST->GetAttrFLOAT(ID_SIGHT));
						pINST->m_dwInstID = (*itOBJ).first;
					}

					break;

				case OT_FOG			: break;
				case OT_BSP			:
					{
						FLOAT fSizeX = (*itOBJ).second->m_vSCALE.x * GetAttrFLOAT( (*itOBJ).second, ID_SIZE_X);
						FLOAT fSizeY = (*itOBJ).second->m_vSCALE.y * GetAttrFLOAT( (*itOBJ).second, ID_SIZE_Y);
						FLOAT fSizeZ = (*itOBJ).second->m_vSCALE.z * GetAttrFLOAT( (*itOBJ).second, ID_SIZE_Z);
						FLOAT fSight = max(
							(*itOBJ).second->m_vSCALE.x, max(
							(*itOBJ).second->m_vSCALE.y,
							(*itOBJ).second->m_vSCALE.z)) * GetAttrFLOAT( (*itOBJ).second, ID_SIGHT);
						LPTBSPINFO pTBSP = new TBSPINFO();

						pTBSP->m_dwBspID = GetAttrINT( (*itOBJ).second, ID_MAP_ID);
						pTBSP->m_dwHouseID = GetAttrINT(
							(*itOBJ).second,
							ID_HOUSE_ID);

						pTBSP->m_fRadius = max( max( fSizeX, fSizeY), max( fSizeZ, fSight));
						pTBSP->m_bINDEX = m_pMAP->m_bUNIT[i];

						pTBSP->m_vSCALE = (*itOBJ).second->m_vSCALE;
						pTBSP->m_vPOS = (*itOBJ).second->m_vPOS;
						pTBSP->m_vROT = (*itOBJ).second->m_vROT;

						pTLOAD->m_vTBSPINFO.push_back(pTBSP);
					}
				default		:
					{
						FLOAT fSizeX = (*itOBJ).second->m_vSCALE.x * GetAttrFLOAT( (*itOBJ).second, ID_SIZE_X);
						FLOAT fSizeY = (*itOBJ).second->m_vSCALE.y * GetAttrFLOAT( (*itOBJ).second, ID_SIZE_Y);
						FLOAT fSizeZ = (*itOBJ).second->m_vSCALE.z * GetAttrFLOAT( (*itOBJ).second, ID_SIZE_Z);
						FLOAT fDepth = (*itOBJ).second->m_vSCALE.y * GetAttrFLOAT( (*itOBJ).second, ID_DEPTH);
						FLOAT fSight = max(
							(*itOBJ).second->m_vSCALE.x, max(
							(*itOBJ).second->m_vSCALE.y,
							(*itOBJ).second->m_vSCALE.z)) * GetAttrFLOAT( (*itOBJ).second, ID_SIGHT);

						FLOAT fLocalX = (*itOBJ).second->m_vPOS.x - fUnitX;
						FLOAT fLocalZ = (*itOBJ).second->m_vPOS.z - fUnitZ;

						if( bOBJType == OT_WATER || (bOBJType == OT_COLLISION || bOBJType == OT_PATHWALL ? 1.0f : 2.0f) * TOBJCELL_LENGTH < max( fSizeX, fSizeZ) )
						{
							if( m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_wCount >= WORD(m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_vTOBJ.size()) )
								m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_vTOBJ.push_back(new CTClientObject());
							CTClientObject *pINST = (CTClientObject *) m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_vTOBJ[m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_wCount];

							m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_wCount++;
							pINST->ReleaseData();
							pINST->m_bZWrite = bOBJType == OT_WATER ? FALSE : TRUE;

							pINST->InitOBJ( pDevice, (*itOBJ).second);
							pINST->m_bCollisionType = pINST->GetAttrINT(ID_COLLISION_TYPE);
							pINST->m_bCamCollision = pINST->GetAttrINT(ID_CAM_COLLISION);
							pINST->m_dwID = pINST->GetAttrINT(ID_MAP_ID);
							pINST->m_bType = pINST->GetAttrINT(ID_OBJ_TYPE);
							pINST->m_bForceDraw = BYTE(pINST->GetAttrINT(ID_FORCE_DRAW));
							pINST->m_bHouseMesh = BYTE(pINST->GetAttrINT(ID_HOUSE_MESH));
							pINST->m_dwHouseID = DWORD(pINST->GetAttrINT(ID_HOUSE_ID));
							pINST->m_bLastDraw = BYTE(pINST->GetAttrINT(ID_LAST_DRAW));

							pINST->InitSIZE(
								fSizeX,
								fSizeY,
								fSizeZ,
								fDepth,
								fSight);
							pINST->m_dwInstID = (*itOBJ).first;

							if( pINST->m_bCollisionType == TCOLLISION_POLY )
								pINST->InitCollision();
						}
						else if( 0.0f <= fLocalX && fLocalX < fUnitLength && 0.0f <= fLocalZ && fLocalZ < fUnitLength )
						{
							int nLocalX = INT(fLocalX / TOBJCELL_LENGTH);
							int nLocalZ = INT(fLocalZ / TOBJCELL_LENGTH);
							int nIndex = nLocalZ * TOBJCELL_WIDTH + nLocalX;

							m_mapTMAPOBJ[m_pMAP->m_bUNIT[i]][nIndex].insert( MAPMAPOBJ::value_type( (*itOBJ).first, (*itOBJ).second));
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

	if(pTLOAD)
	{
		LPTLOADCMD pTCMD = new TLOADCMD;

		pTCMD->m_dwTCMD = TLOADCMD_OBJ;
		pTCMD->m_pTBUF = pTLOAD;
		pTAPP->m_qTLOADCMD.push(pTCMD);

		if(!bLoadNOW)
			SetEvent(pTAPP->m_hLoadEvent);
	}

	if( bLoadNOW && (!pTAPP->m_bWait || !pTAPP->m_qTLOADCMD.empty()) )
	{
		pTAPP->ExitLoadThread(FALSE);
		pTAPP->CreateLoadThread();
	}
}

void CTClientMAP::LoadMAPOBJ( CD3DDevice *pDevice,
							  LPVTNPC pTMAPNPC,
							  LPMAPMAPOBJ pTMAPOBJ,
							  LPVTMAPSFX pTMAPSFX,
							  LPVTMAPSND pTMAPSND,
							  LPTOBJBLOCK pTOBJ,
							  LPTOBJBLOCK pTNPC,
							  LPTSFXBLOCK pTSFX,
							  LPTSNDBLOCK pTSND)
{
	FLOAT fUnitLength = FLOAT(m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);
	MAPMAPOBJ::iterator itTOBJ;

	for( int i=0; i<INT(pTMAPNPC->size()); i++)
	{
		if( pTNPC->m_wCount >= WORD(pTNPC->m_vTOBJ.size()) )
			pTNPC->m_vTOBJ.push_back(new CTClientNpc());
		CTClientNpc *pINST = (CTClientNpc *) pTNPC->m_vTOBJ[pTNPC->m_wCount];

		pINST->ReleaseData();
		pTNPC->m_wCount++;

		pINST->m_pTNPC = (*pTMAPNPC)[i]->m_pTNPC;
		pINST->m_dwID = (*pTMAPNPC)[i]->m_dwID;

		pINST->m_dwQuestID = (*pTMAPNPC)[i]->m_dwQuestID;
		pINST->m_dwHouseID = (*pTMAPNPC)[i]->m_dwHouseID;
		pINST->m_dwExecID = (*pTMAPNPC)[i]->m_dwExecID;
		pINST->m_wItemID = (*pTMAPNPC)[i]->m_wItemID;
		pINST->m_dwMaxHP = (*pTMAPNPC)[i]->m_dwMaxHP;

		pINST->m_strTITLE = (*pTMAPNPC)[i]->m_strTITLE;
		pINST->m_strNAME = (*pTMAPNPC)[i]->m_strNAME;

		pINST->m_bCollisionType = (*pTMAPNPC)[i]->m_bCollisionType;
		pINST->m_bCanSelected = (*pTMAPNPC)[i]->m_bCanSelected;
		pINST->m_bHouseMesh = (*pTMAPNPC)[i]->m_bHouseMesh;
		pINST->m_bDrawGhost = (*pTMAPNPC)[i]->m_bDrawGhost;
		pINST->m_bDrawMark = (*pTMAPNPC)[i]->m_bDrawMark;
		pINST->m_bDrawName = (*pTMAPNPC)[i]->m_bDrawName;
		pINST->m_bNPCType = (*pTMAPNPC)[i]->m_bNPCType;
		pINST->m_bClassID = (*pTMAPNPC)[i]->m_bClassID;
		pINST->m_pTPOPUP = (*pTMAPNPC)[i]->m_pTPOPUP;
		pINST->m_bLevel = (*pTMAPNPC)[i]->m_bLevel;
		pINST->m_bLand = (*pTMAPNPC)[i]->m_bLand;
		pINST->m_bMode = (*pTMAPNPC)[i]->m_bMode;
		pINST->m_bCamp = (*pTMAPNPC)[i]->m_bCamp;

		// 국가ID를 두개로 나눈 이유는 m_bCountryID는 중립지역 NPC같은 경우
		// 해당 국가ID(드프겔,크락시온)로 세팅되기 때문에 원래 중립지역 NPC인 경우를 알기 위해서 오리지널 국가ID를 따로 보존한다.
		pINST->m_bOriginalContryID = (*pTMAPNPC)[i]->m_bCountryID;
		pINST->m_bContryID = (*pTMAPNPC)[i]->m_bCountryID; // <- 얘는 추후에 패킷을 통해서 다시 변경 될 것이다.

		pINST->InitTNPC(
			pDevice,
			m_pMAP->m_pRES,
			(*pTMAPNPC)[i]);
		pINST->m_dwHP = pINST->m_dwMaxHP;

		if( pINST->m_bNPCType != TNPC_MAPPORTAL )
		{
			m_mapTNpc.insert( MAPOBJECT::value_type( pINST->m_dwID, pINST));
			m_vNEWQUESTOBJ.push_back(pINST->m_dwID);
		}
		else
		{
			pINST->m_bCamCollision = TRUE;
			m_mapTPortal.insert( MAPOBJECT::value_type( pINST->m_dwID, pINST));
		}
	}

	for( itTOBJ = pTMAPOBJ->begin(); itTOBJ != pTMAPOBJ->end(); itTOBJ++)
	{
		if( pTOBJ->m_wCount >= WORD(pTOBJ->m_vTOBJ.size()) )
			pTOBJ->m_vTOBJ.push_back(new CTClientObject());
		CTClientObject *pINST = (CTClientObject *) pTOBJ->m_vTOBJ[pTOBJ->m_wCount];

		pINST->ReleaseData();
		pTOBJ->m_wCount++;
		pINST->InitOBJ( pDevice, (*itTOBJ).second);

		pINST->m_bCollisionType = pINST->GetAttrINT(ID_COLLISION_TYPE);
		pINST->m_bCamCollision = pINST->GetAttrINT(ID_CAM_COLLISION);
		pINST->m_dwID = pINST->GetAttrINT(ID_MAP_ID);
		pINST->m_bType = pINST->GetAttrINT(ID_OBJ_TYPE);
		pINST->m_bForceDraw = BYTE(pINST->GetAttrINT(ID_FORCE_DRAW));
		pINST->m_bHouseMesh = BYTE(pINST->GetAttrINT(ID_HOUSE_MESH));
		pINST->m_dwHouseID = DWORD(pINST->GetAttrINT(ID_HOUSE_ID));
		pINST->m_bLastDraw = BYTE(pINST->GetAttrINT(ID_LAST_DRAW));

		pINST->InitSIZE(
			(*itTOBJ).second->m_vSCALE.x * pINST->GetAttrFLOAT(ID_SIZE_X),
			(*itTOBJ).second->m_vSCALE.y * pINST->GetAttrFLOAT(ID_SIZE_Y),
			(*itTOBJ).second->m_vSCALE.z * pINST->GetAttrFLOAT(ID_SIZE_Z),
			(*itTOBJ).second->m_vSCALE.y * pINST->GetAttrFLOAT(ID_DEPTH),
			max( (*itTOBJ).second->m_vSCALE.x, max(
			(*itTOBJ).second->m_vSCALE.y,
			(*itTOBJ).second->m_vSCALE.z)) * pINST->GetAttrFLOAT(ID_SIGHT));
		pINST->m_dwInstID = (*itTOBJ).first;

		if( pINST->m_bCollisionType == TCOLLISION_POLY )
			pINST->InitCollision();
	}

	for( i=0; i<INT(pTMAPSFX->size()); i++)
	{
		if( pTSFX->m_wCount >= WORD(pTSFX->m_vTSFX.size()) )
			pTSFX->m_vTSFX.push_back(new CTClientSFX());
		CTClientSFX *pINST = (CTClientSFX *) pTSFX->m_vTSFX[pTSFX->m_wCount];
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

		pINST->m_dwInstID = CTachyonSFX::RegSFX( pINST, (*pTMAPSFX)[i]->m_bBias);
	}

	for( i=0; i<INT(pTMAPSND->size()); i++)
	{
		if( pTSND->m_wCount >= WORD(pTSND->m_vTSND.size()) )
			pTSND->m_vTSND.push_back(new SNDIDX());
		LPSNDIDX pIDX = pTSND->m_vTSND[pTSND->m_wCount];
		pTSND->m_wCount++;

		pIDX->m_pSND = (CD3DSound *) CTachyonRes::m_MEDIA.GetData(
			MEDIA_TSOUND,
			(*pTMAPSND)[i]->m_dwMediaID);
		pIDX->m_nIndex = pIDX->m_pSND->Lock();

		if( pIDX->m_nIndex >= 0 )
		{
			LPDIRECTSOUND3DBUFFER p3DBUF = pIDX->m_pSND->GetDS3D(pIDX->m_nIndex);

			if(p3DBUF)
			{
				p3DBUF->SetPosition(
					(*pTMAPSND)[i]->m_vPOS.x,
					(*pTMAPSND)[i]->m_vPOS.y,
					(*pTMAPSND)[i]->m_vPOS.z,
					DS3D_IMMEDIATE);
			}
		}
		else
		{
			pIDX->m_nIndex = -1;
			pIDX->m_pSND = NULL;

			pTSND->m_wCount--;
		}
	}
}

void CTClientMAP::ClearMAPOBJ( LPTOBJBLOCK pTOBJ,
							   LPTOBJBLOCK pTNPC,
							   LPTSFXBLOCK pTSFX,
							   LPTSNDBLOCK pTSND)
{
	for( WORD i=0; i<pTOBJ->m_wCount; i++)
		pTOBJ->m_vTOBJ[i]->Release();

	for( i=0; i<pTNPC->m_wCount; i++)
	{
		LPMAPOBJECT pTMAPNPC = pTNPC->m_vTOBJ[i]->m_bNPCType == TNPC_MAPPORTAL ?
			&m_mapTPortal : &m_mapTNpc;

		MAPOBJECT::iterator finder = pTMAPNPC->find(pTNPC->m_vTOBJ[i]->m_dwID);

		if( finder != pTMAPNPC->end() )
			pTMAPNPC->erase(finder);

		pTNPC->m_vTOBJ[i]->Release();
	}

	for( i=0; i<pTSFX->m_wCount; i++)
	{
		CTClientSFX *pINST = (CTClientSFX *) pTSFX->m_vTSFX[i];
		MAPTSFX::iterator finder = CTachyonSFX::m_mapINST.find( pINST->m_dwInstID);

		if( finder != CTachyonSFX::m_mapINST.end() )
			CTachyonSFX::m_mapINST.erase(finder);

		pINST->m_dwInstID = 0;
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
	pTNPC->m_wCount = 0;
	pTSFX->m_wCount = 0;
	pTSND->m_wCount = 0;
}

void CTClientMAP::CheckMAPOBJ( CD3DDevice *pDevice,
							   LPD3DXVECTOR3 pPOS,
							   BYTE bUPDATE)
{
	int nTCellX = INT(pPOS->x / TOBJCELL_LENGTH) - TDRAWCELL_WIDTH / 2;
	int nTCellZ = INT(pPOS->z / TOBJCELL_LENGTH) - TDRAWCELL_WIDTH / 2;

	if( nTCellX != m_nTCellX ||
		nTCellZ != m_nTCellZ ||
		bUPDATE )
	{
		FLOAT fUnitLength = FLOAT(m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);

		if( nTCellX != m_nTCellX ||
			nTCellZ != m_nTCellZ )
		{
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
							&m_vTNPC[vTCELL[i]],
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

			m_nTCellX = nTCellX;
			m_nTCellZ = nTCellZ;
		}

		for( WORD i=0; i<TDRAWCELL_COUNT; i++)
		{
			FLOAT fPosX = FLOAT(nTCellX + i % TDRAWCELL_WIDTH) * TOBJCELL_LENGTH;
			FLOAT fPosZ = FLOAT(nTCellZ + i / TDRAWCELL_WIDTH) * TOBJCELL_LENGTH;

			int nUnitX = INT(fPosX / fUnitLength) - m_pMAP->m_nUnitX;
			int nUnitZ = INT(fPosZ / fUnitLength) - m_pMAP->m_nUnitZ;

			if( nUnitX >= 0 && nUnitX < 3 &&
				nUnitZ >= 0 && nUnitZ < 3 )
			{
				BYTE bIndex = nUnitZ * 3 + nUnitX;

				if( m_vTCELL[i] == 0xFFFF && m_pMAP->m_pUNIT[bIndex] && IsValidLoadUNIT( m_pTLOADJOINT, INT(bIndex)) )
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
									&m_vTMAPNPC[m_pMAP->m_bUNIT[bIndex]][nIndex],
									&m_mapTMAPOBJ[m_pMAP->m_bUNIT[bIndex]][nIndex],
									&m_vTMAPSFX[m_pMAP->m_bUNIT[bIndex]][nIndex],
									&m_vTMAPSND[m_pMAP->m_bUNIT[bIndex]][nIndex],
									&m_vTOBJ[j],
									&m_vTNPC[j],
									&m_vTSFX[j],
									&m_vTSND[j]);

								break;
							}
					}
				}
				else if( m_vTCELL[i] != 0xFFFF && !IsValidLoadUNIT( m_pTLOADJOINT, INT(bIndex)) )
				{
					ClearMAPOBJ(
						&m_vTOBJ[m_vTCELL[i]],
						&m_vTNPC[m_vTCELL[i]],
						&m_vTSFX[m_vTCELL[i]],
						&m_vTSND[m_vTCELL[i]]);

					m_vLOCK[m_vTCELL[i]] = FALSE;
					m_vTCELL[i] = 0xFFFF;
				}
			}
		}
	}

	CheckUNIT(bUPDATE);
}

void CTClientMAP::CheckUNIT( BYTE bUPDATE)
{
	if( m_pMAP->m_nCellX != m_nUnitCellX ||
		m_pMAP->m_nCellZ != m_nUnitCellZ ||
		bUPDATE )
	{
		MAPTPATHFLAG::iterator itTPATH;

		int nCellLength = m_pMAP->m_nTileLength * m_pMAP->m_nUnitLength / m_pMAP->m_nCellCount;
		int nCellX = m_pMAP->m_nCellX + 1;
		int nCellZ = m_pMAP->m_nCellZ + 1;

		CRect rect(
			-nCellLength,
			-nCellLength,
			2 * nCellLength,
			2 * nCellLength);

		for( BYTE i=0; i<9; i++)
			m_vUNIT[i] = FALSE;

		rect.OffsetRect(
			nCellX * nCellLength,
			nCellZ * nCellLength);
		m_vTPATH.clear();

		m_nUnitCellX = m_pMAP->m_nCellX;
		m_nUnitCellZ = m_pMAP->m_nCellZ;

		nCellX %= m_pMAP->m_nCellCount;
		nCellZ %= m_pMAP->m_nCellCount;

		m_vUNIT[0] = m_pMAP->m_pUNIT[0] && !nCellX && !nCellZ ? TRUE : FALSE;
		m_vUNIT[1] = m_pMAP->m_pUNIT[1] && !nCellZ ? TRUE : FALSE;
		m_vUNIT[2] = m_pMAP->m_pUNIT[2] && nCellX >= m_pMAP->m_nCellCount - 1 && !nCellZ ? TRUE : FALSE;
		m_vUNIT[3] = m_pMAP->m_pUNIT[3] && !nCellX ? TRUE : FALSE;
		m_vUNIT[4] = m_pMAP->m_pUNIT[4] ? TRUE : FALSE;
		m_vUNIT[5] = m_pMAP->m_pUNIT[5] && nCellX >= m_pMAP->m_nCellCount - 1 ? TRUE : FALSE;
		m_vUNIT[6] = m_pMAP->m_pUNIT[6] && !nCellX && nCellZ >= m_pMAP->m_nCellCount - 1 ? TRUE : FALSE;
		m_vUNIT[7] = m_pMAP->m_pUNIT[7] && nCellZ >= m_pMAP->m_nCellCount - 1 ? TRUE : FALSE;
		m_vUNIT[8] = m_pMAP->m_pUNIT[8] && nCellX >= m_pMAP->m_nCellCount - 1 && nCellZ >= m_pMAP->m_nCellCount - 1 ? TRUE : FALSE;

		for( itTPATH = CTClientFlag::m_mapTPATHFLAG.begin(); itTPATH != CTClientFlag::m_mapTPATHFLAG.end(); itTPATH++)
			if(rect.PtInRect(CPoint( INT((*itTPATH).second->m_vPOS.x), INT((*itTPATH).second->m_vPOS.z))))
				m_vTPATH.push_back((*itTPATH).second);
	}
}

FLOAT CTClientMAP::GetIntensity()
{
	return m_pMAP->m_fIntensity;
}

FLOAT CTClientMAP::GetSpecRange()
{
	return m_pMAP->m_fSpecRange;
}

FLOAT CTClientMAP::GetSpecMax()
{
	return m_pMAP->m_fSpecMax;
}

FLOAT CTClientMAP::GetHeight( CTClientObjBase *pOBJ,
							  CTClientObjBase **pFLOOR,
							  LPD3DXVECTOR3 pPOINT,
							  FLOAT fMove,
							  BYTE bCheckBound)
{
	FLOAT fResult = m_pMAP->GetHeight(
		pPOINT->x,
		pPOINT->y,
		pPOINT->z);

	CTClientObjBase *pFloorSUB = NULL;
	VTOBJBASE vTCOLLISION;

	FLOAT fPrevH = pPOINT->y + fMove / CTClientMAP::m_fMoveBound;
	FLOAT fLandH = fResult;
	FLOAT fFloorH = TMIN_HEIGHT;

	if( pPOINT->y + pOBJ->m_fSizeY < fResult ||
		GetRegionID( pPOINT->x, pPOINT->z) == BLANK_REGION )
		fResult = TMIN_HEIGHT;

	if(pFLOOR)
		(*pFLOOR) = NULL;

	vTCOLLISION.clear();
	GetCollisionOBJ(
		&vTCOLLISION,
		pPOINT->x,
		pPOINT->z,
		pOBJ->m_fRadius);

	int nCount = INT(vTCOLLISION.size());
	for( int i=0; i<nCount; i++)
	{
		CTClientObjBase *pTCOLL = vTCOLLISION[i];

		if( !pTCOLL->m_bGARBAGE && 
			pTCOLL->m_bCollisionType != TCOLLISION_NONE && (
			pTCOLL->m_bType != OT_GATE || (
			((CTClientGate *) pTCOLL)->m_pTGATE &&
			((CTClientGate *) pTCOLL)->m_pTGATE->m_bStair)) &&
			pTCOLL->m_bType != OT_SWITCH &&
			pTCOLL != pOBJ )
		{
			FLOAT fHeight = pTCOLL->GetHeight(
				pOBJ,
				&D3DXVECTOR3(
				pPOINT->x,
				fPrevH,
				pPOINT->z));

			if( fResult < fHeight && fHeight <= fPrevH )
			{
				if(pFLOOR)
					(*pFLOOR) = pTCOLL;

				fResult = fHeight;
			}

			if( fHeight > TMIN_HEIGHT && (!pFloorSUB || fHeight < fFloorH) )
			{
				pFloorSUB = pTCOLL;
				fFloorH = fHeight;
			}
		}
	}
	vTCOLLISION.clear();

	if( fResult <= TMIN_HEIGHT )
	{
		BYTE bSUB = pFloorSUB && fFloorH < fLandH ? TRUE : FALSE;

		if(pFLOOR)
			(*pFLOOR) = bSUB ? pFloorSUB : NULL;

		fResult = bSUB ? fFloorH : fLandH;
	}

	return fResult + THEIGHT_ZERO;
}

FLOAT CTClientMAP::GetLandHeight( FLOAT fPosX,
								  FLOAT fPosY,
								  FLOAT fPosZ)
{
	return m_pMAP ? m_pMAP->GetHeight(
		fPosX,
		fPosY,
		fPosZ) + THEIGHT_ZERO : 0.0f;
}

FLOAT CTClientMAP::GetWaterHeight( CTClientObjBase *pWATER,
								   FLOAT fPosX,
								   FLOAT fPosZ)
{
	MAPCLKINST::iterator it;

	if( !pWATER || pWATER->m_bGARBAGE )
		return TMIN_HEIGHT;

	for( it = pWATER->m_OBJ.m_mapCLK.begin(); it != pWATER->m_OBJ.m_mapCLK.end(); it++)
	{
		LPCLKINST pCI = (*it).second;

		if( pCI && pCI->m_pMESH && pCI->m_pMESH->m_pMESH &&
			pCI->m_pMESH->m_pMESH->m_pDATA &&
			pCI->m_pMESH->m_pMESH->m_pDATA->m_dwCount )
		{
			LPMESHVERTEX pDATA = (LPMESHVERTEX) pCI->m_pMESH->m_pMESH->m_pDATA->m_pVB;
			CTachyonMesh *pMESH = pCI->m_pMESH->m_pMESH;

			for( int j=0; j<pMESH->m_dwMeshCount; j++)
				for( int k=0; k<INT(pMESH->m_pMESH[j][0]->m_vIB.size()); k++)
					for( int l=0; l<pMESH->m_pMESH[j][0]->m_vIB[k]->m_dwCount / 3; l++)
					{
						D3DXVECTOR3 vPoint[3];

						FLOAT fDist;
						FLOAT fU;
						FLOAT fV;

						for( int m=0; m<3; m++)
						{
							vPoint[m] = D3DXVECTOR3(
								pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[l * 3 + m]].m_fPosX,
								pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[l * 3 + m]].m_fPosY,
								pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[l * 3 + m]].m_fPosZ);

							CTMath::Transform(
								&pWATER->GetPositionMat(),
								&vPoint[m]);
						}

						if( D3DXIntersectTri(
							&vPoint[0],
							&vPoint[1],
							&vPoint[2],
							&D3DXVECTOR3(
							fPosX,
							-TMIN_HEIGHT,
							fPosZ),
							&D3DXVECTOR3(
							0.0f,
							-1.0f,
							0.0f),
							&fU, &fV,
							&fDist))
							return -TMIN_HEIGHT - fDist;
					}
		}
	}

	return TMIN_HEIGHT;
}

FLOAT CTClientMAP::GetWaterHeight( FLOAT fPosX,
								   FLOAT fPosZ)
{
	FLOAT fHeight = TMIN_HEIGHT;
	VTOBJBASE vTWATER;

	vTWATER.clear();
	GetWaterOBJ(
		&vTWATER,
		fPosX,
		fPosZ);

	int nCount = INT(vTWATER.size());
	for( int i=0; i<nCount; i++)
	{
		FLOAT fLocal = GetWaterHeight( vTWATER[i], fPosX, fPosZ);

		if( fHeight < fLocal )
			fHeight = fLocal;
	}
	vTWATER.clear();

	return fHeight;
}

LPTREGIONINFO CTClientMAP::GetRegionINFO( FLOAT fPosX,
										  FLOAT fPosZ)
{
	BYTE bRegionID = GetRegionID( fPosX, fPosZ);

	BYTE bUnitX = INT(fPosX) / (m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);
	BYTE bUnitZ = INT(fPosZ) / (m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);
	BYTE bUnitID = bUnitZ * m_pMAP->m_nUnitCountX + bUnitX;

	return CTChart::FindTREGIONINFO(MAKELONG( MAKEWORD( bRegionID, bUnitID), m_wMapID));
}

BYTE CTClientMAP::GetRegionID( FLOAT fPosX,
							   FLOAT fPosZ)
{
	int nTileX = INT(fPosX) / m_pMAP->m_nTileLength;
	int nTileZ = INT(fPosZ) / m_pMAP->m_nTileLength;

	return m_pMAP->GetRegionID( nTileX, nTileZ);
}

BYTE CTClientMAP::PtInSHADOW( FLOAT fPosX,
							  FLOAT fPosZ)
{
	return m_pMAP->PtInSHADOW( fPosX, fPosZ);
}

void CTClientMAP::ResetTRSCS( LPDIRECT3DTEXTURE9 pTBACKTEX,
							  LPDIRECT3DTEXTURE9 pTRSCSTEX,
							  LPDIRECT3DTEXTURE9 pTMASKTEX,
							  CTRSCSDlg *pTRSCS)
{
	static D3DXMATRIX vWORLD(
		 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.0f, 0.5f, 1.0f);

	if( !pTBACKTEX || !pTRSCSTEX || !pTMASKTEX || !pTRSCS )
		return;

	LPDIRECT3DSURFACE9 pTBACKBUF = NULL;
	LPDIRECT3DSURFACE9 pTRSCSBUF = NULL;
	LPDIRECT3DSURFACE9 pTMASKBUF = NULL;
	LPDIRECT3DSURFACE9 pBACK = NULL;
	LPDIRECT3DSURFACE9 pZBUF = NULL;

	MAPTENEMY::iterator itTENEMY;
	CD3DCamera vCAMERA;
	FLOAT fMIP = 0.0f;

	D3DVIEWPORT9 vNEW = {
		0, 0,
		TMINIMAPTEX_SIZE,
		TMINIMAPTEX_SIZE,
		0.0f,
		1.0f};
	D3DVIEWPORT9 vOLD;

	pTRSCS->m_pDevice->m_pDevice->GetDepthStencilSurface(&pZBUF);
	pTRSCS->m_pDevice->m_pDevice->GetRenderTarget( 0, &pBACK);
	pTRSCS->m_pDevice->m_pDevice->GetViewport(&vOLD);

	pTBACKTEX->GetSurfaceLevel( 0, &pTBACKBUF);
	pTRSCSTEX->GetSurfaceLevel( 0, &pTRSCSBUF);
	pTMASKTEX->GetSurfaceLevel( 0, &pTMASKBUF);

	pTRSCS->m_pDevice->m_pDevice->SetDepthStencilSurface(NULL);
	pTRSCS->m_pDevice->m_pDevice->SetViewport(&vNEW);

	vCAMERA.InitOrthoCamera(
		pTRSCS->m_pDevice->m_pDevice,
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

	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	pTRSCS->m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &fMIP));
	pTRSCS->m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x000000E0);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

	pTRSCS->m_pDevice->m_pDevice->SetStreamSource( 0, CTClientGame::m_vTRECTVB.GetVB(), 0, sizeof(PVERTEX));
	pTRSCS->m_pDevice->m_pDevice->SetFVF(T3DFVF_PVERTEX);
	pTRSCS->m_pDevice->m_pDevice->SetRenderTarget( 0, pTMASKBUF);

	pTRSCS->m_pDevice->m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(255,0,0,0),
		1.0f, 0);

	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_MAX);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pTRSCS->m_pDevice->m_pDevice->SetTexture( 0, CTRSCSDlg::m_pTMASK ? CTRSCSDlg::m_pTMASK->GetTexture() : NULL);

	for( BYTE i=0; i<BYTE(pTRSCS->m_vTCORPS.m_vTSQUAD.size()); i++)
		if( pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_wPartyID == pTRSCS->m_pHost->m_wPartyID || pTRSCS->m_pHost->m_dwChiefID == pTRSCS->m_pHost->m_dwID )
			for( BYTE j=0; j<BYTE(pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT.size()); j++)
				if( pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]->m_wMapID == pTRSCS->m_wMapID &&
					pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]->m_dwHP && (
					pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_dwChiefID == pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]->m_dwCharID ||
					pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_wPartyID == pTRSCS->m_pHost->m_wPartyID) )
					ResetTRSCSMASK( pTRSCS, pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]);

	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	pTRSCS->m_pDevice->m_pDevice->SetRenderTarget( 0, pTRSCSBUF);

	pTRSCS->m_pDevice->m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(
		0, 0, 0, 0),
		1.0f, 0);

	if( pTRSCS->m_dwBspID == NODEID_NULL || m_dwHouseID == 0 ||
		!ResetTMINIMAP( pTRSCS->m_pDevice, WORD(pTRSCS->m_dwBspID), pTRSCS->m_vTCENTER.x, pTRSCS->m_vTCENTER.y, pTRSCS->m_fTSCALE) )
		ResetTMINIMAP( pTRSCS->m_pDevice, pTRSCS->m_wMapID, pTRSCS->m_vTCENTER.x, pTRSCS->m_vTCENTER.y, pTRSCS->m_fTSCALE);
	pTRSCS->m_pDevice->m_pDevice->SetRenderTarget( 0, pTBACKBUF);

	pTRSCS->m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);
	pTRSCS->m_pDevice->m_pDevice->SetTexture( 0, pTRSCSTEX);

	pTRSCS->m_pDevice->m_pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0, 2);

	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pTRSCS->m_pDevice->m_pDevice->SetTexture( 0, m_pTMON ? m_pTMON->GetTexture() : NULL);

	for( itTENEMY = pTRSCS->m_vTCORPS.m_mapFIXRECALL.begin(); itTENEMY != pTRSCS->m_vTCORPS.m_mapFIXRECALL.end(); itTENEMY++)
		if( (*itTENEMY).second->m_wMapID == pTRSCS->m_wMapID )
			ResetTRSCSOBJ( pTRSCS, &pTRSCS->GetEnemyPOS((*itTENEMY).second));

	for( itTENEMY = pTRSCS->m_vTCORPS.m_mapRECALL.begin(); itTENEMY != pTRSCS->m_vTCORPS.m_mapRECALL.end(); itTENEMY++)
		if( (*itTENEMY).second->m_wMapID == pTRSCS->m_wMapID )
			ResetTRSCSOBJ( pTRSCS, &pTRSCS->GetEnemyPOS((*itTENEMY).second));

	for( itTENEMY = pTRSCS->m_vTCORPS.m_mapMON.begin(); itTENEMY != pTRSCS->m_vTCORPS.m_mapMON.end(); itTENEMY++)
		if( (*itTENEMY).second->m_wMapID == pTRSCS->m_wMapID )
			ResetTRSCSOBJ( pTRSCS, &pTRSCS->GetEnemyPOS((*itTENEMY).second));
	pTRSCS->m_pDevice->m_pDevice->SetTexture( 0, m_pTPC ? m_pTPC->GetTexture() : NULL);

	for( itTENEMY = pTRSCS->m_vTCORPS.m_mapPC.begin(); itTENEMY != pTRSCS->m_vTCORPS.m_mapPC.end(); itTENEMY++)
		if( (*itTENEMY).second->m_wMapID == pTRSCS->m_wMapID )
			ResetTRSCSOBJ( pTRSCS, &pTRSCS->GetEnemyPOS((*itTENEMY).second));

	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);

	pTRSCS->m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);
	pTRSCS->m_pDevice->m_pDevice->SetTexture( 0, pTMASKTEX);

	pTRSCS->m_pDevice->m_pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0, 2);

	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pTRSCS->m_pDevice->m_pDevice->SetRenderTarget( 0, pTRSCSBUF);

	pTRSCS->m_pDevice->m_pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0, 2);

	pTRSCS->m_pDevice->m_pDevice->SetRenderTarget( 0, pTBACKBUF);
	pTRSCS->m_pDevice->m_pDevice->SetTexture( 0, pTRSCSTEX);

	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);

	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_BLENDFACTOR, 0xFFB0B0B0);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);

	pTRSCS->m_pDevice->m_pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0, 2);

	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFF0F020);

	for( i=0; i<BYTE(pTRSCS->m_vTCORPS.m_vTSQUAD.size()); i++)
		if( pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_wPartyID == pTRSCS->m_pHost->m_wPartyID || pTRSCS->m_pHost->m_dwChiefID == pTRSCS->m_pHost->m_dwID )
			for( BYTE j=0; j<BYTE(pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT.size()); j++)
				if( pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]->m_wMapID == pTRSCS->m_wMapID &&
					pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]->m_dwHP && (
					pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_dwChiefID == pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]->m_dwCharID ||
					pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_wPartyID == pTRSCS->m_pHost->m_wPartyID) )
					ResetTRSCSUNIT( pTRSCS, pTRSCS->m_vTCORPS.m_vTSQUAD[i], pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]);

	if( pTRSCS->m_wMapID == m_wMapID && pTRSCS->m_dwTALARM )
	{
		pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000001);
		pTRSCS->m_pDevice->m_pDevice->SetTexture( 0, m_pTCMDALARM ? m_pTCMDALARM->GetTexture() : NULL);

		ResetTRSCSALARM(pTRSCS);
	}

	CTClientGame* pGame = CTClientGame::GetInstance();

	MAPSIEGEGODTOWER::iterator itor_tower = SiegeMgr->m_mapTowers.begin();
	while( itor_tower != SiegeMgr->m_mapTowers.end() )
	{
		BYTE bCAMP = (*itor_tower).second->GetCamp();

		ResetTMINIMAPSIEGE(
			pTRSCS->m_pDevice,
			pTRSCS->m_wMapID,
			m_pTGodTower[bCAMP] ? m_pTGodTower[bCAMP]->GetTexture() : NULL,
			&D3DXVECTOR2(
			pTRSCS->m_vTCENTER.x,
			pTRSCS->m_vTCENTER.y),
			&D3DXVECTOR2(
			(*itor_tower).second->GetPositionX(),
			(*itor_tower).second->GetPositionZ() ),
			m_fTSCALE );

		++itor_tower;
	}

	MAPSIEGEGODBALL::iterator itor_ball = SiegeMgr->m_mapBalls.begin();
	while( itor_ball != SiegeMgr->m_mapBalls.end() )
	{
		LPTEXTURESET pTexBall = m_pTSiegeGodBall;
		CTClientChar* pChar = pGame->GetMainChar();
		WORD wBallCamp = (*itor_ball).second->GetCamp();

		if( wBallCamp == pChar->GetCamp() &&
			(*itor_ball).second->IsVisible() )
		{
			D3DXVECTOR3 vPosBall = (*itor_ball).second->GetPosition();

			ResetTMINIMAPSIEGE(
				pTRSCS->m_pDevice,
				pTRSCS->m_wMapID,
				pTexBall ? pTexBall->GetTexture() : NULL,
				&D3DXVECTOR2(
				pTRSCS->m_vTCENTER.x,
				pTRSCS->m_vTCENTER.y),
				&D3DXVECTOR2(
				vPosBall.x,
				vPosBall.z),
				m_fTSCALE);
		}

		++itor_ball;
	}

	pTRSCS->m_pDevice->m_pDevice->SetRenderTarget( 0, pBACK);
	pTRSCS->m_pDevice->m_pDevice->SetDepthStencilSurface(pZBUF);
	pTRSCS->m_pDevice->m_pDevice->SetViewport(&vOLD);

	pTBACKBUF->Release();
	pTRSCSBUF->Release();
	pTMASKBUF->Release();
	pBACK->Release();
	pZBUF->Release();

	pTRSCS->m_pCAM->Activate(TRUE);
}

void CTClientMAP::ResetTRSCSUNIT( CTRSCSDlg *pTRSCS,
								  LPTSQUAD pTSQUAD,
								  LPTUNIT pTUNIT)
{
	FLOAT fSCALE = pTRSCS->m_fTSCALE / FLOAT(m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);

	LPTEXTURESET pTCMD = NULL;
	PVERTEX vTLINE[2];
	D3DXVECTOR2 vTUNIT = pTRSCS->GetUnitPOS(pTUNIT);

	BYTE bROT = FALSE;

	CTClientChar* pHost = CTClientGame::GetInstance()->GetMainChar();

	if ( pTUNIT->m_dwCharID == pHost->m_dwID )
	{
		pTRSCS->m_pDevice->m_pDevice->SetTexture( 0, m_pTPOS ? m_pTPOS->GetTexture() : NULL);
		bROT = TRUE;
	}
	else if( pTSQUAD->m_dwChiefID != pTUNIT->m_dwCharID )
		pTRSCS->m_pDevice->m_pDevice->SetTexture( 0, m_pTUNIT ? m_pTUNIT->GetTexture() : NULL);
	else if( pTRSCS->m_pHost->m_wCommanderID != pTSQUAD->m_wPartyID )
		pTRSCS->m_pDevice->m_pDevice->SetTexture( 0, m_pTSQUAD ? m_pTSQUAD->GetTexture() : NULL);
	else
		pTRSCS->m_pDevice->m_pDevice->SetTexture( 0, m_pTCOMMANDER ? m_pTCOMMANDER->GetTexture() : NULL);

	switch(pTUNIT->m_bTCMD)
	{
	case TRSCSCMD_PROTECT	:
		{
			LPTUNIT pTPROTECT = pTRSCS->FindTUNIT(
				pTRSCS->FindTSQUAD(pTUNIT->m_wTargetX),
				pTUNIT->m_dwTargetID);

			if( !pTPROTECT || !pTPROTECT->m_dwHP )
			{
				ResetTRSCSOBJ( pTRSCS, &vTUNIT);
				return;
			}

			D3DXVECTOR2 vTPOS = pTRSCS->GetUnitPOS(pTPROTECT);
			vTLINE[1].m_fPosX = vTPOS.x;
			vTLINE[1].m_fPosZ = vTPOS.y;
			pTCMD = m_pTCMDPROTECT;
		}

		break;

	case TRSCSCMD_ATTACK	:
		{
			LPTENEMY pTENEMY = pTRSCS->FindTENEMY(
				pTUNIT->m_dwTargetID,
				pTUNIT->m_bTargetType);

			if(!pTENEMY)
			{
				ResetTRSCSOBJ( pTRSCS, &vTUNIT);
				return;
			}

			D3DXVECTOR2 vTPOS = pTRSCS->GetEnemyPOS(pTENEMY);
			vTLINE[1].m_fPosX = vTPOS.x;
			vTLINE[1].m_fPosZ = vTPOS.y;
			pTCMD = m_pTCMDATTACK;
		}

		break;

	case TRSCSCMD_MOVE		:
		{
			vTLINE[1].m_fPosX = FLOAT(pTUNIT->m_wTargetX);
			vTLINE[1].m_fPosZ = FLOAT(pTUNIT->m_wTargetZ);
			pTCMD = m_pTCMDMOVE;
		}

		break;

	default					: ResetTRSCSOBJ( pTRSCS, &vTUNIT, bROT); return;
	}

	D3DXMATRIX vSCALE;
	D3DXMATRIX vPOS;

	D3DXMatrixScaling(
		&vSCALE,
		fSCALE,
		1.0f,
		fSCALE);

	D3DXMatrixTranslation(
		&vPOS,
		-pTRSCS->m_vTCENTER.x,
		0.0f,
		-pTRSCS->m_vTCENTER.y);
	vPOS *= vSCALE;

	vTLINE[0].m_fPosX = vTUNIT.x;
	vTLINE[0].m_fPosZ = vTUNIT.y;

	vTLINE[0].m_fPosY = 0.5f;
	vTLINE[0].m_fU = 0.0f;
	vTLINE[0].m_fV = 0.0f;

	vTLINE[1].m_fPosY = 0.5f;
	vTLINE[1].m_fU = 0.0f;
	vTLINE[1].m_fV = 0.0f;

	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	pTRSCS->m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vPOS);

	pTRSCS->m_pDevice->m_pDevice->DrawPrimitiveUP(
		D3DPT_LINELIST, 1,
		vTLINE, sizeof(PVERTEX));
	pTRSCS->m_pDevice->m_pDevice->SetStreamSource( 0, CTClientGame::m_vTRECTVB.GetVB(), 0, sizeof(PVERTEX));

	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pTRSCS->m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	ResetTRSCSOBJ( pTRSCS, &vTUNIT, TRUE);

	pTRSCS->m_pDevice->m_pDevice->SetTexture( 0, pTCMD ? pTCMD->GetTexture() : NULL);
	ResetTRSCSOBJ(
		pTRSCS,
		&D3DXVECTOR2(
		vTLINE[1].m_fPosX,
		vTLINE[1].m_fPosZ));
}

void CTClientMAP::ResetTRSCSALARM( CTRSCSDlg *pTRSCS)
{
	D3DXVECTOR2 vTPOS = pTRSCS->m_fTSCALE / FLOAT(m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength) * (pTRSCS->m_vTALARM - pTRSCS->m_vTCENTER);
	D3DXMATRIX vSCALE;
	D3DXMATRIX vPOS;

	FLOAT fSCALE = TPOS_SCALE + TCMDALARM_SCALE * FLOAT(TCMDALARM_TICK - pTRSCS->m_dwTALARM) / FLOAT(TCMDALARM_TICK);
	DWORD dwTFACTOR = ((0x000000FF * pTRSCS->m_dwTALARM / TCMDALARM_TICK) << 24) & 0xFF000000;

	D3DXMatrixScaling(
		&vSCALE,
		fSCALE,
		fSCALE,
		fSCALE);

	D3DXMatrixTranslation(
		&vPOS,
		-0.5f,
		0.0f,
		0.5f);
	vPOS *= vSCALE;

	vPOS._41 += vTPOS.x;
	vPOS._43 += vTPOS.y;

	pTRSCS->m_pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwTFACTOR);
	pTRSCS->m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vPOS);
	pTRSCS->m_pDevice->m_pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0, 2);
}

void CTClientMAP::ResetTRSCSMASK( CTRSCSDlg *pTRSCS,
								  LPTUNIT pTUNIT)
{
	FLOAT fSCALE = pTRSCS->m_fTSCALE / FLOAT(m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);

	D3DXVECTOR2 vTPOS = fSCALE * (pTRSCS->GetUnitPOS(pTUNIT) - pTRSCS->m_vTCENTER);
	FLOAT fDist = D3DXVec2Length(&vTPOS);

	D3DXMATRIX vSCALE;
	D3DXMATRIX vPOS;

	D3DXMatrixScaling(
		&vSCALE,
		CTClientObjBase::m_fCamDist * fSCALE,
		CTClientObjBase::m_fCamDist * fSCALE,
		CTClientObjBase::m_fCamDist * fSCALE);

	D3DXMatrixTranslation(
		&vPOS,
		-0.5f,
		0.0f,
		0.5f);
	vPOS *= vSCALE;

	vPOS._41 += vTPOS.x;
	vPOS._43 += vTPOS.y;

	pTRSCS->m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vPOS);
	pTRSCS->m_pDevice->m_pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0, 2);
}

void CTClientMAP::ResetTRSCSOBJ( CTRSCSDlg *pTRSCS,
								 LPD3DXVECTOR2 pTPOS,
								 BYTE bROT )
{
	D3DXVECTOR2 vTPOS = pTRSCS->m_fTSCALE / FLOAT(m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength) * ((*pTPOS) - pTRSCS->m_vTCENTER);
	FLOAT fDist = D3DXVec2Length(&vTPOS);

	D3DXMATRIX vSCALE;
	D3DXMATRIX vPOS;
	D3DXMATRIX vROT;
	D3DXMatrixIdentity( &vROT );

	D3DXMatrixScaling(
		&vSCALE,
		TPOS_SCALE,
		TPOS_SCALE,
		TPOS_SCALE);

	D3DXMatrixTranslation(
		&vPOS,
		-0.5f,
		0.0f,
		0.5f);

	if ( bROT )
	{
		CTClientChar* pHost = CTClientGame::GetInstance()->GetMainChar();
		CTClientPet* pPet = pHost->GetRidingPet();

		FLOAT fDIR = FLOAT(pPet? pPet->m_wDIR: pHost->m_wDIR);

		D3DXMatrixRotationY(
			&vROT,
			D3DX_PI + fDIR * D3DX_PI / 900.0f);

	}

	vPOS *= vSCALE * vROT;

	vPOS._41 += vTPOS.x;
	vPOS._43 += vTPOS.y;

	pTRSCS->m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vPOS);
	pTRSCS->m_pDevice->m_pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0, 2);
}

void CTClientMAP::ResetTMINIMAP( LPDIRECT3DTEXTURE9 pTARGET,
								 LPMAPMONSTER pTMON,
								 CTRSCSDlg *m_pTRSCS,
								 CTClientChar *pHost,
								 CD3DDevice *pDevice,
								 CTClientCAM *pCamera)
{
	WORD i,j;

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

	if(!pTARGET)
		return;

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

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x000000E0);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);

	pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

	pDevice->m_pDevice->SetStreamSource( 0, CTClientGame::m_vTRECTVB.GetVB(), 0, sizeof(PVERTEX));
	pDevice->m_pDevice->SetFVF(T3DFVF_PVERTEX);

	pTARGET->GetSurfaceLevel( 0, &pTMINIMAP);
	pDevice->m_pDevice->SetRenderTarget( 0, pTMINIMAP);

	pDevice->m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(255, 0, 0, 0),
		1.0f, 0);

	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &fMIP));
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	WORD wMAPID = WORD(m_dwBspID);
	if( m_dwBspID == NODEID_NULL || m_dwHouseID == 0 ||
		!ResetTMINIMAP( pDevice, WORD(m_dwBspID), pHost->GetPositionX(), pHost->GetPositionZ(), m_fTSCALE) )
	{
		wMAPID = m_wMapID;
		ResetTMINIMAP( pDevice, m_wMapID, pHost->GetPositionX(), pHost->GetPositionZ(), m_fTSCALE);
	}
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	if(pHost->m_bGhost)
	{
		ResetTMINIMAPOBJ(
			pDevice,
			wMAPID,
			m_pTDEADDIR ? m_pTDEADDIR->GetTexture() : NULL,
			m_pTDEAD ? m_pTDEAD->GetTexture() : NULL,
			&D3DXVECTOR2(
			pHost->GetPositionX(),
			pHost->GetPositionZ()),
			&D3DXVECTOR2(
			pHost->m_vTDEAD.x,
			pHost->m_vTDEAD.z),
			m_fTSCALE);
	}
	else if( pTMON && pHost->m_wPursuit != 0xFFFF )
	{
		CTClientBSP *pTBSP = NULL;

		if( m_dwBspID != NODEID_NULL )
		{
			MAPTBSP::iterator itTBSP = m_mapTBsp.find(m_dwBspID);

			if( itTBSP != m_mapTBsp.end() )
				pTBSP = (*itTBSP).second;
		}

		if(pTBSP)
		{
			MAPMONSTER::iterator itTMON;

			for( itTMON = pTMON->begin(); itTMON != pTMON->end(); itTMON++)
				if( (*itTMON).second && (*itTMON).second->m_pMON && (*itTMON).second->m_pMON->m_wKind == pHost->m_wPursuit )
				{
					DWORD dwNodeID = pTBSP->GetNodeID(D3DXVECTOR3(
						(*itTMON).second->GetPositionX(),
						(*itTMON).second->GetPositionY(),
						(*itTMON).second->GetPositionZ()));

					if( dwNodeID != NODEID_NULL )
					{
						ResetTMINIMAPOBJ(
							pDevice,
							wMAPID,
							m_pTPURSUITDIR ? m_pTPURSUITDIR->GetTexture() : NULL,
							m_pTMON ? m_pTMON->GetTexture() : NULL,
							&D3DXVECTOR2(
							pHost->GetPositionX(),
							pHost->GetPositionZ()),
							&D3DXVECTOR2(
							(*itTMON).second->GetPositionX(),
							(*itTMON).second->GetPositionZ()),
							m_fTSCALE);
					}
				}
		}
		else
		{
			MAPMONSTER::iterator itTMON;

			for( itTMON = pTMON->begin(); itTMON != pTMON->end(); itTMON++)
				if( (*itTMON).second && (*itTMON).second->m_pMON && (*itTMON).second->m_pMON->m_wKind == pHost->m_wPursuit )
				{
					DWORD dwBspID = NODEID_NULL;

					for( int i=0; i<INT(m_vDRAWBSP.size()); i++)
					{
						DWORD dwNodeID = m_vDRAWBSP[i]->GetNodeID(D3DXVECTOR3(
							(*itTMON).second->GetPositionX(),
							(*itTMON).second->GetPositionY(),
							(*itTMON).second->GetPositionZ()));

						if( dwNodeID != NODEID_NULL )
						{
							dwBspID = m_vDRAWBSP[i]->m_dwBspID;
							break;
						}
					}

					if( dwBspID == NODEID_NULL )
					{
						ResetTMINIMAPOBJ(
							pDevice,
							wMAPID,
							m_pTPURSUITDIR ? m_pTPURSUITDIR->GetTexture() : NULL,
							m_pTMON ? m_pTMON->GetTexture() : NULL,
							&D3DXVECTOR2(
							pHost->GetPositionX(),
							pHost->GetPositionZ()),
							&D3DXVECTOR2(
							(*itTMON).second->GetPositionX(),
							(*itTMON).second->GetPositionZ()),
							m_fTSCALE);
					}
				}
		}
	}

	for( i=0; i<TDRAWCELL_COUNT; i++)
	{
		if(!m_vLOCK[i])
			continue;

		for( j=0; j<m_vTNPC[i].m_wCount; j++)
		{
			CTClientObjBase *pNPC = m_vTNPC[i].m_vTOBJ[j];

			if( !(pNPC->m_bDrawMark & (1 << TDRAWMARK_QUEST)) || !pNPC->CheckRequired() )
				continue;

			LPTEXTURESET pTPOS = NULL;
			switch(pNPC->m_bHasQuest)
			{
			case THASQUEST_MISSION		: pTPOS = m_pTQUESTRECV_O; break;
			case THASQUEST_COMPLETE		: pTPOS = m_pTQUESTCOMP_O; break;
			case THASQUEST_MISSION_X	: pTPOS = m_pTQUESTRECV_X; break;
			case THASQUEST_COMPLETE_X	: pTPOS = m_pTQUESTCOMP_X; break;
			}

			if(!pTPOS)
				continue;

			ResetTMINIMAPOBJ(
				pDevice,
				wMAPID,
				NULL,
				pTPOS ? pTPOS->GetTexture() : NULL,
				&D3DXVECTOR2(
				pHost->GetPositionX(),
				pHost->GetPositionZ()),
				&D3DXVECTOR2(
				pNPC->GetPositionX(),
				pNPC->GetPositionZ()),
				m_fTSCALE);
		}
	}

	VTSQUAD& vCORPS = m_pTRSCS->m_vTCORPS.m_vTSQUAD;
	for( i=0; i<WORD(vCORPS.size()); ++i )
	{
		for( j=0; j<WORD(vCORPS[i]->m_vTUNIT.size()); ++j )
		{
			LPTUNIT pTUNIT = vCORPS[i]->m_vTUNIT[j];
			if( pTUNIT->m_dwCharID == pHost->m_dwID )
				continue;

			D3DXVECTOR2 vTPOS = m_pTRSCS->GetUnitPOS(pTUNIT);
			ResetTMINIMAPOBJ(
				pDevice,
				wMAPID,
				m_pTPARTYDIR ? m_pTPARTYDIR->GetTexture() : NULL,
				m_pTPARTY ? m_pTPARTY->GetTexture() : NULL,
				&D3DXVECTOR2(
				pHost->GetPositionX(),
				pHost->GetPositionZ()),
				&vTPOS,
				m_fTSCALE);
		}
	}

	CTClientGame* pGame = CTClientGame::GetInstance();

	MAPSIEGEGODTOWER::iterator itor_tower = SiegeMgr->m_mapTowers.begin();
	while( itor_tower != SiegeMgr->m_mapTowers.end() )
	{
		BYTE bCAMP = (*itor_tower).second->GetCamp();

		ResetTMINIMAPSIEGE(
			pDevice,
			wMAPID,
			m_pTGodTower[bCAMP] ? m_pTGodTower[bCAMP]->GetTexture() : NULL,
			&D3DXVECTOR2(
			pHost->GetPositionX(),
			pHost->GetPositionZ() ),
			&D3DXVECTOR2(
			(*itor_tower).second->GetPositionX(),
			(*itor_tower).second->GetPositionZ() ),
			m_fTSCALE );

		++itor_tower;
	}

	MAPSIEGEGODBALL::iterator itor_ball = SiegeMgr->m_mapBalls.begin();
	while( itor_ball != SiegeMgr->m_mapBalls.end() )
	{
		CTClientChar* pChar = pGame->GetMainChar();
		LPTEXTURESET pTexBall = m_pTSiegeGodBall;

		if( (*itor_ball).second->GetCamp() == pChar->GetCamp() &&
			(*itor_ball).second->IsVisible() )
		{
			D3DXVECTOR3 vPosBall = (*itor_ball).second->GetPosition();

			ResetTMINIMAPSIEGE(
				pDevice,
				wMAPID,
				pTexBall ? pTexBall->GetTexture() : NULL,
				&D3DXVECTOR2(
				pHost->GetPositionX(),
				pHost->GetPositionZ() ),
				&D3DXVECTOR2(
				vPosBall.x,
				vPosBall.z ),
				m_fTSCALE );
		}

		++itor_ball;
	}

	D3DXMATRIX vSCALE;
	D3DXMATRIX vROT;
	D3DXMATRIX vPOS;

	D3DXMatrixScaling(
		&vSCALE,
		TPOS_SCALE,
		TPOS_SCALE,
		TPOS_SCALE);

	CTClientPet* pPet = pHost->GetRidingPet();
	FLOAT fDIR = FLOAT(pPet? pPet->m_wDIR: pHost->m_wDIR);

	D3DXMatrixRotationY(
		&vROT,
		D3DX_PI + fDIR * D3DX_PI / 900.0f);

	D3DXMatrixTranslation(
		&vPOS,
		-0.5f,
		0.0f,
		0.5f);
	vPOS *= vSCALE * vROT;

	pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vPOS);
	pDevice->m_pDevice->SetTexture( 0, m_pTPOS ? m_pTPOS->GetTexture() : NULL);

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

BYTE CTClientMAP::ResetTMINIMAP( CD3DDevice *pDevice,
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

		MAPTMINIMAP::iterator finder = CTChart::m_mapTMINIMAP.find(MAKELONG( MAKEWORD(
			nUnitX,
			nUnitZ), wMapID));

		if( finder != CTChart::m_mapTMINIMAP.end() && (*finder).second->m_pTEX )
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
			pDevice->m_pDevice->SetTexture( 0, (*finder).second->m_pTEX ? (*finder).second->m_pTEX->GetTexture() : NULL);

			pDevice->m_pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				0, 2);
			bResult = TRUE;
		}
	}

	return bResult;
}

void CTClientMAP::ResetTMINIMAPOBJ( CD3DDevice *pDevice,
								    WORD wMapID,
								    LPDIRECT3DTEXTURE9 pTDIRTEX,
									LPDIRECT3DTEXTURE9 pTPOSTEX,
									LPD3DXVECTOR2 pTCENTER,
									LPD3DXVECTOR2 pTPOS,
									FLOAT fSCALE)
{
	FLOAT fLength = ((FLOAT) m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);
	
	INT nUnitX = INT(pTCENTER->x / fLength);
	INT nUnitZ = INT(pTCENTER->y / fLength);

	MAPTMINIMAP::iterator finder = CTChart::m_mapTMINIMAP.find(MAKELONG( MAKEWORD(
		nUnitX,
		nUnitZ), wMapID));

	if( finder == CTChart::m_mapTMINIMAP.end() )
		return;

	FLOAT fRATIO = fSCALE * (*finder).second->m_fSCALE;
	D3DXVECTOR2 vTPOS = fRATIO / fLength * ((*pTPOS) - (*pTCENTER));
	FLOAT fDist = D3DXVec2Length(&vTPOS);

	D3DXMATRIX vSCALE;
	D3DXMATRIX vROT;
	D3DXMATRIX vPOS;

	D3DXMatrixScaling(
		&vSCALE,
		TPOS_SCALE,
		TPOS_SCALE,
		TPOS_SCALE);

	D3DXMatrixTranslation(
		&vPOS,
		-0.5f,
		0.0f,
		0.5f);

	vPOS *= vSCALE;

	// 훗날 적용함.
	//vPOS._11 = TMINIMAPOBJTEX_SIZE / TMINIMAPTEX_SIZE;
	//vPOS._22 = 1.0f;
	//vPOS._33 = TMINIMAPOBJTEX_SIZE / TMINIMAPTEX_SIZE;

	if( fDist > TMINIMAPMARK_DIST )
	{
		if( !pTDIRTEX )
			return;

		pDevice->m_pDevice->SetTexture( 0, pTDIRTEX);

		D3DXMatrixRotationY(
			&vROT,
			atan2f( vTPOS.x, vTPOS.y));

		vTPOS *= TMINIMAPMARK_DIST / fDist;
		vPOS *= vROT;

		vPOS._41 += vTPOS.x;
		vPOS._43 += vTPOS.y;
	}
	else
	{
		pDevice->m_pDevice->SetTexture( 0, pTPOSTEX);

		vPOS._41 += vTPOS.x;
		vPOS._43 += vTPOS.y;
	}

	pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vPOS);
	pDevice->m_pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0, 2);
}

void CTClientMAP::ResetTMINIMAPSIEGE( CD3DDevice *pDevice,
								    WORD wMapID,
									LPDIRECT3DTEXTURE9 pTTEX,
									LPD3DXVECTOR2 pTCENTER,
									LPD3DXVECTOR2 pTPOS,
									FLOAT fSCALE)
{
	FLOAT fLength = ((FLOAT) m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);
	
	INT nUnitX = INT(pTCENTER->x / fLength);
	INT nUnitZ = INT(pTCENTER->y / fLength);

	MAPTMINIMAP::iterator finder = CTChart::m_mapTMINIMAP.find(MAKELONG( MAKEWORD(
		nUnitX,
		nUnitZ), wMapID));

	if( finder == CTChart::m_mapTMINIMAP.end() )
		return;

	FLOAT fRATIO = fSCALE * (*finder).second->m_fSCALE;
	D3DXVECTOR2 vTPOS = fRATIO / fLength * ((*pTPOS) - (*pTCENTER));
	FLOAT fDist = D3DXVec2Length(&vTPOS);

	D3DXMATRIX vSCALE;
	D3DXMATRIX vROT;
	D3DXMATRIX vPOS;

	D3DXMatrixScaling(
		&vSCALE,
		TPOS_SCALE,
		TPOS_SCALE,
		TPOS_SCALE);

	D3DXMatrixTranslation(
		&vPOS,
		-0.5f,
		0.0f,
		0.5f);
	vPOS *= vSCALE;

	{
		pDevice->m_pDevice->SetTexture( 0, pTTEX);

		vPOS._41 += vTPOS.x;
		vPOS._43 += vTPOS.y;
	}

	pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vPOS);
	pDevice->m_pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0, 2);
}

void CTClientMAP::Render( CD3DDevice *pDevice)
{
	pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, m_dwTMapCOLOR);
	m_pMAP->m_bDrawShadow = CTClientGame::GetMapSHADOWOption();
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

void CTClientMAP::ResetTMAP( CD3DDevice *pDevice,
							 CTClientCAM *pCamera,
							 DWORD dwTick,
							 BYTE bLoadNOW)
{
	BYTE bUPDATE = FALSE;
	BYTE bInit = FALSE;

	for( BYTE i=0; i<9; i++)
		if(m_pMAP->m_bUPDATE[i])
		{
			bInit = TRUE;
			break;
		}

	if( bInit || IsEXTVisible(pCamera) )
	{
		D3DXVECTOR3 vDIR = pCamera->m_vTarget - pCamera->m_vPosition;
		D3DXVECTOR3 vPOS = pCamera->m_vPosition;
		vDIR.y = 0.0f;

		vDIR *= (pCamera->m_fZoomMAX + TCAMFIX_DIST + CTachyonHUGEMAP::m_nMaxLength) / D3DXVec3Length(&vDIR);
		pCamera->m_vPosition = pCamera->m_vTarget - vDIR;

		bUPDATE = ResetVB(
			pDevice,
			pCamera,
			TRUE,
			FALSE);
		pCamera->m_vPosition = vPOS;
	}

	LoadMAPOBJ( pDevice, bLoadNOW || HasEXTCell(&pCamera->m_vTarget));
	CheckMAPOBJ(
		pDevice,
		&pCamera->m_vTarget,
		bUPDATE);
	ResetVisibleOBJ( pDevice, pCamera, dwTick);
}

BYTE CTClientMAP::ResetVB( CD3DDevice *pDevice,
						   CTClientCAM *pCamera,
						   BYTE bCullView,
						   BYTE bTargetLOD)
{
	IPOINT vCENTER(
		INT((bTargetLOD ? pCamera->m_vTarget.x : pCamera->m_vPosition.x) / m_pMAP->m_nTileLength),
		INT((bTargetLOD ? pCamera->m_vTarget.z : pCamera->m_vPosition.z) / m_pMAP->m_nTileLength));

	IPOINT vPIVOT(
		INT(pCamera->m_vTarget.x / m_pMAP->m_nTileLength),
		INT(pCamera->m_vTarget.z / m_pMAP->m_nTileLength));

	FLOAT fRANGE = 0.0f;
	FLOAT fDIFF = 0.0f;
	FLOAT fDIR = 0.0f;
	BYTE bRESULT = FALSE;

	MAPDWORD mapTILE;
	mapTILE.clear();

	if(bCullView)
	{
		D3DXVECTOR3 vDIR = pCamera->m_vTarget - pCamera->m_vPosition;

		if( !pCamera->IsOrthoCamera() && (vDIR.x != 0.0f || vDIR.z != 0.0f) )
		{
			fRANGE = pCamera->m_fFOV * pCamera->m_fWidth / pCamera->m_fHeight + D3DX_PI / 6.0f;
			fDIR = atan2f( vDIR.x, vDIR.z) - fRANGE / 2.0f;

			if( fDIR < -D3DX_PI )
				fDIR += 2.0f * D3DX_PI;

			fDIFF = fabs(m_pMAP->m_fDIR - fDIR);
			if( fDIFF > D3DX_PI )
				fDIFF = 2.0f * D3DX_PI - fDIFF;
		}
		else
			bCullView = FALSE;
	}

	for( int i=0; i<9; i++)
		m_pMAP->m_bUPDATE[i] = FALSE;

	if( abs(m_pMAP->m_vPIVOT.m_nX - vPIVOT.m_nX) < 3 &&
		abs(m_pMAP->m_vPIVOT.m_nY - vPIVOT.m_nY) < 3 &&
		fDIFF < D3DX_PI / 6.0f && !m_pMAP->m_bResetVB )
		return bRESULT;

	if( pDevice->m_vCAPS.MaxVertexIndex <= 0x0000FFFF )
	{
		int nMaxUnit = CTachyonHUGEMAP::m_nMaxLength / 2;

		vCENTER.m_nX /= nMaxUnit;
		vCENTER.m_nY /= nMaxUnit;

		vCENTER.m_nX *= nMaxUnit;
		vCENTER.m_nY *= nMaxUnit;
	}

	m_pMAP->m_vCENTER.m_dwID = vCENTER.m_dwID;
	m_pMAP->m_vPIVOT.m_dwID = vPIVOT.m_dwID;
	m_pMAP->m_fDIR = fDIR;
	m_pMAP->m_bResetVB = FALSE;

	bRESULT = CheckVB( pDevice->m_pDevice, &m_pMAP->m_vPIVOT);
	m_pMAP->BuildBLOCKIDX(
		&mapTILE,
		&vCENTER,
		fRANGE,
		fDIR,
		bCullView);

	if( pDevice->m_vCAPS.MaxVertexIndex > 0x0000FFFF )
		m_pMAP->ResetVB32( pDevice, &mapTILE, &vCENTER);
	else
		m_pMAP->ResetVB16( pDevice, &mapTILE, &vCENTER);

	m_pMAP->ResetTEX( pDevice->m_pDevice, &vCENTER);
	pCamera->Activate(TRUE);
	mapTILE.clear();

	return bRESULT;
}

BYTE CTClientMAP::CheckVB( LPDIRECT3DDEVICE9 pDevice,
						   LPIPOINT pPIVOT)
{
	int nCellX = pPIVOT->m_nX * m_pMAP->m_nCellCount / m_pMAP->m_nUnitLength - (pPIVOT->m_nX < 0 ? 2 : 1);
	int nCellZ = pPIVOT->m_nY * m_pMAP->m_nCellCount / m_pMAP->m_nUnitLength - (pPIVOT->m_nY < 0 ? 2 : 1);
	int nUnitX = pPIVOT->m_nX / m_pMAP->m_nUnitLength - (pPIVOT->m_nX < 0 ? 2 : 1);
	int nUnitZ = pPIVOT->m_nY / m_pMAP->m_nUnitLength - (pPIVOT->m_nY < 0 ? 2 : 1);

	LPTMAPJOINT pTJOINT = GetTLoadJOINT(
		INT(pPIVOT->m_nX * m_pMAP->m_nTileLength),
		INT(pPIVOT->m_nY * m_pMAP->m_nTileLength));
	BYTE bRESULT = m_pTLOADJOINT == pTJOINT ? FALSE : TRUE;

	if( nUnitX != m_pMAP->m_nUnitX ||
		nUnitZ != m_pMAP->m_nUnitZ )
	{
		BYTE bUNIT[9];

		for( int i=0; i<9; i++)
		{
			bUNIT[i] = m_pMAP->m_bUNIT[i];
			m_pMAP->m_bLOAD[i] = FALSE;
		}

		for( i=0; i<9; i++)
		{
			int nLocalX = nUnitX - m_pMAP->m_nUnitX + i % 3;
			int nLocalZ = nUnitZ - m_pMAP->m_nUnitZ + i / 3;

			if( nLocalX < 0 || nLocalX > 2 ||
				nLocalZ < 0 || nLocalZ > 2 )
			{
				m_pMAP->m_pUNIT[i] = NULL;
				m_pMAP->m_bUNIT[i] = 0xFF;
			}
			else
			{
				int nIndex = nLocalZ * 3 + nLocalX;

				if( bUNIT[nIndex] != 0xFF )
				{
					m_pMAP->m_bLOAD[bUNIT[nIndex]] = TRUE;

					m_pMAP->m_pUNIT[i] = &m_pMAP->m_UNIT[bUNIT[nIndex]];
					m_pMAP->m_bUNIT[i] = bUNIT[nIndex];
				}
				else
				{
					m_pMAP->m_pUNIT[i] = NULL;
					m_pMAP->m_bUNIT[i] = 0xFF;
				}
			}
		}

		m_pMAP->m_nUnitX = nUnitX;
		m_pMAP->m_nUnitZ = nUnitZ;
	}

	if( nCellX != m_pMAP->m_nCellX ||
		nCellZ != m_pMAP->m_nCellZ ||
		bRESULT )
	{
		BYTE bResetUV = FALSE;
		m_pTLOADJOINT = pTJOINT;

		if( nCellX != m_pMAP->m_nCellX ||
			nCellZ != m_pMAP->m_nCellZ )
		{
			BYTE bINDEX[9];

			for( int i=0; i<9; i++)
			{
				bINDEX[i] = m_pMAP->m_bINDEX[i];
				m_pMAP->m_bLOCK[i] = FALSE;
			}

			for( i=0; i<9; i++)
			{
				int nLocalX = nCellX - m_pMAP->m_nCellX + i % 3;
				int nLocalZ = nCellZ - m_pMAP->m_nCellZ + i / 3;

				if( nLocalX < 0 || nLocalX > 2 ||
					nLocalZ < 0 || nLocalZ > 2 )
					m_pMAP->m_bINDEX[i] = 0xFF;
				else
				{
					int nIndex = nLocalZ * 3 + nLocalX;

					if( bINDEX[nIndex] != 0xFF )
					{
						m_pMAP->m_bLOCK[bINDEX[nIndex]] = TRUE;
						m_pMAP->m_bINDEX[i] = bINDEX[nIndex];
					}
					else
						m_pMAP->m_bINDEX[i] = 0xFF;
				}
			}

			for( i=0; i<9; i++)
				if(!m_pMAP->m_bLOCK[i])
					m_pMAP->FreeDIFFUSE(i);

			m_pMAP->m_nCellX = nCellX;
			m_pMAP->m_nCellZ = nCellZ;
			bResetUV = TRUE;
		}

		for( int i=0; i<9; i++)
		{
			nCellX = m_pMAP->m_nCellX + i % 3;
			nCellZ = m_pMAP->m_nCellZ + i / 3;

			nUnitX = nCellX / m_pMAP->m_nCellCount - (nCellX < 0 ? 1 : 0);
			nUnitZ = nCellZ / m_pMAP->m_nCellCount - (nCellZ < 0 ? 1 : 0);

			if( nUnitX >= 0 && nUnitX < m_pMAP->m_nUnitCountX &&
				nUnitZ >= 0 && nUnitZ < m_pMAP->m_nUnitCountZ )
			{
				int nIndex = (nUnitZ - m_pMAP->m_nUnitZ) * 3 + nUnitX - m_pMAP->m_nUnitX;

				if( m_pMAP->m_bINDEX[i] == 0xFF )
				{
					int nUnitID = nUnitZ * m_pMAP->m_nUnitCountX + nUnitX;

					if( m_pMAP->m_pENABLE[nUnitID] && IsValidLoadUNIT( m_pTLOADJOINT, nIndex) )
					{
						if( m_pMAP->m_bUNIT[nIndex] == 0xFF )
						{
							m_pMAP->m_bUNIT[nIndex] = m_pMAP->FindFreeUNIT();

							m_pMAP->m_pUNIT[nIndex] = &m_pMAP->m_UNIT[m_pMAP->m_bUNIT[nIndex]];
							m_pMAP->m_bLOAD[m_pMAP->m_bUNIT[nIndex]] = TRUE;

							m_pMAP->LoadUNIT(
								pDevice,
								m_pMAP->m_pUNIT[nIndex],
								nUnitID);
							m_pMAP->m_bUPDATE[nIndex] = TRUE;
						}

						m_pMAP->m_bINDEX[i] = m_pMAP->FindFreeCELL();
						m_pMAP->m_bLOCK[m_pMAP->m_bINDEX[i]] = TRUE;

						m_pMAP->LoadVB(
							m_pMAP->m_pUNIT[nIndex], i,
							nUnitX,
							nUnitZ,
							nCellX,
							nCellZ);

						m_pMAP->BuildDIFFUSE(
							pDevice, i,
							nCellX,
							nCellZ);
						bResetUV = TRUE;
					}
				}
				else if( m_pMAP->m_bUNIT[nIndex] != 0xFF && !IsValidLoadUNIT( m_pTLOADJOINT, nIndex) )
				{
					m_pMAP->FreeDIFFUSE(m_pMAP->m_bINDEX[i]);

					m_pMAP->m_bLOCK[m_pMAP->m_bINDEX[i]] = FALSE;
					m_pMAP->m_bINDEX[i] = 0xFF;
					bResetUV = TRUE;
				}
			}
		}

		for( i=0; i<9; i++)
			if( m_pMAP->m_bUNIT[i] != 0xFF && !IsValidLoadUNIT( m_pTLOADJOINT, i) )
			{
				for( WORD j=0; j<TOBJCELL_COUNT; j++)
				{
					ClearTNPC(&m_vTMAPNPC[m_pMAP->m_bUNIT[i]][j]);
					m_mapTMAPOBJ[m_pMAP->m_bUNIT[i]][j].clear();
					m_vTMAPSFX[m_pMAP->m_bUNIT[i]][j].clear();
					m_vTMAPSND[m_pMAP->m_bUNIT[i]][j].clear();
				}

				ClearTSILHOUETTE(m_pMAP->m_bUNIT[i]);
				ClearBIGOBJ(m_pMAP->m_bUNIT[i]);

				ClearTHEIGHTINFO(m_pMAP->m_bUNIT[i]);
				ClearTFLAG(MAKEWORD(
					BYTE(m_pMAP->m_nUnitX + i % 3),
					BYTE(m_pMAP->m_nUnitZ + i / 3)));
				ClearTBSP(m_pMAP->m_bUNIT[i]);

				m_pMAP->ReleaseUNIT(m_pMAP->m_pUNIT[i]);
				m_pMAP->m_pUNIT[i] = NULL;

				m_pMAP->m_bLOAD[m_pMAP->m_bUNIT[i]] = FALSE;
				m_pMAP->m_bUNIT[i] = 0xFF;
				m_pMAP->m_bUPDATE[i] = FALSE;
				bResetUV = TRUE;
			}

		if(bResetUV)
			m_pMAP->ResetUV();
	}

	return bRESULT;
}

BYTE CTClientMAP::IsInValidUNIT( CTClientObjBase *pTOBJ)
{
	if( !m_pMAP || !pTOBJ )
		return FALSE;

	FLOAT fPosX = pTOBJ->GetPositionX();
	FLOAT fPosZ = pTOBJ->GetPositionZ();

	int nUnitLength = m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength;
	int nUnitX = INT(fPosX) / nUnitLength - (fPosX < 0.0f ? 1 : 0);
	int nUnitZ = INT(fPosZ) / nUnitLength - (fPosZ < 0.0f ? 1 : 0);

	nUnitX -= m_pMAP->m_nUnitX;
	nUnitZ -= m_pMAP->m_nUnitZ;

	if( nUnitX < 0 || nUnitX > 2 ||
		nUnitZ < 0 || nUnitZ > 2 )
		return FALSE;
	int nIndex = nUnitZ * 3 + nUnitX;

	return m_pMAP->m_pUNIT[nIndex] && IsValidLoadUNIT( m_pTLOADJOINT, nIndex) ? TRUE : FALSE;
}

BYTE CTClientMAP::IsValidLoadUNIT( LPTMAPJOINT pTJOINT, int nIndex)
{
	return IsValidUNIT( pTJOINT ? pTJOINT->m_bJOINT : 0, nIndex);
}

BYTE CTClientMAP::IsValidUNIT( BYTE bCODE,
							   int nIndex)
{
	if( nIndex > 4 )
		nIndex--;
	else if( nIndex == 4 )
		return TRUE;

	return BYTE(1 << nIndex) & bCODE ? TRUE : FALSE;
}

LPTMAPJOINT CTClientMAP::GetTLoadJOINT( int nPosX,
									    int nPosZ)
{
	if(!m_pMAP)
		return NULL;

	int nUnitLength = m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength;
	LPVTMAPJOINT pVTMAPJOINT = CTChart::GetTMAPJOINT(
		m_wMapID,
		nPosX / nUnitLength,
		nPosZ / nUnitLength);

	if(pVTMAPJOINT)
	{
		CPoint point(
			nPosX % nUnitLength,
			nPosZ % nUnitLength);

		for( int i=0; i<INT(pVTMAPJOINT->size()); i++)
			if( (*pVTMAPJOINT)[i]->m_bJOINT && (*pVTMAPJOINT)[i]->m_vRECT.PtInRect(point) )
				return (*pVTMAPJOINT)[i];
	}

	return NULL;
}

LPTMAPJOINT CTClientMAP::GetTMoveJOINT( int nPosX,
									    int nPosZ)
{
	if(!m_pMAP)
		return NULL;

	int nUnitLength = m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength;
	LPVTMAPJOINT pVTMAPJOINT = CTChart::GetTMAPJOINT(
		m_wMapID,
		nPosX / nUnitLength,
		nPosZ / nUnitLength);

	if(pVTMAPJOINT)
	{
		CPoint point(
			nPosX % nUnitLength,
			nPosZ % nUnitLength);

		for( int i=0; i<INT(pVTMAPJOINT->size()); i++)
			if( !(*pVTMAPJOINT)[i]->m_bJOINT && (*pVTMAPJOINT)[i]->m_vRECT.PtInRect(point) )
				return (*pVTMAPJOINT)[i];
	}

	return NULL;
}

void CTClientMAP::ResetVisibleOBJ( CD3DDevice *pDevice,
								   CTClientCAM *pCamera,
								   DWORD dwTick)
{
	MAPTGATE::iterator itTGATE;

	for( WORD i=0; i<9; i++)
		m_vDRAWOBJ[i].clear();

	m_mapTCOLLOBJ.clear();
	m_mapTWATER.clear();

	m_vTDRAWGATE.clear();
	m_vTCOLLNPC.clear();
	m_vDRAWBSP.clear();

	for( i=0; i<TDRAWCELL_COUNT; i++)
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

	for( i=0; i<TDRAWCELL_COUNT; i++)
		if(m_vLOCK[i])
		{
			for( WORD j=0; j<m_vTNPC[i].m_wCount; j++)
			{
				ResetVisibleOBJ(
					pDevice,
					m_vTNPC[i].m_vTOBJ[j],
					pCamera,
					dwTick);
			}
		}

	for( i=0; i<9; i++)
		if(m_vUNIT[i])
		{
			for( WORD j=0; j<m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_wCount; j++)
				if(m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ[j])
				{
					m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ[j]->ResetVisible(
						pCamera,
						m_dwHouseID,
						m_bHouseMesh,
						dwTick);
				}
		}

	for( i=0; i<9; i++)
		if(m_vUNIT[i])
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

	for( itTGATE = m_mapTGate.begin(); itTGATE != m_mapTGate.end(); itTGATE++)
		if( !(*itTGATE).second->m_bGARBAGE && (*itTGATE).second->IsActiveTGate() )
		{
			ResetVisibleOBJ(
				pDevice,
				(*itTGATE).second,
				pCamera,
				dwTick);
		}
}

void CTClientMAP::ResetVisibleOBJ( CD3DDevice *pDevice,
								   CTClientObjBase *pTOBJ,
								   CTClientCAM *pCamera,
								   DWORD dwTick)
{
	if( !pTOBJ || (pTOBJ->m_bType != OT_WATER && pTOBJ->m_bDETAILLevel > CTClientGame::GetMapDETAILOption()) )
	{
		if(pTOBJ)
			pTOBJ->CalcRender(pDevice);

		return;
	}

	pTOBJ->ResetVisible(
		pCamera,
		m_dwHouseID,
		m_bHouseMesh,
		dwTick);

	if( pTOBJ->m_bType != OT_WATER )
	{
		if( pTOBJ->m_bType == OT_BSP )
		{
			MAPTBSP::iterator finder = m_mapTBsp.find(pTOBJ->m_dwID);

			if( finder != m_mapTBsp.end() )
				ResetPVS( (*finder).second, pCamera);
		}

		if( pTOBJ->m_bType != OT_GATE && pTOBJ->m_bType != OT_NPC &&
			pTOBJ->m_dwInstID && (
			pTOBJ->m_bCollisionType != TCOLLISION_NONE || (
			pTOBJ->m_bType == OT_COLLISION &&
			pTOBJ->m_bCamCollision )))
			m_mapTCOLLOBJ.insert( MAPOBJECT::value_type( pTOBJ->m_dwInstID, pTOBJ));

		if(IsDrawOBJ( pTOBJ, pCamera))
		{
			FLOAT fUnitLength = FLOAT(m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);

			int nUnitX = INT(pTOBJ->GetPositionX() / fUnitLength - m_pMAP->m_nUnitX - (pTOBJ->GetPositionX() < 0.0f ? 1.0f : 0.0f));
			int nUnitZ = INT(pTOBJ->GetPositionZ() / fUnitLength - m_pMAP->m_nUnitZ - (pTOBJ->GetPositionZ() < 0.0f ? 1.0f : 0.0f));

			if( nUnitX >= 0 && nUnitX < 3 &&
				nUnitZ >= 0 && nUnitZ < 3 )
			{
				int nIndex = nUnitZ * 3 + nUnitX;

				if(pTOBJ->m_bVisible)
				{
					switch(pTOBJ->m_bType)
					{
					case OT_GATE:
						{
							m_vTDRAWGATE.push_back(pTOBJ);
						}
						break;

					case OT_NPC:
						{
							if( pTOBJ->m_bCollisionType != TCOLLISION_NONE )
								m_vTCOLLNPC.push_back(pTOBJ);
						}
						break;
					}

					m_vDRAWOBJ[nIndex].push_back(pTOBJ);
				}
				else if( pTOBJ->m_bType != OT_COLLISION && pTOBJ->m_bType != OT_PATHWALL )
					pTOBJ->CalcRender(pDevice);
			}
		}
	}
	else
		m_mapTWATER.insert( MAPOBJECT::value_type( pTOBJ->m_dwInstID, pTOBJ));
}

BYTE CTClientMAP::IsDrawOBJ( CTClientObjBase *pTOBJ,
							 CTClientCAM *pCamera)
{
	switch(pTOBJ->m_bType)
	{
	case OT_COLLISION	: return FALSE;
	default				:
		if( pTOBJ->m_fCamDIST - pTOBJ->m_fSight < CTClientObjBase::m_fCamDist )
			return TRUE;

		break;
	}

	return FALSE;
}

BYTE CTClientMAP::CheckCollision( CTClientObjBase *pOBJ,
								  LPD3DXVECTOR3 pRESULT,
								  LPD3DXVECTOR3 pPREV,
								  FLOAT fNextH)
{
	if(!m_pMAP)
		return FALSE;

	CTClientObjBase *pTSECOND = NULL;
	CTClientObjBase *pTFIRST = NULL;

	D3DXVECTOR3 vTEND = pOBJ->GetPosition();
	D3DXVECTOR3 vTSTART = (*pPREV);
	vTEND.y = fNextH;

	if(CheckCollision( &pTFIRST, pOBJ, &vTSTART, &vTEND, pOBJ->m_fSizeY))
	{
		D3DXVECTOR3 vTS = vTSTART;

		if(CheckCollision( &pTSECOND, pOBJ, &vTSTART, &vTEND, pOBJ->m_fSizeY))
			vTEND = pTFIRST == pTSECOND ? vTS : vTSTART;
		(*pRESULT) = vTEND;

		return TRUE;
	}

	return FALSE;
}

BYTE CTClientMAP::CheckCollision( CTClientObjBase **pTCOLL,
								  CTClientObjBase *pOBJ,
								  LPD3DXVECTOR3 pTSTART,
								  LPD3DXVECTOR3 pTEND,
								  FLOAT fSizeY)
{
	D3DXVECTOR3 vTSTART = (*pTSTART);
	D3DXVECTOR3 vTEND = (*pTEND);

	if( !m_pMAP || vTSTART == vTEND )
		return FALSE;
	(*pTCOLL) = NULL;

	D3DXVECTOR3 vTS = vTSTART;
	D3DXVECTOR3 vTE = vTEND;
	BYTE bRESULT = FALSE;

	FLOAT fLOCAL = 0.0f;
	FLOAT fDIST = 0.0f;

	if( pOBJ->m_bLand && !pOBJ->IsFlyOBJ() && CheckCollision( pOBJ, &vTS, &vTE, fSizeY, &fLOCAL) )
	{
		pTSTART->x = vTS.x;
		pTSTART->z = vTS.z;

		pTEND->x = vTE.x;
		pTEND->z = vTE.z;

		fDIST = fLOCAL;
		bRESULT = TRUE;
	}

	vTS = vTSTART;
	vTE = vTEND;

	if( CheckJoint( &vTS, &vTE, &fLOCAL) && (!bRESULT || fLOCAL < fDIST) )
	{
		pTSTART->x = vTS.x;
		pTSTART->z = vTS.z;

		pTEND->x = vTE.x;
		pTEND->z = vTE.z;

		fDIST = fLOCAL;
		bRESULT = TRUE;
	}

	VTOBJBASE vTCOLLISION;
	vTCOLLISION.clear();

	GetLineCollisionOBJ(
		&vTCOLLISION,
		&vTSTART,
		&vTEND,
		2.0f * pOBJ->m_fRadius);

	vTSTART.y += TCOLL_EPSILON;
	vTEND.y += TCOLL_EPSILON;

	for( int i=0; i<INT(vTCOLLISION.size()); i++)
	{
		vTS = vTSTART;
		vTE = vTEND;

		if( !vTCOLLISION[i]->m_bGARBAGE && (
			vTCOLLISION[i]->m_bType != OT_GATE ||
			!pOBJ->m_bGhost) &&
			pOBJ->CheckCollision(
			vTCOLLISION[i],
			&vTS,
			&vTE,
			fSizeY,
			&fLOCAL) && (
			!bRESULT ||
			fLOCAL < fDIST) )
		{
			pTSTART->x = vTS.x;
			pTSTART->z = vTS.z;

			pTEND->x = vTE.x;
			pTEND->z = vTE.z;

			(*pTCOLL) = vTCOLLISION[i];
			fDIST = fLOCAL;
			bRESULT = TRUE;
		}
	}

	return bRESULT;
}

BYTE CTClientMAP::CheckCollision( CTClientObjBase *pOBJ,
								  LPD3DXVECTOR3 pTSTART,
								  LPD3DXVECTOR3 pTEND,
								  FLOAT fSizeY,
								  FLOAT *pDIST)
{
	FLOAT fWIDTH = TCOLL_EPSILON + FLOAT(m_pMAP->m_nTileLength);
	FLOAT fPosY = min( pTSTART->y, pTEND->y);

	FLOAT fMaxX = max( pTSTART->x, pTEND->x) + fWIDTH;
	FLOAT fMinX = min( pTSTART->x, pTEND->x) - fWIDTH;

	FLOAT fMaxZ = max( pTSTART->z, pTEND->z) + fWIDTH;
	FLOAT fMinZ = min( pTSTART->z, pTEND->z) - fWIDTH;

	D3DXVECTOR2 vO( pTSTART->x, pTSTART->z);
	D3DXVECTOR2 vA( pTEND->x, pTEND->z);
	D3DXVECTOR2 vP;
	D3DXVECTOR2 vQ;

	D3DXVECTOR3 vTSTART = (*pTSTART);
	D3DXVECTOR3 vTEND = (*pTEND);
	BYTE bRESULT = FALSE;

	if( !m_pMAP || vA == vO )
		return FALSE;
	int nSIZE = m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength;

	fWIDTH *= fWIDTH;
	vA -= vO;

	for( FLOAT fPosX = fMinX; fPosX < fMaxX; fPosX += FLOAT(m_pMAP->m_nTileLength))
	{
		vP.x = FLOAT(INT(fPosX / m_pMAP->m_nTileLength) * m_pMAP->m_nTileLength) + FLOAT(m_pMAP->m_nTileLength) / 2.0f - vO.x;

		for( FLOAT fPosZ = fMinZ; fPosZ < fMaxZ; fPosZ += FLOAT(m_pMAP->m_nTileLength))
		{
			vP.y = FLOAT(INT(fPosZ / m_pMAP->m_nTileLength) * m_pMAP->m_nTileLength) + FLOAT(m_pMAP->m_nTileLength) / 2.0f - vO.y;
			vQ = vP - D3DXVec2Dot( &vA, &vP) / D3DXVec2Dot( &vA, &vA) * vA;

			if( GetRegionID( fPosX, fPosZ) != BLANK_REGION && D3DXVec2LengthSq(&vQ) < fWIDTH )
			{
				int nUnitX = INT(vO.x + vP.x) / nSIZE - m_pMAP->m_nUnitX;
				int nUnitZ = INT(vO.y + vP.y) / nSIZE - m_pMAP->m_nUnitZ;

				if( nUnitX >= 0 && nUnitX < 3 &&
					nUnitZ >= 0 && nUnitZ < 3 )
				{
					BYTE bINDEX = BYTE(nUnitZ * 3 + nUnitX);

					if( m_pMAP->m_pUNIT[bINDEX] && m_pMAP->m_pUNIT[bINDEX]->m_pDATA )
					{
						FLOAT *pDATA = m_pMAP->m_pUNIT[bINDEX]->m_pDATA;

						FLOAT fLEFT = FLOAT(INT((vO.x + vP.x) / m_pMAP->m_nTileLength) * m_pMAP->m_nTileLength);
						FLOAT fTOP = FLOAT(INT((vO.y + vP.y) / m_pMAP->m_nTileLength) * m_pMAP->m_nTileLength);
						FLOAT fRIGHT = fLEFT + m_pMAP->m_nTileLength;
						FLOAT fBOTTOM = fTOP + m_pMAP->m_nTileLength;

						int nPosX = INT(vO.x + vP.x) % nSIZE;
						int nPosZ = INT(vO.y + vP.y) % nSIZE;

						nPosX /= m_pMAP->m_nTileLength;
						nPosZ /= m_pMAP->m_nTileLength;

						D3DXVECTOR3 vTPOLY[2][3] = {{
							D3DXVECTOR3( fLEFT, pDATA[(nPosZ + 1) * (m_pMAP->m_nUnitLength + 1) + nPosX], fBOTTOM),
							D3DXVECTOR3( fRIGHT, pDATA[nPosZ * (m_pMAP->m_nUnitLength + 1) + nPosX + 1], fTOP),
							D3DXVECTOR3( fLEFT, pDATA[nPosZ * (m_pMAP->m_nUnitLength + 1) + nPosX], fTOP)}, {
							D3DXVECTOR3( fRIGHT, pDATA[(nPosZ + 1) * (m_pMAP->m_nUnitLength + 1) + nPosX + 1], fBOTTOM),
							D3DXVECTOR3( fRIGHT, pDATA[nPosZ * (m_pMAP->m_nUnitLength + 1) + nPosX + 1], fTOP),
							D3DXVECTOR3( fLEFT, pDATA[(nPosZ + 1) * (m_pMAP->m_nUnitLength + 1) + nPosX], fBOTTOM)}};
						FLOAT fHEIGHT = fPosY;

						for( int i=0; i<2; i++)
							for( int j=0; j<3; j++)
								if( fHEIGHT < vTPOLY[i][j].y )
									fHEIGHT = vTPOLY[i][j].y;

						fHEIGHT += fSizeY;
						fHEIGHT -= fPosY;

						for( i=0; i<2; i++)
						{
							D3DXVECTOR3 vTS = vTSTART;
							D3DXVECTOR3 vTE = vTEND;
							FLOAT fLOCAL = 0.0f;

							if( pOBJ->CheckPoly( vTPOLY[i], &vTS, &vTE, TCOLL_EPSILON, fHEIGHT, 2.0f * m_fMoveBound, &fLOCAL) && (!bRESULT || fLOCAL < (*pDIST)) )
							{
								(*pTSTART) = vTS;
								(*pTEND) = vTE;
								(*pDIST) = fLOCAL;

								bRESULT = TRUE;
							}
						}
					}
				}
			}
		}
	}

	return bRESULT;
}

BYTE CTClientMAP::CheckJoint( LPD3DXVECTOR3 pTSTART,
							  LPD3DXVECTOR3 pTEND,
							  FLOAT *pDIST)
{
	if(!m_pMAP)
		return FALSE;

	int nSIZE = m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength;
	int nUnitX = INT(pTSTART->x) / nSIZE;
	int nUnitZ = INT(pTSTART->z) / nSIZE;

	CRect vRECT;
	CRect rect(
		0, 0,
		nSIZE,
		nSIZE);

	rect.OffsetRect(
		nUnitX * nSIZE,
		nUnitZ * nSIZE);

	rect.DeflateRect(
		TUNIT_BOUND,
		TUNIT_BOUND);
	vRECT = rect;

	if( rect.PtInRect(CPoint( INT(pTSTART->x), INT(pTSTART->z))) &&
		rect.PtInRect(CPoint( INT(pTEND->x), INT(pTEND->z))) )
		return FALSE;

	int nCountX = abs(INT(pTEND->x) - INT(pTSTART->x)) + 1;
	int nCountZ = abs(INT(pTEND->z) - INT(pTSTART->z)) + 1;

	int nMoveX = pTEND->x > pTSTART->x ? 1 : -1;
	int nMoveZ = pTEND->z > pTSTART->z ? 1 : -1;

	D3DXVECTOR2 vO( pTSTART->x, pTSTART->z);
	D3DXVECTOR2 vA( pTEND->x, pTEND->z);
	D3DXVECTOR3 vTLINE;

	if( !m_pMAP || vA == vO )
		return FALSE;
	vA -= vO;

	vTLINE.z = vA.x * vO.y - vA.y * vO.x;
	vTLINE.y = -vA.x;
	vTLINE.x = vA.y;

	for( int i=0; i<nCountX; i++)
	{
		int nPosX = INT(pTSTART->x) + i * nMoveX;

		for( int j=0; j<nCountZ; j++)
		{
			int nPosZ = INT(pTSTART->z) + j * nMoveZ;

			if( !vRECT.PtInRect(CPoint( nPosX, nPosZ)) &&
				!rect.PtInRect(CPoint( nPosX, nPosZ)) &&
				CheckLINE( &vTLINE, nPosX, nPosZ) )
			{
				LPTMAPJOINT pTJOINT = GetTMoveJOINT(
					nPosX,
					nPosZ);

				if(pTJOINT)
				{
					nUnitX = nPosX / nSIZE;
					nUnitZ = nPosZ / nSIZE;

					vRECT = pTJOINT->m_vRECT;
					vRECT.OffsetRect(
						nUnitX * nSIZE,
						nUnitZ * nSIZE);
				}
				else
					return CheckRECT( pTSTART, pTEND, vRECT, pDIST);
			}
		}
	}

	return FALSE;
}

BYTE CTClientMAP::CheckRECT( LPD3DXVECTOR3 pTSTART,
							 LPD3DXVECTOR3 pTEND,
							 CRect vTRECT,
							 FLOAT *pDIST)
{
	D3DXVECTOR3 vTSTART(
		pTSTART->x,
		0.0f,
		pTSTART->z);

	D3DXVECTOR3 vTEND(
		pTEND->x,
		0.0f,
		pTEND->z);

	D3DXVECTOR3 vTPOINT[4] = {
		D3DXVECTOR3( FLOAT(vTRECT.left), 0.0f, FLOAT(vTRECT.top)),
		D3DXVECTOR3( FLOAT(vTRECT.right), 0.0f, FLOAT(vTRECT.top)),
		D3DXVECTOR3( FLOAT(vTRECT.right), 0.0f, FLOAT(vTRECT.bottom)),
		D3DXVECTOR3( FLOAT(vTRECT.left), 0.0f, FLOAT(vTRECT.bottom))};

	D3DXVECTOR3 vTDIR = vTEND - vTSTART;
	BYTE bRESULT = FALSE;

	for( int i=0; i<4; i++)
	{
		D3DXPLANE vTPLANE;

		D3DXVECTOR3 vTS = vTSTART;
		D3DXVECTOR3 vTE = vTEND;
		FLOAT fLOCAL = 0.0f;

		D3DXPlaneFromPoints(
			&vTPLANE,
			&vTPOINT[i],
			&vTPOINT[(i + 1) % 4],
			&D3DXVECTOR3(
			vTPOINT[i].x,
			1.0f,
			vTPOINT[i].z));

		D3DXPlaneNormalize(
			&vTPLANE,
			&vTPLANE);

		if( D3DXPlaneDotNormal( &vTPLANE, &vTDIR) < 0.0f &&
			CTMath::CheckCollision( &vTPLANE, &vTS, &vTE, 0.0f, &fLOCAL) && (
			!bRESULT || fLOCAL < (*pDIST)) )
		{
			pTSTART->x = vTS.x;
			pTSTART->z = vTS.z;

			pTEND->x = vTE.x;
			pTEND->z = vTE.z;

			(*pDIST) = fLOCAL;
			bRESULT = TRUE;
		}
	}

	return bRESULT;
}

BYTE CTClientMAP::CheckLINE( LPD3DXVECTOR3 pTLINE,
							 int nPosX,
							 int nPosZ)
{
	D3DXVECTOR3 vTPOS( FLOAT(nPosX), FLOAT(nPosZ), 1.0f);
	D3DXVECTOR3 vTLINE = (*pTLINE);

	if( D3DXVec3Dot( &vTLINE, &vTPOS) < 0.0f )
		vTLINE = -vTLINE;

	for( int i=0; i<2; i++)
	{
		vTPOS.x = FLOAT(nPosX + i);

		for( int j=0; j<2; j++)
		{
			vTPOS.y = FLOAT(nPosZ + j);

			if( D3DXVec3Dot( &vTLINE, &vTPOS) < 0.0f )
				return TRUE;
		}
	}

	return FALSE;
}

void CTClientMAP::CalcMAPOBJ( CD3DDevice *pDevice,
							  CTClientCAM *pCamera,
							  DWORD dwTick)
{
	if(!m_pMAP)
		return;

	BYTE bSFX = CTClientGame::GetMapSFXOption();
	CTClientGame* pGame = CTClientGame::GetInstance();
	MAPTGATE::iterator itTGATE;

	for( WORD i=0; i<TDRAWCELL_COUNT; i++)
		if(m_vLOCK[i])
			for( WORD j=0; j<m_vTOBJ[i].m_wCount; j++)
			{
				m_vTOBJ[i].m_vTOBJ[j]->m_bSFX = bSFX;
				m_vTOBJ[i].m_vTOBJ[j]->CalcTick( pDevice->m_pDevice, dwTick);
			}

	if(pGame->m_bEnableNPCTick)
		for( i=0; i<TDRAWCELL_COUNT; i++)
			if(m_vLOCK[i])
				for( WORD j=0; j<m_vTNPC[i].m_wCount; j++)
				{
					CTClientNpc* pNPC = (CTClientNpc*)m_vTNPC[i].m_vTOBJ[j];
					pNPC->CalcTick( pDevice, m_pMAP->m_pRES, dwTick);
				}

	for( i=0; i<9; i++)
		if(m_vUNIT[i])
			for( WORD j=0; j<m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_wCount; j++)
			{
				m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ[j]->m_bSFX = bSFX;
				m_vTSILHOUETTE[m_pMAP->m_bUNIT[i]].m_vTOBJ[j]->CalcTick( pDevice->m_pDevice, dwTick);
			}

	for( i=0; i<9; i++)
		if(m_vUNIT[i])
			for( WORD j=0; j<m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_wCount; j++)
			{
				m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_vTOBJ[j]->m_bSFX = bSFX;
				m_vBIGOBJ[m_pMAP->m_bUNIT[i]].m_vTOBJ[j]->CalcTick( pDevice->m_pDevice, dwTick);
			}

	for( itTGATE = m_mapTGate.begin(); itTGATE != m_mapTGate.end(); itTGATE++)
		if( !(*itTGATE).second->m_bGARBAGE && (*itTGATE).second->IsActiveTGate() )
			(*itTGATE).second->CalcTick( pDevice->m_pDevice, dwTick);
}

void CTClientMAP::RenderWATER( CD3DDevice *pDevice, CTClientCAM *pCamera)
{
	MAPOBJECT::iterator itWATER;

	for( itWATER = m_mapTWATER.begin(); itWATER != m_mapTWATER.end(); itWATER++)
		if((*itWATER).second)
			(*itWATER).second->Render( pDevice, pCamera);
}

void CTClientMAP::ResetPVS( CTClientBSP *pTBSP,
						    CTClientCAM *pCamera)
{
	if( !pTBSP || !pCamera )
		return;
	DWORD dwNodeID = pTBSP->GetNodeID(pCamera->m_vTarget);

	if( m_dwHouseID && dwNodeID != NODEID_NULL && m_dwBspID != pTBSP->m_dwBspID )
	{
		MAPD3DXVECTOR3::iterator itTNODE;
		BYTE bENTER = FALSE;

		for( itTNODE = pTBSP->m_pINFO->m_mapTNODE.begin(); itTNODE != pTBSP->m_pINFO->m_mapTNODE.end(); itTNODE++)
			if( pTBSP->m_mapPVS.find((*itTNODE).first) != pTBSP->m_mapPVS.end() )
			{
				bENTER = TRUE;
				break;
			}

		if(bENTER)
			m_dwBspID = pTBSP->m_dwBspID;
	}
	else if( dwNodeID == NODEID_NULL && m_dwBspID == pTBSP->m_dwBspID )
		m_dwBspID = NODEID_NULL;

	if( m_dwBspID == NODEID_NULL || m_bHouseMesh || m_dwBspID == pTBSP->m_dwBspID || m_dwHouseID == pTBSP->m_dwHouseID )
	{
		pTBSP->m_fCamDIST = 0.0f;

		if( dwNodeID == NODEID_NULL || m_dwBspID != pTBSP->m_dwBspID )
		{
			MAPD3DXVECTOR3::iterator itTNODE;

			for( itTNODE = pTBSP->m_pINFO->m_mapTNODE.begin(); itTNODE != pTBSP->m_pINFO->m_mapTNODE.end(); itTNODE++)
			{
				FLOAT fLocal = D3DXVec3Length(&(pCamera->m_vPosition - (*(*itTNODE).second)));

				if( dwNodeID == NODEID_NULL || fLocal < pTBSP->m_fCamDIST )
				{
					pTBSP->m_vNodePOS = *(*itTNODE).second;
					dwNodeID = (*itTNODE).first;
					pTBSP->m_fCamDIST = fLocal;
				}
			}
		}

		if( pTBSP->m_fCamDIST - pTBSP->m_fRadius < CTClientObjBase::m_fCamDist )
		{
			m_vDRAWBSP.push_back(pTBSP);
			pTBSP->ResetPVS(dwNodeID);
		}
	}
}

BYTE CTClientMAP::CanMove( CTClientObjBase *pOBJ,
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
	{
		VTOBJBASE vTCOLLISION;

		vTCOLLISION.clear();
		GetLineCollisionOBJ(
			&vTCOLLISION,
			&vSTART,
			&vTARGET,
			pOBJ->m_fRadius);

		int nCount = INT(vTCOLLISION.size());
		for( int i=0; i<nCount; i++)
			if( !vTCOLLISION[i]->m_bGARBAGE &&
				vTCOLLISION[i]->HitTest(
				&vSTART,
				&vTARGET,
				pOBJ->m_fRadius,
				pOBJ->m_fSizeY) )
			{
				vTCOLLISION.clear();
				return FALSE;
			}
		vTCOLLISION.clear();
	}

	return TRUE;
}

CTClientFlag *CTClientMAP::FindPath( LPMAPTPATHFLAG pTPATH,
									 CTClientFlag *pTFLAG,
									 LPD3DXVECTOR3 pSTART,
									 LPD3DXVECTOR3 pTARGET)
{
	CTClientFlag *pTRESULT = NULL;
	FLOAT fDIST = 0.0f;

	for( int i=0; i<INT(pTFLAG->m_vPATH.size()); i++)
		if( pTPATH->find(pTFLAG->m_vPATH[i]) == pTPATH->end() )
		{
			CTClientFlag *pTNEXT = CTClientFlag::FindTFLAG(pTFLAG->m_vPATH[i]);

			if(pTNEXT)
			{
				FLOAT fLocal = D3DXVec2Length(&D3DXVECTOR2(
					pTNEXT->m_vPOS.x - pSTART->x,
					pTNEXT->m_vPOS.z - pSTART->z)) + D3DXVec2Length(&D3DXVECTOR2(
					pTNEXT->m_vPOS.x - pTARGET->x,
					pTNEXT->m_vPOS.z - pTARGET->z)) + D3DXVec2Length(&D3DXVECTOR2(
					pTNEXT->m_vPOS.x - pTFLAG->m_vPOS.x,
					pTNEXT->m_vPOS.z - pTFLAG->m_vPOS.z));

				if( !pTRESULT || fLocal < fDIST )
				{
					pTRESULT = pTNEXT;
					fDIST = fLocal;
				}
			}
		}

	return pTRESULT;
}

D3DXVECTOR3 CTClientMAP::GetTargetPoint( CTClientObjBase *pOBJ,
										 LPD3DXVECTOR3 pTARGET)
{
	D3DXVECTOR3 vSTART(
		pOBJ->GetPositionX(),
		pOBJ->GetPositionY() + TPATH_HEIGHT,
		pOBJ->GetPositionZ());

	CTClientFlag *pTFLAG = NULL;
	MAPTPATHFLAG mapTPATH;
	FLOAT fDIST = 0.0f;

	if(CanMove( pOBJ, &vSTART, pTARGET))
		return (*pTARGET);

	for( int i=0; i<INT(m_vTPATH.size()); i++)
	{
		FLOAT fLocal = D3DXVec3Length(&(m_vTPATH[i]->m_vPOS - (*pTARGET)));

		if( !pTFLAG || fLocal < fDIST )
		{
			pTFLAG = m_vTPATH[i];
			fDIST = fLocal;
		}
	}

	if(!pTFLAG)
		return (*pTARGET);

	CTClientFlag *pTSTART = pTFLAG;
	mapTPATH.clear();

	while( pTFLAG && !CanMove( pOBJ, &vSTART, &pTFLAG->m_vPOS) )
	{
		mapTPATH.insert( MAPTPATHFLAG::value_type( pTFLAG->m_dwID, pTFLAG));

		pTFLAG = FindPath(
			&mapTPATH,
			pTFLAG,
			&vSTART,
			pTARGET);

		if(!pTFLAG)
		{
			pTFLAG = FindPath(
				&mapTPATH,
				pTSTART,
				&vSTART,
				pTARGET);
		}
	}
	mapTPATH.clear();

	return pTFLAG ? pTFLAG->m_vPOS : (*pTARGET);
}

CTClientObjBase *CTClientMAP::FindTNPC( DWORD dwNpcID)
{
	MAPOBJECT::iterator finder = m_mapTNpc.find(dwNpcID);

	if( finder != m_mapTNpc.end() )
		return (CTClientObjBase *) (*finder).second;

	return NULL;
}

void CTClientMAP::ClearTNPC( LPVTNPC pTNPC)
{
	while(!pTNPC->empty())
	{
		delete pTNPC->back();
		pTNPC->pop_back();
	}
}

void CTClientMAP::ClearTFLAG( WORD wUnitID)
{
	MAPTPATHFLAG::iterator itTPATH = CTClientFlag::m_mapTPATHFLAG.begin();
	VTPATHFLAG::iterator itVPATH = m_vTPATH.begin();

	while( itVPATH != m_vTPATH.end() )
		if( HIWORD((*itVPATH)->m_dwID) == wUnitID )
			m_vTPATH.erase(itVPATH);
		else
			itVPATH++;

	while( itTPATH != CTClientFlag::m_mapTPATHFLAG.end() )
	{
		MAPTPATHFLAG::iterator itNEXT = itTPATH;
		itNEXT++;

		if( HIWORD((*itTPATH).second->m_dwID) == wUnitID )
			delete (*itTPATH).second;

		itTPATH = itNEXT;
	}
}

void CTClientMAP::ClearTBSP( BYTE bINDEX)
{
	while(!m_vTBSP[bINDEX].empty())
	{
		CTClientBSP *pTBSP = m_vTBSP[bINDEX].back();
		MAPTBSP::iterator finder = m_mapTBsp.find(pTBSP->m_dwBspID);

		if( finder != m_mapTBsp.end() )
			m_mapTBsp.erase(finder);

		m_vTBSP[bINDEX].pop_back();
		delete pTBSP;
	}
}

void CTClientMAP::ClearTHEIGHTINFO( BYTE bINDEX)
{
	MAPTHEIGHTINFO::iterator itHEIGHT;

	for( itHEIGHT = m_mapTHEIGHTINFO[bINDEX].begin(); itHEIGHT != m_mapTHEIGHTINFO[bINDEX].end(); itHEIGHT++)
	{
		(*itHEIGHT).second->clear();
		delete (*itHEIGHT).second;
	}

	m_mapTHEIGHTINFO[bINDEX].clear();
}

void CTClientMAP::ClearTSILHOUETTE( BYTE bINDEX)
{
	for( WORD i=0; i<m_vTSILHOUETTE[bINDEX].m_wCount; i++)
		m_vTSILHOUETTE[bINDEX].m_vTOBJ[i]->Release();

	m_vTSILHOUETTE[bINDEX].m_wCount = 0;
}

void CTClientMAP::ClearBIGOBJ( BYTE bINDEX)
{
	for( WORD i=0; i<m_vBIGOBJ[bINDEX].m_wCount; i++)
		m_vBIGOBJ[bINDEX].m_vTOBJ[i]->Release();

	m_vBIGOBJ[bINDEX].m_wCount = 0;
}

void CTClientMAP::LoadTHEIGHTINFO( LPMAPTHEIGHTINFO pTHEIGHTINFO,
								   WORD wUnitID)
{
	TRY
	{
		CString strFile;
		strFile = CTChart::Format( TSTR_FMT_HEIGHTFILE, MAKELONG( wUnitID, m_wMapID));

		CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
		DWORD dwCount = 0;

		file.Read( &dwCount, sizeof(DWORD));
		for( DWORD i=0; i<dwCount; i++)
		{
			LPVECTORDWORD pTINFO = new VECTORDWORD();
			DWORD dwObjCount;
			DWORD dwPosID;

			file.Read( &dwPosID, sizeof(DWORD));
			file.Read( &dwObjCount, sizeof(DWORD));

			for( DWORD j=0; j<dwObjCount; j++)
			{
				DWORD dwObjID;

				file.Read( &dwObjID, sizeof(DWORD));
				pTINFO->push_back(dwObjID);
			}

			pTHEIGHTINFO->insert( MAPTHEIGHTINFO::value_type( dwPosID, pTINFO));
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL
}

void CTClientMAP::LoadTFLAG( LPVTPATHFLAG pTFLAG,
							 WORD wUnitID)
{
	TRY
	{
		CString strFile;
		strFile = CTChart::Format( TSTR_FMT_PATHFILE, MAKELONG( wUnitID, m_wMapID));

		CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
		CArchive ar( &file, CArchive::load);

		DWORD dwCount;
		ar >> dwCount;

		for( DWORD i=0; i<dwCount; i++)
		{
			CTClientFlag *pTINFO = new CTClientFlag();
			DWORD dwTFLAG;

			ar	>> pTINFO->m_dwID
				>> pTINFO->m_vPOS.x
				>> pTINFO->m_vPOS.y
				>> pTINFO->m_vPOS.z
				>> dwTFLAG;

			for( DWORD j=0; j<dwTFLAG; j++)
			{
				DWORD dwNODE;
				ar >> dwNODE;

				pTINFO->m_vPATH.push_back(dwNODE);
			}

			pTFLAG->push_back(pTINFO);
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL
}

void CTClientMAP::LoadTNPC( LPVTNPC pTNPC,
						    WORD wUnitID)
{
	TRY
	{
		CString strFile;
		strFile = CTChart::Format( TSTR_FILE_MAPNPC, MAKELONG( wUnitID, m_wMapID));

		CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
		CArchive ar( &file, CArchive::load);

		WORD wCount;
		ar >> wCount;

		for( WORD i=0; i<wCount; i++)
		{
			LPTNPC pTINFO = new TNPC();

			CTChart::ReadTNPC( ar, pTINFO);
			pTNPC->push_back(pTINFO);
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL
}

BYTE CTClientMAP::HitTest( FLOAT *pDIST,
						   LPD3DXVECTOR3 pSTART,
						   LPD3DXVECTOR3 pEND,
						   FLOAT fLineH,
						   FLOAT fLineV,
						   BYTE bOnlyForCam)
{
	VTOBJBASE vTCOLLISION;

	if(!m_pMAP)
		return FALSE;

	vTCOLLISION.clear();
	GetLineCollisionOBJ(
		&vTCOLLISION,
		pSTART,
		pEND,
		fLineH);

	BYTE bResult = HitTestHeight( pDIST, pSTART, pEND, fLineH, fLineV);
	int nCount = INT(vTCOLLISION.size());

	for( int i=0; i<nCount; i++)
	{
		CTClientObjBase *pTCOLLOBJ = vTCOLLISION[i];
		FLOAT fLocal = 0.0f;

		if( !pTCOLLOBJ->m_bGARBAGE && (
			!bOnlyForCam ||
			pTCOLLOBJ->m_bCamCollision) &&
			pTCOLLOBJ->m_bType != OT_PATHWALL &&
			pTCOLLOBJ->HitPoly(
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
	vTCOLLISION.clear();

	return bResult;
}

BYTE CTClientMAP::HitTestHeight( FLOAT *pDIST,
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
			FLOAT fPosX = fStartX + FLOAT(j * m_pMAP->m_nTileLength);

			if( GetRegionID( fPosX, fPosZ) != BLANK_REGION )
			{
				static int nIndex[2][3] = {
					{ 0, 1, 2},
					{ 2, 1, 3}};

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
	}

	return bResult;
}

void CTClientMAP::FixTOBJHeight( CTClientObjBase *pTOBJ)
{
	CTClientObjBase *pFLOOR = NULL;

	pTOBJ->AddPositionY(pTOBJ->m_fSizeY);
	pTOBJ->m_vWorld._42 += pTOBJ->m_fSizeY;

	FLOAT fHeight = GetHeight(
		pTOBJ,
		&pFLOOR,
		&D3DXVECTOR3(
		pTOBJ->GetPositionX(),
		pTOBJ->GetPositionY(),
		pTOBJ->GetPositionZ()),
		0.0f, FALSE);

	pTOBJ->AddPositionY(-pTOBJ->m_fSizeY);
	pTOBJ->m_vWorld._42 -= pTOBJ->m_fSizeY;

	if(pFLOOR)
	{
		pTOBJ->m_bHouseMesh = pFLOOR->m_bHouseMesh;
		pTOBJ->m_dwHouseID = pFLOOR->m_dwHouseID;
	}

	if( fHeight > pTOBJ->m_vWorld._42 )
	{
		pTOBJ->SetPositionY(fHeight);
		pTOBJ->m_vWorld._42 = fHeight;
	}
}

void CTClientMAP::LandTOBJ( CTClientObjBase *pTOBJ)
{
	CTClientObjBase *pFLOOR = NULL;

	pTOBJ->AddPositionY(pTOBJ->m_fSizeY);
	pTOBJ->m_vWorld._42 += pTOBJ->m_fSizeY;

	FLOAT fWaterH = GetWaterHeight(
		pTOBJ->GetPositionX(),
		pTOBJ->GetPositionZ());

	FLOAT fHeight = GetHeight(
		pTOBJ,
		&pFLOOR,
		&D3DXVECTOR3(
		pTOBJ->GetPositionX(),
		pTOBJ->GetPositionY(),
		pTOBJ->GetPositionZ()),
		0.0f, FALSE);

	if(!pTOBJ->CanDIVE())
		fHeight = max( fWaterH, fHeight);

	pTOBJ->AddPositionY(-pTOBJ->m_fSizeY);
	pTOBJ->m_vWorld._42 -= pTOBJ->m_fSizeY;

	if( fWaterH >= pTOBJ->m_vWorld._42 &&
		fHeight <= pTOBJ->m_vWorld._42 )
		return;

	if( fHeight < fWaterH && fWaterH < pTOBJ->m_vWorld._42 )
		fHeight = fWaterH;

	if(pFLOOR)
	{
		pTOBJ->m_bHouseMesh = pFLOOR->m_bHouseMesh;
		pTOBJ->m_dwHouseID = pFLOOR->m_dwHouseID;
	}

	pTOBJ->SetPositionY(fHeight);
	pTOBJ->m_vWorld._42 = fHeight;
}

DWORD CTClientMAP::GetTStepSND( FLOAT fPosX,
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
		MAPDWORD::iterator finder = CTChart::m_mapTSTEPSND.find(MAKEWORD( bTileID, BYTE(m_wMapID)));

		if( finder != CTChart::m_mapTSTEPSND.end() )
			return (*finder).second;
	}

	return TDEFSTEP_SND;
}

BYTE CTClientMAP::IsEXTVisible( CTClientCAM *pCamera)
{
	return !m_dwHouseID || m_bHouseMesh ? TRUE : FALSE;
}

BYTE CTClientMAP::HasEXTCell( LPD3DXVECTOR3 pPOS)
{
	int nTCellX = INT(pPOS->x / TOBJCELL_LENGTH) - TDRAWCELL_WIDTH / 2;
	int nTCellZ = INT(pPOS->z / TOBJCELL_LENGTH) - TDRAWCELL_WIDTH / 2;
	int nUnitLength = m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength;

	CRect vINTER;
	CRect vUNIT(
		0, 0,
		nUnitLength,
		nUnitLength);

	CRect vTCELL(
		0, 0,
		INT(TOBJCELL_LENGTH * TDRAWCELL_WIDTH),
		INT(TOBJCELL_LENGTH * TDRAWCELL_WIDTH));

	vTCELL.OffsetRect(
		nTCellX * INT(TOBJCELL_LENGTH),
		nTCellZ * INT(TOBJCELL_LENGTH));

	vUNIT.OffsetRect(
		nUnitLength * (m_pMAP->m_nUnitX + 1),
		nUnitLength * (m_pMAP->m_nUnitZ + 1));

	vINTER.IntersectRect(
		&vTCELL,
		&vUNIT);

	return vTCELL == vINTER ? FALSE : TRUE;
}

void CTClientMAP::DoTLOADCMD_OBJ( CD3DDevice *pDevice,
								  CTachyonRes *pRES,
								  CTClientApp *pTAPP,
								  LPTLOADOBJ pTINFO)
{
	MAPTHEIGHTINFO mapTHEIGHTINFO[9];
	VTPATHFLAG vTFLAG[9];
	VTNPC vTNPC[9];
	VTBSP vTBSP[9];
	MAPTBSP mapTBSP;

	for( int i=0; i<9; i++)
	{
		mapTHEIGHTINFO[i].clear();
		vTFLAG[i].clear();
		vTNPC[i].clear();
		vTBSP[i].clear();
	}
	mapTBSP.clear();

	for( i=0; i<INT(pTINFO->m_vUnitID.size()); i++)
	{
		BYTE bINDEX = BYTE(HIWORD(pTINFO->m_vUnitID[i]));
		WORD wUnitID = LOWORD(pTINFO->m_vUnitID[i]);

		LoadTHEIGHTINFO( &mapTHEIGHTINFO[bINDEX], wUnitID);
		LoadTFLAG( &vTFLAG[bINDEX], wUnitID);
		LoadTNPC( &vTNPC[bINDEX], wUnitID);
	}

	for( i=0; i<INT(pTINFO->m_vTBSPINFO.size()); i++)
	{
		LPTBSPINFO pTDATA = pTINFO->m_vTBSPINFO[i];

		if( mapTBSP.find(pTDATA->m_dwBspID) == mapTBSP.end() )
		{
			CTClientBSP *pTBSP = new CTClientBSP();

			if(pTBSP->Lock( pRES, pTDATA->m_dwBspID))
			{
				pTBSP->m_dwHouseID = pTDATA->m_dwHouseID;
				pTBSP->m_fRadius = pTDATA->m_fRadius;
				pTBSP->InitTBSP(pTDATA);

				mapTBSP.insert( MAPTBSP::value_type( pTBSP->m_dwBspID, pTBSP));
				vTBSP[pTDATA->m_bINDEX].push_back(pTBSP);
			}
			else
				delete pTBSP;
		}
	}
	mapTBSP.clear();

	SMART_LOCKCS(&pTAPP->m_cs);
	if( m_pMAP && !pTAPP->m_bCancelLoad )
		for( i=0; i<INT(pTINFO->m_vUnitID.size()); i++)
		{
			BYTE bINDEX = BYTE(HIWORD(pTINFO->m_vUnitID[i]));
			WORD wUnitID = LOWORD(pTINFO->m_vUnitID[i]);
			MAPTHEIGHTINFO::iterator itHEIGHT;

			BYTE bUnitX = LOBYTE(wUnitID);
			BYTE bUnitZ = HIBYTE(wUnitID);

			FLOAT fUnitLength = FLOAT(m_pMAP->m_nUnitLength) * FLOAT(m_pMAP->m_nTileLength);
			FLOAT fUnitX = FLOAT(bUnitX) * fUnitLength;
			FLOAT fUnitZ = FLOAT(bUnitZ) * fUnitLength;

			for( int j=0; j<TOBJCELL_COUNT; j++)
				ClearTNPC(&m_vTMAPNPC[bINDEX][j]);

			ClearTHEIGHTINFO(bINDEX);
			ClearTFLAG(wUnitID);
			ClearTBSP(bINDEX);

			for( itHEIGHT = mapTHEIGHTINFO[bINDEX].begin(); itHEIGHT != mapTHEIGHTINFO[bINDEX].end(); itHEIGHT++)
				m_mapTHEIGHTINFO[bINDEX].insert( MAPTHEIGHTINFO::value_type( (*itHEIGHT).first, (*itHEIGHT).second));
			mapTHEIGHTINFO[bINDEX].clear();

			while(!vTNPC[bINDEX].empty())
			{
				LPTNPC pTNPC = vTNPC[bINDEX].back();

				FLOAT fLocalX = pTNPC->m_fPosX - fUnitX;
				FLOAT fLocalZ = pTNPC->m_fPosZ - fUnitZ;

				int nLocalX = INT(fLocalX / TOBJCELL_LENGTH);
				int nLocalZ = INT(fLocalZ / TOBJCELL_LENGTH);
				int nIndex = nLocalZ * TOBJCELL_WIDTH + nLocalX;

				m_vTMAPNPC[bINDEX][nIndex].push_back(pTNPC);
				vTNPC[bINDEX].pop_back();
			}

			while(!vTFLAG[bINDEX].empty())
			{
				CTClientFlag *pTFLAG = vTFLAG[bINDEX].back();
				MAPTPATHFLAG::iterator finder = CTClientFlag::m_mapTPATHFLAG.find(pTFLAG->m_dwID);

				if( finder != CTClientFlag::m_mapTPATHFLAG.end() )
				{
					pTFLAG->m_dwID = TPATHFLAG_NULL;
					delete pTFLAG;
				}
				else
					CTClientFlag::m_mapTPATHFLAG.insert( MAPTPATHFLAG::value_type( pTFLAG->m_dwID, pTFLAG));

				vTFLAG[bINDEX].pop_back();
			}

			while(!vTBSP[bINDEX].empty())
			{
				CTClientBSP *pTBSP = vTBSP[bINDEX].back();

				m_mapTBsp.insert( MAPTBSP::value_type( pTBSP->m_dwBspID, pTBSP));
				m_vTBSP[bINDEX].push_back(pTBSP);
				vTBSP[bINDEX].pop_back();
			}
		}

	for( i=0; i<9; i++)
	{
		MAPTHEIGHTINFO::iterator itHEIGHT;

		for( itHEIGHT = mapTHEIGHTINFO[i].begin(); itHEIGHT != mapTHEIGHTINFO[i].end(); itHEIGHT++)
		{
			(*itHEIGHT).second->clear();
			delete (*itHEIGHT).second;
		}
		mapTHEIGHTINFO[i].clear();

		while(!vTNPC[i].empty())
		{
			delete vTNPC[i].back();
			vTNPC[i].pop_back();
		}

		while(!vTFLAG[i].empty())
		{
			delete vTFLAG[i].back();
			vTFLAG[i].pop_back();
		}

		while(!vTBSP[i].empty())
		{
			delete vTBSP[i].back();
			vTBSP[i].pop_back();
		}
	}
}
 