// TConfigMP.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"		// 주 기호


// CTConfigMPApp:
// 이 클래스의 구현에 대해서는 TConfigMP.cpp을 참조하십시오.
//

class CTConfigMPApp : public CWinApp
{
public:
	CTConfigMPApp();

// Overrides
	public:
	virtual void LoadStdProfileSettings();
	virtual void SaveStdProfileSettings();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTConfigMPApp theApp;
