#pragma once
#include "GridDoc.h"
#include "MeshBar.h"


// CMeshDoc 문서입니다.

class CMeshDoc : public CGridDoc
{
	DECLARE_DYNCREATE(CMeshDoc)

public:
	CMeshDoc();
	virtual ~CMeshDoc();
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CMeshBar *GetMeshBar();

public:
	CMeshItem* m_pItem;

	LPBYTE m_pCheck;
	int m_nCurSel;

public:
	virtual void InitData( LPVOID pData);
	virtual void Update();

	void RedrawAllView();
	void SetCamDist(
		CWnd *pWnd,
		FLOAT fDist);

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
