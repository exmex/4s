#pragma once


struct tagPOINTWINDING
{
	VECTORPOINT m_vPoint;
	D3DXPLANE m_vPlane;

	tagPOINTWINDING()
	{
		m_vPoint.clear();
	}

	~tagPOINTWINDING()
	{
		while(!m_vPoint.empty())
		{
			delete m_vPoint.back();
			m_vPoint.pop_back();
		}
	}
};


class CTBSPPortal
{
public:
	static BYTE m_bBasePortalID;

	DWORD m_dwFrontID;
	DWORD m_dwBackID;
	DWORD m_dwNodeID;
	DWORD m_dwID;

	POINTWINDING m_vFace;
	BYTE m_bDirection;

	MAPDWORD m_mapFrontPVS;
	MAPDWORD m_mapBackPVS;

protected:
	static MAPTBSPPORTAL m_mapTBSPPortal;
	static DWORD m_dwPortalID;

	static void GetNewID( CTBSPPortal *pItem);
	static void ReleaseID( DWORD dwID);

public:
	static CTBSPPortal *GetPortal( DWORD dwID);

	static void ReleaseCommon();
	static void InitCommon();

	BOOL SetID( DWORD dwID);
	BOOL AddPVS(
		DWORD dwNodeID,
		BYTE bDirection);

public:
	CTBSPPortal( CTBSPPortal *pItem);
	CTBSPPortal();

	virtual ~CTBSPPortal();
};
