#include "StdAfx.h"

CT3DVertex CTachyonSlashSFX::m_vVB;
BYTE CTachyonSlashSFX::m_bSTEP = 0;


CTachyonSlashSFX::CTachyonSlashSFX()
{
	D3DXMatrixIdentity(&m_vLENGTH);
	m_vKEY.clear();

	m_dwTexTick = 0;
	m_dwCOLOR = 0;
	m_bINTER = 0;

	m_pPIVOT = NULL;
	m_pTEX = NULL;
	m_bSTOP = TRUE;
}

CTachyonSlashSFX::~CTachyonSlashSFX()
{
	ResetKEY();
}

void CTachyonSlashSFX::InitVB( BYTE bSTEP)
{
	LPWLVERTEX pVBDATA = new WLVERTEX[2 * bSTEP];
	LPWLVERTEX pDATA = pVBDATA;

	for( BYTE i=0; i<bSTEP; i++)
	{
		static FLOAT vDATA[2][2] = {
			{ 0.0f, 0.0f},
			{ 1.0f, 1.0f}};

		DWORD dwCOLOR = ((0xFF - 0xFF * i / (bSTEP - 1)) << 24) | 0x00FFFFFF;
		FLOAT fU = 1.0f - FLOAT(i) / FLOAT(bSTEP - 1);

		for( BYTE j=0; j<2; j++)
		{
			pDATA->m_dwColor = dwCOLOR;
			pDATA->m_dwMatIndex = i;

			pDATA->m_fPosX = vDATA[j][0];
			pDATA->m_fPosY = 0.0f;
			pDATA->m_fPosZ = 0.0f;

			pDATA->m_fV = vDATA[j][1];
			pDATA->m_fU = fU;

			for( BYTE k=0; k<WEIGHT_COUNT; k++)
				pDATA->m_fWeight[k] = 0.0f;

			pDATA->m_fWeight[0] = 1.0f;
			pDATA++;
		}
	}

	m_vVB.LoadT3DVB(
		2 * bSTEP * sizeof(WLVERTEX),
		D3DUSAGE_SOFTWAREPROCESSING,
		T3DFVF_WLVERTEX);

	m_vVB.LoadT3DVBDATA(
		(LPBYTE) pVBDATA,
		2 * bSTEP * sizeof(WLVERTEX), 0);

	delete[] pVBDATA;
	m_bSTEP = bSTEP;
}

void CTachyonSlashSFX::ReleaseVB()
{
	m_vVB.ReleaseDATA();
	m_bSTEP = 0;
}

BYTE CTachyonSlashSFX::IsPlay()
{
	return m_bSTOP && m_vKEY.empty() ? FALSE : TRUE;
}

void CTachyonSlashSFX::ResetKEY()
{
	while(!m_vKEY.empty())
	{
		delete m_vKEY.back();
		m_vKEY.pop_back();
	}

	m_dwTexTick = 0;
	m_pPIVOT = NULL;

	SFXStop();
}

void CTachyonSlashSFX::InitSFX( LPD3DXMATRIX pPIVOT,
							    LPTEXTURESET pTEX,
								DWORD dwCOLOR,
								FLOAT fLength,
								BYTE bINTER)
{
	ResetKEY();

	m_dwCOLOR = dwCOLOR;
	m_bINTER = bINTER;
	m_pPIVOT = pPIVOT;
	m_pTEX = pTEX;
	m_vLENGTH._11 = fLength;
}

void CTachyonSlashSFX::SFXStart()
{
	if(m_pPIVOT)
	{
		m_bSTOP = FALSE;

		if(m_vKEY.empty())
		{
			LPD3DXMATRIX pKEY = new D3DXMATRIX();

			(*pKEY) = m_vLENGTH * (*m_pPIVOT);
			m_dwTexTick = 0;

			m_vKEY.push_back(pKEY);
		}
	}
}

void CTachyonSlashSFX::SFXStop()
{
	m_bSTOP = TRUE;
}

void CTachyonSlashSFX::CalcTick( DWORD dwTick)
{
	if( IsPlay() && m_pTEX )
	{
		m_dwTexTick += dwTick;
		m_dwTexTick %= m_pTEX->m_dwTotalTick;
	}
}

void CTachyonSlashSFX::CalcKEY()
{
	if(!IsPlay())
		return;

	for( BYTE i=0; i<m_bINTER && m_bSTOP; i++)
	{
		delete m_vKEY.back();
		m_vKEY.pop_back();

		if(m_vKEY.empty())
			return;
	}

	D3DXQUATERNION vKeyR[2];
	D3DXVECTOR3 vKeyP[2];
	D3DXVECTOR3 vKeyS;

	D3DXMatrixDecompose(
		&vKeyS,
		&vKeyR[0],
		&vKeyP[0],
		m_vKEY[0]);

	D3DXMatrixDecompose(
		&vKeyS,
		&vKeyR[1],
		&vKeyP[1],
		&(m_vLENGTH * (*m_pPIVOT)));

	for( i=0; i<m_bINTER; i++)
	{
		FLOAT fTime = FLOAT(i + 1) / FLOAT(m_bINTER);
		LPD3DXMATRIX pKEY = NULL;

		D3DXQUATERNION vROT;
		D3DXVECTOR3 vPOS;
		D3DXMATRIX vMAT;

		if( m_bSTOP || INT(m_vKEY.size()) >= m_bSTEP )
		{
			pKEY = m_vKEY.back();
			m_vKEY.pop_back();
		}
		else
			pKEY = new D3DXMATRIX();

		D3DXQuaternionSlerp(
			&vROT,
			&vKeyR[0],
			&vKeyR[1],
			fTime);

		D3DXVec3Lerp(
			&vPOS,
			&vKeyP[0],
			&vKeyP[1],
			fTime);

		D3DXMatrixScaling(
			pKEY,
			vKeyS.x,
			vKeyS.y,
			vKeyS.z);

		D3DXMatrixRotationQuaternion(
			&vMAT,
			&vROT);
		(*pKEY) *= vMAT;

		D3DXMatrixTranslation(
			&vMAT,
			vPOS.x,
			vPOS.y,
			vPOS.z);
		(*pKEY) *= vMAT;

		m_vKEY.insert( m_vKEY.begin(), pKEY);
	}
}

void CTachyonSlashSFX::BeginSFXScene( CD3DDevice *pDevice)
{
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT);

	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

	pDevice->m_pDevice->SetSoftwareVertexProcessing(TRUE);
	pDevice->m_pDevice->SetFVF(T3DFVF_WLVERTEX);

	pDevice->m_pDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
	pDevice->m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS);

	pDevice->m_pDevice->SetStreamSource( 0, m_vVB.GetVB(), 0, sizeof(WLVERTEX));
	pDevice->m_pDevice->SetVertexShader(NULL);
	pDevice->m_pDevice->SetPixelShader(NULL);
}

void CTachyonSlashSFX::EndSFXScene( CD3DDevice *pDevice)
{
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	pDevice->m_pDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	pDevice->m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);

	pDevice->m_pDevice->SetSoftwareVertexProcessing(FALSE);
}

void CTachyonSlashSFX::Render( CD3DDevice *pDevice)
{
	if(!IsPlay())
		return;
	CalcKEY();

	int nCount = INT(m_vKEY.size());
	for( int i=0; i<nCount; i++)
		pDevice->m_pDevice->SetTransform( D3DTS_WORLDMATRIX(i), m_vKEY[i]);

	if( nCount > 2 )
	{
		D3DXMATRIX vMAT;

		D3DXMatrixIdentity(&vMAT);
		if(m_pTEX)
		{
			m_pTEX->m_dwCurTick = m_dwTexTick;
			UVKEY vUV = m_pTEX->GetUV();
			CTMath::GetUVMatrix( &vMAT, &vUV);

			pDevice->m_pDevice->SetTexture( 0, m_pTEX->GetTexture());
			pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		}
		else
		{
			pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
			pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
		}

		pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0x00000000);
		pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
		pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &vMAT);
		pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 * (nCount - 1));

		pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, m_dwCOLOR);
		pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
		pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 * (nCount - 1));
	}
}
