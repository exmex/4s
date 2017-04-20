#pragma once

class CTTextLinkData
{
public:
	static const CHAR REAL_REP_MARK;
	static const CHAR NULL_REP_MARK;

protected:
	UINT			m_nPos;
	CString			m_vData;

	LPTQUEST		m_pLastUnpackQuest;
	CTChatItem*	m_pLastUnpackItem;
	
public:
	CTTextLinkData();
	~CTTextLinkData();

	template <typename T>
	CTTextLinkData& operator << (const T& in)
	{
		BYTE* ptr = (BYTE*)(&in);
		for(UINT i=0; i<sizeof(T); ++i, ++ptr)
		{
			BYTE b = *ptr;
			if( b != NULL )
			{
				m_vData.AppendChar(REAL_REP_MARK);
				m_vData.AppendChar(b);
			}
			else
				m_vData.AppendChar(NULL_REP_MARK);
		}

		return *this;
	}

	template <typename T>
	CTTextLinkData& operator >> (T& out)
	{
		BYTE* ptr = (BYTE*)(&out);
		for(UINT i=0; i<sizeof(T); ++i, ++ptr)
		{
			BYTE bRepMark = m_vData.GetAt(m_nPos++);
			if( bRepMark == NULL_REP_MARK )
				*ptr = NULL;
			else
				*ptr = m_vData.GetAt(m_nPos++);
		}

		return *this;
	}

	TTEXT_LINK_TYPE GetType();

	LPTQUEST UnPackQuest();
	CTChatItem* UnPackItem();

	LPTQUEST GetLastUnkpackQuest() const		{ return m_pLastUnpackQuest; }
	CTChatItem* GetLastUnkpackItem() const	{ return m_pLastUnpackItem; }
	void ResetPosition()						{ m_nPos = 0; }
	BOOL IsEndOfPosition() const				{ return (m_vData.GetLength() <= m_nPos); }
	const CString& ToStr() const				{ return m_vData; }
	void FromStr(const CString& str)			{ m_vData = str; }
};

CString MakeNetToLinkText( const CString& strNetMsg);