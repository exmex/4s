#pragma once

#include <vector>
#include <map>

using namespace std;


typedef enum TERM_EXT_TYPE
{
	QTT_TSTART_POS = QTT_QUESTCOMPLETED + 1,
	QTT_TCOMP_POS
} TERM_EXT_TYPE, *LPTERM_EXT_TYPE;


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


typedef map< DWORD, LPVTMDATA>		MAPTMDATA, *LPMAPTMDATA;
typedef map< DWORD, LPTCDATA>		MAPTCDATA, *LPMAPTCDATA;
typedef map< DWORD, LPTQDATA>		MAPTQDATA, *LPMAPTQDATA;
typedef map< DWORD, LPTNDATA>		MAPTNDATA, *LPMAPTNDATA;
typedef map< DWORD, LPTQITEM>		MAPTQITEM, *LPMAPTQITEM;

typedef map< DWORD, LPMAPTMDATA>	MAPMAPTMDATA, *LPMAPMAPTMDATA;
typedef map< DWORD, LPVDWORD>		MAPVDWORD, *LPMAPVDWORD;


#include "TQuestDB.h"
#include "TGameDB.h"


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

	DWORD m_dwNpcID;
	DWORD m_dwMapID;

	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	tagTTITEM()
	{
		m_vTTERM.m_strTermMSG.Empty();

		m_vTTERM.m_bTermType = 0;
		m_vTTERM.m_dwTermID = 0;

		m_dwNpcID = 0;
		m_dwMapID = 0;

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
