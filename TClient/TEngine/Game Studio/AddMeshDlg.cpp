// AddMeshDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "AddTextureDlg.h"
#include "AddMeshDlg.h"


// CAddMeshDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAddMeshDlg, CDialog)
CAddMeshDlg::CAddMeshDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAddMeshDlg::IDD, pParent)
, m_strFileName(_T(""))
, m_strFolderName(_T(""))
, m_nLoadType(0)
{
}

CAddMeshDlg::~CAddMeshDlg()
{
}

void CAddMeshDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOADTYPE, m_cLoadType);
	DDX_Control(pDX, IDC_BUTTON_MESHFILE, m_cFile);
	DDX_Control(pDX, IDC_BUTTON_FOLDER, m_cFolder);
	DDX_Control(pDX, IDC_STATIC_FILE, m_cFileTitle);
	DDX_Control(pDX, IDC_STATIC_FOLDER, m_cFolderTitle);
	DDX_Control(pDX, IDC_EDIT_MESHFILE, m_cFileEdit);
	DDX_Control(pDX, IDC_EDIT_FOLDERNAME, m_cFolderEdit);
	DDX_Text(pDX, IDC_EDIT_MESHFILE, m_strFileName);
	DDX_Text(pDX, IDC_EDIT_FOLDERNAME, m_strFolderName);
	DDX_CBIndex(pDX, IDC_COMBO_LOADTYPE, m_nLoadType);
}


BEGIN_MESSAGE_MAP(CAddMeshDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_LOADTYPE, OnCbnSelchangeComboLoadtype)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, OnBnClickedButtonFolder)
	ON_BN_CLICKED(IDC_BUTTON_MESHFILE, OnBnClickedButtonMeshfile)
END_MESSAGE_MAP()


// CAddMeshDlg 메시지 처리기입니다.

BOOL CAddMeshDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_cFile.SetIcon(hIcon);
	m_cFolder.SetIcon(hIcon);
	m_cLoadType.ResetContent();

	m_cLoadType.AddString("Mesh File");
	m_cLoadType.AddString("Mesh Folder");
	m_cLoadType.SetCurSel(DEF_LOADTYPE_FILE);

	OnCbnSelchangeComboLoadtype();
	UpdateData(FALSE);

	return TRUE;
}

void CAddMeshDlg::OnCbnSelchangeComboLoadtype()
{
	UpdateData();
	m_cFileTitle.EnableWindow(m_nLoadType == DEF_LOADTYPE_FILE);
	m_cFileEdit.EnableWindow(m_nLoadType == DEF_LOADTYPE_FILE);
	m_cFile.EnableWindow(m_nLoadType == DEF_LOADTYPE_FILE);

	m_cFolderTitle.EnableWindow(m_nLoadType == DEF_LOADTYPE_FOLDER);
	m_cFolderEdit.EnableWindow(m_nLoadType == DEF_LOADTYPE_FOLDER);
	m_cFolder.EnableWindow(m_nLoadType == DEF_LOADTYPE_FOLDER);

	if( m_nLoadType != DEF_LOADTYPE_FILE )
		m_strFileName.Empty();

	if( m_nLoadType != DEF_LOADTYPE_FOLDER )
		m_strFolderName.Empty();

	UpdateData(FALSE);
}

void CAddMeshDlg::OnBnClickedButtonFolder()
{
	CTShell dlg;

	dlg.m_strTitle = _T("Image Folder");
	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strFolderName = dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CAddMeshDlg::OnBnClickedButtonMeshfile()
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
		m_strFileName = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CAddMeshDlg::OnOK()
{
	UpdateData();

	m_strFolderName.TrimRight();
	m_strFolderName.TrimLeft();

	m_strFileName.TrimRight();
	m_strFileName.TrimLeft();

	switch(m_nLoadType)
	{
	case DEF_LOADTYPE_FILE		:
		if(m_strFileName.IsEmpty())
		{
			CString strMessage;

			strMessage.LoadString(IDS_ERR_NOFILENAME);
			MessageBox(strMessage);

			return;
		}

		break;

	case DEF_LOADTYPE_FOLDER	:
		if(m_strFolderName.IsEmpty())
		{
			CString strMessage;

			strMessage.LoadString(IDS_ERR_NOFOLDERNAME);
			MessageBox(strMessage);

			return;
		}

		break;
	}

	CDialog::OnOK();
}
