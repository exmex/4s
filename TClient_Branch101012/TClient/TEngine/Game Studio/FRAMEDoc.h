#pragma once
#include "GuideDoc.h"
#include "FRAMEBar.h"


// CFRAMEDoc 문서입니다.

class CFRAMEDoc : public CGuideDoc
{
	DECLARE_DYNCREATE(CFRAMEDoc)

public:
	CFRAMEDoc();
	virtual ~CFRAMEDoc();
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CFRAMEBar *GetFRAMEBar();

public:
	CFRAMEItem* m_pItem;

public:
	virtual void InitData( LPVOID pData);
	virtual void Update();

	virtual void SetCurSel( int nIndex);
	virtual CSize GetSize();

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
