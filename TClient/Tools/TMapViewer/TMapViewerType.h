#pragma once


///////////////////////////////////////////////////////////////////////////
// TMapViewer macro

#define TMapViewer_ZERO			((FLOAT) 0.0000001f)
#define THEIGHT_ZERO			((FLOAT) 0.01f)

#define TMINIMAP_SCALE_FACTOR	((FLOAT) 1.2f)
#define TMINIMAP_SCALE_COUNT	((FLOAT) 5.0f)
#define TMINIMAP_INIT_SCALE		((FLOAT) 5.0f)
#define TWORLDMAP_MINSCALE		((FLOAT) 1.0f)
#define TWORLDMAP_SIZE			(980)

#define TWATER_ESCAPE			((FLOAT) 0.2f)
#define TTEXT_HEIGHT			((FLOAT) 5.0f)
#define TCAM_SIZE				((FLOAT) 1.4f)
#define TMINFALL_SPEED			((FLOAT) 3.4f)

#define TUNITLERP_LENGTH		((FLOAT) 64.0f)
#define TCAM_LENGTH				((FLOAT) 110.0f)
#define TPOS_SCALE				((FLOAT) 0.05f)
#define TLIGHT_SCALE			((FLOAT) 0.64f)
#define TSKY_SCALE				((FLOAT) 0.9f)

#define TPATHFLAG_NULL			((DWORD) 0x00000000)
#define TMAPID_NULL				((WORD) 0xFFFF)

#define TPITCH_MAX				(1575)
#define TPITCH_MIN				(300)

#define THIDECHAR_SPEED			((FLOAT) 0.01f)
#define TCAMOUT_SPEED			((FLOAT) 0.05f)

#define TOBJSHADOW_BIAS			((FLOAT) 0.0000002f)
#define TOBJSELECT_BIAS			((FLOAT) 0.0000001f)
#define TOBJSHADOW_SCALE		((FLOAT) 1.8f)

#define OM_TSUBACTENDING		(0x10000002)
#define OM_TSUBACTENDED			(0x10000003)

#define TOBJCELL_LENGTH			((FLOAT) 8.0f)
#define TOBJCELL_WIDTH			(128)
#define TOBJCELL_COUNT			(16384)

#define TDRAWCELL_COUNT			(361)
#define TDRAWCELL_WIDTH			(19)

#define DEF_GLOW_RANGE			(32.0f)
#define DEF_INDEXCNT			(36000)
#define DEF_LODBOUND			(58)
#define DEF_DIFFUSECNT			(10)
#define DEF_MAXLEVEL			(10)
#define DEF_MAXLENGTH			(4)

#define TWATERSTEP_SND			(10009)
#define TDEFSTEP_SND			(10000)

#define TMINIMAPTEX_SIZE		(256)
#define TMAINRACE_COUNT			(3)

#define TLOADING_TEXT_CLR		(0xFFFFFFFF)
#define TLOADING_IMAGE_COUNT	((BYTE) 4)
#define TLOADING_GAUGE_X		(47)
#define TLOADING_GAUGE_Y		(701)

#define FOLLOW_RANGE			(50)
#define DIR_RANGE				(25)

#define TCHAT_MARGINE			(10)
#define TCHAT_SIZE				(50)

#define TTICK_INTERVAL			(50)
#define CLICK_DELAY				(180)
#define SYNC_TICK				(1000)

#define ALPHA_MAX				(255)
#define ALPHA_MIN				(0)

#define TMAXSHADOW_TICK			(200)
#define TMAXALPHA_TICK			(800)
#define TMAXPASS_TICK			(200)
#define TMAXSKY_TICK			(1000)

#define TINV_SIN60				((FLOAT) 1.1548f)
#define TCAMFIX_DIST			((FLOAT) 7.0f)
#define TALPHA_DIST				((FLOAT) 3.0f)
#define TFOG_LENGTH				((FLOAT) 64.0f)
#define TCAM_MARGINE			((FLOAT) 0.2f)
#define TPATH_HEIGHT			((FLOAT) 0.3f)
#define TMOVE_RADIUS			((FLOAT) 1.1f)
#define TCOLLISION_HEIGHT		((FLOAT) 2.0f)
#define TCOLLISION_SIZE			((FLOAT) 1.4f)
#define THIT_RANGE				((FLOAT) 12.0f)

#define TLOWCULL_BOUND			((FLOAT) 1.0f)
#define TMIDCULL_BOUND			((FLOAT) 2.0f)
#define THICULL_BOUND			((FLOAT) 4.0f)

#define TLOWCULL_FACTOR			((FLOAT) 0.3f)
#define TMIDCULL_FACTOR			((FLOAT) 0.5f)
#define THICULL_FACTOR			((FLOAT) 0.7f)

#define TBGM_VOLUME				(5000)
#define TBGM_TIMER				(5000)
#define TBGM_RATE				(3)

#define TREPLACE_COUNT			(7)
#define TCLEAR_COUNT			(5)
#define TWEAPON_COUNT			(3)

#define TFACE_COUNT				(4)
#define THAIR_COUNT				(4)

#define TPANTS_COUNT			(2)
#define TBODY_COUNT				(2)
#define THAND_COUNT				(2)
#define TFOOT_COUNT				(2)


///////////////////////////////////////////////////////////////////////////
// TMapViewer enum define

typedef enum TCOLLISION_TYPE
{
	TCOLLISION_NONE = 0,
	TCOLLISION_BOX,
	TCOLLISION_CYLINDER,
	TCOLLISION_POLY,
	TCOLLISION_COUNT
} *LPTCOLLISION_TYPE;

typedef enum TLIGHT_SET
{
	TLIGHTSET_MAIN = 0,
	TLIGHTSET_MAPCOLOR,
	TLIGHTSET_SHADOW,
	TLIGHTSET_MTRL,
	TLIGHTSET_COUNT
} *LPTLIGHT_SET;

typedef enum TLIGHT_POS
{
	TLIGHT_CENTER = 0,
	TLIGHT_SIDE,
	TLIGHT_BACK,
	TLIGHT_COUNT
} *LPTLIGHT_POS;

typedef enum TKEY_SET
{
	TKEY_NONE = 0,
	TKEY_FORWARD,
	TKEY_BACK,
	TKEY_LEFT,
	TKEY_RIGHT,
	TKEY_JUMP,
	TKEY_AUTORUN,
	TKEY_RUN,
	TKEY_ROT,			// Do not customize - only mouse rbutton command
	TKEY_CAM_LEFT,
	TKEY_CAM_RIGHT,
	TKEY_CAM_UP,
	TKEY_CAM_DOWN,
	TKEY_CAM_ROT,		// Do not customize - only mouse lbutton command
	TKEY_ZOOM_IN,
	TKEY_ZOOM_OUT,
	TKEY_ADDFLAG,
	TKEY_FINDPATH,
	TKEY_CHAT,
	TKEY_COUNT
} *LPTKEY_SET;

typedef enum TCOLLISION_RESULT
{
	TCR_NONE = 0,
	TCR_CORRECT,
	TCR_BACK,
	TCR_COUNT
} *LPTCOLLISION_RESULT;

typedef enum TMAINMOVE_TYPE
{
	TMAINMOVE_NORMAL = 0,
	TMAINMOVE_CLICK,
	TMAINMOVE_COUNT
} *LPTMAINMOVE_TYPE;

typedef enum TMINIMAP_COMP
{
	TMINIMAPCOMP_MIN = 0,
	TMINIMAPCOMP_MAX,
	TMINIMAPCOMP_MIN_BTN,
	TMINIMAPCOMP_MAX_BTN,
	TMINIMAPCOMP_WORLD,
	TMINIMAPCOMP_CMD,
	TMINIMAPCOMP_ZOOMIN,
	TMINIMAPCOMP_ZOOMOUT,
	TMINIMAPCOMP_COUNT
} *LPTMINIMAP_COMP;

typedef enum TEQUIP_MODE
{
	TEM_NORMAL = 0,
	TEM_WEAPON,
	TEM_LONG,
	TEM_COUNT
} *LPTEQUIP_MODE;

typedef enum TSKYOBJ_TYPE
{
	TSKYOBJ_FAR = 0,
	TSKYOBJ_SKY,
	TSKYOBJ_COUNT
} *LPTSKYOBJ_TYPE;


///////////////////////////////////////////////////////////////////////////
// TMapViewer structure type

typedef struct tagTNPCTEMP			TNPCTEMP, *LPTNPCTEMP;
typedef struct tagTSFXTEMP			TSFXTEMP, *LPTSFXTEMP;

typedef struct tagTSKYBOX			TSKYBOX, *LPTSKYBOX;
typedef struct tagTLIGHT			TLIGHT, *LPTLIGHT;
typedef struct tagTUNITFOG			TUNITFOG, *LPTUNITFOG;

typedef struct tagTACTIONDATA		TACTIONDATA, *LPTACTIONDATA;
typedef struct tagTHIDEPART			THIDEPART, *LPTHIDEPART;
typedef struct tagTACTION			TACTION, *LPTACTION;
typedef struct tagTCLOTH			TCLOTH, *LPTCLOTH;
typedef struct tagTITEM				TITEM, *LPTITEM;
typedef struct tagTFOG				TFOG, *LPTFOG;
typedef struct tagTNPC				TNPC, *LPTNPC;

typedef struct tagTOBJBLOCK			TOBJBLOCK, *LPTOBJBLOCK;
typedef struct tagTSFXBLOCK			TSFXBLOCK, *LPTSFXBLOCK;
typedef struct tagTSNDBLOCK			TSNDBLOCK, *LPTSNDBLOCK;
typedef struct tagTBSPBLOCK			TBSPBLOCK, *LPTBSPBLOCK;

typedef struct tagTREGIONINFO		TREGIONINFO, *LPTREGIONINFO;
typedef struct tagTMAPINFO			TMAPINFO, *LPTMAPINFO;
typedef struct tagTMINIMAP			TMINIMAP, *LPTMINIMAP;


///////////////////////////////////////////////////////////////////////////
// TMapViewer class type

class CTMapViewerObjBase;
class CTMapViewerMoveObj;
class CTMapViewerObject;
class CTMapViewerInven;
class CTMapViewerItem;
class CTMapViewerChar;
class CTMapViewerFlag;
class CTMapViewerNpc;
class CTMapViewerSFX;
class CTMapViewerBSP;
class CTMapViewerMAP;
class CTMapViewerCAM;
class CTMapViewerKEY;


///////////////////////////////////////////////////////////////////////////
// TMapViewer map type

typedef map< DWORD, CTMapViewerFlag *>					MAPTPATHFLAG, *LPMAPTPATHFLAG;
typedef map< DWORD, CTMapViewerBSP *>					MAPTBSP, *LPMAPTBSP;

typedef map< BYTE, CTMapViewerInven *>					MAPTINVEN, *LPMAPTINVEN;
typedef map< BYTE, CTMapViewerItem *>					MAPTITEM, *LPMAPTITEM;

typedef map< DWORD, LPD3DXVECTOR3>						MAPD3DXVECTOR3, *LPMAPD3DXVECTOR3;
typedef map< DWORD, LPTREGIONINFO>						MAPTREGIONINFO, *LPMAPTREGIONINFO;
typedef map< DWORD, LPTMINIMAP>							MAPTMINIMAP, *LPMAPTMINIMAP;
typedef map< DWORD, LPTMAPINFO>							MAPTMAPINFO, *LPMAPTMAPINFO;
typedef map< DWORD, LPTACTION>							MAPTACTION, *LPMAPTACTION;

typedef map< DWORD, LPTUNITFOG>							MAPTUNITFOG, *LPMAPTUNITFOG;
typedef map< DWORD, LPTSKYBOX>							MAPTSKYBOX, *LPMAPTSKYBOX;
typedef map< DWORD, LPTLIGHT>							MAPTLIGHT, *LPMAPTLIGHT;

typedef map< DWORD, LPTSFXTEMP>							MAPTSFXTEMP, *LPMAPTSFXTEMP;
typedef map< WORD, LPTNPCTEMP>							MAPNPCTEMP, *LPMAPNPCTEMP;
typedef map< WORD, LPTITEM>								MAPTITEMTEMP, *LPMAPTITEMTEMP;
typedef map< WORD, WORD>								MAPWORD, *LPMAPWORD;


///////////////////////////////////////////////////////////////////////////
// TMapViewer vector type

typedef vector<CTMapViewerObjBase *>						VTOBJBASE, *LPVTOBJBASE;
typedef vector<CTMapViewerBSP *>							VTBSP, *LPVTBSP;
typedef vector<CTMapViewerFlag *>						VTPATHFLAG, *LPVTPATHFLAG;
typedef vector<CD3DLight *>								VTLIGHT, *LPVTLIGHT;
typedef list<CTMapViewerObjBase *>						LISTTOBJBASE, *LPLISTTOBJBASE;

typedef vector<LPTACTIONDATA>							VTACTIONDATA, *LPVTACTIONDATA;
typedef vector<LPMAPOBJ>								VTMAPOBJ, *LPVTMAPOBJ;
typedef vector<LPMAPSFX>								VTMAPSFX, *LPVTMAPSFX;
typedef vector<LPMAPSND>								VTMAPSND, *LPVTMAPSND;
typedef vector<LPSNDIDX>								VTSNDIDX, *LPVTSNDIDX;
typedef vector<LPTNPC>									VTNPC, *LPVTNPC;
typedef vector<LPTFOG>									VTFOG, *LPVTFOG;


///////////////////////////////////////////////////////////////////////////
// TMapViewer structure define

struct tagTSKYBOX
{
	DWORD m_dwOBJ;
	DWORD m_dwCLK;
	DWORD m_dwCL;
	DWORD m_dwMESH;
	DWORD m_dwACT;
	DWORD m_dwANI;
};

struct tagTLIGHT
{
	D3DXVECTOR3 m_vDIR;

	FLOAT m_fAmbientR;
	FLOAT m_fAmbientG;
	FLOAT m_fAmbientB;

	FLOAT m_fDiffuseR;
	FLOAT m_fDiffuseG;
	FLOAT m_fDiffuseB;
};

struct tagTOBJBLOCK
{
	VTOBJBASE m_vTOBJ;
	WORD m_wCount;

	tagTOBJBLOCK()
	{
		m_vTOBJ.clear();
		m_wCount = 0;
	};
};

struct tagTSFXBLOCK
{
	VECTORTSFX m_vTSFX;
	WORD m_wCount;

	tagTSFXBLOCK()
	{
		m_vTSFX.clear();
		m_wCount = 0;
	};
};

struct tagTSNDBLOCK
{
	VTSNDIDX m_vTSND;
	WORD m_wCount;

	tagTSNDBLOCK()
	{
		m_vTSND.clear();
		m_wCount = 0;
	};
};

struct tagTBSPBLOCK
{
	VTBSP m_vTBSP;
	WORD m_wCount;

	tagTBSPBLOCK()
	{
		m_vTBSP.clear();
		m_wCount = 0;
	};
};

struct tagTSFXTEMP
{
	DWORD m_dwID;

	DWORD m_dwPosID;
	LPSFX m_pTSFX;

	tagTSFXTEMP()
	{
		m_pTSFX = NULL;

		m_dwPosID = 0;
		m_dwID = 0;
	};
};

struct tagTREGIONINFO
{
	VECTORDWORD m_vTBGM;
	CString m_strNAME;
	DWORD m_dwID;

	D3DXVECTOR3 m_vTHUMB;
	BYTE m_bContryID;
	BYTE m_bCanFly;
	WORD m_wLocalID;

	tagTREGIONINFO()
	{
		m_vTBGM.clear();
	};

	~tagTREGIONINFO()
	{
		m_vTBGM.clear();
	};
};

struct tagTMAPINFO
{
	MAPD3DXVECTOR3 m_mapTNODE;
	DWORD m_dwMapID;

	tagTMAPINFO()
	{
		m_mapTNODE.clear();
		m_dwMapID = 0;
	};

	~tagTMAPINFO()
	{
		MAPD3DXVECTOR3::iterator itTNODE;

		for( itTNODE = m_mapTNODE.begin(); itTNODE != m_mapTNODE.end(); itTNODE++)
			delete (*itTNODE).second;

		m_mapTNODE.clear();
		m_dwMapID = 0;
	};
};

struct tagTMINIMAP
{
	LPDIRECT3DTEXTURE9 m_pTEX;

	FLOAT m_fSCALE;
	FLOAT m_fPosX;
	FLOAT m_fPosZ;

	BYTE m_bWorldID;

	tagTMINIMAP()
	{
		m_pTEX = NULL;

		m_fSCALE = 1.0f;
		m_fPosX = 0.0f;
		m_fPosZ = 0.0f;

		m_bWorldID = 0;
	};

	~tagTMINIMAP()
	{
		m_pTEX = NULL;

		m_fSCALE = 1.0f;
		m_fPosX = 0.0f;
		m_fPosZ = 0.0f;

		m_bWorldID = 0;
	};
};

struct tagTNPCTEMP
{
	WORD m_wID;

	LPTITEM m_pTITEM[ES_COUNT];
	BYTE m_bActionID;

	DWORD m_dwOBJ;
	DWORD m_dwSND;
};

struct tagTACTIONDATA
{
	BYTE m_bLoopAction;
	BYTE m_bSubAction;
	BYTE m_bLevel;

	BYTE m_bContinue;
	BYTE m_bCancel;
	BYTE m_bNavAction;
	BYTE m_bSkipMain;
};

struct tagTACTION
{
	DWORD m_dwTSLASH;
	DWORD m_dwActID;
	DWORD m_dwAniID;

	BYTE m_bEquipMode;
};

struct tagTHIDEPART
{
	BYTE m_bHideSlotID;
	BYTE m_bHidePartID;
	BYTE m_bHideRaceID;
};

struct tagTCLOTH
{
	BYTE m_bHideSlotID;
	BYTE m_bHidePartID;
	BYTE m_bHideRaceID;

	DWORD m_dwCLK;
	DWORD m_dwCL;
	DWORD m_dwMESH;
};

struct tagTFOG
{
	BYTE m_bType;

	BYTE m_bRValue;
	BYTE m_bGValue;
	BYTE m_bBValue;

	FLOAT m_fRADIUS;
	FLOAT m_fRANGE;
	FLOAT m_fPosX;
	FLOAT m_fPosZ;

	FLOAT m_fDENSITY;
	FLOAT m_fSTART;
	FLOAT m_fEND;
};

struct tagTUNITFOG
{
	LPTFOG m_pTGLOBAL;
	VTFOG m_vTLOCAL;

	tagTUNITFOG()
	{
		m_vTLOCAL.clear();
		m_pTGLOBAL = NULL;
	};

	~tagTUNITFOG()
	{
		while(!m_vTLOCAL.empty())
		{
			delete m_vTLOCAL.back();
			m_vTLOCAL.pop_back();
		}

		if(m_pTGLOBAL)
		{
			delete m_pTGLOBAL;
			m_pTGLOBAL = NULL;
		}
	};
};

struct tagTNPC
{
	LPTNPCTEMP m_pTNPC;
	DWORD m_dwID;

	CString m_strTITLE;
	CString m_strNAME;

	BYTE m_bCollisionType;
	BYTE m_bCountryID;
	BYTE m_bNPCType;
	BYTE m_bLand;
	BYTE m_bMode;

	FLOAT m_fSizeX;
	FLOAT m_fSizeY;
	FLOAT m_fSizeZ;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	FLOAT m_fDIR;

	tagTNPC()
	{
		m_pTNPC = NULL;
		m_dwID = 0;

		m_strTITLE.Empty();
		m_strNAME.Empty();

		m_bCollisionType = 0;
		m_bNPCType = 0;
		m_bLand = FALSE;

		m_bCountryID = TCONTRY_N;
		m_bMode = MT_NORMAL;

		m_fSizeX = 0.0f;
		m_fSizeY = 0.0f;
		m_fSizeZ = 0.0f;
		m_fPosX = 0.0f;
		m_fPosY = 0.0f;
		m_fPosZ = 0.0f;
		m_fDIR = 0.0f;
	};
};

struct tagTITEM
{
	CString m_strNAME;
	DWORD m_dwSlotID;
	WORD m_wItemID;
	WORD m_wImageID;

	BYTE m_bHideSlotID;
	BYTE m_bHidePartID;
	BYTE m_bHideRaceID;
	BYTE m_bPrmSlotID;
	BYTE m_bSubSlotID;
	BYTE m_bWeaponID;

	DWORD m_dwTSFXColor;
	DWORD m_dwTSFXTexID;
	DWORD m_dwOBJ;
	DWORD m_dwCLK;
	DWORD m_dwCL;

	DWORD m_dwPivot[MT_COUNT];
	DWORD m_dwMESH[MT_COUNT];

	FLOAT m_fPosX[MT_COUNT];
	FLOAT m_fPosY[MT_COUNT];
	FLOAT m_fPosZ[MT_COUNT];
	FLOAT m_fLength;
};


///////////////////////////////////////////////////////////////////////////
// TMapViewer class define

#include "TMapViewerObjBase.h"
#include "TMapViewerMoveObj.h"
#include "TMapViewerObject.h"
#include "TMapViewerInven.h"
#include "TMapViewerItem.h"
#include "TMapViewerChar.h"
#include "TMapViewerFlag.h"
#include "TMapViewerNpc.h"
#include "TMapViewerSFX.h"
#include "TMapViewerBSP.h"
#include "TMapViewerMAP.h"
#include "TMapViewerCAM.h"
#include "TMapViewerKEY.h"
#include "TWorldmapDlg.h"
#include "TMinimapDlg.h"
