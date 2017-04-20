#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////
// TRelaySvr macro

///////////////////////////////////////////////////////////////////////////////////////////////
// TRelaySvr enum



///////////////////////////////////////////////////////////////////////////////////////////////
// TRelaySvr struct define

typedef struct tagTCOMMAND						TCOMMAND,*LPTCOMMAND;
typedef struct tagTCHATLOG						TCHATLOG, *LPTCHATLOG;

///////////////////////////////////////////////////////////////////////////////////////////////
// TRelaySvr class type
class CTRelaySession;
class CTPlayer;
class CTParty;
class CTGuild;
class CTCorps;

///////////////////////////////////////////////////////////////////////////////////////////////
// TRelaySvr vector type
typedef vector< DWORD >							VECDWORD, *LPVECDWORD;
typedef vector< WORD >							VECWORD, *LPVECWORD;
typedef vector< CString >						VECSTRING, *LPVECSTRING;
typedef vector< TCHATLOG >						VCHATLOG, *LPVCHATLOG;

///////////////////////////////////////////////////////////////////////////////////////////////
// TRelaySvr map type
typedef map< DWORD_PTR,CTPlayer* >				MAPPLAYER,*LPMAPPLAYER;
typedef map< CString, CTPlayer* >				MAPPLAYERNAME, *LPMAPPLAYERNAME;


typedef map< WORD, CTParty* >					MAPTPARTY, *LPMAPPARTY;
typedef map< DWORD, CTGuild* >					MAPTGUILD, *LPMAPTGUILD;
typedef map< DWORD, CTCorps* >					MAPTCORPS,*LPMAPTCORPS;

typedef map< DWORD, CTPlayer* >					MAPMEMBER, *LPMAPMEMBER;
typedef map< DWORD, DWORD >						MAPDWORD, *LPMAPDWORD;
typedef map< DWORD, CString >					MAPTSTRING, *LPMAPTSTRING;
typedef map< CString, BYTE>						MAPTSTRBYTE, *LPMAPTSTRBYTE;
typedef map< WORD, MAPPLAYER>					MAPMEETING, *LPMAPMEETING;


///////////////////////////////////////////////////////////////////////////////////////////////
// TRelaySvr type

///////////////////////////////////////////////////////////////////////////////////////////////
// TRelaySvr struct type


struct tagTCOMMAND
{
	WORD	m_wSquadID;
	DWORD	m_dwCharID;
	WORD	m_wMapID;
	BYTE	m_bCommand;
	BYTE	m_bTgType;
	DWORD	m_dwTgObjID;
	WORD	m_wTgPosX;
	WORD	m_wTgPosZ;

	tagTCOMMAND()
	{
		m_wSquadID = 0;
		m_dwCharID = 0;
		m_wMapID = 0;
		m_bCommand = 0;
		m_bTgType = 0;
		m_dwTgObjID = 0;
		m_wTgPosX = 0;
		m_wTgPosZ = 0;
	};
};

struct tagTCHATLOG
{
	DWORD m_dwRecent;
	DWORD m_dwSize;

	tagTCHATLOG()
	{
		m_dwRecent = 0;
		m_dwSize = 0;
	};

	void reset()
	{
		m_dwRecent = 0;
		m_dwSize = 0;
	};
};

///////////////////////////////////////////////////////////////////////////////////////////////
#include "TRelaySession.h"
#include "TPlayer.h"
#include "TParty.h"
#include "TGuild.h"
#include "TCorps.h"
#include "TMiniDump.h"