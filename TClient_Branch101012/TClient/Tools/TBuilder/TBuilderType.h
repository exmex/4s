#pragma once


///////////////////////////////////////////////////////////////////////////
// TBuilder macro

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

#define TIMGBUF_MAX_BOUND					(96)
#define TIMGBUF_MAX_SIZE					(512)


///////////////////////////////////////////////////////////////////////////
// TBuilder enum define


///////////////////////////////////////////////////////////////////////////
// TBuilder structure type

typedef struct tagATTRITEM					ATTRITEM, *LPATTRITEM;
typedef struct tagTEXINDEX					TEXINDEX, *LPTEXINDEX;
typedef struct tagIDINDEX					IDINDEX, *LPIDINDEX;
typedef struct tagGSFONT					GSFONT, *LPGSFONT;
typedef struct tagGEOTEX					GEOTEX, *LPGEOTEX;
typedef struct tagCOMP						COMP, *LPCOMP;
typedef struct tagCTRL						CTRL, *LPCTRL;

typedef struct tagTPROJECT					TPROJECT, *LPTPROJECT;
typedef struct tagTIMGBUF					TIMGBUF, *LPTIMGBUF;
typedef struct tagTIMGPART					TIMGPART, *LPTIMGPART;
typedef struct tagTGSFFILE					TGSFFILE, *LPTGSFFILE;

///////////////////////////////////////////////////////////////////////////
// TBuilder class type

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


///////////////////////////////////////////////////////////////////////////
// TBuilder vector type

typedef vector<LPTPROJECT>					VECTORTPROJECT, *LPVECTORTPROJECT;
typedef vector<LPIDINDEX>					VECTORIDINDEX, *LPVECTORIDINDEX;
typedef vector<LPTIMGPART>					VECTORTIMGPART, *LPVECTORTIMGPART;
typedef vector<LPGEOTEX>					VECTORGEOTEX, *LPVECTORGEOTEX;
typedef vector<LPCOMP>						VECTORCOMP, *LPVECTORCOMP;
typedef vector<LPCTRL>						VECTORCTRL;
typedef vector< CItemBase *>				VECTORITEM, *LPVECTORITEM;
typedef vector< LPTGSFFILE >				VECTORGSFFILE, *LPVECTORGSFFILE;
typedef list< CImageItem *>					LISTIMAGE, *LPLISTIMAGE;
typedef list< CRect *>						LISTCRECT, *LPLISTCRECT;

///////////////////////////////////////////////////////////////////////////
// TBuilder map type

typedef map< CString, VECTORIDINDEX *>		MAPIDINDEX, *LPMAPIDINDEX;
typedef map< CString, DWORD>				MAPSTRID, *LPMAPSTRID;
typedef map< DWORD, LPTEXINDEX>				MAPTEXINDEX, *LPMAPTEXINDEX;
typedef map< DWORD, LPATTRITEM>				MAPATTRITEM, *LPMAPATTRITEM;
typedef map< DWORD, LPGSFONT>				MAPGSFONT, *LPMAPGSFONT;
typedef map< DWORD, CString>				MAPSTR, *LPMAPSTR;
typedef map< DWORD, LPTIMGBUF>				MAPTIMGBUF, *LPMAPTIMGBUF;
typedef map< DWORD, CItemBase *>			MAPITEM, *LPMAPITEM;


///////////////////////////////////////////////////////////////////////////
// TBuilder class define

#include "ItemBase.h"


///////////////////////////////////////////////////////////////////////////
// TBuilder structure define

struct tagTPROJECT
{
	VECTORITEM m_vTITEM;

	CString m_strNAME;
	DWORD m_dwID;

	tagTPROJECT()
	{
		m_vTITEM.clear();
	};

	~tagTPROJECT()
	{
		while(!m_vTITEM.empty())
		{
			delete m_vTITEM.back();
			m_vTITEM.pop_back();
		}
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


struct tagTIMGBUF
{
	LPDIRECT3DSURFACE9 m_pTIMG;

	LISTCRECT m_vTGARBAGE;
	LISTCRECT m_vTRECT;

	DWORD m_dwID;

	tagTIMGBUF()
	{
		m_vTGARBAGE.clear();
		m_vTRECT.clear();

		m_pTIMG = NULL;
		m_dwID = 0;
	};

	~tagTIMGBUF()
	{
		Release();
	};

	void Release()
	{
		while(!m_vTGARBAGE.empty())
		{
			delete m_vTGARBAGE.back();
			m_vTGARBAGE.pop_back();
		}

		while(!m_vTRECT.empty())
		{
			delete m_vTRECT.back();
			m_vTRECT.pop_back();
		}

		if(m_pTIMG)
		{
			m_pTIMG->Release();
			m_pTIMG = NULL;
		}

		m_dwID = 0;
	};
};

struct tagTIMGPART
{
	TVERTEX m_vTMESH[4];
	DWORD m_dwImageID;

	tagTIMGPART()
	{
		m_dwImageID = 0;
	};
};

struct tagTGSFFILE
{
	INT m_nIndex;
	CString m_strPath;
	CFile m_File;
	BOOL	m_bOptimize;
	DWORD m_dwTSEED;
};
