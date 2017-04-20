#pragma once


///////////////////////////////////////////////////////////////////////////
// TExplorer macro
#define TTEXTSHADOW_COLOR				((DWORD) 0xFF000000)

#define TBASE_SCREEN_X					1024
#define TBASE_SCREEN_Y					768

#define TEXPLORER_ZERO			((FLOAT) 0.0000001f)
#define THEIGHT_ZERO			((FLOAT) 0.01f)

#define TMINIMAP_SCALE_FACTOR	((FLOAT) 1.2f)
#define TMINIMAP_SCALE_COUNT	((FLOAT) 5.0f)
#define TMINIMAP_INIT_SCALE		((FLOAT) 7.0f)
#define TWORLDMAP_MINSCALE		((FLOAT) 1.0f)
#define TWORLDMAP_SIZE			(980)

#define TWATER_ESCAPE			((FLOAT) 0.2f)
#define TTEXT_HEIGHT			((FLOAT) 5.0f)
#define TCAM_SIZE				((FLOAT) 1.4f)
#define TMINFALL_SPEED			((FLOAT) 3.4f)

#define TUNITLERP_LENGTH		((FLOAT) 64.0f)
#define TCAM_LENGTH				((FLOAT) 110.0f)
#define TPOS_SCALE				((FLOAT) 0.09f)
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

#define TMINIMAPTEX_SIZE		(192)
#define TMAINRACE_COUNT			(3)

#define TLOADING_TEXT_CLR		(0xFFFFFFFF)
#define TLOADING_IMAGE_COUNT	((BYTE) 1)
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
#define THAIR_COUNT				(7)

#define TPANTS_COUNT			(2)
#define TBODY_COUNT				(2)
#define THAND_COUNT				(2)
#define TFOOT_COUNT				(2)


///////////////////////////////////////////////////////////////////////////
// TExplorer enum define

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
	TLIGHTSET_OBJMTRL,
	TLIGHTSET_COUNT
} *LPTLIGHT_SET;

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
	TKEY_SHOW_UI,
	TKEY_GETPOS,
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
	TMINIMAPCOMP_MAX = 0,
	TMINIMAPCOMP_WORLD,
	TMINIMAPCOMP_TOGGLE_RSCS,
	TMINIMAPCOMP_ZOOM,
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

typedef enum TENVSND_TYPE
{
	TENVSND_LOOP = 0,
	TENVSND_RAND1,
	TENVSND_RAND2,
	TENVSND_COUNT
} *LPTENVSND_TYPE;

typedef enum TWORLDMAP_LEVEL
{
	TWORLDMAP_LEVEL_NONE,
	TWORLDMAP_LEVEL_UNIT,
	TWORLDMAP_LEVEL_ALL,
	TWORLDMAP_LEVEL_COUNT
} *LPTWORLDMAP_LEVEL;

///////////////////////////////////////////////////////////////////////////
// TExplorer structure type
typedef struct tagTBSPINFO				TBSPINFO, *LPTBSPINFO;

typedef struct tagTHIDEPART			THIDEPART, *LPTHIDEPART;

typedef struct tagTOBJBLOCK			TOBJBLOCK, *LPTOBJBLOCK;
typedef struct tagTSFXBLOCK			TSFXBLOCK, *LPTSFXBLOCK;
typedef struct tagTSNDBLOCK			TSNDBLOCK, *LPTSNDBLOCK;
typedef struct tagTBSPBLOCK			TBSPBLOCK, *LPTBSPBLOCK;

///////////////////////////////////////////////////////////////////////////
// TExplorer class type

class CTExplorerObjBase;
class CTExplorerMoveObj;
class CTExplorerObject;
class CTExplorerInven;
class CTExplorerItem;
class CTExplorerChar;
class CTExplorerFlag;
class CTExplorerNpc;
class CTExplorerSFX;
class CTExplorerBSP;
class CTExplorerMAP;
class CTExplorerCAM;
class CTExplorerKEY;


///////////////////////////////////////////////////////////////////////////
// TExplorer map type

typedef map< DWORD, CTExplorerFlag *>					MAPTPATHFLAG, *LPMAPTPATHFLAG;
typedef map< DWORD, CTExplorerBSP *>					MAPTBSP, *LPMAPTBSP;
typedef map< DWORD, CTachyonBSPMAP *>					MAPTBSPMAP, *LPMAPTBSPMAP;
typedef map< BYTE, CTExplorerInven *>					MAPTINVEN, *LPMAPTINVEN;
typedef map< BYTE, CTExplorerItem *>					MAPTITEM, *LPMAPTITEM;

///////////////////////////////////////////////////////////////////////////
// TExplorer vector type

typedef vector<CTExplorerObjBase *>						VTOBJBASE, *LPVTOBJBASE;
typedef vector<CTExplorerBSP *>							VTBSP, *LPVTBSP;
typedef vector<CTExplorerFlag *>						VTPATHFLAG, *LPVTPATHFLAG;
typedef vector<CD3DLight *>								VTLIGHT, *LPVTLIGHT;
typedef list<CTExplorerObjBase *>						LISTTOBJBASE, *LPLISTTOBJBASE;

typedef vector<LPTBSPINFO>								VTBSPINFO, *LPVTBSPINFO;
typedef vector<LPMAPOBJ>								VTMAPOBJ, *LPVTMAPOBJ;
typedef vector<LPMAPSFX>								VTMAPSFX, *LPVTMAPSFX;
typedef vector<LPMAPSND>								VTMAPSND, *LPVTMAPSND;
typedef vector<LPSNDIDX>								VTSNDIDX, *LPVTSNDIDX;
typedef vector<LPTNPC>									VTNPC, *LPVTNPC;
///////////////////////////////////////////////////////////////////////////
// TExplorer structure define
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

struct tagTHIDEPART
{
	BYTE m_bHideSlotID;
	BYTE m_bHidePartID;
	BYTE m_bHideRaceID;
};

struct tagTBSPINFO
{
	D3DXVECTOR3 m_vSCALE;
	D3DXVECTOR3 m_vROT;
	D3DXVECTOR3 m_vPOS;

	DWORD m_dwHouseID;
	DWORD m_dwBspID;
	BYTE m_bINDEX;

	FLOAT m_fRadius;
};
///////////////////////////////////////////////////////////////////////////
// TExplorer class define

#include "TExplorerObjBase.h"
#include "TExplorerMoveObj.h"
#include "TExplorerObject.h"
#include "TExplorerInven.h"
#include "TExplorerItem.h"
#include "TExplorerChar.h"
#include "TExplorerFlag.h"
#include "TExplorerNpc.h"
#include "TExplorerSFX.h"
#include "TExplorerBSP.h"
#include "TExplorerMAP.h"
#include "TExplorerCAM.h"
#include "TExplorerKEY.h"
#include "TWorldmapDlg.h"
#include "TMinimapDlg.h"
