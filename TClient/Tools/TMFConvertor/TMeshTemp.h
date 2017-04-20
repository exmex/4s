#pragma once

#define TARGET_TMF_VERSION 1
#define TMF_VERSION 300

typedef struct tagVBDATA											VBDATA, *LPVBDATA;
typedef vector<LPVBDATA>											VECTORVBDATA, *LPVECTORVBDATA;
typedef vector<D3DXMATRIX>											VECTORBONE, *LPVECTORBONE;

class CTMeshTemp
{
public:
	CTMeshTemp(void);
	~CTMeshTemp(void);
	void Release();

public:
	int			m_nTMFVersion;
	DWORD		m_dwNodeCount;
	FLOAT		m_fRadius;

	D3DXVECTOR3	 m_vCenterPoint;
	LPVECTORBONE m_pBones;

	LPVECTORIBDATA m_pMESHIBDATA;
	LPVECTORIB	m_pMESHIB;

	LPVBDATA	m_pDATA;

	DWORD		m_dwMeshCount;
	LPVECTORPTR m_pMESH;
	LPIBDATA	m_pINDEX;
	DWORD*		m_dwVBCnt;
	int		m_nMESHCNT;
};	