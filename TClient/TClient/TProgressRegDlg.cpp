#include "Stdafx.h"
#include "TProgressRegDlg.h"
#include "TClientWnd.h"

// ====================================================================================================
CTProgressRegDlg::CTProgressRegDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent, pDesc), m_dwTick(0), m_dwPeriod(0)
{
	m_pTitle = FindKid(ID_CTRLINST_TITLE);
	m_pMsg = FindKid(ID_CTRLINST_TXT);

	m_pGauge = static_cast<TGauge*>( FindKid(ID_CTRLINST_GAUGE) );
	m_pGauge->SetStyle(TGS_GROW_RIGHT);
}
// ----------------------------------------------------------------------------------------------------
CTProgressRegDlg::~CTProgressRegDlg()
{
}
// ====================================================================================================

// ====================================================================================================
void CTProgressRegDlg::Show(CString strTitle,
							CString strMsg,
							DWORD dwPeriod,
							DWORD dwEndGM)
{
	if( m_pTitle )
		m_pTitle->m_strText = strTitle;

	m_pMsg->m_strText = strMsg;
	m_dwPeriod = dwPeriod;
	m_dwEndGM = dwEndGM;

	CTClientWnd::GetInstance()
		->DoModalFrame(TFRAME_PROGRESS_REG);
}
// ====================================================================================================
BOOL CTProgressRegDlg::Update(DWORD dwTickCount)
{
	m_dwTick += dwTickCount;

	if( m_dwTick >= m_dwPeriod )
	{
		m_pGauge->m_strText = "100 %%";
		m_pGauge->SetGauge(m_dwPeriod, m_dwPeriod, FALSE);
		
		TFrame::m_pCommandHandler
			->m_vCOMMAND.push_back(m_dwEndGM);

		CTClientWnd::GetInstance()
			->CloseModalFrame();

		return TRUE;
	}
	else
	{
		m_pGauge->m_strText.Format("%d %%", m_dwTick*100/m_dwPeriod);
		m_pGauge->SetGauge(m_dwTick, m_dwPeriod, FALSE);

		return FALSE;
	}
}
// ====================================================================================================
void CTProgressRegDlg::ShowComponent(BOOL bVisible)
{
	TComponent::ShowComponent(bVisible);

	m_pGauge->m_strText = "0 %";
	m_pGauge->SetGauge(0, m_dwPeriod, FALSE);
	
	m_dwTick = 0;
}
// ====================================================================================================
BOOL CTProgressRegDlg::CanWithItemUI()
{
	return TRUE;
}
// ====================================================================================================
