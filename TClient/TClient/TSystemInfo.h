#pragma once

struct IDxDiagContainer;

class CTSystemInfo
{
public:
	// System Information
	CString		m_strDirectXVersion;
	CString		m_strMachineName;
	CString		m_strOSEnglish;
	CString		m_strLanguages;
	CString		m_strBIOS;
	CString		m_strPhysicalMemory;
	CString		m_strProcessor;

	// Display Information
	CString		m_strDeviceName;
	CString		m_strChipType;
	CString		m_strDisplayMemory;
	CString		m_strDriverName;
	CString		m_strDriverVersion;

public:
	void InitInfo();
	CString GetInfo() const;

protected:
	void InitSystemInfo(IDxDiagContainer* pDxDiagRoot);
	void InitDisplayInfo(IDxDiagContainer* pDxDiagRoot);

public:
	CTSystemInfo();

//protected:
//	bool		m_bCPUInformationObtained;
//	CString 	m_strCPUVendor;
//	CString 	m_strCPUType;
//	CString 	m_strCPUArchitecture;
//	CString 	m_CPULevel;
//	CString 	m_CPURevision;
//	long 		m_lNumberOfProcessors;
//	long 		m_lCPUSpeed;
//	long 		m_lPageSize;
//	long 		m_lActiveMask;
//
//public:
//	HRESULT InitCPUInformation();
//	CString GetCPUInformation() const;
//
//	const CString& GetCPUVendor() const			{ return m_strCPUVendor; }
//	const CString& GetCPUType() const			{ return m_strCPUType; }
//	const CString& GetCPUArchitecture() const	{ return m_strCPUArchitecture; }
//	const CString& GetCPULevel() const			{ return m_CPULevel; }
//	const CString& GetCPURevision() const		{ return m_CPURevision; }
//	long GetNumberOfProcessors() const			{ return m_lNumberOfProcessors; }
//	long GetCPUSpeed() const					{ return m_lCPUSpeed; }
//
//protected:
//	BOOL		m_bOSInfoGenerated;
//	BOOL		m_bIsWin95;
//	long		m_sBuildNumber;
//	CString 	m_strPlatform;
//	CString 	m_strSuites;
//	CString 	m_strMinorVersion;
//	CString 	m_strServicePack;
//
//public:
//	HRESULT InitOSInformation();
//	CString GetOSInformation() const;
//
//	BOOL IsWin95() const						{ return m_bIsWin95; }
//	long GetOSBuildNumber() const				{ return m_sBuildNumber; }
//	const CString& GetOSPlatform() const		{ return m_strPlatform; }
//	const CString& GetOSSuites() const			{ return m_strSuites; }
//	const CString& GetOSMinorVersion() const	{ return m_strMinorVersion; }
//	const CString& GetOSServicePack() const		{ return m_strServicePack; }
//
//protected:
//	BOOL		m_bRAMInformationObtained;
//	ULONG		m_lRAMInstalled;
//	ULONG		m_lMemoryAvailable;
//	ULONG		m_lVirtualMemory;
//	ULONG		m_sMemoryLoad;
//	
//public:
//	HRESULT InitRAMInformation();
//	CString GetRAMInformation() const;
//
//	ULONG GetRAMInstalled() const				{ return m_lRAMInstalled; }
//	ULONG GetMemoryAvailable() const				{ return m_lMemoryAvailable; }
//	ULONG GetVirtualMemory() const				{ return m_lVirtualMemory; }
//	ULONG GetMemoryLoad() const					{ return m_sMemoryLoad; }
//
//protected:
//	DWORD		m_dwDxVersion;
//	CString		m_strDxVersion;
//	CString		m_strDeviceName;
//	CString		m_strChipType;
//	CString		m_strRevision;
//	CString		m_strDisplayMemory;
//	CString		m_strDriverName;
//	CString		m_strDriverVersion;
//	
//public:
//	HRESULT InitVideoInformation();
//	CString GetVideoInformation() const;
//
//protected:
//	HRESULT GetDxVersion( DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion );
//	HRESULT GetDirectXVersionViaDxDiag( DWORD* pdwDirectXVersionMajor, DWORD* pdwDirectXVersionMinor, TCHAR* pcDirectXVersionLetter );
//	HRESULT GetDirectXVersionViaFileVersions( DWORD* pdwDirectXVersionMajor, DWORD* pdwDirectXVersionMinor, TCHAR* pcDirectXVersionLetter );
//	HRESULT GetFileVersion( TCHAR* szPath, ULARGE_INTEGER* pllFileVersion );
//	ULARGE_INTEGER MakeInt64( WORD a, WORD b, WORD c, WORD d );
//	int CompareLargeInts( ULARGE_INTEGER ullParam1, ULARGE_INTEGER ullParam2 );
//	HRESULT InitDisplayInfoDxDiag();
};
	
