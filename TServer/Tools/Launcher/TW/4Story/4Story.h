// 4Story.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"		// 주 기호


// CStoryApp:
// 이 클래스의 구현에 대해서는 4Story.cpp을 참조하십시오.
//

class CStoryApp : public CWinApp
{
public:
	CStoryApp();

// 재정의
	public:
	virtual BOOL InitInstance();

// 구현

	DECLARE_MESSAGE_MAP()

protected:
	struct SCREENMODE
	{
		DWORD dwWidth;
		DWORD dwHeight;
		CString strMode;
	};

public:
	BYTE m_bOptionLevel;
	vector< SCREENMODE > m_vScreenMode;

public:
	void InsertResolution(DWORD dwWidth,DWORD dwHeight,CString strMode);
};

extern CStoryApp theApp;
