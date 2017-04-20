// TCMLParserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TCMLParser.h"
#include "TCMLParserDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern COMPDESC* TCMLFrame;

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


// CTCMLParserDlg dialog



CTCMLParserDlg::CTCMLParserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTCMLParserDlg::IDD, pParent)
	, m_strSRC(_T(""))
	, m_strDEST(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCMLParserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DEST, m_cDEST);
	DDX_Control(pDX, IDC_BUTTON_SRC, m_cSRC);
	DDX_Text(pDX, IDC_EDIT_DEST, m_strDEST);
	DDX_Text(pDX, IDC_EDIT_SRC, m_strSRC);
}

BEGIN_MESSAGE_MAP(CTCMLParserDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SRC, OnBnClickedButtonSrc)
	ON_BN_CLICKED(IDC_BUTTON_DEST, OnBnClickedButtonDest)
	ON_BN_CLICKED(ID_COMPILE, OnBnClickedCompile)
END_MESSAGE_MAP()


// CTCMLParserDlg message handlers

BOOL CTCMLParserDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTCMLParserDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTCMLParserDlg::OnPaint() 
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
HCURSOR CTCMLParserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTCMLParserDlg::OnBnClickedButtonSrc()
{
	CFileDialog dlg(
		TRUE,
		_T("*.tsc"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Tachyon Script Files (*.tsc)|*.tsc|All Files (*.*)|*.*||"));

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

void CTCMLParserDlg::OnBnClickedButtonDest()
{
	CFileDialog dlg(
		FALSE,
		_T("*.tif"),
		"TClientCmd",
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("T-Project Interface Data Files (*.tif)|*.tif||"));

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

void CTCMLParserDlg::OnBnClickedCompile()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	TCMLParser vTParser;

	COMP_MAP::iterator itCOMP;
	FONT_MAP::iterator itFONT;

	int nret = vTParser.Parse((char *) LPCTSTR(m_strSRC));
	if( nret != 0 )
	{
		CString strFMT;
		strFMT.Format("Error :%d",
			nret);
		AfxMessageBox( strFMT );
	}

	int nCount = INT(vTParser.m_Comps.size());
	vDEST.Write( &nCount, sizeof(int));

	for( itCOMP = vTParser.m_Comps.begin(); itCOMP != vTParser.m_Comps.end(); itCOMP++)
		WriteFRAME( &vDEST, (*itCOMP).second);

	nCount = INT(vTParser.m_Fonts.size());
	vDEST.Write( &nCount, sizeof(int));

	for( itFONT = vTParser.m_Fonts.begin(); itFONT != vTParser.m_Fonts.end(); itFONT++)
		vDEST.Write( (*itFONT).second, sizeof(TCML_LOGFONT));

	CDialog::OnOK();
}

int CTCMLParserDlg::GetNodeCount( LP_COMPDESC pCOMP)
{
	int nCount = 0;

	while(pCOMP)
	{
		pCOMP = pCOMP->next;
		nCount++;
	}

	return nCount;
}

void CTCMLParserDlg::WriteFRAME( CFile *pFILE, LP_COMPDESC pCOMP)
{
	pFILE->Write( &pCOMP->id, sizeof(DWORD));
	pFILE->Write( &pCOMP->type, sizeof(BYTE));

	pFILE->Write( pCOMP->menu, TCML_MENU_COUNT * sizeof(DWORD));
	pFILE->Write( pCOMP->images, 2 * sizeof(DWORD));
	pFILE->Write( &pCOMP->tipface, sizeof(DWORD));
	pFILE->Write( &pCOMP->face, sizeof(DWORD));
	pFILE->Write( &pCOMP->style, sizeof(DWORD));
	pFILE->Write( &pCOMP->color, sizeof(DWORD));
	pFILE->Write( &pCOMP->sound, sizeof(DWORD));

	pFILE->Write( &pCOMP->hmargine, sizeof(int));
	pFILE->Write( &pCOMP->vmargine, sizeof(int));
	pFILE->Write( &pCOMP->x, sizeof(int));
	pFILE->Write( &pCOMP->y, sizeof(int));
	pFILE->Write( &pCOMP->width, sizeof(int));
	pFILE->Write( &pCOMP->height, sizeof(int));

	pFILE->Write( &pCOMP->display, sizeof(BYTE));
	pFILE->Write( &pCOMP->align, sizeof(BYTE));
	pFILE->Write( &pCOMP->ex, sizeof(TSATR));

	int nCount = INT(strlen(pCOMP->tooltip));
	pFILE->Write( &nCount, sizeof(int));
	if( nCount > 0 )
		pFILE->Write( pCOMP->tooltip, nCount * sizeof(char));

	nCount = INT(strlen(pCOMP->text));
	pFILE->Write( &nCount, sizeof(int));
	if( nCount > 0 )
		pFILE->Write( pCOMP->text, nCount * sizeof(char));

	if(pCOMP->child)
	{
		LP_COMPDESC pCHILD = pCOMP->child;
		nCount = GetNodeCount(pCHILD);

		pFILE->Write( &nCount, sizeof(int));
		while(pCHILD)
		{
			WriteFRAME( pFILE, pCHILD);
			pCHILD = pCHILD->next;
		}
	}
	else
	{
		nCount = 0;
		pFILE->Write( &nCount, sizeof(int));
	}
}
