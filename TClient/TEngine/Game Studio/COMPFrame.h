#pragma once
#include "GSDialogBar.h"
#include "GuideFrame.h"
#include "COMPBar.h"


// CCOMPFrame 프레임입니다.

class CCOMPFrame : public CGuideFrame
{
	DECLARE_DYNCREATE(CCOMPFrame)
protected:
	CCOMPFrame();
	virtual ~CCOMPFrame();

public:
	CGSDialogBar m_wndControlBar;
	CCOMPBar m_wndCOMPBar;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
