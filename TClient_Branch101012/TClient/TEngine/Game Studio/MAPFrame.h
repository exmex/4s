#pragma once
#include "GSDialogBar.h"
#include "PlayCtrlBar.h"
#include "MAPLIGHTBar.h"
#include "GridFrame.h"
#include "MAPOBJBar.h"
#include "MAPSFXBar.h"
#include "MAPSNDBar.h"


// CMAPFrame 프레임입니다.

class CMAPFrame : public CGridFrame
{
	DECLARE_DYNCREATE(CMAPFrame)
protected:
	CMAPFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMAPFrame();

public:
	CPlayCtrlBar m_wndPlayCtrlBar;
	CGSDialogBar m_wndMAPLIGHTBar;
	CGSDialogBar m_wndMAPOBJBar;
	CGSDialogBar m_wndMAPSFXBar;
	CGSDialogBar m_wndMAPSNDBar;

	CMAPLIGHTBar m_wndMAPLIGHTDlg;
	CMAPOBJBar m_wndMAPOBJDlg;
	CMAPSFXBar m_wndMAPSFXDlg;
	CMAPSNDBar m_wndMAPSNDDlg;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus( CWnd* pOldWnd);
};
