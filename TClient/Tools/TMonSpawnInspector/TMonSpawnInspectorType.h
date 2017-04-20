#pragma once

class CTMonSpawnInspectorObjBase;


// Define

#define TLOWCULL_BOUND				((FLOAT) 1.0f)
#define TMIDCULL_BOUND				((FLOAT) 2.0f)
#define THICULL_BOUND					((FLOAT) 4.0f)

#define TLOWCULL_FACTOR				((FLOAT) 0.3f)
#define TMIDCULL_FACTOR				((FLOAT) 0.5f)
#define THICULL_FACTOR					((FLOAT) 0.7f)

#define TOBJCELL_LENGTH				((FLOAT) 8.0f)
#define TOBJCELL_WIDTH					(128)
#define TOBJCELL_COUNT				(16384)

#define TRESULT_INVALID_POINT		(BYTE)0x01
#define TRESULT_INVALID_HEIGHT		(BYTE)0x10
#define TRESULT_INVALID_UNIT			(BYTE)0x04

#define THEIGHT_ZERO			((FLOAT) 0.01f)
#define TCOLLISION_HEIGHT		((FLOAT) 2.0f)
#define TCOLLISION_SIZE			((FLOAT) 1.4f)

// typedef

typedef struct tagTOBJBLOCK					TOBJBLOCK, *LPTOBJBLOCK;
typedef struct tagTMAPINFO						TMAPINFO, *LPTMAPINFO;
typedef struct tagMAPOBJ						MAPOBJ, *LPMAPOBJ;
typedef struct tagTSPAWNDATA				TSPAWNDATA, *LPTSPAWNDATA;
typedef struct tagTRESULT						TRESULT, *LPTRESULT;
typedef struct tagTSORTEDUNIT					TSORTEDUNIT, *LPTSORTEDUNIT;
typedef struct tagTSORTEDMAP					TSORTEDMAP, *LPTSORTEDMAP;

typedef map< DWORD, LPD3DXVECTOR3>					MAPD3DXVECTOR3, *LPMAPD3DXVECTOR3;
typedef vector<CTMonSpawnInspectorObjBase *>		VTOBJBASE, *LPVTOBJBASE;
typedef vector<LPMAPOBJ>										VTMAPOBJ, *LPVTMAPOBJ;
typedef vector<struct tagTSPAWNDATA>					VTSPAWNDATA, *LPVTSPAWNDATA;
typedef vector<struct tagTRESULT>							VTRESULT, *LPVTRESULT;
typedef vector<struct tagTSORTEDUNIT>					VTSORTEDUNIT, *LPVTSORTEDUNIT;
typedef vector<struct tagTSORTEDMAP>					VTSORTEDMAP, *LPVTSORTEDMAP;
typedef std::list<struct tagTSPAWNDATA>					LTSPAWNDATA, &LPLTSPAWNDATA;
typedef map< DWORD, LPD3DXVECTOR3>					MAPD3DXVECTOR3, *LPMAPD3DXVECTOR3;
typedef map< DWORD, LPTMAPINFO>						MAPTMAPINFO, *LPMAPTMAPINFO;

// struct

typedef enum TCOLLISION_TYPE
{
	TCOLLISION_NONE = 0,
	TCOLLISION_BOX,
	TCOLLISION_CYLINDER,
	TCOLLISION_POLY,
	TCOLLISION_COUNT
} *LPTCOLLISION_TYPE;

struct tagTMAPINFO
{
	MAPD3DXVECTOR3 m_mapTNODE;
	DWORD m_dwMapID;
	BYTE m_bDungeon;

	tagTMAPINFO()
	{
		m_mapTNODE.clear();
		m_dwMapID = 0;

		m_bDungeon = FALSE;
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

struct tagTSPAWNDATA
{
	WORD wSpawnID;
	WORD wMapID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	BYTE bRange;

	BOOL bEmptyLine;
};

struct tagTRESULT
{
	TSPAWNDATA		SpawnData;

	BYTE bResult;
	// 0 : 충돌없음
	// 1 : 오브젝트와 충돌
	// 2 : 옳지 않은 높이값

	FLOAT fResultHeight;
	FLOAT fResultLandHeight;

	D3DXVECTOR3		vRevisedPos;
};

struct tagTSORTEDUNIT
{
	INT						nUnitIndex;
	INT						nUnitX;
	INT						nUnitZ;
	LTSPAWNDATA		lSpawnData;

	tagTSORTEDUNIT( INT _nUnitIndex, INT _nUnitX, INT _nUnitZ )
		: nUnitIndex(_nUnitIndex), nUnitX(_nUnitX), nUnitZ(_nUnitZ) {}
};

struct tagTSORTEDMAP
{
	WORD					wMapID;
	LTSPAWNDATA		lSpawnData;

	tagTSORTEDMAP( WORD _wMapID ) : wMapID(_wMapID) {}
};
