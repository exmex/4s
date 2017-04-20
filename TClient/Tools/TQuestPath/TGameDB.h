#pragma once


class CTGameDB
{
public:
	MAPTNDATA m_mapTNDATA;
	VTMDATA m_vTMDATA;

public:
	MAPMAPTMDATA m_mapTMONIDINDEX;
	MAPMAPTMDATA m_mapTKINDINDEX;

public:
	LPTNDATA FindTNDATA( DWORD dwID);

	LPMAPTMDATA FindMAPTMDATA(
		LPMAPMAPTMDATA pINDEX,
		DWORD dwID);

	LPVTMDATA FindVTMDATA(
		LPMAPTMDATA pMAPTMDATA,
		DWORD dwMapID);

public:
	void ClearINDEX( LPMAPMAPTMDATA pINDEX);

	void AddToINDEX(
		LPMAPMAPTMDATA pINDEX,
		DWORD dwID,
		LPTMDATA pTMDATA);

	void WriteDATA( CArchive& vAR);
	void ReadDATA( CArchive& vAR);

	void ReleaseDATA();
	BYTE LoadDATA();

public:
	CTGameDB();
	virtual ~CTGameDB();
};
