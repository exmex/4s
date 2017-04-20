#include "Stdafx.h"
#include "TMiniPopupDlg.h"

// ======================================================================================
const DWORD CTMiniPopupDlg::FONT_COLOR	= 0xFFB19D67;
// ======================================================================================
CTMiniPopupDlg::CTMiniPopupDlg(TComponent* pParent, LP_FRAMEDESC pDesc, BOOL bStateMode)
	: CTClientUIBase(pParent, pDesc), m_bStateMode(bStateMode), m_nLastSelIndex(T_INVALID)
{
	CSize sz;
	
	m_pUpper = FindKid( ID_CTRLINST_UPPER );
	if( m_pUpper )
	{
		m_pUpper->GetComponentSize(&sz);
		m_nUpperHeight = sz.cy;
	}
	else
		m_nUpperHeight = 0;

	m_pLower = FindKid( ID_CTRLINST_LOWER );

	m_pBtnTemplate = FindKid( ID_CTRLINST_MIDDLE );
	m_pBtnTemplate->GetComponentSize(&sz);
	m_nButtonHeight = sz.cy;
	RemoveKid(m_pBtnTemplate);

	m_tip.m_ptr = NULL;
}
// --------------------------------------------------------------------------------------
CTMiniPopupDlg::~CTMiniPopupDlg()
{
	delete m_pBtnTemplate;
}
// ======================================================================================

// ======================================================================================
void CTMiniPopupDlg::AddButton(const CString& strTitle, DWORD dwMenuID)
{
	CTButtonMP* pBtn = new CTButtonMP(this, m_pBtnTemplate->m_pDESC);
	pBtn->m_id = GetUniqueID();
	pBtn->m_nIndex = GetButtonCount();
	pBtn->m_dwMenuID = dwMenuID;
	pBtn->m_strText = strTitle;
	pBtn->SetTextClr(FONT_COLOR);
			
	if( m_bStateMode )
		pBtn->SetStateButton();
	else
		pBtn->SetPushButton();

	AddKid(pBtn);
	m_vButtons.push_back(pBtn);

	ReFitSize();
}
// --------------------------------------------------------------------------------------
void CTMiniPopupDlg::AddButton(const CString& strTitle, FP_BTN_ACT pBtnFunc, DWORD dwUserData)
{
	AddButton(strTitle, (DWORD)0);
	CTButtonMP* pBtn = m_vButtons.back();
	pBtn->m_fpAct = pBtnFunc;
	pBtn->m_dwUser = dwUserData;
}
// --------------------------------------------------------------------------------------
void CTMiniPopupDlg::ClearButtons()
{
	for(INT i=0; i<GetButtonCount(); ++i)
	{
		RemoveKid( m_vButtons[i] );
		delete m_vButtons[i];
	}

	m_tip.m_ptr = NULL;
	m_vButtons.clear();
}
// ======================================================================================
void CTMiniPopupDlg::SetButtonColor(INT nIdx, DWORD dwColor)
{
	CD3DFont* pFONT = m_vButtons[nIdx]->Get3DFont();
	if( pFONT )
		pFONT->m_dwColor = dwColor;
}
// ======================================================================================
BOOL CTMiniPopupDlg::CanWithItemUI()
{
	return TRUE;
}
// ======================================================================================

// ======================================================================================
void CTMiniPopupDlg::ReFitSize()
{
	INT y = m_nUpperHeight;
	for(INT i=0; i<GetButtonCount(); ++i)
	{
		m_vButtons[i]->MoveComponent( CPoint(0,y) );
		y += m_nButtonHeight;
	}

	if( m_pLower )
		m_pLower->MoveComponent( CPoint(0,y) );
	
	CRect rc;

	if( m_pUpper )
		m_pUpper->GetComponentRect(&rc);
	else if( !m_vButtons.empty() )
		m_vButtons.front()->GetComponentRect(&rc);
	else
		rc = CRect(0,0,0,0);

	m_rc.left = rc.left;
	m_rc.top = rc.top;

	if( m_pLower )
        m_pLower->GetComponentRect(&rc);
	else if( !m_vButtons.empty() )
		m_vButtons.back()->GetComponentRect(&rc);
	else
		rc = CRect(0,0,0,0);

	m_rc.right = rc.right;
	m_rc.bottom = rc.bottom;
}
// ======================================================================================



// ======================================================================================
void CTMiniPopupDlg::CTButtonMP::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if( m_bDown && HitTest(pt) )
	{
		m_pHost->SetLastSelIndex(m_nIndex);

		if( m_dwMenuID && m_pHost->m_pCommandHandler )
			m_pHost->m_pCommandHandler->m_vCOMMAND.push_back(m_dwMenuID);
		else if( m_fpAct )
			(*m_fpAct)(m_pHost, this);
	}

	m_bDown = FALSE;

	TButton::OnLButtonUp(nFlags,pt);
}
// --------------------------------------------------------------------------------------
void CTMiniPopupDlg::CTButtonMP::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if( HitTest(pt) )
		m_bDown = TRUE;
	else
		m_bDown = FALSE;

	TButton::OnLButtonDown(nFlags,pt);
}
// ======================================================================================
CTMiniPopupDlg::CTButtonMP::CTButtonMP(CTMiniPopupDlg* pHost, LP_FRAMEDESC pDesc)
	: TButton(pHost,pDesc), m_pHost(pHost), m_nIndex(T_INVALID), m_fpAct(NULL), 
	  m_bDown(FALSE), m_dwUser(0)
{
}
// ======================================================================================























