#include "StdAfx.h"

CD3DDevice *CTachyonRes::m_pDEVICE = NULL;
CTachyonMedia CTachyonRes::m_MEDIA;

#ifdef MODIFY_LOADING
VECTORTLOADINGSCENE CTachyonRes::m_vTLOADINGSCENE;
DWORD CTachyonRes::m_dwBackColor;
#else
VECTORIMAGE CTachyonRes::m_vBACKIMG;
CD3DImage CTachyonRes::m_vGAUGE;
int CTachyonRes::m_nGaugeX = 0;
int CTachyonRes::m_nGaugeY = 0;
int CTachyonRes::m_nTextX = 0;
int CTachyonRes::m_nTextY = 0;
#endif

CD3DFont CTachyonRes::m_vTEXT;
CFont CTachyonRes::m_vFONT;

BOOL CTachyonRes::m_bUseNullTex = FALSE;
LPDIRECT3DTEXTURE9 CTachyonRes::m_pNULLTEX = NULL;
// Arab Ver.
BOOL CTachyonRes::m_bInverseDirection = FALSE;
BOOL CTachyonRes::m_bShowbar = FALSE;

CTachyonRes::CTachyonRes()
{
	m_mapIMGBUF.clear();
	m_mapTEXSRC.clear();
	m_mapIMGSRC.clear();
	m_mapANISRC.clear();
	m_mapMEDIA.clear();
	m_mapTEX.clear();
	m_mapIMG.clear();
	m_mapANI.clear();
	m_mapMESH.clear();
	m_mapOBJ.clear();
	m_mapSFX.clear();
	m_mapMAP.clear();

	m_vMESHFILE.clear();
	m_vANIFILE.clear();
	m_vMAPFILE.clear();
	m_bGroupID = 0;

	m_pNULLTEX = NULL;
}

CTachyonRes::~CTachyonRes()
{
	MAPT3DTEXTURE::iterator itIMGBUF;
	MAPRES::iterator it;

	for( it = m_mapTEXSRC.begin(); it != m_mapTEXSRC.end(); it++)
		delete ((CT3DTexture *) (*it).second);
	m_mapTEXSRC.clear();

	for( itIMGBUF = m_mapIMGBUF.begin(); itIMGBUF != m_mapIMGBUF.end(); itIMGBUF++)
		delete (*itIMGBUF).second;
	m_mapIMGBUF.clear();

	for( it = m_mapTEX.begin(); it != m_mapTEX.end(); it++)
		delete ((LPTEXTURESET) (*it).second);
	m_mapTEX.clear();

	for( it = m_mapIMGSRC.begin(); it != m_mapIMGSRC.end(); it++)
		delete ((CD3DImage *) (*it).second);
	m_mapIMGSRC.clear();

	for( it = m_mapIMG.begin(); it != m_mapIMG.end(); it++)
		delete ((LPIMAGESET) (*it).second);
	m_mapIMG.clear();

	for( it = m_mapANISRC.begin(); it != m_mapANISRC.end(); it++)
		delete ((CTachyonAnimation *) (*it).second);
	m_mapANISRC.clear();

	for( it = m_mapANI.begin(); it != m_mapANI.end(); it++)
		delete ((LPANISET) (*it).second);
	m_mapANI.clear();

	for( it = m_mapMESH.begin(); it != m_mapMESH.end(); it++)
		delete ((CTachyonMesh *) (*it).second);
	m_mapMESH.clear();

	for( it = m_mapOBJ.begin(); it != m_mapOBJ.end(); it++)
		delete ((LPOBJECT) (*it).second);
	m_mapOBJ.clear();

	for( it = m_mapSFX.begin(); it != m_mapSFX.end(); it++)
		delete ((LPSFX) (*it).second);
	m_mapSFX.clear();

	for( it = m_mapMAP.begin(); it != m_mapMAP.end(); it++)
		delete ((LPRESINDEX) (*it).second);

	m_mapMEDIA.clear();
	m_mapMAP.clear();

	m_vMESHFILE.clear();
	m_vANIFILE.clear();
	m_vMAPFILE.clear();
}
#ifdef MODIFY_LOADING
void CTachyonRes::RenderBACK( BYTE bSTEP,
							  BYTE bProgress)
{
	static BYTE bSTART[][2] = {
		{ 0, 6 },	// T3DRES_TEX
		{ 6, 6 },	// T3DRES_IMGBUF
		{ 12, 2 },	// T3DRES_IMG
		{ 14, 2 },	// T3DRES_MEDIA
		{ 16, 13 },	// T3DRES_ANI
		{ 29, 13 },	// T3DRES_MESH
		{ 42, 7 },	// T3DRES_OBJ
		{ 49, 8 },	// T3DRES_SFX
		{ 57, 1 },	// T3DRES_MAP
		{ 58, 4 }, // TIF Loading
		{ 62, 8 }, // Char Loading 1
		{ 70, 7 }, // Char Loading 2
		{ 77, 6 }, // Char Loading 3
		{ 83, 6 }, // Char Loading 4
		{ 89, 6 }, // Char Loading 5
		{ 95, 5 } }; // Char Loading 6
	static BYTE bPrvProgress = 0xFF;

	BOOL bExistRealTime = FALSE;

	for( INT i=0 ; i < CTachyonRes::m_vTLOADINGSCENE.size() ; ++i )
	{
		LPTLOADINGSCENE pLoadingScene = CTachyonRes::m_vTLOADINGSCENE[ i ];
		if( pLoadingScene->m_nState &&
			pLoadingScene->m_bRealTime )
		{
			bExistRealTime = TRUE;
			break;
		}
	}

	if( CTachyonRes::m_vTLOADINGSCENE.empty() )
		return ;

	if( !bExistRealTime && bPrvProgress == bProgress )
		return ;

	bPrvProgress = bProgress;

	static DWORD dwOldTime = 0;
	static BOOL bInited = FALSE;

	DWORD dwElapsedTime = 0;
	DWORD dwNowTime = timeGetTime();
	dwElapsedTime = dwNowTime - dwOldTime;
	dwOldTime = dwNowTime;

	if( !bInited )
	{
		dwOldTime = timeGetTime();
		dwElapsedTime = 0;
		bInited = TRUE;
	}

	m_pDEVICE->m_pDevice->BeginScene();

	m_pDEVICE->m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		m_dwBackColor,
		1.0f, 0);

	if( bInited )
		for( INT i=0 ; i < CTachyonRes::m_vTLOADINGSCENE.size() ; ++i )
		{
			LPTLOADINGSCENE pLoadingScene = CTachyonRes::m_vTLOADINGSCENE[ i ];
			if( 0 != pLoadingScene->m_nState )
			{
				BYTE bAlpha = 0;

				// 업데이트
				switch( pLoadingScene->m_nState )
				{
				case 1: // Fade In (Blending)
					{
						if( pLoadingScene->m_nFadeInTime == 0 )
							break;

						pLoadingScene->m_fAlpha += (FLOAT)(dwElapsedTime * 256.0f / (FLOAT)pLoadingScene->m_nFadeInTime);

						if( pLoadingScene->m_fAlpha > 256.0f )
						{
							pLoadingScene->m_fAlpha = 255.0f;
							pLoadingScene->m_nState = 2; // FadeIn끝나면 Kepp으로 넘어가자.
							bAlpha = 255;
						}
						else
							bAlpha = (INT) pLoadingScene->m_fAlpha;
					}
					break;

				case 2: // Keep (Not Blending)
					{
						bAlpha = 255;
						// Arab Ver.
						if( pLoadingScene->m_bShowBar )
							CTachyonRes::m_bShowbar = TRUE;

						if( pLoadingScene->m_nKeepTime == -1 )
							break;

						pLoadingScene->m_dwKeepTime += dwElapsedTime;

						if( pLoadingScene->m_dwKeepTime > pLoadingScene->m_nKeepTime )
							pLoadingScene->m_nState = 3; // Keep이 끝나면 Fade Out으로 가자.
					}
					break;

				case 3: // Fade Out Init
					{
						// 내가 Fade Out을 시작할 때 나의 다음번 Scene로 활성화시켜주자.
						if( CTachyonRes::m_vTLOADINGSCENE.size() > i+1 )
						{
							if( CTachyonRes::m_vTLOADINGSCENE[ i+1] )
								CTachyonRes::m_vTLOADINGSCENE[ i+1]->m_nState = 1;
						}

						pLoadingScene->m_nState = 4;
						pLoadingScene->m_fAlpha = 255.0f;
						bAlpha = 255;
					}
					break;

				case 4: // Fade Out (Blending)
					{
						pLoadingScene->m_fAlpha -= (FLOAT)(dwElapsedTime * 256.0f / (FLOAT)pLoadingScene->m_nFadeOutTime);

						if( pLoadingScene->m_fAlpha < 0.0f )
						{
							pLoadingScene->m_nState = 0;
							continue;
						}
						else
						{
							bAlpha = (INT) pLoadingScene->m_fAlpha;
						}
					}

					break;
				}

				if(bAlpha)
				{
					CD3DImage *pIMG = pLoadingScene->m_vIMG.GetIMG();

					if(pIMG)
					{
						pIMG->Render(
							m_pDEVICE->m_pDevice,
							D3DCOLOR_ARGB( bAlpha, 0xFF, 0xFF, 0xFF ),
							m_pDEVICE->m_option.m_dwScreenX / 2 - pIMG->GetWidth() / 2,
							m_pDEVICE->m_option.m_dwScreenY / 2 - pIMG->GetHeight() / 2);
					}
					// Arab Ver.
					if(CTachyonRes::m_bShowbar)
					{
						pIMG = pLoadingScene->m_vGAUGE.GetIMG();

						if(pIMG)
						{
							FLOAT fProgress = FLOAT(bSTART[bSTEP][0]) + FLOAT(bProgress) * FLOAT(bSTART[bSTEP][1]) / 100.0f;
							CRect rect(
									0, 0,
									INT((FLOAT) pIMG->GetWidth() * pIMG->GetScaleX()),
									INT((FLOAT) pIMG->GetHeight() * pIMG->GetScaleY()));
							CString strProgress;

							// Arab Ver.
							if(m_bInverseDirection)
								rect.left = rect.right - INT(rect.right * fProgress / 100.0f);
							else
								rect.right = INT(rect.right * fProgress / 100.0f);

							strProgress.Format( _T("%d%%"), INT(fProgress));

							pIMG->Render(
								m_pDEVICE->m_pDevice,
								0xFFFFFFFF,
								m_pDEVICE->m_option.m_dwScreenX/2 + pLoadingScene->m_nGaugeX,
								m_pDEVICE->m_option.m_dwScreenY/2 + pLoadingScene->m_nGaugeY,
								&rect);

							m_vTEXT.TextOut(
								m_pDEVICE->m_pDevice,
								strProgress,
								m_pDEVICE->m_option.m_dwScreenX/2 + pLoadingScene->m_nTextX,
								m_pDEVICE->m_option.m_dwScreenY/2 + pLoadingScene->m_nTextY);
							// Arab Ver.
							CTachyonRes::m_bShowbar = FALSE;
						}
					}
				}
			}
		}

	m_pDEVICE->m_pDevice->EndScene();

	if( !AfxGetApp()->m_pMainWnd->IsIconic() && !m_pDEVICE->GetResetFlag() &&
		FAILED(m_pDEVICE->m_pDevice->Present( NULL, NULL, NULL, NULL)) )
		m_pDEVICE->ResetHost();
}
#else
void CTachyonRes::RenderBACK( BYTE bSTEP,
							  BYTE bProgress)
{
	static FLOAT fPrevProgress = 0.0f;
	static BYTE bSTART[][2] = {
		{  0, 10},	// T3DRES_TEX
		{ 10, 10},	// T3DRES_IMGBUF
		{ 20,  4},	// T3DRES_IMG
		{ 24,  4},	// T3DRES_MEDIA
		{ 28, 18},	// T3DRES_ANI
		{ 46, 18},	// T3DRES_MESH
		{ 64, 14},	// T3DRES_OBJ
		{ 78, 18},	// T3DRES_SFX
		{ 96,  4}};	// T3DRES_MAP
	static BYTE bIMG = 0xFF;
	static BYTE bPrvProgress = 0xFF;

	if( m_vBACKIMG.empty() || bPrvProgress == bProgress )
		return;
	bPrvProgress = bProgress;

	m_pDEVICE->m_pDevice->BeginScene();

	m_pDEVICE->m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(255,0,0,0),
		1.0f, 0);

	CString strProgress;
	CRect rect(
		0, 0,
		(int)( (FLOAT)m_vGAUGE.GetWidth() * m_vGAUGE.GetScaleX() ),
		(int)( (FLOAT)m_vGAUGE.GetHeight() * m_vGAUGE.GetScaleY() ));
	FLOAT fProgress = FLOAT(bSTART[bSTEP][0]) + FLOAT(bProgress) * FLOAT(bSTART[bSTEP][1]) / 100.0f;

	if( bIMG == 0xFF || fProgress - fPrevProgress > 30.0f )
	{
		BYTE bCOUNT = BYTE(m_vBACKIMG.size());
		BYTE bINDEX = BYTE(rand() % bCOUNT);

		while( bCOUNT > 1 && bINDEX == bIMG )
			bINDEX = BYTE(rand() % bCOUNT);

		if( bIMG != 0xFF )
			fPrevProgress += 30.0f;
		bIMG = bINDEX;
	}
	rect.right = INT(rect.right * fProgress / 100.0f);

	m_vBACKIMG[bIMG]->Render(
		m_pDEVICE->m_pDevice,
		0xFFFFFFFF,
		0, 0);

	m_vGAUGE.Render(
		m_pDEVICE->m_pDevice,
		0xFFFFFFFF,
		m_nGaugeX,
		m_nGaugeY,
		&rect);

	strProgress.Format( _T("%d%%"), INT(fProgress));
	rect.right = m_vGAUGE.GetWidth();

	m_vTEXT.TextOut(
		m_pDEVICE->m_pDevice,
		strProgress,
		m_nTextX,
		m_nTextY);

	m_pDEVICE->m_pDevice->EndScene();

	if( !AfxGetApp()->m_pMainWnd->IsIconic() && !m_pDEVICE->GetResetFlag() &&
		FAILED(m_pDEVICE->m_pDevice->Present( NULL, NULL, NULL, NULL)) )
		m_pDEVICE->ResetHost();
}
#endif

void CTachyonRes::Init( CString strGroupID, BYTE bGroupID)
{
	m_bGroupID = bGroupID;

	Load(strGroupID);
	Complate();
}

void CTachyonRes::Load( CString strGroupID)
{
	LoadTEX(strGroupID);
	LoadIMGBUF(strGroupID);
	LoadIMG(strGroupID);
	LoadMEDIA(strGroupID);
	LoadANI(strGroupID);
	LoadMESH(strGroupID);
	LoadOBJ(strGroupID);
	LoadSFX(strGroupID);
	LoadMAP(strGroupID);
}

void CTachyonRes::Complate()
{
	ComplateTEX();
	ComplateIMG();
	ComplateANI();
	ComplateOBJ();
	ComplateSFX();

	RenderBACK( T3DRES_MAP, 100);
}

CTachyonMAP *CTachyonRes::LockMAP( DWORD dwID)
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
				pResult = new CTachyonHUGEMAP();

				file.Read( &((CTachyonHUGEMAP *) pResult)->m_nUnitCountX, sizeof(int));
				file.Read( &((CTachyonHUGEMAP *) pResult)->m_nUnitCountZ, sizeof(int));
				file.Read( &((CTachyonHUGEMAP *) pResult)->m_nUnitLength, sizeof(int));
				file.Read( &((CTachyonHUGEMAP *) pResult)->m_nTileLength, sizeof(int));
				file.Read( &((CTachyonHUGEMAP *) pResult)->m_nCellCount, sizeof(int));
				file.Read( &((CTachyonHUGEMAP *) pResult)->m_nTextureSize, sizeof(int));
				file.Read( &((CTachyonHUGEMAP *) pResult)->m_nShadowPixel, sizeof(int));
				file.Read( &((CTachyonHUGEMAP *) pResult)->m_vLIGHT, sizeof(D3DXVECTOR3));

				file.Read( &((CTachyonHUGEMAP *) pResult)->m_bUseSHADER, sizeof(BYTE));
				file.Read( &((CTachyonHUGEMAP *) pResult)->m_fIntensity, sizeof(FLOAT));
				file.Read( &((CTachyonHUGEMAP *) pResult)->m_fSpecRange, sizeof(FLOAT));
				file.Read( &((CTachyonHUGEMAP *) pResult)->m_fSpecMax, sizeof(FLOAT));
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
						((CTachyonHUGEMAP *) pResult)->m_vDETAILDATA.push_back((LPTEXTURESET) (*finder).second);
					else
						((CTachyonHUGEMAP *) pResult)->m_vDETAILDATA.push_back(NULL);
					((CTachyonHUGEMAP *) pResult)->m_vDETAILSDATA.push_back(fDetailS);
				}

				int nCount = ((CTachyonHUGEMAP *) pResult)->m_nUnitCountX * ((CTachyonHUGEMAP *) pResult)->m_nUnitCountZ;
				((CTachyonHUGEMAP *) pResult)->InitVB(m_pDEVICE);
				file.Read( ((CTachyonHUGEMAP *) pResult)->m_pENABLE, nCount * sizeof(BYTE));

				nCount = ((CTachyonHUGEMAP *) pResult)->m_nUnitLength;
				((CTachyonHUGEMAP *) pResult)->m_pRES = this;
				((CTachyonHUGEMAP *) pResult)->m_dwID = dwID;

				for( i=0; i<9; i++)
				{
					((CTachyonHUGEMAP *) pResult)->m_UNIT[i].m_nTileCount = ((CTachyonHUGEMAP *) pResult)->m_nUnitLength;
					((CTachyonHUGEMAP *) pResult)->m_UNIT[i].m_pSHADOW = new WORD[nCount * nCount];
					((CTachyonHUGEMAP *) pResult)->m_UNIT[i].m_pREGION = new BYTE[nCount * nCount];
					((CTachyonHUGEMAP *) pResult)->m_UNIT[i].m_pDETAIL = new BYTE[nCount * nCount];
					((CTachyonHUGEMAP *) pResult)->m_UNIT[i].m_pDATA = new FLOAT[(nCount + 1) * (nCount + 1)];
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

LPOBJECT CTachyonRes::GetOBJ( DWORD dwID)
{
	MAPRES::iterator finder = m_mapOBJ.find(dwID);
	return finder != m_mapOBJ.end() ? (LPOBJECT) (*finder).second : NULL;
}

void CTachyonRes::ReleaseOBJ( LPOBJECT pOBJ)
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
				(*itMESH).second->m_pMESH->Unlock();
		}
	}

	for( itACT = pOBJ->m_mapACTION.begin(); itACT != pOBJ->m_mapACTION.end(); itACT++)
	{
		MAPANI::iterator itANI;

		for( itANI = (*itACT).second->m_mapANI.begin(); itANI != (*itACT).second->m_mapANI.end(); itANI++)
			for( int i=0; i<INT((*itANI).second->m_pANI->m_vAni.size()); i++)
				(*itANI).second->m_pANI->m_vAni[i]->m_pAni->Unlock();
	}
}

void CTachyonRes::ReleaseSFX( LPSFX pSFX)
{
	if(!pSFX)
		return;

	if(pSFX->m_pSprayANI)
		for( BYTE i=0; i<BYTE(pSFX->m_pSprayANI->m_vAni.size()); i++)
			pSFX->m_pSprayANI->m_vAni[i]->m_pAni->Unlock();

	if(pSFX->m_pANISRC)
		for( BYTE i=0; i<BYTE(pSFX->m_pANISRC->m_vAni.size()); i++)
			pSFX->m_pANISRC->m_vAni[i]->m_pAni->Unlock();
}

void CTachyonRes::LockOBJ( LPOBJECT pOBJ)
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
				(*itMESH).second->m_pMESH->Lock( m_pDEVICE->m_pDevice, &m_vMESHFILE);

				if( pOBJ->m_bPivotCount < (*itMESH).second->m_pMESH->m_dwNodeCount + 1 )
					pOBJ->m_bPivotCount = (BYTE) (*itMESH).second->m_pMESH->m_dwNodeCount + 1;
			}
		}
	}

	for( itACT = pOBJ->m_mapACTION.begin(); itACT != pOBJ->m_mapACTION.end(); itACT++)
	{
		MAPANI::iterator itANI;

		for( itANI = (*itACT).second->m_mapANI.begin(); itANI != (*itACT).second->m_mapANI.end(); itANI++)
			for( int i=0; i<INT((*itANI).second->m_pANI->m_vAni.size()); i++)
			{
				(*itANI).second->m_pANI->m_vAni[i]->m_pAni->Lock(&m_vANIFILE);

				if( pOBJ->m_bPivotCount < (*itANI).second->m_pANI->m_vAni[i]->m_pAni->GetNodeCount() + 1 )
					pOBJ->m_bPivotCount = (BYTE) (*itANI).second->m_pANI->m_vAni[i]->m_pAni->GetNodeCount() + 1;
			}
	}
}

void CTachyonRes::LockOBJProgress( LPOBJECT pOBJ, BYTE bStep )
{
	if(!pOBJ)
		return;

	DWORD dwProgress = 0;
	DWORD dwTotal = pOBJ->m_mapCLKIND.size() + pOBJ->m_mapACTION.size();

	RenderBACK( bStep, 0 );
    
	for( MAPCLKIND::iterator itCLK = pOBJ->m_mapCLKIND.begin(); itCLK != pOBJ->m_mapCLKIND.end(); itCLK++)
	{
		MAPCLOTH::iterator itCL;

		for( itCL = (*itCLK).second->m_mapCLOTH.begin(); itCL != (*itCLK).second->m_mapCLOTH.end(); itCL++)
		{
			MAPMESH::iterator itMESH;

			for( itMESH = (*itCL).second->m_mapMESH.begin(); itMESH != (*itCL).second->m_mapMESH.end(); itMESH++)
			{
				(*itMESH).second->m_pMESH->Lock( m_pDEVICE->m_pDevice, &m_vMESHFILE);

				if( pOBJ->m_bPivotCount < (*itMESH).second->m_pMESH->m_dwNodeCount + 1 )
					pOBJ->m_bPivotCount = (BYTE) (*itMESH).second->m_pMESH->m_dwNodeCount + 1;
			}
		}

		RenderBACK( bStep, (BYTE)( ((FLOAT) dwProgress / (FLOAT) dwTotal) * 100.0f ) );
		++dwProgress;
	}

	for( MAPACTION::iterator itACT = pOBJ->m_mapACTION.begin(); itACT != pOBJ->m_mapACTION.end(); itACT++)
	{
		MAPANI::iterator itANI;

		for( itANI = (*itACT).second->m_mapANI.begin(); itANI != (*itACT).second->m_mapANI.end(); itANI++)
			for( int i=0; i<INT((*itANI).second->m_pANI->m_vAni.size()); i++)
			{
				(*itANI).second->m_pANI->m_vAni[i]->m_pAni->Lock(&m_vANIFILE);

				if( pOBJ->m_bPivotCount < (*itANI).second->m_pANI->m_vAni[i]->m_pAni->GetNodeCount() + 1 )
					pOBJ->m_bPivotCount = (BYTE) (*itANI).second->m_pANI->m_vAni[i]->m_pAni->GetNodeCount() + 1;
			}

		RenderBACK( bStep, (BYTE)( ((FLOAT) dwProgress / (FLOAT) dwTotal) * 100.0f ) );
		++dwProgress;
	}
}

void CTachyonRes::LockSFX( LPSFX pSFX)
{
	if(!pSFX)
		return;

	if(pSFX->m_pSprayANI)
		for( BYTE i=0; i<BYTE(pSFX->m_pSprayANI->m_vAni.size()); i++)
			pSFX->m_pSprayANI->m_vAni[i]->m_pAni->Lock(&m_vANIFILE);

	if(pSFX->m_pANISRC)
		for( BYTE i=0; i<BYTE(pSFX->m_pANISRC->m_vAni.size()); i++)
			pSFX->m_pANISRC->m_vAni[i]->m_pAni->Lock(&m_vANIFILE);
}

void CTachyonRes::LoadTEX( CString strGroupID)
{
	CString strINDEX;

	int nTotal = 0;
	int nCount = 0;
	int nIndex = 0;

	strINDEX.Format( ".\\Index\\%u_%sS.IDX", m_pDEVICE->m_option.m_nTextureDetail, strGroupID);
	CFile file( strINDEX, CFile::modeRead|CFile::typeBinary);

	file.Read( &nCount, sizeof(int));
	file.Read( &nTotal, sizeof(int));

	LPMAPRES pTRESDATA = new MAPRES[nCount];
	LPMAPRES pTRES = new MAPRES[nCount];
	RenderBACK( T3DRES_TEX, 0);

	for( int i=0; i<nCount; i++)
		LoadTEX( LoadString(&file), &pTRESDATA[i], &pTRES[i], nIndex, nTotal);

	for( i=0; i<nTotal; i++)
	{
		DWORD dwFileID;
		DWORD dwPOS;
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &dwFileID, sizeof(DWORD));
		file.Read( &dwPOS, sizeof(DWORD));

		MAPRES::iterator finder = pTRESDATA[dwFileID].find(dwPOS);
		if( finder != pTRESDATA[dwFileID].end() )
		{
			m_mapTEXSRC.insert( MAPRES::value_type( dwID, (*finder).second));
			pTRESDATA[dwFileID].erase(finder);
		}

		finder = pTRES[dwFileID].find(dwPOS);
		if( finder != pTRES[dwFileID].end() )
		{
			m_mapTEX.insert( MAPRES::value_type( dwID, (*finder).second));
			pTRES[dwFileID].erase(finder);
		}
	}

	for( i=0; i<nCount; i++)
	{
		MAPRES::iterator it;

		if(!m_bUseNullTex)
			for( it = pTRESDATA[i].begin(); it != pTRESDATA[i].end(); it++)
				((LPDIRECT3DTEXTURE9) (*it).second)->Release();
		pTRESDATA[i].clear();

		for( it = pTRES[i].begin(); it != pTRES[i].end(); it++)
			delete (*it).second;
		pTRES[i].clear();
	}

	delete[] pTRESDATA;
	delete[] pTRES;
}

void CTachyonRes::LoadIMGBUF( CString strGroupID)
{
	CString strINDEX;

	int nTotal = 0;
	int nCount = 0;
	int nIndex = 0;

	strINDEX.Format( ".\\Index\\%sList.LST", strGroupID);
	CFile file( strINDEX, CFile::modeRead|CFile::typeBinary);

	file.Read( &nCount, sizeof(int));
	file.Read( &nTotal, sizeof(int));
	RenderBACK( T3DRES_IMGBUF, 0);

	for( int i=0; i<nCount; i++)
		LoadIMGBUF( LoadString(&file), nIndex, nTotal);
}

void CTachyonRes::LoadIMG( CString strGroupID)
{
	CString strINDEX;

	int nTotal = 0;
	int nCount = 0;
	int nIndex = 0;

	strINDEX.Format( ".\\Index\\%sI.IDX", strGroupID);
	CFile file( strINDEX, CFile::modeRead|CFile::typeBinary);

	file.Read( &nCount, sizeof(int));
	file.Read( &nTotal, sizeof(int));

	LPMAPRES pTRESDATA = new MAPRES[nCount];
	LPMAPRES pTRES = new MAPRES[nCount];
	RenderBACK( T3DRES_IMG, 0);

	for( int i=0; i<nCount; i++)
		LoadIMG( LoadString(&file), &pTRESDATA[i], &pTRES[i], nIndex, nTotal);

	for( i=0; i<nTotal; i++)
	{
		DWORD dwFileID;
		DWORD dwPOS;
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &dwFileID, sizeof(DWORD));
		file.Read( &dwPOS, sizeof(DWORD));

		MAPRES::iterator finder = pTRESDATA[dwFileID].find(dwPOS);
		if( finder != pTRESDATA[dwFileID].end() )
		{
			m_mapIMGSRC.insert( MAPRES::value_type( dwID, (*finder).second));
			pTRESDATA[dwFileID].erase(finder);
		}

		finder = pTRES[dwFileID].find(dwPOS);
		if( finder != pTRES[dwFileID].end() )
		{
			m_mapIMG.insert( MAPRES::value_type( dwID, (*finder).second));
			pTRES[dwFileID].erase(finder);
		}
	}

	for( i=0; i<nCount; i++)
	{
		MAPRES::iterator it;

		for( it = pTRESDATA[i].begin(); it != pTRESDATA[i].end(); it++)
			delete (*it).second;
		pTRESDATA[i].clear();

		for( it = pTRES[i].begin(); it != pTRES[i].end(); it++)
			delete (*it).second;
		pTRES[i].clear();
	}

	delete[] pTRESDATA;
	delete[] pTRES;
}

void CTachyonRes::LoadMEDIA( CString strGroupID)
{
	VECTORSTRING vFILE;
	CString strINDEX;

	int nTotal = 0;
	int nCount = 0;

	strINDEX.Format( ".\\Index\\%sW.IDX", strGroupID);
	CFile file( strINDEX, CFile::modeRead|CFile::typeBinary);

	file.Read( &nCount, sizeof(int));
	file.Read( &nTotal, sizeof(int));
	RenderBACK( T3DRES_MEDIA, 0);

	for( int i=0; i<nCount; i++)
	{
		CString strFILE = LoadString(&file);
		vFILE.push_back(strFILE);
	}

	for( i=0; i<nTotal; i++)
	{
		DWORD dwFileID;
		DWORD dwPOS;
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &dwFileID, sizeof(DWORD));
		file.Read( &dwPOS, sizeof(DWORD));

		LoadMEDIA( dwID, vFILE[dwFileID], dwPOS);
		RenderBACK( T3DRES_MEDIA, BYTE(i * 100 / nTotal));
	}
}

void CTachyonRes::LoadANI( CString strGroupID)
{
	CString strINDEX;

	int nTotal = 0;
	int nCount = 0;
	int nIndex = 0;

	strINDEX.Format( ".\\Index\\%sA.IDX", strGroupID);
	CFile file( strINDEX, CFile::modeRead|CFile::typeBinary);

	file.Read( &nCount, sizeof(int));
	file.Read( &nTotal, sizeof(int));

	LPMAPRES pTRESDATA = new MAPRES[nCount];
	LPMAPRES pTRES = new MAPRES[nCount];
	RenderBACK( T3DRES_ANI, 0);

	for( int i=0; i<nCount; i++)
	{
		m_vANIFILE.push_back(LoadString(&file));
		LoadANI( DWORD(i), &pTRESDATA[i], &pTRES[i], nIndex, nTotal);
	}

	for( i=0; i<nTotal; i++)
	{
		DWORD dwFileID;
		DWORD dwPOS;
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &dwFileID, sizeof(DWORD));
		file.Read( &dwPOS, sizeof(DWORD));

		MAPRES::iterator finder = pTRESDATA[dwFileID].find(dwPOS);
		if( finder != pTRESDATA[dwFileID].end() )
		{
			m_mapANISRC.insert( MAPRES::value_type( dwID, (*finder).second));
			pTRESDATA[dwFileID].erase(finder);
		}

		finder = pTRES[dwFileID].find(dwPOS);
		if( finder != pTRES[dwFileID].end() )
		{
			m_mapANI.insert( MAPRES::value_type( dwID, (*finder).second));
			pTRES[dwFileID].erase(finder);
		}
	}

	for( i=0; i<nCount; i++)
	{
		MAPRES::iterator it;

		for( it = pTRESDATA[i].begin(); it != pTRESDATA[i].end(); it++)
			delete (*it).second;
		pTRESDATA[i].clear();

		for( it = pTRES[i].begin(); it != pTRES[i].end(); it++)
			delete (*it).second;
		pTRES[i].clear();
	}

	delete[] pTRESDATA;
	delete[] pTRES;
}

void CTachyonRes::LoadMESH( CString strGroupID)
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
	RenderBACK( T3DRES_MESH, 0);

	for( int i=0; i<nCount; i++)
	{
		CTachyonMesh::m_vFILEVB.push_back(new FILEVB());
		m_vMESHFILE.push_back(LoadString(&file));

		LoadMESH( dwBASE, DWORD(i), &pTRES[i], nIndex, nTotal);
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
	}

	for( i=0; i<nCount; i++)
	{
		MAPRES::iterator it;

		for( it = pTRES[i].begin(); it != pTRES[i].end(); it++)
			delete (*it).second;
		pTRES[i].clear();
	}

	delete[] pTRES;
}

void CTachyonRes::LoadOBJ( CString strGroupID)
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
	RenderBACK( T3DRES_OBJ, 0);

	for( int i=0; i<nCount; i++)
		LoadOBJ( LoadString(&file), &pTRES[i], nIndex, nTotal);

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
	}

	for( i=0; i<nCount; i++)
	{
		MAPRES::iterator it;

		for( it = pTRES[i].begin(); it != pTRES[i].end(); it++)
			delete (*it).second;
		pTRES[i].clear();
	}

	delete[] pTRES;
}

void CTachyonRes::LoadSFX( CString strGroupID)
{
	CString strINDEX;

	int nTotal = 0;
	int nCount = 0;
	int nIndex = 0;

	strINDEX.Format( ".\\Index\\%sX.IDX", strGroupID);
	CFile file( strINDEX, CFile::modeRead|CFile::typeBinary);

	file.Read( &nCount, sizeof(int));
	file.Read( &nTotal, sizeof(int));

	LPMAPRES pTRES = new MAPRES[nCount];
	RenderBACK( T3DRES_SFX, 0);

	for( int i=0; i<nCount; i++)
		LoadSFX( LoadString(&file), &pTRES[i], nIndex, nTotal);

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
			m_mapSFX.insert( MAPRES::value_type( dwID, (*finder).second));
			pTRES[dwFileID].erase(finder);
		}
	}

	for( i=0; i<nCount; i++)
	{
		MAPRES::iterator it;

		for( it = pTRES[i].begin(); it != pTRES[i].end(); it++)
			delete (*it).second;
		pTRES[i].clear();
	}

	delete[] pTRES;
}

void CTachyonRes::LoadMAP( CString strGroupID)
{
	CString strINDEX;
	int nTotal = 0;
	int nCount = 0;

	strINDEX.Format( ".\\Index\\%sP.IDX", strGroupID);
	CFile file( strINDEX, CFile::modeRead|CFile::typeBinary);

	file.Read( &nCount, sizeof(int));
	file.Read( &nTotal, sizeof(int));
	RenderBACK( T3DRES_MAP, 0);

	for( int i=0; i<nCount; i++)
		m_vMAPFILE.push_back(LoadString(&file));

	for( i=0; i<nTotal; i++)
	{
		LPRESINDEX pRES = new RESINDEX();
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &pRES->m_dwFileID, sizeof(DWORD));
		file.Read( &pRES->m_dwPOS, sizeof(DWORD));

		m_mapMAP.insert( MAPRES::value_type( dwID, pRES));
		RenderBACK( T3DRES_MAP, BYTE(i * 100 / nTotal));
	}
}

void CTachyonRes::LoadTEX( CString strFILE,
						   LPMAPRES pTRESDATA,
						   LPMAPRES pTRES,
						   int& nIndex,
						   int nTotal)
{
	CFile file( _T(".\\Data\\") + strFILE, CFile::modeRead|CFile::typeBinary);

	DWORD dwLENGTH = DWORD(file.GetLength());
	DWORD dwPOS = DWORD(file.GetPosition());

	while( dwPOS < dwLENGTH )
	{
		CTachyonUncompressor ucpr;
		ucpr.FromFile(&file);

		LPTEXTURESET pTEX = new TEXTURESET();

		int nCount = 0;

		ucpr.Read( &nCount, sizeof(int));
		for( int i=0; i<nCount; i++)
		{
			CT3DTexture *pDATA;

			ucpr.Read( &pDATA, sizeof(DWORD));
			pTEX->PushTexturePtr(pDATA);
		}

		ucpr.Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			LPUVKEY pKEY = new UVKEY();

			ucpr.Read( &pKEY->m_dwTick, sizeof(DWORD));
			ucpr.Read( &pKEY->m_fKeyU, sizeof(FLOAT));
			ucpr.Read( &pKEY->m_fKeyV, sizeof(FLOAT));
			ucpr.Read( &pKEY->m_fKeyR, sizeof(FLOAT));
			ucpr.Read( &pKEY->m_fKeySU, sizeof(FLOAT));
			ucpr.Read( &pKEY->m_fKeySV, sizeof(FLOAT));

			pTEX->m_vKey.push_back(pKEY);
		}

		ucpr.Read( &pTEX->m_dwTotalTick, sizeof(DWORD));
		ucpr.Read( &pTEX->m_dwMipFilter, sizeof(DWORD));
		ucpr.Read( &pTEX->m_fMipBias, sizeof(FLOAT));

		BYTE bTextureOption;
		ucpr.Read( &bTextureOption, sizeof(BYTE));

		BYTE bFormat;
		ucpr.Read( &bFormat, sizeof(BYTE));

		DWORD dwOrgSize = 0;
		ucpr.Read( &dwOrgSize, sizeof(DWORD));

		DWORD dwSize = 0;
		ucpr.Read( &dwSize, sizeof(DWORD));

		if( dwSize > 0 )
		{
			CT3DTexture *pTEXSRC = new CT3DTexture();

			if(m_bUseNullTex)
			{
				ucpr.Seek( dwSize, CFile::current);
				LoadNullTEX();

				pTEXSRC->m_pTDATA = (LPBYTE) m_pNULLTEX;
				pTEXSRC->m_bEnabled = TRUE;
			}
			else
			{
				LPBYTE pDATA = (LPBYTE) ucpr.GetCurData();

				pTEXSRC->LoadT3DTEX( pDATA, dwSize, dwOrgSize, bFormat);
				ucpr.Seek( dwSize, CFile::current);
			}

			pTRESDATA->insert( MAPRES::value_type( dwPOS, pTEXSRC));
		}

		pTRES->insert( MAPRES::value_type( dwPOS, pTEX));
		dwPOS = DWORD(file.GetPosition());
		nIndex++;

		RenderBACK( T3DRES_TEX, BYTE(nIndex * 100 / nTotal));
	}
}

void CTachyonRes::LoadIMGBUF( CString strFILE,
							  int& nIndex,
							  int nTotal)
{
	CFile file( _T(".\\Data\\") + strFILE, CFile::modeRead|CFile::typeBinary);

	DWORD dwLENGTH = DWORD(file.GetLength());
	DWORD dwPOS = DWORD(file.GetPosition());

	while( dwPOS < dwLENGTH )
	{
		CTachyonUncompressor ucpr;
		ucpr.FromFile(&file);

		CT3DTexture *pTEX = new CT3DTexture();
		BYTE bFormat;

		DWORD dwDATA;
		DWORD dwSIZE;
		DWORD dwID;

		ucpr.Read( &dwID, sizeof(DWORD));
		ucpr.Read( &bFormat, sizeof(BYTE));
		ucpr.Read( &dwSIZE, sizeof(DWORD));
		ucpr.Read( &dwDATA, sizeof(DWORD));

		LPBYTE pDATA = (LPBYTE) ucpr.GetCurData();
		ucpr.Seek( dwDATA, CFile::current);

		pTEX->LoadT3DTEX(
			pDATA,
			dwDATA,
			dwSIZE,
			bFormat);

		m_mapIMGBUF.insert( MAPT3DTEXTURE::value_type( dwID, pTEX));
		dwPOS = DWORD(file.GetPosition());
		nIndex++;

		RenderBACK( T3DRES_IMGBUF, BYTE(nIndex * 100 / nTotal));
	}
}

void CTachyonRes::LoadIMG( CString strFILE,
						   LPMAPRES pTRESDATA,
						   LPMAPRES pTRES,
						   int& nIndex,
						   int nTotal)
{
	CFile file( _T(".\\Data\\") + strFILE, CFile::modeRead|CFile::typeBinary);

	DWORD dwLENGTH = DWORD(file.GetLength());
	DWORD dwPOS = DWORD(file.GetPosition());

	while( dwPOS < dwLENGTH )
	{
		CTachyonUncompressor ucpr;
		ucpr.FromFile(&file);

		LPIMAGESET pIMG = new IMAGESET();
		int nCount = 0;

		ucpr.Read( &nCount, sizeof(int));
		for( int i=0; i<nCount; i++)
		{
			CD3DImage *pDATA;

			ucpr.Read( &pDATA, sizeof(DWORD));
			pIMG->m_vImage.push_back(pDATA);
		}

		ucpr.Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			LPCLRKEY pKEY = new CLRKEY();

			ucpr.Read( &pKEY->m_dwTick, sizeof(DWORD));
			ucpr.Read( &pKEY->m_dwColor, sizeof(DWORD));

			pIMG->m_vKey.push_back(pKEY);
		}

		ucpr.Read( &pIMG->m_dwTotalTick, sizeof(DWORD));

		BYTE bFormat = NON_COMP;
		ucpr.Read( &bFormat, sizeof(BYTE));

		DWORD dwSize = 0;
		ucpr.Read( &dwSize, sizeof(DWORD));

		if( dwSize > 0 )
		{
			CD3DImage *pIMGSRC = new CD3DImage();

			LPBYTE pDATA = new BYTE[dwSize];
			ucpr.Read( pDATA, dwSize);

			pIMGSRC->Load(
				&m_mapIMGBUF,
				pDATA);

			pTRESDATA->insert( MAPRES::value_type( dwPOS, pIMGSRC));
			delete[] pDATA;
		}

		pTRES->insert( MAPRES::value_type( dwPOS, pIMG));
		dwPOS = DWORD(file.GetPosition());
		nIndex++;

		RenderBACK( T3DRES_IMG, BYTE(nIndex * 100 / nTotal));
	}
}

void CTachyonRes::LoadMEDIA( DWORD dwID, CString strFILE, DWORD dwPOS)
{
	strFILE = _T(".\\Data\\") + strFILE;

	switch(LOWORD(dwPOS))
	{
	case MEDIA_TSHOW	:
		{
			m_MEDIA.LoadDShow(
				NULL, NULL,
				dwID, 0,
				(LPTSTR) LPCSTR(strFILE),
				BYTE(HIWORD(dwPOS)));
		}

		break;

	case MEDIA_TSOUND	: m_MEDIA.LoadDSound( dwID, (LPTSTR) LPCSTR(strFILE), BYTE(HIWORD(dwPOS))); break;
	case MEDIA_TMUSIC	: m_MEDIA.LoadDMusic( dwID, (LPTSTR) LPCSTR(strFILE), BYTE(HIWORD(dwPOS))); break;
	}
}

void CTachyonRes::LoadANI( DWORD dwFileID,
						   LPMAPRES pTRESDATA,
						   LPMAPRES pTRES,
						   int& nIndex,
						   int nTotal)
{
	CFile file( _T(".\\Data\\") + m_vANIFILE[dwFileID], CFile::modeRead|CFile::typeBinary);

	DWORD dwLENGTH = DWORD(file.GetLength());
	DWORD dwPOS = DWORD(file.GetPosition());

	while( dwPOS < dwLENGTH )
	{
		LPANISET pANI = new ANISET();
		int nCount = 0;

		file.Read( &nCount, sizeof(int));
		for( int i=0; i<nCount; i++)
		{
			LPANIDATA pDATA = new ANIDATA();

			file.Read( &pDATA->m_pAni, sizeof(DWORD));
			file.Read( &pDATA->m_bReverse, sizeof(BYTE));

			pANI->m_vAni.push_back(pDATA);
		}

		file.Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			LPANIKEY pKEY = new ANIKEY();

			file.Read( &pKEY->m_fTime, sizeof(FLOAT));
			file.Read( &pKEY->m_fPosX, sizeof(FLOAT));
			file.Read( &pKEY->m_fPosY, sizeof(FLOAT));
			file.Read( &pKEY->m_fPosZ, sizeof(FLOAT));
			file.Read( &pKEY->m_fRotX, sizeof(FLOAT));
			file.Read( &pKEY->m_fRotY, sizeof(FLOAT));
			file.Read( &pKEY->m_fRotZ, sizeof(FLOAT));
			file.Read( &pKEY->m_fScaleX, sizeof(FLOAT));
			file.Read( &pKEY->m_fScaleY, sizeof(FLOAT));
			file.Read( &pKEY->m_fScaleZ, sizeof(FLOAT));
			file.Read( &pKEY->m_dwEventID, sizeof(DWORD));

			pANI->m_vKey.push_back(pKEY);
		}

		file.Read( &pANI->m_fTimeScale, sizeof(FLOAT));
		file.Read( &pANI->m_dwLoopID, sizeof(DWORD));
		file.Read( &pANI->m_bLoop, sizeof(BYTE));

		DWORD dwSize;
		file.Read( &dwSize, sizeof(DWORD));

		if( dwSize > 0 )
		{
			CTachyonAnimation *pANISRC = new CTachyonAnimation();

			pANISRC->m_bFileID = BYTE(dwFileID);
			pANISRC->m_dwSIZE = dwSize;
			pANISRC->m_dwPOS = DWORD(file.GetPosition());

			pTRESDATA->insert( MAPRES::value_type( dwPOS, pANISRC));
			file.Seek( dwSize, CFile::current);
		}

		pTRES->insert( MAPRES::value_type( dwPOS, pANI));
		dwPOS = DWORD(file.GetPosition());
		nIndex++;

		RenderBACK( T3DRES_ANI, BYTE(nIndex * 100 / nTotal));
	}
}

void CTachyonRes::LoadMESH( DWORD dwBASE,
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

		RenderBACK( T3DRES_MESH, BYTE(nIndex * 100 / nTotal));
	}
}

void CTachyonRes::LoadOBJ( CString strFILE,
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
			LPSFXINST pSFX = new SFXINST();
			DWORD dwSFX;

			file.Read( &dwSFX, sizeof(DWORD));
			pSFX->m_dwSFX = dwSFX;
			file.Read( &pSFX->m_pSFX, sizeof(DWORD));
			file.Read( &pSFX->m_vPOS, sizeof(D3DXVECTOR3));
			file.Read( &pSFX->m_vROT, sizeof(D3DXVECTOR3));
			file.Read( &pSFX->m_nPivotID, sizeof(int));
			file.Read( &pSFX->m_bBias, sizeof(BYTE));
			file.Read( &pSFX->m_dwFuncID, sizeof(DWORD));
			file.Read( &pSFX->m_dwTick, sizeof(DWORD));
			file.Read( &pSFX->m_bDeleteOnStop, sizeof(BOOL));

#ifdef HEBA
			int nHEBACount = 0;
			file.Read( &nHEBACount, sizeof(int));
			for( int j=0 ; j < nHEBACount ; ++j )
			{
				DWORD dwAction = 0;
				file.Read( &dwAction, sizeof(DWORD));
				pSFX->m_mapHideByAction.insert( MAPDWORD::value_type(dwAction, dwAction) );
			}
#endif

			pOBJ->m_mapSFX.insert( MAPSFXINST::value_type( dwSFX, pSFX));
		}

		file.Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			LPSNDINST pSND = new SNDINST();
			DWORD dwSND;

			file.Read( &dwSND, sizeof(DWORD));
			file.Read( &pSND->m_dwMediaID, sizeof(DWORD));
			file.Read( &pSND->m_dwFuncID, sizeof(DWORD));
			file.Read( &pSND->m_dwTick, sizeof(DWORD));
			file.Read( &pSND->m_bDeleteOnStop, sizeof(BOOL));

			pOBJ->m_mapSND.insert( MAPSNDINST::value_type( dwSND, pSND));
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
			LPACTION pACT = new ACTION();
			DWORD dwACT = 0;

			int nANI = 0;
			int nSFX = 0;
			int nSND = 0;

			file.Read( &dwACT, sizeof(DWORD));
			file.Read( &nSFX, sizeof(int));
			for( int j=0; j<nSFX; j++)
			{
				LPSFXINST pSFX = new SFXINST();
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

				pACT->m_mapSFX.insert( MAPSFXINST::value_type( dwSFX, pSFX));
			}

			file.Read( &nSND, sizeof(int));
			for( j=0; j<nSND; j++)
			{
				LPSNDINST pSND = new SNDINST();
				DWORD dwSND;

				file.Read( &dwSND, sizeof(DWORD));
				file.Read( &pSND->m_dwMediaID, sizeof(DWORD));
				file.Read( &pSND->m_dwFuncID, sizeof(DWORD));
				file.Read( &pSND->m_dwTick, sizeof(DWORD));
				file.Read( &pSND->m_bDeleteOnStop, sizeof(BOOL));

				pACT->m_mapSND.insert( MAPSNDINST::value_type( dwSND, pSND));
			}

			file.Read( &nANI, sizeof(int));
			for( j=0; j<nANI; j++)
			{
				LPANI pANI = new ANI();
				DWORD dwANIID;

				file.Read( &dwANIID, sizeof(DWORD));
				file.Read( &pANI->m_pANI, sizeof(DWORD));

				file.Read( &nSFX, sizeof(int));
				for( int k=0; k<nSFX; k++)
				{
					LPSFXINST pSFX = new SFXINST();
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

					pANI->m_mapSFX.insert( MAPSFXINST::value_type( dwSFX, pSFX));
				}

				file.Read( &nSND, sizeof(int));
				for( k=0; k<nSND; k++)
				{
					LPSNDINST pSND = new SNDINST();
					DWORD dwSND;

					file.Read( &dwSND, sizeof(DWORD));
					file.Read( &pSND->m_dwMediaID, sizeof(DWORD));
					file.Read( &pSND->m_dwFuncID, sizeof(DWORD));
					file.Read( &pSND->m_dwTick, sizeof(DWORD));
					file.Read( &pSND->m_bDeleteOnStop, sizeof(BOOL));

					pANI->m_mapSND.insert( MAPSNDINST::value_type( dwSND, pSND));
				}

				pACT->m_mapANI.insert( MAPANI::value_type( dwANIID, pANI));
			}
			pOBJ->m_mapACTION.insert( MAPACTION::value_type( dwACT, pACT));
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
						LPOBJTEX pTEX = new OBJTEX();

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

						pMESH->m_vTEX.push_back(pTEX);
					}
				}
			}
		}
		pOBJ->m_pRES = this;

		pTRES->insert( MAPRES::value_type( dwPOS, pOBJ));
		dwPOS = DWORD(file.GetPosition());
		nIndex++;

		RenderBACK( T3DRES_OBJ, BYTE(nIndex * 100 / nTotal));
	}
}

void CTachyonRes::LoadSFX( CString strFILE,
						   LPMAPRES pTRES,
						   int& nIndex,
						   int nTotal)
{
	CFile file( _T(".\\Data\\") + strFILE, CFile::modeRead|CFile::typeBinary);

	DWORD dwLENGTH = DWORD(file.GetLength());
	DWORD dwPOS = DWORD(file.GetPosition());

	while( dwPOS < dwLENGTH )
	{
		CTachyonUncompressor ucpr;
		ucpr.FromFile(&file);

		LPSFX pSFX = new SFX();
		int nCount = 0;

		ucpr.Read( &nCount, sizeof(int));
		for( int i=0; i<nCount; i++)
		{
			LPCLRKEY pKEY = new CLRKEY();

			ucpr.Read( &pKEY->m_dwColor, sizeof(DWORD));
			ucpr.Read( &pKEY->m_dwTick, sizeof(DWORD));

			pSFX->m_vKey.push_back(pKEY);
		}

		ucpr.Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			LPSFX pCOMBINE = NULL;

			ucpr.Read( &pCOMBINE, sizeof(DWORD));
			pSFX->m_vSFX.push_back(pCOMBINE);
		}

		ucpr.Read( &pSFX->m_bSFXType, sizeof(BYTE));
		ucpr.Read( &pSFX->m_bSRCType, sizeof(BYTE));
		ucpr.Read( &pSFX->m_SRC.m_pTEX, sizeof(DWORD));

		ucpr.Read( &pSFX->m_pANISRC, sizeof(DWORD));
		ucpr.Read( &pSFX->m_dwLifeTime, sizeof(DWORD));

		ucpr.Read( &pSFX->m_fFollow, sizeof(FLOAT));
		ucpr.Read( &pSFX->m_fSizeX, sizeof(FLOAT));
		ucpr.Read( &pSFX->m_fSizeY, sizeof(FLOAT));

		ucpr.Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			LPCLKINST pCI = new CLKINST();
			DWORD dwCI = 0;

			ucpr.Read( &dwCI, sizeof(DWORD));
			ucpr.Read( &pCI->m_dwCurCL, sizeof(DWORD));
			ucpr.Read( &pCI->m_dwCurMESH, sizeof(DWORD));

			pSFX->m_mapCLK.insert( MAPCLKINST::value_type( dwCI, pCI));
		}

		ucpr.Read( &pSFX->m_dwACT, sizeof(DWORD));
		ucpr.Read( &pSFX->m_dwANI, sizeof(DWORD));

		ucpr.Read( &pSFX->m_dwOBJFuncID, sizeof(DWORD));
		ucpr.Read( &pSFX->m_dwACTFuncID, sizeof(DWORD));
		ucpr.Read( &pSFX->m_dwANIFuncID, sizeof(DWORD));

		ucpr.Read( &pSFX->m_pSprayANI, sizeof(DWORD));
		ucpr.Read( &pSFX->m_bSprayType, sizeof(BYTE));
		ucpr.Read( &pSFX->m_bZEnable, sizeof(BYTE));
		ucpr.Read( &pSFX->m_bZWrite, sizeof(BYTE));
		ucpr.Read( &pSFX->m_bStrict, sizeof(BYTE));
		ucpr.Read( &pSFX->m_bFollow, sizeof(BYTE));

		ucpr.Read( &pSFX->m_dwDESTBlend, sizeof(DWORD));
		ucpr.Read( &pSFX->m_dwSRCBlend, sizeof(DWORD));
		ucpr.Read( &pSFX->m_dwBlendOP, sizeof(DWORD));

		ucpr.Read( &pSFX->m_dwGTime, sizeof(DWORD));
		ucpr.Read( &pSFX->m_dwGCnt, sizeof(DWORD));

		ucpr.Read( &pSFX->m_vOutBound, sizeof(D3DXVECTOR3));
		ucpr.Read( &pSFX->m_vInBound, sizeof(D3DXVECTOR3));

		ucpr.Read( &pSFX->m_dwPLifeTime, sizeof(DWORD));
		ucpr.Read( &pSFX->m_dwERRTime, sizeof(DWORD));

		ucpr.Read( &pSFX->m_vERRScale, sizeof(D3DXVECTOR3));
		ucpr.Read( &pSFX->m_vERRPos, sizeof(D3DXVECTOR3));
		ucpr.Read( &pSFX->m_vERRRot, sizeof(D3DXVECTOR3));
		ucpr.Read( &pSFX->m_dwLodLevel, sizeof(DWORD));
		pSFX->m_pRES = this;

		pTRES->insert( MAPRES::value_type( dwPOS, pSFX));
		dwPOS = DWORD(file.GetPosition());
		nIndex++;

		RenderBACK( T3DRES_SFX, BYTE(nIndex * 100 / nTotal));
	}
}

void CTachyonRes::ComplateTEX()
{
	MAPRES::iterator it;

	for( it = m_mapTEX.begin(); it != m_mapTEX.end(); it++)
	{
		LPTEXTURESET pTEX = (LPTEXTURESET) (*it).second;

		if(pTEX)
		{
			int nCount = pTEX->GetTextureCount();

			for( int i=0; i<nCount; i++)
			{
				MAPRES::iterator finder = m_mapTEXSRC.find((DWORD) pTEX->GetTexturePtr(i));
				pTEX->SetTexturePtr( i, (CT3DTexture *) (*finder).second);
			}
		}
	}
}

void CTachyonRes::ComplateIMG()
{
	MAPRES::iterator it;

	for( it = m_mapIMG.begin(); it != m_mapIMG.end(); it++)
	{
		LPIMAGESET pIMG = (LPIMAGESET) (*it).second;

		if(pIMG)
		{
			int nCount = INT(pIMG->m_vImage.size());

			for( int i=0; i<nCount; i++)
			{
				MAPRES::iterator finder = m_mapIMGSRC.find((DWORD) pIMG->m_vImage[i]);
				pIMG->m_vImage[i] = (CD3DImage *) (*finder).second;
			}
		}
	}
}

void CTachyonRes::ComplateANI()
{
	MAPRES::iterator it;

	for( it = m_mapANI.begin(); it != m_mapANI.end(); it++)
	{
		LPANISET pANI = (LPANISET) (*it).second;

		if(pANI)
		{
			int nCount = INT(pANI->m_vAni.size());

			for( int i=0; i<nCount; i++)
			{
				MAPRES::iterator finder = m_mapANISRC.find((DWORD) pANI->m_vAni[i]->m_pAni);
				pANI->m_vAni[i]->m_pAni = (CTachyonAnimation *) (*finder).second;
			}
		}
	}
}

void CTachyonRes::ComplateOBJ()
{
	MAPRES::iterator it;

	for( it = m_mapOBJ.begin(); it != m_mapOBJ.end(); it++)
	{
		LPOBJECT pOBJ = (LPOBJECT) (*it).second;

		if(pOBJ)
		{
			MAPSFXINST::iterator itSFX;
			MAPACTION::iterator itACT;
			MAPCLKIND::iterator itCLK;

			for( itSFX = pOBJ->m_mapSFX.begin(); itSFX != pOBJ->m_mapSFX.end(); itSFX++)
			{
				MAPRES::iterator itRES = m_mapSFX.find((DWORD) (*itSFX).second->m_pSFX);
				(*itSFX).second->m_pSFX = (LPSFX) (*itRES).second;
			}

			for( itACT = pOBJ->m_mapACTION.begin(); itACT != pOBJ->m_mapACTION.end(); itACT++)
			{
				LPACTION pACT = (*itACT).second;
				MAPANI::iterator itANI;

				for( itSFX = pACT->m_mapSFX.begin(); itSFX != pACT->m_mapSFX.end(); itSFX++)
				{
					MAPRES::iterator itRES = m_mapSFX.find((DWORD) (*itSFX).second->m_pSFX);
					(*itSFX).second->m_pSFX = (LPSFX) (*itRES).second;
				}

				for( itANI = pACT->m_mapANI.begin(); itANI != pACT->m_mapANI.end(); itANI++)
				{
					MAPRES::iterator finder = m_mapANI.find((DWORD) (*itANI).second->m_pANI);
					(*itANI).second->m_pANI = (LPANISET) (*finder).second;

					for( itSFX = (*itANI).second->m_mapSFX.begin(); itSFX != (*itANI).second->m_mapSFX.end(); itSFX++)
					{
						MAPRES::iterator itRES = m_mapSFX.find((DWORD) (*itSFX).second->m_pSFX);
						(*itSFX).second->m_pSFX = (LPSFX) (*itRES).second;
					}
				}
			}

			for( itCLK = pOBJ->m_mapCLKIND.begin(); itCLK != pOBJ->m_mapCLKIND.end(); itCLK++)
			{
				LPCLKIND pCLK = (*itCLK).second;
				MAPCLOTH::iterator itCL;

				for( itCL = pCLK->m_mapCLOTH.begin(); itCL != pCLK->m_mapCLOTH.end(); itCL++)
				{
					LPCLOTH pCL = (*itCL).second;
					MAPMESH::iterator itMESH;

					for( itMESH = pCL->m_mapMESH.begin(); itMESH != pCL->m_mapMESH.end(); itMESH++)
					{
						LPMESH pMESH = (*itMESH).second;

						MAPRES::iterator finder = m_mapMESH.find((DWORD) pMESH->m_pMESH);
						int nCount = INT(pMESH->m_vTEX.size());

						pMESH->m_pMESH = (CTachyonMesh *) (*finder).second;
						pOBJ->m_bFileID = pMESH->m_pMESH->m_bFileID;

						for( int i=0; i<nCount; i++)
						{
							finder = m_mapTEX.find((DWORD) pMESH->m_vTEX[i]->m_pTEX[0]);
							if( finder != m_mapTEX.end() )
								pMESH->m_vTEX[i]->m_pTEX[0] = (LPTEXTURESET) (*finder).second;

							finder = m_mapTEX.find((DWORD) pMESH->m_vTEX[i]->m_pTEX[1]);
							if( finder != m_mapTEX.end() )
								pMESH->m_vTEX[i]->m_pTEX[1] = (LPTEXTURESET) (*finder).second;
						}
					}
				}
			}
		}
	}
}

void CTachyonRes::ComplateSFX()
{
	MAPRES::iterator it;

	for( it = m_mapSFX.begin(); it != m_mapSFX.end(); it++)
	{
		LPSFX pSFX = (LPSFX) (*it).second;
		int nCount = INT(pSFX->m_vSFX.size());

		for( int i=0; i<nCount; i++)
		{
			MAPRES::iterator finder = m_mapSFX.find((DWORD) pSFX->m_vSFX[i]);

			if( finder != m_mapSFX.end() )
				pSFX->m_vSFX[i] = (LPSFX) (*finder).second;
			else
				pSFX->m_vSFX[i] = NULL;
		}

		switch(pSFX->m_bSRCType)
		{
		case SFXSRC_TEX	:
			{
				MAPRES::iterator finder = m_mapTEX.find((DWORD) pSFX->m_SRC.m_pTEX);

				if( finder != m_mapTEX.end() )
					pSFX->m_SRC.m_pTEX = (LPTEXTURESET) (*finder).second;
				else
					pSFX->m_SRC.m_pTEX = NULL;
			}

			break;

		case SFXSRC_OBJ	:
			{
				MAPRES::iterator finder = m_mapOBJ.find((DWORD) pSFX->m_SRC.m_pTEX);

				if( finder != m_mapOBJ.end() )
				{
					pSFX->m_SRC.m_pOBJ = (LPOBJECT) (*finder).second;
					LockOBJ(pSFX->m_SRC.m_pOBJ);
				}
				else
					pSFX->m_SRC.m_pOBJ = NULL;
			}

			break;

		case SFXSRC_SFX	:
			{
				MAPRES::iterator finder = m_mapSFX.find((DWORD) pSFX->m_SRC.m_pTEX);

				if( finder != m_mapSFX.end() )
					pSFX->m_SRC.m_pSFX = (LPSFX) (*finder).second;
				else
					pSFX->m_SRC.m_pSFX = NULL;
			}

			break;
		}

		MAPRES::iterator finder = m_mapANI.find((DWORD) pSFX->m_pANISRC);
		if( finder != m_mapANI.end() )
			pSFX->m_pANISRC = (LPANISET) (*finder).second;
		else
			pSFX->m_pANISRC = NULL;

		finder = m_mapANI.find((DWORD) pSFX->m_pSprayANI);
		if( finder != m_mapANI.end() )
			pSFX->m_pSprayANI = (LPANISET) (*finder).second;
		else
			pSFX->m_pSprayANI = NULL;
	}
}

CString CTachyonRes::LoadString( CFile* pFile)
{
	CString strResult("");
	int nLength;

	pFile->Read( &nLength, sizeof(int));
	if( nLength > 0 )
	{
		pFile->Read( strResult.GetBuffer(nLength), nLength);
		strResult.ReleaseBuffer(nLength);

		return strResult.Left(nLength);
	}

	return strResult;
}

void CTachyonRes::LoadNullTEX()
{
	static const BYTE pNULLDATA[] = 
	{
		0x44, 0x44, 0x53, 0x20, 0x7C, 0x00, 0x00, 0x00, 0x07, 0x10, 0x02, 0x00, 0x10, 0x00, 0x00, 0x00,
		0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
		0x04, 0x00, 0x00, 0x00, 0x44, 0x58, 0x54, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x40, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x0F, 0x02, 0x50, 0x40, 0x00, 0x01,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x10, 0x02, 0x55, 0x55, 0x55, 0x54,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x10, 0x02, 0x55, 0x55, 0x55, 0x15,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x0F, 0x02, 0x05, 0x01, 0x00, 0x40,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x10, 0x02, 0x05, 0x15, 0x55, 0x55,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x0F, 0x02, 0x50, 0x40, 0x00, 0x01,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x0F, 0x02, 0x05, 0x01, 0x00, 0x40,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x10, 0x02, 0x50, 0x54, 0x55, 0x55,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x10, 0x02, 0x55, 0x55, 0x15, 0x05,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x0F, 0x02, 0x01, 0x00, 0x40, 0x50,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x0F, 0x02, 0x40, 0x00, 0x01, 0x05,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x10, 0x02, 0x55, 0x55, 0x54, 0x50,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x0F, 0x02, 0x01, 0x00, 0x40, 0x50,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x10, 0x02, 0x54, 0x55, 0x55, 0x55,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x10, 0x02, 0x15, 0x55, 0x55, 0x55,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x0F, 0x02, 0x40, 0x00, 0x01, 0x05,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x0F, 0x02, 0x5C, 0x72, 0xC9, 0x25,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x0F, 0x02, 0x35, 0x8D, 0x63, 0x58,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x0F, 0x02, 0x25, 0xC9, 0x72, 0x5C,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xB8, 0x0F, 0x02, 0x58, 0x63, 0x8D, 0x35,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x33, 0x89, 0xF0, 0x01, 0x14, 0xC3, 0xC3, 0x14,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x91, 0x51, 0x72, 0x51, 0x88, 0x22, 0x88, 0x08,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x91, 0x51, 0x72, 0x51, 0x88, 0x22, 0x88, 0x08
	};

	if(!m_pNULLTEX)
	{
		D3DXCreateTextureFromFileInMemoryEx(
			m_pDEVICE->m_pDevice,
			pNULLDATA,
			sizeof(pNULLDATA),
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT, 0,
			D3DFMT_DXT3,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0, NULL,
			NULL,
			&m_pNULLTEX);
	}
	else
		m_pNULLTEX->AddRef();
}
