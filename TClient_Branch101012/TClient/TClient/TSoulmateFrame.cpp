#include "Stdafx.h"
#include "TSoulmateFrame.h"
#include "TClientGame.h"

/*// ====================================================================
CTSoulmateFrame::CTSoulmateFrame(TComponent *pParent, LP_FRAMEDESC pDesc)
:	ITInnerFrame(pParent,pDesc,NULL)
{
	m_pName = FindKid(ID_CTRLINST_NAME);
	m_pClass = FindKid(ID_CTRLINST_CLASS);
	m_pRace = FindKid(ID_CTRLINST_RACE);
	m_pSex = FindKid(ID_CTRLINST_SEX);
}
// --------------------------------------------------------------------
CTSoulmateFrame::~CTSoulmateFrame()
{
}
// ====================================================================

// ====================================================================
void CTSoulmateFrame::UpdateSoulmateInfo()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	m_pName->m_strText = pGame->GetSoulName();

	m_pClass->m_strText.Empty();
	m_pRace->m_strText.Empty();
	m_pSex->m_strText.Empty();
}
// ====================================================================*/