// HappyDoc.cpp : CHappyDoc 클래스의 구현
//

#include "stdafx.h"
#include "Happy.h"

#include "HappyDoc.h"
#include "MainFrm.h"
#include "HappyView.h"

#include <direct.h>
#include "Zip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

uLong filetime(TCHAR *f, uLong * dt)
/*f: name of file to get info on */
/* dt: dostime */
{
    int ret = 0;  
    {      
        FILETIME ftLocal;      
        HANDLE hFind;
        WIN32_FIND_DATA  ff32;      
        hFind = FindFirstFile(f,&ff32);
        if (hFind != INVALID_HANDLE_VALUE)      
        {
            FileTimeToLocalFileTime(&(ff32.ftLastWriteTime),&ftLocal);
            FileTimeToDosDateTime(&ftLocal,((LPWORD)dt)+1,((LPWORD)dt)+0);
            FindClose(hFind);        
            ret = 1;      
        }  
    }  
    return ret;
}

// CHappyDoc

IMPLEMENT_DYNCREATE(CHappyDoc, CDocument)

BEGIN_MESSAGE_MAP(CHappyDoc, CDocument)
	ON_COMMAND(ID_SERVICE_ALERT, OnServiceAlert)
	ON_BN_CLICKED(IDC_BUTTON_COMPARE, OnBnClickedButtonCompare)
	ON_BN_CLICKED(IDC_BUTTON_PATCHUP, OnBnClickedButtonPatchup)
	ON_BN_CLICKED(IDC_BUTTON_PREPATCHUP, OnBnClickedButtonPrepatchup)
END_MESSAGE_MAP()


// CHappyDoc 생성/소멸

CHappyDoc::CHappyDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_session.Close();
	m_session.SetOwner(this);

	m_bAlertOnFail = FALSE;
	m_dwSelectedService = 0;
	m_bViewAllService = 0;	
	m_dwUploadedSize = 0;
	m_dwPatchSize = 0;
	m_pPatchProgress = NULL;
	m_bPatchUploading = FALSE;
	m_bSameFileExist = FALSE;
	m_bServiceAutoStart = 0;
	m_bBtnAutoStart = 0;
	//////////////////////////////////
	// 현승룡 Graph
	m_bGraphCheck = 0; 
	m_curView = 0; 
	//////////////////////////////////

	m_bPatchType = 0;
	m_bLog = 0;
	m_bLogTime = 0;

	m_pMonSpawn = NULL;
	m_pdlgGMTool = NULL;

	// 06/02/01 박상연 Compare버튼과 PatchUpdate 버튼 초기 활성화 셋팅(비활성화)
	//-------------------------------------------------------------------
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	
	pFrm->SetPatchBarCompareVisible(FALSE);
	pFrm->SetPatchBarPatchUpdateVisible(FALSE);
	m_bErrorStatus = 0;			// 에러 상태값 초기 설정
	//-------------------------------------------------------------------

	GetCurrentDirectory(_MAX_PATH, m_szCurrentDirectory);

	CFile f;
	if(f.Open(FILE_NAME_CONNECT, CFile::modeRead))
	{
		CString strData;
		TCHAR id[50]={0};

		if(!f.Read(&m_dwIP,sizeof(DWORD)))
			m_dwIP=0;
		if(!f.Read(&m_dwPort,sizeof(DWORD)))
			m_dwPort=0;
		if(!f.Read(id, 50))
			m_strUserID = _T("");
		else
			m_strUserID = id;

		if(!f.Read(&m_dwPatchIP ,sizeof(DWORD)))
			m_dwPatchIP = 0;
		if(!f.Read(&m_dwPatchPort ,sizeof(DWORD)))
			m_dwPatchPort = 21;
		if(!f.Read(id, 50))
			m_strPatchUserID = _T("Administrator");
		else
			m_strPatchUserID = id;

		f.Close();
	}
	LoadPosListFromFile();

	m_strAdminWeb = OPWEB_STR;

	m_pdlgRet = new CRetDlg();
	m_pdlgRet->Create(IDD_RET);
	
	m_dlgPreVersion = new CPreVersion();
	m_dlgPreVersion->Create(IDD_VERSION);
}

CHappyDoc::~CHappyDoc()
{
	ClearData();
}

void CHappyDoc::ClearPatchFiles()
{
	while(m_vPatchFiles.size())
	{
		delete m_vPatchFiles.back();
		m_vPatchFiles.pop_back();
	}
}

void CHappyDoc::ClearSvrType()
{
	MAPSVRTYPE::iterator it;
	for(it=m_mapSvrType.begin(); it!=m_mapSvrType.end(); it++)
		delete (*it).second;
	m_mapSvrType.clear();
}

void CHappyDoc::ClearMachine()
{
	MAPMACHINE::iterator itM;
	for(itM=m_mapMachine.begin(); itM!=m_mapMachine.end(); itM++)
		delete (*itM).second;
	m_mapMachine.clear();
}
void CHappyDoc::ClearGroup()
{
	MAPGROUP::iterator itG;
	for(itG=m_mapGroup.begin(); itG!=m_mapGroup.end(); itG++)
		delete (*itG).second;
	m_mapGroup.clear();
}
void CHappyDoc::ClearService()
{
	MAPSERVICE::iterator itS;
	for(itS=m_mapService.begin(); itS!=m_mapService.end(); itS++)
		delete (*itS).second;
	m_mapService.clear();
}

void CHappyDoc::ClearPlatform()
{
	m_mapPLATFORM.clear();
}

void CHappyDoc::ClearServiceGraph()
{
	m_mapSERVICEGRAPH.clear();
}

void CHappyDoc::ClearData()
{
	SetCurrentDirectory(m_szCurrentDirectory);

	CFile f;
	if(f.Open(FILE_NAME_CONNECT,CFile::modeCreate | CFile::modeWrite))
	{
		f.Write(&m_dwIP, sizeof(DWORD));
		f.Write(&m_dwPort, sizeof(DWORD));
		f.Write(m_strUserID, 50);
		f.Write(&m_dwPatchIP, sizeof(DWORD));
		f.Write(&m_dwPatchPort, sizeof(DWORD));
		f.Write(m_strPatchUserID, 50);
		f.Close();
	}

	if(m_fileUpload.m_hFile != CFile::hFileNull)
		m_fileUpload.Close();

	DeletePatchProgress();

	ClearMachine();
	ClearGroup();
	ClearSvrType();
	ClearService();
	ClearPlatform();
	ClearPatchFiles();
	ClearServiceGraph();

	m_pMonSpawn = NULL;

	SavePosListToFile();
	ClearPosList();

	if(m_pdlgRet)
	{
		m_pdlgRet->DestroyWindow();
		delete m_pdlgRet;
		m_pdlgRet = NULL;
	}
	
	if(m_dlgPreVersion)
	{
		m_dlgPreVersion->DestroyWindow();
		delete m_dlgPreVersion;
		m_dlgPreVersion = NULL;
	}

	m_mapEventName.clear();
	ClearLuckyEvent();
	ClearTournament();
}

void CHappyDoc::DisconnectServer()
{
	m_session.Close();
	m_session.m_bValid = FALSE;

	if(m_bLog)
	{
		m_bLog = 0;
		CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
		pFrm->SetLogTimer(m_bLog, 0);
	}
	
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->SetPatchBarPatchUpdateVisible(FALSE);
	pFrm->SetPatchBarCompareVisible(FALSE);

	ClearService();
	ClearGroup();
	ClearSvrType();
	ClearMachine();
	ClearPlatform();
	ClearPatchFiles();
	ClearServiceGraph();	

	CServiceTree * pTree = ( CServiceTree * )pFrm->m_wndSplitter.GetView( pFrm->m_nTree );
	pTree->UpdateServiceTree(0xffffffff);
	// 현승룡 Machine
	CMachine * pMachine = (CMachine *)pFrm->m_wndSplitter.GetView(pFrm->m_nMachine);
	pMachine->ClearMachineTree();
	CPatchList * pList = (CPatchList *)pFrm->m_wndSplitter.GetView(pFrm->m_nListPatch);
	pList->UpdatePatchList();
	CServiceList * pSList = (CServiceList *)pFrm->m_wndSplitter.GetView(pFrm->m_nList);
	pSList->UpdateServiceList(0);

	UpdateAllViews(NULL);

	m_pdlgGMTool->SetShowChildDlgAll(FALSE);

	m_mapEventSvr.clear();
	
}
LPHAPPYSERVICE CHappyDoc::FindService(DWORD dwID)
{
	MAPSERVICE::iterator it = m_mapService.find(dwID);
	if(it!=m_mapService.end())
		return (*it).second;
	else
		return NULL;
}
BYTE CHappyDoc::IsGroup()
{
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CServiceTree * pTree = ( CServiceTree * )pFrm->m_wndSplitter.GetView( pFrm->m_nTree );
	return pTree->m_bIsGroup;
}
void CHappyDoc::GroupStart()
{
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CServiceTree * pTree = ( CServiceTree * )pFrm->m_wndSplitter.GetView( pFrm->m_nTree );

	HTREEITEM hGroup = (pTree->m_treeCtrl).GetParentItem(pTree->m_treeCtrl.GetSelectedItem());

	CString strWorld = (pTree->m_treeCtrl).GetItemText(hGroup);
	CString strGroup = (pTree->m_treeCtrl).GetItemText(pTree->m_treeCtrl.GetSelectedItem());

	MAPSERVICEGRAPH::iterator it; // 서비스 검색
	for(it = m_mapSERVICEGRAPH.begin(); it != m_mapSERVICEGRAPH.end(); it++)
	{
		if( !lstrcmp(strWorld, (*it).second.m_strWorld) && 
			!lstrcmp(strGroup, (*it).second.m_strGroup) )
			SendCT_SERVICECONTROL_REQ((*it).second.m_dwID, TRUE);
	}
}
void CHappyDoc::GroupStop()
{
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CServiceTree * pTree = ( CServiceTree * )pFrm->m_wndSplitter.GetView( pFrm->m_nTree );

	HTREEITEM hGroup = (pTree->m_treeCtrl).GetParentItem(pTree->m_treeCtrl.GetSelectedItem());

	CString strWorld = (pTree->m_treeCtrl).GetItemText(hGroup);
	CString strGroup = (pTree->m_treeCtrl).GetItemText(pTree->m_treeCtrl.GetSelectedItem());

	MAPSERVICEGRAPH::iterator it; // 서비스 검색
	for(it = m_mapSERVICEGRAPH.begin(); it != m_mapSERVICEGRAPH.end(); it++)
	{
		if( !lstrcmp(strWorld, (*it).second.m_strWorld) && 
			!lstrcmp(strGroup, (*it).second.m_strGroup) )
			SendCT_SERVICECONTROL_REQ((*it).second.m_dwID, FALSE);
	}
}
BOOL CHappyDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CServiceTree * pTree = (CServiceTree *)pFrm->m_wndSplitter.GetView(pFrm->m_nTree);
	CServiceList * pList = (CServiceList *)pFrm->m_wndSplitter.GetView(pFrm->m_nList);
	CAutoPatch * pPatch = (CAutoPatch *)pFrm->m_wndSplitter.GetView(pFrm->m_nPatch);
	CPatchList * pPatchList = (CPatchList *)pFrm->m_wndSplitter.GetView(pFrm->m_nListPatch);
	CServiceGraph * pGraph = (CServiceGraph *)pFrm->m_wndSplitter.GetView(pFrm->m_nGraph); // 현승룡 Graph
	AddView(pTree);
	AddView(pList);
	AddView(pPatch);
	AddView(pPatchList);
	AddView(pGraph); // 현승룡 Graph
	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}
CWnd * CHappyDoc::GetPatchListView()
{
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	return pFrm->m_wndSplitter.GetView(pFrm->m_nListPatch);
}
CWnd * CHappyDoc::GetServiceListView()
{
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	return pFrm->m_wndSplitter.GetView(pFrm->m_nList);
}
// CHappyDoc serialization

void CHappyDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

void CHappyDoc::OnConnect(int nErrorCode)
{
	if(nErrorCode)
	{
		AfxMessageBox(_T("Failed to connect to server")); // 서버 접속에 실패했습니다
		return;
	}
	SendCT_OPLOGIN_REQ();
}
void CHappyDoc::OnClose(int nErrorCode)
{
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();

	ClearService();
	ClearGroup();
	ClearSvrType();
	ClearMachine();
	ClearPlatform();
	ClearPatchFiles();
	ClearServiceGraph();

	CServiceTree * pTree = ( CServiceTree * )pFrm->m_wndSplitter.GetView( pFrm->m_nTree );
	pTree->UpdateServiceTree(0xffffffff);
	CPatchList * pList = (CPatchList *)pFrm->m_wndSplitter.GetView(pFrm->m_nListPatch);
	pList->UpdatePatchList();
	CServiceList * pSList = (CServiceList *)pFrm->m_wndSplitter.GetView(pFrm->m_nList);
	pSList->UpdateServiceList(0);

	pFrm->SetAllControlEnable(FALSE);
	CloseAllDialog();
	m_dwManagerID = 0;
    
	UpdateAllViews(NULL);

	AfxMessageBox(IDS_CONNECT_CLOSE);

}
void CHappyDoc::OnReceive(CPacket * pPacket)
{
	switch(pPacket->GetID())
	{
	HP_RECEIVE(CT_OPLOGIN_ACK)
	HP_RECEIVE(CT_SVRTYPELIST_ACK)
	HP_RECEIVE(CT_MACHINELIST_ACK)
	HP_RECEIVE(CT_GROUPLIST_ACK)
	HP_RECEIVE(CT_SERVICESTAT_ACK)
	HP_RECEIVE(CT_SERVICECHANGE_ACK)
	HP_RECEIVE(CT_SERVICEDATA_ACK)
	HP_RECEIVE(CT_SERVICEUPLOADSTART_ACK)
	HP_RECEIVE(CT_SERVICEUPLOAD_ACK)
	HP_RECEIVE(CT_SERVICEUPLOADEND_ACK)
	HP_RECEIVE(CT_UPDATEPATCH_ACK)
    HP_RECEIVE(CT_AUTHORITY_ACK)
	HP_RECEIVE(CT_PLATFORM_ACK)
	HP_RECEIVE(CT_MONSPAWNFIND_ACK)
	HP_RECEIVE(CT_USERPROTECTED_ACK)
	HP_RECEIVE(CT_SERVICEAUTOSTART_ACK)
	HP_RECEIVE(CT_CHATBAN_ACK)
	HP_RECEIVE(CT_ITEMFIND_ACK)
	HP_RECEIVE(CT_ITEMSTATE_ACK)
	HP_RECEIVE(CT_CHATBANLIST_ACK)
	HP_RECEIVE(CT_CASTLEINFO_ACK)
	HP_RECEIVE(CT_CASTLEGUILDCHG_ACK)
	HP_RECEIVE(CT_EVENTCHANGE_ACK)
	HP_RECEIVE(CT_EVENTLIST_ACK)
	HP_RECEIVE(CT_CASHITEMLIST_ACK)
	HP_RECEIVE(CT_EVENTQUARTERUPDATE_ACK)	
	HP_RECEIVE(CT_EVENTQUARTERLIST_ACK)
	HP_RECEIVE(CT_TOURNAMENTEVENT_ACK)
	HP_RECEIVE(CT_RPSGAMEDATA_ACK)
	HP_RECEIVE(CT_PREVERSIONTABLE_ACK)
	HP_RECEIVE(CT_CMGIFT_ACK)
	HP_RECEIVE(CT_CMGIFTLIST_ACK)
	default:
		break;
	}
}
void CHappyDoc::SessionStart(CString strIp, DWORD dwPort)
{
	if(!m_session.Start(strIp, dwPort))
		AfxMessageBox(_T("Failed to connect to server ")); //서버 접속에 실패했습니다
}
void CHappyDoc::Say(CPacket * pPacket)
{
	m_session.Say(pPacket);
}
// CHappyDoc 진단

#ifdef _DEBUG
void CHappyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHappyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CHappyDoc::OnServiceAlert()
{
	if(GetCurView() == 3) return;
	m_bAlertOnFail = !m_bAlertOnFail;
}
void CHappyDoc::NotifyUploadEnd(BYTE bRet)
{
	if(m_dlgUploadProgress.m_hWnd)
	{
		CEdit * pEdit = (CEdit *)m_dlgUploadProgress.GetDlgItem(IDC_EDIT_CURFILENAME);
		CButton * pButton = (CButton *)m_dlgUploadProgress.GetDlgItem(IDCANCEL);

		if(bRet)
			pEdit->SetWindowText(_T("The error occurred during the transfer")); //전송중 에러가 발생했습니다.
		else
			pEdit->SetWindowText(_T("The transfer is complete"));//전송이 완료되었습니다.

		pButton->SetWindowText(_T("OK"));//확인
	}
	if(m_fileUpload.m_hFile != CFile::hFileNull)
		m_fileUpload.Close();

	m_dwUploadedSize = 0;
}
void CHappyDoc::UpdateUploadProgress()
{
	CEdit * pEdit = (CEdit *)m_dlgUploadProgress.GetDlgItem(IDC_EDIT_CURFILENAME);

	CString strText;
	m_dlgUploadProgress.SetPos((100*m_dwUploadedSize)/m_dlgUploadProgress.m_dwFileSize);
	strText.Format("%s(%d/%d)",m_dlgUploadProgress.m_strFileName, m_dwUploadedSize, m_dlgUploadProgress.m_dwFileSize);
	pEdit->SetWindowText(strText);
}

BYTE CHappyDoc::ServiceUpload()
{
	TRY
	{
		BYTE data[SIZE_FILESEND] = {0};
		WORD wSize = m_fileUpload.Read(data, SIZE_FILESEND);
		m_dwUploadedSize += wSize;
		UpdateUploadProgress();

		if(m_dwUploadedSize == m_dlgUploadProgress.m_dwFileSize)
		{
			SendCT_SERVICEUPLOADEND_REQ(FALSE, data, wSize);
			m_dwUploadedSize = 0;
			return FALSE;
		}

		SendCT_SERVICEUPLOAD_REQ(data, wSize);
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
		m_dwUploadedSize = 0;
		return FALSE;
	}
	END_CATCH

	return TRUE;
}
BYTE CHappyDoc::IsSamePatchFile(LPPATCHFILE pFile)
{
	for(WORD i = 0; i < (WORD)m_vPatchFiles.size(); i++)
		if(m_vPatchFiles[i]->m_strOriFileName == pFile->m_strOriFileName )
			return TRUE;

	return FALSE;
}

void CHappyDoc::AddPatchFile(LPPATCHFILE pFile)
{
	if(IsSamePatchFile(pFile))
	{
		m_bSameFileExist = TRUE;
		CString str = _T("");
		str.Format(_T("%s file is exist "),pFile->m_strFileName);
		AfxMessageBox(str);		
	}
	m_vPatchFiles.push_back(pFile);	
}

void CHappyDoc::OnPatchUpload()
{
	if(m_bPatchUploading)
		return;
	
	m_bPatchUploading = TRUE;

	DWORD dwThreadID;

	DeletePatchProgress();

	m_pPatchProgress = new CDlgPatchProgress();
	m_pPatchProgress->Create(IDD_DIALOG_PATCHPROGRESS);
	m_pPatchProgress->SetWindowText(_T("Patching..."));//패치중
	CPatchList * pPatch = (CPatchList *)GetPatchListView();
	CRect rect, rect2;
	CPoint point;
	pPatch->GetClientRect(&rect);
	m_pPatchProgress->GetClientRect(&rect2);
	point = rect.CenterPoint();
	point.x -= rect2.Width()/2;
	point.y -= rect2.Height();
	pPatch->ClientToScreen(&point);
	rect2.MoveToXY(point);
	rect2.bottom += 25;
	m_pPatchProgress->MoveWindow(&rect2);
	m_pPatchProgress->ShowWindow(SW_SHOW);

	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->GetFtp(&m_strFtpAddr, &m_strFtpUser, &m_strFtpPasswd, &m_wFtpPort);


	m_hThreadPatchUpload = CreateThread(
		NULL,
		0,
		_ThreadPatchUpload,
		(LPVOID)this,
		0,
		&dwThreadID);

	if(!m_hThreadPatchUpload)
	{
		m_pPatchProgress->SetStatus(_ERR_THREADFAIL);		
	}
}
DWORD WINAPI CHappyDoc::_ThreadPatchUpload(LPVOID lpParam)
{
	return ((CHappyDoc *)lpParam)->ThreadPatchUpload();
}
DWORD CHappyDoc::ThreadPatchUpload()
{
	BYTE bError = FALSE;
	TCHAR buffer[1024];
	GetCurrentDirectory(1024, buffer);
	m_bErrorStatus = 0;				// 에러 상태 지정값

	m_pPatchProgress->SetStatus(1);

	CString strTemporary;
	if(!CreateTempDirectory(strTemporary))
	{
		bError = TRUE;
		m_bErrorStatus = _ERR_TEMPDIR;	// ERROR 상태 입력
		goto exit_patch_upload;	
	}

	//Compress patch files
	if(!ZipPatchFiles(strTemporary))
	{
		bError = TRUE;
		m_bErrorStatus = _ERR_ZIP;		// ERROR 상태 입력
		goto exit_patch_upload;
	}

	/*
	//Upload to a FTP Server

	m_pPatchProgress->SetStatus(_FTP_CONNECT);
	if(!UploadPatchFiles(strTemporary))
	{
		bError = TRUE;	
		goto exit_patch_upload;
	}
	*/

	// 현승룡 임시 UploadPatch
	if(!NewUploadPatchFiles(strTemporary))
	{
		bError = TRUE;	
		goto exit_patch_upload;
	}

	//Notify em to CTRLSVR
	if(m_bPatchType == 1)
		SendCT_UPDATEPATCH_REQ();
	else


	if( m_bErrorStatus == _ERR_DBWRITE )
	{
		bError = TRUE;
		goto exit_patch_upload;
	}

exit_patch_upload:

	/*
	if(!strTemporary.IsEmpty())
		DeleteDirContents(strTemporary);
		*/
	
	SetCurrentDirectory(buffer);

	/*
	if(!strTemporary.IsEmpty())
		RemoveDirectory(strTemporary);
		*/

	m_bPatchUploading = FALSE;

	if(bError)
	{
		int nError = GetLastError();
		CString strError;
		strError.Format(_T("Error Code %d"),nError);
		m_pPatchProgress->SetProgress(0);

		if( m_bErrorStatus != 0 )	// 상태같이 지정되었으면 상태값의 상태로 에러 메시지를 보여준다.
			m_pPatchProgress->SetStatus(m_bErrorStatus);
		else
			m_pPatchProgress->SetStatus(3);
		m_pPatchProgress->SetFileName(strError);
	}
	else
	{
		if( m_bErrorStatus != 0 )	// 상태같이 지정되었으면 상태값의 상태로 에러 메시지를 보여준다.
			m_pPatchProgress->SetStatus(m_bErrorStatus);
		else
			m_pPatchProgress->SetStatus(0);

		/*
		// 박상연 전송 완료후 FTP 주소 ACCOUNT 저장
		//-------------------------------------------------------
		DWORD dwIPTemp;
		dwIPTemp = DWORD( inet_addr( m_strFtpAddr ) );

		m_dwPatchIP = dwIPTemp;
		m_strPatchUserID = m_strFtpUser;
		m_dwPatchPort = DWORD(m_wFtpPort);
		//-------------------------------------------------------
		*/
	}

	return 0;
}

BYTE CHappyDoc::NewUploadPatchFiles(LPCTSTR strSourcePath)
{
	if(!SetCurrentDirectory(strSourcePath))
		return FALSE;

	for( DWORD i = 0; i < (DWORD)m_vPatchFiles.size(); i++)
	{
		LPPATCHFILE pPatch = m_vPatchFiles[i];
		if(!pPatch) continue;

		CString strZipFile;
		if(!pPatch->m_strPath.IsEmpty())
		{
			strZipFile = pPatch->m_strPath;
			strZipFile += _T("\\");

			strZipFile = strZipFile.Right( strZipFile.GetLength() -1);
		}
		strZipFile += pPatch->m_strZipFile;

		CFile fileLocal;
		if(fileLocal.Open(strZipFile, CFile::modeRead))
		{
			pPatch->m_dwZipSize = (DWORD)fileLocal.GetLength();
			fileLocal.Close();
		}
		else
			return FALSE;
	}

	return TRUE;
}

BOOL CHappyDoc::UploadPatchFiles(LPCTSTR strSourcePath)
{
	int nTotal = 0;
	for( DWORD i = 0; i < (DWORD)m_vPatchFiles.size(); i++)
		nTotal += m_vPatchFiles[i]->m_dwSize;
	
	if(m_strFtpAddr.IsEmpty() || m_strFtpUser.IsEmpty() || m_strFtpPasswd.IsEmpty() || !m_wFtpPort)
	{
		m_bErrorStatus = _ERR_EMPTY;					// ERROR 상태 입력
		return FALSE;
	}

	if(!SetCurrentDirectory(strSourcePath))
	{
		m_bErrorStatus = _ERR_CURRENTDIR;				// ERROR 상태 입력
		return FALSE;
	}

	BYTE bRet = TRUE;
	
	HINTERNET hInternet = InternetOpen("Happy",0,0,0,0);
	HINTERNET hConnect = InternetConnect(
		hInternet,
		m_strFtpAddr,
		m_wFtpPort,
		m_strFtpUser,
		m_strFtpPasswd,
		INTERNET_SERVICE_FTP,
		INTERNET_FLAG_PASSIVE,
		0);
	
	TRY
	{
		if(hConnect)
		{
			m_pPatchProgress->SetStatus(2);
			for( DWORD i = 0; i < (DWORD)m_vPatchFiles.size(); i++)
			{
				LPPATCHFILE pPatch = m_vPatchFiles[i];
				if(!pPatch) continue;
				
				DWORD dwLen = _MAX_PATH;
				TCHAR prev_dir[_MAX_PATH];

				FtpGetCurrentDirectory(hConnect, prev_dir, &dwLen);

				if(VerifyRemoteDirectory(hConnect, pPatch) &&
				   !UploadPatchFile(hConnect, pPatch, nTotal))
				{
					InternetCloseHandle(hConnect);
					InternetCloseHandle(hInternet);
					return FALSE;
				}
				
				FtpSetCurrentDirectory(hConnect, prev_dir);
			}
		}
		else
		{
			m_bErrorStatus = _ERR_ADDRORACCOUNT;		// ERROR 상태 입력
		}
	}
	CATCH(CInternetException, pEx)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);

		char szError[255];
		pEx->GetErrorMessage(szError, 255);
		m_pPatchProgress->SetFileName(szError);
		bRet = FALSE;
	}END_CATCH

	return bRet;
}
BYTE CHappyDoc::UploadPatchFile(HINTERNET hConnect, LPPATCHFILE pPatch, int& nSentTotal)
{
	CString strZipFile;
	if(!pPatch->m_strPath.IsEmpty())
	{
		strZipFile = pPatch->m_strPath;
		strZipFile += _T("\\");

		strZipFile = strZipFile.Right( strZipFile.GetLength() -1);
	}
	strZipFile += pPatch->m_strZipFile;

	m_pPatchProgress->SetFileName(strZipFile);

	HINTERNET hFile = FtpOpenFile(
		hConnect,
		pPatch->m_strZipFile,
		GENERIC_WRITE,
		FTP_TRANSFER_TYPE_BINARY,
		0);

	if(!hFile)
	{
		int n = GetLastError();
		return FALSE;
	}

	CFile *fileLocal = new CFile(strZipFile, CFile::modeRead);
	pPatch->m_dwZipSize = (DWORD)fileLocal->GetLength();

	if(fileLocal)
	{
		char buf[1024];
		DWORD dwSum = 0;
		DWORD dwWriteBytes = 0;
		DWORD lpdwNumberOfBytesWrite;
		while((dwWriteBytes = fileLocal->Read(buf, 1024))>0)
		{
			if(!InternetWriteFile(hFile,buf,dwWriteBytes,&lpdwNumberOfBytesWrite))
			{
				InternetCloseHandle(hFile);
				return FALSE;
			}
			dwSum += dwWriteBytes;
			m_pPatchProgress->SetProgress(BYTE((float)dwSum/pPatch->m_dwZipSize*100));
		}
	}
	
	if(fileLocal)
	{
		fileLocal->Close();
		delete fileLocal;
	}

	InternetCloseHandle(hFile);
	return TRUE;
}
/////////oo
//void CHappyDoc::OnPrePatchUpload()
//{
//	if(m_bPatchUploading)
//		return;
//	
//	m_bPatchUploading = TRUE;
//
//	DWORD dwThreadID;
//
//	DeletePatchProgress();
//
//	m_pPatchProgress = new CDlgPatchProgress();
//	m_pPatchProgress->Create(IDD_DIALOG_PATCHPROGRESS);
//	m_pPatchProgress->SetWindowText(_T("PrePatching..."));//패치중
//	CPatchList * pPatch = (CPatchList *)GetPatchListView();
//	CRect rect, rect2;
//	CPoint point;
//	pPatch->GetClientRect(&rect);
//	m_pPatchProgress->GetClientRect(&rect2);
//	point = rect.CenterPoint();
//	point.x -= rect2.Width()/2;
//	point.y -= rect2.Height();
//	pPatch->ClientToScreen(&point);
//	rect2.MoveToXY(point);
//	rect2.bottom += 25;
//	m_pPatchProgress->MoveWindow(&rect2);
//	m_pPatchProgress->ShowWindow(SW_SHOW);
//
//	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
//	pFrm->GetFtp(&m_strFtpAddr, &m_strFtpUser, &m_strFtpPasswd, &m_wFtpPort);
//
//
//	m_hThreadPatchUpload = CreateThread(
//		NULL,
//		0,
//		_ThreadPatchUpload,
//		(LPVOID)this,
//		0,
//		&dwThreadID);
//
//	if(!m_hThreadPatchUpload)
//	{
//		m_pPatchProgress->SetStatus(_ERR_THREADFAIL);		
//	}
//}

void CHappyDoc::DeleteDirContents(LPCTSTR strPath)
{//Delete all contents under the path
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

					DeleteDirContents(strFullPath);
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

void CHappyDoc::DeletePatchProgress()
{
	if(m_pPatchProgress)
	{
		m_pPatchProgress->DestroyWindow();
		delete m_pPatchProgress;
		m_pPatchProgress = NULL;
	}
}
BOOL CHappyDoc::CreateTempDirectory(CString &strFullPath)
{
	int nCount = 0;
	CString strTemp = PATCH_FOLDER;

	if(SetCurrentDirectory(m_strNextFolder))
	{
		BOOL bContinue = TRUE;
		while(bContinue)
		{
			strFullPath = m_strNextFolder;
			strFullPath += _T("\\");

			strFullPath += strTemp;

			if(CreateDirectory(strFullPath, NULL))
			{
				bContinue = FALSE;
			}
			else
			{
				strFullPath.Empty();
				if(ERROR_ALREADY_EXISTS!=GetLastError())
					return FALSE;

				nCount++;
				strTemp.Format(_T("temp_%d"), nCount);
			}
		}
	}

	return TRUE;
}
BOOL CHappyDoc::ZipPatchFiles(LPCTSTR strTargetPath)
{
	for( DWORD i = 0; i < (DWORD)m_vPatchFiles.size(); i++)
	{
		if(!ZipPatchFile(strTargetPath, m_vPatchFiles[i])) return FALSE;
	}
	return TRUE;
}
BOOL CHappyDoc::CreateDirectoryEx(CString strPath)
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
BOOL CHappyDoc::ZipPatchFile(LPCTSTR strTargetPath, LPPATCHFILE pPatchFile)
{
	int errclose = ZIP_OK;
	int err = 0;

	//Make the zip file name
	CString strZipFile;
	RetrieveZipFile(strTargetPath, pPatchFile);
	
	strZipFile = strTargetPath; 
	if( !pPatchFile->m_strPath.IsEmpty() )
	{
		strZipFile += pPatchFile->m_strPath;
		if( !CreateDirectoryEx( strZipFile ))
			return FALSE;
	}	
	
	strZipFile += _T("\\");
	strZipFile += pPatchFile->m_strZipFile;

	m_pPatchProgress->SetProgress(0);
	m_pPatchProgress->SetFileName(strZipFile);
	//Create write buffer
	void *buf = NULL;
	DWORD size_buf = WRITEBUFFERSIZE;

	buf = malloc(size_buf);
	if(!buf)
		return FALSE;	

	//ZIP
	zipFile zf = zipOpen(strZipFile, 0);

	DWORD size_read;
	DWORD total_read = 0;

    zip_fileinfo zi;
    int opt_compress_level=Z_BEST_COMPRESSION;
	
	zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour = 
            zi.tmz_date.tm_mday = zi.tmz_date.tm_min = zi.tmz_date.tm_year = 0;
    zi.dosDate = 0;
    zi.internal_fa = 0;
    zi.external_fa = 0;
    filetime(pPatchFile->m_strFullFile.LockBuffer(),&zi.dosDate);
	pPatchFile->m_strFullFile.ReleaseBuffer();

	err = zipOpenNewFileInZip(
		zf,pPatchFile->m_strFileName,&zi,
		NULL,0,NULL,0,NULL,(opt_compress_level != 0)?Z_DEFLATED:0,opt_compress_level);

	if(err == ZIP_OK)
	{
		FILE *fin = fopen(pPatchFile->m_strFullFile, _T("rb"));
		if(fin != NULL)
		{
			do
			{
				size_read = (int)fread(buf, 1, size_buf, fin);

				if(size_read < size_buf && feof(fin) == 0)
				{
					err = ZIP_ERRNO;
				}
				if(err == ZIP_OK && size_read > 0)
				{
					err = zipWriteInFileInZip(zf, buf, size_read);
					if(err < 0)
					{
						TRACE(_T("Write Zip Error\n"));
					}
				}

				total_read += size_read;
				m_pPatchProgress->SetProgress(BYTE((float)total_read/pPatchFile->m_dwSize*100));

			}while(err==ZIP_OK && size_read>0);
			
			fclose(fin);	
		}
	}
	
	errclose = zipClose(zf, NULL);
	free(buf);

	return (errclose == ZIP_OK && err == ZIP_OK);
}

BOOL CHappyDoc::VerifyRemoteDirectory(HINTERNET hConnect, LPPATCHFILE pPatch)
{
	if( !pPatch->m_strPath.IsEmpty()&&
		!FtpSetCurrentDirectory(hConnect, pPatch->m_strPath))
	{
		if(!CreateFtpDirectoryEx( hConnect, pPatch->m_strPath))
			return FALSE;
		else
			return FtpSetCurrentDirectory(hConnect, pPatch->m_strPath);
	}

	return TRUE;
}
BOOL CHappyDoc::CreateFtpDirectoryEx(HINTERNET hConnect, CString strPath)
{
	if( !hConnect || strPath.IsEmpty() ) return TRUE;

	if( !FtpCreateDirectory(hConnect, strPath ))
	{
		if( GetLastError() == 12003 )
		{
			int nPos = 0;
			int nNext = strPath.Find(_T("\\"), 0);
			while(nNext >=0 )
			{
				nPos = nNext;
				nNext = strPath.Find(_T("\\"), nPos+1 );
			}

			CString strCreate = strPath.Left( nPos );
			if( CreateFtpDirectoryEx( hConnect, strCreate) )
				return FtpCreateDirectory(hConnect, strPath );
		}
		return FALSE;
	}

	return TRUE;
}
void CHappyDoc::RetrieveZipFile(LPCTSTR strTargetPath, LPPATCHFILE pPatchFile)
{//retrieve a name for a new zip file
	CString strZipFile;
	TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szName[_MAX_FNAME], szExt[_MAX_EXT];

	BOOL bContinue = TRUE;
	int nCount = 0;

	strZipFile = strTargetPath;
	strZipFile += pPatchFile->m_strRelFile;

	_tsplitpath(strZipFile, szDrive, szDir, szName, szExt);
	do
	{
		_tmakepath(strZipFile.GetBufferSetLength(_MAX_PATH+32),szDrive,szDir,szName, _T("zip"));
		strZipFile.ReleaseBuffer();

		CFile file;
		CFileException ex;
		if(!file.Open(strZipFile, CFile::modeRead, &ex))
		{
			if( ex.m_lOsError == ERROR_FILE_NOT_FOUND ||
				ex.m_lOsError == ERROR_PATH_NOT_FOUND )
				bContinue = FALSE;
		}

		if(bContinue)
		{
			nCount++;
			sprintf(szName, "%s_%d", szName, nCount);
		}
	}while(bContinue);

	pPatchFile->m_strZipFile = szName;
	pPatchFile->m_strZipFile += _T(".");
	pPatchFile->m_strZipFile += _T("zip");
}


void CHappyDoc::OnBnClickedButtonCompare()
{
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CAutoPatch * pPatch = (CAutoPatch *)pFrm->m_wndSplitter.GetView(pFrm->m_nPatch);
	ClearPatchFiles();
	m_bSameFileExist = FALSE;

	if(pPatch->OnCompare())
	{
		CPatchList * pList = (CPatchList *)pFrm->m_wndSplitter.GetView(pFrm->m_nListPatch);
		pList->UpdatePatchList();
		if(m_bSameFileExist)
			return;

		CString strMsg;
		strMsg.Format(_T("Abstracted %d file /%10.2f Kb"), m_vPatchFiles.size(), m_dwPatchSize/1000.0);//%d 파일/%10.2f Kb가 추출 되었습니다.
		AfxMessageBox(strMsg);
				
		pFrm->SetPatchBarPatchUpdateVisible(TRUE); // 06/02/01 박상연 PatchUpdate 버튼 활성화 셋팅

		//CopyFileNotZip();
	}
}

void CHappyDoc::CopyFileNotZip()
{
	CString strGamePatchPath;
	CString strNewFullPath;
	CString strNewFullFile;
	
	DWORD i;
	BOOL bSuccess = FALSE;

	if(!(DWORD)m_vPatchFiles.size() )
		return;

	if(!CreateTempDirectory(strGamePatchPath))
	{
		AfxMessageBox(_T("GamePatch Folder Create F7ailed "));
		return;
	}

	for(i = 0; i < m_vPatchFiles.size(); i++)
	{	
		LPPATCHFILE pFile = m_vPatchFiles[i];

		strNewFullPath = strGamePatchPath + pFile->m_strPath;

		if(!CreateDirectoryEx(strNewFullPath))
		{
			AfxMessageBox(_T("Sub Folder Failed"));
			return;
		}

		strNewFullFile = strNewFullPath + _T("\\") + pFile->m_strFileName;

		bSuccess = CopyFile(pFile->m_strFullFile, strNewFullFile,TRUE);
		if(bSuccess == FALSE)
		{
			AfxMessageBox(_T("Copy File Error"));
			return;
		}
	}
	
	AfxMessageBox(_T("Complete file copy"));
}

void CHappyDoc::OnBnClickedButtonPatchup()
{
	m_bPatchType = 1;	// 패치
	if(m_vPatchFiles.size())
		OnPatchUpload();
	else
		AfxMessageBox("There is no patch file"); //패치할 파일이 없습니다.
}

void CHappyDoc::OnBnClickedButtonPrepatchup()
{	
	m_bPatchType = 2; // pre패치
	if(m_vPatchFiles.size())
		OnPatchUpload();
	else
		m_dlgPreVersion->Init();
}

////////////////////////////////////////
// 현승룡 Graph
BYTE CHappyDoc::GetCurView()
{	
	return m_curView;
}
void CHappyDoc::SetCurView(BYTE bCur)
{
	m_curView = bCur;
}

void CHappyDoc::SaveLogData()
{
	/*
	if(!m_mapService.size())
		return;

	CTime time = CTime::GetCurrentTime();
	CString strFileName;
	strFileName.Format(_T("%d_%d_%d.txt"), time.GetYear(), time.GetMonth(), time.GetDay());
	
	CFile f;
	if(f.Open(strFileName,CFile::modeReadWrite))
		f.SeekToEnd();
	else 
		f.Open(strFileName,CFile::modeCreate | CFile::modeWrite);

	CString strData;
	strData = _T("\r\n");
	f.Write(strData, strData.GetLength());

	strData.Format(_T("<%d-%d-%d(%d:%d)>\r\n"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
	f.Write(strData, strData.GetLength());

	strData.Format(_T("----------------------------- SERVICE INFORMATION -----------------------------\r\n"));
	f.Write(strData, strData.GetLength());

	MAPSERVICE::iterator it;
	for(it=m_mapService.begin(); it!=m_mapService.end(); it++)
	{
		CString strStopTime;
		if((*it).second->m_nStopTime)
		{
			CTime t((*it).second->m_nStopTime);
			strStopTime.Format("%d-%d-%d %d:%d",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
		}
		else
			strStopTime = _T("0-0-0 0:0");

		CString strPickTime;
		if((*it).second->m_nPickTime)
		{
			CTime t((*it).second->m_nPickTime);
			strPickTime.Format("%d-%d-%d %d:%d",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
		}
		else
			strPickTime = _T("0-0-0 0:0");

		strData.Format(_T("%s(%s) : %d %d %d %d %d %s %s\r\n"), 
			(*it).second->m_strName, 
			(*it).second->m_pSvrType->m_strName,
			(*it).second->m_dwCurSession, 
			(*it).second->m_dwCurUser,
            (*it).second->m_dwMaxUser,
			(*it).second->m_dwPing,
			(*it).second->m_dwStopCount, 
			strStopTime,
			strPickTime );

		f.Write(strData, strData.GetLength());
	}

	strData.Format(_T("\r\n----------------------------- MACHINE INFORMATION -----------------------------\r\n"));
	f.Write(strData, strData.GetLength());

	MAPPLATFORM::iterator itP;
	for(itP=m_mapPLATFORM.begin(); itP!=m_mapPLATFORM.end(); itP++)
	{
		VPLATFORM vPlatform = (*itP).second;

		MAPMACHINE::iterator find = m_mapMachine.find((*itP).first);
		if(find==m_mapMachine.end() || vPlatform.size() == 0 )
			continue;

		strData.Format(_T("%s : %d %d %.2f\r\n"), 
			(*find).second->m_strName, 
			vPlatform[vPlatform.size()-1].m_dwCPU, 
			vPlatform[vPlatform.size()-1].m_dwMEM, 
			vPlatform[vPlatform.size()-1].m_fNET );

		f.Write(strData, strData.GetLength());
	}
	
	f.Close();
	*/
	//----------------------------------------------------

	if(!m_mapService.size())
		return;
	

	CTime time = CTime::GetCurrentTime();
	CString strFileName;
	strFileName.Format(_T("%d_%d_%d.txt"), time.GetYear(), time.GetMonth(), time.GetDay());
	
	CFile f;
	if(f.Open(strFileName,CFile::modeReadWrite))
		f.SeekToEnd();
	else 
		f.Open(strFileName,CFile::modeCreate | CFile::modeWrite);

	CString strData;
	strData = _T("\r\n");
	f.Write(strData, strData.GetLength());

	strData.Format(_T("< %d-%d-%d %d:%d [%s] >\r\n"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(),m_strIP);
	f.Write(strData, strData.GetLength());

	strData.Format(_T("============================= SERVICE INFORMATION =============================\r\n"));
	f.Write(strData, strData.GetLength());

	CString strMax;
	INT dwMaxString = 0;
	MAPSERVICE::iterator itT;
	for(itT = m_mapService.begin(); itT != m_mapService.end(); itT++)
	{
		strMax = (*itT).second->m_strName;
		if( dwMaxString < strMax.GetLength() )
			dwMaxString = strMax.GetLength();
	}

	CString strTab= _T("\t");
	BYTE bTab = 7;
	BYTE bMaxTabCnt = BYTE((dwMaxString+1) / bTab);
	for(BYTE b = 1; b < bMaxTabCnt; b++)
		strTab += strTab;	
	
	strData.Format(_T("Service %s Cur_Session \t Cur_User \t Max_User \t Active_User \t StopCount \t LatestTime \t\t PickTime \r\n"),strTab);
	f.Write(strData, strData.GetLength());
	
		
	BYTE bTabCnt = 0;
	MAPGROUP::iterator itG;
	MAPSERVICE::iterator itS;
	for(itG = m_mapGroup.begin(); itG != m_mapGroup.end(); itG++)
	{		
		strData.Format(_T("\r\n[ Group %d ]\r\n"),(*itG).second->m_bID);
		f.Write(strData, strData.GetLength());

        for(itS = m_mapService.begin(); itS != m_mapService.end(); itS++)
		{			
			LPHAPPYSERVICE pS = (*itS).second;
			if( pS->m_pGroup->m_bID != (*itG).second->m_bID || pS->m_dwStatus != DCSVC_STAT_RUNNING || pS->m_pSvrType->m_bType == SVRGRP_LOG )
				continue;
			
			strTab = _T("\t");
			CString strLStart,strPTime;
			CTime sTime(pS->m_nStopTime);
			CTime pTime(pS->m_nPickTime);

			bTabCnt = BYTE((pS->m_strName.GetLength()+1) / bTab);
			while(bTabCnt < bMaxTabCnt && bTabCnt > 0)
			{
				strTab += strTab;
				bTabCnt++;
			}

			strLStart.Format("%d-%d-%d %d:%d",sTime.GetYear(),sTime.GetMonth(),sTime.GetDay(),sTime.GetHour(),sTime.GetMinute());
			strPTime.Format("%d-%d-%d %d:%d",sTime.GetYear(),sTime.GetMonth(),sTime.GetDay(),sTime.GetHour(),sTime.GetMinute());
            
			strData.Format(_T(" %s %s %d \t\t %d \t\t %d \t\t %d \t\t %d \t\t %s \t %s \r\n"),
				pS->m_strName,strTab,pS->m_dwCurSession,pS->m_dwCurUser,pS->m_dwMaxUser,pS->m_dwActiveUser,pS->m_dwStopCount, strLStart,strPTime);
			f.Write(strData, strData.GetLength());
		}
	}

	strData.Format(_T("\r\n----------------------------- MACHINE INFORMATION -----------------------------\r\n"));
	f.Write(strData, strData.GetLength());

	MAPPLATFORM::iterator itP;
	for(itP=m_mapPLATFORM.begin(); itP!=m_mapPLATFORM.end(); itP++)
	{
		VPLATFORM vPlatform = (*itP).second;

		MAPMACHINE::iterator find = m_mapMachine.find((*itP).first);
		if(find==m_mapMachine.end() || vPlatform.size() == 0 )
			continue;

		strData.Format(_T("%s : %d %d %.2f\r\n"), 
			(*find).second->m_strName, 
			vPlatform[vPlatform.size()-1].m_dwCPU, 
			vPlatform[vPlatform.size()-1].m_dwMEM, 
			vPlatform[vPlatform.size()-1].m_fNET );

		f.Write(strData, strData.GetLength());
	}
	
	f.Close();
}
BYTE CHappyDoc::CheckBeep()
{
	MAPSERVICE::iterator it;
	for(it=m_mapService.begin(); it!=m_mapService.end(); it++)
	{
		LPHAPPYSERVICE pService = (*it).second;
		if( pService->m_dwStatus != DCSVC_STAT_RUNNING &&
			pService->m_dwStatus != DCSVC_STAT_CANNOTCONTROL)
			return TRUE;
	}

	return FALSE;
}
////////////////////////////////////////


void CHappyDoc::SavePos(USERPOS& _UserPos)
{
	LPUSERPOS pUSER = new USERPOS;

	pUSER->bChannelNum = _UserPos.bChannelNum;
	pUSER->wMapID = _UserPos.wMapID;;

	pUSER->PosX = _UserPos.PosX;
	pUSER->PosY = _UserPos.PosY;
	pUSER->PosZ = _UserPos.PosZ;
	pUSER->strName = _UserPos.strName;

	//m_pdlgGMTool->m_dlgUserMove->m_pdlgPosList->InsertItemToListCtr( (int)m_setUSERPOS.size(), pUSER);
	//m_pdlgGMTool->m_dlgUserMove->m_pdlgPosList->m_listctrPos.UpdateData();
	m_setUSERPOS.insert(pUSER);	

}

void CHappyDoc::LoadPosListFromFile()
{
	int index = 0;
	const char* szPosFileName = "UserPosList.txt";
	FILE* fp = NULL;
	fp = fopen(szPosFileName,"rt");
	if(fp == NULL)
		return;

	ClearPosList();	

	char strLine[1024];	
	fgets(strLine, 1024, fp);	//fprintf(fp,"NAME\tChannel\tMapID\tPosX\t\tPosY\t\tPosZ\n");
	memset(strLine,0,1024);

	while(fgets(strLine, 1024, fp))
	{
		int iStart = 0;
		CString strTemp = strLine;

		LPUSERPOS pUSER = new USERPOS();
		pUSER->strName		= strTemp.Tokenize("\t", iStart);
		pUSER->bChannelNum	= (BYTE)atoi(strTemp.Tokenize("\t", iStart));
		pUSER->wMapID		= (WORD)atoi(strTemp.Tokenize("\t", iStart));
		pUSER->PosX			= (float)atof(strTemp.Tokenize("\t", iStart));
		pUSER->PosY			= (float)atof(strTemp.Tokenize("\t", iStart));
		pUSER->PosZ			= (float)atof(strTemp.Tokenize("\t", iStart));

		//if( fscanf(fp,"%s\t%d\t%d\t%f\t%f\t%f\n",pUSER->POSNAME,&pUSER->ChannelNum,&pUSER->MapID,&pUSER->PosX ,&pUSER->PosY,&pUSER->PosZ) == EOF)
		//{
		//	delete pUSER;
		//	break;
		//}

		m_setUSERPOS.insert(pUSER);
		memset(strLine,0,1024);
	}

	fclose(fp);
}

void CHappyDoc::ClearPosList()
{
	SETUSERPOS::iterator it;	
	for(it = m_setUSERPOS.begin(); it != m_setUSERPOS.end(); it++)
		delete *it;
	m_setUSERPOS.clear();
}

void CHappyDoc::SavePosListToFile()
{
	if(m_setUSERPOS.empty())
		return;

	FILE* fp = NULL;
	const char* szPosFileName = "UserPosList.txt";
	fp = fopen(szPosFileName,"wt");
	
	if( fp == NULL)
		return;

	fprintf(fp,"NAME\tChannel\tMapID\tPosX\t\tPosY\t\tPosZ\n");

	SETUSERPOS::iterator it;
	for( it = m_setUSERPOS.begin() ; it != m_setUSERPOS.end(); it++)
	{
		LPUSERPOS pUser = (LPUSERPOS)*it;
		fprintf(fp,"%s\t%d\t%d\t%f\t%f\t%f\n",pUser->strName,pUser->bChannelNum,pUser->wMapID,pUser->PosX,pUser->PosY,pUser->PosZ);
	}

	fclose(fp);
}

void CHappyDoc::CloseAllDialog()
{
	// View 얻기
	CHappyView* pView = (CHappyView*)(((CMainFrame *)AfxGetMainWnd())->GetActiveFrame())->GetActiveView();
	if(pView && pView->m_dlgGMTool)
	{
		pView->m_dlgGMTool->SetShowChildDlgAll(FALSE);
		pView->m_dlgGMTool->ShowWindow(SW_HIDE);
		pView->m_dlgChatBanList->ShowWindow(SW_HIDE);
	}
}

void CHappyDoc::ShowRetDlg(BYTE bShow,BYTE bRet,CString str)
{
	if(!m_pdlgRet)
		return;
	if(str.GetLength() > 0 )
	{
		m_pdlgRet->SetMsg(str);
		m_pdlgRet->ShowWindow(bShow);
	}
	else
	{
		CString strRet,strName,strMin;
		if(bRet)
		{
			// View 얻기
			CHappyView* pView = (CHappyView*)(((CMainFrame *)AfxGetMainWnd())->GetActiveFrame())->GetActiveView();
			if(pView && pView->m_dlgGMTool)
			{
				pView->m_dlgGMTool->m_dlgChatBan->GetDlgItemText(IDC_CHATBAN_CHAR,strName);
				pView->m_dlgGMTool->m_dlgChatBan->GetDlgItemText(IDC_CHATBAN_MIN,strMin);
				strRet.Format(_T("%s was banned for %s minutes"),strName,strMin);
			}
		}
		else
			strRet.Format(_T("Enter the correct name"));	
		
		m_pdlgRet->SetMsg(strRet);
		m_pdlgRet->ShowWindow(bShow);
	}
}


CString CHappyDoc::GetSvrName( BYTE bGroup,BYTE bSvrType,BYTE bSvrID )
{
	CString str = _T("");
	DWORD dwID = MAKESVRID(bGroup, bSvrType, bSvrID);
	MAPSERVICE::iterator itS = m_mapService.find(dwID);
	if( itS != m_mapService.end())
		str = (*itS).second->m_strName;

	return str;
}

void CHappyDoc::InitEventTitle()
{
	m_mapEventName.clear();

	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_EXPADD,_T("Experience")));
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_CASHSALE,_T("Cash Item Sale")));
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_ITEMDROP,_T("Item Drop")));
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_ITEMMAGICDROP,_T("Magic Item Drop")));
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_REFINE,_T("Refine")));
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_TRANS,_T("Refine Transfer")));
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_ITEMUPGRADE,_T("Item Upgrade")));
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_MAGICUPGRADE,_T("Magic Upgrade")));
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_RAREMAGICUPGRADE,_T("Rare Magic Upgrade")));
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_GAMBLEOPTION,_T("Gamble Option")));
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_MONSPAWN,_T("Monster Spawn")));
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_MONREGEN,_T("Monster Regen")));
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_MONEYDROP,_T("Money Drop")));	
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_LOTTERY,_T("Item Lottery")));	
	m_mapEventName.insert(MAPBYTESTRING::value_type(EVENT_GIFTTIME,_T("Gift Time")));
}

void CHappyDoc::ClearLuckyEvent()
{
	m_mapLuckEvent.clear();
}
void CHappyDoc::AddLuckEvent(LUCKYEVENT stLuckyEvent)
{
	m_mapLuckEvent.insert(MAPLUCKYEVENT::value_type(stLuckyEvent.m_wID,stLuckyEvent));
}

void CHappyDoc::DelLuckyEvent(WORD wID)
{
	MAPLUCKYEVENT::iterator itE = m_mapLuckEvent.find(wID);
	if( itE != m_mapLuckEvent.end())
		m_mapLuckEvent.erase(itE);
}

LPLUCKYEVENT CHappyDoc::GetLuckyEvent(WORD wID)
{
	MAPLUCKYEVENT::iterator itE = m_mapLuckEvent.find(wID);
	if( itE != m_mapLuckEvent.end())
		return &((*itE).second);

	return NULL;
}

void CHappyDoc::AddTournament( LPTOURNAMENT pTNT )
{		
	m_mapTournament.insert(MAPTOURNAMENT::value_type(pTNT->m_wID, pTNT));
}

void CHappyDoc::DelTournament( WORD wID )
{
	MAPTOURNAMENT::iterator it = m_mapTournament.find(wID);
	if(it != m_mapTournament.end())
	{
		delete (*it).second;
		m_mapTournament.erase(it);
	}
}

void CHappyDoc::ClearTournament()
{
	MAPTOURNAMENT::iterator it;
	for( it = m_mapTournament.begin(); it != m_mapTournament.end(); it++)
	{
		LPTOURNAMENT pTNT = (*it).second;
		delete (*it).second;
	}
	m_mapTournament.clear();	
}

LPTOURNAMENT CHappyDoc::GetTournament( WORD wID )
{
	MAPTOURNAMENT::iterator it = m_mapTournament.find(wID);
	if(it != m_mapTournament.end())
	{
		return (*it).second;		
	}

	return NULL;
}

LPTOURNAMENT CHappyDoc::GetTournament( CString strTitle )
{
	MAPTOURNAMENT::iterator it;
	for(it = m_mapTournament.begin(); it != m_mapTournament.end(); it++)
	{
		if( (*it).second->m_strTitle == strTitle)
			return (*it).second;
	}

	return NULL;
}

void CHappyDoc::ChangeXListCtrl( DWORD dwCtrlID,int nRow, int nCol, int nCtrlType )
{	
    //nCtrlType: 1- EDIT. 2- COMBO. 3- DATA. 4- Check

	// View 얻기
	CHappyView* pView = (CHappyView*)(((CMainFrame *)AfxGetMainWnd())->GetActiveFrame())->GetActiveView();
	if(!pView || !pView->m_dlgGMTool)
		return;
	
	switch(dwCtrlID)
	{
	case IDC_LC_TEVENT:
		{
			pView->m_dlgGMTool->m_dlgTournament->UpdateEventList(nRow,nCol);
		}
		break;
	case IDC_LC_REWARD:
		{
			pView->m_dlgGMTool->m_dlgTournament->UpdateReward(nRow,nCol);
		}
		break;
	case IDC_LC_TSTEP:
		{
			pView->m_dlgGMTool->m_dlgTournament->UpdateStep(nRow,nCol);
		}
		break;
	case IDC_LC_RPSLIST:
		{
			pView->m_dlgGMTool->m_dlgRPSGame->CheckValue(nRow,nCol);
		}
		break;
	default:
		break;
	}

}

int CHappyDoc::GetClassIndex( DWORD dwClass )
{
	if(dwClass == 0 || dwClass > 63 )
		return -1;
	if(dwClass == 63)
		return 6;

	int nIndex = 0;	
	while( dwClass != 1 && dwClass > 0)
	{
		dwClass = dwClass >> 1;
		nIndex++;
	}

	if(nIndex > 5)
		return -1;
	
	return nIndex;
}