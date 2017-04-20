// TBuilderDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "TBuilder.h"
#include "TBuilderDlg.h"
#include ".\tbuilderdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString g_strFILEHEADER("GAME STUDIO FILE");


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


// CTBuilderDlg 대화 상자

struct binary_rect : public binary_function< CRect *, CRect *, bool>
{
	bool operator () ( const CRect * &_Left, const CRect * &_Right) const
	{
		if( max( _Left->Width(), _Left->Height()) < max( _Right->Width(), _Right->Height()) )
			return true;

		return false;
	};
};

struct binary_image : public binary_function< CImageItem *, CImageItem *, bool>
{
	bool operator () ( const CImageItem * &_Left, const CImageItem * &_Right) const
	{
		if(!_Right->m_dwSize)
			return false;

		if(!_Left->m_dwSize)
			return true;

		if( max( _Left->m_nWidth, _Left->m_nHeight) < max( _Right->m_nWidth, _Right->m_nHeight) )
			return true;

		return false;
	};
};


CTBuilderDlg::CTBuilderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTBuilderDlg::IDD, pParent)
	, m_strSRC(_T(""))
	, m_strDEST(_T(""))
	, m_strLIST(_T(""))
	, m_strHISTORY(_T(""))
	, m_bUsePrevID(TRUE)
	, m_bImageCheck(FALSE)
	, m_bMediaCheck(FALSE)
	, m_bAniCheck(FALSE)
	, m_bMeshCheck(FALSE)
	, m_bObjCheck(FALSE)
	, m_bSfxCheck(FALSE)
	, m_bMapCheck(FALSE)
	, m_bAllCheck(FALSE)
	, m_bTexCheck(FALSE)
	, m_bSaveIMGBUF(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTBuilderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SRC, m_cSRC);
	DDX_Control(pDX, IDC_BUTTON_DEST, m_cDEST);
	DDX_Text(pDX, IDC_EDIT_SRC, m_strSRC);
	DDX_Text(pDX, IDC_EDIT_DEST, m_strDEST);
	DDX_Text(pDX, IDC_EDIT_GSFLIST, m_strLIST);
	DDX_Control(pDX, IDC_BUTTON_GSFLIST, m_cLIST);
	DDX_Control(pDX, IDC_BUTTON_HISTORY, m_cHISTORY);
	DDX_Text(pDX, IDC_EDIT_HISTORY, m_strHISTORY);
	DDX_Check(pDX, IDC_CHECK_USEPREVID, m_bUsePrevID);
	DDX_Control(pDX, IDC_EDIT_HISTORY, m_cHISTORYEDIT);
	DDX_Control(pDX, IDC_STATIC_HISTORY, m_cHISTORYTITLE);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Text(pDX, IDC_STATIC_STATUS, m_strStatus);
	DDX_Check(pDX, IDC_IMAGE_CHECK, m_bImageCheck);
	DDX_Check(pDX, IDC_MEDIA_CHECK, m_bMediaCheck);
	DDX_Check(pDX, IDC_ANI_CHECK, m_bAniCheck);
	DDX_Check(pDX, IDC_MESH_CHECK, m_bMeshCheck);
	DDX_Check(pDX, IDC_OBJECT_CHECK, m_bObjCheck);
	DDX_Check(pDX, IDC_SFX_CHECK, m_bSfxCheck);
	DDX_Check(pDX, IDC_MAP_CHECK, m_bMapCheck);
	DDX_Check(pDX, IDC_ALL_CHECK, m_bAllCheck);
	DDX_Control(pDX, IDC_IMAGE_CHECK, m_cImage);
	DDX_Control(pDX, IDC_MEDIA_CHECK, m_cMedia);
	DDX_Control(pDX, IDC_ANI_CHECK, m_cAni);
	DDX_Control(pDX, IDC_MESH_CHECK, m_cMesh);
	DDX_Control(pDX, IDC_OBJECT_CHECK, m_cObj);
	DDX_Control(pDX, IDC_SFX_CHECK, m_cSfx);
	DDX_Control(pDX, IDC_MAP_CHECK, m_cMap);
	DDX_Check(pDX, IDC_TEX_CHECK, m_bTexCheck);
	DDX_Control(pDX, IDC_TEX_CHECK, m_cTex);
	DDX_Check(pDX, IDC_IMG_OUT, m_bSaveIMGBUF);
}

BEGIN_MESSAGE_MAP(CTBuilderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DEST, OnBnClickedButtonDest)
	ON_BN_CLICKED(IDC_BUTTON_GSFLIST, OnBnClickedButtonGsflist)
	ON_BN_CLICKED(IDC_BUTTON_SRC, OnBnClickedButtonSrc)
	ON_BN_CLICKED(IDC_BUILD, OnBnClickedBuild)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_USEPREVID, OnBnClickedCheckUseprevid)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY, OnBnClickedButtonHistory)
	ON_BN_CLICKED(IDC_IMAGE_CHECK, OnBnClickedImageCheck)
	ON_BN_CLICKED(IDC_MEDIA_CHECK, OnBnClickedMediaCheck)
	ON_BN_CLICKED(IDC_ANI_CHECK, OnBnClickedAniCheck)
	ON_BN_CLICKED(IDC_MESH_CHECK, OnBnClickedMeshCheck)
	ON_BN_CLICKED(IDC_OBJECT_CHECK, OnBnClickedObjectCheck)
	ON_BN_CLICKED(IDC_SFX_CHECK, OnBnClickedSfxCheck)
	ON_BN_CLICKED(IDC_MAP_CHECK, OnBnClickedMapCheck)
	ON_BN_CLICKED(IDC_ALL_CHECK, OnBnClickedAllCheck)
	ON_BN_CLICKED(IDC_TEX_CHECK, OnBnClickedTexCheck)
	ON_BN_CLICKED(IDC_IMG_OUT, OnBnClickedImgOut)
END_MESSAGE_MAP()


// CTBuilderDlg 메시지 처리기

BOOL CTBuilderDlg::OnInitDialog()
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
	m_cHISTORY.SetIcon(hIcon);
	m_cLIST.SetIcon(hIcon);
	m_cDEST.SetIcon(hIcon);
	m_cSRC.SetIcon(hIcon);

	Clear();
	ResetCtrl();

	m_Progress.SetRange(0, 100);
	m_Progress.SetPos(0);
	m_Progress.SetStep(1);
	m_strStatus = " ";

	m_bTexCheck = m_bAniCheck = m_bMapCheck = m_bSfxCheck = m_bMediaCheck = m_bMeshCheck = m_bImageCheck = m_bObjCheck = TRUE;
	m_bAllCheck = TRUE;

	m_cMap.EnableWindow(FALSE);
	m_cAni.EnableWindow(FALSE);
	m_cMesh.EnableWindow(FALSE);
	m_cObj.EnableWindow(FALSE);
	m_cImage.EnableWindow(FALSE);
	m_cSfx.EnableWindow(FALSE);
	m_cMesh.EnableWindow(FALSE);
	m_cMedia.EnableWindow(FALSE);
	m_cTex.EnableWindow(FALSE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTBuilderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTBuilderDlg::OnPaint() 
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
HCURSOR CTBuilderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTBuilderDlg::OnBnClickedButtonGsflist()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("GSF LIST Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

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

void CTBuilderDlg::OnBnClickedButtonHistory()
{
	CFileDialog dlg(
		TRUE,
		_T("*.tid"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("ID Files (*.tid)|*.tid|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strHISTORYFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strHISTORYFOLDER = dlg.GetFileName();
		m_strHISTORY = dlg.GetPathName();
		m_strHISTORYFOLDER = m_strHISTORY.Left(m_strHISTORY.GetLength() - m_strHISTORYFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTBuilderDlg::OnBnClickedButtonSrc()
{
	CTShell dlg;

	dlg.m_strSelDir = m_strSRCFOLDER;
	dlg.m_strTitle = _T("GSF Folder");

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strSRCFOLDER = dlg.m_strPath;
		m_strSRC = dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CTBuilderDlg::OnBnClickedButtonDest()
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

void CTBuilderDlg::OnBnClickedCheckUseprevid()
{
	UpdateData(TRUE);
	ResetCtrl();
}

void CTBuilderDlg::OnBnClickedBuild()
{
	CStdioFile vSRC( LPCSTR(m_strLIST), CFile::modeRead|CFile::typeText);
	CString strFILE;
	CString strDATA;
	Clear();

	CTBSPNode::InitCommon(&CItemBase::m_Device);
	CTachyonSFX::InitCommon();

	vSRC.ReadString(m_strGROUP);
	m_strSRC.TrimRight('\\');
	m_strSRC += CString("\\");
	CItemBase::m_bMerge = FALSE;

	if(UseHISTORY())
	{
		LoadHISTORY();
		m_Progress.StepIt();
	}

	BOOL bLoadData = FALSE;

	while(vSRC.ReadString(strFILE))
	{
		DWORD dwTSEED = 0;
		BOOL bOptimize = FALSE;

		vSRC.ReadString(strDATA);
		sscanf( LPCSTR(strDATA), _T("%u %u"), &bOptimize, &dwTSEED);

		LPTGSFFILE pGSFFILE = new TGSFFILE;
		pGSFFILE->m_strPath = m_strSRC + strFILE;
		pGSFFILE->m_bOptimize = bOptimize;
		pGSFFILE->m_dwTSEED = dwTSEED;

		if( pGSFFILE->m_File.Open( pGSFFILE->m_strPath, CFile::modeRead|CFile::typeBinary ) )
		{
			pGSFFILE->m_nIndex = (INT) m_vGSFFILE.size();
			m_vGSFFILE.push_back( pGSFFILE );
		}
		else
		{
			CString strFMT;
			strFMT.Format( "Can not found [%s]", pGSFFILE->m_strPath );
			AfxMessageBox( strFMT );

			delete pGSFFILE;
			ClearGSFFILE();
			return ;
		}
	}

	{
		VECTORGSFFILE::iterator itGSF, end;
		itGSF = m_vGSFFILE.begin();
		end = m_vGSFFILE.end();

		for(; itGSF != end ; ++itGSF )
		{
			CItemBase::m_bOptimize = (*itGSF)->m_bOptimize;
			LoadGSF( *itGSF );
			CItemBase::m_bMerge = TRUE;
		}
	}

	ReportDATA();

	m_strStatus = "Build DATA";
	UpdateData(FALSE);
	InitDevice();

	BuildDATA();

	Clear();
	vSRC.Close();
	ReleaseDevice();
	MessageBox("Complete Build", "Build", MB_OK);
}

void CTBuilderDlg::OnDestroy()
{
	Clear();
	CDialog::OnDestroy();
}

void CTBuilderDlg::Clear()
{
	MAPIDINDEX::iterator itINDEX;
	MAPGSFONT::iterator itFONT;

	for( itINDEX = CItemBase::m_mapLocalID.begin(); itINDEX != CItemBase::m_mapLocalID.end(); itINDEX++)
	{
		VECTORIDINDEX *pINDEX = (*itINDEX).second;

		while(!pINDEX->empty())
		{
			delete pINDEX->back();
			pINDEX->pop_back();
		}

		delete pINDEX;
	}

	for( itFONT = CItemBase::m_mapFont.begin(); itFONT != CItemBase::m_mapFont.end(); itFONT++)
		delete (*itFONT).second;

	CItemBase::m_mapGlobalID.clear();
	CItemBase::m_mapLocalID.clear();
	CItemBase::m_mapMenuID.clear();
	CItemBase::m_mapFont.clear();

	CItemBase::m_mapAnimationItem.clear();
	CItemBase::m_mapTextureItem.clear();
	CItemBase::m_mapImageItem.clear();
	CItemBase::m_mapMediaItem.clear();
	CItemBase::m_mapFrameItem.clear();
	CItemBase::m_mapCompItem.clear();
	CItemBase::m_mapMeshItem.clear();
	CItemBase::m_mapMAPItem.clear();
	CItemBase::m_mapOBJItem.clear();
	CItemBase::m_mapSFXItem.clear();

	ClearItem(&m_vTANIMATION);
	ClearItem(&m_vTOBJECT);
	ClearItem(&m_vTEXTURE);
	ClearItem(&m_vTIMAGE);
	ClearItem(&m_vTMEDIA);
	ClearItem(&m_vTFRAME);
	ClearItem(&m_vTMESH);
	ClearItem(&m_vTCOMP);
	ClearItem(&m_vTSFX);
	ClearItem(&m_vTMAP);
	m_strGROUP.Empty();

	CTachyonSFX::ReleaseCommon();
	CTBSPNode::ReleaseCommon();

	{
		VECTORGSFFILE::iterator itGSF, end;
		itGSF = m_vGSFFILE.begin();
		end = m_vGSFFILE.end();

		for(; itGSF != end ; ++itGSF )
		{
			LPTGSFFILE pGSFFILE = (*itGSF);

			if( pGSFFILE )
				pGSFFILE->m_File.Close();

			delete pGSFFILE;
		}

		m_vGSFFILE.clear();
	}
}

void CTBuilderDlg::ClearItem( LPVECTORTPROJECT pTPROJECT)
{
	while(!pTPROJECT->empty())
	{
		delete pTPROJECT->back();
		pTPROJECT->pop_back();
	}
}

void CTBuilderDlg::ClearGSFFILE()
{
	VECTORGSFFILE::iterator it, end;
	it = m_vGSFFILE.begin();
	end = m_vGSFFILE.end();

	for(; it != end ; ++it )
	{
		LPTGSFFILE pGSFFILE = (*it);
		if( pGSFFILE )
			pGSFFILE->m_File.Close();
		delete pGSFFILE;
	}

	m_vGSFFILE.clear();
}

void CTBuilderDlg::LoadGSF( LPTGSFFILE pGSFFILE )
{
	DWORD dwAnimationID;
	DWORD dwOBJTypeID;
	DWORD dwTextureID;
	DWORD dwImageID;
	DWORD dwMediaID;
	DWORD dwFrameID;
	DWORD dwCompID;
	DWORD dwMeshID;
	DWORD dwMAPID;
	DWORD dwOBJID;
	DWORD dwSFXID;

	pGSFFILE->m_File.Seek( g_strFILEHEADER.GetLength() + sizeof(DWORD), CFile::begin);
	pGSFFILE->m_File.Read( &dwAnimationID, sizeof(DWORD));
	pGSFFILE->m_File.Read( &dwOBJTypeID, sizeof(DWORD));
	pGSFFILE->m_File.Read( &dwTextureID, sizeof(DWORD));
	pGSFFILE->m_File.Read( &dwImageID, sizeof(DWORD));
	pGSFFILE->m_File.Read( &dwMediaID, sizeof(DWORD));
	pGSFFILE->m_File.Read( &dwFrameID, sizeof(DWORD));
	pGSFFILE->m_File.Read( &dwCompID, sizeof(DWORD));
	pGSFFILE->m_File.Read( &dwMeshID, sizeof(DWORD));
	pGSFFILE->m_File.Read( &dwMAPID, sizeof(DWORD));
	pGSFFILE->m_File.Read( &dwOBJID, sizeof(DWORD));
	pGSFFILE->m_File.Read( &dwSFXID, sizeof(DWORD));

	if(CItemBase::m_bMerge)
	{
		CItemBase::g_dwAnimationID = pGSFFILE->m_dwTSEED;
		CItemBase::g_dwOBJTypeID = pGSFFILE->m_dwTSEED;
		CItemBase::g_dwTextureID = pGSFFILE->m_dwTSEED;
		CItemBase::g_dwImageID = pGSFFILE->m_dwTSEED;
		CItemBase::g_dwMediaID = pGSFFILE->m_dwTSEED;
		CItemBase::g_dwFrameID = pGSFFILE->m_dwTSEED;
		CItemBase::g_dwCompID = pGSFFILE->m_dwTSEED;
		CItemBase::g_dwMeshID = pGSFFILE->m_dwTSEED;
		CItemBase::g_dwMAPID = pGSFFILE->m_dwTSEED;
		CItemBase::g_dwOBJID = pGSFFILE->m_dwTSEED;
		CItemBase::g_dwSFXID = pGSFFILE->m_dwTSEED;
	}

	CTBSPPortal::m_bBasePortalID = 0xFF;
	CTBSPNode::m_bBaseNodeID = 0xFF;
	LoadGlobal(&(pGSFFILE->m_File));

	LoadRoot( &m_vTEXTURE, pGSFFILE );
	LoadRoot( &m_vTIMAGE, pGSFFILE );
	LoadRoot( &m_vTMEDIA, pGSFFILE );
	LoadRoot( &m_vTANIMATION, pGSFFILE );
	LoadRoot( &m_vTMESH, pGSFFILE );
	LoadRoot( &m_vTOBJECT, pGSFFILE );
	LoadRoot( &m_vTSFX, pGSFFILE );
	LoadRoot( &m_vTMAP, pGSFFILE );
	LoadRoot( &m_vTCOMP, pGSFFILE );
	LoadRoot( &m_vTFRAME, pGSFFILE );

	if(CItemBase::m_bMerge)
	{
		CItemBase::g_dwAnimationID += dwAnimationID;
		CItemBase::g_dwOBJTypeID += dwOBJTypeID;
		CItemBase::g_dwTextureID += dwTextureID;
		CItemBase::g_dwImageID += dwImageID;
		CItemBase::g_dwMediaID += dwMediaID;
		CItemBase::g_dwFrameID += dwFrameID;
		CItemBase::g_dwCompID += dwCompID;
		CItemBase::g_dwMeshID += dwMeshID;
		CItemBase::g_dwMAPID += dwMAPID;
		CItemBase::g_dwOBJID += dwOBJID;
		CItemBase::g_dwSFXID += dwSFXID;
	}
	else
	{
		CItemBase::g_dwAnimationID = dwAnimationID;
		CItemBase::g_dwOBJTypeID = dwOBJTypeID;
		CItemBase::g_dwTextureID = dwTextureID;
		CItemBase::g_dwImageID = dwImageID;
		CItemBase::g_dwMediaID = dwMediaID;
		CItemBase::g_dwFrameID = dwFrameID;
		CItemBase::g_dwCompID = dwCompID;
		CItemBase::g_dwMeshID = dwMeshID;
		CItemBase::g_dwMAPID = dwMAPID;
		CItemBase::g_dwOBJID = dwOBJID;
		CItemBase::g_dwSFXID = dwSFXID;
	}
}

void CTBuilderDlg::LoadHISTORY()
{
	CFile file( m_strHISTORY, CFile::modeRead|CFile::typeBinary);
	LoadGlobal(&file);
	CItemBase::m_bMerge = TRUE;
	file.Close();
}

void CTBuilderDlg::LoadGlobal( CFile *pFILE)
{
	DWORD dwLength = (DWORD) pFILE->GetLength();
	int nCount = 0;

	if(!CItemBase::m_bMerge)
		pFILE->Read( &CItemBase::g_dwLocalID, sizeof(DWORD));
	else
		pFILE->Seek( sizeof(DWORD), CFile::current);
	pFILE->Read( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		CString strID = CItemBase::LoadString(pFILE);
		DWORD dwID;

		pFILE->Read( &dwID, sizeof(DWORD));
		if(!CItemBase::m_bMerge)
			CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( strID, dwID));
	}

	pFILE->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		CString strID = CItemBase::LoadString(pFILE);
		VECTORIDINDEX *pINDEX = new VECTORIDINDEX();
		int nINDEX = 0;

		pFILE->Read( &nINDEX, sizeof(int));
		for( int j=0; j<nINDEX; j++)
		{
			LPIDINDEX pID = new IDINDEX();

			pFILE->Read( &pID->m_bType, sizeof(BYTE));
			pFILE->Read( &pID->m_dwID, sizeof(DWORD));

			if(!CItemBase::m_bMerge)
				pINDEX->push_back(pID);
			else
				delete pID;
		}

		if(!CItemBase::m_bMerge)
			CItemBase::m_mapLocalID.insert( MAPIDINDEX::value_type( strID, pINDEX));
		else
			delete pINDEX;
	}

	pFILE->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPGSFONT pFONT = new GSFONT();
		DWORD dwID;

		pFILE->Read( &dwID, sizeof(DWORD));
		pFILE->Read( &pFONT->m_FONT, sizeof(LOGFONT));
		pFILE->Read( &pFONT->m_dwColor, sizeof(DWORD));

		pFONT->m_strFontID = CItemBase::LoadString(pFILE);
		pFONT->m_GDIFONT.CreateFontIndirect(&pFONT->m_FONT);
		pFONT->m_D3DFONT.SetFont(&pFONT->m_GDIFONT);
		pFONT->m_D3DFONT.m_dwColor = pFONT->m_dwColor;

		if(CItemBase::m_bMerge)
			if( CItemBase::m_mapGlobalID.find(pFONT->m_strFontID) == CItemBase::m_mapGlobalID.end() )
			{
				dwID = 1;

				while( CItemBase::m_mapFont.find(dwID) != CItemBase::m_mapFont.end() )
					dwID++;

				CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( pFONT->m_strFontID, dwID));
			}
			else
			{
				MAPGSFONT::iterator finder = CItemBase::m_mapFont.find(dwID);

				if( finder != CItemBase::m_mapFont.end() )
				{
					delete (*finder).second;
					CItemBase::m_mapFont.erase(finder);
				}
			}
		CItemBase::m_mapFont.insert( MAPGSFONT::value_type( dwID, pFONT));
	}

	pFILE->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		CString strID;
		DWORD dwID;

		pFILE->Read( &dwID, sizeof(DWORD));
		strID = CItemBase::LoadString(pFILE);

		if( CItemBase::m_bMerge && CItemBase::m_mapGlobalID.find(strID) == CItemBase::m_mapGlobalID.end() )
		{
			dwID = 1;

			while( CItemBase::m_mapMenuID.find(dwID) != CItemBase::m_mapMenuID.end() )
				dwID++;

			CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( strID, dwID));
		}

		CItemBase::m_mapMenuID.insert( MAPSTR::value_type( dwID, strID));
	}
}

void CTBuilderDlg::LoadRoot( LPVECTORTPROJECT pTPROJECT, LPTGSFFILE pGSFFILE )
{
	int nCount = 0;

	pGSFFILE->m_File.Read( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		int nChild = 0;

		pGSFFILE->m_File.Read( &nChild, sizeof(int));
		CItemBase *pItem = CItemBase::CreateItem( pGSFFILE );

		if(!pItem)
		{
			CString strTitle;
			CString strText;

			strTitle.LoadString(IDS_ERR_TITLE);
			strText.LoadString(IDS_ERR_INVALID_FILE);

			AfxGetMainWnd()->MessageBox(
				strText,
				strTitle,
				MB_ICONHAND|
				MB_OK);

			return;
		}

		AddItem( pTPROJECT, pItem);
		BYTE bResult = LoadData(
			pTPROJECT,
			pGSFFILE,
			nChild );

		if(!bResult)
		{
			CString strTitle;
			CString strText;

			strTitle.LoadString(IDS_ERR_TITLE);
			strText.LoadString(IDS_ERR_INVALID_FILE);

			AfxGetMainWnd()->MessageBox(
				strText,
				strTitle,
				MB_ICONHAND|
				MB_OK);

			return;
		}
	}
}

BYTE CTBuilderDlg::LoadData(
	LPVECTORTPROJECT pTPROJECT,
	LPTGSFFILE pGSFFILE,
	int nCount )
{
	for( int i=0; i<nCount; i++)
	{
		int nChild = 0;

		pGSFFILE->m_File.Read( &nChild, sizeof(int));
		CItemBase *pItem = CItemBase::CreateItem( pGSFFILE );

		if(!pItem)
			return FALSE;

		AddItem( pTPROJECT, pItem);

		BYTE bResult = LoadData(
			pTPROJECT,
			pGSFFILE,
			nChild );

		if(!bResult)
			return FALSE;
	}

	m_Progress.SetRange(0, 100);
	m_Progress.StepIt();
	CString strfilename = pGSFFILE->m_File.GetFileName();
	m_strStatus = "Loading Data : ";
	m_strStatus += strfilename;
	UpdateData(FALSE); 
	
	
	return TRUE;
}

void CTBuilderDlg::AddItem( LPVECTORTPROJECT pTPROJECT,
						    CItemBase *pItem)
{
	switch(pItem->m_bType)
	{
	case ITEM_PROJECT	:
		{
			for( BYTE i=0; i<BYTE(pTPROJECT->size()); i++)
				if( (*pTPROJECT)[i]->m_strNAME == pItem->m_strName )
				{
					delete pItem;
					return;
				}
			LPTPROJECT pNEW = new TPROJECT();

			pNEW->m_strNAME = pItem->m_strName;
			pNEW->m_dwID = pItem->m_dwID;

			pTPROJECT->push_back(pNEW);
			delete pItem;
		}

		break;

	case ITEM_TEXTURE	:
	case ITEM_IMAGE		:
	case ITEM_MEDIA		:
	case ITEM_ANIMATION	:
	case ITEM_MESH		:
	case ITEM_OBJECT	:
	case ITEM_SFX		:
	case ITEM_EXTMAP	:
	case ITEM_BSPMAP	:
	case ITEM_COMP		:
	case ITEM_FRAME		: pTPROJECT->back()->m_vTITEM.push_back(pItem); break;
	default				: delete pItem; break;
	}
}

void CTBuilderDlg::BuildDATA()
{
	MAPRESINDEX::iterator it;
	MAPRESINDEX mapTINDEX;

	VECTORSTRING vTFILE;
	CString strINDEX;

	int range = INT (m_vTEXTURE.size() + m_vTIMAGE.size() + m_vTMEDIA.size() + m_vTANIMATION.size() + 
				m_vTMESH.size() + m_vTOBJECT.size() + m_vTSFX.size() + m_vTMAP.size() );
	m_Progress.SetRange(0, range);
	
	if(m_bTexCheck)
	{
		for( int eTexDet=TEXTURE_DETAIL_LOW; eTexDet<TEXTURE_DETAIL_COUNT; eTexDet++)
		{
			for( int i=0; i<INT(m_vTEXTURE.size()); i++)
			{
				BuildTEX( m_vTEXTURE[i], &vTFILE, &mapTINDEX, (TEXTURE_DETAIL_LEVEL)eTexDet);
				m_Progress.StepIt();
			}
			strINDEX.Format( IDS_FMT_TEXIDX, eTexDet,m_strGROUP);
			BuildINDEX( strINDEX, &vTFILE, &mapTINDEX);

			for( it = mapTINDEX.begin(); it != mapTINDEX.end(); it++)
				delete (*it).second;
			mapTINDEX.clear();
			vTFILE.clear();
		}
	}

	if(m_bImageCheck)
	{
		VECTORSTRING vTIS;

		vTIS.clear();
		m_dwTIS = 0;
		m_nIMGCOUNT = 0;

		for( int i=0; i<INT(m_vTIMAGE.size()); i++)
		{
			BuildIMG( m_vTIMAGE[i], &vTIS, &vTFILE, &mapTINDEX);
			m_Progress.StepIt();
		}

		strINDEX.Format( IDS_FMT_IMGIDX, m_strGROUP);
		BuildINDEX( strINDEX, &vTFILE, &mapTINDEX);
		BuildTIS(&vTIS);

		for( it = mapTINDEX.begin(); it != mapTINDEX.end(); it++)
			delete (*it).second;

		mapTINDEX.clear();
		vTFILE.clear();
		vTIS.clear();
	}
	if(m_bMediaCheck)
	{
		for( int i=0; i<INT(m_vTMEDIA.size()); i++)
		{
			BuildMEDIA( m_vTMEDIA[i], &vTFILE, &mapTINDEX);
			m_Progress.StepIt();
		}
		strINDEX.Format( IDS_FMT_MEDIAIDX, m_strGROUP);
		BuildINDEX( strINDEX, &vTFILE, &mapTINDEX);

		for( it = mapTINDEX.begin(); it != mapTINDEX.end(); it++)
			delete (*it).second;
	
		mapTINDEX.clear();
		vTFILE.clear();
	}
	if(m_bAniCheck)
	{
		for(int i=0; i<INT(m_vTANIMATION.size()); i++)
		{
			BuildANI( m_vTANIMATION[i], &vTFILE, &mapTINDEX);
			m_Progress.StepIt();
		}
		strINDEX.Format( IDS_FMT_ANIIDX, m_strGROUP);
		BuildINDEX( strINDEX, &vTFILE, &mapTINDEX);

		for( it = mapTINDEX.begin(); it != mapTINDEX.end(); it++)
			delete (*it).second;

		mapTINDEX.clear();
		vTFILE.clear();
	}
	if(m_bMeshCheck)
	{
		for(int i=0; i<INT(m_vTMESH.size()); i++)
		{
			BuildMESH( m_vTMESH[i], &vTFILE, &mapTINDEX);
			m_Progress.StepIt();
		}
	
		strINDEX.Format( IDS_FMT_MESHIDX, m_strGROUP);
		BuildINDEX( strINDEX, &vTFILE, &mapTINDEX);

		for( it = mapTINDEX.begin(); it != mapTINDEX.end(); it++)
			delete (*it).second;

		mapTINDEX.clear();
		vTFILE.clear();
	}
	if(m_bObjCheck)
	{
		for(int i=0; i<INT(m_vTOBJECT.size()); i++)
		{
			BuildOBJ( m_vTOBJECT[i], &vTFILE, &mapTINDEX);
			m_Progress.StepIt();
		}
		strINDEX.Format( IDS_FMT_OBJIDX, m_strGROUP);
		BuildINDEX( strINDEX, &vTFILE, &mapTINDEX);

		for( it = mapTINDEX.begin(); it != mapTINDEX.end(); it++)
			delete (*it).second;

		mapTINDEX.clear();
		vTFILE.clear();
	}
	if(m_bSfxCheck)
	{
		for(int i=0; i<INT(m_vTSFX.size()); i++)
		{
			BuildSFX( m_vTSFX[i], &vTFILE, &mapTINDEX);
			m_Progress.StepIt();
		}
		strINDEX.Format( IDS_FMT_SFXIDX, m_strGROUP);
		BuildINDEX( strINDEX, &vTFILE, &mapTINDEX);

		for( it = mapTINDEX.begin(); it != mapTINDEX.end(); it++)
			delete (*it).second;

		mapTINDEX.clear();
		vTFILE.clear();
	}
	if(m_bMapCheck)
	{
		for(int i=0; i<INT(m_vTMAP.size()); i++)
		{
			BuildMAP( m_vTMAP[i], &vTFILE, &mapTINDEX);
			m_Progress.StepIt();
		}
		strINDEX.Format( IDS_FMT_MAPIDX, m_strGROUP);
		BuildINDEX( strINDEX, &vTFILE, &mapTINDEX);

		for( it = mapTINDEX.begin(); it != mapTINDEX.end(); it++)
			delete (*it).second;

		mapTINDEX.clear();
		vTFILE.clear();
	}
	BuildTSC();
	BuildHEADER();
	BuildHISTORY();
}

void CTBuilderDlg::ReportDATA()
{
	CString strPATH = m_strDEST;
	strPATH.TrimRight('\\');
	strPATH += _T("\\report.log");
	CFile file(strPATH, CFile::modeCreate|CFile::modeWrite);

	CString strMSG;

	strMSG = "\n< Optimization : Removed Textures >\n";
	TRACE(strMSG);
	file.Write(strMSG, strMSG.GetLength());

	UINT total=0;
	for(size_t i=0; i<m_vTEXTURE.size(); ++i)
	{
		const VECTORITEM& vTITEM = m_vTEXTURE[i]->m_vTITEM;
		for(size_t j=0; j<vTITEM.size(); ++j)
		{
			if( !vTITEM[j]->m_bValid )
			{
				strMSG.Format("#%04u : %s\n", ++total, vTITEM[j]->m_strName);
				TRACE(strMSG);
				file.Write(strMSG, strMSG.GetLength());
			}
		}
	}

	strMSG = "\n< Optimization : Removed Meshes >\n";
	TRACE(strMSG);
	file.Write(strMSG, strMSG.GetLength());

	total = 0;
	for(size_t i=0; i<m_vTMESH.size(); ++i)
	{
		const VECTORITEM& vTITEM = m_vTMESH[i]->m_vTITEM;
		for(size_t j=0; j<vTITEM.size(); ++j)
		{
			if( !vTITEM[j]->m_bValid )
			{
				strMSG.Format("#%04u : %s\n", ++total, vTITEM[j]->m_strName);
				TRACE(strMSG);
				file.Write(strMSG, strMSG.GetLength());
			}
		}
	}
}

void CTBuilderDlg::GenerateMipmap(LPBYTE pData, DWORD dwSize, DWORD dwSizeDiv, LPBYTE* ppGenData, DWORD* pGenSize, DWORD* pOrgSize, BYTE bFormat)
{
	HRESULT hResult;
	LPDIRECT3DTEXTURE9 pTex;

	UINT nWidth;
	UINT nHeight;
	DWORD dwFilter;

	if( dwSizeDiv > 1 )
	{
		D3DXIMAGE_INFO info;
		hResult = D3DXGetImageInfoFromFileInMemory(
			pData,
			dwSize,
			&info);

		if( hResult != D3D_OK )
			return;

		if( info.Width >= dwSizeDiv )
			nWidth = info.Width/dwSizeDiv;
		else
			nWidth = info.Width;

		if( info.Height >= dwSizeDiv )
			nHeight = info.Height/dwSizeDiv;
		else
			nHeight = info.Height;

		dwFilter = D3DX_DEFAULT;
	}
	else
	{
		nWidth = D3DX_DEFAULT;
		nHeight = D3DX_DEFAULT;
		dwFilter = D3DX_FILTER_NONE;
	}

	D3DFORMAT eFormat = D3DFMT_DXT3;

	switch(bFormat)
	{
	case NON_COMP	: eFormat = D3DFMT_A8R8G8B8; break;
	case DXT1		: eFormat = D3DFMT_DXT1; break;
	case DXT2		: eFormat = D3DFMT_DXT2; break;
	case DXT3		: eFormat = D3DFMT_DXT3; break;
	case DXT4		: eFormat = D3DFMT_DXT4; break;
	case DXT5		: eFormat = D3DFMT_DXT5; break;
	}

	hResult = D3DXCreateTextureFromFileInMemoryEx(
		CItemBase::m_Device.m_pDevice,
		pData,
		dwSize,
		nWidth,
		nHeight,
		min( nWidth, nHeight) > 16 ? 5 :
		D3DX_DEFAULT, 0,
		eFormat,
		D3DPOOL_MANAGED,
		dwFilter,
		D3DX_DEFAULT,
		0, 
		NULL,
		NULL,
		&pTex);

	if( hResult == D3D_OK )
	{
		LPD3DXBUFFER pBuffer = NULL;
		hResult = D3DXSaveTextureToFileInMemory(
			&pBuffer, 
			D3DXIFF_DDS,
			pTex,
			NULL);

		if( hResult == D3D_OK )
		{
			*pOrgSize = pBuffer->GetBufferSize();
			*pGenSize = (*pOrgSize) + (*pOrgSize) / 10 + 12;

			*ppGenData = new BYTE[*pGenSize];
			compress2( *ppGenData, pGenSize, (LPBYTE) pBuffer->GetBufferPointer(), *pOrgSize, 9);

			pBuffer->Release();
		}

		pTex->Release();
	}
}

void CTBuilderDlg::BuildHISTORY()
{
	int nCount = INT(CItemBase::m_mapGlobalID.size());
	CString strPATH = m_strDEST;

	strPATH.TrimRight('\\');
	strPATH = strPATH + _T('\\') + m_strGROUP + _T(".tid");
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	MAPIDINDEX::iterator itL;
	MAPGSFONT::iterator itF;
	MAPSTRID::iterator itG;
	MAPSTR::iterator itM;

	file.Write( &CItemBase::g_dwLocalID, sizeof(DWORD));
	file.Write( &nCount, sizeof(int));

	int size = INT(CItemBase::m_mapGlobalID.size() + CItemBase::m_mapLocalID.size() + CItemBase::m_mapFont.size() +
		       CItemBase::m_mapMenuID.size() );

	m_Progress.SetRange(0, size);

	for( itG = CItemBase::m_mapGlobalID.begin(); itG != CItemBase::m_mapGlobalID.end(); itG++)
	{
		CItemBase::WriteString( &file, CString((*itG).first));
		file.Write( &(*itG).second, sizeof(DWORD));

		m_Progress.StepIt();
	}

	nCount = INT(CItemBase::m_mapLocalID.size());
	file.Write( &nCount, sizeof(int));

	for( itL = CItemBase::m_mapLocalID.begin(); itL != CItemBase::m_mapLocalID.end(); itL++)
	{
		CItemBase::WriteString( &file, CString((*itL).first));
		nCount = INT((*itL).second->size());
		file.Write( &nCount, sizeof(int));

		for( int i=0; i<nCount; i++)
		{
			file.Write( &((*((*itL).second))[i]->m_bType), sizeof(BYTE));
			file.Write( &((*((*itL).second))[i]->m_dwID), sizeof(DWORD));
		}
		m_Progress.StepIt();
	}

	nCount = INT(CItemBase::m_mapFont.size());
	file.Write( &nCount, sizeof(int));

	for( itF = CItemBase::m_mapFont.begin(); itF != CItemBase::m_mapFont.end(); itF++)
	{
		file.Write( &(*itF).first, sizeof(DWORD));
		file.Write( &(*itF).second->m_FONT, sizeof(LOGFONT));
		file.Write( &(*itF).second->m_dwColor, sizeof(DWORD));
		CItemBase::WriteString( &file, (*itF).second->m_strFontID);
		
		m_Progress.StepIt();
	}

	nCount = INT(CItemBase::m_mapMenuID.size());
	file.Write( &nCount, sizeof(int));

	for( itM = CItemBase::m_mapMenuID.begin(); itM != CItemBase::m_mapMenuID.end(); itM++)
	{
		file.Write( &(*itM).first, sizeof(DWORD));
		CItemBase::WriteString( &file, (*itM).second);

		m_Progress.StepIt();
	}
}

void CTBuilderDlg::BuildHEADER()
{
	int nCount = INT(CItemBase::m_mapGlobalID.size());

	CString strPATH = m_strDEST;
	MAPSTRID::iterator it;

	strPATH.TrimRight('\\');
	strPATH = strPATH + _T('\\') + m_strGROUP + _T("ID.h");
	CStdioFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeText);

	file.WriteString("#pragma once\n\n");

	m_Progress.SetRange(0, INT(CItemBase::m_mapGlobalID.size()));

	for( it = CItemBase::m_mapGlobalID.begin(); it != CItemBase::m_mapGlobalID.end(); it++)
	{
		CString strLINE;

		strLINE.Format( IDS_FMT_DEFINE, (*it).first, (*it).second);
		file.WriteString(strLINE);
		m_Progress.StepIt();
	}
}

void CTBuilderDlg::BuildTSC()
{
	CString strPATH = m_strDEST;

	strPATH.TrimRight('\\');
	strPATH = strPATH + _T('\\') + m_strGROUP + _T(".tsc");

	CStdioFile file(
		strPATH,
		CFile::modeCreate|
		CFile::modeWrite|
		CFile::typeText);

	PrintTime(file);
	PrintNewLine( file, 2);

	PrintOpenTag( file, HTAG_TCML);
	PrintNewLine( file, 2);

	PrintOpenTag( file, HTAG_HEAD);
	PrintNewLine(file);
	
	int range = INT( CItemBase::m_mapImageItem.size() + CItemBase::m_mapCompItem.size() + 
				CItemBase::m_mapFrameItem.size() + CItemBase::m_mapMenuID.size() + 
				CItemBase::m_mapFont.size() );

	m_Progress.SetRange(0, range);

	MAPITEM::iterator it;
	for( it = CItemBase::m_mapImageItem.begin(); it != CItemBase::m_mapImageItem.end(); it++)
	{
		CString strImageID;
		strImageID.Format( "ID_IMAGE_%d", (*it).second->m_dwID);

		PrintTab(file);
		PrintID( file, strImageID, (*it).second->m_dwID);
		PrintNewLine(file);

		m_Progress.StepIt();
	}
	PrintNewLine(file);

	for( it = CItemBase::m_mapCompItem.begin(); it != CItemBase::m_mapCompItem.end(); it++)
	{
		PrintTab(file);
		PrintID( file, (*it).second->m_strID, (*it).second->m_dwID);
		PrintNewLine(file);
		
		m_Progress.StepIt();
	}
	PrintNewLine(file);

	for( it = CItemBase::m_mapFrameItem.begin(); it != CItemBase::m_mapFrameItem.end(); it++)
	{
		CFRAMEItem *pFRAME = (CFRAMEItem *) (*it).second;

		if(pFRAME)
		{
			int nCount = INT(pFRAME->m_vCTRL.size());

			for( int i=0; i<nCount; i++)
			{
				PrintTab(file);
				PrintID( file, pFRAME->m_vCTRL[i]->m_strCtrlID, CItemBase::GetID(pFRAME->m_vCTRL[i]->m_strCtrlID));
				PrintNewLine(file);
			}
			m_Progress.StepIt();
		}
	}
	PrintNewLine(file);

	for( it = CItemBase::m_mapFrameItem.begin(); it != CItemBase::m_mapFrameItem.end(); it++)
	{
		PrintTab(file);
		PrintID( file, (*it).second->m_strID, (*it).second->m_dwID);
		PrintNewLine(file);

		m_Progress.StepIt();
	}
	PrintNewLine(file);

	MAPSTR::iterator itMENU;
	for( itMENU = CItemBase::m_mapMenuID.begin(); itMENU != CItemBase::m_mapMenuID.end(); itMENU++)
	{
		PrintTab(file);
		PrintID( file, (*itMENU).second, (*itMENU).first);
		PrintNewLine(file);

		m_Progress.StepIt();
	}
	PrintNewLine(file);

	MAPGSFONT::iterator itFONT;
	for( itFONT = CItemBase::m_mapFont.begin(); itFONT != CItemBase::m_mapFont.end(); itFONT++)
	{
		PrintTab(file);
		PrintID( file, (*itFONT).second->m_strFontID, (*itFONT).first);
		PrintNewLine(file);

		m_Progress.StepIt();
	}
	PrintNewLine(file);

	for( itFONT = CItemBase::m_mapFont.begin(); itFONT != CItemBase::m_mapFont.end(); itFONT++)
	{
		PrintTab(file);
		PrintFontType( file, (*itFONT).second);
		PrintNewLine(file);

		m_Progress.StepIt();
	}
	PrintCloseTag( file, HTAG_HEAD);
	PrintNewLine( file, 2);

	for( it = CItemBase::m_mapFrameItem.begin(); it != CItemBase::m_mapFrameItem.end(); it++)
	{
		PrintFrame( file, (CFRAMEItem *) (*it).second);

		m_Progress.StepIt();
	}

	PrintCloseTag( file, HTAG_TCML);
}

void CTBuilderDlg::BuildTIS( LPVECTORSTRING pTFILE)
{
	CString strPATH = m_strDEST;
	CString strINDEX;

	strINDEX.Format( IDS_FMT_IMGLIST, m_strGROUP);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Index\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strINDEX;

	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	int nCount = INT(pTFILE->size());

	file.Write( &nCount, sizeof(int));
	file.Write( &m_nIMGCOUNT, sizeof(int));

	for( int i=0; i<nCount; i++)
		CItemBase::WriteString( &file, (*pTFILE)[i]);
}

void CTBuilderDlg::BuildINDEX( CString strINDEX,
							   LPVECTORSTRING pTFILE,
							   LPMAPRESINDEX pTINDEX)
{
	CString strPATH = m_strDEST;

	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Index\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strINDEX;

	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	MAPRESINDEX::iterator it;

	int nTotal = INT(pTINDEX->size());
	int nCount = INT(pTFILE->size());

	file.Write( &nCount, sizeof(int));
	file.Write( &nTotal, sizeof(int));

	for( int i=0; i<nCount; i++)
		CItemBase::WriteString( &file, (*pTFILE)[i]);

	for( it = pTINDEX->begin(); it != pTINDEX->end(); it++)
	{
		LPRESINDEX pRES = (*it).second;

		file.Write( &(*it).first, sizeof(DWORD));
		file.Write( &pRES->m_dwFileID, sizeof(DWORD));
		file.Write( &pRES->m_dwPOS, sizeof(DWORD));
	}
}

void CTBuilderDlg::BuildTEX( LPTPROJECT pTPROJECT,
							 LPVECTORSTRING pTFILE,
							 LPMAPRESINDEX pTINDEX,
							 TEXTURE_DETAIL_LEVEL eTexDet)
{
	CString strPATH = m_strDEST;
	CString strFILE;

	DWORD dwFileID = (DWORD) pTFILE->size();

	strFILE.Format( IDS_EXT_TEX, eTexDet,pTPROJECT->m_strNAME);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("Skin\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	pTFILE->push_back(_T("Skin\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	for( int i=0; i<INT(pTPROJECT->m_vTITEM.size()); i++)
		BuildTEX( pTINDEX, pTPROJECT->m_vTITEM[i], dwFileID, &file, eTexDet);
}

void CTBuilderDlg::BuildIMG( LPTPROJECT pTPROJECT,
							 LPVECTORSTRING pTIS,
							 LPVECTORSTRING pTFILE,
							 LPMAPRESINDEX pTINDEX)
{
	CString strPATH = m_strDEST;
	CString strFILE;

	CString strTISPATH;
	CString strTISFILE;

	DWORD dwFileID = (DWORD) pTFILE->size();

	strTISFILE.Format( IDS_EXT_IMGSOURCE, pTPROJECT->m_strNAME);
	strFILE.Format( IDS_EXT_IMG, pTPROJECT->m_strNAME);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("Img\\");

	CreateDirectory( strPATH, NULL);
	strTISPATH = strPATH + strTISFILE;
	strPATH = strPATH + strFILE;

	pTIS->push_back(_T("Img\\") + strTISFILE);
	pTFILE->push_back(_T("Img\\") + strFILE);

	CFile vTIS( strTISPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	MAPTIMGBUF mapTIMGBUF;
	LISTIMAGE vITEM;

	mapTIMGBUF.clear();
	vITEM.clear();

	for( int i=0; i<INT(pTPROJECT->m_vTITEM.size()); i++)
		if( pTPROJECT->m_vTITEM[i]->m_bType == ITEM_IMAGE )
			vITEM.push_back((CImageItem *) pTPROJECT->m_vTITEM[i]);

	vITEM.sort(binary_image());
	while(!vITEM.empty())
	{
		BuildIMG( &mapTIMGBUF, pTINDEX, vITEM.back(), dwFileID, &file, &vTIS);
		vITEM.pop_back();
	}

	MAPTIMGBUF::iterator it;
	for( it = mapTIMGBUF.begin(); it != mapTIMGBUF.end(); it++)
	{
		LPTIMGBUF pTBUF = (*it).second;

		if(pTBUF->m_pTIMG)
		{
			BYTE bFormat = (BYTE) (*it).first;

			D3DFORMAT nFORMAT = GetD3DFormat(bFormat);
			CTachyonCompressor tis;

			LPDIRECT3DSURFACE9 pDDSIMG = NULL;
			LPD3DXBUFFER pDDS = NULL;

			CItemBase::m_Device.m_pDevice->CreateOffscreenPlainSurface(
				TIMGBUF_MAX_SIZE,
				TIMGBUF_MAX_SIZE,
				nFORMAT,
				D3DPOOL_SCRATCH,
				&pDDSIMG,
				NULL);

			D3DXLoadSurfaceFromSurface(
				pDDSIMG,
				NULL,
				NULL,
				pTBUF->m_pTIMG,
				NULL,
				NULL,
				D3DX_FILTER_NONE,
				NULL);

			D3DXSaveSurfaceToFileInMemory(
				&pDDS,
				D3DXIFF_DDS,
				pDDSIMG,
				NULL,
				NULL);

			if(m_bSaveIMGBUF)
			{
				CString strPATH = m_strSRC;
				CString strDDS;

				strPATH.TrimRight('\\');
				strPATH = strPATH + _T("\\IMAGE\\");

				CreateDirectory( strPATH, NULL);
				strDDS.Format(
					_T("%s%d.JPG"),
					strPATH,
					pTBUF->m_dwID);

				D3DXSaveSurfaceToFile(
					strDDS,
					D3DXIFF_JPG,
					pDDSIMG,
					NULL,
					NULL);
			}

			tis.Write( &pTBUF->m_dwID, sizeof(DWORD));
			tis.Write( &bFormat, sizeof(BYTE));

			DWORD dwSIZE = pDDS->GetBufferSize();
			DWORD dwDATA = dwSIZE + dwSIZE / 10 + 12;
			LPBYTE pIDATA = new BYTE[dwDATA];

			compress2( pIDATA, &dwDATA, (LPBYTE) pDDS->GetBufferPointer(), dwSIZE, 9);
			tis.Write( &dwSIZE, sizeof(DWORD));
			tis.Write( &dwDATA, sizeof(DWORD));
			tis.Write( pIDATA, dwDATA);
			m_nIMGCOUNT++;

			pDDSIMG->Release();
			pDDS->Release();
			delete[] pIDATA;
			tis.ToFile(&vTIS);
		}

		delete pTBUF;
	}

	mapTIMGBUF.clear();
}

void CTBuilderDlg::BuildMEDIA( LPTPROJECT pTPROJECT,
							   LPVECTORSTRING pTFILE,
							   LPMAPRESINDEX pTINDEX)
{
	for( int i=0; i<INT(pTPROJECT->m_vTITEM.size()); i++)
		BuildMEDIA( pTPROJECT, pTFILE, pTINDEX, pTPROJECT->m_vTITEM[i]);
}

void CTBuilderDlg::BuildANI( LPTPROJECT pTPROJECT,
							 LPVECTORSTRING pTFILE,
							 LPMAPRESINDEX pTINDEX)
{
	CString strPATH = m_strDEST;
	CString strFILE;

	DWORD dwFileID = (DWORD) pTFILE->size();

	strFILE.Format( IDS_EXT_ACT, pTPROJECT->m_strNAME);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("Action\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	pTFILE->push_back(_T("Action\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	for( int i=0; i<INT(pTPROJECT->m_vTITEM.size()); i++)
		BuildANI( pTINDEX, pTPROJECT->m_vTITEM[i], dwFileID, &file);
}

void CTBuilderDlg::BuildMESH( LPTPROJECT pTPROJECT,
							  LPVECTORSTRING pTFILE,
							  LPMAPRESINDEX pTINDEX)
{
	CString strPATH = m_strDEST;
	CString strFILE;

	DWORD dwFileID = (DWORD) pTFILE->size();

	strFILE.Format( IDS_EXT_MESH, pTPROJECT->m_strNAME);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("Mesh\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	pTFILE->push_back(_T("Mesh\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	for( int i=0; i<INT(pTPROJECT->m_vTITEM.size()); i++)
		BuildMESH( pTINDEX, pTPROJECT->m_vTITEM[i], dwFileID, &file);
}

void CTBuilderDlg::BuildOBJ( LPTPROJECT pTPROJECT,
							 LPVECTORSTRING pTFILE,
							 LPMAPRESINDEX pTINDEX)
{
	CString strPATH = m_strDEST;
	CString strFILE;

	DWORD dwFileID = (DWORD) pTFILE->size();

	strFILE.Format( IDS_EXT_OBJ, pTPROJECT->m_strNAME);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("OBJ\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	pTFILE->push_back(_T("OBJ\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	for( int i=0; i<INT(pTPROJECT->m_vTITEM.size()); i++)
		BuildOBJ( pTINDEX, pTPROJECT->m_vTITEM[i], dwFileID, &file);
}

void CTBuilderDlg::BuildSFX( LPTPROJECT pTPROJECT,
							 LPVECTORSTRING pTFILE,
							 LPMAPRESINDEX pTINDEX)
{
	CString strPATH = m_strDEST;
	CString strFILE;

	DWORD dwFileID = (DWORD) pTFILE->size();

	strFILE.Format( IDS_EXT_SFX, pTPROJECT->m_strNAME);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("SFX\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	pTFILE->push_back(_T("SFX\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	for( int i=0; i<INT(pTPROJECT->m_vTITEM.size()); i++)
		BuildSFX( pTINDEX, pTPROJECT->m_vTITEM[i], dwFileID, &file);
}

void CTBuilderDlg::BuildMAP( LPTPROJECT pTPROJECT,
							 LPVECTORSTRING pTFILE,
							 LPMAPRESINDEX pTINDEX)
{
	CString strPATH = m_strDEST;
	CString strFILE;

	DWORD dwFileID = (DWORD) pTFILE->size();

	strFILE.Format( IDS_EXT_MAP, pTPROJECT->m_strNAME);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("MAP\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	pTFILE->push_back(_T("MAP\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	for( int i=0; i<INT(pTPROJECT->m_vTITEM.size()); i++)
		BuildMAP( pTINDEX, pTPROJECT->m_vTITEM[i], dwFileID, &file);
}

void CTBuilderDlg::BuildTEX( LPMAPRESINDEX pTINDEX,
							 CItemBase *pItem,
							 DWORD dwFileID,
							 CFile *pTFILE,
							 TEXTURE_DETAIL_LEVEL eTexDet)
{
	if( pItem->m_bType != ITEM_TEXTURE || !pItem->m_bValid )
		return;

	LPRESINDEX pRES = new RESINDEX();

	pRES->m_dwPOS = (DWORD) pTFILE->GetPosition();
	pRES->m_dwFileID = dwFileID;

	pTINDEX->insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
	BuildTEX( (CTextureItem *) pItem, pTFILE, eTexDet);
}

void CTBuilderDlg::BuildIMG( LPMAPTIMGBUF pTIMGBUF,
							 LPMAPRESINDEX pTINDEX,
							 CItemBase *pItem,
							 DWORD dwFileID,
							 CFile *pTFILE,
							 CFile *pTIS)
{
	if( pItem->m_bType != ITEM_IMAGE )
		return;
	LPRESINDEX pRES = new RESINDEX();

	pRES->m_dwPOS = (DWORD) pTFILE->GetPosition();
	pRES->m_dwFileID = dwFileID;

	pTINDEX->insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
	BuildIMG( pTIMGBUF, (CImageItem *) pItem, pTFILE, pTIS);
}

void CTBuilderDlg::BuildMEDIA( LPTPROJECT pTPROJECT,
							   LPVECTORSTRING pTFILE,
							   LPMAPRESINDEX pTINDEX,
							   CItemBase *pItem)
{
	if( pItem->m_bType != ITEM_MEDIA )
		return;

	DWORD dwFileID = (DWORD) pTFILE->size();
	LPRESINDEX pRES = new RESINDEX();

	CString strPATH = m_strDEST;
	CString strFILE;

	strFILE.Format( IDS_EXT_MEDIA, ((CMediaItem *) pItem)->GetPlayerType(), pItem->m_dwID);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("Media\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	pRES->m_dwPOS = MAKELONG( WORD(((CMediaItem *) pItem)->GetPlayerType()), WORD(((CMediaItem *) pItem)->m_bVolume));
	pRES->m_dwFileID = dwFileID;

	pTINDEX->insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
	pTFILE->push_back(_T("Media\\") + strFILE);

	CFile file;
	if( file.Open( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
	{
		CMediaItem* pMediaItem = (CMediaItem*) pItem;

		if( pMediaItem->LoadData() )
		{
			file.Write( pMediaItem->m_pData, pMediaItem->m_dwSize );
			pMediaItem->ReleaseData();
		}

		file.Close();
	}

	//CopyFile( ((CMediaItem *) pItem)->m_szFileName, strPATH, FALSE);
}

void CTBuilderDlg::BuildANI( LPMAPRESINDEX pTINDEX,
							 CItemBase *pItem,
							 DWORD dwFileID,
							 CFile *pTFILE)
{
	if( pItem->m_bType != ITEM_ANIMATION )
		return;
	LPRESINDEX pRES = new RESINDEX();

	pRES->m_dwPOS = (DWORD) pTFILE->GetPosition();
	pRES->m_dwFileID = dwFileID;

	pTINDEX->insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
	BuildANI( (CAnimationItem *) pItem, pTFILE);
}

void CTBuilderDlg::BuildMESH( LPMAPRESINDEX pTINDEX,
							  CItemBase *pItem,
							  DWORD dwFileID,
							  CFile *pTFILE)
{
	if( pItem->m_bType != ITEM_MESH || !pItem->m_bValid )
		return;

	LPRESINDEX pRES = new RESINDEX();

	pRES->m_dwPOS = (DWORD) pTFILE->GetPosition();
	pRES->m_dwFileID = dwFileID;

	pTINDEX->insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
	BuildMESH( (CMeshItem *) pItem, pTFILE);
}

void CTBuilderDlg::BuildOBJ( LPMAPRESINDEX pTINDEX,
							 CItemBase *pItem,
							 DWORD dwFileID,
							 CFile *pTFILE)
{
	if( pItem->m_bType != ITEM_OBJECT )
		return;
	LPRESINDEX pRES = new RESINDEX();

	pRES->m_dwPOS = (DWORD) pTFILE->GetPosition();
	pRES->m_dwFileID = dwFileID;

	pTINDEX->insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
	BuildOBJ( (COBJItem *) pItem, pTFILE);
}

void CTBuilderDlg::BuildSFX( LPMAPRESINDEX pTINDEX,
							 CItemBase *pItem,
							 DWORD dwFileID,
							 CFile *pTFILE)
{
	if( pItem->m_bType != ITEM_SFX )
		return;
	LPRESINDEX pRES = new RESINDEX();

	pRES->m_dwPOS = (DWORD) pTFILE->GetPosition();
	pRES->m_dwFileID = dwFileID;

	pTINDEX->insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
	BuildSFX( (CSFXItem *) pItem, pTFILE);
}

void CTBuilderDlg::BuildMAP( LPMAPRESINDEX pTINDEX,
							 CItemBase *pItem,
							 DWORD dwFileID,
							 CFile *pTFILE)
{
	switch(pItem->m_bType)
	{
	case ITEM_EXTMAP	:
		{
			LPRESINDEX pRES = new RESINDEX();

			pRES->m_dwPOS = (DWORD) pTFILE->GetPosition();
			pRES->m_dwFileID = dwFileID;

			pTINDEX->insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
			BuildEXTMAP( (CEXTMAPItem *) pItem, pTFILE);
		}

		break;

	case ITEM_BSPMAP	:
		{
			LPRESINDEX pRES = new RESINDEX();

			pRES->m_dwPOS = (DWORD) pTFILE->GetPosition();
			pRES->m_dwFileID = dwFileID;

			pTINDEX->insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
			BuildBSPMAP( (CBSPMAPItem *) pItem, pTFILE);
		}

		break;
	}
}

void CTBuilderDlg::BuildTEX( CTextureItem *pItem,
							 CFile *pTFILE,
							 TEXTURE_DETAIL_LEVEL eTexDet)
{
	BOOL bRet = pItem->LoadData();

	if( !bRet )
		return ;

	CTachyonCompressor cpr;
	
	DWORD dwTexDet = max(pItem->m_bFullSize, eTexDet);
	DWORD dwDiv = 1 << (TEXTURE_DETAIL_COUNT - dwTexDet - 1);

	LPBYTE pGenData = NULL;
	DWORD dwGenSize = 0;
	DWORD dwOrgSize = 0;
	if( pItem->m_dwSize > 0 )
		GenerateMipmap(pItem->m_pData, pItem->m_dwSize, dwDiv, &pGenData, &dwGenSize, &dwOrgSize, pItem->m_bFormat);

	int nCount = INT(pItem->m_vTextureID.size());

	cpr.Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
		cpr.Write( &pItem->m_vTextureID[i], sizeof(DWORD));

	nCount = INT(pItem->m_pTextureSet->m_vKey.size());
	cpr.Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		cpr.Write( &pItem->m_pTextureSet->m_vKey[i]->m_dwTick, sizeof(DWORD));
		cpr.Write( &pItem->m_pTextureSet->m_vKey[i]->m_fKeyU, sizeof(FLOAT));
		cpr.Write( &pItem->m_pTextureSet->m_vKey[i]->m_fKeyV, sizeof(FLOAT));
		cpr.Write( &pItem->m_pTextureSet->m_vKey[i]->m_fKeyR, sizeof(FLOAT));
		cpr.Write( &pItem->m_pTextureSet->m_vKey[i]->m_fKeySU, sizeof(FLOAT));
		cpr.Write( &pItem->m_pTextureSet->m_vKey[i]->m_fKeySV, sizeof(FLOAT));
	}

	cpr.Write( &pItem->m_pTextureSet->m_dwTotalTick, sizeof(DWORD));
	cpr.Write( &pItem->m_pTextureSet->m_dwMipFilter, sizeof(DWORD));
	cpr.Write( &pItem->m_pTextureSet->m_fMipBias, sizeof(FLOAT));
	cpr.Write( &pItem->m_bFullSize, sizeof(BYTE));
	cpr.Write( &pItem->m_bFormat, sizeof(BYTE));
	cpr.Write( &dwOrgSize, sizeof(DWORD));
	cpr.Write( &dwGenSize, sizeof(DWORD));

	if( dwGenSize > 0 )
	{
		cpr.Write( pGenData, dwGenSize);
		delete [] pGenData;
	}

	cpr.ToFile(pTFILE);

	pItem->ReleaseData();
}

void CTBuilderDlg::BuildIMG( LPMAPTIMGBUF pTIMGBUF,
							 CImageItem *pItem,
							 CFile *pTFILE,
							 CFile *pTIS)
{
	BOOL bRet = pItem->LoadData();

	if( !bRet )
		return ;

	CTachyonCompressor cpr;

	int nCount = INT(pItem->m_vImageID.size());

	cpr.Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
		cpr.Write( &pItem->m_vImageID[i], sizeof(DWORD));

	
	nCount = INT(pItem->m_pImageSet->m_vKey.size());
	cpr.Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		cpr.Write( &pItem->m_pImageSet->m_vKey[i]->m_dwTick, sizeof(DWORD));
		cpr.Write( &pItem->m_pImageSet->m_vKey[i]->m_dwColor, sizeof(DWORD));
	}

	cpr.Write( &pItem->m_pImageSet->m_dwTotalTick, sizeof(DWORD));
	cpr.Write( &pItem->m_bFormat2, sizeof(BYTE));

	if( pItem->m_dwSize > 0 )
	{
		// GSF파일에 있는 Image들은 무조건 NON_COMP이다.
		// m_bFormat은 단지 변환해야할 포맷의 마크용일뿐 
		// 그래서 m_bFormat에 맞는 실질적인 포맷으로 변환하여 저장한다.

		VECTORTIMGPART vTIMGPART;
		CD3DImage vTDATA;

		vTDATA.Load(pItem->m_pData2);
		vTIMGPART.clear();

		LPDIRECT3DSURFACE9 pIMG = vTDATA.CreateOffscreenPlainSurface(CItemBase::m_Device.m_pDevice);
		int nHeight = vTDATA.GetHeight();
		int nWidth = vTDATA.GetWidth();

		if(pIMG)
		{
			MAPTIMGBUF::iterator finder = pTIMGBUF->find(pItem->m_bFormat2);
			LPTIMGBUF pTBUF = NULL;

			LISTCRECT vTSRCIMG;
			vTSRCIMG.clear();

			vTSRCIMG.push_back(new CRect( 0, 0, nWidth, nHeight));
			if( finder == pTIMGBUF->end() )
			{
				pTBUF = new TIMGBUF();

				pTIMGBUF->insert( MAPTIMGBUF::value_type(
					pItem->m_bFormat2,
					pTBUF));
				m_dwTIS++;
				pTBUF->m_dwID = m_dwTIS;
			}
			else
				pTBUF = (*finder).second;

			while(!vTSRCIMG.empty())
			{
				vTSRCIMG.sort(binary_rect());

				CRect *pTSRC = vTSRCIMG.back();
				vTSRCIMG.pop_back();

				int nBoundH = min( TIMGBUF_MAX_BOUND, pTSRC->Height());
				int nBoundW = min( TIMGBUF_MAX_BOUND, pTSRC->Width());
				LISTCRECT::iterator it = pTBUF->m_vTGARBAGE.begin();

				while(it != pTBUF->m_vTGARBAGE.end())
				{
					LISTCRECT::iterator itNEXT = it;
					itNEXT++;

					if( (*it)->Width() >= nBoundW && (*it)->Height() >= nBoundH )
					{
						pTBUF->m_vTRECT.push_back(*it);
						pTBUF->m_vTGARBAGE.erase(it);
					}

					it = itNEXT;
				}

				it = pTBUF->m_vTRECT.begin();
				while(it != pTBUF->m_vTRECT.end())
				{
					LISTCRECT::iterator itNEXT = it;
					itNEXT++;

					if( (*it)->Width() < nBoundW || (*it)->Height() < nBoundH )
					{
						pTBUF->m_vTGARBAGE.push_back(*it);
						pTBUF->m_vTRECT.erase(it);
					}

					it = itNEXT;
				}

				if(pTBUF->m_vTRECT.empty())
				{
					if(pTBUF->m_pTIMG)
					{
						D3DFORMAT nFORMAT = GetD3DFormat(pItem->m_bFormat2);
						CTachyonCompressor tis;

						LPDIRECT3DSURFACE9 pDDSIMG = NULL;
						LPD3DXBUFFER pDDS = NULL;

						CItemBase::m_Device.m_pDevice->CreateOffscreenPlainSurface(
							TIMGBUF_MAX_SIZE,
							TIMGBUF_MAX_SIZE,
							nFORMAT,
							D3DPOOL_SCRATCH,
							&pDDSIMG,
							NULL);

						D3DXLoadSurfaceFromSurface(
							pDDSIMG,
							NULL,
							NULL,
							pTBUF->m_pTIMG,
							NULL,
							NULL,
							D3DX_FILTER_NONE,
							NULL);

						D3DXSaveSurfaceToFileInMemory(
							&pDDS,
							D3DXIFF_DDS,
							pDDSIMG,
							NULL,
							NULL);

						if(m_bSaveIMGBUF)
						{
							CString strPATH = m_strSRC;
							CString strDDS;

							strPATH.TrimRight('\\');
							strPATH = strPATH + _T("\\IMAGE\\");

							CreateDirectory( strPATH, NULL);
							strDDS.Format(
								_T("%s%d.JPG"),
								strPATH,
								pTBUF->m_dwID);

							D3DXSaveSurfaceToFile(
								strDDS,
								D3DXIFF_JPG,
								pDDSIMG,
								NULL,
								NULL);
						}

						tis.Write( &pTBUF->m_dwID, sizeof(DWORD));
						tis.Write( &pItem->m_bFormat2, sizeof(BYTE));

						DWORD dwSIZE = pDDS->GetBufferSize();
						DWORD dwDATA = dwSIZE + dwSIZE / 10 + 12;
						LPBYTE pIDATA = new BYTE[dwDATA];

						compress2( pIDATA, &dwDATA, (LPBYTE) pDDS->GetBufferPointer(), dwSIZE, 9);
						tis.Write( &dwSIZE, sizeof(DWORD));
						tis.Write( &dwDATA, sizeof(DWORD));
						tis.Write( pIDATA, dwDATA);
						m_nIMGCOUNT++;

						pDDSIMG->Release();
						pDDS->Release();
						delete[] pIDATA;
						tis.ToFile(pTIS);
					}
					m_dwTIS++;

					pTBUF->Release();
					pTBUF->m_dwID = m_dwTIS;

					CItemBase::m_Device.m_pDevice->CreateOffscreenPlainSurface(
						TIMGBUF_MAX_SIZE,
						TIMGBUF_MAX_SIZE,
						D3DFMT_A8R8G8B8,
						D3DPOOL_SCRATCH,
						&pTBUF->m_pTIMG,
						NULL);
					pTBUF->m_vTRECT.push_back(new CRect( 0, 0, TIMGBUF_MAX_SIZE, TIMGBUF_MAX_SIZE));
				}
				pTBUF->m_vTRECT.sort(binary_rect());

				CRect *pTDEST = pTBUF->m_vTRECT.back();
				CRect rcTSRC(pTSRC);
				CRect rcTDEST;
				LPTIMGPART pTPART = new TIMGPART();

				pTBUF->m_vTRECT.pop_back();
				FillRect(
					&pTBUF->m_vTRECT,
					&vTSRCIMG,
					pTDEST,
					pTSRC,
					&rcTDEST);

				rcTSRC.bottom = rcTSRC.top + rcTDEST.Height();
				rcTSRC.right = rcTSRC.left + rcTDEST.Width();

				D3DXLoadSurfaceFromSurface(
					pTBUF->m_pTIMG,
					NULL,
					&rcTDEST,
					pIMG,
					NULL,
					&rcTSRC,
					D3DX_FILTER_NONE,
					NULL);

				pTPART->m_dwImageID = pTBUF->m_dwID;
				for( i=0; i<4; i++)
				{
					pTPART->m_vTMESH[i].m_fPosX = FLOAT(i % 2 ? rcTSRC.right : rcTSRC.left) + 0.5f;
					pTPART->m_vTMESH[i].m_fPosY = FLOAT(i / 2 ? rcTSRC.bottom : rcTSRC.top) + 0.5f;
					pTPART->m_vTMESH[i].m_fPosZ = 0.5f;
					pTPART->m_vTMESH[i].m_fRHW = 1.0f;

					pTPART->m_vTMESH[i].m_fU = FLOAT(i % 2 ? rcTDEST.right : rcTDEST.left) / FLOAT(TIMGBUF_MAX_SIZE);
					pTPART->m_vTMESH[i].m_fV = FLOAT(i / 2 ? rcTDEST.bottom : rcTDEST.top) / FLOAT(TIMGBUF_MAX_SIZE);
				}
				vTIMGPART.push_back(pTPART);

				delete pTDEST;
				delete pTSRC;
			}

			pIMG->Release();
		}
		nCount = INT(vTIMGPART.size());

		DWORD dwSIZE = nCount ? 4 * sizeof(int) + nCount * (sizeof(DWORD) + 4 * sizeof(TVERTEX)) : 0;
		if(dwSIZE)
		{
			int nPitch = nWidth % 8 ? nWidth / 8 + 1 : nWidth / 8;
			int nMask = nPitch * nHeight;

			dwSIZE += nMask;
			cpr.Write( &dwSIZE, sizeof(DWORD));
			cpr.Write( &nCount, sizeof(int));
			cpr.Write( &nWidth, sizeof(int));
			cpr.Write( &nHeight, sizeof(int));

			for( i=0; i<nCount; i++)
			{
				cpr.Write( &vTIMGPART[i]->m_dwImageID, sizeof(DWORD));
				cpr.Write( vTIMGPART[i]->m_vTMESH, 4 * sizeof(TVERTEX));
			}

			cpr.Write( &nMask, sizeof(int));
			cpr.Write( vTDATA.GetMask(), nMask);
		}
		else
			cpr.Write( &dwSIZE, sizeof(DWORD));

		while(!vTIMGPART.empty())
		{
			delete vTIMGPART.back();
			vTIMGPART.pop_back();
		}
	}
	else
		cpr.Write( &pItem->m_dwSize, sizeof(DWORD));

	cpr.ToFile(pTFILE);
	pItem->ReleaseData();
}

void CTBuilderDlg::BuildANI( CAnimationItem *pItem,
							 CFile *pTFILE)
{
	BOOL bRet = pItem->LoadData();

	if( !bRet )
		return ;

	int nCount = INT(pItem->m_vAniID.size());

	pTFILE->Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		pTFILE->Write( &pItem->m_vAniID[i], sizeof(DWORD));
		pTFILE->Write( &pItem->m_vReverse[i], sizeof(BYTE));
	}

	nCount = INT(pItem->m_Aniset.m_vKey.size());
	pTFILE->Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		pTFILE->Write( &pItem->m_Aniset.m_vKey[i]->m_fTime, sizeof(FLOAT));
		pTFILE->Write( &pItem->m_Aniset.m_vKey[i]->m_fPosX, sizeof(FLOAT));
		pTFILE->Write( &pItem->m_Aniset.m_vKey[i]->m_fPosY, sizeof(FLOAT));
		pTFILE->Write( &pItem->m_Aniset.m_vKey[i]->m_fPosZ, sizeof(FLOAT));
		pTFILE->Write( &pItem->m_Aniset.m_vKey[i]->m_fRotX, sizeof(FLOAT));
		pTFILE->Write( &pItem->m_Aniset.m_vKey[i]->m_fRotY, sizeof(FLOAT));
		pTFILE->Write( &pItem->m_Aniset.m_vKey[i]->m_fRotZ, sizeof(FLOAT));
		pTFILE->Write( &pItem->m_Aniset.m_vKey[i]->m_fScaleX, sizeof(FLOAT));
		pTFILE->Write( &pItem->m_Aniset.m_vKey[i]->m_fScaleY, sizeof(FLOAT));
		pTFILE->Write( &pItem->m_Aniset.m_vKey[i]->m_fScaleZ, sizeof(FLOAT));
		pTFILE->Write( &pItem->m_Aniset.m_vKey[i]->m_dwEventID, sizeof(DWORD));
	}

	pTFILE->Write( &pItem->m_Aniset.m_fTimeScale, sizeof(FLOAT));
	pTFILE->Write( &pItem->m_Aniset.m_dwLoopID, sizeof(DWORD));
	pTFILE->Write( &pItem->m_Aniset.m_bLoop, sizeof(BYTE));

	if( pItem->m_dwSize > 0 )
	{
		LPBYTE pDATA = new BYTE[pItem->m_dwSize];

		DWORD dwNodeCount = *((LPDWORD) &pItem->m_pData[sizeof(int)]);
		DWORD dwSIZE = pItem->m_dwSize;
		int nRES = dwSIZE;

		LPBYTE pSRC = pItem->m_pData;
		LPBYTE pDEST = pDATA;
		int nVersion = *((int *) pSRC);

		memcpy( pDEST, pSRC, sizeof(int) + 3 * sizeof(DWORD) + dwNodeCount * sizeof(D3DMATRIX));
		pDEST += sizeof(int) + 3 * sizeof(DWORD) + dwNodeCount * sizeof(D3DMATRIX);
		pSRC += sizeof(int) + 3 * sizeof(DWORD) + dwNodeCount * sizeof(D3DMATRIX);
		nRES -= sizeof(int) + 3 * sizeof(DWORD) + dwNodeCount * sizeof(D3DMATRIX);

		for( i=0; i<dwNodeCount; i++)
		{
			DWORD dwCount = *((LPDWORD) pSRC);

			*((LPDWORD) pDEST) = 0;
			pDEST += sizeof(DWORD);
			pSRC += sizeof(DWORD);
			nRES -= sizeof(DWORD);

			DWORD dwSKIP = dwCount * (nVersion < 301 ? sizeof(WMESHVERTEX) : sizeof(TMESHBUFFER));
			pSRC += dwSKIP;
			nRES -= dwSKIP;

			if( dwSIZE > dwSKIP )
				dwSIZE -= dwSKIP;
			else
				dwSIZE = 0;
		}

		if( nRES > 0 )
			memcpy( pDEST, pSRC, nRES);

		pTFILE->Write( &dwSIZE, sizeof(DWORD));
		if( dwSIZE > 0 )
			pTFILE->Write( pDATA, dwSIZE);

		delete[] pDATA;
	}
	else
		pTFILE->Write( &pItem->m_dwSize, sizeof(DWORD));

	pItem->ReleaseData();
}

void CTBuilderDlg::BuildMESH( CMeshItem *pItem,
							  CFile *pTFILE)
{
	BOOL bRet = pItem->LoadData();

	if( !bRet )
		return ;
    
	DWORD dwLevel = INT(pItem->m_Mesh.m_vDist.size() + 1);

	pTFILE->Write( &pItem->m_Mesh.m_dwMeshCount, sizeof(DWORD));
	pTFILE->Write( &pItem->m_Mesh.m_dwNodeCount, sizeof(DWORD));
	pTFILE->Write( &pItem->m_Mesh.m_bUseVB, sizeof(BYTE));
	pTFILE->Write( &dwLevel, sizeof(DWORD));

	pTFILE->Write( &pItem->m_Mesh.m_vCenterPoint, sizeof(D3DXVECTOR3));
	pTFILE->Write( &pItem->m_Mesh.m_fRadius, sizeof(FLOAT));

	if( pItem->m_Mesh.m_dwNodeCount > 0 )
		pTFILE->Write( pItem->m_Mesh.m_pBones, pItem->m_Mesh.m_dwNodeCount * sizeof(D3DXMATRIX));

	if(pItem->m_Mesh.m_pDATA)
	{
		pTFILE->Write( &pItem->m_Mesh.m_pDATA->m_dwCount, sizeof(DWORD));
		pTFILE->Write( pItem->m_Mesh.m_pDATA->m_pVB, pItem->m_Mesh.m_pDATA->m_dwCount * (pItem->m_Mesh.m_dwNodeCount > 0 ? sizeof(WMESHVERTEX) : sizeof(MESHVERTEX)));
	}
	else
	{
		DWORD dwCount = 0;
		pTFILE->Write( &dwCount, sizeof(DWORD));
	}

	for( int i=0; i<pItem->m_Mesh.m_dwMeshCount; i++)
		for( int j=0; j<dwLevel; j++)
		{
			DWORD dwCount = DWORD(pItem->m_Mesh.m_pMESH[i][j]->m_vIB.size());
			pTFILE->Write( &dwCount, sizeof(DWORD));

			for( DWORD k=0; k<dwCount; k++)
			{
				LPIBDATA pDATA = pItem->m_Mesh.m_pMESH[i][j]->m_vIB[k];

				pTFILE->Write( &pDATA->m_dwCount, sizeof(DWORD));
				pTFILE->Write( &pDATA->m_wMatrix, sizeof(WORD));

				if( pDATA->m_dwCount > 0 )
					pTFILE->Write( pDATA->m_pIB, pDATA->m_dwCount * sizeof(WORD));
			}
		}

	for( i=0; i<INT(dwLevel) - 1; i++)
		pTFILE->Write( &pItem->m_Mesh.m_vDist[i], sizeof(FLOAT));

	pItem->ReleaseData();
}

void CTBuilderDlg::BuildOBJ( COBJItem *pItem,
							 CFile *pTFILE)
{
	MAPATTRITEM::iterator itATTR;
	MAPSFXINST::iterator itSFX;
	MAPSNDINST::iterator itSND;
	MAPACTION::iterator itACT;
	MAPCLKIND::iterator itCLK;
	MAPSTR::iterator itSTR;

	BYTE bPivotCount = BYTE(pItem->GetPivotCount() + 1);
	pTFILE->Write( &bPivotCount, sizeof(BYTE));

	int nCount = INT(pItem->m_mapPIVOT.size());
	pTFILE->Write( &nCount, sizeof(int));

	for( itSTR = pItem->m_mapPIVOT.begin(); itSTR != pItem->m_mapPIVOT.end(); itSTR++)
	{
		DWORD dwID = CItemBase::GetID((*itSTR).second);

		pTFILE->Write( &(*itSTR).first, sizeof(DWORD));
		pTFILE->Write( &dwID, sizeof(DWORD));
	}

	nCount = INT(pItem->m_OBJ.m_mapSFX.size());
	pTFILE->Write( &nCount, sizeof(int));

	for( itSFX = pItem->m_OBJ.m_mapSFX.begin(); itSFX != pItem->m_OBJ.m_mapSFX.end(); itSFX++)
	{
		MAPDWORD::iterator itID = pItem->m_mapSFX.find((*itSFX).first);

		pTFILE->Write( &(*itID).first, sizeof(DWORD));
		pTFILE->Write( &(*itID).second, sizeof(DWORD));
		pTFILE->Write( &(*itSFX).second->m_vPOS, sizeof(D3DXVECTOR3));
		pTFILE->Write( &(*itSFX).second->m_vROT, sizeof(D3DXVECTOR3));
		pTFILE->Write( &(*itSFX).second->m_nPivotID, sizeof(int));
		pTFILE->Write( &(*itSFX).second->m_bBias, sizeof(BYTE));
		pTFILE->Write( &(*itSFX).second->m_dwFuncID, sizeof(DWORD));
		pTFILE->Write( &(*itSFX).second->m_dwTick, sizeof(DWORD));
		pTFILE->Write( &(*itSFX).second->m_bDeleteOnStop, sizeof(BOOL));
#ifdef HEBA
		int nHEBACount = (*itSFX).second->m_mapHideByAction.size();
		pTFILE->Write( &nHEBACount, sizeof(int));

		MAPDWORD::iterator itHEBA, endHEBA;
		itHEBA = (*itSFX).second->m_mapHideByAction.begin();
		endHEBA = (*itSFX).second->m_mapHideByAction.end();
		for(; itHEBA != endHEBA ; ++itHEBA )
		{
			MAPSTR::iterator itSTR = pItem->m_mapNAME.find(itHEBA->first);
			DWORD dwHEBA = CItemBase::GetID((*itSTR).second);
			pTFILE->Write( &dwHEBA, sizeof(DWORD));
		}
#endif
	}

	nCount = INT(pItem->m_OBJ.m_mapSND.size());
	pTFILE->Write( &nCount, sizeof(int));

	for( itSND = pItem->m_OBJ.m_mapSND.begin(); itSND != pItem->m_OBJ.m_mapSND.end(); itSND++)
	{
		pTFILE->Write( &(*itSND).first, sizeof(DWORD));
		pTFILE->Write( &(*itSND).second->m_dwMediaID, sizeof(DWORD));
		pTFILE->Write( &(*itSND).second->m_dwFuncID, sizeof(DWORD));
		pTFILE->Write( &(*itSND).second->m_dwTick, sizeof(DWORD));
		pTFILE->Write( &(*itSND).second->m_bDeleteOnStop, sizeof(BOOL));
	}

	nCount = INT(pItem->m_mapATTR.size());
	pTFILE->Write( &nCount, sizeof(int));

	for( itATTR = pItem->m_mapATTR.begin(); itATTR != pItem->m_mapATTR.end(); itATTR++)
	{
		itSTR = pItem->m_mapNAME.find((*itATTR).first);
		DWORD dwATTR = CItemBase::GetID((*itSTR).second);

		pTFILE->Write( &dwATTR, sizeof(DWORD));
		pTFILE->Write( &(*itATTR).second->m_bType, sizeof(BYTE));

		switch((*itATTR).second->m_bType)
		{
		case ATTRTYPE_INT		:
			{
				int nValue = atoi((*itATTR).second->m_strValue);
				int nSize = sizeof(int);

				pTFILE->Write( &nSize, sizeof(int));
				pTFILE->Write( &nValue, nSize);
			}

			break;

		case ATTRTYPE_FLOAT		:
			{
				FLOAT fValue = (FLOAT) atof((*itATTR).second->m_strValue);
				int nSize = sizeof(FLOAT);

				pTFILE->Write( &nSize, sizeof(int));
				pTFILE->Write( &fValue, nSize);
			}

			break;

		case ATTRTYPE_STRING	:
			{
				int nSize = (*itATTR).second->m_strValue.GetLength();

				pTFILE->Write( &nSize, sizeof(int));
				pTFILE->Write( LPCSTR((*itATTR).second->m_strValue), nSize);
			}

			break;
		}
	}

	nCount = INT(pItem->m_OBJ.m_mapACTION.size());
	pTFILE->Write( &nCount, sizeof(int));

	for( itACT = pItem->m_OBJ.m_mapACTION.begin(); itACT != pItem->m_OBJ.m_mapACTION.end(); itACT++)
	{
		LPACTION pACT = (*itACT).second;
		MAPANI::iterator itANI;

		itSTR = pItem->m_mapNAME.find((*itACT).first);
		DWORD dwACT = CItemBase::GetID((*itSTR).second);

		pTFILE->Write( &dwACT, sizeof(DWORD));
		nCount = INT(pACT->m_mapSFX.size());
		pTFILE->Write( &nCount, sizeof(int));

		for( itSFX = pACT->m_mapSFX.begin(); itSFX != pACT->m_mapSFX.end(); itSFX++)
		{
			MAPDWORD::iterator itID = pItem->m_mapSFX.find((*itSFX).first);

			pTFILE->Write( &(*itID).first, sizeof(DWORD));
			pTFILE->Write( &(*itID).second, sizeof(DWORD));
			pTFILE->Write( &(*itSFX).second->m_vPOS, sizeof(D3DXVECTOR3));
			pTFILE->Write( &(*itSFX).second->m_vROT, sizeof(D3DXVECTOR3));
			pTFILE->Write( &(*itSFX).second->m_nPivotID, sizeof(int));
			pTFILE->Write( &(*itSFX).second->m_bBias, sizeof(BYTE));
			pTFILE->Write( &(*itSFX).second->m_dwFuncID, sizeof(DWORD));
			pTFILE->Write( &(*itSFX).second->m_dwTick, sizeof(DWORD));
			pTFILE->Write( &(*itSFX).second->m_bDeleteOnStop, sizeof(BOOL));
		}

		nCount = INT(pACT->m_mapSND.size());
		pTFILE->Write( &nCount, sizeof(int));

		for( itSND = pACT->m_mapSND.begin(); itSND != pACT->m_mapSND.end(); itSND++)
		{
			pTFILE->Write( &(*itSND).first, sizeof(DWORD));
			pTFILE->Write( &(*itSND).second->m_dwMediaID, sizeof(DWORD));
			pTFILE->Write( &(*itSND).second->m_dwFuncID, sizeof(DWORD));
			pTFILE->Write( &(*itSND).second->m_dwTick, sizeof(DWORD));
			pTFILE->Write( &(*itSND).second->m_bDeleteOnStop, sizeof(BOOL));
		}

		nCount = INT(pACT->m_mapANI.size());
		pTFILE->Write( &nCount, sizeof(int));

		for( itANI = pACT->m_mapANI.begin(); itANI != pACT->m_mapANI.end(); itANI++)
		{
			MAPDWORD::iterator finder = pItem->m_mapANI.find((*itANI).first);

			itSTR = pItem->m_mapNAME.find((*itANI).first);
			DWORD dwANI = CItemBase::GetID((*itSTR).second);

			pTFILE->Write( &dwANI, sizeof(DWORD));
			pTFILE->Write( &(*finder).second, sizeof(DWORD));

			nCount = INT((*itANI).second->m_mapSFX.size());
			pTFILE->Write( &nCount, sizeof(int));

			for( itSFX = (*itANI).second->m_mapSFX.begin(); itSFX != (*itANI).second->m_mapSFX.end(); itSFX++)
			{
				MAPDWORD::iterator itID = pItem->m_mapSFX.find((*itSFX).first);

				pTFILE->Write( &(*itID).first, sizeof(DWORD));
				pTFILE->Write( &(*itID).second, sizeof(DWORD));
				pTFILE->Write( &(*itSFX).second->m_vPOS, sizeof(D3DXVECTOR3));
				pTFILE->Write( &(*itSFX).second->m_vROT, sizeof(D3DXVECTOR3));
				pTFILE->Write( &(*itSFX).second->m_nPivotID, sizeof(int));
				pTFILE->Write( &(*itSFX).second->m_bBias, sizeof(BYTE));
				pTFILE->Write( &(*itSFX).second->m_dwFuncID, sizeof(DWORD));
				pTFILE->Write( &(*itSFX).second->m_dwTick, sizeof(DWORD));
				pTFILE->Write( &(*itSFX).second->m_bDeleteOnStop, sizeof(BOOL));
			}

			nCount = INT((*itANI).second->m_mapSND.size());
			pTFILE->Write( &nCount, sizeof(int));

			for( itSND = (*itANI).second->m_mapSND.begin(); itSND != (*itANI).second->m_mapSND.end(); itSND++)
			{
				pTFILE->Write( &(*itSND).first, sizeof(DWORD));
				pTFILE->Write( &(*itSND).second->m_dwMediaID, sizeof(DWORD));
				pTFILE->Write( &(*itSND).second->m_dwFuncID, sizeof(DWORD));
				pTFILE->Write( &(*itSND).second->m_dwTick, sizeof(DWORD));
				pTFILE->Write( &(*itSND).second->m_bDeleteOnStop, sizeof(BOOL));
			}
		}
	}

	nCount = INT(pItem->m_OBJ.m_mapCLKIND.size());
	pTFILE->Write( &nCount, sizeof(int));

	for( itCLK = pItem->m_OBJ.m_mapCLKIND.begin(); itCLK != pItem->m_OBJ.m_mapCLKIND.end(); itCLK++)
	{
		LPCLKIND pCLK = (*itCLK).second;
		MAPCLOTH::iterator itCL;

		itSTR = pItem->m_mapNAME.find((*itCLK).first);
		DWORD dwCLK = CItemBase::GetID((*itSTR).second);

		pTFILE->Write( &dwCLK, sizeof(DWORD));
		nCount = INT(pCLK->m_mapCLOTH.size());
		pTFILE->Write( &nCount, sizeof(int));

		for( itCL = pCLK->m_mapCLOTH.begin(); itCL != pCLK->m_mapCLOTH.end(); itCL++)
		{
			LPCLOTH pCL = (*itCL).second;
			MAPMESH::iterator itMESH;

			itSTR = pItem->m_mapNAME.find((*itCL).first);
			DWORD dwCL = CItemBase::GetID((*itSTR).second);

			pTFILE->Write( &dwCL, sizeof(DWORD));
			nCount = INT(pCL->m_mapMESH.size());
			pTFILE->Write( &nCount, sizeof(int));

			for( itMESH = pCL->m_mapMESH.begin(); itMESH != pCL->m_mapMESH.end(); itMESH++)
			{
				MAPDWORD::iterator finder = pItem->m_mapMESH.find((*itMESH).first);
				LPMESH pMESH = (*itMESH).second;

				itSTR = pItem->m_mapNAME.find((*itMESH).first);
				DWORD dwMESH = CItemBase::GetID((*itSTR).second);

				pTFILE->Write( &dwMESH, sizeof(DWORD));
				pTFILE->Write( &(*finder).second, sizeof(DWORD));
				nCount = INT(pMESH->m_vTEX.size());
				pTFILE->Write( &nCount, sizeof(int));

				for( int i=0; i<nCount; i++)
				{
					LPOBJTEX pTEX = pMESH->m_vTEX[i];

					DWORD dwTEX = pItem->GetTexID( (*itMESH).first, i, 0);
					finder = pItem->m_mapTEX.find(dwTEX);
					if( finder != pItem->m_mapTEX.end() )
						dwTEX = (*finder).second;
					else
						dwTEX = 0;
					pTFILE->Write( &dwTEX, sizeof(DWORD));

					dwTEX = pItem->GetTexID( (*itMESH).first, i, 1);
					finder = pItem->m_mapTEX.find(dwTEX);
					if( finder != pItem->m_mapTEX.end() )
						dwTEX = (*finder).second;
					else
						dwTEX = 0;
					pTFILE->Write( &dwTEX, sizeof(DWORD));

					pTFILE->Write( &pTEX->m_bType[0], sizeof(BYTE));
					pTFILE->Write( &pTEX->m_bType[1], sizeof(BYTE));

					DWORD dwCOLORID = pItem->GetColorID( (*itMESH).first, i);
					itSTR = pItem->m_mapNAME.find(dwCOLORID);
					if( itSTR != pItem->m_mapNAME.end() )
						dwCOLORID = CItemBase::GetID((*itSTR).second);
					else
						dwCOLORID = 0;
					pTFILE->Write( &dwCOLORID, sizeof(DWORD));

					pTFILE->Write( &pTEX->m_dwCOLOR, sizeof(DWORD));
					pTFILE->Write( &pTEX->m_dwOrder, sizeof(DWORD));
					pTFILE->Write( &pTEX->m_dwOP, sizeof(DWORD));
					pTFILE->Write( &pTEX->m_dwDESTBlend, sizeof(DWORD));
					pTFILE->Write( &pTEX->m_dwSRCBlend, sizeof(DWORD));
					pTFILE->Write( &pTEX->m_dwBlendOP, sizeof(DWORD));
					pTFILE->Write( &pTEX->m_fIntensity, sizeof(FLOAT));
					pTFILE->Write( &pTEX->m_bMinAlpha, sizeof(BYTE));
					pTFILE->Write( &pTEX->m_bZEnable, sizeof(BYTE));
					pTFILE->Write( &pTEX->m_bZWrite, sizeof(BYTE));
					pTFILE->Write( &pTEX->m_b2side, sizeof(BYTE));
					pTFILE->Write( &pTEX->m_bUseDirLight, sizeof(BOOL));
					pTFILE->Write( &pTEX->m_dwAmbientCOLOR, sizeof(DWORD));
				}
			}
		}
	}
}

void CTBuilderDlg::BuildSFX( CSFXItem *pItem,
							 CFile *pTFILE)
{
	BOOL bRet = pItem->LoadData();

	if( !bRet )
		return ;

	CTachyonCompressor cpr;

	int nCount = INT(pItem->m_pSFX->m_vKey.size());

	cpr.Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		cpr.Write( &pItem->m_pSFX->m_vKey[i]->m_dwColor, sizeof(DWORD));
		cpr.Write( &pItem->m_pSFX->m_vKey[i]->m_dwTick, sizeof(DWORD));
	}

	nCount = INT(pItem->m_vSFXID.size());
	cpr.Write( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
		cpr.Write( &pItem->m_vSFXID[i], sizeof(DWORD));

	cpr.Write( &pItem->m_pSFX->m_bSFXType, sizeof(BYTE));
	cpr.Write( &pItem->m_pSFX->m_bSRCType, sizeof(BYTE));
	cpr.Write( &pItem->m_dwSFXSRC, sizeof(DWORD));

	cpr.Write( &pItem->m_dwANISRC, sizeof(DWORD));
	cpr.Write( &pItem->m_pSFX->m_dwLifeTime, sizeof(DWORD));

	cpr.Write( &pItem->m_pSFX->m_fFollow, sizeof(FLOAT));
	cpr.Write( &pItem->m_pSFX->m_fSizeX, sizeof(FLOAT));
	cpr.Write( &pItem->m_pSFX->m_fSizeY, sizeof(FLOAT));

	if( pItem->m_pSFX->m_bSRCType == SFXSRC_OBJ )
	{
		COBJItem *pOBJ = CItemBase::GetOBJItem(pItem->m_dwSFXSRC);
		MAPCLKINST::iterator itCI;

		nCount = INT(pItem->m_pSFX->m_mapCLK.size());
		cpr.Write( &nCount, sizeof(int));
		for( itCI = pItem->m_pSFX->m_mapCLK.begin(); itCI != pItem->m_pSFX->m_mapCLK.end(); itCI++)
		{
			LPCLKINST pCI = (*itCI).second;

			MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find((*itCI).first);
			DWORD dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			cpr.Write( &dwID, sizeof(DWORD));

			itSTR = pOBJ->m_mapNAME.find(pCI->m_dwCurCL);
			dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			cpr.Write( &dwID, sizeof(DWORD));

			itSTR = pOBJ->m_mapNAME.find(pCI->m_dwCurMESH);
			dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			cpr.Write( &dwID, sizeof(DWORD));
		}

		MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find(pItem->m_pSFX->m_dwACT);
		DWORD dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
		cpr.Write( &dwID, sizeof(DWORD));

		itSTR = pOBJ->m_mapNAME.find(pItem->m_pSFX->m_dwANI);
		dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
		cpr.Write( &dwID, sizeof(DWORD));
	}
	else
	{
		nCount = 0;
		cpr.Write( &nCount, sizeof(int));

		cpr.Write( &pItem->m_pSFX->m_dwACT, sizeof(DWORD));
		cpr.Write( &pItem->m_pSFX->m_dwANI, sizeof(DWORD));
	}

	cpr.Write( &pItem->m_pSFX->m_dwOBJFuncID, sizeof(DWORD));
	cpr.Write( &pItem->m_pSFX->m_dwACTFuncID, sizeof(DWORD));
	cpr.Write( &pItem->m_pSFX->m_dwANIFuncID, sizeof(DWORD));

	cpr.Write( &pItem->m_dwSANISRC, sizeof(DWORD));
	cpr.Write( &pItem->m_pSFX->m_bSprayType, sizeof(BYTE));
	cpr.Write( &pItem->m_pSFX->m_bZEnable, sizeof(BYTE));
	cpr.Write( &pItem->m_pSFX->m_bZWrite, sizeof(BYTE));
	cpr.Write( &pItem->m_pSFX->m_bStrict, sizeof(BYTE));
	cpr.Write( &pItem->m_pSFX->m_bFollow, sizeof(BYTE));

	cpr.Write( &pItem->m_pSFX->m_dwDESTBlend, sizeof(DWORD));
	cpr.Write( &pItem->m_pSFX->m_dwSRCBlend, sizeof(DWORD));
	cpr.Write( &pItem->m_pSFX->m_dwBlendOP, sizeof(DWORD));

	cpr.Write( &pItem->m_pSFX->m_dwGTime, sizeof(DWORD));
	cpr.Write( &pItem->m_pSFX->m_dwGCnt, sizeof(DWORD));

	cpr.Write( &pItem->m_pSFX->m_vOutBound, sizeof(D3DXVECTOR3));
	cpr.Write( &pItem->m_pSFX->m_vInBound, sizeof(D3DXVECTOR3));

	cpr.Write( &pItem->m_pSFX->m_dwPLifeTime, sizeof(DWORD));
	cpr.Write( &pItem->m_pSFX->m_dwERRTime, sizeof(DWORD));

	cpr.Write( &pItem->m_pSFX->m_vERRScale, sizeof(D3DXVECTOR3));
	cpr.Write( &pItem->m_pSFX->m_vERRPos, sizeof(D3DXVECTOR3));
	cpr.Write( &pItem->m_pSFX->m_vERRRot, sizeof(D3DXVECTOR3));
	cpr.Write( &pItem->m_pSFX->m_dwLodLevel, sizeof(DWORD));

	cpr.ToFile(pTFILE);

	pItem->ReleaseData();
}

void CTBuilderDlg::BuildEXTMAP( CEXTMAPItem *pItem,
							    CFile *pTFILE)
{
	CString strPATH = pTFILE->GetFilePath();
	BYTE vDETAIL[256];
	BYTE bINDEX = 0;

	strPATH = strPATH.Left(strPATH.GetLength() - pTFILE->GetFileName().GetLength());
	pTFILE->Write( &pItem->m_bMAPType, sizeof(BYTE));
	pTFILE->Write( &pItem->m_pHugeMAP->m_nUnitCountX, sizeof(int));
	pTFILE->Write( &pItem->m_pHugeMAP->m_nUnitCountZ, sizeof(int));
	pTFILE->Write( &pItem->m_pHugeMAP->m_nUnitLength, sizeof(int));
	pTFILE->Write( &pItem->m_pHugeMAP->m_nTileLength, sizeof(int));
	pTFILE->Write( &pItem->m_pHugeMAP->m_nCellCount, sizeof(int));
	pTFILE->Write( &pItem->m_pHugeMAP->m_nTextureSize, sizeof(int));
	pTFILE->Write( &pItem->m_pHugeMAP->m_nShadowPixel, sizeof(int));
	pTFILE->Write( &pItem->m_pHugeMAP->m_vLIGHT, sizeof(D3DXVECTOR3));

	pTFILE->Write( &pItem->m_pHugeMAP->m_bUseSHADER, sizeof(BYTE));
	pTFILE->Write( &pItem->m_pHugeMAP->m_fIntensity, sizeof(FLOAT));
	pTFILE->Write( &pItem->m_pHugeMAP->m_fSpecRange, sizeof(FLOAT));
	pTFILE->Write( &pItem->m_pHugeMAP->m_fSpecMax, sizeof(FLOAT));

	int nCount = INT(pItem->m_vDETAIL.size());
	for( int i=0; i<nCount; i++)
	{
		CTextureItem *pTEX = CItemBase::GetTextureItem(pItem->m_vDETAIL[i]);

		if(pTEX)
		{
			vDETAIL[i] = bINDEX;
			bINDEX++;
		}
		else
			vDETAIL[i] = TILE_NULL;
	}

	pTFILE->Write( &bINDEX, sizeof(BYTE));
	for( i=0; i<nCount; i++)
		if( vDETAIL[i] != TILE_NULL )
		{
			pTFILE->Write( &pItem->m_vDETAILS[i], sizeof(FLOAT));
			pTFILE->Write( &pItem->m_vDETAIL[i], sizeof(DWORD));
		}

	nCount = pItem->m_pHugeMAP->m_nUnitLength;
	vDETAIL[TILE_NULL] = TILE_NULL;

	FLOAT *pHEIGHT = new FLOAT[(nCount + 1) * (nCount + 1)];
	LPWORD pSHADOW = new WORD[nCount * nCount];
	LPBYTE pREGION = new BYTE[nCount * nCount];
	LPBYTE pDETAIL = new BYTE[nCount * nCount];

	nCount = pItem->m_pHugeMAP->m_nUnitCountX * pItem->m_pHugeMAP->m_nUnitCountZ;
	pTFILE->Write( pItem->m_pHugeMAP->m_pENABLE, nCount * sizeof(BYTE));

	for( i=0; i<nCount; i++)
		if(pItem->m_pHugeMAP->m_pENABLE[i])
		{
			LPDIRECT3DTEXTURE9 pSHADE = NULL;
			LPDIRECT3DTEXTURE9 pSKIN = NULL;
			LPDIRECT3DTEXTURE9 pTILE = NULL;

			MAPMAPOBJ::iterator itOBJ;
			MAPMAPSFX::iterator itSFX;
			MAPMAPSND::iterator itSND;

			MAPMAPOBJ mapOBJ;
			MAPMAPSFX mapSFX;
			MAPMAPSND mapSND;

			MAPDWORD mapOBJID;
			MAPDWORD mapSFXID;

			VECTORTEXTURE vSHADOW;
			CString strUNIT;

			strUNIT.Format( "%sEM%d_%d.tmu", strPATH, pItem->m_dwID, i);
			CFile unit( strUNIT, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

			mapOBJ.clear();
			mapSFX.clear();
			mapSND.clear();

			mapOBJID.clear();
			mapSFXID.clear();
			vSHADOW.clear();

			pItem->LoadUNIT( &pSHADE, &pSKIN, &pTILE, &vSHADOW, pREGION, pDETAIL, pHEIGHT, i, FALSE);
			pItem->LoadOBJ(
				&mapOBJ,
				&mapSFX,
				&mapSND,
				&mapOBJID,
				&mapSFXID, i);

			int nSIZE = (pItem->m_pHugeMAP->m_nUnitLength + 1) * (pItem->m_pHugeMAP->m_nUnitLength + 1);
			unit.Write( &nSIZE, sizeof(int));

			if( nSIZE > 0 )
				unit.Write( pHEIGHT, nSIZE * sizeof(FLOAT));

			nSIZE = pItem->m_pHugeMAP->m_nUnitLength * pItem->m_pHugeMAP->m_nUnitLength;
			unit.Write( &nSIZE, sizeof(int));

			if( nSIZE > 0 )
				unit.Write( pREGION, nSIZE * sizeof(BYTE));

			pItem->BuildSREGION( &vSHADOW, pSHADOW);
			unit.Write( &nSIZE, sizeof(int));

			if( nSIZE > 0 )
				unit.Write( pSHADOW, nSIZE * sizeof(WORD));

			unit.Write( &nSIZE, sizeof(int));
			for( int j=0; j<nSIZE; j++)
				unit.Write( &vDETAIL[pDETAIL[j]], sizeof(BYTE));

			nSIZE = INT(mapOBJ.size());
			unit.Write( &nSIZE, sizeof(int));

			for( itOBJ = mapOBJ.begin(); itOBJ != mapOBJ.end(); itOBJ++)
			{
				MAPDWORD::iterator itID = mapOBJID.find((*itOBJ).first);
				COBJItem *pOBJ = CItemBase::GetOBJItem((*itID).second);
				LPMAPOBJ pMAPOBJ = (*itOBJ).second;

				MAPATTRIBUTE::iterator itATTR;
				MAPCLKINST::iterator itCI;

				pItem->SyncOBJATTR( pOBJ, pMAPOBJ);
				unit.Write( &(*itOBJ).first, sizeof(DWORD));
				unit.Write( &(*itID).second, sizeof(DWORD));

				unit.Write( &pMAPOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
				unit.Write( &pMAPOBJ->m_vPOS, sizeof(D3DXVECTOR3));
				unit.Write( &pMAPOBJ->m_vROT, sizeof(D3DXVECTOR3));

				unit.Write( &pMAPOBJ->m_dwOBJFuncID, sizeof(DWORD));
				unit.Write( &pMAPOBJ->m_dwACTFuncID, sizeof(DWORD));
				unit.Write( &pMAPOBJ->m_dwANIFuncID, sizeof(DWORD));

				MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find(pMAPOBJ->m_dwCurACT);
				DWORD dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
				unit.Write( &dwID, sizeof(DWORD));

				itSTR = pOBJ->m_mapNAME.find(pMAPOBJ->m_dwCurANI);
				dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
				unit.Write( &dwID, sizeof(DWORD));

				int nCI = INT(pMAPOBJ->m_mapCLK.size());
				unit.Write( &nCI, sizeof(int));

				for( itCI = pMAPOBJ->m_mapCLK.begin(); itCI != pMAPOBJ->m_mapCLK.end(); itCI++)
				{
					LPCLKINST pCI = (*itCI).second;

					itSTR = pOBJ->m_mapNAME.find((*itCI).first);
					DWORD dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
					unit.Write( &dwID, sizeof(DWORD));

					itSTR = pOBJ->m_mapNAME.find(pCI->m_dwCurCL);
					dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
					unit.Write( &dwID, sizeof(DWORD));

					itSTR = pOBJ->m_mapNAME.find(pCI->m_dwCurMESH);
					dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
					unit.Write( &dwID, sizeof(DWORD));
				}

				int nATTR = (INT) pMAPOBJ->m_mapATTR.size();
				unit.Write( &nATTR, sizeof(int));

				for( itATTR = pMAPOBJ->m_mapATTR.begin(); itATTR != pMAPOBJ->m_mapATTR.end(); itATTR++)
				{
					itSTR = pOBJ->m_mapNAME.find((*itATTR).first);
					DWORD dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
					unit.Write( &dwID, sizeof(DWORD));

					unit.Write( &(*itATTR).second->m_bValueType, sizeof(BYTE));
					unit.Write( &(*itATTR).second->m_nSize, sizeof(int));
					unit.Write( (*itATTR).second->m_pValue, (*itATTR).second->m_nSize);
				}

				delete pMAPOBJ;
			}

			nSIZE = INT(mapSFX.size());
			unit.Write( &nSIZE, sizeof(int));

			for( itSFX = mapSFX.begin(); itSFX != mapSFX.end(); itSFX++)
			{
				MAPDWORD::iterator itID = mapSFXID.find((*itSFX).first);
				LPMAPSFX pMAPSFX = (*itSFX).second;

				unit.Write( &(*itSFX).first, sizeof(DWORD));
				unit.Write( &(*itID).second, sizeof(DWORD));
				unit.Write( &pMAPSFX->m_vPOS, sizeof(D3DXVECTOR3));
				unit.Write( &pMAPSFX->m_vROT, sizeof(D3DXVECTOR3));
				unit.Write( &pMAPSFX->m_bBias, sizeof(BYTE));

				delete pMAPSFX;
			}

			nSIZE = INT(mapSND.size());
			unit.Write( &nSIZE, sizeof(int));

			for( itSND = mapSND.begin(); itSND != mapSND.end(); itSND++)
			{
				LPMAPSND pMAPSND = (*itSND).second;

				unit.Write( &(*itSND).first, sizeof(DWORD));
				unit.Write( &pMAPSND->m_dwMediaID, sizeof(DWORD));
				unit.Write( &pMAPSND->m_vPOS, sizeof(D3DXVECTOR3));

				delete pMAPSND;
			}

			mapOBJ.clear();
			mapSFX.clear();
			mapSND.clear();

			mapOBJID.clear();
			mapSFXID.clear();

			if( pSKIN && pSHADE )
			{
				pItem->Blend( pSKIN, pSHADE, 0, 0);
				LPT3DDATA pDXT = CD3DImage::CreateDXT(
					CItemBase::m_Device.m_pDevice,
					pSKIN, DXT1);

				unit.Write( &pDXT->m_dwSize, sizeof(DWORD));
				unit.Write( pDXT->m_pData, pDXT->m_dwSize);

				delete pDXT;
			}
			else
			{
				DWORD dwLength = 0;
				unit.Write( &dwLength, sizeof(DWORD));
			}

			if(pTILE)
			{
				LPT3DDATA pDXT = CD3DImage::CreateDXT(
					CItemBase::m_Device.m_pDevice,
					pTILE, DXT1);

				unit.Write( &pDXT->m_dwSize, sizeof(DWORD));
				unit.Write( pDXT->m_pData, pDXT->m_dwSize);

				delete pDXT;
			}
			else
			{
				DWORD dwLength = 0;
				unit.Write( &dwLength, sizeof(DWORD));
			}

			nSIZE = INT(vSHADOW.size());
			unit.Write( &nSIZE, sizeof(int));

			for( j=0; j<nSIZE; j++)
			{
				LPT3DDATA pDXT = CD3DImage::CreateDXT(
					CItemBase::m_Device.m_pDevice,
					vSHADOW[j], DXT1);
				vSHADOW[j]->Release();

				unit.Write( &pDXT->m_dwSize, sizeof(DWORD));
				unit.Write( pDXT->m_pData, pDXT->m_dwSize);

				delete pDXT;
			}
			vSHADOW.clear();

			if(pTILE)
				pTILE->Release();

			if(pSHADE)
				pSHADE->Release();

			if(pSKIN)
				pSKIN->Release();
		}

	delete[] pHEIGHT;
	delete[] pSHADOW;
	delete[] pREGION;
	delete[] pDETAIL;
}

void CTBuilderDlg::BuildBSPMAP( CBSPMAPItem *pItem,
							    CFile *pTFILE)
{
	MAPMAPLIGHT::iterator itLIGHT;
	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;
	MAPDWORD::iterator itID;

	if( !pItem->LoadData() )
		return ;

	pItem->LoadDATA();

	pTFILE->Write( &pItem->m_bMAPType, sizeof(BYTE));
	pItem->m_BSPMAP.SaveMAP(pTFILE);

	int nCount = INT(pItem->m_vTEX.size());
	pTFILE->Write( pItem->m_BSPMAP.m_pRegionID, pItem->m_BSPMAP.m_dwMeshCount * sizeof(BYTE));
	pTFILE->Write( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		int nTEXCount = INT(pItem->m_vTEX[i]->size());
		pTFILE->Write( &nTEXCount, sizeof(int));

		for( itID = pItem->m_vTEX[i]->begin(); itID != pItem->m_vTEX[i]->end(); itID++)
		{
			pTFILE->Write( &(*itID).first, sizeof(DWORD));
			pTFILE->Write( &(*itID).second, sizeof(DWORD));
		}
	}

	nCount = INT(pItem->m_mapLIGHT.size());
	pTFILE->Write( &nCount, sizeof(int));

	for( itLIGHT = pItem->m_mapLIGHT.begin(); itLIGHT != pItem->m_mapLIGHT.end(); itLIGHT++)
	{
		pTFILE->Write( &(*itLIGHT).first, sizeof(DWORD));
		pTFILE->Write( &(*itLIGHT).second->m_dwTextureID, sizeof(DWORD));
	}

	nCount = INT(pItem->m_BSPMAP.m_mapOBJ.size());
	pTFILE->Write( &nCount, sizeof(int));

	for( itOBJ = pItem->m_BSPMAP.m_mapOBJ.begin(); itOBJ != pItem->m_BSPMAP.m_mapOBJ.end(); itOBJ++)
	{
		LPMAPOBJ pMAPOBJ = (*itOBJ).second;
		DWORD dwID = (*itOBJ).first;
		itID = pItem->m_mapOBJ.find(dwID);

		COBJItem *pOBJ = CItemBase::GetOBJItem((*itID).second);
		MAPATTRIBUTE::iterator itATTR;
		MAPCLKINST::iterator itCI;

		pItem->SyncOBJATTR( pOBJ, pMAPOBJ);
		pTFILE->Write( &dwID, sizeof(DWORD));
		pTFILE->Write( &(*itID).second, sizeof(DWORD));

		pTFILE->Write( &pMAPOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
		pTFILE->Write( &pMAPOBJ->m_vPOS, sizeof(D3DXVECTOR3));
		pTFILE->Write( &pMAPOBJ->m_vROT, sizeof(D3DXVECTOR3));

		pTFILE->Write( &pMAPOBJ->m_dwOBJFuncID, sizeof(DWORD));
		pTFILE->Write( &pMAPOBJ->m_dwACTFuncID, sizeof(DWORD));
		pTFILE->Write( &pMAPOBJ->m_dwANIFuncID, sizeof(DWORD));

		MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find(pMAPOBJ->m_dwCurACT);
		dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
		pTFILE->Write( &dwID, sizeof(DWORD));

		itSTR = pOBJ->m_mapNAME.find(pMAPOBJ->m_dwCurANI);
		dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
		pTFILE->Write( &dwID, sizeof(DWORD));

		int nCI = INT(pMAPOBJ->m_mapCLK.size());
		pTFILE->Write( &nCI, sizeof(int));

		for( itCI = pMAPOBJ->m_mapCLK.begin(); itCI != pMAPOBJ->m_mapCLK.end(); itCI++)
		{
			LPCLKINST pCI = (*itCI).second;

			itSTR = pOBJ->m_mapNAME.find((*itCI).first);
			dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			pTFILE->Write( &dwID, sizeof(DWORD));

			itSTR = pOBJ->m_mapNAME.find(pCI->m_dwCurCL);
			dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			pTFILE->Write( &dwID, sizeof(DWORD));

			itSTR = pOBJ->m_mapNAME.find(pCI->m_dwCurMESH);
			dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			pTFILE->Write( &dwID, sizeof(DWORD));
		}

		int nATTR = (INT) pMAPOBJ->m_mapATTR.size();
		pTFILE->Write( &nATTR, sizeof(int));

		for( itATTR = pMAPOBJ->m_mapATTR.begin(); itATTR != pMAPOBJ->m_mapATTR.end(); itATTR++)
		{
			itSTR = pOBJ->m_mapNAME.find((*itATTR).first);
			dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			pTFILE->Write( &dwID, sizeof(DWORD));

			pTFILE->Write( &(*itATTR).second->m_bValueType, sizeof(BYTE));
			pTFILE->Write( &(*itATTR).second->m_nSize, sizeof(int));
			pTFILE->Write( (*itATTR).second->m_pValue, (*itATTR).second->m_nSize);
		}
	}

	nCount = INT(pItem->m_BSPMAP.m_mapSFX.size());
	pTFILE->Write( &nCount, sizeof(int));

	for( itSFX = pItem->m_BSPMAP.m_mapSFX.begin(); itSFX != pItem->m_BSPMAP.m_mapSFX.end(); itSFX++)
	{
		LPMAPSFX pMAPSFX = (*itSFX).second;

		itID = pItem->m_mapSFX.find((*itSFX).first);
		pTFILE->Write( &(*itSFX).first, sizeof(DWORD));
		pTFILE->Write( &(*itID).second, sizeof(DWORD));

		pTFILE->Write( &pMAPSFX->m_vPOS, sizeof(D3DXVECTOR3));
		pTFILE->Write( &pMAPSFX->m_vROT, sizeof(D3DXVECTOR3));
		pTFILE->Write( &pMAPSFX->m_bBias, sizeof(BYTE));
	}

	nCount = INT(pItem->m_BSPMAP.m_mapSND.size());
	pTFILE->Write( &nCount, sizeof(int));

	for( itSND = pItem->m_BSPMAP.m_mapSND.begin(); itSND != pItem->m_BSPMAP.m_mapSND.end(); itSND++)
	{
		LPMAPSND pMAPSND = (*itSND).second;

		pTFILE->Write( &(*itSND).first, sizeof(DWORD));
		pTFILE->Write( &pMAPSND->m_dwMediaID, sizeof(DWORD));
		pTFILE->Write( &pMAPSND->m_vPOS, sizeof(D3DXVECTOR3));
	}

	pItem->ClearDATA();
	pItem->ReleaseData();
}

void CTBuilderDlg::PrintText( CStdioFile& file, CString strText)
{
	CString strValue;

	strValue.Format( "\"%s\"", strText);
	file.WriteString(strValue);
}

void CTBuilderDlg::PrintText( CStdioFile& file,
							  CString strTag,
							  CString strText)
{
	CString strValue;

	strValue.Format( "%s=\"%s\"", strTag, strText);
	file.WriteString(strValue);
}

void CTBuilderDlg::PrintInt( CStdioFile& file, int nData)
{
	CString strValue;

	strValue.Format( "%lu", nData);
	file.WriteString(strValue);
}

void CTBuilderDlg::PrintInt( CStdioFile& file,
							 CString strTag,
							 int nData)
{
	CString strValue;

	strValue.Format( "%s=%lu", strTag, nData);
	file.WriteString(strValue);
}

void CTBuilderDlg::PrintDWORD( CStdioFile& file,
							   CString strTag,
							   DWORD dwData)
{
	CString strValue;

	strValue.Format( "%s=%lu", strTag, dwData);
	file.WriteString(strValue);
}

void CTBuilderDlg::PrintOpenTag( CStdioFile& file, CString strTag)
{
	CString strValue;

	strValue.Format( "<%s>", strTag);
	file.WriteString(strValue);
}

void CTBuilderDlg::PrintCloseTag( CStdioFile& file, CString strTag)
{
	CString strValue;

	strValue.Format( "</%s>", strTag);
	file.WriteString(strValue);
}

void CTBuilderDlg::PrintNewLine( CStdioFile& file, int nCount)
{
	for( int i=0; i<nCount; i++)
	{
		CString strValue;

		strValue.Format("\n");
		file.WriteString(strValue);
	}
}

void CTBuilderDlg::PrintTab( CStdioFile& file, int nCount)
{
	for( int i=0; i<nCount; i++)
	{
		CString strValue;

		strValue.Format("\t");
		file.WriteString(strValue);
	}
}

void CTBuilderDlg::PrintTime( CStdioFile& file)
{
	CTime time = CTime::GetCurrentTime();
	CString strValue;

	strValue.Format(
		"/* Tachyon Component Markup Language ( %d/%d/%d %02d:%02d ) */",
		time.GetYear(),
		time.GetMonth(),
		time.GetDay(),
		time.GetHour(),
		time.GetMinute());

	file.WriteString(strValue);
}

CString CTBuilderDlg::GetAlign( BYTE bAlign)
{
	switch(bAlign)
	{
	case GSTA_LEFT		: return _T("ALIGN_LEFT"); break;
	case GSTA_CENTER	: return _T("ALIGN_CENTER"); break;
	case GSTA_RIGHT		: return _T("ALIGN_RIGHT"); break;
	}

	return _T("");
}

void CTBuilderDlg::PrintID( CStdioFile& file,
						    CString strID,
							DWORD dwID)
{
	CString strValue;

	strValue.Format( "<DEFINE %s = %d>", strID, dwID);
	file.WriteString(strValue);
}

void CTBuilderDlg::PrintCTRL( CStdioFile& file,
							  LPCTRL pCTRL,
							  LPCOMP pCOMP,
							  BOOL bFRAME,
							  int nTab)
{
	CCOMPItem *pItem = CItemBase::GetCOMPItem(pCOMP->m_dwCompID);
	CString strCTRL(_T(""));

	CString strNotify[TCML_MENU_COUNT] = {
		ATAG_LCLICK,
		ATAG_RCLICK,
		ATAG_DBLCLICK,
		ATAG_LINEUP,
		ATAG_LINEDOWN,
		ATAG_VSCROLL,
		ATAG_HSCROLL,
		ATAG_SEL_CHANGE,
		ATAG_TAB_ACTIVE,
		ATAG_ENTER,
		ATAG_ESC};

	switch(pItem->m_bCtrlType)
	{
	case CT_NONE		:
		if(!pItem->m_vCOMP.empty())
		{
			int nCount = INT(pItem->m_vCOMP.size());

			for( int i=0; i<nCount; i++)
			{
				PrintCTRL(
					file,
					pCTRL,
					pItem->m_vCOMP[i],
					FALSE,
					nTab);
			}

			return;
		}
		else
			strCTRL.Format(HTAG_COMPONENT);

		break;

	case CT_BUTTON		: strCTRL.Format(HTAG_BUTTON); break;
	case CT_SCROLL		: strCTRL.Format(HTAG_SCROLL); break;
	case CT_EDIT		: strCTRL.Format(HTAG_EDIT); break;
	case CT_LIST		: strCTRL.Format(HTAG_LIST); break;
	case CT_COMBO		: strCTRL.Format(HTAG_COMBO); break;
	case CT_GAUGE		: strCTRL.Format(HTAG_GAUGE); break;
	case CT_IMGLIST		: strCTRL.Format(HTAG_IMAGELIST); break;
	case CT_METER		: strCTRL.Format(HTAG_METER); break;
	case CT_TABCTRL		: strCTRL.Format(HTAG_TABCTRL); break;
	}

	CImageItem *pDefault = CItemBase::GetImageItem(pItem->m_dwDefaultID);
	CImageItem *pDisable = CItemBase::GetImageItem(pItem->m_dwDisableID);
	CString strDefaultID(_T(""));
	CString strDisableID(_T(""));
	CString strAlign(_T(""));

	if(bFRAME)
		strAlign = GetAlign(pCTRL->m_bAlign);

	if(pDefault)
		strDefaultID.Format( "ID_IMAGE_%d", pDefault->m_dwID);

	if(pDisable)
		strDisableID.Format( "ID_IMAGE_%d", pDisable->m_dwID);

	PrintTab( file, nTab);
	file.WriteString(MTAG_OPENBRACE);
	file.WriteString(strCTRL);
	file.WriteString(MTAG_SPACE);

	PrintText( file, ATAG_ID, bFRAME ? pCTRL->m_strCtrlID : pItem->m_strID);
	file.WriteString(MTAG_SPACE);

	if(!strDefaultID.IsEmpty())
	{
		PrintText( file, ATAG_DEFAULT, strDefaultID);
		file.WriteString(MTAG_SPACE);
	}

	if(!strDisableID.IsEmpty())
	{
		PrintText( file, ATAG_DISABLE, strDisableID);
		file.WriteString(MTAG_SPACE);
	}

	if( bFRAME && !pCTRL->m_strText.IsEmpty() )
	{
		PrintText( file, ATAG_TEXT, pCTRL->m_strText);
		file.WriteString(MTAG_SPACE);
	}

	if( bFRAME && !pCTRL->m_strToolTip.IsEmpty() )
	{
		PrintText( file, ATAG_TOOLTIP, pCTRL->m_strToolTip);
		file.WriteString(MTAG_SPACE);
	}

	if(!strAlign.IsEmpty())
	{
		PrintText( file, ATAG_ALIGN, strAlign);
		file.WriteString(MTAG_SPACE);
	}

	if( pItem->m_nHMargine > 0 )
	{
		PrintInt( file, ATAG_HMARGINE, pItem->m_nHMargine);
		file.WriteString(MTAG_SPACE);
	}

	if( pItem->m_nVMargine > 0 )
	{
		PrintInt( file, ATAG_VMARGINE, pItem->m_nVMargine);
		file.WriteString(MTAG_SPACE);
	}

	if( pItem->m_nWidth > 0 )
	{
		PrintInt( file, ATAG_WIDTH, pItem->m_nWidth);
		file.WriteString(MTAG_SPACE);
	}

	if( pItem->m_nHeight > 0 )
	{
		PrintInt( file, ATAG_HEIGHT, pItem->m_nHeight);
		file.WriteString(MTAG_SPACE);
	}

	if( pCOMP->m_nPosX > 0 )
	{
		PrintInt( file, ATAG_POSX, pCOMP->m_nPosX);
		file.WriteString(MTAG_SPACE);
	}

	if( pCOMP->m_nPosY > 0 )
	{
		PrintInt( file, ATAG_POSY, pCOMP->m_nPosY);
		file.WriteString(MTAG_SPACE);
	}

	if( pCOMP->m_dwMediaID > 0 )
	{
		PrintInt( file, ATAG_SOUND, pCOMP->m_dwMediaID);
		file.WriteString(MTAG_SPACE);
	}

	if( bFRAME && !pCTRL->m_strFontID.IsEmpty() )
	{
		PrintText( file, ATAG_FONT, pCTRL->m_strFontID);
		file.WriteString(MTAG_SPACE);
	}

	if( bFRAME && !pCTRL->m_strTipID.IsEmpty() )
	{
		PrintText( file, ATAG_TIPFONT, pCTRL->m_strTipID);
		file.WriteString(MTAG_SPACE);
	}

	if( bFRAME && (pCTRL->m_dwFlag&TS_CUSTOM_COLOR) )
	{
		PrintInt( file, ATAG_COLOR, pCTRL->m_dwColor);
		file.WriteString(MTAG_SPACE);
	}

	if(bFRAME)
	{
		for( int i=0; i<TCML_MENU_COUNT; i++)
			if(!pCTRL->m_strMenuID[i].IsEmpty())
			{
				PrintText( file, strNotify[i], pCTRL->m_strMenuID[i]);
				file.WriteString(MTAG_SPACE);
			}
	}

	switch(pItem->m_bCtrlType)
	{
	case CT_METER		:
		if( pItem->m_nLevel > 0 )
		{
			PrintInt( file, ATAG_METER_SUPERLEVEL, pItem->m_nLevel);
			file.WriteString(MTAG_SPACE);
		}

		break;

	case CT_EDIT		:
		if( bFRAME && (pCTRL->m_dwFlag&TES_PASSWORD) )
		{
			PrintInt( file, ATAG_EDIT_PASSWORD, 1);
			file.WriteString(MTAG_SPACE);
		}

		break;

	case CT_BUTTON		:
		if( bFRAME && (pCTRL->m_dwFlag&TBS_STATE_BUTTON) )
		{
			PrintInt( file, ATAG_BUTTON_STATEBUTTON, 1);
			file.WriteString(MTAG_SPACE);
		}

		break;
	}

	int nCount = INT(pItem->m_vCOMP.size());
	for( int i=0; i<nCount; i++)
	{
		CString strType[3] = {
			_T(""),
			_T(""),
			_T("")};

		switch(pItem->m_bCtrlType)
		{
		case CT_NONE		:
		case CT_IMGLIST		:
		case CT_TABCTRL		: break;
		case CT_BUTTON		:
			{
				strType[0].Format(TAG_BUTTON_DOWN);
				strType[1].Format(TAG_BUTTON_UP);
				strType[2].Format(TAG_BUTTON_HOVER);
			}

			break;

		case CT_SCROLL		:
			{
				strType[0].Format(TAG_SCROLL_UL);
				strType[1].Format(TAG_SCROLL_DR);
				strType[2].Format(TAG_SCROLL_SCROLL);
			}

			break;

		case CT_EDIT		:
			{
				strType[0].Format(TAG_EDIT_CARET);
			}

			break;

		case CT_LIST		:
			{
				strType[0].Format(TAG_LIST_ITEM);
				strType[1].Format(TAG_LIST_VERTICAL);
			}

			break;

		case CT_COMBO		:
			{
				strType[0].Format(TAG_COMBO_DROP);
				strType[1].Format(TAG_COMBO_DOWNLIST);
			}

			break;

		case CT_GAUGE		:
			{
				strType[0].Format(TAG_GAUGE_BAR);
			}

			break;

		case CT_METER		:
			{
				strType[0].Format(TAG_METER_SUB);
				strType[1].Format(TAG_METER_SUPER);
			}

			break;
		}

		if( pItem->m_vCOMP[i]->m_bUseage > 0 )
		{
			CCOMPItem *pComp = CItemBase::GetCOMPItem(pItem->m_vCOMP[i]->m_dwCompID);

			PrintText( file, strType[pItem->m_vCOMP[i]->m_bUseage - 1], pComp->m_strID);
			file.WriteString(MTAG_SPACE);
		}
	}

	file.WriteString(MTAG_CLOSEBRACE);
	PrintNewLine(file);

	for( i=0; i<nCount; i++)
	{
		PrintCTRL(
			file,
			pCTRL,
			pItem->m_vCOMP[i],
			FALSE,
			nTab + 1);
	}

	PrintTab( file, nTab);
	PrintCloseTag( file, strCTRL);
	PrintNewLine(file);
}

void CTBuilderDlg::PrintFrame( CStdioFile& file, CFRAMEItem *pFRAME)
{
	CImageItem *pDefault = CItemBase::GetImageItem(pFRAME->m_dwDefaultID);
	CImageItem *pDisable = CItemBase::GetImageItem(pFRAME->m_dwDisableID);
	CString strDefaultID(_T(""));
	CString strDisableID(_T(""));

	if(pDefault)
		strDefaultID.Format( "ID_IMAGE_%d", pDefault->m_dwID);

	if(pDisable)
		strDisableID.Format( "ID_IMAGE_%d", pDisable->m_dwID);

	file.WriteString(MTAG_OPENBRACE);
	file.WriteString(HTAG_FRAME);
	file.WriteString(MTAG_SPACE);

	PrintText( file, ATAG_ID, pFRAME->m_strID);
	file.WriteString(MTAG_SPACE);

	if(!strDefaultID.IsEmpty())
	{
		PrintText( file, ATAG_DEFAULT, strDefaultID);
		file.WriteString(MTAG_SPACE);
	}

	if(!strDisableID.IsEmpty())
	{
		PrintText( file, ATAG_DISABLE, strDisableID);
		file.WriteString(MTAG_SPACE);
	}

	if( pFRAME->m_nWidth > 0 )
	{
		PrintInt( file, ATAG_WIDTH, pFRAME->m_nWidth);
		file.WriteString(MTAG_SPACE);
	}

	if( pFRAME->m_nHeight > 0 )
	{
		PrintInt( file, ATAG_HEIGHT, pFRAME->m_nHeight);
		file.WriteString(MTAG_SPACE);
	}

	if(!pFRAME->m_strFontID.IsEmpty())
	{
		PrintText( file, ATAG_FONT, pFRAME->m_strFontID);
		file.WriteString(MTAG_SPACE);
	}

	file.WriteString(MTAG_CLOSEBRACE);
	PrintNewLine(file);

	int nCount = INT(pFRAME->m_vCTRL.size());
	for( int i=0; i<nCount; i++)
	{
		COMP vCOMP;

		vCOMP.m_dwCompID = pFRAME->m_vCTRL[i]->m_dwCompID;
		vCOMP.m_nPosX = pFRAME->m_vCTRL[i]->m_nPosX;
		vCOMP.m_nPosY = pFRAME->m_vCTRL[i]->m_nPosY;
		vCOMP.m_bUseage = 0;

		PrintCTRL(
			file,
			pFRAME->m_vCTRL[i],
			&vCOMP,
			TRUE, 1);
	}

	PrintCloseTag( file, HTAG_FRAME);
	PrintNewLine( file, 2);
}

void CTBuilderDlg::PrintFontType( CStdioFile& file,
								  LPGSFONT pFONT)
{
	file.WriteString(MTAG_OPENBRACE);
	file.WriteString(ATAG_FONT);
	file.WriteString(MTAG_SPACE);

	PrintText( file, ATAG_ID, pFONT->m_strFontID);
	file.WriteString(MTAG_SPACE);

	PrintText( file, ATAG_FACENAME, pFONT->m_FONT.lfFaceName);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_WIDTH, pFONT->m_FONT.lfWidth);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_HEIGHT, pFONT->m_FONT.lfHeight);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_ESCAPEMENT, pFONT->m_FONT.lfEscapement);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_ORIENTATION, pFONT->m_FONT.lfOrientation);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_WEIGHT, pFONT->m_FONT.lfWeight);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_ITALIC, pFONT->m_FONT.lfItalic);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_UNDERLINE, pFONT->m_FONT.lfUnderline);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_STRIKEOUT, pFONT->m_FONT.lfStrikeOut);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_CHARSET, pFONT->m_FONT.lfCharSet);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_OUTPRECISION, pFONT->m_FONT.lfOutPrecision);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_CLIPPRECISION, pFONT->m_FONT.lfClipPrecision);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_QUALITY, pFONT->m_FONT.lfQuality);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_PITCHANDFAMILY, pFONT->m_FONT.lfPitchAndFamily);
	file.WriteString(MTAG_SPACE);

	PrintDWORD( file, ATAG_FACECLR, pFONT->m_dwColor);
	file.WriteString(MTAG_SPACE);
	file.WriteString(MTAG_CLOSEBRACE);
}

void CTBuilderDlg::InitDevice()
{
	PVERTEX vMESH[4] = {
		{ 0.0f, 0.0f,  0.0f, 0.0f, 0.0f},
		{ 1.0f, 0.0f,  0.0f, 1.0f, 0.0f},
		{ 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
		{ 1.0f, 0.0f, -1.0f, 1.0f, 1.0f}};
	LPPVERTEX pBUF = NULL;

	CWnd *pDeskTop = GetDesktopWindow();
	CRect rcScreen;

	pDeskTop->GetWindowRect(&rcScreen);
	ReleaseDevice();

	CItemBase::m_Device.m_option.m_dwScreenX = rcScreen.Width();
	CItemBase::m_Device.m_option.m_dwScreenY = rcScreen.Height();
	CItemBase::m_Device.m_option.m_bWindowedMode = TRUE;
	CItemBase::m_Device.m_option.m_bUseSHADER = FALSE;
	CItemBase::m_Device.InitDevices(pDeskTop);
	CTachyonRes::m_pDEVICE = &CItemBase::m_Device;

	CD3DLight::SetDevice(CItemBase::m_Device.m_pDevice);
	if(CItemBase::m_pVB)
	{
		CItemBase::m_pVB->Release();
		CItemBase::m_pVB = NULL;
	}

	CItemBase::m_Device.m_pDevice->CreateVertexBuffer(
		4 * sizeof(PVERTEX),
		0, T3DFVF_PVERTEX,
		D3DPOOL_MANAGED,
		&CItemBase::m_pVB, NULL);

	CItemBase::m_pVB->Lock( 0, 4 * sizeof(PVERTEX), (LPVOID *) &pBUF, 0);
	for( int i=0; i<4; i++)
	{
		pBUF[i].m_fPosX = vMESH[i].m_fPosX;
		pBUF[i].m_fPosY = vMESH[i].m_fPosY;
		pBUF[i].m_fPosZ = vMESH[i].m_fPosZ;
		pBUF[i].m_fU = vMESH[i].m_fU;
		pBUF[i].m_fV = vMESH[i].m_fV;
	}
	CItemBase::m_pVB->Unlock();
}

void CTBuilderDlg::ReleaseDevice()
{
	if(CItemBase::m_pVB)
	{
		CItemBase::m_pVB->Release();
		CItemBase::m_pVB = NULL;
	}

	CD3DLight::SetDevice(NULL);
	CTBSPNode::ReleaseCommon();
	CItemBase::m_Device.ReleaseDevice();
}

void CTBuilderDlg::ResetCtrl()
{
	m_cHISTORYTITLE.EnableWindow(m_bUsePrevID);
	m_cHISTORYEDIT.EnableWindow(m_bUsePrevID);
	m_cHISTORY.EnableWindow(m_bUsePrevID);
}

BYTE CTBuilderDlg::UseHISTORY()
{
	return m_bUsePrevID && !m_strHISTORY.IsEmpty() ? TRUE : FALSE;
}

void CTBuilderDlg::OnBnClickedImageCheck()
{
	UpdateData();
}

void CTBuilderDlg::OnBnClickedImgOut()
{
	UpdateData();
}

void CTBuilderDlg::OnBnClickedMediaCheck()
{
	UpdateData();
}

void CTBuilderDlg::OnBnClickedAniCheck()
{
	UpdateData();
}

void CTBuilderDlg::OnBnClickedMeshCheck()
{
	UpdateData();
}

void CTBuilderDlg::OnBnClickedObjectCheck()
{
	UpdateData();
}

void CTBuilderDlg::OnBnClickedSfxCheck()
{
	UpdateData();
}

void CTBuilderDlg::OnBnClickedMapCheck()
{
	UpdateData();
}

void CTBuilderDlg::OnBnClickedAllCheck()
{
	m_cMap.EnableWindow(m_bAllCheck);
	m_cAni.EnableWindow(m_bAllCheck);
	m_cMesh.EnableWindow(m_bAllCheck);
	m_cObj.EnableWindow(m_bAllCheck);
	m_cImage.EnableWindow(m_bAllCheck);
	m_cSfx.EnableWindow(m_bAllCheck);
	m_cMedia.EnableWindow(m_bAllCheck);
	m_cTex.EnableWindow(m_bAllCheck);
	UpdateData();

	if(m_bAllCheck)
	{
		m_bTexCheck = m_bAniCheck = m_bMapCheck = m_bSfxCheck = m_bMediaCheck = m_bMeshCheck = m_bImageCheck = m_bObjCheck = TRUE;
		UpdateData(FALSE);
	}
}

void CTBuilderDlg::OnBnClickedTexCheck()
{
	UpdateData();
}

void CTBuilderDlg::FillRect( LPLISTCRECT pBLANK,
							 LPLISTCRECT pRES,
							 CRect *pDEST,
							 CRect *pSRC,
							 CRect *pRESULT)
{
	int nDestHeight = pDEST->Height();
	int nDestWidth = pDEST->Width();

	int nSrcHeight = pSRC->Height();
	int nSrcWidth = pSRC->Width();

	int nDiffHeight = nDestHeight - nSrcHeight;
	int nDiffWidth = nDestWidth - nSrcWidth;

	pRESULT->left = pDEST->left;
	pRESULT->top = pDEST->top;

	pRESULT->bottom = pRESULT->top + min( nSrcHeight, nDestHeight);
	pRESULT->right = pRESULT->left + min( nSrcWidth, nDestWidth);

	if( nDiffWidth > nDiffHeight )
		if( nDiffHeight > 0 )
		{
			// 대상 세로분할 후 남은 대상 가로분할
			CRect *pRECT = new CRect(pDEST);
			pRECT->left += nSrcWidth;
			pBLANK->push_back(pRECT);

			pRECT = new CRect(pDEST);
			pRECT->right = pRECT->left + nSrcWidth;
			pRECT->top += nSrcHeight;
			pBLANK->push_back(pRECT);
		}
		else if( nDiffWidth > 0 )
		{
			// 대상 세로분할 후 소스 가로분할
			CRect *pRECT = new CRect(pDEST);
			pRECT->left += nSrcWidth;
			pBLANK->push_back(pRECT);

			if( nDiffHeight < 0 )
			{
				pRECT = new CRect(pSRC);
				pRECT->top += nDestHeight;
				pRES->push_back(pRECT);
			}
		}
		else
		{
			// 소스 가로분할 후 남은 소스 세로분할
			if( nDiffHeight < 0 )
			{
				CRect *pRECT = new CRect(pSRC);
				pRECT->top += nDestHeight;
				pRES->push_back(pRECT);
			}

			if( nDiffWidth < 0 )
			{
				CRect *pRECT = new CRect(pSRC);
				pRECT->bottom = pRECT->top + nDestHeight;
				pRECT->left += nDestWidth;
				pRES->push_back(pRECT);
			}
		}
	else if( nDiffWidth > 0 )
	{
		// 대상 가로분할 후 남은 대상 세로분할
		CRect *pRECT = new CRect(pDEST);
		pRECT->top += nSrcHeight;
		pBLANK->push_back(pRECT);

		pRECT = new CRect(pDEST);
		pRECT->bottom = pRECT->top + nSrcHeight;
		pRECT->left += nSrcWidth;
		pBLANK->push_back(pRECT);
	}
	else if( nDiffHeight > 0 )
	{
		// 대상 가로분할 후 소스 세로분할
		CRect *pRECT = new CRect(pDEST);
		pRECT->top += nSrcHeight;
		pBLANK->push_back(pRECT);

		if( nDiffWidth < 0 )
		{
			pRECT = new CRect(pSRC);
			pRECT->left += nDestWidth;
			pRES->push_back(pRECT);
		}
	}
	else
	{
		// 소스 세로분할 후 남은 소스 가로분할
		if( nDiffWidth < 0 )
		{
			CRect *pRECT = new CRect(pSRC);
			pRECT->left += nDestWidth;
			pRES->push_back(pRECT);
		}

		if( nDiffHeight < 0 )
		{
			CRect *pRECT = new CRect(pSRC);
			pRECT->right = pRECT->left + nDestWidth;
			pRECT->top += nDestHeight;
			pRES->push_back(pRECT);
		}
	}
}

D3DFORMAT CTBuilderDlg::GetD3DFormat( BYTE bFormat)
{
	switch(bFormat)
	{
	case NON_COMP	: return D3DFMT_A8R8G8B8;
	case DXT1		: return D3DFMT_DXT1;
	case DXT2		: return D3DFMT_DXT2;
	case DXT3		: return D3DFMT_DXT3;
	case DXT4		: return D3DFMT_DXT4;
	case DXT5		: return D3DFMT_DXT5;
	}

	return D3DFMT_DXT3;
}
