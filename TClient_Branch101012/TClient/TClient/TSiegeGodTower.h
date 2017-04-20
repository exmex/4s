#pragma once


//! GodTower Class.
/*!
*	@brief	성물을 꽂아 영역표시한다..
*			( 성물을 이 타워에 Mount & Demount )
*
*	@date	2009/08/21
*	@author	권 성혁
*
*/
class CTGodTower : public CTClientMoveObj
{
public :	// DEFINITION.
	
	typedef map< DWORD, LPOBJECT > MAP_RESOURCE_TOWER;
	typedef MAP_RESOURCE_TOWER::iterator MAP_TOWER_ITOR;

private :	// MEMBER DATA.

	WORD						m_wTowerID;
	BYTE						m_bCamp;
	BOOL						m_bValid;
	LPTGODTOWER					m_pTowerInfo;
	static MAP_RESOURCE_TOWER	m_mapResTower;

private :	// PRIVATE FUNCTION.

	/*! */
	LPOBJECT					FindObject( DWORD dwObjID, CTachyonRes* pResource );

public :

								CTGodTower();
	virtual						~CTGodTower();

public :	// MEMBER FUNCTION.

	/*! */
	BOOL						InitTower( WORD wTowerID, D3DXVECTOR3& vPosition,
		CD3DDevice* pDevice, CTachyonRes* pResource );
	/*! */
	void						AddGodTower( BYTE bCamp, D3DXVECTOR3& vPosition );
	/*! */
	void						DelGodTower();
	/*! */
	void						MountBall( BYTE bCamp );
	/*! */
	void						DeMountBall();
	/*! */
	void						UpdateTowerSFX();
	/*! */
	void						Release();

public :	// VIRTUAL FUNCTION.

	virtual CString				GetName();

public :	// INLINE FUNCTION.

	/* 현재 점령중인 타워인가..? */
	BOOL						IsOccupation() const;
	BOOL						IsValid() const;
	BOOL						IsVisible() const;
	WORD						GetTowerID() const;
	BYTE						GetCamp() const;
	LPTGODTOWER					GetTowerInfo() const;

    void						SetTowerID( WORD wTowerID );
	void						SetCamp( BYTE bCamp );
	void						SetValid( BOOL bValid );
};
