// TChartProtectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TChartProtect.h"
#include "TChartProtectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef vector<CString *>		VTSTRING, *LPVTSTRING;

typedef enum EQUIP_SLOT
{
	ES_PRMWEAPON = 0,
	ES_SNDWEAPON,
	ES_LONGWEAPON,
	ES_HEAD,
	ES_BACK,
	ES_BODY,
	ES_PANTS,
	ES_FOOT,
	ES_HAND,
	ES_NECK,
	ES_LFINGER,
	ES_RFINGER,
	ES_LEAR,
	ES_REAR,
	ES_FACE,
	ES_COSTUME_HAT,
	ES_COSTUME_DRESS,
	ES_COSTUME_ACCESSORY,
	ES_COSTUME_RACE,
	ES_COUNT
} *LPEQUIP_SLOT;

typedef enum CHART_TYPE
{
	CT_ITEM = 0,
	CT_ITEMVISUAL,
	CT_ITEMMAGICSFX,
	CT_ITEMGRADESFX,
	CT_MONSTER,
	CT_RACE,
	CT_HEAD,
	CT_NUDE,
	CT_ACTION,
	CT_SKILL,
	CT_SKILLTREE,
	CT_SKILLFUNCTION,
	CT_REGION,
	CT_INFO,
	CT_MINIMAP,
	CT_NPC_OUTWARDFORM,
	CT_NPC,
	CT_SFX,
	CT_STEP,
	CT_MAP,
	CT_NODE,
	CT_SWITCH,
	CT_GATE,
	CT_BGM,
	CT_ENV,
	CT_LIGHT,
	CT_ACTDEF,
	CT_MENUITEM,
	CT_POPUPMENU,
	CT_SKYBOX,
	CT_FOG,
	CT_LEVEL,
	CT_DYNAMIC,
	CT_ARROWDIR,
	CT_ITEMMAGIC,
	CT_PET,
	CT_PORTAL,
	CT_ITEMATTR,
	CT_ITEMGRADE,
	CT_QUESTITEM,
	CT_SKILLPOINT,
	CT_FORMULA,
	CT_JOINT,
	CT_ITEMGRADEVISUAL,
	CT_HELP,
	CT_HELPLINK,
	CT_GODTOWER,
	CT_GODBALL,
	CT_MONSHOP,
	CT_PORTALREGION,
	CT_MANTLETEXTURE,
	CT_AUCTIONTREE,
	CT_FAMERANK,
	CT_EQUIPCREATECHAR,
	CT_RPS,
	CT_STRING,
	CT_COUNT
} *LPCHART_TYPE;

const CString CHART_STRS[] =
{
	"아이템 챠트",				//CT_ITEM 
	"아이템 비주얼",			//CT_ITEMVISUAL
	"아이템 매직 SFX",			//CT_ITEMMAGICSFX
	"아이템 강화 SFX",			//CT_ITEMGRADESFX
	"몬스터 챠트",				//CT_MONSTER,
	"종족/성별 챠트",			//CT_RACE,
	"머리/얼굴 챠트",			//CT_HEAD,
	"기본의상 챠트",			//CT_NUDE,
	"액션 챠트",				//CT_ACTION,
	"스킬 챠트",				//CT_SKILL,
	"스킬트리 챠트",			//CT_SKILLTREE,
	"스킬펑션 챠트",			//CT_SKILLFUNCTION,
	"리전 챠트",				//CT_REGION,
	"인포 챠트",				//CT_INFO,
	"미니맵 챠트",				//CT_MINIMAP,
	"NPC 외형 챠트",			//CT_NPC_OUTWARDFORM,
	"NPC 챠트",					//CT_NPC,
	"이펙트 챠트",				//CT_SFX,
	"발소리 챠트",				//CT_STEP,
	"맵 챠트",					//CT_MAP,
	"맵 노드 챠트",				//CT_NODE,
	"스위치 챠트",				//CT_SWITCH,
	"게이트 챠트",				//CT_GATE,
	"BGM 챠트",					//CT_BGM,
	"ENV 챠트",					//CT_ENV,
	"라이트 챠트",				//CT_LIGHT,
	"액션레벨 챠트",			//CT_ACTDEF,
	"메뉴아이템 챠트",			//CT_MENUITEM,
	"팝업메뉴 챠트",			//CT_POPUPMENU,
	"Sky박스 챠트",				//CT_SKYBOX,
	"포그 챠트",				//CT_FOG,
	"레벨 차트",				//CT_LEVEL,
	"다이나믹헬프 챠트",		//CT_DYNAMIC,
	"애로우 차트",				//CT_ARROWDIR,
	"아이템 매직 차트",			//CT_ITEMMAGIC,
	"펫 차트",					//CT_PET
	"포탈 차트",				//CT_PORTAL,
	"아이템특성 차트",			//CT_ITEMATTR,
	"아이템등급 차트",			//CT_ITEMGRADE,
	"퀘스트아이템 차트",		//CT_QUESTITEM,
	"스킬포인트 차트",			//CT_SKILLPOINT
	"포뮬라 차트",				//CT_FORMULA
	"조인트 차트",				//CT_JOINT
	"아이템 강화 VISUAL",		//CT_ITEMGRADEVISUAL
	"도움말 차트",				//CT_HELP
	"도움말-퀘스트 연결차트",	//CT_HELPLINK
	"공성타워 차트",			//CT_GODTOWER
	"공성볼 차트",				//CT_GODBALL
	"몬스터샵 차트",			//CT_MONSHOP
	"포탈지역 차트",			//CT_PORTALREGION
	"망토텍스쳐 차트",			//CT_MANTLETEXTURE
	"경매검색트리 차트",		//CT_AUCTIONTREE
	"명성랭킹 차트",			//CT_FAME
	"TEQUIPCREATECHAR",			//CT_EQUIPCREATECHAR
	"가위바위보 차트", //CT_RPS
	"문자열 차트",				//CT_STRING
};

typedef struct tagTCHART	// 챠트를 모아서 변환 
{
	CString	m_strDEST;		// 출력 화일명
	CString	m_strSRC;		// 입력 화일명

	int m_nType;			// 데이터 타입

	tagTCHART()
	{
		m_strDEST.Empty();
		m_strSRC.Empty();
		m_nType = 0;
	};
} TCHART, *LPTCHART;

struct tagTNPC
{
	CString m_strNAME[2];

	DWORD m_dwHouseID;
	DWORD m_dwTempID;
	DWORD m_dwID;

	DWORD m_dwMaxHP;
	DWORD m_dwMapID;
	DWORD m_dwDIR;

	DWORD m_dwCollisionType;
	DWORD m_dwCanSelected;
	DWORD m_dwClassID;
	DWORD m_dwLevel;
	DWORD m_dwType;
	DWORD m_dwLand;

	DWORD m_dwCountryID;
	DWORD m_dwMenuID;
	DWORD m_dwExecID;
	DWORD m_dwMode;
	DWORD m_dwHouseMesh;
	DWORD m_dwDrawGhost;
	DWORD m_dwDrawMark;
	DWORD m_dwDrawName;
	DWORD m_dwQuestID;
	DWORD m_dwItemID;
	DWORD m_dwGlobal;
	DWORD m_dwCamp;

	FLOAT m_fScaleX;
	FLOAT m_fScaleY;
	FLOAT m_fScaleZ;

	FLOAT m_fSizeX;
	FLOAT m_fSizeY;
	FLOAT m_fSizeZ;

	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	tagTNPC()
	{
		for( BYTE i=0; i<2; i++)
			m_strNAME[i].Empty();

		m_dwHouseID = 0;
		m_dwTempID = 0;
		m_dwID = 0;

		m_dwMaxHP = 0;
		m_dwMapID = 0;
		m_dwDIR = 0;

		m_dwCollisionType = 0;
		m_dwCountryID = 0;
		m_dwClassID = 0;
		m_dwLevel = 0;
		m_dwType = 0;

		m_dwCanSelected = TRUE;
		m_dwHouseMesh = FALSE;
		m_dwDrawGhost = FALSE;
		m_dwDrawMark = FALSE;
		m_dwDrawName = TRUE;
		m_dwLand = FALSE;

		m_dwQuestID = 0;
		m_dwItemID = 0;
		m_dwMenuID = 0;
		m_dwExecID = 0;
		m_dwMode = 0;

		m_fScaleX = 1.0f;
		m_fScaleY = 1.0f;
		m_fScaleZ = 1.0f;

		m_fSizeX = 0.0f;
		m_fSizeY = 0.0f;
		m_fSizeZ = 0.0f;

		m_fPosX = 0.0f;
		m_fPosY = 0.0f;
		m_fPosZ = 0.0f;

		m_dwGlobal = 0;
		m_dwCamp = 0;
	};
};


typedef map< DWORD, LPVTNPC>	MAPVTNPC, *LPMAPVTNPC;

#define UNIT_SIZE				((FLOAT) 1024.0f)
#define TMONSKILL_COUNT			((BYTE) 4)
#define TMONSKILL_DEFAULT		((BYTE) 0)

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


// CTChartProtectDlg dialog



CTChartProtectDlg::CTChartProtectDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTChartProtectDlg::IDD, pParent)
, m_nType(0)
, m_strSRC(_T(""))
, m_strDEST(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTChartProtectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_CHART_TYPE, m_nType);
	DDX_Text(pDX, IDC_EDIT_SRC, m_strSRC);
	DDX_Text(pDX, IDC_EDIT_DEST, m_strDEST);
	DDX_Text(pDX, IDC_EDIT_LIST, m_strLIST);

	DDX_Control(pDX, IDC_BUTTON_SRC, m_cSRC);
	DDX_Control(pDX, IDC_BUTTON_DEST, m_cDEST);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_cLIST);
	DDX_Control(pDX, IDC_COMBO_CHART_TYPE, m_ComboBox);
}

BEGIN_MESSAGE_MAP(CTChartProtectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DEST, OnBnClickedButtonDest)
	ON_BN_CLICKED(IDC_BUTTON_SRC, OnBnClickedButtonSrc)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, OnBnClickedButtonConvert)
	ON_CBN_SELCHANGE(IDC_COMBO_CHART_TYPE, OnCbnSelchangeComboChartType)
	ON_BN_CLICKED(IDC_BUTTON_LIST, OnBnClickedButtonList)
	ON_BN_CLICKED(IDC_BUTTON_LIST_CONVERT, OnBnClickedButtonListConvert)
END_MESSAGE_MAP()


// CTChartProtectDlg message handlers

BOOL CTChartProtectDlg::OnInitDialog()
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
	m_cDEST.SetIcon(hIcon);
	m_cSRC.SetIcon(hIcon);
	m_cLIST.SetIcon(hIcon);

	for(INT i=0; i<CT_COUNT; ++i )
		m_ComboBox.AddString( CHART_STRS[i] );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTChartProtectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTChartProtectDlg::OnPaint() 
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
HCURSOR CTChartProtectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CTChartProtectDlg::Tokenize( CString strTEXT,
									CString strTOK,
									int& nPOS)
{
	CString strRESULT(_T(""));

	int nLENGTH = strTEXT.GetLength();
	int nIndex = nPOS;

	if( nIndex < 0 || nLENGTH <= nIndex )
	{
		nPOS = -1;
		return strRESULT;
	}

	nIndex = strTEXT.Find( strTOK, nPOS);
	if( nIndex < 0 )
	{
		strRESULT = strTEXT.Mid(nPOS);
		nIndex = nLENGTH;
	}
	else
	{
		strRESULT = strTEXT.Mid( nPOS, nIndex - nPOS);
		nIndex += strTOK.GetLength();
	}
	nPOS = nIndex;

	return strRESULT;
}


bool CTChartProtectDlg::LoopLine(const CString& strSrc, int& nPos, CString& outLine)
{
	static const CString 	SPACESTR(" ");
	static const CString 	RETMARKSTR("\\n");
	static const int		RETMARKLEN(2);

	int nLen = strSrc.GetLength();
	if( nPos >= nLen )
		return false;

	int nFind = nLen;
	for(int i=nPos; i<nLen-1; ++i)
	{
		if( strSrc.Mid(i,2) == RETMARKSTR )
		{
			nFind = i;
			break;
		}
	}

	if( nFind != nPos )
		outLine = strSrc.Mid(nPos, nFind-nPos);
	else
		outLine = SPACESTR;

	nPos = nFind+RETMARKLEN;
	return true;
}

void CTChartProtectDlg::OnBnClickedButtonDest()
{
	CFileDialog dlg(
		FALSE,
		_T("*.tcd"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("T-Project Chart Data Files (*.tcd)|*.tcd||"));

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

void CTChartProtectDlg::OnBnClickedButtonSrc()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Chart Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

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

void CTChartProtectDlg::OnCbnSelchangeComboChartType()
{
	UpdateData();
}

void CTChartProtectDlg::OnBnClickedButtonConvert()
{
	switch(m_nType)
	{
	case CT_MONSTER			: ConvertMonsterChart();		break;
	case CT_ITEM			: ConvertItemChart();			break;
	case CT_ITEMVISUAL		: ConvertItemVisualChar();		break;
	case CT_ITEMMAGICSFX	: ConvertItemMagicSfxChar();	break;
	case CT_ITEMGRADESFX	: ConvertItemGradeSfxChar();	break;
	case CT_ITEMGRADEVISUAL	: ConvertItemGradeVisualChart(); break;
	case CT_RACE			: ConvertRaceChart(); 			break;
	case CT_HEAD			: ConvertHeadChart(); 			break;
	case CT_NUDE			: ConvertNudeChart(); 			break;
	case CT_ACTION			: ConvertActionChart();			break;
	case CT_SKILL			: ConvertSkillChart();			break;
	case CT_SKILLTREE		: ConvertSkillTreeChart();		break;
	case CT_SKILLFUNCTION	: ConvertSkillFunctionChart();	break;
	case CT_REGION			: ConvertRegionChart();			break;
	case CT_INFO			: ConvertInfoChart();			break;
	case CT_MINIMAP			: ConvertMinimapChart();		break;
	case CT_NPC_OUTWARDFORM	: ConvertNPCOutwardChart();		break;
	case CT_NPC				: ConvertNPCChart();			break;
	case CT_SFX				: ConvertSFXChart();			break;
	case CT_STEP			: ConvertSTEPChart();			break;
	case CT_MAP				: ConvertMAPChart();			break;
	case CT_NODE			: ConvertNODEChart();			break;
	case CT_SWITCH			: ConvertSwitchChart();			break;
	case CT_GATE			: ConvertGateChart();			break;
	case CT_BGM				: ConvertBGMChart();			break;
	case CT_ENV				: ConvertENVChart();			break;
	case CT_LIGHT			: ConvertLIGHTChart();			break;
	case CT_ACTDEF			: ConvertActionDefChart();		break;
	case CT_MENUITEM		: ConvertMenuItemChart();		break;
	case CT_POPUPMENU		: ConvertPopupMenuChart();		break;
	case CT_SKYBOX			: ConvertSkyBoxChart();			break;
	case CT_FOG				: ConvertFogChart();			break;
	case CT_LEVEL			: ConvertLevelChart();			break;
	case CT_DYNAMIC			: ConvertDynamicHelpChart();			break;
	case CT_ARROWDIR		: ConvertArrowDIRChart();		break;
	case CT_ITEMMAGIC		: ConvertItemMagicChart();		break;
	case CT_PET				: ConvertPetChart();			break;
	case CT_PORTAL			: ConvertPortalChart();			break;
	case CT_ITEMATTR		: ConvertItemAttrChart();		break;
	case CT_ITEMGRADE		: ConvertItemGradeChart();		break;
	case CT_QUESTITEM		: ConvertQuestItemChart();		break;
	case CT_SKILLPOINT		: ConvertSkillPointChart();		break;
	case CT_FORMULA			: ConvertFormulaChart();		break;
	case CT_JOINT			: ConvertJointChart();			break;
	case CT_HELP			: ConvertHelpChart();			break;
	case CT_HELPLINK		: ConvertHelpLinkChart();		break;
	case CT_GODTOWER		: ConvertGodTowerChart();		break;
	case CT_GODBALL			: ConvertGodBallChart();		break;
	case CT_MONSHOP			: ConvertMonsterShop();			break;
	case CT_PORTALREGION	: ConvertPortalRegion();		break;
	case CT_MANTLETEXTURE: ConvertMantleTexture(); break;
	case CT_AUCTIONTREE		: ConvertAuctionTreeChart();	break;
	case CT_FAMERANK		: ConvertFameRankChart();		break;
	case CT_EQUIPCREATECHAR : ConvertEquipCreateChar();		break;
	case CT_RPS : ConvertRpsChart(); break;
	case CT_STRING			: ConvertStringChart();		break;
	}
}

void CTChartProtectDlg::ConvertPortalRegion()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pNAME = new CString[dwCount];
	for( DWORD i=0; i<dwCount; i++)
		vSRC.ReadString(pNAME[i]);

	for( i=0; i<dwCount; i++)
	{
		vSRC.ReadString(strDATA);
		DWORD dwID;

		sscanf( LPCSTR(strDATA), _T("%d"), &dwID);
		vArchive
			<< (WORD)dwID
			<< pNAME[i];
	}

	delete[] pNAME;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertMantleTexture()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	CString* vDETAIL = new CString[ dwCount ];

	for( DWORD i=0 ; i < dwCount ; i++ )
		vSRC.ReadString( vDETAIL[i] );

	vArchive
		<< WORD(dwCount);

	for( DWORD i=0; i<dwCount; i++)
	{
		vSRC.ReadString(strDATA);
		
		DWORD bRaceID;
		DWORD bSexID;
		DWORD dwDetailTextureID;
		DWORD dwCond;

		sscanf( LPCSTR(strDATA), _T("%d %d %x %d"),
			&bRaceID,
			&bSexID,
			&dwDetailTextureID,
			&dwCond );

		vArchive
			<< (BYTE)( bRaceID )
			<< (BYTE)( bSexID )
			<< dwDetailTextureID
			<< dwCond
			<< vDETAIL[ i ];
	}

	if( vDETAIL )
		delete [] vDETAIL;

	vArchive.Close();
}

void CTChartProtectDlg::ConvertMonsterChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pTITLE = new CString[dwCount];
	CString *pNAME = new CString[dwCount];

	for( DWORD i=0; i<dwCount; i++)
		vSRC.ReadString(pTITLE[i]);

	for( i=0; i<dwCount; i++)
		vSRC.ReadString(pNAME[i]);

	for( i=0; i<dwCount; i++)
	{
		CString strKEY;
		int nPOS = 0;

		DWORD bCanSelected;
		DWORD bCanAttack;
		DWORD bCanTame;
		DWORD bNotNockBack;
		DWORD bSlidingWhenDead;
		DWORD bDrawNameWhenDead;
		DWORD wSpawnSFX;
		DWORD dwSpawnSND;
		DWORD bDrawName;
		DWORD bApplyAI;
		DWORD bIsVisible;
		DWORD bCanFly;
		DWORD dwOBJECT;
		DWORD dwMenuID;
		DWORD wMonID;
		DWORD wKind;
		DWORD bDetect;
		DWORD wFaceicon;

		DWORD wSkill[TMONSKILL_COUNT];
		DWORD dwItemID[ES_COUNT];

		FLOAT fLOST;
		FLOAT fLB;
		FLOAT fAB;

		FLOAT fSize;
		FLOAT fScaleX;
		FLOAT fScaleY;
		FLOAT fScaleZ;

		vSRC.ReadString(strDATA);
		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &wMonID);					///일련번호

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &wKind);					///몬스터종류

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bDetect);				///은신감지

		for( BYTE j=0; j<TMONSKILL_COUNT; j++)				
		{
			strKEY = strDATA.Tokenize( "\t", nPOS);
			sscanf( LPCSTR(strKEY), _T("%d"), &wSkill[j]);			///스킬1~4
		}

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bNotNockBack);			///넉백안됨

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bCanSelected);			///선택가능

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bCanFly);				///비행가능

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bCanAttack);				///공격가능

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bCanTame);				///길들이기 가능

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bIsVisible);				///적에게 보이는지 여부

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bDrawName);				///필드에서 이름이 보이는지 여부

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bApplyAI);				///AI적용여부(방향전환)

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fLB);					///시야

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fLOST);					///추적거리

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fAB);					///공격범위(스킬우선)

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%x"), &dwOBJECT);				///오브젝트ID

		for( j=0; j<ES_COSTUME_HAT; j++)
		{
			strKEY = strDATA.Tokenize( "\t", nPOS);
			sscanf( LPCSTR(strKEY), _T("%d"), &dwItemID[j]);		///장착
		}

		for(; j < ES_COUNT ; ++j ) // 코스튬 아이템은 0으로 초기화
			dwItemID[ j ] = 0;

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwMenuID);				///소환수메뉴

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fSize);					///크기

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fScaleX);				///스케일X

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fScaleY);				///스케일Y

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fScaleZ);				///스케일Z

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &wSpawnSFX);				///스폰SFX

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%x"), &dwSpawnSND);				///스폰효과음

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &wFaceicon);				///이미지

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bSlidingWhenDead);			///죽었을때 밀려남

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bDrawNameWhenDead);			///죽엇을때 이름표시

		vArchive
			<< (WORD)(wMonID)										///일련번호
			<< (WORD)(wKind)										///몬스터종류
			<< pTITLE[i]											///몬스터이름(상단)
			<< pNAME[i]												///몬스터이름(하단)
			<< fLB													///시야
				<< fLOST												///추적거리
				<< fAB													///공격범위(스킬우선)
				<< dwOBJECT;											///오브젝트ID

			for( j=0; j<ES_COSTUME_HAT; j++)
				vArchive << (WORD)(dwItemID[j]);						///장착

			for(; j < ES_COUNT ; ++j ) // 코스튬 아이템은 0으로 초기화
				vArchive << (WORD)(0);

			for( j=0; j<TMONSKILL_COUNT; j++)
				vArchive << (WORD)(wSkill[j]);							///스킬1~4

			vArchive
				<< (BYTE)(bNotNockBack)									///넉백안됨
				<< (BYTE)(bCanSelected)									///선택가능
				<< (BYTE)(bCanFly)										///비행가능
				<< (BYTE)(bCanAttack)									///공격가능
				<< (BYTE)(bDrawName)									///필드에서 이름이 보이는지 여부
				<< (BYTE)(bCanTame)										///길들이기 가능
				<< (BYTE)(bIsVisible)									///적에게 보이는지 여부
				<< (BYTE)(bApplyAI)										///AI적용여부(방향전환)
				<< dwMenuID												///소환수메뉴
				<< fSize												///크기
				<< fScaleX												///스케일X
				<< fScaleY												///스케일Y
				<< fScaleZ												///스케일Z
				<< (WORD)(wSpawnSFX)									///스폰SFX
				<< dwSpawnSND											///스폰효과음
				<< (WORD)(wFaceicon)									///이미지
				<< (BYTE)(bDetect)										///은신탐지
				<< (BYTE) bSlidingWhenDead						/// jkchoi
				<< (BYTE) bDrawNameWhenDead;					/// jkchoi
	}

	delete[] pTITLE;
	delete[] pNAME;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertItemChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pNAME = new CString[dwCount];
	for( DWORD i=0; i<dwCount; i++)
		vSRC.ReadString(pNAME[i]);

	for( i=0; i<dwCount; i++)
	{
		DWORD	wItemID;			
		DWORD	bType;				
		DWORD	bKind;				
		DWORD	wAttrID;			
		DWORD	wUseValue;			
		DWORD	dwSlotID;			
		DWORD	dwClassID;			
		DWORD	bPrmSlotID;		
		DWORD	bSubSlotID;		
		DWORD	bLevel;			
		DWORD	bCanRepair;		
		DWORD	dwDuraMax;		
		DWORD	bRefineMax;		
		FLOAT	fPrice;
		DWORD	dwBasePrice;			
		DWORD	bMinRange;			
		DWORD	bMaxRange;			
		DWORD	bStack;			
		DWORD	bSlotCount;		
		DWORD	bCanGamble;
		DWORD	bGambleProb;		
		DWORD	bDestoryProb;		
		DWORD	bCanGrade;			
		DWORD	bCanMagic;			
		DWORD	bCanRare;
		DWORD	wDelayGroup;
		DWORD	dwDelay;			
		DWORD	bIsSell;
		DWORD	bIsSpecial;
		DWORD	wUseTime;
		DWORD	bUseType;
		DWORD	bWeaponID;			
		FLOAT	fShotSpeed;		
		FLOAT	fGravity;			
		DWORD	dwInfoID;			
		DWORD	bShotType;			
		DWORD	wVisual[5];		
		DWORD	wGradeSFX;			
		DWORD	wOptionSFX[3];
		DWORD bCanWrap;
		DWORD	dwAuctionCode;
		DWORD bCanColor;

		vSRC.ReadString(strDATA);
		sscanf( LPCSTR(strDATA), _T("%d %d %d %d %d %d %d %d %d %d %d %d %d %f %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %f %f %d %d %d %d %d %d %d %d %d %d %d %d %d %d"),
			&wItemID,						///일련번호
			&bType,							///종류
			&bKind,							///구분
			&wAttrID,						///성능일련번호
			&wUseValue,						///사용효과값
			&dwSlotID,						///장착위치
			&dwClassID,						///사용직업
			&bPrmSlotID,					///주무기장착위치
			&bSubSlotID,					///보조무기장착위치
			&bLevel,						///필요레벨
			&bCanRepair,					///수리여부(추가)
			&dwDuraMax,						///최대내구(추가)
			&bRefineMax,					///제련횟수(추가)
			&fPrice,						/// 가격비율
			&dwBasePrice,					///기준가격
			&bMinRange,						///최소사정거리
			&bMaxRange,						///최대사정거리
			&bStack,						///최대수량
			&bSlotCount,					///슬롯갯수
			&bCanGamble,					///봉인 생성여부
			&bGambleProb,					///치환여부(추가)
			&bDestoryProb,					///소멸여부(추가)
			&bCanGrade,						///등급 가능여부
			&bCanMagic,						///마법 생성여부
			&bCanRare,						///희귀 생성여부
			&wDelayGroup,					///딜레이 그룹 ID
			&dwDelay,						///재사용대기시간
			&bIsSell,						///거래,판매 가능 여부
			&bIsSpecial,					///캐쉬 아이템 여부
			&wUseTime,						///사용 기간(일/시간)
			&bUseType,						///사용 타입
			&bWeaponID,						///WEAPON ID
			&fShotSpeed,					///SHOT SPEED
			&fGravity,						///GRAVITY
			&dwInfoID,						///INFO ID
			&bShotType,						///발사타입
			&wVisual[0],					///0단계 비주얼(추가)
			&wVisual[1],					///1단계 비주얼(추가)
			&wVisual[2],					///2단계 비주얼(추가)
			&wVisual[3],					///3단계 비주얼(추가)
			&wVisual[4],					///4단계 비주얼(추가)
			&wGradeSFX,						///등급 이펙트(추가)
			&wOptionSFX[0],					///0단계 옵션 이펙트(추가)
			&wOptionSFX[1],					///1단계 옵션 이펙트(추가)
			&wOptionSFX[2],					///2단계 옵션 이펙트(추가)
			&bCanWrap,						///밀랍가능
			&dwAuctionCode,				///경매전용ID
			&bCanColor );				///염색가능

		vArchive
			<< (WORD)wItemID				///일련번호
			<< (BYTE)bType					///종류
			<< (BYTE)bKind					///구분
			<< (WORD)wAttrID				///성능일련번호
			<< pNAME[i]						///이름
			<< (WORD)wUseValue				///사용효과값
				<< (DWORD)dwSlotID				///장착위치
				<< (DWORD)dwClassID				///사용직업
				<< (BYTE)bPrmSlotID				///주무기장착위치
				<< (BYTE)bSubSlotID				///보조무기장착위치
				<< (BYTE)bLevel					///필요레벨
				<< (BYTE)bCanRepair				///수리여부(추가)
				<< (DWORD)dwDuraMax				///최대내구(추가)
				<< (BYTE)bRefineMax				///제련횟수(추가)
				<< (FLOAT)fPrice				///가격비율
				<< (DWORD)dwBasePrice			///기준가격
				<< (BYTE)bMinRange				///최소사정거리
				<< (BYTE)bMaxRange				///최대사정거리
				<< (BYTE)bStack					///최대수량
				<< (BYTE)bSlotCount				///슬롯갯수
				<< (BYTE)bCanGamble				///봉인 생성여부
				<< (BYTE)bGambleProb			///치환여부(추가)
				<< (BYTE)bDestoryProb			///소멸여부(추가)
				<< (BYTE)bCanGrade				///등급 가능여부
				<< (BYTE)bCanMagic				///마법 생성여부
				<< (BYTE)bCanRare				///희귀 생성여부
				<< (WORD)wDelayGroup			///딜레이 그룹
				<< (DWORD)dwDelay				///재사용대기시간
				<< (BYTE)bIsSell				///거래판매 가능 여부
				<< (BYTE)bIsSpecial				///캐쉬 아이템 여부
				<< (WORD)wUseTime				///사용 기간(일/시간)
				<< (BYTE)bUseType				///사용 타입
				<< (BYTE)bWeaponID				///WEAPON ID
				<< (FLOAT)fShotSpeed			///SHOT SPEED
				<< (FLOAT)fGravity				///GRAVITY
				<< (DWORD)dwInfoID				///INFO ID
				<< (BYTE)bShotType				///발사타입
				<< (WORD)wVisual[0]				///0단계 비주얼(추가)
				<< (WORD)wVisual[1]				///1단계 비주얼(추가)
				<< (WORD)wVisual[2]				///2단계 비주얼(추가)
				<< (WORD)wVisual[3]				///3단계 비주얼(추가)
				<< (WORD)wVisual[4]				///4단계 비주얼(추가)
				<< (WORD)wGradeSFX				///등급 이펙트(추가)
				<< (WORD)wOptionSFX[0]			///0단계 옵션 이펙트(추가)
				<< (WORD)wOptionSFX[1]			///1단계 옵션 이펙트(추가)
				<< (WORD)wOptionSFX[2]			///2단계 옵션 이펙트(추가)
				<< (BYTE) bCanWrap						///밀랍가능
				<< (DWORD)dwAuctionCode				///염색가능
				<< (BYTE) bCanColor;					///염색가능
	}

	delete[] pNAME;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertItemVisualChar()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	for(DWORD i=0; i<dwCount; i++)
	{
		DWORD	wID;	
		DWORD	dwInvenID;
		DWORD	dwObjectID;
		DWORD	dwCLKID;
		DWORD	dwCLIID;
		DWORD	dwNormalMesh;
		DWORD	dwBattleMesh;
		DWORD	dwNormalPivot;
		DWORD	dwBattlePivot;
		DWORD	wIcon;
		DWORD	bHideSlot;
		DWORD	bHidePart;
		DWORD	bHideRace;
		DWORD	dwSlashColor;
		DWORD	dwSlashTex;
		FLOAT	fSlashLen;
		DWORD	dwEffectFuncID[2];
		DWORD dwCostumeHide;

		vSRC.ReadString(strDATA);
		sscanf( LPCSTR(strDATA), _T("%d %x %x %x %x %x %x %x %x %d %d %d %d %x %x %f %d %d %u"),
			&wID,	
			&dwInvenID,
			&dwObjectID,
			&dwCLKID,
			&dwCLIID,
			&dwNormalMesh,
			&dwBattleMesh,
			&dwNormalPivot,
			&dwBattlePivot,
			&wIcon,
			&bHideSlot,
			&bHidePart,
			&bHideRace,
			&dwSlashColor,
			&dwSlashTex,
			&fSlashLen,
			&dwEffectFuncID[0],
			&dwEffectFuncID[1],
			&dwCostumeHide );

		vArchive
			<< (WORD)wID	
			<< (DWORD)dwInvenID
			<< (DWORD)dwObjectID
			<< (DWORD)dwCLKID
			<< (DWORD)dwCLIID
			<< (DWORD)dwNormalMesh
			<< (DWORD)dwBattleMesh
			<< (DWORD)dwNormalPivot
			<< (DWORD)dwBattlePivot
			<< (WORD)wIcon
			<< (BYTE)bHideSlot
			<< (BYTE)bHidePart
			<< (BYTE)bHideRace
			<< (DWORD)dwSlashColor
			<< (DWORD)dwSlashTex
			<< (FLOAT)fSlashLen
			<< (DWORD)dwEffectFuncID[0]
			<< (DWORD)dwEffectFuncID[1]
			<< (DWORD)dwCostumeHide;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertItemMagicSfxChar()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	for(DWORD i=0; i<dwCount; i++)
	{
		CString strKEY;
		int nPOS = 0;

		DWORD m_wID;
		DWORD m_wSFX[15];

		vSRC.ReadString(strDATA);
		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &m_wID);					

		for( BYTE j=0; j<15; j++)				
		{
			strKEY = strDATA.Tokenize( "\t", nPOS);
			sscanf( LPCSTR(strKEY), _T("%d"), &m_wSFX[j]);			
		}

		vArchive
			<< (WORD)m_wID;

		for( BYTE j=0; j<15; j++)
		{
			vArchive
				<< (WORD)m_wSFX[j];
		}
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertItemGradeSfxChar()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	for(DWORD i=0; i<dwCount; i++)
	{
		CString strKEY;
		int nPOS = 0;

		DWORD m_wID;
		DWORD m_wSFX[4];

		vSRC.ReadString(strDATA);
		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &m_wID);					

		for( BYTE j=0; j<4; j++)				
		{
			strKEY = strDATA.Tokenize( "\t", nPOS);
			sscanf( LPCSTR(strKEY), _T("%d"), &m_wSFX[j]);			
		}

		vArchive
			<< (WORD)m_wID;

		for( BYTE j=0; j<4; j++)
		{
			vArchive
				<< (WORD)m_wSFX[j];
		}
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertItemGradeVisualChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	for(DWORD i=0; i<dwCount; i++)
	{
		CString strKEY;
		int nPOS = 0;

		DWORD bKind;	
		DWORD wGrade;
		DWORD dwTextureID;
		DWORD bOPCode;

		vSRC.ReadString(strDATA);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bKind);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &wGrade);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%x"), &dwTextureID);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &bOPCode);

		vArchive
			<< (BYTE) bKind
			<< (WORD) wGrade
			<< (DWORD) dwTextureID
			<< (BYTE) bOPCode;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertRaceChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwRACE;
		DWORD dwSEX;
		DWORD dwOBJECT;
		FLOAT fSCALE;

		sscanf( LPCSTR(strDATA), _T("%d %d %x %f"),
			&dwRACE,
			&dwSEX,
			&dwOBJECT,
			&fSCALE);

		vArchive
			<< BYTE(dwRACE)
			<< BYTE(dwSEX)
			<< dwOBJECT
			<< fSCALE;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertHeadChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwHEAD;
		DWORD dwHAIR;
		DWORD dwCLK;
		DWORD dwCL;
		DWORD dwMESH;

		DWORD dwHideSlotID;
		DWORD dwHidePartID;
		DWORD dwHideRaceID;

		sscanf( LPCSTR(strDATA), _T("%d %d %x %x %x %d %d %d"),
			&dwHEAD,
			&dwHAIR,
			&dwCLK,
			&dwCL,
			&dwMESH,
			&dwHideSlotID,
			&dwHidePartID,
			&dwHideRaceID);

		vArchive
			<< BYTE(dwHEAD)
			<< BYTE(dwHAIR)
			<< dwCLK
			<< dwCL
			<< dwMESH
			<< BYTE(dwHideSlotID)
			<< BYTE(dwHidePartID)
			<< BYTE(dwHideRaceID);
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertNudeChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwNudeID;
		DWORD dwCLK;
		DWORD dwCL;
		DWORD dwMESH;

		DWORD dwHideSlotID;
		DWORD dwHidePartID;
		DWORD dwHideRaceID;

		sscanf( LPCSTR(strDATA), _T("%d %x %x %x %d %d %d"),
			&dwNudeID,
			&dwCLK,
			&dwCL,
			&dwMESH,
			&dwHideSlotID,
			&dwHidePartID,
			&dwHideRaceID);

		vArchive
			<< BYTE(dwNudeID)
			<< dwCLK
			<< dwCL
			<< dwMESH
			<< BYTE(dwHideSlotID)
			<< BYTE(dwHidePartID)
			<< BYTE(dwHideRaceID);
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertActionChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwActionID;
		DWORD dwActID;

		DWORD dwEquipMode;
		DWORD dwSlashPOS;

		sscanf( LPCSTR(strDATA), _T("%d %x %d %d"),
			&dwActionID,
			&dwActID,
			&dwEquipMode,
			&dwSlashPOS);

		vArchive
			<< dwActionID
			<< dwActID
			<< BYTE(dwEquipMode)
			<< dwSlashPOS;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertSkillChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pNAME = new CString[dwCount];
	for( DWORD i=0; i<dwCount; i++)
		vSRC.ReadString(pNAME[i]);

	for( i=0; i<dwCount; i++)
	{
		DWORD bPositive;
		DWORD wSkillID;
		DWORD wImageID;
		DWORD wItemID[2];
		DWORD dwActionTime;
		DWORD dwReuseDelay;
		DWORD dwKindDelay;
		DWORD bLoopDelay;
		DWORD bActive;
		DWORD bSpeedApply;
		DWORD bLoop;
		DWORD bContinue;

		DWORD dwDurationInc;
		DWORD dwDuration;

		DWORD wUseHP;
		DWORD wUseMP;

		DWORD bLevelInc;
		DWORD bLevel;
		DWORD bMaxLevel;

		DWORD bActionID[4];
		DWORD dwSFXID[7];
		DWORD dwInfoID;
		DWORD bTarget;
		DWORD bTargetRange;
		DWORD wPosture;

		DWORD bShowCritical;
		DWORD bShowIcon;
		DWORD bShowTime;

		DWORD wTargetActiveID;
		DWORD wPrevSkill;
		DWORD wNeedItemID;
		DWORD dwClassID;
		DWORD bKind;
		DWORD dwWeaponID;
		DWORD bIsRide;
		DWORD bIsUseInHold;

		FLOAT fAtkRange;
		FLOAT fBufRange;
		FLOAT fMinRange;
		FLOAT fMaxRange;
		FLOAT fMoveDist;
		FLOAT fPrice;

		int nReuseDelayInc;

		DWORD bDuraSlot;
		DWORD bHitInit;
		DWORD bHitInc;
		DWORD bCanCancel;
		DWORD bStaticWhenDie;

		vSRC.ReadString(strDATA);

		sscanf( LPCSTR(strDATA), _T("%d %d %d %d %d %d %d %d %d %d %f %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %f %f %f %f %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %f"),
			&wSkillID,					///스킬ID
			&wPrevSkill,				///이전성공스킬
			&wNeedItemID,				///소모 아이템
			&wTargetActiveID,			///대상이걸린버프
			&dwClassID,					///직업코드
			&bKind,						///특성구분
			&bPositive,					///이로움 해로움
			&bLevel,					///요구레벨
			&bLevelInc,					///요구레벨증가
			&bMaxLevel,					///최대스킬레벨
			&fPrice,					///가격비율
			&bDuraSlot,					///내구감소슬롯(추가)
			&dwWeaponID,				///사용무기
			&wUseHP,					///HP소모비율
			&wUseMP,					///MP소모비율
			&bHitInit,					///성공율초기값
			&bHitInc,					///성공율증가값
			&dwActionTime,				///발동시간
			&bIsRide,					///버프중 탈 수 없는 스킬
			&dwReuseDelay,				///재사용대기
			&nReuseDelayInc,			///재사용 대기 증감비율
			&dwKindDelay,				///계열대기
			&bLoopDelay,				///반복시간
			&bSpeedApply,				///공속적용
			&wPosture,					///태세
			&bTarget,					///사용대상 피아
			&bTargetRange,				///대상판별
			&fMinRange,					///최소사정거리
			&fMaxRange,					///최대사정기리
			&fAtkRange,					///인식 폭,반경
			&fBufRange,					///오라범위
			&dwDuration,				///적용시간초기값
			&dwDurationInc,				///적용시간증가비율
			&bCanCancel,				///사용자 취소 가능 여부
			&bContinue,					///동작지속여부
			&wImageID,					///아이콘ID
			&wItemID[0],				///발사아이템1
			&wItemID[1],				///발사아이템2
			&bActive,					///패시브=0 액티브=1
			&bLoop,						///자동반복여부
			&bActionID[0],				///주문동작
			&bActionID[1],				///발동동작
			&bActionID[2],				///이동발동동작
			&bActionID[3],				///방어동작
			&dwInfoID,					///툴팁ID
			&dwSFXID[0],				///시전이펙트
			&dwSFXID[1],				///발동이펙트
			&dwSFXID[2],				///타겟이펙트
			&dwSFXID[3],				///타겟발동이펙트
			&dwSFXID[4],				///유지이펙트
			&dwSFXID[5],				///해제이펙트
			&dwSFXID[6],				///발사체이펙트
			&bShowIcon,					///아이콘보임
			&bShowTime,					///남은시간보임
			&bShowCritical,				///크리티컬적용
			&bIsUseInHold,				///갇힘 상태에서 사용 가능여부
			&bStaticWhenDie,			/// 사망시 버프 유지
			&fMoveDist);

		vArchive
			<< (WORD)wSkillID			///스킬ID
			<< pNAME[i]					///스킬이름
			<< (WORD)wPrevSkill			///이전성공스킬
				<< (WORD)wNeedItemID		///소모 아이템
				<< (WORD)wTargetActiveID	///대상이걸린버프
				<< (DWORD)dwClassID			///직업코드
				<< (BYTE)bKind				///특성구분
				<< (BYTE)bPositive			///이로움 해로움
				<< (BYTE)bLevel				///요구레벨
				<< (BYTE)bLevelInc			///요구레벨증가
				<< (BYTE)bMaxLevel			///최대스킬레벨
				<< (FLOAT)fPrice			///가격비율
				<< (BYTE)bDuraSlot			///내구감소슬롯(추가)
				<< (DWORD)dwWeaponID		///사용무기
				<< (WORD)wUseHP				///HP소모비율
				<< (WORD)wUseMP				///MP소모비율
				<< (BYTE)bHitInit			///성공율초기값(추가)
				<< (BYTE)bHitInc			///성공율증가값(추가)
				<< (DWORD)dwActionTime		///발동시간
				<< (BYTE)bIsRide			///버프중 탈 수 없는 스킬
				<< (DWORD)dwReuseDelay		///재사용대기
				<< (INT)nReuseDelayInc		///재사용 대기 증감비율
				<< (DWORD)dwKindDelay		///계열대기
				<< (DWORD)bLoopDelay		///반복시간
				<< (BYTE)bSpeedApply		///공속적용
				<< (WORD)wPosture			///태세
				<< (BYTE)bTarget			///사용대상 피아
				<< (BYTE)bTargetRange		///대상판별
				<< (FLOAT)fMinRange			///최소사정거리
				<< (FLOAT)fMaxRange			///최대사정기리
				<< (FLOAT)fAtkRange			///인식 폭,반경
				<< (FLOAT)fBufRange			///오라범위
				<< (DWORD)dwDuration		///적용시간초기값
				<< (DWORD)dwDurationInc		///적용시간증가비율
				<< (BYTE)bCanCancel			///사용자 취소 가능 여부(추가)
				<< (BYTE)bContinue			///동작지속여부
				<< (WORD)wImageID			///아이콘ID
				<< (WORD)wItemID[0]			///발사아이템1
				<< (WORD)wItemID[1]			///발사아이템2
				<< (BYTE)bActive			///패시브=0 액티브=1
					<< (BYTE)bLoop				///자동반복여부
					<< (BYTE)bActionID[0]		///주문동작
					<< (BYTE)bActionID[1]		///발동동작
					<< (BYTE)bActionID[2]		///이동발동동작
					<< (BYTE)bActionID[3]		///방어동작
					<< (DWORD)dwInfoID			///툴팁ID
						<< (DWORD)dwSFXID[0]		///시전이펙트
						<< (DWORD)dwSFXID[1]		///발동이펙트
						<< (DWORD)dwSFXID[2]		///타겟이펙트
						<< (DWORD)dwSFXID[3]		///타겟발동이펙트
						<< (DWORD)dwSFXID[4]		///유지이펙트
						<< (DWORD)dwSFXID[5]		///해제이펙트
						<< (DWORD)dwSFXID[6]		///발사체이펙트
						<< (BYTE)bShowIcon			///아이콘보임
							<< (BYTE)bShowTime			///남은시간보임
							<< (BYTE)bShowCritical		///크리티컬적용
							<< (BYTE)bIsUseInHold		///갇힘 상태에서 사용 가능여부
							<< (BYTE)bStaticWhenDie		///사망시 버프 유지
							<< (FLOAT)fMoveDist;		///순간이동 거리
	}

	delete[] pNAME;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertSkillTreeChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwContryID;
		DWORD dwClassID;
		DWORD dwTabID;
		DWORD dwTreeID;
		DWORD dwSkillID;

		sscanf( LPCSTR(strDATA), _T("%d %d %d %d %d"),
			&dwContryID,
			&dwClassID,
			&dwTabID,
			&dwTreeID,
			&dwSkillID);

		vArchive
			<< BYTE(dwContryID)
			<< BYTE(dwClassID)
			<< BYTE(dwTabID)
			<< BYTE(dwTreeID)
			<< WORD(dwSkillID);
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertSkillFunctionChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD wSkillID;
		DWORD bAction;
		DWORD bType;
		DWORD bExec;
		DWORD bInc;
		DWORD bCalc;
		DWORD wValue;
		DWORD wValueInc;

		sscanf( LPCSTR(strDATA), _T("%d %d %d %d %d %d %d %d"),
			&wSkillID,				///일련번호
			&bAction,				///적용방식
			&bType,					///효과종류
			&bExec,					///효과내용
			&bInc,					///수치적용
			&bCalc,					///성능계산(추가)
			&wValue,				///초기값
			&wValueInc);			///증가값

		vArchive
			<< WORD(wSkillID)		///일련번호
			<< BYTE(bAction)		///적용방식
			<< BYTE(bType)			///효과종류
			<< BYTE(bExec)			///효과내용
			<< BYTE(bInc)			///수치적용
			<< BYTE(bCalc)			///성능계산
			<< WORD(wValue)			///초기값
			<< WORD(wValueInc);		///증가값
	}								

	vArchive.Close();
}

void CTChartProtectDlg::ConvertRegionChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pNAME = new CString[dwCount];
	for( DWORD i=0; i<dwCount; i++)
		vSRC.ReadString(pNAME[i]);

	for( i=0; i<dwCount; i++)
	{
		vSRC.ReadString(strDATA);
		DWORD dwRegionID;
		DWORD dwContryID;
		DWORD dwCanFly;
		DWORD dwLocalID;
		FLOAT fDThumbX;
		FLOAT fDThumbY;
		FLOAT fDThumbZ;
		FLOAT fCThumbX;
		FLOAT fCThumbY;
		FLOAT fCThumbZ;
		FLOAT fBThumbX;
		FLOAT fBThumbY;
		FLOAT fBThumbZ;
		DWORD bCanMail;
		DWORD dwENVLoop;
		DWORD dwBMGID;
		DWORD dwENVID;
		DWORD bInfo;

		sscanf( LPCSTR(strDATA), _T("%d %d %d %d %f %f %f %f %f %f %f %f %f %d %x %d %d %d"),
			&dwRegionID,
			&dwContryID,
			&dwCanFly,
			&dwLocalID,
			&fDThumbX,
			&fDThumbY,
			&fDThumbZ,
			&fCThumbX,
			&fCThumbY,
			&fCThumbZ,
			&fBThumbX,
			&fBThumbY,
			&fBThumbZ,
			&bCanMail,
			&dwENVLoop,
			&dwBMGID,
			&dwENVID,
			&bInfo);

		vArchive
			<< dwRegionID
			<< pNAME[i]
			<< (BYTE)dwContryID
				<< (BYTE)dwCanFly
				<< (WORD)dwLocalID
				<< fDThumbX
				<< fDThumbY
				<< fDThumbZ
				<< fCThumbX
				<< fCThumbY
				<< fCThumbZ
				<< fBThumbX
				<< fBThumbY
				<< fBThumbZ
				<< (BYTE)bCanMail
				<< dwENVLoop
				<< dwBMGID
				<< dwENVID
				<< (BYTE)bInfo;
	}

	delete[] pNAME;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertInfoChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pTINFO = new CString[dwCount];
	for( DWORD i=0; i<dwCount; i++)
		vSRC.ReadString(pTINFO[i]);

	for( i=0; i<dwCount; i++)
	{
		vSRC.ReadString(strDATA);
		int nPOS = 0;

		VTSTRING vTINFO;
		DWORD dwID;
		vTINFO.clear();

		CString strMSG;
		while( LoopLine(pTINFO[i], nPOS, strMSG) )
			vTINFO.push_back(new CString(strMSG));

		sscanf( LPCSTR(strDATA), _T("%d"), &dwID);
		vArchive
			<< dwID
			<< BYTE(vTINFO.size());

		for( BYTE j=0; j<BYTE(vTINFO.size()); j++)
			vArchive << (*vTINFO[j]);

		while(!vTINFO.empty())
		{
			delete vTINFO.back();
			vTINFO.pop_back();
		}
	}

	delete[] pTINFO;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertMinimapChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwUnitID;
		DWORD dwTexID;
		DWORD dwWorldID;
		DWORD dwWorldButton;
		DWORD dwWorldMapLevel;

		FLOAT fRatio;
		FLOAT fPosX;
		FLOAT fPosZ;

		sscanf( LPCSTR(strDATA), _T("%d %x %f %f %f %d %x %u"),
			&dwUnitID,
			&dwTexID,
			&fRatio,
			&fPosX,
			&fPosZ,
			&dwWorldID,
			&dwWorldButton,
			&dwWorldMapLevel);

		vArchive
			<< dwUnitID
			<< dwTexID
			<< fRatio
			<< fPosX
			<< fPosZ
			<< (BYTE)dwWorldID
			<< dwWorldButton
			<< (BYTE)dwWorldMapLevel;
	}

	vArchive.Close();
}

// 06/05/08 PSY
void CTChartProtectDlg::ConvertNPCOutwardChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	for( DWORD i=0; i<dwCount; i++)
	{
		CString strKEY;
		int nPOS = 0;

		DWORD dwItemID[ES_COUNT];
		DWORD dwTempID;
		DWORD dwOBJID;
		DWORD dwRActID[2];
		DWORD dwACT;
		DWORD dwSND;
		DWORD dwFaceIcon;
		DWORD dwBoxAnim;

		vSRC.ReadString(strDATA);
		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwTempID);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%x"), &dwOBJID);

		for( BYTE j=0; j<ES_COSTUME_HAT; j++)
		{
			strKEY = strDATA.Tokenize( "\t", nPOS);
			sscanf( LPCSTR(strKEY), _T("%d"), &dwItemID[j]);
		}

		for(; j < ES_COUNT ; ++j ) // 코스튬 아이템은 0으로 초기화.
			dwItemID[j] = 0;

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf(LPCSTR(strKEY), _T("%d"), &dwACT);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf(LPCSTR(strKEY), _T("%d"), &dwRActID[0]);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf(LPCSTR(strKEY), _T("%d"), &dwRActID[1]);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf(LPCSTR(strKEY), _T("%d"), &dwSND);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf(LPCSTR(strKEY), _T("%d"), &dwFaceIcon);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf(LPCSTR(strKEY), _T("%d"), &dwBoxAnim);

		vArchive
			<< (WORD)dwTempID
			<< dwOBJID;

		for( j=0; j<ES_COSTUME_HAT; j++)
			vArchive << (WORD)dwItemID[j];

		for(; j < ES_COUNT ; ++j ) // 코스튬 아이템은 0으로 초기화.
			vArchive << (WORD)(0);
		

		vArchive
			<< (BYTE)dwACT
			<< (BYTE)dwRActID[0]
			<< (BYTE)dwRActID[1]
			<< dwSND
				<< (WORD)dwFaceIcon
				<< (BYTE)dwBoxAnim;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertNPCChart()
{
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	LPTNPC pTNPC = new TNPC[dwCount];
	MAPVTNPC mapTNPC;
	VTNPC vGlobalTNPC;
	mapTNPC.clear();

	for( DWORD i=0; i<dwCount; i++)
		vSRC.ReadString(pTNPC[i].m_strNAME[0]);

	for( i=0; i<dwCount; i++)
		vSRC.ReadString(pTNPC[i].m_strNAME[1]);

	for( i=0; i<dwCount; i++)
	{
		vSRC.ReadString(strDATA);

		sscanf( LPCSTR(strDATA), _T("%d %d %d %d %d %d %d %d %f %f %f %d %f %f %f %f %f %f %d %d %d %x %d %d %d %d %d %d %d %d %d %d %d"),
			&pTNPC[i].m_dwID,
			&pTNPC[i].m_dwTempID,
			&pTNPC[i].m_dwType,
			&pTNPC[i].m_dwClassID,
			&pTNPC[i].m_dwLevel,
			&pTNPC[i].m_dwCollisionType,
			&pTNPC[i].m_dwMaxHP,
			&pTNPC[i].m_dwMapID,
			&pTNPC[i].m_fPosX,
			&pTNPC[i].m_fPosY,
			&pTNPC[i].m_fPosZ,
			&pTNPC[i].m_dwDIR,
			&pTNPC[i].m_fSizeX,
			&pTNPC[i].m_fSizeY,
			&pTNPC[i].m_fSizeZ,
			&pTNPC[i].m_fScaleX,
			&pTNPC[i].m_fScaleY,
			&pTNPC[i].m_fScaleZ,
			&pTNPC[i].m_dwLand,
			&pTNPC[i].m_dwMode,
			&pTNPC[i].m_dwMenuID,
			&pTNPC[i].m_dwExecID,
			&pTNPC[i].m_dwCountryID,
			&pTNPC[i].m_dwDrawGhost,
			&pTNPC[i].m_dwDrawMark,
			&pTNPC[i].m_dwHouseID,
			&pTNPC[i].m_dwHouseMesh,
			&pTNPC[i].m_dwDrawName,
			&pTNPC[i].m_dwItemID,
			&pTNPC[i].m_dwGlobal,
			&pTNPC[i].m_dwCamp,
			&pTNPC[i].m_dwQuestID,
			&pTNPC[i].m_dwCanSelected);

		if( pTNPC[i].m_dwGlobal == 0 )
		{
			BYTE bUnitX = BYTE(pTNPC[i].m_fPosX / UNIT_SIZE);
			BYTE bUnitZ = BYTE(pTNPC[i].m_fPosZ / UNIT_SIZE);

			DWORD dwID = MAKELONG( pTNPC[i].m_dwMapID ? WORD(0) : MAKEWORD( bUnitX, bUnitZ), WORD(pTNPC[i].m_dwMapID));

			MAPVTNPC::iterator finder = mapTNPC.find(dwID);
			LPVTNPC pUNIT = NULL;

			if( finder == mapTNPC.end() )
			{
				pUNIT = new VTNPC();
				mapTNPC.insert( MAPVTNPC::value_type( dwID, pUNIT));
			}
			else
				pUNIT = (*finder).second;

			pUNIT->push_back(&pTNPC[i]);
		}
		else
			vGlobalTNPC.push_back(&pTNPC[i]);
	}

	CString strNAME = m_strDEST.Left(m_strDEST.ReverseFind('.'));
	MAPVTNPC::iterator it;

	for( it = mapTNPC.begin(); it != mapTNPC.end(); it++)
	{
		LPVTNPC pUNIT = (*it).second;
		CString strDEST;

		strDEST.Format( _T("%s%08x.tcd"), strNAME, (*it).first);
		CFile vDEST( LPCSTR(strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		CArchive vArchive( &vDEST, CArchive::store);

		WriteTNPC(
			&vArchive,
			pUNIT);

		vArchive.Close();
		delete pUNIT;
	}

	strNAME = m_strDEST.Left(m_strDEST.ReverseFind('.'));
	CString strDEST;
	strDEST.Format( _T("%sGlobal.tcd"), strNAME);

	CFile vDEST( LPCSTR(strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CArchive vArchive( &vDEST, CArchive::store);

	WriteTNPC(
		&vArchive,
		&vGlobalTNPC);

	vArchive.Close();
	mapTNPC.clear();

	delete[] pTNPC;
}

void CTChartProtectDlg::WriteTNPC( CArchive *pAR,
								   LPVTNPC pVTNPC)
{
	(*pAR)
		<< WORD(pVTNPC->size());

	while(!pVTNPC->empty())
	{
		LPTNPC pNPC = pVTNPC->back();

		(*pAR)
			<< pNPC->m_dwID
			<< WORD(pNPC->m_dwTempID)
			<< pNPC->m_strNAME[0]
			<< pNPC->m_strNAME[1]
			<< BYTE(pNPC->m_dwType)
			<< BYTE(pNPC->m_dwClassID)
			<< BYTE(pNPC->m_dwLevel)
			<< BYTE(pNPC->m_dwCountryID)
			<< BYTE(pNPC->m_dwCollisionType)
			<< BYTE(pNPC->m_dwCanSelected)
			<< BYTE(pNPC->m_dwLand)
			<< BYTE(pNPC->m_dwMode)
			<< BYTE(pNPC->m_dwDrawGhost)
			<< BYTE(pNPC->m_dwDrawMark)
			<< BYTE(pNPC->m_dwDrawName)
			<< BYTE(pNPC->m_dwHouseMesh)
			<< pNPC->m_dwHouseID
			<< pNPC->m_dwMenuID
			<< pNPC->m_dwExecID
			<< pNPC->m_dwQuestID
			<< WORD(pNPC->m_dwItemID)
			<< pNPC->m_dwMaxHP
			<< FLOAT(FLOAT(pNPC->m_dwDIR) * D3DX_PI / 180.0f)
			<< pNPC->m_fPosX
			<< pNPC->m_fPosY
			<< pNPC->m_fPosZ
			<< pNPC->m_fSizeX
			<< pNPC->m_fSizeY
			<< pNPC->m_fSizeZ
			<< pNPC->m_fScaleX
			<< pNPC->m_fScaleY
			<< pNPC->m_fScaleZ
			<< BYTE(pNPC->m_dwCamp);

		pVTNPC->pop_back();
	}
}

void CTChartProtectDlg::OnBnClickedButtonList()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Chart Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strSRCFOLDER);
	if( dlg.DoModal() == IDOK )
	{
		UpdateData();
		m_strLISTFOLDER = dlg.GetFileName();
		m_strLIST = dlg.GetPathName();
		m_strLISTFOLDER = m_strLIST.Left(m_strLIST.GetLength() - m_strLISTFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTChartProtectDlg::OnBnClickedButtonListConvert()
{
	CStdioFile vSRC( LPCSTR(m_strLIST), CFile::modeRead|CFile::typeText);
	CTShell dlg;

	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	LPTCHART pDATA = new TCHART[dwCount];
	for( DWORD i=0; i<dwCount; i++)
		vSRC.ReadString(pDATA[i].m_strSRC);

	for( i=0; i<dwCount; i++)
	{
		DWORD nType;

		if(vSRC.ReadString(strDATA))
			sscanf( LPCSTR(strDATA), _T("%d"), &nType);

		pDATA[i].m_nType = nType;
	}

	for( i=0; i<dwCount; i++)
		vSRC.ReadString(pDATA[i].m_strDEST);

	dlg.m_strTitle = _T("Image Folder");
	dlg.m_strSelDir = m_strDESTFOLDER;

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		m_strDESTFOLDER = dlg.m_strPath;

		for( DWORD i=0; i<dwCount; i++)
		{
			m_strDEST = dlg.m_strPath + "\\" + pDATA[i].m_strDEST;
			m_nType = pDATA[i].m_nType;
			m_strSRC = pDATA[i].m_strSRC;

			OnBnClickedButtonConvert();
		}
	}

	delete[] pDATA;
}

void CTChartProtectDlg::ConvertSFXChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwSFX;
		DWORD dwID;
		DWORD dwPOS;

		sscanf( LPCSTR(strDATA), _T("%d %x %d"),
			&dwSFX,
			&dwID,
			&dwPOS);

		vArchive
			<< dwSFX
			<< dwID
			<< dwPOS;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertSTEPChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwFUNC;
		DWORD dwID;

		sscanf( LPCSTR(strDATA), _T("%d %d"),
			&dwID,
			&dwFUNC);

		vArchive
			<< dwID
			<< dwFUNC;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertMAPChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwDUNGEON;
		DWORD dwINDEX;
		DWORD dwID;
		DWORD dwNPCCALL=0;

		FLOAT fScaleX;
		FLOAT fScaleY;
		FLOAT fScaleZ;

		FLOAT fPosX;
		FLOAT fPosY;
		FLOAT fPosZ;

		FLOAT fRotX;
		FLOAT fRotY;
		FLOAT fRotZ;

		sscanf( LPCSTR(strDATA), _T("%d %x %f %f %f %f %f %f %f %f %f %d %d"),
			&dwINDEX,
			&dwID,
			&fScaleX,
			&fScaleY,
			&fScaleZ,
			&fPosX,
			&fPosY,
			&fPosZ,
			&fRotX,
			&fRotY,
			&fRotZ,
			&dwDUNGEON,
			&dwNPCCALL );

		fRotX *= D3DX_PI / 180.0f;
		fRotY *= D3DX_PI / 180.0f;
		fRotZ *= D3DX_PI / 180.0f;

		vArchive
			<< dwINDEX
			<< dwID
			<< fScaleX
			<< fScaleY
			<< fScaleZ
			<< fPosX
			<< fPosY
			<< fPosZ
			<< fRotX
			<< fRotY
			<< fRotZ
			<< BYTE(dwDUNGEON)
			<< BYTE(dwNPCCALL);
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertNODEChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwMapID;
		DWORD dwNode;

		FLOAT fPosX;
		FLOAT fPosY;
		FLOAT fPosZ;

		sscanf( LPCSTR(strDATA), _T("%x %d %f %f %f"),
			&dwMapID,
			&dwNode,
			&fPosX,
			&fPosY,
			&fPosZ);

		vArchive
			<< dwMapID
			<< dwNode
			<< fPosX
			<< fPosY
			<< fPosZ;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertSwitchChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	for( DWORD i=0; i<dwCount; i++)
	{
		CString strKEY;
		int nPOS = 0;

		DWORD dwSwitchID;
		DWORD dwMapID;
		DWORD dwOBJ;

		FLOAT fScaleX;
		FLOAT fScaleY;
		FLOAT fScaleZ;

		FLOAT fPosX;
		FLOAT fPosY;
		FLOAT fPosZ;
		FLOAT fDIR;

		DWORD dwItemID[ES_COUNT];
		DWORD dwClose;
		DWORD dwOpen;
		DWORD dwLockOnClose;
		DWORD dwLockOnOpen;

		DWORD dwHouseMesh;
		DWORD dwHouseID;
		DWORD dwSoundID;

		vSRC.ReadString(strDATA);
		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwSwitchID);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwMapID);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fPosX);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fPosY);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fPosZ);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fDIR);
		fDIR *= D3DX_PI / 180.0f;

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fScaleX);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fScaleY);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fScaleZ);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%x"), &dwOBJ);

		for( BYTE j=0; j<ES_COSTUME_HAT; j++)
		{
			strKEY = strDATA.Tokenize( "\t", nPOS);
			sscanf( LPCSTR(strKEY), _T("%d"), &dwItemID[j]);
		}

		for(; j < ES_COUNT; ++j ) // 코스튬 아이템은 0으로 초기화
			dwItemID[j] = 0;

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwClose);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwOpen);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwLockOnOpen);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwLockOnClose);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwHouseID);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwHouseMesh);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%x"), &dwSoundID);

		vArchive
			<< dwSwitchID
			<< WORD(dwMapID)
			<< fPosX
			<< fPosY
			<< fPosZ
			<< fDIR
			<< fScaleX
			<< fScaleY
			<< fScaleZ
			<< dwOBJ;

		for( j=0; j<ES_COSTUME_HAT; j++)
			vArchive << WORD(dwItemID[j]);

		for(; j < ES_COUNT ; ++j ) // 코스튬 아이템은 0으로 초기화
			vArchive << (WORD)(0);

		vArchive
			<< BYTE(dwClose)
			<< BYTE(dwOpen)
			<< BYTE(dwLockOnOpen)
			<< BYTE(dwLockOnClose)
			<< BYTE(dwHouseMesh)
			<< dwHouseID
			<< dwSoundID;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertGateChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	for( DWORD i=0; i<dwCount; i++)
	{
		CString strKEY;
		int nPOS = 0;

		DWORD dwSwitchID;
		DWORD dwGateID;
		DWORD dwMapID;
		DWORD dwOBJ;

		FLOAT fScaleX;
		FLOAT fScaleY;
		FLOAT fScaleZ;

		FLOAT fPosX;
		FLOAT fPosY;
		FLOAT fPosZ;
		FLOAT fDIR;

		DWORD dwItemID[ES_COUNT];
		DWORD dwCloseAct;
		DWORD dwClose;
		DWORD dwOpenAct;
		DWORD dwOpen;
		DWORD dwDeleteOnOpen;
		DWORD dwDeleteOnClose;
		DWORD dwOpenSFX;
		DWORD dwCloseSFX;

		DWORD dwHouseMesh;
		DWORD dwHouseID;
		DWORD dwStair;

		vSRC.ReadString(strDATA);
		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwGateID);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwSwitchID);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwMapID);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fPosX);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fPosY);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fPosZ);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fDIR);
		fDIR *= D3DX_PI / 180.0f;

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fScaleX);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fScaleY);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%f"), &fScaleZ);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%x"), &dwOBJ);

		for( BYTE j=0; j<ES_COSTUME_HAT; j++)
		{
			strKEY = strDATA.Tokenize( "\t", nPOS);
			sscanf( LPCSTR(strKEY), _T("%d"), &dwItemID[j]);
		}
		for(; j < ES_COUNT ; ++j ) //코스튬 아이템은 0으로 초기화
			dwItemID[j] = 0;

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwClose);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwCloseAct);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwOpen);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwOpenAct);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwDeleteOnOpen);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwDeleteOnClose);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwHouseID);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwHouseMesh);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwOpenSFX);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwCloseSFX);

		strKEY = strDATA.Tokenize( "\t", nPOS);
		sscanf( LPCSTR(strKEY), _T("%d"), &dwStair);

		vArchive
			<< dwGateID
			<< dwSwitchID
			<< WORD(dwMapID)
			<< fPosX
			<< fPosY
			<< fPosZ
			<< fDIR
			<< fScaleX
			<< fScaleY
			<< fScaleZ
			<< dwOBJ;

		for( j=0; j<ES_COSTUME_HAT; j++)
			vArchive << WORD(dwItemID[j]);

		for(; j < ES_COUNT ; ++j ) // 코스튬 아이템은 0으로 초기화.
			vArchive << (WORD)(0);

		vArchive
			<< BYTE(dwClose)
			<< BYTE(dwCloseAct)
			<< BYTE(dwOpen)
			<< BYTE(dwOpenAct)
			<< BYTE(dwDeleteOnOpen)
			<< BYTE(dwDeleteOnClose)
			<< BYTE(dwHouseMesh)
			<< dwHouseID
			<< dwOpenSFX
			<< dwCloseSFX
			<< BYTE(dwStair);
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertBGMChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwID;
		DWORD dwBGM;

		sscanf( LPCSTR(strDATA), _T("%d %x"),
			&dwID,
			&dwBGM);

		vArchive
			<< dwID
			<< dwBGM;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertENVChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwID;
		DWORD dwENV;

		sscanf( LPCSTR(strDATA), _T("%d %x"),
			&dwID,
			&dwENV);

		vArchive
			<< dwID
			<< dwENV;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertLIGHTChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwLightID;

		FLOAT fDirX;
		FLOAT fDirY;
		FLOAT fDirZ;

		FLOAT fAmbientR;
		FLOAT fAmbientG;
		FLOAT fAmbientB;

		FLOAT fDiffuseR;
		FLOAT fDiffuseG;
		FLOAT fDiffuseB;

		sscanf( LPCSTR(strDATA), _T("%d %f %f %f %f %f %f %f %f %f"),
			&dwLightID,
			&fDirX,
			&fDirY,
			&fDirZ,
			&fAmbientR,
			&fAmbientG,
			&fAmbientB,
			&fDiffuseR,
			&fDiffuseG,
			&fDiffuseB);

		vArchive
			<< dwLightID
			<< fDirX
			<< fDirY
			<< fDirZ
			<< fAmbientR
			<< fAmbientG
			<< fAmbientB
			<< fDiffuseR
			<< fDiffuseG
			<< fDiffuseB;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertActionDefChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	CString *pACT = new CString[dwCount];
	for( DWORD i=0; i<dwCount; i++)
	{
		vSRC.ReadString(pACT[i]);
	}

	vArchive
		<< WORD(dwCount);

	i = 0;
	while(vSRC.ReadString(strDATA))
	{
		DWORD dwLoopAction;
		DWORD dwSubAction;
		DWORD dwLevel;

		DWORD dwContinue;
		DWORD dwCancel;
		DWORD dwNavAction;
		DWORD dwSkipMain;
		DWORD dwHideOnCapeMode;

		sscanf( LPCSTR(strDATA), _T("%d %d %d %d %d %d %d %d"),
			&dwLevel,
			&dwSubAction,
			&dwLoopAction,
			&dwContinue,
			&dwCancel,
			&dwNavAction,
			&dwSkipMain,
			&dwHideOnCapeMode);

		vArchive
			<< pACT[i++]
			<< BYTE(dwLevel)
			<< BYTE(dwSubAction)
			<< BYTE(dwLoopAction)
			<< BYTE(dwContinue)
			<< BYTE(dwCancel)
			<< BYTE(dwNavAction)
			<< BYTE(dwSkipMain)
			<< BYTE(dwHideOnCapeMode);
	}

	vArchive.Close();
	delete [] pACT;
}

void CTChartProtectDlg::ConvertMenuItemChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pNAME = new CString[dwCount];
	for( DWORD i=0; i<dwCount; i++)
		vSRC.ReadString(pNAME[i]);

	for( i=0; i<dwCount; i++)
	{
		vSRC.ReadString(strDATA);
		DWORD dwItemID;
		DWORD dwMenuID;
		DWORD dwGhost;

		sscanf( LPCSTR(strDATA), _T("%d %x %d"),
			&dwItemID,
			&dwMenuID,
			&dwGhost);

		vArchive
			<< pNAME[i]
			<< dwItemID
				<< dwMenuID
				<< BYTE(dwGhost);
	}

	delete[] pNAME;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertPopupMenuChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwPopupID;
		DWORD dwItemID;

		sscanf( LPCSTR(strDATA), _T("%d %d"),
			&dwPopupID,
			&dwItemID);

		vArchive
			<< dwPopupID
			<< dwItemID;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertSkyBoxChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwTSkyID;
		DWORD dwOBJ;
		DWORD dwCLK;
		DWORD dwCL;
		DWORD dwMESH;
		DWORD dwActID;
		DWORD dwAniID;

		sscanf( LPCSTR(strDATA), _T("%d %x %x %x %x %x %x"),
			&dwTSkyID,
			&dwOBJ,
			&dwCLK,
			&dwCL,
			&dwMESH,
			&dwActID,
			&dwAniID);

		vArchive
			<< dwTSkyID
			<< dwOBJ
			<< dwCLK
			<< dwCL
			<< dwMESH
			<< dwActID
			<< dwAniID;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertFogChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwDefault;
		DWORD dwFogID;
		DWORD dwType;

		DWORD dwValueR;
		DWORD dwValueG;
		DWORD dwValueB;

		FLOAT fRadius;
		FLOAT fRange;
		FLOAT fPosX;
		FLOAT fPosZ;

		FLOAT fDENSITY;
		FLOAT fSTART;
		FLOAT fEND;

		sscanf( LPCSTR(strDATA), _T("%d %d %d %f %f %f %f %d %d %d %f %f %f"),
			&dwFogID,
			&dwDefault,
			&dwType,
			&fPosX,
			&fPosZ,
			&fRange,
			&fRadius,
			&dwValueR,
			&dwValueG,
			&dwValueB,
			&fSTART,
			&fEND,
			&fDENSITY);

		vArchive
			<< dwFogID
			<< BYTE(dwDefault)
			<< BYTE(dwType)
			<< BYTE(dwValueR)
			<< BYTE(dwValueG)
			<< BYTE(dwValueB)
			<< fRadius
			<< fRange
			<< fPosX
			<< fPosZ
			<< fDENSITY
			<< fSTART
			<< fEND;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertLevelChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD bLevel;
		DWORD dwMoney;
		DWORD dwRegCost;
		DWORD dwSearchCost;

		sscanf( LPCSTR(strDATA), _T("%d %d %d %d"),
			&bLevel,
			&dwMoney,
			&dwRegCost,
			&dwSearchCost);

		vArchive
			<< (BYTE)bLevel
			<< (DWORD)dwMoney
			<< (DWORD)dwRegCost
			<< (DWORD)dwSearchCost;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertDynamicHelpChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pNAME = new CString[dwCount];
	for( DWORD i=0; i<dwCount; i++)
		vSRC.ReadString(pNAME[i]);

	for( i=0; i<dwCount; i++)
	{
		vSRC.ReadString(strDATA);
		DWORD dwID;

		sscanf( LPCSTR(strDATA), _T("%d"), &dwID);
		pNAME[i].Replace( _T("\\n"), _T("\n"));

		vArchive
			<< pNAME[i]
			<< dwID;
	}

	delete[] pNAME;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertArrowDIRChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwSkillID;
		DWORD dwDIR;

		sscanf( LPCSTR(strDATA), _T("%d %d"),
			&dwSkillID,
			&dwDIR);

		vArchive
			<< WORD(dwSkillID)
			<< FLOAT(FLOAT(dwDIR) * D3DX_PI / 180.0f);
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertItemMagicChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pNAME = new CString[dwCount];
	for( DWORD i=0; i<dwCount; i++)
	{
		vSRC.ReadString(pNAME[i]);
	}

	for( i=0; i<dwCount; i++)
	{
		vSRC.ReadString(strDATA);

		DWORD dwID;
		DWORD bSecOption;
		FLOAT fUtil;
		DWORD bSFX;

		sscanf( LPCSTR(strDATA), _T("%d %d %f %d"),
			&dwID,
			&bSecOption,
			&fUtil,
			&bSFX);

		vArchive
			<< pNAME[i]
			<< (WORD)dwID
				<< (BYTE)bSecOption
				<< (FLOAT)fUtil
				<< (BYTE)bSFX;
	}

	delete[] pNAME;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertPetChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD wID;
		DWORD wMonID;
		DWORD bRecallKind1;
		DWORD bRecallKind2;
		DWORD wRecallValue1;
		DWORD wRecallValue2;
		DWORD bConditionType;
		DWORD dwConditionValue;
		DWORD wIcon;

		sscanf( LPCSTR(strDATA), _T("%d %d %d %d %d %d %d %d %d"),
			&wID,
			&wMonID,
			&bRecallKind1,
			&bRecallKind2,
			&wRecallValue1,
			&wRecallValue2,
			&bConditionType,
			&dwConditionValue,
			&wIcon);

		vArchive
			<< (WORD)wID
			<< (WORD)wMonID
			<< (BYTE)bRecallKind1
			<< (BYTE)bRecallKind2
			<< (WORD)wRecallValue1
			<< (WORD)wRecallValue2
			<< (BYTE)bConditionType
			<< (DWORD)dwConditionValue
			<< (WORD)wIcon;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertPortalChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pNAME = new CString[dwCount];
	for( DWORD i=0; i<dwCount; i++)
		vSRC.ReadString(pNAME[i]);

	for( i=0; i<dwCount; i++)
	{
		vSRC.ReadString(strDATA);
		DWORD dwID;
		DWORD dwPortalRegionID;

		sscanf( LPCSTR(strDATA), _T("%d %d"), &dwID, &dwPortalRegionID);
		vArchive
			<< (WORD)dwID
			<< pNAME[i]
			<< (WORD)dwPortalRegionID;
	}

	delete[] pNAME;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertItemAttrChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD wID;
		DWORD bGrade;
		DWORD wMinAP;
		DWORD wMaxAP;
		DWORD wDP;
		DWORD wMinMAP;
		DWORD wMaxMAP;
		DWORD wMDP;
		DWORD bBlockProb;
		DWORD bSpeedInc;

		sscanf( LPCSTR(strDATA), _T("%d %d %d %d %d %d %d %d %d %d"),
			&wID,
			&bGrade,
			&wMinAP,
			&wMaxAP,
			&wDP,
			&wMinMAP,
			&wMaxMAP,
			&wMDP,
			&bBlockProb,
			&bSpeedInc);

		vArchive
			<< (WORD)wID
			<< (BYTE)bGrade
			<< (WORD)wMinAP
			<< (WORD)wMaxAP
			<< (WORD)wDP
			<< (WORD)wMinMAP
			<< (WORD)wMaxMAP
			<< (WORD)wMDP
			<< (BYTE)bBlockProb
			<< (BYTE)bSpeedInc;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertItemGradeChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD bLevel;
		DWORD bGrade;

		sscanf( LPCSTR(strDATA), _T("%d %d"),
			&bLevel,
			&bGrade);

		vArchive
			<< (BYTE)bLevel
			<< (BYTE)bGrade;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertQuestItemChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD	dwID;
		DWORD	wItemID;
		DWORD	bLevel;
		DWORD	bGLevel;
		DWORD	dwDuraMax;
		DWORD	dwDuraCur;
		DWORD	bRefineCur;
		DWORD	bMagic1;
		DWORD	bValue1;
		DWORD	bMagic2;
		DWORD	bValue2;
		DWORD	bMagic3;
		DWORD	bValue3;
		DWORD	bMagic4;
		DWORD	bValue4;
		DWORD	bMagic5;
		DWORD	bValue5;
		DWORD	bMagic6;
		DWORD	bValue6;

		sscanf( LPCSTR(strDATA), _T("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"),
			&dwID,
			&wItemID,
			&bLevel,
			&bGLevel,
			&dwDuraMax,
			&dwDuraCur,
			&bRefineCur,
			&bMagic1,
			&bValue1,
			&bMagic2,
			&bValue2,
			&bMagic3,
			&bValue3,
			&bMagic4,
			&bValue4,
			&bMagic5,
			&bValue5,
			&bMagic6,
			&bValue6);

		vArchive
			<< (DWORD)dwID
			<< (WORD)wItemID
			<< (BYTE)bLevel
			<< (BYTE)bGLevel
			<< (DWORD)dwDuraMax
			<< (DWORD)dwDuraCur
			<< (BYTE)bRefineCur
			<< (BYTE)bMagic1
			<< (BYTE)bValue1
			<< (BYTE)bMagic2
			<< (BYTE)bValue2
			<< (BYTE)bMagic3
			<< (BYTE)bValue3
			<< (BYTE)bMagic4
			<< (BYTE)bValue4
			<< (BYTE)bMagic5
			<< (BYTE)bValue5
			<< (BYTE)bMagic6
			<< (BYTE)bValue6;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertSkillPointChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD wID;						//스킬일련번호
		DWORD bLevel;					//스킬레벨
		DWORD bSkillPoint;				//필요스킬포인트
		DWORD bGroupPoint;				//필요계열포인트
		DWORD bPrevSkillLevel;			//이전스킬레벨

		sscanf( LPCSTR(strDATA), _T("%d %d %d %d %d"),
			&wID,						//스킬일련번호
			&bLevel,					//스킬레벨
			&bSkillPoint,				//필요스킬포인트
			&bGroupPoint,				//필요계열포인트
			&bPrevSkillLevel);			//이전스킬레벨

		vArchive
			<< (WORD)wID				//스킬일련번호
			<< (BYTE)bLevel				//스킬레벨
			<< (BYTE)bSkillPoint		//필요스킬포인트
			<< (BYTE)bGroupPoint		//필요계열포인트
			<< (BYTE)bPrevSkillLevel;	//이전스킬레벨
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertFormulaChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD bID;
		DWORD dwInit;

		FLOAT fRateX;
		FLOAT fRateY;

		sscanf( LPCSTR(strDATA), _T("%d %d %f %f"),
			&bID,
			&dwInit,
			&fRateX,
			&fRateY);

		vArchive
			<< BYTE(bID)
			<< dwInit
			<< fRateX
			<< fRateY;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertJointChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	while(vSRC.ReadString(strDATA))
	{
		DWORD wMapID;
		DWORD bUnitX;
		DWORD bUnitZ;

		int nLeft;
		int nTop;
		int nRight;
		int nBottom;

		DWORD bJoint;

		sscanf( LPCSTR(strDATA), _T("%d %d %d %d %d %d %d %d"),
			&wMapID,
			&bUnitX,
			&bUnitZ,
			&nLeft,
			&nTop,
			&nRight,
			&nBottom,
			&bJoint);

		vArchive
			<< DWORD(MAKELONG( MAKEWORD( BYTE(bUnitX), BYTE(bUnitZ)), WORD(wMapID)))
			<< nLeft
			<< nTop
			<< nRight
			<< nBottom
			<< BYTE(bJoint);
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertHelpChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pTITLE = new CString[dwCount];
	CString *pTEXT = new CString[dwCount];

	for( DWORD i=0 ; i < dwCount ; ++i )
		vSRC.ReadString( pTITLE[i] );

	for( DWORD i=0 ; i < dwCount ; ++i )
		vSRC.ReadString( pTEXT[i] );

	for( i=0; i<dwCount; i++)
	{
		vSRC.ReadString(strDATA);

		DWORD dwHelpID;
		DWORD dwImageLeft;
		DWORD dwImageRight;

		sscanf( LPCSTR(strDATA), _T("%d %d %d"),
			&dwHelpID,
			&dwImageLeft,
			&dwImageRight );

		vArchive
			<< (DWORD) dwHelpID
			<< (WORD) dwImageLeft
			<< (WORD) dwImageRight
			<< pTITLE[ i ]
			<< pTEXT[ i ];
	}

	delete[] pTITLE;
	delete[] pTEXT;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertHelpLinkChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	for( DWORD i=0; i<dwCount; i++)
	{
		vSRC.ReadString(strDATA);
		DWORD dwQuestID;
		DWORD bTrigger;
		DWORD dwHelpID;

		sscanf( LPCSTR(strDATA), _T("%d %d %d"),
			&dwQuestID,
			&bTrigger,
			&dwHelpID );

		vArchive
			<< (DWORD) dwQuestID
			<< (BYTE) bTrigger
			<< (DWORD) dwHelpID;
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertGodTowerChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pName = new CString[ dwCount ];
	for( DWORD i=0; i<dwCount; ++i )
		vSRC.ReadString( pName[i] );

	for( DWORD i=0; i<dwCount; ++i )
	{
		vSRC.ReadString(strDATA);
		DWORD dwID;
		FLOAT fRot;
		DWORD dwOBJID;
		DWORD dwIcon;
		DWORD dwSFXATKID;
		DWORD dwSFXDEFID;
		DWORD dwSFXNORID;

		sscanf( LPCSTR(strDATA), _T("%d %f %x %d %d %d %d"),
			&dwID,
			&fRot,
			&dwOBJID,
			&dwIcon,
			&dwSFXATKID,
			&dwSFXDEFID,
			&dwSFXNORID );

		pName[ i ].Replace( _T("\\n"), _T("\n"));

		vArchive
			<< pName[i]
			<< (DWORD)dwID
			<< (FLOAT)fRot
			<< (DWORD)dwOBJID
			<< (DWORD)dwIcon
			<< (DWORD)dwSFXATKID
			<< (DWORD)dwSFXDEFID
			<< (DWORD)dwSFXNORID;
	}
	
	delete []pName;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertGodBallChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pName = new CString[ dwCount ];
	for( DWORD i=0; i<dwCount; ++i )
		vSRC.ReadString( pName[i] );

	for( DWORD i=0; i<dwCount; ++i )
	{
		vSRC.ReadString(strDATA);
		DWORD dwID;
		FLOAT fRot;
		DWORD dwOBJID;
		DWORD dwIconID;
		DWORD dwSFXID;

		sscanf( LPCSTR(strDATA), _T("%d %f %x %d %d"),
			&dwID,
			&fRot,
			&dwOBJID,
			&dwIconID,
			&dwSFXID );

		pName[ i ].Replace( _T("\\n"), _T("\n"));

		vArchive
			<< pName[i]
			<< (DWORD)dwID
			<< (FLOAT)fRot
			<< (DWORD)dwOBJID
			<< (DWORD)dwIconID
			<< (DWORD)dwSFXID;
	}

	delete []pName;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertMonsterShop()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pNAME = new CString[dwCount];
	for( DWORD i=0; i<dwCount; i++)
		vSRC.ReadString(pNAME[i]);

	for( int i=0; i < (int)dwCount; ++i )
	{
		vSRC.ReadString( strDATA );

		DWORD dwID;
		DWORD dwSpawnID;
		DWORD dwIcon;

		sscanf( (LPCSTR)strDATA, "%d %d %d",
			&dwID,
			&dwSpawnID,
			&dwIcon );

		pNAME[ i ].Replace( _T("\\n"), _T("\n"));

		vArchive
			<< pNAME[i]
			<< (DWORD)dwID
			<< (DWORD)dwSpawnID
			<< (DWORD)dwIcon;
	}

	delete []pNAME;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertStringChart()
{
	CFile vDEST( LPCSTR(m_strDEST), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	CStdioFile vSRC( LPCSTR(m_strSRC), CFile::modeRead|CFile::typeText);

	CArchive vArchive( &vDEST, CArchive::store);
	CString strDATA;
	DWORD dwCount = 0;

	if(vSRC.ReadString(strDATA))
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

	vArchive
		<< WORD(dwCount);

	CString *pTEXT = new CString[dwCount];

	for( DWORD i=0 ; i < dwCount ; ++i )
		vSRC.ReadString( pTEXT[i] );

	for( i=0; i<dwCount; i++)
	{
		vSRC.ReadString(strDATA);

		DWORD wStringID;

		sscanf( LPCSTR(strDATA), _T("%d"),
			&wStringID );

		pTEXT[ i ].Replace( _T("\\n"), _T("\n"));

		vArchive
			<< (WORD) wStringID
			<< pTEXT[ i ];
	}

	delete[] pTEXT;
	vArchive.Close();
}

void CTChartProtectDlg::ConvertAuctionTreeChart()
{
	CFile vDEST( LPCSTR( m_strDEST ), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary );
	CStdioFile vSRC( LPCSTR( m_strSRC ), CFile::modeRead | CFile::typeText );

	CArchive vArchive( &vDEST, CArchive::store );
	CString strDATA;
	DWORD dwCount = 0;

	if( vSRC.ReadString( strDATA ) )
		sscanf( LPCSTR( strDATA ), _T("%d"), &dwCount );

	vArchive
		<< (WORD)dwCount;

	CString *pTEXT = new CString[ dwCount ];

	DWORD i = 0;
	for( ; i < dwCount; ++i )
		vSRC.ReadString( pTEXT[i] );

	for( i = 0; i < dwCount; ++i )
	{
		vSRC.ReadString( strDATA );

		DWORD dwStringID;

		sscanf( LPCSTR(strDATA), _T("%d"), &dwStringID );

		pTEXT[i].Replace( _T("\\n"), _T("\n") );

		vArchive
			<< pTEXT[i]
			<< (DWORD) dwStringID;
	}

	delete[] pTEXT;
	vArchive.Close();
}


void CTChartProtectDlg::ConvertFameRankChart()
{
	CFile vDEST( LPCSTR( m_strDEST ), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary );
	CStdioFile vSRC( LPCSTR( m_strSRC ), CFile::modeRead | CFile::typeText );

	CArchive vArchive( &vDEST, CArchive::store );
	CString strDATA;
	DWORD dwCount = 0;

	if( vSRC.ReadString( strDATA ) )
		sscanf( LPCSTR( strDATA ), _T("%d"), &dwCount );

	vArchive
		<< (WORD)dwCount;

	CString *pTEXT = new CString[ dwCount ];

	for( DWORD i = 0; i < dwCount; ++i )
		vSRC.ReadString( pTEXT[i] );

	for( DWORD i = 0; i < dwCount; ++i )
	{
		vSRC.ReadString( strDATA );

		DWORD	bRANK;
		DWORD	bSFX;
		DWORD	bSFXID;

		sscanf( LPCSTR(strDATA), _T("%d %d %d"),
			&bRANK,
			&bSFX,
			&bSFXID );

		pTEXT[i].Replace( _T("\\n"), _T("\n") );

		vArchive
			<< (BYTE) bRANK		// 랭킹
			<< pTEXT[i]			// 칭호
			<< (BYTE) bSFX		// SFX 사용 여부
			<< (BYTE) bSFXID;	// SFX ID
	}

	delete[] pTEXT;
	vArchive.Close();
}


void CTChartProtectDlg::ConvertEquipCreateChar()
{
	CFile vDEST( LPCSTR( m_strDEST ), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary );
	CStdioFile vSRC( LPCSTR( m_strSRC ), CFile::modeRead | CFile::typeText );

	CArchive vArchive( &vDEST, CArchive::store );
	CString strDATA;
	DWORD dwCount = 0;

	if( vSRC.ReadString( strDATA ) )
		sscanf( LPCSTR( strDATA ), _T("%d"), &dwCount );

	vArchive
		<< (WORD)dwCount;

	WORD i = 0;
	for( ; i < dwCount; ++i )
	{
		vSRC.ReadString( strDATA );
		
		INT nPos = 0;

		DWORD	bCountry = 0;
		DWORD	bClass = 0;
		DWORD	bSex = 0;
	
		CString strKEY = strDATA.Tokenize( "\t", nPos );
		sscanf( LPCSTR( strKEY ), _T("%d"), &bCountry );

		strKEY = strDATA.Tokenize( "\t", nPos );
		sscanf( LPCSTR( strKEY ), _T("%d"), &bClass );

		strKEY = strDATA.Tokenize( "\t", nPos );
		sscanf( LPCSTR( strKEY ), _T("%d"), &bSex );

		vArchive
			<< (BYTE)bCountry
			<< (BYTE)bClass
			<< (BYTE)bSex;

		WORD j = 0;
		for( ; j < 9; ++j )		// 착용갯수
		{
			DWORD wItemID = 0;
			DWORD wGrade = 0;
			DWORD bGradeEffect = 0;

			strKEY = strDATA.Tokenize( "\t", nPos);
			sscanf( LPCSTR( strKEY ), _T("%d"), &wItemID );

			strKEY = strDATA.Tokenize( "\t", nPos );
			sscanf( LPCSTR( strKEY ), _T("%d"), &wGrade );
			
			strKEY = strDATA.Tokenize( "\t", nPos );
			sscanf( LPCSTR( strKEY ), _T("%d"), &bGradeEffect );

			vArchive
				<< (WORD)wItemID
				<< (WORD)wGrade
				<< (BYTE)bGradeEffect;
		}
	}

	vArchive.Close();
}

void CTChartProtectDlg::ConvertRpsChart()
{
	CFile vDEST( LPCSTR( m_strDEST ), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary );
	CStdioFile vSRC( LPCSTR( m_strSRC ), CFile::modeRead | CFile::typeText );

	CArchive vArchive( &vDEST, CArchive::store );
	CString strDATA;
	DWORD dwCount = 0;

	if( vSRC.ReadString( strDATA ) )
		sscanf( LPCSTR( strDATA ), _T("%d"), &dwCount );

	vArchive
		<< (WORD)dwCount;

	WORD i = 0;
	for( ; i < dwCount; ++i )
	{
		vSRC.ReadString( strDATA );

		DWORD bType;
		DWORD bWinCount;
		DWORD wNeedItem;
		DWORD wRewardItem1;
		DWORD bRewardCount1;
		DWORD wRewardItem2;
		DWORD bRewardCount2;
		DWORD dwRewardMoney;

		sscanf( (LPCSTR)strDATA, "%d %d %d %d %d %d %d %d",
			&bType,
			&bWinCount,
			&wNeedItem,
			&wRewardItem1,
			&bRewardCount1,
			&wRewardItem2,
			&bRewardCount2,
			&dwRewardMoney );

		vArchive
			<< (BYTE) bType
			<< (BYTE) bWinCount
			<< (WORD) wNeedItem
			<< (WORD) wRewardItem1
			<< (BYTE) bRewardCount1
			<< (WORD) wRewardItem2
			<< (BYTE) bRewardCount2
			<< (DWORD) dwRewardMoney;
	}

	vArchive.Close();
}