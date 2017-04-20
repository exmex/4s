#include <stdafx.h>
#include "md5.h"


#define TMSG_BASE						20200
#define TMAX_TEXT						1024

//////////////////////////////////////////
// Useage (Parameter) :
// Exec @dwReturn = TMD5 @szParam OUTPUT
//////////////////////////////////////////

enum TMD5_RESULT
{
	TMD5_SUCCESS = 0,
	TMD5_WRONG_PARAM,
	TMD5_CONV_FAILED
};

enum TPARAM_INDEX
{
	TPARAM_TARGET = 0,
	TPARAM_COUNT
};

#ifdef __cplusplus
extern "C" {
#endif

RETCODE __declspec(dllexport) TMD5(SRV_PROC *srvproc);

#ifdef __cplusplus
}
#endif


BYTE TLoadPARAM( SRV_PROC *srvproc, LPBYTE szPARAM, int nIndex);
void TPrintMSG( SRV_PROC *srvproc, const char* pMSG);


RETCODE __declspec(dllexport) TMD5( SRV_PROC *srvproc)
{
	int nCount = srv_rpcparams(srvproc);

	BYTE szPARAM[TMAX_TEXT];
	MD5 vTMD5;

	if( nCount != TPARAM_COUNT )
	{
		TPrintMSG( srvproc, "Wrong parameter.\nUseage : Exec @dwReturn = TMD5 @szParam OUTPUT");
		return TMD5_WRONG_PARAM;
	}

	if(!TLoadPARAM( srvproc, szPARAM, TPARAM_TARGET))
		return TMD5_WRONG_PARAM;

	vTMD5.update( szPARAM, (unsigned int) strlen((const char *) szPARAM));
	vTMD5.finalize();
	LPBYTE pTMD5 = (LPBYTE) vTMD5.hex_digest();

	if(!pTMD5)
	{
		TPrintMSG( srvproc, "MD5 Convert failed.");
		return TMD5_CONV_FAILED;
	}

	if( srv_paramsetoutput( srvproc, TPARAM_TARGET + 1, pTMD5, (unsigned int) strlen((const char *) pTMD5), FALSE) != SUCCEED )
	{
		BYTE szMSG[TMAX_TEXT];

		sprintf( (char *) szMSG, "Invalid parameter. ( Index : %d )", TPARAM_TARGET + 1);
		TPrintMSG( srvproc, (const char *) szMSG);
		delete[] pTMD5;

		return TMD5_WRONG_PARAM;
	}
	delete[] pTMD5;

	return TMD5_SUCCESS;
}

BYTE TLoadPARAM( SRV_PROC *srvproc, LPBYTE szPARAM, int nIndex)
{
	LPVOID pPARAM = srv_paramdata( srvproc, nIndex + 1);
	int nCount = srv_paramlen( srvproc, nIndex + 1);

	if( nCount > TMAX_TEXT - 1 || nCount < 1 || !pPARAM )
	{
		BYTE szMSG[TMAX_TEXT];

		sprintf((char *) szMSG, "Invalid parameter. ( Index : %d )", nIndex + 1);
		TPrintMSG( srvproc, (const char *) szMSG);

		return FALSE;
	}

	memset( szPARAM, 0, TMAX_TEXT);
	memcpy( szPARAM, pPARAM, nCount);

	return TRUE;
}

void TPrintMSG( SRV_PROC *srvproc, const char* pMSG)
{
	srv_sendmsg(
		srvproc,
		SRV_MSG_ERROR,
		TMSG_BASE,
		SRV_INFO, 1,
		NULL, 0,
		(DBUSMALLINT) __LINE__,
		(char *) pMSG,
		SRV_NULLTERM);

	srv_senddone(
		srvproc,
		SRV_DONE_ERROR|
		SRV_DONE_MORE,
		0, 0);
}
