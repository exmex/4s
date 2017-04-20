#include "Stdafx.h"
#include "TItemRegBox.h"
#include "TClientGame.h"
#include "Resource.h"

// ====================================================================================================
CTItemRegBox::CTItemRegBox( TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent, pDesc), m_pItem(NULL)
{
	m_pTitle = FindKid(ID_CTRLINST_TITLE);
	m_pOK = static_cast< TButton* >( FindKid( ID_CTRLINST_OK ) );

	TComponent* pTemp;
	
	pTemp = FindKid(ID_CTRLINST_EDIT_COUNT);
	m_pCount = new CTNumberEdit(this, pTemp->m_pDESC, MAX_COUNT_POSCNT);
	
	RemoveKid(pTemp);
	AddKid(m_pCount);
	delete pTemp;
	

	pTemp = FindKid(ID_CTRLINST_RUNE);
	m_pRune = new CTNumberEdit(this, pTemp->m_pDESC, TRUNE_LENGTH);
	
	RemoveKid(pTemp);
	AddKid(m_pRune);
	delete pTemp;

	pTemp = FindKid(ID_CTRLINST_LUNA);
	m_pLuna = new CTNumberEdit(this, pTemp->m_pDESC, TLUNA_LENGTH);
	
	RemoveKid(pTemp);
	AddKid(m_pLuna);
	delete pTemp;

	pTemp = FindKid(ID_CTRLINST_CRON);
	m_pCron = new CTNumberEdit(this, pTemp->m_pDESC, TCRON_LENGTH);
	
	RemoveKid(pTemp);
	AddKid(m_pCron);
	delete pTemp;

	FindKid(ID_CTRLINST_OK)->m_menu[TNM_LCLICK] = GM_REG_ITEM_PRVSHOP;
}
// ----------------------------------------------------------------------------------------------------
CTItemRegBox::~CTItemRegBox()
{
}
// ====================================================================================================
void CTItemRegBox::SetItem(CTClientItem* pItem, BYTE bInven, BYTE bInvenSlot)	
{
	if( pItem )
		m_pTitle->m_strText = pItem->GetTITEM()->m_strNAME;

    m_pItem = pItem;
	m_bInven = bInven;
	m_bInvenSlot = bInvenSlot;

	m_pCount->m_strText = CTChart::Format( TSTR_FMT_NUMBER, pItem->GetCount());

	DWORD dwRune = 0;
	DWORD dwLuna = 0;
	DWORD dwCron = 0;

	CTClientGame::SplitMoney( pItem->GetPrice(), &dwRune, &dwLuna, &dwCron );
	m_pRune->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwRune );
	m_pLuna->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwLuna );
	m_pCron->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwCron );

	//DWORD dwPrice = pItem->GetPrice();
	//m_pRune->m_strText = CTChart::Format( TSTR_FMT_NUMBER, DWORD(dwPrice/TRUNE_DIVIDER) );
	//m_pLuna->m_strText = CTChart::Format( TSTR_FMT_NUMBER, DWORD((dwPrice%TRUNE_DIVIDER) / TLUNA_DIVIDER) );
	//m_pCron->m_strText = CTChart::Format( TSTR_FMT_NUMBER, DWORD(dwPrice%TLUNA_DIVIDER) );

	m_pCount->MoveCaretToFront();
	SwitchFocus(m_pCount);
}
// ====================================================================================================
void CTItemRegBox::SetCount(DWORD dwCnt)
{
	CString str;
	str = CTChart::Format( TSTR_FMT_NUMBER, dwCnt);
	m_pCount->SetText(str);
}
// ----------------------------------------------------------------------------------------------------
void CTItemRegBox::SetRune(DWORD dwRune)
{
	CString str;
	str = CTChart::Format( TSTR_FMT_NUMBER, dwRune);
	m_pCount->SetText(str);
}
// ----------------------------------------------------------------------------------------------------
void CTItemRegBox::SetLuna(DWORD dwLuna)
{
	CString str;
	str = CTChart::Format( TSTR_FMT_NUMBER, dwLuna);
	m_pCount->SetText(str);
}
// ----------------------------------------------------------------------------------------------------
void CTItemRegBox::SetCron(DWORD dwCron)
{
	CString str;
	str = CTChart::Format( TSTR_FMT_NUMBER, dwCron);
	m_pCount->SetText(str);
}
// ----------------------------------------------------------------------------------------------------
void CTItemRegBox::SetOkGM( DWORD dwGM )
{
	if( m_pOK )
	{
		m_pOK->m_menu[ TNM_LCLICK ] = dwGM;
	}
}
// ====================================================================================================
DWORD CTItemRegBox::GetCount() const
{
	return (DWORD)(::atoi(m_pCount->m_strText));
}
// ----------------------------------------------------------------------------------------------------
DWORD CTItemRegBox::GetRune() const
{
	return (DWORD)(::atoi(m_pRune->m_strText));
}
// ----------------------------------------------------------------------------------------------------
DWORD CTItemRegBox::GetLuna() const
{
	return (DWORD)(::atoi(m_pLuna->m_strText));
}
// ----------------------------------------------------------------------------------------------------
DWORD CTItemRegBox::GetCron() const
{
	return (DWORD)(::atoi(m_pCron->m_strText));
}
// ====================================================================================================
TEdit* CTItemRegBox::GetCurEdit()
{
	if( !IsVisible() || !GetFocus() )
		return NULL;

	if( m_pCount->GetFocus() )
		return m_pCount;
	if( m_pRune->GetFocus() )
		return m_pRune;
	if( m_pLuna->GetFocus() )
		return m_pLuna;
	if( m_pCron->GetFocus() )
		return m_pCron;

	return NULL;
}
// ====================================================================================================
BOOL CTItemRegBox::CanWithItemUI()
{
	return TRUE;
}
// ====================================================================================================








