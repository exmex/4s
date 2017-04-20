/*
 *	XRegKey.h
 */
#pragma once


class CXRegKey : public CRegKey
{
public:
	CXRegKey()
		: CRegKey()
	{}

public:
	LONG	SetBinaryValue(const void* pData, LPCTSTR lpszValueName, DWORD dwBytes);
	LONG	QueryBinaryValue(void* pValue, LPCTSTR lpszValueName, DWORD* pdwCount);
};



inline LONG CXRegKey::SetBinaryValue(const void* pData, LPCTSTR lpszValueName, DWORD dwBytes)
{
	ATLASSERT( pData != NULL );
	ATLASSERT( m_hKey != NULL );

	return ::RegSetValueEx(m_hKey, lpszValueName, NULL, REG_BINARY, (BYTE * const)pData, dwBytes);
}




inline LONG CXRegKey::QueryBinaryValue(void* pValue, LPCTSTR lpszValueName, DWORD* pdwCount)
{
	ATLASSERT( pdwCount != NULL );
	ATLASSERT( m_hKey != NULL );
	
	DWORD	dwType = NULL;	

	LONG lRes = ::RegQueryValueEx(m_hKey, lpszValueName, NULL, &dwType, (LPBYTE)pValue,pdwCount);

	ATLASSERT( (lRes != ERROR_SUCCESS) || (dwType == REG_BINARY) );	

	return lRes;
}

