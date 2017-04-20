#ifndef CHATFILTERFILE_H
#define CHATFILTERFILE_H


//!	ChatFilterFile Class.
class CTChatFilterFile
{
private :

	TMAP_FILTERDATA			m_mapFilterData;
	int						m_nMaxLength;
	int						m_nMinLength;

	CString					m_strFilterData;
	CString					m_strWordList;
	CString					m_strExceptList;

	BOOL					ReadWString( CFile* pFile, CString& strTEXT, DWORD dwCP );
	CString					Encrypt( const CString& _str );
	CString					Decrypt( const CString& _str );
	void					LoadExcept( std::vector< CString >& vecExcept );

public :

							CTChatFilterFile();
							~CTChatFilterFile();

	bool					LoadTxtWords( CString& strFileName );
	bool					LoadTxtExcept( CString& strFileName );
	void					SaveTxtWords( CString& strFileName );
	void					SaveTxtExcept( CString& strFileName );

	bool					LoadData( CString& strFileName );
	bool					SaveData( CString& strFileName );

	void					ReleaseAll();
	void					ReleaseExcept();

public :

	int						GetMaxLength() const;
	int						GetMinLength() const;

	TMAP_FILTERDATA*		GetFilterData();
};

#endif