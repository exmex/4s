#pragma once


class CTMapViewerChar : public CTMapViewerMoveObj
{
public:
	CString m_strNAME;

	BYTE m_bRaceID;
	BYTE m_bSex;

	BYTE m_bHair;
	BYTE m_bFace;
	BYTE m_bBody;
	BYTE m_bPants;
	BYTE m_bHand;
	BYTE m_bFoot;

public:
	virtual void ReleaseData();
	virtual CString GetName();

	virtual THIDEPART FindHidePart(
		CTMapViewerItem *pTITEM,
		BYTE bSlotID);
	virtual BYTE CheckOBJPart( LPTHIDEPART pTHIDE);

	virtual void ResetDEFOBJPart( LPTHIDEPART pTHIDE);
	virtual void ResetDEFEQUIP(
		CD3DDevice *pDevice,
		BYTE bEquipPOS);

public:
	CTMapViewerChar();
	virtual ~CTMapViewerChar();
};
