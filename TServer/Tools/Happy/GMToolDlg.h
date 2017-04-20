#pragma once

#include "Announce.h"
#include "MonsterSpawn.h"
#include "CharMsg.h"
#include "UserMove.h"
#include "UserKickout.h"
#include "GuildChange.h"
#include "LocalInit.h"
#include "ChatBanDlg.h"
#include "ItemChgState.h"
#include "CastleSettingDlg.h"
#include "EventDlg.h"
#include "LuckyEventDlg.h"
#include "Tournament.h"
#include "HelpMessage.h"
#include "RPSGame.h"
#include "TGiftItem.h"

#include "afxwin.h"


// CGMToolDlg 대화 상자입니다.

class CGMToolDlg : public CDialog
{
	DECLARE_DYNAMIC(CGMToolDlg)

public:
	CGMToolDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGMToolDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GMTOOLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CAnnounce *		m_dlgAnnounce;
	CMonsterSpawn *	m_dlgMonsterSpawn;
	CCharMsg *		m_dlgCharMsg;
	CUserMove *		m_dlgUserMove;
	CUserKickout *	m_dlgUserKickout;
	CGuildChange *	m_dlgGuildChange;
	CLocalInit *	m_dlgLocalInit;
	CChatBanDlg*	m_dlgChatBan;
	CItemChgState*	m_dlgItemChgState;
	CCastleSettingDlg* m_dlgCastleSetting;
	CEventDlg*			m_dlgEvent;
	CLuckyEventDlg*		m_dlgLuckyEvent;
	CTournament*		m_dlgTournament;
	CHelpMessage*		m_dlgHelpMessage;
	CRPSGame*			m_dlgRPSGame;
	CTGiftItem*			m_dlgCMGift;

	CButton m_btnItemState;
	CButton m_btnCastle;
	CButton m_btnNotice;
	CButton m_btnMonster;
	CButton m_btnMessage;
	CButton m_btnMove;
	CButton m_btnChatBan;
	CButton m_btnKickOut;
	CButton m_btnEvent;
	CButton m_btnLuckyEvent;
	CButton m_btnTournament;
	CButton m_btnHelpMsg;
	CButton m_btnRPSGame;

public:
	void SetShowChildDlgAll(BOOL _bShow);
	void Init();
	void SetEnableBtn(BYTE bLevel);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAnnounce();
	afx_msg void OnBnClickedButtonMonsterspawn();
	afx_msg void OnBnClickedButtonUsermove();
	afx_msg void OnBnClickedButtonCharmsg();
	afx_msg void OnBnClickedButtonUserkick();
	afx_msg void OnBnClickedButtonGuildchange();
	afx_msg void OnBnClickedButtonLocalinit();
	afx_msg void OnBnClickedBtnChatban();	
	afx_msg void OnBnClickedBtnItemState();
	afx_msg void OnBnClickedBtnCastle();	
	afx_msg void OnBnClickedBtnEvent();	
	afx_msg void OnBnClickedBtnLuckyevent();	
	afx_msg void OnBnClickedBtnTonament();
	afx_msg void OnBnClickedBtnHelp();
	afx_msg void OnBnClickedBtnRps();
	afx_msg void OnBnClickedBtnGift();
};
