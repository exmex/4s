#include "Stdafx.h"
#include "TKeySettingDlg.h"
#include "TClientWnd.h"
#include "Resource.h"

// =========================================================================
CTKeySettingDlg::CTKeySettingDlg(TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent,pDesc), m_bNeedUpdate(FALSE), m_bWaitInput(FALSE)
{
	m_pList	= static_cast<TList*>(FindKid(ID_CTRLINST_LIST));
	m_pInfo	= FindKid(ID_CTRLINST_INFO);

	SetResult(RT_NONE);
}
// -------------------------------------------------------------------------
CTKeySettingDlg::~CTKeySettingDlg()
{
}
// =========================================================================

// =========================================================================
void CTKeySettingDlg::Update(DWORD dwTick)
{
	if( !IsVisible() || !m_bNeedUpdate )
		return;

	CTKeySetting* pKeySet = CTKeySetting::GetInstance();

	INT nItemCnt = m_pList->GetItemCount();
	if( nItemCnt == 0 )
	{
		m_pList->RemoveAll();

		for(DWORD i=0; i<TKEY_COUNT; ++i)
		{
			TKEY_SET eKey = (TKEY_SET)i;
			if( !pKeySet->NeedCustomize(eKey) )
				continue;

			if( !CTNationOption::KOR && eKey == TKEY_CAUTION15 )
				continue;

			int nRow = m_pList->AddString("");
			m_pList->SetItemData(nRow, 1, i);
		}

		nItemCnt = m_pList->GetItemCount();
	}

	for(DWORD i=0; i<nItemCnt; ++i)
	{
		TKEY_SET eKey = (TKEY_SET)m_pList->GetItemData(i,1);

		WORD wVKey = pKeySet->GetCurVKey(eKey);
		WORD wModKey = pKeySet->GetCurModKey(eKey);

		CString strKey = pKeySet->VKeyToStr(wVKey, wModKey);

		m_pList->SetItemString(i, 0, pKeySet->GetKeyDesc(eKey));
		m_pList->SetItemString(i, 1, strKey);
	}

	CTClientGame* pGame = CTClientGame::GetInstance();
	((CTGaugePannel*) pGame->GetFrame(TFRAME_GAUGE))->ResetHotkeyStr();

	m_bNeedUpdate = FALSE;
}
// =========================================================================

// =========================================================================
void CTKeySettingDlg::NotifyUpdate()
{
	m_bNeedUpdate = TRUE;
}
// =========================================================================
void CTKeySettingDlg::SetToDefault()
{
	CTKeySetting::GetInstance()->SetToDefault();
	
	NotifyUpdate();
}
// =========================================================================
void CTKeySettingDlg::SetResult(Result eResult)
{
	switch(eResult)
	{
	case RT_NONE:		m_pInfo->m_strText = CTChart::LoadString( TSTR_KEYRT_NONE); break;
	case RT_SUCCESS:	m_pInfo->m_strText = CTChart::LoadString( TSTR_KEYRT_SUCCESS); break;
	case RT_WAITINPUT:	m_pInfo->m_strText = CTChart::LoadString( TSTR_KEYRT_WAITINPUT); break;
	case RT_CANNOTUSE:	m_pInfo->m_strText = CTChart::LoadString( TSTR_KEYRT_CANNOTUSE); break;
	default: return;
	}

	m_eLastResult = eResult;
}
// -------------------------------------------------------------------------
void CTKeySettingDlg::SetReplaceResult(TKEY_SET eSrcKeySet, TKEY_SET eRepKeySet)
{
	if( eSrcKeySet == eRepKeySet )
		return;

	CTKeySetting* pKeySet = CTKeySetting::GetInstance();

	CString strSrcKeySet = pKeySet->GetKeyDesc(eSrcKeySet);
	if( strSrcKeySet.IsEmpty() )
		return;

	CString strRepKeySet = pKeySet->GetKeyDesc(eRepKeySet);
	if( strRepKeySet.IsEmpty() )
		return;

	m_pInfo->m_strText = CTChart::Format( TSTR_KEYRT_REPLACE, strSrcKeySet,strRepKeySet);
	m_eLastResult = RT_REPLACE;
}
// =========================================================================

// =========================================================================
HRESULT CTKeySettingDlg::Render(DWORD dwTickCount)
{
	Update(dwTickCount);

	return CTClientUIBase::Render(dwTickCount);
}
// =========================================================================
void CTKeySettingDlg::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
    if( !m_bWaitInput )
		CTClientUIBase::OnKeyDown(nChar, nRepCnt, nFlags);
}
// -------------------------------------------------------------------------
void CTKeySettingDlg::OnKeyUp(UINT nChar, int nRepCnt, UINT nFlags)
{
    if( m_bWaitInput )
	{
		int nSel = m_pList->GetSel();
		if( nSel >= 0 && nSel < m_pList->GetItemCount() )
		{
			CTKeySetting* pKeySet = CTKeySetting::GetInstance();

			WORD wVKey;
			if( nChar == VK_SNAPSHOT )
				wVKey = VK_SNAPSHOT;
			else
				wVKey = (WORD) MapVirtualKey(LOBYTE(nFlags),1);

			WORD wModKey = CTClientKEY::GetCurMOD();

			TKEY_SET eKey = (TKEY_SET)m_pList->GetItemData(nSel, 1);
			DWORD dwKeyResult = pKeySet->SetKey(eKey,wVKey,wModKey);

			if( dwKeyResult == TKEY_RESULT_NOCHANGE )
				SetResult(RT_NONE);
			else if( dwKeyResult == TKEY_RESULT_NOCUSTOM )
				SetResult(RT_CANNOTUSE);
			else
			{
				if( dwKeyResult )
					SetReplaceResult(eKey, (TKEY_SET)dwKeyResult);
				else
					SetResult(RT_SUCCESS);

				NotifyUpdate();
			}
		}
		
		m_bWaitInput = FALSE;
		m_pList->SetCurSelItem(-1);
	}
	else
		CTClientUIBase::OnKeyUp(nChar, nRepCnt, nFlags);
}
// =========================================================================
void CTKeySettingDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if( m_bWaitInput )
		return;

	CTClientUIBase::OnLButtonDown(nFlags,pt);
}
// -------------------------------------------------------------------------
void CTKeySettingDlg::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if( m_bWaitInput )
		return;

	TListItem* pItem = m_pList->GetHitItem(pt);
	if( pItem && pItem->m_param )
	{
		int nSel = m_pList->GetSel();
		if( nSel >= 0 && nSel < m_pList->GetItemCount() )
		{
			m_bWaitInput = TRUE;
			SetResult(RT_WAITINPUT);
		
			CTClientUIBase::OnLButtonUp(nFlags,pt);
			return;
		}
	}
		
	m_pList->SetCurSelItem(-1);
	
	CTClientUIBase::OnLButtonUp(nFlags,pt);
}
// =========================================================================
void CTKeySettingDlg::OnMButtonDown(UINT nFlags, CPoint pt)
{
}
// -------------------------------------------------------------------------
void CTKeySettingDlg::OnMButtonUp(UINT nFlags, CPoint pt)
{
}
// =========================================================================
void CTKeySettingDlg::OnRButtonDown(UINT nFlags, CPoint pt)
{
}
// -------------------------------------------------------------------------
void CTKeySettingDlg::OnRButtonUp(UINT nFlags, CPoint pt)
{
}
// =========================================================================
void CTKeySettingDlg::OnLButtonDblClk(UINT nFlags, CPoint pt)
{
}
// -------------------------------------------------------------------------
void CTKeySettingDlg::OnMButtonDblClk(UINT nFlags, CPoint pt)
{
}
// -------------------------------------------------------------------------
void CTKeySettingDlg::OnRButtonDblClk(UINT nFlags, CPoint pt)
{
}
// =========================================================================


























