// PlatformUsage.cpp: implementation of the CPlatformUsage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlatformUsage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlatformUsage::CPlatformUsage(BYTE bMachineID)
{
	m_bOpen = FALSE;
	m_hQuery = NULL;
	m_bMachineID = bMachineID;
	for( INT i = 0; i < NUM_PERFORMANCE_COUNTER; i++) 
	{
		m_hCounter[i] = 0;
		m_fValue[i] = 0.0;
	}

	m_szPerformancePath[0] = "\\Processor(_Total)\\% Processor Time"; // 238 Processor
	m_szPerformancePath[1] = "\\Memory\\% Committed Bytes In Use"; // 4 Memory
	m_szPerformancePath[2] = "Need a network reg string"; // 510 Network Interface


}

CPlatformUsage::~CPlatformUsage()
{
	Close();
}

BOOL CPlatformUsage::IsOpen()
{
	return m_bOpen;
}

void CPlatformUsage::SetNETREG(LPCTSTR strNet)
{	
	m_szPerformancePath[COUNTER_NET] = strNet;
}

BOOL CPlatformUsage::Open(LPCTSTR strMachine)
{
	m_strMachine = strMachine;

	TCHAR szPath[MAX_PATH_BUFFER];
	ZeroMemory(szPath, MAX_PATH_BUFFER);

	PDH_STATUS pdhStatus = PdhOpenQuery (0, 0, &m_hQuery);
	if(pdhStatus != ERROR_SUCCESS) 
		return FALSE;

	for( int i = 0; i < NUM_PERFORMANCE_COUNTER; i++)
	{		
		sprintf(szPath, "\\\\%s%s", strMachine, m_szPerformancePath[i]);
		pdhStatus = PdhAddCounter(m_hQuery, szPath, 0, &m_hCounter[i]);
		if(pdhStatus != ERROR_SUCCESS)
		{
			Close();
			return FALSE;
		}
	}

	m_bOpen = TRUE;
	pdhStatus = PdhCollectQueryData (m_hQuery); 
	return TRUE;
};

void CPlatformUsage::Close()
{
	m_bOpen = FALSE;
	for( INT i = 0; i < NUM_PERFORMANCE_COUNTER; i++)
	{
		PdhRemoveCounter(m_hCounter[i]);
		m_hCounter[i] = 0;
		m_fValue[i] = 0.0;
	}
	PdhCloseQuery(m_hQuery);
	m_hQuery = NULL;
}

PDH_STATUS CPlatformUsage::QueryPlatformUsage()
{
	PDH_STATUS pdhStatus = PdhCollectQueryData (m_hQuery); 
	if(pdhStatus!= ERROR_SUCCESS)
	{
		Close();
		return pdhStatus;
	}

	DWORD ctrType;
	for( INT i = 0; i < NUM_PERFORMANCE_COUNTER; i++)
	{
		pdhStatus = PdhGetFormattedCounterValue (
			m_hCounter[i], PDH_FMT_DOUBLE, &ctrType, &m_fmtValue[i]);
		if(pdhStatus!= ERROR_SUCCESS)
		{
			Close();
			return pdhStatus;
		}
		m_fValue[i] = (FLOAT)m_fmtValue[i].doubleValue;
	}	
	return ERROR_SUCCESS;
}

LPCSTR CPlatformUsage::GetMachineName()
{
	if(!IsOpen())
		return NULL;

	return m_strMachine;
}

DWORD CPlatformUsage::GetCPU()
{
	if(!IsOpen())
		return 0;

	return (DWORD)m_fValue[0];
}

DWORD CPlatformUsage::GetMEM()
{
	if(!IsOpen())
		return 0;

	return (DWORD)m_fValue[1];
}

float CPlatformUsage::GetNET(FLOAT bps)
{
	//1gbit/sec x * 8 / 10485760.0 
	//100mbit/sec x * 8 / 1048576.0
	//1mbit/sec x * 8 / 10485.76

	if(!IsOpen())
		return 0.0;

	return (float)(m_fmtValue[COUNTER_NET].doubleValue * 8.0 / (10485.76 * bps));
}