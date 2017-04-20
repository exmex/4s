#include "Stdafx.h"
#include "TItemUpGaugeDlg.h"
#include "TClientGame.h"
#include "TColorPicker.h"

// ====================================================================================================
CTItemUpGaugeDlg::CTItemUpGaugeDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent, pDesc), m_dwTick(0)
{
	m_pGauge = static_cast<TGauge*>( FindKid(ID_CTRLINST_ITUP_GAGUE) );
	m_pGauge->SetStyle(TGS_GROW_RIGHT);

	m_pTarget = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITUP_SLOT1) );
	m_pScroll = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITUP_SLOT2) );

	m_bCanDrag = FALSE;
	m_bShowCancel = TRUE;

	m_pCancelButton = FindKid( ID_CTRLINST_CANCEL );
	m_pCloseButton = FindKid( ID_CTRLINST_CLOSE );
}
// ----------------------------------------------------------------------------------------------------
CTItemUpGaugeDlg::~CTItemUpGaugeDlg()
{
}
// ====================================================================================================

// ====================================================================================================
void CTItemUpGaugeDlg::SetImage(WORD wTarget, WORD wScroll)
{
	m_pTarget->SetCurImage(wTarget);
	m_pScroll->SetCurImage(wScroll);
}
// ====================================================================================================
void CTItemUpGaugeDlg::SetSkinImage( WORD wSkinImage )
{
	m_pTarget->SetSkinImage( wSkinImage );
}
// ====================================================================================================
void CTItemUpGaugeDlg::SetSkinImageEmpty()
{
	m_pTarget->SetSkinImageEmpty();
}
// ====================================================================================================
BOOL CTItemUpGaugeDlg::Update(DWORD dwTickCount)
{
	m_dwTick += dwTickCount;

	CTClientGame* pTGAME = CTClientGame::GetInstance();
	CTColorPicker* pColorPicker = static_cast<CTColorPicker*>( pTGAME->GetFrame(TFRAME_COLOR_PICKER) );
	if( pColorPicker->IsVisible() && m_dwTick >= (TITEM_UP_DELAY-1000) )
		pColorPicker->StopRandAnimation();

	if( m_dwTick >= TITEM_UP_DELAY )
	{
		m_pGauge->m_strText = "100 %%";
		m_pGauge->SetGauge(TITEM_UP_DELAY, TITEM_UP_DELAY, FALSE);
		
		return TRUE;
	}
	else
	{
		m_pGauge->m_strText.Format("%d %%", m_dwTick*100/TITEM_UP_DELAY);
		m_pGauge->SetGauge(m_dwTick, TITEM_UP_DELAY, FALSE);
		
		return FALSE;
	}
}
// ====================================================================================================
void CTItemUpGaugeDlg::ShowComponent(BOOL bVisible)
{
	TComponent::ShowComponent(bVisible);

	m_pGauge->m_strText = "0 %";
	m_pGauge->SetGauge(0, TITEM_UP_DELAY, FALSE);
	m_dwTick = 0;

	if(bVisible)
		CTachyonRes::m_MEDIA.Play( MEDIA_TSOUND, ID_SND_ITEM_START);

	if( !bVisible )
	{
		CTClientGame* pTGAME = CTClientGame::GetInstance();
		CTColorPicker* pColorPicker = static_cast<CTColorPicker*>( pTGAME->GetFrame( TFRAME_COLOR_PICKER ) );
		pColorPicker->ShowComponent( FALSE );
		pColorPicker->SetBindTarget( NULL );
	}

	m_pCancelButton->ShowComponent( m_bShowCancel );
	m_pCloseButton->ShowComponent( m_bShowCancel );
}
// ====================================================================================================
void CTItemUpGaugeDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonDown( nFlags, pt );
}
// ====================================================================================================
void CTItemUpGaugeDlg::OnLButtonUp(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonUp( nFlags, pt );
}
