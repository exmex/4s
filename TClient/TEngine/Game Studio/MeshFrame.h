#pragma once
#include "GSDialogBar.h"
#include "GridFrame.h"
#include "MeshBar.h"


// CMeshFrame 프레임입니다.

class CMeshFrame : public CGridFrame
{
	DECLARE_DYNCREATE(CMeshFrame)
protected:
	CMeshFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMeshFrame();

public:
	CGSDialogBar m_wndControlBar;
	CMeshBar m_wndMeshBar;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
