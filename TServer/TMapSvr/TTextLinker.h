#pragma once

class CTTextLinker
{
public:
	static const CString	LINK_DEF_TOK;

	static const CHAR		LINK_STR_START;
	static const CHAR		LINK_STR_END;
	static const DWORD		LINK_DATA_POOL_SIZE;
	static const DWORD		LINK_QUEST_COLOR;

protected:
	DWORD			m_nTextLinkDataPos;
	CTTextLinkData*	m_pTextLinkDataPool;

public:
	/// 새로운 텍스트 링크 데이타 인스턴스를 얻는다.
	DWORD NewTextLinkData(CTTextLinkData** pOut);
	/// 주어진 아이디의 텍스트 링크 데이타를 얻는다.
	CTTextLinkData* GetTextLinkData(DWORD nPos) const;

public:

	/// 주어진 형식의 문자열 포맷에 맞추어 링크 텍스트를 네트워크 전송 가능한 형태로 만든다.
	CString MakeItemNetText(const CString& strFormat, const VDWORD& vItemID);

	CString BuildNetString( const CString& strHeader, const CString& strBody);
	CString AppendNetString( const CString& strMSG1, const CString& strMSG2);
	/// 주어진 문자열을 개행문자 단위로 나눈다.
	CString SplitTextByCrLf(const CString& strMSG, const CString& strTOK, INT& nPos, BOOL bTrimCrLf=TRUE);

protected:
	CTTextLinker();
	~CTTextLinker();

public:
	static CTTextLinker* GetInstance();
};
