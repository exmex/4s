#pragma once


class CTSpCode
{
public:
	TSCENE_PLAYER_CODE m_eCode;
	LPTEXTURESET m_pTEX[2];

	DWORD m_dwCOLOR;
	DWORD m_dwTotal;
	BYTE m_bTEXT;

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
		DWORD dwCOLOR2);

	void Fill(
		CTClientGame *pTGAME,
		LPDIRECT3DTEXTURE9 pTEX,
		DWORD dwCOLOR);

public:
	CTSpCode();
	virtual ~CTSpCode();
};


class CTScenePlayer
{
public:
	MAPTEXTURESET m_mapTEX;
	VTSPCODE m_vTSCENE;

	BYTE m_bINDEX;

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
