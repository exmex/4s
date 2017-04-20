#include "StdAfx.h"


CTClientArrow::CTClientArrow()
{
	m_bHostType = OT_NONE;
	m_dwHostID = 0;
	m_pTARGET = NULL;

	m_dwTotalTick = 0;
	m_dwTick = 0;

	m_fVelocityX = 0.0f;
	m_fGravity = 0.0f;
	m_fStartH = 0.0f;

	m_vTACCEL = D3DXVECTOR2( 0.0f, 0.0f);
	m_bAccel = FALSE;
	m_vTARROWSFX.clear();
}

CTClientArrow::~CTClientArrow()
{
	if(m_pTARGET)
	{
		delete m_pTARGET;
		m_pTARGET = NULL;
	}

	RemoveTArrowSFX();
}

void CTClientArrow::Release()
{
	if(m_pTARGET)
	{
		delete m_pTARGET;
		m_pTARGET = NULL;
	}
	RemoveTArrowSFX();

	CTachyonObject::Release();
}

void CTClientArrow::CalcTick( LPDIRECT3DDEVICE9 pDevice,
							  DWORD dwTick)
{
	m_vTSLASH.CalcTick(dwTick);
	CTachyonObject::CalcTick( pDevice, dwTick);
}

void CTClientArrow::PlayTArrowSFX( DWORD dwFuncID)
{
	LPTSFXTEMP pTEMP = CTChart::FindTSFXTEMP( dwFuncID );

	if(!pTEMP)
		return;

	CTClientSFX *pTSFX = new CTClientSFX();
	LPOBJSFX pItem = new OBJSFX();

	pItem->m_pSFX = new SFXINST();
	pItem->m_bDeleteINST = TRUE;

	pItem->m_pSFX->m_bDeleteOnStop = FALSE;
	pItem->m_pSFX->m_dwFuncID = dwFuncID;
	pItem->m_pSFX->m_nPivotID = 0;
	pItem->m_pSFX->m_dwTick = 0;
	pItem->m_pSFX->m_bBias = 0;

	pItem->m_pSFX->m_vPOS = D3DXVECTOR3( 0, 0, 0);
	pItem->m_pSFX->m_vROT = D3DXVECTOR3( 0, 0, 0);
	pItem->m_pSFX->m_pSFX = pTEMP->m_pTSFX;

	pTSFX->m_bHasHost = TRUE;
	pTSFX->m_pHost = m_pHost;
	pTSFX->m_dwInitTick = 0;

	pItem->m_dwID = CTachyonSFX::RegSFX(
		pTSFX,
		pItem->m_pSFX->m_bBias);

	m_vTARROWSFX.push_back(pItem);
}

void CTClientArrow::RemoveTArrowSFX()
{
	for( int i=0; i<INT(m_vTARROWSFX.size()); i++)
	{
		CTachyonSFX::Release(m_vTARROWSFX[i]->m_dwID);
		delete m_vTARROWSFX[i];
	}

	m_vTARROWSFX.clear();
}

void CTClientArrow::Render( CD3DDevice *pDevice,
						    CD3DCamera *pCamera)
{
	CTachyonObject::Render(
		pDevice,
		pCamera);
	CalcSFX( pDevice, &m_vTARROWSFX);
}
