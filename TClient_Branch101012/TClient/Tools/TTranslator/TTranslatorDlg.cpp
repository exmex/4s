// TTranslatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TTranslator.h"
#include "TProgressDlg.h"
#include "TTranslatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString CTTranslatorDlg::m_vTDBNAME[TACCESSID_COUNT] = {
	CString(_T("ITEM_KIND")),
	CString(_T("SKILL_TAB")),
	CString(_T("INFO_TEXT"))};

CString CTTranslatorDlg::m_vTDOCKIND[TDOC_COUNT] = {
	CString(_T("TDOC_NONE")),
	CString(_T("TDOC_TRANSLATTION")),
	CString(_T("TDOC_TCLIENT")),
	CString(_T("TDOC_INTERFACE")),
	CString(_T("TDOC_INFOCHART")),
	CString(_T("TDOC_EVENTQUARTER")),
	CString(_T("TDOC_COUNTRYINFO")),
	CString(_T("TDOC_RACEINFO")),
	CString(_T("TDOC_CLASSINFO")),
	CString(_T("TDOC_DYNAMICHELP")),
	CString(_T("TDOC_HELP")),
	CString(_T("TDOC_ACCESSID")),
	CString(_T("TDOC_QUEST_OUTPUT")),
	CString(_T("TDOC_QUEST_OBJECTIVE")),
	CString(_T("TDOC_QUEST_CATEGORY")),
	CString(_T("TDOC_QUEST_CHAPTER")),
	CString(_T("TDOC_QUEST_CRAFT")),
	CString(_T("TDOC_QUEST_MATERIAL")),
	CString(_T("TDOC_MONSTER")),
	CString(_T("TDOC_SVRMSG")),
	CString(_T("TDOC_TOURNAMENT")),
	CString(_T("TDOC_TOURNAMENT_SCHEDULE"))};
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


// CTTranslatorDlg dialog



CTTranslatorDlg::CTTranslatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTTranslatorDlg::IDD, pParent)
	, m_nFROM(0)
	, m_nTO(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bCANCEL = FALSE;
	m_nTOTAL = 0;
	m_nTSTEP = 0;

	InitializeCriticalSection(&m_cs);
	ReleaseDB();
}

void CTTranslatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEXT_FOLDER, m_strTEXT);
	DDX_Text(pDX, IDC_EDIT_SOURCE, m_strSOURCE);
	DDX_Text(pDX, IDC_EDIT_TARGET, m_strTARGET);
	DDX_Control(pDX, IDC_BUTTON_TEXT_FOLDER, m_cTEXT);
	DDX_Control(pDX, IDC_BUTTON_SOURCE, m_cSOURCE);
	DDX_Control(pDX, IDC_BUTTON_TARGET, m_cTARGET);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE_FROM, m_cLANG_FROM);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE_TO, m_cLANG_TO);
	DDX_CBIndex(pDX, IDC_COMBO_LANGUAGE_FROM, m_nFROM);
	DDX_CBIndex(pDX, IDC_COMBO_LANGUAGE_TO, m_nTO);
}

BEGIN_MESSAGE_MAP(CTTranslatorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_TEXT_FOLDER, OnBnClickedButtonTextFolder)
	ON_BN_CLICKED(IDC_BUTTON_SOURCE, OnBnClickedButtonSource)
	ON_BN_CLICKED(IDC_BUTTON_TARGET, OnBnClickedButtonTarget)
	ON_BN_CLICKED(IDC_BUTTON_BUILD, OnBnClickedButtonBuild)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTTranslatorDlg message handlers

BOOL CTTranslatorDlg::OnInitDialog()
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
	m_cSOURCE.SetIcon(hIcon);
	m_cTARGET.SetIcon(hIcon);
	m_cTEXT.SetIcon(hIcon);

	int nCount = INT(m_vTLANGUAGE[TLANG_CODE_NAME].size());
	for( int i=0; i<nCount; i++)
	{
		m_cLANG_FROM.AddString(m_vTLANGUAGE[TLANG_CODE_NAME][i]);
		m_cLANG_TO.AddString(m_vTLANGUAGE[TLANG_CODE_NAME][i]);
	}

	if( m_nFROM < 0 || nCount <= m_nFROM )
		m_nFROM = 0;

	if( m_nTO < 0 || nCount <= m_nTO )
		m_nTO = 0;

	UpdateData(FALSE);
	ReleaseDB();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTTranslatorDlg::OnDestroy()
{
	DeleteCriticalSection(&m_cs);
	ReleaseDB();
	CDialog::OnDestroy();
}

void CTTranslatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTTranslatorDlg::OnPaint() 
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
HCURSOR CTTranslatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTTranslatorDlg::OnBnClickedButtonTextFolder()
{
	CTShell dlg;

	dlg.m_strTitle = _T("Text Folder");
	dlg.m_strSelDir = m_strTEXT;

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strTEXT = dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CTTranslatorDlg::OnBnClickedButtonSource()
{
	CTShell dlg;

	dlg.m_strTitle = _T("Source Folder");
	dlg.m_strSelDir = m_strSOURCE;

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strSOURCE = dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CTTranslatorDlg::OnBnClickedButtonTarget()
{
	CTShell dlg;

	dlg.m_strTitle = _T("Target Folder");
	dlg.m_strSelDir = m_strTARGET;

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strTARGET = dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CTTranslatorDlg::OnBnClickedButtonBuild()
{
	CTProgressDlg dlg;
	TBDPARAM vPARAM;

	UpdateData();
	m_strSOURCE.TrimRight('\\');
	m_strTARGET.TrimRight('\\');
	m_strTEXT.TrimRight('\\');

	ShowWindow(SW_SHOWMINIMIZED);
	vPARAM.m_pPROGRESS = &dlg;
	vPARAM.m_pHOST = this;

	dlg.m_pTHREAD = (LPTHREAD_START_ROUTINE) __Build;
	dlg.m_lParam = &vPARAM;
	dlg.DoModal();
	ShowWindow(SW_RESTORE);

	AfxMessageBox(GetCANCEL() ? IDS_FMT_BUILD_CANCEL : IDS_FMT_BUILD_COMPLETED);
	SetCANCEL(FALSE);
}

DWORD CTTranslatorDlg::__Build( DWORD_PTR lParam)
{
	if( lParam && ((LPTBDPARAM) lParam)->m_pHOST && ((LPTBDPARAM) lParam)->m_pPROGRESS )
		((CTTranslatorDlg *) ((LPTBDPARAM) lParam)->m_pHOST)->Build((LPTBDPARAM) lParam);

	return 0;
}

void CTTranslatorDlg::Build( LPTBDPARAM lParam)
{
	CString strDATA;
	strDATA.Format( IDS_FMT_LOG_FILE, m_strTARGET);

	m_nTOTAL = GetTotalStep();
	m_nTSTEP = 0;

	CFile vLOG( strDATA, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	OpenUnicode(&vLOG);

	TEXEC(LogString( &vLOG, IDS_LOG_BEGIN_BUILD, CP_ACP));
	TEXEC(LogString( &vLOG, IDS_LOG_TEXT_PATH, m_strTEXT, CP_ACP));
	TEXEC(LogString( &vLOG, IDS_LOG_SOURCE_PATH, m_strSOURCE, CP_ACP));
	TEXEC(LogString( &vLOG, IDS_LOG_TARGET_PATH, m_strTARGET, CP_ACP));
	TEXEC(LogString( &vLOG, _T(""), CP_ACP));
	LoadDB( &vLOG, lParam);
	TEXEC(LogString( &vLOG, _T(""), CP_ACP));

	strDATA.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	CreateDirectory( LPCSTR(strDATA), NULL);

	strDATA.Format( IDS_FMT_PATH_SERVER, m_strTARGET);
	CreateDirectory( LPCSTR(strDATA), NULL);

	strDATA.Format( IDS_FMT_PATH_TCD, m_strTARGET);
	CreateDirectory( LPCSTR(strDATA), NULL);

	TEXEC(LogString( &vLOG, IDS_LOG_BEGIN_CONVERT, CP_ACP));
	TEXEC(BuildTPORTALREGIONCHART_TCD( &vLOG, lParam));
	TEXEC(BuildTMONSTERSHOPCHART_TCD( &vLOG, lParam));
	TEXEC(BuildTITEMMAGICCHART_TCD( &vLOG, lParam));
	TEXEC(BuildTMONTHRANKCHART_TCD( &vLOG, lParam));
	TEXEC(BuildTGODTOWERCHART_TCD( &vLOG, lParam));
	TEXEC(BuildTGODBALLCHART_TCD( &vLOG, lParam));
	TEXEC(BuildTAUCTIONCHART_TCD( &vLOG, lParam));
	TEXEC(BuildTMONSTERCHART_TCD( &vLOG, lParam));
	TEXEC(BuildTPORTALCHART_TCD( &vLOG, lParam));
	TEXEC(BuildTSKILLCHART_TCD( &vLOG, lParam));
	TEXEC(BuildTITEMCHART_TCD( &vLOG, lParam));
	TEXEC(BuildTNPCCHART_TCD( &vLOG, lParam));

	TEXEC(BuildTAction_Define_Chart_TCD( &vLOG, lParam));
	TEXEC(BuildTRegionChart_TCD( &vLOG, lParam));
	TEXEC(BuildTCountry_Info_TCD( &vLOG, lParam));
	TEXEC(BuildTInfo_Chart_TCD( &vLOG, lParam));
	TEXEC(BuildTClass_Info_TCD( &vLOG, lParam));
	TEXEC(BuildTMenu_Item_TCD( &vLOG, lParam));
	TEXEC(BuildTRace_Info_TCD( &vLOG, lParam));
	TEXEC(BuildTString_TCD( &vLOG, lParam));
	TEXEC(BuildTHelp_TCD( &vLOG, lParam));

	TEXEC(BuildTIF( &vLOG, lParam));
	TEXEC(BuildMPQ( &vLOG, lParam));

	TEXEC(BuildTEVENTQUARTERCHART_SQL( &vLOG, lParam));
	TEXEC(BuildTQUEST_SQL( &vLOG, lParam));
	TEXEC(BuildTGAME_SQL( &vLOG, lParam));

	ReleaseDB();
	TEXEC(LogString( &vLOG, IDS_LOG_END_CONVERT, CP_ACP));
	TEXEC(LogString( &vLOG, IDS_LOG_END_BUILD, CP_ACP));

	if(GetCANCEL())
		LogString( &vLOG, IDS_LOG_BUILD_CANCEL, CP_ACP);
	SetActionText( lParam, IDS_FMT_WAIT);

	if( lParam && lParam->m_pPROGRESS )
		lParam->m_pPROGRESS->PostMessage( WM_COMMAND, ID_MENU_CLOSE);
}

void CTTranslatorDlg::BuildTPORTALREGIONCHART_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_PORTAL_REGION_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_PORTAL_REGION_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strNAME;
		WORD wID;

		vFrom >> wID >> strNAME;
		vTo << wID << Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTMONSTERSHOPCHART_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_MONSTER_SHOP_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_MONSTER_SHOP_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strNAME;

		vFrom >> strNAME;
		strNAME.Replace( TDEF_REPLACE_TO, TDEF_REPLACE_FROM);
		strNAME = Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);
		strNAME.Replace( TDEF_REPLACE_FROM, TDEF_REPLACE_TO);
		vTo << strNAME;

		CopyDWORD( &vFrom, &vTo, 3);
		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTITEMMAGICCHART_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_ITEM_MAGIC_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_ITEM_MAGIC_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strNAME;

		vFrom >> strNAME;
		vTo << Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);

		CopyWORD( &vFrom, &vTo, 1);
		CopyBYTE( &vFrom, &vTo, 1);

		CopyFLOAT( &vFrom, &vTo, 1);
		CopyBYTE( &vFrom, &vTo, 1);

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTMONTHRANKCHART_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_MONTH_RANK_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_MONTH_RANK_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strNAME;

		CopyBYTE( &vFrom, &vTo, 1);
		vFrom >> strNAME;
		strNAME.Replace( TDEF_REPLACE_TO, TDEF_REPLACE_FROM);
		strNAME = Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);
		strNAME.Replace( TDEF_REPLACE_FROM, TDEF_REPLACE_TO);
		vTo << strNAME;

		CopyBYTE( &vFrom, &vTo, 2);
		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTGODTOWERCHART_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_GOD_TOWER_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_GOD_TOWER_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strNAME;

		vFrom >> strNAME;
		strNAME.Replace( TDEF_REPLACE_TO, TDEF_REPLACE_FROM);
		strNAME = Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);
		strNAME.Replace( TDEF_REPLACE_FROM, TDEF_REPLACE_TO);
		vTo << strNAME;

		CopyDWORD( &vFrom, &vTo, 1);
		CopyFLOAT( &vFrom, &vTo, 1);
		CopyDWORD( &vFrom, &vTo, 5);

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTGODBALLCHART_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_GOD_BALL_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_GOD_BALL_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strNAME;

		vFrom >> strNAME;
		strNAME.Replace( TDEF_REPLACE_TO, TDEF_REPLACE_FROM);
		strNAME = Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);
		strNAME.Replace( TDEF_REPLACE_FROM, TDEF_REPLACE_TO);
		vTo << strNAME;

		CopyDWORD( &vFrom, &vTo, 1);
		CopyFLOAT( &vFrom, &vTo, 1);
		CopyDWORD( &vFrom, &vTo, 3);

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTAUCTIONCHART_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_AUCTION_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_AUCTION_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strNAME;

		vFrom >> strNAME;
		strNAME.Replace( TDEF_REPLACE_TO, TDEF_REPLACE_FROM);
		strNAME = Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);
		strNAME.Replace( TDEF_REPLACE_FROM, TDEF_REPLACE_TO);
		vTo << strNAME;

		CopyDWORD( &vFrom, &vTo, 1);
		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTMONSTERCHART_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_MONSTER_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_MONSTER_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
		TranslateMON( lParam, pLOG, &vFrom, &vTo, &dwConvert);

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTPORTALCHART_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_PORTAL_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_PORTAL_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strNAME;

		CopyWORD( &vFrom, &vTo, 1);
		vFrom >> strNAME;
		vTo << Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);

		CopyWORD( &vFrom, &vTo, 1);
		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTSKILLCHART_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_SKILL_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_SKILL_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strNAME;
		CString strTEXT;
		WORD wID;

		vFrom >> wID;
		vTo << wID;

		vFrom >> strNAME;
		strNAME = Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);
		vTo << strNAME;

		strNAME.Replace( _T("'"), _T("''"));
		strTEXT.Format( IDS_FMT_SKILL_SQL, strNAME, wID);
		m_vTSKILL_SQL.push_back(strTEXT);

		CopyWORD( &vFrom, &vTo, 3);
		CopyDWORD( &vFrom, &vTo, 1);
		CopyBYTE( &vFrom, &vTo, 5);
		CopyFLOAT( &vFrom, &vTo, 1);
		CopyBYTE( &vFrom, &vTo, 1);
		CopyDWORD( &vFrom, &vTo, 1);
		CopyWORD( &vFrom, &vTo, 2);
		CopyBYTE( &vFrom, &vTo, 2);
		CopyDWORD( &vFrom, &vTo, 1);
		CopyBYTE( &vFrom, &vTo, 1);
		CopyDWORD( &vFrom, &vTo, 1);
		CopyINT( &vFrom, &vTo, 1);
		CopyDWORD( &vFrom, &vTo, 2);
		CopyBYTE( &vFrom, &vTo, 1);
		CopyWORD( &vFrom, &vTo, 1);
		CopyBYTE( &vFrom, &vTo, 2);
		CopyFLOAT( &vFrom, &vTo, 4);
		CopyDWORD( &vFrom, &vTo, 2);
		CopyBYTE( &vFrom, &vTo, 2);
		CopyWORD( &vFrom, &vTo, 3);
		CopyBYTE( &vFrom, &vTo, 6);
		CopyDWORD( &vFrom, &vTo, 8);
		CopyBYTE( &vFrom, &vTo, 5);
		CopyFLOAT( &vFrom, &vTo, 1);

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTITEMCHART_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_ITEM_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_ITEM_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strNAME;
		CString strTEXT;
		WORD wID;

		vFrom >> wID;
		vTo << wID;

		CopyBYTE( &vFrom, &vTo, 2);
		CopyWORD( &vFrom, &vTo, 1);

		vFrom >> strNAME;
		strNAME = Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);
		vTo << strNAME;

		strNAME.Replace( _T("'"), _T("''"));
		strTEXT.Format( IDS_FMT_ITEM_SQL, strNAME, wID);
		m_vTITEM_SQL.push_back(strTEXT);

		CopyWORD( &vFrom, &vTo, 1);
		CopyDWORD( &vFrom, &vTo, 2);
		CopyBYTE( &vFrom, &vTo, 4);
		CopyDWORD( &vFrom, &vTo, 1);
		CopyBYTE( &vFrom, &vTo, 1);
		CopyFLOAT( &vFrom, &vTo, 1);
		CopyDWORD( &vFrom, &vTo, 1);
		CopyBYTE( &vFrom, &vTo, 10);
		CopyWORD( &vFrom, &vTo, 1);
		CopyDWORD( &vFrom, &vTo, 1);
		CopyBYTE( &vFrom, &vTo, 2);
		CopyWORD( &vFrom, &vTo, 1);
		CopyBYTE( &vFrom, &vTo, 2);
		CopyFLOAT( &vFrom, &vTo, 2);
		CopyDWORD( &vFrom, &vTo, 1);
		CopyBYTE( &vFrom, &vTo, 1);
		CopyWORD( &vFrom, &vTo, 9);
		CopyBYTE( &vFrom, &vTo, 1);
		CopyDWORD( &vFrom, &vTo, 1);
		CopyBYTE( &vFrom, &vTo, 1);

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTNPCCHART_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CFileFind finder;
	CTShell vTSHELL;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_NPC_TCD, m_strSOURCE);
	BYTE bFIND = finder.FindFile(vTSHELL.m_strPath);

	while(bFIND)
	{
		bFIND = finder.FindNextFile();
		vTSHELL.m_strPath = finder.GetFilePath();

		if( finder.GetFileName() != TDEF_FILE_NPCTEMP )
		{
			CFile vFROM;
			CFile vTO;

			LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
			SetActionText( lParam, IDS_FMT_ACTION_BUILD_SUB, _T("TNPC"), vTSHELL.GetFileName(vTSHELL.m_strPath));
			m_nTSTEP++;

			if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
			{
				LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
				LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
				LogString( pLOG, _T(""), CP_ACP);

				return;
			}

			vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_TCD, m_strTARGET);
			vTSHELL.m_strPath.Format( IDS_FMT_PATH, vTSHELL.m_strInitDir, finder.GetFileName());

			if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
			{
				LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
				LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
				LogString( pLOG, _T(""), CP_ACP);

				return;
			}

			CArchive vFrom( &vFROM, CArchive::load);
			CArchive vTo( &vTO, CArchive::store);

			DWORD dwConvert = 0;
			WORD wCount;

			UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
			vFrom >> wCount;
			vTo << wCount;

			for( WORD i=0; i<wCount; i++)
			{
				CString strTITLE;
				CString strNAME;
				CString strTEXT;

				DWORD dwID;

				vFrom >> dwID;
				vTo << dwID;

				CopyWORD( &vFrom, &vTo, 1);

				vFrom >> strNAME;
				strNAME = Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);
				vTo << strNAME;

				vFrom >> strTITLE;
				strTITLE = Translate( &m_mapTRANSLATION, pLOG, strTITLE, &dwConvert);
				vTo << strTITLE;

				strTEXT.Format( IDS_FMT_NPC_NAME, strNAME);
				m_mapNPCNAME.insert( MAPNAME::value_type( dwID, strTEXT));

				strTITLE.Replace( _T("'"), _T("''"));
				strNAME.Replace( _T("'"), _T("''"));

				strTEXT.Format( IDS_FMT_NPC_SQL, strTITLE, strNAME, dwID);
				m_vTNPC_SQL.push_back(strTEXT);

				CopyBYTE( &vFrom, &vTo, 12);
				CopyDWORD( &vFrom, &vTo, 4);
				CopyWORD( &vFrom, &vTo, 1);
				CopyDWORD( &vFrom, &vTo, 1);
				CopyFLOAT( &vFrom, &vTo, 10);
				CopyBYTE( &vFrom, &vTo, 1);

				UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
			}

			vFrom.Close();
			vTo.Close();

			vFROM.Close();
			vTO.Close();

			LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
			LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
			LogString( pLOG, _T(""), CP_ACP);
		}
	}
}

void CTTranslatorDlg::BuildTAction_Define_Chart_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_ACTION_DEFINE_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_ACTION_DEFINE_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strTCMD;

		vFrom >> strTCMD;
		vTo << Translate( &m_mapTRANSLATION, pLOG, strTCMD, &dwConvert);

		CopyBYTE( &vFrom, &vTo, 8);
		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTRegionChart_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_REGION_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_REGION_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strNAME;

		CopyDWORD( &vFrom, &vTo, 1);
		vFrom >> strNAME;
		vTo << Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);

		CopyBYTE( &vFrom, &vTo, 2);
		CopyWORD( &vFrom, &vTo, 1);
		CopyFLOAT( &vFrom, &vTo, 9);
		CopyBYTE( &vFrom, &vTo, 1);
		CopyDWORD( &vFrom, &vTo, 3);
		CopyBYTE( &vFrom, &vTo, 1);

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTCountry_Info_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_COUNTRY_INFO_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_COUNTRY_INFO_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strTEXT;
		DWORD dwID;

		vFrom >> strTEXT;
		vFrom >> dwID;

		LPTDICBUFF pTDICBUFF = FindTDICBUF(
			pLOG,
			dwID,
			&m_mapTCOUNTRY);
		LPTDICDATA pTDATA = pTDICBUFF && !pTDICBUFF->m_vTDIC.empty() ? pTDICBUFF->m_vTDIC[0] : NULL;

		strTEXT.Replace( TDEF_REPLACE_TO, TDEF_REPLACE_FROM);
		strTEXT = Translate( pTDATA, pLOG, strTEXT, &dwConvert);
		strTEXT.Replace( TDEF_REPLACE_FROM, TDEF_REPLACE_TO);

		vTo << strTEXT;
		vTo << dwID;

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTInfo_Chart_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_INFO_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_INFO_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
		TranslateINFO( lParam, pLOG, &vFrom, &vTo, &dwConvert);

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTClass_Info_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_CLASS_INFO_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_CLASS_INFO_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strTEXT;
		DWORD dwID;

		vFrom >> strTEXT;
		vFrom >> dwID;

		LPTDICBUFF pTDICBUFF = FindTDICBUF(
			pLOG,
			dwID,
			&m_mapTCLASS);
		LPTDICDATA pTDATA = pTDICBUFF && !pTDICBUFF->m_vTDIC.empty() ? pTDICBUFF->m_vTDIC[0] : NULL;

		strTEXT.Replace( TDEF_REPLACE_TO, TDEF_REPLACE_FROM);
		strTEXT = Translate( pTDATA, pLOG, strTEXT, &dwConvert);
		strTEXT.Replace( TDEF_REPLACE_FROM, TDEF_REPLACE_TO);

		vTo << strTEXT;
		vTo << dwID;

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTMenu_Item_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_MENU_ITEM_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_MENU_ITEM_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strNAME;

		vFrom >> strNAME;
		vTo << Translate( &m_mapTRANSLATION, pLOG, strNAME, &dwConvert);

		CopyDWORD( &vFrom, &vTo, 2);
		CopyBYTE( &vFrom, &vTo, 1);

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTRace_Info_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_RACE_INFO_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_RACE_INFO_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strTEXT;
		DWORD dwID;

		vFrom >> strTEXT;
		vFrom >> dwID;

		LPTDICBUFF pTDICBUFF = FindTDICBUF(
			pLOG,
			dwID,
			&m_mapTRACE);
		LPTDICDATA pTDATA = pTDICBUFF && !pTDICBUFF->m_vTDIC.empty() ? pTDICBUFF->m_vTDIC[0] : NULL;

		strTEXT.Replace( TDEF_REPLACE_TO, TDEF_REPLACE_FROM);
		strTEXT = Translate( pTDATA, pLOG, strTEXT, &dwConvert);
		strTEXT.Replace( TDEF_REPLACE_FROM, TDEF_REPLACE_TO);

		vTo << strTEXT;
		vTo << dwID;

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTString_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_TSTRING_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_TSTRING_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strFROM;
		CString strTO;
		WORD wID;

		vFrom >> wID;
		vTo << wID;

		vFrom >> strFROM;
		if(wID)
		{
			LPTDICBUFF pTDICBUFF = FindTDICBUF(
				NULL,
				wID,
				&m_mapTCLIENT);
			LPTDICDATA pTDATA = pTDICBUFF && !pTDICBUFF->m_vTDIC.empty() ? pTDICBUFF->m_vTDIC[0] : NULL;

			strFROM.Replace( TDEF_REPLACE_TO, TDEF_REPLACE_FROM);
			strTO = pTDATA ? Translate( pTDATA, pLOG, strFROM, &dwConvert) : strFROM;

			if( GetSimpleFormat(strFROM) != GetSimpleFormat(strTO) )
			{
				LogString( pLOG, IDS_LOG_ERR_INVALID_FORMAT_STRING, wID, CP_ACP);
				strTO = GetFormat(strFROM);
			}
		}
		else
			strTO = m_vTLANGUAGE[TLANG_CODE_CODE][m_nTO];

		strTO.Replace( TDEF_REPLACE_FROM, TDEF_REPLACE_TO);
		vTo << strTO;

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTHelp_TCD( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_HELP_TCD, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_HELP_TCD, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	CArchive vFrom( &vFROM, CArchive::load);
	CArchive vTo( &vTO, CArchive::store);

	DWORD dwConvert = 0;
	WORD wCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	vFrom >> wCount;
	vTo << wCount;

	for( WORD i=0; i<wCount; i++)
	{
		CString strTITLE;
		CString strTEXT;
		CString strID;

		DWORD dwID;
		WORD wPage;

		vFrom >> dwID;
		vTo << dwID;

		vFrom >> wPage;
		vTo << wPage;

		CopyWORD( &vFrom, &vTo, 1);
		vFrom >> strTITLE;
		vFrom >> strTEXT;

		strID.Format(
			IDS_FMT_INTERFACE_ID,
			dwID,
			wPage);

		LPTDICBUFF pTDICBUFF = FindTDICBUF(
			pLOG,
			strID,
			&m_mapTHELP);

		vTo << Translate( pTDICBUFF && pTDICBUFF->m_vTDIC.size() > 0 ? pTDICBUFF->m_vTDIC[0] : NULL, pLOG, strTITLE, &dwConvert);
		vTo << Translate( pTDICBUFF && pTDICBUFF->m_vTDIC.size() > 1 ? pTDICBUFF->m_vTDIC[1] : NULL, pLOG, strTEXT, &dwConvert);

		UpdateProgressBar( lParam, m_nTSTEP, &vFrom);
	}

	vFrom.Close();
	vTo.Close();

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTEVENTQUARTERCHART_SQL( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;
	CFile vTSQL;

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_SERVER, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_TEVENT_SQL, vTSHELL.m_strInitDir);
	LogString( pLOG, IDS_LOG_FMT_START_SQL_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vTSQL.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_SQL_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	MAPTSTRINGDICBUFF::iterator it;
	OpenUnicode(&vTSQL);

	DWORD dwTotal = DWORD(m_mapTEVENTQUATER.size());
	DWORD dwCount = 0;

	LogString( &vTSQL, IDS_FMT_START_TEVENT, m_vCODEPAGE[m_nTO]);
	UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);

	for( it = m_mapTEVENTQUATER.begin(); it != m_mapTEVENTQUATER.end(); it++)
	{
		CString strTEXT[TEVENT_TEXT_COUNT];
		CString strTSQL;

		DWORD dwDAY;
		DWORD dwH;
		DWORD dwM;

		sscanf( LPCSTR((*it).first), _T("%d %d %d"), &dwDAY, &dwH, &dwM);
		for( int i=0; i<TEVENT_TEXT_COUNT; i++)
		{
			strTEXT[i] = i < INT((*it).second->m_vTDIC.size()) ? (*it).second->m_vTDIC[i]->m_strTO : CString(_T(""));
			strTEXT[i].Replace( _T("'"), _T("''"));
		}

		strTSQL.Format(
			IDS_FMT_TEVENT_SQL,
			strTEXT[TEVENT_TEXT_TITLE],
			strTEXT[TEVENT_TEXT_MSG],
			strTEXT[TEVENT_TEXT_PRESENT],
			strTEXT[TEVENT_TEXT_ANNOUNCE],
			dwDAY, dwH, dwM);

		LogString( &vTSQL, strTSQL, m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}

	vTSQL.Close();
	LogString( pLOG, IDS_LOG_FMT_END_SQL_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTQUEST_SQL( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;
	CFile vTSQL;

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_SERVER, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_TQUEST_SQL, vTSHELL.m_strInitDir);
	LogString( pLOG, IDS_LOG_FMT_START_SQL_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vTSQL.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_SQL_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}
	OpenUnicode(&vTSQL);

	DWORD dwTotal = DWORD(m_vTQCATEGORY_SQL.size() + m_vTQTITLE_SQL.size() + m_vTQTERM_SQL.size());
	DWORD dwCount = 0;

	LogString( &vTSQL, IDS_FMT_START_QCLASS, m_vCODEPAGE[m_nTO]);
	UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);

	for( int i=0; i<INT(m_vTQCATEGORY_SQL.size()); i++)
	{
		LogString( &vTSQL, m_vTQCATEGORY_SQL[i], m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}

	LogString( &vTSQL, _T(""), m_vCODEPAGE[m_nTO]);
	LogString( &vTSQL, IDS_FMT_START_QTITLE, m_vCODEPAGE[m_nTO]);

	for( i=0; i<INT(m_vTQTITLE_SQL.size()); i++)
	{
		LogString( &vTSQL, m_vTQTITLE_SQL[i], m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}

	LogString( &vTSQL, _T(""), m_vCODEPAGE[m_nTO]);
	LogString( &vTSQL, IDS_FMT_START_QTERM, m_vCODEPAGE[m_nTO]);

	for( i=0; i<INT(m_vTQTERM_SQL.size()); i++)
	{
		LogString( &vTSQL, m_vTQTERM_SQL[i], m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}

	vTSQL.Close();
	LogString( pLOG, IDS_LOG_FMT_END_SQL_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTGAME_SQL( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;
	CFile vTSQL;

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_SERVER, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_TGAME_SQL, vTSHELL.m_strInitDir);
	LogString( pLOG, IDS_LOG_FMT_START_SQL_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vTSQL.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_SQL_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}
	OpenUnicode(&vTSQL);

	MAPTDWORDDICBUFF::iterator itTDWORD;
	MAPTRANSLATION::iterator it;
	DWORD dwConvert = 0;

	DWORD dwTotal = DWORD(m_mapTBATTLE.size() + m_mapTDHELP.size() + m_vTMONSTER_SQL.size() + m_vTSKILL_SQL.size() + m_vTITEM_SQL.size() + m_vTNPC_SQL.size() + m_mapSVRMSG.size() + m_mapTOURNAMENT.size() + m_mapTOURNAMENTSCHEDULE.size() );
	DWORD dwCount = 0;

	LogString( &vTSQL, IDS_FMT_START_BATTLE_ZONE, m_vCODEPAGE[m_nTO]);
	UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);

	for( it = m_mapTBATTLE.begin(); it != m_mapTBATTLE.end(); it++)
	{
		CString strNAME = Translate( &m_mapTRANSLATION, pLOG, (*it).second, &dwConvert);
		CString strTSQL;
		WORD wID = (WORD) atoi((*it).first);

		strNAME.Replace( _T("'"), _T("''"));
		strTSQL.Format( IDS_FMT_TBATTLEZONE_SQL, strNAME, wID);
		LogString( &vTSQL, strTSQL, m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}

	LogString( &vTSQL, _T(""), m_vCODEPAGE[m_nTO]);
	LogString( &vTSQL, IDS_FMT_START_MONSTER, m_vCODEPAGE[m_nTO]);

	for( int i=0; i<INT(m_vTMONSTER_SQL.size()); i++)
	{
		LogString( &vTSQL, m_vTMONSTER_SQL[i], m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}

	LogString( &vTSQL, _T(""), m_vCODEPAGE[m_nTO]);
	LogString( &vTSQL, IDS_FMT_START_SKILL, m_vCODEPAGE[m_nTO]);

	for( i=0; i<INT(m_vTSKILL_SQL.size()); i++)
	{
		LogString( &vTSQL, m_vTSKILL_SQL[i], m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}

	LogString( &vTSQL, _T(""), m_vCODEPAGE[m_nTO]);
	LogString( &vTSQL, IDS_FMT_START_ITEM, m_vCODEPAGE[m_nTO]);

	for( i=0; i<INT(m_vTITEM_SQL.size()); i++)
	{
		LogString( &vTSQL, m_vTITEM_SQL[i], m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}

	LogString( &vTSQL, _T(""), m_vCODEPAGE[m_nTO]);
	LogString( &vTSQL, IDS_FMT_START_NPC, m_vCODEPAGE[m_nTO]);

	for( i=0; i<INT(m_vTNPC_SQL.size()); i++)
	{
		LogString( &vTSQL, m_vTNPC_SQL[i], m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}

	LogString( &vTSQL, _T(""), m_vCODEPAGE[m_nTO]);
	LogString( &vTSQL, IDS_FMT_START_DHELP, m_vCODEPAGE[m_nTO]);

	for( itTDWORD = m_mapTDHELP.begin(); itTDWORD != m_mapTDHELP.end(); itTDWORD++)
	{
		CString strTEXT = (*itTDWORD).second->m_vTDIC[0]->m_strTO;
		CString strTSQL;

		strTEXT.Replace( _T("'"), _T("''"));
		dwConvert++;
		strTSQL.Format( IDS_FMT_THELPMESSAGETABLE_SQL, strTEXT, (*itTDWORD).first);

		LogString( &vTSQL, strTSQL, m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}

	LogString( &vTSQL, _T(""), m_vCODEPAGE[m_nTO]);
	LogString( &vTSQL, IDS_FMT_START_SVRMSG, m_vCODEPAGE[m_nTO]);

	for( it = m_mapSVRMSG.begin(); it != m_mapSVRMSG.end(); it++)
	{
		CString strNAME = (*it).second; // Translate( &m_mapTRANSLATION, pLOG, (*it).second, &dwConvert);
		CString strTSQL;
		WORD wID = (WORD) atoi((*it).first);

		strNAME.Replace( _T("'"), _T("''"));
		strTSQL.Format( IDS_FMT_SVRMSG_SQL, strNAME, wID);
		LogString( &vTSQL, strTSQL, m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}


	LogString( &vTSQL, _T(""), m_vCODEPAGE[m_nTO]);
	LogString( &vTSQL, IDS_FMT_START_TOURNAMENT, m_vCODEPAGE[m_nTO]);

	for( it = m_mapTOURNAMENT.begin(); it != m_mapTOURNAMENT.end(); it++)
	{
		CString strNAME = (*it).second; // Translate( &m_mapTRANSLATION, pLOG, (*it).second, &dwConvert);
		CString strTSQL;
		BYTE bID = (BYTE) atoi((*it).first);

		strNAME.Replace( _T("'"), _T("''"));
		strTSQL.Format( IDS_FMT_TOURNAMENT_SQL, strNAME, bID);
		LogString( &vTSQL, strTSQL, m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}

	LogString( &vTSQL, _T(""), m_vCODEPAGE[m_nTO]);
	LogString( &vTSQL, IDS_FMT_START_TOURNAMENTSCHEDULE, m_vCODEPAGE[m_nTO]);

	for( it = m_mapTOURNAMENTSCHEDULE.begin(); it != m_mapTOURNAMENTSCHEDULE.end(); it++)
	{
		CString strNAME = (*it).second; // Translate( &m_mapTRANSLATION, pLOG, (*it).second, &dwConvert);
		CString strTSQL;
		BYTE bID = (BYTE) atoi((*it).first);

		strNAME.Replace( _T("'"), _T("''"));
		strTSQL.Format( IDS_FMT_TOURNAMENTSCHEDULE_SQL, strNAME, bID);
		LogString( &vTSQL, strTSQL, m_vCODEPAGE[m_nTO]);
		dwCount++;
		UpdateProgressBar( lParam, m_nTSTEP, dwTotal, dwCount);
	}

	vTSQL.Close();
	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_SQL_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildTIF( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_TIF, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_TIF, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	DWORD dwConvert = 0;
	int nCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFROM);
	vFROM.Read( &nCount, sizeof(int));
	vTO.Write( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
		TranslateFRAME( lParam, pLOG, &vFROM, &vTO, 0, &dwConvert);

	vFROM.Read( &nCount, sizeof(int));
	vTO.Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		TCML_LOGFONT vFONT;

		vFROM.Read( &vFONT, sizeof(TCML_LOGFONT));
		vTO.Write( &vFONT, sizeof(TCML_LOGFONT));
	}

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::BuildMPQ( CFile *pLOG, LPTBDPARAM lParam)
{
	CTShell vTSHELL;

	CFile vFROM;
	CFile vTO;

	vTSHELL.m_strPath.Format( IDS_FMT_FILE_MPQ, m_strSOURCE);
	LogString( pLOG, IDS_LOG_FMT_START_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_BUILD, vTSHELL.GetFileName(vTSHELL.m_strPath));
	m_nTSTEP++;

	if(!vFROM.Open( vTSHELL.m_strPath, CFile::modeRead|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	vTSHELL.m_strInitDir.Format( IDS_FMT_PATH_CLIENT, m_strTARGET);
	vTSHELL.m_strPath.Format( IDS_FMT_FILE_MPQ, vTSHELL.m_strInitDir);

	if(!vTO.Open( vTSHELL.m_strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		LogString( pLOG, IDS_LOG_ERROR_FILE_OPEN, vTSHELL.m_strPath, CP_ACP);
		LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
		LogString( pLOG, _T(""), CP_ACP);

		return;
	}

	DWORD dwLength = DWORD(vFROM.GetLength());
	DWORD dwConvert = 0;
	int nCount;

	UpdateProgressBar( lParam, m_nTSTEP, &vFROM);
	vFROM.Read( &nCount, sizeof(int));
	vTO.Write( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		CString strNAME;
		CString strTEXT;

		DWORD dwID;
		BYTE bMAIN;

		vFROM.Read( &dwID, sizeof(DWORD));
		strNAME = ReadStringFromIDH(&vFROM);
		vFROM.Read( &bMAIN, sizeof(BYTE));

		LPTDICBUFF pTDICBUFF = FindTDICBUF(
			NULL,
			dwID,
			&m_mapTQCATEGORY);
		LPTDICDATA pTDATA = pTDICBUFF && !pTDICBUFF->m_vTDIC.empty() ? pTDICBUFF->m_vTDIC[0] : NULL;

		strNAME = Translate( pTDATA, NULL, strNAME, &dwConvert);
		vTO.Write( &dwID, sizeof(DWORD));
		WriteTEXT( &vTO, strNAME);
		vTO.Write( &bMAIN, sizeof(BYTE));

		strNAME.Replace( _T("'"), _T("''"));
		strTEXT.Format( IDS_FMT_TQCATEGORY_SQL, strNAME, dwID);

		m_vTQCATEGORY_SQL.push_back(strTEXT);
		UpdateProgressBar( lParam, m_nTSTEP, &vFROM);
	}

	while(vFROM.GetPosition() < dwLength)
		TranslateTQUEST( lParam, pLOG, &vFROM, &vTO, &dwConvert);

	vFROM.Close();
	vTO.Close();

	LogString( pLOG, IDS_LOG_SUCCESS_COUNT, dwConvert, CP_ACP);
	LogString( pLOG, IDS_LOG_FMT_END_CLIENT_FILE, vTSHELL.GetFileName(vTSHELL.m_strPath), CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::LoadDB( CFile *pLOG, LPTBDPARAM lParam)
{
	TEXEC(LogString( pLOG, IDS_LOG_BEGIN_LOAD_DB, CP_ACP));
	ReleaseDB();

	CString strDATA;
	strDATA.Format( IDS_FMT_LIST_FILE, m_strTEXT);

	CFile vLIST( strDATA, CFile::modeRead|CFile::typeBinary);
	while( ReadWString( &vLIST, strDATA, CP_ACP) && !GetCANCEL() )
		LoadDB( lParam, pLOG, strDATA);
	TEXEC(LogString( pLOG, IDS_LOG_END_LOAD_DB, CP_ACP));
}

void CTTranslatorDlg::ReleaseDB()
{
	MAPTDWORDDICBUFF::iterator itTDWORD;
	MAPTSTRINGDICBUFF::iterator itTSTR;

	for( itTSTR = m_mapTEVENTQUATER.begin(); itTSTR != m_mapTEVENTQUATER.end(); itTSTR++)
		delete (*itTSTR).second;

	for( itTSTR = m_mapTINTERFACE.begin(); itTSTR != m_mapTINTERFACE.end(); itTSTR++)
		delete (*itTSTR).second;

	for( itTSTR = m_mapTHELP.begin(); itTSTR != m_mapTHELP.end(); itTSTR++)
		delete (*itTSTR).second;

	for( itTSTR = m_mapTQOBJECTIVE.begin(); itTSTR != m_mapTQOBJECTIVE.end(); itTSTR++)
		delete (*itTSTR).second;

	for( itTDWORD = m_mapTQCATEGORY.begin(); itTDWORD != m_mapTQCATEGORY.end(); itTDWORD++)
		delete (*itTDWORD).second;

	for( itTDWORD = m_mapTQTITLE.begin(); itTDWORD != m_mapTQTITLE.end(); itTDWORD++)
		delete (*itTDWORD).second;

	for( itTDWORD = m_mapTCOUNTRY.begin(); itTDWORD != m_mapTCOUNTRY.end(); itTDWORD++)
		delete (*itTDWORD).second;

	for( itTDWORD = m_mapTCLIENT.begin(); itTDWORD != m_mapTCLIENT.end(); itTDWORD++)
		delete (*itTDWORD).second;

	for( itTDWORD = m_mapTCLASS.begin(); itTDWORD != m_mapTCLASS.end(); itTDWORD++)
		delete (*itTDWORD).second;

	for( itTDWORD = m_mapTDHELP.begin(); itTDWORD != m_mapTDHELP.end(); itTDWORD++)
		delete (*itTDWORD).second;

	for( itTDWORD = m_mapTRACE.begin(); itTDWORD != m_mapTRACE.end(); itTDWORD++)
		delete (*itTDWORD).second;

	for( itTDWORD = m_mapTINFO.begin(); itTDWORD != m_mapTINFO.end(); itTDWORD++)
		delete (*itTDWORD).second;

	m_mapTRANSLATION.clear();
	m_mapTINFOTEXT.clear();
	m_mapTMONSTER.clear();
	m_mapTBATTLE.clear();

	m_mapTEVENTQUATER.clear();
	m_mapTINTERFACE.clear();
	m_mapTHELP.clear();

	m_mapTQOBJECTIVE.clear();
	m_mapTQCATEGORY.clear();
	m_mapTQTITLE.clear();
	m_mapNPCNAME.clear();

	m_mapTCOUNTRY.clear();
	m_mapTCLIENT.clear();
	m_mapTCLASS.clear();
	m_mapTDHELP.clear();
	m_mapTRACE.clear();
	m_mapTINFO.clear();

	m_vTQCATEGORY_SQL.clear();
	m_vTQTITLE_SQL.clear();
	m_vTQTERM_SQL.clear();

	m_vTMONSTER_SQL.clear();
	m_vTSKILL_SQL.clear();
	m_vTITEM_SQL.clear();
	m_vTNPC_SQL.clear();
}

void CTTranslatorDlg::LoadDB( LPTBDPARAM lParam,
							  CFile *pLOG,
							  CString& strFILE)
{
	CString strDATA;
	strDATA.Format( IDS_FMT_PATH, m_strTEXT, strFILE);

	CFile vFILE( strDATA, CFile::modeRead|CFile::typeBinary);
	LogString( pLOG, IDS_LOG_FMT_START_TXT, strFILE, CP_ACP);
	SetActionText( lParam, IDS_FMT_ACTION_LOAD, strFILE);
	m_nTSTEP++;
	UpdateProgressBar( lParam, m_nTSTEP, &vFILE);

	if(ReadWString( &vFILE, strDATA, CP_ACP))
	{
		BYTE bKIND = (BYTE) atoi(LPCSTR(strDATA));

		LogString( pLOG, IDS_LOG_TEXT_KIND, m_vTDOCKIND[bKIND], CP_ACP);
		switch(bKIND)
		{
		case TDOC_TRANSLATTION		: LoadDIC( lParam, pLOG, &vFILE, &m_mapTRANSLATION); break;
		case TDOC_ACCESSID			:
			{
				SetActionText( lParam, IDS_FMT_ACTION_LOAD_SUB, strFILE, m_vTDBNAME[TACCESSID_ITEMKIND]);
				LogString( pLOG, IDS_FMT_ACCESSID_DB_NAME, m_vTDBNAME[TACCESSID_ITEMKIND], CP_ACP);
				LoadDIC( lParam, pLOG, &vFILE, &m_mapTINFOTEXT);

				SetActionText( lParam, IDS_FMT_ACTION_LOAD_SUB, strFILE, m_vTDBNAME[TACCESSID_SKILLTAB]);
				LogString( pLOG, IDS_FMT_ACCESSID_DB_NAME, m_vTDBNAME[TACCESSID_SKILLTAB], CP_ACP);
				LoadDIC( lParam, pLOG, &vFILE, &m_mapTINFOTEXT);

				SetActionText( lParam, IDS_FMT_ACTION_LOAD_SUB, strFILE, m_vTDBNAME[TACCESSID_INFOTEXT]);
				LogString( pLOG, IDS_FMT_ACCESSID_DB_NAME, m_vTDBNAME[TACCESSID_INFOTEXT], CP_ACP);
				LoadDIC( lParam, pLOG, &vFILE, &m_mapTINFOTEXT);
			}

			break;

		case TDOC_EVENTQUARTER		: LoadTEVENTQUATER( lParam, pLOG, &vFILE, &m_mapTEVENTQUATER); break;
		case TDOC_INTERFACE			: LoadTINTERFACE( lParam, pLOG, &vFILE, &m_mapTINTERFACE); break;
		case TDOC_TCLIENT			: LoadTCLIENT( lParam, pLOG, &vFILE, &m_mapTCLIENT); break;
		case TDOC_HELP				: LoadTPAIRSTRDICBUFF( lParam, pLOG, &vFILE, &m_mapTHELP); break;

		case TDOC_QUEST_OBJECTIVE	:
		case TDOC_QUEST_MATERIAL	: LoadTPAIRSTRDICBUFF( lParam, pLOG, &vFILE, &m_mapTQOBJECTIVE); break;
		case TDOC_QUEST_CATEGORY	: LoadTDWORDDICBUFF( lParam, pLOG, &vFILE, &m_mapTQCATEGORY); break;
		case TDOC_QUEST_CHAPTER		:
		case TDOC_QUEST_OUTPUT		:
		case TDOC_QUEST_CRAFT		: LoadTDWORDDICBUFF( lParam, pLOG, &vFILE, &m_mapTQTITLE); break;

		case TDOC_COUNTRYINFO		: LoadTDWORDDICBUFF( lParam, pLOG, &vFILE, &m_mapTCOUNTRY); break;
		case TDOC_CLASSINFO			: LoadTDWORDDICBUFF( lParam, pLOG, &vFILE, &m_mapTCLASS); break;
		case TDOC_DYNAMICHELP		: LoadTDWORDDICBUFF( lParam, pLOG, &vFILE, &m_mapTDHELP); break;
		case TDOC_RACEINFO			: LoadTDWORDDICBUFF( lParam, pLOG, &vFILE, &m_mapTRACE); break;
		case TDOC_INFOCHART			: LoadTDWORDDICBUFF( lParam, pLOG, &vFILE, &m_mapTINFO); break;

		case TDOC_BATTLE_ZONE		: LoadTBATTLEZONE( lParam, pLOG, &vFILE, &m_mapTBATTLE); break;
		case TDOC_MONSTER			: LoadTMONSTER( lParam, pLOG, &vFILE, &m_mapTMONSTER); break;
		case TDOC_SVRMSG				: LoadSVRMSG( lParam, pLOG, &vFILE, &m_mapSVRMSG); break;
		case TDOC_TOURNAMENT			: LoadTOURNAMENT( lParam, pLOG, &vFILE, &m_mapTOURNAMENT); break;
		case TDOC_TOURNAMENT_SCHEDULE	: LoadTOURNAMENTSCHEDULE( lParam, pLOG, &vFILE, &m_mapTOURNAMENTSCHEDULE); break;
		}
	}

	LogString( pLOG, IDS_LOG_FMT_END_TXT, strFILE, CP_ACP);
	LogString( pLOG, _T(""), CP_ACP);
}

void CTTranslatorDlg::LoadDIC( LPTBDPARAM lParam,
							   CFile *pLOG,
							   CFile *pFILE,
							   LPMAPTRANSLATION pTDIC)
{
	int nCount = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));
	int nField = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));

	LogString( pLOG, IDS_LOG_ROW_COUNT, nCount, CP_ACP);
	LogString( pLOG, IDS_LOG_FIELD_COUNT, nField, CP_ACP);

	if(nField)
	{
		AfxMessageBox( IDS_ERROR_INVALID_SRC_DATA, MB_OK, NULL);
		return;
	}

	for( int i=0; i<nCount; i++)
	{
		CString strFROM = ReadTEXT( pFILE, m_vCODEPAGE[m_nFROM]);
		CString strTO = ReadTEXT( pFILE, m_vCODEPAGE[m_nTO]);

		MAPTRANSLATION::iterator finder = pTDIC->find(strFROM);
		if( finder == pTDIC->end() )
			pTDIC->insert( MAPTRANSLATION::value_type( strFROM, strTO));
		else if( (*finder).second != strTO )
			LogString( pLOG, IDS_LOG_ERR_DUP_WORD, strFROM, m_vCODEPAGE[m_nFROM]);

		UpdateProgressBar( lParam, m_nTSTEP, pFILE);
	}
}

void CTTranslatorDlg::LoadTBATTLEZONE( LPTBDPARAM lParam,
									   CFile *pLOG,
									   CFile *pFILE,
									   LPMAPTRANSLATION pTDIC)
{
	int nCount = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));
	int nField = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));

	LogString( pLOG, IDS_LOG_ROW_COUNT, nCount, CP_ACP);
	LogString( pLOG, IDS_LOG_FIELD_COUNT, nField, CP_ACP);

	if(nField)
	{
		AfxMessageBox( IDS_ERROR_INVALID_SRC_DATA, MB_OK, NULL);
		return;
	}

	for( int i=0; i<nCount; i++)
	{
		CString strID = ReadTEXT( pFILE, CP_ACP);
		CString strNAME = ReadTEXT( pFILE, m_vCODEPAGE[m_nFROM]);

		MAPTRANSLATION::iterator finder = pTDIC->find(strID);
		if( finder == pTDIC->end() )
			pTDIC->insert( MAPTRANSLATION::value_type( strID, strNAME));
		else if( (*finder).second != strNAME )
			LogString( pLOG, IDS_LOG_ERR_DUP_STRING_ID, strID, CP_ACP);

		UpdateProgressBar( lParam, m_nTSTEP, pFILE);
	}
}

void CTTranslatorDlg::LoadTDWORDDICBUFF( LPTBDPARAM lParam,
										 CFile *pLOG,
										 CFile *pFILE,
										 LPMAPTDWORDDICBUFF pTDWORDDIC)
{
	int nCount = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));
	int nField = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));

	LogString( pLOG, IDS_LOG_ROW_COUNT, nCount, CP_ACP);
	LogString( pLOG, IDS_LOG_FIELD_COUNT, nField, CP_ACP);

	for( int i=0; i<nCount; i++)
	{
		DWORD dwID = (DWORD) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));
		LPTDICBUFF pTDICBUFF = NULL;

		MAPTDWORDDICBUFF::iterator finder = pTDWORDDIC->find(dwID);
		if( finder == pTDWORDDIC->end() )
		{
			pTDICBUFF = new TDICBUFF();
			pTDWORDDIC->insert( MAPTDWORDDICBUFF::value_type( dwID, pTDICBUFF));
		}
		else
			LogString( pLOG, IDS_LOG_ERR_DUP_DWORD_ID, dwID, CP_ACP);

		for( int j=0; j<nField; j++)
		{
			CString strFROM = ReadTEXT( pFILE, m_vCODEPAGE[m_nFROM]);
			CString strTO = ReadTEXT( pFILE, m_vCODEPAGE[m_nTO]);

			if(pTDICBUFF)
			{
				LPTDICDATA pDATA = new TDICDATA();

				pDATA->m_strFROM = strFROM;
				pDATA->m_strTO = strTO;

				pTDICBUFF->m_vTDIC.push_back(pDATA);
			}
		}

		UpdateProgressBar( lParam, m_nTSTEP, pFILE);
	}
}

void CTTranslatorDlg::LoadTEVENTQUATER( LPTBDPARAM lParam,
									    CFile *pLOG,
									    CFile *pFILE,
										LPMAPTSTRINGDICBUFF pTSTRINGDIC)
{
	int nCount = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));
	int nField = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));

	LogString( pLOG, IDS_LOG_ROW_COUNT, nCount, CP_ACP);
	LogString( pLOG, IDS_LOG_FIELD_COUNT, nField, CP_ACP);
	nField--;

	for( int i=0; i<nCount; i++)
	{
		CString strDAY = ReadTEXT( pFILE, CP_ACP);
		CString strH = ReadTEXT( pFILE, CP_ACP);
		CString strM = ReadTEXT( pFILE, CP_ACP);

		LPTDICBUFF pTDICBUFF = NULL;
		CString strID;

		strID.Format(
			IDS_FMT_TRIPLE_ID,
			LPCSTR(strDAY),
			LPCSTR(strH),
			LPCSTR(strM));

		MAPTSTRINGDICBUFF::iterator finder = pTSTRINGDIC->find(strID);
		if( finder == pTSTRINGDIC->end() )
		{
			pTDICBUFF = new TDICBUFF();
			pTSTRINGDIC->insert( MAPTSTRINGDICBUFF::value_type( strID, pTDICBUFF));
		}
		else
			LogString( pLOG, IDS_LOG_ERR_DUP_STRING_ID, strID, CP_ACP);

		for( int j=0; j<nField; j++)
		{
			CString strFROM = ReadTEXT( pFILE, m_vCODEPAGE[m_nFROM]);
			CString strTO = ReadTEXT( pFILE, m_vCODEPAGE[m_nTO]);

			if(pTDICBUFF)
			{
				LPTDICDATA pDATA = new TDICDATA();

				pDATA->m_strFROM = strFROM;
				pDATA->m_strTO = strTO;

				pTDICBUFF->m_vTDIC.push_back(pDATA);
			}
		}

		UpdateProgressBar( lParam, m_nTSTEP, pFILE);
	}
}

void CTTranslatorDlg::LoadTMONSTER( LPTBDPARAM lParam,
								    CFile *pLOG,
								    CFile *pFILE,
									LPMAPTRANSLATION pTDIC)
{
	int nCount = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));
	int nField = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));

	LogString( pLOG, IDS_LOG_ROW_COUNT, nCount, CP_ACP);
	LogString( pLOG, IDS_LOG_FIELD_COUNT, nField, CP_ACP);
	nField--;

	if(nField)
	{
		AfxMessageBox( IDS_ERROR_INVALID_SRC_DATA, MB_OK, NULL);
		return;
	}

	for( int i=0; i<nCount; i++)
	{
		CString strTYPE = ReadTEXT( pFILE, CP_ACP);
		CString strNAME = ReadTEXT( pFILE, m_vCODEPAGE[m_nFROM]);
		CString strTO = ReadTEXT( pFILE, m_vCODEPAGE[m_nTO]);
		CString strFROM;

		strFROM.Format(
			IDS_FMT_MONSTER_ID,
			GetTID( pLOG, strTYPE),
			LPCSTR(strNAME));

		MAPTRANSLATION::iterator finder = pTDIC->find(strFROM);
		if( finder == pTDIC->end() )
			pTDIC->insert( MAPTRANSLATION::value_type( strFROM, strTO));
		else if( (*finder).second != strTO )
			LogString( pLOG, IDS_LOG_ERR_DUP_STRING_ID, strFROM, m_vCODEPAGE[m_nFROM]);

		UpdateProgressBar( lParam, m_nTSTEP, pFILE);
	}
}

void CTTranslatorDlg::LoadSVRMSG( LPTBDPARAM lParam,
								 CFile *pLOG,
								 CFile *pFILE,
								 LPMAPTRANSLATION pTDIC)
{
	int nCount = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));
	int nField = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));

	LogString( pLOG, IDS_LOG_ROW_COUNT, nCount, CP_ACP);
	LogString( pLOG, IDS_LOG_FIELD_COUNT, nField, CP_ACP);

	for( int i=0; i<nCount; i++)
	{
		CString strID = ReadTEXT( pFILE, CP_ACP);
		CString strTemp = ReadTEXT( pFILE, m_vCODEPAGE[m_nFROM]);
		CString strNAME = ReadTEXT( pFILE, m_vCODEPAGE[m_nTO]);

		MAPTRANSLATION::iterator finder = pTDIC->find(strID);
		if( finder == pTDIC->end() )
			pTDIC->insert( MAPTRANSLATION::value_type( strID, strNAME));
		else if( (*finder).second != strNAME )
			LogString( pLOG, IDS_LOG_ERR_DUP_STRING_ID, strID, CP_ACP);

		UpdateProgressBar( lParam, m_nTSTEP, pFILE);
	}
}

void CTTranslatorDlg::LoadTOURNAMENT( LPTBDPARAM lParam,
									 CFile *pLOG,
									 CFile *pFILE,
									 LPMAPTRANSLATION pTDIC)
{
	int nCount = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));
	int nField = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));

	LogString( pLOG, IDS_LOG_ROW_COUNT, nCount, CP_ACP);
	LogString( pLOG, IDS_LOG_FIELD_COUNT, nField, CP_ACP);

	for( int i=0; i<nCount; i++)
	{
		CString strID = ReadTEXT( pFILE, CP_ACP);
		CString strTemp = ReadTEXT( pFILE, m_vCODEPAGE[m_nFROM]);
		CString strNAME = ReadTEXT( pFILE, m_vCODEPAGE[m_nTO]);

		MAPTRANSLATION::iterator finder = pTDIC->find(strID);
		if( finder == pTDIC->end() )
			pTDIC->insert( MAPTRANSLATION::value_type( strID, strNAME));
		else if( (*finder).second != strNAME )
			LogString( pLOG, IDS_LOG_ERR_DUP_STRING_ID, strID, CP_ACP);

		UpdateProgressBar( lParam, m_nTSTEP, pFILE);
	}
}

void CTTranslatorDlg::LoadTOURNAMENTSCHEDULE( LPTBDPARAM lParam,
											 CFile *pLOG,
											 CFile *pFILE,
											 LPMAPTRANSLATION pTDIC)
{
	int nCount = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));
	int nField = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));

	LogString( pLOG, IDS_LOG_ROW_COUNT, nCount, CP_ACP);
	LogString( pLOG, IDS_LOG_FIELD_COUNT, nField, CP_ACP);

	for( int i=0; i<nCount; i++)
	{
		CString strID = ReadTEXT( pFILE, CP_ACP);
		CString strTemp = ReadTEXT( pFILE, m_vCODEPAGE[m_nFROM]);
		CString strNAME = ReadTEXT( pFILE, m_vCODEPAGE[m_nTO]);

		MAPTRANSLATION::iterator finder = pTDIC->find(strID);
		if( finder == pTDIC->end() )
			pTDIC->insert( MAPTRANSLATION::value_type( strID, strNAME));
		else if( (*finder).second != strNAME )
			LogString( pLOG, IDS_LOG_ERR_DUP_STRING_ID, strID, CP_ACP);

		UpdateProgressBar( lParam, m_nTSTEP, pFILE);
	}
}

void CTTranslatorDlg::LoadTCLIENT( LPTBDPARAM lParam,
								   CFile *pLOG,
								   CFile *pFILE,
								   LPMAPTDWORDDICBUFF pTDWORDDIC)
{
	int nCount = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));
	int nField = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));

	LogString( pLOG, IDS_LOG_ROW_COUNT, nCount, CP_ACP);
	LogString( pLOG, IDS_LOG_FIELD_COUNT, nField, CP_ACP);

	for( int i=0; i<nCount; i++)
	{
		DWORD dwID = GetTSTRING( pLOG, ReadTEXT( pFILE, CP_ACP));
		LPTDICBUFF pTDICBUFF = NULL;

		if( dwID && pTDWORDDIC->find(dwID) == pTDWORDDIC->end() )
		{
			pTDICBUFF = new TDICBUFF();
			pTDWORDDIC->insert( MAPTDWORDDICBUFF::value_type( dwID, pTDICBUFF));
		}
		else if(dwID)
			LogString( pLOG, IDS_LOG_ERR_DUP_DWORD_ID, dwID, CP_ACP);

		for( int j=0; j<nField; j++)
		{
			CString strFROM = ReadTEXT( pFILE, m_vCODEPAGE[m_nFROM]);
			CString strTO = ReadTEXT( pFILE, m_vCODEPAGE[m_nTO]);

			if(pTDICBUFF)
			{
				LPTDICDATA pDATA = new TDICDATA();

				pDATA->m_strFROM = strFROM;
				pDATA->m_strTO = strTO;

				pTDICBUFF->m_vTDIC.push_back(pDATA);
			}
		}

		UpdateProgressBar( lParam, m_nTSTEP, pFILE);
	}
}

void CTTranslatorDlg::LoadTPAIRSTRDICBUFF( LPTBDPARAM lParam,
										   CFile *pLOG,
										   CFile *pFILE,
										   LPMAPTSTRINGDICBUFF pTSTRINGDIC)
{
	int nCount = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));
	int nField = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));

	LogString( pLOG, IDS_LOG_ROW_COUNT, nCount, CP_ACP);
	LogString( pLOG, IDS_LOG_FIELD_COUNT, nField, CP_ACP);
	nField--;

	for( int i=0; i<nCount; i++)
	{
		CString strID = ReadTEXT( pFILE, CP_ACP);
		LPTDICBUFF pTDICBUFF = NULL;

		CString strPrimID = ReadTEXT( pFILE, CP_ACP);
		CString strSubID = ReadTEXT( pFILE, CP_ACP);

		strID.Format(
			IDS_FMT_PAIR_ID,
			LPCSTR(strPrimID),
			LPCSTR(strSubID));

		MAPTSTRINGDICBUFF::iterator finder = pTSTRINGDIC->find(strID);
		if( finder == pTSTRINGDIC->end() )
		{
			pTDICBUFF = new TDICBUFF();
			pTSTRINGDIC->insert( MAPTSTRINGDICBUFF::value_type( strID, pTDICBUFF));
		}
		else
			LogString( pLOG, IDS_LOG_ERR_DUP_STRING_ID, strID, CP_ACP);

		for( int j=0; j<nField; j++)
		{
			CString strFROM = ReadTEXT( pFILE, m_vCODEPAGE[m_nFROM]);
			CString strTO = ReadTEXT( pFILE, m_vCODEPAGE[m_nTO]);

			if(pTDICBUFF)
			{
				LPTDICDATA pDATA = new TDICDATA();

				pDATA->m_strFROM = strFROM;
				pDATA->m_strTO = strTO;

				pTDICBUFF->m_vTDIC.push_back(pDATA);
			}
		}

		UpdateProgressBar( lParam, m_nTSTEP, pFILE);
	}
}

void CTTranslatorDlg::LoadTINTERFACE( LPTBDPARAM lParam,
									  CFile *pLOG,
									  CFile *pFILE,
									  LPMAPTSTRINGDICBUFF pTSTRINGDIC)
{
	int nCount = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));
	int nField = (int) atoi(LPCSTR(ReadTEXT( pFILE, CP_ACP)));

	LogString( pLOG, IDS_LOG_ROW_COUNT, nCount, CP_ACP);
	LogString( pLOG, IDS_LOG_FIELD_COUNT, nField, CP_ACP);
	nField--;

	for( int i=0; i<nCount; i++)
	{
		CString strID = ReadTEXT( pFILE, CP_ACP);
		LPTDICBUFF pTDICBUFF = NULL;

		CString strFRAME = ReadTEXT( pFILE, CP_ACP);
		CString strCTRL = ReadTEXT( pFILE, CP_ACP);

		strID.Format(
			IDS_FMT_INTERFACE_ID,
			GetTID( pLOG, strFRAME),
			GetTID( pLOG, strCTRL));

		MAPTSTRINGDICBUFF::iterator finder = pTSTRINGDIC->find(strID);
		if( finder == pTSTRINGDIC->end() )
		{
			pTDICBUFF = new TDICBUFF();
			pTSTRINGDIC->insert( MAPTSTRINGDICBUFF::value_type( strID, pTDICBUFF));
		}
		else
			LogString( pLOG, IDS_LOG_ERR_DUP_STRING_ID, strID, CP_ACP);

		for( int j=0; j<nField; j++)
		{
			CString strFROM = ReadTEXT( pFILE, m_vCODEPAGE[m_nFROM]);
			CString strTO = ReadTEXT( pFILE, m_vCODEPAGE[m_nTO]);

			if(pTDICBUFF)
			{
				LPTDICDATA pDATA = new TDICDATA();

				pDATA->m_strFROM = strFROM;
				pDATA->m_strTO = strTO;

				pTDICBUFF->m_vTDIC.push_back(pDATA);
			}
		}

		UpdateProgressBar( lParam, m_nTSTEP, pFILE);
	}
}

CString CTTranslatorDlg::ReadTEXT( CFile *pFILE, DWORD dwCP)
{
	CString strRESULT(_T(""));
	CString strREP;

	strREP.Format( IDS_FMT_REPLACE, TDEF_CHAR_CR, TDEF_CHAR_NL);
	if(pFILE)
		ReadWString( pFILE, strRESULT, dwCP);
	strRESULT.Replace( TDEF_NEWLINE, LPCSTR(strREP));

	return strRESULT;
}

void CTTranslatorDlg::WriteTEXT( CFile *pFILE, CString strTEXT)
{
	int nCount = INT(strTEXT.GetLength());

	pFILE->Write( &nCount, sizeof(int));
	if( nCount > 0 )
		pFILE->Write( LPCSTR(strTEXT), nCount);
}

BYTE CTTranslatorDlg::ReadWString( CFile *pFILE,
								   CString& strTEXT,
								   DWORD dwCP)
{
	TCHAR vCH[TMAX_MBCS_SIZE];
	WCHAR wCH;

	strTEXT.Empty();
	if( pFILE->Read( &wCH, sizeof(WCHAR)) < sizeof(WCHAR) )
		return FALSE;

	int nLength = WideCharToMultiByte(
		dwCP, 0,
		&wCH, 1,
		vCH, TMAX_MBCS_SIZE,
		NULL,
		NULL);

	while( nLength != TDEF_CHAR_SIZE || vCH[0] != TDEF_CHAR_NL )
	{
		if( wCH != TDEF_WCHAR_NBS && (nLength != TDEF_CHAR_SIZE || vCH[0] != TDEF_CHAR_CR) )
			for( int i=0; i<nLength; i++)
				strTEXT.AppendChar(vCH[i]);

		if( pFILE->Read( &wCH, sizeof(WCHAR)) < sizeof(WCHAR) )
			return TRUE;

		nLength = WideCharToMultiByte(
			dwCP, 0,
			&wCH, 1,
			vCH, TMAX_MBCS_SIZE,
			NULL,
			NULL);
	}

	return TRUE;
}

void CTTranslatorDlg::WriteWString( CFile *pFILE,
								    CString strTEXT,
									DWORD dwCP)
{
	if(strTEXT.IsEmpty())
		return;
	int nLength = strTEXT.GetLength() + 1;
	WCHAR *pBUF = new WCHAR[nLength];

	nLength = MultiByteToWideChar(
		dwCP, 0,
		LPCSTR(strTEXT), -1,
		pBUF,
		nLength) - 1;

	if( nLength > 0 )
		pFILE->Write( pBUF, nLength * sizeof(WCHAR));

	delete[] pBUF;
}

void CTTranslatorDlg::OpenUnicode( CFile *pFILE)
{
	WCHAR wCH = TDEF_WCHAR_NBS;
	pFILE->Write( &wCH, sizeof(WCHAR));
}

void CTTranslatorDlg::LogString( CFile *pLOG,
								 CString strLOG,
								 DWORD dwCP)
{
	if(pLOG)
	{
		WriteWString( pLOG, strLOG, dwCP);
		WriteWString( pLOG, _T("\r\n"), dwCP);
	}
}

void CTTranslatorDlg::LogString( CFile *pLOG,
								 DWORD dwID,
								 DWORD dwCP)
{
	CString strLOG;

	strLOG.LoadString(dwID);
	LogString( pLOG, strLOG, dwCP);
}

void CTTranslatorDlg::LogString( CFile *pLOG,
								 DWORD dwFMT,
								 CString strDATA,
								 DWORD dwCP)
{
	CString strLOG;

	strLOG.Format( dwFMT, strDATA);
	LogString( pLOG, strLOG, dwCP);
}

void CTTranslatorDlg::LogString( CFile *pLOG,
								 DWORD dwFMT,
								 DWORD dwDATA,
								 DWORD dwCP)
{
	CString strLOG;

	strLOG.Format( dwFMT, dwDATA);
	LogString( pLOG, strLOG, dwCP);
}

CString CTTranslatorDlg::ReadStringFromIDH( CFile* pFILE)
{
	CString strResult;
	int nLength;

	pFILE->Read( &nLength, sizeof(int));
	if(nLength)
	{
		pFILE->Read( strResult.GetBuffer(nLength), nLength);
		strResult.ReleaseBuffer(nLength);

		return strResult.Left(nLength);
	}
	else
		strResult.Empty();

	return strResult;
}

void CTTranslatorDlg::ReleaseTID()
{
	for( int i=0; i<TLANG_CODE_COUNT; i++)
		m_vTLANGUAGE[i].clear();
	m_vCODEPAGE.clear();

	for( int i=0; i<TINFO_TEXT_COUNT; ++i )
		m_vTINFOSTRING[i].clear();

	m_mapMISSION.clear();
	m_mapNPC.clear();

	m_mapTSTRING.clear();
	m_mapTERM.clear();
	m_mapTID.clear();
}

BYTE CTTranslatorDlg::LoadTID()
{
	ReleaseTID();

	CString strIDH;
	CFile vIDH;

	strIDH.LoadString(IDS_FILE_IDH);
	if(!vIDH.Open( LPCSTR(strIDH), CFile::modeRead|CFile::typeBinary))
		return FALSE;
	DWORD dwCount = 0;

	vIDH.Read( &dwCount, sizeof(DWORD));
	for( DWORD i=0; i<dwCount; i++)
	{
		CString strID = ReadStringFromIDH(&vIDH);
		DWORD dwID = 0;
		vIDH.Read( &dwID, sizeof(DWORD));

		if( m_mapTID.find(strID) != m_mapTID.end() )
		{
			ReleaseTID();
			return FALSE;
		}
		else
			m_mapTID.insert( MAPTID::value_type( strID, dwID));
	}

	vIDH.Read( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		DWORD dwMissionID;
		DWORD dwQuestID;

		vIDH.Read( &dwQuestID, sizeof(DWORD));
		vIDH.Read( &dwMissionID, sizeof(DWORD));

		if( m_mapMISSION.find(dwQuestID) != m_mapMISSION.end() )
		{
			ReleaseTID();
			return FALSE;
		}
		else
			m_mapMISSION.insert( MAPDWORD::value_type( dwQuestID, dwMissionID));
	}

	vIDH.Read( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		CString strID;

		DWORD dwQuestID;
		DWORD dwTermID;
		DWORD dwID;
		BYTE bType;

		vIDH.Read( &dwQuestID, sizeof(DWORD));
		vIDH.Read( &bType, sizeof(BYTE));
		vIDH.Read( &dwTermID, sizeof(DWORD));
		vIDH.Read( &dwID, sizeof(DWORD));

		strID.Format(
			IDS_FMT_TERM_ID,
			dwQuestID,
			bType,
			dwTermID);

		if( m_mapTERM.find(strID) != m_mapTERM.end() )
		{
			ReleaseTID();
			return FALSE;
		}
		else
			m_mapTERM.insert( MAPTID::value_type( strID, dwID));
	}

	vIDH.Read( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		DWORD dwQuestID;
		DWORD dwNPCID;

		vIDH.Read( &dwQuestID, sizeof(DWORD));
		vIDH.Read( &dwNPCID, sizeof(DWORD));

		if( m_mapNPC.find(dwQuestID) != m_mapNPC.end() )
		{
			ReleaseTID();
			return FALSE;
		}
		else
			m_mapNPC.insert( MAPDWORD::value_type( dwQuestID, dwNPCID));
	}

	vIDH.Read( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		CString strID = ReadStringFromIDH(&vIDH);
		DWORD dwID = 0;
		vIDH.Read( &dwID, sizeof(DWORD));

		if( m_mapTSTRING.find(strID) != m_mapTSTRING.end() )
		{
			ReleaseTID();
			return FALSE;
		}
		else
			m_mapTSTRING.insert( MAPTID::value_type( strID, dwID));
	}

	vIDH.Read( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		DWORD dwCP;

		for( int j=0; j<TLANG_CODE_COUNT; j++)
			m_vTLANGUAGE[j].push_back(ReadStringFromIDH(&vIDH));

		vIDH.Read( &dwCP, sizeof(DWORD));
		m_vCODEPAGE.push_back(dwCP);
	}

	vIDH.Read( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		for( int j=0; j<TLANG_CODE_COUNT; j++)
			m_vTINFOSTRING[j].push_back(ReadStringFromIDH(&vIDH));
	}

	return TRUE;
}

CString CTTranslatorDlg::GetSimpleFormat( CString strTEXT)
{
	strTEXT.Replace( TDEF_FORMAT_IGNORE, _T(""));
	CString strRESULT(_T(""));

	int nIndex = strTEXT.Find(TDEF_FORMAT_HEADER);
	int nLength = strTEXT.GetLength();

	while( 0 <= nIndex && nIndex < nLength )
	{
		strRESULT.AppendChar(strTEXT.GetAt(nIndex));
		nIndex++;

		while( nIndex < nLength && CString(TDEF_FORMAT_FLAGS).Find(strTEXT.GetAt(nIndex)) >= 0 )
			nIndex++;

		if( nIndex < nLength )
		{
			strRESULT.AppendChar(strTEXT.GetAt(nIndex));
			nIndex++;

			if( nIndex < nLength )
				nIndex = strTEXT.Find( TDEF_FORMAT_HEADER, nIndex);
		}
	}
	strRESULT.Trim();

	return strRESULT.MakeLower();
}

CString CTTranslatorDlg::GetFormat( CString strTEXT)
{
	strTEXT.Replace( TDEF_FORMAT_IGNORE, _T(""));
	CString strRESULT(_T(""));

	int nIndex = strTEXT.Find(TDEF_FORMAT_HEADER);
	int nLength = strTEXT.GetLength();

	while( 0 <= nIndex && nIndex < nLength )
	{
		strRESULT.AppendChar(strTEXT.GetAt(nIndex));
		nIndex++;

		while( nIndex < nLength && CString(TDEF_FORMAT_FLAGS).Find(strTEXT.GetAt(nIndex)) >= 0 )
		{
			strRESULT.AppendChar(strTEXT.GetAt(nIndex));
			nIndex++;
		}

		if( nIndex < nLength )
		{
			strRESULT.AppendChar(strTEXT.GetAt(nIndex));
			strRESULT.AppendChar(TDEF_FORMAT_BLANK);
			nIndex++;

			if( nIndex < nLength )
				nIndex = strTEXT.Find( TDEF_FORMAT_HEADER, nIndex);
		}
	}
	strRESULT.Trim();

	return strRESULT;
}

CString CTTranslatorDlg::Translate( LPMAPTRANSLATION pTDIC,
								    CFile *pLOG,
									CString strTEXT,
									LPDWORD pCOUNTER)
{
	MAPTRANSLATION::iterator finder = pTDIC->find(strTEXT);

	if( finder == pTDIC->end() )
	{
		if(pLOG)
			LogString( pLOG, IDS_LOG_ERROR_CANNOT_FIND_STRING, strTEXT, m_vCODEPAGE[m_nFROM]);

		return CString(TDEF_WORD);
	}
	(*pCOUNTER)++;

	return (*finder).second;
}

CString CTTranslatorDlg::Translate( LPTDICDATA pTDATA,
								    CFile *pLOG,
									CString strTEXT,
									LPDWORD pCOUNTER)
{
	if(!pTDATA)
	{
		if(pLOG)
			LogString( pLOG, IDS_LOG_ERROR_CANNOT_FIND_STRING, strTEXT, m_vCODEPAGE[m_nFROM]);

		return CString(TDEF_WORD);
	}

	if( pTDATA->m_strFROM != strTEXT && pLOG )
		LogString( pLOG, IDS_LOG_ERROR_INVALID_SOURCE_TEXT, strTEXT, m_vCODEPAGE[m_nFROM]);
	(*pCOUNTER)++;

	return pTDATA->m_strTO;
}

LPTDICBUFF CTTranslatorDlg::FindTDICBUF( CFile *pLOG,
										 CString strID,
										 LPMAPTSTRINGDICBUFF pTSTRINGDIC)
{
	MAPTSTRINGDICBUFF::iterator finder = pTSTRINGDIC->find(strID);

	if( finder == pTSTRINGDIC->end() )
	{
		if(pLOG)
			LogString( pLOG, IDS_LOG_ERROR_CANNOT_FIND_STR_ID, strID, CP_ACP);

		return NULL;
	}

	return (*finder).second;
}

LPTDICBUFF CTTranslatorDlg::FindTDICBUF( CFile *pLOG,
										 DWORD dwID,
										 LPMAPTDWORDDICBUFF pTDWORDDIC)
{
	MAPTDWORDDICBUFF::iterator finder = pTDWORDDIC->find(dwID);

	if( finder == pTDWORDDIC->end() )
	{
		if(pLOG)
			LogString( pLOG, IDS_LOG_ERROR_CANNOT_FIND_DWORD_ID, dwID, CP_ACP);

		return NULL;
	}

	return (*finder).second;
}

DWORD CTTranslatorDlg::GetMissionID( CFile *pLOG, DWORD dwID)
{
	MAPDWORD::iterator finder = m_mapMISSION.find(dwID);

	if( finder == m_mapMISSION.end() )
	{
		if(pLOG)
			LogString( pLOG, IDS_LOG_ERROR_CANNOT_FIND_MISSION, dwID, CP_ACP);

		return 0;
	}

	return (*finder).second;
}

DWORD CTTranslatorDlg::GetTermID( CFile *pLOG,
								  DWORD dwQuestID,
								  BYTE bTermType,
								  DWORD dwTermID)
{
	CString strID;

	strID.Format(
		IDS_FMT_TERM_ID,
		dwQuestID,
		bTermType,
		dwTermID);

	MAPTID::iterator finder = m_mapTERM.find(strID);
	if( finder == m_mapTERM.end() )
	{
		LogString( pLOG, IDS_LOG_ERROR_CANNOT_FIND_TERM_ID, strID, CP_ACP);
		return 0;
	}

	return (*finder).second;
}

CString CTTranslatorDlg::GetNPCName( CFile *pLOG,
									 DWORD dwQuestID,
									 CString strTEXT,
									 LPTDICDATA pTDATA,
									 LPDWORD pCOUNTER)
{
	if( strTEXT == _T("\"%%s\"") )
		return strTEXT;
	MAPDWORD::iterator finder = m_mapNPC.find(dwQuestID);

	if( finder != m_mapNPC.end() )
	{
		MAPNAME::iterator itNAME = m_mapNPCNAME.find((*finder).second);

		if( itNAME != m_mapNPCNAME.end() )
		{
			(*pCOUNTER)++;
			return (*itNAME).second;
		}
	}

	if(pTDATA)
	{
		if( pTDATA->m_strFROM != strTEXT )
			LogString( pLOG, IDS_LOG_ERROR_INVALID_SOURCE_TEXT, strTEXT, m_vCODEPAGE[m_nFROM]);
		(*pCOUNTER)++;

		return pTDATA->m_strTO;
	}

	LogString( pLOG, IDS_LOG_ERROR_CANNOT_FIND_STRING, strTEXT, m_vCODEPAGE[m_nFROM]);
	return CString(TDEF_WORD);
}

DWORD CTTranslatorDlg::GetTSTRING( CFile *pLOG, CString strID)
{
	MAPTID::iterator finder = m_mapTSTRING.find(strID);

	if( finder == m_mapTSTRING.end() )
	{
		LogString( pLOG, IDS_LOG_ERROR_CANNOT_FIND_TSTRING, strID, CP_ACP);
		return 0;
	}

	return (*finder).second;
}

DWORD CTTranslatorDlg::GetTID( CFile *pLOG, CString strID)
{
	MAPTID::iterator finder = m_mapTID.find(strID);

	if( finder == m_mapTID.end() )
	{
		LogString( pLOG, IDS_LOG_ERROR_CANNOT_FIND_TID, strID, CP_ACP);
		return 0;
	}

	return (*finder).second;
}

void CTTranslatorDlg::TranslateMON( LPTBDPARAM lParam,
								    CFile *pLOG,
								    CArchive *pFROM,
									CArchive *pTO,
									LPDWORD pCOUNTER)
{
	CString strTITLE;
	CString strNAME;
	CString strTEXT;

	DWORD dwOBJ;
	WORD wMonID;

	FLOAT fLOST;
	FLOAT fLB;
	FLOAT fAB;

	(*pFROM) >> wMonID;
	(*pTO) << wMonID;

	CopyWORD( pFROM, pTO, 1);
	(*pFROM)
		>> strTITLE
		>> strNAME
		>> fLB
		>> fLOST
		>> fAB
		>> dwOBJ;

	if(!m_mapTMONSTER.empty())
	{
		strTEXT.Format( IDS_FMT_PAIR_ID, strTITLE, strNAME);
		strTEXT.Remove('_');
		strTEXT.Trim();
		strTITLE.Format( IDS_FMT_MONSTER_ID, dwOBJ, strTEXT);

		strNAME = Translate( &m_mapTMONSTER, pLOG, strTITLE, pCOUNTER);
		strTITLE.Empty();
	}
	else
	{
		strTITLE = Translate( &m_mapTRANSLATION, pLOG, strTITLE, pCOUNTER);
		strNAME = Translate( &m_mapTRANSLATION, pLOG, strNAME, pCOUNTER);
	}

	(*pTO)
		<< strTITLE
		<< strNAME
		<< fLB
		<< fLOST
		<< fAB
		<< dwOBJ;

	strTITLE.Replace( _T("'"), _T("''"));
	strNAME.Replace( _T("'"), _T("''"));

	strTEXT.Format( IDS_FMT_MONSTER_SQL, strNAME, strTITLE, wMonID);
	m_vTMONSTER_SQL.push_back(strTEXT);

	CopyWORD( pFROM, pTO, ES_COUNT + TMONSKILL_COUNT);
	CopyBYTE( pFROM, pTO, 8);
	CopyDWORD( pFROM, pTO, 1);
	CopyFLOAT( pFROM, pTO, 4);
	CopyWORD( pFROM, pTO, 1);
	CopyDWORD( pFROM, pTO, 1);
	CopyWORD( pFROM, pTO, 1);
	CopyBYTE( pFROM, pTO, 3);

	UpdateProgressBar( lParam, m_nTSTEP, pFROM);
}

void CTTranslatorDlg::TranslateINFO( LPTBDPARAM lParam,
									 CFile *pLOG,
									 CArchive *pFROM,
									 CArchive *pTO,
									 LPDWORD pCOUNTER)
{
	LISTTDICDATA vTDIC;
	CString strTEXT;

	BYTE bCount;
	DWORD dwID;

	(*pFROM) >> dwID;
	(*pTO) << dwID;
	(*pFROM) >> bCount;

	strTEXT.Empty();
	vTDIC.clear();

	for( BYTE i=0; i<bCount; i++)
	{
		CString strLINE;

		if(!strTEXT.IsEmpty())
			strTEXT += CString(TDEF_REPLACE_FROM);
		(*pFROM) >> strLINE;

		if( strLINE != TDEF_SPACE )
			strTEXT += strLINE;
	}

	////////////////////////////////////////////////////
	// 예약어 -> 대체어
	Replace_TempInfo(&strTEXT);

	BuildTINFODIC(
		&vTDIC,
		strTEXT,
		dwID,
		pLOG,
		pCOUNTER);
	strTEXT.Replace( "~", "-" );

	while(!vTDIC.empty())
	{
		LPTDICDATA pTDATA = vTDIC.back();

		pTDATA->m_strFROM.Replace( "~", "-" );
		pTDATA->m_strTO.Replace( "~", "-" );

		strTEXT.Replace( pTDATA->m_strFROM, pTDATA->m_strTO);
		vTDIC.pop_back();

		delete pTDATA;
	}

	while( strTEXT.Find(TDEF_COLON_FROM) >= 0 )
		strTEXT.Replace( TDEF_COLON_FROM, TDEF_COLON_TO);

	////////////////////////////////////////////////////
	// 예약어 <- 대체어
	Replace_OrigInfo(&strTEXT);

	VSTRING vTEXT;
	vTEXT.clear();

	int nLENGTH = strTEXT.GetLength();
	int nFIND = 0;

	while(nFIND < nLENGTH)
	{
		int nNEXT = strTEXT.Find( TDEF_REPLACE_FROM, nFIND);

		if( nNEXT < 0 )
			nNEXT = nLENGTH;

		if( nNEXT > nFIND )
			vTEXT.push_back(strTEXT.Mid( nFIND, nNEXT - nFIND));
		else
			vTEXT.push_back(CString(TDEF_SPACE));

		nFIND = nNEXT + 2;
	}

	bCount = BYTE(vTEXT.size());
	(*pTO) << bCount;

	for( i=0; i<bCount; i++)
		(*pTO) << vTEXT[i];
	vTEXT.clear();

	UpdateProgressBar( lParam, m_nTSTEP, pFROM);
}

void CTTranslatorDlg::Replace_TempInfo( CString* strTEXT )
{
	size_t size = m_vTINFOSTRING[TINFO_TEXT_ORIG].size();

	for( size_t i=0; i < size; ++i )
	{
		strTEXT->Replace(
			m_vTINFOSTRING[ TINFO_TEXT_ORIG ][i],
			m_vTINFOSTRING[ TINFO_TEXT_TRANS ][i] );
	}
}

void CTTranslatorDlg::Replace_OrigInfo( CString* strTEXT )
{
	size_t size = m_vTINFOSTRING[ TINFO_TEXT_ORIG ].size();

	for( size_t i=0; i < size; ++i )
	{
		strTEXT->Replace(
			m_vTINFOSTRING[ TINFO_TEXT_TRANS ][i],
			m_vTINFOSTRING[ TINFO_TEXT_ORIG ][i] );
	}
}

void CTTranslatorDlg::TranslateFRAME( LPTBDPARAM lParam,
									  CFile *pLOG,
									  CFile *pFROM,
									  CFile *pTO,
									  DWORD dwFrameID,
									  LPDWORD pCOUNTER)
{
	COMPINST vTCOMP;
	int nCount;

	pFROM->Read( &vTCOMP.m_dwID, sizeof(DWORD));
	pTO->Write( &vTCOMP.m_dwID, sizeof(DWORD));

	if(!dwFrameID)
		dwFrameID = vTCOMP.m_dwID;

	pFROM->Read( &vTCOMP.m_bType, sizeof(BYTE));
	pTO->Write( &vTCOMP.m_bType, sizeof(BYTE));

	pFROM->Read( vTCOMP.m_vMENU, TCML_MENU_COUNT * sizeof(DWORD));
	pTO->Write( vTCOMP.m_vMENU, TCML_MENU_COUNT * sizeof(DWORD));

	pFROM->Read( vTCOMP.m_dwImageID, 2 * sizeof(DWORD));
	pTO->Write( vTCOMP.m_dwImageID, 2 * sizeof(DWORD));

	pFROM->Read( &vTCOMP.m_dwTooltipID, sizeof(DWORD));
	pTO->Write( &vTCOMP.m_dwTooltipID, sizeof(DWORD));

	pFROM->Read( &vTCOMP.m_dwFontID, sizeof(DWORD));
	pTO->Write( &vTCOMP.m_dwFontID, sizeof(DWORD));

	pFROM->Read( &vTCOMP.m_dwStyle, sizeof(DWORD));
	pTO->Write( &vTCOMP.m_dwStyle, sizeof(DWORD));

	pFROM->Read( &vTCOMP.m_dwCOLOR, sizeof(DWORD));
	pTO->Write( &vTCOMP.m_dwCOLOR, sizeof(DWORD));

	pFROM->Read( &vTCOMP.m_dwSND, sizeof(DWORD));
	pTO->Write( &vTCOMP.m_dwSND, sizeof(DWORD));

	pFROM->Read( &vTCOMP.m_nMargineH, sizeof(int));
	pTO->Write( &vTCOMP.m_nMargineH, sizeof(int));

	pFROM->Read( &vTCOMP.m_nMargineV, sizeof(int));
	pTO->Write( &vTCOMP.m_nMargineV, sizeof(int));

	pFROM->Read( &vTCOMP.m_nPosX, sizeof(int));
	pTO->Write( &vTCOMP.m_nPosX, sizeof(int));

	pFROM->Read( &vTCOMP.m_nPosY, sizeof(int));
	pTO->Write( &vTCOMP.m_nPosY, sizeof(int));

	pFROM->Read( &vTCOMP.m_nWidth, sizeof(int));
	pTO->Write( &vTCOMP.m_nWidth, sizeof(int));

	pFROM->Read( &vTCOMP.m_nHeight, sizeof(int));
	pTO->Write( &vTCOMP.m_nHeight, sizeof(int));

	pFROM->Read( &vTCOMP.m_bDisplay, sizeof(BYTE));
	pTO->Write( &vTCOMP.m_bDisplay, sizeof(BYTE));

	pFROM->Read( &vTCOMP.m_bAlign, sizeof(BYTE));
	pTO->Write( &vTCOMP.m_bAlign, sizeof(BYTE));

	pFROM->Read( &vTCOMP.m_vEX, sizeof(TSATR));
	pTO->Write( &vTCOMP.m_vEX, sizeof(TSATR));

	CString strTEXT = ReadStringFromIDH(pFROM);
	WriteTEXT( pTO, strTEXT);

	strTEXT = ReadStringFromIDH(pFROM);
	if(!strTEXT.IsEmpty())
	{
		CString strID;

		strID.Format(
			IDS_FMT_INTERFACE_ID,
			dwFrameID,
			vTCOMP.m_dwID);

		LPTDICBUFF pTDICBUFF = FindTDICBUF(
			NULL,
			strID,
			&m_mapTINTERFACE);

		LPTDICDATA pTDATA = pTDICBUFF && !pTDICBUFF->m_vTDIC.empty() ? pTDICBUFF->m_vTDIC[0] : NULL;
		strTEXT = Translate( pTDATA, NULL, strTEXT, pCOUNTER);
	}
	WriteTEXT( pTO, strTEXT);

	pFROM->Read( &nCount, sizeof(int));
	pTO->Write( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
		TranslateFRAME( lParam, pLOG, pFROM, pTO, dwFrameID, pCOUNTER);

	UpdateProgressBar( lParam, m_nTSTEP, pFROM);
}

void CTTranslatorDlg::TranslateTQUEST( LPTBDPARAM lParam,
									   CFile *pLOG,
									   CFile *pFROM,
									   CFile *pTO,
									   LPDWORD pCOUNTER)
{
	CString strTEXT[TQUEST_TEXT_COUNT];
	BYTE bTranslate;

	DWORD dwDATA;
	BYTE bDATA;

	DWORD dwID;
	BYTE bType;
	int nCount;

	pFROM->Read( &dwID, sizeof(DWORD));
	pTO->Write( &dwID, sizeof(DWORD));

	pFROM->Read( &bType, sizeof(BYTE));
	pTO->Write( &bType, sizeof(BYTE));

	pFROM->Read( &dwDATA, sizeof(DWORD));
	pTO->Write( &dwDATA, sizeof(DWORD));

	pFROM->Read( &bDATA, sizeof(BYTE));
	pTO->Write( &bDATA, sizeof(BYTE));

	switch(bType)
	{
	case 1	:
	case 7	:
	case 8	:
	case 10	:
	case 12	:
	case 17	:
	case 18	: bTranslate = TRUE; break;
	default	: bTranslate = FALSE; break;
	}

	LPTDICBUFF pTDICBUFF = bTranslate ? FindTDICBUF(
		NULL,
		bType == 7 ? GetMissionID( NULL, dwID) : dwID,
		&m_mapTQTITLE) : NULL;
	nCount = pTDICBUFF ? INT(pTDICBUFF->m_vTDIC.size()) : 0;

	for( int i=0; i<TQUEST_TEXT_COUNT; i++)
	{
		BYTE bSKIP = !bTranslate || (bType == 7 && i != TQUEST_TEXT_TRIGGER && i != TQUEST_TEXT_NPC) ? TRUE : FALSE;
		int nIndex = bType == 7 && i == TQUEST_TEXT_TRIGGER ? TQUEST_TEXT_FINISH : i;

		strTEXT[i] = ReadStringFromIDH(pFROM);
		if( bSKIP || strTEXT[i].IsEmpty() )
			strTEXT[i].Empty();
		else if( i == TQUEST_TEXT_NPC )
			strTEXT[i] = GetNPCName( pLOG, dwID, strTEXT[i], nIndex < nCount ? pTDICBUFF->m_vTDIC[nIndex] : NULL, pCOUNTER);
		else
			strTEXT[i] = Translate( nIndex < nCount ? pTDICBUFF->m_vTDIC[nIndex] : NULL, NULL, strTEXT[i], pCOUNTER);

		WriteTEXT( pTO, strTEXT[i]);
		strTEXT[i].Replace( _T("'"), _T("''"));
	}

	if(bTranslate)
	{
		CString strTSQL;

		strTSQL.Format(
			IDS_FMT_QUEST_SQL,
			strTEXT[TQUEST_TEXT_TITLE],
			strTEXT[TQUEST_TEXT_TRIGGER],
			strTEXT[TQUEST_TEXT_COMPLETE],
			strTEXT[TQUEST_TEXT_ACCEPT],
			strTEXT[TQUEST_TEXT_REJECT],
			strTEXT[TQUEST_TEXT_SUMMARY],
			strTEXT[TQUEST_TEXT_NPC],
			strTEXT[TQUEST_TEXT_REPLY],
			dwID);
		m_vTQTITLE_SQL.push_back(strTSQL);
	}

	pFROM->Read( &bDATA, sizeof(BYTE));
	pTO->Write( &bDATA, sizeof(BYTE));

	pFROM->Read( &nCount, sizeof(int));
	pTO->Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
		TranslateTQTERM( pLOG, pFROM, pTO, bTranslate ? dwID : 0, pCOUNTER);

	pFROM->Read( &nCount, sizeof(int));
	pTO->Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		pFROM->Read( &bDATA, sizeof(BYTE));
		pTO->Write( &bDATA, sizeof(BYTE));

		pFROM->Read( &dwDATA, sizeof(DWORD));
		pTO->Write( &dwDATA, sizeof(DWORD));

		pFROM->Read( &bDATA, sizeof(BYTE));
		pTO->Write( &bDATA, sizeof(BYTE));

		pFROM->Read( &bDATA, sizeof(BYTE));
		pTO->Write( &bDATA, sizeof(BYTE));

		pFROM->Read( &bDATA, sizeof(BYTE));
		pTO->Write( &bDATA, sizeof(BYTE));
	}

	pFROM->Read( &dwDATA, sizeof(DWORD));
	pTO->Write( &dwDATA, sizeof(DWORD));

	pFROM->Read( &dwDATA, sizeof(DWORD));
	pTO->Write( &dwDATA, sizeof(DWORD));

	UpdateProgressBar( lParam, m_nTSTEP, pFROM);
}

void CTTranslatorDlg::TranslateTQTERM( CFile *pLOG,
									   CFile *pFROM,
									   CFile *pTO,
									   DWORD dwQuestID,
									   LPDWORD pCOUNTER)
{
	CString strTEXT[TQTERM_TEXT_COUNT];
	CString strID;
	DWORD dwID;

	BYTE bCount;
	BYTE bType;

	pFROM->Read( &bType, sizeof(BYTE));
	pTO->Write( &bType, sizeof(BYTE));

	pFROM->Read( &dwID, sizeof(DWORD));
	pTO->Write( &dwID, sizeof(DWORD));

	pFROM->Read( &bCount, sizeof(BYTE));
	pTO->Write( &bCount, sizeof(BYTE));

	dwID = dwQuestID ? GetTermID(
		pLOG,
		dwQuestID,
		bType,
		dwID) : 0;

	strID.Format(
		IDS_FMT_INTERFACE_ID,
		dwQuestID,
		dwID);

	LPTDICBUFF pTDICBUFF = dwID ? FindTDICBUF(
		NULL,
		strID,
		&m_mapTQOBJECTIVE) : NULL;
	int nCount = pTDICBUFF ? INT(pTDICBUFF->m_vTDIC.size()) : 0;

	for( int i=0; i<TQTERM_TEXT_COUNT; i++)
	{
		strTEXT[i] = ReadStringFromIDH(pFROM);

		if( dwID && !strTEXT[i].IsEmpty() )
			strTEXT[i] = Translate( i < nCount ? pTDICBUFF->m_vTDIC[i] : NULL, NULL, strTEXT[i], pCOUNTER);
		else
			strTEXT[i].Empty();

		WriteTEXT( pTO, strTEXT[i]);
		strTEXT[i].Replace( _T("'"), _T("''"));
	}

	if(dwID)
	{
		CString strTSQL;

		strTSQL.Format(
			IDS_FMT_TERM_SQL,
			strTEXT[TQTERM_TEXT_TERM],
			strTEXT[TQTERM_TEXT_FAILED],
			dwID);
		m_vTQTERM_SQL.push_back(strTSQL);
	}
}

void CTTranslatorDlg::BuildTINFODIC( LPLISTTDICDATA pTDIC,
									 CString strTEXT,
									 DWORD dwID,
									 CFile *pLOG,
									 LPDWORD pCOUNTER)
{
	CString strLINE = GetTITLEFromInfoString(strTEXT);

	if(!strLINE.IsEmpty())
	{
		LPTDICDATA pTDATA = new TDICDATA();

		pTDATA->m_strTO = Translate( &m_mapTRANSLATION, NULL, strLINE, pCOUNTER);
		pTDATA->m_strFROM = strLINE;

		pTDIC->push_back(pTDATA);
	}
	else
		LogString( pLOG, IDS_LOG_ERROR_INFO_TITLE_IS_EMPTY, dwID, CP_ACP);

	strLINE = GetTPREVSKILLFromInfoString(strTEXT);
	if(!strLINE.IsEmpty())
	{
		LPTDICDATA pTDATA = new TDICDATA();

		pTDATA->m_strTO = Translate( &m_mapTRANSLATION, pLOG, strLINE, pCOUNTER);
		pTDATA->m_strFROM = strLINE;

		pTDIC->push_back(pTDATA);
	}

	LPTDICBUFF pTDICBUFF = FindTDICBUF(
		pLOG,
		dwID,
		&m_mapTINFO);
	int nCount = pTDICBUFF ? INT(pTDICBUFF->m_vTDIC.size()) : 0;

	for( int i=0; i<nCount; i++)
		if(!pTDICBUFF->m_vTDIC[i]->m_strFROM.IsEmpty())
		{
			LPTDICDATA pTDATA = new TDICDATA();

			pTDATA->m_strFROM = pTDICBUFF->m_vTDIC[i]->m_strFROM;
			pTDATA->m_strTO = pTDICBUFF->m_vTDIC[i]->m_strTO;

			pTDIC->push_back(pTDATA);
		}
	MAPTRANSLATION::iterator it;

	for( it = m_mapTINFOTEXT.begin(); it != m_mapTINFOTEXT.end(); it++)
	{
		LPTDICDATA pTDATA = new TDICDATA();

		pTDATA->m_strFROM = (*it).first;
		pTDATA->m_strTO = (*it).second;

		pTDIC->push_back(pTDATA);
	}

	pTDIC->sort(binary_dic_data());
}

CString CTTranslatorDlg::GetTPREVSKILLFromInfoString( CString strTEXT)
{
	Replace_OrigInfo(&strTEXT);

	int nIndex = strTEXT.Find(TDEF_TAG_PREV_SKILL);
	CString strRESULT;

	strRESULT.Empty();
	if( nIndex < 0 )
		return strRESULT;

	strRESULT = strTEXT.Left(nIndex);
	nIndex = strRESULT.ReverseFind(TDEF_TAG_COLON);

	strRESULT = strRESULT.Mid(nIndex + 1);
	strRESULT.Trim();

	return strRESULT;
}

CString CTTranslatorDlg::GetTITLEFromInfoString( CString strTEXT)
{
	Replace_OrigInfo( &strTEXT );

	int nIndex = strTEXT.Find(TDEF_REPLACE_FROM);
	CString strRESULT = nIndex < 0 ? strTEXT : strTEXT.Left(nIndex);

	strRESULT.Replace( TDEF_TAG_ITEM_NAME, _T(""));
	strRESULT.Trim();

	return strRESULT;
}

void CTTranslatorDlg::CopyFLOAT( CArchive *pFROM,
								 CArchive *pTO,
								 BYTE bCount)
{
	FLOAT fDATA;

	for( BYTE i=0; i<bCount; i++)
	{
		(*pFROM) >> fDATA;
		(*pTO) << fDATA;
	}
}

void CTTranslatorDlg::CopyDWORD( CArchive *pFROM,
								 CArchive *pTO,
								 BYTE bCount)
{
	DWORD dwDATA;

	for( BYTE i=0; i<bCount; i++)
	{
		(*pFROM) >> dwDATA;
		(*pTO) << dwDATA;
	}
}

void CTTranslatorDlg::CopyWORD( CArchive *pFROM,
							    CArchive *pTO,
								BYTE bCount)
{
	WORD wDATA;

	for( BYTE i=0; i<bCount; i++)
	{
		(*pFROM) >> wDATA;
		(*pTO) << wDATA;
	}
}

void CTTranslatorDlg::CopyBYTE( CArchive *pFROM,
							    CArchive *pTO,
								BYTE bCount)
{
	BYTE bDATA;

	for( BYTE i=0; i<bCount; i++)
	{
		(*pFROM) >> bDATA;
		(*pTO) << bDATA;
	}
}

void CTTranslatorDlg::CopyINT( CArchive *pFROM,
							   CArchive *pTO,
							   BYTE bCount)
{
	INT nDATA;

	for( BYTE i=0; i<bCount; i++)
	{
		(*pFROM) >> nDATA;
		(*pTO) << nDATA;
	}
}

void CTTranslatorDlg::UpdateProgressBar( LPTBDPARAM lParam,
										 int nTSTEP,
										 DWORD dwTotal,
										 DWORD dwCurrent)
{
	CTProgressDlg *pPROGRESS = lParam ? (CTProgressDlg *) lParam->m_pPROGRESS : NULL;

	if( !pPROGRESS || !dwTotal || nTSTEP < 1 || m_nTOTAL < 1 )
		return;

	dwCurrent = (DWORD) (FLOAT(TDEF_MAX_PROGRESS) * (FLOAT(dwCurrent) / FLOAT(dwTotal)));
	nTSTEP--;
	FLOAT fRANGE = FLOAT(TDEF_MAX_PROGRESS) / FLOAT(m_nTOTAL);

	if( pPROGRESS->m_cCURRENT.GetPos() != dwCurrent )
	{
		pPROGRESS->m_strCURRENT.Format( IDS_FMT_PROGRESS, dwCurrent * 100 / TDEF_MAX_PROGRESS);
		pPROGRESS->m_cCURRENT.SetPos(dwCurrent);
	}

	dwCurrent = (DWORD) (FLOAT(nTSTEP) * fRANGE + FLOAT(dwCurrent) * fRANGE / FLOAT(TDEF_MAX_PROGRESS));
	if( pPROGRESS->m_cTOTAL.GetPos() != dwCurrent )
	{
		pPROGRESS->m_strTOTAL.Format( IDS_FMT_PROGRESS, dwCurrent * 100 / TDEF_MAX_PROGRESS);
		pPROGRESS->m_cTOTAL.SetPos(dwCurrent);
	}

	pPROGRESS->SendMessage( WM_COMMAND, ID_MENU_UPDATE);
}

void CTTranslatorDlg::UpdateProgressBar( LPTBDPARAM lParam,
										 int nTSTEP,
										 CFile *pFILE)
{
	if(!pFILE)
		return;

	UpdateProgressBar(
		lParam,
		nTSTEP,
		(DWORD) pFILE->GetLength(),
		(DWORD) pFILE->GetPosition());
}

void CTTranslatorDlg::UpdateProgressBar( LPTBDPARAM lParam,
										 int nTSTEP,
										 CArchive *pFILE)
{
	UpdateProgressBar(
		lParam,
		nTSTEP,
		pFILE ? pFILE->GetFile() : NULL);
}

void CTTranslatorDlg::SetActionText( LPTBDPARAM lParam,
									 CString strACTION)
{
	SMART_LOCKCS(&m_cs);

	if( !lParam || !lParam->m_pPROGRESS || m_bCANCEL )
		return;

	((CTProgressDlg *) lParam->m_pPROGRESS)->m_strACTION = strACTION;
	lParam->m_pPROGRESS->SendMessage( WM_COMMAND, ID_MENU_UPDATE);
}

void CTTranslatorDlg::SetActionText( LPTBDPARAM lParam,
									 DWORD dwID)
{
	CString strACTION;

	strACTION.LoadString(dwID);
	SetActionText( lParam, strACTION);
}

void CTTranslatorDlg::SetActionText( LPTBDPARAM lParam,
									 DWORD dwID,
									 CString strTEXT)
{
	CString strACTION;

	strACTION.Format( dwID, strTEXT);
	SetActionText( lParam, strACTION);
}

void CTTranslatorDlg::SetActionText( LPTBDPARAM lParam,
									 DWORD dwID,
									 CString strTEXT,
									 CString strSUB)
{
	CString strACTION;

	strACTION.Format( dwID, strTEXT, strSUB);
	SetActionText( lParam, strACTION);
}

int CTTranslatorDlg::GetTotalStep()
{
	int nCount = TDEF_BASIC_TSTEP;

	CFileFind finder;
	CString strDATA;

	strDATA.Format( IDS_FMT_LIST_FILE, m_strTEXT);
	CFile vLIST( strDATA, CFile::modeRead|CFile::typeBinary);

	while(ReadWString( &vLIST, strDATA, CP_ACP))
		nCount++;

	strDATA.Format( IDS_FMT_FILE_NPC_TCD, m_strSOURCE);
	BYTE bFIND = finder.FindFile(strDATA);

	while(bFIND)
	{
		bFIND = finder.FindNextFile();

		if( finder.GetFileName() != TDEF_FILE_NPCTEMP )
			nCount++;
	}

	return nCount;
}

void CTTranslatorDlg::SetCANCEL( BYTE bCANCEL)
{
	SMART_LOCKCS(&m_cs);
	m_bCANCEL = bCANCEL;
}

BYTE CTTranslatorDlg::GetCANCEL()
{
	SMART_LOCKCS(&m_cs);
	return m_bCANCEL;
}
