// GuildChange.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "GuildChange.h"


// CGuildChange 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGuildChange, CDialog)
CGuildChange::CGuildChange(CWnd* pParent /*=NULL*/)
	: CDialog(CGuildChange::IDD, pParent)
{
}

CGuildChange::~CGuildChange()
{
}

void CGuildChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGuildChange, CDialog)
END_MESSAGE_MAP()


// CGuildChange 메시지 처리기입니다.
