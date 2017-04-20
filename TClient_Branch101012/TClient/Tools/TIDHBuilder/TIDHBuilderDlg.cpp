// TIDHBuilderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TIDHBuilder.h"
#include "TIDHBuilderDlg.h"
#include ".\tidhbuilderdlg.h"

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


// CTIDHBuilderDlg dialog



CTIDHBuilderDlg::CTIDHBuilderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTIDHBuilderDlg::IDD, pParent)
	, m_strTSTRING(_T(""))
	, m_strMISSION(_T(""))
	, m_strTERM(_T(""))
	, m_strLANG(_T(""))
	, m_strHEADER(_T(""))
	, m_strIDH(_T(""))
	, m_strNPC(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTIDHBuilderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SOURCE, m_strHEADER);
	DDX_Control(pDX, IDC_BUTTON_SOURCE, m_cHEADER);
	DDX_Text(pDX, IDC_EDIT_LANGUAGE, m_strLANG);
	DDX_Control(pDX, IDC_BUTTON_LANGUAGE, m_cLANG);
	DDX_Text(pDX, IDC_EDIT_MISSION, m_strMISSION);
	DDX_Control(pDX, IDC_BUTTON_MISSION, m_cMISSION);
	DDX_Text(pDX, IDC_EDIT_TERM, m_strTERM);
	DDX_Control(pDX, IDC_BUTTON_TERM, m_cTERM);
	DDX_Text(pDX, IDC_EDIT_NPC, m_strNPC);
	DDX_Control(pDX, IDC_BUTTON_NPC, m_cNPC);
	DDX_Text(pDX, IDC_EDIT_TSTRING, m_strTSTRING);
	DDX_Control(pDX, IDC_BUTTON_TSTRING, m_cTSTRING);
	DDX_Text(pDX, IDC_EDIT_INFOSTRING, m_strINFO );
	DDX_Control(pDX, IDC_BUTTON_INFOSTRING, m_cINFO );
	DDX_Text(pDX, IDC_EDIT_TARGET, m_strIDH);
	DDX_Control(pDX, IDC_BUTTON_TARGET, m_cIDH);
}

BEGIN_MESSAGE_MAP(CTIDHBuilderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SOURCE, OnBnClickedButtonSource)
	ON_BN_CLICKED(IDC_BUTTON_TARGET, OnBnClickedButtonTarget)
	ON_BN_CLICKED(IDC_BUTTON_BUILD, OnBnClickedButtonBuild)
	ON_BN_CLICKED(IDC_BUTTON_MISSION, OnBnClickedButtonMission)
	ON_BN_CLICKED(IDC_BUTTON_TERM, OnBnClickedButtonTerm)
	ON_BN_CLICKED(IDC_BUTTON_TSTRING, OnBnClickedButtonTstring)
	ON_BN_CLICKED(IDC_BUTTON_LANGUAGE, OnBnClickedButtonLanguage)
	ON_BN_CLICKED(IDC_BUTTON_INFOSTRING, OnBnClickedButtonInfostring)
	ON_BN_CLICKED(IDC_BUTTON_NPC, OnBnClickedButtonNpc)
END_MESSAGE_MAP()


// CTIDHBuilderDlg message handlers

BOOL CTIDHBuilderDlg::OnInitDialog()
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
	m_cTSTRING.SetIcon(hIcon);
	m_cMISSION.SetIcon(hIcon);
	m_cTERM.SetIcon(hIcon);
	m_cLANG.SetIcon(hIcon);
	m_cINFO.SetIcon(hIcon);
	m_cNPC.SetIcon(hIcon);

	m_cHEADER.SetIcon(hIcon);
	m_cIDH.SetIcon(hIcon);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTIDHBuilderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTIDHBuilderDlg::OnPaint() 
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
HCURSOR CTIDHBuilderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTIDHBuilderDlg::OnBnClickedButtonSource()
{
	CFileDialog dlg(
		TRUE,
		_T("*.h"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Header Files (*.h)|*.h|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strHEADERFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strHEADERFOLDER = dlg.GetFileName();
		m_strHEADER = dlg.GetPathName();
		m_strHEADERFOLDER = m_strHEADER.Left(m_strHEADER.GetLength() - m_strHEADERFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTIDHBuilderDlg::OnBnClickedButtonMission()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strMISSIONFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strMISSIONFOLDER = dlg.GetFileName();
		m_strMISSION = dlg.GetPathName();
		m_strMISSIONFOLDER = m_strMISSION.Left(m_strMISSION.GetLength() - m_strMISSIONFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTIDHBuilderDlg::OnBnClickedButtonTerm()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strTERMFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strTERMFOLDER = dlg.GetFileName();
		m_strTERM = dlg.GetPathName();
		m_strTERMFOLDER = m_strTERM.Left(m_strTERM.GetLength() - m_strTERMFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTIDHBuilderDlg::OnBnClickedButtonNpc()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strNPCFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strNPCFOLDER = dlg.GetFileName();
		m_strNPC = dlg.GetPathName();
		m_strNPCFOLDER = m_strNPC.Left(m_strNPC.GetLength() - m_strNPCFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTIDHBuilderDlg::OnBnClickedButtonTstring()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strTSTRINGFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strTSTRINGFOLDER = dlg.GetFileName();
		m_strTSTRING = dlg.GetPathName();
		m_strTSTRINGFOLDER = m_strTSTRING.Left(m_strTSTRING.GetLength() - m_strTSTRINGFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTIDHBuilderDlg::OnBnClickedButtonLanguage()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strLANGFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strLANGFOLDER = dlg.GetFileName();
		m_strLANG = dlg.GetPathName();
		m_strLANGFOLDER = m_strLANG.Left(m_strLANG.GetLength() - m_strLANGFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTIDHBuilderDlg::OnBnClickedButtonInfostring()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strINFOFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strINFOFOLDER = dlg.GetFileName();
		m_strINFO = dlg.GetPathName();
		m_strINFOFOLDER = m_strINFO.Left(m_strINFO.GetLength() - m_strINFOFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTIDHBuilderDlg::OnBnClickedButtonTarget()
{
	CFileDialog dlg(
		FALSE,
		_T("*.idh"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("IDH Files (*.idh)|*.idh||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strIDHFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strIDHFOLDER = dlg.GetFileName();
		m_strIDH = dlg.GetPathName();
		m_strIDHFOLDER = m_strIDH.Left(m_strIDH.GetLength() - m_strIDHFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTIDHBuilderDlg::OnBnClickedButtonBuild()
{
	UpdateData();
	MAPID mapID;
	mapID.clear();

	CStdioFile vHEADER( m_strHEADER, CFile::modeRead|CFile::typeText);
	CString strDATA;

	CFile vIDH( m_strIDH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	while(vHEADER.ReadString(strDATA))
		if( strDATA.Find( _T("#define")) >= 0 )
		{
			CHAR szID[MAX_PATH];
			DWORD dwID;

			strDATA.Replace( _T("#define"), _T(""));
			strDATA.Replace( _T("("), _T(""));
			strDATA.Replace( _T(")"), _T(""));
			strDATA.Trim();

			sscanf( LPCSTR(strDATA), _T("%s %x"), szID, &dwID);
			CString strID(szID);
			strID.Trim();

			if(!strID.IsEmpty())
				if( mapID.find(strID) != mapID.end() )
				{
					AfxMessageBox( IDS_ERROR_DUPLICATE_ID, MB_OK);
					mapID.clear();

					return;
				}
				else
					mapID.insert( MAPID::value_type( strID, dwID));
		}

	DWORD dwCount = DWORD(mapID.size());
	MAPID::iterator itID;

	vIDH.Write( &dwCount, sizeof(DWORD));
	for( itID = mapID.begin(); itID != mapID.end(); itID++)
	{
		WriteString( &vIDH, (*itID).first);
		vIDH.Write( &(*itID).second, sizeof(DWORD));
	}
	mapID.clear();

	CStdioFile vMISSION( m_strMISSION, CFile::modeRead|CFile::typeText);
	vMISSION.ReadString(strDATA);
	vMISSION.ReadString(strDATA);

	dwCount = (DWORD) atoi(strDATA);
	vMISSION.ReadString(strDATA);

	if( atoi(strDATA) != 0 )
	{
		AfxMessageBox( IDS_ERROR_INVALID_MISSION, MB_OK, NULL);
		return;
	}

	vIDH.Write( &dwCount, sizeof(DWORD));
	for( DWORD i=0; i<dwCount; i++)
	{
		DWORD dwMissionID;
		DWORD dwQuestID;

		vMISSION.ReadString(strDATA);
		dwQuestID = (DWORD) atoi(strDATA);

		vMISSION.ReadString(strDATA);
		dwMissionID = (DWORD) atoi(strDATA);

		vIDH.Write( &dwQuestID, sizeof(DWORD));
		vIDH.Write( &dwMissionID, sizeof(DWORD));
	}

	CStdioFile vTERM( m_strTERM, CFile::modeRead|CFile::typeText);
	vTERM.ReadString(strDATA);
	vTERM.ReadString(strDATA);

	dwCount = (DWORD) atoi(strDATA);
	vTERM.ReadString(strDATA);

	if( atoi(strDATA) != 2 )
	{
		AfxMessageBox( IDS_ERROR_INVALID_TERM, MB_OK, NULL);
		return;
	}

	vIDH.Write( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		DWORD dwQuestID;
		DWORD dwTermID;
		DWORD dwID;

		BYTE bTermType;

		vTERM.ReadString(strDATA);
		vTERM.ReadString(strDATA);
		dwID = (DWORD) atoi(strDATA);

		vTERM.ReadString(strDATA);
		dwQuestID = (DWORD) atoi(strDATA);

		vTERM.ReadString(strDATA);
		bTermType = (BYTE) atoi(strDATA);

		vTERM.ReadString(strDATA);
		dwTermID = (DWORD) atoi(strDATA);

		vIDH.Write( &dwQuestID, sizeof(DWORD));
		vIDH.Write( &bTermType, sizeof(BYTE));
		vIDH.Write( &dwTermID, sizeof(DWORD));
		vIDH.Write( &dwID, sizeof(DWORD));
	}

	CStdioFile vNPC( m_strNPC, CFile::modeRead|CFile::typeText);
	vNPC.ReadString(strDATA);
	vNPC.ReadString(strDATA);

	dwCount = (DWORD) atoi(strDATA);
	vNPC.ReadString(strDATA);

	if( atoi(strDATA) != 0 )
	{
		AfxMessageBox( IDS_ERROR_INVALID_NPC, MB_OK, NULL);
		return;
	}

	vIDH.Write( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		DWORD dwQuestID;
		DWORD dwNPCID;

		vNPC.ReadString(strDATA);
		dwQuestID = (DWORD) atoi(strDATA);

		vNPC.ReadString(strDATA);
		dwNPCID = (DWORD) atoi(strDATA);

		vIDH.Write( &dwQuestID, sizeof(DWORD));
		vIDH.Write( &dwNPCID, sizeof(DWORD));
	}

	CStdioFile vTSTRING( m_strTSTRING, CFile::modeRead|CFile::typeText);
	vTSTRING.ReadString(strDATA);
	vTSTRING.ReadString(strDATA);

	dwCount = (DWORD) atoi(strDATA);
	vTSTRING.ReadString(strDATA);

	if( atoi(strDATA) != 0 )
	{
		AfxMessageBox( IDS_ERROR_INVALID_TSTRING, MB_OK, NULL);
		return;
	}

	vIDH.Write( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		CString strID;
		DWORD dwID;

		vTSTRING.ReadString(strDATA);
		strID.Format( IDS_FMT_IDS, strDATA.Mid(4));

		vTSTRING.ReadString(strDATA);
		dwID = (DWORD) atoi(strDATA);

		WriteString( &vIDH, strID);
		vIDH.Write( &dwID, sizeof(DWORD));
	}

	CStdioFile vLANG( m_strLANG, CFile::modeRead|CFile::typeText);
	vLANG.ReadString(strDATA);
	vLANG.ReadString(strDATA);

	dwCount = (DWORD) atoi(strDATA);
	vLANG.ReadString(strDATA);

	if( atoi(strDATA) != 1 )
	{
		AfxMessageBox( IDS_ERROR_INVALID_LANG, MB_OK, NULL);
		return;
	}

	vIDH.Write( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		vLANG.ReadString(strDATA);
		WriteString( &vIDH, strDATA);
		vLANG.ReadString(strDATA);
		WriteString( &vIDH, strDATA);

		vLANG.ReadString(strDATA);
		DWORD dwCP = atoi(strDATA);
		vIDH.Write( &dwCP, sizeof(DWORD));
	}

	CStdioFile vINFO( m_strINFO, CFile::modeRead|CFile::typeText);
	vINFO.ReadString(strDATA);
	vINFO.ReadString(strDATA);

	dwCount = (DWORD) atoi(strDATA);
	vINFO.ReadString(strDATA);

	if( atoi(strDATA) != 0 )
	{
		AfxMessageBox( IDS_ERROR_INVALID_INFO, MB_OK, NULL);
		return;
	}

	vIDH.Write( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		vINFO.ReadString(strDATA);
		WriteString( &vIDH, strDATA);
		vINFO.ReadString(strDATA);
		WriteString( &vIDH, strDATA);
	}
}

void CTIDHBuilderDlg::WriteString( CFile* pFile, CString strValue)
{
	int nLength = strValue.GetLength();

	pFile->Write( &nLength, sizeof(int));
	if(nLength)
		pFile->Write( LPCSTR(strValue), nLength);
}
