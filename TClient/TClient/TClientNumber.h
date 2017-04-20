#pragma once


class CTClientNumber
{
public:
	static LPTEXTURESET m_pTEX;
	static int m_nTexPOS[11][3];

public:
	static void BeginDraw(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera);
	static void EndDraw( CD3DCamera *pCamera);

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	VECTORBYTE m_vDIGIT;

	DWORD m_dwColor;
	DWORD m_dwTick;

	int m_nNumber;

public:
	D3DXVECTOR3 m_vPOS;
	D3DXVECTOR2 m_vDIR;
	FLOAT m_fSCALE;

public:
	void InitNumber(
		CD3DDevice *pDevice,
		DWORD dwColor,
		int nNumber);

	void Release();
	BYTE IsDead();

public:
	virtual void CalcTick( DWORD dwTick);
	virtual void Render(
		CD3DDevice *pDevice,
		LPD3DXMATRIX pPROJ);

public:
	CTClientNumber();
	virtual ~CTClientNumber();
};
