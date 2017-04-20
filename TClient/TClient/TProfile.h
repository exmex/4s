#pragma once

class CProfiler
{
public:
	double m_dAccumulatedSec;
	UINT m_uNumberOfTimes;

public:
	CProfiler()
	{
		m_dAccumulatedSec = 0;
		m_uNumberOfTimes = 0;
	}

	~CProfiler()
	{
	}
};

class CProfileSample
{
public:
	inline CProfileSample( CProfiler& p, BOOL bProfile=TRUE )
		: m_Profiler(p), m_bProfile(bProfile)
	{
		if( m_bStartProfiling && m_bProfile )
			QueryPerformanceCounter( &m_liStartTime );
	}

	inline ~CProfileSample()
	{
		if( m_bStartProfiling && m_bProfile )
		{
			LARGE_INTEGER liEnd;
			QueryPerformanceCounter(&liEnd);

			++m_Profiler.m_uNumberOfTimes;
			m_Profiler.m_dAccumulatedSec +=
				( ((double)liEnd.QuadPart) / m_liFrequency.QuadPart) - ( ((double)m_liStartTime.QuadPart) / m_liFrequency.QuadPart);
		}
	}

public:
	CProfiler& m_Profiler;
	LARGE_INTEGER m_liStartTime;
	BOOL m_bProfile;

public:
	static LARGE_INTEGER m_liFrequency;
	static BOOL m_bStartProfiling;

	static void InitFrequency()
	{
		QueryPerformanceFrequency(&m_liFrequency);
	}
	
	static void StartProfiling()
	{
		m_bStartProfiling = TRUE;
	}
};