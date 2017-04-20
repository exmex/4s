#include "StdAfx.h"


int CTClientNumber::m_nTexPOS[11][3] = {
	{ 144, 64, 36},
	{   0,  0, 36},
	{  36,  0, 36},
	{  72,  0, 36},
	{ 108,  0, 36},
	{ 144,  0, 36},
	{   0, 64, 36},
	{  36, 64, 36},
	{  72, 64, 36},
	{ 108, 64, 36},
	{ 180, 64, 36}};
LPTEXTURESET CTClientNumber::m_pTEX = NULL;


CTClientNumber::CTClientNumber()
{
	m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vDIR = D3DXVECTOR2( 0.0f, 0.0f);
	m_vDIGIT.clear();
	m_pVB = NULL;

	m_fSCALE = 1.0f;
	m_dwColor = 0;
	m_dwTick = 0;
	m_nNumber = 0;
}

CTClientNumber::~CTClientNumber()
{
	Release();
}

BYTE CTClientNumber::IsDead()
{
	return m_dwTick < TNUMBER_TICK ? FALSE : TRUE;
}

void CTClientNumber::CalcTick( DWORD dwTick)
{
	m_dwTick += dwTick;

	if( m_dwTick > TNUMBER_TICK )
		m_dwTick = TNUMBER_TICK;
}

void CTClientNumber::BeginDraw( CD3DDevice *pDevice,
							    CD3DCamera *pCamera)
{
	pCamera->Activate(TRUE);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	pDevice->m_pDevice->SetFVF(T3DFVF_PVERTEX);

	pDevice->m_pDevice->SetTexture( 0, m_pTEX ? m_pTEX->GetTexture() : NULL);
	pDevice->m_pDevice->SetVertexShader(NULL);
	pDevice->m_pDevice->SetPixelShader(NULL);
}

void CTClientNumber::EndDraw( CD3DCamera *pCamera)
{
	pCamera->Activate(TRUE);
}

void CTClientNumber::Render( CD3DDevice *pDevice,
							 LPD3DXMATRIX pPROJ)
{
	FLOAT fINIT = FLOAT(min( m_dwTick, TDAMAGE_TICK)) / FLOAT(TNUMBER_TICK);
	FLOAT fTIME = FLOAT(m_dwTick) / FLOAT(TNUMBER_TICK) - fINIT;
	FLOAT fRATE = FLOAT(TNUMBER_TICK) / FLOAT(TDAMAGE_TICK);
	FLOAT fSCALE = fINIT * fRATE * (2.0f - fINIT * fRATE);

	D3DXVECTOR3 vTPOS = m_vPOS;
	D3DXMATRIX vWORLD;

	D3DXMatrixIdentity(&vWORLD);
	CTMath::Transform(
		pPROJ,
		&vTPOS);
	fRATE = TNUMBER_SPEED_RATE * fINIT - 0.5f * (TNUMBER_SPEED_RATE - 1.0f) * fRATE * fINIT * fINIT + fTIME;

	vWORLD._11 = fSCALE * m_fSCALE * TNUMBER_SCALE;
	vWORLD._22 = vWORLD._11;
	vWORLD._33 = vWORLD._11;

	vWORLD._41 = vTPOS.x + m_vDIR.x * fSCALE * TNUMBER_SPEED_X;
	vWORLD._42 = vTPOS.y + m_vDIR.y * fRATE * TNUMBER_SPEED_Y;
	vWORLD._43 = vTPOS.z;

	fTIME += fINIT;
	fTIME *= fTIME;
	m_dwColor = ((DWORD(0xFF * (1.0f - fTIME)) << 24) & 0xFF000000) | (m_dwColor & 0x00FFFFFF);

	pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, m_dwColor);
	pDevice->m_pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(PVERTEX));
	pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);

	for( int i=0; i<INT(m_vDIGIT.size()); i++)
	{
		D3DXMatrixIdentity(&vWORLD);
		vWORLD._11 = FLOAT(m_nTexPOS[m_vDIGIT[i]][2]) / TNUMPANNEL_WIDTH;
		vWORLD._22 = FLOAT(TNUMBER_HEIGHT) / TNUMPANNEL_HEIGHT;

		vWORLD._32 = FLOAT(m_nTexPOS[m_vDIGIT[i]][1]) / TNUMPANNEL_HEIGHT;
		vWORLD._31 = FLOAT(m_nTexPOS[m_vDIGIT[i]][0]) / TNUMPANNEL_WIDTH;

		pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &vWORLD);
		pDevice->m_pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			i * 4, 2);
	}
}

void CTClientNumber::Release()
{
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
	m_vDIGIT.clear();

	m_fSCALE = 1.0f;
	m_dwColor = 0;
	m_dwTick = 0;
	m_nNumber = 0;
}

void CTClientNumber::InitNumber( CD3DDevice *pDevice,
								 DWORD dwColor,
								 int nNumber)
{
	int nWidth = 0;
	Release();

	m_dwColor = dwColor;
	m_nNumber = nNumber;

	nNumber = abs(nNumber);
	do
	{
		BYTE bDigit = nNumber % 10;

		nWidth += m_nTexPOS[bDigit][2];
		m_vDIGIT.push_back(bDigit);

		nNumber /= 10;
	}
	while(nNumber);

	if( m_nNumber < 0 )
	{
		nWidth += m_nTexPOS[TMINUS_SIGN_INDEX][2];
		m_vDIGIT.push_back(TMINUS_SIGN_INDEX);
	}

	pDevice->m_pDevice->CreateVertexBuffer(
		4 * INT(m_vDIGIT.size()) * sizeof(PVERTEX),
		0, T3DFVF_PVERTEX,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL);

	LPPVERTEX pDATA = NULL;
	int nPOS = nWidth / 2;

	m_pVB->Lock( 0, 0, (LPVOID *) &pDATA, 0);
	for( int i=0; i<INT(m_vDIGIT.size()); i++)
	{
		int nLocal = m_nTexPOS[m_vDIGIT[i]][2];
		int nVert[4][4] = {
			{ nPOS - nLocal, TNUMBER_HEIGHT, 0, 0},
			{		   nPOS, TNUMBER_HEIGHT, 1, 0},
			{ nPOS - nLocal,			  0, 0, 1},
			{		   nPOS,			  0, 1, 1}};

		for( int j=0; j<4; j++)
		{
			pDATA->m_fPosX = FLOAT(nVert[j][0]);
			pDATA->m_fPosY = FLOAT(nVert[j][1]);
			pDATA->m_fPosZ = 0.0f;
			pDATA->m_fU = FLOAT(nVert[j][2]);
			pDATA->m_fV = FLOAT(nVert[j][3]);

			pDATA++;
		}

		nPOS -= nLocal;
	}

	m_pVB->Unlock();
}
