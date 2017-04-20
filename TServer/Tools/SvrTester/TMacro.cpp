#include "stdafx.h"
#include ".\tmacro.h"

///////////////////////////////////////////////////////////////////////////////////////
CTMacro::CTMacro(CString sName, CString sParent)
{
	m_bEnable = FALSE;
	m_bLoop = FALSE;
	SetMacroName(sName, sParent);
	m_vParam.clear();
	m_vPInfo.clear();
	ClearSendRecvInfo();

	TRACE(_T("New : %d %s %s %d %d\n"), m_bEnable, sName, sParent, m_vParam.size(), m_vPInfo.size());
}
CTMacro::CTMacro(BOOL bEnable, BOOL bLoop, CString sName, CString sParent, VECTORPARAM vParam, VECTORPINFO vPInfo)
{
	m_bEnable = bEnable;
	m_bLoop = bLoop;
	SetMacroName(sName, sParent);
	m_vParam = vParam;
	m_vPInfo = vPInfo;
	ClearSendRecvInfo();

	TRACE(_T("New : %d %s %s %d %d\n"), bEnable, sName, sParent, m_vParam.size(), m_vPInfo.size());
}
CTMacro::~CTMacro(void)
{
	m_bEnable = FALSE;
	m_bLoop = FALSE;
	m_vParam.clear();
	m_vPInfo.clear();
	ClearSendRecvInfo();
	TRACE(_T("Delete : %s %s %d %d\n"), m_sName, m_sParent, m_vParam.size(), m_vPInfo.size());
}
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Macro Enable Flag
void CTMacro::SetMacroEnable(BOOL flag)
{
	m_bEnable = flag;
}
BOOL CTMacro::GetMacroEnable()
{
	return m_bEnable;
}
// Macro Valid Check
BOOL CTMacro::ValidCheck(MAPPROTOCOL mapProtocol)
{
	if( m_vPInfo.size() == 0 )
	{
		m_bEnable = FALSE;
		return FALSE;
	}

	for(int i = 0; i < (int)m_vPInfo.size(); i++)
	{
		DWORD pID = m_vPInfo[i].dwPID;
		CString sName = m_vPInfo[i].strName;

		MAPPROTOCOL::iterator finder = mapProtocol.find(pID);
		if( finder == mapProtocol.end() )
		{
			m_bEnable = FALSE;
			return FALSE;
		}

		if( strstr(sName, _T("_ACK")) )
		{
			BOOL flag = FALSE;
			VECTORPARAM vP = m_vPInfo[i].vParam;
			for(int k = 0; k < (int)vP.size(); k++)
			{
				if( vP[k].strName == _T("<Self>") )
				{
					if( !m_vPInfo[i].bSelf )
					{
						m_bEnable = FALSE;
						return FALSE;
					}
				}					
			}
			
			continue;
		}

		VECTORPARAM vParam = m_vPInfo[i].vParam;
		for(int j = 0; j < (int)vParam.size(); j++)
		{
			if( vParam[j].strName == _T("Default") || vParam[j].strName == _T("<Self>") )
			{
				m_bEnable = FALSE;
				return FALSE;
			}
		}		
	}

	m_bEnable = TRUE;
    return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Macro Enable Flag
void CTMacro::SetMacroLoop(BOOL flag)
{
	m_bLoop = flag;
}
BOOL CTMacro::GetMacroLoop()
{
	return m_bLoop;
}
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Macro
void CTMacro::SetMacroName(CString sName, CString sParent)
{
	m_sName = sName;
	m_sParent = sParent;
}
void CTMacro::UpdateMacroName(CString sName)
{
	m_sName = sName;
}
void CTMacro::UpdateMacroParent(CString sParent)
{
	m_sParent = sParent;
}
CString CTMacro::GetMacroName()
{
	return m_sName;
}
CString CTMacro::GetMacroParent()
{
	return m_sParent;
}
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Macro Paramater
BOOL CTMacro::AddMacroParam(STRUCTPARAM sParam)
{
	CString sName = sParam.strName;
	CString sType = sParam.strType;	

	for(int i = 0; i < (int)m_vParam.size(); i++)
	{
		if( m_vParam[i].strName == sName )
			return FALSE;
	}

	m_vParam.push_back(sParam);
	return TRUE;	
}
int CTMacro::SearchMacroParam(CString sName, CString sType)
{
	for(int i = 0; i < (int)m_vParam.size(); i++)
	{
		if( m_vParam[i].strName == sName &&	m_vParam[i].strType == sType )		
			return i;
	}

	return -1;
}
void CTMacro::DelMacroParam(CString sName, CString sType)
{
	int i = SearchMacroParam(sName, sType);
	if( i < 0 )
		return;	
	
	m_vParam.erase(m_vParam.begin() + i);	
}
void CTMacro::DelAllMacroParam()
{
	m_vParam.clear();
}
void CTMacro::SetMacroParam(VECTORPARAM vParam)
{
	m_vParam.clear();
	m_vParam = vParam;
}
VECTORPARAM CTMacro::GetMacroParam()
{		
	return m_vParam;
}
BOOL CTMacro::IsEmptyParam()
{
	if( m_vParam.size() == 0 )
		return TRUE;

	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Macro Protocol Info Vector
BOOL CTMacro::AddMacroPInfo(STRUCTPINFO sPInfo)
{
	CString sName = sPInfo.strName;

	if( SearchMacroPInfo(sName) >= 0 )
		return FALSE;
	
	m_vPInfo.push_back(sPInfo);
	return TRUE;
}
int CTMacro::SearchMacroPInfo(CString sName)
{
	for(int i = 0; i < (int)m_vPInfo.size(); i++)
	{
		if( m_vPInfo[i].strName == sName )
			return i;
	}   

	return -1;
}
void CTMacro::DelMacroPInfo(CString sName)
{
	int i = SearchMacroPInfo(sName);
	if( i < 0 )
		return;
	
	m_vPInfo.erase(m_vPInfo.begin() + i);

    for(i = 0; i < (int)m_vPInfo.size(); i++)
	{
		if( m_vPInfo[i].strParent == sName )
			m_vPInfo.erase(m_vPInfo.begin() + i);			
	}
}
void CTMacro::DelAllMacroPInfo()
{
	m_vPInfo.clear();
}
VECTORPINFO CTMacro::GetMacroPInfo()
{
	return m_vPInfo;
}
BOOL CTMacro::IsEmptyPInfo()
{
	if( m_vPInfo.size() == 0 )
		return TRUE;

	return FALSE;
}

// Macro Protocol Info Struct Set / Get / Name Update
void CTMacro::SetMacroPInfo(CString sName, STRUCTPINFO sPInfo)
{
	int i = SearchMacroPInfo(sName);
	if( i < 0 )
		return;

	m_vPInfo[i].dwPID = sPInfo.dwPID;
	m_vPInfo[i].strName = sPInfo.strName;
	m_vPInfo[i].strParent = sPInfo.strParent;
	m_vPInfo[i].bSleep = sPInfo.bSleep;
	m_vPInfo[i].sleep = sPInfo.sleep;
	m_vPInfo[i].bSelf = sPInfo.bSelf;
	m_vPInfo[i].vParam.clear();
	m_vPInfo[i].vParam = sPInfo.vParam;
}
STRUCTPINFO CTMacro::GetMacroPInfo(CString sName)
{
	int i = SearchMacroPInfo(sName);
	if( i < 0 )
	{
		STRUCTPINFO temp;
		return temp;
	}

	return m_vPInfo[i];
}
BOOL CTMacro::UpdatePInfoName(CString sOld, CString sName, DWORD pID)
{
	int i = SearchMacroPInfo(sName);
	if( i >= 0 )
		return FALSE;

	i = SearchMacroPInfo(sOld);
	if( i < 0 )
		return FALSE;

	m_vPInfo[i].strName = sName;
	m_vPInfo[i].dwPID = pID;
	for(int j = 0; j < (int)m_vPInfo.size(); j++)
	{
		if( m_vPInfo[j].strParent == sOld )
			m_vPInfo[j].strParent = sName;
	}

	return TRUE;
}
// Macro Protocol Info Sleep Set / Clear
void CTMacro::SetPInfoSleep(CString sName, int sleep)
{
	int i = SearchMacroPInfo(sName);
	if( i < 0 )
		return;

	m_vPInfo[i].bSleep = TRUE;
	m_vPInfo[i].sleep = sleep;
}
void CTMacro::ClearPInfoSleep(CString sName)
{
	int i = SearchMacroPInfo(sName);
	if( i < 0 )
		return;

	m_vPInfo[i].bSleep = FALSE;
	m_vPInfo[i].sleep = 0;
}
// Macro Protocol Info Self Set / Get / Clear
void CTMacro::SetPInfoSelf(CString sName, BOOL flag, VECTORPARAM vParam)
{
	int i = SearchMacroPInfo(sName);
	if( i < 0 )
		return;

	m_vPInfo[i].bSelf = flag;
	m_vPInfo[i].vParam = vParam;	
}
int CTMacro::GetPInfoSelf(CString sName)
{
    int i = SearchMacroPInfo(sName);
	if( i < 0 )
		return -1;

	if( m_vPInfo[i].bSelf )
		return i;

	return -1;
}
void CTMacro::ClearPInfoSelf(CString sName)
{
    int i = SearchMacroPInfo(sName);
	if( i < 0 )
		return;

	m_vPInfo[i].bSelf = FALSE;

	VECTORPARAM vParam = m_vPInfo[i].vParam;
	for(int j = 0; j < (int)vParam.size(); j++)
	{
		if( vParam[j].strName == _T("<Self>") )
		{
			m_vPInfo[i].vParam[j].strName = _T("Default");
			break;
		}
	}
}
// Macro Protocol Info Paramater Set / Clear
void CTMacro::SetPInfoParam(CString sName, VECTORPARAM vParam)
{
	int i = SearchMacroPInfo(sName);
	if( i < 0 )
		return;

	m_vPInfo[i].vParam.clear();
	m_vPInfo[i].vParam = vParam;	
}
void CTMacro::ClearPInfoParam(CString sName)
{
	int i = SearchMacroPInfo(sName);
	if( i < 0 )
		return;

	m_vPInfo[i].vParam.clear();
}
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Send / Receive Vector Set / Get / Clear
void CTMacro::SetSendRecvInfo()
{
	ClearSendRecvInfo();

	for(int i = 0; i < (int)m_vPInfo.size(); i++)
	{
		if( strstr(m_vPInfo[i].strName, _T("_ACK")) )
		{
			STRUCTPINFO sPInfo = m_vPInfo[i];
			m_vRecv.push_back(sPInfo);

			SetSelfInfo(sPInfo);
		}

		if( m_vPInfo[i].strParent == _T("") )
		{
			if( strstr(m_vPInfo[i].strName, _T("_REQ")) )
			{
				STRUCTPINFO sPInfo = m_vPInfo[i];
				m_vSend.push_back(sPInfo);
			}
			SetSendRecvInfo(m_vPInfo[i].strName);
		}
	}
}
void CTMacro::SetSendRecvInfo(CString sName)
{
	for(int i = 0; i < (int)m_vPInfo.size(); i++)
	{
		if( m_vPInfo[i].strParent == sName )
		{
			if( strstr(m_vPInfo[i].strName, _T("_REQ")) )
			{
				STRUCTPINFO sPInfo = m_vPInfo[i];
				m_vSend.push_back(sPInfo);
			}
			SetSendRecvInfo(m_vPInfo[i].strName);
		}
	}
}
VECTORSRINFO CTMacro::GetSendInfo()
{
	return m_vSend;
}
VECTORSRINFO CTMacro::GetRecvInfo()
{
	return m_vRecv;
}
void CTMacro::ClearSendRecvInfo()
{	
	m_vSend.clear();
	m_vRecv.clear();
	ClearSelfInfo();
}

// Get Current Send
STRUCTPINFO CTMacro::GetCurSendInfo(int i)
{
	if( i >= (int)m_vSend.size() )
	{
		STRUCTPINFO temp;
		return temp;
	}

	return m_vSend[i];
}
// Is Send Last Cnt
BOOL CTMacro::IsSendLast(int i)
{
	if( m_vSend.size() == i )
		return TRUE;

    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Session Macro Self Info Vector Set / Get / Clear
void CTMacro::SetSelfInfo(STRUCTPINFO pInfo)
{
	if( !pInfo.bSelf )
		return;

	STRUCTSELFINFO sInfo;
	sInfo.dwPID = pInfo.dwPID;
	sInfo.strName = pInfo.strName;

	VECTORPARAM vParam = pInfo.vParam;
	for(int i = 0; i < (int)vParam.size(); i++)
	{
		if( vParam[i].strName == _T("<Self>") )
			break;		
	}

	if( i == (int)vParam.size() )
		return;

	sInfo.pos = i;

	m_vSelf.push_back(sInfo);
}
VECTORSELFINFO CTMacro::GetSelfInfo()
{
	return m_vSelf;
}
void CTMacro::ClearSelfInfo()
{
	m_vSelf.clear();
}
///////////////////////////////////////////////////////////////////////////////////////