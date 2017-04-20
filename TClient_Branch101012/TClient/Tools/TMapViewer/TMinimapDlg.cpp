#include "StdAfx.h"

LPDIRECT3DTEXTURE9 CTMinimapDlg::m_pTMINIMAP = NULL;
LPDIRECT3DTEXTURE9 CTMinimapDlg::m_pTMASK = NULL;


CTMinimapDlg::CTMinimapDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
:TFrame( pParent, pDesc)
{
	static DWORD dwCompID[] = {
		ID_CTRLINST_MIN,
		ID_CTRLINST_MAX,
		ID_CTRLINST_MIN_BTN,
		ID_CTRLINST_MAX_BTN,
		ID_CTRLINST_WORLD,
		ID_CTRLINST_CMD,
		ID_CTRLINST_ZOOM_IN,
		ID_CTRLINST_ZOOM_OUT};

	for( BYTE i=0; i<TMINIMAPCOMP_COUNT; i++)
		m_vTCOMP[i] = FindKid(dwCompID[i]);

	m_pDevice = NULL;
	m_pHost = NULL;
	m_pTMAP = NULL;
	m_pCAM = NULL;

	TComponent *pTAREA = FindKid(ID_CTRLINST_AREA);
	pTAREA->GetComponentRect(&m_rcAREA);

	RemoveKid(pTAREA);
	delete pTAREA;
}

CTMinimapDlg::~CTMinimapDlg()
{
}

void CTMinimapDlg::RenderTMINIMAP()
{
	CRect rect(
		0, 0,
		TMINIMAPTEX_SIZE,
		TMINIMAPTEX_SIZE);

	rect.OffsetRect(
		m_rc.left + m_rcAREA.left - (TMINIMAPTEX_SIZE - m_rcAREA.Width()) / 2,
		m_rc.top + m_rcAREA.top - (TMINIMAPTEX_SIZE - m_rcAREA.Height()) / 2);

	TNLVERTEX vRECT[4] = {
		{ FLOAT(rect.left), FLOAT(rect.top), 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f},
		{ FLOAT(rect.right), FLOAT(rect.top), 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f},
		{ FLOAT(rect.left), FLOAT(rect.bottom), 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f},
		{ FLOAT(rect.right), FLOAT(rect.bottom), 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f}};
	FLOAT fMIP = 0.0f;

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &fMIP));
	m_pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &fMIP));
	m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);

	m_pDevice->m_pDevice->SetTexture( 1, m_pTMINIMAP);
	m_pDevice->m_pDevice->SetTexture( 0, m_pTMASK);
	m_pDevice->m_pDevice->SetFVF(T3DFVF_TNLVERTEX);

	m_pDevice->m_pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2, vRECT, sizeof(TNLVERTEX));
}

HRESULT CTMinimapDlg::Render( DWORD dwTickCount)
{
	if(m_pTMAP)
		if(m_pTMAP->m_pMAP)
			if(m_bVisible)
			{
				m_pTMAP->ResetTMINIMAP(
					m_pTMINIMAP,
					m_pHost,
					m_pDevice,
					m_pCAM);
				RenderTMINIMAP();
			}
	return TFrame::Render(dwTickCount);
}
