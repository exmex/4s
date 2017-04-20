// MeshDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MeshFrame.h"
#include "MeshDoc.h"


// CMeshDoc

IMPLEMENT_DYNCREATE(CMeshDoc, CGridDoc)

CMeshDoc::CMeshDoc()
{
	m_bDocType = ITEM_MESH;
	m_pCheck = NULL;
	m_pItem = NULL;

	m_nCurSel = -1;
}

BOOL CMeshDoc::OnNewDocument()
{
	if (!CGridDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CMeshDoc::~CMeshDoc()
{
	if(m_pItem)
	{
		m_pItem->SetMainDocument(NULL);
		m_pItem = NULL;
	}

	if(m_pCheck)
	{
		delete[] m_pCheck;
		m_pCheck = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMeshDoc, CGridDoc)
END_MESSAGE_MAP()


// CMeshDoc 진단입니다.

#ifdef _DEBUG
void CMeshDoc::AssertValid() const
{
	CGridDoc::AssertValid();
}

void CMeshDoc::Dump(CDumpContext& dc) const
{
	CGridDoc::Dump(dc);
}
#endif //_DEBUG


// CMeshDoc serialization입니다.

void CMeshDoc::Serialize( CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}


// CMeshDoc 명령입니다.

CMeshBar *CMeshDoc::GetMeshBar()
{
	CMeshFrame *pFrame = (CMeshFrame *) GetFrameWnd();
	return pFrame ? &pFrame->m_wndMeshBar : NULL;
}

void CMeshDoc::InitData( LPVOID pData)
{
	CGridDoc::InitData(pData);
	if(!pData)
		return;

	m_pItem = (CMeshItem *) pData;
	m_pItem->SetMainDocument(this);
	Update();
}

void CMeshDoc::Update()
{
	if(m_pItem)
	{
		CMeshBar *pControl = GetMeshBar();

		SetTitle(m_pItem->m_strName);
		m_pItem->Regulate();

		if(m_pCheck)
		{
			delete[] m_pCheck;
			m_pCheck = NULL;
		}

		m_pCheck = new BYTE[m_pItem->m_Mesh.m_dwMeshCount];
		ZeroMemory( m_pCheck, m_pItem->m_Mesh.m_dwMeshCount * sizeof(BYTE));

		if(pControl)
			pControl->SetItem(m_pItem);
	}
}

void CMeshDoc::SetCamDist( CWnd *pWnd, FLOAT fDist)
{
	CMeshFrame *pFrame = (CMeshFrame *) GetFrameWnd();

	if( pFrame->GetActiveView() == pWnd )
		pFrame->m_wndMeshBar.SetCamDist(fDist);
}

void CMeshDoc::RedrawAllView()
{
	CMeshFrame *pFrame = (CMeshFrame *) GetFrameWnd();

	if(!m_bAutoDelete)
		return;

	for( int i=0; i<2; i++)
		for( int j=0; j<2; j++)
			pFrame->m_pGridView[i][j]->Render();
}
