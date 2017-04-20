#include "stdafx.h"
#include "TSiegeGodTower.h"

CTGodTower::MAP_RESOURCE_TOWER CTGodTower::m_mapResTower;

//////////////////////////////////////////////////////////////////////////
//	AUCTION COMMANDER CONSTRUCTOR & DESTRUCTOR.
CTGodTower::CTGodTower()
:	m_wTowerID( 0 ),
	m_bCamp( 0 ),
	m_bValid( TRUE ),
    m_pTowerInfo( NULL )
{
	m_bType = OT_GODTOWER;

	m_bCanSelected = TRUE;	
	m_bDynamicOBJ = FALSE;
	m_bDrawName = TRUE;

	m_bHouseMesh = 0;
	m_dwHouseID = 0;

	m_mapResTower.clear();
}

CTGodTower::~CTGodTower()
{
	Release();
}
//	END OF AUCTION COMMANDER CONSTRUCTOR & DESTRUCTOR.
//////////////////////////////////////////////////////////////////////////

/*! Find Tower Object */
LPOBJECT	CTGodTower::FindObject( DWORD dwObjID, CTachyonRes* pResource )
{
	MAP_TOWER_ITOR itor = m_mapResTower.find( dwObjID );
	if( itor != m_mapResTower.end() )	//!< 이미 한번찾았던 오브젝트면..
	{
		return (*itor).second;
	}
	else
	{
		MAPRES::iterator itor = pResource->m_mapOBJ.find( dwObjID );

		if( itor != pResource->m_mapOBJ.end() )
		{
			m_mapResTower.insert( make_pair( dwObjID, (LPOBJECT)(*itor).second ) );
			return (LPOBJECT)(*itor).second;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//	MEMBER FUNCTION IMPLEMENTATION.

BOOL	CTGodTower::InitTower( WORD wTowerID, D3DXVECTOR3& vPosition, CD3DDevice* pDevice, CTachyonRes* pResource )
{
	m_pTowerInfo = CTChart::FindTGODTOWER( wTowerID );
	if( m_pTowerInfo )
	{
		m_wTowerID = wTowerID;
		
		LPOBJECT pObject = FindObject( m_pTowerInfo->m_dwOBJID, pResource );
		if( pObject )
		{
			CTClientInven* pTEQUIP = new CTClientInven();
			ClearInven();

			pTEQUIP->m_bInvenID = INVEN_EQUIP;
			pTEQUIP->m_wItemID = 0;
			m_mapTINVEN.insert( make_pair( pTEQUIP->m_bInvenID, pTEQUIP ) );

			InitOBJ( pObject );

			size_t i = 0;
			for( ; i < ES_COUNT; ++i )
			{
				CTClientItem *pTITEM = new CTClientItem();

				if( i == ES_BODY )
					pTITEM->SetItemID( CTChart::FindTITEMTEMP(19009)->m_wItemID );
				else
					pTITEM->SetItemID( 0 );

				pTITEM->SetItemSlot( i );
				pTITEM->SetCount( 1 );

				pTEQUIP->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
			}

			ResetEQUIP( pDevice, pResource );
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

	SetAction( 0, 0 );

	m_bCollisionType = GetAttrINT( ID_COLLISION_TYPE );

	InitSIZE(
		GetAttrFLOAT(ID_SIZE_X),
		GetAttrFLOAT(ID_SIZE_Y),
		GetAttrFLOAT(ID_SIZE_Z),
		GetAttrFLOAT(ID_DEPTH),
		GetAttrFLOAT(ID_SIGHT));

	D3DXMatrixRotationY(
		&m_vWorld,
		m_pTowerInfo->m_fRot * 5.0f * D3DX_PI / 900.0f );

	D3DXMatrixScaling( &m_vScale, 1.0f, 1.0f, 1.0f );

	m_vWorld._41 = vPosition.x;
	m_vWorld._42 = vPosition.y;
	m_vWorld._43 = vPosition.z;

	m_vScaleSFX = CTMath::Inverse( &m_vScale );
	m_vPosition = m_vScale * m_vWorld;

	return TRUE;
}

void		CTGodTower::AddGodTower( BYTE bCamp, D3DXVECTOR3& vPosition )
{
	SetValid( TRUE );
	SetCamp( bCamp );
	SetPosition( vPosition );
	UpdateTowerSFX();
}

void		CTGodTower::DelGodTower()
{
	SetValid( FALSE );
}

void		CTGodTower::MountBall( BYTE bCamp )
{
	SetCamp( bCamp );

	UpdateTowerSFX();
}

void		CTGodTower::DeMountBall()
{
	SetCamp( 0 );

	UpdateTowerSFX();
}

void		CTGodTower::UpdateTowerSFX()
{
	if( m_pTowerInfo == NULL )
		return ;

	DWORD dwSFXID[ CAMP_COUNT ] =
	{
		m_pTowerInfo->m_dwSfxNorID,
		m_pTowerInfo->m_dwSfxDefID,
		m_pTowerInfo->m_dwSfxAtkID
	};

	size_t i = 0;
	for( ; i < CAMP_COUNT; ++i )
	{
		if( GetCamp() == i )
			PlayTActionSFX( &m_vTGARBAGESFX, dwSFXID[ i ] );
		else
			ClearSFX( &m_vTGARBAGESFX, dwSFXID[ i ] );
	}
}

void		CTGodTower::Release()
{
	m_mapResTower.clear();
}

//	END OF MEMBER FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	VIRTUAL FUNCTION IMPLEMENTATION.
CString	CTGodTower::GetName()
{
	if( m_pTowerInfo )
		return m_pTowerInfo->m_strName;

	return "";
}
//	END OF VIRTUAL FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	INLINE FUNCTION IMPLEMENTATION.
BOOL	CTGodTower::IsOccupation() const
{
	return m_bCamp == 0 ? FALSE : TRUE;
}

BOOL	CTGodTower::IsValid() const
{
	return m_bValid;
}

BOOL	CTGodTower::IsVisible() const
{
	return IsValid() && m_bVisible;
}

WORD	CTGodTower::GetTowerID() const
{
	return m_wTowerID;
}

BYTE	CTGodTower::GetCamp() const
{
	return m_bCamp;
}

LPTGODTOWER	CTGodTower::GetTowerInfo() const
{
	return m_pTowerInfo;
}

void	CTGodTower::SetTowerID( WORD wTowerID )
{
	m_wTowerID = wTowerID;
}

void	CTGodTower::SetCamp( BYTE bCamp )
{
	m_bCamp = bCamp;
}

void	CTGodTower::SetValid( BOOL bValid )
{
	m_bValid = bValid;
}

//	END OF INLINE FUNCTION.
//////////////////////////////////////////////////////////////////////////
