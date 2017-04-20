#ifndef __EXPORTCOMM__H
#define __EXPORTCOMM__H


#include <atlstr.h>
#include <d3dx9.h>
#include <decomp.h>
#include <vector>
#include <list>
#include <map>

using namespace std;

typedef struct tagTMESHBUFFER					TMESHBUFFER, *LPTMESHBUFFER;
typedef struct tagWEIGHT						WEIGHT, *LPWEIGHT;

typedef struct tagINODEDATA						INODEDATA, *LPINODEDATA;
typedef struct tagBONESNODE						BONESNODE, *LPBONESNODE;
typedef struct tagMESHNODE						MESHNODE, *LPMESHNODE;
typedef struct tagMESHDATA						MESHDATA, *LPMESHDATA;

typedef struct tagTPOINTKEY						TPOINTKEY, *LPTPOINTKEY;
typedef struct tagTROTKEY						TROTKEY, *LPTROTKEY;
typedef struct tagTSCALEKEY						TSCALEKEY, *LPTSCALEKEY;

#define ALMOST_ZERO								1.0e-3f
#define WEIGHT_COUNT							3

typedef vector<LPTMESHBUFFER>					VTMESHBUFFER, *LPVTMESHBUFFER;
typedef vector<LPMESHDATA>						VMESHDATA, *LPVMESHDATA;
typedef vector<WORD>							VWORD, *LPVWORD;

typedef list<LPINODEDATA>						LISTINODEDATA, *LPLISTINODEDATA;
typedef list<LPMESHNODE>						LISTMESHNODE, *LPLISTMESHNODE;
typedef list<LPWEIGHT>							LISTWEIGHT, *LPLISTWEIGHT;

typedef map< CString, DWORD>					MAPSTRING, *LPMAPSTRING;
typedef map< DWORD, IKey *>						MAPIKEY, *LPMAPIKEY;

typedef enum __TEX_CHANNEL
{
	S_DI = 0,
	S_DK,
	S_COUNT
} TEX_CHANNEL, *LPTEX_CHANNEL;


struct tagTMESHBUFFER
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	FLOAT m_fWeight[WEIGHT_COUNT];
	DWORD m_dwMatIndex;

	FLOAT m_fNormalX;
	FLOAT m_fNormalY;
	FLOAT m_fNormalZ;

	FLOAT m_fU1;
	FLOAT m_fV1;
	FLOAT m_fU2;
	FLOAT m_fV2;
};

struct tagWEIGHT
{
	FLOAT m_fWeight;
	BYTE m_bINDEX;

	tagWEIGHT()
	{
		m_fWeight = 0.0f;
		m_bINDEX = 0;
	};
};

struct tagBONESNODE
{
	BYTE m_bParentROT;
	BYTE m_bParentPOS;
	BYTE m_bINDEX;

	tagBONESNODE()
	{
		m_bParentROT = 0;
		m_bParentPOS = 0;
		m_bINDEX = 0;
	};

	~tagBONESNODE()
	{
	};
};

struct tagMESHNODE
{
	VWORD m_vMESH;
	Mtl *m_pMTRL;

	tagMESHNODE()
	{
		m_vMESH.clear();
		m_pMTRL = NULL;
	};

	~tagMESHNODE()
	{
		m_vMESH.clear();
		m_pMTRL = NULL;
	};
};

struct tagMESHDATA
{
	VTMESHBUFFER m_vMESH;
	INode *m_pINODE;

	tagMESHDATA()
	{
		m_vMESH.clear();
		m_pINODE = NULL;
	};

	~tagMESHDATA()
	{
		while(!m_vMESH.empty())
		{
			delete m_vMESH.back();
			m_vMESH.pop_back();
		}

		m_pINODE = NULL;
	};
};

struct tagINODEDATA
{
	INode *m_pINODE;

	BYTE m_bINDEX;
	BYTE m_bNODE;

	tagINODEDATA()
	{
		m_pINODE = NULL;
		m_bINDEX = 0;
		m_bNODE = 0;
	};

	~tagINODEDATA()
	{
	};
};

struct tagTPOINTKEY
{
	DWORD m_dwTime;
	D3DXVECTOR3 m_vKeyPoint;
};

struct tagTROTKEY
{
	DWORD m_dwTime;
	D3DXQUATERNION m_vKeyQuat;
};

struct tagTSCALEKEY
{
	DWORD m_dwTime;
	D3DXVECTOR3 m_vKeyScale;
};


#endif // __EXPORTCOMM__H
