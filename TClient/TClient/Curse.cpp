#include "Stdafx.h"
#include "Curse.h"
#include "Resource.h"

// ===============================================================================
CString				CCurse::CONVERT_STRING("*");
CHAR				CCurse::CONVERT_CHAR(0x1D);
int					CCurse::m_nMaxLen = 0;
vector<CString> 	CCurse::m_vStrCurse;	
// ===============================================================================
DWORD CCurse::ReadString(CHAR* pDATA, CString& outSTR)
{
	const char CR = 13;

	outSTR.Empty();

	DWORD dwPOS = 0;
	while(pDATA[dwPOS])
	{
		if(MBCS_LEAD_TEST(pDATA[dwPOS]))
		{
			outSTR.AppendChar( pDATA[dwPOS++] );
			outSTR.AppendChar( pDATA[dwPOS++] );
		}
		else
		{
			if( pDATA[dwPOS] == CR )
			{
				dwPOS += 2;
				break;
			}

			outSTR.AppendChar( pDATA[dwPOS++] );
		}
	}

	return dwPOS;
}
// ===============================================================================
void CCurse::Load(HINSTANCE hInstance, DWORD dwResID, CString strResGrp)
{
	HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(dwResID), strResGrp);
	if( !hResource )
		return;

	HGLOBAL hGlobal = LoadResource(hInstance, hResource);
	if( !hGlobal )
		return;

	DWORD dwSize = SizeofResource(hInstance, hResource);
	if( dwSize == 0 )
		return;

	CHAR* pBUF = new CHAR[dwSize];
	memcpy(pBUF, LockResource(hGlobal), dwSize);

	CHAR* pCUR = pBUF;
	DWORD dwCUR = 0;

	m_nMaxLen = 0;

	while( dwCUR < dwSize )
	{
		CString str;
		DWORD dwLEN = ReadString(pCUR, str);

		dwCUR += dwLEN;
		pCUR += dwLEN;

		str.TrimRight();
		str.TrimLeft();
		
		int nLen = str.GetLength();
		if( nLen > 0 )
		{
			if( nLen > m_nMaxLen )
				m_nMaxLen = nLen;

			if( CTNationOption::ENGLISH )
				str.MakeUpper();

			m_vStrCurse.push_back(str);
		}
	}

	delete pBUF;

	std::sort(m_vStrCurse.begin(), m_vStrCurse.end());
}
// ===============================================================================
bool  CCurse::IsCurse(const CString& str)
{
	static CString strTEMP;
	int nLen,nMaxLen,nInc,i,j;

	i = 0;
	nLen = str.GetLength();
	while( i < nLen )
	{
		nInc = MBCS_LEAD_TEST(str.GetAt(i)) ? 2 : 1;
		nMaxLen = min( m_nMaxLen, nLen - i);

		j = nInc;
		while( j <= nMaxLen )
		{
			strTEMP = str.Mid( i, j);

			CString strTARGET = strTEMP;

			if( CTNationOption::ENGLISH )
				strTARGET.MakeUpper();

			if(std::binary_search( m_vStrCurse.begin(), m_vStrCurse.end(), strTARGET))
				return true;

			j += MBCS_LEAD_TEST(str.GetAt(i + j)) ? 2 : 1;
		}

		i += nInc;
	}

	return false;
}
// ===============================================================================
const CString& CCurse::ConvertString( const CString& str, BYTE bCheckHeader)
{
	static CString strREPLACE(_T(""));
	static CString strTEMP;
	static CString strRESULT;

	int nLen,nMaxLen,nInc,i,j;
	int nPOS = 0;

	BYTE bCurseAtFirst = FALSE;
	BYTE bHasCurse = FALSE;

	if(strREPLACE.IsEmpty())
		strREPLACE = CString( CONVERT_CHAR, m_nMaxLen);

	strRESULT.Empty();
	i = 0;
	nLen = str.GetLength();

	if(bCheckHeader)
	{
		if( nLen < TCHAT_DWORD_SIZE )
			return strRESULT;

		DWORD dwCODE = 0;
		WORD wHEAD = 0;
		WORD wBODY = 0;

		sscanf( LPCSTR(str.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
		wHEAD = HIWORD(dwCODE);
		wBODY = LOWORD(dwCODE);

		if( wHEAD + wBODY + TCHAT_DWORD_SIZE != nLen )
			return strRESULT;

		nPOS = TCHAT_DWORD_SIZE + wHEAD;
		i = nPOS;
	}
	strRESULT = str;

	while( i < nLen )
	{
		nInc = MBCS_LEAD_TEST(str.GetAt(i)) ? 2 : 1;
		nMaxLen = min(m_nMaxLen, nLen-i);

		j = nInc;
		while( j <= nMaxLen )
		{
			strTEMP = str.Mid(i, j);

			CString strTARGET = strTEMP;

			if( CTNationOption::ENGLISH )
				strTARGET.MakeUpper();

			if( std::binary_search(m_vStrCurse.begin(), m_vStrCurse.end(), strTARGET) )
			{
				strRESULT.Replace( strTEMP, bCheckHeader ? strREPLACE.Left(strTEMP.GetLength()) : CONVERT_STRING);

				if( i == nPOS )
					bCurseAtFirst = TRUE;

				bHasCurse = TRUE;
				i += j-nInc;

				break;
			}

			j += MBCS_LEAD_TEST(str.GetAt(i + j)) ? 2 : 1;
		}

		i += nInc;
	}

	if( bHasCurse && bCheckHeader )
	{
		CTTextLinker *pTextLinker = CTTextLinker::GetInstance();

		CString strCONVERT = pTextLinker->BuildNetString( CString(_T("")), CONVERT_STRING);
		CString strTOK(CONVERT_CHAR);
		BYTE bLAST = strRESULT.GetAt(nLen - 1) == CONVERT_CHAR ? TRUE : FALSE;

		strTEMP = bCurseAtFirst ? strCONVERT : pTextLinker->BuildNetString( CString(_T("")), CString(_T("")));
		nPOS = 0;
		CString strLINE = pTextLinker->SplitTextByCrLf( strRESULT, strTOK, nPOS);

		while(!strLINE.IsEmpty())
		{
			strTEMP = pTextLinker->AppendNetString( strTEMP, strLINE);
			strLINE = pTextLinker->SplitTextByCrLf( strRESULT, strTOK, nPOS);
			bCurseAtFirst = FALSE;

			if(!strLINE.IsEmpty())
				strTEMP = pTextLinker->AppendNetString( strTEMP, strCONVERT);
		}

		if( !bCurseAtFirst && bLAST )
			strTEMP = pTextLinker->AppendNetString( strTEMP, strCONVERT);

		return strTEMP;
	}

	return strRESULT;
}
// ===============================================================================