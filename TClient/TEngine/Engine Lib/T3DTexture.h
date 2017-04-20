#pragma once


class CT3DTexture
{
public:
	LPBYTE m_pTDATA;
	BYTE m_bEnabled;

public:
	LPDIRECT3DTEXTURE9 GetTEX();

	void LoadT3DTEX(
		LPBYTE pDATA,
		DWORD dwDATA,
		DWORD dwSIZE,
		BYTE bFormat);
	void ReleaseDATA();

public:
	CT3DTexture();
	virtual ~CT3DTexture();
};


class CT3DVertex
{
public:
	VTLOADBUF m_vTDATA;

	LPBYTE m_pTDATA;
	BYTE m_bEnabled;

public:
	LPDIRECT3DVERTEXBUFFER9 GetVB();

	void LoadT3DVBDATA(
		LPBYTE pDATA,
		DWORD dwSIZE,
		DWORD dwPOS);

	void LoadT3DVB(
		DWORD dwSIZE,
		DWORD dwUSE,
		DWORD dwFVF);
	void ReleaseDATA();

protected:
	BYTE UpdateT3DVB(
		LPBYTE pDATA,
		DWORD dwSIZE,
		DWORD dwPOS);

	BYTE CreateVB(
		DWORD dwSIZE,
		DWORD dwUSE,
		DWORD dwFVF);

public:
	CT3DVertex();
	virtual ~CT3DVertex();
};


class CT3DIndex
{
public:
	VTLOADBUF m_vTDATA;

	LPBYTE m_pTDATA;
	BYTE m_bEnabled;

public:
	LPDIRECT3DINDEXBUFFER9 GetIB();

	void LoadT3DIBDATA(
		LPBYTE pDATA,
		DWORD dwSIZE,
		DWORD dwPOS);

	void LoadT3DIB(
		DWORD dwSIZE,
		DWORD dwUSE,
		D3DFORMAT nFORMAT);
	void ReleaseDATA();

protected:
	BYTE UpdateT3DIB(
		LPBYTE pDATA,
		DWORD dwSIZE,
		DWORD dwPOS);

	BYTE CreateIB(
		DWORD dwSIZE,
		DWORD dwUSE,
		D3DFORMAT nFORMAT);

public:
	CT3DIndex();
	virtual ~CT3DIndex();
};


class CT3DImage
{
public:
	LPBYTE m_pTDATA;
	BYTE m_bEnabled;

public:
	CD3DImage *GetIMG();

	void LoadT3DIMG(
		LPBYTE pDATA,
		DWORD dwSIZE);
	void ReleaseDATA();

public:
	CT3DImage();
	virtual ~CT3DImage();
};
