// TQuestEditorDoc.cpp : implementation of the CTQuestEditorDoc class
//

#include "stdafx.h"
#include "TQuestEditor.h"

#include "TQuestEditorDoc.h"
#include "TQuestEditorView.h"
#include "TQuestView.h"

#include "TConditionSet.h"
#include "TTermTextSet.h"
#include "TRewardSet.h"
#include "TQClassSet.h"
#include "TQuestSet.h"
#include "TTitleSet.h"
#include "TTermSet.h"

#include "TQClassDlg.h"
#include "FindQuestTitleDlg.h"
#include "FindQuestListDlg.h"
#include "TChangeParentDlg.h"
#include "MainFrm.h"
#include ".\tquesteditordoc.h"
#include "TMonsterChartSet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorDoc

IMPLEMENT_DYNCREATE(CTQuestEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CTQuestEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CTQuestEditorDoc)
	ON_COMMAND(ID_DATABASE_OPEN, OnDatabaseOpen)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_OPEN, OnUpdateDatabaseOpen)
	ON_COMMAND(ID_DATABASE_CLOSE, OnDatabaseClose)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_CLOSE, OnUpdateDatabaseClose)
	ON_COMMAND(ID_QUEST_ADD, OnQuestAdd)
	ON_UPDATE_COMMAND_UI(ID_QUEST_ADD, OnUpdateQuestAdd)
	ON_COMMAND(ID_QUEST_DELETE, OnQuestDelete)
	ON_UPDATE_COMMAND_UI(ID_QUEST_DELETE, OnUpdateQuestDelete)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_EDIT_CLASS, OnEditClass)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLASS, OnUpdateEditClass)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_QUEST_FIND, OnQuestFind)
	ON_COMMAND(ID_FIND, OnFind)
	ON_COMMAND(ID_QUEST_CHGPARENT, OnQuestChgparent)
	ON_COMMAND(ID_QUEST_PRINTSUMMARY, OnQuestPrintSummary)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorDoc construction/destruction

CTQuestEditorDoc::CTQuestEditorDoc()
{
	m_vNULLClass.m_strNAME.Format("None");
	m_vNULLClass.m_dwClassID = 0;
	m_vNULLClass.m_bClassMain = 0;
	m_pTQuestView = NULL;
	m_pClassTitleDlg = NULL;
}

CTQuestEditorDoc::~CTQuestEditorDoc()
{
	if(m_pClassTitleDlg != NULL)
	{
		m_pClassTitleDlg->DestroyWindow();
		delete m_pClassTitleDlg;
		m_pClassTitleDlg = NULL;
	}
	ReleaseTQClass();
}

BOOL CTQuestEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorDoc serialization

void CTQuestEditorDoc::Serialize( CArchive& ar)
{
	if(ar.IsStoring())
	{
		MAPTQCLASS::iterator it;

		CTreeCtrl *pTree = GetTreeCtrl();
		CFile *pFILE = ar.GetFile();

		int nCount = INT(m_mapTQCLASS.size());
		pFILE->Write( &nCount, sizeof(int));

		for( it = m_mapTQCLASS.begin(); it != m_mapTQCLASS.end(); it++)
		{
			LPTQCLASS pTQCLASS = (*it).second;

			pFILE->Write( &pTQCLASS->m_dwClassID, sizeof(DWORD));
			int nLength = pTQCLASS->m_strNAME.GetLength();
			pFILE->Write( &nLength, sizeof(int));
			pFILE->Write( LPCTSTR(pTQCLASS->m_strNAME), nLength);
			pFILE->Write( &pTQCLASS->m_bClassMain, sizeof(BYTE));
		}

		SaveMPQ( pFILE, pTree->GetRootItem(), pTree);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorDoc diagnostics

#ifdef _DEBUG
void CTQuestEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTQuestEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorDoc commands

void CTQuestEditorDoc::OnDatabaseOpen() 
{
	if(m_db.Open("TQUEST"))
		LoadDataProc();
}

void CTQuestEditorDoc::OnUpdateDatabaseOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_db.IsOpen());
}

void CTQuestEditorDoc::OnDatabaseClose() 
{
	CTreeCtrl *pTree = GetTreeCtrl();

	m_db.Close();
	pTree->DeleteAllItems();
	UpdateData(NULL);
}

void CTQuestEditorDoc::OnUpdateDatabaseClose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_db.IsOpen());
}

void CTQuestEditorDoc::OnQuestAdd() 
{
	AddQuest(NULL);
}

void CTQuestEditorDoc::OnUpdateQuestAdd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_db.IsOpen());
}

void CTQuestEditorDoc::OnQuestFind()
{
	enum
	{
		_SENT_SEARCH = 0,
		_WORD_SEARCH,
		_ID_SEARCH
	};

	CTreeCtrl *pTree = GetTreeCtrl();
	HTREEITEM item = pTree->GetRootItem();
	HTREEITEM nextItem;
	CString *strSearch = new CString;
	INT nSearchType = 0;

	if( !(nextItem = pTree->GetNextItem(item,TVGN_NEXTVISIBLE)) )
	{
		AfxMessageBox("No More Quest",MB_OK | MB_ICONWARNING );
		return;
	}

	LPTQUEST pQT = (LPTQUEST) pTree->GetItemData(nextItem);
	CString sTe = pQT->m_strTitle;

	CFindQuestTitleDlg dlgFindTitle;
	if( dlgFindTitle.DoModal() == IDOK )
	{
		dlgFindTitle.GetSearchString( strSearch );
		dlgFindTitle.GetSearchType(&nSearchType);
	}
	else
		return;

	VECHITEM vHItem;
	VECHITEM vHSearchItem;
	VECHITEM::iterator itVHITEM;
	GetQuestTitle(&vHItem, nextItem);

	switch( nSearchType )
	{
	case _SENT_SEARCH:
		for( itVHITEM = vHItem.begin() ; itVHITEM != vHItem.end() ; itVHITEM++ )
		{
			LPTQUEST pQUEST = (LPTQUEST) pTree->GetItemData((*itVHITEM));
			CString sTitle = pQUEST->m_strTitle;
			if( sTitle.Find( (*strSearch) ) != -1 )
				vHSearchItem.push_back((*itVHITEM));
		}
		break;
	case _WORD_SEARCH:
		for( itVHITEM = vHItem.begin() ; itVHITEM != vHItem.end() ; itVHITEM++ )
		{
			LPTQUEST pQUEST = (LPTQUEST) pTree->GetItemData((*itVHITEM));
			CString sTitle = pQUEST->m_strTitle;

			CString sToken;
			int nPos = 0;
			int nWordCnt = 0;
			int nFindCnt = 0;
			do 
			{
				sToken = strSearch->Tokenize(" ",nPos);
				if( !sToken.IsEmpty() )
				{
					if( sTitle.Find( sToken ) != -1 )
					{ 
						nFindCnt++;
					}
					nWordCnt++;
				}
			}
			while( sToken != "" );

			if( nWordCnt == nFindCnt )
				vHSearchItem.push_back((*itVHITEM));
		}
		break;
	case _ID_SEARCH:
        for( itVHITEM = vHItem.begin() ; itVHITEM != vHItem.end() ; itVHITEM++ )
		{
			LPTQUEST pQUEST = (LPTQUEST) pTree->GetItemData((*itVHITEM));
			DWORD dwSearchID = 0;
			sscanf( (*strSearch), "%ld", &dwSearchID);

			if(pQUEST->m_dwQuestID == dwSearchID)
				vHSearchItem.push_back((*itVHITEM));
		}
	}
	if( !vHSearchItem.empty() )
	{
		CFindQuestListDlg dlgFindList;
		dlgFindList.InitListData( pTree, &vHSearchItem );
		if( dlgFindList.DoModal() == IDOK )
		{
			dlgFindList.GetSelectedItem(&item);
			if( item != NULL )
			{
				pTree->SelectItem(item);
				LPTQUEST pQUEST = (LPTQUEST) pTree->GetItemData(item);
				CString sTitle = pQUEST->m_strTitle;

				ParentExpand( item );
			}
		}
	}
	else
		AfxMessageBox("Not Find",MB_OK);

	vHItem.clear();
	vHSearchItem.clear();
	delete strSearch;
}

void CTQuestEditorDoc::ParentExpand(HTREEITEM  hItem)
{
	CTreeCtrl *pTree = GetTreeCtrl();
	HTREEITEM hParentItem;
	if( (hParentItem = pTree->GetParentItem(hItem)) != NULL )
	{
		ParentExpand( hParentItem );
	}
	pTree->Expand(hItem, TVE_EXPAND);
}

void CTQuestEditorDoc::ChildExpand(HTREEITEM  hItem)
{
	CTreeCtrl *pTree = GetTreeCtrl();
	HTREEITEM hChildItem;
	if( (hChildItem = pTree->GetChildItem(hItem)) != NULL )
		ChildExpand( hChildItem );
	pTree->Expand(hItem, TVE_EXPAND);

	HTREEITEM hNextItem;
	if( (hNextItem = pTree->GetNextItem(hItem, TVGN_NEXT)) != NULL )
		ChildExpand( hNextItem );
}

void CTQuestEditorDoc::ChildCollapse(HTREEITEM hItem)
{
	CTreeCtrl *pTree = GetTreeCtrl();
	HTREEITEM hChildItem;
	if( (hChildItem = pTree->GetChildItem(hItem)) != NULL )
		ChildCollapse( hChildItem );
	pTree->Expand(hItem, TVE_COLLAPSE);

	HTREEITEM hNextItem;
	if( (hNextItem = pTree->GetNextItem(hItem, TVGN_NEXT)) != NULL )
		ChildCollapse( hNextItem );
}

void CTQuestEditorDoc::GetQuestTitle(VECHITEM* pVECHITEM, HTREEITEM hItem)
{
	CTreeCtrl *pTree = GetTreeCtrl();
	while( hItem )
	{
		if( pTree->ItemHasChildren( hItem ) )
			GetQuestTitle( pVECHITEM, pTree->GetChildItem(hItem) );

		pVECHITEM->push_back( hItem );
		hItem = pTree->GetNextSiblingItem( hItem );
	}
}

void CTQuestEditorDoc::OnQuestDelete() 
{
	CTreeCtrl *pTree = GetTreeCtrl();
	HTREEITEM itemPos = pTree->GetSelectedItem();

	if( itemPos
		&& ( AfxMessageBox( "퀘스트를 삭제하시겠습니까?", MB_YESNO | MB_ICONSTOP ) == IDYES ) )
		DeleteData( pTree, itemPos);
}

void CTQuestEditorDoc::OnUpdateQuestDelete(CCmdUI* pCmdUI) 
{
	CTreeCtrl *pTree = GetTreeCtrl();
	HTREEITEM itemPos = pTree ? pTree->GetSelectedItem() : NULL;

	pCmdUI->Enable(m_db.IsOpen() && itemPos && pTree->GetItemData(itemPos));
}

CTreeCtrl* CTQuestEditorDoc::GetTreeCtrl()
{
	POSITION pos = GetFirstViewPosition();

	while(pos)
	{
		CView *pView = GetNextView(pos);

		if( pView && pView->IsKindOf(RUNTIME_CLASS(CTQuestEditorView)) )
			return &((CTQuestEditorView *) pView)->GetTreeCtrl();
	}

	return NULL;
}

void CTQuestEditorDoc::UpdateData( LPTQUEST pQUEST)
{
	POSITION pos = GetFirstViewPosition();

	while(pos)
	{
		CView *pView = GetNextView(pos);

		if( pView && pView->IsKindOf(RUNTIME_CLASS(CTQuestView)) )
			((CTQuestView *) pView)->UpdateData(pQUEST);

	}
}

void CTQuestEditorDoc::LoadData( CTreeCtrl *pTree, HTREEITEM itemPos)
{
	LPTQUEST pQUEST = (LPTQUEST) pTree->GetItemData(itemPos);
	CTQuestSet setQuest(&m_db);

	setQuest.m_strFilter.Format( "dwParentID = %d", pQUEST ? pQUEST->m_dwQuestID : 0);
	if(setQuest.Open())
	{
		if(!setQuest.IsEOF())
			setQuest.MoveFirst();

		while(!setQuest.IsEOF())
		{
			CTTitleSet setTitle(&m_db);

			setTitle.m_strFilter.Format( "dwQuestID = %d", setQuest.m_dwQuestID);
			if(setTitle.Open())
			{
				if(!setTitle.IsEOF())
				{
					pQUEST = new TQUEST();
					((CMainFrame *) AfxGetMainWnd())->SetProgressBar();

					pQUEST->m_strComplete = setTitle.m_szComplete;
					pQUEST->m_strMessage = setTitle.m_szMessage;
					pQUEST->m_strAccept = setTitle.m_szAccept;
					pQUEST->m_strReject = setTitle.m_szReject;
					pQUEST->m_strSummary = setTitle.m_szSummary;
					pQUEST->m_strTitle = setTitle.m_szTitle;
					pQUEST->m_strNPCName = setTitle.m_szNPCName;
					pQUEST->m_strReply = setTitle.m_szReply;

					pQUEST->m_dwTriggerID = setQuest.m_dwTriggerID;
					pQUEST->m_dwParentID = setQuest.m_dwParentID;
					pQUEST->m_dwQuestID = setQuest.m_dwQuestID;
					pQUEST->m_dwClassID = setTitle.m_dwClassID;

					pQUEST->m_bTriggerType = setQuest.m_bTriggerType;
					pQUEST->m_bForceRun = setQuest.m_bForceRun;
					pQUEST->m_bMain = setQuest.m_bMain;
					pQUEST->m_bConditionCheck = setQuest.m_bConditionCheck;
					
					pQUEST->m_bLevel = setQuest.m_bLevel;
					pQUEST->m_bType = setQuest.m_bType;
					pQUEST->m_bCountMax = setQuest.m_bCountMax;

					// 01/19 박상연 퀘스트 + 퀘스트 아이디
					//-------------------------------------------------------------------------
					CHAR strQuestID[255];
					sprintf( strQuestID, "%d", pQUEST->m_dwQuestID );
					setTitle.m_szTitle	  = pQUEST->m_strTitle + "( " + strQuestID + " )";	
					//-------------------------------------------------------------------------

					HTREEITEM itemNew = pTree->InsertItem( setTitle.m_szTitle, 0, 1, itemPos);
					pTree->SetItemData( itemNew, (DWORD) pQUEST);

					CTConditionSet setCondition(&m_db);
					CTTermTextSet setTermText(&m_db);
					CTTermSet setTerm(&m_db);
					CTRewardSet setReward(&m_db);

					setCondition.m_strFilter.Format( "dwQuestID = %d", pQUEST->m_dwQuestID);
					if(setCondition.Open())
					{
						if(!setCondition.IsEOF())
							setCondition.MoveFirst();

						while(!setCondition.IsEOF())
						{
							LPTCONDITION pCONDITION = new TCONDITION;

							pCONDITION->m_bConditionType = setCondition.m_bConditionType;
							pCONDITION->m_bCount = setCondition.m_bCount;
							pCONDITION->m_dwConditionID = setCondition.m_dwConditionID;
							pCONDITION->m_dwID = setCondition.m_dwID;

							pQUEST->m_vCondition.push_back(pCONDITION);
							setCondition.MoveNext();
						}

						setCondition.Close();
					}

					setReward.m_strFilter.Format( "dwQuestID = %d", pQUEST->m_dwQuestID);
					if(setReward.Open())
					{
						if(!setReward.IsEOF())
							setReward.MoveFirst();

						while(!setReward.IsEOF())
						{
							LPTQUESTREWARD pREWARD = new TQUESTREWARD;

							pREWARD->m_bRewardType = setReward.m_bRewardType;
							pREWARD->m_dwRewardID = setReward.m_dwRewardID;
							pREWARD->m_bTakeMethod = setReward.m_bTakeMethod;
							pREWARD->m_bTakeData = setReward.m_bTakeData;
							pREWARD->m_bCount = setReward.m_bCount;
							pREWARD->m_dwID = setReward.m_dwID;
							pREWARD->m_dwQuestMob = setReward.m_dwQuestMob;
							pREWARD->m_dwQuestTime = setReward.m_dwQuestTime;
							pREWARD->m_dwQuestPathMob = setReward.m_dwQuestPathMob;
							pREWARD->m_dwTicketID = setReward.m_dwTicketID;
							pREWARD->m_bSendQ = setReward.m_bSendQ;
							
							pQUEST->m_vReward.push_back(pREWARD);
							setReward.MoveNext();
						}

						setReward.Close();
					}

					setTerm.m_strFilter.Format( "dwQuestID = %d", pQUEST->m_dwQuestID);
					if(setTerm.Open())
					{
						if(!setTerm.IsEOF())
							setTerm.MoveFirst();

						while(!setTerm.IsEOF())
						{
							LPTQUESTTERM pTERM = new TQUESTTERM();

							pTERM->m_bTermType = setTerm.m_bTermType;
							pTERM->m_dwTermID = setTerm.m_dwTermID;
							pTERM->m_bCount = setTerm.m_bCount;
							pTERM->m_dwID = setTerm.m_dwID;

							setTermText.m_strFilter.Format( "dwTermID = %d", pTERM->m_dwID);
							if(setTermText.Open())
							{
								if(!setTermText.IsEOF())
								{
									setTermText.MoveFirst();

									pTERM->m_strObjective = setTermText.m_szObjective;
									pTERM->m_strMessage = setTermText.m_szMessage;
								}

								setTermText.Close();
							}

							pQUEST->m_vTerm.push_back(pTERM);
							setTerm.MoveNext();
						}

						setTerm.Close();
					}
				}

				setTitle.Close();
			}

			pTree->Expand( pTree->GetRootItem(), TVE_EXPAND);
			setQuest.MoveNext();
		}

		setQuest.Close();
	}

	HTREEITEM itemChild = pTree->GetChildItem(itemPos);
	while(itemChild)
	{
		LoadData( pTree, itemChild);
		itemChild = pTree->GetNextSiblingItem(itemChild);
	}
}

void CTQuestEditorDoc::DeleteData( CTreeCtrl *pTree, HTREEITEM itemPos)
{
	LPTQUEST pQUEST = (LPTQUEST) pTree->GetItemData(itemPos);
	HTREEITEM itemChild = pTree->GetChildItem(itemPos);

	while(itemChild)
	{
		DeleteData( pTree, itemChild);
		itemChild = pTree->GetChildItem(itemPos);
	}

	if(pQUEST)
	{
		CTConditionSet setCondition(&m_db);
		CTTermTextSet setTermText(&m_db);
		CTRewardSet setReward(&m_db);
		CTQuestSet setQuest(&m_db);
		CTTitleSet setTitle(&m_db);
		CTTermSet setTerm(&m_db);

		setQuest.m_strFilter.Format( "dwQuestID = %d", pQUEST->m_dwQuestID);
		if(setQuest.Open())
		{
			if(!setQuest.IsEOF())
				setQuest.Delete();

			setQuest.Close();
		}

		setTitle.m_strFilter.Format( "dwQuestID = %d", pQUEST->m_dwQuestID);
		if(setTitle.Open())
		{
			if(!setTitle.IsEOF())
				setTitle.Delete();

			setTitle.Close();
		}

		setCondition.m_strFilter.Format( "dwQuestID = %d", pQUEST->m_dwQuestID);
		if(setCondition.Open())
		{
			while(!setCondition.IsEOF())
			{
				setCondition.Delete();
				setCondition.Requery();
			}

			setCondition.Close();
		}

		setReward.m_strFilter.Format( "dwQuestID = %d", pQUEST->m_dwQuestID);
		if(setReward.Open())
		{
			while(!setReward.IsEOF())
			{
				setReward.Delete();
				setReward.Requery();
			}

			setReward.Close();
		}

		setTermText.m_strFilter.Format( "dwQuestID = %d", pQUEST->m_dwQuestID);
		if(setTermText.Open())
		{
			while(!setTermText.IsEOF())
			{
				setTermText.Delete();
				setTermText.Requery();
			}

			setTermText.Close();
		}

		setTerm.m_strFilter.Format( "dwQuestID = %d", pQUEST->m_dwQuestID);
		if(setTerm.Open())
		{
			while(!setTerm.IsEOF())
			{
				setTerm.Delete();
				setTerm.Requery();
			}

			setTerm.Close();
		}
	}

	pTree->DeleteItem(itemPos);
}

void CTQuestEditorDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_db.IsOpen());
}

void CTQuestEditorDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_db.IsOpen());
}

void CTQuestEditorDoc::SaveMPQ( CFile *pFile,
							    HTREEITEM itemPos,
								CTreeCtrl *pTree)
{
	LPTQUEST pQUEST = (LPTQUEST) pTree->GetItemData(itemPos);

	if(pQUEST)
	{
		// Quest ID
		pFile->Write( &pQUEST->m_dwQuestID, sizeof(DWORD));
		pFile->Write( &pQUEST->m_bType, sizeof(BYTE));
		pFile->Write( &pQUEST->m_dwClassID, sizeof(DWORD));
		pFile->Write( &pQUEST->m_bClassMain, sizeof(BYTE));

		// Title
		int nLength = pQUEST->m_strTitle.GetLength();
		pFile->Write( &nLength, sizeof(int));
		pFile->Write( LPCTSTR(pQUEST->m_strTitle), nLength);

		// Trigger Message
		nLength = pQUEST->m_strMessage.GetLength();
		pFile->Write( &nLength, sizeof(int));
		pFile->Write( LPCTSTR(pQUEST->m_strMessage), nLength);

		// Complete Message
		nLength = pQUEST->m_strComplete.GetLength();
		pFile->Write( &nLength, sizeof(int));
		pFile->Write( LPCTSTR(pQUEST->m_strComplete), nLength);

		// Accept Message
		nLength = pQUEST->m_strAccept.GetLength();
		pFile->Write( &nLength, sizeof(int));
		pFile->Write( LPCTSTR(pQUEST->m_strAccept), nLength);

		// Reject Message
		nLength = pQUEST->m_strReject.GetLength();
		pFile->Write( &nLength, sizeof(int));
		pFile->Write( LPCTSTR(pQUEST->m_strReject), nLength);

		// Summary Message
		nLength = pQUEST->m_strSummary.GetLength();
		pFile->Write( &nLength, sizeof(int));
		pFile->Write( LPCTSTR(pQUEST->m_strSummary), nLength);

		// NPC Name
		nLength = pQUEST->m_strNPCName.GetLength();
		pFile->Write( &nLength, sizeof(int));
		pFile->Write( LPCTSTR(pQUEST->m_strNPCName), nLength);

		// Reply Message
		nLength = pQUEST->m_strReply.GetLength();
		pFile->Write( &nLength, sizeof(int));
		pFile->Write( LPCTSTR(pQUEST->m_strReply), nLength);

		// Level
		pFile->Write( &pQUEST->m_bLevel, sizeof(BYTE));

		// Term Count
		int nCount = INT(pQUEST->m_vTerm.size());
		pFile->Write( &nCount, sizeof(int));

		// Term Data
		for( int i=0; i<nCount; i++)
		{
			pFile->Write( &pQUEST->m_vTerm[i]->m_bTermType, sizeof(BYTE));
			pFile->Write( &pQUEST->m_vTerm[i]->m_dwTermID, sizeof(DWORD));
			pFile->Write( &pQUEST->m_vTerm[i]->m_bCount, sizeof(BYTE));

			nLength = pQUEST->m_vTerm[i]->m_strObjective.GetLength();
			pFile->Write( &nLength, sizeof(int));
			pFile->Write( LPCTSTR(pQUEST->m_vTerm[i]->m_strObjective), nLength);

			nLength = pQUEST->m_vTerm[i]->m_strMessage.GetLength();
			pFile->Write( &nLength, sizeof(int));
			pFile->Write( LPCTSTR(pQUEST->m_vTerm[i]->m_strMessage), nLength);
		}

		// Reward Count
		nCount = INT(pQUEST->m_vReward.size());
		pFile->Write( &nCount, sizeof(int));

		// Reward Data
		for( i=0; i<nCount; i++)
		{
			pFile->Write( &pQUEST->m_vReward[i]->m_bRewardType, sizeof(BYTE));
			pFile->Write( &pQUEST->m_vReward[i]->m_dwRewardID, sizeof(DWORD));
			pFile->Write( &pQUEST->m_vReward[i]->m_bCount, sizeof(BYTE));
			pFile->Write( &pQUEST->m_vReward[i]->m_bTakeMethod, sizeof(BYTE));
			pFile->Write( &pQUEST->m_vReward[i]->m_bTakeData, sizeof(BYTE));
		}

		nCount = (INT)pQUEST->m_vCondition.size();

		// Min Level Condition
		DWORD dwMin = 0;
		for( i=0; i<nCount; ++i)
		{
			if( pQUEST->m_vCondition[i]->m_bConditionType == QCT_UPPERLEVEL )
			{
				dwMin = pQUEST->m_vCondition[i]->m_dwConditionID;
				break;
			}
		}
		pFile->Write( &dwMin, sizeof(DWORD));

		// Max Level Condition
		DWORD dwMax = 0;
		for( i=0; i<nCount; ++i)
		{
			if( pQUEST->m_vCondition[i]->m_bConditionType == QCT_LOWERLEVEL )
			{
				dwMin = pQUEST->m_vCondition[i]->m_dwConditionID;
				break;
			}
		}
		pFile->Write( &dwMax, sizeof(DWORD));
	}

	HTREEITEM itemChild = pTree->GetChildItem(itemPos);

	while(itemChild)
	{
		SaveMPQ( pFile, itemChild, pTree);
		itemChild = pTree->GetNextSiblingItem(itemChild);
	}
}

void CTQuestEditorDoc::LoadDataProc()
{
	CTQuestSet setQuest(&m_db);

	if( setQuest.Open() )
	{
		for( int nRow=0; !setQuest.IsEOF(); ++nRow)
			setQuest.MoveNext();
		((CMainFrame *) AfxGetMainWnd())->InitProgressBar(nRow);
		setQuest.Close();

		CTreeCtrl *pTree = GetTreeCtrl();
		HTREEITEM itemPos = pTree->InsertItem( "Root", 0, 1 );

		pTree->SetItemData( itemPos, NULL );
		pTree->SelectItem( itemPos );

		LoadTQClass();
		LoadData( pTree, itemPos);
		ResetTQClassData();
		pTree->RedrawWindow();

		pTree->Expand( itemPos, TVE_EXPAND);
		((CMainFrame *) AfxGetMainWnd())->RemoveProgressBar();
	}
}

void CTQuestEditorDoc::OnEditClass()
{
	CTQClassDlg dlg;

	dlg.m_pTQCLASS = &m_mapTQCLASS;
	dlg.m_pDB = &m_db;

	dlg.DoModal();
	ResetTQClassData();
}

void CTQuestEditorDoc::OnUpdateEditClass( CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_db.IsOpen());
}

void CTQuestEditorDoc::ReleaseTQClass()
{
	MAPTQCLASS::iterator it;

	for( it = m_mapTQCLASS.begin(); it != m_mapTQCLASS.end(); it++)
		delete (*it).second;

	m_mapTQCLASS.clear();
}

void CTQuestEditorDoc::LoadTQClass()
{
	ReleaseTQClass();
	CTQClassSet set(&m_db);

	if(set.Open())
	{
		if(!set.IsEOF())
			set.MoveFirst();

		while(!set.IsEOF())
		{
			LPTQCLASS pTQCLASS = new TQCLASS();

			pTQCLASS->m_dwClassID = set.m_dwClassID;
			pTQCLASS->m_strNAME = set.m_szNAME;
			pTQCLASS->m_bClassMain = set.m_bClassMain;

			m_mapTQCLASS.insert( MAPTQCLASS::value_type( pTQCLASS->m_dwClassID, pTQCLASS));
			set.MoveNext();
		}

		set.Close();
	}
}

void CTQuestEditorDoc::ResetTQClassData()
{
	MAPTQCLASS::iterator it;

	m_pTQuestView->m_cTQClass.ResetContent();
	int nIndex = m_pTQuestView->m_cTQClass.AddString(m_vNULLClass.m_strNAME);
	m_pTQuestView->m_cTQClass.SetItemData( nIndex, (DWORD_PTR) &m_vNULLClass);

	for( it = m_mapTQCLASS.begin(); it != m_mapTQCLASS.end(); it++)
	{
		nIndex = m_pTQuestView->m_cTQClass.AddString((*it).second->m_strNAME);
		m_pTQuestView->m_cTQClass.SetItemData( nIndex, (DWORD_PTR) (*it).second);
	}

	UpdateData(m_pTQuestView->m_pQUEST);
}


void CTQuestEditorDoc::OnFind()
{
	OnQuestFind();
}

BOOL CTQuestEditorDoc::GetClassMain(DWORD dwClassID)
{
	CTQClassSet set(&m_db);

	if(m_pTQuestView->m_pQUEST->m_dwClassID)
	{
		set.m_strFilter.Format("dwClassID = %d", m_pTQuestView->m_pQUEST->m_dwClassID);
		if(set.Open())
		{
			set.Edit();
			m_pTQuestView->m_bClassMain = set.m_bClassMain;
			return set.m_bClassMain;
		}
	}
	return FALSE;
}

void CTQuestEditorDoc::SetClassMain(BYTE bMain)
{
	CTQClassSet set(&m_db);

	if(m_pTQuestView->m_pQUEST->m_dwClassID)
	{
		set.m_strFilter.Format( "dwClassID = %d", m_pTQuestView->m_pQUEST->m_dwClassID);
		if(set.Open())
		{
			set.Edit();
			set.m_bClassMain = bMain;
			set.Update();
			set.Close();

			m_pTQuestView->m_pQUEST->m_bClassMain = bMain;
		}
	}
}

LPTQUEST CTQuestEditorDoc::AddQuest( LPTQUEST pSourceQUEST )
{
	LPTQUEST pQUEST = NULL;

	CTreeCtrl *pTree = GetTreeCtrl();
	CTQuestSet setQuest(&m_db);

	if(setQuest.Open())
	{
		HTREEITEM itemPos = pTree->GetSelectedItem();
		LPTQUEST pParentQUEST = NULL;
		
		if(itemPos)
			pParentQUEST = (LPTQUEST) pTree->GetItemData(itemPos);

		m_db.BeginTrans();
		setQuest.AddNew();
		setQuest.m_dwParentID = pParentQUEST ? pParentQUEST->m_dwQuestID : 0;
		setQuest.m_bTriggerType = pSourceQUEST ? pSourceQUEST->m_bTriggerType : 0;
		setQuest.m_dwTriggerID = pSourceQUEST ? pSourceQUEST->m_dwTriggerID : 0;
		setQuest.m_bCountMax = pSourceQUEST ? pSourceQUEST->m_bCountMax : 0;
		setQuest.m_bForceRun = pSourceQUEST ? pSourceQUEST->m_bForceRun : 0;
		setQuest.m_bMain = pSourceQUEST ? pSourceQUEST->m_bMain : 0;
		setQuest.m_bLevel = pSourceQUEST ? pSourceQUEST->m_bLevel : 1;
		setQuest.m_bType = pSourceQUEST ? pSourceQUEST->m_bType : 0;
		setQuest.Update();
		setQuest.Requery();
		setQuest.MoveLast();

		CTTitleSet setTitle(&m_db);
		setTitle.m_strFilter.Format( "dwQuestID = %d", setQuest.m_dwQuestID);

		if(setTitle.Open())
		{
			setTitle.AddNew();
			setTitle.m_dwQuestID = setQuest.m_dwQuestID;
			setTitle.m_szComplete = pSourceQUEST ? pSourceQUEST->m_strComplete : CString(_T(""));
			setTitle.m_szMessage = pSourceQUEST ? pSourceQUEST->m_strMessage : CString(_T(""));
			setTitle.m_szAccept = pSourceQUEST ? pSourceQUEST->m_strAccept : CString(_T(""));
			setTitle.m_szReject = pSourceQUEST ? pSourceQUEST->m_strReject : CString(_T(""));
			setTitle.m_szSummary = pSourceQUEST ? pSourceQUEST->m_strSummary : CString(_T(""));
			setTitle.m_szNPCName = pSourceQUEST ? pSourceQUEST->m_strNPCName : CString(_T(""));
			setTitle.m_szReply = pSourceQUEST ? pSourceQUEST->m_strReply : CString(_T(""));
			setTitle.m_dwClassID = pSourceQUEST ? pSourceQUEST->m_dwClassID : 0;

			if( pSourceQUEST )
				setTitle.m_szTitle = pSourceQUEST->m_strTitle;
			else
				setTitle.m_szTitle.Format( "New Quest (%d)", setTitle.m_dwQuestID);

			setTitle.Update();
			setTitle.Requery();

			pQUEST = new TQUEST();
			pQUEST->m_dwParentID = setQuest.m_dwParentID;
			pQUEST->m_dwQuestID = setQuest.m_dwQuestID;
			pQUEST->m_dwClassID = setTitle.m_dwClassID;
			pQUEST->m_bCountMax = setQuest.m_bCountMax;
			pQUEST->m_bType = setQuest.m_bType;

			pQUEST->m_bTriggerType = setQuest.m_bTriggerType;
			pQUEST->m_bForceRun = setQuest.m_bForceRun;
			pQUEST->m_bMain = setQuest.m_bMain;
			pQUEST->m_bLevel = setQuest.m_bLevel;

			pQUEST->m_dwTriggerID = setQuest.m_dwTriggerID;
			pQUEST->m_strComplete = setTitle.m_szComplete;
			pQUEST->m_strMessage = setTitle.m_szMessage;
			pQUEST->m_strAccept = setTitle.m_szAccept;
			pQUEST->m_strReject = setTitle.m_szReject;
			pQUEST->m_strSummary = setTitle.m_szSummary;
			pQUEST->m_strNPCName = setTitle.m_szNPCName;
			pQUEST->m_strReply = setTitle.m_szReply;
			pQUEST->m_strTitle = setTitle.m_szTitle;

			HTREEITEM itemNew = pTree->InsertItem( setTitle.m_szTitle, 0, 1, itemPos);
			CString strQuestID;
			strQuestID.Format("%d", pQUEST->m_dwQuestID);
			pTree->SetItemText( itemNew, setTitle.m_szTitle + "( " + strQuestID + " )" );
			pTree->SetItemData( itemNew, (DWORD) pQUEST);
			pTree->Expand( itemPos, TVE_EXPAND);

			m_db.CommitTrans();
			setTitle.Close();
		}
		else
			m_db.Rollback();

		setQuest.Close();
	}

	return pQUEST;
}

void CTQuestEditorDoc::AddTerm( LPTQUEST pQUEST, LPTQUESTTERM pSourceTERM )
{
	if(pQUEST && pSourceTERM)
	{
		LPTQUESTTERM pTERM = new TQUESTTERM();

		CTTermTextSet setTermText(&m_db);
		CTTermSet setTerm(&m_db);

		if(setTerm.Open())
		{
			setTerm.AddNew();
			setTerm.m_dwQuestID = pQUEST->m_dwQuestID;
			setTerm.m_bTermType = pSourceTERM->m_bTermType;
			setTerm.m_dwTermID = pSourceTERM->m_dwTermID;
			setTerm.m_bCount = pSourceTERM->m_bCount;
			setTerm.Update();
			setTerm.Requery();
			setTerm.MoveLast();

			pTERM->m_bTermType = setTerm.m_bTermType;
			pTERM->m_dwTermID = setTerm.m_dwTermID;
			pTERM->m_bCount = setTerm.m_bCount;
			pTERM->m_dwID = setTerm.m_dwID;

			setTerm.Close();
		}

		if(setTermText.Open())
		{
			setTermText.AddNew();
			setTermText.m_dwQuestID = pQUEST->m_dwQuestID;
			setTermText.m_dwTermID = pTERM->m_dwID;
			setTermText.m_szObjective = pSourceTERM->m_strObjective;
			setTermText.m_szMessage = pSourceTERM->m_strMessage;
			setTermText.Update();
			setTermText.Requery();
			setTermText.MoveLast();

			pTERM->m_strObjective = setTermText.m_szObjective;
			pTERM->m_strMessage = setTermText.m_szMessage;

			setTermText.Close();
		}

		pQUEST->m_vTerm.push_back(pTERM);
	}
}

void CTQuestEditorDoc::AddCond( LPTQUEST pQUEST, LPTCONDITION pSourceCOND )
{
	if(pQUEST && pSourceCOND)
	{
		CTConditionSet set(&m_db);

		if(set.Open())
		{
			set.AddNew();
			set.m_dwQuestID = pQUEST->m_dwQuestID;
			set.m_bConditionType = pSourceCOND->m_bConditionType;
			set.m_dwConditionID = pSourceCOND->m_dwConditionID;
			set.m_bCount = pSourceCOND->m_bCount;
			set.Update();
			set.Requery();
			set.MoveLast();

			LPTCONDITION pCONDITION = new TCONDITION;
			pCONDITION->m_bConditionType = set.m_bConditionType;
			pCONDITION->m_dwConditionID = set.m_dwConditionID;
			pCONDITION->m_bCount = set.m_bCount;
			pCONDITION->m_dwID = set.m_dwID;

			pQUEST->m_vCondition.push_back(pCONDITION);
			set.Close();
		}
	}
}

void CTQuestEditorDoc::AddReward( LPTQUEST pQUEST, LPTQUESTREWARD pSourceREWARD )
{
	if(pQUEST && pSourceREWARD)
	{
		CTRewardSet set(&m_db);

		if(set.Open())
		{
			set.AddNew();
			set.m_dwQuestID = pQUEST->m_dwQuestID;
			set.m_bRewardType = pSourceREWARD->m_bRewardType;
			set.m_dwRewardID = pSourceREWARD->m_dwRewardID;
			set.m_bTakeMethod = pSourceREWARD->m_bTakeMethod;
			set.m_bTakeData = pSourceREWARD->m_bTakeData;
			set.m_bCount = pSourceREWARD->m_bCount;
			set.m_dwQuestMob = pSourceREWARD->m_dwQuestMob;
			set.m_dwQuestTime = pSourceREWARD->m_dwQuestTime;
			set.m_dwQuestPathMob = pSourceREWARD->m_dwQuestPathMob;
			set.m_dwTicketID = pSourceREWARD->m_dwTicketID;
			set.m_bSendQ = pSourceREWARD->m_bSendQ;
			set.Update();
			set.Requery();
			set.MoveLast();

			LPTQUESTREWARD pREWARD = new TQUESTREWARD;
			pREWARD->m_bRewardType = set.m_bRewardType;
			pREWARD->m_dwRewardID = set.m_dwRewardID;
			pREWARD->m_bTakeMethod = set.m_bTakeMethod;
			pREWARD->m_bTakeData = set.m_bTakeData;
			pREWARD->m_bCount = set.m_bCount;
			pREWARD->m_dwID = set.m_dwID;
			pREWARD->m_dwQuestMob = set.m_dwQuestMob;
			pREWARD->m_dwQuestPathMob = set.m_dwQuestPathMob;
			pREWARD->m_dwQuestTime = set.m_dwQuestTime;
			pREWARD->m_dwTicketID = set.m_dwTicketID;
			pREWARD->m_bSendQ = set.m_bSendQ;

			pQUEST->m_vReward.push_back(pREWARD);
			set.Close();
		}
	}
}

void CTQuestEditorDoc::OnQuestChgparent()
{
	CTChangeParentDlg dlg;

	if( m_pTQuestView->m_pQUEST && dlg.DoModal() == IDOK &&
		m_pTQuestView->m_pQUEST->m_dwParentID != dlg.m_dwParentID &&
		m_pTQuestView->m_pQUEST->m_dwQuestID != dlg.m_dwParentID )
	{
		CTreeCtrl *pTree = GetTreeCtrl();

		HTREEITEM itemROOT = pTree->GetRootItem();
		HTREEITEM itemCUR = pTree->GetSelectedItem();
		HTREEITEM itemPARENT = dlg.m_dwParentID ? FindNode(
			itemROOT,
			pTree,
			dlg.m_dwParentID) : itemROOT;

		if(itemPARENT)
		{
			HTREEITEM itemNEXT = itemPARENT;
			CTQuestSet set(&m_db);

			while( itemNEXT != itemROOT )
			{
				if( itemNEXT == itemCUR )
					return;

				itemNEXT = pTree->GetParentItem(itemNEXT);
			}

			set.m_strFilter.Format( "dwQuestID = %d", m_pTQuestView->m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_dwParentID = dlg.m_dwParentID;
				set.Update();
				set.Close();

				m_pTQuestView->m_pQUEST->m_dwParentID = dlg.m_dwParentID;
			}

			MoveSubTree( pTree, itemCUR, itemPARENT);
			pTree->DeleteItem(itemCUR);
		}
	}
}

void CTQuestEditorDoc::MoveSubTree( CTreeCtrl *pTree,
								    HTREEITEM itemCUR,
									HTREEITEM itemPARENT)
{
	itemPARENT = pTree->InsertItem( pTree->GetItemText(itemCUR), itemPARENT);

	pTree->SetItemData( itemPARENT, pTree->GetItemData(itemCUR));
	pTree->SetItemData( itemCUR, NULL);
	itemCUR = pTree->GetChildItem(itemCUR);

	while(itemCUR)
	{
		MoveSubTree( pTree, itemCUR, itemPARENT);
		itemCUR = pTree->GetNextSiblingItem(itemCUR);
	}
}

HTREEITEM CTQuestEditorDoc::FindNode( HTREEITEM itemPos,
									  CTreeCtrl *pTree,
									  DWORD dwQuestID)
{
	LPTQUEST pQUEST = (LPTQUEST) pTree->GetItemData(itemPos);

	if( pQUEST && pQUEST->m_dwQuestID == dwQuestID )
		return itemPos;

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		HTREEITEM hRESULT = FindNode( itemPos, pTree, dwQuestID);

		if(hRESULT)
			return hRESULT;

		itemPos = pTree->GetNextSiblingItem(itemPos);
	}

	return NULL;
}

void CTQuestEditorDoc::_Recursive_CollectQuestByType(
	CTreeCtrl* pTree,
	HTREEITEM itemPos,
	BYTE bQuestType_Filter,
	std::vector<LPTQUEST>& vOutput )
{
	LPTQUEST pQUEST = (LPTQUEST) pTree->GetItemData( itemPos );

	if( pQUEST && pQUEST->m_bType == bQuestType_Filter )
		vOutput.push_back( pQUEST );

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		_Recursive_CollectQuestByType(
			pTree,
			itemPos,
			bQuestType_Filter,
			vOutput);

		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CTQuestEditorDoc::OnQuestPrintSummary()
{
	CTreeCtrl *pTree = GetTreeCtrl();
	HTREEITEM itemPos = pTree->GetSelectedItem();

	if( itemPos )
	{
		CFileDialog dlg(
			FALSE,
			_T("*.txt"),
			NULL,
			OFN_HIDEREADONLY,
			_T("Text Files (*.txt)|*.txt||"));

		if( dlg.DoModal() != IDOK )
			return ;

		CString strPATH = dlg.GetPathName();
		CStdioFile vDEST( LPCSTR(strPATH), CFile::modeCreate|CFile::modeWrite|CFile::typeText);

		CString strMSG;
		strMSG.Format("퀘스트아이디\t트리거타입\t트리거아이디\t레벨\t몬스터최고레벨\t잡아야할몬스터수\t카테고리아이디\n");
		vDEST.WriteString( strMSG );

		std::vector<LPTQUEST> vQUEST;

		_Recursive_CollectQuestByType(
			pTree,
			itemPos,
			QT_MISSION,
			vQUEST );

		if( !vQUEST.empty() )
		{
			std::vector<LPTQUEST>::iterator it, end;
			it = vQUEST.begin();
			end = vQUEST.end();

			for(; it != end ; ++it )
			{
				INT nMostMobLevel = 0;
				INT nTotalMobCatch = 0;

				for( INT t=0; t < (INT)(*it)->m_vTerm.size(); ++t )
				{
					LPTQUESTTERM pTERM = (*it)->m_vTerm[ t ];

					if( pTERM->m_bTermType == 3 ) //사냥
					{
						INT nLevel = (INT)GetMonsterLevelByKind(pTERM->m_dwTermID); // m_dwTermID == 몬스터의 wKind
                        
						if( nLevel > nMostMobLevel )
							nMostMobLevel = nLevel;

						nTotalMobCatch += pTERM->m_bCount;
					}
				}

				CString strDATA;
				strDATA.Format( "%u\t%u\t%u\t%u\t%u\t%u\t%u\n",
					(*it)->m_dwQuestID,
					(*it)->m_bTriggerType,
					(*it)->m_dwTriggerID,
					(*it)->m_bLevel,
					nMostMobLevel,
					nTotalMobCatch,
					(*it)->m_dwClassID );

				vDEST.WriteString( strDATA );
			}
		}
		else
		{
			CString strMSG;
			strMSG.LoadString( ID_NOT_COLLECT_QUEST );
			AfxMessageBox( strMSG );
		}

		vDEST.Close();

		strMSG.LoadString( ID_SUCCESS_PRINT_SUMMARY );
		AfxMessageBox( strMSG );
	}
}

BYTE CTQuestEditorDoc::GetMonsterLevelByKind( DWORD dwKind )
{
	CTMonsterChartSet set(&m_db);

	set.m_strFilter.Format( "wKind = %d", (WORD)dwKind);

	if( set.Open() )
	{
		BYTE bLevel = 0;

		if(!set.IsEOF())
			set.MoveFirst();

		while(!set.IsEOF())
		{
			if( bLevel < set.m_bLevel )
				bLevel = set.m_bLevel;
			
			set.MoveNext();
		}

		set.Close();

		return bLevel;
	}
	
	return 0;
}

/*LPCTSTR CTQuestEditorDoc::GetTriggerTypeString( BYTE bTriggerType )
{
	switch( bTriggerType )
	{
	case 0: return "0-없음";
	case 1: return "1-퀘스트 시작";
	case 2: return "2-특정위치도착";
	case 3: return "3-NPC와 대화";
	case 4: return "4-아이템 습득";
	case 5: return "5-몬스터 죽임";
	case 6: return "6-적군 죽임";
	case 7: return "7-스위치 작동";
	case 8: return "8-게이트 작동";
	}
	return "";
}*/