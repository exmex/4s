#pragma once

class CTGuildApplicationDlg : public CTClientUIBase
{
public:
	enum GuildRegularOfferInfo
	{
		GROI_NAME=0,
		GROI_TITLE=1,
		GROI_LEVEL=2,
		GROI_ENDTIME=3,
		GROI_APPLIED=4,
		GROI_COUNT
	};

	enum GuildTacticsOfferInfo
	{
		GTOI_NAME=0,
		GTOI_TITLE,
		GTOI_LEVEL,
		GTOI_MONEY,
		GTOI_POINT,
		GTOI_DAY,
		GTOI_ENDTIME,
		GTOI_APPLIED,
		GTOI_COUNT
	};

public:
	INT m_nCurPage;
	TEdit* GetCurEdit();
	BOOL IsEditMode();
	void RequestInfo();
	void ResetInfo();
	void ChangePage( INT nType );

	TList* m_pList;
	TComponent* m_pText;
	TComponent* m_pColText[8];
	TButton* m_pButton[2];
	INT m_nLastSelType;
	INT m_nLastSelIdx;
	TButton* m_pBtnJOIN;
	TButton* m_pBtnJOINCANCEL;

public:
	virtual void OnNotify( DWORD from, WORD msg, LPVOID param );
	virtual void OnLButtonDown( UINT nFlags, CPoint pt );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint pt );
	virtual void ShowComponent( BOOL bVisible = 1 );
	virtual void ResetPosition();
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
	virtual HRESULT Render( DWORD dwTickCount );

public:
	CTGuildApplicationDlg(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTGuildApplicationDlg();
};