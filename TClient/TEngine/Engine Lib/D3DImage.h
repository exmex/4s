// D3DImage.h: interface for the CD3DImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined ___D3DIMAGE_H
#define ___D3DIMAGE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CD3DImage
{
protected:
	CT3DTexture **m_pT3DTEX;
	CT3DVertex **m_pT3DVB;
	LPBYTE m_pMask;
	BYTE m_bUseIMGBUF;

	int m_nPartCount;
	int m_nHeight;
	int m_nWidth;

	float** m_ppPosBufX;
	float** m_ppPosBufY;

	int m_nPosX;
	int m_nPosY;

	float m_fScaleX;
	float m_fScaleY;
	float m_fPrvScaleX;
	float m_fPrvScaleY;

public:
	static LPT3DDATA CreateDXT(
		LPDIRECT3DDEVICE9 pDevice,
		LPDIRECT3DSURFACE9 pImage,
		BYTE bDXT);

	static LPT3DDATA CreateDXT(
		LPDIRECT3DDEVICE9 pDevice,
		LPDIRECT3DTEXTURE9 pTexture,
		BYTE bDXT);

	static DWORD m_dwScreenX;
	static DWORD m_dwScreenY;
	static BOOL m_bUseGlobalClipRect;

public:
	CT3DTexture** GetT3DTEX() { return m_pT3DTEX;};
	CT3DVertex** GetT3DVB() { return m_pT3DVB;};
	LPBYTE GetMask() { return m_pMask;};

	int GetPartCount() { return m_nPartCount;};
	int GetHeight() { return m_nHeight;};
	int GetWidth() { return m_nWidth;};
	int GetPosX() { return m_nPosX;};
	int GetPosY() { return m_nPosY;};

	float GetScaleX() { return m_fScaleX; }
	float GetScaleY() { return m_fScaleY; }
	void SetScaleX(float fScale);
	void SetScaleY(float fScale);

	LPT3DDATA BuildImageData( LPDIRECT3DDEVICE9 pDevice, BYTE bFormat=NON_COMP );
	LPDIRECT3DSURFACE9 CreateOffscreenPlainSurface( LPDIRECT3DDEVICE9 pDevice);

	BOOL GetMask( int nPosX, int nPosY);
	BOOL IsEnable();

	BYTE LoadImageFile(
		LPDIRECT3DDEVICE9 pDevice,
		LPBYTE pResData,
		UINT nLength);

	void SaveImageFile(
		LPDIRECT3DDEVICE9 pDevice,
		LPCSTR strFileName, 
		D3DXIMAGE_FILEFORMAT destFormat);

	void Load(
		LPMAPT3DTEXTURE pIMGBUF,
		LPBYTE pResData);

	void Load(
		LPBYTE pResData,
		BYTE bFormat = NON_COMP);

	void Release();
	void Render(
		LPDIRECT3DDEVICE9 pDevice,
		DWORD dwColor,
		int nPosX,
		int nPosY,
		LPRECT pRectSRC = NULL);

public:
	CD3DImage();
	virtual ~CD3DImage();
};


#endif // !defined ___D3DIMAGE_H
