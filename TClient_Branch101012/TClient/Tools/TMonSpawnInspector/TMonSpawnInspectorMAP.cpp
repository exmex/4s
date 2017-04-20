#include "StdAfx.h"
#include "TMonSpawnInspectorMAP.h"
#include "TMonSpawnInspectorObjBase.h"
#include "TMonSpawnInspectorObject.h"
#include "TMonSpawnInspectorHUGEMAP.h"
#include "resource.h"

MAPTMAPINFO CTMonSpawnInspectorMAP::m_mapTMAPINFO;
FLOAT CTMonSpawnInspectorMAP::m_fMoveBound = 0.58f;

CTMonSpawnInspectorMAP::CTMonSpawnInspectorMAP(void)
{
}

CTMonSpawnInspectorMAP::~CTMonSpawnInspectorMAP(void)
{
}

BOOL CTMonSpawnInspectorMAP::InitResource( CString strGroupID )
{
	m_Res.LoadMESH( strGroupID );
	m_Res.LoadOBJ( strGroupID );
	m_Res.LoadMAP( strGroupID );

	SetProgressText( "ComplateOBJ" );
	m_Res.ComplateOBJ();

	InitTMAPINFO();
	return TRUE;
}

VOID CTMonSpawnInspectorMAP::TermResource()
{
	MAPRES::iterator it;

	for( it = m_Res.m_mapMESH.begin(); it != m_Res.m_mapMESH.end(); it++)
		delete ((CTachyonMesh *) (*it).second);
	m_Res.m_mapMESH.clear();

	for( it = m_Res.m_mapOBJ.begin(); it != m_Res.m_mapOBJ.end(); it++)
		delete ((LPOBJECT) (*it).second);
	m_Res.m_mapOBJ.clear();

	for( it = m_Res.m_mapMAP.begin(); it != m_Res.m_mapMAP.end(); it++)
		delete ((LPRESINDEX) (*it).second);
	m_Res.m_mapMAP.clear();

	ReleaseTMAPINFO();

	CTachyonMesh::ReleaseGlobalVB();
}

VOID CTMonSpawnInspectorMAP::ClearCOLL()
{
	std::vector< CTMonSpawnInspectorObjBase* >::iterator it, end;
	it = m_vCOLL.begin();
	end = m_vCOLL.end();

	for(; it != end ; ++it )
		delete (*it);

	m_vCOLL.clear();
}

VOID CTMonSpawnInspectorMAP::InitTMAPINFO()
{
	MAPTMAPINFO::iterator itTNODE;
	MAPTMAPINFO mapTNODE;

	CString strFile;
	WORD wCount;

	ReleaseTMAPINFO();
	mapTNODE.clear();

	strFile.LoadString(IDS_FILE_NODECHART);
	CFile vTNODE( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive vTNODECHART( &vTNODE, CArchive::load);

	vTNODECHART
		>> wCount;

	DWORD dwProgress = 0;
	SetProgressText( "TNODE.tcd 읽기" );
	SetProgressRange( 0, wCount );

	for( WORD i=0; i<wCount; i++)
	{
		LPD3DXVECTOR3 pPOS = new D3DXVECTOR3();
		LPTMAPINFO pTNODE = NULL;

		DWORD dwNodeID;
		DWORD dwMapID;

		vTNODECHART
			>> dwMapID
			>> dwNodeID
			>> pPOS->x
			>> pPOS->y
			>> pPOS->z;

		itTNODE = mapTNODE.find(dwMapID);
		if( itTNODE == mapTNODE.end() )
		{
			pTNODE = new TMAPINFO;
			mapTNODE.insert( MAPTMAPINFO::value_type( dwMapID, pTNODE));
		}
		else
			pTNODE = (*itTNODE).second;

		pTNODE->m_mapTNODE.insert( MAPD3DXVECTOR3::value_type( dwNodeID, pPOS));

		SetProgressPos( ++dwProgress );
	}

	strFile.LoadString(IDS_FILE_MAPCHART);
	CFile vTMAP( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive vTMAPCHART( &vTMAP, CArchive::load);

	vTMAPCHART
		>> wCount;

	dwProgress = 0;
	SetProgressText( "TMAP.tcd 읽기" );
	SetProgressRange( 0, wCount );

	for( i=0; i<wCount; i++)
	{
		LPTMAPINFO pTMAPINFO = new TMAPINFO();
		DWORD dwMapID;

		FLOAT fScaleX;
		FLOAT fScaleY;
		FLOAT fScaleZ;

		FLOAT fPosX;
		FLOAT fPosY;
		FLOAT fPosZ;

		FLOAT fRotX;
		FLOAT fRotY;
		FLOAT fRotZ;

		vTMAPCHART
			>> dwMapID
			>> pTMAPINFO->m_dwMapID
			>> fScaleX
			>> fScaleY
			>> fScaleZ
			>> fPosX
			>> fPosY
			>> fPosZ
			>> fRotX
			>> fRotY
			>> fRotZ
			>> pTMAPINFO->m_bDungeon;

		itTNODE = mapTNODE.find(pTMAPINFO->m_dwMapID);
		if( itTNODE != mapTNODE.end() )
		{
			MAPD3DXVECTOR3::iterator itTPOS;
			D3DXMATRIX vPOS;
			D3DXMATRIX vROT;

			D3DXMatrixScaling(
				&vPOS,
				fScaleX,
				fScaleY,
				fScaleZ);

			D3DXMatrixRotationX(
				&vROT,
				fRotX);
			vPOS *= vROT;

			D3DXMatrixRotationY(
				&vROT,
				fRotY);
			vPOS *= vROT;

			D3DXMatrixRotationZ(
				&vROT,
				fRotZ);
			vPOS *= vROT;

			vPOS._41 = fPosX;
			vPOS._42 = fPosY;
			vPOS._43 = fPosZ;

			for( itTPOS = (*itTNODE).second->m_mapTNODE.begin(); itTPOS != (*itTNODE).second->m_mapTNODE.end(); itTPOS++)
			{
				LPD3DXVECTOR3 pPOS = new D3DXVECTOR3(
					(*itTPOS).second->x,
					(*itTPOS).second->y,
					(*itTPOS).second->z);

				CTMath::Transform(
					&vPOS,
					pPOS);

				pTMAPINFO->m_mapTNODE.insert( MAPD3DXVECTOR3::value_type( (*itTPOS).first, pPOS));
			}
		}

		CTMonSpawnInspectorMAP::m_mapTMAPINFO.insert( MAPTMAPINFO::value_type( dwMapID, pTMAPINFO));

		SetProgressPos( ++dwProgress );
	}

	for( itTNODE = mapTNODE.begin(); itTNODE != mapTNODE.end(); itTNODE++)
		delete (*itTNODE).second;
	mapTNODE.clear();
}

void CTMonSpawnInspectorMAP::ReleaseTMAPINFO()
{
	MAPTMAPINFO::iterator itTMAP;

	for( itTMAP = CTMonSpawnInspectorMAP::m_mapTMAPINFO.begin(); itTMAP != CTMonSpawnInspectorMAP::m_mapTMAPINFO.end(); itTMAP++)
		delete (*itTMAP).second;

	CTMonSpawnInspectorMAP::m_mapTMAPINFO.clear();
}

LPTMAPINFO CTMonSpawnInspectorMAP::GetMapINFO( DWORD dwMapID)
{
	MAPTMAPINFO::iterator finder = m_mapTMAPINFO.find(dwMapID);

	if( finder != m_mapTMAPINFO.end() )
		return (*finder).second;

	return NULL;
}

FLOAT CTMonSpawnInspectorMAP::GetAttrFLOAT( LPMAPOBJ pTOBJ,
										   DWORD dwID)
{
	MAPATTRIBUTE::iterator finder = pTOBJ->m_mapATTR.find(dwID);

	if( finder == pTOBJ->m_mapATTR.end() )
		return 0;

	return *((FLOAT *) (*finder).second->m_pValue);
}

int CTMonSpawnInspectorMAP::GetAttrINT( LPMAPOBJ pTOBJ,
									   DWORD dwID)
{
	MAPATTRIBUTE::iterator finder = pTOBJ->m_mapATTR.find(dwID);

	if( finder == pTOBJ->m_mapATTR.end() )
		return 0;

	return *((int *) (*finder).second->m_pValue);
}

void CTMonSpawnInspectorMAP::LoadMAPOBJ( CTachyonHUGEMAP* m_pMAP )
{
	//for( BYTE i=0; i<9; i++)
	//	if(m_pMAP->m_bUPDATE[i])
	{
		int i=0;

		BYTE bUnitX = m_pMAP->m_nUnitX + i % 3;
		BYTE bUnitZ = m_pMAP->m_nUnitZ + i / 3;

		FLOAT fUnitLength = FLOAT(m_pMAP->m_nUnitLength * m_pMAP->m_nTileLength);
		FLOAT fUnitX = FLOAT(bUnitX) * fUnitLength;
		FLOAT fUnitZ = FLOAT(bUnitZ) * fUnitLength;

		MAPMAPOBJ::iterator itOBJ;
		//MAPMAPSFX::iterator itSFX;
		//MAPMAPSND::iterator itSND;

		//for( WORD j=0; j<TOBJCELL_COUNT; j++)
		//{
		//	ClearTNPC(&m_vTMAPNPC[m_pMAP->m_bUNIT[i]][j]);
		//	m_vTMAPOBJ[m_pMAP->m_bUNIT[i]][j].clear();
		//	m_vTMAPSFX[m_pMAP->m_bUNIT[i]][j].clear();
		//	m_vTMAPSND[m_pMAP->m_bUNIT[i]][j].clear();
		//}

		//ClearTSILHOUETTE(m_pMAP->m_bUNIT[i]);
		//ClearBIGOBJ(m_pMAP->m_bUNIT[i]);
		//ClearTBSP(m_pMAP->m_bUNIT[i]);
		//LoadTNPC( pDevice, i);

		//ClearTFLAG(i);
		//LoadTFLAG(i);

		DWORD dwProgress = 0;
		SetProgressRange( 0, (INT)m_pMAP->m_pUNIT[i]->m_mapOBJ.size() );
		SetProgressPos( 0 );

		for( itOBJ = m_pMAP->m_pUNIT[i]->m_mapOBJ.begin(); itOBJ != m_pMAP->m_pUNIT[i]->m_mapOBJ.end(); itOBJ++)
		{
			BYTE bOBJType = GetAttrINT( (*itOBJ).second, ID_OBJ_TYPE);

			switch(bOBJType)
			{
			case OT_SILHOUETTE	: break;
			case OT_FOG				: break;
			case OT_BSP				: break;
			case OT_WATER			: break;

			case OT_COLLISION		:
			case OT_PATHWALL		:
			default						:
				{
					FLOAT fSizeX = (*itOBJ).second->m_vSCALE.x * GetAttrFLOAT( (*itOBJ).second, ID_SIZE_X);
					FLOAT fSizeY = (*itOBJ).second->m_vSCALE.y * GetAttrFLOAT( (*itOBJ).second, ID_SIZE_Y);
					FLOAT fSizeZ = (*itOBJ).second->m_vSCALE.z * GetAttrFLOAT( (*itOBJ).second, ID_SIZE_Z);
					FLOAT fDepth = (*itOBJ).second->m_vSCALE.y * GetAttrFLOAT( (*itOBJ).second, ID_DEPTH);
					FLOAT fSight = max(
						(*itOBJ).second->m_vSCALE.x, max(
						(*itOBJ).second->m_vSCALE.y,
						(*itOBJ).second->m_vSCALE.z)) * GetAttrFLOAT( (*itOBJ).second, ID_SIGHT);

					CTMonSpawnInspectorObjBase* pINST = new CTMonSpawnInspectorObject();
					pINST->ReleaseData();

					pINST->InitOBJ( NULL, (*itOBJ).second);
					pINST->m_bCollisionType = pINST->GetAttrINT(ID_COLLISION_TYPE);
					pINST->m_bCamCollision = pINST->GetAttrINT(ID_CAM_COLLISION);
					pINST->m_dwID = pINST->GetAttrINT(ID_MAP_ID);
					pINST->m_bType = pINST->GetAttrINT(ID_OBJ_TYPE);
					pINST->m_bHouseMesh = BYTE(pINST->GetAttrINT(ID_HOUSE_MESH));
					//pINST->m_bHouseOUT = BYTE(pINST->GetAttrINT(ID_HOUSE_OUT));
					pINST->m_dwHouseID = DWORD(pINST->GetAttrINT(ID_HOUSE_ID));
					pINST->m_bLastDraw = BYTE(pINST->GetAttrINT(ID_LAST_DRAW));

					pINST->InitSIZE(
						fSizeX,
						fSizeY,
						fSizeZ,
						fDepth,
						fSight);

					pINST->m_dwUnitID = MAKELONG(
						WORD(pINST->GetPositionX() / fUnitLength),
						WORD(pINST->GetPositionY() / fUnitLength));

					if( pINST->m_bCollisionType == TCOLLISION_POLY )
						pINST->InitCollision();

					m_vCOLL.push_back( pINST );
				}
				break;
			}

			SetProgressPos( ++dwProgress );
		}
	}
}

BOOL CTMonSpawnInspectorMAP::IsValidPoint(
	D3DXVECTOR3 vSpawnPos,
	FLOAT fRange )
{
	// 스폰 위치랑 교차하는 오브젝트가 있는지 확인한다.
	CTMonSpawnInspectorObjBase vMOVE;
	vMOVE.ReleaseData();

	D3DXMatrixIdentity(&vMOVE.m_vWorld);
	vMOVE.m_vWorld._41 = vSpawnPos.x;
	vMOVE.m_vWorld._42 = vSpawnPos.y;
	vMOVE.m_vWorld._43 = vSpawnPos.z;
	vMOVE.m_vPosition = vMOVE.m_vWorld;

	/*vMOVE.InitSIZE(
	TCOLLISION_SIZE,
	TCOLLISION_HEIGHT,
	TCOLLISION_SIZE,
	0.0f, 0.0f);*/

	vMOVE.InitSIZE(
		fRange+0.1f,
		TCOLLISION_HEIGHT,
		fRange+0.1f,
		0.0f, 0.0f);

	vMOVE.m_bCollisionType = TCOLLISION_CYLINDER;

	BOOL bResult = TRUE;

	std::vector< CTMonSpawnInspectorObjBase* >::iterator it, end;
	it = m_vCOLL.begin();
	end = m_vCOLL.end();

	for(; it != end ; ++it )
	{
		CTMonSpawnInspectorObjBase *pMAPOBJ = (*it);

		FLOAT fLength = D3DXVec3Length(&D3DXVECTOR3(
			pMAPOBJ->m_vPosition._41 - vMOVE.m_vPosition._41,
			pMAPOBJ->m_vPosition._42 - vMOVE.m_vPosition._42,
			pMAPOBJ->m_vPosition._43 - vMOVE.m_vPosition._43));
		FLOAT fRadius = pMAPOBJ->m_fRadiusH + vMOVE.m_fRadiusH;

		if( fLength < fRadius && vMOVE.CheckCollision(pMAPOBJ) )
			return FALSE;
	}

	return TRUE;
}

BOOL CTMonSpawnInspectorMAP::FindValidPoint(
	D3DXVECTOR3 vSpawnPos,
	FLOAT fRange,
	CTMonSpawnInspectorHUGEMAP* pMAP,
	D3DXVECTOR3* vValidPos )
{
	INT nRetryCount = 0;

RETRY:

	if( nRetryCount > 10 )		// 10번이나 밀어내기 시도를 했는데 안되면 포기한다.
		return FALSE;

	// 스폰 위치랑 교차하는 오브젝트가 있는지 확인한다.
	CTMonSpawnInspectorObjBase vMOVE;
	vMOVE.ReleaseData();

	D3DXMatrixIdentity(&vMOVE.m_vWorld);
	vMOVE.m_vWorld._41 = vSpawnPos.x;
	vMOVE.m_vWorld._42 = vSpawnPos.y;
	vMOVE.m_vWorld._43 = vSpawnPos.z;
	vMOVE.m_vPosition = vMOVE.m_vWorld;

	/*vMOVE.InitSIZE(
	TCOLLISION_SIZE,
	TCOLLISION_HEIGHT,
	TCOLLISION_SIZE,
	0.0f, 0.0f);*/

	vMOVE.InitSIZE(
		fRange+0.1f,
		TCOLLISION_HEIGHT,
		fRange+0.1f,
		0.0f, 0.0f);

	vMOVE.m_bCollisionType = TCOLLISION_CYLINDER;

	BOOL bResult = TRUE;

	std::vector< CTMonSpawnInspectorObjBase* >::iterator it, end;
	it = m_vCOLL.begin();
	end = m_vCOLL.end();

	for(; it != end ; ++it )
	{
		CTMonSpawnInspectorObjBase *pMAPOBJ = (*it);

		FLOAT fLength = D3DXVec3Length(&D3DXVECTOR3(
			pMAPOBJ->m_vPosition._41 - vMOVE.m_vPosition._41,
			pMAPOBJ->m_vPosition._42 - vMOVE.m_vPosition._42,
			pMAPOBJ->m_vPosition._43 - vMOVE.m_vPosition._43));

		FLOAT fRadius = pMAPOBJ->m_fRadiusH + vMOVE.m_fRadiusH;

		if( fLength < fRadius && vMOVE.CheckCollision(pMAPOBJ) )
		{
			// 충돌되면 오브젝트의 정반대방향으로 밀어낸다.
			
			D3DXVECTOR3 vObj(
				pMAPOBJ->m_vPosition._41,
				pMAPOBJ->m_vPosition._42,
				pMAPOBJ->m_vPosition._43);

			D3DXVECTOR3 vSpawn(
				vMOVE.m_vPosition._41,
				vMOVE.m_vPosition._42,
				vMOVE.m_vPosition._43);

			D3DXVECTOR3 vNorm = vSpawn - vObj;		
			D3DXVec3Normalize( &vNorm, &vNorm);
			vNorm *= fRange;

			vSpawnPos += vNorm;

			++nRetryCount;
			goto RETRY;

			return FALSE;
		}
	}

	FLOAT fResultHeight = 0.0f;
	FLOAT fResultLandHeight = 0.0f;

	if( FALSE == IsValidHeight(
								pMAP,
								vSpawnPos,
								fRange,
								&fResultHeight,
								&fResultLandHeight))
	{
		vSpawnPos.y = fResultHeight;
	}

	if( vValidPos )
		*vValidPos = vSpawnPos;

	return TRUE;
}

BOOL CTMonSpawnInspectorMAP::IsValidHeight(
	CTMonSpawnInspectorHUGEMAP* pMAP,
	D3DXVECTOR3 vSpawnPos,
	FLOAT fRange,
	FLOAT* pResultHeight,
	FLOAT* fLandHeight)
{
	// 스폰 위치랑 교차하는 오브젝트가 있는지 확인한다.
	CTMonSpawnInspectorObjBase vMOVE;
	vMOVE.ReleaseData();

	D3DXMatrixIdentity(&vMOVE.m_vWorld);
	vMOVE.m_vWorld._41 = vSpawnPos.x;
	vMOVE.m_vWorld._42 = vSpawnPos.y;
	vMOVE.m_vWorld._43 = vSpawnPos.z;
	vMOVE.m_vPosition = vMOVE.m_vWorld;

	/*vMOVE.InitSIZE(
	TCOLLISION_SIZE,
	TCOLLISION_HEIGHT,
	TCOLLISION_SIZE,
	0.0f, 0.0f);*/

	vMOVE.InitSIZE(
		fRange+0.1f,
		TCOLLISION_HEIGHT,
		fRange+0.1f,
		0.0f, 0.0f);

	vMOVE.m_bCollisionType = TCOLLISION_CYLINDER;

	/////

	FLOAT fHeight = GetHeight(
		pMAP,
		&m_vCOLL,
		&vMOVE,
		NULL,
		&vSpawnPos,
		0.0f, FALSE,
		fLandHeight);

	*pResultHeight = fHeight;

	FLOAT fA = fabs(vSpawnPos.y);
	FLOAT fB = fabs(fHeight);

	if( (fA+0.01f) > fB )
		return TRUE;

	return FALSE;
}

FLOAT CTMonSpawnInspectorMAP::GetHeight(
										CTMonSpawnInspectorHUGEMAP* pMAP,
										LPVTOBJBASE pTMAPOBJ,
										CTMonSpawnInspectorObjBase *pOBJ,
										CTMonSpawnInspectorObjBase **pFLOOR,
										LPD3DXVECTOR3 pPOINT,
										FLOAT fMove,
										BYTE bCheckBound,
										FLOAT* fLandHeight)
{
	FLOAT fResult = pMAP->GetHeight(
		pPOINT->x,
		pPOINT->y,
		pPOINT->z);

	*fLandHeight = fResult;

	FLOAT fPrevH = pPOINT->y + fMove / CTMonSpawnInspectorMAP::m_fMoveBound;
	FLOAT fLandH = fResult;

	if( pPOINT->y + pOBJ->m_fSizeY < fResult /*||
											 GetRegionID( pPOINT->x, pPOINT->z) == BLANK_REGION*/ )
											 fResult = TMIN_HEIGHT;

	if(pFLOOR)
		(*pFLOOR) = NULL;

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
			{
				if(pFLOOR)
					(*pFLOOR) = (*pTMAPOBJ)[i];

				fResult = fHeight;
			}
		}
	}

	if( fResult <= TMIN_HEIGHT )
	{
		if(pFLOOR)
			(*pFLOOR) = NULL;

		fResult = fLandH;
	}

	return fResult + THEIGHT_ZERO;
}