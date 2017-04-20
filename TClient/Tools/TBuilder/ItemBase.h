#if (!defined ___ITEMBASE_H)
#define ___ITEMBASE_H


class CItemBase : public CObject
{
public:
	static CAnimationItem* GetAnimationItem( DWORD dwID);
	static CTextureItem* GetTextureItem( DWORD dwID);
	static CEXTMAPItem* GetEXTMAPItem( DWORD dwID);
	static CBSPMAPItem* GetBSPMAPItem( DWORD dwID);
	static CImageItem* GetImageItem( DWORD dwID);
	static CMediaItem* GetMediaItem( DWORD dwID);
	static CFRAMEItem* GetFRAMEItem( DWORD dwID);
	static CCOMPItem* GetCOMPItem( DWORD dwID);
	static CMeshItem* GetMeshItem( DWORD dwID);
	static CMAPItem* GetMAPItem( DWORD dwID);
	static COBJItem* GetOBJItem( DWORD dwID);
	static CSFXItem* GetSFXItem( DWORD dwID);

	static void SetValidTexture( DWORD dwID);
	static void SetValidMesh( DWORD dwID);

	static CItemBase* CreateItem( LPTGSFFILE pGSFFILE );
	static CString LoadString( CFile* pFile);
	static DWORD GetID( CString strID);

	static void WriteString(
		CFile* pFile,
		CString& strValue);

	static DWORD RegLocalID(
		CString strID,
		CItemBase *pItem);

	static void ReleaseLocalID(
		CString strID,
		CItemBase *pItem);

	static char m_szTempPath[_MAX_PATH];

public:
	static LPDIRECT3DVERTEXBUFFER9 m_pVB;
	static CD3DDevice m_Device;
	static MAPSTRID m_mapGlobalID;

	static MAPIDINDEX m_mapLocalID;
	static MAPGSFONT m_mapFont;
	static MAPSTR m_mapMenuID;

	static MAPITEM m_mapAnimationItem;
	static MAPITEM m_mapTextureItem;
	static MAPITEM m_mapImageItem;
	static MAPITEM m_mapMediaItem;
	static MAPITEM m_mapFrameItem;
	static MAPITEM m_mapCompItem;
	static MAPITEM m_mapMeshItem;
	static MAPITEM m_mapMAPItem;
	static MAPITEM m_mapOBJItem;
	static MAPITEM m_mapSFXItem;
	
	static DWORD g_dwAnimationID;
	static DWORD g_dwOBJTypeID;
	static DWORD g_dwTextureID;
	static DWORD g_dwImageID;
	static DWORD g_dwMediaID;
	static DWORD g_dwFrameID;
	static DWORD g_dwCompID;
	static DWORD g_dwMeshID;
	static DWORD g_dwMAPID;
	static DWORD g_dwOBJID;
	static DWORD g_dwSFXID;
	static DWORD g_dwLocalID;

	static BYTE m_bMerge;
	static BYTE m_bOptimize;

public:
	virtual void Load( LPTGSFFILE );

public:
	VECTORSTRING m_vLocalID;

	CString m_strName;
	CString m_strID;

	DWORD m_dwID;
	BYTE m_bType;
	BYTE m_bValid;

public:
	CItemBase( BYTE bType);
	~CItemBase();
};


class CProjectItem : public CItemBase
{
public:
	CProjectItem();
	virtual ~CProjectItem();
};


class CFolderItem : public CItemBase
{
public:
	CFolderItem();
	virtual ~CFolderItem();
};


class CTextureItem : public CItemBase
{
public:
	virtual void Load( LPTGSFFILE );

public: // pre-loaded ID information for remove optimization.
	VECTORDWORD m_vTextureID;

public: // body data.
	LPBYTE m_pData;
	DWORD m_dwSize;
	BYTE m_bFullSize;
	BYTE m_bFormat;
	TEXTURESET* m_pTextureSet;

public: // dynamic loading info.
	LPTGSFFILE m_pGSFFILE;
	ULONGLONG m_dwGSFOffset;
	BOOL LoadData();
	void ReleaseData();

public:
	CTextureItem();
	virtual ~CTextureItem();
};


class CImageItem : public CItemBase
{
public:
	virtual void Load( LPTGSFFILE );

public: // pre-loaded ID information for remove optimization.
	VECTORDWORD m_vImageID;
	int m_nWidth;
	int m_nHeight;

public:  // body data.
	LPBYTE m_pData2;
	DWORD m_dwSize;
	IMAGESET* m_pImageSet;
	BYTE m_bFormat2;

public: // dynamic loading info.
	LPTGSFFILE m_pGSFFILE;
	ULONGLONG m_dwGSFOffset;
	BOOL LoadData();
	void ReleaseData();

public:
	CImageItem();
	virtual ~CImageItem();
};


class CMediaItem : public CItemBase
{
public:
	virtual void Load( LPTGSFFILE );

public:
	BYTE GetPlayerType();

public:
	LPTGSFFILE m_pGSFFILE;
	ULONGLONG m_dwGSFOffset;
	BOOL LoadData();
	void ReleaseData();

public:
	BYTE m_bMediaType;
	BYTE m_bVolume;
	LPBYTE m_pData;
	DWORD m_dwSize;

public:
	CMediaItem();
	virtual ~CMediaItem();
};


class CAnimationItem : public CItemBase
{
public:
	virtual void Load( LPTGSFFILE );

public:
	LPBYTE m_pData;
	DWORD m_dwSize;

public:
	CString m_strLoopID;
	ANISET m_Aniset;
	DWORD m_dwNodeCount;

	VECTORSTRING m_vEventID;
	VECTORDWORD m_vAniID;
	VECTORBYTE m_vReverse;

public:
	LPTGSFFILE m_pGSFFILE;
	ULONGLONG m_dwGSFOffset;
	BOOL LoadData();
	void ReleaseData();

public:
	CAnimationItem();
	virtual ~CAnimationItem();
};


class CMeshItem : public CItemBase
{
public:
	virtual void Load( LPTGSFFILE );

public:
	CTachyonMesh m_Mesh;
	DWORD m_dwNodeCount;

public:
	LPTGSFFILE m_pGSFFILE;
	ULONGLONG m_dwGSFOffset;
	BOOL LoadData();
	void ReleaseData();

public:
	CMeshItem();
	virtual ~CMeshItem();
};


class COBJTypeItem : public CItemBase
{
public:
	virtual void Load( LPTGSFFILE );

public:
	MAPATTRITEM m_mapATTR;
	OBJECT m_OBJ;

	MAPSTR m_mapNAME;

public:
	COBJTypeItem();
	virtual ~COBJTypeItem();
};


class COBJItem : public CItemBase
{
public:
	virtual void Load( LPTGSFFILE );

public:
	int GetPivotCount( CAnimationItem *pItem);
	int GetPivotCount();

	DWORD GetTexID(
		DWORD dwMeshID,
		int nIndex,
		int nStage);

	DWORD GetColorID(
		DWORD dwMeshID,
		int nIndex);

public:
	MAPATTRITEM m_mapATTR;
	MAPSTR m_mapPIVOT;
	OBJECT m_OBJ;

	MAPTEXINDEX m_mapCOLOR;
	MAPTEXINDEX m_mapTI;
	MAPDWORD m_mapMESH;
	MAPDWORD m_mapTEX;
	MAPDWORD m_mapANI;
	MAPDWORD m_mapSFX;
	MAPSTR m_mapNAME;

public:
	COBJItem();
	virtual ~COBJItem();
};


class CSFXItem : public CItemBase
{
public:
	virtual void Load( LPTGSFFILE );

public:
	SFX* m_pSFX;

	VECTORDWORD m_vSFXID;
	DWORD m_dwTotalFrame;
	DWORD m_dwSANISRC;
	DWORD m_dwANISRC;
	DWORD m_dwSFXSRC;
	DWORD m_dwCurCLK;

public:
	LPTGSFFILE m_pGSFFILE;
	ULONGLONG m_dwGSFOffset;
	BOOL LoadData();
	void ReleaseData();

public:
	CSFXItem();
	virtual ~CSFXItem();
};


class CMAPItem : public CItemBase
{
public:
	virtual void Load( LPTGSFFILE );

public:
	void SyncOBJATTR(
		COBJItem *pItem,
		LPMAPOBJ pOBJ);

public:
	MAPMAPLIGHT m_mapLIGHT;
	MAPDWORD m_mapOBJ;
	MAPDWORD m_mapSFX;

	BYTE m_bMAPType;

public:
	CMAPItem( BYTE bType);
	virtual ~CMAPItem();
};


class CEXTMAPItem : public CMAPItem
{
public:
	virtual void Load( LPTGSFFILE );

public:
	void LoadOBJ(
		MAPMAPOBJ *pOBJ,
		MAPMAPSFX *pSFX,
		MAPMAPSND *pSND,
		MAPDWORD *pOBJID,
		MAPDWORD *pSFXID,
		DWORD dwIndex);

	void LoadUNIT(
		LPDIRECT3DTEXTURE9 *pSHADE,
		LPDIRECT3DTEXTURE9 *pSKIN,
		LPDIRECT3DTEXTURE9 *pTILE,
		LPVECTORTEXTURE pSHADOW,
		LPBYTE pREGION,
		LPVECTORWORD pVDETAIL,
		LPBYTE pDETAIL,
		FLOAT *pHEIGHT,
		DWORD dwIndex,
		BOOL bSYSMEM);

	void BuildSREGION(
		LPVECTORTEXTURE pSHADOW,
		LPWORD pSREGION);

	void Blend(
		LPDIRECT3DTEXTURE9 pTARGET,
		LPDIRECT3DTEXTURE9 pIMG,
		FLOAT fPosX,
		FLOAT fPosZ);

public:
	CTachyonHUGEMAP* m_pHugeMAP;
	CString m_strUNIT;
	CString m_strOBJ;

	VECTORFLOAT m_vDETAILS;
	VECTORDWORD m_vDETAIL;
	VECTORGEOTEX m_vGEO;

	FLOAT m_fMinHeight;
	FLOAT m_fMaxHeight;

	DWORD m_dwNewOBJID;
	DWORD m_dwNewSFXID;
	DWORD m_dwNewSNDID;

public:
	LPTGSFFILE m_pGSFFILE;
	ULONGLONG m_dwGSFOffset;
	void ReleaseData();

public:
	CEXTMAPItem();
	virtual ~CEXTMAPItem();
};


class CBSPMAPItem : public CMAPItem
{
public:
	virtual void Load( LPTGSFFILE );
	static void SeekBSPNode( CFile* pFile );

public:
	void ClearDATA();
	void LoadDATA();

public:
	CTachyonBSPMAP m_BSPMAP;
	VECTORMAPDWORD m_vTEX;
	CString m_strPATH;

	MAPDWORD m_mapRegionID;
	DWORD m_dwTexOffsetID;
	DWORD m_dwObjOffsetID;
	DWORD m_dwSfxOffsetID;
	DWORD m_dwMediaOffsetID;

public:
	LPTGSFFILE m_pGSFFILE;
	ULONGLONG m_dwGSFOffset;
	BOOL LoadData();
	void ReleaseData();

public:
	CBSPMAPItem();
	virtual ~CBSPMAPItem();
};


class CGuideItem : public CItemBase
{
public:
	virtual void Load( LPTGSFFILE );

public:
	VECTORINT m_vGuideH;
	VECTORINT m_vGuideV;

public:
	CGuideItem( BYTE bType);
	virtual ~CGuideItem();
};


class CCOMPItem : public CGuideItem
{
public:
	virtual void Load( LPTGSFFILE );

public:
	VECTORCOMP m_vCOMP;

	DWORD m_dwDefaultID;
	DWORD m_dwDisableID;
	BYTE m_bCtrlType;

	int m_nHMargine;
	int m_nVMargine;
	int m_nLevel;
	int m_nWidth;
	int m_nHeight;

public:
	LPTGSFFILE m_pGSFFILE;
	ULONGLONG m_dwGSFOffset;


public:
	CCOMPItem();
	virtual ~CCOMPItem();
};


class CFRAMEItem : public CGuideItem
{
public:
	virtual void Load( LPTGSFFILE );

public:
	VECTORCTRL m_vCTRL;

	CString m_strFontID;
	DWORD m_dwDefaultID;
	DWORD m_dwDisableID;

	int m_nWidth;
	int m_nHeight;

public:
	LPTGSFFILE m_pGSFFILE;
	ULONGLONG m_dwGSFOffset;


public:
	CFRAMEItem();
	virtual ~CFRAMEItem();
};


#endif // #if (!defined ___ITEMBASE_H)
