////////////////////////////////////////////////////////////////////////////////
//	File Name	:	Profile.h
//
//	Copyright	:	(c) Zemi Interactive, Inc. 2002-2009
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

using std::string;


/**
 * @author		Jason
 * @version		1.0
 * @date		03/01/2002
 * @brief		Read an INI file
 */
class CProfile  
{

public:

	CProfile(const char* pcszFileName_);
	CProfile(
			const char* pcszDirName_,
			const char* pcszFileName_);
	~CProfile() {}

	unsigned int GetIntKey(
		const char* pcszSection_, 
		const char* pcszKey_, 
		UINT unDefaultValue_);

	unsigned long GetStringKey(
		const char* pcszSection_, 
		const char* pcszKey_, 
		char* pszOutBuf_, 
		DWORD dwSizeOutBuf_,  
		const char* pcszDefaultString_);

private:

	string m_FileName;					///< Name of the ini file
};


/**
 * Set a file name
 * @param pcszFileName_				INI file name
 */
inline CProfile::CProfile(const char* pcszFileName_)
{
	char  szPath[MAX_PATH];

	ZeroMemory(szPath, MAX_PATH);

	GetCurrentDirectory(MAX_PATH, szPath);

	m_FileName = szPath;
	m_FileName += "\\";
	m_FileName += pcszFileName_;
}

/**
 * Set a directory and file name
 * @param pcszDirName_					Directory
 * @param pcszFileName_					INI file name
 */
inline CProfile::CProfile(
		const char* pcszDirName_, 
		const char* pcszFileName_)
{
	m_FileName = pcszDirName_;
	m_FileName += "\\";
	m_FileName += pcszFileName_;
}

/**    
 * Get a int value
 * @param	pcszSection_				Session name that has the key value
 * @param	pcszKey_					Key value
 * @param	unDefaultValue_				Default value
 * @return	Key value
 */
inline UINT CProfile::GetIntKey(
		const char* pcszSection_,
		const char* pcszKey_,				
		UINT unDefaultValue_)				
{
	return (GetPrivateProfileInt(
		pcszSection_, 
		pcszKey_, 
		unDefaultValue_, 
		m_FileName.c_str()));
}

/**    
 * Get a string value
 * @param	pcszSection_				Session name that has the key value
 * @param	pcszKey_					key string
 * @param	pszOutBuf_					[out] Pointer to a variable that receives the string
 * @param	dwSizeOutBuf_				[out] Pointer to a variable that receives the buffer's size
 * @param	pcszDefaultString_			Default value
 * @return	String Key
 */
inline DWORD CProfile::GetStringKey(
		const char* pcszSection_, 
		const char* pcszKey_, 
		char* pszOutBuf_, 
		DWORD dwSizeOutBuf_, 
		const char* pcszDefaultString_)
{
	return (GetPrivateProfileString(
		pcszSection_, 
		pcszKey_, 
		pcszDefaultString_, 
		pszOutBuf_, 
		dwSizeOutBuf_, 
		m_FileName.c_str()));
}

