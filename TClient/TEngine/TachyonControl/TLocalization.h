#if (!defined __TLOCALIZATION_H)
#define __TLOCALIZATION_H

#include "TMfcDef.h"


class __TUI_API CTLocalResourceHelper
{
	HINSTANCE m_hInstPrevResource;
	BOOL m_bFree:TRUE;

	void Init( HINSTANCE hInstResourceModule)
	{
		m_hInstPrevResource = ::AfxGetResourceHandle();
		::AfxSetResourceHandle(hInstResourceModule);
		m_bFree = FALSE;
	};

	void Init( LPCTSTR strResourceModulePath)
	{
		HINSTANCE hInstResourceModule = ::GetModuleHandle(strResourceModulePath);
		Init(hInstResourceModule);
	};

public:
#if (defined _AFXDLL && !defined __TUI_WITH_DLLMFC__)
	CTLocalResourceHelper() : m_hInstPrevResource(NULL), m_bFree(TRUE)
	{
		Init(__TUI_MODULE_NAME);
	};
#endif // #if (defined _AFXDLL && !defined __TUI_WITH_DLLMFC__)

	CTLocalResourceHelper( HINSTANCE hInstResourceModule) : m_hInstPrevResource(NULL), m_bFree(TRUE)
	{
		Init(hInstResourceModule);
	};

	CTLocalResourceHelper( LPCTSTR strResourceModulePath) : m_hInstPrevResource(NULL), m_bFree(TRUE)
	{
		Init(strResourceModulePath);
	};

	~CTLocalResourceHelper()
	{
		Free();
	};

	void Free()
	{
		if(m_bFree)
			return;

		::AfxSetResourceHandle(m_hInstPrevResource);
		m_hInstPrevResource = NULL;

		m_bFree = TRUE;
	};
};


#endif // #if (!defined __TLOCALIZATION_H)
