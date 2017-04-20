// TachyonAnimation.h: interface for the CTachyonAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __TACHYONANIMATION_H
#define __TACHYONANIMATION_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTachyonAnimation : public CObject
{
public:
	void InitPositionKey( LPBONESANIMATION pNode);
	void InitRotationKey( LPBONESANIMATION pNode);
	void InitScaleKey( LPBONESANIMATION pNode);

	virtual DWORD GetInterval();
	BOOL IsEnable();

	virtual BOOL GetSize(
		D3DXVECTOR3& vMin,
		D3DXVECTOR3& vMax);

	BOOL LoadAnimation( CString strFileName);
	BOOL LoadAnimation( LPBYTE pData);

	virtual int HitTest(
		LPD3DXMATRIX pBones,
		CD3DCamera *pCam,
		int nPosX,
		int nPosY);

	void GetBlendKEY(
		LPD3DXQUATERNION pBlendKEY,
		LPD3DXMATRIX pPREV,
		LPD3DXMATRIX pNEXT);

	virtual void GetFrameMatrix(
		LPD3DXMATRIX pResult,
		LPD3DXMATRIX pBlend,
		LPD3DXQUATERNION pBlendKEY,
		LPTPIVOT pPivot,
		D3DXMATRIX& vWorld,
		BYTE bRootID,
		FLOAT fTimeValue,
		FLOAT fBlendTime);

	void Render(
		LPDIRECT3DDEVICE9 pDevice,
		LPTPIVOT pPivot,
		D3DXMATRIX& vWorld,
		FLOAT fTimeValue);

	void Render(
		LPDIRECT3DDEVICE9 pDevice,
		int nIndex);

	void Lock( LPVECTORSTRING pFILE);
	void Release();
	void Unlock();

	void SetNodeCount( DWORD dwNodeCount ) { m_dwNodeCount=dwNodeCount; }
	virtual DWORD GetNodeCount() { return m_dwNodeCount; }

protected:
	void GetFrameMatrix(
		LPD3DXMATRIX pResult,
		LPTPIVOT pPivot,
		BYTE bNODE,
		FLOAT fTimeValue);

	D3DXQUATERNION GetReviseROT(
		LPD3DXVECTOR3 pPREV,
		LPD3DXVECTOR3 pNEXT);

	D3DXVECTOR3 CalcPositionVector(
		LPBONESANIMATION pNode,
		FLOAT fTimeValue);

	D3DXQUATERNION CalcRotation(
		LPBONESANIMATION pNode,
		FLOAT fTimeValue);

	D3DXVECTOR3 CalcScale(
		LPBONESANIMATION pNode,
		FLOAT fTimeValue);

	BYTE IsValidNode(
		LPBONESNODE pNode,
		LPBYTE pFILTER);

private:
	DWORD m_dwNodeCount;

public:
	BYTE m_bFileID;

	DWORD m_dwSIZE;
	DWORD m_dwPOS;
	DWORD m_dwREF;

	LPBONESANIMATION m_pNode;
	LPD3DXMATRIX m_pBones;
	LPBONESNODE m_pTREE;
	LPVBDATA m_pDATA;
	LPMESHVB m_pVB;

	DWORD m_dwStartTick;
	DWORD m_dwEndTick;

public:
	CTachyonAnimation();
	virtual ~CTachyonAnimation();
};


#endif // !defined __TACHYONANIMATION_H
