#include "StdAfx.h"


CTClientPath::CTClientPath()
{
	for( int i=0; i<TPATH_CASH_COUNT; i++)
		m_mapTPATHCASH[i].clear();

	m_mapTPOINT.clear();
	m_mapTPATH.clear();
	m_vTBLOCK.clear();

	m_fLengthX = 0.0f;
	m_fLengthZ = 0.0f;
	m_fBaseX = 0.0f;
	m_fBaseZ = 0.0f;
}

CTClientPath::~CTClientPath()
{
	ReleaseTPATH();
}

void CTClientPath::LoadTPATH( CString strFileName)
{
	ReleaseTPATH();

	TRY
	{
		CFile file( LPCSTR(strFileName), CFile::modeRead|CFile::typeBinary);
		CArchive ar( &file, CArchive::load);

		DWORD dwCount;
		ar >> dwCount;

		for( DWORD i=0; i<dwCount; i++)
		{
			T2DPOINT lPoint;
			WORD wPointID;

			ar  >> wPointID
				>> lPoint;

			m_mapTPOINT.insert( MAPT2DPOINT::value_type( wPointID, lPoint));
		}
		ar >> dwCount;

		for(int i=0; i<dwCount; i++)
		{
			DWORD dwTBLOCK;
			ar >> dwTBLOCK;

			m_vTBLOCK.push_back(dwTBLOCK);
		}
		ar >> dwCount;

		for(int i=0; i<dwCount; i++)
		{
			WORD wPointID;
			DWORD dwPATH;

			ar  >> wPointID
				>> dwPATH;

			if(dwPATH)
			{
				LPMAPTPATHINFO pTINFO = new MAPTPATHINFO();

				m_mapTPATH.insert( MAPTPATHDATA::value_type( wPointID, pTINFO));
				for( DWORD j=0; j<dwPATH; j++)
				{
					LPTPATHINFO pTPATH = new TPATHINFO();
					WORD wTargetID;
					DWORD dwNODE;

					ar  >> wTargetID
						>> pTPATH->m_fDIST
						>> dwNODE;

					pTINFO->insert( MAPTPATHINFO::value_type( wTargetID, pTPATH));
					for( DWORD k=0; k<dwNODE; k++)
					{
						WORD wNodeID;
						ar >> wNodeID;

						pTPATH->m_vTPATH.push_back(wNodeID);
					}
				}
			}
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL
}

void CTClientPath::LoadTPATH( CTClientPath *pTPATH)
{
	MAPTPATHDATA::iterator itTPATH;
	MAPT2DPOINT::iterator itTPOINT;
	ReleaseTPATH();

	for( itTPOINT = pTPATH->m_mapTPOINT.begin(); itTPOINT != pTPATH->m_mapTPOINT.end(); itTPOINT++)
		m_mapTPOINT.insert( MAPT2DPOINT::value_type( (*itTPOINT).first, (*itTPOINT).second));

	for( itTPATH = pTPATH->m_mapTPATH.begin(); itTPATH != pTPATH->m_mapTPATH.end(); itTPATH++)
		m_mapTPATH.insert( MAPTPATHDATA::value_type( (*itTPATH).first, (*itTPATH).second));

	for( int i=0; i<INT(pTPATH->m_vTBLOCK.size()); i++)
		m_vTBLOCK.push_back(pTPATH->m_vTBLOCK[i]);

	m_fLengthX = pTPATH->m_fLengthX;
	m_fLengthZ = pTPATH->m_fLengthZ;
	m_fBaseX = pTPATH->m_fBaseX;
	m_fBaseZ = pTPATH->m_fBaseZ;

	for(int i=0; i<TPATH_CASH_COUNT; i++)
		pTPATH->m_mapTPATHCASH[i].clear();

	pTPATH->m_mapTPOINT.clear();
	pTPATH->m_mapTPATH.clear();
	pTPATH->m_vTBLOCK.clear();
	pTPATH->ReleaseTPATH();
}

void CTClientPath::ReleaseTPATH()
{
	MAPTPATHDATA::iterator itTPATH;

	for( itTPATH = m_mapTPATH.begin(); itTPATH != m_mapTPATH.end(); itTPATH++)
	{
		LPMAPTPATHINFO pTINFO = (*itTPATH).second;

		if(pTINFO)
		{
			MAPTPATHINFO::iterator itTINFO;

			for( itTINFO = pTINFO->begin(); itTINFO != pTINFO->end(); itTINFO++)
				if((*itTINFO).second)
					delete (*itTINFO).second;

			pTINFO->clear();
			delete pTINFO;
		}
	}

	for( int i=0; i<TPATH_CASH_COUNT; i++)
		m_mapTPATHCASH[i].clear();

	m_fLengthX = 0.0f;
	m_fLengthZ = 0.0f;
	m_fBaseX = 0.0f;
	m_fBaseZ = 0.0f;

	m_mapTPOINT.clear();
	m_mapTPATH.clear();
	m_vTBLOCK.clear();
}

BYTE CTClientPath::FindT3DPOINT( LPD3DXVECTOR3 pTPOS,
								 WORD wPointID)
{
	MAPT2DPOINT::iterator finder = m_mapTPOINT.find(wPointID);

	if( finder != m_mapTPOINT.end() )
	{
		T2DPOINT lPoint = (*finder).second;

		pTPOS->x = GET2D_X(lPoint) + m_fBaseX;
		pTPOS->y = 0.0f;
		pTPOS->z = GET2D_Y(lPoint) + m_fBaseZ;

		return TRUE;
	}

	return FALSE;
}

void CTClientPath::BuildTSTARTPOINT( LPD3DXVECTOR3 pTPOS,
									 int nCashIndex)
{
	if( nCashIndex < 0 || nCashIndex >= TPATH_CASH_COUNT )
		return;
	MAPTPATHDATA::iterator itTPATH;

	T2DPOINT lPoint = GetT2DPOINT(pTPOS);
	m_mapTPATHCASH[nCashIndex].clear();

	for( itTPATH = m_mapTPATH.begin(); itTPATH != m_mapTPATH.end(); itTPATH++)
		if(CanMove( lPoint, (*itTPATH).first))
			m_mapTPATHCASH[nCashIndex].insert( MAPTPATHDATA::value_type( (*itTPATH).first, (*itTPATH).second));
}

BYTE CTClientPath::FindTPATH( LPTPATHINFO *pTPATHINFO,
							  LPD3DXVECTOR3 pTSTART,
							  LPD3DXVECTOR3 pTTARGET,
							  int nCashIndex)
{
	if( nCashIndex < 0 || nCashIndex >= TPATH_CASH_COUNT )
		return FALSE;
	MAPTPATHDATA::iterator itTPATH;
	(*pTPATHINFO) = NULL;

	T2DPOINT lFrom = GetT2DPOINT(pTSTART);
	T2DPOINT lTo = GetT2DPOINT(pTTARGET);
	BYTE bDirect = CanMove( lFrom, lTo);

	FLOAT fDirect = bDirect ? GetLineLength( lFrom, lTo) : 0.0f;
	FLOAT fDIST = fDirect;

	for( itTPATH = m_mapTPATHCASH[nCashIndex].begin(); itTPATH != m_mapTPATHCASH[nCashIndex].end(); itTPATH++)
	{
		MAPT2DPOINT::iterator itTPOINT = m_mapTPOINT.find((*itTPATH).first);

		if( itTPOINT != m_mapTPOINT.end() )
		{
			T2DPOINT lPoint = (*itTPOINT).second;
			FLOAT fLocal = GetLineLength( lFrom, lPoint);

			if( fLocal < fDIST || (!bDirect && !(*pTPATHINFO)) )
			{
				LPMAPTPATHINFO pTPATH = (*itTPATH).second;
				MAPTPATHINFO::iterator itTINFO;

				FLOAT fLAST = 0.0f;
				BYTE bFind = FALSE;

				for( itTINFO = pTPATH->begin(); itTINFO != pTPATH->end(); itTINFO++)
				{
					MAPT2DPOINT::iterator itTLAST = m_mapTPOINT.find((*itTINFO).first);

					if( itTLAST != m_mapTPOINT.end() )
					{
						T2DPOINT lLast = (*itTLAST).second;
						FLOAT fLocalLast = GetLineLength( lTo, lLast);

						if( (fLocalLast < fLAST || !bFind) && CanMove( lTo, lLast) )
						{
							(*pTPATHINFO) = (*itTINFO).second;
							fDIST = fLocal;

							fLAST = fLocalLast;
							bFind = TRUE;
						}
					}
				}
			}
		}
	}

	if( (*pTPATHINFO) && !(*pTPATHINFO)->m_vTPATH.empty() )
	{
		int nCount = INT((*pTPATHINFO)->m_vTPATH.size());
		int nIndex = 0;
		int nLast = nCount - 1;

		while( nCount > nIndex + 1 && CheckBreak( lFrom, (*pTPATHINFO)->m_vTPATH[nIndex], (*pTPATHINFO)->m_vTPATH[nIndex + 1]) )
			nIndex++;

		while( nLast > nIndex && CheckBreak( lTo, (*pTPATHINFO)->m_vTPATH[nLast], (*pTPATHINFO)->m_vTPATH[nLast - 1]) )
			nLast--;

		WORD vPointID[2] = {
			(*pTPATHINFO)->m_vTPATH[nIndex],
			(*pTPATHINFO)->m_vTPATH[nLast]};

		if(bDirect)
		{
			T2DPOINT vPoint[2] = {
				lFrom,
				lTo};

			for( int i=0; i<2; i++)
			{
				MAPT2DPOINT::iterator finder = m_mapTPOINT.find(vPointID[i]);

				if( finder == m_mapTPOINT.end() || fDirect < GetLineLength( vPoint[i], (*finder).second) )
				{
					(*pTPATHINFO) = NULL;
					return TRUE;
				}
			}
		}

		itTPATH = m_mapTPATH.find(vPointID[0]);
		if( itTPATH != m_mapTPATH.end() )
		{
			LPMAPTPATHINFO pTPATH = (*itTPATH).second;

			if(pTPATH)
			{
				MAPTPATHINFO::iterator itTINFO = pTPATH->find(vPointID[1]);

				if( itTINFO != pTPATH->end() )
					(*pTPATHINFO) = (*itTINFO).second;
			}
		}
	}

	return bDirect || (*pTPATHINFO) ? TRUE : FALSE;
}

BYTE CTClientPath::CanMove( T2DPOINT lFrom,
						    WORD wPointID)
{
	MAPT2DPOINT::iterator finder = m_mapTPOINT.find(wPointID);

	return finder != m_mapTPOINT.end() ? CanMove( lFrom, (*finder).second) : FALSE;
}

BYTE CTClientPath::CanMove( T2DPOINT lFrom,
						    T2DPOINT lTo)
{
	FLOAT fA = 0.0f;
	FLOAT fB = 0.0f;
	FLOAT fC = 0.0f;

	if( lFrom == lTo || !GetT2DLINE( lFrom, lTo, &fA, &fB, &fC) )
		return TRUE;

	for( int i=0; i<INT(m_vTBLOCK.size()); i++)
	{
		T2DPOINT lPoint[2] = { 0, 0};
		BYTE bValid = TRUE;

		WORD vPointID[2] = {
			LOWORD(m_vTBLOCK[i]),
			HIWORD(m_vTBLOCK[i])};

		for( int j=0; j<2; j++)
		{
			MAPT2DPOINT::iterator finder = m_mapTPOINT.find(vPointID[j]);

			if( finder != m_mapTPOINT.end() )
				lPoint[j] = (*finder).second;
			else
				bValid = FALSE;
		}

		if(bValid)
		{
			FLOAT fLA = 0.0f;
			FLOAT fLB = 0.0f;
			FLOAT fLC = 0.0f;

			if( GetT2DLINE( lPoint[0], lPoint[1], &fLA, &fLB, &fLC) &&
				GetT2DDotCoord( lPoint[0], fA, fB, fC) * GetT2DDotCoord( lPoint[1], fA, fB, fC) < 0.0f &&
				GetT2DDotCoord( lFrom, fLA, fLB, fLC) * GetT2DDotCoord( lTo, fLA, fLB, fLC) < 0.0f )
				return FALSE;
		}
	}

	return TRUE;
}

BYTE CTClientPath::CheckBreak( T2DPOINT lPoint,
							   WORD wLineS,
							   WORD wLineE)
{
	WORD wLineID[2] = {
		wLineS,
		wLineE};
	T2DPOINT vPoint[2];

	FLOAT fA = 0.0f;
	FLOAT fB = 0.0f;
	FLOAT fC = 0.0f;

	for( int i=0; i<2; i++)
	{
		MAPT2DPOINT::iterator finder = m_mapTPOINT.find(wLineID[i]);

		if( finder == m_mapTPOINT.end() )
			return FALSE;

		vPoint[i] = (*finder).second;
	}

	if(!GetT2DNORMALLINE( lPoint, vPoint[0], vPoint[1], &fA, &fB, &fC))
		return FALSE;

	if( GetT2DDotCoord( vPoint[0], fA, fB, fC) * GetT2DDotCoord( vPoint[1], fA, fB, fC) < 0.0f &&
		CanMove( lPoint, vPoint[1]) )
		return TRUE;

	return FALSE;
}

BYTE CTClientPath::GetT2DNORMALLINE( T2DPOINT lPoint,
									 T2DPOINT lLineS,
									 T2DPOINT lLineE,
									 FLOAT *pA,
									 FLOAT *pB,
									 FLOAT *pC)
{
	FLOAT fStartX = GET2D_X(lPoint);
	FLOAT fStartY = GET2D_Y(lPoint);

	return GetT2DLINE(
		fStartX,
		fStartY,
		fStartX - GET2D_Y(lLineE) + GET2D_Y(lLineS),
		fStartY + GET2D_X(lLineE) - GET2D_X(lLineS),
		pA, pB, pC);
}

BYTE CTClientPath::GetT2DLINE( T2DPOINT lStart,
							   T2DPOINT lEnd,
							   FLOAT *pA,
							   FLOAT *pB,
							   FLOAT *pC)
{
	return GetT2DLINE(
		GET2D_X(lStart),
		GET2D_Y(lStart),
		GET2D_X(lEnd),
		GET2D_Y(lEnd),
		pA, pB, pC);
}

BYTE CTClientPath::GetT2DLINE( FLOAT fStartX,
							   FLOAT fStartY,
							   FLOAT fEndX,
							   FLOAT fEndY,
							   FLOAT *pA,
							   FLOAT *pB,
							   FLOAT *pC)
{
	FLOAT fC = fStartY * fEndX - fStartX * fEndY;
	FLOAT fA = fEndY - fStartY;
	FLOAT fB = fStartX - fEndX;
	FLOAT fL = sqrt(fA * fA + fB * fB);

	if( fL != 0.0f )
	{
		(*pA) = fA / fL;
		(*pB) = fB / fL;
		(*pC) = fC / fL;

		return TRUE;
	}

	return FALSE;
}

FLOAT CTClientPath::GetT2DDotCoord( T2DPOINT lPoint,
								    FLOAT fA,
									FLOAT fB,
									FLOAT fC)
{
	return GET2D_X(lPoint) * fA + GET2D_Y(lPoint) * fB + fC;
}

T2DPOINT CTClientPath::GetT2DPOINT( LPD3DXVECTOR3 pTPOINT)
{
	return MAKET2DPOINT( pTPOINT->x - m_fBaseX, pTPOINT->z - m_fBaseZ);
}

FLOAT CTClientPath::GetLineLength( T2DPOINT lFrom,
								   WORD wPointID)
{
	MAPT2DPOINT::iterator finder = m_mapTPOINT.find(wPointID);

	return finder != m_mapTPOINT.end() ? GetLineLength( lFrom, (*finder).second) : 0.0f;
}

FLOAT CTClientPath::GetLineLength( T2DPOINT lFrom,
								   T2DPOINT lTo)
{
	FLOAT fX = GET2D_X(lFrom) - GET2D_X(lTo);
	FLOAT fY = GET2D_Y(lFrom) - GET2D_Y(lTo);

	return sqrt(fX * fX + fY * fY);
}
