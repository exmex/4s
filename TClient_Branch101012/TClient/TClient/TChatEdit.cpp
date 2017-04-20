#include "Stdafx.h"
#include "TChatFrame.h"
#include "TChatEdit.h"

// ===============================================================================
//CTChatEdit::CTChatEdit(CTChatFrame* pHost, LP_FRAMEDESC pDesc)
//	: TEdit(pHost, pDesc), m_pHost(pHost), m_ptPrvOuputText(T_INVALID,T_INVALID), m_nPrvStartIndex(T_INVALID)
//{
//}
// ===============================================================================

// ===============================================================================
//void CTChatEdit::InsertInfo(LPTQUEST pQuest)
//{
//	CTChatLinkData* pData;
//	DWORD dwID = m_pHost->NewChatLinkData(&pData);
//	pData->PackQuest(pQuest);
//
//	CString strText = CString(CTChatFrame::LINK_INFO_STR_START)
//		+ pQuest->m_strTITLE + CString(CTChatFrame::LINK_INFO_STR_END);
//	
//	InsertInfo(strText, CTChatFrame::QUEST_COLOR, dwID);
//}
//// -------------------------------------------------------------------------------
//void CTChatEdit::InsertInfo(LPTITEM pItem, CTClientItem* pClientItem)
//{
//	CTChatLinkData* pData;
//	DWORD dwID = m_pHost->NewChatLinkData(&pData);
//	pData->PackItem(pItem, pClientItem);
//
//	CString strText = CString(CTChatFrame::LINK_INFO_STR_START)
//		+ pItem->m_strNAME + CString(CTChatFrame::LINK_INFO_STR_END);
//
//	DWORD dwColor;
//	if( pClientItem )
//		dwColor = pClientItem->GetQualityColor();
//	else
//		dwColor = CTClientItem::m_dwNORMALCOLOR;
//
//	InsertInfo(strText, dwColor, dwID);
//}
//// -------------------------------------------------------------------------------
//void CTChatEdit::InsertInfo(const CString& strText, DWORD dwColor, DWORD dwID)
//{
//	INT nTextLen = strText.GetLength();
//	if( m_nCaret < m_strText.GetLength() )
//	{
//		TextSetting setting;
//		if( FindTextSetting(m_nCaret, setting) )
//		{
//			if( m_nCaret != setting.iStart )
//				RemoveTextSetting(setting);
//		}
//
//		MoveTextSetting(m_nCaret, nTextLen);
//		m_strText.Insert(m_nCaret, strText);
//	}
//	else
//		m_strText += strText;
//
//	AddTextSetting(m_nCaret, m_nCaret+nTextLen-1, dwColor, dwID);
//	m_nCaret += nTextLen;
//}
//// ===============================================================================
//void CTChatEdit::PushFrontMsg(const CString& strMsg)
//{
//	TextSetting setting;
//	if( FindTextSetting(0, setting) )
//	{
//		if( setting.iStart != 0 )
//			RemoveTextSetting(setting);
//	}
//
//	INT nMsgLen = strMsg.GetLength();
//	MoveTextSetting(0, nMsgLen);
//	m_strText.Insert(0, strMsg);
//
//	m_nCaret = nMsgLen;
//}
// ===============================================================================
//CString CTChatEdit::MakeSendChatMsg()
//{
//	if( IsTextSettingEmpty() )
//		return m_strText;
//
//	CString strResult;
//	strResult.AppendChar( CTChatFrame::CHAT_LINK_MARK );
//	strResult.AppendChar( m_TextSettingSet.size() );
//
//	INT nLast = 0;
//
//	TextSettingSet::iterator itr,end;
//	itr = m_TextSettingSet.begin();
//	end = m_TextSettingSet.end();
//
//	for( ; itr != end; ++itr )
//	{
//		const TextSetting& data = *itr;
//
//		INT nPrvLen = data.iStart - nLast;
//		if( nPrvLen )
//			strResult += m_strText.Mid(nLast, nPrvLen);
//
//		CTChatLinkData* pLinkData = m_pHost->GetChatLinkData( data.dwUser );
//		if( pLinkData )
//		{
//			const CString& strLinkData = pLinkData->ToStr();
//			BYTE nLinkDataLen = (BYTE)strLinkData.GetLength();
//
//			if( nLinkDataLen )
//			{
//				strResult.AppendChar( CTChatFrame::CHAT_LINK_MARK );
//				strResult.AppendChar( nLinkDataLen );
//				strResult += strLinkData;
//			}
//		}
//
//		nLast = data.iEnd + 1;
//	}
//
//	if( nLast < m_strText.GetLength() )
//		strResult += m_strText.Mid(nLast, m_strText.GetLength() - nLast);
//
//	return strResult;
//}
// ===============================================================================

// ===============================================================================
//HRESULT CTChatEdit::DrawText()
//{
//	if(!CTachyonRes::m_pDEVICE)
//		return E_FAIL;
//
//	if(!m_pFont || !IsVisible())
//		return S_OK;
//
//	//construct full text includint IME comp
//	int nCaret = m_nCaret;
//
//	if(m_strComposition.GetLength() <= 0)
//		m_strDisplayText = m_strText;
//	else
//	{
//		m_strDisplayText = m_strText.Left(m_nCaret);
//		CString strRemain = m_strText.Right(m_strText.GetLength() - m_nCaret);
//
//		m_strDisplayText += m_strComposition;
//		m_strDisplayText += strRemain;		
//
//		nCaret +=2;
//	}
//
//	if( m_style & TES_PASSWORD )
//	{//replace all password to asterisk '*'
//		for( int i = 0; i < m_strDisplayText.GetLength(); i++)
//			m_strDisplayText.SetAt(i, '*');
//	}
//
//	//Set up the start position of drawing text
//	CString strDrawBuf;
//	CPoint ptCaret, ptText;
//
//	int nFrom = 0;
//	BOOL bContinue = TRUE;
//
//	if(m_strDisplayText.GetLength() > 0)
//	{
//		while(bContinue)
//		{
//			if(nFrom > nCaret) nFrom --;
//
//			if(nFrom < 0) nFrom = 0;
//
//			if(nFrom > m_strDisplayText.GetLength())
//				nFrom = m_strDisplayText.GetLength();
//
//			int nRight = m_strDisplayText.GetLength() - nFrom;
//			if(nRight < 0) nRight = 0;
//
//			strDrawBuf = m_strDisplayText.Right(nRight);
//
//			TextToPoint(strDrawBuf, nCaret - nFrom, ptCaret);
//			TextToPoint(strDrawBuf, 0, ptText);
//
//			if( nFrom < 0 ||
//				m_nTextExtent > ptCaret.x - ptText.x|| 
//				nFrom >= m_strDisplayText.GetLength()) 
//				bContinue = FALSE;
//			else
//			{
//				TCHAR chCharCode = m_strDisplayText.GetAt(nFrom);
//				
//				nFrom++;
//				if(IsDBCSLeadByte(chCharCode)) nFrom ++;
//			}
//		}
//	}
//
//	if( m_pCaret && m_pCaret->IsVisible() && !m_blIsLocked )
//	{//set caret position
//		if(strDrawBuf.IsEmpty())
//			TextToPoint(strDrawBuf, 0, ptCaret);
//
//		ComponentToScreen(&ptCaret);		
//		m_pCaret->ScreenToComponent(&ptCaret);
//
//		ptCaret.x += m_pCaret->m_pDESC->m_vCOMP.m_nPosX;
//		ptCaret.y += m_pCaret->m_pDESC->m_vCOMP.m_nPosY;
//
//		m_pCaret->MoveComponent(ptCaret);
//	}
//
//	//Set up the end position of drawing text
//	if(strDrawBuf.GetLength() > 0)
//	{
//		bContinue = TRUE;
//		while(bContinue)
//		{
//			CSize size;
//			GetTextExtentPoint(strDrawBuf, size);
//			if( m_nTextExtent > size.cx )
//				bContinue = FALSE;
//			else
//			{
//				if(strDrawBuf.GetLength() <= 0)
//					return S_OK;
//
//				strDrawBuf.Delete(strDrawBuf.GetLength()-1);
//			}
//		}
//	}
//	
//	if(strDrawBuf.GetLength() <= 0)
//		return S_OK;
//
//    TextToPoint(strDrawBuf, -1, ptText);
//
//	TextOutputData sTextOutputData;
//	if( m_strPrvText != m_strText || m_ptPrvOuputText != ptText || m_strPrvOuputText != strDrawBuf )
//	{
//		ClearTextOutputData();
//		
//		m_strPrvText = m_strText;
//		m_ptPrvOuputText = ptText;
//		m_strPrvOuputText = strDrawBuf;
//		m_bNeedUpdateTextSetting = TRUE;
//		
//		sTextOutputData.iDepth	= 0;
//		sTextOutputData.dwUser	= 0;
//		sTextOutputData.pFont	= NULL;
//		sTextOutputData.ptPoint = ptText;
//		sTextOutputData.strText = strDrawBuf;
//
//		m_TextOutputDataSet.insert(sTextOutputData);
//	}
//	
//	if( m_bNeedUpdateTextSetting || m_nPrvStartIndex != nFrom )
//	{
//		m_nPrvStartIndex = nFrom;
//
//		CSize szTEXT;
//		CString strTEXT;
//		
//		INT iSTART = nFrom;
//		INT iEND = iSTART + strDrawBuf.GetLength();
//
//		TextSettingSet::iterator itr,end;
//		itr = m_TextSettingSet.begin();
//		end = m_TextSettingSet.end();
//
//		sTextOutputData.iDepth = 1;
//
//		while(itr != end)
//		{
//			const TextSetting& data = *itr;
//			if( data.iStart > iEND )
//				break;
//
//			INT s = data.iStart - iSTART;
//			if( s > 0 )
//			{
//				strTEXT = strDrawBuf.Left(s);
//				GetTextExtentPoint(strTEXT, szTEXT);
//			}
//			else
//				szTEXT.cx = 0;
//
//			sTextOutputData.dwUser = data.dwUser;
//			sTextOutputData.ptPoint = ptText + CPoint(szTEXT.cx,0);
//
//			sTextOutputData.pFont = new CD3DFont();
//			sTextOutputData.pFont->Release();
//			sTextOutputData.pFont->SetFont(&m_pGDIFont->m_font);
//			sTextOutputData.pFont->m_dwColor = data.dwColor; 
//			
//			if( iEND < data.iEnd )
//			{
//				if( s < 0 )
//					s = 0;
//
//				sTextOutputData.strText = strDrawBuf.Right(strDrawBuf.GetLength() - s);
//				GetTextExtentPoint(sTextOutputData.strText, sTextOutputData.szSize);
//				m_TextOutputDataSet.insert(sTextOutputData);
//				break;
//			}
//			else
//			{
//				if( s > 0 )
//					sTextOutputData.strText = strDrawBuf.Mid(s, data.iEnd-data.iStart+1);
//				else
//					sTextOutputData.strText = strDrawBuf.Left(data.iEnd-iSTART+1);
//			}
//
//			GetTextExtentPoint(sTextOutputData.strText, sTextOutputData.szSize);
//			m_TextOutputDataSet.insert(sTextOutputData);
//
//			++itr;
//		}
//
//		m_bNeedUpdateTextSetting = FALSE;
//	}
//
//	TextOutputDataSet::iterator titr,tend;
//	titr = m_TextOutputDataSet.begin();
//	tend = m_TextOutputDataSet.end();
//	
//	for( ; titr!=tend; ++titr )
//		TextOut(*titr);
//
//	return S_OK;
//}
//// -------------------------------------------------------------------------------
//void CTChatEdit::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
//{
//	if( m_blIsLocked || !IsVisible() ) return;
//
//	BOOL bDESCLeadByte = FALSE;
//
//	switch(nChar)
//	{
//	case VK_HOME:
//		m_nCaret= 0;
//		break;
//	case VK_END:
//		m_nCaret = m_strText.GetLength();
//		break;
//	case VK_LEFT:
//		{
//			if(m_nCaret >= 1)
//			{
//				bDESCLeadByte = IsDBCSLeadByte(m_strText.GetAt(m_nCaret -1));
//				LeftCaret(bDESCLeadByte);
//			}
//		}
//		break;
//	case VK_RIGHT:
//		{
//			if(m_nCaret < m_strText.GetLength())
//			{
//				bDESCLeadByte = IsDBCSLeadByte(m_strText.GetAt(m_nCaret));
//				RightCaret(bDESCLeadByte);
//			}
//		}
//		break;
//	case VK_BACK:
//		{
//			if(m_nCaret >= 1)
//			{
//				bDESCLeadByte = IsDBCSLeadByte(m_strText.GetAt(m_nCaret -1));
//				
//				TextSetting setting;
//				if( FindTextSetting(m_nCaret-1, setting) )
//					RemoveTextSetting(setting);
//
//				MoveTextSetting(m_nCaret, bDESCLeadByte? -2: -1);
//			
//				CString strRight = m_strText.Right(m_strText.GetLength() - m_nCaret);
//				LeftCaret(bDESCLeadByte);
//				CString strLeft = m_strText.Left(m_nCaret);
//
//				m_strText = strLeft + strRight;
//			}
//		}
//		break;
//	case VK_DELETE:
//		{
//			if(m_nCaret < m_strText.GetLength())
//			{
//				bDESCLeadByte = IsDBCSLeadByte(m_strText.GetAt(m_nCaret));
//
//				TextSetting setting;
//				if( FindTextSetting(m_nCaret, setting) )
//					RemoveTextSetting(setting);
//
//				MoveTextSetting(m_nCaret, bDESCLeadByte? -2: -1);
//
//				CString strRight = m_strText.Right(m_strText.GetLength() - (m_nCaret+((bDESCLeadByte)?2:1)));
//				CString strLeft = m_strText.Left(m_nCaret);
//				m_strText = strLeft + strRight;
//			}			
//		}
//		break;
//	}
//}
//// -------------------------------------------------------------------------------
//void CTChatEdit::OnChar( UINT nChar, int nRepCnt, UINT nFlags)
//{
//	CString strResult;
//
//	if(m_blIsLocked || !IsVisible())
//		return;
//
//	HandleChar(
//		(TCHAR) nChar,
//		nRepCnt,
//		strResult);
//
//	INT iLen = strResult.GetLength();
//	if( iLen > 0 )
//	{
//		TextSetting setting;
//		if( FindTextSetting(m_nCaret, setting) )
//		{
//			if( m_nCaret != setting.iStart )
//				RemoveTextSetting(setting);
//		}
//
//		MoveTextSetting(m_nCaret, iLen);
//
//		m_strText.Insert( m_nCaret, strResult);
//		RightCaret(FALSE);
//	}
//
//	TComponent::OnChar( nChar, nRepCnt, nFlags);
//}
//// -------------------------------------------------------------------------------
//void CTChatEdit::OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
//{
//	if( m_blIsLocked || !IsVisible() ) return;
//
//	BOOL bComp = !m_strComposition.IsEmpty();
//
//	CString strResult;
//	HandleImeComposition( hWnd, wParam, lParam, m_strComposition, strResult);
//
//	if( strResult.IsEmpty() )
//	{
//		if(!bComp && !m_strComposition.IsEmpty())
//		{
//			TextSetting setting;
//			if( FindTextSetting(m_nCaret, setting) )
//			{
//				if( m_nCaret != setting.iStart )
//					RemoveTextSetting(setting);
//			}
//			
//			MoveTextSetting(m_nCaret, 2);
//		}
//		else if( bComp && m_strComposition.IsEmpty() )
//		{
//			MoveTextSetting(m_nCaret, -2);
//		}
//	}
//	else
//	{
//		m_strText.Insert(m_nCaret, strResult);
//		RightCaret(TRUE);
//	}
//}
//// ===============================================================================
//
//
//// ===============================================================================
//void CTChatEdit::MoveTextSetting(INT iFrom, INT iCount)
//{
//	TextSettingSet::iterator itr, end;
//	itr = GetTextSettingItr(iFrom);
//	end = m_TextSettingSet.end();
//
//	for( ; itr!=end; ++itr )
//	{
//		itr->iStart += iCount;
//		itr->iEnd += iCount;
//	}
//}
// ===============================================================================























