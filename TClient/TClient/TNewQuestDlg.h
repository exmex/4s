#pragma once

class CTNewQuestDlg : public CTClientUIBase
{
public:
	static CPoint m_ptPOS;

	enum 
	{
		TERM_SLOT0 = 0,
		TERM_SLOT1,
		TERM_SLOT2,
		TERM_SLOT_COUNT
	};

	enum
	{
		TERM_QUEST_COMPLETED = 0,
		TERM_QUEST_INCOMPLETED,
		TERM_QUEST_FAIL,
		TERM_QUEST_COUNT
	};

	CTNewQuestDlg( TComponent* pParent, LP_FRAMEDESC pDesc, CTClientChar* pHost);
	virtual ~CTNewQuestDlg(void);

	virtual HRESULT Render( DWORD dwTickCount );
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);
	virtual void OnRButtonUp(UINT nFlags, CPoint pt);
	virtual void ShowComponent(BOOL bVisible = TRUE);
	virtual ITDetailInfoPtr GetTInfoKey(const CPoint& point);
	virtual BOOL GetTChatInfo(const CPoint& point, TCHATINFO& outInfo);
	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);

	// 퀘스트출력
	void Reset();
	void ResetTQUEST( LPTQUEST pTQUEST);
	void SummaryMessage( CString strText);
	void TermMessage(DWORD dwID, CString strTERM, CString strRESULT, INT nLine, INT& nNumber );
	void TextMessage(CString strTitle, CString strText);
	void TextMessage(DWORD dwTitleID, CString strText);

	CString GetResultString(CTClientQuest* pTQUEST);
	CString GetSpeakerString( CString strSpeaker);
	DWORD GetTQUESTColor(LPTQUEST pTQUEST);

	// 외부에서 접근
public:
	CTClientObjBase *m_pHost;
	DWORD m_dwCompleteID;
	DWORD m_dwAcceptID;
	LPTQUEST m_pTQUEST;
	BYTE m_bPrintMSG;
	LPTREWARD m_pTSELREWARD;
	TButton* m_pAccept;
	TButton* m_pRefuse;
	TList* m_pContents;
	TImageList* m_pStatusIcon[5];

	CString m_strNPCTitle;
	CString m_strNPCTalk;
	CString m_strAnswerWhenNPCTalk;

	// 공통
protected:
	LPTREWARD 		m_pTTOPREWARD;

	// 퀘스트출력
protected:
	TList* m_pSummary;
	TList* m_pTerm;
	TList* m_pReward;
	TImageList* m_pITEM[TREWARDITEMCOUNT];
	TImageList* m_pSKILL[TREWARDITEMCOUNT];
	TComponent* m_pSEL[TREWARDITEMCOUNT];
	TComponent* m_pMONEY[3];
	TComponent* m_pEXP;
	TButton *m_pREFUSE;

public:
	TButton *m_pACCEPT;

	// Quest Map
public:
	CTClientCAM *m_pCAM;
	CTClientMAP *m_pTMAP;
	CD3DDevice *m_pDevice;
	LPDIRECT3DSURFACE9 m_pBACKBUF2;
	LPDIRECT3DTEXTURE9 m_pBACKTEX2;

	INT m_nMainUnitX;
	INT m_nMainUnitZ;

	D3DXVECTOR3 m_ptMapIcon;
	D3DXVECTOR2 m_vTCENTER;
	FLOAT m_fTSCALE;
	BYTE m_bHideMap;
	BYTE m_bMOVE;
	BYTE m_bRenderMap;

	TButton* m_pMapOpen;
	TButton* m_pMapClose;

	TComponent* m_pTermICon[TERM_QUEST_COUNT][TERM_SLOT_COUNT];
	TComponent* m_pMapFrame;
	TComponent* m_pMap;
	TComponent* m_pMapQuestIcon;
	TImageList* m_pMapImageList;

public:

	void	OnQuestMapOpen();
	void	OnQuestMapClose();
	void	GetUnitPosition(FLOAT fPosX, FLOAT fPosY, INT& outUnitX, INT& outUnitY);

	void	ResetMap( LPTTERM pTerm );
	void	ResetMap( LPTQUEST pTQUEST);
	void	ResetMap( LPTQUEST pTQUEST, LPTTERM pTerm, DWORD idx );
	void	RenderMap( DWORD dwTickCount );
	void	RederMapPos( DWORD dwTickCount );

	LPTTERM GetSelTerm();
	DWORD  GetSelTermID();
	DWORD	MakeUnitID(WORD wMapID, BYTE nUnitX, BYTE nUnitY);
	CPoint  GetPosition( FLOAT fPosX, FLOAT fPosZ, int nWidth, int nHeight );

	
 
	// 트리
public:
	enum
	{
		COL_ITEM,
		COL_LEVEL,
		COL_STATE,
		COL_CATEGORY,
		COL_COUNT
	};

    void ExpendTree(BYTE bIndex, BYTE bExpend);
	void ResetTree();
	void ResetButton();

	INT GetTop() { return m_pQLIST->GetTop(); }
	INT GetCount() { return m_pQLIST->GetItemCount(); }
	INT GetSel() { return m_pQLIST->GetSel(); }
	void SetCurSel(INT nIdx) { m_pQLIST->SetCurSelItem(nIdx); }
	void RemoveAll() { m_pQLIST->RemoveAll(); }

	INT AddQuest(LPTQUEST pTQUEST, CTClientQuest* pCQUEST=NULL, BOOL bCheckComplete=TRUE);
	INT AddQClass(CTClientQClass* pQCLASS);

	void SetQuestState(INT nIdx, LPTQUEST pTQUEST);
	void CheckShowRight(BYTE bIndex);

	LPTQUEST GetTQUEST(INT nIdx);
	CTClientQClass* GetQCLASS(INT nIdx);
	CTClientQuest* GetCQUEST(INT nIdx);

protected:
	TList *m_pQLIST;
	INT m_nGBTNCOUNT;
	TButton **m_vTCOLLAPSE;
	TButton **m_vTEXPEND;
	TButton **m_vTCHECK;
	CTNewQuestDlg *m_pQuestDlg;
};