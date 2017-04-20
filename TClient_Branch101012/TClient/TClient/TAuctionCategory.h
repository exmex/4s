#pragma once

class CTSearchItem;
class CTCtrlList;

//! Category Entity Class.
class CTCategoryEntity
{
protected :	// Member Data.

	CTSearchItem*						m_pItem;
	TAUCTIONTREE						m_TreeInfo;
	BOOL								m_bExpand;
	BOOL								m_bCheck;
	DWORD								m_dwTreeLevel;
	std::vector< CTCategoryEntity* >	m_vChild;

public :	// Const... & Destructure Function.

	CTCategoryEntity();
	~CTCategoryEntity();

public :	// Member Function.

	BOOL								FindAdd( LPTAUCTIONTREE pTreeInfo );
	void								Attach( CTCategoryEntity* pEntity );
	void								AllAddItem( CTCtrlList* pList );
	void								SetCheck_Child( BOOL bCheck );
	void								SetExpand_Child( BOOL bExpand );
	LPVTDWORD							GetCheckList_Child( LPVTDWORD pVTDWORD );

public :	// Inline Function.

	void								SetItem( CTSearchItem* pItem );
	void								SetID( DWORD dwID );
	void								SetName( CString strName );
	void								SetCheckState( BOOL bCheck );
	void								SetLevel( DWORD dwLevel );
	void								SetExpand( BOOL bExpand );

	CTSearchItem*						GetItem();
	DWORD								GetID();
	CString								GetName();
	BOOL								GetCheckState();
	DWORD								GetLevel();
	size_t								GetChildSize();

	void								OpenCategory();
	void								CloseCategory();
	BOOL								IsExpand();
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//! Auction Category Class.
class CTSearchCategory
{
private :	// Member Data.

	std::vector< CTCategoryEntity* >	m_vEntity;
	CTCategoryEntity*					m_pRootEntity;
	BOOL								m_bNeedUpdate;

public :	// Const... & Destructure Function.

	CTSearchCategory();
	~CTSearchCategory();

public :	// Member Function.

	void								BuildCategory();
	void								AddEntity( CTCategoryEntity* pEntity );
	void								Update( CTCtrlList* pList );

public :	// Inline Function.

	CTCategoryEntity*					GetRootEntity();
	void								NotifyUpdate();
};
