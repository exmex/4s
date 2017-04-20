#ifndef __ApexProxyh__
#define __ApexProxyh__
//////////////////////////////


//#define no_anti_hook


//////////////////////////////
#ifdef WIN32
#include "windows.h"
#else
#include <iostream>
#endif


typedef long CH_RESULT;
#define FLAG_KILLUSER  1002		//The Flag of kill user function;

//Send  and Rec Function type;
typedef long (*_FUNC_S_SEND) (signed int nId,const char * pBuffer,int nLen);
typedef long (*_FUNC_S_REC) (char cMsgId,signed int nId,const char * pBuffer,int nLen);

//Kill user function
typedef long (*_FUNC_S_KILLUSER) (signed int nId ,int Action);


#ifdef __cplusplus
extern "C" {
#endif

	//
	//Start Apex Proxy Server ;
	//
	//Para:
	//pfSend ,Apex will use pfSend to send the data by Game Server;
	//pfRec, When Game Client receive some data that belong to Apex,Game Client need use pfRec tell Apex ;

	extern "C" CH_RESULT   CHSStart(const _FUNC_S_SEND pfSend , _FUNC_S_REC & pfRec);

	//
	//Close Apex Client;
	//
	extern "C" void   CHSEnd();

	//tell Apex Proxy Server some funcions,such as how to kill users;
	//
	//Para:
	//pFunc,the address of Function
	//nFuncFlag,the  Flag of the function,such as FLAG_KILLUSER;
	extern "C" void   CHSSetFunc(void * pFunc,int nFuncFlag);


#ifdef __cplusplus
}
#endif


#endif

