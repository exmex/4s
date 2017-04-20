// TMonSpawnInspectorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "TMonSpawnInspector.h"
#include "TMonSpawnInspectorDlg.h"
#include "TMonSpawnInspectorCnPDlg.h"
#include "TMonSpawnInspectorMAP.h"
#include "TMonSpawnInspectorObjBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTMonSpawnInspectorDlg 대화 상자

CTMonSpawnInspectorDlg::CTMonSpawnInspectorDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTMonSpawnInspectorDlg::IDD, pParent)
, m_strCLIENTPATH(_T(""))
, m_strDESTPATH(_T(""))
, m_bStopWorking(FALSE)
, m_bReviseHeight(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CTMonSpawnInspectorDlg::~CTMonSpawnInspectorDlg()
{
}

void CTMonSpawnInspectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLIENT, m_btnClientPath);
	DDX_Control(pDX, IDC_BUTTON_DEST, m_btnDestPath);
	DDX_Text(pDX, IDC_EDIT_CLIENT, m_strCLIENTPATH);
	DDX_Text(pDX, IDC_EDIT_DEST, m_strDESTPATH);
	DDX_Control(pDX, IDC_COMBO_INPUT_TYPE, m_cmbInputMethod);
	DDX_Control(pDX, IDC_WORKING_TEXT, m_stcWorkingText);
	DDX_Control(pDX, IDC_WORKING_PROGRESS, m_ctrlWorkingProgress);
	DDX_Check(pDX, IDC_CHECK_REVISE_HEIGHT, m_bReviseHeight);
}

BEGIN_MESSAGE_MAP(CTMonSpawnInspectorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CLIENT, OnBnClickedButtonClient)
	ON_BN_CLICKED(IDC_BUTTON_DEST, OnBnClickedButtonDest)
	ON_BN_CLICKED(IDC_BUTTON_INSPECTION, OnBnClickedButtonInspection)
END_MESSAGE_MAP()


// CTMonSpawnInspectorDlg 메시지 처리기

BOOL CTMonSpawnInspectorDlg::OnInitDialog()
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

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_btnClientPath.SetIcon(hIcon);
	m_btnDestPath.SetIcon(hIcon);

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CTMonSpawnInspectorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CTMonSpawnInspectorDlg::OnPaint() 
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
HCURSOR CTMonSpawnInspectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTMonSpawnInspectorDlg::OnBnClickedButtonClient()
{
	CTShell dlg;

	TCHAR szCurPath[MAX_PATH];
	::GetCurrentDirectory( MAX_PATH, szCurPath );

	dlg.m_strSelDir = szCurPath;
	dlg.m_strTitle = _T("Client Folder");

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strCLIENTPATH = dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CTMonSpawnInspectorDlg::OnBnClickedButtonDest()
{
	CFileDialog dlg(
		FALSE,
		_T("*.csv"),
		"result.csv",
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Report File(CSV) (*.csv)|*.csv||"));

	TCHAR szCurPath[MAX_PATH];
	::GetCurrentDirectory( MAX_PATH, szCurPath );

	dlg.m_ofn.lpstrInitialDir = szCurPath;
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strDESTPATH = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CTMonSpawnInspectorDlg::OnBnClickedButtonInspection()
{
	UpdateData();

	// 초기화
	m_vTSPAWNDATA.clear();
	m_vTRESULT.clear();
	m_vTSORTEDMAP.clear();

	SetProgressText("");
	SetProgressClear();

	// 1. 클라이언트 경로 점검
	if( m_strCLIENTPATH == "" )
	{
		AfxMessageBox( "클라이언트 경로가 유효하지 않습니다.", MB_OK );
		return ;
	}

	// 2. 결과파일 경로 점검
	if( m_strDESTPATH == "" )
	{
		AfxMessageBox( "출력 파일이 유효하지 않습니다.", MB_OK );
		return ;
	}

	// 3. 입력방식 선택
	INT nInputMethod = m_cmbInputMethod.GetCurSel();
	// -1 : None
	// 0 : 파일
	// 1 : Copy & Paste

	switch( nInputMethod )
	{
	case -1:
		{
			AfxMessageBox( "입력방식을 선택하십시오.", MB_OK );
			return ;
		}

		// 3.1 입력방식 : 파일
	case 0:
		{
			TCHAR szCurPath[MAX_PATH];
			GetCurrentDirectory( MAX_PATH, szCurPath );
			BOOL bRet = OnInputFromFile();
			SetCurrentDirectory( szCurPath );

			if( FALSE == bRet )
				return ;
			else
				break;
		}

		// 3.2 입력방식 : Copy & Paste
	case 1:
		{
			if( FALSE == OnInputFromEditBox() )
				return ;
			else
				break;
		}
	}

	// 4. 컨트롤 표시를 작업모드로 변경
	SetControlWorkingMode(TRUE);

	if( TRUE == ProcInspectData() ) // 5. 데이터 검사
		ProcWriteReport(); // 6. 완료 - 리포트 작성

	// 7. 컨트롤 표시를 초기상태로 변경
	SetControlWorkingMode(FALSE);
}

BOOL CTMonSpawnInspectorDlg::AddSpawnData(
	LPCTSTR szLine)
{
	if( szLine[0] == '\0' ||
		szLine[0] == '\t' ||
		szLine[0] == '\r' || 
		szLine[0] == '\n' )
	{
		TSPAWNDATA SPAWNDATA;
		SPAWNDATA.bEmptyLine = TRUE;

		m_vTSPAWNDATA.push_back( SPAWNDATA );
		return TRUE;
	}

	DWORD wID = 0;
	DWORD wGroup = 0;
	DWORD wLocalID = 0;
	DWORD wMapID = 0;
	FLOAT fPosX = 0.0f;
	FLOAT fPosY = 0.0f;
	FLOAT fPosZ = 0.0f;
	DWORD wDir = 0;
	DWORD bCount = 0;
	DWORD bRange = 0;
	DWORD bArea = 0;
	DWORD bProb = 0;
	DWORD bLink = 0;
	DWORD dwDelay = 0;
	DWORD bRoamType = 0;
	DWORD bCountry = 0;
	DWORD dwRegion = 0;
	DWORD bEvent = 0;
	DWORD wPartyID = 0;

	sscanf( szLine, "%d %d %d %d %f %f %f %d %d %d %d %d %d %d %d %d %d %d %d",
		&wID,
		&wGroup,
		&wLocalID,
		&wMapID,
		&fPosX,
		&fPosY,
		&fPosZ,
		&wDir,
		&bCount,
		&bRange,
		&bArea,
		&bProb,
		&bLink,
		&dwDelay,
		&bRoamType,
		&bCountry,
		&dwRegion,
		&bEvent,
		&wPartyID );

	TSPAWNDATA SPAWNDATA;

	SPAWNDATA.wSpawnID = (WORD)wID;
	SPAWNDATA.wMapID = (WORD)wMapID;
	SPAWNDATA.fPosX = fPosX;
	SPAWNDATA.fPosY = fPosY;
	SPAWNDATA.fPosZ = fPosZ;
	SPAWNDATA.bRange = (BYTE)bRange;
	SPAWNDATA.bEmptyLine = FALSE;

	m_vTSPAWNDATA.push_back( SPAWNDATA );

	return TRUE;
}

VOID CTMonSpawnInspectorDlg::SetControlWorkingMode(
	BOOL bWorkingMode)
{
	CWnd* pWnd = NULL;

	if( bWorkingMode ) // 작업 모드 일 경우
	{
		// 초기화면 컨트롤들은 Hide
		pWnd = GetDlgItem( IDC_STATIC1 );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE );

		pWnd = GetDlgItem( IDC_STATIC2 );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE );

		pWnd = GetDlgItem( IDC_STATIC3 );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE );

		pWnd = GetDlgItem( IDC_COMBO_INPUT_TYPE );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE ); 

		pWnd = GetDlgItem( IDC_EDIT_CLIENT );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE ); 

		pWnd = GetDlgItem( IDC_EDIT_DEST );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE ); 

		pWnd = GetDlgItem( IDC_WORKING_PROGRESS );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE ); 

		pWnd = GetDlgItem( IDC_BUTTON_INSPECTION );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE ); 

		pWnd = GetDlgItem( IDOK );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE );

		pWnd = GetDlgItem( IDC_BUTTON_CLIENT );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE );

		pWnd = GetDlgItem( IDC_BUTTON_DEST );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE );
		
		pWnd = GetDlgItem( IDC_CHECK_REVISE_HEIGHT );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE );

		// 작업 중에 표시할 컨트롤들은 Show
		pWnd = GetDlgItem( IDC_WORKING_TEXT );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW );

		pWnd = GetDlgItem( IDC_WORKING_PROGRESS );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW );
	}
	else // 작업 모드가 아닐 경우
	{
		CWnd* pWnd = NULL;

		// 초기화면 컨트롤들은 Show
		pWnd = GetDlgItem( IDC_STATIC1 );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW );

		pWnd = GetDlgItem( IDC_STATIC2 );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW );

		pWnd = GetDlgItem( IDC_STATIC3 );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW );

		pWnd = GetDlgItem( IDC_COMBO_INPUT_TYPE );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW ); 

		pWnd = GetDlgItem( IDC_EDIT_CLIENT );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW ); 

		pWnd = GetDlgItem( IDC_EDIT_DEST );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW ); 

		pWnd = GetDlgItem( IDC_WORKING_PROGRESS );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW ); 

		pWnd = GetDlgItem( IDC_BUTTON_INSPECTION );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW ); 

		pWnd = GetDlgItem( IDOK );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW );

		pWnd = GetDlgItem( IDC_BUTTON_CLIENT );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW );

		pWnd = GetDlgItem( IDC_BUTTON_DEST );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW );
		
		pWnd = GetDlgItem( IDC_CHECK_REVISE_HEIGHT );
		if( pWnd ) pWnd->ShowWindow( SW_SHOW );

		// 작업 중에 표시할 컨트롤들은 Hide
		pWnd = GetDlgItem( IDC_WORKING_TEXT );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE );

		pWnd = GetDlgItem( IDC_WORKING_PROGRESS );
		if( pWnd ) pWnd->ShowWindow( SW_HIDE );
	}
}

void CTMonSpawnInspectorDlg::OnBnClickedWorkingStop()
{
	m_bStopWorking = TRUE;
}

BOOL CTMonSpawnInspectorDlg::OnInputFromFile()
{
	// 데이타 파일 선택하기
	CFileDialog dlg(
		FALSE,
		_T("*.*"),
		"입력할 데이타 파일을 선택하여 주십시오",
		OFN_HIDEREADONLY,
		_T("Text File (*.txt)|*.txt||"));

	CString strPath("");

	TCHAR szCurPath[MAX_PATH];
	::GetCurrentDirectory( MAX_PATH, szCurPath );

	dlg.m_ofn.lpstrInitialDir = szCurPath;
	if( dlg.DoModal() == IDOK )
	{
		// 데이타 파일에서 데이타 가져오기
		strPath = dlg.GetPathName();

		CStdioFile file;
		if( !file.Open( strPath, CStdioFile::modeRead ) )
		{
			AfxMessageBox( "파일이 유효하지 않습니다", MB_OK );
			return FALSE;
		}

		SetControlWorkingMode(TRUE);
		SetProgressText( "파일에서 데이타를 읽는 중 입니다." );
		SetProgressRange( 0, (INT)file.GetLength() );

		INT nLineCount = 0;
		INT nBegin = 0;
		DWORD dwWorkCount = 0;
		while(1)
		{
			CString strLine;
			if( FALSE == file.ReadString(strLine) )
				break;

			if( FALSE == AddSpawnData( strLine ) )
			{
				CString strError;
				strError.Format( "%d 라인에서 문제가 생겼습니다.", ++nLineCount );
				AfxMessageBox( strError, MB_OK );
				file.Close();
				SetControlWorkingMode(FALSE);
				return FALSE;
			}

			nBegin += strLine.GetLength();
			m_ctrlWorkingProgress.SetPos( nBegin );

			if( !PeekAndPump( dwWorkCount, 20 ) )	 // 20라인에 한번씩 메시지 펌핑
			{
				file.Close();
				return FALSE;
			}

			if( m_bStopWorking )
			{
				file.Close();
				SetControlWorkingMode(FALSE);
				m_bStopWorking = FALSE;
				return FALSE;
			}
		}

		file.Close();
		return TRUE;
	}
	return FALSE; // 파일선택창에서 취소버튼을 눌렀을 경우.
}

BOOL CTMonSpawnInspectorDlg::OnInputFromEditBox()
{
	CString strSource;

	CTMonSpawnInspectorCnPDlg dlg;
	dlg.m_pDest = &strSource;

	if( dlg.DoModal() == IDOK )
	{
		if( 0 != strSource.GetLength() )
		{
			SetControlWorkingMode(TRUE);

			m_stcWorkingText.SetWindowText("데이타를 읽는 중 입니다.");
			m_ctrlWorkingProgress.SetRange32( 0, strSource.GetLength() );

			INT nBegin = 0;
			INT nLineCount = 0;
			DWORD dwWorkCount = 0;
			while(1)
			{
				INT nEnd = strSource.Find( _T('\n'), nBegin );

				if( nEnd == -1 )
					break;

				CString strLine = strSource.Mid( nBegin, nEnd-nBegin );

				if( FALSE == AddSpawnData( strLine ) )
				{
					CString strError;
					strError.Format( "%d 라인에서 문제가 생겼습니다.", ++nLineCount );
					AfxMessageBox( strError, MB_OK );
					SetControlWorkingMode(FALSE);
					return FALSE;
				}

				nBegin = nEnd+1;
				m_ctrlWorkingProgress.SetPos( nBegin );

				if( !PeekAndPump( dwWorkCount, 100 ) ) // 100번에 한번꼴로 메시지 펌핑
					return FALSE;

				if( m_bStopWorking )
				{
					SetControlWorkingMode(FALSE);
					m_bStopWorking = FALSE;
					return FALSE;
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}

INT CTMonSpawnInspectorDlg::ProcSortDataByMapID()
{
	DWORD dwProgress = 0;
	SetProgressText( "스폰데이타를 맵별로 소팅" );
	SetProgressRange( 0, (INT)m_vTSPAWNDATA.size() );

	for( VTSPAWNDATA::iterator it = m_vTSPAWNDATA.begin();
		it != m_vTSPAWNDATA.end();
		++it )
	{
		if( it->bEmptyLine == FALSE )
			InsertSpawnByMapID( it->wMapID, (*it) );

		SetProgressPos( ++dwProgress );
	}

	return TRUE;
}

INT CTMonSpawnInspectorDlg::ProcInspectData()
{
	// 1. 리소스 초기화
	CTMonSpawnInspectorMAP* pMAP = new CTMonSpawnInspectorMAP;

	if( FALSE == pMAP->InitResource( "TClient" ) )
	{
		AfxMessageBox( "게임리소스 초기화 실패", MB_OK );
		return FALSE;
	}

	// 2. 스폰데이타를 맵아이디별로 정렬
	if( FALSE ==  ProcSortDataByMapID() )
	{
		pMAP->TermResource();
		delete pMAP;
		return FALSE;
	}

	// 3. (분류된) 맵별로 돌면서 스폰데이타를 처리하자.
	VTSORTEDMAP::iterator MapItr, MapEnd;
	MapItr = m_vTSORTEDMAP.begin();
	MapEnd = m_vTSORTEDMAP.end();

	for(; MapItr != MapEnd ; ++MapItr )
	{
		// 월드 인덱스로 맵이 어느 파일에 저장되어있는지 파일인덱스를 가져온다.
		LPTMAPINFO pMAPINFO = pMAP->GetMapINFO( MapItr->wMapID );

		CString strMSG;
		strMSG.Format( "%d 맵 로딩",
			MapItr->wMapID );

		SetProgressText( strMSG );
		SetProgressClear();

		// 맵로딩 - 파일인덱스로 맵을 연다.
		CTMonSpawnInspectorHUGEMAP* HUGEMAP = 	(CTMonSpawnInspectorHUGEMAP*)pMAP->m_Res.LockMAP( pMAPINFO->m_dwMapID );

		SetProgressRange(0,1);
		SetProgressPos(1);

		if( HUGEMAP == NULL )
		{
			CString strError;
			strError.Format( "%d맵을 열지 못 했습니다.",
				MapItr->wMapID );
			AfxMessageBox( strError, MB_OK );
			continue;
		}

		VTSORTEDUNIT vUnits;			// 맵의 모든 스폰데이터를 유닛별로 분류하여 저장시킬 곳.

		// 4. 맵안에서도 유닛별로 스폰데이타를 분류한다.
		{
			DWORD dwProgress = 0;

			CString strMSG;
			strMSG.Format( "%d 맵에 있는 스폰데이타를 유닛별로 분류",
				MapItr->wMapID );

			SetProgressText( strMSG );
			SetProgressClear();

			LTSPAWNDATA::iterator SpawnItr, SpawnEnd;
			SpawnItr = MapItr->lSpawnData.begin();
			SpawnEnd = MapItr->lSpawnData.end();

			for(; SpawnItr != SpawnEnd ; ++SpawnItr )
			{
				// 위치값으로 맵안의 어느 유닛에 존재하는지 알애낸다.
				INT nUnitIndex = HUGEMAP->GetUNITID( D3DXVECTOR3( SpawnItr->fPosX, SpawnItr->fPosY, SpawnItr->fPosZ ) );

				// 그리고 그 알아낸 유닛인덱스로 맞는 곳에 스폰데이타를 넣어주자.
				LTSPAWNDATA& lSpawnData = FindSpawnArrayByUnitIndex( vUnits, nUnitIndex, HUGEMAP->m_nUnitX, HUGEMAP->m_nUnitZ );
				lSpawnData.push_back( *SpawnItr );

				SetProgressPos( ++dwProgress );
			}

			if( dwProgress != MapItr->lSpawnData.size() )
			{
				CString strError;
				strError.Format( "%d맵에서 총 스폰데이타 갯수와 유닛별로 분류한 스폰데이터의 갯수가 같지 않습니다.",
					MapItr->wMapID );
				AfxMessageBox( strError, MB_OK );
			}

			MapItr->lSpawnData.clear();
		}

		// 5. 이제 유닛별로 스폰데이타가 정렬됐고 유닛별로 로딩하면서 스폰데이타를 개별 검사한다.
		{
			VTSORTEDUNIT::iterator UnitItr, UnitEnd;
			UnitItr = vUnits.begin();
			UnitEnd = vUnits.end();

			for(; UnitItr != UnitEnd ; ++UnitItr )
			{
				INT nUnitID = UnitItr->nUnitIndex;

				SetProgressText( "" );
				SetProgressClear();

				// 실제 로딩할 수 있는 유닛인지 검사한다.
				if( FALSE == HUGEMAP->CheckAvailableLoadUNIT( nUnitID ) )
				{
					LTSPAWNDATA::iterator SpawnItr, SpawnEnd;
					SpawnItr = UnitItr->lSpawnData.begin();
					SpawnEnd = UnitItr->lSpawnData.end();

					// 유닛을 로딩할 수 없으면 그 유닛에 있는 모든 스폰데이타는 오류로 간주.
					for(; SpawnItr != SpawnEnd ; ++SpawnItr )
					{
						TRESULT RESULT;					
						RESULT.SpawnData = *SpawnItr;
						RESULT.bResult = TRESULT_INVALID_UNIT;
						m_vTRESULT.push_back( RESULT );
					}
					continue;
				}

				strMSG.Format( "%d맵의 %d,%d 유닛 로딩", MapItr->wMapID, UnitItr->nUnitX, UnitItr->nUnitZ );
				SetProgressText( strMSG );
				SetProgressClear();

				// 그 유닛인덱스로 실질적으로 유닛을 로드한다.
				HUGEMAP->LoadUNIT( nUnitID );

				strMSG.Format( "%d맵의 %d,%d유닛 초기화", MapItr->wMapID, UnitItr->nUnitX, UnitItr->nUnitZ );
				SetProgressText( strMSG );
				SetProgressClear();

				// 로드한 그 유닛안에 (인스턴스데이타)오브젝트들의 초기화를 수행한다.
				pMAP->LoadMAPOBJ( HUGEMAP );

				// 이제 유닛안에 들어있는 (인스턴스데이타)오브젝트들이 객체로 생성되어
				// pMAP->m_vCOLL에 저장됐다.

				LTSPAWNDATA::iterator SpawnItr, SpawnEnd;
				SpawnItr = UnitItr->lSpawnData.begin();
				SpawnEnd = UnitItr->lSpawnData.end();

				strMSG.Format( "%d맵의 %d,%d유닛의 스폰 검사", MapItr->wMapID, UnitItr->nUnitX, UnitItr->nUnitZ );

				DWORD dwProgress = 0;
				SetProgressText( strMSG );
				SetProgressRange( 0, (INT)UnitItr->lSpawnData.size() );

				DWORD dwWorkCount = 0;

				// 이제 이 유닛에 속한 스폰데이타들을 모두 검사해본다.
				// (pMAP->m_vCOLL에 있는 객체들과 충돌하는지 검사)
				for(; SpawnItr != SpawnEnd ; ++SpawnItr )
				{
					D3DXVECTOR3 vPos( SpawnItr->fPosX, SpawnItr->fPosY, SpawnItr->fPosZ );
					FLOAT fRange = SpawnItr->bRange;

					BOOL bValidPoint = FALSE;
					BOOL bValidHeight = FALSE;

					FLOAT fResultHeight = 0.0f;
					FLOAT fResultLandHeight = 0.0f;

					bValidPoint = pMAP->IsValidPoint( vPos, fRange );					// 이 위치는 주변에 오브젝트가 없어서 유효한가?
					bValidHeight = pMAP->IsValidHeight( HUGEMAP, vPos, fRange, &fResultHeight, &fResultLandHeight );	// 이 위치의 높이값은 유효한가?

					if( !bValidPoint || !bValidHeight ) // 충돌된다면 기록!
					{
						TRESULT RESULT;

						RESULT.SpawnData = *SpawnItr;
						RESULT.bResult = 0;
						RESULT.fResultHeight = fResultHeight;
						RESULT.fResultLandHeight = fResultLandHeight;

						if( !bValidPoint )
						{
							RESULT.bResult |= TRESULT_INVALID_POINT;
						}

						if( !bValidHeight )
						{
							RESULT.bResult |= TRESULT_INVALID_HEIGHT;
						}

						m_vTRESULT.push_back( RESULT );
					}

					SetProgressPos( ++dwProgress );

				} // end for(; SpawnItr != SpawnEnd ; ++SpawnItr )

				// 이제 이 유닛에 존재하는 모든 스폰데이터는 검사했으므로
				// 할당했던 자원을 해제하고 다음 유닛으로 넘어간다.
				pMAP->ClearCOLL();					// 위의 pMAP->LoadMAPOBJ에서 할당한 자원 해제
				HUGEMAP->UnloadUNIT();

			} // end for(; UnitItr != UnitEnd ; ++UnitItr )
		}

		// 위의 pMAP->m_Res.LockMAP에서 할당한 자원 해제
		HUGEMAP->Release();
		for( int i=0 ; i < 9 ; ++i )
			HUGEMAP->m_UNIT[ i ].Release();

		// 마무리
		delete HUGEMAP;
	}

	// 마무리
	pMAP->TermResource();
	delete pMAP;
	return TRUE;
}

INT CTMonSpawnInspectorDlg::ProcWriteReport()
{
	SetProgressText( "레포트 작성" );
	SetProgressClear();

	if( 0 == m_vTRESULT.size() )
	{
		AfxMessageBox( "모든 몬스터의 스폰 위치가 유효합니다!", MB_OK );
		SetProgressRange(0,1);
		SetProgressPos(1);
		return TRUE;
	}

	FILE* fp = fopen( m_strDESTPATH, "wt" );
	if( !fp )
	{
		CString strError;
		strError.Format( "%s에 파일을 생성 할 수 없습니다.", m_strDESTPATH );
		AfxMessageBox( strError, MB_OK );
		return FALSE;
	}

	fprintf( fp, "%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
		"스폰 지역",
		"출현 맵",
		"POS X",
		"POS Y",
		"POS Z",
		"스폰 반경",
		"상태",
		"최종높이",
		"땅높이");

	DWORD dwProgress = 0;
	SetProgressRange( 0, (INT)m_vTRESULT.size() );

	for( VTRESULT::iterator itRESULT = m_vTRESULT.begin();
		itRESULT != m_vTRESULT.end();
		++itRESULT )
	{
		CString strError;

		if( (itRESULT->bResult & TRESULT_INVALID_POINT) == TRESULT_INVALID_POINT )
			strError += "오브젝트 충돌;";

		if( (itRESULT->bResult & TRESULT_INVALID_HEIGHT) == TRESULT_INVALID_HEIGHT )
			strError += "잘못된 높이;";

		if( (itRESULT->bResult & TRESULT_INVALID_UNIT) == TRESULT_INVALID_UNIT )
			strError += "없는 유닛;";

		// 작업
		fprintf( fp, "%d,%d,%f,%f,%f,%d,%s,%f,%f\n",
			itRESULT->SpawnData.wSpawnID,
			itRESULT->SpawnData.wMapID,
			itRESULT->SpawnData.fPosX,
			itRESULT->SpawnData.fPosY,
			itRESULT->SpawnData.fPosZ,
			itRESULT->SpawnData.bRange,
			(LPCTSTR)strError,
			itRESULT->fResultHeight,
			itRESULT->fResultLandHeight);

		SetProgressPos(++dwProgress);
	}

	fclose(fp);

	if( m_bReviseHeight )
		ProcWriteReviseHeightReport();

	// 완료
	CString strError;
	strError.Format( "총 %d개의 스폰위치에서 에러가 검출되었습니다.\n%s 파일을 확인하십시오",
		(DWORD)m_vTRESULT.size(),
		m_strDESTPATH );

	AfxMessageBox( strError, MB_OK );

	return TRUE;
}

class Cond_Same_ID
{
public:
	WORD m_wSpawnId;

	bool operator()(const TSPAWNDATA &spawnData)
	{
		if( m_wSpawnId == spawnData.wSpawnID )
			return true;
		else
			return false;
	};

	Cond_Same_ID( WORD wSpawnId )
		: m_wSpawnId(wSpawnId)
	{}
};

INT CTMonSpawnInspectorDlg::ProcWriteReviseHeightReport()
{
	INT nCount = 0;
	
	SetProgressClear();
	SetProgressText( "보정된 높이값 레포트 작성" );

	SetProgressRange( 0, (INT)m_vTRESULT.size() );
	
	// 1. 잘못된 애들을 찾아서 원본에 넣어주자.
	for( VTRESULT::iterator itRESULT = m_vTRESULT.begin();
		itRESULT != m_vTRESULT.end();
		++itRESULT )
	{
		// 이걸로 m_vTSPAWNDATA 에서 찾는다.

		// 그냥 선형검색하자.
		VTSPAWNDATA::iterator it = 	std::find_if(
													m_vTSPAWNDATA.begin(),
													m_vTSPAWNDATA.end(),
													Cond_Same_ID(itRESULT->SpawnData.wSpawnID) );

		if( it != m_vTSPAWNDATA.end() )
			it->fPosY = itRESULT->fResultHeight; //fResultHeight <- 요게 옳바른 높이값
		else
			AfxMessageBox( "있을 수 없는 일");

		SetProgressPos( nCount++ );
	}
	
	nCount = 0;

	SetProgressClear();
	SetProgressRange( 0, (INT)m_vTSPAWNDATA.size() );

	// 2. (높이값이 변경된)원본을 그대로 출력. (CnP를 할 수 있게끔)
	CString strDESTPATH = m_strDESTPATH + "_height.txt";

	FILE* fp = fopen( strDESTPATH, "wt" );
	if( !fp )
	{
		CString strError;
		strError.Format( "%s에 파일을 생성 할 수 없습니다.", strDESTPATH );
		AfxMessageBox( strError, MB_OK );
		return FALSE;
	}
	
	for( VTSPAWNDATA::iterator itSPAWNDATA = m_vTSPAWNDATA.begin();
		itSPAWNDATA != m_vTSPAWNDATA.end();
		++itSPAWNDATA )
	{
		if( itSPAWNDATA->bEmptyLine )
			fprintf( fp, "\n");
		else
			fprintf( fp, "%.2f\n", itSPAWNDATA->fPosY);

		SetProgressPos( nCount++ );
	}

	fclose(fp);

	return TRUE;
}

VOID CTMonSpawnInspectorDlg::InsertSpawnByMapID(
	WORD wMapID,
	TSPAWNDATA& data)
{
	// 맵아이디로 맵리스트에서 찾아서 해당 스폰데이터를 저장.
	for(VTSORTEDMAP::iterator itTSORTEDMAP = m_vTSORTEDMAP.begin();
		itTSORTEDMAP != m_vTSORTEDMAP.end();
		++itTSORTEDMAP)
	{
		if( itTSORTEDMAP->wMapID == wMapID )
		{
			itTSORTEDMAP->lSpawnData.push_back( data );
			return ;
		}
	}

	// 집어넣고자하는 스폰데이타의 맵이 리스트에 없으면 추가.
	m_vTSORTEDMAP.push_back( TSORTEDMAP(wMapID) );
	itTSORTEDMAP = --m_vTSORTEDMAP.end();
	itTSORTEDMAP->lSpawnData.push_back( data );
	return ;
}


LTSPAWNDATA& CTMonSpawnInspectorDlg::FindSpawnArrayByUnitIndex(
	VTSORTEDUNIT& vUnits,
	INT nUnitIndex,
	INT nUnitX,
	INT nUnitZ )
{
	for( VTSORTEDUNIT::iterator itSORTEDUNIT = vUnits.begin();
		itSORTEDUNIT != vUnits.end();
		++itSORTEDUNIT )
	{
		if( itSORTEDUNIT->nUnitIndex == nUnitIndex &&
			itSORTEDUNIT->nUnitX == nUnitX &&
			itSORTEDUNIT->nUnitZ == nUnitZ )
			return itSORTEDUNIT->lSpawnData;
	}

	vUnits.push_back( TSORTEDUNIT(nUnitIndex,nUnitX,nUnitZ) );
	itSORTEDUNIT = --vUnits.end();
	return itSORTEDUNIT->lSpawnData;
}