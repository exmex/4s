#pragma once

#include <winsock2.h>
#include <afxsock.h>

#include <map>
#include <vector>
#include <string>
#include <queue>
#include <deque>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////
typedef map<BYTE, CMultiDocTemplate*>	MAPDOCTEMP;			// Doc Template
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// File Name Connect
#define FILE_NAME_CONNECT _T("SvrTest.log")
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// Protocol Type Enum
#define TYPE_BYTE		0
#define TYPE_WORD		1
#define TYPE_DWORD		2
#define TYPE_FLOAT		3
#define TYPE_STRING		4
#define TYPE_INT64		5
#define TYPE_SOCKADDR	6
#define TYPE_LOOPSTART	7
#define TYPE_LOOPEND	8

typedef map<DWORD, CString>				MAPPROTOCOL;		// Protocol
typedef vector<DWORD>					VECTORTYPE;			// Protocol Type Vector 
typedef map<DWORD, VECTORTYPE>			MAPPROTOCOLTYPE;	// Protocol Type Map

typedef struct tagSTRING				STRUCTSTRING;		// Struct List
struct tagSTRING
{
	CString strProtocol;
	CString strValue;
};
typedef deque<STRUCTSTRING>				DEQUELIST;			// List Deque

// Char Info Enum
#define CHAR_USERID		0
#define CHAR_CHARID		1
#define CHAR_KEY		2
#define CHAR_GROUPID	3
#define CHAR_CHANNEL	4
#define CHAR_SVRID		5
#define MAX_CHARINFO	6
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
typedef map<DWORD, CTachyonSession*>	MAPPSESSION;		// TachyonSession Map
typedef vector<DWORD>					VECTORTHREAD;		// Thread ID Vector

typedef map<DWORD, HANDLE>				MAPEVENTHANDLE;		// Event Handle Map
typedef map<DWORD, CTachyonSession *>	MAPTHREADSESSION;	// Thread Handle Map

typedef struct tagList					STRUCTLIST;			// Receive Data List
struct tagList
{	
	CTachyonSession* pSession;
	CString strP;
	CString data;
};
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
typedef map<DWORD, CTMacro *>			MAPMACRO;			// Macro Map

typedef struct tagParam					STRUCTPARAM;		// Macro Paramater Struct
struct tagParam
{	
	CString strName;
	CString strType;
	CString strData;
};
typedef vector<STRUCTPARAM>				VECTORPARAM;		// Macro Paramater Vector

typedef struct tagProtocolInfo			STRUCTPINFO;		// Macro Protocol Info Struct
struct tagProtocolInfo
{
	DWORD dwPID;
	CString strName;
    CString strParent;		
	BOOL bSleep;
	int sleep;
	BOOL bSelf;
    VECTORPARAM vParam;
};
typedef vector<STRUCTPINFO>				VECTORPINFO;		// Macro Protocol Info Vector
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
typedef vector<CString>					VECTORSTRING;		// CString Vector
typedef vector<DWORD>					VECTORDWORD;		// DWORD Vector
typedef queue<DWORD>					QUEUEDWORD;			// DWORD Queue
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
typedef map<DWORD, VECTORPARAM>			MAPSESSIONPARAM;	// Session Macro Param Map
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
typedef vector<STRUCTPINFO>				VECTORSRINFO;		// Session Macro Info Send / Receive Vector
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagSelfInfo				STRUCTSELFINFO;		// Macro Self Info Struct
struct tagSelfInfo
{
    DWORD dwPID;
	CString strName;
    int pos;
};
typedef vector<STRUCTSELFINFO>			VECTORSELFINFO;		// Macro Self Info Vector
/////////////////////////////////////////////////////////////////////////////////////////////