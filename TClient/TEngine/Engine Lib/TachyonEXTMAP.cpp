#include "StdAfx.h"


CTachyonEXTMAP::CTachyonEXTMAP()
{
	m_vSHADOWTEX.clear();

	m_pTILETEX = NULL;
	m_pGEOTEX = NULL;

	m_pSHADOW = NULL;
	m_pREGION = NULL;
	m_pDETAIL = NULL;
	m_pDATA = NULL;

	m_vDETAIL.clear();

	m_nTileCount = 0;
}

CTachyonEXTMAP::~CTachyonEXTMAP()
{
	Release();
}

void CTachyonEXTMAP::Release()
{
	while(!m_vSHADOWTEX.empty())
	{
		m_vSHADOWTEX.back()->Release();
		m_vSHADOWTEX.pop_back();
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

	if(m_pSHADOW)
	{
		delete[] m_pSHADOW;
		m_pSHADOW = NULL;
	}

	if(m_pREGION)
	{
		delete[] m_pREGION;
		m_pREGION = NULL;
	}

	if(m_pDETAIL)
	{
		delete[] m_pDETAIL;
		m_pDETAIL = NULL;
	}

	if(m_pDATA)
	{
		delete[] m_pDATA;
		m_pDATA = NULL;
	}

	m_vDETAIL.clear();
}

WORD CTachyonEXTMAP::FindDetailID( int nTileX, int nTileZ)
{
	BYTE bDETAIL = m_pDETAIL[nTileZ * m_nTileCount + nTileX];
	if( bDETAIL == 0xFF )
		return TILE_NULL;

	return m_vDETAIL[ bDETAIL ];
}

WORD CTachyonEXTMAP::FindDetailID( int nIndex )
{
	BYTE bDETAIL = m_pDETAIL[ nIndex ];
	if( bDETAIL == 0xFF )
		return TILE_NULL;

	return m_vDETAIL[ bDETAIL ];
}

FLOAT CTachyonEXTMAP::GetHeight( FLOAT fPosX,
								 FLOAT fPosY,
								 FLOAT fPosZ)
{
	int nPosX = INT(fPosX);
	int nPosZ = INT(fPosZ);

	FLOAT fResX = fPosX - nPosX;
	FLOAT fResZ = fPosZ - nPosZ;

	if( fResX > 0.0f ||
		fResZ > 0.0f )
	{
		D3DXPLANE vPlane;

		if( fResX + fResZ < 1.0f )
		{
			D3DXVECTOR3 vPoint[3] = {
				D3DXVECTOR3( 0.0f, m_pDATA[(nPosZ + 1) * (m_nTileCount + 1) + nPosX], 1.0f),
				D3DXVECTOR3( 1.0f, m_pDATA[nPosZ * (m_nTileCount + 1) + nPosX + 1], 0.0f),
				D3DXVECTOR3( 0.0f, m_pDATA[nPosZ * (m_nTileCount + 1) + nPosX], 0.0f)};

			D3DXPlaneFromPoints(
				&vPlane,
				&vPoint[0],
				&vPoint[1],
				&vPoint[2]);
		}
		else
		{
			D3DXVECTOR3 vPoint[3] = {
				D3DXVECTOR3( 1.0f, m_pDATA[(nPosZ + 1) * (m_nTileCount + 1) + nPosX + 1], 1.0f),
				D3DXVECTOR3( 1.0f, m_pDATA[nPosZ * (m_nTileCount + 1) + nPosX + 1], 0.0f),
				D3DXVECTOR3( 0.0f, m_pDATA[(nPosZ + 1) * (m_nTileCount + 1) + nPosX], 1.0f)};

			D3DXPlaneFromPoints(
				&vPlane,
				&vPoint[0],
				&vPoint[1],
				&vPoint[2]);
		}

		return -(vPlane.a * fResX + vPlane.c * fResZ + vPlane.d) / vPlane.b;
	}

	return nPosX < 0 || nPosZ < 0 || nPosX > m_nTileCount || nPosZ > m_nTileCount ? 0.0f : m_pDATA[nPosZ * (m_nTileCount + 1) + nPosX];
}
