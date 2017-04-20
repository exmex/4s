// EditFovDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "EditFovDlg.h"


// CEditFovDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditFovDlg, CDialog)
CEditFovDlg::CEditFovDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditFovDlg::IDD, pParent)
	, m_strFOV(_T(""))
{
}

CEditFovDlg::~CEditFovDlg()
{
}

void CEditFovDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FOV, m_strFOV);
}


BEGIN_MESSAGE_MAP(CEditFovDlg, CDialog)
END_MESSAGE_MAP()


// CEditFovDlg 메시지 처리기입니다.
