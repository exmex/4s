#include "stdafx.h"
#include "ChatFilterFile.h"


CTChatFilterFile::CTChatFilterFile()
:	m_nMaxLength( 0 ),
	m_nMinLength( 1024 )
{
	ReleaseAll();
}

CTChatFilterFile::~CTChatFilterFile()
{
	ReleaseAll();
}

//----------------------------------------------------------------
//		Implementation CTChatFilterSys public Functions.
//----------------------------------------------------------------
BOOL CTChatFilterFile::ReadWString( CFile* pFile, CString& strTEXT, DWORD dwCP )
{
	if( !pFile )
		return FALSE;

	TCHAR vCH[TMAX_MBCS_SIZE];
	WCHAR wCH;

	strTEXT.Empty();
	if( pFile->Read( &wCH, sizeof(WCHAR)) < sizeof(WCHAR) )
		return FALSE;

	int nLength = WideCharToMultiByte(
		dwCP, 0,
		&wCH, 1,
		vCH, TMAX_MBCS_SIZE,
		NULL,
		NULL);

	while( nLength != TDEF_CHAR_SIZE || vCH[0] != TDEF_CHAR_NL )
	{
		if( wCH != TDEF_WCHAR_NBS && (nLength != TDEF_CHAR_SIZE || vCH[0] != TDEF_CHAR_CR) )
			for( int i=0; i<nLength; i++)
				strTEXT.AppendChar(vCH[i]);

		if( pFile->Read( &wCH, sizeof(WCHAR)) < sizeof(WCHAR) )
			return TRUE;

		nLength = WideCharToMultiByte(
			dwCP, 0,
			&wCH, 1,
			vCH, TMAX_MBCS_SIZE,
			NULL,
			NULL);
	}

	return TRUE;
}

const int nHexa = 7;
const unsigned char chHexaCode[nHexa] = { 0xA0, 0x04, 0x54, 0x7A, 0xDA, 0x0A, 0x77 };
CString CTChatFilterFile::Encrypt( const CString& _str )
{
	char* pString = LPSTR(LPCTSTR( _str ));
	size_t size = _str.GetLength();
	size_t i = 0;
	for( ; i < size; ++i )
		pString[i] = pString[i] ^ chHexaCode[ i % nHexa ];

	return _str;
}

CString CTChatFilterFile::Decrypt( const CString& _str )
{
	char* pString = LPSTR(LPCTSTR( _str ));
	size_t size = _str.GetLength();
	size_t i = 0;
	for( ; i < size; ++i )
		pString[i] = pString[i] ^ chHexaCode[ i % nHexa ];

	return _str;
}

void CTChatFilterFile::LoadExcept( std::vector< CString >& vecExcept )
{
	if( vecExcept.size() == 0 )
		return ;

	TOuter_Iter o_iter = m_mapFilterData.begin();
	TOuter_Iter o_end = m_mapFilterData.end();
	while( o_iter != o_end )
	{
		std::vector< CString >::iterator vIter = vecExcept.begin();
		while( vIter != vecExcept.end() )
		{
			TOuter_Iter find = m_mapFilterData.find( (*vIter) );
			if( find == o_end )
			{
				int find_pos = (*vIter).Find( (*o_iter).first );
				if( find_pos != -1 )
				{
					TFILTER_EXCEPTINFO ExceptGap;
					ExceptGap.head_gap = find_pos;
					ExceptGap.tail_gap = (*vIter).GetLength() - ( find_pos + (*o_iter).first.GetLength() );

					if( (*o_iter).second == 0 )
					{
						TMAP_EXCEPTDATA* pMapExcept = new TMAP_EXCEPTDATA;
						(*o_iter).second = pMapExcept;
					}

					(*o_iter).second->insert( std::make_pair( (*vIter), ExceptGap ) );
					//	vIter = vecExcept.erase( vIter );
					//}
					//else
					//{
					//	++vIter;
				}
			}

			++vIter;
		}

		++o_iter;
	}
}

bool CTChatFilterFile::LoadTxtWords( CString& strFileName )
{
	ReleaseAll();

	UINT nOpenFlags = CFile::modeRead | CFile::typeBinary;

	CFile fWordList;
	if( fWordList.Open( strFileName, nOpenFlags ) )
	{
		CString str;

		while( ReadWString( &fWordList, str, CP_ACP ) )
		{
			if( str.IsEmpty() )
				continue;

			CTChatFilterOption::Apply( str );

			int nLength = str.GetLength();
			m_nMaxLength = max( m_nMaxLength, nLength );
			m_nMinLength = min( m_nMinLength, nLength );

			m_mapFilterData.insert( std::make_pair( str, (TMAP_EXCEPTDATA*)0 ) );
		}
	}
	else
	{
		fWordList.Close();
		return false;
	}

	fWordList.Close();
	return true;
}

bool CTChatFilterFile::LoadTxtExcept( CString& strFileName )
{
	if( m_mapFilterData.size() == 0 )
		return false;

	UINT nOpenFlags = CFile::modeRead | CFile::typeBinary;

	std::vector< CString >	vecExcept;
	CFile fExcept;
	if( fExcept.Open( strFileName, nOpenFlags ) )
	{
		ReleaseExcept();

		CString str;
		while( ReadWString( &fExcept, str, CP_ACP ) )
		{
			if( str.IsEmpty() )
				continue;

			CTChatFilterOption::Apply( str );

			vecExcept.push_back( str );
		}

		LoadExcept( vecExcept );
	}
	else
	{
		fExcept.Close();
		return false;
	}

	fExcept.Close();
	return true;
}

void CTChatFilterFile::SaveTxtWords( CString& strFileName )
{
	int nOpenFlags = CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::typeBinary;
	CStdioFile fWordList;
	if( !fWordList.Open( strFileName, nOpenFlags ) )
	{
		fWordList.Close();

		// Error.
		return ;
	}

	WCHAR wCH = TDEF_WCHAR_NBS;
	fWordList.Write( &wCH, sizeof( WCHAR ) );

	TOuter_Iter o_iter = m_mapFilterData.begin();
	TOuter_Iter o_end = m_mapFilterData.end();
	while( o_iter != o_end )
	{
		if( (*o_iter).first.IsEmpty() )
		{
			++o_iter;
			continue;
		}

		fWordList.WriteString( (*o_iter).first );

		++o_iter;
	}

	fWordList.Close();
}

void CTChatFilterFile::SaveTxtExcept( CString& strFileName )
{
	int nOpenFlags = CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::typeBinary;
	CStdioFile fExceptList;
	if( !fExceptList.Open( strFileName, nOpenFlags ) )
	{
		fExceptList.Close();

		// Error.
		return ;
	}

	WCHAR wCH = TDEF_WCHAR_NBS;
	fExceptList.Write( &wCH, sizeof( WCHAR ) );

	TOuter_Iter o_iter = m_mapFilterData.begin();
	TOuter_Iter o_end = m_mapFilterData.end();
	while( o_iter != o_end )
	{
		if( (*o_iter).first.IsEmpty() )
		{
			++o_iter;
			continue;
		}

		if( (*o_iter).second )
		{
			TInner_Iter i_iter = (*o_iter).second->begin();
			TInner_Iter i_end = (*o_iter).second->end();
			while( i_iter != i_end )
			{
				if( (*i_iter).first.IsEmpty() )
				{
					++i_iter;
					continue;
				}

				fExceptList.WriteString( (*i_iter).first );

				++i_iter;
			}
		}

		++o_iter;
	}

	fExceptList.Close();
}

bool CTChatFilterFile::LoadData( CString& strFileName )
{
	int nOpenFlags = CFile::modeRead | CFile::typeBinary;
	CFile file;
	if( !file.Open( strFileName, nOpenFlags ) )
	{
		// Error.
		return false;
	}

	ReleaseAll();

	CArchive ar( &file, CArchive::load );

	ar
		>> CTChatFilterOption::bIgnoreSpace
		>> CTChatFilterOption::bIgnoreNumChar
		>> CTChatFilter::strIgnoreNumChar
		>> CTChatFilterOption::bBlockAllSentence
		>> CTChatFilter::strBlockMsg;

	Decrypt( CTChatFilter::strIgnoreNumChar );
	Decrypt( CTChatFilter::strBlockMsg );

	// Word List.
	DWORD nSize = 0;
	ar >> nSize;
	DWORD i = 0;
	for( ; i < nSize; ++i )
	{
		CString str;
		ar >> str;

		Decrypt( str );

		if( str.IsEmpty() )
			continue;

		CTChatFilterOption::Apply( str );

		int nLength = str.GetLength();
		m_nMaxLength = max( m_nMaxLength, nLength );
		m_nMinLength = min( m_nMinLength, nLength );

		m_mapFilterData.insert( std::make_pair( str, (TMAP_EXCEPTDATA*)0 ) );
	}

	std::vector< CString > vecExcept;
	ar >> nSize;
	i = 0;
	for( ; i < nSize; ++i )
	{
		CString str;
		ar >> str;

		Decrypt( str );

		if( str.IsEmpty() )
			continue;

		CTChatFilterOption::Apply( str );
		vecExcept.push_back( str );
	}

	LoadExcept( vecExcept );

	return true;
}

bool CTChatFilterFile::SaveData( CString& strFileName )
{
	int nOpenFlags = CFile::modeCreate | CFile::modeWrite | CFile::typeBinary;

	CFile file( LPCSTR(strFileName), nOpenFlags );
	CArchive ar( &file, CArchive::store );

	CString strEncryptNumChar = CTChatFilter::strIgnoreNumChar;
	CString strEncryptBlockMsg = CTChatFilter::strBlockMsg;
	Encrypt( strEncryptNumChar );
	Encrypt( strEncryptBlockMsg );

	ar
		<< CTChatFilterOption::bIgnoreSpace
		<< CTChatFilterOption::bIgnoreNumChar
		<< strEncryptNumChar
		<< CTChatFilterOption::bBlockAllSentence
		<< strEncryptBlockMsg;

	ar << (DWORD)m_mapFilterData.size();

	std::vector< CString > vecExcept;

	TOuter_Iter	o_iter = m_mapFilterData.begin();
	TOuter_Iter	o_end = m_mapFilterData.end();
	while( o_iter != o_end )
	{
		if( (*o_iter).first.IsEmpty() )
		{
			++o_iter;
			continue;
		}

		CString str = (*o_iter).first;
		Encrypt( str );
		// Write String.
		ar << str;

		if( (*o_iter).second )
		{
			TInner_Iter i_iter = (*o_iter).second->begin();
            TInner_Iter i_end = (*o_iter).second->end();
			while( i_iter != i_end )
			{
				if( (*i_iter).first.IsEmpty() )
				{
					++i_iter;
					continue;
				}

				vecExcept.push_back( (*i_iter).first );

				++i_iter;
			}
		}

		++o_iter;
	}

	// Except list size.
	DWORD size = (DWORD)vecExcept.size();
	ar << size;

	DWORD i = 0;
	for( ; i < size; ++i )
	{
		if( vecExcept[i].IsEmpty() )
			continue;

		CString str = vecExcept[i];
		Encrypt( str );

		// Write Exception String.
		ar << str;
	}

	return true;
}

void CTChatFilterFile::ReleaseAll()
{
	m_nMaxLength = 0;
	m_nMinLength = 1024;

	ReleaseExcept();

	m_mapFilterData.clear();
}

void CTChatFilterFile::ReleaseExcept()
{
	TOuter_Iter iter = m_mapFilterData.begin();
	TOuter_Iter end = m_mapFilterData.end();

	while( iter != end )
	{
		if( (*iter).second )
		{
			(*iter).second->clear();
			delete (*iter).second;
			(*iter).second = NULL;
		}
		++iter;
	}
}
//----------------------------------------------------------------
//		End of CTChatFilterSys public Functions.
//----------------------------------------------------------------


//----------------------------------------------------------------
//		Implementation CTChatFilterSys inline Functions.
//----------------------------------------------------------------
//void CTChatFilterFile::SetDir_FilterData( CString strFileName )
//{
//	m_strFilterData = strFileName;
//}
//
//void CTChatFilterFile::SetDir_WordList( CString strFileName )
//{
//	m_strWordList = strFileName;
//}
//
//void CTChatFilterFile::SetDir_ExceptList( CString strFileName )
//{
//	m_strExceptList = strFileName;
//}
//
//CString CTChatFilterFile::GetDir_FilterData() const
//{
//	return m_strFilterData;
//}
//
//CString CTChatFilterFile::GetDir_WordList() const
//{
//	return m_strWordList;
//}
//
//CString CTChatFilterFile::GetDir_ExceptList() const
//{
//	return m_strExceptList;
//}

int CTChatFilterFile::GetMaxLength() const
{
	return m_nMaxLength;
}

int CTChatFilterFile::GetMinLength() const
{
	return m_nMinLength;
}


TMAP_FILTERDATA* CTChatFilterFile::GetFilterData()
{
	return &m_mapFilterData;
}
//----------------------------------------------------------------
//		End of CTChatFilterSys inline Functions.
//----------------------------------------------------------------
