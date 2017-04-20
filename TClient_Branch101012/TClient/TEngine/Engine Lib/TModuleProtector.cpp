#include "StdAfx.h"

typedef LONG (__stdcall *FN_UNMAPSECTION)( IN HANDLE, IN PVOID);

#define TMP_CHECK_TICK			(900000)
#define TMP_TICK				(500)

#define MAX_PROCESS_COUNT		(2048)


CTModuleProtector::CTModuleProtector()
{
	InitializeCriticalSection(&m_cs);
	m_hThread = NULL;
	m_bRUN = FALSE;

	m_vTMODULE.clear();
	m_vBLOCK.clear();

	m_dwVERSION = 0;
	m_dwTICK = 0;
}

CTModuleProtector::~CTModuleProtector()
{
	ClearModule();
	DeleteCriticalSection(&m_cs);
}

DWORD WINAPI CTModuleProtector::_WatchThread( LPVOID lpParam)
{
	((CTModuleProtector *) lpParam)->WatchThread();
	return 0;
}

void CTModuleProtector::WatchThread()
{
	while( !HackDetected() && CheckALL() )
	{
		SetTICK();
		Sleep(TMP_TICK);
	}

	SetRUN(FALSE);
}

BYTE CTModuleProtector::CheckImageBase( LPCSTR szFILE)
{
	CFile vFILE;

	if(!vFILE.Open( szFILE, CFile::modeRead|CFile::typeBinary))
		return FALSE;

	IMAGE_DOS_HEADER vDOS;
	IMAGE_NT_HEADERS vNT;

	vFILE.Read( &vDOS, sizeof(IMAGE_DOS_HEADER));
	if( vDOS.e_magic != IMAGE_DOS_SIGNATURE )
	{
		vFILE.Close();
		return FALSE;
	}

	vFILE.Seek( vDOS.e_lfanew, CFile::begin);
	vFILE.Read( &vNT, sizeof(IMAGE_NT_HEADERS));
	vFILE.Close();

	if( vNT.Signature != IMAGE_NT_SIGNATURE || !CheckVERSION(vNT.FileHeader.TimeDateStamp) )
		return FALSE;

	return TRUE;
}

BYTE CTModuleProtector::CheckValid()
{
	DWORD dwTick = GetTickCount();
	SMART_LOCKCS(&m_cs);
	return !m_bRUN || dwTick > TMP_CHECK_TICK + m_dwTICK ? FALSE : TRUE;
}

BYTE CTModuleProtector::HackDetected()
{
	SMART_LOCKCS(&m_cs);
	return m_bRUN ? FALSE : TRUE;
}

void CTModuleProtector::SetRUN( BYTE bRUN)
{
	SMART_LOCKCS(&m_cs);
	m_bRUN = bRUN;
}

void CTModuleProtector::SetTICK()
{
	SMART_LOCKCS(&m_cs);
	m_dwTICK = GetTickCount();
}

BYTE CTModuleProtector::BeginWatch()
{
	DWORD dwThreadID;
	EndWatch();

	m_hThread = CreateThread(
		NULL, 0,
		_WatchThread,
		(LPVOID) this,
		CREATE_SUSPENDED,
		&dwThreadID);

	if(!m_hThread)
		return FALSE;

	if(!SetThreadPriority( m_hThread, THREAD_PRIORITY_TIME_CRITICAL))
	{
		SetRUN(FALSE);
		ResumeThread(m_hThread);
		WaitForSingleObject(
			m_hThread,
			INFINITE);
		m_hThread = NULL;

		return FALSE;
	}

	SetRUN(TRUE);
	SetTICK();
	ResumeThread(m_hThread);

	return TRUE;
}

void CTModuleProtector::EndWatch()
{
	if(m_hThread)
	{
		SetRUN(FALSE);
		WaitForSingleObject(
			m_hThread,
			INFINITE);

		m_hThread = NULL;
	}
}

void CTModuleProtector::ClearModule()
{
	EndWatch();

	while(!m_vTMODULE.empty())
	{
		delete m_vTMODULE.back();
		m_vTMODULE.pop_back();
	}

	m_vBLOCK.clear();
	m_dwVERSION = 0;
}

BYTE CTModuleProtector::InitProtector( LPCSTR szResourceID, LPCSTR szType)
{
	HRSRC hINFO = FindResource( NULL, szResourceID, szType);
	if(!hINFO)
		return FALSE;

	HGLOBAL hDATA = LoadResource( NULL, hINFO);
	if(!hDATA)
		return FALSE;

	LPBYTE pBUF = (LPBYTE) LockResource(hDATA);
	if(!pBUF)
		return FALSE;

	return InitProtector(pBUF);
}

BYTE CTModuleProtector::InitProtector( CString& strFILE)
{
	HANDLE hFILE = CreateFile(
		LPCSTR(strFILE),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0, NULL);

	if( hFILE == INVALID_HANDLE_VALUE )
		return FALSE;

	HANDLE hMAP = CreateFileMapping(
		hFILE,
		NULL,
		PAGE_READONLY,
		0, 0, NULL);

	if(!hMAP)
	{
		CloseHandle(hFILE);
		return FALSE;
	}

	LPBYTE pBUF = (LPBYTE) MapViewOfFile(
		hMAP,
		FILE_MAP_READ,
		0, 0, 0);
	BYTE bResult = InitProtector(pBUF);

	UnmapViewOfFile(pBUF);
	CloseHandle(hMAP);
	CloseHandle(hFILE);

	return bResult;
}

BYTE CTModuleProtector::InitProtector( LPBYTE pBUF)
{
	VTPROTECTED_MODULE vTMODULE;
	VECTORSTRING vBLOCK;
	DWORD dwVERSION;

	vTMODULE.clear();
	vBLOCK.clear();
	dwVERSION = 0;

	LPBYTE pDATA = pBUF + pBUF[0] + 1;
	LPBYTE pMASK = pBUF + 1;
	BYTE bMIDX = 0;

	dwVERSION = ReadDWORD(
		&pDATA,
		pMASK,
		&bMIDX,
		pBUF[0]);

	DWORD dwCOUNT = (DWORD) ReadWORD(
		&pDATA,
		pMASK,
		&bMIDX,
		pBUF[0]);

	for( DWORD i=0; i<dwCOUNT; i++)
		vBLOCK.push_back(ReadString( &pDATA, pMASK, &bMIDX, pBUF[0]));

	LPTPROTECTED_MODULE pTESSMODULE = new TPROTECTED_MODULE();
	LPTPROTECTED_PROC pTESSPROC[TESSPROC_COUNT];

	pTESSMODULE->m_strModuleName = ReadString(
		&pDATA,
		pMASK,
		&bMIDX,
		pBUF[0]);
	vTMODULE.push_back(pTESSMODULE);

	for( i=0; i<TESSPROC_COUNT; i++)
		pTESSPROC[i] = pTESSMODULE->AddProc(ReadString( &pDATA, pMASK, &bMIDX, pBUF[0]));

	dwCOUNT = (DWORD) ReadWORD(
		&pDATA,
		pMASK,
		&bMIDX,
		pBUF[0]);

	for( i=0; i<dwCOUNT; i++)
	{
		LPTPROTECTED_MODULE pTMODULE = NULL;
		WORD wPROC = ReadWORD(
			&pDATA,
			pMASK,
			&bMIDX,
			pBUF[0]);

		CString strNAME = ReadString(
			&pDATA,
			pMASK,
			&bMIDX,
			pBUF[0]);

		if( pTESSMODULE->m_strModuleName != strNAME )
		{
			pTMODULE = new TPROTECTED_MODULE();

			pTMODULE->m_strModuleName = strNAME;
			vTMODULE.push_back(pTMODULE);
		}
		else
			pTMODULE = pTESSMODULE;

		for( WORD j=0; j<wPROC; j++)
		{
			BYTE bESS = FALSE;

			strNAME = ReadString(
				&pDATA,
				pMASK,
				&bMIDX,
				pBUF[0]);

			if( pTMODULE == pTESSMODULE )
				for( int k=0; k<TESSPROC_COUNT; k++)
					if( pTESSPROC[k]->m_strProcName == strNAME )
					{
						bESS = TRUE;
						break;
					}

			if(!bESS)
				pTMODULE->AddProc(strNAME);
		}
	}

	BYTE bResult = InitProtector(
		&vTMODULE,
		&vBLOCK,
		dwVERSION);

	while(!vTMODULE.empty())
	{
		delete vTMODULE.back();
		vTMODULE.pop_back();
	}

	vBLOCK.clear();
	dwVERSION = 0;

	return bResult;
}

BYTE CTModuleProtector::InitProtector( LPVTPROTECTED_MODULE pVMODULE,
									   LPVECTORSTRING pBLOCK,
									   DWORD dwVERSION)
{
	TCHAR szNAME[MAX_PATH];
	LPSTR pNAME = NULL;
	ClearModule();

	for( int i=0; i<INT(pVMODULE->size()); i++)
	{
		if(!SearchPath(
			NULL,
			LPCSTR((*pVMODULE)[i]->m_strModuleName),
			_T(".dll"),
			MAX_PATH,
			szNAME,
			&pNAME))
		{
			ClearModule();
			return FALSE;
		}

		HANDLE hFILE = CreateFile(
			szNAME,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0, NULL);

		if( hFILE == INVALID_HANDLE_VALUE )
		{
			ClearModule();
			return FALSE;
		}

		HANDLE hMAP = CreateFileMapping(
			hFILE,
			NULL,
			PAGE_READONLY,
			0, 0, NULL);

		if(!hMAP)
		{
			CloseHandle(hFILE);
			ClearModule();

			return FALSE;
		}

		LPBYTE pBUF = (LPBYTE) MapViewOfFile(
			hMAP,
			FILE_MAP_READ,
			0, 0, 0);

		PIMAGE_DOS_HEADER pDOS = (PIMAGE_DOS_HEADER) pBUF;
		if( pDOS->e_magic != IMAGE_DOS_SIGNATURE )
		{
			UnmapViewOfFile(pBUF);
			CloseHandle(hMAP);
			CloseHandle(hFILE);
			ClearModule();

			return FALSE;
		}

		PIMAGE_NT_HEADERS pNT = (PIMAGE_NT_HEADERS) (pBUF + pDOS->e_lfanew);
		if( pNT->Signature != IMAGE_NT_SIGNATURE )
		{
			UnmapViewOfFile(pBUF);
			CloseHandle(hMAP);
			CloseHandle(hFILE);
			ClearModule();

			return FALSE;
		}

		PIMAGE_SECTION_HEADER pISECTION = (PIMAGE_SECTION_HEADER) (pBUF + pDOS->e_lfanew + sizeof(IMAGE_NT_HEADERS));
		DWORD dwRAW = GetRAWFromRVA(
			pISECTION,
			pNT->FileHeader.NumberOfSections,
			pNT->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

		if( dwRAW == TINVALID_INDEX )
		{
			UnmapViewOfFile(pBUF);
			CloseHandle(hMAP);
			CloseHandle(hFILE);
			ClearModule();

			return FALSE;
		}

		PIMAGE_EXPORT_DIRECTORY pIDIR = (PIMAGE_EXPORT_DIRECTORY) (pBUF + dwRAW);
		dwRAW = GetRAWFromRVA(
			pISECTION,
			pNT->FileHeader.NumberOfSections,
			pIDIR->AddressOfNames);

		if( dwRAW == TINVALID_INDEX )
		{
			UnmapViewOfFile(pBUF);
			CloseHandle(hMAP);
			CloseHandle(hFILE);
			ClearModule();

			return FALSE;
		}

		LPDWORD pNAMERVA = (LPDWORD) (pBUF + dwRAW);
		dwRAW = GetRAWFromRVA(
			pISECTION,
			pNT->FileHeader.NumberOfSections,
			pIDIR->AddressOfNameOrdinals);

		if( dwRAW == TINVALID_INDEX )
		{
			UnmapViewOfFile(pBUF);
			CloseHandle(hMAP);
			CloseHandle(hFILE);
			ClearModule();

			return FALSE;
		}

		LPWORD pORD = (LPWORD) (pBUF + dwRAW);
		dwRAW = GetRAWFromRVA(
			pISECTION,
			pNT->FileHeader.NumberOfSections,
			pIDIR->AddressOfFunctions);

		if( dwRAW == TINVALID_INDEX )
		{
			UnmapViewOfFile(pBUF);
			CloseHandle(hMAP);
			CloseHandle(hFILE);
			ClearModule();

			return FALSE;
		}

		LPDWORD pPROCRVA = (LPDWORD) (pBUF + dwRAW);
		MAPSTRING mapPROC;
		mapPROC.clear();

		for( DWORD j=0; j<pIDIR->NumberOfNames; j++)
		{
			dwRAW = GetRAWFromRVA(
				pISECTION,
				pNT->FileHeader.NumberOfSections,
				pNAMERVA[j]);

			if( dwRAW == TINVALID_INDEX )
			{
				UnmapViewOfFile(pBUF);
				CloseHandle(hMAP);
				CloseHandle(hFILE);
				ClearModule();
				mapPROC.clear();

				return FALSE;
			}

			mapPROC.insert( MAPSTRING::value_type( CString(pBUF + dwRAW), pPROCRVA[pORD[j]]));
		}

		LPTPROTECTED_MODULE pTMODULE = new TPROTECTED_MODULE();
		m_vTMODULE.push_back(pTMODULE);
		pTMODULE->m_strModuleName = (*pVMODULE)[i]->m_strModuleName;

		for( j=0; j<INT((*pVMODULE)[i]->m_vTPROC.size()); j++)
		{
			MAPSTRING::iterator finder = mapPROC.find((*pVMODULE)[i]->m_vTPROC[j]->m_strProcName);

			if( finder == mapPROC.end() )
			{
				UnmapViewOfFile(pBUF);
				CloseHandle(hMAP);
				CloseHandle(hFILE);
				ClearModule();
				mapPROC.clear();

				return FALSE;
			}

			DWORD dwISECTION = GetImageSectionFromRVA(
				pISECTION,
				pNT->FileHeader.NumberOfSections,
				(*finder).second);

			if( dwISECTION == TINVALID_INDEX )
			{
				UnmapViewOfFile(pBUF);
				CloseHandle(hMAP);
				CloseHandle(hFILE);
				ClearModule();
				mapPROC.clear();

				return FALSE;
			}

			dwRAW = GetRAWFromRVA(
				pISECTION + dwISECTION,
				(*finder).second);

			if( dwRAW == TINVALID_INDEX )
			{
				UnmapViewOfFile(pBUF);
				CloseHandle(hMAP);
				CloseHandle(hFILE);
				ClearModule();
				mapPROC.clear();

				return FALSE;
			}

			LPTPROTECTED_PROC pTPROC = pTMODULE->AddProc((*pVMODULE)[i]->m_vTPROC[j]->m_strProcName);
			pTPROC->m_bSIZE = GetCheckSIZE(
				pISECTION + dwISECTION,
				pPROCRVA,
				pIDIR->NumberOfFunctions,
				(*finder).second);

			if(!pTPROC->m_bSIZE)
			{
				UnmapViewOfFile(pBUF);
				CloseHandle(hMAP);
				CloseHandle(hFILE);
				ClearModule();
				mapPROC.clear();

				return FALSE;
			}

			memcpy( pTPROC->m_vDATA, pBUF + dwRAW, pTPROC->m_bSIZE);
		}

		UnmapViewOfFile(pBUF);
		CloseHandle(hMAP);
		CloseHandle(hFILE);
		mapPROC.clear();
	}

	for( i=0; i<INT(pBLOCK->size()); i++)
	{
		(*pBLOCK)[i].MakeUpper();
		m_vBLOCK.push_back((*pBLOCK)[i]);
	}
	m_dwVERSION = dwVERSION ^ 0xFFFFFFFF;

	return TRUE;
}

BYTE CTModuleProtector::CheckVERSION( DWORD dwVERSION)
{
#if defined(TEST_MODE) || defined(DEBUG) || defined(_DEBUG)
	return TRUE;
#else
	return m_dwVERSION == (dwVERSION ^ 0xFFFFFFFF) ? TRUE : FALSE;
#endif
}

BYTE CTModuleProtector::CheckMODULE()
{
#ifndef _DEBUG
	if(IsDebuggerPresent())
		return FALSE;
#endif

	if(m_vTMODULE.empty())
		return FALSE;

	for( int i=0; i<INT(m_vTMODULE.size()); i++)
	{
		HMODULE hModule = GetModuleHandle(LPCSTR(m_vTMODULE[i]->m_strModuleName));

		if(!hModule)
			return FALSE;

		for( int j=0; j<INT(m_vTMODULE[i]->m_vTPROC.size()); j++)
		{
			FARPROC hProc = GetProcAddress( hModule, LPCSTR(m_vTMODULE[i]->m_vTPROC[j]->m_strProcName));

			if( !hProc || memcmp( hProc, m_vTMODULE[i]->m_vTPROC[j]->m_vDATA, m_vTMODULE[i]->m_vTPROC[j]->m_bSIZE) )
				return FALSE;
		}
	}

	return TRUE;
}

BYTE CTModuleProtector::CheckBLOCK()
{
	DWORD vPROCESS[MAX_PROCESS_COUNT];
	DWORD dwCOUNT;
	DWORD dwREAD;

	if(!EnumProcesses( vPROCESS, MAX_PROCESS_COUNT * sizeof(DWORD), &dwREAD))
		return FALSE;
	dwCOUNT = dwREAD / sizeof(DWORD);

	for( DWORD i=0; i<dwCOUNT; i++)
	{
		HANDLE hPROCESS = OpenProcess(
			PROCESS_QUERY_INFORMATION|
			PROCESS_VM_READ,
			FALSE,
			vPROCESS[i]);

		if(hPROCESS)
		{
			HMODULE hMODULE = NULL;

			if(EnumProcessModules( hPROCESS, &hMODULE, sizeof(HMODULE), &dwREAD))
			{
				CHAR szNAME[MAX_PATH];

				GetModuleFileNameEx(
					hPROCESS,
					hMODULE,
					szNAME,
					MAX_PATH);

				dwREAD = GetFileVersionInfoSize( szNAME, 0);
				if( dwREAD > 0 )
				{
					LPBYTE pDATA = new BYTE[dwREAD];

					if(GetFileVersionInfo( szNAME, NULL, dwREAD, pDATA))
					{
						LPDWORD pINFO = NULL;
						UINT nLEN = 0;

						if(VerQueryValue( pDATA, _T("\\VarFileInfo\\Translation"), (LPVOID *) &pINFO, &nLEN))
						{
							CString strDIR;

							strDIR.Format( _T("\\StringFileInfo\\%04x%04x\\InternalName"), LOWORD(*pINFO), HIWORD(*pINFO));
							if( VerQueryValue( pDATA, (LPSTR) LPCSTR(strDIR), (LPVOID *) &pINFO, &nLEN) &&
								!CheckSTRING((LPCSTR) pINFO) )
							{
								CloseHandle(hPROCESS);
								delete[] pDATA;

								return FALSE;
							}
						}
					}

					delete[] pDATA;
				}
			}

			CloseHandle(hPROCESS);
		}
	}

	return TRUE;
}

BYTE CTModuleProtector::CheckSTRING( LPCSTR szTEXT)
{
	int nCount = INT(m_vBLOCK.size());
	CString strTEXT(szTEXT);

	strTEXT.Remove(' ');
	strTEXT.MakeUpper();

	for( int i=0; i<nCount; i++)
		if( strTEXT.Find(m_vBLOCK[i]) >= 0 )
			return FALSE;

	return TRUE;
}

BYTE CTModuleProtector::CheckALL()
{
	return CheckMODULE() && CheckBLOCK() ? TRUE : FALSE;
}

DWORD CTModuleProtector::GetImageSectionFromRVA( PIMAGE_SECTION_HEADER pISECTION,
												 DWORD dwCount,
												 DWORD dwRVA)
{
	if(!pISECTION)
		return TINVALID_INDEX;

	for( DWORD i=0; i<dwCount; i++)
		if( pISECTION[i].VirtualAddress <= dwRVA && dwRVA < pISECTION[i].VirtualAddress + pISECTION[i].SizeOfRawData )
			return i;

	return TINVALID_INDEX;
}

DWORD CTModuleProtector::GetRAWFromRVA( PIMAGE_SECTION_HEADER pISECTION,
									    DWORD dwRVA)
{
	if(!pISECTION)
		return TINVALID_INDEX;

	return (dwRVA - pISECTION->VirtualAddress) + pISECTION->PointerToRawData;
}

DWORD CTModuleProtector::GetRAWFromRVA( PIMAGE_SECTION_HEADER pISECTION,
									    DWORD dwCount,
										DWORD dwRVA)
{
	DWORD dwISECTION = GetImageSectionFromRVA(
		pISECTION,
		dwCount,
		dwRVA);

	return dwISECTION < dwCount ? GetRAWFromRVA( pISECTION + dwISECTION, dwRVA) : TINVALID_INDEX;
}

BYTE CTModuleProtector::GetCheckSIZE( PIMAGE_SECTION_HEADER pISECTION,
									  LPDWORD pTPROC,
									  DWORD dwCount,
									  DWORD dwRVA)
{
	DWORD dwSIZE = pISECTION->VirtualAddress + pISECTION->Misc.VirtualSize - dwRVA;

	for( DWORD i=0; i<dwCount; i++)
		if( pTPROC[i] > dwRVA )
		{
			DWORD dwLOCAL = pTPROC[i] - dwRVA;

			if( dwLOCAL < dwSIZE )
				dwSIZE = dwLOCAL;
		}

	return dwSIZE < TMAX_CHECK_CODE_SIZE ? BYTE(dwSIZE) : TMAX_CHECK_CODE_SIZE;
}

CString CTModuleProtector::ReadString( LPBYTE *pFILE,
									   LPBYTE pMASK,
									   LPBYTE pMIDX,
									   BYTE bMASK)
{
	BYTE vTEXTBUF[MAX_PATH];
	WORD wSIZE = ReadWORD(
		pFILE,
		pMASK,
		pMIDX,
		bMASK);

	ZeroMemory( vTEXTBUF, MAX_PATH);
	ReadDATA(
		pFILE,
		vTEXTBUF,
		pMASK,
		pMIDX,
		bMASK,
		wSIZE);

	return CString(vTEXTBUF);
}

DWORD CTModuleProtector::ReadDWORD( LPBYTE *pFILE,
								    LPBYTE pMASK,
									LPBYTE pMIDX,
									BYTE bMASK)
{
	DWORD dwRESULT = 0;

	ReadDATA(
		pFILE,
		&dwRESULT,
		pMASK,
		pMIDX,
		bMASK,
		sizeof(DWORD));

	return dwRESULT;
}

WORD CTModuleProtector::ReadWORD( LPBYTE *pFILE,
								  LPBYTE pMASK,
								  LPBYTE pMIDX,
								  BYTE bMASK)
{
	WORD wRESULT = 0;

	ReadDATA(
		pFILE,
		&wRESULT,
		pMASK,
		pMIDX,
		bMASK,
		sizeof(WORD));

	return wRESULT;
}

void CTModuleProtector::ReadDATA( LPBYTE *pFILE,
								  LPVOID pDATA,
								  LPBYTE pMASK,
								  LPBYTE pMIDX,
								  BYTE bMASK,
								  int nSIZE)
{
	LPBYTE pBUF = (LPBYTE) pDATA;

	for( int i=0; i<nSIZE; i++)
	{
		(*pBUF) = (*(*pFILE)) ^ pMASK[(*pMIDX)];

		(*pMIDX) = ((*pMIDX) + 1) % bMASK;
		(*pFILE)++;
		pBUF++;
	}
}
