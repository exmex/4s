// AddAnimationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "AddAnimationDlg.h"
#include "AddTextureDlg.h"


// CAddAnimationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAddAnimationDlg, CDialog)
CAddAnimationDlg::CAddAnimationDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAddAnimationDlg::IDD, pParent)
, m_strFileName(_T(""))
, m_strFolderName(_T(""))
, m_nLoadType(0)
{
}

CAddAnimationDlg::~CAddAnimationDlg()
{
}

void CAddAnimationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOADTYPE, m_cLoadType);
	DDX_Control(pDX, IDC_BUTTON_ANIMATIONFILE, m_cFile);
	DDX_Control(pDX, IDC_BUTTON_FOLDER, m_cFolder);
	DDX_Control(pDX, IDC_STATIC_FILE, m_cFileTitle);
	DDX_Control(pDX, IDC_STATIC_FOLDER, m_cFolderTitle);
	DDX_Control(pDX, IDC_EDIT_ANIMATIONFILE, m_cFileEdit);
	DDX_Control(pDX, IDC_EDIT_FOLDERNAME, m_cFolderEdit);
	DDX_Text(pDX, IDC_EDIT_ANIMATIONFILE, m_strFileName);
	DDX_Text(pDX, IDC_EDIT_FOLDERNAME, m_strFolderName);
	DDX_CBIndex(pDX, IDC_COMBO_LOADTYPE, m_nLoadType);
}


BEGIN_MESSAGE_MAP(CAddAnimationDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_LOADTYPE, OnCbnSelchangeComboLoadtype)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, OnBnClickedButtonFolder)
	ON_BN_CLICKED(IDC_BUTTON_ANIMATIONFILE, OnBnClickedButtonAnimationfile)
END_MESSAGE_MAP()


// CAddAnimationDlg 메시지 처리기입니다.

BOOL CAddAnimationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_cFile.SetIcon(hIcon);
	m_cFolder.SetIcon(hIcon);
	m_cLoadType.ResetContent();

	m_cLoadType.AddString("Animation File");
	m_cLoadType.AddString("Animation Folder");
	m_cLoadType.AddString("Blank Animation");
	m_cLoadType.SetCurSel(DEF_LOADTYPE_FILE);

	OnCbnSelchangeComboLoadtype();
	UpdateData(FALSE);

	return TRUE;
}

void CAddAnimationDlg::OnCbnSelchangeComboLoadtype()
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

void CAddAnimationDlg::OnBnClickedButtonFolder()
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

void CAddAnimationDlg::OnBnClickedButtonAnimationfile()
{
	CFileDialog dlg(
		TRUE,
		_T("*.taf"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Animation Files (*.taf)|*.taf|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strFileName = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CAddAnimationDlg::OnOK()
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
