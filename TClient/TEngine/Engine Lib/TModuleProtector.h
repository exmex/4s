#pragma once


class CTModuleProtector
{
private:
	VTPROTECTED_MODULE m_vTMODULE;
	VECTORSTRING m_vBLOCK;

	CRITICAL_SECTION m_cs;
	HANDLE m_hThread;

	DWORD m_dwVERSION;
	DWORD m_dwTICK;
	BYTE m_bRUN;

private:
	static DWORD WINAPI _WatchThread( LPVOID lpParam);
	void WatchThread();

public:
	BYTE InitProtector( CString& strFILE);
	BYTE InitProtector(
		LPCSTR szResourceID,
		LPCSTR szType);

	BYTE CheckImageBase( LPCSTR szFILE);
	BYTE HackDetected();
	BYTE CheckValid();

	BYTE BeginWatch();
	void EndWatch();
	void ClearModule();

private:
	BYTE CheckVERSION( DWORD dwVERSION);
	BYTE CheckSTRING( LPCSTR szTEXT);
	BYTE CheckMODULE();
	BYTE CheckBLOCK();
	BYTE CheckALL();

	void SetRUN( BYTE bRUN);
	void SetTICK();

	BYTE InitProtector( LPBYTE pBUF);
	BYTE InitProtector(
		LPVTPROTECTED_MODULE pVMODULE,
		LPVECTORSTRING pBLOCK,
		DWORD dwVERSION);

	DWORD GetImageSectionFromRVA(
		PIMAGE_SECTION_HEADER pISECTION,
		DWORD dwCount,
		DWORD dwRVA);

	DWORD GetRAWFromRVA(
		PIMAGE_SECTION_HEADER pISECTION,
		DWORD dwRVA);

	DWORD GetRAWFromRVA(
		PIMAGE_SECTION_HEADER pISECTION,
		DWORD dwCount,
		DWORD dwRVA);

	BYTE GetCheckSIZE(
		PIMAGE_SECTION_HEADER pISECTION,
		LPDWORD pTPROC,
		DWORD dwCount,
		DWORD dwRVA);

	void ReadDATA(
		LPBYTE *pFILE,
		LPVOID pDATA,
		LPBYTE pMASK,
		LPBYTE pMIDX,
		BYTE bMASK,
		int nSIZE);

	CString ReadString(
		LPBYTE *pFILE,
		LPBYTE pMASK,
		LPBYTE pMIDX,
		BYTE bMASK);

	DWORD ReadDWORD(
		LPBYTE *pFILE,
		LPBYTE pMASK,
		LPBYTE pMIDX,
		BYTE bMASK);

	WORD ReadWORD(
		LPBYTE *pFILE,
		LPBYTE pMASK,
		LPBYTE pMIDX,
		BYTE bMASK);

public:
	CTModuleProtector();
	virtual ~CTModuleProtector();
};
