// Game StudioDoc.cpp : implementation of the CGameStudioDoc class
//

#include "stdafx.h"
#include "Game Studio.h"
#include <stack>

#include "Game StudioView.h"
#include "Game StudioDoc.h"
#include "UpdateDlg.h"
#include "BSPMAPDoc.h"
#include "EXTMAPDoc.h"
#include "MainFrm.h"

#include "BuildShadowDlg.h"
#include "AddAnimationDlg.h"
#include "AddHeightMapDlg.h"
#include "AddTextureDlg.h"
#include "AddBSPMapDlg.h"
#include "AddMediaDlg.h"
#include "AddMeshDlg.h"
#include "AddMapDlg.h"
#include "EditUNITDlg.h"
#include ".\game studiodoc.h"
#include "SetImportOffsetDlg.h"
#include "EditEnableUNITDlg.h"
#include "EXTMAPBar.h"
#include "SelectFormatDlg.h"
#include "TextureFrame.h"
#include "DetailTextureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameStudioDoc

CPreviewDlg *CGameStudioDoc::m_pPreview = NULL;


IMPLEMENT_DYNCREATE(CGameStudioDoc, CDocument)

BEGIN_MESSAGE_MAP(CGameStudioDoc, CDocument)
	ON_COMMAND(ID_ITEM_NEWPROJECT, OnItemNewproject)
	ON_COMMAND(ID_CANCEL, OnCancel)
	ON_COMMAND(ID_ITEM_NEWFOLDER, OnItemNewfolder)
	ON_COMMAND(ID_ITEM_ADD, OnItemAdd)
	ON_COMMAND(ID_ITEM_DELETE, OnItemDelete)
	ON_COMMAND(ID_EXP_GEO, OnExpGeo)
	ON_COMMAND(ID_IMP_GEO, OnImpGeo)
	ON_COMMAND(ID_EXP_OBJ, OnExpObj)
	ON_COMMAND(ID_IMP_OBJ, OnImpObj)
	ON_COMMAND(ID_MAP_EDITDETAIL, OnEditDetail)
	ON_COMMAND(ID_ITEM_UPDATE, OnItemUpdate)
	ON_COMMAND(ID_EXP_BSP, OnExpBsp)
	ON_COMMAND(ID_IMP_BSP, OnImpBsp)
	ON_COMMAND(ID_EXP_BSPOBJ, OnExpBspobj)
	ON_COMMAND(ID_IMP_BSPOBJ, OnImpBspobj)
	ON_COMMAND(ID_MAP_ENABLEUNIT, OnEditEnableUNIT)
	ON_COMMAND(ID_ITEM_EXPORT, OnItemExport)
	ON_COMMAND(ID_EXT_BUILDTEXTURESHADOW, OnExtBuildtextureshadow)
	ON_COMMAND(ID_ITEM_BATCHFORMATCHANGE, OnItemBatchformatchange)
	ON_UPDATE_COMMAND_UI(ID_ITEM_BATCHFORMATCHANGE, OnUpdateItemBatchformatchange)
	ON_COMMAND(ID_EXPORTITEMINFOLDER, OnExportiteminfolder)
	ON_UPDATE_COMMAND_UI(ID_EXPORTITEMINFOLDER, OnUpdateExportItemInFolder)
	ON_COMMAND(ID_FOLDER_LISTUP, OnFolderListup)
END_MESSAGE_MAP()


// CGameStudioDoc construction/destruction

CGameStudioDoc::CGameStudioDoc()
: m_pSrcItem(NULL)
{
	m_bType = 0xFF;
}

CGameStudioDoc::~CGameStudioDoc()
{
}

BOOL CGameStudioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}




// CGameStudioDoc serialization

void CGameStudioDoc::Serialize( CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}


// CGameStudioDoc diagnostics

#ifdef _DEBUG
void CGameStudioDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGameStudioDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGameStudioDoc commands

void CGameStudioDoc::Clear()
{
	GetTreeCtrl().DeleteAllItems();
}

void CGameStudioDoc::SaveData( LPFILETHREADPARAM lParam)
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetRootItem();

	int nSaveCount = 0;
	int nCount = 0;

	while(itemPos)
	{
		itemPos = cTree.GetNextSiblingItem(itemPos);
		nCount++;
	}

	lParam->m_pFile->Write( &nCount, sizeof(int));
	itemPos = cTree.GetRootItem();

	while(itemPos)
	{
		SaveData( lParam, itemPos, &nSaveCount);
		itemPos = cTree.GetNextSiblingItem(itemPos);
	}
}

void CGameStudioDoc::SaveData( LPFILETHREADPARAM lParam,
							   HTREEITEM itemPos,
							   int *pCount)
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemChild = cTree.GetChildItem(itemPos);
	int nCount = 0;

	while(itemChild)
	{
		itemChild = cTree.GetNextSiblingItem(itemChild);
		nCount++;
	}
	CItemBase *pItem = (CItemBase *) cTree.GetItemData(itemPos);

	lParam->m_pFile->Write( &nCount, sizeof(int));
	pItem->Save(lParam->m_pFile);
	(*pCount)++;

	lParam->m_pWnd->m_bPercent = (BYTE) (*pCount) * 100 / cTree.GetCount();
	lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
	lParam->m_pWnd->SendMessage( WM_COMMAND, ID_PROGRESS_UPDATE);

	itemChild = cTree.GetChildItem(itemPos);
	while(itemChild)
	{
		SaveData( lParam, itemChild, pCount);
		itemChild = cTree.GetNextSiblingItem(itemChild);
	}
}

void CGameStudioDoc::LoadData( LPFILETHREADPARAM lParam)
{
	int nCount = 0;

	lParam->m_pFile->Read( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		int nChild = 0;

		lParam->m_pFile->Read( &nChild, sizeof(int));
		CItemBase *pItem = CItemBase::CreateItem(lParam->m_pFile);

		if(!pItem)
		{
			CString strTitle;
			CString strText;

			strTitle.LoadString(IDS_ERR_TITLE);
			strText.LoadString(IDS_ERR_INVALID_FILE);

			lParam->m_pOwner->MessageBox(
				strText,
				strTitle,
				MB_ICONHAND|
				MB_OK);

			return;
		}

		if(CItemBase::m_bMerge)
		{
			CTreeCtrl& cTree = GetTreeCtrl();

			if(!CanBeInsert( NULL, cTree, pItem->m_strName))
				pItem->m_strName = NewItemName( NULL, cTree, pItem->m_strName);
		}

		AddItem( NULL, pItem);
		BOOL bResult = LoadData(
			lParam,
			pItem->m_itemPos,
			nChild);

		if(!bResult)
		{
			CString strTitle;
			CString strText;

			strTitle.LoadString(IDS_ERR_TITLE);
			strText.LoadString(IDS_ERR_INVALID_FILE);

			lParam->m_pOwner->MessageBox(
				strText,
				strTitle,
				MB_ICONHAND|
				MB_OK);

			return;
		}
	}
}

BOOL CGameStudioDoc::LoadData( LPFILETHREADPARAM lParam,
							   HTREEITEM itemPos,
							   int nCount)
{
	for( int i=0; i<nCount; i++)
	{
		int nChild = 0;

		lParam->m_pFile->Read( &nChild, sizeof(int));
		CItemBase *pItem = CItemBase::CreateItem(lParam->m_pFile);

		if(!pItem)
			return FALSE;

		if(CItemBase::m_bMerge)
		{
			CTreeCtrl& cTree = GetTreeCtrl();

			if(!CanBeInsert( itemPos, cTree, pItem->m_strName))
				pItem->m_strName = NewItemName( itemPos, cTree, pItem->m_strName);
		}

		AddItem( itemPos, pItem);
		BOOL bResult = LoadData(
			lParam,
			pItem->m_itemPos,
			nChild);

		if(!bResult)
			return FALSE;

		lParam->m_pWnd->m_bPercent = (BYTE) (((FLOAT) lParam->m_pFile->GetPosition()) * 100.0f / ((FLOAT) lParam->m_pFile->GetLength()));
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage( WM_COMMAND, ID_PROGRESS_UPDATE);
	}

	return TRUE;
}

CGameStudioView* CGameStudioDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	return (CGameStudioView *) GetNextView(pos);
}

CTreeCtrl& CGameStudioDoc::GetTreeCtrl()
{
	return GetView()->GetTreeCtrl();
}

void CGameStudioDoc::AddItem( HTREEITEM itemPos, CItemBase *pItem)
{
	CTreeCtrl& cTree = GetTreeCtrl();
	
	pItem->m_itemPos = cTree.InsertItem(
		pItem->m_strName,
		pItem->GetIconIndex(),
		pItem->GetIconIndex() + 1,
		itemPos);

	cTree.SetItemData(
		pItem->m_itemPos,
		(DWORD_PTR) pItem);
}

BOOL CGameStudioDoc::CanBeInsert( HTREEITEM itemPos,
								  CTreeCtrl& cTree,
								  CString strName)
{
	if(itemPos)
		itemPos = cTree.GetChildItem(itemPos);
	else
		itemPos = cTree.GetRootItem();

	while(itemPos)
	{
		CItemBase *pItem = (CItemBase *) cTree.GetItemData(itemPos);

		if( pItem->m_strName == strName )
			return FALSE;

		itemPos = cTree.GetNextSiblingItem(itemPos);
	}

	return TRUE;
}

BOOL CGameStudioDoc::IsAnimationFile( CString strFileName)
{
	CString strExt;
	CTShell shell;

	strExt = shell.GetFileExt(strFileName);
	strExt.MakeUpper();
	strExt.TrimRight();
	strExt.TrimLeft();

	return strExt == _T(".TAF");
}

BOOL CGameStudioDoc::IsMeshFile( CString strFileName)
{
	CString strExt;
	CTShell shell;

	strExt = shell.GetFileExt(strFileName);
	strExt.MakeUpper();
	strExt.TrimRight();
	strExt.TrimLeft();

	return strExt == _T(".TMF");
}

BOOL CGameStudioDoc::IsBSPOBJFile( CString strFileName)
{
	CString strExt;
	CTShell shell;

	strExt = shell.GetFileExt(strFileName);
	strExt.MakeUpper();
	strExt.TrimRight();
	strExt.TrimLeft();

	return strExt == _T(".TBO");
}

BOOL CGameStudioDoc::IsOBJFile( CString strFileName)
{
	CString strExt;
	CTShell shell;

	strExt = shell.GetFileExt(strFileName);
	strExt.MakeUpper();
	strExt.TrimRight();
	strExt.TrimLeft();

	return strExt == _T(".TMO");
}

BOOL CGameStudioDoc::IsGEOFile( CString strFileName)
{
	CString strExt;
	CTShell shell;

	strExt = shell.GetFileExt(strFileName);
	strExt.MakeUpper();
	strExt.TrimRight();
	strExt.TrimLeft();

	return strExt == _T(".TMG");
}

BOOL CGameStudioDoc::IsImageFile( CString strFileName)
{
	CString strExt;
	CTShell shell;

	strExt = shell.GetFileExt(strFileName);
	strExt.MakeUpper();
	strExt.TrimRight();
	strExt.TrimLeft();

	return strExt == _T(".TGA") ||
		strExt == _T(".PNG") ||
		strExt == _T(".BMP") ||
		strExt == _T(".JPG") ||
		strExt == _T(".DDS");
}

BOOL CGameStudioDoc::IsMediaFile( CString strFileName)
{
	CString strEXT;
	CTShell shell;

	strEXT = shell.GetFileExt(strFileName);
	strEXT.MakeUpper();
	strEXT.TrimRight();
	strEXT.TrimLeft();

	return strEXT == _T(".MPA") ||
		strEXT == _T(".MP2") ||
		strEXT == _T(".MP3") ||
		strEXT == _T(".AU") ||
		strEXT == _T(".AIF") ||
		strEXT == _T(".AIFF") ||
		strEXT == _T(".SND") ||
		strEXT == _T(".MID") ||
		strEXT == _T(".MIDI") ||
		strEXT == _T(".RMI") ||
		strEXT == _T(".AVI") ||
		strEXT == _T(".QT") ||
		strEXT == _T(".MOV") ||
		strEXT == _T(".MPG") ||
		strEXT == _T(".MPEG") ||
		strEXT == _T(".M1V") ||
		strEXT == _T(".DAT") ||
		strEXT == _T(".WAV");
}

CString CGameStudioDoc::NewItemName( HTREEITEM itemPos,
									 CTreeCtrl& cTree,
									 DWORD dwFormatID)
{
	CString strResult;
	DWORD dwCount = 0;

	do strResult.Format( dwFormatID, dwCount++);
	while(!CanBeInsert( itemPos, cTree, strResult));

	return strResult;
}

CString CGameStudioDoc::NewItemName( HTREEITEM itemPos,
									 CTreeCtrl& cTree,
									 CString strHeader)
{
	CString strResult;
	DWORD dwCount = 0;

	do strResult.Format( "%s[%d]", strHeader, dwCount++);
	while(!CanBeInsert( itemPos, cTree, strResult));

	return strResult;
}

void CGameStudioDoc::OnCancel()
{
}

void CGameStudioDoc::OnItemNewproject()
{
	CProjectItem *pItem = new CProjectItem();
	CTreeCtrl& cTree = GetTreeCtrl();

	pItem->m_strName = NewItemName(
		NULL,
		cTree,
		IDS_FMT_NEWPRJ);

	AddItem(
		NULL,
		pItem);

	if(pItem->m_itemPos)
		cTree.SelectItem(pItem->m_itemPos);
}

void CGameStudioDoc::OnItemNewfolder()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();
	CFolderItem *pItem = new CFolderItem();

	pItem->m_strName = NewItemName(
		itemPos,
		cTree,
		IDS_FMT_NEWFOLDER);

	AddItem(
		itemPos,
		pItem);

	if(pItem->m_itemPos)
		cTree.SelectItem(pItem->m_itemPos);
}

void CGameStudioDoc::OnItemUpdate()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	switch(m_bType)
	{
	case ITEM_TEXTURE	:
		{
			CUpdateDlg dlg;

			dlg.m_strFilter.LoadString(IDS_FILTER_TEXFILE);
			dlg.m_strEXT.LoadString(IDS_EXT_TEXFILE);
			dlg.m_strTitle.LoadString(IDS_UPDATETEXTURE);

			if( dlg.DoModal() == IDOK )
			{
				CTextureItem *pItem = (CTextureItem *) cTree.GetItemData(itemPos);

				CItemBase::TextureItemUpdate(
					pItem,
					dlg.m_strFileName );
			}
		}

		break;

	case ITEM_IMAGE		:
		{
			CUpdateDlg dlg;

			dlg.m_strFilter.LoadString(IDS_FILTER_TEXFILE);
			dlg.m_strEXT.LoadString(IDS_EXT_TEXFILE);
			dlg.m_strTitle.LoadString(IDS_UPDATEIMAGE);

			if( dlg.DoModal() == IDOK )
			{
				CImageItem *pItem = (CImageItem *) cTree.GetItemData(itemPos);

				CItemBase::ImageItemUpdate(
					pItem,
					dlg.m_strFileName);
			}
		}

		break;

	case ITEM_MEDIA		:
		{
			CUpdateDlg dlg;

			dlg.m_strFilter.LoadString(IDS_FILTER_MEDIAFILE);
			dlg.m_strEXT.LoadString(IDS_EXT_MEDIAFILE);
			dlg.m_strTitle.LoadString(IDS_UPDATEMEDIA);

			if( dlg.DoModal() == IDOK )
			{
				CMediaItem *pItem = (CMediaItem *) cTree.GetItemData(itemPos);

				CItemBase::MediaItemUpdate(
					pItem,
					dlg.m_strFileName);
			}
		}

		break;

	case ITEM_ANIMATION	:
		{
			CUpdateDlg dlg;

			dlg.m_strFilter.LoadString(IDS_FILTER_ANIFILE);
			dlg.m_strEXT.LoadString(IDS_EXT_ANIFILE);
			dlg.m_strTitle.LoadString(IDS_UPDATEANI);

			if( dlg.DoModal() == IDOK )
			{
				CAnimationItem *pItem = (CAnimationItem *) cTree.GetItemData(itemPos);

				CItemBase::AnimationItemUpdate(
					pItem,
					dlg.m_strFileName);
			}
		}

		break;

	case ITEM_MESH		:
		{
			CUpdateDlg dlg;

			dlg.m_strFilter.LoadString(IDS_FILTER_MESHFILE);
			dlg.m_strEXT.LoadString(IDS_EXT_ANIFILE);
			dlg.m_strTitle.LoadString(IDS_UPDATEMESH);

			if( dlg.DoModal() == IDOK )
			{
				CMeshItem *pItem = (CMeshItem *) cTree.GetItemData(itemPos);

				CItemBase::MeshItemUpdate(
					pItem,
					dlg.m_strFileName);
			}
		}

		break;
	}
}

void CGameStudioDoc::OnItemExport()
{
	if( m_bType != ITEM_IMAGE &&
		m_bType != ITEM_TEXTURE )
		return;

	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CFileDialog dlgFile(
		FALSE,
		_T("*.tga"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Image File (*.tga)|*.tga|All Files (*.*)|*.*||"));

	if( dlgFile.DoModal() != IDOK )
		return;

	switch(m_bType)
	{
	case ITEM_IMAGE:
		ExportImage( dlgFile.GetPathName(), (CImageItem*)cTree.GetItemData(itemPos) );
		break;

	case ITEM_TEXTURE:
		ExportTexture( dlgFile.GetPathName(), (CTextureItem*)cTree.GetItemData(itemPos) );
		break;
	}
}

void CGameStudioDoc::ExportImage(const CString& strFile, CImageItem* pItem, D3DXIMAGE_FILEFORMAT eFormat)
{
	pItem->m_Image.SaveImageFile(
		CMainFrame::m_3DDevice.m_pDevice, 
		strFile,
		eFormat);
}

void CGameStudioDoc::ExportTexture(const CString& strFile, CTextureItem* pItem, D3DXIMAGE_FILEFORMAT eFormat)
{
	if( pItem->GetLPDIRECT3DTEXTURE9() )
	{
		D3DXSaveTextureToFile(
			(LPCSTR)strFile,
			eFormat,
			pItem->GetLPDIRECT3DTEXTURE9(), 
			NULL);
	}
}

void CGameStudioDoc::OnItemAdd()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	switch(m_bType)
	{
	case ITEM_TEXTURE	:
		{
			CAddTextureDlg dlg;
			CTShell shell;

			dlg.m_strTitle.LoadString(IDS_ADDTEXTURE);
			if( dlg.DoModal() == IDOK )
			{
				theApp.BeginWaitCursor();

				switch(dlg.m_nLoadType)
				{
				case DEF_LOADTYPE_FILE		:
					{
						CString strItemName = shell.GetFileTitle(dlg.m_strFileName);

						if(!CanBeInsert( itemPos, cTree, strItemName))
							strItemName = NewItemName( itemPos, cTree, strItemName);

						AddTextureItem(
							itemPos,
							strItemName,
							dlg.m_strFileName);
					}

					break;

				case DEF_LOADTYPE_FOLDER	:
					{
						AddTextureFolder(
							itemPos,
							cTree,
							dlg.m_strFolderName);
					}

					break;

				case DEF_LOADTYPE_SPRITE	:
					{
						AddTextureItem(
							itemPos,
							NewItemName(
							itemPos,
							cTree,
							IDS_FMT_NEWTEXTURE), _T(""));
					}

					break;
				}

				theApp.EndWaitCursor();
			}
		}

		break;

	case ITEM_IMAGE		:
		{
			CAddTextureDlg dlg;
			CTShell shell;

			dlg.m_strTitle.LoadString(IDS_ADDIMAGE);
			if( dlg.DoModal() == IDOK )
			{
				theApp.BeginWaitCursor();

				switch(dlg.m_nLoadType)
				{
				case DEF_LOADTYPE_FILE		:
					{
						CString strItemName = shell.GetFileTitle(dlg.m_strFileName);

						if(!CanBeInsert( itemPos, cTree, strItemName))
							strItemName = NewItemName( itemPos, cTree, strItemName);

						AddImageItem(
							itemPos,
							strItemName,
							dlg.m_strFileName);
					}

					break;

				case DEF_LOADTYPE_FOLDER	:
					{
						AddImageFolder(
							itemPos,
							cTree,
							dlg.m_strFolderName);
					}

					break;

				case DEF_LOADTYPE_SPRITE	:
					{
						AddImageItem(
							itemPos,
							NewItemName(
							itemPos,
							cTree,
							IDS_FMT_NEWIMAGE), _T(""));
					}

					break;
				}

				theApp.EndWaitCursor();
			}
		}

		break;

	case ITEM_MEDIA		:
		{
			CAddMediaDlg dlg;
			CTShell shell;

			if( dlg.DoModal() == IDOK )
			{
				theApp.BeginWaitCursor();

				switch(dlg.m_nLoadType)
				{
				case DEF_LOADTYPE_FILE		:
					{
						CString strItemName = shell.GetFileTitle(dlg.m_strFileName);

						if(!CanBeInsert( itemPos, cTree, strItemName))
							strItemName = NewItemName( itemPos, cTree, strItemName);

						AddMediaItem(
							itemPos,
							strItemName,
							dlg.m_strFileName);
					}

					break;

				case DEF_LOADTYPE_FOLDER	:
					{
						AddMediaFolder(
							itemPos,
							cTree,
							dlg.m_strFolderName);
					}

					break;
				}

				theApp.EndWaitCursor();
			}
		}

		break;

	case ITEM_ANIMATION	:
		{
			CAddAnimationDlg dlg;
			CTShell shell;

			if( dlg.DoModal() == IDOK )
			{
				theApp.BeginWaitCursor();

				switch(dlg.m_nLoadType)
				{
				case DEF_LOADTYPE_FILE		:
					{
						CString strItemName = shell.GetFileTitle(dlg.m_strFileName);

						if(!CanBeInsert( itemPos, cTree, strItemName))
							strItemName = NewItemName( itemPos, cTree, strItemName);

						AddAnimationItem(
							itemPos,
							strItemName,
							dlg.m_strFileName);
					}

					break;

				case DEF_LOADTYPE_FOLDER	:
					{
						AddAnimationFolder(
							itemPos,
							cTree,
							dlg.m_strFolderName);
					}

					break;

				case DEF_LOADTYPE_SPRITE	:
					{
						AddAnimationItem(
							itemPos,
							NewItemName(
							itemPos,
							cTree,
							IDS_FMT_NEWANIMATION), _T(""));
					}

					break;
				}

				theApp.EndWaitCursor();
			}
		}

		break;

	case ITEM_MESH		:
		{
			CAddMeshDlg dlg;
			CTShell shell;

			if( dlg.DoModal() == IDOK )
			{
				theApp.BeginWaitCursor();

				switch(dlg.m_nLoadType)
				{
				case DEF_LOADTYPE_FILE		:
					{
						CString strItemName = shell.GetFileTitle(dlg.m_strFileName);

						if(!CanBeInsert( itemPos, cTree, strItemName))
							strItemName = NewItemName( itemPos, cTree, strItemName);

						AddMeshItem(
							itemPos,
							strItemName,
							dlg.m_strFileName);
					}

					break;

				case DEF_LOADTYPE_FOLDER	:
					{
						AddMeshFolder(
							itemPos,
							cTree,
							dlg.m_strFolderName);
					}

					break;
				}

				theApp.EndWaitCursor();
			}
		}

		break;

	case ITEM_OBJECT	:
		{
			CItemBase *pItem = (CItemBase *) cTree.GetItemData(itemPos);

			if( pItem->m_bType != ITEM_OBJTYPE )
			{
				pItem = new COBJTypeItem();

				pItem->m_strName = NewItemName(
					itemPos,
					cTree,
					IDS_FMT_NEWOBJTYPE);

				AddItem( itemPos, pItem);
				cTree.SelectItem(pItem->m_itemPos);
			}
			else
				AddOBJItem( itemPos, NewItemName( itemPos, cTree, IDS_FMT_NEWOBJECT));
		}

		break;

	case ITEM_SFX		:
		{
			CSFXItem *pItem = new CSFXItem();

			pItem->m_strName = NewItemName( itemPos, cTree, IDS_FMT_NEWSFX);
			pItem->Regulate();
			AddItem( itemPos, pItem);

			cTree.SelectItem(pItem->m_itemPos);
		}

		break;

	case ITEM_EXTMAP	:
		{
			CAddMapDlg dlg;

			if( dlg.DoModal() == IDOK )
				if( dlg.m_nType == MT_EXT )
				{
					CAddHeightMapDlg dlg;

					if( dlg.DoModal() == IDOK )
					{
						CEXTMAPItem *pItem = new CEXTMAPItem();

						pItem->m_strName = NewItemName( itemPos, cTree, IDS_FMT_NEWEXTMAP);
						pItem->m_strID = CItemBase::GetNewID(IDS_FMT_NEWEXTMAPID);
						pItem->Build(
							dlg.m_strFile,
							D3DXVECTOR3(
							(FLOAT) atof(dlg.m_strLightX),
							(FLOAT) atof(dlg.m_strLightY),
							(FLOAT) atof(dlg.m_strLightZ)),
							(FLOAT) atof(dlg.m_strMinH),
							(FLOAT) atof(dlg.m_strMaxH),
							dlg.m_bCell,
							dlg.m_bUNIT,
							dlg.m_dwUnit,
							dlg.m_dwCountX,
							dlg.m_dwCountY);
						pItem->Regulate();

						CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( pItem->m_strID, pItem->m_dwID));
						AddItem( itemPos, pItem);

						cTree.SelectItem(pItem->m_itemPos);
					}
				}
				else
				{
					CAddBSPMapDlg dlg;

					if( dlg.DoModal() == IDOK )
					{
						CTachyonDoc::CloseAllBSP();

						CTBSPPortal::m_bBasePortalID = CBSPMAPItem::GetNextBaseID();
						CTBSPNode::m_bBaseNodeID = CTBSPPortal::m_bBasePortalID;
						CBSPMAPItem *pItem = new CBSPMAPItem();

						pItem->m_strName = NewItemName( itemPos, cTree, IDS_FMT_NEWBSPMAP);
						pItem->m_strID = CItemBase::GetNewID(IDS_FMT_NEWBSPMAPID);
						pItem->m_BSPMAP.m_bPVS = dlg.m_bPVS;
						pItem->Build(dlg.m_strFile);

						CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( pItem->m_strID, pItem->m_dwID));
						AddItem( itemPos, pItem);

						cTree.SelectItem(pItem->m_itemPos);
					}
				}
		}

		break;

	case ITEM_COMP		:
		{
			CCOMPItem *pItem = new CCOMPItem();

			pItem->m_strName = NewItemName( itemPos, cTree, IDS_FMT_NEWCOMP);
			pItem->m_strID = CItemBase::GetNewID(IDS_FMT_NEWCOMPID);
			pItem->Regulate();

			CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( pItem->m_strID, pItem->m_dwID));
			AddItem( itemPos, pItem);

			cTree.SelectItem(pItem->m_itemPos);
		}

		break;

	case ITEM_FRAME		:
		{
			CFRAMEItem *pItem = new CFRAMEItem();

			pItem->m_strName = NewItemName( itemPos, cTree, IDS_FMT_NEWFRAME);
			pItem->m_strID = CItemBase::GetNewID(IDS_FMT_NEWFRAMEID);
			pItem->Regulate();

			CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( pItem->m_strID, pItem->m_dwID));
			AddItem( itemPos, pItem);

			cTree.SelectItem(pItem->m_itemPos);
		}

		break;
	}
}

void CGameStudioDoc::AddOBJItem( HTREEITEM itemPos, CString strItemName)
{
	COBJTypeItem *pTYPE = (COBJTypeItem *) GetTreeCtrl().GetItemData(itemPos);
	COBJItem *pItem = new COBJItem();

	pItem->m_strID = CItemBase::GetNewID(IDS_FMT_NEWOBJ);
	pItem->m_strName = strItemName;
	CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( pItem->m_strID, pItem->m_dwID));

	MAPATTRITEM::iterator itATTR;
	MAPACTION::iterator itACT;
	MAPCLKIND::iterator itCLK;
	MAPSTR::iterator itSTR;

	for( itATTR = pTYPE->m_mapATTR.begin(); itATTR != pTYPE->m_mapATTR.end(); itATTR++)
	{
		LPATTRITEM pATTR = new ATTRITEM();

		pATTR->m_strValue = (*itATTR).second->m_strValue;
		pATTR->m_bType = (*itATTR).second->m_bType;

		pItem->m_mapATTR.insert( MAPATTRITEM::value_type( (*itATTR).first, pATTR));
	}

	for( itACT = pTYPE->m_OBJ.m_mapACTION.begin(); itACT != pTYPE->m_OBJ.m_mapACTION.end(); itACT++)
	{
		LPACTION pACTION = new ACTION();
		pItem->m_OBJ.m_mapACTION.insert( MAPACTION::value_type( (*itACT).first, pACTION));
	}

	for( itCLK = pTYPE->m_OBJ.m_mapCLKIND.begin(); itCLK != pTYPE->m_OBJ.m_mapCLKIND.end(); itCLK++)
	{
		LPCLKIND pCLK = new CLKIND();
		MAPCLOTH::iterator itCL;

		pItem->m_OBJ.m_mapCLKIND.insert( MAPCLKIND::value_type( (*itCLK).first, pCLK));
		for( itCL = (*itCLK).second->m_mapCLOTH.begin(); itCL != (*itCLK).second->m_mapCLOTH.end(); itCL++)
		{
			LPCLOTH pCLOTH = new CLOTH();
			pCLK->m_mapCLOTH.insert( MAPCLOTH::value_type( (*itCL).first, pCLOTH));
		}
	}

	for( itSTR = pTYPE->m_mapNAME.begin(); itSTR != pTYPE->m_mapNAME.end(); itSTR++)
	{
		pItem->m_mapNAME.insert( MAPSTR::value_type( (*itSTR).first, (*itSTR).second));
		CItemBase::RegLocalID( (*itSTR).second, pItem);
	}

	pItem->Regulate();
	AddItem( itemPos, pItem);
	GetTreeCtrl().SelectItem(pItem->m_itemPos);
}

void CGameStudioDoc::AddMeshItem( HTREEITEM itemPos,
								  CString strItemName,
								  CString strFileName)
{
	CMeshItem *pItem = new CMeshItem();
	pItem->m_strName = strItemName;

	CFile file( strFileName, CFile::modeRead);
	DWORD dwSize = (DWORD) file.GetLength();

	LPBYTE pData = new BYTE[dwSize];
	file.Read( pData, dwSize);

	pItem->m_Mesh.LoadMesh(
		CMainFrame::m_3DDevice.m_pDevice,
		pData);

	delete[] pData;
	pData = NULL;

	pItem->Regulate();
	AddItem( itemPos, pItem);
}

void CGameStudioDoc::AddMeshFolder( HTREEITEM itemPos,
								    CTreeCtrl& cTree,
									CString strFolderName)
{
	CString strFind = strFolderName;
	CFileFind finder;

	strFolderName.TrimRight('\\');
	strFind.Format(
		"%s\\*.*",
		strFolderName);

	BOOL bFind = finder.FindFile(strFind);
	while(bFind)
	{
		bFind = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			CFolderItem *pItem = new CFolderItem();
			pItem->m_strName = finder.GetFileTitle();

			if(!CanBeInsert( itemPos, cTree, pItem->m_strName))
				pItem->m_strName = NewItemName( itemPos, cTree, pItem->m_strName);

			AddItem( itemPos, pItem);
			AddMeshFolder(
				pItem->m_itemPos,
				cTree,
				finder.GetFilePath());

			continue;
		}

		if(IsMeshFile(finder.GetFilePath()))
		{
			CString strItemName = finder.GetFileTitle();

			if(!CanBeInsert( itemPos, cTree, strItemName))
				strItemName = NewItemName( itemPos, cTree, strItemName);

			AddMeshItem(
				itemPos,
				strItemName,
				finder.GetFilePath());
		}
	}
}

void CGameStudioDoc::AddAnimationItem( HTREEITEM itemPos,
									   CString strItemName,
									   CString strFileName)
{
	CAnimationItem *pItem = new CAnimationItem();

	pItem->m_strName = strItemName;
	if(!strFileName.IsEmpty())
	{
		CFile file( strFileName, CFile::modeRead);

		pItem->LoadAnimationFromCFile( file, TRUE );

		/*// begin LoadAnimationFromCFile
		pItem->m_dwSize = (DWORD) file.GetLength();
		pItem->m_pData = new BYTE[pItem->m_dwSize];
		file.Read( pItem->m_pData, pItem->m_dwSize);

		pItem->LoadAnimation(CMainFrame::m_3DDevice.m_pDevice,
			pItem->m_pData);
		// end LoadAnimationCFile*/

		pItem->m_vAniID.push_back(pItem->m_dwID);
		pItem->m_vReverse.push_back(FALSE);

		pItem->Regulate();
	}
	else
	{
		pItem->SetNullAnimation();
	}

	AddItem( itemPos, pItem);
	if(strFileName.IsEmpty())
		GetTreeCtrl().SelectItem(pItem->m_itemPos);
}

void CGameStudioDoc::AddAnimationFolder( HTREEITEM itemPos,
										 CTreeCtrl& cTree,
										 CString strFolderName)
{
	CString strFind = strFolderName;
	CFileFind finder;

	strFolderName.TrimRight('\\');
	strFind.Format(
		"%s\\*.*",
		strFolderName);

	BOOL bFind = finder.FindFile(strFind);
	while(bFind)
	{
		bFind = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			CFolderItem *pItem = new CFolderItem();
			pItem->m_strName = finder.GetFileTitle();

			if(!CanBeInsert( itemPos, cTree, pItem->m_strName))
				pItem->m_strName = NewItemName( itemPos, cTree, pItem->m_strName);

			AddItem( itemPos, pItem);
			AddAnimationFolder(
				pItem->m_itemPos,
				cTree,
				finder.GetFilePath());

			continue;
		}

		if(IsAnimationFile(finder.GetFilePath()))
		{
			CString strItemName = finder.GetFileTitle();

			if(!CanBeInsert( itemPos, cTree, strItemName))
				strItemName = NewItemName( itemPos, cTree, strItemName);

			AddAnimationItem(
				itemPos,
				strItemName,
				finder.GetFilePath());
		}
	}
}

void CGameStudioDoc::AddMediaItem( HTREEITEM itemPos,
								   CString strItemName,
								   CString strFileName)
{
	CMediaItem *pItem = new CMediaItem();
	CString strEXT;
	CTShell shell;

	char szTempPath[_MAX_PATH];
	char szTempFile[_MAX_PATH];

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
	
	pItem->m_strName = strItemName;

	pItem->GetStudioTempPath(szTempPath);

	GetTempFileName(
		szTempPath,
		_T("_MEDIA"),
		0, szTempFile);

	CopyFile( strFileName, szTempFile, FALSE);
	memcpy( pItem->m_szFileName, szTempFile, _MAX_PATH);

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

	pItem->Regulate();
	AddItem( itemPos, pItem);
}

void CGameStudioDoc::AddMediaFolder( HTREEITEM itemPos,
									 CTreeCtrl& cTree,
									 CString strFolderName)
{
	CString strFind = strFolderName;
	CFileFind finder;

	strFolderName.TrimRight('\\');
	strFind.Format(
		"%s\\*.*",
		strFolderName);

	BOOL bFind = finder.FindFile(strFind);
	while(bFind)
	{
		bFind = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			CFolderItem *pItem = new CFolderItem();
			pItem->m_strName = finder.GetFileTitle();

			if(!CanBeInsert( itemPos, cTree, pItem->m_strName))
				pItem->m_strName = NewItemName( itemPos, cTree, pItem->m_strName);

			AddItem( itemPos, pItem);
			AddMediaFolder(
				pItem->m_itemPos,
				cTree,
				finder.GetFilePath());

			continue;
		}

		if(IsMediaFile(finder.GetFilePath()))
		{
			CString strItemName = finder.GetFileTitle();

			if(!CanBeInsert( itemPos, cTree, strItemName))
				strItemName = NewItemName( itemPos, cTree, strItemName);

			AddMediaItem(
				itemPos,
				strItemName,
				finder.GetFilePath());
		}
	}
}

void CGameStudioDoc::AddImageItem( HTREEITEM itemPos,
								   CString strItemName,
								   CString strFileName)
{
	CImageItem *pItem = new CImageItem();

	pItem->m_strName = strItemName;
	pItem->m_vImageID.clear();

	if(!strFileName.IsEmpty())
	{
		CFile file( strFileName, CFile::modeRead);

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

		pItem->m_vImageID.push_back(pItem->m_dwID);
		pItem->Regulate();
	}
	else
	{
		pItem->m_Image.Release();
		pItem->m_pData = NULL;
	}

	AddItem( itemPos, pItem);
	if(strFileName.IsEmpty())
		GetTreeCtrl().SelectItem(pItem->m_itemPos);
}

void CGameStudioDoc::AddImageFolder( HTREEITEM itemPos,
									 CTreeCtrl& cTree,
									 CString strFolderName)
{
	CString strFind = strFolderName;
	CFileFind finder;

	strFolderName.TrimRight('\\');
	strFind.Format(
		"%s\\*.*",
		strFolderName);

	BOOL bFind = finder.FindFile(strFind);
	while(bFind)
	{
		bFind = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			CFolderItem *pItem = new CFolderItem();
			pItem->m_strName = finder.GetFileTitle();

			if(!CanBeInsert( itemPos, cTree, pItem->m_strName))
				pItem->m_strName = NewItemName( itemPos, cTree, pItem->m_strName);

			AddItem( itemPos, pItem);
			AddImageFolder(
				pItem->m_itemPos,
				cTree,
				finder.GetFilePath());

			continue;
		}

		if(IsImageFile(finder.GetFilePath()))
		{
			CString strItemName = finder.GetFileTitle();

			if(!CanBeInsert( itemPos, cTree, strItemName))
				strItemName = NewItemName( itemPos, cTree, strItemName);

			AddImageItem(
				itemPos,
				strItemName,
				finder.GetFilePath());
		}
	}
}

void CGameStudioDoc::AddTextureItem( HTREEITEM itemPos,
									 CString strItemName,
									 CString strFileName)
{
	CTextureItem *pItem = new CTextureItem();

	pItem->m_strName = strItemName;
	pItem->m_vTextureID.clear();

	if(!strFileName.IsEmpty())
	{
		CFile file( strFileName, CFile::modeRead);

		pItem->LoadTextureFromCFile( file);

		pItem->m_vTextureID.push_back(pItem->m_dwID);
		pItem->Regulate();
	}
	else
	{
		pItem->SetNullTexture();
	}

	AddItem( itemPos, pItem);
	if(strFileName.IsEmpty())
		GetTreeCtrl().SelectItem(pItem->m_itemPos);
}

void CGameStudioDoc::AddTextureFolder( HTREEITEM itemPos,
									   CTreeCtrl& cTree,
									   CString strFolderName)
{
	CString strFind = strFolderName;
	CFileFind finder;

	strFolderName.TrimRight('\\');
	strFind.Format(
		"%s\\*.*",
		strFolderName);

	BOOL bFind = finder.FindFile(strFind);
	while(bFind)
	{
		bFind = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			CFolderItem *pItem = new CFolderItem();
			pItem->m_strName = finder.GetFileTitle();

			if(!CanBeInsert( itemPos, cTree, pItem->m_strName))
				pItem->m_strName = NewItemName( itemPos, cTree, pItem->m_strName);

			AddItem( itemPos, pItem);
			AddTextureFolder(
				pItem->m_itemPos,
				cTree,
				finder.GetFilePath());

			continue;
		}

		if(IsImageFile(finder.GetFilePath()))
		{
			CString strItemName = finder.GetFileTitle();

			if(!CanBeInsert( itemPos, cTree, strItemName))
				strItemName = NewItemName( itemPos, cTree, strItemName);

			AddTextureItem(
				itemPos,
				strItemName,
				finder.GetFilePath());
		}
	}
}

void CGameStudioDoc::OnItemDelete()
{
	if( AfxMessageBox( IDS_DELETE_ITEM, MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2) == IDOK )
	{
		CTreeCtrl& cTree = GetTreeCtrl();
		HTREEITEM itemPos = cTree.GetSelectedItem();

		if(itemPos)
		{
			cTree.DeleteItem(itemPos);
			cTree.Invalidate();
		}
	}
}

void CGameStudioDoc::OnEditEnableUNIT()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CEXTMAPItem *pItem = (CEXTMAPItem *) cTree.GetItemData(itemPos);
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) pItem->GetMainDocument();
	CEditUNITDlg dlg;

	dlg.InitData(pItem);
	dlg.DoModal();

	if(pDoc)
		pDoc->RefreshView();
}

LPT3DDATA CGameStudioDoc::CompressTextureFileData( LPBYTE pData, DWORD dwSize)
{
	LPDIRECT3DSURFACE9 pImage = NULL;
	D3DXIMAGE_INFO info;
	D3DFORMAT format = D3DFMT_DXT3;

	D3DXGetImageInfoFromFileInMemory(
		pData,
		dwSize,
		&info);

	info.Height = CTMath::GetNearPow(info.Height);
	info.Width = CTMath::GetNearPow(info.Width);

	CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
		info.Width,
		info.Height,
		D3DFMT_DXT3,
		D3DPOOL_SCRATCH,
		&pImage,
		NULL);

	D3DXLoadSurfaceFromFileInMemory(
		pImage,
		NULL,
		NULL,
		pData,
		dwSize,
		NULL,
		D3DX_DEFAULT,
		0, NULL);

	LPT3DDATA pResult = CD3DImage::CreateDXT(
		CMainFrame::m_3DDevice.m_pDevice,
		pImage,
		DXT3);
	pImage->Release();

	return pResult;
}

void CGameStudioDoc::OnExpGeo()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CEXTMAPItem *pItem = (CEXTMAPItem *) cTree.GetItemData(itemPos);
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) pItem->GetMainDocument();

	DWORD dwCount = pItem->m_HugeMAP.m_nUnitCountX * pItem->m_HugeMAP.m_nUnitCountZ;
	LPBYTE pENABLE = new BYTE[dwCount];
	memcpy( pENABLE, pItem->m_HugeMAP.m_pENABLE, dwCount * sizeof(BYTE));

	CEditEnableUNITDlg vUnitDlg;
	vUnitDlg.InitData(&pItem->m_HugeMAP);

	if( vUnitDlg.DoModal() == IDOK )
	{
		CFileDialog dlg(
			FALSE, _T("*.tmg"), NULL,
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			_T("Map Geometry Files (*.tmg)|*.tmg||"));
		BMPARAM Param;

		Param.m_vUNIT.clear();
		for( DWORD i=0; i<dwCount; i++)
			if(pItem->m_HugeMAP.m_pENABLE[i])
				Param.m_vUNIT.push_back(i);

		memcpy( pItem->m_HugeMAP.m_pENABLE, pENABLE, dwCount * sizeof(BYTE));
		if( dlg.DoModal() == IDOK )
		{
			if(pDoc)
				pDoc->UpdateData();
			CProgressDlg Progress;

			Param.m_strFileName = dlg.GetPathName();
			Param.m_pWnd = &Progress;
			Param.m_pHost = pItem;

			Progress.m_strCancel.LoadString(IDS_CANCEL_EXPGEO);
			Progress.m_strTitle.LoadString(IDS_EXP_GEO);
			Progress.m_pThreadFunc = (AFX_THREADPROC) CEXTMAPItem::__ExpGEO;
			Progress.m_lParam = &Param;
			Progress.DoModal();
		}
	}

	delete[] pENABLE;
	pENABLE = NULL;
}

void CGameStudioDoc::OnImpGeo()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CEXTMAPItem *pItem = (CEXTMAPItem *) cTree.GetItemData(itemPos);
	CTShell dlg;

	dlg.m_strTitle = _T("Geometry Folder");
	if( dlg.BrowseForFolder(AfxGetMainWnd()) == IDOK )
	{
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) pItem->GetMainDocument();
		CSetImportOffsetDlg vImportDlg;

		vImportDlg.m_nFromHeight = pItem->m_HugeMAP.m_nUnitCountZ * pItem->m_HugeMAP.m_nUnitLength;
		vImportDlg.m_nFromWidth = pItem->m_HugeMAP.m_nUnitCountX * pItem->m_HugeMAP.m_nUnitLength;
		vImportDlg.m_nFromLeft = 0;
		vImportDlg.m_nFromTop = 0;
		vImportDlg.m_nToLeft = 0;
		vImportDlg.m_nToTop = 0;

		if( vImportDlg.DoModal() == IDOK )
		{
			if(pDoc)
				pDoc->UpdateData();

			pItem->m_vFROM.SetRect(
				vImportDlg.m_nFromLeft,
				vImportDlg.m_nFromTop,
				vImportDlg.m_nFromLeft + vImportDlg.m_nFromWidth,
				vImportDlg.m_nFromTop + vImportDlg.m_nFromHeight);

			pItem->m_vTO.SetPoint(
				vImportDlg.m_nToLeft,
				vImportDlg.m_nToTop);

			ImpGEO( pItem, dlg.m_strPath);
			if(pDoc)
			{
				pDoc->Reload();
				pDoc->ResetDETAIL();
				pDoc->RefreshView();
			}
		}
	}
}

void CGameStudioDoc::OnExpObj()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CEXTMAPItem *pItem = (CEXTMAPItem *) cTree.GetItemData(itemPos);
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) pItem->GetMainDocument();

	DWORD dwCount = pItem->m_HugeMAP.m_nUnitCountX * pItem->m_HugeMAP.m_nUnitCountZ;
	LPBYTE pENABLE = new BYTE[dwCount];
	memcpy( pENABLE, pItem->m_HugeMAP.m_pENABLE, dwCount * sizeof(BYTE));

	CEditEnableUNITDlg vUnitDlg;
	vUnitDlg.InitData(&pItem->m_HugeMAP);

	if( vUnitDlg.DoModal() == IDOK )
	{
		CFileDialog dlg(
			FALSE,
			_T("*.tmo"),
			NULL,
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			_T("Map Object Files (*.tmo)|*.tmo||"));
		BMPARAM Param;

		Param.m_vUNIT.clear();
		for( DWORD i=0; i<dwCount; i++)
			if(pItem->m_HugeMAP.m_pENABLE[i])
				Param.m_vUNIT.push_back(i);

		memcpy( pItem->m_HugeMAP.m_pENABLE, pENABLE, dwCount * sizeof(BYTE));
		if( dlg.DoModal() == IDOK )
		{
			if(pDoc)
				pDoc->UpdateData();
			CProgressDlg Progress;

			Param.m_strFileName = dlg.GetPathName();
			Param.m_pWnd = &Progress;
			Param.m_pHost = pItem;

			Progress.m_strCancel.LoadString(IDS_CANCEL_EXPOBJ);
			Progress.m_strTitle.LoadString(IDS_EXP_OBJ);
			Progress.m_pThreadFunc = (AFX_THREADPROC) CEXTMAPItem::__ExpOBJ;
			Progress.m_lParam = &Param;
			Progress.DoModal();
		}
	}

	delete[] pENABLE;
	pENABLE = NULL;
}

void CGameStudioDoc::OnImpObj()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CEXTMAPItem *pItem = (CEXTMAPItem *) cTree.GetItemData(itemPos);
	CTShell dlg;

	dlg.m_strTitle = _T("Object Folder");
	if( dlg.BrowseForFolder(AfxGetMainWnd()) == IDOK )
	{
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) pItem->GetMainDocument();
		CSetImportOffsetDlg vImportDlg;

		vImportDlg.m_nFromHeight = pItem->m_HugeMAP.m_nUnitCountZ * pItem->m_HugeMAP.m_nUnitLength;
		vImportDlg.m_nFromWidth = pItem->m_HugeMAP.m_nUnitCountX * pItem->m_HugeMAP.m_nUnitLength;
		vImportDlg.m_nFromLeft = 0;
		vImportDlg.m_nFromTop = 0;
		vImportDlg.m_nToLeft = 0;
		vImportDlg.m_nToTop = 0;

		if( vImportDlg.DoModal() == IDOK )
		{
			if(pDoc)
				pDoc->UpdateData();

			pItem->m_vFROM.SetRect(
				vImportDlg.m_nFromLeft,
				vImportDlg.m_nFromTop,
				vImportDlg.m_nFromLeft + vImportDlg.m_nFromWidth,
				vImportDlg.m_nFromTop + vImportDlg.m_nFromHeight);

			pItem->m_vTO.SetPoint(
				vImportDlg.m_nToLeft,
				vImportDlg.m_nToTop);

			ImpOBJ( pItem, dlg.m_strPath);
			CTachyonDoc::UpdateAll(pDoc);

			if(pDoc)
			{
				pDoc->m_mapFREEZE.clear();
				pDoc->m_mapHIDE.clear();

				pDoc->Reload();
				pDoc->RefreshView();
			}
		}
	}
}

void CGameStudioDoc::OnEditDetail()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CEXTMAPItem *pItem = (CEXTMAPItem *) cTree.GetItemData(itemPos);
	CDetailTextureDlg::StartEdit( NULL, pItem);
}

void CGameStudioDoc::ImpOBJ( CEXTMAPItem *pItem, CString strFolder)
{
	CString strFind = strFolder;
	CFileFind finder;

	strFolder.TrimRight('\\');
	strFind.Format(
		"%s\\*.*",
		strFolder);
	BOOL bFind = finder.FindFile(strFind);

	while(bFind)
	{
		bFind = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			ImpOBJ( pItem, finder.GetFilePath());
			continue;
		}

		if(IsOBJFile(finder.GetFilePath()))
		{
			CProgressDlg Progress;
			BMPARAM Param;

			Param.m_strFileName = finder.GetFilePath();
			Param.m_pWnd = &Progress;
			Param.m_pHost = pItem;

			Progress.m_strCancel.LoadString(IDS_CANCEL_IMPOBJ);
			Progress.m_strTitle.LoadString(IDS_IMP_OBJ);
			Progress.m_pThreadFunc = (AFX_THREADPROC) CEXTMAPItem::__ImpOBJ;
			Progress.m_lParam = &Param;
			Progress.DoModal();
		}
	}
}

void CGameStudioDoc::ImpGEO( CEXTMAPItem *pItem, CString strFolder)
{
	CString strFind = strFolder;
	CFileFind finder;

	strFolder.TrimRight('\\');
	strFind.Format(
		"%s\\*.*",
		strFolder);
	BOOL bFind = finder.FindFile(strFind);

	while(bFind)
	{
		bFind = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			ImpGEO( pItem, finder.GetFilePath());
			continue;
		}

		if(IsGEOFile(finder.GetFilePath()))
		{
			CProgressDlg Progress;
			BMPARAM Param;

			Param.m_strFileName = finder.GetFilePath();
			Param.m_pWnd = &Progress;
			Param.m_pHost = pItem;

			Progress.m_strCancel.LoadString(IDS_CANCEL_IMPGEO);
			Progress.m_strTitle.LoadString(IDS_IMP_GEO);
			Progress.m_pThreadFunc = (AFX_THREADPROC) CEXTMAPItem::__ImpGEO;
			Progress.m_lParam = &Param;
			Progress.DoModal();
		}
	}
}

void CGameStudioDoc::OnExpBsp()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CBSPMAPItem *pItem = (CBSPMAPItem *) cTree.GetItemData(itemPos);
	CFileDialog dlg(
		FALSE,
		_T("*.tbm"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("BSP Map Files (*.tbm)|*.tbm||"));

	if( dlg.DoModal() == IDOK )
	{
		CFile file( dlg.GetPathName(), CFile::typeBinary|CFile::modeCreate|CFile::modeWrite);

		CTachyonDoc::CloseAllBSP();
		pItem->ExpBSP(&file);
	}
}

void CGameStudioDoc::OnImpBsp()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CBSPMAPItem *pItem = (CBSPMAPItem *) cTree.GetItemData(itemPos);
	CFileDialog dlg(
		TRUE,
		_T("*.tbm"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("BSP Map Files (*.tbm)|*.tbm||"));

	if( dlg.DoModal() == IDOK )
	{
		CFile file( dlg.GetPathName(), CFile::typeBinary|CFile::modeRead);

		CTachyonDoc::CloseAllBSP();
		pItem->ImpBSP(&file);
	}
}

void CGameStudioDoc::OnExpBspobj()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CBSPMAPItem *pItem = (CBSPMAPItem *) cTree.GetItemData(itemPos);
	CFileDialog dlg(
		FALSE,
		_T("*.tbo"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("BSP Object Files (*.tbo)|*.tbo||"));

	if( dlg.DoModal() == IDOK )
	{
		CFile file( dlg.GetPathName(), CFile::typeBinary|CFile::modeCreate|CFile::modeWrite);

		CTachyonDoc::CloseAllBSP();
		pItem->ExpOBJ(&file);
	}
}

void CGameStudioDoc::OnImpBspobj()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CBSPMAPItem *pItem = (CBSPMAPItem *) cTree.GetItemData(itemPos);
	CTShell dlg;

	dlg.m_strTitle = _T("Object Folder");
	if( dlg.BrowseForFolder(AfxGetMainWnd()) == IDOK )
	{
		CTachyonDoc::CloseAllBSP();
		pItem->LoadDATA();

		ImpBSPOBJ( pItem, dlg.m_strPath);
		pItem->SaveDATA();
		pItem->ClearDATA();
	}
}

void CGameStudioDoc::ImpBSPOBJ( CBSPMAPItem *pItem, CString strFolder)
{
	CString strFind = strFolder;
	CFileFind finder;

	strFolder.TrimRight('\\');
	strFind.Format(
		"%s\\*.*",
		strFolder);

	BOOL bFind = finder.FindFile(strFind);
	while(bFind)
	{
		bFind = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			ImpBSPOBJ( pItem, finder.GetFilePath());
			continue;
		}

		if(IsBSPOBJFile(finder.GetFilePath()))
		{
			CProgressDlg Progress;
			BMPARAM Param;

			Param.m_strFileName = finder.GetFilePath();
			Param.m_pWnd = &Progress;
			Param.m_pHost = pItem;

			Progress.m_strCancel.LoadString(IDS_CANCEL_IMPOBJ);
			Progress.m_strTitle.LoadString(IDS_IMP_OBJ);
			Progress.m_pThreadFunc = (AFX_THREADPROC) CBSPMAPItem::__ImpOBJ;
			Progress.m_lParam = &Param;
			Progress.DoModal();
		}
	}
}
// 06/05/18 PSY
void CGameStudioDoc::AddCopyItem(void)
{	
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();
	CItemBase* pCurItem = (CItemBase*)cTree.GetItemData(itemPos);
	if( !m_pSrcItem || ( pCurItem->m_bType != ITEM_FOLDER && pCurItem->m_bType != ITEM_PROJECT ) )
		return;
	
	CItemBase* pItem;

	switch( m_pSrcItem->m_bType )
	{
	case ITEM_FRAME:
		pItem = new CFRAMEItem();				
		pItem->m_strName = NewItemName( itemPos, cTree, m_pSrcItem->m_strName );
		pItem->m_strID = CItemBase::GetNewID(IDS_FMT_NEWFRAMEID);
		pItem->CopyItem(m_pSrcItem);
		pItem->Regulate();
		CItemBase::RegLocalID( pItem->m_strID, pItem);
		AddItem( itemPos, pItem);		
		cTree.SelectItem(pItem->m_itemPos);
		break;

	case ITEM_SFX:
		pItem = new CSFXItem();		
		pItem->m_strName = NewItemName( itemPos, cTree, m_pSrcItem->m_strName );
		pItem->CopyItem(m_pSrcItem);
		pItem->Regulate();
		AddItem( itemPos, pItem);
		cTree.SelectItem(pItem->m_itemPos);
		break;
	}
}

HTREEITEM CGameStudioDoc::FindItemByID( DWORD dwID )
{
	CTreeCtrl& TreeCtrl = GetTreeCtrl();

	std::stack<HTREEITEM> stItem;

	HTREEITEM hRoot = TreeCtrl.GetRootItem();
	stItem.push( hRoot );

	while(1)
	{
		hRoot = TreeCtrl.GetNextItem( hRoot, TVGN_NEXT );
		if( hRoot != NULL )
			stItem.push( hRoot );
		else
			break;
	}
	
	while( !stItem.empty() )
	{
		HTREEITEM hCurItem = stItem.top();
		stItem.pop();

		if( TreeCtrl.ItemHasChildren( hCurItem ) )
		{
			HTREEITEM hChildItem = TreeCtrl.GetChildItem( hCurItem );
			stItem.push( hChildItem );

			while(1)
			{
				HTREEITEM hNextItem = TreeCtrl.GetNextItem(hChildItem,TVGN_NEXT);

				if( hNextItem == NULL )
					break;

				stItem.push( hNextItem );
				hChildItem = hNextItem;
			}
		}
		else
		{
			CItemBase* pItem = (CItemBase*)TreeCtrl.GetItemData( hCurItem );

			if( pItem->m_bMenuID != 1 && // 폴더가 아니라면
				pItem->m_dwID == dwID ) // 아이디가 맞다면
			{
				return hCurItem;
			}
		}
	}

	return NULL;
}

CFRAMEItem* CGameStudioDoc::FindFrameItemByID( CString strID )
{
	if( m_bType != ITEM_FRAME )
		return NULL;

	CTreeCtrl& TreeCtrl = GetTreeCtrl();

	std::stack<HTREEITEM> stItem;

	HTREEITEM hRoot = TreeCtrl.GetRootItem();
	stItem.push( hRoot );

	while(1)
	{
		hRoot = TreeCtrl.GetNextItem( hRoot, TVGN_NEXT );
		if( hRoot != NULL )
			stItem.push( hRoot );
		else
			break;
	}
	
	while( !stItem.empty() )
	{
		HTREEITEM hCurItem = stItem.top();
		stItem.pop();

		if( TreeCtrl.ItemHasChildren( hCurItem ) )
		{
			HTREEITEM hChildItem = TreeCtrl.GetChildItem( hCurItem );
			stItem.push( hChildItem );

			while(1)
			{
				HTREEITEM hNextItem = TreeCtrl.GetNextItem(hChildItem,TVGN_NEXT);

				if( hNextItem == NULL )
					break;

				stItem.push( hNextItem );
				hChildItem = hNextItem;
			}
		}
		else
		{
			CItemBase* pItem = (CItemBase*)TreeCtrl.GetItemData( hCurItem );
			CFRAMEItem* pFrameItem = (CFRAMEItem *) pItem;
			if( pItem->m_bMenuID != 1 && // 폴더가 아니라면
				pFrameItem->m_strID == strID )
			{
				return pFrameItem;
			}
		}
	}

	return NULL;
}

void CGameStudioDoc::OnExtBuildtextureshadow()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CEXTMAPItem *pItem = (CEXTMAPItem *) cTree.GetItemData(itemPos);
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) pItem->GetMainDocument();
	CBuildShadowDlg dlg;

	dlg.m_bShadowPixel = (BYTE) pItem->m_HugeMAP.m_nShadowPixel;
	if( dlg.DoModal() == IDOK )
	{
		if(pDoc)
			pDoc->UpdateData();

		CProgressDlg Progress;
		BMPARAM Param;

		Param.m_vUNIT.push_back(dlg.m_bShadowPixel);
		Param.m_pWnd = &Progress;
		Param.m_pHost = pItem;

		Progress.m_strCancel.LoadString(IDS_CANCEL_BUILD_SHADOWTEX);
		Progress.m_strTitle.LoadString(IDS_BUILD_SHADOWTEX);
		Progress.m_pThreadFunc = (AFX_THREADPROC) CEXTMAPItem::__BuildSHADOWTEX;
		Progress.m_lParam = &Param;
		Progress.DoModal();
	}
}

void IteratingInFolderForChangeFormat(
	HTREEITEM itemPos,
	CTreeCtrl& cTrr,
	BYTE bFormat)
{
	HTREEITEM hIterator = itemPos;

	do
	{
		CItemBase* pItem = (CItemBase*)cTrr.GetItemData( hIterator);
		if( pItem == NULL )
			return ;

		switch( pItem->m_bType )
		{
		case ITEM_TEXTURE:
			{
				CTextureItem* pTexItem = (CTextureItem*)pItem;
				pTexItem->m_bTextureFormat = bFormat;
			}
			break;

		case ITEM_IMAGE:
			{
				CImageItem* pImgItem = (CImageItem*)pItem;
				pImgItem->m_bImageFormat = bFormat;
			}
			break;

		case ITEM_PROJECT:
		case ITEM_FOLDER:
			{
				HTREEITEM childItem = cTrr.GetChildItem( hIterator);
				if( childItem != NULL )
				{
					IteratingInFolderForChangeFormat(
						childItem,
						cTrr,
						bFormat);
				}
			}
			break;
		}
	}
	while(  hIterator = cTrr.GetNextItem(hIterator,TVGN_NEXT) );
}

void CGameStudioDoc::ChangeFormatInFolder(
	HTREEITEM itemPos,
	CTreeCtrl& cTrr,
	BYTE bFormat)
{
	IteratingInFolderForChangeFormat( itemPos, cTrr, bFormat);
	CTachyonDoc::UpdateAll(NULL);
}

void CGameStudioDoc::OnItemBatchformatchange()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	switch(m_bType)
	{
	case ITEM_PROJECT:
	case ITEM_FOLDER:
	case ITEM_TEXTURE	:
	case ITEM_IMAGE:
		{
			CSelectFormatDlg dlg;
			dlg.m_nSelectedFormat = SELECT_FORMAT::DEFAULT_INDEX;

			if( dlg.DoModal() )
			{
				INT nSelected = dlg.m_nSelectedFormat;

				ChangeFormatInFolder(
					itemPos,
					cTree,
					SELECT_FORMAT::AVAILABLE[nSelected].m_bFormat);
			}
		}
		break;
	}
}

void CGameStudioDoc::OnUpdateItemBatchformatchange(CCmdUI *pCmdUI)
{
	if( m_bType == ITEM_TEXTURE || m_bType == ITEM_IMAGE ) //탭이 TEXTURE이거나 IMAGE이면서
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CGameStudioDoc::ExportLoc()
{
	if(m_bType == ITEM_FRAME)
	{
		CFileDialog dlg(
			FALSE,
			_T("*.csv"),
			NULL,
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			_T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||"));

		if( dlg.DoModal() == IDOK )
		{
			CString strName = dlg.GetPathName();

			FILE* fp = fopen( (LPCTSTR)strName, "wt" );
			if( !fp )
				return ;

			CTreeCtrl& TreeCtrl = GetTreeCtrl();

			std::stack<HTREEITEM> stItem;

			HTREEITEM hRoot = TreeCtrl.GetRootItem();
			stItem.push( hRoot );

			while(1)
			{
				hRoot = TreeCtrl.GetNextItem( hRoot, TVGN_NEXT );
				if( hRoot != NULL )
					stItem.push( hRoot );
				else
					break;
			}
			
			while( !stItem.empty() )
			{
				HTREEITEM hCurItem = stItem.top();
				stItem.pop();

				if( TreeCtrl.ItemHasChildren( hCurItem ) )
				{
					HTREEITEM hChildItem = TreeCtrl.GetChildItem( hCurItem );
					stItem.push( hChildItem );

					while(1)
					{
						HTREEITEM hNextItem = TreeCtrl.GetNextItem(hChildItem,TVGN_NEXT);

						if( hNextItem == NULL )
							break;

						stItem.push( hNextItem );
						hChildItem = hNextItem;
					}
				}
				else
				{
					CItemBase* pItem = (CItemBase*)TreeCtrl.GetItemData( hCurItem );

					if( pItem->m_bMenuID != 1 ) // 폴더가 아니라면
					{
						CFRAMEItem* pFrameItem = (CFRAMEItem*) pItem;

						for( INT i=0 ; i < (INT)(pFrameItem->m_vCTRL.size()); ++i )
						{
							CString& strText = pFrameItem->m_vCTRL[ i ]->m_strText;

							if( strText == "" )
								continue;

							BOOL bOnlyDigit = TRUE;

							for( INT j=0 ; j < strText.GetLength() ; ++j )
							{
								if( '\0' > strText[j] || strText[j] > '9' )
								{
									bOnlyDigit = FALSE;
									break;
								}
							}

							if( bOnlyDigit )
								continue;

							fprintf( fp, "\"%s\",\"%s\",\"%s\"\n",
								(LPCTSTR) pFrameItem->m_strID,
								(LPCTSTR) pFrameItem->m_vCTRL[ i ]->m_strCtrlID,
								(LPCTSTR) pFrameItem->m_vCTRL[ i ]->m_strText);
						}
					}
				}
			}

			fclose(fp);
		}
	}
}

int ParseCSV( LPCTSTR str, std::vector<CString> *pList) 
{ 
	static LPCTSTR vREPLACE[][1] = 
	{
		{ "<_RO_S_>" },
		{ "<_RO_s_>" },
		{ "<_RO_T_>" },
		{ "<_RO_t_>" },
	};

	static char vREPLACECHAR[] =
	{
		0xAA,
		0xBA,
		0xDE,
		0XFE,
	};

    int cnt = 0; 
    CString buffer;
	CString tempStr = str;
	CString strReplace;
    int mode = 0; 

	for( int i = 0; i < 4; ++i )
	{
		strReplace.Format("%c",vREPLACECHAR[i]);
		tempStr.Replace( vREPLACE[i][0], strReplace );
	}

	str = (LPCTSTR) tempStr;
    
    while(*str) 
    {  
        if(mode == 0) 
        {      
            if(*str == ',') 
            { 
                pList->push_back(buffer); 
                buffer = ""; 
            } 
            else if(*str == '\"') 
            { 
                mode = 1;            
            } 
            else 
                buffer += *str; 
        } 
        else if(mode == 1) 
        { 
            if(*str == '\"') 
            { 
                ++str; 
                if(*str == '\"') 
                    buffer += '\"'; 
                else if(*str == ',') 
                { 
                    mode = 0; 
                    pList->push_back(buffer); 
                    buffer = ""; 
                } 
                else 
                    buffer += *str; 
            } 
            else 
                buffer += *str; 
        } 
        
        ++str; 
    }    

    pList->push_back(buffer); 
    return (int)pList->size();
} 

void CGameStudioDoc::ImportLoc()
{
	if(m_bType == ITEM_FRAME)
	{
		CFileDialog dlg(
			FALSE,
			_T("*.csv"),
			NULL,
			OFN_READONLY,
			_T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||"));

		if( dlg.DoModal() == IDOK )
		{
			CStdioFile vSRC;
			
			CString strName = dlg.GetPathName();
			if( vSRC.Open( strName, CFile::modeRead|CFile::typeText) )
			{
				CString strDATA;
				std::vector<CString> vELEM;

				while(vSRC.ReadString(strDATA))
				{
					vELEM.clear();

					ParseCSV( (LPCTSTR)strDATA, &vELEM );
					if( vELEM.size() >= 4 )
					{
						//vELEM[0]; // 프레임 이름
						//vELEM[1]; // 컨트롤 이름
						//vELEM[2]; // 원본 텍스트
						//vELEM[3]; // 번역 텍스트

						CFRAMEItem* pFrameItem = NULL;
						pFrameItem = FindFrameItemByID( vELEM[0] );

						if( pFrameItem )
							for( INT i=0 ; i < (INT)(pFrameItem->m_vCTRL.size()) ; ++i )
								if( pFrameItem->m_vCTRL[ i ]->m_strCtrlID == vELEM[1] )
								{
									if( vELEM[3] != "" )
										pFrameItem->m_vCTRL[ i ]->m_strText = vELEM[3];
								}
					}
				}

				vSRC.Close();
			}
		}
	}
}

void CGameStudioDoc::IteratingInFolderForExport(
	HTREEITEM itemPos,
	CTreeCtrl& cTrr,
	CString strPATH,
	DWORD dwFormat,
	CString strEXT)
{
	HTREEITEM hIterator = itemPos;

	do
	{
		CItemBase* pItem = (CItemBase*)cTrr.GetItemData( hIterator);
		if( pItem == NULL )
			return ;

		switch( pItem->m_bType )
		{
		case ITEM_TEXTURE:
			{
				CTextureItem* pTexItem = (CTextureItem*)pItem;

				CString strFILEPATH = strPATH;
				strFILEPATH += "\\";
				strFILEPATH += pTexItem->m_strName;
				strFILEPATH += strEXT;

				ExportTexture( strFILEPATH, pTexItem, (D3DXIMAGE_FILEFORMAT)dwFormat );
			}
			break;

		case ITEM_IMAGE:
			{
				CImageItem* pImgItem = (CImageItem*)pItem;

				CString strFILEPATH = strPATH;
				strFILEPATH += "\\";
				strFILEPATH += pImgItem->m_strName;
				strFILEPATH += strEXT;

				ExportImage( strFILEPATH, pImgItem, (D3DXIMAGE_FILEFORMAT)dwFormat );
			}
			break;

		case ITEM_PROJECT:
		case ITEM_FOLDER:
			{
				CString strNEWPATH = strPATH;
				strNEWPATH += "\\";
				strNEWPATH += pItem->m_strName;

				CreateDirectory(
					(LPCTSTR)strNEWPATH,
					NULL );

				HTREEITEM childItem = cTrr.GetChildItem( hIterator);
				if( childItem != NULL )
				{
					IteratingInFolderForExport(
						childItem,
						cTrr,
						strNEWPATH,
						dwFormat,
						strEXT);
				}
			}
			break;
		}
	}
	while(  hIterator = cTrr.GetNextItem(hIterator,TVGN_NEXT) );
}

void CGameStudioDoc::OnExportiteminfolder()
{
	if( m_bType != ITEM_TEXTURE &&
		m_bType != ITEM_IMAGE )
		return ;

	CTShell PathDlg;
	PathDlg.m_strTitle = _T("Target Folder");

	CSelectFormatDlg FmtDlg;
	FmtDlg.m_nType = 2;

	if( PathDlg.BrowseForFolder(AfxGetMainWnd()) == IDOK &&
		FmtDlg.DoModal() == IDOK )
	{
		DWORD dwFormat = D3DXIFF_TGA;
		CString strEXT = ".tga";
		switch( FmtDlg.m_nSelectedFormat )
		{
			case 0: dwFormat = D3DXIFF_TGA; strEXT = ".tga"; break;
			case 1: dwFormat = D3DXIFF_JPG; strEXT = ".jpg"; break;
		}

		CString strPATH = PathDlg.m_strPath;

		CTreeCtrl& cTree = GetTreeCtrl();
		HTREEITEM itemPos = cTree.GetSelectedItem();

		CItemBase* pItemPos = (CItemBase*) cTree.GetItemData( itemPos);

		if( pItemPos->m_bType == ITEM_PROJECT ||
			pItemPos->m_bType == ITEM_FOLDER)
		{
			strPATH += "\\";
			strPATH += pItemPos->m_strName;

			CreateDirectory(
				(LPCTSTR)strPATH,
				NULL );

			HTREEITEM itemChild = cTree.GetChildItem( itemPos);

			CItemBase* pItemChild = (CItemBase*)cTree.GetItemData( itemChild);

			if( pItemChild )
			{
				IteratingInFolderForExport(
					itemChild,
					cTree,
					strPATH,
					dwFormat,
					strEXT);
			}

			AfxMessageBox("완료되었습니다.");		
		}
	}
}

void CGameStudioDoc::OnUpdateExportItemInFolder(CCmdUI *pCmdUI)
{
	if( m_bType == ITEM_TEXTURE || m_bType == ITEM_IMAGE ) //탭이 TEXTURE이거나 IMAGE이면서
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

struct tagLISTUP
{
	CStdioFile&		m_File;
	CString			m_strParentFolder;
	INT				m_nDepth;

	struct tagLISTUP( CStdioFile& file, CString str, INT nDepth )
		: m_File(file), m_strParentFolder(str), m_nDepth(nDepth) {}
	struct tagLISTUP( struct tagLISTUP& other )
		: m_File(other.m_File), m_strParentFolder(other.m_strParentFolder), m_nDepth(other.m_nDepth) {}
};

typedef struct tagLISTUP TLISTUP;

void IteratingInFolderForListup(
	HTREEITEM itemPos,
	CTreeCtrl& cTrr,
	TLISTUP Info)
{
	HTREEITEM hIterator = itemPos;

	do
	{
		CItemBase* pItem = (CItemBase*)cTrr.GetItemData( hIterator);
		if( pItem == NULL )
			return ;

		if( pItem->m_bType != ITEM_PROJECT &&
			pItem->m_bType != ITEM_FOLDER )
		{
			CString strTYPE;
			switch( pItem->m_bType )
			{
				case ITEM_PROJECT: strTYPE = "ITEM_PROJECT"; break;
				case ITEM_FOLDER: strTYPE = "ITEM_FOLDER"; break;
				case ITEM_TEXTURE: strTYPE = "ITEM_TEXTURE"; break;
				case ITEM_IMAGE: strTYPE = "ITEM_IMAGE"; break;
				case ITEM_MEDIA: strTYPE = "ITEM_MEDIA"; break;
				case ITEM_ANIMATION: strTYPE = "ITEM_ANIMATION"; break;
				case ITEM_MESH: strTYPE = "ITEM_MESH"; break;
				case ITEM_OBJTYPE: strTYPE = "ITEM_OBJTYPE"; break;
				case ITEM_OBJECT: strTYPE = "ITEM_OBJECT"; break;
				case ITEM_SFX: strTYPE = "ITEM_SFX"; break;
				case ITEM_EXTMAP: strTYPE = "ITEM_EXTMAP"; break;
				case ITEM_BSPMAP: strTYPE = "ITEM_BSPMAP"; break;
				case ITEM_COMP: strTYPE = "ITEM_COMP"; break;
				case ITEM_FRAME: strTYPE = "ITEM_FRAME"; break;
				default: strTYPE = "Unknown"; break;
			}

			CString strFMT;
			strFMT.Format( "%d\t%s\t%s\t%s\t%s\t%s\n",
				pItem->m_dwID,
				strTYPE,
				"",
				pItem->m_strID,
				Info.m_strParentFolder,
				pItem->m_strName);

			Info.m_File.WriteString( strFMT );

			if( Info.m_nDepth == 0 )
				return ;
		}
		else
		{
			HTREEITEM childItem = cTrr.GetChildItem( hIterator);
			if( childItem != NULL )
			{
				TLISTUP myInfo(Info);
				CString strFMT;
				strFMT.Format( "\\%s", pItem->m_strName );
				myInfo.m_strParentFolder.Append( strFMT );
				myInfo.m_nDepth += 1;

				IteratingInFolderForListup(
					childItem,
					cTrr,
					myInfo );
			}
		}
	}
	while(  (hIterator = cTrr.GetNextItem(hIterator,TVGN_NEXT)) && (Info.m_nDepth!=0) );
}

void GetParentPath(
	HTREEITEM itemPos,
	CTreeCtrl& cTree,
	CString& strPath)
{
	HTREEITEM hParentPos = cTree.GetParentItem( itemPos);
	if( hParentPos != NULL )
	{
		CItemBase* pITEM = (CItemBase*) cTree.GetItemData( hParentPos );
		if( pITEM == NULL )
			return ;

		CString strName = pITEM->m_strName;
		strName += "/";
		strName.Append( strPath );
		strPath = strName;

		GetParentPath( hParentPos, cTree, strPath );
	}
}


void CGameStudioDoc::OnFolderListup()
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CItemBase* pItem = (CItemBase*) cTree.GetItemData( itemPos);
	
	if( pItem != NULL &&
		(pItem->m_bType == ITEM_PROJECT || pItem->m_bType == ITEM_FOLDER))
	{
		CFileDialog dlg(
			FALSE,
			_T("*.txt"),
			NULL,
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

		if( dlg.DoModal() == IDOK )
		{
			CStdioFile file;
			if( FALSE == file.Open( dlg.GetPathName(), CStdioFile::modeCreate | CStdioFile::modeWrite ) )
			{
				AfxMessageBox("파일을 열 수 없습니다.");
				return ;
			}
		
			CString strLINE;
			strLINE.Format(
				"ID\tTYPE\tNew File Path\tString ID\tStudio Path\tItem Name\n");
			file.WriteString(strLINE);

			CString strPath;
			GetParentPath( itemPos, cTree, strPath );

			TLISTUP Info(file, strPath, 0);

			IteratingInFolderForListup(
				itemPos,
				cTree,
				Info);

			file.Close();
		}
	}
}
