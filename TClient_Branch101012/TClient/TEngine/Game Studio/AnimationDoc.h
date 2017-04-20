#pragma once
#include "GridDoc.h"
#include "AnimationBar.h"
#include "PlayCtrlBar.h"


// CAnimationDoc 문서입니다.

class CAnimationDoc : public CGridDoc
{
	DECLARE_DYNCREATE(CAnimationDoc)

public:
	CAnimationDoc();
	virtual ~CAnimationDoc();
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	D3DXMATRIX CalcFrameMatrix( BYTE bUpdate = FALSE);

	CAnimationBar *GetAnimationBar();
	CPlayCtrl *GetPlayCtrl();

	void RedrawAllView();
	void ResetCamera();

public:
	CAnimationItem* m_pItem;
	D3DXMATRIX m_vScale;
	D3DXMATRIX m_vWorld;
	FLOAT m_fTime;

public:
	virtual void InitData( LPVOID pData);
	virtual void Update();

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
