#pragma once


class CTSpCode
{
public:
	TSCENE_PLAYER_CODE m_eCode;
	LPTEXTURESET m_pTEX[2];

	DWORD m_dwCOLOR;
	DWORD m_dwTotal;
	BYTE m_bTEXT;
	DWORD m_dwUV;
	DWORD m_dwUV2;
	int m_nAct;

public:
	DWORD m_dwTexTick[2];
	DWORD m_dwTick;

public:
	void Render( CTClientGame *pTGAME);
	BYTE CalcTick(
		CTClientGame *pTGAME,
		DWORD dwTick);

protected:
	DWORD CalcTexTick(
		BYTE bINDEX,
		DWORD dwTick);

	void Fill(
		CTClientGame *pTGAME,
		LPDIRECT3DTEXTURE9 pTEX1,
		LPDIRECT3DTEXTURE9 pTEX2,
		DWORD dwCOLOR1,
		DWORD dwCOLOR2,
		DWORD dwUV1,
		DWORD dwUV2 );

	void Fill(
		CTClientGame *pTGAME,
		LPDIRECT3DTEXTURE9 pTEX,
		DWORD dwCOLOR,
		DWORD dwUV );

	void FillFade(
		CTClientGame *pTGAME,
		LPDIRECT3DTEXTURE9 pTEX1,
		LPDIRECT3DTEXTURE9 pTEX2,
		BYTE bAlpha,
		DWORD dwUV1,
		DWORD dwUV2 );

	D3DXVECTOR3 GetCameraDIR(
		BYTE bCountry );
public:
	CTSpCode();
	virtual ~CTSpCode();
};


class CTScenePlayer
{
public:
	MAPTEXTURESET m_mapTEX;
	VTSPCODE m_vTSCENE;
	INT m_nCurScene;
	BYTE m_bINDEX;
	BYTE m_bPrevCountry;

public:
	void Render( CTClientGame *pTGAME);
	void Release();
	BYTE IsRunning();

	void LoadScript(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		CString *pScript,
		int nCount);

	void StartScene(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		int nTSCENE);

	BYTE CalcTick(
		CTClientGame *pTGAME,
		DWORD dwTick);

public:
	CTScenePlayer();
	virtual ~CTScenePlayer();
};
