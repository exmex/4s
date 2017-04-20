// 4Story.h : PROJECT_NAME ﾀﾀｿ・ﾇﾁｷﾎｱﾗｷ･ｿ｡ ｴ・?ﾁﾖ ?E・ﾆﾄﾀﾏﾀﾔｴﾏｴﾙ.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCHｿ｡ｼｭ ﾀﾌ ﾆﾄﾀﾏﾀｻ ﾆﾔﾇﾏｱ・ﾀ・?'stdafx.h'ｸｦ ﾆﾔﾇﾏｽﾊｽﾃｿﾀ.
#endif

#include "resource.h"		// ﾁﾖ ｱ篳｣


// CStoryApp:
// ﾀﾌ ﾅｬｷ｡ｽｺﾀﾇ ｱｸﾇ｡ ｴ・ﾘｼ??4Story.cppﾀｻ ﾂ・ｶﾇﾏｽﾊｽﾃｿ?
//

class CStoryApp : public CWinApp
{
public:
	CStoryApp();

// ﾀ鄰､ﾀﾇ
	public:
	virtual BOOL InitInstance();

// ｱｸ?E

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
