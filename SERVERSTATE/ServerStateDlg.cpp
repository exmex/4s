// ServerStateDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ServerState.h"
#include "ServerStateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEF_BEEP_EVENT				(1000)


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


// CServerStateDlg 대화 상자



CServerStateDlg::CServerStateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerStateDlg::IDD, pParent)
	, m_bChecked(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST, m_ctrlServerState);
	DDX_Check(pDX, IDC_CHECK1, m_bChecked);
}

BEGIN_MESSAGE_MAP(CServerStateDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_BUTTON, OnBnClickedCheckButton)
	ON_BN_CLICKED(IDC_LIST_BUTTON, OnBnClickedListButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_NOTIFY(LVN_DELETEITEM, IDC_LIST, OnLvnDeleteitemList)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CServerStateDlg 메시지 처리기

BOOL CServerStateDlg::OnInitDialog()
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

	m_ctrlServerState.SetExtendedStyle(m_ctrlServerState.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ctrlServerState.InsertColumn(0, _T("서버 이름"), LVCFMT_CENTER, 100);
	m_ctrlServerState.InsertColumn(1, _T("IP"), LVCFMT_CENTER, 100);
	m_ctrlServerState.InsertColumn(2, _T("Port"), LVCFMT_CENTER, 60);
	m_ctrlServerState.InsertColumn(3, _T("접속 상태"), LVCFMT_CENTER, 100);
	m_ctrlServerState.DeleteAllItems();

	CServerStateSock::m_pctrlServerState = &m_ctrlServerState;
	CServerStateSock::m_pHost = this;

	m_bChecked = TRUE;
	UpdateData(FALSE);

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CServerStateDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CServerStateDlg::OnPaint() 
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
HCURSOR CServerStateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerStateDlg::OnBnClickedCheckButton()
{
	DoBeep(FALSE);

	for( int i=0; i<m_ctrlServerState.GetItemCount(); i++)
	{
		CServerStateSock *pCON = (CServerStateSock *) m_ctrlServerState.GetItemData(i);
		CString strIP = m_ctrlServerState.GetItemText( i, 1);
		CString strPort = m_ctrlServerState.GetItemText( i, 2);

		if(pCON)
			pCON->Connect( LPCSTR(strIP), atoi(strPort));
	}
}

void CServerStateDlg::OnBnClickedListButton()
{
	CStdioFile file;
	CString str;

	file.Open(_T("test.txt"), CFile::modeRead); 
	CString resToken;

	while(file.ReadString(str))
	{
		str.TrimRight();
		str.TrimLeft();

		if(!str.IsEmpty())
		{
			CServerStateSock *pCON = new CServerStateSock();
			TCHAR strNAME[MAX_PATH];
			TCHAR strIP[MAX_PATH];
			TCHAR strPort[MAX_PATH];

			sscanf( str, _T("%s %s %s"),
				strNAME,
				strIP,
				strPort);

			pCON->m_nIndex = m_ctrlServerState.InsertItem(
				m_ctrlServerState.GetItemCount(),
				strNAME);

			pCON->Create(
				0, SOCK_STREAM,
				FD_READ|
				FD_WRITE|
				FD_OOB|
				FD_CONNECT
				FD_CLOSE);

			m_ctrlServerState.SetItemText( pCON->m_nIndex, 1, strIP);
			m_ctrlServerState.SetItemText( pCON->m_nIndex, 2, strPort);
			m_ctrlServerState.SetItemData( pCON->m_nIndex, (DWORD_PTR) pCON);
		}
	}

	file.Close();
}

void CServerStateDlg::DoBeep( BOOL flag)
{
	if(flag)
		SetTimer( DEF_BEEP_EVENT, 1000, NULL);
	else
		KillTimer(DEF_BEEP_EVENT);
}

void CServerStateDlg::OnTimer(UINT nIDEvent)
{
	CDialog::OnTimer(nIDEvent);

	if( nIDEvent == DEF_BEEP_EVENT )
		Beep( 2000, 500);
}

void CServerStateDlg::OnBnClickedCheck1()
{
	UpdateData();
}

void CServerStateDlg::OnLvnDeleteitemList( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if( pNMLV && pNMLV->iItem >= 0 )
	{
		CServerStateSock *pCON = (CServerStateSock *) m_ctrlServerState.GetItemData(pNMLV->iItem);
		delete pCON;
	}
}

void CServerStateDlg::OnDestroy()
{
	m_ctrlServerState.DeleteAllItems();
	CDialog::OnDestroy();
}
