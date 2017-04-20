#include "StdAfx.h"
#include "TMonSpawnInspectorHUGEMAP.h"

CTMonSpawnInspectorHUGEMAP::CTMonSpawnInspectorHUGEMAP(void)
{
}

CTMonSpawnInspectorHUGEMAP::~CTMonSpawnInspectorHUGEMAP(void)
{
}

int CTMonSpawnInspectorHUGEMAP::GetUNITID( D3DXVECTOR3 vSpawnPos )
{
	IPOINT vPIVOT(
		INT(vSpawnPos.x / m_nTileLength),
		INT(vSpawnPos.z / m_nTileLength));

	int nUnitX = vPIVOT.m_nX / m_nUnitLength;
	int nUnitZ = vPIVOT.m_nY / m_nUnitLength;

	m_nUnitX = nUnitX;
	m_nUnitZ = nUnitZ;

	int nUnitID = nUnitZ * m_nUnitCountX + nUnitX;

	return nUnitID;
}

BOOL CTMonSpawnInspectorHUGEMAP::CheckAvailableLoadUNIT(
	INT nUnitID)
{
	CString strFileName;
	strFileName.Format( ".\\Data\\MAP\\EM%d_%d.tmu", m_dwID, nUnitID);

	FILE* fp = fopen( (LPCTSTR)strFileName, "rb" );

	if( !fp )
		return FALSE;

	fclose(fp);
	return TRUE;
}

void CTMonSpawnInspectorHUGEMAP::LoadUNIT( 
	INT nUnitID )
{
	// 유닛은 한번에 한개씩만 로딩할꺼니깐 m_UNIT[0]만 쓰도록 한다.
	m_pUNIT[0] = &m_UNIT[0];
	m_bUNIT[0] = 0;

	LoadUNIT( NULL, m_pUNIT[0], nUnitID );
}

void CTMonSpawnInspectorHUGEMAP::UnloadUNIT()
{
	CTachyonEXTMAP *pUNIT = m_pUNIT[0];

	{
		MAPMAPOBJ::iterator itOBJ;
		for( itOBJ = pUNIT->m_mapOBJ.begin(); itOBJ != pUNIT->m_mapOBJ.end(); itOBJ++)
			if((*itOBJ).second)
				delete (*itOBJ).second;
		pUNIT->m_mapOBJ.clear();
	}

	{
		MAPMAPSFX::iterator itSFX;
		for( itSFX = pUNIT->m_mapSFX.begin(); itSFX != pUNIT->m_mapSFX.end(); itSFX++)
			if((*itSFX).second)
				delete (*itSFX).second;
		pUNIT->m_mapSFX.clear();
	}

	{
		MAPMAPSND::iterator itSND;
		for( itSND = pUNIT->m_mapSND.begin(); itSND != pUNIT->m_mapSND.end(); itSND++)
			if((*itSND).second)
				delete (*itSND).second;
		pUNIT->m_mapSND.clear();
	}
}

void CTMonSpawnInspectorHUGEMAP::LoadUNIT( LPDIRECT3DDEVICE9 pDevice,
										  CTachyonEXTMAP *pUNIT,
										  int nUnitID)
{
	CString strFileName;

	strFileName.Format( ".\\Data\\MAP\\EM%d_%d.tmu", m_dwID, nUnitID);
	CFile file( strFileName, CFile::modeRead|CFile::typeBinary);

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

	SetProgressRange( 0, nSIZE );
	SetProgressPos( 0 );

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

		SetProgressPos( i );
	}

	nSIZE = *((int *) pDATA);
	pDATA += sizeof(int);

	for( i=0; i<nSIZE; i++)
	{
		MAPSFX SFX;
		LPMAPSFX pSFX = &SFX;//new MAPSFX();
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

		/*MAPRES::iterator finder = m_pRES->m_mapSFX.find(dwID);
		if( finder != m_pRES->m_mapSFX.end() )
		pSFX->m_pSFX = (LPSFX) (*finder).second;*/

		//pUNIT->m_mapSFX.insert( MAPMAPSFX::value_type( dwInstID, pSFX));
	}

	nSIZE = *((int *) pDATA);
	pDATA += sizeof(int);

	for( i=0; i<nSIZE; i++)
	{
		MAPSND SND;
		LPMAPSND pSND = &SND;
		//LPMAPSND pSND = new MAPSND();
		DWORD dwInstID;

		dwInstID = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		pSND->m_dwMediaID = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		pSND->m_vPOS = *((LPD3DXVECTOR3) pDATA);
		pDATA += sizeof(D3DXVECTOR3);

		//pUNIT->m_mapSND.insert( MAPMAPSND::value_type( dwInstID, pSND));
	}

	DWORD dwCount = *((LPDWORD) pDATA);
	pDATA += sizeof(DWORD);

	if( dwCount > 0 )
	{
		/*D3DXCreateTextureFromFileInMemoryEx(
		CTachyonRes::m_pDEVICE->m_pDevice,
		pDATA,
		dwCount,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		1, 0,
		D3DFMT_DXT1,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0, NULL,
		NULL,
		&pUNIT->m_pGEOTEX);*/

		pDATA += dwCount * sizeof(BYTE);
	}

	dwCount = *((LPDWORD) pDATA);
	pDATA += sizeof(DWORD);

	if( dwCount > 0 )
	{
		/*if( CTachyonRes::m_pDEVICE )
		D3DXCreateTextureFromFileInMemoryEx(
		CTachyonRes::m_pDEVICE->m_pDevice,
		pDATA,
		dwCount,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		1, 0,
		D3DFMT_DXT1,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0, NULL,
		NULL,
		&pUNIT->m_pTILETEX);*/

		pDATA += dwCount * sizeof(BYTE);
	}

	nSIZE = *((int *) pDATA);
	pDATA += sizeof(int);

	for( i=0; i<nSIZE; i++)
	{
		LPDIRECT3DTEXTURE9 pTEX = NULL;

		dwCount = *((LPDWORD) pDATA);
		pDATA += sizeof(DWORD);

		/*if( CTachyonRes::m_pDEVICE )
		{
		D3DXCreateTextureFromFileInMemoryEx(
		CTachyonRes::m_pDEVICE->m_pDevice,
		pDATA,
		dwCount,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		1, 0,
		D3DFMT_DXT1,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0, NULL,
		NULL, &pTEX);

		pUNIT->m_vSHADOWTEX.push_back(pTEX);
		}*/

		pDATA += dwCount * sizeof(BYTE);
	}

	delete[] pBUF;
}

void CTMonSpawnInspectorHUGEMAP::InitVB( CD3DDevice *pDevice)
{
	static PVERTEX vMESH[4] = {
		{ 0.0f, 0.0f,  0.0f, 0.0f, 0.0f},
		{ 1.0f, 0.0f,  0.0f, 1.0f, 0.0f},
		{ 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
		{ 1.0f, 0.0f, -1.0f, 1.0f, 1.0f}};
		LPPVERTEX pDATA = NULL;

		int nLength = m_nUnitLength / m_nCellCount;
		int nCount = nLength + 1;
		Release();

		/*for( int i=0; i<9; i++)
		{
		CRect rect( 0, 0, nLength, nLength);

		rect.OffsetRect(
		-nLength / 2,
		-nLength / 2);

		rect.OffsetRect(
		(i % 3 - 1) * nLength,
		(i / 3 - 1) * nLength);

		BuildBLOCK(rect);
		}*/

		m_pENABLE = new BYTE[m_nUnitCountX * m_nUnitCountZ];
		m_pBLOCK = new LPBLOCK[INT(m_vBLOCK.size())];
		ComplateBLOCK();

		nLength = 9 * nLength * nLength;
		nCount = 9 * nCount * nCount;
		m_nBlockCount = 0;

		/*for( i=0; i<m_nDiffuseCount; i++)
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
		InitForNULLTILE();*/

		/*pDevice->m_pDevice->CreateIndexBuffer(
		pDevice->m_vCAPS.MaxVertexIndex > 0x0000FFFF ?
		m_nIndexCount * sizeof(DWORD) :
		m_nIndexCount * sizeof(WORD),
		D3DUSAGE_DYNAMIC,
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

		pDevice->m_pDevice->CreateVertexBuffer(
		4 * sizeof(PVERTEX),
		0, T3DFVF_PVERTEX,
		D3DPOOL_MANAGED,
		&m_pTEXVB, NULL);

		m_pTEXVB->Lock( 0, 0, (LPVOID *) &pDATA, 0);
		memcpy( pDATA, vMESH, 4 * sizeof(PVERTEX));
		m_pTEXVB->Unlock();*/
}

FLOAT CTMonSpawnInspectorHUGEMAP::GetHeight(
	FLOAT fPosX,
	FLOAT fPosY,
	FLOAT fPosZ)
{
	fPosX /= FLOAT(m_nTileLength);
	fPosZ /= FLOAT(m_nTileLength);

	int nTileX = INT(fPosX);
	int nTileZ = INT(fPosZ);

	fPosX -= FLOAT(nTileX);
	fPosZ -= FLOAT(nTileZ);

	FLOAT fResX = FLOAT(nTileX % m_nUnitLength) + fPosX;
	FLOAT fResZ = FLOAT(nTileZ % m_nUnitLength) + fPosZ;

	return m_pUNIT[0]->GetHeight(fResX, fPosY, fResZ);
}