#include "Stdafx.h"
#include "TFrameGroupBase.h"

// CTFrameGroupBase
// ======================================================================
CTFrameGroupBase::CTFrameGroupBase(TComponent* pParent, LP_FRAMEDESC pDesc, DWORD dwInnerPosID)
	: CTClientUIBase(pParent,pDesc), m_ptInnerFrame(0,0), m_nSelectedFrame(T_INVALID)
{
	m_pBlank = FindKid(dwInnerPosID);

	if( m_pBlank )
		m_pBlank->GetComponentPos(&m_ptInnerFrame);
}
// ----------------------------------------------------------------------
CTFrameGroupBase::CTFrameGroupBase(TComponent* pParent, LP_FRAMEDESC pDesc, const CPoint& ptInnerPos)
	: CTClientUIBase(pParent,pDesc), m_ptInnerFrame(ptInnerPos), m_nSelectedFrame(T_INVALID), m_pBlank(NULL)
{
}
// ----------------------------------------------------------------------
CTFrameGroupBase::~CTFrameGroupBase()
{
}
// ======================================================================

// ======================================================================
UINT CTFrameGroupBase::AddFrame(ITInnerFrame* pFrame, DWORD dwTabID, const CString& strBlankMsg)
{
	m_FrameInfoArray.resize( m_FrameInfoArray.size()+1 );

	FrameInfo& frminf = m_FrameInfoArray.back();
	frminf.m_nFrameID = pFrame->GetFrameID();
	frminf.m_pFrameCtrl = pFrame;
	frminf.m_bUseBlank = !(strBlankMsg.IsEmpty());
	frminf.m_strBlankMsg = strBlankMsg;
	
	frminf.m_pTabCtrl = static_cast<TButton*>( FindKid(dwTabID) );
	ASSERT(frminf.m_pTabCtrl && "주어진 탭은 이 프레임의 자식이 아닙니다.");
	
	frminf.m_pTabCtrl->SetStyle( frminf.m_pTabCtrl->GetStyle() | TBS_STATE_BUTTON );
	frminf.m_pFrameCtrl->m_id = GetUniqueID();
	AddKid(frminf.m_pFrameCtrl);

	frminf.m_pFrameCtrl->MoveComponent(m_ptInnerFrame);

	if( m_nSelectedFrame == T_INVALID )
		SetSelect(0);

	return GetFrameCount();
}
// ----------------------------------------------------------------------
void CTFrameGroupBase::ClearFrame()
{
	m_FrameInfoArray.clear();
	m_nSelectedFrame = T_INVALID;
}
// ======================================================================
UINT CTFrameGroupBase::GetFrameCount() const
{
	return (UINT)m_FrameInfoArray.size();
}
// ----------------------------------------------------------------------
ITInnerFrame* CTFrameGroupBase::GetInnerFrame(UINT nFrame) const
{
	return const_cast<ITInnerFrame*>(m_FrameInfoArray[nFrame].m_pFrameCtrl);
}
// ======================================================================
void CTFrameGroupBase::SetBlank(UINT nFrame, BOOL bBlank, const CString& strBlankMsg)
{
	FrameInfo& frminf = m_FrameInfoArray[nFrame];

	frminf.m_bUseBlank = bBlank;
	frminf.m_strBlankMsg = strBlankMsg;
}
// ======================================================================
void CTFrameGroupBase::RequestInfo()
{
	if( m_nSelectedFrame != T_INVALID )
	{
		FrameInfo& frminf = m_FrameInfoArray[m_nSelectedFrame];
		
		if( !frminf.m_bUseBlank )
			frminf.m_pFrameCtrl->RequestInfo();
	}
}
// ----------------------------------------------------------------------
void CTFrameGroupBase::ResetInfo()
{
	if( m_nSelectedFrame != T_INVALID )
	{
		FrameInfo& frminf = m_FrameInfoArray[m_nSelectedFrame];
		
		if( !frminf.m_bUseBlank )
			frminf.m_pFrameCtrl->ResetInfo();
	}
}
// ======================================================================
void CTFrameGroupBase::SetSelect(UINT nFrame)
{
	if( m_nSelectedFrame != T_INVALID )
	{
		FrameInfo& frminf = m_FrameInfoArray[m_nSelectedFrame];
		
		frminf.m_pTabCtrl->Select(FALSE);
		frminf.m_pFrameCtrl->ShowComponent(FALSE);
	}

	m_nSelectedFrame = (INT)nFrame;

	if( m_nSelectedFrame != T_INVALID )
	{
		FrameInfo& frminf = m_FrameInfoArray[nFrame];

		if( m_pBlank )
		{
			m_pBlank->m_strText = frminf.m_strBlankMsg;
			m_pBlank->ShowComponent(frminf.m_bUseBlank);
		}

		frminf.m_pTabCtrl->Select(TRUE);
		frminf.m_pFrameCtrl->ShowComponent(!frminf.m_bUseBlank);
		
		RequestInfo();
	}
}
// ======================================================================
INT CTFrameGroupBase::GetSelectedFrame()
{
	return m_nSelectedFrame;
}
// ----------------------------------------------------------------------
BOOL CTFrameGroupBase::IsSelect(UINT nFrame) const
{
	return ( m_nSelectedFrame == (INT)nFrame );
}
// ======================================================================

// ======================================================================
void CTFrameGroupBase::ShowComponent(BOOL bVisible)
{
	CTClientUIBase::ShowComponent(bVisible);

	if( bVisible )
	{
		size_t cnt = m_FrameInfoArray.size();
		for(size_t i=0; i<cnt; ++i)
		{
			FrameInfo& frminf = m_FrameInfoArray[i];

			if( IsSelect(i) )
			{
				if( m_pBlank )
				{
					m_pBlank->m_strText = frminf.m_strBlankMsg;
					m_pBlank->ShowComponent(frminf.m_bUseBlank);
				}

				frminf.m_pFrameCtrl->ShowComponent(!frminf.m_bUseBlank);
			}
			else
				frminf.m_pFrameCtrl->ShowComponent(FALSE);
		}

		RequestInfo();
	}
}	
// ======================================================================
void CTFrameGroupBase::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonDown(nFlags, pt);

	size_t cnt = m_FrameInfoArray.size();
	for(size_t i=0; i<cnt; ++i)
	{
		if( m_FrameInfoArray[i].m_pTabCtrl->HitTest(pt) )
		{
			SetSelect(i);
			break;
		}
	}
}
// ======================================================================


























