#pragma once

class CTachyonSlashSFX
{
public:
	static CT3DVertex m_vVB;
	static BYTE m_bSTEP;

public:
	static void InitVB( BYTE bSTEP);
	static void ReleaseVB();

	static void BeginSFXScene( CD3DDevice *pDevice);
	static void EndSFXScene( CD3DDevice *pDevice);

public:
	VECTORMATRIX m_vKEY;

	LPD3DXMATRIX m_pPIVOT;
	LPTEXTURESET m_pTEX;
	D3DXMATRIX m_vLENGTH;

	DWORD m_dwTexTick;
	DWORD m_dwCOLOR;
	BYTE m_bINTER;
	BYTE m_bSTOP;

public:
	void ResetKEY();
	void CalcKEY();
	BYTE IsPlay();

	void InitSFX(
		LPD3DXMATRIX pPIVOT,
		LPTEXTURESET pTEX,
		DWORD dwCOLOR,
		FLOAT fLength,
		BYTE bINTER);

	void SFXStart();
	void SFXStop();

public:
	virtual void Render( CD3DDevice *pDevice);
	virtual void CalcTick( DWORD dwTick);

public:
	CTachyonSlashSFX();
	virtual ~CTachyonSlashSFX();
};
