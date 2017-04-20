#pragma once


class CTClientObjBase : public CTachyonObject
{
public:
	static CT3DVertex m_vTSQUARE;
	static CFont *m_pFONT;
	static FLOAT m_fCamDist;

	static LPTEXTURESET m_pTSELECT;
	static LPTEXTURESET m_pTSHADOW;

	static MAPTSELECTOBJ m_mapTSELECTDEADOBJ;
	static MAPTSELECTOBJ m_mapTSELECTOBJ;

	static MAPTOBJTEXCOLORPAIR m_mapOBJTEXCOLOR;
	static VTOBJBASE m_vTGARBAGE;

	static D3DXVECTOR3 m_vADJACENCY[ADJACENCY_COUNT];
	static FLOAT m_fABOFFSET;

public:
	static void BeginDrawSquare( CD3DDevice *pDevice);

	static void InitSquareResource( CTachyonRes *pRES);
	static void ReleaseSquareResource();

	static void DeleteOBJ( CTClientObjBase *pTOBJ);
	static void ClearGARBAGE();
	static FLOAT GetCullDIST();

	static LPOBJTEX NewGradeObjTex(
		LPOBJTEX pTEX,
		INT nType, // only TT_COLOR or TT_TEX
		DWORD dwCOLOR,
		LPTEXTURESET pSkinTEX,
		BYTE bPSTYPE);

	static void ReleaseGradeObjTex();
	static void InitADJACENCY();

public:
	MAPSFXINST m_mapTSUBACTSFXINST;
	MAPSFXINST m_mapTSUBANISFXINST;

	MAPSNDINST m_mapTSUBACTSNDINST;
	MAPSNDINST m_mapTSUBANISNDINST;

	VECTOROBJSFX m_vTSUBACTSFX;
	VECTOROBJSFX m_vTSUBANISFX;

	VECTORTVERTEXINDEX m_vHCOLLISION;
	VECTORTVERTEXINDEX m_vVCOLLISION;

	MAPDWORD m_mapTSUBACTSFX;
	MAPDWORD m_mapTSUBANISFX;

	MAPOBJSND m_mapTSUBACTSND;
	MAPOBJSND m_mapTSUBANISND;
	ACTINST m_vTSUBACT;

	FLOAT m_fSubActTime;
	DWORD m_dwTSUBACT;
	int m_nSubKeyBase;

	VECTORDWORD m_vEquipSFXFuncID;
	TMAPWEAPONEQUIPINFO m_mapEQUIPInfo;

	// Monster Adjacency
	BYTE m_bAdjacencyMon[ ADJACENCY_COUNT ];
	INT m_nHostAdjacency;

	VTSKILLTARGET m_vTSKILLTARGET;
	VECTOROBJSFX m_vTGARBAGESFX;
	VECTOROBJSFX m_vTACTIONSFX;
	VECTORDWORD m_vTDAMAGE;
	MAPTOBJSFXS m_mapITEMOBJSFX;

	MAPTQUESTTEMP m_mapTQUESTTEMP;
	MAPTQCLASS m_mapTQCLASS;
	MAPDWORD m_mapTQUESTCOND;

	MAPTSKILL m_mapTSKILL;
	MAPTINVEN m_mapTINVEN;

	DWORD m_dwHouseID;
	DWORD m_dwInstID;
	DWORD m_dwID;

	DWORD m_dwMaxHP;
	DWORD m_dwMaxMP;

	DWORD m_dwAttackDelayRate[TAD_COUNT];
	DWORD m_dwAttackDelay[TAD_COUNT];

	DWORD m_dwDropDamage;
	DWORD m_dwHP;
	DWORD m_dwMP;

	DWORD m_dwFlashColor;
	BYTE m_bFlash;

	BYTE m_bCamCollision;
	BYTE m_bCanSelected;
	BYTE m_bActionLock;
	BYTE m_bDynamicOBJ;
	BYTE m_bCanDefend;
	BYTE m_bParashute;
	BYTE m_bDrawGhost;
	BYTE m_bDrawMark;
	BYTE m_bInSHADOW;
	BYTE m_bDrawTalk;
	BYTE m_bDrawName;
	BYTE m_bACTLevel;
	BYTE m_bPrevFLY;
	BYTE m_bMermaid;
	BYTE m_bAction;
	BYTE m_bFPView;
	BYTE m_bDetect;
	BYTE m_bGhost;
	BYTE m_bLevel;
	BYTE m_bShoot;
	BYTE m_bSlide;
	BYTE m_bDown;
	BYTE m_bLand;
	BYTE m_bSwim;
	BYTE m_bMode;
	BYTE m_bType;
	BYTE m_bHide;
	BYTE m_bClarity;
	BYTE m_bStandHide;

	BYTE m_bTakenItemAll;

	/// T_INVALID : 변장아님, TCONTRY_TYPE : 해당국가로 변장 중
	BYTE m_bDisguise;	

	BYTE m_bIsHolded;
	BYTE m_bIsStuned;
	BYTE m_bLastACT;

	BYTE m_bChargeSpeed;
	BYTE m_bCLEARTARGET;
	BYTE m_bSTEPSND;
	BYTE m_bGARBAGE;
	BYTE m_bTCORPSMOVE;

	BYTE m_bHouseMesh;
	BYTE m_bForceDraw;

	BYTE m_bHideOnCapeMode;
	BYTE m_bEquipMode;
	BYTE m_bInBattle;

	BYTE m_bAllianceID;
	BYTE m_bContryID;
	BYTE m_bColorID;

	BYTE m_bHelmetHide;
	BYTE m_bSelectable;

	TSKILLDATA m_vTSKILLDATA;
	TSKILLDATA m_vTBACKSKILLDATA;

	D3DXVECTOR2 m_vSlideDIR;
	D3DXVECTOR2 m_vPushDIR;
	D3DXVECTOR2 m_vJumpDIR;
	D3DXVECTOR2 m_vFallDIR;

	DWORD m_dwSlideTick;
	DWORD m_dwPushTick;
	DWORD m_dwJumpTick;
	DWORD m_dwFallTick;

	DWORD m_dwSHADOWTick;
	DWORD m_dwAlphaTick;
	DWORD m_dwTalkTick;

	FLOAT m_fPushHeight;
	FLOAT m_fFallHeight;
	FLOAT m_fJumpHeight;
	FLOAT m_fMoveSpeed;
	FLOAT m_fPushSpeed;
	FLOAT m_fJumpSpeed;

	BYTE m_bLastDraw;

	CTachyonSlashSFX m_vTSLASH[TSLASHSFX_COUNT];
	CTachyonObject m_vTMARKER;
	CTDynamicBillboard *m_pTDBB;

	LPTREGIONINFO m_pTREGION;
	LPTMONTEMP m_pTRANS;
	D3DXVECTOR3 m_vTDEAD;

	CTClientTalkBox m_vTALK;
	D3DXVECTOR2 m_vNAMEPOS;
	D3DXVECTOR2 m_vUSERTITLEPOS;
	CD3DFont m_vNAME;
	CD3DFont m_vUSERTITLE;

	BYTE m_bCollisionType;
	BYTE m_bDETAILLevel;
	BYTE m_bVisible;
	BYTE m_bNPCType;
	BYTE m_bClassID;
	BYTE m_bHasQuest;
	BYTE m_bSubActEnd;
	BYTE m_bSkillSilence;

	WORD m_wPursuit;
	WORD m_wTDETECT;
	WORD m_wMoveDIR;
	WORD m_wPITCH;
	WORD m_wDIR;

	FLOAT m_fBreathHeight;
	FLOAT m_fSquareHeight;
	FLOAT m_fSpeedFactor;
	FLOAT m_fRangeFactor;
	FLOAT m_fCamDIST;
	FLOAT m_fRadiusH;
	FLOAT m_fTDEFAULTSPEED;
	FLOAT m_fRadius;
	FLOAT m_fZValue;
	FLOAT m_fSight;
	FLOAT m_fRange;
	FLOAT m_fSizeX;
	FLOAT m_fSizeY;
	FLOAT m_fSizeZ;
	FLOAT m_fDepth;
	FLOAT m_fPickWeight;

	LISTTMAINTAIN m_vTPRotation;
	LISTTMAINTAIN m_vTNRotation;

	BYTE m_bTournamentTeam;
	BYTE m_bDoubleHead;

public:
	MAPTMAINTAIN m_mapTMAINTAIN;

	MAPDWORD m_mapSendedRemoveTMAINTAIN;
	MAPDWORD m_mapSendedAddTMAINTAIN;

	DWORD m_vDamageTick[TNUMBERPOS_COUNT];
	DWORD m_vWeaponPOS[TWEAPON_COUNT];
	DWORD m_dwActionID;
	DWORD m_dwAniID;
	BYTE m_bRootID;

public:
	CTClientItem *FindSkillWeapon( LPTSKILL pTSKILL);
	CTClientQClass *FindTQClass( DWORD dwClassID);
	CTClientQuest *FindTQuest( DWORD dwQuestID);
	CTClientSkill *FindTSkill( WORD wSkillID);
	CTClientInven *FindTInven( BYTE bInvenID);

	void ClearSkillTarget();
	void ClearTQClass();
	void ClearSkill();
	void ClearInven();
	void ClearMaintainOnDie();

	// 버프 관련 메소드
	void ClearMaintain();
	void InsertMaintain( WORD wID, CTClientMaintain *pTMAINTAIN);
	void EraseMaintain( WORD wSkillID);
	void EraseSendedAddMaintain( WORD wSkillID);
	void EraseSendedRemoveMaintain( WORD wSkillID);
	CTClientMaintain* FindMaintain( WORD wID);
	void CalcMaintain( DWORD dwTick, BYTE bHandling, CTClientGame *pTGAME);
	void CheckMaintainOBJ( CTClientGame *pTGAME, BOOL bBroadCast = TRUE);
	void BuildMaintainList( CTClientGame *pTGAME);
	void AdjustSDT_STATUS_MOVESPEED();
	LPMAPTMAINTAIN GetTMAINTAIN() { return &m_mapTMAINTAIN; }

	void DoSkillFunc(
		LPTSKILL pTSKILL,
		WORD wPursuit,
		BYTE bLevel,
		LPD3DXVECTOR3 pAtkPos,
		LPD3DXVECTOR3 pDefPos,
		BOOL bApplyMaintain,
		CTClientGame *pTGAME);
	void DoSkillFuncAll( CTClientGame *pTGAME);
	bool VerifySkillFunc();

	virtual void ShowSFX();
	virtual void HideSFX();

	void SetHide(BOOL bHide);
	void CancelHiding( CTClientGame* pTGAME );
	void CancelMaintainByFunction( CTClientGame* pTGAME, DWORD* dwFunction, DWORD dwCount );

	void RemoveTActionSFX( LPVECTOROBJSFX pTACTIONSFX);
	void ClearTActionSFX( LPVECTOROBJSFX pTACTIONSFX);
	void PlayTActionSFX(
		LPVECTOROBJSFX pTACTIONSFX,
		DWORD dwFuncID,
		BOOL bShow = TRUE);

	void PlayTSLASH( DWORD dwTSLASH);
	void StopTSLASH();
	void ClearTARROW();

	void InsertTItemMagicSFX(
		LPOBJSFX pOBJSFX,
		CTClientSFX *pTSFX,
		CTachyonObject *pTOBJ);

	void PlayTMagicItemSFX(
		CTachyonObject *pTOBJ,
		DWORD dwFuncID);

	void RemoveTMagicItemSFX( CTachyonObject *pTOBJ);
	void RemoveAllTMagicItemSFX();

	void RemoveAllEquipSFX();
	void RemoveAllWeaponSFX();
	void SetAlphaOfAllEquipSFX( BYTE bAlpha);
	void SetAlphaOfAllWeaponSFX( BYTE bAlpha);
	void CalcTMagicItemSFX( CD3DDevice *pDevice);

	void ApplyGradeEffect(
		CTachyonObject *pItemObj,
		CTClientItem *pTITEM);

	void ApplyGradeEffect(
		CTClientItem** pApplyGradeList,
		INT nCount);

	void InsertTQuest( CTClientQuest *pTQuest);
	void EraseTQuest( DWORD dwQuestID);
	void ResetRootID( DWORD dwRootID);

	void PlayRandomSND(
		LPMAPSNDINST pINST,
		LPMAPOBJSND pSND,
		DWORD dwFuncID,
		BYTE b3DSound);

	void PlaySND(
		DWORD dwSndID,
		BYTE bType,
		BYTE b3DSound);

	void PushCollisionPoly(
		LPD3DXVECTOR3 pPOLY,
		WORD wMESH,
		WORD wINDEX,
		WORD wPOLY);

	void InitCollision();
	void InitSIZE(
		FLOAT fSizeX,
		FLOAT fSizeY,
		FLOAT fSizeZ,
		FLOAT fDepth,
		FLOAT fSight);

	LPTACTION FindEquipTACTION( 
		BYTE bActionID,
		BYTE bWeaponID);

	TACTION FindActionID(
		BYTE bActionID,
		BYTE bWeaponID);

	BYTE FindActionID(
		LPTACTION pTACTION,
		BYTE bActionID,
		BYTE bWeaponID);

	BYTE GetEquipMode(
		BYTE bActionID,
		BYTE bWeaponID);

	FLOAT GetPolyHeight(
		LPD3DXVECTOR3 pPOLY,
		LPD3DXVECTOR3 pPOS,
		FLOAT fRadius);

	BYTE CheckSkillPrevACT( LPTSKILL pTSKILL);
	BYTE CheckSkillRegion( LPTSKILL pTSKILL);
	BYTE CheckSkillWespon( LPTSKILL pTSKILL);
	BYTE CheckSkillSilence( LPTSKILL pTSKILL);
	BYTE CheckSkillMode( LPTSKILL pTSKILL);
	BYTE CheckSkill( LPTSKILL pTSKILL);
	BYTE CheckTransMode( LPTSKILL pTSKILL);

	BYTE GetSkillItemType( LPTSKILL pTSKILL);
	WORD GetItemCount( WORD wItemID);

	BYTE CanUseSubANI();
	BYTE CheckPoly(
		LPD3DXVECTOR3 pPOLY,
		LPD3DXVECTOR3 pTSTART,
		LPD3DXVECTOR3 pTEND,
		FLOAT fRadius,
		FLOAT fSizeY,
		FLOAT fMoveBound,
		FLOAT *pDIST);

	BYTE CheckAction(
		DWORD dwAction,
		DWORD dwAni);

	BYTE IsDisguise();

	// Monster Adjacency
	void ClearAdjacencyMon();
	void AddAdjacency( CTClientObjBase* );
	void LeaveAdjacency( CTClientObjBase* );
	INT FindNearAdjacencyIndex( D3DXVECTOR3& normal );
	INT GetAdjacencyIndex( INT index );
	INT GetSizeGrade();

public:
	virtual void ClearAnimationID();
	virtual void ClearSND();
	virtual void ClearSFX();
	virtual void ClearSFX( LPVECTOROBJSFX pTSFX, DWORD dwID );
	virtual void Release();

	virtual void CalcRender( CD3DDevice *pDevice);
	virtual void CalcOBJSFX( CD3DDevice *pDevice);
	virtual void ClearACTSFX();
	virtual void ClearOBJSFX();

	virtual void CancelTAction();
	virtual void RemoveTAction();

	virtual void OnShotSkill();
	virtual void OnDIE(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);
	virtual LRESULT OnActEndMsg();

	virtual BYTE GetTLOOPAction( BYTE bActionID);
	virtual BYTE GetTAction();

	virtual void ReleaseData();
	virtual CString GetTitle();
	virtual CString GetName();
	virtual CString GetUserTitle();
	virtual DWORD GetHostID();

	virtual BYTE GetWeaponID( BYTE bMode);
	virtual BYTE CheckRequired();
	virtual BYTE CanDIVE();
	virtual BYTE CanFLY();

	virtual BYTE CheckDIR(
		WORD wDIR,
		WORD wPITCH);

	virtual BYTE IsReadyForUSE( CTClientSkill *pTSKILL);
	virtual BYTE IsAlliance( CTClientObjBase *pTARGET);
	virtual BYTE IsInBattle();

	virtual BYTE IsFlyOBJ();
	virtual BYTE IsBlock();
	virtual BYTE IsDead();
	virtual BYTE IsDrawNameWhenDead();
	virtual BYTE IsRoam();
	virtual BYTE IsJump();
	virtual BYTE IsFall();
	virtual BYTE IsPush();
	virtual BYTE IsDown();
	virtual BYTE IsRide();
	virtual BYTE IsSlidingWhenLastHit();

	virtual FLOAT LockDamagePOS();
	virtual FLOAT GetCullRANGE();
	virtual BYTE GetDrawShadow();
	virtual BYTE GetDrawName();

	virtual UINT CountMaintain(
		WORD wSkillID, 
		UINT nLimit = 0xFFFFFFFF);

	virtual UINT CountMaintainFunc(
		BYTE bFuncType, 
		BYTE bFuncID,
		UINT nLimit = 0xFFFFFFFF);

	virtual UINT CountBlockMaintain(
		UINT nLimit = 0xFFFFFFFF);

	virtual UINT CountFlashMaintain(
		UINT nLimit = 0xFFFFFFFF);

	virtual UINT CountHideMaintain(
		UINT nLimit = 0xFFFFFFFF);

	virtual UINT CountPriceUpMaintain(
		INT* pValue );

	virtual UINT CountCanNotUsePetMaintain();

	virtual UINT CountHideDetectMaintain();
    
	virtual void OnMaintainEnd(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		CTClientSkill *pTSKILL);

	virtual void DoTRANS(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		WORD wMonID);

	virtual void DoRETRANS(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	virtual void DoDISGUISE(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		BYTE bCountry);

	virtual void EndDISGUISE(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	virtual void DoSLIDE(
		LPD3DXVECTOR3 pATTACK,
		LPD3DXVECTOR3 pDEFEND,
		BYTE bForceRUN);
	virtual void EndSLIDE();

	virtual void Push(
		FLOAT fPushDist,
		LPD3DXVECTOR2 pDIR);

	virtual void ApplyMatrix( CD3DDevice *pDevice);
	virtual void CalcSUBFrame();
	virtual void CalcSUBTick(
		LPDIRECT3DDEVICE9 pDevice,
		DWORD dwTick);

	virtual CString GetDefaultTALK();
	virtual void ActivateTALK(
		LPDIRECT3DDEVICE9 pDevice,
		CString strTALK,
		DWORD dwCOLOR);

	virtual WORD GetTargetPITCH(
		FLOAT fTargetX,
		FLOAT fTargetY,
		FLOAT fTargetZ);

	virtual WORD GetTargetDIR(
		FLOAT fTargetX,
		FLOAT fTargetZ);

	virtual void CalcDIR(
		WORD wMoveDIR,
		WORD wDIR,
		WORD wPITCH);

	virtual void FinishSUBACT();
	virtual void FinishACT();

	virtual BYTE ForecastHeight(
		CTClientMAP *pTMAP,
		FLOAT *pTHEIGHT,
		FLOAT *pTWATER,
		DWORD dwTick);

	virtual void CalcTick(
		LPDIRECT3DDEVICE9 pDevice,
		DWORD dwTick);

	virtual THIDEPART FindHidePart(
		CTClientItem *pTITEM,
		BYTE bSlotID);
	virtual BYTE CheckOBJPart( LPTHIDEPART pTHIDE);

	virtual void ResetDEFOBJPart( LPTHIDEPART pTHIDE);
	virtual void ResetOBJPart( CD3DDevice *pDevice);

	virtual void RenderTSLASH( CD3DDevice *pDevice);
	virtual void Render(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera);

	void RenderWeaponEffect(
		CD3DDevice* pDevice,
		CD3DCamera *pCamera );

	virtual void ResetTQUESTMARKER(
		CTachyonRes *pRES);

	virtual void ResetWeaponPOS(
		DWORD dwHideEquipSlot,
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	virtual void ResetDEFEQUIP(
		CD3DDevice *pDevice,
		BYTE bEquipPOS);
	
	virtual void ClearDEFEQUIP(
		CD3DDevice *pDevice,
		BYTE bEquipPOS);

	virtual void ResetEQUIP(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	virtual void ResetVisible(
		CTClientCAM *pCamera,
		DWORD dwHouseID,
		BYTE bHouseMesh,
		DWORD dwTick);

	virtual BYTE GetEquipModeIndex(
		BYTE bEquipPOS,
		BYTE bEquipMode);

	virtual OBJHIT HitTest(
		CTClientCAM *pCamera,
		int nPosX,
		int nPosY,
		float fRadScale=1.0f);

	virtual BYTE HitTest(
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fLineH,
		FLOAT fLineV);

	virtual BYTE HitCylinder(
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fLineH,
		FLOAT fLineV);

	virtual BYTE HitBox(
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fLineH,
		FLOAT fLineV);

	virtual BYTE HitPoly(
		FLOAT *pDIST,
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fLineH,
		FLOAT fLineV);

	virtual void SetAction(
		DWORD dwActionID,
		DWORD dwAniID = -1);

	virtual void SetSubAction(
		DWORD dwActionID,
		DWORD dwAniID = -1);

	virtual void ApplyTAction(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		BYTE bAction);

	virtual void SetTAction(
		BYTE bAction);

	virtual FLOAT GetMinRange(
		CTClientObjBase *pTARGET,
		LPTSKILL pTSKILL);

	virtual FLOAT GetMaxRange(
		CTClientObjBase *pTARGET,
		LPTSKILL pTSKILL);

	virtual FLOAT GetHeight(
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT);

	virtual FLOAT GetCylinderHeight(
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT);

	virtual FLOAT GetBoxHeight(
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT);

	virtual FLOAT GetPolyHeight(
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT);

	virtual BYTE CheckCollision(
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pTSTART,
		LPD3DXVECTOR3 pTEND,
		FLOAT fSizeY,
		FLOAT *pDIST);

	virtual BYTE CheckPoly(
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pTSTART,
		LPD3DXVECTOR3 pTEND,
		FLOAT fSizeY,
		FLOAT *pDIST);

	virtual BYTE CheckCylinder(
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pTSTART,
		LPD3DXVECTOR3 pTEND,
		FLOAT fSizeY,
		FLOAT *pDIST);

	virtual BYTE CheckBox(
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pTSTART,
		LPD3DXVECTOR3 pTEND,
		FLOAT fSizeY,
		FLOAT *pDIST);

	virtual BYTE GetSkillActivateID(
		LPTSKILL pTSKILL);

	virtual BYTE GetAftermathStep();

protected:
	using CTachyonObject::m_vPosition;

public:
	inline D3DXMATRIX& GetNativePositionMat()
	{
		return m_vPosition;
	}

	virtual D3DXMATRIX& GetPositionMat();
	virtual D3DXVECTOR3 GetPosition();

	virtual FLOAT GetPositionX();
	virtual FLOAT GetPositionY();
	virtual FLOAT GetPositionZ();

	virtual void SetPosition( D3DXMATRIX& matPOS);
	virtual void SetPosition( D3DXVECTOR3& vPOS);
	
	virtual void SetPositionX(FLOAT fX);
	virtual void SetPositionY(FLOAT fY);
	virtual void SetPositionZ(FLOAT fZ);

	virtual void AddPositionX(FLOAT fX);
	virtual void AddPositionY(FLOAT fY);
	virtual void AddPositionZ(FLOAT fZ);

public:
	CTClientObjBase();
	virtual ~CTClientObjBase();
};
