// PlatformUsage.h: interface for the CPlatformUsage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSPERFORMANCE_H__DF6306EE_DFDF_47AF_A804_4FA9182142EC__INCLUDED_)
#define AFX_SYSPERFORMANCE_H__DF6306EE_DFDF_47AF_A804_4FA9182142EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Pdh.h>
#pragma comment(lib, "pdh.lib")

#define NUM_PERFORMANCE_COUNTER			3
#define COUNTER_CPU						0
#define COUNTER_MEM						1
#define COUNTER_NET						2

#define MAX_PATH_BUFFER					1024

class CPlatformUsage  
{
public:	
	CPlatformUsage(BYTE bMachineID);
	virtual ~CPlatformUsage();

	void Close();
	PDH_STATUS QueryPlatformUsage();
	BOOL Open(LPCTSTR strMachine);
	BOOL IsOpen();
	LPCSTR GetMachineName();
	DWORD GetCPU();
	DWORD GetMEM();
	void SetNETREG(LPCTSTR strNet);
	float GetNET(FLOAT bps);

	BYTE					m_bMachineID;
	BOOL					m_bOpen;
	HQUERY					m_hQuery;
	FLOAT					m_fValue[NUM_PERFORMANCE_COUNTER];
	HCOUNTER				m_hCounter[NUM_PERFORMANCE_COUNTER];
	PDH_FMT_COUNTERVALUE	m_fmtValue[NUM_PERFORMANCE_COUNTER];
	LPCTSTR					m_strMachine;
	CString					m_szPerformancePath[NUM_PERFORMANCE_COUNTER];
};

#endif // !defined(AFX_SYSPERFORMANCE_H__DF6306EE_DFDF_47AF_A804_4FA9182142EC__INCLUDED_)
