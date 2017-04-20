// BSPMAPDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "BSPMAPFrame.h"
#include "BSPMAPView.h"
#include "BSPMAPDoc.h"
#include "MainFrm.h"
#include ".\bspmapdoc.h"


// CBSPMAPDoc

IMPLEMENT_DYNCREATE(CBSPMAPDoc, CMAPDoc)

CBSPMAPDoc::CBSPMAPDoc()
{
	m_bDocType = ITEM_BSPMAP;
	m_pItem = NULL;

	m_nIndex = -1;
}

BOOL CBSPMAPDoc::OnNewDocument()
{
	if (!CMAPDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CBSPMAPDoc::~CBSPMAPDoc()
{
	if(m_pItem)
	{
		m_pItem->SaveDATA();
		m_pItem->ClearDATA();

		m_pItem->SetMainDocument(NULL);
		m_pItem = NULL;
	}
}


BEGIN_MESSAGE_MAP(CBSPMAPDoc, CMAPDoc)
END_MESSAGE_MAP()


// CBSPMAPDoc 진단입니다.

#ifdef _DEBUG
void CBSPMAPDoc::AssertValid() const
{
	CMAPDoc::AssertValid();
}

void CBSPMAPDoc::Dump(CDumpContext& dc) const
{
	CMAPDoc::Dump(dc);
}
#endif //_DEBUG


// CBSPMAPDoc serialization입니다.

void CBSPMAPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}


// CBSPMAPDoc 명령입니다.

CBSPMAPBar *CBSPMAPDoc::GetBSPMAPBar()
{
	CBSPMAPFrame *pFrame = (CBSPMAPFrame *) GetFrameWnd();
	return pFrame ? &pFrame->m_wndBSPMAPBar : NULL;
}

void CBSPMAPDoc::InitData( LPVOID pData)
{
	CTachyonDoc::InitData(pData);
	if(!pData)
		return;

	m_pItem = (CBSPMAPItem *) pData;
	m_pItem->SetMainDocument(this);
	m_pMAP = m_pItem;

	Update();
}

void CBSPMAPDoc::Update()
{
	if(m_pItem)
	{
		CBSPMAPBar *pControl = GetBSPMAPBar();
		m_pItem->LoadDATA();

		SetTitle(m_pItem->m_strName);
		ClearPVS();
		BuildOBJ();

		if(pControl)
			pControl->SetItem(m_pItem);

		CMAPDoc::Update();
	}
}

BOOL CBSPMAPDoc::GetWorldCenter( LPD3DXVECTOR3 pResult)
{
	CMAPDoc::GetWorldCenter(pResult);
	return TRUE;
}

void CBSPMAPDoc::DeleteLIGHT()
{
	while(!m_vSELLIGHT.empty())
	{
		MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(m_vSELLIGHT.back());

		m_pItem->m_BSPMAP.DeleteLight(m_vSELLIGHT.back());
		delete (*finder).second;
		m_pItem->m_mapLIGHT.erase(finder);

		m_vSELLIGHT.pop_back();
	}

	ClearPVS();
}

void CBSPMAPDoc::UpdateLIGHT()
{
	CMAPLIGHTBar *pControl = GetMAPLIGHTBar();
	D3DXVECTOR3 vMOVE = D3DXVECTOR3(
		(FLOAT) atof(pControl->m_strPosX),
		(FLOAT) atof(pControl->m_strPosY),
		(FLOAT) atof(pControl->m_strPosZ)) - GetSELLIGHTPOS();

	for( int i=0; i<INT(m_vSELLIGHT.size()); i++)
	{
		MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(m_vSELLIGHT[i]);

		LPMAPLIGHT pLIGHT = finder != m_pItem->m_mapLIGHT.end() ? (*finder).second : NULL;
		MAPLIGHT vLIGHT;

		m_pItem->m_BSPMAP.DeleteLight(m_vSELLIGHT[i]);
		if(pLIGHT)
		{
			CTextureItem *pTEX = CItemBase::GetTextureItem(pLIGHT->m_dwTextureID);
			vLIGHT.m_vPOS = pLIGHT->m_vPOS + vMOVE;

			vLIGHT.m_dwTextureID = pLIGHT->m_dwTextureID;
			vLIGHT.m_dwDiffuse = pLIGHT->m_dwDiffuse;
			vLIGHT.m_dwAmbient = pLIGHT->m_dwAmbient;
			vLIGHT.m_bUseDL = pLIGHT->m_bUseDL;
			vLIGHT.m_bUseLM = pLIGHT->m_bUseLM;
			vLIGHT.m_fRange = pLIGHT->m_fRange;
			vLIGHT.m_bType = pLIGHT->m_bType;
			vLIGHT.m_fROT = pLIGHT->m_fROT;
			vLIGHT.m_vDIR = pLIGHT->m_vDIR;

			m_pItem->m_BSPMAP.AddLight( pTEX ? pTEX->GetTEXTURESET() : NULL, &vLIGHT, m_vSELLIGHT[i]);
		}
	}

	ClearPVS();
	RedrawAllView();
}

void CBSPMAPDoc::RotateLIGHT( D3DXVECTOR3 vROT)
{
	MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(m_vSELLIGHT[0]);
	CMAPLIGHTBar *pControl = GetMAPLIGHTBar();

	switch((*finder).second->m_bType)
	{
	case MLT_DIRECTION	:
		{
			D3DXMATRIX vRotX;
			D3DXMATRIX vRotY;
			D3DXMATRIX vRotZ;

			D3DXMatrixRotationX(
				&vRotX,
				vROT.x);

			D3DXMatrixRotationY(
				&vRotY,
				vROT.y);

			D3DXMatrixRotationZ(
				&vRotZ,
				vROT.z);

			vRotX = vRotX * vRotY * vRotZ;
			CTMath::Transform(
				&vRotX,
				&((*finder).second->m_vDIR));

			pControl->m_strDirX.Format( "%.2f", (*finder).second->m_vDIR.x);
			pControl->m_strDirY.Format( "%.2f", (*finder).second->m_vDIR.y);
			pControl->m_strDirZ.Format( "%.2f", (*finder).second->m_vDIR.z);

			(*finder).second->m_vDIR = D3DXVECTOR3(
				(FLOAT) atof(pControl->m_strDirX),
				(FLOAT) atof(pControl->m_strDirY),
				(FLOAT) atof(pControl->m_strDirZ));
		}

		break;

	case MLT_POINT		:
		{
			vROT.x += ((FLOAT) atof(pControl->m_strROT)) * D3DX_PI / 180.0f;
			pControl->m_strROT.Format( "%.2f", vROT.x * 180.0f / D3DX_PI);

			(*finder).second->m_fROT = ((FLOAT) atof(pControl->m_strROT)) * D3DX_PI / 180.0f;
		}

		break;
	}

	pControl->UpdateData(FALSE);
	UpdateLIGHT();
}

void CBSPMAPDoc::MoveLIGHT( D3DXVECTOR3 vMOVE)
{
	CMAPLIGHTBar *pControl = GetMAPLIGHTBar();

	vMOVE.x += (FLOAT) atof(pControl->m_strPosX);
	vMOVE.y += (FLOAT) atof(pControl->m_strPosY);
	vMOVE.z += (FLOAT) atof(pControl->m_strPosZ);

	pControl->m_strPosX.Format( "%.2f", vMOVE.x);
	pControl->m_strPosY.Format( "%.2f", vMOVE.y);
	pControl->m_strPosZ.Format( "%.2f", vMOVE.z);

	pControl->UpdateData(FALSE);
	UpdateLIGHT();
}

void CBSPMAPDoc::AddLIGHT( LPD3DXVECTOR3 pPOS)
{
	CMAPLIGHTBar *pControl = GetMAPLIGHTBar();

	LPMAPLIGHT pLIGHT = new MAPLIGHT();
	DWORD dwID = GetNewLIGHTID();

	CTextureItem *pTEX = CItemBase::GetTextureItem(pControl->m_dwTextureID);
	pControl->UpdateData();

	pLIGHT->m_fROT = ((FLOAT) atof(pControl->m_strROT)) * D3DX_PI / 180.0f;
	pLIGHT->m_fRange = (FLOAT) atof(pControl->m_strRange);
	pLIGHT->m_dwTextureID = pTEX ? pTEX->m_dwID : 0;
	pLIGHT->m_bType = (BYTE) pControl->m_nType;
	pLIGHT->m_bUseDL = pControl->m_bUseDL;
	pLIGHT->m_bUseLM = pControl->m_bUseLM;
	pLIGHT->m_vPOS = *(pPOS);

	pLIGHT->m_dwAmbient = VALUEARGB(
		0xFF,
		pControl->m_dwRValueA,
		pControl->m_dwGValueA,
		pControl->m_dwBValueA);

	pLIGHT->m_dwDiffuse = VALUEARGB(
		0xFF,
		pControl->m_dwRValueD,
		pControl->m_dwGValueD,
		pControl->m_dwBValueD);

	pLIGHT->m_vDIR = D3DXVECTOR3(
		(FLOAT) atof(pControl->m_strDirX),
		(FLOAT) atof(pControl->m_strDirY),
		(FLOAT) atof(pControl->m_strDirZ));

	m_pItem->m_BSPMAP.AddLight( pTEX ? pTEX->GetTEXTURESET() : NULL, pLIGHT, dwID);
	m_pItem->m_mapLIGHT.insert( MAPMAPLIGHT::value_type( dwID, pLIGHT));

	ClearPVS();
}

void CBSPMAPDoc::CopyLIGHT( D3DXVECTOR3 vPOS)
{
	int nCount = INT(m_vSELLIGHT.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(m_vSELLIGHT[i]);

		LPMAPLIGHT pLIGHT = new MAPLIGHT();
		DWORD dwID = GetNewLIGHTID();

		CTextureItem *pTEX = CItemBase::GetTextureItem((*finder).second->m_dwTextureID);

		pLIGHT->m_dwTextureID = (*finder).second->m_dwTextureID;
		pLIGHT->m_dwAmbient = (*finder).second->m_dwAmbient;
		pLIGHT->m_dwDiffuse = (*finder).second->m_dwDiffuse;
		pLIGHT->m_fRange = (*finder).second->m_fRange;
		pLIGHT->m_bUseDL = (*finder).second->m_bUseDL;
		pLIGHT->m_bUseLM = (*finder).second->m_bUseLM;
		pLIGHT->m_bType = (*finder).second->m_bType;
		pLIGHT->m_vPOS = (*finder).second->m_vPOS + vPOS;
		pLIGHT->m_fROT = (*finder).second->m_fROT;
		pLIGHT->m_vDIR = (*finder).second->m_vDIR;

		m_pItem->m_BSPMAP.AddLight( pTEX ? pTEX->GetTEXTURESET() : NULL, pLIGHT, dwID);
		m_pItem->m_mapLIGHT.insert( MAPMAPLIGHT::value_type( dwID, pLIGHT));
	}

	ClearPVS();
	RedrawAllView();
}

void CBSPMAPDoc::SetNodeInfo( CWnd *pWnd,
							  DWORD dwNodeID,
							  DWORD dwNodeCNT,
							  DWORD dwPVS)
{
	CBSPMAPFrame *pFrame = (CBSPMAPFrame *) GetFrameWnd();

	if( pFrame->GetActiveView() == pWnd )
		pFrame->m_wndBSPMAPBar.SetNodeInfo( dwNodeID, dwNodeCNT, dwPVS);
}

void CBSPMAPDoc::ClearPVS()
{
	POSITION pos = GetFirstViewPosition();

	while(pos)
	{
		CView *pView = GetNextView(pos);

		if( pView && pView->IsKindOf(RUNTIME_CLASS(CBSPMAPView)) )
			((CBSPMAPView *) pView)->m_mapPVS.clear();
	}
}

void CBSPMAPDoc::CalcMAPTick( DWORD dwTick)
{
	m_pItem->m_BSPMAP.m_dwTick = dwTick;
}

void CBSPMAPDoc::BuildOBJ()
{
	MAPMAPOBJ::iterator itMAPOBJ;
	MAPOBJECT::iterator itOBJ;

	for( itOBJ = m_mapOBJ.begin(); itOBJ != m_mapOBJ.end(); itOBJ++)
		delete (*itOBJ).second;

	m_vSELOBJ.clear();
	m_mapOBJ.clear();

	for( itMAPOBJ = m_pItem->m_BSPMAP.m_mapOBJ.begin(); itMAPOBJ != m_pItem->m_BSPMAP.m_mapOBJ.end(); itMAPOBJ++)
	{
		CTachyonObject *pOBJ = new CTachyonObject();

		pOBJ->InitOBJ( &CMainFrame::m_3DDevice, (*itMAPOBJ).second);
		pOBJ->m_pHost = GetMAPOBJBar();

		m_mapOBJ.insert( MAPOBJECT::value_type( (*itMAPOBJ).first, pOBJ));
	}
}

void CBSPMAPDoc::Restore()
{
	m_pItem->m_BSPMAP.ResetLight();
	CMAPDoc::Restore();
}

FLOAT CBSPMAPDoc::GetHeight( FLOAT fPosX,
							 FLOAT fPosY,
							 FLOAT fPosZ)
{
	return m_pItem->m_BSPMAP.GetHeight(
		fPosX,
		fPosY,
		fPosZ);
}

void CBSPMAPDoc::ResetOBJATTR()
{
}

void CBSPMAPDoc::ObjectAttach(MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID, DWORD* dwUnitPos, D3DXVECTOR3* vMovePos)
{
}

void CBSPMAPDoc::ObjectAttach(BOOL bALLBOJ)
{
}

void CBSPMAPDoc::ObjectReplace(MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID, DWORD* dwUnitPos, D3DXVECTOR3* vMovePos)
{
}

void CBSPMAPDoc::ObjectReplace(BOOL bALLBOJ)
{
}

void CBSPMAPDoc::AdjustObjectPos(CPoint* pUnitXZ, D3DXVECTOR3* vMoveOffset, BOOL bAttachType)
{
}

void CBSPMAPDoc::UpdateData()
{
	if(m_pItem)
		m_pItem->SaveDATA();
}
