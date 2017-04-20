// TMultiLineEdit.cpp: implementation of the TMultiLineEdit class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#define LINE_SPACE_RATE		1.4f

TMultiLineEdit::TMultiLineEdit(TComponent *pParent, LP_FRAMEDESC pDesc)
	: TEdit(pParent,pDesc), m_pScroll(NULL), m_nFontHeight(0), m_nLinePerPage(0),
	  m_nPrvCaret(0), m_nCtLine(0), m_nCtPosInLine(0), m_nScrollPos(0),
	  m_nCUDLinePos(0), m_bSetupCUD(FALSE), m_nWheelMoveValue(2)
{
	TCOMP_LIST::iterator itr = GetFirstKidsFinder();
	while( !EndOfKids(itr) )
	{
		TComponent* pComp = GetNextKid(itr);
		if( pComp->IsTypeOf(TCML_TYPE_SCROLL) )
		{
			m_pScroll = static_cast<TScroll*>(pComp);
			
			CPoint pt;
			m_pScroll->GetComponentPos(&pt);
			m_nScrollPos = m_rc.Width() - pt.x;

			break;
		}
	}

	m_bShowCaret = m_pCaret? TRUE: FALSE;
}

TMultiLineEdit::~TMultiLineEdit()
{
}

void TMultiLineEdit::InsertReturnChar()
{
	if( m_nCaret >= m_strText.GetLength() )
		m_strText.AppendChar('\n');
	else
		m_strText.Insert(m_nCaret, '\n');

	RightCaret(FALSE);
	Update();
}

void TMultiLineEdit::MoveScrollToCaret(BOOL bTop)
{
	if( !m_pScroll )
		return;

	INT nRng, nPos;
	m_pScroll->GetScrollPos(nRng,nPos);

	if( bTop )
	{
		nPos = m_nCtLine;
		if( nPos > nRng )
			nPos = nRng;
	}
	else
	{
		nPos = m_nCtLine - m_nLinePerPage + 1;
		if( nPos < 0 )
			nPos = 0;
	}

	m_pScroll->SetScrollPos(nRng,nPos);
}

void TMultiLineEdit::MoveScrollToCaretAuto()
{
	if( !m_pScroll )
		return;

	INT nScrPos = GetScrollPos();
	if( nScrPos > m_nCtLine )
		MoveScrollToCaret(TRUE);
	else if( nScrPos+m_nLinePerPage <= m_nCtLine ) 
		MoveScrollToCaret(FALSE);
}

INT TMultiLineEdit::GetScrollPos()
{
	INT nPos(0), nRng(0);
	if( m_pScroll )
		m_pScroll->GetScrollPos(nRng,nPos);
		
	return nPos;
}

INT TMultiLineEdit::GetScrollRng()
{
	INT nPos(0), nRng(0);
	if( m_pScroll )
		m_pScroll->GetScrollPos(nRng,nPos);
		
	return nRng;
}

void TMultiLineEdit::Update()
{
	if( m_nFontHeight == 0 || m_nLinePerPage == 0 )
	{
		CString strTEMP("ABC");

		CSize szTEXT;
		GetTextExtentPoint(strTEMP, szTEXT);

		CSize szCTRL = GetTextArea();
		int nLINE = INT(min(szCTRL.cy, szTEXT.cy) * LINE_SPACE_RATE);
		nLINE = min(szCTRL.cy, nLINE);

		m_nFontHeight = nLINE;
		m_nLinePerPage = szCTRL.cy/nLINE;
		
		m_nDefVMargine = m_nVMargine;
		m_nVMargine = m_nDefVMargine + (m_nFontHeight - szTEXT.cy) / 2;

		if( m_nFontHeight == 0 || m_nLinePerPage == 0 )
			return;
	}

	int nCaret = m_nCaret;
	INT nCompStart = 0;
	INT nCompEnd = 0;

	if( m_strComposition.IsEmpty() )
	{
		m_strDisplayText = m_strText;
		RemoveCompTextSetting();
	}
	else
	{
		m_strDisplayText = m_strText.Left(m_nCaret);
		CString strRemain = m_strText.Right(m_strText.GetLength() - m_nCaret);

		nCompStart = m_strDisplayText.GetLength();
		nCompEnd = nCompStart + m_strComposition.GetLength() - 1;

		m_strDisplayText += m_strComposition;
		m_strDisplayText += strRemain;		

		nCaret += m_strComposition.GetLength();
	}
	
	if( m_bNeedUpdateComposition )
		ResetCompTextSetting( nCompStart, nCompEnd );

	if( m_style & TES_PASSWORD )
	{
		for( int i = 0; i < m_strDisplayText.GetLength(); i++)
		{
			if( m_strDisplayText.GetAt(i) != '\n' )
				m_strDisplayText.SetAt(i, '*');
		}
	}

	if( m_strDisplayText != m_strPrvText || m_nPrvCaret != nCaret )
	{
		m_vTextLine.clear();
		m_vLineInfos.clear();

		m_strPrvText = m_strDisplayText;
		m_nPrvCaret = nCaret;
		
		m_bNeedUpdateTextSetting = TRUE;

		INT nPOS(0);
		CString strTEXT;

		while( SplitTextByCrLf(m_strDisplayText, nPOS, strTEXT) )
			SplitTextByLine(strTEXT, m_vTextLine, m_vLineInfos);
	}
	
	CRect rectScreen(m_rc);
	ComponentToScreen( &rectScreen);

	if( m_bNeedUpdateTextSetting ||
		m_PrvFlagAlign != m_flagAlign ||
		m_rcPrv != rectScreen ||
		m_bNeedUpdateComposition)
	{
		m_PrvFlagAlign = m_flagAlign;
		m_rcPrv = rectScreen;
		
		RecalcScrollRange(m_vTextLine);

		ClearTextOutputData();
		BuildTextOutputData(m_vTextLine, m_vLineInfos);
		
		m_bNeedUpdateComposition = FALSE;
		m_bNeedUpdateTextSetting = FALSE;

		//_LOG_(__FUNCTION__, __FILE__, __LINE__);
	}

	if( m_pCaret )
		LocateCaret(nCaret);
}

HRESULT TMultiLineEdit::DrawText()
{
	if(!CTachyonRes::m_pDEVICE)
		return E_FAIL;

	if(!m_pFont)
		return S_OK;

	Update();

	TextOutputDataSet::iterator itr,end;
	itr = m_TextOutputDataSet.begin();
	end = m_TextOutputDataSet.end();
	
	for( ; itr!=end; ++itr )
		TextOut(*itr);

	return S_OK;
}

void TMultiLineEdit::InsertString(const CString& strText)
{
	TEdit::InsertString(strText);

	Update();
	MoveScrollToCaretAuto();
}

void TMultiLineEdit::InsertString(const CString& strText, DWORD dwColor, DWORD dwID)
{
	TEdit::InsertString(strText,dwColor,dwID);

	Update();
	MoveScrollToCaretAuto();
}

void TMultiLineEdit::PushFrontString(const CString& strText)
{
	TEdit::PushFrontString(strText);

	Update();
	MoveScrollToCaretAuto();
}

void TMultiLineEdit::ShowComponent( BOOL bVisible )
{
	TEdit::ShowComponent(bVisible);

	if( bVisible && m_pCaret && !m_bShowCaret )
		m_pCaret->ShowComponent(FALSE);
}

BOOL TMultiLineEdit::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if(!CanProcess() || !m_pScroll) 
		return FALSE;

	CRect rc = m_rc;
	ComponentToScreen(&rc);
	if(!rc.PtInRect(pt))
		return FALSE;

	int nRange, nPos;
	m_pScroll->GetScrollPos( nRange, nPos);

	nPos += zDelta>0? -m_nWheelMoveValue: m_nWheelMoveValue;
	nPos = min(max(nPos, 0), nRange);

	m_pScroll->SetScrollPos( nRange, nPos);

	return TRUE;
}

void TMultiLineEdit::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if(!CanProcess() || !m_pScroll || !HitTest(pt)) 
		return;

	if( m_pScroll->HitTest(pt) )
		m_pScroll->OnLButtonDown(nFlags, pt);
	else
	{
		ScreenToComponent(&pt);

		INT nScrPos = GetScrollPos();
		INT nLineCnt = (INT) m_vLineInfos.size();
		for(INT i=nScrPos; i<nLineCnt; ++i)
		{
			INT ypos = m_vLineInfos[i].ptBase.y + (i-nScrPos)*m_nFontHeight;
			if( pt.y >= ypos && pt.y < ypos+m_nFontHeight )
			{
				const VECTORINT& vCharSize = m_vLineInfos[i].vCharSize;

				INT nHitCIdx = m_vLineInfos[i].nEnd - m_vLineInfos[i].nStart;
				INT nCharCnt = (INT) vCharSize.size();
				for(INT j=0; j<nCharCnt; ++j)
				{
					INT c = ( j == 0 )? 0: (m_vLineInfos[i].ptBase.x + vCharSize[j-1]);
					INT n = m_vLineInfos[i].ptBase.x + vCharSize[j];

					if( pt.x >= c && pt.x <= n )
					{
						if( c + ((n-c)>>1) > pt.x )
							nHitCIdx = j+1;
						else
							nHitCIdx = j;

						break;
					}
				}

				SetCaret(i,nHitCIdx);
				break;
			}
		}
	}
}

void TMultiLineEdit::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if( m_pScroll ) 
		m_pScroll->OnLButtonUp(nFlags, pt);
}

void TMultiLineEdit::OnMouseMove(UINT nFlags, CPoint pt)
{
	if(!CanProcess() || !m_pScroll || !HitTest(pt)) 
		return;

	m_pScroll->OnMouseMove(nFlags, pt);
}

void TMultiLineEdit::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	if(m_blIsLocked)
		return;
	BYTE bMBCSByte = FALSE;

	if(m_style & TES_PASSWORD)
		if( nChar == VK_PROCESSKEY )
		{
			nChar = SCodeToAscii(nFlags);

			if( (nChar >= 'A' && nChar <= 'Z') ||
				(nChar >= 'a' && nChar <= 'z') )
			{
				m_strText.Insert( m_nCaret, nChar);
				RightCaret(FALSE);

				TRACE("KeyDown:%c\n", nChar);
				m_bProcPASS = TRUE;
			}
			else
				m_bProcPASS = FALSE;

			return;
		}
		else
			m_bProcPASS = FALSE;

	switch(nChar)
	{  
	case VK_NEXT	: m_nCaret = m_strText.GetLength(); break;
	case VK_PRIOR	: m_nCaret = 0; break;
	case VK_LEFT	:
		{
			if( m_nCaret > 0 )
			{
				CString strMBCS = BuildMBCSInfo(m_strText);
				bMBCSByte = IS_MBCS_TAIL(strMBCS.GetAt(m_nCaret - 1));
			}

			LeftCaret(bMBCSByte);
		}

		break;

	case VK_RIGHT	:
		{
			if( m_nCaret < m_strText.GetLength() )
			{
				CString strMBCS = BuildMBCSInfo(m_strText);
				bMBCSByte = IS_MBCS_LEAD(strMBCS.GetAt(m_nCaret));
			}

			RightCaret(bMBCSByte);
		}

		break;

	case VK_DOWN	: DownCaret(); break;
	case VK_UP		: UpCaret(); break;
	case VK_HOME	: MoveCaretLineFront(); break;
	case VK_END		: MoveCaretLineBack(); break;
	case VK_BACK	:
		if( m_nCaret > 0 )
		{
			CString strMBCS = BuildMBCSInfo(m_strText);

			bMBCSByte = IS_MBCS_TAIL(strMBCS.GetAt(m_nCaret - 1));
			TextSetting setting;

			if(FindTextSetting( m_nCaret - 1, setting))
				RemoveTextSetting(setting);
			MoveTextSetting( m_nCaret, bMBCSByte ? -2 : -1);

			CString strRight = m_strText.Right(m_strText.GetLength() - m_nCaret);
			LeftCaret(bMBCSByte);
			CString strLeft = m_strText.Left(m_nCaret);

			m_nDisplayPos -= bMBCSByte ? 2 : 1;
			m_strText = strLeft + strRight;

			if( m_nDisplayPos < 0 )
				m_nDisplayPos = 0;
		}

		break;

	case VK_DELETE	:
		if( m_nCaret < m_strText.GetLength() )
		{
			CString strMBCS = BuildMBCSInfo(m_strText);

			bMBCSByte = IS_MBCS_LEAD(strMBCS.GetAt(m_nCaret));
			TextSetting setting;

			if(FindTextSetting( m_nCaret, setting))
				RemoveTextSetting(setting);
			MoveTextSetting( m_nCaret, bMBCSByte ? -2 : -1);

			CString strRight = m_strText.Right(m_strText.GetLength() - (m_nCaret + (bMBCSByte ? 2 : 1)));
			CString strLeft = m_strText.Left(m_nCaret);

			m_strText = strLeft + strRight;
		}

		break;
	}
}

void TMultiLineEdit::OnChar( UINT nChar, int nRepCnt, UINT nFlags)
{
	CString strResult;

	if(m_blIsLocked)
		return;

	HandleChar(
		(TCHAR) nChar,
		nRepCnt,
		strResult);

	if( !m_bProcPASS && !strResult.IsEmpty() )
	{
		TextSetting setting;
		if( FindTextSetting(m_nCaret, setting) )
		{
			if( m_nCaret != setting.iStart )
				RemoveTextSetting(setting);
		}

		INT iLen = strResult.GetLength();
		MoveTextSetting(m_nCaret, iLen);

		m_strText.Insert( m_nCaret, strResult);
		RightCaret(FALSE);

		Update();
		MoveScrollToCaretAuto();
	}

	TComponent::OnChar( nChar, nRepCnt, nFlags);
}

void TMultiLineEdit::OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	TEdit::g_hWnd = hWnd;

	if( m_blIsLocked || (m_style & TES_PASSWORD) ) 
		return;

	BOOL bComp = !m_strComposition.IsEmpty();
	INT nCompLength = m_strComposition.GetLength();

	CString strResult;
	HandleImeComposition(
		hWnd,
		wParam,
		lParam,
		m_strComposition,
		strResult,
		m_pCompAttr,
		&m_nCompAttrLength,
		m_pCompClause,
		&m_nCompClauseLength);

	if( strResult.IsEmpty() )
	{
		if(!bComp && !m_strComposition.IsEmpty())
		{
			TextSetting setting;
			if( FindTextSetting(m_nCaret, setting) )
			{
				if( m_nCaret != setting.iStart )
					RemoveTextSetting(setting);
			}
			
			MoveTextSetting(m_nCaret, m_strComposition.GetLength());
		}
		else if( bComp && m_strComposition.IsEmpty() )
		{
			MoveTextSetting(m_nCaret, -nCompLength);
			RemoveCompTextSetting();
		}
	}
	else
	{
		m_strText.Insert(m_nCaret, strResult);
		RightCaretCount( strResult.GetLength() );
		RemoveCompTextSetting();
		m_bNeedUpdateComposition = TRUE;

		Update();
		MoveScrollToCaretAuto();
	}

	m_bNeedUpdateComposition = TRUE;
}

HRESULT TMultiLineEdit::TextOut(const TextOutputData& ptd)
{
	CD3DFont* pFont = ptd.pFont? ptd.pFont: m_pFont;

	if(pFont)
	{
		INT nScrollPos = GetScrollPos();
		if( ptd.iLine < nScrollPos || nScrollPos+m_nLinePerPage <= ptd.iLine )
			return S_OK;

		CPoint pt = ptd.ptPoint;
		pt.y += (ptd.iLine-nScrollPos) * m_nFontHeight;
		ComponentToScreen(&pt);

		pFont->TextOut(
			CTachyonRes::m_pDEVICE->m_pDevice,
			ptd.strText, pt.x, pt.y);

		if( ptd.pTextLine)
			ptd.pTextLine->Draw( CTachyonRes::m_pDEVICE->m_pDevice);
	}

	return S_OK;
}

void TMultiLineEdit::SetCaret( INT nLine, INT nPos)
{
	const LineInfo& nline = m_vLineInfos[nLine];
	m_nCaret = nline.nStart + nPos;

	if( m_vTextLine[nLine].IsEmpty() )
		return;

	if( m_nCaret >= nline.nEnd )
	{
		TCHAR c = m_vTextLine[nLine].GetAt(m_vTextLine[nLine].GetLength() - 1);
		m_nCaret = c == '\n' ? nline.nEnd - 1 : nline.nEnd;
	}
	else if( nPos > 0 )
	{
		CString strMBCS = BuildMBCSInfo(m_strDisplayText.Mid( nline.nStart, nPos + 1));

		if(IS_MBCS_TAIL(strMBCS.GetAt(nPos)))
			m_nCaret++;
	}
}

void TMultiLineEdit::UpCaret()
{
	if( m_nCtLine == 0 )
		return;

	SetCaret( --m_nCtLine, m_nCUDLinePos);
	MoveScrollToCaretAuto();
}

void TMultiLineEdit::DownCaret()
{
	INT nLineCnt = (INT) m_vLineInfos.size() - 1;
	if( m_vLineInfos.empty() || m_nCtLine>nLineCnt )
		return;

	if( m_nCtLine == nLineCnt )
	{
		m_nCaret = m_strDisplayText.GetLength();
	}
	else
	{
		SetCaret(++m_nCtLine, m_nCUDLinePos);
		MoveScrollToCaretAuto();
	}
}

void TMultiLineEdit::LeftCaret(BOOL bDouble)
{
	TEdit::LeftCaret(bDouble);
	MoveScrollToCaretAuto();

	m_bSetupCUD = TRUE;
}

void TMultiLineEdit::RightCaret(BOOL bDouble)
{
	TEdit::RightCaret(bDouble);
	MoveScrollToCaretAuto();

	m_bSetupCUD = TRUE;
}

void TMultiLineEdit::RightCaretCount(UINT uCount)
{
	TEdit::RightCaretCount( uCount);
	MoveScrollToCaretAuto();

	m_bSetupCUD = TRUE;
}

void TMultiLineEdit::MoveCaretLineFront()
{
	SetCaret(m_nCtLine, 0);
	
	m_bSetupCUD = TRUE;
}

void TMultiLineEdit::MoveCaretLineBack()
{
	const LineInfo& nline = m_vLineInfos[m_nCtLine];
	SetCaret(m_nCtLine, nline.nEnd-nline.nStart);

	m_bSetupCUD = TRUE;
}

CSize TMultiLineEdit::GetTextArea()
{
	int nWidth  = m_rc.Width();
	int nHeight = m_rc.Height();

	CSize szCTRL;
	szCTRL.cx = nWidth - (m_flagAlign == ALIGN_CENTER ? 2 * m_nHMargine : m_nHMargine) - m_nScrollPos;
	szCTRL.cy = nHeight - m_nVMargine;

	return szCTRL;
}

BOOL TMultiLineEdit::SplitTextByCrLf(const CString& strTEXT, INT& nPos, CString& outLINE)
{
	INT iLen = strTEXT.GetLength();
	if( iLen <= nPos )
		return FALSE;

	INT iStart = nPos;
	while(nPos < iLen)
	{
		TCHAR c = strTEXT.GetAt(nPos);

		if(MBCS_LEAD_TEST(c))
			nPos += 2;
		else
		{
			nPos++;

			if( c == '\n' || c == '\r' )
			{
				outLINE = strTEXT.Mid( iStart, nPos - iStart);
				return TRUE;
			}
		}
	}
	outLINE = strTEXT.Right(iLen - iStart);

	return TRUE;
}

void TMultiLineEdit::SplitTextByLine(const CString& strTEXT, VECTORSTRING& outLINESTR, LineInfoArray& outLINEINF)
{
	CSize szSIZE;
	CSize szCTRL = GetTextArea();

	CString strNEXT;
	CString strLEFT = strTEXT;

	INT nMsgIndex(0), nStartIndex(0);
	CString strTEMP, strLINE;
	VECTORINT vCHARSIZE;

	INT nPrvEnd = 0;
	if( !outLINEINF.empty() )
		nPrvEnd = outLINEINF.back().nEnd;

	while(!strLEFT.IsEmpty())
	{
		strTEMP.Empty();
		vCHARSIZE.clear();

		nStartIndex = nMsgIndex;
		GetTextExtentPoint(strLEFT, szSIZE);

		if( szSIZE.cx > szCTRL.cx )
		{
			while(!strLEFT.IsEmpty())
			{
				CHAR c = strLEFT.GetAt(0);
				strTEMP.AppendChar(c);

				BYTE bMBCS = MBCS_LEAD_TEST(c);
				if(bMBCS)
					strTEMP.AppendChar(strLEFT.GetAt(1));

				if(!strTEMP.IsEmpty())
				{
					GetTextExtentPoint( strTEMP, szSIZE);

					if( szSIZE.cx >= szCTRL.cx )
						break;

					vCHARSIZE.push_back(szSIZE.cx);
					if(bMBCS)
						vCHARSIZE.push_back(szSIZE.cx);
				}

				strLEFT.Delete(0);
				nMsgIndex++;

				if(bMBCS)
				{
					strLEFT.Delete(0);
					nMsgIndex++;
				}

				strLINE = strTEMP;
			}
		}
		else
		{
			strLINE = strLEFT;
			nMsgIndex += strLINE.GetLength();

			for( INT i=0; i<strLINE.GetLength(); i++)
			{
				strTEMP = strLINE.Left(i + 1);
				GetTextExtentPoint( strTEMP, szSIZE);
				vCHARSIZE.push_back(szSIZE.cx);
			}

			strLEFT.Empty();
		}
		outLINESTR.push_back(strLINE);	

		outLINEINF.resize(outLINEINF.size() + 1);
		LineInfo& linf = outLINEINF.back();

		linf.nLineIdx = (INT) outLINESTR.size();
		linf.nStart = nPrvEnd + nStartIndex;
		linf.nEnd = nPrvEnd + nMsgIndex;
		linf.vCharSize.swap(vCHARSIZE);
	}
}

void TMultiLineEdit::RecalcScrollRange(const VECTORSTRING& vLINESTR)
{
	int nScrollRng(0), nScrollPos(0);
	if( m_pScroll )
	{
		m_pScroll->GetScrollPos(nScrollRng,nScrollPos);

		int nLINECNT = INT(vLINESTR.size());
		if( m_nLinePerPage <= nLINECNT )
			nScrollRng = nLINECNT - m_nLinePerPage + 1;
		else
			nScrollRng = 0;

		if( nScrollPos > nScrollRng )
			nScrollPos = nScrollRng;

		m_pScroll->SetScrollPos(nScrollRng,nScrollPos);
	}
}

void TMultiLineEdit::BuildTextOutputData(const VECTORSTRING& vLINESTR, LineInfoArray& outLINEINF)
{
	if( vLINESTR.empty() )
		return;

	CString strTEXT;
	TextOutputData sTextOutputData;
	
	CSize szCTRL = GetTextArea();

	int nLINECNT = INT(vLINESTR.size());
	int nBasePos = m_rc.top + m_nVMargine;
	
	TextSettingSet::iterator itr,end;
	itr = m_TextSettingSet.begin();
	end = m_TextSettingSet.end();

	INT iSTART(0), iEND(0);
	for( INT i=0; i<nLINECNT; ++i )
	{
		INT nLineWidth(0);
		if( !outLINEINF[i].vCharSize.empty() )
			nLineWidth =  outLINEINF[i].vCharSize.back();

		const CString& strLine = vLINESTR[i];

		CPoint ptTextOut;
		ptTextOut.y = nBasePos;

		switch(m_flagAlign)
		{
		case ALIGN_CENTER	: ptTextOut.x = m_rc.left + m_nHMargine + (szCTRL.cx - nLineWidth) / 2; break;
		case ALIGN_RIGHT	: ptTextOut.x = m_rc.right - nLineWidth - m_nHMargine; break;
		default				: ptTextOut.x = m_rc.left + m_nHMargine; break;
		}

		outLINEINF[i].ptBase = ptTextOut;

		sTextOutputData.iLine = i;
		sTextOutputData.iDepth = 0;
		sTextOutputData.dwUser = 0;
		sTextOutputData.ptPoint = ptTextOut;
		sTextOutputData.strText = strLine;
		sTextOutputData.pFont = NULL;
		
		m_TextOutputDataSet.insert(sTextOutputData);

		iEND += strLine.GetLength();

		CSize szTEXT;
		while( itr != end )
		{
			TextOutputData sTextOutputData;
			sTextOutputData.iLine = i;
			sTextOutputData.iDepth = 1;

			const TextSetting& data = *itr;
			if( data.iStart > iEND )
				break;

			INT s = data.iStart - iSTART;
			if( s > 0 )
			{
				strTEXT = strLine.Left(s);
				GetTextExtentPoint(strTEXT, szTEXT);
			}
			else
				szTEXT.cx = 0;

			sTextOutputData.dwUser  = data.dwUser;
			sTextOutputData.ptPoint = ptTextOut + CPoint(szTEXT.cx,0);

			sTextOutputData.pFont = new CD3DFont();
			sTextOutputData.pFont->Release();
			sTextOutputData.pFont->SetFont(m_pGDIFont->m_pFont);
			sTextOutputData.pFont->m_dwColor = data.dwColor; 
			
			if( iEND < data.iEnd )
			{
				if( s < 0 )
					s = 0;

				sTextOutputData.strText = strLine.Right(strLine.GetLength() - s);
				GetTextExtentPoint(sTextOutputData.strText, sTextOutputData.szSize);

				INT nScrollPos = GetScrollPos();
				INT nOffset = (sTextOutputData.iLine-nScrollPos) * m_nFontHeight;
				CRect rt(
					sTextOutputData.ptPoint.x,
					sTextOutputData.ptPoint.y+nOffset,
					sTextOutputData.ptPoint.x+sTextOutputData.szSize.cx,
					sTextOutputData.ptPoint.y+sTextOutputData.szSize.cy+nOffset+2);
				ComponentToScreen( &rt );

				ResetTextSettingLine( data, rt, sTextOutputData);
				
				m_TextOutputDataSet.insert(sTextOutputData);
				break;
			}
			else
			{
				if( s > 0 )
					sTextOutputData.strText = strLine.Mid(s, data.iEnd-data.iStart+1);
				else
					sTextOutputData.strText = strLine.Left(data.iEnd-iSTART+1);
			}

			GetTextExtentPoint(sTextOutputData.strText, sTextOutputData.szSize);

			INT nScrollPos = GetScrollPos();
			INT nOffset = (sTextOutputData.iLine-nScrollPos) * m_nFontHeight;
			CRect rt(
				sTextOutputData.ptPoint.x,
				sTextOutputData.ptPoint.y+nOffset,
				sTextOutputData.ptPoint.x+sTextOutputData.szSize.cx,
				sTextOutputData.ptPoint.y+sTextOutputData.szSize.cy+nOffset+2);
			ComponentToScreen( &rt );

			ResetTextSettingLine( data, rt, sTextOutputData);

			m_TextOutputDataSet.insert(sTextOutputData);
			
			++itr;
		}

		iSTART = iEND;
	}
}

void TMultiLineEdit::LocateCaret(INT nCaret)
{
	if( !GetFocus() )
	{
		m_bShowCaret = FALSE;
		m_pCaret->ShowComponent(FALSE);
		return;
	}

	CPoint ptCaret(0, m_rc.top + m_nVMargine);

	switch(m_flagAlign)
	{
	case ALIGN_CENTER	: ptCaret.x = m_rc.left + m_nHMargine + (GetTextArea().cx / 2); break;
	case ALIGN_RIGHT	: ptCaret.x = m_rc.right - m_nHMargine; break;
	default				: ptCaret.x = m_rc.left + m_nHMargine; break;
	}

	m_bShowCaret = TRUE;
	m_nCtLine = 0;
	m_nCtPosInLine = 0;

	int nScrollPos = GetScrollPos();

	INT nLINECNT = (INT)m_vLineInfos.size();
	for(INT nLine=0; nLine<nLINECNT; ++nLine)
	{
		const LineInfo& linf = m_vLineInfos[nLine];

		INT iStart = linf.nStart;
		INT iEnd = linf.nEnd;

		if( iStart <= nCaret && iEnd >= nCaret )
		{
			m_nCtLine = nLine;
			m_nCtPosInLine = nCaret - iStart;

			if( nScrollPos > nLine || nScrollPos+m_nLinePerPage <= nLine )
				m_bShowCaret = FALSE;
			else
			{
				if( iEnd == nCaret )
				{
					if( nLine != nLINECNT-1 )
						continue;

					const CString& strLINE = m_vTextLine[nLine];
					if( !strLINE.IsEmpty() && strLINE.GetAt( strLINE.GetLength()-1 ) == '\n' )
					{
						m_nCtLine = nLine+1;
						m_nCtPosInLine = 0;

						ptCaret.y = (nLine+1-nScrollPos) * m_nFontHeight + linf.ptBase.y;
						break;
					}
				}

				if( m_bSetupCUD )
				{
					m_nCUDLinePos = m_nCtPosInLine;
					m_bSetupCUD = FALSE;
				}

				ptCaret.x = linf.ptBase.x;
				if( m_nCtPosInLine > 0 )
					ptCaret.x += linf.vCharSize[m_nCtPosInLine-1];

				ptCaret.y = (nLine-nScrollPos) * m_nFontHeight + linf.ptBase.y;
			}

			break;
		}
	}
	
	if(m_bShowCaret)
	{
		ComponentToScreen(&ptCaret);
		m_pCaret->ScreenToComponent(&ptCaret);

		ptCaret.x += m_pCaret->m_pDESC->m_vCOMP.m_nPosX;
		ptCaret.y += m_pCaret->m_pDESC->m_vCOMP.m_nPosY;

		m_pCaret->MoveComponent(ptCaret);
	}
	
	m_pCaret->ShowComponent(m_bShowCaret);
}

#ifdef DEBUG
void TMultiLineEdit::_LOG_( LPCSTR func, LPCSTR file, LONG line )
{
	CString strTEMP = m_strDisplayText;
	strTEMP.Replace('\n','@');

	TRACE("---------------------------------------------------\n");
	TRACE("< %s [FILE:%s, LINE:%u] >\n" ,func,file,line);
	TRACE("캐럿:%d\n", m_nCaret);
	TRACE("원본 문자열:%s\n", (LPCSTR)strTEMP);
	for(size_t i=0; i<m_vLineInfos.size(); ++i)
	{
		const LineInfo& lfn = m_vLineInfos[i];

		strTEMP = m_vTextLine[i];
		strTEMP.Replace('\n','@');


		TRACE("[#%u] [%d~%d, 베이스(%d,%d), [텍스트:%s]\n", 
			lfn.nLineIdx, lfn.nStart, lfn.nEnd, lfn.ptBase.x, lfn.ptBase.y, (LPCSTR)strTEMP);

		TRACE("    ->");
		for(size_t j=0; j<lfn.vCharSize.size(); ++j)
			TRACE("%d,", lfn.vCharSize[j]);

		TRACE("\n");
	}
	TRACE("---------------------------------------------------\n");
}
#endif












































