#include "Stdafx.h"
#include "TGuildAppInnerFrame.h"
#include "TClientGame.h"

// CTGuildAppInnerFrame
// ======================================================================
CTGuildAppInnerFrame::CSelecter::CSelecter(TButton* pLB, TButton* pRB, TComponent* pTXT)
	: m_pLB(pLB), m_pRB(pRB), m_pTXT(pTXT), m_nCUR(T_INVALID), m_bEnable(FALSE)
{

}
// ----------------------------------------------------------------------
void CTGuildAppInnerFrame::CSelecter::AddTxt(DWORD dwID, DWORD dwDATA)
{
	CString str;
	str = CTChart::LoadString( (TSTRING) dwID);
	m_vTXTS.push_back(str);
	
	m_vDATAS.push_back(dwDATA);

	if( m_nCUR == T_INVALID )
		 SetCur(0);
}
// ----------------------------------------------------------------------
void CTGuildAppInnerFrame::CSelecter::ClearTxt()
{
	m_vTXTS.clear();
	SetCur(T_INVALID);
}
// ----------------------------------------------------------------------
void CTGuildAppInnerFrame::CSelecter::Move(BOOL bLeft)
{
	INT n = GetCur();
	if( bLeft )
		n = (n > 0)? n-1: m_vTXTS.size()-1;
	else
		n = (n < m_vTXTS.size()-1)? n+1: 0;

	SetCur(n);
}
// ----------------------------------------------------------------------
void CTGuildAppInnerFrame::CSelecter::SetCur(INT nIndex)
{
	m_nCUR = nIndex;
	m_pTXT->m_strText = m_vTXTS[m_nCUR];
}
// ----------------------------------------------------------------------
INT CTGuildAppInnerFrame::CSelecter::GetCur()
{
	return m_nCUR;
}
// ----------------------------------------------------------------------
DWORD CTGuildAppInnerFrame::CSelecter::GetData(INT nIndex)
{
	return m_vDATAS[nIndex];
}
// ----------------------------------------------------------------------
BOOL CTGuildAppInnerFrame::CSelecter::HitTest(CPoint pt)
{
	if( !m_bEnable )
		return FALSE;

	if( m_pLB->HitTest(pt) )
	{
		Move(TRUE);
		return TRUE;
	}

	if( m_pRB->HitTest(pt) )
	{
		Move(FALSE);
		return TRUE;
	}

	return FALSE;
}
// ----------------------------------------------------------------------
void CTGuildAppInnerFrame::CSelecter::SetEnable(BOOL bEnable)
{
	m_bEnable = bEnable;
}
// ----------------------------------------------------------------------
BOOL CTGuildAppInnerFrame::CSelecter::IsEnable()
{
	return m_bEnable;
}
// ======================================================================

// CTGuildAppInnerFrame
// ======================================================================
CTGuildAppInnerFrame::CTGuildAppInnerFrame(TComponent *pParent, LP_FRAMEDESC pDesc, UINT nFrameID)
	: ITInnerFrame(pParent,pDesc,nFrameID), m_pSelecter(NULL),
	  m_bInit(FALSE), m_nCurMode(0), m_nPrvSel(T_INVALID)
{
}
// ----------------------------------------------------------------------
CTGuildAppInnerFrame::~CTGuildAppInnerFrame()
{
	if( m_pSelecter )
		delete m_pSelecter;
}
// ======================================================================

// ======================================================================
TEdit* CTGuildAppInnerFrame::GetCurEdit()
{
	if( IsEditMode() && m_pContentEdit->IsVisible() && m_pFocus == m_pContentEdit )
		return static_cast<TEdit*>(m_pFocus);

	return NULL;
}
// ======================================================================

// ======================================================================
void CTGuildAppInnerFrame::SetContent(const CString& strText)
{
	CTTextLinker *pTextLinker = CTTextLinker::GetInstance();

	if( IsEditMode() )
	{
		m_pContentEdit->ResetTextSetting();
		m_pContentEdit->ClearText();
		m_pContentEdit->m_strText = pTextLinker->MakeNetToLinkText(m_pContentEdit, strText);
		m_pContentEdit->MoveCaretToBack();

		m_pContentList->RemoveAll();
		m_pContentList->ResetTextSetting();
	}
	else
	{
		m_pContentEdit->ClearText();
		m_pContentEdit->ResetTextSetting();

		m_pContentList->RemoveAll();
		m_pContentList->ResetTextSetting();

		if( strText.IsEmpty() )
			return;

		TLLINESETTING_ARRAY2D vSETTINGS2D;
		TTLINKLINE_ARRAY vLINES;
		INT nPos = 0;

		CString strLINE = pTextLinker->SplitTextByCrLf( strText, CTTextLinker::LINK_DEF_TOK, nPos, FALSE);
		while(!strLINE.IsEmpty())
		{
			CString strPMSG = pTextLinker->MakeNetToLinkText( this, strLINE);
			strPMSG.Remove('\r');

			CSize szITEM = m_pContentList->GetItemSize(0);
			pTextLinker->SplitTextByComp( m_pContentList, szITEM.cx, strPMSG, vLINES);
			pTextLinker->GetTextSettings( this, vLINES, vSETTINGS2D);

			INT nLineSize = (INT) vLINES.size();
			for( INT i=0; i<nLineSize; i++)
			{
				const TTEXT_LINK_LINE& line = vLINES[i];
				const TLLINESETTING_ARRAY& vLineSettings = vSETTINGS2D[i];

				INT iRow = m_pContentList->AddString(line.strMSG);
				for( INT k=0; k<INT(vLineSettings.size()); k++)
				{
					const TLINK_LINE_SETTING& s = vLineSettings[k];
					m_pContentList->AddTextSetting( iRow, 0, s.nStart, s.nEnd, s.dwColor, s.dwInfo);
				}
			}

			vLINES.clear();
			vSETTINGS2D.clear();
			ResetTextSetting();

			strLINE = pTextLinker->SplitTextByCrLf( strText, CTTextLinker::LINK_DEF_TOK, nPos, FALSE);
		}
	}
}
// ======================================================================
void CTGuildAppInnerFrame::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	if(!CanProcess()) 
		return;

	if( nChar == VK_RETURN && m_pContentEdit->GetFocus() )
	{
		m_pContentEdit->InsertReturnChar();
		m_pContentEdit->MoveScrollToCaret(FALSE);
	}
	else
		ITInnerFrame::OnKeyDown(nChar,nRepCnt,nFlags);
}
// ======================================================================
void CTGuildAppInnerFrame::OnLButtonDown( UINT nFlags, CPoint pt )
{
	if( m_pSelecter && IsEditMode() )
		m_pSelecter->HitTest(pt);

	if(HitTest(pt))
	{
		CTTextLinker *pTextLinker = CTTextLinker::GetInstance();
		CTClientGame *pTGAME = CTClientGame::GetInstance();
		DWORD dwInfo;

		if( m_pContentList->HitTestTextSetting(pt, &dwInfo) )
		{
			CTTextLinkData *pLinkData = pTextLinker->GetTextLinkData(dwInfo);

			if( pLinkData && pLinkData->IsItemType() )
			{
				CTClientItem *pItem = pLinkData->GetLastUnkpackItem();

				if(pItem)
					pTGAME->ShowChatDetailInfo(pItem);

				return;
			}
		}

		SwitchFocus(m_pContentList);
	}

	ITInnerFrame::OnLButtonDown(nFlags,pt);
}
// ----------------------------------------------------------------------
void CTGuildAppInnerFrame::OnRButtonDown( UINT nFlags, CPoint pt )
{
	if( m_pList && m_pList->CanProcess() )
		m_pList->SetCurSelItem(T_INVALID);
}
// ----------------------------------------------------------------------
void CTGuildAppInnerFrame::SwitchFocus( TComponent *pCandidate )
{
	if( m_pFocus == pCandidate )
		return;

	if( m_pFocus )
	{
		m_pFocus->SetFocus(FALSE);
		m_pFocus->OnLoseFocus(pCandidate);
	}

	if( pCandidate )
	{
		pCandidate->SetFocus(TRUE);
		pCandidate->OnSetFocus(m_pFocus);
	}

	m_pFocus = pCandidate;
}
// ----------------------------------------------------------------------
void CTGuildAppInnerFrame::ShowComponent( BOOL bVisible )
{
	if( m_bVisible == bVisible )
		return;

	ITInnerFrame::ShowComponent(bVisible);

	if( bVisible )
	{
		m_bVisible = bVisible;

		if( !m_bInit )
			Init();
		
		RequestInfo();
	}
}
// ======================================================================
HRESULT CTGuildAppInnerFrame::Render( DWORD dwTickCount )
{
	if( m_nCurMode == (UINT)T_INVALID )
		return S_OK;

	if( IsVisible() && !IsEditMode() )
	{
	 	int nCurSel = m_pList->GetSel();
		if( m_nPrvSel == T_INVALID || m_nPrvSel != nCurSel )
			UpdateByListSel();

		m_nPrvSel = nCurSel;
	}

	return ITInnerFrame::Render(dwTickCount);
}
// ====================================================================













