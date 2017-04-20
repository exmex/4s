#include "stdafx.h"
#include "THelpFrame.h"
#include "TClientGame.h"

CTHelpFrame::CTHelpFrame( TComponent *pParent, LP_FRAMEDESC pDesc )
: CTClientUIBase( pParent, pDesc ),
m_pCurrentHelp(NULL),
m_bShowBtn(FALSE)
{
	m_pTITLE = FindKid( ID_CTRLINST_HELP_TITLE);
	m_pTEXTLIST = (TList*) FindKid( ID_CTRLINST_TEXTLIST);
	m_pImage = (TImageList*) FindKid( ID_CTRLINST_IMAGE );
	m_pLeftBtn = (TButton*) FindKid( ID_CTRLINST_LEFT );
	m_pRightBtn = (TButton*) FindKid( ID_CTRLINST_RIGHT );
	m_pUpBtn = (TButton*) FindKid( ID_CTRLINST_UP );
	m_pDownBtn = (TButton*) FindKid( ID_CTRLINST_DOWN );

	m_pLeftBtn->m_pUp->m_bHitTestType = HITTEST_TYPE_RECT;
	m_pLeftBtn->m_pDown->m_bHitTestType = HITTEST_TYPE_RECT;
	m_pLeftBtn->m_pHover->m_bHitTestType = HITTEST_TYPE_RECT;
	m_pRightBtn->m_pUp->m_bHitTestType = HITTEST_TYPE_RECT;
	m_pRightBtn->m_pDown->m_bHitTestType = HITTEST_TYPE_RECT;
	m_pRightBtn->m_pHover->m_bHitTestType = HITTEST_TYPE_RECT;

	ClearHelp();
}

CTHelpFrame::~CTHelpFrame()
{
}

void CTHelpFrame::SetHelp( DWORD dwHelpID )
{
	if( m_pCurrentHelp &&
		m_pCurrentHelp->m_dwCategoryID == dwHelpID )
		return ;

	LPTHELP pTHELP = CTChart::FindTHELP( dwHelpID );
	if( !pTHELP )
		return ;

	if( pTHELP->m_vPage.empty() )
		return ;

	m_pCurrentHelp = pTHELP;
	m_wCurrentPage = 0;

	SetPage(m_wCurrentPage);
}

void CTHelpFrame::SetPage( WORD wPage )
{
	if( !m_pCurrentHelp )
		return ;

	if( wPage < m_pCurrentHelp->m_vPage.size() )
	{
		m_pTITLE->m_strText = m_pCurrentHelp->m_vPage[wPage]->m_strTitle;
		CString strTEXT = m_pCurrentHelp->m_vPage[wPage]->m_strText;
		m_pImage->SetCurImage( m_pCurrentHelp->m_vPage[wPage]->m_wImage );

		strTEXT.Replace( _T("\\n"), _T("\n") );
		m_pTEXTLIST->ClipAndInsertString( strTEXT);
		m_wCurrentPage = wPage;
	}
}

void CTHelpFrame::LeftPage()
{
	if( m_pCurrentHelp )
	{
		WORD wOldPage = m_wCurrentPage;

		SetPage(m_wCurrentPage-1);

		if( CTClientGame::GetInstance()->GetClientMap()->m_wMapID != TUTORIAL_MAPID &&
			wOldPage == m_wCurrentPage ) // 페이지가 안 바꼈다는건 처음이라는 것.
		{
			LPTHELP pOldHelp = m_pCurrentHelp;
			PrevHelp();
			if( pOldHelp != m_pCurrentHelp ) // 카테고리가 바꼈다면!
				SetPage( (m_pCurrentHelp->m_vPage.size() != 0) ? m_pCurrentHelp->m_vPage.size()-1 : 0 );
		}
	}
}

void CTHelpFrame::RightPage()
{
	if( m_pCurrentHelp )
	{
		WORD wOldPage = m_wCurrentPage;
		
		SetPage(m_wCurrentPage+1);
		
		if( CTClientGame::GetInstance()->GetClientMap()->m_wMapID != TUTORIAL_MAPID &&
			wOldPage == m_wCurrentPage ) // 페이지가 안 바꼈다는건 끝이라는 것.
			NextHelp();
	}
}

void CTHelpFrame::ClearHelp()
{
	m_pTITLE->m_strText.Empty();
	m_pTEXTLIST->RemoveAll();
	m_pImage->SetCurImage( 0 );
}

void CTHelpFrame::NextHelp()
{
	DWORD dwNext = 1;
	if( m_pCurrentHelp )
		dwNext = m_pCurrentHelp->m_dwCategoryID + 1;
	SetHelp( dwNext );
}

void CTHelpFrame::PrevHelp()
{
	DWORD dwPrev = 1;
	if( m_pCurrentHelp )
		dwPrev = m_pCurrentHelp->m_dwCategoryID - 1;
	SetHelp( dwPrev );
}

void CTHelpFrame::OnQuestEvent( THELPLINK_TRIGGER trigger, DWORD dwQuestID )
{
	MAPTHELPLINK::iterator itLINK = CTChart::m_mapTHELPLINK[ trigger ].find( dwQuestID );
	if( itLINK == CTChart::m_mapTHELPLINK[ trigger ].end() )
		return ;

	LPTHELP pTHELP = CTChart::FindTHELP( itLINK->second );
	if( !pTHELP )
		return ;

	SetHelp( itLINK->second );

	CTClientGame::GetInstance()->EnableUI( TFRAME_HELP );
}

BOOL CTHelpFrame::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt) )
		return FALSE;

	if( m_pTEXTLIST->HitTest(pt) )
	{
		m_pTEXTLIST->DoMouseWheel(nFlags,zDelta,pt);
		return TRUE;
	}

	if( m_pImage->HitTest(pt) )
	{
		if( zDelta > 0 )
			LeftPage();
		else
			RightPage();

		return TRUE;
	}
	return TRUE;
}

BOOL CTHelpFrame::CanWithItemUI()
{
	return TRUE;
}

void CTHelpFrame::ShowComponent(BOOL bVisible )
{
	CTClientUIBase::ShowComponent(bVisible);
}

void CTHelpFrame::OnMouseMove(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnMouseMove(nFlags, pt);
}

HRESULT CTHelpFrame::Render(DWORD dwTickCount)
{
	if( !m_bVisible )
		return S_OK;

	WORD wMapID = CTClientGame::GetInstance()->GetClientMap()->m_wMapID;
	m_pUpBtn->ShowComponent(wMapID != TUTORIAL_MAPID);
	m_pDownBtn->ShowComponent(wMapID != TUTORIAL_MAPID);

	return CTClientUIBase::Render(dwTickCount);
}