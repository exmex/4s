#pragma once


//! GodBall Class.
/*!
*	@brief	성물쟁탈전에서의 성물을 위한 클래스.
*			( 냅다들고 뛰어서 타워에 Mount 시켜야함.. )
*
*	@date	2009/08/21
*	@author	권 성혁
*
*/
class CTGodBall : public CTClientObjBase
{
public :	// DEFINITION.
	
	typedef map< DWORD, LPOBJECT > MAP_RESOURCE_BALL;
	typedef MAP_RESOURCE_BALL::iterator MAP_BALL_ITOR;

private :	// MEMBER DATA.

	WORD						m_wBallID;			//!< 성물ID..
	BOOL						m_bInGround;		//!< 맵에 떨어지면 true..
	BYTE						m_bCamp;			//!< 공격측 or 수비측 구분..
	BOOL						m_bValid;			//!< 유효범위 내에 있을때 TRUE..

	/*! @note	차트데이터 : m_dwID, m_strName, m_fRot, m_dwOBJID, m_dwIconID, m_dwSfxID.. */
	LPTGODBALL					m_pBallInfo;

	static MAP_RESOURCE_BALL	m_mapResBall;		//!< 성물오브젝트 리소스보관..

private :	// PRIVATE FUNCTION.

	/*!  */
	LPOBJECT					FindObject( DWORD dwObjID, CTachyonRes* pResource );

public :

								CTGodBall();
	virtual						~CTGodBall();

public :	// MEMBER FUNCTION.

	/*!  */
	BOOL						InitGodBall( WORD wID, CD3DDevice* pDevice, CTachyonRes* pResource );
	/*!  */
	void						AddGodBall( BYTE bCamp, D3DXVECTOR3& vPosition, BOOL bInGround );
	/*!  */
	void						TakeGodBall();
	/*!  */
	void						DelGodBall();
	/*!  */
	void						StartBallAction();
	/*!  */
	void						Release();

public :	// VIRTUAL FUNCTION.

	virtual CString				GetName();

public :	// INLINE FUNCTION.

	WORD						GetBallID() const;
	BOOL						InGround() const;
	BYTE						GetCamp() const;
	LPTGODBALL					GetBallInfo() const;
	BOOL						IsValid() const;
	BOOL						IsVisible() const;

	void						SetBallID( WORD wBallID );
	void						SetCamp( BYTE bCamp );
	void						SetGroundPosition( D3DXVECTOR3& vPosition );
	void						SetValid( BOOL bValid );
};
