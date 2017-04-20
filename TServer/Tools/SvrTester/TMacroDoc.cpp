// TMacroDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SvrTester.h"
#include "TMacroDoc.h"


// CTMacroDoc

IMPLEMENT_DYNCREATE(CTMacroDoc, CDocument)

CTMacroDoc::CTMacroDoc()
{	
}

BOOL CTMacroDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CTMacroDoc::~CTMacroDoc()
{	
}


BEGIN_MESSAGE_MAP(CTMacroDoc, CDocument)
END_MESSAGE_MAP()


// CTMacroDoc 진단입니다.

#ifdef _DEBUG
void CTMacroDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTMacroDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTMacroDoc serialization입니다.

void CTMacroDoc::Serialize(CArchive& ar)
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


// CTMacroDoc 명령입니다.