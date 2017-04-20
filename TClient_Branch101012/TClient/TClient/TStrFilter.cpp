#include "stdafx.h"
#include "TStrFilter.h"
#include "TClientGame.h"
#include "TClientChar.h"


CTChatFilterFile	CTStrFilter::m_file;
CTChatFilterSys		CTStrFilter::m_sys;


int CTStrFilter::GetFirstPosition( CString& strUserChat )
{
	int nLen = strUserChat.GetLength();

	if( nLen < TCHAT_DWORD_SIZE )
		return -1;

	DWORD dwCODE = 0;
	WORD wHEAD = 0;
	WORD wBODY = 0;

	sscanf( LPCSTR(strUserChat.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
	wHEAD = HIWORD(dwCODE);
	wBODY = LOWORD(dwCODE);

	if( wHEAD + wBODY + TCHAT_DWORD_SIZE != nLen )
		return -1;

	return ( TCHAT_DWORD_SIZE + wHEAD );
}

CString CTStrFilter::MakeNetString( CString& strUserChat )
{
	CTTextLinker *pTextLinker = CTTextLinker::GetInstance();

	CString strCONVERT = pTextLinker->BuildNetString( CString(_T("")), CTChatFilter::strReplace );
	CString strTOK( CTChatFilter::chReplace );
	BYTE bLAST = strUserChat.GetAt( strUserChat.GetLength() - 1 ) == CTChatFilter::chReplace ? TRUE : FALSE;

	bool bFobiddenAtFirst = m_sys.UsedFobiddenAtFirst();

	CString strTEMP;
	if( bFobiddenAtFirst )
		strTEMP = strCONVERT;
	else
		strTEMP = pTextLinker->BuildNetString( CString(_T("")), CString(_T("")) );

	int nPOS = 0;
	CString strLINE = pTextLinker->SplitTextByCrLf( strUserChat, strTOK, nPOS);

	while(!strLINE.IsEmpty())
	{
		strTEMP = pTextLinker->AppendNetString( strTEMP, strLINE);
		strLINE = pTextLinker->SplitTextByCrLf( strUserChat, strTOK, nPOS);
		bFobiddenAtFirst = false;

		if(!strLINE.IsEmpty())
			strTEMP = pTextLinker->AppendNetString( strTEMP, strCONVERT);
	}

	if( !bFobiddenAtFirst && bLAST )
		strTEMP = pTextLinker->AppendNetString( strTEMP, strCONVERT);

	return strTEMP;
}

bool CTStrFilter::LoadTStringCF()
{
	CString strFileName = _T(".\\Tcd\\TStringCF.tcd");
	return m_file.LoadData( strFileName );
}

bool CTStrFilter::CheckFobiddenWords( CString& strUserChat )
{
	return m_sys.CheckFobiddenWords( strUserChat );
}

bool CTStrFilter::ConvertString( CString& strUserChat, bool bCheckHeader )
{
	int nStart = bCheckHeader ? GetFirstPosition( strUserChat ) : 0;
	if( nStart == -1 )
	{
		strUserChat.Empty();
		return false;
	}

	CString strTemp = strUserChat.Mid( nStart );
	CString strHeader = strUserChat.Mid( 0, nStart );

	if( CTChatFilterOption::bBlockAllSentence )
	{
		if( m_sys.CheckFobiddenWords( strTemp ) )
		{
			CTClientGame* pGame = CTClientGame::GetInstance();

			pGame->m_pChatFrame->ChatSysMSG( 
				CTChart::LoadString( TSTR_INFO_TITLE ),
				pGame->GetMainChar()->GetName(),
				CTChatFilter::strBlockMsg,
				TCOLOR_ERROR,
				TCHAT_FLAG_INFO);

			return false;
		}
	}
	else
	{
		m_sys.ConvertString( strTemp, bCheckHeader );
		strTemp = strHeader + strTemp;

		if( m_sys.UsedFobiddenWords() && bCheckHeader )
			strUserChat = MakeNetString( strTemp );
	}

	return true;
}

void CTStrFilter::Release()
{
	m_file.ReleaseAll();
}
