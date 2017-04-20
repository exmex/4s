#include "Stdafx.h"
#include "TCommunityDlg.h"
#include "TGuildRTTypeFrame.h"

// ======================================================================
CTGuildRTTypeFrame::CTGuildRTTypeFrame(TComponent *pParent, LP_FRAMEDESC pDesc, UINT nFrameID, UINT nPageCount)
	: ITInnerFrame(pParent, pDesc, nFrameID), m_nCurPage(0), m_nPageMax(nPageCount), m_ptLastLClick(0,0)
{
	m_vTab = new TButton*[nPageCount];
	m_vList = new CTListEx*[nPageCount];
}
// ----------------------------------------------------------------------
CTGuildRTTypeFrame::~CTGuildRTTypeFrame()
{
	delete [] m_vTab;
	delete [] m_vList;
}
// ======================================================================

// ======================================================================
void CTGuildRTTypeFrame::ResetInfo()
{
	for(int i=0; i<m_nPageMax; ++i)
	{
		m_vTab[i]->Select( i == m_nCurPage );

		m_vList[i]->ShowComponent( i == m_nCurPage );
		m_vList[i]->EnableComponent( i == m_nCurPage );
	}
}
// ======================================================================

// ======================================================================
void CTGuildRTTypeFrame::OnLButtonDown( UINT nFlags, CPoint pt )
{
	m_ptLastLClick = pt;

	for(BYTE i=0; i<m_nPageMax; ++i)
	{
		if( m_vTab[i]->HitTest(pt) )
		{
			ChangePage(i);
			return;
		}
	}

	ITInnerFrame::OnLButtonDown(nFlags,pt);
}
// ======================================================================
void CTGuildRTTypeFrame::SwitchFocus(TComponent *pCandidate)
{
	if( m_pFocus == pCandidate )
		return;

	if( m_pFocus )
		m_pFocus->SetFocus(FALSE);

	if( pCandidate )
		pCandidate->SetFocus(TRUE);

	m_pFocus = pCandidate;
}
// ======================================================================
void CTGuildRTTypeFrame::ShowComponent( BOOL bVisible )
{
	if( !bVisible )
		ITInnerFrame::ShowComponent(FALSE);
	else
	{
		m_bVisible = bVisible;

		TCOMP_LIST::iterator itr,end;
		itr = m_kids.begin();
		end = m_kids.end();

		for(; itr != end ; ++itr)
		{
			TComponent* pKid = *itr;
			pKid->ShowComponent(pKid->IsEnable());
		}
	}
}
// ======================================================================

// ======================================================================
void CTGuildRTTypeFrame::ChangePage(UINT nPage)
{
	if( m_nCurPage == nPage )
		return;

	m_nCurPage = nPage;

	RequestInfo();
}
// ======================================================================



















