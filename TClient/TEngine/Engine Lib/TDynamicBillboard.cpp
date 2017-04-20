#include "stdafx.h"


LPDIRECT3DSURFACE9 CTDynamicBillboard::m_pZBUF = NULL;

BYTE CTDynamicBillboard::m_bLevel = TDBB_LEVEL_HI;
FLOAT CTDynamicBillboard::m_fMinDIST = 7.0f;

MAPTDBB CTDynamicBillboard::m_mapTACTIVE;
VTDBB CTDynamicBillboard::m_vTUPDATE;
VTDBB CTDynamicBillboard::m_vTRENDER;


CTDynamicBillboard::CTDynamicBillboard()
{
	m_dwTick = TDBB_UPDATE_TICK;
	m_pTOBJ = NULL;
	m_pTEX = NULL;

	m_fSIZE = 0.0f;
	m_nTEX = 0;
}

CTDynamicBillboard::~CTDynamicBillboard()
{
	Release();
}

BYTE CTDynamicBillboard::InitTDBB( CD3DDevice *pDevice)
{
	if(!pDevice)
		return FALSE;
	ReleaseTDBB();

	pDevice->m_pDevice->CreateDepthStencilSurface(
		TDBB_MAX_PIXEL,
		TDBB_MAX_PIXEL,
		pDevice->m_vPRESENT.AutoDepthStencilFormat,
		D3DMULTISAMPLE_NONE,
		0, FALSE,
		&m_pZBUF,
		NULL);

	if(!m_pZBUF)
		return FALSE;

	return TRUE;
}

void CTDynamicBillboard::ReleaseTDBB()
{
	if(m_pZBUF)
	{
		m_pZBUF->Release();
		m_pZBUF = NULL;
	}

	ClearBUF();
}

BYTE CTDynamicBillboard::CheckInit()
{
	return m_pZBUF ? TRUE : FALSE;
}

BYTE CTDynamicBillboard::Restore( CD3DDevice *pDevice)
{
	MAPTDBB::iterator it;

	if(!InitTDBB(pDevice))
		return FALSE;

	for( it = m_mapTACTIVE.begin(); it != m_mapTACTIVE.end(); it++)
	{
		CTDynamicBillboard *pTDBB = (*it).second;

		if(pTDBB)
		{
			if(pTDBB->m_pTEX)
			{
				pTDBB->m_pTEX->Release();
				pTDBB->m_pTEX = NULL;
			}

			D3DXCreateTexture(
				pDevice->m_pDevice,
				pTDBB->m_nTEX,
				pTDBB->m_nTEX, 1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT,
				&pTDBB->m_pTEX);

			pTDBB->ResetTick();
		}
	}

	return TRUE;
}

void CTDynamicBillboard::DeleteForRestore()
{
	MAPTDBB::iterator it;

	for( it = m_mapTACTIVE.begin(); it != m_mapTACTIVE.end(); it++)
	{
		CTDynamicBillboard *pTDBB = (*it).second;

		if( pTDBB && pTDBB->m_pTEX )
		{
			pTDBB->m_pTEX->Release();
			pTDBB->m_pTEX = NULL;
		}
	}

	ReleaseTDBB();
}

void CTDynamicBillboard::ClearTACTIVE()
{
	m_mapTACTIVE.clear();
}

void CTDynamicBillboard::ClearBUF()
{
	m_vTUPDATE.clear();
	m_vTRENDER.clear();
}

DWORD CTDynamicBillboard::GetTDBBCount()
{
	return DWORD(m_vTRENDER.size());
}

BYTE CTDynamicBillboard::CanUSE( CD3DDevice *pDevice,
								 CD3DCamera *pCamera,
								 LPD3DXVECTOR3 pTPOS,
								 FLOAT fRadius,
								 FLOAT fHeight)
{
	static FLOAT vLEVEL[TDBB_LEVEL_COUNT] = { 0.8f, 1.0f, 1.2f};

	if( !pDevice || !pCamera || !pTPOS || fRadius == 0.0f || fHeight == 0.0f )
		return FALSE;

	FLOAT fDIST = D3DXVec3Length(&(pCamera->m_vPosition - (*pTPOS)));
	if( fDIST < m_fMinDIST )
		return FALSE;

	int nSIZE = CalcPixel(
		pDevice,
		pCamera,
		fDIST,
		fRadius,
		fHeight);

	if( nSIZE > TDBB_MAX_PIXEL * vLEVEL[m_bLevel] )
		return FALSE;

	return TRUE;
}

int CTDynamicBillboard::CalcPixel( CD3DDevice *pDevice,
								   CD3DCamera *pCamera,
								   FLOAT fDist,
								   FLOAT fRadius,
								   FLOAT fHeight)
{
	FLOAT fSIZE = CalcSIZE(
		fRadius,
		fHeight);

	return (int) (fSIZE * FLOAT(pDevice->m_option.m_dwScreenY) / (2.0f * fDist * tanf(pCamera->m_fFOV / 2.0f)));
}

FLOAT CTDynamicBillboard::CalcSIZE( FLOAT fRadius,
								    FLOAT fHeight)
{
	fRadius *= 2.0f;
	return max( fHeight, fRadius) + 1.0f;
}

D3DXVECTOR3 CTDynamicBillboard::GetTOBJPOS()
{
	return m_pTOBJ ? D3DXVECTOR3(
		m_pTOBJ->m_vPosition._41,
		m_pTOBJ->m_vPosition._42 + m_fSIZE / 2.0f,
		m_pTOBJ->m_vPosition._43) : D3DXVECTOR3(
		0.0f, m_fSIZE / 2.0f, 0.0f);
}

void CTDynamicBillboard::CalcTick( DWORD dwTick)
{
	if(!m_pTOBJ)
		return;

	m_vTRENDER.push_back(this);
	m_dwTick += dwTick;

	if( m_dwTick > TDBB_UPDATE_TICK )
	{
		m_dwTick %= TDBB_UPDATE_TICK;
		m_vTUPDATE.push_back(this);
	}
}

void CTDynamicBillboard::UpdateBUF( CD3DDevice *pDevice,
								    CD3DCamera *pCamera)
{
	if(m_vTUPDATE.empty())
		return;

	if( !pDevice || !pCamera || !m_pZBUF )
	{
		m_vTUPDATE.clear();
		return;
	}

	LPDIRECT3DSURFACE9 pRTARGET = NULL;
	LPDIRECT3DSURFACE9 pZBUF = NULL;
	D3DVIEWPORT9 vVIEW;

	pDevice->m_pDevice->GetRenderTarget( 0, &pRTARGET);
	pDevice->m_pDevice->GetDepthStencilSurface(&pZBUF);
	pDevice->m_pDevice->GetViewport(&vVIEW);

	pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);
	pDevice->m_pDevice->SetDepthStencilSurface(m_pZBUF);
	CTachyonMesh::BeginGlobalDraw(pDevice->m_pDevice);

	for( int i=0; i<INT(m_vTUPDATE.size()); i++)
	{
		D3DXVECTOR3 vPOS = m_vTUPDATE[i]->GetTOBJPOS();
		D3DXVECTOR3 vDIR(
			pCamera->m_vPosition.x - vPOS.x,
			0.0f,
			pCamera->m_vPosition.z - vPOS.z);
		FLOAT fDIST = D3DXVec3Length(&vDIR);

		if( m_vTUPDATE[i]->m_pTOBJ && m_vTUPDATE[i]->m_pTEX && fDIST != 0.0f )
		{
			LPDIRECT3DSURFACE9 pTEX = NULL;

			m_vTUPDATE[i]->m_pTEX->GetSurfaceLevel( 0, &pTEX);
			if(pTEX)
			{
				D3DVIEWPORT9 vNEW = {
					0, 0,
					m_vTUPDATE[i]->m_nTEX,
					m_vTUPDATE[i]->m_nTEX,
					0.0f, 1.0f};
				CD3DCamera vTCAM;

				pDevice->m_pDevice->SetRenderTarget( 0, pTEX);
				pDevice->m_pDevice->SetViewport(&vNEW);
				pDevice->m_pDevice->Clear(
					0, NULL,
					D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
					D3DCOLOR_ARGB( 0, 0, 0, 0),
					1.0f, 0);

				vTCAM.InitOrthoCamera(
					pDevice->m_pDevice,
					0.01f,
					fDIST + m_vTUPDATE[i]->m_fSIZE,
					m_vTUPDATE[i]->m_fSIZE,
					m_vTUPDATE[i]->m_fSIZE);

				vTCAM.SetPosition(
					vPOS + vDIR,
					vPOS,
					-pCamera->m_vUp,
					FALSE);
				vTCAM.Activate(TRUE);

				m_vTUPDATE[i]->m_pTOBJ->Render(
					pDevice,
					pCamera);
				pTEX->Release();
			}
		}
	}

	pDevice->m_pDevice->SetRenderTarget( 0, pRTARGET);
	pDevice->m_pDevice->SetDepthStencilSurface(pZBUF);
	pDevice->m_pDevice->SetViewport(&vVIEW);

	CTachyonMesh::EndGlobalDraw(pDevice->m_pDevice);
	pCamera->Activate(TRUE);

	if(pRTARGET)
		pRTARGET->Release();

	if(pZBUF)
		pZBUF->Release();

	m_vTUPDATE.clear();
}

void CTDynamicBillboard::RenderBUF( CD3DDevice *pDevice,
								    CD3DCamera *pCamera)
{
	if(m_vTRENDER.empty())
		return;

	if( !pDevice || !pCamera )
	{
		m_vTRENDER.clear();
		return;
	}

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000040);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	pDevice->m_pDevice->SetStreamSource( 0, pDevice->m_pBACKVB, 0, sizeof(PVERTEX));
	pDevice->m_pDevice->SetFVF(T3DFVF_PVERTEX);

	CTachyonMesh::BeginGlobalDraw(pDevice->m_pDevice);
	for( int i=0; i<INT(m_vTRENDER.size()); i++)
	{
		D3DXVECTOR3 vPOS = m_vTRENDER[i]->GetTOBJPOS();
		D3DXVECTOR2 vDIR(
			pCamera->m_vPosition.x - vPOS.x,
			pCamera->m_vPosition.z - vPOS.z);

		if( m_vTRENDER[i]->m_pTOBJ && m_vTRENDER[i]->m_pTEX && vDIR.x != 0.0f && vDIR.y != 0.0f )
		{
			D3DXMATRIX vWORLD;
			D3DXMATRIX vROT;
			FLOAT fTHETA;

			D3DXMatrixScaling(
				&vWORLD,
				m_vTRENDER[i]->m_fSIZE,
				m_vTRENDER[i]->m_fSIZE,
				m_vTRENDER[i]->m_fSIZE);

			D3DXMatrixRotationX(
				&vROT,
				D3DX_PI / 2.0f);
			vWORLD *= vROT;

			if( fabs(vDIR.y) > 0.001f )
				if( vDIR.y < 0.0f )
					fTHETA = D3DX_PI + atanf(vDIR.x / vDIR.y);
				else
					fTHETA = atanf(vDIR.x / vDIR.y);
			else if( vDIR.x < 0.0f )
				fTHETA = -D3DX_PI / 2.0f;
			else
				fTHETA = D3DX_PI / 2.0f;

			D3DXMatrixRotationY(
				&vROT,
				fTHETA);
			vWORLD *= vROT;

			vWORLD._41 = vPOS.x;
			vWORLD._42 = vPOS.y;
			vWORLD._43 = vPOS.z;

			pDevice->m_pDevice->SetTexture( 0, m_vTRENDER[i]->m_pTEX);
			pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);

			pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}

	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	CTachyonMesh::EndGlobalDraw(pDevice->m_pDevice);
	m_vTRENDER.clear();
}

BYTE CTDynamicBillboard::CreateDynamicBillboard( CD3DDevice *pDevice,
												 CD3DCamera *pCamera,
												 CTachyonObject *pTOBJ,
												 FLOAT fRadius,
												 FLOAT fHeight)
{
	int vTDBBSIZE[TDBB_SIZE_COUNT] = {
		32,
		TDBB_MAX_PIXEL};

	if( !pDevice || !pCamera || !pTOBJ || fRadius == 0.0f || fHeight == 0.0f )
		return FALSE;
	Release();

	m_nTEX = CalcPixel(
		pDevice,
		pCamera,
		m_fMinDIST,
		fRadius,
		fHeight);

	m_fSIZE = CalcSIZE(
		fRadius,
		fHeight);

	m_dwTick = TDBB_UPDATE_TICK;
	m_pTOBJ = pTOBJ;

	for( BYTE i=0; i<TDBB_SIZE_COUNT; i++)
		if( m_nTEX <= vTDBBSIZE[i] )
		{
			m_nTEX = vTDBBSIZE[i];
			break;
		}

	if( m_nTEX > TDBB_MAX_PIXEL )
		m_nTEX = TDBB_MAX_PIXEL;

	D3DXCreateTexture(
		pDevice->m_pDevice,
		m_nTEX,
		m_nTEX, 1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTEX);

	if(!m_pTEX)
	{
		Release();
		return FALSE;
	}
	m_mapTACTIVE.insert( MAPTDBB::value_type( this, this));

	return TRUE;
}

void CTDynamicBillboard::Release()
{
	MAPTDBB::iterator finder = m_mapTACTIVE.find(this);

	if( finder != m_mapTACTIVE.end() )
		m_mapTACTIVE.erase(finder);

	if(m_pTEX)
	{
		m_pTEX->Release();
		m_pTEX = NULL;
	}

	m_dwTick = TDBB_UPDATE_TICK;
	m_pTOBJ = NULL;

	m_fSIZE = 0.0f;
	m_nTEX = 0;
}

void CTDynamicBillboard::ResetTick()
{
	m_dwTick = TDBB_UPDATE_TICK;
}
