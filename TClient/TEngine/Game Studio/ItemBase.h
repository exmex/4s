#if (!defined ___ITEMBASE_H)
#define ___ITEMBASE_H

#include "ProgressDlg.h"
#include "TachyonDoc.h"
#include "atltypes.h"


#define ITEM_PROJECT						((BYTE) 0x00)
#define ITEM_FOLDER							((BYTE) 0x01)
#define ITEM_TEXTURE						((BYTE) 0x02)
#define ITEM_IMAGE							((BYTE) 0x03)
#define ITEM_MEDIA							((BYTE) 0x04)
#define ITEM_ANIMATION						((BYTE) 0x05)
#define ITEM_MESH							((BYTE) 0x06)
#define ITEM_OBJTYPE						((BYTE) 0x07)
#define ITEM_OBJECT							((BYTE) 0x08)
#define ITEM_SFX							((BYTE) 0x09)
#define ITEM_EXTMAP							((BYTE) 0x0A)
#define ITEM_BSPMAP							((BYTE) 0x0B)
#define ITEM_COMP							((BYTE) 0x0C)
#define ITEM_FRAME							((BYTE) 0x0D)

#define APPLY_NONE							((BYTE) 0x00)
#define APPLY_X								((BYTE) 0x01)
#define APPLY_Y								((BYTE) 0x02)
#define APPLY_Z								((BYTE) 0x03)
#define APPLY_ALL							((BYTE) 0x04)

#define CM_NONE								((BYTE) 0x00)
#define CM_ROT								((BYTE) 0x01)
#define CM_ZOOM								((BYTE) 0x02)
#define CM_MOVE								((BYTE) 0x03)
#define CM_PAINT							((BYTE) 0x04)
#define CM_FLATH							((BYTE) 0x05)
#define CM_HBRUSH							((BYTE) 0x06)
#define CM_SMOOTH							((BYTE) 0x07)
#define CM_HEIGHT							((BYTE) 0x08)
#define CM_REGION							((BYTE) 0x09)
#define CM_DETAIL							((BYTE) 0x0A)
#define CM_SELOBJ							((BYTE) 0x0B)
#define CM_SELSFX							((BYTE) 0x0C)
#define CM_SELSND							((BYTE) 0x0D)
#define CM_SELLIGHT							((BYTE) 0x0E)
#define CM_OBJSPARY							((BYTE) 0x0F)
#define CM_ADDFLATH							((BYTE) 0x10)

#define CT_NONE								((BYTE) 0x00)
#define CT_BUTTON							((BYTE) 0x01)
#define CT_SCROLL							((BYTE) 0x02)
#define CT_EDIT								((BYTE) 0x03)
#define CT_LIST								((BYTE) 0x04)
#define CT_COMBO							((BYTE) 0x05)
#define CT_GAUGE							((BYTE) 0x06)
#define CT_IMGLIST							((BYTE) 0x07)
#define CT_METER							((BYTE) 0x08)
#define CT_TABCTRL							((BYTE) 0x09)

#define GSTA_LEFT							((BYTE) 0x00)
#define GSTA_CENTER							((BYTE) 0x01)
#define GSTA_RIGHT							((BYTE) 0x02)

#define OEK_OBJ								((BYTE) 0x00)
#define OEK_ACT								((BYTE) 0x01)
#define OEK_ANI								((BYTE) 0x02)

#define OSK_OBJ								((BYTE) 0x00)
#define OSK_ACT								((BYTE) 0x01)
#define OSK_ANI								((BYTE) 0x02)

#define EXT_UNIT_HEIGHT						((BYTE) 0x00)		// File Write Order
#define EXT_UNIT_SHADER						((BYTE) 0x01)
#define EXT_UNIT_SKIN						((BYTE) 0x02)
#define EXT_UNIT_REGION						((BYTE) 0x03)
#define EXT_UNIT_DETAIL						((BYTE) 0x04)
#define EXT_UNIT_TILE						((BYTE) 0x05)
#define EXT_UNIT_SHADOW						((BYTE) 0x06)
#define EXT_UNIT_UPDATE_CNT					((BYTE) 0x07)

#define EXT_EXPORT_GEO_INIT_DETAIL			((BYTE) 0x00)
#define EXT_EXPORT_GEO_RESET_UNIT_DETAIL	((BYTE) 0x01)
#define EXT_IMPORT_GEO_DETAIL				((BYTE) 0x02)

#define ROOT_ACTIONID						((DWORD) 0x10000000)
#define ROOT_CLKINDID						((DWORD) 0x20000000)
#define ROOT_CLOTHID						((DWORD) 0x30000000)
#define ROOT_MESHID							((DWORD) 0x40000000)
#define ROOT_ANIID							((DWORD) 0x50000000)
#define ROOT_ATTRID							((DWORD) 0x60000000)
#define ROOT_TEXID							((DWORD) 0x70000000)
#define ROOT_COLORID						((DWORD) 0x80000000)

#define CENTER_BOUND						64


typedef struct tagATTRITEM					ATTRITEM, *LPATTRITEM;
typedef struct tagTEXINDEX					TEXINDEX, *LPTEXINDEX;
typedef struct tagBMPARAM					BMPARAM, *LPBMPARAM;
typedef struct tagADOBPARAM					ADOBPARAM, *LPADOBPARAM;
typedef struct tagIDINDEX					IDINDEX, *LPIDINDEX;
typedef struct tagGEOTEX					GEOTEX, *LPGEOTEX;
typedef struct tagDETTEX					DETTEX, *LPDETTEX;
typedef struct tagRMERGEWORK				RMERGEWORK, *LPRMERGEWORK;
typedef struct tagGSFONT					GSFONT, *LPGSFONT;
typedef struct tagLAYER						LAYER, *LPLAYER;
typedef struct tagCOMP						COMP, *LPCOMP;
typedef struct tagCTRL						CTRL, *LPCTRL;
typedef struct tagSELECT_FORMAT			SELECT_FORMAT, *LPSELECT_FORMAT;

class CAnimationItem;
class CTextureItem;
class COBJTypeItem;
class CEXTMAPItem;
class CBSPMAPItem;
class CImageItem;
class CMediaItem;
class CFRAMEItem;
class CCOMPItem;
class CMeshItem;
class CItemBase;
class CMAPItem;
class COBJItem;
class CSFXItem;


typedef vector<CTachyonObject *>			VECTOROBJECT, *LPVECTOROBJECT;
typedef vector<CFRAMEItem *>				VECTORFRAMEITEM;
typedef vector<CCOMPItem *>					VECTORCOMPITEM;
typedef vector<LPMAPDWORD>					VECTORMAPDWORD;
typedef vector<LPIDINDEX>					VECTORIDINDEX;
typedef vector<LPGEOTEX>					VECTORGEOTEX;
typedef vector<LPRMERGEWORK>				VECTORRMERGEWORK;
typedef vector<LPCTRL>						VECTORCTRL;
typedef vector<LPCOMP>						VECTORCOMP;
typedef vector<WORD>						VECTORWORD;

typedef map< DWORD, LPATTRITEM>				MAPATTRITEM;
typedef map< DWORD, LPTEXINDEX>				MAPTEXINDEX;
typedef map< DWORD, CItemBase *>			MAPITEM;
typedef map< DWORD, LPGSFONT>				MAPGSFONT;
typedef map< DWORD, COLORREF>				MAPCOLOR;
typedef map< FLOAT, LPLAYER>				MAPLAYER;
typedef map< DWORD, CString>				MAPSTR, *LPMAPSTR;

typedef map< CString, VECTORIDINDEX *>		MAPIDINDEX;
typedef map< CString, DWORD>				MAPSTRID;

typedef std::vector<SELECT_FORMAT>		VSELECT_FORMAT;

class CTachyonAnimationGS : public CTachyonAnimation
{
public:
	CTachyonAnimationGS( CAnimationItem& );
	virtual ~CTachyonAnimationGS();

	virtual void GetFrameMatrix(
		LPD3DXMATRIX pResult,
		LPD3DXMATRIX pBlend,
		LPD3DXQUATERNION pBlendKEY,
		LPTPIVOT pPivot,
		D3DXMATRIX& vWorld,
		BYTE bRootID,
		FLOAT fTimeValue,
		FLOAT fBlendTime);

	virtual int HitTest(
		LPD3DXMATRIX pBones,
		CD3DCamera *pCam,
		int nPosX,
		int nPosY);

	virtual BOOL GetSize(
		D3DXVECTOR3& vMin,
		D3DXVECTOR3& vMax);

	virtual DWORD GetInterval();

	virtual DWORD GetNodeCount();

protected:
	CAnimationItem& m_vParent;
};

class TEXTURESETSTUDIO : public tagTEXTURESET
{
protected:
	std::vector< CTextureItem* >	m_vTextureItems;

public:
	void PushTextureItem( CTextureItem* pTextureItem);
	void InsertTextureItem( int nIndex, CTextureItem* pTextureItem);
	void EraseTextureItem( int nIndex);
	virtual CT3DTexture *GetTexturePtr( int nIndex);
	void ClearTextureItem();
};

typedef TEXTURESETSTUDIO		*LPTEXTURESETSTUDIO;

struct tagADOBPARAM
{
	CTachyonObject* m_pOBJ;
	CProgressDlg *m_pWnd;
	CMAPItem *m_pHost;

	D3DXVECTOR3* m_vMovePos;
	DWORD m_dwBASEOBJID;
	DWORD m_dwUnitPOS;

	BOOL m_bAttachType;

	tagADOBPARAM()
	{
		m_bAttachType = FALSE;
		m_dwUnitPOS = 0;
		m_dwBASEOBJID = 0;
		m_pOBJ = NULL;
		m_pHost = NULL;
		m_pWnd = NULL;
		m_vMovePos = NULL;
	};
};

struct tagBMPARAM
{
	CString m_strFileName;

	CProgressDlg *m_pWnd;
	CMAPItem *m_pHost;

	VECTORDWORD m_vUNIT;

	tagBMPARAM()
	{
		m_strFileName.Empty();
		m_pHost = NULL;
		m_pWnd = NULL;
		m_vUNIT.clear();
	};
};

struct tagIDINDEX
{
	DWORD m_dwID;
	BYTE m_bType;

	tagIDINDEX()
	{
		m_bType = 0;
		m_dwID = 0;
	};
};


struct tagATTRITEM
{
	CString m_strValue;
	BYTE m_bType;

	tagATTRITEM()
	{
		m_strValue.Empty();
		m_bType = 0;
	};
};


struct tagTEXINDEX
{
	DWORD m_dwMeshID;

	int m_nIndex;
	int m_nStage;

	tagTEXINDEX()
	{
		m_dwMeshID = 0;

		m_nIndex = -1;
		m_nStage = 0;
	};
};

struct tagGEOTEX
{
	DWORD m_dwTEXID;
	FLOAT m_fHeight;

	tagGEOTEX()
	{
		m_fHeight = 0.0f;
		m_dwTEXID = 0;
	};
};

struct tagDETTEX
{
	DWORD m_dwTEXID;
	DWORD m_dwREPID;

	FLOAT m_fSCALE;

	tagDETTEX()
	{
		m_dwTEXID = 0;
		m_dwREPID = 0;

		m_fSCALE = 32.0f;
	};
};

struct tagRMERGEWORK
{
	VECTORDWORD m_vADDS;
	DWORD m_dwTARGET;	

	tagRMERGEWORK()
	{
		m_dwTARGET = 0xFFFFFFFF;
	};
};

struct tagGSFONT
{
	CString m_strFontID;

	CD3DFont m_D3DFONT;
	CFont m_GDIFONT;
	DWORD m_dwColor;

	LOGFONT m_FONT;

	tagGSFONT()
	{
		m_dwColor = 0x00000000;
		m_strFontID.Empty();
	};
};


struct tagCTRL
{
	CString m_strMenuID[TCML_MENU_COUNT];
	CString m_strCtrlID;
	CString m_strFontID;
	CString m_strTipID;

	CString m_strToolTip;
	CString m_strText;

	DWORD m_dwCompID;
	DWORD m_dwColor;
	DWORD m_dwFlag;
	BYTE m_bAlign;

	int m_nPosX;
	int m_nPosY;

	tagCTRL()
	{
		for( int i=0; i<TCML_MENU_COUNT; i++)
			m_strMenuID[i].Empty();

		m_strCtrlID.Empty();
		m_strFontID.Empty();
		m_strTipID.Empty();

		m_strToolTip.Empty();
		m_strText.Empty();
		m_bAlign = GSTA_LEFT;

		m_dwColor = 0xFF000000;
		m_dwCompID = 0;
		m_dwFlag = 0;
		m_nPosX = 0;
		m_nPosY = 0;
	};
};


struct tagCOMP
{
	DWORD m_dwCompID;

	DWORD m_dwMediaID;
	BYTE m_bUseage;

	int m_nPosX;
	int m_nPosY;

	tagCOMP()
	{
		m_dwMediaID = 0;
		m_dwCompID = 0;
		m_bUseage = 0;

		m_nPosX = 0;
		m_nPosY = 0;
	};
};

struct tagLAYER
{
	LPDIRECT3DSURFACE9 m_pLAYER;
	int m_nWidth;
	int m_nHeight;

	tagLAYER()
	{
		m_pLAYER = NULL;
		m_nWidth = 0;
		m_nHeight = 0;
	};

	~tagLAYER()
	{
		if(m_pLAYER)
		{
			m_pLAYER->Release();
			m_pLAYER = NULL;
		}

		m_nWidth = 0;
		m_nHeight = 0;
	};
};

struct tagSELECT_FORMAT
{
	CString m_strTitle;
	BYTE m_bFormat;

	tagSELECT_FORMAT( CString strTitle, BYTE bFormat )
	{
		m_strTitle = strTitle;
		m_bFormat = bFormat;
	};

	~tagSELECT_FORMAT()
	{
	};

	static SELECT_FORMAT AVAILABLE[];
	static INT COUNT;
	static BYTE DEFAULT_INDEX;
};

class CItemBase : public CObject
{
protected:
	CTachyonDoc* m_pMainDocument;

public:
	virtual CTachyonDoc* GetMainDocument();

	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);

	virtual void DeleteForRestore();
	virtual void Restore();
	virtual void Regulate();

	virtual void SetMainDocument( CTachyonDoc* pDoc);
	virtual int GetIconIndex();

	virtual void CopyItem(CItemBase* pSrcItem);

	virtual void OnDelete();

	BOOL CheckDupLocalID(
		CString strPrevID,
		CString strNewID);

public:
	static CAnimationItem* GetAnimationItem( DWORD dwID);
	static COBJTypeItem* GetOBJTypeItem( DWORD dwID);
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

	static CItemBase* CreateItem( CFile* pFile);
	static CString LoadString( CFile* pFile);

	static void DeleteForReset( BYTE bType);
	static void Reset( BYTE bType);

	static void RegulateAll();
	static void WriteString(
		CFile* pFile,
		CString& strValue);

	static CString GetNewID( CString strHead);
	static CString GetNewID( DWORD dwFormat);
	static DWORD GetID( CString strID);

	static void ReleaseLocalID(
		CString strID,
		CItemBase *pItem);

	static DWORD RegLocalID(
		CString strID,
		CItemBase *pItem);

	static DWORD RegGlobalID(
		CString strID,
		DWORD dwID);

	static BOOL CheckDupID(
		CString strPrevID,
		CString strNewID);
	static BOOL CheckID( CString strID);

	static void TextureItemUpdate(
		CTextureItem *pItem,
		CString strFileName );

	static void TextureItemUpdateByID(
		DWORD dwID,
		CString strFileName);

	static void ImageItemUpdate(
		CImageItem *pItem,
		CString strFileName );

	static void ImageItemUpdateByID(
		DWORD dwID,
		CString strFileName);

	static void MediaItemUpdate(
		CMediaItem *pItem,
		CString strFileName );

	static void MediaItemUpdateByID(
		DWORD dwID,
		CString strFileName);

	static void AnimationItemUpdate(
		CAnimationItem *pItem,
		CString strFileName );

	static void AnimationItemUpdateByID(
		DWORD dwID,
		CString strFileName);

	static void MeshItemUpdate(
		CMeshItem *pItem,
		CString strFileName );
	
	static void MeshItemUpdateByID(
		DWORD dwID,
		CString strFileName);
public:
	static MAPSTRID m_mapGlobalID;

	static MAPIDINDEX m_mapLocalID;
	static MAPGSFONT m_mapFont;
	static MAPSTR m_mapMenuID;

	static MAPITEM m_mapAnimationItem;
	static MAPITEM m_mapOBJTypeItem;
	static MAPITEM m_mapTextureItem;
	static MAPITEM m_mapImageItem;
	static MAPITEM m_mapMediaItem;
	static MAPITEM m_mapFrameItem;
	static MAPITEM m_mapCompItem;
	static MAPITEM m_mapMeshItem;
	static MAPITEM m_mapMAPItem;
	static MAPITEM m_mapOBJItem;
	static MAPITEM m_mapSFXItem;

	static DWORD m_dwAnimationID;
	static DWORD m_dwOBJTypeID;
	static DWORD m_dwTextureID;
	static DWORD m_dwImageID;
	static DWORD m_dwMediaID;
	static DWORD m_dwFrameID;
	static DWORD m_dwCompID;
	static DWORD m_dwMeshID;
	static DWORD m_dwMAPID;
	static DWORD m_dwOBJID;
	static DWORD m_dwSFXID;
	static DWORD m_dwLocalID;

	static CString m_strTempPath;
	static BYTE m_bCanTempPath;
	static BYTE m_bRegulateAll;
	static BYTE m_bMerge;

public:
	VECTORBYTE m_vDropTarget;
	VECTORSTRING m_vLocalID;

	DROPEFFECT m_nDropEffect;
	HTREEITEM m_itemPos;

	CString m_strName;
	CString m_strID;
	DWORD m_dwID;

	BYTE m_bPreviewID;
	BYTE m_bDeleted;
	BYTE m_bMenuID;
	BYTE m_bNeedID;
	BYTE m_bType;

public:
	static void GetStudioTempPath(CHAR* pPath);

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
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);
	virtual void Regulate();

public:
	LPDIRECT3DTEXTURE9 GetLPDIRECT3DTEXTURE9();
	CT3DTexture *GetT3DTEX();
	LPBYTE GetTexData();
	LPTEXTURESETSTUDIO GetTEXTURESET();

	void LoadTextureFromCFile( CFile& );
	void LoadTextureFromOrgGSF();
	void SetNullTexture();

private:
	CT3DTexture m_vDATA;
	LPBYTE m_pTexData;
	BOOL m_bTexLoaded;
	TEXTURESETSTUDIO m_TextureSet;

public:
	DWORD m_dwSize;
	ULONGLONG m_ullOffsetData;

public:
	VECTORDWORD m_vTextureID;

	COLORREF m_dwBackColor;
	DWORD m_dwTotalFrame;
	BYTE m_bTextureOption;
	BYTE m_bTextureFormat;

public:
	CTextureItem();
	virtual ~CTextureItem();
};


class CImageItem : public CItemBase
{
public:
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);
	virtual void Regulate();

public:
	CD3DImage m_Image;
	LPBYTE m_pData;
	DWORD m_dwSize;

public:
	VECTORDWORD m_vImageID;
	IMAGESET m_ImageSet;

	COLORREF m_dwBackColor;
	DWORD m_dwTotalFrame;
	BYTE m_bImageFormat;

public:
	CImageItem();
	virtual ~CImageItem();
};


class CMediaItem : public CItemBase
{
public:
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);

public:
	BYTE GetPlayerType();

public:
	TCHAR m_szFileName[MAX_PATH];

public:
	BYTE m_bMediaType;
	BYTE m_bVolume;

public:
	CMediaItem();
	virtual ~CMediaItem();
};


class CAnimationItem : public CItemBase
{
public:
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);
	virtual void Regulate();

public:
	void LoadAnimation(
		LPDIRECT3DDEVICE9 pDevice,
		LPBYTE pData);

	void Render(
		LPDIRECT3DDEVICE9 pDevice,
		LPTPIVOT pPivot,
		D3DXMATRIX& vWorld,
		FLOAT fTimeValue);

	void Render(
		LPDIRECT3DDEVICE9 pDevice,
		int nIndex);

	CAnimationItem *GetAniItem( LPANIDATA pANI);
	CTachyonAnimation& GetAnimation();
	LPBYTE GetAniData();
	void ReleaseAnimation();
	void LoadAnimationFromCFile( CFile&, BOOL bAdd );
	void LoadAnimationFromOrgGSF();
	void SetNullAnimation();
	BOOL GetAniLoaded() { return m_bAniLoaded; }

private:
	CTachyonAnimationGS m_vAnimation;
	LPBYTE m_pAniData;
	BOOL m_bAniLoaded;
	LPVBDATA m_pVB;

public:
	ANISET m_Aniset;
	DWORD m_dwSize;
	ULONGLONG m_ullOffsetData;

	VECTORSTRING m_vEventID;
	VECTORDWORD m_vAniID;
	VECTORBYTE m_vReverse;

	DWORD m_dwTotalFrame;
	CString m_strLoopID;

public:
	CAnimationItem();
	virtual ~CAnimationItem();
};


class CMeshItem : public CItemBase
{
public:
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);

public:
	WORD FindMESH(
		LPVECTORPTR pMESH,
		LPWMESHVERTEX pPOINT);

	WORD FindMESH(
		LPVECTORPTR pMESH,
		LPMESHVERTEX pPOINT);

public:
	void AddLevel( DWORD dwLevel);
	void DelLevel( DWORD dwLevel);

	void ProgressMesh(
		LPBYTE pCheck,
		DWORD dwLevel,
		FLOAT fPolyLevel);

	void RebuildMESH();
	void AutoLOD();

public:
	CTachyonMesh m_Mesh;

public:
	CMeshItem();
	virtual ~CMeshItem();
};


class COBJTypeItem : public CItemBase
{
public:
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);

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
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);
	virtual void OnDelete();
	virtual void Regulate();

public:
	void Regulate( CTachyonObject *pOBJ);
	void Regulate(
		LPMAPSFXINST pSFX,
		LPMAPSNDINST pSND);

	void DeleteTEX(
		DWORD dwMeshID,
		LPMESH pMESH,
		int nTEX);

	int GetPivotCount( LPANISET pItem);
	int GetPivotCount();

	BOOL GetSize(
		D3DXVECTOR3& vMin,
		D3DXVECTOR3& vMax);

	DWORD GetTexID(
		DWORD dwMeshID,
		int nIndex,
		int nStage);

	DWORD GetColorID(
		DWORD dwMeshID,
		int nIndex);

	DWORD GetAttributeID(
		CString strNAME);

public:
	CTachyonObject m_TOBJ;
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
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);
	virtual void Regulate();

public:
	BOOL FindSFX( DWORD dwID);

public:
	CTachyonSFX m_TSFX;
	SFX m_SFX;

	VECTORDWORD m_vSFXID;
	DWORD m_dwTotalFrame;
	DWORD m_dwSANISRC;
	DWORD m_dwANISRC;
	DWORD m_dwSFXSRC;
	DWORD m_dwCurCLK;

public:
	CSFXItem();
	virtual ~CSFXItem();
	virtual void CopyItem(CItemBase* pSrcItem);
};


class CMAPItem : public CItemBase
{
public:
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);

public:
	void SyncOBJATTR(
		COBJItem *pItem,
		LPMAPOBJ pOBJ);

public:
	CTachyonMAP *m_pMAP;

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
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);

	virtual void DeleteForRestore();
	virtual void Restore();
	virtual void Regulate();

public:
	static DWORD __ExpOBJ( DWORD_PTR lParam);
	static DWORD __ImpOBJ( DWORD_PTR lParam);
	static DWORD __ExpGEO( DWORD_PTR lParam);
	static DWORD __ImpGEO( DWORD_PTR lParam);
	static DWORD __Build( DWORD_PTR lParam);
	static DWORD __ResetATTR(DWORD_PTR lParam);
	static DWORD __AdjustOBJPOS(DWORD_PTR lParam);
	static DWORD __BuildSHADOWTEX( DWORD_PTR lParam);

public:
	void LoadOBJ(
		MAPMAPOBJ *pOBJ,
		MAPMAPSFX *pSFX,
		MAPMAPSND *pSND,
		MAPDWORD *pOBJID,
		MAPDWORD *pSFXID,
		DWORD dwIndex);

	void UpdateOBJ(
		MAPMAPOBJ *pOBJ,
		MAPMAPSFX *pSFX,
		MAPMAPSND *pSND,
		MAPDWORD *pOBJID,
		MAPDWORD *pSFXID,
		DWORD dwIndex);

	void LoadUNIT(
		LPDIRECT3DSURFACE9 *pSHADE,
		LPDIRECT3DSURFACE9 *pSKIN,
		LPDIRECT3DSURFACE9 *pTILE,
		LPVECTORSURFACE pSHADOW,
		LPBYTE pREGION,
		LPVECTORWORD pVDETAIL,
		LPBYTE pDETAIL,
		FLOAT *pHEIGHT,
		DWORD dwIndex);

	void UpdateUNIT(
		LPDIRECT3DSURFACE9 pSHADE,
		LPDIRECT3DSURFACE9 pSKIN,
		LPDIRECT3DSURFACE9 pTILE,
		LPVECTORSURFACE pSHADOW,
		LPBYTE pREGION,
		LPVECTORWORD pVDETAIL,
		LPBYTE pDETAIL,
		FLOAT *pHEIGHT,
		DWORD dwIndex);

	CTachyonObject *CreateTOBJ(
		LPMAPOBJ pOBJ,
		DWORD dwItemID);

	BYTE HasTSHADOW(
		COBJItem *pItem,
		LPMAPOBJ pOBJ);

	void Build( LPBMPARAM lParam);
	void Build(
		CString strImageFile,
		D3DXVECTOR3 vLIGHT,
		FLOAT fMinH,
		FLOAT fMaxH,
		BYTE bCell,
		BYTE bUNIT,
		DWORD dwUnit,
		DWORD dwCountX,
		DWORD dwCountY);
	void BuildOBJBUF( LPBMPARAM lParam);

	void BuildHEIGHT( LPBMPARAM lParam);
	void BuildHEIGHT(
		LPDIRECT3DSURFACE9 pIMG,
		DWORD dwIndex,
		FLOAT *pHEIGHT);

	void SaveHeight(
		int nPosX,
		int nPosZ, 
		CString strFileName );		// 맵의 Height 값 읽어들이기 -02/02/08- 박상연
	void SaveHeight( LPBMPARAM lParam );

	void RegulateHEIGHT( LPBMPARAM lParam);
	void RegulateHEIGHT(
		int nPosX,
		int nPosZ);

	void BuildSHADOWTEX( LPBMPARAM lParam);
	void BuildSHADOWTEX(
		BYTE bPixelPerTile,
		int nUnitID);

	void BuildSHADOWTEX(
		LPVECTORSURFACE pTSHADOW,
		LPVECTOROBJECT pVOBJ,
		LPD3DXVECTOR3 pLIGHT,
		BYTE bPixelPerTile,
		int nUnitID);

	void BuildSHADE( LPBMPARAM lParam);
	void BuildSHADE(
		int nPosX,
		int nPosZ);

	void RegulateSHADE( LPBMPARAM lParam);
	void RegulateSHADE(
		int nPosX,
		int nPosZ);

	void BuildSKIN( LPBMPARAM lParam);
	void BuildSKIN(
		MAPLAYER *pLAYER,
		int nPosX,
		int nPosZ);

	void RegulateSKIN( LPBMPARAM lParam);
	void RegulateSKIN(
		int nPosX,
		int nPosZ);

	void BuildLAYER( MAPLAYER *pLAYER);
	int GetLayer(
		MAPLAYER *pLAYER,
		FLOAT *pHEIGHT,
		int nPosX,
		int nPosY,
		FLOAT& fAlpha);

	void BuildTILETEX( LPBMPARAM lParam);
	void BuildTILETEX(
		int nPosX,
		int nPosZ);

	void BuildSREGION(
		LPVECTORSURFACE pSHADOW,
		LPWORD pSREGION);

	void ExpOBJ( LPBMPARAM lParam);
	void ImpOBJ( LPBMPARAM lParam);
	void ExpGEO( LPBMPARAM lParam);
	void ImpGEO( LPBMPARAM lParam);
	void ResetATTR( LPBMPARAM lParam);
	void AdjustOBJPOS( LPADOBPARAM lParam);

	void ReIndexTEX(
		LPMAPDWORD pINDEX,
		LPVECTORDWORD pVDETAIL,
		LPVECTORFLOAT pVDETAILS,
		LPVECTORDWORD pVUNIT);

	void ResetDETAIL(
		LPMAPDWORD pINDEX,
		LPVECTORWORD pVDETAIL,
		LPBYTE pDETAIL,
		DWORD dwCOUNT);

	void GetRenderTargetData(
		LPDIRECT3DSURFACE9 pTRENDER,
		LPDIRECT3DSURFACE9 pTARGET);

	void Blend(
		LPDIRECT3DSURFACE9 pTARGET,
		LPDIRECT3DSURFACE9 pIMG,
		FLOAT fPosX,
		FLOAT fPosZ);

	void Blend(
		LPDIRECT3DSURFACE9 pTARGET,
		LPDIRECT3DTEXTURE9 pIMG,
		FLOAT fPosX,
		FLOAT fPosZ);

	void RegulateOBJATTR(
		LPMAPOBJ pMAPOBJ,
		LPMAPSTR pNAME,
		COBJItem *pItem);

public:
	CTachyonHUGEMAP m_HugeMAP;
	CString m_strUNIT;
	CString m_strOBJ;

	VECTORGEOTEX m_vGEO;
	MAPCOLOR m_mapCOLOR;

	VECTORFLOAT m_vDETAILS;	// Scale Value.
	VECTORDWORD m_vDETAIL;	// 텍스쳐ID.

	FLOAT m_fMinHeight;
	FLOAT m_fMaxHeight;

	DWORD m_dwNewOBJID;
	DWORD m_dwNewSFXID;
	DWORD m_dwNewSNDID;

	CRect m_vFROM;
	CPoint m_vTO;

	std::map< DWORD, VECTORWORD > m_mapDETAIL;

public:
	CEXTMAPItem();
	virtual ~CEXTMAPItem();
};


class CBSPMAPItem : public CMAPItem
{
public:
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);
	virtual void Regulate();

public:
	static DWORD __ImpOBJ( DWORD_PTR lParam);
	static DWORD __Build( DWORD_PTR lParam);

	static DWORD CALLBACK __BuildLevel(
		LPBMPARAM lParam,
		int nLevel,
		CString strTitle);

	static DWORD CALLBACK __BuildStep(
		LPBMPARAM lParam,
		int nProcess);
	static BYTE GetNextBaseID();

public:
	void Build( CString strFileName);
	void Build( LPBMPARAM lParam);

	void ClearLIGHT();
	void ClearTEX();
	void ClearOBJ();
	void ClearSFX();
	void ClearSND();

	void ImpOBJ( LPBMPARAM lParam);
	void ImpBSP( CFile *pFile);
	void ExpOBJ( CFile *pFile);
	void ExpBSP( CFile *pFile);

	void ClearDATA();
	void LoadDATA();
	void SaveDATA();

public:
	CTachyonBSPMAP m_BSPMAP;
	VECTORMAPDWORD m_vTEX;
	CString m_strPATH;

	MAPDWORD m_mapRegionID;
	DWORD m_dwPolyCNT;
	BYTE m_bBaseID;

public:
	CBSPMAPItem();
	virtual ~CBSPMAPItem();
};


class CGuideItem : public CItemBase
{
public:
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);

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
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);
	virtual void Regulate();

public:
	BOOL FindCOMPItem( DWORD dwID);
	CSize GetSize();

	int HitTest( CPoint point);
	void Render(
		DWORD dwColor,
		BOOL bCusColor,
		int nPosX,
		int nPosY);

public:
	VECTORCOMPITEM m_vCOMPITEM;
	VECTORCOMP m_vCOMP;

	CImageItem *m_pDefault;
	CImageItem *m_pDisable;

	DWORD m_dwDefaultID;
	DWORD m_dwDisableID;
	BYTE m_bCtrlType;

	int m_nHMargine;
	int m_nVMargine;
	int m_nLevel;
	int m_nWidth;
	int m_nHeight;

public:
	CCOMPItem();
	virtual ~CCOMPItem();
};


class CFRAMEItem : public CGuideItem
{
public:
	virtual void Load( CFile* pFile);
	virtual void Save( CFile* pFile);
	virtual void Regulate();

public:
	CSize GetSize();

	int HitTest( CPoint point);
	void Render(
		int nPosX,
		int nPosY);

public:
	VECTORCOMPITEM m_vCOMP;
	VECTORCTRL m_vCTRL;

	CImageItem *m_pDefault;
	CImageItem *m_pDisable;

	CString m_strFontID;
	DWORD m_dwDefaultID;
	DWORD m_dwDisableID;

	int m_nWidth;
	int m_nHeight;

public:
	CFRAMEItem();
	virtual ~CFRAMEItem();
	virtual void CopyItem(CItemBase* pSrcItem);
};


#endif // #if (!defined ___ITEMBASE_H)
