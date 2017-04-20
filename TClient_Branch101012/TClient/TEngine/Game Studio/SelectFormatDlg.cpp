// SelectFormatDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "SelectFormatDlg.h"
#include ".\selectformatdlg.h"
#include "ItemBase.h"

// CSelectFormatDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSelectFormatDlg, CDialog)
CSelectFormatDlg::CSelectFormatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectFormatDlg::IDD, pParent)
	, m_nSelectedFormat(SELECT_FORMAT::DEFAULT_INDEX),m_nType(1)
{
}

CSelectFormatDlg::~CSelectFormatDlg()
{
}

void CSelectFormatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cFormat);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nSelectedFormat);
}

BOOL CSelectFormatDlg::OnInitDialog()
{
	if( CDialog::OnInitDialog() == FALSE )
		return FALSE;

	switch( m_nType )
	{
	case 1:
		for( INT i=0 ; i < SELECT_FORMAT::COUNT ; ++i )
			m_cFormat.AddString( SELECT_FORMAT::AVAILABLE[ i ].m_strTitle );
		m_cFormat.SetCurSel( m_nSelectedFormat );
		break;

	case 2:
		m_cFormat.AddString("TGA");
		m_cFormat.AddString("JPG");
		m_cFormat.SetCurSel( 0 );
		break;
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CSelectFormatDlg, CDialog)
END_MESSAGE_MAP()
