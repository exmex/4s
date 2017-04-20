#include "StdAfx.h"

LPDIRECT3DTEXTURE9 CTMinimapDlg::m_pTMINIMAP = NULL;
LPDIRECT3DTEXTURE9 CTMinimapDlg::m_pTMASK = NULL;


CTMinimapDlg::CTMinimapDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
:TFrame( pParent, pDesc)
{
	static DWORD dwCompID[] = {
		ID_CTRLINST_MAX,
		ID_CTRLINST_WORLD,
		ID_CTRLINST_MINIMAP,
		ID_CTRLINST_ZOOM};

	for( BYTE i=0; i<TMINIMAPCOMP_COUNT; i++)
		m_vTCOMP[i] = FindKid(dwCompID[i]);

	m_pTITLE = FindKid(ID_CTRLINST_MAX_TITLE);
	m_pMINMAXBTN = FindKid(ID_CTRLINST_MAX_BTN);
	m_pTOPFRAME = FindKid(ID_CTRLINST_MIN);

	m_bMinimize = FALSE;
	m_bSND = FALSE;

	m_pDevice = NULL;
	m_pHost = NULL;
	m_pTMAP = NULL;
	m_pCAM = NULL;

	TComponent *pTAREA = FindKid(ID_CTRLINST_AREA);
	pTAREA->GetComponentRect(&m_rcAREA);

	RemoveKid(pTAREA);
	delete pTAREA;

	m_pTZOOMSCROLL = (TScroll*) m_vTCOMP[TMINIMAPCOMP_ZOOM];
	
	if( m_pTZOOMSCROLL )
		m_pTZOOMSCROLL->SetScrollPos((INT)TMINIMAP_SCALE_COUNT, (INT)TMINIMAP_SCALE_COUNT);
}

CTMinimapDlg::~CTMinimapDlg()
{
}

void CTMinimapDlg::ResetVisible()
{
	for( BYTE i=0; i<TMINIMAPCOMP_COUNT; i++)
	{
		if( m_vTCOMP[i])
		{
			m_vTCOMP[i]->EnableComponent(!m_bMinimize);
			m_vTCOMP[i]->ShowComponent(!m_bMinimize);
		}
	}
}

void CTMinimapDlg::ShowComponent( BOOL bVisible)
{
	TFrame::ShowComponent(bVisible);

	if(m_bVisible)
		ResetVisible();
}

void CTMinimapDlg::RenderTMINIMAP()
{
	int nRng;
	int nPos;

	if( m_pTZOOMSCROLL )
	{
		m_pTZOOMSCROLL->GetScrollPos(nRng, nPos);
        m_pTMAP->m_fTSCALE = TMINIMAP_INIT_SCALE / powf(TMINIMAP_SCALE_FACTOR, (FLOAT)nPos);
	}

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

	m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &fMIP));
	m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pDevice->m_pDevice->SetTexture( 0, m_pTMINIMAP);
	m_pDevice->m_pDevice->SetFVF(T3DFVF_TNLVERTEX);

	m_pDevice->m_pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2, vRECT, sizeof(TNLVERTEX));
}

HRESULT CTMinimapDlg::Render( DWORD dwTickCount)
{
	if( m_pTMAP && m_pTMAP->m_pMAP && m_bVisible && !m_bMinimize )
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
