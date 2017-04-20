// TachyonAnimation.cpp: implementation of the CTachyonAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CTachyonAnimation::CTachyonAnimation()
{
	m_dwNodeCount = 0;
	m_dwStartTick = 0;
	m_dwEndTick = 0;
	m_dwSIZE = 0;
	m_dwPOS = 0;
	m_dwREF = 0;

	m_pBones = NULL;
	m_pTREE = NULL;
	m_pNode = NULL;
	m_pDATA = NULL;
	m_pVB = NULL;

	m_bFileID = 0;
}

CTachyonAnimation::~CTachyonAnimation()
{
	Release();
}

BOOL CTachyonAnimation::LoadAnimation( LPBYTE pData)
{
	int nTAFVersion = 0;

	nTAFVersion = *((int *) pData); pData += sizeof(int);
	Release();

	switch(nTAFVersion)
	{
	case 1		:
		{
			m_dwNodeCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_dwStartTick = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_dwEndTick = *((LPDWORD) pData); pData += sizeof(DWORD);

			if( m_dwNodeCount > 0 )
			{
				m_pBones = new D3DXMATRIX[m_dwNodeCount];
				m_pTREE = new BONESNODE[m_dwNodeCount];
				m_pNode = new BONESANIMATION[m_dwNodeCount];

				m_pDATA = new VBDATA[m_dwNodeCount];
				m_pVB = new MESHVB[m_dwNodeCount];

				memcpy( m_pBones, pData, sizeof(D3DXMATRIX) * m_dwNodeCount);
				pData += sizeof(D3DXMATRIX) * m_dwNodeCount;

				for( DWORD i=0; i<m_dwNodeCount; i++)
				{
					m_pDATA[i].m_dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
					m_pDATA[i].m_dwFVF = D3DFVF_XYZ;
					m_pVB[i].m_dwCount = m_pDATA[i].m_dwCount;

					if(m_pVB[i].m_dwCount)
					{
						LPMESHVERTEX pDEST = new MESHVERTEX[m_pVB[i].m_dwCount];
						LPWMESHVERTEX pSRC = (LPWMESHVERTEX) pData;

						m_pDATA[i].m_pVB = new BYTE[m_pDATA[i].m_dwCount * sizeof(D3DXVECTOR3)];
						for( DWORD j=0; j<m_pVB[i].m_dwCount; j++)
						{
							((LPD3DXVECTOR3) m_pDATA[i].m_pVB)[j] = D3DXVECTOR3(
								pSRC[j].m_fPosX,
								pSRC[j].m_fPosY,
								pSRC[j].m_fPosZ);

							pDEST[j].m_fNormalX = pSRC[j].m_fNormalX;
							pDEST[j].m_fNormalY = pSRC[j].m_fNormalY;
							pDEST[j].m_fNormalZ = pSRC[j].m_fNormalZ;
							pDEST[j].m_fPosX = pSRC[j].m_fPosX;
							pDEST[j].m_fPosY = pSRC[j].m_fPosY;
							pDEST[j].m_fPosZ = pSRC[j].m_fPosZ;
							pDEST[j].m_fU1 = pSRC[j].m_fU;
							pDEST[j].m_fV1 = pSRC[j].m_fV;
							pDEST[j].m_fU2 = pSRC[j].m_fU;
							pDEST[j].m_fV2 = pSRC[j].m_fV;
						}

						m_pVB[i].m_vVB.LoadT3DVB(
							m_pVB[i].m_dwCount * sizeof(MESHVERTEX),
							0, T3DFVF_MESHVERTEX);

						m_pVB[i].m_vVB.LoadT3DVBDATA(
							(LPBYTE) pDEST,
							m_pVB[i].m_dwCount * sizeof(MESHVERTEX), 0);
						delete[] pDEST;

						pData += m_pVB[i].m_dwCount * sizeof(WMESHVERTEX);
						m_pDATA[i].m_dwCount /= 3;
						m_pVB[i].m_dwCount /= 3;
					}
				}
				pData += sizeof(BYTE);

				for( i=0; i<m_dwNodeCount; i++)
				{
					m_pTREE[i].m_bINDEX = *((LPBYTE) pData); pData += sizeof(BYTE);
					m_pTREE[i].m_bParentROT = *((LPBYTE) pData); pData += sizeof(BYTE);
					m_pTREE[i].m_bParentPOS = *((LPBYTE) pData); pData += sizeof(BYTE);
				}

				for( i=0; i<m_dwNodeCount; i++)
				{
					int nNODE = 0;

					for( int j=0; j<m_dwNodeCount; j++)
						if( m_pTREE[j].m_bINDEX == i )
							nNODE = j;

					m_pNode[nNODE].m_nPositionKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_pNode[nNODE].m_nPositionKeyCount > 0 )
					{
						LPTPOINTKEY pTKEY = new TPOINTKEY[m_pNode[nNODE].m_nPositionKeyCount];
						BYTE bSIZE[3][2] = {
							{ 36, 24},
							{ 40, 28},
							{ 16,  4}};
						BYTE bTYPE = *((LPBYTE) pData);

						m_pNode[nNODE].m_pPositionKey = pTKEY;
						pData += sizeof(BYTE);

						for( j=0; j<m_pNode[nNODE].m_nPositionKeyCount; j++)
						{
							pTKEY[j].m_vKeyPoint = *((LPD3DXVECTOR3) &pData[bSIZE[bTYPE - 1][1]]);
							pTKEY[j].m_dwTime = *((LPDWORD) &pData[0]);
							pData += bSIZE[bTYPE - 1][0];
						}
					}

					m_pNode[nNODE].m_nRotationKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_pNode[nNODE].m_nRotationKeyCount > 0 )
					{
						LPTROTKEY pTKEY = new TROTKEY[m_pNode[nNODE].m_nRotationKeyCount];
						BYTE bSIZE[3][2] = {
							{ 40, 24},
							{ 52,  4},
							{ 20,  4}};
						BYTE bTYPE = *((LPBYTE) pData);

						m_pNode[nNODE].m_pRotationKey = pTKEY;
						pData += sizeof(BYTE);

						for( j=0; j<m_pNode[nNODE].m_nRotationKeyCount; j++)
						{
							if( bTYPE == 1 )
							{
								D3DXQuaternionRotationAxis(
									&pTKEY[j].m_vKeyQuat,
									(LPD3DXVECTOR3) &pData[bSIZE[bTYPE - 1][1]],
									*((FLOAT *) &pData[bSIZE[bTYPE - 1][1] + sizeof(D3DXVECTOR3)]));
							}
							else
								pTKEY[j].m_vKeyQuat = *((LPD3DXQUATERNION) &pData[bSIZE[bTYPE - 1][1]]);

							pTKEY[j].m_dwTime = *((LPDWORD) &pData[0]);
							pData += bSIZE[bTYPE - 1][0];
						}

						if( bTYPE != 1 )
							for( j=m_pNode[nNODE].m_nRotationKeyCount - 1; j>0; j--)
							{
								D3DXQUATERNION vINV;

								D3DXQuaternionInverse(
									&vINV,
									&pTKEY[j - 1].m_vKeyQuat);

								pTKEY[j].m_vKeyQuat = vINV * pTKEY[j].m_vKeyQuat;
							}
					}

					m_pNode[nNODE].m_nScaleKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_pNode[nNODE].m_nScaleKeyCount > 0 )
					{
						LPTSCALEKEY pTKEY = new TSCALEKEY[m_pNode[nNODE].m_nScaleKeyCount];
						BYTE bSIZE[3][2] = {
							{ 52, 40},
							{ 56, 44},
							{ 32, 20}};
						BYTE bTYPE = *((LPBYTE) pData);

						m_pNode[nNODE].m_pScaleKey = pTKEY;
						pData += sizeof(BYTE);

						for( j=0; j<m_pNode[nNODE].m_nScaleKeyCount; j++)
						{
							pTKEY[j].m_vKeyScale = *((LPD3DXVECTOR3) &pData[bSIZE[bTYPE - 1][1]]);
							pTKEY[j].m_dwTime = *((LPDWORD) &pData[0]);
							pData += bSIZE[bTYPE - 1][0];
						}
					}
				}
			}
		}

		break;

	case 300	:
		{
			m_dwNodeCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_dwStartTick = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_dwEndTick = *((LPDWORD) pData); pData += sizeof(DWORD);

			if( m_dwNodeCount > 0 )
			{
				m_pBones = new D3DXMATRIX[m_dwNodeCount];
				m_pTREE = new BONESNODE[m_dwNodeCount];
				m_pNode = new BONESANIMATION[m_dwNodeCount];

				m_pDATA = new VBDATA[m_dwNodeCount];
				m_pVB = new MESHVB[m_dwNodeCount];

				memcpy( m_pBones, pData, sizeof(D3DXMATRIX) * m_dwNodeCount);
				pData += sizeof(D3DXMATRIX) * m_dwNodeCount;

				for( DWORD i=0; i<m_dwNodeCount; i++)
				{
					m_pDATA[i].m_dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
					m_pDATA[i].m_dwFVF = D3DFVF_XYZ;
					m_pVB[i].m_dwCount = m_pDATA[i].m_dwCount;

					if(m_pVB[i].m_dwCount)
					{
						LPMESHVERTEX pDEST = new MESHVERTEX[m_pVB[i].m_dwCount];
						LPWMESHVERTEX pSRC = (LPWMESHVERTEX) pData;

						m_pDATA[i].m_pVB = new BYTE[m_pDATA[i].m_dwCount * sizeof(D3DXVECTOR3)];
						for( DWORD j=0; j<m_pVB[i].m_dwCount; j++)
						{
							((LPD3DXVECTOR3) m_pDATA[i].m_pVB)[j] = D3DXVECTOR3(
								pSRC[j].m_fPosX,
								pSRC[j].m_fPosY,
								pSRC[j].m_fPosZ);

							pDEST[j].m_fNormalX = pSRC[j].m_fNormalX;
							pDEST[j].m_fNormalY = pSRC[j].m_fNormalY;
							pDEST[j].m_fNormalZ = pSRC[j].m_fNormalZ;
							pDEST[j].m_fPosX = pSRC[j].m_fPosX;
							pDEST[j].m_fPosY = pSRC[j].m_fPosY;
							pDEST[j].m_fPosZ = pSRC[j].m_fPosZ;
							pDEST[j].m_fU1 = pSRC[j].m_fU;
							pDEST[j].m_fV1 = pSRC[j].m_fV;
							pDEST[j].m_fU2 = pSRC[j].m_fU;
							pDEST[j].m_fV2 = pSRC[j].m_fV;
						}

						m_pVB[i].m_vVB.LoadT3DVB(
							m_pVB[i].m_dwCount * sizeof(MESHVERTEX),
							0, T3DFVF_MESHVERTEX);

						m_pVB[i].m_vVB.LoadT3DVBDATA(
							(LPBYTE) pDEST,
							m_pVB[i].m_dwCount * sizeof(MESHVERTEX), 0);
						delete[] pDEST;

						pData += m_pVB[i].m_dwCount * sizeof(WMESHVERTEX);
						m_pDATA[i].m_dwCount /= 3;
						m_pVB[i].m_dwCount /= 3;
					}
				}

				memcpy( m_pTREE, pData, m_dwNodeCount * sizeof(BONESNODE));
				pData += m_dwNodeCount * sizeof(BONESNODE);

				for( i=0; i<m_dwNodeCount; i++)
				{
					m_pNode[i].m_nPositionKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_pNode[i].m_nPositionKeyCount > 0 )
					{
						m_pNode[i].m_pPositionKey = new TPOINTKEY[m_pNode[i].m_nPositionKeyCount];

						memcpy( m_pNode[i].m_pPositionKey, pData, m_pNode[i].m_nPositionKeyCount * sizeof(TPOINTKEY));
						pData += m_pNode[i].m_nPositionKeyCount * sizeof(TPOINTKEY);
					}

					m_pNode[i].m_nRotationKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_pNode[i].m_nRotationKeyCount > 0 )
					{
						m_pNode[i].m_pRotationKey = new TROTKEY[m_pNode[i].m_nRotationKeyCount];

						memcpy( m_pNode[i].m_pRotationKey, pData, m_pNode[i].m_nRotationKeyCount * sizeof(TROTKEY));
						pData += m_pNode[i].m_nRotationKeyCount * sizeof(TROTKEY);
					}

					m_pNode[i].m_nScaleKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_pNode[i].m_nScaleKeyCount > 0 )
					{
						m_pNode[i].m_pScaleKey = new TSCALEKEY[m_pNode[i].m_nScaleKeyCount];

						memcpy( m_pNode[i].m_pScaleKey, pData, m_pNode[i].m_nScaleKeyCount * sizeof(TSCALEKEY));
						pData += m_pNode[i].m_nScaleKeyCount * sizeof(TSCALEKEY);
					}
				}
			}
		}

		break;

	case 301	:
		{
			m_dwNodeCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_dwStartTick = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_dwEndTick = *((LPDWORD) pData); pData += sizeof(DWORD);

			if( m_dwNodeCount > 0 )
			{
				m_pBones = new D3DXMATRIX[m_dwNodeCount];
				m_pTREE = new BONESNODE[m_dwNodeCount];
				m_pNode = new BONESANIMATION[m_dwNodeCount];

				m_pDATA = new VBDATA[m_dwNodeCount];
				m_pVB = new MESHVB[m_dwNodeCount];

				memcpy( m_pBones, pData, sizeof(D3DXMATRIX) * m_dwNodeCount);
				pData += sizeof(D3DXMATRIX) * m_dwNodeCount;

				for( DWORD i=0; i<m_dwNodeCount; i++)
				{
					m_pDATA[i].m_dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
					m_pDATA[i].m_dwFVF = D3DFVF_XYZ;
					m_pVB[i].m_dwCount = m_pDATA[i].m_dwCount;

					if(m_pVB[i].m_dwCount)
					{
						LPMESHVERTEX pDEST = new MESHVERTEX[m_pVB[i].m_dwCount];
						LPTMESHBUFFER pSRC = (LPTMESHBUFFER) pData;

						m_pDATA[i].m_pVB = new BYTE[m_pDATA[i].m_dwCount * sizeof(D3DXVECTOR3)];
						for( DWORD j=0; j<m_pVB[i].m_dwCount; j++)
						{
							((LPD3DXVECTOR3) m_pDATA[i].m_pVB)[j] = D3DXVECTOR3(
								pSRC[j].m_fPosX,
								pSRC[j].m_fPosY,
								pSRC[j].m_fPosZ);

							pDEST[j].m_fNormalX = pSRC[j].m_fNormalX;
							pDEST[j].m_fNormalY = pSRC[j].m_fNormalY;
							pDEST[j].m_fNormalZ = pSRC[j].m_fNormalZ;
							pDEST[j].m_fPosX = pSRC[j].m_fPosX;
							pDEST[j].m_fPosY = pSRC[j].m_fPosY;
							pDEST[j].m_fPosZ = pSRC[j].m_fPosZ;
							pDEST[j].m_fU1 = pSRC[j].m_fU1;
							pDEST[j].m_fV1 = pSRC[j].m_fV1;
							pDEST[j].m_fU2 = pSRC[j].m_fU2;
							pDEST[j].m_fV2 = pSRC[j].m_fV2;
						}

						m_pVB[i].m_vVB.LoadT3DVB(
							m_pVB[i].m_dwCount * sizeof(MESHVERTEX),
							0, T3DFVF_MESHVERTEX);

						m_pVB[i].m_vVB.LoadT3DVBDATA(
							(LPBYTE) pDEST,
							m_pVB[i].m_dwCount * sizeof(MESHVERTEX), 0);
						delete[] pDEST;

						pData += m_pVB[i].m_dwCount * sizeof(TMESHBUFFER);
						m_pDATA[i].m_dwCount /= 3;
						m_pVB[i].m_dwCount /= 3;
					}
				}

				memcpy( m_pTREE, pData, m_dwNodeCount * sizeof(BONESNODE));
				pData += m_dwNodeCount * sizeof(BONESNODE);

				for( i=0; i<m_dwNodeCount; i++)
				{
					m_pNode[i].m_nPositionKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_pNode[i].m_nPositionKeyCount > 0 )
					{
						m_pNode[i].m_pPositionKey = new TPOINTKEY[m_pNode[i].m_nPositionKeyCount];

						memcpy( m_pNode[i].m_pPositionKey, pData, m_pNode[i].m_nPositionKeyCount * sizeof(TPOINTKEY));
						pData += m_pNode[i].m_nPositionKeyCount * sizeof(TPOINTKEY);
					}

					m_pNode[i].m_nRotationKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_pNode[i].m_nRotationKeyCount > 0 )
					{
						m_pNode[i].m_pRotationKey = new TROTKEY[m_pNode[i].m_nRotationKeyCount];

						memcpy( m_pNode[i].m_pRotationKey, pData, m_pNode[i].m_nRotationKeyCount * sizeof(TROTKEY));
						pData += m_pNode[i].m_nRotationKeyCount * sizeof(TROTKEY);
					}

					m_pNode[i].m_nScaleKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_pNode[i].m_nScaleKeyCount > 0 )
					{
						m_pNode[i].m_pScaleKey = new TSCALEKEY[m_pNode[i].m_nScaleKeyCount];

						memcpy( m_pNode[i].m_pScaleKey, pData, m_pNode[i].m_nScaleKeyCount * sizeof(TSCALEKEY));
						pData += m_pNode[i].m_nScaleKeyCount * sizeof(TSCALEKEY);
					}
				}
			}
		}

		break;

	default		:
		{
			Release();
			return FALSE;
		}

		break;
	}

	for( int i=0; i<m_dwNodeCount; i++)
	{
		InitPositionKey(&m_pNode[i]);
		InitRotationKey(&m_pNode[i]);
		InitScaleKey(&m_pNode[i]);
	}

	return TRUE;
}

BOOL CTachyonAnimation::LoadAnimation( CString strFileName)
{
	LPBYTE pData = NULL;
	BOOL bResult = TRUE;

	TRY
	{
		CFile file( strFileName, CFile::modeRead);
		int nLength = INT(file.GetLength());

		pData = new BYTE[nLength];
		file.Read( pData, nLength);
		bResult = LoadAnimation(pData);

		if(pData)
		{
			delete[] pData;
			pData = NULL;
		}
	}
	CATCH_ALL(e)
	{
		if(pData)
		{
			delete[] pData;
			pData = NULL;
		}

		Release();
		return FALSE;
	}
	END_CATCH_ALL

	if(pData)
	{
		delete[] pData;
		pData = NULL;
	}

	return bResult;
}

void CTachyonAnimation::Release()
{
	if(m_pBones)
	{
		delete[] m_pBones;
		m_pBones = NULL;
	}

	if(m_pTREE)
	{
		delete[] m_pTREE;
		m_pTREE = NULL;
	}

	if(m_pNode)
	{
		delete[] m_pNode;
		m_pNode = NULL;
	}

	if(m_pDATA)
	{
		delete[] m_pDATA;
		m_pDATA = NULL;
	}

	if(m_pVB)
	{
		delete[] m_pVB;
		m_pVB = NULL;
	}

	m_dwNodeCount = 0;
	m_dwStartTick = 0;
	m_dwEndTick = 0;
	m_dwREF = 0;
}

void CTachyonAnimation::Lock( LPVECTORSTRING pFILE)
{
	if( !m_dwREF && m_dwSIZE )
	{
		CFile file( _T(".\\Data\\") + (*pFILE)[m_bFileID], CFile::modeRead|CFile::typeBinary);
		LPBYTE pDATA = new BYTE[m_dwSIZE];

		file.Seek( m_dwPOS, SEEK_SET);
		file.Read( pDATA, m_dwSIZE);
		LoadAnimation(pDATA);

		delete[] pDATA;
	}

	m_dwREF++;
}

void CTachyonAnimation::Unlock()
{
	if(m_dwREF)
	{
		m_dwREF--;

		if(!m_dwREF)
			Release();
	}
}

void CTachyonAnimation::Render( LPDIRECT3DDEVICE9 pDevice,
							    LPTPIVOT pPivot,
								D3DXMATRIX& vWorld,
								FLOAT fTimeValue)
{
	static D3DXMATRIX vLocal[255];

	if(!pDevice)
		return;

	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE);
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
	pDevice->SetFVF(T3DFVF_MESHVERTEX);

	GetFrameMatrix(
		vLocal,
		NULL,
		NULL,
		pPivot,
		vWorld, 0,
		fTimeValue,
		0.0f);

	for( DWORD i=0; i<m_dwNodeCount; i++)
		if( m_pVB[i].m_dwCount > 0 )
		{
			pDevice->SetTransform( D3DTS_WORLD, &vLocal[i + 1]);
			Render( pDevice, i);
		}
}

void CTachyonAnimation::Render( LPDIRECT3DDEVICE9 pDevice, int nIndex)
{
	pDevice->SetStreamSource( 0, m_pVB[nIndex].m_vVB.GetVB(), 0, sizeof(MESHVERTEX));
	pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_pVB[nIndex].m_dwCount);
}

void CTachyonAnimation::GetBlendKEY( LPD3DXQUATERNION pBlendKEY,
									 LPD3DXMATRIX pPREV,
									 LPD3DXMATRIX pNEXT)
{
	for( DWORD i=0; i<m_dwNodeCount; i++)
	{
		D3DXQUATERNION vPREV;
		D3DXQUATERNION vNEXT;
		D3DXQUATERNION vINV;

		D3DXVECTOR3 vPREVS;
		D3DXVECTOR3 vNEXTS;
		D3DXVECTOR3 vPOS;

		D3DXMatrixDecompose(
			&vPREVS,
			&vPREV,
			&vPOS,
			&pPREV[m_pTREE[i].m_bINDEX + 1]);

		D3DXMatrixDecompose(
			&vNEXTS,
			&vNEXT,
			&vPOS,
			&pNEXT[m_pTREE[i].m_bINDEX + 1]);

		D3DXQuaternionInverse(
			&vINV,
			&vPREV);

		pBlendKEY[m_pTREE[i].m_bINDEX + 1] = vINV * GetReviseROT( &vPREVS, &vNEXTS) * vNEXT;
	}
}

D3DXQUATERNION CTachyonAnimation::GetReviseROT( LPD3DXVECTOR3 pPREV,
											    LPD3DXVECTOR3 pNEXT)
{
	D3DXQUATERNION vRESULT(
		0.0f,
		0.0f,
		0.0f,
		1.0f);

	if( pPREV->x * pNEXT->x < 0.0f &&
		pPREV->y * pNEXT->y < 0.0f )
	{
		D3DXQuaternionRotationAxis(
			&vRESULT,
			&D3DXVECTOR3( 0.0f, 0.0f, pPREV->y < 0.0f ? -1.0f : 1.0f),
			D3DX_PI);

		return vRESULT;
	}

	if( pPREV->y * pNEXT->y < 0.0f &&
		pPREV->z * pNEXT->z < 0.0f )
	{
		D3DXQuaternionRotationAxis(
			&vRESULT,
			&D3DXVECTOR3( pPREV->z < 0.0f ? -1.0f : 1.0f, 0.0f, 0.0f),
			D3DX_PI);

		return vRESULT;
	}

	if( pPREV->z * pNEXT->z < 0.0f &&
		pPREV->x * pNEXT->x < 0.0f )
	{
		D3DXQuaternionRotationAxis(
			&vRESULT,
			&D3DXVECTOR3( 0.0f, pPREV->x < 0.0f ? -1.0f : 1.0f, 0.0f),
			D3DX_PI);

		return vRESULT;
	}

	return vRESULT;
}

void CTachyonAnimation::GetFrameMatrix( LPD3DXMATRIX pResult,
									    LPD3DXMATRIX pBlend,
										LPD3DXQUATERNION pBlendKEY,
									    LPTPIVOT pPivot,
										D3DXMATRIX& vWorld,
										BYTE bRootID,
										FLOAT fTimeValue,
										FLOAT fBlendTime)
{
	static D3DXQUATERNION vIDENTITY(
		0.0f,
		0.0f,
		0.0f,
		1.0f);

	static D3DXMATRIX vRESULT[MAX_PIVOT];
	static D3DXMATRIX vTSCALE[MAX_PIVOT];
	static BYTE vFILTER[MAX_PIVOT];

	ZeroMemory( vFILTER, MAX_PIVOT * sizeof(BYTE));
	vFILTER[bRootID] = TRUE;

	D3DXMatrixIdentity(&vTSCALE[0]);
	D3DXMatrixIdentity(&vRESULT[0]);

	for( BYTE i=0; i<m_dwNodeCount; i++)
	{
		GetFrameMatrix(
			vRESULT,
			pPivot, i,
			fTimeValue);
		D3DXMatrixIdentity(&vTSCALE[m_pTREE[i].m_bINDEX + 1]);

		if(IsValidNode( &m_pTREE[i], vFILTER))
			vFILTER[m_pTREE[i].m_bINDEX + 1] = TRUE;
	}

	if(bRootID)
	{
		if(pPivot)
		{
			D3DXMatrixScaling(
				&vTSCALE[bRootID],
				pPivot[bRootID - 1].m_vScale.x,
				pPivot[bRootID - 1].m_vScale.y,
				pPivot[bRootID - 1].m_vScale.z);
		}

		vRESULT[bRootID] = CTMath::Inverse(&(m_pBones[bRootID - 1] * vRESULT[bRootID]));
	}

	for( i=0; i<m_dwNodeCount; i++)
		if(IsValidNode( &m_pTREE[i], vFILTER))
		{
			D3DXQUATERNION vROT = vIDENTITY;
			D3DXVECTOR3 vPOS;

			if(pBlend)
			{
				D3DXMATRIX vBLEND;

				D3DXVECTOR3 vPREV(
					pBlend[m_pTREE[i].m_bINDEX + 1]._41,
					pBlend[m_pTREE[i].m_bINDEX + 1]._42,
					pBlend[m_pTREE[i].m_bINDEX + 1]._43);

				D3DXVECTOR3 vNEXT(
					m_pBones[m_pTREE[i].m_bINDEX]._41,
					m_pBones[m_pTREE[i].m_bINDEX]._42,
					m_pBones[m_pTREE[i].m_bINDEX]._43);

				D3DXQuaternionSlerp(
					&vROT,
					&vIDENTITY,
					&pBlendKEY[m_pTREE[i].m_bINDEX + 1],
					fBlendTime);

				D3DXVec3Lerp(
					&vPOS,
					&vPREV,
					&vNEXT,
					fBlendTime);

				D3DXMatrixRotationQuaternion(
					&vBLEND,
					&vROT);

				vBLEND = pBlend[m_pTREE[i].m_bINDEX + 1] * vBLEND;
				vBLEND._41 = vPOS.x;
				vBLEND._42 = vPOS.y;
				vBLEND._43 = vPOS.z;

				pResult[m_pTREE[i].m_bINDEX + 1] = vBLEND * vRESULT[m_pTREE[i].m_bINDEX + 1];
			}
			else
				pResult[m_pTREE[i].m_bINDEX + 1] = m_pBones[m_pTREE[i].m_bINDEX] * vRESULT[m_pTREE[i].m_bINDEX + 1];

			if(pPivot)
			{
				D3DXQUATERNION vTBASIS;

				CTMath::GetRotationQUAT(
					&vTBASIS,
					&vRESULT[m_pTREE[i].m_bINDEX + 1]);
				vTBASIS = vROT * vTBASIS;

				D3DXQuaternionNormalize(
					&vROT,
					&vTBASIS);

				vPOS.x = pResult[m_pTREE[i].m_bINDEX + 1]._41;
				vPOS.y = pResult[m_pTREE[i].m_bINDEX + 1]._42;
				vPOS.z = pResult[m_pTREE[i].m_bINDEX + 1]._43;

				D3DXMatrixTransformation(
					&vTSCALE[m_pTREE[i].m_bINDEX + 1],
					&vPOS,
					&vROT,
					&pPivot[m_pTREE[i].m_bINDEX].m_vScale,
					NULL,
					NULL,
					NULL);
			}
			vTSCALE[m_pTREE[i].m_bINDEX + 1] = vTSCALE[m_pTREE[i].m_bParentPOS] * vTSCALE[m_pTREE[i].m_bINDEX + 1];

			pResult[m_pTREE[i].m_bINDEX + 1] *= vTSCALE[m_pTREE[i].m_bINDEX + 1] * vRESULT[bRootID] * vWorld;
		}

	if(!bRootID)
		pResult[bRootID] = vWorld;
}

void CTachyonAnimation::GetFrameMatrix( LPD3DXMATRIX pResult,
										LPTPIVOT pPivot,
										BYTE bNODE,
										FLOAT fTimeValue)
{
	D3DXQUATERNION vParent;
	D3DXMATRIX vLocal;

	D3DXQUATERNION vRotation = CalcRotation(
		&m_pNode[bNODE],
		fTimeValue);

	CTMath::GetRotationQUAT(
		&vParent,
		&pResult[m_pTREE[bNODE].m_bParentROT]);

	if(pPivot)
		vRotation *= pPivot[m_pTREE[bNODE].m_bINDEX].m_vRot;
	vRotation *= vParent;

	D3DXQuaternionNormalize(
		&vRotation,
		&vRotation);

	D3DXVECTOR3 vPosition = CalcPositionVector(
		&m_pNode[bNODE],
		fTimeValue);

	D3DXVECTOR3 vScale = CalcScale(
		&m_pNode[bNODE],
		fTimeValue);

	D3DXVECTOR3 vCenter(
		m_pBones[m_pTREE[bNODE].m_bINDEX]( 3, 0),
		m_pBones[m_pTREE[bNODE].m_bINDEX]( 3, 1),
		m_pBones[m_pTREE[bNODE].m_bINDEX]( 3, 2));
	D3DXVECTOR3 vMove = vCenter;

	vLocal = pResult[m_pTREE[bNODE].m_bParentPOS];
	CTMath::Transform(
		&vLocal,
		&vMove);

	vLocal = pResult[m_pTREE[bNODE].m_bParentROT];
	vLocal._41 = 0.0f;
	vLocal._42 = 0.0f;
	vLocal._43 = 0.0f;

	CTMath::Transform(
		&vLocal,
		&vPosition);
	vPosition += vMove - vCenter;

	D3DXMatrixTransformation(
		&vLocal,
		&vCenter,
		&D3DXQUATERNION( 0.0f, 0.0f, 0.0f, 1.0f),
		&vScale,
		&vCenter,
		&vRotation,
		&vPosition);

	pResult[m_pTREE[bNODE].m_bINDEX + 1] = vLocal;
}

BYTE CTachyonAnimation::IsValidNode( LPBONESNODE pNode,
									 LPBYTE pFILTER)
{
	return pFILTER[pNode->m_bParentPOS] || pFILTER[pNode->m_bParentROT] ? TRUE : FALSE;
}

D3DXVECTOR3 CTachyonAnimation::CalcPositionVector( LPBONESANIMATION pNode,
												   FLOAT fTimeValue)
{
	D3DXVECTOR3 vRESULT( 0.0f, 0.0f, 0.0f);

	if( pNode->m_nPositionKeyCount > 0 )
	{
		LPTPOINTKEY pKey = (LPTPOINTKEY) pNode->m_pPositionKey;

		if( INT(pKey[pNode->m_nPositionKeyCount - 1].m_dwTime) < fTimeValue )
			fTimeValue = FLOAT(INT(pKey[pNode->m_nPositionKeyCount - 1].m_dwTime));
		vRESULT += pKey[0].m_vKeyPoint;

		for( int i=1; i<pNode->m_nPositionKeyCount; i++)
			if( INT(pKey[i].m_dwTime) >= fTimeValue )
			{
				FLOAT fInterval = (FLOAT) (INT(pKey[i].m_dwTime) - INT(pKey[i - 1].m_dwTime));
				FLOAT fTime = (fTimeValue - ((FLOAT) INT(pKey[i - 1].m_dwTime))) / fInterval;

				vRESULT += fTime * pKey[i].m_vKeyPoint;
				break;
			}
			else
				vRESULT += pKey[i].m_vKeyPoint;
	}

	return vRESULT;
}

D3DXQUATERNION CTachyonAnimation::CalcRotation( LPBONESANIMATION pNode,
											    FLOAT fTimeValue)
{
	D3DXQUATERNION vRESULT( 0.0f, 0.0f, 0.0f, 1.0f);

	if( pNode->m_nRotationKeyCount > 0 )
	{
		LPTROTKEY pKey = (LPTROTKEY) pNode->m_pRotationKey;

		if( INT(pKey[pNode->m_nRotationKeyCount - 1].m_dwTime) < fTimeValue )
			fTimeValue = FLOAT(INT(pKey[pNode->m_nRotationKeyCount - 1].m_dwTime));

		for( int i=1; i<pNode->m_nRotationKeyCount; i++)
			if( INT(pKey[i].m_dwTime) >= fTimeValue )
			{
				FLOAT fInterval = (FLOAT) (INT(pKey[i].m_dwTime) - INT(pKey[i - 1].m_dwTime));
				FLOAT fTime = (fTimeValue - ((FLOAT) INT(pKey[i - 1].m_dwTime))) / fInterval;

				D3DXQUATERNION vInitPos;
				D3DXQuaternionInverse(
					&vInitPos,
					&pKey[0].m_vKeyQuat);

				vRESULT = vInitPos * CTMath::MaxSlerp(
					pKey[i - 1].m_vKeyQuat,
					pKey[i].m_vKeyQuat,
					fTime);

				break;
			}
	}

	return vRESULT;
}

D3DXVECTOR3 CTachyonAnimation::CalcScale( LPBONESANIMATION pNode,
										  FLOAT fTimeValue)
{
	D3DXVECTOR3 vRESULT( 1.0f, 1.0f, 1.0f);

	if( pNode->m_nScaleKeyCount > 0 )
	{
		LPTSCALEKEY pKey = (LPTSCALEKEY) pNode->m_pScaleKey;

		if( INT(pKey[pNode->m_nScaleKeyCount - 1].m_dwTime) < fTimeValue )
			fTimeValue = FLOAT(INT(pKey[pNode->m_nScaleKeyCount - 1].m_dwTime));

		for( int i=1; i<pNode->m_nScaleKeyCount; i++)
			if( INT(pKey[i].m_dwTime) >= fTimeValue )
			{
				FLOAT fInterval = (FLOAT) (INT(pKey[i].m_dwTime) - INT(pKey[i - 1].m_dwTime));
				FLOAT fTime = (fTimeValue - ((FLOAT) INT(pKey[i - 1].m_dwTime))) / fInterval;

				D3DXVec3Lerp(
					&vRESULT,
					&pKey[i - 1].m_vKeyScale,
					&pKey[i].m_vKeyScale,
					fTime);

				break;
			}
	}

	return vRESULT;
}

void CTachyonAnimation::InitPositionKey( LPBONESANIMATION pNode)
{
	if( pNode->m_nPositionKeyCount <= 0 )
		return;
	LPTPOINTKEY pKey = (LPTPOINTKEY) pNode->m_pPositionKey;

	for( int i=pNode->m_nPositionKeyCount - 1; i>0; i--)
		pKey[i].m_vKeyPoint -= pKey[i - 1].m_vKeyPoint;
	pKey[0].m_vKeyPoint = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
}

void CTachyonAnimation::InitRotationKey( LPBONESANIMATION pNode)
{
	if( pNode->m_nRotationKeyCount <= 0 )
		return;
	LPTROTKEY pKey = (LPTROTKEY) pNode->m_pRotationKey;

	for( int i=1; i<pNode->m_nRotationKeyCount; i++)
		pKey[i].m_vKeyQuat = pKey[i - 1].m_vKeyQuat * pKey[i].m_vKeyQuat;
}

void CTachyonAnimation::InitScaleKey( LPBONESANIMATION pNode)
{
}

DWORD CTachyonAnimation::GetInterval()
{
	return m_dwEndTick - m_dwStartTick;
}

BOOL CTachyonAnimation::IsEnable()
{
	return m_pNode && m_pBones;
}

BOOL CTachyonAnimation::GetSize( D3DXVECTOR3& vMin, D3DXVECTOR3& vMax)
{
	BOOL bInit = FALSE;

	for( DWORD i=0; i<m_dwNodeCount; i++)
		if(m_pDATA[i].m_pVB)
		{
			LPD3DXVECTOR3 pDATA = (LPD3DXVECTOR3) m_pDATA[i].m_pVB;

			for( DWORD j=0; j<3 * m_pDATA[i].m_dwCount; j++)
			{
				D3DXVECTOR3 vPoint = pDATA[j];

				CTMath::Transform(
					&m_pBones[i],
					&vPoint);

				if(bInit)
				{
					if( vMax.x < vPoint.x )
						vMax.x = vPoint.x;

					if( vMax.y < vPoint.y )
						vMax.y = vPoint.y;

					if( vMax.z < vPoint.z )
						vMax.z = vPoint.z;

					if( vMin.x > vPoint.x )
						vMin.x = vPoint.x;

					if( vMin.y > vPoint.y )
						vMin.y = vPoint.y;

					if( vMin.z > vPoint.z )
						vMin.z = vPoint.z;
				}
				else
				{
					vMax = vPoint;
					vMin = vPoint;

					bInit = TRUE;
				}
			}
		}

	return bInit;
}

int CTachyonAnimation::HitTest( LPD3DXMATRIX pBones,
							    CD3DCamera *pCam,
								int nPosX,
								int nPosY)
{
	D3DXVECTOR3 vPos = pCam->GetCameraPosition( nPosX, nPosY);
	D3DXVECTOR3 vDir = pCam->GetRayDirection( nPosX, nPosY);
	FLOAT fDist = 0.0f;
	int nResult = -1;

	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;

	if(!GetSize( vMin, vMax))
		return nResult;

	D3DXVECTOR3 vCenter = (vMin + vMax) / 2.0f;
	FLOAT fRadius = D3DXVec3Length(&(vMax - vMin));

	CTMath::Transform(
		&pBones[0],
		&vCenter);

	if(!D3DXSphereBoundProbe( &vCenter, fRadius / 2.0f, &vPos, &vDir))
		return nResult;

	for( int i=0; i<m_dwNodeCount; i++)
		if( m_pDATA[i].m_dwCount > 0 )
		{
			LPD3DXVECTOR3 pDATA = new D3DXVECTOR3[3 * m_pDATA[i].m_dwCount];

			memcpy( pDATA, m_pDATA[i].m_pVB, 3 * m_pDATA[i].m_dwCount * sizeof(D3DXVECTOR3));
			for( int j=0; j<m_pDATA[i].m_dwCount; j++)
			{
				FLOAT fCurDist = 0.0f;
				FLOAT fU;
				FLOAT fV;

				for( int k=0; k<3; k++)
				{
					CTMath::Transform(
						&pBones[i + 1],
						&pDATA[3 * j + k]);
				}

				if( D3DXIntersectTri(
					&pDATA[3 * j],
					&pDATA[3 * j + 1],
					&pDATA[3 * j + 2],
					&vPos, &vDir,
					&fU, &fV,
					&fCurDist) && (
					nResult == -1 ||
					fCurDist < fDist))
				{
					fDist = fCurDist;
					nResult = i;
				}
			}

			delete[] pDATA;
		}

	return nResult;
}
