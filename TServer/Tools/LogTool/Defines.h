////////////////////////////////////////////////////////////////////////////////
//	File Name	:	Defines.h
//
//	Copyright	:	(c) Zemi Interactive, Inc.	2002-2009
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../../../TProtocol/LogPacket.h"

#define TIMEOUT								10

#define SQLQUERY_BASE						"SELECT LT_SEQ, LT_LOGDATE, LT_SERVERID, LT_CLIENTIP, LT_ACTION, LT_MAPID, LT_X, LT_Y, LT_Z, LT_DWKEY1, LT_DWKEY2, LT_DWKEY3, LT_DWKEY4, LT_DWKEY5, LT_DWKEY6, LT_DWKEY7, LT_DWKEY8, LT_DWKEY9, LT_DWKEY10, LT_DWKEY11, LT_KEY1, LT_KEY2, LT_KEY3, LT_KEY4, LT_KEY5, LT_KEY6, LT_KEY7, LT_FMT, convert(char(512), LT_LOG) FROM ITEMLOGTL"
#define SQLQUERY_ORDERBY					"ORDER BY LT_SEQ"




#define NUM_COLUMNS							27		//	colum ¼ýÀÚ
#define NUM_DAYS							10


#define NAME_MENU_CONNECT					"Connect"
#define NAME_MENU_SAVE						"Save"
#define NAME_MENU_SEARCH					"Search"


#define RESULT_SUCCESS_CONNECT				"Success to connect to DB"
#define RESULT_FAIL_CONNECT					"Fail to connect to DB"
#define RESULT_ASK_CONNECT					"Please, connect to DB"
#define RESULT_SUCCESS_RECORDSET			"Success to get record sets"
#define RESULT_FAIL_RECORDSET				"Fail to get record sets"
#define RESULT_FAIL_NORECORDSET				"No record sets found under the conditions given"
#define RESULT_FAIL_LONGPERIOD				"Cannot exceed more than 10 days long"
#define RESULT_FAIL_CANCEL					""
#define RESULT_FAIL_OPEN					"Fail to open record sets"
#define RESULT_FAIL_NEEDLOCATION			"Please, input the correct location corresponding to coordinates"
#define RESULT_FAIL_NEEDNUMBER				"Please, input coordinates"
#define RESULT_FAIL_NEEDITEMINFO			"Please, input the type of a item"

#define FORM_DATETIME						"%y-%m-%d %H:%M:%S"
#define FORM_ASKTOSAVE						"Do you want to save?"
#define FORM_ASKTOSAVE_LOG_COLUMN			"Do you want to save log column ?"


#define NAME_LOG_SERVER						"Server ID : %d"
#define NAME_LOG_CLIENT						"Client IP : %s"
#define NAME_LOG_ACTION						"Action : %s"
#define NAME_LOG_LOG						"Log : "


typedef struct  
{
	char szServName[100];
	char szLogServAddr[30];
	char szLogServDSN[30];
	char szGameServAddr[30];
	char szChartServAddr[30];
}SERVERINFO, *LPSERVERINFO;


typedef struct 
{
	int			Seq;

	struct tm	tTimeStamp;				//	Time Stamp
	DWORD		dwServerID;				//	Server ID
	CHAR		szClientIP[16];			//	Client		
	DWORD		dwAction;				//	Log Command

	WORD		wMapID;					//	Map ID
	DWORD		dwX;					//		X
	DWORD		dwY;					//		Y
	DWORD		dwZ;					//		Z

	__int64		dwKey[11];				//	Search Key
	CHAR		szKey[7][50];			//	Search Key	(String)
	DWORD		dwFormat;				//	Log Format
	CHAR		szLog[512];	

}RECORDSET, *LPRECORDSET;


typedef enum 
{
	LT_SEQ,	
	LT_LOGDATE,
	LT_ACTION,
	LT_CLIENTIP,
	LT_MAPID,
	LT_DK1,
	LT_DK2,
	LT_DK3,
	LT_DK4,
	LT_DK5,
	LT_DK6,
	LT_DK7,
	LT_DK8,
	LT_DK9,
	LT_DK10,
	LT_DK11,
	LT_KEY1,
	LT_KEY2,
	LT_KEY3,
	LT_KEY4,
	LT_KEY5,	
	LT_KEY6,
	LT_KEY7,
	LT_LOG,
}DBFIELDIDX;

typedef enum 
{
	ITEMTYPE_NORMAL,
	ITEMTYPE_MAGIC,
	ITEMTYPE_RARE,
	ITEMTYPE_QUESTSCROLL,
	ITEMTYPE_SET,
	ITEMTYPE_QUEST,
}ITEMTYPEIDX;

typedef struct 
{
	DWORD	Idx;
	CTime	value1;
	DWORD	value2;
	TCHAR	value3[256];
	__int64 value4;
}QUERY_CONDITION, *LPQUERY_CONDITION;







