// AttributeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "AttributeDlg.h"


// CAttributeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAttributeDlg, CDialog)
CAttributeDlg::CAttributeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAttributeDlg::IDD, pParent)
	, m_strTitle(_T(""))
	, m_strValue(_T(""))
	, m_bDataType(0)
{
}

CAttributeDlg::~CAttributeDlg()
{
}

void CAttributeDlg::DoDataExchange( CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_strValue);
}


BEGIN_MESSAGE_MAP(CAttributeDlg, CDialog)
END_MESSAGE_MAP()


// CAttributeDlg 메시지 처리기입니다.
