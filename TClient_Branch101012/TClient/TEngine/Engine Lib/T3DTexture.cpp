#include "StdAfx.h"


CT3DTexture::CT3DTexture()
{
	m_bEnabled = FALSE;
	m_pTDATA = NULL;
}

CT3DTexture::~CT3DTexture()
{
	ReleaseDATA();
}

LPDIRECT3DTEXTURE9 CT3DTexture::GetTEX()
{
	if( !m_pTDATA || m_bEnabled )
		return (LPDIRECT3DTEXTURE9) m_pTDATA;

	LPTTEXLOADBUF pBUF = (LPTTEXLOADBUF) m_pTDATA;
	if(!pBUF->m_pDATA)
	{
		ReleaseDATA();
		return NULL;
	}

	D3DFORMAT eFormat = D3DFMT_DXT3;
	LPBYTE pDATA = NULL;

	if(pBUF->m_dwSIZE)
	{
		pDATA = new BYTE[pBUF->m_dwSIZE];
		uncompress( pDATA, &pBUF->m_dwSIZE, pBUF->m_pDATA, pBUF->m_dwDATA);
	}

	LPDIRECT3DTEXTURE9 pTEX = NULL;
	D3DXIMAGE_INFO vIMG;

	switch(pBUF->m_bFormat)
	{
	case NON_COMP	: eFormat = D3DFMT_A8R8G8B8; break;
	case DXT1		: eFormat = D3DFMT_DXT1; break;
	case DXT2		: eFormat = D3DFMT_DXT2; break;
	case DXT3		: eFormat = D3DFMT_DXT3; break;
	case DXT4		: eFormat = D3DFMT_DXT4; break;
	case DXT5		: eFormat = D3DFMT_DXT5; break;
	}

	HRESULT hr = D3DXGetImageInfoFromFileInMemory(
		pDATA ? pDATA : pBUF->m_pDATA,
		pDATA ? pBUF->m_dwSIZE : pBUF->m_dwDATA,
		&vIMG);

	if(FAILED(hr))
	{
		if(pDATA)
			delete[] pDATA;

		return NULL;
	}

	hr = D3DXCreateTextureFromFileInMemoryEx(
		CTachyonRes::m_pDEVICE->m_pDevice,
		pDATA ? pDATA : pBUF->m_pDATA,
		pDATA ? pBUF->m_dwSIZE : pBUF->m_dwDATA,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		vIMG.MipLevels, 0,
		eFormat,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT, 0,
		NULL,
		NULL,
		&pTEX);

	if(pDATA)
	{
		delete[] pDATA;
		pDATA = NULL;
	}

	if( FAILED(hr) || !pTEX )
		return NULL;
	ReleaseDATA();

	m_pTDATA = (LPBYTE) pTEX;
	m_bEnabled = TRUE;

	return pTEX;
}

void CT3DTexture::LoadT3DTEX( LPBYTE pDATA,
							  DWORD dwDATA,
							  DWORD dwSIZE,
							  BYTE bFormat)
{
	LPTTEXLOADBUF pBUF = new TTEXLOADBUF();

	pBUF->m_pDATA = new BYTE[dwDATA];
	pBUF->m_bFormat = bFormat;
	pBUF->m_dwDATA = dwDATA;
	pBUF->m_dwSIZE = dwSIZE;

	memcpy( pBUF->m_pDATA, pDATA, dwDATA);
	m_pTDATA = (LPBYTE) pBUF;
}

void CT3DTexture::ReleaseDATA()
{
	if(m_pTDATA)
		if(!m_bEnabled)
		{
			LPTTEXLOADBUF pBUF = (LPTTEXLOADBUF) m_pTDATA;

			if(pBUF->m_pDATA)
			{
				delete[] pBUF->m_pDATA;
				pBUF->m_pDATA = NULL;
			}

			delete pBUF;
		}
		else
			((LPDIRECT3DTEXTURE9) m_pTDATA)->Release();

	m_bEnabled = FALSE;
	m_pTDATA = NULL;
}


CT3DVertex::CT3DVertex()
{
	m_vTDATA.clear();

	m_bEnabled = FALSE;
	m_pTDATA = NULL;
}

CT3DVertex::~CT3DVertex()
{
	ReleaseDATA();
}

LPDIRECT3DVERTEXBUFFER9 CT3DVertex::GetVB()
{
	if(!m_pTDATA)
		return NULL;

	if(!m_bEnabled)
	{
		LPTVBLOADBUF pBUF = (LPTVBLOADBUF) m_pTDATA;

		if(!CreateVB( pBUF->m_dwSIZE, pBUF->m_dwUSE, pBUF->m_dwFVF))
			return NULL;

		delete pBUF;
	}

	while(!m_vTDATA.empty())
	{
		LPTLOADBUF pBUF = m_vTDATA.back();

		if(!UpdateT3DVB( pBUF->m_pDATA, pBUF->m_dwSIZE, pBUF->m_dwPOS))
			return NULL;

		m_vTDATA.pop_back();
		delete pBUF;
	}

	return (LPDIRECT3DVERTEXBUFFER9) m_pTDATA;
}

BYTE CT3DVertex::CreateVB( DWORD dwSIZE,
						   DWORD dwUSE,
						   DWORD dwFVF)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;

	HRESULT hr = CTachyonRes::m_pDEVICE->m_pDevice->CreateVertexBuffer(
		dwSIZE,
		dwUSE,
		dwFVF,
		D3DPOOL_MANAGED,
		&pVB,
		NULL);

	if( FAILED(hr) || !pVB )
		return FALSE;

	m_pTDATA = (LPBYTE) pVB;
	m_bEnabled = TRUE;

	return TRUE;
}

BYTE CT3DVertex::UpdateT3DVB( LPBYTE pDATA,
							  DWORD dwSIZE,
							  DWORD dwPOS)
{
	if( !m_bEnabled || !m_pTDATA )
		return FALSE;

	LPDIRECT3DVERTEXBUFFER9 pVB = (LPDIRECT3DVERTEXBUFFER9) m_pTDATA;
	LPVOID pBUF = NULL;

	HRESULT hr = pVB->Lock( dwPOS, dwSIZE, &pBUF, 0);
	if(FAILED(hr))
		return FALSE;

	memcpy( pBUF, pDATA, dwSIZE);
	pVB->Unlock();

	return TRUE;
}

void CT3DVertex::LoadT3DVB( DWORD dwSIZE,
							DWORD dwUSE,
							DWORD dwFVF)
{
	ReleaseDATA();

	if(!CreateVB( dwSIZE, dwUSE, dwFVF))
	{
		LPTVBLOADBUF pBUF = new TVBLOADBUF();

		pBUF->m_dwSIZE = dwSIZE;
		pBUF->m_dwUSE = dwUSE;
		pBUF->m_dwFVF = dwFVF;

		m_pTDATA = (LPBYTE) pBUF;
	}
}

void CT3DVertex::LoadT3DVBDATA( LPBYTE pDATA,
							    DWORD dwSIZE,
								DWORD dwPOS)
{
	if(!UpdateT3DVB( pDATA, dwSIZE, dwPOS))
	{
		LPTLOADBUF pBUF = new TLOADBUF();

		pBUF->m_pDATA = new BYTE[dwSIZE];
		pBUF->m_dwSIZE = dwSIZE;
		pBUF->m_dwPOS = dwPOS;
		memcpy( pBUF->m_pDATA, pDATA, dwSIZE);

		m_vTDATA.push_back(pBUF);
	}
}

void CT3DVertex::ReleaseDATA()
{
	if(m_pTDATA)
		if(!m_bEnabled)
		{
			LPTVBLOADBUF pBUF = (LPTVBLOADBUF) m_pTDATA;
			delete pBUF;
		}
		else
			((LPDIRECT3DVERTEXBUFFER9) m_pTDATA)->Release();

	while(!m_vTDATA.empty())
	{
		delete m_vTDATA.back();
		m_vTDATA.pop_back();
	}

	m_bEnabled = FALSE;
	m_pTDATA = NULL;
}


CT3DIndex::CT3DIndex()
{
	m_vTDATA.clear();

	m_bEnabled = FALSE;
	m_pTDATA = NULL;
}

CT3DIndex::~CT3DIndex()
{
	ReleaseDATA();
}

LPDIRECT3DINDEXBUFFER9 CT3DIndex::GetIB()
{
	if(!m_pTDATA)
		return NULL;

	if(!m_bEnabled)
	{
		LPTIBLOADBUF pBUF = (LPTIBLOADBUF) m_pTDATA;

		if(!CreateIB( pBUF->m_dwSIZE, pBUF->m_dwUSE, pBUF->m_nFORMAT))
			return NULL;

		delete pBUF;
	}

	while(!m_vTDATA.empty())
	{
		LPTLOADBUF pBUF = m_vTDATA.back();

		if(!UpdateT3DIB( pBUF->m_pDATA, pBUF->m_dwSIZE, pBUF->m_dwPOS))
			return NULL;

		m_vTDATA.pop_back();
		delete pBUF;
	}

	return (LPDIRECT3DINDEXBUFFER9) m_pTDATA;
}

BYTE CT3DIndex::CreateIB( DWORD dwSIZE,
						  DWORD dwUSE,
						  D3DFORMAT nFORMAT)
{
	LPDIRECT3DINDEXBUFFER9 pIB = NULL;

	HRESULT hr = CTachyonRes::m_pDEVICE->m_pDevice->CreateIndexBuffer(
		dwSIZE,
		dwUSE,
		nFORMAT,
		D3DPOOL_MANAGED,
		&pIB,
		NULL);

	if( FAILED(hr) || !pIB )
		return FALSE;

	m_pTDATA = (LPBYTE) pIB;
	m_bEnabled = TRUE;

	return TRUE;
}

BYTE CT3DIndex::UpdateT3DIB( LPBYTE pDATA,
							 DWORD dwSIZE,
							 DWORD dwPOS)
{
	if( !m_bEnabled || !m_pTDATA )
		return FALSE;

	LPDIRECT3DINDEXBUFFER9 pIB = (LPDIRECT3DINDEXBUFFER9) m_pTDATA;
	LPVOID pBUF = NULL;

	HRESULT hr = pIB->Lock( dwPOS, dwSIZE, &pBUF, 0);
	if(FAILED(hr))
		return FALSE;

	memcpy( pBUF, pDATA, dwSIZE);
	pIB->Unlock();

	return TRUE;
}

void CT3DIndex::LoadT3DIB( DWORD dwSIZE,
						   DWORD dwUSE,
						   D3DFORMAT nFORMAT)
{
	ReleaseDATA();

	if(!CreateIB( dwSIZE, dwUSE, nFORMAT))
	{
		LPTIBLOADBUF pBUF = new TIBLOADBUF();

		pBUF->m_dwSIZE = dwSIZE;
		pBUF->m_dwUSE = dwUSE;
		pBUF->m_nFORMAT = nFORMAT;

		m_pTDATA = (LPBYTE) pBUF;
	}
}

void CT3DIndex::LoadT3DIBDATA( LPBYTE pDATA,
							   DWORD dwSIZE,
							   DWORD dwPOS)
{
	if(!UpdateT3DIB( pDATA, dwSIZE, dwPOS))
	{
		LPTLOADBUF pBUF = new TLOADBUF();

		pBUF->m_pDATA = new BYTE[dwSIZE];
		pBUF->m_dwSIZE = dwSIZE;
		pBUF->m_dwPOS = dwPOS;
		memcpy( pBUF->m_pDATA, pDATA, dwSIZE);

		m_vTDATA.push_back(pBUF);
	}
}

void CT3DIndex::ReleaseDATA()
{
	if(m_pTDATA)
		if(!m_bEnabled)
		{
			LPTIBLOADBUF pBUF = (LPTIBLOADBUF) m_pTDATA;
			delete pBUF;
		}
		else
			((LPDIRECT3DINDEXBUFFER9) m_pTDATA)->Release();

	while(!m_vTDATA.empty())
	{
		delete m_vTDATA.back();
		m_vTDATA.pop_back();
	}

	m_bEnabled = FALSE;
	m_pTDATA = NULL;
}


CT3DImage::CT3DImage()
{
	m_bEnabled = FALSE;
	m_pTDATA = NULL;
}

CT3DImage::~CT3DImage()
{
	ReleaseDATA();
}

CD3DImage *CT3DImage::GetIMG()
{
	if( !m_pTDATA || m_bEnabled )
		return (CD3DImage *) m_pTDATA;

	LPTLOADBUF pBUF = (LPTLOADBUF) m_pTDATA;
	if(!pBUF->m_pDATA)
	{
		ReleaseDATA();
		return NULL;
	}
	CD3DImage *pIMG = new CD3DImage();

	if(!pIMG->LoadImageFile( CTachyonRes::m_pDEVICE->m_pDevice, pBUF->m_pDATA, pBUF->m_dwSIZE))
	{
		delete pIMG;
		return NULL;
	}
	ReleaseDATA();

	m_pTDATA = (LPBYTE) pIMG;
	m_bEnabled = TRUE;

	return pIMG;
}

void CT3DImage::LoadT3DIMG( LPBYTE pDATA,
						    DWORD dwSIZE)
{
	CD3DImage *pIMG = new CD3DImage();
	ReleaseDATA();

	if(pIMG->LoadImageFile( CTachyonRes::m_pDEVICE->m_pDevice, pDATA, dwSIZE))
	{
		m_pTDATA = (LPBYTE) pIMG;
		m_bEnabled = TRUE;
	}
	else
	{
		LPTLOADBUF pBUF = new TLOADBUF();

		pBUF->m_pDATA = new BYTE[dwSIZE];
		pBUF->m_dwSIZE = dwSIZE;
		memcpy( pBUF->m_pDATA, pDATA, dwSIZE);

		m_pTDATA = (LPBYTE) pBUF;
		delete pIMG;
	}
}

void CT3DImage::ReleaseDATA()
{
	if(m_pTDATA)
		if(m_bEnabled)
			delete (CD3DImage *) m_pTDATA;
		else
			delete (LPTLOADBUF) m_pTDATA;

	m_bEnabled = FALSE;
	m_pTDATA = NULL;
}
