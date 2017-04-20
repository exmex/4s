// TQuestPathDoc.h : CTQuestPathDoc 클래스의 인터페이스
//

#pragma once


class CTQuestPathView;
class CTMapView;

class CTQuestPathDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CTQuestPathDoc();
	DECLARE_DYNCREATE(CTQuestPathDoc)

private:
	static void _Build( LPTBUILDINFO lParam);

protected:
	void Build( LPTBUILDINFO lParam);

// 특성
public:
	MAPTMAPDATA m_mapTMAPDB;

	MAPTQITEM m_mapTMISSION;
	MAPTQITEM m_mapTQCLASS;

	MAPVDWORD m_mapTMONITEM;
	MAPVDWORD m_mapTNPCITEM;

	CTGameDB m_vTGAMEDB;
	LPTQITEM m_pTQITEM;

	BYTE m_bEditPATH;
	int m_nBound;
	int m_nPathMode;

public:
	void ReleaseTMAPDB();
	void ReleaseBUF();

	void ClearPathData( LPTMAPIMG pTUNIT);
	void ImportTQUEST();
	void ImportTPATH();

	DWORD GetAllPathCount();
	DWORD GetChildCount(
		HTREEITEM hITEM,
		CTreeCtrl& cTree);

	void WriteMAPTMAPDATA(
		LPMAPTMAPDATA pTDATA,
		CArchive& vAR);

	void ReadMAPTMAPDATA(
		LPMAPTMAPDATA pTDATA,
		CArchive& vAR);

	void WriteMAPVDWORD(
		LPMAPVDWORD pTDATA,
		CArchive& vAR);

	void ReadMAPVDWORD(
		LPMAPVDWORD pTDATA,
		CArchive& vAR);

	void WriteHTREEITEM(
		HTREEITEM hITEM,
		CTreeCtrl& cTree,
		CArchive& vAR);

	void ReadHTREEITEM(
		HTREEITEM hITEM,
		CTreeCtrl& cTree,
		CArchive& vAR);

	void BuildTQUEST( LPTBUILDINFO lParam);
	void BuildTMAP( LPTBUILDINFO lParam);

// 작업
public:
	CTQuestPathView *GetTQuestPathView();
	CTMapView *GetTMapView();
	CTreeCtrl *GetQuestTreeCtrl();

	LPTQITEM FindTMISSION( DWORD dwID);
	LPTQITEM FindTQCLASS( DWORD dwID);

	LPTMAPDATA FindTMAPDATA( DWORD dwMapID);
	LPTMAPIMG FindTMAPIMG(
		DWORD dwMapID,
		FLOAT fPosX,
		FLOAT fPosZ);

	LPTMAPIMG FindTMAPIMG(
		LPTMAPDATA pTMAP,
		WORD wUnitX,
		WORD wUnitZ);

	WORD FindSimilarPoint(
		LPTMAPIMG pTUNIT,
		T2DPOINT lPoint,
		int nBound);

	CString ReadTMAPLIST( CStdioFile *pFILE);
	CString GetTNPCName( WORD wID);
	CString GetTFMTName(
		CString strTITLE,
		CString strNAME);

	void UpdateTMONName(
		CListCtrl *pLIST,
		DWORD dwKind,
		DWORD dwMapID);

	void EnableTPATHWND( int nIndex);
	void ResetMode();

	BYTE CalcTMONKIND(
		LPTMDATA pTMDATA,
		DWORD dwMonKind);

	LPTTITEM PickTTITEM(
		LPVTTITEM pVTTITEM,
		BYTE bType,
		DWORD dwID);

	BYTE LoadTMAPDB( CString strFILE);
	BYTE LoadTQUEST();

	BYTE GetAutosetData(
		LPTTITEM pTRESULT,
		LPTQITEM pTQITEM,
		LPTTITEM pTTITEM);

	FLOAT GetLineLength(
		T2DPOINT lStart,
		T2DPOINT lEnd);

	void ClearPathData( LPMAPTPATHDATA pTPATHDATA);
	void MakePathData(
		LPMAPTPATHDATA pTPATHDATA,
		LPTMAPIMG pTUNIT);

	void MakeFullPathData(
		LPMAPTPATHDATA pTPATHDATA,
		LPMAPTPATHINFO pTFULLPATH,
		WORD wPointID,
		LPMAPT2DPOINT pTPOINT,
		LPVDWORD pTPATH,
		FLOAT fDIST);

	void MergePathData(
		LPTMAPIMG pFROM,
		LPTMAPIMG pTO);

	void MovePathPoint(
		LPTMAPIMG pTUNIT,
		WORD wCurrentID,
		FLOAT fPosX,
		FLOAT fPosZ);

	void DeletePathPoint(
		LPTMAPIMG pTUNIT,
		WORD wPointID,
		WORD wReplaceID);

	void UpdateDATA(
		int nIndex,
		int nType,
		LPARAM lParam);

	void UpdateTMAP( LPTTITEM pTTITEM);
	void UpdateDATA( LPTQITEM pTQITEM);
	void ResetTERM( LPTQITEM pTQITEM);
	void RedrawTMAP();

// 재정의
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현
public:
	virtual ~CTQuestPathDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateMapEditmode(CCmdUI *pCmdUI);
	afx_msg void OnMapImpErrBound();
	afx_msg void OnMapGotoTpoint();
	afx_msg void OnMapEditmode();
	afx_msg void OnMapScale();
	afx_msg void OnDbAutosetAll();
	afx_msg void OnDbUnlockAll();
	afx_msg void OnDbLockAll();
	afx_msg void OnDbLoadQuest();
	afx_msg void OnDbLoadGame();
	afx_msg void OnDbLoadMap();
	afx_msg void OnFileExport();
	afx_msg void OnFileImport();
	afx_msg void OnFileBuild();
};
