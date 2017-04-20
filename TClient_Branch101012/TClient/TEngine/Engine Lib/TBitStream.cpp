#include "StdAfx.h"


CTBitStream::CTBitStream( LPBYTE pSTREAM, DWORD dwSIZE)
{
	m_pEND = dwSIZE ? pSTREAM + dwSIZE - 1 : pSTREAM;
	m_pCUR = pSTREAM;
	m_pHEAD = pSTREAM;

	m_bPOS = 0;
}

CTBitStream::~CTBitStream()
{
}

DWORD CTBitStream::ReadBITs( BYTE bCount)
{
	LPBYTE pCUR = m_pCUR;
	BYTE bPOS = m_bPOS;
	DWORD dwResult = PopBITs(bCount);

	m_pCUR = pCUR;
	m_bPOS = bPOS;

	return dwResult;
}

DWORD CTBitStream::PopBITs( BYTE bCount)
{
	static BYTE vMASK[2][8] = {
		{ 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F},
		{ 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE}};

	if( bCount > 32 )
	{
		SeekBITs(bCount - 32);
		bCount = 32;
	}

	BYTE bREAD = bCount + m_bPOS;
	DWORD dwVALUE = 0;
	LPBYTE pVALUE = (LPBYTE) &dwVALUE;

	for( BYTE i=0; i<4; i++)
		dwVALUE = (dwVALUE << 8) | m_pCUR[i];
	dwVALUE <<= m_bPOS;

	if( bREAD > 32 )
	{
		BYTE bVALUE = *(m_pCUR + sizeof(DWORD));
		BYTE bINDEX = 8 - m_bPOS;

		pVALUE[0] = (pVALUE[0] & vMASK[1][bINDEX]) | ((bVALUE >> bINDEX) & vMASK[0][m_bPOS]);
	}

	bCount = 32 - bCount;
	dwVALUE >>= bCount;

	BYTE bBYTE = bCount / 8;
	BYTE bBIT = bCount % 8;

	for( i=0; i<bBYTE; i++)
		pVALUE[3 - i] = 0x00;

	if(bBIT)
		pVALUE[3 - bBYTE] &= vMASK[0][8 - bBIT];

	m_pCUR += bREAD / 8;
	m_bPOS = bREAD % 8;

	return dwVALUE;
}

void CTBitStream::SeekBITs( DWORD dwCount)
{
	dwCount += m_bPOS;

	m_pCUR += dwCount / 8;
	m_bPOS = BYTE(dwCount % 8);
}

DWORD CTBitStream::GetPOS()
{
	return (DWORD(m_pCUR - m_pHEAD) << 3) + DWORD(m_bPOS);
}

BYTE CTBitStream::IsOverRun()
{
	return m_pCUR > m_pEND ? TRUE : FALSE;
}
