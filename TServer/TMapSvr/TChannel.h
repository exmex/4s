#pragma once


class CTChannel
{
public:
	MAPDWORD m_mapTINSTANCEMAPID;
	MAPTMAP m_mapTMAP;
	BYTE m_bChannel;

public:
	CTMap *CreateInstanceMAP(
		WORD wPartyID,
		WORD wMapID,
		BYTE bCountryID);
	void DeleteInstanceMAP( CTMap *pTMAP);

	CTMap *FindTMap(
		WORD wPartyID,
		WORD wMapID);

	WORD GetInstanceMapID(
		WORD wPartyID,
		WORD wMapID);

public:
	void DelAllMonAtSpawn( LPTMONSPAWN pSPAWN);
	void DelMonSpawn( LPTMONSPAWN pSPAWN);
	void InitChannel(
		CSqlDatabase *pDB,
		BYTE bServerID);

	void AddMonSpawn(
		LPTMONSPAWN pSPAWN,
		BYTE bCountry);

public:
	CTChannel();
	virtual ~CTChannel();
};
