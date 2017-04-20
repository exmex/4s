// TEdit.cpp: implementation of the TEdit class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

TCandidate* TEdit::m_pCandidate = NULL;
TEdit* TEdit::m_pCandidateOwner = NULL;
CRect TEdit::m_rtCandidateTarget;
HWND TEdit::g_hWnd = NULL;
UINT TEdit::m_nCountryFlag = COUNTRY_FLAG_KOREA;
BOOL TEdit::g_bPassImeNotify = FALSE;
HKL TEdit::m_hKL = NULL;

CHAR TEdit::SCodeToAscii( UINT nScanCode)
{
	BYTE bUpper = ::GetKeyState(VK_CAPITAL) & 0x0001 ? TRUE : FALSE;

	if(::GetKeyState(VK_SHIFT) & 0x0100)
		bUpper = !bUpper;

	CHAR nChar[2] = {
		CHAR(MapVirtualKey( LOBYTE(nScanCode), 1)),
		NULL};

	if(!bUpper)
		strlwr(nChar);

	return nChar[0];
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TEdit::TEdit(TComponent *pParent, LP_FRAMEDESC pDesc)
:TComponent(pParent, pDesc)
{
	m_bType = TCML_TYPE_EDIT;
	m_style |= TS_FOCUSABLE;
	m_blIsLocked = FALSE;
	m_nCaret = m_strText.GetLength();
	m_bProcDBCS = FALSE;
	m_bProcPASS = FALSE;
	m_ptPrvOuputText = CPoint(-1,-1);
	m_nPrvStartIndex = -1;
	m_nDisplayPos = 0;
	m_nCompCaretOffset = 0;
	m_bConverting = FALSE;
	m_bNeedUpdateComposition = FALSE;

	if( pDesc )
	{
		m_pCaret = FindKid( pDesc->m_vCOMP.m_vEX.edit.caret );

		if( m_pCaret )
		{
			m_pCaret->ShowComponent( GetFocus() );
			m_pCaret->m_blIsCaret = TRUE;
		}
	}
}

TEdit::~TEdit()
{
}

void TEdit::InsertString(const CString& strText)
{
	INT nTextLen = strText.GetLength();
	if( m_nCaret < m_strText.GetLength() )
	{
		TextSetting setting;
		if( FindTextSetting(m_nCaret, setting) )
		{
			if( m_nCaret != setting.iStart )
				RemoveTextSetting(setting);
		}

		MoveTextSetting(m_nCaret, nTextLen);
		m_strText.Insert(m_nCaret, strText);
	}
	else
		m_strText += strText;

	m_nCaret += nTextLen;
}

void TEdit::InsertString(const CString& strText, DWORD dwColor, DWORD dwID)
{
	INT nTextLen = strText.GetLength();

	InsertString(strText);
	AddTextSetting(m_nCaret-nTextLen, m_nCaret-1, dwColor, dwID);
}

void TEdit::PushFrontString(const CString& strText)
{
	TextSetting setting;
	if( FindTextSetting(0, setting) )
	{
		if( setting.iStart != 0 )
			RemoveTextSetting(setting);
	}

	INT nTextLen = strText.GetLength();
	MoveTextSetting(0, nTextLen);
	m_strText.Insert(0, strText);

	m_nCaret = nTextLen;
}

HRESULT TEdit::DrawText()
{
	if(!CTachyonRes::m_pDEVICE)
		return E_FAIL;

	if(!m_pFont)
		return S_OK;

	//construct full text includint IME comp
	int nCaret = m_nCaret;

	INT nCompStart = 0;
	INT nCompEnd = 0;

	if(m_strComposition.GetLength() <= 0)
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

	if( m_bConverting )
		nCaret = nCompStart + m_nCompCaretOffset;

	if( m_bNeedUpdateComposition )
		ResetCompTextSetting( nCompStart, nCompEnd );

	if( m_style & TES_PASSWORD )
	{//replace all password to asterisk '*'
		for( int i = 0; i < m_strDisplayText.GetLength(); i++)
			m_strDisplayText.SetAt(i, '*');
	}

	//Set up the start position of drawing text
	CString strDrawBuf;
	CPoint ptCaret, ptText;

	int nFrom = 0;
	if( m_bConverting )
		nFrom = min(m_nDisplayPos, nCaret);
	else
		nFrom = min(m_nDisplayPos, m_nCaret);
	BOOL bContinue = TRUE;

	if(m_strDisplayText.GetLength() > 0)
	{
		CString strMBCS = BuildMBCSInfo(m_strDisplayText);

		while(bContinue)
		{
			if(nFrom > nCaret)
				nFrom --;

			if(nFrom < 0)
				nFrom = 0;

			if(nFrom > m_strDisplayText.GetLength())
				nFrom = m_strDisplayText.GetLength();

			int nRight = m_strDisplayText.GetLength() - nFrom;

			if(nRight < 0)
				nRight = 0;
			strDrawBuf = m_strDisplayText.Right(nRight);

			TextToPoint(strDrawBuf, nCaret - nFrom, ptCaret);
			TextToPoint(strDrawBuf, 0, ptText);

			if( nFrom < 0 ||
				m_nTextExtent > ptCaret.x - ptText.x|| 
				nFrom >= m_strDisplayText.GetLength()) 
				bContinue = FALSE;
			else
			{
				if(IS_MBCS_LEAD(strMBCS.GetAt(nFrom)))
					nFrom++;

				nFrom++;
			}
		}
	}
	m_nDisplayPos = nFrom;

	if( m_pCaret && !m_blIsLocked )
	{
		//set caret position
		if(strDrawBuf.IsEmpty())
			TextToPoint(strDrawBuf, 0, ptCaret);

		ComponentToScreen(&ptCaret);		
		m_pCaret->ScreenToComponent(&ptCaret);

		ptCaret.x += m_pCaret->m_pDESC->m_vCOMP.m_nPosX;
		ptCaret.y += m_pCaret->m_pDESC->m_vCOMP.m_nPosY;

		m_pCaret->MoveComponent(ptCaret);
		m_pCaret->ShowComponent( (GetFocus() && !m_bConverting) );
	}

	//Set up the end position of drawing text
	if(strDrawBuf.GetLength() > 0)
	{
		CString strMBCS = BuildMBCSInfo(strDrawBuf);
		CString strTEMP;

		for( INT i=0; i<strDrawBuf.GetLength(); i++)
		{
			CHAR bCODE = strDrawBuf.GetAt(i);
			strTEMP.AppendChar(bCODE);

			BYTE bLead = IS_MBCS_LEAD(strMBCS.GetAt(i));
			if(bLead)
				strTEMP.AppendChar(strDrawBuf.GetAt(i + 1));

			CSize size;
			GetTextExtentPoint( strTEMP, size);

			if( m_nTextExtent < size.cx )
			{
				strTEMP.Delete( i, bLead ? 2 : 1);
				break;
			}

			if(bLead)
				i++;
		}

		strDrawBuf = strTEMP;
	}

	if( strDrawBuf.GetLength() <= 0 )
		return S_OK;
	TextToPoint( strDrawBuf, -1, ptText);

	CRect rectScreen(m_rc);
	ComponentToScreen( &rectScreen);

	TextOutputData sTextOutputData;
	if( m_strPrvText != m_strText ||
		m_ptPrvOuputText != ptText ||
		m_strPrvOuputText != strDrawBuf ||
		m_rcPrv != rectScreen ||
		m_bNeedUpdateComposition )
	{
		m_rcPrv = rectScreen;
		ClearTextOutputData();

		m_strPrvText = m_strText;
		m_ptPrvOuputText = ptText;
		m_strPrvOuputText = strDrawBuf;
		m_bNeedUpdateTextSetting = TRUE;

		sTextOutputData.iDepth	= 0;
		sTextOutputData.dwUser	= 0;
		sTextOutputData.pFont	= NULL;
		sTextOutputData.ptPoint = ptText;
		sTextOutputData.strText = strDrawBuf;
		sTextOutputData.pTextLine = NULL;

		m_TextOutputDataSet.insert(sTextOutputData);
	}

	if( m_bNeedUpdateTextSetting ||
		m_nPrvStartIndex != nFrom )
	{
		m_nPrvStartIndex = nFrom;

		CSize szTEXT;
		CString strTEXT;
		
		INT iSTART = nFrom;
		INT iEND = iSTART + strDrawBuf.GetLength();

		TextSettingSet::iterator itr,end;
		itr = m_TextSettingSet.begin();
		end = m_TextSettingSet.end();

		while(itr != end)
		{
			TextOutputData sTextOutputData;
			sTextOutputData.iDepth = 1;

			const TextSetting& data = *itr;
			if( data.iStart > iEND )
				break;

			INT s = data.iStart - iSTART;
			if( s > 0 )
			{
				strTEXT = strDrawBuf.Left(s);
				GetTextExtentPoint(strTEXT, szTEXT);
			}
			else
				szTEXT.cx = 0;

			sTextOutputData.dwUser = data.dwUser;
			sTextOutputData.ptPoint = ptText + CPoint(szTEXT.cx,0);

			sTextOutputData.pFont = new CD3DFont();
			sTextOutputData.pFont->Release();
			sTextOutputData.pFont->SetFont(m_pGDIFont->m_pFont);
			sTextOutputData.pFont->m_dwColor = data.dwColor; 
			
			if( iEND < data.iEnd )
			{
				if( s < 0 )
					s = 0;

				sTextOutputData.strText = strDrawBuf.Right(strDrawBuf.GetLength() - s);
				GetTextExtentPoint(sTextOutputData.strText, sTextOutputData.szSize);

				CRect rt(
					sTextOutputData.ptPoint.x,
					sTextOutputData.ptPoint.y,
					sTextOutputData.ptPoint.x+sTextOutputData.szSize.cx,
					sTextOutputData.ptPoint.y+sTextOutputData.szSize.cy);
				ComponentToScreen( &rt );
				ResetTextSettingLine( data, rt, sTextOutputData);

				m_TextOutputDataSet.insert(sTextOutputData);
				break;
			}
			else
			{
				if( s > 0 )
					sTextOutputData.strText = strDrawBuf.Mid(s, data.iEnd-data.iStart+1);
				else
					sTextOutputData.strText = strDrawBuf.Left(data.iEnd-iSTART+1);
			}

			GetTextExtentPoint(sTextOutputData.strText, sTextOutputData.szSize);

			CRect rt(
				sTextOutputData.ptPoint.x,
				sTextOutputData.ptPoint.y,
				sTextOutputData.ptPoint.x+sTextOutputData.szSize.cx,
				sTextOutputData.ptPoint.y+sTextOutputData.szSize.cy);
			ComponentToScreen( &rt );
			ResetTextSettingLine( data, rt, sTextOutputData);

			m_TextOutputDataSet.insert(sTextOutputData);

			++itr;
		}

		m_bNeedUpdateTextSetting = FALSE;
	}
	m_bNeedUpdateComposition = FALSE;

	TextOutputDataSet::iterator titr,tend;
	titr = m_TextOutputDataSet.begin();
	tend = m_TextOutputDataSet.end();
	
	for( ; titr!=tend; ++titr )
		TextOut(*titr);

	return S_OK;
}

void TEdit::ClearText()
{
	m_strDisplayText.Empty();
	m_strComposition.Empty();
	m_strText.Empty();
	m_nCaret = 0;
}

void TEdit::SetText(const CString& txt)
{
	ClearText();

	m_strText = txt;
	MoveCaretToBack();
}

void TEdit::MoveCaretToFront()
{
	m_nCaret = 0;
}

void TEdit::MoveCaretToBack()
{
	m_nCaret = 0;
	CHAR bCODE = m_strText.GetAt(m_nCaret);

	while(bCODE)
	{
		if(MBCS_LEAD_TEST(bCODE))
			m_nCaret += 2;
		else
			m_nCaret++;

		bCODE = m_strText.GetAt(m_nCaret);
	}
}

void TEdit::SetPassword(BOOL bPassword)
{
	m_style |= TES_PASSWORD;
}

void TEdit::SetLock( BOOL blLock )
{
	m_blIsLocked = blLock;
}

const CString& TEdit::GetCompositionStr() const
{
	return m_strComposition;
}

void TEdit::LeftCaret(BOOL bDouble)
{
	m_nCaret -= (bDouble)? 2:1;

	if(m_nCaret < 0)
		m_nCaret = 0;
}

void TEdit::RightCaret(BOOL bDouble)
{
	m_nCaret += (bDouble)? 2: 1;

	if(m_nCaret > m_strText.GetLength())
		m_nCaret = m_strText.GetLength();
}

void TEdit::RightCaretCount(UINT uCount)
{
	m_nCaret += uCount;
	if(m_nCaret > m_strText.GetLength())
		m_nCaret = m_strText.GetLength();
}

int TEdit::HandleImeComposition(
	HWND hWnd,
	WPARAM wParam,
	LPARAM lParam,
	CString & strComp,
	CString & strResult,
	BYTE* pbAttr,
	INT* pbAttrLength,
	DWORD* pbClause,
	INT* pbClauseLength)
{
	if( m_style & TES_NOCONMODE )
		m_hKL = ::GetKeyboardLayout(0);

	if( m_blIsLocked ) return 0;

	HIMC hImc = ImmGetContext(hWnd);				

	BYTE bCompAttr[512];
	DWORD dwCompAttrLen;

	DWORD dwCompCls[512];
	DWORD dwCompClsLen;

	WCHAR szCompReadStr[512];
	DWORD dwCompReadStrLen;

	BYTE bCompReadAttr[512];
	DWORD dwCompReadAttrLen;

	DWORD dwCompReadCls[512];
	DWORD dwCompReadClsLen;

	if (lParam & GCS_COMPATTR)
	{
		dwCompAttrLen = ImmGetCompositionString(hImc,GCS_COMPATTR,bCompAttr,sizeof(bCompAttr));
		if( pbAttr && pbAttrLength )
		{
			*pbAttrLength = dwCompAttrLen;
			memcpy( pbAttr, bCompAttr, dwCompAttrLen );
		}
	}
	else
	{
		dwCompAttrLen = 0;
		bCompAttr[0] = 0;

		if( pbAttr && pbAttrLength )
		{
			pbAttr[0] = '\0';
			*pbAttrLength = 0;
		}
	}

	if (lParam & GCS_COMPCLAUSE)
	{
		dwCompClsLen = ImmGetCompositionString(hImc,GCS_COMPCLAUSE,dwCompCls,sizeof(dwCompCls));
		if( pbClause && pbClauseLength )
		{
			*pbClauseLength = dwCompClsLen / sizeof(DWORD);
			memcpy( pbClause, dwCompCls, dwCompClsLen );
		}
	}
	else
	{
		dwCompClsLen = 0;
		dwCompCls[0] = 0;
		dwCompCls[1] = 0;

		if( pbClause && pbClauseLength )
		{
			pbClause[0] = '\0';
			*pbClauseLength = 0;
		}
	}

	if(!(lParam & GCS_COMPREADSTR))
		 dwCompReadStrLen = 0;
	else
	{
		dwCompReadStrLen = ImmGetCompositionString(hImc,GCS_COMPREADSTR,szCompReadStr,sizeof(szCompReadStr));
		dwCompReadStrLen /= sizeof(WCHAR);
	}

	szCompReadStr[dwCompReadStrLen] = '\0';

	if (lParam & GCS_COMPREADATTR)
		dwCompReadAttrLen = ImmGetCompositionString(hImc,GCS_COMPREADATTR,bCompReadAttr,sizeof(bCompReadAttr));

	else
	{
		dwCompReadAttrLen = 0;
		bCompReadAttr[0] = 0;
	}

	if (lParam & GCS_COMPREADCLAUSE)
		dwCompReadClsLen = ImmGetCompositionString(hImc,GCS_COMPREADCLAUSE,dwCompReadCls,sizeof(dwCompReadCls));
	else
	{
		dwCompReadClsLen = 0;
		dwCompReadCls[0] = 0;
		dwCompReadCls[1] = 0;
	}

	CHAR szCompStr[1024];
	CHAR szCompResultStr[1024];

	strComp.Empty();
	if( (lParam & GCS_COMPSTR) == GCS_COMPSTR )
	{
		INT nCompSize = ImmGetCompositionString( hImc, GCS_COMPSTR, NULL, 0 );
		ImmGetCompositionString( hImc, GCS_COMPSTR, szCompStr, nCompSize );
		szCompStr[nCompSize] = '\0';

		for( INT i=0 ; i < nCompSize ; ++i )
			strComp.AppendChar( szCompStr[ i ] );
	}

	strResult.Empty();
	if( (lParam & GCS_RESULTSTR) == GCS_RESULTSTR )
	{
		if( m_pCandidate && m_pCandidateOwner == this )
		{
			ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_COMPLETE, 0);
			m_pCandidate->ShowComponent(FALSE);
			m_pCandidateOwner = NULL;
		}

		INT nResultSize = ImmGetCompositionString( hImc, GCS_RESULTSTR, NULL, 0 );
		ImmGetCompositionString(hImc, GCS_RESULTSTR, szCompResultStr, nResultSize );
		szCompResultStr[ nResultSize ] = '\0';

		for( INT i=0 ; i < nResultSize ; ++i )
			strResult.AppendChar( szCompResultStr[ i ] );
	}

	//TRACE(_T("COMP:%s, RESULT:%s\n"), strComp, strResult);
	ImmReleaseContext(hWnd, hImc);

	return strResult.GetLength();
}

int TEdit::HandleChar(TCHAR chCharCode, int nRepCnt, CString& strResult)
{
	if( m_style & TES_NOCONMODE )
		m_hKL = ::GetKeyboardLayout(0);
	
	if(m_blIsLocked)
		return 0;

	if(m_bProcDBCS)
	{
		m_bProcDBCS = FALSE;
		return 0;
	}

	if(MBCS_LEAD_TEST(chCharCode))
	{
		m_bProcDBCS = TRUE;
		return 0;
	}

	if( CheckCountryCharCode( chCharCode ) )
	{
		int ptrlen = 0;

		strResult.Empty();
		for( int i = 0; i < nRepCnt; i++)
			strResult = strResult + chCharCode;
	}

	return strResult.GetLength();
}

void TEdit::OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if( m_style & TES_NOCONMODE )
		m_hKL = ::GetKeyboardLayout(0);
	
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
	}

	m_bNeedUpdateComposition = TRUE;

	INT nCurPos = 0;
	for( INT i=1 ; i < m_nCompClauseLength ; ++i )
	{
		BYTE bAttr = m_pCompAttr[ m_pCompClause[i-1] ];
		INT nCharLength = m_pCompClause[i] - m_pCompClause[i-1];

		if( bAttr == ATTR_TARGET_CONVERTED || bAttr == ATTR_TARGET_NOTCONVERTED )
		{
			// nCurPos에서 nCurPos+nCharLength-1 까지 타겟문자열이다. 이것이 화면에 보이도록 nCaret을 조정해야된다.
			INT nTargetCompOffsetStart = nCurPos;
			INT nTargetCompOffsetEnd = nCurPos+nCharLength;

			if( nTargetCompOffsetStart < m_nCompCaretOffset )
				m_nCompCaretOffset = nTargetCompOffsetStart;
			else if( nTargetCompOffsetStart >= m_nCompCaretOffset )
				m_nCompCaretOffset = nTargetCompOffsetEnd;

			m_bConverting = TRUE;
			return ;
		}

		nCurPos += nCharLength;
	}

	m_bConverting = FALSE;
	m_nCompCaretOffset = m_nCaret + m_strComposition.GetLength();

	m_rtCandidateTarget.left = 0;
	m_rtCandidateTarget.top = 0;
	m_rtCandidateTarget.right = 0;
	m_rtCandidateTarget.bottom = 0;

	if( m_pCaret )
	{
		CRect rt;
		m_pCaret->GetComponentRect( &rt);
		m_pCaret->ComponentToScreen( &rt);
		m_rtCandidateTarget = rt;
	}
}

BOOL TEdit::OnInputLangChangeRequest( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	if( GetFocus() )
	{
		if( m_style & TES_NOCONMODE )
		{
			if( m_strText.IsEmpty() && m_strComposition.IsEmpty() )
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

void TEdit::OnImeNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if( GetFocus() )
	{
		if( m_pCandidate )
		{
			switch( wParam )
			{
			case IMN_OPENCANDIDATE:
			case IMN_CHANGECANDIDATE:
				{
					HIMC hIMC = ImmGetContext(hWnd);

					int size = ImmGetCandidateList(hIMC, 0, NULL, 0);
					if( size > 0 )
					{
						BYTE* pBuf = new BYTE[ size ];
						ImmGetCandidateList( hIMC, 0, (LPCANDIDATELIST) pBuf, size );

						LPCANDIDATELIST pCandiList = (LPCANDIDATELIST) pBuf;

						// 일어IME 버그. ImmGetCandidateListW함수에서 나온 것임에도 불구하고 pCandi->dwPageStart가 잘못나온다.
						DWORD dwRealStart = (pCandiList->dwSelection / pCandiList->dwPageSize) * pCandiList->dwPageSize;
						if(pCandiList->dwPageStart != dwRealStart)
						{
							ImmNotifyIME( hIMC, NI_SETCANDIDATE_PAGESTART, 0, dwRealStart );
							pCandiList->dwPageStart = dwRealStart;
						}

						std::vector<CString> vCANDIDATE;

						for( INT i=pCandiList->dwPageStart ; (i < pCandiList->dwCount) && (vCANDIDATE.size() < 9)  ; ++i )
						{
							CHAR* pCHAR = (CHAR*)(pBuf + pCandiList->dwOffset[ i ]);
							vCANDIDATE.push_back( CString(pCHAR));
						}

						m_pCandidate->SetCandidateChar( vCANDIDATE );
						m_pCandidate->SetSelection( pCandiList->dwSelection % 9 );

						delete [] pBuf;

						m_pCandidate->MoveComponentAdjustText( m_rtCandidateTarget );
						m_pCandidate->ShowComponent(TRUE);
					}

					ImmReleaseContext(hWnd,hIMC);
				}
				break;

			case IMN_CLOSECANDIDATE:
				{
					if(m_pCandidate->IsVisible())
					{
						m_pCandidate->ResetCandidateChar();
						m_pCandidate->ShowComponent(FALSE);
					}
				}
				break;
			}
		}

		switch( wParam )
		{
		case IMN_SETCONVERSIONMODE:
			{
				if( m_style & TES_PASSWORD )
				{
					TEdit::g_bPassImeNotify = TRUE;

					DWORD dwConv, dwSent, dwTemp;
					HIMC hIMC = ::ImmGetContext(g_hWnd);
					::ImmGetConversionStatus(hIMC, &dwConv, &dwSent);
					if(dwConv & IME_CMODE_NATIVE)
					{
						dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
						dwConv = dwTemp;
						::ImmSetConversionStatus( hIMC, dwConv, dwSent);
					}
					::ImmReleaseContext( hWnd, hIMC);

					TEdit::g_bPassImeNotify = FALSE;
				}
				else if( m_style & TES_NOCONMODE )
				{
					if( m_strText.IsEmpty() && m_strComposition.IsEmpty() )
					{
						// 변경허용
					}
					else
					{
						// 변경불허 - 다시 원입력상태로 되돌아감.
						TEdit::g_bPassImeNotify = TRUE;

						DWORD dwConv, dwSent, dwTemp;
						HIMC hIMC = ::ImmGetContext(g_hWnd);
						::ImmGetConversionStatus(hIMC, &dwConv, &dwSent);
						if(dwConv & IME_CMODE_NATIVE)
						{
							dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
							dwConv = dwTemp;
						}
						else
						{
							dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
							dwTemp |= IME_CMODE_NATIVE;
							dwConv = dwTemp;
						}
						::ImmSetConversionStatus( hIMC, dwConv, dwSent);
						::ImmReleaseContext( hWnd, hIMC);

						TEdit::g_bPassImeNotify = FALSE;
					}
				}
				
			}
			break;
		}
	}
}

void TEdit::ShowComponent( BOOL bVisible)
{
	TComponent::ShowComponent( bVisible);

	if(!bVisible)
		if(!m_strComposition.IsEmpty())
			OnImeComposition( g_hWnd, 0, GCS_RESULTSTR);
		else if( m_pCandidate && m_pCandidateOwner == this )
		{
			m_pCandidate->ShowComponent(FALSE);
			m_pCandidateOwner = NULL;
		}
}

void TEdit::OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags)
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
	case VK_END		: m_nCaret = m_strText.GetLength(); break;
	case VK_HOME	: m_nCaret = 0; break;
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

void TEdit::OnChar( UINT nChar, int nRepCnt, UINT nFlags)
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
	}

	TComponent::OnChar( nChar, nRepCnt, nFlags);
}

void TEdit::OnSetFocus(TComponent *pLoseFocus)
{
	if(m_pCaret)
		m_pCaret->ShowComponent(GetFocus());

	m_pCandidateOwner = this;

	if( m_style & TES_PASSWORD )
	{
		DWORD dwConv, dwSent, dwTemp;
		HIMC hIMC = ::ImmGetContext(g_hWnd);
		::ImmGetConversionStatus(hIMC, &dwConv, &dwSent);
		if(dwConv & IME_CMODE_NATIVE)
		{
			dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
			dwConv = dwTemp;
			::ImmSetConversionStatus( hIMC, dwConv, dwSent);
		}
		::ImmReleaseContext( g_hWnd, hIMC);
	}
}

void TEdit::OnLoseFocus(TComponent *pSetFocus)
{
	if(m_pCaret)
		m_pCaret->ShowComponent(GetFocus());

	if(!m_strComposition.IsEmpty())
		OnImeComposition( g_hWnd, 0, GCS_RESULTSTR);
	else if( m_pCandidate && m_pCandidateOwner == this )
	{
		m_pCandidate->ShowComponent(FALSE);
		m_pCandidateOwner = NULL;
	}
}

void TEdit::MoveTextSetting(INT iFrom, INT iCount)
{
	TextSettingSet::iterator itr, end;
	itr = GetTextSettingItr(iFrom);
	end = m_TextSettingSet.end();

	for( ; itr!=end; ++itr )
	{
		itr->iStart += iCount;
		itr->iEnd += iCount;
	}
}

INT GetAttrToType( BYTE bType )
{
	switch( bType )
	{
		case ATTR_INPUT: // 점선
			return TTEXTSETTINGTYPE_LINE_DOT;

		case ATTR_TARGET_CONVERTED: // 두꺼운 실선
		case ATTR_TARGET_NOTCONVERTED:
			return TTEXTSETTINGTYPE_LINE_THICK;

		case ATTR_CONVERTED: // 실선
		case ATTR_INPUT_ERROR:
		case ATTR_FIXEDCONVERTED:
			return TTEXTSETTINGTYPE_LINE;

		default:
			return TTEXTSETTINGTYPE_NONE;
	}
}

void TEdit::ResetCompTextSetting(
	INT nCompStart,
	INT nCompEnd )
{
	RemoveCompTextSetting();

	if( nCompStart < nCompEnd )
	{
		INT nCurPos = nCompStart;

		for( INT i=1 ; i < m_nCompClauseLength ; ++i )
		{
			BYTE bAttr = m_pCompAttr[ m_pCompClause[i-1] ];
			INT nCharLength = m_pCompClause[i] - m_pCompClause[i-1];

			// 문자열의 szComp+dwPrevPos에서 nCharLength만큼 해당이다.

			AddTextSetting(
				nCurPos,
				nCurPos+nCharLength-1,
				m_pFont->m_dwColor,
				NULL,
				TTEXTSETTINGTYPE_COMP | GetAttrToType( bAttr ) );

			nCurPos += nCharLength;
		}
	}
}

void TEdit::ResetTextSettingLine(
								 const TextSetting& data,
								 const CRect& rectScreenText,
								 TextOutputData& sTextOutputData)
{
	// 밑줄 그리기
	if( (data.iType&TTEXTSETTINGTYPE_LINE) == TTEXTSETTINGTYPE_LINE ||
		(data.iType&TTEXTSETTINGTYPE_LINE_THICK) == TTEXTSETTINGTYPE_LINE_THICK ||
		(data.iType&TTEXTSETTINGTYPE_LINE_DOT) == TTEXTSETTINGTYPE_LINE_DOT )
	{
		DWORD dwColor = m_pFont->m_dwColor;

		if( sTextOutputData.pTextLine )
			delete sTextOutputData.pTextLine;
		sTextOutputData.pTextLine = NULL;

		if((data.iType&TTEXTSETTINGTYPE_LINE_THICK) == TTEXTSETTINGTYPE_LINE_THICK)
		{
			dwColor = D3DCOLOR_XRGB(255,255,128);

			m_rtCandidateTarget = rectScreenText;
			m_rtCandidateTarget.bottom += 3;
		}

		sTextOutputData.pTextLine = new TComponent::TextLine;

		INT nStartX = rectScreenText.left+2;
		INT nStartY = rectScreenText.bottom+2;
		INT nEndX = rectScreenText.right;
		INT nEndY = rectScreenText.bottom+2;

		if( (data.iType&TTEXTSETTINGTYPE_LINE_DOT) == TTEXTSETTINGTYPE_LINE_DOT )
		{
			sTextOutputData.pTextLine->MakeDotLine(
				CTachyonRes::m_pDEVICE->m_pDevice,
				nStartX, nStartY, nEndX, nEndY,
				dwColor,
				4, 2);
		}
		else
		{
			sTextOutputData.pTextLine->MakeLine(
				CTachyonRes::m_pDEVICE->m_pDevice,
				nStartX, nStartY, nEndX, nEndY,
				dwColor);
		}
	}
	else
		sTextOutputData.pTextLine = NULL;
}

void TEdit::RemoveCompTextSetting()
{
	TextSettingSet::iterator itr;
	itr = m_TextSettingSet.begin();

	while( itr != m_TextSettingSet.end() )
	{
		if( (itr->iType&TTEXTSETTINGTYPE_COMP) == TTEXTSETTINGTYPE_COMP)
		{
			itr = m_TextSettingSet.erase( itr );
			continue;
		}

		++itr;
	}
}

BOOL TEdit::CheckCountryCharCode(  TCHAR chCharCode  )
{
	if ( m_nCountryFlag == COUNTRY_FLAG_FRANCE )
	{
		if( (chCharCode == ' ') ||
			(chCharCode >=33 && chCharCode <=126) ||
			(chCharCode == -4) ||
			(chCharCode == -10) ||
			(chCharCode == -28) ||
			(chCharCode == -36) ||
			(chCharCode == -42) ||
			(chCharCode == -60) ||
			(chCharCode == -33) ||
			(chCharCode == -97) ||
			(chCharCode == -32) ||
			(chCharCode == -63) ||
			(chCharCode == -31) ||
			(chCharCode == -62) ||
			(chCharCode == -30) ||
			(chCharCode == -58) ||
			(chCharCode == -26) ||
			(chCharCode == -57) ||
			(chCharCode == -25) ||
			(chCharCode == -56) ||
			(chCharCode == -24) ||
			(chCharCode == -55) ||
			(chCharCode == -23) ||
			(chCharCode == -54) ||
			(chCharCode == -22) ||
			(chCharCode == -53) ||
			(chCharCode == -21) ||
			(chCharCode == -50) ||
			(chCharCode == -18) ||
			(chCharCode == -49) ||
			(chCharCode == -17) ||
			(chCharCode == -44) ||
			(chCharCode == -12) ||
			(chCharCode == -116) ||
			(chCharCode == -100) ||
			(chCharCode == -39) ||
			(chCharCode == -7) ||
			(chCharCode == -37) ||
			(chCharCode == -5) ||
			(chCharCode == -97) ||
			(chCharCode == -1) ||
			(chCharCode == -88) ||
			(chCharCode == -76) )
		{
				return TRUE;
		}

		return FALSE;
	}
	else if ( m_nCountryFlag == COUNTRY_FLAG_POLAND )
	{
		if( (chCharCode == ' ') ||
			(chCharCode >=33 && chCharCode <=126) ||
			(chCharCode == -4) ||
			(chCharCode == -10) ||
			(chCharCode == -28) ||
			(chCharCode == -36) ||
			(chCharCode == -42) ||
			(chCharCode == -60) ||
			(chCharCode == -33) ||
			(chCharCode == -91) ||
			(chCharCode == -71) ||
			(chCharCode == -58) ||
			(chCharCode == -26) ||
			(chCharCode == -54) ||
			(chCharCode == -22) ||
			(chCharCode == -93) ||
			(chCharCode == -77) ||
			(chCharCode == -47) ||
			(chCharCode == -15) ||
			(chCharCode == -45) ||
			(chCharCode == -13) ||
			(chCharCode == -116) ||
			(chCharCode == -100) ||
			(chCharCode == -97) ||
			(chCharCode == -81) ||
			(chCharCode == -65) ||
			(chCharCode == -88) ||
			(chCharCode == -76) )
		{
			return TRUE;
		}

		return FALSE;
	}
	else if ( m_nCountryFlag == COUNTRY_FLAG_CZECH )
	{
		if( (chCharCode == ' ') ||
			(chCharCode >=33 && chCharCode <=126) ||
			(chCharCode == -4) ||
			(chCharCode == -10) ||
			(chCharCode == -28) ||
			(chCharCode == -36) ||
			(chCharCode == -42) ||
			(chCharCode == -60) ||
			(chCharCode == -33) ||
			(chCharCode == -63) ||
			(chCharCode == -31) ||
			(chCharCode == -56) ||
			(chCharCode == -24) ||
			(chCharCode == -49) ||
			(chCharCode == -17) ||
			(chCharCode == -55) ||
			(chCharCode == -23) ||
			(chCharCode == -52) ||
			(chCharCode == -20) ||
			(chCharCode == -51) ||
			(chCharCode == -19) ||
			(chCharCode == -46) ||
			(chCharCode == -14) ||
			(chCharCode == -45) ||
			(chCharCode == -40) ||
			(chCharCode == -8) ||
			(chCharCode == -118) ||
			(chCharCode == -102) ||
			(chCharCode == -115) ||
			(chCharCode == -99) ||
			(chCharCode == -38) ||
			(chCharCode == -6) ||
			(chCharCode == -39) ||
			(chCharCode == -7) ||
			(chCharCode == -35) ||
			(chCharCode == -3) ||
			(chCharCode == -114) ||
			(chCharCode == -98) ||
			(chCharCode == -88) ||
			(chCharCode == -76) )
		{
			return TRUE;
		}

		return FALSE;
	}
	else if ( m_nCountryFlag == COUNTRY_FLAG_ITALY )
	{
		if( (chCharCode == ' ') ||
			(chCharCode >=33 && chCharCode <=126) ||
			(chCharCode == -4) ||
			(chCharCode == -10) ||
			(chCharCode == -28) ||
			(chCharCode == -36) ||
			(chCharCode == -42) ||
			(chCharCode == -60) ||
			(chCharCode == -33) ||
			(chCharCode == -64) ||
			(chCharCode == -32) ||
			(chCharCode == -56) ||
			(chCharCode == -24) ||
			(chCharCode == -55) ||
			(chCharCode == -23) ||
			(chCharCode == -52) ||
			(chCharCode == -20) ||
			(chCharCode == -51) ||
			(chCharCode == -19) ||
			(chCharCode == -49) ||
			(chCharCode == -17) ||
			(chCharCode == -46) ||
			(chCharCode == -14) ||
			(chCharCode == -13) ||
			(chCharCode == -39) ||
			(chCharCode == -7) ||
			(chCharCode == -38) ||
			(chCharCode == -6) ||
			(chCharCode == -88) ||
			(chCharCode == -76) )
		{
			return TRUE;
		}

		return FALSE;
	}
	else if ( m_nCountryFlag == COUNTRY_FLAG_SPAIN )
	{
		if( (chCharCode == ' ') ||
			(chCharCode >=33 && chCharCode <=126) ||
			(chCharCode == -4) ||
			(chCharCode == -10) ||
			(chCharCode == -28) ||
			(chCharCode == -36) ||
			(chCharCode == -42) ||
			(chCharCode == -60) ||
			(chCharCode == -33) ||
			(chCharCode == -63) ||
			(chCharCode == -31) ||
			(chCharCode == -25) ||
			(chCharCode == -55) ||
			(chCharCode == -23) ||
			(chCharCode == -51) ||
			(chCharCode == -19) ||
			(chCharCode == -47) ||
			(chCharCode == -15) ||
			(chCharCode == -45) ||
			(chCharCode == -13) ||
			(chCharCode == -47) ||
			(chCharCode == -38) ||
			(chCharCode == -6) ||
			(chCharCode == -4) ||
			(chCharCode == -86) ||
			(chCharCode == -70) ||
			(chCharCode == -95) ||
			(chCharCode == -65) ||
			(chCharCode == -76) )
		{
			return TRUE;
		}

		return FALSE;
	}
	else if ( m_nCountryFlag == COUNTRY_FLAG_GREECE )
	{
		if( (chCharCode == ' ') ||
			(chCharCode >=33 && chCharCode <=126) ||
			(chCharCode == -4) ||
			(chCharCode == -10) ||
			(chCharCode == -28) ||
			(chCharCode == -36) ||
			(chCharCode == -42) ||
			(chCharCode == -60) ||
			(chCharCode == -33)  || 
			(chCharCode == -63) || 
			(chCharCode == -62) || 
			(chCharCode == -61) || 
			(chCharCode == -60) || 
			(chCharCode == -59) || 
			(chCharCode == -58) || 
			(chCharCode == -57) || 
			(chCharCode == -56) || 
			(chCharCode == -55) || 
			(chCharCode == -54) || 
			(chCharCode == -53) || 
			(chCharCode == -52) || 
			(chCharCode == -51) || 
			(chCharCode == -50) || 
			(chCharCode == -49) || 
			(chCharCode == -48) || 
			(chCharCode == -47) || 
			(chCharCode == -45) || 
			(chCharCode == -44) || 
			(chCharCode == -43) || 
			(chCharCode == -42) || 
			(chCharCode == -41) || 
			(chCharCode == -40) || 
			(chCharCode == -40) || 
			(chCharCode == -39) || 
			(chCharCode == -31) || 
			(chCharCode == -30) || 
			(chCharCode == -29) || 
			(chCharCode == -28) || 
			(chCharCode == -27) || 
			(chCharCode == -26) || 
			(chCharCode == -25) || 
			(chCharCode == -24) || 
			(chCharCode == -23) || 
			(chCharCode == -22) || 
			(chCharCode == -21) || 
			(chCharCode == -20) || 
			(chCharCode == -19) || 
			(chCharCode == -18) || 
			(chCharCode == -17) || 
			(chCharCode == -16) || 
			(chCharCode == -15) || 
			(chCharCode == -13) || 
			(chCharCode == -12) || 
			(chCharCode == -11) || 
			(chCharCode == -10) || 
			(chCharCode == -9) || 
			(chCharCode == -8) || 
			(chCharCode == -7) 	)
		{
			return TRUE;
		}

		return FALSE;
	}
	else if ( m_nCountryFlag == COUNTRY_FLAG_RUSSIA )
	{
		if( (chCharCode == ' ') ||
			(chCharCode >=33 && chCharCode <=126) ||
			(chCharCode == -4) ||
			(chCharCode == -10) ||
			(chCharCode == -28) ||
			(chCharCode == -36) ||
			(chCharCode == -42) ||
			(chCharCode == -60) ||
			(chCharCode == -64) ||
			(chCharCode == -63) ||
			(chCharCode == -62) ||
			(chCharCode == -61) ||
			(chCharCode == -60) ||
			(chCharCode == -59) ||
			(chCharCode == -88) ||
			(chCharCode == -58) ||
			(chCharCode == -57) ||
			(chCharCode == -56) ||
			(chCharCode == -55) ||
			(chCharCode == -54) ||
			(chCharCode == -53) ||
			(chCharCode == -52) ||
			(chCharCode == -51) ||
			(chCharCode == -50) ||
			(chCharCode == -49) ||
			(chCharCode == -48) ||
			(chCharCode == -47) ||
			(chCharCode == -46) ||
			(chCharCode == -45) ||
			(chCharCode == -44) ||
			(chCharCode == -43) ||
			(chCharCode == -42) ||
			(chCharCode == -41) ||
			(chCharCode == -40) ||
			(chCharCode == -39) ||
			(chCharCode == -38) ||
			(chCharCode == -37) ||
			(chCharCode == -36) ||
			(chCharCode == -35) ||
			(chCharCode == -34) ||
			(chCharCode == -33) ||
			(chCharCode == -32) ||
			(chCharCode == -31) ||
			(chCharCode == -30) ||
			(chCharCode == -29) ||
			(chCharCode == -28) ||
			(chCharCode == -27) ||
			(chCharCode == -72) ||
			(chCharCode == -25) ||
			(chCharCode == -24) ||
			(chCharCode == -26) ||
			(chCharCode == -23) ||
			(chCharCode == -22) ||
			(chCharCode == -21) ||
			(chCharCode == -20) ||
			(chCharCode == -19) ||
			(chCharCode == -18) ||
			(chCharCode == -17) ||
			(chCharCode == -16) ||
			(chCharCode == -15) ||
			(chCharCode == -14) ||
			(chCharCode == -13) ||
			(chCharCode == -12) ||
			(chCharCode == -11) ||
			(chCharCode == -10) ||
			(chCharCode == -9) ||
			(chCharCode == -8) ||
			(chCharCode == -7) ||
			(chCharCode == -6) ||
			(chCharCode == -5) ||
			(chCharCode == -4) ||
			(chCharCode == -3) ||
			(chCharCode == -2) ||
			(chCharCode == -1) )
		{
			return TRUE;
		}

		return FALSE;
	}
	else if ( m_nCountryFlag == COUNTRY_FLAG_ROMANIA )
	{
		if( (chCharCode == ' ') ||
			(chCharCode >=33 && chCharCode <=126) ||
			(chCharCode == -4) ||
			(chCharCode == -10) ||
			(chCharCode == -28) ||
			(chCharCode == -36) ||
			(chCharCode == -42) ||
			(chCharCode == -60) ||
			(chCharCode == -33) ||
			(chCharCode == -29) ||
			(chCharCode == -61) ||
			(chCharCode == -30) ||
			(chCharCode == -62) ||
			(chCharCode == -18) ||
			(chCharCode == -50) ||
			(chCharCode == -70) ||
			(chCharCode == -86) ||
			(chCharCode == -2) ||
			(chCharCode == -34) )
		{
			return TRUE;
		}

		return FALSE;
	}
	else if ( m_nCountryFlag == COUNTRY_FLAG_HUNGARY )
	{
		if( (chCharCode == ' ') ||
			(chCharCode >=33 && chCharCode <=126) ||
			(chCharCode == -4) ||
			(chCharCode == -10) ||
			(chCharCode == -28) ||
			(chCharCode == -36) ||
			(chCharCode == -42) ||
			(chCharCode == -60) ||
			(chCharCode == -33) ||
			(chCharCode == -63) ||
			(chCharCode == -55) ||
			(chCharCode == -51) ||
			(chCharCode == -45) ||
			(chCharCode == -42) ||
			(chCharCode == -43) ||
			(chCharCode == -38) ||
			(chCharCode == -36) ||
			(chCharCode == -37) ||
			(chCharCode == -31) ||
			(chCharCode == -23) ||
			(chCharCode == -19) ||
			(chCharCode == -13) ||
			(chCharCode == -10) ||
			(chCharCode == -11) ||
			(chCharCode == -6) ||
			(chCharCode == -4) ||
			(chCharCode == -5) )

		{
			return TRUE;
		}

		return FALSE;
	}
	else if ( m_nCountryFlag == COUNTRY_FLAG_TURKEY )
	{
		if( (chCharCode == ' ') ||
			(chCharCode >=33 && chCharCode <=126) ||
			(chCharCode == -4) ||
			(chCharCode == -10) ||
			(chCharCode == -28) ||
			(chCharCode == -36) ||
			(chCharCode == -42) ||
			(chCharCode == -60) ||
			(chCharCode == -57) ||
			(chCharCode == -48) ||
			(chCharCode == 73) ||
			(chCharCode == -35) ||
			(chCharCode == -34) ||
			(chCharCode == 32) ||
			(chCharCode == -16) ||
			(chCharCode == -3) ||
			(chCharCode == 105) ||
			(chCharCode == -2) ||
			(chCharCode == -33) )
		{
			return TRUE;
		}

		return FALSE;
	}
	else if ( m_nCountryFlag == COUNTRY_FLAG_PORTUGAL )
	{
		if( (chCharCode == ' ') ||
			(chCharCode >=33 && chCharCode <=126) ||
			(chCharCode == -4) ||
			(chCharCode == -10) ||
			(chCharCode == -28) ||
			(chCharCode == -36) ||
			(chCharCode == -42) ||
			(chCharCode == -60) ||
			(chCharCode == -33) ||
			(chCharCode == -64) ||
			(chCharCode == -32) ||
			(chCharCode == -61) ||
			(chCharCode == -29) ||
			(chCharCode == -31) ||
			(chCharCode == -57) ||
			(chCharCode == -25) ||
			(chCharCode == -54) ||
			(chCharCode == -22) ||
			(chCharCode == -55) ||
			(chCharCode == -23) ||
			(chCharCode == -19) ||
			(chCharCode == -43) ||
			(chCharCode == -11) ||
			(chCharCode == -13) ||
			(chCharCode == -6) )
		{
			return TRUE;
		}

		return FALSE;
	}
	else	// 일본, 한국, 독일
	{
		if( (chCharCode == ' ') ||
			(chCharCode >=33 && chCharCode <=126) ||
			(chCharCode == -4) ||
			(chCharCode == -10) ||
			(chCharCode == -28) ||
			(chCharCode == -36) ||
			(chCharCode == -42) ||
			(chCharCode == -60) ||
			(chCharCode == -33) )
		{
			return TRUE;
		}
		
		return FALSE;
	}

	return FALSE;
}
