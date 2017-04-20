#pragma once
#include "TClientUIBase.h"

class CTFameRank;

class CTFameListDlg : public CTClientUIBase
{
public:
	CTFameListDlg( TComponent *pParent, LP_FRAMEDESC pDesc );
	virtual ~CTFameListDlg();

protected:
	TComponent*		m_pTITLE;

public:
	TList* m_pLIST;
	TComponent* m_pMonth;
	BYTE m_bMonth;
	VTFAMERANK m_vtFAMERANK;

public:
	void ReleaseData();
	void ResetList();
};
