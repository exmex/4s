// D3DFont.h: interface for the CD3DFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined ___D3DFONT_H
#define ___D3DFONT_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CD3DFont
{
public:
	static int m_nRenderingHint;

protected:
	LPDIRECT3DVERTEXBUFFER9 *m_pVB3D;
	LPDIRECT3DVERTEXBUFFER9 *m_pVB;
	LPDIRECT3DTEXTURE9 *m_pTEX[2];

	CString m_strTEXT;
	CFont *m_pFont;
	UINT m_nFormat;

	int m_nPartCount;
	int m_nHeight;
	int m_nWidth;
	int m_nPosX;
	int m_nPosY;

	float** m_ppPosBufX;
	float** m_ppPosBufY;

	float m_fScaleX;
	float m_fScaleY;
	float m_fPrvScaleX;
	float m_fPrvScaleY;

public:
	DWORD m_dwLineColor;
	DWORD m_dwColor;

	BYTE m_bOutLine;
	BYTE m_bShadow;

protected:
	WORD GetColorByLinePixel(
		LPDWORD pBits,
		int nPosX, 
		int nPosY);

public:
	void SetFont( CFont *pFont);
	void Release();

	void MakeText(
		LPDIRECT3DDEVICE9 pDevice,
		CString strText,
		CRect& rect,
		UINT nFormat = 0);

	void TextOut(
		LPDIRECT3DDEVICE9 pDevice,
		CString strText,
		CRect& rect,
		UINT nFormat = 0,
		BOOL bOpaque = FALSE);

	void TextOut(
		LPDIRECT3DDEVICE9 pDevice,
		CString strText,
		int nPosX,
		int nPosY,
		BOOL bOpaque = FALSE);

	void TextOut(
		LPDIRECT3DDEVICE9 pDevice,
		D3DXMATRIX& vWorld,
		CString strText,
		FLOAT fTextHeight = 0.3f,
		UINT nFormat = 0);

	int GetWidth() { return m_nWidth; };
	int GetHeight() { return m_nHeight; };

	float GetScaleX() { return m_fScaleX; }
	float GetScaleY() { return m_fScaleY; }
	void SetScaleX(float fScale);
	void SetScaleY(float fScale);

public:
	CD3DFont();
	virtual ~CD3DFont();
};


#endif // !defined ___D3DFONT_H
