#include "stdafx.h"
#include "MainFrm.h"
#include "TFontTestView.h"

IMPLEMENT_DYNCREATE(CTFontTestView, CScrollView)

CTFontTestView::CTFontTestView()
: m_pGDIFont(NULL)
, m_rtPos( CRect(0,0,0,0 ) )
, m_strText(_T("Test"))
{

}

CTFontTestView::~CTFontTestView()
{
	if ( m_pGDIFont )
	{
		if ( m_pGDIFont->m_pFont )
		{
			delete m_pGDIFont->m_pFont;
			m_pGDIFont->m_pFont = NULL;
		}

		delete m_pGDIFont;
		m_pGDIFont = NULL;
	}

	m_Font.Release();
}

BEGIN_MESSAGE_MAP(CTFontTestView, CScrollView)
END_MESSAGE_MAP()

void CTFontTestView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	memset(&m_lf, 0x00, sizeof(LOGFONT));

	m_lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lf.lfOutPrecision = OUT_CHARACTER_PRECIS;
	m_lf.lfPitchAndFamily = FF_DONTCARE;
	m_lf.lfQuality = ANTIALIASED_QUALITY;	// default value is ANTIALIASED_QUALITY.
	m_lf.lfCharSet = DEFAULT_CHARSET;

	strcpy(m_lf.lfFaceName, _T("Arial"));

	CClientDC dc(this);
	m_lf.lfHeight = -MulDiv(9, dc.GetDeviceCaps(LOGPIXELSY), 72);

	m_pGDIFont = new CGDIFont();
	m_pGDIFont->m_pFont = new CFont;
	m_pGDIFont->m_pFont->CreateFontIndirect(&m_lf);

	m_Font.SetFont( m_pGDIFont->m_pFont );
}

void CTFontTestView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	// TODO: 여기에 그리기 코드를 추가합니다.
	Render();
}


#ifdef _DEBUG
void CTFontTestView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTFontTestView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

void CTFontTestView::Render()
{
	if( !IsWindowVisible() || !CMainFrame::m_bRender )
		return;

	D3DXMATRIX vWorld;
	CRect rect;

	D3DXMatrixIdentity(&vWorld);
	GetClientRect(&rect);

	rect.OffsetRect(
		(CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
		(CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

	CMainFrame::m_3DDevice.m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET|
		D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(
		0,
		0,
		0),
		1.0f, 0);

	CMainFrame::m_3DDevice.m_pDevice->BeginScene();

	//CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	//CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	//CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	//CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	//CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	//CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	//CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);
	/*CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_TEXTURE0, &vWorld);
	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vWorld);*/

	DrawContent();

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->EndScene();

	HWND hWND = GetSafeHwnd();
	if(hWND)
	{
		HRESULT hr = CMainFrame::m_pSWAPCHAIN->Present(
			&rect,
			NULL,
			hWND,
			NULL, 0);

		if(FAILED(hr))
			CMainFrame::RestoreDevice();
	}
}

void CTFontTestView::DrawContent()
{
	CRect rect;
	GetClientRect(&rect);

	rect.OffsetRect(
		(CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
		(CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

	m_Img.Render( CMainFrame::m_3DDevice.m_pDevice, D3DCOLOR_ARGB( 0xFF, 0xFF, 0xFF, 0xFF ), rect.left, rect.top, 0 );

	rect.left += m_rtPos.left;
	rect.top += m_rtPos.top;

	m_Font.TextOut( CMainFrame::m_3DDevice.m_pDevice, m_strText, rect, DT_CALCRECT );
}

void CTFontTestView::SetFont( LOGFONT* of, BOOL bOutline, BOOL bShadow, DWORD dwColor, INT nFontRender, CRect rt  )
{
	strcpy( m_lf.lfFaceName, of->lfFaceName );
	m_lf.lfHeight = of->lfHeight * 100 / 100;

	m_lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lf.lfOutPrecision = OUT_CHARACTER_PRECIS;
	m_lf.lfPitchAndFamily = FF_DONTCARE;
	m_lf.lfQuality = ANTIALIASED_QUALITY;	// default value is ANTIALIASED_QUALITY.
	m_lf.lfCharSet = DEFAULT_CHARSET;

    if ( m_pGDIFont )
	{
		if ( m_pGDIFont->m_pFont )
		{
			delete m_pGDIFont->m_pFont;
			m_pGDIFont->m_pFont = NULL;
		}
			
		delete m_pGDIFont;
		m_pGDIFont = NULL;
	}

	m_pGDIFont = new CGDIFont();
	m_pGDIFont->m_pFont = new CFont;
	m_pGDIFont->m_pFont->CreateFontIndirect(&m_lf);

	CD3DFont::m_nRenderingHint = nFontRender;

	m_Font.m_bOutLine = bOutline;
	m_Font.m_bShadow = bShadow;
	m_Font.m_dwColor = dwColor;
	m_Font.SetFont( m_pGDIFont->m_pFont );

	m_rtPos = rt;

	CRect rect;
	GetClientRect(&rect);

	rect.OffsetRect(
		(CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
		(CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

	m_Img.Render( CMainFrame::m_3DDevice.m_pDevice, D3DCOLOR_ARGB( 0xFF, 0xFF, 0xFF, 0xFF ), rect.left, rect.top, 0 );

	rect.left += m_rtPos.left;
	rect.top += m_rtPos.top;

	m_Font.TextOut( CMainFrame::m_3DDevice.m_pDevice, m_strText, rect, DT_CALCRECT|DT_CENTER);
	
}

void CTFontTestView::SetText( CString str )
{
	m_strText = str;
}

void CTFontTestView::LoadImage( CString str )
{
	m_Img.Release();
	CMainFrame::LoadImageFile( str, &m_Img );
}