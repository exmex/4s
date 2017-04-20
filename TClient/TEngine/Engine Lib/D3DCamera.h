// D3DCamera.h: interface for the CD3DCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined ___D3DCAMERA_H
#define ___D3DCAMERA_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CD3DCamera
{
public:
	BOOL IsOrthoCamera();

	BOOL InitOrthoCamera(
		LPDIRECT3DDEVICE9 pDevice,
		FLOAT fNearPlane,
		FLOAT fFarPlane,
		FLOAT fWidth,
		FLOAT fHeight);

	BOOL InitCamera(
		LPDIRECT3DDEVICE9 pDevice,
		FLOAT fNearPlane,
		FLOAT fFarPlane,
		FLOAT fFovV,
		int nWidth,
		int nHeight);

	void SetPosition(
		D3DXVECTOR3& vPosition,
		D3DXVECTOR3& vTarget,
		D3DXVECTOR3& vUp,
		BOOL bMove = TRUE);

	void ZoomOrthoCamera( FLOAT fZoom);
	void Activate( BOOL bInit = FALSE);
	void ResetFOV( FLOAT fFOV);

	void Move(
		LPD3DXPLANE pPlane,
		FLOAT fAxisX,
		FLOAT fAxisY,
		BOOL bAbsAxis = FALSE);

	void Move(
		FLOAT fAxisX,
		FLOAT fAxisY,
		FLOAT fAxisZ,
		BOOL bAbsAxis = FALSE);

	void Rotate(
		FLOAT fYaw,
		FLOAT fPitch,
		FLOAT fRoll,
		BOOL bAbsAxis = FALSE);

	D3DXVECTOR3 GetRayDirection(
		int nPosX,
		int nPosY);

	D3DXVECTOR3 GetPlanePoint(
		LPD3DXPLANE pPlane,
		int nPosX,
		int nPosY);

	D3DXVECTOR3 GetCameraPosition(
		int nPosX,
		int nPosY);

	D3DXMATRIX GetWorldMatrix(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

public:
	D3DXMATRIX m_matProjection;
	D3DXMATRIX m_matInvView;
	D3DXMATRIX m_matView;

	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vTarget;
	D3DXVECTOR3 m_vUp;

	FLOAT m_fNearPlane;
	FLOAT m_fFarPlane;

	FLOAT m_fDotFOV;
	FLOAT m_fAspect;
	FLOAT m_fFOV;

	FLOAT m_fHeight;
	FLOAT m_fWidth;

	D3DXVECTOR3 m_vAxisZ;
	D3DXVECTOR3 m_vAxisY;
	D3DXVECTOR3 m_vAxisX;

	LPDIRECT3DDEVICE9 m_pDevice;

public:
	CD3DCamera();
	virtual ~CD3DCamera();
};


#endif // !defined ___D3DCAMERA_H
