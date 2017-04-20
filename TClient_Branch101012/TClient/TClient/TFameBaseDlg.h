#pragma once

class CTFameRank;
class CTFameListDlg;

class CTFameBaseDlg :	public CTClientUIBase
{
public:
	enum { MAX_KNIGHT_SLOT_COUNT = 9 };
	enum 
	{
		TFRAMEBASE_HERO = 0,
		TFRAMEBASE_GOD,
		TFRAMEBASE_GODDESS,
		TFRAMEBASE_COUNT
	};

public:
	CTFameBaseDlg( TComponent *pParent, LP_FRAMEDESC pDesc, TCMLParser *pParser );
	virtual ~CTFameBaseDlg();
	void ShowComponent(BOOL bVisible);
	void ClearRank();
	void ResetRankInfo();
	void ResetTab();
	void OnEnable();
	void OnNotify(DWORD from, WORD msg, LPVOID param);
	void OnSelectRank(CTFameRank* pRank, DWORD dwMonthRank, BYTE bType );

public:
	WORD m_wYear;
	BYTE m_bMonth;
	TMAPFAMERANK m_mapFAMERANK;
	CTFameRank* m_pWarLord;
	BYTE m_bType;
	CTFameListDlg* m_pFAME_FIRST_LIST;	

protected:
	TComponent* m_pDATE;
	TComponent* m_pWARLORD_NAME;
	TImageList* m_pWARLORD_FACE;
	TImageList* m_pWARLORD_CLASS;
	TImageList* m_pWARLORD_COUNTRY;
	TComponent* m_pWARLORD_SAY;
	TComponent* m_pWARLORD_RANK;
	TComponent* m_pWARLORD_FAME;
	TComponent* m_pWARLORD_BATTLE;
	TComponent* m_pWARLORD_LEVEL;
	TComponent* m_pWARLORD_TITLE;
	TList* m_pLIST;
};
