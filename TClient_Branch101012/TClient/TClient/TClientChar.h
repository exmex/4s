#pragma once


class CTClientChar : public CTClientMoveObj
{
public:
	DWORD m_dwEncryptEXP;

	WORD m_vTSKILLPOINT[TPOINT_COUNT];
	BYTE m_vTRECALL[TRECALL_MAX];

	CString m_strNAME;
	CString m_strGUILD;
	CString m_strTACTICS;

	DWORD m_dwGuildID;
	DWORD m_dwTacticsID;
	BYTE m_bGuildDuty;
	BYTE m_bGuildPeer;

	DWORD m_dwChiefID;
    WORD m_wCommanderID;
	WORD m_wPartyID;

	DWORD m_dwRune;
	DWORD m_dwLuna;
	DWORD m_dwCron;

	DWORD m_dwPrevEXP;
	DWORD m_dwNextEXP;
	//DWORD m_dwEXP;
	
	BYTE m_bAfterMath;
	BYTE m_bRaceID_;

	BYTE m_bSex;
	DWORD m_dwMantleCond;

	BYTE m_bHair;
	BYTE m_bFace;
	BYTE m_bBody;
	BYTE m_bPants;
	BYTE m_bHand;
	BYTE m_bFoot;

	BYTE m_bInPCROOM;
	BYTE m_bPrivateShop;
	CString m_strPrivateShop;

	BOOL m_bEnterCastle;
	WORD m_wCastleID;
	WORD m_wBallID;
	BYTE m_bCamp;

    CTClientGuildMark* m_pGuildMark;
	CTClientPet* m_pRidingPet;

	BYTE m_bPrevMainRecallAI;

	CString m_strUserTitle;

	TSTATINFO m_StatInfo;

	DWORD m_dwPvPTotalPoint;
	DWORD m_dwPvPUseablePoint;

	DWORD m_dwPvPMonthPoint;
	BYTE m_bUseFameTitle;
	CString m_strFameTitle;

public:
	void SetRaceID(BYTE bRace);
	BYTE GetRaceID();
	BYTE GetCostumeRaceID();

	BYTE GetFreeRecallPOS();

	CTClientPet* GetRidingPet() const;

	void SetRidingPet(
		CTClientPet* pTPET);

	void DoResetObj(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	BOOL CanTalkWithNPC(
		CTClientNpc* );

	virtual BYTE GetDrawName();
	virtual void ReleaseData();
	virtual CString GetName();

	virtual THIDEPART FindHidePart(
		CTClientItem *pTITEM,
		BYTE bSlotID);

	virtual BYTE CheckOBJPart(
		LPTHIDEPART pTHIDE);

	virtual void ResetDEFOBJPart(
		LPTHIDEPART pTHIDE);

	virtual void ResetDEFEQUIP(
		CD3DDevice *pDevice,
		BYTE bEquipPOS);

	virtual void ClearDEFEQUIP(
		CD3DDevice *pDevice,
		BYTE bEquipPOS);

	virtual void ResetWeaponPOS(
		DWORD dwHideEquipSlot,
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

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

	virtual BYTE IsAlliance(
		CTClientObjBase *pTARGET);

	virtual void OnDIE(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	virtual void ActivateTALK(
		LPDIRECT3DDEVICE9 pDevice,
		CString strTALK,
		DWORD dwCOLOR);

	virtual D3DXMATRIX& GetPositionMat();

	virtual CString GetUserTitle() { return m_strUserTitle; }

	virtual void ResetEQUIP(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	void ApplyMantleTexture();
	virtual BYTE GetAftermathStep();

	bool GetHasShield();

public :	// º∫π∞¿Ô≈ª¿¸.

	void PlayGodBallSFX( BOOL bShow = TRUE );
	void ClearGodBallSFX();

	void EnterCastle();
	void LeaveCastle();

	BOOL InCastle() const;
	WORD GetCastleID() const;
	WORD GetGodBallID() const;
	BYTE GetCamp() const;

	void SetCastleID( WORD wCastleID );
	void SetGodBallID( WORD wBallID );
	void SetCamp( BYTE bCamp );

	DWORD GetEXP();
	void SetEXP(DWORD dwEXP);

public:
	CTClientChar();
	virtual ~CTClientChar();

public:
	DWORD m_zEncryptKEY;
};

