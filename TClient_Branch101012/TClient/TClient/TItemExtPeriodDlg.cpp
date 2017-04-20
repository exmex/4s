#include "stdafx.h"
#include "TItemExtPeriodDlg.h"
#include "Resource.h"
#include "TClientGame.h"

// ====================================================================================================
CTItemExtPeriodDlg::CTItemExtPeriodDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
:	CTItemSlotBaseDlg(pParent, pDesc, SLOT_COUNT)
{
	m_pSlotCtrl[TARGET_SLOT] = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITEM) );

	TButton* pBTN = (TButton*) FindKid(ID_CTRLINST_OK);

	if( pBTN )
		pBTN->m_menu[TNM_LCLICK] = GM_EXTEND_ITEM_PERIOD;

	ClearItem();
	ShowComponent(FALSE);
}
// ----------------------------------------------------------------------------------------------------
CTItemExtPeriodDlg::~CTItemExtPeriodDlg()
{
}
// ====================================================================================================

// ====================================================================================================
// 이 메소드는 아이템을 슬롯에 올릴때 유효한 아이템인가를 판별해주는 메소드이다.
BOOL CTItemExtPeriodDlg::CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const
{
	LPTITEM pItem = pClientItem->GetTITEM();

	// 기간연장이 허용되지 않는 아이템을 걸려내는 등... 이런 조건문 다음과 같이 처리한다.
	// 모든 슬롯의 아이템에 공통된 조건
	/*if( pClientItem->CanGamble() )
	{
		m_strLastError = CTChart::LoadString( TSTR_ITEM_ERR_GAMBLE);
			return FALSE;
	}*/

	/*if( pClientItem->GetRefineMax() == 0 )
	{
		m_strLastError = CTChart::LoadString( TSTR_ITRF_ERR_CANNOTREFINE);
		return FALSE;
	}*/

	switch( bSlotID ) // 슬롯별 아이템 조건 검사
	{
	case TARGET_SLOT	:
		if( pClientItem->GetEndTime() == 0 )
		{
			m_strLastError = CTChart::LoadString( TSTR_ISNT_EXTPERIOD_ITEM);
			return FALSE;
		}
		break;
	}
	return TRUE;
}