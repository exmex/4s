#include "StdAfx.h"
#include <afxocc.h>
#include "TOccManager.h"
#include "THtmlDocWnd.h"

#define BUF_SIZE 1024

DWORD THttpCtrl::HTTP_REDRAW_TIMER_IDENTITY = 0;
LPDIRECT3DTEXTURE9 THttpCtrl::m_pTIMG = NULL;
CSize THttpCtrl::m_vTBUFSIZE( 0, 0);
BYTE THttpCtrl::m_bRedrawHTTP = FALSE;
CWnd THttpCtrl::m_vHTTP;
BOOL THttpCtrl::m_bUnDXRender = FALSE;

THttpCtrl::THttpCtrl( TComponent* pParent, LP_FRAMEDESC pDesc)
:TComponent( pParent, pDesc)
{

	m_bHitTestType = HITTEST_TYPE_RECT;
	m_bType = TCML_TYPE_HTTPCTRL;
	m_style |= TS_FOCUSABLE;

	m_strURL.Empty();
	m_pHTTP = NULL;

	m_bLButtonDown = FALSE;
	m_bCaret = FALSE;
	m_dwTick = 0;
}

THttpCtrl::~THttpCtrl()
{
	ReleaseHTTP();
}

void THttpCtrl::UpdateTIMGBUF()
{
	CheckBUF();

	if( !m_pTIMG )
		return;

	CDC* pDC = m_vHTTP.GetDC();
	HDC hdc = pDC->GetSafeHdc();
	HDC hMemDC = CreateCompatibleDC(hdc);

	/*BITMAPFILEHEADER fileHeader;
	fileHeader.bfType = 0x4d42;
	fileHeader.bfSize = 0;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);*/

	int bitmap_dx= BUF_SIZE;
	int bitmap_dy = BUF_SIZE;

	BITMAPINFOHEADER infoHeader;
	infoHeader.biSize = sizeof(infoHeader);
	infoHeader.biWidth = bitmap_dx;
	infoHeader.biHeight = -bitmap_dy;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 32;
	infoHeader.biCompression = BI_RGB;
	infoHeader.biSizeImage = 0;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant  = 0;

	BITMAPINFO info;
	info.bmiHeader = infoHeader; 
	LPBYTE bits = NULL;
	HBITMAP hBitmap = CreateDIBSection(
		hdc,
		&info,
		DIB_RGB_COLORS,
		(void**) &bits,
		0, 0);

	HBITMAP hOldBitmap = (HBITMAP) ::SelectObject( hMemDC, hBitmap );
	BitBlt(
		hMemDC,
		0, 0,
		bitmap_dx, bitmap_dy,
		hdc,
		0, 0,
		SRCCOPY);

	/*int bytes = (((32*bitmap_dx + 31) & (~31))/8)*bitmap_dy;
	FILE* fp = fopen("test.bmp", "wb");
	if( fp )
	{
		fwrite( &fileHeader, sizeof(fileHeader), 1, fp );
		fwrite( &infoHeader, sizeof(infoHeader), 1, fp );
		fwrite( bits, bytes, 1, fp );
		fclose(fp);
	}*/

	D3DLOCKED_RECT lr;
	if( D3D_OK == m_pTIMG->LockRect(0, &lr, NULL, D3DLOCK_DISCARD) )
	{
		LPBYTE pSource = bits;
		LPBYTE pDest = (LPBYTE) lr.pBits;

		for( int y=0 ; y < m_vTBUFSIZE.cy ; ++y )
		{
			DWORD dwPitch = sizeof(DWORD)*m_vTBUFSIZE.cx;

			memcpy(
				pDest,
				pSource,
				dwPitch );

			pSource += dwPitch;
			pDest += lr.Pitch;
		}

		m_pTIMG->UnlockRect(0);
	}

	/*D3DXSaveTextureToFile(
		"final.bmp",
		D3DXIFF_BMP,
		m_pTIMG,
		NULL );*/

	::DeleteObject( ::SelectObject( hMemDC, hOldBitmap ) );
	::DeleteDC( hMemDC );

	m_vHTTP.ReleaseDC( pDC );
}

void THttpCtrl::CheckBUF()
{
	if(!m_pTIMG)
	{
		m_vTBUFSIZE.cx = BUF_SIZE;
		m_vTBUFSIZE.cy = BUF_SIZE;

		CTachyonRes::m_pDEVICE->m_pDevice->CreateTexture(
			m_vTBUFSIZE.cx, m_vTBUFSIZE.cy, 1, 0,
			D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
			&m_pTIMG, NULL );
	}
}

void THttpCtrl::ReleaseBUF()
{
	if(m_pTIMG)
	{
		m_pTIMG->Release();
		m_pTIMG = NULL;
	}

	m_vTBUFSIZE.cx = 0;
	m_vTBUFSIZE.cy = 0;
}

BYTE THttpCtrl::IsFocusedHTML( HWND hWND)
{
	CWnd *pWND = CWnd::FromHandlePermanent(hWND);

	return !pWND || !pWND->IsWindowVisible() ||
		!pWND->IsKindOf(RUNTIME_CLASS(THtmlDocWnd)) ||
		!((THtmlDocWnd *) pWND)->CanFocus() ? FALSE : TRUE;
}

BYTE THttpCtrl::Navigate( LPCTSTR szURL)
{
	if( (!m_vHTTP.GetSafeHwnd() || !m_pHTTP) && !InitHTTP() )
		return FALSE;

	CString strURL(szURL);
	CComBSTR vURL;
	COleSafeArray vPOST;

	m_strURL = strURL;
	vURL.Attach(strURL.AllocSysString());

	m_pHTTP->Navigate(
		vURL,
		COleVariant( (long) NULL, VT_I4),
		COleVariant( (char *) NULL, VT_BSTR),
		vPOST,
		COleVariant( (char *) NULL, VT_BSTR));

	return TRUE;
}

BYTE THttpCtrl::InitHTTP()
{
	TOccManager vTOCC;
	CRect rect = m_rc;

	ComponentToScreen(&rect);
	ReleaseHTTP();
	vTOCC.m_pTHOST = this;
	AfxEnableControlContainer(&vTOCC);

	if(!m_vHTTP.CreateControl( CLSID_WebBrowser, NULL, WS_CHILD|WS_VISIBLE, rect, AfxGetMainWnd(), m_id))
	{
		AfxEnableControlContainer();
		return FALSE;
	}

	AfxEnableControlContainer();
	LPUNKNOWN pCOM = m_vHTTP.GetControlUnknown();

	if(!SUCCEEDED(pCOM->QueryInterface( IID_IWebBrowser2, (LPVOID *) &m_pHTTP)))
	{
		m_vHTTP.DestroyWindow();
		return FALSE;
	}

	m_pHTTP->put_Silent(TRUE);
	
	m_bCaret = FALSE;
	m_dwTick = 0;

	return TRUE;
}

void THttpCtrl::ReleaseHTTP()
{
	if(m_pHTTP)
	{
		m_pHTTP->Release();
		m_pHTTP = NULL;
	}

	if(m_vHTTP.GetSafeHwnd())
	{
		CWnd *pMainWnd = AfxGetMainWnd();

		if(pMainWnd)
			pMainWnd->SetFocus();

		m_vHTTP.DestroyWindow();
	}

	m_bCaret = FALSE;
	m_dwTick = 0;
}

BYTE THttpCtrl::HaveCaret( CRect *pRECT)
{
	CWnd *pWND = CWnd::GetFocus();

	if( !m_pHTTP || !m_vHTTP.GetSafeHwnd() ||
		!m_vHTTP.IsWindowVisible() ||
		!pWND || !m_vHTTP.IsChild(pWND) )
		return FALSE;

	if(pRECT)
		pRECT->SetRectEmpty();

	HIMC hIME = ImmGetContext(pWND->GetSafeHwnd());
	if(hIME)
	{
		WORD wBUF;

		LONG nCount = ImmGetCompositionString(
			hIME,
			GCS_COMPSTR,
			&wBUF,
			sizeof(WORD));

		ImmReleaseContext( pWND->GetSafeHwnd(), hIME);
		if( nCount > 0 )
			return TRUE;
	}

	IHTMLDocument2 *pDOC = NULL;
	IDispatch *pUk = NULL;

	if( FAILED(m_pHTTP->get_Document(&pUk)) || !pUk )
		return FALSE;

	if( FAILED(pUk->QueryInterface( IID_IHTMLDocument2, (LPVOID *) &pDOC)) || !pDOC )
	{
		pUk->Release();
		return FALSE;
	}

	IDisplayServices *pDS = NULL;
	IHTMLCaret *pCaret = NULL;
	BOOL bIsVisible = FALSE;

	if( FAILED(pDOC->QueryInterface( IID_IDisplayServices, (LPVOID *) &pDS)) || !pDS )
	{
		pDOC->Release();
		pUk->Release();

		return FALSE;
	}

	if( FAILED(pDS->GetCaret(&pCaret)) || !pCaret ||
		FAILED(pCaret->IsVisible(&bIsVisible)) || !bIsVisible )
	{
		if(pCaret)
			pCaret->Release();

		pDS->Release();
		pDOC->Release();
		pUk->Release();

		return FALSE;
	}

	if(pRECT)
	{
		CWnd *pMAIN = AfxGetMainWnd();
		CPoint point;

		if( pMAIN && SUCCEEDED(pCaret->GetLocation( &point, TRUE)) )
		{
			pWND->ClientToScreen(&point);
			pMAIN->ScreenToClient(&point);

			pRECT->SetRect(
				0, 0,
				TCARET_WIDTH,
				TCARET_HEIGHT);

			pRECT->OffsetRect(
				point.x,
				point.y);
		}
	}

	pCaret->Release();
	pDS->Release();
	pDOC->Release();
	pUk->Release();

	return TRUE;
}

CWnd *THttpCtrl::GetChildFromPoint( CPoint point)
{
	CWnd *pMAIN = AfxGetMainWnd();

	if( !pMAIN || !m_vHTTP.GetSafeHwnd() || !m_vHTTP.IsWindowVisible() )
		return NULL;
	CRect rect = m_rc;

	ComponentToScreen(&rect);
	pMAIN->ClientToScreen(&rect);
	pMAIN->ClientToScreen(&point);

	if(!rect.PtInRect(point))
		return NULL;

	CWnd *pPARENT = &m_vHTTP;
	while(pPARENT)
	{
		pPARENT->ScreenToClient(&point);

		pMAIN = pPARENT->ChildWindowFromPoint(
			point,
			CWP_SKIPDISABLED|
			CWP_SKIPINVISIBLE|
			CWP_SKIPTRANSPARENT);

		if( pMAIN == &m_vHTTP )
			return NULL;

		if( pMAIN == pPARENT )
			return pMAIN->IsKindOf(RUNTIME_CLASS(THtmlDocWnd)) ? pMAIN : NULL;

		pPARENT->ClientToScreen(&point);
		pPARENT = pMAIN;
	}

	return NULL;
}

BYTE THttpCtrl::OnMouseMSG( UINT nMSG,
						    UINT nFlags,
							CPoint point)
{
	CWnd *pMAIN = AfxGetMainWnd();
	CWnd *pWND = GetChildFromPoint(point);

	if( !pWND || !pMAIN )
		return FALSE;

	pMAIN->ClientToScreen(&point);
	pWND->ScreenToClient(&point);

	CallWindowProc(
		(WNDPROC) ((THtmlDocWnd *) pWND)->m_pWINPROC,
		pWND->GetSafeHwnd(),
		nMSG,
		nFlags,
		MAKELONG( point.x, point.y));

	return TRUE;
}

struct FVF_XYZ_UV
{
	FLOAT x, y, z, u, v;
	static DWORD GetFVF()
	{
		return D3DFVF_XYZ | D3DFVF_TEX1;
	}
};

HRESULT THttpCtrl::Render( DWORD dwTickCount)
{
	if( THttpCtrl::m_bUnDXRender )
	{
		return S_OK;
	}


	if(!m_bVisible)
		return S_OK;

	if(!CTachyonRes::m_pDEVICE)
		return E_FAIL;

	LPDIRECT3DDEVICE9 pDevice = CTachyonRes::m_pDEVICE->m_pDevice;
	DWORD dwCaret = GetCaretBlinkTime();

	m_dwTick += dwTickCount;

	if( dwCaret < TCARET_TICK )
		dwCaret = TCARET_TICK;

	if( m_dwTick > dwCaret )
	{
		m_bCaret = !m_bCaret;
		m_dwTick %= dwCaret;
	}

	if( m_vHTTP.GetSafeHwnd() && m_pHTTP && m_pTIMG )
	{
		CRect vHTTP = m_rc;
		CRect vBUF(
			0, 0,
			m_vTBUFSIZE.cx,
			m_vTBUFSIZE.cy);
		CRect vTCARET;

		ComponentToScreen(&vHTTP);
		HaveCaret(&vTCARET);

		FLOAT w = (FLOAT) CTachyonRes::m_pDEVICE->m_option.m_dwScreenX;
		FLOAT h = (FLOAT) CTachyonRes::m_pDEVICE->m_option.m_dwScreenY;

		if( m_pTIMG )
		{
			CRect rcHTTP = m_rc;
			ComponentToScreen( &rcHTTP );

			FLOAT u = rcHTTP.Width() / (FLOAT)(BUF_SIZE);
			FLOAT v = rcHTTP.Height() / (FLOAT)(BUF_SIZE);

			FVF_XYZ_UV vVertices[4] =
			{
				{ 2.0f*(rcHTTP.left-0.5f) / w - 1.0f, -2.0f*(rcHTTP.top-0.5f) / h + 1.0f, 0.5f, 0.0f, 0.0f },
				{ 2.0f*(rcHTTP.right-0.5f) / w - 1.0f, -2.0f*(rcHTTP.top-0.5f) / h + 1.0f, 0.5f, u, 0.0f, },
				{ 2.0f*(rcHTTP.left-0.5f) / w - 1.0f, -2.0f*(rcHTTP.bottom-0.5f) / h + 1.0f, 0.5f, 0.0f, v },
				{ 2.0f*(rcHTTP.right-0.5f) / w - 1.0f, -2.0f*(rcHTTP.bottom-0.5f) / h + 1.0f, 0.5f, u, v }
			};

			D3DXMATRIX matIdentity;
			D3DXMatrixIdentity( &matIdentity );
			pDevice->SetTransform( D3DTS_WORLD, &matIdentity );
			pDevice->SetTransform( D3DTS_VIEW, &matIdentity );
			pDevice->SetTransform( D3DTS_PROJECTION, &matIdentity );

			pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
			pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
			pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
			pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
			pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			pDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
			pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

			pDevice->SetFVF( FVF_XYZ_UV::GetFVF() );
			pDevice->SetTexture( 0, m_pTIMG );
			pDevice->SetTexture( 1, NULL );

			pDevice->DrawPrimitiveUP(
				D3DPT_TRIANGLESTRIP,
				2,
				vVertices,
				sizeof(FVF_XYZ_UV) );
		}

		if( m_bCaret && !vTCARET.IsRectEmpty() )
		{
			FVF_XYZ_UV vVertices[4] =
			{
				{ 2.0f*(vTCARET.left-0.5f) / w - 1.0f, -2.0f*(vTCARET.top-0.5f) / h + 1.0f, 0.5f, 0.0f, 0.0f },
				{ 2.0f*(vTCARET.right-0.5f) / w - 1.0f, -2.0f*(vTCARET.top-0.5f) / h + 1.0f, 0.5f, 0.0f, 0.0f, },
				{ 2.0f*(vTCARET.left-0.5f) / w - 1.0f, -2.0f*(vTCARET.bottom-0.5f) / h + 1.0f, 0.5f, 0.0f, 0.0f },
				{ 2.0f*(vTCARET.right-0.5f) / w - 1.0f, -2.0f*(vTCARET.bottom-0.5f) / h + 1.0f, 0.5f, 0.0f, 0.0f }
			};
			
			pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
			pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
			pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
			pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB( 0xFF, 0x30, 0x30, 0x30) );
			pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
			pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
			pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			pDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
			pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

			pDevice->SetFVF( FVF_XYZ_UV::GetFVF() );
			pDevice->SetTexture( 0, NULL );
			pDevice->SetTexture( 1, NULL );

			pDevice->DrawPrimitiveUP(
				D3DPT_TRIANGLESTRIP,
				2,
				vVertices,
				sizeof(FVF_XYZ_UV) );
		}
	}

	return TComponent::Render(dwTickCount);
}

void THttpCtrl::ShowComponent( BOOL bVisible)
{
	TComponent::ShowComponent(bVisible);

	if(bVisible)
		Navigate(LPCTSTR(m_strURL));
	else
	{
		ReleaseHTTP();
		ReleaseBUF();
	}
}

BOOL THttpCtrl::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message, CPoint pt)
{
	CWnd *pWND = GetChildFromPoint(pt);

	if(!pWND)
		return FALSE;

	CallWindowProc(
		(WNDPROC) ((THtmlDocWnd *) pWND)->m_pWINPROC,
		pWND->GetSafeHwnd(),
		WM_SETCURSOR,
		(WPARAM) (pWnd ? pWnd->GetSafeHwnd() : NULL),
		(LPARAM) MAKELONG( nHitTest, message));

	return TRUE;
}

BOOL THttpCtrl::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	CWnd *pWND = GetChildFromPoint(pt);
	CWnd *pMAIN = AfxGetMainWnd();
	CRect rc = m_rc;

	ComponentToScreen(&rc);
	if( !pWND || !pMAIN ||
		!rc.PtInRect(pt) )
		return FALSE;

	pMAIN->ClientToScreen(&pt);
	CallWindowProc(
		(WNDPROC) ((THtmlDocWnd *) pWND)->m_pWINPROC,
		pWND->GetSafeHwnd(),
		WM_MOUSEWHEEL,
		(WPARAM) MAKELONG( nFlags, zDelta),
		MAKELONG( pt.x, pt.y));

	return TRUE;
}

void THttpCtrl::OnLButtonDblClk( UINT nFlags, CPoint pt)
{
	OnMouseMSG( WM_LBUTTONDBLCLK, nFlags, pt);
	TComponent::OnLButtonDblClk( nFlags, pt);
}

void THttpCtrl::OnMButtonDblClk( UINT nFlags, CPoint pt)
{
	OnMouseMSG( WM_MBUTTONDBLCLK, nFlags, pt);
	TComponent::OnMButtonDblClk( nFlags, pt);
}

void THttpCtrl::OnRButtonDblClk( UINT nFlags, CPoint pt)
{
	TComponent::OnRButtonDblClk( nFlags, pt);
}

void THttpCtrl::OnLButtonDown( UINT nFlags, CPoint pt)
{
	m_bLButtonDown = TRUE;
	OnMouseMSG( WM_LBUTTONDOWN, nFlags, pt);
	TComponent::OnLButtonDown( nFlags, pt);
}

void THttpCtrl::OnMButtonDown( UINT nFlags, CPoint pt)
{
	OnMouseMSG( WM_MBUTTONDOWN, nFlags, pt);
	TComponent::OnMButtonDown( nFlags, pt);
}

void THttpCtrl::OnRButtonDown( UINT nFlags, CPoint pt)
{
	TComponent::OnRButtonDown( nFlags, pt);
}

void THttpCtrl::OnLButtonUp( UINT nFlags, CPoint pt)
{
	m_bLButtonDown = FALSE;
	OnMouseMSG( WM_LBUTTONUP, nFlags, pt);
	TComponent::OnLButtonUp( nFlags, pt);
}

void THttpCtrl::OnMButtonUp( UINT nFlags, CPoint pt)
{
	OnMouseMSG( WM_MBUTTONUP, nFlags, pt);
	TComponent::OnMButtonUp( nFlags, pt);
}

void THttpCtrl::OnRButtonUp( UINT nFlags, CPoint pt)
{
	TComponent::OnRButtonUp( nFlags, pt);
}

void THttpCtrl::OnMouseMove( UINT nFlags, CPoint pt)
{
	if(!m_bLButtonDown)
		OnMouseMSG( WM_MOUSEMOVE, nFlags, pt);
	TComponent::OnMouseMove( nFlags, pt);
}

void THttpCtrl::OnChar( UINT nChar, int nRepCnt, UINT nFlags)
{
	CWnd *pWND = CWnd::GetFocus();

	if( !m_pHTTP || !m_vHTTP.GetSafeHwnd() ||
		!m_vHTTP.IsWindowVisible() ||
		!pWND || !m_vHTTP.IsChild(pWND) )
		return;

	IHTMLDocument2 *pDOC = NULL;
	IDispatch *pUk = NULL;

	if( FAILED(m_pHTTP->get_Document(&pUk)) || !pUk )
		return;

	if( FAILED(pUk->QueryInterface( IID_IHTMLDocument2, (LPVOID *) &pDOC)) || !pDOC )
	{
		pUk->Release();
		return;
	}

	IDisplayServices *pDS = NULL;
	IHTMLCaret *pCaret = NULL;
	BOOL bIsVisible = FALSE;

	if( FAILED(pDOC->QueryInterface( IID_IDisplayServices, (LPVOID *) &pDS)) || !pDS )
	{
		pDOC->Release();
		pUk->Release();

		return;
	}

	if( FAILED(pDS->GetCaret(&pCaret)) || !pCaret ||
		FAILED(pCaret->IsVisible(&bIsVisible)) || !bIsVisible )
	{
		if(pCaret)
			pCaret->Release();

		pDS->Release();
		pDOC->Release();
		pUk->Release();

		return;
	}

	WORD wTLANG = LOWORD(GetKeyboardLayout(0));
	BYTE vTBUFF[7];

	GetLocaleInfo(
		MAKELCID( wTLANG, SORT_DEFAULT),
		LOCALE_IDEFAULTANSICODEPAGE,
		(LPSTR) vTBUFF, 7);
	int nTCP = atoi((LPCSTR) vTBUFF);

	MultiByteToWideChar(
		nTCP, 0,
		(LPCSTR) &nChar,
		sizeof(UINT),
		&wTLANG, 1);

	for( int i=0; i<nRepCnt; i++)
		pCaret->InsertText( &wTLANG, 1);

	pCaret->Release();
	pDS->Release();
	pDOC->Release();
	pUk->Release();
}

void THttpCtrl::OnLoseFocus( TComponent *pSetFocus)
{
	CWnd *pMainWnd = AfxGetMainWnd();

	if( pMainWnd && m_vHTTP.GetSafeHwnd() )
	{
		COleControlContainer *pOLE = pMainWnd->GetControlContainer();

		if(pOLE)
			pOLE->OnUIActivate(NULL);

		pMainWnd->SetFocus();
	}
}

void THttpCtrl::OnSetFocus( TComponent *pLoseFocus)
{
	CWnd *pMainWnd = AfxGetMainWnd();

	if( pMainWnd && m_vHTTP.GetSafeHwnd() )
	{
		COleControlSite *pOLE = pMainWnd->GetOleControlSite(m_id);
		pOLE->SetFocus();
	}
}

void THttpCtrl::MoveWndHTTP()
{
	CRect rtHTTP = m_rc;
	ComponentToScreen(&rtHTTP);

	m_vHTTP.MoveWindow(
		&rtHTTP,
		FALSE);
}

void THttpCtrl::Redraw()
{
	if( m_vHTTP.GetSafeHwnd() != NULL )
	{
		m_vHTTP.InvalidateRect( NULL, FALSE );

		EnumChildWindows(
			m_vHTTP.GetSafeHwnd(),
			THtmlDocWnd::__RedrawWnd,
			NULL);
	}
}

#undef BUF_SIZE