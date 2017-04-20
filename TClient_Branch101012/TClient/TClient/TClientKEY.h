#pragma once


class CTClientKEY
{
public:
	DWORD m_vTICK[TKEY_COUNT];
	TKEY_STATE m_vSTATE[TKEY_COUNT];
	
	CPoint m_point;
	DWORD m_dwTick;

public:
	static WORD GetCurMOD();

	static BYTE GetCTRL();
	static BYTE GetALT();
	static BYTE GetSHIFT();
	
	static BYTE IsKeyDown(BYTE bVKey);

	void ResetSTATE();

public:
	CTClientKEY();
	virtual ~CTClientKEY();
};
