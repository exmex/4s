#include "stdafx.h"
#include "TSettingFrame.h"
#include "TClientGame.h"

CTSettingFrame::CTSettingFrame(TComponent* pParent, LP_FRAMEDESC pDesc, DWORD dwInnerPosID)
: CTFrameGroupBase(pParent, pDesc, dwInnerPosID)
{
	m_pBTN_SHOW_CAUTION15 = (TButton*) FindKid(ID_CTRLINST_BTN_SHOW_CAUTION15);

	if( !CTNationOption::KOR || !m_pBTN_SHOW_CAUTION15 )
	{
		TComponent *pText_Aution15 = FindKid(ID_CTRLINST_TEXT_SHOW_CAUTION15);

		if(m_pBTN_SHOW_CAUTION15)
		{
			RemoveKid(m_pBTN_SHOW_CAUTION15);

			delete m_pBTN_SHOW_CAUTION15;
			m_pBTN_SHOW_CAUTION15 = NULL;
		}

		if(pText_Aution15)
		{
			RemoveKid(pText_Aution15);
			delete pText_Aution15;
		}
	}
	else
		m_pBTN_SHOW_CAUTION15->m_bHitTestType = HITTEST_TYPE_RECT;

	UpdateUI();
}

CTSettingFrame::~CTSettingFrame()
{
}

void CTSettingFrame::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTFrameGroupBase::OnLButtonDown(nFlags, pt);
	if( m_pBTN_SHOW_CAUTION15 )
	{
		CTClientGame::m_vTOPTION.m_bShowCaution15 = m_pBTN_SHOW_CAUTION15->IsStateDown() ? TRUE : FALSE;

		CTGaugePannel* pTGAUGE = static_cast<CTGaugePannel*>( CTClientGame::GetInstance()->GetFrame( TFRAME_GAUGE ) );
		if( CTClientGame::m_vTOPTION.m_bShowCaution15 )
			pTGAUGE->AttachCaution15();
		else
			pTGAUGE->DettachCaution15();
	}
}

void CTSettingFrame::UpdateUI()
{
	if( m_pBTN_SHOW_CAUTION15 )
		m_pBTN_SHOW_CAUTION15->Select( CTClientGame::m_vTOPTION.m_bShowCaution15 ? TRUE : FALSE);
}