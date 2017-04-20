#pragma once


class CTQuestDB
{
public:
	MAPVDWORD m_mapTMONITEMDATA;
	MAPVDWORD m_mapTNPCITEMDATA;

	MAPTQDATA m_mapTQDATA;
	MAPTCDATA m_mapTCDATA;

public:
	static void ClearMAPVDWORD( LPMAPVDWORD pTDATA);

	static LPVDWORD FindVDWORD(
		LPMAPVDWORD pTITEMDATA,
		DWORD dwItemID);

	static void MergeVDWORD(
		LPMAPVDWORD pTFROM,
		LPMAPVDWORD pTINTO);

public:
	LPTQDATA FindTMISSION( LPTQDATA pTQDATA);
	LPTQDATA FindTQDATA( DWORD dwQuestID);
	LPTCDATA FindTCDATA( DWORD dwClassID);

	void ReleaseDATA();
	BYTE LoadDATA();

	BYTE LoadTTERM(
		LPTQDATA pTQDATA,
		CDatabase *pDB);

public:
	CTQuestDB();
	virtual ~CTQuestDB();
};
