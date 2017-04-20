#pragma once


class CTSkillDlg : public CTClientUIBase
{
public:
	MAPTSKILLTEMP	m_mapTSKILL;		///< 배울수 있는 스킬 목록
	MAPTSKILLTEMP	m_mapTINITSKILL;	///< 초기화 가능한 스킬 목록

	CTClientChar*	m_pMainChar;
	CPoint			m_vPOS;

	TTabCtrl*		m_pTTAB;
	
	BYTE 			m_bTRADE;			///< 스킬을 배우는 중인가?
	BYTE 			m_bSKILLINIT;		///< 스킬을 초기화 하는 중인가?
	BYTE			m_bSiInvenID;		///< 스킬 초기화 아이템의 인벤 아이디
	BYTE			m_bSiSlotID;		///< 스킬 초기화 아이템의 인벤에서의 슬롯 아이디
	WORD			m_bSiSelectID;		///< 스킬 초기화를 위해 가장 최근에 선택된 스킬의 아이디

	LPTOPENBYCASH m_pOpenByCash;
	void SetOpenByCash( LPTOPENBYCASH );
	void ClearOpenByCash();

public:
	/// 해당 스킬에 대한 배우기 버튼을 표시할지 여부를 얻는다.
	BYTE IsButtonVisible(
		LPTSKILL pTSKILL,
		BYTE bLevel);
	
	/// 현재 선택된 탭을 얻는다.
	BYTE GetCurSel();
	
	/// 스킬 초기화 모드를 시작한다.
	BYTE StartSkillInitMode(BYTE bInvenID, BYTE bSlotID);
	/// 스킬 초기화 모드를 끝낸다.
	BYTE EndSkillInitMode();

	/// 스킬 초기화 가능 목록을 전부 지운다.
	void ClearInitPossSkill();
	/// 스킬 초기화 가능 목록을 추가한다.
	void AddInitPossSkill(WORD wSkillID);

protected:
	void UpdateNormalMode();
	void UpdateInitMode();

public:
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
	virtual BYTE OnBeginDrag(LPTDRAG pDRAG, CPoint point);
	virtual BOOL CanWithItemUI();

	virtual HRESULT Render( DWORD dwTickCount);

public:
	CTSkillDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTSkillDlg();
};
