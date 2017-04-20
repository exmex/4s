// TPathMergeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TPathMerge.h"
#include "TPathMergeDlg.h"
#include "PathFlag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTPathMergeDlg dialog



CTPathMergeDlg::CTPathMergeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTPathMergeDlg::IDD, pParent)
	, m_strSRC(_T(""))
	, m_strDEST(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTPathMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DEST, m_cDEST);
	DDX_Control(pDX, IDC_BUTTON_SRC, m_cSRC);
	DDX_Text(pDX, IDC_EDIT_SRC, m_strSRC);
	DDX_Text(pDX, IDC_EDIT_DEST, m_strDEST);
}

BEGIN_MESSAGE_MAP(CTPathMergeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DEST, OnBnClickedButtonDest)
	ON_BN_CLICKED(IDC_BUTTON_SRC, OnBnClickedButtonSrc)
	ON_BN_CLICKED(ID_MERGE, OnBnClickedMerge)
END_MESSAGE_MAP()


// CTPathMergeDlg message handlers

BOOL CTPathMergeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_cDEST.SetIcon(hIcon);
	m_cSRC.SetIcon(hIcon);

	m_bInit = FALSE;
	m_bUnitX = 0;
	m_bUnitZ = 0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTPathMergeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTPathMergeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTPathMergeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTPathMergeDlg::OnBnClickedButtonDest()
{
	CFileDialog dlg(
		FALSE,
		_T("*.tpf"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("T-Project Path Data Files (*.tpf)|*.tpf|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strDESTFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strDESTFOLDER = dlg.GetFileName();
		m_strDEST = dlg.GetPathName();
		m_strDESTFOLDER = m_strDEST.Left(m_strDEST.GetLength() - m_strDESTFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTPathMergeDlg::OnBnClickedButtonSrc()
{
	CTShell dlg;

	dlg.m_strSelDir = m_strSRCFOLDER;
	dlg.m_strTitle = _T("GSF Folder");

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strSRCFOLDER = dlg.m_strPath;
		m_strSRC = dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CTPathMergeDlg::OnBnClickedMerge()
{
	m_bInit = FALSE;
	m_bUnitX = 0;
	m_bUnitZ = 0;

	CPathFlag::ReleaseTPATH();
	LoadTPATH(m_strSRC);
	SaveTPATH();
	CPathFlag::ReleaseTPATH();
}

BYTE CTPathMergeDlg::IsPathFile( CString strFILE)
{
	CString strExt;
	CTShell shell;

	strExt = shell.GetFileExt(strFILE);
	strExt.MakeUpper();
	strExt.TrimRight();
	strExt.TrimLeft();

	return strExt == _T(".TPF") ? TRUE : FALSE;
}

void CTPathMergeDlg::LoadTPATH( CString strSRC)
{
	CFileFind finder;
	CString strFIND;

	strSRC.TrimRight('\\');
	strFIND.Format("%s\\*.*", strSRC);

	BOOL bFind = finder.FindFile(strFIND);
	while(bFind)
	{
		bFind = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			LoadTPATH(strSRC + CString("\\") + finder.GetFileTitle());
			continue;
		}

		if(IsPathFile(finder.GetFilePath()))
		{
			CFile file( LPCSTR(finder.GetFilePath()), CFile::modeRead|CFile::typeBinary);
			CArchive ar( &file, CArchive::load);

			DWORD dwCount;
			ar >> dwCount;

			for( DWORD i=0; i<dwCount; i++)
			{
				CPathFlag *pTFLAG = new CPathFlag();
				DWORD dwTFLAG;

				ar	>> pTFLAG->m_dwID
					>> pTFLAG->m_vPOS.x
					>> pTFLAG->m_vPOS.y
					>> pTFLAG->m_vPOS.z
					>> dwTFLAG;

				for( DWORD j=0; j<dwTFLAG; j++)
				{
					DWORD dwTPATH;
					ar	>> dwTPATH;
				}

				if(!m_bInit)
				{
					WORD wUnitID = HIWORD(pTFLAG->m_dwID);

					m_bUnitX = LOBYTE(wUnitID);
					m_bUnitZ = HIBYTE(wUnitID);
					m_bInit = TRUE;
				}

				if( HIWORD(pTFLAG->m_dwID) == MAKEWORD( m_bUnitX, m_bUnitZ) )
					CPathFlag::RegTPATH( pTFLAG, m_bUnitX, m_bUnitZ);
			}
		}
	}
}

void CTPathMergeDlg::SaveTPATH()
{
	CFile file( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CArchive ar( &file, CArchive::store);

	MAPTPATHFLAG::iterator itTPATH;
	ar << DWORD(CPathFlag::m_mapTPATHFLAG.size());

	for( itTPATH = CPathFlag::m_mapTPATHFLAG.begin(); itTPATH != CPathFlag::m_mapTPATHFLAG.end(); itTPATH++)
	{
		ar	<< (*itTPATH).second->m_dwID
			<< (*itTPATH).second->m_vPOS.x
			<< (*itTPATH).second->m_vPOS.y
			<< (*itTPATH).second->m_vPOS.z
			<< DWORD(0);
	}
}
