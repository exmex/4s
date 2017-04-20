#include "stdafx.h"
#include "TSiegeGodBall.h"

CTGodBall::MAP_RESOURCE_BALL CTGodBall::m_mapResBall;

//////////////////////////////////////////////////////////////////////////
//	AUCTION COMMANDER CONSTRUCTOR & DESTRUCTOR.
CTGodBall::CTGodBall()
:	m_wBallID( 0 ),
	m_bInGround( FALSE ),
	m_bCamp( 0 ),
	m_bValid( TRUE ),
	m_pBallInfo( NULL )
{
	m_bType = OT_GODBALL;
	
	m_bCanSelected = TRUE;	
	m_bDynamicOBJ = FALSE;
	m_bDrawName = TRUE;

	m_bHouseMesh = 0;
	m_dwHouseID = 0;

	m_mapResBall.clear();
}

CTGodBall::~CTGodBall()
{
	Release();
}
//	END OF AUCTION COMMANDER CONSTRUCTOR & DESTRUCTOR.
//////////////////////////////////////////////////////////////////////////

/*! FindObject */
LPOBJECT	CTGodBall::FindObject( DWORD dwObjID, CTachyonRes* pResource )
{
	MAP_BALL_ITOR itor = m_mapResBall.find( dwObjID );
	if( itor != m_mapResBall.end() )	//!< 이미 한번찾았던 오브젝트면..
	{
		return (*itor).second;
	}
	else
	{
		MAPRES::iterator itor = pResource->m_mapOBJ.find( dwObjID );

		if( itor != pResource->m_mapOBJ.end() )
		{
			m_mapResBall.insert( make_pair( dwObjID, (LPOBJECT)(*itor).second ) );
			return (LPOBJECT)(*itor).second;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//	MEMBER FUNCTION IMPLEMENTATION.

/*! InitGodBall */
BOOL	CTGodBall::InitGodBall( WORD wBallID, CD3DDevice* pDevice, CTachyonRes* pResource )
{
	WORD wChartID = (WORD)(( wBallID + 1.5f ) / 2.0f);
	m_pBallInfo = CTChart::FindTGODBALL( wChartID );

	if( m_pBallInfo )
	{
		m_wBallID = wBallID;

		LPOBJECT pObject = FindObject( m_pBallInfo->m_dwOBJID, pResource );
		if( pObject )
		{
			ClearInven();
			InitOBJ( pObject );
			ClearEquip();

			SetCloth( pDevice, ID_CLK_MAPOBJ, ID_CL_MAPOBJ, ID_MESH_0 );
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	StartBallAction();

	ResetOBJPart( pDevice );

	m_bCollisionType = GetAttrINT(ID_COLLISION_TYPE);

	InitSIZE(
		GetAttrFLOAT(ID_SIZE_X),
		GetAttrFLOAT(ID_SIZE_Y),
		GetAttrFLOAT(ID_SIZE_Z),
		GetAttrFLOAT(ID_DEPTH),
		GetAttrFLOAT(ID_SIGHT));

	D3DXMatrixRotationY(
		&m_vWorld,
		m_pBallInfo->m_fRot * 5.0f * D3DX_PI / 900.0f );

	D3DXMatrixScaling(
		&m_vScale,
		1.0f,
		1.0f,
		1.0f);

	SetGroundPosition( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

	m_vPosition = m_vScale * m_vWorld;

	return TRUE;
}

/*!  */
void	CTGodBall::AddGodBall( BYTE bCamp, D3DXVECTOR3& vPosition, BOOL bInGround )
{
	m_bInGround	= bInGround;

	SetValid( TRUE );
	SetCamp( bCamp );
	StartBallAction();
	SetGroundPosition( vPosition );
}

/*!  */
void	CTGodBall::TakeGodBall()
{
	m_bInGround = FALSE;
}

/*!  */
void	CTGodBall::DelGodBall()
{
	m_bInGround	= FALSE;
	SetValid( FALSE );
	SetAction( 0, 0 );
}

void	CTGodBall::StartBallAction()
{
	/*! 수성측. 임시..
	*	캐릭터GSF에서 공격, 수비 오브젝트에서
	*	서로 액션ID가 다르게 되어있기때문에.. 나중에 수정해야함.
	*/
	if( m_wBallID >= 1 && m_wBallID <= 4 )
	{
		SetAction( ID_ACT_DEFAULT, ID_ANI_STAND_NORMAL );
	}
	else
	{
		SetAction( ID_STAND_NORMAL, ID_ANI_STAND_NORMAL );
	}
}

void	CTGodBall::Release()
{
	m_mapResBall.clear();
}

//	END OF MEMBER FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	VIRTUAL FUNCTION IMPLEMENTATION.

CString	CTGodBall::GetName()
{
	if( m_pBallInfo != NULL )
		return m_pBallInfo->m_strName;

	return "";
}

//	END OF VIRTUAL FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	INLINE FUNCTION IMPLEMENTATION.
WORD	CTGodBall::GetBallID() const
{
	return m_wBallID;
}

BOOL	CTGodBall::InGround() const
{
	return m_bInGround;
}

BYTE	CTGodBall::GetCamp() const
{
	return m_bCamp;
}

LPTGODBALL	CTGodBall::GetBallInfo() const
{
	return m_pBallInfo;
}

BOOL	CTGodBall::IsValid() const
{
	return m_bValid;
}

BOOL	CTGodBall::IsVisible() const
{
	return IsValid() && InGround();
}

void	CTGodBall::SetBallID( WORD wBallID )
{
	m_wBallID = wBallID;
}

void	CTGodBall::SetCamp( BYTE bCamp )
{
	m_bCamp = bCamp;
}

void	CTGodBall::SetGroundPosition( D3DXVECTOR3& vPosition )
{
	m_vWorld._41 = vPosition.x;
	m_vWorld._42 = vPosition.y;
	m_vWorld._43 = vPosition.z;
}

void	CTGodBall::SetValid( BOOL bValid )
{
	m_bValid = bValid;
}

//	END OF INLINE FUNCTION.
//////////////////////////////////////////////////////////////////////////
