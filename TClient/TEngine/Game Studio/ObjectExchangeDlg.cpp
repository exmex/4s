// ObjectExchangeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "ObjectExchangeDlg.h"
#include ".\objectexchangedlg.h"


// CObjectExchangeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectExchangeDlg, CDialog)
CObjectExchangeDlg::CObjectExchangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CObjectExchangeDlg::IDD, pParent)
	, m_bOBJExchange(FALSE)
	, m_bAllOBJ(TRUE)
{
}

CObjectExchangeDlg::~CObjectExchangeDlg()
{
}

void CObjectExchangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_OBJ_REPLACE, m_bOBJExchange);
	DDX_Check(pDX, IDC_CHECK_ALLOBJ, m_bAllOBJ);
}


BEGIN_MESSAGE_MAP(CObjectExchangeDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_ALLOBJ, OnBnClickedCheck)
END_MESSAGE_MAP()


// CObjectExchangeDlg 메시지 처리기입니다.

void CObjectExchangeDlg::OnBnClickedCheck()
{
	UpdateData();
}
