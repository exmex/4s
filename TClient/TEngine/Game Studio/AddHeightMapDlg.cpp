// AddHeightMapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "AddHeightMapDlg.h"
#include ".\addheightmapdlg.h"


// CAddHeightMapDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAddHeightMapDlg, CDialog)
CAddHeightMapDlg::CAddHeightMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddHeightMapDlg::IDD, pParent)
	, m_dwCountX(128)
	, m_dwCountY(128)
	, m_strMinH(_T("0.00"))
	, m_strMaxH(_T("100.00"))
	, m_dwUnit(128)
	, m_strFile(_T(""))
	, m_strLightX(_T("1.00"))
	, m_strLightY(_T("-0.50"))
	, m_strLightZ(_T("-1.00"))
	, m_bUNIT(4)
	, m_bCell(4)
{
}

CAddHeightMapDlg::~CAddHeightMapDlg()
{
}

void CAddHeightMapDlg::DoDataExchange( CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COUNT_X, m_dwCountX);
	DDX_Text(pDX, IDC_EDIT_COUNT_Y, m_dwCountY);
	DDX_Text(pDX, IDC_EDIT_MINH, m_strMinH);
	DDX_Text(pDX, IDC_EDIT_MAXH, m_strMaxH);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_dwUnit);
	DDX_Control(pDX, IDC_SPIN_COUNT_X, m_spinCountX);
	DDX_Control(pDX, IDC_SPIN_COUNT_Y, m_spinCountY);
	DDX_Control(pDX, IDC_SPIN_MINH, m_spinMinH);
	DDX_Control(pDX, IDC_SPIN_MAXH, m_spinMaxH);
	DDX_Control(pDX, IDC_SPIN_UNIT, m_spinUnit);
	DDX_Control(pDX, IDC_BUTTON_IMAGEFILE, m_cFile);
	DDX_Text(pDX, IDC_EDIT_IMAGEFILE, m_strFile);
	DDX_Control(pDX, IDC_SPIN_LX, m_spinLX);
	DDX_Control(pDX, IDC_SPIN_LY, m_spinLY);
	DDX_Control(pDX, IDC_SPIN_LZ, m_spinLZ);
	DDX_Text(pDX, IDC_EDIT_LZ, m_strLightZ);
	DDX_Text(pDX, IDC_EDIT_LY, m_strLightY);
	DDX_Text(pDX, IDC_EDIT_LX, m_strLightX);
	DDX_Text(pDX, IDC_EDIT_TILE, m_bUNIT);
	DDX_Text(pDX, IDC_EDIT_CELL, m_bCell);
	DDX_Control(pDX, IDC_SPIN_CELL, m_spinCell);
	DDV_MinMaxByte(pDX, m_bCell, 1, 8);
}


BEGIN_MESSAGE_MAP(CAddHeightMapDlg, CDialog)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_IMAGEFILE, OnBnClickedButtonImagefile)
END_MESSAGE_MAP()


// CAddHeightMapDlg 메시지 처리기입니다.

BOOL CAddHeightMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_cFile.SetIcon(hIcon);

	m_spinCountX.SetRange( 0, 10000);
	m_spinCountY.SetRange( 0, 10000);
	m_spinUnit.SetRange( 0, 10000);
	m_spinCell.SetRange( 1, 8);
	m_spinMinH.SetRange( -1, 1);
	m_spinMaxH.SetRange( -1, 1);
	m_spinLX.SetRange( -1, 1);
	m_spinLY.SetRange( -1, 1);
	m_spinLZ.SetRange( -1, 1);

	return TRUE;
}

void CAddHeightMapDlg::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinMinH )
	{
		FLOAT fMinH = (FLOAT) atof(m_strMinH);

		fMinH += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinMinH.SetPos(0);

		m_strMinH.Format( "%.2f", fMinH);
		UpdateData(FALSE);
	}

	if( pScrollBar == (CScrollBar *) &m_spinMaxH )
	{
		FLOAT fMaxH = (FLOAT) atof(m_strMaxH);

		fMaxH += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinMaxH.SetPos(0);

		m_strMaxH.Format( "%.2f", fMaxH);
		UpdateData(FALSE);
	}

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

void CAddHeightMapDlg::OnBnClickedButtonImagefile()
{
	CFileDialog dlg(
		TRUE,
		_T("*.tga; *.bmp; *.png; *.jpg; *.dds"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Image Files (*.tga, *.bmp, *.png, *.jpg, *.dds)|*.tga; *.bmp; *.png; *.jpg; *.dds|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CAddHeightMapDlg::OnOK()
{
	UpdateData();

	m_strFile.TrimRight();
	m_strFile.TrimLeft();

	if(m_strFile.IsEmpty())
	{
		CString strMessage;

		strMessage.LoadString(IDS_ERR_NOFILENAME);
		MessageBox(strMessage);

		return;
	}

	CDialog::OnOK();
}
