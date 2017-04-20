#include "StdAfx.h"


CTachyonWater::CTachyonWater()
{
	ZeroMemory( &m_vMaterial, sizeof(D3DMATERIAL9));
	m_vMaterial.Specular.r = 1.0f;
	m_vMaterial.Specular.g = 1.0f;
	m_vMaterial.Specular.b = 1.0f;
	m_vMaterial.Specular.a = 1.0f;
	m_vMaterial.Diffuse.a = 1.0f;
	m_vMaterial.Power = 50.0f;

	m_pSPEC = NULL;
	m_pDATA = NULL;
	m_pGEO = NULL;
	m_pREF = NULL;
	m_pVB = NULL;

	m_dwVBCnt = 24000;
	m_dwPolyCnt = 0;

	m_wMainBound = 8;
	m_wSubBound = 8;

	m_wMaxBlock = 64;
	m_wCountX = 64;
	m_wCountZ = 64;
	m_wSTEP = 64;
	m_wREF = 64;

	m_fLENGTH = 512.0f;
	m_fSCALE = 0.0f;
	m_fUNIT = 0.0f;
	m_fREF = 0.0f;
	m_bLAYER = 0;
	m_bLEVEL = 2;

	D3DXMatrixIdentity(&m_vPosition);
	m_mapUSED.clear();
}

CTachyonWater::~CTachyonWater()
{
	Release();
}

void CTachyonWater::Release()
{
	if(m_pDATA)
	{
		delete[] m_pDATA;
		m_pDATA = NULL;
	}

	if(m_pGEO)
	{
		delete[] m_pGEO;
		m_pGEO = NULL;
	}

	if(m_pREF)
	{
		m_pREF->Release();
		m_pREF = NULL;
	}

	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
	m_pSPEC = NULL;

	m_dwVBCnt = 24000;
	m_dwPolyCnt = 0;

	m_wMainBound = 8;
	m_wSubBound = 8;

	m_wMaxBlock = 64;
	m_wCountX = 64;
	m_wCountZ = 64;
	m_wSTEP = 64;
	m_wREF = 64;

	m_fLENGTH = 512.0f;
	m_fSCALE = 0.0f;
	m_fUNIT = 0.0f;
	m_fREF = 0.0f;
	m_bLAYER = 0;
	m_bLEVEL = 2;

	D3DXMatrixIdentity(&m_vPosition);
	m_mapUSED.clear();
}

void CTachyonWater::Init( LPDIRECT3DDEVICE9 pDevice,
						  LPTEXTURESET pSPEC,
						  FLOAT fLENGTH,
						  FLOAT fSCALE,
						  FLOAT fREF,
						  DWORD dwVBCnt,
						  WORD wMaxBlock,
						  WORD wMainBound,
						  WORD wSubBound,
						  WORD wCountX,
						  WORD wCountZ,
						  WORD wSTEP,
						  WORD wREF,
						  BYTE bLEVEL)
{
	Release();

	m_wMainBound = wMainBound;
	m_wSubBound = wSubBound;

	m_dwVBCnt = dwVBCnt;
	m_fLENGTH = fLENGTH;
	m_fSCALE = fSCALE;
	m_fREF = fREF;

	m_wMaxBlock = wMaxBlock;
	m_wCountX = wCountX;
	m_wCountZ = wCountZ;
	m_wSTEP = wSTEP;
	m_wREF = wREF;

	m_bLEVEL = bLEVEL;
	m_pSPEC = pSPEC;

	pDevice->CreateVertexBuffer(
		6 * INT(m_wREF - 1) * INT(m_wREF - 1) * sizeof(D3DXVECTOR3),
		0, D3DFVF_XYZ,
		D3DPOOL_MANAGED,
		&m_pREF, NULL);

	pDevice->CreateVertexBuffer(
		m_dwVBCnt * sizeof(MESHVERTEX),
		0, T3DFVF_MESHVERTEX,
		D3DPOOL_MANAGED,
		&m_pVB, NULL);
	m_mapUSED.clear();

	LPDIRECT3DSURFACE9 pIMG = NULL;
	D3DSURFACE_DESC desc;
	UVKEY vKEY = m_pSPEC->GetUV();

	m_bLAYER = BYTE(m_pSPEC->m_vTexture.size());
	m_fUNIT = fLENGTH / vKEY.m_fKeySU;
	m_pDATA = new BYTE[INT(m_bLAYER) * INT(m_wSTEP) * INT(m_wSTEP)];
	m_pGEO = new FLOAT[INT(m_wREF) * INT(m_wREF)];
	m_pSPEC->m_vTexture[0]->GetLevelDesc( 0, &desc);

	pDevice->CreateOffscreenPlainSurface(
		desc.Width,
		desc.Height,
		D3DFMT_A8R8G8B8,
		D3DPOOL_SCRATCH,
		&pIMG, NULL);

	for( BYTE i=0; i<m_bLAYER; i++)
	{
		LPBYTE pDATA = m_pDATA + INT(i) * INT(m_wSTEP) * INT(m_wSTEP);

		LPDIRECT3DSURFACE9 pTEX = NULL;
		D3DLOCKED_RECT lr;

		m_pSPEC->m_vTexture[i]->GetSurfaceLevel( 0, &pTEX);
		D3DXLoadSurfaceFromSurface(
			pIMG, NULL, NULL,
			pTEX, NULL, NULL,
			D3DX_FILTER_NONE, 0);
		pTEX->Release();

		pIMG->LockRect( &lr, NULL, 0);
		for( WORD j=0; j<m_wSTEP; j++)
		{
			LPDWORD pCLR = (LPDWORD) (((LPBYTE) lr.pBits) + INT(j) * desc.Height / INT(m_wSTEP) * lr.Pitch);

			for( WORD k=0; k<m_wSTEP; k++)
				pDATA[j * m_wSTEP + k] = (BYTE) (pCLR[INT(k) * desc.Width / INT(m_wSTEP)] & 0x000000FF);
		}
		pIMG->UnlockRect();
	}

	pIMG->Release();
}

MESHVERTEX CTachyonWater::GetGEO( int nPosX, int nPosZ)
{
	MESHVERTEX vRESULT;

	vRESULT.m_fPosX = FLOAT(nPosX);
	vRESULT.m_fPosY = 0.0f;
	vRESULT.m_fPosZ = FLOAT(nPosZ);

	vRESULT.m_fNormalX = 0.0f;
	vRESULT.m_fNormalY = 1.0f;
	vRESULT.m_fNormalZ = 0.0f;

	vRESULT.m_fU = FLOAT(nPosX) / m_fLENGTH;
	vRESULT.m_fV = FLOAT(nPosZ) / m_fLENGTH;

	return vRESULT;
}

FLOAT CTachyonWater::GetHeight( FLOAT fPosX, FLOAT fPosZ)
{
	fPosX -= m_vPosition._41;
	fPosZ -= m_vPosition._43;

	while( fPosX < 0.0f )
		fPosX += m_fUNIT;

	while( fPosZ < 0.0f )
		fPosZ += m_fUNIT;

	fPosX = fPosX * FLOAT(m_wSTEP) / m_fUNIT;
	fPosZ = fPosZ * FLOAT(m_wSTEP) / m_fUNIT;

	int nLAYER = INT(m_pSPEC->m_dwCurTick) * INT(m_bLAYER) / INT(m_pSPEC->m_dwTotalTick);
	int nPosX = INT(fPosX);
	int nPosZ = INT(fPosZ);

	int nNextX = nPosX + 1;
	int nNextZ = nPosZ + 1;

	fPosX -= nPosX;
	fPosZ -= nPosZ;

	nNextX %= m_wSTEP;
	nNextZ %= m_wSTEP;
	nPosX %= m_wSTEP;
	nPosZ %= m_wSTEP;

	if( nLAYER >= m_bLAYER )
		nLAYER--;

	LPBYTE pDATA = m_pDATA + nLAYER * INT(m_wSTEP) * INT(m_wSTEP);
	FLOAT fResult = FLOAT(pDATA[nPosZ * m_wSTEP + nPosX]) * m_fSCALE / 0xFF;
	FLOAT fHeight[3] = {
		fResult + fPosX * (FLOAT(pDATA[nPosZ * m_wSTEP + nNextX]) * m_fSCALE / 0xFF - fResult),
		fResult + fPosZ * (FLOAT(pDATA[nNextZ * m_wSTEP + nPosX]) * m_fSCALE / 0xFF - fResult),
		fResult + sqrt((fPosX * fPosX + fPosZ * fPosZ) / 2.0f) * (FLOAT(pDATA[nNextZ * m_wSTEP + nNextX]) * m_fSCALE / 0xFF - fResult)};

	for( int i=0; i<3; i++)
		fResult += fHeight[i];

	return fResult / 4.0f;
}

FLOAT CTachyonWater::GetDistance( D3DXVECTOR3& vPos, CRect& rect)
{
	CPoint center = rect.CenterPoint();
	D3DXVECTOR3 vPoint = D3DXVECTOR3(
		((FLOAT) center.x) + m_vPosition._41,
		0.0f,
		((FLOAT) center.y) + m_vPosition._43) - vPos;

	return max( 0.0f, D3DXVec3Length(&vPoint) - sqrtf((FLOAT) (rect.Height() * rect.Height() + rect.Width() * rect.Width())));
}

BOOL CTachyonWater::RectInView( CD3DCamera *pCamera, CRect& rect)
{
	D3DXVECTOR3 vCamPOS = pCamera->m_vPosition - pCamera->m_vAxisZ / D3DXVec3Length(&pCamera->m_vAxisZ);
	D3DXVECTOR3 vNormal[5] = {
		pCamera->m_vAxisX,
		-pCamera->m_vAxisX,
		pCamera->m_vAxisY,
		-pCamera->m_vAxisY,
		pCamera->m_vAxisZ};
	D3DXPLANE vPlane[5];

	D3DXVECTOR3 vRECT[4] = {
		D3DXVECTOR3(
		((FLOAT) rect.left) + m_vPosition._41,
		m_vPosition._42,
		((FLOAT) rect.top) + m_vPosition._43),
		D3DXVECTOR3(
		((FLOAT) rect.right) + m_vPosition._41,
		m_vPosition._42,
		((FLOAT) rect.top) + m_vPosition._43),
		D3DXVECTOR3(
		((FLOAT) rect.right) + m_vPosition._41,
		m_vPosition._42,
		((FLOAT) rect.bottom) + m_vPosition._43),
		D3DXVECTOR3(
		((FLOAT) rect.left) + m_vPosition._41,
		m_vPosition._42,
		((FLOAT) rect.bottom) + m_vPosition._43)};

	if(pCamera->IsOrthoCamera())
	{
		FLOAT fLEN[4] = {
			pCamera->m_fWidth / 2.0f,
			pCamera->m_fWidth / 2.0f,
			pCamera->m_fHeight / 2.0f,
			pCamera->m_fHeight / 2.0f};

		for( int i=0; i<4; i++)
		{
			D3DXVECTOR3 vPOS = vCamPOS;

			vPOS -= fLEN[i] * vNormal[i] / D3DXVec3Length(&vNormal[i]);
			D3DXPlaneFromPointNormal(
				&vPlane[i],
				&vPOS,
				&vNormal[i]);
		}
	}
	else
	{
		D3DXVECTOR3 vAxis[4] = {
			pCamera->m_vAxisY,
			pCamera->m_vAxisY,
			pCamera->m_vAxisX,
			pCamera->m_vAxisX};

		FLOAT fROT[4] = {
			-pCamera->m_fFOV * pCamera->m_fAspect / 2.0f,
			pCamera->m_fFOV * pCamera->m_fAspect / 2.0f,
			pCamera->m_fFOV / 2.0f,
			-pCamera->m_fFOV / 2.0f};

		for( int i=0; i<4; i++)
		{
			D3DXMATRIX vROT;

			D3DXMatrixRotationAxis(
				&vROT,
				&vAxis[i],
				fROT[i]);

			CTMath::Transform(
				&vROT,
				&vNormal[i]);

			D3DXPlaneFromPointNormal(
				&vPlane[i],
				&vCamPOS,
				&vNormal[i]);
		}
	}

	D3DXPlaneFromPointNormal(
		&vPlane[4],
		&vCamPOS,
		&vNormal[4]);

	for( int i=0; i<5; i++)
	{
		BYTE bBACK = TRUE;

		for( int j=0; j<4 && bBACK; j++)
			if( D3DXPlaneDotCoord( &vPlane[i], &vRECT[j]) > 0.0f )
				bBACK = FALSE;

		if(bBACK)
			return FALSE;
	}

	return TRUE;
}

void CTachyonWater::LODMatric( VECTORBLOCK& vBlock,
							   CD3DCamera *pCamera,
							   CRect rect)
{
	if(RectInView( pCamera, rect))
	{
		FLOAT fDIST = GetDistance( pCamera->m_vPosition, rect);
		int nLevel = FLOAT(m_wMainBound) > fDIST ? INT(m_bLEVEL) : min( 10, INT((fDIST - FLOAT(m_wMainBound)) / m_wSubBound) + INT(m_bLEVEL) + 1);
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

				LODMatric( vBlock, pCamera, rc);
			}
		}
		else
		{
			m_mapUSED.insert( MAPDWORD::value_type( rect.bottom * (INT(m_wMaxBlock) * INT(m_wCountX) + 1) + rect.right, 0));
			m_mapUSED.insert( MAPDWORD::value_type( rect.bottom * (INT(m_wMaxBlock) * INT(m_wCountX) + 1) + rect.left, 0));
			m_mapUSED.insert( MAPDWORD::value_type( rect.top * (INT(m_wMaxBlock) * INT(m_wCountX) + 1) + rect.right, 0));
			m_mapUSED.insert( MAPDWORD::value_type( rect.top * (INT(m_wMaxBlock) * INT(m_wCountX) + 1) + rect.left, 0));

			vBlock.push_back(new CRect(rect));
		}
	}
}

void CTachyonWater::BuildBlock( VECTORBLOCK& vBlock, CD3DCamera *pCamera)
{
	CRect rect(
		0, 0,
		m_wMaxBlock,
		m_wMaxBlock);

	m_mapUSED.clear();
	vBlock.clear();

	for( WORD i=0; i<m_wCountZ; i++)
	{
		rect.right = m_wMaxBlock;
		rect.left = 0;

		for( WORD j=0; j<m_wCountX; j++)
		{
			LODMatric( vBlock, pCamera, rect);
			rect.OffsetRect( m_wMaxBlock, 0);
		}

		rect.OffsetRect( 0, m_wMaxBlock);
	}
}

void CTachyonWater::ResetVB( LPDIRECT3DDEVICE9 pDevice, VECTORBLOCK& vBlock)
{
	static int nActiveIndex[4] = { 1, 5, 7, 3};

	LPMESHVERTEX pBUF = NULL;
	int nBound = 1;

	m_pVB->Lock( 0, m_dwVBCnt * sizeof(MESHVERTEX), (LPVOID *) &pBUF, 0);
	m_dwPolyCnt = 0;

	while(!vBlock.empty())
	{
		VECTORBLOCK::iterator it = vBlock.begin();

		while(it != vBlock.end())
		{
			CRect *pRECT = (*it);

			if( pRECT->Width() <= nBound )
			{
				CPoint center = pRECT->CenterPoint();

				int nJoint[4] = {
					center.y * (INT(m_wMaxBlock) * INT(m_wCountX) + 1) + pRECT->left,
					center.y * (INT(m_wMaxBlock) * INT(m_wCountX) + 1) + pRECT->right,
					pRECT->top * (INT(m_wMaxBlock) * INT(m_wCountX) + 1) + center.x,
					pRECT->bottom * (INT(m_wMaxBlock) * INT(m_wCountX) + 1) + center.x};

				BOOL bJoint = FALSE;
				BOOL bActive[9] = {
					TRUE, FALSE,
					TRUE, FALSE,
					TRUE, FALSE,
					TRUE, FALSE,
					TRUE};

				if( pRECT->Width() > 1 )
					for( int i=0; i<4; i++)
						if( m_mapUSED.find(nJoint[i]) != m_mapUSED.end() )
						{
							bActive[nActiveIndex[i]] = TRUE;
							bJoint = TRUE;
						}

				if(bJoint)
				{
					MESHVERTEX vDATA[9] = {
						GetGEO( pRECT->left, pRECT->top),
						GetGEO( pRECT->left, center.y),
						GetGEO( pRECT->left, pRECT->bottom),
						GetGEO( center.x, pRECT->bottom),
						GetGEO( pRECT->right, pRECT->bottom),
						GetGEO( pRECT->right, center.y),
						GetGEO( pRECT->right, pRECT->top),
						GetGEO( center.x, pRECT->top),
						GetGEO( pRECT->left, pRECT->top)};

					MESHVERTEX vCENTER = GetGEO( center.x, center.y);
					int nPrev = -1;

					for( int i=0; i<9; i++)
						if(bActive[i])
						{
							if( nPrev != -1 )
							{
								memcpy( &pBUF[m_dwPolyCnt * 3], &vCENTER, sizeof(MESHVERTEX));
								memcpy( &pBUF[m_dwPolyCnt * 3 + 1], &vDATA[nPrev], sizeof(MESHVERTEX));
								memcpy( &pBUF[m_dwPolyCnt * 3 + 2], &vDATA[i], sizeof(MESHVERTEX));

								m_dwPolyCnt++;
							}

							nPrev = i;
						}
				}
				else
				{
					MESHVERTEX vDATA[6] = {
						GetGEO( pRECT->left, pRECT->top),
						GetGEO( pRECT->left, pRECT->bottom),
						GetGEO( pRECT->right, pRECT->top),
						GetGEO( pRECT->right, pRECT->top),
						GetGEO( pRECT->left, pRECT->bottom),
						GetGEO( pRECT->right, pRECT->bottom)};

					for( int i=0; i<6; i++)
						memcpy( &pBUF[m_dwPolyCnt * 3 + i], &vDATA[i], sizeof(MESHVERTEX));
					m_dwPolyCnt += 2;
				}

				vBlock.erase(it);
				delete pRECT;
			}
			else
				it++;
		}

		nBound *= 2;
	}

	m_pVB->Unlock();
}

void CTachyonWater::ResetREF( LPDIRECT3DDEVICE9 pDevice,
							  D3DXVECTOR3 vPOS)
{
	LPD3DXVECTOR3 pBUF = NULL;

	vPOS -= D3DXVECTOR3(
		m_fREF * FLOAT(m_wREF) / 2.0f,
		0.0f,
		m_fREF * FLOAT(m_wREF) / 2.0f);

	for( WORD i=0; i<m_wREF; i++)
		for( WORD j=0; j<m_wREF; j++)
			m_pGEO[INT(i) * INT(m_wREF) + INT(j)] = 2.0f * GetHeight( FLOAT(j) * m_fREF, FLOAT(i) * m_fREF);

	vPOS -= D3DXVECTOR3(
		m_vPosition._41,
		0.0f,
		m_vPosition._43);

	m_pREF->Lock( 0, 6 * INT(m_wREF - 1) * INT(m_wREF - 1) * sizeof(D3DXVECTOR3), (LPVOID *) &pBUF, 0);
	for( i=0; i<m_wREF - 1; i++)
		for( WORD j=0; j<m_wREF - 1; j++)
		{
			D3DXVECTOR3 vPOLY[6] = {
				vPOS + D3DXVECTOR3( FLOAT(j) * m_fREF, m_pGEO[INT(i) * INT(m_wREF) + INT(j)], FLOAT(i) * m_fREF),
				vPOS + D3DXVECTOR3( FLOAT(j) * m_fREF, m_pGEO[INT(i + 1) * INT(m_wREF) + INT(j)], FLOAT(i + 1) * m_fREF),
				vPOS + D3DXVECTOR3( FLOAT(j + 1) * m_fREF, m_pGEO[INT(i) * INT(m_wREF) + INT(j + 1)], FLOAT(i) * m_fREF),
				vPOS + D3DXVECTOR3( FLOAT(j + 1) * m_fREF, m_pGEO[INT(i) * INT(m_wREF) + INT(j + 1)], FLOAT(i) * m_fREF),
				vPOS + D3DXVECTOR3( FLOAT(j) * m_fREF, m_pGEO[INT(i + 1) * INT(m_wREF) + INT(j)], FLOAT(i + 1) * m_fREF),
				vPOS + D3DXVECTOR3( FLOAT(j + 1) * m_fREF, m_pGEO[INT(i + 1) * INT(m_wREF) + INT(j + 1)], FLOAT(i + 1) * m_fREF)};

			memcpy( &pBUF[6 * (INT(i) * INT(m_wREF - 1) + INT(j))], vPOLY, 6 * sizeof(D3DXVECTOR3));
		}

	m_pREF->Unlock();
}

void CTachyonWater::Render( LPDIRECT3DDEVICE9 pDevice)
{
	D3DMATERIAL9 vMaterial;

	pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	pDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE);
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000000);
	pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_DEPTHBIAS, 5);

	pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(MESHVERTEX));
	pDevice->SetFVF(T3DFVF_MESHVERTEX);
	pDevice->SetTransform( D3DTS_WORLD, &m_vPosition);

	pDevice->GetMaterial(&vMaterial);
	pDevice->SetMaterial(&m_vMaterial);
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLELIST,
		0, m_dwPolyCnt);

	pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState( D3DRS_DEPTHBIAS, 0);
	pDevice->SetMaterial(&vMaterial);
}

void CTachyonWater::RenderREF( LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000000);
	pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE);
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState( D3DRS_DEPTHBIAS, 7);

	pDevice->SetStreamSource( 0, m_pREF, 0, sizeof(D3DXVECTOR3));
	pDevice->SetFVF(D3DFVF_XYZ);
	pDevice->SetTransform( D3DTS_WORLD, &m_vPosition);

	pDevice->DrawPrimitive(
		D3DPT_TRIANGLELIST,
		0, 2 * INT(m_wREF - 1) * INT(m_wREF - 1));

	pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_DEPTHBIAS, 0);
}
