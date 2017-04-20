#pragma once
#include "TachyonDoc.h"
#include "PlayCtrlBar.h"
#include "ImageBar.h"
#include "ItemBase.h"


// CImageDoc 문서입니다.

class CImageDoc : public CTachyonDoc
{
	DECLARE_DYNCREATE(CImageDoc)

public:
	CImageDoc();
	virtual ~CImageDoc();
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CImageBar *GetImageBar();
	CPlayCtrl *GetPlayCtrl();

	void RedrawAllView();

public:
	CImageItem* m_pItem;
	DWORD m_dwTick;

public:
	virtual void InitData( LPVOID pData);
	virtual void Update();

	virtual void DeleteForRestore();
	virtual void Restore();

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
