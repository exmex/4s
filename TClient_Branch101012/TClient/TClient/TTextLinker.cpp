#include "Stdafx.h"
#include "TTextLinker.h"

// ===============================================================================
const CString	CTTextLinker::LINK_DEF_TOK(_T("\n\r"));

const CHAR		CTTextLinker::LINK_STR_START		= '[';
const CHAR		CTTextLinker::LINK_STR_END			= ']';
const DWORD		CTTextLinker::LINK_DATA_POOL_SIZE	= 100;
const DWORD		CTTextLinker::LINK_QUEST_COLOR		= 0xFFFFFF7F;
// ===============================================================================

// ===============================================================================
CTTextLinker::CTTextLinker()
{
	m_nTextLinkDataPos = 0;
	m_pTextLinkDataPool = new CTTextLinkData[LINK_DATA_POOL_SIZE];
}
// -------------------------------------------------------------------------------
CTTextLinker::~CTTextLinker()
{
	delete [] m_pTextLinkDataPool;
}
// -------------------------------------------------------------------------------
CTTextLinker* CTTextLinker::GetInstance()
{
	static CTTextLinker instance;
	return &instance;
}
// ===============================================================================

// ===============================================================================
DWORD CTTextLinker::NewTextLinkData(CTTextLinkData** pOut)
{
	*pOut = &(m_pTextLinkDataPool[ m_nTextLinkDataPos ]);
	
	(*pOut)->FromStr("");
	(*pOut)->ResetPosition();

	DWORD nRetID = m_nTextLinkDataPos;
	if( m_nTextLinkDataPos >= LINK_DATA_POOL_SIZE-1 )
		m_nTextLinkDataPos = 0;
	else
		++m_nTextLinkDataPos;

	return nRetID;
}
// -------------------------------------------------------------------------------
CTTextLinkData* CTTextLinker::GetTextLinkData(DWORD nPos) const
{
	return &(m_pTextLinkDataPool[nPos]);
}
// ===============================================================================
void CTTextLinker::InsertLinkTextToEdit(TEdit* pEdit, LPTQUEST pQuest)
{
	CTTextLinkData* pData;
	DWORD dwID = NewTextLinkData(&pData);
	pData->PackQuest(pQuest);

	CString strText = CString(LINK_STR_START)
		+ pQuest->m_strTITLE + CString(LINK_STR_END);
	
	pEdit->InsertString(strText, LINK_QUEST_COLOR, dwID);
}
// -------------------------------------------------------------------------------
void CTTextLinker::InsertLinkTextToEdit(TEdit* pEdit, LPTITEM pItem, CTClientItem* pClientItem)
{
	CTTextLinkData* pData;
	DWORD dwID = NewTextLinkData(&pData);
	pData->PackItem(pItem, pClientItem);

	CString strText = CString(LINK_STR_START)
		+ pItem->m_strNAME + CString(LINK_STR_END);

	DWORD dwColor;
	if( pClientItem )
		dwColor = pClientItem->GetQualityColor();
	else
		dwColor = CTClientItem::m_dwNORMALCOLOR;

	pEdit->InsertString(strText, dwColor, dwID);
}
// ===============================================================================
CString CTTextLinker::MakeItemNetText( const CString& strFormat, const VECTORDWORD& vItemID)
{
	WORD wSIZE = TCHAT_DWORD_SIZE;
	WORD wPOS = 0;

	CString strHEAD(_T(""));
	CString strBODY(_T(""));

	for( int i=0; i<INT(vItemID.size()); i++)
	{
		int nPOS = strFormat.Find( _T("%s"), wPOS);

		if( nPOS < wPOS )
			break;

		CTTextLinkData *pLINK = GetTextLinkData(vItemID[i]);
		WORD wLEN = nPOS - wPOS;

		if(wLEN)
		{
			strBODY += strFormat.Mid( wPOS, wLEN);
			wSIZE += wLEN;

			if( wSIZE > TCHAT_CHAR_MAX )
			{
				wLEN = strBODY.GetLength();
				wSIZE -= TCHAT_CHAR_MAX;

				if( wSIZE > wLEN )
					return BuildNetString( CString(_T("")), CString(_T("")));

				return BuildNetString( strHEAD, strBODY.Left(wLEN - wSIZE));
			}
		}

		if(pLINK)
		{
			const CString& strLINK = pLINK->ToStr();
			WORD vLINK[2] = {
				strLINK.GetLength(),
				strBODY.GetLength()};

			if(vLINK[0])
			{
				CString strSIZE;

				if( wSIZE + vLINK[0] + TCHAT_DWORD_SIZE > TCHAT_CHAR_MAX )
					return BuildNetString( strHEAD, strBODY);

				strSIZE.Format( _T("%04X%04X"), vLINK[0], vLINK[1]);
				strHEAD += strSIZE + strLINK;
				wSIZE += vLINK[0] + TCHAT_DWORD_SIZE;
			}
		}

		wPOS = nPOS + 2;
	}

	WORD wLEN = strFormat.GetLength();
	if( wPOS < wLEN )
	{
		strBODY += strFormat.Mid(wPOS);
		wSIZE += wLEN - wPOS;

		if( wSIZE > TCHAT_CHAR_MAX )
		{
			wLEN = strBODY.GetLength();
			wSIZE -= TCHAT_CHAR_MAX;

			if( wSIZE > wLEN )
				return BuildNetString( CString(_T("")), CString(_T("")));

			strBODY = strBODY.Left(wLEN - wSIZE);
		}
	}

	return BuildNetString( strHEAD, strBODY);
}

// ===============================================================================
CString CTTextLinker::BuildNetString( const CString& strHeader, const CString& strBody)
{
	CString strSIZE;

	strSIZE.Format( _T("%04X%04X"), strHeader.GetLength(), strBody.GetLength());
	return strSIZE + strHeader + strBody;
}

CString CTTextLinker::MakeLinkToNetText( TEdit *pEdit, BOOL bLengthSafety, INT nMaxLength)
{
	CString strHeader(_T(""));
	CString strBody(_T(""));

	WORD wSIZE = TCHAT_DWORD_SIZE;
	WORD wPOS = 0;

	for( TComponent::TextSettingSet::iterator it = pEdit->GetTextSettingItrBegin(); it != pEdit->GetTextSettingItrEnd(); it++)
	{
		TComponent::TextSetting& vDATA = (*it);

		if( wPOS <= vDATA.iStart && wPOS <= vDATA.iEnd )
		{
			CTTextLinkData *pLINK = GetTextLinkData(vDATA.dwUser);
			WORD wLEN = vDATA.iStart - wPOS;

			if(wLEN)
			{
				strBody += pEdit->m_strText.Mid( wPOS, wLEN);
				wSIZE += wLEN;

				if( bLengthSafety && wSIZE > nMaxLength )
				{
					wLEN = strBody.GetLength();
					wSIZE -= nMaxLength;

					if( wSIZE > wLEN )
						return BuildNetString( CString(_T("")), CString(_T("")));

					return BuildNetString( strHeader, strBody.Left(wLEN - wSIZE));
				}
			}

			if(pLINK)
			{
				const CString& strLINK = pLINK->ToStr();
				WORD vLINK[2] = {
					strLINK.GetLength(),
					strBody.GetLength()};

				if(vLINK[0])
				{
					CString strSIZE;

					if( bLengthSafety && wSIZE + vLINK[0] + TCHAT_DWORD_SIZE > nMaxLength )
						return BuildNetString( strHeader, strBody);

					strSIZE.Format( _T("%04X%04X"), vLINK[0], vLINK[1]);
					strHeader += strSIZE + strLINK;
					wSIZE += vLINK[0] + TCHAT_DWORD_SIZE;
				}
			}

			wPOS = vDATA.iEnd + 1;
		}
	}

	WORD wLEN = pEdit->m_strText.GetLength();
	if( wPOS < wLEN )
	{
		strBody += pEdit->m_strText.Mid(wPOS);
		wSIZE += wLEN - wPOS;

		if( bLengthSafety && wSIZE > nMaxLength )
		{
			wLEN = strBody.GetLength();
			wSIZE -= nMaxLength;

			if( wSIZE > wLEN )
				return BuildNetString( CString(_T("")), CString(_T("")));

			strBody = strBody.Left(wLEN - wSIZE);
		}
	}

	return BuildNetString( strHeader, strBody);
}

// -------------------------------------------------------------------------------
CString CTTextLinker::AppendNetString( const CString& strMSG1, const CString& strMSG2)
{
	WORD wLENGTH = strMSG1.GetLength();

	if( wLENGTH < TCHAT_DWORD_SIZE )
		return BuildNetString( CString(_T("")), CString(_T("")));

	DWORD dwCODE = 0;
	WORD wHEAD = 0;
	WORD wBODY = 0;
	WORD wBASE = 0;

	sscanf( LPCSTR(strMSG1.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
	wHEAD = HIWORD(dwCODE);
	wBODY = LOWORD(dwCODE);

	if( wHEAD + wBODY + TCHAT_DWORD_SIZE != wLENGTH )
		return BuildNetString( CString(_T("")), CString(_T("")));

	CString strBODY = strMSG1.Mid( TCHAT_DWORD_SIZE + wHEAD, wBODY);
	CString strHEAD = strMSG1.Mid( TCHAT_DWORD_SIZE, wHEAD);
	CString strDATA(_T(""));

	wLENGTH = strMSG2.GetLength();
	wBASE = wBODY;

	if( wLENGTH < TCHAT_DWORD_SIZE )
		return strMSG1;

	sscanf( LPCSTR(strMSG2.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
	wHEAD = HIWORD(dwCODE);
	wBODY = LOWORD(dwCODE);

	if( wHEAD + wBODY + TCHAT_DWORD_SIZE != wLENGTH )
		return strMSG1;

	strBODY += strMSG2.Mid( TCHAT_DWORD_SIZE + wHEAD, wBODY);
	strDATA = strMSG2.Mid( TCHAT_DWORD_SIZE, wHEAD);

	wLENGTH = strDATA.GetLength();
	wBODY = strBODY.GetLength();

	while( wLENGTH >= TCHAT_DWORD_SIZE )
	{
		CString strSIZE;

		sscanf( LPCSTR(strDATA.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
		LPWORD pPOS = LPWORD(&dwCODE);

		if( wLENGTH < TCHAT_DWORD_SIZE + pPOS[1] ||
			wBODY < wBASE + pPOS[0] )
			break;

		pPOS[0] += wBASE;
		strSIZE.Format( _T("%08X"), dwCODE);

		strHEAD += strSIZE + strDATA.Mid( TCHAT_DWORD_SIZE, pPOS[1]);
		strDATA = strDATA.Mid(TCHAT_DWORD_SIZE + pPOS[1]);
		wLENGTH = strDATA.GetLength();
	}

	return BuildNetString( strHEAD, strBODY);
}

CString CTTextLinker::MakeNetToSimpleLinkText( TComponent* pTxtSettingComp, const CString& strNetMsg)
{
	CString strRESULT = BuildNetString( CString(_T("")), CString(_T("")));
	INT nPOS = 0;

	CString strLINE = SplitTextByCrLf( strNetMsg, LINK_DEF_TOK, nPOS);
	while(!strLINE.IsEmpty())
	{
		strRESULT = AppendNetString( strRESULT, strLINE);
		strLINE = SplitTextByCrLf( strNetMsg, LINK_DEF_TOK, nPOS);
	}

	return MakeNetToLinkText( pTxtSettingComp, strRESULT);
}

CString CTTextLinker::MakeNetToLinkText( TComponent* pTxtSettingComp, const CString& strNetMsg)
{
	WORD wLENGTH = strNetMsg.GetLength();

	if( wLENGTH < TCHAT_DWORD_SIZE )
		return CString(_T(""));

	DWORD dwCODE = 0;
	WORD wHEAD = 0;
	WORD wBODY = 0;
	WORD wBASE = 0;

	sscanf( LPCSTR(strNetMsg.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
	wHEAD = HIWORD(dwCODE);
	wBODY = LOWORD(dwCODE);

	if( wHEAD + wBODY + TCHAT_DWORD_SIZE != wLENGTH )
		return CString(_T(""));

	CString strBODY = strNetMsg.Mid( TCHAT_DWORD_SIZE + wHEAD, wBODY);
	CString strHEAD = strNetMsg.Mid( TCHAT_DWORD_SIZE, wHEAD);
	wLENGTH = strHEAD.GetLength();

	while( wLENGTH >= TCHAT_DWORD_SIZE )
	{
		CTTextLinkData *pLINK;

		CString strNAME(_T(""));
		DWORD dwCOLOR;

		sscanf( LPCSTR(strHEAD.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
		wBODY = LOWORD(dwCODE) + wBASE;
		wHEAD = HIWORD(dwCODE);

		if( TCHAT_DWORD_SIZE + wHEAD > wLENGTH ||
			wBODY > strBODY.GetLength() )
			return strBODY;
		DWORD dwID = NewTextLinkData(&pLINK);

		pLINK->FromStr(strHEAD.Mid( TCHAT_DWORD_SIZE, wHEAD));
		switch(pLINK->GetType())
		{
		case TTEXT_LINK_TYPE_QUEST	:
			{
				LPTQUEST pQuest = pLINK->UnPackQuest();

				if(pQuest)
				{
					strNAME = CString(LINK_STR_START) + pQuest->m_strTITLE + CString(LINK_STR_END);
					dwCOLOR = CTTextLinker::LINK_QUEST_COLOR;
				}
			}

			break;

		case TTEXT_LINK_TYPE_ITEM	:
			{
				CTClientItem *pItem = pLINK->UnPackItem();

				if(pItem)
				{
					strNAME = CString(LINK_STR_START) + pItem->GetTITEM()->m_strNAME + CString(LINK_STR_END);
					dwCOLOR = pItem->GetQualityColor();
				}
			}

			break;

		case TTEXT_LINK_TYPE_ITEM_OLD	:
			{
				CTClientItem *pItem = pLINK->UnPackItemOLD();

				if(pItem)
				{
					strNAME = CString(LINK_STR_START) + pItem->GetTITEM()->m_strNAME + CString(LINK_STR_END);
					dwCOLOR = pItem->GetQualityColor();
				}
			}

			break;
		}

		WORD wLEN = strNAME.GetLength();
		if(wLEN)
		{
			if( pTxtSettingComp )
				pTxtSettingComp->AddTextSetting( wBODY, wBODY + wLEN - 1, dwCOLOR, dwID);

			strBODY.Insert( wBODY, LPCSTR(strNAME));
			wBASE += wLEN;
		}

		strHEAD = strHEAD.Mid(TCHAT_DWORD_SIZE + wHEAD);
		wLENGTH = strHEAD.GetLength();
	}

	return strBODY;
}

// ===============================================================================
CString CTTextLinker::SplitTextByCrLf( const CString& strMSG, const CString& strTOK, INT& nPos, BOOL bTrimCrLf)
{
	WORD wLENGTH = strMSG.GetLength();

	if( wLENGTH < TCHAT_DWORD_SIZE )
		return CString(_T(""));

	DWORD dwCODE = 0;
	WORD wHEAD = 0;
	WORD wBODY = 0;

	sscanf( LPCSTR(strMSG.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
	wHEAD = HIWORD(dwCODE);
	wBODY = LOWORD(dwCODE);

	if( wHEAD + wBODY + TCHAT_DWORD_SIZE != wLENGTH || wBODY < nPos )
		return CString(_T(""));

	CString strBODY = strMSG.Mid( TCHAT_DWORD_SIZE + wHEAD + nPos, wBODY - nPos);
	CString strDATA = strMSG.Mid( TCHAT_DWORD_SIZE, wHEAD);
	CString strHEAD(_T(""));

	if(bTrimCrLf)
		strBODY.TrimLeft(strTOK);

	wLENGTH = strBODY.GetLength();
	if( wBODY < wLENGTH )
		return CString(_T(""));

	int nFIND = strBODY.FindOneOf(strTOK);
	int nPOS = wBODY - wLENGTH;

	if( nFIND >= 0 )
		strBODY = strBODY.Left(nFIND + 1);
	else
		nFIND = strBODY.GetLength();
	nFIND++;

	while( wHEAD >= TCHAT_DWORD_SIZE )
	{
		sscanf( LPCSTR(strDATA.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
		LPWORD pPOS = LPWORD(&dwCODE);

		if( wHEAD < TCHAT_DWORD_SIZE + pPOS[1] ||
			nPOS + nFIND <= pPOS[0] )
			break;

		if( nPos <= pPOS[0] && pPOS[0] < nPOS )
			pPOS[0] = nPOS;

		if( nPOS <= pPOS[0] )
		{
			CString strSIZE;

			pPOS[0] -= nPOS;
			strSIZE.Format( _T("%08X"), dwCODE);

			strHEAD += strSIZE + strDATA.Mid( TCHAT_DWORD_SIZE, pPOS[1]);
		}

		strDATA = strDATA.Mid(TCHAT_DWORD_SIZE + pPOS[1]);
		wHEAD = strDATA.GetLength();
	}

	if(bTrimCrLf)
		strBODY.TrimRight(strTOK);
	nPos = nPOS + nFIND;

	if( strHEAD.IsEmpty() && strBODY.IsEmpty() )
		return CString(_T(""));

	return BuildNetString( strHEAD, strBODY);
}

// ===============================================================================
CString CTTextLinker::SplitTextByComp(TComponent* pComp, LONG nSize, const CString& strMSG, TTLINKLINE_ARRAY& outLINES)
{
	outLINES.clear();

	CSize szSIZE;
	CString strRESULT;
	CString strNEXT;
	CString strLEFT = strMSG;

	BOOL bTrimLeft;

	INT nMsgIndex(0), nStartIndex(0);
	CString strTEMP, strLINE;

	while(!strLEFT.IsEmpty())
	{
		bTrimLeft = TRUE;
		strTEMP.Empty();

		INT nTrimCnt = 0;
		nStartIndex = nMsgIndex;

		pComp->GetTextExtentPoint(strLEFT, szSIZE);
		if( szSIZE.cx > nSize )
		{
			strLINE.Empty();
			CString strWORD;

			while(!strLEFT.IsEmpty())
			{
				CHAR c = strLEFT.GetAt(0);
				if( bTrimLeft && c == ' ' )
				{
					strLEFT.Delete(0);
					++nMsgIndex;

					++nTrimCnt;
				}
				else if( c == ' ' )
				{
					if( !strWORD.IsEmpty() )
					{
						// 시험에 들게 하자.
						CString strTEST(strLINE);
						strTEST.Append( strWORD );
						pComp->GetTextExtentPoint( strTEST, szSIZE );
						if( szSIZE.cx > nSize )
						{
							// 이 단어를 포함할순없다.
							nMsgIndex -= strWORD.GetLength();
							strWORD.Append( strLEFT );
							strLEFT = strWORD;
							strWORD.Empty();
							break;
						}
						else
						{
							strLINE.Append( strWORD );
							strWORD.Empty();
						}
					}

					CString strTEST(strLINE);
					strTEST.AppendChar( c );
					pComp->GetTextExtentPoint( strTEST, szSIZE );
					if( szSIZE.cx > nSize )
						break;
					else
					{
						strLEFT.Delete(0);
						++nMsgIndex;

						strLINE.AppendChar( c );
					}
				}
				else
				{
					bTrimLeft = FALSE;
					strWORD.AppendChar(c);

					BYTE bMBCS = MBCS_LEAD_TEST(c);
					if(bMBCS)
						strWORD.AppendChar(strLEFT.GetAt(1));

					strLEFT.Delete(0);
					nMsgIndex++;

					if(bMBCS)
					{
						strLEFT.Delete(0);
						nMsgIndex++;
					}

					CSize sizeWORD;
					pComp->GetTextExtentPoint( strWORD, sizeWORD);

					if( sizeWORD.cx > nSize ) // 한 단어 길이가 컴포넌트의 가로길이보다 길다. 이럴땐 무조건 자르기.
					{
						CSize sizeLINE;
						pComp->GetTextExtentPoint( strLINE, sizeLINE );
						INT nRemainSize = nSize - sizeLINE.cx;

						CString strNewWord;
						for( INT i=0; i<strWORD.GetLength(); i++)
						{
							CString strCHAR;
							CHAR c = strWORD.GetAt(i);

							strCHAR.AppendChar(c);
							if(MBCS_LEAD_TEST(c))
								strCHAR.AppendChar(strWORD.GetAt(++i));

							CString strTEST(strNewWord);
							strTEST.Append(strCHAR);
							pComp->GetTextExtentPoint( strTEST, szSIZE);

							if( szSIZE.cx < nRemainSize )
								strNewWord.Append(strCHAR);
							else
								break;
						}

						// strNewWord만 strLINE에 포함, 나머진 strLEFT로 되돌아감.
						strWORD = strWORD.Right( strWORD.GetLength() - strNewWord.GetLength() );
						nMsgIndex -= strWORD.GetLength();
						strWORD.Append( strLEFT );
						strLEFT = strWORD;
						strWORD.Empty();

						strLINE.Append( strNewWord );
						break;
					}
				}
			}

			if( !strWORD.IsEmpty() )
			{
				// 시험에 들게 하자.
				CString strTEST(strLINE);
				strTEST.Append( strWORD );
				pComp->GetTextExtentPoint( strTEST, szSIZE );
				if( szSIZE.cx > nSize )
				{
					// 이 단어를 포함할순없다.
					nMsgIndex -= strWORD.GetLength();
					strWORD.Append( strLEFT );
					strLEFT = strWORD;
					strWORD.Empty();
				}
				else
				{
					strLINE.Append( strWORD );
					strWORD.Empty();
				}
			}
		}
		else
		{
			strLINE = strLEFT;
			nMsgIndex += strLINE.GetLength();

			if( nStartIndex )
			{
				nTrimCnt = strLINE.GetLength();
				strLINE.TrimLeft(" ");
				nTrimCnt -= strLINE.GetLength();
			}

			strLEFT.Empty();
		}
	
		strRESULT += strLINE + CString("\n");
		outLINES.push_back( TTEXT_LINK_LINE(nStartIndex,nMsgIndex,nTrimCnt,strLINE) );
	}

	return strRESULT;
}
// ===============================================================================
void CTTextLinker::GetTextSettings( TComponent* pTxtSettingComp, 
									const TTLINKLINE_ARRAY& vLINES,
									TLLINESETTING_ARRAY2D& outSettings )
{
	TComponent::TextSettingSet::iterator itr,end;
	itr = pTxtSettingComp->GetTextSettingItrBegin();
	end = pTxtSettingComp->GetTextSettingItrEnd();

	INT nLineSize = (INT) vLINES.size();

	outSettings.clear();
	outSettings.resize(nLineSize);

	for( INT i=0; i < nLineSize; ++i )
	{
		TLLINESETTING_ARRAY& vTextLineSettings = outSettings[i];

		const TTEXT_LINK_LINE& line = vLINES[i];
		while( itr != end )
		{
			TComponent::TextSetting& data = *itr;
			if( data.iStart >= line.nEnd )
				break;

			INT s = data.iStart - line.nStart;
			INT e = data.iEnd - line.nStart;
			INT l = line.nEnd - line.nStart - 1;
			
			if( s >= 0 )
			{
				s -= line.nTrimCnt;
				if( s < 0 )
					s = 0;

				e -= line.nTrimCnt;
				
				if( e >= 0 )
				{
					if( e <= l )
						vTextLineSettings.push_back( TLINK_LINE_SETTING(s,e,data.dwColor,data.dwUser) );
					else
					{
						vTextLineSettings.push_back( TLINK_LINE_SETTING(s,l,data.dwColor,data.dwUser) );
						break;
					}
				}
			}
			else
			{
				e -= line.nTrimCnt;

				if( e >= 0 )
				{
					if( e <= l )
						vTextLineSettings.push_back( TLINK_LINE_SETTING(0,e,data.dwColor,data.dwUser) );
					else
					{
						vTextLineSettings.push_back( TLINK_LINE_SETTING(0,l,data.dwColor,data.dwUser) );
						break;
					}
				}
			}

			++itr;
		}
	}
}
