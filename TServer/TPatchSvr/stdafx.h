// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef STRICT
#define STRICT
#endif

// 아래 지정된 플랫폼에 우선하는 플랫폼을 대상으로 해야 한다면 다음 정의를 수정하십시오.
// 다른 플랫폼에 사용되는 해당 값의 최신 정보는 MSDN을 참조하십시오.
#ifndef WINVER				// Windows 95 및 Windows NT 4 이상에서만 기능을 사용할 수 있습니다.
#define WINVER 0x0400		// 이 값을 Windows 98 및 Windows 2000 이상을 대상으로 하는 데 적합한 값으로 변경하십시오.
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 이상에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINNT 0x0500	// 이 값을 Windows 2000 이상을 대상으로 하는 데 적합한 값으로 변경하십시오.
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 이상에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINDOWS 0x0410 // 이 값을 Windows Me 이상을 대상으로 하는 데 적합한 값으로 변경하십시오.
#endif

#ifndef _WIN32_IE			// IE 4.0 이상에서만 기능을 사용할 수 있습니다.
#define _WIN32_IE 0x0400	// 이 값을 IE 5.0 이상을 대상으로 하는 데 적합한 값으로 변경하십시오.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 일부 CString 생성자는 명시적으로 선언됩니다.

// ATL의 일반적이며 대개 안전하게 무시되는 일부 경고 메시지 숨김을 해제합니다.
#define _ATL_NO_COM_SUPPORT
#define _ATL_ALL_WARNINGS

#include "resource.h"

#include "TNetLib.h"
#include "NetCode.h"
#include "TPatchType.h"
#include "CTProtocol.h"
#include "DBAccess.h"
