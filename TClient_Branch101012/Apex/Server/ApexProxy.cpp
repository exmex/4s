#include "stdafx.h"
#include "ApexProxy.h"

#if !defined(no_anti_hook)

#include <iostream>
#ifdef WIN32
#else
#include <dlfcn.h>
#endif

typedef CH_RESULT   SH_START_PROC(_FUNC_S_SEND pfSend , _FUNC_S_REC & pfRec);
typedef void		SH_DONE_PROC(void);

typedef void		SetFunc_Proc(void * pFunc,int nFuncFlag);

static SH_START_PROC *g_sh_start = NULL;
static SH_DONE_PROC *g_sh_exit = NULL;


static SetFunc_Proc *g_sh_setfunc=NULL;

#ifdef WIN32
static HMODULE g_hASclient = NULL;
#else
void  *g_hASclient=NULL;     
#endif


static void
cleanup_sh()
{
	if (g_sh_exit )
	{
		g_sh_exit();
	}

 
	if(NULL!=g_hASclient)
#ifdef WIN32
		FreeLibrary(g_hASclient);
#else
			dlclose(g_hASclient);
#endif

	g_hASclient=NULL;
	g_sh_exit = NULL;
	g_sh_start = NULL;

	g_sh_setfunc=NULL;
}

extern "C"  CH_RESULT   CHSStart( _FUNC_S_SEND pfSend , _FUNC_S_REC & pfRec)
{	
#ifdef WIN32
	if (!g_hASclient) {
		g_hASclient = LoadLibrary("ApexProxy.dll");
		if (!g_hASclient) 
			return -2;
		g_sh_exit = (SH_DONE_PROC *) GetProcAddress(g_hASclient, "sh_load_exit");
		g_sh_start = (SH_START_PROC *) GetProcAddress(g_hASclient, "sh_load_start");

 		g_sh_setfunc = (SetFunc_Proc *) GetProcAddress(g_hASclient, "sh_setfunc");
	}
#else
  	if (!g_hASclient) {
 		g_hASclient = dlopen("./ApexProxy.so",RTLD_NOW );
		const char *dlError = dlerror();
		if( dlError ) 
		{
			std::cout<<dlError;
 		}
		if (!g_hASclient) 
			return -2;
  		g_sh_exit = (SH_DONE_PROC *) dlsym(g_hASclient, "sh_load_exit");
		if( dlError ) 
		{
			std::cout<<dlError;
 		}
		g_sh_start = (SH_START_PROC *) dlsym(g_hASclient, "sh_load_start");

		if( dlError ) 
		{
			std::cout<<dlError;
 		}
 		g_sh_setfunc = (SetFunc_Proc *) dlsym(g_hASclient, "sh_setfunc");
		if( dlError ) 
		{
			std::cout<<dlError;
 		}
	}
 #endif
	if (!g_sh_exit || !g_sh_start ||!g_sh_setfunc)
	{
 		if(!g_hASclient)
#ifdef WIN32
			FreeLibrary(g_hASclient);
#else
			dlclose(g_hASclient);
#endif
		g_hASclient=NULL;
		g_sh_exit = NULL;
		g_sh_start = NULL;

 		g_sh_setfunc=NULL;
		return -3;
	}
 	return g_sh_start(pfSend,pfRec);
}
extern "C" void   CHSEnd()
{
	cleanup_sh();
}
extern "C" void   CHSSetFunc(void * pFunc,int nFuncFlag)
{
	if(g_sh_setfunc)
		g_sh_setfunc(pFunc,nFuncFlag);
	return;
}
#endif
