// TextureDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "TextureFrame.h"
#include "TextureView.h"
#include "TextureDoc.h"
#include "MainFrm.h"

// CTextureDoc

IMPLEMENT_DYNCREATE(CTextureDoc, CTachyonDoc)

CTextureDoc::CTextureDoc()
{
	m_bDocType = ITEM_TEXTURE;
	m_pItem = NULL;
	m_dwTick = 0;

	D3DXMatrixIdentity(&m_matObject);
	m_bApplyMode = APPLY_NONE;
	m_bRenderType = RT_SPHERE;
}

BOOL CTextureDoc::OnNewDocument()
{
	if (!CTachyonDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CTextureDoc::~CTextureDoc()
{
	if(m_pItem)
	{
		m_pItem->SetMainDocument(NULL);
		m_pItem = NULL;
	}
}


BEGIN_MESSAGE_MAP(CTextureDoc, CTachyonDoc)
END_MESSAGE_MAP()


// CTextureDoc 진단입니다.

#ifdef _DEBUG
void CTextureDoc::AssertValid() const
{
	CTachyonDoc::AssertValid();
}

void CTextureDoc::Dump(CDumpContext& dc) const
{
	CTachyonDoc::Dump(dc);
}
#endif //_DEBUG


// CTextureDoc serialization입니다.

void CTextureDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}


// CTextureDoc 명령입니다.

CPlayCtrl *CTextureDoc::GetPlayCtrl()
{
	CTextureFrame *pFrame = (CTextureFrame *) GetFrameWnd();
	return pFrame ? pFrame->m_wndPlayCtrlBar.GetPlayCtrl() : NULL;
}

CTextureBar *CTextureDoc::GetTextureBar()
{
	CTextureFrame *pFrame = (CTextureFrame *) GetFrameWnd();
	return pFrame ? &pFrame->m_wndTextureBar : NULL;
}

void CTextureDoc::InitData( LPVOID pData)
{
	CTachyonDoc::InitData(pData);
	if(!pData)
		return;

	m_pItem = (CTextureItem *) pData;
	m_pItem->SetMainDocument(this);

	Update();
}

void CTextureDoc::Update()
{
	if(m_pItem)
	{
		CTextureBar *pControl = GetTextureBar();

		SetTitle(m_pItem->m_strName);
		m_pItem->Regulate();

		if(pControl)
			pControl->SetItem(m_pItem);

		InitDraw();
	}
}

void CTextureDoc::InitDraw()
{
	POSITION pos = GetFirstViewPosition();

	while(pos)
	{
		CView *pView = GetNextView(pos);

		if( pView && pView->IsKindOf(RUNTIME_CLASS(CTextureView)) )
			((CTextureView *) pView)->InitDraw();
	}
}

void CTextureDoc::RedrawAllView()
{
	POSITION pos = GetFirstViewPosition();

	if(!m_bAutoDelete)
		return;

	while(pos)
	{
		CView *pView = GetNextView(pos);

		if( pView && pView->IsKindOf(RUNTIME_CLASS(CTextureView)) )
			((CTextureView *) pView)->Render();
	}
}

void CTextureDoc::DeleteForRestore()
{
	POSITION pos = GetFirstViewPosition();

	while(pos)
	{
		CView *pView = GetNextView(pos);

		if( pView && pView->IsKindOf(RUNTIME_CLASS(CTextureView)) )
			((CTextureView *) pView)->DeleteForRestore();
	}
}

void CTextureDoc::Restore()
{
	POSITION pos = GetFirstViewPosition();

	while(pos)
	{
		CView *pView = GetNextView(pos);

		if( pView && pView->IsKindOf(RUNTIME_CLASS(CTextureView)) )
		{
			((CTextureView *) pView)->Restore();
			((CTextureView *) pView)->Render();
		}
	}
}
