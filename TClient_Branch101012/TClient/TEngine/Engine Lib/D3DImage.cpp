// D3DImage.cpp: implementation of the CD3DImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

DWORD CD3DImage::m_dwScreenX = 1024;
DWORD CD3DImage::m_dwScreenY = 768;
BOOL CD3DImage::m_bUseGlobalClipRect = FALSE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD3DImage::CD3DImage()
{
	m_bUseIMGBUF = FALSE;
	m_nPartCount = 0;
	m_nHeight = 1;
	m_nWidth = 1;
	m_nPosX = 0;
	m_nPosY = 0;

	m_pT3DTEX = NULL;
	m_pT3DVB = NULL;
	m_pMask = NULL;

	m_ppPosBufX = NULL;
	m_ppPosBufY = NULL;

	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;
	m_fPrvScaleX = 1.0f;
	m_fPrvScaleY = 1.0f;
}

CD3DImage::~CD3DImage()
{
	Release();
}

BOOL CD3DImage::GetMask( int nPosX, int nPosY)
{
	CRect rect(
		0, 0,
		m_nWidth,
		m_nHeight);

	if(!rect.PtInRect(CPoint( nPosX, nPosY)))
		return FALSE;

	DWORD dwPitch = m_nWidth % 8 ? m_nWidth / 8 + 1 : m_nWidth / 8;
	DWORD dwIndex = dwPitch * nPosY + nPosX / 8;

	return m_pMask[dwIndex] & (0x80 >> (nPosX % 8));
}

void CD3DImage::Load( LPMAPT3DTEXTURE pIMGBUF,
					  LPBYTE pResData)
{
	Release();
	m_bUseIMGBUF = TRUE;
	m_nPosX = 0;
	m_nPosY = 0;

	memcpy( &m_nPartCount, pResData, sizeof(int));
	pResData += sizeof(int);

	memcpy( &m_nWidth, pResData, sizeof(int));
	pResData += sizeof(int);

	memcpy( &m_nHeight, pResData, sizeof(int));
	pResData += sizeof(int);

	m_pT3DTEX = new CT3DTexture*[m_nPartCount];
	m_pT3DVB = new CT3DVertex*[m_nPartCount];

	m_ppPosBufX = new float*[m_nPartCount];
	m_ppPosBufY = new float*[m_nPartCount];

	for( int i=0; i<m_nPartCount; i++)
	{
		DWORD dwImageID;

		memcpy( &dwImageID, pResData, sizeof(DWORD));
		pResData += sizeof(DWORD);

		MAPT3DTEXTURE::iterator finder = pIMGBUF->find(dwImageID);
		if( finder != pIMGBUF->end() )
			m_pT3DTEX[i] = (*finder).second;
		else
			m_pT3DTEX[i] = NULL;

		m_pT3DVB[i] = new CT3DVertex();
		m_pT3DVB[i]->LoadT3DVB(
			4 * sizeof(TVERTEX),
			0, T3DFVF_TVERTEX);

		m_pT3DVB[i]->LoadT3DVBDATA(
			pResData,
			4 * sizeof(TVERTEX), 0);

		m_ppPosBufX[i] = new float[4];
		m_ppPosBufY[i] = new float[4];

		for( int j=0; j<4; ++j )
		{
			m_ppPosBufX[i][j] = ((TVERTEX*)pResData)[j].m_fPosX;
			m_ppPosBufY[i][j] = ((TVERTEX*)pResData)[j].m_fPosY;
		}

		pResData += 4 * sizeof(TVERTEX);
	}

	int nMask;
	memcpy( &nMask, pResData, sizeof(int));
	pResData += sizeof(int);

	m_pMask = new BYTE[nMask];
	memcpy( m_pMask, pResData, nMask);
}

void CD3DImage::Load( LPBYTE pResData,
					  BYTE bFormat)
{
	Release();
	m_nPosX = 0;
	m_nPosY = 0;

	memcpy( &m_nPartCount, pResData, sizeof(int));
	pResData += sizeof(int);

	memcpy( &m_nWidth, pResData, sizeof(int));
	pResData += sizeof(int);

	memcpy( &m_nHeight, pResData, sizeof(int));
	pResData += sizeof(int);

	m_pT3DTEX = new CT3DTexture*[m_nPartCount];
	m_pT3DVB = new CT3DVertex*[m_nPartCount];

	m_ppPosBufX = new float*[m_nPartCount];
	m_ppPosBufY = new float*[m_nPartCount];

	for( int i=0; i<m_nPartCount; i++)
	{
		DWORD dwSize;

		memcpy( &dwSize, pResData, sizeof(DWORD));
		pResData += sizeof(DWORD);

		m_pT3DTEX[i] = new CT3DTexture();
		m_pT3DTEX[i]->LoadT3DTEX(
			pResData,
			dwSize, 0,
			bFormat);
		pResData += dwSize;

		m_pT3DVB[i] = new CT3DVertex();
		m_pT3DVB[i]->LoadT3DVB(
			4 * sizeof(TVERTEX),
			0, T3DFVF_TVERTEX);

		m_pT3DVB[i]->LoadT3DVBDATA(
			pResData,
			4 * sizeof(TVERTEX), 0);

		m_ppPosBufX[i] = new float[4];
		m_ppPosBufY[i] = new float[4];

		for( int j=0; j<4; j++)
		{
			m_ppPosBufX[i][j] = ((LPTVERTEX) pResData)[j].m_fPosX;
			m_ppPosBufY[i][j] = ((LPTVERTEX) pResData)[j].m_fPosY;
		}

		pResData += 4 * sizeof(TVERTEX);
	}

	int nMask;
	memcpy( &nMask, pResData, sizeof(int));
	pResData += sizeof(int);

	m_pMask = new BYTE[nMask];
	memcpy( m_pMask, pResData, nMask);
}

void CD3DImage::SetScaleX( float fScale)
{
	m_fScaleX = fScale;
}

void CD3DImage::SetScaleY( float fScale)
{
	m_fScaleY = fScale;
}

LPT3DDATA CD3DImage::BuildImageData( LPDIRECT3DDEVICE9 pDevice, BYTE bFormat)
{
	LPTVERTEX *pVert = new LPTVERTEX[m_nPartCount];
	LPT3DDATA *pDXT = new LPT3DDATA[m_nPartCount];
	LPT3DDATA pResult = new T3DDATA();

	pResult->m_dwSize = 4 * sizeof(int) + m_nPartCount * sizeof(DWORD);

	for( int i=0; i<m_nPartCount; i++)
	{
		LPDIRECT3DVERTEXBUFFER9 pVB = m_pT3DVB[i]->GetVB();
		LPTVERTEX pBuf = NULL;

		pDXT[i] = CreateDXT( pDevice, m_pT3DTEX[i] ? m_pT3DTEX[i]->GetTEX() : NULL, bFormat);
		pVert[i] = new TVERTEX[4];

		if(pVB)
		{
			pVB->Lock( 0, 4 * sizeof(TVERTEX), (LPVOID *) &pBuf, 0);
			memcpy( pVert[i], pBuf, 4 * sizeof(TVERTEX));
			pVB->Unlock();

			for( int j=0; j<4; j++)
			{
				m_ppPosBufX[i][j] -= (FLOAT) m_nPosX;
				m_ppPosBufY[i][j] -= (FLOAT) m_nPosY;

				pVert[i][j].m_fPosX = m_ppPosBufX[i][j];
				pVert[i][j].m_fPosY = m_ppPosBufY[i][j];
			}
		}

		pResult->m_dwSize += 4 * sizeof(TVERTEX);
		pResult->m_dwSize += pDXT[i]->m_dwSize;
	}

	int nPitch = m_nWidth % 8 ? m_nWidth / 8 + 1 : m_nWidth / 8;
	int nMask = nPitch * m_nHeight;

	pResult->m_dwSize += nMask;
	pResult->m_pData = new BYTE[pResult->m_dwSize];

	LPBYTE pData = pResult->m_pData;
	memcpy( pData, &m_nPartCount, sizeof(int));
	pData += sizeof(int);

	memcpy( pData, &m_nWidth, sizeof(int));
	pData += sizeof(int);

	memcpy( pData, &m_nHeight, sizeof(int));
	pData += sizeof(int);

	for( i=0; i<m_nPartCount; i++)
	{
		memcpy( pData, &pDXT[i]->m_dwSize, sizeof(DWORD));
		pData += sizeof(DWORD);

		memcpy( pData, pDXT[i]->m_pData, pDXT[i]->m_dwSize);
		pData += pDXT[i]->m_dwSize;

		memcpy( pData, pVert[i], 4 * sizeof(TVERTEX));
		pData += 4 * sizeof(TVERTEX);

		delete[] pVert[i];
		delete pDXT[i];
	}

	memcpy( pData, &nMask, sizeof(int));
	pData += sizeof(int);
	memcpy( pData, m_pMask, nMask);

	delete[] pVert;
	delete[] pDXT;

	return pResult;
}

BYTE CD3DImage::LoadImageFile( LPDIRECT3DDEVICE9 pDevice,
							   LPBYTE pResData,
							   UINT nLength)
{
	LPDIRECT3DSURFACE9 pImage = NULL;
	D3DXIMAGE_INFO info;
	D3DLOCKED_RECT lr;

	HRESULT hr = D3DXGetImageInfoFromFileInMemory(
		pResData,
		nLength,
		&info);

	if(FAILED(hr))
		return FALSE;

	UINT nCountX = 0;
	UINT nCountY = 0;
	UINT nHeight = 0;
	UINT nWidth = 0;

	while( nHeight < info.Height )
	{
		nHeight += CTMath::GetNearPow(info.Height - nHeight);
		nCountY++;
	}

	while( nWidth < info.Width )
	{
		nWidth += CTMath::GetNearPow(info.Width - nWidth);
		nCountX++;
	}

	hr = pDevice->CreateOffscreenPlainSurface(
		nWidth,
		nHeight,
		D3DFMT_A8R8G8B8,
		D3DPOOL_SCRATCH,
		&pImage,
		NULL);

	if(FAILED(hr))
		return FALSE;

	if(FAILED(pImage->LockRect( &lr, NULL, 0)))
	{
		pImage->Release();
		return FALSE;
	}

	memset( lr.pBits, 0, lr.Pitch * nHeight);
	pImage->UnlockRect();

	hr = D3DXLoadSurfaceFromFileInMemory(
		pImage,
		NULL,
		&CRect( 0, 0, info.Width, info.Height),
		pResData,
		nLength,
		NULL,
		D3DX_FILTER_NONE,
		0, NULL);

	if(FAILED(hr))
	{
		pImage->Release();
		return FALSE;
	}
	Release();

	if(FAILED(pImage->LockRect( &lr, NULL, D3DLOCK_NO_DIRTY_UPDATE)))
	{
		pImage->Release();
		return FALSE;
	}

	DWORD dwPitch = info.Width % 8 ? info.Width / 8 + 1 : info.Width / 8;
	DWORD dwMask = dwPitch * info.Height;

	LPDWORD pImageData = (LPDWORD) lr.pBits;
	LPBYTE pBuffer = (LPBYTE) lr.pBits;

	m_pMask = new BYTE[dwMask];
	memset( m_pMask, 0, dwMask);

	for( int i=0; i<info.Height; i++)
	{
		for( int j=0; j<info.Width; j++)
		{
			if((*pImageData)&0xFF000000)
			{
				LPBYTE pMask = m_pMask + dwPitch * i + j / 8;
				(*pMask) |= 0x80 >> (j % 8);
			}

			pImageData++;
		}

		pBuffer += lr.Pitch;
		pImageData = (LPDWORD) pBuffer;
	}
	pImage->UnlockRect();

	m_nPartCount = nCountX * nCountY;
	m_nHeight = info.Height;
	m_nWidth = info.Width;
	m_nPosX = 0;
	m_nPosY = 0;

	m_pT3DTEX = new CT3DTexture*[m_nPartCount];
	m_pT3DVB = new CT3DVertex*[m_nPartCount];

	m_ppPosBufX = new float*[m_nPartCount];
	m_ppPosBufY = new float*[m_nPartCount];

	ZeroMemory( m_pT3DTEX, m_nPartCount * sizeof(CT3DTexture *));
	ZeroMemory( m_pT3DVB, m_nPartCount * sizeof(CT3DVertex *));

	ZeroMemory( m_ppPosBufX, m_nPartCount * sizeof(float *));
	ZeroMemory( m_ppPosBufY, m_nPartCount * sizeof(float *));

	int nIndex = 0;
	nHeight = 0;
	nWidth = 0;

	while( nHeight < info.Height )
	{
		int nLocalH = CTMath::GetNearPow(info.Height - nHeight);

		while( nWidth < info.Width )
		{
			int nLocalW = CTMath::GetNearPow(info.Width - nWidth);
			int vPoint[4][4] = {
				{ nWidth, nHeight, 0, 0},
				{ nWidth + nLocalW, nHeight, 1, 0},
				{ nWidth, nHeight + nLocalH, 0, 1},
				{ nWidth + nLocalW, nHeight + nLocalH, 1, 1}};

			LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
			LPTVERTEX pData = NULL;

			hr = pDevice->CreateVertexBuffer(
				4 * sizeof(TVERTEX),
				0, T3DFVF_TVERTEX,
				D3DPOOL_MANAGED,
				&pVB, NULL);

			if( FAILED(hr) || !pVB )
			{
				pImage->Release();
				Release();

				return FALSE;
			}

			m_ppPosBufX[nIndex] = new float[4];
			m_ppPosBufY[nIndex] = new float[4];

			if(FAILED(pVB->Lock( 0, 4 * sizeof(TVERTEX), (LPVOID *) &pData, 0)))
			{
				pImage->Release();
				pVB->Release();
				Release();

				return FALSE;
			}

			for( i=0; i<4; i++)
			{
				pData[i].m_fPosX = ((FLOAT) vPoint[i][0]) + 0.5f;
				pData[i].m_fPosY = ((FLOAT) vPoint[i][1]) + 0.5f;
				pData[i].m_fPosZ = 0.5f;
				pData[i].m_fRHW = 1.0f;

				pData[i].m_fU = (FLOAT) vPoint[i][2];
				pData[i].m_fV = (FLOAT) vPoint[i][3];

				m_ppPosBufX[nIndex][i] = pData[i].m_fPosX;
				m_ppPosBufY[nIndex][i] = pData[i].m_fPosY;
			}
			pVB->Unlock();

			m_pT3DVB[nIndex] = new CT3DVertex();
			m_pT3DVB[nIndex]->m_pTDATA = (LPBYTE) pVB;
			m_pT3DVB[nIndex]->m_bEnabled = TRUE;

			LPDIRECT3DSURFACE9 pLevel = NULL;
			LPDIRECT3DTEXTURE9 pTEX = NULL;

			hr = D3DXCreateTexture(
				pDevice,
				nLocalW,
				nLocalH,
				1, 0,
				D3DFMT_A8R8G8B8,
				D3DPOOL_MANAGED,
				&pTEX);

			if( FAILED(hr) || !pTEX )
			{
				pImage->Release();
				Release();

				return FALSE;
			}

			hr = pTEX->GetSurfaceLevel( 0, &pLevel);
			if( FAILED(hr) || !pLevel )
			{
				pImage->Release();
				pTEX->Release();
				Release();

				return FALSE;
			}

			hr = D3DXLoadSurfaceFromSurface(
				pLevel,
				NULL,
				NULL,
				pImage,
				NULL,
				&CRect(
				vPoint[0][0],
				vPoint[0][1],
				vPoint[3][0],
				vPoint[3][1]),
				D3DX_FILTER_NONE,
				NULL);

			if(FAILED(hr))
			{
				pImage->Release();
				pLevel->Release();
				pTEX->Release();
				Release();

				return FALSE;
			}
			pLevel->Release();

			m_pT3DTEX[nIndex] = new CT3DTexture();
			m_pT3DTEX[nIndex]->m_pTDATA = (LPBYTE) pTEX;
			m_pT3DTEX[nIndex]->m_bEnabled = TRUE;

			nWidth += nLocalW;
			nIndex++;
		}

		nHeight += nLocalH;
		nWidth = 0;
	}
	pImage->Release();

	return TRUE;
}

LPDIRECT3DSURFACE9 CD3DImage::CreateOffscreenPlainSurface( LPDIRECT3DDEVICE9 pDevice)
{
	LPDIRECT3DSURFACE9 pRESULT = NULL;

	if(!m_nPartCount)
		return pRESULT;

	pDevice->CreateOffscreenPlainSurface(
		m_nWidth,
		m_nHeight,
		D3DFMT_A8R8G8B8,
		D3DPOOL_SCRATCH,
		&pRESULT,
		NULL);

	for( int i=0; i<m_nPartCount; i++)
	{
		LPDIRECT3DVERTEXBUFFER9 pVB = m_pT3DVB[i]->GetVB();
		LPDIRECT3DTEXTURE9 pTEX = m_pT3DTEX[i]->GetTEX();

		if( pTEX && pVB )
		{
			LPDIRECT3DSURFACE9 pSRC = NULL;
			LPTVERTEX pData = NULL;	
			CRect rcDEST;

			pVB->Lock( 0, 4 * sizeof(TVERTEX), (LPVOID *) &pData, 0);
			rcDEST.left = LONG(pData[0].m_fPosX - 0.5f);
			rcDEST.top = LONG(pData[0].m_fPosY - 0.5f);
			rcDEST.right = LONG(pData[3].m_fPosX - 0.5f);
			rcDEST.bottom = LONG(pData[3].m_fPosY - 0.5f);
			rcDEST.right = min( rcDEST.right, m_nWidth);
			rcDEST.bottom = min( rcDEST.bottom, m_nHeight);
			pVB->Unlock();

			pTEX->GetSurfaceLevel( 0, &pSRC);
			D3DXLoadSurfaceFromSurface(
				pRESULT,
				NULL,
				&rcDEST,
				pSRC,
				NULL,
				NULL,
				D3DX_FILTER_NONE,
				NULL);

			pSRC->Release();
		}
	}

	return pRESULT;
}

void CD3DImage::SaveImageFile( LPDIRECT3DDEVICE9 pDevice, LPCSTR strFileName, D3DXIMAGE_FILEFORMAT destFormat)
{
	LPDIRECT3DSURFACE9 pIMG = CreateOffscreenPlainSurface(pDevice);

	if(pIMG)
	{
		D3DXSaveSurfaceToFile(
			strFileName, 
			destFormat,
			pIMG,
			NULL,
			NULL);

		pIMG->Release();
	}
}

void CD3DImage::Release()
{
	if(m_pT3DTEX)
	{
		if(!m_bUseIMGBUF)
			for( int i=0; i<m_nPartCount; i++)
				SAFE_DELETE(m_pT3DTEX[i]);

		delete[] m_pT3DTEX;
		m_pT3DTEX = NULL;
	}

	if(m_pT3DVB)
	{
		for( int i=0; i<m_nPartCount; i++)
			SAFE_DELETE(m_pT3DVB[i]);

		delete[] m_pT3DVB;
		m_pT3DVB = NULL;
	}

	if(m_pMask)
	{
		delete[] m_pMask;
		m_pMask = NULL;
	}

	if(m_ppPosBufX)
	{
		for( int i=0; i<m_nPartCount; i++)
			if(m_ppPosBufX[i])
				delete[] m_ppPosBufX[i];

		delete[] m_ppPosBufX;
		m_ppPosBufX = NULL;
	}

	if(m_ppPosBufY)
	{
		for( int i=0; i<m_nPartCount; i++)
			if(m_ppPosBufY[i])
				delete[] m_ppPosBufY[i];

		delete[] m_ppPosBufY;
		m_ppPosBufY = NULL;
	}

	m_bUseIMGBUF = FALSE;
	m_nPartCount = 0;
	m_nHeight = 1;
	m_nWidth = 1;
	m_nPosX = 0;
	m_nPosY = 0;

	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;
	m_fPrvScaleX = 1.0f;
	m_fPrvScaleY = 1.0f;
}

BOOL CD3DImage::IsEnable()
{
	return m_pT3DTEX && m_pT3DVB && m_pMask;
}

void CD3DImage::Render( LPDIRECT3DDEVICE9 pDevice,
					    DWORD dwColor,
						int nPosX,
						int nPosY,
						LPRECT pRectSRC)
{
	D3DVIEWPORT9 vVIEW;

	if( !pDevice || (pRectSRC && (pRectSRC->left >= pRectSRC->right || pRectSRC->top >= pRectSRC->bottom)) )
		return;

	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000000);
	pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwColor);
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

	if( pRectSRC || CD3DImage::m_bUseGlobalClipRect )
	{
		pDevice->SetFVF(T3DFVF_TVERTEX);

		if( nPosX != m_nPosX ||
			nPosY != m_nPosY ||
			m_fScaleX != m_fPrvScaleX ||
			m_fScaleY != m_fPrvScaleY )
		{
			LPTVERTEX pData = NULL;

			float fBasePosX = (float) nPosX;
			float fBasePosY = (float) nPosY;

			for( int i=0; i<m_nPartCount; i++)
			{
				LPDIRECT3DVERTEXBUFFER9 pVB = m_pT3DVB[i]->GetVB();

				if( !pVB || FAILED(pVB->Lock( 0, 4 * sizeof(TVERTEX), (LPVOID *) &pData, 0)) )
					return;

				for( int j=0; j<4; ++j)
				{
					pData[j].m_fPosX = m_ppPosBufX[i][j] * m_fScaleX + fBasePosX;
					pData[j].m_fPosY = m_ppPosBufY[i][j] * m_fScaleY + fBasePosY;
				}

				pVB->Unlock();
			}

			m_nPosX = nPosX;
			m_nPosY = nPosY;
			m_fPrvScaleX = m_fScaleX;
			m_fPrvScaleY = m_fScaleY;
		}

		if(pRectSRC)
		{
			pDevice->GetViewport(&vVIEW);
			D3DVIEWPORT9 vRECT;

			vRECT.Height = (DWORD)( (FLOAT)(pRectSRC->bottom - pRectSRC->top) * m_fScaleY );
			vRECT.Width = (DWORD)( (FLOAT)(pRectSRC->right - pRectSRC->left) * m_fScaleX );
			vRECT.X = (DWORD)( (FLOAT)(max( INT(vVIEW.X), INT(m_nPosX + pRectSRC->left)) * m_fScaleX) );
			vRECT.Y = (DWORD)( (FLOAT)(max( INT(vVIEW.Y), INT(m_nPosY + pRectSRC->top)) * m_fScaleY) );

			vRECT.MinZ = 0.0f;
			vRECT.MaxZ = 1.0f;

			if( vVIEW.Y + vVIEW.Height < vRECT.Y + vRECT.Height )
				vRECT.Height = vVIEW.Y + vVIEW.Height - vRECT.Y;

			if( vVIEW.X + vVIEW.Width < vRECT.X + vRECT.Width )
				vRECT.Width = vVIEW.X + vVIEW.Width - vRECT.X;

			pDevice->SetViewport(&vRECT);
		}
	}
	else
	{
		m_nPosX = nPosX;
		m_nPosY = nPosY;
		m_fPrvScaleX = m_fScaleX;
		m_fPrvScaleY = m_fScaleY;
		
		pDevice->SetFVF(T3DFVF_LVERTEX);

		D3DXMATRIXA16 mat;
		mat._11 = 2.0f / m_dwScreenX * m_fScaleX;
		mat._21 = 0.0f;
		mat._31 = 0.0f;
		mat._41 = -1.0f + 2.0f / m_dwScreenX * nPosX;
		
		mat._12 = 0.0f;
		mat._22 = -2.0f / m_dwScreenY * m_fScaleY;
		mat._32 = 0.0f;
		mat._42 = 1.0f + 2.0f / m_dwScreenY * -nPosY;

		mat._13 = 0.0f;
		mat._23 = 0.0f;
		mat._33 = 0.0f;
		mat._43 = 0.5f;
		
		mat._14 = 0.0f;
		mat._24 = 0.0f;
		mat._34 = 0.0f;
		mat._44 = 1.0f;
		pDevice->SetTransform( D3DTS_WORLD, &mat );

		D3DXMatrixIdentity( &mat );
		pDevice->SetTransform( D3DTS_VIEW, &mat );
		pDevice->SetTransform( D3DTS_PROJECTION, &mat );
	}

	for( int i=0; i<m_nPartCount; i++)
	{
		LPDIRECT3DVERTEXBUFFER9 pVB = m_pT3DVB[i]->GetVB();
		LPDIRECT3DTEXTURE9 pTEX = m_pT3DTEX[i]->GetTEX();

		if( pTEX && pVB )
		{
			pDevice->SetStreamSource( 0, pVB, 0, sizeof(TVERTEX));
			pDevice->SetTexture( 0, pTEX);

			pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}

	if(pRectSRC)
		pDevice->SetViewport(&vVIEW);

	pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
}

LPT3DDATA CD3DImage::CreateDXT( LPDIRECT3DDEVICE9 pDevice,
							    LPDIRECT3DSURFACE9 pImage,
								BYTE bDXT)
{
	LPDIRECT3DSURFACE9 pMEM = NULL;
	D3DSURFACE_DESC desc;
	D3DFORMAT format;

	LPT3DDATA pResult = new T3DDATA();
	LPD3DXBUFFER pBUF = NULL;

	switch(bDXT)
	{
	case NON_COMP	: format = D3DFMT_A8R8G8B8; break;
	case DXT1		: format = D3DFMT_DXT1; break;
	case DXT2		: format = D3DFMT_DXT2; break;
	case DXT3		: format = D3DFMT_DXT3; break;
	case DXT4		: format = D3DFMT_DXT4; break;
	case DXT5		: format = D3DFMT_DXT5; break;
	default			: format = D3DFMT_DXT3; bDXT = DXT3; break;
	}
	pImage->GetDesc(&desc);

	pDevice->CreateOffscreenPlainSurface(
		desc.Width,
		desc.Height,
		format,
		D3DPOOL_SCRATCH,
		&pMEM,
		NULL);

	D3DXLoadSurfaceFromSurface(
		pMEM, NULL,
		NULL, pImage,
		NULL, NULL,
		D3DX_FILTER_NONE, 0);

	D3DXSaveSurfaceToFileInMemory(
		&pBUF,
		D3DXIFF_DDS,
		pMEM,
		NULL,
		NULL);

	pResult->m_dwSize = pBUF->GetBufferSize();
	pResult->m_pData = new BYTE[pResult->m_dwSize];
	memcpy( pResult->m_pData, pBUF->GetBufferPointer(), pResult->m_dwSize);

	pBUF->Release();
	pMEM->Release();

	return pResult;
}

LPT3DDATA CD3DImage::CreateDXT( LPDIRECT3DDEVICE9 pDevice,
							    LPDIRECT3DTEXTURE9 pTexture,
								BYTE bDXT)
{
	LPDIRECT3DSURFACE9 pImage = NULL;
	pTexture->GetSurfaceLevel( 0, &pImage);

	LPT3DDATA pResult = CreateDXT( pDevice, pImage, bDXT);
	pImage->Release();

	return pResult;
}
