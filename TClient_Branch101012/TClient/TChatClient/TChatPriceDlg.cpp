// TChatPriceDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TChatClient.h"
#include "TChatPriceDlg.h"
#include "TChatItem.h"
#include ".\tchatpricedlg.h"


// CTChatPriceDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTChatPriceDlg, CDialog)
CTChatPriceDlg::CTChatPriceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTChatPriceDlg::IDD, pParent)
	, m_dwRune(0)
	, m_dwLuna(0)
	, m_dwCron(0)
{
	m_pTITEM = NULL;
}

CTChatPriceDlg::~CTChatPriceDlg()
{
}

void CTChatPriceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RUNE, m_dwRune);
	DDX_Text(pDX, IDC_EDIT_LUNA, m_dwLuna);
	DDX_Text(pDX, IDC_EDIT_CRON, m_dwCron);
}


BEGIN_MESSAGE_MAP(CTChatPriceDlg, CDialog)
END_MESSAGE_MAP()


// CTChatPriceDlg 메시지 처리기입니다.

BOOL CTChatPriceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	DWORD dwPrice = m_pTITEM->m_pTITEM->m_dwPrice / 2;
	m_dwRune = dwPrice / 1000000;
	m_dwLuna = (dwPrice % 1000000) / 1000;
	m_dwCron = dwPrice % 1000;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTChatPriceDlg::OnOK()
{
	UpdateData();
	CDialog::OnOK();
}
