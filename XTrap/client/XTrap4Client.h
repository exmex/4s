
// ***************************************************************
//  X-Trap Interface Library For Client
//  -------------------------------------------------------------
//  Copyright (C)WiseLogic 2005 - All Rights Reserved
// ***************************************************************

#ifndef __XTRAPAPI_CLIENT_H
#define __XTRAPAPI_CLIENT_H



/////////////////////////////////////////////////
// X-Trap Client Interface
// Required
/////////////////////////////////////////////////

VOID XTrap_C_Start( 
		IN LPCSTR	lpArgv, 
		IN LPCSTR	lpXTrapPath 
		);

VOID XTrap_C_KeepAlive();

VOID XTrap_C_SetUserInfo( 
		IN LPCSTR	lpUserID,
		IN LPCSTR	lpServerName,
		IN LPCSTR	lpCharacterName,
		IN LPCSTR	lpCharacterClass,
		IN DWORD	dwCharacterLevel
		);



/////////////////////////////////////////////////
// X-Trap Client-Server Interface
// Required
/////////////////////////////////////////////////

// Buffer Size
#define XTRAP_CS4_COMSIZE_PACKDATA					128
#define XTRAP_CS4_BUFSIZE_SESSION					320
#define XTRAP_CS4_BUFSIZE_SESSIONADDON				80
#define	XTRAP_CS4_BUFSIZE_MAP						45000

// Timeout Limit
#define XTRAP_CS4_MAXTIMEOUT						(int)600
#define XTRAP_CS4_MINTIMEOUT						(int)100

// Return Value
#define XTRAP_API_RETURN_OK							0x00000000
#define XTRAP_API_RETURN_ERROR						0x00000001
#define XTRAP_API_RETURN_UNKNOWN					0x00000002
#define XTRAP_API_RETURN_INVALID_PARAMETER			0x00000003
#define XTRAP_API_RETURN_INVALID_CRC				0x00000004
#define XTRAP_API_RETURN_TIMEOUT					0x00000005
#define XTRAP_API_RETURN_DETECTHACK					0x0000000F

// Protect Option
#define	XTRAP_PROTECT_PE							0x00000001
#define	XTRAP_PROTECT_TEXT							0x00000010
#define	XTRAP_PROTECT_RDATA							0x00000100
#define	XTRAP_PROTECT_EDATA							0x00001000
#define	XTRAP_PROTECT_RSRC							0x00010000
#define	XTRAP_PROTECT_RELOC							0x00100000
#define XTRAP_PROTECT_EXCEPT_VIRUS					0x00000002


// Old Macro
#define XTRAP_CC4_BUFFERSIZE_COMMUNICATION_BASE		XTRAP_CS4_COMSIZE_PACKDATA
#define XTRAP_CC4_BUFFERSIZE_SESSION_BASE			XTRAP_CS4_BUFSIZE_SESSION
#define XTRAP_CC4_BUFFERSIZE_SESSION_ADDON			XTRAP_CS4_BUFSIZE_SESSIONADDON
#define	XTRAP_CC4_FILESIZE_MAP						XTRAP_CS4_BUFSIZE_MAP

#define XTRAP_CC_MAXTIMEOUT							XTRAP_CS4_MAXTIMEOUT
#define XTRAP_CC_MINTIMEOUT							XTRAP_CS4_MINTIMEOUT

#define XTRAP_CC_RETURN_OK							XTRAP_API_RETURN_OK
#define XTRAP_CC_RETURN_ERROR						XTRAP_API_RETURN_ERROR
#define XTRAP_CC_RETURN_UNKNOWN						XTRAP_API_RETURN_UNKNOWN
#define XTRAP_CC_RETURN_INVALID_PARAMETER			XTRAP_API_RETURN_INVALID_PARAMETER
#define XTRAP_CC_RETURN_INVALID_CRC					XTRAP_API_RETURN_INVALID_CRC
#define XTRAP_CC_RETURN_TIMEOUT						XTRAP_API_RETURN_TIMEOUT
#define XTRAP_CC_RETURN_DETECTHACK					XTRAP_API_RETURN_DETECTHACK

// Ver4
DWORD __cdecl XTrap_CS_Step2(
		IN  LPCVOID	lpBufPackData_IN, 
		OUT LPVOID	lpBufPackData_OUT,
		IN	DWORD	dwMethod);


#endif
