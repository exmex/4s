#if (!defined __TMFC_DEF_H)
#define __TMFC_DEF_H

#pragma warning ( disable : 4201 )
	#include <mmsystem.h>
#pragma warning ( default : 4201 )

#pragma message("Automatically linking with WinMM library")
#pragma comment( lib, "winmm.lib")


#define __TUI_VERSION	DWORD(0x00000001)

#if (defined _AFXDLL)
	#if (defined __TUI_WITH_DLLMFC__)
		#if (defined _DEBUG)
			#if (defined _UNICODE)
				#define __TUI_LIB_NAME		   "TUIud.lib"
			#elif (defined _MBCS)
				#define __TUI_LIB_NAME		   "TUId.lib"
			#else
				#define __TUI_LIB_NAME		   "TUId.lib"
			#endif // _UNICODE
		#else
			#if (defined _UNICODE)
				#define __TUI_LIB_NAME		   "TUIu.lib"
			#elif (defined _MBCS)
				#define __TUI_LIB_NAME		   "TUI.lib"
			#else
				#define __TUI_LIB_NAME		   "TUI.lib"
			#endif // _UNICODE
		#endif // _DEBUG
	#else
		#if (defined _DEBUG)
			#if (defined _UNICODE)
				#define __TUI_MODULE_NAME		_T("TUIud.dll")
				#define __TUI_LIB_NAME		   "TUIud.lib"
			#elif (defined _MBCS)
				#define __TUI_MODULE_NAME	_T("TUId.dll")
				#define __TUI_LIB_NAME		   "TUId.lib"
			#else
				#define __TUI_MODULE_NAME	_T("TUId.dll")
				#define __TUI_LIB_NAME		   "TUId.lib"
			#endif // _UNICODE
		#else
			#if (defined _UNICODE)
				#define __TUI_MODULE_NAME	_T("TUIu.dll")
				#define __TUI_LIB_NAME		   "TUIu.lib"
			#elif (defined _MBCS)
				#define __TUI_MODULE_NAME	_T("TUI.dll")
				#define __TUI_LIB_NAME		   "TUI.lib"
			#else
				#define __TUI_MODULE_NAME	_T("TUI.dll")
				#define __TUI_LIB_NAME		   "TUI.lib"
			#endif // _UNICODE
		#endif // _DEBUG
	#endif // #if (defined __TUI_WITH_DLLMFC__)
#else
		#if (defined _DEBUG)
			#if (defined _UNICODE)
				#define __TUI_LIB_NAME		   "TUIsud.lib"
			#elif (defined _MBCS)
				#define __TUI_LIB_NAME		   "TUIsd.lib"
			#else
				#define __TUI_LIB_NAME		   "TUIsd.lib"
			#endif // _UNICODE
		#else
			#if (defined _UNICODE)
				#define __TUI_LIB_NAME		   "TUIsu.lib"
			#elif (defined _MBCS)
				#define __TUI_LIB_NAME		   "TUIs.lib"
			#else
				#define __TUI_LIB_NAME		   "TUIs.lib"
			#endif // _UNICODE
		#endif // _DEBUG
#endif // _AFXDLL

#if defined __TUIS_IMPL__
	#if (defined _AFXDLL && !defined __TUI_WITH_DLLMFC__)
		#define __TUI_API _declspec(dllexport)
	#else
		#define __TUI_API
	#endif // _AFXDLL
#else 
	#if (defined _AFXDLL && !defined __TUI_WITH_DLLMFC__)
		#define __TUI_API _declspec(dllimport)
	#else
		#define __TUI_API
	#endif // _AFXDLL

	#pragma message("Automatically linking with TUI library")
	#pragma comment( lib, __TUI_LIB_NAME)
#endif

#endif // #if (!defined __TMFC_DEF_H)