#pragma once
#include "TachyonDoc.h"
#include "ItemBase.h"
#include "atltypes.h"


#define GHP_TL				((BYTE) 0x00)
#define GHP_TC				((BYTE) 0x01)
#define GHP_TR				((BYTE) 0x02)
#define GHP_CL				((BYTE) 0x03)
#define GHP_CC				((BYTE) 0x04)
#define GHP_CR				((BYTE) 0x05)
#define GHP_BL				((BYTE) 0x06)
#define GHP_BC				((BYTE) 0x07)
#define GHP_BR				((BYTE) 0x08)

#define GNC_SIZE_X			((WORD) 0x000C)
#define GNC_SIZE_Y			((WORD) 0x000C)

#define GG_SIZE_X			((WORD) 0x0003)
#define GG_SIZE_Y			((WORD) 0x0003)


// CGuideDoc 문서입니다.

class CGuideDoc : public CTachyonDoc
{
	DECLARE_DYNCREATE(CGuideDoc)

public:
	CGuideDoc();
	virtual ~CGuideDoc();
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CGuideItem* m_pGuide;
	BYTE m_bHotPoint;

public:
	virtual void SetCurSel( int nIndex);
	virtual CSize GetSize();

	virtual void DeleteForRestore();
	virtual void Restore();

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	VECTORINT m_vSEL;
	VECTORINT m_vPreRECTinSEL;
	CRect m_rectMouseDrag;
	BYTE m_bMouseCheck;
	CPoint m_ptPreItemPos;
};
