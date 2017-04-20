#pragma once

class CTGuildMemberFrame : public ITInnerFrame
{
public:
	typedef CTGuildCommander::GuildDetInfo GuildDetInfo;
	typedef CTGuildCommander::GuildMember GuildMember;
	typedef CTGuildCommander::GuildTactics GuildTactics;
	typedef CTGuildCommander::GuildMemberVec GuildMemberVec;
	typedef CTGuildCommander::GuildTacticsVec GuildTacticsVec;
	typedef CTGuildCommander::PopupBtnInfo PopupBtnInfo;

public:
	static const PopupBtnInfo m_PopupBtnInfo[TGM_PBTN_COUNT];

public:
	void ChangePage( INT nType );
	void RequestInfo();
	void ResetInfo();
	void ShowPopup(const CPoint& pt);
	void CancelPopup();
	BOOL IsPopupVisible() const;
	//void EnableGuildUI( BOOL bEnable );
	BOOL ShowPopupCastle( WORD wCastleID );
	void ResetTabButton( BYTE bTabIndex );

public:
	virtual HRESULT Render( DWORD dwTickCount );
	virtual void OnLButtonDown( UINT nFlags, CPoint pt );
	virtual void OnLButtonUp( UINT nFlags, CPoint pt );
	virtual void ShowComponent(BOOL bVisible);

public:
	int OnGM_GMP_WHISPER();		
	int OnGM_GMP_MAIL();
	int OnGM_GMP_PARTY();
	int OnGM_GMP_FRIEND();
	int OnGM_GMP_KICK();
	int OnGM_GMP_DISORG_TACTICS();
	int OnGM_GMP_ET_GM();		
	int OnGM_GMP_SET_GSM();		
	int OnGM_GMP_CANCEL_GSM();	
	int OnGM_GMP_PEER_BARON();	
	int OnGM_GMP_PEER_VISCOUNT();
	int OnGM_GMP_PEER_COUNT();	
	int OnGM_GMP_PEER_MARQUIS();	
	int OnGM_GMP_PEER_DUKE();	
	int OnGM_GMP_CANCEL_PEER();	
	int OnGM_GMP_CANCEL();
	int OnGM_GMP_POINT_REWARD();
	int OnGM_GMP_VOLUNTEER_HESED();
	int OnGM_GMP_VOLUNTEER_ARDRI();
	int OnGM_GMP_VOLUNTEER_TYCON();
	int OnGM_GMP_VOLUNTEER_GEHBRA();
	int OnGM_GMP_VOLUNTEER_CANCEL();
	int OnGM_GMP_TACTICS_RE_CONTRACT();
	int OnGM_GMP_TACTICS_KICK();
	int OnGM_GMP_TACTICS_SELF_KICK();
	int OnGM_GMP_TACTICS_REWARD_POINT();

public:
	TList* m_pColumnC;
	TList* m_pColumnD;
	TList* m_pListC;
	TList* m_pListD;
	TComponent* m_pMissionTxt;
	TComponent* m_pMemCntTxt;
	TButton* m_pTAB[4];
	INT m_nTABType[4];
	INT m_nCurPage;
	BOOL m_bPopupVisible;
	CTMiniPopupDlg* m_pPopup;
	INT m_nLastSelType;
	INT m_nLastSelIdx;
	TListItem* m_pDownItem;
	//BOOL m_bGuildEnable;

public:
	CTGuildMemberFrame(TComponent *pParent, LP_FRAMEDESC pDesc, LP_FRAMEDESC pPopupDesc);
	virtual ~CTGuildMemberFrame();
};









