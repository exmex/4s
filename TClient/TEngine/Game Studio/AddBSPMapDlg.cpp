// AddBSPMapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "AddBSPMapDlg.h"


// CAddBSPMapDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAddBSPMapDlg, CDialog)
CAddBSPMapDlg::CAddBSPMapDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAddBSPMapDlg::IDD, pParent)
, m_strFile(_T(""))
, m_bPVS(TRUE)
{
}

CAddBSPMapDlg::~CAddBSPMapDlg()
{
}

void CAddBSPMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MESHFILE, m_strFile);
	DDX_Control(pDX, IDC_BUTTON_MESHFILE, m_cFile);
	DDX_Check(pDX, IDC_CHECK_PVS, m_bPVS);
}


BEGIN_MESSAGE_MAP(CAddBSPMapDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MESHFILE, OnBnClickedButtonMeshfile)
END_MESSAGE_MAP()


// CAddBSPMapDlg 메시지 처리기입니다.

BOOL CAddBSPMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_cFile.SetIcon(hIcon);

	return TRUE;
}

void CAddBSPMapDlg::OnBnClickedButtonMeshfile()
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

void CAddBSPMapDlg::OnOK()
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
