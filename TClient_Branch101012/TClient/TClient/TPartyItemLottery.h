#pragma once


class CTPartyItemLottery : public CTClientUIBase
{
public:
	typedef std::vector<CTPartyItemLottery*>		VECTORINSTANCES;
	typedef std::list<CTPartyItemLottery*>			LISTINSTANCES;

protected:
	static VECTORINSTANCES		ms_vFREES;
	static VECTORINSTANCES		ms_vACTIVES;
	
	static TComponent*			ms_pPARENT;
	static LP_FRAMEDESC			ms_pFRAMEDESC;
	
	static CSize				ms_szSCREEN;
	static CPoint				ms_ptINITPOS;
	static CRect				ms_rcRECT;

	static LISTINSTANCES		ms_vGMTARGETS;
		
public:
	static void InitInstance(TComponent* pPARENT, LP_FRAMEDESC pDESC, CSize szSCREEN);
	static void ReleaseInstance();

	static CTPartyItemLottery* NewInstance(DWORD dwMonID, CTClientItem* pItem);
	static void DeleteInstance(CTPartyItemLottery* pInst);
	static void DeleteAllInstance();
	
	static INT GetActiveInstCnt();
	static CTPartyItemLottery* GetActiveInst(INT nIdx);

	static CTPartyItemLottery* GetGMTarget();

protected:
	BOOL			m_bActive;

	DWORD			m_dwMonID;
	CTClientItem*	m_pItem;

	TImageList*		m_pItemIcon;
	TComponent*		m_pItemName;
	
	DWORD			m_dwLeftTime;
	TComponent* 	m_pLeftTime;

	TComponent*		m_pOkButton;
	TComponent*		m_pCancelButton;
	TComponent*		m_pCloseButton;

public:
	BOOL IsActive() const			{ return m_bActive; }
	DWORD GetMonID() const			{ return m_dwMonID; }
	CTClientItem* GetItem() const	{ return const_cast<CTClientItem*>(m_pItem); }

protected:
	void SetData(DWORD dwMonID, CTClientItem* pItem);
	void NotifyGM(DWORD dwGM);

public:
	virtual BOOL CanWithItemUI();
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual HRESULT Render( DWORD dwTickCount );

	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );

protected:
	CTPartyItemLottery( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTPartyItemLottery();
};
