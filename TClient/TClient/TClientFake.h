#pragma once


class CTClientFake : public CTClientRecall
{
public:
	CTClientChar *m_pHOST;
	D3DXVECTOR3 m_vTROAM;
	TMONTEMP m_vTEMP;

	BYTE m_bRaceID;
	BYTE m_bSex;

	BYTE m_bHair;
	BYTE m_bFace;
	BYTE m_bBody;
	BYTE m_bPants;
	BYTE m_bHand;
	BYTE m_bFoot;

public:
	virtual BYTE GetDrawName();
	virtual void ReleaseData();

	virtual THIDEPART FindHidePart(
		CTClientItem *pTITEM,
		BYTE bSlotID);
	virtual BYTE CheckOBJPart( LPTHIDEPART pTHIDE);

	virtual void ResetDEFOBJPart( LPTHIDEPART pTHIDE);
	virtual void ResetDEFEQUIP(
		CD3DDevice *pDevice,
		BYTE bEquipPOS);

	virtual void DoRETRANS(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	virtual void InitRecall(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		WORD wTempID,
		BYTE bLevel);

	virtual D3DXVECTOR3 GetRoamTarget(
		LPD3DXMATRIX pDIR,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);
	
	virtual D3DXVECTOR3 AdjustRoamTarget(
		CTClientChar* pHOST,
		CTClientMAP* pMAP,
		LPD3DXMATRIX pDIR,
		D3DXVECTOR3 vTARGET );

public:
	CTClientFake();
	virtual ~CTClientFake();
};
