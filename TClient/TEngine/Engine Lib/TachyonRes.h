#pragma once

class CTachyonRes
{
public:
	static CTachyonMedia m_MEDIA;
	static CD3DDevice *m_pDEVICE;
#ifdef MODIFY_LOADING
	static VECTORTLOADINGSCENE m_vTLOADINGSCENE;
	static D3DCOLOR m_dwBackColor;
#else
	static VECTORIMAGE m_vBACKIMG;
	static CD3DImage m_vGAUGE;
	static int m_nGaugeX;
	static int m_nGaugeY;
	static int m_nTextX;
	static int m_nTextY;
#endif
	static CD3DFont m_vTEXT;
	static CFont m_vFONT;

	static BOOL m_bUseNullTex;
	static LPDIRECT3DTEXTURE9 m_pNULLTEX;

public:
	static void RenderBACK(
		BYTE bSTEP,
		BYTE bProgress);

public:
	VECTORSTRING m_vMESHFILE;
	VECTORSTRING m_vANIFILE;
	VECTORSTRING m_vMAPFILE;

	MAPT3DTEXTURE m_mapIMGBUF;
	MAPRES m_mapTEXSRC;
	MAPRES m_mapIMGSRC;
	MAPRES m_mapANISRC;
	MAPRES m_mapMEDIA;
	MAPRES m_mapMESH;
	MAPRES m_mapTEX;
	MAPRES m_mapIMG;
	MAPRES m_mapANI;
	MAPRES m_mapOBJ;
	MAPRES m_mapSFX;
	MAPRES m_mapMAP;

	BYTE m_bGroupID;

public:
	void Init( CString strGroupID, BYTE bGroupID);
	void Load( CString strGroupID);
	void Complate();

	void ReleaseOBJ( LPOBJECT pOBJ);
	void ReleaseSFX( LPSFX pSFX);

	void LockOBJ( LPOBJECT pOBJ);
	void LockSFX( LPSFX pSFX);

	void LockOBJProgress( LPOBJECT pOBJ, BYTE bStep );

	void LoadIMGBUF( CString strGroupID);
	void LoadMEDIA( CString strGroupID);
	void LoadMESH( CString strGroupID);
	void LoadTEX( CString strGroupID);
	void LoadIMG( CString strGroupID);
	void LoadANI( CString strGroupID);
	void LoadOBJ( CString strGroupID);
	void LoadSFX( CString strGroupID);
	void LoadMAP( CString strGroupID);

	void LoadTEX(
		CString strFILE,
		LPMAPRES pTRESDATA,
		LPMAPRES pTRES,
		int& nIndex,
		int nTotal);

	void LoadIMGBUF(
		CString strFILE,
		int& nIndex,
		int nTotal);

	void LoadIMG(
		CString strFILE,
		LPMAPRES pTRESDATA,
		LPMAPRES pTRES,
		int& nIndex,
		int nTotal);

	void LoadMEDIA(
		DWORD dwID,
		CString strFILE,
		DWORD dwPOS);

	void LoadANI(
		DWORD dwFileID,
		LPMAPRES pTRESDATA,
		LPMAPRES pTRES,
		int& nIndex,
		int nTotal);

	void LoadMESH(
		DWORD dwBASE,
		DWORD dwFileID,
		LPMAPRES pTRES,
		int& nIndex,
		int nTotal);

	void LoadOBJ(
		CString strFILE,
		LPMAPRES pTRES,
		int& nIndex,
		int nTotal);

	void LoadSFX(
		CString strFILE,
		LPMAPRES pTRES,
		int& nIndex,
		int nTotal);

	void ComplateTEX();
	void ComplateIMG();
	void ComplateANI();
	void ComplateOBJ();
	void ComplateSFX();

	CString LoadString( CFile* pFile);
	void LoadNullTEX();

	CTachyonMAP *LockMAP( DWORD dwID);
	LPOBJECT GetOBJ( DWORD dwID);

public:
	CTachyonRes();
	~CTachyonRes();
};
