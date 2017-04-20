#pragma once
#include "TachyonDoc.h"
#include "PlayCtrl.h"
#include "ItemBase.h"
#include "MediaDlg.h"


// CMediaDoc 문서입니다.

class CMediaDoc : public CTachyonDoc
{
	DECLARE_DYNCREATE(CMediaDoc)

public:
	CMediaDoc();
	virtual ~CMediaDoc();
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CMediaDlg *GetMediaDlg();
	CPlayCtrl *GetPlayCtrl();

public:
	CMediaItem* m_pItem;
	int m_nIndex;

public:
	virtual void InitData( LPVOID pData);
	virtual void Update();

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
