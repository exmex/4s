#pragma once


///////////////////////////////////////////////////////////////////////////
// TMHBuilder macro

#define DEF_INDEXCNT					(36000)
#define DEF_LODBOUND					(58)
#define DEF_DIFFUSECNT					(10)
#define DEF_MAXLEVEL					(10)
#define DEF_MAXLENGTH					(4)

#define TCAM_LENGTH						((FLOAT) 110.0f)
#define THEIGHT_ZERO					((FLOAT) 0.01f)

#define TWATERGRID_SIZE					(8)
#define TGRID_SIZE						(1)


///////////////////////////////////////////////////////////////////////////
// TMHBuilder enum define

typedef enum TCOLLISION_TYPE
{
	TCOLLISION_NONE = 0,
	TCOLLISION_BOX,
	TCOLLISION_CYLINDER,
	TCOLLISION_POLY,
	TCOLLISION_COUNT
} *LPTCOLLISION_TYPE;


///////////////////////////////////////////////////////////////////////////
// TMHBuilder structure type

typedef struct tagTMAPITEM					TMAPITEM, *LPTMAPITEM;


///////////////////////////////////////////////////////////////////////////
// TMHBuilder class type

class CTMHBuilderMAP;
class CTMHBuilderOBJ;


///////////////////////////////////////////////////////////////////////////
// TMHBuilder vector type

typedef vector<WORD>						VWORD, *LPVWORD;


///////////////////////////////////////////////////////////////////////////
// TMHBuilder map type

typedef map< DWORD, LPVECTORDWORD>			MAPTHEIGHTINFO, *LPMAPTHEIGHTINFO;
typedef map< DWORD, CTMHBuilderOBJ *>		MAPTMHOBJ, *LPMAPTMHOBJ;
typedef map< WORD, LPTMAPITEM>				MAPTMAPITEM, *LPMAPTMAPITEM;


///////////////////////////////////////////////////////////////////////////
// TMHBuilder class define

#include "TMHBuilderMap.h"
#include "TMHBuilderOBJ.h"


///////////////////////////////////////////////////////////////////////////
// TMHBuilder structure define

struct tagTMAPITEM
{
	VWORD m_vUNIT;
	BYTE m_bAll;

	tagTMAPITEM()
	{
		m_vUNIT.clear();
		m_bAll = FALSE;
	};

	~tagTMAPITEM()
	{
		m_vUNIT.clear();
	};
};
