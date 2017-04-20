// GMToolDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "GMToolDlg.h"
#include ".\gmtooldlg.h"
#include "MainFrm.h"
#include "HappyDoc.h"


// CGMToolDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGMToolDlg, CDialog)
CGMToolDlg::CGMToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGMToolDlg::IDD, pParent)
{
	m_dlgAnnounce = new CAnnounce();
	m_dlgAnnounce->Create(IDD_ANNOUNCE);

	m_dlgMonsterSpawn = new CMonsterSpawn();
	m_dlgMonsterSpawn->Create(IDD_MONSTERSPAWN);

	m_dlgCharMsg = new CCharMsg();
	m_dlgCharMsg->Create(IDD_CHARMSG);

	m_dlgUserMove = new CUserMove();
	m_dlgUserMove->Create(IDD_USERMOVE);

	m_dlgUserKickout = new CUserKickout();
	m_dlgUserKickout->Create(IDD_USERKICKOUT);

	m_dlgGuildChange = new CGuildChange();
	m_dlgGuildChange->Create(IDD_GUILDCHANGE);

	m_dlgLocalInit = new CLocalInit();
	m_dlgLocalInit->Create(IDD_LOCALINIT);

	m_dlgChatBan = new CChatBanDlg();
	m_dlgChatBan->Create(IDD_CHATBAN);

	m_dlgItemChgState = new CItemChgState();
	m_dlgItemChgState->Create(IDD_ITEMSTATE);
	m_dlgItemChgState->InitFindListCtrl();

	m_dlgCastleSetting = new CCastleSettingDlg();
	m_dlgCastleSetting->Create(IDD_CASTLE);
	m_dlgCastleSetting->InitCastleInfoListControl();

	m_dlgEvent = new CEventDlg();
	m_dlgEvent->Create(IDD_EVENT);
	
	m_dlgLuckyEvent = new CLuckyEventDlg();
	m_dlgLuckyEvent->Create(IDD_LUCKYEVENT);

	m_dlgTournament = new CTournament();
	m_dlgTournament->Create(IDD_TOURNAMENTINFO);

	m_dlgHelpMessage = new CHelpMessage();
	m_dlgHelpMessage->Create(IDD_HELPINFO);

#ifdef RPSGAME_ON
	m_dlgRPSGame = new CRPSGame();
	m_dlgRPSGame->Create(IDD_RPSGAME);
#endif

	m_dlgCMGift = new CTGiftItem();
	m_dlgCMGift->Create(IDD_GIFTTOOL);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	pDoc->m_pdlgGMTool = this;
}

CGMToolDlg::~CGMToolDlg()
{
	if(m_dlgAnnounce)
	{
		m_dlgAnnounce->DestroyWindow();
		delete m_dlgAnnounce;
		m_dlgAnnounce = NULL;
	}

	if(m_dlgMonsterSpawn)
	{
		m_dlgMonsterSpawn->DestroyWindow();
		delete m_dlgMonsterSpawn;
		m_dlgMonsterSpawn = NULL;
	}

	if(m_dlgCharMsg)
	{
		m_dlgCharMsg->DestroyWindow();
		delete m_dlgCharMsg;
		m_dlgCharMsg = NULL;
	}

	if(m_dlgUserMove)
	{
		m_dlgUserMove->DestroyWindow();
		delete m_dlgUserMove;
		m_dlgUserMove = NULL;
	}

	if(m_dlgUserKickout)
	{
		m_dlgUserKickout->DestroyWindow();
		delete m_dlgUserKickout;
		m_dlgUserKickout = NULL;
	}

	if(m_dlgGuildChange)
	{
		m_dlgGuildChange->DestroyWindow();
		delete m_dlgGuildChange;
		m_dlgGuildChange = NULL;
	}

	if(m_dlgLocalInit)
	{
		m_dlgLocalInit->DestroyWindow();
		delete m_dlgLocalInit;
		m_dlgLocalInit = NULL;
	}

	if(m_dlgChatBan)
	{
		m_dlgChatBan->DestroyWindow();
		delete m_dlgChatBan;
		m_dlgChatBan = NULL;
	}

	if(m_dlgItemChgState)
	{
		m_dlgItemChgState->DestroyWindow();
		delete m_dlgItemChgState;
		m_dlgItemChgState = NULL;
	}

	if(m_dlgCastleSetting)
	{
		m_dlgCastleSetting->DestroyWindow();
		delete m_dlgCastleSetting;
		m_dlgCastleSetting = NULL;
	}

	if(m_dlgEvent)
	{
		m_dlgEvent->DestroyWindow();
		delete m_dlgEvent;
		m_dlgEvent = NULL;
	}

	if(m_dlgLuckyEvent)
	{
		m_dlgLuckyEvent->DestroyWindow();
		delete m_dlgLuckyEvent;
		m_dlgLuckyEvent = NULL;
	}

	if(m_dlgTournament)
	{
		m_dlgTournament->DestroyWindow();
		delete m_dlgTournament;
		m_dlgTournament = NULL;
	}

	if(m_dlgHelpMessage)
	{
		m_dlgHelpMessage->DestroyWindow();
		delete m_dlgHelpMessage;
		m_dlgHelpMessage = NULL;
	}

#ifdef RPSGAME_ON
	if(m_dlgRPSGame)
	{
		m_dlgRPSGame->DestroyWindow();
		delete m_dlgRPSGame;
		m_dlgRPSGame = NULL;
	}
#endif

	if(m_dlgCMGift)
	{
		m_dlgCMGift->DestroyWindow();
		delete m_dlgCMGift;
		m_dlgCMGift = NULL;
	}
}

void CGMToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_ITEMSTATE, m_btnItemState);
	DDX_Control(pDX, IDC_BTN_CASTLE, m_btnCastle);
	DDX_Control(pDX, IDC_BUTTON_ANNOUNCE, m_btnNotice);
	DDX_Control(pDX, IDC_BUTTON_MONSTERSPAWN, m_btnMonster);
	DDX_Control(pDX, IDC_BUTTON_CHARMSG, m_btnMessage);
	DDX_Control(pDX, IDC_BUTTON_USERMOVE, m_btnMove);
	DDX_Control(pDX, IDC_BTN_CHATBAN, m_btnChatBan);
	DDX_Control(pDX, IDC_BUTTON_USERKICK, m_btnKickOut);
	DDX_Control(pDX, IDC_BTN_EVENT, m_btnEvent);
	DDX_Control(pDX, IDC_BTN_LUCKYEVENT, m_btnLuckyEvent);
	DDX_Control(pDX, IDC_BTN_TONAMENT, m_btnTournament);	
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelpMsg);
	DDX_Control(pDX, IDC_BTN_RPS, m_btnRPSGame);
}


BEGIN_MESSAGE_MAP(CGMToolDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ANNOUNCE, OnBnClickedButtonAnnounce)
	ON_BN_CLICKED(IDC_BUTTON_MONSTERSPAWN, OnBnClickedButtonMonsterspawn)
	ON_BN_CLICKED(IDC_BUTTON_USERMOVE, OnBnClickedButtonUsermove)
	ON_BN_CLICKED(IDC_BUTTON_CHARMSG, OnBnClickedButtonCharmsg)
	ON_BN_CLICKED(IDC_BUTTON_USERKICK, OnBnClickedButtonUserkick)
	ON_BN_CLICKED(IDC_BUTTON_GUILDCHANGE, OnBnClickedButtonGuildchange)
	ON_BN_CLICKED(IDC_BUTTON_LOCALINIT, OnBnClickedButtonLocalinit)
	ON_BN_CLICKED(IDC_BTN_CHATBAN, OnBnClickedBtnChatban)
	ON_BN_CLICKED(IDC_BTN_ITEMSTATE, OnBnClickedBtnItemState)
	ON_BN_CLICKED(IDC_BTN_CASTLE, OnBnClickedBtnCastle)
	ON_BN_CLICKED(IDC_BTN_EVENT, OnBnClickedBtnEvent)
	ON_BN_CLICKED(IDC_BTN_LUCKYEVENT, OnBnClickedBtnLuckyevent)
	ON_BN_CLICKED(IDC_BTN_TONAMENT, OnBnClickedBtnTonament)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBnClickedBtnHelp)
	ON_BN_CLICKED(IDC_BTN_RPS, OnBnClickedBtnRps)
	ON_BN_CLICKED(IDC_BTN_GIFT, OnBnClickedBtnGift)
END_MESSAGE_MAP()


// CGMToolDlg 메시지 처리기입니다.

void CGMToolDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CGMToolDlg::OnBnClickedButtonAnnounce()
{
	m_dlgAnnounce->SetWorld();
	m_dlgAnnounce->ShowWindow(SW_SHOW);
}

void CGMToolDlg::OnBnClickedButtonMonsterspawn()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	pDoc->m_pMonSpawn = m_dlgMonsterSpawn;

	m_dlgMonsterSpawn->SetWorld();
	m_dlgMonsterSpawn->ShowWindow(SW_SHOW);
}

void CGMToolDlg::OnBnClickedButtonUsermove()
{
	m_dlgUserMove->Init();
	m_dlgUserMove->ShowWindow(SW_SHOW);
}

void CGMToolDlg::OnBnClickedButtonCharmsg()
{
	m_dlgCharMsg->ShowWindow(SW_SHOW);
}

void CGMToolDlg::OnBnClickedButtonUserkick()
{
	m_dlgUserKickout->ShowWindow(SW_SHOW);
}

void CGMToolDlg::OnBnClickedButtonGuildchange()
{
	m_dlgGuildChange->ShowWindow(SW_SHOW);
}

void CGMToolDlg::OnBnClickedButtonLocalinit()
{
	m_dlgLocalInit->ShowWindow(SW_SHOW);
}

void CGMToolDlg::OnBnClickedBtnRps()
{
#ifdef RPSGAME_ON
	m_dlgRPSGame->Init();
	m_dlgRPSGame->ShowWindow(SW_SHOW);
#endif

}

void CGMToolDlg::OnBnClickedBtnHelp()
{
	m_dlgHelpMessage->Init();
	m_dlgHelpMessage->ShowWindow(SW_SHOW);
}

void CGMToolDlg::OnBnClickedBtnChatban()
{
	m_dlgChatBan->ShowWindow(SW_SHOW);
	m_dlgChatBan->Init();
}

void CGMToolDlg::OnBnClickedBtnItemState()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(pDoc->m_bAuthority != MANAGER_ALL)
	{
		AfxMessageBox("You don't have the authority");
		return;
	}

	m_dlgItemChgState->ShowWindow(SW_SHOW);
	m_dlgItemChgState->Init();
}

void CGMToolDlg::OnBnClickedBtnCastle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_dlgCastleSetting->Init();
	m_dlgCastleSetting->SetWorld();
	m_dlgCastleSetting->ShowWindow(SW_SHOW);	
}

void CGMToolDlg::OnBnClickedBtnEvent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_dlgEvent->ShowWindow(SW_SHOW);
	m_dlgEvent->Init();

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	pDoc->SendCT_EVENTLIST_REQ();
	pDoc->SendCT_CASHITEMLIST_REQ();
}

void CGMToolDlg::OnBnClickedBtnLuckyevent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_dlgLuckyEvent->ShowWindow(SW_SHOW);
	m_dlgLuckyEvent->Init();
}

void CGMToolDlg::OnBnClickedBtnTonament()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_dlgTournament->ShowWindow(SW_SHOW);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	m_dlgTournament->ClearCtrl();
	m_dlgTournament->Init();
	m_dlgTournament->InitWorld();
}


void CGMToolDlg::SetShowChildDlgAll(BOOL _bShow)
{
	int nShow = 0;
	if(_bShow)
		nShow = SW_SHOW;
	else
		nShow = SW_HIDE;

	m_dlgAnnounce->ShowWindow(nShow);
	m_dlgMonsterSpawn->ShowWindow(nShow);
	m_dlgCharMsg->ShowWindow(nShow);
	m_dlgUserMove->ShowWindow(nShow);
	m_dlgUserKickout->ShowWindow(nShow);
	m_dlgGuildChange->ShowWindow(nShow);
	m_dlgLocalInit->ShowWindow(nShow);
	m_dlgChatBan->ShowWindow(nShow);
	m_dlgItemChgState->ShowWindow(nShow);
	m_dlgCastleSetting->ShowWindow(nShow);
	m_dlgEvent->ShowWindow(nShow);
	m_dlgLuckyEvent->ShowWindow(nShow);
	m_dlgTournament->ShowWindow(nShow);
	m_dlgHelpMessage->ShowWindow(nShow);
#ifdef RPSGAME_ON
	m_dlgRPSGame->ShowWindow(nShow);
#endif
}

void CGMToolDlg::Init()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	SetEnableBtn(pDoc->m_bAuthority);	
}

void CGMToolDlg::SetEnableBtn(BYTE bLevel)
{
	m_btnItemState.EnableWindow(FALSE);
	m_btnCastle.EnableWindow(FALSE);
	m_btnNotice.EnableWindow(FALSE);
	m_btnMonster.EnableWindow(FALSE);
	m_btnMessage.EnableWindow(FALSE);
	m_btnMove.EnableWindow(FALSE);
	m_btnChatBan.EnableWindow(FALSE);
	m_btnKickOut.EnableWindow(FALSE);
	m_btnEvent.EnableWindow(FALSE);
	m_btnLuckyEvent.EnableWindow(FALSE);
	m_btnTournament.EnableWindow(FALSE);
	m_btnHelpMsg.EnableWindow(FALSE);
#ifdef RPSGAME_ON
	m_btnRPSGame.EnableWindow(FALSE);
#else
	m_btnRPSGame.ShowWindow(SW_HIDE);
#endif

	switch(bLevel)
	{
	case MANAGER_ALL:		
			m_btnItemState.EnableWindow(TRUE);
			m_btnCastle.EnableWindow(TRUE);
			m_btnLuckyEvent.EnableWindow(TRUE);
			m_btnTournament.EnableWindow(TRUE);
			m_btnHelpMsg.EnableWindow(TRUE);
			m_btnRPSGame.EnableWindow(TRUE);
	case MANAGER_CONTROL:				
	case MANAGER_USER:					
	case MANAGER_SERVICE:		
	case MANAGER_GMLEVEL1:	
			m_btnNotice.EnableWindow(TRUE);
			m_btnMonster.EnableWindow(TRUE);	
	case MANAGER_GMLEVEL2:	
			m_btnMessage.EnableWindow(TRUE);
			m_btnMove.EnableWindow(TRUE);
			m_btnChatBan.EnableWindow(TRUE);
			m_btnKickOut.EnableWindow(TRUE);	
	case MANAGER_GMLEVEL3:			
			m_btnEvent.EnableWindow(TRUE);
	default:
		break;
	};
}

void CGMToolDlg::OnBnClickedBtnGift()
{
	m_dlgCMGift->ShowWindow(SW_SHOW);
	m_dlgCMGift->Init();
}
