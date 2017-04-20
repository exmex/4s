// TachyonDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "TachyonDoc.h"
#include "MainFrm.h"
#include "GridView.h"
#include ".\tachyondoc.h"


VECTORDOC CTachyonDoc::m_vDOC;
// CTachyonDoc


IMPLEMENT_DYNCREATE(CTachyonDoc, CDocument)

CTachyonDoc::CTachyonDoc()
{
	m_pFRAME = NULL;
	m_bDocType = 0;
}

BOOL CTachyonDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

CTachyonDoc::~CTachyonDoc()
{
	VECTORDOC::iterator it = m_vDOC.begin();

	while(it != m_vDOC.end())
		if( (*it) == this )
			m_vDOC.erase(it);
		else
			it++;
}


BEGIN_MESSAGE_MAP(CTachyonDoc, CDocument)
END_MESSAGE_MAP()


#ifdef _DEBUG
void CTachyonDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTachyonDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void CTachyonDoc::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
	}
	else
	{
	}
}

void CTachyonDoc::UpdateAll()
{
	UpdateAll(this);
}

void CTachyonDoc::UpdateAll( CTachyonDoc *pExcept)
{
	int nCount = INT(m_vDOC.size());

	for( int i=0; i<nCount; i++)
		if( m_vDOC[i] != pExcept )
			m_vDOC[i]->Update();
}


void CTachyonDoc::UpdateFOV( FLOAT fFOV)
{
	int nCount = INT(m_vDOC.size());
		
	for( int i=0; i<nCount; i++)
	{
		POSITION pos =  m_vDOC[i]->GetFirstViewPosition();
			
		while(pos)
		{
			CView *pView = m_vDOC[i]->GetNextView(pos);

			if( pView && pView->IsKindOf(RUNTIME_CLASS(CGridView)) )
			{
				CGridView * gridView = ( CGridView* )pView;
				gridView->InitFOV( fFOV );
			}
		}
	}
}

void CTachyonDoc::CloseAllBSP()
{
	VECTORDOC::iterator it = m_vDOC.begin();

	while(it != m_vDOC.end())
		if( (*it)->m_bDocType == ITEM_BSPMAP )
			(*it)->OnCloseDocument();
		else
			it++;

	CTBSPNode::ReleaseCommon();
	CTBSPNode::InitCommon(&CMainFrame::m_3DDevice);
}

void CTachyonDoc::DeleteForReset()
{
	int nCount = INT(m_vDOC.size());

	for( int i=0; i<nCount; i++)
		m_vDOC[i]->DeleteForRestore();
}

void CTachyonDoc::Reset()
{
	int nCount = INT(m_vDOC.size());

	for( int i=0; i<nCount; i++)
		m_vDOC[i]->Restore();
}

void CTachyonDoc::DeleteForRestore()
{
}

void CTachyonDoc::Restore()
{
}

void CTachyonDoc::Update()
{
}

void CTachyonDoc::InitData( LPVOID pData)
{
	POSITION pos = GetFirstViewPosition();
	DWORD dwPass;

	if(pos)
	{
		CView *pView = GetNextView(pos);

		if(pView)
			m_pFRAME = pView->GetParentFrame();
	}

	if(FAILED(CMainFrame::m_3DDevice.m_pDevice->ValidateDevice(&dwPass)))
		CMainFrame::RestoreDevice();
	m_vDOC.push_back(this);
}

CFrameWnd *CTachyonDoc::GetFrameWnd()
{
	return m_pFRAME;
}

