// ImageView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "ImageView.h"
#include "MainFrm.h"
#include ".\imageview.h"


// CImageView

IMPLEMENT_DYNCREATE(CImageView, CScrollView)

CImageView::CImageView()
{
}

CImageView::~CImageView()
{
}


BEGIN_MESSAGE_MAP(CImageView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CImageView 그리기입니다.

void CImageView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	ResetScrollSize();
}

void CImageView::ResetScrollSize()
{
	CImageDoc *pDoc = GetDocument();

	if( pDoc && pDoc->m_pItem )
	{
		int nCount = INT(pDoc->m_pItem->m_ImageSet.m_vImage.size());
		CSize szTotal( 1, 1);

		for( int i=0; i<nCount; i++)
		{
			CD3DImage *pImage = pDoc->m_pItem->m_ImageSet.m_vImage[i];

			if(pImage)
			{
				if( szTotal.cx < pImage->GetWidth() )
					szTotal.cx = pImage->GetWidth();

				if( szTotal.cy < pImage->GetHeight() )
					szTotal.cy = pImage->GetHeight();
			}
		}

		if( GetTotalSize() != szTotal )
			SetScrollSizes( MM_TEXT, szTotal);
	}
}

void CImageView::OnDraw(CDC* pDC)
{
	Render();
}


// CImageView 진단입니다.

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


// CImageView 메시지 처리기입니다.

void CImageView::Render()
{
	CImageDoc *pDoc = GetDocument();

	if( pDoc && CMainFrame::m_bRender )
	{
		ResetScrollSize();

		CMainFrame::m_3DDevice.m_pDevice->Clear(
			0, NULL,
			D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(
			GetRValue(pDoc->m_pItem->m_dwBackColor),
			GetGValue(pDoc->m_pItem->m_dwBackColor),
			GetBValue(pDoc->m_pItem->m_dwBackColor)),
			1.0f, 0);
		CMainFrame::m_3DDevice.m_pDevice->BeginScene();

		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		pDoc->m_pItem->m_ImageSet.m_dwCurTick = pDoc->m_dwTick;
		CD3DImage *pImage = pDoc->m_pItem->m_ImageSet.GetImage();

		if(pImage)
		{
			pImage->Render(
				CMainFrame::m_3DDevice.m_pDevice, pDoc->m_pItem->m_ImageSet.GetColor(),
				-GetScrollPos(SB_HORZ),
				-GetScrollPos(SB_VERT));
		}
		CMainFrame::m_3DDevice.m_pDevice->EndScene();

		CRect rect;
		GetClientRect(&rect);

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
}

BOOL CImageView::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

void CImageView::OnDestroy()
{
	CScrollView::OnDestroy();
}

void CImageView::DeleteForRestore()
{
}

void CImageView::Restore()
{
}
