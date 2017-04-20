// SFXDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "SFXFrame.h"
#include "SFXDoc.h"
#include "MainFrm.h"


// CSFXDoc

IMPLEMENT_DYNCREATE(CSFXDoc, CGridDoc)

CSFXDoc::CSFXDoc()
{
	m_bDocType = ITEM_SFX;
	m_pItem = NULL;
}

BOOL CSFXDoc::OnNewDocument()
{
	if (!CGridDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSFXDoc::~CSFXDoc()
{
	if(m_pItem)
	{
		m_pItem->SetMainDocument(NULL);
		m_pItem->m_TSFX.Release();
		m_pItem = NULL;
	}
}


BEGIN_MESSAGE_MAP(CSFXDoc, CGridDoc)
END_MESSAGE_MAP()


// CSFXDoc 진단입니다.

#ifdef _DEBUG
void CSFXDoc::AssertValid() const
{
	CGridDoc::AssertValid();
}

void CSFXDoc::Dump(CDumpContext& dc) const
{
	CGridDoc::Dump(dc);
}
#endif //_DEBUG


// CSFXDoc serialization입니다.

void CSFXDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}


// CSFXDoc 명령입니다.

void CSFXDoc::InitData( LPVOID pData)
{
	CGridDoc::InitData(pData);
	if(!pData)
		return;

	m_pItem = (CSFXItem *) pData;
	m_pItem->SetMainDocument(this);

	Update();
}

void CSFXDoc::Update()
{
	if(m_pItem)
	{
		CParticleBar *pPSFXBAR = GetParticleBar();
		CSFXBar *pSFXBAR = GetSFXBar();

		SetTitle(m_pItem->m_strName);
		m_pItem->Regulate();

		m_pItem->m_TSFX.InitSFX(
			&CMainFrame::m_3DDevice,
			&m_pItem->m_SFX);

		if(pPSFXBAR)
			pPSFXBAR->SetItem(m_pItem);

		if(pSFXBAR)
			pSFXBAR->SetItem(m_pItem);
	}
}

void CSFXDoc::RedrawAllView()
{
	CSFXFrame *pFrame = (CSFXFrame *) GetFrameWnd();

	if(!m_bAutoDelete)
		return;

	for( int i=0; i<2; i++)
		for( int j=0; j<2; j++)
			pFrame->m_pGridView[i][j]->Render();
}

CPlayCtrl *CSFXDoc::GetPlayCtrl()
{
	CSFXFrame *pFrame = (CSFXFrame *) GetFrameWnd();
	return pFrame ? pFrame->m_wndPlayCtrlBar.GetPlayCtrl() : NULL;
}

CParticleBar *CSFXDoc::GetParticleBar()
{
	CSFXFrame *pFrame = (CSFXFrame *) GetFrameWnd();
	return pFrame ? &pFrame->m_wndPSFXBar : NULL;
}

CSFXBar *CSFXDoc::GetSFXBar()
{
	CSFXFrame *pFrame = (CSFXFrame *) GetFrameWnd();
	return pFrame ? &pFrame->m_wndSFXBar : NULL;
}

void CSFXDoc::SetFPS( DWORD dwFPS )
{
	CSFXFrame *pFrame = (CSFXFrame *) GetFrameWnd();
	pFrame->SetFPS( dwFPS );
}
