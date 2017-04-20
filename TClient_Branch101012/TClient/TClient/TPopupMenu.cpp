#include "StdAfx.h"


CTPopupMenu::CTPopupMenu( TComponent *pParent, LP_FRAMEDESC pDesc)
:CTClientUIBase( pParent, pDesc)
{
	m_pTBUTTON = (TButton *) FindKid(ID_CTRLINST_MENU);
	m_pTBUTTON->GetComponentRect(&m_vTBUTTON);
	RemoveKid(m_pTBUTTON);

	m_pTBACK = FindKid(ID_CTRLINST_BACK);
	m_pTBACK->GetComponentRect(&m_vTBACK);
	RemoveKid(m_pTBACK);

	m_pTEND = FindKid(ID_CTRLINST_END);
	ClearMENU();

	m_bSND = FALSE;
	m_nSELECT = T_INVALID;
}

CTPopupMenu::~CTPopupMenu()
{
	if(m_pTBUTTON)
	{
		delete m_pTBUTTON;
		m_pTBUTTON = NULL;
	}

	if(m_pTBACK)
	{
		delete m_pTBACK;
		m_pTBACK = NULL;
	}

	ClearMENU();
}

void CTPopupMenu::ClearMENU()
{
	while(!m_vTPANNEL.empty())
	{
		RemoveKid(m_vTPANNEL.back());
		delete m_vTPANNEL.back();

		m_vTPANNEL.pop_back();
	}

	while(!m_vTMENU.empty())
	{
		RemoveKid(m_vTMENU.back());
		delete m_vTMENU.back();

		m_vTMENU.pop_back();
	}

	m_blDragging = FALSE;
	m_pFocus = NULL;

	m_ptPrev = CPoint( 0, 0);
	m_tip.Reset();
}

void CTPopupMenu::AddMENU( DWORD dwTitleID,
						   TCOMMAND dwMenuID)
{
	CString strTitle;

	strTitle = CTChart::LoadString( (TSTRING) dwTitleID);
	AddMENU( strTitle, dwMenuID);
}

void CTPopupMenu::AddMENU( CString strTitle,
						   TCOMMAND dwMenuID)
{
	TComponent *pTPANNEL = new TComponent( this, m_pTBACK->m_pDESC);
	TButton *pTMENU = new TButton( this, m_pTBUTTON->m_pDESC);

	int nCount = INT(m_vTMENU.size());

	pTPANNEL->m_id = MAKELONG( WORD(m_id), WORD(TPOPUPMAX_COUNT + nCount + 1));
	pTMENU->m_id = MAKELONG( WORD(m_id), WORD(nCount + 1));

	pTMENU->m_menu[TNM_LCLICK] = dwMenuID;
	pTMENU->m_strText = strTitle;

	pTPANNEL->MoveComponent(CPoint(
		m_vTBACK.left,
		m_vTBACK.top + nCount * m_vTBACK.Height()));

	pTMENU->MoveComponent(CPoint(
		m_vTBUTTON.left,
		m_vTBUTTON.top + nCount * m_vTBACK.Height()));

	m_pTEND->MoveComponent(CPoint(
		m_vTBACK.left,
		m_vTBACK.top + (nCount + 1) * m_vTBACK.Height()));

	m_vTPANNEL.push_back(pTPANNEL);
	m_vTMENU.push_back(pTMENU);

	AddKid(pTPANNEL);
	AddKid(pTMENU);

	CRect rc;
	m_pTEND->GetComponentRect(&rc);
	m_rc.bottom = m_rc.top + rc.bottom;
}

BOOL CTPopupMenu::CanWithItemUI()
{
	return TRUE;
}

void CTPopupMenu::OnLButtonDown(UINT nFlags, CPoint pt)
{
	m_nSELECT = T_INVALID;

	for(size_t i=0; i<m_vTMENU.size(); ++i)
	{
		if( m_vTMENU[i]->HitTest(pt) )
		{
			m_nSELECT = (INT) i;
			break;
		}
	}

	CTClientUIBase::OnLButtonDown(nFlags,pt);
}








