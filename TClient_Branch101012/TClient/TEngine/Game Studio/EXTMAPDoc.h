#pragma once
#include "MAPDoc.h"
#include "EXTMAPBar.h"


typedef map< CWnd *, CWnd *>	MAPVIEW;

// CEXTMAPDoc 문서입니다.

class CEXTMAPDoc : public CMAPDoc
{
	DECLARE_DYNCREATE(CEXTMAPDoc)

public:
	CEXTMAPDoc();
	virtual ~CEXTMAPDoc();
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CEXTMAPBar *GetEXTMAPBar();

	BYTE LockUNIT(
		CWnd *pHost,
		DWORD dwUnitID);

	void ReleaseUNIT(
		CWnd *pHost,
		DWORD dwUnitID);

	void LoadUNIT(
		CTachyonEXTMAP *pUNIT,
		LPDIRECT3DSURFACE9 *pREGION,
		LPDIRECT3DSURFACE9 *pSHADE,
		LPDIRECT3DSURFACE9 *pSKIN,
		DWORD dwUnitID);

	void BuildOBJ( DWORD dwUnitID);
	void BuildTEX(
		CTachyonEXTMAP *pUNIT,
		LPDIRECT3DSURFACE9 pREGION,
		LPDIRECT3DSURFACE9 pSHADE,
		LPDIRECT3DSURFACE9 pSKIN);

	void RegulateOBJ( DWORD dwUnitID);
	void SetHeight( FLOAT fHeight);
	void ClearSPRAY();

	void ResetDETAIL();
	void RefreshView();
	void UpdateData();
	void Reload();

	void SelectUNIT(
		CWnd *pWnd,
		int nPosX,
		int nPosZ);

	void JumpView(
		FLOAT fPosX,
		FLOAT fPosZ);

	BOOL InsertMapOBJ( LPMAPOBJ pOLDOBJ);
	BOOL InsertMapOBJ(
		MAPMAPOBJ *pMAPMAPOBJ,
		MAPDWORD *pMAPOBJID,
		LPMAPOBJ pOLDOBJ,
		D3DXVECTOR3 *vMovePos);

	FLOAT GetHeight( D3DXVECTOR3 vPos);
	FLOAT GetPolyHeight(
		LPD3DXVECTOR3 pPOLY,
		LPD3DXVECTOR3 pPOS,
		FLOAT fRadius);

	LPVECLPOINT GetCollisionPos(
		CTachyonObject *pOBJ,
		COBJItem *pOBJITEM);

	void GetEqualOBJ(
		VECTORDWORD *vEqualOBJ,
		MAPMAPOBJ *pMAPOBJ,
		MAPDWORD *pMAPID,
		CTachyonObject *pOBJ,
		DWORD dwID);

public:
	LPDIRECT3DSURFACE9 m_pREGION[36];
	LPDIRECT3DSURFACE9 m_pSHADE[36];
	LPDIRECT3DSURFACE9 m_pSKIN[36];
	LPDIRECT3DTEXTURE9 m_pSPRAY;
	CTachyonEXTMAP m_UNIT[36];
	MAPVIEW m_mapHOST[36];

	MAPDWORD m_mapUNIT;
	FLOAT m_fBRUSH;
	FLOAT m_fFLAT;
	FLOAT m_fADDFLAT;
	BYTE m_bResetVB;

	CEXTMAPItem* m_pItem;

public:
	virtual BOOL GetWorldCenter( LPD3DXVECTOR3 pResult);
	virtual void InitData( LPVOID pData);
	virtual void Update();

	virtual void DeleteForRestore();
	virtual void Restore();

	virtual DWORD GetNewOBJID();
	virtual DWORD GetNewSFXID();
	virtual DWORD GetNewSNDID();

	virtual FLOAT GetHeight(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);
	virtual void GetEqualOBJ(VECTORDWORD* vEqualOBJ, BOOL bALLOBJ);
	
protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCloseDocument();
	virtual void GetOBJRANDOMPos(D3DXVECTOR3* vPos);
	virtual void RotateOBJbyPlane(D3DXVECTOR3* vROT, D3DXVECTOR3* vPOS, CTachyonObject* pOBJ);
	virtual void ObjectReplace(BOOL bALLBOJ);
	virtual void ObjectAttach(BOOL bALLBOJ);	
	virtual void ObjectAttach(MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID, DWORD* dwUnitPos, D3DXVECTOR3* vMovePos );
	virtual void ObjectReplace(MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID, DWORD* dwUnitPos, D3DXVECTOR3* vMovePos );
	virtual void ResetOBJATTR();
	virtual void AdjustObjectPos(CPoint* pUnitXZ, D3DXVECTOR3* vMoveOffset, BOOL bAttachType);
};
