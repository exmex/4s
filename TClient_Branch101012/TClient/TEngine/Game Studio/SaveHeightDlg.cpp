// SaveHeightDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "SaveHeightDlg.h"


// CSaveHeightDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSaveHeightDlg, CDialog)
CSaveHeightDlg::CSaveHeightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveHeightDlg::IDD, pParent)
	, m_strMinH(_T("0.00"))
	, m_strMaxH(_T("100.00"))
	, m_nType(FALSE)
{
}

CSaveHeightDlg::~CSaveHeightDlg()
{
}

void CSaveHeightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_SEL_UNIT, m_nType);
	DDX_Text(pDX, IDC_EDIT_HEIGHT_MIN, m_strMinH);
	DDX_Text(pDX, IDC_EDIT_HEIGHT_MAX, m_strMaxH);
	DDX_Control(pDX, IDC_SPIN_H_MIN, m_spinMinH);
	DDX_Control(pDX, IDC_SPIN_H_MAX, m_spinMaxH);
}

BOOL CSaveHeightDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	m_spinMinH.SetRange( -1, 1);
	m_spinMaxH.SetRange( -1, 1);

	m_strMaxH = "255.00";

	UpdateData(FALSE);
	return TRUE;
}

void CSaveHeightDlg::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	if( pScrollBar == (CScrollBar *) &m_spinMinH )
	{
		FLOAT fMinH = ( FLOAT )atof( m_strMinH );

		fMinH += ( ( FLOAT )INT( nPos ) ) / 100.0f;
		m_spinMinH.SetPos(0);

		m_strMinH.Format( "%.2f", fMinH );
		UpdateData(FALSE);
	}
	
	if( pScrollBar == ( CScrollBar * )&m_spinMaxH )
	{
		FLOAT fMaxH = ( FLOAT )atof( m_strMaxH );

		fMaxH +=( ( FLOAT )INT( nPos ) ) / 100.0f;
		m_spinMaxH.SetPos(0);

		m_strMaxH.Format( "%.2f", fMaxH );
		UpdateData(FALSE);
	}

	CDialog::OnVScroll( nSBCode, nPos, pScrollBar );
}
void CSaveHeightDlg::OnOK()
{
	UpdateData();
	CDialog::OnOK();
}


BEGIN_MESSAGE_MAP(CSaveHeightDlg, CDialog)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CSaveHeightDlg 메시지 처리기입니다.
