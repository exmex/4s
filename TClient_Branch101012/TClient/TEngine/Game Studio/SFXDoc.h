#pragma once
#include "GridDoc.h"
#include "PlayCtrlBar.h"
#include "ParticleBar.h"
#include "SFXBar.h"


// CSFXDoc 문서입니다.

class CSFXDoc : public CGridDoc
{
	DECLARE_DYNCREATE(CSFXDoc)

public:
	CSFXDoc();
	virtual ~CSFXDoc();
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CPlayCtrl *GetPlayCtrl();

	CParticleBar *GetParticleBar();
	CSFXBar *GetSFXBar();

	void SetFPS( DWORD dwFPS );

public:
	CSFXItem* m_pItem;

public:
	virtual void InitData( LPVOID pData);
	virtual void Update();

	void RedrawAllView();

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
