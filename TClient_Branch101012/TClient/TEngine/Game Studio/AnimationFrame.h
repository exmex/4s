#pragma once
#include "GridFrame.h"
#include "AnimationBar.h"
#include "PlayCtrlBar.h"


// CAnimationFrame 프레임입니다.

class CAnimationFrame : public CGridFrame
{
	DECLARE_DYNCREATE(CAnimationFrame)
protected:
	CAnimationFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CAnimationFrame();

public:
	CPlayCtrlBar m_wndPlayCtrlBar;
	CGSDialogBar m_wndControlBar;

	CAnimationBar m_wndAnimationBar;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
