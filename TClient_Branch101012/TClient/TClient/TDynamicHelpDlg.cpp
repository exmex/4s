#include "StdAfx.h"

#define TSTART_TEXTSIZE		(64)
#define TMAX_TEXTSIZE		(512)

#define TTEXT_SPEED			(20)


CTDynamicHelpDlg::CTDynamicHelpDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc)
{
	m_pTEX = NULL;
	m_pVB = NULL;

	m_pHEIGHT = NULL;
	m_pWIDTH = NULL;

	m_nPartCount = 0;
	m_nHEIGHT = 0;
	m_nWIDTH = 0;

	m_dwTSPEED = TTEXT_SPEED;
	m_dwTRES = 0;
	m_dwTPOS = 0;

	m_dwColor = CTChatFrame::GetChatTypeColor(CHAT_WORLD);
	m_pGDIFont = FindFont(ID_FONT_QUEST_NORMAL);

	m_bSND = FALSE;
}

CTDynamicHelpDlg::~CTDynamicHelpDlg()
{
	Release();
}

void CTDynamicHelpDlg::Release()
{
	if(m_pTEX)
	{
		for( int i=0; i<m_nPartCount; i++)
			if(m_pTEX[i])
				m_pTEX[i]->Release();

		delete[] m_pTEX;
		m_pTEX = NULL;
	}

	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	if(m_pHEIGHT)
	{
		delete[] m_pHEIGHT;
		m_pHEIGHT = NULL;
	}

	if(m_pWIDTH)
	{
		delete[] m_pWIDTH;
		m_pWIDTH = NULL;
	}

	m_nPartCount = 0;
	m_nHEIGHT = 0;
	m_nWIDTH = 0;

	m_dwTPOS = 0;
	m_dwTRES = 0;
}

void CTDynamicHelpDlg::InitTEXT( CString& strTEXT)
{
	WCHAR pBUF[TGDIBUFFER_SIZE];
	Release();

	// Arab Ver.
	MultiByteToWideChar(	
		CP_THREAD_ACP, 0,	
		LPCSTR(strTEXT),
		strTEXT.GetLength() + 1,
		pBUF, TGDIBUFFER_SIZE);

	HDC hDC = CreateCompatibleDC(NULL);
	HFONT hOLDFONT = (HFONT) SelectObject(
		hDC,
		m_pGDIFont->m_pFont->GetSafeHandle());

	Graphics *pGDI = new Graphics(hDC);
	StringFormat vFORMAT;
	Font vFONT(hDC);

	CRect rect( 0, 0, 0, 0);
	RectF vRECT;
	// Arab Ver.
	if( CTNationOption::GERMANY || CTNationOption::FRANCE || CTNationOption::POLAND || CTNationOption::CZECH || 
		CTNationOption::ITALY || CTNationOption::SPAIN || CTNationOption::UNITEDKINGDOM ||
		CTNationOption::GREECE || CTNationOption::RUSSIA || CTNationOption::ROMANIA || CTNationOption::EGYPT ||
		CTNationOption::UNITEDSTATE || CTNationOption::HUNGARY || CTNationOption::TURKEY ||
		CTNationOption::PORTUGAL )
	{
		pGDI->SetTextRenderingHint(static_cast<TextRenderingHint>(CD3DFont::m_nRenderingHint));
	}
	else
        pGDI->SetTextRenderingHint(TextRenderingHintAntiAlias);
	pGDI->MeasureString(
		pBUF, -1,
		&vFONT,
		PointF(
		FLOAT(rect.left),
		FLOAT(rect.top)),
		&vRECT);

	delete pGDI;
	pGDI = NULL;

	rect.bottom = INT(rect.top + vRECT.Height + 1);
	rect.right = INT(rect.left + vRECT.Width + 1);

	m_nHEIGHT = rect.Height();
	m_nWIDTH = rect.Width();

	m_dwTPOS = 0;
	m_dwTRES = 0;
	rect = m_rc;

	rect.DeflateRect(
		(rect.Height() - m_nHEIGHT) / 2,
		(rect.Height() - m_nHEIGHT) / 2);
	m_nWIDTH += rect.Width();

	BITMAPINFO bmi;
	LPDWORD pBits;

	ZeroMemory( &bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = m_nWIDTH;
	bmi.bmiHeader.biHeight = -m_nHEIGHT;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount = 32;

	HBITMAP hBMP = CreateDIBSection(
		hDC,
		&bmi,
		DIB_RGB_COLORS,
		(LPVOID *) &pBits,
		NULL, 0);
	HBITMAP hOLDBMP = (HBITMAP) SelectObject( hDC, hBMP);

	vFORMAT.SetAlignment(StringAlignmentNear);
	vFORMAT.SetLineAlignment(StringAlignmentNear);
	pGDI = new Graphics(hDC);
	// Arab Ver.
	if( CTNationOption::GERMANY || CTNationOption::FRANCE || CTNationOption::POLAND || CTNationOption::CZECH || 
		CTNationOption::ITALY || CTNationOption::SPAIN || CTNationOption::UNITEDKINGDOM ||
		CTNationOption::GREECE || CTNationOption::RUSSIA || CTNationOption::ROMANIA || CTNationOption::EGYPT ||
		CTNationOption::UNITEDSTATE || CTNationOption::HUNGARY || CTNationOption::TURKEY ||
		CTNationOption::PORTUGAL )
	{
		pGDI->SetTextRenderingHint(static_cast<TextRenderingHint>(CD3DFont::m_nRenderingHint));
	}
	else
        pGDI->SetTextRenderingHint(TextRenderingHintAntiAlias);
	pGDI->DrawString(
		pBUF, -1,
		&vFONT,
		RectF(
		FLOAT(rect.Width()),
		1.0f,
		FLOAT(m_nWIDTH),
		FLOAT(m_nHEIGHT) - 1.0f),
		&vFORMAT,
		&SolidBrush( Color( 0xFF, 0xFF, 0xFF)));

	delete pGDI;
	pGDI = NULL;

	int nHeight = TSTART_TEXTSIZE;
	int nWidth = TSTART_TEXTSIZE;
	int nLine = 1;

	while( nWidth < m_nWIDTH )
		nWidth *= 2;

	if( nWidth > TMAX_TEXTSIZE )
	{
		nLine = nWidth / TMAX_TEXTSIZE + (nWidth % TMAX_TEXTSIZE ? 1 : 0);
		nWidth = TMAX_TEXTSIZE;
	}
	m_nPartCount = 1;

	while( nHeight < nLine * m_nHEIGHT )
	{
		nHeight *= 2;

		if( nHeight > TMAX_TEXTSIZE )
		{
			nLine -= TMAX_TEXTSIZE / m_nHEIGHT;
			nHeight = TSTART_TEXTSIZE;
			m_nPartCount++;
		}
	}

	m_pTEX = new LPDIRECT3DTEXTURE9[m_nPartCount];
	m_pHEIGHT = new WORD[m_nPartCount];
	m_pWIDTH = new WORD[m_nPartCount];

	for( int i=0; i<m_nPartCount - 1; i++)
	{
		m_pHEIGHT[i] = TMAX_TEXTSIZE;
		m_pWIDTH[i] = TMAX_TEXTSIZE;

		D3DXCreateTexture(
			CTachyonRes::m_pDEVICE->m_pDevice,
			m_pWIDTH[i],
			m_pHEIGHT[i],
			1, 0,
			D3DFMT_A4R4G4B4,
			D3DPOOL_MANAGED,
			&m_pTEX[i]);
	}

	m_pHEIGHT[i] = nHeight;
	m_pWIDTH[i] = nWidth;

	D3DXCreateTexture(
		CTachyonRes::m_pDEVICE->m_pDevice,
		m_pWIDTH[i],
		m_pHEIGHT[i],
		1, 0,
		D3DFMT_A4R4G4B4,
		D3DPOOL_MANAGED,
		&m_pTEX[i]);

	for( i=0; i<m_nPartCount; i++)
	{
		int nBASE = TMAX_TEXTSIZE / m_nHEIGHT * i * TMAX_TEXTSIZE;

		if(m_pTEX[i])
		{
			D3DLOCKED_RECT lr;

			if(SUCCEEDED(m_pTEX[i]->LockRect( 0, &lr, NULL, 0)))
			{
				LPBYTE pDestRow = (LPBYTE) lr.pBits;
				LPWORD pDst16;

				for( int nPosY = 0; nPosY < m_pHEIGHT[i]; nPosY++)
				{
					int nLEFT = nBASE + nPosY / m_nHEIGHT * TMAX_TEXTSIZE;
					int nY = nPosY % m_nHEIGHT;

					pDst16 = (LPWORD) pDestRow;
					for( int nPosX = 0; nPosX < m_pWIDTH[i]; nPosX++)
					{
						int nX = nLEFT + nPosX;
						BYTE bColor = nX < m_nWIDTH ? (BYTE) ((pBits[nY * m_nWIDTH + nX] & 0xFF) >> 4) : 0x00;

						if( bColor > 0 )
						{
							bColor += TGDICOLOR_OFFSET;

							bColor = max( TGDITEXTCLR_MIN, bColor);
							bColor = min( TGDITEXTCLR_MAX, bColor);

							*pDst16 = (WORD(bColor) << 12)|0x0FFF;
						}
						else
							*pDst16 = 0x0000;

						pDst16++;
					}

					pDestRow += lr.Pitch;
				}

				m_pTEX[i]->UnlockRect(0);
			}
		}
	}

	CTachyonRes::m_pDEVICE->m_pDevice->CreateVertexBuffer(
		4 * sizeof(TVERTEX),
		0, T3DFVF_TVERTEX,
		D3DPOOL_MANAGED,
		&m_pVB, NULL);

	SelectObject( hDC, hOLDBMP);
	SelectObject( hDC, hOLDFONT);
	DeleteObject(hBMP);
	DeleteDC(hDC);
}

HRESULT CTDynamicHelpDlg::Render( DWORD dwTickCount)
{
	if(!IsVisible())
		return S_OK;

	HRESULT hr = CTClientUIBase::Render(dwTickCount);
	CRect rect = m_rc;

	if(!m_nPartCount)
		return hr;

	DWORD dwTextREF = ((m_dwColor & 0xFF000000) >> 24) * (TGDITEXTCLR_MIN - 1) / TGDITEXTCLR_MAX;
	DWORD dwTMOVE = m_dwTSPEED * dwTickCount + m_dwTRES;
	WORD wTWIDTH = m_nPartCount > 1 ? TMAX_TEXTSIZE : m_pWIDTH[0];

	m_dwTPOS += dwTMOVE / 1000;
	m_dwTPOS %= m_nWIDTH;
	m_dwTRES = dwTMOVE % 1000;

	ComponentToScreen(&rect);
	rect.DeflateRect(
		(rect.Height() - m_nHEIGHT) / 2,
		(rect.Height() - m_nHEIGHT) / 2);
	rect.OffsetRect( 0, 2);

	int nTPAGE = TMAX_TEXTSIZE / m_nHEIGHT;
	int nTWIDTH = rect.Width();

	int nTOTAL = m_nWIDTH / wTWIDTH + (m_nWIDTH % wTWIDTH ? 1 : 0);
	int nTLEFT = m_dwTPOS % wTWIDTH;
	int nTLINE = (m_dwTPOS / wTWIDTH) % nTOTAL;

	CTachyonRes::m_pDEVICE->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CTachyonRes::m_pDEVICE->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CTachyonRes::m_pDEVICE->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

	CTachyonRes::m_pDEVICE->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	CTachyonRes::m_pDEVICE->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	CTachyonRes::m_pDEVICE->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	CTachyonRes::m_pDEVICE->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CTachyonRes::m_pDEVICE->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	CTachyonRes::m_pDEVICE->m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	CTachyonRes::m_pDEVICE->m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	CTachyonRes::m_pDEVICE->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	CTachyonRes::m_pDEVICE->m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	CTachyonRes::m_pDEVICE->m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	CTachyonRes::m_pDEVICE->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	CTachyonRes::m_pDEVICE->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CTachyonRes::m_pDEVICE->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CTachyonRes::m_pDEVICE->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	CTachyonRes::m_pDEVICE->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	CTachyonRes::m_pDEVICE->m_pDevice->SetFVF(T3DFVF_TVERTEX);

	CTachyonRes::m_pDEVICE->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, m_dwColor);
	CTachyonRes::m_pDEVICE->m_pDevice->SetRenderState( D3DRS_ALPHAREF, dwTextREF);

	while( nTWIDTH > 0 )
	{
		int nTRES = min( nTWIDTH, wTWIDTH - nTLEFT);
		int nTPART = nTLINE / nTPAGE;
		int nLOCAL = m_nHEIGHT * (nTLINE % nTPAGE);

		FLOAT vUV[2][2] = {{
			FLOAT(nTLEFT) / FLOAT(m_pWIDTH[nTPART]),
			FLOAT(nLOCAL) / FLOAT(m_pHEIGHT[nTPART])}, {
			FLOAT(nTLEFT + nTRES) / FLOAT(m_pWIDTH[nTPART]),
			FLOAT(nLOCAL + m_nHEIGHT) / FLOAT(m_pHEIGHT[nTPART])}};

		FLOAT vPOINT[2][2] = {{
			FLOAT(rect.left) - 0.5f,
			FLOAT(rect.top) - 0.5f}, {
			FLOAT(rect.left + nTRES),
			FLOAT(rect.bottom)}};

		TVERTEX vMESH[4] = {
			{ vPOINT[0][0], vPOINT[0][1], 0.5f, 1.0f, vUV[0][0], vUV[0][1]},
			{ vPOINT[1][0], vPOINT[0][1], 0.5f, 1.0f, vUV[1][0], vUV[0][1]},
			{ vPOINT[0][0], vPOINT[1][1], 0.5f, 1.0f, vUV[0][0], vUV[1][1]},
			{ vPOINT[1][0], vPOINT[1][1], 0.5f, 1.0f, vUV[1][0], vUV[1][1]}};
		LPTVERTEX pDATA = NULL;

		m_pVB->Lock( 0, 4 * sizeof(TVERTEX), (LPVOID *) &pDATA, 0);
		memcpy( pDATA, vMESH, 4 * sizeof(TVERTEX));
		m_pVB->Unlock();

		CTachyonRes::m_pDEVICE->m_pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(TVERTEX));
		CTachyonRes::m_pDEVICE->m_pDevice->SetTexture( 0, m_pTEX[nTPART]);
		CTachyonRes::m_pDEVICE->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

		nTLINE = (nTLINE + 1) % nTOTAL;
		if(!nTLINE)
			nTRES = min( nTWIDTH, m_nWIDTH % wTWIDTH - nTLEFT);

		rect.left += nTRES;
		nTWIDTH -= nTRES;
		nTLEFT = 0;
	}

	CTachyonRes::m_pDEVICE->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	CTachyonRes::m_pDEVICE->m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

	return hr;
}
