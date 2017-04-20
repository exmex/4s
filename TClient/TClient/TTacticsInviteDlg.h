
#define TACTICS_INVITE_DLG_MODE_EDIT 0
#define TACTICS_INVITE_DLG_MODE_VIEW 1

class CTTacticsInviteDlg : public CTClientUIBase
{
public:
	CTTacticsInviteDlg( TComponent* pParent, LP_FRAMEDESC pDesc);
	~CTTacticsInviteDlg();

	TEdit* GetCurEdit();
	virtual void ShowComponent( BOOL bVisible);

	void SetInviteCond(
		CString& strTarget );

	void SetReInviteCond(
		CString strTarget );

	void SetShowAccept(
		CString strTarget,
		CString strInviter,
		CString strGuild,
		BYTE bDay,
		DWORD dwPoint,
		DWORD dwGold,
		DWORD dwSilver,
		DWORD dwCooper );

	int m_nMode;

	CString m_strTarget;
	CString m_strInviter;
	TComponent* m_pTITLE;
	TComponent* m_pMSG;
	CTNumberEdit* m_pPeriodEdit;
	CTNumberEdit* m_pPointEdit;
	CTNumberEdit* m_pRuneEdit;
	CTNumberEdit* m_pLunaEdit;
	CTNumberEdit* m_pCronEdit;
	TButton* m_pBtnOK;
	TButton* m_pBtnCancel;

	TComponent* m_pPeriod;
	TComponent* m_pHornor;
	TComponent* m_pRune;
	TComponent* m_pLuna;
	TComponent* m_pCron;
	BOOL m_bCloseUI;
};