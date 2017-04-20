// SFXView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MainFrm.h"
#include "SFXView.h"


// CSFXView

IMPLEMENT_DYNCREATE(CSFXView, CGridView)

CSFXView::CSFXView()
{
	D3DLIGHT9 light;

	ZeroMemory( &light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 0.3f;
	light.Diffuse.g = 0.3f;
	light.Diffuse.b = 0.3f;
	light.Diffuse.a = 1.0f;

	light.Ambient.r = 0.1f;
	light.Ambient.g = 0.1f;
	light.Ambient.b = 0.1f;
	light.Ambient.a = 1.0f;
	light.Range = T3DLIGHT_RANGE_MAX;
	light.Direction = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);

	m_LightCenter.SetLight(&light);
	m_LightRight.SetLight(&light);
	m_LightTop.SetLight(&light);

	m_nFrameMode = GSVIEW_SFX;
}

CSFXView::~CSFXView()
{
}

BEGIN_MESSAGE_MAP(CSFXView, CGridView)
END_MESSAGE_MAP()


// CSFXView 진단입니다.

#ifdef _DEBUG
void CSFXView::AssertValid() const
{
	CGridView::AssertValid();
}

void CSFXView::Dump(CDumpContext& dc) const
{
	CGridView::Dump(dc);
}
#endif //_DEBUG


// CSFXView 메시지 처리기입니다.

void CSFXView::DrawContent()
{
	CGridView::DrawContent();

	CSFXDoc *pDoc = GetDocument();

	if(pDoc)
	{
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		if(!pCtrl->m_bPlay)
			CTachyonMedia::m_bON = FALSE;

		EnableLight();
		pDoc->m_pItem->m_TSFX.Render( &CMainFrame::m_3DDevice, &m_Camera);
		DisableLight();

		if(!pCtrl->m_bPlay)
			CTachyonMedia::m_bON = TRUE;
	}
}

void CSFXView::InitCamera()
{
	CGridView::InitCamera();
}
