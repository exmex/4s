// SetImportOffsetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "SetImportOffsetDlg.h"
#include ".\setimportoffsetdlg.h"

#define _IMPORT_EXPMAP		0
#define _IMPORT_EXPOBJECT	1

// CSetImportOffsetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetImportOffsetDlg, CDialog)
CSetImportOffsetDlg::CSetImportOffsetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetImportOffsetDlg::IDD, pParent)
	, m_nFromLeft(0)
	, m_nFromTop(0)
	, m_nFromWidth(0)
	, m_nFromHeight(0)
	, m_nToLeft(0)
	, m_nToTop(0)
	, m_fToHeight(0.0f)
{
}

CSetImportOffsetDlg::~CSetImportOffsetDlg()
{
}

void CSetImportOffsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LEFT_FROM, m_strFromLeft);
	DDX_Text(pDX, IDC_EDIT_TOP_FROM, m_strFromTop);
	DDX_Text(pDX, IDC_EDIT_WIDTH_FROM, m_strFromWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT_FROM, m_strFromHeight);
	DDX_Text(pDX, IDC_EDIT_LEFT_TO, m_strToLeft);
	DDX_Text(pDX, IDC_EDIT_TOP_TO, m_strToTop);
//	DDX_Text(pDX, IDC_EDIT_HEIGHT_TO, m_strToHeight);
}


BEGIN_MESSAGE_MAP(CSetImportOffsetDlg, CDialog)
END_MESSAGE_MAP()


// CSetImportOffsetDlg 메시지 처리기입니다.

BOOL CSetImportOffsetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	m_strFromLeft.Format( "%d", m_nFromLeft );
	m_strFromTop.Format( "%d", m_nFromTop );
	m_strFromWidth.Format( "%d", m_nFromWidth );
	m_strFromHeight.Format( "%d", m_nFromHeight );

	m_strToLeft.Format( "%d", m_nToLeft );
	m_strToTop.Format( "%d", m_nToTop );
	m_strToHeight.Format( "%f", m_fToHeight );

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSetImportOffsetDlg::OnOK()
{
	UpdateData();

	m_nFromLeft = atoi( m_strFromLeft.GetString() );
	m_nFromTop = atoi( m_strFromTop.GetString() );
	m_nFromWidth = atoi( m_strFromWidth.GetString() );
	m_nFromHeight = atoi( m_strFromHeight.GetString() );

	m_nToLeft = atoi( m_strToLeft.GetString() );
	m_nToTop = atoi( m_strToTop.GetString() );
	m_fToHeight = (float)atof( m_strToHeight.GetString() );

	CDialog::OnOK();
}
