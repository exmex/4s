#pragma once

#include <vector>
#include <map>

using namespace std;


#define TQUESTVIEW_X					240
#define TPATHVIEW_X						259

#define TLINESIZE_DRAW					4
#define TLINESIZE_FIND					3
#define TLINESIZE_PATH					2

#define TLINECLR_DBLOCK					0x007171F9
#define TLINECLR_DPATH					0x00FA7474
#define TLINECLR_BLOCK					0x000A0AF1
#define TLINECLR_PATH					0x00F84B4B
#define TLINECLR_FIND					0x0053F2FF

#define TPOINTSIZE_TARGET				8
#define TPOINTSIZE_PTEST				6
#define TPOINTSIZE_PSEL					6
#define TPOINTSIZE_PATH					5
#define TPOINTSIZE_FIND					5
#define TPOINTSIZE_MON					6

#define TPOINTCLR_TARGET				0x0046F7F2
#define TPOINTCLR_PSTART				0x0046F7F2
#define TPOINTCLR_PEND					0x00FFFFFF
#define TPOINTCLR_PSEL					0x0004EC67
#define TPOINTCLR_PATH					0x00FD999E
#define TPOINTCLR_FIND					0x0004EC67
#define TPOINTCLR_MON					0x00FFFFFF

#define TCHECKMASK_BIT					12

#define MAKET2DPOINT( x, y)				T2DPOINT((T2DPOINT((x) * 100.0f) & 0xFFFFFFFF) | ((T2DPOINT((y) * 100.0f) << 32) & 0xFFFFFFFF00000000))
#define GET2D_Y(l)						(FLOAT(DWORD(T2DPOINT((T2DPOINT(l) >> 32) & 0xFFFFFFFF))) / 100.0f)
#define GET2D_X(l)						(FLOAT(DWORD(T2DPOINT(T2DPOINT(l) & 0xFFFFFFFF))) / 100.0f)

#define MAKET2DPOINTL( x, y)			T2DPOINT((T2DPOINT(x) & 0xFFFFFFFF) | ((T2DPOINT(y) << 32) & 0xFFFFFFFF00000000))
#define GET2DL_Y(l)						DWORD(T2DPOINT((T2DPOINT(l) >> 32) & 0xFFFFFFFF))
#define GET2DL_X(l)						DWORD(T2DPOINT(T2DPOINT(l) & 0xFFFFFFFF))


typedef enum TMAPEXPORT_TYPE
{
	TMAPEXPORT_POINT = 0,
	TMAPEXPORT_LINE,
	TMAPEXPORT_COUNT
} TMAPEXPORT_TYPE, *LPTMAPEXPORT_TYPE;


typedef enum TLINE_TYPE
{
	TLINE_BLOCK = 0,
	TLINE_PATH
} TLINE_TYPE, *LPTLINE_TYPE;


typedef enum TEDITPATH_TYPE
{
	TEDITPATH_TEST_PATH = 0,
	TEDITPATH_ADD_POINT,
	TEDITPATH_MOVE_POINT,
	TEDITPATH_DEL_POINT,
	TEDITPATH_ADD_BLOCK,
	TEDITPATH_ADD_PATH,
	TEDITPATH_DEL_LINE,
	TEDITPATH_PICK_POS,
	TEDITPATH_COUNT
} TEDITPATH_TYPE, *LPTEDITPATH_TYPE;


typedef enum TCHECK_STATUS
{
	TCHECK_NONE = 0,
	TCHECK_CHECK,
	TCHECK_MIXED,
	TCHECK_COUNT
} TCHECK_STATUS, *LPTCHECK_STATUS;


typedef enum TUPDATE_TYPE
{
	TUPDATE_NONE = 0,
	TUPDATE_RESET,
	TUPDATE_NPC,
	TUPDATE_MON,
	TUPDATE_POS
} TUPDATE_TYPE, *LPTUPDATE_TYPE;


typedef enum TPATHWND_TYPE
{
	TPATHWND_MAIN = 0,
	TPATHWND_NPC,
	TPATHWND_MON,
	TPATHWND_POS,
	TPATHWND_MAP,
	TPATHWND_COUNT
} TPATHWND_TYPE, *LPTPATHWND_TYPE;


typedef enum TPANE_INFO
{
	TPANE_NONE = 0,
	TPANE_POS_X,
	TPANE_POS_Z,
	TPANE_SCALE
} TPANE_INFO, *LPTPANE_INFO;


typedef enum __TQITEM_TYPE
{
	TQITEM_CLASS = 0,
	TQITEM_MISSION,
	TQITEM_COUNT
} TQITEM_TYPE, *LPTQITEM_TYPE;


typedef enum __TITEM_LOC_TYPE
{
	TITEM_LOC_NONE = 0,
	TITEM_LOC_NPC,
	TITEM_LOC_MON,
	TITEM_LOC_COUNT
} TITEM_LOC_TYPE, *LPTITEM_LOC_TYPE;


typedef __int64						T2DPOINT, *LPT2DPOINT;

typedef struct tagTBUILDINFO		TBUILDINFO, *LPTBUILDINFO;
typedef struct tagTFINDINFO			TFINDINFO, *LPTFINDINFO;
typedef struct tagTPATHINFO			TPATHINFO, *LPTPATHINFO;

typedef struct tagTMAPDATA			TMAPDATA, *LPTMAPDATA;
typedef struct tagTMAPIMG			TMAPIMG, *LPTMAPIMG;

typedef struct tagTCDATA			TCDATA, *LPTCDATA;
typedef struct tagTQDATA			TQDATA, *LPTQDATA;
typedef struct tagTTDATA			TTDATA, *LPTTDATA;
typedef struct tagTNDATA			TNDATA, *LPTNDATA;
typedef struct tagTMDATA			TMDATA, *LPTMDATA;

typedef struct tagTQITEM			TQITEM, *LPTQITEM;
typedef struct tagTTITEM			TTITEM, *LPTTITEM;


typedef vector<LPTTDATA>			VTTDATA, *LPVTTDATA;
typedef vector<LPTMDATA>			VTMDATA, *LPVTMDATA;
typedef vector<LPTTITEM>			VTTITEM, *LPVTTITEM;
typedef vector<DWORD>				VDWORD, *LPVDWORD;


typedef map< WORD, T2DPOINT>		MAPT2DPOINT, *LPMAPT2DPOINT;
typedef map< T2DPOINT, WORD>		MAPT2DINDEX, *LPMAPT2DINDEX;

typedef map< DWORD, LPVTMDATA>		MAPTMDATA, *LPMAPTMDATA;
typedef map< DWORD, LPTCDATA>		MAPTCDATA, *LPMAPTCDATA;
typedef map< DWORD, LPTQDATA>		MAPTQDATA, *LPMAPTQDATA;
typedef map< DWORD, LPTNDATA>		MAPTNDATA, *LPMAPTNDATA;
typedef map< DWORD, LPTQITEM>		MAPTQITEM, *LPMAPTQITEM;

typedef map< DWORD, LPMAPTMDATA>	MAPMAPTMDATA, *LPMAPMAPTMDATA;
typedef map< DWORD, LPVDWORD>		MAPVDWORD, *LPMAPVDWORD;
typedef map< DWORD, DWORD>			MAPDWORD, *LPMAPDWORD;
typedef map< DWORD, FLOAT>			MAPFLOAT, *LPMAPFLOAT;

typedef map< WORD, LPTPATHINFO>		MAPTPATHINFO, *LPMAPTPATHINFO;
typedef map< WORD, LPMAPFLOAT>		MAPTPATHDATA, *LPMAPTPATHDATA;

typedef map< DWORD, LPTMAPDATA>		MAPTMAPDATA, *LPMAPTMAPDATA;
typedef map< DWORD, LPTMAPIMG>		MAPTMAPIMG, *LPMAPTMAPIMG;


#include "TQuestPath.h"
#include "TQuestDB.h"
#include "TGameDB.h"

#include "TQuestPathDoc.h"
#include "ProgressDlg.h"


struct tagTBUILDINFO
{
	CTQuestPathDoc *m_pDOC;
	CProgressDlg *m_pWnd;
	CArchive *m_pTAR;

	tagTBUILDINFO()
	{
		m_pTAR = NULL;
		m_pDOC = NULL;
		m_pWnd = NULL;
	};
};


struct tagTFINDINFO
{
	CProgressDlg *m_pWnd;
	CTMapView *m_pHost;

	tagTFINDINFO()
	{
		m_pHost = NULL;
		m_pWnd = NULL;
	};
};


struct tagTPATHINFO
{
	VDWORD m_vTPATH;
	FLOAT m_fDIST;

	tagTPATHINFO()
	{
		m_vTPATH.clear();
		m_fDIST = 0.0f;
	};

	~tagTPATHINFO()
	{
		m_vTPATH.clear();
	};
};


struct tagTMAPIMG
{
	MAPT2DPOINT m_mapTPOINT;
	MAPT2DINDEX m_mapTINDEX;
	MAPDWORD m_mapTLINE;

	union
	{
		DWORD m_dwID;

		struct
		{
			WORD m_wUnitX;
			WORD m_wUnitZ;
		};
	} m_vID;

	struct
	{
		DWORD m_dwSIZE;
		union
		{
			PBITMAPINFOHEADER m_pTHEADER;
			PBITMAPINFO m_pTINFO;
			LPBYTE m_pTBUF;
		};
	} m_vTBMPINFO;

	struct
	{
		DWORD m_dwSIZE;
		union
		{
			LPVOID m_pTDATA;
			LPBYTE m_pTBUF;
		};
	} m_vTBMPBITS;

	tagTMAPIMG()
	{
		m_vTBMPINFO.m_pTBUF = NULL;
		m_vTBMPBITS.m_pTBUF = NULL;

		m_vTBMPINFO.m_dwSIZE = 0;
		m_vTBMPBITS.m_dwSIZE = 0;
		m_vID.m_dwID = 0;

		m_mapTPOINT.clear();
		m_mapTINDEX.clear();
		m_mapTLINE.clear();
	};

	~tagTMAPIMG()
	{
		if(m_vTBMPINFO.m_pTBUF)
			delete[] m_vTBMPINFO.m_pTBUF;

		if(m_vTBMPBITS.m_pTBUF)
			delete[] m_vTBMPBITS.m_pTBUF;

		m_mapTPOINT.clear();
		m_mapTINDEX.clear();
		m_mapTLINE.clear();
	};
};


struct tagTMAPDATA
{
	MAPTMAPIMG m_mapTUNIT;
	DWORD m_dwID;

	DWORD m_dwUnitX;
	DWORD m_dwUnitZ;

	tagTMAPDATA()
	{
		m_mapTUNIT.clear();
		m_dwID = 0;

		m_dwUnitX = 0;
		m_dwUnitZ = 0;
	};

	~tagTMAPDATA()
	{
		MAPTMAPIMG::iterator it;

		for( it = m_mapTUNIT.begin(); it != m_mapTUNIT.end(); it++)
			delete (*it).second;

		m_mapTUNIT.clear();
	};
};


struct tagTMDATA
{
	CString m_strTITLE;
	CString m_strNAME;

	WORD m_wMonID;
	WORD m_wMapID;
	WORD m_wKind;

	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	tagTMDATA()
	{
		m_strTITLE.Empty();
		m_strNAME.Empty();

		m_wMonID = 0;
		m_wMapID = 0;
		m_wKind = 0;

		m_fPosX = 0.0f;
		m_fPosY = 0.0f;
		m_fPosZ = 0.0f;
	};
};


struct tagTNDATA
{
	CString m_strTITLE;
	CString m_strNAME;

	WORD m_wMapID;
	WORD m_wID;

	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	tagTNDATA()
	{
		m_strTITLE.Empty();
		m_strNAME.Empty();

		m_wMapID = 0;
		m_wID = 0;

		m_fPosX = 0.0f;
		m_fPosY = 0.0f;
		m_fPosZ = 0.0f;
	};
};


struct tagTCDATA
{
	CString m_strNAME;
	DWORD m_dwClassID;

	tagTCDATA()
	{
		m_strNAME.Empty();
		m_dwClassID = 0;
	};
};


struct tagTTDATA
{
	CString m_strTermMSG;

	DWORD m_dwTermID;
	BYTE m_bTermType;

	tagTTDATA()
	{
		m_strTermMSG.Empty();

		m_bTermType = 0;
		m_dwTermID = 0;
	};
};


struct tagTQDATA
{
	CString m_strCompleteMSG;
	CString m_strTriggerMSG;
	CString m_strTITLE;

	DWORD m_dwTriggerID;
	DWORD m_dwQuestID;
	DWORD m_dwClassID;

	BYTE m_bTriggerType;
	BYTE m_bType;

	VTTDATA m_vTTERM;

	tagTQDATA()
	{
		m_strCompleteMSG.Empty();
		m_strTriggerMSG.Empty();
		m_strTITLE.Empty();

		m_dwTriggerID = 0;
		m_dwClassID = 0;
		m_dwQuestID = 0;

		m_bTriggerType = 0;
		m_bType = 0;

		m_vTTERM.clear();
	};

	~tagTQDATA()
	{
		while(!m_vTTERM.empty())
		{
			delete m_vTTERM.back();
			m_vTTERM.pop_back();
		}
	};
};


struct tagTTITEM
{
	TTDATA m_vTTERM;

	DWORD m_dwMonKind;
	DWORD m_dwNpcID;
	DWORD m_dwMapID;
	BYTE m_bLOCK;

	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	tagTTITEM()
	{
		m_vTTERM.m_strTermMSG.Empty();

		m_vTTERM.m_bTermType = 0;
		m_vTTERM.m_dwTermID = 0;

		m_dwMonKind = 0;
		m_dwNpcID = 0;
		m_dwMapID = 0;
		m_bLOCK = FALSE;

		m_fPosX = 0.0f;
		m_fPosY = 0.0f;
		m_fPosZ = 0.0f;
	};
};


struct tagTQITEM
{
	HTREEITEM m_itemPOS;
	VTTITEM m_vTTERM;

	DWORD m_dwID;
	BYTE m_bType;

	LPTQDATA m_pComplete;
	LPTQDATA m_pMission;

	tagTQITEM()
	{
		m_vTTERM.clear();
		m_itemPOS = NULL;

		m_bType = TQITEM_CLASS;
		m_dwID = 0;

		m_pComplete = NULL;
		m_pMission = NULL;
	};

	~tagTQITEM()
	{
		while(!m_vTTERM.empty())
		{
			delete m_vTTERM.back();
			m_vTTERM.pop_back();
		}

		if(m_pComplete)
		{
			delete m_pComplete;
			m_pComplete = NULL;
		}

		if(m_pMission)
		{
			delete m_pMission;
			m_pMission = NULL;
		}
	};
};
