#pragma once

class CTGuildVolunteerFrame : public ITInnerFrame
{
public:
	enum GuildRegularVolunteerInfo
	{
		GRVI_NAME=0,
		GRVI_CLASS=1,
		GRVI_LEVEL=2,
		GRVI_REGION=3,
		GRVI_COUNT
	};

	enum GuildTacticsVolunteerInfo
	{
		GTVI_NAME=0,
		GTVI_CLASS=1,
		GTVI_LEVEL=2,
		GTVI_DAY=3,
		GTVI_POINT=4,
		GTVI_MONEY=5,
		GTVI_REGION=6,
		GTVI_COUNT
	};

	/// 팝업 버튼 정보
	enum PopupMenu
	{
		GVFPM_MAIL=0,
		GVFPM_REGULAR_ACCEPT,
		GVFPM_REGULAR_REJECT,
		GVFPM_TACTICS_ACCEPT,
		GVFPM_TACTICS_REJECT,
		GVFPM_CLOSE,
		GVFPM_COUNT
	};

	static struct PopupBtnInfo
	{
		DWORD			m_dwTXT;
		DWORD			m_dwGM;
	}
	m_PopupBtnInfo[GVFPM_COUNT];

public:
	UINT m_nCurPage;
	TButton* m_pTAB[2];
	INT m_nTABType[2];
	TList* m_pList;
	TList* m_pColumnC;
	CTMiniPopupDlg* m_pPopup;
	BOOL m_bPopupVisible;
	TListItem* m_pDownItem;
	INT m_nLastSelType;
	INT m_nLastSelIdx;
	
	void ShowPopup(const CPoint& pt);
	void CancelPopup();
	void ChangePage( INT nType );
	void ResetTabButton( BYTE bTabIndex );

	virtual void RequestInfo();
	virtual void ResetInfo();

	int OnGM_GVFPM_MAIL();	///< GVFPM_MAIL
	int OnGM_GVFPM_REGULAR_ACCEPT();	///< GVFPM_REGULAR_ACCEPT
	int OnGM_GVFPM_REGULAR_REJECT();	///< GVFPM_REGULAR_REJECT
	int OnGM_GVFPM_TACTICS_ACCEPT();	///< GVFPM_TACTICS_ACCEPT
	int OnGM_GVFPM_TACTICS_REJECT();	///< GVFPM_TACTICS_REJECT
	int OnGM_GVFPM_CLOSE();	///< GVFPM_CLOSE

public:
	virtual HRESULT Render( DWORD dwTickCount );
	virtual void OnLButtonDown( UINT nFlags, CPoint pt );
	virtual void OnLButtonUp( UINT nFlags, CPoint pt );
	virtual void ShowComponent(BOOL bVisible);

public:
	CTGuildVolunteerFrame(TComponent *pParent, LP_FRAMEDESC pDesc, LP_FRAMEDESC pPopupDesc);
	virtual ~CTGuildVolunteerFrame();
};