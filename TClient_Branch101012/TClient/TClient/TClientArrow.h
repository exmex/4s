#pragma once


class CTClientArrow : public CTachyonObject
{
public:
	LPTSKILLTARGET m_pTARGET;
	TSKILLDATA m_vTSKILLDATA;

	CTachyonSlashSFX m_vTSLASH;
	VECTOROBJSFX m_vTARROWSFX;
	D3DXVECTOR2 m_vTACCEL;

	DWORD m_dwHostID;
	BYTE m_bHostType;
	BYTE m_bAccel;

	FLOAT m_fVelocityX;
	FLOAT m_fGravity;
	FLOAT m_fStartH;

	DWORD m_dwTotalTick;
	DWORD m_dwTick;

public:
	void PlayTArrowSFX( DWORD dwFuncID);
	void RemoveTArrowSFX();

public:
	virtual void Release();

	virtual void CalcTick(
		LPDIRECT3DDEVICE9 pDevice,
		DWORD dwTick);

	virtual void Render(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera);

public:
	CTClientArrow();
	virtual ~CTClientArrow();
};
