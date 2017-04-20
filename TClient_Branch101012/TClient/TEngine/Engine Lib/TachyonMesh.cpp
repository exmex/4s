// TachyonMesh.cpp: implementation of the CTachyonMeshFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


MAPINDEXEDMESH CTachyonMesh::m_mapVB;
VECTORFILEVB CTachyonMesh::m_vFILEVB;

DWORD CTachyonMesh::m_dwCurrentVB = 0xFFFFFFFF;
DWORD CTachyonMesh::m_dwMaxVB = 0;
FLOAT CTachyonMesh::m_fLevelFactor = 1.0f;
BYTE CTachyonMesh::m_bSoftwareVP = FALSE;


CTachyonMesh::CTachyonMesh()
{
	m_vCenterPoint = D3DXVECTOR3(
		0.0f,
		0.0f,
		0.0f);
	m_bUseVB = TRUE;

	m_bMESHType = MESHTYPE_NONE;
	m_bVBType = VBTYPE_LOCAL;

	m_pMESHIB = NULL;
	m_pBones = NULL;
	m_pDATA = NULL;
	m_pMESH = NULL;
	m_pVB = NULL;
	m_pIB = NULL;

	m_dwNodeCount = 0;
	m_dwMeshCount = 0;
	m_dwVBIndex = 0;
	m_dwInitVB = 0;
	m_dwPOS = 0;
	m_dwREF = 0;

	m_bGroupID = 0;
	m_bFileID = 0;
	m_bVBID = 0;

	m_fRadius = 0.0f;
	m_vDist.clear();
}

CTachyonMesh::~CTachyonMesh()
{
	Release();
}

void CTachyonMesh::InitGlobalVB()
{
	m_vFILEVB.clear();
	m_mapVB.clear();
}

void CTachyonMesh::ReleaseGlobalVB()
{
	while(!m_vFILEVB.empty())
	{
		delete m_vFILEVB.back();
		m_vFILEVB.pop_back();
	}

	m_mapVB.clear();
}

void CTachyonMesh::BeginGlobalDraw( LPDIRECT3DDEVICE9 pDevice)
{
	m_dwCurrentVB = 0xFFFFFFFF;
}

void CTachyonMesh::EndGlobalDraw( LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetSoftwareVertexProcessing(FALSE);
}

LPINDEXEDMESH CTachyonMesh::GetGlobalVB( BYTE bGroupID,
										 BYTE bFileID,
										 BYTE bMESHType,
										 BYTE bVBID)
{
	MAPINDEXEDMESH::iterator finder = m_mapVB.find(GLOBALVB_ID( bGroupID, bFileID, bMESHType, bVBID));

	if( finder != m_mapVB.end() )
		return (*finder).second;

	return NULL;
}

LPINDEXEDMESH CTachyonMesh::GetGlobalVB()
{
	return GetGlobalVB(
		m_bGroupID,
		m_bFileID,
		m_bMESHType,
		m_bVBID);
}

BOOL CTachyonMesh::LoadMesh( LPDIRECT3DDEVICE9 pDevice, LPBYTE pData)
{
	int nTMFVersion = 0;

	if(!pDevice)
		return FALSE;

	nTMFVersion = *((int*) pData); pData += sizeof(int);
	Release();

	switch(nTMFVersion)
	{
	case 300	:
		{
			m_dwNodeCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_fRadius = *((FLOAT *) pData); pData += sizeof(FLOAT);
			m_vCenterPoint = *((LPD3DXVECTOR3) pData); pData += sizeof(D3DXVECTOR3);

			if( m_dwNodeCount > 0 )
			{
				m_pBones = new D3DXMATRIX[m_dwNodeCount];

				for( DWORD i=0; i<m_dwNodeCount; i++)
				{
					D3DXMATRIX vBone;

					memcpy( &vBone, pData, sizeof(D3DMATRIX));
					pData += sizeof(D3DMATRIX);

					D3DXMatrixInverse(
						&m_pBones[i],
						NULL,
						&vBone);
				}
			}
			m_pDATA = new VBDATA();

			m_pDATA->m_dwFVF = m_dwNodeCount > 0 ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX;
			m_pDATA->m_dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);

			switch(m_pDATA->m_dwFVF)
			{
			case T3DFVF_WMESHVERTEX	:
				{
					m_pDATA->m_pVB = new BYTE[m_pDATA->m_dwCount * sizeof(WMESHVERTEX)];

					memcpy( m_pDATA->m_pVB, pData, m_pDATA->m_dwCount * sizeof(WMESHVERTEX));
					pData += m_pDATA->m_dwCount * sizeof(WMESHVERTEX);
				}

				break;

			case T3DFVF_MESHVERTEX	:
				{
					LPWMESHVERTEX pDATA = (LPWMESHVERTEX) pData;
					LPMESHVERTEX pBUF = NULL;

					m_pDATA->m_pVB = new BYTE[m_pDATA->m_dwCount * sizeof(MESHVERTEX)];
					pBUF = (LPMESHVERTEX) m_pDATA->m_pVB;
					pData += m_pDATA->m_dwCount * sizeof(WMESHVERTEX);

					for( DWORD i=0; i<m_pDATA->m_dwCount; i++)
					{
						pBUF[i].m_fNormalX = pDATA[i].m_fNormalX;
						pBUF[i].m_fNormalY = pDATA[i].m_fNormalY;
						pBUF[i].m_fNormalZ = pDATA[i].m_fNormalZ;

						pBUF[i].m_fPosX = pDATA[i].m_fPosX;
						pBUF[i].m_fPosY = pDATA[i].m_fPosY;
						pBUF[i].m_fPosZ = pDATA[i].m_fPosZ;

						pBUF[i].m_fU1 = pDATA[i].m_fU;
						pBUF[i].m_fV1 = pDATA[i].m_fV;
						pBUF[i].m_fU2 = pDATA[i].m_fU;
						pBUF[i].m_fV2 = pDATA[i].m_fV;
					}
				}

				break;
			}

			m_dwMeshCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_pMESH = new VECTORIBINFO[m_dwMeshCount];

			for( DWORD i=0; i<m_dwMeshCount; i++)
			{
				DWORD dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);

				if( dwCount > 0 )
				{
					LPWORD pIB = new WORD[dwCount];

					memcpy( pIB, pData, dwCount * sizeof(WORD));
					pData += dwCount * sizeof(WORD);

					m_pMESH[i].push_back(BuildMeshINFO( pIB, dwCount));
					delete[] pIB;
				}
				else
					m_pMESH[i].push_back(new IBINFO());
			}

			Restore(pDevice);
		}

		break;

	case 301	:
		{
			m_dwNodeCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_fRadius = *((FLOAT *) pData); pData += sizeof(FLOAT);
			m_vCenterPoint = *((LPD3DXVECTOR3) pData); pData += sizeof(D3DXVECTOR3);

			if( m_dwNodeCount > 0 )
			{
				m_pBones = new D3DXMATRIX[m_dwNodeCount];

				for( DWORD i=0; i<m_dwNodeCount; i++)
				{
					D3DXMATRIX vBone;

					memcpy( &vBone, pData, sizeof(D3DMATRIX));
					pData += sizeof(D3DMATRIX);

					D3DXMatrixInverse(
						&m_pBones[i],
						NULL,
						&vBone);
				}
			}
			m_pDATA = new VBDATA();

			m_pDATA->m_dwFVF = m_dwNodeCount > 0 ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX;
			m_pDATA->m_dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);

			switch(m_pDATA->m_dwFVF)
			{
			case T3DFVF_WMESHVERTEX	:
				{
					LPTMESHBUFFER pDATA = (LPTMESHBUFFER) pData;
					LPWMESHVERTEX pBUF = NULL;

					m_pDATA->m_pVB = new BYTE[m_pDATA->m_dwCount * sizeof(WMESHVERTEX)];
					pBUF = (LPWMESHVERTEX) m_pDATA->m_pVB;
					pData += m_pDATA->m_dwCount * sizeof(TMESHBUFFER);

					for( DWORD i=0; i<m_pDATA->m_dwCount; i++)
						memcpy( &pBUF[i], &pDATA[i], sizeof(WMESHVERTEX));
				}

				break;

			case T3DFVF_MESHVERTEX	:
				{
					LPTMESHBUFFER pDATA = (LPTMESHBUFFER) pData;
					LPMESHVERTEX pBUF = NULL;

					m_pDATA->m_pVB = new BYTE[m_pDATA->m_dwCount * sizeof(MESHVERTEX)];
					pBUF = (LPMESHVERTEX) m_pDATA->m_pVB;
					pData += m_pDATA->m_dwCount * sizeof(TMESHBUFFER);

					for( DWORD i=0; i<m_pDATA->m_dwCount; i++)
					{
						pBUF[i].m_fNormalX = pDATA[i].m_fNormalX;
						pBUF[i].m_fNormalY = pDATA[i].m_fNormalY;
						pBUF[i].m_fNormalZ = pDATA[i].m_fNormalZ;

						pBUF[i].m_fPosX = pDATA[i].m_fPosX;
						pBUF[i].m_fPosY = pDATA[i].m_fPosY;
						pBUF[i].m_fPosZ = pDATA[i].m_fPosZ;

						pBUF[i].m_fU1 = pDATA[i].m_fU1;
						pBUF[i].m_fV1 = pDATA[i].m_fV1;
						pBUF[i].m_fU2 = pDATA[i].m_fU2;
						pBUF[i].m_fV2 = pDATA[i].m_fV2;
					}
				}

				break;
			}

			m_dwMeshCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_pMESH = new VECTORIBINFO[m_dwMeshCount];

			for( DWORD i=0; i<m_dwMeshCount; i++)
			{
				DWORD dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);

				if( dwCount > 0 )
				{
					LPWORD pIB = new WORD[dwCount];

					memcpy( pIB, pData, dwCount * sizeof(WORD));
					pData += dwCount * sizeof(WORD);

					m_pMESH[i].push_back(BuildMeshINFO( pIB, dwCount));
					delete[] pIB;
				}
				else
					m_pMESH[i].push_back(new IBINFO());
			}

			Restore(pDevice);
		}

		break;

	default		: return FALSE;
	}

	return TRUE;
}

void CTachyonMesh::Release()
{
	if(m_pMESHIB)
	{
		for( DWORD i=0; i<m_dwMeshCount; i++)
			while(!m_pMESHIB[i].empty())
			{
				delete m_pMESHIB[i].back();
				m_pMESHIB[i].pop_back();
			}

		delete[] m_pMESHIB;
		m_pMESHIB = NULL;
	}

	if(m_pMESH)
	{
		for( DWORD i=0; i<m_dwMeshCount; i++)
			while(!m_pMESH[i].empty())
			{
				delete m_pMESH[i].back();
				m_pMESH[i].pop_back();
			}

		delete[] m_pMESH;
		m_pMESH = NULL;
	}

	if(m_pIB)
	{
		for( int i=0; i<m_dwMeshCount; i++)
			while(!m_pIB[i].empty())
			{
				LPDIRECT3DINDEXBUFFER9 pIB = m_pIB[i].back();
				if(pIB)
					pIB->Release();
				m_pIB[i].pop_back();
			}

		delete[] m_pIB;
		m_pIB = NULL;
	}

	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	if(m_pDATA)
	{
		delete m_pDATA;
		m_pDATA = NULL;
	}

	if(m_pBones)
	{
		delete[] m_pBones;
		m_pBones = NULL;
	}

	m_vCenterPoint = D3DXVECTOR3(
		0.0f,
		0.0f,
		0.0f);

	m_bMESHType = MESHTYPE_NONE;
	m_bVBType = VBTYPE_LOCAL;

	m_dwNodeCount = 0;
	m_dwMeshCount = 0;
	m_dwVBIndex = 0;
	m_dwInitVB = 0;
	m_dwPOS = 0;
	m_dwREF = 0;

	m_bFileID = 0;
	m_bVBID = 0;

	m_fRadius = 0.0f;
	m_vDist.clear();
}

void CTachyonMesh::LoadToGlobalVB( LPDIRECT3DDEVICE9 pDevice)
{
	LPINDEXEDMESH pMESH = GetGlobalVB();

	if( pMESH->m_mapLOCK.find(m_dwVBIndex) == pMESH->m_mapLOCK.end() )
		pMESH->m_mapLOCK.insert( MAPDWORD::value_type( m_dwVBIndex, m_dwVBIndex));

	if(!pMESH->m_vT3DVB.m_pTDATA)
	{
		pMESH->m_vT3DVB.LoadT3DVB(
			pMESH->m_dwVB * (m_dwNodeCount > 0 ? sizeof(WMESHVERTEX) : sizeof(MESHVERTEX)),
			m_dwNodeCount > 0 ? D3DUSAGE_SOFTWAREPROCESSING : 0,
			m_dwNodeCount > 0 ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX);

		pMESH->m_dwKEY++;
	}

	if(!pMESH->m_vT3DIB.m_pTDATA)
	{
		pMESH->m_vT3DIB.LoadT3DIB(
			pMESH->m_dwIB * sizeof(WORD),
			m_dwNodeCount > 0 ? D3DUSAGE_SOFTWAREPROCESSING : 0,
			D3DFMT_INDEX16);
	}

	if( pMESH->m_dwKEY == m_dwInitVB )
		return;
	m_dwInitVB = pMESH->m_dwKEY;

	if(m_pDATA)
	{
		DWORD dwSIZE = m_dwNodeCount > 0 ? sizeof(WMESHVERTEX) : sizeof(MESHVERTEX);

		pMESH->m_vT3DVB.LoadT3DVBDATA(
			m_pDATA->m_pVB,
			m_pDATA->m_dwCount * dwSIZE,
			m_dwVBIndex * dwSIZE);
	}

	for( DWORD i=0; i<m_dwMeshCount; i++)
		for( int j=0; j<INT(m_pMESHIB[i].size()); j++)
			for( int k=0; k<INT(m_pMESHIB[i][j]->m_vIB.size()); k++)
				if(m_pMESHIB[i][j]->m_vIB[k]->m_dwCount)
				{
					pMESH->m_vT3DIB.LoadT3DIBDATA(
						(LPBYTE) m_pMESH[i][j]->m_vIB[k]->m_pIB,
						m_pMESHIB[i][j]->m_vIB[k]->m_dwCount * sizeof(WORD),
						m_pMESHIB[i][j]->m_vIB[k]->m_dwPOS * sizeof(WORD));
				}
}

void CTachyonMesh::Restore( LPDIRECT3DDEVICE9 pDevice)
{
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	if(!m_pIB)
		m_pIB = new VECTORIB[m_dwMeshCount];

	if(m_pDATA)
	{
		LPVOID pBUF = NULL;

		pDevice->CreateVertexBuffer(
			m_pDATA->m_dwCount * (m_dwNodeCount > 0 ? sizeof(WMESHVERTEX) : sizeof(MESHVERTEX)),
			m_dwNodeCount > 0 ? D3DUSAGE_SOFTWAREPROCESSING : 0,
			m_pDATA->m_dwFVF,
			D3DPOOL_MANAGED,
			&m_pVB, NULL);

		m_pVB->Lock( 0, 0, &pBUF, 0);
		memcpy( pBUF, m_pDATA->m_pVB, m_pDATA->m_dwCount * (m_dwNodeCount > 0 ? sizeof(WMESHVERTEX) : sizeof(MESHVERTEX)));
		m_pVB->Unlock();
	}

	for( DWORD i=0; i<m_dwMeshCount; i++)
	{
		while(!m_pIB[i].empty())
		{
			LPDIRECT3DINDEXBUFFER9 pIB = m_pIB[i].back();
			if(pIB)
				pIB->Release();
			m_pIB[i].pop_back();
		}

		for( int j=0; j<INT(m_pMESH[i].size()); j++)
		{
			LPDIRECT3DINDEXBUFFER9 pIB = NULL;
			m_pMESH[i][j]->m_dwCount = 0;

			for( int k=0; k<INT(m_pMESH[i][j]->m_vIB.size()); k++)
				m_pMESH[i][j]->m_dwCount += m_pMESH[i][j]->m_vIB[k]->m_dwCount;

			if( m_pMESH[i][j]->m_dwCount > 0 )
			{
				LPWORD pBUF = NULL;
				DWORD dwCOPY = 0;

				pDevice->CreateIndexBuffer(
					m_pMESH[i][j]->m_dwCount * sizeof(WORD),
					m_dwNodeCount > 0 ? D3DUSAGE_SOFTWAREPROCESSING : 0,
					D3DFMT_INDEX16,
					D3DPOOL_MANAGED,
					&pIB, NULL);

				pIB->Lock( 0, 0, (LPVOID *) &pBUF, 0);
				for( k=0; k<INT(m_pMESH[i][j]->m_vIB.size()); k++)
				{
					memcpy( &pBUF[dwCOPY], m_pMESH[i][j]->m_vIB[k]->m_pIB, m_pMESH[i][j]->m_vIB[k]->m_dwCount * sizeof(WORD));
					dwCOPY += m_pMESH[i][j]->m_vIB[k]->m_dwCount;
				}
				pIB->Unlock();
			}

			m_pIB[i].push_back(pIB);
		}
	}
}

void CTachyonMesh::ReleaseDATA( BYTE bReleaseVB)
{
	if(m_pMESH)
		for( DWORD i=0; i<m_dwMeshCount; i++)
			while(!m_pMESH[i].empty())
			{
				delete m_pMESH[i].back();
				m_pMESH[i].pop_back();
			}

	if( m_pDATA && m_pDATA->m_pVB )
	{
		delete[] m_pDATA->m_pVB;
		m_pDATA->m_pVB = NULL;
	}

	if(m_pBones)
	{
		delete[] m_pBones;
		m_pBones = NULL;
	}

	if( m_bUseVB && bReleaseVB )
	{
		LPINDEXEDMESH pMESH = GetGlobalVB();

		if(pMESH)
		{
			MAPDWORD::iterator finder = pMESH->m_mapLOCK.find(m_dwVBIndex);

			if( finder != pMESH->m_mapLOCK.end() )
				pMESH->m_mapLOCK.erase(finder);

			if(pMESH->m_mapLOCK.empty())
			{
				pMESH->m_vT3DVB.ReleaseDATA();
				pMESH->m_vT3DIB.ReleaseDATA();
			}
		}
	}
}

void CTachyonMesh::Lock( LPDIRECT3DDEVICE9 pDevice,
						 LPVECTORSTRING pFILE)
{
	if(!m_dwREF)
	{
		CFile file( _T(".\\Data\\") + (*pFILE)[m_bFileID], CFile::modeRead|CFile::typeBinary);
		DWORD dwLevel = DWORD(m_vDist.size() + 1);

		file.Seek( m_dwPOS, SEEK_SET);
		ReleaseDATA(FALSE);

		if( m_dwNodeCount > 0 )
		{
			m_pBones = new D3DXMATRIX[m_dwNodeCount];
			file.Read( m_pBones, m_dwNodeCount * sizeof(D3DXMATRIX));
		}

		file.Seek( sizeof(DWORD), SEEK_CUR);
		if( m_pDATA && m_pDATA->m_dwCount )
			switch(m_pDATA->m_dwFVF)
			{
			case T3DFVF_WMESHVERTEX	:
				{
					m_pDATA->m_pVB = new BYTE[m_pDATA->m_dwCount * sizeof(WMESHVERTEX)];
					file.Read( m_pDATA->m_pVB, m_pDATA->m_dwCount * sizeof(WMESHVERTEX));
				}

				break;

			case T3DFVF_MESHVERTEX	:
				{
					m_pDATA->m_pVB = new BYTE[m_pDATA->m_dwCount * sizeof(MESHVERTEX)];
					file.Read( m_pDATA->m_pVB, m_pDATA->m_dwCount * sizeof(MESHVERTEX));
				}

				break;
			}

		for( DWORD i=0; i<m_dwMeshCount; i++)
			for( DWORD j=0; j<dwLevel; j++)
			{
				LPIBINFO pINFO = new IBINFO();

				file.Seek( sizeof(DWORD), SEEK_CUR);
				pINFO->m_dwCount = 0;

				for( DWORD k=0; k<INT(m_pMESHIB[i][j]->m_vIB.size()); k++)
				{
					LPIBDATA pDATA = new IBDATA();

					pDATA->m_dwCount = m_pMESHIB[i][j]->m_vIB[k]->m_dwCount;
					pINFO->m_dwCount += pDATA->m_dwCount;

					file.Seek( sizeof(DWORD), SEEK_CUR);
					file.Read( &pDATA->m_wMatrix, sizeof(WORD));

					if( pDATA->m_dwCount > 0 )
					{
						pDATA->m_pIB = new WORD[pDATA->m_dwCount];
						file.Read( pDATA->m_pIB, pDATA->m_dwCount * sizeof(WORD));
					}

					pINFO->m_vIB.push_back(pDATA);
				}

				m_pMESH[i].push_back(pINFO);
			}

		if(m_bUseVB)
			LoadToGlobalVB(pDevice);
	}

	m_dwREF++;
}

void CTachyonMesh::Unlock()
{
	if(m_dwREF)
	{
		m_dwREF--;

		if(!m_dwREF)
			ReleaseDATA(TRUE);
	}
}

void CTachyonMesh::Render( LPDIRECT3DDEVICE9 pDevice, int nLevel)
{
	pDevice->SetRenderState( D3DRS_VERTEXBLEND, m_dwNodeCount ? D3DVBF_3WEIGHTS : D3DVBF_DISABLE);
	pDevice->SetFVF(m_dwNodeCount ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX);

	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE);
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

	for( DWORD i=0; i<m_dwMeshCount; i++)
		Render( pDevice, i, nLevel);

	pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
}

void CTachyonMesh::Render( LPDIRECT3DDEVICE9 pDevice,
						   DWORD dwIndex,
						   int nLevel)
{
	if( nLevel < 0 )
		nLevel = 0;

	switch(m_bVBType)
	{
	case VBTYPE_GLOBAL	:
		if( m_pMESH[dwIndex][nLevel]->m_dwCount > 0 )
		{
			if( m_dwCurrentVB != GLOBALVB_ID( m_bGroupID, m_bFileID, m_bMESHType, m_bVBID) )
			{
				LPINDEXEDMESH pMESH = GetGlobalVB();

				LPDIRECT3DVERTEXBUFFER9 pVB = pMESH->m_vT3DVB.GetVB();
				LPDIRECT3DINDEXBUFFER9 pIB = pMESH->m_vT3DIB.GetIB();

				if( !pVB || !pIB )
					return;

				pDevice->SetStreamSource( 0, pVB, 0, m_dwNodeCount ? sizeof(WMESHVERTEX) : sizeof(MESHVERTEX));
				pDevice->SetIndices(pIB);
				pDevice->SetSoftwareVertexProcessing(m_dwNodeCount ? TRUE : m_bSoftwareVP);

				m_dwCurrentVB = GLOBALVB_ID( m_bGroupID, m_bFileID, m_bMESHType, m_bVBID);
			}

			pDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, m_dwNodeCount ? TRUE : FALSE);
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				m_dwVBIndex, 0,
				m_pDATA->m_dwCount,
				m_pMESHIB[dwIndex][nLevel]->m_vIB[0]->m_dwPOS,
				m_pMESH[dwIndex][nLevel]->m_dwCount / 3);
			pDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		}

		break;

	case VBTYPE_LOCAL	:
		if( m_pMESH[dwIndex][nLevel]->m_dwCount > 0 )
		{
			pDevice->SetStreamSource( 0, m_pVB, 0, m_dwNodeCount ? sizeof(WMESHVERTEX) : sizeof(MESHVERTEX));
			pDevice->SetIndices(m_pIB[dwIndex][nLevel]);
			pDevice->SetSoftwareVertexProcessing(m_dwNodeCount ? TRUE : m_bSoftwareVP);

			pDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, m_dwNodeCount ? TRUE : FALSE);
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0, 0,
				m_pDATA->m_dwCount, 0,
				m_pMESH[dwIndex][nLevel]->m_dwCount / 3);

			pDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
			pDevice->SetSoftwareVertexProcessing(FALSE);
		}

		break;
	}
}

int CTachyonMesh::HitTest( CD3DCamera *pCam,
						   int nPosX,
						   int nPosY,
						   int nLevel)
{
	D3DXVECTOR3 vPos = pCam->GetCameraPosition( nPosX, nPosY);
	D3DXVECTOR3 vDir = pCam->GetRayDirection( nPosX, nPosY);

	FLOAT fDist = 0.0f;
	int nResult = -1;

	if( !D3DXSphereBoundProbe( &m_vCenterPoint, m_fRadius, &vPos, &vDir) ||
		!m_pDATA || !m_pDATA->m_dwCount )
		return nResult;

	LPD3DXVECTOR3 vPoint = new D3DXVECTOR3[m_pDATA->m_dwCount];
	if( m_dwNodeCount > 0 )
	{
		LPWMESHVERTEX pDATA = (LPWMESHVERTEX) m_pDATA->m_pVB;

		for( int i=0; i<m_pDATA->m_dwCount; i++)
		{
			vPoint[i] = D3DXVECTOR3(
				pDATA[i].m_fPosX,
				pDATA[i].m_fPosY,
				pDATA[i].m_fPosZ);
		}
	}
	else
	{
		LPMESHVERTEX pDATA = (LPMESHVERTEX) m_pDATA->m_pVB;

		for( int i=0; i<m_pDATA->m_dwCount; i++)
		{
			vPoint[i] = D3DXVECTOR3(
				pDATA[i].m_fPosX,
				pDATA[i].m_fPosY,
				pDATA[i].m_fPosZ);
		}
	}

	for( int i=0; i<m_dwMeshCount; i++)
	{
		nLevel = min( nLevel, INT(m_pMESH[i].size()) - 1);

		for( int j=0; j<INT(m_pMESH[i][nLevel]->m_vIB.size()); j++)
			for( int k=0; k<m_pMESH[i][nLevel]->m_vIB[j]->m_dwCount / 3; k++)
			{
				D3DXVECTOR3 vCross;

				FLOAT fCurDist = 0.0f;
				FLOAT fU;
				FLOAT fV;

				D3DXVec3Cross(
					&vCross,
					&(vPoint[m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k + 1]] - vPoint[m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k]]),
					&(vPoint[m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k + 2]] - vPoint[m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k]]));

				if( D3DXIntersectTri(
					&vPoint[m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k]],
					&vPoint[m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k + 1]],
					&vPoint[m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k + 2]],
					&vPos, &vDir,
					&fU, &fV,
					&fCurDist) &&
					D3DXVec3Dot(
					&vCross,
					&vDir) < 0.0f && (
					nResult == -1 ||
					fCurDist < fDist))
				{
					fDist = fCurDist;
					nResult = i;
				}
			}
	}
	delete[] vPoint;

	return nResult;
}

int CTachyonMesh::GetLevel( FLOAT fDist)
{
	int nCount = INT(m_vDist.size());
	int nResult = 0;

	for( int i=0; i<nCount; i++)
		if( fDist > m_fLevelFactor * m_vDist[i] )
			nResult = i + 1;

	return min( nResult, nCount);
}

LPIBINFO CTachyonMesh::BuildMeshINFO( LPVECTORDWORD pMESH)
{
	DWORD dwNodeCount = m_dwNodeCount + 1;
	LPIBINFO pINFO = new IBINFO();

	if( !pMESH || pMESH->empty() )
		return pINFO;

	LPVECTORDWORD pBUF = new VECTORDWORD[dwNodeCount];
	VECTORDWORD vBLEND;

	for( DWORD i=0; i<dwNodeCount; i++)
		pBUF[i].clear();
	vBLEND.clear();

	for( i=0; i<DWORD(pMESH->size() / 3); i++)
	{
		WORD wMatrix = 0xFFFF;

		if( m_dwNodeCount > 0 )
		{
			LPWMESHVERTEX pDATA = (LPWMESHVERTEX) m_pDATA->m_pVB;

			for( int j=0; j<3; j++)
			{
				if( pDATA[(*pMESH)[3 * i + j]].m_fWeight[0] < 0.9f || (
					wMatrix != 0xFFFF &&
					wMatrix != MATRIXID_1ST(pDATA[(*pMESH)[3 * i + j]].m_dwMatIndex)) )
				{
					wMatrix = 0xFFFF;
					break;
				}

				wMatrix = WORD(MATRIXID_1ST(pDATA[(*pMESH)[3 * i + j]].m_dwMatIndex));
			}
		}
		else
			wMatrix = 0;

		if( wMatrix != 0xFFFF )
		{
			for( int j=0; j<3; j++)
				pBUF[wMatrix].push_back((*pMESH)[3 * i + j]);
		}
		else
		{
			for( int j=0; j<3; j++)
				vBLEND.push_back((*pMESH)[3 * i + j]);
		}
	}

	for( i=0; i<dwNodeCount; i++)
	{
		if(!pBUF[i].empty())
		{
			LPIBDATA pDATA = new IBDATA();

			pDATA->m_dwCount = DWORD(pBUF[i].size());
			pDATA->m_pIB = new WORD[pDATA->m_dwCount];
			pDATA->m_wMatrix = WORD(i);

			for( DWORD j=0; j<pDATA->m_dwCount; j++)
				pDATA->m_pIB[j] = WORD(pBUF[i][j]);

			pINFO->m_vIB.push_back(pDATA);
		}

		pBUF[i].clear();
	}

	if(!vBLEND.empty())
	{
		LPIBDATA pDATA = new IBDATA();

		pDATA->m_dwCount = DWORD(vBLEND.size());
		pDATA->m_pIB = new WORD[pDATA->m_dwCount];
		pDATA->m_wMatrix = 0xFFFF;

		for( i=0; i<pDATA->m_dwCount; i++)
			pDATA->m_pIB[i] = WORD(vBLEND[i]);

		pINFO->m_vIB.push_back(pDATA);
	}

	vBLEND.clear();
	delete[] pBUF;

	return pINFO;
}

LPIBINFO CTachyonMesh::BuildMeshINFO( LPWORD pMESH,
									  DWORD dwCount)
{
	DWORD dwNodeCount = m_dwNodeCount + 1;
	LPIBINFO pINFO = new IBINFO();

	if(!dwCount)
		return pINFO;

	LPVECTORDWORD pBUF = new VECTORDWORD[dwNodeCount];
	VECTORDWORD vBLEND;

	for( DWORD i=0; i<dwNodeCount; i++)
		pBUF[i].clear();
	vBLEND.clear();

	for( i=0; i<dwCount / 3; i++)
	{
		WORD wMatrix = 0xFFFF;

		if( m_dwNodeCount > 0 )
		{
			LPWMESHVERTEX pDATA = (LPWMESHVERTEX) m_pDATA->m_pVB;

			for( int j=0; j<3; j++)
			{
				if( pDATA[pMESH[3 * i + j]].m_fWeight[0] < 0.9f || (
					wMatrix != 0xFFFF &&
					wMatrix != MATRIXID_1ST(pDATA[pMESH[3 * i + j]].m_dwMatIndex)) )
				{
					wMatrix = 0xFFFF;
					break;
				}

				wMatrix = WORD(MATRIXID_1ST(pDATA[pMESH[3 * i + j]].m_dwMatIndex));
			}
		}
		else
			wMatrix = 0;

		if( wMatrix != 0xFFFF )
		{
			for( int j=0; j<3; j++)
				pBUF[wMatrix].push_back(pMESH[3 * i + j]);
		}
		else
		{
			for( int j=0; j<3; j++)
				vBLEND.push_back(pMESH[3 * i + j]);
		}
	}

	for( i=0; i<dwNodeCount; i++)
	{
		if(!pBUF[i].empty())
		{
			LPIBDATA pDATA = new IBDATA();

			pDATA->m_dwCount = DWORD(pBUF[i].size());
			pDATA->m_pIB = new WORD[pDATA->m_dwCount];
			pDATA->m_wMatrix = WORD(i);

			for( DWORD j=0; j<pDATA->m_dwCount; j++)
				pDATA->m_pIB[j] = WORD(pBUF[i][j]);

			pINFO->m_vIB.push_back(pDATA);
		}

		pBUF[i].clear();
	}

	if(!vBLEND.empty())
	{
		LPIBDATA pDATA = new IBDATA();

		pDATA->m_dwCount = DWORD(vBLEND.size());
		pDATA->m_pIB = new WORD[pDATA->m_dwCount];
		pDATA->m_wMatrix = 0xFFFF;

		for( i=0; i<pDATA->m_dwCount; i++)
			pDATA->m_pIB[i] = WORD(vBLEND[i]);

		pINFO->m_vIB.push_back(pDATA);
	}

	vBLEND.clear();
	delete[] pBUF;

	return pINFO;
}
