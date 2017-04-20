#pragma once
#include "GridFrame.h"
#include "PlayCtrlBar.h"
#include "ParticleBar.h"
#include "SFXBar.h"


// CSFXFrame 프레임입니다.

class CSFXFrame : public CGridFrame
{
	DECLARE_DYNCREATE(CSFXFrame)
protected:
	CSFXFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSFXFrame();

public:
	CPlayCtrlBar m_wndPlayCtrlBar;
	CGSDialogBar m_wndParticleBar;
	CGSDialogBar m_wndControlBar;

	CParticleBar m_wndPSFXBar;
	CSFXBar m_wndSFXBar;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
