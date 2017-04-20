// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once


// 여기에 헤더를 삽입합니다.
#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

// ODS 헤더가 포함됩니다.
#ifdef __cplusplus
extern "C" {
#endif 

#include <Srv.h>		// 다른 모든 헤더 파일이 들어 있는 기본 헤더 파일입니다.

#ifdef __cplusplus
}
#endif
