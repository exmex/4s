// RebuildBSPDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "RebuildBSPDlg.h"
#include ".\rebuildbspdlg.h"


// CRebuildBSPDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRebuildBSPDlg, CDialog)
CRebuildBSPDlg::CRebuildBSPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRebuildBSPDlg::IDD, pParent)
	, m_strFile(_T(""))
	, m_bPVS(TRUE)
	, m_bTEX(FALSE)
	, m_bOBJ(FALSE)
	, m_bSFX(FALSE)
	, m_bSND(FALSE)
	, m_bLIGHT(FALSE)
{
}

CRebuildBSPDlg::~CRebuildBSPDlg()
{
}

void CRebuildBSPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MESHFILE, m_strFile);
	DDX_Control(pDX, IDC_BUTTON_MESHFILE, m_cFile);
	DDX_Check(pDX, IDC_CHECK_PVS, m_bPVS);
	DDX_Check(pDX, IDC_CHECK_TEX, m_bTEX);
	DDX_Check(pDX, IDC_CHECK_OBJ, m_bOBJ);
	DDX_Check(pDX, IDC_CHECK_SFX, m_bSFX);
	DDX_Check(pDX, IDC_CHECK_SND, m_bSND);
	DDX_Check(pDX, IDC_CHECK_LIGHT, m_bLIGHT);
}


BEGIN_MESSAGE_MAP(CRebuildBSPDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MESHFILE, OnBnClickedButtonMeshfile)
END_MESSAGE_MAP()


// CRebuildBSPDlg 메시지 처리기입니다.

BOOL CRebuildBSPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_cFile.SetIcon(hIcon);

	return TRUE;
}

void CRebuildBSPDlg::OnBnClickedButtonMeshfile()
{
	CFileDialog dlg(
		TRUE,
		_T("*.tmf"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Mesh Files (*.tmf)|*.tmf|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strFile = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CRebuildBSPDlg::OnOK()
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
