// ProtocolLoad.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "SvrTester.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

/////////////////////////////////////////////////////////////////////////////////////
// string -> byte
DWORD CMainFrame::strTobyte(CString str)
{	
	char *p = str.GetBuffer(0);
	char *endp;

	int i = (int)strtoul(p, &endp, 16); // 문자열 16진수를 숫자로 변환

	return i;
}

// Protocol Base
BYTE CMainFrame::LoadProtocolBase()
{
	CString fileName;

	fileName = _T(".\\ProtocolBase.h");
	CFile Rfile;
	if( !Rfile.Open(fileName, CFile::modeRead) )
		return FALSE;
	
	UINT FileLength = (UINT)Rfile.GetLength();
	
	char* data = new char[FileLength];
	Rfile.Read(data,FileLength);
	Rfile.Close();

	CString str, temp;
	str = data;
	int pos;	

	while(1)
	{
		if( str.Find(_T("#define")) < 0 )
			break;
		pos = str.Find(_T("#define"));
		str = str.Mid(pos+7);

		pos = str.Find(_T(")"));
		temp = str.Left(pos+1);

		SplitBase(temp);

		str = str.Mid(pos);		
	}	
	delete data;	

	return TRUE;
}
// Protocol Base Split
void CMainFrame::SplitBase(CString str)
{
	int pos;

	CString strProtocol = str;
	CString strValue;

	pos = strProtocol.Find(_T("("));

	strProtocol = strProtocol.Left(pos);
	strProtocol.Trim(_T("\t "));
	
	strValue = str.Mid(pos+1);
	pos = strValue.Find(_T("0"));
	strValue = strValue.Mid(pos);
	pos = strValue.Find(_T(")"));
	strValue = strValue.Left(pos);

	DWORD value = strTobyte(strValue);

	m_mapBase.insert(MAPPROTOCOL::value_type(value, strProtocol));
}

// Protocol
BYTE CMainFrame::LoadProtocol(CString fileName)
{	
	CFile Rfile;
	if( !Rfile.Open(fileName, CFile::modeRead) )
		return FALSE;

	UINT FileLength = (UINT)Rfile.GetLength();
	
	char* data = new char[FileLength];
	Rfile.Read(data,FileLength);
	Rfile.Close();

	CString str, temp;
	str = data;
	int pos;	

	while(1)
	{
		if( str.Find(_T("#define")) < 0 )
			break;
		pos = str.Find(_T("#define"));
		str = str.Mid(pos+7);

		pos = str.Find(_T(")"));
		temp = str.Left(pos+1);

		DWORD value = SplitProtocol(temp);

		str = str.Mid(pos);	

        SplitType(value, str);		
	}	
	delete data;	

	return TRUE;
}
// Protocol Split
DWORD CMainFrame::SplitProtocol(CString strP)
{
	int pos;

	CString strProtocol = strP;
	CString strValue;
	CString strBase;

	pos = strProtocol.Find(_T("("));

	strProtocol = strProtocol.Left(pos);
	strProtocol.Trim(_T("\t "));
	
	strValue = strP.Mid(pos+1);
	pos = strValue.Find(_T(" "));
	strBase = strValue.Left(pos);

	pos = strValue.Find(_T("0"));
	strValue = strValue.Mid(pos);
	pos = strValue.Find(_T(")"));
	strValue = strValue.Left(pos);

	DWORD value;
	MAPPROTOCOL::iterator it;
	for(it = m_mapBase.begin(); it != m_mapBase.end(); it++)
	{
		CString str = (*it).second;
		
		if( (*it).second == strBase )
		{
            value = (*it).first + strTobyte(strValue);
			break;
		}
	}

	m_mapProtocol.insert(MAPPROTOCOL::value_type(value, strProtocol));

	return value;
}

// Protocol Type Split
void CMainFrame::SplitType(DWORD value, CString strT)
{
	CString temp;
	VECTORTYPE v;

	int pos = strT.Find(_T("#define"));
	if( pos == -1 )
		temp = strT;
	else
		temp = strT.Left(pos);

	TCHAR* strType = temp.GetBuffer(0);	

	TCHAR* token;
	token = strtok( strType, _T("\n") ); 

    while( token != NULL ) 
	{
       if( strstr(token, _T("BYTE")) )
		   v.push_back(TYPE_BYTE);
	   if( strstr(token, _T("WORD")) )
	   {
		   if( strstr(token, _T("DWORD")) )
			   v.push_back(TYPE_DWORD);
		   else
			   v.push_back(TYPE_WORD);
	   }	   
	   if( strstr(token, _T("FLOAT")) )
		   v.push_back(TYPE_FLOAT);
	   if( strstr(token, _T("STRING")) || strstr(token, _T("CString")) )
		   v.push_back(TYPE_STRING);
	   if( strstr(token, _T("__int64")) )
		   v.push_back(TYPE_INT64);
	   if( strstr(token, _T("SOCKADDR")) )
		   v.push_back(TYPE_SOCKADDR);
	   if( strstr(token, _T("{")) )
		   v.push_back(TYPE_LOOPSTART);
	   if( strstr(token, _T("}")) )
		   v.push_back(TYPE_LOOPEND);

       token = strtok( NULL, _T("\n") ); 
	}

	m_mapType.insert(MAPPROTOCOLTYPE::value_type(value, v));
}
/////////////////////////////////////////////////////////////////////////////////////