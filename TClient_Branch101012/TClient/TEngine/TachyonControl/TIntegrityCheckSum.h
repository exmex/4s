#if (!defined __TINTEGRITYCHECKSUM_H)
#define __TINTEGRITYCHECKSUM_H

#include "TMfcDef.h"
#include <AfxConv.h>
#include <io.h>


class __TUI_API CTIntegrityCheckSum
{
public:
	static BOOL IsFileExists( LPCTSTR strFilePath)
	{
		if(!strFilePath)
			return FALSE;

#if (defined _UNICODE)
		struct _wfinddata_t fd;
#else
		struct _finddata_t fd;
#endif
		long hNextFile =
#if (defined _UNICODE)
			_wfindfirst(
#else
			_findfirst(
#endif
			(LPTSTR) strFilePath,
			&fd);

		BOOL bExists = TRUE;

		if( hNextFile < 0 )
			bExists = FALSE;
		else if(fd.attrib&_A_SUBDIR)
			bExists = FALSE;

		_findclose(hNextFile);
		return bExists;
	};

public:
	//constructor/destructor
	CTIntegrityCheckSum()
	{
		Init();
	};

	~CTIntegrityCheckSum()
	{
	};

protected:
	int m_nCurrent;
	DWORD m_dwHashStarts[4];

	void Init()
	{
		m_nCurrent = 0L;
		::memset( m_dwHashStarts, 0, sizeof(m_dwHashStarts));
	}

	void JumpNextItem()
	{
		m_nCurrent++;
		if( m_nCurrent < sizeof(m_dwHashStarts) / sizeof(m_dwHashStarts[0]) )
			return;

		m_nCurrent = 0L;
	}

public:
	void Update(
		const BYTE *pInput,
		ULONG nInputLen)
	{
		for( ULONG nIdx=0; nIdx<nInputLen; nIdx++, pInput++)
		{
			DWORD &dwHashItem = m_dwHashStarts[m_nCurrent];
			DWORD dwRecalc;

			dwHashItem = ( dwHashItem << 4 ) + (*pInput);
			if((dwRecalc = dwHashItem & 0xF0000000L))
			{
				dwHashItem ^= dwRecalc >> 24;
				dwHashItem ^= dwRecalc;
			}

			JumpNextItem();
		}
	}

	CString Final()
	{
		CString strHash(_T(""));

		for( int nIdx = 0; nIdx < sizeof(m_dwHashStarts) / sizeof(m_dwHashStarts[0]); nIdx++)
		{
			CString strTmp;

			strTmp.Format(
				_T("%s%02X %02X %02X %02X"),
				nIdx > 0 ? _T(" ") :_T(""),
				(INT(m_dwHashStarts[nIdx]) >> 24) & 0x0FF,
				(INT(m_dwHashStarts[nIdx]) >> 16) & 0x0FF,
				(INT(m_dwHashStarts[nIdx]) >> 8) & 0x0FF,
				INT(m_dwHashStarts[nIdx]) & 0x0FF);

			strHash += strTmp;
		}

		return strHash;
	}
};


#endif // #if (!defined __TINTEGRITYCHECKSUM_H)