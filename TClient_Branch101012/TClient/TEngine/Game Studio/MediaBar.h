#pragma once
#include "GSDialogBar.h"
#include "PlayCtrl.h"


// CMediaBar

class CMediaBar : public CGSDialogBar
{
	DECLARE_DYNAMIC(CMediaBar)

public:
	CMediaBar();
	virtual ~CMediaBar();

public:
	CPlayCtrl* GetPlayCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create( CWnd* pParentWnd, DWORD dwStyle);
	afx_msg void OnDestroy();
};
