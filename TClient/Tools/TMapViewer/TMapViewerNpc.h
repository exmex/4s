#pragma once


class CTMapViewerNpc : public CTMapViewerObjBase
{
public:
	static MAPNPCTEMP m_mapTNPCTEMP;		// NPC template data

public:
	static LPTNPCTEMP FindTNPC( WORD wTempID);

public:
	LPTNPCTEMP m_pTNPC;

	CString m_strTITLE;
	CString m_strNAME;

public:
	void InitTNPC(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

public:
	virtual void ReleaseData();
	virtual void Release();

	virtual CString GetTitle();
	virtual CString GetName();

public:
	CTMapViewerNpc();
	virtual ~CTMapViewerNpc();
};
