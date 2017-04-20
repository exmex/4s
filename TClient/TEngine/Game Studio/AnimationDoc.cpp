// AnimationDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "AnimationFrame.h"
#include "AnimationDoc.h"


// CAnimationDoc

IMPLEMENT_DYNCREATE(CAnimationDoc, CGridDoc)

CAnimationDoc::CAnimationDoc()
{
	m_bDocType = ITEM_ANIMATION;
	m_pItem = NULL;
	m_fTime = 0.0f;

	D3DXMatrixIdentity(&m_vWorld);
	D3DXMatrixIdentity(&m_vScale);
}

BOOL CAnimationDoc::OnNewDocument()
{
	if (!CGridDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CAnimationDoc::~CAnimationDoc()
{
	if(m_pItem)
	{
		m_pItem->SetMainDocument(NULL);
		m_pItem = NULL;
	}
}


BEGIN_MESSAGE_MAP(CAnimationDoc, CGridDoc)
END_MESSAGE_MAP()


// CAnimationDoc 진단입니다.

#ifdef _DEBUG
void CAnimationDoc::AssertValid() const
{
	CGridDoc::AssertValid();
}

void CAnimationDoc::Dump(CDumpContext& dc) const
{
	CGridDoc::Dump(dc);
}
#endif //_DEBUG


// CAnimationDoc serialization입니다.

void CAnimationDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}


// CAnimationDoc 명령입니다.

CPlayCtrl *CAnimationDoc::GetPlayCtrl()
{
	CAnimationFrame *pFrame = (CAnimationFrame *) GetFrameWnd();
	return pFrame ? pFrame->m_wndPlayCtrlBar.GetPlayCtrl() : NULL;
}

CAnimationBar *CAnimationDoc::GetAnimationBar()
{
	CAnimationFrame *pFrame = (CAnimationFrame *) GetFrameWnd();
	return pFrame ? &pFrame->m_wndAnimationBar : NULL;
}

void CAnimationDoc::InitData( LPVOID pData)
{
	CGridDoc::InitData(pData);
	if(!pData)
		return;

	m_pItem = (CAnimationItem *) pData;
	m_pItem->SetMainDocument(this);

	Update();
}

void CAnimationDoc::Update()
{
	if(m_pItem)
	{
		CAnimationBar *pControl = GetAnimationBar();

		SetTitle(m_pItem->m_strName);
		m_pItem->Regulate();

		if(pControl)
			pControl->SetItem(m_pItem);
	}
}

D3DXMATRIX CAnimationDoc::CalcFrameMatrix( BYTE bUpdate)
{
	m_pItem->m_Aniset.m_fCurTime = m_fTime;

	ANIKEY vKey = m_pItem->m_Aniset.GetAniKey();
	D3DXMATRIX vScale;
	D3DXMATRIX vRotX;
	D3DXMATRIX vRotY;
	D3DXMATRIX vRotZ;
	D3DXMATRIX vPos;

	D3DXMatrixScaling(
		&vScale,
		vKey.m_fScaleX,
		vKey.m_fScaleY,
		vKey.m_fScaleZ);

	D3DXMatrixRotationX(
		&vRotX,
		vKey.m_fRotX);

	D3DXMatrixRotationY(
		&vRotY,
		vKey.m_fRotY);

	D3DXMatrixRotationZ(
		&vRotZ,
		vKey.m_fRotZ);

	D3DXMatrixTranslation(
		&vPos,
		vKey.m_fPosX,
		vKey.m_fPosY,
		vKey.m_fPosZ);

	if(bUpdate)
	{
		m_vWorld = vRotX * vRotY * vRotZ * vPos * m_vWorld;
		m_vScale = m_vScale * vScale;

		vPos = m_vScale * m_vWorld;
	}
	else
		vPos = m_vScale * vScale * vRotX * vRotY * vRotZ * vPos * m_vWorld;

	return vPos;
}

void CAnimationDoc::ResetCamera()
{
	((CAnimationFrame *) GetFrameWnd())->ResetCamera();
}

void CAnimationDoc::RedrawAllView()
{
	CAnimationFrame *pFrame = (CAnimationFrame *) GetFrameWnd();

	if(!m_bAutoDelete)
		return;

	for( int i=0; i<2; i++)
		for( int j=0; j<2; j++)
			pFrame->m_pGridView[i][j]->Render();
}
