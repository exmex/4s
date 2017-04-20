#include "StdAfx.h"
#include ".\tmeshtemp.h"

CTMeshTemp::CTMeshTemp(void)
{
	m_nTMFVersion	= 0;
	m_dwNodeCount	= 0;
	m_fRadius		= 0.0f;
	m_vCenterPoint	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_pBones		= NULL;
	m_dwMeshCount	= 0;
	m_pMESH			= NULL;
	m_pINDEX		= NULL;
}

CTMeshTemp::~CTMeshTemp(void)
{
	Release();
}

void CTMeshTemp::Release()
{
	/*if(m_pBones->size())
	{
		for(int i=0; i<m_pBones->size(); i++)
			delete m_pBones->at(i);

		m_pBones->clear();
	}

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

	if(m_pINDEX)
	{
		for( int i=0; i<m_dwMeshCount; i++)
			while(!m_pINDEX->m_pIB[i].empty())
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
	}*/

	m_nTMFVersion	= 0;
	m_dwNodeCount	= 0;
	m_fRadius		= 0.0f;
	m_vCenterPoint	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_pBones		= NULL;
	m_dwMeshCount	= 0;
	m_pMESH			= NULL;
	m_pINDEX		= NULL;
}
