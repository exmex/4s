#pragma once

#include <NetCode.h>
#include <vector>
#include <list>
#include <map>

using namespace std;


#define TEXEC(x)				if(!GetCANCEL()) {##x;}

#define TDEF_FORMAT_PREFIX		_T("dDuUxXfFeEcCsS")
#define TDEF_FORMAT_FLAGS		_T(".+- 0123456789")
#define TDEF_FORMAT_IGNORE		_T("%%")
#define TDEF_FORMAT_HEADER		('%')
#define TDEF_FORMAT_BLANK		(' ')

#define TDEF_FILE_NPCTEMP		_T("TNPCTemp.tcd")

#define TDEF_TAG_ITEM_NAME		_T("/ItemNormal")
#define TDEF_TAG_PREV_SKILL		_T("/PrevLevel")
#define TDEF_TAG_COLON			_T(':')

#define TDEF_REPLACE_FROM		_T("\\n")
#define TDEF_REPLACE_TO			_T("\n")
#define TDEF_SPACE				_T(" ")

#define TDEF_COLON_FROM			_T("  :")
#define TDEF_COLON_TO			_T(" :")

#define TDEF_NEWLINE			_T("<__NL__>")
#define TDEF_WORD				_T("&&&")

#define TDEF_WCHAR_NBS			((WCHAR) 0xFEFF)

#define TDEF_MAX_PROGRESS		((int) 1000)
#define TDEF_BASIC_TSTEP		((int) 26)

#define TDEF_CHAR_CR			((BYTE) 13)
#define TDEF_CHAR_NL			((BYTE) 10)
#define TDEF_CHAR_SIZE			((BYTE) 1)

#define TMONSKILL_COUNT			((BYTE) 4)
#define TMAX_MBCS_SIZE			((int) 16)


typedef enum TDOC_KIND
{
	TDOC_NONE = 0,
	TDOC_TRANSLATTION,
	TDOC_TCLIENT,
	TDOC_INTERFACE,
	TDOC_INFOCHART,
	TDOC_EVENTQUARTER,
	TDOC_COUNTRYINFO,
	TDOC_RACEINFO,
	TDOC_CLASSINFO,
	TDOC_DYNAMICHELP,
	TDOC_HELP,
	TDOC_ACCESSID,
	TDOC_QUEST_OUTPUT,
	TDOC_QUEST_OBJECTIVE,
	TDOC_QUEST_CATEGORY,
	TDOC_QUEST_CHAPTER,
	TDOC_QUEST_CRAFT,
	TDOC_QUEST_MATERIAL,
	TDOC_BATTLE_ZONE,
	TDOC_MONSTER,
	TDOC_SVRMSG,
	TDOC_TOURNAMENT,
	TDOC_TOURNAMENT_SCHEDULE,
	TDOC_COUNT
} *LPTDOC_KIND;

typedef enum TACCESSID_DB_NAME
{
	TACCESSID_ITEMKIND = 0,
	TACCESSID_SKILLTAB,
	TACCESSID_INFOTEXT,
	TACCESSID_COUNT
} *LPTACCESSID_DB_NAME;

typedef enum TQUEST_TEXT_KIND
{
	TQUEST_TEXT_TITLE = 0,
	TQUEST_TEXT_TRIGGER,
	TQUEST_TEXT_COMPLETE,
	TQUEST_TEXT_ACCEPT,
	TQUEST_TEXT_REJECT,
	TQUEST_TEXT_SUMMARY,
	TQUEST_TEXT_NPC,
	TQUEST_TEXT_REPLY,
	TQUEST_TEXT_FINISH,
	TQUEST_TEXT_COUNT = TQUEST_TEXT_FINISH
} *LPTQUEST_TEXT_KIND;

typedef enum TQTERM_TEXT_KIND
{
	TQTERM_TEXT_TERM = 0,
	TQTERM_TEXT_FAILED,
	TQTERM_TEXT_COUNT
} *LPTQTERM_TEXT_KIND;

typedef enum TLANG_CODE
{
	TLANG_CODE_NAME = 0,
	TLANG_CODE_CODE,
	TLANG_CODE_COUNT
} *LPTLANG_CODE;

typedef enum TEVENT_TEXT
{
	TEVENT_TEXT_TITLE = 0,
	TEVENT_TEXT_MSG,
	TEVENT_TEXT_PRESENT,
	TEVENT_TEXT_ANNOUNCE,
	TEVENT_TEXT_COUNT
} *LPTEVENT_TEXT;

typedef enum TINFO_STRING
{
	TINFO_TEXT_ORIG = 0,
	TINFO_TEXT_TRANS,
	TINFO_TEXT_COUNT
} *LPTINFO_TEXT;


typedef struct tagTDICBUFF			TDICBUFF, *LPTDICBUFF;
typedef struct tagTDICDATA			TDICDATA, *LPTDICDATA;
typedef struct tagTBDPARAM			TBDPARAM, *LPTBDPARAM;

typedef map< CString, LPTDICBUFF>	MAPTSTRINGDICBUFF, *LPMAPTSTRINGDICBUFF;
typedef map< DWORD, LPTDICBUFF>		MAPTDWORDDICBUFF, *LPMAPTDWORDDICBUFF;
typedef map< CString, CString>		MAPTRANSLATION, *LPMAPTRANSLATION;
typedef map< DWORD, CString>		MAPNAME, *LPMAPNAME;
typedef map< CString, DWORD>		MAPTID, *LPMAPTID;
typedef map< DWORD, DWORD>			MAPDWORD, *LPMAPDWORD;

typedef vector<LPTDICDATA>			VTDICDATA, *LPVTDICDATA;
typedef vector<CString>				VSTRING, *LPVSTRING;
typedef vector<DWORD>				VDWORD, *LPVDWORD;

typedef list<LPTDICDATA>			LISTTDICDATA, *LPLISTTDICDATA;


struct tagTDICDATA
{
	CString m_strFROM;
	CString m_strTO;

	tagTDICDATA()
	{
		m_strFROM.Empty();
		m_strTO.Empty();
	};
};

struct tagTDICBUFF
{
	VTDICDATA m_vTDIC;

	tagTDICBUFF()
	{
		m_vTDIC.clear();
	};

	~tagTDICBUFF()
	{
		while(!m_vTDIC.empty())
		{
			delete m_vTDIC.back();
			m_vTDIC.pop_back();
		}
	};
};

struct tagTBDPARAM
{
	CWnd *m_pPROGRESS;
	CWnd *m_pHOST;

	tagTBDPARAM()
	{
		m_pPROGRESS = NULL;
		m_pHOST = NULL;
	};
};

struct binary_dic_data : public binary_function< LPTDICDATA, LPTDICDATA, bool>
{
	bool operator () ( const LPTDICDATA &_Left, const LPTDICDATA &_Right) const
	{
		if( _Left->m_strFROM.GetLength() < _Right->m_strFROM.GetLength() )
			return true;

		return false;
	};
};

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
