#pragma once

#include <map>

using namespace std;
class CPathFlag;

typedef map< DWORD, CPathFlag *>				MAPTPATHFLAG, *LPMAPTPATHFLAG;
#define TPATHFLAG_NULL							((DWORD) 0x00000000)

class CPathFlag
{
public:
	static MAPTPATHFLAG m_mapTPATHFLAG;
	static WORD m_wNewID;

public:
	static void ReleaseTPATH();
	static void InitTPATH();

public:
	static void RegTPATH(
		CPathFlag *pTPATH,
		BYTE bUnitX,
		BYTE bUnitZ);

public:
	D3DXVECTOR3 m_vPOS;
	DWORD m_dwID;

public:
	CPathFlag();
	virtual ~CPathFlag();
};
