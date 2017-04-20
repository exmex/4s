#pragma once


class CTDynamicHelpDlg : public CTClientUIBase
{
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DTEXTURE9 *m_pTEX;

	LPWORD m_pHEIGHT;
	LPWORD m_pWIDTH;

	DWORD m_dwTRES;
	DWORD m_dwTPOS;

	int m_nPartCount;
	int m_nHEIGHT;
	int m_nWIDTH;

public:
	DWORD m_dwTSPEED;

public:
	void InitTEXT( CString& strTEXT);
	void Release();

public:
	virtual HRESULT Render( DWORD dwTickCount);

public:
	CTDynamicHelpDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTDynamicHelpDlg();
};
