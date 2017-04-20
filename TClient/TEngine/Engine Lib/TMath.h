// TMath.h: interface for the CTMath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __TMATH_H
#define __TMATH_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTMath
{
public:
	static void DeleteWinding(
		LPVECTORPOINTWINDING pWinding);

	static void BuildBoundWinding(
		LPVECTORPOINTWINDING pResult,
		LPD3DXMATRIX pTrans,
		LPD3DXVECTOR3 pMinPoint,
		LPD3DXVECTOR3 pMaxPoint);

	static BYTE GetProjectionPoint(
		LPD3DXVECTOR3 pTRESULT,
		LPD3DXVECTOR3 pTPOINT,
		LPD3DXVECTOR3 pTSTART,
		LPD3DXVECTOR3 pTEND);

	static BYTE CheckCollision(
		LPD3DXPLANE pTPLANE,
		LPD3DXVECTOR3 pTSTART,
		LPD3DXVECTOR3 pTEND,
		FLOAT fRadius,
		FLOAT *pDIST);

	static BYTE CheckCollision(
		LPVECTORPOINTWINDING pSRC,
		LPVECTORPOINTWINDING pDEST);

	static LPVERTEXWINDING CopyWinding(
		LPVERTEXWINDING pWinding);

	static LPPOINTWINDING CopyWinding(
		LPPOINTWINDING pWinding);

	static BYTE GetPointPosition(
		LPD3DXVECTOR3 pPoint,
		LPD3DXPLANE pPlane);

	static BYTE GetPointPosition(
		LPMESHVERTEX pPoint,
		LPD3DXPLANE pPlane);

	static BYTE GetWindingPosition(
		LPVERTEXWINDING pWinding,
		LPD3DXPLANE pPlane);

	static BYTE GetWindingPosition(
		LPPOINTWINDING pWinding,
		LPD3DXPLANE pPlane);

	static void SplitWinding(
		LPD3DXPLANE pPlane,
		LPPOINTWINDING pWinding,
		LPPOINTWINDING pFront,
		LPPOINTWINDING pBack);

	static void SplitWinding(
		LPD3DXPLANE pPlane,
		LPVERTEXWINDING pWinding,
		LPVERTEXWINDING pFront,
		LPVERTEXWINDING pBack);

	static void CutWinding(
		LPD3DXPLANE pPlane,
		LPPOINTWINDING pWinding);

	static void CutWinding(
		LPD3DXPLANE pPlane,
		LPVERTEXWINDING pWinding);

	static BYTE PlaneIntersectLine(
		LPD3DXVECTOR3 pResult,
		LPD3DXVECTOR3 pBegin,
		LPD3DXVECTOR3 pEnd,
		LPD3DXPLANE pPlane);

	static BYTE IntersectPlane(
		LPMESHVERTEX pResult,
		LPMESHVERTEX pBegin,
		LPMESHVERTEX pEnd,
		LPD3DXPLANE pPlane);

	static BYTE IntersectPlane(
		LPD3DXVECTOR3 pResult,
		LPD3DXVECTOR3 pBegin,
		LPD3DXVECTOR3 pEnd,
		LPD3DXPLANE pPlane);

	static D3DXQUATERNION MaxSquad(
		D3DXQUATERNION& vStart,
		D3DXQUATERNION& vA,
		D3DXQUATERNION& vB,
		D3DXQUATERNION& vEnd,
		FLOAT fTime);

	static D3DXQUATERNION MaxSlerp(
		D3DXQUATERNION& vStart,
		D3DXQUATERNION& vEnd,
		FLOAT fTime);

	static D3DXQUATERNION LnDiff(
		D3DXQUATERNION& vA,
		D3DXQUATERNION& vB);

	static D3DXMATRIX Inverse(
		LPD3DXMATRIX pMAT);

	static D3DXMATRIX GetTransformMatrix(
		LPD3DXMATRIX pBones,
		LPD3DXMATRIX pLocal,
		LPWMESHVERTEX pVertex);

	static WMESHVERTEX Average(
		LPWMESHVERTEX pPoint1,
		LPWMESHVERTEX pPoint2);

	static MESHVERTEX Average(
		LPMESHVERTEX pPoint1,
		LPMESHVERTEX pPoint2);

	static BYTE IsCut(
		LPD3DXPLANE pPlane,
		LPPOINTWINDING pWinding);

	static BYTE IsEqual(
		LPMESHVERTEX pPoint1,
		LPMESHVERTEX pPoint2);

	static BYTE IsEqual(
		LPWMESHVERTEX pPoint1,
		LPWMESHVERTEX pPoint2);

	static BYTE IsEqual(
		LPD3DXVECTOR3 pPoint1,
		LPD3DXVECTOR3 pPoint2);

	static BYTE IsEqual( 
		FLOAT l,
		FLOAT r);

	static void Transform(
		LPD3DXMATRIX pMatrix,
		LPD3DXVECTOR3 pVector);

	static void ProgressMesh(
		LPVECTORDWORD pMESH,
		LPVECTORPTR pDATA,
		LPBYTE pCheck,
		DWORD dwMeshCount,
		DWORD dwFVF);

	static FLOAT GetPITCH( LPD3DXVECTOR3 pDIR);
	static FLOAT GetYAW( LPD3DXVECTOR3 pDIR);

	static FLOAT GetTimeValue( DWORD dwTick);
	static DWORD GetTimeTick( FLOAT fTime);
	static int GetNearPow( int nValue);

	static void GetRotationQUAT(
		LPD3DXQUATERNION pTQUAT,
		LPD3DXMATRIX pTMAT);

	static void GetUVMatrix(
		LPD3DXMATRIX pRESULT,
		LPUVKEY pKEY);

	static void GetBasisMatrix(
		LPD3DXMATRIX pRESULT,
		LPD3DXVECTOR3 pPOINT,
		LPD3DXVECTOR3 pDIR);

	static BYTE HitPoly(
		LPD3DXVECTOR3 pPOLY,
		LPD3DXVECTOR3 pRECT,
		LPD3DXPLANE pBOUND,
		FLOAT *pDIST);

public:
	CTMath();
	virtual ~CTMath();
};


#endif // !defined __TMATH_H
