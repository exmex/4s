#pragma once

#include "xskinbutton.h"

#define WS_EX_LAYERED     0x00080000 
#define LWA_COLORKEY      1 
#define LWA_ALPHA         2 

// CPlaySetting 대화 상자입니다.

class CPlaySetting : public CDialog
{
	DECLARE_DYNAMIC(CPlaySetting)

public:
	CPlaySetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPlaySetting();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PLAYSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:	

	//배경스킨
	HBITMAP		hBmp;
	CBitmap		m_bmpBG;
	BYTE		sFlag;
	int			m_iBGWidth;
	int			m_iBGHeight;

	CxSkinButton m_chkNPCName;			// NPC 이름
	CxSkinButton m_chkCharName;			// 캐릭터 이름
	CxSkinButton m_chkMonName;			// 몬스터 이름
	CxSkinButton m_chkAutoHelp;			// 자동 도움말
	CxSkinButton m_chkTalkBox;			// 말풍선 보이기
	CxSkinButton m_chkHUD;				// HUD 보이기
	CxSkinButton m_chkDenyWhisper;		// 귓말 거부
	CxSkinButton m_chkDenyCommunity;	// 커뮤니티 거부
	CxSkinButton m_chkConChat;			// 연속 채팅 지원
	CxSkinButton m_btnClose;			// 창닫기 버튼
	CxSkinButton m_btnSys;				// 시스템 탭 버튼
	CxSkinButton m_chkClickMove;		// 마우스 클릭 이동

	BOOL m_bNPCName;		
	BOOL m_bCharName;
	BOOL m_bMonName;
	BOOL m_bAutoHelp;
	BOOL m_bTalkBox;
	BOOL m_bHUD;
	BOOL m_bDenyWhisper;
	BOOL m_bDenyCom;
	BOOL m_bConChat;
	BOOL m_bClickMove;
	
	CString m_strCountry;

public:

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnSetTab();
	afx_msg void OnBnClickedClosePlayset();
	afx_msg void OnBnClickedChkNpcname();
	afx_msg void OnBnClickedChkCharname();
	afx_msg void OnBnClickedChkMonname();
	afx_msg void OnBnClickedChkAutohelp();
	afx_msg void OnBnClickedChkTalkbox();
	afx_msg void OnBnClickedChkHud();
	afx_msg void OnBnClickedChkDenywhi();
	afx_msg void OnBnClickedChkDenycom();
	afx_msg void OnBnClickedChkConchat();
	afx_msg void OnBnClickedChkClickmove();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	void LoadBGSkin();
	void LoadControlSkin();
	BOOL WriteRegistry();
	BOOL ReadRegistry();
	void SetDefaultOption();
	
	// 스킨 입힌 다이얼로그에서 특정색을 투명색으로 바꾸기.
	// Load한 비트맵(CBitmap)을 넘겨 준다.
	void TransparentDialog(CBitmap* _pCBitmap, COLORREF  _color); 
	void TransparentDialog2(COLORREF _color); // winNT이상 user32.dll 사용
	
};
