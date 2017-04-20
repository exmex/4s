// TChannelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TChatClient.h"
#include "TChannelDlg.h"


// CTChannelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTChannelDlg, CDialog)
CTChannelDlg::CTChannelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTChannelDlg::IDD, pParent)
{
	m_pTCHANNEL = NULL;
	ClearTCHANNEL();
}

CTChannelDlg::~CTChannelDlg()
{
	ClearTCHANNEL();
}

void CTChannelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHANNEL, m_cTCHANNEL);
}


BEGIN_MESSAGE_MAP(CTChannelDlg, CDialog)
END_MESSAGE_MAP()


// CTChannelDlg 메시지 처리기입니다.

void CTChannelDlg::ClearTCHANNEL()
{
	while(!m_vTCHANNEL.empty())
	{
		delete m_vTCHANNEL.back();
		m_vTCHANNEL.pop_back();
	}
}

BOOL CTChannelDlg::OnInitDialog()
{
	static CString strSTATE[4] = {
		CString(_T("Maintenance")),
		CString(_T("Normal")),
		CString(_T("Congestion")),
		CString(_T("Saturation"))};
	CDialog::OnInitDialog();
	CRect rect;

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);
	m_cTCHANNEL.DeleteAllItems();

	m_cTCHANNEL.GetClientRect(&rect);
	rect.DeflateRect( 1, 1);
	m_cTCHANNEL.InsertColumn( 0, _T("Channel"), LVCFMT_CENTER, rect.Width() * 2 / 3);
	m_cTCHANNEL.InsertColumn( 1, _T("Status"), LVCFMT_CENTER, rect.Width() / 3);

	m_cTCHANNEL.SetExtendedStyle(
		LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES);

	for( int i=0; i<INT(m_vTCHANNEL.size()); i++)
	{
		int nIndex = m_cTCHANNEL.InsertItem( i, m_vTCHANNEL[i]->m_strNAME);

		m_cTCHANNEL.SetItemText( nIndex, 1, strSTATE[m_vTCHANNEL[i]->m_bStatus]);
		m_cTCHANNEL.SetItemData( nIndex, (DWORD_PTR) m_vTCHANNEL[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTChannelDlg::OnOK()
{
	POSITION pos = m_cTCHANNEL.GetFirstSelectedItemPosition();

	if(pos)
	{
		int nIndex = m_cTCHANNEL.GetNextSelectedItem(pos);

		if( nIndex >= 0 && nIndex < INT(m_vTCHANNEL.size()) )
			m_pTCHANNEL = m_vTCHANNEL[nIndex];
	}

	CDialog::OnOK();
}
