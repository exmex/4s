#pragma once
#include "GSDialogBar.h"
#include "PlayCtrl.h"


// CPlayCtrlBar

class CPlayCtrlBar : public CGSDialogBar
{
	DECLARE_DYNAMIC(CPlayCtrlBar)

public:
	CPlayCtrlBar();
	virtual ~CPlayCtrlBar();

public:
	CPlayCtrl* GetPlayCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create( CWnd* pParentWnd, DWORD dwStyle);
	afx_msg void OnDestroy();
};
