#include "StdAfx.h"

LPTEXTURESET CTClientTalkBox::m_pSKIN = NULL;
CFont *CTClientTalkBox::m_pFONT = NULL;

CT3DVertex CTClientTalkBox::m_vVB;
CT3DIndex CTClientTalkBox::m_vIB;


CTClientTalkBox::CTClientTalkBox()
{
	D3DXMatrixIdentity(&m_vWORLD);
}

CTClientTalkBox::~CTClientTalkBox()
{
}

void CTClientTalkBox::ReleaseTalkBox()
{
	m_vVB.ReleaseDATA();
	m_vIB.ReleaseDATA();

	m_pSKIN = NULL;
	m_pFONT = NULL;
}

void CTClientTalkBox::InitTalkBox( LPDIRECT3DDEVICE9 pDevice,
								   CTachyonRes *pRES)
{
	static WPVERTEX vMESH[16] = {
		{ -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0, 0.0f, 0.0f},
		{  0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0, 0.5f, 0.0f},
		{ -1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0, 0.0f, 0.5f},
		{  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0, 0.5f, 0.5f},
		{  0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1, 0.5f, 0.0f},
		{  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1, 1.0f, 0.0f},
		{  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1, 0.5f, 0.5f},
		{  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1, 1.0f, 0.5f},
		{ -1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 2, 0.0f, 0.5f},
		{  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 2, 0.5f, 0.5f},
		{ -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 2, 0.0f, 1.0f},
		{  0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 2, 0.5f, 1.0f},
		{  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 3, 0.5f, 0.5f},
		{  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 3, 1.0f, 0.5f},
		{  0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 3, 0.5f, 1.0f},
		{  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 3, 1.0f, 1.0f}};

	static WORD vINDEX[54] = {
		 0,  1,  2,  2,  1,  3,
		 1,  4,  3,  3,  4,  6,
		 4,  5,  6,  6,  5,  7,
		 2,  3,  8,  8,  3,  9,
		 3,  6,  9,  9,  6, 12,
		 6,  7, 12, 12,  7, 13,
		 8,  9, 10, 10,  9, 11,
		 9, 12, 11, 11, 12, 14,
		12, 13, 14, 14, 13, 15};
	MAPRES::iterator finder = pRES->m_mapTEX.find(ID_TALK_SKIN);

	m_vVB.LoadT3DVB(
		16 * sizeof(WPVERTEX),
		0, T3DFVF_WPVERTEX);

	m_vVB.LoadT3DVBDATA(
		(LPBYTE) vMESH,
		16 * sizeof(WPVERTEX), 0);

	m_vIB.LoadT3DIB(
		54 * sizeof(WORD),
		0, D3DFMT_INDEX16);

	m_vIB.LoadT3DIBDATA(
		(LPBYTE) vINDEX,
		54 * sizeof(WORD), 0);

	if( finder != pRES->m_mapTEX.end() )
		m_pSKIN = (LPTEXTURESET) (*finder).second;

	CGDIFont *pGDIFont = FindFont(ID_FONT_LOGIN);
	m_pFONT = pGDIFont->m_pFont;
}

void CTClientTalkBox::SetTALK( LPDIRECT3DDEVICE9 pDevice, CString& strTALK)
{
	CRect rect;
	rect.SetRectEmpty();

	MakeText( pDevice, strTALK, rect, DT_CALCRECT);
}

void CTClientTalkBox::Render( LPDIRECT3DDEVICE9 pDevice,
							  LPD3DXVECTOR3 pPOS)
{
	m_vWORLD = D3DXMATRIX(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pPOS->x, pPOS->y, pPOS->z, 1.0f);

	D3DXMATRIX vLOCAL(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	FLOAT fCORNER = TTALKLINE_HEIGHT;
	FLOAT fHEIGHT = FLOAT(m_nHeight);
	FLOAT fWIDTH = FLOAT(m_nWidth);
	FLOAT fDepthBias = BSPBIAS_BASE;

	FLOAT vCORNER[4][2] = {
		{ -fWIDTH / 2.0f, fHEIGHT + fCORNER},
		{  fWIDTH / 2.0f, fHEIGHT + fCORNER},
		{ -fWIDTH / 2.0f, fCORNER},
		{  fWIDTH / 2.0f, fCORNER}};

	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000010);
	pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0x70000000);
	pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));

	pDevice->SetStreamSource( 0, m_vVB.GetVB(), 0, sizeof(WPVERTEX));
	pDevice->SetIndices(m_vIB.GetIB());

	pDevice->SetSoftwareVertexProcessing(TRUE);
	pDevice->SetFVF(T3DFVF_WPVERTEX);
	pDevice->SetTexture( 0, m_pSKIN ? m_pSKIN->GetTexture() : NULL);

	pDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS);

	vLOCAL._11 = fCORNER;
	vLOCAL._22 = fCORNER;

	for( int i=0; i<4; i++)
	{
		vLOCAL._41 = vCORNER[i][0];
		vLOCAL._42 = vCORNER[i][1];

		pDevice->SetTransform( D3DTS_WORLDMATRIX(i), &(vLOCAL * m_vWORLD));
	}

	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0, 0, 16, 0, 18);
	fDepthBias = EXTBIAS_BASE;

	pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
	pDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	pDevice->SetSoftwareVertexProcessing(FALSE);

	vLOCAL._11 = 1.0f;
	vLOCAL._22 = 1.0f;
	vLOCAL._41 = 0.0f;
	vLOCAL._42 = fCORNER;
	vLOCAL._43 = -0.01f;

	TextOut( pDevice, vLOCAL * m_vWORLD, m_strTEXT, FLOAT(m_nHeight));
}

CRect CTClientTalkBox::GetRect()
{
	return CRect(
		(int)m_vWORLD._41, 
		(int)m_vWORLD._42, 
		(int)m_vWORLD._41 + m_nWidth,
		(int)m_vWORLD._42 + m_nHeight);
}





