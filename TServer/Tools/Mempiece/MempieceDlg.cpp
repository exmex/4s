// MempieceDlg.cpp : 구현 파일
//

#include "stdafx.h"

#include "Unzip.h"
#include "Mempiece.h"
#include "MempieceDlg.h"
#include ".\mempiecedlg.h"
#include <XTrapArg.h>
#include <XTrap4Launcher.h>

// 현승룡 환경설정
#include "GameSet.h"

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

// CMempieceDlg 대화 상자 

CMempieceDlg::CMempieceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMempieceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//////////////////////////////////
// 현승룡 스킨이미지
CMempieceDlg::~CMempieceDlg() 
{
	if( sFlag )
	{
		m_bSkin.Detach();
		//::DeleteObject(hBmp);
	}

	// 버튼 깜박임
	if(m_bFlash)
	{
		m_bFlash = FALSE;
		KillTimer(1);
	}
}
//////////////////////////////////

void CMempieceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_DOWNLOAD_CURRENT, m_staticCurrent);
	DDX_Control(pDX, IDC_PROGRESS_CURRENT, m_progressCurrent);
	//DDX_Control(pDX, IDC_DOWNLOAD_TOTAL, m_staticTotal);
	DDX_Control(pDX, IDC_PROGRESS_TOTAL, m_progressTotal);
	DDX_Control(pDX, IDOK, m_bOK);
	DDX_Control(pDX, IDCANCEL, m_bCANCEL);	
	//DDX_Control(pDX, IDC_STATIC_CURRENT, m_strCurrent);
	//DDX_Control(pDX, IDC_STATIC_TOTAL, m_strTotal);
	DDX_Control(pDX, IDC_BUTTON_HOMEPAGE, m_bHome);
	DDX_Control(pDX, IDC_BUTTON_SETTING, m_bSetting);
}

BEGIN_MESSAGE_MAP(CMempieceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()	
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_HOMEPAGE, OnBnClickedButtonHomepage)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, OnBnClickedButtonSetting)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMempieceDlg 메시지 처리기

BOOL CMempieceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	WSADATA wsaDATA;
	WORD wVersionRequested = MAKEWORD( 2, 2);
	int nERROR = WSAStartup( wVersionRequested, &wsaDATA);

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
	m_bCancel = FALSE;
	m_blPatchFileUpdate = FALSE;
	m_bDownloading = FALSE;
	m_bIsFtpConnecting = FALSE;
	m_strAppName = APP_NAME;
	m_hDownload = NULL;
	
	InitWeb();

	if(!ReadRegistry())
	{
		OnCancel();
		return FALSE;
	}

	HMODULE hModule = ::GetModuleHandle(NULL);
	if(hModule == 0)
	{
		OnCancel();
		return FALSE;
	}
	::GetModuleFileName(hModule, m_szPatchFileName, _MAX_PATH);
	
	m_session.SetOwner(this);
	SessionStart(m_strIP, m_wPort);

	m_progressCurrent.SetRange( 0, 100 );
	m_progressTotal.SetRange( 0, 100 );

	CButton * pOK = (CButton *)GetDlgItem(IDOK);
	if(pOK)
		pOK->EnableWindow(FALSE);

	CButton * pSet = (CButton *)GetDlgItem(IDC_BUTTON_SETTING);
	if(pSet)
		pSet->EnableWindow(FALSE);

	m_bFlash = FALSE; // 버튼 깜박임

	////////////////////////////////////////////////////////////////
	// 현승룡 스킨이미지
	LoadSkin();

	m_bOK.LoadBitmap(IDB_BITMAP5, IDB_BITMAP1, IDB_BITMAP4, IDB_BITMAP17);
	m_bCANCEL.LoadBitmap(IDB_BITMAP8, IDB_BITMAP6, IDB_BITMAP7, 0);
	m_bHome.LoadBitmap(IDB_BITMAP11, IDB_BITMAP9, IDB_BITMAP10, 0);
	m_bSetting.LoadBitmap(IDB_BITMAP14, IDB_BITMAP12, IDB_BITMAP13, 0);

	CString str = _T("게임시작");
	m_bOK.SetToolTipText(&str);

	str = _T("게임종료");
	m_bCANCEL.SetToolTipText(&str);

	str = _T("홈페이지");
	m_bHome.SetToolTipText(&str);

	str = _T("환경설정");
	m_bSetting.SetToolTipText(&str);

	SetControlPos(); // 컨트롤 위치 지정

	// progress bar color change
	::SendMessage(m_progressCurrent, PBM_SETBARCOLOR, 0, (LPARAM)(COLORREF)RGB(0,169,157));
	::SendMessage(m_progressTotal, PBM_SETBARCOLOR, 0, (LPARAM)(COLORREF)RGB(0,169,157));

	m_progressCurrent.SetBkColor(TRANSPARENT);
	m_progressTotal.SetBkColor(TRANSPARENT);

	SetWindowText(m_strAppName);

	/*
	m_staticCurrent.SetTextColor( RGB(100, 100, 100) );
	m_staticCurrent.SetBkColor( RGB(230, 230, 255) );
	m_staticCurrent.SetLink(FALSE);

	m_staticTotal.SetTextColor( RGB(100, 100, 100) );
	m_staticTotal.SetBkColor( RGB(230, 230, 255) );
	m_staticTotal.SetLink(FALSE);

	m_strCurrent.SetTextColor( RGB(100, 100, 100) );
	m_strCurrent.SetBkColor( RGB(230, 230, 255) );
	m_strCurrent.SetLink(FALSE);

	m_strTotal.SetTextColor( RGB(100, 100, 100) );
	m_strTotal.SetBkColor( RGB(230, 230, 255) );
	m_strTotal.SetLink(FALSE);
	*/	
	////////////////////////////////////////////////////////////////	

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CMempieceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMempieceDlg::OnPaint() 
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
//////////////////////////////////////////////////////////////
// 현승룡 스킨이미지
void CMempieceDlg::LoadSkin()
{
	sFlag = m_bSkin.LoadBitmap(IDB_BITMAP16);
	
	if( sFlag )
	{
		BITMAP bmp;
		m_bSkin.GetBitmap(&bmp);

		SetWindowPos( NULL, 0, 0, bmp.bmWidth, bmp.bmHeight, SWP_NOZORDER );
	}
}
// 컨트롤 위치 지정
void CMempieceDlg::SetControlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_webCtrl.MoveWindow(rect.Width()/3-30, 92, rect.Width()/2+137, rect.Height()/2+46);

	m_progressCurrent.MoveWindow( rect.Width()/2-65, 426, rect.Width()/2+41, 8 );
	m_progressTotal.MoveWindow( rect.Width()/2-65, 442, rect.Width()/2+41, 8 );

	CRect bRect;
	GetDlgItem(IDOK)->GetWindowRect(&bRect);
	
	m_bHome.MoveWindow( 226, 470, bRect.Width(), bRect.Height() );
	m_bSetting.MoveWindow( 226+bRect.Width()+17, 470, bRect.Width(), bRect.Height() );
	m_bOK.MoveWindow( 226+bRect.Width()*2+34, 470, bRect.Width(), bRect.Height() );
	m_bCANCEL.MoveWindow( 226+bRect.Width()*3+51, 470, bRect.Width(), bRect.Height() );	
}
//////////////////////////////////////////////////////////////
BOOL CMempieceDlg::DestroyWindow()
{
	DownloadEnd();
	return CDialog::DestroyWindow();
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CMempieceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMempieceDlg::InitWeb()
{
	CRect rect;
	GetClientRect(&rect);

	CRect WebRect( 0, 0, rect.Width()/2, rect.Height()/2 );

	CString strUrl = _T("http://www.4story.com/news/launcher.asp");
	m_webCtrl.Create(_T("WebControl"), WS_CHILD|WS_VISIBLE, WebRect, (CWnd*)this, 1001 );
	m_webCtrl.Navigate( strUrl, NULL, NULL, NULL, NULL );
}

BOOL CMempieceDlg::ReadRegistry()
{
	BYTE bCurrent = FALSE;

	HKEY hKeyRet;
	HKEY hKey = HKEY_LOCAL_MACHINE;
	
	CString strSubkey;
	strSubkey = m_strAppName;
	strSubkey += _T("\\PB");

	m_strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

	int err = RegOpenKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
	{
		RegCloseKey(hKeyRet);
		hKey = HKEY_CURRENT_USER;

		bCurrent = TRUE;
	}

	err = RegCreateKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
		return FALSE;

	BYTE	data[1024];
	DWORD   type;
	DWORD   cbdata =1024;

	//version
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_VERSION, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	m_dwVersion = *((LPDWORD)data);
	m_dwNextVersion = m_dwVersion;

	//local path 
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_LOCAL, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_SZ)
		return FALSE;
	
	m_strLocal = m_strDownload = data;

	//exe file
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_EXE, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_SZ)
		return FALSE;

	m_strGame = data;

	//patch svr address
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_PATCHSVR, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_SZ)
		return FALSE;

	m_strIP = data;

	//patch svr port
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_PATCHPORT, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	m_wPort = *((LPWORD)data);

	/*
	/////////////////////////////////////////////////////////////////////////////////////
	// 현승룡 스킨 이미지
	//skin image
    cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_BACKGROUND, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_SZ)
		return FALSE;
	
	// directory 의 값에따라 수정예정
	m_strSkin.Format(_T("%s\\%s"), m_strLocal, data);
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	// 현승룡 그래픽 설정
	// window mode
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_WINDOW, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	m_dwWindowMode = *((LPWORD)data);
	// shader mode
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_SHADER, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	m_dwShaderMode = *((LPWORD)data);
	// character mode
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_CHARACTER, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	m_dwCharMode = *((LPWORD)data);
	// paper matrix mode
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_PAPERMATRIX, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	m_dwPaperMode = *((LPWORD)data);
	// background mode
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_BACK, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	m_dwBackMode = *((LPWORD)data);
	/////////////////////////////////////////////////////////////////////////////////////
	*/

	RegCloseKey(hKeyRet);

	if(bCurrent)
	{
		if(!CopyRegistry())
			return FALSE;
	}

	return TRUE;
}

BOOL CMempieceDlg::CopyRegistry()
{
	HKEY hKeyRet;
	HKEY hKey = HKEY_LOCAL_MACHINE;

	CString strSubkey;
	strSubkey = m_strAppName;
	strSubkey += _T("\\PB");

	m_strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

	int err = RegCreateKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
		return FALSE;

	BYTE	data[1024];
	DWORD   type = REG_DWORD;
	DWORD   cbData = 4;
	
	memcpy(data, &m_dwVersion, sizeof(DWORD));
	err = RegSetValueEx(hKeyRet, REG_VALUE_VERSION, 0, REG_DWORD, data, cbData);
	if(ERROR_SUCCESS != err)
		return FALSE;

	LPCSTR strLocal = m_strLocal;
	err = RegSetValueEx(
		hKeyRet,
		REG_VALUE_LOCAL, 
		0, 
		REG_SZ, 
		(LPBYTE)strLocal, 
		(DWORD) (lstrlen(strLocal)+1)*sizeof(TCHAR));
	if(ERROR_SUCCESS != err)
		return FALSE;

	LPCSTR strGame = m_strGame;
	err = RegSetValueEx(
		hKeyRet,
		REG_VALUE_EXE, 
		0, 
		REG_SZ, 
		(LPBYTE)strGame, 
		(DWORD) (lstrlen(strGame)+1)*sizeof(TCHAR));
	if(ERROR_SUCCESS != err)
		return FALSE;

	LPCSTR strIP = m_strIP;
	err = RegSetValueEx(
		hKeyRet,
		REG_VALUE_PATCHSVR, 
		0, 
		REG_SZ, 
		(LPBYTE)strIP, 
		(DWORD) (lstrlen(strIP)+1)*sizeof(TCHAR));
	if(ERROR_SUCCESS != err)
		return FALSE;

	DWORD dwPort = m_wPort;
	memcpy(data, &dwPort, sizeof(DWORD));
	err = RegSetValueEx(hKeyRet, REG_VALUE_PATCHPORT, 0, REG_DWORD, data, cbData);
	if(ERROR_SUCCESS != err)
		return FALSE;

	RegCloseKey(hKeyRet);
	return TRUE;
}

void CMempieceDlg::OnConnect(int nErrorCode)
{
	if(nErrorCode)
	{
		AfxMessageBox(LoadString(IDS_CONNECT_FAIL));
		return;
	}
	else
		SendCT_PATCH_REQ();
}
void CMempieceDlg::OnClose(int nErrorCode)
{
	if(nErrorCode)
	{
		DownloadEnd();
		CString str;
		str.Format(_T("Error Code %d"), nErrorCode);
		AfxMessageBox(str);
		//AfxMessageBox(LoadString(IDS_ERROR_CODE),nErrorCode);
	}
}
DWORD CMempieceDlg::OnReceive(CPacket * pPacket)
{
	switch(pPacket->GetID())
	{
		HP_RECEIVE(CT_PATCH_ACK)
	default:
		return 1;
	}
}
void CMempieceDlg::SessionStart(CString strIp, DWORD dwPort)
{
	if(!m_session.Start(strIp, dwPort))
		AfxMessageBox(LoadString(IDS_CONNECT_FAIL));
}
void CMempieceDlg::Say(CPacket * pPacket)
{
	m_session.Say(pPacket);
}

DWORD WINAPI CMempieceDlg::_Download(LPVOID lpParam)
{
	return ((CMempieceDlg*)lpParam)->Download();
}
DWORD CMempieceDlg::Download()
{
	m_bDownloading = TRUE;

	////////////////////////////////////////////////
	// 현승룡 웹 뜨기전까지 쓰레드 실행 막음
	while( m_webCtrl.GetBusy() )
	{
		Sleep(500);
	}
	////////////////////////////////////////////////

	CFile fileNew;
	CStdioFile * remotefile = NULL;

	__int64 nTotalRead = 0;
	size_t nTotal = m_qPatchFile.size();
	int nCurrent=0;
	DWORD	dwRead=0;

	while(m_bDownloading)
	{
		LPPATCHFILE pPatchFile = NULL;
		if(m_qPatchFile.size())
			pPatchFile = (LPPATCHFILE)m_qPatchFile.front();

		if(!pPatchFile || !m_bDownloading )
			break;

		if(!pPatchFile->m_strPath.IsEmpty())
		{
			CString strPathName = m_strDownload+_T("\\")+pPatchFile->m_strPath;
			if(!CreateDirectoryEx(strPathName))
			{
				m_bDownloading = FALSE;
				break;
			}
			if(!SetCurrentDirectory(strPathName))
			{
				m_bDownloading = FALSE;
				break;
			}
		}
		else
		{	
			if(!SetCurrentDirectory(m_strDownload))
			{
				m_bDownloading = FALSE;
				break;
			}
		}

		char	lpBuffer[1024+1];
		DWORD	dwNumberOfBytesRead;

		TCHAR   szCause[255];
		CString Cause;
		Cause.Format("YES");

		CString strURL = m_strFtpSvr + _T("/");
		if(!pPatchFile->m_strPath.IsEmpty())
			strURL += pPatchFile->m_strPath + _T("/");

		strURL += pPatchFile->m_strName;

		TRY
		{
			CInternetSession mysession;
			remotefile = mysession.OpenURL(strURL,1,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD);
			if(!remotefile)
			{
				m_bDownloading = FALSE;
				break;
			}

			remotefile->Seek(dwRead, CFile::begin);

			if(fileNew.m_hFile == CFile::hFileNull)
			{
				fileNew.Open(pPatchFile->m_strName, CFile::modeCreate | CFile::modeWrite);
				if(fileNew.m_hFile == CFile::hFileNull)
				{
					m_bDownloading = FALSE;
					break;
				}
			}

			while (dwNumberOfBytesRead = remotefile->Read(lpBuffer, 1024))
			{
				if(!m_bDownloading)
					break;

				fileNew.Write(lpBuffer, dwNumberOfBytesRead);
				dwRead += dwNumberOfBytesRead;
				nTotalRead += dwNumberOfBytesRead;
				Progress(pPatchFile->m_strName, pPatchFile->m_dwSize, dwRead, nTotalRead, PROGRESS_TYPE_DOWNLOAD);
			}

			remotefile->Close();
			delete remotefile;
			remotefile = NULL;
			dwRead = 0;

			fileNew.Close();

			if(m_bDownloading)
			{
				CString strSource, strTarget;
				strTarget.Empty();
				strSource.Empty();

				strSource = m_strDownload;
				strSource += _T("\\");

				if(!pPatchFile->m_strPath.IsEmpty())
				{
					strSource += pPatchFile->m_strPath + _T("\\");
					strTarget = _T("\\")+pPatchFile->m_strPath;
				}
				strSource += pPatchFile->m_strName;
				Progress(pPatchFile->m_strName, pPatchFile->m_dwSize, dwRead, nTotalRead, PROGRESS_TYPE_UNZIP);
				if( UNZ_OK != Unzip( strSource, strTarget ))
				{
					m_bDownloading = FALSE;
					break;
				}
				SetVersion(pPatchFile->m_dwVersion);

				delete pPatchFile;
				pPatchFile = NULL;
				m_qPatchFile.pop();

				if(m_blPatchFileUpdate)
					break;
			}
		}
		CATCH_ALL(error)
		{
			error->GetErrorMessage(szCause,254,NULL);

			if(remotefile)
			{
				remotefile->Close();
				delete remotefile;
				remotefile = NULL;
			}

			Cause.Format(LoadString(IDS_DOWNLOAD_RETRY), szCause);
			if(AfxMessageBox(Cause, MB_YESNO) == IDYES)
				continue;
			else
				m_bDownloading = FALSE;

			if(fileNew.m_hFile != CFile::hFileNull)
				fileNew.Close();
		}
		END_CATCH_ALL;
	}

	if(fileNew.m_hFile != CFile::hFileNull)
		fileNew.Close();

	if(remotefile)
	{
		remotefile->Close();
		delete remotefile;
		remotefile = NULL;
	}

	DownloadFinish();
	m_bDownloading = FALSE;

	return 0;
}
void CMempieceDlg::DownloadFinish()
{
	RemoveDownloadDirectory();

	if(m_bDownloading)
		CheckPatch();
	else
	{
		if(m_bCancel)
			PostMessage(WM_STOP_DOWNLOAD);
		else
		{
			AfxMessageBox(LoadString(IDS_FTP_FAIL));
			//m_staticCurrent.SetWindowText(LoadString(IDS_DOWNLOAD_FAIL));
		}
	}

	PostMessage(WM_CLOSE_SESSION);
}

void CMempieceDlg::DownloadEnd()
{
	if( m_hDownload )
	{
		m_bDownloading = FALSE;
		CloseHandle(m_hDownload);
		m_hDownload = NULL;
		RemoveDownloadDirectory();
	}

	while(m_qPatchFile.size())
	{
		delete m_qPatchFile.front();
		m_qPatchFile.pop();
	}

	if(m_session.m_hSocket != INVALID_SOCKET)
		m_session.End();
}
void CMempieceDlg::RemoveDownloadDirectory()
{
	DeleteDirectory(m_strDownload);

	if(SetCurrentDirectory(m_strLocal))
		_rmdir(m_strDownload);
}
void CMempieceDlg::DeleteDirectory(LPCTSTR strPath)
{
	WIN32_FIND_DATA FileData;
	if(SetCurrentDirectory(strPath))
	{
		HANDLE hSearch = FindFirstFile(_T("*"), &FileData);
		if( hSearch == INVALID_HANDLE_VALUE) return;

		BOOL bContinue = TRUE;
		while(bContinue)
		{
			if( FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if( 0!=strcmp(FileData.cFileName, ".") && 
					0!=strcmp(FileData.cFileName, "..") )
				{
					CString strFullPath = strPath;

					strFullPath+= _T("\\");
					strFullPath += FileData.cFileName;

					DeleteDirectory(strFullPath);
					if(SetCurrentDirectory(strPath))
						_rmdir(strFullPath);
				}
			}
			else
				DeleteFile(FileData.cFileName);

			if(!FindNextFile(hSearch, &FileData)) bContinue = FALSE;
		}

		FindClose(hSearch);
	}
}

void CMempieceDlg::CheckPatch()
{
	if(m_blPatchFileUpdate)
		PostMessage(WM_RESTART_PATCH);
	else
	{
		m_strDownload.ReleaseBuffer();
		CString strNew=m_szPatchFileName;
        	
		//m_staticCurrent.SetWindowText(LoadString(IDS_DOWNLOAD_SUCCESS));

		CButton * pOK = (CButton *)GetDlgItem(IDOK);
		if(pOK)
		{
			m_bFlash = TRUE; // 버튼 깜박임
			pOK->EnableWindow(TRUE);
			SetTimer(1, 500, NULL);
		}

		int n = strNew.Find(BACKUP_EXT);
		if(n > 0)
		{
			strNew.Delete(n,lstrlen(BACKUP_EXT));
			n = strNew.ReverseFind('\\');
			if(n >= 0)
				strNew.Delete(0, n+1);

			CopyFile(m_szPatchFileName, strNew, FALSE);
		}
	}	
}

CString CMempieceDlg::LoadString(int nID)
{
	CString strMsg;
	strMsg.LoadString(nID);
	return strMsg;
}
int CMempieceDlg::Unzip(LPCTSTR strZip, LPCTSTR strDirectory)
{
	int err = UNZ_OK;
	unz_global_info zipinfo;

	unzFile zip = unzOpen( strZip );
	if( NULL == zip )
		return -1;

	err = unzGetGlobalInfo( zip, &zipinfo );
	if( UNZ_OK != err)
	{
		unzClose( zip );
		return err;
	}

	DWORD totalcnt = zipinfo.number_entry;

	err =unzGoToFirstFile( zip );
	if( UNZ_OK != err)
	{
		unzClose( zip );
		return err;
	}

	for( DWORD i = 0; i < totalcnt && err == UNZ_OK; i++)
	{
		if( UNZ_OK == err || UNZ_END_OF_LIST_OF_FILE == err )
		{
			TCHAR szFileName[256];
			unz_file_info file_info;

			err = unzGetCurrentFileInfo( zip, &file_info, szFileName, 256, NULL, 0, NULL, 0);
			if( UNZ_OK != err )
			{
				unzClose( zip );
				return err;
			}
						
			VerifyDirectory(m_strLocal, strDirectory, FALSE);
			
			CString strTargetDir = m_strLocal + strDirectory;
			if( !SetCurrentDirectory( strTargetDir ))
			{
				unzClose( zip );
				return -1;
			}

			CString strOld, strNew;
			char szPatchFileName[_MAX_PATH], szFileExt[_MAX_PATH];
			_splitpath(m_szPatchFileName, NULL, NULL, szPatchFileName, szFileExt);

			strOld = strTargetDir+szFileName;
			if(!lstrcmpi(strOld, m_szPatchFileName))
			{
				CString strNew = szPatchFileName;
				strNew += BACKUP_EXT;
				strNew += szFileExt;
				lstrcpy(szFileName, strNew);
				m_blPatchFileUpdate = TRUE;
			}

			VerifyDirectory(strTargetDir, szFileName, TRUE);

			CFile file;
			if(!file.Open(szFileName, CFile::modeCreate|CFile::shareExclusive|CFile::modeWrite))
			{
				SetFileAttributes( szFileName, FILE_ATTRIBUTE_NORMAL );
				if( !file.Open(szFileName, CFile::modeCreate|CFile::shareExclusive|CFile::modeWrite) )
				{
					unzClose( zip );
					return -1; // 수정
				}
			}

			err = unzOpenCurrentFile( zip );
			if( UNZ_OK != err)
			{
				unzClose( zip);
				file.Close();

				return err;
			}

			DWORD dwSize = file_info.uncompressed_size;
			LPVOID data =  malloc(dwSize);

			if(! data )
			{
				unzClose( zip);
				file.Close();
				return -1;
			}

			err = unzReadCurrentFile( zip, data, dwSize );
			if( err < 0)
			{
				unzCloseCurrentFile( zip );
				unzClose( zip );
				file.Close();
				return err;
			}
			
			file.Write(data, dwSize);
			file.Flush();

			free(data);

			if( UNZ_CRCERROR == unzCloseCurrentFile( zip ))
			{
				unzClose( zip);
				file.Flush();
				file.Close();
				return UNZ_CRCERROR;
			}

			file.Close();
		}
		err = unzGoToNextFile( zip );
	}

	return unzClose( zip );
}
BOOL CMempieceDlg::SetVersion(DWORD dwVer)
{
	HKEY hKeyRet;
	HKEY hKey = HKEY_LOCAL_MACHINE;

	CString strSubkey;
	strSubkey = m_strAppName;
	strSubkey += _T("\\PB");

	m_strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

	int err = RegCreateKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
		return FALSE;

	BYTE	data[1024];
	DWORD   type = REG_DWORD;
	DWORD   cbData = 4;
	
	memcpy(data, &dwVer, sizeof(DWORD));

	err = RegSetValueEx(hKeyRet, REG_VALUE_VERSION, 0, REG_DWORD, data, cbData);
	if(ERROR_SUCCESS != err)
		return FALSE;

	RegCloseKey(hKeyRet);
	return TRUE;
}
void CMempieceDlg::VerifyDirectory(LPCTSTR target, LPCTSTR path, BOOL bHaveFile)
{
	TCHAR parse[256];
	char * tok = NULL;
	CString strDirectory;

	lstrcpy( parse, path );
	tok = strtok((char *)parse, PATH_DELIMETERS);
	if( !tok )
		return;

	strDirectory = target;

	if( bHaveFile )
	{
		do
		{
			if(lstrcmp(tok, _T(".")))
			{
				CreateDirectory(strDirectory, NULL);
				strDirectory += '\\';
				strDirectory += tok;
			}
		}while(NULL!= (tok = strtok(NULL, PATH_DELIMETERS)));
	}
	else
	{	
		CreateDirectory(strDirectory, NULL);										
		do
		{
			if(lstrcmp(tok, _T(".")))
			{	
				strDirectory += '\\';
				strDirectory += tok;
				CreateDirectory(strDirectory, NULL);										
			}
		}while(NULL!= (tok = strtok(NULL, PATH_DELIMETERS)));
	}
}
BOOL CMempieceDlg::CreateDirectoryEx(CString strPath)
{
	if( strPath.IsEmpty() ) return TRUE;

	if( !CreateDirectory( strPath, NULL ))
	{
		DWORD dwErr = GetLastError();
		if( dwErr == ERROR_ALREADY_EXISTS ) return TRUE;

		if( dwErr == ERROR_PATH_NOT_FOUND )
		{
			int nPos = 0;
			int nNext = strPath.Find(_T("\\"), 0);
			while(nNext >=0 )
			{
				nPos = nNext;
				nNext = strPath.Find(_T("\\"), nPos+1 );
			}

			CString strCreate = strPath.Left( nPos );
			if( CreateDirectoryEx( strCreate) )
				return CreateDirectory(strPath, NULL );
		}
		return FALSE;
	}

	return TRUE;
}
void CMempieceDlg::Progress(CString strName, DWORD dwSize, DWORD dwCurrent, __int64 nTotal, BYTE bType)
{
	//CString strCurrent;
	//CString strTotal;

	if(!m_bDownloading)
		return;

	switch(bType)
	{
		/*
	case PROGRESS_TYPE_UNZIP:
		{
			strCurrent.Format("%s %s",strName, LoadString(IDS_UNZIPPING));
			m_staticCurrent.SetWindowText(strCurrent);
		}
		break;
	case PROGRESS_TYPE_START:
		{
			strCurrent.Format("%s", LoadString(IDS_CONNECT_FTP));
			m_staticCurrent.SetWindowText(strCurrent);

			strTotal.Format("0/%d KBytes", m_dwPatchTotalSize/1000);
			m_staticTotal.SetWindowText(strTotal);
		}
		break;
		*/
	case PROGRESS_TYPE_DOWNLOAD:
		{
			//strCurrent.Format("%s(%d/%d) %s", strName, dwCurrent, dwSize, LoadString(IDS_DOWNLOADING));
			//m_staticCurrent.SetWindowText(strCurrent);

			int nPos = int(dwSize ? __int64(100) * dwCurrent / dwSize : 0);
			m_progressCurrent.SetPos( nPos );

			nPos = int(100 * nTotal / m_nPatchTotalSize);

			//strTotal.Format("%d/%d KBytes",dwTotal, m_dwPatchTotalSize/1000);
			//m_staticTotal.SetWindowText(strTotal);
			m_progressTotal.SetPos( nPos );
		}
		break;
	}	
}

void CMempieceDlg::StartGame()
{
	if(SetCurrentDirectory( m_strLocal ))
	{
		STARTUPINFO StartupInfo;
		PROCESS_INFORMATION ProcessInfo;

		ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
		ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION)); 

		StartupInfo.cb = sizeof(STARTUPINFO);
		StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
		StartupInfo.wShowWindow = SW_SHOWNORMAL;
		
		CString strCommandLine;
		strCommandLine.Format(_T("%s %s %d"), m_strGame, m_strGameSvr, m_wGamePort);

		if(!CreateProcess(m_strGame, strCommandLine.LockBuffer(), 
			NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
		{
			strCommandLine.ReleaseBuffer();
			return;
		}

		strCommandLine.ReleaseBuffer();
	}
}

DWORD CMempieceDlg::OnCT_PATCH_ACK(CPacket * pPacket)
{
	m_nPatchTotalSize = 0;

	DWORD dwLoginAddr;
	WORD wLoginPort;

	WORD wCount;
	DWORD dwVersion;
	CString strPath;
	CString strName;
	DWORD dwSize;

	(*pPacket)
		>> m_strFtpSvr
		>> dwLoginAddr
		>> wLoginPort
		>> wCount;

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = dwLoginAddr;
	m_strGameSvr = inet_ntoa(addr.sin_addr);
	m_wGamePort = wLoginPort;

	for(WORD i=0; i<wCount; i++)
	{
		(*pPacket)
			>> dwVersion
			>> strPath
			>> strName
			>> dwSize;

		LPPATCHFILE pFile = new PATCHFILE();
		pFile->m_dwVersion = dwVersion;
		pFile->m_strPath = strPath;
		pFile->m_strName = strName;
		pFile->m_dwSize = dwSize;

		m_nPatchTotalSize += dwSize;
		m_qPatchFile.push(pFile);
	}

	SendCT_PATCHSTART_REQ();

	Progress(_T(""), 0, 0, 0, PROGRESS_TYPE_START);

	m_strDownload += _T("\\_download");
	CFileFind ff;
	if(ff.FindFile(m_strDownload))
		RemoveDownloadDirectory();

	if(wCount)
	{
		DWORD dwThreadID;
		CreateDirectory( m_strDownload , NULL);
		m_hDownload = CreateThread(NULL, 0, _Download, (LPVOID)this, CREATE_SUSPENDED, &dwThreadID);
		if( !m_hDownload )
		{
			AfxMessageBox(LoadString(IDS_THREAD_FAIL));
			return 1;
		}
		ResumeThread(m_hDownload);
	}
	else
	{
		m_progressCurrent.SetPos(100);
		m_progressTotal.SetPos(100);
		CheckPatch();
		return 1;
	}
	return 0;
}
void CMempieceDlg::SendCT_PATCH_REQ()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_PATCH_REQ)
		<< m_dwVersion;
	Say(pMsg);
}

void CMempieceDlg::SendCT_PATCHSTART_REQ()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_PATCHSTART_REQ);
	Say(pMsg);
}

void CMempieceDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 버튼 깜박임
	if(m_bFlash)
	{
		m_bFlash = FALSE;
		KillTimer(1);
	}

	if(m_bDownloading)
	{
		m_bCancel = TRUE;
		DownloadEnd();
	}
	else
		DestroyWindow();
}

void CMempieceDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 버튼 깜박임
    if(m_bFlash)
	{
		m_bFlash = FALSE;
		KillTimer(1);
	}

	XTrap_L_Patch((char*)xArg, NULL, 60000);

	StartGame();
	OnOK();
}

LRESULT CMempieceDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch(message)
	{
	case WM_RESTART_PATCH:
		{
			OnCancel();

			m_strDownload.ReleaseBuffer();
			CString strNew=m_szPatchFileName;

			STARTUPINFO StartupInfo;
			PROCESS_INFORMATION ProcessInfo;

			ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
			ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION)); 

			StartupInfo.cb = sizeof(STARTUPINFO);
			StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
			StartupInfo.wShowWindow = SW_SHOWNORMAL;

			int n = strNew.ReverseFind('.');
			strNew.Insert(n,BACKUP_EXT);

			if(!CreateProcess(strNew, NULL, 
				NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
				AfxMessageBox(IDS_DOWNLOAD_FAIL);
		}
		break;
	case WM_STOP_DOWNLOAD:
		DestroyWindow();
		break;

	case WM_CLOSE_SESSION:
		DownloadEnd();
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////////////
// 현승룡 스킨 이미지
HBRUSH CMempieceDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT nID = pWnd->GetDlgCtrlID();
	
	// static text box 투명하게 하기
	/*if( nID == IDC_HOMEPAGE )
	{
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)::GetStockObject(NULL_BRUSH);
    }*/
	
	return hbr;
}

void CMempieceDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 타이틀바가 없는 윈도우 위치 바꾸기
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CMempieceDlg::OnEraseBkgnd(CDC* pDC)
{	
	if( sFlag )
	{	
		CRect rect;
		GetClientRect(&rect);		

		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap* pbmpOld = MemDC.SelectObject( &m_bSkin );

		BITMAP bmp;
		m_bSkin.GetBitmap(&bmp);

		pDC->StretchBlt(0, 0, rect.right, rect.bottom, &MemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		MemDC.SelectObject( pbmpOld );

		ReleaseDC(pDC);
	}	

	return TRUE;

//	return CDialog::OnEraseBkgnd(pDC);
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Clicked Homepage / Setting Button
void CMempieceDlg::OnBnClickedButtonHomepage()
{
	ShellExecute(NULL, _T("open"), _T("iexplore"), HOMEPAGE_NAME, NULL, SW_SHOWNORMAL);
}

void CMempieceDlg::OnBnClickedButtonSetting()
{
	/*
	CGameSet dlg;

	dlg.SetGraphicMode(m_dwWindowMode, m_dwShaderMode, m_dwCharMode, m_dwPaperMode, m_dwBackMode);

	if( dlg.DoModal() == IDOK )
	{
		dlg.GetGraphicMode(&m_dwWindowMode, &m_dwShaderMode, &m_dwCharMode, &m_dwPaperMode, &m_dwBackMode);
		WriteRegistry();
	}
	*/
}
BOOL CMempieceDlg::WriteRegistry()
{
	/*
	HKEY hKeyRet;
	HKEY hKey = HKEY_LOCAL_MACHINE;

	CString strSubkey;
	strSubkey = m_strAppName;
	strSubkey += _T("\\PB");

	m_strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

	int err = RegCreateKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
		return FALSE;

	BYTE	data[1024];
	DWORD   type = REG_DWORD;
	DWORD   cbData = 4;

	// window mode
	memcpy(data, &m_dwWindowMode, sizeof(DWORD));
	err = RegSetValueEx(hKeyRet, REG_VALUE_WINDOW, 0, REG_DWORD, data, cbData);
	if(ERROR_SUCCESS != err)
		return FALSE;

	// shader mode
	memcpy(data, &m_dwShaderMode, sizeof(DWORD));
	err = RegSetValueEx(hKeyRet, REG_VALUE_SHADER, 0, REG_DWORD, data, cbData);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	// character mode
	memcpy(data, &m_dwCharMode, sizeof(DWORD));
	err = RegSetValueEx(hKeyRet, REG_VALUE_CHARACTER, 0, REG_DWORD, data, cbData);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	// paper matrix mode
	memcpy(data, &m_dwPaperMode, sizeof(DWORD));
	err = RegSetValueEx(hKeyRet, REG_VALUE_PAPERMATRIX, 0, REG_DWORD, data, cbData);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	// background mode
	memcpy(data, &m_dwBackMode, sizeof(DWORD));
	err = RegSetValueEx(hKeyRet, REG_VALUE_BACK, 0, REG_DWORD, data, cbData);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;

	RegCloseKey(hKeyRet);
	*/
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
void CMempieceDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		m_bOK.FlashButton();
		m_bFlash++;
		break;
	}

	if(m_bFlash == 10)
		KillTimer(1);

	CDialog::OnTimer(nIDEvent);
}
