// TConfigMPDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "TConfigMP.h"
#include "TConfigMPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

// 구현
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


// CTConfigMPDlg 대화 상자



CTConfigMPDlg::CTConfigMPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTConfigMPDlg::IDD, pParent)
	, m_strMainPE(_T(""))
	, m_strDEST(_T(""))
	, m_strSRC(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTConfigMPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MAIN_PE, m_strMainPE);
	DDX_Text(pDX, IDC_EDIT_DEST, m_strDEST);
	DDX_Text(pDX, IDC_EDIT_SRC, m_strSRC);
	DDX_Control(pDX, IDC_BUTTON_MAIN_PE, m_cMainPE);
	DDX_Control(pDX, IDC_BUTTON_DEST, m_cDEST);
	DDX_Control(pDX, IDC_BUTTON_SRC, m_cSRC);
}

BEGIN_MESSAGE_MAP(CTConfigMPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_MAIN_PE, OnBnClickedButtonMainPe)
	ON_BN_CLICKED(IDC_BUTTON_DEST, OnBnClickedButtonDest)
	ON_BN_CLICKED(IDC_BUTTON_SRC, OnBnClickedButtonSrc)
	ON_BN_CLICKED(ID_COMPILE, OnBnClickedCompile)
END_MESSAGE_MAP()


// CTConfigMPDlg 메시지 처리기

BOOL CTConfigMPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_cMainPE.SetIcon(hIcon);
	m_cDEST.SetIcon(hIcon);
	m_cSRC.SetIcon(hIcon);
	srand(::GetCurrentTime());

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CTConfigMPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CTConfigMPDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CTConfigMPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTConfigMPDlg::OnBnClickedButtonMainPe()
{
	CFileDialog dlg(
		TRUE,
		_T("*.exe"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("PE Files (*.exe)|*.exe|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strMAINPEFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strMAINPEFOLDER = dlg.GetFileName();
		m_strMainPE = dlg.GetPathName();
		m_strMAINPEFOLDER = m_strMainPE.Left(m_strMainPE.GetLength() - m_strMAINPEFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTConfigMPDlg::OnBnClickedButtonDest()
{
	CFileDialog dlg(
		FALSE,
		_T("*.mpc"),
		"TClientMP",
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Module Protector Config Files (*.mpc)|*.mpc||"));

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

void CTConfigMPDlg::OnBnClickedButtonSrc()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Module list Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

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

void CTConfigMPDlg::OnBnClickedCompile()
{
	UpdateData();

	m_strMainPE.Trim();
	m_strDEST.Trim();
	m_strSRC.Trim();

	if( m_strMainPE.IsEmpty() ||
		m_strDEST.IsEmpty() ||
		m_strSRC.IsEmpty() )
	{
		AfxMessageBox(IDS_ERROR_WRONG_PARAM);
		return;
	}

	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	BYTE vMASK[TMAX_CHECK_CODE_SIZE] = {
		BYTE(rand() % 256),
		BYTE(rand() % 256),
		BYTE(rand() % 256),
		BYTE(rand() % 256),
		BYTE(rand() % 256),
		BYTE(rand() % 256),
		BYTE(rand() % 256)};

	BYTE bMASK = BYTE(rand() % (TMAX_CHECK_CODE_SIZE - TMIN_CHECK_CODE_SIZE + 1)) + TMIN_CHECK_CODE_SIZE;
	BYTE bMIDX = 0;

	vDEST.Write( &bMASK, sizeof(BYTE));
	vDEST.Write( vMASK, bMASK);

	if(BuildPEINFO( &vDEST, vMASK, &bMIDX, bMASK))
	{
		BuildMODULEINFO( &vDEST, vMASK, &bMIDX, bMASK);
		AfxMessageBox(IDS_SUCCESS);
	}
}

BYTE CTConfigMPDlg::BuildPEINFO( CFile *pFile,
								 LPBYTE pMASK,
								 LPBYTE pMIDX,
								 BYTE bMASK)
{
	CFile vMAIN( LPCSTR(m_strMainPE), CFile::modeRead|CFile::typeBinary);
	DWORD dwLength = (DWORD) vMAIN.GetLength();

	if(!dwLength)
	{
		CString strNAME = pFile->GetFilePath();

		pFile->Close();
		DeleteFile(LPCSTR(strNAME));
		AfxMessageBox(IDS_ERROR_SIZE);

		return FALSE;
	}

	LPBYTE pMAIN = new BYTE[dwLength];
	vMAIN.Read( pMAIN, dwLength);

	PIMAGE_NT_HEADERS pNTMAIN = GetNTHeader(pMAIN);
	if(!pNTMAIN)
	{
		CString strNAME = pFile->GetFilePath();

		pFile->Close();
		DeleteFile(LPCSTR(strNAME));
		AfxMessageBox(IDS_ERROR_INVALID_PE);

		if(pMAIN)
		{
			delete[] pMAIN;
			pMAIN = NULL;
		}

		return FALSE;
	}

	WriteDATA( &pNTMAIN->FileHeader.TimeDateStamp, pFile, pMASK, pMIDX, bMASK, sizeof(DWORD));
	if(pMAIN)
	{
		delete[] pMAIN;
		pMAIN = NULL;
	}

	return TRUE;
}

PIMAGE_NT_HEADERS CTConfigMPDlg::GetNTHeader( LPBYTE pDATA)
{
	PIMAGE_DOS_HEADER pDOS = (PIMAGE_DOS_HEADER) pDATA;
	if( pDOS->e_magic != IMAGE_DOS_SIGNATURE )
		return NULL;

	PIMAGE_NT_HEADERS pNT = (PIMAGE_NT_HEADERS) (pDATA + pDOS->e_lfanew);
	if( pNT->Signature != IMAGE_NT_SIGNATURE )
		return NULL;

	return pNT;
}

void CTConfigMPDlg::BuildMODULEINFO( CFile *pFile,
									 LPBYTE pMASK,
									 LPBYTE pMIDX,
									 BYTE bMASK)
{
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CString strESSMODULE(_T("Kernel32"));
	CString strESSPROC[TESSPROC_COUNT] = {
		CString(_T("CheckRemoteDebuggerPresent")),
		CString(_T("IsDebuggerPresent"))};

	CString strDATA = ReadString(&vSRC);
	DWORD dwMCOUNT;
	WORD wDATA;

	sscanf( LPCSTR(strDATA), _T("%d"), &dwMCOUNT);
	wDATA = WORD(dwMCOUNT);

	WriteDATA( &wDATA, pFile, pMASK, pMIDX, bMASK, sizeof(WORD));
	for( DWORD i=0; i<dwMCOUNT; i++)
	{
		strDATA = ReadString(&vSRC);
		WriteString( &strDATA, pFile, pMASK, pMIDX, bMASK);
	}

	WriteString( &strESSMODULE, pFile, pMASK, pMIDX, bMASK);
	for( i=0; i<TESSPROC_COUNT; i++)
		WriteString( &strESSPROC[i], pFile, pMASK, pMIDX, bMASK);

	strDATA = ReadString(&vSRC);
	sscanf( LPCSTR(strDATA), _T("%d"), &dwMCOUNT);
	wDATA = WORD(dwMCOUNT);
	WriteDATA( &wDATA, pFile, pMASK, pMIDX, bMASK, sizeof(WORD));

	for( i=0; i<dwMCOUNT; i++)
	{
		BYTE vTEXTBUF[MAX_PATH];
		DWORD dwPCOUNT;

		strDATA = ReadString(&vSRC);
		ZeroMemory( vTEXTBUF, MAX_PATH);

		sscanf( LPCSTR(strDATA), _T("%s %d"), vTEXTBUF, &dwPCOUNT);
		wDATA = WORD(dwPCOUNT);

		WriteDATA( &wDATA, pFile, pMASK, pMIDX, bMASK, sizeof(WORD));
		WriteString( &CString(vTEXTBUF), pFile, pMASK, pMIDX, bMASK);

		for( DWORD j=0; j<dwPCOUNT; j++)
		{
			strDATA = ReadString(&vSRC);
			WriteString( &strDATA, pFile, pMASK, pMIDX, bMASK);
		}
	}
}

CString CTConfigMPDlg::ReadString( CStdioFile *pFILE)
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

void CTConfigMPDlg::WriteDATA( LPVOID pDATA,
							   CFile *pFile,
							   LPBYTE pMASK,
							   LPBYTE pMIDX,
							   BYTE bMASK,
							   int nSIZE)
{
	LPBYTE pBUF = (LPBYTE) pDATA;

	for( int i=0; i<nSIZE; i++)
	{
		BYTE bCH = pBUF[i] ^ pMASK[(*pMIDX)];

		(*pMIDX) = ((*pMIDX) + 1) % bMASK;
		pFile->Write( &bCH, sizeof(BYTE));
	}
}

void CTConfigMPDlg::WriteString( CString *pTEXT,
								 CFile *pFile,
								 LPBYTE pMASK,
								 LPBYTE pMIDX,
								 BYTE bMASK)
{
	WORD wSIZE = (WORD) pTEXT->GetLength();

	WriteDATA(
		&wSIZE,
		pFile,
		pMASK,
		pMIDX,
		bMASK,
		sizeof(WORD));

	WriteDATA(
		(LPVOID) LPCSTR(*pTEXT),
		pFile,
		pMASK,
		pMIDX,
		bMASK,
		wSIZE);
}
