#include "Stdafx.h"
#include "TClientGuildMark.h"
#include "TClientWnd.h"


CTClientGuildMark::MAPGMARKTEXTURE CTClientGuildMark::m_mapTEXTURE;
DWORD CTClientGuildMark::m_dwLEFTTICK = TMAXGUILDMARK_MAKETICK;

VECTORTEXTURESET CTClientGuildMark::m_vFRONTTEX;
VECTORTEXTURESET CTClientGuildMark::m_vBACKTEX;

VECTORIMAGE CTClientGuildMark::m_vFRONTIMG;
VECTORIMAGE CTClientGuildMark::m_vBACKIMG;


DWORD CTClientGuildMark::m_dwCOLOR[TMAXGUILD_MARK_COLOR] = 
{
	D3DCOLOR_XRGB(0,0,0),
	D3DCOLOR_XRGB(51,51,51),
	D3DCOLOR_XRGB(102,102,102),
	D3DCOLOR_XRGB(153,153,153),
	D3DCOLOR_XRGB(204,204,204),

	D3DCOLOR_XRGB(255,255,255),
	D3DCOLOR_XRGB(255,248,163),
	D3DCOLOR_XRGB(255,255,81),
	D3DCOLOR_XRGB(255,219,0),
	D3DCOLOR_XRGB(233,140,56),

	D3DCOLOR_XRGB(255,131,0),
	D3DCOLOR_XRGB(135,85,70),
	D3DCOLOR_XRGB(141,95,43),
	D3DCOLOR_XRGB(34,31,29),
	D3DCOLOR_XRGB(36,76,41),

	D3DCOLOR_XRGB(0,91,76),
	D3DCOLOR_XRGB(0,136,2),
	D3DCOLOR_XRGB(87,184,70),
	D3DCOLOR_XRGB(194,226,84),
	D3DCOLOR_XRGB(199,234,251),

	D3DCOLOR_XRGB(0,181,233),
	D3DCOLOR_XRGB(44,110,213),
	D3DCOLOR_XRGB(108,122,224),
	D3DCOLOR_XRGB(0,54,99),
	D3DCOLOR_XRGB(0,33,87),

	D3DCOLOR_XRGB(13,0,76),
	D3DCOLOR_XRGB(75,0,73),
	D3DCOLOR_XRGB(82,39,121),
	D3DCOLOR_XRGB(144,23,128),
	D3DCOLOR_XRGB(220,1,131),

	D3DCOLOR_XRGB(224,62,156),
	D3DCOLOR_XRGB(250,66,81),
	D3DCOLOR_XRGB(255,102,102),
	D3DCOLOR_XRGB(253,71,21),
	D3DCOLOR_XRGB(242,21,28),

	D3DCOLOR_XRGB(121,0,0),
	D3DCOLOR_XRGB(51,0,0),
	D3DCOLOR_XRGB(23,0,0)
};

BOOL CTClientGuildMark::OnDeviceReset( BOOL bRelease )
{
	if( bRelease ) // Release
	{
		MAPTMANTLETEXTURE::iterator it, end;
		it = m_mapTMANTLEBAKINGTEXTURE.begin();
		end = m_mapTMANTLEBAKINGTEXTURE.end();

		for(; it != end ; ++it )
		{
			CT3DTexture *pTEX = it->second->GetTexturePtr(0);

			delete pTEX;
			it->second->ClearTexture();
		}
	}
	else // Restore
	{
		CTClientWnd* pMainWnd = CTClientWnd::GetInstance();
		LPDIRECT3DDEVICE9 pDevice = pMainWnd->m_Device.m_pDevice;
		pDevice->BeginScene();

		MAPTMANTLETEXTURE::iterator it, end;
		it = m_mapTMANTLEBAKINGTEXTURE.begin();
		end = m_mapTMANTLEBAKINGTEXTURE.end();

		for(; it != end ; ++it )
		{
			LPTEXTURESET pTextureSet = GetMantleTexture( it->first, TRUE );
			it->second->PushTexturePtr( pTextureSet->GetTexturePtr(0) );
			delete pTextureSet;
		}

		pDevice->EndScene();
	}

	return TRUE;
}

LPDIRECT3DTEXTURE9 CTClientGuildMark::MCreateTexture(const CTClientGuildMark& r)
{
	if( m_dwLEFTTICK == 0 ||
		r.GetFront() >= m_vFRONTIMG.size() ||
		r.GetBack() >= m_vBACKIMG.size() )
	{
		return NULL;
	}

	DWORD dwStartTick = ::timeGetTime();

	CTClientWnd* pMainWnd = CTClientWnd::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pMainWnd->m_Device.m_pDevice;

	HRESULT hResult;
	LPDIRECT3DTEXTURE9 pRNDTEX;

	hResult = D3DXCreateTexture(
				pDevice,
				(UINT)TGUILDMARK_TEXSIZE,
				(UINT)TGUILDMARK_TEXSIZE, 1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT,
				&pRNDTEX);

	if( FAILED(hResult) )
	{
		return NULL;
	}

	LPDIRECT3DSURFACE9 pBACKUPBUF;
	hResult = pDevice->GetRenderTarget(0, &pBACKUPBUF);
	if( FAILED(hResult) )
	{
		pRNDTEX->Release();
		return NULL;
	}

	LPDIRECT3DSURFACE9 pRNDBUF;
	hResult = pRNDTEX->GetSurfaceLevel( 0, &pRNDBUF);
	if( FAILED(hResult) )
	{
		pRNDTEX->Release();
		return NULL;
	}

	hResult = pDevice->SetRenderTarget(0, pRNDBUF);
	if( FAILED(hResult) )
	{
		pRNDTEX->Release();
		return NULL;
	}

	pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		r.GetBackColor1(),
		1.0f, 0);

	CD3DImage* pFrontImg = m_vFRONTIMG[r.GetFront()];
	CD3DImage* pBackImg = m_vBACKIMG[r.GetBack()];

	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = (LONG)TGUILDMARK_TEXSIZE;
	rc.bottom = (LONG)TGUILDMARK_TEXSIZE;

	pBackImg->Render(pDevice, r.GetBackColor2(), 0,0, &rc);
	pFrontImg->Render(pDevice, r.GetFrontColor(), 0,0, &rc);

	pDevice->SetRenderTarget(0, pBACKUPBUF);
	m_mapTEXTURE.insert( make_pair(r,pRNDTEX) );

	DWORD dwElapsed = max(TMAXGUILDMARK_BASETICK, ::timeGetTime() - dwStartTick);
	if( dwElapsed >= m_dwLEFTTICK )
		m_dwLEFTTICK = 0;
	else 
		m_dwLEFTTICK -= dwElapsed;

	pBACKUPBUF->Release();
	pRNDBUF->Release();

	return pRNDTEX;
}
// ====================================================================
void CTClientGuildMark::MAddFrontImage(CD3DImage* pImage)
{
	m_vFRONTIMG.push_back(pImage);
}
// --------------------------------------------------------------------
void CTClientGuildMark::MAddBackImage(CD3DImage* pImage)
{
	m_vBACKIMG.push_back(pImage);
}

void CTClientGuildMark::LoadBakingTexture()
{
	CTachyonRes* pRES = CTClientGame::GetInstance()->GetResource();

	static dwBACK[] =
	{
		GUILD_BACK_00,
		GUILD_BACK_01,
		GUILD_BACK_02,
		GUILD_BACK_03,
		GUILD_BACK_04,
		GUILD_BACK_05,
		GUILD_BACK_06,
		GUILD_BACK_07,
		GUILD_BACK_08,
		GUILD_BACK_09,
		GUILD_BACK_10,
		GUILD_BACK_11,
		GUILD_BACK_12,
		GUILD_BACK_13,
		GUILD_BACK_14,
		GUILD_BACK_15,
		GUILD_BACK_16,
		GUILD_BACK_17,
		GUILD_BACK_18
	};

	static dwMARK[] =
	{
		GUILD_MARK_00,
		GUILD_MARK_01,
		GUILD_MARK_02,
		GUILD_MARK_03,
		GUILD_MARK_04,
		GUILD_MARK_05,
		GUILD_MARK_06,
		GUILD_MARK_07,
		GUILD_MARK_08,
		GUILD_MARK_09,
		GUILD_MARK_10,
		GUILD_MARK_11,
		GUILD_MARK_12,
		GUILD_MARK_13,
		GUILD_MARK_14,
		GUILD_MARK_15,
		GUILD_MARK_16,
		GUILD_MARK_17,
		GUILD_MARK_18
	};

	for( INT i=0; i<sizeof(dwBACK) / sizeof(DWORD); i++)
	{
		MAPRES::iterator finder = pRES->m_mapTEX.find(dwBACK[i]);
		if( finder != pRES->m_mapTEX.end() )
			 MAddBakingBackTexture((LPTEXTURESET) (*finder).second);
	}

	for( INT i=0; i<sizeof(dwMARK) / sizeof(DWORD); i++)
	{
		MAPRES::iterator finder = pRES->m_mapTEX.find(dwMARK[i]);
		if( finder != pRES->m_mapTEX.end() )
			 MAddBakingFrontTexture((LPTEXTURESET) (*finder).second);
	}
}
void CTClientGuildMark::MAddBakingFrontTexture( LPTEXTURESET p)
{
	m_vFRONTTEX.push_back(p);
}
void CTClientGuildMark::MAddBakingBackTexture( LPTEXTURESET p)
{
	m_vBACKTEX.push_back(p);
}
// ====================================================================
void CTClientGuildMark::MReset()
{
	m_dwLEFTTICK = TMAXGUILDMARK_MAKETICK;
}
// --------------------------------------------------------------------
void CTClientGuildMark::MRelease()
{
	MAPGMARKTEXTURE::iterator itr,end;
	itr = m_mapTEXTURE.begin();
	end = m_mapTEXTURE.end();

	for(; itr!=end; ++itr)
		itr->second->Release();

	m_mapTEXTURE.clear();
}
// ====================================================================
LPDIRECT3DTEXTURE9 CTClientGuildMark::MGetTexture(const CTClientGuildMark& r)
{
	MAPGMARKTEXTURE::iterator finder = m_mapTEXTURE.find(r);
	if( finder != m_mapTEXTURE.end() )
		return finder->second;

	return MCreateTexture(r); 
}
// ====================================================================

// ====================================================================
CTClientGuildMark::CTClientGuildMark(BYTE bFront, 
									 BYTE bBack, 
									 BYTE bFrontColor,
									 BYTE bBackColor1,
									 BYTE bBackColor2)
:	m_bFront(bFront), 
	m_bBack(bBack), 
	m_bFrontColor(bFrontColor), 
	m_bBackColor1(bBackColor1), 
	m_bBackColor2(bBackColor2)
{
}
// --------------------------------------------------------------------
CTClientGuildMark::CTClientGuildMark(const CTClientGuildMark& r)
{
	*this = r;
}
// ====================================================================
bool CTClientGuildMark::operator < ( const CTClientGuildMark& r) const
{
	if( m_bFront != r.m_bFront )
		return m_bFront < r.m_bFront;

	if( m_bBack != r.m_bBack )
		return m_bBack < r.m_bBack;

	if( m_bFrontColor != r.m_bFrontColor )
		return m_bFrontColor < r.m_bFrontColor;

	if( m_bBackColor1 != r.m_bBackColor1 )
		return m_bBackColor1 < r.m_bBackColor1;

	return m_bBackColor2 < r.m_bBackColor2;
}
// --------------------------------------------------------------------
CTClientGuildMark& CTClientGuildMark::operator = ( const CTClientGuildMark& r)
{
	m_bFront		= r.m_bFront;
	m_bBack			= r.m_bBack;
	m_bFrontColor	= r.m_bFrontColor;
	m_bBackColor1	= r.m_bBackColor1;
	m_bBackColor2	= r.m_bBackColor2;

	return *this;
}
// ====================================================================
LPDIRECT3DTEXTURE9 CTClientGuildMark::GetTexture()
{
	return MGetTexture(*this);
}
// ====================================================================
void CTClientGuildMark::SetMantleTexture( CTClientChar* pPlayer, CTClientGuildMark* pMark )
{
	MAPCLKINST::iterator itCLK = pPlayer->m_OBJ.m_mapCLK.find( ID_CLK_BACK );
	if( itCLK == pPlayer->m_OBJ.m_mapCLK.end() )
		return ;

	if( itCLK->second->m_dwCurCL != ID_MANTLE )
		return ;

	if( itCLK->second->m_pMESH &&
		itCLK->second->m_pMESH->m_pMESH )
	{
		CTachyonMesh* pMESH = itCLK->second->m_pMESH->m_pMESH;

		if( pMESH )
		{
			MAPOBJPART::iterator itDRAW, endDRAW;
			itDRAW = pPlayer->m_OBJ.m_mapDRAW.begin();
			endDRAW = pPlayer->m_OBJ.m_mapDRAW.end();

			for(; itDRAW != endDRAW ; ++itDRAW)
			{
				VECTOROBJPART *pDRAW = itDRAW->second;

				if( !pDRAW )
					continue;

				VECTOROBJPART::iterator itPART, endPART;
				itPART = pDRAW->begin();
				endPART = pDRAW->end();

				for(; itPART != endPART ; ++itPART)
				{
					LPOBJPART pPART = (*itPART);

					if( pPART &&
						pPART->m_pMESH == pMESH)
					{
						CTMantleTextureKey k;
						k.m_bBack = pMark->GetBack();
						k.m_bBackColor1 = pMark->GetBackColorID1();
						k.m_bBackColor2 = pMark->GetBackColorID2();
						k.m_bFront = pMark->GetFront();
						k.m_bFrontColor = pMark->GetFrontColorID();
						k.m_bRaceID = pPlayer->GetRaceID();
						k.m_bSexID = pPlayer->m_bSex;
						k.m_dwMESH = itCLK->second->m_dwCurMESH;

						LPTEXTURESET pTextureSet = GetMantleTexture( k );
						if( pTextureSet == NULL || pPART->m_pTEX->m_pTEX[1] == pTextureSet )
							continue;

						LPOBJTEX pNewTex = new OBJTEX;
						*pNewTex = *(pPART->m_pTEX);

						pNewTex->m_pTEX[1] = GetMantleTexture( k );
						pNewTex->m_bType[1] = TT_TEX;
						pNewTex->m_dwOP = D3DTOP_MODULATE2X;
						pNewTex->m_dwCOLOR = D3DCOLOR_XRGB(0,0,0);

						if( pPART->m_bSelfDeleteTEX )
							delete pPART->m_pTEX;

						pPART->m_pTEX = pNewTex;
						pPART->m_bSelfDeleteTEX = TRUE;
					}
				}
			}
		}
	}
}

void CTClientGuildMark::SetMantleColor( CTClientChar* pPlayer, TCOLOR color )
{
	MAPCLKINST::iterator itCLK = pPlayer->m_OBJ.m_mapCLK.find( ID_CLK_BACK );
	if( itCLK == pPlayer->m_OBJ.m_mapCLK.end() )
		return ;

	if( itCLK->second->m_dwCurCL != ID_MANTLE )
		return ;

	if( itCLK->second->m_pMESH &&
		itCLK->second->m_pMESH->m_pMESH )
	{
		CTachyonMesh* pMESH = itCLK->second->m_pMESH->m_pMESH;

		if( pMESH )
		{
			MAPOBJPART::iterator itDRAW, endDRAW;
			itDRAW = pPlayer->m_OBJ.m_mapDRAW.begin();
			endDRAW = pPlayer->m_OBJ.m_mapDRAW.end();

			for(; itDRAW != endDRAW ; ++itDRAW)
			{
				VECTOROBJPART *pDRAW = itDRAW->second;

				if( !pDRAW )
					continue;

				VECTOROBJPART::iterator itPART, endPART;
				itPART = pDRAW->begin();
				endPART = pDRAW->end();

				for(; itPART != endPART ; ++itPART)
				{
					LPOBJPART pPART = (*itPART);

					if( pPART &&
						pPART->m_pMESH == pMESH)
					{
						LPOBJTEX pNewTex = new OBJTEX;
						*pNewTex = *(pPART->m_pTEX);
						if( pPART->m_bSelfDeleteTEX )
							delete pPART->m_pTEX;
						pPART->m_pTEX = pNewTex;
						pPART->m_bSelfDeleteTEX = TRUE;

						pPART->m_pTEX->m_pTEX[1] = NULL;
						pPART->m_pTEX->m_bType[1] = TT_COLOR;
						pPART->m_pTEX->m_dwOP = D3DTOP_MODULATE2X;

						if( color < TCOLOR_COUNT )
                            pPART->m_pTEX->m_dwCOLOR = dwTCOLOR[ color ];
						else
							pPART->m_pTEX->m_dwCOLOR = dwTCOLOR[ TCOLOR_MANTLE_DEFAULT ];
					}
				}
			}
		}
	}
}

MAPTMANTLETEXTURE CTClientGuildMark::m_mapTMANTLEBAKINGTEXTURE;

LPTEXTURESET CTClientGuildMark::GetMantleTexture(
	const CTMantleTextureKey& k,
	BOOL bDeviceReset )
{
	CTClientWnd* pMainWnd = CTClientWnd::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pMainWnd->m_Device.m_pDevice;

	if( !bDeviceReset )
	{
		MAPTMANTLETEXTURE::iterator it = m_mapTMANTLEBAKINGTEXTURE.find( k );
		if( it != m_mapTMANTLEBAKINGTEXTURE.end() )
			return it->second;
	}

	if( k.m_bFront >= m_vFRONTTEX.size() || k.m_bBack >= m_vBACKTEX.size() )
		return NULL;

	LPTMANTLECOORDINFO pCoordInfo = CTChart::GetTMANTLECOORD( TMANTLECOORDKEY( k.m_bRaceID, k.m_bSexID, k.m_dwMESH ) );
	if( !pCoordInfo )
		return NULL;

	if( k.m_bFront >= pCoordInfo->m_vUV3.size() || k.m_bBack >= pCoordInfo->m_vUV1.size() || k.m_bBack >= pCoordInfo->m_vUV2.size() )
		return NULL;

	LPDIRECT3DTEXTURE9 pRNDTEX2;
	HRESULT hResult = D3DXCreateTexture(
		pDevice,
		MANTLE_FINAL_SIZE, MANTLE_FINAL_SIZE,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&pRNDTEX2);

	if( FAILED(hResult) )
		return NULL;

	LPDIRECT3DSURFACE9 pBACKUPBUF;
	LPDIRECT3DSURFACE9 pRNDBUF2;
	hResult = pDevice->GetRenderTarget(0, &pBACKUPBUF);
	if( FAILED(hResult) )
	{
		pRNDTEX2->Release();
		return NULL;
	}

	hResult = pRNDTEX2->GetSurfaceLevel( 0, &pRNDBUF2);
	if( FAILED(hResult) )
	{
		pRNDTEX2->Release();
		return NULL;
	}

	hResult = pDevice->SetRenderTarget(0, pRNDBUF2);
	if( FAILED(hResult) )
	{
		pRNDTEX2->Release();
		return NULL;
	}

	D3DVIEWPORT9 vCULLRECT;
	vCULLRECT.X = 0;
	vCULLRECT.Y = 0;
	vCULLRECT.Width = MANTLE_FINAL_SIZE;
	vCULLRECT.Height = MANTLE_FINAL_SIZE;
	vCULLRECT.MinZ = 0.0f;
	vCULLRECT.MaxZ = 1.0f;
	pDevice->SetViewport( &vCULLRECT );

	pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		CTClientGuildMark::m_dwCOLOR[k.m_bBackColor1],
		1.0f, 0);

	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);

	FLOAT dwLODBias = 0;
	DWORD dwMaxMip = 0;
	pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &dwLODBias) );
	pDevice->SetSamplerState( 0, D3DSAMP_MAXMIPLEVEL, dwMaxMip );

	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity( &mat );
	pDevice->SetTransform( D3DTS_WORLD, &mat );
	pDevice->SetTransform( D3DTS_PROJECTION, &mat );
	pDevice->SetTransform( D3DTS_VIEW, &mat );
	pDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );

	pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, CTClientGuildMark::m_dwCOLOR[k.m_bBackColor2] );
	pDevice->SetTexture( 0, m_vBACKTEX[k.m_bBack]->GetTexture());

	{
		TMANTLECOORD& uv1 = pCoordInfo->m_vUV1[ k.m_bBack ];
		D3DXVECTOR2& vLeftTop = uv1.m_vLeftTop;
		D3DXVECTOR2& vLeftBottom = uv1.m_vLeftBottom;
		D3DXVECTOR2& vRightTop = uv1.m_vRightTop;
		D3DXVECTOR2& vRightBottom = uv1.m_vRightBottom;
		FLOAT avgX = (vLeftTop.x+vRightTop.x+vLeftBottom.x+vRightBottom.x) / 4;
		FLOAT avgY = (vLeftTop.y+vRightTop.y+vLeftBottom.y+vRightBottom.y) / 4;

		FLOAT vVeritces[4*6] =
		{
			vLeftTop.x + (vLeftTop.x>avgX?1.0f:0.0f), vLeftTop.y + (vLeftTop.y>avgY?1.0f:0.0f),				0.5f, 1.0f, 0.0f, 0.0f,
			vRightTop.x + (vRightTop.x>avgX?1.0f:0.0f), vRightTop.y + (vRightTop.y>avgY?1.0f:0.0f),			0.5f, 1.0f, 1.0f, 0.0f,
			vLeftBottom.x + (vLeftBottom.x>avgX?1.0f:0.0f), vLeftBottom.y + (vLeftBottom.y>avgY?1.0f:0.0f),		0.5f, 1.0f, 0.0f, 1.0f,
			vRightBottom.x + (vRightBottom.x>avgX?1.0f:0.0f), vRightBottom.y + (vRightBottom.y>avgY?1.0f:0.0f),		0.5f, 1.0f, 1.0f, 1.0f
		};
		pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT );
		pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vVeritces, sizeof(FLOAT)*6 );
	}

	{
		TMANTLECOORD& uv2 = pCoordInfo->m_vUV2[ k.m_bBack ];
		D3DXVECTOR2& vLeftTop = uv2.m_vLeftTop;
		D3DXVECTOR2& vLeftBottom = uv2.m_vLeftBottom;
		D3DXVECTOR2& vRightTop = uv2.m_vRightTop;
		D3DXVECTOR2& vRightBottom = uv2.m_vRightBottom;

		FLOAT avgX = (vLeftTop.x+vRightTop.x+vLeftBottom.x+vRightBottom.x) / 4;
		FLOAT avgY = (vLeftTop.y+vRightTop.y+vLeftBottom.y+vRightBottom.y) / 4;

		FLOAT vVeritces[4*6] =
		{
			vLeftTop.x + (vLeftTop.x>avgX?1.0f:0.0f), vLeftTop.y + (vLeftTop.y>avgY?1.0f:0.0f),				0.5f, 1.0f, 0.0f, 0.0f,
			vRightTop.x + (vRightTop.x>avgX?1.0f:0.0f), vRightTop.y + (vRightTop.y>avgY?1.0f:0.0f),			0.5f, 1.0f, 1.0f, 0.0f,
			vLeftBottom.x + (vLeftBottom.x>avgX?1.0f:0.0f), vLeftBottom.y + (vLeftBottom.y>avgY?1.0f:0.0f),		0.5f, 1.0f, 0.0f, 1.0f,
			vRightBottom.x + (vRightBottom.x>avgX?1.0f:0.0f), vRightBottom.y + (vRightBottom.y>avgY?1.0f:0.0f),		0.5f, 1.0f, 1.0f, 1.0f
		};
		pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT );
		pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vVeritces, sizeof(FLOAT)*6 );
	}

	{
		TMANTLECOORD& uv3 = pCoordInfo->m_vUV3[ k.m_bBack ];
		D3DXVECTOR2& vLeftTop = uv3.m_vLeftTop;
		D3DXVECTOR2& vLeftBottom = uv3.m_vLeftBottom;
		D3DXVECTOR2& vRightTop = uv3.m_vRightTop;
		D3DXVECTOR2& vRightBottom = uv3.m_vRightBottom;

		FLOAT avgX = (vLeftTop.x+vRightTop.x+vLeftBottom.x+vRightBottom.x) / 4;
		FLOAT avgY = (vLeftTop.y+vRightTop.y+vLeftBottom.y+vRightBottom.y) / 4;

		FLOAT vVeritces[4*6] =
		{
			vLeftTop.x + (vLeftTop.x>avgX?1.0f:0.0f), vLeftTop.y + (vLeftTop.y>avgY?1.0f:0.0f),				0.5f, 1.0f, 0.0f, 0.0f,
			vRightTop.x + (vRightTop.x>avgX?1.0f:0.0f), vRightTop.y + (vRightTop.y>avgY?1.0f:0.0f),			0.5f, 1.0f, 1.0f, 0.0f,
			vLeftBottom.x + (vLeftBottom.x>avgX?1.0f:0.0f), vLeftBottom.y + (vLeftBottom.y>avgY?1.0f:0.0f),		0.5f, 1.0f, 0.0f, 1.0f,
			vRightBottom.x + (vRightBottom.x>avgX?1.0f:0.0f), vRightBottom.y + (vRightBottom.y>avgY?1.0f:0.0f),		0.5f, 1.0f, 1.0f, 1.0f
		};
		pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, CTClientGuildMark::m_dwCOLOR[k.m_bFrontColor] );
		pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
		pDevice->SetTexture( 0, m_vFRONTTEX[k.m_bFront]->GetTexture());
		pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vVeritces, sizeof(FLOAT)*6 );
	}

	pDevice->SetRenderTarget(0, pBACKUPBUF);

	pBACKUPBUF->Release();
	pRNDBUF2->Release();

	CRect rt = CTClientUIBase::GetScreenRect();
	D3DVIEWPORT9 vOrgVP;
	vOrgVP.X = rt.left;
	vOrgVP.Y = rt.top;
	vOrgVP.Width = rt.right;
	vOrgVP.Height = rt.bottom;
	vOrgVP.MinZ = 0.0f;
	vOrgVP.MaxZ = 1.0f;
	pDevice->SetViewport( &vOrgVP );

	LPTEXTURESET pTextureSet = new TEXTURESET;
	CT3DTexture *pT3DTEX = new CT3DTexture();

	pT3DTEX->m_pTDATA = (LPBYTE) pRNDTEX2;
	pT3DTEX->m_bEnabled = TRUE;
	pTextureSet->PushTexturePtr(pT3DTEX);

	if( !bDeviceReset )
		m_mapTMANTLEBAKINGTEXTURE.insert( MAPTMANTLETEXTURE::value_type( k, pTextureSet ) );

	return pTextureSet;
}

void CTClientGuildMark::ReleaseTMantleBakingTexture()
{
	MAPTMANTLETEXTURE::iterator it, end;
	it = m_mapTMANTLEBAKINGTEXTURE.begin();
	end = m_mapTMANTLEBAKINGTEXTURE.end();

	for(; it != end ; ++it )
	{
		CT3DTexture *pTEX = it->second->GetTexturePtr(0);

		delete pTEX;
		delete it->second;
	}

	m_mapTMANTLEBAKINGTEXTURE.clear();
}