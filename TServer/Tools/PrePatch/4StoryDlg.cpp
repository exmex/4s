// 4StoryDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include ".\4storydlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CStoryDlg::CStoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStoryDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_TMENU);

	m_bConnect		= TRUE;
	m_bDownloading	= NULL;
	m_hDownload		= NULL;
	m_bFlag			= NULL;
	m_bShow			= NULL;
	m_bFinish		= FALSE;
	m_dwLastVer		= 0;
	m_bInCom		= FALSE;
	m_nTotalPos		= 0;

	m_rtText.left	= 15;
	m_rtText.top	= 10;
	m_rtText.right	= 180;
	m_rtText.bottom = 24;
}

CStoryDlg::~CStoryDlg() 
{
	if( m_bFlag )
		m_bSkin.Detach();
	::Shell_NotifyIcon( NIM_DELETE, &m_nid );
}

void CStoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_CURRENT, m_progressCurrent);
	DDX_Control(pDX, IDC_PROGRESS_TOTAL, m_progressTotal);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
}

BEGIN_MESSAGE_MAP(CStoryDlg, CDialog)
	ON_MESSAGE(UM_TRAY, OnTray)	
	ON_COMMAND(ID_MENU_EXIT, OnCancel)
	ON_COMMAND(ID_MENU_CONNECT, OnMenuConnect)
	ON_COMMAND(IDC_BTN_CLOSE, OnCloseBtn)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CStoryDlg 메시지 처리기
BOOL CStoryDlg::OnInitDialog()
{
	LOGMSG(_T("=====PrePatch Run====="));
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//	트레이아이콘 설정
	GotoTray();

	HMODULE hModule = ::GetModuleHandle(NULL);
	if(hModule == 0)
	{					
		OnCancel();
		ERRMSG(_T("ModuleHandle error"));
		return FALSE;
	}
	::GetModuleFileName(hModule, m_szPatchFileName, _MAX_PATH);
	

	char GAME_TITLE[256]={0,};	
	CString strNew = m_szPatchFileName;	
	int n = strNew.Find(APP_EXT);
	if(n > 0)
	{
		strNew.Delete(n,lstrlen(APP_EXT));
		n = strNew.ReverseFind('\\');
		if(n >= 0)
			strNew.Delete(0, n+1);
	}
	strcpy(GAME_TITLE,strNew);	

	HANDLE hMutex = NULL;
	if( !OpenMutex(MUTEX_ALL_ACCESS,false,GAME_TITLE) )
		hMutex = CreateMutex(NULL,TRUE,GAME_TITLE);
	else
	{
#ifdef _GOA
		AfxMessageBox(_T("GOA PrePatch Already Running"));
#else
		AfxMessageBox(_T("4Story PrePatch Already Running"));
#endif
		OnCancel();
		return FALSE;
	}

	WSADATA wsaDATA;
	WORD wVersionRequested = MAKEWORD( 2, 2);
	int nERROR = WSAStartup( wVersionRequested, &wsaDATA);

	ReadTextFile();

	if(!ReadRegistry())
	{
		ERRMSG(_T("Registry error"));
		OnCancel();
		return FALSE;
	}

	DisplayText("Connect");
	m_session.SetOwner(this);
	SessionStart();

	LoadSkin();

	SetWindowText(APP_NAME);

	//IDC_FILENAME
	m_progressCurrent.SetRange( 0, 100 );
	m_progressTotal.SetRange( 0, 100 );

	m_progressCurrent.SetPos( 0 );		
	m_progressTotal.SetPos( 0 );

	// progress bar color change
	::SendMessage(m_progressCurrent, PBM_SETBARCOLOR, 0, (LPARAM)(COLORREF)RGB(200,180,50));
	::SendMessage(m_progressTotal, PBM_SETBARCOLOR, 0, (LPARAM)(COLORREF)RGB(200,180,50));
	
	m_progressCurrent.SetBkColor(TRANSPARENT);
	m_progressTotal.SetBkColor(TRANSPARENT);

	InfoFileRead();
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CStoryDlg::GotoTray(void)
{		
	::ZeroMemory(&m_nid, sizeof(m_nid));
	m_nid.cbSize				= sizeof(NOTIFYICONDATA);
	m_nid.hWnd					= m_hWnd;
	m_nid.uID					= IDI_TMENU;
	m_nid.uFlags				= NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_nid.uCallbackMessage		= UM_TRAY;
	m_nid.hIcon = AfxGetApp()->LoadIcon(IDI_TMENU);
	wsprintf(m_nid.szTip, _T("%s (Connect)"), APP_NAME);

	::Shell_NotifyIcon(NIM_ADD, &m_nid);
}

LRESULT CStoryDlg::OnTray(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	//case WM_RBUTTONDOWN:
	//	SetCapture();
	//	break;
	case WM_RBUTTONUP:
		{
			CMenu menu, *pMenu = NULL;
			CPoint pt;

			menu.LoadMenu(IDR_TMENU);
			pMenu = menu.GetSubMenu(0);

			if(!m_bConnect || m_nTotalPos == 100)
				pMenu->EnableMenuItem(ID_MENU_CONNECT, MF_ENABLED);
			else
				pMenu->EnableMenuItem(ID_MENU_CONNECT, MF_DISABLED|MF_GRAYED);

			GetCursorPos(&pt);
			SetForegroundWindow();
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
				pt.x, pt.y, this);
			::PostMessage(m_hWnd, WM_NULL, 0, 0);

			//ReleaseCapture();
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			m_bShow = !IsWindowVisible();
			ShowWindow(m_bShow);
		}
		break;
	case WM_MOUSEFIRST:
		{
			static dwtime = 0;
			if(500 < GetCurrentTime() - dwtime && m_bDownloading)
			{
				dwtime = GetCurrentTime();
				wsprintf(m_nid.szTip, _T("%s (%d%%)"), APP_NAME, m_nTotalPos);
				::Shell_NotifyIcon(NIM_MODIFY, &m_nid);
			}
		}
		break;
	}

	return 0;
}
void CStoryDlg::OnCancel()
{
	m_bDownloading = FALSE;
	ShowWindow(SW_HIDE);
	DownloadEnd();	
	DestroyWindow();
	LOGMSG(_T("=====PrePatch End====="));
}

LRESULT CStoryDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch(message)
	{
	case WM_CLOSE_SESSION:
		DownloadEnd();
		break;
	case WM_CONNECT_SESSION:
		LOGMSG(_T("Finish Count Add"));
		SessionStart();
		break;
	case WM_PREPATCH_RRESET:
		LOGMSG(_T("Remote Reset"));
		OnMenuConnect();
		break;
	case WM_PREPATCH_RQUIT:
		LOGMSG(_T("Remote Quit"));
		OnCancel();
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CStoryDlg::ReadRegistry()
{
	BYTE bCurrent = FALSE;

	HKEY hKeyRet;	
	HKEY hKey = HKEY_LOCAL_MACHINE;
	
	CString strSubkey;
	strSubkey = m_strSubkey;
	if(strSubkey == _T(""))
	{
		strSubkey = REG_NAME;
		strSubkey += REG_COUNTRY;
	}

	strSubkey += _T("\\PB");	

	m_strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

	int err = RegOpenKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
	{
		RegCloseKey(hKeyRet);
		hKey = HKEY_CURRENT_USER;
	}

	err = RegOpenKey(hKey, m_strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
		return FALSE;

	BYTE	data[1024];
	DWORD   type;
	DWORD   cbdata =1024;

	//local path 
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, REG_VALUE_LOCAL, NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_SZ)
		return FALSE;
	
	m_strLocal = data;
	m_strDownload = m_strLocal + APP_DOWNFOLDER;

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

	RegCloseKey(hKeyRet);
	
#ifdef _LOGMSG
	CString str;
	str.Format("RegSubKey=%s\nLocalPath=%s\nIpaddr=%s %d", m_strRegSubKey, m_strLocal, m_strIP, m_wPort);
	LOGMSG(str);
#endif
	return TRUE;
}

void CStoryDlg::OnMenuConnect()
{
	if(m_bDownloading)
		return;

	LOGMSG(_T("Connect"));
	m_bFinish = FALSE;
	InfoFileRead();
	DisplayText("Connect");
	wsprintf(m_nid.szTip, _T("%s (Connect)"), APP_NAME);
	::Shell_NotifyIcon(NIM_MODIFY, &m_nid);
	SessionStart();
}

void CStoryDlg::OnConnect(int nErrorCode)
{	
	if(nErrorCode)
		DisConnected();
	else
	{
		m_bConnect = TRUE;
		if(m_bFinish)
			SendCT_PREPATCHCOMPLETE_REQ();
		else
			SendCT_PREPATCH_REQ();
	}
}
void CStoryDlg::OnClose(int nErrorCode)
{
#ifdef _LOGMSG
	CString str;
	str.Format(_T("Onclose ErrCode:%d"), nErrorCode);
	LOGMSG(str);
#endif
	if(nErrorCode)
		DisConnected();
}

void CStoryDlg::DisConnected()
{
	LOGMSG(_T("Disconnected"));
	m_bConnect = FALSE;
	DisplayText("Disconnected");
	wsprintf(m_nid.szTip, _T("%s (Disconnected)"), APP_NAME);
	::Shell_NotifyIcon(NIM_MODIFY, &m_nid);
}

DWORD CStoryDlg::OnReceive(CPacket * pPacket)
{
	switch(pPacket->GetID())
	{
		HP_RECEIVE(CT_PREPATCH_ACK)
	default:
		return 1;
	}
}

void CStoryDlg::SessionStart()
{
	LOGMSG("Connect");
	if(!m_session.Start(m_strIP, m_wPort))
	{
		m_bConnect = FALSE;
		ERRMSG(_T("Connect error"));
	}
}

void CStoryDlg::Say(CPacket * pPacket)
{
	m_session.Say(pPacket);
}

DWORD WINAPI CStoryDlg::_Download(LPVOID lpParam)
{
	return ((CStoryDlg*)lpParam)->Download();
}

DWORD CStoryDlg::Download()
{
	m_bDownloading = TRUE;

	CFile*			fileNew = NULL;
	CStdioFile*		remotefile = NULL;
	DWORD			dwRead=0;
	__int64			nTotalRead = 0;

	VPATCHFILE::iterator it = m_VPATCHFILE.begin();
	while(m_bDownloading)
	{
		LPPATCHFILE PathFile = (*it);

		if(it == m_VPATCHFILE.end())
		{
			m_bDownloading = FALSE;
			break;
		}
		CString strPath;
        //	다운로드 디렉토리 생성, 설정
		if(!PathFile->m_strPath.IsEmpty())
			strPath = m_strDownload + PathFile->m_strPath;
		else 
			strPath = m_strDownload;

		LOGMSG(PathFile->m_strName);
		if(!CreateDirectoryEx(strPath))
		{
			ERRMSG(_T("CreateDirectory error"));
			m_bDownloading = FALSE;
			break;
		}

		if(!SetCurrentDirectory(strPath))
		{
			ERRMSG(_T("SetCurrentDirectory error"));
			m_bDownloading = FALSE;
			break;
		}

		//	FTP URL 설정
		CString strURL = m_strFtpSvr + _T("/");
		if(!PathFile->m_strPath.IsEmpty())
		{
			strURL += PathFile->m_strPath + _T("/");
			strURL.Replace('\\','/');
		}

		strURL += PathFile->m_strName;

		TRY
		{
			CInternetSession mysession;
			remotefile = mysession.OpenURL(strURL,1,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD);
			if(!remotefile)
			{
				ERRMSG(_T("remotefile Error"));
				m_bDownloading = FALSE;
				break;
			}

			//	쓰기 파일 열기
			fileNew = new CFile;

			if(!fileNew)
			{
				ERRMSG(_T("File new error"));
				m_bDownloading = FALSE;
				break;
			}

			UINT flag = CFile::modeCreate | CFile::modeWrite;
			if(m_bInCom)
				flag = CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite;

			fileNew->Open(PathFile->m_strName, flag);

			if(fileNew->m_hFile == CFile::hFileNull)
			{
				ERRMSG(_T("fileopen error"));
				int error = GetLastError();
				m_bDownloading = FALSE;
				break;
			}
			else
			{
				dwRead = (DWORD)fileNew->SeekToEnd();					
				remotefile->Seek(dwRead, CFile::begin);
				if(dwRead)
					nTotalRead += dwRead;
			}
			
			remotefile->Seek(dwRead, CFile::begin);

			PATCHFILE info;
			info.m_dwBetaVer	= PathFile->m_dwBetaVer;
			info.m_strName		= PathFile->m_strName;
			info.m_strPath		= PathFile->m_strPath;
			info.m_dwSize		= 0;
			info.m_dwVersion	= 0;
			SetVersion(&info);

			m_dwLastVer = PathFile->m_dwBetaVer;

			DWORD	dwNumberOfBytesRead;
			BYTE	bSleepCount =0;
			char	lpBuffer[1024+1];

			while (dwNumberOfBytesRead = remotefile->Read(lpBuffer, 1024))
			{
#ifndef _DEBUG
				if(bSleepCount > 3)
				{
					Sleep(1);
					bSleepCount = 0;
				}
				bSleepCount++;
#endif
				if(!m_bDownloading)
					break;

				fileNew->Write(lpBuffer, dwNumberOfBytesRead);
				dwRead += dwNumberOfBytesRead;
				nTotalRead += dwNumberOfBytesRead;
				Progress(PathFile->m_strName, PathFile->m_dwSize, dwRead, nTotalRead);
			}

			remotefile->Close();
			delete remotefile;
			remotefile = NULL;
			dwRead = 0;

			fileNew->Close();
			delete fileNew;
			fileNew = NULL;

			if(TRUE == m_bDownloading)
			{
				CString strSource, strTarget;
				strTarget.Empty();
				strSource.Empty();

				strSource = m_strDownload;

				if(!PathFile->m_strPath.IsEmpty())
				{
					strSource += PathFile->m_strPath + _T("\\");
					strTarget = _T("\\")+PathFile->m_strPath;
				}
				strSource += PathFile->m_strName;
				SetVersion(PathFile);
				it++;

				if(m_bInCom)
					m_bInCom = FALSE;
			}
		}
		CATCH_ALL(error)
		{
			TCHAR   szCause[255];
			error->GetErrorMessage(szCause,254,NULL);
			LOGMSG(szCause);

			if(remotefile)
			{
				remotefile->Close();
				delete remotefile;
				remotefile = NULL;
			}
			m_bDownloading = FALSE;

			if(fileNew)
			{
				fileNew->Close();
				delete fileNew;
				fileNew = NULL;
			}
			
			nTotalRead -= dwRead;
			DisConnected();
			return 0;
		}
		END_CATCH_ALL;
	}

	if(fileNew)
	{
		fileNew->Close();
		delete fileNew;
		fileNew = NULL;
	}

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
void CStoryDlg::DownloadFinish()
{
	m_bFinish = TRUE;
	PostMessage(WM_CONNECT_SESSION);
}

void CStoryDlg::DownloadEnd()
{
	m_bDownloading = FALSE;
	
	if( m_hDownload )
	{
		WaitForSingleObject(m_hDownload,10000);
		CloseHandle(m_hDownload);
		m_hDownload = NULL;
	}
	for(VPATCHFILE::iterator it = m_VLocal.begin(); it != m_VLocal.end(); it++)
		delete *it;
	m_VLocal.clear();

	for(VPATCHFILE::iterator it = m_VPATCHFILE.begin(); it != m_VPATCHFILE.end(); it++)
		delete *it;
	m_VPATCHFILE.clear();

	if(m_session.m_hSocket != INVALID_SOCKET)
		m_session.End();
	
}
void CStoryDlg::SetVersion(LPPATCHFILE p_PatchFile)
{
	if(!p_PatchFile)
		return;

	for(DWORD i = 0; i < m_VLocal.size(); i++)
	{
		if(	m_VLocal[i]->m_strName == p_PatchFile->m_strName &&
			m_VLocal[i]->m_strPath == p_PatchFile->m_strPath)
		{
			m_VLocal[i]->m_dwBetaVer = p_PatchFile->m_dwBetaVer;
			m_VLocal[i]->m_dwSize	= p_PatchFile->m_dwSize;

			InfoFileWrite();
			return;
		}
	}
	LPPATCHFILE pPush = new PATCHFILE;
	pPush->m_dwBetaVer	= p_PatchFile->m_dwBetaVer;
	pPush->m_dwSize		= p_PatchFile->m_dwSize;
	pPush->m_dwVersion	= p_PatchFile->m_dwVersion;
	pPush->m_strName	= p_PatchFile->m_strName;
	pPush->m_strPath	= p_PatchFile->m_strPath;
	m_VLocal.push_back(pPush);
	InfoFileWrite();
}

BOOL CStoryDlg::CreateDirectoryEx(CString strPath)
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
void CStoryDlg::Progress(CString strName, DWORD dwSize, DWORD dwCurrent, __int64 nTotal)
{
	if(!m_bDownloading)
		return;

	int nPos = int(dwSize ? __int64(100) * dwCurrent / dwSize : 0);

	CString strPer;
	m_progressCurrent.SetPos( nPos );
	strPer.Format("%s (%d%%)", strName, nPos);
	DisplayText(strPer);

	m_nTotalPos = int(100 * nTotal / m_nPatchTotalSize);
	m_nTotalPos = (m_nTotalPos>99)?99:m_nTotalPos;
	m_progressTotal.SetPos( m_nTotalPos );

}

DWORD CStoryDlg::OnCT_PREPATCH_ACK(CPacket * pPacket)
{
	for(VPATCHFILE::iterator it = m_VPATCHFILE.begin(); it != m_VPATCHFILE.end(); it++)
		delete *it;
	m_VPATCHFILE.clear();

	m_nPatchTotalSize = 0;

	DWORD dwLoginAddr;
	WORD wLoginPort;
	WORD wCount;

	(*pPacket)
		>> m_strFtpSvr
		>> dwLoginAddr
		>> wLoginPort
		>> wCount;
#ifdef _LOGMSG
	CString str;
	str.Format("OnCT_PREPATCH_ACK FTP:%s Count:%d",m_strFtpSvr, wCount);
	LOGMSG(str);
#endif
	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = dwLoginAddr;
	m_strGameSvr = inet_ntoa(addr.sin_addr);
	m_wGamePort = wLoginPort;

	for(WORD i=0; i<wCount; i++)
	{
		LPPATCHFILE pFile = new PATCHFILE;

		(*pPacket)
			>> pFile->m_dwBetaVer
			>> pFile->m_strPath
			>> pFile->m_strName
			>> pFile->m_dwSize;
		pFile->m_dwVersion = 0;

		m_nPatchTotalSize += pFile->m_dwSize;
		m_VPATCHFILE.push_back(pFile);
	}

	SendCT_PATCHSTART_REQ();

	if(wCount)
	{
		DWORD dwThreadID;
		CreateDirectory( m_strDownload , NULL);
		m_hDownload = CreateThread(NULL, 0, _Download, (LPVOID)this, CREATE_SUSPENDED, &dwThreadID);
		if( !m_hDownload )
			return 1;
		ResumeThread(m_hDownload);
	}
	else
	{
		LOGMSG(_T("Complete"));
		DisplayText("Complete");
		m_progressCurrent.SetPos(100);
		m_progressTotal.SetPos(100);
		m_nTotalPos = 100;
		wsprintf(m_nid.szTip, _T("%s (Complete)"), APP_NAME);
		::Shell_NotifyIcon(NIM_MODIFY, &m_nid);
		
		DownloadEnd();
		return 1;
	}
	return 0;
}

void CStoryDlg::SendCT_PREPATCH_REQ()
{
	DWORD dwCount = (DWORD)m_VLocal.size();

	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_PREPATCH_REQ);

	DWORD dwMaxVer = 0;
	for(DWORD i = 0; i < dwCount; i++)
	{
		if(m_VLocal[i]->m_dwBetaVer > dwMaxVer && m_VLocal[i]->m_dwSize)
			dwMaxVer = m_VLocal[i]->m_dwBetaVer;
	}

	(*pMsg)
		<< dwMaxVer;

	Say(pMsg);
}

void CStoryDlg::SendCT_PATCHSTART_REQ()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_PATCHSTART_REQ);
	Say(pMsg);
}

void CStoryDlg::SendCT_PREPATCHCOMPLETE_REQ()
{	
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_PREPATCHCOMPLETE_REQ)
		<< m_dwLastVer;
	
	Say(pMsg);

	m_progressCurrent.SetPos(100);
	m_progressTotal.SetPos(100);
	m_nTotalPos = 100;
	LOGMSG(_T("Complete Msg"));
	DisplayText("Complete");
	wsprintf(m_nid.szTip, _T("%s (Complete)"), APP_NAME);
	::Shell_NotifyIcon(NIM_MODIFY, &m_nid);
	DownloadEnd();
}

void CStoryDlg::InfoFileWrite()
{
	char strFile[MAX_PATH];

	sprintf( strFile, "%sPrePatch.btp", m_strDownload);

	FILE* f = fopen( strFile ,"wt");
	if( !f ) return;

	VPATCHFILE::iterator it;
	for(it = m_VLocal.begin(); it != m_VLocal.end(); it++)
	{	
		fprintf(f,"%d\t%s\t%d\t%s\n", (*it)->m_dwBetaVer, (*it)->m_strName, (*it)->m_dwSize, (*it)->m_strPath);
	}
	fclose(f);
}

void CStoryDlg::InfoFileRead()
{	
	for(VPATCHFILE::iterator it = m_VLocal.begin(); it != m_VLocal.end(); it++)
		delete *it;
	m_VLocal.clear();

	char strFile[MAX_PATH];
	char strLine[MAX_PATH];

	sprintf( strFile, "%sPrePatch.btp", m_strDownload);

	FILE* f = fopen( strFile ,"rt");
	if( !f ) return;

	CString pszParam;

	while(fgets(strLine, MAX_PATH, f))
	{
		strLine[strlen(strLine)-1] = NULL;
		LPPATCHFILE PatchFile = new PATCHFILE;
		int nIndex = 0;

		PatchFile->m_dwBetaVer	= (DWORD)atoi(strtok(strLine,"\t"));
		PatchFile->m_strName	= strtok(NULL,"\t");
		PatchFile->m_dwSize		= (DWORD)atoi(strtok(NULL,"\t"));
		PatchFile->m_strPath	= strtok(NULL,"\t");
		PatchFile->m_dwVersion	= 0;

		CString strPath = m_strDownload + PatchFile->m_strPath +'\\'+ PatchFile->m_strName;
		
		if(FindPatchFile(strPath, PatchFile->m_dwSize))
			m_VLocal.push_back(PatchFile);
		else
			delete PatchFile;
	}
	fclose(f);
}

BYTE CStoryDlg::FindPatchFile(CString strPathName, DWORD dwSize)
{ 
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(strPathName, &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose (hFind);
		if(dwSize)
			return TRUE;
		else
			m_bInCom = TRUE;
	}
	return FALSE;
}


void CStoryDlg::LoadSkin()
{
	RECT r1;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &r1, 0);

	m_bFlag = m_bSkin.LoadBitmap(IDB_BITMAP_BASE);

	if( m_bFlag )
	{
		BITMAP bmp;
		m_bSkin.GetBitmap(&bmp);

		SetWindowPos( NULL, r1.right - bmp.bmWidth, r1.bottom - bmp.bmHeight, bmp.bmWidth, bmp.bmHeight, SWP_NOZORDER );
	}
	else
	{
		RECT r2;
		GetWindowRect(&r2);
		MoveWindow(r1.right - r2.right, r1.bottom - r2.bottom, r2.right, r2.bottom);
	}
	
	if(m_btnClose.LoadBitmaps(IDB_CLOSEBTN,IDB_CLOSEBTN,IDB_CLOSEBTN,IDB_CLOSEBTN))
		m_btnClose.SizeToContent();

}
BOOL CStoryDlg::OnEraseBkgnd(CDC* pDC)
{
	if( m_bFlag )
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
		
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);
		CFont Font;		
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(lf));
		lf.lfHeight = 12;		
		wsprintf(lf.lfFaceName, _T("돋움"));
		Font.CreateFontIndirect(&lf);
		pDC->SelectObject(&Font);
		pDC->DrawText(m_strDrawText,&m_rtText,DT_LEFT);

		ReleaseDC(pDC);
		return true;
	}
	else
		return CDialog::OnEraseBkgnd(pDC);
	
}

void CStoryDlg::ReadTextFile()
{
	m_strSubkey = _T("");
	CString strPath = m_szPatchFileName;
	int n = strPath.ReverseFind('\\');
	strPath = strPath.Left(n);
#ifdef _GOA
	strPath += _T("\\GOA.txt");
#else
	strPath += _T("\\4story.txt");
#endif
	CStdioFile file;

	if( file.Open( strPath, CFile::modeRead | CFile::typeText ) )
	{
		file.ReadString( m_strSubkey );
		file.Close();
	}
}