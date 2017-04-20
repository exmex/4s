#pragma once
#include "GridFrame.h"
#include "OBJBar.h"
#include "PlayCtrlBar.h"


// COBJFrame 프레임입니다.

class COBJFrame : public CGridFrame
{
	DECLARE_DYNCREATE(COBJFrame)
protected:
	COBJFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~COBJFrame();

public:
	CPlayCtrlBar m_wndPlayCtrlBar;
	CGSDialogBar m_wndControlBar;

	COBJBar m_wndOBJBar;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};
