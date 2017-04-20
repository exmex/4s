// TMHBuilderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMHBuilder.h"
#include "TMHBuilderDlg.h"

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


// CTMHBuilderDlg dialog



CTMHBuilderDlg::CTMHBuilderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTMHBuilderDlg::IDD, pParent)
	, m_strLIST(_T(""))
	, m_strRES(_T(""))
	, m_strTMH(_T(""))
	, m_strTASK(_T(""))
	, m_strDETAIL(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_mapTMAPITEM.clear();
	m_pRES = NULL;
}

void CTMHBuilderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_cLIST);
	DDX_Control(pDX, IDC_BUTTON_RES, m_cRES);
	DDX_Control(pDX, IDC_BUTTON_TMH, m_cTMH);
	DDX_Text(pDX, IDC_EDIT_LIST, m_strLIST);
	DDX_Text(pDX, IDC_EDIT_RES, m_strRES);
	DDX_Text(pDX, IDC_EDIT_TMH, m_strTMH);
	DDX_Text(pDX, IDC_STATIC_PROGRESS, m_strTASK);
	DDX_Text(pDX, IDC_STATIC_DETAIL, m_strDETAIL);
}

BEGIN_MESSAGE_MAP(CTMHBuilderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LIST, OnBnClickedButtonList)
	ON_BN_CLICKED(IDC_BUTTON_RES, OnBnClickedButtonRes)
	ON_BN_CLICKED(IDC_BUTTON_TMH, OnBnClickedButtonTmh)
	ON_BN_CLICKED(IDC_BUTTON_BUILD, OnBnClickedButtonBuild)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTMHBuilderDlg message handlers

BOOL CTMHBuilderDlg::OnInitDialog()
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

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_cLIST.SetIcon(hIcon);
	m_cRES.SetIcon(hIcon);
	m_cTMH.SetIcon(hIcon);
	InitDevice();
	ReleaseDATA();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTMHBuilderDlg::OnDestroy()
{
	ReleaseDevice();
	CDialog::OnDestroy();
}

void CTMHBuilderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTMHBuilderDlg::OnPaint() 
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
HCURSOR CTMHBuilderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTMHBuilderDlg::ReleaseDevice()
{
	CTachyonRes::m_pDEVICE = NULL;
	CD3DLight::SetDevice(NULL);
	m_vDEVICE.ReleaseDevice();
}

void CTMHBuilderDlg::InitDevice()
{
	CWnd *pDeskTop = GetDesktopWindow();
	CRect rcScreen;

	pDeskTop->GetWindowRect(&rcScreen);
	ReleaseDevice();

	m_vDEVICE.m_option.m_dwScreenX = rcScreen.Width();
	m_vDEVICE.m_option.m_dwScreenY = rcScreen.Height();
	m_vDEVICE.m_option.m_bWindowedMode = TRUE;
	m_vDEVICE.m_option.m_bUseSHADER = FALSE;
	m_vDEVICE.InitDevices(pDeskTop);

	CD3DLight::SetDevice(m_vDEVICE.m_pDevice);
	CTachyonRes::m_pDEVICE = &m_vDEVICE;

	CTachyonHUGEMAP::m_nDiffuseCount = DEF_DIFFUSECNT;
	CTachyonHUGEMAP::m_nMaxLength = DEF_MAXLENGTH;
	CTachyonHUGEMAP::m_nMaxLevel = DEF_MAXLEVEL;
	CTachyonHUGEMAP::m_nLODBound = DEF_LODBOUND;
	CTachyonHUGEMAP::m_nIndexCount = DEF_INDEXCNT;
	CTachyonHUGEMAP::m_fCullDIST = TCAM_LENGTH;
}

void CTMHBuilderDlg::OnBnClickedButtonList()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Map List Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strLISTFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strLISTFOLDER = dlg.GetFileName();
		m_strLIST = dlg.GetPathName();
		m_strLISTFOLDER = m_strLIST.Left(m_strLIST.GetLength() - m_strLISTFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTMHBuilderDlg::OnBnClickedButtonRes()
{
	CTShell dlg;

	dlg.m_strSelDir = m_strRESFOLDER;
	dlg.m_strTitle = _T("Target Folder");

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strRESFOLDER = dlg.m_strPath;
		m_strRES = dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CTMHBuilderDlg::OnBnClickedButtonTmh()
{
	CTShell dlg;

	dlg.m_strSelDir = m_strTMHFOLDER;
	dlg.m_strTitle = _T("Target Folder");

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strTMHFOLDER = dlg.m_strPath;
		m_strTMH = dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CTMHBuilderDlg::OnBnClickedButtonBuild()
{
	LoadDATA();
	InitNAV();
	BuildDATA();
	ReleaseDATA();
}

void CTMHBuilderDlg::ReleaseDATA()
{
	if(m_pRES)
	{
		delete m_pRES;
		m_pRES = NULL;
	}

	CTMHBuilderMAP::ReleaseMAPINFO();
	CTachyonMesh::ReleaseGlobalVB();
	ReleaseLIST();

	SetProgressText( "", "");
}

void CTMHBuilderDlg::BuildDATA()
{
	MAPTMAPITEM::iterator it;

	for( it = m_mapTMAPITEM.begin(); it != m_mapTMAPITEM.end(); it++)
	{
		LPTMAPITEM pTMAPITEM = (*it).second;
		CTMHBuilderMAP vMAP;
		WORD wMapID = (*it).first;

		if(vMAP.LockMAP( m_pRES, wMapID))
		{
			VECTORDWORD vUnitID;
			vUnitID.clear();

			if(pTMAPITEM->m_bAll)
			{
				int nCount = vMAP.m_pMAP->m_nUnitCountX * vMAP.m_pMAP->m_nUnitCountZ;

				for( int i=0; i<nCount; i++)
					if(vMAP.m_pMAP->m_pENABLE[i])
						vUnitID.push_back(i);
			}
			else
			{
				for( int i=0; i<INT(pTMAPITEM->m_vUNIT.size()); i++)
				{
					BYTE bUnitX = LOBYTE(pTMAPITEM->m_vUNIT[i]);
					BYTE bUnitZ = HIBYTE(pTMAPITEM->m_vUNIT[i]);
					int nUnitID = INT(bUnitZ) * vMAP.m_pMAP->m_nUnitCountX + INT(bUnitX);

					if(vMAP.m_pMAP->m_pENABLE[nUnitID])
						vUnitID.push_back(nUnitID);
				}
			}

			for( int i=0; i<INT(vUnitID.size()); i++)
			{
				BYTE bUnitX = BYTE(vUnitID[i] % vMAP.m_pMAP->m_nUnitCountX);
				BYTE bUnitZ = BYTE(vUnitID[i] / vMAP.m_pMAP->m_nUnitCountX);

				CString strDETAIL;
				CString strTASK;

				strDETAIL.Format( _T("UnitX = %d, UnitZ = %d"), bUnitX, bUnitZ);
				strTASK.Format( _T("TMH 빌드 (Map ID = %d)"), wMapID);

				SetProgressText(
					strTASK,
					strDETAIL);

				vMAP.LoadUNIT(
					&m_vDEVICE,
					vUnitID[i]);

				BuildTMH(
					&vMAP.m_mapTHEIGHTINFO,
					wMapID,
					MAKEWORD(
					bUnitX,
					bUnitZ));
			}
		}
	}
}

void CTMHBuilderDlg::BuildTMH( LPMAPTHEIGHTINFO pTHEIGHTINFO,
							   WORD wMapID,
							   WORD wUnitID)
{
	MAPTHEIGHTINFO::iterator itINFO;
	CString strFileName;

	m_strTMH.TrimRight("\\");
	strFileName.Format( "%s\\MH%08x.tmh", m_strTMH, MAKELONG( wUnitID, wMapID));
	CFile file( strFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	DWORD dwCount = DWORD(pTHEIGHTINFO->size());
	file.Write( &dwCount, sizeof(DWORD));

	for( itINFO = pTHEIGHTINFO->begin(); itINFO != pTHEIGHTINFO->end(); itINFO++)
	{
		dwCount = DWORD((*itINFO).second->size());
		file.Write( &((*itINFO).first), sizeof(DWORD));
		file.Write( &dwCount, sizeof(DWORD));

		for( DWORD i=0; i<dwCount; i++)
			file.Write( &((*((*itINFO).second))[i]), sizeof(DWORD));
	}
}

void CTMHBuilderDlg::InitNAV()
{
	CTMHBuilderOBJ vOBJ;
	vOBJ.InitOBJ(m_pRES->GetOBJ(ID_MAPNAV_SETTING));

	CTMHBuilderMAP::m_fClimbBound = vOBJ.GetAttrFLOAT(ID_CLIMB_BOUND);
	CTMHBuilderMAP::m_fMoveBound = vOBJ.GetAttrFLOAT(ID_MOVE_BOUND);
	CTMHBuilderMAP::m_fFallBound = vOBJ.GetAttrFLOAT(ID_FALL_BOUND);
	CTMHBuilderMAP::m_fGravity = vOBJ.GetAttrFLOAT(ID_GRAVITY);
}

void CTMHBuilderDlg::LoadDATA()
{
	static CString vTASK[] = {
		CString("리스트 파일 로딩"),
		CString("리소스 로딩")};

	static CString vDETAIL[] = {
		CString("텍스쳐 로딩"),
		CString("이미지 로딩"),
		CString("미디어 로딩"),
		CString("애니메이션 로딩"),
		CString("메쉬 로딩"),
		CString("오브젝트 로딩"),
		CString("이펙트 로딩"),
		CString("맵 로딩"),
		CString("리소스 정리")};
	ReleaseDATA();

	SetProgressText(
		vTASK[0],
		vTASK[0]);
	LoadLIST();

	SetCurrentDirectory(m_strRES);
	CTMHBuilderMAP::InitMAPINFO();
	CTachyonMesh::InitGlobalVB();
	m_pRES = new CTachyonRes();

	SetProgressText(
		vTASK[1],
		vDETAIL[0]);
	m_pRES->LoadTEX(m_strGroupID);
	SetProgressText(
		vTASK[1],
		vDETAIL[1]);
	m_pRES->LoadIMGBUF(m_strGroupID);
	m_pRES->LoadIMG(m_strGroupID);
	SetProgressText(
		vTASK[1],
		vDETAIL[2]);
	m_pRES->LoadMEDIA(m_strGroupID);
	SetProgressText(
		vTASK[1],
		vDETAIL[3]);
	m_pRES->LoadANI(m_strGroupID);
	SetProgressText(
		vTASK[1],
		vDETAIL[4]);
	m_pRES->LoadMESH(m_strGroupID);
	SetProgressText(
		vTASK[1],
		vDETAIL[5]);
	m_pRES->LoadOBJ(m_strGroupID);
	SetProgressText(
		vTASK[1],
		vDETAIL[6]);
	m_pRES->LoadSFX(m_strGroupID);
	SetProgressText(
		vTASK[1],
		vDETAIL[7]);
	m_pRES->LoadMAP(m_strGroupID);
	SetProgressText(
		vTASK[1],
		vDETAIL[8]);
	m_pRES->Complate();
}

void CTMHBuilderDlg::ReleaseLIST()
{
	MAPTMAPITEM::iterator it;

	for( it = m_mapTMAPITEM.begin(); it != m_mapTMAPITEM.end(); it++)
		delete (*it).second;

	m_mapTMAPITEM.clear();
	m_strGroupID.Empty();
}

void CTMHBuilderDlg::LoadLIST()
{
	CStdioFile vLIST( LPCSTR(m_strLIST), CFile::modeRead|CFile::typeText);
	ReleaseLIST();

	CString strDATA;
	DWORD dwCount;

	m_strGroupID = ReadString(&vLIST);
	strDATA = ReadString(&vLIST);

	sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);
	for( DWORD i=0; i<dwCount; i++)
	{
		LPTMAPITEM pTMAPITEM = new TMAPITEM();
		DWORD dwMapID;
		DWORD dwUnit;
		DWORD dwAll;

		strDATA = ReadString(&vLIST);
		sscanf( LPCSTR(strDATA), _T("%d %d %d"),
			&dwMapID,
			&dwAll,
			&dwUnit);
		pTMAPITEM->m_bAll = BYTE(dwAll);

		for( DWORD j=0; j<dwUnit; j++)
		{
			DWORD dwUnitX;
			DWORD dwUnitZ;

			strDATA = ReadString(&vLIST);
			sscanf( LPCSTR(strDATA), _T("%d %d"),
				&dwUnitX,
				&dwUnitZ);

			pTMAPITEM->m_vUNIT.push_back(MAKEWORD(
				BYTE(dwUnitX),
				BYTE(dwUnitZ)));
		}

		m_mapTMAPITEM.insert( MAPTMAPITEM::value_type( WORD(dwMapID), pTMAPITEM));
	}
}

CString CTMHBuilderDlg::ReadString( CStdioFile *pFILE)
{
	CString strRESULT;

	do
	{
		if(!pFILE->ReadString(strRESULT))
		{
			strRESULT.Empty();
			return strRESULT;
		}

		strRESULT.TrimRight();
		strRESULT.TrimLeft();
	} while( strRESULT.IsEmpty() || strRESULT.GetAt(0) == ';' );

	return strRESULT;
}

void CTMHBuilderDlg::SetProgressText( CString strTASK,
									  CString strDETAIL)
{
	m_strDETAIL = strDETAIL;
	m_strTASK = strTASK;

	UpdateData(FALSE);
}
