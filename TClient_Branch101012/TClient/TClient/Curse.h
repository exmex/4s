#pragma once

/**
	문장에서 비속어를 찾기 위한 클래스이다.
	
	1. 검색에 binary search 를 사용함.
	2. 비속어 리스트의 단어중 가장 긴 단어의 문자길이가 길어질 수록 검색시간이 증가함.
	3. CONVERT_STRING 에 비속어 검색시 대체하고자 하는 문자를 지정 할 수 있음.
*/
class CCurse
{
public:
	static CString CONVERT_STRING;
	static CHAR CONVERT_CHAR;

private:
	static int				m_nMaxLen;
	static vector<CString>	m_vStrCurse;	

private:
	static DWORD ReadString(CHAR* pDATA, CString& outSTR);

public:
	/**
		텍스트 형식의 리소스로 부터 비속어 데이타를 로드해온다.
		비속어 검사전 반드시 호출해 주어야 한다.
	*/
	static void Load(HINSTANCE hInstance, DWORD dwResID, CString strResGrp);	

	/**
		주어진 문장에 비속어가 있는지 검사한다.
	*/
	static bool IsCurse(const CString& str);					

	/**
		주어진 문장에서 비속어를 찾아 지정된 문자열로 치환한다.
	*/
	static const CString& ConvertString( const CString& str, BYTE bCheckHeader);

	/**
		로드된 모든 데이터를 메모리에서 제거한다.
	*/
	static void Release() { m_vStrCurse.clear(); m_nMaxLen = 0; }
};
