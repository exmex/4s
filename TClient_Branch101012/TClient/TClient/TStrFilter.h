#pragma once


//! Client StringFilter class.
class CTStrFilter
{
public :

	static CTChatFilterFile		m_file;
	static CTChatFilterSys		m_sys;

private :

	static int					GetFirstPosition( CString& strUserChat );
	static CString				MakeNetString( CString& strUserChat );

public :

	static bool				LoadTStringCF();
	static bool				CheckFobiddenWords( CString& strUserChat );
	static bool				ConvertString( CString& strUserChat, bool bCheckHeader );
	static void				Release();
};

