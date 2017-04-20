// SvrTester.h : SvrTester 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"       // 주 기호

#include "STType.h"			// SvrTester Type

#include "TSessionFrm.h"
#include "TSessionView.h"

#include "TMacroFrm.h"
#include "TMacroView.h"

// CSvrTesterApp:
// 이 클래스의 구현에 대해서는 SvrTester.cpp을 참조하십시오.
//

class CSvrTesterApp : public CWinApp
{
public:
	CSvrTesterApp();
	~CSvrTesterApp();


// 재정의
public:
	virtual BOOL InitInstance();

// 구현
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	/////////////////////////////////////////////////////////////////////////////////////
	MAPDOCTEMP m_mapDocTemp; // STType.h 에 정의
	CMultiDocTemplate* GetDocTemplate(int nPageID); // DocTemplate 리턴
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	// TSessionView / TSessionFrm Return
	CTSessionView* GetTSessionView(); 	
	CTSessionFrm* GetTSessionFrm();
	CTSessionFrm* pTSessionFrm; // TSessionFrm의 클래스포인터
	/////////////////////////////////////////////////////////////////////////////////////	

	/////////////////////////////////////////////////////////////////////////////////////	
	// TMacroView / TMacroFrm Return
	CTMacroView* GetTMacroView();	
	CTMacroFrm* GetTMacroFrm();
	CTMacroFrm* pTMacroFrm;	
	/////////////////////////////////////////////////////////////////////////////////////	
};

extern CSvrTesterApp theApp;
