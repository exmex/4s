#pragma once

class CTTournamentMode;

class CTGaugePannel : public CTClientUIBase
{
public:
	CTMaintainPannel *m_vTMAINTAIN[TPANNEL_COUNT];
	DWORD m_dwCurQuestID;
	BYTE m_bShowPath;
	BYTE m_bAutoPath;
	TList* m_pQList;
	TComponent *m_pLUCKY;														// 행운번호 표시 (이벤트용)
	TComponent *m_pLUCKY2;

	TComponent *m_pLUCKY3;
	TComponent *m_pLUCKY4;
	TComponent *m_pLUCKY5;
	TComponent *m_pLUCKY6;

	TComponent* m_pCAUTION15;

	CTRSCSDlg *m_pTRSCS;
	CTClientUIBase *m_pTRECALLFRM;
	TImageList *m_pTRECALLSKILLDOWN[TMONSKILL_COUNT];							// Recall skill push image
	TComponent *m_pTRECALLSKILLSM[TMONSKILL_COUNT];								// Recall skill select marker ctrl
	TGauge *m_pTRECALLSKILLGAUGE[TMONSKILL_COUNT];								// Recall skill gauge ctrl
	TImageList *m_pTRECALLAIDOWN[TRECALLAI_COUNT];								// Recall AI push image
	TComponent *m_pTRECALLAISM[TRECALLAI_COUNT];								// Recall AI select marker ctrl
	TComponent *m_pTRECALLAI[TRECALLAI_COUNT];									// Recall AI select ctrl
	TImageList *m_pTRECALLSKILL[TMONSKILL_COUNT];								// Recall skill ctrl
	CTClientUIBase *m_pTHOTKEYFRM[THOTKEYBASE_COUNT];
	TImageList *m_pTHOTKEY[THOTKEYBASE_COUNT][MAX_HOTKEY_POS][2];				// Hot key ctrl
	TImageList *m_pTHOTKEYDOWN[THOTKEYBASE_COUNT][MAX_HOTKEY_POS];				// Hot key push image
	TGauge *m_pTHOTKEYGAUGE[THOTKEYBASE_COUNT][MAX_HOTKEY_POS];					// Hot key gauge ctrl
	TComponent *m_pTHOTKEYSM[THOTKEYBASE_COUNT][MAX_HOTKEY_POS];				// Hot key select marker ctrl
	TComponent *m_pTHOTKEYCTRL[THOTKEYBASE_COUNT][THOTKEYCTRL_COUNT];			// Hotkey base ctrl
	THOTKEY m_vTHOTKEY[MAX_HOTKEY_INVEN][MAX_HOTKEY_POS];						// Hot key pool
	BYTE m_bHotkeyID[THOTKEYBASE_COUNT];
	BYTE m_bHotkeyBase;
	CPoint m_ptPrev;
	CTClientRecall* m_pMainRecall;
	CTClientChar	* m_pHost;
	DWORD m_dwCaution15State;
	DWORD m_dwCaution15Tick;
	BYTE m_bAddedCaution15;
	CTTournamentMode* m_pTournamentInfo;
	TComponent* m_pSiegeAlarm;
	TButton* m_pExitTutorial;
	BYTE m_bAddedSiegeAlarm;
	BYTE m_bAddedExitTutorial;
	BOOL m_bNotifySiegeAlarm;
	DWORD m_dwNotifySiegeAlarmTick;
	TComponent* m_pTASSISTANT[3];

public:
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point);
	virtual BOOL HitTest( CPoint pt);
	virtual BOOL CanWithItemUI();
	virtual void DefaultPosition( CPoint* vBASIS, BOOL bRestore );
	virtual void ResetPosition();
	virtual TDROPINFO OnDrop( CPoint point);
	virtual BYTE OnBeginDrag(
		LPTDRAG pDRAG,
		CPoint point);
	void OnLButtonDown( UINT nFlags, CPoint pt);
	void OnLButtonUp( UINT nFlags, CPoint pt);
	void OnMouseMove( UINT nFlags, CPoint pt);
	HRESULT Render( DWORD dwTickCount );
	void ShowComponent(BOOL bVisible = 1);

public:
	void UpdateAllQuestINFO( CTClientChar *pTCHAR);
	void UpdateQuestINFO( CTClientQuest *pTQUEST);
	void UpdateQuestBUTTON();
	CString GetResultString( CTClientTerm *pTERM);
	void SetQuestState( LPTQUEST pTQUEST);
	void CalcRecallTick( CTClientRecall *pTRECALL);
	void ResetRecallUI( CTClientRecall *pTRECALL);
	void ResetHotkeyStr();
	void CalcHotkeyTick();
	void ShowHotkeyBase( BYTE bShow);
	BYTE CheckHotkeyBase( CPoint point, THOTKEY_BASE* outType=NULL );
	void ResetHotkeyUI();
	void ReleaseHotkey();
	ITDetailInfoPtr GetTInfoKey_Hotkey(const CPoint& point, INT nHotkeyID, INT nSlotID);
	ITDetailInfoPtr GetTInfoKey_RecallSkill(const CPoint& point, INT nID);
	ITDetailInfoPtr GetTInfoKey_RecallAi(const CPoint& point, INT nID);
	void PushIcon( CPoint point);
	void ReleaseIcon();
	void HotkeyNext(THOTKEY_BASE eHOTKEY);
	void HotkeyPrev(THOTKEY_BASE eHOTKEY);
	BOOL IsEnableHotkeySkill(CTClientSkill * pTSKILL);
	BOOL IsEnableHotkeySkill(BYTE bRESULT);
	LPTHOTKEY GetHotKey(THOTKEY_BASE eTYPE, BYTE bHotkeyID);
	void CalcCaution15Tick(DWORD dwTick);
	void AttachCaution15();
	void DettachCaution15();
	void AttachSiegeAlarm();
	void DettachSiegeAlarm();
	void SetNotifySiegeAlarm(BOOL bNotify);
	void AttachExitTutorial();
	void DettachExitTutorial();

public:
	CTGaugePannel( TComponent *pParent, LP_FRAMEDESC pDesc, TCMLParser *pParser, CTClientGame *pMainGame, CTClientChar *pHost);
	virtual ~CTGaugePannel();
};
