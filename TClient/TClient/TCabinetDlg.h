#pragma once

class CTNormalCabinetDlg;
class CTCabinetSlot;
class CTCabinetItem;

class CTCabinetSlot : public CTCtrlListSlot
{
public:
	BOOL			m_bEmpty;
	TImageList*		m_pIcon;
	TComponent*		m_pName;

public:
	virtual void ShowComponent(BOOL bShow);
	virtual void Select(BOOL bSel);
	
public:
	CTCabinetSlot() : m_bEmpty(TRUE) {}
	virtual ~CTCabinetSlot() {}
};

class CTCabinetItem : public CTCtrlListItem
{
public:
	CTClientItem*	m_pClientItem;
	BYTE			m_bItemCnt;
	DWORD			m_dwStItemID;

public:
	CTCabinetItem* Clone();

public:
	virtual void ReflectSlot(CTCtrlListSlot* pSlot);
	
public:
	CTCabinetItem() : m_pClientItem(NULL) {}
	virtual ~CTCabinetItem();
};

/// 보관함 구현을 위한 베이스 인터페이스 클래스
class CTCabinetDlg : public CTClientUIBase
{
public:
	enum CABUPDATE_STATE
	{
		CABUPDATE_NOTYET,		///< 아직 보관함에 대한 아무런 정보도 가지고 있지 않음
		CABUPDATE_DOING,		///< 보관함 정보를 얻어오는중
		CABUPDATE_COMPLETE,		///< 보관함 정보를 얻어왔음
	};

	struct Cabinet
	{
		BOOL			m_bNeedUpdate;
		BOOL			m_bOpen;
		CTCLItemArray	m_vItems;

		Cabinet() 
			: m_bNeedUpdate(TRUE), m_bOpen(FALSE)
		{}
	};

	TComponent* m_pCharge;
	BOOL m_bShowCashCharge;
	BYTE m_bRememberPrevScrollPos;

protected:
	CABUPDATE_STATE	m_nCabUpdateSt;

	INT				m_nMaxItemSlot;		///< 전체 아이템 슬롯의 수
	INT				m_nMaxItemCol;		///< 아이템 가로 슬롯의 수
	INT				m_nMaxCabCnt;		///< 보관함의 수
	INT				m_nMaxStorage;		///< 한 보관함에 저장할 수 있는 최대 아이템 수

	TComponent*		m_pTitle;
	TComponent*		m_pSelectTxt;
	TComponent*		m_pCostTxt;

	TButton**		m_ppOpenBtn;
	Cabinet*		m_pCabinets;
	CTCtrlList*		m_pList;

	UINT			m_nSelectCab;
	INT				m_nLastHitBtn;
	BOOL			m_bNeedSetComp;
	BOOL			m_bNeedUpdate;

public:
	/**
		하위 클래스는 이 클래스에 다음 멤버들을 반드시 구축해 주어야 한다.

		m_nMaxItemSlot	: 아이템 슬롯의 전체 갯수
		m_nMaxItemCol	: 아이템 슬롯의 가로 갯수
		m_nMaxCabCnt	: 보관함 갯수
		m_pTitle		: 타이틀
		m_pSelectTxt	: 선택
		m_pCostTxt		: 비용
		m_ppOpenBtn		: 보관함 번호 선택 버튼
		m_pCabinets		: 보관함 번호 정보
	*/
	virtual void SetupComponent() = 0;

	/// 보관함 인터페이스를 초기화한다.
	virtual void Initial();

	/// 보관함을 화면에 표시하도록 요청한다.
	virtual void RequestShowComponent() = 0;

	/// 해당 보관함의 열림 닫힘을 설정한다.
	virtual void SetCabinetInfo(UINT nCab, BOOL bOpen);

	/// 주어진 보관함을 선택한다.
	virtual BOOL SelectCab(UINT nCab);

	/// 주어진 보관함을 비운다.
	virtual void Clear(UINT nCab);
	/// 전체 보관함을 비운다.
	virtual void ClearAll();

	/// 보관함을 갱신한다.
	virtual void Update(DWORD dwTick);

	/// 해당 보관함을 열기위한 가격을 얻는다.
	virtual INT64 GetCabOpenCost(UINT nCab);
	/// 해당 보관함을 이용하기 위한 가격을 얻는다.
	virtual INT64 GetCabUseCost(UINT nCab);

public:
	/// 보관함의 정보 갱신이 완료되었다고 설정한다.
	void CompleteCabUpdate();
	/// 주어진 보관함의 정보 갱신이 완료되었다고 설정한다.
	void CompleteCabinetUpdate(UINT nCab);

	/// 주어진 보관함에 아이템을 추가한다.
	virtual BOOL AddItem(UINT nCab, CTCabinetItem* pItem);
	/// 주어진 보관함에서 아이템을 삭제한다.
	VOID DeleteItem(UINT nCab, DWORD dwCabintItemID);
	/// StID 를 가지고 보관함에서 아이템을 찾는다.
	CTCabinetItem* GetCabItemByStID(UINT nCab, DWORD dwStItemID);
	/// 현재 선택된 아이템을 얻는다.
	CTCabinetItem* GetSelectCabItem();

	/// 현재 선택된 보관함을 얻는다.
	UINT GetSelectCab() const;

	/// 보관함의 용량을 설정한다.
	void SetMaxStorage(INT nMax);
	/// 보관함의 타이틀을 설정한다.
	void SetTitle(const CString& strTITLE);

	/// 이 보관함의 갱신이 필요함을 통보한다.
	void NotifyUpdate();

	void SelectLastestItem();

public:
	virtual void SwitchFocus(TComponent* pCandidate);
	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual HRESULT Render(DWORD dwTickCount);

	virtual TDROPINFO OnDrop(CPoint point);
	virtual BYTE OnBeginDrag(LPTDRAG pDRAG, CPoint point);

	virtual ITDetailInfoPtr GetTInfoKey(const CPoint& point );
	virtual BOOL GetTChatInfo(const CPoint& point, TCHATINFO& outInfo );

public:
	CTCtrlList* GetList() const							{ return m_pList; }
	CTCLItemArray& GetItemArray(UINT nCab)				{ return m_pCabinets[nCab].m_vItems; }
	const CTCLItemArray& GetItemArray(UINT nCab) const	{ return m_pCabinets[nCab].m_vItems; }
	Cabinet* GetCabinet(UINT nCab)						{ return &m_pCabinets[nCab]; }
	const Cabinet* GetCabinet(UINT nCab) const			{ return &m_pCabinets[nCab]; }
	BOOL IsOpen(UINT nCab) const						{ return m_pCabinets[nCab].m_bOpen; }
	INT GetLastHitBtn() const							{ return m_nLastHitBtn; }
	INT GetMaxStorage() const							{ return m_nMaxStorage; }
	
public:
	CTCabinetDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTCabinetDlg();
};

