// TMacroMachineFile.cpp : CTMacroMachineDoc 클래스의 구현
//

#include "stdafx.h"
#include "TMacroMachine.h"

#include "TMacroMachineDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL CTMacroMachineDoc::LoadProtocol(CString strFile)
{	
	CFile Rfile;
	if( !Rfile.Open(strFile, CFile::modeRead) )
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

		DWORD dwValue = SplitProtocol(temp);
		if(!dwValue)
			return FALSE;

		str = str.Mid(pos);

        SplitType(dwValue, str);
	}
	delete data;

	if(!m_mapProtocol.size())
		return FALSE;

	return TRUE;
}

DWORD CTMacroMachineDoc::SplitProtocol(CString strP)
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

	DWORD dwValue;

	if(strBase == STR_SM_BASE)
		dwValue = SM_BASE;
	else if(strBase == STR_MW_BASE)
		dwValue = MW_BASE;
	else if(strBase == STR_DM_BASE)
		dwValue = DM_BASE;
	else if(strBase == STR_CS_LOGIN)
		dwValue = CS_LOGIN;
	else if(strBase == STR_CS_MAP)
		dwValue = CS_MAP;
	else if(strBase == STR_CT_CONTROL)
		dwValue = CT_CONTROL;
	else if(strBase == STR_CT_PATCH)
		dwValue = CT_PATCH;

	char *p = strValue.GetBuffer(0);
	char *endp;

	// 문자열 16진수를 숫자로 변환
	dwValue += DWORD(strtoul(p, &endp, 16));

	m_mapProtocol.insert(MAPTPROTOCOL::value_type(dwValue, strProtocol));

	return dwValue;
}

void CTMacroMachineDoc::SplitType(DWORD dwValue, CString strT)
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
	   if( strstr(token, _T("__int64")) || strstr(token, _T("__time64_t")) )
		   v.push_back(TYPE_INT64);
	   if( strstr(token, _T("SOCKADDR")) )
		   v.push_back(TYPE_SOCKADDR);
	   if( strstr(token, _T("{")) )
		   v.push_back(TYPE_LOOPSTART);
	   if( strstr(token, _T("}")) )
		   v.push_back(TYPE_LOOPEND);

       token = strtok( NULL, _T("\n") );
	}

	m_mapType.insert(MAPTPROTOCOLTYPE::value_type(dwValue, v));
}

BOOL CTMacroMachineDoc::LoadMacro(CString strFile)
{
	while(m_qTMACRO.size())
		m_qTMACRO.pop();

	CFile file(strFile, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	Serialize(ar);
	ar.Close();
	file.Close();

	if(m_qTMACRO.empty())
	{
		m_mapTVALUE.clear();
		return FALSE;
	}

	QTMACRO tQMacro = m_qTMACRO;

	while(tQMacro.size())
	{
		TMACRO tMacro = tQMacro.front();
		TRACE(_T("%d %s %d\n"), tMacro.m_dwID, tMacro.m_strName, tMacro.m_bLoop);
		while(tMacro.m_qProtocol.size())
		{
			TQPROTOCOL tProtocol = tMacro.m_qProtocol.front();
			
			MAPTPROTOCOL::iterator find = m_mapProtocol.find(tProtocol.m_dwID);
			if(find!=m_mapProtocol.end())
				TRACE(_T("<%s> \n"), (*find).second);

			for(int i = 0; i < (int)tProtocol.m_vTChild.size(); i++)
			{
				find = m_mapProtocol.find(tProtocol.m_vTChild[i].m_dwID );
				if(find!=m_mapProtocol.end())
					TRACE(_T("\t -- <%s>\n"), (*find).second);
				else
					TRACE(_T("\n"));
			}

			tMacro.m_qProtocol.pop();
		}
		tQMacro.pop();
	}

	return TRUE;
}

// CTMacroMachineDoc serialization

void CTMacroMachineDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		CString str;
		ar >> str;
		if(str != STR_TMACROMACHINE)
			return;

		INT cnt;
		ar >> cnt;

		for(INT i=0; i<cnt; i++)
		{
			TVALUE tValue;
			ar >> tValue.m_dwID >> tValue.m_strName >> tValue.m_bType >> tValue.m_bValueType >> tValue.m_strValue;
			m_mapTVALUE.insert(MAPTVALUE::value_type(tValue.m_dwID, tValue));
		}

		ar >> cnt;

		for(i=0; i<cnt; i++)
		{
			DWORD dwTemp;
			WORD wTemp;
			BYTE bTemp;
			VTQPROTOCOL vTemp;

			TMACRO tMacro;
			ar >> tMacro.m_dwID >> tMacro.m_strName >> tMacro.m_bLoop;

			INT cntPt;
			ar >> cntPt;
			for(INT j=0; j<cntPt; j++)
			{
				TQPROTOCOL tProtocol;
				ar >> dwTemp >> tProtocol.m_dwID >> dwTemp >> bTemp >> tProtocol.m_bLoop >> tProtocol.m_wSleep;

				INT cntType;
				ar >> cntType;
				for(INT k=0; k<cntType; k++)
				{
					TPTVALUE tValue;
					ar >> tValue.m_bValueType >> tValue.m_dwValueID;
					tProtocol.m_vType.push_back(tValue);
				}

				vTemp.push_back(tProtocol);

				LPTPROTOCOL pProtocol = new TPROTOCOL;
				pProtocol->m_bLoop = tProtocol.m_bLoop;
				pProtocol->m_bSelf = 0;
				pProtocol->m_dwID = tProtocol.m_dwID;
				pProtocol->m_dwIndex = j;
				pProtocol->m_dwParent = 0;
				pProtocol->m_vType = tProtocol.m_vType;
				pProtocol->m_wSleep	= tProtocol.m_wSleep;

				m_mapTProtocol.insert(MAPTPROTOCOLTEMP::value_type( pProtocol->m_dwID, pProtocol ) );
			}

			ar >> cntPt;
			for(j=0; j<cntPt; j++)
			{
				DWORD dwParent;
				TCHILD tChild;
				ar >> dwTemp >> tChild.m_dwID >> dwParent >> tChild.m_bSelf >> bTemp >> wTemp;

				INT cntType;
				ar >> cntType;
				for(INT k=0; k<cntType; k++)
				{
					TPTVALUE tValue;
					ar >> tValue.m_bValueType >> tValue.m_dwValueID;
					tChild.m_vType.push_back(tValue);
				}

				DWORD m = 0;
				DWORD dwChildIndex = 0;
				for( m=0; m < vTemp.size(); m++)
				{
					if(vTemp[m].m_dwID == dwParent)
					{
						vTemp[m].m_vTChild.push_back(tChild);
						dwChildIndex = (DWORD)vTemp[m].m_vTChild.size();
						break;
					}
				}

				LPTPROTOCOL pProtocol = new TPROTOCOL;
				pProtocol->m_bLoop = 0;
				pProtocol->m_bSelf = tChild.m_bSelf;
				pProtocol->m_dwID = tChild.m_dwID;
				pProtocol->m_dwIndex = m * 10 + dwChildIndex ;
				pProtocol->m_dwParent = dwParent;
				pProtocol->m_vType = tChild.m_vType;
				pProtocol->m_wSleep	= 0;

				m_mapTProtocol.insert(MAPTPROTOCOLTEMP::value_type( pProtocol->m_dwID, pProtocol ) );
			}


			VDWORD vDWORDTmp;
			for(DWORD dwC = 0; dwC < vTemp.size(); dwC++ )
			{
                DWORD dwChildCount = (DWORD)vTemp[dwC].m_vTChild.size();
				
				if(dwChildCount > 0 )
				{
                    vTemp[dwC].m_vTChild[dwChildCount-1].m_bEndChild = TRUE;
					vDWORDTmp.push_back(vTemp[dwC].m_vTChild[dwChildCount-1].m_dwID);
				}
			}

			for(j=0; j<(INT)vTemp.size(); j++)
				tMacro.m_qProtocol.push(vTemp[j]);

			m_qTMACRO.push(tMacro);

			VDWORD::iterator itDW;
			for(itDW = vDWORDTmp.begin(); itDW != vDWORDTmp.end(); itDW++)
			{
				MAPTPROTOCOLTEMP::iterator itPT = m_mapTProtocol.find( (*itDW) );
				if( itPT != m_mapTProtocol.end() )
					(*itPT).second->m_bEndChild = TRUE;
			}
		}
	}
}