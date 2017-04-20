#include "StdAfx.h"
#include "Resource.h"


CTWorldmapDlg::CTWorldmapDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
:TFrame( pParent, pDesc)
{
	TComponent *pTCOMP = FindKid(ID_CTRLINST_DEAD);
	RemoveKid(pTCOMP);
	delete pTCOMP;

	m_pTWORLDMAP = (TImageList *) FindKid(ID_CTRLINST_MAP);
	RemoveKid(m_pTWORLDMAP);

	m_pTITLE = (TImageList *) FindKid(ID_CTRLINST_TITLE);
	m_pTCHAR = (TImageList *) FindKid(ID_CTRLINST_PC);
	RemoveKid(m_pTCHAR);

	m_vTCENTER = D3DXVECTOR2( 0.0f, 0.0f);
	m_fTSCALE = 1.0f;

	m_pDevice = NULL;
	m_pHost = NULL;
	m_pTMAP = NULL;
	m_pCAM = NULL;
	m_bMOVE = FALSE;
}

CTWorldmapDlg::~CTWorldmapDlg()
{
	if(m_pTWORLDMAP)
	{
		delete m_pTWORLDMAP;
		m_pTWORLDMAP = NULL;
	}

	if(m_pTCHAR)
	{
		delete m_pTCHAR;
		m_pTCHAR = NULL;
	}

	if(m_pTEXT)
	{
		delete m_pTEXT;
		m_pTEXT = NULL;
	}
}

BOOL CTWorldmapDlg::HitTest( CPoint pt)
{
	return IsVisible() ? TRUE : FALSE;
}

CPoint CTWorldmapDlg::GetPosition( FLOAT fPosX,
								   FLOAT fPosZ,
								   int nWidth,
								   int nHeight)
{
	FLOAT fSCALE = ((FLOAT) m_pTMAP->m_pMAP->m_nUnitLength * m_pTMAP->m_pMAP->m_nTileLength) / m_fTSCALE;
	CPoint vRESULT(
		INT(m_pDevice->m_option.m_dwScreenX / 2.0f),
		INT(m_pDevice->m_option.m_dwScreenY / 2.0f));

	fPosX -= m_vTCENTER.x;
	fPosZ -= m_vTCENTER.y;

	vRESULT.x += INT(fPosX * FLOAT(TWORLDMAP_SIZE) / fSCALE);
	vRESULT.y -= INT(fPosZ * FLOAT(TWORLDMAP_SIZE) / fSCALE);

	vRESULT.y -= nHeight / 2;
	vRESULT.x -= nWidth / 2;

	return vRESULT;
}

void CTWorldmapDlg::OnLButtonDown( UINT nFlags, CPoint pt)
{
	TFrame::OnLButtonDown( nFlags, pt);

	if( IsVisible() && !TFrame::HitTest(pt) )
	{
		CPoint point(
			m_pDevice->m_option.m_dwScreenX / 2,
			m_pDevice->m_option.m_dwScreenY / 2);

		m_pCommandHandler->ClientToScreen(&point);
		ShowCursor(FALSE);
		SetCursorPos(
			point.x,
			point.y);

		m_bMOVE = TRUE;
	}
}

void CTWorldmapDlg::OnMouseMove( UINT nFlags, CPoint pt)
{
	if( IsVisible() && m_bMOVE )
	{
		FLOAT fLength = ((FLOAT) m_pTMAP->m_pMAP->m_nUnitLength * m_pTMAP->m_pMAP->m_nTileLength);
		FLOAT fMinX = fLength / (2.0f * m_fTSCALE);
		FLOAT fMaxX = fLength - fMinX;
		FLOAT fMinZ = fMinX * FLOAT(m_pDevice->m_option.m_dwScreenY) / FLOAT(m_pDevice->m_option.m_dwScreenX);
		FLOAT fMaxZ = fLength - fMinZ;

		int nUnitX = INT(m_vTCENTER.x / fLength);
		int nUnitZ = INT(m_vTCENTER.y / fLength);

		fMinX += FLOAT(nUnitX) * fLength;
		fMaxX += FLOAT(nUnitX) * fLength;
		fMinZ += FLOAT(nUnitZ) * fLength;
		fMaxZ += FLOAT(nUnitZ) * fLength;

		CPoint point(
			m_pDevice->m_option.m_dwScreenX / 2,
			m_pDevice->m_option.m_dwScreenY / 2);

		m_pCommandHandler->ClientToScreen(&point);
		SetCursorPos(
			point.x,
			point.y);
		m_pCommandHandler->ScreenToClient(&point);
		point -= pt;

		m_vTCENTER.x += fLength * FLOAT(point.x) / (m_fTSCALE * FLOAT(TWORLDMAP_SIZE));
		m_vTCENTER.y -= fLength * FLOAT(point.y) / (m_fTSCALE * FLOAT(TWORLDMAP_SIZE));

		m_vTCENTER.x = min( max( m_vTCENTER.x, fMinX), fMaxX);
		m_vTCENTER.y = min( max( m_vTCENTER.y, fMinZ), fMaxZ);
	}

	TFrame::OnMouseMove( nFlags, pt);
}

void CTWorldmapDlg::OnLButtonUp( UINT nFlags, CPoint pt)
{
	if( IsVisible() && m_bMOVE )
	{
		CPoint point(
			m_pDevice->m_option.m_dwScreenX / 2,
			m_pDevice->m_option.m_dwScreenY / 2);

		m_pCommandHandler->ClientToScreen(&point);
		SetCursorPos(
			point.x,
			point.y);
		ShowCursor(TRUE);

		m_bMOVE = FALSE;
	}

	TFrame::OnLButtonUp( nFlags, pt);
}

void CTWorldmapDlg::RenderWORLDMAP( DWORD dwTickCount)
{
	FLOAT fLength = ((FLOAT) m_pTMAP->m_pMAP->m_nUnitLength * m_pTMAP->m_pMAP->m_nTileLength);
	FLOAT fMinX = fLength / (2.0f * m_fTSCALE);
	FLOAT fMaxX = fLength - fMinX;
	FLOAT fMinZ = fMinX * FLOAT(m_pDevice->m_option.m_dwScreenY) / FLOAT(m_pDevice->m_option.m_dwScreenX);
	FLOAT fMaxZ = fLength - fMinZ;
	BYTE bResult = FALSE;

	int nUnitX = INT(m_vTCENTER.x / fLength);
	int nUnitZ = INT(m_vTCENTER.y / fLength);

	fMinX += FLOAT(nUnitX) * fLength;
	fMaxX += FLOAT(nUnitX) * fLength;
	fMinZ += FLOAT(nUnitZ) * fLength;
	fMaxZ += FLOAT(nUnitZ) * fLength;

	m_vTCENTER.x = min( max( m_vTCENTER.x, fMinX), fMaxX);
	m_vTCENTER.y = min( max( m_vTCENTER.y, fMinZ), fMaxZ);

	MAPTMINIMAP::iterator finder = CTMapViewerMAP::m_mapTMINIMAP.find(MAKELONG( MAKEWORD(
		nUnitX,
		nUnitZ), m_pTMAP->m_wMapID));

	if( finder != CTMapViewerMAP::m_mapTMINIMAP.end() )
	{
		int nPosX = INT((FLOAT(nUnitX) * fLength + fLength / 2.0f - m_vTCENTER.x) * TWORLDMAP_SIZE / fLength) + INT(m_pDevice->m_option.m_dwScreenX - TWORLDMAP_SIZE) / 2;
		int nPosZ = INT((FLOAT(nUnitZ) * fLength + fLength / 2.0f - m_vTCENTER.y) * TWORLDMAP_SIZE / fLength) - INT(m_pDevice->m_option.m_dwScreenY - TWORLDMAP_SIZE) / 2;

		CTMapViewerCAM vCAMERA;
		D3DXMATRIX vWorld;

		D3DXMatrixScaling(
			&vWorld,
			m_fTSCALE,
			m_fTSCALE,
			m_fTSCALE);

		vCAMERA.InitOrthoCamera(
			m_pDevice->m_pDevice,
			-1.0f,
			1.0f,
			1.0f,
			1.0f);

		vCAMERA.SetPosition(
			D3DXVECTOR3( 0.0f, 0.5f, 0.0f),
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
			D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
			FALSE);
		vCAMERA.Activate(TRUE);

		m_pTWORLDMAP->SetCurImage((*finder).second->m_bWorldID);
		m_pTITLE->SetCurImage((*finder).second->m_bWorldID);

		m_pTWORLDMAP->MoveComponent(CPoint( nPosX, -nPosZ));
		m_pTWORLDMAP->ShowComponent(TRUE);

		m_pDevice->m_pDevice->SetRenderTarget( 0, m_pDevice->m_pBACKBUF);
		m_pTWORLDMAP->Render(dwTickCount);
		m_pDevice->m_pDevice->SetRenderTarget( 0, m_pDevice->m_pRTARGET);

		m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

		m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

		m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

		m_pDevice->m_pDevice->SetStreamSource( 0, m_pDevice->m_pBACKVB, 0, sizeof(PVERTEX));
		m_pDevice->m_pDevice->SetTexture( 0, m_pDevice->m_pBACKTEX);

		m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vWorld);
		m_pDevice->m_pDevice->SetFVF(T3DFVF_PVERTEX);
		m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
		m_pCAM->Activate(TRUE);
	}
}

void CTWorldmapDlg::RenderOBJ( DWORD dwTickCount)
{
	CRect rect(
		0, 0,
		m_pDevice->m_option.m_dwScreenX,
		m_pDevice->m_option.m_dwScreenY);

	CTMapViewerChar *pTHITCHAR = NULL;

	CSize szTEXT;
	CSize size;
	CPoint pt;

	m_pTEXT->GetComponentSize(&szTEXT);
	m_pTEXT->ShowComponent(TRUE);

	rect.InflateRect(size);

	if(!m_bMOVE)
	{
		GetCursorPos(&pt);
		m_pCommandHandler->ScreenToClient(&pt);
	}

	
	rect.DeflateRect(size);
	m_pTCHAR->GetComponentSize(&size);
	m_pTCHAR->ShowComponent(TRUE);
	rect.InflateRect(size);

	CPoint point = GetPosition(
		m_pHost->m_vPosition._41,
		m_pHost->m_vPosition._43,
		size.cx,
		size.cy);

	if(rect.PtInRect(point))
	{
		m_pTCHAR->MoveComponent(point);
		m_pTCHAR->SetCurImage(0);
		m_pTCHAR->Render(dwTickCount);

		if( !m_bMOVE && m_pTCHAR->HitTest(pt) )
			pTHITCHAR = m_pHost;
	}

	if(pTHITCHAR)
	{
		m_pTCHAR->GetComponentSize(&size);
		point = GetPosition(
			pTHITCHAR->m_vPosition._41,
			pTHITCHAR->m_vPosition._43,
			size.cx,
			size.cy);

		point.x -= (szTEXT.cx - size.cx) / 2;
		point.y -= szTEXT.cy;

		m_pTEXT->m_strText = pTHITCHAR->GetName();
		m_pTEXT->MoveComponent(point);
		m_pTEXT->Render(dwTickCount);
	}
}

HRESULT CTWorldmapDlg::Render( DWORD dwTickCount)
{
	if(IsVisible())
	{
		RenderWORLDMAP(dwTickCount);
		RenderOBJ(dwTickCount);
	}

	return TFrame::Render(dwTickCount);
}
