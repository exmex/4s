// TPathTimeDoc.h : interface of the CTPathTimeDoc class
//
#pragma once


class CTPathTimeDoc : public CDocument
{
protected: // create from serialization only
	CTPathTimeDoc();
	DECLARE_DYNCREATE(CTPathTimeDoc)

// Attributes
public:
	MAPTJOINT m_mapTJOINT;

	VTSPOT m_vTSPOTSEL;
	VTSPOT m_vTSPOT;

	LPTJOINT m_pTMOVE;
	HBITMAP m_hBMP;

public:
	CString m_strBMP;

	BYTE m_bCalcReturn;
	BYTE m_bEditMode;
	WORD m_wUnitID;

// Operations
public:
	void LoadBMP( CString& strFileName);
	void UpdatePathTime();
	void UpdateUnitID();
	void ClearTSPOTSEL();

	float CalcPathDist();
	int CalcPathTime();

	DWORD GetNewJointID();

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CTPathTimeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileLoadbmp();
	afx_msg void OnSpotSpotdisable();
	afx_msg void OnUpdateSpotSpotdisable(CCmdUI *pCmdUI);
	afx_msg void OnSpotSpotselect();
	afx_msg void OnUpdateSpotSpotselect(CCmdUI *pCmdUI);
	afx_msg void OnSpotSpotadd();
	afx_msg void OnUpdateSpotSpotadd(CCmdUI *pCmdUI);
	afx_msg void OnSpotSpotdelete();
	afx_msg void OnUpdateSpotSpotdelete(CCmdUI *pCmdUI);
	afx_msg void OnSpotCalcreturn();
	afx_msg void OnUpdateSpotCalcreturn(CCmdUI *pCmdUI);
	afx_msg void OnSpotSettimepermeter();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnSpotSpotcancellastselect();
	afx_msg void OnUpdateSpotSpotcancellastselect(CCmdUI *pCmdUI);
	afx_msg void OnUnitSelect();
	afx_msg void OnJointAdd();
	afx_msg void OnUpdateJointAdd(CCmdUI *pCmdUI);
	afx_msg void OnJointDelete();
	afx_msg void OnUpdateJointDelete(CCmdUI *pCmdUI);
	afx_msg void OnJointEdit();
	afx_msg void OnUpdateJointEdit(CCmdUI *pCmdUI);
	afx_msg void OnJointMove();
	afx_msg void OnUpdateJointMove(CCmdUI *pCmdUI);
	afx_msg void OnJointSetjointsize();
	afx_msg void OnUpdateJointSetjointsize(CCmdUI *pCmdUI);
	afx_msg void OnJointSetsnapsize();
	afx_msg void OnUpdateJointSetsnapsize(CCmdUI *pCmdUI);
};


