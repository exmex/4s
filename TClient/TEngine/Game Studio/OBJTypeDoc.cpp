// OBJTypeDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "OBJTypeDoc.h"


// COBJTypeDoc

IMPLEMENT_DYNCREATE(COBJTypeDoc, CTachyonDoc)

COBJTypeDoc::COBJTypeDoc()
{
	m_bDocType = ITEM_OBJTYPE;
	m_pItem = NULL;
}

BOOL COBJTypeDoc::OnNewDocument()
{
	if (!CTachyonDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

COBJTypeDoc::~COBJTypeDoc()
{
	if(m_pItem)
	{
		m_pItem->SetMainDocument(NULL);
		m_pItem = NULL;
	}
}


BEGIN_MESSAGE_MAP(COBJTypeDoc, CTachyonDoc)
END_MESSAGE_MAP()


// COBJTypeDoc 진단입니다.

#ifdef _DEBUG
void COBJTypeDoc::AssertValid() const
{
	CTachyonDoc::AssertValid();
}

void COBJTypeDoc::Dump(CDumpContext& dc) const
{
	CTachyonDoc::Dump(dc);
}
#endif //_DEBUG


// COBJTypeDoc serialization입니다.

void COBJTypeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// COBJTypeDoc 명령입니다.

void COBJTypeDoc::InitData( LPVOID pData)
{
	CTachyonDoc::InitData(pData);
	if(!pData)
		return;

	m_pItem = (COBJTypeItem *) pData;
	m_pItem->SetMainDocument(this);

	Update();
}

void COBJTypeDoc::Update()
{
	if(m_pItem)
	{
		SetTitle(m_pItem->m_strName);
		m_pItem->Regulate();
	}

	UpdateAllViews(NULL);
}
