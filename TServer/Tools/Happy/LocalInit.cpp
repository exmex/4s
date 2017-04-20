// LocalInit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "LocalInit.h"


// CLocalInit 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLocalInit, CDialog)
CLocalInit::CLocalInit(CWnd* pParent /*=NULL*/)
	: CDialog(CLocalInit::IDD, pParent)
{
}

CLocalInit::~CLocalInit()
{
}

void CLocalInit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLocalInit, CDialog)
END_MESSAGE_MAP()


// CLocalInit 메시지 처리기입니다.
