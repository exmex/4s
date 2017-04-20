// GMToolDlg.h : 헤더 파일
//

#pragma once

#include "CharMsgDlg.h"
#include "UserMoveDlg.h"
#include "ChatBanDlg.h"
#include "UserKickOutDlg.h"
#include "IPChangeDlg.h"
#include "AnnounceDlg.h"
#include "MonsterSpawn.h"

// CGMToolDlg 대화 상자
class CGMToolDlg : public CDialog
{
// 생성
public:
	CGMToolDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_GMTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CCharMsgDlg*		m_pdlgCharMsg;
	CUserMoveDlg*		m_pdlgUserMove;
	CChatBanDlg*		m_pdlgChatBan;
	CUserKickOutDlg*	m_pdlgUserKickOut;
	CIPChangeDlg*		m_pdlgIPChange;
	CAnnounceDlg*		m_pdlgAnnounce;
	CMonsterSpawnDlg*	m_pdlgMonsterSpawn;
	

	CString		m_strIP;
	DWORD		m_dwPort;
	CString		m_strID;
	CString		m_strPW;

	BYTE		m_bDisconnect;

	
public:
	void LoadConfig();
	void SaveConfig();
	void SetConfigText(CString strIP,DWORD dwPort);
	void SetIPPort(CString strIP,DWORD dwPort);
	void SetEnableMenu(BYTE bLevel = 0);
	void SetShowChildDlgAll(BOOL _bShow);
	
public:
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnCharmsg();
	afx_msg void OnBnClickedBtnMove();
	afx_msg void OnBnClickedBtnBan();
	afx_msg void OnBnClickedBtnKickout();
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnChgip();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnAnnounce();
	afx_msg void OnBnClickedBtnMonster();
};
