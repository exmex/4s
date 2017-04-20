#include "Stdafx.h"
#include "TGuildCmdHelpDlg.h"
#include "Resource.h"

// ======================================================================
CTGuildCmdHelpDlg::CTGuildCmdHelpDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent, pDesc), m_bNeedInit(TRUE)
{
	m_pList = static_cast<TList*>( FindKid(ID_CTRLINST_LIST) );
	m_pList->ApplyUserColor(TRUE);
}
// ----------------------------------------------------------------------
CTGuildCmdHelpDlg::~CTGuildCmdHelpDlg()
{
}
// ======================================================================

// ======================================================================
void CTGuildCmdHelpDlg::Init()
{
	m_bNeedInit = FALSE;

	INT nIndex;
	CString strTMP,strCMD, strDESC;

	for(INT i=0; i<CTGuildCommander::CMD_COUNT; ++i)
	{
		strTMP = CTChart::LoadString( (TSTRING) CTGuildCommander::CMD_STRING[i]);
		strCMD = CTChart::Format( TSTR_FMT_GUILDCMD, strTMP);

		strTMP = CTChart::LoadString( (TSTRING) CTGuildCommander::CMD_DESC_STRING[i]);
		strDESC = CTChart::Format( TSTR_FMT_GUILDCMDESC, strTMP);

		nIndex = m_pList->AddString(strCMD);
		m_pList->SetUserColor(nIndex, 0, TCOLOR_GUILDCMD_CMD);

		nIndex = m_pList->AddString(strDESC);
		m_pList->SetUserColor(nIndex, 0, TCOLOR_GUILDCMD_DESC);
	}
}
// ======================================================================

// ======================================================================
void CTGuildCmdHelpDlg::ShowComponent( BOOL bVisible )
{
	if( m_bNeedInit && bVisible )
		Init();

	CTClientUIBase::ShowComponent(bVisible);
}
// ======================================================================