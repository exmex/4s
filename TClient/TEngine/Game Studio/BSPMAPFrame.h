#pragma once
#include "GSDialogBar.h"
#include "MAPFrame.h"
#include "BSPMAPBar.h"


// CBSPMAPFrame 프레임입니다.

class CBSPMAPFrame : public CMAPFrame
{
	DECLARE_DYNCREATE(CBSPMAPFrame)
protected:
	CBSPMAPFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CBSPMAPFrame();

public:
	CGSDialogBar m_wndControlBar;
	CBSPMAPBar m_wndBSPMAPBar;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMapBack();
};


