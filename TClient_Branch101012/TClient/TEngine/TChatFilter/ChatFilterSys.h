#ifndef CHATFILTER_SYS
#define CHATFILTER_SYS


//!	Chat Filtering System.
/*!
*	@brief		채팅필터링.
*
*/
class CTChatFilterSys
{
private :

	CTChatFilterFile*				m_pFile;
	bool							m_bForbiddenAtFirst;
	bool							m_bUsedForbiddenWords;

protected :

	int								ReplaceNoCase( CString& strUserChat, CString strOld, CString strNew );
	bool							Function( CString& strUserChat, int nLength, bool bCheckHeader = false, bool bFastCheck = false );

public :

									CTChatFilterSys();
									~CTChatFilterSys();

									/*!
									*	@brief	금지어를 포함하고 있는지 체크.
									*	@param	strUserChat은 체크하는 동안 내용이 변경 될 수 있기 때문에 복사본임.
									*/
	bool							CheckFobiddenWords( CString strUserChat );

									/*!
									*	@brief	금지어를 포함하고 있으면 Block.
									*	@param	strUserChat에 금지어가 포함되어 있으면 CTChatFilter::strReplace 로 대체됨.
									*/
	void							ConvertString( CString& strUserChat, bool bCheckHeader = false );

public :

	bool							UsedFobiddenAtFirst() const;
	bool							UsedFobiddenWords() const;
	void							SetFilterFile( CTChatFilterFile* pFile );
	CTChatFilterFile*				GetFilterFile();
};

#endif