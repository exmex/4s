// GMToolDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "GMTool.h"
#include "GMToolDlg.h"
#include ".\gmtooldlg.h"

#include "GMToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGMToolDlg 대화 상자



CGMToolDlg::CGMToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGMToolDlg::IDD, pParent)
	, m_strID(_T(""))
	, m_strPW(_T(""))
	, m_bDisconnect(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_pdlgCharMsg = new CCharMsgDlg();
	m_pdlgCharMsg->Create(IDD_CHARMSG,CWnd::GetDesktopWindow());

	m_pdlgUserMove = new CUserMoveDlg();
	m_pdlgUserMove->Create(IDD_USERMOVE,CWnd::GetDesktopWindow()); 

	m_pdlgChatBan = new CChatBanDlg();
	m_pdlgChatBan->Create(IDD_CHATBAN,CWnd::GetDesktopWindow());	

	m_pdlgUserKickOut = new CUserKickOutDlg();
	m_pdlgUserKickOut->Create(IDD_USERKICKOUT,CWnd::GetDesktopWindow());

	m_pdlgIPChange = new CIPChangeDlg();
	m_pdlgIPChange->Create(IDD_IP,CWnd::GetDesktopWindow() );

	m_pdlgAnnounce = new CAnnounceDlg();
	m_pdlgAnnounce->Create(IDD_ANNOUNCE,CWnd::GetDesktopWindow() );

	m_pdlgMonsterSpawn = new CMonsterSpawnDlg();
	m_pdlgMonsterSpawn->Create(IDD_MONSPAWN,CWnd::GetDesktopWindow() );
	

	// CWnd::GetDesktopWindow() : 포커스 대화상자를 앞으로 오도록 하기-> 윈도우 태스크 바에 대화상자가 성성됨
	// 태스크 바에 대화상자 안생기게 할려면  해당 대화상자의 OnInitDialog() 에 아래 코드 추가해야함.
	// ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);
	
}

void CGMToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGMToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_CHARMSG, OnBnClickedBtnCharmsg)
	ON_BN_CLICKED(IDC_BTN_MOVE, OnBnClickedBtnMove)
	ON_BN_CLICKED(IDC_BTN_BAN, OnBnClickedBtnBan)
	ON_BN_CLICKED(IDC_BTN_KICKOUT, OnBnClickedBtnKickout)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBnClickedBtnDisconnect)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CHGIP, OnBnClickedBtnChgip)
	ON_BN_CLICKED(IDC_BTN_ANNOUNCE, OnBnClickedBtnAnnounce)
	ON_BN_CLICKED(IDC_BTN_MONSTER, OnBnClickedBtnMonster)
END_MESSAGE_MAP()


// CGMToolDlg 메시지 처리기

BOOL CGMToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetEnableMenu(MANAGER_COUNT);

	m_dwPort = DEF_PORT;	
	m_strIP = DEF_IP;
	LoadConfig();

	SetConfigText(m_strIP,m_dwPort);

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CGMToolDlg::OnPaint() 
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
HCURSOR CGMToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGMToolDlg::OnBnClickedBtnDisconnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_bDisconnect)
		return;

	int nRet = AfxMessageBox(_T("Are you sure you want to disconnect?"),MB_YESNO); // 진정 접속을 종료하겠습니까?
	if(nRet != IDYES)
	{
		m_bDisconnect = FALSE;
		return;
	}
	m_bDisconnect = TRUE;
	
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CHGIP)->EnableWindow(TRUE);
	
	SetShowChildDlgAll(FALSE);

	CGMToolDoc * pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return ;

	pDoc->DisconnectServer();
	m_pdlgUserMove->m_pdlgPosList->m_bLoad = FALSE;
	SetEnableMenu(MANAGER_COUNT);
	SaveConfig();

	// 메인 윈도우 타이틀 바꾸기	
	CString strTitle = _T(" - GMTool ");
	CWnd* pWnd = AfxGetMainWnd();	
	pWnd->SetWindowText(strTitle);
	
}

void CGMToolDlg::OnBnClickedBtnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetConfigText(m_strIP,m_dwPort);

	CGMToolDoc * pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;

	GetDlgItemText(IDC_EDIT_ID,m_strID);
	GetDlgItemText(IDC_EDIT_PW,m_strPW);
	pDoc->SetUserData(m_strID,m_strPW);
	pDoc->SessionStart(m_strIP,m_dwPort);
}


void CGMToolDlg::LoadConfig()
{
	FILE* pf = fopen(CONFIG_FILE,"rt");
	if( !pf )
		return;

	char strIP[50]={0,};
	DWORD dwPort;
	if( fscanf(pf,"%s %d",strIP,&dwPort) == EOF )
	{
		fclose(pf);
		return;
	}

	m_strIP = strIP;
	m_dwPort = dwPort;
	fclose(pf);
}

void CGMToolDlg::SaveConfig()
{
	FILE* pf = fopen(CONFIG_FILE,"wt");
	if( !pf )
		return;

	fprintf(pf,"%s %d",m_strIP, m_dwPort);
	fclose(pf);
}

void CGMToolDlg::SetConfigText(CString strIP,DWORD dwPort)
{	
	CString strInfo;
	strInfo.Format("[IP] %s  [PORT] %d",m_strIP,m_dwPort);
	SetDlgItemText(IDC_STATIC_INFO,strInfo);
}

void CGMToolDlg::SetIPPort(CString strIP, DWORD dwPort)
{
	m_strIP = strIP;
	m_dwPort = dwPort;
	SetConfigText(m_strIP,m_dwPort);
}

void CGMToolDlg::SetEnableMenu(BYTE bLevel)
{
	GetDlgItem(IDC_BTN_BAN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_KICKOUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CHARMSG)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_MOVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANNOUNCE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_MONSTER)->EnableWindow(FALSE);

	switch(bLevel)
	{
	case MANAGER_ALL:
	case MANAGER_CONTROL:
	case MANAGER_USER:
	case MANAGER_SERVICE:
	case MANAGER_GMLEVEL1:
		{
			GetDlgItem(IDC_BTN_ANNOUNCE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_MONSTER)->EnableWindow(TRUE);			
		}
		
	case MANAGER_GMLEVEL2:
		{
			GetDlgItem(IDC_BTN_BAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_KICKOUT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_CHARMSG)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_MOVE)->EnableWindow(TRUE);
		}
		
	default:
		break;
	}
}

void CGMToolDlg::OnBnClickedBtnCharmsg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pdlgCharMsg->ShowWindow(SW_SHOW);
}

void CGMToolDlg::OnBnClickedBtnMove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pdlgUserMove->Init();
	m_pdlgUserMove->SetWorld();	
	m_pdlgUserMove->ShowWindow(SW_SHOW);	
}

void CGMToolDlg::OnBnClickedBtnBan()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pdlgChatBan->Init();
	m_pdlgChatBan->ShowWindow(SW_SHOW);	
}

void CGMToolDlg::OnBnClickedBtnKickout()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pdlgUserKickOut->ShowWindow(SW_SHOW);
}

void CGMToolDlg::OnBnClickedBtnChgip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pdlgIPChange->ShowWindow(SW_SHOW);
	m_pdlgIPChange->m_ctrIPAddr.SetFocus();
}

void CGMToolDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedBtnDisconnect();
	
	if(!m_bDisconnect)
		return;

	if(m_pdlgCharMsg)
	{
		m_pdlgCharMsg->DestroyWindow();
		delete m_pdlgCharMsg;
		m_pdlgCharMsg = NULL;
	}

	if(m_pdlgUserMove)
	{
		m_pdlgUserMove->DestroyWindow();
		delete m_pdlgUserMove;
		m_pdlgUserMove = NULL;
	}

	if(m_pdlgChatBan)
	{
		m_pdlgChatBan->DestroyWindow();
		delete m_pdlgChatBan;
		m_pdlgChatBan = NULL;
	}

	if(m_pdlgUserKickOut)
	{
		m_pdlgUserKickOut->DestroyWindow();
		delete m_pdlgUserKickOut;
		m_pdlgUserKickOut = NULL;
	}

	if(m_pdlgIPChange)
	{
		m_pdlgIPChange->DestroyWindow();
		delete m_pdlgIPChange;
		m_pdlgIPChange = NULL;
	}

	if(m_pdlgAnnounce)
	{
		m_pdlgAnnounce->DestroyWindow();
		delete m_pdlgAnnounce;
		m_pdlgAnnounce = NULL;
	}

	if(m_pdlgMonsterSpawn)
	{
		m_pdlgMonsterSpawn->DestroyWindow();
		delete m_pdlgMonsterSpawn;
		m_pdlgMonsterSpawn = NULL;
	}


	OnOK();
}

void CGMToolDlg::SetShowChildDlgAll(BOOL _bShow)
{
	int nShow = 0;
	if(_bShow)
		nShow = SW_SHOW;
	else
		nShow = SW_HIDE;

	m_pdlgCharMsg->ShowWindow(nShow);
	m_pdlgUserMove->SetShowChildDlgAll(_bShow);
	m_pdlgUserMove->ShowWindow(nShow);
	m_pdlgChatBan->ShowWindow(nShow);
	m_pdlgUserKickOut->ShowWindow(nShow);
	m_pdlgIPChange->ShowWindow(nShow);
	m_pdlgAnnounce->ShowWindow(nShow);
	m_pdlgMonsterSpawn->ShowWindow(nShow);
}


BOOL CGMToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 에디트 박스에서 리턴키 막기.
	// 에디트 박스 속성에서 Password 가 TRUE 면 Multiline(true),want return(true) 속성이 적용안됨
	// 코드에서 엔터키 적용안되도록 막음

	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_PW); 
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->hwnd == pEdit->m_hWnd)
		{
			if(pMsg->wParam == VK_RETURN)
				return FALSE;
		}
	}   

	return CDialog::PreTranslateMessage(pMsg);
}

void CGMToolDlg::OnBnClickedBtnAnnounce()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pdlgAnnounce->ShowWindow(SW_SHOW);
	m_pdlgAnnounce->SetWorld();
}

void CGMToolDlg::OnBnClickedBtnMonster()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pdlgMonsterSpawn->ShowWindow(SW_SHOW);
	m_pdlgMonsterSpawn->SetWorld();
}
