#include "stdafx.h"
#include "Game Studio.h"
#include "Itembase.h"
#include "MainFrm.h"

#include "AnimationDoc.h"
#include "EXTMAPView.h"
#include "BSPMAPDoc.h"
#include "MeshDoc.h"
#include "OBJDoc.h"
#include "SFXDoc.h"

#define	DEF_UNITSIZE		4096 * 2048
#define	DEF_OBJSIZE			4096 * 2048

#define MAX_OBJHEIGHT		(100)
#define TOTALFRAME			(30)

MAPIDINDEX CItemBase::m_mapLocalID;
MAPSTRID CItemBase::m_mapGlobalID;
MAPGSFONT CItemBase::m_mapFont;
MAPSTR CItemBase::m_mapMenuID;

MAPITEM CItemBase::m_mapAnimationItem;
MAPITEM CItemBase::m_mapOBJTypeItem;
MAPITEM CItemBase::m_mapTextureItem;
MAPITEM CItemBase::m_mapImageItem;
MAPITEM CItemBase::m_mapMediaItem;
MAPITEM CItemBase::m_mapFrameItem;
MAPITEM CItemBase::m_mapCompItem;
MAPITEM CItemBase::m_mapMeshItem;
MAPITEM CItemBase::m_mapMAPItem;
MAPITEM CItemBase::m_mapOBJItem;
MAPITEM CItemBase::m_mapSFXItem;

DWORD CItemBase::m_dwAnimationID = 1;
DWORD CItemBase::m_dwOBJTypeID = 1;
DWORD CItemBase::m_dwTextureID = 1;
DWORD CItemBase::m_dwImageID = 1;
DWORD CItemBase::m_dwMediaID = 1;
DWORD CItemBase::m_dwFrameID = 1;
DWORD CItemBase::m_dwCompID = 1;
DWORD CItemBase::m_dwMeshID = 1;
DWORD CItemBase::m_dwMAPID = 1;
DWORD CItemBase::m_dwOBJID = 1;
DWORD CItemBase::m_dwSFXID = 1;
DWORD CItemBase::m_dwLocalID = 1;

CString CItemBase::m_strTempPath(_T(""));
BYTE CItemBase::m_bCanTempPath = FALSE;

BYTE CItemBase::m_bRegulateAll = TRUE;
BYTE CItemBase::m_bMerge = FALSE;

SELECT_FORMAT SELECT_FORMAT::AVAILABLE[] =
{
	SELECT_FORMAT( "Compression None(TGA)", NON_COMP),
	SELECT_FORMAT( "Compression Mid(DXT3)", DXT3),
	SELECT_FORMAT( "Compression High(DXT1)", DXT1)
};

INT SELECT_FORMAT::COUNT = sizeof(SELECT_FORMAT::AVAILABLE) / sizeof(SELECT_FORMAT);
BYTE SELECT_FORMAT::DEFAULT_INDEX = 2; // DXT1 | 배열의 인덱스로 디폴트를 지정해야 한다.

CTachyonAnimationGS::CTachyonAnimationGS( CAnimationItem& v )
:m_vParent(v)
{
}

CTachyonAnimationGS::~CTachyonAnimationGS()
{
}

void CTachyonAnimationGS::GetFrameMatrix(
	LPD3DXMATRIX pResult,
	LPD3DXMATRIX pBlend,
	LPD3DXQUATERNION pBlendKEY,
	LPTPIVOT pPivot,
	D3DXMATRIX& vWorld,
	BYTE bRootID,
	FLOAT fTimeValue,
	FLOAT fBlendTime)
{
	if( !m_vParent.GetAniLoaded() )
		m_vParent.LoadAnimationFromOrgGSF();

	CTachyonAnimation::GetFrameMatrix(
		pResult,
		pBlend,
		pBlendKEY,
		pPivot,
		vWorld,
		bRootID,
		fTimeValue,
		fBlendTime );
}

int CTachyonAnimationGS::HitTest(
	LPD3DXMATRIX pBones,
	CD3DCamera *pCam,
	int nPosX,
	int nPosY)
{
	if( !m_vParent.GetAniLoaded() )
		m_vParent.LoadAnimationFromOrgGSF();

	return CTachyonAnimation::HitTest(
		pBones,
		pCam,
		nPosX,
		nPosY);
}

BOOL CTachyonAnimationGS::GetSize(
	D3DXVECTOR3& vMin,
	D3DXVECTOR3& vMax)
{
	if( !m_vParent.GetAniLoaded() )
		m_vParent.LoadAnimationFromOrgGSF();

	return CTachyonAnimation::GetSize(
		vMin,
		vMax);
}

DWORD CTachyonAnimationGS::GetInterval()
{
	if( !m_vParent.GetAniLoaded() )
		m_vParent.LoadAnimationFromOrgGSF();

	return CTachyonAnimation::GetInterval();
}

DWORD CTachyonAnimationGS::GetNodeCount()
{
	if( !m_vParent.GetAniLoaded() )
		m_vParent.LoadAnimationFromOrgGSF();

	return CTachyonAnimation::GetNodeCount();
}

/////////////////////////////////////////////////////////////////////////////////////////////

CItemBase::CItemBase( BYTE bType)
{
	m_pMainDocument = NULL;
	m_vDropTarget.clear();
	m_vLocalID.clear();

	m_strName = _T("");
	m_strID = _T("");

	m_nDropEffect = DROPEFFECT_NONE;
	m_bPreviewID = FALSE;
	m_bDeleted = FALSE;
	m_bNeedID = FALSE;
	m_bMenuID = 0;

	m_itemPos = NULL;
	m_bType = bType;
	m_dwID = 0;

	switch(bType)
	{
	case ITEM_ANIMATION		: m_dwID = m_bMerge ? m_dwAnimationID : m_dwAnimationID++; break;
	case ITEM_OBJTYPE		: m_dwID = m_bMerge ? m_dwOBJTypeID : m_dwOBJTypeID++; break;
	case ITEM_TEXTURE		: m_dwID = m_bMerge ? m_dwTextureID : m_dwTextureID++; break;
	case ITEM_IMAGE			: m_dwID = m_bMerge ? m_dwImageID : m_dwImageID++; break;
	case ITEM_MEDIA			: m_dwID = m_bMerge ? m_dwMediaID : m_dwMediaID++; break;
	case ITEM_MESH			: m_dwID = m_bMerge ? m_dwMeshID : m_dwMeshID++; break;
	case ITEM_OBJECT		: m_dwID = m_bMerge ? m_dwOBJID : m_dwOBJID++; break;
	case ITEM_SFX			: m_dwID = m_bMerge ? m_dwSFXID : m_dwSFXID++; break;
	case ITEM_EXTMAP		: m_dwID = m_bMerge ? m_dwMAPID : m_dwMAPID++; break;
	case ITEM_BSPMAP		: m_dwID = m_bMerge ? m_dwMAPID : m_dwMAPID++; break;
	case ITEM_FRAME			: m_dwID = m_bMerge ? m_dwFrameID : m_dwFrameID++; break;
	case ITEM_COMP			: m_dwID = m_bMerge ? m_dwCompID : m_dwCompID++; break;
	}
}

CItemBase::~CItemBase()
{
}

void CItemBase::OnDelete()
{
	MAPSTRID::iterator finder = m_mapGlobalID.find(m_strID);
	if( finder != m_mapGlobalID.end() )
		m_mapGlobalID.erase(finder);

	while(!m_vLocalID.empty())
		ReleaseLocalID( m_vLocalID[0], this);

	if(m_pMainDocument)
		m_pMainDocument->OnCloseDocument();

	m_bDeleted = TRUE;
	if(m_bRegulateAll)
		RegulateAll();

	int nCount = INT(CTachyonDoc::m_vDOC.size());
	for( int i=0; i<nCount; i++)
		CTachyonDoc::m_vDOC[i]->Update();

	m_vDropTarget.clear();
	m_vLocalID.clear();
}

void CItemBase::RegulateAll()
{
	MAPITEM::iterator it;

	for( it = m_mapAnimationItem.begin(); it != m_mapAnimationItem.end(); it++)
		if(!(*it).second->m_bDeleted)
			(*it).second->Regulate();

	for( it = m_mapOBJTypeItem.begin(); it != m_mapOBJTypeItem.end(); it++)
		if(!(*it).second->m_bDeleted)
			(*it).second->Regulate();

	for( it = m_mapTextureItem.begin(); it != m_mapTextureItem.end(); it++)
		if(!(*it).second->m_bDeleted)
			(*it).second->Regulate();

	for( it = m_mapImageItem.begin(); it != m_mapImageItem.end(); it++)
		if(!(*it).second->m_bDeleted)
			(*it).second->Regulate();

	for( it = m_mapMediaItem.begin(); it != m_mapMediaItem.end(); it++)
		if(!(*it).second->m_bDeleted)
			(*it).second->Regulate();

	for( it = m_mapFrameItem.begin(); it != m_mapFrameItem.end(); it++)
		if(!(*it).second->m_bDeleted)
			(*it).second->Regulate();

	for( it = m_mapCompItem.begin(); it != m_mapCompItem.end(); it++)
		if(!(*it).second->m_bDeleted)
			(*it).second->Regulate();

	for( it = m_mapMeshItem.begin(); it != m_mapMeshItem.end(); it++)
		if(!(*it).second->m_bDeleted)
			(*it).second->Regulate();

	for( it = m_mapOBJItem.begin(); it != m_mapOBJItem.end(); it++)
		if(!(*it).second->m_bDeleted)
			(*it).second->Regulate();

	for( it = m_mapMAPItem.begin(); it != m_mapMAPItem.end(); it++)
		if(!(*it).second->m_bDeleted)
			(*it).second->Regulate();

	for( it = m_mapSFXItem.begin(); it != m_mapSFXItem.end(); it++)
		if(!(*it).second->m_bDeleted)
			(*it).second->Regulate();
}

void CItemBase::DeleteForReset( BYTE bType)
{
	MAPITEM::iterator itITEM;
	MAPITEM *pMAPITEM = NULL;

	switch(bType)
	{
	case ITEM_EXTMAP		:
	case ITEM_BSPMAP		: pMAPITEM = &m_mapMAPItem; break;
	case ITEM_TEXTURE		: pMAPITEM = &m_mapTextureItem; break;
	case ITEM_OBJTYPE		: pMAPITEM = &m_mapOBJTypeItem; break;
	case ITEM_IMAGE			: pMAPITEM = &m_mapImageItem; break;
	case ITEM_MEDIA			: pMAPITEM = &m_mapMediaItem; break;
	case ITEM_ANIMATION		: pMAPITEM = &m_mapAnimationItem; break;
	case ITEM_MESH			: pMAPITEM = &m_mapMeshItem; break;
	case ITEM_OBJECT		: pMAPITEM = &m_mapOBJItem; break;
	case ITEM_SFX			: pMAPITEM = &m_mapSFXItem; break;
	case ITEM_COMP			: pMAPITEM = &m_mapCompItem; break;
	case ITEM_FRAME			: pMAPITEM = &m_mapFrameItem; break;
	}

	if(pMAPITEM)
		for( itITEM = pMAPITEM->begin(); itITEM != pMAPITEM->end(); itITEM++)
			(*itITEM).second->DeleteForRestore();
}

void CItemBase::Reset( BYTE bType)
{
	MAPITEM::iterator itITEM;
	MAPITEM *pMAPITEM = NULL;

	switch(bType)
	{
	case ITEM_EXTMAP		:
	case ITEM_BSPMAP		: pMAPITEM = &m_mapMAPItem; break;
	case ITEM_TEXTURE		: pMAPITEM = &m_mapTextureItem; break;
	case ITEM_OBJTYPE		: pMAPITEM = &m_mapOBJTypeItem; break;
	case ITEM_IMAGE			: pMAPITEM = &m_mapImageItem; break;
	case ITEM_MEDIA			: pMAPITEM = &m_mapMediaItem; break;
	case ITEM_ANIMATION		: pMAPITEM = &m_mapAnimationItem; break;
	case ITEM_MESH			: pMAPITEM = &m_mapMeshItem; break;
	case ITEM_OBJECT		: pMAPITEM = &m_mapOBJItem; break;
	case ITEM_SFX			: pMAPITEM = &m_mapSFXItem; break;
	case ITEM_COMP			: pMAPITEM = &m_mapCompItem; break;
	case ITEM_FRAME			: pMAPITEM = &m_mapFrameItem; break;
	}

	if(pMAPITEM)
		for( itITEM = pMAPITEM->begin(); itITEM != pMAPITEM->end(); itITEM++)
			(*itITEM).second->Restore();
}

void CItemBase::DeleteForRestore()
{
}

void CItemBase::Restore()
{
}

void CItemBase::Load( CFile* pFile)
{
	int nCount = 0;
	DWORD dwID;

	pFile->Read( &dwID, sizeof(DWORD));
	m_dwID = m_bMerge ? m_dwID + dwID : dwID;
	m_strID = LoadString(pFile);
	m_strName = LoadString(pFile);

	pFile->Read( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		CString strLocalID = LoadString(pFile);
		if(!m_bMerge)
			m_vLocalID.push_back(strLocalID);
	}

	if( m_bMerge && !m_strID.IsEmpty() )
	{
		if( m_mapGlobalID.find(m_strID) != m_mapGlobalID.end() )
			m_strID = GetNewID(m_strID);

		m_mapGlobalID.insert( MAPSTRID::value_type( m_strID, m_dwID));
	}
}

void CItemBase::Save( CFile* pFile)
{
	pFile->Write( &m_bType, sizeof(BYTE));
	pFile->Write( &m_dwID, sizeof(DWORD));
	WriteString( pFile, m_strID);
	WriteString( pFile, m_strName);

	int nCount = INT(m_vLocalID.size());
	pFile->Write( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
		WriteString( pFile, m_vLocalID[i]);
}

CString CItemBase::LoadString( CFile* pFile)
{
	CString strResult("");
	int nLength;

	pFile->Read( &nLength, sizeof(int));
	if( nLength > 0 )
	{
		pFile->Read( strResult.GetBuffer(nLength), nLength);
		strResult.ReleaseBuffer(nLength);

		return strResult.Left(nLength);
	}

	return strResult;
}

void CItemBase::WriteString( CFile* pFile, CString& strValue)
{
	int nLength = strValue.GetLength();

	pFile->Write( &nLength, sizeof(int));
	if( nLength > 0 )
		pFile->Write( LPCSTR(strValue), nLength);
}

CItemBase* CItemBase::CreateItem( CFile* pFile)
{
	CItemBase* pItem = NULL;
	BYTE bType;

	pFile->Read( &bType, sizeof(BYTE));
	switch(bType)
	{
	case ITEM_PROJECT		: pItem = new CProjectItem(); break;
	case ITEM_FOLDER		: pItem = new CFolderItem(); break;
	case ITEM_TEXTURE		: pItem = new CTextureItem(); break;
	case ITEM_OBJTYPE		: pItem = new COBJTypeItem(); break;
	case ITEM_IMAGE			: pItem = new CImageItem(); break;
	case ITEM_MEDIA			: pItem = new CMediaItem(); break;
	case ITEM_ANIMATION		: pItem = new CAnimationItem(); break;
	case ITEM_MESH			: pItem = new CMeshItem(); break;
	case ITEM_OBJECT		: pItem = new COBJItem(); break;
	case ITEM_SFX			: pItem = new CSFXItem(); break;
	case ITEM_EXTMAP		: pItem = new CEXTMAPItem(); break;
	case ITEM_BSPMAP		: pItem = new CBSPMAPItem(); break;
	case ITEM_COMP			: pItem = new CCOMPItem(); break;
	case ITEM_FRAME			: pItem = new CFRAMEItem(); break;
	}

	if(pItem)
		pItem->Load(pFile);

	return pItem;
}

int CItemBase::GetIconIndex()
{
	return m_bType * 2;
}

void CItemBase::SetMainDocument( CTachyonDoc *pDoc)
{
	m_pMainDocument = pDoc;
}

CTachyonDoc* CItemBase::GetMainDocument()
{
	return m_pMainDocument;
}

void CItemBase::Regulate()
{
}

CMeshItem* CItemBase::GetMeshItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapMeshItem.find(dwID);

	if( finder != m_mapMeshItem.end() && !(*finder).second->m_bDeleted )
		return (CMeshItem *) (*finder).second;

	return NULL;
}

CEXTMAPItem* CItemBase::GetEXTMAPItem( DWORD dwID)
{
	CMAPItem *pMAP = GetMAPItem(dwID);

	if( pMAP && pMAP->m_bMAPType == MT_EXT )
		return (CEXTMAPItem *) pMAP;

	return NULL;
}

CBSPMAPItem* CItemBase::GetBSPMAPItem( DWORD dwID)
{
	CMAPItem *pMAP = GetMAPItem(dwID);

	if( pMAP && pMAP->m_bMAPType == MT_BSP )
		return (CBSPMAPItem *) pMAP;

	return NULL;
}

CMAPItem* CItemBase::GetMAPItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapMAPItem.find(dwID);

	if( finder != m_mapMAPItem.end() && !(*finder).second->m_bDeleted )
		return (CMAPItem *) (*finder).second;

	return NULL;
}

CSFXItem* CItemBase::GetSFXItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapSFXItem.find(dwID);

	if( finder != m_mapSFXItem.end() && !(*finder).second->m_bDeleted )
		return (CSFXItem *) (*finder).second;

	return NULL;
}

CAnimationItem* CItemBase::GetAnimationItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapAnimationItem.find(dwID);

	if( finder != m_mapAnimationItem.end() && !(*finder).second->m_bDeleted )
		return (CAnimationItem *) (*finder).second;

	return NULL;
}

COBJItem* CItemBase::GetOBJItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapOBJItem.find(dwID);

	if( finder != m_mapOBJItem.end() && !(*finder).second->m_bDeleted )
		return (COBJItem *) (*finder).second;

	return NULL;
}

COBJTypeItem* CItemBase::GetOBJTypeItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapOBJTypeItem.find(dwID);

	if( finder != m_mapOBJTypeItem.end() && !(*finder).second->m_bDeleted )
		return (COBJTypeItem *) (*finder).second;

	return NULL;
}

CTextureItem* CItemBase::GetTextureItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapTextureItem.find(dwID);

	if( finder != m_mapTextureItem.end() && !(*finder).second->m_bDeleted )
		return (CTextureItem *) (*finder).second;

	return NULL;
}

CImageItem* CItemBase::GetImageItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapImageItem.find(dwID);

	if( finder != m_mapImageItem.end() && !(*finder).second->m_bDeleted )
		return (CImageItem *) (*finder).second;

	return NULL;
}

CMediaItem* CItemBase::GetMediaItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapMediaItem.find(dwID);

	if( finder != m_mapMediaItem.end() && !(*finder).second->m_bDeleted )
		return (CMediaItem *) (*finder).second;

	return NULL;
}

CFRAMEItem* CItemBase::GetFRAMEItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapFrameItem.find(dwID);

	if( finder != m_mapFrameItem.end() && !(*finder).second->m_bDeleted )
		return (CFRAMEItem *) (*finder).second;

	return NULL;
}

CCOMPItem* CItemBase::GetCOMPItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapCompItem.find(dwID);

	if( finder != m_mapCompItem.end() && !(*finder).second->m_bDeleted )
		return (CCOMPItem *) (*finder).second;

	return NULL;
}

CString CItemBase::GetNewID( CString strHead)
{
	CString strResult;
	DWORD dwCount = 0;

	do strResult.Format( "%s%d", strHead, dwCount++);
	while( m_mapGlobalID.find(strResult) != m_mapGlobalID.end() );

	return strResult;
}

CString CItemBase::GetNewID( DWORD dwFormat)
{
	CString strResult;
	DWORD dwCount = 0;

	do strResult.Format( dwFormat, dwCount++);
	while( m_mapGlobalID.find(strResult) != m_mapGlobalID.end() );

	return strResult;
}

DWORD CItemBase::GetID( CString strID)
{
	MAPSTRID::iterator finder = m_mapGlobalID.find(strID);
	return finder != m_mapGlobalID.end() ? (*finder).second : 0;
}

BOOL CItemBase::CheckDupLocalID( CString strPrevID, CString strNewID)
{
	if( strNewID == strPrevID )
		return TRUE;

	if( m_mapGlobalID.find(strNewID) != m_mapGlobalID.end() &&
		m_mapLocalID.find(strNewID) == m_mapLocalID.end() )
		return FALSE;

	int nCount = INT(m_vLocalID.size());
	for( int i=0; i<nCount; i++)
		if( m_vLocalID[i] == strNewID )
			return FALSE;

	return TRUE;
}

BOOL CItemBase::CheckDupID( CString strPrevID, CString strNewID)
{
	if( strNewID == strPrevID )
		return TRUE;

	MAPSTRID::iterator finder = m_mapGlobalID.find(strNewID);
	if( finder != m_mapGlobalID.end() )
		return FALSE;

	return TRUE;
}

BOOL CItemBase::CheckID( CString strID)
{
	if(strID.IsEmpty())
		return TRUE;

	int nCount = strID.GetLength();
	char bChar = strID[0];

	if(( bChar < 'A' || bChar > 'Z' ) && bChar != '_' )
		return FALSE;

	for( int i=1; i<nCount; i++)
	{
		bChar = strID[i];

		if( ( bChar < 'A' || bChar > 'Z' ) && ( bChar < '0' || bChar > '9' ) && bChar != '_' )
			return FALSE;
	}

	return TRUE;
}

void CItemBase::ReleaseLocalID( CString strID, CItemBase *pItem)
{
	VECTORSTRING::iterator itID = pItem->m_vLocalID.begin();
	MAPIDINDEX::iterator finder = m_mapLocalID.find(strID);

	while( itID != pItem->m_vLocalID.end() )
		if( (*itID) == strID )
			pItem->m_vLocalID.erase(itID);
		else
			itID++;

	if( finder != m_mapLocalID.end() )
	{
		VECTORIDINDEX::iterator itIDX = (*finder).second->begin();

		while( itIDX != (*finder).second->end() )
			if( (*itIDX)->m_bType == pItem->m_bType && (*itIDX)->m_dwID == pItem->m_dwID )
			{
				delete (*itIDX);
				(*finder).second->erase(itIDX);
			}
			else
				itIDX++;

		if((*finder).second->empty())
		{
			MAPSTRID::iterator it = m_mapGlobalID.find(strID);

			if( it != m_mapGlobalID.end() )
				m_mapGlobalID.erase(it);

			delete (*finder).second;
			m_mapLocalID.erase(finder);
		}
	}
}

DWORD CItemBase::RegLocalID( CString strID, CItemBase *pItem)
{
	MAPIDINDEX::iterator finder = m_mapLocalID.find(strID);
	VECTORIDINDEX *pINDEX = NULL;
	LPIDINDEX pID = new IDINDEX();

	pID->m_bType = pItem->m_bType;
	pID->m_dwID = pItem->m_dwID;

	if( finder == m_mapLocalID.end() )
	{
		m_mapGlobalID.insert( MAPSTRID::value_type( strID, m_dwLocalID));
		m_dwLocalID++;

		pINDEX = new VECTORIDINDEX();
		pINDEX->clear();

		m_mapLocalID.insert( MAPIDINDEX::value_type( strID, pINDEX));
	}
	else
	{
		pINDEX = (*finder).second;

		VECTORIDINDEX::iterator it = pINDEX->begin();
		while(it != pINDEX->end())
			if( (*it)->m_bType == pItem->m_bType && (*it)->m_dwID == pItem->m_dwID )
			{
				delete (*it);
				pINDEX->erase(it);
			}
			else
				it++;
	}

	VECTORSTRING::iterator itSTR = pItem->m_vLocalID.begin();
	MAPSTRID::iterator itID = m_mapGlobalID.find(strID);

	while( itSTR != pItem->m_vLocalID.end() )
		if( (*itSTR) == strID )
			pItem->m_vLocalID.erase(itSTR);
		else
			itSTR++;

	pItem->m_vLocalID.push_back(strID);
	pINDEX->push_back(pID);

	return (*itID).second;
}

DWORD CItemBase::RegGlobalID( CString strID, DWORD dwID)
{
	MAPSTRID::iterator finder = m_mapGlobalID.find(strID);

	if( finder != m_mapGlobalID.end() )
		dwID = (*finder).second;
	else
		m_mapGlobalID.insert( MAPSTRID::value_type( strID, dwID));

	return dwID;
}

void CItemBase::GetStudioTempPath( CHAR* pPath)
{
	ZeroMemory( pPath, _MAX_PATH);
	memcpy( pPath, LPCSTR(m_strTempPath), m_strTempPath.GetLength());
}


//////////////////////////////////////////////////////////////////
// CProjectItem Function...
//////////////////////////////////////////////////////////////////

CProjectItem::CProjectItem()
:CItemBase(ITEM_PROJECT)
{
	m_bPreviewID = FALSE;
	m_bMenuID = 1;
}

CProjectItem::~CProjectItem()
{
	m_vDropTarget.clear();
}


//////////////////////////////////////////////////////////////////
// CFolderItem Function...
//////////////////////////////////////////////////////////////////

CFolderItem::CFolderItem()
:CItemBase(ITEM_FOLDER)
{
	m_vDropTarget.push_back(ITEM_PROJECT);
	m_vDropTarget.push_back(ITEM_FOLDER);

	m_nDropEffect = DROPEFFECT_MOVE;
	m_bPreviewID = FALSE;
	m_bMenuID = 1;
}

CFolderItem::~CFolderItem()
{
	m_vDropTarget.clear();
}


//////////////////////////////////////////////////////////////////
// CTextureItem Function...
//////////////////////////////////////////////////////////////////

CTextureItem::CTextureItem()
:CItemBase(ITEM_TEXTURE)
{
	m_vDropTarget.push_back(ITEM_PROJECT);
	m_vDropTarget.push_back(ITEM_FOLDER);
	m_vTextureID.clear();

	m_nDropEffect = DROPEFFECT_MOVE;
	m_pTexData = NULL;

	m_dwBackColor = RGB( 0, 0, 0);
	m_dwTotalFrame = TOTALFRAME;
	m_dwSize = 0;
	m_ullOffsetData = 0;
	m_bTexLoaded = FALSE;

	m_bPreviewID = TRUE;
	m_bMenuID = 2;
	m_bTextureOption = TEXTURE_DETAIL_LOW;
	m_bTextureFormat = DXT1;

	m_mapTextureItem.insert( MAPITEM::value_type( m_dwID, this));
}

CTextureItem::~CTextureItem()
{
	MAPITEM::iterator finder = m_mapTextureItem.find(m_dwID);
	OnDelete();

	if( finder != m_mapTextureItem.end() )
		m_mapTextureItem.erase(finder);

	if(m_pTexData)
	{
		delete[] m_pTexData;
		m_pTexData = NULL;
	}

	m_vTextureID.clear();
}

void CTextureItem::Load( CFile* pFile)
{
	MAPITEM::iterator finder = m_mapTextureItem.find(m_dwID);
	if( finder != m_mapTextureItem.end() )
		m_mapTextureItem.erase(finder);
	CItemBase::Load(pFile);
	m_mapTextureItem.insert( MAPITEM::value_type( m_dwID, this));

	pFile->Read( &m_dwSize, sizeof(DWORD));
	if( m_dwSize > 0 )
	{
		m_ullOffsetData = pFile->GetPosition();
		pFile->Seek(m_dwSize,CFile::current);
		m_bTexLoaded = FALSE;
		//m_pData = new BYTE[m_dwSize];
		//pFile->Read( m_pData, m_dwSize);
	}

	int nCount = 0;
	pFile->Read( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		if( m_bMerge && dwID )
			dwID += m_dwTextureID;

		m_vTextureID.push_back(dwID);
	}

	pFile->Read( &m_dwBackColor, sizeof(COLORREF));
	pFile->Read( &m_dwTotalFrame, sizeof(DWORD));

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPUVKEY pKEY = new UVKEY();

		pFile->Read( &pKEY->m_dwTick, sizeof(DWORD));
		pFile->Read( &pKEY->m_fKeySU, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fKeySV, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fKeyU, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fKeyV, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fKeyR, sizeof(FLOAT));

		m_TextureSet.m_vKey.push_back(pKEY);
	}

	pFile->Read( &m_TextureSet.m_dwTotalTick, sizeof(DWORD));
	pFile->Read( &m_TextureSet.m_dwMipFilter, sizeof(DWORD));
	pFile->Read( &m_TextureSet.m_fMipBias, sizeof(FLOAT));
	pFile->Read( &m_bTextureOption, sizeof(BYTE));
	pFile->Read( &m_bTextureFormat, sizeof(BYTE));

	/*if(m_pData)
	{
		m_pTexture = CMainFrame::m_3DDevice.LoadTexture(
			m_pData,
			m_dwSize,
			D3DX_DEFAULT);
	}*/
}

void CTextureItem::Save( CFile* pFile)
{
	CItemBase::Save(pFile);

	pFile->Write( &m_dwSize, sizeof(DWORD));

	if( m_dwSize > 0 )
	{
		ULONGLONG ullNewOffsetData = pFile->GetPosition();

		if( m_bTexLoaded )
		{
			pFile->Write( m_pTexData, m_dwSize);
		}
		else if( m_ullOffsetData != 0 )
		{
			// 이 텍스처는 한번도 메모리에 로딩된적이 없는 파일이다.
			// 따라서 예전 GSF파일에서 메모리로 읽은 다음에 파일에 써주자!
			CFile file( CMainFrame::m_strFileName, CFile::modeRead|CFile::typeBinary);

			m_pTexData = new BYTE[m_dwSize];
			file.Seek(m_ullOffsetData, CFile::begin);
			file.Read( m_pTexData, m_dwSize);
			file.Close();

			pFile->Write( m_pTexData, m_dwSize);
			delete m_pTexData;
			m_pTexData = NULL;
		}

		m_ullOffsetData = ullNewOffsetData;
	}

	int nCount = INT(m_vTextureID.size());
	pFile->Write( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
		pFile->Write( &m_vTextureID[i], sizeof(DWORD));

	pFile->Write( &m_dwBackColor, sizeof(COLORREF));
	pFile->Write( &m_dwTotalFrame, sizeof(DWORD));

	nCount = INT(m_TextureSet.m_vKey.size());
	pFile->Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		pFile->Write( &m_TextureSet.m_vKey[i]->m_dwTick, sizeof(DWORD));
		pFile->Write( &m_TextureSet.m_vKey[i]->m_fKeySU, sizeof(FLOAT));
		pFile->Write( &m_TextureSet.m_vKey[i]->m_fKeySV, sizeof(FLOAT));
		pFile->Write( &m_TextureSet.m_vKey[i]->m_fKeyU, sizeof(FLOAT));
		pFile->Write( &m_TextureSet.m_vKey[i]->m_fKeyV, sizeof(FLOAT));
		pFile->Write( &m_TextureSet.m_vKey[i]->m_fKeyR, sizeof(FLOAT));
	}

	pFile->Write( &m_TextureSet.m_dwTotalTick, sizeof(DWORD));
	pFile->Write( &m_TextureSet.m_dwMipFilter, sizeof(DWORD));
	pFile->Write( &m_TextureSet.m_fMipBias, sizeof(FLOAT));
	pFile->Write( &m_bTextureOption, sizeof(BYTE));
	pFile->Write( &m_bTextureFormat, sizeof(BYTE));
}

void CTextureItem::Regulate()
{
	VECTORDWORD::iterator it = m_vTextureID.begin();
	m_TextureSet.ClearTextureItem();

	while(it != m_vTextureID.end())
	{
		CTextureItem *pTEX = GetTextureItem(*it);

		if( pTEX && !pTEX->m_bDeleted )
		{
			m_TextureSet.PushTextureItem( pTEX);
			it++;
		}
		else
			m_vTextureID.erase(it);
	}
}

LPDIRECT3DTEXTURE9 CTextureItem::GetLPDIRECT3DTEXTURE9()
{
	CT3DTexture *pTEX = GetT3DTEX();
	return pTEX ? pTEX->GetTEX() : NULL;
}

CT3DTexture *CTextureItem::GetT3DTEX()
{
	if(!m_bTexLoaded)
		LoadTextureFromOrgGSF();

	return &m_vDATA;
}

LPBYTE CTextureItem::GetTexData()
{
	if( m_bTexLoaded)
	{
		return m_pTexData;
	}
	else
	{
		LoadTextureFromOrgGSF();
		return m_pTexData;
	}
	return NULL;
}

LPTEXTURESETSTUDIO CTextureItem::GetTEXTURESET()
{
	return &m_TextureSet;
}

void CTextureItem::LoadTextureFromOrgGSF()
{
	if( !m_bTexLoaded && m_ullOffsetData != 0)
	{
		CFile file( CMainFrame::m_strFileName, CFile::modeRead|CFile::typeBinary);

		m_pTexData = new BYTE[m_dwSize];

		if(!m_pTexData)
		{
			SetNullTexture();
			return;
		}

		file.Seek(m_ullOffsetData, CFile::begin);
		file.Read( m_pTexData, m_dwSize);

		LPDIRECT3DTEXTURE9 pTEX = CMainFrame::m_3DDevice.LoadTexture(
			m_pTexData,
			m_dwSize,
			D3DX_DEFAULT);

		if(!pTEX)
		{
			delete [] m_pTexData;
			SetNullTexture();
		}
		else
		{
			m_vDATA.m_pTDATA = (LPBYTE) pTEX;
			m_vDATA.m_bEnabled = TRUE;
		}

		m_bTexLoaded = TRUE;
		m_ullOffsetData = 0;
	}
}

void CTextureItem::LoadTextureFromCFile( CFile& file)
{
	if( m_pTexData )
		delete [] m_pTexData;

	m_vDATA.ReleaseDATA();
	m_pTexData = NULL;

	m_dwSize = (DWORD) file.GetLength();
	m_pTexData = new BYTE[ m_dwSize ];
	file.Read( m_pTexData, m_dwSize);

	LPT3DDATA pData = CGameStudioDoc::CompressTextureFileData(
		m_pTexData,
		m_dwSize);

	delete [] m_pTexData;
	m_dwSize = pData->m_dwSize;
	m_pTexData = pData->m_pData;
	pData->m_pData = NULL;
	delete pData;

	m_vDATA.m_pTDATA = (LPBYTE) CMainFrame::m_3DDevice.LoadTexture(
		m_pTexData,
		m_dwSize,
		D3DX_DEFAULT);
	m_vDATA.m_bEnabled = TRUE;

	m_bTexLoaded = TRUE;
	m_ullOffsetData = 0;
}

void CTextureItem::SetNullTexture()
{
	m_vDATA.ReleaseDATA();
	m_pTexData = NULL;
	m_dwSize = 0;
	m_bTexLoaded = TRUE; // 로드는 됐지만 m_pTexture,m_pTexData가 유효하지 않다! 그럼 NULL텍스쳐다.
	m_ullOffsetData = 0;
}

//////////////////////////////////////////////////////////////////
// CImageItem Function...
//////////////////////////////////////////////////////////////////

CImageItem::CImageItem()
:CItemBase(ITEM_IMAGE)
{
	m_vDropTarget.push_back(ITEM_PROJECT);
	m_vDropTarget.push_back(ITEM_FOLDER);
	m_vImageID.clear();
	m_Image.Release();

	m_nDropEffect = DROPEFFECT_MOVE;
	m_pData = NULL;

	m_dwBackColor = RGB( 255, 255, 255);
	m_dwTotalFrame = TOTALFRAME;
	m_dwSize = 0;

	m_bPreviewID = TRUE;
	m_bMenuID = 2;

	m_mapImageItem.insert( MAPITEM::value_type( m_dwID, this));
	m_bImageFormat = DXT1;
}

CImageItem::~CImageItem()
{
	MAPITEM::iterator finder = m_mapImageItem.find(m_dwID);
	OnDelete();

	if( finder != m_mapImageItem.end() )
		m_mapImageItem.erase(finder);

	if(m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}

	m_vImageID.clear();
	m_Image.Release();
}

void CImageItem::Load( CFile* pFile)
{
	MAPITEM::iterator finder = m_mapImageItem.find(m_dwID);
	if( finder != m_mapImageItem.end() )
		m_mapImageItem.erase(finder);
	CItemBase::Load(pFile);
	m_mapImageItem.insert( MAPITEM::value_type( m_dwID, this));

	pFile->Read( &m_dwSize, sizeof(DWORD));
	if( m_dwSize > 0 )
	{
		m_pData = new BYTE[m_dwSize];
		pFile->Read( m_pData, m_dwSize);
	}

	int nCount = 0;
	pFile->Read( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		DWORD dwID = 0;

		pFile->Read( &dwID, sizeof(DWORD));
		if( m_bMerge && dwID )
			dwID += m_dwImageID;

		m_vImageID.push_back(dwID);
	}

	pFile->Read( &m_dwBackColor, sizeof(COLORREF));
	pFile->Read( &m_dwTotalFrame, sizeof(DWORD));
	pFile->Read( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		LPCLRKEY pKEY = new CLRKEY();

		pFile->Read( &pKEY->m_dwColor, sizeof(DWORD));
		pFile->Read( &pKEY->m_dwTick, sizeof(DWORD));

		m_ImageSet.m_vKey.push_back(pKEY);
	}

	pFile->Read( &m_ImageSet.m_dwTotalTick, sizeof(DWORD));
	pFile->Read( &m_bImageFormat, sizeof(BYTE));

	if(m_pData)
		m_Image.Load(m_pData);
}

void CImageItem::Save( CFile* pFile)
{
	CItemBase::Save(pFile);

	pFile->Write( &m_dwSize, sizeof(DWORD));
	if( m_dwSize > 0 )
		pFile->Write( m_pData, m_dwSize);

	int nCount = INT(m_vImageID.size());
	pFile->Write( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
		pFile->Write( &m_vImageID[i], sizeof(DWORD));

	pFile->Write( &m_dwBackColor, sizeof(COLORREF));
	pFile->Write( &m_dwTotalFrame, sizeof(DWORD));

	nCount = INT(m_ImageSet.m_vKey.size());
	pFile->Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		pFile->Write( &m_ImageSet.m_vKey[i]->m_dwColor, sizeof(DWORD));
		pFile->Write( &m_ImageSet.m_vKey[i]->m_dwTick, sizeof(DWORD));
	}

	pFile->Write( &m_ImageSet.m_dwTotalTick, sizeof(DWORD));
	pFile->Write( &m_bImageFormat, sizeof(BYTE));
}

void CImageItem::Regulate()
{
	VECTORDWORD::iterator it = m_vImageID.begin();
	m_ImageSet.m_vImage.clear();

	while(it != m_vImageID.end())
	{
		CImageItem *pIMG = GetImageItem(*it);

		if( pIMG && !pIMG->m_bDeleted )
		{
			m_ImageSet.m_vImage.push_back(&pIMG->m_Image);
			it++;
		}
		else
			m_vImageID.erase(it);
	}
}


//////////////////////////////////////////////////////////////////
// CMediaItem Function...
//////////////////////////////////////////////////////////////////

CMediaItem::CMediaItem()
:CItemBase(ITEM_MEDIA)
{
	m_vDropTarget.push_back(ITEM_PROJECT);
	m_vDropTarget.push_back(ITEM_FOLDER);

	m_nDropEffect = DROPEFFECT_MOVE;
	m_bMediaType = MEDIATYPE_WAV;
	m_bVolume = 100;

	m_bPreviewID = TRUE;
	m_bMenuID = 2;

	m_szFileName[0] = L'\0';
	m_mapMediaItem.insert( MAPITEM::value_type( m_dwID, this));
}

CMediaItem::~CMediaItem()
{
	MAPITEM::iterator finder = m_mapMediaItem.find(m_dwID);
	OnDelete();

	if( finder != m_mapMediaItem.end() )
		m_mapMediaItem.erase(finder);

	CMainFrame::m_TachyonMedia.Unregister(
		GetPlayerType(),
		m_dwID);

	if( m_szFileName[0] != L'\0' )
	{
		CString strFileName;
		strFileName.Format( _T("%s"), m_szFileName);

		DeleteFile(strFileName);
		m_szFileName[0] = L'\0';
	}
}

void CMediaItem::Load( CFile* pFile)
{
	MAPITEM::iterator finder = m_mapMediaItem.find(m_dwID);
	if( finder != m_mapMediaItem.end() )
		m_mapMediaItem.erase(finder);
	CItemBase::Load(pFile);
	m_mapMediaItem.insert( MAPITEM::value_type( m_dwID, this));

	DWORD dwSize = 0;
	pFile->Read( &dwSize, sizeof(DWORD));

	if( dwSize > 0 )
	{
		char szTempPath[_MAX_PATH];
		char szTempFile[_MAX_PATH];
		
		GetStudioTempPath(szTempPath);

		GetTempFileName(
			szTempPath,
			_T("_MEDIA"),
			0, szTempFile);
		memcpy( m_szFileName, szTempFile, _MAX_PATH);

		CFile file( m_szFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		LPBYTE pDATA = new BYTE[dwSize];

		pFile->Read( pDATA, dwSize);
		file.Write( pDATA, dwSize);

		delete[] pDATA;
	}

	pFile->Read( &m_bMediaType, sizeof(BYTE));
	pFile->Read( &m_bVolume, sizeof(BYTE));

	switch(GetPlayerType())
	{
	case MEDIA_TMUSIC	:
		{
			CMainFrame::m_TachyonMedia.LoadDMusic(
				m_dwID,
				m_szFileName,
				m_bVolume);
		}

		break;

	case MEDIA_TSOUND	:
		{
			CMainFrame::m_TachyonMedia.LoadDSound(
				m_dwID,
				m_szFileName,
				m_bVolume);
		}

		break;

	case MEDIA_TSHOW	:
		{
			CMainFrame::m_TachyonMedia.LoadDShow(
				NULL, NULL,
				m_dwID, 0,
				m_szFileName,
				m_bVolume);
		}

		break;
	}
}

void CMediaItem::Save( CFile* pFile)
{
	CItemBase::Save(pFile);
	CFile file( m_szFileName, CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary);
	DWORD dwSize = (DWORD) file.GetLength();

	pFile->Write( &dwSize, sizeof(DWORD));
	if( dwSize > 0 )
	{
		LPBYTE pDATA = new BYTE[dwSize];
		file.Read( pDATA, dwSize);

		pFile->Write( pDATA, dwSize);
		delete[] pDATA;
	}

	pFile->Write( &m_bMediaType, sizeof(BYTE));
	pFile->Write( &m_bVolume, sizeof(BYTE));
}

BYTE CMediaItem::GetPlayerType()
{
	switch(m_bMediaType)
	{
	case MEDIATYPE_SND	:
	case MEDIATYPE_WAV	: return MEDIA_TSOUND;
	case MEDIATYPE_MPA	:
	case MEDIATYPE_MP2	:
	case MEDIATYPE_MP3	: return MEDIA_TMUSIC;
	}

	return MEDIA_TSHOW;
}


//////////////////////////////////////////////////////////////////
// CAnimationItem Function...
//////////////////////////////////////////////////////////////////

CAnimationItem::CAnimationItem()
:CItemBase(ITEM_ANIMATION),
m_vAnimation( *this )
{
	m_vDropTarget.push_back(ITEM_PROJECT);
	m_vDropTarget.push_back(ITEM_FOLDER);

	m_nDropEffect = DROPEFFECT_MOVE;
	m_vAnimation.Release();
	m_vReverse.clear();
	m_vEventID.clear();
	m_vAniID.clear();

	m_dwTotalFrame = TOTALFRAME;
	m_dwSize = 0;
	m_ullOffsetData = 0;
	m_bAniLoaded = FALSE;

	m_strLoopID = _T("");
	m_pAniData = NULL;
	m_pVB = NULL;

	m_bPreviewID = TRUE;
	m_bMenuID = 2;

	m_mapAnimationItem.insert( MAPITEM::value_type( m_dwID, this));
}

CAnimationItem::~CAnimationItem()
{
	MAPITEM::iterator finder = m_mapAnimationItem.find(m_dwID);
	OnDelete();

	if( finder != m_mapAnimationItem.end() )
		m_mapAnimationItem.erase(finder);

	if(m_pAniData)
	{
		delete[] m_pAniData;
		m_pAniData = NULL;
	}

	ReleaseAnimation();
	m_vReverse.clear();
	m_vEventID.clear();
	m_vAniID.clear();

	m_strLoopID = _T("");
}

void CAnimationItem::Load( CFile* pFile)
{
	MAPITEM::iterator finder = m_mapAnimationItem.find(m_dwID);
	if( finder != m_mapAnimationItem.end() )
		m_mapAnimationItem.erase(finder);
	CItemBase::Load(pFile);
	m_mapAnimationItem.insert( MAPITEM::value_type( m_dwID, this));

	pFile->Read( &m_dwSize, sizeof(DWORD));
	if( m_dwSize > 0 )
	{
		m_ullOffsetData = pFile->GetPosition();
		pFile->Seek(m_dwSize,CFile::current);
		m_bAniLoaded = FALSE;
		//m_pAniData = new BYTE[m_dwSize];
		//pFile->Read( m_pAniData, m_dwSize);
	}

	int nCount = 0;
	pFile->Read( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		DWORD dwAniID;
		BYTE bReverse;

		pFile->Read( &dwAniID, sizeof(DWORD));
		pFile->Read( &bReverse, sizeof(BYTE));

		if( m_bMerge && dwAniID )
			dwAniID += m_dwAnimationID;

		m_vReverse.push_back(bReverse);
		m_vAniID.push_back(dwAniID);
	}

	pFile->Read( &m_dwTotalFrame, sizeof(DWORD));
	m_strLoopID = LoadString(pFile);
	pFile->Read( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		LPANIKEY pKEY = new ANIKEY();
		CString strEventID;

		pFile->Read( &pKEY->m_dwEventID, sizeof(DWORD));
		pFile->Read( &pKEY->m_fTime, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fPosX, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fPosY, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fPosZ, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fRotX, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fRotY, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fRotZ, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fScaleX, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fScaleY, sizeof(FLOAT));
		pFile->Read( &pKEY->m_fScaleZ, sizeof(FLOAT));
		strEventID = LoadString(pFile);

		if( m_bMerge && !strEventID.IsEmpty() )
			pKEY->m_dwEventID = RegLocalID( strEventID, this);

		m_Aniset.m_vKey.push_back(pKEY);
		m_vEventID.push_back(strEventID);
	}

	pFile->Read( &m_Aniset.m_fTimeScale, sizeof(FLOAT));
	pFile->Read( &m_Aniset.m_dwLoopID, sizeof(DWORD));
	pFile->Read( &m_Aniset.m_bLoop, sizeof(BYTE));

	if( m_bMerge && !m_strLoopID.IsEmpty() )
		m_Aniset.m_dwLoopID = RegLocalID( m_strLoopID, this);

	/*if(m_pAniData)
		LoadAnimation( CMainFrame::m_3DDevice.m_pDevice, m_pAniData);*/
}

void CAnimationItem::Save( CFile* pFile)
{
	CItemBase::Save(pFile);

	pFile->Write( &m_dwSize, sizeof(DWORD));
	if( m_dwSize > 0 )
	{
		ULONGLONG ullNewOffsetData = pFile->GetPosition();

		if( m_bAniLoaded )
		{
			pFile->Write( m_pAniData, m_dwSize);
		}
		else if( m_ullOffsetData != 0 )
		{
			// 이 텍스처는 한번도 메모리에 로딩된적이 없는 파일이다.
			// 따라서 예전 GSF파일에서 메모리로 읽은 다음에 파일에 써주자!
			CFile file( CMainFrame::m_strFileName, CFile::modeRead|CFile::typeBinary);

			m_pAniData = new BYTE[m_dwSize];
			file.Seek(m_ullOffsetData, CFile::begin);
			file.Read( m_pAniData, m_dwSize);
			file.Close();

			pFile->Write( m_pAniData, m_dwSize);
			delete m_pAniData;
			m_pAniData = NULL;
		}

		m_ullOffsetData = ullNewOffsetData;
	}

	int nCount = INT(m_vAniID.size());
	pFile->Write( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		pFile->Write( &m_vAniID[i], sizeof(DWORD));
		pFile->Write( &m_vReverse[i], sizeof(BYTE));
	}

	pFile->Write( &m_dwTotalFrame, sizeof(DWORD));
	WriteString( pFile, m_strLoopID);

	nCount = INT(m_Aniset.m_vKey.size());
	pFile->Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		pFile->Write( &m_Aniset.m_vKey[i]->m_dwEventID, sizeof(DWORD));
		pFile->Write( &m_Aniset.m_vKey[i]->m_fTime, sizeof(FLOAT));
		pFile->Write( &m_Aniset.m_vKey[i]->m_fPosX, sizeof(FLOAT));
		pFile->Write( &m_Aniset.m_vKey[i]->m_fPosY, sizeof(FLOAT));
		pFile->Write( &m_Aniset.m_vKey[i]->m_fPosZ, sizeof(FLOAT));
		pFile->Write( &m_Aniset.m_vKey[i]->m_fRotX, sizeof(FLOAT));
		pFile->Write( &m_Aniset.m_vKey[i]->m_fRotY, sizeof(FLOAT));
		pFile->Write( &m_Aniset.m_vKey[i]->m_fRotZ, sizeof(FLOAT));
		pFile->Write( &m_Aniset.m_vKey[i]->m_fScaleX, sizeof(FLOAT));
		pFile->Write( &m_Aniset.m_vKey[i]->m_fScaleY, sizeof(FLOAT));
		pFile->Write( &m_Aniset.m_vKey[i]->m_fScaleZ, sizeof(FLOAT));
		WriteString( pFile, m_vEventID[i]);
	}

	pFile->Write( &m_Aniset.m_fTimeScale, sizeof(FLOAT));
	pFile->Write( &m_Aniset.m_dwLoopID, sizeof(DWORD));
	pFile->Write( &m_Aniset.m_bLoop, sizeof(BYTE));
}

void CAnimationItem::Regulate()
{
	VECTORDWORD::iterator itA = m_vAniID.begin();
	VECTORBYTE::iterator itR = m_vReverse.begin();

	while(!m_Aniset.m_vAni.empty())
	{
		delete m_Aniset.m_vAni.back();
		m_Aniset.m_vAni.pop_back();
	}

	while(itA != m_vAniID.end())
	{
		CAnimationItem *pANI = GetAnimationItem(*itA);

		if( pANI && !pANI->m_bDeleted )
		{
			LPANIDATA pDATA = new ANIDATA();

			pDATA->m_pAni = &pANI->m_vAnimation;
			pDATA->m_bReverse = (*itR);

			m_Aniset.m_vAni.push_back(pDATA);
			itA++; itR++;
		}
		else
		{
			itR = m_vReverse.erase(itR);
			itA = m_vAniID.erase(itA);
		}
	}
}

CTachyonAnimation& CAnimationItem::GetAnimation()
{
	if( m_bAniLoaded)
	{
		return m_vAnimation;
	}
	else
	{
		LoadAnimationFromOrgGSF();
		return m_vAnimation;
	}
}

LPBYTE CAnimationItem::GetAniData()
{
	if( m_bAniLoaded)
	{
		return m_pAniData;
	}
	else
	{
		LoadAnimationFromOrgGSF();
		return m_pAniData;
	}
	return NULL;
}

void CAnimationItem::LoadAnimationFromCFile( CFile& file, BOOL bAdd )
{
	if( bAdd )
	{
		// begin LoadAnimationFromCFile in AddAnimationItem function.

		m_dwSize = (DWORD) file.GetLength();
		m_pAniData = new BYTE[m_dwSize];
		file.Read( m_pAniData, m_dwSize);

		LoadAnimation(CMainFrame::m_3DDevice.m_pDevice,
			m_pAniData);

		// end LoadAnimationCFile
	}
	else
	{
		// begin LoadAnimationFromCFile in AnimationItemUpdate function.

		if(m_pAniData)
			delete[] m_pAniData;
		m_vAnimation.Release();

		m_dwSize = (DWORD) file.GetLength();
		m_pAniData = new BYTE[m_dwSize];
		file.Read( m_pAniData, m_dwSize);

		m_vAnimation.LoadAnimation(m_pAniData);
		// end LoadAnimationFromCFile
	}

	m_bAniLoaded = TRUE;
	m_ullOffsetData = 0;
}

void CAnimationItem::LoadAnimationFromOrgGSF()
{
	if( !m_bAniLoaded && m_ullOffsetData != 0)
	{
		CFile file( CMainFrame::m_strFileName, CFile::modeRead|CFile::typeBinary);

		m_pAniData = new BYTE[m_dwSize];
		if(!m_pAniData)
		{
			SetNullAnimation();
			return ;
		}

		file.Seek(m_ullOffsetData, CFile::begin);
		file.Read( m_pAniData, m_dwSize);

		if(m_pAniData)
			LoadAnimation( CMainFrame::m_3DDevice.m_pDevice, m_pAniData);

		m_bAniLoaded = TRUE;
		m_ullOffsetData = 0;
	}
}

void CAnimationItem::SetNullAnimation()
{
	m_vAnimation.Release();
	m_vReverse.clear();
	m_vAniID.clear();
	m_pAniData = NULL;
}

//////////////////////////////////////////////////////////////////
// CMeshItem Function...
//////////////////////////////////////////////////////////////////

CMeshItem::CMeshItem()
:CItemBase(ITEM_MESH)
{
	m_vDropTarget.push_back(ITEM_PROJECT);
	m_vDropTarget.push_back(ITEM_FOLDER);

	m_nDropEffect = DROPEFFECT_MOVE;
	m_bPreviewID = TRUE;
	m_bMenuID = 2;

	m_mapMeshItem.insert( MAPITEM::value_type( m_dwID, this));
}

CMeshItem::~CMeshItem()
{
	MAPITEM::iterator finder = m_mapMeshItem.find(m_dwID);
	OnDelete();

	if( finder != m_mapMeshItem.end() )
		m_mapMeshItem.erase(finder);
}

void CMeshItem::Load( CFile* pFile)
{
	MAPITEM::iterator finder = m_mapMeshItem.find(m_dwID);
	if( finder != m_mapMeshItem.end() )
		m_mapMeshItem.erase(finder);
	CItemBase::Load(pFile);
	m_mapMeshItem.insert( MAPITEM::value_type( m_dwID, this));

	DWORD dwLevel = 0;
	DWORD dwCount = 0;

	pFile->Read( &m_Mesh.m_dwMeshCount, sizeof(DWORD));
	pFile->Read( &m_Mesh.m_dwNodeCount, sizeof(DWORD));
	pFile->Read( &m_Mesh.m_bUseVB, sizeof(BYTE));
	pFile->Read( &dwLevel, sizeof(DWORD));

	pFile->Read( &m_Mesh.m_vCenterPoint, sizeof(D3DXVECTOR3));
	pFile->Read( &m_Mesh.m_fRadius, sizeof(FLOAT));

	if( m_Mesh.m_dwNodeCount > 0 )
	{
		m_Mesh.m_pBones = new D3DXMATRIX[m_Mesh.m_dwNodeCount];
		pFile->Read( m_Mesh.m_pBones, m_Mesh.m_dwNodeCount * sizeof(D3DXMATRIX));
	}

	if( m_Mesh.m_dwMeshCount > 0 )
	{
		m_Mesh.m_pMESH = new VECTORIBINFO[m_Mesh.m_dwMeshCount];
		m_Mesh.m_pIB = new VECTORIB[m_Mesh.m_dwMeshCount];
	}

	pFile->Read( &dwCount, sizeof(DWORD));
	if( dwCount > 0 )
	{
		m_Mesh.m_pDATA = new VBDATA();

		m_Mesh.m_pDATA->m_dwFVF = m_Mesh.m_dwNodeCount > 0 ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX;
		m_Mesh.m_pDATA->m_dwCount = dwCount;
		m_Mesh.m_pDATA->m_pVB = new BYTE[dwCount * (m_Mesh.m_dwNodeCount > 0 ? sizeof(WMESHVERTEX) : sizeof(MESHVERTEX))];

		pFile->Read( m_Mesh.m_pDATA->m_pVB, dwCount * (m_Mesh.m_dwNodeCount > 0 ? sizeof(WMESHVERTEX) : sizeof(MESHVERTEX)));
	}

	for( int i=0; i<m_Mesh.m_dwMeshCount; i++)
		for( int j=0; j<dwLevel; j++)
		{
			LPIBINFO pINFO = new IBINFO();

			pFile->Read( &dwCount, sizeof(DWORD));
			for( DWORD k=0; k<dwCount; k++)
			{
				LPIBDATA pDATA = new IBDATA();

				pFile->Read( &pDATA->m_dwCount, sizeof(DWORD));
				pFile->Read( &pDATA->m_wMatrix, sizeof(WORD));

				if( pDATA->m_dwCount > 0 )
				{
					pDATA->m_pIB = new WORD[pDATA->m_dwCount];
					pFile->Read( pDATA->m_pIB, pDATA->m_dwCount * sizeof(WORD));
				}

				pINFO->m_vIB.push_back(pDATA);
			}

			m_Mesh.m_pMESH[i].push_back(pINFO);
		}

	for( i=0; i<INT(dwLevel) - 1; i++)
	{
		FLOAT fDist;

		pFile->Read( &fDist, sizeof(FLOAT));
		m_Mesh.m_vDist.push_back(fDist);
	}

	m_Mesh.Restore(CMainFrame::m_3DDevice.m_pDevice);
}

void CMeshItem::Save( CFile* pFile)
{
	CItemBase::Save(pFile);
	DWORD dwLevel = DWORD(m_Mesh.m_vDist.size() + 1);

	pFile->Write( &m_Mesh.m_dwMeshCount, sizeof(DWORD));
	pFile->Write( &m_Mesh.m_dwNodeCount, sizeof(DWORD));
	pFile->Write( &m_Mesh.m_bUseVB, sizeof(BYTE));
	pFile->Write( &dwLevel, sizeof(DWORD));

	pFile->Write( &m_Mesh.m_vCenterPoint, sizeof(D3DXVECTOR3));
	pFile->Write( &m_Mesh.m_fRadius, sizeof(FLOAT));

	if( m_Mesh.m_dwNodeCount > 0 )
		pFile->Write( m_Mesh.m_pBones, m_Mesh.m_dwNodeCount * sizeof(D3DXMATRIX));

	if(m_Mesh.m_pDATA)
	{
		pFile->Write( &m_Mesh.m_pDATA->m_dwCount, sizeof(DWORD));
		pFile->Write( m_Mesh.m_pDATA->m_pVB, m_Mesh.m_pDATA->m_dwCount * (m_Mesh.m_dwNodeCount > 0 ? sizeof(WMESHVERTEX) : sizeof(MESHVERTEX)));
	}
	else
	{
		DWORD dwCount = 0;
		pFile->Write( &dwCount, sizeof(DWORD));
	}

	for( int i=0; i<m_Mesh.m_dwMeshCount; i++)
		for( int j=0; j<dwLevel; j++)
		{
			LPIBINFO pINFO = m_Mesh.m_pMESH[i][j];
			DWORD dwCount = DWORD(pINFO->m_vIB.size());

			pFile->Write( &dwCount, sizeof(DWORD));
			for( DWORD k=0; k<dwCount; k++)
			{
				LPIBDATA pDATA = pINFO->m_vIB[k];

				pFile->Write( &pDATA->m_dwCount, sizeof(DWORD));
				pFile->Write( &pDATA->m_wMatrix, sizeof(WORD));

				if( pDATA->m_dwCount > 0 )
					pFile->Write( pDATA->m_pIB, pDATA->m_dwCount * sizeof(WORD));
			}
		}

	for( i=0; i<INT(dwLevel) - 1; i++)
		pFile->Write( &m_Mesh.m_vDist[i], sizeof(FLOAT));
}

void CMeshItem::RebuildMESH()
{
	if(!m_Mesh.m_pDATA)
		return;

	switch(m_Mesh.m_pDATA->m_dwFVF)
	{
	case T3DFVF_WMESHVERTEX	:
		{
			LPWMESHVERTEX pBUF = (LPWMESHVERTEX) m_Mesh.m_pDATA->m_pVB;

			VECTORPTR vDATA;
			vDATA.clear();

			for( DWORD i=0; i<m_Mesh.m_dwMeshCount; i++)
				for( DWORD j=0; j<DWORD(m_Mesh.m_pMESH[i].size()); j++)
					for( DWORD k=0; k<DWORD(m_Mesh.m_pMESH[i][j]->m_vIB.size()); k++)
						for( DWORD l=0; l<m_Mesh.m_pMESH[i][j]->m_vIB[k]->m_dwCount; l++)
						{
							WORD wINDEX = FindMESH(
								&vDATA,
								&pBUF[m_Mesh.m_pMESH[i][j]->m_vIB[k]->m_pIB[l]]);

							if( wINDEX >= WORD(vDATA.size()) )
							{
								LPWMESHVERTEX pDATA = new WMESHVERTEX();

								memcpy( pDATA, &pBUF[m_Mesh.m_pMESH[i][j]->m_vIB[k]->m_pIB[l]], sizeof(WMESHVERTEX));
								m_Mesh.m_pMESH[i][j]->m_vIB[k]->m_pIB[l] = WORD(vDATA.size());
								vDATA.push_back(pDATA);
							}
							else
								m_Mesh.m_pMESH[i][j]->m_vIB[k]->m_pIB[l] = wINDEX;
						}

			delete m_Mesh.m_pDATA;
			m_Mesh.m_pDATA = NULL;

			if(!vDATA.empty())
			{
				m_Mesh.m_pDATA = new VBDATA();

				m_Mesh.m_pDATA->m_dwCount = DWORD(vDATA.size());
				m_Mesh.m_pDATA->m_dwFVF = T3DFVF_WMESHVERTEX;

				m_Mesh.m_pDATA->m_pVB = new BYTE[m_Mesh.m_pDATA->m_dwCount * sizeof(WMESHVERTEX)];
				pBUF = (LPWMESHVERTEX) m_Mesh.m_pDATA->m_pVB;

				for( i=0; i<m_Mesh.m_pDATA->m_dwCount; i++)
				{
					memcpy( &pBUF[i], vDATA[i], sizeof(WMESHVERTEX));
					delete vDATA[i];
				}

				vDATA.clear();
			}
		}

		break;

	case T3DFVF_MESHVERTEX	:
		{
			LPMESHVERTEX pBUF = (LPMESHVERTEX) m_Mesh.m_pDATA->m_pVB;

			VECTORPTR vDATA;
			vDATA.clear();

			for( DWORD i=0; i<m_Mesh.m_dwMeshCount; i++)
				for( DWORD j=0; j<DWORD(m_Mesh.m_pMESH[i].size()); j++)
					for( DWORD k=0; k<DWORD(m_Mesh.m_pMESH[i][j]->m_vIB.size()); k++)
						for( DWORD l=0; l<m_Mesh.m_pMESH[i][j]->m_vIB[k]->m_dwCount; l++)
						{
							WORD wINDEX = FindMESH(
								&vDATA,
								&pBUF[m_Mesh.m_pMESH[i][j]->m_vIB[k]->m_pIB[l]]);

							if( wINDEX >= WORD(vDATA.size()) )
							{
								LPMESHVERTEX pDATA = new MESHVERTEX();

								memcpy( pDATA, &pBUF[m_Mesh.m_pMESH[i][j]->m_vIB[k]->m_pIB[l]], sizeof(MESHVERTEX));
								m_Mesh.m_pMESH[i][j]->m_vIB[k]->m_pIB[l] = WORD(vDATA.size());
								vDATA.push_back(pDATA);
							}
							else
								m_Mesh.m_pMESH[i][j]->m_vIB[k]->m_pIB[l] = wINDEX;
						}

			delete m_Mesh.m_pDATA;
			m_Mesh.m_pDATA = NULL;

			if(!vDATA.empty())
			{
				m_Mesh.m_pDATA = new VBDATA();

				m_Mesh.m_pDATA->m_dwCount = DWORD(vDATA.size());
				m_Mesh.m_pDATA->m_dwFVF = T3DFVF_MESHVERTEX;

				m_Mesh.m_pDATA->m_pVB = new BYTE[m_Mesh.m_pDATA->m_dwCount * sizeof(MESHVERTEX)];
				pBUF = (LPMESHVERTEX) m_Mesh.m_pDATA->m_pVB;

				for( i=0; i<m_Mesh.m_pDATA->m_dwCount; i++)
				{
					memcpy( &pBUF[i], vDATA[i], sizeof(MESHVERTEX));
					delete vDATA[i];
				}

				vDATA.clear();
			}
		}

		break;
	}

	m_Mesh.Restore(CMainFrame::m_3DDevice.m_pDevice);
}

WORD CMeshItem::FindMESH( LPVECTORPTR pMESH,
						  LPWMESHVERTEX pPOINT)
{
	WORD wCount = WORD(pMESH->size());

	for( WORD i=0; i<wCount; i++)
		if(CTMath::IsEqual( (LPWMESHVERTEX) (*pMESH)[i], pPOINT))
			return i;

	return wCount;
}

WORD CMeshItem::FindMESH( LPVECTORPTR pMESH,
						  LPMESHVERTEX pPOINT)
{
	WORD wCount = WORD(pMESH->size());

	for( WORD i=0; i<wCount; i++)
		if(CTMath::IsEqual( (LPMESHVERTEX) (*pMESH)[i], pPOINT))
			return i;

	return wCount;
}

void CMeshItem::AddLevel( DWORD dwLevel)
{
	m_Mesh.m_vDist.insert(
		m_Mesh.m_vDist.begin() + dwLevel,
		dwLevel > 0 ? m_Mesh.m_vDist[dwLevel - 1] : 0.0f);

	for( int i=0; i<m_Mesh.m_dwMeshCount; i++)
	{
		LPDIRECT3DINDEXBUFFER9 pIB = NULL;
		LPIBINFO pINFO = new IBINFO();

		for( int j=0; j<INT(m_Mesh.m_pMESH[i][dwLevel]->m_vIB.size()); j++)
		{
			LPIBDATA pMESH = new IBDATA();

			pMESH->m_dwCount = m_Mesh.m_pMESH[i][dwLevel]->m_vIB[j]->m_dwCount;
			pMESH->m_wMatrix = m_Mesh.m_pMESH[i][dwLevel]->m_vIB[j]->m_wMatrix;
			pINFO->m_dwCount += pMESH->m_dwCount;

			if(pMESH->m_dwCount)
			{
				pMESH->m_pIB = new WORD[pMESH->m_dwCount];
				memcpy( pMESH->m_pIB, m_Mesh.m_pMESH[i][dwLevel]->m_vIB[j]->m_pIB, pMESH->m_dwCount * sizeof(WORD));
			}

			pINFO->m_vIB.push_back(pMESH);
		}

		if( pINFO->m_dwCount > 0 )
		{
			LPWORD pBUF = NULL;
			DWORD dwCOPY = 0;

			CMainFrame::m_3DDevice.m_pDevice->CreateIndexBuffer(
				pINFO->m_dwCount * sizeof(WORD), 0,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&pIB, NULL);

			pIB->Lock( 0, 0, (LPVOID *) &pBUF, 0);
			for( j=0; j<INT(pINFO->m_vIB.size()); j++)
			{
				memcpy( &pBUF[dwCOPY], pINFO->m_vIB[j]->m_pIB, pINFO->m_vIB[j]->m_dwCount * sizeof(WORD));
				dwCOPY += pINFO->m_vIB[j]->m_dwCount;
			}
			pIB->Unlock();
		}

		m_Mesh.m_pMESH[i].insert(
			m_Mesh.m_pMESH[i].begin() + dwLevel + 1,
			pINFO);

		m_Mesh.m_pIB[i].insert(
			m_Mesh.m_pIB[i].begin() + dwLevel + 1,
			pIB);
	}
}

void CMeshItem::DelLevel( DWORD dwLevel)
{
	m_Mesh.m_vDist.erase(m_Mesh.m_vDist.begin() + dwLevel - 1);

	for( int i=0; i<m_Mesh.m_dwMeshCount; i++)
	{
		if(m_Mesh.m_pIB[i][dwLevel])
			m_Mesh.m_pIB[i][dwLevel]->Release();
		delete m_Mesh.m_pMESH[i][dwLevel];

		m_Mesh.m_pMESH[i].erase(m_Mesh.m_pMESH[i].begin() + dwLevel);
		m_Mesh.m_pIB[i].erase(m_Mesh.m_pIB[i].begin() + dwLevel);
	}

	RebuildMESH();
}

void CMeshItem::ProgressMesh( LPBYTE pCheck,
							  DWORD dwLevel,
							  FLOAT fPolyLevel)
{
	if( !m_Mesh.m_dwMeshCount ||
		!m_Mesh.m_pDATA ||
		!m_Mesh.m_pDATA->m_pVB )
		return;

	LPVECTORDWORD pMESH = new VECTORDWORD[m_Mesh.m_dwMeshCount];
	CMeshDoc *pDoc = (CMeshDoc *) GetMainDocument();
	CMeshBar *pBar = pDoc ? pDoc->GetMeshBar() : NULL;

	DWORD dwCount = 0;
	DWORD dwRetry = 0;
	DWORD dwLOD = 0;
	DWORD dwFVF = m_Mesh.m_dwNodeCount > 0 ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX;

	VECTORPTR vDATA;
	vDATA.clear();

	if( m_Mesh.m_dwNodeCount > 0 )
	{
		LPWMESHVERTEX pDATA = (LPWMESHVERTEX) m_Mesh.m_pDATA->m_pVB;

		for( DWORD i=0; i<m_Mesh.m_pDATA->m_dwCount; i++)
		{
			LPWMESHVERTEX pBUF = new WMESHVERTEX();

			memcpy( pBUF, &pDATA[i], sizeof(WMESHVERTEX));
			vDATA.push_back(pBUF);
		}
	}
	else
	{
		LPMESHVERTEX pDATA = (LPMESHVERTEX) m_Mesh.m_pDATA->m_pVB;

		for( DWORD i=0; i<m_Mesh.m_pDATA->m_dwCount; i++)
		{
			LPMESHVERTEX pBUF = new MESHVERTEX();

			memcpy( pBUF, &pDATA[i], sizeof(MESHVERTEX));
			vDATA.push_back(pBUF);
		}
	}

	for( DWORD i=0; i<m_Mesh.m_dwMeshCount; i++)
	{
		pMESH[i].clear();

		for( DWORD j=0; j<DWORD(m_Mesh.m_pMESH[i][pCheck[i] ? dwLevel - 1 : dwLevel]->m_vIB.size()); j++)
			for( DWORD k=0; k<m_Mesh.m_pMESH[i][pCheck[i] ? dwLevel - 1 : dwLevel]->m_vIB[j]->m_dwCount; k++)
				pMESH[i].push_back(m_Mesh.m_pMESH[i][pCheck[i] ? dwLevel - 1 : dwLevel]->m_vIB[j]->m_pIB[k]);

		if(pCheck[i])
			dwCount += DWORD(pMESH[i].size()) / 3;
	}
	dwLOD = dwCount;

	while(FLOAT(dwLOD) * 100.0f / FLOAT(dwCount) > fPolyLevel)
	{
		DWORD dwPREV = dwLOD;

		CTMath::ProgressMesh(
			pMESH,
			&vDATA,
			pCheck,
			m_Mesh.m_dwMeshCount,
			dwFVF);
		dwLOD = 0;

		for( i=0; i<m_Mesh.m_dwMeshCount; i++)
			if(pCheck[i])
				dwLOD += DWORD(pMESH[i].size()) / 3;

		if( dwLOD != dwPREV )
			dwRetry = 0;
		else
			dwRetry++;

		if( dwRetry > 2 )
			break;
	}

	if(m_Mesh.m_pDATA)
	{
		delete m_Mesh.m_pDATA;
		m_Mesh.m_pDATA = NULL;
	}

	if(m_Mesh.m_pVB)
	{
		m_Mesh.m_pVB->Release();
		m_Mesh.m_pVB = NULL;
	}

	if(!vDATA.empty())
	{
		m_Mesh.m_pDATA = new VBDATA();

		m_Mesh.m_pDATA->m_dwCount = DWORD(vDATA.size());
		m_Mesh.m_pDATA->m_dwFVF = dwFVF;

		if( m_Mesh.m_dwNodeCount > 0 )
		{
			LPWMESHVERTEX pBUF = NULL;

			m_Mesh.m_pDATA->m_pVB = new BYTE[m_Mesh.m_pDATA->m_dwCount * sizeof(WMESHVERTEX)];
			pBUF = (LPWMESHVERTEX) m_Mesh.m_pDATA->m_pVB;

			for( i=0; i<m_Mesh.m_pDATA->m_dwCount; i++)
			{
				memcpy( &pBUF[i], vDATA[i], sizeof(WMESHVERTEX));
				delete vDATA[i];
			}
		}
		else
		{
			LPMESHVERTEX pBUF = NULL;

			m_Mesh.m_pDATA->m_pVB = new BYTE[m_Mesh.m_pDATA->m_dwCount * sizeof(MESHVERTEX)];
			pBUF = (LPMESHVERTEX) m_Mesh.m_pDATA->m_pVB;

			for( i=0; i<m_Mesh.m_pDATA->m_dwCount; i++)
			{
				memcpy( &pBUF[i], vDATA[i], sizeof(MESHVERTEX));
				delete vDATA[i];
			}
		}

		vDATA.clear();
	}

	for( i=0; i<m_Mesh.m_dwMeshCount; i++)
	{
		LPIBINFO pINFO = m_Mesh.BuildMeshINFO(&pMESH[i]);
		pMESH[i].clear();

		delete m_Mesh.m_pMESH[i][dwLevel];
		m_Mesh.m_pMESH[i][dwLevel] = pINFO;
	}
	delete[] pMESH;

	RebuildMESH();
	if(pBar)
		pBar->SetItemData();
}

void CMeshItem::AutoLOD()
{
	CStdioFile vSRC( _T("LOD.txt"), CFile::modeRead|CFile::typeText);
	CString strDATA;

	while(!m_Mesh.m_vDist.empty())
		DelLevel(DWORD(m_Mesh.m_vDist.size()));

	LPBYTE pCheck = new BYTE[m_Mesh.m_dwMeshCount];
	for( DWORD i=0; i<m_Mesh.m_dwMeshCount; i++)
		pCheck[i] = TRUE;

	while(vSRC.ReadString(strDATA))
	{
		DWORD dwLevel = DWORD(m_Mesh.m_vDist.size());
		FLOAT fLevel;
		FLOAT fDist;

		sscanf( LPCSTR(strDATA), _T("%f %f"),
			&fDist,
			&fLevel);
		AddLevel(dwLevel);
		m_Mesh.m_vDist[dwLevel] = fDist;

		ProgressMesh(
			pCheck,
			dwLevel + 1,
			fLevel);
	}

	delete[] pCheck;
}


//////////////////////////////////////////////////////////////////
// COBJTypeItem Function...
//////////////////////////////////////////////////////////////////

COBJTypeItem::COBJTypeItem()
:CItemBase(ITEM_OBJTYPE)
{
	m_vDropTarget.push_back(ITEM_PROJECT);
	m_vDropTarget.push_back(ITEM_FOLDER);

	m_nDropEffect = DROPEFFECT_MOVE;
	m_bPreviewID = FALSE;
	m_bMenuID = 3;

	m_mapATTR.clear();
	m_mapNAME.clear();

	m_mapOBJTypeItem.insert( MAPITEM::value_type( m_dwID, this));
}

COBJTypeItem::~COBJTypeItem()
{
	MAPITEM::iterator finder = m_mapOBJTypeItem.find(m_dwID);
	MAPATTRITEM::iterator it;
	OnDelete();

	if( finder != m_mapOBJTypeItem.end() )
		m_mapOBJTypeItem.erase(finder);

	for( it = m_mapATTR.begin(); it != m_mapATTR.end(); it++)
		delete (*it).second;

	m_mapATTR.clear();
	m_mapNAME.clear();
}

void COBJTypeItem::Load( CFile* pFile)
{
	MAPITEM::iterator finder = m_mapOBJTypeItem.find(m_dwID);
	if( finder != m_mapOBJTypeItem.end() )
		m_mapOBJTypeItem.erase(finder);
	CItemBase::Load(pFile);
	m_mapOBJTypeItem.insert( MAPITEM::value_type( m_dwID, this));

	int nCount = 0;
	pFile->Read( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		CString strName;
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		strName = LoadString(pFile);

		m_mapNAME.insert( MAPSTR::value_type( dwID, strName));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPATTRITEM pATTR = new ATTRITEM();
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		pATTR->m_strValue = LoadString(pFile);
		pFile->Read( &pATTR->m_bType, sizeof(BYTE));

		m_mapATTR.insert( MAPATTRITEM::value_type( dwID, pATTR));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPACTION pACT = new ACTION();
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		m_OBJ.m_mapACTION.insert( MAPACTION::value_type( dwID, pACT));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPCLKIND pCLK = new CLKIND();
		DWORD dwCLK;
		int nCL;

		pFile->Read( &dwCLK, sizeof(DWORD));
		pFile->Read( &nCL, sizeof(int));
		m_OBJ.m_mapCLKIND.insert( MAPCLKIND::value_type( dwCLK, pCLK));

		for( int j=0; j<nCL; j++)
		{
			LPCLOTH pCL = new CLOTH();
			DWORD dwCL;

			pFile->Read( &dwCL, sizeof(DWORD));
			pCLK->m_mapCLOTH.insert( MAPCLOTH::value_type( dwCL, pCL));
		}
	}
}

void COBJTypeItem::Save( CFile* pFile)
{
	CItemBase::Save(pFile);

	MAPATTRITEM::iterator itATTR;
	MAPACTION::iterator itACT;
	MAPCLKIND::iterator itCLK;
	MAPSTR::iterator itSTR;

	int nCount = INT(m_mapNAME.size());
	pFile->Write( &nCount, sizeof(int));

	for( itSTR = m_mapNAME.begin(); itSTR != m_mapNAME.end(); itSTR++)
	{
		pFile->Write( &(*itSTR).first, sizeof(DWORD));
		WriteString( pFile, (*itSTR).second);
	}

	nCount = INT(m_mapATTR.size());
	pFile->Write( &nCount, sizeof(int));

	for( itATTR = m_mapATTR.begin(); itATTR != m_mapATTR.end(); itATTR++)
	{
		LPATTRITEM pATTR = (*itATTR).second;

		pFile->Write( &(*itATTR).first, sizeof(DWORD));
		WriteString( pFile, pATTR->m_strValue);
		pFile->Write( &pATTR->m_bType, sizeof(BYTE));
	}

	nCount = INT(m_OBJ.m_mapACTION.size());
	pFile->Write( &nCount, sizeof(int));

	for( itACT = m_OBJ.m_mapACTION.begin(); itACT != m_OBJ.m_mapACTION.end(); itACT++)
		pFile->Write( &(*itACT).first, sizeof(DWORD));

	nCount = INT(m_OBJ.m_mapCLKIND.size());
	pFile->Write( &nCount, sizeof(int));

	for( itCLK = m_OBJ.m_mapCLKIND.begin(); itCLK != m_OBJ.m_mapCLKIND.end(); itCLK++)
	{
		LPCLKIND pCLK = (*itCLK).second;
		MAPCLOTH::iterator itCL;

		pFile->Write( &(*itCLK).first, sizeof(DWORD));
		nCount = INT(pCLK->m_mapCLOTH.size());
		pFile->Write( &nCount, sizeof(int));

		for( itCL = pCLK->m_mapCLOTH.begin(); itCL != pCLK->m_mapCLOTH.end(); itCL++)
			pFile->Write( &(*itCL).first, sizeof(DWORD));
	}
}


//////////////////////////////////////////////////////////////////
// COBJItem Function...
//////////////////////////////////////////////////////////////////

COBJItem::COBJItem()
:CItemBase(ITEM_OBJECT)
{
	m_vDropTarget.push_back(ITEM_PROJECT);
	m_vDropTarget.push_back(ITEM_FOLDER);

	m_nDropEffect = DROPEFFECT_MOVE;
	m_bPreviewID = TRUE;
	m_bNeedID = TRUE;
	m_bMenuID = 4;

	m_OBJ.m_bPivotCount = MAX_PIVOT;
	m_TOBJ.InitOBJ(&m_OBJ);
	m_mapPIVOT.clear();
	m_mapATTR.clear();

	m_mapCOLOR.clear();
	m_mapNAME.clear();
	m_mapMESH.clear();
	m_mapTEX.clear();
	m_mapANI.clear();
	m_mapSFX.clear();
	m_mapTI.clear();

	m_mapOBJItem.insert( MAPITEM::value_type( m_dwID, this));
}

COBJItem::~COBJItem()
{
	MAPITEM::iterator finder = m_mapOBJItem.find(m_dwID);
	MAPATTRITEM::iterator itATTR;
	MAPTEXINDEX::iterator itTI;
	OnDelete();

	if( finder != m_mapOBJItem.end() )
		m_mapOBJItem.erase(finder);

	for( itATTR = m_mapATTR.begin(); itATTR != m_mapATTR.end(); itATTR++)
		delete (*itATTR).second;

	for( itTI = m_mapCOLOR.begin(); itTI != m_mapCOLOR.end(); itTI++)
		delete (*itTI).second;

	for( itTI = m_mapTI.begin(); itTI != m_mapTI.end(); itTI++)
		delete (*itTI).second;

	m_mapPIVOT.clear();
	m_mapATTR.clear();

	m_mapCOLOR.clear();
	m_mapNAME.clear();
	m_mapMESH.clear();
	m_mapTEX.clear();
	m_mapANI.clear();
	m_mapSFX.clear();
	m_mapTI.clear();
}

void COBJItem::OnDelete()
{
	CItemBase::OnDelete();

	if( CMAPOBJBar::m_pOBJ == this )
		CMAPOBJBar::SetOBJ(NULL);

	if( COBJBar::m_pDEST == this )
		COBJBar::SetDEST(NULL);
}

void COBJItem::Load( CFile* pFile)
{
	MAPITEM::iterator finder = m_mapOBJItem.find(m_dwID);
	if( finder != m_mapOBJItem.end() )
		m_mapOBJItem.erase(finder);
	CItemBase::Load(pFile);
	m_mapOBJItem.insert( MAPITEM::value_type( m_dwID, this));

	int nCount = 0;
	pFile->Read( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		CString strID;
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		strID = LoadString(pFile);

		if( m_bMerge && !strID.IsEmpty() )
			RegLocalID( strID, this);

		m_mapNAME.insert( MAPSTR::value_type( dwID, strID));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		CString strID;
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		strID = LoadString(pFile);

		if( m_bMerge && !strID.IsEmpty() )
			RegLocalID( strID, this);

		m_mapPIVOT.insert( MAPSTR::value_type( dwID, strID));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		DWORD dwItemID;
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		pFile->Read( &dwItemID, sizeof(DWORD));

		if( m_bMerge && dwItemID )
			dwItemID += m_dwMeshID;

		m_mapMESH.insert( MAPDWORD::value_type( dwID, dwItemID));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		DWORD dwItemID;
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		pFile->Read( &dwItemID, sizeof(DWORD));

		if( m_bMerge && dwItemID )
			dwItemID += m_dwTextureID;

		m_mapTEX.insert( MAPDWORD::value_type( dwID, dwItemID));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		DWORD dwItemID;
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		pFile->Read( &dwItemID, sizeof(DWORD));

		if( m_bMerge && dwItemID )
			dwItemID += m_dwAnimationID;

		m_mapANI.insert( MAPDWORD::value_type( dwID, dwItemID));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		DWORD dwItemID;
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		pFile->Read( &dwItemID, sizeof(DWORD));

		if( m_bMerge && dwItemID )
			dwItemID += m_dwSFXID;

		m_mapSFX.insert( MAPDWORD::value_type( dwID, dwItemID));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPATTRITEM pATTR = new ATTRITEM();
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		pATTR->m_strValue = LoadString(pFile);
		pFile->Read( &pATTR->m_bType, sizeof(BYTE));

		m_mapATTR.insert( MAPATTRITEM::value_type( dwID, pATTR));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPTEXINDEX pTI = new TEXINDEX();
		DWORD dwTI;

		pFile->Read( &dwTI, sizeof(DWORD));
		pFile->Read( &pTI->m_dwMeshID, sizeof(DWORD));
		pFile->Read( &pTI->m_nIndex, sizeof(int));
		pFile->Read( &pTI->m_nStage, sizeof(int));

		m_mapCOLOR.insert( MAPTEXINDEX::value_type( dwTI, pTI));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPTEXINDEX pTI = new TEXINDEX();
		DWORD dwTI;

		pFile->Read( &dwTI, sizeof(DWORD));
		pFile->Read( &pTI->m_dwMeshID, sizeof(DWORD));
		pFile->Read( &pTI->m_nIndex, sizeof(int));
		pFile->Read( &pTI->m_nStage, sizeof(int));

		m_mapTI.insert( MAPTEXINDEX::value_type( dwTI, pTI));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPSFXINST pSFX = new SFXINST();
		DWORD dwSFX;

		pFile->Read( &dwSFX, sizeof(DWORD));
		pFile->Read( &pSFX->m_vPOS, sizeof(D3DXVECTOR3));
		pFile->Read( &pSFX->m_vROT, sizeof(D3DXVECTOR3));
		pFile->Read( &pSFX->m_nPivotID, sizeof(int));
		pFile->Read( &pSFX->m_bBias, sizeof(BYTE));
		pFile->Read( &pSFX->m_dwFuncID, sizeof(DWORD));
		pFile->Read( &pSFX->m_dwTick, sizeof(DWORD));
		pFile->Read( &pSFX->m_bDeleteOnStop, sizeof(BOOL));

#ifdef HEBA
	#ifdef HEBA_CONVERT
	#else
			int nHebaCount=0;
			pFile->Read( &nHebaCount, sizeof(BOOL));
			for( int j=0 ; j < nHebaCount ; ++j )
			{
				DWORD dwHEBA=0;
				pFile->Read( &dwHEBA, sizeof(dwHEBA));
				pSFX->m_mapHideByAction.insert( MAPDWORD::value_type(dwHEBA, dwHEBA) );
			}
	#endif
#endif

		m_OBJ.m_mapSFX.insert( MAPSFXINST::value_type( dwSFX, pSFX));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPSNDINST pSND = new SNDINST();
		DWORD dwSND;

		pFile->Read( &dwSND, sizeof(DWORD));
		pFile->Read( &pSND->m_dwMediaID, sizeof(DWORD));
		pFile->Read( &pSND->m_dwFuncID, sizeof(DWORD));
		pFile->Read( &pSND->m_dwTick, sizeof(DWORD));
		pFile->Read( &pSND->m_bDeleteOnStop, sizeof(BOOL));

		if( m_bMerge && pSND->m_dwMediaID )
			pSND->m_dwMediaID += m_dwMediaID;

		m_OBJ.m_mapSND.insert( MAPSNDINST::value_type( dwSND, pSND));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPACTION pACT = new ACTION;
		MAPANISET::iterator itANI;
		DWORD dwACT;
		int nSFX;
		int nSND;
		int nANI;

		pFile->Read( &dwACT, sizeof(DWORD));
		pFile->Read( &nSFX, sizeof(int));

		for( int j=0; j<nSFX; j++)
		{
			LPSFXINST pSFX = new SFXINST();
			DWORD dwSFX;

			pFile->Read( &dwSFX, sizeof(DWORD));
			pFile->Read( &pSFX->m_vPOS, sizeof(D3DXVECTOR3));
			pFile->Read( &pSFX->m_vROT, sizeof(D3DXVECTOR3));
			pFile->Read( &pSFX->m_nPivotID, sizeof(int));
			pFile->Read( &pSFX->m_bBias, sizeof(BYTE));
			pFile->Read( &pSFX->m_dwFuncID, sizeof(DWORD));
			pFile->Read( &pSFX->m_dwTick, sizeof(DWORD));
			pFile->Read( &pSFX->m_bDeleteOnStop, sizeof(BOOL));

			pACT->m_mapSFX.insert( MAPSFXINST::value_type( dwSFX, pSFX));
		}

		pFile->Read( &nSND, sizeof(int));
		for( j=0; j<nSND; j++)
		{
			LPSNDINST pSND = new SNDINST();
			DWORD dwSND;

			pFile->Read( &dwSND, sizeof(DWORD));
			pFile->Read( &pSND->m_dwMediaID, sizeof(DWORD));
			pFile->Read( &pSND->m_dwFuncID, sizeof(DWORD));
			pFile->Read( &pSND->m_dwTick, sizeof(DWORD));
			pFile->Read( &pSND->m_bDeleteOnStop, sizeof(BOOL));

			if( m_bMerge && pSND->m_dwMediaID )
				pSND->m_dwMediaID += m_dwMediaID;

			pACT->m_mapSND.insert( MAPSNDINST::value_type( dwSND, pSND));
		}

		pFile->Read( &nANI, sizeof(int));
		for( j=0; j<nANI; j++)
		{
			LPANI pANI = new ANI();
			DWORD dwANI;

			pFile->Read( &dwANI, sizeof(DWORD));
			pFile->Read( &nSFX, sizeof(int));

			for( int k=0; k<nSFX; k++)
			{
				LPSFXINST pSFX = new SFXINST();
				DWORD dwSFX;

				pFile->Read( &dwSFX, sizeof(DWORD));
				pFile->Read( &pSFX->m_vPOS, sizeof(D3DXVECTOR3));
				pFile->Read( &pSFX->m_vROT, sizeof(D3DXVECTOR3));
				pFile->Read( &pSFX->m_nPivotID, sizeof(int));
				pFile->Read( &pSFX->m_bBias, sizeof(BYTE));
				pFile->Read( &pSFX->m_dwFuncID, sizeof(DWORD));
				pFile->Read( &pSFX->m_dwTick, sizeof(DWORD));
				pFile->Read( &pSFX->m_bDeleteOnStop, sizeof(BOOL));

				pANI->m_mapSFX.insert( MAPSFXINST::value_type( dwSFX, pSFX));
			}

			pFile->Read( &nSND, sizeof(int));
			for( k=0; k<nSND; k++)
			{
				LPSNDINST pSND = new SNDINST();
				DWORD dwSND;

				pFile->Read( &dwSND, sizeof(DWORD));
				pFile->Read( &pSND->m_dwMediaID, sizeof(DWORD));
				pFile->Read( &pSND->m_dwFuncID, sizeof(DWORD));
				pFile->Read( &pSND->m_dwTick, sizeof(DWORD));
				pFile->Read( &pSND->m_bDeleteOnStop, sizeof(BOOL));

				if( m_bMerge && pSND->m_dwMediaID )
					pSND->m_dwMediaID += m_dwMediaID;

				pANI->m_mapSND.insert( MAPSNDINST::value_type( dwSND, pSND));
			}

			pACT->m_mapANI.insert( MAPANI::value_type( dwANI, pANI));
		}
		m_OBJ.m_mapACTION.insert( MAPACTION::value_type( dwACT, pACT));
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPCLKIND pCLK = new CLKIND();
		DWORD dwCLK;
		int nCL;

		pFile->Read( &dwCLK, sizeof(DWORD));
		pFile->Read( &nCL, sizeof(int));
		m_OBJ.m_mapCLKIND.insert( MAPCLKIND::value_type( dwCLK, pCLK));

		for( int j=0; j<nCL; j++)
		{
			LPCLOTH pCL = new CLOTH();
			DWORD dwCL;
			int nMESH;

			pFile->Read( &dwCL, sizeof(DWORD));
			pFile->Read( &nMESH, sizeof(int));
			pCLK->m_mapCLOTH.insert( MAPCLOTH::value_type( dwCL, pCL));

			for( int k=0; k<nMESH; k++)
			{
				LPMESH pMESH = new MESH();
				DWORD dwMESH;
				int nTEX;

				pFile->Read( &dwMESH, sizeof(DWORD));
				pFile->Read( &nTEX, sizeof(int));
				pCL->m_mapMESH.insert( MAPMESH::value_type( dwMESH, pMESH));

				for( int l=0; l<nTEX; l++)
				{
					LPOBJTEX pTEX = new OBJTEX();

					pFile->Read( &pTEX->m_bType[0], sizeof(BYTE));
					pFile->Read( &pTEX->m_bType[1], sizeof(BYTE));
					pFile->Read( &pTEX->m_dwCOLORID, sizeof(DWORD));
					pFile->Read( &pTEX->m_dwCOLOR, sizeof(DWORD));
					pFile->Read( &pTEX->m_dwOrder, sizeof(DWORD));
					pFile->Read( &pTEX->m_dwOP, sizeof(DWORD));
					pFile->Read( &pTEX->m_dwDESTBlend, sizeof(DWORD));
					pFile->Read( &pTEX->m_dwSRCBlend, sizeof(DWORD));
					pFile->Read( &pTEX->m_dwBlendOP, sizeof(DWORD));
					pFile->Read( &pTEX->m_fIntensity, sizeof(FLOAT));
					pFile->Read( &pTEX->m_bMinAlpha, sizeof(BYTE));
					pFile->Read( &pTEX->m_bZEnable, sizeof(BYTE));
					pFile->Read( &pTEX->m_bZWrite, sizeof(BYTE));
					pFile->Read( &pTEX->m_b2side, sizeof(BYTE));
					pFile->Read( &pTEX->m_bUseDirLight, sizeof(BOOL));
					pFile->Read( &pTEX->m_dwAmbientCOLOR, sizeof(DWORD));

					pMESH->m_vTEX.push_back(pTEX);
				}
			}
		}
	}

	m_OBJ.m_bPivotCount = MAX_PIVOT;
	m_TOBJ.InitOBJ(&m_OBJ);
}

void COBJItem::Save( CFile* pFile)
{
	CItemBase::Save(pFile);

	MAPATTRITEM::iterator itATTR;
	MAPTEXINDEX::iterator itTI;
	MAPACTION::iterator itACT;
	MAPCLKIND::iterator itCLK;

	MAPSFXINST::iterator itSFX;
	MAPSNDINST::iterator itSND;

	MAPDWORD::iterator itDWORD;
	MAPSTR::iterator itSTR;

	int nCount = INT(m_mapNAME.size());
	pFile->Write( &nCount, sizeof(int));

	for( itSTR = m_mapNAME.begin(); itSTR != m_mapNAME.end(); itSTR++)
	{
		pFile->Write( &(*itSTR).first, sizeof(DWORD));
		WriteString( pFile, (*itSTR).second);
	}

	nCount = INT(m_mapPIVOT.size());
	pFile->Write( &nCount, sizeof(int));

	for( itSTR = m_mapPIVOT.begin(); itSTR != m_mapPIVOT.end(); itSTR++)
	{
		pFile->Write( &(*itSTR).first, sizeof(DWORD));
		WriteString( pFile, (*itSTR).second);
	}

	nCount = INT(m_mapMESH.size());
	pFile->Write( &nCount, sizeof(int));

	for( itDWORD = m_mapMESH.begin(); itDWORD != m_mapMESH.end(); itDWORD++)
	{
		pFile->Write( &(*itDWORD).first, sizeof(DWORD));
		pFile->Write( &(*itDWORD).second, sizeof(DWORD));
	}

	nCount = INT(m_mapTEX.size());
	pFile->Write( &nCount, sizeof(int));

	for( itDWORD = m_mapTEX.begin(); itDWORD != m_mapTEX.end(); itDWORD++)
	{
		pFile->Write( &(*itDWORD).first, sizeof(DWORD));
		pFile->Write( &(*itDWORD).second, sizeof(DWORD));
	}

	nCount = INT(m_mapANI.size());
	pFile->Write( &nCount, sizeof(int));

	for( itDWORD = m_mapANI.begin(); itDWORD != m_mapANI.end(); itDWORD++)
	{
		pFile->Write( &(*itDWORD).first, sizeof(DWORD));
		pFile->Write( &(*itDWORD).second, sizeof(DWORD));
	}

	nCount = INT(m_mapSFX.size());
	pFile->Write( &nCount, sizeof(int));

	for( itDWORD = m_mapSFX.begin(); itDWORD != m_mapSFX.end(); itDWORD++)
	{
		pFile->Write( &(*itDWORD).first, sizeof(DWORD));
		pFile->Write( &(*itDWORD).second, sizeof(DWORD));
	}

	nCount = INT(m_mapATTR.size());
	pFile->Write( &nCount, sizeof(int));

	for( itATTR = m_mapATTR.begin(); itATTR != m_mapATTR.end(); itATTR++)
	{
		LPATTRITEM pATTR = (*itATTR).second;

		pFile->Write( &(*itATTR).first, sizeof(DWORD));
		WriteString( pFile, pATTR->m_strValue);
		pFile->Write( &pATTR->m_bType, sizeof(BYTE));
	}

	nCount = INT(m_mapCOLOR.size());
	pFile->Write( &nCount, sizeof(int));

	for( itTI = m_mapCOLOR.begin(); itTI != m_mapCOLOR.end(); itTI++)
	{
		LPTEXINDEX pTI = (*itTI).second;

		pFile->Write( &(*itTI).first, sizeof(DWORD));
		pFile->Write( &pTI->m_dwMeshID, sizeof(DWORD));
		pFile->Write( &pTI->m_nIndex, sizeof(int));
		pFile->Write( &pTI->m_nStage, sizeof(int));
	}

	nCount = INT(m_mapTI.size());
	pFile->Write( &nCount, sizeof(int));

	for( itTI = m_mapTI.begin(); itTI != m_mapTI.end(); itTI++)
	{
		LPTEXINDEX pTI = (*itTI).second;

		pFile->Write( &(*itTI).first, sizeof(DWORD));
		pFile->Write( &pTI->m_dwMeshID, sizeof(DWORD));
		pFile->Write( &pTI->m_nIndex, sizeof(int));
		pFile->Write( &pTI->m_nStage, sizeof(int));
	}

	nCount = INT(m_OBJ.m_mapSFX.size());
	pFile->Write( &nCount, sizeof(int));

	for( itSFX = m_OBJ.m_mapSFX.begin(); itSFX != m_OBJ.m_mapSFX.end(); itSFX++)
	{
		pFile->Write( &(*itSFX).first, sizeof(DWORD));
		pFile->Write( &(*itSFX).second->m_vPOS, sizeof(D3DXVECTOR3));
		pFile->Write( &(*itSFX).second->m_vROT, sizeof(D3DXVECTOR3));
		pFile->Write( &(*itSFX).second->m_nPivotID, sizeof(int));
		pFile->Write( &(*itSFX).second->m_bBias, sizeof(BYTE));
		pFile->Write( &(*itSFX).second->m_dwFuncID, sizeof(DWORD));
		pFile->Write( &(*itSFX).second->m_dwTick, sizeof(DWORD));
		pFile->Write( &(*itSFX).second->m_bDeleteOnStop, sizeof(BOOL));
#ifdef HEBA
		int nHebaCount = (*itSFX).second->m_mapHideByAction.size();
		pFile->Write( &nHebaCount, sizeof(int));

		MAPDWORD::iterator itHEBA, endHEBA;
		itHEBA = (*itSFX).second->m_mapHideByAction.begin();
		endHEBA = (*itSFX).second->m_mapHideByAction.end();
		for(; itHEBA != endHEBA ; ++itHEBA )
		{
			DWORD dwHEBA = itHEBA->first;
			pFile->Write( &dwHEBA, sizeof(DWORD));
		}
#endif
	}

	nCount = INT(m_OBJ.m_mapSND.size());
	pFile->Write( &nCount, sizeof(int));

	for( itSND = m_OBJ.m_mapSND.begin(); itSND != m_OBJ.m_mapSND.end(); itSND++)
	{
		pFile->Write( &(*itSND).first, sizeof(DWORD));
		pFile->Write( &(*itSND).second->m_dwMediaID, sizeof(DWORD));
		pFile->Write( &(*itSND).second->m_dwFuncID, sizeof(DWORD));
		pFile->Write( &(*itSND).second->m_dwTick, sizeof(DWORD));
		pFile->Write( &(*itSND).second->m_bDeleteOnStop, sizeof(BOOL));
	}

	nCount = INT(m_OBJ.m_mapACTION.size());
	pFile->Write( &nCount, sizeof(int));

	for( itACT = m_OBJ.m_mapACTION.begin(); itACT != m_OBJ.m_mapACTION.end(); itACT++)
	{
		LPACTION pACT = (*itACT).second;
		MAPANI::iterator itANI;

		pFile->Write( &(*itACT).first, sizeof(DWORD));
		nCount = INT(pACT->m_mapSFX.size());
		pFile->Write( &nCount, sizeof(int));

		for( itSFX = pACT->m_mapSFX.begin(); itSFX != pACT->m_mapSFX.end(); itSFX++)
		{
			pFile->Write( &(*itSFX).first, sizeof(DWORD));
			pFile->Write( &(*itSFX).second->m_vPOS, sizeof(D3DXVECTOR3));
			pFile->Write( &(*itSFX).second->m_vROT, sizeof(D3DXVECTOR3));
			pFile->Write( &(*itSFX).second->m_nPivotID, sizeof(int));
			pFile->Write( &(*itSFX).second->m_bBias, sizeof(BYTE));
			pFile->Write( &(*itSFX).second->m_dwFuncID, sizeof(DWORD));
			pFile->Write( &(*itSFX).second->m_dwTick, sizeof(DWORD));
			pFile->Write( &(*itSFX).second->m_bDeleteOnStop, sizeof(BOOL));
		}

		nCount = INT(pACT->m_mapSND.size());
		pFile->Write( &nCount, sizeof(int));

		for( itSND = pACT->m_mapSND.begin(); itSND != pACT->m_mapSND.end(); itSND++)
		{
			pFile->Write( &(*itSND).first, sizeof(DWORD));
			pFile->Write( &(*itSND).second->m_dwMediaID, sizeof(DWORD));
			pFile->Write( &(*itSND).second->m_dwFuncID, sizeof(DWORD));
			pFile->Write( &(*itSND).second->m_dwTick, sizeof(DWORD));
			pFile->Write( &(*itSND).second->m_bDeleteOnStop, sizeof(BOOL));
		}

		nCount = INT(pACT->m_mapANI.size());
		pFile->Write( &nCount, sizeof(int));

		for( itANI = pACT->m_mapANI.begin(); itANI != pACT->m_mapANI.end(); itANI++)
		{
			LPANI pANI = (*itANI).second;

			pFile->Write( &(*itANI).first, sizeof(DWORD));
			nCount = INT(pANI->m_mapSFX.size());
			pFile->Write( &nCount, sizeof(int));

			for( itSFX = pANI->m_mapSFX.begin(); itSFX != pANI->m_mapSFX.end(); itSFX++)
			{
				pFile->Write( &(*itSFX).first, sizeof(DWORD));
				pFile->Write( &(*itSFX).second->m_vPOS, sizeof(D3DXVECTOR3));
				pFile->Write( &(*itSFX).second->m_vROT, sizeof(D3DXVECTOR3));
				pFile->Write( &(*itSFX).second->m_nPivotID, sizeof(int));
				pFile->Write( &(*itSFX).second->m_bBias, sizeof(BYTE));
				pFile->Write( &(*itSFX).second->m_dwFuncID, sizeof(DWORD));
				pFile->Write( &(*itSFX).second->m_dwTick, sizeof(DWORD));
				pFile->Write( &(*itSFX).second->m_bDeleteOnStop, sizeof(BOOL));
			}

			nCount = INT(pANI->m_mapSND.size());
			pFile->Write( &nCount, sizeof(int));

			for( itSND = pANI->m_mapSND.begin(); itSND != pANI->m_mapSND.end(); itSND++)
			{
				pFile->Write( &(*itSND).first, sizeof(DWORD));
				pFile->Write( &(*itSND).second->m_dwMediaID, sizeof(DWORD));
				pFile->Write( &(*itSND).second->m_dwFuncID, sizeof(DWORD));
				pFile->Write( &(*itSND).second->m_dwTick, sizeof(DWORD));
				pFile->Write( &(*itSND).second->m_bDeleteOnStop, sizeof(BOOL));
			}
		}
	}

	nCount = INT(m_OBJ.m_mapCLKIND.size());
	pFile->Write( &nCount, sizeof(int));

	for( itCLK = m_OBJ.m_mapCLKIND.begin(); itCLK != m_OBJ.m_mapCLKIND.end(); itCLK++)
	{
		LPCLKIND pCLK = (*itCLK).second;
		MAPCLOTH::iterator itCL;

		pFile->Write( &(*itCLK).first, sizeof(DWORD));
		nCount = INT(pCLK->m_mapCLOTH.size());
		pFile->Write( &nCount, sizeof(int));

		for( itCL = pCLK->m_mapCLOTH.begin(); itCL != pCLK->m_mapCLOTH.end(); itCL++)
		{
			LPCLOTH pCL = (*itCL).second;
			MAPMESH::iterator itMESH;

			pFile->Write( &(*itCL).first, sizeof(DWORD));
			nCount = INT(pCL->m_mapMESH.size());
			pFile->Write( &nCount, sizeof(int));

			for( itMESH = pCL->m_mapMESH.begin(); itMESH != pCL->m_mapMESH.end(); itMESH++)
			{
				LPMESH pMESH = (*itMESH).second;

				pFile->Write( &(*itMESH).first, sizeof(DWORD));
				nCount = INT(pMESH->m_vTEX.size());
				pFile->Write( &nCount, sizeof(int));

				for( int i=0; i<nCount; i++)
				{
					LPOBJTEX pTEX = pMESH->m_vTEX[i];

					pFile->Write( &pTEX->m_bType[0], sizeof(BYTE));
					pFile->Write( &pTEX->m_bType[1], sizeof(BYTE));
					pFile->Write( &pTEX->m_dwCOLORID, sizeof(DWORD));
					pFile->Write( &pTEX->m_dwCOLOR, sizeof(DWORD));
					pFile->Write( &pTEX->m_dwOrder, sizeof(DWORD));
					pFile->Write( &pTEX->m_dwOP, sizeof(DWORD));
					pFile->Write( &pTEX->m_dwDESTBlend, sizeof(DWORD));
					pFile->Write( &pTEX->m_dwSRCBlend, sizeof(DWORD));
					pFile->Write( &pTEX->m_dwBlendOP, sizeof(DWORD));
					pFile->Write( &pTEX->m_fIntensity, sizeof(FLOAT));
					pFile->Write( &pTEX->m_bMinAlpha, sizeof(BYTE));
					pFile->Write( &pTEX->m_bZEnable, sizeof(BYTE));
					pFile->Write( &pTEX->m_bZWrite, sizeof(BYTE));
					pFile->Write( &pTEX->m_b2side, sizeof(BYTE));
					pFile->Write( &pTEX->m_bUseDirLight, sizeof(BOOL));
					pFile->Write( &pTEX->m_dwAmbientCOLOR, sizeof(DWORD));
				}
			}
		}
	}
}

BOOL COBJItem::GetSize( D3DXVECTOR3& vMin, D3DXVECTOR3& vMax)
{
	if(m_TOBJ.GetSize( vMin, vMax))
		return TRUE;

	MAPACTINST::iterator finder = m_TOBJ.m_OBJ.m_mapACT.find(m_TOBJ.m_OBJ.m_dwCurACT);
	if( finder != m_TOBJ.m_OBJ.m_mapACT.end() )
	{
		LPACTINST pAI = (*finder).second;

		if( pAI && pAI->m_pANI && pAI->m_pANI->m_pANI )
		{
			pAI->m_pANI->m_pANI->m_fCurTime = m_TOBJ.m_fActTime;
			LPANIDATA pANI = pAI->m_pANI->m_pANI->GetAniData();

			if( pANI && pANI->m_pAni &&
				pANI->m_pAni->GetSize( vMin, vMax) )
				return TRUE;
		}
	}

	return FALSE;
}

DWORD COBJItem::GetColorID( DWORD dwMeshID, int nIndex)
{
	MAPTEXINDEX::iterator itTI;

	for( itTI = m_mapCOLOR.begin(); itTI != m_mapCOLOR.end(); itTI++)
	{
		LPTEXINDEX pTI = (*itTI).second;

		if( pTI->m_dwMeshID == dwMeshID &&
			pTI->m_nIndex == nIndex )
			return (*itTI).first;
	}

	return 0;
}

DWORD COBJItem::GetTexID( DWORD dwMeshID,
						  int nIndex,
						  int nStage)
{
	MAPTEXINDEX::iterator it;

	for( it = m_mapTI.begin(); it != m_mapTI.end(); it++)
	{
		LPTEXINDEX pTEX = (*it).second;

		if( pTEX && pTEX->m_dwMeshID == dwMeshID &&
			pTEX->m_nIndex == nIndex &&
			pTEX->m_nStage == nStage )
			return (*it).first;
	}

	return 0;
}

int COBJItem::GetPivotCount( LPANISET pItem)
{
	if( pItem->m_vAni.empty() || !pItem->m_vAni[0]->m_pAni )
		return 0;

	return INT(pItem->m_vAni[0]->m_pAni->GetNodeCount());
}

int COBJItem::GetPivotCount()
{
	MAPDWORD::iterator it;

	for( it = m_mapANI.begin(); it != m_mapANI.end(); it++)
	{
		CAnimationItem *pItem = CItemBase::GetAnimationItem((*it).second);

		if( pItem && !pItem->m_bDeleted )
			return GetPivotCount(&pItem->m_Aniset);
	}

	for( it = m_mapMESH.begin(); it != m_mapMESH.end(); it++)
	{
		CMeshItem *pItem = CItemBase::GetMeshItem((*it).second);

		if( pItem && !pItem->m_bDeleted )
			return pItem->m_Mesh.m_dwNodeCount;
	}

	return -1;
}

DWORD COBJItem::GetAttributeID(CString strNAME)
{
	MAPSTR::iterator itNAME;
	for( itNAME = m_mapNAME.begin(); itNAME != m_mapNAME.end(); itNAME++)
		if((*itNAME).second == strNAME)
			break;

	if(itNAME != m_mapNAME.end())
		return (*itNAME).first;

	return -1;
}

void COBJItem::DeleteTEX( DWORD dwMeshID,
						  LPMESH pMESH,
						  int nTEX)
{
	if( !pMESH || nTEX >= INT(pMESH->m_vTEX.size()) )
		return;
	DWORD dwCOLOR = GetColorID( dwMeshID, nTEX);

	if( dwCOLOR > 0 )
	{
		MAPTEXINDEX::iterator itTI = m_mapCOLOR.find(dwCOLOR);
		MAPSTR::iterator itID = m_mapNAME.find(dwCOLOR);

		ReleaseLocalID( (*itID).second, this);
		m_mapNAME.erase(itID);

		delete (*itTI).second;
		m_mapCOLOR.erase(itTI);
	}

	for( int i=0; i<2; i++)
	{
		DWORD dwTEX = GetTexID(
			dwMeshID,
			nTEX, i);

		if( dwTEX > 0 )
		{
			MAPTEXINDEX::iterator itTI = m_mapTI.find(dwTEX);

			m_mapTEX.erase(m_mapTEX.find(dwTEX));
			delete (*itTI).second;
			m_mapTI.erase(itTI);
		}
	}

	delete pMESH->m_vTEX[nTEX];
	pMESH->m_vTEX.erase(pMESH->m_vTEX.begin() + nTEX);
}

void COBJItem::Regulate()
{
	MAPACTION::iterator itACT;
	MAPCLKIND::iterator itCLK;

	Regulate(
		&m_OBJ.m_mapSFX,
		&m_OBJ.m_mapSND);

	for( itACT = m_OBJ.m_mapACTION.begin(); itACT != m_OBJ.m_mapACTION.end(); itACT++)
	{
		LPACTION pACT = (*itACT).second;
		MAPANI::iterator itANI = pACT->m_mapANI.begin();

		Regulate(
			&pACT->m_mapSFX,
			&pACT->m_mapSND);

		while(itANI != pACT->m_mapANI.end())
		{
			MAPDWORD::iterator finder = m_mapANI.find((*itANI).first);
			CAnimationItem *pANI = GetAnimationItem((*finder).second);

			if( pANI && !pANI->m_bDeleted )
			{
				Regulate(
					&(*itANI).second->m_mapSFX,
					&(*itANI).second->m_mapSND);

				(*itANI).second->m_pANI = &pANI->m_Aniset;
				itANI++;
			}
			else
			{
				MAPANI::iterator itNEXT = itANI;
				MAPSFXINST::iterator itSFX;
				itNEXT++;

				for( itSFX = (*itANI).second->m_mapSFX.begin(); itSFX != (*itANI).second->m_mapSFX.end(); itSFX++)
				{
					MAPDWORD::iterator itID = m_mapSFX.find((*itSFX).first);

					if(itID != m_mapSFX.end())
						m_mapSFX.erase(itID);
				}

				MAPSTR::iterator itID = m_mapNAME.find((*itANI).first);

				ReleaseLocalID( (*itID).second, this);
				m_mapNAME.erase(itID);
				m_mapANI.erase(finder);
				delete (*itANI).second;
				pACT->m_mapANI.erase(itANI);

				itANI = itNEXT;
			}
		}
	}

	for( itCLK = m_OBJ.m_mapCLKIND.begin(); itCLK != m_OBJ.m_mapCLKIND.end(); itCLK++)
	{
		LPCLKIND pCLK = (*itCLK).second;
		MAPCLOTH::iterator itCL;

		for( itCL = pCLK->m_mapCLOTH.begin(); itCL != pCLK->m_mapCLOTH.end(); itCL++)
		{
			LPCLOTH pCL = (*itCL).second;
			MAPMESH::iterator itMESH = pCL->m_mapMESH.begin();

			while(itMESH != pCL->m_mapMESH.end())
			{
				LPMESH pMESH = (*itMESH).second;

				MAPDWORD::iterator finder = m_mapMESH.find((*itMESH).first);
				CMeshItem *pDATA = GetMeshItem((*finder).second);

				if( pDATA && !pDATA->m_bDeleted )
				{
					int nCount = INT(pMESH->m_vTEX.size());

					while(nCount > pDATA->m_Mesh.m_dwMeshCount)
					{
						DeleteTEX( (*itMESH).first, pMESH, nCount - 1);
						nCount = INT(pMESH->m_vTEX.size());
					}

					while(nCount < pDATA->m_Mesh.m_dwMeshCount)
					{
						LPOBJTEX pTEX = new OBJTEX();

						pTEX->m_dwOrder = DWORD(nCount);
						pMESH->m_vTEX.push_back(pTEX);
						nCount = INT(pMESH->m_vTEX.size());
					}

					pMESH->m_pMESH = &pDATA->m_Mesh;
					for( int i=0; i<nCount; i++)
						for( int j=0; j<2; j++)
						{
							DWORD dwTEX = GetTexID(
								(*itMESH).first,
								i, j);
							pMESH->m_vTEX[i]->m_pTEX[j] = NULL;

							if( dwTEX > 0 )
							{
								finder = m_mapTEX.find(dwTEX);
								CTextureItem *pTEX = GetTextureItem((*finder).second);

								if( pTEX && !pTEX->m_bDeleted )
									pMESH->m_vTEX[i]->m_pTEX[j] = pTEX->GetTEXTURESET();
								else
								{
									MAPTEXINDEX::iterator itTI = m_mapTI.find(dwTEX);

									m_mapTEX.erase(m_mapTEX.find(dwTEX));
									delete (*itTI).second;
									m_mapTI.erase(itTI);
								}
							}
						}
					itMESH++;
				}
				else
				{
					MAPMESH::iterator itNEXT = itMESH;
					itNEXT++;

					while(!pMESH->m_vTEX.empty())
						DeleteTEX( (*itMESH).first, pMESH, 0);
					delete pMESH;

					MAPSTR::iterator itID = m_mapNAME.find((*itMESH).first);
					ReleaseLocalID( (*itID).second, this);
					m_mapNAME.erase(itID);
					m_mapMESH.erase(finder);
					pCL->m_mapMESH.erase(itMESH);

					itMESH = itNEXT;
				}
			}
		}
	}

	Regulate(&m_TOBJ);
}

void COBJItem::Regulate( CTachyonObject *pOBJ)
{
	MAPCLKINST::iterator itCI = pOBJ->m_OBJ.m_mapCLK.begin();
	MAPACTINST::iterator itAI = pOBJ->m_OBJ.m_mapACT.begin();

	while(itCI != pOBJ->m_OBJ.m_mapCLK.end())
	{
		LPCLKINST pCI = (*itCI).second;
		MAPCLKIND::iterator itCLK = m_OBJ.m_mapCLKIND.find((*itCI).first);

		if( itCLK == m_OBJ.m_mapCLKIND.end() )
		{
			MAPCLKINST::iterator itNEXT = itCI;
			itNEXT++;

			if( pOBJ->m_OBJ.m_dwCurCLK == (*itCI).first )
				pOBJ->m_OBJ.m_dwCurCLK = 0;

			pOBJ->m_OBJ.m_mapCLK.erase(itCI);
			delete pCI;

			itCI = itNEXT;
		}
		else
		{
			LPCLKIND pCLK = (*itCLK).second;
			MAPCLOTH::iterator itCL = pCLK->m_mapCLOTH.find(pCI->m_dwCurCL);

			if( itCL == pCLK->m_mapCLOTH.end() )
			{
				pCI->m_dwHideID = 0xFFFFFFFF;
				pCI->m_dwCurMESH = 0;
				pCI->m_dwCurCL = 0;

				pCI->m_pMESH = NULL;
			}
			else
			{
				LPCLOTH pCL = (*itCL).second;
				MAPMESH::iterator itMESH = pCL->m_mapMESH.find(pCI->m_dwCurMESH);

				if( itMESH == pCL->m_mapMESH.end() )
				{
					pCI->m_dwHideID = 0xFFFFFFFF;
					pCI->m_dwCurMESH = 0;
					pCI->m_pMESH = NULL;
				}
			}

			itCI++;
		}
	}

	while(itAI != pOBJ->m_OBJ.m_mapACT.end())
	{
		LPACTINST pAI = (*itAI).second;
		MAPACTION::iterator itACT = m_OBJ.m_mapACTION.find((*itAI).first);

		if( itACT == m_OBJ.m_mapACTION.end() )
		{
			MAPACTINST::iterator itNEXT = itAI;
			itNEXT++;

			if( pOBJ->m_OBJ.m_dwCurACT == (*itAI).first )
				pOBJ->m_OBJ.m_dwCurACT = 0;

			pOBJ->m_OBJ.m_mapACT.erase(itAI);
			delete pAI;

			itAI = itNEXT;
		}
		else
		{
			LPACTION pACT = (*itACT).second;
			MAPANI::iterator itANI = pACT->m_mapANI.find(pAI->m_dwCurANI);

			if( itANI == pACT->m_mapANI.end() )
			{
				pAI->m_dwCurANI = 0;
				pAI->m_pANI = NULL;
			}

			itAI++;
		}
	}

	MAPSFXINST::iterator itSFX;
	MAPSNDINST::iterator itSND;

	pOBJ->ClearSFX();
	pOBJ->ClearSND();

	for( itSFX = pOBJ->m_OBJ.m_pOBJ->m_mapSFX.begin(); itSFX != pOBJ->m_OBJ.m_pOBJ->m_mapSFX.end(); itSFX++)
		pOBJ->m_mapOBJSFXINST.insert( MAPSFXINST::value_type( (*itSFX).first, (*itSFX).second));

	for( itSND = pOBJ->m_OBJ.m_pOBJ->m_mapSND.begin(); itSND != pOBJ->m_OBJ.m_pOBJ->m_mapSND.end(); itSND++)
		pOBJ->m_mapOBJSNDINST.insert( MAPSNDINST::value_type( (*itSND).first, (*itSND).second));

	pOBJ->ResetOBJ(&CMainFrame::m_3DDevice);
}

void COBJItem::Regulate( LPMAPSFXINST pSFX, LPMAPSNDINST pSND)
{
	MAPSFXINST::iterator itSFX = pSFX->begin();
	MAPSNDINST::iterator itSND = pSND->begin();

	while(itSFX != pSFX->end())
	{
		MAPDWORD::iterator finder = m_mapSFX.find((*itSFX).first);
		CSFXItem *pItem = GetSFXItem((*finder).second);

		MAPSFXINST::iterator itNEXT = itSFX;
		itNEXT++;

		if( !pItem || pItem->m_bDeleted )
		{
			delete (*itSFX).second;

			m_mapSFX.erase(finder);
			pSFX->erase(itSFX);
		}
		else
			(*itSFX).second->m_pSFX = &pItem->m_SFX;

		itSFX = itNEXT;
	}

	while(itSND != pSND->end())
	{
		CMediaItem *pItem = GetMediaItem((*itSND).second->m_dwMediaID);
		MAPSNDINST::iterator itNEXT = itSND;
		itNEXT++;

		if( !pItem || pItem->m_bDeleted || pItem->GetPlayerType() != MEDIA_TSOUND )
		{
			delete (*itSND).second;
			pSND->erase(itSND);
		}

		itSND = itNEXT;
	}
}


//////////////////////////////////////////////////////////////////
// CSFXItem Function...
//////////////////////////////////////////////////////////////////

CSFXItem::CSFXItem()
:CItemBase(ITEM_SFX)
{
	m_vDropTarget.push_back(ITEM_PROJECT);
	m_vDropTarget.push_back(ITEM_FOLDER);

	m_nDropEffect = DROPEFFECT_MOVE;
	m_bPreviewID = TRUE;
	m_bMenuID = 2;

	m_dwTotalFrame = TOTALFRAME;
	m_dwSANISRC = 0;
	m_dwANISRC = 0;
	m_dwSFXSRC = 0;
	m_dwCurCLK = 0;

	m_mapSFXItem.insert( MAPITEM::value_type( m_dwID, this));
	m_vSFXID.clear();
}

CSFXItem::~CSFXItem()
{
	MAPITEM::iterator finder = m_mapSFXItem.find(m_dwID);
	OnDelete();

	if( finder != m_mapSFXItem.end() )
		m_mapSFXItem.erase(finder);
	m_vSFXID.clear();
}

void CSFXItem::Load( CFile* pFile)
{
	MAPITEM::iterator finder = m_mapSFXItem.find(m_dwID);
	if( finder != m_mapSFXItem.end() )
		m_mapSFXItem.erase(finder);
	CItemBase::Load(pFile);
	m_mapSFXItem.insert( MAPITEM::value_type( m_dwID, this));

	MAPCLKINST::iterator it;
	int nCount = 0;

	pFile->Read( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		if( m_bMerge && dwID )
			dwID += m_dwSFXID;

		m_vSFXID.push_back(dwID);
	}

	pFile->Read( &m_dwTotalFrame, sizeof(DWORD));
	pFile->Read( &m_dwSANISRC, sizeof(DWORD));
	pFile->Read( &m_dwANISRC, sizeof(DWORD));
	pFile->Read( &m_dwSFXSRC, sizeof(DWORD));
	pFile->Read( &m_dwCurCLK, sizeof(DWORD));

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPCLRKEY pKEY = new CLRKEY();

		pFile->Read( &pKEY->m_dwColor, sizeof(DWORD));
		pFile->Read( &pKEY->m_dwTick, sizeof(DWORD));

		m_SFX.m_vKey.push_back(pKEY);
	}

	pFile->Read( &m_SFX.m_bSFXType, sizeof(BYTE));
	pFile->Read( &m_SFX.m_bSRCType, sizeof(BYTE));

	if(m_bMerge)
	{
		if(m_dwSANISRC)
			m_dwSANISRC += m_dwAnimationID;

		if(m_dwANISRC)
			m_dwANISRC += m_dwAnimationID;

		if(m_dwSFXSRC)
			switch(m_SFX.m_bSRCType)
			{
			case SFXSRC_TEX	: m_dwSFXSRC += m_dwTextureID; break;
			case SFXSRC_OBJ	: m_dwSFXSRC += m_dwOBJID; break;
			case SFXSRC_SFX	: m_dwSFXSRC += m_dwSFXID; break;
			}
	}

	pFile->Read( &m_SFX.m_fFollow, sizeof(FLOAT));
	pFile->Read( &m_SFX.m_fSizeX, sizeof(FLOAT));
	pFile->Read( &m_SFX.m_fSizeY, sizeof(FLOAT));

	pFile->Read( &m_SFX.m_dwDESTBlend, sizeof(DWORD));
	pFile->Read( &m_SFX.m_dwSRCBlend, sizeof(DWORD));
	pFile->Read( &m_SFX.m_dwBlendOP, sizeof(DWORD));

	pFile->Read( &m_SFX.m_dwPLifeTime, sizeof(DWORD));
	pFile->Read( &m_SFX.m_dwLifeTime, sizeof(DWORD));

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPCLKINST pCI = new CLKINST();
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		pFile->Read( &pCI->m_dwCurMESH, sizeof(DWORD));
		pFile->Read( &pCI->m_dwCurCL, sizeof(DWORD));

		m_SFX.m_mapCLK.insert( MAPCLKINST::value_type( dwID, pCI));
	}

	pFile->Read( &m_SFX.m_dwOBJFuncID, sizeof(DWORD));
	pFile->Read( &m_SFX.m_dwACTFuncID, sizeof(DWORD));
	pFile->Read( &m_SFX.m_dwANIFuncID, sizeof(DWORD));
	pFile->Read( &m_SFX.m_dwACT, sizeof(DWORD));
	pFile->Read( &m_SFX.m_dwANI, sizeof(DWORD));
	pFile->Read( &m_SFX.m_bSprayType, sizeof(BYTE));
	pFile->Read( &m_SFX.m_bZEnable, sizeof(BYTE));
	pFile->Read( &m_SFX.m_bZWrite, sizeof(BYTE));
	pFile->Read( &m_SFX.m_bStrict, sizeof(BYTE));
	pFile->Read( &m_SFX.m_bFollow, sizeof(BYTE));
	pFile->Read( &m_SFX.m_dwGTime, sizeof(DWORD));
	pFile->Read( &m_SFX.m_dwGCnt, sizeof(DWORD));

	pFile->Read( &m_SFX.m_vOutBound, sizeof(D3DXVECTOR3));
	pFile->Read( &m_SFX.m_vInBound, sizeof(D3DXVECTOR3));
	pFile->Read( &m_SFX.m_dwERRTime, sizeof(DWORD));

	pFile->Read( &m_SFX.m_vERRPos, sizeof(D3DXVECTOR3));
	pFile->Read( &m_SFX.m_vERRRot, sizeof(D3DXVECTOR3));
	pFile->Read( &m_SFX.m_vERRScale, sizeof(D3DXVECTOR3));
	pFile->Read( &m_SFX.m_dwLodLevel, sizeof(DWORD));
}

void CSFXItem::Save( CFile* pFile)
{
	CItemBase::Save(pFile);

	int nCount = INT(m_vSFXID.size());
	MAPCLKINST::iterator it;

	pFile->Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
		pFile->Write( &m_vSFXID[i], sizeof(DWORD));

	pFile->Write( &m_dwTotalFrame, sizeof(DWORD));
	pFile->Write( &m_dwSANISRC, sizeof(DWORD));
	pFile->Write( &m_dwANISRC, sizeof(DWORD));
	pFile->Write( &m_dwSFXSRC, sizeof(DWORD));
	pFile->Write( &m_dwCurCLK, sizeof(DWORD));

	nCount = INT(m_SFX.m_vKey.size());
	pFile->Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		pFile->Write( &m_SFX.m_vKey[i]->m_dwColor, sizeof(DWORD));
		pFile->Write( &m_SFX.m_vKey[i]->m_dwTick, sizeof(DWORD));
	}

	pFile->Write( &m_SFX.m_bSFXType, sizeof(BYTE));
	pFile->Write( &m_SFX.m_bSRCType, sizeof(BYTE));

	pFile->Write( &m_SFX.m_fFollow, sizeof(FLOAT));
	pFile->Write( &m_SFX.m_fSizeX, sizeof(FLOAT));
	pFile->Write( &m_SFX.m_fSizeY, sizeof(FLOAT));

	pFile->Write( &m_SFX.m_dwDESTBlend, sizeof(DWORD));
	pFile->Write( &m_SFX.m_dwSRCBlend, sizeof(DWORD));
	pFile->Write( &m_SFX.m_dwBlendOP, sizeof(DWORD));

	pFile->Write( &m_SFX.m_dwPLifeTime, sizeof(DWORD));
	pFile->Write( &m_SFX.m_dwLifeTime, sizeof(DWORD));

	nCount = INT(m_SFX.m_mapCLK.size());
	pFile->Write( &nCount, sizeof(int));

	for( it = m_SFX.m_mapCLK.begin(); it != m_SFX.m_mapCLK.end(); it++)
	{
		pFile->Write( &(*it).first, sizeof(DWORD));
		pFile->Write( &(*it).second->m_dwCurMESH, sizeof(DWORD));
		pFile->Write( &(*it).second->m_dwCurCL, sizeof(DWORD));
	}

	pFile->Write( &m_SFX.m_dwOBJFuncID, sizeof(DWORD));
	pFile->Write( &m_SFX.m_dwACTFuncID, sizeof(DWORD));
	pFile->Write( &m_SFX.m_dwANIFuncID, sizeof(DWORD));
	pFile->Write( &m_SFX.m_dwACT, sizeof(DWORD));
	pFile->Write( &m_SFX.m_dwANI, sizeof(DWORD));
	pFile->Write( &m_SFX.m_bSprayType, sizeof(BYTE));
	pFile->Write( &m_SFX.m_bZEnable, sizeof(BYTE));
	pFile->Write( &m_SFX.m_bZWrite, sizeof(BYTE));
	pFile->Write( &m_SFX.m_bStrict, sizeof(BYTE));
	pFile->Write( &m_SFX.m_bFollow, sizeof(BYTE));
	pFile->Write( &m_SFX.m_dwGTime, sizeof(DWORD));
	pFile->Write( &m_SFX.m_dwGCnt, sizeof(DWORD));

	pFile->Write( &m_SFX.m_vOutBound, sizeof(D3DXVECTOR3));
	pFile->Write( &m_SFX.m_vInBound, sizeof(D3DXVECTOR3));
	pFile->Write( &m_SFX.m_dwERRTime, sizeof(DWORD));

	pFile->Write( &m_SFX.m_vERRPos, sizeof(D3DXVECTOR3));
	pFile->Write( &m_SFX.m_vERRRot, sizeof(D3DXVECTOR3));
	pFile->Write( &m_SFX.m_vERRScale, sizeof(D3DXVECTOR3));
	pFile->Write( &m_SFX.m_dwLodLevel, sizeof(DWORD));
}

void CSFXItem::Regulate()
{
	CAnimationItem *pANI = GetAnimationItem(m_dwSANISRC);

	if( pANI && !pANI->m_bDeleted )
		m_SFX.m_pSprayANI = &pANI->m_Aniset;
	else
		m_dwSANISRC = 0;

	pANI = GetAnimationItem(m_dwANISRC);
	if( pANI && !pANI->m_bDeleted )
		m_SFX.m_pANISRC = &pANI->m_Aniset;
	else
		m_dwANISRC = 0;

	switch(m_SFX.m_bSRCType)
	{
	case SFXSRC_TEX	:
		{
			CTextureItem *pTEX = GetTextureItem(m_dwSFXSRC);

			if( pTEX && !pTEX->m_bDeleted )
				m_SFX.m_SRC.m_pTEX = pTEX->GetTEXTURESET();
			else
			{
				m_SFX.m_SRC.m_pTEX = NULL;
				m_dwSFXSRC = 0;
			}

			m_dwCurCLK = 0;
		}

		break;

	case SFXSRC_OBJ	:
		{
			COBJItem *pOBJ = GetOBJItem(m_dwSFXSRC);

			if( pOBJ && !pOBJ->m_bDeleted )
			{
				MAPACTION::iterator itACT = pOBJ->m_OBJ.m_mapACTION.find(m_SFX.m_dwACT);
				MAPCLKIND::iterator itCLK = pOBJ->m_OBJ.m_mapCLKIND.find(m_dwCurCLK);
				MAPCLKINST::iterator itCI = m_SFX.m_mapCLK.begin();

				if( itACT == pOBJ->m_OBJ.m_mapACTION.end() )
				{
					m_SFX.m_dwACT = 0;
					m_SFX.m_dwANI = 0;
				}
				else
				{
					LPACTION pACT = (*itACT).second;
					MAPANI::iterator itANI = pACT->m_mapANI.find(m_SFX.m_dwANI);

					if( itANI == pACT->m_mapANI.end() )
						m_SFX.m_dwANI = 0;
				}

				if( itCLK == pOBJ->m_OBJ.m_mapCLKIND.end() )
					m_dwCurCLK = 0;

				while(itCI != m_SFX.m_mapCLK.end())
				{
					LPCLKINST pCI = (*itCI).second;
					itCLK = pOBJ->m_OBJ.m_mapCLKIND.find((*itCI).first);

					if( itCLK == pOBJ->m_OBJ.m_mapCLKIND.end() )
					{
						MAPCLKINST::iterator itNEXT = itCI;
						itNEXT++;

						m_SFX.m_mapCLK.erase(itCI);
						delete pCI;

						itCI = itNEXT;
					}
					else
					{
						LPCLKIND pCLK = (*itCLK).second;
						MAPCLOTH::iterator itCL = pCLK->m_mapCLOTH.find(pCI->m_dwCurCL);

						if( itCL == pCLK->m_mapCLOTH.end() )
						{
							pCI->m_dwHideID = 0xFFFFFFFF;
							pCI->m_dwCurMESH = 0;
							pCI->m_dwCurCL = 0;
							pCI->m_pMESH = NULL;
						}
						else
						{
							LPCLOTH pCL = (*itCL).second;
							MAPMESH::iterator itMESH = pCL->m_mapMESH.find(pCI->m_dwCurMESH);

							if( itMESH == pCL->m_mapMESH.end() )
							{
								pCI->m_dwHideID = 0xFFFFFFFF;
								pCI->m_dwCurMESH = 0;
								pCI->m_pMESH = NULL;
							}
						}

						itCI++;
					}
				}

				m_SFX.m_SRC.m_pOBJ = &pOBJ->m_OBJ;
			}
			else
			{
				m_SFX.m_SRC.m_pOBJ = NULL;
				m_dwSFXSRC = 0;
				m_dwCurCLK = 0;
			}
		}

		break;

	case SFXSRC_SFX	:
		{
			CSFXItem *pSFX = GetSFXItem(m_dwSFXSRC);

			if( pSFX && !pSFX->m_bDeleted )
				m_SFX.m_SRC.m_pSFX = &pSFX->m_SFX;
			else
			{
				m_SFX.m_SRC.m_pSFX = NULL;
				m_dwSFXSRC = 0;
			}

			m_dwCurCLK = 0;
		}

		break;
	}

	VECTORDWORD::iterator it = m_vSFXID.begin();
	m_SFX.m_vSFX.clear();

	while(it != m_vSFXID.end())
	{
		CSFXItem *pSFX = GetSFXItem(*it);

		if( pSFX && !pSFX->m_bDeleted )
		{
			m_SFX.m_vSFX.push_back(&pSFX->m_SFX);
			it++;
		}
		else
			m_vSFXID.erase(it);
	}

	if( m_dwSFXSRC == 0 )
	{
		m_TSFX.Release();
		m_SFX.m_bSRCType = SFXSRC_TEX;
	}
}

BOOL CSFXItem::FindSFX( DWORD dwID)
{
	if( dwID == m_dwID )
		return TRUE;

	if( m_SFX.m_bSRCType == SFXSRC_SFX && m_dwSFXSRC == dwID )
		return TRUE;

	int nCount = INT(m_vSFXID.size());
	for( int i=0; i<nCount; i++)
	{
		CSFXItem *pSFX = GetSFXItem(m_vSFXID[i]);

		if( pSFX && pSFX->FindSFX(dwID) )
			return TRUE;
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////
// CMAPItem Function...
//////////////////////////////////////////////////////////////////

CMAPItem::CMAPItem( BYTE bType)
:CItemBase(bType)
{
	m_vDropTarget.push_back(ITEM_PROJECT);
	m_vDropTarget.push_back(ITEM_FOLDER);

	m_nDropEffect = DROPEFFECT_MOVE;
	m_bPreviewID = TRUE;
	m_bNeedID = TRUE;
	m_bMenuID = 2;

	m_mapLIGHT.clear();
	m_mapOBJ.clear();
	m_mapSFX.clear();
	m_pMAP = NULL;

	m_mapMAPItem.insert( MAPITEM::value_type( m_dwID, this));
}

CMAPItem::~CMAPItem()
{
	MAPMAPLIGHT::iterator itLIGHT;

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		delete (*itLIGHT).second;

	m_mapLIGHT.clear();
	m_mapOBJ.clear();
	m_mapSFX.clear();

	m_pMAP = NULL;
}

void CMAPItem::Load( CFile* pFile)
{
	MAPITEM::iterator finder = m_mapMAPItem.find(m_dwID);
	if( finder != m_mapMAPItem.end() )
		m_mapMAPItem.erase(finder);
	CItemBase::Load(pFile);
	m_mapMAPItem.insert( MAPITEM::value_type( m_dwID, this));
}

void CMAPItem::Save( CFile* pFile)
{
	CItemBase::Save(pFile);
}

void CMAPItem::SyncOBJATTR( COBJItem *pItem, LPMAPOBJ pOBJ)
{
	MAPATTRIBUTE::iterator itOBJ = pOBJ->m_mapATTR.begin();
	MAPATTRITEM::iterator itITEM;

	while(itOBJ != pOBJ->m_mapATTR.end())
	{
		MAPATTRIBUTE::iterator itNEXT = itOBJ;
		itNEXT++;

		if( pItem->m_mapATTR.find((*itOBJ).first) == pItem->m_mapATTR.end() )
		{
			delete (*itOBJ).second;
			pOBJ->m_mapATTR.erase(itOBJ);
		}

		itOBJ = itNEXT;
	}

	for( itITEM = pItem->m_mapATTR.begin(); itITEM != pItem->m_mapATTR.end(); itITEM++)
	{
		LPATTRIBUTE pATTR = NULL;

		itOBJ = pOBJ->m_mapATTR.find((*itITEM).first);
		if( itOBJ == pOBJ->m_mapATTR.end() )
		{
			pATTR = new ATTRIBUTE();

			pOBJ->m_mapATTR.insert( MAPATTRIBUTE::value_type(
				(*itITEM).first,
				pATTR));
			pATTR->m_bValueType = (*itITEM).second->m_bType;
		}
		else if( (*itOBJ).second->m_bValueType != (*itITEM).second->m_bType )
		{
			pATTR = (*itOBJ).second;

			if(pATTR->m_pValue)
			{
				delete[] pATTR->m_pValue;
				pATTR->m_pValue = NULL;
			}
			pATTR->m_bValueType = (*itITEM).second->m_bType;
		}

		if(pATTR)
			switch(pATTR->m_bValueType)
			{
			case ATTRTYPE_STRING	:
				{
					pATTR->m_nSize = (*itITEM).second->m_strValue.GetLength();
					pATTR->m_pValue = new BYTE[pATTR->m_nSize];

					memcpy( pATTR->m_pValue, LPCSTR((*itITEM).second->m_strValue), pATTR->m_nSize);
				}

				break;

			case ATTRTYPE_FLOAT		:
				{
					pATTR->m_nSize = sizeof(FLOAT);
					pATTR->m_pValue = new BYTE[pATTR->m_nSize];

					*((FLOAT *) pATTR->m_pValue) = (FLOAT) atof((*itITEM).second->m_strValue);
				}

				break;

			case ATTRTYPE_INT		:
				{
					pATTR->m_nSize = sizeof(int);
					pATTR->m_pValue = new BYTE[pATTR->m_nSize];

					*((int *) pATTR->m_pValue) = atoi((*itITEM).second->m_strValue);
				}

				break;
			}
	}
}


//////////////////////////////////////////////////////////////////
// CEXTMAPItem Function...
//////////////////////////////////////////////////////////////////

CEXTMAPItem::CEXTMAPItem()
:CMAPItem(ITEM_EXTMAP)
{
	m_bMAPType = MT_EXT;
	m_mapCOLOR.clear();

	m_vDETAILS.clear();
	m_vDETAIL.clear();
	m_vGEO.clear();

	m_strUNIT.Empty();
	m_strOBJ.Empty();

	m_fMaxHeight = 0.0f;
	m_fMinHeight = 0.0f;
	m_dwNewOBJID = 1;
	m_dwNewSFXID = 1;
	m_dwNewSNDID = 1;
	m_bMenuID = 5;

	m_vFROM.SetRectEmpty();
	m_vTO.SetPoint( 0, 0);

	m_pMAP = &m_HugeMAP;
}

CEXTMAPItem::~CEXTMAPItem()
{
	MAPITEM::iterator finder = m_mapMAPItem.find(m_dwID);
	OnDelete();

	if( finder != m_mapMAPItem.end() )
		m_mapMAPItem.erase(finder);

	while(!m_vGEO.empty())
	{
		delete m_vGEO.back();
		m_vGEO.pop_back();
	}
	m_mapCOLOR.clear();

	if(!m_strUNIT.IsEmpty())
		DeleteFile(m_strUNIT);
	if(!m_strOBJ.IsEmpty())
		DeleteFile(m_strOBJ);

	m_vDETAILS.clear();
	m_vDETAIL.clear();

	m_fMaxHeight = 0.0f;
	m_fMinHeight = 0.0f;

	m_vFROM.SetRectEmpty();
	m_vTO.SetPoint( 0, 0);
}

void CEXTMAPItem::Load( CFile* pFile)
{
	MAPITEM::iterator finder = m_mapMAPItem.find(m_dwID);
	if( finder != m_mapMAPItem.end() )
		m_mapMAPItem.erase(finder);
	CItemBase::Load(pFile);
	m_mapMAPItem.insert( MAPITEM::value_type( m_dwID, this));

	pFile->Read( &m_HugeMAP.m_nUnitCountX, sizeof(int));
	pFile->Read( &m_HugeMAP.m_nUnitCountZ, sizeof(int));
	pFile->Read( &m_HugeMAP.m_nUnitLength, sizeof(int));
	pFile->Read( &m_HugeMAP.m_nTileLength, sizeof(int));
	pFile->Read( &m_HugeMAP.m_nCellCount, sizeof(int));
	pFile->Read( &m_HugeMAP.m_nTextureSize, sizeof(int));
	pFile->Read( &m_HugeMAP.m_nShadowPixel, sizeof(int));
	pFile->Read( &m_HugeMAP.m_vLIGHT, sizeof(D3DXVECTOR3));

	pFile->Read( &m_HugeMAP.m_bUseSHADER, sizeof(BYTE));
	pFile->Read( &m_HugeMAP.m_fIntensity, sizeof(FLOAT));
	pFile->Read( &m_HugeMAP.m_fSpecRange, sizeof(FLOAT));
	pFile->Read( &m_HugeMAP.m_fSpecMax, sizeof(FLOAT));

	m_HugeMAP.InitVB(&CMainFrame::m_3DDevice);
	m_vDETAILS.clear();
	m_vDETAIL.clear();
	m_vGEO.clear();

	LPBYTE pUNIT = new BYTE[DEF_UNITSIZE];
	LPBYTE pOBJ = new BYTE[DEF_OBJSIZE];

	char szTempPath[_MAX_PATH];
	char szTempFile[_MAX_PATH];
	int nCount = 0;

	pFile->Read( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		DWORD dwDetailID;
		FLOAT fDetailS;

		pFile->Read( &dwDetailID, sizeof(DWORD));
		pFile->Read( &fDetailS, sizeof(FLOAT));

		if(m_bMerge)
			dwDetailID += m_dwTextureID;

		m_vDETAIL.push_back(dwDetailID);
		m_vDETAILS.push_back(fDetailS);
	}
	
	GetStudioTempPath(szTempPath);
	GetTempFileName(
		szTempPath,
		_T("_EM"),
		0, szTempFile);

	CFile unit( szTempFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	m_strUNIT = unit.GetFilePath();

	GetTempFileName(
		szTempPath,
		_T("_EO"),
		0, szTempFile);

	CFile obj( szTempFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	m_strOBJ = obj.GetFilePath();

	nCount = m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ;
	for( i=0; i<nCount; i++)
	{
		LPBYTE pOBJDATA = pOBJ;
		LPBYTE pDATA = pUNIT;
		DWORD dwLength;

		memset( pOBJDATA, 0x00, DEF_OBJSIZE);
		memset( pDATA, 0x00, DEF_UNITSIZE);

		pFile->Read( &dwLength, sizeof(DWORD));
		*((LPDWORD) pDATA) = dwLength;
		pDATA += sizeof(DWORD);

		if( dwLength > 0 )
		{
			pFile->Read( pDATA, dwLength);
			pDATA += dwLength;
		}

		pFile->Read( &dwLength, sizeof(DWORD));
		*((LPDWORD) pDATA) = dwLength;
		pDATA += sizeof(DWORD);

		if( dwLength > 0 )
		{
			pFile->Read( pDATA, dwLength);
			pDATA += dwLength;
		}

		pFile->Read( &dwLength, sizeof(DWORD));
		*((LPDWORD) pDATA) = dwLength;
		pDATA += sizeof(DWORD);

		if( dwLength > 0 )
		{
			pFile->Read( pDATA, dwLength);
			pDATA += dwLength;
		}

		pFile->Read( &dwLength, sizeof(DWORD));
		*((LPDWORD) pDATA) = dwLength;
		pDATA += sizeof(DWORD);

		if( dwLength > 0 )
		{
			pFile->Read( pDATA, dwLength);
			pDATA += dwLength;
		}

		pFile->Read( &dwLength, sizeof(DWORD));
		*((LPDWORD) pDATA) = dwLength;
		pDATA += sizeof(DWORD);

		if( dwLength > 0 )
		{
			pFile->Read( pDATA, dwLength);
			pDATA += dwLength;
		}

		pFile->Read( &dwLength, sizeof(DWORD));
		*((LPDWORD) pDATA) = dwLength;
		pDATA += sizeof(DWORD);

		if( dwLength > 0 )
		{
			pFile->Read( pDATA, dwLength);
			pDATA += dwLength;
		}

		DWORD dwCount = 0;
		pFile->Read( &dwCount, sizeof(DWORD));
		*((LPDWORD) pDATA) = dwCount;
		pDATA += sizeof(DWORD);

		for( DWORD j=0; j<dwCount; j++)
		{
			pFile->Read( &dwLength, sizeof(DWORD));
			*((LPDWORD) pDATA) = dwLength;
			pDATA += sizeof(DWORD);

			if( dwLength > 0 )
			{
				pFile->Read( pDATA, dwLength);
				pDATA += dwLength;
			}
		}

		pFile->Read( &dwLength, sizeof(DWORD));
		*((LPDWORD) pOBJDATA) = dwLength;
		pOBJDATA += sizeof(DWORD);

		for( DWORD j=0; j<dwLength; j++)
		{
			int nATTR;
			int nCLK;

			pFile->Read( pOBJDATA, 2 * sizeof(DWORD));
			LPDWORD pID = (LPDWORD) pOBJDATA;
			pOBJDATA += sizeof(DWORD);
			LPDWORD pItemID = (LPDWORD) pOBJDATA;
			pOBJDATA += sizeof(DWORD);

			if( m_bMerge && (*pItemID) )
				*pItemID += m_dwOBJID;

			pFile->Read( pOBJDATA, 3 * sizeof(D3DXVECTOR3));
			pOBJDATA += 3 * sizeof(D3DXVECTOR3);

			pFile->Read( pOBJDATA, 5 * sizeof(DWORD));
			pOBJDATA += 5 * sizeof(DWORD);

			pFile->Read( &nCLK, sizeof(int));
			*((int *) pOBJDATA) = nCLK;
			pOBJDATA += sizeof(int);

			pFile->Read( pOBJDATA, nCLK * 3 * sizeof(DWORD));
			pOBJDATA += nCLK * 3 * sizeof(DWORD);

			pFile->Read( &nATTR, sizeof(int));
			*((int *) pOBJDATA) = nATTR;
			pOBJDATA += sizeof(int);

			for( int k=0; k<nATTR; k++)
			{
				int nSIZE = 0;

				pFile->Read( pOBJDATA, sizeof(DWORD));
				pOBJDATA += sizeof(DWORD);

				pFile->Read( pOBJDATA, sizeof(BYTE));
				pOBJDATA += sizeof(BYTE);

				pFile->Read( &nSIZE, sizeof(int));
				*((int *) pOBJDATA) = nSIZE;
				pOBJDATA += sizeof(int);

				pFile->Read( pOBJDATA, nSIZE);
				pOBJDATA += nSIZE;
			}
		}

		pFile->Read( &dwLength, sizeof(DWORD));
		*((LPDWORD) pOBJDATA) = dwLength;
		pOBJDATA += sizeof(DWORD);

		for( j=0; j<dwLength; j++)
		{
			pFile->Read( pOBJDATA, 2 * sizeof(DWORD));
			LPDWORD pID = (LPDWORD) pOBJDATA;
			pOBJDATA += sizeof(DWORD);
			LPDWORD pItemID = (LPDWORD) pOBJDATA;
			pOBJDATA += sizeof(DWORD);

			if( m_bMerge && (*pItemID) )
				*pItemID += m_dwSFXID;

			pFile->Read( pOBJDATA, 2 * sizeof(D3DXVECTOR3));
			pOBJDATA += 2 * sizeof(D3DXVECTOR3);

			pFile->Read( pOBJDATA, sizeof(BYTE));
			pOBJDATA += sizeof(BYTE);
		}

		pFile->Read( &dwLength, sizeof(DWORD));
		*((LPDWORD) pOBJDATA) = dwLength;
		pOBJDATA += sizeof(DWORD);

		for( j=0; j<dwLength; j++)
		{
			pFile->Read( pOBJDATA, 2 * sizeof(DWORD));
			LPDWORD pID = (LPDWORD) pOBJDATA;
			pOBJDATA += sizeof(DWORD);
			LPDWORD pItemID = (LPDWORD) pOBJDATA;
			pOBJDATA += sizeof(DWORD);

			if( m_bMerge && (*pItemID) )
				*pItemID += m_dwMediaID;

			pFile->Read( pOBJDATA, sizeof(D3DXVECTOR3));
			pOBJDATA += sizeof(D3DXVECTOR3);
		}

		unit.Write( pUNIT, DEF_UNITSIZE);
		obj.Write( pOBJ, DEF_OBJSIZE);
	}

	delete[] pUNIT;
	pUNIT = NULL;

	delete[] pOBJ;
	pOBJ = NULL;

	if( nCount > 0 )
		pFile->Read( m_HugeMAP.m_pENABLE, nCount * sizeof(BYTE));

	pFile->Read( &m_fMinHeight, sizeof(FLOAT));
	pFile->Read( &m_fMaxHeight, sizeof(FLOAT));

	pFile->Read( &m_dwNewOBJID, sizeof(DWORD));
	pFile->Read( &m_dwNewSFXID, sizeof(DWORD));
	pFile->Read( &m_dwNewSNDID, sizeof(DWORD));

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPGEOTEX pGEOTEX = new GEOTEX();

		pFile->Read( &pGEOTEX->m_dwTEXID, sizeof(DWORD));
		pFile->Read( &pGEOTEX->m_fHeight, sizeof(FLOAT));

		if( m_bMerge && pGEOTEX->m_dwTEXID )
			pGEOTEX->m_dwTEXID += m_dwTextureID;

		m_vGEO.push_back(pGEOTEX);
	}
}

void CEXTMAPItem::Save( CFile* pFile)
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) GetMainDocument();
	if(pDoc)
		pDoc->UpdateData();
	CItemBase::Save(pFile);

	CFile unit( m_strUNIT, CFile::modeRead|CFile::typeBinary);
	CFile obj( m_strOBJ, CFile::modeRead|CFile::typeBinary);
	LPBYTE pDATA = new BYTE[max( DEF_UNITSIZE, DEF_OBJSIZE)];

	pFile->Write( &m_HugeMAP.m_nUnitCountX, sizeof(int));
	pFile->Write( &m_HugeMAP.m_nUnitCountZ, sizeof(int));
	pFile->Write( &m_HugeMAP.m_nUnitLength, sizeof(int));
	pFile->Write( &m_HugeMAP.m_nTileLength, sizeof(int));
	pFile->Write( &m_HugeMAP.m_nCellCount, sizeof(int));
	pFile->Write( &m_HugeMAP.m_nTextureSize, sizeof(int));
	pFile->Write( &m_HugeMAP.m_nShadowPixel, sizeof(int));
	pFile->Write( &m_HugeMAP.m_vLIGHT, sizeof(D3DXVECTOR3));

	pFile->Write( &m_HugeMAP.m_bUseSHADER, sizeof(BYTE));
	pFile->Write( &m_HugeMAP.m_fIntensity, sizeof(FLOAT));
	pFile->Write( &m_HugeMAP.m_fSpecRange, sizeof(FLOAT));
	pFile->Write( &m_HugeMAP.m_fSpecMax, sizeof(FLOAT));

	int nCount = INT(m_vDETAIL.size());
	pFile->Write( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		pFile->Write( &m_vDETAIL[i], sizeof(DWORD));
		pFile->Write( &m_vDETAILS[i], sizeof(FLOAT));
	}

	nCount = m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ;
	for( i=0; i<nCount; i++)
	{
		unit.Seek( i * DEF_UNITSIZE, CFile::begin);
		obj.Seek( i * DEF_OBJSIZE, CFile::begin);
		DWORD dwLength = 0;

		unit.Read( &dwLength, sizeof(DWORD));
		pFile->Write( &dwLength, sizeof(DWORD));

		if( dwLength > 0 )
		{
			unit.Read( pDATA, dwLength);
			pFile->Write( pDATA, dwLength);
		}

		unit.Read( &dwLength, sizeof(DWORD));
		pFile->Write( &dwLength, sizeof(DWORD));

		if( dwLength > 0 )
		{
			unit.Read( pDATA, dwLength);
			pFile->Write( pDATA, dwLength);
		}

		unit.Read( &dwLength, sizeof(DWORD));
		pFile->Write( &dwLength, sizeof(DWORD));

		if( dwLength > 0 )
		{
			unit.Read( pDATA, dwLength);
			pFile->Write( pDATA, dwLength);
		}

		unit.Read( &dwLength, sizeof(DWORD));
		pFile->Write( &dwLength, sizeof(DWORD));

		if( dwLength > 0 )
		{
			unit.Read( pDATA, dwLength);
			pFile->Write( pDATA, dwLength);
		}

		unit.Read( &dwLength, sizeof(DWORD));
		pFile->Write( &dwLength, sizeof(DWORD));

		if( dwLength > 0 )
		{
			unit.Read( pDATA, dwLength);
			pFile->Write( pDATA, dwLength);
		}

		unit.Read( &dwLength, sizeof(DWORD));
		pFile->Write( &dwLength, sizeof(DWORD));

		if( dwLength > 0 )
		{
			unit.Read( pDATA, dwLength);
			pFile->Write( pDATA, dwLength);
		}

		DWORD dwCount = 0;
		unit.Read( &dwCount, sizeof(DWORD));
		pFile->Write( &dwCount, sizeof(DWORD));

		for( DWORD j=0; j<dwCount; j++)
		{
			unit.Read( &dwLength, sizeof(DWORD));
			pFile->Write( &dwLength, sizeof(DWORD));

			if( dwLength > 0 )
			{
				unit.Read( pDATA, dwLength);
				pFile->Write( pDATA, dwLength);
			}
		}

		obj.Read( &dwLength, sizeof(DWORD));
		pFile->Write( &dwLength, sizeof(DWORD));

		for( j=0; j<dwLength; j++)
		{
			int nATTR;
			int nCLK;

			obj.Read( pDATA, 2 * sizeof(DWORD));
			pFile->Write( pDATA, 2 * sizeof(DWORD));

			obj.Read( pDATA, 3 * sizeof(D3DXVECTOR3));
			pFile->Write( pDATA, 3 * sizeof(D3DXVECTOR3));

			obj.Read( pDATA, 5 * sizeof(DWORD));
			pFile->Write( pDATA, 5 * sizeof(DWORD));

			obj.Read( &nCLK, sizeof(int));
			pFile->Write( &nCLK, sizeof(int));

			obj.Read( pDATA, nCLK * 3 * sizeof(DWORD));
			pFile->Write( pDATA, nCLK * 3 * sizeof(DWORD));

			obj.Read( &nATTR, sizeof(int));
			pFile->Write( &nATTR, sizeof(int));

			for( int k=0; k<nATTR; k++)
			{
				DWORD dwATTR = 0;
				ATTRIBUTE vATTR;

				obj.Read( &dwATTR, sizeof(DWORD));
				pFile->Write( &dwATTR, sizeof(DWORD));

				obj.Read( &vATTR.m_bValueType, sizeof(BYTE));
				pFile->Write( &vATTR.m_bValueType, sizeof(BYTE));

				obj.Read( &vATTR.m_nSize, sizeof(int));
				pFile->Write( &vATTR.m_nSize, sizeof(int));
				vATTR.m_pValue = new BYTE[vATTR.m_nSize];

				obj.Read( vATTR.m_pValue, vATTR.m_nSize);
				pFile->Write( vATTR.m_pValue, vATTR.m_nSize);
			}
		}

		obj.Read( &dwLength, sizeof(DWORD));
		pFile->Write( &dwLength, sizeof(DWORD));

		for( j=0; j<dwLength; j++)
		{
			obj.Read( pDATA, 2 * sizeof(DWORD));
			pFile->Write( pDATA, 2 * sizeof(DWORD));

			obj.Read( pDATA, 2 * sizeof(D3DXVECTOR3));
			pFile->Write( pDATA, 2 * sizeof(D3DXVECTOR3));

			obj.Read( pDATA, sizeof(BYTE));
			pFile->Write( pDATA, sizeof(BYTE));
		}

		obj.Read( &dwLength, sizeof(DWORD));
		pFile->Write( &dwLength, sizeof(DWORD));

		for( j=0; j<dwLength; j++)
		{
			obj.Read( pDATA, 2 * sizeof(DWORD));
			pFile->Write( pDATA, 2 * sizeof(DWORD));

			obj.Read( pDATA, sizeof(D3DXVECTOR3));
			pFile->Write( pDATA, sizeof(D3DXVECTOR3));
		}
	}

	delete[] pDATA;
	pDATA = NULL;

	if( nCount > 0 )
		pFile->Write( m_HugeMAP.m_pENABLE, nCount * sizeof(BYTE));

	pFile->Write( &m_fMinHeight, sizeof(FLOAT));
	pFile->Write( &m_fMaxHeight, sizeof(FLOAT));

	pFile->Write( &m_dwNewOBJID, sizeof(DWORD));
	pFile->Write( &m_dwNewSFXID, sizeof(DWORD));
	pFile->Write( &m_dwNewSNDID, sizeof(DWORD));

	nCount = INT(m_vGEO.size());
	pFile->Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		pFile->Write( &m_vGEO[i]->m_dwTEXID, sizeof(DWORD));
		pFile->Write( &m_vGEO[i]->m_fHeight, sizeof(FLOAT));
	}
}

void CEXTMAPItem::DeleteForRestore()
{
	m_HugeMAP.DeleteForRestore();
}

void CEXTMAPItem::Restore()
{
	m_HugeMAP.Restore( &CMainFrame::m_3DDevice, NULL);
}

void CEXTMAPItem::Regulate()
{
	m_HugeMAP.m_vDETAILDATA.clear();
	m_HugeMAP.m_vDETAILSDATA.clear();

	for( int i=0; i<INT(m_vDETAIL.size()); i++)
	{
		CTextureItem *pTEX = GetTextureItem(m_vDETAIL[i]);

		m_HugeMAP.m_vDETAILDATA.push_back(pTEX ? pTEX->GetTEXTURESET() : NULL);
		m_HugeMAP.m_vDETAILSDATA.push_back(m_vDETAILS[i]);
	}
}

void CEXTMAPItem::LoadOBJ( MAPMAPOBJ *pOBJ,
						   MAPMAPSFX *pSFX,
						   MAPMAPSND *pSND,
						   MAPDWORD *pOBJID,
						   MAPDWORD *pSFXID,
						   DWORD dwIndex)
{
	CFile file( m_strOBJ, CFile::modeRead|CFile::typeBinary);
	DWORD dwCount;

	file.Seek( DEF_OBJSIZE * dwIndex, CFile::begin);
	file.Read( &dwCount, sizeof(DWORD));

	for( DWORD i=0; i<dwCount; i++)
	{
		COBJItem *pITEM = NULL;
		DWORD dwItemID;
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &dwItemID, sizeof(DWORD));
		pITEM = CItemBase::GetOBJItem(dwItemID);

		if(pITEM)
		{
			LPMAPOBJ pMAPOBJ = new MAPOBJ();
			int nATTR;
			int nCLK;

			pOBJID->insert( MAPDWORD::value_type(
				dwID,
				dwItemID));
			pMAPOBJ->m_pOBJ = &pITEM->m_OBJ;

			file.Read( &pMAPOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
			file.Read( &pMAPOBJ->m_vPOS, sizeof(D3DXVECTOR3));
			file.Read( &pMAPOBJ->m_vROT, sizeof(D3DXVECTOR3));

			file.Read( &pMAPOBJ->m_dwOBJFuncID, sizeof(DWORD));
			file.Read( &pMAPOBJ->m_dwACTFuncID, sizeof(DWORD));
			file.Read( &pMAPOBJ->m_dwANIFuncID, sizeof(DWORD));

			file.Read( &pMAPOBJ->m_dwCurANI, sizeof(DWORD));
			file.Read( &pMAPOBJ->m_dwCurACT, sizeof(DWORD));

			file.Read( &nCLK, sizeof(int));
			for( int j=0; j<nCLK; j++)
			{
				LPCLKINST pCLK = new CLKINST();
				DWORD dwCLK = 0;

				file.Read( &dwCLK, sizeof(DWORD));
				file.Read( &pCLK->m_dwCurMESH, sizeof(DWORD));
				file.Read( &pCLK->m_dwCurCL, sizeof(DWORD));

				pMAPOBJ->m_mapCLK.insert( MAPCLKINST::value_type(
					dwCLK,
					pCLK));
			}

			file.Read( &nATTR, sizeof(int));
			for( j=0; j<nATTR; j++)
			{
				LPATTRIBUTE pATTR = new ATTRIBUTE();
				DWORD dwATTR;

				file.Read( &dwATTR, sizeof(DWORD));
				file.Read( &pATTR->m_bValueType, sizeof(BYTE));
				file.Read( &pATTR->m_nSize, sizeof(int));
				pATTR->m_pValue = new BYTE[pATTR->m_nSize];
				file.Read( pATTR->m_pValue, pATTR->m_nSize);

				pMAPOBJ->m_mapATTR.insert( MAPATTRIBUTE::value_type( dwATTR, pATTR));
			}

			pOBJ->insert( MAPMAPOBJ::value_type( dwID, pMAPOBJ));
		}
		else
		{
			int nATTR;
			int nCLK;

			file.Seek( 3 * sizeof(D3DXVECTOR3), CFile::current);
			file.Seek( 5 * sizeof(DWORD), CFile::current);

			file.Read( &nCLK, sizeof(int));
			file.Seek( nCLK * 3 * sizeof(DWORD), CFile::current);

			file.Read( &nATTR, sizeof(int));
			for( int j=0; j<nATTR; j++)
			{
				int nSize = 0;

				file.Seek( sizeof(DWORD), CFile::current);
				file.Seek( sizeof(BYTE), CFile::current);
				file.Read( &nSize, sizeof(int));
				file.Seek( nSize, CFile::current);
			}
		}
	}

	file.Read( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		CSFXItem *pITEM = NULL;
		DWORD dwItemID;
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &dwItemID, sizeof(DWORD));
		pITEM = CItemBase::GetSFXItem(dwItemID);

		if( pITEM && pSFX && pSFXID )
		{
			LPMAPSFX pMAPSFX = new MAPSFX();

			pSFXID->insert( MAPDWORD::value_type(
				dwID,
				dwItemID));
			pMAPSFX->m_pSFX = &pITEM->m_SFX;

			file.Read( &pMAPSFX->m_vPOS, sizeof(D3DXVECTOR3));
			file.Read( &pMAPSFX->m_vROT, sizeof(D3DXVECTOR3));
			file.Read( &pMAPSFX->m_bBias, sizeof(BYTE));

			pSFX->insert( MAPMAPSFX::value_type( dwID, pMAPSFX));
		}
		else
		{
			file.Seek( 2 * sizeof(D3DXVECTOR3), CFile::current);
			file.Seek( sizeof(BYTE), CFile::current);
		}
	}

	file.Read( &dwCount, sizeof(DWORD));
	for( i=0; i<dwCount; i++)
	{
		CMediaItem *pITEM = NULL;
		DWORD dwItemID;
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &dwItemID, sizeof(DWORD));
		pITEM = CItemBase::GetMediaItem(dwItemID);

		if( pITEM && pSND )
		{
			LPMAPSND pMAPSND = new MAPSND();

			pMAPSND->m_dwMediaID = dwItemID;
			file.Read( &pMAPSND->m_vPOS, sizeof(D3DXVECTOR3));

			pSND->insert( MAPMAPSND::value_type( dwID, pMAPSND));
		}
		else
			file.Seek( sizeof(D3DXVECTOR3), CFile::current);
	}
}

void CEXTMAPItem::UpdateOBJ( MAPMAPOBJ *pOBJ,
							 MAPMAPSFX *pSFX,
							 MAPMAPSND *pSND,
							 MAPDWORD *pOBJID,
							 MAPDWORD *pSFXID,
							 DWORD dwIndex)
{
	CFile file( m_strOBJ, CFile::modeReadWrite|CFile::typeBinary);
	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;
	DWORD dwCount = (DWORD) pOBJ->size();

	file.Seek( DEF_OBJSIZE * dwIndex, CFile::begin);
	file.Write( &dwCount, sizeof(DWORD));

	for( itOBJ = pOBJ->begin(); itOBJ != pOBJ->end(); itOBJ++)
	{
		LPMAPOBJ pMAPOBJ = (*itOBJ).second;
		DWORD dwID = (*itOBJ).first;

		MAPDWORD::iterator itID = pOBJID->find(dwID);
		MAPATTRIBUTE::iterator itATTR;
		MAPCLKINST::iterator itCLK;
		DWORD dwItemID = (*itID).second;

		file.Write( &dwID, sizeof(DWORD));
		file.Write( &dwItemID, sizeof(DWORD));

		file.Write( &pMAPOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
		file.Write( &pMAPOBJ->m_vPOS, sizeof(D3DXVECTOR3));
		file.Write( &pMAPOBJ->m_vROT, sizeof(D3DXVECTOR3));

		file.Write( &pMAPOBJ->m_dwOBJFuncID, sizeof(DWORD));
		file.Write( &pMAPOBJ->m_dwACTFuncID, sizeof(DWORD));
		file.Write( &pMAPOBJ->m_dwANIFuncID, sizeof(DWORD));

		file.Write( &pMAPOBJ->m_dwCurANI, sizeof(DWORD));
		file.Write( &pMAPOBJ->m_dwCurACT, sizeof(DWORD));

		int nCLK = (INT) pMAPOBJ->m_mapCLK.size();
		file.Write( &nCLK, sizeof(int));

		for( itCLK = pMAPOBJ->m_mapCLK.begin(); itCLK != pMAPOBJ->m_mapCLK.end(); itCLK++)
		{
			LPCLKINST pCLK = (*itCLK).second;
			DWORD dwCLK = (*itCLK).first;

			file.Write( &dwCLK, sizeof(DWORD));
			file.Write( &pCLK->m_dwCurMESH, sizeof(DWORD));
			file.Write( &pCLK->m_dwCurCL, sizeof(DWORD));
		}

		int nATTR = (INT) pMAPOBJ->m_mapATTR.size();
		file.Write( &nATTR, sizeof(int));

		for( itATTR = pMAPOBJ->m_mapATTR.begin(); itATTR != pMAPOBJ->m_mapATTR.end(); itATTR++)
		{
			DWORD dwATTR = (*itATTR).first;

			file.Write( &dwATTR, sizeof(DWORD));
			file.Write( &(*itATTR).second->m_bValueType, sizeof(BYTE));
			file.Write( &(*itATTR).second->m_nSize, sizeof(int));
			file.Write( (*itATTR).second->m_pValue, (*itATTR).second->m_nSize);
		}
	}

	dwCount = (DWORD) pSFX->size();
	file.Write( &dwCount, sizeof(DWORD));

	for( itSFX = pSFX->begin(); itSFX != pSFX->end(); itSFX++)
	{
		LPMAPSFX pMAPSFX = (*itSFX).second;
		DWORD dwID = (*itSFX).first;

		MAPDWORD::iterator itID = pSFXID->find(dwID);
		DWORD dwItemID = (*itID).second;

		file.Write( &dwID, sizeof(DWORD));
		file.Write( &dwItemID, sizeof(DWORD));

		file.Write( &pMAPSFX->m_vPOS, sizeof(D3DXVECTOR3));
		file.Write( &pMAPSFX->m_vROT, sizeof(D3DXVECTOR3));
		file.Write( &pMAPSFX->m_bBias, sizeof(BYTE));
	}

	dwCount = (DWORD) pSND->size();
	file.Write( &dwCount, sizeof(DWORD));

	for( itSND = pSND->begin(); itSND != pSND->end(); itSND++)
	{
		LPMAPSND pMAPSND = (*itSND).second;
		DWORD dwID = (*itSND).first;

		file.Write( &dwID, sizeof(DWORD));
		file.Write( &pMAPSND->m_dwMediaID, sizeof(DWORD));
		file.Write( &pMAPSND->m_vPOS, sizeof(D3DXVECTOR3));
	}
}

void CEXTMAPItem::LoadUNIT( LPDIRECT3DSURFACE9 *pSHADE,
							LPDIRECT3DSURFACE9 *pSKIN,
							LPDIRECT3DSURFACE9 *pTILE,
							LPVECTORSURFACE pSHADOW,
							LPBYTE pREGION,
							LPBYTE pDETAIL,
							FLOAT *pHEIGHT,
							DWORD dwIndex)
{
	DWORD dwCount;
	CFile file;

	while(!file.Open( m_strUNIT, CFile::modeRead|CFile::typeBinary))
		Sleep(200);

	file.Seek( DEF_UNITSIZE * dwIndex, CFile::begin);
	file.Read( &dwCount, sizeof(DWORD));

	if(pHEIGHT)
		file.Read( pHEIGHT, dwCount);
	else
		file.Seek( dwCount, CFile::current);

	file.Read( &dwCount, sizeof(DWORD));
	if( pSHADE && dwCount )
	{
		LPBYTE pDATA = new BYTE[dwCount];
		D3DXIMAGE_INFO vIMG;

		file.Read( pDATA, dwCount);
		D3DXGetImageInfoFromFileInMemory(
			pDATA,
			dwCount,
			&vIMG);

		CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
			vIMG.Width,
			vIMG.Height,
			D3DFMT_A8R8G8B8,
			D3DPOOL_SCRATCH,
			pSHADE,
			NULL);

		D3DXLoadSurfaceFromFileInMemory(
			*pSHADE,
			NULL,
			NULL,
			pDATA,
			dwCount,
			NULL,
			D3DX_FILTER_NONE,
			NULL,
			NULL);

		delete[] pDATA;
	}
	else
		file.Seek( dwCount, CFile::current);

	file.Read( &dwCount, sizeof(DWORD));
	if( pSKIN && dwCount )
	{
		LPBYTE pDATA = new BYTE[dwCount];
		D3DXIMAGE_INFO vIMG;

		file.Read( pDATA, dwCount);
		D3DXGetImageInfoFromFileInMemory(
			pDATA,
			dwCount,
			&vIMG);

		CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
			vIMG.Width,
			vIMG.Height,
			D3DFMT_A8R8G8B8,
			D3DPOOL_SCRATCH,
			pSKIN,
			NULL);

		D3DXLoadSurfaceFromFileInMemory(
			*pSKIN,
			NULL,
			NULL,
			pDATA,
			dwCount,
			NULL,
			D3DX_FILTER_NONE,
			NULL,
			NULL);

		delete[] pDATA;
	}
	else
		file.Seek( dwCount, CFile::current);

	file.Read( &dwCount, sizeof(DWORD));
	if(pREGION)
		file.Read( pREGION, dwCount);
	else
		file.Seek( dwCount, CFile::current);

	file.Read( &dwCount, sizeof(DWORD));
	if(pDETAIL)
		file.Read( pDETAIL, dwCount);
	else
		file.Seek( dwCount, CFile::current);

	file.Read( &dwCount, sizeof(DWORD));
	if( pTILE && dwCount )
	{
		LPBYTE pDATA = new BYTE[dwCount];
		D3DXIMAGE_INFO vIMG;

		file.Read( pDATA, dwCount);
		D3DXGetImageInfoFromFileInMemory(
			pDATA,
			dwCount,
			&vIMG);

		CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
			vIMG.Width,
			vIMG.Height,
			D3DFMT_A8R8G8B8,
			D3DPOOL_SCRATCH,
			pTILE,
			NULL);

		D3DXLoadSurfaceFromFileInMemory(
			*pTILE,
			NULL,
			NULL,
			pDATA,
			dwCount,
			NULL,
			D3DX_FILTER_NONE,
			NULL,
			NULL);

		delete[] pDATA;
	}
	else
		file.Seek( dwCount, CFile::current);

	file.Read( &dwCount, sizeof(DWORD));
	for( DWORD i=0; i<dwCount; i++)
	{
		DWORD dwLength = 0;

		file.Read( &dwLength, sizeof(DWORD));
		if(pSHADOW)
		{
			LPBYTE pDATA = new BYTE[dwLength];
			LPDIRECT3DSURFACE9 pIMG = NULL;
			D3DXIMAGE_INFO vIMG;

			file.Read( pDATA, dwLength);
			D3DXGetImageInfoFromFileInMemory(
				pDATA,
				dwLength,
				&vIMG);

			CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
				vIMG.Width,
				vIMG.Height,
				D3DFMT_A8R8G8B8,
				D3DPOOL_SCRATCH,
				&pIMG,
				NULL);

			D3DXLoadSurfaceFromFileInMemory(
				pIMG,
				NULL,
				NULL,
				pDATA,
				dwLength,
				NULL,
				D3DX_FILTER_NONE,
				NULL,
				NULL);

			pSHADOW->push_back(pIMG);
			delete[] pDATA;
		}
		else
			file.Seek( dwLength, CFile::current);
	}

	file.Close();
}

void CEXTMAPItem::UpdateUNIT( LPDIRECT3DSURFACE9 pSHADE,
							  LPDIRECT3DSURFACE9 pSKIN,
							  LPDIRECT3DSURFACE9 pTILE,
							  LPVECTORSURFACE pSHADOW,
							  LPBYTE pREGION,
							  LPBYTE pDETAIL,
							  FLOAT *pHEIGHT,
							  DWORD dwIndex)
{
	CFile file;

	while(!file.Open( m_strUNIT, CFile::modeReadWrite|CFile::typeBinary))
		Sleep(200);

	file.Seek( DEF_UNITSIZE * dwIndex, CFile::begin);
	if(pHEIGHT)
	{
		DWORD dwCount = (m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1) * sizeof(FLOAT);

		file.Write( &dwCount, sizeof(DWORD));
		file.Write( pHEIGHT, dwCount);
	}
	else
	{
		DWORD dwCount = 0;

		file.Read( &dwCount, sizeof(DWORD));
		file.Seek( dwCount, CFile::current);
	}

	if(pSHADE)
	{
		LPT3DDATA pDXT = CD3DImage::CreateDXT(
			CMainFrame::m_3DDevice.m_pDevice,
			pSHADE, DXT3);

		file.Write( &pDXT->m_dwSize, sizeof(DWORD));
		file.Write( pDXT->m_pData, pDXT->m_dwSize);

		delete pDXT;
	}
	else
	{
		DWORD dwCount = 0;

		file.Read( &dwCount, sizeof(DWORD));
		file.Seek( dwCount, CFile::current);
	}

	if(pSKIN)
	{
		LPT3DDATA pDXT = CD3DImage::CreateDXT(
			CMainFrame::m_3DDevice.m_pDevice,
			pSKIN, DXT1);

		file.Write( &pDXT->m_dwSize, sizeof(DWORD));
		file.Write( pDXT->m_pData, pDXT->m_dwSize);

		delete pDXT;
	}
	else
	{
		DWORD dwCount = 0;

		file.Read( &dwCount, sizeof(DWORD));
		file.Seek( dwCount, CFile::current);
	}

	if(pREGION)
	{
		DWORD dwCount = m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength * sizeof(BYTE);

		file.Write( &dwCount, sizeof(DWORD));
		file.Write( pREGION, dwCount);
	}
	else
	{
		DWORD dwCount = 0;

		file.Read( &dwCount, sizeof(DWORD));
		file.Seek( dwCount, CFile::current);
	}

	if(pDETAIL)
	{
		DWORD dwCount = m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength * sizeof(BYTE);

		file.Write( &dwCount, sizeof(DWORD));
		file.Write( pDETAIL, dwCount);
	}
	else
	{
		DWORD dwCount = 0;

		file.Read( &dwCount, sizeof(DWORD));
		file.Seek( dwCount, CFile::current);
	}

	if(pTILE)
	{
		LPT3DDATA pDXT = CD3DImage::CreateDXT(
			CMainFrame::m_3DDevice.m_pDevice,
			pTILE, DXT1);

		file.Write( &pDXT->m_dwSize, sizeof(DWORD));
		file.Write( pDXT->m_pData, pDXT->m_dwSize);

		delete pDXT;
	}
	else
	{
		DWORD dwCount = 0;

		file.Read( &dwCount, sizeof(DWORD));
		file.Seek( dwCount, CFile::current);
	}

	if(pSHADOW)
	{
		DWORD dwCount = DWORD(pSHADOW->size());

		file.Write( &dwCount, sizeof(DWORD));
		for( DWORD i=0; i<dwCount; i++)
		{
			LPT3DDATA pDXT = CD3DImage::CreateDXT(
				CMainFrame::m_3DDevice.m_pDevice,
				(*pSHADOW)[i], DXT1);

			file.Write( &pDXT->m_dwSize, sizeof(DWORD));
			file.Write( pDXT->m_pData, pDXT->m_dwSize);

			delete pDXT;
		}
	}
	else
	{
		DWORD dwCount = 0;

		file.Read( &dwCount, sizeof(DWORD));
		for( DWORD i=0; i<dwCount; i++)
		{
			DWORD dwLength = 0;

			file.Read( &dwLength, sizeof(DWORD));
			file.Seek( dwLength, CFile::current);
		}
	}

	file.Close();
}

void CEXTMAPItem::Build( CString strImageFile,
						 D3DXVECTOR3 vLIGHT,
						 FLOAT fMinH,
						 FLOAT fMaxH,
						 BYTE bCell,
						 BYTE bUNIT,
						 DWORD dwUnit,
						 DWORD dwCountX,
						 DWORD dwCountY)
{
	DWORD dwCount = dwCountX * dwCountY;
	CProgressDlg Progress;
	BMPARAM Param;

	m_HugeMAP.m_nUnitCountX = dwCountX;
	m_HugeMAP.m_nUnitCountZ = dwCountY;
	m_HugeMAP.m_nUnitLength = dwUnit;
	m_HugeMAP.m_nTileLength = bUNIT;
	m_HugeMAP.m_nCellCount = bCell;
	m_HugeMAP.m_vLIGHT = vLIGHT;

	m_HugeMAP.m_nTextureSize = CTMath::GetNearPow(dwUnit);
	m_fMinHeight = fMinH;
	m_fMaxHeight = fMaxH;

	m_HugeMAP.InitVB(&CMainFrame::m_3DDevice);
	m_vDETAILS.clear();
	m_vDETAIL.clear();

	for( int i=0; i<dwCount; i++)
		m_HugeMAP.m_pENABLE[i] = TRUE;

	Param.m_strFileName = strImageFile;
	Param.m_pWnd = &Progress;
	Param.m_pHost = this;

	Progress.m_strCancel.LoadString(IDS_CANCEL_BUILD_EXTMAP);
	Progress.m_strTitle.LoadString(IDS_BUILD_EXTMAP);
	Progress.m_pThreadFunc = (AFX_THREADPROC) __Build;
	Progress.m_lParam = &Param;
	Progress.DoModal();
}

void CEXTMAPItem::Build( LPBMPARAM lParam)
{
	BuildHEIGHT(lParam);
	RegulateHEIGHT(lParam);

	BuildSHADE(lParam);
	RegulateSHADE(lParam);

	BuildSKIN(lParam);
	RegulateSKIN(lParam);

	BuildTILETEX(lParam);
	BuildOBJBUF(lParam);

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CEXTMAPItem::BuildOBJBUF( LPBMPARAM lParam)
{
	LPBYTE pDATA = new BYTE[DEF_OBJSIZE];

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_OBJBUF);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	memset( pDATA, 0x00, DEF_OBJSIZE);

	char szTempPath[_MAX_PATH];
	char szTempFile[_MAX_PATH];

	GetStudioTempPath(szTempPath);
	/*GetTempPath(
		_MAX_PATH,
		szTempPath);*/

	GetTempFileName(
		szTempPath,
		_T("_EO"),
		0, szTempFile);

	CFile file( szTempFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	m_strOBJ = file.GetFilePath();

	for( DWORD i=0; i<m_HugeMAP.m_nUnitCountZ; i++)
		for( DWORD j=0; j<m_HugeMAP.m_nUnitCountX; j++)
		{
			file.Write( pDATA, DEF_OBJSIZE);

			lParam->m_pWnd->m_bPercent = (BYTE) ((i * m_HugeMAP.m_nUnitCountX + j) * 100 / (m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ));
			lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
			lParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}

	delete[] pDATA;
}

void CEXTMAPItem::BuildTILETEX( LPBMPARAM lParam)
{
	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_TILETEX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	for( DWORD i=0; i<m_HugeMAP.m_nUnitCountZ; i++)
		for( DWORD j=0; j<m_HugeMAP.m_nUnitCountX; j++)
		{
			BuildTILETEX( j, i);

			lParam->m_pWnd->m_bPercent = (BYTE) ((i * m_HugeMAP.m_nUnitCountX + j) * 100 / (m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ));
			lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
			lParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}
}

void CEXTMAPItem::BuildTILETEX( int nPosX, int nPosZ)
{
	LPBYTE pDETAIL = new BYTE[m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength];
	int nIndex = nPosZ * m_HugeMAP.m_nUnitCountX + nPosX;

	LPDIRECT3DSURFACE9 pTRENDER = NULL;
	LPDIRECT3DSURFACE9 pBACK = NULL;
	LPDIRECT3DSURFACE9 pZBUF = NULL;
	LPDIRECT3DSURFACE9 pTILE = NULL;

	CMainFrame::m_3DDevice.m_pDevice->GetDepthStencilSurface(&pZBUF);
	CMainFrame::m_3DDevice.m_pDevice->GetRenderTarget( 0, &pBACK);

	memset( pDETAIL, -1, m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength * sizeof(BYTE));
	LoadUNIT( NULL, NULL, NULL, NULL, NULL, pDETAIL, NULL, nIndex);

	CMainFrame::m_3DDevice.m_pDevice->CreateRenderTarget(
		m_HugeMAP.m_nUnitLength,
		m_HugeMAP.m_nUnitLength,
		D3DFMT_A8R8G8B8,
		D3DMULTISAMPLE_NONE,
		0, FALSE,
		&pTRENDER,
		NULL);

	CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
		m_HugeMAP.m_nUnitLength,
		m_HugeMAP.m_nUnitLength,
		D3DFMT_A8R8G8B8,
		D3DPOOL_SCRATCH,
		&pTILE,
		NULL);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pTRENDER);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(NULL);

	CMainFrame::m_3DDevice.m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(
		0, 0, 0, 0),
		1.0f, 0);
	CMainFrame::m_3DDevice.m_pDevice->BeginScene();

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_TVERTEX);

	for( int i=0; i<m_HugeMAP.m_nUnitLength; i++)
		for( int j=0; j<m_HugeMAP.m_nUnitLength; j++)
		{
			BYTE bDETAIL = pDETAIL[i * m_HugeMAP.m_nUnitLength + j];
			FLOAT fSCALE = bDETAIL == TILE_NULL ? 32.0f : m_vDETAILS[bDETAIL];

			TVERTEX vPOINT[] = {{
				FLOAT(j),
				FLOAT(i),
				0.5f,
				1.0f,
				fSCALE * FLOAT(j) / FLOAT(m_HugeMAP.m_nUnitLength),
				fSCALE * FLOAT(i) / FLOAT(m_HugeMAP.m_nUnitLength)}, {
				FLOAT(j + 1),
				FLOAT(i),
				0.5f,
				1.0f,
				fSCALE * FLOAT(j + 1) / FLOAT(m_HugeMAP.m_nUnitLength),
				fSCALE * FLOAT(i) / FLOAT(m_HugeMAP.m_nUnitLength)}, {
				FLOAT(j),
				FLOAT(i + 1),
				0.5f,
				1.0f,
				fSCALE * FLOAT(j) / FLOAT(m_HugeMAP.m_nUnitLength),
				fSCALE * FLOAT(i + 1) / FLOAT(m_HugeMAP.m_nUnitLength)}, {
				FLOAT(j + 1),
				FLOAT(i + 1),
				0.5f,
				1.0f,
				fSCALE * FLOAT(j + 1) / FLOAT(m_HugeMAP.m_nUnitLength),
				FLOAT(i + 1) / FLOAT(m_HugeMAP.m_nUnitLength)}};
			CTextureItem *pTEX = bDETAIL == TILE_NULL ? NULL : GetTextureItem(m_vDETAIL[bDETAIL]);

			if(pTEX)
				pTEX->GetTEXTURESET()->m_dwCurTick = 0;

			CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, pTEX ? pTEX->GetTEXTURESET()->GetTexture() : NULL);
			CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
				D3DPT_TRIANGLESTRIP, 2,
				vPOINT,
				sizeof(TVERTEX));
		}
	CMainFrame::m_3DDevice.m_pDevice->EndScene();

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pBACK);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(pZBUF);

	GetRenderTargetData(
		pTRENDER,
		pTILE);

	UpdateUNIT( NULL, NULL, pTILE, NULL, NULL, NULL, NULL, nIndex);
	pTRENDER->Release();
	pBACK->Release();
	pZBUF->Release();
	pTILE->Release();

	delete[] pDETAIL;
}

void CEXTMAPItem::BuildHEIGHT( LPBMPARAM lParam)
{
	FLOAT *pHEIGHT = new FLOAT[(m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1)];
	LPBYTE pDATA = new BYTE[DEF_UNITSIZE];
	LPDWORD pCount = (LPDWORD) pDATA;

	LPDIRECT3DSURFACE9 pDUMY;
	LPDIRECT3DSURFACE9 pIMG;
	D3DXIMAGE_INFO vINFO;

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_GEO);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	D3DXGetImageInfoFromFile(
		lParam->m_strFileName,
		&vINFO);

	CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
		m_HugeMAP.m_nUnitLength + 1,
		m_HugeMAP.m_nUnitLength + 1,
		D3DFMT_A8R8G8B8,
		D3DPOOL_SCRATCH,
		&pIMG,
		NULL);

	CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
		3 * m_HugeMAP.m_nUnitLength,
		3 * m_HugeMAP.m_nUnitLength,
		D3DFMT_A8R8G8B8,
		D3DPOOL_SCRATCH,
		&pDUMY,
		NULL);

	FLOAT fRatioY = ((FLOAT) vINFO.Height) / ((FLOAT) (m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitCountZ + 1));
	FLOAT fRatioX = ((FLOAT) vINFO.Width) / ((FLOAT) (m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitCountX + 1));
	char szTempPath[_MAX_PATH];
	char szTempFile[_MAX_PATH];

	GetStudioTempPath(szTempPath);

	GetTempFileName(
		szTempPath,
		_T("_EM"),
		0, szTempFile);

	CFile file( szTempFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	m_strUNIT = file.GetFilePath();

	for( DWORD i=0; i<m_HugeMAP.m_nUnitCountZ; i++)
		for( DWORD j=0; j<m_HugeMAP.m_nUnitCountX; j++)
		{
			DWORD dwIndex = i * m_HugeMAP.m_nUnitCountX + j;
			CRect rcDEST(
				0, 0,
				3 * m_HugeMAP.m_nUnitLength,
				3 * m_HugeMAP.m_nUnitLength);

			CRect rcSRC(
				-INT(m_HugeMAP.m_nUnitLength),
				-INT(m_HugeMAP.m_nUnitLength),
				2 * m_HugeMAP.m_nUnitLength,
				2 * m_HugeMAP.m_nUnitLength);

			rcSRC.OffsetRect(
				j * m_HugeMAP.m_nUnitLength,
				(m_HugeMAP.m_nUnitCountZ - i - 1) * m_HugeMAP.m_nUnitLength);

			if( rcSRC.left < 0 )
			{
				rcDEST.left += m_HugeMAP.m_nUnitLength;
				rcSRC.left += m_HugeMAP.m_nUnitLength;
			}

			if( rcSRC.top < 0 )
			{
				rcDEST.top += m_HugeMAP.m_nUnitLength;
				rcSRC.top += m_HugeMAP.m_nUnitLength;
			}

			if( rcSRC.right > m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitCountX + 1 )
			{
				rcDEST.right -= m_HugeMAP.m_nUnitLength;
				rcSRC.right -= m_HugeMAP.m_nUnitLength;
			}

			if( rcSRC.bottom > m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitCountZ + 1 )
			{
				rcDEST.bottom -= m_HugeMAP.m_nUnitLength;
				rcSRC.bottom -= m_HugeMAP.m_nUnitLength;
			}

			rcSRC.left = (INT) (((FLOAT) rcSRC.left) * fRatioX);
			rcSRC.right = (INT) (((FLOAT) rcSRC.right) * fRatioX);

			rcSRC.top = (INT) (((FLOAT) rcSRC.top) * fRatioY);
			rcSRC.bottom = (INT) (((FLOAT) rcSRC.bottom) * fRatioY);

			D3DXLoadSurfaceFromFile(
				pDUMY,
				NULL,
				&rcDEST,
				lParam->m_strFileName,
				&rcSRC,
				D3DX_DEFAULT,
				0, NULL);

			rcSRC.SetRect(
				m_HugeMAP.m_nUnitLength,
				m_HugeMAP.m_nUnitLength,
				2 * m_HugeMAP.m_nUnitLength + 1,
				2 * m_HugeMAP.m_nUnitLength + 1);

			D3DXLoadSurfaceFromSurface(
				pIMG,
				NULL,
				NULL,
				pDUMY,
				NULL,
				&rcSRC,
				D3DX_FILTER_NONE, 0);
			BuildHEIGHT( pIMG, dwIndex, pHEIGHT);

			memset( pDATA, 0x00, DEF_UNITSIZE);
			*pCount = (m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1) * sizeof(FLOAT);
			memcpy( pDATA + sizeof(DWORD), pHEIGHT, *pCount);
			file.Write( pDATA, DEF_UNITSIZE);

			lParam->m_pWnd->m_bPercent = (BYTE) (dwIndex * 100 / (m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ));
			lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
			lParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}

	delete[] pHEIGHT;
	delete[] pDATA;

	pDUMY->Release();
	pDUMY = NULL;

	pIMG->Release();
	pIMG = NULL;
}

void CEXTMAPItem::BuildHEIGHT( LPDIRECT3DSURFACE9 pIMG,
							   DWORD dwIndex,
							   FLOAT *pHEIGHT)
{
	D3DLOCKED_RECT lr;

	pIMG->LockRect( &lr, NULL, 0);
	LPBYTE pDATA = (LPBYTE) lr.pBits;
	LPDWORD pCLR = NULL;

	for( int i=0; i<m_HugeMAP.m_nUnitLength + 1; i++)
	{
		pCLR = (LPDWORD) pDATA;

		for( int j=0; j<m_HugeMAP.m_nUnitLength + 1; j++)
		{
			int nIndex = (m_HugeMAP.m_nUnitLength - i) * (m_HugeMAP.m_nUnitLength + 1) + j;
			pHEIGHT[nIndex] = m_fMinHeight + ((FLOAT) GETVALUE_B(pCLR[j])) * (m_fMaxHeight - m_fMinHeight) / ((FLOAT) 0xFF);
		}

		pDATA += lr.Pitch;
	}
	pIMG->UnlockRect();
}

void CEXTMAPItem::RegulateHEIGHT( LPBMPARAM lParam)
{
	lParam->m_pWnd->m_strStatus.LoadString(IDS_REGULATE_HEIGHT);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	for( DWORD i=0; i<m_HugeMAP.m_nUnitCountZ; i++)
		for( DWORD j=0; j<m_HugeMAP.m_nUnitCountX; j++)
		{
			RegulateHEIGHT( j, i);

			lParam->m_pWnd->m_bPercent = (BYTE) ((i * m_HugeMAP.m_nUnitCountX + j) * 100 / (m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ));
			lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
			lParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}
}

void CEXTMAPItem::RegulateHEIGHT( int nPosX, int nPosZ)
{
	FLOAT *pHEIGHT = new FLOAT[(m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1)];
	FLOAT *pH = new FLOAT[(m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1)];

	LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pHEIGHT, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX);
	if( nPosX > 0 && nPosZ > 0 )
	{
		LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pH, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX - 1);
		pH[(m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1) - 1] = pHEIGHT[0];
		UpdateUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pH, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX - 1);
	}

	if( nPosZ > 0 )
	{
		LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pH, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX);
		memcpy( &pH[m_HugeMAP.m_nUnitLength * (m_HugeMAP.m_nUnitLength + 1)], pHEIGHT, (m_HugeMAP.m_nUnitLength + 1) * sizeof(FLOAT));
		UpdateUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pH, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX);
	}

	if( nPosX > 0 )
	{
		LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pH, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX - 1);
		for( int i=0; i<m_HugeMAP.m_nUnitLength + 1; i++)
			pH[(i + 1) * (m_HugeMAP.m_nUnitLength + 1) - 1] = pHEIGHT[i * (m_HugeMAP.m_nUnitLength + 1)];
		UpdateUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pH, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX - 1);
	}

	delete[] pHEIGHT;
	delete[] pH;
}

void CEXTMAPItem::BuildSHADOWTEX( LPBMPARAM lParam)
{
	DWORD dwCount = m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ;

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_SHADOWTEX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	for( DWORD i=0; i<dwCount; i++)
	{
		if(m_HugeMAP.m_pENABLE[i])
			BuildSHADOWTEX( BYTE(lParam->m_vUNIT[0]), i);

		lParam->m_pWnd->m_bPercent = (BYTE) (i * 100 / dwCount);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CEXTMAPItem::BuildSHADOWTEX( BYTE bPixelPerTile,
								  int nUnitID)
{
	D3DXVECTOR3 vLIGHT = m_HugeMAP.m_vLIGHT;
	FLOAT fLength = D3DXVec3Length(&vLIGHT);

	if( fLength > 0.001f )
		vLIGHT /= fLength;

	if( fLength > 0.001f &&
		fabs(vLIGHT.y) > 0.001f && (
		fabs(vLIGHT.x) > 0.001f ||
		fabs(vLIGHT.z) > 0.001f ))
	{
		MAPMAPOBJ::iterator itOBJ;

		VECTORSURFACE vTSHADOW;
		VECTOROBJECT vTOBJ;

		MAPDWORD mapOBJID;
		MAPMAPOBJ mapOBJ;

		vTSHADOW.clear();
		vTOBJ.clear();

		mapOBJID.clear();
		mapOBJ.clear();

		LoadOBJ(
			&mapOBJ,
			NULL,
			NULL,
			&mapOBJID,
			NULL,
			nUnitID);

		for( itOBJ = mapOBJ.begin(); itOBJ != mapOBJ.end(); itOBJ++)
		{
			LPMAPOBJ pOBJ = (*itOBJ).second;
			DWORD dwID = (*itOBJ).first;

			if( pOBJ && pOBJ->m_pOBJ )
			{
				MAPDWORD::iterator itID = mapOBJID.find(dwID);
				COBJItem *pItem = GetOBJItem((*itID).second);

				if(HasTSHADOW( pItem, pOBJ))
				{
					CTachyonObject *pTOBJ = CreateTOBJ(
						pOBJ,
						(*itID).second);

					if(pTOBJ)
						vTOBJ.push_back(pTOBJ);
				}
			}

			delete pOBJ;
		}

		BuildSHADOWTEX(
			&vTSHADOW,
			&vTOBJ,
			&vLIGHT,
			bPixelPerTile,
			nUnitID);

		UpdateUNIT(
			NULL,
			NULL,
			NULL,
			&vTSHADOW,
			NULL,
			NULL,
			NULL,
			nUnitID);

		while(!vTSHADOW.empty())
		{
			vTSHADOW.back()->Release();
			vTSHADOW.pop_back();
		}

		while(!vTOBJ.empty())
		{
			delete vTOBJ.back();
			vTOBJ.pop_back();
		}

		mapOBJID.clear();
		mapOBJ.clear();
	}
}

void CEXTMAPItem::BuildSHADOWTEX( LPVECTORSURFACE pTSHADOW,
								  LPVECTOROBJECT pVOBJ,
								  LPD3DXVECTOR3 pLIGHT,
								  BYTE bPixelPerTile,
								  int nUnitID)
{
	CTachyonEXTMAP extmap;
	CD3DCamera vCAMERA;

	extmap.m_pDATA = new FLOAT[(m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1)];
	extmap.m_nTileCount = m_HugeMAP.m_nUnitLength;

	memset( extmap.m_pDATA, 0x00, sizeof(FLOAT) * (m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1));
	LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, extmap.m_pDATA, nUnitID);

	int nUnitX = nUnitID % m_HugeMAP.m_nUnitCountX;
	int nUnitZ = nUnitID / m_HugeMAP.m_nUnitCountX;
	int nCount = m_HugeMAP.m_nUnitLength * bPixelPerTile;

	FLOAT fLightX = pLIGHT->x / pLIGHT->y;
	FLOAT fLightZ = pLIGHT->z / pLIGHT->y;

	D3DXMATRIX vSHADOW(
		1.0f, 0.0f, 0.0f, 0.0f,
		-fLightX, 0.0f,
		-fLightZ, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	LPDIRECT3DSURFACE9 pTARGET = NULL;
	LPDIRECT3DSURFACE9 pTBUF = NULL;
	LPDIRECT3DSURFACE9 pBACK = NULL;
	LPDIRECT3DSURFACE9 pZBUF = NULL;

	D3DVIEWPORT9 vNEW = {
		0, 0,
		SHADOWTEX_SIZE,
		SHADOWTEX_SIZE,
		0.0f,
		1.0f};
	D3DVIEWPORT9 vOLD;

	nCount = nCount / SHADOWTEX_SIZE + (nCount % SHADOWTEX_SIZE ? 1 : 0);
	nUnitX *= m_HugeMAP.m_nUnitLength;
	nUnitZ *= m_HugeMAP.m_nUnitLength;

	CMainFrame::m_3DDevice.m_pDevice->GetDepthStencilSurface(&pZBUF);
	CMainFrame::m_3DDevice.m_pDevice->GetRenderTarget( 0, &pBACK);
	CMainFrame::m_3DDevice.m_pDevice->GetViewport(&vOLD);

	CMainFrame::m_3DDevice.m_pDevice->CreateRenderTarget(
		SHADOWTEX_SIZE,
		SHADOWTEX_SIZE,
		D3DFMT_A8R8G8B8,
		D3DMULTISAMPLE_NONE,
		0, FALSE,
		&pTARGET,
		NULL);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pTARGET);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(NULL);
	CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vNEW);

	vCAMERA.InitOrthoCamera(
		CMainFrame::m_3DDevice.m_pDevice,
		-MAX_OBJHEIGHT,
		MAX_OBJHEIGHT,
		FLOAT(m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nTileLength) / FLOAT(nCount),
		FLOAT(m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nTileLength) / FLOAT(nCount));

	vCAMERA.SetPosition(
		D3DXVECTOR3( 0.0f, MAX_OBJHEIGHT / 2.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
		FALSE);
	vCAMERA.Activate(TRUE);

	for( int i=0; i<nCount; i++)
	{
		FLOAT fPosZ = FLOAT(m_HugeMAP.m_nTileLength) * (FLOAT(nUnitZ) + (FLOAT(i) + 0.5f) * FLOAT(m_HugeMAP.m_nUnitLength) / FLOAT(nCount));

		for( int j=0; j<nCount; j++)
		{
			FLOAT fPosX = FLOAT(m_HugeMAP.m_nTileLength) * (FLOAT(nUnitX) + (FLOAT(j) + 0.5f) * FLOAT(m_HugeMAP.m_nUnitLength) / FLOAT(nCount));

			CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
				SHADOWTEX_SIZE,
				SHADOWTEX_SIZE,
				D3DFMT_A8R8G8B8,
				D3DPOOL_SCRATCH,
				&pTBUF,
				NULL);

			CMainFrame::m_3DDevice.m_pDevice->Clear(
				0, NULL,
				D3DCLEAR_TARGET,
				D3DCOLOR_ARGB(
				0, 0, 0, 0),
				1.0f, 0);
			CMainFrame::m_3DDevice.m_pDevice->BeginScene();

			for( int k=0; k<INT(pVOBJ->size()); k++)
			{
				CTachyonObject *pOBJ = (*pVOBJ)[k];
				D3DXVECTOR3 vOBJ(
					pOBJ->m_vPosition._41 - fPosX,
					pOBJ->m_vPosition._42 - extmap.GetHeight(
					pOBJ->m_vPosition._41 / m_HugeMAP.m_nTileLength - FLOAT(nUnitX),
					pOBJ->m_vPosition._42,
					pOBJ->m_vPosition._43 / m_HugeMAP.m_nTileLength - FLOAT(nUnitZ)),
					pOBJ->m_vPosition._43 - fPosZ);

				pOBJ->m_vPosition._41 = 0.0f;
				pOBJ->m_vPosition._42 = 0.0f;
				pOBJ->m_vPosition._43 = 0.0f;
				pOBJ->m_vPosition *= vSHADOW;

				pOBJ->m_vPosition._41 += vOBJ.x;
				pOBJ->m_vPosition._42 += vOBJ.y;
				pOBJ->m_vPosition._43 += vOBJ.z;
				pOBJ->m_bLOD = FALSE;

				pOBJ->Render( &CMainFrame::m_3DDevice, &vCAMERA);
				pOBJ->CalcTick( CMainFrame::m_3DDevice.m_pDevice, 0);
				pOBJ->m_bLOD = TRUE;
			}

			CMainFrame::m_3DDevice.m_pDevice->EndScene();
			GetRenderTargetData(
				pTARGET,
				pTBUF);
			pTSHADOW->push_back(pTBUF);
		}
	}

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pBACK);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(pZBUF);
	CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vOLD);

	m_HugeMAP.m_nShadowPixel = bPixelPerTile;
	pTARGET->Release();
	pBACK->Release();
	pZBUF->Release();
}

BYTE CEXTMAPItem::HasTSHADOW( COBJItem *pItem,
							  LPMAPOBJ pOBJ)
{
	MAPATTRIBUTE::iterator itATTR;

	if( !pItem || !pOBJ || !pOBJ->m_pOBJ )
		return FALSE;

	for( itATTR = pOBJ->m_mapATTR.begin(); itATTR != pOBJ->m_mapATTR.end(); itATTR++)
	{
		MAPSTR::iterator itSTR = pItem->m_mapNAME.find((*itATTR).first);

		if( itSTR != pItem->m_mapNAME.end() &&
			(*itSTR).second == CString("ID_OBJ_TYPE") )
		{
			LPATTRIBUTE pATTR = (*itATTR).second;

			if( pATTR && pATTR->m_bValueType == ATTRTYPE_INT && (
				*((int *) (*itATTR).second->m_pValue) == 5 ||
				*((int *) (*itATTR).second->m_pValue) == 6 ))
				return FALSE;

			return TRUE;
		}
	}

	for( itATTR = pOBJ->m_pOBJ->m_mapATTR.begin(); itATTR != pOBJ->m_pOBJ->m_mapATTR.end(); itATTR++)
	{
		MAPSTR::iterator itSTR = pItem->m_mapNAME.find((*itATTR).first);

		if( itSTR != pItem->m_mapNAME.end() &&
			(*itSTR).second == CString("ID_OBJ_TYPE") )
		{
			LPATTRIBUTE pATTR = (*itATTR).second;

			if( pATTR && pATTR->m_bValueType == ATTRTYPE_INT && (
				*((int *) (*itATTR).second->m_pValue) == 5 ||
				*((int *) (*itATTR).second->m_pValue) == 6 ))
				return FALSE;

			break;
		}
	}
	return TRUE;
}

CTachyonObject *CEXTMAPItem::CreateTOBJ( LPMAPOBJ pOBJ,
										 DWORD dwItemID)
{
	COBJItem *pItem = GetOBJItem(dwItemID);
	CTachyonObject *pTOBJ = NULL;

	if(pItem)
	{
		pTOBJ = new CTachyonObject();

		pOBJ->m_pOBJ->m_bPivotCount = BYTE(pItem->GetPivotCount() + 1);
		pTOBJ->InitOBJ( &CMainFrame::m_3DDevice, pOBJ);
		pOBJ->m_pOBJ->m_bPivotCount = MAX_PIVOT;
		pTOBJ->CalcFrame(FALSE);
	}

	return pTOBJ;
}

void CEXTMAPItem::BuildSHADE( LPBMPARAM lParam)
{
	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_SHADE);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	for( DWORD i=0; i<m_HugeMAP.m_nUnitCountZ; i++)
		for( DWORD j=0; j<m_HugeMAP.m_nUnitCountX; j++)
		{
			BuildSHADE( j, i);

			lParam->m_pWnd->m_bPercent = (BYTE) ((i * m_HugeMAP.m_nUnitCountX + j) * 100 / (m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ));
			lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
			lParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}
}

void CEXTMAPItem::BuildSHADE( int nPosX, int nPosZ)
{
	D3DXVECTOR3 vDirection = m_HugeMAP.m_vLIGHT / D3DXVec3Length(&m_HugeMAP.m_vLIGHT);
	LPDIRECT3DSURFACE9 pIMG = NULL;
	D3DLOCKED_RECT lr;

	FLOAT *pHEIGHT = new FLOAT[(m_HugeMAP.m_nUnitLength + 3) * (m_HugeMAP.m_nUnitLength + 3)];
	FLOAT *pCENTER = new FLOAT[(m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1)];
	FLOAT *pSIDE = new FLOAT[(m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1)];
	BYTE *pSHADE = new BYTE[(m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1)];

	LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pCENTER, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX);
	if( nPosX > 0 && nPosZ > 0 )
	{
		LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pSIDE, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX - 1);
		pHEIGHT[0] = pSIDE[m_HugeMAP.m_nUnitLength * (m_HugeMAP.m_nUnitLength + 1) - 2];
	}
	else
		pHEIGHT[0] = pCENTER[0];

	if( nPosZ > 0 )
	{
		LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pSIDE, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX);
		memcpy( &pHEIGHT[1], &pSIDE[(m_HugeMAP.m_nUnitLength - 1) * (m_HugeMAP.m_nUnitLength + 1)], (m_HugeMAP.m_nUnitLength + 1) * sizeof(FLOAT));
	}
	else
		memcpy( &pHEIGHT[1], pCENTER, (m_HugeMAP.m_nUnitLength + 1) * sizeof(FLOAT));

	if( nPosX + 1 < m_HugeMAP.m_nUnitCountX && nPosZ > 0 )
	{
		LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pSIDE, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX + 1);
		pHEIGHT[m_HugeMAP.m_nUnitLength + 2] = pSIDE[(m_HugeMAP.m_nUnitLength - 1) * (m_HugeMAP.m_nUnitLength + 1) + 1];
	}
	else
		pHEIGHT[m_HugeMAP.m_nUnitLength + 2] = pCENTER[m_HugeMAP.m_nUnitLength];

	if( nPosX > 0 )
	{
		LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pSIDE, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX - 1);

		for( int i=0; i<m_HugeMAP.m_nUnitLength + 1; i++)
			pHEIGHT[(i + 1) * (m_HugeMAP.m_nUnitLength + 3)] = pSIDE[(i + 1) * (m_HugeMAP.m_nUnitLength + 1) - 2];
	}
	else
	{
		for( int i=0; i<m_HugeMAP.m_nUnitLength + 1; i++)
			pHEIGHT[(i + 1) * (m_HugeMAP.m_nUnitLength + 3)] = pCENTER[i * (m_HugeMAP.m_nUnitLength + 1)];
	}

	if( nPosX + 1 < m_HugeMAP.m_nUnitCountX )
	{
		LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pSIDE, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX + 1);

		for( int i=0; i<m_HugeMAP.m_nUnitLength + 1; i++)
			pHEIGHT[(i + 2) * (m_HugeMAP.m_nUnitLength + 3) - 1] = pSIDE[i * (m_HugeMAP.m_nUnitLength + 1) + 1];
	}
	else
	{
		for( int i=0; i<m_HugeMAP.m_nUnitLength + 1; i++)
			pHEIGHT[(i + 2) * (m_HugeMAP.m_nUnitLength + 3) - 1] = pCENTER[(i + 1) * (m_HugeMAP.m_nUnitLength + 1) - 1];
	}

	if( nPosX > 0 && nPosZ + 1 < m_HugeMAP.m_nUnitCountZ )
	{
		LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pSIDE, (nPosZ + 1) * m_HugeMAP.m_nUnitCountX + nPosX - 1);
		pHEIGHT[(m_HugeMAP.m_nUnitLength + 2) * (m_HugeMAP.m_nUnitLength + 3)] = pSIDE[2 * (m_HugeMAP.m_nUnitLength + 1) - 2];
	}
	else
		pHEIGHT[(m_HugeMAP.m_nUnitLength + 2) * (m_HugeMAP.m_nUnitLength + 3)] = pCENTER[m_HugeMAP.m_nUnitLength * (m_HugeMAP.m_nUnitLength + 1)];

	if( nPosZ + 1 < m_HugeMAP.m_nUnitCountZ )
	{
		LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pSIDE, (nPosZ + 1) * m_HugeMAP.m_nUnitCountX + nPosX);
		memcpy( &pHEIGHT[(m_HugeMAP.m_nUnitLength + 2) * (m_HugeMAP.m_nUnitLength + 3) + 1], &pSIDE[m_HugeMAP.m_nUnitLength + 1], (m_HugeMAP.m_nUnitLength + 1) * sizeof(FLOAT));
	}
	else
		memcpy( &pHEIGHT[(m_HugeMAP.m_nUnitLength + 2) * (m_HugeMAP.m_nUnitLength + 3) + 1], &pCENTER[m_HugeMAP.m_nUnitLength * (m_HugeMAP.m_nUnitLength + 1)], (m_HugeMAP.m_nUnitLength + 1) * sizeof(FLOAT));

	if( nPosX + 1 < m_HugeMAP.m_nUnitCountX && nPosZ + 1 < m_HugeMAP.m_nUnitCountZ )
	{
		LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pSIDE, (nPosZ + 1) * m_HugeMAP.m_nUnitCountX + nPosX + 1);
		pHEIGHT[(m_HugeMAP.m_nUnitLength + 3) * (m_HugeMAP.m_nUnitLength + 3) - 1] = pSIDE[m_HugeMAP.m_nUnitLength + 2];
	}
	else
		pHEIGHT[(m_HugeMAP.m_nUnitLength + 3) * (m_HugeMAP.m_nUnitLength + 3) - 1] = pCENTER[(m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1) - 1];

	for( int i=0; i<m_HugeMAP.m_nUnitLength + 1; i++)
		memcpy( &pHEIGHT[(i + 1) * (m_HugeMAP.m_nUnitLength + 3) + 1], &pCENTER[i * (m_HugeMAP.m_nUnitLength + 1)], (m_HugeMAP.m_nUnitLength + 1) * sizeof(FLOAT));

	for( i=0; i<m_HugeMAP.m_nUnitLength + 1; i++)
		for( int j=0; j<m_HugeMAP.m_nUnitLength + 1; j++)
		{
			D3DXVECTOR3 vPoint[10] = {
				D3DXVECTOR3( (FLOAT) (j + 1), pHEIGHT[(i + 1) * (m_HugeMAP.m_nUnitLength + 3) + (j + 1)], (FLOAT) (i + 1)),
				D3DXVECTOR3( (FLOAT) j, pHEIGHT[i * (m_HugeMAP.m_nUnitLength + 3) + j], (FLOAT) i),
				D3DXVECTOR3( (FLOAT) (j + 1), pHEIGHT[i * (m_HugeMAP.m_nUnitLength + 3) + (j + 1)], (FLOAT) i),
				D3DXVECTOR3( (FLOAT) (j + 2), pHEIGHT[i * (m_HugeMAP.m_nUnitLength + 3) + (j + 2)], (FLOAT) i),
				D3DXVECTOR3( (FLOAT) (j + 2), pHEIGHT[(i + 1) * (m_HugeMAP.m_nUnitLength + 3) + (j + 2)], (FLOAT) (i + 1)),
				D3DXVECTOR3( (FLOAT) (j + 2), pHEIGHT[(i + 2) * (m_HugeMAP.m_nUnitLength + 3) + (j + 2)], (FLOAT) (i + 2)),
				D3DXVECTOR3( (FLOAT) (j + 1), pHEIGHT[(i + 2) * (m_HugeMAP.m_nUnitLength + 3) + (j + 1)], (FLOAT) (i + 2)),
				D3DXVECTOR3( (FLOAT) j, pHEIGHT[(i + 2) * (m_HugeMAP.m_nUnitLength + 3) + j], (FLOAT) (i + 2)),
				D3DXVECTOR3( (FLOAT) j, pHEIGHT[(i + 1) * (m_HugeMAP.m_nUnitLength + 3) + j], (FLOAT) (i + 1)),
				D3DXVECTOR3( (FLOAT) j, pHEIGHT[i * (m_HugeMAP.m_nUnitLength + 3) + j], (FLOAT) i)};
			D3DXVECTOR3 vNormal( 0.0f, 0.0f, 0.0f);

			for( int k=0; k<10; k++)
			{
				vPoint[k].x *= FLOAT(m_HugeMAP.m_nTileLength);
				vPoint[k].z *= FLOAT(m_HugeMAP.m_nTileLength);
			}

			for( k=0; k<8; k++)
			{
				D3DXVECTOR3 vCross;

				D3DXVec3Cross(
					&vCross,
					&(vPoint[k + 2] - vPoint[0]),
					&(vPoint[k + 1] - vPoint[0]));

				vCross /= D3DXVec3Length(&vCross);
				vNormal += vCross;
			}
			vNormal /= D3DXVec3Length(&vNormal);

			FLOAT fDOT = max( -0.8f, min( 0.8f, D3DXVec3Dot(
				&vDirection,
				&vNormal)));
			pSHADE[i * (m_HugeMAP.m_nUnitLength + 1) + j] = (BYTE) (0xFF * (1.0f + fDOT / 0.8f) / 2.0f);
		}

	CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
		m_HugeMAP.m_nTextureSize,
		m_HugeMAP.m_nTextureSize,
		D3DFMT_A8R8G8B8,
		D3DPOOL_SCRATCH,
		&pIMG,
		NULL);

	pIMG->LockRect( &lr, NULL, 0);
	LPBYTE pDATA = (LPBYTE) lr.pBits;
	LPDWORD pCLR = NULL;

	for( i=0; i<m_HugeMAP.m_nTextureSize; i++)
	{
		int nLocZ = i * (m_HugeMAP.m_nUnitLength + 1) / m_HugeMAP.m_nTextureSize;
		pCLR = (LPDWORD) pDATA;

		for( int j=0; j<m_HugeMAP.m_nTextureSize; j++)
		{
			int nLocX = j * (m_HugeMAP.m_nUnitLength + 1) / m_HugeMAP.m_nTextureSize;
			pCLR[j] = VALUEARGB(
				pSHADE[nLocZ * (m_HugeMAP.m_nUnitLength + 1) + nLocX],
				0x00, 0x00, 0x00);
		}

		pDATA += lr.Pitch;
	}
	pIMG->UnlockRect();

	UpdateUNIT( pIMG, NULL, NULL, NULL, NULL, NULL, NULL, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX);
	pIMG->Release();

	delete[] pHEIGHT;
	delete[] pCENTER;
	delete[] pSHADE;
	delete[] pSIDE;
}

void CEXTMAPItem::RegulateSHADE( LPBMPARAM lParam)
{
	lParam->m_pWnd->m_strStatus.LoadString(IDS_REGULATE_SHADE);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	for( DWORD i=0; i<m_HugeMAP.m_nUnitCountZ; i++)
		for( DWORD j=0; j<m_HugeMAP.m_nUnitCountX; j++)
		{
			RegulateSHADE( j, i);

			lParam->m_pWnd->m_bPercent = (BYTE) ((i * m_HugeMAP.m_nUnitCountX + j) * 100 / (m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ));
			lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
			lParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}
}

void CEXTMAPItem::RegulateSHADE( int nPosX, int nPosZ)
{
	LPDIRECT3DSURFACE9 pSHADE = NULL;
	LPDIRECT3DSURFACE9 pSIDE = NULL;
	D3DLOCKED_RECT lr;

	LoadUNIT( &pSHADE, NULL, NULL, NULL, NULL, NULL, NULL, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX);
	pSHADE->LockRect( &lr, NULL, 0);

	if( nPosX > 0 && nPosZ > 0 )
	{
		D3DLOCKED_RECT lrSIDE;

		LoadUNIT( &pSIDE, NULL, NULL, NULL, NULL, NULL, NULL, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX - 1);
		pSIDE->LockRect( &lrSIDE, NULL, 0);

		LPBYTE pDATADEST = (LPBYTE) lrSIDE.pBits;
		LPBYTE pDATASRC = (LPBYTE) lr.pBits;

		LPDWORD pCLRDEST = (LPDWORD) (pDATADEST + (m_HugeMAP.m_nTextureSize - 1) * lrSIDE.Pitch);
		LPDWORD pCLRSRC = (LPDWORD) pDATASRC;
		pCLRDEST[m_HugeMAP.m_nTextureSize - 1] = pCLRSRC[0];

		pSIDE->UnlockRect();
		UpdateUNIT( pSIDE, NULL, NULL, NULL, NULL, NULL, NULL, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX - 1);
		pSIDE->Release();
		pSIDE = NULL;
	}

	if( nPosZ > 0 )
	{
		D3DLOCKED_RECT lrSIDE;

		LoadUNIT( &pSIDE, NULL, NULL, NULL, NULL, NULL, NULL, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX);
		pSIDE->LockRect( &lrSIDE, NULL, 0);

		LPBYTE pDATADEST = (LPBYTE) lrSIDE.pBits;
		LPBYTE pDATASRC = (LPBYTE) lr.pBits;

		LPDWORD pCLRDEST = (LPDWORD) (pDATADEST + (m_HugeMAP.m_nTextureSize - 1) * lrSIDE.Pitch);
		LPDWORD pCLRSRC = (LPDWORD) pDATASRC;
		memcpy( pCLRDEST, pCLRSRC, lr.Pitch);

		pSIDE->UnlockRect();
		UpdateUNIT( pSIDE, NULL, NULL, NULL, NULL, NULL, NULL, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX);
		pSIDE->Release();
		pSIDE = NULL;
	}

	if( nPosX > 0 )
	{
		D3DLOCKED_RECT lrSIDE;

		LoadUNIT( &pSIDE, NULL, NULL, NULL, NULL, NULL, NULL, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX - 1);
		pSIDE->LockRect( &lrSIDE, NULL, 0);

		LPBYTE pDATADEST = (LPBYTE) lrSIDE.pBits;
		LPBYTE pDATASRC = (LPBYTE) lr.pBits;

		for( int i=0; i<m_HugeMAP.m_nTextureSize; i++)
		{
			LPDWORD pCLRDEST = (LPDWORD) pDATADEST;
			LPDWORD pCLRSRC = (LPDWORD) pDATASRC;

			pCLRDEST[m_HugeMAP.m_nTextureSize - 1] = pCLRSRC[0];
			pDATADEST += lrSIDE.Pitch;
			pDATASRC += lr.Pitch;
		}

		pSIDE->UnlockRect();
		UpdateUNIT( pSIDE, NULL, NULL, NULL, NULL, NULL, NULL, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX - 1);
		pSIDE->Release();
		pSIDE = NULL;
	}

	pSHADE->UnlockRect();
	pSHADE->Release();
}

void CEXTMAPItem::BuildSKIN( LPBMPARAM lParam)
{
	MAPLAYER::iterator it;
	MAPLAYER mapLAYER;

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_GEOTEX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	mapLAYER.clear();
	BuildLAYER(&mapLAYER);

	for( DWORD i=0; i<m_HugeMAP.m_nUnitCountZ; i++)
		for( DWORD j=0; j<m_HugeMAP.m_nUnitCountX; j++)
		{
			BuildSKIN( &mapLAYER, j, i);

			lParam->m_pWnd->m_bPercent = (BYTE) ((i * m_HugeMAP.m_nUnitCountX + j) * 100 / (m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ));
			lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
			lParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}

	for( it = mapLAYER.begin(); it != mapLAYER.end(); it++)
		delete (*it).second;
	mapLAYER.clear();
}

void CEXTMAPItem::BuildSKIN( MAPLAYER *pLAYER,
							 int nPosX,
							 int nPosZ)
{
	FLOAT *pHEIGHT = new FLOAT[(m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1)];

	LPDIRECT3DSURFACE9 pIMG = NULL;
	D3DLOCKED_RECT lr;

	LPDWORD pIMGCLR = NULL;
	LPBYTE pIMGDATA = NULL;

	LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pHEIGHT, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX);
	CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
		m_HugeMAP.m_nTextureSize,
		m_HugeMAP.m_nTextureSize,
		D3DFMT_A8R8G8B8,
		D3DPOOL_SCRATCH,
		&pIMG,
		NULL);

	int nCount = INT(pLAYER->size());
	pIMG->LockRect( &lr, NULL, 0);
	pIMGDATA = (LPBYTE) lr.pBits;

	if( nCount > 0 )
	{
		D3DLOCKED_RECT *pLR = new D3DLOCKED_RECT[nCount];
		LPLAYER *pINFO = new LPLAYER[nCount];
		LPDWORD *pCLR = new LPDWORD[nCount];
		LPBYTE *pDATA = new LPBYTE[nCount];

		MAPLAYER::iterator it;
		int nLAYER = 0;

		for( it = pLAYER->begin(); it != pLAYER->end(); it++)
		{
			pINFO[nLAYER] = (*it).second;
			pINFO[nLAYER]->m_pLAYER->LockRect( &pLR[nLAYER], NULL, D3DLOCK_READONLY);
			nLAYER++;
		}

		for( int i=0; i<m_HugeMAP.m_nTextureSize; i++)
		{
			int nLocY = INT(i + nPosZ * m_HugeMAP.m_nTextureSize);

			for( nLAYER=0; nLAYER<nCount; nLAYER++)
			{
				pDATA[nLAYER] = (LPBYTE) pLR[nLAYER].pBits;
				pDATA[nLAYER] += (nLocY % pINFO[nLAYER]->m_nHeight) * pLR[nLAYER].Pitch;
				pCLR[nLAYER] = (LPDWORD) pDATA[nLAYER];
			}

			pIMGCLR = (LPDWORD) pIMGDATA;
			pIMGDATA += lr.Pitch;

			for( int j=0; j<m_HugeMAP.m_nTextureSize; j++)
			{
				FLOAT fAlpha = 1.0f;

				int nLAYER = GetLayer(
					pLAYER,
					pHEIGHT,
					j, i,
					fAlpha);

				int nLocX = INT(j + nPosX * m_HugeMAP.m_nTextureSize);
				int nPREV = nLAYER > 0 ? nLAYER - 1 : nLAYER;

				FLOAT fGreen = fAlpha * ((FLOAT) GETVALUE_G(pCLR[nLAYER][nLocX % pINFO[nLAYER]->m_nWidth]));
				FLOAT fBlue = fAlpha * ((FLOAT) GETVALUE_B(pCLR[nLAYER][nLocX % pINFO[nLAYER]->m_nWidth]));
				FLOAT fRed = fAlpha * ((FLOAT) GETVALUE_R(pCLR[nLAYER][nLocX % pINFO[nLAYER]->m_nWidth]));
				fAlpha = 1.0f - fAlpha;

				fGreen += fAlpha * ((FLOAT) GETVALUE_G(pCLR[nPREV][nLocX % pINFO[nLAYER]->m_nWidth]));
				fBlue += fAlpha * ((FLOAT) GETVALUE_B(pCLR[nPREV][nLocX % pINFO[nLAYER]->m_nWidth]));
				fRed += fAlpha * ((FLOAT) GETVALUE_R(pCLR[nPREV][nLocX % pINFO[nLAYER]->m_nWidth]));
				pIMGCLR[j] = VALUEARGB( 0xFF, fRed, fGreen, fBlue);
			}
		}

		for( it = pLAYER->begin(); it != pLAYER->end(); it++)
			(*it).second->m_pLAYER->UnlockRect();

		delete[] pINFO;
		delete[] pDATA;
		delete[] pCLR;
		delete[] pLR;
	}
	else
		memset( pIMGDATA, 0xFF, lr.Pitch * m_HugeMAP.m_nTextureSize);
	pIMG->UnlockRect();

	UpdateUNIT( NULL, pIMG, NULL, NULL, NULL, NULL, NULL, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX);
	pIMG->Release();

	delete[] pHEIGHT;
}

void CEXTMAPItem::RegulateSKIN( LPBMPARAM lParam)
{
	lParam->m_pWnd->m_strStatus.LoadString(IDS_REGULATE_SKIN);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	for( DWORD i=0; i<m_HugeMAP.m_nUnitCountZ; i++)
		for( DWORD j=0; j<m_HugeMAP.m_nUnitCountX; j++)
		{
			RegulateSKIN( j, i);

			lParam->m_pWnd->m_bPercent = (BYTE) ((i * m_HugeMAP.m_nUnitCountX + j) * 100 / (m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ));
			lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
			lParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}
}

void CEXTMAPItem::RegulateSKIN( int nPosX, int nPosZ)
{
	LPDIRECT3DSURFACE9 pSKIN = NULL;
	LPDIRECT3DSURFACE9 pSIDE = NULL;
	D3DLOCKED_RECT lr;

	LoadUNIT( NULL, &pSKIN, NULL, NULL, NULL, NULL, NULL, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX);
	pSKIN->LockRect( &lr, NULL, 0);

	if( nPosX > 0 && nPosZ > 0 )
	{
		D3DLOCKED_RECT lrSIDE;

		LoadUNIT( NULL, &pSIDE, NULL, NULL, NULL, NULL, NULL, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX - 1);
		pSIDE->LockRect( &lrSIDE, NULL, 0);

		LPBYTE pDATADEST = (LPBYTE) lrSIDE.pBits;
		LPBYTE pDATASRC = (LPBYTE) lr.pBits;

		LPDWORD pCLRDEST = (LPDWORD) (pDATADEST + (m_HugeMAP.m_nTextureSize - 1) * lrSIDE.Pitch);
		LPDWORD pCLRSRC = (LPDWORD) pDATASRC;
		pCLRDEST[m_HugeMAP.m_nTextureSize - 1] = pCLRSRC[0];

		pSIDE->UnlockRect();
		UpdateUNIT( NULL, pSIDE, NULL, NULL, NULL, NULL, NULL, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX - 1);
		pSIDE->Release();
		pSIDE = NULL;
	}

	if( nPosZ > 0 )
	{
		D3DLOCKED_RECT lrSIDE;

		LoadUNIT( NULL, &pSIDE, NULL, NULL, NULL, NULL, NULL, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX);
		pSIDE->LockRect( &lrSIDE, NULL, 0);

		LPBYTE pDATADEST = (LPBYTE) lrSIDE.pBits;
		LPBYTE pDATASRC = (LPBYTE) lr.pBits;

		LPDWORD pCLRDEST = (LPDWORD) (pDATADEST + (m_HugeMAP.m_nTextureSize - 1) * lrSIDE.Pitch);
		LPDWORD pCLRSRC = (LPDWORD) pDATASRC;
		memcpy( pCLRDEST, pCLRSRC, lr.Pitch);

		pSIDE->UnlockRect();
		UpdateUNIT( NULL, pSIDE, NULL, NULL, NULL, NULL, NULL, (nPosZ - 1) * m_HugeMAP.m_nUnitCountX + nPosX);
		pSIDE->Release();
		pSIDE = NULL;
	}

	if( nPosX > 0 )
	{
		D3DLOCKED_RECT lrSIDE;

		LoadUNIT( NULL, &pSIDE, NULL, NULL, NULL, NULL, NULL, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX - 1);
		pSIDE->LockRect( &lrSIDE, NULL, 0);

		LPBYTE pDATADEST = (LPBYTE) lrSIDE.pBits;
		LPBYTE pDATASRC = (LPBYTE) lr.pBits;

		for( int i=0; i<m_HugeMAP.m_nTextureSize; i++)
		{
			LPDWORD pCLRDEST = (LPDWORD) pDATADEST;
			LPDWORD pCLRSRC = (LPDWORD) pDATASRC;

			pCLRDEST[m_HugeMAP.m_nTextureSize - 1] = pCLRSRC[0];
			pDATADEST += lrSIDE.Pitch;
			pDATASRC += lr.Pitch;
		}

		pSIDE->UnlockRect();
		UpdateUNIT( NULL, pSIDE, NULL, NULL, NULL, NULL, NULL, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX - 1);
		pSIDE->Release();
		pSIDE = NULL;
	}

	pSKIN->UnlockRect();
	pSKIN->Release();
}

void CEXTMAPItem::BuildLAYER( MAPLAYER *pLAYER)
{
	int nCount = INT(m_vGEO.size());

	for( int i=0; i<nCount; i++)
	{
		CTextureItem *pTEX = CItemBase::GetTextureItem(m_vGEO[i]->m_dwTEXID);

		if( pTEX && pTEX->GetTexData() )
		{
			MAPLAYER::iterator finder = pLAYER->find(m_vGEO[i]->m_fHeight);
			LPLAYER pDATA = new LAYER();
			D3DSURFACE_DESC desc;

			if( finder != pLAYER->end() )
			{
				delete (*finder).second;
				pLAYER->erase(finder);
			}

			pTEX->GetLPDIRECT3DTEXTURE9()->GetLevelDesc( 0, &desc);
			pDATA->m_nHeight = desc.Height;
			pDATA->m_nWidth = desc.Width;

			CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
				desc.Width,
				desc.Height,
				D3DFMT_A8R8G8B8,
				D3DPOOL_SCRATCH,
				&pDATA->m_pLAYER,
				NULL);

			D3DXLoadSurfaceFromFileInMemory(
				pDATA->m_pLAYER,
				NULL,
				NULL,
				pTEX->GetTexData(),
				pTEX->m_dwSize,
				NULL,
				D3DX_FILTER_NONE,
				0, NULL);

			pLAYER->insert( MAPLAYER::value_type( m_vGEO[i]->m_fHeight, pDATA));
		}
	}
}

int CEXTMAPItem::GetLayer( MAPLAYER *pLAYER,
						   FLOAT *pHEIGHT,
						   int nPosX,
						   int nPosY,
						   FLOAT& fAlpha)
{
	nPosY *= m_HugeMAP.m_nUnitLength;
	nPosX *= m_HugeMAP.m_nUnitLength;

	nPosY /= m_HugeMAP.m_nTextureSize;
	nPosX /= m_HugeMAP.m_nTextureSize;

	int nIndex = nPosY * (m_HugeMAP.m_nUnitLength + 1) + nPosX;
	int nCount = INT(pLAYER->size());
	int nResult = 0;

	FLOAT fHeight = pHEIGHT[nIndex];
	FLOAT fMinHeight = 0.0f;
	FLOAT fMaxHeight = 0.0f;

	MAPLAYER::iterator it;
	for( it = pLAYER->begin(); it != pLAYER->end(); it++)
		if( (*it).first > fHeight )
		{
			fMaxHeight = (*it).first;
			break;
		}
		else
		{
			fMinHeight = (*it).first;
			nResult++;
		}

	if( fMaxHeight - fMinHeight > 0.0f )
		fAlpha = (fHeight - fMinHeight) / (fMaxHeight - fMinHeight);
	else
		fAlpha = 0.0f;

	if( nResult >= nCount )
	{
		nResult = nCount - 1;
		fAlpha = 1.0f;
	}

	if( fAlpha > 1.0f )
		fAlpha = 1.0f;

	return nResult;
}

void CEXTMAPItem::ExpOBJ( LPBMPARAM lParam)
{
	CFile file( lParam->m_strFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_EXP_OBJ);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	DWORD dwCount = DWORD(lParam->m_vUNIT.size());
	DWORD dwVER = __T3D_SDK_VER;

	MAPDWORD mapUNIT;
	mapUNIT.clear();

	for( DWORD i=0; i<dwCount; i++)
		mapUNIT.insert( MAPDWORD::value_type( lParam->m_vUNIT[i], 0));

	dwCount = m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ;
	file.Write( &dwVER, sizeof(DWORD));
	file.Write( &dwCount, sizeof(DWORD));
	file.Write( &m_HugeMAP.m_nUnitCountX, sizeof(int));
	file.Write( &m_HugeMAP.m_nTileLength, sizeof(int));
	file.Write( &m_HugeMAP.m_nUnitLength, sizeof(int));

	for( i=0; i<dwCount; i++)
	{
		int nEnable = mapUNIT.find(i) == mapUNIT.end() ? FALSE : TRUE;

		file.Write( &nEnable, sizeof(int));
		if(nEnable)
		{
			MAPMAPOBJ::iterator itOBJ;
			MAPMAPSFX::iterator itSFX;
			MAPMAPSND::iterator itSND;

			MAPMAPOBJ mapOBJ;
			MAPMAPSFX mapSFX;
			MAPMAPSND mapSND;

			MAPDWORD mapOBJID;
			MAPDWORD mapSFXID;

			mapOBJ.clear();
			mapSFX.clear();
			mapSND.clear();

			mapOBJID.clear();
			mapSFXID.clear();

			LoadOBJ(
				&mapOBJ,
				&mapSFX,
				&mapSND,
				&mapOBJID,
				&mapSFXID, i);

			DWORD dwOBJ = (DWORD) mapOBJ.size();
			file.Write( &dwOBJ, sizeof(DWORD));

			for( itOBJ = mapOBJ.begin(); itOBJ != mapOBJ.end(); itOBJ++)
			{
				LPMAPOBJ pMAPOBJ = (*itOBJ).second;
				DWORD dwID = (*itOBJ).first;

				MAPDWORD::iterator itID = mapOBJID.find(dwID);
				MAPATTRIBUTE::iterator itATTR;
				MAPCLKINST::iterator itCLK;
				DWORD dwItemID = (*itID).second;

				file.Write( &dwID, sizeof(DWORD));
				file.Write( &dwItemID, sizeof(DWORD));

				file.Write( &pMAPOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
				file.Write( &pMAPOBJ->m_vPOS, sizeof(D3DXVECTOR3));
				file.Write( &pMAPOBJ->m_vROT, sizeof(D3DXVECTOR3));

				file.Write( &pMAPOBJ->m_dwOBJFuncID, sizeof(DWORD));
				file.Write( &pMAPOBJ->m_dwACTFuncID, sizeof(DWORD));
				file.Write( &pMAPOBJ->m_dwANIFuncID, sizeof(DWORD));

				file.Write( &pMAPOBJ->m_dwCurANI, sizeof(DWORD));
				file.Write( &pMAPOBJ->m_dwCurACT, sizeof(DWORD));

				int nCLK = (INT) pMAPOBJ->m_mapCLK.size();
				file.Write( &nCLK, sizeof(int));

				for( itCLK = pMAPOBJ->m_mapCLK.begin(); itCLK != pMAPOBJ->m_mapCLK.end(); itCLK++)
				{
					LPCLKINST pCLK = (*itCLK).second;
					DWORD dwCLK = (*itCLK).first;

					file.Write( &dwCLK, sizeof(DWORD));
					file.Write( &pCLK->m_dwCurMESH, sizeof(DWORD));
					file.Write( &pCLK->m_dwCurCL, sizeof(DWORD));
				}

				COBJItem *pITEM = CItemBase::GetOBJItem(dwItemID);
				if(pITEM)
				{
					MAPATTRIBUTE mapATTR;

					mapATTR.clear();
					for( itATTR = pMAPOBJ->m_mapATTR.begin(); itATTR != pMAPOBJ->m_mapATTR.end(); itATTR++)
					{
						MAPSTR::iterator finder = pITEM->m_mapNAME.find((*itATTR).first);

						if( finder != pITEM->m_mapNAME.end() )
							mapATTR.insert( MAPATTRIBUTE::value_type( (*itATTR).first, (*itATTR).second));
					}

					int nATTR = INT(mapATTR.size());
					file.Write( &nATTR, sizeof(int));

					for( itATTR = mapATTR.begin(); itATTR != mapATTR.end(); itATTR++)
					{
						MAPSTR::iterator finder = pITEM->m_mapNAME.find((*itATTR).first);

						WriteString( &file, (*finder).second);
						file.Write( &(*itATTR).second->m_bValueType, sizeof(BYTE));
						file.Write( &(*itATTR).second->m_nSize, sizeof(int));
						file.Write( (*itATTR).second->m_pValue, (*itATTR).second->m_nSize);
					}
				}
				else
				{
					int nATTR = 0;
					file.Write( &nATTR, sizeof(int));
				}

				delete pMAPOBJ;
			}

			DWORD dwSFX = (DWORD) mapSFX.size();
			file.Write( &dwSFX, sizeof(DWORD));

			for( itSFX = mapSFX.begin(); itSFX != mapSFX.end(); itSFX++)
			{
				LPMAPSFX pMAPSFX = (*itSFX).second;
				DWORD dwID = (*itSFX).first;

				MAPDWORD::iterator itID = mapSFXID.find(dwID);
				DWORD dwItemID = (*itID).second;

				file.Write( &dwID, sizeof(DWORD));
				file.Write( &dwItemID, sizeof(DWORD));

				file.Write( &pMAPSFX->m_vPOS, sizeof(D3DXVECTOR3));
				file.Write( &pMAPSFX->m_vROT, sizeof(D3DXVECTOR3));
				file.Write( &pMAPSFX->m_bBias, sizeof(BYTE));

				delete pMAPSFX;
			}

			DWORD dwSND = (DWORD) mapSND.size();
			file.Write( &dwSND, sizeof(DWORD));

			for( itSND = mapSND.begin(); itSND != mapSND.end(); itSND++)
			{
				LPMAPSND pMAPSND = (*itSND).second;
				DWORD dwID = (*itSND).first;

				file.Write( &dwID, sizeof(DWORD));
				file.Write( &pMAPSND->m_dwMediaID, sizeof(DWORD));
				file.Write( &pMAPSND->m_vPOS, sizeof(D3DXVECTOR3));

				delete pMAPSND;
			}

			mapOBJ.clear();
			mapSFX.clear();
			mapSND.clear();

			mapOBJID.clear();
			mapSFXID.clear();
		}
		else
		{
			DWORD dwOBJ = 0;
			file.Write( &dwOBJ, sizeof(DWORD));
			file.Write( &dwOBJ, sizeof(DWORD));
			file.Write( &dwOBJ, sizeof(DWORD));
		}

		lParam->m_pWnd->m_bPercent = (BYTE) (i * 100 / dwCount);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}
	mapUNIT.clear();

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CEXTMAPItem::ImpOBJ( LPBMPARAM lParam)
{
	CFile file( lParam->m_strFileName, CFile::modeRead|CFile::typeBinary);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_IMP_OBJ);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	DWORD dwCount;
	DWORD dwVER;

	int nUnitLength;
	int nUnitCountX;
	int nTileLength;

	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;

	MAPMAPOBJ mapOBJ;
	MAPMAPSFX mapSFX;
	MAPMAPSND mapSND;

	MAPDWORD mapOBJID;
	MAPDWORD mapSFXID;

	MAPDWORD mapALLOBJID;
	MAPDWORD mapALLSFXID;
	MAPDWORD mapALLSNDID;

	dwCount = m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ;
	mapALLOBJID.clear();
	mapALLSFXID.clear();
	mapALLSNDID.clear();

	for( DWORD i=0; i<dwCount; i++)
	{
		mapOBJ.clear();
		mapSFX.clear();
		mapSND.clear();

		mapOBJID.clear();
		mapSFXID.clear();

		LoadOBJ(
			&mapOBJ,
			&mapSFX,
			&mapSND,
			&mapOBJID,
			&mapSFXID, i);

		for( itOBJ = mapOBJ.begin(); itOBJ != mapOBJ.end(); itOBJ++)
		{
			if( mapALLOBJID.find((*itOBJ).first) == mapALLOBJID.end() )
				mapALLOBJID.insert( MAPDWORD::value_type( (*itOBJ).first, (*itOBJ).first));

			if((*itOBJ).second)
				delete (*itOBJ).second;
		}

		for( itSFX = mapSFX.begin(); itSFX != mapSFX.end(); itSFX++)
		{
			if( mapALLSFXID.find((*itSFX).first) == mapALLSFXID.end() )
				mapALLSFXID.insert( MAPDWORD::value_type( (*itSFX).first, (*itSFX).first));

			if((*itSFX).second)
				delete (*itSFX).second;
		}

		for( itSND = mapSND.begin(); itSND != mapSND.end(); itSND++)
		{
			if( mapALLSNDID.find((*itSND).first) == mapALLSNDID.end() )
				mapALLSNDID.insert( MAPDWORD::value_type( (*itSND).first, (*itSND).first));

			if((*itSND).second)
				delete (*itSND).second;
		}

		mapOBJ.clear();
		mapSFX.clear();
		mapSND.clear();

		mapOBJID.clear();
		mapSFXID.clear();
	}

	if(!mapALLOBJID.empty())
	{
		MAPDWORD::iterator itMAX = mapALLOBJID.end();
		itMAX--;

		m_dwNewOBJID = (*itMAX).first + 1;
	}
	else
		m_dwNewOBJID = 1;

	if(!mapALLSFXID.empty())
	{
		MAPDWORD::iterator itMAX = mapALLSFXID.end();
		itMAX--;

		m_dwNewSFXID = (*itMAX).first + 1;
	}
	else
		m_dwNewSFXID = 1;

	if(!mapALLSNDID.empty())
	{
		MAPDWORD::iterator itMAX = mapALLSNDID.end();
		itMAX--;

		m_dwNewSNDID = (*itMAX).first + 1;
	}
	else
		m_dwNewSNDID = 1;

	file.Read( &dwVER, sizeof(DWORD));
	file.Read( &dwCount, sizeof(DWORD));

	file.Read( &nUnitCountX, sizeof(int));
	file.Read( &nTileLength, sizeof(int));
	file.Read( &nUnitLength, sizeof(int));

	for( i=0; i<dwCount; i++)
	{
		MAPSTRID mapATTRID;
		MAPSTR mapNAME;

		MAPMAPOBJ mapEXTOBJ;
		MAPMAPSFX mapEXTSFX;
		MAPMAPSND mapEXTSND;

		MAPDWORD mapEXTOBJID;
		MAPDWORD mapEXTSFXID;

		DWORD dwCOUNT = 0;
		DWORD dwROOT = 0;
		int nEnable;

		file.Read( &nEnable, sizeof(int));
		file.Read( &dwCOUNT, sizeof(DWORD));

		mapATTRID.clear();
		mapNAME.clear();

		mapEXTOBJ.clear();
		mapEXTSFX.clear();
		mapEXTSND.clear();

		mapEXTOBJID.clear();
		mapEXTSFXID.clear();

		for( int j=0; j<dwCOUNT; j++)
		{
			DWORD dwItemID;
			DWORD dwID;

			file.Read( &dwID, sizeof(DWORD));
			file.Read( &dwItemID, sizeof(DWORD));

			LPMAPOBJ pMAPOBJ = new MAPOBJ();
			int nCNT;

			mapEXTOBJID.insert( MAPDWORD::value_type(
				dwID,
				dwItemID));

			file.Read( &pMAPOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
			file.Read( &pMAPOBJ->m_vPOS, sizeof(D3DXVECTOR3));
			file.Read( &pMAPOBJ->m_vROT, sizeof(D3DXVECTOR3));

			file.Read( &pMAPOBJ->m_dwOBJFuncID, sizeof(DWORD));
			file.Read( &pMAPOBJ->m_dwACTFuncID, sizeof(DWORD));
			file.Read( &pMAPOBJ->m_dwANIFuncID, sizeof(DWORD));

			file.Read( &pMAPOBJ->m_dwCurANI, sizeof(DWORD));
			file.Read( &pMAPOBJ->m_dwCurACT, sizeof(DWORD));

			file.Read( &nCNT, sizeof(int));
			for( int k=0; k<nCNT; k++)
			{
				LPCLKINST pCLK = new CLKINST();
				DWORD dwCLK = 0;

				file.Read( &dwCLK, sizeof(DWORD));
				file.Read( &pCLK->m_dwCurMESH, sizeof(DWORD));
				file.Read( &pCLK->m_dwCurCL, sizeof(DWORD));

				pMAPOBJ->m_mapCLK.insert( MAPCLKINST::value_type(
					dwCLK,
					pCLK));
			}

			file.Read( &nCNT, sizeof(int));
			for( k=0; k<nCNT; k++)
			{
				LPATTRIBUTE pATTR = new ATTRIBUTE();
				CString strATTR = LoadString(&file);

				file.Read( &pATTR->m_bValueType, sizeof(BYTE));
				file.Read( &pATTR->m_nSize, sizeof(int));
				pATTR->m_pValue = new BYTE[pATTR->m_nSize];
				file.Read( pATTR->m_pValue, pATTR->m_nSize);

				MAPSTRID::iterator finder = mapATTRID.find(strATTR);
				DWORD dwATTR = 0;

				if( finder == mapATTRID.end() )
				{
					dwATTR = dwROOT;
					dwROOT++;

					mapATTRID.insert( MAPSTRID::value_type( strATTR, dwATTR));
					mapNAME.insert( MAPSTR::value_type( dwATTR, strATTR));
				}
				else
					dwATTR = (*finder).second;

				pMAPOBJ->m_mapATTR.insert( MAPATTRIBUTE::value_type( dwATTR, pATTR));
			}

			mapEXTOBJ.insert( MAPMAPOBJ::value_type( dwID, pMAPOBJ));
		}

		file.Read( &dwCOUNT, sizeof(DWORD));
		for( j=0; j<dwCOUNT; j++)
		{
			LPMAPSFX pMAPSFX = new MAPSFX();

			DWORD dwItemID;
			DWORD dwID;

			file.Read( &dwID, sizeof(DWORD));
			file.Read( &dwItemID, sizeof(DWORD));

			mapEXTSFXID.insert( MAPDWORD::value_type(
				dwID,
				dwItemID));

			file.Read( &pMAPSFX->m_vPOS, sizeof(D3DXVECTOR3));
			file.Read( &pMAPSFX->m_vROT, sizeof(D3DXVECTOR3));
			file.Read( &pMAPSFX->m_bBias, sizeof(BYTE));

			mapEXTSFX.insert( MAPMAPSFX::value_type( dwID, pMAPSFX));
		}

		file.Read( &dwCOUNT, sizeof(DWORD));
		for( j=0; j<dwCOUNT; j++)
		{
			LPMAPSND pMAPSND = new MAPSND();
			DWORD dwID;

			file.Read( &dwID, sizeof(DWORD));
			file.Read( &pMAPSND->m_dwMediaID, sizeof(DWORD));
			file.Read( &pMAPSND->m_vPOS, sizeof(D3DXVECTOR3));

			mapEXTSND.insert( MAPMAPSND::value_type( dwID, pMAPSND));
		}

		if(nEnable)
		{
			CRect vUNIT(
				0, 0,
				nUnitLength,
				nUnitLength);

			vUNIT.OffsetRect(
				(i % nUnitCountX) * nUnitLength,
				(i / nUnitCountX) * nUnitLength);

			if(vUNIT.IntersectRect( &vUNIT, &m_vFROM))
			{
				vUNIT.OffsetRect(
					m_vTO.x - m_vFROM.left,
					m_vTO.y - m_vFROM.top);

				int nUnitZ = vUNIT.top / m_HugeMAP.m_nUnitLength;
				int nCountZ = vUNIT.bottom / m_HugeMAP.m_nUnitLength - nUnitZ + 1;

				for( j=0; j<nCountZ; j++)
				{
					int nUnitX = vUNIT.left / m_HugeMAP.m_nUnitLength;
					int nCountX = vUNIT.right / m_HugeMAP.m_nUnitLength - nUnitX + 1;

					for( int k=0; k<nCountX; k++)
					{
						CRect vTARGET(
							0, 0,
							m_HugeMAP.m_nUnitLength,
							m_HugeMAP.m_nUnitLength);

						vTARGET.OffsetRect(
							(nUnitX + k) * m_HugeMAP.m_nUnitLength,
							(nUnitZ + j) * m_HugeMAP.m_nUnitLength);

						if( nUnitX + k >= 0 && nUnitX + k < m_HugeMAP.m_nUnitCountX &&
							nUnitZ + j >= 0 && nUnitZ + j < m_HugeMAP.m_nUnitCountZ &&
							vTARGET.IntersectRect( &vTARGET, &vUNIT))
						{
							int nUnitID = (nUnitZ + j) * m_HugeMAP.m_nUnitCountX + nUnitX + k;

							mapOBJ.clear();
							mapSFX.clear();
							mapSND.clear();

							mapOBJID.clear();
							mapSFXID.clear();

							m_HugeMAP.m_pENABLE[nUnitID] = TRUE;
							LoadOBJ(
								&mapOBJ,
								&mapSFX,
								&mapSND,
								&mapOBJID,
								&mapSFXID,
								nUnitID);

							itOBJ = mapEXTOBJ.begin();
							while( itOBJ != mapEXTOBJ.end() )
							{
								LPMAPOBJ pMAPOBJ = (*itOBJ).second;
								MAPMAPOBJ::iterator itNEXT = itOBJ;

								D3DXVECTOR3 vPOS(pMAPOBJ->m_vPOS);
								itNEXT++;

								vPOS.x /= FLOAT(nTileLength);
								vPOS.z /= FLOAT(nTileLength);

								vPOS.x += FLOAT(m_vTO.x - m_vFROM.left);
								vPOS.z += FLOAT(m_vTO.y - m_vFROM.top);

								if(vTARGET.PtInRect(CPoint( INT(vPOS.x), INT(vPOS.z))))
								{
									MAPDWORD::iterator itID = mapEXTOBJID.find((*itOBJ).first);

									if( itID != mapEXTOBJID.end() )
									{
										COBJItem *pItem = GetOBJItem((*itID).second);

										if(pItem)
										{
											DWORD dwInstID = mapALLOBJID.find((*itOBJ).first) == mapALLOBJID.end() ? (*itOBJ).first : m_dwNewOBJID;

											vPOS.x *= FLOAT(m_HugeMAP.m_nTileLength);
											vPOS.z *= FLOAT(m_HugeMAP.m_nTileLength);
											pMAPOBJ->m_vPOS = vPOS;

											RegulateOBJATTR(
												pMAPOBJ,
												&mapNAME,
												pItem);

											mapALLOBJID.insert( MAPDWORD::value_type( dwInstID, dwInstID));
											m_dwNewOBJID = max( dwInstID + 1, m_dwNewOBJID);

											mapOBJID.insert( MAPDWORD::value_type( dwInstID, (*itID).second));
											mapOBJ.insert( MAPMAPOBJ::value_type( dwInstID, pMAPOBJ));

											mapEXTOBJID.erase(itID);
											mapEXTOBJ.erase(itOBJ);
										}
									}
								}

								itOBJ = itNEXT;
							}

							itSFX = mapEXTSFX.begin();
							while( itSFX != mapEXTSFX.end() )
							{
								LPMAPSFX pMAPSFX = (*itSFX).second;
								MAPMAPSFX::iterator itNEXT = itSFX;

								D3DXVECTOR3 vPOS(pMAPSFX->m_vPOS);
								itNEXT++;

								vPOS.x /= FLOAT(nTileLength);
								vPOS.z /= FLOAT(nTileLength);

								vPOS.x += FLOAT(m_vTO.x - m_vFROM.left);
								vPOS.z += FLOAT(m_vTO.y - m_vFROM.top);

								if(vTARGET.PtInRect(CPoint( INT(vPOS.x), INT(vPOS.z))))
								{
									MAPDWORD::iterator itID = mapEXTSFXID.find((*itSFX).first);

									if( itID != mapEXTSFXID.end() && GetSFXItem((*itID).second) )
									{
										DWORD dwInstID = mapALLSFXID.find((*itSFX).first) == mapALLSFXID.end() ? (*itSFX).first : m_dwNewSFXID;

										vPOS.x *= FLOAT(m_HugeMAP.m_nTileLength);
										vPOS.z *= FLOAT(m_HugeMAP.m_nTileLength);
										pMAPSFX->m_vPOS = vPOS;

										mapALLSFXID.insert( MAPDWORD::value_type( dwInstID, dwInstID));
										m_dwNewSFXID = max( dwInstID + 1, m_dwNewSFXID);

										mapSFXID.insert( MAPDWORD::value_type( dwInstID, (*itID).second));
										mapSFX.insert( MAPMAPSFX::value_type( dwInstID, pMAPSFX));

										mapEXTSFXID.erase(itID);
										mapEXTSFX.erase(itSFX);
									}
								}

								itSFX = itNEXT;
							}

							itSND = mapEXTSND.begin();
							while( itSND != mapEXTSND.end() )
							{
								LPMAPSND pMAPSND = (*itSND).second;
								MAPMAPSND::iterator itNEXT = itSND;

								D3DXVECTOR3 vPOS(pMAPSND->m_vPOS);
								itNEXT++;

								vPOS.x /= FLOAT(nTileLength);
								vPOS.z /= FLOAT(nTileLength);

								vPOS.x += FLOAT(m_vTO.x - m_vFROM.left);
								vPOS.z += FLOAT(m_vTO.y - m_vFROM.top);

								if( vTARGET.PtInRect(CPoint( INT(vPOS.x), INT(vPOS.z))) && GetMediaItem(pMAPSND->m_dwMediaID) )
								{
									DWORD dwInstID = mapALLSNDID.find((*itSND).first) == mapALLSNDID.end() ? (*itSND).first : m_dwNewSNDID;

									vPOS.x *= FLOAT(m_HugeMAP.m_nTileLength);
									vPOS.z *= FLOAT(m_HugeMAP.m_nTileLength);
									pMAPSND->m_vPOS = vPOS;

									mapALLSNDID.insert( MAPDWORD::value_type( dwInstID, dwInstID));
									m_dwNewSNDID = max( dwInstID + 1, m_dwNewSNDID);

									mapSND.insert( MAPMAPSND::value_type( dwInstID, pMAPSND));
									mapEXTSND.erase(itSND);
								}

								itSND = itNEXT;
							}

							UpdateOBJ(
								&mapOBJ,
								&mapSFX,
								&mapSND,
								&mapOBJID,
								&mapSFXID,
								nUnitID);

							for( itOBJ = mapOBJ.begin(); itOBJ != mapOBJ.end(); itOBJ++)
								if((*itOBJ).second)
									delete (*itOBJ).second;

							for( itSFX = mapSFX.begin(); itSFX != mapSFX.end(); itSFX++)
								if((*itSFX).second)
									delete (*itSFX).second;

							for( itSND = mapSND.begin(); itSND != mapSND.end(); itSND++)
								if((*itSND).second)
									delete (*itSND).second;

							mapOBJ.clear();
							mapSFX.clear();
							mapSND.clear();

							mapOBJID.clear();
							mapSFXID.clear();

							BuildSHADOWTEX( m_HugeMAP.m_nShadowPixel, nUnitID);
						}
					}
				}
			}
		}

		for( itOBJ = mapEXTOBJ.begin(); itOBJ != mapEXTOBJ.end(); itOBJ++)
			if((*itOBJ).second)
				delete (*itOBJ).second;

		for( itSFX = mapEXTSFX.begin(); itSFX != mapEXTSFX.end(); itSFX++)
			if((*itSFX).second)
				delete (*itSFX).second;

		for( itSND = mapEXTSND.begin(); itSND != mapEXTSND.end(); itSND++)
			if((*itSND).second)
				delete (*itSND).second;

		mapATTRID.clear();
		mapNAME.clear();

		mapEXTOBJ.clear();
		mapEXTSFX.clear();
		mapEXTSND.clear();

		mapEXTOBJID.clear();
		mapEXTSFXID.clear();

		lParam->m_pWnd->m_bPercent = (BYTE) (i * 100 / dwCount);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	mapALLOBJID.clear();
	mapALLSFXID.clear();
	mapALLSNDID.clear();

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CEXTMAPItem::RegulateOBJATTR( LPMAPOBJ pMAPOBJ,
								   LPMAPSTR pNAME,
								   COBJItem *pItem)
{
	if( !pMAPOBJ || !pNAME || !pItem )
		return;

	MAPATTRITEM::iterator itATTR;
	MAPATTRIBUTE::iterator it;

	MAPATTRIBUTE mapATTR;
	MAPSTRID mapNAME;

	mapATTR.clear();
	mapNAME.clear();

	for( it = pMAPOBJ->m_mapATTR.begin(); it != pMAPOBJ->m_mapATTR.end(); it++)
		mapATTR.insert( MAPATTRIBUTE::value_type( (*it).first, (*it).second));
	pMAPOBJ->m_mapATTR.clear();

	for( itATTR = pItem->m_mapATTR.begin(); itATTR != pItem->m_mapATTR.end(); itATTR++)
	{
		MAPSTR::iterator finder = pItem->m_mapNAME.find((*itATTR).first);

		if( finder != pItem->m_mapNAME.end() && mapNAME.find((*finder).second) == mapNAME.end() )
			mapNAME.insert( MAPSTRID::value_type( (*finder).second, (*finder).first));
	}

	for( it = mapATTR.begin(); it != mapATTR.end(); it++)
	{
		MAPSTR::iterator finder = pNAME->find((*it).first);
		LPATTRIBUTE pATTR = (*it).second;

		if( pATTR && finder != pNAME->end() )
		{
			MAPSTRID::iterator itID = mapNAME.find((*finder).second);
			DWORD dwATTR = ROOT_ATTRID;

			if( itID == mapNAME.end() )
			{
				LPATTRITEM pATTRITEM = new ATTRITEM();

				if(!pItem->m_mapATTR.empty())
				{
					itATTR = pItem->m_mapATTR.end();
					itATTR--;

					dwATTR = (*itATTR).first + 1;
				}

				pATTRITEM->m_bType = pATTR->m_bValueType;
				switch(pATTRITEM->m_bType)
				{
				case ATTRTYPE_STRING	: pATTRITEM->m_strValue = CString(pATTR->m_pValue).Left(pATTR->m_nSize); break;
				case ATTRTYPE_FLOAT		: pATTRITEM->m_strValue.Format( "%.2f", *((FLOAT *) pATTR->m_pValue)); break;
				case ATTRTYPE_INT		: pATTRITEM->m_strValue.Format( "%d", *((int *) pATTR->m_pValue)); break;
				}

				pItem->m_mapNAME.insert( MAPSTR::value_type( dwATTR, (*finder).second));
				pItem->m_mapATTR.insert( MAPATTRITEM::value_type( dwATTR, pATTRITEM));
				mapNAME.insert( MAPSTRID::value_type( (*finder).second, dwATTR));

				RegLocalID( (*finder).second, pItem);
			}
			else
				dwATTR = (*itID).second;

			if( pMAPOBJ->m_mapATTR.find(dwATTR) == pMAPOBJ->m_mapATTR.end() )
			{
				pMAPOBJ->m_mapATTR.insert( MAPATTRIBUTE::value_type( dwATTR, pATTR));
				pATTR = NULL;
			}
		}

		if(pATTR)
			delete pATTR;
	}

	mapATTR.clear();
	mapNAME.clear();
}

void CEXTMAPItem::ReIndexTEX( LPMAPDWORD pINDEX,
							  LPVECTORDWORD pVDETAIL,
							  LPVECTORFLOAT pVDETAILS,
							  LPVECTORDWORD pVUNIT)
{
	if( !pINDEX || !pVDETAIL || !pVDETAILS || !pVUNIT )
		return;

	DWORD dwSIZE = m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength;
	int nCount = INT(pVUNIT->size());

	LPBYTE pDETAIL = new BYTE[dwSIZE];
	MAPDWORD::iterator it;

	pVDETAILS->clear();
	pVDETAIL->clear();
	pINDEX->clear();

	for( int i=0; i<nCount; i++)
	{
		memset( pDETAIL, TILE_NULL, dwSIZE * sizeof(BYTE));
		LoadUNIT(
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			pDETAIL,
			NULL,
			(*pVUNIT)[i]);

		for( DWORD j=0; j<dwSIZE; j++)
		{
			BYTE bTILE = pDETAIL[j] & 0x7F;

			if( pDETAIL[j] != TILE_NULL && pINDEX->find(bTILE) == pINDEX->end() && m_vDETAIL[bTILE] )
				pINDEX->insert( MAPDWORD::value_type( bTILE, TILE_NULL));
		}
	}

	delete[] pDETAIL;
	pDETAIL = NULL;

	for( it = pINDEX->begin(); it != pINDEX->end(); it++)
	{
		(*it).second = DWORD(pVDETAIL->size());

		pVDETAILS->push_back(m_vDETAILS[(*it).first]);
		pVDETAIL->push_back(m_vDETAIL[(*it).first]);
	}
}

void CEXTMAPItem::ResetDETAIL( LPMAPDWORD pINDEX,
							   LPBYTE pDETAIL,
							   DWORD dwCOUNT)
{
	if( !pINDEX || !pDETAIL )
		return;

	for( DWORD i=0; i<dwCOUNT; i++)
	{
		BYTE bTILE = pDETAIL[i] & 0x7F;
		MAPDWORD::iterator finder = pINDEX->find(bTILE);

		if( finder != pINDEX->end() )
		{
			bTILE = (BYTE) (*finder).second;

			if(pDETAIL[i] & 0x80)
				bTILE |= 0x80;

			pDETAIL[i] = bTILE;
		}
		else
			pDETAIL[i] = TILE_NULL;
	}
}

void CEXTMAPItem::ExpGEO( LPBMPARAM lParam)
{
	CFile file( lParam->m_strFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_EXP_GEO);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	FLOAT *pHEIGHT = new FLOAT[(m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1)];
	LPBYTE pREGION = new BYTE[m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength];
	LPBYTE pDETAIL = new BYTE[m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength];
	DWORD dwVER = __T3D_SDK_VER;

	file.Write( &dwVER, sizeof(DWORD));
	file.Write( &m_HugeMAP.m_nUnitLength, sizeof(DWORD));

	VECTORFLOAT vDETAILS;
	VECTORDWORD vDETAIL;

	MAPDWORD mapINDEX;
	MAPDWORD mapUNIT;

	DWORD dwCount = DWORD(lParam->m_vUNIT.size());
	mapUNIT.clear();

	for( DWORD i=0; i<dwCount; i++)
		mapUNIT.insert( MAPDWORD::value_type( lParam->m_vUNIT[i], 0));

	ReIndexTEX(
		&mapINDEX,
		&vDETAIL,
		&vDETAILS,
		&lParam->m_vUNIT);

	dwCount = INT(vDETAIL.size());
	file.Write( &dwCount, sizeof(DWORD));

	for( i=0; i<dwCount; i++)
	{
		file.Write( &vDETAIL[i], sizeof(DWORD));
		file.Write( &vDETAILS[i], sizeof(FLOAT));
	}

	dwCount = m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ;
	file.Write( &dwCount, sizeof(DWORD));
	file.Write( &m_HugeMAP.m_nUnitCountX, sizeof(int));
	file.Write( &m_HugeMAP.m_nTileLength, sizeof(int));

	for( i=0; i<dwCount; i++)
	{
		BYTE bEnable = mapUNIT.find(i) == mapUNIT.end() ? FALSE : TRUE;

		file.Write( &bEnable, sizeof(BYTE));
		if(bEnable)
		{
			LPDIRECT3DSURFACE9 pSHADE;
			LPDIRECT3DSURFACE9 pSKIN;
			LPDIRECT3DSURFACE9 pTILE;
			VECTORSURFACE vSHADOW;

			LoadUNIT(
				&pSHADE,
				&pSKIN,
				&pTILE,
				&vSHADOW,
				pREGION,
				pDETAIL,
				pHEIGHT, i);

			DWORD dwLength = (m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1) * sizeof(FLOAT);
			file.Write( &dwLength, sizeof(DWORD));
			file.Write( pHEIGHT, dwLength);

			dwLength = m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength * sizeof(BYTE);
			file.Write( &dwLength, sizeof(DWORD));
			file.Write( pREGION, dwLength);

			ResetDETAIL( &mapINDEX, pDETAIL, m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength);
			dwLength = m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength * sizeof(BYTE);
			file.Write( &dwLength, sizeof(DWORD));
			file.Write( pDETAIL, dwLength);

			if(pSHADE)
			{
				LPT3DDATA pDXT = CD3DImage::CreateDXT(
					CMainFrame::m_3DDevice.m_pDevice,
					pSHADE, DXT3);

				file.Write( &pDXT->m_dwSize, sizeof(DWORD));
				file.Write( pDXT->m_pData, pDXT->m_dwSize);

				pSHADE->Release();
				delete pDXT;
			}
			else
			{
				dwLength = 0;
				file.Write( &dwLength, sizeof(DWORD));
			}

			if(pSKIN)
			{
				LPT3DDATA pDXT = CD3DImage::CreateDXT(
					CMainFrame::m_3DDevice.m_pDevice,
					pSKIN, DXT1);

				file.Write( &pDXT->m_dwSize, sizeof(DWORD));
				file.Write( pDXT->m_pData, pDXT->m_dwSize);

				pSKIN->Release();
				delete pDXT;
			}
			else
			{
				dwLength = 0;
				file.Write( &dwLength, sizeof(DWORD));
			}

			if(pTILE)
			{
				LPT3DDATA pDXT = CD3DImage::CreateDXT(
					CMainFrame::m_3DDevice.m_pDevice,
					pTILE, DXT1);

				file.Write( &pDXT->m_dwSize, sizeof(DWORD));
				file.Write( pDXT->m_pData, pDXT->m_dwSize);

				pTILE->Release();
				delete pDXT;
			}
			else
			{
				dwLength = 0;
				file.Write( &dwLength, sizeof(DWORD));
			}

			if(!vSHADOW.empty())
			{	
				DWORD dwCount = DWORD(vSHADOW.size());
				file.Write( &dwCount, sizeof(DWORD));

				for( DWORD j=0; j<dwCount; j++)
				{
					LPDIRECT3DSURFACE9 pSHADOW = vSHADOW[j];

					LPT3DDATA pDXT = CD3DImage::CreateDXT(
						CMainFrame::m_3DDevice.m_pDevice,
						pSHADOW, DXT1);

					file.Write( &pDXT->m_dwSize, sizeof(DWORD));
					file.Write( pDXT->m_pData, pDXT->m_dwSize);

					delete pDXT;
				}

				for( j=0; j<vSHADOW.size(); j++)
					vSHADOW[j]->Release();
				vSHADOW.clear();
			}
			else
			{
				dwLength = 0;
				file.Write( &dwLength, sizeof(DWORD));
			}
		}

		lParam->m_pWnd->m_bPercent = (BYTE) (i * 100 / dwCount);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	delete[] pHEIGHT;
	delete[] pREGION;
	delete[] pDETAIL;

	vDETAILS.clear();
	vDETAIL.clear();

	mapINDEX.clear();
	mapUNIT.clear();

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CEXTMAPItem::ImpGEO( LPBMPARAM lParam)
{
	CFile file( lParam->m_strFileName, CFile::modeRead|CFile::typeBinary);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_IMP_GEO);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	FLOAT *pHEIGHT = new FLOAT[(m_HugeMAP.m_nUnitLength + 1) * (m_HugeMAP.m_nUnitLength + 1)];
	LPBYTE pREGION = new BYTE[m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength];
	LPBYTE pDETAIL = new BYTE[m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength];

	MAPDWORD mapINDEX;
	MAPDWORD mapTILE;

	DWORD dwCount = DWORD(m_vDETAIL.size());
	DWORD dwVER;
	DWORD dwUNIT;

	int nUnitCountX;
	int nTileLength;

	mapINDEX.clear();
	mapTILE.clear();

	for( DWORD i=0; i<dwCount; i++)
		if(m_vDETAIL[i])
		{
			MAPDWORD::iterator finder = mapTILE.find(m_vDETAIL[i]);

			if( finder == mapTILE.end() )
				mapTILE.insert( MAPDWORD::value_type( m_vDETAIL[i], i));
		}

	file.Read( &dwVER, sizeof(DWORD));
	file.Read( &dwUNIT, sizeof(DWORD));
	file.Read( &dwCount, sizeof(DWORD));

	for( i=0; i<dwCount; i++)
	{
		BYTE bTILE = TILE_NULL;

		DWORD dwDETAIL;
		FLOAT fDETAILS;

		file.Read( &dwDETAIL, sizeof(DWORD));
		file.Read( &fDETAILS, sizeof(FLOAT));

		if(dwDETAIL)
		{
			MAPDWORD::iterator finder = mapTILE.find(dwDETAIL);

			if( finder == mapTILE.end() )
			{
				bTILE = BYTE(m_vDETAIL.size());
				mapTILE.insert( MAPDWORD::value_type( dwDETAIL, DWORD(bTILE)));

				m_vDETAILS.push_back(fDETAILS);
				m_vDETAIL.push_back(dwDETAIL);
			}
			else
				bTILE = BYTE((*finder).second);
		}

		if( mapINDEX.find(i) == mapINDEX.end() )
			mapINDEX.insert( MAPDWORD::value_type( i, DWORD(bTILE)));
	}

	file.Read( &dwCount, sizeof(DWORD));
	file.Read( &nUnitCountX, sizeof(int));
	file.Read( &nTileLength, sizeof(int));

	FLOAT *pEXTHEIGHT = new FLOAT[(dwUNIT + 1) * (dwUNIT + 1)];
	LPBYTE pEXTREGION = new BYTE[dwUNIT * dwUNIT];
	LPBYTE pEXTDETAIL = new BYTE[dwUNIT * dwUNIT];

	for( i=0; i<dwCount; i++)
	{
		BYTE bEnable;

		file.Read( &bEnable, sizeof(BYTE));
		if(bEnable)
		{
			LPDIRECT3DSURFACE9 pEXTSKIN = NULL;

			DWORD dwSHADOW;
			DWORD dwLength;

			CRect vUNIT(
				0, 0,
				dwUNIT,
				dwUNIT);

			vUNIT.OffsetRect(
				(i % nUnitCountX) * dwUNIT,
				(i / nUnitCountX) * dwUNIT);

			file.Read( &dwLength, sizeof(DWORD));
			file.Read( pEXTHEIGHT, dwLength);

			file.Read( &dwLength, sizeof(DWORD));
			file.Read( pEXTREGION, dwLength);

			file.Read( &dwLength, sizeof(DWORD));
			file.Read( pEXTDETAIL, dwLength);

			file.Read( &dwLength, sizeof(DWORD));
			if(dwLength)
				file.Seek( dwLength, CFile::current);

			file.Read( &dwLength, sizeof(DWORD));
			if(dwLength)
			{
				LPBYTE pTBUF = new BYTE[dwLength];
				D3DXIMAGE_INFO vINFO;

				file.Read( pTBUF, dwLength);
				D3DXGetImageInfoFromFileInMemory(
					pTBUF,
					dwLength,
					&vINFO);

				CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
					vINFO.Width,
					vINFO.Height,
					D3DFMT_A8R8G8B8,
					D3DPOOL_SCRATCH,
					&pEXTSKIN,
					NULL);

				D3DXLoadSurfaceFromFileInMemory(
					pEXTSKIN,
					NULL,
					NULL,
					pTBUF,
					dwLength,
					NULL,
					D3DX_FILTER_NONE,
					NULL,
					NULL);

				delete[] pTBUF;
			}

			file.Read( &dwLength, sizeof(DWORD));
			if(dwLength)
				file.Seek( dwLength, CFile::current);

			file.Read( &dwSHADOW, sizeof(DWORD));
			for( int j=0; j<dwSHADOW; j++)
			{
				file.Read( &dwLength, sizeof(DWORD));
				if(dwLength)
					file.Seek( dwLength, CFile::current);
			}

			if(vUNIT.IntersectRect( &vUNIT, &m_vFROM))
			{
				vUNIT.OffsetRect(
					m_vTO.x - m_vFROM.left,
					m_vTO.y - m_vFROM.top);
				ResetDETAIL( &mapINDEX, pEXTDETAIL, dwUNIT * dwUNIT);

				int nUnitZ = vUNIT.top / m_HugeMAP.m_nUnitLength;
				int nCountZ = vUNIT.bottom / m_HugeMAP.m_nUnitLength - nUnitZ + 1;

				for( j=0; j<nCountZ; j++)
				{
					int nUnitX = vUNIT.left / m_HugeMAP.m_nUnitLength;
					int nCountX = vUNIT.right / m_HugeMAP.m_nUnitLength - nUnitX + 1;

					for( int k=0; k<nCountX; k++)
					{
						CRect vTARGET(
							0, 0,
							m_HugeMAP.m_nUnitLength,
							m_HugeMAP.m_nUnitLength);

						vTARGET.OffsetRect(
							(nUnitX + k) * m_HugeMAP.m_nUnitLength,
							(nUnitZ + j) * m_HugeMAP.m_nUnitLength);

						if( nUnitX + k >= 0 && nUnitX + k < m_HugeMAP.m_nUnitCountX &&
							nUnitZ + j >= 0 && nUnitZ + j < m_HugeMAP.m_nUnitCountZ &&
							vTARGET.IntersectRect( &vTARGET, &vUNIT))
						{
							int nUnitID = (nUnitZ + j) * m_HugeMAP.m_nUnitCountX + nUnitX + k;
							int nWidth = vTARGET.Width();
							int nROW = vTARGET.Height();

							LPDIRECT3DSURFACE9 pSKIN = NULL;
							CRect vFROM(&vTARGET);

							vFROM.OffsetRect(
								m_vFROM.left - m_vTO.x - (i % nUnitCountX) * dwUNIT,
								m_vFROM.top - m_vTO.y - (i / nUnitCountX) * dwUNIT);

							vTARGET.OffsetRect(
								-(nUnitX + k) * m_HugeMAP.m_nUnitLength,
								-(nUnitZ + j) * m_HugeMAP.m_nUnitLength);

							m_HugeMAP.m_pENABLE[nUnitID] = TRUE;
							LoadUNIT(
								NULL,
								&pSKIN,
								NULL,
								NULL,
								pREGION,
								pDETAIL,
								pHEIGHT,
								nUnitID);

							D3DXLoadSurfaceFromSurface(
								pSKIN,
								NULL,
								&vTARGET,
								pEXTSKIN,
								NULL,
								&vFROM,
								D3DX_FILTER_NONE, 0);

							int nFromZ = vFROM.top + nROW;
							int nFromX = vFROM.left;

							int nToZ = vTARGET.top + nROW;
							int nToX = vTARGET.left;

							memcpy( pHEIGHT + nToZ * (m_HugeMAP.m_nUnitLength + 1) + nToX, pEXTHEIGHT + nFromZ * (dwUNIT + 1) + nFromX, (nWidth + 1) * sizeof(FLOAT));
							for( int l=0; l<nROW; l++)
							{
								nFromZ = vFROM.top + l;
								nToZ = vTARGET.top + l;

								memcpy( pHEIGHT + nToZ * (m_HugeMAP.m_nUnitLength + 1) + nToX, pEXTHEIGHT + nFromZ * (dwUNIT + 1) + nFromX, (nWidth + 1) * sizeof(FLOAT));
								memcpy( pREGION + nToZ * m_HugeMAP.m_nUnitLength + nToX, pEXTREGION + nFromZ * dwUNIT + nFromX, nWidth * sizeof(BYTE));
								memcpy( pDETAIL + nToZ * m_HugeMAP.m_nUnitLength + nToX, pEXTDETAIL + nFromZ * dwUNIT + nFromX, nWidth * sizeof(BYTE));
							}

							UpdateUNIT(
								NULL,
								pSKIN,
								NULL,
								NULL,
								pREGION,
								pDETAIL,
								pHEIGHT,
								nUnitID);

							if(pSKIN)
							{
								pSKIN->Release();
								pSKIN = NULL;
							}

							BuildTILETEX( nUnitX + k, nUnitZ + j);
							BuildSHADE( nUnitX + k, nUnitZ + j);
						}
					}
				}
			}

			if(pEXTSKIN)
			{
				pEXTSKIN->Release();
				pEXTSKIN = NULL;
			}
		}

		lParam->m_pWnd->m_bPercent = (BYTE) (i * 100 / dwCount);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	delete[] pEXTHEIGHT;
	delete[] pEXTREGION;
	delete[] pEXTDETAIL;
	delete[] pHEIGHT;
	delete[] pREGION;
	delete[] pDETAIL;

	mapINDEX.clear();
	mapTILE.clear();

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CEXTMAPItem::ResetATTR(LPBMPARAM lParam)
{
	lParam->m_pWnd->m_strStatus.LoadString(IDS_RESET_ATTRIBUTE);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	for( int i=0; i<m_HugeMAP.m_nUnitCountZ; i++)
		for( int j=0; j<m_HugeMAP.m_nUnitCountX; j++)
		{
			MAPMAPOBJ::iterator itOBJ;
			MAPMAPSFX::iterator itSFX;
			MAPMAPSND::iterator itSND;

			MAPDWORD mapOBJID;
			MAPDWORD mapSFXID;

			MAPMAPOBJ mapOBJ;
			MAPMAPSFX mapSFX;
			MAPMAPSND mapSND;

			mapOBJID.clear();
			mapSFXID.clear();

			mapOBJ.clear();
			mapSFX.clear();
			mapSND.clear();

			LoadOBJ( &mapOBJ, &mapSFX, &mapSND, &mapOBJID, &mapSFXID, (i * m_HugeMAP.m_nUnitCountX) + j);
			for( itOBJ = mapOBJ.begin(); itOBJ != mapOBJ.end(); itOBJ++)
			{
				MAPDWORD::iterator itID = mapOBJID.find((*itOBJ).first);

				if( itID != mapOBJID.end() )
				{
					COBJItem *pItem = GetOBJItem((*itID).second);

					if(pItem)
					{
						MAPATTRIBUTE::iterator itATTR = (*itOBJ).second->m_mapATTR.begin();

						while(itATTR != (*itOBJ).second->m_mapATTR.end())
						{
							MAPSTR::iterator itSTR = pItem->m_mapNAME.find((*itATTR).first);
							MAPATTRIBUTE::iterator itNEXT = itATTR;
							itNEXT++;

							if( itSTR == pItem->m_mapNAME.end() || (
								(*itSTR).second != CString("ID_HOUSE_MESH") &&
								(*itSTR).second != CString("ID_HOUSE_ID") &&
								(*itSTR).second != CString("ID_OBJ_TYPE") &&
								(*itSTR).second != CString("ID_MAP_ID") ))
							{
								delete (*itATTR).second;
								(*itOBJ).second->m_mapATTR.erase(itATTR);
							}

							itATTR = itNEXT;
						}
					}
				}
			}
			UpdateOBJ( &mapOBJ, &mapSFX, &mapSND, &mapOBJID, &mapSFXID, (i * m_HugeMAP.m_nUnitCountX) + j);

			for( itOBJ = mapOBJ.begin(); itOBJ != mapOBJ.end(); itOBJ++)
				if((*itOBJ).second)
					delete (*itOBJ).second;

			for( itSFX = mapSFX.begin(); itSFX != mapSFX.end(); itSFX++)
				if((*itSFX).second)
					delete (*itSFX).second;

			for( itSND = mapSND.begin(); itSND != mapSND.end(); itSND++)
				if((*itSND).second)
					delete (*itSND).second;

			mapOBJID.clear();
			mapSFXID.clear();

			mapOBJ.clear();
			mapSFX.clear();
			mapSND.clear();

			lParam->m_pWnd->m_bPercent = (BYTE) ((i * m_HugeMAP.m_nUnitCountX + j) * 100 / (m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ));
			lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
			lParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

DWORD CEXTMAPItem::__BuildSHADOWTEX( DWORD_PTR lParam)
{
	((CEXTMAPItem *) ((LPBMPARAM) lParam)->m_pHost)->BuildSHADOWTEX((LPBMPARAM) lParam);
	return 0;
}

DWORD CEXTMAPItem::__ExpOBJ( DWORD_PTR lParam)
{
	((CEXTMAPItem *) ((LPBMPARAM) lParam)->m_pHost)->ExpOBJ((LPBMPARAM) lParam);
	return 0;
}

DWORD CEXTMAPItem::__ImpOBJ( DWORD_PTR lParam)
{
	((CEXTMAPItem *) ((LPBMPARAM) lParam)->m_pHost)->ImpOBJ((LPBMPARAM) lParam);
	return 0;
}

DWORD CEXTMAPItem::__ExpGEO( DWORD_PTR lParam)
{
	((CEXTMAPItem *) ((LPBMPARAM) lParam)->m_pHost)->ExpGEO((LPBMPARAM) lParam);
	return 0;
}

DWORD CEXTMAPItem::__ImpGEO( DWORD_PTR lParam)
{
	((CEXTMAPItem *) ((LPBMPARAM) lParam)->m_pHost)->ImpGEO((LPBMPARAM) lParam);
	return 0;
}

DWORD CEXTMAPItem::__Build( DWORD_PTR lParam)
{
	((CEXTMAPItem *) ((LPBMPARAM) lParam)->m_pHost)->Build((LPBMPARAM) lParam);
	return 0;
}

DWORD CEXTMAPItem::__ResetATTR(DWORD_PTR lParam)
{	
	((CEXTMAPItem *) ((LPBMPARAM) lParam)->m_pHost)->ResetATTR((LPBMPARAM) lParam);
	return 0;
}

void CEXTMAPItem::Blend( LPDIRECT3DSURFACE9 pTARGET,
						 LPDIRECT3DSURFACE9 pIMG,
						 FLOAT fPosX,
						 FLOAT fPosZ)
{
	D3DXMATRIX vWORLD(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	LPDIRECT3DTEXTURE9 pIMGTEX = NULL;

	LPDIRECT3DSURFACE9 pTRENDER = NULL;
	LPDIRECT3DSURFACE9 pBACK = NULL;
	LPDIRECT3DSURFACE9 pZBUF = NULL;

	D3DSURFACE_DESC vDESC;
	CD3DCamera vCAMERA;
	pTARGET->GetDesc(&vDESC);

	D3DVIEWPORT9 vNEW = {
		0, 0,
		vDESC.Width,
		vDESC.Height,
		0.0f,
		1.0f};
	D3DVIEWPORT9 vOLD;

	vWORLD._41 = fPosX / vDESC.Width - 0.5f;
	vWORLD._43 = 0.5f - fPosZ / vDESC.Height;

	vCAMERA.InitOrthoCamera(
		CMainFrame::m_3DDevice.m_pDevice,
		-1.0f,
		1.0f,
		1.0f,
		1.0f);

	vCAMERA.SetPosition(
		D3DXVECTOR3( 0.0f, 0.5f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
		FALSE);
	vCAMERA.Activate(TRUE);

	CMainFrame::m_3DDevice.m_pDevice->CreateRenderTarget(
		vDESC.Width,
		vDESC.Height,
		D3DFMT_A8R8G8B8,
		D3DMULTISAMPLE_NONE,
		0, FALSE,
		&pTRENDER,
		NULL);

	D3DXLoadSurfaceFromSurface(
		pTRENDER,
		NULL,
		NULL,
		pTARGET,
		NULL,
		NULL,
		D3DX_FILTER_NONE,
		NULL);

	pIMG->GetDesc(&vDESC);
	D3DXCreateTexture(
		CMainFrame::m_3DDevice.m_pDevice,
		vDESC.Width,
		vDESC.Height,
		1, 0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		&pIMGTEX);

	pIMGTEX->GetSurfaceLevel( 0, &pBACK);
	D3DXLoadSurfaceFromSurface(
		pBACK,
		NULL,
		NULL,
		pIMG,
		NULL,
		NULL,
		D3DX_FILTER_NONE,
		NULL);
	pBACK->Release();

	CMainFrame::m_3DDevice.m_pDevice->GetDepthStencilSurface(&pZBUF);
	CMainFrame::m_3DDevice.m_pDevice->GetRenderTarget( 0, &pBACK);
	CMainFrame::m_3DDevice.m_pDevice->GetViewport(&vOLD);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pTRENDER);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(NULL);
	CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vNEW);
	CMainFrame::m_3DDevice.m_pDevice->BeginScene();

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, CEXTMAPView::m_pVB, 0, sizeof(PVERTEX));
	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);
	CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_PVERTEX);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState(
		D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_BLUE|
		D3DCOLORWRITEENABLE_GREEN|
		D3DCOLORWRITEENABLE_RED);

	CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, pIMGTEX);
	CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	CMainFrame::m_3DDevice.m_pDevice->EndScene();

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState(
		D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_ALPHA|
		D3DCOLORWRITEENABLE_BLUE|
		D3DCOLORWRITEENABLE_GREEN|
		D3DCOLORWRITEENABLE_RED);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pBACK);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(pZBUF);
	CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vOLD);

	GetRenderTargetData(
		pTRENDER,
		pTARGET);

	pTRENDER->Release();
	pIMGTEX->Release();
	pBACK->Release();
	pZBUF->Release();
}

void CEXTMAPItem::Blend( LPDIRECT3DSURFACE9 pTARGET,
						 LPDIRECT3DTEXTURE9 pIMG,
						 FLOAT fPosX,
						 FLOAT fPosZ)
{
	D3DXMATRIX vWORLD(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	LPDIRECT3DSURFACE9 pTRENDER = NULL;
	LPDIRECT3DSURFACE9 pBACK = NULL;
	LPDIRECT3DSURFACE9 pZBUF = NULL;

	D3DSURFACE_DESC vDESC;
	CD3DCamera vCAMERA;
	pTARGET->GetDesc(&vDESC);

	D3DVIEWPORT9 vNEW = {
		0, 0,
		vDESC.Width,
		vDESC.Height,
		0.0f,
		1.0f};
	D3DVIEWPORT9 vOLD;

	vWORLD._41 = fPosX / vDESC.Width - 0.5f;
	vWORLD._43 = 0.5f - fPosZ / vDESC.Height;

	vCAMERA.InitOrthoCamera(
		CMainFrame::m_3DDevice.m_pDevice,
		-1.0f,
		1.0f,
		1.0f,
		1.0f);

	vCAMERA.SetPosition(
		D3DXVECTOR3( 0.0f, 0.5f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
		FALSE);
	vCAMERA.Activate(TRUE);

	CMainFrame::m_3DDevice.m_pDevice->CreateRenderTarget(
		vDESC.Width,
		vDESC.Height,
		D3DFMT_A8R8G8B8,
		D3DMULTISAMPLE_NONE,
		0, FALSE,
		&pTRENDER,
		NULL);

	D3DXLoadSurfaceFromSurface(
		pTRENDER,
		NULL,
		NULL,
		pTARGET,
		NULL,
		NULL,
		D3DX_FILTER_NONE,
		NULL);

	CMainFrame::m_3DDevice.m_pDevice->GetDepthStencilSurface(&pZBUF);
	CMainFrame::m_3DDevice.m_pDevice->GetRenderTarget( 0, &pBACK);
	CMainFrame::m_3DDevice.m_pDevice->GetViewport(&vOLD);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pTRENDER);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(NULL);
	CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vNEW);
	CMainFrame::m_3DDevice.m_pDevice->BeginScene();

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, CEXTMAPView::m_pVB, 0, sizeof(PVERTEX));
	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);
	CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_PVERTEX);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState(
		D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_BLUE|
		D3DCOLORWRITEENABLE_GREEN|
		D3DCOLORWRITEENABLE_RED);

	CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, pIMG);
	CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	CMainFrame::m_3DDevice.m_pDevice->EndScene();

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState(
		D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_ALPHA|
		D3DCOLORWRITEENABLE_BLUE|
		D3DCOLORWRITEENABLE_GREEN|
		D3DCOLORWRITEENABLE_RED);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pBACK);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(pZBUF);
	CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vOLD);

	GetRenderTargetData(
		pTRENDER,
		pTARGET);

	pTRENDER->Release();
	pBACK->Release();
	pZBUF->Release();
}

void CEXTMAPItem::GetRenderTargetData( LPDIRECT3DSURFACE9 pTRENDER,
									   LPDIRECT3DSURFACE9 pTARGET)
{
	LPDIRECT3DSURFACE9 pTBUF = NULL;
	D3DSURFACE_DESC vDESC;
	pTRENDER->GetDesc(&vDESC);

	CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
		vDESC.Width,
		vDESC.Height,
		vDESC.Format,
		D3DPOOL_SYSTEMMEM,
		&pTBUF,
		NULL);

	CMainFrame::m_3DDevice.m_pDevice->GetRenderTargetData(
		pTRENDER,
		pTBUF);

	D3DXLoadSurfaceFromSurface(
		pTARGET,
		NULL,
		NULL,
		pTBUF,
		NULL,
		NULL,
		D3DX_FILTER_NONE,
		NULL);

	pTBUF->Release();
}

void CEXTMAPItem::BuildSREGION( LPVECTORSURFACE pSHADOW,
							    LPWORD pSREGION)
{
	int nWidth = m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nShadowPixel;
	int nCount = INT(pSHADOW->size());

	memset( pSREGION, 0, m_HugeMAP.m_nUnitLength * m_HugeMAP.m_nUnitLength * sizeof(WORD));
	nWidth = nWidth / SHADOWTEX_SIZE + (nWidth % SHADOWTEX_SIZE ? 1 : 0);

	for( int i=0; i<nCount; i++)
	{
		int nBlockX = (i % nWidth) * SHADOWTEX_SIZE / m_HugeMAP.m_nShadowPixel;
		int nBlockZ = (i / nWidth) * SHADOWTEX_SIZE / m_HugeMAP.m_nShadowPixel;

		LPDIRECT3DSURFACE9 pIMG = NULL;
		D3DSURFACE_DESC vDESC;
		D3DLOCKED_RECT lr;

		(*pSHADOW)[i]->GetDesc(&vDESC);
		CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
			vDESC.Width,
			vDESC.Height,
			D3DFMT_A8R8G8B8,
			D3DPOOL_SCRATCH,
			&pIMG,
			NULL);

		D3DXLoadSurfaceFromSurface(
			pIMG,
			NULL,
			NULL,
			(*pSHADOW)[i],
			NULL,
			NULL,
			D3DX_FILTER_NONE,
			NULL);

		pIMG->LockRect( &lr, NULL, 0);
		LPBYTE pDATA = (LPBYTE) lr.pBits;

		for( int j=0; j<SHADOWTEX_SIZE; j++)
		{
			int nTileZ = nBlockZ + (SHADOWTEX_SIZE - j - 1) / m_HugeMAP.m_nShadowPixel;
			int nResZ = (SHADOWTEX_SIZE - j - 1) % m_HugeMAP.m_nShadowPixel;

			LPDWORD pCOLOR = (LPDWORD) pDATA;

			for( int k=0; k<SHADOWTEX_SIZE; k++)
			{
				if(((*pCOLOR) & 0xFF000000))
				{
					int nTileX = nBlockX + k / m_HugeMAP.m_nShadowPixel;
					int nResX = k % m_HugeMAP.m_nShadowPixel;

					int nTileIndex = nTileZ * m_HugeMAP.m_nUnitLength + nTileX;
					int nResIndex = nResZ * m_HugeMAP.m_nShadowPixel + nResX;

					pSREGION[nTileIndex] |= (0x0001 << nResIndex);
				}

				pCOLOR++;
			}

			pDATA += lr.Pitch;
		}

		pIMG->UnlockRect();
		pIMG->Release();
	}
}

//////////////////////////////////////////////////////////////////
// CBSPMAPItem Function...
//////////////////////////////////////////////////////////////////


CBSPMAPItem::CBSPMAPItem()
:CMAPItem(ITEM_BSPMAP)
{
	m_bMAPType = MT_BSP;
	m_pMAP = &m_BSPMAP;
	m_bMenuID = 6;
	m_bBaseID = 0;

	m_mapRegionID.clear();
	m_vTEX.clear();

	m_strPATH.Empty();
	ClearDATA();
}

CBSPMAPItem::~CBSPMAPItem()
{
	MAPITEM::iterator finder = m_mapMAPItem.find(m_dwID);
	OnDelete();

	if( finder != m_mapMAPItem.end() )
		m_mapMAPItem.erase(finder);

	while(!m_vTEX.empty())
	{
		m_vTEX.back()->clear();
		delete m_vTEX.back();

		m_vTEX.pop_back();
	}

	if(!m_strPATH.IsEmpty())
		DeleteFile(m_strPATH);
	m_mapRegionID.clear();
}

void CBSPMAPItem::Load( CFile* pFile)
{
	CMAPItem::Load(pFile);
	DWORD dwLength;

	char szTempPath[_MAX_PATH];
	char szTempFile[_MAX_PATH];
	GetStudioTempPath(szTempPath);

	GetTempFileName(
		szTempPath,
		_T("_BS"),
		0, szTempFile);
	CFile file( szTempFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	pFile->Read( &dwLength, sizeof(DWORD));
	LPBYTE pDATA = new BYTE[dwLength];

	pFile->Read( pDATA, dwLength);
	file.Write( pDATA, dwLength);

	m_strPATH = file.GetFilePath();
	delete[] pDATA;
	file.Close();

	if(m_bMerge)
	{
		ClearDATA();
		SaveDATA();

		m_bBaseID = GetNextBaseID();
	}
	else
	{
		LoadDATA();
		m_bBaseID = BYTE((m_BSPMAP.m_Head.m_dwID >> 24) & 0x000000FF);
		ClearDATA();
	}
}

void CBSPMAPItem::Save( CFile* pFile)
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) GetMainDocument();
	if(pDoc)
		pDoc->UpdateData();
	CMAPItem::Save(pFile);

	CFile file( m_strPATH, CFile::modeRead|CFile::typeBinary);
	DWORD dwLength = (DWORD) file.GetLength();
	LPBYTE pDATA = new BYTE[dwLength];

	pFile->Write( &dwLength, sizeof(DWORD));
	file.Read( pDATA, dwLength);
	pFile->Write( pDATA, dwLength);
	delete[] pDATA;
}

void CBSPMAPItem::LoadDATA()
{
	if(m_strPATH.IsEmpty())
		return;

	CFile file( m_strPATH, CFile::modeRead|CFile::typeBinary);
	ClearDATA();
	int nCount;

	file.Read( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		LPMAPLIGHT pLIGHT = new MAPLIGHT();
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &pLIGHT->m_vDIR, sizeof(D3DXVECTOR3));
		file.Read( &pLIGHT->m_vPOS, sizeof(D3DXVECTOR3));
		file.Read( &pLIGHT->m_dwTextureID, sizeof(DWORD));
		file.Read( &pLIGHT->m_dwAmbient, sizeof(DWORD));
		file.Read( &pLIGHT->m_dwDiffuse, sizeof(DWORD));
		file.Read( &pLIGHT->m_fRange, sizeof(FLOAT));
		file.Read( &pLIGHT->m_fROT, sizeof(FLOAT));
		file.Read( &pLIGHT->m_bUseDL, sizeof(BYTE));
		file.Read( &pLIGHT->m_bUseLM, sizeof(BYTE));
		file.Read( &pLIGHT->m_bType, sizeof(BYTE));

		m_mapLIGHT.insert( MAPMAPLIGHT::value_type( dwID, pLIGHT));
	}

	file.Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPMAPOBJ pMAPOBJ = new MAPOBJ();

		DWORD dwItemID;
		DWORD dwID;

		int nATTR;
		int nCLK;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &dwItemID, sizeof(DWORD));

		m_mapOBJ.insert( MAPDWORD::value_type( dwID, dwItemID));
		file.Read( &pMAPOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
		file.Read( &pMAPOBJ->m_vPOS, sizeof(D3DXVECTOR3));
		file.Read( &pMAPOBJ->m_vROT, sizeof(D3DXVECTOR3));

		file.Read( &pMAPOBJ->m_dwOBJFuncID, sizeof(DWORD));
		file.Read( &pMAPOBJ->m_dwACTFuncID, sizeof(DWORD));
		file.Read( &pMAPOBJ->m_dwANIFuncID, sizeof(DWORD));

		file.Read( &pMAPOBJ->m_dwCurANI, sizeof(DWORD));
		file.Read( &pMAPOBJ->m_dwCurACT, sizeof(DWORD));
		file.Read( &nCLK, sizeof(int));

		for( int j=0; j<nCLK; j++)
		{
			LPCLKINST pCLK = new CLKINST();
			DWORD dwCLK;

			file.Read( &dwCLK, sizeof(DWORD));
			file.Read( &pCLK->m_dwCurMESH, sizeof(DWORD));
			file.Read( &pCLK->m_dwCurCL, sizeof(DWORD));

			pMAPOBJ->m_mapCLK.insert( MAPCLKINST::value_type( dwCLK, pCLK));
		}

		file.Read( &nATTR, sizeof(int));
		for( j=0; j<nATTR; j++)
		{
			LPATTRIBUTE pATTR = new ATTRIBUTE();
			DWORD dwATTR;

			file.Read( &dwATTR, sizeof(DWORD));
			file.Read( &pATTR->m_bValueType, sizeof(BYTE));
			file.Read( &pATTR->m_nSize, sizeof(int));
			pATTR->m_pValue = new BYTE[pATTR->m_nSize];
			file.Read( pATTR->m_pValue, pATTR->m_nSize);

			pMAPOBJ->m_mapATTR.insert( MAPATTRIBUTE::value_type( dwATTR, pATTR));
		}

		m_BSPMAP.m_mapOBJ.insert( MAPMAPOBJ::value_type( dwID, pMAPOBJ));
	}

	file.Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPMAPSFX pMAPSFX = new MAPSFX();

		DWORD dwItemID;
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &dwItemID, sizeof(DWORD));

		m_mapSFX.insert( MAPDWORD::value_type( dwID, dwItemID));
		file.Read( &pMAPSFX->m_vPOS, sizeof(D3DXVECTOR3));
		file.Read( &pMAPSFX->m_vROT, sizeof(D3DXVECTOR3));
		file.Read( &pMAPSFX->m_bBias, sizeof(BYTE));

		m_BSPMAP.m_mapSFX.insert( MAPMAPSFX::value_type( dwID, pMAPSFX));
	}

	file.Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPMAPSND pMAPSND = new MAPSND();
		DWORD dwID;

		file.Read( &dwID, sizeof(DWORD));
		file.Read( &pMAPSND->m_dwMediaID, sizeof(DWORD));
		file.Read( &pMAPSND->m_vPOS, sizeof(D3DXVECTOR3));

		m_BSPMAP.m_mapSND.insert( MAPMAPSND::value_type( dwID, pMAPSND));
	}

	file.Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPMAPDWORD pTEX = new MAPDWORD();
		int nTEXCount;
		file.Read( &nTEXCount, sizeof(int));

		for( int j=0; j<nTEXCount; j++)
		{
			DWORD dwIndex;
			DWORD dwTexID;

			file.Read( &dwIndex, sizeof(DWORD));
			file.Read( &dwTexID, sizeof(DWORD));

			pTEX->insert( MAPDWORD::value_type( dwIndex, dwTexID));
		}

		m_vTEX.push_back(pTEX);
	}

	file.Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		DWORD dwIndex;
		DWORD dwID;

		file.Read( &dwIndex, sizeof(DWORD));
		file.Read( &dwID, sizeof(DWORD));

		m_mapRegionID.insert( MAPDWORD::value_type( dwIndex, dwID));
	}

	file.Read( &m_dwPolyCNT, sizeof(DWORD));
	m_BSPMAP.Load(&file);
	m_BSPMAP.ResetLight();
	Regulate();
}

void CBSPMAPItem::SaveDATA()
{
	if(m_strPATH.IsEmpty())
	{
		char szTempPath[_MAX_PATH];
		char szTempFile[_MAX_PATH];
		GetStudioTempPath(szTempPath);

		GetTempFileName(
			szTempPath,
			_T("_BS"),
			0, szTempFile);
		m_strPATH = CString(szTempFile);
	}

	CFile file( m_strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	MAPMAPLIGHT::iterator itLIGHT;
	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;
	MAPDWORD::iterator itID;

	int nCount = INT(m_mapLIGHT.size());
	file.Write( &nCount, sizeof(int));

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
	{
		file.Write( &(*itLIGHT).first, sizeof(DWORD));

		file.Write( &(*itLIGHT).second->m_vDIR, sizeof(D3DXVECTOR3));
		file.Write( &(*itLIGHT).second->m_vPOS, sizeof(D3DXVECTOR3));
		file.Write( &(*itLIGHT).second->m_dwTextureID, sizeof(DWORD));
		file.Write( &(*itLIGHT).second->m_dwAmbient, sizeof(DWORD));
		file.Write( &(*itLIGHT).second->m_dwDiffuse, sizeof(DWORD));
		file.Write( &(*itLIGHT).second->m_fRange, sizeof(FLOAT));
		file.Write( &(*itLIGHT).second->m_fROT, sizeof(FLOAT));
		file.Write( &(*itLIGHT).second->m_bUseDL, sizeof(BYTE));
		file.Write( &(*itLIGHT).second->m_bUseLM, sizeof(BYTE));
		file.Write( &(*itLIGHT).second->m_bType, sizeof(BYTE));
	}

	nCount = INT(m_BSPMAP.m_mapOBJ.size());
	file.Write( &nCount, sizeof(int));

	for( itOBJ = m_BSPMAP.m_mapOBJ.begin(); itOBJ != m_BSPMAP.m_mapOBJ.end(); itOBJ++)
	{
		LPMAPOBJ pMAPOBJ = (*itOBJ).second;
		DWORD dwID = (*itOBJ).first;
		itID = m_mapOBJ.find(dwID);

		MAPATTRIBUTE::iterator itATTR;
		MAPCLKINST::iterator itCLK;
		DWORD dwItemID = (*itID).second;

		file.Write( &dwID, sizeof(DWORD));
		file.Write( &dwItemID, sizeof(DWORD));

		file.Write( &pMAPOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
		file.Write( &pMAPOBJ->m_vPOS, sizeof(D3DXVECTOR3));
		file.Write( &pMAPOBJ->m_vROT, sizeof(D3DXVECTOR3));

		file.Write( &pMAPOBJ->m_dwOBJFuncID, sizeof(DWORD));
		file.Write( &pMAPOBJ->m_dwACTFuncID, sizeof(DWORD));
		file.Write( &pMAPOBJ->m_dwANIFuncID, sizeof(DWORD));

		file.Write( &pMAPOBJ->m_dwCurANI, sizeof(DWORD));
		file.Write( &pMAPOBJ->m_dwCurACT, sizeof(DWORD));

		int nCLK = (INT) pMAPOBJ->m_mapCLK.size();
		file.Write( &nCLK, sizeof(int));

		for( itCLK = pMAPOBJ->m_mapCLK.begin(); itCLK != pMAPOBJ->m_mapCLK.end(); itCLK++)
		{
			LPCLKINST pCLK = (*itCLK).second;
			DWORD dwCLK = (*itCLK).first;

			file.Write( &dwCLK, sizeof(DWORD));
			file.Write( &pCLK->m_dwCurMESH, sizeof(DWORD));
			file.Write( &pCLK->m_dwCurCL, sizeof(DWORD));
		}

		int nATTR = (INT) pMAPOBJ->m_mapATTR.size();
		file.Write( &nATTR, sizeof(int));

		for( itATTR = pMAPOBJ->m_mapATTR.begin(); itATTR != pMAPOBJ->m_mapATTR.end(); itATTR++)
		{
			DWORD dwATTR = (*itATTR).first;

			file.Write( &dwATTR, sizeof(DWORD));
			file.Write( &(*itATTR).second->m_bValueType, sizeof(BYTE));
			file.Write( &(*itATTR).second->m_nSize, sizeof(int));
			file.Write( (*itATTR).second->m_pValue, (*itATTR).second->m_nSize);
		}
	}

	nCount = INT(m_BSPMAP.m_mapSFX.size());
	file.Write( &nCount, sizeof(int));

	for( itSFX = m_BSPMAP.m_mapSFX.begin(); itSFX != m_BSPMAP.m_mapSFX.end(); itSFX++)
	{
		LPMAPSFX pMAPSFX = (*itSFX).second;
		DWORD dwID = (*itSFX).first;
		itID = m_mapSFX.find(dwID);

		DWORD dwItemID = (*itID).second;

		file.Write( &dwID, sizeof(DWORD));
		file.Write( &dwItemID, sizeof(DWORD));

		file.Write( &pMAPSFX->m_vPOS, sizeof(D3DXVECTOR3));
		file.Write( &pMAPSFX->m_vROT, sizeof(D3DXVECTOR3));
		file.Write( &pMAPSFX->m_bBias, sizeof(BYTE));
	}

	nCount = INT(m_BSPMAP.m_mapSND.size());
	file.Write( &nCount, sizeof(int));

	for( itSND = m_BSPMAP.m_mapSND.begin(); itSND != m_BSPMAP.m_mapSND.end(); itSND++)
	{
		LPMAPSND pMAPSND = (*itSND).second;
		DWORD dwID = (*itSND).first;

		file.Write( &dwID, sizeof(DWORD));
		file.Write( &pMAPSND->m_dwMediaID, sizeof(DWORD));
		file.Write( &pMAPSND->m_vPOS, sizeof(D3DXVECTOR3));
	}

	nCount = INT(m_vTEX.size());
	file.Write( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		int nTEXCount = INT(m_vTEX[i]->size());
		file.Write( &nTEXCount, sizeof(int));

		for( itID = m_vTEX[i]->begin(); itID != m_vTEX[i]->end(); itID++)
		{
			file.Write( &(*itID).first, sizeof(DWORD));
			file.Write( &(*itID).second, sizeof(DWORD));
		}
	}

	nCount = INT(m_mapRegionID.size());
	file.Write( &nCount, sizeof(int));

	for( itID = m_mapRegionID.begin(); itID != m_mapRegionID.end(); itID++)
	{
		file.Write( &(*itID).first, sizeof(DWORD));
		file.Write( &(*itID).second, sizeof(DWORD));
	}

	file.Write( &m_dwPolyCNT, sizeof(DWORD));
	m_BSPMAP.Save(&file);
}

void CBSPMAPItem::ClearDATA()
{
	MAPMAPLIGHT::iterator itLIGHT;

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		delete (*itLIGHT).second;

	m_mapLIGHT.clear();
	ClearOBJ();
	ClearSFX();
	ClearSND();

	while(!m_vTEX.empty())
	{
		m_vTEX.back()->clear();
		delete m_vTEX.back();

		m_vTEX.pop_back();
	}

	m_mapRegionID.clear();
	m_BSPMAP.Clear();
}

void CBSPMAPItem::ClearLIGHT()
{
	MAPD3DLIGHT::iterator itD3DLIGHT;
	MAPMAPLIGHT::iterator itLIGHT;

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		delete (*itLIGHT).second;
	m_mapLIGHT.clear();

	for( itD3DLIGHT = m_BSPMAP.m_mapLIGHT.begin(); itD3DLIGHT != m_BSPMAP.m_mapLIGHT.end(); itD3DLIGHT++)
		delete (*itD3DLIGHT).second;
	m_BSPMAP.m_mapLIGHT.clear();

	if(m_BSPMAP.m_pLIGHT)
	{
		m_BSPMAP.m_pLIGHT->Release();
		m_BSPMAP.m_pLIGHT = NULL;
	}
	m_BSPMAP.m_Head.ClearLightMap();
}

void CBSPMAPItem::ClearTEX()
{
	while(!m_vTEX.empty())
	{
		m_vTEX.back()->clear();
		delete m_vTEX.back();

		m_vTEX.pop_back();
	}
	m_vTEX.push_back(new MAPDWORD());
}

void CBSPMAPItem::ClearOBJ()
{
	MAPMAPOBJ::iterator itOBJ;

	for( itOBJ = m_BSPMAP.m_mapOBJ.begin(); itOBJ != m_BSPMAP.m_mapOBJ.end(); itOBJ++)
		delete (*itOBJ).second;

	m_BSPMAP.m_mapOBJ.clear();
	m_mapOBJ.clear();
}

void CBSPMAPItem::ClearSFX()
{
	MAPMAPSFX::iterator itSFX;

	for( itSFX = m_BSPMAP.m_mapSFX.begin(); itSFX != m_BSPMAP.m_mapSFX.end(); itSFX++)
		delete (*itSFX).second;

	m_BSPMAP.m_mapSFX.clear();
	m_mapSFX.clear();
}

void CBSPMAPItem::ClearSND()
{
	MAPMAPSND::iterator itSND;

	for( itSND = m_BSPMAP.m_mapSND.begin(); itSND != m_BSPMAP.m_mapSND.end(); itSND++)
		delete (*itSND).second;
	m_BSPMAP.m_mapSND.clear();
}

void CBSPMAPItem::Build( CString strFileName)
{
	CProgressDlg Progress;
	BMPARAM Param;

	Param.m_strFileName = strFileName;
	Param.m_pWnd = &Progress;
	Param.m_pHost = this;

	Progress.m_strCancel.LoadString(IDS_CANCEL_BUILD_BSPMAP);
	Progress.m_strTitle.LoadString(IDS_BUILD_BSPMAP);
	Progress.m_pThreadFunc = (AFX_THREADPROC) __Build;
	Progress.m_lParam = &Param;
	Progress.DoModal();
}

void CBSPMAPItem::Build( LPBMPARAM lParam)
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) GetMainDocument();

	if(!pDoc)
		LoadDATA();

	CFile file( lParam->m_strFileName, CFile::modeRead|CFile::typeBinary);
	DWORD dwLength = (DWORD) file.GetLength();

	if( dwLength > 0 )
	{
		LPBYTE pDATA = new BYTE[dwLength];

		MAPMESHVB::iterator itMESH;
		MAPTEXVB::iterator itLIGHT;

		MAPTBSPNODE mapPVS;
		MAPTEXVB mapLIGHT;
		MAPMESHVB mapVB;

		file.Read( pDATA, dwLength);
		CTBSPNode::m_pProcLevel = (FNPROCLEVEL) __BuildLevel;
		CTBSPNode::m_pProcStep = (FNPROCSTEP) __BuildStep;
		((CBSPMAPItem *) lParam->m_pHost)->m_BSPMAP.LoadFromTMF( (LPARAM) lParam, pDATA);
		CTBSPNode::m_pProcLevel = NULL;
		CTBSPNode::m_pProcStep = NULL;

		mapLIGHT.clear();
		mapPVS.clear();
		mapVB.clear();

		m_BSPMAP.BuildPVS( &mapPVS, &mapLIGHT, &mapVB, NODEID_NULL);
		m_dwPolyCNT = 0;

		for( itMESH = mapVB.begin(); itMESH != mapVB.end(); itMESH++)
		{
			m_dwPolyCNT += (*itMESH).second->m_dwCount;
			delete (*itMESH).second;
		}

		for( itLIGHT = mapLIGHT.begin(); itLIGHT != mapLIGHT.end(); itLIGHT++)
			delete (*itLIGHT).second;

		mapLIGHT.clear();
		mapPVS.clear();
		mapVB.clear();

		delete[] pDATA;
	}

	CTBSPPortal::m_bBasePortalID = 0xFF;
	CTBSPNode::m_bBaseNodeID = 0xFF;

	if(m_vTEX.empty())
		m_vTEX.push_back(new MAPDWORD());
	SaveDATA();

	if(!pDoc)
		ClearDATA();

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

BYTE CBSPMAPItem::GetNextBaseID()
{
	MAPITEM::iterator it;
	MAPDWORD mapID;
	mapID.clear();

	for( it = m_mapMAPItem.begin(); it != m_mapMAPItem.end(); it++)
		if( (*it).second->m_bType == ITEM_BSPMAP )
		{
			CBSPMAPItem *pItem = (CBSPMAPItem *) (*it).second;
			mapID.insert( MAPDWORD::value_type( pItem->m_bBaseID, pItem->m_bBaseID));
		}

	BYTE bBaseID = 0;
	while( mapID.find(bBaseID) != mapID.end() )
		bBaseID++;
	mapID.clear();

	return bBaseID;
}

DWORD CBSPMAPItem::__BuildLevel( LPBMPARAM lParam, int nLevel, CString strTitle)
{
	lParam->m_pWnd->m_strStatus = strTitle;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	return 0;
}

DWORD CBSPMAPItem::__BuildStep( LPBMPARAM lParam, int nProcess)
{
	lParam->m_pWnd->m_bPercent = (BYTE) nProcess;
	lParam->m_pWnd->m_cProgressBar.SetPos(nProcess);
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	return 0;
}

DWORD CBSPMAPItem::__ImpOBJ( DWORD_PTR lParam)
{
	((CBSPMAPItem *) ((LPBMPARAM) lParam)->m_pHost)->ImpOBJ((LPBMPARAM) lParam);
	return 0;
}

DWORD CBSPMAPItem::__Build( DWORD_PTR lParam)
{
	((CBSPMAPItem *) ((LPBMPARAM) lParam)->m_pHost)->Build((LPBMPARAM) lParam);
	return 0;
}

void CBSPMAPItem::ExpBSP( CFile *pFile)
{
	MAPMAPLIGHT::iterator itLIGHT;
	MAPDWORD::iterator itID;
	LoadDATA();

	DWORD dwVER = __T3D_SDK_VER;
	pFile->Write( &dwVER, sizeof(DWORD));

	int nCount = INT(m_mapLIGHT.size());
	pFile->Write( &nCount, sizeof(int));

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
	{
		pFile->Write( &(*itLIGHT).first, sizeof(DWORD));
		pFile->Write( &(*itLIGHT).second->m_vDIR, sizeof(D3DXVECTOR3));
		pFile->Write( &(*itLIGHT).second->m_vPOS, sizeof(D3DXVECTOR3));
		pFile->Write( &(*itLIGHT).second->m_dwTextureID, sizeof(DWORD));
		pFile->Write( &(*itLIGHT).second->m_dwAmbient, sizeof(DWORD));
		pFile->Write( &(*itLIGHT).second->m_dwDiffuse, sizeof(DWORD));
		pFile->Write( &(*itLIGHT).second->m_fRange, sizeof(FLOAT));
		pFile->Write( &(*itLIGHT).second->m_fROT, sizeof(FLOAT));
		pFile->Write( &(*itLIGHT).second->m_bUseDL, sizeof(BYTE));
		pFile->Write( &(*itLIGHT).second->m_bUseLM, sizeof(BYTE));
		pFile->Write( &(*itLIGHT).second->m_bType, sizeof(BYTE));
	}

	nCount = INT(m_vTEX.size());
	pFile->Write( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		int nTEXCount = INT(m_vTEX[i]->size());
		pFile->Write( &nTEXCount, sizeof(int));

		for( itID = m_vTEX[i]->begin(); itID != m_vTEX[i]->end(); itID++)
		{
			pFile->Write( &(*itID).first, sizeof(DWORD));
			pFile->Write( &(*itID).second, sizeof(DWORD));
		}
	}

	nCount = INT(m_mapRegionID.size());
	pFile->Write( &nCount, sizeof(int));

	for( itID = m_mapRegionID.begin(); itID != m_mapRegionID.end(); itID++)
	{
		pFile->Write( &(*itID).first, sizeof(DWORD));
		pFile->Write( &(*itID).second, sizeof(DWORD));
	}

	pFile->Write( &m_dwPolyCNT, sizeof(DWORD));
	m_BSPMAP.Save(pFile);
	ClearDATA();
}

void CBSPMAPItem::ExpOBJ( CFile *pFile)
{
	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;
	MAPDWORD::iterator itID;
	LoadDATA();

	DWORD dwVER = __T3D_SDK_VER;
	pFile->Write( &dwVER, sizeof(DWORD));

	int nCount = INT(m_BSPMAP.m_mapOBJ.size());
	pFile->Write( &nCount, sizeof(int));

	for( itOBJ = m_BSPMAP.m_mapOBJ.begin(); itOBJ != m_BSPMAP.m_mapOBJ.end(); itOBJ++)
	{
		LPMAPOBJ pMAPOBJ = (*itOBJ).second;
		itID = m_mapOBJ.find((*itOBJ).first);

		MAPATTRIBUTE::iterator itATTR;
		MAPCLKINST::iterator itCLK;

		pFile->Write( &(*itID).second, sizeof(DWORD));
		pFile->Write( &pMAPOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
		pFile->Write( &pMAPOBJ->m_vPOS, sizeof(D3DXVECTOR3));
		pFile->Write( &pMAPOBJ->m_vROT, sizeof(D3DXVECTOR3));

		pFile->Write( &pMAPOBJ->m_dwOBJFuncID, sizeof(DWORD));
		pFile->Write( &pMAPOBJ->m_dwACTFuncID, sizeof(DWORD));
		pFile->Write( &pMAPOBJ->m_dwANIFuncID, sizeof(DWORD));

		pFile->Write( &pMAPOBJ->m_dwCurANI, sizeof(DWORD));
		pFile->Write( &pMAPOBJ->m_dwCurACT, sizeof(DWORD));

		int nCLK = (INT) pMAPOBJ->m_mapCLK.size();
		pFile->Write( &nCLK, sizeof(int));

		for( itCLK = pMAPOBJ->m_mapCLK.begin(); itCLK != pMAPOBJ->m_mapCLK.end(); itCLK++)
		{
			LPCLKINST pCLK = (*itCLK).second;
			DWORD dwCLK = (*itCLK).first;

			pFile->Write( &dwCLK, sizeof(DWORD));
			pFile->Write( &pCLK->m_dwCurMESH, sizeof(DWORD));
			pFile->Write( &pCLK->m_dwCurCL, sizeof(DWORD));
		}

		int nATTR = (INT) pMAPOBJ->m_mapATTR.size();
		pFile->Write( &nATTR, sizeof(int));

		for( itATTR = pMAPOBJ->m_mapATTR.begin(); itATTR != pMAPOBJ->m_mapATTR.end(); itATTR++)
		{
			DWORD dwATTR = (*itATTR).first;

			pFile->Write( &dwATTR, sizeof(DWORD));
			pFile->Write( &(*itATTR).second->m_bValueType, sizeof(BYTE));
			pFile->Write( &(*itATTR).second->m_nSize, sizeof(int));
			pFile->Write( (*itATTR).second->m_pValue, (*itATTR).second->m_nSize);
		}
	}

	nCount = INT(m_BSPMAP.m_mapSFX.size());
	pFile->Write( &nCount, sizeof(int));

	for( itSFX = m_BSPMAP.m_mapSFX.begin(); itSFX != m_BSPMAP.m_mapSFX.end(); itSFX++)
	{
		itID = m_mapSFX.find((*itSFX).first);

		pFile->Write( &(*itID).second, sizeof(DWORD));
		pFile->Write( &(*itSFX).second->m_vPOS, sizeof(D3DXVECTOR3));
		pFile->Write( &(*itSFX).second->m_vROT, sizeof(D3DXVECTOR3));
		pFile->Write( &(*itSFX).second->m_bBias, sizeof(BYTE));
	}

	nCount = INT(m_BSPMAP.m_mapSND.size());
	pFile->Write( &nCount, sizeof(int));

	for( itSND = m_BSPMAP.m_mapSND.begin(); itSND != m_BSPMAP.m_mapSND.end(); itSND++)
	{
		pFile->Write( &(*itSND).second->m_dwMediaID, sizeof(DWORD));
		pFile->Write( &(*itSND).second->m_vPOS, sizeof(D3DXVECTOR3));
	}

	ClearDATA();
}

void CBSPMAPItem::ImpBSP( CFile *pFile)
{
	MAPMAPLIGHT::iterator itLIGHT;
	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;

	DWORD dwVER;
	int nCount;

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		delete (*itLIGHT).second;

	for( itOBJ = m_BSPMAP.m_mapOBJ.begin(); itOBJ != m_BSPMAP.m_mapOBJ.end(); itOBJ++)
		delete (*itOBJ).second;

	for( itSFX = m_BSPMAP.m_mapSFX.begin(); itSFX != m_BSPMAP.m_mapSFX.end(); itSFX++)
		delete (*itSFX).second;

	for( itSND = m_BSPMAP.m_mapSND.begin(); itSND != m_BSPMAP.m_mapSND.end(); itSND++)
		delete (*itSND).second;

	while(!m_vTEX.empty())
	{
		m_vTEX.back()->clear();
		delete m_vTEX.back();

		m_vTEX.pop_back();
	}

	while(!m_vLocalID.empty())
		ReleaseLocalID( m_vLocalID[0], this);

	m_mapRegionID.clear();
	m_mapLIGHT.clear();
	m_mapOBJ.clear();
	m_mapSFX.clear();

	m_BSPMAP.m_mapOBJ.clear();
	m_BSPMAP.m_mapSFX.clear();
	m_BSPMAP.m_mapSND.clear();
	m_BSPMAP.Clear();

	CTBSPPortal::m_bBasePortalID = 0xFF;
	CTBSPNode::m_bBaseNodeID = 0xFF;
	ClearDATA();

	pFile->Read( &dwVER, sizeof(DWORD));
	pFile->Read( &nCount, sizeof(int));

	if( dwVER != __T3D_SDK_VER )
	{
		MessageBox( NULL, _T("Invalid SDK Version"), _T("Game Studio"), MB_OK);
		return;
	}

	for( int i=0; i<nCount; i++)
	{
		LPMAPLIGHT pLIGHT = new MAPLIGHT();
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		pFile->Read( &pLIGHT->m_vDIR, sizeof(D3DXVECTOR3));
		pFile->Read( &pLIGHT->m_vPOS, sizeof(D3DXVECTOR3));
		pFile->Read( &pLIGHT->m_dwTextureID, sizeof(DWORD));
		pFile->Read( &pLIGHT->m_dwAmbient, sizeof(DWORD));
		pFile->Read( &pLIGHT->m_dwDiffuse, sizeof(DWORD));
		pFile->Read( &pLIGHT->m_fRange, sizeof(FLOAT));
		pFile->Read( &pLIGHT->m_fROT, sizeof(FLOAT));
		pFile->Read( &pLIGHT->m_bUseDL, sizeof(BYTE));
		pFile->Read( &pLIGHT->m_bUseLM, sizeof(BYTE));
		pFile->Read( &pLIGHT->m_bType, sizeof(BYTE));

		m_mapLIGHT.insert( MAPMAPLIGHT::value_type( dwID, pLIGHT));
	}

	pFile->Read( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		LPMAPDWORD pTEX = new MAPDWORD();
		int nTEXCount;
		pFile->Read( &nTEXCount, sizeof(int));

		for( int j=0; j<nTEXCount; j++)
		{
			DWORD dwIndex;
			DWORD dwTexID;

			pFile->Read( &dwIndex, sizeof(DWORD));
			pFile->Read( &dwTexID, sizeof(DWORD));

			pTEX->insert( MAPDWORD::value_type( dwIndex, dwTexID));
		}

		m_vTEX.push_back(pTEX);
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		DWORD dwIndex;
		DWORD dwID;

		pFile->Read( &dwIndex, sizeof(DWORD));
		pFile->Read( &dwID, sizeof(DWORD));

		m_mapRegionID.insert( MAPDWORD::value_type( dwIndex, dwID));
	}

	pFile->Read( &m_dwPolyCNT, sizeof(DWORD));
	m_BSPMAP.Load(pFile);
	Regulate();

	BYTE bBaseID = BYTE((m_BSPMAP.m_Head.m_dwID >> 24) & 0x000000FF);
	BYTE bSAVE = TRUE;
	MAPITEM::iterator it;

	for( it = m_mapMAPItem.begin(); it != m_mapMAPItem.end(); it++)
		if( (*it).second != this && (*it).second->m_bType == ITEM_BSPMAP )
		{
			CBSPMAPItem *pItem = (CBSPMAPItem *) (*it).second;

			if( pItem->m_bBaseID == bBaseID )
			{
				MessageBox( NULL, _T("Duplicate Node base ID"), _T("Game Studio"), MB_OK);

				bSAVE = FALSE;
				break;
			}
		}

	if(bSAVE)
	{
		SaveDATA();
		m_bBaseID = bBaseID;
	}
	ClearDATA();
}

void CBSPMAPItem::ImpOBJ( LPBMPARAM lParam)
{
	CFile file( lParam->m_strFileName, CFile::modeRead|CFile::typeBinary);

	DWORD dwVER;
	int nCount;
	
	lParam->m_pWnd->m_strStatus.LoadString(IDS_IMP_OBJ);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	file.Read( &dwVER, sizeof(DWORD));
	file.Read( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		LPMAPOBJ pMAPOBJ = new MAPOBJ();
		DWORD dwItemID;
		DWORD dwID = 1;

		int nATTR;
		int nCLK;

		while( m_mapOBJ.find(dwID) != m_mapOBJ.end() )
			dwID++;

		file.Read( &dwItemID, sizeof(DWORD));
		m_mapOBJ.insert( MAPDWORD::value_type( dwID, dwItemID));

		file.Read( &pMAPOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
		file.Read( &pMAPOBJ->m_vPOS, sizeof(D3DXVECTOR3));
		file.Read( &pMAPOBJ->m_vROT, sizeof(D3DXVECTOR3));

		file.Read( &pMAPOBJ->m_dwOBJFuncID, sizeof(DWORD));
		file.Read( &pMAPOBJ->m_dwACTFuncID, sizeof(DWORD));
		file.Read( &pMAPOBJ->m_dwANIFuncID, sizeof(DWORD));

		file.Read( &pMAPOBJ->m_dwCurANI, sizeof(DWORD));
		file.Read( &pMAPOBJ->m_dwCurACT, sizeof(DWORD));
		file.Read( &nCLK, sizeof(int));

		for( int j=0; j<nCLK; j++)
		{
			LPCLKINST pCLK = new CLKINST();
			DWORD dwCLK;

			file.Read( &dwCLK, sizeof(DWORD));
			file.Read( &pCLK->m_dwCurMESH, sizeof(DWORD));
			file.Read( &pCLK->m_dwCurCL, sizeof(DWORD));

			pMAPOBJ->m_mapCLK.insert( MAPCLKINST::value_type( dwCLK, pCLK));
		}

		file.Read( &nATTR, sizeof(int));
		for( j=0; j<nATTR; j++)
		{
			LPATTRIBUTE pATTR = new ATTRIBUTE();
			DWORD dwATTR;

			file.Read( &dwATTR, sizeof(DWORD));
			file.Read( &pATTR->m_bValueType, sizeof(BYTE));
			file.Read( &pATTR->m_nSize, sizeof(int));
			pATTR->m_pValue = new BYTE[pATTR->m_nSize];
			file.Read( pATTR->m_pValue, pATTR->m_nSize);

			pMAPOBJ->m_mapATTR.insert( MAPATTRIBUTE::value_type( dwATTR, pATTR));
		}
		m_BSPMAP.m_mapOBJ.insert( MAPMAPOBJ::value_type( dwID, pMAPOBJ));

		lParam->m_pWnd->m_bPercent = (BYTE) (i * 100 / nCount);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	lParam->m_pWnd->m_strStatus.LoadString(IDS_IMP_SFX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	file.Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPMAPSFX pMAPSFX = new MAPSFX();

		DWORD dwItemID;
		DWORD dwID = 1;

		while( m_mapSFX.find(dwID) != m_mapSFX.end() )
			dwID++;

		file.Read( &dwItemID, sizeof(DWORD));
		m_mapSFX.insert( MAPDWORD::value_type( dwID, dwItemID));

		file.Read( &pMAPSFX->m_vPOS, sizeof(D3DXVECTOR3));
		file.Read( &pMAPSFX->m_vROT, sizeof(D3DXVECTOR3));
		file.Read( &pMAPSFX->m_bBias, sizeof(BYTE));
		m_BSPMAP.m_mapSFX.insert( MAPMAPSFX::value_type( dwID, pMAPSFX));

		lParam->m_pWnd->m_bPercent = (BYTE) (i * 100 / nCount);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	lParam->m_pWnd->m_strStatus.LoadString(IDS_IMP_SND);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	file.Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPMAPSND pMAPSND = new MAPSND();
		DWORD dwID = 1;

		while( m_BSPMAP.m_mapSND.find(dwID) != m_BSPMAP.m_mapSND.end() )
			dwID++;

		file.Read( &pMAPSND->m_dwMediaID, sizeof(DWORD));
		file.Read( &pMAPSND->m_vPOS, sizeof(D3DXVECTOR3));
		m_BSPMAP.m_mapSND.insert( MAPMAPSND::value_type( dwID, pMAPSND));

		lParam->m_pWnd->m_bPercent = (BYTE) (i * 100 / nCount);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CBSPMAPItem::Regulate()
{
	MAPMAPLIGHT::iterator itLIGHT;
	MAPDWORD::iterator itID;

	MAPTEXTURESET mapLightID;

	while(!m_BSPMAP.m_vTexture.empty())
	{
		delete[] m_BSPMAP.m_vTexture.back();
		m_BSPMAP.m_vTexture.pop_back();
	}

	if(m_BSPMAP.m_pRegionID)
		delete[] m_BSPMAP.m_pRegionID;
	mapLightID.clear();

	m_BSPMAP.m_pRegionID = new BYTE[m_BSPMAP.m_dwMeshCount];
	memset( m_BSPMAP.m_pRegionID, 0x00, m_BSPMAP.m_dwMeshCount * sizeof(BYTE));

	itID = m_mapRegionID.begin();
	while(itID != m_mapRegionID.end())
	{
		MAPDWORD::iterator itNEXT = itID;
		itNEXT++;

		if( (*itID).first < m_BSPMAP.m_dwMeshCount )
			m_BSPMAP.m_pRegionID[(*itID).first] = (BYTE) (*itID).second;
		else
			m_mapRegionID.erase(itID);

		itID = itNEXT;
	}

	for( int i=0; i<INT(m_vTEX.size()); i++)
	{
		LPTEXTURESET *pTEX = new LPTEXTURESET[m_BSPMAP.m_dwMeshCount];
		itID = m_vTEX[i]->begin();

		memset( pTEX, 0x00, m_BSPMAP.m_dwMeshCount * sizeof(LPTEXTURESET));
		m_BSPMAP.m_vTexture.push_back(pTEX);

		while(itID != m_vTEX[i]->end())
		{
			CTextureItem *pItem = GetTextureItem((*itID).second);

			MAPDWORD::iterator itNEXT = itID;
			itNEXT++;

			if( pItem && (*itID).first < m_BSPMAP.m_dwMeshCount )
				m_BSPMAP.m_vTexture[i][(*itID).first] = pItem->GetTEXTURESET();
			else
				m_vTEX[i]->erase(itID);

			itID = itNEXT;
		}
	}

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
	{
		CTextureItem *pTEX = GetTextureItem((*itLIGHT).second->m_dwTextureID);

		if(!pTEX)
			(*itLIGHT).second->m_dwTextureID = 0;

		mapLightID.insert( MAPTEXTURESET::value_type( (*itLIGHT).first, pTEX ? pTEX->GetTEXTURESET() : NULL));
	}

	MAPMAPOBJ::iterator itOBJ = m_BSPMAP.m_mapOBJ.begin();
	while(itOBJ != m_BSPMAP.m_mapOBJ.end())
	{
		MAPMAPOBJ::iterator itNEXT = itOBJ;
		itNEXT++;

		itID = m_mapOBJ.find((*itOBJ).first);
		if( itID != m_mapOBJ.end() )
		{
			COBJItem *pOBJ = GetOBJItem((*itID).second);

			if(!pOBJ)
			{
				delete (*itOBJ).second;
				m_BSPMAP.m_mapOBJ.erase(itOBJ);
				m_mapOBJ.erase(itID);
			}
			else
				(*itOBJ).second->m_pOBJ = &pOBJ->m_OBJ;
		}
		else
		{
			delete (*itOBJ).second;
			m_BSPMAP.m_mapOBJ.erase(itOBJ);
		}

		itOBJ = itNEXT;
	}

	MAPMAPSFX::iterator itSFX = m_BSPMAP.m_mapSFX.begin();
	while(itSFX != m_BSPMAP.m_mapSFX.end())
	{
		MAPMAPSFX::iterator itNEXT = itSFX;
		itNEXT++;

		itID = m_mapSFX.find((*itSFX).first);
		if( itID != m_mapSFX.end() )
		{
			CSFXItem *pSFX = GetSFXItem((*itID).second);

			if(!pSFX)
			{
				delete (*itSFX).second;
				m_BSPMAP.m_mapSFX.erase(itSFX);
				m_mapSFX.erase(itID);
			}
			else
				(*itSFX).second->m_pSFX = &pSFX->m_SFX;
		}
		else
		{
			delete (*itSFX).second;
			m_BSPMAP.m_mapSFX.erase(itSFX);
		}

		itSFX = itNEXT;
	}

	m_BSPMAP.m_Head.RegulateLight(&mapLightID);
	mapLightID.clear();
}


//////////////////////////////////////////////////////////////////
// CGuideItem Function...
//////////////////////////////////////////////////////////////////

CGuideItem::CGuideItem( BYTE bType)
:CItemBase(bType)
{
	m_vDropTarget.push_back(ITEM_PROJECT);
	m_vDropTarget.push_back(ITEM_FOLDER);

	m_nDropEffect = DROPEFFECT_MOVE;
	m_bPreviewID = TRUE;
	m_bNeedID = TRUE;
	m_bMenuID = 2;

	m_vGuideH.clear();
	m_vGuideV.clear();
}

CGuideItem::~CGuideItem()
{
	m_vGuideH.clear();
	m_vGuideV.clear();
}

void CGuideItem::Load( CFile* pFile)
{
	CItemBase::Load(pFile);
	int nCount = 0;

	pFile->Read( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		int nPOS;

		pFile->Read( &nPOS, sizeof(int));
		m_vGuideH.push_back(nPOS);
	}

	pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		int nPOS;

		pFile->Read( &nPOS, sizeof(int));
		m_vGuideV.push_back(nPOS);
	}
}

void CGuideItem::Save( CFile* pFile)
{
	CItemBase::Save(pFile);
	int nCount = INT(m_vGuideH.size());

	pFile->Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
		pFile->Write( &m_vGuideH[i], sizeof(int));

	nCount = INT(m_vGuideV.size());
	pFile->Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
		pFile->Write( &m_vGuideV[i], sizeof(int));
}


//////////////////////////////////////////////////////////////////
// CCOMPItem Function...
//////////////////////////////////////////////////////////////////

CCOMPItem::CCOMPItem()
:CGuideItem(ITEM_COMP)
{
	m_vCOMPITEM.clear();
	m_vCOMP.clear();

	m_pDefault = NULL;
	m_pDisable = NULL;

	m_dwDefaultID = 0;
	m_dwDisableID = 0;

	m_bCtrlType = CT_NONE;
	m_nHMargine = 0;
	m_nVMargine = 0;
	m_nLevel = 0;
	m_nWidth = 0;
	m_nHeight = 0;

	m_mapCompItem.insert( MAPITEM::value_type( m_dwID, this));
}

CCOMPItem::~CCOMPItem()
{
	MAPITEM::iterator finder = m_mapCompItem.find(m_dwID);
	OnDelete();

	if( finder != m_mapCompItem.end() )
		m_mapCompItem.erase(finder);

	while(!m_vCOMP.empty())
	{
		delete m_vCOMP.back();
		m_vCOMP.pop_back();
	}
	m_vCOMPITEM.clear();
}

void CCOMPItem::Load( CFile* pFile)
{
	MAPITEM::iterator finder = m_mapCompItem.find(m_dwID);
	if( finder != m_mapCompItem.end() )
		m_mapCompItem.erase(finder);
	CGuideItem::Load(pFile);
	m_mapCompItem.insert( MAPITEM::value_type( m_dwID, this));

	int nCount = 0;
	pFile->Read( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		LPCOMP pComp = new COMP;

		pFile->Read( &pComp->m_dwCompID, sizeof(DWORD));
		pFile->Read( &pComp->m_dwMediaID, sizeof(DWORD));
		pFile->Read( &pComp->m_bUseage, sizeof(BYTE));
		pFile->Read( &pComp->m_nPosX, sizeof(int));
		pFile->Read( &pComp->m_nPosY, sizeof(int));

		if( m_bMerge && pComp->m_dwCompID )
			pComp->m_dwCompID += m_dwCompID;

		m_vCOMP.push_back(pComp);
	}

	pFile->Read( &m_dwDefaultID, sizeof(DWORD));
	pFile->Read( &m_dwDisableID, sizeof(DWORD));
	pFile->Read( &m_bCtrlType, sizeof(BYTE));

	if( m_bMerge && m_dwDefaultID )
		m_dwDefaultID += m_dwImageID;

	if( m_bMerge && m_dwDisableID )
		m_dwDisableID += m_dwImageID;

	pFile->Read( &m_nHMargine, sizeof(int));
	pFile->Read( &m_nVMargine, sizeof(int));
	pFile->Read( &m_nLevel, sizeof(int));
	pFile->Read( &m_nWidth, sizeof(int));
	pFile->Read( &m_nHeight, sizeof(int));
}

void CCOMPItem::Save( CFile* pFile)
{
	CGuideItem::Save(pFile);
	int nCount = INT(m_vCOMP.size());

	pFile->Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		pFile->Write( &m_vCOMP[i]->m_dwCompID, sizeof(DWORD));
		pFile->Write( &m_vCOMP[i]->m_dwMediaID, sizeof(DWORD));
		pFile->Write( &m_vCOMP[i]->m_bUseage, sizeof(BYTE));
		pFile->Write( &m_vCOMP[i]->m_nPosX, sizeof(int));
		pFile->Write( &m_vCOMP[i]->m_nPosY, sizeof(int));
	}

	pFile->Write( &m_dwDefaultID, sizeof(DWORD));
	pFile->Write( &m_dwDisableID, sizeof(DWORD));
	pFile->Write( &m_bCtrlType, sizeof(BYTE));

	pFile->Write( &m_nHMargine, sizeof(int));
	pFile->Write( &m_nVMargine, sizeof(int));
	pFile->Write( &m_nLevel, sizeof(int));
	pFile->Write( &m_nWidth, sizeof(int));
	pFile->Write( &m_nHeight, sizeof(int));
}

void CCOMPItem::Regulate()
{
	VECTORCOMP::iterator it = m_vCOMP.begin();
	m_vCOMPITEM.clear();

	while(it != m_vCOMP.end())
	{
		CCOMPItem *pCOMP = GetCOMPItem((*it)->m_dwCompID);

		if(pCOMP)
		{
			m_vCOMPITEM.push_back(pCOMP);
			it++;
		}
		else
		{
			delete (*it);
			m_vCOMP.erase(it);
		}
	}

	m_pDefault = GetImageItem(m_dwDefaultID);
	m_pDisable = GetImageItem(m_dwDisableID);

	if(!m_pDefault)
		m_dwDefaultID = 0;

	if(!m_pDisable)
		m_dwDisableID = 0;
}

BOOL CCOMPItem::FindCOMPItem( DWORD dwID)
{
	if( dwID == m_dwID )
		return TRUE;

	int nCount = INT(m_vCOMPITEM.size());
	for( int i=0; i<nCount; i++)
		if(m_vCOMPITEM[i]->FindCOMPItem(dwID))
			return TRUE;

	return FALSE;
}

void CCOMPItem::Render( DWORD dwColor,
					    BOOL bCusColor,
						int nPosX,
						int nPosY)
{
	if(m_pDefault)
	{
		m_pDefault->m_ImageSet.m_dwCurTick = 0;
		CD3DImage *pIMG = m_pDefault->m_ImageSet.GetImage();

		pIMG->Render(
			CMainFrame::m_3DDevice.m_pDevice,
			bCusColor ? dwColor :
			m_pDefault->m_ImageSet.GetColor(),
			nPosX,
			nPosY);
	}
	else if(m_pDisable)
	{
		m_pDisable->m_ImageSet.m_dwCurTick = 0;
		CD3DImage *pIMG = m_pDisable->m_ImageSet.GetImage();

		pIMG->Render(
			CMainFrame::m_3DDevice.m_pDevice,
			bCusColor ? dwColor :
			m_pDisable->m_ImageSet.GetColor(),
			nPosX,
			nPosY);
	}

	int nCount = INT(m_vCOMP.size());
	for( int i=0; i<nCount; i++)
	{
		m_vCOMPITEM[i]->Render(
			dwColor,
			bCusColor,
			nPosX + m_vCOMP[i]->m_nPosX,
			nPosY + m_vCOMP[i]->m_nPosY);
	}
}

CSize CCOMPItem::GetSize()
{
	CSize size( 0, 0);

	if(m_pDefault)
		size = m_pDefault->m_ImageSet.GetSize();
	else if(m_pDisable)
		size = m_pDisable->m_ImageSet.GetSize();

	int nCount = INT(m_vCOMP.size());
	for( int i=0; i<nCount; i++)
	{
		CSize local = m_vCOMPITEM[i]->GetSize();

		local.cx += m_vCOMP[i]->m_nPosX;
		local.cy += m_vCOMP[i]->m_nPosY;

		size.cx = max( size.cx, local.cx);
		size.cy = max( size.cy, local.cy);
	}

	size.cx = max( size.cx, m_nWidth );
	size.cy = max( size.cy, m_nHeight );

	return size;
}

int CCOMPItem::HitTest( CPoint point)
{
	int nCount = INT(m_vCOMP.size());

	for( int i=0; i<nCount; i++)
	{
		CPoint pt = point;

		pt.x -= m_vCOMP[nCount - i - 1]->m_nPosX;
		pt.y -= m_vCOMP[nCount - i - 1]->m_nPosY;

		if( m_vCOMPITEM[nCount - i - 1]->HitTest(pt) != -1 )
			return nCount - i - 1;
	}

	if(m_pDefault)
	{
		CRect rect(
			0, 0,
			m_pDefault->m_ImageSet.m_vImage[0]->GetWidth(),
			m_pDefault->m_ImageSet.m_vImage[0]->GetHeight());

		if( rect.PtInRect(point) && m_pDefault->m_ImageSet.m_vImage[0]->GetMask( point.x, point.y) )
			return i;
	}

	if(m_pDisable)
	{
		CRect rect(
			0, 0,
			m_pDisable->m_ImageSet.m_vImage[0]->GetWidth(),
			m_pDisable->m_ImageSet.m_vImage[0]->GetHeight());

		if( rect.PtInRect(point) && m_pDisable->m_ImageSet.m_vImage[0]->GetMask( point.x, point.y) )
			return i;
	}

	{
		CRect rect(
			0, 0,
			this->m_nWidth,
			this->m_nHeight );

		if( rect.PtInRect(point) )
			return i;
	}

	return -1;
}


//////////////////////////////////////////////////////////////////
// CFRAMEItem Function...
//////////////////////////////////////////////////////////////////

CFRAMEItem::CFRAMEItem()
:CGuideItem(ITEM_FRAME)
{
	m_vCTRL.clear();
	m_vCOMP.clear();

	m_pDefault = NULL;
	m_pDisable = NULL;

	m_strFontID.Empty();
	m_dwDefaultID = 0;
	m_dwDisableID = 0;

	m_nWidth = 0;
	m_nHeight = 0;

	m_mapFrameItem.insert( MAPITEM::value_type( m_dwID, this));
}

CFRAMEItem::~CFRAMEItem()
{
	MAPITEM::iterator finder = m_mapFrameItem.find(m_dwID);
	OnDelete();

	if( finder != m_mapFrameItem.end() )
		m_mapFrameItem.erase(finder);

	while(!m_vCTRL.empty())
	{
		delete m_vCTRL.back();
		m_vCTRL.pop_back();
	}
	m_vCOMP.clear();
}

void CFRAMEItem::Load( CFile* pFile)
{
	MAPITEM::iterator finder = m_mapFrameItem.find(m_dwID);
	if( finder != m_mapFrameItem.end() )
		m_mapFrameItem.erase(finder);
	CGuideItem::Load(pFile);
	m_mapFrameItem.insert( MAPITEM::value_type( m_dwID, this));

	int nCount = 0;
	pFile->Read( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		LPCTRL pCtrl = new CTRL();

		for( int j=0; j<TCML_MENU_COUNT; j++)
			pCtrl->m_strMenuID[j] = LoadString(pFile);

		pCtrl->m_strCtrlID = LoadString(pFile);
		pCtrl->m_strFontID = LoadString(pFile);
		pCtrl->m_strTipID = LoadString(pFile);
		pCtrl->m_strToolTip = LoadString(pFile);
		pCtrl->m_strText = LoadString(pFile);

		if( m_bMerge && !pCtrl->m_strCtrlID.IsEmpty() )
			RegLocalID( pCtrl->m_strCtrlID, this);

		pFile->Read( &pCtrl->m_dwCompID, sizeof(DWORD));
		pFile->Read( &pCtrl->m_dwColor, sizeof(DWORD));
		pFile->Read( &pCtrl->m_dwFlag, sizeof(DWORD));
		pFile->Read( &pCtrl->m_bAlign, sizeof(BYTE));
		pFile->Read( &pCtrl->m_nPosX, sizeof(int));
		pFile->Read( &pCtrl->m_nPosY, sizeof(int));

		if( m_bMerge && pCtrl->m_dwCompID )
			pCtrl->m_dwCompID += m_dwCompID;

		m_vCTRL.push_back(pCtrl);
	}
	m_strFontID = LoadString(pFile);

	pFile->Read( &m_dwDefaultID, sizeof(DWORD));
	pFile->Read( &m_dwDisableID, sizeof(DWORD));

	if( m_bMerge && m_dwDefaultID )
		m_dwDefaultID += m_dwImageID;

	if( m_bMerge && m_dwDisableID )
		m_dwDisableID += m_dwImageID;

	pFile->Read( &m_nWidth, sizeof(int));
	pFile->Read( &m_nHeight, sizeof(int));
}

void CFRAMEItem::Save( CFile* pFile)
{
	CGuideItem::Save(pFile);
	int nCount = INT(m_vCOMP.size());

	pFile->Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		for( int j=0; j<TCML_MENU_COUNT; j++)
			WriteString( pFile, m_vCTRL[i]->m_strMenuID[j]);

		WriteString( pFile, m_vCTRL[i]->m_strCtrlID);
		WriteString( pFile, m_vCTRL[i]->m_strFontID);
		WriteString( pFile, m_vCTRL[i]->m_strTipID);
		WriteString( pFile, m_vCTRL[i]->m_strToolTip);
		WriteString( pFile, m_vCTRL[i]->m_strText);

		pFile->Write( &m_vCTRL[i]->m_dwCompID, sizeof(DWORD));
		pFile->Write( &m_vCTRL[i]->m_dwColor, sizeof(DWORD));
		pFile->Write( &m_vCTRL[i]->m_dwFlag, sizeof(DWORD));
		pFile->Write( &m_vCTRL[i]->m_bAlign, sizeof(BYTE));
		pFile->Write( &m_vCTRL[i]->m_nPosX, sizeof(int));
		pFile->Write( &m_vCTRL[i]->m_nPosY, sizeof(int));
	}

	WriteString( pFile, m_strFontID);
	pFile->Write( &m_dwDefaultID, sizeof(DWORD));
	pFile->Write( &m_dwDisableID, sizeof(DWORD));

	pFile->Write( &m_nWidth, sizeof(int));
	pFile->Write( &m_nHeight, sizeof(int));
}

void CFRAMEItem::Regulate()
{
	VECTORCTRL::iterator it = m_vCTRL.begin();
	m_vCOMP.clear();

	while(it != m_vCTRL.end())
	{
		CCOMPItem *pCOMP = GetCOMPItem((*it)->m_dwCompID);

		if(pCOMP)
		{
			for( int i=0; i<TCML_MENU_COUNT; i++)
			{
				MAPSTRID::iterator itMENU = m_mapGlobalID.find((*it)->m_strMenuID[i]);

				if( itMENU == m_mapGlobalID.end() )
					(*it)->m_strMenuID[i].Empty();
			}

			MAPSTRID::iterator finder = m_mapGlobalID.find((*it)->m_strFontID);
			if( finder == m_mapGlobalID.end() )
				(*it)->m_strFontID.Empty();

			finder = m_mapGlobalID.find((*it)->m_strTipID);
			if( finder == m_mapGlobalID.end() )
				(*it)->m_strTipID.Empty();

			m_vCOMP.push_back(pCOMP);
			it++;
		}
		else
		{
			ReleaseLocalID( (*it)->m_strCtrlID, this);

			delete (*it);
			m_vCTRL.erase(it);
		}
	}

	m_pDefault = GetImageItem(m_dwDefaultID);
	m_pDisable = GetImageItem(m_dwDisableID);

	if(!m_pDefault)
		m_dwDefaultID = 0;

	if(!m_pDisable)
		m_dwDisableID = 0;
}

void CFRAMEItem::Render( int nPosX, int nPosY)
{
	if(m_pDefault)
	{
		m_pDefault->m_ImageSet.m_dwCurTick = 0;
		CD3DImage *pIMG = m_pDefault->m_ImageSet.GetImage();

		pIMG->Render(
			CMainFrame::m_3DDevice.m_pDevice,
			m_pDefault->m_ImageSet.GetColor(),
			nPosX,
			nPosY);
	}
	else if(m_pDisable)
	{
		m_pDisable->m_ImageSet.m_dwCurTick = 0;
		CD3DImage *pIMG = m_pDisable->m_ImageSet.GetImage();

		pIMG->Render(
			CMainFrame::m_3DDevice.m_pDevice,
			m_pDisable->m_ImageSet.GetColor(),
			nPosX,
			nPosY);
	}

	int nCount = INT(m_vCOMP.size());
	for( int i=0; i<nCount; i++)
	{
		m_vCOMP[i]->Render(
			m_vCTRL[i]->m_dwColor,
			m_vCTRL[i]->m_dwFlag&TS_CUSTOM_COLOR ? TRUE : FALSE,
			nPosX + m_vCTRL[i]->m_nPosX,
			nPosY + m_vCTRL[i]->m_nPosY);

		if( !m_vCTRL[i]->m_strText.IsEmpty() &&
			!m_vCTRL[i]->m_strFontID.IsEmpty() )
		{
			MAPSTRID::iterator finder = m_mapGlobalID.find(m_vCTRL[i]->m_strFontID);

			if( finder != m_mapGlobalID.end() )
			{
				MAPGSFONT::iterator it = m_mapFont.find((*finder).second);
				CSize size = m_vCOMP[i]->GetSize();

				if( it != m_mapFont.end() )
				{
					UINT nFormat = DT_VCENTER|DT_SINGLELINE;

					LPGSFONT pFONT = (*it).second;
					CRect rect;

					switch(m_vCTRL[i]->m_bAlign)
					{
					case GSTA_LEFT		: nFormat |= DT_LEFT; break;
					case GSTA_CENTER	: nFormat |= DT_CENTER; break;
					case GSTA_RIGHT		: nFormat |= DT_RIGHT; break;
					}

					rect.SetRect(
						nPosX + m_vCTRL[i]->m_nPosX,
						nPosY + m_vCTRL[i]->m_nPosY,
						nPosX + m_vCTRL[i]->m_nPosX + size.cx,
						nPosY + m_vCTRL[i]->m_nPosY + size.cy);

					pFONT->m_D3DFONT.TextOut(
						CMainFrame::m_3DDevice.m_pDevice,
						m_vCTRL[i]->m_strText,
						rect,
						nFormat);
				}
			}
		}
	}
}

CSize CFRAMEItem::GetSize()
{
	CSize size( 0, 0);

	if(m_pDefault)
		size = m_pDefault->m_ImageSet.GetSize();
	else if(m_pDisable)
		size = m_pDefault->m_ImageSet.GetSize();

	int nCount = INT(m_vCOMP.size());
	for( int i=0; i<nCount; i++)
	{
		CSize local = m_vCOMP[i]->GetSize();

		local.cx += m_vCTRL[i]->m_nPosX;
		local.cy += m_vCTRL[i]->m_nPosY;

		size.cx = max( size.cx, local.cx);
		size.cy = max( size.cy, local.cy);
	}

	return size;
}

int CFRAMEItem::HitTest( CPoint point)
{
	int nCount = INT(m_vCTRL.size());

	for( int i=0; i<nCount; i++)
	{
		CPoint pt = point;

		pt.x -= m_vCTRL[nCount - i - 1]->m_nPosX;
		pt.y -= m_vCTRL[nCount - i - 1]->m_nPosY;

		if( m_vCOMP[nCount - i - 1]->HitTest(pt) != -1 )
			return nCount - i - 1;
	}

	return -1;
}

void CEXTMAPItem::SaveHeight( LPBMPARAM lParam )
{
	lParam->m_pWnd->m_strStatus.LoadString( IDS_SAVE_HEIGHTMAP );
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	for( DWORD i = 0 ; i < m_HugeMAP.m_nUnitCountZ ; i++ )
		for( DWORD j = 0 ; j < m_HugeMAP.m_nUnitCountX ; j++ )
		{
			SaveHeight( j, i, lParam->m_strFileName );

			lParam->m_pWnd->m_bPercent = ( BYTE )( ( i * m_HugeMAP.m_nUnitCountX + j ) * 100 / ( m_HugeMAP.m_nUnitCountX * m_HugeMAP.m_nUnitCountZ ) );
			lParam->m_pWnd->m_cProgressBar.SetPos( lParam->m_pWnd->m_bPercent );
			lParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}
}

// 맵의 Height 값 읽어들이기 -02/02/08- 박상연
void CEXTMAPItem::SaveHeight(int nPosX, int nPosZ, CString strFileName)
{
	// Height Map Data 읽어들이기
	LPDIRECT3DSURFACE9	pIMG = NULL;
	D3DLOCKED_RECT lr;

	FLOAT *pHEIGHT = new FLOAT[ ( m_HugeMAP.m_nUnitLength + 1 ) * ( m_HugeMAP.m_nUnitLength + 1 ) ];
	LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, pHEIGHT, nPosZ * m_HugeMAP.m_nUnitCountX + nPosX);

	CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
		m_HugeMAP.m_nUnitLength + 1,
		m_HugeMAP.m_nUnitLength + 1,
		D3DFMT_A8R8G8B8,
		D3DPOOL_SCRATCH,
		&pIMG,
		NULL);

	pIMG->LockRect( &lr, NULL, 0);
	LPBYTE pDATA = (LPBYTE) lr.pBits;
	LPDWORD pCLR = NULL;

	FLOAT fMaxH = m_fMaxHeight - m_fMinHeight;

	for( int i = m_HugeMAP.m_nUnitLength ; i >= 0 ; i--)
	{
		pCLR = (LPDWORD) pDATA;

		for( int j=0; j<m_HugeMAP.m_nUnitLength + 1; j++)
		{
			FLOAT fHeight = pHEIGHT[i * (m_HugeMAP.m_nUnitLength + 1) + j] - m_fMinHeight;
			fHeight = max( min(fHeight,fMaxH), 0.0f );
			
			pCLR[j] = VALUEARGB(
				0x00,
				(DWORD)fHeight,
				(DWORD)fHeight,
				(DWORD)fHeight);
			
		}
		pDATA += lr.Pitch;
		
	}
	pIMG->UnlockRect();

	D3DXSaveSurfaceToFile(
		strFileName,
		D3DXIFF_BMP,
		pIMG,
		NULL,
		NULL);

	pIMG->Release();
	delete[] pHEIGHT;
}

void CItemBase::CopyItem(CItemBase* pSrcItem)
{
}

void CFRAMEItem::CopyItem(CItemBase* pSrcItem)
{
	CFRAMEItem *pSrc;
	pSrc = (CFRAMEItem *)pSrcItem;
	VECTORCOMPITEM::iterator itCOMP;
	VECTORCTRL::iterator itCTRL;

	for( itCOMP = (*pSrc).m_vCOMP.begin() ; itCOMP != (*pSrc).m_vCOMP.end() ; itCOMP++ )
	{
		CCOMPItem *pComp = new CCOMPItem();
		pComp = (*itCOMP);
		m_vCOMP.push_back( pComp );
	}

	for( itCTRL = (*pSrc).m_vCTRL.begin() ; itCTRL != (*pSrc).m_vCTRL.end() ; itCTRL++ )
	{
		LPCTRL pCtrl = new CTRL();
		*pCtrl = *(*itCTRL);
		m_vCTRL.push_back( pCtrl );
	}
	m_pDefault = (*pSrc).m_pDefault;
	m_pDisable = (*pSrc).m_pDisable;
	m_strFontID = (*pSrc).m_strFontID;
	m_dwDefaultID = (*pSrc).m_dwDefaultID;
	m_dwDisableID = (*pSrc).m_dwDisableID;
	m_nWidth = (*pSrc).m_nWidth;
	m_nHeight = (*pSrc).m_nHeight;
}

void CSFXItem::CopyItem(CItemBase* pSrcItem)
{
	CSFXItem *pSrc;
	pSrc = ( CSFXItem* )pSrcItem;

	VECTORDWORD::iterator itSFXID;

	for( itSFXID = (*pSrc).m_vSFXID.begin() ; itSFXID != (*pSrc).m_vSFXID.end() ; itSFXID++ )
	{
		DWORD *dwSFXID = new DWORD();
		*dwSFXID = (*itSFXID);
		m_vSFXID.push_back( *dwSFXID );
	}
	
	m_SFX = (*pSrc).m_SFX;

	m_TSFX = (*pSrc).m_TSFX;

	m_SFX.m_vKey.clear();
	m_SFX.m_vSFX.clear();

	m_TSFX.m_vCSFX.clear();
	m_TSFX.m_vPSFX.clear();

	m_dwTotalFrame = (*pSrc).m_dwTotalFrame;
	m_dwSANISRC = (*pSrc).m_dwSANISRC;
	m_dwAnimationID = (*pSrc).m_dwAnimationID;
	m_dwANISRC = (*pSrc).m_dwANISRC;
	m_dwSFXSRC = (*pSrc).m_dwSFXSRC;
	m_dwCurCLK = (*pSrc).m_dwCurCLK;

	if( m_dwTotalFrame )
	{
		size_t size = (*pSrc).m_SFX.m_vKey.size();
		for( int i=0; i < size; ++i )
		{
			int nIndex = m_SFX.FindKey( (*pSrc).m_SFX.m_vKey[i]->m_dwTick );
			if( nIndex == -1 )
			{
				LPCLRKEY pKey = new CLRKEY();

				pKey->m_dwColor = (*pSrc).m_SFX.m_vKey[i]->m_dwColor;
				pKey->m_dwTick = (*pSrc).m_SFX.m_vKey[i]->m_dwTick;

				m_SFX.InsertKey( pKey );
			}
		}
	}
}

DWORD CEXTMAPItem::__AdjustOBJPOS(DWORD_PTR lParam)
{
	((CEXTMAPItem *) ((LPADOBPARAM) lParam)->m_pHost)->AdjustOBJPOS((LPADOBPARAM) lParam);
	return 0;
}

void CEXTMAPItem::AdjustOBJPOS(LPADOBPARAM lParam)
{
	lParam->m_pWnd->m_strStatus.Format(IDS_PROGRESS_UNIT, 0, 0);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	MAPMAPOBJ mapOBJ;
	MAPMAPSFX mapSFX;
	MAPMAPSND mapSND;

	MAPDWORD mapOBJID;
	MAPDWORD mapSFXID;

	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;
	MAPDWORD::iterator itOBJID;

	MAPDWORD::iterator itSFXID;

	CEXTMAPDoc* pDoc = (CEXTMAPDoc*)GetMainDocument();
	
	if( lParam->m_dwUnitPOS == -1 )
	{
		for( DWORD i = 0 ; i < m_HugeMAP.m_nUnitCountZ ; i++)
		{
			for( DWORD j = 0 ; j < m_HugeMAP.m_nUnitCountX ; j++)
			{
				lParam->m_pWnd->m_strStatus.Format(IDS_PROGRESS_UNIT, j * m_HugeMAP.m_nUnitLength, i * m_HugeMAP.m_nUnitLength);
				lParam->m_pWnd->m_cProgressBar.SetPos(0);
				lParam->m_pWnd->m_bPercent = 0;
				lParam->m_pWnd->SendMessage(
					WM_COMMAND,
					ID_PROGRESS_UPDATE);

				mapOBJ.clear();
				mapSFX.clear();
				mapSND.clear();

				mapOBJID.clear();
				mapSFXID.clear();

				LoadOBJ(
				&mapOBJ,
				&mapSFX,
				&mapSND,
				&mapOBJID,
				&mapSFXID, ( i * m_HugeMAP.m_nUnitCountX ) + j );
				if( lParam->m_bAttachType )
					pDoc->ObjectAttach( &mapOBJ, &mapOBJID, lParam->m_pOBJ, lParam->m_dwBASEOBJID, &lParam->m_dwUnitPOS, lParam->m_vMovePos );
				else
					pDoc->ObjectReplace( &mapOBJ, &mapOBJID, lParam->m_pOBJ, lParam->m_dwBASEOBJID, &lParam->m_dwUnitPOS, lParam->m_vMovePos );

				
				UpdateOBJ( 
				&mapOBJ,
				&mapSFX,
				&mapSND,
				&mapOBJID,
				&mapSFXID, ( i * m_HugeMAP.m_nUnitCountX ) + j );

                for( itOBJ = mapOBJ.begin(); itOBJ != mapOBJ.end(); itOBJ++)
				if((*itOBJ).second)
					delete (*itOBJ).second;

				for( itSFX = mapSFX.begin(); itSFX != mapSFX.end(); itSFX++)
					if((*itSFX).second)
						delete (*itSFX).second;

				for( itSND = mapSND.begin(); itSND != mapSND.end(); itSND++)
					if((*itSND).second)
						delete (*itSND).second;

				mapOBJ.clear();
				mapSFX.clear();
				mapSND.clear();

				mapOBJID.clear();
				mapSFXID.clear();
			}
		}
	}
	else
	{
		lParam->m_pWnd->m_strStatus.Format( IDS_PROGRESS_UNIT, IDS_PROGRESS_UNIT, lParam->m_dwUnitPOS / m_HugeMAP.m_nUnitCountX,lParam->m_dwUnitPOS % m_HugeMAP.m_nUnitCountX);
		lParam->m_pWnd->m_cProgressBar.SetPos(0);
		lParam->m_pWnd->m_bPercent = 0;
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);

		mapOBJ.clear();
		mapSFX.clear();
		mapSND.clear();

		mapOBJID.clear();
		mapSFXID.clear();

		LoadOBJ(
		&mapOBJ,
		&mapSFX,
		&mapSND,
		&mapOBJID,
		&mapSFXID, lParam->m_dwUnitPOS );

		if( lParam->m_bAttachType )
			pDoc->ObjectAttach( &mapOBJ, &mapOBJID, lParam->m_pOBJ, lParam->m_dwBASEOBJID, &lParam->m_dwUnitPOS, lParam->m_vMovePos );
		else
			pDoc->ObjectReplace( &mapOBJ, &mapOBJID, lParam->m_pOBJ, lParam->m_dwBASEOBJID, &lParam->m_dwUnitPOS, lParam->m_vMovePos );

		
		UpdateOBJ( 
		&mapOBJ,
		&mapSFX,
		&mapSND,
		&mapOBJID,
		&mapSFXID, lParam->m_dwUnitPOS);

        for( itOBJ = mapOBJ.begin(); itOBJ != mapOBJ.end(); itOBJ++)
		if((*itOBJ).second)
			delete (*itOBJ).second;

		for( itSFX = mapSFX.begin(); itSFX != mapSFX.end(); itSFX++)
			if((*itSFX).second)
				delete (*itSFX).second;

		for( itSND = mapSND.begin(); itSND != mapSND.end(); itSND++)
			if((*itSND).second)
				delete (*itSND).second;

		mapOBJ.clear();
		mapSFX.clear();
		mapSND.clear();

		mapOBJID.clear();
		mapSFXID.clear();
	}

	lParam->m_pWnd->m_bPercent = 100;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CAnimationItem::ReleaseAnimation()
{
	if(m_pVB)
	{
		delete[] m_pVB;
		m_pVB = NULL;
	}

	m_vAnimation.Release();
}

void CAnimationItem::LoadAnimation(LPDIRECT3DDEVICE9 pDevice, LPBYTE pData)
{
	int nTAFVersion = 0;

	nTAFVersion = *((int *) pData); pData += sizeof(int);
	ReleaseAnimation();

	DWORD dwNodeCount = 0;
	switch(nTAFVersion)
	{
	case 1		:
		{
			dwNodeCount = *((LPDWORD) pData);
			m_vAnimation.SetNodeCount( dwNodeCount );
			pData += sizeof(DWORD);
			m_vAnimation.m_dwStartTick = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_vAnimation.m_dwEndTick = *((LPDWORD) pData); pData += sizeof(DWORD);

			if( dwNodeCount > 0 )
			{
				m_vAnimation.m_pBones = new D3DXMATRIX[dwNodeCount];
				m_vAnimation.m_pTREE = new BONESNODE[dwNodeCount];
				m_vAnimation.m_pNode = new BONESANIMATION[dwNodeCount];

				m_vAnimation.m_pDATA = new VBDATA[dwNodeCount];
				m_vAnimation.m_pVB = new MESHVB[dwNodeCount];
				m_pVB = new VBDATA[dwNodeCount];

				memcpy( m_vAnimation.m_pBones, pData, sizeof(D3DXMATRIX) * dwNodeCount);
				pData += sizeof(D3DXMATRIX) * dwNodeCount;

				for( DWORD i=0; i<dwNodeCount; i++)
				{
					m_vAnimation.m_pDATA[i].m_dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
					m_vAnimation.m_pDATA[i].m_dwFVF = D3DFVF_XYZ;
					m_vAnimation.m_pVB[i].m_dwCount = 0;

					m_pVB[i].m_dwCount = m_vAnimation.m_pDATA[i].m_dwCount;
					m_pVB[i].m_dwFVF = T3DFVF_MESHVERTEX;

					if(m_pVB[i].m_dwCount)
					{
						LPWMESHVERTEX pSRC = (LPWMESHVERTEX) pData;
						LPMESHVERTEX pDEST = NULL;

						m_vAnimation.m_pDATA[i].m_pVB = new BYTE[m_pVB[i].m_dwCount * sizeof(D3DXVECTOR3)];
						m_pVB[i].m_pVB = new BYTE[m_pVB[i].m_dwCount * sizeof(MESHVERTEX)];
						pDEST = (LPMESHVERTEX) m_pVB[i].m_pVB;

						for( DWORD j=0; j<m_pVB[i].m_dwCount; j++)
						{
							((LPD3DXVECTOR3) m_vAnimation.m_pDATA[i].m_pVB)[j] = D3DXVECTOR3(
								pSRC[j].m_fPosX,
								pSRC[j].m_fPosY,
								pSRC[j].m_fPosZ);

							pDEST[j].m_fNormalX = pSRC[j].m_fNormalX;
							pDEST[j].m_fNormalY = pSRC[j].m_fNormalY;
							pDEST[j].m_fNormalZ = pSRC[j].m_fNormalZ;
							pDEST[j].m_fPosX = pSRC[j].m_fPosX;
							pDEST[j].m_fPosY = pSRC[j].m_fPosY;
							pDEST[j].m_fPosZ = pSRC[j].m_fPosZ;
							pDEST[j].m_fU1 = pSRC[j].m_fU;
							pDEST[j].m_fV1 = pSRC[j].m_fV;
							pDEST[j].m_fU2 = pSRC[j].m_fU;
							pDEST[j].m_fV2 = pSRC[j].m_fV;
						}

						pData += m_pVB[i].m_dwCount * sizeof(WMESHVERTEX);
						m_vAnimation.m_pDATA[i].m_dwCount /= 3;
						m_pVB[i].m_dwCount /= 3;
					}
				}
				pData += sizeof(BYTE);

				for( i=0; i<dwNodeCount; i++)
				{
					m_vAnimation.m_pTREE[i].m_bINDEX = *((LPBYTE) pData); pData += sizeof(BYTE);
					m_vAnimation.m_pTREE[i].m_bParentROT = *((LPBYTE) pData); pData += sizeof(BYTE);
					m_vAnimation.m_pTREE[i].m_bParentPOS = *((LPBYTE) pData); pData += sizeof(BYTE);
				}

				for( i=0; i<dwNodeCount; i++)
				{
					int nNODE = 0;

					for( int j=0; j<dwNodeCount; j++)
						if( m_vAnimation.m_pTREE[j].m_bINDEX == i )
							nNODE = j;

					m_vAnimation.m_pNode[nNODE].m_nPositionKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_vAnimation.m_pNode[nNODE].m_nPositionKeyCount > 0 )
					{
						LPTPOINTKEY pTKEY = new TPOINTKEY[m_vAnimation.m_pNode[nNODE].m_nPositionKeyCount];
						BYTE bSIZE[3][2] = {
							{ 36, 24},
							{ 40, 28},
							{ 16,  4}};
						BYTE bTYPE = *((LPBYTE) pData);

						m_vAnimation.m_pNode[nNODE].m_pPositionKey = pTKEY;
						pData += sizeof(BYTE);

						for( j=0; j<m_vAnimation.m_pNode[nNODE].m_nPositionKeyCount; j++)
						{
							pTKEY[j].m_vKeyPoint = *((LPD3DXVECTOR3) &pData[bSIZE[bTYPE - 1][1]]);
							pTKEY[j].m_dwTime = *((LPDWORD) &pData[0]);
							pData += bSIZE[bTYPE - 1][0];
						}
					}

					m_vAnimation.m_pNode[nNODE].m_nRotationKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_vAnimation.m_pNode[nNODE].m_nRotationKeyCount > 0 )
					{
						LPTROTKEY pTKEY = new TROTKEY[m_vAnimation.m_pNode[nNODE].m_nRotationKeyCount];
						BYTE bSIZE[3][2] = {
							{ 40, 24},
							{ 52,  4},
							{ 20,  4}};
						BYTE bTYPE = *((LPBYTE) pData);

						m_vAnimation.m_pNode[nNODE].m_pRotationKey = pTKEY;
						pData += sizeof(BYTE);

						for( j=0; j<m_vAnimation.m_pNode[nNODE].m_nRotationKeyCount; j++)
						{
							if( bTYPE == 1 )
							{
								D3DXQuaternionRotationAxis(
									&pTKEY[j].m_vKeyQuat,
									(LPD3DXVECTOR3) &pData[bSIZE[bTYPE - 1][1]],
									*((FLOAT *) &pData[bSIZE[bTYPE - 1][1] + sizeof(D3DXVECTOR3)]));
							}
							else
								pTKEY[j].m_vKeyQuat = *((LPD3DXQUATERNION) &pData[bSIZE[bTYPE - 1][1]]);

							pTKEY[j].m_dwTime = *((LPDWORD) &pData[0]);
							pData += bSIZE[bTYPE - 1][0];
						}

						if( bTYPE != 1 )
							for( j=m_vAnimation.m_pNode[nNODE].m_nRotationKeyCount - 1; j>0; j--)
							{
								D3DXQUATERNION vINV;

								D3DXQuaternionInverse(
									&vINV,
									&pTKEY[j - 1].m_vKeyQuat);

								pTKEY[j].m_vKeyQuat = vINV * pTKEY[j].m_vKeyQuat;
							}
					}

					m_vAnimation.m_pNode[nNODE].m_nScaleKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_vAnimation.m_pNode[nNODE].m_nScaleKeyCount > 0 )
					{
						LPTSCALEKEY pTKEY = new TSCALEKEY[m_vAnimation.m_pNode[nNODE].m_nScaleKeyCount];
						BYTE bSIZE[3][2] = {
							{ 52, 40},
							{ 56, 44},
							{ 32, 20}};
						BYTE bTYPE = *((LPBYTE) pData);

						m_vAnimation.m_pNode[nNODE].m_pScaleKey = pTKEY;
						pData += sizeof(BYTE);

						for( j=0; j<m_vAnimation.m_pNode[nNODE].m_nScaleKeyCount; j++)
						{
							pTKEY[j].m_vKeyScale = *((LPD3DXVECTOR3) &pData[bSIZE[bTYPE - 1][1]]);
							pTKEY[j].m_dwTime = *((LPDWORD) &pData[0]);
							pData += bSIZE[bTYPE - 1][0];
						}
					}
				}
			}
		}

		break;

	case 300	:
		{
			dwNodeCount = *((LPDWORD) pData);
			m_vAnimation.SetNodeCount( dwNodeCount );
			pData += sizeof(DWORD);
			m_vAnimation.m_dwStartTick = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_vAnimation.m_dwEndTick = *((LPDWORD) pData); pData += sizeof(DWORD);

			if( dwNodeCount > 0 )
			{
				m_vAnimation.m_pBones = new D3DXMATRIX[dwNodeCount];
				m_vAnimation.m_pTREE = new BONESNODE[dwNodeCount];
				m_vAnimation.m_pNode = new BONESANIMATION[dwNodeCount];

				m_vAnimation.m_pDATA = new VBDATA[dwNodeCount];
				m_vAnimation.m_pVB = new MESHVB[dwNodeCount];
				m_pVB = new VBDATA[dwNodeCount];

				memcpy( m_vAnimation.m_pBones, pData, sizeof(D3DXMATRIX) * dwNodeCount);
				pData += sizeof(D3DXMATRIX) * dwNodeCount;

				for( DWORD i=0; i<dwNodeCount; i++)
				{
					m_vAnimation.m_pDATA[i].m_dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
					m_vAnimation.m_pDATA[i].m_dwFVF = D3DFVF_XYZ;
					m_vAnimation.m_pVB[i].m_dwCount = 0;

					m_pVB[i].m_dwCount = m_vAnimation.m_pDATA[i].m_dwCount;
					m_pVB[i].m_dwFVF = T3DFVF_MESHVERTEX;

					if(m_pVB[i].m_dwCount)
					{
						LPWMESHVERTEX pSRC = (LPWMESHVERTEX) pData;
						LPMESHVERTEX pDEST = NULL;

						m_vAnimation.m_pDATA[i].m_pVB = new BYTE[m_pVB[i].m_dwCount * sizeof(D3DXVECTOR3)];
						m_pVB[i].m_pVB = new BYTE[m_pVB[i].m_dwCount * sizeof(MESHVERTEX)];
						pDEST = (LPMESHVERTEX) m_pVB[i].m_pVB;

						for( DWORD j=0; j<m_pVB[i].m_dwCount; j++)
						{
							((LPD3DXVECTOR3) m_vAnimation.m_pDATA[i].m_pVB)[j] = D3DXVECTOR3(
								pSRC[j].m_fPosX,
								pSRC[j].m_fPosY,
								pSRC[j].m_fPosZ);

							pDEST[j].m_fNormalX = pSRC[j].m_fNormalX;
							pDEST[j].m_fNormalY = pSRC[j].m_fNormalY;
							pDEST[j].m_fNormalZ = pSRC[j].m_fNormalZ;
							pDEST[j].m_fPosX = pSRC[j].m_fPosX;
							pDEST[j].m_fPosY = pSRC[j].m_fPosY;
							pDEST[j].m_fPosZ = pSRC[j].m_fPosZ;
							pDEST[j].m_fU1 = pSRC[j].m_fU;
							pDEST[j].m_fV1 = pSRC[j].m_fV;
							pDEST[j].m_fU2 = pSRC[j].m_fU;
							pDEST[j].m_fV2 = pSRC[j].m_fV;
						}

						pData += m_pVB[i].m_dwCount * sizeof(WMESHVERTEX);
						m_vAnimation.m_pDATA[i].m_dwCount /= 3;
						m_pVB[i].m_dwCount /= 3;
					}
				}

				memcpy( m_vAnimation.m_pTREE, pData, dwNodeCount * sizeof(BONESNODE));
				pData += dwNodeCount * sizeof(BONESNODE);

				for( i=0; i<dwNodeCount; i++)
				{
					m_vAnimation.m_pNode[i].m_nPositionKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_vAnimation.m_pNode[i].m_nPositionKeyCount > 0 )
					{
						m_vAnimation.m_pNode[i].m_pPositionKey = new TPOINTKEY[m_vAnimation.m_pNode[i].m_nPositionKeyCount];

						memcpy( m_vAnimation.m_pNode[i].m_pPositionKey, pData, m_vAnimation.m_pNode[i].m_nPositionKeyCount * sizeof(TPOINTKEY));
						pData += m_vAnimation.m_pNode[i].m_nPositionKeyCount * sizeof(TPOINTKEY);
					}

					m_vAnimation.m_pNode[i].m_nRotationKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_vAnimation.m_pNode[i].m_nRotationKeyCount > 0 )
					{
						m_vAnimation.m_pNode[i].m_pRotationKey = new TROTKEY[m_vAnimation.m_pNode[i].m_nRotationKeyCount];

						memcpy( m_vAnimation.m_pNode[i].m_pRotationKey, pData, m_vAnimation.m_pNode[i].m_nRotationKeyCount * sizeof(TROTKEY));
						pData += m_vAnimation.m_pNode[i].m_nRotationKeyCount * sizeof(TROTKEY);
					}

					m_vAnimation.m_pNode[i].m_nScaleKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_vAnimation.m_pNode[i].m_nScaleKeyCount > 0 )
					{
						m_vAnimation.m_pNode[i].m_pScaleKey = new TSCALEKEY[m_vAnimation.m_pNode[i].m_nScaleKeyCount];

						memcpy( m_vAnimation.m_pNode[i].m_pScaleKey, pData, m_vAnimation.m_pNode[i].m_nScaleKeyCount * sizeof(TSCALEKEY));
						pData += m_vAnimation.m_pNode[i].m_nScaleKeyCount * sizeof(TSCALEKEY);
					}
				}
			}
		}

		break;

	case 301	:
		{
			dwNodeCount = *((LPDWORD) pData);
			m_vAnimation.SetNodeCount( dwNodeCount );
			pData += sizeof(DWORD);
			m_vAnimation.m_dwStartTick = *((LPDWORD) pData); pData += sizeof(DWORD);
			m_vAnimation.m_dwEndTick = *((LPDWORD) pData); pData += sizeof(DWORD);

			if( dwNodeCount > 0 )
			{
				m_vAnimation.m_pBones = new D3DXMATRIX[dwNodeCount];
				m_vAnimation.m_pTREE = new BONESNODE[dwNodeCount];
				m_vAnimation.m_pNode = new BONESANIMATION[dwNodeCount];

				m_vAnimation.m_pDATA = new VBDATA[dwNodeCount];
				m_vAnimation.m_pVB = new MESHVB[dwNodeCount];
				m_pVB = new VBDATA[dwNodeCount];

				memcpy( m_vAnimation.m_pBones, pData, sizeof(D3DXMATRIX) * dwNodeCount);
				pData += sizeof(D3DXMATRIX) * dwNodeCount;

				for( DWORD i=0; i<dwNodeCount; i++)
				{
					m_vAnimation.m_pDATA[i].m_dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
					m_vAnimation.m_pDATA[i].m_dwFVF = D3DFVF_XYZ;
					m_vAnimation.m_pVB[i].m_dwCount = 0;

					m_pVB[i].m_dwCount = m_vAnimation.m_pDATA[i].m_dwCount;
					m_pVB[i].m_dwFVF = T3DFVF_MESHVERTEX;

					if(m_pVB[i].m_dwCount)
					{
						LPTMESHBUFFER pSRC = (LPTMESHBUFFER) pData;
						LPMESHVERTEX pDEST = NULL;

						m_vAnimation.m_pDATA[i].m_pVB = new BYTE[m_pVB[i].m_dwCount * sizeof(D3DXVECTOR3)];
						m_pVB[i].m_pVB = new BYTE[m_pVB[i].m_dwCount * sizeof(MESHVERTEX)];
						pDEST = (LPMESHVERTEX) m_pVB[i].m_pVB;

						for( DWORD j=0; j<m_pVB[i].m_dwCount; j++)
						{
							((LPD3DXVECTOR3) m_vAnimation.m_pDATA[i].m_pVB)[j] = D3DXVECTOR3(
								pSRC[j].m_fPosX,
								pSRC[j].m_fPosY,
								pSRC[j].m_fPosZ);

							pDEST[j].m_fNormalX = pSRC[j].m_fNormalX;
							pDEST[j].m_fNormalY = pSRC[j].m_fNormalY;
							pDEST[j].m_fNormalZ = pSRC[j].m_fNormalZ;
							pDEST[j].m_fPosX = pSRC[j].m_fPosX;
							pDEST[j].m_fPosY = pSRC[j].m_fPosY;
							pDEST[j].m_fPosZ = pSRC[j].m_fPosZ;
							pDEST[j].m_fU1 = pSRC[j].m_fU1;
							pDEST[j].m_fV1 = pSRC[j].m_fV1;
							pDEST[j].m_fU2 = pSRC[j].m_fU2;
							pDEST[j].m_fV2 = pSRC[j].m_fV2;
						}

						pData += m_pVB[i].m_dwCount * sizeof(TMESHBUFFER);
						m_vAnimation.m_pDATA[i].m_dwCount /= 3;
						m_pVB[i].m_dwCount /= 3;
					}
				}

				memcpy( m_vAnimation.m_pTREE, pData, dwNodeCount * sizeof(BONESNODE));
				pData += dwNodeCount * sizeof(BONESNODE);

				for( i=0; i<dwNodeCount; i++)
				{
					m_vAnimation.m_pNode[i].m_nPositionKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_vAnimation.m_pNode[i].m_nPositionKeyCount > 0 )
					{
						m_vAnimation.m_pNode[i].m_pPositionKey = new TPOINTKEY[m_vAnimation.m_pNode[i].m_nPositionKeyCount];

						memcpy( m_vAnimation.m_pNode[i].m_pPositionKey, pData, m_vAnimation.m_pNode[i].m_nPositionKeyCount * sizeof(TPOINTKEY));
						pData += m_vAnimation.m_pNode[i].m_nPositionKeyCount * sizeof(TPOINTKEY);
					}

					m_vAnimation.m_pNode[i].m_nRotationKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_vAnimation.m_pNode[i].m_nRotationKeyCount > 0 )
					{
						m_vAnimation.m_pNode[i].m_pRotationKey = new TROTKEY[m_vAnimation.m_pNode[i].m_nRotationKeyCount];

						memcpy( m_vAnimation.m_pNode[i].m_pRotationKey, pData, m_vAnimation.m_pNode[i].m_nRotationKeyCount * sizeof(TROTKEY));
						pData += m_vAnimation.m_pNode[i].m_nRotationKeyCount * sizeof(TROTKEY);
					}

					m_vAnimation.m_pNode[i].m_nScaleKeyCount = *((int *) pData);
					pData += sizeof(int);

					if( m_vAnimation.m_pNode[i].m_nScaleKeyCount > 0 )
					{
						m_vAnimation.m_pNode[i].m_pScaleKey = new TSCALEKEY[m_vAnimation.m_pNode[i].m_nScaleKeyCount];

						memcpy( m_vAnimation.m_pNode[i].m_pScaleKey, pData, m_vAnimation.m_pNode[i].m_nScaleKeyCount * sizeof(TSCALEKEY));
						pData += m_vAnimation.m_pNode[i].m_nScaleKeyCount * sizeof(TSCALEKEY);
					}
				}
			}
		}

		break;

	default		: ReleaseAnimation(); break;
	}

	for( int i=0; i< dwNodeCount ; i++)
	{
		m_vAnimation.InitPositionKey(&m_vAnimation.m_pNode[i]);
		m_vAnimation.InitRotationKey(&m_vAnimation.m_pNode[i]);
		m_vAnimation.InitScaleKey(&m_vAnimation.m_pNode[i]);
	}
}

CAnimationItem *CAnimationItem::GetAniItem( LPANIDATA pANI)
{
	for( int i=0; i<INT(m_Aniset.m_vAni.size()); i++)
		if( m_Aniset.m_vAni[i] == pANI )
			return GetAnimationItem(m_vAniID[i]);

	return NULL;
}

void CAnimationItem::Render( LPDIRECT3DDEVICE9 pDevice,
							 LPTPIVOT pPivot,
							 D3DXMATRIX& vWorld,
							 FLOAT fTimeValue)
{
	static D3DXMATRIX vLocal[255];

	if(!pDevice)
		return;

	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE);
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
	pDevice->SetFVF(T3DFVF_MESHVERTEX);

	m_vAnimation.GetFrameMatrix(
		vLocal,
		NULL,
		NULL,
		pPivot,
		vWorld, 0,
		fTimeValue,
		0.0f);

	for( DWORD i=0; i<m_vAnimation.GetNodeCount(); i++)
		if( m_pVB[i].m_dwCount > 0 )
		{
			pDevice->SetTransform( D3DTS_WORLD, &vLocal[i + 1]);
			Render( pDevice, i);
		}
}

void CAnimationItem::Render( LPDIRECT3DDEVICE9 pDevice, int nIndex)
{
	pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		m_pVB[nIndex].m_dwCount,
		m_pVB[nIndex].m_pVB,
		sizeof(MESHVERTEX));
}

//------------------------------------------------------------------------------------------------------------------------//
void TEXTURESETSTUDIO::PushTextureItem( CTextureItem* pTextureItem)
{
	m_vTextureItems.push_back( pTextureItem);
	m_vTexture.push_back(NULL);
}
//------------------------------------------------------------------------------------------------------------------------//
void TEXTURESETSTUDIO::InsertTextureItem( int nIndex, CTextureItem* pTextureItem)
{
	m_vTextureItems.insert( m_vTextureItems.begin() + nIndex, pTextureItem);
	m_vTexture.insert( m_vTexture.begin() + nIndex, NULL);
}
//------------------------------------------------------------------------------------------------------------------------//
void TEXTURESETSTUDIO::EraseTextureItem( int nIndex)
{
	m_vTextureItems.erase( m_vTextureItems.begin()+nIndex);
	m_vTexture.erase( m_vTexture.begin()+nIndex);
}
//------------------------------------------------------------------------------------------------------------------------//
CT3DTexture *TEXTURESETSTUDIO::GetTexturePtr( int nIndex)
{
	if( m_vTexture[nIndex] == NULL &&
		m_vTextureItems[nIndex] != NULL)
	{
		CT3DTexture *pTex = m_vTextureItems[nIndex]->GetT3DTEX();

		if(!pTex)
			return NULL;

		m_vTexture[nIndex] = pTex;
	}

	return m_vTexture[nIndex];
}
//------------------------------------------------------------------------------------------------------------------------//
void TEXTURESETSTUDIO::ClearTextureItem()
{
	m_vTextureItems.clear();
	m_vTexture.clear();
}


void CItemBase::TextureItemUpdate(
	CTextureItem *pItem,
	CString strFileName )
{
	CFile file( strFileName, CFile::modeRead);

	pItem->LoadTextureFromCFile( file);

	int nCount = INT(pItem->m_vTextureID.size());
	BOOL bHasID = FALSE;

	for( int i=0; i<nCount; i++)
		if( pItem->m_vTextureID[i] == pItem->m_dwID )
		{
			bHasID = TRUE;
			break;
		}

	if(!bHasID)
		pItem->m_vTextureID.push_back(pItem->m_dwID);

	MAPITEM::iterator it;
	for( it = CItemBase::m_mapTextureItem.begin(); it != CItemBase::m_mapTextureItem.end(); it++)
		(*it).second->Regulate();

	CTachyonDoc::UpdateAll(NULL);
}

void CItemBase::TextureItemUpdateByID(
	DWORD dwID,
	CString strFileName)
{
	CTextureItem* pItem = CItemBase::GetTextureItem( dwID );
	if( pItem )
	{
		TextureItemUpdate(
			pItem,
			strFileName);
	}
}

void CItemBase::ImageItemUpdate(
	CImageItem *pItem,
	CString strFileName )
{
	CFile file( strFileName, CFile::modeRead);

	if(pItem->m_pData)
		delete[] pItem->m_pData;
	pItem->m_Image.Release();

	pItem->m_dwSize = (DWORD) file.GetLength();
	pItem->m_pData = new BYTE[pItem->m_dwSize];
	file.Read( pItem->m_pData, pItem->m_dwSize);

	pItem->m_Image.LoadImageFile(
		CMainFrame::m_3DDevice.m_pDevice,
		pItem->m_pData,
		pItem->m_dwSize);

	LPT3DDATA pData = pItem->m_Image.BuildImageData(CMainFrame::m_3DDevice.m_pDevice);
	delete[] pItem->m_pData;
	pItem->m_dwSize = pData->m_dwSize;
	pItem->m_pData = pData->m_pData;
	pData->m_pData = NULL;
	delete pData;
	pItem->m_Image.Load(pItem->m_pData);

	int nCount = INT(pItem->m_vImageID.size());
	BOOL bHasID = FALSE;

	for( int i=0; i<nCount; i++)
		if( pItem->m_vImageID[i] == pItem->m_dwID )
		{
			bHasID = TRUE;
			break;
		}

	if(!bHasID)
		pItem->m_vImageID.push_back(pItem->m_dwID);

	MAPITEM::iterator it;
	for( it = CItemBase::m_mapImageItem.begin(); it != CItemBase::m_mapImageItem.end(); it++)
		(*it).second->Regulate();

	CTachyonDoc::UpdateAll(NULL);
}

void CItemBase::ImageItemUpdateByID(
	DWORD dwID,
	CString strFileName)
{
	CImageItem* pItem = CItemBase::GetImageItem( dwID );
	if( pItem )
	{
		ImageItemUpdate(
			pItem,
			strFileName);
	}
}

void CItemBase::MediaItemUpdate(
	CMediaItem *pItem,
	CString strFileName )
{
	CString strEXT;
	CTShell shell;

	CMainFrame::m_TachyonMedia.Unregister(
		pItem->GetPlayerType(),
		pItem->m_dwID);

	strEXT = shell.GetFileExt(strFileName);
	strEXT.MakeUpper();
	strEXT.TrimRight();
	strEXT.TrimLeft();

	if( strEXT == _T(".WAV") )
		pItem->m_bMediaType = MEDIATYPE_WAV;
	else if( strEXT == _T(".MPA") )
		pItem->m_bMediaType = MEDIATYPE_MPA;
	else if( strEXT == _T(".MP2") )
		pItem->m_bMediaType = MEDIATYPE_MP2;
	else if( strEXT == _T(".MP3") )
		pItem->m_bMediaType = MEDIATYPE_MP3;
	else if( strEXT == _T(".AU") )
		pItem->m_bMediaType = MEDIATYPE_AU;
	else if( strEXT == _T(".AIF") )
		pItem->m_bMediaType = MEDIATYPE_AIF;
	else if( strEXT == _T(".AIFF") )
		pItem->m_bMediaType = MEDIATYPE_AIFF;
	else if( strEXT == _T(".SND") )
		pItem->m_bMediaType = MEDIATYPE_SND;
	else if( strEXT == _T(".MID") )
		pItem->m_bMediaType = MEDIATYPE_MID;
	else if( strEXT == _T(".MIDI") )
		pItem->m_bMediaType = MEDIATYPE_MIDI;
	else if( strEXT == _T(".RMI") )
		pItem->m_bMediaType = MEDIATYPE_RMI;
	else if( strEXT == _T(".AVI") )
		pItem->m_bMediaType = MEDIATYPE_AVI;
	else if( strEXT == _T(".QT") )
		pItem->m_bMediaType = MEDIATYPE_QT;
	else if( strEXT == _T(".MOV") )
		pItem->m_bMediaType = MEDIATYPE_MOV;
	else if( strEXT == _T(".MPG") )
		pItem->m_bMediaType = MEDIATYPE_MPG;
	else if( strEXT == _T(".MPEG") )
		pItem->m_bMediaType = MEDIATYPE_MPEG;
	else if( strEXT == _T(".M1V") )
		pItem->m_bMediaType = MEDIATYPE_M1V;
	else if( strEXT == _T(".DAT") )
		pItem->m_bMediaType = MEDIATYPE_DAT;
	else
		pItem->m_bMediaType = 0xFF;
	CopyFile( strFileName, pItem->m_szFileName, FALSE);

	switch(pItem->GetPlayerType())
	{
	case MEDIA_TMUSIC	:
		{
			CMainFrame::m_TachyonMedia.LoadDMusic(
				pItem->m_dwID,
				pItem->m_szFileName,
				pItem->m_bVolume);
		}

		break;

	case MEDIA_TSOUND	:
		{
			CMainFrame::m_TachyonMedia.LoadDSound(
				pItem->m_dwID,
				pItem->m_szFileName,
				pItem->m_bVolume);

			CD3DSound *pWAV = (CD3DSound *) CMainFrame::m_TachyonMedia.GetData(
				MEDIA_TSOUND,
				pItem->m_dwID);
			pWAV->LoadData();

			if(!pWAV->GetDSB(0))
			{
				CMainFrame::m_TachyonMedia.Unregister(
					MEDIA_TSOUND,
					pItem->m_dwID);

				pItem->m_bMediaType = MEDIATYPE_MP3;
			}
			else
				break;
		}

	case MEDIA_TSHOW	:
		{
			CMainFrame::m_TachyonMedia.LoadDShow(
				NULL, NULL,
				pItem->m_dwID, 0,
				pItem->m_szFileName,
				pItem->m_bVolume);
		}

		break;
	}

	CTachyonDoc::UpdateAll(NULL);
}

void CItemBase::MediaItemUpdateByID(
	DWORD dwID,
	CString strFileName)
{
	CMediaItem* pItem = CItemBase::GetMediaItem( dwID );
	if( pItem )
	{
		MediaItemUpdate(
			pItem,
			strFileName);
	}
}

void CItemBase::AnimationItemUpdate(
	CAnimationItem *pItem,
	CString strFileName )
{
	CFile file( strFileName, CFile::modeRead);

	pItem->LoadAnimationFromCFile( file, FALSE );

	int nCount = INT(pItem->m_vAniID.size());
	BOOL bHasID = FALSE;

	for( int i=0; i<nCount; i++)
		if( pItem->m_vAniID[i] == pItem->m_dwID )
		{
			bHasID = TRUE;
			break;
		}

	if(!bHasID)
	{
		pItem->m_vAniID.push_back(pItem->m_dwID);
		pItem->m_vReverse.push_back(FALSE);
	}

	MAPITEM::iterator it;
	for( it = CItemBase::m_mapAnimationItem.begin(); it != CItemBase::m_mapAnimationItem.end(); it++)
		(*it).second->Regulate();

	CTachyonDoc::UpdateAll(NULL);
}

void CItemBase::AnimationItemUpdateByID(
	DWORD dwID,
	CString strFileName)
{
	CAnimationItem* pItem = CItemBase::GetAnimationItem( dwID );
	if( pItem )
	{
		AnimationItemUpdate(
			pItem,
			strFileName);
	}
}

void CItemBase::MeshItemUpdate(
	CMeshItem *pItem,
	CString strFileName )
{
	CFile file( strFileName, CFile::modeRead);
	DWORD dwSize = (DWORD) file.GetLength();

	LPBYTE pData = new BYTE[dwSize];
	file.Read( pData, dwSize);

	pItem->m_Mesh.Release();
	pItem->m_Mesh.LoadMesh(
		CMainFrame::m_3DDevice.m_pDevice,
		pData);

	delete[] pData;
	pData = NULL;

	CTachyonDoc::UpdateAll(NULL);
}

void CItemBase::MeshItemUpdateByID(
		DWORD dwID,
		CString strFileName)
{
	CMeshItem* pItem = CItemBase::GetMeshItem( dwID );
	if( pItem )
	{
		MeshItemUpdate(
			pItem,
			strFileName);
	}
}