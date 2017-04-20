#pragma once

class CTItemRegBox : public CTClientUIBase
{
public:
	enum { MAX_COUNT_POSCNT = 3 };

protected:
	TComponent*		m_pTitle;
	TButton*		m_pOK;

	TEdit*			m_pCount;
	TEdit*			m_pRune;
	TEdit*			m_pLuna;
	TEdit*			m_pCron;

	CTClientItem*	m_pItem;
	BYTE			m_bInven;
	BYTE			m_bInvenSlot;

public:
	void SetItem(CTClientItem* pItem, BYTE bInven, BYTE bInvenSlot);
	
	CTClientItem* GetItem() const	{ return m_pItem; }
	BYTE GetInven() const			{ return m_bInven; }
	BYTE GetInvenSlot() const		{ return m_bInvenSlot; }
	
	void SetCount(DWORD dwCnt);
	void SetRune(DWORD dwRune); 
	void SetLuna(DWORD dwLuna); 
	void SetCron(DWORD dwCron); 
	void SetOkGM( DWORD dwGM );

	DWORD GetCount() const;
	DWORD GetRune() const;
	DWORD GetLuna() const;
	DWORD GetCron() const;

	TEdit* GetCurEdit();

	virtual BOOL CanWithItemUI();

public:
	CTItemRegBox( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTItemRegBox();
};