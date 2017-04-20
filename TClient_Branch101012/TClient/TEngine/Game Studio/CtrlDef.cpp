#include "stdafx.h"
#include "CtrlDef.h"


BOOL TRegisterWindow( WNDCLASS& wndclass)
{
	WNDCLASS _class;

	if(::GetClassInfo( NULL, wndclass.lpszClassName, &_class))
		return TRUE;

	memcpy( &_class, &wndclass, sizeof(WNDCLASS));
	_class.lpszMenuName = NULL;
	_class.hInstance = NULL;
	_class.cbClsExtra = 0;
	_class.cbWndExtra = 0;

	if(!_class.hbrBackground)
		_class.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);

	if(!_class.hCursor)
		_class.hCursor = ::LoadCursor( NULL, IDC_ARROW);

	if(!::AfxRegisterClass(&_class))
		AfxThrowResourceException();

	return TRUE;
}
