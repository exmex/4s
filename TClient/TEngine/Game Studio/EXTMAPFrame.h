#pragma once
#include "GSDialogBar.h"
#include "MAPFrame.h"
#include "EXTMAPBar.h"


// CEXTMAPFrame 프레임입니다.

class CEXTMAPFrame : public CMAPFrame
{
	DECLARE_DYNCREATE(CEXTMAPFrame)
protected:
	CEXTMAPFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CEXTMAPFrame();

public:
	CGSDialogBar m_wndControlBar;
	CEXTMAPBar m_wndEXTMAPBar;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMapBack();
};
