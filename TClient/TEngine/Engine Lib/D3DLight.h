// D3DLight.h: interface for the CD3DLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined ___D3DLIGHT_H
#define ___D3DLIGHT_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CD3DLight
{
public:
	static void SetDevice( LPDIRECT3DDEVICE9 pDevice);

protected:
	static LPDIRECT3DDEVICE9 m_pDevice;
	static QUEUEDWORD m_queueFreeLight;
	static MAPDWORD m_mapUsedLight;
	static DWORD m_dwNextIndex;
	static BOOL m_bInit;

	static DWORD PickLightIndex();

	static void ReleaseLightIndex( DWORD dwIndex);
	static void ReleaseD3DLight();
	static void InitD3DLight();

public:
	void SetLight( D3DLIGHT9 *pLight = NULL);
	void EnableLight( BOOL bEnable = TRUE);

	void SetPosition(
		LPD3DXVECTOR3 pPOS,
		LPD3DXVECTOR3 pDIR);

	void Move(
		FLOAT fMoveX,
		FLOAT fMoveY,
		FLOAT fMoveZ);

	void Rotate(
		LPD3DXVECTOR3 pAxis,
		FLOAT fAngle);

	D3DLIGHT9 m_Light;

protected:
	void Release();

protected:
	DWORD m_dwIndex;

public:
	CD3DLight();
	virtual ~CD3DLight();
};


#endif // !defined ___D3DLIGHT_H
