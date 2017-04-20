#pragma once


class CTBitStream
{
public:
	void SeekBITs( DWORD dwCount);
	DWORD PopBITs( BYTE bCount);
	DWORD ReadBITs( BYTE bCount);

	BYTE IsOverRun();
	DWORD GetPOS();

public:
	LPBYTE m_pHEAD;
	LPBYTE m_pEND;
	LPBYTE m_pCUR;

	BYTE m_bPOS;

public:
	CTBitStream( LPBYTE pSTREAM, DWORD dwSIZE);
	virtual ~CTBitStream();
};
