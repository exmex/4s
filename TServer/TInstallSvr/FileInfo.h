#pragma once

class CFileInfo
{
public:
	CFileInfo(void);
	~CFileInfo(void);

public:
	DWORD	m_dwVersion;
	CString m_strName;
	DWORD	m_dwSize;
	DWORD	m_dwPatchVer;
	WORD	m_wType;

private:
	map<DWORD, TUserInfo> m_User;

public:

};
