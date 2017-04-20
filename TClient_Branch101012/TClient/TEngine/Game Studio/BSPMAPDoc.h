#pragma once
#include "MAPDoc.h"
#include "BSPMAPBar.h"


// CBSPMAPDoc 문서입니다.

class CBSPMAPDoc : public CMAPDoc
{
	DECLARE_DYNCREATE(CBSPMAPDoc)

public:
	CBSPMAPDoc();
	virtual ~CBSPMAPDoc();
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CBSPMAPBar *GetBSPMAPBar();

public:
	CBSPMAPItem* m_pItem;
	int m_nIndex;
	
public:
	virtual BOOL GetWorldCenter( LPD3DXVECTOR3 pResult);
	virtual void CalcMAPTick( DWORD dwTick);
	virtual void InitData( LPVOID pData);
	virtual void UpdateData();
	virtual void Update();
	virtual void Restore();

	virtual void RotateLIGHT( D3DXVECTOR3 vROT);
	virtual void MoveLIGHT( D3DXVECTOR3 vMOVE);
	virtual void CopyLIGHT( D3DXVECTOR3 vPOS);
	virtual void AddLIGHT( LPD3DXVECTOR3 pPOS);
	virtual void DeleteLIGHT();
	virtual void UpdateLIGHT();
	virtual void ResetOBJATTR();
	virtual void ObjectReplace(BOOL bALLBOJ);
	virtual void ObjectReplace(MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID, DWORD* dwUnitPos, D3DXVECTOR3* vMovePos);
	virtual void ObjectAttach(BOOL bALLBOJ);
	virtual void ObjectAttach(MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID, DWORD* dwUnitPos, D3DXVECTOR3* vMovePos);
	virtual void AdjustObjectPos(CPoint* pUnitXZ, D3DXVECTOR3* vMoveOffset, BOOL bAttachType);

	virtual FLOAT GetHeight(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	void SetNodeInfo(
		CWnd *pWnd,
		DWORD dwNodeID,
		DWORD dwNodeCNT,
		DWORD dwPVS);

	void BuildOBJ();
	void ClearPVS();

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
