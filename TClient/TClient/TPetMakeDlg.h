#pragma once

#include "TPetDlg.h"

class CTPetMakeDlg : public CTPetDlg
{
protected:
	enum { MAX_LINE = 4 };
	TComponent*	m_pLines[MAX_LINE];

	BYTE	m_bInvenID;
	BYTE	m_bInvenSlot;

	TComponent* m_pMakeButton;

public:
	TComponent* GetMakeButton() const	{ return m_pMakeButton; }

	void SetPetItem(BYTE bInvenID, BYTE bInvenSlot)
	{
		m_bInvenID = bInvenID;
		m_bInvenSlot = bInvenSlot;
	}

	BYTE GetInvenID() const				{ return m_bInvenID; }
	BYTE GetInvenSlot() const			{ return m_bInvenSlot; }

	virtual CString GetEndTimeStr();
	virtual void Update(DWORD dwTickCount);

public:
	CString m_strEndTime;

public:
	CTPetMakeDlg(TComponent* pParent, LP_FRAMEDESC pDesc, CD3DDevice* pDevice);
	virtual ~CTPetMakeDlg();
};