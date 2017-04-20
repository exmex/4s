// T3D.h: interface for the Tachyon 3D Library
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined ___T3D_H
#define ___T3D_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define __T3D_SDK_VER			3001
#include <mmsystem.h>
#include <Iphlpapi.h>
#include <Wbemidl.h>
#include <dmusicc.h>
#include <dmusici.h>
#include <gdiplus.h>
#include <dsound.h>
#include <ddraw.h>
#include <dshow.h>
#include <d3dx9.h>


#pragma warning( disable : 4786 4530 4018 4995)
#pragma comment( lib, "Wbemuuid.lib")
#pragma comment( lib, "Iphlpapi.lib")
#pragma comment( lib, "gdiplus")

#include <vector>
#include <queue>
#include <map>
#include <set>
#include <stack>
#include <deque>

using namespace Gdiplus;
using namespace std;

#define MODIFY_LOADING
//#define HEBA

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Vertex Format

#define T3DFVF_WMESHVERTEX											(D3DFVF_XYZB4|D3DFVF_LASTBETA_UBYTE4|D3DFVF_NORMAL|D3DFVF_TEX1)
#define T3DFVF_WLVERTEX												(D3DFVF_XYZB4|D3DFVF_LASTBETA_UBYTE4|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define T3DFVF_WPVERTEX												(D3DFVF_XYZB4|D3DFVF_LASTBETA_UBYTE4|D3DFVF_TEX1)

#define T3DFVF_MESHVERTEX											(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2)
#define T3DFVF_TNLVERTEX											(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define T3DFVF_TVERTEX												(D3DFVF_XYZRHW|D3DFVF_TEX1)
#define T3DFVF_LVERTEX												(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define T3DFVF_PVERTEX												(D3DFVF_XYZ|D3DFVF_TEX1)
#define T3DFVF_TNLPOINT												(D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
#define T3DFVF_TPOINT												(D3DFVF_XYZRHW)
#define T3DFVF_LPOINT												(D3DFVF_XYZ|D3DFVF_DIFFUSE)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Constant

#define T3DLIGHT_RANGE_MAX											(1.8446743e+019F)

#define WM_TCOMMAND													(WM_USER + 0x1000)
#define WM_TOBJMSG													(WM_USER + 0x2000)
#define WM_SESSION_MSG												(WM_USER + 0x3000)

#define OM_ACTENDING												(0x10000000)
#define OM_ACTENDED													(0x10000001)

#define EXTMAP_CAMEPSILON											((FLOAT) 3.0f)
#define TMIN_HEIGHT													((FLOAT) -1000.0f)
#define EXTBIAS_BASE												((FLOAT) 0.000001f)
#define BSPBIAS_BASE												((FLOAT) 0.00001f)
#define DEF_EPSILON													((FLOAT) 0.005f)
#define DEF_MIPFACTOR												((FLOAT) -1.0f)
#define TIME_RATIO													((FLOAT) 4.8f)

#define TGDITEXTCLR_MAX												15
#define TGDITEXTCLR_MIN												8

#define TGDIBUFFER_SIZE												4096
#define TGDICOLOR_OFFSET											3

#define DEF_BLENDTICK												120
#define MAX_PIVOT													255
#define WEIGHT_COUNT												3
#define MP_COUNT													4

#define MP_LEFT														((BYTE) 0x00)
#define MP_TOP														((BYTE) 0x01)
#define MP_RIGHT													((BYTE) 0x02)
#define MP_BOTTOM													((BYTE) 0x03)

#define DXT1														((BYTE) 0x00)
#define DXT2														((BYTE) 0x01)
#define DXT3														((BYTE) 0x02)
#define DXT4														((BYTE) 0x03)
#define DXT5														((BYTE) 0x04)
#define NON_COMP													((BYTE) 0x05)

#define ATTRTYPE_INT												((BYTE) 0x00)
#define ATTRTYPE_FLOAT												((BYTE) 0x01)
#define ATTRTYPE_STRING												((BYTE) 0x02)

#define VOLUME_MAX													((BYTE) 100)

#define MEDIA_TSHOW													((BYTE) 0x00)
#define MEDIA_TSOUND												((BYTE) 0x01)
#define MEDIA_TMUSIC												((BYTE) 0x02)

#define MEDIATYPE_WAV												((BYTE) 0x00)
#define MEDIATYPE_MPA												((BYTE) 0x01)
#define MEDIATYPE_MP2												((BYTE) 0x02)
#define MEDIATYPE_MP3												((BYTE) 0x03)
#define MEDIATYPE_AU												((BYTE) 0x04)
#define MEDIATYPE_AIF												((BYTE) 0x05)
#define MEDIATYPE_AIFF												((BYTE) 0x06)
#define MEDIATYPE_SND												((BYTE) 0x07)
#define MEDIATYPE_MID												((BYTE) 0x08)
#define MEDIATYPE_MIDI												((BYTE) 0x09)
#define MEDIATYPE_RMI												((BYTE) 0x10)
#define MEDIATYPE_AVI												((BYTE) 0x11)
#define MEDIATYPE_QT												((BYTE) 0x12)
#define MEDIATYPE_MOV												((BYTE) 0x13)
#define MEDIATYPE_MPG												((BYTE) 0x14)
#define MEDIATYPE_MPEG												((BYTE) 0x15)
#define MEDIATYPE_M1V												((BYTE) 0x16)
#define MEDIATYPE_DAT												((BYTE) 0x17)

#define TT_TEX														((BYTE) 0x00)
#define TT_COLOR													((BYTE) 0x01)
#define TT_ENV														((BYTE) 0x02)

#define MT_EXT														((BYTE) 0x00)
#define MT_BSP														((BYTE) 0x01)

#define SFX_SINGLE													((BYTE) 0x00)
#define SFX_PARTICLE												((BYTE) 0x01)

#define SFXSRC_TEX													((BYTE) 0x00)
#define SFXSRC_OBJ													((BYTE) 0x01)
#define SFXSRC_SFX													((BYTE) 0x02)

#define SPRAY_BOX													((BYTE) 0x00)
#define SPRAY_SPHERE												((BYTE) 0x01)
#define SPRAY_CYLINDER												((BYTE) 0x02)

#define INVALID_INIT_TICK											((DWORD) 0xFFFFFFFF)
#define NODEID_NULL													((DWORD) 0xFFFFFFFF)

#define PORTALDIR_FRONT												((BYTE) 0x00)
#define PORTALDIR_BACK												((BYTE) 0x01)

#define POINTPOS_ONPLANE											((BYTE) 0x00)
#define POINTPOS_FRONT												((BYTE) 0x01)
#define POINTPOS_BACK												((BYTE) 0x02)

#define WINDINGPOS_NONE												((BYTE) 0x00)
#define WINDINGPOS_FRONT											((BYTE) 0x01)
#define WINDINGPOS_ONFRONT											((BYTE) 0x02)
#define WINDINGPOS_BACK												((BYTE) 0x03)
#define WINDINGPOS_ONBACK											((BYTE) 0x04)

#define MLT_DIRECTION												((BYTE) 0x00)
#define MLT_POINT													((BYTE) 0x01)

#define SHADOWTEX_SIZE												(512)
#define DEFAULT_PPT													(4)

#define TILE_NULL													((BYTE) 0xFF)
#define BLANK_REGION												((BYTE) 0xFF)

#define MESHTYPE_NONE												((BYTE) 0x00)
#define MESHTYPE_MESH												((BYTE) 0x01)
#define MESHTYPE_WMESH												((BYTE) 0x02)

#define VBTYPE_GLOBAL												((BYTE) 0x00)
#define VBTYPE_LOCAL												((BYTE) 0x01)

#define MPEG_GLOBAL_GAIN_SCALE										(4 * 15)
#define MPEG_BUFFER_COUNT											(2)

#define VBR_FRAMES_FLAG												(0x0001)
#define VBR_BYTES_FLAG												(0x0002)
#define VBR_TOC_FLAG												(0x0004)
#define VBR_SCALE_FLAG												(0x0008)

#define DEF_NBUF													(8192)
#define IS_MAX														(32)
#define DEF_BUF_TRIGGER												(DEF_NBUF - 1500)

#define TMAX_CHECK_CODE_SIZE										((BYTE) 7)
#define TESSPROC_COUNT												(2)

#define TDBB_UPDATE_TICK											(200)
#define TDBB_SIZE_COUNT												(2)
#define TDBB_MAX_PIXEL												(64)

#define TINVALID_INDEX												((DWORD) 0xFFFFFFFF)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macro
#define SAFE_DELETE(ptr)			if(ptr) {delete ptr; ptr=NULL;}
#define SAFE_DELETE_ARRAY(ptr)		if(ptr) {delete [] ptr; ptr=NULL;}
#define SAFE_RELEASE(ptr)			if(ptr) {ptr->Release(); ptr=NULL;}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Enum

typedef enum T3DRES_TYPE
{
	T3DRES_TEX = 0,
	T3DRES_IMGBUF,
	T3DRES_IMG,
	T3DRES_MEDIA,
	T3DRES_ANI,
	T3DRES_MESH,
	T3DRES_OBJ,
	T3DRES_SFX,
	T3DRES_MAP,
	T3DRES_COUNT
} *LPT3DRES_TYPE;

typedef enum __MM_STATE
{
	MMS_CLOSED = 0,
	MMS_OPEN,
	MMS_PLAY,
	MMS_PAUSE,
	MMS_COUNT
} MM_STATE, *LPMM_STATE;

typedef enum __HUFF_CASE
{
	HUFFCASE_NOBIT = 0,
	HUFFCASE_ONE,
	HUFFCASE_NOLIN,
	HUFFCASE_LIN,
	HUFFCASE_QUADA,
	HUFFCASE_QUADB,
	HUFFCASE_COUNT
} HUFF_CASE, *LPHUFF_CASE;

typedef enum TDBB_LEVEL
{
	TDBB_LEVEL_HI = 0,
	TDBB_LEVEL_MID,
	TDBB_LEVEL_LOW,
	TDBB_LEVEL_COUNT
} *LPTDBB_LEVEL;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Union

typedef union __HUFF_ELEMENT
{
	int m_nDATA;
	struct
	{
		BYTE m_bSIGN;
		BYTE m_bX;
		BYTE m_bY;
		BYTE m_bPURGE;
	} m_vElement;
} HUFF_ELEMENT, *LPHUFF_ELEMENT;

typedef union __SAMPLE
{
	FLOAT m_fF;
	int m_nI;
} SAMPLE, *LPSAMPLE;

typedef union __SAMPLEBUF
{
	SAMPLE m_vA[2][2][576];
	SAMPLE m_vB[2304];
} SAMPLEBUF, *LPSAMPLEBUF;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Structure

typedef struct tagTMESHBUFFER										TMESHBUFFER, *LPTMESHBUFFER;

typedef struct tagWMESHVERTEX										WMESHVERTEX, *LPWMESHVERTEX;
typedef struct tagWLVERTEX											WLVERTEX, *LPWLVERTEX;
typedef struct tagWPVERTEX											WPVERTEX, *LPWPVERTEX;

typedef struct tagMESHVERTEX										MESHVERTEX, *LPMESHVERTEX;
typedef struct tagTNLVERTEX											TNLVERTEX, *LPTNLVERTEX;
typedef struct tagTVERTEX											TVERTEX, *LPTVERTEX;
typedef struct tagLVERTEX											LVERTEX, *LPLVERTEX;
typedef struct tagPVERTEX											PVERTEX, *LPPVERTEX;

typedef struct tagTNLPOINT											TNLPOINT, *LPTNLPOINT;
typedef struct tagTPOINT											TPOINT, *LPTPOINT;
typedef struct tagLPOINT											LPOINT, *LPLPOINT;

typedef struct tagINDEXEDMESH										INDEXEDMESH, *LPINDEXEDMESH;
typedef struct tagIBINDEXINFO										IBINDEXINFO, *LPIBINDEXINFO;
typedef struct tagIBINDEX											IBINDEX, *LPIBINDEX;

typedef struct tagTVERTEXINDEX										TVERTEXINDEX, *LPTVERTEXINDEX;
typedef struct tagWEIGHTBUF											WEIGHTBUF, *LPWEIGHTBUF;
typedef struct tagT3DDATA											T3DDATA, *LPT3DDATA;
typedef struct tagANIDATA											ANIDATA, *LPANIDATA;
typedef struct tagFILEVB											FILEVB, *LPFILEVB;
typedef struct tagMESHVB											MESHVB, *LPMESHVB;
typedef struct tagVBDATA											VBDATA, *LPVBDATA;
typedef struct tagIBDATA											IBDATA, *LPIBDATA;
typedef struct tagIBINFO											IBINFO, *LPIBINFO;

typedef struct tagBONESANIMATION									BONESANIMATION, *LPBONESANIMATION;
typedef struct tagBONESNODE											BONESNODE, *LPBONESNODE;

typedef struct tagTPOINTKEY											TPOINTKEY, *LPTPOINTKEY;
typedef struct tagTROTKEY											TROTKEY, *LPTROTKEY;
typedef struct tagTSCALEKEY											TSCALEKEY, *LPTSCALEKEY;
typedef struct tagAFFINEKEY											AFFINEKEY, *LPAFFINEKEY;
typedef struct tagANIKEY											ANIKEY, *LPANIKEY;
typedef struct tagCLRKEY											CLRKEY, *LPCLRKEY;
typedef struct tagUVKEY												UVKEY, *LPUVKEY;

typedef struct tagTEXTURESET										TEXTURESET, *LPTEXTURESET;
typedef struct tagIMAGESET											IMAGESET, *LPIMAGESET;
typedef struct tagANISET											ANISET, *LPANISET;

typedef struct tagSNDINST											SNDINST, *LPSNDINST;
typedef struct tagSFXINST											SFXINST, *LPSFXINST;
typedef struct tagOBJINST											OBJINST, *LPOBJINST;
typedef struct tagCLKINST											CLKINST, *LPCLKINST;
typedef struct tagACTINST											ACTINST, *LPACTINST;

typedef struct tagATTRIBUTE											ATTRIBUTE, *LPATTRIBUTE;
typedef struct tagOBJECT											OBJECT, *LPOBJECT;
typedef struct tagACTION											ACTION, *LPACTION;
typedef struct tagCLKIND											CLKIND, *LPCLKIND;
typedef struct tagOBJTEX											OBJTEX, *LPOBJTEX;
typedef struct tagCLOTH												CLOTH, *LPCLOTH;
typedef struct tagMESH												MESH, *LPMESH;
typedef struct tagANI												ANI, *LPANI;

typedef struct tagOBJPART											OBJPART, *LPOBJPART;
typedef struct tagOBJMSG											OBJMSG, *LPOBJMSG;
typedef struct tagOBJHIT											OBJHIT, *LPOBJHIT;
typedef struct tagOBJSND											OBJSND, *LPOBJSND;
typedef struct tagOBJSFX											OBJSFX, *LPOBJSFX;

typedef struct tagRESINDEX											RESINDEX, *LPRESINDEX;
typedef struct tagSNDIDX											SNDIDX, *LPSNDIDX;
typedef struct tagSFX												SFX, *LPSFX;

typedef struct tagMAPLIGHT											MAPLIGHT, *LPMAPLIGHT;
typedef struct tagMAPOBJ											MAPOBJ, *LPMAPOBJ;
typedef struct tagMAPSFX											MAPSFX, *LPMAPSFX;
typedef struct tagMAPSND											MAPSND, *LPMAPSND;

typedef struct tagIPOINT											IPOINT, *LPIPOINT;
typedef struct tagIRECT												IRECT, *LPIRECT;
typedef struct tagBLOCK												BLOCK, *LPBLOCK;

typedef struct tagVERTEXWINDING										VERTEXWINDING, *LPVERTEXWINDING;
typedef struct tagPOINTWINDING										POINTWINDING, *LPPOINTWINDING;
typedef struct tagTEXWINDING										TEXWINDING, *LPTEXWINDING;

typedef struct tagFARIMGPT											FARIMGPT, *LPFARIMGPT;
typedef struct tagFARIMG											FARIMG, *LPFARIMG;

typedef struct tagMPEG_DECODE_OPTION								MPEG_DECODE_OPTION, *LPMPEG_DECODE_OPTION;
typedef struct tagMPEG_DECODE_INFO									MPEG_DECODE_INFO, *LPMPEG_DECODE_INFO;

typedef struct tagHUFF_SETUP										HUFF_SETUP, *LPHUFF_SETUP;
typedef struct tagSIDE_INFO											SIDE_INFO, *LPSIDE_INFO;

typedef struct tagGR_INFO											GR_INFO, *LPGR_INFO;
typedef struct tagCB_INFO											CB_INFO, *LPCB_INFO;

typedef struct tagSCALE_FACTOR										SCALE_FACTOR, *LPSCALE_FACTOR;
typedef struct tagIS_SF_INFO										IS_SF_INFO, *LPIS_SF_INFO;
typedef struct tagTLOADINGSCENE										TLOADINGSCENE, *LPTLOADINGSCENE;

typedef struct tagTPROTECTED_MODULE									TPROTECTED_MODULE, *LPTPROTECTED_MODULE;
typedef struct tagTPROTECTED_PROC									TPROTECTED_PROC, *LPTPROTECTED_PROC;

typedef struct tagTPIVOT											TPIVOT, *LPTPIVOT;

typedef struct tagTTEXLOADBUF										TTEXLOADBUF, *LPTTEXLOADBUF;
typedef struct tagTVBLOADBUF										TVBLOADBUF, *LPTVBLOADBUF;
typedef struct tagTIBLOADBUF										TIBLOADBUF, *LPTIBLOADBUF;
typedef struct tagTLOADBUF											TLOADBUF, *LPTLOADBUF;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D standard vector

typedef vector<LPVERTEXWINDING>										VECTORVERTEXWINDING, *LPVECTORVERTEXWINDING;
typedef vector<LPPOINTWINDING>										VECTORPOINTWINDING, *LPVECTORPOINTWINDING;
typedef vector<LPTEXWINDING>										VECTORTEXWINDING, *LPVECTORTEXWINDING;

typedef vector<LPDIRECTSOUND3DBUFFER>								VDIRECTSOUND3DBUFFER, *LPVDIRECTSOUND3DBUFFER;
typedef vector<LPDIRECTSOUNDBUFFER>									VDIRECTSOUNDBUFFER, *LPVDIRECTSOUNDBUFFER;

typedef vector<LPDIRECT3DVERTEXBUFFER9>								VECTORVB, *LPVECTORVB;
typedef vector<LPDIRECT3DINDEXBUFFER9>								VECTORIB, *LPVECTORIB;

typedef vector<LPDIRECT3DSURFACE9>									VECTORSURFACE, *LPVECTORSURFACE;
typedef vector<LPDIRECT3DTEXTURE9>									VECTORTEXTURE, *LPVECTORTEXTURE;
typedef vector<LPTEXTURESET *>										VECTORTEXSET, *LPVECTORTEXSET;
typedef vector<LPTEXTURESET>										VECTORTEXTURESET, *LPVECTORTEXTURESET;
typedef vector<LPANIDATA>											VECTORANIDATA, *LPVECTORANIDATA;

typedef vector<LPTVERTEXINDEX>										VECTORTVERTEXINDEX, *LPVECTORTVERTEXINDEX;
typedef vector<LPINDEXEDMESH>										VECTORINDEXEDMESH, *LPVECTORINDEXEDMESH;
typedef vector<LPIBINDEXINFO>										VECTORIBINDEXINFO, *LPVECTORIBINDEXINFO;
typedef vector<LPIBINDEX>											VECTORIBINDEX, *LPVECTORIBINDEX;

typedef vector<LPFILEVB>											VECTORFILEVB, *LPVECTORFILEVB;
typedef vector<LPMESHVB>											VECTORMESHVB, *LPVECTORMESHVB;
typedef vector<LPIBDATA>											VECTORIBDATA, *LPVECTORIBDATA;
typedef vector<LPIBINFO>											VECTORIBINFO, *LPVECTORIBINFO;

typedef vector<LPIPOINT>											VECTORIPOINT, *LPVECTORIPOINT;
typedef vector<LPBLOCK>												VECTORBLOCK, *LPVECTORBLOCK;

typedef vector<LPAFFINEKEY>											VECTORAFFINEKEY, *LPVECTORAFFINEKEY;
typedef vector<LPANIKEY>											VECTORANIKEY, *LPVECTORANIKEY;
typedef vector<LPCLRKEY>											VECTORCLRKEY, *LPVECTORCLRKEY;
typedef vector<LPUVKEY>												VECTORUVKEY, *LPVECTORUVKEY;
typedef vector<LPSFX>												VECTORSFX, *LPVECTORSFX;

typedef vector<CString>												VECTORSTRING, *LPVECTORSTRING;
typedef vector<FLOAT>												VECTORFLOAT, *LPVECTORFLOAT;
typedef vector<DWORD>												VECTORDWORD, *LPVECTORDWORD;
typedef vector<BYTE>												VECTORBYTE, *LPVECTORBYTE;
typedef vector<int>													VECTORINT, *LPVECTORINT;
typedef vector<LPVOID>												VECTORPTR, *LPVECTORPTR;

typedef vector<LPD3DXMATRIX>										VECTORMATRIX, *LPVECTORMATRIX;
typedef vector<D3DXVECTOR3>											VECTORVEC,	*LPVECTORVEC;
typedef vector<LPD3DXVECTOR3>										VECTORPOINT, *LPVECTORPOINT;
typedef vector<LPMESHVERTEX>										VECTORVERTEX, *LPVECTORVERTEX;

typedef vector<LPSNDINST>											VSNDINST, *LPVSNDINST;

typedef vector<LPOBJPART>											VECTOROBJPART, *LPVECTOROBJPART;
typedef vector<LPOBJSFX>											VECTOROBJSFX, *LPVECTOROBJSFX;
typedef vector<LPOBJTEX>											VECTOROBJTEX, *LPVECTOROBJTEX;
typedef queue<DWORD>												QUEUEDWORD, *LPQUEUEDWORD;

typedef vector<LPOINT>												VECTORLPOINT, *LPLVECTORPOINT;
typedef vector<LPTLOADINGSCENE>										VECTORTLOADINGSCENE, *LPVECTORTLOADINGSCENE;

typedef vector<LPTPROTECTED_MODULE>									VTPROTECTED_MODULE, *LPVTPROTECTED_MODULE;
typedef vector<LPTPROTECTED_PROC>									VTPROTECTED_PROC, *LPVTPROTECTED_PROC;

typedef vector<LPTTEXLOADBUF>										VTTEXLOADBUF, *LPVTTEXLOADBUF;
typedef vector<LPTLOADBUF>											VTLOADBUF, *LPVTLOADBUF;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D map

typedef map< LPTEXTURESET, LPVECTORTEXWINDING>						MAPVECTORTEXWINDING, *LPMAPVECTORTEXWINDING;
typedef map< LPTEXTURESET, LPMESHVB>								MAPTEXVB, *LPMAPTEXVB;

typedef map< DWORD, LPTEXTURESET>									MAPTEXTURESET, *LPMAPTEXTURESET;
typedef map< DWORD, LPTEXWINDING>									MAPTEXWINDING, *LPMAPTEXWINDING;
typedef map< DWORD, LPD3DXPLANE>									MAPPLANE, *LPMAPPLANE;
typedef map< DWORD, LPBLOCK>										MAPBLOCK, *LPMAPBLOCK;

typedef map< int, LPVECTORVERTEXWINDING>							MAPVERTEXWINDING, *LPMAPVERTEXWINDING;
typedef map< int, LPVECTORMESHVB>									MAPVECTORVB, *LPMAPVECTORVB;

typedef map< int, LPINDEXEDMESH>									MAPINDEXEDMESH, *LPMAPINDEXEDMESH;
typedef map< int, LPMESHVB>											MAPMESHVB, *LPMAPMESHVB;

typedef map< FLOAT, VECTORINT *>									MAPWEIGHTINDEXBUF, *LPMAPWEIGHTINDEXBUF;
typedef map< int, LPWEIGHTBUF>										MAPWEIGHTBUF, *LPMAPWEIGHTBUF;

typedef map< DWORD, LPVOID>											MAPMEDIA, *LPMAPMEDIA;
typedef map< DWORD, DWORD>											MAPDWORD, *LPMAPDWORD;
typedef map< DWORD, FLOAT>											MAPFLOAT, *LPMAPFLOAT;

typedef map< DWORD, VECTOROBJPART *>								MAPOBJPART, *LPMAPOBJPART;
typedef map< DWORD, LPOBJSND>										MAPOBJSND, *LPMAPOBJSND;

typedef map< DWORD, LPSNDINST>										MAPSNDINST, *LPMAPSNDINST;
typedef map< DWORD, LPSFXINST>										MAPSFXINST, *LPMAPSFXINST;
typedef map< DWORD, LPCLKINST>										MAPCLKINST, *LPMAPCLKINST;
typedef map< DWORD, LPACTINST>										MAPACTINST, *LPMAPACTINST;

typedef map< DWORD, LPMAPLIGHT>										MAPMAPLIGHT, *LPMAPMAPLIGHT;
typedef map< DWORD, LPMAPOBJ>										MAPMAPOBJ, *LPMAPMAPOBJ;
typedef map< DWORD, LPMAPSFX>										MAPMAPSFX, *LPMAPMAPSFX;
typedef map< DWORD, LPMAPSND>										MAPMAPSND, *LPMAPMAPSND;

typedef map< DWORD, LPFARIMG>										MAPFARIMG, *LPMAPFARIMG;

typedef map< DWORD, LPATTRIBUTE>									MAPATTRIBUTE, *LPMAPATTRIBUTE;
typedef map< DWORD, LPACTION>										MAPACTION, *LPMAPACTION;
typedef map< DWORD, LPANISET>										MAPANISET, *LPMAPANISET;
typedef map< DWORD, LPCLKIND>										MAPCLKIND, *LPMAPCLKIND;
typedef map< DWORD, LPCLOTH>										MAPCLOTH, *LPMAPCLOTH;
typedef map< DWORD, LPMESH>											MAPMESH, *LPMAPMESH;
typedef map< DWORD, LPANI>											MAPANI, *LPMAPANI;

typedef map< DWORD, LPRESINDEX>										MAPRESINDEX, *LPMAPRESINDEX;
typedef map< DWORD, LPSNDIDX>										MAPSNDIDX, *LPMAPSNDIDX;

typedef map< CString, DWORD>										MAPSTRING, *LPMAPSTRING;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D multimap

typedef multimap< DWORD, LPVOID>									MULTIMAPMEDIA, *LPMULTIMAPMEDIA;
typedef multimap< DWORD, DWORD>										MULTIMAPDWORD, *LPMULTIMAPDWORD;
typedef multimap< DWORD, FLOAT>										MULTIMAPFLOAT, *LPMULTIMAPFLOAT;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D set

typedef set< LPVOID>												SETMEDIA, *LPSETMEDIA;
typedef set< DWORD>													SETDWORD, *LPSETDWORD;
typedef set< FLOAT>													SETFLOAT, *LPSETFLOAT;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Macro

#define GLOBALVB_ID( bGroupID, bFileID, bMESHType, bVBID)			((INT) MAKELONG( MAKEWORD( BYTE(bVBID), BYTE(bMESHType)), MAKEWORD( BYTE(bFileID), BYTE(bGroupID))))

#define MATRIXID_1ST(x)												(((DWORD) (x)) & 0x000000FF)
#define MATRIXID_2ND(x)												((((DWORD) (x)) >> 8) & 0x000000FF)
#define MATRIXID_3RD(x)												((((DWORD) (x)) >> 16) & 0x000000FF)
#define MATRIXID_4TH(x)												((((DWORD) (x)) >> 24) & 0x000000FF)

#define VALUEARGB( a, r, g, b)										((DWORD) ((((DWORD) (a)) & 0x000000FF) << 24)|((((DWORD) (r)) & 0x000000FF) << 16)|((((DWORD) (g)) & 0x000000FF) << 8)|(((DWORD) (b)) & 0x000000FF))
#define GETVALUE_A(c)												((INT) ((((DWORD) (c)) & 0xFF000000) >> 24))
#define GETVALUE_R(c)												((INT) ((((DWORD) (c)) & 0x00FF0000) >> 16))
#define GETVALUE_G(c)												((INT) ((((DWORD) (c)) & 0x0000FF00) >> 8))
#define GETVALUE_B(c)												((INT) (((DWORD) (c)) & 0x000000FF))


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Base Utility Class

class CT3DTexture;
class CTBitStream;
class CD3DDevice;
class CD3DCamera;
class CD3DLight;
class CD3DImage;
class CD3DSound;
class CT3DMusic;
class CD3DFont;
class CTShow;
class CTMath;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Object Class

class CTachyonAnimation;
class CTachyonObject;
class CTachyonMesh;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D SFX Class

class CTachyonSlashSFX;
class CTachyonSFX;
class CSFX;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Map Class

class CTachyonHUGEMAP;
class CTachyonEXTMAP;
class CTachyonBSPMAP;
class CTachyonMAP;
class CTBSPPortal;
class CTBSPNode;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Sound Class

class CTachyonMedia;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D UI Class

class CFrameGroup;
class TFrame;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Network Class

class CTachyonSession;
class CPacket;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Framework Class

class CTachyonApp;
class CTachyonWnd;
class CTachyonRes;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Dynamic billboard Class

class CTDynamicBillboard;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D class vector

typedef vector<CTBSPPortal *>							VECTORTBSPPORTAL, *LPVECTORTBSPPORTAL;
typedef vector<CT3DTexture *>							VECTORT3DTEX, *LPVECTORT3DTEX;
typedef vector<CD3DImage *>								VECTORIMAGE, *LPVECTORIMAGE;

typedef vector<CTachyonSFX *>							VECTORTSFX, *LPVECTORTSFX;
typedef vector<CSFX *>									VECTORCSFX, *LPVECTORCSFX;

typedef vector<CTDynamicBillboard *>					VTDBB, *LPVTDBB;
typedef vector<CTachyonObject*>							VECTORTOBJ, *LPVECTORTOBJ;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D class queue

typedef queue<CPacket *>								QPACKET, *LPQPACKET;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D class map

typedef map< DWORD, CT3DTexture *>						MAPT3DTEXTURE, *LPMAPT3DTEXTURE;
typedef map< DWORD, CD3DLight *>						MAPD3DLIGHT, *LPMAPD3DLIGHT;
typedef map< DWORD, TFrame *>							MAPFRAME, *LPMAPFRAME;
typedef map< DWORD, LPVOID>								MAPRES, *LPMAPRES;

typedef map< SOCKET, CTachyonSession *>					MAPSESSION, *LPMAPSESSION;
typedef map< DWORD, CTachyonObject *>					MAPOBJECT, *LPMAPOBJECT;
typedef map< DWORD, CTachyonSFX *>						MAPTSFX, *LPMAPTSFX;

typedef map< DWORD, CTBSPPortal *>						MAPTBSPPORTAL, *LPMAPTBSPPORTAL;
typedef map< DWORD, CTBSPNode *>						MAPTBSPNODE, *LPMAPTBSPNODE;

typedef map< LPVOID, CTDynamicBillboard *>				MAPTDBB, *LPMAPTDBB;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D function type

typedef void (*XFORM_PROC)( CT3DMusic *pMUSIC, LPSIDE_INFO pSIDE, LPVOID pPCM, int nVersion, int nFREQ, int nGROUP);
typedef void (*SBT_PROC)( CT3DMusic *pMUSIC, FLOAT *pSAMPLE, LPVOID pPCM, int nCH);
typedef int (*FRAME_PROC)( CT3DMusic *pMUSIC, int nFRAME);


//////////////////////////////////////////////////////////////////////////////
// Smart sync class for critical section

class CCSLock
{
public:
	CCSLock( CRITICAL_SECTION *pCS)
	{
		EnterCriticalSection(pCS);
		m_pCS = pCS;
	};

	virtual ~CCSLock()
	{
		LeaveCriticalSection(m_pCS);
	};

protected:
	CRITICAL_SECTION *m_pCS;
};

#define SMART_LOCKCS(x)					CCSLock lock_object(x);

struct tagCOMMAND
{
	DWORD m_dwID;
	DWORD m_dwParam;

	struct tagCOMMAND()
		: m_dwID(0),
		m_dwParam(0)
	{}

	struct tagCOMMAND( DWORD dwID )
		: m_dwID(dwID),
		m_dwParam(0)
	{}

	struct tagCOMMAND( DWORD dwID, DWORD dwPARAM)
		: m_dwID(dwID),
		m_dwParam(dwPARAM)
	{}

	operator DWORD()
	{
		return m_dwID;
	}
};

typedef struct tagCOMMAND									TCOMMAND, *LPTCOMMAND;
typedef vector<TCOMMAND>									VTCOMMAND, *LPVTCOMMAND;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D class Impl

#include <TConsole.h>
#include <TProfile.h>

#include <T3DTexture.h>
#include <TBitStream.h>
#include <D3DDevice.h>
#include <D3DCamera.h>
#include <D3DLight.h>
#include <D3DImage.h>
#include <T3DMusic.h>
#include <D3DSound.h>
#include <D3DFont.h>
#include <Packet.h>
#include <TShow.h>
#include <TComp.h>
#include <TMath.h>

#include <TachyonCompressor.h>
#include <TNICEventSink.h>

#include <TachyonAnimation.h>
#include <TachyonObject.h>
#include <TachyonMesh.h>
#include <TachyonMAP.h>
#include <TBSPPortal.h>
#include <TBSPNode.h>

#include <TachyonEXTMAP.h>
#include <TachyonBSPMAP.h>

#include <TachyonSlashSFX.h>
#include <TachyonHUGEMAP.h>
#include <TachyonMedia.h>
#include <TachyonSFX.h>

#include <TModuleProtector.h>

#include <TachyonSession.h>
#include <TachyonRes.h>
#include <TachyonApp.h>
#include <TachyonWnd.h>
#include <FrameGroup.h>

#include <TDynamicBillboard.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T3D Structure Impl

struct tagTMESHBUFFER
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	FLOAT m_fWeight[WEIGHT_COUNT];
	DWORD m_dwMatIndex;

	FLOAT m_fNormalX;
	FLOAT m_fNormalY;
	FLOAT m_fNormalZ;

	FLOAT m_fU1;
	FLOAT m_fV1;
	FLOAT m_fU2;
	FLOAT m_fV2;
};

struct tagWMESHVERTEX
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	FLOAT m_fWeight[WEIGHT_COUNT];
	DWORD m_dwMatIndex;

	FLOAT m_fNormalX;
	FLOAT m_fNormalY;
	FLOAT m_fNormalZ;

	FLOAT m_fU;
	FLOAT m_fV;
};

struct tagWLVERTEX
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	FLOAT m_fWeight[WEIGHT_COUNT];
	DWORD m_dwMatIndex;

	DWORD m_dwColor;

	FLOAT m_fU;
	FLOAT m_fV;
};

struct tagWPVERTEX
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	FLOAT m_fWeight[WEIGHT_COUNT];
	DWORD m_dwMatIndex;

	FLOAT m_fU;
	FLOAT m_fV;
};

struct tagMESHVERTEX
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	FLOAT m_fNormalX;
	FLOAT m_fNormalY;
	FLOAT m_fNormalZ;

	FLOAT m_fU1;
	FLOAT m_fV1;
	FLOAT m_fU2;
	FLOAT m_fV2;
};

struct tagTNLVERTEX
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	FLOAT m_fRHW;

	DWORD m_dwColor;

	FLOAT m_fU;
	FLOAT m_fV;
};

struct tagTVERTEX
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	FLOAT m_fRHW;

	FLOAT m_fU;
	FLOAT m_fV;
};

struct tagLVERTEX
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	DWORD m_dwColor;
	FLOAT m_fU;
	FLOAT m_fV;
};

struct tagPVERTEX
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	FLOAT m_fU;
	FLOAT m_fV;
};

struct tagTNLPOINT
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	FLOAT m_fRHW;

	DWORD m_dwColor;
};

struct tagTPOINT
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	FLOAT m_fRHW;
};

struct tagLPOINT
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	DWORD m_dwColor;
};

struct tagVBDATA
{
	LPBYTE m_pVB;

	DWORD m_dwCount;
	DWORD m_dwFVF;

	tagVBDATA()
	{
		m_pVB = NULL;

		m_dwCount = 0;
		m_dwFVF = 0;
	};

	~tagVBDATA()
	{
		if(m_pVB)
		{
			delete[] m_pVB;
			m_pVB = NULL;
		}

		m_dwCount = 0;
		m_dwFVF = 0;
	};
};

struct tagIBDATA
{
	LPWORD m_pIB;

	DWORD m_dwCount;
	WORD m_wMatrix;

	tagIBDATA()
	{
		m_dwCount = 0;
		m_wMatrix = 0;

		m_pIB = NULL;
	};

	~tagIBDATA()
	{
		if(m_pIB)
		{
			delete[] m_pIB;
			m_pIB = NULL;
		}

		m_dwCount = 0;
		m_wMatrix = 0;
	};
};

struct tagIBINFO
{
	VECTORIBDATA m_vIB;
	DWORD m_dwCount;

	tagIBINFO()
	{
		m_vIB.clear();
		m_dwCount = 0;
	};

	~tagIBINFO()
	{
		while(!m_vIB.empty())
		{
			delete m_vIB.back();
			m_vIB.pop_back();
		}

		m_dwCount = 0;
	};
};

struct tagTVERTEXINDEX
{
	WORD m_wMESH;
	WORD m_wINDEX;
	WORD m_wPOLY;
};

struct tagWEIGHTBUF
{
	FLOAT m_fWeight;
	int m_nCount;

	tagWEIGHTBUF()
	{
		m_fWeight = 0.0f;
		m_nCount = 0;
	};
};

struct tagT3DDATA
{
	LPBYTE m_pData;
	DWORD m_dwSize;

	tagT3DDATA()
	{
		m_pData = NULL;
		m_dwSize = 0;
	};

	~tagT3DDATA()
	{
		if(m_pData)
		{
			delete[] m_pData;
			m_pData = NULL;
		}

		m_dwSize = 0;
	};
};

struct tagANIDATA
{
	CTachyonAnimation *m_pAni;
	BYTE m_bReverse;

	tagANIDATA()
	{
		m_bReverse = FALSE;
		m_pAni = NULL;
	};
};

struct tagIBINDEX
{
	DWORD m_dwCount;
	DWORD m_dwPOS;

	tagIBINDEX()
	{
		m_dwCount = 0;
		m_dwPOS = 0;
	};
};

struct tagIBINDEXINFO
{
	VECTORIBINDEX m_vIB;

	tagIBINDEXINFO()
	{
		m_vIB.clear();
	};

	~tagIBINDEXINFO()
	{
		while(!m_vIB.empty())
		{
			delete m_vIB.back();
			m_vIB.pop_back();
		}
	};
};

struct tagINDEXEDMESH
{
	CT3DVertex m_vT3DVB;
	CT3DIndex m_vT3DIB;

	MAPDWORD m_mapLOCK;
	DWORD m_dwKEY;
	DWORD m_dwVB;
	DWORD m_dwIB;

	tagINDEXEDMESH()
	{
		m_mapLOCK.clear();

		m_dwKEY = 0;
		m_dwVB = 0;
		m_dwIB = 0;
	};

	~tagINDEXEDMESH()
	{
		m_mapLOCK.clear();

		m_dwKEY = 0;
		m_dwVB = 0;
		m_dwIB = 0;
	};
};

struct tagMESHVB
{
	CT3DVertex m_vVB;
	DWORD m_dwCount;

	tagMESHVB()
	{
		m_dwCount = 0;
	};
};

struct tagFILEVB
{
	VECTORINDEXEDMESH m_vWMESH;
	VECTORINDEXEDMESH m_vMESH;

	tagFILEVB()
	{
		m_vWMESH.clear();
		m_vMESH.clear();
	};

	~tagFILEVB()
	{
		while(!m_vWMESH.empty())
		{
			delete m_vWMESH.back();
			m_vWMESH.pop_back();
		}

		while(!m_vMESH.empty())
		{
			delete m_vMESH.back();
			m_vMESH.pop_back();
		}
	};
};

struct tagBONESNODE
{
	BYTE m_bParentROT;
	BYTE m_bParentPOS;
	BYTE m_bINDEX;

	tagBONESNODE()
	{
		m_bParentROT = 0;
		m_bParentPOS = 0;
		m_bINDEX = 0;
	};

	~tagBONESNODE()
	{
	};
};

struct tagBONESANIMATION
{
	int m_nPositionKeyCount;
	int m_nRotationKeyCount;
	int m_nScaleKeyCount;

	LPVOID m_pPositionKey;
	LPVOID m_pRotationKey;
	LPVOID m_pScaleKey;

	tagBONESANIMATION()
	{
		m_nPositionKeyCount = 0;
		m_nRotationKeyCount = 0;
		m_nScaleKeyCount = 0;

		m_pPositionKey = NULL;
		m_pRotationKey = NULL;
		m_pScaleKey = NULL;
	};

	~tagBONESANIMATION()
	{
		if(m_pPositionKey)
			delete[] m_pPositionKey;

		if(m_pRotationKey)
			delete[] m_pRotationKey;

		if(m_pScaleKey)
			delete[] m_pScaleKey;
	};
};

struct tagTPOINTKEY
{
	DWORD m_dwTime;
	D3DXVECTOR3 m_vKeyPoint;
};

struct tagTROTKEY
{
	DWORD m_dwTime;
	D3DXQUATERNION m_vKeyQuat;
};

struct tagTSCALEKEY
{
	DWORD m_dwTime;
	D3DXVECTOR3 m_vKeyScale;
};

struct tagAFFINEKEY
{
	D3DXQUATERNION m_vROT;

	D3DXVECTOR3 m_vSCALE;
	D3DXVECTOR3 m_vPOS;
};

struct tagCLRKEY
{
	DWORD m_dwTick;
	DWORD m_dwColor;

	tagCLRKEY()
	{
		m_dwColor = 0xFFFFFFFF;
		m_dwTick = 0;
	};
};

struct tagUVKEY
{
	DWORD m_dwTick;
	FLOAT m_fKeySU;
	FLOAT m_fKeySV;
	FLOAT m_fKeyU;
	FLOAT m_fKeyV;
	FLOAT m_fKeyR;

	tagUVKEY()
	{
		m_dwTick = 0;

		m_fKeySU = 1.0f;
		m_fKeySV = 1.0f;

		m_fKeyU = 0.0f;
		m_fKeyV = 0.0f;
		m_fKeyR = 0.0f;
	};
};

struct tagANIKEY
{
	DWORD m_dwEventID;

	FLOAT m_fTime;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	FLOAT m_fRotX;
	FLOAT m_fRotY;
	FLOAT m_fRotZ;

	FLOAT m_fScaleX;
	FLOAT m_fScaleY;
	FLOAT m_fScaleZ;

	tagANIKEY()
	{
		m_fTime = 0;

		m_dwEventID = 0;
		m_fPosX = 0.0f;
		m_fPosY = 0.0f;
		m_fPosZ = 0.0f;

		m_fRotX = 0.0f;
		m_fRotY = 0.0f;
		m_fRotZ = 0.0f;

		m_fScaleX = 1.0f;
		m_fScaleY = 1.0f;
		m_fScaleZ = 1.0f;
	};
};

struct tagANISET
{
	VECTORANIDATA m_vAni;
	VECTORANIKEY m_vKey;

	FLOAT m_fLocalTime;
	FLOAT m_fCurTime;
	int m_nKeyBase;

	FLOAT m_fTimeScale;
	DWORD m_dwLoopID;
	BYTE m_bLoop;

	LPANIDATA m_pLastData;

	tagANISET()
	{
		m_fTimeScale = 1.0f;
		m_fLocalTime = 0.0f;
		m_fCurTime = 0.0f;
		m_nKeyBase = 0;
		m_dwLoopID = 0;
		m_bLoop = FALSE;
		m_pLastData = NULL;

		m_vAni.clear();
		m_vKey.clear();
	};

	~tagANISET()
	{
		while(!m_vKey.empty())
		{
			delete m_vKey.back();
			m_vKey.pop_back();
		}

		ClearAni();
	};

	inline void ClearAni()
	{
		m_pLastData = NULL;

		while(!m_vAni.empty())
		{
			delete m_vAni.back();
			m_vAni.pop_back();
		}
	};

	inline void InsertAniData( CTachyonAnimation *pItem,
							   BYTE bReverse,
							   int nPos)
	{
		LPANIDATA pANI = new ANIDATA();

		pANI->m_bReverse = bReverse;
		pANI->m_pAni = pItem;

		if( nPos < 0 )
			nPos = INT(m_vAni.size());
		m_vAni.insert( m_vAni.begin() + nPos, pANI);
	};

	inline void DeleteAniData( int nPos)
	{
		if( nPos >= 0 && nPos < m_vAni.size() )
		{
			VECTORANIDATA::iterator it = m_vAni.begin();

			it += nPos;
			delete (*it);

			m_vAni.erase(it);
		}
	};

	inline FLOAT GetTotalTime()
	{
		int nCount = INT(m_vAni.size());
		DWORD dwTotal = 0;

		if( nCount == 0 )
			return m_fTimeScale * 4800.0f;

		for( int i=0; i<nCount; i++)
			dwTotal += m_vAni[i]->m_pAni->GetInterval();

		return ((FLOAT) dwTotal) * m_fTimeScale;
	};

	inline LPANIDATA GetAniData()
	{
		int nCount = INT(m_vAni.size());
		FLOAT fTotal = 0.0f;

		for( int i=0; i<nCount; i++)
		{
			FLOAT fInterval = m_fTimeScale * ((FLOAT) m_vAni[i]->m_pAni->GetInterval());

			fTotal += fInterval;
			if( m_fCurTime < fTotal )
			{
				m_fLocalTime = fTotal - m_fCurTime;

				if(!m_vAni[i]->m_bReverse)
					m_fLocalTime = fInterval - m_fLocalTime;
				m_fLocalTime /= m_fTimeScale;

				m_pLastData = m_vAni[i];
				return m_pLastData;
			}
		}

		if( nCount > 0 && m_fCurTime == fTotal )
		{
			m_fLocalTime = m_vAni[nCount - 1]->m_bReverse ? 0.0f : (FLOAT) m_vAni[nCount - 1]->m_pAni->GetInterval();
			m_pLastData = m_vAni[nCount - 1];
			return m_pLastData;
		}

		m_pLastData = NULL;
		return NULL;
	};

	inline ANIKEY GetAniKey()
	{
		int nCount = INT(m_vKey.size()) - 1;
		ANIKEY vResult;

		vResult.m_fTime = m_fCurTime;
		if(!m_vKey.empty())
		{
			vResult.m_fScaleX = m_vKey[0]->m_fScaleX;
			vResult.m_fScaleY = m_vKey[0]->m_fScaleY;
			vResult.m_fScaleZ = m_vKey[0]->m_fScaleZ;
			vResult.m_fPosX = m_vKey[0]->m_fPosX;
			vResult.m_fPosY = m_vKey[0]->m_fPosY;
			vResult.m_fPosZ = m_vKey[0]->m_fPosZ;
			vResult.m_fRotX = m_vKey[0]->m_fRotX;
			vResult.m_fRotY = m_vKey[0]->m_fRotY;
			vResult.m_fRotZ = m_vKey[0]->m_fRotZ;
		}
		else
		{
			vResult.m_fScaleX = 1.0f;
			vResult.m_fScaleY = 1.0f;
			vResult.m_fScaleZ = 1.0f;
			vResult.m_fPosX = 0.0f;
			vResult.m_fPosY = 0.0f;
			vResult.m_fPosZ = 0.0f;
			vResult.m_fRotX = 0.0f;
			vResult.m_fRotY = 0.0f;
			vResult.m_fRotZ = 0.0f;
		}

		for( int i=0; i<nCount; i++)
		{
			if( m_fCurTime < m_vKey[i]->m_fTime )
				break;

			if( m_fCurTime < m_vKey[i + 1]->m_fTime )
			{
				FLOAT fTotal = m_vKey[i + 1]->m_fTime - m_vKey[i]->m_fTime;
				FLOAT fTime = m_fCurTime - m_vKey[i]->m_fTime;

				vResult.m_fScaleX = m_vKey[i]->m_fScaleX + (m_vKey[i + 1]->m_fScaleX - m_vKey[i]->m_fScaleX) * fTime / fTotal;
				vResult.m_fScaleY = m_vKey[i]->m_fScaleY + (m_vKey[i + 1]->m_fScaleY - m_vKey[i]->m_fScaleY) * fTime / fTotal;
				vResult.m_fScaleZ = m_vKey[i]->m_fScaleZ + (m_vKey[i + 1]->m_fScaleZ - m_vKey[i]->m_fScaleZ) * fTime / fTotal;
				vResult.m_fPosX = m_vKey[i]->m_fPosX + (m_vKey[i + 1]->m_fPosX - m_vKey[i]->m_fPosX) * fTime / fTotal;
				vResult.m_fPosY = m_vKey[i]->m_fPosY + (m_vKey[i + 1]->m_fPosY - m_vKey[i]->m_fPosY) * fTime / fTotal;
				vResult.m_fPosZ = m_vKey[i]->m_fPosZ + (m_vKey[i + 1]->m_fPosZ - m_vKey[i]->m_fPosZ) * fTime / fTotal;
				vResult.m_fRotX = m_vKey[i]->m_fRotX + (m_vKey[i + 1]->m_fRotX - m_vKey[i]->m_fRotX) * fTime / fTotal;
				vResult.m_fRotY = m_vKey[i]->m_fRotY + (m_vKey[i + 1]->m_fRotY - m_vKey[i]->m_fRotY) * fTime / fTotal;
				vResult.m_fRotZ = m_vKey[i]->m_fRotZ + (m_vKey[i + 1]->m_fRotZ - m_vKey[i]->m_fRotZ) * fTime / fTotal;

				break;
			}

			vResult.m_fScaleX = m_vKey[i + 1]->m_fScaleX;
			vResult.m_fScaleY = m_vKey[i + 1]->m_fScaleY;
			vResult.m_fScaleZ = m_vKey[i + 1]->m_fScaleZ;
			vResult.m_fPosX = m_vKey[i + 1]->m_fPosX;
			vResult.m_fPosY = m_vKey[i + 1]->m_fPosY;
			vResult.m_fPosZ = m_vKey[i + 1]->m_fPosZ;
			vResult.m_fRotX = m_vKey[i + 1]->m_fRotX;
			vResult.m_fRotY = m_vKey[i + 1]->m_fRotY;
			vResult.m_fRotZ = m_vKey[i + 1]->m_fRotZ;
		}
		m_nKeyBase = i;

		return vResult;
	};

	inline int FindKey( FLOAT fTime)
	{
		int nCount = INT(m_vKey.size());

		for( int i=0; i<nCount; i++)
			if( m_vKey[i]->m_fTime == fTime )
				return i;

		return -1;
	};

	inline void InsertKey( LPANIKEY pKey)
	{
		VECTORANIKEY::iterator it;

		for( it = m_vKey.begin(); it != m_vKey.end(); it++)
			if( pKey->m_fTime < (*it)->m_fTime )
				break;

		if( it == m_vKey.end() )
			m_vKey.push_back(pKey);
		else
			m_vKey.insert( it, pKey);
	};

	inline void DeleteKey( FLOAT fTime)
	{
		VECTORANIKEY::iterator it = m_vKey.begin();

		while( it != m_vKey.end() )
			if( (*it)->m_fTime == fTime )
			{
				delete (*it);
				m_vKey.erase(it);
			}
			else
				it++;
	};

	inline void ClearKey()
	{
		VECTORANIKEY::iterator it = m_vKey.begin();

		while( it != m_vKey.end() )
		{
			delete (*it);
			it = m_vKey.erase(it);
		}
	}
};

struct tagIMAGESET
{
	VECTORIMAGE m_vImage;
	VECTORCLRKEY m_vKey;

	DWORD m_dwTotalTick;
	DWORD m_dwCurTick;

	tagIMAGESET()
	{
		m_dwTotalTick = 1000;
		m_dwCurTick = 0;

		m_vImage.clear();
		m_vKey.clear();
	};

	~tagIMAGESET()
	{
		while(!m_vKey.empty())
		{
			delete m_vKey.back();
			m_vKey.pop_back();
		}

		m_vImage.clear();
	};

	inline CD3DImage *GetImage()
	{
		int nCount = INT(m_vImage.size());
		int nIndex = m_dwCurTick * nCount / m_dwTotalTick;

		return nCount > 0 ? m_vImage[nIndex < nCount ? nIndex : nIndex - 1] : NULL;
	};

	inline DWORD GetColor()
	{
		int nCount = INT(m_vKey.size()) - 1;
		CLRKEY vResult;

		vResult.m_dwTick = m_dwCurTick;
		if(!m_vKey.empty())
			vResult.m_dwColor = m_vKey[0]->m_dwColor;
		else
			vResult.m_dwColor = 0xFFFFFFFF;

		for( int i=0; i<nCount; i++)
		{
			if( m_dwCurTick < m_vKey[i]->m_dwTick )
				break;

			if( m_dwCurTick < m_vKey[i + 1]->m_dwTick )
			{
				DWORD dwTotalTick = m_vKey[i + 1]->m_dwTick - m_vKey[i]->m_dwTick;
				DWORD dwTick = m_dwCurTick - m_vKey[i]->m_dwTick;

				vResult.m_dwColor = VALUEARGB(
					GETVALUE_A(m_vKey[i]->m_dwColor) + (GETVALUE_A(m_vKey[i + 1]->m_dwColor) - GETVALUE_A(m_vKey[i]->m_dwColor)) * INT(dwTick) / INT(dwTotalTick),
					GETVALUE_R(m_vKey[i]->m_dwColor) + (GETVALUE_R(m_vKey[i + 1]->m_dwColor) - GETVALUE_R(m_vKey[i]->m_dwColor)) * INT(dwTick) / INT(dwTotalTick),
					GETVALUE_G(m_vKey[i]->m_dwColor) + (GETVALUE_G(m_vKey[i + 1]->m_dwColor) - GETVALUE_G(m_vKey[i]->m_dwColor)) * INT(dwTick) / INT(dwTotalTick),
					GETVALUE_B(m_vKey[i]->m_dwColor) + (GETVALUE_B(m_vKey[i + 1]->m_dwColor) - GETVALUE_B(m_vKey[i]->m_dwColor)) * INT(dwTick) / INT(dwTotalTick));

				break;
			}

			vResult.m_dwColor = m_vKey[i + 1]->m_dwColor;
		}

		return vResult.m_dwColor;
	};

	inline CSize GetSize()
	{
		int nCount = INT(m_vImage.size());
		CSize size( 0, 0);

		for( int i=0; i<nCount; i++)
		{
			size.cx = max( size.cx, m_vImage[i]->GetWidth());
			size.cy = max( size.cy, m_vImage[i]->GetHeight());
		}

		return size;
	};

	inline int FindKey( DWORD dwTick)
	{
		int nCount = INT(m_vKey.size());

		for( int i=0; i<nCount; i++)
			if( m_vKey[i]->m_dwTick == dwTick )
				return i;

		return -1;
	};

	inline void InsertKey( LPCLRKEY pKey)
	{
		VECTORCLRKEY::iterator it;

		for( it = m_vKey.begin(); it != m_vKey.end(); it++)
			if( pKey->m_dwTick < (*it)->m_dwTick )
				break;

		if( it == m_vKey.end() )
			m_vKey.push_back(pKey);
		else
			m_vKey.insert( it, pKey);
	};

	inline void DeleteKey( DWORD dwTick)
	{
		VECTORCLRKEY::iterator it = m_vKey.begin();

		while( it != m_vKey.end() )
			if( (*it)->m_dwTick == dwTick )
			{
				delete (*it);
				m_vKey.erase(it);
			}
			else
				it++;
	};
};

struct tagTEXTURESET
{
protected:
	VECTORT3DTEX m_vTexture;

public:
	VECTORUVKEY m_vKey;

	DWORD m_dwTotalTick;
	DWORD m_dwCurTick;

	DWORD m_dwMipFilter;
	FLOAT m_fMipBias;

	tagTEXTURESET()
	{
		m_dwTotalTick = 1000;
		m_dwCurTick = 0;

		m_dwMipFilter = D3DTEXF_LINEAR;
		m_fMipBias = 0.0f;

		m_vTexture.clear();
		m_vKey.clear();
	};

	~tagTEXTURESET()
	{
		while(!m_vKey.empty())
		{
			delete m_vKey.back();
			m_vKey.pop_back();
		}

		m_vTexture.clear();
	};

	inline void PushTexturePtr( CT3DTexture *pTexture)
	{
		m_vTexture.push_back(pTexture);
	}

	virtual CT3DTexture *GetTexturePtr( int nIndex)
	{
		return m_vTexture[nIndex];
	}

	inline void SetTexturePtr( int nIndex, CT3DTexture *pTexture)
	{
		m_vTexture[nIndex] = pTexture;
	}

	inline int GetTextureCount()
	{
		return (int) m_vTexture.size();
	}

	inline void ClearTexture()
	{
		m_vTexture.clear();
	}

	LPDIRECT3DTEXTURE9 GetTexture()
	{
		int nCount = INT(m_vTexture.size());
		int nIndex = m_dwTotalTick ? (m_dwCurTick * nCount / m_dwTotalTick) : 0;
		CT3DTexture *pTEX = nCount > 0 ? GetTexturePtr(nIndex < nCount ? nIndex : nIndex - 1) : NULL;

		return pTEX ? pTEX->GetTEX() : NULL;
	};

	inline UVKEY GetUV()
	{
		int nCount = INT(m_vKey.size()) - 1;
		UVKEY vResult;

		vResult.m_dwTick = m_dwCurTick;
		if(!m_vKey.empty())
		{
			vResult.m_fKeySU = m_vKey[0]->m_fKeySU;
			vResult.m_fKeySV = m_vKey[0]->m_fKeySV;

			vResult.m_fKeyU = m_vKey[0]->m_fKeyU;
			vResult.m_fKeyV = m_vKey[0]->m_fKeyV;
			vResult.m_fKeyR = m_vKey[0]->m_fKeyR;
		}
		else
		{
			vResult.m_fKeySU = 1.0f;
			vResult.m_fKeySV = 1.0f;

			vResult.m_fKeyU = 0.0f;
			vResult.m_fKeyV = 0.0f;
			vResult.m_fKeyR = 0.0f;
		}

		for( int i=0; i<nCount; i++)
		{
			if( m_dwCurTick < m_vKey[i]->m_dwTick )
				break;

			if( m_dwCurTick < m_vKey[i + 1]->m_dwTick )
			{
				DWORD dwTotalTick = m_vKey[i + 1]->m_dwTick - m_vKey[i]->m_dwTick;
				DWORD dwTick = m_dwCurTick - m_vKey[i]->m_dwTick;

				vResult.m_fKeySU = m_vKey[i]->m_fKeySU + (m_vKey[i + 1]->m_fKeySU - m_vKey[i]->m_fKeySU) * dwTick / dwTotalTick;
				vResult.m_fKeySV = m_vKey[i]->m_fKeySV + (m_vKey[i + 1]->m_fKeySV - m_vKey[i]->m_fKeySV) * dwTick / dwTotalTick;

				vResult.m_fKeyU = m_vKey[i]->m_fKeyU + (m_vKey[i + 1]->m_fKeyU - m_vKey[i]->m_fKeyU) * dwTick / dwTotalTick;
				vResult.m_fKeyV = m_vKey[i]->m_fKeyV + (m_vKey[i + 1]->m_fKeyV - m_vKey[i]->m_fKeyV) * dwTick / dwTotalTick;
				vResult.m_fKeyR = m_vKey[i]->m_fKeyR + (m_vKey[i + 1]->m_fKeyR - m_vKey[i]->m_fKeyR) * dwTick / dwTotalTick;

				break;
			}

			vResult.m_fKeySU = m_vKey[i + 1]->m_fKeySU;
			vResult.m_fKeySV = m_vKey[i + 1]->m_fKeySV;

			vResult.m_fKeyU = m_vKey[i + 1]->m_fKeyU;
			vResult.m_fKeyV = m_vKey[i + 1]->m_fKeyV;
			vResult.m_fKeyR = m_vKey[i + 1]->m_fKeyR;
		}

		return vResult;
	};

	inline int FindKey( DWORD dwTick)
	{
		int nCount = INT(m_vKey.size());

		for( int i=0; i<nCount; i++)
			if( m_vKey[i]->m_dwTick == dwTick )
				return i;

		return -1;
	};

	inline void InsertKey( LPUVKEY pKey)
	{
		VECTORUVKEY::iterator it;

		for( it = m_vKey.begin(); it != m_vKey.end(); it++)
			if( pKey->m_dwTick < (*it)->m_dwTick )
				break;

		if( it == m_vKey.end() )
			m_vKey.push_back(pKey);
		else
			m_vKey.insert( it, pKey);
	};

	inline void DeleteKey( DWORD dwTick)
	{
		VECTORUVKEY::iterator it = m_vKey.begin();

		while( it != m_vKey.end() )
			if( (*it)->m_dwTick == dwTick )
			{
				delete (*it);
				m_vKey.erase(it);
			}
			else
				it++;
	};
};

struct tagSNDINST
{
	DWORD m_dwMediaID;
	DWORD m_dwFuncID;
	DWORD m_dwTick;

	BOOL m_bDeleteOnStop;

	tagSNDINST()
	{
		m_dwMediaID = 0;
		m_dwFuncID = 0;
		m_dwTick = 0;

		m_bDeleteOnStop = FALSE;
	};
};

struct tagOBJSND
{
	CD3DSound *m_pWAV;
	LPSNDINST m_pSND;

	BYTE m_bReset;
	BYTE m_bPlay;

	int m_nIndex;

	tagOBJSND()
	{
		m_nIndex = -1;

		m_bReset = FALSE;
		m_bPlay = TRUE;

		m_pWAV = NULL;
		m_pSND = NULL;
	};
};

struct tagSFXINST
{
	LPSFX m_pSFX;
	DWORD m_dwSFX;

	D3DXVECTOR3 m_vPOS;
	D3DXVECTOR3 m_vROT;

	DWORD m_dwFuncID;
	DWORD m_dwTick;
	int m_nPivotID;

	BOOL m_bDeleteOnStop;
	BYTE m_bBias;
	MAPDWORD m_mapHideByAction;

	tagSFXINST()
	{
		m_dwSFX = 0;

		m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		m_vROT = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

		m_bDeleteOnStop = FALSE;
		m_bBias = 0;

		m_nPivotID = 0;
		m_pSFX = NULL;

		m_dwFuncID = 0;
		m_dwTick = 0;
	};
};

struct tagOBJSFX
{
	LPSFXINST m_pSFX;
	DWORD m_dwID;

	BYTE m_bUseOBJScale;
	BYTE m_bDeleteINST;

	LPVOID m_pUserData;

	tagOBJSFX()
	: m_bUseOBJScale( FALSE ),
	m_bDeleteINST( FALSE ),
	m_pSFX( NULL ),
	m_dwID( 0 ),
	m_pUserData(NULL)
	{
	};

	~tagOBJSFX()
	{
		if( m_bDeleteINST && m_pSFX )
		{
			delete m_pSFX;
			m_pSFX = NULL;
		}
	};
};

struct tagANI
{
	MAPSNDINST m_mapSND;
	MAPSFXINST m_mapSFX;
	LPANISET m_pANI;

	tagANI()
	{
		m_mapSFX.clear();
		m_mapSND.clear();
		m_pANI = NULL;
	};

	~tagANI()
	{
		MAPSNDINST::iterator itSND;
		MAPSFXINST::iterator itSFX;

		for( itSND = m_mapSND.begin(); itSND != m_mapSND.end(); itSND++)
			delete (*itSND).second;

		for( itSFX = m_mapSFX.begin(); itSFX != m_mapSFX.end(); itSFX++)
			delete (*itSFX).second;

		m_mapSND.clear();
		m_mapSFX.clear();
	};
};

struct tagACTION
{
	MAPSNDINST m_mapSND;
	MAPSFXINST m_mapSFX;
	MAPANI m_mapANI;

	tagACTION()
	{
		m_mapANI.clear();
		m_mapSFX.clear();
		m_mapSND.clear();
	};

	~tagACTION()
	{
		MAPSNDINST::iterator itSND;
		MAPSFXINST::iterator itSFX;
		MAPANI::iterator it;

		for( itSND = m_mapSND.begin(); itSND != m_mapSND.end(); itSND++)
			delete (*itSND).second;

		for( itSFX = m_mapSFX.begin(); itSFX != m_mapSFX.end(); itSFX++)
			delete (*itSFX).second;

		for( it = m_mapANI.begin(); it != m_mapANI.end(); it++)
			delete (*it).second;

		m_mapANI.clear();
		m_mapSND.clear();
		m_mapSFX.clear();
	};
};

struct tagOBJTEX
{
	LPTEXTURESET m_pTEX[2];
	BYTE m_bType[2];

	FLOAT m_fIntensity;
	DWORD m_dwCOLORID;
	DWORD m_dwCOLOR;
	DWORD m_dwOrder;
	DWORD m_dwOP;

	DWORD m_dwDESTBlend;
	DWORD m_dwSRCBlend;
	DWORD m_dwBlendOP;

	BYTE m_bMinAlpha;
	BYTE m_bZEnable;
	BYTE m_bZWrite;
	BYTE m_b2side;

	BOOL m_bUseDirLight;
	DWORD m_dwAmbientCOLOR;

	tagOBJTEX()
	{
		for( int i=0; i<2; i++)
		{
			m_bType[i] = TT_TEX;
			m_pTEX[i] = NULL;
		}

		m_dwDESTBlend = D3DBLEND_INVSRCALPHA;
		m_dwSRCBlend = D3DBLEND_SRCALPHA;
		m_dwBlendOP = D3DBLENDOP_ADD;

		m_dwOP = D3DTOP_DISABLE;
		m_dwCOLOR = 0xFFFFFFFF;
		m_dwOrder = 0;
		m_dwCOLORID = 0;

		m_fIntensity = 100.0f;
		m_bMinAlpha = 0x00;
		m_bZEnable = TRUE;
		m_bZWrite = TRUE;
		m_b2side = FALSE;

		m_bUseDirLight = TRUE;
		m_dwAmbientCOLOR = 0xFFFFFFFF;
	};
};

struct tagMESH
{
	CTachyonMesh* m_pMESH;
	VECTOROBJTEX m_vTEX;

	tagMESH()
	{
		m_pMESH = NULL;
		m_vTEX.clear();
	};

	~tagMESH()
	{
		while(!m_vTEX.empty())
		{
			delete m_vTEX.back();
			m_vTEX.pop_back();
		}

		m_pMESH = NULL;
	};
};

struct tagCLOTH
{
	MAPMESH m_mapMESH;

	tagCLOTH()
	{
		m_mapMESH.clear();
	};

	~tagCLOTH()
	{
		MAPMESH::iterator it;

		for( it = m_mapMESH.begin(); it != m_mapMESH.end(); it++)
			delete (*it).second;
	};
};

struct tagCLKIND
{
	MAPCLOTH m_mapCLOTH;

	tagCLKIND()
	{
		m_mapCLOTH.clear();
	};

	~tagCLKIND()
	{
		MAPCLOTH::iterator it;

		for( it = m_mapCLOTH.begin(); it != m_mapCLOTH.end(); it++)
			delete (*it).second;
	};
};

struct tagCLKINST
{
	DWORD m_dwCurMESH;
	DWORD m_dwCurCL;
	DWORD m_dwHideID;

	LPMESH m_pMESH;

	tagCLKINST()
	{
		m_dwHideID = 0xFFFFFFFF;
		m_dwCurMESH = 0;
		m_dwCurCL = 0;

		m_pMESH = NULL;
	};
};

struct tagSFX
{
	VECTORCLRKEY m_vKey;
	VECTORSFX m_vSFX;
	CTachyonRes *m_pRES;

	BYTE m_bSFXType;
	BYTE m_bSRCType;
	union
	{
		LPTEXTURESET m_pTEX;
		LPOBJECT m_pOBJ;
		LPSFX m_pSFX;
	} m_SRC;

	LPANISET m_pANISRC;
	DWORD m_dwLifeTime;

	FLOAT m_fFollow;
	FLOAT m_fSizeX;
	FLOAT m_fSizeY;

	MAPCLKINST m_mapCLK;
	DWORD m_dwOBJFuncID;
	DWORD m_dwACTFuncID;
	DWORD m_dwANIFuncID;
	DWORD m_dwACT;
	DWORD m_dwANI;

	LPANISET m_pSprayANI;
	BYTE m_bSprayType;
	BYTE m_bZEnable;
	BYTE m_bZWrite;
	BYTE m_bStrict;
	BYTE m_bFollow;

	DWORD m_dwDESTBlend;
	DWORD m_dwSRCBlend;
	DWORD m_dwBlendOP;

	DWORD m_dwGTime;
	DWORD m_dwGCnt;

	D3DXVECTOR3 m_vOutBound;
	D3DXVECTOR3 m_vInBound;

	DWORD m_dwPLifeTime;
	DWORD m_dwERRTime;

	D3DXVECTOR3 m_vERRScale;
	D3DXVECTOR3 m_vERRPos;
	D3DXVECTOR3 m_vERRRot;
	DWORD m_dwLodLevel;

	tagSFX()
	{
		m_dwDESTBlend = D3DBLEND_INVSRCALPHA;
		m_dwSRCBlend = D3DBLEND_SRCALPHA;
		m_dwBlendOP = D3DBLENDOP_ADD;

		m_bSRCType = SFXSRC_TEX;
		m_bSFXType = SFX_SINGLE;
		m_SRC.m_pTEX = NULL;

		m_dwLifeTime = 0;
		m_pANISRC = NULL;
		m_pRES = NULL;

		m_fFollow = 0.0f;
		m_fSizeX = 1.0f;
		m_fSizeY = 1.0f;

		m_mapCLK.clear();
		m_dwOBJFuncID = 0;
		m_dwACTFuncID = 0;
		m_dwANIFuncID = 0;
		m_dwACT = 0;
		m_dwANI = 0;

		m_bSprayType = SPRAY_BOX;
		m_pSprayANI = NULL;
		m_bZEnable = TRUE;
		m_bZWrite = FALSE;
		m_bStrict = FALSE;
		m_bFollow = FALSE;

		m_dwGTime = 100;
		m_dwGCnt = 1;

		m_vOutBound = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
		m_vInBound = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

		m_dwPLifeTime = 1000;
		m_dwERRTime = 0;

		m_vERRScale = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		m_vERRPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		m_vERRRot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		m_dwLodLevel = 6;

		m_vKey.clear();
		m_vSFX.clear();
	};

	~tagSFX()
	{
		MAPCLKINST::iterator it;

		for( it = m_mapCLK.begin(); it != m_mapCLK.end(); it++)
			if( (*it).second )
			{
				delete (*it).second;
				(*it).second = NULL;
			}

		while(!m_vKey.empty())
		{
			if( m_vKey.back() )
			{
				delete m_vKey.back();
				m_vKey.back() = NULL;
			}
			m_vKey.pop_back();
		}

		m_mapCLK.clear();
		m_vSFX.clear();
		m_pRES = NULL;
	};

	inline DWORD GetColor( DWORD dwTick)
	{
		int nCount = INT(m_vKey.size()) - 1;
		CLRKEY vResult;

		vResult.m_dwTick = dwTick;
		if(!m_vKey.empty())
			vResult.m_dwColor = m_vKey[0]->m_dwColor;
		else
			vResult.m_dwColor = 0xFFFFFFFF;

		for( int i=0; i<nCount; i++)
		{
			if( dwTick < m_vKey[i]->m_dwTick )
				break;

			if( dwTick < m_vKey[i + 1]->m_dwTick )
			{
				DWORD dwTotalTick = m_vKey[i + 1]->m_dwTick - m_vKey[i]->m_dwTick;
				DWORD dwCurTick = dwTick - m_vKey[i]->m_dwTick;

				vResult.m_dwColor = VALUEARGB(
					GETVALUE_A(m_vKey[i]->m_dwColor) + (GETVALUE_A(m_vKey[i + 1]->m_dwColor) - GETVALUE_A(m_vKey[i]->m_dwColor)) * INT(dwCurTick) / INT(dwTotalTick),
					GETVALUE_R(m_vKey[i]->m_dwColor) + (GETVALUE_R(m_vKey[i + 1]->m_dwColor) - GETVALUE_R(m_vKey[i]->m_dwColor)) * INT(dwCurTick) / INT(dwTotalTick),
					GETVALUE_G(m_vKey[i]->m_dwColor) + (GETVALUE_G(m_vKey[i + 1]->m_dwColor) - GETVALUE_G(m_vKey[i]->m_dwColor)) * INT(dwCurTick) / INT(dwTotalTick),
					GETVALUE_B(m_vKey[i]->m_dwColor) + (GETVALUE_B(m_vKey[i + 1]->m_dwColor) - GETVALUE_B(m_vKey[i]->m_dwColor)) * INT(dwCurTick) / INT(dwTotalTick));

				break;
			}

			vResult.m_dwColor = m_vKey[i + 1]->m_dwColor;
		}

		return vResult.m_dwColor;
	};

	inline int FindKey( DWORD dwTick)
	{
		int nCount = INT(m_vKey.size());

		for( int i=0; i<nCount; i++)
			if( m_vKey[i]->m_dwTick == dwTick )
				return i;

		return -1;
	};

	inline void InsertKey( LPCLRKEY pKey)
	{
		VECTORCLRKEY::iterator it;

		for( it = m_vKey.begin(); it != m_vKey.end(); it++)
			if( pKey->m_dwTick < (*it)->m_dwTick )
				break;

		if( it == m_vKey.end() )
			m_vKey.push_back(pKey);
		else
			m_vKey.insert( it, pKey);
	};

	inline void DeleteKey( DWORD dwTick)
	{
		VECTORCLRKEY::iterator it = m_vKey.begin();

		while( it != m_vKey.end() )
			if( (*it)->m_dwTick == dwTick )
			{
				delete (*it);
				m_vKey.erase(it);
			}
			else
				it++;
	};
};

struct tagRESINDEX
{
	DWORD m_dwFileID;
	DWORD m_dwPOS;

	tagRESINDEX()
	{
		m_dwFileID = 0;
		m_dwPOS = 0;
	};
};

struct tagSNDIDX
{
	CD3DSound *m_pSND;
	DWORD m_dwMediaID;
	int m_nIndex;

	tagSNDIDX()
	{
		m_pSND = NULL;

		m_dwMediaID = 0;
		m_nIndex = -1;
	};
};

struct tagATTRIBUTE
{
	BYTE m_bValueType;
	LPBYTE m_pValue;
	int m_nSize;

	tagATTRIBUTE()
	{
		m_bValueType = ATTRTYPE_INT;
		m_pValue = NULL;
		m_nSize = 0;
	};

	~tagATTRIBUTE()
	{
		if(m_pValue)
			delete[] m_pValue;
	};
};

struct tagMAPOBJ
{
	MAPATTRIBUTE m_mapATTR;

	MAPCLKINST m_mapCLK;
	LPOBJECT m_pOBJ;

	DWORD m_dwOBJFuncID;
	DWORD m_dwACTFuncID;
	DWORD m_dwANIFuncID;

	DWORD m_dwCurACT;
	DWORD m_dwCurANI;

	D3DXVECTOR3 m_vSCALE;
	D3DXVECTOR3 m_vPOS;
	D3DXVECTOR3 m_vROT;

	tagMAPOBJ()
	{
		m_vSCALE = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
		m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		m_vROT = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

		m_dwOBJFuncID = 0;
		m_dwACTFuncID = 0;
		m_dwANIFuncID = 0;

		m_mapATTR.clear();
		m_mapCLK.clear();
		m_dwCurACT = 0;
		m_dwCurANI = 0;
		m_pOBJ = NULL;
	};

	~tagMAPOBJ()
	{
		MAPATTRIBUTE::iterator itATTR;
		MAPCLKINST::iterator itCI;

		for( itATTR = m_mapATTR.begin(); itATTR != m_mapATTR.end(); itATTR++)
			delete (*itATTR).second;

		for( itCI = m_mapCLK.begin(); itCI != m_mapCLK.end(); itCI++)
			delete (*itCI).second;

		m_mapATTR.clear();
		m_mapCLK.clear();
		m_pOBJ = NULL;
	};
};

struct tagMAPSFX
{
	LPSFX m_pSFX;

	D3DXVECTOR3 m_vPOS;
	D3DXVECTOR3 m_vROT;
	BYTE m_bBias;

	tagMAPSFX()
	{
		m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		m_vROT = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

		m_pSFX = NULL;
		m_bBias = 0;
	};
};

struct tagMAPSND
{
	D3DXVECTOR3 m_vPOS;
	DWORD m_dwMediaID;

	tagMAPSND()
	{
		m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		m_dwMediaID = 0;
	};
};

struct tagVERTEXWINDING
{
	VECTORVERTEX m_vPoint;

	D3DXPLANE m_vPlane;
	BOOL m_bCheck;

	int m_nMeshIndex;
	int m_nIndex;
	int m_nCount;

	tagVERTEXWINDING()
	{
		m_vPoint.clear();

		m_nMeshIndex = 0;
		m_nIndex = 0;
		m_nCount = 0;
		m_bCheck = FALSE;
	};

	~tagVERTEXWINDING()
	{
		while(!m_vPoint.empty())
		{
			delete m_vPoint.back();
			m_vPoint.pop_back();
		}

		m_nMeshIndex = 0;
		m_nIndex = 0;
		m_nCount = 0;
		m_bCheck = FALSE;
	};
};

struct tagTEXWINDING
{
	VECTORVERTEXWINDING m_vWINDING;
	LPTEXTURESET m_pTEX;

	DWORD m_dwColor;

	tagTEXWINDING()
	{
		m_vWINDING.clear();

		m_dwColor = 0xFF000000;
		m_pTEX = NULL;
	};

	~tagTEXWINDING()
	{
		while(!m_vWINDING.empty())
		{
			delete m_vWINDING.back();
			m_vWINDING.pop_back();
		}

		m_dwColor = 0xFF000000;
		m_pTEX = NULL;
	};
};

struct tagMAPLIGHT
{
	D3DXVECTOR3 m_vPOS;
	D3DXVECTOR3 m_vDIR;

	FLOAT m_fRange;
	FLOAT m_fROT;

	DWORD m_dwTextureID;
	DWORD m_dwAmbient;
	DWORD m_dwDiffuse;

	BYTE m_bUseLM;
	BYTE m_bUseDL;
	BYTE m_bType;

	tagMAPLIGHT()
	{
		m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		m_vDIR = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

		m_fRange = 0.0f;
		m_fROT = 0.0f;

		m_dwAmbient = 0x00000000;
		m_dwDiffuse = 0x00000000;
		m_dwTextureID = 0;

		m_bType = MLT_DIRECTION;
		m_bUseLM = TRUE;
		m_bUseDL = TRUE;
	};
};

struct tagHUFF_SETUP
{
	LPHUFF_ELEMENT m_pTABLE;

	int m_nBITS;
	int m_nCASE;
};

struct tagMPEG_DECODE_OPTION
{
	int m_nReduction;
	int m_nConvert;
	int m_nFreqLimit;
};

struct tagMPEG_DECODE_INFO
{
	MPEG_HEADER m_vHeader;

	int m_nChannels;
	int m_nBitsPerSample;
	int m_nBitRate;

	int m_nFrames;
	int m_nSkipSize;
	int m_nDataSize;

	int m_nMinInputSize;
	int m_nMaxInputSize;
	int m_nOutputSize;
};

struct tagGR_INFO
{
	DWORD m_dwPart2_3_Length;
	DWORD m_dwBigValues;
	DWORD m_dwGlobalGain;
	DWORD m_dwSFCompress;
	DWORD m_dwWindowSwitchingFlag;
	DWORD m_dwBlockType;
	DWORD m_dwMixedBlockFlag;
	DWORD m_dwTableSelect[3];
	DWORD m_dwSubblockGain[3];
	DWORD m_dwRegion0Count;
	DWORD m_dwRegion1Count;
	DWORD m_dwPreFlag;
	DWORD m_dwSFScale;
	DWORD m_dwCount1TableSelect;
};

struct tagCB_INFO
{
	int m_nMaxS[3];
	int m_nMax;

	int m_nLBType;
	int m_nType;

	int m_nStart;
	int m_nEnd;
};

struct tagSIDE_INFO
{
	GR_INFO m_vINFO[2][2];
	DWORD m_dwSCFSI[2];

	DWORD m_dwPRIVATE;
	DWORD m_dwSTART;
};

struct tagSCALE_FACTOR
{
	int m_nS[3][13];
	int m_nL[23];
};

struct tagIS_SF_INFO
{
	int m_nIScale;

	int m_vSL[3];
	int m_vR[3];
};

struct tagTLOADINGSCENE
{
	int m_nGaugeX;
	int m_nGaugeY;
	int m_nTextX;
	int m_nTextY;
	int m_nFadeInTime;
	int m_nKeepTime;
	int m_nFadeOutTime;
	BYTE m_bR, m_bG, m_bB;
	BOOL m_bShowBar;
	int m_nSeq;

	CT3DImage m_vGAUGE;
	CT3DImage m_vIMG;

	int m_nState; // 0:None 1:FadeIn 2:Keep 3:FadeOut
	FLOAT m_fAlpha;
	DWORD m_dwKeepTime;
	BOOL m_bRealTime;
};

struct tagTPROTECTED_PROC
{
	BYTE m_vDATA[TMAX_CHECK_CODE_SIZE];
	BYTE m_bSIZE;

	CString m_strProcName;

	tagTPROTECTED_PROC()
	{
		ZeroMemory( m_vDATA, TMAX_CHECK_CODE_SIZE);

		m_strProcName.Empty();
		m_bSIZE = 0;
	};

	~tagTPROTECTED_PROC()
	{
		ZeroMemory( m_vDATA, TMAX_CHECK_CODE_SIZE);

		m_strProcName.Empty();
		m_bSIZE = 0;
	};
};

struct tagTPROTECTED_MODULE
{
	VTPROTECTED_PROC m_vTPROC;
	CString m_strModuleName;

	void Clear()
	{
		while(!m_vTPROC.empty())
		{
			delete m_vTPROC.back();
			m_vTPROC.pop_back();
		}
		m_strModuleName.Empty();
	};

	LPTPROTECTED_PROC AddProc( CString& strNAME)
	{
		LPTPROTECTED_PROC pTPROC = new TPROTECTED_PROC();

		pTPROC->m_strProcName = strNAME;
		m_vTPROC.push_back(pTPROC);

		return pTPROC;
	};

	tagTPROTECTED_MODULE()
	{
		m_strModuleName.Empty();
		m_vTPROC.clear();
	};

	~tagTPROTECTED_MODULE()
	{
		Clear();
	};
};

struct tagTPIVOT
{
	D3DXVECTOR3 m_vScale;
	D3DXQUATERNION m_vRot;
};

struct tagOBJPART
{
	CTachyonMesh *m_pMESH;
	LPOBJTEX m_pTEX;
	DWORD m_dwIndex;
	BYTE m_bSelfDeleteTEX;

	tagOBJPART()
	{
		m_pMESH = NULL;
		m_pTEX = NULL;
		m_dwIndex = 0;
		m_bSelfDeleteTEX = FALSE;
	};

	~tagOBJPART()
	{
		if( m_bSelfDeleteTEX && m_pTEX)
			delete m_pTEX;

		m_pMESH = NULL;
		m_pTEX = NULL;
		m_dwIndex = 0;
		m_bSelfDeleteTEX = FALSE;
	};
};

struct tagOBJHIT
{
	LPMESH m_pMESH;

	DWORD m_dwMESH;
	DWORD m_dwCLK;
	DWORD m_dwCL;

	FLOAT m_fDist;
	int m_nIndex;

	tagOBJHIT()
	{
		m_pMESH = NULL;
		m_fDist = 0.0f;
		m_nIndex = -1;

		m_dwMESH = 0;
		m_dwCLK = 0;
		m_dwCL = 0;
	};
};

struct tagOBJMSG
{
	DWORD_PTR m_pPARAM;
	DWORD_PTR m_pOBJ;
	DWORD m_dwMSG;

	tagOBJMSG()
	{
		m_pPARAM = NULL;
		m_pOBJ = NULL;

		m_dwMSG = 0;
	};
};

struct tagACTINST
{
	DWORD m_dwCurANI;
	LPANI m_pANI;

	tagACTINST()
	{
		m_dwCurANI = 0;
		m_pANI = NULL;
	};
};

struct tagOBJECT
{
	MAPATTRIBUTE m_mapATTR;
	MAPDWORD m_mapPIVOT;

	MAPCLKIND m_mapCLKIND;
	MAPACTION m_mapACTION;
	MAPSNDINST m_mapSND;
	MAPSFXINST m_mapSFX;

	CTachyonRes *m_pRES;
	BYTE m_bPivotCount;
	BYTE m_bFileID;

	tagOBJECT()
	{
		m_mapCLKIND.clear();
		m_mapACTION.clear();
		m_mapPIVOT.clear();
		m_mapATTR.clear();
		m_mapSFX.clear();
		m_mapSND.clear();

		m_bPivotCount = 0;
		m_bFileID = 0;
		m_pRES = NULL;
	};

	~tagOBJECT()
	{
		MAPATTRIBUTE::iterator itATTR;
		MAPSNDINST::iterator itSND;
		MAPSFXINST::iterator itSFX;
		MAPCLKIND::iterator itCLK;
		MAPACTION::iterator itACT;

		for( itATTR = m_mapATTR.begin(); itATTR != m_mapATTR.end(); itATTR++)
			delete (*itATTR).second;

		for( itCLK = m_mapCLKIND.begin(); itCLK != m_mapCLKIND.end(); itCLK++)
			delete (*itCLK).second;

		for( itACT = m_mapACTION.begin(); itACT != m_mapACTION.end(); itACT++)
			delete (*itACT).second;

		for( itSND = m_mapSND.begin(); itSND != m_mapSND.end(); itSND++)
			delete (*itSND).second;

		for( itSFX = m_mapSFX.begin(); itSFX != m_mapSFX.end(); itSFX++)
			delete (*itSFX).second;

		m_mapCLKIND.clear();
		m_mapACTION.clear();
		m_mapPIVOT.clear();
		m_mapATTR.clear();
		m_mapSFX.clear();
		m_mapSND.clear();
		m_pRES = NULL;
	};
};

struct tagTTEXLOADBUF
{
	DWORD m_dwDATA;
	DWORD m_dwSIZE;

	LPBYTE m_pDATA;
	BYTE m_bFormat;

	tagTTEXLOADBUF()
	{
		m_bFormat = DXT3;
		m_pDATA = NULL;

		m_dwDATA = 0;
		m_dwSIZE = 0;
	};
};

struct tagTVBLOADBUF
{
	DWORD m_dwSIZE;
	DWORD m_dwUSE;
	DWORD m_dwFVF;

	tagTVBLOADBUF()
	{
		m_dwSIZE = 0;
		m_dwUSE = 0;
		m_dwFVF = 0;
	};
};

struct tagTIBLOADBUF
{
	D3DFORMAT m_nFORMAT;

	DWORD m_dwSIZE;
	DWORD m_dwUSE;

	tagTIBLOADBUF()
	{
		m_nFORMAT = D3DFMT_INDEX16;

		m_dwSIZE = 0;
		m_dwUSE = 0;
	};
};

struct tagTLOADBUF
{
	LPBYTE m_pDATA;

	DWORD m_dwSIZE;
	DWORD m_dwPOS;

	tagTLOADBUF()
	{
		m_pDATA = NULL;

		m_dwSIZE = 0;
		m_dwPOS = 0;
	};

	~tagTLOADBUF()
	{
		if(m_pDATA)
			delete[] m_pDATA;
	};
};


#endif // !defined ___T3D_H
