// TChatClientDoc.cpp : CTChatClientDoc 클래스의 구현
//

#include "stdafx.h"
#include "TChatClient.h"

#include "TChatClientDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTChatClientDoc

IMPLEMENT_DYNCREATE(CTChatClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CTChatClientDoc, CDocument)
END_MESSAGE_MAP()


// CTChatClientDoc 생성/소멸

CTChatClientDoc::CTChatClientDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CTChatClientDoc::~CTChatClientDoc()
{
}

BOOL CTChatClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CTChatClientDoc serialization

void CTChatClientDoc::Serialize(CArchive& ar)
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


// CTChatClientDoc 진단

#ifdef _DEBUG
void CTChatClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTChatClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTChatClientDoc 명령
