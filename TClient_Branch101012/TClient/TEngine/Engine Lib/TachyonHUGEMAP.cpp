#include "StdAfx.h"

FLOAT CTachyonHUGEMAP::m_fCullDIST = 192.0f;

int CTachyonHUGEMAP::m_nDiffuseCount = 10;
int CTachyonHUGEMAP::m_nLODBound = 0;
int CTachyonHUGEMAP::m_nIndexCount = 0;

int CTachyonHUGEMAP::m_nMaxLength = 0;
int CTachyonHUGEMAP::m_nMaxLevel = 10;


CTachyonHUGEMAP::CTachyonHUGEMAP()
{
	for( BYTE i=0; i<9; i++)
	{
		m_mapOBJECT[i].clear();
		m_mapMAPSFX[i].clear();
		m_mapMAPSND[i].clear();

		m_bUPDATE[i] = FALSE;
		m_bLOAD[i] = FALSE;
		m_bLOCK[i] = FALSE;
		m_pUNIT[i] = NULL;

		m_bINDEX[i] = 0xFF;
		m_bUNIT[i] = 0xFF;
	}

	m_vLIGHT.x = 0.0f;
	m_vLIGHT.y = 0.0f;
	m_vLIGHT.z = 0.0f;

	m_fSpecRange = 256.0f;
	m_fIntensity = 35.0f;
	m_fSpecMax = 0.7f;

	m_bDrawShadow = TRUE;
	m_bUseSHADER = FALSE;
	m_bResetVB = FALSE;
	m_bSHADOW = 0x60;

	m_nTextureSize = 512;
	m_nShadowPixel = 1;
	m_nTileLength = 0;
	m_nUnitLength = 0;
	m_nUnitCountX = 0;
	m_nUnitCountZ = 0;
	m_nBlockCount = 0;
	m_nCellCount = 0;

	m_nUnitX = -2;
	m_nUnitZ = -2;
	m_nCellX = -2;
	m_nCellZ = -2;

	m_vDETAILSDATA.clear();
	m_vDETAILDATA.clear();

	m_vDETAILS.clear();
	m_vDETAIL.clear();

	m_mapDIFFUSEINDEX.clear();
	m_vDIFFUSELOCK.clear();
	m_vDIFFUSEBUF.clear();
	m_vDIFFUSEID.clear();
	m_vDIFFUSE.clear();

	m_vBLENDTILE.clear();
	m_vBLOCK.clear();
	m_vBLEND.clear();
	m_vCOUNT.clear();

	m_vBSTART.clear();
	m_vSTART.clear();

	m_pSHADOWTEX = NULL;
	m_pTILETEX = NULL;
	m_pGEOTEX = NULL;

	m_pINDEXBUF = NULL;
	m_pINDEX = NULL;
	m_pENABLE = NULL;
	m_pBLOCK = NULL;

	m_pPOSVB = NULL;
	m_pUVVB = NULL;
	m_pRES = NULL;
	m_dwID = 0;

	m_fDIR = 2.0f * D3DX_PI;
	m_vCENTER.m_dwID = 0;
	m_vPIVOT.m_dwID = 0;

	D3DXMatrixIdentity(&m_vSHADOWTRANS);
}

CTachyonHUGEMAP::~CTachyonHUGEMAP()
{
	m_vDETAILSDATA.clear();
	m_vDETAILDATA.clear();

	m_vDIFFUSEID.clear();
	m_vDETAILS.clear();
	m_vDETAIL.clear();

	m_vBLEND.clear();
	m_vCOUNT.clear();

	m_vBSTART.clear();
	m_vSTART.clear();

	Release();
}

void CTachyonHUGEMAP::InitVB( CD3DDevice *pDevice)
{
	int nLength = m_nUnitLength / m_nCellCount;
	int nCount = nLength + 1;
	Release();

	for( int i=0; i<9; i++)
	{
		CRect rect( 0, 0, nLength, nLength);

		rect.OffsetRect(
			-nLength / 2,
			-nLength / 2);

		rect.OffsetRect(
			(i % 3 - 1) * nLength,
			(i / 3 - 1) * nLength);

		BuildBLOCK(rect);
	}

	m_pENABLE = new BYTE[m_nUnitCountX * m_nUnitCountZ];
	m_pBLOCK = new LPBLOCK[INT(m_vBLOCK.size())];
	ComplateBLOCK();

	nLength = 9 * nLength * nLength;
	nCount = 9 * nCount * nCount;
	m_nBlockCount = 0;

	for( i=0; i<m_nDiffuseCount; i++)
	{
		LPDIRECT3DVERTEXBUFFER9 pVB = NULL;

		pDevice->m_pDevice->CreateVertexBuffer(
			nCount * sizeof(DWORD),
			0, D3DFVF_DIFFUSE,
			D3DPOOL_MANAGED,
			&pVB, NULL);

		m_vDIFFUSEBUF.push_back(new DWORD[nCount]);
		m_vDIFFUSE.push_back(pVB);
		m_vDIFFUSELOCK.push_back(0);
		m_vBLENDTILE.push_back(new BYTE[nLength]);

		memset( LPDWORD(m_vDIFFUSEBUF.back()), 0x00000000, nCount * sizeof(DWORD));
		memset( LPBYTE(m_vBLENDTILE.back()), 0xFF, nLength * sizeof(BYTE));
	}
	InitForNULLTILE();

	pDevice->m_pDevice->CreateIndexBuffer(
		pDevice->m_vCAPS.MaxVertexIndex > 0x0000FFFF ?
		m_nIndexCount * sizeof(DWORD) :
		m_nIndexCount * sizeof(WORD),
		D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,
		pDevice->m_vCAPS.MaxVertexIndex > 0x0000FFFF ?
		D3DFMT_INDEX32 : D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&m_pINDEX, NULL);

	m_pINDEXBUF = pDevice->m_vCAPS.MaxVertexIndex > 0x0000FFFF ?
		(LPVOID) new DWORD[m_nIndexCount] :
		(LPVOID) new WORD[m_nIndexCount];

	pDevice->m_pDevice->CreateVertexBuffer(
		nCount * sizeof(D3DXVECTOR3),
		0, D3DFVF_XYZ,
		D3DPOOL_MANAGED,
		&m_pPOSVB, NULL);

	pDevice->m_pDevice->CreateVertexBuffer(
		nCount * sizeof(D3DXVECTOR2),
		0, D3DFVF_TEX1,
		D3DPOOL_MANAGED,
		&m_pUVVB, NULL);
}

void CTachyonHUGEMAP::Release()
{
	for( BYTE i=0; i<9; i++)
	{
		MAPOBJECT::iterator itOBJ;
		MAPDWORD::iterator itSFX;
		MAPSNDIDX::iterator itSND;

		for( itOBJ = m_mapOBJECT[i].begin(); itOBJ != m_mapOBJECT[i].end(); itOBJ++)
			if((*itOBJ).second)
				delete (*itOBJ).second;
		m_mapOBJECT[i].clear();

		for( itSFX = m_mapMAPSFX[i].begin(); itSFX != m_mapMAPSFX[i].end(); itSFX++)
			CTachyonSFX::Release((*itSFX).second);
		m_mapMAPSFX[i].clear();

		for( itSND = m_mapMAPSND[i].begin(); itSND != m_mapMAPSND[i].end(); itSND++)
		{
			LPSNDIDX pIDX = (*itSND).second;

			if( pIDX && pIDX->m_pSND )
			{
				pIDX->m_pSND->Stop(pIDX->m_nIndex);
				pIDX->m_pSND->Unlock(pIDX->m_nIndex);
			}

			delete pIDX;
		}
		m_mapMAPSND[i].clear();

		m_bUPDATE[i] = FALSE;
		m_bLOAD[i] = FALSE;
		m_bLOCK[i] = FALSE;
		m_pUNIT[i] = NULL;

		m_bINDEX[i] = 0xFF;
		m_bUNIT[i] = 0xFF;
	}

	while(!m_vBLENDTILE.empty())
	{
		delete[] m_vBLENDTILE.back();
		m_vBLENDTILE.pop_back();
	}

	while(!m_vDIFFUSEBUF.empty())
	{
		delete[] m_vDIFFUSEBUF.back();
		m_vDIFFUSEBUF.pop_back();
	}

	while(!m_vDIFFUSE.empty())
	{
		m_vDIFFUSE.back()->Release();
		m_vDIFFUSE.pop_back();
	}

	while(!m_vBLOCK.empty())
	{
		delete m_vBLOCK.back();
		m_vBLOCK.pop_back();
	}

	if(m_pBLOCK)
	{
		delete[] m_pBLOCK;
		m_pBLOCK = NULL;
	}

	if(m_pINDEXBUF)
	{
		delete[] m_pINDEXBUF;
		m_pINDEXBUF = NULL;
	}

	if(m_pINDEX)
	{
		m_pINDEX->Release();
		m_pINDEX = NULL;
	}

	if(m_pPOSVB)
	{
		m_pPOSVB->Release();
		m_pPOSVB = NULL;
	}

	if(m_pUVVB)
	{
		m_pUVVB->Release();
		m_pUVVB = NULL;
	}

	if(m_pSHADOWTEX)
	{
		m_pSHADOWTEX->Release();
		m_pSHADOWTEX = NULL;
	}

	if(m_pTILETEX)
	{
		m_pTILETEX->Release();
		m_pTILETEX = NULL;
	}

	if(m_pGEOTEX)
	{
		m_pGEOTEX->Release();
		m_pGEOTEX = NULL;
	}

	if(m_pENABLE)
	{
		delete[] m_pENABLE;
		m_pENABLE = NULL;
	}

	m_mapDIFFUSEINDEX.clear();
	m_vDIFFUSELOCK.clear();

	m_fDIR = 2.0f * D3DX_PI;
	m_vCENTER.m_dwID = 0;
	m_vPIVOT.m_dwID = 0;
	m_bResetVB = FALSE;

	D3DXMatrixIdentity(&m_vSHADOWTRANS);
	m_nBlockCount = 0;
	m_nUnitX = -2;
	m_nUnitZ = -2;
	m_nCellX = -2;
	m_nCellZ = -2;
}

void CTachyonHUGEMAP::DeleteForRestore()
{
	if(m_pINDEX)
	{
		m_pINDEX->Release();
		m_pINDEX = NULL;
	}
}

void CTachyonHUGEMAP::Restore( CD3DDevice *pDevice, CD3DCamera *pCamera)
{
	LPVOID pBUF = NULL;

	pDevice->m_pDevice->CreateIndexBuffer(
		pDevice->m_vCAPS.MaxVertexIndex > 0x0000FFFF ?
		m_nIndexCount * sizeof(DWORD) :
		m_nIndexCount * sizeof(WORD),
		D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,
		pDevice->m_vCAPS.MaxVertexIndex > 0x0000FFFF ?
		D3DFMT_INDEX32 : D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&m_pINDEX, NULL);

	m_pINDEX->Lock( 0, 0, &pBUF, NULL);
	memcpy( pBUF, m_pINDEXBUF, m_nIndexCount * (pDevice->m_vCAPS.MaxVertexIndex > 0x0000FFFF ? sizeof(DWORD) : sizeof(WORD)));
	m_pINDEX->Unlock();

	m_bResetVB = TRUE;
}

void CTachyonHUGEMAP::InitForNULLTILE()
{
	LPDWORD pDIFFUSE = (LPDWORD) m_vDIFFUSEBUF[0];

	int nLength = m_nUnitLength / m_nCellCount;
	int nCount = nLength + 1;

	nLength = 9 * nLength * nLength;
	nCount = 9 * nCount * nCount;

	for( int i=0; i<nCount; i++)
		pDIFFUSE[i] = 0xFF000000;

	m_vDIFFUSE[0]->Lock( 0, 0, (LPVOID *) &pDIFFUSE, 0);
	memcpy( pDIFFUSE, m_vDIFFUSEBUF[0], nCount * sizeof(DWORD));
	m_vDIFFUSE[0]->Unlock();

	m_mapDIFFUSEINDEX.insert( MAPDWORD::value_type( TILE_NULL, 0));
	m_vDIFFUSELOCK[0] = 0x000001FF;
	memset( LPBYTE(m_vBLENDTILE[0]), 0xFF, nLength * sizeof(BYTE));
}

void CTachyonHUGEMAP::ResetTEX( LPDIRECT3DDEVICE9 pDevice,
								LPIPOINT pCENTER)
{
	LPDIRECT3DSURFACE9 pTTILE = NULL;
	LPDIRECT3DSURFACE9 pTIMG = NULL;

	if( m_bDrawShadow && !m_pSHADOWTEX )
	{
		D3DXCreateTexture(
			pDevice,
			SHADOWTEX_SIZE,
			SHADOWTEX_SIZE,
			1, 0,
			D3DFMT_A8,
			D3DPOOL_MANAGED,
			&m_pSHADOWTEX);
	}

	if(!m_pTILETEX)
	{
		D3DXCreateTexture(
			pDevice,
			m_nTextureSize,
			m_nTextureSize,
			1, 0,
			D3DFMT_R5G6B5,
			D3DPOOL_MANAGED,
			&m_pTILETEX);
	}

	if(!m_pGEOTEX)
	{
		D3DXCreateTexture(
			pDevice,
			m_nTextureSize,
			m_nTextureSize,
			1, 0,
			D3DFMT_R5G6B5,
			D3DPOOL_MANAGED,
			&m_pGEOTEX);
	}

	VECTORBYTE vUNIT;
	vUNIT.clear();

	CRect rect(
		m_nCellX * m_nUnitLength / m_nCellCount,
		m_nCellZ * m_nUnitLength / m_nCellCount,
		(m_nCellX + 3) * m_nUnitLength / m_nCellCount,
		(m_nCellZ + 3) * m_nUnitLength / m_nCellCount);

	m_pTILETEX->GetSurfaceLevel( 0, &pTTILE);
	m_pGEOTEX->GetSurfaceLevel( 0, &pTIMG);

	for( BYTE i=0; i<9; i++)
		if(m_pUNIT[i])
		{
			CRect unit(
				(m_nUnitX + i % 3) * m_nUnitLength,
				(m_nUnitZ + i / 3) * m_nUnitLength,
				(m_nUnitX + i % 3 + 1) * m_nUnitLength,
				(m_nUnitZ + i / 3 + 1) * m_nUnitLength);
			CRect joint;

			if(joint.IntersectRect( &rect, &unit))
			{
				CRect vTDEST(&joint);
				CRect vTSRC(&joint);

				vTDEST.OffsetRect(
					-rect.left,
					-rect.top);

				vTSRC.OffsetRect(
					-unit.left,
					-unit.top);

				vTDEST.MulDiv( m_nTextureSize, m_nUnitLength);
				vTSRC.MulDiv( m_nTextureSize, m_nUnitLength);

				D3DXLoadSurfaceFromSurface(
					pTTILE,
					NULL,
					&vTDEST,
					m_pUNIT[i]->m_pTILETEX,
					NULL,
					&vTSRC,
					D3DX_FILTER_NONE,
					NULL);

				D3DXLoadSurfaceFromSurface(
					pTIMG,
					NULL,
					&vTDEST,
					m_pUNIT[i]->m_pGEOTEX,
					NULL,
					&vTSRC,
					D3DX_FILTER_NONE,
					NULL);
				vUNIT.push_back(i);
			}
		}

	pTTILE->Release();
	pTIMG->Release();

	///////////////////////////////////////////////////////////////////
	// build shadow texture
	if(m_bDrawShadow)
	{
		int nTextureCount = m_nUnitLength * m_nShadowPixel;

		nTextureCount = nTextureCount / SHADOWTEX_SIZE + (nTextureCount % SHADOWTEX_SIZE ? 1 : 0);
		m_pSHADOWTEX->GetSurfaceLevel( 0, &pTIMG);

		rect.SetRect(
			0, 0,
			SHADOWTEX_SIZE,
			SHADOWTEX_SIZE);

		rect.OffsetRect(
			pCENTER->m_nX * m_nShadowPixel - SHADOWTEX_SIZE / 2,
			pCENTER->m_nY * m_nShadowPixel - SHADOWTEX_SIZE / 2);

		for( i=0; i<INT(vUNIT.size()); i++)
		{
			int nPosX = (m_nUnitX + vUNIT[i] % 3) * m_nUnitLength * m_nShadowPixel;
			int nPosZ = (m_nUnitZ + vUNIT[i] / 3) * m_nUnitLength * m_nShadowPixel;

			for( int j=0; j<INT(m_pUNIT[vUNIT[i]]->m_vSHADOWTEX.size()); j++)
			{
				CRect unit(
					0, 0,
					SHADOWTEX_SIZE,
					SHADOWTEX_SIZE);
				CRect joint;

				unit.OffsetRect(
					nPosX + (j % nTextureCount) * SHADOWTEX_SIZE,
					nPosZ + (j / nTextureCount) * SHADOWTEX_SIZE);

				if(joint.IntersectRect( &rect, &unit))
				{
					CRect vTDEST(&joint);
					CRect vTSRC(&joint);

					vTDEST.OffsetRect(
						-rect.left,
						-rect.top);

					joint.top = SHADOWTEX_SIZE - vTDEST.bottom;
					vTDEST.bottom = SHADOWTEX_SIZE - vTDEST.top;
					vTDEST.top = joint.top;

					vTSRC.OffsetRect(
						-unit.left,
						-unit.top);

					joint.top = SHADOWTEX_SIZE - vTSRC.bottom;
					vTSRC.bottom = SHADOWTEX_SIZE - vTSRC.top;
					vTSRC.top = joint.top;

					D3DXLoadSurfaceFromSurface(
						pTIMG,
						NULL,
						&vTDEST,
						m_pUNIT[vUNIT[i]]->m_vSHADOWTEX[j],
						NULL,
						&vTSRC,
						D3DX_FILTER_NONE,
						NULL);
				}
			}
		}

		FLOAT fSCALE = FLOAT(m_nShadowPixel * m_nUnitLength) / FLOAT(SHADOWTEX_SIZE);
		FLOAT fU = fSCALE * FLOAT(m_nCellX) / FLOAT(m_nCellCount) - FLOAT(pCENTER->m_nX * m_nShadowPixel) / FLOAT(SHADOWTEX_SIZE) + 0.5f;
		FLOAT fV = FLOAT(pCENTER->m_nY * m_nShadowPixel) / FLOAT(SHADOWTEX_SIZE) - fSCALE * FLOAT(m_nCellZ) / FLOAT(m_nCellCount) + 0.5f;

		m_vSHADOWTRANS._11 = fSCALE;
		m_vSHADOWTRANS._22 = -fSCALE;

		m_vSHADOWTRANS._31 = fU;
		m_vSHADOWTRANS._32 = fV;

		pTIMG->Release();
	}

	vUNIT.clear();
}

void CTachyonHUGEMAP::Render( CD3DDevice *pDevice)
{
	static D3DXMATRIX vWorld(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	static FLOAT vCONST[16];

	int nLength = m_nUnitLength / m_nCellCount + 1;
	int nCount = 9 * nLength * nLength;
	int nTotal = 0;

	FLOAT fDetailU = FLOAT(m_nCellX) / FLOAT(m_nCellCount);
	FLOAT fDetailV = FLOAT(m_nCellZ) / FLOAT(m_nCellCount);

	FLOAT fDepthBias = FLOAT(m_vDETAIL.size() + 1) * EXTBIAS_BASE;
	FLOAT fMipBias = 0.0f;

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &fMipBias));
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

	pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);

	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000000);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	pDevice->m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	nLength *= nLength;

	vWorld._11 = 1.0f;
	vWorld._22 = 1.0f;
	vWorld._31 = 0.0f;
	vWorld._32 = 0.0f;

	if( m_bUseSHADER && pDevice->m_bEnableSHADER )
	{
		D3DXMatrixTranspose( (LPD3DXMATRIX) vCONST, &vWorld);
		pDevice->m_pDevice->SetVertexShaderConstantF(
			pDevice->m_vConstantVS[VC_WORLD],
			vCONST, 3);

		pDevice->m_pDevice->SetVertexDeclaration(pDevice->m_pDECL[VS_LVERTEX]);
		pDevice->m_pDevice->SetVertexShader(pDevice->m_pVertexShader[VS_LVERTEX]);
	}
	else
	{
		pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vWorld);
		pDevice->m_pDevice->SetFVF(T3DFVF_LVERTEX);

		pDevice->m_pDevice->SetVertexDeclaration(pDevice->m_pDECL[VS_LVERTEX]);
	}

	pDevice->m_pDevice->SetStreamSource( 0, m_pPOSVB, 0, sizeof(D3DXVECTOR3));
	pDevice->m_pDevice->SetStreamSource( 2, m_pUVVB, 0, sizeof(D3DXVECTOR2));

	pDevice->m_pDevice->SetTexture( 1, m_pGEOTEX);
	pDevice->m_pDevice->SetIndices(m_pINDEX);

	for( int i=0; i<INT(m_vDETAIL.size()); i++)
	{
		if(m_vDETAIL[i])
			m_vDETAIL[i]->m_dwCurTick = 0;
		LPDIRECT3DTEXTURE9 pTexture = m_vDETAIL[i] ? m_vDETAIL[i]->GetTexture() : NULL;

		pDevice->m_pDevice->SetStreamSource( 1, m_vDIFFUSE[m_vDIFFUSEID[i]], 0, sizeof(DWORD));
		pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
		fDepthBias -= EXTBIAS_BASE;
		vWorld._11 = m_vDETAILS[i];
		vWorld._22 = m_vDETAILS[i];
		vWorld._31 = m_vDETAILS[i] * fDetailU;
		vWorld._32 = m_vDETAILS[i] * fDetailV;

		if( m_bUseSHADER && pDevice->m_bEnableSHADER )
		{
			PSTYPE nPS = pTexture ? PS_DETAILMAP : PS_MAP;

			pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, pTexture ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
			pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, pTexture ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);

			if(pTexture)
			{
				D3DXMatrixTranspose( (LPD3DXMATRIX) vCONST, &vWorld);
				pDevice->m_pDevice->SetVertexShaderConstantF(
					pDevice->m_vConstantVS[VC_TEXTRAN0],
					vCONST, 2);
			}

			vWorld._11 = 1.0f;
			vWorld._22 = 1.0f;
			vWorld._31 = 0.0f;
			vWorld._32 = 0.0f;

			if(!pTexture)
			{
				D3DXMatrixTranspose( (LPD3DXMATRIX) vCONST, &vWorld);
				pDevice->m_pDevice->SetVertexShaderConstantF(
					pDevice->m_vConstantVS[VC_TEXTRAN0],
					vCONST, 2);
			}

			pDevice->m_pDevice->SetVertexShaderConstantF(
				pDevice->m_vConstantVS[VC_TEXTRAN1],
				(FLOAT *) &vWorld, 2);

			pDevice->m_pDevice->SetTexture( 0, pTexture ? pTexture : m_pTILETEX);
			pDevice->m_pDevice->SetPixelShader(pDevice->m_pPixelShader[nPS]);
		}
		else if(pTexture)
		{
			pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE);
			pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &vWorld);
			vWorld._11 = 1.0f;
			vWorld._22 = 1.0f;
			vWorld._31 = 0.0f;
			vWorld._32 = 0.0f;
			pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE1, &vWorld);
			pDevice->m_pDevice->SetTexture( 0, pTexture);
		}
		else
		{
			pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE);
			vWorld._11 = 1.0f;
			vWorld._22 = 1.0f;
			vWorld._31 = 0.0f;
			vWorld._32 = 0.0f;

			pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &vWorld);
			pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE1, &vWorld);
			pDevice->m_pDevice->SetTexture( 0, m_pTILETEX);
		}

		if( pDevice->m_vCAPS.MaxVertexIndex > 0x0000FFFF )
		{
			if(m_vCOUNT[i])
			{
				pDevice->m_pDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,
					0, 0, nCount,
					m_vSTART[i],
					m_vCOUNT[i]);
				nTotal += m_vCOUNT[i];
			}

			if(m_vBLEND[i])
			{
				pDevice->m_pDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,
					0, 0, nCount,
					m_vBSTART[i],
					m_vBLEND[i]);
			}
		}
		else
		{
			for( int j=0; j<9; j++)
			{
				int nBASE = j * nLength;

				if(m_vCOUNT[i * 9 + j])
				{
					pDevice->m_pDevice->DrawIndexedPrimitive(
						D3DPT_TRIANGLELIST,
						nBASE, 0,
						nCount - nBASE,
						m_vSTART[i * 9 + j],
						m_vCOUNT[i * 9 + j]);
				}

				if(m_vBLEND[i * 9 + j])
				{
					pDevice->m_pDevice->DrawIndexedPrimitive(
						D3DPT_TRIANGLELIST,
						nBASE, 0,
						nCount - nBASE,
						m_vBSTART[i * 9 + j],
						m_vBLEND[i * 9 + j]);
				}
			}
		}
	}

	if( m_bDrawShadow && m_pSHADOWTEX )
	{
		pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

		pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

		pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB( m_bSHADOW, 0x00, 0x00, 0x00));
		pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
		pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);

		pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &m_vSHADOWTRANS);
		pDevice->m_pDevice->SetTexture( 0, m_pSHADOWTEX);

		if( pDevice->m_vCAPS.MaxVertexIndex > 0x0000FFFF )
			if( pDevice->m_vCAPS.MaxPrimitiveCount < nTotal )
				for( i=0; i<INT(m_vDETAIL.size()); i++)
				{
					pDevice->m_pDevice->DrawIndexedPrimitive(
						D3DPT_TRIANGLELIST,
						0, 0, nCount,
						m_vSTART[i],
						m_vCOUNT[i]);
				}
			else
			{
				pDevice->m_pDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,
					0, 0, nCount,
					0, nTotal);
			}
		else
		{
			for( i=0; i<INT(m_vDETAIL.size()); i++)
				for( int j=0; j<9; j++)
				{
					pDevice->m_pDevice->DrawIndexedPrimitive(
						D3DPT_TRIANGLELIST,
						j * nLength, 0,
						nCount - j * nLength,
						m_vSTART[i * 9 + j],
						m_vCOUNT[i * 9 + j]);
				}
		}
	}
	fDepthBias = EXTBIAS_BASE;

	pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	pDevice->m_pDevice->SetVertexShader(NULL);
	pDevice->m_pDevice->SetPixelShader(NULL);
}

void CTachyonHUGEMAP::ResetVB( CD3DDevice *pDevice,
							   CD3DCamera *pCamera,
							   BYTE bCullView,
							   BYTE bTargetLOD)
{
	IPOINT vCENTER(
		INT((bTargetLOD ? pCamera->m_vTarget.x : pCamera->m_vPosition.x) / m_nTileLength),
		INT((bTargetLOD ? pCamera->m_vTarget.z : pCamera->m_vPosition.z) / m_nTileLength));

	IPOINT vPIVOT(
		INT(pCamera->m_vTarget.x / m_nTileLength),
		INT(pCamera->m_vTarget.z / m_nTileLength));

	FLOAT fRANGE = 0.0f;
	FLOAT fDIFF = 0.0f;
	FLOAT fDIR = 0.0f;

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

			fDIFF = fabs(m_fDIR - fDIR);
			if( fDIFF > D3DX_PI )
				fDIFF = 2.0f * D3DX_PI - fDIFF;
		}
		else
			bCullView = FALSE;
	}

	for( int i=0; i<9; i++)
		m_bUPDATE[i] = FALSE;

	if( abs(m_vPIVOT.m_nX - vPIVOT.m_nX) < 3 &&
		abs(m_vPIVOT.m_nY - vPIVOT.m_nY) < 3 &&
		fDIFF < D3DX_PI / 6.0f && !m_bResetVB )
		return;

	if( pDevice->m_vCAPS.MaxVertexIndex <= 0x0000FFFF )
	{
		int nMaxUnit = m_nMaxLength / 2;

		vCENTER.m_nX /= nMaxUnit;
		vCENTER.m_nY /= nMaxUnit;

		vCENTER.m_nX *= nMaxUnit;
		vCENTER.m_nY *= nMaxUnit;
	}

	m_vCENTER.m_dwID = vCENTER.m_dwID;
	m_vPIVOT.m_dwID = vPIVOT.m_dwID;
	m_fDIR = fDIR;
	m_bResetVB = FALSE;

	CheckVB( pDevice->m_pDevice, &m_vPIVOT);
	BuildBLOCKIDX(
		&mapTILE,
		&vCENTER,
		fRANGE,
		fDIR,
		bCullView);

	if( pDevice->m_vCAPS.MaxVertexIndex > 0x0000FFFF )
		ResetVB32( pDevice, &mapTILE, &vCENTER);
	else
		ResetVB16( pDevice, &mapTILE, &vCENTER);

	ResetTEX( pDevice->m_pDevice, &vCENTER);
	pCamera->Activate(TRUE);

	mapTILE.clear();
}

void CTachyonHUGEMAP::BuildBLOCKIDX( LPMAPDWORD pTILE,
									 LPIPOINT pCENTER,
									 FLOAT fRANGE,
									 FLOAT fDIR,
									 BYTE bCullView)
{
	m_nBlockCount = 0;

	for( int i=0; i<INT(m_vBLOCK.size()); i++)
		if((!bCullView || BLOCKInVIEW( m_vBLOCK[i], fDIR, fRANGE)) && IsValidBLOCK( m_vBLOCK[i], pCENTER))
		{
			IPOINT vPOINT(
				pCENTER->m_nX + m_vBLOCK[i]->m_vRECT.m_nLEFT,
				pCENTER->m_nY + m_vBLOCK[i]->m_vRECT.m_nTOP);
			BYTE bTILE = TILE_NULL;

			if( m_vBLOCK[i]->Width() < 2 )
			{
				m_vBLOCK[i]->m_bTILE = FindDetailID(
					vPOINT.m_nX,
					vPOINT.m_nY);
				bTILE = m_vBLOCK[i]->m_bTILE == TILE_NULL ? TILE_NULL : m_vBLOCK[i]->m_bTILE & 0x7F;

				if( bTILE >= INT(m_vDETAILDATA.size()) || !m_vDETAILDATA[bTILE] )
				{
					m_vBLOCK[i]->m_bTILE = TILE_NULL;
					bTILE = TILE_NULL;
				}
			}

			if( pTILE->find(bTILE) == pTILE->end() )
				pTILE->insert( MAPDWORD::value_type( bTILE, 0));

			m_pBLOCK[m_nBlockCount] = m_vBLOCK[i];
			m_nBlockCount++;
		}
}

void CTachyonHUGEMAP::ResetVB32( CD3DDevice *pDevice,
								 LPMAPDWORD pTILE,
								 LPIPOINT pCENTER)
{
	MAPDWORD::iterator itTILE;

	m_vDIFFUSEID.clear();
	m_vDETAILS.clear();
	m_vDETAIL.clear();
	m_vBLEND.clear();
	m_vCOUNT.clear();

	m_vBSTART.clear();
	m_vSTART.clear();

	if(!pTILE->empty())
	{
		int nCount = m_nUnitLength / m_nCellCount;

		VECTORBLOCK vBLEND[256];
		DWORD vSTART[256];
		DWORD vINDEX[256];

		for( int i=0; i<INT(pTILE->size()); i++)
			vBLEND[i].clear();

		if( pTILE->find(TILE_NULL) != pTILE->end() )
		{
			m_vDETAILS.push_back(0.0f);
			m_vDETAIL.push_back(NULL);

			m_vDIFFUSEID.push_back(0);
			m_vBLEND.push_back(0);
			m_vCOUNT.push_back(0);
			vINDEX[TILE_NULL] = 0;
		}

		for( itTILE = pTILE->begin(); itTILE != pTILE->end(); itTILE++)
			if( (*itTILE).first != TILE_NULL )
			{
				MAPDWORD::iterator itDIFFUSE = m_mapDIFFUSEINDEX.find((*itTILE).first);

				m_vDETAILS.push_back(m_vDETAILSDATA[(*itTILE).first]);
				m_vDETAIL.push_back(m_vDETAILDATA[(*itTILE).first]);

				m_vDIFFUSEID.push_back(BYTE((*itDIFFUSE).second));
				m_vBLEND.push_back(0);
				m_vCOUNT.push_back(0);
				vINDEX[(*itTILE).first] = m_vDETAIL.size() - 1;
			}

		for( i=0; i<m_nBlockCount; i++)
		{
			LPBLOCK pRECT = m_pBLOCK[i];

			DWORD dwIndex = vINDEX[pRECT->m_bTILE == TILE_NULL ? TILE_NULL : (pRECT->m_bTILE) & 0x7F];
			SHORT nWIDTH = pRECT->Width();

			BYTE bREGION = GetRegionID(
				pCENTER->m_nX + pRECT->m_vRECT.m_nLEFT,
				pCENTER->m_nY + pRECT->m_vRECT.m_nTOP);

			if( nWIDTH > 1 || bREGION != BLANK_REGION )
			{
				m_vCOUNT[dwIndex] += INT(pRECT->m_vPOLY.size()) / 3;

				if( nWIDTH < 2 )
				{
					IPOINT vPOINT(
						pCENTER->m_nX + pRECT->m_vRECT.m_nLEFT,
						pCENTER->m_nY + pRECT->m_vRECT.m_nTOP);

					int nCellX = vPOINT.m_nX * m_nCellCount / m_nUnitLength - m_nCellX;
					int nCellZ = vPOINT.m_nY * m_nCellCount / m_nUnitLength - m_nCellZ;
					BYTE bINDEX = m_bINDEX[nCellZ * 3 + nCellX];

					if( bINDEX != 0xFF )
					{
						nCellX += m_nCellX;
						nCellZ += m_nCellZ;

						nCellX *= nCount;
						nCellZ *= nCount;

						vPOINT.m_nX -= nCellX;
						vPOINT.m_nY -= nCellZ;

						for( int j=0; j<INT(m_vBLENDTILE.size()); j++)
						{
							BYTE bTILE = ((LPBYTE) m_vBLENDTILE[j])[bINDEX * nCount * nCount + vPOINT.m_nY * nCount + vPOINT.m_nX];

							if( bTILE != TILE_NULL && pTILE->find(bTILE) != pTILE->end() )
							{
								m_vBLEND[vINDEX[bTILE]] += INT(pRECT->m_vPOLY.size()) / 3;
								vBLEND[vINDEX[bTILE]].push_back(pRECT);
							}
						}
					}
				}
			}
		}

		LPVOID pBUF = NULL;
		DWORD dwTOTAL = 0;

		for( i=0; i<INT(pTILE->size()); i++)
		{
			m_vSTART.push_back(dwTOTAL);
			vSTART[i] = dwTOTAL;

			dwTOTAL += 3 * m_vCOUNT[i];
		}

		for( i=0; i<INT(pTILE->size()); i++)
		{
			m_vBSTART.push_back(dwTOTAL);
			dwTOTAL += 3 * m_vBLEND[i];
		}

		if( dwTOTAL > m_nIndexCount )
		{
			m_nIndexCount = dwTOTAL;

			delete[] m_pINDEXBUF;
			m_pINDEX->Release();

			pDevice->m_pDevice->CreateIndexBuffer(
				m_nIndexCount * sizeof(DWORD),
				D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX32,
				D3DPOOL_DEFAULT,
				&m_pINDEX, NULL);

			m_pINDEXBUF = new DWORD[m_nIndexCount];
		}

		for( i=0; i<m_nBlockCount; i++)
		{
			LPBLOCK pRECT = m_pBLOCK[i];

			DWORD dwIndex = vINDEX[pRECT->m_bTILE == TILE_NULL ? TILE_NULL : (pRECT->m_bTILE) & 0x7F];
			SHORT nWIDTH = pRECT->Width();
			LPDWORD pINDEX = (LPDWORD) m_pINDEXBUF;

			BYTE bREGION = GetRegionID(
				pCENTER->m_nX + pRECT->m_vRECT.m_nLEFT,
				pCENTER->m_nY + pRECT->m_vRECT.m_nTOP);

			if( nWIDTH > 1 || bREGION != BLANK_REGION )
			{
				VECTORDWORD vPOINT;
				vPOINT.clear();

				for( int j=0; j<INT(pRECT->m_vPOINT.size()); j++)
				{
					vPOINT.push_back( GetVBIndex32(
						pCENTER,
						pRECT, j));
				}

				for( j=0; j<INT(pRECT->m_vPOLY.size()); j++)
				{
					pINDEX[vSTART[dwIndex]] = vPOINT[pRECT->m_vPOLY[j]];
					vSTART[dwIndex]++;
				}

				vPOINT.clear();
			}
		}

		for( i=0; i<INT(pTILE->size()); i++)
		{
			LPDWORD pINDEX = (LPDWORD) m_pINDEXBUF;
			pINDEX += m_vBSTART[i];

			for( int j=0; j<INT(vBLEND[i].size()); j++)
			{
				LPBLOCK pRECT = vBLEND[i][j];

				VECTORDWORD vPOINT;
				vPOINT.clear();

				for( int k=0; k<INT(pRECT->m_vPOINT.size()); k++)
				{
					vPOINT.push_back( GetVBIndex32(
						pCENTER,
						pRECT, k));
				}

				for( k=0; k<INT(pRECT->m_vPOLY.size()); k++)
				{
					(*pINDEX) = vPOINT[pRECT->m_vPOLY[k]];
					pINDEX++;
				}

				vPOINT.clear();
			}
		}

		m_pINDEX->Lock( 0, dwTOTAL * sizeof(DWORD), &pBUF, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD);
		memcpy( pBUF, m_pINDEXBUF, dwTOTAL * sizeof(DWORD));
		m_pINDEX->Unlock();

		for( i=0; i<INT(pTILE->size()); i++)
			vBLEND[i].clear();
	}
}

void CTachyonHUGEMAP::ResetVB16( CD3DDevice *pDevice,
								 LPMAPDWORD pTILE,
								 LPIPOINT pCENTER)
{
	MAPDWORD::iterator itTILE;

	m_vDIFFUSEID.clear();
	m_vDETAILS.clear();
	m_vDETAIL.clear();
	m_vBLEND.clear();
	m_vCOUNT.clear();

	m_vBSTART.clear();
	m_vSTART.clear();

	if(!pTILE->empty())
	{
		int nCount = m_nUnitLength / m_nCellCount;

		VECTORBLOCK vBLEND[9][256];
		DWORD vSTART[9][256];
		DWORD vCOUNT[9][256];

		DWORD vINDEX[256];

		for( int i=0; i<INT(pTILE->size()); i++)
			for( int j=0; j<9; j++)
				vBLEND[j][i].clear();

		if( pTILE->find(TILE_NULL) != pTILE->end() )
		{
			m_vDETAILS.push_back(0.0f);
			m_vDETAIL.push_back(NULL);

			m_vDIFFUSEID.push_back(0);
			vINDEX[TILE_NULL] = 0;

			for( i=0; i<9; i++)
			{
				m_vBLEND.push_back(0);
				m_vCOUNT.push_back(0);
				vCOUNT[i][TILE_NULL] = m_vCOUNT.size() - 1;
			}
		}

		for( itTILE = pTILE->begin(); itTILE != pTILE->end(); itTILE++)
			if( (*itTILE).first != TILE_NULL )
			{
				MAPDWORD::iterator itDIFFUSE = m_mapDIFFUSEINDEX.find((*itTILE).first);

				m_vDETAILS.push_back(m_vDETAILSDATA[(*itTILE).first]);
				m_vDETAIL.push_back(m_vDETAILDATA[(*itTILE).first]);

				m_vDIFFUSEID.push_back(BYTE((*itDIFFUSE).second));
				vINDEX[(*itTILE).first] = m_vDETAIL.size() - 1;

				for( i=0; i<9; i++)
				{
					m_vBLEND.push_back(0);
					m_vCOUNT.push_back(0);
					vCOUNT[i][(*itTILE).first] = m_vCOUNT.size() - 1;
				}
			}

		for( i=0; i<m_nBlockCount; i++)
		{
			LPBLOCK pRECT = m_pBLOCK[i];

			SHORT nWIDTH = pRECT->Width();
			BYTE bREGION = GetRegionID(
				pCENTER->m_nX + pRECT->m_vRECT.m_nLEFT,
				pCENTER->m_nY + pRECT->m_vRECT.m_nTOP);

			if( nWIDTH > 1 || bREGION != BLANK_REGION )
			{
				BYTE bTILE = pRECT->m_bTILE == TILE_NULL ? TILE_NULL : pRECT->m_bTILE & 0x7F;

				IPOINT vPOINT(
					pCENTER->m_nX + pRECT->m_vRECT.m_nLEFT,
					pCENTER->m_nY + pRECT->m_vRECT.m_nTOP);

				int nCellX = vPOINT.m_nX * m_nCellCount / m_nUnitLength - m_nCellX;
				int nCellZ = vPOINT.m_nY * m_nCellCount / m_nUnitLength - m_nCellZ;
				BYTE bINDEX = m_bINDEX[nCellZ * 3 + nCellX];

				m_vCOUNT[vCOUNT[bINDEX][bTILE]] += INT(pRECT->m_vPOLY.size()) / 3;
				if( nWIDTH < 2 )
				{
					nCellX += m_nCellX;
					nCellZ += m_nCellZ;

					nCellX *= nCount;
					nCellZ *= nCount;

					vPOINT.m_nX -= nCellX;
					vPOINT.m_nY -= nCellZ;

					for( int j=0; j<INT(m_vBLENDTILE.size()); j++)
					{
						bTILE = ((LPBYTE) m_vBLENDTILE[j])[bINDEX * nCount * nCount + vPOINT.m_nY * nCount + vPOINT.m_nX];

						if( bTILE != TILE_NULL && pTILE->find(bTILE) != pTILE->end() )
						{
							m_vBLEND[vCOUNT[bINDEX][bTILE]] += INT(pRECT->m_vPOLY.size()) / 3;
							vBLEND[bINDEX][vINDEX[bTILE]].push_back(pRECT);
						}
					}
				}
			}
		}

		LPVOID pBUF = NULL;
		DWORD dwTOTAL = 0;

		for( i=0; i<INT(pTILE->size()); i++)
			for( int j=0; j<9; j++)
			{
				m_vSTART.push_back(dwTOTAL);
				vSTART[j][i] = dwTOTAL;

				dwTOTAL += 3 * m_vCOUNT[i * 9 + j];
			}

		for( i=0; i<INT(pTILE->size()); i++)
			for( int j=0; j<9; j++)
			{
				m_vBSTART.push_back(dwTOTAL);
				dwTOTAL += 3 * m_vBLEND[i * 9 + j];
			}

		if( dwTOTAL > m_nIndexCount )
		{
			m_nIndexCount = dwTOTAL;

			delete[] m_pINDEXBUF;
			m_pINDEX->Release();

			pDevice->m_pDevice->CreateIndexBuffer(
				m_nIndexCount * sizeof(WORD),
				D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_DEFAULT,
				&m_pINDEX, NULL);

			m_pINDEXBUF = new WORD[m_nIndexCount];
		}

		for( i=0; i<m_nBlockCount; i++)
		{
			LPBLOCK pRECT = m_pBLOCK[i];

			LPWORD pINDEX = (LPWORD) m_pINDEXBUF;
			SHORT nWIDTH = pRECT->Width();

			BYTE bREGION = GetRegionID(
				pCENTER->m_nX + pRECT->m_vRECT.m_nLEFT,
				pCENTER->m_nY + pRECT->m_vRECT.m_nTOP);

			if( nWIDTH > 1 || bREGION != BLANK_REGION )
			{
				BYTE bTILE = pRECT->m_bTILE == TILE_NULL ? TILE_NULL : pRECT->m_bTILE & 0x7F;

				IPOINT vPOS(
					pCENTER->m_nX + pRECT->m_vRECT.m_nLEFT,
					pCENTER->m_nY + pRECT->m_vRECT.m_nTOP);

				int nCellX = vPOS.m_nX * m_nCellCount / m_nUnitLength - m_nCellX;
				int nCellZ = vPOS.m_nY * m_nCellCount / m_nUnitLength - m_nCellZ;
				BYTE bINDEX = m_bINDEX[nCellZ * 3 + nCellX];

				VECTORDWORD vPOINT;
				vPOINT.clear();

				for( int j=0; j<INT(pRECT->m_vPOINT.size()); j++)
				{
					vPOINT.push_back( GetVBIndex16(
						pCENTER,
						pRECT, j));
				}

				for( j=0; j<INT(pRECT->m_vPOLY.size()); j++)
				{
					pINDEX[vSTART[bINDEX][vINDEX[bTILE]]] = (WORD) vPOINT[pRECT->m_vPOLY[j]];
					vSTART[bINDEX][vINDEX[bTILE]]++;
				}

				vPOINT.clear();
			}
		}

		for( i=0; i<INT(pTILE->size()); i++)
			for( int j=0; j<9; j++)
			{
				LPWORD pINDEX = &((LPWORD) m_pINDEXBUF)[m_vBSTART[i * 9 + j]];

				for( int k=0; k<INT(vBLEND[j][i].size()); k++)
				{
					LPBLOCK pRECT = vBLEND[j][i][k];

					VECTORDWORD vPOINT;
					vPOINT.clear();

					for( int l=0; l<INT(pRECT->m_vPOINT.size()); l++)
					{
						vPOINT.push_back( GetVBIndex16(
							pCENTER,
							pRECT, l));
					}

					for( l=0; l<INT(pRECT->m_vPOLY.size()); l++)
					{
						(*pINDEX) = (WORD) vPOINT[pRECT->m_vPOLY[l]];
						pINDEX++;
					}

					vPOINT.clear();
				}
			}

		m_pINDEX->Lock( 0, dwTOTAL * sizeof(WORD), &pBUF, D3DLOCK_NOSYSLOCK|D3DLOCK_DISCARD);
		memcpy( pBUF, m_pINDEXBUF, dwTOTAL * sizeof(WORD));
		m_pINDEX->Unlock();

		for( i=0; i<INT(pTILE->size()); i++)
			for( int j=0; j<9; j++)
				vBLEND[j][i].clear();
	}
}

void CTachyonHUGEMAP::CheckVB( LPDIRECT3DDEVICE9 pDevice,
							   LPIPOINT pPIVOT)
{
	int nCellX = pPIVOT->m_nX * m_nCellCount / m_nUnitLength - (pPIVOT->m_nX < 0 ? 2 : 1);
	int nCellZ = pPIVOT->m_nY * m_nCellCount / m_nUnitLength - (pPIVOT->m_nY < 0 ? 2 : 1);
	int nUnitX = pPIVOT->m_nX / m_nUnitLength - (pPIVOT->m_nX < 0 ? 2 : 1);
	int nUnitZ = pPIVOT->m_nY / m_nUnitLength - (pPIVOT->m_nY < 0 ? 2 : 1);

	if( nUnitX != m_nUnitX ||
		nUnitZ != m_nUnitZ )
	{
		BYTE bUNIT[9];

		for( int i=0; i<9; i++)
		{
			bUNIT[i] = m_bUNIT[i];
			m_bLOAD[i] = FALSE;
		}

		for( i=0; i<9; i++)
		{
			int nLocalX = nUnitX - m_nUnitX + i % 3;
			int nLocalZ = nUnitZ - m_nUnitZ + i / 3;

			if( nLocalX < 0 || nLocalX > 2 ||
				nLocalZ < 0 || nLocalZ > 2 )
			{
				m_pUNIT[i] = NULL;
				m_bUNIT[i] = 0xFF;
			}
			else
			{
				int nIndex = nLocalZ * 3 + nLocalX;

				if( bUNIT[nIndex] != 0xFF )
				{
					m_bLOAD[bUNIT[nIndex]] = TRUE;

					m_pUNIT[i] = &m_UNIT[bUNIT[nIndex]];
					m_bUNIT[i] = bUNIT[nIndex];
				}
				else
				{
					m_pUNIT[i] = NULL;
					m_bUNIT[i] = 0xFF;
				}
			}
		}

		m_nUnitX = nUnitX;
		m_nUnitZ = nUnitZ;
	}

	if( nCellX != m_nCellX ||
		nCellZ != m_nCellZ )
	{
		BYTE bINDEX[9];

		for( int i=0; i<9; i++)
		{
			bINDEX[i] = m_bINDEX[i];
			m_bLOCK[i] = FALSE;
		}

		for( i=0; i<9; i++)
		{
			int nLocalX = nCellX - m_nCellX + i % 3;
			int nLocalZ = nCellZ - m_nCellZ + i / 3;

			if( nLocalX < 0 || nLocalX > 2 ||
				nLocalZ < 0 || nLocalZ > 2 )
				m_bINDEX[i] = 0xFF;
			else
			{
				int nIndex = nLocalZ * 3 + nLocalX;

				if( bINDEX[nIndex] != 0xFF )
				{
					m_bLOCK[bINDEX[nIndex]] = TRUE;
					m_bINDEX[i] = bINDEX[nIndex];
				}
				else
					m_bINDEX[i] = 0xFF;
			}
		}

		for( i=0; i<9; i++)
			if(!m_bLOCK[i])
				FreeDIFFUSE(i);

		m_nCellX = nCellX;
		m_nCellZ = nCellZ;

		for( i=0; i<9; i++)
			if( m_bINDEX[i] == 0xFF )
			{
				nCellX = m_nCellX + i % 3;
				nCellZ = m_nCellZ + i / 3;

				nUnitX = nCellX / m_nCellCount - (nCellX < 0 ? 1 : 0);
				nUnitZ = nCellZ / m_nCellCount - (nCellZ < 0 ? 1 : 0);

				if( nUnitX >= 0 && nUnitX < m_nUnitCountX &&
					nUnitZ >= 0 && nUnitZ < m_nUnitCountZ )
				{
					int nIndex = (nUnitZ - m_nUnitZ) * 3 + nUnitX - m_nUnitX;
					int nUnitID = nUnitZ * m_nUnitCountX + nUnitX;

					if(m_pENABLE[nUnitID])
					{
						if( m_bUNIT[nIndex] == 0xFF )
						{
							m_bUNIT[nIndex] = FindFreeUNIT();

							m_pUNIT[nIndex] = &m_UNIT[m_bUNIT[nIndex]];
							m_bLOAD[m_bUNIT[nIndex]] = TRUE;

							LoadUNIT(
								pDevice,
								m_pUNIT[nIndex],
								nUnitID);
							m_bUPDATE[nIndex] = TRUE;
						}

						m_bINDEX[i] = FindFreeCELL();
						m_bLOCK[m_bINDEX[i]] = TRUE;

						LoadVB(
							m_pUNIT[nIndex], i,
							nUnitX,
							nUnitZ,
							nCellX,
							nCellZ);

						BuildDIFFUSE(
							pDevice, i,
							nCellX,
							nCellZ);
					}
				}
			}
		ResetUV();
	}
}

void CTachyonHUGEMAP::FreeDIFFUSE( BYTE bINDEX)
{
	int nCount = m_nUnitLength / m_nCellCount;

	MAPDWORD::iterator itDIFFUSE;
	BYTE bTileID[256];
	DWORD dwMASK = ~DWORD(1 << bINDEX);

	for( itDIFFUSE = m_mapDIFFUSEINDEX.begin(); itDIFFUSE != m_mapDIFFUSEINDEX.end(); itDIFFUSE++)
		bTileID[(*itDIFFUSE).second] = BYTE((*itDIFFUSE).first);
	nCount = 9 * nCount * nCount;

	for( BYTE i=1; i<BYTE(m_vDIFFUSELOCK.size()); i++)
		if(m_vDIFFUSELOCK[i])
		{
			m_vDIFFUSELOCK[i] &= dwMASK;

			if(!m_vDIFFUSELOCK[i])
			{
				itDIFFUSE = m_mapDIFFUSEINDEX.find(bTileID[i]);

				if( itDIFFUSE != m_mapDIFFUSEINDEX.end() )
					m_mapDIFFUSEINDEX.erase(itDIFFUSE);

				memset( LPBYTE(m_vBLENDTILE[i]), 0xFF, nCount * sizeof(BYTE));
			}
		}
}

void CTachyonHUGEMAP::ComplateBLOCK()
{
	MAPBLOCK mapBLOCK;

	for( int i=0; i<INT(m_vBLOCK.size()); i++)
		mapBLOCK.insert( MAPBLOCK::value_type( m_vBLOCK[i]->GetID(), m_vBLOCK[i]));

	for( i=0; i<INT(m_vBLOCK.size()); i++)
	{
		SHORT nWIDTH = m_vBLOCK[i]->Width();

		if( nWIDTH > 1 )
		{
			IPOINT vCENTER = m_vBLOCK[i]->Center();
			BYTE vJOINT[8] = {
				TRUE, FALSE,
				TRUE, FALSE,
				TRUE, FALSE,
				TRUE, FALSE};
			BYTE bJOINT = FALSE;
			nWIDTH /= 2;

			IPOINT vCHECK[4] = {
				IPOINT( vCENTER.m_nX, m_vBLOCK[i]->m_vRECT.m_nTOP - nWIDTH),
				IPOINT( m_vBLOCK[i]->m_vRECT.m_nRIGHT, vCENTER.m_nY),
				IPOINT( vCENTER.m_nX, m_vBLOCK[i]->m_vRECT.m_nBOTTOM),
				IPOINT( m_vBLOCK[i]->m_vRECT.m_nLEFT - nWIDTH, vCENTER.m_nY)};

			for( int j=0; j<4; j++)
				if( mapBLOCK.find(vCHECK[j].m_dwID) != mapBLOCK.end() )
				{
					vJOINT[j * 2 + 1] = TRUE;
					bJOINT = TRUE;
				}

			if(bJOINT)
			{
				BYTE bINDEX = 1;

				for( j=0; j<7; j++)
					if(vJOINT[j + 1])
					{
						m_vBLOCK[i]->m_vPOLY.push_back(bINDEX);
						m_vBLOCK[i]->m_vPOLY.push_back(0);
						bINDEX++;
						m_vBLOCK[i]->m_vPOLY.push_back(bINDEX);
					}

				m_vBLOCK[i]->m_vPOLY.push_back(bINDEX);
				m_vBLOCK[i]->m_vPOLY.push_back(0);
				m_vBLOCK[i]->m_vPOLY.push_back(1);

				m_vBLOCK[i]->m_vPOINT.push_back(new IPOINT(
					vCENTER.m_nX,
					vCENTER.m_nY));
			}
			else
			{
				m_vBLOCK[i]->m_vPOLY.push_back(0);
				m_vBLOCK[i]->m_vPOLY.push_back(3);
				m_vBLOCK[i]->m_vPOLY.push_back(1);
				m_vBLOCK[i]->m_vPOLY.push_back(1);
				m_vBLOCK[i]->m_vPOLY.push_back(3);
				m_vBLOCK[i]->m_vPOLY.push_back(2);
			}

			IPOINT vPOINT[8] = {
				IPOINT( m_vBLOCK[i]->m_vRECT.m_nLEFT, m_vBLOCK[i]->m_vRECT.m_nTOP),
				IPOINT( vCENTER.m_nX, m_vBLOCK[i]->m_vRECT.m_nTOP),
				IPOINT( m_vBLOCK[i]->m_vRECT.m_nRIGHT, m_vBLOCK[i]->m_vRECT.m_nTOP),
				IPOINT( m_vBLOCK[i]->m_vRECT.m_nRIGHT, vCENTER.m_nY),
				IPOINT( m_vBLOCK[i]->m_vRECT.m_nRIGHT, m_vBLOCK[i]->m_vRECT.m_nBOTTOM),
				IPOINT( vCENTER.m_nX, m_vBLOCK[i]->m_vRECT.m_nBOTTOM),
				IPOINT( m_vBLOCK[i]->m_vRECT.m_nLEFT, m_vBLOCK[i]->m_vRECT.m_nBOTTOM),
				IPOINT( m_vBLOCK[i]->m_vRECT.m_nLEFT, vCENTER.m_nY)};

			for( j=0; j<8; j++)
				if(vJOINT[j])
					m_vBLOCK[i]->m_vPOINT.push_back(new IPOINT( vPOINT[j].m_nX, vPOINT[j].m_nY));
		}
		else
		{
			m_vBLOCK[i]->m_vPOINT.push_back(new IPOINT( m_vBLOCK[i]->m_vRECT.m_nLEFT, m_vBLOCK[i]->m_vRECT.m_nTOP));
			m_vBLOCK[i]->m_vPOINT.push_back(new IPOINT( m_vBLOCK[i]->m_vRECT.m_nRIGHT, m_vBLOCK[i]->m_vRECT.m_nTOP));
			m_vBLOCK[i]->m_vPOINT.push_back(new IPOINT( m_vBLOCK[i]->m_vRECT.m_nRIGHT, m_vBLOCK[i]->m_vRECT.m_nBOTTOM));
			m_vBLOCK[i]->m_vPOINT.push_back(new IPOINT( m_vBLOCK[i]->m_vRECT.m_nLEFT, m_vBLOCK[i]->m_vRECT.m_nBOTTOM));

			m_vBLOCK[i]->m_vPOLY.push_back(0);
			m_vBLOCK[i]->m_vPOLY.push_back(3);
			m_vBLOCK[i]->m_vPOLY.push_back(1);
			m_vBLOCK[i]->m_vPOLY.push_back(1);
			m_vBLOCK[i]->m_vPOLY.push_back(3);
			m_vBLOCK[i]->m_vPOLY.push_back(2);
		}

		BuildBLOCKDIR(m_vBLOCK[i]);
	}

	mapBLOCK.clear();
}

void CTachyonHUGEMAP::BuildBLOCK( CRect rect)
{
	int nLevel = GetLODLevel(rect);
	int nBound = INT(pow( 2, nLevel));

	if( nBound < rect.Width() )
	{
		static offset[4][2] = {
			{ 0, 0},
			{ 1, 0},
			{ 0, 1},
			{ 1, 1}};

		rect.bottom -= rect.Height() / 2;
		rect.right -= rect.Width() / 2;

		for( int i=0; i<4; i++)
		{
			CRect rc(rect);

			rc.OffsetRect(
				rect.Width() * offset[i][0],
				rect.Height() * offset[i][1]);

			BuildBLOCK(rc);
		}
	}
	else if( !m_nMaxLength || rect.Width() < m_nMaxLength )
	{
		CPoint center = rect.CenterPoint();
		LPBLOCK pBLOCK = new BLOCK();

		pBLOCK->m_fDIST = D3DXVec2Length( &D3DXVECTOR2(
			FLOAT(center.x),
			FLOAT(center.y)));
		pBLOCK->m_vRECT.m_nLEFT = SHORT(rect.left);
		pBLOCK->m_vRECT.m_nTOP = SHORT(rect.top);
		pBLOCK->m_vRECT.m_nRIGHT = SHORT(rect.right);
		pBLOCK->m_vRECT.m_nBOTTOM = SHORT(rect.bottom);

		m_vBLOCK.push_back(pBLOCK);
	}
}

void CTachyonHUGEMAP::BuildBLOCKDIR( LPBLOCK pBLOCK)
{
	IPOINT vPOINT[4] = {
		IPOINT( pBLOCK->m_vRECT.m_nLEFT, pBLOCK->m_vRECT.m_nTOP),
		IPOINT( pBLOCK->m_vRECT.m_nRIGHT, pBLOCK->m_vRECT.m_nTOP),
		IPOINT( pBLOCK->m_vRECT.m_nLEFT, pBLOCK->m_vRECT.m_nBOTTOM),
		IPOINT( pBLOCK->m_vRECT.m_nRIGHT, pBLOCK->m_vRECT.m_nBOTTOM)};

	FLOAT fMIN = D3DX_PI;
	FLOAT fMAX = -fMIN;

	for( int i=0; i<4; i++)
		if(vPOINT[i].m_dwID)
		{
			FLOAT fLOCAL = atan2f( FLOAT(vPOINT[i].m_nX), FLOAT(vPOINT[i].m_nY));

			fMIN = min( fMIN, fLOCAL);
			fMAX = max( fMAX, fLOCAL);
		}
	pBLOCK->m_fRANGE = fMAX - fMIN;

	if( pBLOCK->m_fRANGE > D3DX_PI )
	{
		pBLOCK->m_fRANGE = 2.0f * D3DX_PI - pBLOCK->m_fRANGE;
		pBLOCK->m_fDIR = fMAX;
	}
	else
		pBLOCK->m_fDIR = fMIN;
}

int CTachyonHUGEMAP::GetLODLevel( CRect rect)
{
	CPoint center = rect.CenterPoint();
	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		FLOAT(center.x),
		FLOAT(center.y))) -
		D3DXVec2Length(&D3DXVECTOR2(
		FLOAT(rect.Width()),
		FLOAT(rect.Height()))) / 2.0f;
	fDist = max( 0.0f, fDist);

	return min( m_nMaxLevel, INT(fDist) / m_nLODBound);
}

BYTE CTachyonHUGEMAP::IsValidBLOCK( LPBLOCK pBLOCK, LPIPOINT pCENTER)
{
	CPoint vPOINT[4] = {
		CPoint( pCENTER->m_nX + pBLOCK->m_vRECT.m_nLEFT, pCENTER->m_nY + pBLOCK->m_vRECT.m_nTOP),
		CPoint( pCENTER->m_nX + pBLOCK->m_vRECT.m_nRIGHT, pCENTER->m_nY + pBLOCK->m_vRECT.m_nTOP),
		CPoint( pCENTER->m_nX + pBLOCK->m_vRECT.m_nLEFT, pCENTER->m_nY + pBLOCK->m_vRECT.m_nBOTTOM),
		CPoint( pCENTER->m_nX + pBLOCK->m_vRECT.m_nRIGHT, pCENTER->m_nY + pBLOCK->m_vRECT.m_nBOTTOM)};

	for( BYTE i=0; i<4; i++)
	{
		int nCellX = vPOINT[i].x * m_nCellCount / m_nUnitLength - m_nCellX - (vPOINT[i].x < 0 ? 1 : 0);
		int nCellZ = vPOINT[i].y * m_nCellCount / m_nUnitLength - m_nCellZ - (vPOINT[i].y < 0 ? 1 : 0);

		if( nCellX < 0 || nCellX > 2 ||
			nCellZ < 0 || nCellZ > 2 ||
			m_bINDEX[nCellZ * 3 + nCellX] == 0xFF )
			return FALSE;
	}

	return TRUE;
}

BYTE CTachyonHUGEMAP::BLOCKInVIEW( LPBLOCK pBLOCK,
								   FLOAT fDIR,
								   FLOAT fRANGE)
{
	FLOAT fLOCAL = pBLOCK->m_fDIR;

	if( fDIR > 0.0f && fLOCAL < 0.0f )
		fLOCAL += 2.0f * D3DX_PI;

	if( pBLOCK->m_fDIST > m_fCullDIST ||
		fLOCAL + pBLOCK->m_fRANGE < fDIR ||
		fLOCAL > fDIR + fRANGE )
		return FALSE;

	return TRUE;
}

FLOAT CTachyonHUGEMAP::GetHeight( FLOAT fPosX,
								  FLOAT fPosY,
								  FLOAT fPosZ)
{
	fPosX /= FLOAT(m_nTileLength);
	fPosZ /= FLOAT(m_nTileLength);

	int nTileX = INT(fPosX);
	int nTileZ = INT(fPosZ);

	int nUnitX = nTileX / m_nUnitLength - m_nUnitX - (nTileX < 0 ? 1 : 0);
	int nUnitZ = nTileZ / m_nUnitLength - m_nUnitZ - (nTileZ < 0 ? 1 : 0);

	if( nUnitX > 2 || nUnitX < 0 ||
		nUnitZ > 2 || nUnitZ < 0 )
		return TMIN_HEIGHT;
	int nIndex = nUnitZ * 3 + nUnitX;

	fPosX -= FLOAT(nTileX);
	fPosZ -= FLOAT(nTileZ);

	FLOAT fResX = FLOAT(nTileX % m_nUnitLength) + fPosX;
	FLOAT fResZ = FLOAT(nTileZ % m_nUnitLength) + fPosZ;

	return m_pUNIT[nIndex] ? m_pUNIT[nIndex]->GetHeight( fResX, fPosY, fResZ) : 0.0f;
}

DWORD CTachyonHUGEMAP::GetVBIndex32( LPIPOINT pCENTER,
									 LPBLOCK pBLOCK,
									 BYTE bPOINT)
{
	IPOINT vBLOCK(
		pCENTER->m_nX + pBLOCK->m_vRECT.m_nLEFT,
		pCENTER->m_nY + pBLOCK->m_vRECT.m_nTOP);

	IPOINT vPOINT(
		pCENTER->m_nX + pBLOCK->m_vPOINT[bPOINT]->m_nX,
		pCENTER->m_nY + pBLOCK->m_vPOINT[bPOINT]->m_nY);

	IPOINT vBCELL(
		vBLOCK.m_nX * m_nCellCount / m_nUnitLength - m_nCellX,
		vBLOCK.m_nY * m_nCellCount / m_nUnitLength - m_nCellZ);

	IPOINT vPCELL(
		vPOINT.m_nX * m_nCellCount / m_nUnitLength - m_nCellX,
		vPOINT.m_nY * m_nCellCount / m_nUnitLength - m_nCellZ);

	int nCount = m_nUnitLength / m_nCellCount;
	int nResX = vPOINT.m_nX % nCount;
	int nResZ = vPOINT.m_nY % nCount;

	if( vBCELL.m_nX != vPCELL.m_nX && !nResX )
	{
		vPCELL.m_nX = vBCELL.m_nX;
		nResX = nCount;
	}

	if( vBCELL.m_nY != vPCELL.m_nY && !nResZ )
	{
		vPCELL.m_nY = vBCELL.m_nY;
		nResZ = nCount;
	}
	int nIndex = vPCELL.m_nY * 3 + vPCELL.m_nX;

	return DWORD(m_bINDEX[nIndex] * (nCount + 1) * (nCount + 1) + nResZ * (nCount + 1) + nResX);
}

WORD CTachyonHUGEMAP::GetVBIndex16( LPIPOINT pCENTER,
								    LPBLOCK pBLOCK,
									BYTE bPOINT)
{
	IPOINT vBLOCK(
		pCENTER->m_nX + pBLOCK->m_vRECT.m_nLEFT,
		pCENTER->m_nY + pBLOCK->m_vRECT.m_nTOP);

	IPOINT vPOINT(
		pCENTER->m_nX + pBLOCK->m_vPOINT[bPOINT]->m_nX,
		pCENTER->m_nY + pBLOCK->m_vPOINT[bPOINT]->m_nY);

	IPOINT vBCELL(
		vBLOCK.m_nX * m_nCellCount / m_nUnitLength - m_nCellX,
		vBLOCK.m_nY * m_nCellCount / m_nUnitLength - m_nCellZ);

	IPOINT vPCELL(
		vPOINT.m_nX * m_nCellCount / m_nUnitLength - m_nCellX,
		vPOINT.m_nY * m_nCellCount / m_nUnitLength - m_nCellZ);

	int nCount = m_nUnitLength / m_nCellCount;
	int nResX = vPOINT.m_nX % nCount;
	int nResZ = vPOINT.m_nY % nCount;

	if( vBCELL.m_nX != vPCELL.m_nX && !nResX )
		nResX = nCount;

	if( vBCELL.m_nY != vPCELL.m_nY && !nResZ )
		nResZ = nCount;

	return WORD(nResZ * (nCount + 1) + nResX);
}

BYTE CTachyonHUGEMAP::HitTest( LPD3DXVECTOR3 pResult,
							   LPD3DXVECTOR3 pPOS,
							   LPD3DXVECTOR3 pDIR)
{
	D3DXVECTOR3 vUP( 0.0f, 1.0f, 0.0f);

	BYTE bResult = FALSE;
	FLOAT fDist = 0.0f;

	(*pDIR) /= D3DXVec3Length(pDIR);
	for( int i=0; i<9; i++)
		if(m_pUNIT[i])
		{
			D3DXVECTOR3 vPOS = (*pPOS);

			vPOS.x -= FLOAT((m_nUnitX + i % 3) * m_nTileLength * m_nUnitLength);
			vPOS.z -= FLOAT((m_nUnitZ + i / 3) * m_nTileLength * m_nUnitLength);

			if( fabs(D3DXVec3Dot( &vUP, pDIR)) >= 1.0f )
			{
				vPOS /= FLOAT(m_nTileLength);

				if( vPOS.x >= 0.0f && vPOS.z >= 0.0f &&
					vPOS.x <= m_nUnitLength &&
					vPOS.z <= m_nUnitLength )
				{
					(*pResult) = (*pPOS);
					bResult = TRUE;

					pResult->y = m_pUNIT[i]->GetHeight(
						vPOS.x,
						0.0f,
						vPOS.z);
				}
			}
			else
			{
				D3DXPLANE vCUTTERH;
				D3DXPLANE vCUTTERV;

				D3DXPlaneFromPoints(
					&vCUTTERV,
					&(vPOS + (*pDIR)),
					&(vPOS + vUP),
					&vPOS);

				D3DXVec3Cross(
					&vUP,
					pDIR,
					&vUP);

				D3DXPlaneFromPoints(
					&vCUTTERH,
					&(vPOS + (*pDIR)),
					&(vPOS + vUP),
					&vPOS);

				for( int j=0; j<m_nUnitLength; j++)
					for( int k=0; k<m_nUnitLength; k++)
					{
						D3DXVECTOR3 vPoint[4] = {
							D3DXVECTOR3(
								FLOAT(m_nTileLength * k),
								m_pUNIT[i]->m_pDATA[(m_nUnitLength + 1) * j + k],
								FLOAT(m_nTileLength * j)),
							D3DXVECTOR3(
								FLOAT(m_nTileLength * (k + 1)),
								m_pUNIT[i]->m_pDATA[(m_nUnitLength + 1) * j + k + 1],
								FLOAT(m_nTileLength * j)),
							D3DXVECTOR3(
								FLOAT(m_nTileLength * (k + 1)),
								m_pUNIT[i]->m_pDATA[(m_nUnitLength + 1) * (j + 1) + k + 1],
								FLOAT(m_nTileLength * (j + 1))),
							D3DXVECTOR3(
								FLOAT(m_nTileLength * k),
								m_pUNIT[i]->m_pDATA[(m_nUnitLength + 1) * (j + 1) + k],
								FLOAT(m_nTileLength * (j + 1)))};
						BOOL bHIT = FALSE;

						for( int l=0; l<4; l++)
							if( D3DXPlaneDotCoord( &vCUTTERV, &vPoint[l]) *
								D3DXPlaneDotCoord( &vCUTTERV, &vPoint[(l + 1) % 4]) <= 0.0f )
							{
								bHIT = TRUE;
								break;
							}

						if(bHIT)
						{
							bHIT = FALSE;

							for( l=0; l<4; l++)
								if( D3DXPlaneDotCoord( &vCUTTERH, &vPoint[l]) *
									D3DXPlaneDotCoord( &vCUTTERH, &vPoint[(l + 1) % 4]) <= 0.0f )
								{
									bHIT = TRUE;
									break;
								}

							if(bHIT)
							{
								static int nIndex[2][3] = {
									{ 0, 1, 2},
									{ 2, 3, 0}};

								FLOAT fLocal = 0.0f;
								FLOAT fU = 0.0f;
								FLOAT fV = 0.0f;

								for( int l=0; l<2; l++)
									if( D3DXIntersectTri(
										&vPoint[nIndex[l][0]],
										&vPoint[nIndex[l][1]],
										&vPoint[nIndex[l][2]],
										&vPOS,
										pDIR,
										&fU, &fV,
										&fLocal) && (
										!bResult ||
										fLocal < fDist ))
									{
										(*pResult) = (*pPOS) + fLocal * (*pDIR);

										bResult = TRUE;
										fDist = fLocal;
									}
							}
						}
					}
			}
		}

	return bResult;
}

BYTE CTachyonHUGEMAP::PtInSHADOW( FLOAT fPosX,
								  FLOAT fPosZ)
{
	fPosX /= FLOAT(m_nTileLength);
	fPosZ /= FLOAT(m_nTileLength);

	int nUnitX = INT(fPosX) / m_nUnitLength - m_nUnitX;
	int nUnitZ = INT(fPosZ) / m_nUnitLength - m_nUnitZ;

	if( nUnitX > 2 || nUnitX < 0 ||
		nUnitZ > 2 || nUnitZ < 0 )
		return FALSE;

	int nPixelX = INT((fPosX - INT(fPosX)) * m_nShadowPixel);
	int nPixelZ = INT((fPosZ - INT(fPosZ)) * m_nShadowPixel);
	int nTileX = INT(fPosX) % m_nUnitLength;
	int nTileZ = INT(fPosZ) % m_nUnitLength;

	int nPixelIndex = nPixelZ * m_nShadowPixel + nPixelX;
	int nTileIndex = nTileZ * m_nUnitLength + nTileX;
	int nIndex = nUnitZ * 3 + nUnitX;

	return m_pUNIT[nIndex] && m_pUNIT[nIndex]->m_pSHADOW[nTileIndex] & (0x0001 << nPixelIndex) ? TRUE : FALSE;
}

BYTE CTachyonHUGEMAP::GetRegionID( int nTileX,
								   int nTileZ)
{
	int nUnitX = nTileX / m_nUnitLength - m_nUnitX;
	int nUnitZ = nTileZ / m_nUnitLength - m_nUnitZ;
	int nResX = nTileX % m_nUnitLength;
	int nResZ = nTileZ % m_nUnitLength;

	if( nUnitX > 2 || nUnitX < 0 ||
		nUnitZ > 2 || nUnitZ < 0 )
		return 0;
	int nIndex = nUnitZ * 3 + nUnitX;

	return m_pUNIT[nIndex] ? m_pUNIT[nIndex]->m_pREGION[nResZ * m_nUnitLength + nResX] : 0;
}

BYTE CTachyonHUGEMAP::FindDetailID( int nTileX,
								    int nTileZ)
{
	int nUnitX = nTileX / m_nUnitLength - m_nUnitX;
	int nUnitZ = nTileZ / m_nUnitLength - m_nUnitZ;
	int nResX = nTileX % m_nUnitLength;
	int nResZ = nTileZ % m_nUnitLength;

	if( nUnitX > 2 || nUnitX < 0 ||
		nUnitZ > 2 || nUnitZ < 0 ||
		nResX < 0 || nResZ < 0 )
		return TILE_NULL;
	int nIndex = nUnitZ * 3 + nUnitX;

	return m_pUNIT[nIndex] ? m_pUNIT[nIndex]->FindDetailID( nResX, nResZ) : TILE_NULL;
}

BYTE CTachyonHUGEMAP::FindFreeDIFFUSE( LPDIRECT3DDEVICE9 pDevice)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;

	int nLength = m_nUnitLength / m_nCellCount;
	int nCount = nLength + 1;

	nLength = 9 * nLength * nLength;
	nCount = 9 * nCount * nCount;

	for( BYTE i=1; i<BYTE(m_vDIFFUSELOCK.size()); i++)
		if(!m_vDIFFUSELOCK[i])
		{
			memset( LPDWORD(m_vDIFFUSEBUF[i]), 0x00000000, nCount * sizeof(DWORD));
			memset( LPBYTE(m_vBLENDTILE[i]), 0xFF, nLength * sizeof(BYTE));

			return i;
		}

	pDevice->CreateVertexBuffer(
		nCount * sizeof(DWORD),
		0, D3DFVF_DIFFUSE,
		D3DPOOL_MANAGED,
		&pVB, NULL);

	m_vDIFFUSEBUF.push_back(new DWORD[nCount]);
	m_vDIFFUSE.push_back(pVB);
	m_vDIFFUSELOCK.push_back(0);
	m_vBLENDTILE.push_back(new BYTE[nLength]);

	memset( LPDWORD(m_vDIFFUSEBUF.back()), 0x00000000, nCount * sizeof(DWORD));
	memset( LPBYTE(m_vBLENDTILE.back()), 0xFF, nLength * sizeof(BYTE));
	m_nDiffuseCount++;

	return BYTE(m_vDIFFUSE.size() - 1);
}

BYTE CTachyonHUGEMAP::FindFreeCELL()
{
	for( BYTE i=0; i<9; i++)
		if(!m_bLOCK[i])
			return i;

	return 0xFF;
}

BYTE CTachyonHUGEMAP::FindFreeUNIT()
{
	for( BYTE i=0; i<9; i++)
		if(!m_bLOAD[i])
			return i;

	return 0xFF;
}

void CTachyonHUGEMAP::BuildDIFFUSE( LPDIRECT3DDEVICE9 pDevice,
									BYTE bCELL,
									int nCellX,
									int nCellZ)
{
	static DWORD vBLEND[2][8][4] = {{
		{ 0xFF000000, 0x00000000, 0x00000000, 0x00000000},
		{ 0xFF000000, 0x00000000, 0xFF000000, 0x00000000},
		{ 0x00000000, 0x00000000, 0xFF000000, 0x00000000},
		{ 0xFF000000, 0xFF000000, 0x00000000, 0x00000000},
		{ 0x00000000, 0x00000000, 0xFF000000, 0xFF000000},
		{ 0x00000000, 0xFF000000, 0x00000000, 0x00000000},
		{ 0x00000000, 0xFF000000, 0x00000000, 0xFF000000},
		{ 0x00000000, 0x00000000, 0x00000000, 0xFF000000}}, {
		{ 0x00000000, 0x00000000, 0x00000000, 0xFF000000},
		{ 0x00000000, 0xFF000000, 0x00000000, 0xFF000000},
		{ 0x00000000, 0xFF000000, 0x00000000, 0x00000000},
		{ 0x00000000, 0x00000000, 0xFF000000, 0xFF000000},
		{ 0xFF000000, 0xFF000000, 0x00000000, 0x00000000},
		{ 0x00000000, 0x00000000, 0xFF000000, 0x00000000},
		{ 0xFF000000, 0x00000000, 0xFF000000, 0x00000000},
		{ 0xFF000000, 0x00000000, 0x00000000, 0x00000000}}};
	int nCount = m_nUnitLength / m_nCellCount;

	for( int i=1; i<INT(m_vDIFFUSEBUF.size()); i++)
		if(m_vDIFFUSELOCK[i])
		{
			memset( &LPDWORD(m_vDIFFUSEBUF[i])[m_bINDEX[bCELL] * (nCount + 1) * (nCount + 1)], 0x00000000, (nCount + 1) * (nCount + 1) * sizeof(DWORD));
			memset( &LPBYTE(m_vBLENDTILE[i])[m_bINDEX[bCELL] * nCount * nCount], 0xFF, nCount * nCount * sizeof(BYTE));
		}

	nCellX = nCellX * m_nUnitLength / m_nCellCount;
	nCellZ = nCellZ * m_nUnitLength / m_nCellCount;

	for( i=0; i<nCount; i++)
		for( int j=0; j<nCount; j++)
		{
			static int nBLEND[8][2] = {
				{ -1, -1},
				{  0, -1},
				{  1, -1},
				{ -1,  0},
				{  1,  0},
				{ -1,  1},
				{  0,  1},
				{  1,  1}};

			static int nRECT[4][2] = {
				{ 0, 0},
				{ 0, 1},
				{ 1, 0},
				{ 1, 1}};

			BYTE bTileID = FindDetailID(
				nCellX + j,
				nCellZ + i);

			if( bTileID != TILE_NULL )
			{
				LPDWORD pBUF = GetDIFFUSEBUF(
					pDevice,
					bCELL,
					bTileID & 0x7F);

				for( int k=0; k<4; k++)
					pBUF[(i + nRECT[k][1]) * (nCount + 1) + j + nRECT[k][0]] = 0xFF000000;

				if(!(bTileID&0x80))
					for( k=0; k<8; k++)
					{
						BYTE bBLEND = FindDetailID(
							nCellX + j + nBLEND[k][0],
							nCellZ + i + nBLEND[k][1]);

						if( !(bBLEND & 0x80) && (bBLEND & 0x7F) > (bTileID & 0x7F) )
						{
							BlendTILE(
								pDevice,
								&IPOINT(
								nCellX + j,
								nCellZ + i),
								vBLEND[0][k],
								bBLEND);
						}
						else if( bBLEND == TILE_NULL || (bBLEND & 0x7F) < (bTileID & 0x7F) )
						{
							BlendTILE(
								pDevice,
								&IPOINT(
								nCellX + j + nBLEND[k][0],
								nCellZ + i + nBLEND[k][1]),
								vBLEND[1][k],
								bTileID);
						}
					}
			}
		}

	DWORD dwMASK = DWORD(1 << m_bINDEX[bCELL]);
	nCount++;
	nCount *= nCount;

	for( i=1; i<INT(m_vDIFFUSELOCK.size()); i++)
		if(m_vDIFFUSELOCK[i]&dwMASK)
		{
			LPVOID pBUF = NULL;

			m_vDIFFUSE[i]->Lock( m_bINDEX[bCELL] * nCount * sizeof(DWORD), nCount * sizeof(DWORD), &pBUF, 0);
			memcpy( pBUF, LPDWORD(m_vDIFFUSEBUF[i]) + m_bINDEX[bCELL] * nCount, nCount * sizeof(DWORD));
			m_vDIFFUSE[i]->Unlock();
		}
}

void CTachyonHUGEMAP::BlendTILE( LPDIRECT3DDEVICE9 pDevice,
								 LPIPOINT pPOINT,
								 LPDWORD pDIFFUSE,
								 BYTE bTileID)
{
	static int nRECT[4][2] = {
		{ 0, 0},
		{ 0, 1},
		{ 1, 0},
		{ 1, 1}};

	int nCellX = pPOINT->m_nX * m_nCellCount / m_nUnitLength - m_nCellX;
	int nCellZ = pPOINT->m_nY * m_nCellCount / m_nUnitLength - m_nCellZ;

	int nLength = m_nUnitLength / m_nCellCount;
	int nCount = nLength + 1;

	if( pPOINT->m_nX < 0 || pPOINT->m_nY < 0 ||
		nCellX < 0 || nCellX > 2 ||
		nCellZ < 0 || nCellZ > 2 )
		return;
	BYTE bCELL = nCellZ * 3 + nCellX;

	if( m_bINDEX[bCELL] != 0xFF )
	{
		MAPDWORD::iterator itTILE = m_mapDIFFUSEINDEX.find(DWORD(bTileID&0x7F));

		if( itTILE != m_mapDIFFUSEINDEX.end() )
		{
			LPDWORD pBUF = GetDIFFUSEBUF(
				pDevice,
				bCELL,
				bTileID & 0x7F);

			int nLocalX = nCellX + m_nCellX;
			int nLocalZ = nCellZ + m_nCellZ;

			nLocalX *= nLength;
			nLocalZ *= nLength;

			nLocalX = pPOINT->m_nX - nLocalX;
			nLocalZ = pPOINT->m_nY - nLocalZ;

			for( BYTE i=0; i<4; i++)
				pBUF[(nLocalZ + nRECT[i][1]) * nCount + nLocalX + nRECT[i][0]] |= pDIFFUSE[i];

			(LPBYTE(m_vBLENDTILE[(*itTILE).second]))[m_bINDEX[bCELL] * nLength * nLength + nLocalZ * nLength + nLocalX] = BYTE(bTileID & 0x7F);
		}
	}
}

LPDWORD CTachyonHUGEMAP::GetDIFFUSEBUF( LPDIRECT3DDEVICE9 pDevice,
									    BYTE bCELL,
										BYTE bTileID)
{
	MAPDWORD::iterator finder = m_mapDIFFUSEINDEX.find(bTileID);
	int nCount = m_nUnitLength / m_nCellCount + 1;

	LPDWORD pRESULT = NULL;
	BYTE bINDEX = 0;

	if( finder == m_mapDIFFUSEINDEX.end() )
	{
		bINDEX = FindFreeDIFFUSE(pDevice);
		m_mapDIFFUSEINDEX.insert( MAPDWORD::value_type( bTileID, bINDEX));
	}
	else
		bINDEX = (BYTE) (*finder).second;

	m_vDIFFUSELOCK[bINDEX] |= DWORD(1 << m_bINDEX[bCELL]);
	pRESULT = (LPDWORD) m_vDIFFUSEBUF[bINDEX];

	return pRESULT + m_bINDEX[bCELL] * nCount * nCount;
}

void CTachyonHUGEMAP::LoadVB( CTachyonEXTMAP *pUNIT,
							  BYTE bCELL,
							  int nUnitX,
							  int nUnitZ,
							  int nCellX,
							  int nCellZ)
{
	int nCount = m_nUnitLength / m_nCellCount + 1;
	LPD3DXVECTOR3 pPOSVB = NULL;

	nCellX = nCellX * m_nUnitLength / m_nCellCount;
	nCellZ = nCellZ * m_nUnitLength / m_nCellCount;
	nUnitX *= m_nUnitLength;
	nUnitZ *= m_nUnitLength;

	m_pPOSVB->Lock( m_bINDEX[bCELL] * nCount * nCount * sizeof(D3DXVECTOR3), nCount * nCount * sizeof(D3DXVECTOR3), (LPVOID *) &pPOSVB, 0);
	for( int i=0; i<nCount; i++)
	{
		FLOAT fPosZ = FLOAT(m_nTileLength * (nCellZ + i));
		int nLocalZ = nCellZ - nUnitZ + i;

		for( int j=0; j<nCount; j++)
		{
			FLOAT fPosX = FLOAT(m_nTileLength * (nCellX + j));
			int nLocalX = nCellX - nUnitX + j;

			pPOSVB->y = pUNIT->m_pDATA[nLocalZ * (m_nUnitLength + 1) + nLocalX];
			pPOSVB->x = fPosX;
			pPOSVB->z = fPosZ;

			pPOSVB++;
		}
	}
	m_pPOSVB->Unlock();
}

void CTachyonHUGEMAP::ResetUV()
{
	int nCount = m_nUnitLength / m_nCellCount + 1;
	LPD3DXVECTOR2 pDATA = NULL;

	m_pUVVB->Lock( 0, 0, (LPVOID *) &pDATA, 0);
	for( int i=0; i<9; i++)
		if( m_bINDEX[i] != 0xFF )
		{
			LPD3DXVECTOR2 pUVVB = &pDATA[m_bINDEX[i] * nCount * nCount];

			for( int j=0; j<nCount; j++)
			{
				FLOAT fV = FLOAT((i / 3) * (nCount - 1) + j) / FLOAT(m_nUnitLength);

				for( int k=0; k<nCount; k++)
				{
					pUVVB->x = FLOAT((i % 3) * (nCount - 1) + k) / FLOAT(m_nUnitLength);
					pUVVB->y = fV;

					pUVVB++;
				}
			}
		}
	m_pUVVB->Unlock();
}

void CTachyonHUGEMAP::LoadUNIT( LPDIRECT3DDEVICE9 pDevice,
							    CTachyonEXTMAP *pUNIT,
								int nUnitID)
{
	CString strFileName;

	strFileName.Format( ".\\Data\\MAP\\EM%d_%d.tmu", m_dwID, nUnitID);
	CFile file( strFileName, CFile::modeRead|CFile::typeBinary);
	ReleaseUNIT(pUNIT);

	int nSIZE = INT(file.GetLength());
	LPBYTE pBUF = new BYTE[nSIZE];
	LPBYTE pDATA = pBUF;

	file.Read( pDATA, nSIZE);
	nSIZE = *((int *) pDATA);
	pDATA += sizeof(int);

	memcpy( pUNIT->m_pDATA, pDATA, nSIZE * sizeof(FLOAT));
	pDATA += nSIZE * sizeof(FLOAT);

	nSIZE = *((int *) pDATA);
	pDATA += sizeof(int);

	memcpy( pUNIT->m_pREGION, pDATA, nSIZE * sizeof(BYTE));
	pDATA += nSIZE * sizeof(BYTE);

	nSIZE = *((int *) pDATA);
	pDATA += sizeof(int);

	memcpy( pUNIT->m_pSHADOW, pDATA, nSIZE * sizeof(WORD));
	pDATA += nSIZE * sizeof(WORD);

	nSIZE = *((int *) pDATA);
	pDATA += sizeof(int);

	memcpy( pUNIT->m_pDETAIL, pDATA, nSIZE * sizeof(BYTE));
	pDATA += nSIZE * sizeof(BYTE);

	nSIZE = *((int *) pDATA);
	pDATA += sizeof(int);

	for( int i=0; i<nSIZE; i++)
	{
		LPMAPOBJ pOBJ = new MAPOBJ();
		DWORD dwInstID;
		DWORD dwOBJID;

		int nCICount = 0;
		int nATTR = 0;

		dwInstID = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		dwOBJID = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		pOBJ->m_vSCALE = *((LPD3DXVECTOR3) pDATA);
		pDATA += sizeof(D3DXVECTOR3);

		pOBJ->m_vPOS = *((LPD3DXVECTOR3) pDATA);
		pDATA += sizeof(D3DXVECTOR3);

		pOBJ->m_vROT = *((LPD3DXVECTOR3) pDATA);
		pDATA += sizeof(D3DXVECTOR3);

		pOBJ->m_dwOBJFuncID = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		pOBJ->m_dwACTFuncID = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		pOBJ->m_dwANIFuncID = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		pOBJ->m_dwCurACT = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		pOBJ->m_dwCurANI = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		nCICount = *((int *) pDATA);
		pDATA += sizeof(int);

		for( int j=0; j<nCICount; j++)
		{
			LPCLKINST pCI = new CLKINST();
			DWORD dwID;

			dwID = *((LPDWORD) pDATA);
			pDATA += sizeof(DWORD);

			pCI->m_dwCurCL = *((LPDWORD) pDATA);
			pDATA += sizeof(DWORD);

			pCI->m_dwCurMESH = *((LPDWORD) pDATA);
			pDATA += sizeof(DWORD);

			pOBJ->m_mapCLK.insert( MAPCLKINST::value_type( dwID, pCI));
		}

		nATTR = *((int *) pDATA);
		pDATA += sizeof(int);

		for( j=0; j<nATTR; j++)
		{
			LPATTRIBUTE pATTR = new ATTRIBUTE();
			DWORD dwATTR;

			dwATTR = *((LPDWORD) pDATA);
			pDATA += sizeof(DWORD);

			pATTR->m_bValueType = *((LPBYTE) pDATA);
			pDATA += sizeof(BYTE);

			pATTR->m_nSize = *((int *) pDATA);
			pDATA += sizeof(int);

			pATTR->m_pValue = new BYTE[pATTR->m_nSize];
			memcpy( pATTR->m_pValue, pDATA, pATTR->m_nSize * sizeof(BYTE));
			pDATA += pATTR->m_nSize * sizeof(BYTE);

			pOBJ->m_mapATTR.insert( MAPATTRIBUTE::value_type( dwATTR, pATTR));
		}

		if(m_pRES)
			pOBJ->m_pOBJ = m_pRES->GetOBJ(dwOBJID);
		pUNIT->m_mapOBJ.insert( MAPMAPOBJ::value_type( dwInstID, pOBJ));
	}

	nSIZE = *((int *) pDATA);
	pDATA += sizeof(int);

	for( i=0; i<nSIZE; i++)
	{
		LPMAPSFX pSFX = new MAPSFX();
		DWORD dwInstID;
		DWORD dwID;

		dwInstID = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		dwID = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		pSFX->m_vPOS = *((LPD3DXVECTOR3) pDATA);
		pDATA += sizeof(D3DXVECTOR3);

		pSFX->m_vROT = *((LPD3DXVECTOR3) pDATA);
		pDATA += sizeof(D3DXVECTOR3);

		pSFX->m_bBias = *((LPBYTE) pDATA);
		pDATA += sizeof(BYTE);

		MAPRES::iterator finder = m_pRES->m_mapSFX.find(dwID);
		if( finder != m_pRES->m_mapSFX.end() )
			pSFX->m_pSFX = (LPSFX) (*finder).second;

		pUNIT->m_mapSFX.insert( MAPMAPSFX::value_type( dwInstID, pSFX));
	}

	nSIZE = *((int *) pDATA);
	pDATA += sizeof(int);

	for( i=0; i<nSIZE; i++)
	{
		LPMAPSND pSND = new MAPSND();
		DWORD dwInstID;

		dwInstID = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		pSND->m_dwMediaID = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		pSND->m_vPOS = *((LPD3DXVECTOR3) pDATA);
		pDATA += sizeof(D3DXVECTOR3);

		pUNIT->m_mapSND.insert( MAPMAPSND::value_type( dwInstID, pSND));
	}

	DWORD dwCount = *((LPDWORD) pDATA);
	pDATA += sizeof(DWORD);

	if( dwCount > 0 )
	{
		D3DXIMAGE_INFO vTINFO;

		D3DXGetImageInfoFromFileInMemory(
			pDATA,
			dwCount,
			&vTINFO);

		CTachyonRes::m_pDEVICE->m_pDevice->CreateOffscreenPlainSurface(
			vTINFO.Width,
			vTINFO.Height,
			D3DFMT_R5G6B5,
			D3DPOOL_SCRATCH,
			&pUNIT->m_pGEOTEX,
			NULL);

		D3DXLoadSurfaceFromFileInMemory(
			pUNIT->m_pGEOTEX,
			NULL,
			NULL,
			pDATA,
			dwCount,
			NULL,
			D3DX_FILTER_NONE,
			NULL,
			NULL);

		pDATA += dwCount * sizeof(BYTE);
	}

	dwCount = *((LPDWORD) pDATA);
	pDATA += sizeof(DWORD);

	if( dwCount > 0 )
	{
		D3DXIMAGE_INFO vTINFO;

		D3DXGetImageInfoFromFileInMemory(
			pDATA,
			dwCount,
			&vTINFO);

		CTachyonRes::m_pDEVICE->m_pDevice->CreateOffscreenPlainSurface(
			vTINFO.Width,
			vTINFO.Height,
			D3DFMT_R5G6B5,
			D3DPOOL_SCRATCH,
			&pUNIT->m_pTILETEX,
			NULL);

		D3DXLoadSurfaceFromFileInMemory(
			pUNIT->m_pTILETEX,
			NULL,
			NULL,
			pDATA,
			dwCount,
			NULL,
			D3DX_FILTER_NONE,
			NULL,
			NULL);

		pDATA += dwCount * sizeof(BYTE);
	}

	nSIZE = *((int *) pDATA);
	pDATA += sizeof(int);

	for( i=0; i<nSIZE; i++)
	{
		LPDIRECT3DSURFACE9 pTIMG = NULL;
		D3DXIMAGE_INFO vTINFO;

		dwCount = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		D3DXGetImageInfoFromFileInMemory(
			pDATA,
			dwCount,
			&vTINFO);

		CTachyonRes::m_pDEVICE->m_pDevice->CreateOffscreenPlainSurface(
			vTINFO.Width,
			vTINFO.Height,
			D3DFMT_A8,
			D3DPOOL_SCRATCH,
			&pTIMG,
			NULL);

		D3DXLoadSurfaceFromFileInMemory(
			pTIMG,
			NULL,
			NULL,
			pDATA,
			dwCount,
			NULL,
			D3DX_FILTER_NONE,
			NULL,
			NULL);

		pUNIT->m_vSHADOWTEX.push_back(pTIMG);
		pDATA += dwCount * sizeof(BYTE);
	}

	delete[] pBUF;
}

void CTachyonHUGEMAP::ReleaseUNIT( CTachyonEXTMAP *pUNIT)
{
	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;

	for( itOBJ = pUNIT->m_mapOBJ.begin(); itOBJ != pUNIT->m_mapOBJ.end(); itOBJ++)
		if((*itOBJ).second)
			delete (*itOBJ).second;
	pUNIT->m_mapOBJ.clear();

	for( itSFX = pUNIT->m_mapSFX.begin(); itSFX != pUNIT->m_mapSFX.end(); itSFX++)
		if((*itSFX).second)
			delete (*itSFX).second;
	pUNIT->m_mapSFX.clear();

	for( itSND = pUNIT->m_mapSND.begin(); itSND != pUNIT->m_mapSND.end(); itSND++)
		if((*itSND).second)
			delete (*itSND).second;
	pUNIT->m_mapSND.clear();

	while(!pUNIT->m_vSHADOWTEX.empty())
	{
		pUNIT->m_vSHADOWTEX.back()->Release();
		pUNIT->m_vSHADOWTEX.pop_back();
	}

	if(pUNIT->m_pTILETEX)
	{
		pUNIT->m_pTILETEX->Release();
		pUNIT->m_pTILETEX = NULL;
	}

	if(pUNIT->m_pGEOTEX)
	{
		pUNIT->m_pGEOTEX->Release();
		pUNIT->m_pGEOTEX = NULL;
	}
}
