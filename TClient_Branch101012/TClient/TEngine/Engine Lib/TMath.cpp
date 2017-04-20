// TMath.cpp: implementation of the CTMath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CTMath::CTMath()
{
}

CTMath::~CTMath()
{
}

BYTE CTMath::IsEqual( LPMESHVERTEX pPoint1, LPMESHVERTEX pPoint2)
{
	return fabs(pPoint2->m_fPosX - pPoint1->m_fPosX) < DEF_EPSILON &&
		fabs(pPoint2->m_fPosY - pPoint1->m_fPosY) < DEF_EPSILON &&
		fabs(pPoint2->m_fPosZ - pPoint1->m_fPosZ) < DEF_EPSILON &&
		fabs(pPoint2->m_fNormalX - pPoint1->m_fNormalX) < DEF_EPSILON &&
		fabs(pPoint2->m_fNormalY - pPoint1->m_fNormalY) < DEF_EPSILON &&
		fabs(pPoint2->m_fNormalZ - pPoint1->m_fNormalZ) < DEF_EPSILON &&
		fabs(pPoint2->m_fU1 - pPoint1->m_fU1) < DEF_EPSILON &&
		fabs(pPoint2->m_fV1 - pPoint1->m_fV1) < DEF_EPSILON &&
		fabs(pPoint2->m_fU2 - pPoint1->m_fU2) < DEF_EPSILON &&
		fabs(pPoint2->m_fV2 - pPoint1->m_fV2) < DEF_EPSILON ? TRUE : FALSE;
}

BYTE CTMath::IsEqual( LPWMESHVERTEX pPoint1, LPWMESHVERTEX pPoint2)
{
	return fabs(pPoint2->m_fPosX - pPoint1->m_fPosX) < DEF_EPSILON &&
		fabs(pPoint2->m_fPosY - pPoint1->m_fPosY) < DEF_EPSILON &&
		fabs(pPoint2->m_fPosZ - pPoint1->m_fPosZ) < DEF_EPSILON &&
		fabs(pPoint2->m_fNormalX - pPoint1->m_fNormalX) < DEF_EPSILON &&
		fabs(pPoint2->m_fNormalY - pPoint1->m_fNormalY) < DEF_EPSILON &&
		fabs(pPoint2->m_fNormalZ - pPoint1->m_fNormalZ) < DEF_EPSILON &&
		fabs(pPoint2->m_fU - pPoint1->m_fU) < DEF_EPSILON &&
		fabs(pPoint2->m_fV - pPoint1->m_fV) < DEF_EPSILON &&
		fabs(pPoint2->m_fWeight[0] - pPoint1->m_fWeight[0]) < DEF_EPSILON &&
		fabs(pPoint2->m_fWeight[1] - pPoint1->m_fWeight[1]) < DEF_EPSILON &&
		fabs(pPoint2->m_fWeight[2] - pPoint1->m_fWeight[2]) < DEF_EPSILON &&
		pPoint2->m_dwMatIndex == pPoint1->m_dwMatIndex ? TRUE : FALSE;
}

BYTE CTMath::IsEqual( LPD3DXVECTOR3 pPoint1, LPD3DXVECTOR3 pPoint2)
{
	return fabs(pPoint1->x - pPoint2->x) < DEF_EPSILON &&
		fabs(pPoint1->y - pPoint2->y) < DEF_EPSILON &&
		fabs(pPoint1->z - pPoint2->z) < DEF_EPSILON ? TRUE : FALSE;
}

BYTE CTMath::IsEqual( FLOAT l, FLOAT r)
{
	return fabs(l - r) < DEF_EPSILON ? TRUE : FALSE;
}

void CTMath::Transform( LPD3DXMATRIX pMatrix,
					    LPD3DXVECTOR3 pVector)
{
	D3DXVec3TransformCoord(
		pVector,
		pVector,
		pMatrix);
}

int CTMath::GetNearPow( int nValue)
{
	int nResult = 64;
	nValue = min( nValue, 512);

	while( 2 * nResult <= nValue )
		nResult *= 2;

	return nResult;
}

D3DXQUATERNION CTMath::MaxSquad( D3DXQUATERNION& vStart,
								 D3DXQUATERNION& vA,
								 D3DXQUATERNION& vB,
								 D3DXQUATERNION& vEnd,
								 FLOAT fTime)
{
	return MaxSlerp(
		MaxSlerp( vStart, vEnd, fTime),
		MaxSlerp( vA, vB, fTime),
		2.0f * fTime * ( 1.0f - fTime ));
}

D3DXQUATERNION CTMath::MaxSlerp( D3DXQUATERNION& vStart,
								 D3DXQUATERNION& vEnd,
								 FLOAT fTime)
{
	D3DXQUATERNION vResult;

	D3DXQuaternionSlerp(
		&vResult,
		&vStart,
		&vEnd,
		fTime);

	return vResult;
}

D3DXQUATERNION CTMath::LnDiff( D3DXQUATERNION& vA,
							   D3DXQUATERNION& vB)
{
	D3DXQUATERNION vResult;
	D3DXQUATERNION vInvB;

	D3DXQuaternionInverse(
		&vInvB,
		&vB);

	vResult = vInvB * vA;
	D3DXQuaternionLn(
		&vInvB,
		&vResult);

	return vInvB;
}

FLOAT CTMath::GetTimeValue( DWORD dwTick)
{
	return TIME_RATIO * ((FLOAT) dwTick);
}

DWORD CTMath::GetTimeTick( FLOAT fTime)
{
	return (DWORD) (fTime / TIME_RATIO);
}

WMESHVERTEX CTMath::Average( LPWMESHVERTEX pPoint1, LPWMESHVERTEX pPoint2)
{
	int nIndex[2][WEIGHT_COUNT + 1] = {{
		MATRIXID_1ST(pPoint1->m_dwMatIndex),
		MATRIXID_2ND(pPoint1->m_dwMatIndex),
		MATRIXID_3RD(pPoint1->m_dwMatIndex),
		MATRIXID_4TH(pPoint1->m_dwMatIndex)}, {
		MATRIXID_1ST(pPoint2->m_dwMatIndex),
		MATRIXID_2ND(pPoint2->m_dwMatIndex),
		MATRIXID_3RD(pPoint2->m_dwMatIndex),
		MATRIXID_4TH(pPoint2->m_dwMatIndex)}};

	FLOAT fWeight[2][WEIGHT_COUNT] = {{
		pPoint1->m_fWeight[0],
		pPoint1->m_fWeight[1],
		pPoint1->m_fWeight[2]}, {
		pPoint2->m_fWeight[0],
		pPoint2->m_fWeight[1],
		pPoint2->m_fWeight[2]}};

	MAPWEIGHTINDEXBUF::iterator itBuf;
	MAPWEIGHTINDEXBUF mapWeightIndex;

	MAPWEIGHTBUF::iterator it;
	MAPWEIGHTBUF mapWeight;
	WMESHVERTEX vResult;

	vResult.m_fNormalX = (pPoint1->m_fNormalX + pPoint2->m_fNormalX) / 2.0f;
	vResult.m_fNormalY = (pPoint1->m_fNormalY + pPoint2->m_fNormalY) / 2.0f;
	vResult.m_fNormalZ = (pPoint1->m_fNormalZ + pPoint2->m_fNormalZ) / 2.0f;
	vResult.m_fPosX = (pPoint1->m_fPosX + pPoint2->m_fPosX) / 2.0f;
	vResult.m_fPosY = (pPoint1->m_fPosY + pPoint2->m_fPosY) / 2.0f;
	vResult.m_fPosZ = (pPoint1->m_fPosZ + pPoint2->m_fPosZ) / 2.0f;
	vResult.m_fU = (pPoint1->m_fU + pPoint2->m_fU) / 2.0f;
	vResult.m_fV = (pPoint1->m_fV + pPoint2->m_fV) / 2.0f;

	vResult.m_dwMatIndex = 0x00000000;
	vResult.m_fWeight[0] = 0.0f;
	vResult.m_fWeight[1] = 0.0f;
	vResult.m_fWeight[2] = 0.0f;
	mapWeightIndex.clear();
	mapWeight.clear();

	for( int i=0; i<2; i++)
	{
		FLOAT fSum = 0.0f;

		for( int j=0; j<WEIGHT_COUNT; j++)
		{
			if( nIndex[i][j] > 0 )
			{
				MAPWEIGHTBUF::iterator finder = mapWeight.find(nIndex[i][j]);

				if( finder == mapWeight.end() )
				{
					LPWEIGHTBUF pItem = new WEIGHTBUF;

					pItem->m_fWeight = fWeight[i][j];
					pItem->m_nCount = 1;

					mapWeight.insert( MAPWEIGHTBUF::value_type(
						nIndex[i][j],
						pItem));
				}
				else if(!(*finder).second)
				{
					LPWEIGHTBUF pItem = new WEIGHTBUF;

					pItem->m_fWeight = fWeight[i][j];
					pItem->m_nCount = 1;

					(*finder).second = pItem;
				}
				else
				{
					LPWEIGHTBUF pItem = (*finder).second;

					pItem->m_fWeight += fWeight[i][j];
					pItem->m_nCount++;
				}
			}

			fSum += fWeight[i][j];
		}

		if( nIndex[i][WEIGHT_COUNT] > 0 )
		{
			MAPWEIGHTBUF::iterator finder = mapWeight.find(nIndex[i][WEIGHT_COUNT]);
			fSum = 1.0f - fSum;

			if( finder == mapWeight.end() )
			{
				LPWEIGHTBUF pItem = new WEIGHTBUF;

				pItem->m_fWeight = fSum;
				pItem->m_nCount = 1;

				mapWeight.insert( MAPWEIGHTBUF::value_type(
					nIndex[i][WEIGHT_COUNT],
					pItem));
			}
			else if(!(*finder).second)
			{
				LPWEIGHTBUF pItem = new WEIGHTBUF;

				pItem->m_fWeight = fSum;
				pItem->m_nCount = 1;

				(*finder).second = pItem;
			}
			else
			{
				LPWEIGHTBUF pItem = (*finder).second;

				pItem->m_fWeight += fSum;
				pItem->m_nCount++;
			}
		}
	}

	for( it = mapWeight.begin(); it != mapWeight.end(); it++)
	{
		LPWEIGHTBUF pItem = (*it).second;

		if(pItem)
		{
			pItem->m_fWeight /= pItem->m_nCount;
			pItem->m_nCount = 1;

			MAPWEIGHTINDEXBUF::iterator finder = mapWeightIndex.find(1.0f - pItem->m_fWeight);
			VECTORINT *pWeight = NULL;

			if( finder != mapWeightIndex.end() )
			{
				pWeight = (*finder).second;

				if(!pWeight)
				{
					pWeight = new VECTORINT;
					(*finder).second = pWeight;
				}
			}
			else
			{
				pWeight = new VECTORINT;

				mapWeightIndex.insert( MAPWEIGHTINDEXBUF::value_type(
					1.0f - pItem->m_fWeight,
					pWeight));
			}

			pWeight->push_back((*it).first);
			delete pItem;
		}
	}
	mapWeight.clear();

	FLOAT fLastWeight[WEIGHT_COUNT + 1];
	FLOAT fSum = 0.0f;

	int nLastIndex[WEIGHT_COUNT + 1];
	int nCount = 0;

	for( itBuf = mapWeightIndex.begin(); itBuf != mapWeightIndex.end(); itBuf++)
	{
		VECTORINT *pWeight = (*itBuf).second;

		if(pWeight)
		{
			while(!pWeight->empty())
			{
				if( nCount < WEIGHT_COUNT + 1 )
				{
					fLastWeight[nCount] = 1.0f - (*itBuf).first;
					nLastIndex[nCount] = pWeight->back();

					nCount++;
				}

				pWeight->pop_back();
			}

			delete pWeight;
		}
	}

	for( i=0; i<nCount; i++)
		fSum += fLastWeight[i];

	if( fSum == 0.0f )
	{
		vResult.m_dwMatIndex = 0;

		for( i=0; i<WEIGHT_COUNT; i++)
			vResult.m_fWeight[i] = 0.0f;
	}
	else
	{
		for( i=0; i<nCount; i++)
			fLastWeight[i] /= fSum;

		for( i=0; i<nCount - 1; i++)
		{
			vResult.m_dwMatIndex |= ((DWORD) nLastIndex[i]) << (i*8);
			vResult.m_fWeight[i] = fLastWeight[i];
		}

		if( nCount - 1 >= 0 )
		{
			vResult.m_dwMatIndex |= ((DWORD) nLastIndex[nCount - 1]) << ((nCount - 1) * 8);

			if( nCount - 1 < WEIGHT_COUNT )
				vResult.m_fWeight[nCount - 1] = fLastWeight[nCount - 1];
		}
	}

	return vResult;
}

MESHVERTEX CTMath::Average( LPMESHVERTEX pPoint1, LPMESHVERTEX pPoint2)
{
	MESHVERTEX vResult;

	vResult.m_fNormalX = (pPoint1->m_fNormalX + pPoint2->m_fNormalX) / 2.0f;
	vResult.m_fNormalY = (pPoint1->m_fNormalY + pPoint2->m_fNormalY) / 2.0f;
	vResult.m_fNormalZ = (pPoint1->m_fNormalZ + pPoint2->m_fNormalZ) / 2.0f;
	vResult.m_fPosX = (pPoint1->m_fPosX + pPoint2->m_fPosX) / 2.0f;
	vResult.m_fPosY = (pPoint1->m_fPosY + pPoint2->m_fPosY) / 2.0f;
	vResult.m_fPosZ = (pPoint1->m_fPosZ + pPoint2->m_fPosZ) / 2.0f;
	vResult.m_fU1 = (pPoint1->m_fU1 + pPoint2->m_fU1) / 2.0f;
	vResult.m_fV1 = (pPoint1->m_fV1 + pPoint2->m_fV1) / 2.0f;
	vResult.m_fU2 = (pPoint1->m_fU2 + pPoint2->m_fU2) / 2.0f;
	vResult.m_fV2 = (pPoint1->m_fV2 + pPoint2->m_fV2) / 2.0f;

	return vResult;
}

void CTMath::ProgressMesh( LPVECTORDWORD pMESH,
						   LPVECTORPTR pDATA,
						   LPBYTE pCheck,
						   DWORD dwMeshCount,
						   DWORD dwFVF)
{
	DWORD dwPOINT = DWORD(pDATA->size());

	LPD3DXVECTOR3 pPOINT = new D3DXVECTOR3[dwPOINT];
	LPBYTE pMULTI = new BYTE[dwPOINT];
	MAPDWORD mapEDGE;

	memset( pMULTI, 0, dwPOINT * sizeof(BYTE));
	mapEDGE.clear();

	switch(dwFVF)
	{
	case T3DFVF_WMESHVERTEX	:
		for( DWORD i=0; i<dwPOINT; i++)
		{
			pPOINT[i] = D3DXVECTOR3(
				((LPWMESHVERTEX) (*pDATA)[i])->m_fPosX,
				((LPWMESHVERTEX) (*pDATA)[i])->m_fPosY,
				((LPWMESHVERTEX) (*pDATA)[i])->m_fPosZ);
		}

		break;

	case T3DFVF_MESHVERTEX	:
		for( DWORD i=0; i<dwPOINT; i++)
		{
			pPOINT[i] = D3DXVECTOR3(
				((LPMESHVERTEX) (*pDATA)[i])->m_fPosX,
				((LPMESHVERTEX) (*pDATA)[i])->m_fPosY,
				((LPMESHVERTEX) (*pDATA)[i])->m_fPosZ);
		}

		break;
	}

	for( DWORD i=0; i<dwPOINT; i++)
		if(!pMULTI[i])
			for( DWORD j=0; j<dwPOINT; j++)
				if( i != j && IsEqual( &pPOINT[i], &pPOINT[j]) )
				{
					pMULTI[i] = TRUE;
					pMULTI[j] = TRUE;
				}

	for( i=0; i<dwMeshCount; i++)
	{
		DWORD dwCount = DWORD(pMESH[i].size()) / 3;

		for( DWORD j=0; j<dwCount; j++)
			for( BYTE k=0; k<3; k++)
			{
				DWORD dwEDGE = MAKELONG(
					WORD(max( pMESH[i][3 * j + (k + 1) % 3], pMESH[i][3 * j + k])),
					WORD(min( pMESH[i][3 * j + (k + 1) % 3], pMESH[i][3 * j + k])));
				MAPDWORD::iterator finder = mapEDGE.find(dwEDGE);

				if( finder == mapEDGE.end() )
				{
					WORD wCheck = pCheck[i] ? TRUE : FALSE;
					WORD wCount = pCheck[i] ? 1 : 0;

					mapEDGE.insert( MAPDWORD::value_type( dwEDGE, MAKELONG( wCount, wCheck)));
				}
				else
				{
					WORD wCheck = HIWORD((*finder).second);
					WORD wCount = LOWORD((*finder).second);

					wCheck = wCheck || pCheck[i] ? TRUE : FALSE;
					if(pCheck[i])
						wCount++;

					(*finder).second = MAKELONG( wCount, wCheck);
				}
			}
	}

	for( i=0; i<dwMeshCount; i++)
	{
		DWORD dwCount = DWORD(pMESH[i].size()) / 3;

		for( DWORD j=0; j<dwCount; j++)
		{
			BYTE bCLEAR = FALSE;

			for( BYTE k=0; k<3; k++)
			{
				DWORD dwEDGE = MAKELONG(
					WORD(max( pMESH[i][3 * j + (k + 1) % 3], pMESH[i][3 * j + k])),
					WORD(min( pMESH[i][3 * j + (k + 1) % 3], pMESH[i][3 * j + k])));
				MAPDWORD::iterator finder = mapEDGE.find(dwEDGE);

				if( LOWORD((*finder).second) < 2 )
				{
					bCLEAR = TRUE;
					break;
				}
			}

			if(bCLEAR)
				for( k=0; k<3; k++)
				{
					DWORD dwEDGE = MAKELONG(
						WORD(max( pMESH[i][3 * j + (k + 1) % 3], pMESH[i][3 * j + k])),
						WORD(min( pMESH[i][3 * j + (k + 1) % 3], pMESH[i][3 * j + k])));

					MAPDWORD::iterator finder = mapEDGE.find(dwEDGE);
					WORD wCheck = HIWORD((*finder).second);
					WORD wCount = min( 1, LOWORD((*finder).second));

					(*finder).second = MAKELONG( wCount, wCheck);
				}
		}
	}

	MAPDWORD::iterator itEDGE;
	FLOAT fDIST = 0.0f;

	DWORD dwWEIGHT = 0;
	DWORD dwEDGE = 0;

	for( itEDGE = mapEDGE.begin(); itEDGE != mapEDGE.end(); itEDGE++)
	{
		WORD wCheck = HIWORD((*itEDGE).second);
		WORD wCount = LOWORD((*itEDGE).second);

		if(wCheck)
		{
			FLOAT fLOCAL = D3DXVec3Length(&(pPOINT[LOWORD((*itEDGE).first)] - pPOINT[HIWORD((*itEDGE).first)]));
			DWORD dwLOCAL = MAKELONG(
				wCount > 1 ? 0 : 1,
				!pMULTI[LOWORD((*itEDGE).first)] || !pMULTI[HIWORD((*itEDGE).first)] ? 0 : 1);

			if( !dwEDGE || dwLOCAL < dwWEIGHT || (dwLOCAL == dwWEIGHT && fLOCAL < fDIST) )
			{
				dwWEIGHT = dwLOCAL;
				fDIST = fLOCAL;

				dwEDGE = (*itEDGE).first;
			}
		}
	}

	if(dwEDGE)
	{
		WORD wPOINT = 0xFFFF;

		MAPDWORD mapDELETE;
		MAPDWORD mapPOINT;

		if(HIWORD(dwWEIGHT))
			memset( pMULTI, 0, dwPOINT * sizeof(BYTE));

		mapDELETE.clear();
		mapPOINT.clear();

		for( itEDGE = mapEDGE.begin(); itEDGE != mapEDGE.end(); itEDGE++)
			if((IsEqual( &pPOINT[LOWORD(dwEDGE)], &pPOINT[LOWORD((*itEDGE).first)]) &&
				IsEqual( &pPOINT[HIWORD(dwEDGE)], &pPOINT[HIWORD((*itEDGE).first)])) || (
				IsEqual( &pPOINT[LOWORD(dwEDGE)], &pPOINT[HIWORD((*itEDGE).first)]) &&
				IsEqual( &pPOINT[HIWORD(dwEDGE)], &pPOINT[LOWORD((*itEDGE).first)])))
				mapDELETE.insert( MAPDWORD::value_type( (*itEDGE).first, 0));

		switch(dwFVF)
		{
		case T3DFVF_WMESHVERTEX	:
			for( itEDGE = mapDELETE.begin(); itEDGE != mapDELETE.end(); itEDGE++)
			{
				WORD vPOINT[2] = {
					LOWORD((*itEDGE).first),
					HIWORD((*itEDGE).first)};
				WORD wINDEX = 0xFFFF;

				if(pMULTI[vPOINT[0]])
					wINDEX = vPOINT[0];
				else if(pMULTI[vPOINT[1]])
					wINDEX = vPOINT[1];
				else
				{
					LPWMESHVERTEX pNEW = new WMESHVERTEX();

					(*pNEW) = Average(
						(LPWMESHVERTEX) (*pDATA)[vPOINT[0]],
						(LPWMESHVERTEX) (*pDATA)[vPOINT[1]]);

					for( i=0; i<DWORD(pDATA->size()); i++)
						if(IsEqual( pNEW, (LPWMESHVERTEX) (*pDATA)[i]))
						{
							wINDEX = WORD(i);
							delete pNEW;

							break;
						}

					if( wINDEX == 0xFFFF )
					{
						wINDEX = WORD(pDATA->size());
						pDATA->push_back(pNEW);
					}
				}

				if( (*itEDGE).first == dwEDGE )
					wPOINT = wINDEX;
				(*itEDGE).second = DWORD(wINDEX);

				for( i=0; i<2; i++)
				{
					MAPDWORD::iterator finder = mapPOINT.find(vPOINT[i]);

					if( finder == mapPOINT.end() )
						mapPOINT.insert( MAPDWORD::value_type( vPOINT[i], wINDEX));
					else
						(*finder).second = DWORD(wINDEX);
				}
			}

			break;

		case T3DFVF_MESHVERTEX	:
			for( itEDGE = mapDELETE.begin(); itEDGE != mapDELETE.end(); itEDGE++)
			{
				WORD vPOINT[2] = {
					LOWORD((*itEDGE).first),
					HIWORD((*itEDGE).first)};
				WORD wINDEX = 0xFFFF;

				if(pMULTI[vPOINT[0]])
					wINDEX = vPOINT[0];
				else if(pMULTI[vPOINT[1]])
					wINDEX = vPOINT[1];
				else
				{
					LPMESHVERTEX pNEW = new MESHVERTEX();

					(*pNEW) = Average(
						(LPMESHVERTEX) (*pDATA)[vPOINT[0]],
						(LPMESHVERTEX) (*pDATA)[vPOINT[1]]);

					for( i=0; i<DWORD(pDATA->size()); i++)
						if(IsEqual( pNEW, (LPMESHVERTEX) (*pDATA)[i]))
						{
							wINDEX = WORD(i);
							delete pNEW;

							break;
						}

					if( wINDEX == 0xFFFF )
					{
						wINDEX = WORD(pDATA->size());
						pDATA->push_back(pNEW);
					}
				}

				if( (*itEDGE).first == dwEDGE )
					wPOINT = wINDEX;
				(*itEDGE).second = DWORD(wINDEX);

				for( i=0; i<2; i++)
				{
					MAPDWORD::iterator finder = mapPOINT.find(vPOINT[i]);

					if( finder == mapPOINT.end() )
						mapPOINT.insert( MAPDWORD::value_type( vPOINT[i], wINDEX));
					else
						(*finder).second = DWORD(wINDEX);
				}
			}

			break;
		}

		for( i=0; i<dwPOINT; i++)
			if( mapPOINT.find(i) == mapPOINT.end() && (
				IsEqual( &pPOINT[i], &pPOINT[LOWORD(dwEDGE)]) ||
				IsEqual( &pPOINT[i], &pPOINT[HIWORD(dwEDGE)])) )
				mapPOINT.insert( MAPDWORD::value_type( i, wPOINT));

		for( i=0; i<dwMeshCount; i++)
		{
			DWORD dwCount = DWORD(pMESH[i].size()) / 3;

			for( DWORD j=0; j<dwCount; j++)
				for( BYTE k=0; k<3; k++)
				{
					DWORD dwEDGE = MAKELONG(
						WORD(max( pMESH[i][3 * j + (k + 1) % 3], pMESH[i][3 * j + k])),
						WORD(min( pMESH[i][3 * j + (k + 1) % 3], pMESH[i][3 * j + k])));
					MAPDWORD::iterator finder = mapDELETE.find(dwEDGE);

					if( finder != mapDELETE.end() )
					{
						pMESH[i][3 * j + 2] = 0xFFFF;
						pMESH[i][3 * j + 1] = 0xFFFF;
						pMESH[i][3 * j] = 0xFFFF;

						break;
					}
					else
					{
						finder = mapPOINT.find(pMESH[i][3 * j + k]);

						if( finder != mapPOINT.end() )
							pMESH[i][3 * j + k] = WORD((*finder).second);
					}
				}

			VECTORDWORD::iterator itMESH = pMESH[i].begin();
			while(itMESH != pMESH[i].end())
				if( (*itMESH) == 0xFFFF )
					pMESH[i].erase(itMESH);
				else
					itMESH++;
		}

		mapDELETE.clear();
		mapPOINT.clear();
	}

	delete[] pPOINT;
	delete[] pMULTI;
	mapEDGE.clear();
}

D3DXMATRIX CTMath::GetTransformMatrix( LPD3DXMATRIX pBones,
									   LPD3DXMATRIX pLocal,
									   LPWMESHVERTEX pVertex)
{
	D3DXMATRIX vResult = pBones[0];

	if( pVertex->m_dwMatIndex > 0 )
	{
		int nIndex[WEIGHT_COUNT + 1] = {
			MATRIXID_1ST(pVertex->m_dwMatIndex),
			MATRIXID_2ND(pVertex->m_dwMatIndex),
			MATRIXID_3RD(pVertex->m_dwMatIndex),
			MATRIXID_4TH(pVertex->m_dwMatIndex)};

		D3DXMATRIX vLocal(
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f);
		FLOAT fWeight = 0.0f;

		for( int i=0; i<WEIGHT_COUNT; i++)
		{
			if(nIndex[i])
				vLocal += pVertex->m_fWeight[i] * (pLocal ? pLocal[nIndex[i] - 1] * pBones[nIndex[i]] : pBones[nIndex[i]]);

			fWeight += pVertex->m_fWeight[i];
		}

		if(nIndex[WEIGHT_COUNT])
			vLocal += (1.0f - fWeight) * (pLocal ? pLocal[nIndex[WEIGHT_COUNT] - 1] * pBones[nIndex[WEIGHT_COUNT]] : pBones[nIndex[WEIGHT_COUNT]]);

		return vLocal;
	}

	return vResult;
}

BYTE CTMath::GetPointPosition( LPD3DXVECTOR3 pPoint, LPD3DXPLANE pPlane)
{
	FLOAT fDet = D3DXPlaneDotCoord(
		pPlane,
		pPoint);

	if( fabs(fDet) < DEF_EPSILON )
		return POINTPOS_ONPLANE;

	return fDet < 0.0f ? POINTPOS_BACK : POINTPOS_FRONT;
}

BYTE CTMath::GetPointPosition( LPMESHVERTEX pPoint, LPD3DXPLANE pPlane)
{
	return GetPointPosition(
		&D3DXVECTOR3(
		pPoint->m_fPosX,
		pPoint->m_fPosY,
		pPoint->m_fPosZ),
		pPlane);
}

BYTE CTMath::GetWindingPosition( LPVERTEXWINDING pWinding, LPD3DXPLANE pPlane)
{
	BYTE bMain = POINTPOS_ONPLANE;

	for( int i=0; bMain == POINTPOS_ONPLANE && i<pWinding->m_vPoint.size(); i++)
	{
		bMain = GetPointPosition(
			pWinding->m_vPoint[i],
			pPlane);
	}

	if( bMain == POINTPOS_ONPLANE )
	{
		D3DXVECTOR3 vCross(
			pWinding->m_vPlane.a,
			pWinding->m_vPlane.b,
			pWinding->m_vPlane.c);

		D3DXVECTOR3 vNormal(
			pPlane->a,
			pPlane->b,
			pPlane->c);

		vNormal /= D3DXVec3Length(&vNormal);
		vCross /= D3DXVec3Length(&vCross);

		return D3DXVec3Dot( &vNormal, &vCross) < 0.0f ? WINDINGPOS_ONBACK : WINDINGPOS_ONFRONT;
	}

	for(; i<pWinding->m_vPoint.size(); i++)
	{
		BYTE bDet = GetPointPosition(
			pWinding->m_vPoint[i],
			pPlane);

		if( bDet != POINTPOS_ONPLANE &&
			bDet != bMain )
			return WINDINGPOS_NONE;
	}

	return bMain == POINTPOS_FRONT ? WINDINGPOS_FRONT : WINDINGPOS_BACK;
}

BYTE CTMath::GetWindingPosition( LPPOINTWINDING pWinding, LPD3DXPLANE pPlane)
{
	BYTE bMain = POINTPOS_ONPLANE;

	for( int i=0; bMain == POINTPOS_ONPLANE && i<pWinding->m_vPoint.size(); i++)
	{
		bMain = GetPointPosition(
			pWinding->m_vPoint[i],
			pPlane);
	}

	if( bMain == POINTPOS_ONPLANE )
	{
		D3DXVECTOR3 vCross(
			pWinding->m_vPlane.a,
			pWinding->m_vPlane.b,
			pWinding->m_vPlane.c);

		D3DXVECTOR3 vNormal(
			pPlane->a,
			pPlane->b,
			pPlane->c);

		vNormal /= D3DXVec3Length(&vNormal);
		vCross /= D3DXVec3Length(&vCross);

		return D3DXVec3Dot( &vNormal, &vCross) < 0.0f ? WINDINGPOS_ONBACK : WINDINGPOS_ONFRONT;
	}

	for(; i<pWinding->m_vPoint.size(); i++)
	{
		BYTE bDet = GetPointPosition(
			pWinding->m_vPoint[i],
			pPlane);

		if( bDet != POINTPOS_ONPLANE &&
			bDet != bMain )
			return WINDINGPOS_NONE;
	}

	return bMain == POINTPOS_FRONT ? WINDINGPOS_FRONT : WINDINGPOS_BACK;
}

void CTMath::SplitWinding( LPD3DXPLANE pPlane,
						   LPPOINTWINDING pWinding,
						   LPPOINTWINDING pFront,
						   LPPOINTWINDING pBack)
{
	while(!pFront->m_vPoint.empty())
	{
		delete pFront->m_vPoint.back();
		pFront->m_vPoint.pop_back();
	}

	while(!pBack->m_vPoint.empty())
	{
		delete pBack->m_vPoint.back();
		pBack->m_vPoint.pop_back();
	}

	pFront->m_vPlane = pWinding->m_vPlane;
	pBack->m_vPlane = pWinding->m_vPlane;

	switch(GetWindingPosition( pWinding, pPlane))
	{
	case WINDINGPOS_ONFRONT	:
	case WINDINGPOS_FRONT	:
		{
			for( int i=0; i<pWinding->m_vPoint.size(); i++)
				pFront->m_vPoint.push_back(pWinding->m_vPoint[i]);

			pWinding->m_vPoint.clear();
		}

		break;

	case WINDINGPOS_ONBACK	:
	case WINDINGPOS_BACK	:
		{
			for( int i=0; i<pWinding->m_vPoint.size(); i++)
				pBack->m_vPoint.push_back(pWinding->m_vPoint[i]);

			pWinding->m_vPoint.clear();
		}

		break;

	case WINDINGPOS_NONE	:
		{
			LPPOINTWINDING pCurrent = NULL;
			D3DXVECTOR3 vPoint;
			int nIndex = 0;

			switch(GetPointPosition( pWinding->m_vPoint[nIndex], pPlane))
			{
			case POINTPOS_FRONT		:
				{
					pFront->m_vPoint.push_back(pWinding->m_vPoint[nIndex]);

					pCurrent = pFront;
					nIndex++;
				}

				break;

			case POINTPOS_BACK		:
				{
					pBack->m_vPoint.push_back(pWinding->m_vPoint[nIndex]);

					pCurrent = pBack;
					nIndex++;
				}

				break;

			case POINTPOS_ONPLANE	:
				{
					BYTE bDet = POINTPOS_ONPLANE;
					VECTORPOINT vHead;

					vHead.clear();
					while( bDet == POINTPOS_ONPLANE )
					{
						vHead.push_back(pWinding->m_vPoint[nIndex]);
						nIndex++;

						bDet = GetPointPosition(
							pWinding->m_vPoint[nIndex],
							pPlane);
					}

					if( bDet == POINTPOS_FRONT )
					{
						pBack->m_vPoint.push_back(new D3DXVECTOR3(*vHead[0]));
						pCurrent = pFront;
					}
					else
					{
						pFront->m_vPoint.push_back(new D3DXVECTOR3(*vHead[0]));
						pCurrent = pBack;
					}

					for( int i=0; i<vHead.size(); i++)
						pCurrent->m_vPoint.push_back(vHead[i]);
					vHead.clear();
				}

				break;
			}

			while( nIndex < pWinding->m_vPoint.size() )
			{
				if( IntersectPlane(
					&vPoint,
					pWinding->m_vPoint[nIndex - 1],
					pWinding->m_vPoint[nIndex],
					pPlane))
				{
					pFront->m_vPoint.push_back(new D3DXVECTOR3(vPoint));
					pBack->m_vPoint.push_back(new D3DXVECTOR3(vPoint));

					pCurrent = pCurrent == pFront ? pBack : pFront;
				}

				switch(GetPointPosition( pWinding->m_vPoint[nIndex], pPlane))
				{
				case POINTPOS_FRONT		:
					{
						pFront->m_vPoint.push_back(pWinding->m_vPoint[nIndex]);

						pCurrent = pFront;
						nIndex++;
					}

					break;

				case POINTPOS_BACK		:
					{
						pBack->m_vPoint.push_back(pWinding->m_vPoint[nIndex]);

						pCurrent = pBack;
						nIndex++;
					}

					break;

				case POINTPOS_ONPLANE	:
					{
						BYTE bDet = POINTPOS_ONPLANE;

						while( bDet == POINTPOS_ONPLANE )
						{
							pCurrent->m_vPoint.push_back(pWinding->m_vPoint[nIndex]);
							nIndex++;

							if( nIndex >= pWinding->m_vPoint.size() )
								break;

							bDet = GetPointPosition(
								pWinding->m_vPoint[nIndex],
								pPlane);
						}

						switch(bDet)
						{
						case POINTPOS_FRONT		:
							if( pCurrent == pBack )
							{
								pFront->m_vPoint.push_back(new D3DXVECTOR3(*pBack->m_vPoint.back()));
								pCurrent = pFront;
							}

							break;

						case POINTPOS_BACK		:
							if( pCurrent == pFront )
							{
								pBack->m_vPoint.push_back(new D3DXVECTOR3(*pFront->m_vPoint.back()));
								pCurrent = pBack;
							}

							break;

						case POINTPOS_ONPLANE	:
							if( pCurrent == pFront )
							{
								pBack->m_vPoint.push_back(new D3DXVECTOR3(*pFront->m_vPoint.back()));
								pCurrent = pBack;
							}
							else
							{
								pFront->m_vPoint.push_back(new D3DXVECTOR3(*pBack->m_vPoint.back()));
								pCurrent = pFront;
							}

							break;
						}
					}

					break;
				}
			}

			if( IntersectPlane(
				&vPoint,
				pWinding->m_vPoint[pWinding->m_vPoint.size() - 1],
				pWinding->m_vPoint[0],
				pPlane))
			{
				pFront->m_vPoint.push_back(new D3DXVECTOR3(vPoint));
				pBack->m_vPoint.push_back(new D3DXVECTOR3(vPoint));
			}

			pWinding->m_vPoint.clear();
		}

		break;
	}
}

void CTMath::SplitWinding( LPD3DXPLANE pPlane,
						   LPVERTEXWINDING pWinding,
						   LPVERTEXWINDING pFront,
						   LPVERTEXWINDING pBack)
{
	while(!pFront->m_vPoint.empty())
	{
		delete pFront->m_vPoint.back();
		pFront->m_vPoint.pop_back();
	}

	while(!pBack->m_vPoint.empty())
	{
		delete pBack->m_vPoint.back();
		pBack->m_vPoint.pop_back();
	}

	pFront->m_nMeshIndex = pWinding->m_nMeshIndex;
	pBack->m_nMeshIndex = pWinding->m_nMeshIndex;

	pFront->m_vPlane = pWinding->m_vPlane;
	pBack->m_vPlane = pWinding->m_vPlane;

	pFront->m_bCheck = pWinding->m_bCheck;
	pBack->m_bCheck = pWinding->m_bCheck;

	switch(GetWindingPosition( pWinding, pPlane))
	{
	case WINDINGPOS_ONFRONT	:
	case WINDINGPOS_FRONT	:
		{
			for( int i=0; i<pWinding->m_vPoint.size(); i++)
				pFront->m_vPoint.push_back(pWinding->m_vPoint[i]);

			pWinding->m_vPoint.clear();
		}

		break;

	case WINDINGPOS_ONBACK	:
	case WINDINGPOS_BACK	:
		{
			for( int i=0; i<pWinding->m_vPoint.size(); i++)
				pBack->m_vPoint.push_back(pWinding->m_vPoint[i]);

			pWinding->m_vPoint.clear();
		}

		break;

	case WINDINGPOS_NONE	:
		{
			LPVERTEXWINDING pCurrent = NULL;
			MESHVERTEX vPoint;
			int nIndex = 0;

			switch(GetPointPosition( pWinding->m_vPoint[nIndex], pPlane))
			{
			case POINTPOS_FRONT		:
				{
					pFront->m_vPoint.push_back(pWinding->m_vPoint[nIndex++]);
					pCurrent = pFront;
				}

				break;

			case POINTPOS_BACK		:
				{
					pBack->m_vPoint.push_back(pWinding->m_vPoint[nIndex++]);
					pCurrent = pBack;
				}

				break;

			case POINTPOS_ONPLANE	:
				{
					BYTE bDet = POINTPOS_ONPLANE;
					VECTORVERTEX vHead;
					vHead.clear();

					while( bDet == POINTPOS_ONPLANE )
					{
						vHead.push_back(pWinding->m_vPoint[nIndex++]);
						bDet = GetPointPosition( pWinding->m_vPoint[nIndex], pPlane);
					}

					if( bDet == POINTPOS_FRONT )
					{
						LPMESHVERTEX pPoint = new MESHVERTEX;

						memcpy( pPoint, vHead[0], sizeof(MESHVERTEX));
						pBack->m_vPoint.push_back(pPoint);

						pCurrent = pFront;
					}
					else
					{
						LPMESHVERTEX pPoint = new MESHVERTEX;

						memcpy( pPoint, vHead[0], sizeof(MESHVERTEX));
						pFront->m_vPoint.push_back(pPoint);

						pCurrent = pBack;
					}

					for( int i=0; i<vHead.size(); i++)
						pCurrent->m_vPoint.push_back(vHead[i]);

					vHead.clear();
				}

				break;
			}

			while( nIndex < pWinding->m_vPoint.size() )
			{
				if( IntersectPlane(
					&vPoint,
					pWinding->m_vPoint[nIndex - 1],
					pWinding->m_vPoint[nIndex],
					pPlane))
				{
					LPMESHVERTEX pPoint = new MESHVERTEX;

					memcpy( pPoint, &vPoint, sizeof(MESHVERTEX));
					pFront->m_vPoint.push_back(pPoint);

					pPoint = new MESHVERTEX;
					memcpy( pPoint, &vPoint, sizeof(MESHVERTEX));
					pBack->m_vPoint.push_back(pPoint);

					pCurrent = pCurrent == pFront ? pBack : pFront;
				}

				switch(GetPointPosition( pWinding->m_vPoint[nIndex], pPlane))
				{
				case POINTPOS_FRONT		:
					{
						pFront->m_vPoint.push_back(pWinding->m_vPoint[nIndex++]);
						pCurrent = pFront;
					}

					break;

				case POINTPOS_BACK		:
					{
						pBack->m_vPoint.push_back(pWinding->m_vPoint[nIndex++]);
						pCurrent = pBack;
					}

					break;

				case POINTPOS_ONPLANE	:
					{
						BYTE bDet = POINTPOS_ONPLANE;

						while( bDet == POINTPOS_ONPLANE )
						{
							pCurrent->m_vPoint.push_back(pWinding->m_vPoint[nIndex++]);

							if( nIndex >= pWinding->m_vPoint.size() )
								break;

							bDet = GetPointPosition( pWinding->m_vPoint[nIndex], pPlane);
						}

						switch(bDet)
						{
						case POINTPOS_FRONT		:
							if( pCurrent == pBack )
							{
								LPMESHVERTEX pPoint = new MESHVERTEX;

								memcpy( pPoint, pBack->m_vPoint.back(), sizeof(MESHVERTEX));
								pFront->m_vPoint.push_back(pPoint);

								pCurrent = pFront;
							}

							break;

						case POINTPOS_BACK		:
							if( pCurrent == pFront )
							{
								LPMESHVERTEX pPoint = new MESHVERTEX;

								memcpy( pPoint, pFront->m_vPoint.back(), sizeof(MESHVERTEX));
								pBack->m_vPoint.push_back(pPoint);

								pCurrent = pBack;
							}

							break;

						case POINTPOS_ONPLANE	:
							if( pCurrent == pFront )
							{
								LPMESHVERTEX pPoint = new MESHVERTEX;

								memcpy( pPoint, pFront->m_vPoint.back(), sizeof(MESHVERTEX));
								pBack->m_vPoint.push_back(pPoint);

								pCurrent = pBack;
							}
							else
							{
								LPMESHVERTEX pPoint = new MESHVERTEX;

								memcpy( pPoint, pBack->m_vPoint.back(), sizeof(MESHVERTEX));
								pFront->m_vPoint.push_back(pPoint);

								pCurrent = pFront;
							}

							break;
						}
					}

					break;
				}
			}

			if( IntersectPlane(
				&vPoint,
				pWinding->m_vPoint[pWinding->m_vPoint.size() - 1],
				pWinding->m_vPoint[0],
				pPlane))
			{
				LPMESHVERTEX pPoint = new MESHVERTEX;

				memcpy( pPoint, &vPoint, sizeof(MESHVERTEX));
				pFront->m_vPoint.push_back(pPoint);

				pPoint = new MESHVERTEX;
				memcpy( pPoint, &vPoint, sizeof(MESHVERTEX));
				pBack->m_vPoint.push_back(pPoint);
			}

			pWinding->m_vPoint.clear();
		}

		break;
	}
}

void CTMath::CutWinding( LPD3DXPLANE pPlane, LPPOINTWINDING pWinding)
{
	switch(GetWindingPosition( pWinding, pPlane))
	{
	case WINDINGPOS_ONBACK	:
	case WINDINGPOS_BACK	:
		while(!pWinding->m_vPoint.empty())
		{
			delete pWinding->m_vPoint.back();
			pWinding->m_vPoint.pop_back();
		}

		break;

	case WINDINGPOS_NONE	:
		{
			VECTORPOINT vWinding;
			D3DXVECTOR3 vPoint;
			int nIndex;

			vWinding.clear();
			nIndex = 0;

			switch(GetPointPosition( pWinding->m_vPoint[nIndex], pPlane))
			{
			case POINTPOS_ONPLANE	:
			case POINTPOS_FRONT		:
				{
					vWinding.push_back(new D3DXVECTOR3(
						pWinding->m_vPoint[nIndex]->x,
						pWinding->m_vPoint[nIndex]->y,
						pWinding->m_vPoint[nIndex]->z));
					nIndex++;
				}

				break;

			case POINTPOS_BACK		: nIndex++; break;
			}

			while( nIndex < pWinding->m_vPoint.size() )
			{
				if( IntersectPlane(
					&vPoint,
					pWinding->m_vPoint[nIndex - 1],
					pWinding->m_vPoint[nIndex],
					pPlane))
					vWinding.push_back(new D3DXVECTOR3(vPoint));

				switch(GetPointPosition( pWinding->m_vPoint[nIndex], pPlane))
				{
				case POINTPOS_ONPLANE	:
				case POINTPOS_FRONT		:
					{
						vWinding.push_back(new D3DXVECTOR3(
							pWinding->m_vPoint[nIndex]->x,
							pWinding->m_vPoint[nIndex]->y,
							pWinding->m_vPoint[nIndex]->z));
						nIndex++;
					}

					break;

				case POINTPOS_BACK		: nIndex++; break;
				}
			}

			if( IntersectPlane(
				&vPoint,
				pWinding->m_vPoint[pWinding->m_vPoint.size() - 1],
				pWinding->m_vPoint[0],
				pPlane))
				vWinding.push_back(new D3DXVECTOR3(vPoint));

			while(!pWinding->m_vPoint.empty())
			{
				delete pWinding->m_vPoint.back();
				pWinding->m_vPoint.pop_back();
			}

			for( int i=0; i<vWinding.size(); i++)
				pWinding->m_vPoint.push_back(vWinding[i]);
			vWinding.clear();
		}

		break;
	}
}

void CTMath::CutWinding( LPD3DXPLANE pPlane, LPVERTEXWINDING pWinding)
{
	switch(GetWindingPosition( pWinding, pPlane))
	{
	case WINDINGPOS_ONBACK	:
	case WINDINGPOS_BACK	:
		while(!pWinding->m_vPoint.empty())
		{
			delete pWinding->m_vPoint.back();
			pWinding->m_vPoint.pop_back();
		}

		break;

	case WINDINGPOS_NONE	:
		{
			VECTORVERTEX vWinding;
			MESHVERTEX vPoint;
			int nIndex;

			vWinding.clear();
			nIndex = 0;

			switch(GetPointPosition( pWinding->m_vPoint[nIndex], pPlane))
			{
			case POINTPOS_ONPLANE	:
			case POINTPOS_FRONT		:
				{
					LPMESHVERTEX pPoint = new MESHVERTEX;

					memcpy( pPoint, pWinding->m_vPoint[nIndex], sizeof(MESHVERTEX));
					vWinding.push_back(pPoint);
					nIndex++;
				}

				break;

			case POINTPOS_BACK		: nIndex++; break;
			}

			while( nIndex < pWinding->m_vPoint.size() )
			{
				if( IntersectPlane(
					&vPoint,
					pWinding->m_vPoint[nIndex - 1],
					pWinding->m_vPoint[nIndex],
					pPlane))
				{
					LPMESHVERTEX pPoint = new MESHVERTEX;

					memcpy( pPoint, &vPoint, sizeof(MESHVERTEX));
					vWinding.push_back(pPoint);
				}

				switch(GetPointPosition( pWinding->m_vPoint[nIndex], pPlane))
				{
				case POINTPOS_ONPLANE	:
				case POINTPOS_FRONT		:
					{
						LPMESHVERTEX pPoint = new MESHVERTEX;

						memcpy( pPoint, pWinding->m_vPoint[nIndex], sizeof(MESHVERTEX));
						vWinding.push_back(pPoint);
						nIndex++;
					}

					break;

				case POINTPOS_BACK		: nIndex++; break;
				}
			}

			if( IntersectPlane(
				&vPoint,
				pWinding->m_vPoint[pWinding->m_vPoint.size() - 1],
				pWinding->m_vPoint[0],
				pPlane))
			{
				LPMESHVERTEX pPoint = new MESHVERTEX;

				memcpy( pPoint, &vPoint, sizeof(MESHVERTEX));
				vWinding.push_back(pPoint);
			}

			while(!pWinding->m_vPoint.empty())
			{
				delete pWinding->m_vPoint.back();
				pWinding->m_vPoint.pop_back();
			}

			for( int i=0; i<vWinding.size(); i++)
				pWinding->m_vPoint.push_back(vWinding[i]);
			vWinding.clear();
		}

		break;
	}
}

BYTE CTMath::IntersectPlane( LPMESHVERTEX pResult,
							 LPMESHVERTEX pBegin,
							 LPMESHVERTEX pEnd,
							 LPD3DXPLANE pPlane)
{
	BYTE bBegin = GetPointPosition(
		pBegin,
		pPlane);

	BYTE bEnd = GetPointPosition(
		pEnd,
		pPlane);

	if( bBegin == POINTPOS_ONPLANE ||
		bEnd == POINTPOS_ONPLANE ||
		bBegin == bEnd )
		return FALSE;

	D3DXVECTOR3 vPoint;
	FLOAT fRatio;

	PlaneIntersectLine(
		&vPoint,
		&D3DXVECTOR3( pBegin->m_fPosX, pBegin->m_fPosY, pBegin->m_fPosZ),
		&D3DXVECTOR3( pEnd->m_fPosX, pEnd->m_fPosY, pEnd->m_fPosZ),
		pPlane);

	fRatio = D3DXVec3Length(&(D3DXVECTOR3( pBegin->m_fPosX, pBegin->m_fPosY, pBegin->m_fPosZ) - vPoint));
	fRatio /= D3DXVec3Length(&(D3DXVECTOR3( pBegin->m_fPosX, pBegin->m_fPosY, pBegin->m_fPosZ) - D3DXVECTOR3( pEnd->m_fPosX, pEnd->m_fPosY, pEnd->m_fPosZ)));

	pResult->m_fNormalX = (1.0f - fRatio) * pBegin->m_fNormalX + fRatio * pEnd->m_fNormalX;
	pResult->m_fNormalY = (1.0f - fRatio) * pBegin->m_fNormalY + fRatio * pEnd->m_fNormalY;
	pResult->m_fNormalZ = (1.0f - fRatio) * pBegin->m_fNormalZ + fRatio * pEnd->m_fNormalZ;

	pResult->m_fU1 = (1.0f - fRatio) * pBegin->m_fU1 + fRatio * pEnd->m_fU1;
	pResult->m_fV1 = (1.0f - fRatio) * pBegin->m_fV1 + fRatio * pEnd->m_fV1;
	pResult->m_fU2 = (1.0f - fRatio) * pBegin->m_fU2 + fRatio * pEnd->m_fU2;
	pResult->m_fV2 = (1.0f - fRatio) * pBegin->m_fV2 + fRatio * pEnd->m_fV2;

	pResult->m_fPosX = vPoint.x;
	pResult->m_fPosY = vPoint.y;
	pResult->m_fPosZ = vPoint.z;

	return TRUE;
}

BYTE CTMath::IntersectPlane( LPD3DXVECTOR3 pResult,
							 LPD3DXVECTOR3 pBegin,
							 LPD3DXVECTOR3 pEnd,
							 LPD3DXPLANE pPlane)
{
	BYTE bBegin = GetPointPosition(
		pBegin,
		pPlane);

	BYTE bEnd = GetPointPosition(
		pEnd,
		pPlane);

	if( bBegin == POINTPOS_ONPLANE ||
		bEnd == POINTPOS_ONPLANE ||
		bBegin == bEnd )
		return FALSE;

	PlaneIntersectLine(
		pResult,
		pBegin,
		pEnd,
		pPlane);

	return TRUE;
}

LPVERTEXWINDING CTMath::CopyWinding( LPVERTEXWINDING pWinding)
{
	LPVERTEXWINDING pResult = new VERTEXWINDING;

	pResult->m_vPlane = pWinding->m_vPlane;
	pResult->m_vPoint.clear();

	for( int i=0; i<pWinding->m_vPoint.size(); i++)
	{
		LPMESHVERTEX pPoint = new MESHVERTEX();

		pPoint->m_fNormalX = pWinding->m_vPoint[i]->m_fNormalX;
		pPoint->m_fNormalY = pWinding->m_vPoint[i]->m_fNormalY;
		pPoint->m_fNormalZ = pWinding->m_vPoint[i]->m_fNormalZ;
		pPoint->m_fPosX = pWinding->m_vPoint[i]->m_fPosX;
		pPoint->m_fPosY = pWinding->m_vPoint[i]->m_fPosY;
		pPoint->m_fPosZ = pWinding->m_vPoint[i]->m_fPosZ;
		pPoint->m_fU1 = pWinding->m_vPoint[i]->m_fU1;
		pPoint->m_fV1 = pWinding->m_vPoint[i]->m_fV1;
		pPoint->m_fU2 = pWinding->m_vPoint[i]->m_fU2;
		pPoint->m_fV2 = pWinding->m_vPoint[i]->m_fV2;

		pResult->m_vPoint.push_back(pPoint);
	}

	return pResult;
}

LPPOINTWINDING CTMath::CopyWinding( LPPOINTWINDING pWinding)
{
	LPPOINTWINDING pResult = new POINTWINDING;

	pResult->m_vPlane = pWinding->m_vPlane;
	pResult->m_vPoint.clear();

	for( int i=0; i<pWinding->m_vPoint.size(); i++)
		pResult->m_vPoint.push_back(new D3DXVECTOR3(*pWinding->m_vPoint[i]));

	return pResult;
}

BYTE CTMath::GetProjectionPoint( LPD3DXVECTOR3 pTRESULT,
								 LPD3DXVECTOR3 pTPOINT,
								 LPD3DXVECTOR3 pTSTART,
								 LPD3DXVECTOR3 pTEND)
{
	D3DXVECTOR3 vP = (*pTPOINT) - (*pTSTART);
	D3DXVECTOR3 vA = (*pTEND) - (*pTSTART);

	FLOAT fDotA = D3DXVec3Dot( &vA, &vA);
	FLOAT fDotP = D3DXVec3Dot( &vP, &vA);

	if( fDotA == 0.0f )
		return FALSE;
	(*pTRESULT) = (*pTSTART) + fDotP * vA / fDotA;

	return TRUE;
}

BYTE CTMath::CheckCollision( LPD3DXPLANE pTPLANE,
							 LPD3DXVECTOR3 pTSTART,
							 LPD3DXVECTOR3 pTEND,
							 FLOAT fRadius,
							 FLOAT *pDIST)
{
	D3DXVECTOR3 vTSTART;
	D3DXVECTOR3 vTEND;
	D3DXPLANE vPLANE;

	D3DXPlaneNormalize(
		&vPLANE,
		pTPLANE);
	vPLANE.d -= fRadius + DEF_EPSILON;

	if( D3DXPlaneDotCoord( pTPLANE, pTSTART) * D3DXPlaneDotCoord( &vPLANE, pTEND) > 0.0f ||
		!D3DXPlaneIntersectLine( &vTSTART, &vPLANE, pTSTART, pTEND))
		return FALSE;

	vTEND = D3DXVECTOR3(
		vPLANE.a + pTEND->x,
		vPLANE.b + pTEND->y,
		vPLANE.c + pTEND->z);

	if(!D3DXPlaneIntersectLine( &vTEND, &vPLANE, pTEND, &vTEND))
		return FALSE;

	if(pDIST)
	{
		D3DXVECTOR3 vA = (*pTEND) - (*pTSTART);
		D3DXVECTOR3 vB = vTSTART - (*pTSTART);

		(*pDIST) = (D3DXVec3Dot( &vA, &vB) > 0.0f ? 1.0f : -1.0f) * D3DXVec3Length(&vB);
	}

	(*pTSTART) = vTSTART;
	(*pTEND) = vTEND;

	return TRUE;
}

BYTE CTMath::CheckCollision( LPVECTORPOINTWINDING pSRC,
							 LPVECTORPOINTWINDING pDEST)
{
	VECTORPOINTWINDING vDEST;
	vDEST.clear();

	for( int i=0; i<pDEST->size(); i++)
		vDEST.push_back(CopyWinding((*pDEST)[i]));

	for( i=0; i<pSRC->size(); i++)
		for( int j=0; j<vDEST.size(); j++)
			if(!vDEST[j]->m_vPoint.empty())
				CutWinding( &(*pSRC)[i]->m_vPlane, vDEST[j]);

	BYTE bResult = FALSE;
	for( i=0; i<vDEST.size(); i++)
		if(!vDEST[i]->m_vPoint.empty())
		{
			bResult = TRUE;
			break;
		}

	while(!vDEST.empty())
	{
		delete vDEST.back();
		vDEST.pop_back();
	}

	return bResult;
}

void CTMath::BuildBoundWinding( LPVECTORPOINTWINDING pResult,
							    LPD3DXMATRIX pTrans,
								LPD3DXVECTOR3 pMinPoint,
								LPD3DXVECTOR3 pMaxPoint)
{
	static int nIndex[6][4] = {
		{ 0, 1, 2, 3},
		{ 7, 6, 5, 4},
		{ 0, 4, 5, 1},
		{ 1, 5, 6, 2},
		{ 2, 6, 7, 3},
		{ 3, 7, 4, 0}};

	D3DXVECTOR3 vBound[8] = {
		D3DXVECTOR3( pMinPoint->x, pMinPoint->y, pMinPoint->z),
		D3DXVECTOR3( pMinPoint->x, pMinPoint->y, pMaxPoint->z),
		D3DXVECTOR3( pMaxPoint->x, pMinPoint->y, pMaxPoint->z),
		D3DXVECTOR3( pMaxPoint->x, pMinPoint->y, pMinPoint->z),
		D3DXVECTOR3( pMinPoint->x, pMaxPoint->y, pMinPoint->z),
		D3DXVECTOR3( pMinPoint->x, pMaxPoint->y, pMaxPoint->z),
		D3DXVECTOR3( pMaxPoint->x, pMaxPoint->y, pMaxPoint->z),
		D3DXVECTOR3( pMaxPoint->x, pMaxPoint->y, pMinPoint->z)};

	for( int i=0; i<8; i++)
	{
		Transform(
			pTrans,
			&vBound[i]);
	}

	for( i=0; i<6; i++)
	{
		LPPOINTWINDING pWinding = new POINTWINDING;

		for( int j=0; j<4; j++)
			pWinding->m_vPoint.push_back(new D3DXVECTOR3(vBound[nIndex[i][j]]));

		D3DXPlaneFromPoints(
			&pWinding->m_vPlane,
			pWinding->m_vPoint[0],
			pWinding->m_vPoint[1],
			pWinding->m_vPoint[2]);

		pResult->push_back(pWinding);
	}
}

void CTMath::DeleteWinding( LPVECTORPOINTWINDING pWinding)
{
	while(!pWinding->empty())
	{
		while(!pWinding->back()->m_vPoint.empty())
		{
			delete pWinding->back()->m_vPoint.back();
			pWinding->back()->m_vPoint.pop_back();
		}

		delete pWinding->back();
		pWinding->pop_back();
	}
}

BYTE CTMath::PlaneIntersectLine( LPD3DXVECTOR3 pResult,
								 LPD3DXVECTOR3 pBegin,
								 LPD3DXVECTOR3 pEnd,
								 LPD3DXPLANE pPlane)
{
	if(IsEqual( pBegin, pEnd))
		return FALSE;

	D3DXVECTOR3 vDirection = (*pEnd) - (*pBegin);
	D3DXPLANE vTPLANE = (*pPlane);

	D3DXPlaneNormalize(
		&vTPLANE,
		&vTPLANE);
	vDirection /= D3DXVec3Length(&vDirection);

	FLOAT fDotNormal = D3DXPlaneDotNormal( &vTPLANE, &vDirection);
	FLOAT fDotCoord = D3DXPlaneDotCoord( &vTPLANE, pBegin);

	if( fabs(fDotNormal) < DEF_EPSILON )
		return FALSE;
	(*pResult) = (*pBegin) - fDotCoord * vDirection / fDotNormal;

	return TRUE;
}

BYTE CTMath::IsCut( LPD3DXPLANE pPlane, LPPOINTWINDING pWinding)
{
	BYTE bFIRST = D3DXPlaneDotCoord( pPlane, pWinding->m_vPoint[0]) < 0.0f ? FALSE : TRUE;

	for( int i=1; i<INT(pWinding->m_vPoint.size()); i++)
		if( bFIRST != (D3DXPlaneDotCoord( pPlane, pWinding->m_vPoint[i]) < 0.0f ? FALSE : TRUE) )
			return TRUE;

	return FALSE;
}

void CTMath::GetRotationQUAT( LPD3DXQUATERNION pTQUAT,
							  LPD3DXMATRIX pTMAT)
{
	D3DXVECTOR3 vSCALE;
	D3DXVECTOR3 vTRANS;

	D3DXMatrixDecompose(
		&vSCALE,
		pTQUAT,
		&vTRANS,
		pTMAT);

	D3DXQuaternionNormalize(
		pTQUAT,
		pTQUAT);
}

D3DXMATRIX CTMath::Inverse( LPD3DXMATRIX pMAT)
{
	D3DXMATRIX vResult;
	FLOAT fDET;

	D3DXMatrixInverse(
		&vResult,
		&fDET,
		pMAT);

	return vResult;
}

void CTMath::GetUVMatrix( LPD3DXMATRIX pRESULT, LPUVKEY pKEY)
{
	D3DXMATRIX vSCALE(
		pKEY->m_fKeySU, 0.0f, 0.0f, 0.0f,
		0.0f, pKEY->m_fKeySV, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	D3DXMATRIX vTRANS;

	D3DXMatrixIdentity(pRESULT);
	D3DXMatrixIdentity(&vTRANS);

	pRESULT->_11 = (FLOAT) cosf(pKEY->m_fKeyR);
	pRESULT->_12 = (FLOAT) -sinf(pKEY->m_fKeyR);
	pRESULT->_21 = -pRESULT->_12;
	pRESULT->_22 = pRESULT->_11;
	vTRANS._31 = -0.5f;
	vTRANS._32 = -0.5f;
	(*pRESULT) = vTRANS * (*pRESULT);

	vTRANS._31 = 0.5f;
	vTRANS._32 = 0.5f;
	(*pRESULT) *= vSCALE;
	(*pRESULT) *= vTRANS;

	vTRANS._31 = pKEY->m_fKeyU;
	vTRANS._32 = pKEY->m_fKeyV;
	(*pRESULT) *= vTRANS;
}

void CTMath::GetBasisMatrix( LPD3DXMATRIX pRESULT,
							 LPD3DXVECTOR3 pPOINT,
							 LPD3DXVECTOR3 pDIR)
{
	D3DXVECTOR3 vAxisZ = (*pDIR) / D3DXVec3Length(pDIR);
	D3DXVECTOR3 vAxisY( 0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 vAxisX;
	D3DXMATRIX vRESULT;

	D3DXVec3Cross(
		&vAxisX,
		&vAxisY,
		&vAxisZ);

	D3DXVec3Cross(
		&vAxisY,
		&vAxisZ,
		&vAxisX);

	vAxisX /= D3DXVec3Length(&vAxisX);
	vAxisY /= D3DXVec3Length(&vAxisY);

	D3DXMatrixTranslation(
		&vRESULT,
		-pPOINT->x,
		-pPOINT->y,
		-pPOINT->z);

	pRESULT->_11 = vAxisY.x;
	pRESULT->_21 = vAxisY.y;
	pRESULT->_31 = vAxisY.z;
	pRESULT->_41 = 0.0f;

	pRESULT->_12 = vAxisZ.x;
	pRESULT->_22 = vAxisZ.y;
	pRESULT->_32 = vAxisZ.z;
	pRESULT->_42 = 0.0f;

	pRESULT->_13 = vAxisX.x;
	pRESULT->_23 = vAxisX.y;
	pRESULT->_33 = vAxisX.z;
	pRESULT->_43 = 0.0f;

	pRESULT->_14 = 0.0f;
	pRESULT->_24 = 0.0f;
	pRESULT->_34 = 0.0f;
	pRESULT->_44 = 1.0f;

	(*pRESULT) = vRESULT * (*pRESULT);
}

BYTE CTMath::HitPoly( LPD3DXVECTOR3 pPOLY,
					  LPD3DXVECTOR3 pRECT,
					  LPD3DXPLANE pBOUND,
					  FLOAT *pDIST)
{
	if( pPOLY[0].y <= 0.0f &&
		pPOLY[1].y <= 0.0f &&
		pPOLY[2].y <= 0.0f )
		return FALSE;

	D3DXVECTOR3 vNORMAL;
	BYTE bInit = FALSE;

	D3DXPLANE vBOUND[3];
	D3DXPLANE vPLANE;

	D3DXPlaneFromPoints(
		&vPLANE,
		&pPOLY[0],
		&pPOLY[1],
		&pPOLY[2]);

	vNORMAL = D3DXVECTOR3(
		vPLANE.a,
		vPLANE.b,
		vPLANE.c);
	vNORMAL /= D3DXVec3Length(&vNORMAL);

	if( vNORMAL.y > -DEF_EPSILON )
		return FALSE;

	if( !pBOUND || !pRECT )
	{
		FLOAT fU;
		FLOAT fV;

		return D3DXIntersectTri(
			&pPOLY[0],
			&pPOLY[1],
			&pPOLY[2],
			&D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
			&D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
			&fU, &fV,
			pDIST);
	}

	for( BYTE i=0; i<4; i++)
	{
		BYTE bRESULT = TRUE;

		for( BYTE j=0; j<3; j++)
			if( D3DXPlaneDotCoord( &pBOUND[i], &pPOLY[j]) < 0.0f )
			{
				bRESULT = FALSE;
				break;
			}

		if(bRESULT)
			return FALSE;
	}

	for( i=0; i<3; i++)
	{
		D3DXPlaneFromPoints(
			&vBOUND[i],
			&pPOLY[i],
			&D3DXVECTOR3(
			pPOLY[i].x,
			pPOLY[i].y + 1.0f,
			pPOLY[i].z),
			&pPOLY[(i + 1) % 3]);
	}

	for( i=0; i<3; i++)
	{
		BYTE bRESULT = TRUE;

		for( BYTE j=0; j<4; j++)
			if( D3DXPlaneDotCoord( &vBOUND[i], &pRECT[j]) < 0.0f )
			{
				bRESULT = FALSE;
				break;
			}

		if(bRESULT)
			return FALSE;
	}

	for( i=0; i<4; i++)
	{
		BYTE bRESULT = TRUE;

		for( BYTE j=0; j<3; j++)
			if( D3DXPlaneDotCoord( &vBOUND[j], &pRECT[i]) >= 0.0f )
			{
				bRESULT = FALSE;
				break;
			}

		if(bRESULT)
		{
			FLOAT fLOCAL = -(vPLANE.a * pRECT[i].x + vPLANE.c * pRECT[i].z + vPLANE.d) / vPLANE.b;

			if( fLOCAL >= 0.0f && (!bInit || fLOCAL < (*pDIST)) )
			{
				(*pDIST) = fLOCAL;
				bInit = TRUE;
			}
		}

		for( j=0; j<3; j++)
			if( D3DXPlaneDotCoord( &vBOUND[j], &pRECT[i]) * D3DXPlaneDotCoord( &vBOUND[j], &pRECT[(i + 1) % 4]) < 0.0f &&
				D3DXPlaneDotCoord( &pBOUND[i], &pPOLY[j]) * D3DXPlaneDotCoord( &pBOUND[i], &pPOLY[(j + 1) % 3]) < 0.0f )
			{
				D3DXVECTOR3 vPOINT;

				if(PlaneIntersectLine( &vPOINT, &pPOLY[j], &pPOLY[(j + 1) % 3], &pBOUND[i]))
				{
					FLOAT fLOCAL = -(vPLANE.a * vPOINT.x + vPLANE.c * vPOINT.z + vPLANE.d) / vPLANE.b;

					if( fLOCAL >= 0.0f && (!bInit || fLOCAL < (*pDIST)) )
					{
						(*pDIST) = fLOCAL;
						bInit = TRUE;
					}
				}
			}
	}

	for( i=0; i<3; i++)
	{
		BYTE bRESULT = TRUE;

		for( BYTE j=0; j<4; j++)
			if( D3DXPlaneDotCoord( &pBOUND[j], &pPOLY[i]) >= 0.0f )
			{
				bRESULT = FALSE;
				break;
			}

		if(bRESULT)
		{
			FLOAT fLOCAL = -(vPLANE.a * pPOLY[i].x + vPLANE.c * pPOLY[i].z + vPLANE.d) / vPLANE.b;

			if( fLOCAL >= 0.0f && (!bInit || fLOCAL < (*pDIST)) )
			{
				(*pDIST) = fLOCAL;
				bInit = TRUE;
			}
		}
	}

	return bInit;
}

FLOAT CTMath::GetPITCH( LPD3DXVECTOR3 pDIR)
{
	if( !pDIR || pDIR->y == 0.0f )
		return 0.0f;

	FLOAT fPITCH = D3DXVec3Length(pDIR);
	if( fPITCH == 0.0f )
		return 0.0f;

	fPITCH = atanf(-pDIR->y / fPITCH) * 18000 / D3DX_PI;
	while( fPITCH < 0.0f )
		fPITCH += 36000;

	fPITCH = FLOAT(INT(fPITCH) % 36000);
	fPITCH /= 100.0f;

	return fPITCH;
}

FLOAT CTMath::GetYAW( LPD3DXVECTOR3 pDIR)
{
	if( !pDIR || (pDIR->x == 0.0f && pDIR->z == 0.0f) )
		return 0.0f;

	FLOAT fYAW = atan2f( -pDIR->x, -pDIR->z) * 18000 / D3DX_PI;
	while( fYAW < 0.0f )
		fYAW += 36000;

	fYAW = FLOAT(INT(fYAW) % 36000);
	fYAW /= 100.0f;

	return fYAW;
}
