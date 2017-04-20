#include <stdafx.h>

#define TFMT_HTTP_POST_HEADER			"\
POST %s HTTP/1.0\r\n\
Host: %s\r\n\
Referer: %s\r\n\
Content-type: application/x-www-form-urlencoded\r\n\
Content-Length: %d\r\n\r\n"

#define TFMT_HTTP_POST_CONTENT			"\
LOGIN=%s&\
PASSWORD=%s&\
PASSPORT_SERVER_LOGIN=%s&\
PASSPORT_SERVER_PASSWORD=%s"

#define TCONTENT_LENGTH_MARK			"CONTENT-LENGTH"
#define TCONTENT_VALUE_MARK				":"
#define TCONTENT_VALUE_MARK_SIZE		1

#define TCONTENT_NEWLINE_MARK			"\n"
#define TCONTENT_NEWLINE_MARK_SIZE		1

#define TCONTENT_SUCCESS_MARK			"OK"
#define TCONTENT_SUCCESS_MARK_SIZE		2

#define TCONTENT_FAILED_MARK			"FALSE"
#define TCONTENT_FAILED_MARK_SIZE		5

#define TCONTENT_MARK					"\r\n\r\n"
#define TCONTENT_MARK_SIZE				4

#define TNIKITA_WRONG_PASSWD			19

#define TMSG_BASE						20100
#define TMAX_TEXT						1024


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Useage (Parameter) :
// Exec @dwReturn = TGetUserID @szHostURL, @szProcURL, @szRefererURL, @szHostPort, @szGameID, @szGamePW, @szUserID, @szPassword, @dwUserID OUTPUT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum TSP_RESULT
{
	TSP_SUCCESS = 0,
	TSP_WRONG_PARAM,
	TSP_SOCK_INIT_FAILED,
	TSP_SOCK_CONNECT_FAILED,
	TSP_BUILD_MSG_FAILED,
	TSP_SOCK_SEND_FAILED,
	TSP_SOCK_RECV_FAILED,
	TSP_WRONG_REQUEST,
	TSP_WRONG_USERID,
	TSP_WRONG_PASSWD
};

enum TPARAM_INDEX
{
	TPARAM_HOST_URL = 0,
	TPARAM_PROC_URL,
	TPARAM_REFERER_URL,
	TPARAM_HOST_PORT,
	TPARAM_GAMEID,
	TPARAM_GAMEPW,
	TPARAM_USERID,
	TPARAM_PASSWD,
	TPARAM_SERIAL,
	TPARAM_COUNT
};

enum THTTP_RESULT
{
	THTTP_SUCCESS = 0,
	THTTP_INIT_FAILED,
	THTTP_CONNECT_FAILED,
	THTTP_BUILD_MSG_FAILED,
	THTTP_SEND_FAILED,
	THTTP_RECV_FAILED,
	THTTP_WRONG_REQUEST,
	THTTP_WRONG_USERID,
	THTTP_WRONG_PASSWD
};

typedef struct tagTPARAM_DATA
{
	BYTE m_szPARAM[TPARAM_COUNT - 1][TMAX_TEXT];
	DWORD m_dwSerial;
} TPARAM_DATA, *LPTPARAM_DATA;


#ifdef __cplusplus
extern "C" {
#endif

RETCODE __declspec(dllexport) TGetUserID( SRV_PROC *srvproc);

#ifdef __cplusplus
}
#endif


BYTE TLoadPARAM( SRV_PROC *srvproc, LPBYTE szPARAM, int nIndex);
void TPrintMSG( SRV_PROC *srvproc, const char* pMSG);

LPBYTE TNextLine( LPBYTE pTHTTP_CONTENT);
BYTE TCloseLine( LPBYTE pTHTTP_CONTENT);

THTTP_RESULT TGetSerial( LPTPARAM_DATA pPARAM);


RETCODE __declspec(dllexport) TGetUserID( SRV_PROC *srvproc)
{
	int nCount = srv_rpcparams(srvproc);

	TPARAM_DATA vPARAM;
	vPARAM.m_dwSerial = 0;

	if( nCount != TPARAM_COUNT )
	{
		TPrintMSG( srvproc, "Wrong parameter.\nUseage : Exec @dwReturn = TGetUserID @szHostURL, @szProcURL, @szRefererURL, @szHostPort, @szGameID, @szGamePW, @szUserID, @szPassword, @dwUserID OUTPUT");
		return TSP_WRONG_PARAM;
	}

	for( int i=0; i<TPARAM_COUNT - 1; i++)
		if(!TLoadPARAM( srvproc, vPARAM.m_szPARAM[i], i))
			return TSP_WRONG_PARAM;

	switch(TGetSerial(&vPARAM))
	{
	case THTTP_INIT_FAILED		:
		TPrintMSG( srvproc, "Socket : Initialize Failed.");
		return TSP_SOCK_INIT_FAILED;

	case THTTP_CONNECT_FAILED	:
		TPrintMSG( srvproc, "Socket : Connect failed.");
		return TSP_SOCK_CONNECT_FAILED;

	case THTTP_BUILD_MSG_FAILED	:
		TPrintMSG( srvproc, "HTTP message is too long.");
		return TSP_BUILD_MSG_FAILED;

	case THTTP_SEND_FAILED		:
		TPrintMSG( srvproc, "Socket : Send failed.");
		return TSP_SOCK_SEND_FAILED;

	case THTTP_RECV_FAILED	:
		TPrintMSG( srvproc, "Socket : Recv failed.");
		return TSP_SOCK_RECV_FAILED;

	case THTTP_WRONG_REQUEST	:
		TPrintMSG( srvproc, "HTTP : Invalid request.");
		return TSP_WRONG_REQUEST;

	case THTTP_WRONG_USERID		:
		TPrintMSG( srvproc, "NIKITA : Cannot find user.");
		return TSP_WRONG_USERID;

	case THTTP_WRONG_PASSWD		:
		TPrintMSG( srvproc, "NIKITA : Wrong password.");
		return TSP_WRONG_PASSWD;

	case THTTP_SUCCESS			: break;
	}

	if( srv_paramsetoutput( srvproc, TPARAM_SERIAL + 1, (LPBYTE) &vPARAM.m_dwSerial, sizeof(vPARAM.m_dwSerial), FALSE) != SUCCEED )
	{
		BYTE szMSG[TMAX_TEXT];

		sprintf( (char *) szMSG, "Invalid parameter. ( Index : %d )", TPARAM_SERIAL + 1);
		TPrintMSG( srvproc, (const char *) szMSG);

		return TSP_WRONG_PARAM;
	}

	return TSP_SUCCESS;
}

THTTP_RESULT TGetSerial( LPTPARAM_DATA pPARAM)
{
	int nSIZE = TMAX_TEXT;
	WSADATA vWSADATA;

	if( WSAStartup( MAKEWORD( 1, 1), &vWSADATA) ||
		LOBYTE(vWSADATA.wVersion) != 1 ||
		HIBYTE(vWSADATA.wVersion) != 1 )
		return THTTP_INIT_FAILED;

	SOCKET hSOCK = INVALID_SOCKET;
	BOOL bAsync = FALSE;

	hSOCK = WSASocket(
		AF_INET,
		SOCK_STREAM,
		0, NULL,
		0, 0);

	if( hSOCK == INVALID_SOCKET )
	{
		WSACleanup();
		return THTTP_INIT_FAILED;
	}

	if( ioctlsocket( hSOCK, FIONBIO, (unsigned long *) &bAsync) == SOCKET_ERROR )
	{
		closesocket(hSOCK);
		WSACleanup();
		hSOCK = INVALID_SOCKET;

		return THTTP_INIT_FAILED;
	}

	if( setsockopt( hSOCK, SOL_SOCKET, SO_RCVBUF, (const char *) &nSIZE, sizeof(int)) == SOCKET_ERROR )
	{
		closesocket(hSOCK);
		WSACleanup();
		hSOCK = INVALID_SOCKET;

		return THTTP_INIT_FAILED;
	}

	hostent *pHostent = gethostbyname((const char *) pPARAM->m_szPARAM[TPARAM_HOST_URL]);
	if(!pHostent)
	{
		closesocket(hSOCK);
		WSACleanup();
		hSOCK = INVALID_SOCKET;

		return THTTP_INIT_FAILED;
	}

	LPBYTE pTBUF = (LPBYTE) inet_ntoa(*((in_addr *) *pHostent->h_addr_list));
	BYTE vTBUF[TMAX_TEXT];
	SOCKADDR_IN vTARGET;

	memset( &vTARGET, 0x00, sizeof(SOCKADDR_IN));
	memset( vTBUF, 0x00, TMAX_TEXT);
	memcpy( vTBUF, pTBUF, strlen((const char *) pTBUF));

	vTARGET.sin_family = AF_INET;
	vTARGET.sin_addr.s_addr = inet_addr((const char *) vTBUF);
	vTARGET.sin_port = htons((u_short) atoi((const char *) pPARAM->m_szPARAM[TPARAM_HOST_PORT]));

	if(connect( hSOCK, (SOCKADDR *) &vTARGET, sizeof(SOCKADDR_IN)))
	{
		closesocket(hSOCK);
		WSACleanup();
		hSOCK = INVALID_SOCKET;

		return THTTP_CONNECT_FAILED;
	}

	BYTE szTCONTENT[TMAX_TEXT];
	BYTE szTHEADER[TMAX_TEXT];

	memset( szTCONTENT, 0x00, TMAX_TEXT);
	memset( szTHEADER, 0x00, TMAX_TEXT);
	memset( vTBUF, 0x00, TMAX_TEXT);

	sprintf((char *) szTCONTENT,
		TFMT_HTTP_POST_CONTENT,
		pPARAM->m_szPARAM[TPARAM_USERID],
		pPARAM->m_szPARAM[TPARAM_PASSWD],
		pPARAM->m_szPARAM[TPARAM_GAMEID],
		pPARAM->m_szPARAM[TPARAM_GAMEPW]);
	nSIZE = (int) strlen((const char *) szTCONTENT);

	sprintf((char *) szTHEADER,
		TFMT_HTTP_POST_HEADER,
		pPARAM->m_szPARAM[TPARAM_PROC_URL],
		pPARAM->m_szPARAM[TPARAM_HOST_URL],
		pPARAM->m_szPARAM[TPARAM_REFERER_URL],
		nSIZE);
	nSIZE += (int) strlen((const char *) szTHEADER);

	if( nSIZE > TMAX_TEXT || nSIZE < 1 || szTCONTENT[TMAX_TEXT - 1] || szTHEADER[TMAX_TEXT - 1] )
	{
		closesocket(hSOCK);
		WSACleanup();
		hSOCK = INVALID_SOCKET;

		return THTTP_BUILD_MSG_FAILED;
	}

	sprintf((char *) vTBUF, "%s%s", szTHEADER, szTCONTENT);
	pTBUF = vTBUF;

	while( nSIZE > 0 )
	{
		int nSENT = send( hSOCK, (const char *) pTBUF, nSIZE, 0);

		if( nSENT < 1 )
		{
			closesocket(hSOCK);
			WSACleanup();
			hSOCK = INVALID_SOCKET;

			return THTTP_SEND_FAILED;
		}

		pTBUF += nSENT;
		nSIZE -= nSENT;
	}

	memset( vTBUF, 0x00, TMAX_TEXT);
	pTBUF = vTBUF;
	nSIZE = 0;

	while(TRUE)
	{
		int nRECV = recv( hSOCK, (char *) pTBUF, TMAX_TEXT - nSIZE, 0);

		if( nRECV == SOCKET_ERROR || nSIZE + nRECV >= TMAX_TEXT )
		{
			closesocket(hSOCK);
			WSACleanup();
			hSOCK = INVALID_SOCKET;

			return THTTP_RECV_FAILED;
		}

		if(!nRECV)
			break;

		pTBUF += nRECV;
		nSIZE += nRECV;
	}

	closesocket(hSOCK);
	WSACleanup();

	strupr((char *) vTBUF);
	hSOCK = INVALID_SOCKET;

	if( nSIZE < 4 ||
		vTBUF[0] != 'H' ||
		vTBUF[1] != 'T' ||
		vTBUF[2] != 'T' ||
		vTBUF[3] != 'P' )
		return THTTP_WRONG_REQUEST;

	LPBYTE pTHTTP_LENGTH = (LPBYTE) strstr(
		(const char *) vTBUF,
		TCONTENT_LENGTH_MARK);

	LPBYTE pTHTTP_CONTENT = (LPBYTE) strstr(
		(const char *) vTBUF,
		TCONTENT_MARK);

	if(pTHTTP_LENGTH)
		pTHTTP_LENGTH = (LPBYTE) strstr( (const char *) pTHTTP_LENGTH, TCONTENT_VALUE_MARK);

	if( !pTHTTP_CONTENT || !pTHTTP_LENGTH || !TCloseLine(pTHTTP_LENGTH) )
		return THTTP_WRONG_REQUEST;

	pTHTTP_LENGTH += TCONTENT_VALUE_MARK_SIZE;
	pTHTTP_CONTENT += TCONTENT_MARK_SIZE;
	nSIZE = 0;
	sscanf( (const char *) pTHTTP_LENGTH, "%d", &nSIZE);

	if( nSIZE < 1 || nSIZE != (int) strlen((const char *) pTHTTP_CONTENT) )
		return THTTP_RECV_FAILED;

	pTHTTP_LENGTH = TNextLine(pTHTTP_CONTENT);
	TCloseLine(pTHTTP_CONTENT);
	nSIZE = 0;

	if(pTHTTP_LENGTH)
		sscanf( (const char *) pTHTTP_LENGTH, "%d", &nSIZE);

	if(memcmp( pTHTTP_CONTENT, TCONTENT_SUCCESS_MARK, TCONTENT_SUCCESS_MARK_SIZE))
		return !memcmp( pTHTTP_CONTENT, TCONTENT_FAILED_MARK, TCONTENT_FAILED_MARK_SIZE) && nSIZE == TNIKITA_WRONG_PASSWD ? THTTP_WRONG_PASSWD : THTTP_WRONG_USERID;

	if(!nSIZE)
		return THTTP_RECV_FAILED;
	pPARAM->m_dwSerial = (DWORD) nSIZE;

	return THTTP_SUCCESS;
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

LPBYTE TNextLine( LPBYTE pTHTTP_CONTENT)
{
	pTHTTP_CONTENT = (LPBYTE) strstr(
		(const char *) pTHTTP_CONTENT,
		TCONTENT_NEWLINE_MARK);

	return pTHTTP_CONTENT ? pTHTTP_CONTENT + TCONTENT_NEWLINE_MARK_SIZE : NULL;
}

BYTE TCloseLine( LPBYTE pTHTTP_CONTENT)
{
	pTHTTP_CONTENT = (LPBYTE) strstr(
		(const char *) pTHTTP_CONTENT,
		TCONTENT_NEWLINE_MARK);

	if(!pTHTTP_CONTENT)
		return FALSE;
	pTHTTP_CONTENT[0] = NULL;

	return TRUE;
}
