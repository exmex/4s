#include "Stdafx.h"
#include "TCommunityDlg.h"
#include "TClientGame.h"

// CTCommunityDlg
// ======================================================================
CTCommunityDlg::CTCommunityDlg(TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTFrameGroupBase(pParent,pDesc, ID_CTRLINST_INNERPOS)
{
}
// ----------------------------------------------------------------------
CTCommunityDlg::~CTCommunityDlg()
{
}
// ======================================================================
ITDetailInfoPtr CTCommunityDlg::GetTInfoKey( const CPoint& point )
{
	if( m_nSelectedFrame != T_INVALID )
	{
		FrameInfo& frminf = m_FrameInfoArray[m_nSelectedFrame];
		if( frminf.m_pFrameCtrl)
			return frminf.m_pFrameCtrl->GetTInfoKey( point); 
	}

	ITDetailInfoPtr pInfo;
	return pInfo;
}
// ======================================================================
void CTCommunityDlg::OnLButtonUp(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonUp(nFlags,pt);
}
// ======================================================================
void CTCommunityDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonDown(nFlags,pt);
}
// ======================================================================
void  CTCommunityDlg::ShowComponent(BOOL bVisible)
{
	CTFrameGroupBase::ShowComponent( bVisible );
}

void CTCommunityDlg::ResetPosition()
{
	CTClientUIBase::ResetPosition();
}
