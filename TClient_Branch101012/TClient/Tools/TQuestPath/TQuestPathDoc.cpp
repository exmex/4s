// TQuestPathDoc.cpp : CTQuestPathDoc 클래스의 구현
//

#include "stdafx.h"
#include "TQuestPath.h"

#include "TQuestPathDoc.h"
#include "TQuestPathView.h"
#include "TQuestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTQuestPathDoc

IMPLEMENT_DYNCREATE(CTQuestPathDoc, CDocument)

BEGIN_MESSAGE_MAP(CTQuestPathDoc, CDocument)
	ON_COMMAND(ID_DB_LOAD_QUEST, OnDbLoadQuest)
	ON_COMMAND(ID_DB_LOAD_GAME, OnDbLoadGame)
END_MESSAGE_MAP()


// CTQuestPathDoc 생성/소멸

CTQuestPathDoc::CTQuestPathDoc()
{
	m_vTGAMEDB.ReleaseDATA();
	m_mapTMISSION.clear();
	m_mapTQCLASS.clear();

	m_pTQITEM = NULL;
}

CTQuestPathDoc::~CTQuestPathDoc()
{
	ReleaseBUF();
}

BOOL CTQuestPathDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

void CTQuestPathDoc::ReleaseBUF()
{
	m_vTGAMEDB.ReleaseDATA();
	m_mapTMISSION.clear();
	m_mapTQCLASS.clear();

	UpdateDATA(NULL);
}


// CTQuestPathDoc serialization

void CTQuestPathDoc::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CTQuestPathDoc 진단

#ifdef _DEBUG
void CTQuestPathDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTQuestPathDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


CTreeCtrl *CTQuestPathDoc::GetQuestTreeCtrl()
{
	POSITION pos = GetFirstViewPosition();

	while(pos)
	{
		CView *pView = GetNextView(pos);

		if( pView && pView->IsKindOf(RUNTIME_CLASS(CTQuestView)) )
			return &((CTQuestView *) pView)->GetTreeCtrl();
	}

	return NULL;
}

BYTE CTQuestPathDoc::LoadTQUEST()
{
	CTreeCtrl *pTree = GetQuestTreeCtrl();
	CTQuestDB vTQUEST;
	UpdateDATA(NULL);

	if(!pTree)
		return FALSE;

	if(!vTQUEST.LoadDATA())
	{
		vTQUEST.ReleaseDATA();
		return FALSE;
	}

	MAPTQDATA::iterator itTQDATA;
	MAPTQITEM::iterator itTQITEM;

	itTQITEM = m_mapTMISSION.begin();
	while(itTQITEM != m_mapTMISSION.end())
	{
		LPTQITEM pTQITEM = (*itTQITEM).second;

		MAPTQITEM::iterator itNEXT = itTQITEM;
		itNEXT++;

		if(pTQITEM)
		{
			LPTQDATA pTQDATA = vTQUEST.FindTQDATA(pTQITEM->m_dwID);

			if( !pTQDATA || pTQDATA->m_bType != QT_MISSION )
				if(!pTQITEM->m_itemPOS)
				{
					m_mapTMISSION.erase(itTQITEM);
					delete pTQITEM;
				}
				else
					pTree->DeleteItem(pTQITEM->m_itemPOS);
		}

		itTQITEM = itNEXT;
	}

	itTQITEM = m_mapTQCLASS.begin();
	while(itTQITEM != m_mapTQCLASS.end())
	{
		LPTQITEM pTQITEM = (*itTQITEM).second;

		MAPTQITEM::iterator itNEXT = itTQITEM;
		itNEXT++;

		if(pTQITEM)
			if(!pTQITEM->m_itemPOS)
			{
				m_mapTQCLASS.erase(itTQITEM);
				delete pTQITEM;
			}
			else if(pTree->ItemHasChildren(pTQITEM->m_itemPOS))
			{
				LPTCDATA pTCDATA = vTQUEST.FindTCDATA(pTQITEM->m_dwID);
				CString strTEXT;

				if(!pTCDATA)
					strTEXT.Format( IDS_FMT_DEF_QCLASS_NAME, pTQITEM->m_dwID);
				else
					strTEXT = pTCDATA->m_strNAME;

				pTree->SetItemText( pTQITEM->m_itemPOS, strTEXT);
			}
			else
				pTree->DeleteItem(pTQITEM->m_itemPOS);

		itTQITEM = itNEXT;
	}

	for( itTQDATA = vTQUEST.m_mapTQDATA.begin(); itTQDATA != vTQUEST.m_mapTQDATA.end(); itTQDATA++)
	{
		LPTQDATA pTQDATA = (*itTQDATA).second;

		if( pTQDATA && pTQDATA->m_bType == QT_COMPLETE )
		{
			LPTQDATA pTMISSION = vTQUEST.FindTMISSION(pTQDATA);

			if(pTMISSION)
			{
				LPTQITEM pTQCLASS = FindTQCLASS(pTMISSION->m_dwClassID);
				LPTQITEM pTQITEM = FindTMISSION(pTMISSION->m_dwQuestID);

				if(!pTQCLASS)
				{
					LPTCDATA pTEMP = vTQUEST.FindTCDATA(pTMISSION->m_dwClassID);
					CString strTEXT;

					if(!pTEMP)
						strTEXT.Format( IDS_FMT_DEF_QCLASS_NAME, pTMISSION->m_dwClassID);
					else
						strTEXT = pTEMP->m_strNAME;

					pTQCLASS = new TQITEM();
					pTQCLASS->m_dwID = pTMISSION->m_dwClassID;
					pTQCLASS->m_bType = TQITEM_CLASS;
					pTQCLASS->m_itemPOS = pTree->InsertItem(strTEXT);

					m_mapTQCLASS.insert( MAPTQITEM::value_type( pTQCLASS->m_dwID, pTQCLASS));
					pTree->SetItemData( pTQCLASS->m_itemPOS, (DWORD_PTR) pTQCLASS);
				}

				if(!pTQITEM)
				{
					pTQITEM = new TQITEM();

					pTQITEM->m_dwID = pTMISSION->m_dwQuestID;
					pTQITEM->m_bType = TQITEM_MISSION;

					pTQITEM->m_itemPOS = pTree->InsertItem(
						pTMISSION->m_strTITLE,
						pTQCLASS->m_itemPOS);

					m_mapTMISSION.insert( MAPTQITEM::value_type( pTQITEM->m_dwID, pTQITEM));
					pTree->SetItemData( pTQITEM->m_itemPOS, (DWORD_PTR) pTQITEM);
				}
				else if( pTQCLASS->m_itemPOS != pTree->GetParentItem(pTQITEM->m_itemPOS) )
				{
					pTree->SetItemData( pTQITEM->m_itemPOS, NULL);
					pTree->DeleteItem(pTQITEM->m_itemPOS);

					pTQITEM->m_itemPOS = pTree->InsertItem(
						pTMISSION->m_strTITLE,
						pTQCLASS->m_itemPOS);

					pTree->SetItemData( pTQITEM->m_itemPOS, (DWORD_PTR) pTQITEM);
				}
				else
					pTree->SetItemText( pTQITEM->m_itemPOS, pTMISSION->m_strTITLE);

				if(pTQITEM->m_pComplete)
					delete pTQITEM->m_pComplete;

				if(pTQITEM->m_pMission)
					delete pTQITEM->m_pMission;

				pTQITEM->m_pMission = pTMISSION;
				pTQITEM->m_pComplete = pTQDATA;
				ResetTERM(pTQITEM);
			}
		}
	}

	for( itTQITEM = m_mapTMISSION.begin(); itTQITEM != m_mapTMISSION.end(); itTQITEM++)
	{
		LPTQITEM pTQITEM = (*itTQITEM).second;

		if(pTQITEM->m_pComplete)
		{
			itTQDATA = vTQUEST.m_mapTQDATA.find(pTQITEM->m_pComplete->m_dwQuestID);

			if( itTQDATA != vTQUEST.m_mapTQDATA.end() )
				vTQUEST.m_mapTQDATA.erase(itTQDATA);
		}

		if(pTQITEM->m_pMission)
		{
			itTQDATA = vTQUEST.m_mapTQDATA.find(pTQITEM->m_pMission->m_dwQuestID);

			if( itTQDATA != vTQUEST.m_mapTQDATA.end() )
				vTQUEST.m_mapTQDATA.erase(itTQDATA);
		}
	}

	pTree->SortChildren(TVI_ROOT);
	vTQUEST.ReleaseDATA();

	return TRUE;
}

void CTQuestPathDoc::ResetTERM( LPTQITEM pTQITEM)
{
	VTTITEM vTTITEM;
	vTTITEM.clear();

	for( int i=0; i<INT(pTQITEM->m_vTTERM.size()); i++)
		vTTITEM.push_back(pTQITEM->m_vTTERM[i]);
	pTQITEM->m_vTTERM.clear();

	LPTTITEM pTTITEM = PickTTITEM(
		&vTTITEM,
		QTT_TSTART_POS, 0);

	if(!pTTITEM)
	{
		pTTITEM = new TTITEM();

		pTTITEM->m_vTTERM.m_bTermType = QTT_TSTART_POS;
		pTTITEM->m_vTTERM.m_dwTermID = 0;
	}

	pTTITEM->m_vTTERM.m_strTermMSG = pTQITEM->m_pMission->m_strTriggerMSG;
	pTQITEM->m_vTTERM.push_back(pTTITEM);

	for( i=0; i<INT(pTQITEM->m_pMission->m_vTTERM.size()); i++)
	{
		LPTTDATA pTTDATA = pTQITEM->m_pMission->m_vTTERM[i];

		if( pTTDATA && pTTDATA->m_bTermType != QTT_COMPQUEST )
		{
			pTTITEM = PickTTITEM(
				&vTTITEM,
				pTTDATA->m_bTermType,
				pTTDATA->m_dwTermID);

			if(!pTTITEM)
			{
				pTTITEM = new TTITEM();

				pTTITEM->m_vTTERM.m_bTermType = pTTDATA->m_bTermType;
				pTTITEM->m_vTTERM.m_dwTermID = pTTDATA->m_dwTermID;
			}

			pTTITEM->m_vTTERM.m_strTermMSG = pTTDATA->m_strTermMSG;
			pTQITEM->m_vTTERM.push_back(pTTITEM);
		}
	}

	pTTITEM = PickTTITEM(
		&vTTITEM,
		QTT_TCOMP_POS, 0);

	if(!pTTITEM)
	{
		pTTITEM = new TTITEM();

		pTTITEM->m_vTTERM.m_bTermType = QTT_TCOMP_POS;
		pTTITEM->m_vTTERM.m_dwTermID = 0;
	}

	pTTITEM->m_vTTERM.m_strTermMSG = pTQITEM->m_pMission->m_strCompleteMSG;
	pTQITEM->m_vTTERM.push_back(pTTITEM);

	while(!vTTITEM.empty())
	{
		delete vTTITEM.back();
		vTTITEM.pop_back();
	}
}

LPTTITEM CTQuestPathDoc::PickTTITEM( LPVTTITEM pVTTITEM,
									 BYTE bType,
									 DWORD dwID)
{
	VTTITEM::iterator it;

	for( it = pVTTITEM->begin(); it != pVTTITEM->end(); it++)
	{
		LPTTITEM pTTITEM = (*it);

		if( pTTITEM && pTTITEM->m_vTTERM.m_bTermType == bType && pTTITEM->m_vTTERM.m_dwTermID == dwID )
		{
			pVTTITEM->erase(it);
			return pTTITEM;
		}
	}

	return NULL;
}

LPTQITEM CTQuestPathDoc::FindTMISSION( DWORD dwID)
{
	MAPTQITEM::iterator finder = m_mapTMISSION.find(dwID);

	if( finder != m_mapTMISSION.end() )
		return (*finder).second;

	return NULL;
}

LPTQITEM CTQuestPathDoc::FindTQCLASS( DWORD dwID)
{
	MAPTQITEM::iterator finder = m_mapTQCLASS.find(dwID);

	if( finder != m_mapTQCLASS.end() )
		return (*finder).second;

	return NULL;
}

CString CTQuestPathDoc::GetTNPCName( WORD wID)
{
	CString strRESULT;

	if(wID)
	{
		LPTNDATA pTNDATA = m_vTGAMEDB.FindTNDATA(wID);

		if(pTNDATA)
		{
			CString strTITLE = pTNDATA->m_strTITLE;
			CString strNAME = pTNDATA->m_strNAME;

			strTITLE.Replace( '_', ' ');
			strNAME.Replace( '_', ' ');

			strTITLE.Trim();
			strNAME.Trim();

			if(!strTITLE.IsEmpty())
				strRESULT.Format( IDS_FMT_NPC_NAME, strTITLE, strNAME);
			else
				strRESULT = strNAME;
		}
		else
			strRESULT.LoadString(IDS_NPC_NAME_NOT_FOUND);
	}
	else
		strRESULT.LoadString(IDS_NPC_NAME_NONE);

	return strRESULT;
}

void CTQuestPathDoc::UpdateDATA( LPTQITEM pTQITEM)
{
	POSITION pos = GetFirstViewPosition();
	m_pTQITEM = pTQITEM;

	while(pos)
	{
		CView *pView = GetNextView(pos);

		if( pView && pView->IsKindOf(RUNTIME_CLASS(CTQuestPathView)) )
		{
			((CTQuestPathView *) pView)->UpdateDATA();
			break;
		}
	}
}

void CTQuestPathDoc::OnDbLoadQuest()
{
	if(!LoadTQUEST())
		AfxMessageBox(IDS_ERR_LOAD_QUEST);
}

void CTQuestPathDoc::OnDbLoadGame()
{
	if(!m_vTGAMEDB.LoadDATA())
		AfxMessageBox(IDS_ERR_LOAD_GAME);
}

BYTE CTQuestPathDoc::GetAutosetData( LPTTITEM pTRESULT,
									 LPTQITEM pTQITEM,
									 LPTTITEM pTTITEM)
{
	if( !pTRESULT || !pTQITEM || !pTTITEM )
		return FALSE;

	switch(pTTITEM->m_vTTERM.m_bTermType)
	{
	case QTT_TSTART_POS	:
		{
			if( !pTQITEM->m_pMission || pTQITEM->m_pMission->m_bTriggerType != TT_TALKNPC )
				return FALSE;

			LPTNDATA pTNDATA = m_vTGAMEDB.FindTNDATA(pTQITEM->m_pMission->m_dwTriggerID);
			if(!pTNDATA)
				return FALSE;

			pTRESULT->m_dwMapID = pTNDATA->m_wMapID;
			pTRESULT->m_dwNpcID = pTNDATA->m_wID;

			pTRESULT->m_fPosX = pTNDATA->m_fPosX;
			pTRESULT->m_fPosY = pTNDATA->m_fPosY;
			pTRESULT->m_fPosZ = pTNDATA->m_fPosZ;
		}

		break;

	case QTT_TCOMP_POS	:
		{
		}

		break;
	}

	return TRUE;
}
