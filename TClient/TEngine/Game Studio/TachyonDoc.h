#pragma once


class CTachyonDoc;
typedef vector<CTachyonDoc *>		VECTORDOC;


class CTachyonDoc : public CDocument
{
	DECLARE_DYNCREATE(CTachyonDoc)

public:
	CTachyonDoc();
	virtual ~CTachyonDoc();
	virtual void Serialize( CArchive& ar);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	static VECTORDOC m_vDOC;

public:
	static void UpdateAll( CTachyonDoc *pExcept);
	static void UpdateFOV( FLOAT fFOV);

	static void DeleteForReset();
	static void Reset();
	static void CloseAllBSP();

public:
	CFrameWnd *m_pFRAME;
	BYTE m_bDocType;

// Operations
public:
	virtual CFrameWnd *GetFrameWnd();

	virtual void InitData( LPVOID pData);
	virtual void UpdateAll();
	
	virtual void Update();

	virtual void DeleteForRestore();
	virtual void Restore();

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
