#pragma once

struct tagTENEMY
{
	MAPDWORD m_mapTREPORTER;

	DWORD m_dwTick;
	DWORD m_dwID;
	BYTE m_bType;

	FLOAT m_fSPEED;

	WORD m_wMapID;
	WORD m_wPosX;
	WORD m_wPosZ;
	WORD m_wDIR;

	tagTENEMY()
	{
		m_mapTREPORTER.clear();
		m_bType = OT_NONE;
		m_dwID = 0;

		m_fSPEED = 0.0f;
		m_dwTick = 0;
		m_wMapID = 0;
		m_wPosX = 0;
		m_wPosZ = 0;
		m_wDIR = MOVE_NONE;
	};

	~tagTENEMY()
	{
		m_mapTREPORTER.clear();
		m_bType = OT_NONE;
		m_dwID = 0;

		m_fSPEED = 0.0f;
		m_wMapID = 0;
		m_wPosX = 0;
		m_wPosZ = 0;
		m_wDIR = MOVE_NONE;
	};
};

struct tagTUNIT
{
	CString m_strNAME;
	DWORD m_dwCharID;
	FLOAT m_fSPEED;

	DWORD m_dwTargetID;
	DWORD m_dwBspID;
	DWORD m_dwMaxHP;
	DWORD m_dwHP;
	DWORD m_dwTick;

	WORD m_wTargetX;
	WORD m_wTargetZ;

	WORD m_wMapID;
	WORD m_wPosX;
	WORD m_wPosZ;
	WORD m_wDIR;

	BYTE m_bTargetType;
	BYTE m_bLevel;
	BYTE m_bClass;
	BYTE m_bTCMD;

	BYTE m_bRaceID;
	BYTE m_bSexID;
	BYTE m_bFaceID;
	BYTE m_bHairID;

	tagTUNIT()
	{
		m_strNAME.Empty();
		m_dwCharID = 0;
		m_fSPEED = 0.0f;
		m_dwTick = 0;

		m_dwTargetID = 0;
		m_dwMaxHP = 0;
		m_dwHP = 0;

		m_wTargetX = 0;
		m_wTargetZ = 0;

		m_dwBspID = NODEID_NULL;
		m_wMapID = 0;
		m_wPosX = 0;
		m_wPosZ = 0;
		m_wDIR = MOVE_NONE;

		m_bRaceID = 0;
		m_bSexID = 0;
		m_bFaceID = 0;
		m_bHairID = 0;

		m_bTargetType = OT_NONE;
		m_bLevel = 0;
		m_bClass = 0;
		m_bTCMD = TRSCSCMD_NONE;
	};
};

struct tagTSQUAD
{
	CTClientObjBase m_vTMARK;
	CTClientArrow m_vTARROW;
	VTUNIT m_vTUNIT;

	DWORD m_dwTargetID;
	DWORD m_dwChiefID;

	BYTE m_bTargetType;
	BYTE m_bTCMD;

	WORD m_wTargetMapID;
	WORD m_wTargetX;
	WORD m_wTargetZ;
	WORD m_wPartyID;

	tagTSQUAD( CD3DDevice *pDevice,
			   CTachyonRes *pRES,
			   LPTEXTURESET pTARROW)
	{
		m_vTMARK.ReleaseData();
		m_vTMARK.InitOBJ(pRES->GetOBJ(TCMDTARGET_OBJ));
		m_vTMARK.SetCloth(
			pDevice,
			TCMDTARGET_CLK,
			TCMDTARGET_CL,
			TCMDTARGET_CMDMESH);

		m_vTMARK.SetAction(
			TCMDTARGET_ACT,
			TCMDTARGET_ANI);

		m_vTMARK.InitSIZE(
			TCMDTARGET_SIZE_X,
			TCMDTARGET_SIZE_Y,
			TCMDTARGET_SIZE_Z,
			0.0f, 0.0f);
		m_vTMARK.m_bDynamicOBJ = TRUE;
		m_vTMARK.m_bLand = TRUE;

		m_vTMARK.m_fRange = TRSCSOBJ_RANGE;
		m_vTMARK.ResetOBJPart(pDevice);
		m_vTMARK.m_bContryID = TCONTRY_N;
		m_vTMARK.m_bAlpha = 0;
		m_vTMARK.m_bHide = TRUE;

		m_vTARROW.m_fVelocityX = TCMDARROW_VELOCITY;
		m_vTARROW.m_fGravity = TCMDARROW_GRAVITY;

		m_vTARROW.m_bHostType = OT_NONE;
		m_vTARROW.m_dwHostID = 0;

		m_vTARROW.m_vTSLASH.InitSFX(
			&m_vTARROW.m_vPosition,
			pTARROW,
			TSQUADARROW_COLOR,
			TCMDARROW_WIDTH,
			TSLASH_INTER);
		m_vTARROW.m_vTSLASH.m_vLENGTH._41 = -TCMDARROW_WIDTH / 2.0f;

		m_vTUNIT.clear();
		m_dwTargetID = 0;
		m_dwChiefID = 0;

		m_wTargetMapID = 0xFFFF;
		m_wTargetX = 0;
		m_wTargetZ = 0;
		m_wPartyID = 0;

		m_bTargetType = OT_NONE;
		m_bTCMD = TRSCSCMD_NONE;
	};

	~tagTSQUAD()
	{
		while(!m_vTUNIT.empty())
		{
			delete m_vTUNIT.back();
			m_vTUNIT.pop_back();
		}

		m_dwTargetID = 0;
		m_dwChiefID = 0;

		m_wTargetMapID = 0xFFFF;
		m_wTargetX = 0;
		m_wTargetZ = 0;
		m_wPartyID = 0;

		m_bTargetType = OT_NONE;
		m_bTCMD = TRSCSCMD_NONE;
	};
};

struct tagTCORPS
{
	VTSQUAD m_vTSQUAD;
	WORD m_wID;

	MAPTENEMY m_mapFIXRECALL;
	MAPTENEMY m_mapRECALL;
	MAPTENEMY m_mapMON;
	MAPTENEMY m_mapPC;

	tagTCORPS()
	{
		m_vTSQUAD.clear();
		m_wID = 0;

		m_mapFIXRECALL.clear();
		m_mapRECALL.clear();
		m_mapMON.clear();
		m_mapPC.clear();
	};

	~tagTCORPS()
	{
		MAPTENEMY::iterator it;

		for( it = m_mapFIXRECALL.begin(); it != m_mapFIXRECALL.end(); it++)
			delete (*it).second;

		for( it = m_mapRECALL.begin(); it != m_mapRECALL.end(); it++)
			delete (*it).second;

		for( it = m_mapMON.begin(); it != m_mapMON.end(); it++)
			delete (*it).second;

		for( it = m_mapPC.begin(); it != m_mapPC.end(); it++)
			delete (*it).second;

		while(!m_vTSQUAD.empty())
		{
			delete m_vTSQUAD.back();
			m_vTSQUAD.pop_back();
		}
		m_wID = 0;

		m_mapFIXRECALL.clear();
		m_mapRECALL.clear();
		m_mapMON.clear();
		m_mapPC.clear();
	};
};


class CTRSCSDlg : public CTClientUIBase
{
public:
	static LPDIRECT3DTEXTURE9 m_pTBACK;
	static LPDIRECT3DTEXTURE9 m_pTRSCS;
	static LPTEXTURESET m_pTARROW;
	static LPTEXTURESET m_pTMASK;

public:
	TComponent *m_vTCOMP[TRSCSCOMP_COUNT];
	TComponent *m_pTSQUADNAME[TRSCSHOTKEY_COUNT];
	TComponent *m_pTSQUADSTAT[TRSCSHOTKEY_COUNT];
	TComponent *m_pTUNITNAME[TRSCSHOTKEY_COUNT];
	TGauge *m_pTUNITGAUGE[TRSCSHOTKEY_COUNT];

	TImageList *m_pTDRAG;
	TList *m_pTSQUAD;
	TList *m_pTUNIT;
	CRect m_vTAREA;
	TComponent *m_pTITLE;
	TComponent *m_pTBACKAREA;

	TScroll *m_pTZOOMSCROLL;

	BOOL m_bMinimize;
	BOOL m_bHideSquad;

	BOOL m_bDropping;

public:
	D3DXVECTOR2 m_vTCENTER;
	D3DXVECTOR2 m_vTALARM;
	TCORPS m_vTCORPS;

	DWORD m_dwTALARM;
	FLOAT m_fTSCALE;

	DWORD m_dwBspID;
	WORD m_wMapID;
	BYTE m_bMOVE;

	LPTSQUAD m_pTCURSQUAD;
	LPTUNIT m_pTCURUNIT;

	CTClientChar *m_pHost;
	CTClientMAP *m_pTMAP;
	CTClientCAM *m_pCAM;
	CD3DDevice *m_pDevice;

public:
	void ResetCORPS();
	void ResetUNIT();
	void RenderTRSCS();

	void CheckCURSEL( BYTE bRESET);
	void OnSelChange();
	void MoveCursor( CPoint point);

	void CalcTick( DWORD dwTick);
	void CalcEnemyTick(
		LPMAPTENEMY pTENEMY,
		DWORD dwTick);

	void DeleteReporter( DWORD dwReporterID);
	void DeleteReporter(
		LPMAPTENEMY pTENEMY,
		DWORD dwReporterID);

	LPTSQUAD FindTSQUAD( WORD wSquadID);
	LPTENEMY FindTENEMY(
		DWORD dwID,
		BYTE bType);

	LPTUNIT FindTUNIT(
		LPTSQUAD pTSQUAD,
		DWORD dwCharID);

	D3DXVECTOR2 GetEnemyPOS( LPTENEMY pTENEMY);
	D3DXVECTOR2 GetUnitPOS( LPTUNIT pTUNIT);

	void ResetVisible();
	void AddEnemy(
		DWORD dwReporterID,
		DWORD dwEnemyID,
		BYTE bEnemyType,
		FLOAT fSpeed,
		WORD wMapID,
		WORD wPosX,
		WORD wPosZ,
		WORD wDIR );

public:
	virtual BOOL CanWithItemUI();
	virtual void OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnLButtonDown( UINT nFlags, CPoint pt);
	virtual void OnRButtonDown( UINT nFlags, CPoint pt);
	virtual void OnRButtonUp( UINT nFlags, CPoint pt);
	virtual void OnMouseMove( UINT nFlags, CPoint pt);
	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);

	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual HRESULT Render( DWORD dwTickCount);

	virtual TDROPINFO OnDrop( CPoint point);
	virtual BYTE OnBeginDrag(
		LPTDRAG pDRAG,
		CPoint point);

	virtual void SwitchFocus(TComponent* pCandidate);

public:
	CTRSCSDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTRSCSDlg();
};
