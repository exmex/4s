// TFontViewerDoc.cpp : CTFontViewerDoc 클래스의 구현
//

#include "stdafx.h"
#include "TFontViewer.h"

#include "TFontViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTFontViewerDoc

IMPLEMENT_DYNCREATE(CTFontViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CTFontViewerDoc, CDocument)
END_MESSAGE_MAP()


// CTFontViewerDoc 생성/소멸

CTFontViewerDoc::CTFontViewerDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CTFontViewerDoc::~CTFontViewerDoc()
{
}

BOOL CTFontViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CTFontViewerDoc serialization

void CTFontViewerDoc::Serialize(CArchive& ar)
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


// CTFontViewerDoc 진단

#ifdef _DEBUG
void CTFontViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTFontViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTFontViewerDoc 명령
