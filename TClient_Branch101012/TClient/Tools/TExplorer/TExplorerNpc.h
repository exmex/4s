#pragma once


class CTExplorerNpc : public CTExplorerObjBase
{
public:
	LPTNPCTEMP m_pTNPC;

	CString m_strTITLE;
	CString m_strNAME;

public:
	void InitTNPC(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		LPTNPC pTNPC);

public:
	virtual void ReleaseData();
	virtual void Release();

	virtual CString GetTitle();
	virtual CString GetName();

public:
	CTExplorerNpc();
	virtual ~CTExplorerNpc();
};
