#pragma once


class CTMHBuilderMAP
{
public:
	static MAPDWORD m_mapTMAPINFO;

	static FLOAT m_fClimbBound;
	static FLOAT m_fMoveBound;
	static FLOAT m_fFallBound;
	static FLOAT m_fGravity;

public:
	static DWORD GetMapID( WORD wMapID);
	static void InitMAPINFO();
	static void ReleaseMAPINFO();

public:
	CTachyonHUGEMAP *m_pMAP;
	WORD m_wMapID;

	MAPTHEIGHTINFO m_mapTHEIGHTINFO;
	MAPTMHOBJ m_mapTOBJ;

public:
	BYTE LockMAP(
		CTachyonRes *pRES,
		WORD wMapID);

	void LoadUNIT(
		CD3DDevice *pDevice,
		int nUnitID);

	void ReleaseUNIT();
	void ReleaseMAP();

public:
	CTMHBuilderMAP();
	virtual ~CTMHBuilderMAP();
};
