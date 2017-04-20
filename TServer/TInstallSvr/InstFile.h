#pragma once

#define CHECK_SIZE	1048576
class CInstFile
{	
public:
	CInstFile()
	{
		InitializeCriticalSectionAndSpinCount(&m_csInstFile, 4000);
	}
	~CInstFile()
	{
		DeleteCriticalSection(&m_csInstFile);
		m_mapIpAddr.clear();
	}

private:
	CRITICAL_SECTION m_csInstFile;		
	map<DWORD,WORD> m_mapIpAddr;	//	Ip, Country

public:
	DWORD	m_dwFileID;
	DWORD	m_dwSize;
	CString m_strName;
	CString	m_strCheckSum;

	inline void InsertIp(DWORD dwIpAddr, WORD wCountry)	{SMART_LOCKCS(&m_csInstFile); m_mapIpAddr.insert( map<DWORD,WORD>::value_type(dwIpAddr, wCountry));}

	void RemoveIp(DWORD dwIpAddr) 
	{
		SMART_LOCKCS(&m_csInstFile);
		if(m_mapIpAddr.empty())
			return;

		map<DWORD,WORD>::iterator it = m_mapIpAddr.find(dwIpAddr);

		if(it != m_mapIpAddr.end())
			m_mapIpAddr.erase(it);
	}

	void GetIp(deque<DWORD>& qBuffer, WORD wCountry, CTInstallSession * pSession)
	{
		if(!pSession)
			return;

		qBuffer.clear();
		SMART_LOCKCS(&m_csInstFile);
		for(map<DWORD,WORD>::iterator it = m_mapIpAddr.begin(); it != m_mapIpAddr.end(); it++)
		{
			if(it->second == wCountry)
			{
				if(pSession->GetIp() == it->first)
					continue;
				qBuffer.push_back(it->first);
				if(20 >= qBuffer.size())
					break;
			}
		}
	}



};