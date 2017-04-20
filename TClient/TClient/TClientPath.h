#pragma once


class CTClientPath
{
public:
	MAPTPATHDATA m_mapTPATHCASH[TPATH_CASH_COUNT];

	MAPTPATHDATA m_mapTPATH;
	MAPT2DPOINT m_mapTPOINT;
	VECTORDWORD m_vTBLOCK;

	FLOAT m_fLengthX;
	FLOAT m_fLengthZ;
	FLOAT m_fBaseX;
	FLOAT m_fBaseZ;

public:
	T2DPOINT GetT2DPOINT( LPD3DXVECTOR3 pTPOINT);

	void BuildTSTARTPOINT(
		LPD3DXVECTOR3 pTPOS,
		int nCashIndex);

	BYTE FindT3DPOINT(
		LPD3DXVECTOR3 pTPOS,
		WORD wPointID);

	BYTE FindTPATH(
		LPTPATHINFO *pTPATHINFO,
		LPD3DXVECTOR3 pTSTART,
		LPD3DXVECTOR3 pTTARGET,
		int nCashIndex);

	BYTE CanMove(
		T2DPOINT lFrom,
		WORD wPointID);

	BYTE CanMove(
		T2DPOINT lFrom,
		T2DPOINT lTo);

	BYTE CheckBreak(
		T2DPOINT lPoint,
		WORD wLineS,
		WORD wLineE);

	BYTE GetT2DNORMALLINE(
		T2DPOINT lPoint,
		T2DPOINT lLineS,
		T2DPOINT lLineE,
		FLOAT *pA,
		FLOAT *pB,
		FLOAT *pC);

	BYTE GetT2DLINE(
		T2DPOINT lStart,
		T2DPOINT lEnd,
		FLOAT *pA,
		FLOAT *pB,
		FLOAT *pC);

	BYTE GetT2DLINE(
		FLOAT fStartX,
		FLOAT fStartY,
		FLOAT fEndX,
		FLOAT fEndY,
		FLOAT *pA,
		FLOAT *pB,
		FLOAT *pC);

	FLOAT GetT2DDotCoord(
		T2DPOINT lPoint,
		FLOAT fA,
		FLOAT fB,
		FLOAT fC);

	FLOAT GetLineLength(
		T2DPOINT lFrom,
		WORD wPointID);

	FLOAT GetLineLength(
		T2DPOINT lFrom,
		T2DPOINT lTo);

	void LoadTPATH( CTClientPath *pTPATH);
	void LoadTPATH( CString strFileName);
	void ReleaseTPATH();

public:
	CTClientPath();
	virtual ~CTClientPath();
};
