// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "Game StudioView.h"
#include "TachyonMedia.h"
#include "PreviewDlg.h"


#define ID_PANE_POSX		1
#define ID_PANE_POSY		2
#define ID_PANE_POSZ		3

class CMainFrame;
class CItemFinder;
class CItemScreenShot;


typedef struct tagBUILDTHREADPARAM
{
	CProgressDlg *m_pWnd;
	CMainFrame *m_pOwner;
	DWORD m_dwVER;
	CString m_strPATH;
} BUILDTHREADPARAM, *LPBUILDTHREADPARAM;


class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

public:
	static CString m_strFileName;
	DWORD m_dwToolBarID;

private:
	static void _Build( LPBUILDTHREADPARAM lParam);
	static void _Merge( LPFILETHREADPARAM lParam);
	static void _Load( LPFILETHREADPARAM lParam);
	static void _Save( LPFILETHREADPARAM lParam);

	static void LoadTextureFromResource(
		DWORD dwResourceID,
		LPDIRECT3DTEXTURE9 *ppTexture);

	static void LoadMeshFromResource(
		DWORD dwResourceID,
		CTachyonMesh *pMesh);

protected:
	void PrintText(
		CStdioFile& file,
		CString strText);

	void PrintText(
		CStdioFile& file,
		CString strTag,
		CString strText);

	void PrintInt(
		CStdioFile& file,
		int nData);

	void PrintInt(
		CStdioFile& file,
		CString strTag,
		int nData);

	void PrintDWORD(
		CStdioFile& file,
		CString strTag,
		DWORD dwData);

	void PrintOpenTag(
		CStdioFile& file,
		CString strTag);

	void PrintCloseTag(
		CStdioFile& file,
		CString strTag);

	void PrintNewLine(
		CStdioFile& file,
		int nCount = 1);

	void PrintTab(
		CStdioFile& file,
		int nCount = 1);

	void PrintTime( CStdioFile& file);
	CString GetAlign( BYTE bAlign);

	void PrintID(
		CStdioFile& file,
		CString strID,
		DWORD dwID);

	void PrintCTRL(
		CStdioFile& file,
		LPCTRL pCTRL,
		LPCOMP pCOMP,
		BOOL bFRAME,
		int nTab);

	void PrintFrame(
		CStdioFile& file,
		CFRAMEItem *pFRAME);

	void PrintFontType(
		CStdioFile& file,
		LPGSFONT pFONT);

protected:
	void Build( LPBUILDTHREADPARAM lParam);
	void Merge( LPFILETHREADPARAM lParam);
	void Load( LPFILETHREADPARAM lParam);
	void Save( LPFILETHREADPARAM lParam);

	void SaveGlobal( LPFILETHREADPARAM lParam);
	void LoadGlobal( LPFILETHREADPARAM lParam);

	void BuildTSC(
		LPBUILDTHREADPARAM lParam,
		CString strTITLE);

	void BuildINDEX(
		LPBUILDTHREADPARAM lParam,
		CString strINDEX,
		VECTORSTRING& vFILE,
		MAPRESINDEX& mapINDEX);

	void BuildTEX(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		VECTORSTRING& vFILE,
		MAPRESINDEX& mapINDEX);

	void BuildIMG(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		VECTORSTRING& vFILE,
		MAPRESINDEX& mapINDEX);

	void BuildMEDIA(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		VECTORSTRING& vFILE,
		MAPRESINDEX& mapINDEX);

	void BuildANI(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		VECTORSTRING& vFILE,
		MAPRESINDEX& mapINDEX);

	void BuildMESH(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		VECTORSTRING& vFILE,
		MAPRESINDEX& mapINDEX);

	void BuildOBJ(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		VECTORSTRING& vFILE,
		MAPRESINDEX& mapINDEX);

	void BuildSFX(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		VECTORSTRING& vFILE,
		MAPRESINDEX& mapINDEX);

	void BuildMAP(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		VECTORSTRING& vFILE,
		MAPRESINDEX& mapINDEX);

	void BuildTEX(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		DWORD dwFileID,
		int& nIndex,
		CFile& file,
		MAPRESINDEX& mapINDEX);

	void BuildIMG(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		DWORD dwFileID,
		int& nIndex,
		CFile& file,
		MAPRESINDEX& mapINDEX);

	void BuildANI(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		DWORD dwFileID,
		int& nIndex,
		CFile& file,
		MAPRESINDEX& mapINDEX);

	void BuildMESH(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		DWORD dwFileID,
		int& nIndex,
		CFile& file,
		MAPRESINDEX& mapINDEX);

	void BuildOBJ(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		DWORD dwFileID,
		int& nIndex,
		CFile& file,
		MAPRESINDEX& mapINDEX);

	void BuildSFX(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		DWORD dwFileID,
		int& nIndex,
		CFile& file,
		MAPRESINDEX& mapINDEX);

	void BuildMAP(
		LPBUILDTHREADPARAM lParam,
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		DWORD dwFileID,
		int& nIndex,
		CFile& file,
		MAPRESINDEX& mapINDEX);

	void BuildHEADER(
		LPBUILDTHREADPARAM lParam,
		CString strTITLE);

	void BuildTEX(
		CTextureItem *pItem,
		CFile& file);

	void BuildIMG(
		CImageItem *pItem,
		CFile& file);

	void BuildANI(
		CAnimationItem *pItem,
		CFile& file);

	void BuildMESH(
		CMeshItem *pItem,
		CFile& file);

	void BuildOBJ(
		COBJItem *pItem,
		CFile& file);

	void BuildSFX(
		CSFXItem *pItem,
		CFile& file);

	void BuildEXTMAP(
		CEXTMAPItem *pItem,
		CFile& file);

	void BuildBSPMAP(
		CBSPMAPItem *pItem,
		CFile& file);

	void ResetUseVB(
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		BYTE bUseVB);

	void UpdateSizeAttr(
		CTreeCtrl *pTree,
		HTREEITEM itemPos);

	void ResetTextureOption(
		CTreeCtrl *pTree,
		HTREEITEM itemPos,
		BYTE bTextureOption);

	void AutoLOD(
		CTreeCtrl *pTree,
		HTREEITEM itemPos);

// Attributes
public:
	CGameStudioDoc *m_pDocAnimation;
	CGameStudioDoc *m_pDocTexture;
	CGameStudioDoc *m_pDocObject;
	CGameStudioDoc *m_pDocImage;
	CGameStudioDoc *m_pDocMedia;
	CGameStudioDoc *m_pDocFrame;
	CGameStudioDoc *m_pDocComp;
	CGameStudioDoc *m_pDocMesh;
	CGameStudioDoc *m_pDocSFX;
	CGameStudioDoc *m_pDocMAP;

public:
	static LPDIRECT3DSWAPCHAIN9 m_pSWAPCHAIN;
	static CTachyonMedia m_TachyonMedia;
	static CD3DDevice m_3DDevice;
	static BYTE m_bRender;
	static FLOAT m_fFOV;	// Perspective Ã¢ÀÇ FOV °ª

public:
	static void RestoreDevice();
	static void ReleaseDevice();
	static void InitDevice();

protected:
	virtual CDocument* GetActiveDocument();

	void ClearStaticMap();
	void UpdateTitle();

public:
	void LoadToolBar( UINT nResourceID);
	void OnFileNew();
	void OnFileOpen();
	void OnFileSave();
	void OnFileSaveAs();

	void SetPaneText(
		CString strText,
		int nIndex);

	CString GetFileTitle();
	int GetItemCount();

	void SetActiveWorkspace(
		INT nItemType );

// Operations
public:
	CImageList m_listWorkspaceTabImages;

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CTTabControlBar m_wndWorkSpace;
	CTControlBar m_wndPreview;

protected:  // control bar embedded members
	CTStatusControlBar m_wndStatusBar;
	CTMenuControlBar m_wndMenuBar;
	CTToolControlBar m_wndToolBar;

	CTWRB<CTADLG<CPreviewDlg> > m_wndPreviewID;

	DECLARE_MESSAGE_MAP()
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public :

	static CItemFinder*			m_pFinderDlg;
	static CItemScreenShot*	m_pScreenShotDlg;

public:
	afx_msg void OnDestroy();
	afx_msg void OnFileMerge();
	afx_msg void OnBuild();
	afx_msg void OnEditmenu();
	afx_msg void OnEditfont();
	afx_msg void OnEditFov();
	afx_msg void OnNotuseVb();
	afx_msg void OnUpdateNotuseVb( CCmdUI *pCmdUI);
	afx_msg void OnResetVb();
	afx_msg void OnUpdateResetVb( CCmdUI *pCmdUI);
	afx_msg void OnMenuTexhi();
	afx_msg void OnUpdateMenuTexhi( CCmdUI *pCmdUI);
	afx_msg void OnMenuTexlow();
	afx_msg void OnUpdateMenuTexlow( CCmdUI *pCmdUI);
	afx_msg void OnMenuTexmid();
	afx_msg void OnUpdateMenuTexmid( CCmdUI *pCmdUI);
	afx_msg void OnMenuAutolod();
	afx_msg void OnUpdateMenuAutolod(CCmdUI *pCmdUI);
	afx_msg void OnMenuLocExp();
	afx_msg void OnUpdateMenuLocExp(CCmdUI *pCmdUI);
	afx_msg void OnMenuLocImp();
	afx_msg void OnUpdateMenuLocImp(CCmdUI *pCmdUI);
	afx_msg void OnMenuListUpdate();
	afx_msg void OnItemFinder();
	afx_msg void OnItemScreenShot();
	afx_msg void OnMenuUpdatesize();
	afx_msg void OnUpdateMenuUpdatesize(CCmdUI *pCmdUI);
	afx_msg void OnFileSetTemppath();
	afx_msg void OnUpdateFileSetTemppath(CCmdUI *pCmdUI);
};
