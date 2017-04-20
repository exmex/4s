#include "stdafx.h"
#include "Itembase.h"

#define	DEF_UNITSIZE		4096 * 2048
#define	DEF_OBJSIZE			4096 * 2048
#define TOTALFRAME			(30)

char CItemBase::m_szTempPath[_MAX_PATH];

LPDIRECT3DVERTEXBUFFER9 CItemBase::m_pVB = NULL;
CD3DDevice CItemBase::m_Device;

MAPITEM CItemBase::m_mapAnimationItem;
MAPITEM CItemBase::m_mapTextureItem;
MAPITEM CItemBase::m_mapImageItem;
MAPITEM CItemBase::m_mapMediaItem;
MAPITEM CItemBase::m_mapFrameItem;
MAPITEM CItemBase::m_mapCompItem;
MAPITEM CItemBase::m_mapMeshItem;
MAPITEM CItemBase::m_mapMAPItem;
MAPITEM CItemBase::m_mapOBJItem;
MAPITEM CItemBase::m_mapSFXItem;

MAPIDINDEX CItemBase::m_mapLocalID;
MAPSTRID CItemBase::m_mapGlobalID;
MAPGSFONT CItemBase::m_mapFont;
MAPSTR CItemBase::m_mapMenuID;

DWORD CItemBase::g_dwAnimationID = 1;
DWORD CItemBase::g_dwOBJTypeID = 1;
DWORD CItemBase::g_dwTextureID = 1;
DWORD CItemBase::g_dwImageID = 1;
DWORD CItemBase::g_dwMediaID = 1;
DWORD CItemBase::g_dwFrameID = 1;
DWORD CItemBase::g_dwCompID = 1;
DWORD CItemBase::g_dwMeshID = 1;
DWORD CItemBase::g_dwMAPID = 1;
DWORD CItemBase::g_dwOBJID = 1;
DWORD CItemBase::g_dwSFXID = 1;
DWORD CItemBase::g_dwLocalID = 1;

BYTE CItemBase::m_bMerge = FALSE;
BYTE CItemBase::m_bOptimize = FALSE;


CItemBase::CItemBase( BYTE bType)
{
	m_vLocalID.clear();
	m_strName = _T("");
	m_strID = _T("");

	m_bType = bType;
	m_dwID = 0;
	m_bValid = TRUE;

	switch(bType)
	{
	case ITEM_ANIMATION		: m_dwID = m_bMerge ? g_dwAnimationID : g_dwAnimationID++; break;
	case ITEM_OBJTYPE		: m_dwID = m_bMerge ? g_dwOBJTypeID : g_dwOBJTypeID++; break;
	case ITEM_TEXTURE		: m_dwID = m_bMerge ? g_dwTextureID : g_dwTextureID++; break;
	case ITEM_IMAGE			: m_dwID = m_bMerge ? g_dwImageID : g_dwImageID++; break;
	case ITEM_MEDIA			: m_dwID = m_bMerge ? g_dwMediaID : g_dwMediaID++; break;
	case ITEM_MESH			: m_dwID = m_bMerge ? g_dwMeshID : g_dwMeshID++; break;
	case ITEM_OBJECT		: m_dwID = m_bMerge ? g_dwOBJID : g_dwOBJID++; break;
	case ITEM_SFX			: m_dwID = m_bMerge ? g_dwSFXID : g_dwSFXID++; break;
	case ITEM_EXTMAP		: m_dwID = m_bMerge ? g_dwMAPID : g_dwMAPID++; break;
	case ITEM_BSPMAP		: m_dwID = m_bMerge ? g_dwMAPID : g_dwMAPID++; break;
	case ITEM_FRAME			: m_dwID = m_bMerge ? g_dwFrameID : g_dwFrameID++; break;
	case ITEM_COMP			: m_dwID = m_bMerge ? g_dwCompID : g_dwCompID++; break;
	}
}

CItemBase::~CItemBase()
{
	MAPSTRID::iterator finder = m_mapGlobalID.find(m_strID);
	if( finder != m_mapGlobalID.end() )
		m_mapGlobalID.erase(finder);

	while(!m_vLocalID.empty())
		ReleaseLocalID( m_vLocalID[0], this);
}

CMeshItem* CItemBase::GetMeshItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapMeshItem.find(dwID);

	if( finder != m_mapMeshItem.end() )
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

	if( finder != m_mapMAPItem.end() )
		return (CMAPItem *) (*finder).second;

	return NULL;
}

CSFXItem* CItemBase::GetSFXItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapSFXItem.find(dwID);

	if( finder != m_mapSFXItem.end() )
		return (CSFXItem *) (*finder).second;

	return NULL;
}

CAnimationItem* CItemBase::GetAnimationItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapAnimationItem.find(dwID);

	if( finder != m_mapAnimationItem.end() )
		return (CAnimationItem *) (*finder).second;

	return NULL;
}

COBJItem* CItemBase::GetOBJItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapOBJItem.find(dwID);

	if( finder != m_mapOBJItem.end() )
		return (COBJItem *) (*finder).second;

	return NULL;
}

CTextureItem* CItemBase::GetTextureItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapTextureItem.find(dwID);

	if( finder != m_mapTextureItem.end() )
		return (CTextureItem *) (*finder).second;

	return NULL;
}

CImageItem* CItemBase::GetImageItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapImageItem.find(dwID);

	if( finder != m_mapImageItem.end() )
		return (CImageItem *) (*finder).second;

	return NULL;
}

CMediaItem* CItemBase::GetMediaItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapMediaItem.find(dwID);

	if( finder != m_mapMediaItem.end() )
		return (CMediaItem *) (*finder).second;

	return NULL;
}

CFRAMEItem* CItemBase::GetFRAMEItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapFrameItem.find(dwID);

	if( finder != m_mapFrameItem.end() )
		return (CFRAMEItem *) (*finder).second;

	return NULL;
}

CCOMPItem* CItemBase::GetCOMPItem( DWORD dwID)
{
	MAPITEM::iterator finder = m_mapCompItem.find(dwID);

	if( finder != m_mapCompItem.end() )
		return (CCOMPItem *) (*finder).second;

	return NULL;
}

DWORD CItemBase::GetID( CString strID)
{
	MAPSTRID::iterator finder = m_mapGlobalID.find(strID);
	return finder != m_mapGlobalID.end() ? (*finder).second : 0;
}

void CItemBase::SetValidTexture( DWORD dwID)
{
	if( m_bOptimize )
	{
		MAPITEM::iterator itr = m_mapTextureItem.find(dwID);
		if( itr != m_mapTextureItem.end() )
		{
			CTextureItem* pTItem = static_cast<CTextureItem*>(itr->second);
			pTItem->m_bValid = TRUE;

			size_t cnt = pTItem->m_vTextureID.size();
			for(size_t i=0; i<cnt; ++i)
			{
				if( pTItem->m_vTextureID[i] != dwID )
					SetValidTexture(pTItem->m_vTextureID[i]);
			}
		}
	}
}

void CItemBase::SetValidMesh( DWORD dwID)
{
	if( m_bOptimize )
	{
		MAPITEM::iterator itr = m_mapMeshItem.find(dwID);
		if( itr != m_mapMeshItem.end() )
			itr->second->m_bValid = TRUE;
	}
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
		m_mapGlobalID.insert( MAPSTRID::value_type( strID, g_dwLocalID));
		g_dwLocalID++;

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

CItemBase* CItemBase::CreateItem( LPTGSFFILE pGSFFILE )
{
	CItemBase* pItem = NULL;
	
	BYTE bType;
	pGSFFILE->m_File.Read( &bType, sizeof(BYTE));

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
		pItem->Load( pGSFFILE );

	return pItem;
}

void CItemBase::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

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
		CString strHead = m_strID;
		DWORD dwCount = 0;
		BYTE bContinue = TRUE;

		while(bContinue)
		{
			MAPSTRID::iterator finder = m_mapGlobalID.find(m_strID);

			if( finder == m_mapGlobalID.end() )
			{
				m_mapGlobalID.insert( MAPSTRID::value_type( m_strID, m_dwID));
				bContinue = FALSE;
			}
			else if( m_dwID != (*finder).second )
				m_strID.Format( "%s%d", strHead, dwCount++);
			else
				bContinue = FALSE;
		}
	}
}


//////////////////////////////////////////////////////////////////
// CProjectItem Function...
//////////////////////////////////////////////////////////////////

CProjectItem::CProjectItem()
:CItemBase(ITEM_PROJECT)
{
}

CProjectItem::~CProjectItem()
{
}


//////////////////////////////////////////////////////////////////
// CFolderItem Function...
//////////////////////////////////////////////////////////////////

CFolderItem::CFolderItem()
:CItemBase(ITEM_FOLDER)
{
}

CFolderItem::~CFolderItem()
{
}


//////////////////////////////////////////////////////////////////
// CTextureItem Function...
//////////////////////////////////////////////////////////////////

CTextureItem::CTextureItem()
:CItemBase(ITEM_TEXTURE)
{
	m_vTextureID.clear();
	m_pData = NULL;
	m_dwSize = 0;
	m_bFullSize = FALSE;
	m_bFormat = DXT3;
	m_pTextureSet = NULL;
	m_pGSFFILE = NULL;
	m_dwGSFOffset = 0;
}

CTextureItem::~CTextureItem()
{
	ReleaseData();
	m_vTextureID.clear();
}

void CTextureItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CItemBase::Load( pGSFFILE );
	m_mapTextureItem.insert( MAPITEM::value_type( m_dwID, this));

	if(CItemBase::m_bOptimize)
		m_bValid = !m_strID.IsEmpty();
	else
		m_bValid = TRUE;

	m_pGSFFILE = pGSFFILE;
	m_dwGSFOffset = pFile->GetPosition();

	{
		DWORD dwSize = 0;
		pFile->Read( &dwSize, sizeof(DWORD));

		if( dwSize > 0 )
			pFile->Seek( dwSize, CFile::current );

		int nCount = 0;
		pFile->Read( &nCount, sizeof(int));

		for( int i=0; i<nCount; i++)
		{
			DWORD dwID;

			pFile->Read( &dwID, sizeof(DWORD));
			if( m_bMerge && dwID )
				dwID += g_dwTextureID;

			m_vTextureID.push_back(dwID);
		}

		pFile->Seek( sizeof(COLORREF) + sizeof(DWORD), CFile::current);
		pFile->Read( &nCount, sizeof(int));

		for( i=0; i<nCount; i++)
			pFile->Seek( sizeof(DWORD) + sizeof(FLOAT)*5, CFile::current );

		pFile->Seek( sizeof(DWORD)*2 + sizeof(FLOAT) + sizeof(BYTE)*2, CFile::current );
	}
}

BOOL CTextureItem::LoadData()
{
	if( m_pGSFFILE == NULL || m_dwGSFOffset == 0 )
		return FALSE;

	CFile* pFile = &m_pGSFFILE->m_File;
	ULONGLONG dwOrgPosition = pFile->GetPosition();
	pFile->Seek( m_dwGSFOffset, CFile::begin );

	{
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
			pFile->Seek( sizeof(DWORD), CFile::current );
		}

		pFile->Seek( sizeof(COLORREF) + sizeof(DWORD), CFile::current);
		pFile->Read( &nCount, sizeof(int));

		m_pTextureSet = new TEXTURESET;

		for( i=0; i<nCount; i++)
		{
			LPUVKEY pKEY = new UVKEY();

			pFile->Read( &pKEY->m_dwTick, sizeof(DWORD));
			pFile->Read( &pKEY->m_fKeySU, sizeof(FLOAT));
			pFile->Read( &pKEY->m_fKeySV, sizeof(FLOAT));
			pFile->Read( &pKEY->m_fKeyU, sizeof(FLOAT));
			pFile->Read( &pKEY->m_fKeyV, sizeof(FLOAT));
			pFile->Read( &pKEY->m_fKeyR, sizeof(FLOAT));

			m_pTextureSet->m_vKey.push_back( pKEY );
		}

		pFile->Read( &m_pTextureSet->m_dwTotalTick, sizeof(DWORD));
		pFile->Read( &m_pTextureSet->m_dwMipFilter, sizeof(DWORD));
		pFile->Read( &m_pTextureSet->m_fMipBias, sizeof(FLOAT));
		pFile->Read( &m_bFullSize, sizeof(BYTE));
		pFile->Read( &m_bFormat, sizeof(BYTE));
	}

	pFile->Seek( dwOrgPosition, CFile::begin );
	return TRUE;
}

void CTextureItem::ReleaseData()
{
	if( m_pTextureSet )
		delete m_pTextureSet;

	if( m_pData )
		delete [] m_pData;

	m_dwSize = 0;
	m_pData = NULL;
	m_pTextureSet = NULL;
}

//////////////////////////////////////////////////////////////////
// CImageItem Function...
//////////////////////////////////////////////////////////////////

CImageItem::CImageItem()
:CItemBase(ITEM_IMAGE)
{
	m_vImageID.clear();

	m_pData2 = NULL;
	m_dwSize = 0;
	m_pImageSet = NULL;
	m_bFormat2 = NON_COMP;
	m_pGSFFILE = NULL;
	m_dwGSFOffset = 0;
	m_nWidth = 0;
	m_nHeight = 0;
}

CImageItem::~CImageItem()
{
	ReleaseData();
	m_vImageID.clear();
}

void CImageItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CItemBase::Load( pGSFFILE );
	m_mapImageItem.insert( MAPITEM::value_type( m_dwID, this));

	m_pGSFFILE = pGSFFILE;
	m_dwGSFOffset = pFile->GetPosition();

	{
		pFile->Read( &m_dwSize, sizeof(DWORD));
		if( m_dwSize > 0 )
		{
			pFile->Seek( sizeof(int), CFile::current );
			pFile->Read( &m_nWidth, sizeof(int) );
			pFile->Read( &m_nHeight, sizeof(int) );
			pFile->Seek( m_dwSize-(sizeof(int)*3), CFile::current );
		}

		int nCount = 0;
		pFile->Read( &nCount, sizeof(int));

		for( int i=0; i<nCount; i++)
		{
			DWORD dwID = 0;

			pFile->Read( &dwID, sizeof(DWORD));
			if( m_bMerge && dwID )
				dwID += g_dwImageID;

			m_vImageID.push_back(dwID);
		}

		pFile->Seek( sizeof(COLORREF) + sizeof(DWORD), CFile::current);
		pFile->Read( &nCount, sizeof(int));

		for( i=0; i<nCount; i++)
			pFile->Seek( sizeof(DWORD)*2, CFile::current );

		pFile->Seek( sizeof(DWORD) + sizeof(BYTE), CFile::current );
	}
}

BOOL CImageItem::LoadData()
{
	if( m_pGSFFILE == NULL || m_dwGSFOffset == 0 )
		return FALSE;

	CFile* pFile = &m_pGSFFILE->m_File;
	ULONGLONG dwOrgPosition = pFile->GetPosition();
	pFile->Seek( m_dwGSFOffset, CFile::begin );

	{
		pFile->Read( &m_dwSize, sizeof(DWORD));
		if( m_dwSize > 0 )
		{
			m_pData2 = new BYTE[m_dwSize];
			pFile->Read( m_pData2, m_dwSize);
		}

		int nCount = 0;
		pFile->Read( &nCount, sizeof(int));

		for( int i=0; i<nCount; i++)
		{
			pFile->Seek( sizeof(DWORD), CFile::current );
		}

		pFile->Seek( sizeof(COLORREF) + sizeof(DWORD), CFile::current);
		pFile->Read( &nCount, sizeof(int));

		m_pImageSet = new IMAGESET;

		for( i=0; i<nCount; i++)
		{
			LPCLRKEY pKEY = new CLRKEY();

			pFile->Read( &pKEY->m_dwColor, sizeof(DWORD));
			pFile->Read( &pKEY->m_dwTick, sizeof(DWORD));

			m_pImageSet->m_vKey.push_back( pKEY );
		}

		pFile->Read( &m_pImageSet->m_dwTotalTick, sizeof(DWORD));
		pFile->Read( &m_bFormat2, sizeof(BYTE));
	}

	pFile->Seek( dwOrgPosition, CFile::begin );
	return TRUE;
}

void CImageItem::ReleaseData()
{
	if( m_pImageSet )
		delete m_pImageSet;

	if( m_pData2 )
		delete [] m_pData2;

	m_dwSize = 0;
	m_pData2 = NULL;
	m_pImageSet = NULL;
}

//////////////////////////////////////////////////////////////////
// CMediaItem Function...
//////////////////////////////////////////////////////////////////

CMediaItem::CMediaItem()
:CItemBase(ITEM_MEDIA)
{
	m_bMediaType = MEDIATYPE_WAV;
	m_bVolume = 100;
	m_pData = NULL;
	m_dwSize = 0;
	m_pGSFFILE = NULL;
	m_dwGSFOffset = 0;
}

CMediaItem::~CMediaItem()
{
	ReleaseData();
}

void CMediaItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CItemBase::Load( pGSFFILE );
	m_mapMediaItem.insert( MAPITEM::value_type( m_dwID, this));

	m_pGSFFILE = pGSFFILE;
	m_dwGSFOffset = pFile->GetPosition();

	{
		DWORD dwSize = 0;
		pFile->Read( &dwSize, sizeof(DWORD));

		if( dwSize > 0 )
			pFile->Seek( dwSize, CFile::current );

		pFile->Read( &m_bMediaType, sizeof(BYTE));
		pFile->Read( &m_bVolume, sizeof(BYTE));
	}
}

BOOL CMediaItem::LoadData()
{
	if( m_pGSFFILE == NULL || m_dwGSFOffset == 0 )
		return FALSE;

	CFile* pFile = &m_pGSFFILE->m_File;
	ULONGLONG dwOrgPosition = pFile->GetPosition();
	pFile->Seek( m_dwGSFOffset, CFile::begin );

	{
		pFile->Read( &m_dwSize, sizeof(DWORD));

		if( m_dwSize > 0 )
		{
			m_pData = new BYTE[ m_dwSize ];
			pFile->Read( m_pData, m_dwSize);
		}
	}

	pFile->Seek( dwOrgPosition, CFile::begin );
	return TRUE;
}

void CMediaItem::ReleaseData()
{
	if( m_pData )
		delete [] m_pData;

	m_pData = NULL;
	m_dwSize = 0;
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
:CItemBase(ITEM_ANIMATION)
{
	m_vReverse.clear();
	m_vEventID.clear();
	m_vAniID.clear();

	m_strLoopID = _T("");
	m_pData = NULL;
	m_dwSize = 0;

	m_dwNodeCount = 0;
}

CAnimationItem::~CAnimationItem()
{
	ReleaseData();

	m_vReverse.clear();
	m_vEventID.clear();
	m_vAniID.clear();

	m_strLoopID = _T("");

	m_pGSFFILE = NULL;
	m_dwGSFOffset = 0;
}

void CAnimationItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CItemBase::Load( pGSFFILE );
	m_mapAnimationItem.insert( MAPITEM::value_type( m_dwID, this));
	
	m_pGSFFILE = pGSFFILE;
	m_dwGSFOffset = pFile->GetPosition();

	{
		DWORD dwSize = 0;
		pFile->Read( &dwSize, sizeof(DWORD));
		if( dwSize > 0 )
		{
			pFile->Seek( sizeof(int), CFile::current );
			pFile->Read( &m_dwNodeCount, sizeof(DWORD) );
			pFile->Seek( dwSize - sizeof(int) - sizeof(DWORD), CFile::current );
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
				dwAniID += g_dwAnimationID;

			m_vReverse.push_back(bReverse);
			m_vAniID.push_back(dwAniID);
		}

		pFile->Seek( sizeof(DWORD), CFile::current);
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
	}
}

BOOL CAnimationItem::LoadData()
{
	if( m_pGSFFILE == NULL || m_dwGSFOffset == 0 )
		return FALSE;

	CFile* pFile = &m_pGSFFILE->m_File;
	ULONGLONG dwOrgPosition = pFile->GetPosition();
	pFile->Seek( m_dwGSFOffset, CFile::begin );

	{
		pFile->Read( &m_dwSize, sizeof(DWORD));
		if( m_dwSize > 0 )
		{
			m_pData = new BYTE[m_dwSize];
			pFile->Read( m_pData, m_dwSize);
		}
	}

	pFile->Seek( dwOrgPosition, CFile::begin );
	return TRUE;
}

void CAnimationItem::ReleaseData()
{
	if(m_pData)
		delete[] m_pData;

	m_pData = NULL;
	m_dwSize = 0;
}

//////////////////////////////////////////////////////////////////
// CMeshItem Function...
//////////////////////////////////////////////////////////////////

CMeshItem::CMeshItem()
:CItemBase(ITEM_MESH)
{
	m_pGSFFILE = NULL;
	m_dwGSFOffset = 0;
	m_dwNodeCount = 0;
}

CMeshItem::~CMeshItem()
{
}

void CMeshItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CItemBase::Load( pGSFFILE );
	m_mapMeshItem.insert( MAPITEM::value_type( m_dwID, this));

	if(CItemBase::m_bOptimize)
		m_bValid = !m_strID.IsEmpty();
	else
		m_bValid = TRUE;

	m_pGSFFILE = pGSFFILE;
	m_dwGSFOffset = pFile->GetPosition();

	{
		DWORD dwLevel = 0;
		DWORD dwCount = 0;

		DWORD dwMeshCount = 0;

		pFile->Read( &dwMeshCount, sizeof(DWORD));
		pFile->Read( &m_dwNodeCount, sizeof(DWORD));
		pFile->Seek( sizeof(BYTE), CFile::current );
		pFile->Read( &dwLevel, sizeof(DWORD));

		pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
		pFile->Seek( sizeof(FLOAT), CFile::current );

		if( m_dwNodeCount > 0 )
			pFile->Seek( sizeof(D3DXMATRIX) * m_dwNodeCount, CFile::current );

		pFile->Read( &dwCount, sizeof(DWORD));
		if( dwCount > 0 )
			pFile->Seek( dwCount * (m_dwNodeCount > 0 ? sizeof(WMESHVERTEX) : sizeof(MESHVERTEX)), CFile::current );

		for( int i=0; i<dwMeshCount; i++)
			for( int j=0; j<dwLevel; j++)
			{
				pFile->Read( &dwCount, sizeof(DWORD));

				for( DWORD k=0; k<dwCount; k++)
				{
					DWORD dwLevelIndexCount = 0;
					pFile->Read( &dwLevelIndexCount, sizeof(DWORD));
					pFile->Seek( sizeof(WORD), CFile::current );

					if( dwLevelIndexCount > 0 )
						pFile->Seek( dwLevelIndexCount * sizeof(WORD), CFile::current );
				}
			}

		for( i=0; i<INT(dwLevel) - 1; i++)
			pFile->Seek( sizeof(FLOAT), CFile::current );
	}
}

BOOL CMeshItem::LoadData()
{
	if( m_pGSFFILE == NULL || m_dwGSFOffset == 0 )
		return FALSE;

	CFile* pFile = &m_pGSFFILE->m_File;
	ULONGLONG dwOrgPosition = pFile->GetPosition();
	pFile->Seek( m_dwGSFOffset, CFile::begin );

	{
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
	}

	pFile->Seek( dwOrgPosition, CFile::begin );
	return TRUE;
}

void CMeshItem::ReleaseData()
{
	m_Mesh.Release();
}

//////////////////////////////////////////////////////////////////
// COBJTypeItem Function...
//////////////////////////////////////////////////////////////////

COBJTypeItem::COBJTypeItem()
:CItemBase(ITEM_OBJTYPE)
{
	m_mapATTR.clear();
	m_mapNAME.clear();
}

COBJTypeItem::~COBJTypeItem()
{
	MAPATTRITEM::iterator it;

	for( it = m_mapATTR.begin(); it != m_mapATTR.end(); it++)
		delete (*it).second;

	m_mapATTR.clear();
	m_mapNAME.clear();
}

void COBJTypeItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CItemBase::Load( pGSFFILE );

	{
		int nCount = 0;
		pFile->Read( &nCount, sizeof(int));

		for( int i=0; i<nCount; i++)
		{
			CString strName;
			DWORD dwID;

			pFile->Read( &dwID, sizeof(DWORD));
			strName = LoadString(pFile);
		}

		pFile->Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			pFile->Seek( sizeof(DWORD), CFile::current );
			LoadString(pFile);
			pFile->Seek( sizeof(BYTE), CFile::current );
		}

		pFile->Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
			pFile->Seek( sizeof(DWORD), CFile::current );

		pFile->Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			pFile->Seek( sizeof(DWORD), CFile::current );

			int nCL;
			pFile->Read( &nCL, sizeof(int));

			for( int j=0; j<nCL; j++)
				pFile->Seek( sizeof(DWORD), CFile::current );
		}	
	}
}


//////////////////////////////////////////////////////////////////
// COBJItem Function...
//////////////////////////////////////////////////////////////////

COBJItem::COBJItem()
:CItemBase(ITEM_OBJECT)
{
	m_OBJ.m_bPivotCount = MAX_PIVOT;
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

COBJItem::~COBJItem()
{
	MAPATTRITEM::iterator itATTR;
	MAPTEXINDEX::iterator itTI;

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

void COBJItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CItemBase::Load( pGSFFILE );
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
			dwItemID += g_dwMeshID;

		SetValidMesh(dwItemID);

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
			dwItemID += g_dwTextureID;

		SetValidTexture(dwItemID);

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
			dwItemID += g_dwAnimationID;

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
			dwItemID += g_dwSFXID;

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
		int nHEBACount = 0;
		pFile->Read( &nHEBACount, sizeof(int));
		for( int j=0 ; j < nHEBACount ; ++j )
		{
			DWORD dwHEBA = 0;
			pFile->Read( &dwHEBA, sizeof(DWORD) );
			pSFX->m_mapHideByAction.insert( MAPDWORD::value_type(dwHEBA, dwHEBA) );
		}
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
			pSND->m_dwMediaID += g_dwMediaID;

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
				pSND->m_dwMediaID += g_dwMediaID;

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
					pSND->m_dwMediaID += g_dwMediaID;

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

int COBJItem::GetPivotCount( CAnimationItem *pItem)
{
	if( pItem->m_vAniID.empty() || !pItem->m_vAniID[0] )
		return 0;

	pItem = CItemBase::GetAnimationItem(pItem->m_vAniID[0]);

	if( !pItem )
		return 0;

	return pItem->m_dwNodeCount;
}

int COBJItem::GetPivotCount()
{
	MAPDWORD::iterator it;

	for( it = m_mapANI.begin(); it != m_mapANI.end(); it++)
	{
		CAnimationItem *pItem = CItemBase::GetAnimationItem((*it).second);

		if(pItem)
			return GetPivotCount(pItem);
	}

	for( it = m_mapMESH.begin(); it != m_mapMESH.end(); it++)
	{
		CMeshItem *pItem = CItemBase::GetMeshItem((*it).second);

		if(pItem)
			return pItem->m_dwNodeCount;
	}

	return -1;
}


//////////////////////////////////////////////////////////////////
// CSFXItem Function...
//////////////////////////////////////////////////////////////////

CSFXItem::CSFXItem()
:CItemBase(ITEM_SFX)
{
	m_dwTotalFrame = TOTALFRAME;
	m_dwSANISRC = 0;
	m_dwANISRC = 0;
	m_dwSFXSRC = 0;
	m_dwCurCLK = 0;

	m_vSFXID.clear();

	m_pSFX = NULL;
	m_pGSFFILE = NULL;
	m_dwGSFOffset = 0;
}

CSFXItem::~CSFXItem()
{
	m_vSFXID.clear();
	ReleaseData();
}

void CSFXItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CItemBase::Load( pGSFFILE );
	m_mapSFXItem.insert( MAPITEM::value_type( m_dwID, this));

	m_pGSFFILE = pGSFFILE;
	m_dwGSFOffset = pFile->GetPosition();

	{
		int nCount = 0;
		pFile->Read( &nCount, sizeof(int));

		for( int i=0; i<nCount; i++)
		{
			DWORD dwID;

			pFile->Read( &dwID, sizeof(DWORD));
			if( m_bMerge && dwID )
				dwID += g_dwSFXID;

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
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );
		}

		BYTE bSRCType = 0;
		pFile->Seek( sizeof(BYTE), CFile::current );
		pFile->Read( &bSRCType, sizeof(BYTE));

		if(m_bMerge)
		{
			if(m_dwSANISRC)
				m_dwSANISRC += g_dwAnimationID;

			if(m_dwANISRC)
				m_dwANISRC += g_dwAnimationID;

			if(m_dwSFXSRC)
			{
				switch( bSRCType )
				{
				case SFXSRC_TEX	: m_dwSFXSRC += g_dwTextureID; break;
				case SFXSRC_OBJ	: m_dwSFXSRC += g_dwOBJID; break;
				case SFXSRC_SFX	: m_dwSFXSRC += g_dwSFXID; break;
				}

				if( bSRCType == SFXSRC_TEX )
					SetValidTexture(m_dwSFXSRC);
			}
		}

		pFile->Seek( sizeof(FLOAT), CFile::current );
		pFile->Seek( sizeof(FLOAT), CFile::current );
		pFile->Seek( sizeof(FLOAT), CFile::current );

		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );

		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );

		pFile->Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );
		}

		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(BYTE), CFile::current );
		pFile->Seek( sizeof(BYTE), CFile::current );
		pFile->Seek( sizeof(BYTE), CFile::current );
		pFile->Seek( sizeof(BYTE), CFile::current );
		pFile->Seek( sizeof(BYTE), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );

		pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
		pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );

		pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
		pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
		pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );
	}
}

BOOL CSFXItem::LoadData()
{
	if( m_pGSFFILE == NULL || m_dwGSFOffset == 0 )
		return FALSE;

	CFile* pFile = &m_pGSFFILE->m_File;
	ULONGLONG dwOrgPosition = pFile->GetPosition();
	pFile->Seek( m_dwGSFOffset, CFile::begin );

	m_pSFX = new SFX;
    
	{
		int nCount = 0;
		pFile->Read( &nCount, sizeof(int));

		for( int i=0; i<nCount; i++)
			pFile->Seek( sizeof(DWORD), CFile::current );

		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );

		pFile->Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			LPCLRKEY pKEY = new CLRKEY();

			pFile->Read( &pKEY->m_dwColor, sizeof(DWORD));
			pFile->Read( &pKEY->m_dwTick, sizeof(DWORD));

			m_pSFX->m_vKey.push_back(pKEY);
		}

		pFile->Read( &m_pSFX->m_bSFXType, sizeof(BYTE));
		pFile->Read( &m_pSFX->m_bSRCType, sizeof(BYTE));

		pFile->Read( &m_pSFX->m_fFollow, sizeof(FLOAT));
		pFile->Read( &m_pSFX->m_fSizeX, sizeof(FLOAT));
		pFile->Read( &m_pSFX->m_fSizeY, sizeof(FLOAT));

		pFile->Read( &m_pSFX->m_dwDESTBlend, sizeof(DWORD));
		pFile->Read( &m_pSFX->m_dwSRCBlend, sizeof(DWORD));
		pFile->Read( &m_pSFX->m_dwBlendOP, sizeof(DWORD));

		pFile->Read( &m_pSFX->m_dwPLifeTime, sizeof(DWORD));
		pFile->Read( &m_pSFX->m_dwLifeTime, sizeof(DWORD));

		pFile->Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			LPCLKINST pCI = new CLKINST();
			DWORD dwID;

			pFile->Read( &dwID, sizeof(DWORD));
			pFile->Read( &pCI->m_dwCurMESH, sizeof(DWORD));
			pFile->Read( &pCI->m_dwCurCL, sizeof(DWORD));

			m_pSFX->m_mapCLK.insert( MAPCLKINST::value_type( dwID, pCI));
		}

		pFile->Read( &m_pSFX->m_dwOBJFuncID, sizeof(DWORD));
		pFile->Read( &m_pSFX->m_dwACTFuncID, sizeof(DWORD));
		pFile->Read( &m_pSFX->m_dwANIFuncID, sizeof(DWORD));
		pFile->Read( &m_pSFX->m_dwACT, sizeof(DWORD));
		pFile->Read( &m_pSFX->m_dwANI, sizeof(DWORD));
		pFile->Read( &m_pSFX->m_bSprayType, sizeof(BYTE));
		pFile->Read( &m_pSFX->m_bZEnable, sizeof(BYTE));
		pFile->Read( &m_pSFX->m_bZWrite, sizeof(BYTE));
		pFile->Read( &m_pSFX->m_bStrict, sizeof(BYTE));
		pFile->Read( &m_pSFX->m_bFollow, sizeof(BYTE));
		pFile->Read( &m_pSFX->m_dwGTime, sizeof(DWORD));
		pFile->Read( &m_pSFX->m_dwGCnt, sizeof(DWORD));

		pFile->Read( &m_pSFX->m_vOutBound, sizeof(D3DXVECTOR3));
		pFile->Read( &m_pSFX->m_vInBound, sizeof(D3DXVECTOR3));
		pFile->Read( &m_pSFX->m_dwERRTime, sizeof(DWORD));

		pFile->Read( &m_pSFX->m_vERRPos, sizeof(D3DXVECTOR3));
		pFile->Read( &m_pSFX->m_vERRRot, sizeof(D3DXVECTOR3));
		pFile->Read( &m_pSFX->m_vERRScale, sizeof(D3DXVECTOR3));
		pFile->Read( &m_pSFX->m_dwLodLevel, sizeof(DWORD));
	}

	pFile->Seek( dwOrgPosition, CFile::begin );
	return TRUE;
}

void CSFXItem::ReleaseData()
{
	if( m_pSFX )
		delete m_pSFX;

	m_pSFX = NULL;
}

//////////////////////////////////////////////////////////////////
// CMAPItem Function...
//////////////////////////////////////////////////////////////////

CMAPItem::CMAPItem( BYTE bType)
:CItemBase(bType)
{
	m_mapLIGHT.clear();
	m_mapOBJ.clear();
	m_mapSFX.clear();
}

CMAPItem::~CMAPItem()
{
	MAPMAPLIGHT::iterator itLIGHT;

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		delete (*itLIGHT).second;

	m_mapLIGHT.clear();
	m_mapOBJ.clear();
	m_mapSFX.clear();
}

void CMAPItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CItemBase::Load( pGSFFILE );
	m_mapMAPItem.insert( MAPITEM::value_type( m_dwID, this));
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

	m_pHugeMAP = new CTachyonHUGEMAP;
}

CEXTMAPItem::~CEXTMAPItem()
{
	ReleaseData();
}

void CEXTMAPItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CItemBase::Load( pGSFFILE );
	m_mapMAPItem.insert( MAPITEM::value_type( m_dwID, this));

	pFile->Read( &m_pHugeMAP->m_nUnitCountX, sizeof(int));
	pFile->Read( &m_pHugeMAP->m_nUnitCountZ, sizeof(int));
	pFile->Read( &m_pHugeMAP->m_nUnitLength, sizeof(int));
	pFile->Read( &m_pHugeMAP->m_nTileLength, sizeof(int));
	pFile->Read( &m_pHugeMAP->m_nCellCount, sizeof(int));
	pFile->Read( &m_pHugeMAP->m_nTextureSize, sizeof(int));
	pFile->Read( &m_pHugeMAP->m_nShadowPixel, sizeof(int));
	pFile->Read( &m_pHugeMAP->m_vLIGHT, sizeof(D3DXVECTOR3));

	pFile->Read( &m_pHugeMAP->m_bUseSHADER, sizeof(BYTE));
	pFile->Read( &m_pHugeMAP->m_fIntensity, sizeof(FLOAT));
	pFile->Read( &m_pHugeMAP->m_fSpecRange, sizeof(FLOAT));
	pFile->Read( &m_pHugeMAP->m_fSpecMax, sizeof(FLOAT));

	m_pHugeMAP->m_pENABLE = new BYTE[m_pHugeMAP->m_nUnitCountX * m_pHugeMAP->m_nUnitCountZ];
	m_vDETAILS.clear();
	m_vDETAIL.clear();
	m_vGEO.clear();

	LPBYTE pUNIT = new BYTE[DEF_UNITSIZE];
	LPBYTE pOBJ = new BYTE[DEF_OBJSIZE];

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
			dwDetailID += g_dwTextureID;

		SetValidTexture(dwDetailID);

		m_vDETAIL.push_back(dwDetailID);
		m_vDETAILS.push_back(fDetailS);
	}

	GetTempFileName(
		CItemBase::m_szTempPath,
		_T("_EM"),
		0, szTempFile);

	CFile unit( szTempFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	m_strUNIT = unit.GetFilePath();

	GetTempFileName(
		CItemBase::m_szTempPath,
		_T("_EO"),
		0, szTempFile);

	CFile obj( szTempFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	m_strOBJ = obj.GetFilePath();

	nCount = m_pHugeMAP->m_nUnitCountX * m_pHugeMAP->m_nUnitCountZ;
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
				*pItemID += g_dwOBJID;

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
				*pItemID += g_dwSFXID;

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
				*pItemID += g_dwMediaID;

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
		pFile->Read( m_pHugeMAP->m_pENABLE, nCount * sizeof(BYTE));

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
			pGEOTEX->m_dwTEXID += g_dwTextureID;

		m_vGEO.push_back(pGEOTEX);
	}
}

void CEXTMAPItem::ReleaseData()
{
	while(!m_vGEO.empty())
	{
		delete m_vGEO.back();
		m_vGEO.pop_back();
	}

	if(!m_strUNIT.IsEmpty())
		DeleteFile(m_strUNIT);

	if(!m_strOBJ.IsEmpty())
		DeleteFile(m_strOBJ);

	if( m_pHugeMAP )
		delete m_pHugeMAP;

	m_pHugeMAP = NULL;

	m_vDETAILS.clear();
	m_vDETAIL.clear();

	m_fMaxHeight = 0.0f;
	m_fMinHeight = 0.0f;
}

void CEXTMAPItem::Blend( LPDIRECT3DTEXTURE9 pTARGET,
						 LPDIRECT3DTEXTURE9 pIMG,
						 FLOAT fPosX,
						 FLOAT fPosZ)
{
	D3DXMATRIX vWORLD(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	LPDIRECT3DSURFACE9 pDEST = NULL;
	LPDIRECT3DSURFACE9 pBACK = NULL;
	LPDIRECT3DSURFACE9 pZBUF = NULL;
	CD3DCamera vCAMERA;

	D3DVIEWPORT9 vNEW = {
		0, 0,
		m_pHugeMAP->m_nTextureSize,
		m_pHugeMAP->m_nTextureSize,
		0.0f,
		1.0f};
	D3DVIEWPORT9 vOLD;

	vWORLD._41 = fPosX / m_pHugeMAP->m_nUnitLength - 0.5f;
	vWORLD._43 = 0.5f - fPosZ / m_pHugeMAP->m_nUnitLength;

	vCAMERA.InitOrthoCamera(
		m_Device.m_pDevice,
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

	m_Device.m_pDevice->GetDepthStencilSurface(&pZBUF);
	m_Device.m_pDevice->GetRenderTarget( 0, &pBACK);
	m_Device.m_pDevice->GetViewport(&vOLD);

	pTARGET->GetSurfaceLevel( 0, &pDEST);
	m_Device.m_pDevice->SetRenderTarget( 0, pDEST);
	m_Device.m_pDevice->SetDepthStencilSurface(NULL);
	m_Device.m_pDevice->SetViewport(&vNEW);
	m_Device.m_pDevice->BeginScene();

	m_Device.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	m_Device.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_Device.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_Device.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_Device.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	m_Device.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_Device.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	m_Device.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_Device.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	m_Device.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_Device.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_Device.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	m_Device.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_Device.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_Device.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	m_Device.m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
	m_Device.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	m_Device.m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	m_Device.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	m_Device.m_pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(PVERTEX));
	m_Device.m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);
	m_Device.m_pDevice->SetFVF(T3DFVF_PVERTEX);

	m_Device.m_pDevice->SetRenderState(
		D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_BLUE|
		D3DCOLORWRITEENABLE_GREEN|
		D3DCOLORWRITEENABLE_RED);

	m_Device.m_pDevice->SetTexture( 0, pIMG);
	m_Device.m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	m_Device.m_pDevice->EndScene();

	m_Device.m_pDevice->SetRenderState(
		D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_ALPHA|
		D3DCOLORWRITEENABLE_BLUE|
		D3DCOLORWRITEENABLE_GREEN|
		D3DCOLORWRITEENABLE_RED);

	m_Device.m_pDevice->SetRenderTarget( 0, pBACK);
	m_Device.m_pDevice->SetDepthStencilSurface(pZBUF);
	m_Device.m_pDevice->SetViewport(&vOLD);

	pDEST->Release();
	pBACK->Release();
	pZBUF->Release();
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

		if(pITEM)
		{
			LPMAPSFX pMAPSFX = new MAPSFX();

			pSFXID->insert( MAPDWORD::value_type(
				dwID,
				dwItemID));
			
			//pMAPSFX->m_pSFX = &pITEM->m_SFX; // jkchoi

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

		if(pITEM)
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

void CEXTMAPItem::LoadUNIT( LPDIRECT3DTEXTURE9 *pSHADE,
							LPDIRECT3DTEXTURE9 *pSKIN,
							LPDIRECT3DTEXTURE9 *pTILE,
							LPVECTORTEXTURE pSHADOW,
							LPBYTE pREGION,
							LPBYTE pDETAIL,
							FLOAT *pHEIGHT,
							DWORD dwIndex,
							BOOL bSYSMEM)
{
	CFile file( m_strUNIT, CFile::modeRead|CFile::typeBinary);
	DWORD dwCount;

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

		file.Read( pDATA, dwCount);
		D3DXCreateTextureFromFileInMemoryEx(
			m_Device.m_pDevice,
			pDATA,
			dwCount,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			1, 0,
			bSYSMEM ? D3DFMT_A8R8G8B8 : D3DFMT_DXT3,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_FILTER_NONE,
			0, NULL,
			NULL,
			pSHADE);

		delete[] pDATA;
	}
	else
		file.Seek( dwCount, CFile::current);

	file.Read( &dwCount, sizeof(DWORD));
	if( pSKIN && dwCount )
	{
		LPBYTE pDATA = new BYTE[dwCount];

		file.Read( pDATA, dwCount);
		D3DXCreateTextureFromFileInMemoryEx(
			m_Device.m_pDevice,
			pDATA,
			dwCount,
			D3DX_DEFAULT,
			D3DX_DEFAULT, 1,
			bSYSMEM ? 0 : D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			bSYSMEM ? D3DPOOL_SYSTEMMEM : D3DPOOL_DEFAULT,
			D3DX_FILTER_NONE,
			D3DX_FILTER_NONE,
			0, NULL,
			NULL,
			pSKIN);

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

		file.Read( pDATA, dwCount);
		D3DXCreateTextureFromFileInMemoryEx(
			m_Device.m_pDevice,
			pDATA,
			dwCount,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			1, 0,
			D3DFMT_DXT1,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_FILTER_NONE,
			0, NULL,
			NULL,
			pTILE);

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
			LPDIRECT3DTEXTURE9 pTEX = NULL;

			file.Read( pDATA, dwLength);
			D3DXCreateTextureFromFileInMemoryEx(
				m_Device.m_pDevice,
				pDATA,
				dwLength,
				D3DX_DEFAULT,
				D3DX_DEFAULT, 1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT,
				D3DX_FILTER_NONE,
				D3DX_FILTER_NONE,
				0, NULL,
				NULL,
				&pTEX);

			pSHADOW->push_back(pTEX);
			delete[] pDATA;
		}
		else
			file.Seek( dwLength, CFile::current);
	}
}

void CEXTMAPItem::BuildSREGION( LPVECTORTEXTURE pSHADOW,
							    LPWORD pSREGION)
{
	int nWidth = m_pHugeMAP->m_nUnitLength * m_pHugeMAP->m_nShadowPixel;
	int nCount = INT(pSHADOW->size());

	memset( pSREGION, 0, m_pHugeMAP->m_nUnitLength * m_pHugeMAP->m_nUnitLength * sizeof(WORD));
	nWidth = nWidth / SHADOWTEX_SIZE + (nWidth % SHADOWTEX_SIZE ? 1 : 0);

	for( int i=0; i<nCount; i++)
	{
		int nBlockX = (i % nWidth) * SHADOWTEX_SIZE / m_pHugeMAP->m_nShadowPixel;
		int nBlockZ = (i / nWidth) * SHADOWTEX_SIZE / m_pHugeMAP->m_nShadowPixel;

		LPT3DDATA pDXT = CD3DImage::CreateDXT(
			m_Device.m_pDevice,
			(*pSHADOW)[i], DXT1);

		LPDIRECT3DTEXTURE9 pIMG = NULL;
		D3DLOCKED_RECT lr;

		D3DXCreateTextureFromFileInMemoryEx(
			m_Device.m_pDevice,
			pDXT->m_pData,
			pDXT->m_dwSize,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			1, 0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_SYSTEMMEM,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			NULL,
			NULL,
			NULL,
			&pIMG);

		pIMG->LockRect( 0, &lr, NULL, 0);
		LPBYTE pDATA = (LPBYTE) lr.pBits;

		for( int j=0; j<SHADOWTEX_SIZE; j++)
		{
			int nTileZ = nBlockZ + (SHADOWTEX_SIZE - j - 1) / m_pHugeMAP->m_nShadowPixel;
			int nResZ = (SHADOWTEX_SIZE - j - 1) % m_pHugeMAP->m_nShadowPixel;

			LPDWORD pCOLOR = (LPDWORD) pDATA;

			for( int k=0; k<SHADOWTEX_SIZE; k++)
			{
				if(((*pCOLOR) & 0xFF000000))
				{
					int nTileX = nBlockX + k / m_pHugeMAP->m_nShadowPixel;
					int nResX = k % m_pHugeMAP->m_nShadowPixel;

					int nTileIndex = nTileZ * m_pHugeMAP->m_nUnitLength + nTileX;
					int nResIndex = nResZ * m_pHugeMAP->m_nShadowPixel + nResX;

					pSREGION[nTileIndex] |= (0x0001 << nResIndex);
				}

				pCOLOR++;
			}

			pDATA += lr.Pitch;
		}

		pIMG->UnlockRect(0);
		pIMG->Release();

		delete pDXT;
	}
}


//////////////////////////////////////////////////////////////////
// CBSPMAPItem Function...
//////////////////////////////////////////////////////////////////

CBSPMAPItem::CBSPMAPItem()
:CMAPItem(ITEM_BSPMAP)
{
	m_bMAPType = MT_BSP;

	m_mapRegionID.clear();
	m_vTEX.clear();

	m_dwMediaOffsetID = g_dwMediaID;
	m_dwTexOffsetID = g_dwTextureID;
	m_dwObjOffsetID = g_dwOBJID;
	m_dwSfxOffsetID = g_dwSFXID;

	m_strPATH.Empty();
	ClearDATA();
}

CBSPMAPItem::~CBSPMAPItem()
{
	ReleaseData();
}

void CBSPMAPItem::SeekBSPNode( CFile* pFile )
{
	if(!pFile)
		return ;

	BYTE bFront = 0;
	BYTE bBack = 0;

	pFile->Seek( sizeof(DWORD), CFile::current );
	pFile->Read( &bFront, sizeof(BYTE) );
	pFile->Read( &bBack, sizeof(BYTE) );
	pFile->Seek( sizeof(D3DXPLANE), CFile::current );

	int nCount = 0;
	pFile->Read( &nCount, sizeof(nCount));

	for( int i=0; i<nCount; i++)
	{
		int nPoint = 0;

		pFile->Read( &nPoint, sizeof(int));
		if( nPoint > 0 )
			pFile->Seek( 10 * nPoint * sizeof(FLOAT), CFile::current);

		pFile->Seek( sizeof(D3DXPLANE), CFile::current );
		pFile->Seek( sizeof(int), CFile::current );
	}

	pFile->Read( &nCount, sizeof(nCount));
	for( i=0; i<nCount; i++)
	{
		pFile->Seek( sizeof(DWORD), CFile::current );

		BYTE bData = 0;
		pFile->Read( &bData, sizeof(BYTE) );

		if(bData)
		{
			int nPointCount = 0;
			int nFrontPVS = 0;
			int nBackPVS = 0;

			pFile->Seek( sizeof(D3DXPLANE), CFile::current);
			pFile->Seek( sizeof(BYTE), CFile::current);
			pFile->Seek( sizeof(DWORD), CFile::current);
			pFile->Seek( sizeof(DWORD), CFile::current);
			pFile->Seek( sizeof(DWORD), CFile::current);

			pFile->Read( &nPointCount, sizeof(int));
			pFile->Read( &nFrontPVS, sizeof(int));
			pFile->Read( &nBackPVS, sizeof(int));

			for( int j=0; j<nPointCount; j++)
				pFile->Seek( sizeof(D3DXVECTOR3), CFile::current);

			for( j=0; j<nFrontPVS; j++)
				pFile->Seek( sizeof(DWORD), CFile::current );

			for( j=0; j<nBackPVS; j++)
				pFile->Seek( sizeof(DWORD), CFile::current );
		}
	}

	pFile->Read( &nCount, sizeof(nCount));
	for( i=0; i<nCount; i++)
	{
		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );

		int nWINDING = 0;
		pFile->Read( &nWINDING, sizeof(int));
		for( int j=0; j<nWINDING; j++)
		{
			int nPoint;

			pFile->Seek( sizeof(D3DXPLANE), CFile::current );
			pFile->Seek( sizeof(int), CFile::current );
			pFile->Read( &nPoint, sizeof(int));

			if( nPoint > 0 )
				pFile->Seek( 10 * nPoint * sizeof(FLOAT), CFile::current);
		}
	}

	if(bFront)
		SeekBSPNode( pFile );

	if(bBack)
		SeekBSPNode( pFile );
}

void CBSPMAPItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CMAPItem::Load( pGSFFILE );

	m_pGSFFILE = pGSFFILE;
	m_dwGSFOffset = pFile->GetPosition();

	{
		DWORD dwLength;
		pFile->Read( &dwLength, sizeof(DWORD));

		int nCount;
		pFile->Read( &nCount, sizeof(int));
		for( int i=0; i<nCount; i++)
		{
			DWORD dwTextureID = 0;
			
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
			pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
			pFile->Read( &dwTextureID, sizeof(DWORD));
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(FLOAT), CFile::current );
			pFile->Seek( sizeof(FLOAT), CFile::current );
			pFile->Seek( sizeof(BYTE), CFile::current );
			pFile->Seek( sizeof(BYTE), CFile::current );
			pFile->Seek( sizeof(BYTE), CFile::current );

			if(m_bMerge)
				dwTextureID += m_dwTexOffsetID;

			SetValidTexture(dwTextureID);
		}

		pFile->Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );

			pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
			pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
			pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );

			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );

			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );

			int nCLK;
			pFile->Read( &nCLK, sizeof(int));
			for( int j=0; j<nCLK; j++)
			{
				pFile->Seek( sizeof(DWORD), CFile::current );
				pFile->Seek( sizeof(DWORD), CFile::current );
				pFile->Seek( sizeof(DWORD), CFile::current );
			}

			int nATTR;
			pFile->Read( &nATTR, sizeof(int));
			for( j=0; j<nATTR; j++)
			{
				pFile->Seek( sizeof(DWORD), CFile::current );
				pFile->Seek( sizeof(BYTE), CFile::current );

				int nSize = 0;
				pFile->Read( &nSize, sizeof(int));
				pFile->Seek( nSize, CFile::current );
			}
		}

		pFile->Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
			pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
			pFile->Seek( sizeof(BYTE), CFile::current );
		}

		pFile->Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(D3DXVECTOR3), CFile::current );
		}

		pFile->Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			int nTEXCount;
			pFile->Read( &nTEXCount, sizeof(int));

			for( int j=0; j<nTEXCount; j++)
			{
				pFile->Seek( sizeof(DWORD), CFile::current);

				DWORD dwTexID;
				pFile->Read( &dwTexID, sizeof(DWORD));

				if(m_bMerge)
					dwTexID += m_dwTexOffsetID;

				SetValidTexture(dwTexID);
			}
		}

		pFile->Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
		{
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );
		}

		pFile->Seek( sizeof(DWORD), CFile::current);

		// Load CTachyonBSPMAP
		{
			int nCount;
			pFile->Read( &nCount, sizeof(int));

			for( int i=0; i<nCount; i++)
			{
				pFile->Seek( sizeof(DWORD), CFile::current );
				pFile->Seek( sizeof(D3DLIGHT9), CFile::current );
			}

			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(BYTE), CFile::current );

			CBSPMAPItem::SeekBSPNode( pFile );
		}
	}
}

BOOL CBSPMAPItem::LoadData()
{
	if( m_pGSFFILE == NULL || m_dwGSFOffset == 0 )
		return FALSE;

	CFile* pFile = &m_pGSFFILE->m_File;
	ULONGLONG dwOrgPosition = pFile->GetPosition();
	pFile->Seek( m_dwGSFOffset, CFile::begin );

	{
		DWORD dwLength;

		char szTempFile[_MAX_PATH];

		GetTempFileName(
			CItemBase::m_szTempPath,
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

		LoadDATA();
		ClearDATA();
	}

	pFile->Seek( dwOrgPosition, CFile::begin );
	return TRUE;
}

void CBSPMAPItem::ReleaseData()
{
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

void CBSPMAPItem::LoadDATA()
{
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

		if(m_bMerge)
			pLIGHT->m_dwTextureID += m_dwTexOffsetID;
		SetValidTexture(pLIGHT->m_dwTextureID);

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

		if(m_bMerge)
			dwItemID += m_dwObjOffsetID;

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

		if(m_bMerge)
			dwItemID += m_dwSfxOffsetID;

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

		if(m_bMerge)
			pMAPSND->m_dwMediaID += m_dwMediaOffsetID;
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

			if(m_bMerge)
				dwTexID += m_dwTexOffsetID;
			SetValidTexture(dwTexID);

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

	file.Seek( sizeof(DWORD), CFile::current);
	m_BSPMAP.Load(&file);

	if(m_BSPMAP.m_pRegionID)
		delete[] m_BSPMAP.m_pRegionID;

	m_BSPMAP.m_pRegionID = new BYTE[m_BSPMAP.m_dwMeshCount];
	memset( m_BSPMAP.m_pRegionID, 0x00, m_BSPMAP.m_dwMeshCount * sizeof(BYTE));

	MAPDWORD::iterator itID = m_mapRegionID.begin();
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
}

void CBSPMAPItem::ClearDATA()
{
	MAPMAPLIGHT::iterator itLIGHT;

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		delete (*itLIGHT).second;

	m_mapLIGHT.clear();
	m_mapOBJ.clear();
	m_mapSFX.clear();

	while(!m_vTEX.empty())
	{
		m_vTEX.back()->clear();
		delete m_vTEX.back();

		m_vTEX.pop_back();
	}

	m_mapRegionID.clear();
	m_BSPMAP.Clear();
}


//////////////////////////////////////////////////////////////////
// CGuideItem Function...
//////////////////////////////////////////////////////////////////

CGuideItem::CGuideItem( BYTE bType)
:CItemBase(bType)
{
	m_vGuideH.clear();
	m_vGuideV.clear();
}

CGuideItem::~CGuideItem()
{
	m_vGuideH.clear();
	m_vGuideV.clear();
}

void CGuideItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CItemBase::Load( pGSFFILE );

	{
		int nCount = 0;

		pFile->Read( &nCount, sizeof(int));
		for( int i=0; i<nCount; i++)
			pFile->Seek( sizeof(int), CFile::current );

		pFile->Read( &nCount, sizeof(int));
		for( i=0; i<nCount; i++)
			pFile->Seek( sizeof(int), CFile::current );
	}
}


//////////////////////////////////////////////////////////////////
// CCOMPItem Function...
//////////////////////////////////////////////////////////////////

CCOMPItem::CCOMPItem()
:CGuideItem(ITEM_COMP)
{
	m_vCOMP.clear();

	m_dwDefaultID = 0;
	m_dwDisableID = 0;

	m_bCtrlType = CT_NONE;
	m_nHMargine = 0;
	m_nVMargine = 0;
	m_nLevel = 0;
	m_nWidth = 0;
	m_nHeight = 0;
}

CCOMPItem::~CCOMPItem()
{
	while(!m_vCOMP.empty())
	{
		delete m_vCOMP.back();
		m_vCOMP.pop_back();
	}
}

void CCOMPItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CGuideItem::Load( pGSFFILE );
	m_mapCompItem.insert( MAPITEM::value_type( m_dwID, this));

	//if( bLoadData )
	{
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
				pComp->m_dwCompID += g_dwCompID;

			m_vCOMP.push_back(pComp);
		}

		pFile->Read( &m_dwDefaultID, sizeof(DWORD));
		pFile->Read( &m_dwDisableID, sizeof(DWORD));
		pFile->Read( &m_bCtrlType, sizeof(BYTE));

		if( m_bMerge && m_dwDefaultID )
			m_dwDefaultID += g_dwImageID;

		if( m_bMerge && m_dwDisableID )
			m_dwDisableID += g_dwImageID;

		pFile->Read( &m_nHMargine, sizeof(int));
		pFile->Read( &m_nVMargine, sizeof(int));
		pFile->Read( &m_nLevel, sizeof(int));
		pFile->Read( &m_nWidth, sizeof(int));
		pFile->Read( &m_nHeight, sizeof(int));
	}
	/*else
	{
		int nCount = 0;
		pFile->Read( &nCount, sizeof(int));

		for( int i=0; i<nCount; i++)
		{
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(BYTE), CFile::current );
			pFile->Seek( sizeof(int), CFile::current );
			pFile->Seek( sizeof(int), CFile::current );
		}

		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(BYTE), CFile::current );
		pFile->Seek( sizeof(int), CFile::current );
		pFile->Seek( sizeof(int), CFile::current );
		pFile->Seek( sizeof(int), CFile::current );
		pFile->Seek( sizeof(int), CFile::current );
		pFile->Seek( sizeof(int), CFile::current );
	}*/
}


//////////////////////////////////////////////////////////////////
// CFRAMEItem Function...
//////////////////////////////////////////////////////////////////

CFRAMEItem::CFRAMEItem()
:CGuideItem(ITEM_FRAME)
{
	m_vCTRL.clear();

	m_strFontID.Empty();
	m_dwDefaultID = 0;
	m_dwDisableID = 0;

	m_nWidth = 0;
	m_nHeight = 0;
}

CFRAMEItem::~CFRAMEItem()
{
	while(!m_vCTRL.empty())
	{
		delete m_vCTRL.back();
		m_vCTRL.pop_back();
	}
}

void CFRAMEItem::Load( LPTGSFFILE pGSFFILE )
{
	CFile* pFile = &pGSFFILE->m_File;

	CGuideItem::Load( pGSFFILE );
	m_mapFrameItem.insert( MAPITEM::value_type( m_dwID, this));

	//if( bLoadData )
	{
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
				pCtrl->m_dwCompID += g_dwCompID;

			m_vCTRL.push_back(pCtrl);
		}
		m_strFontID = LoadString(pFile);

		pFile->Read( &m_dwDefaultID, sizeof(DWORD));
		pFile->Read( &m_dwDisableID, sizeof(DWORD));

		if( m_bMerge && m_dwDefaultID )
			m_dwDefaultID += g_dwImageID;

		if( m_bMerge && m_dwDisableID )
			m_dwDisableID += g_dwImageID;

		pFile->Read( &m_nWidth, sizeof(int));
		pFile->Read( &m_nHeight, sizeof(int));
	}
	/*else
	{
		int nCount = 0;
		pFile->Read( &nCount, sizeof(int));

		for( int i=0; i<nCount; i++)
		{
			for( int j=0; j<TCML_MENU_COUNT; j++)
				LoadString(pFile);

			CString strCtrlID = LoadString(pFile);
			LoadString(pFile);
			LoadString(pFile);
			LoadString(pFile);
			LoadString(pFile);

			if( m_bMerge && strCtrlID.IsEmpty() )
				RegLocalID( strCtrlID, this);

			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(DWORD), CFile::current );
			pFile->Seek( sizeof(BYTE), CFile::current );
			pFile->Seek( sizeof(int), CFile::current );
			pFile->Seek( sizeof(int), CFile::current );
		}

		LoadString(pFile);

		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(DWORD), CFile::current );
		pFile->Seek( sizeof(int), CFile::current );
		pFile->Seek( sizeof(int), CFile::current );
	}*/
}
