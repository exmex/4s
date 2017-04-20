// Game StudioDoc.h : interface of the CGameStudioDoc class
//

#pragma once

#include "ProgressDlg.h"
#include "PreviewDlg.h"
#include "Itembase.h"

class CGameStudioView;
class CMainFrame;


typedef struct tagFILETHREADPARAM
{
	CProgressDlg *m_pWnd;
	CMainFrame *m_pOwner;
	DWORD m_dwVER;
	CFile *m_pFile;
} FILETHREADPARAM, *LPFILETHREADPARAM;


class CGameStudioDoc : public CDocument
{
public: // create from serialization only
	CGameStudioDoc();
	DECLARE_DYNCREATE(CGameStudioDoc)

// Attributes
public:
	static CPreviewDlg *m_pPreview;

	CItemBase *m_pSrcItem;
	BYTE m_bType;

// Operations
public:
	CGameStudioView* GetView();
	CTreeCtrl& GetTreeCtrl();

	void Clear();
	void AddItem(
		HTREEITEM itemPos,
		CItemBase *pItem);

	void SaveData( LPFILETHREADPARAM lParam);
	void LoadData( LPFILETHREADPARAM lParam);

	void SaveData(
		LPFILETHREADPARAM lParam,
		HTREEITEM itemPos,
		int *pCount);

	BOOL LoadData(
		LPFILETHREADPARAM lParam,
		HTREEITEM itemPos,
		int nCount);

	BOOL CanBeInsert(
		HTREEITEM itemPos,
		CTreeCtrl& cTree,
		CString strName);

	void AddCopyItem(void);
	HTREEITEM FindItemByID(
		DWORD dwID);

	CFRAMEItem* FindFrameItemByID(
		CString strID);

	void ExportLoc();
	void ImportLoc();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CGameStudioDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL IsAnimationFile( CString strFileName);
	BOOL IsBSPOBJFile( CString strFileName);
	BOOL IsImageFile( CString strFileName);
	BOOL IsMediaFile( CString strFileName);
	BOOL IsMeshFile( CString strFileName);
	BOOL IsOBJFile( CString strFileName);
	BOOL IsGEOFile( CString strFileName);

	CString NewItemName(
		HTREEITEM itemPos,
		CTreeCtrl& cTree,
		DWORD dwFormatID);

	CString NewItemName(
		HTREEITEM itemPos,
		CTreeCtrl& cTree,
		CString strHeader);

public:
	static LPT3DDATA CompressTextureFileData(
		LPBYTE pData,
		DWORD dwSize);

protected:
	void AddOBJItem(
		HTREEITEM itemPos,
		CString strItemName);

	void AddMeshItem(
		HTREEITEM itemPos,
		CString strItemName,
		CString strFileName);

	void AddMeshFolder(
		HTREEITEM itemPos,
		CTreeCtrl& cTree,
		CString strFolderName);

	void AddTextureItem(
		HTREEITEM itemPos,
		CString strItemName,
		CString strFileName);

	void AddTextureFolder(
		HTREEITEM itemPos,
		CTreeCtrl& cTree,
		CString strFolderName);

	void AddImageItem(
		HTREEITEM itemPos,
		CString strItemName,
		CString strFileName);

	void AddImageFolder(
		HTREEITEM itemPos,
		CTreeCtrl& cTree,
		CString strFolderName);

	void AddMediaItem(
		HTREEITEM itemPos,
		CString strItemName,
		CString strFileName);

	void AddMediaFolder(
		HTREEITEM itemPos,
		CTreeCtrl& cTree,
		CString strFolderName);

	void AddAnimationItem(
		HTREEITEM itemPos,
		CString strItemName,
		CString strFileName);

	void AddAnimationFolder(
		HTREEITEM itemPos,
		CTreeCtrl& cTree,
		CString strFolderName);

	void ChangeFormatInFolder(
		HTREEITEM itemPos,
		CTreeCtrl& cTrr,
		BYTE bFormat);

	void ImpBSPOBJ(
		CBSPMAPItem *pItem,
		CString strFolder);

	void ImpOBJ(
		CEXTMAPItem *pItem,
		CString strFolder);

	void ImpGEO(
		CEXTMAPItem *pItem,
		CString strFolder);

	void ExportImage(
		const CString& strFile,
		CImageItem* pItem,
		D3DXIMAGE_FILEFORMAT eFormat=D3DXIFF_TGA);

	void ExportTexture(
		const CString& strFile,
		CTextureItem* pItem,
		D3DXIMAGE_FILEFORMAT eFormat=D3DXIFF_TGA);

	void IteratingInFolderForExport(
		HTREEITEM itemPos,
		CTreeCtrl& cTrr,
		CString strPATH,
		DWORD dwFormat,
		CString strEXT);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnItemNewproject();
	afx_msg void OnCancel();
	afx_msg void OnItemNewfolder();
	afx_msg void OnItemAdd();
	afx_msg void OnItemDelete();
	afx_msg void OnExpGeo();
	afx_msg void OnImpGeo();
	afx_msg void OnExpObj();
	afx_msg void OnImpObj();
	afx_msg void OnEditDetail();
	afx_msg void OnExpBsp();
	afx_msg void OnImpBsp();
	afx_msg void OnItemUpdate();
	afx_msg void OnItemExport();
	afx_msg void OnExpBspobj();
	afx_msg void OnImpBspobj();
	afx_msg void OnEditEnableUNIT();
	afx_msg void OnExtBuildtextureshadow();
	afx_msg void OnItemBatchformatchange();
	afx_msg void OnUpdateItemBatchformatchange(CCmdUI *pCmdUI);
	afx_msg void OnExportiteminfolder();
	afx_msg void OnUpdateExportItemInFolder(CCmdUI *pCmdUI);
	afx_msg void OnFolderListup();
};
