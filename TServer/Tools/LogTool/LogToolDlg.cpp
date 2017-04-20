////////////////////////////////////////////////////////////////////////////////
//	File Name	:	LogToolDlg.Cpp
//
//	Copyright	:	(c) Zemi Interactive, Inc. 2002-2009
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LogTool.h"
#include "LogToolDlg.h"
#include "SearchDlg.h"
#include "ConnectDlg.h"
#include "NamingList.h"

#include <time.h>


#include <vector>
#include <string>
#include ".\logtooldlg.h"

using std::vector;
using std::string;


int		CALLBACK SortProc(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort);

////////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
////////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CLogToolDlg* CLogToolDlg::s_pThis = NULL;
/////////////////////////////////////////////////////////////////////////////
// CLogToolDlg dialog

CLogToolDlg::CLogToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogToolDlg)
		// NOTE: the ClassWizard will add member initialization here
		m_pDlg = NULL;
		m_pSearchDlg = NULL;
		m_nTab = 0;
		m_nCurrentTab = -1;
		m_nCurrentConnect = 0;
		s_pThis = this;
		m_pServList = NULL;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLogToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogToolDlg)
	DDX_Control(pDX, IDC_STATIC_SERVER, m_ctrlServerType);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_ctrlSearch);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_ctrlNext);
	DDX_Control(pDX, IDC_BUTTON_PREV, m_ctrlPrev);
	DDX_Control(pDX, IDC_STATIC_MSG, m_ctrlMsg);
	DDX_Control(pDX, IDC_LIST, m_ctrlView);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLogToolDlg, CDialog)
	//{{AFX_MSG_MAP(CLogToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnClickList)
	ON_COMMAND(ID_QUIT, OnMenuQuit)
	ON_COMMAND(ID_SAVE, OnMenuSave)
	ON_COMMAND(ID_SEARCH, OnMenuSearch)
	ON_COMMAND(ID_TOOLBAR_CONNECT, OnToolbarConnect)
	ON_COMMAND(ID_TOOLBAR_SAVE, OnToolbarSave)
	ON_COMMAND(ID_TOOLBAR_SEARCH, OnToolbarSearch)
	ON_COMMAND(ID_CONNECT, OnMenuConnect)
	ON_COMMAND(ID_INFO, OnInfo)
	ON_COMMAND(ID_POPUP_SEARCH, OnPopupSearch)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_COMMAND(ID_ACCEL_CONNECT, OnAccelConnect)
	ON_COMMAND(ID_ACCEL_QUIT, OnAccelQuit)
	ON_COMMAND(ID_ACCEL_SEARCH, OnAccelSearch)
	ON_COMMAND(ID_ACCEL_SAVE, OnAccelSave)
	ON_COMMAND(ID_ACCEL_SELECTALL, OnAccelSelectAll)
	ON_COMMAND(ID_ACCEL_COPY, OnAccelCopy)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MENU_SAVELOGCOLUMN, OnMenuSavelogcolumn)
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////
//	CLogToolDlg message handlers
////////////////////////////////////////////////////////////////////////////////
BOOL CLogToolDlg::OnInitDialog()
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
	
	//툴바 생성
	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT , WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS ) || 
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR))
	{

	   AfxMessageBox("Failed to create a window: TOOLBAR\n");
	   return FALSE;

	}
	
	//툴바 스타일 지정
	m_ToolBar.SetBarStyle(m_ToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_ToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	//툴바 이름 
	m_ToolBar.SetButtonText(0, NAME_MENU_CONNECT);
	m_ToolBar.SetButtonText(1, NAME_MENU_SAVE);
	m_ToolBar.SetButtonText(2, NAME_MENU_SEARCH);

	//툴바 사이즈 조정 
	CRect temp;
	m_ToolBar.GetItemRect(0,&temp);
	m_ToolBar.SetSizes(CSize(temp.Width(), temp.Height()),CSize(16,15));

	//다이얼로그에서 툴바를 생성할때 이 함수를 호출해야 툴바가 보인다. 
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0); 

	// 단축기 사용
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR)); 

	//
	//	리스트 컬럼
	//
	m_ctrlView.InsertColumn(0,  "Date/Time",LVCFMT_LEFT, 110	);
	m_ctrlView.InsertColumn(1,  "Svr",		LVCFMT_LEFT, 40		);
	m_ctrlView.InsertColumn(2,  "Client IP",LVCFMT_LEFT, 90		);
	m_ctrlView.InsertColumn(3,  "Action",	LVCFMT_LEFT, 70		);
	
	m_ctrlView.InsertColumn(4,  "Map",		LVCFMT_LEFT, 40		);
	m_ctrlView.InsertColumn(5,  "X",		LVCFMT_LEFT, 50		);
	m_ctrlView.InsertColumn(6,  "Y",		LVCFMT_LEFT, 50		);
	m_ctrlView.InsertColumn(7,  "Z",		LVCFMT_LEFT, 50		);

	m_ctrlView.InsertColumn(8,  "dwK1",		LVCFMT_LEFT, 50		);
	m_ctrlView.InsertColumn(9,  "K1",		LVCFMT_LEFT, 50		);

	m_ctrlView.InsertColumn(10, "dwK2",		LVCFMT_LEFT, 50		);
	m_ctrlView.InsertColumn(11, "K2",		LVCFMT_LEFT, 50		);

	m_ctrlView.InsertColumn(12, "dwK3",		LVCFMT_LEFT, 50		);
	m_ctrlView.InsertColumn(13, "K3",		LVCFMT_LEFT, 50		);

	m_ctrlView.InsertColumn(14, "dwK4",		LVCFMT_LEFT, 50		);
	m_ctrlView.InsertColumn(15, "K4",		LVCFMT_LEFT, 50		);

	m_ctrlView.InsertColumn(16, "dw5",		LVCFMT_LEFT, 50		);
	m_ctrlView.InsertColumn(17, "K5",		LVCFMT_LEFT, 50		);

	m_ctrlView.InsertColumn(18, "dwK6",		LVCFMT_LEFT, 50		);
	m_ctrlView.InsertColumn(19, "K6",		LVCFMT_LEFT, 50		);

	m_ctrlView.InsertColumn(20, "dwK7",		LVCFMT_LEFT, 50		);
	m_ctrlView.InsertColumn(21, "K7",		LVCFMT_LEFT, 50		);

	m_ctrlView.InsertColumn(22, "dwK8",		LVCFMT_LEFT, 50		);
	m_ctrlView.InsertColumn(23, "dwK9",		LVCFMT_LEFT, 50		);

	m_ctrlView.InsertColumn(24, "dwK10",		LVCFMT_LEFT, 50		);
	m_ctrlView.InsertColumn(25, "dwK11",	LVCFMT_LEFT, 50		);	

	m_ctrlView.InsertColumn(26, "Log",		LVCFMT_LEFT, 1000	);
	
	//
	//	리스트에 줄 넣기
	//
	m_ctrlView.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
	

BOOL CLogToolDlg::PreTranslateMessage(MSG* pMsg) 
{
	//
	//	단축기 사용
	//
	if (m_hAccelTable != NULL) 
	{ 
		if (TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg)) 

			return TRUE; 
	} 

	return CDialog::PreTranslateMessage(pMsg);
}

void CLogToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLogToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLogToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CLogToolDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnQuit();
	
	return CDialog::DestroyWindow();
}

void CLogToolDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	OnQuit();
}


void CLogToolDlg::OnAccelConnect() 
{
	// TODO: Add your command handler code here
	if(!OnConnect())
	{
		m_ctrlMsg.SetWindowText(RESULT_FAIL_CONNECT);
	}
	else
	{
		m_ctrlMsg.SetWindowText(RESULT_SUCCESS_CONNECT);
	}
}

void CLogToolDlg::OnAccelQuit() 
{
	// TODO: Add your command handler code here
	OnQuit();
}

void CLogToolDlg::OnAccelSearch() 
{
	// TODO: Add your command handler code here
	if(!m_LogConnection.IsConnected())
	{
		m_ctrlMsg.SetWindowText(RESULT_ASK_CONNECT);
		return;
	}

	OnSearch();
}


void CLogToolDlg::OnAccelSave() 
{
	// TODO: Add your command handler code here
	OnSave();
}

void CLogToolDlg::OnAccelSelectAll() 
{
	// TODO: Add your command handler code here
	m_ctrlView.SetFocus();
	for(int i=0; i<m_ctrlView.GetItemCount(); i++)
	{
		m_ctrlView.SetItemState(i, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
	}
}

void CLogToolDlg::OnAccelCopy() 
{
	// TODO: Add your command handler code here
	HGLOBAL		hMem; 
	TCHAR		*pszBuffer = NULL;

	int nCount = m_ctrlView.GetItemCount();

	pszBuffer = new TCHAR[MAX_PATH * nCount];

	memset(pszBuffer, 0x00, sizeof(TCHAR) * MAX_PATH * nCount);
	_tcscpy(pszBuffer, _T(""));

	int idx = ListView_GetNextItem(m_ctrlView.m_hWnd, -1, LVNI_ALL | LVNI_SELECTED ) ;

	if (idx == -1)
	{
		AfxMessageBox("Select rows to copy",MB_OK);
		return;
	}
	else
	{
		int		nOldIdx;
		TCHAR	szText[MAX_PATH];

		do{
			//ListView_GetItemText(m_ctrlView.m_hWnd, idx, 1, name, sizeof(name) ) ;

			for(int j=0; j<NUM_COLUMNS; j++)
			{
				memset(szText, 0x00, sizeof(szText));
				m_ctrlView.GetItemText(idx, j, szText, sizeof(szText));

				_tcscat(pszBuffer, szText);
				_tcscat(pszBuffer, _T("\t"));
			}
			
			_tcscat(pszBuffer, _T("\r\n"));

			nOldIdx = idx;
			idx = ListView_GetNextItem(m_ctrlView.m_hWnd, idx, LVNI_ALL | LVNI_SELECTED ) ;
			
		}while(idx != -1);	
	}

    if( OpenClipboard() )
    {
        if( EmptyClipboard() )
        {
            hMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(TCHAR) * MAX_PATH * nCount );

            if( hMem )
            {
                LPTSTR pClipData = (LPTSTR) GlobalLock(hMem);

                pClipData[0] = 0;

                _tcscpy( pClipData, pszBuffer );

                SetClipboardData( CF_OEMTEXT, hMem );

                GlobalUnlock( hMem );
            }

            CloseClipboard();

            GlobalFree( hMem );
        }
    }

	delete pszBuffer;
}

void CLogToolDlg::OnMenuConnect() 
{
	// TODO: Add your command handler code here
	if(!OnConnect())
	{
		m_ctrlMsg.SetWindowText(RESULT_SUCCESS_CONNECT);
	}
	else
	{
		m_ctrlMsg.SetWindowText(RESULT_FAIL_CONNECT);
	}
}

void CLogToolDlg::OnMenuSave() 
{
	// TODO: Add your command handler code here
	OnSave();
}

void CLogToolDlg::OnMenuQuit() 
{
	// TODO: Add your command handler code here
	OnQuit();
}

void CLogToolDlg::OnMenuSearch() 
{
	// TODO: Add your command handler code here
	if(!m_LogConnection.IsConnected())
	{
		m_ctrlMsg.SetWindowText(RESULT_ASK_CONNECT);
		return;
	}

	OnSearch();
}

void CLogToolDlg::OnInfo()
{
	CAboutDlg dlg;

	dlg.DoModal();
}

void CLogToolDlg::OnToolbarConnect() 
{
	// TODO: Add your command handler code here
	if(!OnConnect())
	{
		m_ctrlMsg.SetWindowText(RESULT_FAIL_CONNECT);
	}
	else
	{
		m_ctrlMsg.SetWindowText(RESULT_SUCCESS_CONNECT);
	}
}

void CLogToolDlg::OnToolbarSave() 
{
	// TODO: Add your command handler code here
	OnSave();
}

void CLogToolDlg::OnToolbarSearch() 
{
	// TODO: Add your command handler code here
	if(!m_LogConnection.IsConnected())
	{
		m_ctrlMsg.SetWindowText(RESULT_ASK_CONNECT);
		return;
	}

	OnSearch();
}

void CLogToolDlg::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	if(!m_LogConnection.IsConnected())
	{
		m_ctrlMsg.SetWindowText(RESULT_ASK_CONNECT);
		return;
	}

	OnSearch();
}


/*
 *	
 */
void CLogToolDlg::OnPopupSearch() 
{
	LPRECORDSET pRecord = NULL;

	if(!m_LogConnection.IsConnected())
	{
		m_ctrlMsg.SetWindowText(RESULT_ASK_CONNECT);
		return;
	}

	LVHITTESTINFO lv;
	ZeroMemory(&lv, sizeof(LVHITTESTINFO));
	lv.pt = m_PopupPt;

	if(-1 == m_ctrlView.SubItemHitTest(&lv))
		return;

	OnSearch();

	if(m_nCurrentTab != -1)
	{
		//레코드 포인터 얻어 오기
		pRecord = (LPRECORDSET)&(((LPRESULTSET)(m_ResultList[m_nCurrentTab]))->at(m_ctrlView.GetItemCount() - lv.iItem - 1));
	}
	else
	{
		//레코드 포인터 얻어 오기
		pRecord = (LPRECORDSET)&(((LPRESULTSET)(&m_AllResults))->at(m_ctrlView.GetItemCount() - lv.iItem - 1));
	}

	if(NULL == pRecord)
		return;
}

/*
 *	OnDblclkList
 */
void CLogToolDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE nia = (LPNMITEMACTIVATE) pNMHDR ;

	char ex[128];

	int nCount = m_ctrlView.GetItemCount();

	if((-1 == nia->iItem) || (nia->iItem > nCount))
		return;
	
	VECTOR_STRING detailInfo;

	LPRESULTSET pRecordSet = NULL;
	if(m_nCurrentTab != -1)
	{
		pRecordSet = (LPRESULTSET)m_ResultList[m_nCurrentTab];
	}
	else
	{
		pRecordSet = &m_AllResults;
	}

	//
	//	vector에 거꾸로 넣어 있으므로 nCount - nia->iItem - 1 (총 카운트에서 인덱스 값과 0 부터 시작 하므로 1를 뺀다)
	//
	int nIdx = nCount - nia->iItem - 1;
	
	//	GetItemDetailInfo((pRecordSet->at(nIdx)), detailInfo);
	
	if(!m_pDlg)
	{
		m_pDlg = new CDetailItemDlg;

		m_pDlg->Create(IDD_DIALOG_DETAIL);
		m_pDlg->ShowWindow(SW_SHOWNORMAL);
	}
	else
	{
		m_pDlg->ShowWindow(SW_SHOWNORMAL);
	}

	m_pDlg->ClearAllItems();
	
	LPRECORDSET pRecord = (LPRECORDSET)&(pRecordSet->at(nIdx));

	//
	//	View Search Condtion
	//
	CString strInfo;
	
	strInfo.Format(NAME_LOG_SERVER, pRecord->dwServerID);
	m_pDlg->InsertList(0, strInfo);

	strInfo.Format(NAME_LOG_CLIENT, pRecord->szClientIP);
	m_pDlg->InsertList(1, strInfo);

	for( int k=0; k < nActionCount; k++)
	{
		if(ActionNum[k] == pRecord->dwAction) break;
	}

	strInfo.Format(NAME_LOG_ACTION, ACTIONLIST[k]);
	m_pDlg->InsertList(2, strInfo);

	wsprintf(ex, "DK1 : %I64d KEY1 : %s", pRecord->dwKey[0], pRecord->szKey[0]);
	strInfo.Format(ex);
	m_pDlg->InsertList(3, strInfo);

	wsprintf(ex, "DK2 : %I64d KEY2 : %s", pRecord->dwKey[1], pRecord->szKey[1]);
	strInfo.Format(ex);
	m_pDlg->InsertList(4, strInfo);

	wsprintf(ex, "DK3 : %I64d KEY3 : %s", pRecord->dwKey[2], pRecord->szKey[2]);
	strInfo.Format(ex);
	m_pDlg->InsertList(5, strInfo);

	wsprintf(ex, "DK4 : %I64d KEY4 : %s", pRecord->dwKey[3], pRecord->szKey[3]);
	strInfo.Format(ex);
	m_pDlg->InsertList(6, strInfo);

	wsprintf(ex, "DK5 : %I64d KEY5 : %s", pRecord->dwKey[4], pRecord->szKey[4]);
	strInfo.Format(ex);
	m_pDlg->InsertList(7, strInfo);

	wsprintf(ex, "DK6 : %I64d KEY6 : %s", pRecord->dwKey[5], pRecord->szKey[5]);
	strInfo.Format(ex);
	m_pDlg->InsertList(8, strInfo);

	wsprintf(ex, "DK7 : %I64d KEY7 : %s", pRecord->dwKey[6], pRecord->szKey[6]);
	strInfo.Format(ex);
	m_pDlg->InsertList(9, strInfo);

	wsprintf(ex, "DK8 : %I64d", pRecord->dwKey[7] );
	strInfo.Format(ex);
	m_pDlg->InsertList(10, strInfo);

	wsprintf(ex, "DK9 : %I64d", pRecord->dwKey[8] );
	strInfo.Format(ex);
	m_pDlg->InsertList(11, strInfo);

	wsprintf(ex, "DK10 : %I64d", pRecord->dwKey[9] );
	strInfo.Format(ex);
	m_pDlg->InsertList(12, strInfo);

	wsprintf(ex, "DK11 : %I64d", pRecord->dwKey[10] );
	strInfo.Format(ex);
	m_pDlg->InsertList(13, strInfo);

	switch(pRecord->dwFormat)
	{
	case LF_CHARBASE:
		{
			_LPLOG_CHARBASE_ LgCharBase;
			memset(&LgCharBase, 0x00, sizeof(_LPLOG_CHARBASE_));
			LgCharBase = (_LPLOG_CHARBASE_)pRecord->szLog;
			
			strInfo.Format(NAME_LOG_LOG);
			m_pDlg->InsertList(14, strInfo);

			strInfo.Format("ID:%u Name:%s Slot:%d", LgCharBase->dwCharID, LgCharBase->szName, LgCharBase->bSlot);
			m_pDlg->InsertList(15, strInfo);

			strInfo.Format("Level:%d Class:%d Race:%d", LgCharBase->bLevel, LgCharBase->bClass, LgCharBase->bRace);
			m_pDlg->InsertList(16, strInfo);

			strInfo.Format("Country:%d Sex:%d Hair:%d", LgCharBase->bCountry, LgCharBase->bSex, LgCharBase->bHair);
			m_pDlg->InsertList(17, strInfo);

			strInfo.Format("Face:%d Body:%d Pants:%d",	LgCharBase->bFace, LgCharBase->bBody, LgCharBase->bPants);
			m_pDlg->InsertList(18, strInfo);

			strInfo.Format("Hands:%d Foot:%d Region:%u", LgCharBase->bHand, LgCharBase->bFoot, LgCharBase->dwRegion);
			m_pDlg->InsertList(19, strInfo);


			//	for(int i=0; i< (int)detailInfo.size(); ++i)
			//	{
			//		m_pDlg->InsertList(i+18, (LPCTSTR)detailInfo[i]);
			//	}			
		}
		break;

	case	LF_ITEM:
		{
			_LPLOG_ITEM_ LgItem;
			memset(&LgItem, 0x00, sizeof(_LPLOG_ITEM_));
			LgItem  = (_LPLOG_ITEM_)pRecord->szLog;

			strInfo.Format(NAME_LOG_LOG);
			m_pDlg->InsertList(14, strInfo);

			strInfo.Format("Unique  : %u", LgItem->dlID	);
			m_pDlg->InsertList(15, strInfo);

			strInfo.Format("Inven : %d Inven No : %d", LgItem->bInven, LgItem->bItemID);
			m_pDlg->InsertList(16, strInfo);

			strInfo.Format("Item Idx : %d Item Name : %s", LgItem->wItemID, LgItem->szItemName);
			m_pDlg->InsertList(17, strInfo);

			strInfo.Format("Upgrade : %d Count : %d", LgItem->bLevel, LgItem->bCount);
			m_pDlg->InsertList(18, strInfo);

			strInfo.Format("GLv : %d DuraMax : %d DuraCur : %d RefineCur : %d", LgItem->bGLevel, LgItem->dwDuraMax, LgItem->dwDuraCur, LgItem->bRefineCur);
			m_pDlg->InsertList(19, strInfo);

			strInfo.Format("Maic1(Magic : %d Value : %d Time : %u)", LgItem->bMagic[0], LgItem->wValue[0], LgItem->dwTime[0]);
			m_pDlg->InsertList(20, strInfo);

			strInfo.Format("Maic2(Magic : %d Value : %d Time : %u)", LgItem->bMagic[1], LgItem->wValue[1], LgItem->dwTime[1]);
			m_pDlg->InsertList(21, strInfo);

			strInfo.Format("Maic3(Magic : %d Value : %d Time : %u)", LgItem->bMagic[2], LgItem->wValue[2], LgItem->dwTime[2]);
			m_pDlg->InsertList(22, strInfo);

			strInfo.Format("Maic4(Magic : %d Value : %d Time : %u)", LgItem->bMagic[3], LgItem->wValue[3], LgItem->dwTime[3]);
			m_pDlg->InsertList(23, strInfo);

			strInfo.Format("Maic5(Magic : %d Value : %d Time : %u)", LgItem->bMagic[4], LgItem->wValue[4], LgItem->dwTime[4]);
			m_pDlg->InsertList(24, strInfo);

			strInfo.Format("Maic6(Magic : %d Value : %d Time : %u)", LgItem->bMagic[5], LgItem->wValue[5], LgItem->dwTime[5]);
			m_pDlg->InsertList(25, strInfo);


			switch( LgItem->bGradeEffect )
			{
			case 0	: strInfo.Format("Grade Effect : NONE");		break;

			case 1	: strInfo.Format("Grade Effect : SEA");			break;

			case 2	: strInfo.Format("Grade Effect : FIRE");		break;

			case 3	: strInfo.Format("Grade Effect : LIGHTING");	break;

			case 4	: strInfo.Format("Grade Effect : ICE");			break;

			case 5	: strInfo.Format("Grade Effect : BLCAK");		break;
			}
			
			m_pDlg->InsertList(26, strInfo);


			CTime time(LgItem->dEndTime);
			
			if(!LgItem->dEndTime)
			{
				strInfo.Format("End Date :  Not Expired");
			}
			else if( time.GetHour()	== 0 && time.GetMinute()== 0 )
			{
				strInfo.Format("End Date : %04d/%02d/%02",time.GetYear(), time.GetMonth(), time.GetDay() );
			}
			else
			{
				strInfo.Format("End Date : %04d/%02d/%02d %02d:%02d",time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
			}

			m_pDlg->InsertList(27, strInfo);
		}
		break;

	case	LF_SKILL	:
		{
			_LPLOG_SKILL_ LgSkill;
			memset(&LgSkill, 0x00, sizeof(_LPLOG_SKILL_));
			LgSkill = (_LPLOG_SKILL_)pRecord->szLog;
		
			strInfo.Format(NAME_LOG_LOG);
			m_pDlg->InsertList(14, strInfo);

			strInfo.Format("CharID : %d CharName : %s", LgSkill->dwCharID, LgSkill->szName);
			m_pDlg->InsertList(15, strInfo);

			strInfo.Format("SkillID : %d SkillLevel : %d", LgSkill->wSkill, LgSkill->bLevel);
			m_pDlg->InsertList(16, strInfo);

			//	for(int i=0; i<(int)detailInfo.size(); ++i)
			//	{
			//		m_pDlg->InsertList(i+12, (LPCTSTR)detailInfo[i]);
			//	}

		}
		break;

	case LF_PET :
		{
			_LPLOG_PET_ LgPet;
			memset(&LgPet, 0x00, sizeof(_LPLOG_PET_));
			LgPet = (_LPLOG_PET_)pRecord->szLog;
		
			strInfo.Format(NAME_LOG_LOG);
			m_pDlg->InsertList(14, strInfo);

			strInfo.Format("CharID : %d CharName : %s", LgPet->dwCharID, LgPet->szName);
			m_pDlg->InsertList(15, strInfo);

			strInfo.Format("PetID : %d PetName : %s", LgPet->m_wPetID, LgPet->szPetName);
			m_pDlg->InsertList(16, strInfo);

			strInfo.Format("Time : %I64d", LgPet->m_ldwTime);
			m_pDlg->InsertList(17, strInfo);
		
			//	for(int i=0; i<detailInfo.size(); ++i)
			//	{
			//		m_pDlg->InsertList(i+14, (LPCTSTR)detailInfo[i]);
			//	}		
		}
		break;

	default:
		{
			strInfo.Format(NAME_LOG_LOG, pRecord->szLog);
			m_pDlg->InsertList(14, strInfo);

			//	for(int i=0; i<detailInfo.size(); ++i)
			//	{
			//		m_pDlg->InsertList(i+11, (LPCTSTR)detailInfo[i]);
			//	}
		}
		break;
	}
}


void CLogToolDlg::OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	*pResult = 0;
}

//pPopup->EnableMenuItem(ID_START, MF_BYCOMMAND | MF_DISABLED |MF_GRAYED);
void CLogToolDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	m_PopupPt = point;
	m_ctrlView.ScreenToClient(&m_PopupPt);

	CMenu menu;

	if (menu.LoadMenu(IDR_POPUP))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, m_ctrlView.GetActiveWindow());
	}

}


void CLogToolDlg::OnQuit()
{
	ClearResultMap();

	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}

	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}

	if(m_pServList)
	{
		delete m_pServList;
		m_pServList = NULL;
	}

	m_LogConnection.Close();

	EndDialog(1);
}


/*
 *	DB CONNECT
 */
BOOL CLogToolDlg::OnConnect()
{
	BOOL bConnected = FALSE;

	m_ctrlMsg.SetWindowText("");
	m_ctrlView.DeleteAllItems();
	
	CConnectDlg dlg;

	if(dlg.DoModal() == IDOK)
	{		
		char cTemp[MAX_PATH]={0,};
		
		//
		//	Connect Log DB
		//
		if(!m_LogConnection.IsConnected() || m_nCurrentConnect != dlg.m_nConnectIdx)
		{
			m_LogConnection.Close();
			m_LogConnection.Init();
		
			wsprintf(cTemp, "DRIVER={sql server};SERVER=%s;""UID=%s;PWD=%s;DATABASE=%s;", m_pServList[dlg.m_nConnectIdx].szLogServAddr, (LPCSTR)(dlg.m_strAccID), (LPCSTR)(dlg.m_strPWD), m_pServList[dlg.m_nConnectIdx].szLogServDSN);

			m_LogConnection.SetConnectionTimeout(TIMEOUT);
			m_LogConnection.SetLoginTimeout(TIMEOUT);

			if(!m_LogConnection.DriverConnect(  cTemp  ))
			{
				return FALSE;
			}

			bConnected = TRUE;
		}

		m_nCurrentConnect = dlg.m_nConnectIdx;
		m_strAccID		  = dlg.m_strAccID;
		m_strAccPWD		  = dlg.m_strPWD;
	}

	m_ctrlServerType.SetWindowText((LPCTSTR)dlg.m_strDBName);

	return (bConnected == TRUE);
}


/*
 *	Save Excel File
 */
BOOL CLogToolDlg::OnSave()
{
	if(IDNO == AfxMessageBox(FORM_ASKTOSAVE, MB_YESNO))
		return FALSE;

	if (m_ctrlView.GetSelectedCount() == 0)
	{
		AfxMessageBox("Select rows to save",MB_OK);
		return FALSE;
	}

	time_t long_time;
	time( &long_time );//Get time as long integer.
    struct tm *newtime = localtime( &long_time ); // Convert to local time.
	
	TCHAR strDir[MAX_PATH];
	memset(strDir, 0x00, sizeof(strDir));

	GetCurrentDirectory(sizeof(strDir), strDir);

	_tcscat(strDir, _T("\\Log"));
	CreateDirectory(strDir, NULL);
		
	TCHAR strName[MAX_PATH];
	memset(strName, 0x00, sizeof(strName));
	
	wsprintf(strName, _T("%s\\Log_%d%.2d%.2d_%.2d%.2d%.2d.xls"), 
			 strDir,
			 1900 + newtime->tm_year,
			 newtime->tm_mon + 1,
			 newtime->tm_mday,
			 newtime->tm_hour,
			 newtime->tm_min,
			 newtime->tm_sec);

	CFile file;
	CFileException ex;
 
	if(!file.Open(strName, CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate, &ex))
	{
		TCHAR szError[MAX_PATH];
        ex.GetErrorMessage(szError, MAX_PATH);
		AfxMessageBox(szError);

		return FALSE;
	}

	int idx = ListView_GetNextItem(m_ctrlView.m_hWnd, -1, LVNI_ALL | LVNI_SELECTED ) ;

	if (idx == -1)
	{
		AfxMessageBox("Select rows to save",MB_OK);
		return FALSE;
	}
	else
	{
		TCHAR szBuffer[1024];
		TCHAR szText[MAX_PATH];
		int nOldIdx;

		//
		//	Title
		//
		wsprintf(szBuffer, _T("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n"), 
			"Date/Time", 
			"Server ID", 
			"Client IP", 
			"Action", 
			"MAPID", 
			"X", 
			"Y", 
			"Z", 

			"DK1",		"KEY1", 
			"DK2",		"KEY2",
			"DK3",		"KEY3", 
			"DK4",		"KEY4", 
			"DK5",		"KEY5", 
			"DK6",		"KEY6", 
			"DK7",		"KEY7", 
			"DK8",
			"DK9",
			"DK10",
			"DK11",
			"LOG");

		file.Write(szBuffer, _tcslen(szBuffer));

		do
		{	
			memset(szBuffer, 0x00, sizeof(szBuffer));
			_tcscpy(szBuffer, _T(""));

			for(int j=0; j<NUM_COLUMNS; j++)
			{
				memset(szText, 0x00, sizeof(szText));
				m_ctrlView.GetItemText(idx, j, szText, sizeof(szText));
				_tcscat(szBuffer, szText);
				_tcscat(szBuffer, _T("\t"));
			}

			_tcscat(szBuffer, _T("\n"));
			file.Write(szBuffer, _tcslen(szBuffer));

			nOldIdx = idx;
			idx = ListView_GetNextItem(m_ctrlView.m_hWnd, idx, LVNI_ALL | LVNI_SELECTED ) ;
			
		}while(idx != -1);	
	}	

	file.Close();

	return TRUE;
}


//
//	
//
void CLogToolDlg::OnSearch()
{
	if(!m_pSearchDlg)
	{
		m_pSearchDlg = new CSearchDlg;
		m_pSearchDlg->Create(IDD_DIALOG_SEARCH);
		m_pSearchDlg->ShowWindow(SW_SHOWNORMAL);
	}
	else
	{
		m_pSearchDlg->ShowWindow(SW_SHOWNORMAL);
	}
}

//
//	
//
BOOL CLogToolDlg::Search(CTime From, CTime To, CString strConditions)
{
	int nTotalResults = 0;
	m_nTab = 0;

	if( !m_pSearchDlg )
	{
		m_ctrlMsg.SetWindowText(RESULT_FAIL_RECORDSET);

		if(m_ctrlView.GetItemCount())
		{
			m_ctrlView.DeleteAllItems();
		}

		return FALSE;
	}


	//
	//	몇일분의 쿼리를 날릴지  알아온다.
	//
	CTimeSpan diff = To - From;
	int nDay = (int)diff.GetTotalHours() / 24;

	//차에서 하루를 더하거나 뺀다 
	if(nDay > 0)
		nDay += 1;
	else //음수이면, From 날짜가 To 날짜 보다 클 경우
		nDay -= 1;

	//총 검색 일이 너무 길 경우(10일 이상) 
	/*if(abs(nDay) > NUM_DAYS)
	{
		m_ctrlMsg.SetWindowText(RESULT_FAIL_LONGPERIOD);

		//리스트에 있는것 전부 지운다
		if(m_ctrlView.GetItemCount())
		{
			m_ctrlView.DeleteAllItems();
		}
		
		return FALSE;
	}*/
	
	ClearResultMap();
	
	CString date;
	char strQuery[1024];	
	
	for(int i=0; i<abs(nDay); i++)
	{
		CODBCRecordset tables(&m_LogConnection);

		//날짜 계산 ////////////////////////////////
		CTime		day = From;
		CTimeSpan	time(i, 0, 0, 0);

		if(nDay > 1)
			day += time;		//하루 이상이면...
		else if(nDay < 0)	
			day -= time;		//음수이면, From 날짜가 To 날짜 보다 클 경우  

		date = day.Format("%Y%m%d");
		////////////////////////////////////////////

		//쿼리문 작성
		memset(strQuery, 0x00, sizeof(strQuery));
		_tcscpy(strQuery, SQLQUERY_BASE);
		_tcscat(strQuery, (LPCTSTR)date);
		_tcscat(strQuery, (LPCTSTR)strConditions);
	
		if(!tables.Open(strQuery))
		{
			tables.Close();
			m_ctrlMsg.SetWindowText(RESULT_FAIL_OPEN);

			//리스트에 있는것 전부 지운다
			if(m_ctrlView.GetItemCount())
			{
				m_ctrlView.DeleteAllItems();
			}

			return FALSE;
		}

		RESULTSET* pRecordSet = new RESULTSET;
		if(NULL == pRecordSet)
		{
			tables.Close();
			m_ctrlMsg.SetWindowText(RESULT_FAIL_RECORDSET);

			//리스트에 있는것 전부 지운다
			if(m_ctrlView.GetItemCount())
			{
				m_ctrlView.DeleteAllItems();
			}
			
			return FALSE;
		}

		pRecordSet->reserve(10000);
	
		RECORDSET	result;
		while(!tables.IsEof())
		{
			memset(&result, 0x00, sizeof(RECORDSET));

			tables.GetFieldValue(0, &(result.Seq));				//	Seq
			tables.GetFieldValue(1, &(result.tTimeStamp));		//	Date
			tables.GetFieldValue(2, &(result.dwServerID));		//	서버 아이디
			tables.GetFieldValue(3, result.szClientIP);			//	클라이언트 IP
			tables.GetFieldValue(4, &(result.dwAction));		//	Action
			tables.GetFieldValue(5, &(result.wMapID));	
			tables.GetFieldValue(6, &(result.dwX));
			tables.GetFieldValue(7, &(result.dwY));
			tables.GetFieldValue(8, &(result.dwZ));

			tables.GetFieldValue(9, &(result.dwKey[0]));
			tables.GetFieldValue(10, &(result.dwKey[1]));
			tables.GetFieldValue(11, &(result.dwKey[2]));
			tables.GetFieldValue(12, &(result.dwKey[3]));
			tables.GetFieldValue(13, &(result.dwKey[4]));
			tables.GetFieldValue(14, &(result.dwKey[5]));
			tables.GetFieldValue(15, &(result.dwKey[6]));
			tables.GetFieldValue(16, &(result.dwKey[7]));
			tables.GetFieldValue(17, &(result.dwKey[8]));
			tables.GetFieldValue(18, &(result.dwKey[9]));
			tables.GetFieldValue(19, &(result.dwKey[10]));
			
			tables.GetFieldValue(20, result.szKey[0]);			// key 1
			tables.GetFieldValue(21, result.szKey[1]);			// key 2
			tables.GetFieldValue(22, result.szKey[2]);			// key 3			
			tables.GetFieldValue(23, result.szKey[3]);			// key 4
			tables.GetFieldValue(24, result.szKey[4]);			// key 5	
			tables.GetFieldValue(25, result.szKey[5]);			// key 6	
			tables.GetFieldValue(26, result.szKey[6]);			// key 7

			tables.GetFieldBlob(27, &(result.dwFormat));
			tables.GetFieldBlob(28, result.szLog);				//	Log


			if(0 != result.Seq)
			{
				pRecordSet->push_back(result);
			}

			tables.MoveNext();
		}

		//결과가 있으면 맵에 저장 
		int nCount = pRecordSet->size();
		if(nCount > 0)
		{
			pair<RESULTLISTITER, bool> ret = m_ResultList.insert(RESULTLISTPAIR(m_nTab, pRecordSet));

			if(ret.second == false)
			{
				tables.Close();
				m_ctrlMsg.SetWindowText(RESULT_FAIL_RECORDSET);

				//리스트에 있는것 전부 지운다
				if(m_ctrlView.GetItemCount())
				{
					m_ctrlView.DeleteAllItems();
				}

				return FALSE;
			}

			++m_nTab;
			nTotalResults += nCount;
		}
		else
		{
			delete pRecordSet;
		}

		tables.Close();
	}

	//하나의 결과라도 있으면 뷰에 보여준다.
	if(nTotalResults == 0)
	{
		m_ctrlMsg.SetWindowText(RESULT_FAIL_NORECORDSET);

		//리스트에 있는것 전부 지운다
		if(m_ctrlView.GetItemCount())
		{
			m_ctrlView.DeleteAllItems();
		}

		return FALSE;
	}
	
	//OnView();

	OnViewAll();

	return TRUE;
}

//
//	OnView
//
BOOL CLogToolDlg::OnView(int nTabIndex)
{
	RESULTLISTITER itr = m_ResultList.find(nTabIndex);
	if(itr == m_ResultList.end())
	{
		return FALSE;
	}

	RESULTSET*  pRecordSet = (LPRESULTSET)itr->second;
	if(NULL == pRecordSet)
		return FALSE;

	if(m_ctrlView.GetItemCount())
	{
		m_ctrlView.DeleteAllItems();
	}
	
	int nResultSets = pRecordSet->size();
	
	if(nResultSets > 0)
	{
		char tmp[2048];
		char ex[32];

		RESULTSETITER itr = pRecordSet->begin();
		while(itr != pRecordSet->end())
		{
			RESULTSETITER pos = itr;
			++itr;

			memset(tmp, 0x00, sizeof(tmp));
			strftime(tmp, sizeof(tmp), FORM_DATETIME, &(pos->tTimeStamp));	

			m_ctrlView.InsertItem(0, tmp);

			wsprintf(ex, "%d", pos->dwServerID);
			m_ctrlView.SetItemText(0, 1, ex);						//	Server Svr
			m_ctrlView.SetItemText(0, 2, pos->szClientIP);			//	Client IP

			for( int k =0; k < nActionCount; k++)
			{
				if(ActionNum[k] == pos->dwAction) break;
			}
			m_ctrlView.SetItemText(0, 3, ACTIONLIST[k]);			//	Action

			wsprintf(ex, "%d", pos->wMapID);
			m_ctrlView.SetItemText(0, 4, ex);						// Map ID
			wsprintf(ex, "%u", pos->dwX);
			m_ctrlView.SetItemText(0, 5, ex);						//	X
			wsprintf(ex, "%u", pos->dwY);
			m_ctrlView.SetItemText(0, 6, ex);						//	Y
			wsprintf(ex, "%u", pos->dwZ);
			m_ctrlView.SetItemText(0, 7, ex);						//	Z

			wsprintf(ex, "%I64d", pos->dwKey[0]);
			m_ctrlView.SetItemText(0, 8, ex);				
			m_ctrlView.SetItemText(0, 9, pos->szKey[0]);			//	Key 1

			wsprintf(ex, "%I64d", pos->dwKey[1]);
			m_ctrlView.SetItemText(0, 10, ex);				
			m_ctrlView.SetItemText(0, 11, pos->szKey[1]);			//	key 2

			wsprintf(ex, "%I64d", pos->dwKey[2]);
			m_ctrlView.SetItemText(0, 12, ex);				
			m_ctrlView.SetItemText(0, 13, pos->szKey[2]);			//	key 3

			wsprintf(ex, "%I64d", pos->dwKey[3]);
			m_ctrlView.SetItemText(0, 14, ex);				
			m_ctrlView.SetItemText(0, 15, pos->szKey[3]);			//	key 4

			wsprintf(ex, "%I64d", pos->dwKey[4]);
			m_ctrlView.SetItemText(0, 16, ex);				
			m_ctrlView.SetItemText(0, 17, pos->szKey[4]);			//	key 5

			wsprintf(ex, "%I64d", pos->dwKey[5]);
			m_ctrlView.SetItemText(0, 18, ex);				
			m_ctrlView.SetItemText(0, 19, pos->szKey[5]);			//	key 6

			wsprintf(ex, "%I64d", pos->dwKey[6]);
			m_ctrlView.SetItemText(0, 20, ex);				
			m_ctrlView.SetItemText(0, 21, pos->szKey[6]);			//	key 7
				
			wsprintf(ex, "%I64d", pos->dwKey[7]);					//	key 8
			m_ctrlView.SetItemText(0, 22, ex);

			wsprintf(ex, "%I64d", pos->dwKey[8]);					//	key 9
			m_ctrlView.SetItemText(0, 23, ex);
			
			switch(pos->dwFormat)
			{
			case	LF_CHARBASE	:
				{				
					_LPLOG_CHARBASE_ LgCharBase;
					memset(&LgCharBase, 0x00, sizeof(_LPLOG_CHARBASE_));
					LgCharBase = (_LPLOG_CHARBASE_)pos->szLog;
					
					memset(tmp, 0x00, sizeof(tmp));
					wsprintf(tmp, "ID:%u Name:%s Slot:%d Level:%d Class:%d Race:%d Country:%d Sex:%d Hair:%d Face:%d Body:%d Pants:%d Hands:%d Foot:%d Region:%u", 
							LgCharBase->dwCharID, 
							LgCharBase->szName,
							LgCharBase->bSlot, 
							LgCharBase->bLevel, 
							LgCharBase->bClass,
							LgCharBase->bRace, 
							LgCharBase->bCountry, 
							LgCharBase->bSex, 
							LgCharBase->bHair, 
							LgCharBase->bFace, 
							LgCharBase->bBody,
							LgCharBase->bPants, 
							LgCharBase->bHand, 
							LgCharBase->bFoot,
							LgCharBase->dwRegion);
				}
				break;
				
			case	LF_ITEM	:
				{
					_LPLOG_ITEM_ LgItem;
					memset(&LgItem, 0x00, sizeof(_LPLOG_ITEM_));
					LgItem  = (_LPLOG_ITEM_)pos->szLog;

					memset(tmp, 0x00, sizeof(tmp));
					wsprintf(tmp, "Ivn:%d Ivn No:%d Idx:%d Name:%s Upgrade:%d Cnt:%d M1(%d %d %u) M2(%d %d %u) M3(%d %d %u) M4(%d %d %u) M5(%d %d %u) M6(%d %d %u)",
						LgItem->bInven, 
						LgItem->bItemID, 
						LgItem->wItemID, 
						LgItem->szItemName, 
						LgItem->bLevel, 
						LgItem->bCount,

						LgItem->bMagic[0],	
						LgItem->wValue[0],	
						LgItem->dwTime[0],

						LgItem->bMagic[1],	
						LgItem->wValue[1],	
						LgItem->dwTime[1], 

						LgItem->bMagic[2],	
						LgItem->wValue[2],	
						LgItem->dwTime[2],

						LgItem->bMagic[3],
						LgItem->wValue[3],
						LgItem->dwTime[3], 

						LgItem->bMagic[4],
						LgItem->wValue[4],
						LgItem->dwTime[4],

						LgItem->bMagic[5],
						LgItem->wValue[5],
						LgItem->dwTime[5] 
					);
				
				}
				break;

			case	LF_SKILL :
				{
					_LPLOG_SKILL_ LgSkill;
					memset(&LgSkill, 0x00, sizeof(_LPLOG_SKILL_));
					LgSkill = (_LPLOG_SKILL_)pos->szLog;

					memset(tmp, 0x00, sizeof(tmp));
					wsprintf(tmp, "CharID ; %d CharName : %s SkillID : %d SkillLevel : %d ",
						LgSkill->dwCharID, 
						LgSkill->szName, 
						LgSkill->wSkill, 
						LgSkill->bLevel	);
				}
				break;

			case	LF_PET :
				{
					_LPLOG_PET_ LgPet;
					memset(&LgPet, 0x00, sizeof(_LPLOG_PET_));
					LgPet = (_LPLOG_PET_)pos->szLog;
						
					memset(tmp, 0x00, sizeof(tmp));
					wsprintf(tmp, "CharID ; %d CharName : %s PetID : %d PetName : %s Time = %I64d",
						LgPet->dwCharID, 
						LgPet->szName, 
						LgPet->m_wPetID, 
						LgPet->szPetName, 
						LgPet->m_ldwTime);
				}
				break;
			
			default:
				{
					wsprintf(tmp, "%s ", pos->szLog);
				}
				break;
			}

			m_ctrlView.SetItemText(0, 24, tmp);
		}
		
	}

	CString strTemp;
	strTemp.Format("[Results: %d] %s", nResultSets, RESULT_SUCCESS_RECORDSET);
	m_ctrlMsg.SetWindowText((LPCTSTR)strTemp);
	
	return TRUE;
}


/*
 *	OnViewAll	
 */
BOOL CLogToolDlg::OnViewAll()
{
	int nResultSets			= 0;

	//	Clear List
	if(m_ctrlView.GetItemCount())
	{
		m_ctrlView.DeleteAllItems();
	}
	
	m_AllResults.clear();

	RESULTLISTITER pos = m_ResultList.begin();
	for(RESULTLISTITER itr = pos; itr != m_ResultList.end(); ++itr)
	{
		RESULTSET* pRecordSet = (LPRESULTSET)itr->second;

		if(!pRecordSet)	return FALSE;
		
		nResultSets += pRecordSet->size();
		
		if(nResultSets > 0)
		{
			char tmp[512];
			char ex[32];

			RESULTSETITER itr = pRecordSet->begin();
			while(itr != pRecordSet->end())
			{
				RESULTSETITER where = itr;
				++itr;

				memset(tmp, 0x00, sizeof(tmp));
				strftime(tmp, sizeof(tmp), FORM_DATETIME, &(where->tTimeStamp));	

				m_ctrlView.InsertItem(0, tmp);

				wsprintf(ex, "%d", where->dwServerID);
				m_ctrlView.SetItemText(0, 1, ex);				// Server ID

				m_ctrlView.SetItemText(0, 2, where->szClientIP);//	Cleint IP

				int k;
				for(k =0; k < nActionCount; k++)
				{
					if(ActionNum[k] == where->dwAction) break;
				}				
				m_ctrlView.SetItemText(0, 3, ACTIONLIST[k]);	//	Action

				wsprintf(ex, "%d", where->wMapID);		
				m_ctrlView.SetItemText(0, 4, ex);				//	Map ID
				wsprintf(ex, "%u", where->dwX);			
				m_ctrlView.SetItemText(0, 5, ex);				//	X

				wsprintf(ex, "%u", where->dwY);			
				m_ctrlView.SetItemText(0, 6, ex);				//	Y

				wsprintf(ex, "%u", where->dwZ);			
				m_ctrlView.SetItemText(0, 7, ex);				//	Z

				wsprintf(ex, "%I64d", where->dwKey[0]);	
				m_ctrlView.SetItemText(0, 8,  ex);	
				m_ctrlView.SetItemText(0, 9, where->szKey[0]);	//	Key 1
				
				wsprintf(ex, "%I64d", where->dwKey[1]);	
				m_ctrlView.SetItemText(0, 10, ex);	
				m_ctrlView.SetItemText(0, 11, where->szKey[1]);	// key 2

				wsprintf(ex, "%I64d", where->dwKey[2]);	
				m_ctrlView.SetItemText(0, 12, ex);	
				m_ctrlView.SetItemText(0, 13, where->szKey[2]);	//	Key 3

				wsprintf(ex, "%I64d", where->dwKey[3]);	
				m_ctrlView.SetItemText(0, 14, ex);	
				m_ctrlView.SetItemText(0, 15, where->szKey[3]);	//	Key 4

				wsprintf(ex, "%I64d", where->dwKey[4]);	
				m_ctrlView.SetItemText(0, 16, ex);	
				m_ctrlView.SetItemText(0, 17, where->szKey[4]);	//	Key 5

				wsprintf(ex, "%I64d", where->dwKey[5]);	
				m_ctrlView.SetItemText(0, 18, ex);	
				m_ctrlView.SetItemText(0, 19, where->szKey[5]);	//	key 6

				wsprintf(ex, "%I64d", where->dwKey[6]);	
				m_ctrlView.SetItemText(0, 20, ex);	
				m_ctrlView.SetItemText(0, 21, where->szKey[6]);	//	key 7

				wsprintf(ex, "%I64d", where->dwKey[7]);	
				m_ctrlView.SetItemText(0, 22, ex);				//	Key 8

				wsprintf(ex, "%I64d", where->dwKey[8]);	
				m_ctrlView.SetItemText(0, 23, ex);				//	Key 9

				wsprintf(ex, "%I64d", where->dwKey[9]);	
				m_ctrlView.SetItemText(0, 24, ex);				//	Key 10

				wsprintf(ex, "%I64d", where->dwKey[10]);	
				m_ctrlView.SetItemText(0, 25, ex);				//	Key 11

				switch(where->dwFormat)
				{
				case	LF_CHARBASE	:
					{					
						_LPLOG_CHARBASE_ LgCharBase;
						memset(&LgCharBase, 0x00, sizeof(_LPLOG_CHARBASE_));
						LgCharBase = (_LPLOG_CHARBASE_)where->szLog;
						
						memset(tmp, 0x00, sizeof(tmp));
						wsprintf(tmp, "ID:%u Name:%s Slot:%d Level:%d Class:%d Race:%d Country:%d Sex:%d Hair:%d Face:%d Body:%d Pants:%d Hands:%d Foot:%d Region:%u", 
							LgCharBase->dwCharID, 
							LgCharBase->szName,
							LgCharBase->bSlot, 
							LgCharBase->bLevel, 
							LgCharBase->bClass,
							LgCharBase->bRace, 
							LgCharBase->bCountry, 
							LgCharBase->bSex, 
							LgCharBase->bHair, 
							LgCharBase->bFace, 
							LgCharBase->bBody,
							LgCharBase->bPants, 
							LgCharBase->bHand, 
							LgCharBase->bFoot,
							LgCharBase->dwRegion);
					}
					break;

				case	LF_ITEM	:
					{
						_LPLOG_ITEM_ LgItem;
						memset(&LgItem, 0x00, sizeof(_LPLOG_ITEM_));
						LgItem  = (_LPLOG_ITEM_)where->szLog;

						memset(tmp, 0x00, sizeof(tmp));

						wsprintf(tmp, "Ivn:%d Ivn No:%d Idx:%d Name:%s Upgrade:%d Cnt:%d M1(%d %d %u) M2(%d %d %u) M3(%d %d %u) M4(%d %d %u) M5(%d %d %u) M6(%d %d %u)",
							LgItem->bInven, 
							LgItem->bItemID, 
							LgItem->wItemID, 
							LgItem->szItemName, 
							LgItem->bLevel, 
							LgItem->bCount,

							LgItem->bMagic[0],	
							LgItem->wValue[0],	
							LgItem->dwTime[0],

							LgItem->bMagic[1],	
							LgItem->wValue[1],	
							LgItem->dwTime[1], 

							LgItem->bMagic[2],	
							LgItem->wValue[2],	
							LgItem->dwTime[2],

							LgItem->bMagic[3],
							LgItem->wValue[3],
							LgItem->dwTime[3], 

							LgItem->bMagic[4],
							LgItem->wValue[4],
							LgItem->dwTime[4],

							LgItem->bMagic[5],
							LgItem->wValue[5],
							LgItem->dwTime[5] 
						);



  
					}
					break;

				case	LF_SKILL:
					{
						_LPLOG_SKILL_ LgSkill;
						memset(&LgSkill, 0x00, sizeof(_LPLOG_SKILL_));
						LgSkill = (_LPLOG_SKILL_)where->szLog;

						memset(tmp, 0x00, sizeof(tmp));
						wsprintf(tmp, "CharID ; %d CharName : %s SkillID : %d SkillLevel : %d ",
							LgSkill->dwCharID, 
							LgSkill->szName, 
							LgSkill->wSkill, 
							LgSkill->bLevel);
					}
					break;

				case LF_PET:
					{
						_LPLOG_PET_ LgPet;
						memset(&LgPet, 0x00, sizeof(_LPLOG_PET_));
						LgPet = (_LPLOG_PET_)where->szLog;
						
						memset(tmp, 0x00, sizeof(tmp));
						wsprintf(tmp, "CharID ; %d CharName : %s PetID : %d PetName : %s Time = %I64d",
							LgPet->dwCharID, 
							LgPet->szName, 
							LgPet->m_wPetID, 
							LgPet->szPetName, 
							LgPet->m_ldwTime);			
					}
					break;
					
				default:
					{
						memset(tmp, 0x00, sizeof(tmp));
						wsprintf(tmp, "%s ", where->szLog);
					}
					break;
				}

				m_ctrlView.SetItemText(0, 26, tmp);

				m_AllResults.push_back(*where);				
			}
		}
	}

	CString strTemp;
	strTemp.Format("[Results: %d] %s", nResultSets, RESULT_SUCCESS_RECORDSET);
	m_ctrlMsg.SetWindowText((LPCTSTR)strTemp);
	
	return TRUE;
}

/*
 *	
 */
void CLogToolDlg::ClearResultMap()
{
	/*
	RESULTLISTITER pos, itr = m_ResultList.begin();
	while(itr != m_ResultList.end())
	{
		pos = itr;
		++itr;		
		delete pos->second;
	}
	m_ResultList.clear();
	*/
	RESULTLISTITER itr = m_ResultList.begin();
	while(itr != m_ResultList.end())
	{		
		itr->second->clear();
		delete itr->second;
		itr++;
	}
	m_ResultList.clear();
}

/*
 *	
 */
void CLogToolDlg::OnButtonPrev() 
{
	if(m_nCurrentTab <= -1)
	{
		m_nCurrentTab = -1;
		return;
	}
	else
	{
		--m_nCurrentTab;
	}
	
	if(m_nCurrentTab != -1)
	{
		OnView(m_nCurrentTab);
	}
	else
	{
		OnViewAll();
	}
}

/*
 *	
 */
void CLogToolDlg::OnButtonNext() 
{
	if(m_nCurrentTab >= m_nTab - 1)
	{
		m_nCurrentTab = m_nTab - 1;
		return;
	}
	else
	{
		++m_nCurrentTab;
	}
	
	if(m_nCurrentTab != -1)
	{
		OnView(m_nCurrentTab);
	}
	else
	{
		OnViewAll();
	}
}

/*
 *	
 */
void CLogToolDlg::OnButtonAll() 
{
	m_nCurrentTab = -1;

	OnViewAll();
}

void CLogToolDlg::OnMenuSavelogcolumn()
{
	if(IDNO == AfxMessageBox(FORM_ASKTOSAVE_LOG_COLUMN, MB_YESNO))
		return;

	if (m_ctrlView.GetSelectedCount() == 0)
	{
		AfxMessageBox("Select rows to save log column",MB_OK);
		return;
	}

	time_t long_time;
	time( &long_time );//Get time as long integer.
    struct tm *newtime = localtime( &long_time ); // Convert to local time.

	TCHAR strDir[MAX_PATH];
	memset(strDir, 0x00, sizeof(strDir));

	GetCurrentDirectory(sizeof(strDir), strDir);

	_tcscat(strDir, _T("\\Log Column"));
	CreateDirectory(strDir, NULL);

	TCHAR strName[MAX_PATH];
	memset(strName, 0x00, sizeof(strName));

	wsprintf(strName, _T("%s\\Log_%d%.2d%.2d_%.2d%.2d%.2d.dat"), 
			 strDir,
			 1900 + newtime->tm_year,
			 newtime->tm_mon + 1,
			 newtime->tm_mday,
			 newtime->tm_hour,
			 newtime->tm_min,
			 newtime->tm_sec);

	CFile file;
	CFileException ex;
 
	if(!file.Open(strName, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary, &ex))
	{
		TCHAR szError[MAX_PATH];
        ex.GetErrorMessage(szError, MAX_PATH);
		AfxMessageBox(szError);

		return;
	}

	int idx = ListView_GetNextItem(m_ctrlView.m_hWnd, -1, LVNI_ALL | LVNI_SELECTED ) ;

	if (idx == -1)
	{
		AfxMessageBox("Select rows to save",MB_OK);
		return;
	}
	else
	{
		do
		{
			LPRECORDSET pRecord = (LPRECORDSET)&(((LPRESULTSET)(&m_AllResults))->at(m_ctrlView.GetItemCount() - idx - 1));

			if(pRecord)
				file.Write(pRecord->szLog, 512);

			idx = ListView_GetNextItem(m_ctrlView.m_hWnd, idx, LVNI_ALL | LVNI_SELECTED ) ;
		}while(idx != -1);	
	}	

	file.Close();
}
