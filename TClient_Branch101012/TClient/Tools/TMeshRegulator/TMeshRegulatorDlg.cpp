// TMeshRegulatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMeshRegulator.h"
#include "TMeshRegulatorDlg.h"
#include ".\tmeshregulatordlg.h"

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


// CTMeshRegulatorDlg dialog



CTMeshRegulatorDlg::CTMeshRegulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTMeshRegulatorDlg::IDD, pParent)
	, m_strSRC(_T(""))
	, m_strDEST(_T(""))
	, m_strTMF(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTMeshRegulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SRC, m_cSRC);
	DDX_Control(pDX, IDC_BUTTON_DEST, m_cDEST);
	DDX_Text(pDX, IDC_EDIT_SRC, m_strSRC);
	DDX_Text(pDX, IDC_EDIT_DEST, m_strDEST);
	DDX_Control(pDX, IDC_BUTTON_SRCFOLDER, m_cSRCFOLDER);
	DDX_Text(pDX, IDC_EDIT_SRCFOLDER, m_strTMF);
}

BEGIN_MESSAGE_MAP(CTMeshRegulatorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DEST, OnBnClickedButtonDest)
	ON_BN_CLICKED(IDC_BUTTON_SRC, OnBnClickedButtonSrc)
	ON_BN_CLICKED(ID_REGULATE, OnBnClickedRegulate)
	ON_BN_CLICKED(IDC_BUTTON_SRCFOLDER, OnBnClickedButtonSrcfolder)
END_MESSAGE_MAP()


// CTMeshRegulatorDlg message handlers

BOOL CTMeshRegulatorDlg::OnInitDialog()
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
	m_cSRCFOLDER.SetIcon(hIcon);
	m_cDEST.SetIcon(hIcon);
	m_cSRC.SetIcon(hIcon);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTMeshRegulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTMeshRegulatorDlg::OnPaint() 
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
HCURSOR CTMeshRegulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTMeshRegulatorDlg::OnBnClickedButtonDest()
{
	CTShell dlg;

	dlg.m_strSelDir = m_strDESTFOLDER;
	dlg.m_strTitle = _T("Target Folder");

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strDESTFOLDER = dlg.m_strPath;
		m_strDEST = dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CTMeshRegulatorDlg::OnBnClickedButtonSrcfolder()
{
	CTShell dlg;

	dlg.m_strSelDir = m_strTMFFOLDER;
	dlg.m_strTitle = _T("Mesh Folder");

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strTMFFOLDER = dlg.m_strPath;
		m_strTMF = dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CTMeshRegulatorDlg::OnBnClickedButtonSrc()
{
	CFileDialog dlg(
		TRUE,
		_T("*.tmf"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Tachyon Mesh Files (*.tmf)|*.tmf|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strSRCFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strSRCFOLDER = dlg.GetFileName();
		m_strSRC = dlg.GetPathName();
		m_strSRCFOLDER = m_strSRC.Left(m_strSRC.GetLength() - m_strSRCFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTMeshRegulatorDlg::OnBnClickedRegulate()
{
	D3DXVECTOR3 vCENTER;
	FLOAT fRADIUS;

	int nTMFVersion = 0;
	CFile vSRC;

	vSRC.Open( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeBinary);
	vSRC.Read( &nTMFVersion, sizeof(int));
	vSRC.Read( &m_dwNodeCount, sizeof(DWORD));

	vSRC.Read( &fRADIUS, sizeof(FLOAT));
	vSRC.Read( &vCENTER, sizeof(D3DXVECTOR3));

	D3DXMatrixIdentity(&m_vMAT[0]);
	if( m_dwNodeCount > 0 )
		for( DWORD i=0; i<m_dwNodeCount; i++)
			vSRC.Read( &m_vMAT[i + 1], sizeof(D3DMATRIX));
	vSRC.Close();

	Regulate( m_strTMF, m_strDEST);
}

BYTE CTMeshRegulatorDlg::IsMeshFile( CString strFileName)
{
	CString strExt;
	CTShell shell;

	strExt = shell.GetFileExt(strFileName);
	strExt.MakeUpper();
	strExt.TrimRight();
	strExt.TrimLeft();

	if( strExt == _T(".TMF") )
	{
		CFile vSRC( LPCSTR(strFileName), CFile::modeRead|CFile::typeBinary);
		DWORD dwNodeCount = 0;
		int nTMFVersion = 0;

		vSRC.Read( &nTMFVersion, sizeof(int));
		vSRC.Read( &dwNodeCount, sizeof(DWORD));

		if( m_dwNodeCount == dwNodeCount )
			return TRUE;
	}

	return FALSE;
}

void CTMeshRegulatorDlg::Regulate( CString strSRC, CString strDEST)
{
	CFileFind finder;
	CString strFIND;

	strDEST.TrimRight('\\');
	strSRC.TrimRight('\\');
	strFIND.Format(
		"%s\\*.*",
		strSRC);

	BOOL bFind = finder.FindFile(strFIND);
	while(bFind)
	{
		bFind = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			CreateDirectory( LPCSTR(strDEST + CString("\\") + finder.GetFileTitle()), NULL);
			Regulate(
				strSRC + CString("\\") + finder.GetFileTitle(),
				strDEST + CString("\\") + finder.GetFileTitle());

			continue;
		}

		if(IsMeshFile(finder.GetFilePath()))
		{
			CFile vDEST( LPCSTR(strDEST + CString("\\") + finder.GetFileName()), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
			CFile vSRC( LPCSTR(finder.GetFilePath()), CFile::modeRead|CFile::typeBinary);

			Regulate( &vSRC, &vDEST);
		}
	}
}

void CTMeshRegulatorDlg::Regulate( CFile *pSRC, CFile *pDEST)
{
	D3DXMATRIX vINV[MAX_PIVOT];
	int nNEWVersion = 301;
	int nTMFVersion = 0;

	D3DXVECTOR3 vCENTER;
	FLOAT fRADIUS;
	DWORD dwCount;

	pSRC->Read( &nTMFVersion, sizeof(int));
	pDEST->Write( &nNEWVersion, sizeof(int));

	pSRC->Seek( sizeof(DWORD), CFile::current);
	pDEST->Write( &m_dwNodeCount, sizeof(DWORD));

	pSRC->Read( &fRADIUS, sizeof(FLOAT));
	pDEST->Write( &fRADIUS, sizeof(FLOAT));

	pSRC->Read( &vCENTER, sizeof(D3DXVECTOR3));
	pDEST->Write( &vCENTER, sizeof(D3DXVECTOR3));

	if( m_dwNodeCount > 0 )
		for( DWORD i=0; i<m_dwNodeCount; i++)
		{
			pSRC->Read( &vINV[i], sizeof(D3DMATRIX));
			vINV[i] = CTMath::Inverse(&vINV[i]);
			pDEST->Write( &m_vMAT[i + 1], sizeof(D3DMATRIX));
		}

	pSRC->Read( &dwCount, sizeof(DWORD));
	pDEST->Write( &dwCount, sizeof(DWORD));

	for( DWORD i=0; i<dwCount; i++)
	{
		TMESHBUFFER vTBUF;
		WMESHVERTEX vDATA;

		pSRC->Read( &vTBUF, nTMFVersion < 301 ? sizeof(WMESHVERTEX) : sizeof(TMESHBUFFER));
		memcpy( &vDATA, &vTBUF, sizeof(WMESHVERTEX));

		if( nTMFVersion < 301 )
		{
			vTBUF.m_fU2 = vTBUF.m_fU1;
			vTBUF.m_fV2 = vTBUF.m_fV1;
		}

		if( m_dwNodeCount > 0 )
		{
			D3DXMATRIX vTRANS = CTMath::GetTransformMatrix(
				m_vMAT,
				vINV,
				&vDATA);

			D3DXVECTOR3 vNORMAL(
				vDATA.m_fNormalX,
				vDATA.m_fNormalY,
				vDATA.m_fNormalZ);

			D3DXVECTOR3 vPOS(
				vDATA.m_fPosX,
				vDATA.m_fPosY,
				vDATA.m_fPosZ);

			CTMath::Transform(
				&vTRANS,
				&vPOS);

			vTRANS._41 = 0.0f;
			vTRANS._42 = 0.0f;
			vTRANS._43 = 0.0f;

			CTMath::Transform(
				&vTRANS,
				&vNORMAL);
			vNORMAL /= D3DXVec3Length(&vNORMAL);

			vDATA.m_fNormalX = vNORMAL.x;
			vDATA.m_fNormalY = vNORMAL.y;
			vDATA.m_fNormalZ = vNORMAL.z;
			vDATA.m_fPosX = vPOS.x;
			vDATA.m_fPosY = vPOS.y;
			vDATA.m_fPosZ = vPOS.z;
		}

		memcpy( &vTBUF, &vDATA, sizeof(WMESHVERTEX));
		pDEST->Write( &vTBUF, sizeof(TMESHBUFFER));
	}

	pSRC->Read( &dwCount, sizeof(DWORD));
	pDEST->Write( &dwCount, sizeof(DWORD));

	for( i=0; i<dwCount; i++)
	{
		IBDATA vIB;

		pSRC->Read( &vIB.m_dwCount, sizeof(DWORD));
		pDEST->Write( &vIB.m_dwCount, sizeof(DWORD));

		vIB.m_pIB = new WORD[vIB.m_dwCount];
		pSRC->Read( vIB.m_pIB, vIB.m_dwCount * sizeof(WORD));
		pDEST->Write( vIB.m_pIB, vIB.m_dwCount * sizeof(WORD));
	}
}
