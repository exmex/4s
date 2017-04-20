#include "stdafx.h"
#include "ChatFilterType.h"

//	struct CTChatFilterOption.
int CTChatFilterOption::bBlockAllSentence		= 0;
int CTChatFilterOption::bUseExceptionList		= 0;
int CTChatFilterOption::bIgnoreSpace			= 1;
int CTChatFilterOption::bIgnoreNumChar			= 1;

// 
CString CTChatFilterOption::Apply( CString& str )
{
	if( str.IsEmpty() )
		return str;

	str.Trim();
	str.MakeUpper();

	if( CTChatFilterOption::bIgnoreSpace )
		str.Remove( _T(' ') );

	if( CTChatFilterOption::bIgnoreNumChar )
	{
		int nPos = 0;
		CString strRes;
		CString strTemp;
		while( ( strRes = str.Tokenize( CTChatFilter::strIgnoreNumChar, nPos ) ) != "" )
			strTemp += strRes;

		str = strTemp;		//!<	무시할 숫자, 문자를 제외한 텍스트.
	}

	return str;
}


//	struct CTChatFilter.
CHAR	CTChatFilter::chReplace					= 0x1D;
CString CTChatFilter::strFileExt				= _T("TCD");
CString CTChatFilter::strFileName				= _T("TStringCF");
CString	CTChatFilter::strVReplace				= _T("");
CString CTChatFilter::strReplace				= _T("*");
CString CTChatFilter::strBlockMsg				= _T("You cannot use forbidden words.");
CString CTChatFilter::strIgnoreNumChar			= _T("~!@#$%^&*()-=\\_+|,./;'[]<>?:\"{}.`0123456789");
CString CTChatFilter::strTagIgnoreSpace			= _T("[IGN_SPACE]");
CString CTChatFilter::strTagIgnoreNumChar		= _T("[IGN_NUMCHAR]");
CString CTChatFilter::strTagBlockAllChat		= _T("[BLOCK_ALL_SENTENCE]");
CString CTChatFilter::strTagException			= _T("[EXCEPTION]");
