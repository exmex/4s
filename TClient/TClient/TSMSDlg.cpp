#include "Stdafx.h"
#include "TSMSDlg.h"
#include "TClientGame.h"
#include "Resource.h"
#include "Curse.h"

// ======================================================================
CTSMSDlg::CTSMSDlg(TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent, pDesc), m_nMaxLen(TMAX_SMS_CHARACTER)
{
	m_pTargetEdit	= static_cast<TEdit*>(FindKid(ID_CTRLINST_TARGET));

	TComponent* pTEMP = FindKid(ID_CTRLINST_EDIT);
	m_pMsgEdit = new TMultiLineEdit(this, pTEMP->m_pDESC);
	
	RemoveKid(pTEMP);
	AddKid(m_pMsgEdit);

	m_pMsgEdit->m_id = pTEMP->m_id;
	delete pTEMP;

	m_pLenTxt		= FindKid(ID_CTRLINST_TXT_LEN);
	m_pWarnTxt		= FindKid(ID_CTRLINST_TXT_WARN);
}
// ----------------------------------------------------------------------
CTSMSDlg::~CTSMSDlg()
{
}
// ======================================================================

// ======================================================================
void CTSMSDlg::Set(TYPE eType, BYTE bInvenID, BYTE bInvenSlot)
{
	m_eType = eType;

	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
	
	CString strTitle;
	strTitle = CTChart::Format( TSTR_FMT_SMSTITLE, pMainChar->m_strNAME);
	m_nMaxLen = TMAX_SMS_CHARACTER - strTitle.GetLength();
	m_pWarnTxt->m_strText = CTChart::Format( TSTR_FMT_SMSWARN, m_nMaxLen/2, m_nMaxLen);

	switch(m_eType)
	{
	case TYPE_PERSON: 
		{
			m_pTargetEdit->ClearText(); 
			m_pTargetEdit->EnableComponent(TRUE);
		}
		break;

	case TYPE_GUILD: 
		{
			m_pTargetEdit->m_strText = pMainChar->m_strGUILD;
			m_pTargetEdit->EnableComponent(FALSE);
		}
		break;

	default: return;
	}

	SetItem(bInvenID, bInvenSlot);

	m_pMsgEdit->ClearText();
}
// ----------------------------------------------------------------------
void CTSMSDlg::SetItem(BYTE bInvenID, BYTE bInvenSlot)
{
	m_bInvenID = bInvenID;
	m_bInvenSlot = bInvenSlot;
}
// ======================================================================
TEdit* CTSMSDlg::GetCurEdit()
{
	if( !IsVisible() )
		return NULL;

	if( m_pTargetEdit->IsVisible() && m_pTargetEdit->CanProcess() && m_pTargetEdit->GetFocus() )
		return m_pTargetEdit;

	if( m_pMsgEdit->IsVisible() && m_pMsgEdit->GetFocus() )
		return m_pMsgEdit;

	return NULL;
}
// ======================================================================
void CTSMSDlg::SendSMS()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientSession* pSession = pGame->GetSession();

	if( !pSession )
		return;

	BYTE bType;
	switch(m_eType)
	{
	case TYPE_PERSON: bType = 1; break;
	case TYPE_GUILD:  bType = 2; break;
	default: return;
	}

	CString strTARGET = m_pTargetEdit->m_strText;
	CString strMSG = CCurse::ConvertString( m_pMsgEdit->m_strText, FALSE);

	if( strMSG.GetLength() > m_nMaxLen )
		strMSG = strMSG.Left(m_nMaxLen);

	pSession->SendCS_SMSSEND_REQ(m_bInvenID, m_bInvenSlot, bType, strTARGET, strMSG);
}
// ======================================================================

// ======================================================================
void CTSMSDlg::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	if(!CanProcess()) 
		return;

	if( nChar == VK_RETURN && m_pMsgEdit->GetFocus() )
	{
		m_pMsgEdit->InsertReturnChar();
		m_pMsgEdit->MoveScrollToCaret(FALSE);
	}
	else
		CTClientUIBase::OnKeyDown(nChar,nRepCnt,nFlags);
}
// ======================================================================
HRESULT CTSMSDlg::Render( DWORD dwTickCount )
{
	if( IsVisible() )
	{
		CString str = m_pMsgEdit->m_strText + m_pMsgEdit->GetCompositionStr();

		INT nLEN = str.GetLength();
		m_pLenTxt->m_strText = CTChart::Format( TSTR_SMS_TXTLEN, nLEN, m_nMaxLen);

		CD3DFont* pFONT = m_pLenTxt->Get3DFont();
		if( nLEN > m_nMaxLen )
			pFONT->m_dwColor = TCOLOR_TXT_WARN;
		else
			pFONT->m_dwColor = TCOLOR_TXT_NORMAL;
	}

	return CTClientUIBase::Render(dwTickCount);
}
// ======================================================================























