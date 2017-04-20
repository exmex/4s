#pragma once
#include "TachyonDoc.h"


// CGridDoc 문서입니다.

class CGridDoc : public CTachyonDoc
{
	DECLARE_DYNCREATE(CGridDoc)

public:
	CGridDoc();
	virtual ~CGridDoc();
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	virtual void DeleteForRestore();
	virtual void Restore();

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
