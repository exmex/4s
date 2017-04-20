#pragma once
#include "PlayCtrlBar.h"
#include "GridDoc.h"
#include "ItemBase.h"
#include "MAPLIGHTBar.h"
#include "MAPOBJBar.h"
#include "MAPSFXBar.h"
#include "MAPSNDBar.h"
#include "atltypes.h"


#define BT_OBJ			0x00
#define BT_SFX			0x01
#define BT_SND			0x02


typedef struct tagUNDOBUF
{
	VECTORDWORD m_vID;
	BYTE m_bType;

	D3DXVECTOR3 m_vSCALE;
	D3DXVECTOR3 m_vPOS;
	D3DXVECTOR3 m_vROT;

	tagUNDOBUF()
	{
		m_vSCALE = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		m_vROT = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

		m_bType = BT_OBJ;
		m_vID.clear();
	};

	~tagUNDOBUF()
	{
		m_vID.clear();
	};
} UNDOBUF, *LPUNDOBUF;

typedef vector<LPUNDOBUF>		VUNDOBUF, *LPVUNDOBUF;


// CMAPDoc 문서입니다.

class CMAPDoc : public CGridDoc
{
	DECLARE_DYNCREATE(CMAPDoc)

public:
	CMAPDoc();
	virtual ~CMAPDoc();
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	BOOL IsHideOBJ( DWORD dwID );	// Hide된 Object인가
	BOOL IsFreezeOBJ( DWORD dwID );	// Freeze된 Object인가
	BOOL IsSelLIGHT( DWORD dwID);
	BOOL IsSelOBJ( DWORD dwID);
	BOOL IsSelSFX( DWORD dwID);
	BOOL IsSelSND( DWORD dwID);
	void ResetEnSelOBJ(void);

	int GetEnSelOBJCNT( DWORD dwID );
	
	D3DXVECTOR3 GetSELLIGHTPOS();
	D3DXVECTOR3 GetSELSNDPOS();
	D3DXVECTOR3 GetSELSFXPOS();
	D3DXVECTOR3 GetSELSFXROT();

	D3DXVECTOR3 GetSELOBJSCALE();
	D3DXVECTOR3 GetSELOBJPOS();
	D3DXVECTOR3 GetSELOBJROT();

public:
	CMAPLIGHTBar *GetMAPLIGHTBar();
	CMAPOBJBar *GetMAPOBJBar();
	CMAPSFXBar *GetMAPSFXBar();
	CMAPSNDBar *GetMAPSNDBar();
	CPlayCtrl *GetPlayCtrl();

public:
	VECTORDWORD m_vGROUPOBJ[10];	// Ctrl + Shift + 1~9 키로 Object Group 지정
	VECTORDWORD m_vSELOBJBUF;
	VECTORDWORD m_vSELLIGHT;
	VECTORDWORD m_vSELOBJ;
	VECTORDWORD m_vSELSFX;
	VECTORDWORD m_vSELSND;

	VECTORVEC m_vOBJPrePos;			// OBJ의 위치 중복을 막기위해 임시 저장하는 Vector
	VUNDOBUF m_vUNDOBUF;

	MAPDWORD m_mapFREEZE;
	MAPDWORD m_mapHIDE;

	MAPOBJECT m_mapOBJ;
	MAPDWORD m_mapSFX;
	MAPDWORD m_mapSND;
	CMAPItem* m_pMAP;

	BOOL m_bEditLIGHT;
	BOOL m_bEditOBJ;
	BOOL m_bEditSFX;
	BOOL m_bEditSND;

	BOOL m_bInsertLIGHT;
	BOOL m_bInsertOBJ;
	BOOL m_bInsertSFX;
	BOOL m_bInsertSND;

	BYTE m_bPushUndoBUF;
	int m_nUndoIndex;

public:
	void CalcSFX( DWORD dwTick);
	void RemoveSFX();
	void RedrawAllView();

	void UpdateLIGHTBAR();
	void UpdateOBJBAR();
	void UpdateSFXBAR();
	void UpdateSNDBAR();

	void AddHIDEOBJ(DWORD dwID);
	void AddFREEZEOBJ(DWORD dwID);

	void ClearSELLIGHT();
	void ClearSELOBJ();
	void ClearSELSFX();
	void ClearSELSND();

	void EraseHIDEOBJ(DWORD dwID);
	void EraseFREEZEOBJ(DWORD dwID);

	void PushUndoBUF(
		D3DXVECTOR3 vSCALE,
		D3DXVECTOR3 vPOS,
		D3DXVECTOR3 vROT,
		BYTE bType);

	void UndoOBJ( LPUNDOBUF pBUF);
	void UndoSFX( LPUNDOBUF pBUF);
	void UndoSND( LPUNDOBUF pBUF);

	void RedoOBJ( LPUNDOBUF pBUF);
	void RedoSFX( LPUNDOBUF pBUF);
	void RedoSND( LPUNDOBUF pBUF);
	
public:
	virtual BOOL GetWorldCenter( LPD3DXVECTOR3 pResult);
	virtual void CalcMAPTick( DWORD dwTick);
	virtual void DeleteLIGHT();
	virtual void UpdateLIGHT();
	virtual void Update();

	virtual DWORD GetNewLIGHTID();
	virtual DWORD GetNewOBJID();
	virtual DWORD GetNewSFXID();
	virtual DWORD GetNewSNDID();

	virtual FLOAT GetHeight(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	virtual void RotateLIGHT( D3DXVECTOR3 vROT);
	virtual void RotateOBJ( D3DXVECTOR3 vROT);
	virtual void RotateSFX( D3DXVECTOR3 vROT);

	virtual void MoveLIGHT( D3DXVECTOR3 vMOVE);
	virtual void MoveOBJ( D3DXVECTOR3 vMOVE);
	virtual void MoveSFX( D3DXVECTOR3 vMOVE);
	virtual void MoveSND( D3DXVECTOR3 vMOVE);

	virtual void AddLIGHT( LPD3DXVECTOR3 pPOS);
	virtual void CopyLIGHT( D3DXVECTOR3 vPOS);
	virtual void CopyOBJ( D3DXVECTOR3 vPOS);
	virtual void CopySFX( D3DXVECTOR3 vPOS);
	virtual void CopySND( D3DXVECTOR3 vPOS);

	virtual void GetOBJRANDOMPos(D3DXVECTOR3* vPos);		// OBJ SPRAY를 위한 OBJ의 Random POS를 리턴
	virtual void RotateOBJbyPlane(D3DXVECTOR3* vROT, D3DXVECTOR3* vPOS, CTachyonObject* pOBJ);	// OBJ를 PLANE의 NORMAL VECTOR 만큼 X/Z축 회전

	virtual void ObjectReplace(BOOL bALLBOJ);
	virtual void ObjectReplace(MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID, DWORD* dwUnitPos, D3DXVECTOR3* vMovePos );
	virtual void ObjectAttach(BOOL bALLBOJ);
	virtual void ObjectAttach(MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID, DWORD* dwUnitPos, D3DXVECTOR3* vMovePos );
	virtual void AdjustObjectPos(CPoint* pUnitXZ, D3DXVECTOR3* vMoveOffset, BOOL bAttachType);

	virtual void GetEqualOBJ(VECTORDWORD* vEqualOBJ, BOOL bALLOBJ);
	virtual void ResetOBJATTR();

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
};
