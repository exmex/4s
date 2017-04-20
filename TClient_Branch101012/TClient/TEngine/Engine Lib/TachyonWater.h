#pragma once


class CTachyonWater
{
public:
	void RenderREF( LPDIRECT3DDEVICE9 pDevice);
	void Render( LPDIRECT3DDEVICE9 pDevice);

	void Release();
	void Init(
		LPDIRECT3DDEVICE9 pDevice,
		LPTEXTURESET pSPEC,
		FLOAT fLENGTH,
		FLOAT fSCALE,
		FLOAT fREF,
		DWORD dwVBCnt,
		WORD wMaxBlock,
		WORD wMainBound,
		WORD wSubBound,
		WORD wCountX,
		WORD wCountZ,
		WORD wSTEP,
		WORD wREF,
		BYTE bLEVEL);

	void BuildBlock(
		VECTORBLOCK& vBlock,
		CD3DCamera *pCamera);

	void ResetREF(
		LPDIRECT3DDEVICE9 pDevice,
		D3DXVECTOR3 vPOS);

	void ResetVB(
		LPDIRECT3DDEVICE9 pDevice,
		VECTORBLOCK& vBlock);

	FLOAT GetHeight(
		FLOAT fPosX,
		FLOAT fPosZ);

protected:
	MESHVERTEX GetGEO(
		int nPosX,
		int nPosZ);

	FLOAT GetDistance(
		D3DXVECTOR3& vPos,
		CRect& rect);

	BOOL RectInView(
		CD3DCamera *pCamera,
		CRect& rect);

	void LODMatric(
		VECTORBLOCK& vBlock,
		CD3DCamera *pCamera,
		CRect rect);

public:
	LPDIRECT3DVERTEXBUFFER9 m_pREF;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	MAPDWORD m_mapUSED;

	LPTEXTURESET m_pSPEC;
	LPBYTE m_pDATA;
	FLOAT *m_pGEO;

	D3DMATERIAL9 m_vMaterial;
	D3DXMATRIX m_vPosition;

	DWORD m_dwPolyCnt;
	DWORD m_dwVBCnt;

	WORD m_wMainBound;
	WORD m_wSubBound;
	WORD m_wMaxBlock;
	WORD m_wCountX;
	WORD m_wCountZ;
	WORD m_wSTEP;
	WORD m_wREF;

	FLOAT m_fLENGTH;
	FLOAT m_fSCALE;
	FLOAT m_fUNIT;
	FLOAT m_fREF;
	BYTE m_bLAYER;
	BYTE m_bLEVEL;

public:
	CTachyonWater();
	~CTachyonWater();
};
