#include "stdafx.h"
#include "ChatFilterSys.h"
#include <windows.h>
#include <algorithm>


CTChatFilterSys::CTChatFilterSys()
{
	m_pFile = NULL;
	m_bForbiddenAtFirst = false;
	m_bUsedForbiddenWords = false;
}

CTChatFilterSys::~CTChatFilterSys()
{
}

//----------------------------------------------------------------
//		Implementation CTChatFilterSys Protected Functions.
//----------------------------------------------------------------
int CTChatFilterSys::ReplaceNoCase( CString& strUserChat, CString strOld, CString strNew )
{
	if( strOld.IsEmpty() ) return 0;
	strOld.MakeUpper();

	CString strTemp = strUserChat;
	CString strTemp2;
	strTemp.MakeUpper();

	int nStart		= 0;
	int nFind_Pos	= 0;
	int nReplaced	= 0;

	while( ( nFind_Pos = strTemp.Find( strOld, nStart ) ) >= 0 )
	{
		int nPos = nFind_Pos - nStart;
		strTemp2 += strUserChat.Mid( nStart, nPos );
		strTemp2 += strNew;
		nStart += ( strOld.GetLength() + nPos );
		++nReplaced;
	}

	if( nReplaced > 0 )
		strUserChat = strTemp2 + strUserChat.Mid( nStart );

	return nReplaced;
}

bool CTChatFilterSys::Function( CString& strUserChat, int nLength, bool bCheckHeader /*= false*/, bool bFastCheck /*= false*/ )
{
	if( !m_pFile )							return false;
	if( strUserChat.IsEmpty() )				return false;
	if( nLength < m_pFile->GetMinLength() )	return false;

	int i = 0;
	int nInc = 0;
	int startPos = 0;
	int endPos = 0;
	int nOffset = strUserChat.GetLength() - nLength;
	for( ; i <= nOffset; i += nInc )
	{
		nInc = ::IsDBCSLeadByte( strUserChat.GetAt( i ) ) ? 2 : 1;

		while( i+nLength > endPos )
			endPos += ::IsDBCSLeadByte( strUserChat.GetAt( endPos ) ) ? 2 : 1;

		if( (i+nLength) != endPos )
			continue;

		CString strOld = strUserChat.Mid( i, nLength );
		strOld.MakeUpper();
		CString strNew = strOld;
		CTChatFilterOption::Apply( strNew );
		if( strNew.IsEmpty() )
			continue;
		if( strOld[0] != strNew[0] )
			continue;

		TMAP_FILTERDATA* pMapFilter = m_pFile->GetFilterData();
		TOuter_Iter find = pMapFilter->find( strNew );
		TOuter_Iter end = pMapFilter->end();
		if( find != end )
		{
			TMAP_EXCEPTDATA* pExceptData = (*find).second;
			bool bFindExcept = false;
			// Exception List 가 있을때..
			if( pExceptData && pExceptData->size() > 0 )
			{
				TInner_Iter ex_iter = pExceptData->begin();
				TInner_Iter ex_end = pExceptData->end();
				while( ex_iter != ex_end )
				{
					int nExStart = i - (*ex_iter).second.head_gap;
					int nExLength = (*ex_iter).first.GetLength();
					if( nExStart < 0 || nExLength < 0 )
					{
						++ex_iter;
						continue;
					}

					CString strExtention = strUserChat.Mid( nExStart, nExLength );
					strExtention.MakeUpper();

					if( strExtention == (*ex_iter).first )
					{
						bFindExcept = true;
						break;
					}

					++ex_iter;
				}
			}

			if( bFindExcept == false )
			{
				m_bUsedForbiddenWords = true;

				if( i == 0 )
					m_bForbiddenAtFirst = true;

				if( CTChatFilterOption::bBlockAllSentence == TRUE || bFastCheck == true )
					return false;

				CString strRep;
				strRep.Format( _T("%c"), TCHAR_REPLACE );

				if( bCheckHeader )
					strRep = CString( CTChatFilter::chReplace, strOld.GetLength() );

				ReplaceNoCase( strUserChat, strOld, strRep );
				nOffset = strUserChat.GetLength() - nLength;
				endPos -= nInc - 1;
				nInc = 1;
			}
		}
	}

	return true;
}

//----------------------------------------------------------------
//		End of CTChatFilterSys Protected Functions.
//----------------------------------------------------------------


//----------------------------------------------------------------
//		Implementation CTChatFilterSys public Functions.
//----------------------------------------------------------------
bool CTChatFilterSys::CheckFobiddenWords( CString strUserChat )
{
	m_bForbiddenAtFirst		= false;
	m_bUsedForbiddenWords	= false;

	int nLength = strUserChat.GetLength();
	while( Function( strUserChat, nLength--, false, true ) );

	CString str;
	str.Format( _T("%c"), TCHAR_REPLACE );
	strUserChat.Replace( str, CTChatFilter::strReplace );

	return m_bUsedForbiddenWords;
}

void CTChatFilterSys::ConvertString( CString& strUserChat, bool bCheckHeader )
{
	m_bForbiddenAtFirst		= false;
	m_bUsedForbiddenWords	= false;

	int nLength = strUserChat.GetLength();
	while( Function( strUserChat, nLength--, bCheckHeader ) );

	CString str;
	str.Format( _T("%c"), TCHAR_REPLACE );
	strUserChat.Replace( str, CTChatFilter::strReplace );
}

bool CTChatFilterSys::UsedFobiddenAtFirst() const
{
	return m_bForbiddenAtFirst;
}

bool CTChatFilterSys::UsedFobiddenWords() const
{
	return m_bUsedForbiddenWords;
}

void CTChatFilterSys::SetFilterFile( CTChatFilterFile* pFile )
{
	m_pFile = pFile;
}

CTChatFilterFile* CTChatFilterSys::GetFilterFile()
{
	return m_pFile;
}
//----------------------------------------------------------------
//		End of CTChatFilterSys public Functions.
//----------------------------------------------------------------

