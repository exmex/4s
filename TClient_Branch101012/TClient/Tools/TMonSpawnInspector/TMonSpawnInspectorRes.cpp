#include "StdAfx.h"
#include "TMonSpawnInspectorRes.h"
#include "TMonSpawnInspectorHUGEMAP.h"

CTMonSpawnInspectorRes::CTMonSpawnInspectorRes(void)
{
}

CTMonSpawnInspectorRes::~CTMonSpawnInspectorRes(void)
{
}

void CTMonSpawnInspectorRes::LoadMESH( CString strGroupID)
{
	DWORD dwBASE = DWORD(CTachyonMesh::m_vFILEVB.size());
	int nTotal = 0;
	int nCount = 0;
	int nIndex = 0;
	CString strINDEX;

	strINDEX.Format( ".\\Index\\%sM.IDX", strGroupID);
	CFile file( strINDEX, CFile::modeRead|CFile::typeBinary);

	file.Read( &nCount, sizeof(int));
	file.Read( &nTotal, sizeof(int));

	LPMAPRES pTRES = new MAPRES[nCount];
	//RenderBACK( T3DRES_MESH, 0);

	DWORD dwProgress = 0;
	SetProgressText("메쉬 초기화");
	SetProgressRange( 0, nCount*2+nTotal );

	for( int i=0; i<nCount; i++)
	{
		CTachyonMesh::m_vFILEVB.push_back(new FILEVB());
		m_vMESHFILE.push_back(LoadString(&file));

		LoadMESH( dwBASE, DWORD(i), &pTRES[i], nIndex, nTotal);

		SetProgressPos( ++dwProgress );
	}

	for( i=0; i<nTotal; i++)
	{
		DWORD dwFileID;
		DWORD dwPOS;
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &dwFileID, sizeof(DWORD));
		file.Read( &dwPOS, sizeof(DWORD));

		MAPRES::iterator finder = pTRES[dwFileID].find(dwPOS);
		if( finder != pTRES[dwFileID].end() )
		{
			m_mapMESH.insert( MAPRES::value_type( dwID, (*finder).second));
			pTRES[dwFileID].erase(finder);
		}

		SetProgressPos( ++dwProgress );
	}

	for( i=0; i<nCount; i++)
	{
		MAPRES::iterator it;

		for( it = pTRES[i].begin(); it != pTRES[i].end(); it++)
			delete (*it).second;
		pTRES[i].clear();

		SetProgressPos( ++dwProgress );
	}

	delete[] pTRES;
}

void CTMonSpawnInspectorRes::LoadMESH( DWORD dwBASE,
									  DWORD dwFileID,
									  LPMAPRES pTRES,
									  int& nIndex,
									  int nTotal)
{
	CFile file( _T(".\\Data\\") + m_vMESHFILE[dwFileID], CFile::modeRead|CFile::typeBinary);

	DWORD dwLENGTH = DWORD(file.GetLength());
	DWORD dwPOS = DWORD(file.GetPosition());

	while( dwPOS < dwLENGTH )
	{
		CTachyonMesh *pMESH = new CTachyonMesh();
		DWORD dwCount = 0;
		DWORD dwLevel = 0;

		file.Read( &pMESH->m_dwMeshCount, sizeof(DWORD));
		file.Read( &pMESH->m_dwNodeCount, sizeof(DWORD));
		file.Read( &pMESH->m_bUseVB, sizeof(BYTE));
		file.Read( &dwLevel, sizeof(DWORD));

		file.Read( &pMESH->m_vCenterPoint, sizeof(D3DXVECTOR3));
		file.Read( &pMESH->m_fRadius, sizeof(FLOAT));

		pMESH->m_bMESHType = pMESH->m_dwNodeCount > 0 ? MESHTYPE_WMESH : MESHTYPE_MESH;
		pMESH->m_bVBType = VBTYPE_GLOBAL;

		pMESH->m_bFileID = BYTE(dwFileID);
		pMESH->m_bGroupID = m_bGroupID;
		pMESH->m_dwPOS = DWORD(file.GetPosition());

		if( pMESH->m_dwNodeCount > 0 )
			file.Seek( pMESH->m_dwNodeCount * sizeof(D3DXMATRIX), SEEK_CUR);

		if( pMESH->m_dwMeshCount > 0 )
		{
			pMESH->m_pMESHIB = new VECTORIBINDEXINFO[pMESH->m_dwMeshCount];
			pMESH->m_pMESH = new VECTORIBINFO[pMESH->m_dwMeshCount];
			pMESH->m_pIB = new VECTORIB[pMESH->m_dwMeshCount];
		}

		file.Read( &dwCount, sizeof(DWORD));
		if( dwCount > 0 )
		{
			pMESH->m_pDATA = new VBDATA();

			pMESH->m_pDATA->m_dwFVF = pMESH->m_dwNodeCount > 0 ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX;
			pMESH->m_pDATA->m_dwCount = dwCount;

			switch(pMESH->m_pDATA->m_dwFVF)
			{
			case T3DFVF_WMESHVERTEX	: file.Seek( dwCount * sizeof(WMESHVERTEX), SEEK_CUR); break;
			case T3DFVF_MESHVERTEX	: file.Seek( dwCount * sizeof(MESHVERTEX), SEEK_CUR); break;
			}
		}

		if(pMESH->m_bUseVB)
		{
			LPVECTORINDEXEDMESH pVBPOOL = pMESH->m_dwNodeCount > 0 ? &CTachyonMesh::m_vFILEVB[dwBASE + dwFileID]->m_vWMESH : &CTachyonMesh::m_vFILEVB[dwBASE + dwFileID]->m_vMESH;
			LPINDEXEDMESH pVB = NULL;

			if( pVBPOOL->empty() || (CTachyonMesh::m_dwMaxVB && pVBPOOL->back()->m_dwVB + dwCount > CTachyonMesh::m_dwMaxVB) )
			{
				pVB = new INDEXEDMESH();
				pVBPOOL->push_back(pVB);
			}
			else
				pVB = pVBPOOL->back();

			pMESH->m_bVBID = BYTE(INT(pVBPOOL->size()) - 1);
			pMESH->m_dwVBIndex = pVB->m_dwVB;
			pVB->m_dwVB += dwCount;

			for( int i=0; i<pMESH->m_dwMeshCount; i++)
				for( int j=0; j<dwLevel; j++)
				{
					LPIBINDEXINFO pIBINFO = new IBINDEXINFO();

					file.Read( &dwCount, sizeof(DWORD));
					for( DWORD k=0; k<dwCount; k++)
					{
						LPIBINDEX pMESHIB = new IBINDEX();

						file.Read( &pMESHIB->m_dwCount, sizeof(DWORD));
						file.Seek( sizeof(WORD), SEEK_CUR);

						if( pMESHIB->m_dwCount > 0 )
							file.Seek( pMESHIB->m_dwCount * sizeof(WORD), SEEK_CUR);

						pMESHIB->m_dwPOS = pVB->m_dwIB;
						pVB->m_dwIB += pMESHIB->m_dwCount;

						pIBINFO->m_vIB.push_back(pMESHIB);
					}

					pMESH->m_pMESHIB[i].push_back(pIBINFO);
				}

				if(!pMESH->GetGlobalVB())
				{
					CTachyonMesh::m_mapVB.insert( MAPINDEXEDMESH::value_type( GLOBALVB_ID(
						pMESH->m_bGroupID,
						pMESH->m_bFileID,
						pMESH->m_bMESHType,
						pMESH->m_bVBID), pVB));
				}
		}
		else
		{
			for( int i=0; i<pMESH->m_dwMeshCount; i++)
				for( int j=0; j<dwLevel; j++)
				{
					LPIBINDEXINFO pIBINFO = new IBINDEXINFO();

					file.Read( &dwCount, sizeof(DWORD));
					for( DWORD k=0; k<dwCount; k++)
					{
						LPIBINDEX pMESHIB = new IBINDEX();

						file.Read( &pMESHIB->m_dwCount, sizeof(DWORD));
						file.Seek( sizeof(WORD), SEEK_CUR);

						if( pMESHIB->m_dwCount > 0 )
							file.Seek( pMESHIB->m_dwCount * sizeof(WORD), SEEK_CUR);

						pIBINFO->m_vIB.push_back(pMESHIB);
						pMESHIB->m_dwPOS = 0;
					}

					pMESH->m_pMESHIB[i].push_back(pIBINFO);
				}
		}

		for( int i=0; i<INT(dwLevel) - 1; i++)
		{
			FLOAT fDist;

			file.Read( &fDist, sizeof(FLOAT));
			pMESH->m_vDist.push_back(fDist);
		}

		pTRES->insert( MAPRES::value_type( dwPOS, pMESH));
		dwPOS = DWORD(file.GetPosition());
		nIndex++;

		//RenderBACK( T3DRES_MESH, BYTE(nIndex * 100 / nTotal));
	}
}

void CTMonSpawnInspectorRes::LoadMAP( CString strGroupID)
{
	CString strINDEX;
	int nTotal = 0;
	int nCount = 0;

	strINDEX.Format( ".\\Index\\%sP.IDX", strGroupID);
	CFile file( strINDEX, CFile::modeRead|CFile::typeBinary);

	file.Read( &nCount, sizeof(int));
	file.Read( &nTotal, sizeof(int));
	//RenderBACK( T3DRES_MAP, 0);
	DWORD dwProgress = 0;
	SetProgressText( "맵 초기화" );
	SetProgressRange( 0, nCount+nTotal );

	for( int i=0; i<nCount; i++)
	{
		m_vMAPFILE.push_back(LoadString(&file));

		SetProgressPos( ++dwProgress );
	}

	for( i=0; i<nTotal; i++)
	{
		LPRESINDEX pRES = new RESINDEX();
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &pRES->m_dwFileID, sizeof(DWORD));
		file.Read( &pRES->m_dwPOS, sizeof(DWORD));

		m_mapMAP.insert( MAPRES::value_type( dwID, pRES));
		//RenderBACK( T3DRES_MAP, BYTE(i * 100 / nTotal));
		SetProgressPos( ++dwProgress );
	}
}

CTachyonMAP *CTMonSpawnInspectorRes::LockMAP( DWORD dwID)
{
	MAPRES::iterator finder = m_mapMAP.find(dwID);

	if( finder != m_mapMAP.end() )
	{
		LPRESINDEX pRES = (LPRESINDEX) (*finder).second;
		CFile file( _T(".\\Data\\") + m_vMAPFILE[pRES->m_dwFileID], CFile::modeRead|CFile::typeBinary);
		file.Seek( pRES->m_dwPOS, SEEK_SET);

		CTachyonMAP *pResult = NULL;
		int nCount = 0;
		BYTE bMAPType;
		file.Read( &bMAPType, sizeof(BYTE));

		switch(bMAPType)
		{
		case MT_EXT	:
			{
				pResult = new CTMonSpawnInspectorHUGEMAP();

				file.Read( &((CTMonSpawnInspectorHUGEMAP *) pResult)->m_nUnitCountX, sizeof(int));
				file.Read( &((CTMonSpawnInspectorHUGEMAP *) pResult)->m_nUnitCountZ, sizeof(int));
				file.Read( &((CTMonSpawnInspectorHUGEMAP *) pResult)->m_nUnitLength, sizeof(int));
				file.Read( &((CTMonSpawnInspectorHUGEMAP *) pResult)->m_nTileLength, sizeof(int));
				file.Read( &((CTMonSpawnInspectorHUGEMAP *) pResult)->m_nCellCount, sizeof(int));
				file.Read( &((CTMonSpawnInspectorHUGEMAP *) pResult)->m_nTextureSize, sizeof(int));
				file.Read( &((CTMonSpawnInspectorHUGEMAP *) pResult)->m_nShadowPixel, sizeof(int));
				file.Read( &((CTMonSpawnInspectorHUGEMAP *) pResult)->m_vLIGHT, sizeof(D3DXVECTOR3));

				file.Read( &((CTMonSpawnInspectorHUGEMAP *) pResult)->m_bUseSHADER, sizeof(BYTE));
				file.Read( &((CTMonSpawnInspectorHUGEMAP *) pResult)->m_fIntensity, sizeof(FLOAT));
				file.Read( &((CTMonSpawnInspectorHUGEMAP *) pResult)->m_fSpecRange, sizeof(FLOAT));
				file.Read( &((CTMonSpawnInspectorHUGEMAP *) pResult)->m_fSpecMax, sizeof(FLOAT));
				BYTE bCount = 0;

				file.Read( &bCount, sizeof(BYTE));
				for( BYTE i=0; i<bCount; i++)
				{
					DWORD dwDetailID;
					FLOAT fDetailS;

					file.Read( &fDetailS, sizeof(FLOAT));
					file.Read( &dwDetailID, sizeof(DWORD));

					MAPRES::iterator finder = m_mapTEX.find(dwDetailID);
					if( finder != m_mapTEX.end() )
						((CTMonSpawnInspectorHUGEMAP *) pResult)->m_vDETAILDATA.push_back((LPTEXTURESET) (*finder).second);
					else
						((CTMonSpawnInspectorHUGEMAP *) pResult)->m_vDETAILDATA.push_back(NULL);
					((CTMonSpawnInspectorHUGEMAP *) pResult)->m_vDETAILSDATA.push_back(fDetailS);
				}

				int nCount = ((CTMonSpawnInspectorHUGEMAP *) pResult)->m_nUnitCountX * ((CTachyonHUGEMAP *) pResult)->m_nUnitCountZ;
				((CTMonSpawnInspectorHUGEMAP *) pResult)->InitVB(m_pDEVICE);
				file.Read( ((CTMonSpawnInspectorHUGEMAP *) pResult)->m_pENABLE, nCount * sizeof(BYTE));

				nCount = ((CTMonSpawnInspectorHUGEMAP *) pResult)->m_nUnitLength;
				((CTMonSpawnInspectorHUGEMAP *) pResult)->m_pRES = this;
				((CTMonSpawnInspectorHUGEMAP *) pResult)->m_dwID = dwID;

				for( i=0; i<9; i++)
				{
					((CTMonSpawnInspectorHUGEMAP *) pResult)->m_UNIT[i].m_nTileCount = ((CTachyonHUGEMAP *) pResult)->m_nUnitLength;
					((CTMonSpawnInspectorHUGEMAP *) pResult)->m_UNIT[i].m_pSHADOW = new WORD[nCount * nCount];
					((CTMonSpawnInspectorHUGEMAP *) pResult)->m_UNIT[i].m_pREGION = new BYTE[nCount * nCount];
					((CTMonSpawnInspectorHUGEMAP *) pResult)->m_UNIT[i].m_pDETAIL = new BYTE[nCount * nCount];
					((CTMonSpawnInspectorHUGEMAP *) pResult)->m_UNIT[i].m_pDATA = new FLOAT[(nCount + 1) * (nCount + 1)];
				}
			}

			break;

		case MT_BSP	:
			{
				pResult = new CTachyonBSPMAP();

				((CTachyonBSPMAP *) pResult)->LoadMAP(&file);
				((CTachyonBSPMAP *) pResult)->m_pRegionID = new BYTE[((CTachyonBSPMAP *) pResult)->m_dwMeshCount];
				file.Read( ((CTachyonBSPMAP *) pResult)->m_pRegionID, ((CTachyonBSPMAP *) pResult)->m_dwMeshCount * sizeof(BYTE));
				file.Read( &nCount, sizeof(int));

				for( int i=0; i<nCount; i++)
				{
					LPTEXTURESET *pTEX = new LPTEXTURESET[((CTachyonBSPMAP *) pResult)->m_dwMeshCount];
					int nTEXCount;

					memset( pTEX, 0x00, ((CTachyonBSPMAP *) pResult)->m_dwMeshCount * sizeof(LPTEXTURESET));
					((CTachyonBSPMAP *) pResult)->m_vTexture.push_back(pTEX);

					file.Read( &nTEXCount, sizeof(int));
					for( int j=0; j<nTEXCount; j++)
					{
						DWORD dwTexID = 0;
						DWORD dwIndex = 0;

						file.Read( &dwIndex, sizeof(DWORD));
						file.Read( &dwTexID, sizeof(DWORD));

						MAPRES::iterator finder = m_mapTEX.find(dwTexID);
						if( finder != m_mapTEX.end() )
							((CTachyonBSPMAP *) pResult)->m_vTexture[i][dwIndex] = (LPTEXTURESET) (*finder).second;
					}
				}

				file.Read( &nCount, sizeof(int));
				MAPTEXTURESET mapLightID;

				for( i=0; i<nCount; i++)
				{
					DWORD dwTexID = 0;
					DWORD dwIndex = 0;

					file.Read( &dwIndex, sizeof(DWORD));
					file.Read( &dwTexID, sizeof(DWORD));

					MAPRES::iterator finder = m_mapTEX.find(dwTexID);
					if( finder != m_mapTEX.end() )
						mapLightID.insert( MAPTEXTURESET::value_type( dwIndex, (LPTEXTURESET) (*finder).second));
					else
						mapLightID.insert( MAPTEXTURESET::value_type( dwIndex, NULL));
				}

				((CTachyonBSPMAP *) pResult)->m_Head.RegulateLight(&mapLightID);
				mapLightID.clear();

				file.Read( &nCount, sizeof(int));
				for( i=0; i<nCount; i++)
				{
					LPMAPOBJ pOBJ = new MAPOBJ();
					DWORD dwInstID;
					DWORD dwOBJID;

					int nCICount = 0;
					int nATTR = 0;

					file.Read( &dwInstID, sizeof(DWORD));
					file.Read( &dwOBJID, sizeof(DWORD));

					file.Read( &pOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
					file.Read( &pOBJ->m_vPOS, sizeof(D3DXVECTOR3));
					file.Read( &pOBJ->m_vROT, sizeof(D3DXVECTOR3));

					file.Read( &pOBJ->m_dwOBJFuncID, sizeof(DWORD));
					file.Read( &pOBJ->m_dwACTFuncID, sizeof(DWORD));
					file.Read( &pOBJ->m_dwANIFuncID, sizeof(DWORD));
					file.Read( &pOBJ->m_dwCurACT, sizeof(DWORD));
					file.Read( &pOBJ->m_dwCurANI, sizeof(DWORD));
					file.Read( &nCICount, sizeof(int));

					for( int j=0; j<nCICount; j++)
					{
						LPCLKINST pCI = new CLKINST();
						DWORD dwID;

						file.Read( &dwID, sizeof(DWORD));
						file.Read( &pCI->m_dwCurCL, sizeof(DWORD));
						file.Read( &pCI->m_dwCurMESH, sizeof(DWORD));

						pOBJ->m_mapCLK.insert( MAPCLKINST::value_type( dwID, pCI));
					}

					file.Read( &nATTR, sizeof(int));
					for( j=0; j<nATTR; j++)
					{
						LPATTRIBUTE pATTR = new ATTRIBUTE();
						DWORD dwATTR;

						file.Read( &dwATTR, sizeof(DWORD));
						file.Read( &pATTR->m_bValueType, sizeof(BYTE));
						file.Read( &pATTR->m_nSize, sizeof(int));
						pATTR->m_pValue = new BYTE[pATTR->m_nSize];
						file.Read( pATTR->m_pValue, pATTR->m_nSize);

						pOBJ->m_mapATTR.insert( MAPATTRIBUTE::value_type( dwATTR, pATTR));
					}

					pOBJ->m_pOBJ = GetOBJ(dwOBJID);
					pResult->m_mapOBJ.insert( MAPMAPOBJ::value_type( dwInstID, pOBJ));
				}

				file.Read( &nCount, sizeof(int));
				for( i=0; i<nCount; i++)
				{
					LPMAPSFX pSFX = new MAPSFX();
					DWORD dwInstID;
					DWORD dwSFXID;

					file.Read( &dwInstID, sizeof(DWORD));
					file.Read( &dwSFXID, sizeof(DWORD));

					file.Read( &pSFX->m_vPOS, sizeof(D3DXVECTOR3));
					file.Read( &pSFX->m_vROT, sizeof(D3DXVECTOR3));
					file.Read( &pSFX->m_bBias, sizeof(BYTE));

					MAPRES::iterator finder = m_mapSFX.find(dwSFXID);
					if( finder != m_mapSFX.end() )
						pSFX->m_pSFX = (LPSFX) (*finder).second;

					pResult->m_mapSFX.insert( MAPMAPSFX::value_type( dwInstID, pSFX));
				}

				file.Read( &nCount, sizeof(int));
				for( i=0; i<nCount; i++)
				{
					LPMAPSND pSND = new MAPSND();
					DWORD dwInstID;

					file.Read( &dwInstID, sizeof(DWORD));
					file.Read( &pSND->m_dwMediaID, sizeof(DWORD));
					file.Read( &pSND->m_vPOS, sizeof(D3DXVECTOR3));

					pResult->m_mapSND.insert( MAPMAPSND::value_type( dwInstID, pSND));
				}
			}

			break;
		}
		pResult->m_bMAPType = bMAPType;

		return pResult;
	}

	return NULL;
}

void CTMonSpawnInspectorRes::LockOBJ( LPOBJECT pOBJ)
{
	if(!pOBJ)
		return;

	MAPCLKIND::iterator itCLK;
	MAPACTION::iterator itACT;

	for( itCLK = pOBJ->m_mapCLKIND.begin(); itCLK != pOBJ->m_mapCLKIND.end(); itCLK++)
	{
		MAPCLOTH::iterator itCL;

		for( itCL = (*itCLK).second->m_mapCLOTH.begin(); itCL != (*itCLK).second->m_mapCLOTH.end(); itCL++)
		{
			MAPMESH::iterator itMESH;

			for( itMESH = (*itCL).second->m_mapMESH.begin(); itMESH != (*itCL).second->m_mapMESH.end(); itMESH++)
			{
				CTachyonMesh* p = (*itMESH).second->m_pMESH;
				p->m_bUseVB = FALSE;
				p->Lock( NULL, &m_vMESHFILE);
			}
		}
	}

	/*for( itACT = pOBJ->m_mapACTION.begin(); itACT != pOBJ->m_mapACTION.end(); itACT++)
	{
	MAPANI::iterator itANI;

	for( itANI = (*itACT).second->m_mapANI.begin(); itANI != (*itACT).second->m_mapANI.end(); itANI++)
	for( int i=0; i<INT((*itANI).second->m_pANI->m_vAni.size()); i++)
	(*itANI).second->m_pANI->m_vAni[i]->m_pAni->Lock( m_pDEVICE->m_pDevice, &m_vANIFILE);
	}*/
}

void CTMonSpawnInspectorRes::LoadOBJ( CString strGroupID)
{
	CString strINDEX;

	int nTotal = 0;
	int nCount = 0;
	int nIndex = 0;

	strINDEX.Format( ".\\Index\\%sO.IDX", strGroupID);
	CFile file( strINDEX, CFile::modeRead|CFile::typeBinary);

	file.Read( &nCount, sizeof(int));
	file.Read( &nTotal, sizeof(int));

	LPMAPRES pTRES = new MAPRES[nCount];
	//RenderBACK( T3DRES_OBJ, 0);
	DWORD dwProgress = 0;
	SetProgressText( "오브젝트 초기화" );
	SetProgressRange( 0, nCount*2+nTotal );

	for( int i=0; i<nCount; i++)
	{
		LoadOBJ( LoadString(&file), &pTRES[i], nIndex, nTotal);

		SetProgressPos( ++dwProgress );
	}

	for( i=0; i<nTotal; i++)
	{
		DWORD dwFileID;
		DWORD dwPOS;
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &dwFileID, sizeof(DWORD));
		file.Read( &dwPOS, sizeof(DWORD));

		MAPRES::iterator finder = pTRES[dwFileID].find(dwPOS);
		if( finder != pTRES[dwFileID].end() )
		{
			m_mapOBJ.insert( MAPRES::value_type( dwID, (*finder).second));
			pTRES[dwFileID].erase(finder);
		}

		SetProgressPos( ++dwProgress );
	}

	for( i=0; i<nCount; i++)
	{
		MAPRES::iterator it;

		for( it = pTRES[i].begin(); it != pTRES[i].end(); it++)
			delete (*it).second;
		pTRES[i].clear();

		SetProgressPos( ++dwProgress );
	}

	delete[] pTRES;
}

void CTMonSpawnInspectorRes::LoadOBJ( CString strFILE,
									 LPMAPRES pTRES,
									 int& nIndex,
									 int nTotal)
{
	CFile file( _T(".\\Data\\") + strFILE, CFile::modeRead|CFile::typeBinary);

	DWORD dwLENGTH = DWORD(file.GetLength());
	DWORD dwPOS = DWORD(file.GetPosition());

	while( dwPOS < dwLENGTH )
	{
		LPOBJECT pOBJ = new OBJECT();
		int nCount = 0;

		file.Read( &pOBJ->m_bPivotCount, sizeof(BYTE));
		file.Read( &nCount, sizeof(int));

		for( int i=0; i<nCount; i++)
		{
			DWORD dwIndex;
			DWORD dwID;

			file.Read( &dwIndex, sizeof(DWORD));
			file.Read( &dwID, sizeof(DWORD));

			pOBJ->m_mapPIVOT.insert( MAPDWORD::value_type( dwID, dwIndex));
		}

		file.Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			SFXINST SFXINST;
			LPSFXINST pSFX = &SFXINST;
			DWORD dwSFX;

			file.Read( &dwSFX, sizeof(DWORD));
			file.Read( &pSFX->m_pSFX, sizeof(DWORD));
			file.Read( &pSFX->m_vPOS, sizeof(D3DXVECTOR3));
			file.Read( &pSFX->m_vROT, sizeof(D3DXVECTOR3));
			file.Read( &pSFX->m_nPivotID, sizeof(int));
			file.Read( &pSFX->m_bBias, sizeof(BYTE));
			file.Read( &pSFX->m_dwFuncID, sizeof(DWORD));
			file.Read( &pSFX->m_dwTick, sizeof(DWORD));
			file.Read( &pSFX->m_bDeleteOnStop, sizeof(BOOL));

			//pOBJ->m_mapSFX.insert( MAPSFXINST::value_type( dwSFX, pSFX));
		}

		file.Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			SNDINST SNDINST;
			LPSNDINST pSND = &SNDINST;
			DWORD dwSND;

			file.Read( &dwSND, sizeof(DWORD));
			file.Read( &pSND->m_dwMediaID, sizeof(DWORD));
			file.Read( &pSND->m_dwFuncID, sizeof(DWORD));
			file.Read( &pSND->m_dwTick, sizeof(DWORD));
			file.Read( &pSND->m_bDeleteOnStop, sizeof(BOOL));

			//pOBJ->m_mapSND.insert( MAPSNDINST::value_type( dwSND, pSND));
		}

		file.Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			LPATTRIBUTE pATTR = new ATTRIBUTE();
			DWORD dwATTR;

			file.Read( &dwATTR, sizeof(DWORD));
			file.Read( &pATTR->m_bValueType, sizeof(BYTE));
			file.Read( &pATTR->m_nSize, sizeof(int));

			pATTR->m_pValue = new BYTE[pATTR->m_nSize];
			file.Read( pATTR->m_pValue, pATTR->m_nSize);

			pOBJ->m_mapATTR.insert( MAPATTRIBUTE::value_type( dwATTR, pATTR));
		}

		file.Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			ACTION ACT;
			LPACTION pACT = &ACT;
			DWORD dwACT = 0;

			int nANI = 0;
			int nSFX = 0;
			int nSND = 0;

			file.Read( &dwACT, sizeof(DWORD));
			file.Read( &nSFX, sizeof(int));
			for( int j=0; j<nSFX; j++)
			{
				SFXINST SFX;
				LPSFXINST pSFX = &SFX;
				DWORD dwSFX;

				file.Read( &dwSFX, sizeof(DWORD));
				file.Read( &pSFX->m_pSFX, sizeof(DWORD));
				file.Read( &pSFX->m_vPOS, sizeof(D3DXVECTOR3));
				file.Read( &pSFX->m_vROT, sizeof(D3DXVECTOR3));
				file.Read( &pSFX->m_nPivotID, sizeof(int));
				file.Read( &pSFX->m_bBias, sizeof(BYTE));
				file.Read( &pSFX->m_dwFuncID, sizeof(DWORD));
				file.Read( &pSFX->m_dwTick, sizeof(DWORD));
				file.Read( &pSFX->m_bDeleteOnStop, sizeof(BOOL));

				//pACT->m_mapSFX.insert( MAPSFXINST::value_type( dwSFX, pSFX));
			}

			file.Read( &nSND, sizeof(int));
			for( j=0; j<nSND; j++)
			{
				SNDINST SNDINST;
				LPSNDINST pSND = &SNDINST;
				DWORD dwSND;

				file.Read( &dwSND, sizeof(DWORD));
				file.Read( &pSND->m_dwMediaID, sizeof(DWORD));
				file.Read( &pSND->m_dwFuncID, sizeof(DWORD));
				file.Read( &pSND->m_dwTick, sizeof(DWORD));
				file.Read( &pSND->m_bDeleteOnStop, sizeof(BOOL));

				//pACT->m_mapSND.insert( MAPSNDINST::value_type( dwSND, pSND));
			}

			file.Read( &nANI, sizeof(int));
			for( j=0; j<nANI; j++)
			{
				ANI ANI;
				LPANI pANI = &ANI;
				DWORD dwANIID;

				file.Read( &dwANIID, sizeof(DWORD));
				file.Read( &pANI->m_pANI, sizeof(DWORD));

				file.Read( &nSFX, sizeof(int));
				for( int k=0; k<nSFX; k++)
				{
					SFXINST SFX;
					LPSFXINST pSFX = &SFX;
					DWORD dwSFX;

					file.Read( &dwSFX, sizeof(DWORD));
					file.Read( &pSFX->m_pSFX, sizeof(DWORD));
					file.Read( &pSFX->m_vPOS, sizeof(D3DXVECTOR3));
					file.Read( &pSFX->m_vROT, sizeof(D3DXVECTOR3));
					file.Read( &pSFX->m_nPivotID, sizeof(int));
					file.Read( &pSFX->m_bBias, sizeof(BYTE));
					file.Read( &pSFX->m_dwFuncID, sizeof(DWORD));
					file.Read( &pSFX->m_dwTick, sizeof(DWORD));
					file.Read( &pSFX->m_bDeleteOnStop, sizeof(BOOL));

					//pANI->m_mapSFX.insert( MAPSFXINST::value_type( dwSFX, pSFX));
				}

				file.Read( &nSND, sizeof(int));
				for( k=0; k<nSND; k++)
				{
					SNDINST SND;
					LPSNDINST pSND = &SND;
					DWORD dwSND;

					file.Read( &dwSND, sizeof(DWORD));
					file.Read( &pSND->m_dwMediaID, sizeof(DWORD));
					file.Read( &pSND->m_dwFuncID, sizeof(DWORD));
					file.Read( &pSND->m_dwTick, sizeof(DWORD));
					file.Read( &pSND->m_bDeleteOnStop, sizeof(BOOL));

					//pANI->m_mapSND.insert( MAPSNDINST::value_type( dwSND, pSND));
				}

				//pACT->m_mapANI.insert( MAPANI::value_type( dwANIID, pANI));
			}
			//pOBJ->m_mapACTION.insert( MAPACTION::value_type( dwACT, pACT));
		}

		file.Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			LPCLKIND pCLK = new CLKIND();
			DWORD dwCLK = 0;
			int nCL = 0;

			file.Read( &dwCLK, sizeof(DWORD));
			file.Read( &nCL, sizeof(int));
			pOBJ->m_mapCLKIND.insert( MAPCLKIND::value_type( dwCLK, pCLK));

			for( int j=0; j<nCL; j++)
			{
				LPCLOTH pCL = new CLOTH();
				DWORD dwCL = 0;
				int nMESH = 0;

				file.Read( &dwCL, sizeof(DWORD));
				file.Read( &nMESH, sizeof(int));
				pCLK->m_mapCLOTH.insert( MAPCLOTH::value_type( dwCL, pCL));

				for( int k=0; k<nMESH; k++)
				{
					LPMESH pMESH = new MESH();
					DWORD dwMESH = 0;
					int nTEX = 0;

					file.Read( &dwMESH, sizeof(DWORD));
					file.Read( &pMESH->m_pMESH, sizeof(DWORD));
					file.Read( &nTEX, sizeof(int));
					pCL->m_mapMESH.insert( MAPMESH::value_type( dwMESH, pMESH));

					for( int l=0; l<nTEX; l++)
					{
						OBJTEX TEX;
						LPOBJTEX pTEX = &TEX;//new OBJTEX();

						file.Read( &pTEX->m_pTEX[0], sizeof(DWORD));
						file.Read( &pTEX->m_pTEX[1], sizeof(DWORD));
						file.Read( &pTEX->m_bType[0], sizeof(BYTE));
						file.Read( &pTEX->m_bType[1], sizeof(BYTE));
						file.Read( &pTEX->m_dwCOLORID, sizeof(DWORD));
						file.Read( &pTEX->m_dwCOLOR, sizeof(DWORD));
						file.Read( &pTEX->m_dwOrder, sizeof(DWORD));
						file.Read( &pTEX->m_dwOP, sizeof(DWORD));
						file.Read( &pTEX->m_dwDESTBlend, sizeof(DWORD));
						file.Read( &pTEX->m_dwSRCBlend, sizeof(DWORD));
						file.Read( &pTEX->m_dwBlendOP, sizeof(DWORD));
						file.Read( &pTEX->m_fIntensity, sizeof(FLOAT));
						file.Read( &pTEX->m_bMinAlpha, sizeof(BYTE));
						file.Read( &pTEX->m_bZEnable, sizeof(BYTE));
						file.Read( &pTEX->m_bZWrite, sizeof(BYTE));
						file.Read( &pTEX->m_b2side, sizeof(BYTE));
						file.Read( &pTEX->m_bUseDirLight, sizeof(BOOL));
						file.Read( &pTEX->m_dwAmbientCOLOR, sizeof(DWORD));

						//pMESH->m_vTEX.push_back(pTEX);
					}
				}
			}
		}
		pOBJ->m_pRES = this;

		pTRES->insert( MAPRES::value_type( dwPOS, pOBJ));
		dwPOS = DWORD(file.GetPosition());
		nIndex++;

		//RenderBACK( T3DRES_OBJ, BYTE(nIndex * 100 / nTotal));
	}
}
