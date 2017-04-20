#if (!defined __CTRLDEF_H)
#define __CTRLDEF_H


extern BOOL TRegisterWindow( WNDCLASS& wndclass);

#define DECLARE_REGISTER_WINDOW(class_name)															\
	DECLARE_DYNAMIC(class_name)																		\
public:																								\
	static BOOL TRegisterClass();																	\
protected:																							\
	static LRESULT CALLBACK TWndClassProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);		\
	static BOOL m_bRegistered;																		\
	virtual void PostNcDestroy();


#define IMPLEMENT_REGISTER_WINDOW( class_name, base_class, dwStyle, hICON, hCURSOR, hBackground)	\
IMPLEMENT_DYNAMIC( class_name, base_class)															\
																									\
BOOL class_name::TRegisterClass()																	\
{																									\
	LPCTSTR pszName = #class_name;																	\
	WNDCLASS wndclass;																				\
																									\
	wndclass.style = dwStyle;																		\
	wndclass.lpfnWndProc = TWndClassProc;															\
	wndclass.hIcon = (HICON) hICON;																	\
	wndclass.hCursor = (HCURSOR) hCURSOR;															\
	wndclass.hbrBackground = (HBRUSH) hBackground;													\
	wndclass.lpszClassName = pszName;																\
																									\
	return TRegisterWindow(wndclass);																\
}																									\
																									\
LRESULT CALLBACK class_name::TWndClassProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		\
{																									\
	switch(LOWORD(msg))																				\
	{																								\
	case WM_CREATE	:																				\
		{																							\
			class_name *pWnd = new class_name();													\
																									\
			if(pWnd->IsKindOf(RUNTIME_CLASS(CDialog)))												\
			{																						\
				pWnd->Create( class_name::IDD, FromHandle(hWnd));									\
				pWnd->SetDlgCtrlID(class_name::IDD);												\
				return ::DefWindowProc( hWnd, msg, wParam, lParam);									\
			}																						\
			else																					\
			{																						\
				pWnd->Attach(hWnd);																	\
				::SetWindowLong( hWnd, GWL_WNDPROC, (LONG) (DWORD_PTR) AfxWndProc);					\
				return AfxWndProc( hWnd, msg, wParam, lParam);										\
			}																						\
		}																							\
																									\
		break;																						\
	}																								\
																									\
	return ::DefWindowProc( hWnd, msg, wParam, lParam);												\
}																									\
																									\
void class_name::PostNcDestroy()																	\
{																									\
	delete this;																					\
}																									\
																									\
BOOL class_name::m_bRegistered = class_name::TRegisterClass();


#endif // #if (!defined __CTRLDEF_H)
