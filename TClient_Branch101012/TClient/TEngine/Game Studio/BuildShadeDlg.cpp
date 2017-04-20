// BuildShadeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "BuildShadeDlg.h"
#include ".\buildshadedlg.h"


// CBuildShadeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBuildShadeDlg, CDialog)
CBuildShadeDlg::CBuildShadeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBuildShadeDlg::IDD, pParent)
	, m_nType(FALSE)
	, m_strLightX(_T(""))
	, m_strLightY(_T(""))
	, m_strLightZ(_T(""))
{
}

CBuildShadeDlg::~CBuildShadeDlg()
{
}

void CBuildShadeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_SEL_UNIT, m_nType);
	DDX_Control(pDX, IDC_SPIN_LX, m_spinLX);
	DDX_Control(pDX, IDC_SPIN_LY, m_spinLY);
	DDX_Control(pDX, IDC_SPIN_LZ, m_spinLZ);
	DDX_Text(pDX, IDC_EDIT_LX, m_strLightX);
	DDX_Text(pDX, IDC_EDIT_LY, m_strLightY);
	DDX_Text(pDX, IDC_EDIT_LZ, m_strLightZ);
}


BEGIN_MESSAGE_MAP(CBuildShadeDlg, CDialog)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CBuildShadeDlg 메시지 처리기입니다.

BOOL CBuildShadeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	m_spinLX.SetRange( -1, 1);
	m_spinLY.SetRange( -1, 1);
	m_spinLZ.SetRange( -1, 1);

	UpdateData(FALSE);
	return TRUE;
}

void CBuildShadeDlg::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinLX )
	{
		FLOAT fLX = (FLOAT) atof(m_strLightX);

		fLX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinLX.SetPos(0);

		m_strLightX.Format( "%.2f", fLX);
		UpdateData(FALSE);
	}

	if( pScrollBar == (CScrollBar *) &m_spinLY )
	{
		FLOAT fLY = (FLOAT) atof(m_strLightY);

		fLY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinLY.SetPos(0);

		m_strLightY.Format( "%.2f", fLY);
		UpdateData(FALSE);
	}

	if( pScrollBar == (CScrollBar *) &m_spinLZ )
	{
		FLOAT fLZ = (FLOAT) atof(m_strLightZ);

		fLZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinLZ.SetPos(0);

		m_strLightZ.Format( "%.2f", fLZ);
		UpdateData(FALSE);
	}

	CDialog::OnVScroll( nSBCode, nPos, pScrollBar);
}
