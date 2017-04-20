// OBJDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MainFrm.h"
#include "OBJFrame.h"
#include "OBJDoc.h"
#include "EditObjectDlg.h"


// COBJDoc

IMPLEMENT_DYNCREATE(COBJDoc, CGridDoc)

COBJDoc::COBJDoc()
{
	m_bDocType = ITEM_OBJECT;
	m_pItem = NULL;

	m_mapEQUIP.clear();
}

BOOL COBJDoc::OnNewDocument()
{
	if (!CGridDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

COBJDoc::~COBJDoc()
{
	if(m_pItem)
	{
		m_pItem->m_TOBJ.m_pHost = NULL;
		m_pItem->m_TOBJ.RemoveOBJSFX();
		m_pItem->m_TOBJ.ClearOBJSND();
		m_pItem->m_TOBJ.ClearEquip();
		m_mapEQUIP.clear();
		RemoveSFX();

		m_pItem->SetMainDocument(NULL);
		m_pItem = NULL;
	}
}


BEGIN_MESSAGE_MAP(COBJDoc, CGridDoc)
	ON_COMMAND(ID_OBJ_EDIT_ATTR, OnObjEditAttr)
END_MESSAGE_MAP()


// COBJDoc 진단입니다.

#ifdef _DEBUG
void COBJDoc::AssertValid() const
{
	CGridDoc::AssertValid();
}

void COBJDoc::Dump(CDumpContext& dc) const
{
	CGridDoc::Dump(dc);
}
#endif //_DEBUG


// COBJDoc serialization입니다.

void COBJDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}


// COBJDoc 명령입니다.

CPlayCtrl *COBJDoc::GetPlayCtrl()
{
	COBJFrame *pFrame = (COBJFrame *) GetFrameWnd();
	return pFrame ? pFrame->m_wndPlayCtrlBar.GetPlayCtrl() : NULL;
}

COBJBar *COBJDoc::GetOBJBar()
{
	COBJFrame *pFrame = (COBJFrame *) GetFrameWnd();
	return pFrame ? &pFrame->m_wndOBJBar : NULL;
}

void COBJDoc::InitData( LPVOID pData)
{
	CGridDoc::InitData(pData);
	if(!pData)
		return;

	m_pItem = (COBJItem *) pData;
	m_pItem->SetMainDocument(this);

	for( int i=0; i<MAX_PIVOT; i++)
	{
		D3DXQuaternionIdentity(&m_pItem->m_TOBJ.m_pPivot[i].m_vRot);
		D3DXMatrixIdentity(&m_pItem->m_TOBJ.m_pBone[i]);

		m_pItem->m_TOBJ.m_pPivot[i].m_vScale.x = 1.0f;
		m_pItem->m_TOBJ.m_pPivot[i].m_vScale.y = 1.0f;
		m_pItem->m_TOBJ.m_pPivot[i].m_vScale.z = 1.0f;
	}

	D3DXMatrixIdentity(&m_pItem->m_TOBJ.m_vPosition);
	D3DXMatrixIdentity(&m_pItem->m_TOBJ.m_vWorld);
	D3DXMatrixIdentity(&m_pItem->m_TOBJ.m_vScale);

	m_pItem->m_TOBJ.m_fActTime = 0.0f;
	m_pItem->m_TOBJ.m_dwTexTick = 0;
	m_pItem->m_TOBJ.m_nKeyBase = -1;

	Update();
}

void COBJDoc::Update()
{
	MAPDWORD::iterator itEO = m_mapEQUIP.begin();
	COBJBar *pControl = GetOBJBar();

	while( itEO != m_mapEQUIP.end() )
	{
		COBJItem *pEO = CItemBase::GetOBJItem((*itEO).second);

		if(!m_pItem)
		{
			MAPDWORD::iterator itNEXT = itEO;
			itNEXT++;

			m_mapEQUIP.erase(itEO);
			itEO = itNEXT;
		}
		else if( !pEO || pEO->m_bDeleted )
		{
			MAPOBJECT::iterator itOBJ = m_pItem->m_TOBJ.m_mapEQUIP.find((*itEO).first);
			MAPDWORD::iterator itNEXT = itEO;
			itNEXT++;

			if( itOBJ != m_pItem->m_TOBJ.m_mapEQUIP.end() )
			{
				CTachyonObject *pOBJ = (*itOBJ).second;

				if(pOBJ)
					delete pOBJ;

				m_pItem->m_TOBJ.m_mapEQUIP.erase(itOBJ);
			}

			m_mapEQUIP.erase(itEO);
			itEO = itNEXT;
		}
		else
		{
			MAPOBJECT::iterator itOBJ = m_pItem->m_TOBJ.m_mapEQUIP.find((*itEO).first);
			pEO->Regulate();

			if( itOBJ != m_pItem->m_TOBJ.m_mapEQUIP.end() )
			{
				CTachyonObject *pOBJ = (*itOBJ).second;

				if(!pOBJ)
				{
					pOBJ = new CTachyonObject();
					pOBJ->InitOBJ(&pEO->m_OBJ);
					pOBJ->m_pHost = pControl;

					(*itOBJ).second = pOBJ;
				}
				else
					pEO->Regulate(pOBJ);
			}
			else
			{
				CTachyonObject *pOBJ = new CTachyonObject();
				pOBJ->m_pHost = pControl;
				pOBJ->InitOBJ(&pEO->m_OBJ);

				m_pItem->m_TOBJ.m_mapEQUIP.insert( MAPOBJECT::value_type( (*itEO).first, pOBJ));
			}

			itEO++;
		}
	}

	if(m_pItem)
	{
		SetTitle(m_pItem->m_strName);
		m_pItem->Regulate();

		m_pItem->m_TOBJ.m_pHost = pControl;
		if(pControl)
			pControl->SetItem(m_pItem);
	}
}

void COBJDoc::OnObjEditAttr()
{
	COBJBar *pControl = GetOBJBar();
	CEditObjectDlg dlg;

	dlg.m_pItem = m_pItem;
	dlg.DoModal();

	pControl->SetActionList();
	pControl->SetClothList();

	if( m_pItem->GetPivotCount() == -1 )
		pControl->m_cPIVOT.ResetContent();
}

void COBJDoc::ResetCamera()
{
	((COBJFrame *) GetFrameWnd())->ResetCamera();
}

void COBJDoc::RedrawAllView()
{
	COBJFrame *pFrame = (COBJFrame *) GetFrameWnd();

	if(!m_bAutoDelete)
		return;

	for( int i=0; i<2; i++)
		for( int j=0; j<2; j++)
			pFrame->m_pGridView[i][j]->Render();
}

void COBJDoc::CalcSFX( DWORD dwTick)
{
	MAPTSFX::iterator it = CTachyonSFX::m_mapINST.begin();
	COBJBar *pControl = GetOBJBar();

	while(it != CTachyonSFX::m_mapINST.end())
	{
		MAPTSFX::iterator itNEXT = it;
		itNEXT++;

		if( (*it).second->m_pSFX && (*it).second->m_pHost == pControl )
			if(!(*it).second->IsDead())
				(*it).second->m_dwInitTick = dwTick;
			else if(!(*it).second->m_bHasHost)
				CTachyonSFX::Release((*it).first);

		it = itNEXT;
	}
}

void COBJDoc::RemoveSFX()
{
	MAPTSFX::iterator it = CTachyonSFX::m_mapINST.begin();
	COBJBar *pControl = GetOBJBar();

	while(it != CTachyonSFX::m_mapINST.end())
	{
		MAPTSFX::iterator itNEXT = it;
		itNEXT++;

		if( (*it).second->m_pHost == pControl &&
			!(*it).second->m_bHasHost )
			CTachyonSFX::Release((*it).first);

		it = itNEXT;
	}
}
