#pragma once

#define TSCHEDULE_COLOR_COUNT 4

class CTTournament : public CTClientUIBase
{
public:
	static bool CTTournament::IsCommonSchedule(BYTE bStep);

#ifdef MODIFY_TT
public:
	static DWORD SCHEDULE_COLOR[ TSCHEDULE_COLOR_COUNT ];
	static DWORD SCHEDULE_COLOR_DISABLE[ TSCHEDULE_COLOR_COUNT ];

public:
	class ScheduleGroup
	{
	public:
		BYTE m_bGroupID;
		BYTE m_bCurrentStep;
		LPTTOURNAMENTSCHEDULE m_pCurrentStep;
		INT m_nScheduleTop;
		VTTOURNAMENTSCHEDULE m_vSchedule;
		void AddSchedule( LPTTOURNAMENTSCHEDULE );
		void AddScheduleFrom( ScheduleGroup* );
		void ClearSchedule();
		void SortByStep();
		~ScheduleGroup()
		{
			VTTOURNAMENTSCHEDULE::iterator it, end;
			it = m_vSchedule.begin();
			end = m_vSchedule.end();
			for(; it != end ; ++it )
				delete (*it);
			m_vSchedule.clear();
		}

		void SetLatestStepInCommon()
		{
			VTTOURNAMENTSCHEDULE::iterator it, end;
			it = m_vSchedule.begin();
			end = m_vSchedule.end();
			for(; it != end ; ++it )
			{
				if( (*it) && CTTournament::IsCommonSchedule((*it)->m_bStep) )
					m_bCurrentStep = (*it)->m_bStep;					
			}
		}

		LPTTOURNAMENTSCHEDULE GetTNMTSTEP_ENTER()
		{
			VTTOURNAMENTSCHEDULE::iterator it, end;
			it = m_vSchedule.begin();
			end = m_vSchedule.end();
			for(; it != end ; ++it )
			{
				if( (*it) && (*it)->m_bStep == TNMTSTEP_ENTER )
					return (*it);
			}
			return NULL;
		}
	};

	typedef std::map<DWORD, ScheduleGroup*> MAPSCHEDULEGROUP;
#endif

public:
	CTTournament( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTTournament();
	virtual void ShowComponent( BOOL bVisible );
	void ReleaseData();
	void ReleaseSchedule();
	void UpdateMatch( TTMODE mode );
	void UpdateSchedule();
	void CalcScheduleTop();
	void OnSelectMatch( TMATCH* pMATCH );
	void RequestData();
	virtual void OnNotify( DWORD from, WORD msg, LPVOID param );
	virtual HRESULT Render( DWORD dwTickCount );
	int OnGM_TOURNAMENT_OK();
	int OnGM_TOURNAMENT_CANCEL();
	LPTMATCHPLAYER FindPlayer( DWORD dwCharID );
	void ResetParty();
	ITDetailInfoPtr GetTInfoKey(const CPoint& point);
	ITDetailInfoPtr GetTInfoKey_Player(LPTMATCHPLAYER pPlayer, BYTE bResult, const CRect& rc);

public:
	TTMODE m_nMode;
	VTMATCH m_vMatch;
	LPTMATCHREWARD m_pTTOPREWARD;

#ifdef MODIFY_TT
	ScheduleGroup* FindScheduleGroup( BYTE bGroup );
	void UpdateToSchedule(TMATCH* pMATCH);
	ScheduleGroup* m_pCurrentGroup;
	MAPSCHEDULEGROUP m_mapGroup;
	BYTE m_bCurrentGroup;
#else

	BYTE m_bCurrentStep;
	VTTOURNAMENTSCHEDULE m_vSchedule;
	INT m_nScheduleTop;
	LPTTOURNAMENTSCHEDULE m_pCurrentStep;
#endif

	INT m_nPrevSelectedMatch;
	
public:
	TComponent* m_pTITLE;
	TComponent* m_pCLOSE;
	TComponent* m_pMATCHLIST_COL_NAME;
	TComponent* m_pMATCHLIST_COL_TYPE;
	TComponent* m_pMATCHLIST_COL_TARGET;
	TComponent* m_pMATCHLIST_COL_PROGRESS;
	TList* m_pMATCHLIST;
	TComponent* m_pTXT_REWARD;
	TList* m_pREWARD;
	TComponent* m_pSLOT2;
	TComponent* m_pMONEY_BASE;
	TComponent* m_pREWARD_RUNE;
	TComponent* m_pREWARD_LUNA;
	TComponent* m_pREWARD_CRON;
	TComponent* m_pBTN_OK;
	TComponent* m_pBTN_CANCEL;
	TImageList* m_pITEM[2];
	TComponent* m_pENTRY_NOTICE;
	TComponent* m_pENTRY_COL_NAME;
	TComponent* m_pENTRY_COL_COUNTRY;
	TComponent* m_pENTRY_COL_LEVEL;
	TComponent* m_pENTRY_COL_JOB;
	TComponent* m_pENTRY_COL_ACCRANK;
	TComponent* m_pENTRY_COL_MONRANK;
	TList* m_pENTRY_PLAYER_LIST;
	TComponent* m_pENTRY_CHARGE1;
	TComponent* m_pENTRY_CHARGE2;
	TComponent* m_pENTRY_CHARGE3;
	TComponent* m_pENTRY_CHARGE4;
	TComponent* m_pENTRY_CHARGE1_MONEYBASE;
	TComponent* m_pENTRY_CHARGE2_MONEYBASE;
	TComponent* m_pENTRY_CHARGE3_VALUE;
	TComponent* m_pENTRY_CHARGE4_VALUE;
	TComponent* m_pENTRY_CHARGE1_RUNE;
	TComponent* m_pENTRY_CHARGE1_LUNA;
	TComponent* m_pENTRY_CHARGE1_CRON;
	TComponent* m_pENTRY_CHARGE2_RUNE;
	TComponent* m_pENTRY_CHARGE2_LUNA;
	TComponent* m_pENTRY_CHARGE2_CRON;
	TList* m_pENTRY_PARTY_LIST;
	TComponent* m_pMATCH_BACK;
	TComponent* m_pMATCH_COL_ENTRY;
	TComponent* m_pMATCH_COL_QF;
	TComponent* m_pMATCH_COL_SF;
	TComponent* m_pMATCH_COL_FINAL;
	TComponent* m_pMATCH_ENTRY[8];
	LPTMATCHPLAYER m_pMATCH_ENTRY_INFO[8];
	TComponent* m_pMATCH_QF[4];
	LPTMATCHPLAYER m_pMATCH_QF_INFO[4];
	TComponent* m_pMATCH_SF[2];
	LPTMATCHPLAYER m_pMATCH_SF_INFO[2];
	TComponent* m_pMATCH_FINAL;
	LPTMATCHPLAYER m_pMATCH_FINAL_INFO;
	TComponent* m_pMATCH_QF_UP[4];
	TComponent* m_pMATCH_QF_UP_HL[4];
	BOOL m_bMATCH_QF_UP[4];
	TComponent* m_pMATCH_QF_DOWN[4];
	TComponent* m_pMATCH_QF_DOWN_HL[4];
	BOOL m_bMATCH_QF_DOWN[4];
	TComponent* m_pMATCH_SF_UP[2];
	TComponent* m_pMATCH_SF_UP_HL[2];
	BOOL m_bMATCH_SF_UP[2];
	TComponent* m_pMATCH_SF_DOWN[2];
	TComponent* m_pMATCH_SF_DOWN_HL[2];
	BOOL m_bMATCH_SF_DOWN[2];
	TComponent* m_pMATCH_FINAL_UP;
	TComponent* m_pMATCH_FINAL_UP_HL;
	BOOL m_bMATCH_FINAL_UP;
	TComponent* m_pMATCH_FINAL_DOWN;
	TComponent* m_pMATCH_FINAL_DOWN_HL;
	BOOL m_bMATCH_FINAL_DOWN;
	TComponent* m_pMATCH_FINAL_HL;
	BOOL m_bMATCH_FINAL;
	TComponent* m_pSTEP_DATE[6];
	TComponent* m_pSTEP_TITLE[6];
	TImageList* m_pSTEP_BACK[6];
	TImageList* m_pSTEP_STICK[6];
	BOOL m_bSTEP[6];
	BOOL m_bShowPartyBtn;
	BOOL m_bShowJoinBtn;
	TButton* m_pBTN_ADD_PARTY;
	TButton* m_pBTN_REMOVE_PARTY[6];	
};