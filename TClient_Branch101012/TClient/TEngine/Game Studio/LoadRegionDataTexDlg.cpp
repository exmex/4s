// LoadRegionDataTexDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "EXTMAPDoc.h"
#include "LoadRegionDataTexDlg.h"
#include ".\loadregiondatatexdlg.h"


// CLoadRegionDataTexDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLoadRegionDataTexDlg, CDialog)
CLoadRegionDataTexDlg::CLoadRegionDataTexDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadRegionDataTexDlg::IDD, pParent)
	, m_strTitle(_T(""))
	, m_strFileName(_T(""))
{
}

CLoadRegionDataTexDlg::~CLoadRegionDataTexDlg()
{
}

void CLoadRegionDataTexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_IMAGEFILE, m_cFile);
	DDX_Text(pDX, IDC_EDIT_IMAGEFILE, m_strFileName);
}

BEGIN_MESSAGE_MAP(CLoadRegionDataTexDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_IMAGEFILE, OnBnClickedButtonImagefile)
END_MESSAGE_MAP()

// CLoadRegionDataTexDlg 메시지 처리기입니다.

BOOL CLoadRegionDataTexDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_cFile.SetIcon(hIcon);

	UpdateData(FALSE);

	return TRUE;
}

void CLoadRegionDataTexDlg::OnBnClickedButtonImagefile()
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
		m_strFileName = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CLoadRegionDataTexDlg::OnOK()
{
	UpdateData();

	m_strFileName.TrimRight();
	m_strFileName.TrimLeft();

	if(m_strFileName.IsEmpty())
	{
		CString strMessage;

		strMessage.LoadString(IDS_ERR_NOFILENAME);
		MessageBox(strMessage);

		return;
	}

	CDialog::OnOK();
}
