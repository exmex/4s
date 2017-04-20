// TImageList.cpp: implementation of the TImageList class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"


// TImageList::TDelegate
// ====================================================================================================
TImageList::TDelegate::TDelegate(LP_FRAMEDESC pDesc)
: TComponent(NULL, pDesc)
{
}
// ----------------------------------------------------------------------------------------------------
TImageList::TDelegate::~TDelegate()
{
}
// ====================================================================================================
TComponent* TImageList::TDelegate::RenderImgList( TImageList* pImgList, DWORD dwCurTick, DWORD dwDeltaTick)
{
	int nIndex = pImgList->GetCurImage();

	TCOMP_LIST::iterator itr = GetFirstKidsFinder();
	std::advance(itr, nIndex);

	if( !EndOfKids(itr) )
	{
		RenderImgComp(pImgList, *itr, dwCurTick, dwDeltaTick);
		return *itr;
	}

	return NULL;
}
// ----------------------------------------------------------------------------------------------------
void TImageList::TDelegate::RenderImgComp( TImageList* pImgList, TComponent* pImgComp, DWORD dwCurTick, DWORD dwDeltaTick)
{
	m_pParent = pImgList->GetParent();

	POINT pt;
	pImgList->GetComponentPos(&pt);
	MoveComponent(pt);

	m_dwTotalTick = dwCurTick;

	if( pImgList->IsUserColorEnabled() )
	{
		pImgComp->SetStyle( pImgComp->GetStyle() | TS_CUSTOM_COLOR );
		pImgComp->m_dwColor = pImgList->GetUserColor();
	}
	else
		pImgComp->SetStyle( pImgComp->GetStyle() & ~TS_CUSTOM_COLOR );
	
	pImgComp->EnableComponent( pImgList->IsEnable() );
	pImgComp->Render(dwDeltaTick);
}
// ====================================================================================================
BOOL TImageList::TDelegate::HitRectDelegate( TImageList* pImgList, CPoint pt)
{
	m_pParent = pImgList->GetParent();

	POINT ptImgList;
	pImgList->GetComponentPos(&ptImgList);
	MoveComponent(ptImgList);

	return TComponent::HitRect(pt);
}
// ----------------------------------------------------------------------------------------------------
BOOL TImageList::TDelegate::HitTestDelegate( TImageList* pImgList, CPoint pt)
{
	m_pParent = pImgList->GetParent();

	POINT ptImgList;
	pImgList->GetComponentPos(&ptImgList);
	MoveComponent(ptImgList);

	return TComponent::HitTest(pt);
}
// ====================================================================================================
int TImageList::TDelegate::GetImageCount()
{
	return GetKidsCount();
}
// ====================================================================================================
TComponent* TImageList::TDelegate::GetImage( int nIndex )
{
	TCOMP_LIST::iterator itr = GetFirstKidsFinder();
	std::advance(itr, nIndex);

	if( !EndOfKids(itr) )
		return *itr;

	return NULL;
}

// TImageList::TDelegateManager
// ====================================================================================================
bool TImageList::TDelegateManager::InstKey::operator < (const InstKey& r) const
{
	if( m_pDesc->m_vCOMP.m_bType != r.m_pDesc->m_vCOMP.m_bType )
		return m_pDesc->m_vCOMP.m_bType < r.m_pDesc->m_vCOMP.m_bType;

	if( m_pDesc->m_vCOMP.m_nWidth != r.m_pDesc->m_vCOMP.m_nWidth )
		return m_pDesc->m_vCOMP.m_nWidth < r.m_pDesc->m_vCOMP.m_nWidth;

	if( m_pDesc->m_vCOMP.m_nHeight != r.m_pDesc->m_vCOMP.m_nHeight )
		return m_pDesc->m_vCOMP.m_nHeight < r.m_pDesc->m_vCOMP.m_nHeight;

	LP_FRAMEDESC pLkid = m_pDesc->m_pCHILD;
	LP_FRAMEDESC pRkid = r.m_pDesc->m_pCHILD;
	
	while( pLkid && pRkid )
	{
		if( pLkid->m_vCOMP.m_dwID != pRkid->m_vCOMP.m_dwID )
			return pLkid->m_vCOMP.m_dwID < pRkid->m_vCOMP.m_dwID;

		pLkid = pLkid->m_pNEXT;
		pRkid = pRkid->m_pNEXT;
	}

	return (pRkid != NULL);
}
// ====================================================================================================
TImageList::TDelegate* TImageList::TDelegateManager::NewInstance(LP_FRAMEDESC pDesc)
{
	if( !pDesc )
		return NULL;

	InstKey key(pDesc);
	TDELEGATEMAP::iterator itr = m_mapDelegates.find(key);
	if( itr != m_mapDelegates.end() )
	{
		++(itr->second.m_nRefCnt);
		return itr->second.m_pInst;
	}

	InstData data( new TImageList::TDelegate(pDesc) );
	m_mapDelegates.insert( std::make_pair(key, data) );

	return data.m_pInst;
}
// ----------------------------------------------------------------------------------------------------
void TImageList::TDelegateManager::ReleaseInstance(TImageList::TDelegate* pInst)
{
	TDELEGATEMAP::iterator itr = m_mapDelegates.find( pInst->m_pDESC );
	if( itr == m_mapDelegates.end() )
		return;

	--(itr->second.m_nRefCnt);

	if( itr->second.m_nRefCnt == 0 )
	{
		delete itr->second.m_pInst;
		m_mapDelegates.erase(itr);
	}
}
// ====================================================================================================


// TImageList::TDelegateManager
// ====================================================================================================
TImageList::TDelegateManager TImageList::m_Manager;
// ====================================================================================================
TImageList::TImageList(TComponent *pParent, LP_FRAMEDESC pDesc)
:	TComponent(pParent, NULL),
	m_nCurIdx(0),
	m_nLastIdx(0),
	m_pLastImg(NULL),
	m_pSkinImg(NULL),
	m_bUserColor(FALSE),
	m_dwUserColor(0),
	m_bUsePixelHitTest(FALSE)	
{
	Init();

	m_pParent = pParent;
	CreateFrom(pDesc);

	m_bType = TCML_TYPE_IMAGELIST;
	m_pDelegate = m_Manager.NewInstance(pDesc);
	
	InitRect();
}
// ----------------------------------------------------------------------------------------------------
TImageList::~TImageList()
{
	m_Manager.ReleaseInstance(m_pDelegate);
}
// ====================================================================================================
void TImageList::BuildKid( LP_FRAMEDESC pDesc )
{
	// nothing
}
// ====================================================================================================
BOOL TImageList::HitRect( CPoint pt)
{
	return m_pDelegate->HitRectDelegate(this,pt);
}
// ----------------------------------------------------------------------------------------------------
BOOL TImageList::HitTest( CPoint pt)
{
	if( IsVisible() )
		return m_bUsePixelHitTest ? m_pDelegate->HitTestDelegate(this,pt) : m_pDelegate->HitRectDelegate(this,pt);
	else
		return FALSE;
}
// ----------------------------------------------------------------------------------------------------
HRESULT TImageList::Render( DWORD dwTickCount)
{
	if( !IsVisible() )
		return S_OK;

	m_dwTotalTick += dwTickCount;

	if( m_pLastImg && m_nCurIdx == m_nLastIdx )
		m_pDelegate->RenderImgComp(this, m_pLastImg, m_dwTotalTick, dwTickCount);
	else
	{
		m_pLastImg = m_pDelegate->RenderImgList(this, m_dwTotalTick, dwTickCount);
		m_nLastIdx = m_nCurIdx;
	}

	if( m_pSkinImg )
		m_pDelegate->RenderImgComp( this, m_pSkinImg, m_dwTotalTick, dwTickCount );

	return DrawText();
}
// ====================================================================================================
BOOL TImageList::EndOfImgs( TCOMP_LIST::iterator it )
{
	return m_pDelegate->EndOfKids(it);
}
// ----------------------------------------------------------------------------------------------------
TCOMP_LIST::iterator TImageList::GetFirstImgsFinder()
{
	return m_pDelegate->GetFirstKidsFinder();
}
// ----------------------------------------------------------------------------------------------------
TComponent* TImageList::GetNextImg( TCOMP_LIST::iterator &it )
{
	return m_pDelegate->GetNextKid(it);
}
// ====================================================================================================
void TImageList::InitRect()
{
	CRect rcDelegate;
	m_pDelegate->GetComponentRect(&rcDelegate);

	m_rc.right  = m_rc.left + rcDelegate.Width();
	m_rc.bottom = m_rc.top  + rcDelegate.Height();

	m_nTextExtent = m_rc.Width() - 2 * m_nHMargine;
}
// ====================================================================================================
void TImageList::SetSkinImage( int nIndex )
{
	m_pSkinImg = m_pDelegate->GetImage( nIndex );	
}
// ====================================================================================================
void TImageList::SetSkinImageEmpty()
{
	m_pSkinImg = NULL;
}