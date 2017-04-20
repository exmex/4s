// MediaView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "BarContent.h"
#include "MediaView.h"
#include "MediaDlg.h"
#include "MainFrm.h"


// CMediaView

IMPLEMENT_DYNCREATE(CMediaView, CView)

CMediaView::CMediaView()
{
}

CMediaView::~CMediaView()
{
}

BEGIN_MESSAGE_MAP(CMediaView, CView)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMediaView 그리기입니다.

void CMediaView::OnDraw( CDC* pDC)
{
}


// CMediaView 진단입니다.

#ifdef _DEBUG
void CMediaView::AssertValid() const
{
	CView::AssertValid();
}

void CMediaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CMediaView 메시지 처리기입니다.

void CMediaView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	CMediaDoc *pDoc = GetDocument();

	if( pDoc && pDoc->m_pItem )
	{
		CMediaItem *pItem = pDoc->m_pItem;

		switch(pItem->GetPlayerType())
		{
		case MEDIA_TSOUND	:
			{
				CD3DSound *pOBJ = (CD3DSound *) CMainFrame::m_TachyonMedia.GetData(
					MEDIA_TSOUND,
					pItem->m_dwID);

				if(pOBJ)
				{
					pDoc->m_nIndex = pOBJ->Lock();
					LPDIRECTSOUND3DBUFFER p3DBUF = pOBJ->GetDS3D(pDoc->m_nIndex);

					if(p3DBUF)
						p3DBUF->SetPosition( 0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);
					pOBJ->m_bVolume = pItem->m_bVolume;
				}
			}

			break;

		case MEDIA_TSHOW	:
			{
				CTShow *pOBJ = (CTShow *) CMainFrame::m_TachyonMedia.GetData(
					MEDIA_TSHOW,
					pItem->m_dwID);

				if(pOBJ)
				{
					pOBJ->InitDShow(
						pDoc->GetMediaDlg(),
						this,
						pItem->m_szFileName,
						WM_MEDIA_MSG);

					CMainFrame::m_TachyonMedia.OnWindowPosition(
						MEDIA_TSHOW,
						pItem->m_dwID);
					pOBJ->m_bVolume = pItem->m_bVolume;
				}
			}

			break;
		}

		CMainFrame::m_TachyonMedia.ResetVolume(
			pItem->GetPlayerType(),
			pItem->m_dwID);
		pDoc->GetMediaDlg()->SetVolume(pItem->m_bVolume);
	}
}

void CMediaView::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CView::OnWindowPosChanged(lpwndpos);
	CMediaDoc *pDoc = GetDocument();

	if( GetSafeHwnd() && pDoc && pDoc->m_pItem )
	{
		CMediaItem *pItem = pDoc->m_pItem;

		CMainFrame::m_TachyonMedia.OnWindowPosition(
			MEDIA_TSHOW,
			pItem->m_dwID);
	}
}

void CMediaView::OnDestroy()
{
	CMediaDoc *pDoc = GetDocument();

	CMainFrame::m_TachyonMedia.Stop(
		pDoc->m_pItem->GetPlayerType(),
		pDoc->m_pItem->m_dwID,
		pDoc->m_nIndex);

	switch(pDoc->m_pItem->GetPlayerType())
	{
	case MEDIA_TSOUND	:
		{
			CD3DSound *pOBJ = (CD3DSound *) CMainFrame::m_TachyonMedia.GetData(
				MEDIA_TSOUND,
				pDoc->m_pItem->m_dwID);

			if(pOBJ)
				pOBJ->Unlock(pDoc->m_nIndex);
		}

		break;

	case MEDIA_TSHOW	:
		{
			CTShow *pOBJ = (CTShow *) CMainFrame::m_TachyonMedia.GetData(
				MEDIA_TSHOW,
				pDoc->m_pItem->m_dwID);

			if(pOBJ)
				pOBJ->InitDShow( NULL, NULL, pDoc->m_pItem->m_szFileName, 0);
		}

		break;
	}

	CView::OnDestroy();
}
