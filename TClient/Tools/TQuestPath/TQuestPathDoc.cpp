// TQuestPathDoc.cpp : CTQuestPathDoc 클래스의 구현
//

#include "stdafx.h"

#include "TImpErrBoundDlg.h"
#include "TQuestPathView.h"
#include "TQuestView.h"
#include "TExportDlg.h"
#include "TImportDlg.h"
#include "TMapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTQuestPathDoc

IMPLEMENT_DYNCREATE(CTQuestPathDoc, CDocument)

BEGIN_MESSAGE_MAP(CTQuestPathDoc, CDocument)
	ON_UPDATE_COMMAND_UI(ID_MAP_EDITMODE, OnUpdateMapEditmode)
	ON_COMMAND(ID_MAP_IMP_ERR_BOUND, OnMapImpErrBound)
	ON_COMMAND(ID_MAP_GOTO_TPOINT, OnMapGotoTpoint)
	ON_COMMAND(ID_MAP_EDITMODE, OnMapEditmode)
	ON_COMMAND(ID_MAP_SCALE, OnMapScale)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	ON_COMMAND(ID_DB_AUTOSET_ALL, OnDbAutosetAll)
	ON_COMMAND(ID_DB_UNLOCK_ALL, OnDbUnlockAll)
	ON_COMMAND(ID_DB_LOCK_ALL, OnDbLockAll)
	ON_COMMAND(ID_DB_LOAD_QUEST, OnDbLoadQuest)
	ON_COMMAND(ID_DB_LOAD_GAME, OnDbLoadGame)
	ON_COMMAND(ID_DB_LOAD_MAP, OnDbLoadMap)
	ON_COMMAND(ID_FILE_BUILD, OnFileBuild)
END_MESSAGE_MAP()


// CTQuestPathDoc 생성/소멸

CTQuestPathDoc::CTQuestPathDoc()
{
	m_vTGAMEDB.ReleaseDATA();
	m_mapTMONITEM.clear();
	m_mapTNPCITEM.clear();
	m_mapTMISSION.clear();
	m_mapTQCLASS.clear();
	m_mapTMAPDB.clear();

	m_bEditPATH = FALSE;
	m_nPathMode = 0;
	m_nBound = 0;
	m_pTQITEM = NULL;
}

CTQuestPathDoc::~CTQuestPathDoc()
{
	ReleaseBUF();
}

BOOL CTQuestPathDoc::OnNewDocument()
{
	ReleaseBUF();

	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

void CTQuestPathDoc::ReleaseBUF()
{
	CTreeCtrl *pTree = GetQuestTreeCtrl();

	if(pTree)
		pTree->DeleteAllItems();

	CTQuestDB::ClearMAPVDWORD(&m_mapTMONITEM);
	CTQuestDB::ClearMAPVDWORD(&m_mapTNPCITEM);
	ReleaseTMAPDB();

	m_vTGAMEDB.ReleaseDATA();
	m_mapTMISSION.clear();
	m_mapTQCLASS.clear();

	UpdateDATA(NULL);
}

void CTQuestPathDoc::ReleaseTMAPDB()
{
	MAPTMAPDATA::iterator itTMAPDATA;

	for( itTMAPDATA = m_mapTMAPDB.begin(); itTMAPDATA != m_mapTMAPDB.end(); itTMAPDATA++)
		delete (*itTMAPDATA).second;

	m_mapTMAPDB.clear();
	UpdateDATA( TPATHWND_MAP, TUPDATE_RESET, NULL);
}


// CTQuestPathDoc serialization

void CTQuestPathDoc::Serialize( CArchive& ar)
{
	if(ar.IsStoring())
	{
		CTreeCtrl *pTree = GetQuestTreeCtrl();

		if(pTree)
		{
			HTREEITEM hITEM = pTree->GetRootItem();

			ar << GetChildCount( TVI_ROOT, *pTree);
			while(hITEM)
			{
				WriteHTREEITEM( hITEM, *pTree, ar);
				hITEM = pTree->GetNextSiblingItem(hITEM);
			}

			WriteMAPVDWORD( &m_mapTMONITEM, ar);
			WriteMAPVDWORD( &m_mapTNPCITEM, ar);

			m_vTGAMEDB.WriteDATA(ar);
			WriteMAPTMAPDATA( &m_mapTMAPDB, ar);
		}
		else
			AfxMessageBox(IDS_ERR_SAVE_TPD);
	}
	else
	{
		CTreeCtrl *pTree = GetQuestTreeCtrl();

		if(pTree)
		{
			DWORD dwCount = 0;

			ar >> dwCount;
			ReleaseBUF();

			for( DWORD i=0; i<dwCount; i++)
				ReadHTREEITEM( TVI_ROOT, *pTree, ar);

			ReadMAPVDWORD( &m_mapTMONITEM, ar);
			ReadMAPVDWORD( &m_mapTNPCITEM, ar);

			m_vTGAMEDB.ReadDATA(ar);
			ReadMAPTMAPDATA( &m_mapTMAPDB, ar);
		}
		else
			AfxMessageBox(IDS_ERR_LOAD_TPD);
	}
}

void CTQuestPathDoc::OnFileExport()
{
	CTExportDlg dlg;

	dlg.m_pTVIEW = GetQuestTreeCtrl();
	dlg.m_pTMAPDATA = &m_mapTMAPDB;
	dlg.DoModal();
}

void CTQuestPathDoc::OnFileImport()
{
	CTImportDlg dlg;

	dlg.m_pDOC = this;
	dlg.DoModal();
}

void CTQuestPathDoc::ClearPathData( LPTMAPIMG pTUNIT)
{
	if(pTUNIT)
	{
		pTUNIT->m_mapTPOINT.clear();
		pTUNIT->m_mapTINDEX.clear();
		pTUNIT->m_mapTLINE.clear();
	}
}

void CTQuestPathDoc::ImportTQUEST()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Quest Term Data Files (*.txt)|*.txt||"));

	if( dlg.DoModal() == IDOK )
	{
		CStdioFile vFILE(
			dlg.GetPathName(),
			CFile::modeRead|
			CFile::typeText);
		CString strDATA;

		while(vFILE.ReadString(strDATA))
		{
			DWORD dwClassID;
			DWORD dwQuestID;

			DWORD dwMonKind;
			DWORD dwNpcID;
			DWORD dwMapID;

			DWORD dwTermType;
			DWORD dwTermID;

			FLOAT fPosX;
			FLOAT fPosY;
			FLOAT fPosZ;

			strDATA.Trim();
			sscanf( LPCSTR(strDATA),
				_T("%d %d %d %d %d %d %d %f %f %f"),
				&dwClassID,
				&dwQuestID,
				&dwTermType,
				&dwTermID,
				&dwMonKind,
				&dwNpcID,
				&dwMapID,
				&fPosX,
				&fPosY,
				&fPosZ);

			LPTQITEM pTQITEM = FindTMISSION(dwQuestID);
			if(pTQITEM)
			{
				int nCount = INT(pTQITEM->m_vTTERM.size());

				for( int i=0; i<nCount; i++)
				{
					LPTTITEM pTTITEM = pTQITEM->m_vTTERM[i];

					if( pTTITEM && pTTITEM->m_vTTERM.m_dwTermID == dwTermID &&
						pTTITEM->m_vTTERM.m_bTermType == dwTermType )
					{
						pTTITEM->m_dwMonKind = dwMonKind;
						pTTITEM->m_dwNpcID = dwNpcID;
						pTTITEM->m_dwMapID = dwMapID;

						pTTITEM->m_fPosX = fPosX;
						pTTITEM->m_fPosY = fPosY;
						pTTITEM->m_fPosZ = fPosZ;

						pTTITEM->m_bLOCK = TRUE;
						break;
					}
				}
			}
		}

		AfxMessageBox(IDS_MSG_IMPORT);
		UpdateDATA(m_pTQITEM);
	}
}

void CTQuestPathDoc::ImportTPATH()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Map Path Data Files (*.txt)|*.txt||"));

	if( dlg.DoModal() == IDOK )
	{
		BYTE bCLEAR = FALSE;

		if( AfxMessageBox( IDS_MSG_CLEAR_PATH, MB_YESNO) == IDYES )
			bCLEAR = TRUE;

		CStdioFile vFILE(
			dlg.GetPathName(),
			CFile::modeRead|
			CFile::typeText);
		CString strDATA;

		MAPTMAPDATA::iterator itTMAP;
		MAPTMAPIMG::iterator itTUNIT;

		MAPTMAPDATA mapTMAPDATA;
		mapTMAPDATA.clear();

		while(vFILE.ReadString(strDATA))
		{
			DWORD dwDATA[3];
			DWORD dwMapID;
			DWORD dwUnitX;
			DWORD dwUnitZ;
			DWORD dwType;

			strDATA.Trim();
			sscanf( LPCSTR(strDATA),
				_T("%d %d %d %d %d %d %d"),
				&dwType,
				&dwMapID,
				&dwUnitX,
				&dwUnitZ,
				&dwDATA[0],
				&dwDATA[1],
				&dwDATA[2]);

			LPTMAPDATA pTMAP = NULL;
			LPTMAPIMG pTUNIT = NULL;

			itTMAP = mapTMAPDATA.find(dwMapID);
			if( itTMAP == mapTMAPDATA.end() )
			{
				pTMAP = new TMAPDATA();
				pTMAP->m_dwID = dwMapID;

				mapTMAPDATA.insert( MAPTMAPDATA::value_type( dwMapID, pTMAP));
			}
			else
				pTMAP = (*itTMAP).second;

			itTUNIT = pTMAP->m_mapTUNIT.find(MAKELONG( WORD(dwUnitX), WORD(dwUnitZ)));
			if( itTUNIT == pTMAP->m_mapTUNIT.end() )
			{
				pTUNIT = new TMAPIMG();
				pTUNIT->m_vID.m_wUnitX = (WORD) dwUnitX;
				pTUNIT->m_vID.m_wUnitZ = (WORD) dwUnitZ;

				pTMAP->m_mapTUNIT.insert( MAPTMAPIMG::value_type( pTUNIT->m_vID.m_dwID, pTUNIT));
			}
			else
				pTUNIT = (*itTUNIT).second;

			switch(dwType)
			{
			case TMAPEXPORT_POINT	: pTUNIT->m_mapTPOINT.insert( MAPT2DPOINT::value_type( WORD(dwDATA[0]), MAKET2DPOINTL( dwDATA[1], dwDATA[2]))); break;
			case TMAPEXPORT_LINE	: pTUNIT->m_mapTLINE.insert( MAPDWORD::value_type( MAKELONG( WORD(dwDATA[0]), WORD(dwDATA[1])), dwDATA[2])); break;
			}
		}

		for( itTMAP = mapTMAPDATA.begin(); itTMAP != mapTMAPDATA.end(); itTMAP++)
		{
			LPTMAPDATA pTMAP = (*itTMAP).second;

			for( itTUNIT = pTMAP->m_mapTUNIT.begin(); itTUNIT != pTMAP->m_mapTUNIT.end(); itTUNIT++)
			{
				LPTMAPIMG pTUNIT = FindTMAPIMG(
					FindTMAPDATA(pTMAP->m_dwID),
					(*itTUNIT).second->m_vID.m_wUnitX,
					(*itTUNIT).second->m_vID.m_wUnitZ);

				if(bCLEAR)
					ClearPathData(pTUNIT);

				MergePathData( (*itTUNIT).second, pTUNIT);
			}

			delete (*itTMAP).second;
		}
		mapTMAPDATA.clear();

		if(m_bEditPATH)
			RedrawTMAP();

		AfxMessageBox(IDS_MSG_IMPORT);
	}
}

void CTQuestPathDoc::MergePathData( LPTMAPIMG pFROM,
								    LPTMAPIMG pTO)
{
	if( !pFROM || !pTO )
		return;

	MAPT2DPOINT::iterator itTPOINT;
	MAPDWORD::iterator itTLINE;

	MAPDWORD mapID;
	mapID.clear();

	for( itTPOINT = pFROM->m_mapTPOINT.begin(); itTPOINT != pFROM->m_mapTPOINT.end(); itTPOINT++)
	{
		WORD wPointID = FindSimilarPoint( pTO, (*itTPOINT).second, m_nBound);

		if(!wPointID)
		{
			while( !wPointID || pTO->m_mapTPOINT.find(wPointID) != pTO->m_mapTPOINT.end() )
				wPointID++;

			pTO->m_mapTPOINT.insert( MAPT2DPOINT::value_type( wPointID, (*itTPOINT).second));
			pTO->m_mapTINDEX.insert( MAPT2DINDEX::value_type( (*itTPOINT).second, wPointID));
		}

		if(wPointID)
			mapID.insert( MAPDWORD::value_type( (*itTPOINT).first, wPointID));
	}

	for( itTLINE = pFROM->m_mapTLINE.begin(); itTLINE != pFROM->m_mapTLINE.end(); itTLINE++)
	{
		WORD vTLINE[2] = {
			LOWORD((*itTLINE).first),
			HIWORD((*itTLINE).first)};

		for( int i=0; i<2; i++)
		{
			MAPDWORD::iterator finder = mapID.find(vTLINE[i]);

			if( finder != mapID.end() )
				vTLINE[i] = (WORD) (*finder).second;
			else
				vTLINE[i] = 0;
		}

		if( vTLINE[0] && vTLINE[1] &&
			pTO->m_mapTLINE.find(MAKELONG( vTLINE[0], vTLINE[1])) == pTO->m_mapTLINE.end() &&
			pTO->m_mapTLINE.find(MAKELONG( vTLINE[1], vTLINE[0])) == pTO->m_mapTLINE.end() )
			pTO->m_mapTLINE.insert( MAPDWORD::value_type( MAKELONG( vTLINE[0], vTLINE[1]), (*itTLINE).second));
	}

	mapID.clear();
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

CTQuestPathView *CTQuestPathDoc::GetTQuestPathView()
{
	POSITION pos = GetFirstViewPosition();

	while(pos)
	{
		CView *pView = GetNextView(pos);

		if( pView && pView->IsKindOf(RUNTIME_CLASS(CTQuestPathView)) )
			return (CTQuestPathView *) pView;
	}

	return NULL;
}

CTMapView *CTQuestPathDoc::GetTMapView()
{
	POSITION pos = GetFirstViewPosition();

	while(pos)
	{
		CView *pView = GetNextView(pos);

		if( pView && pView->IsKindOf(RUNTIME_CLASS(CTMapView)) )
			return (CTMapView *) pView;
	}

	return NULL;
}

CString CTQuestPathDoc::ReadTMAPLIST( CStdioFile *pFILE)
{
	CString strRESULT;

	do
	{
		if(!pFILE->ReadString(strRESULT))
		{
			strRESULT.Empty();
			return strRESULT;
		}

		strRESULT.TrimRight();
		strRESULT.TrimLeft();
	} while( strRESULT.IsEmpty() || strRESULT.GetAt(0) == ';' );

	return strRESULT;
}

BYTE CTQuestPathDoc::LoadTMAPDB( CString strFILE)
{
	TRY
	{
		CStdioFile vLIST( LPCSTR(strFILE), CFile::modeRead|CFile::typeText);

		CString strDATA;
		DWORD dwCount;

		strDATA = ReadTMAPLIST(&vLIST);
		sscanf( LPCSTR(strDATA), _T("%d"), &dwCount);

		for( DWORD i=0; i<dwCount; i++)
		{
			DWORD dwUnitCount = 0;
			DWORD dwMapID = 0;
			DWORD dwUnitX = 0;
			DWORD dwUnitZ = 0;

			strDATA = ReadTMAPLIST(&vLIST);
			sscanf( LPCSTR(strDATA),
				_T("%d %d %d %d"),
				&dwMapID,
				&dwUnitCount,
				&dwUnitX,
				&dwUnitZ);

			LPTMAPDATA pTMAPDATA = FindTMAPDATA(dwMapID);
			if(!pTMAPDATA)
			{
				pTMAPDATA = new TMAPDATA();
				pTMAPDATA->m_dwID = dwMapID;

				m_mapTMAPDB.insert( MAPTMAPDATA::value_type(
					pTMAPDATA->m_dwID,
					pTMAPDATA));
			}

			pTMAPDATA->m_dwUnitX = dwUnitX;
			pTMAPDATA->m_dwUnitZ = dwUnitZ;

			for( DWORD j=0; j<dwUnitCount; j++)
			{
				CString strFILE = ReadTMAPLIST(&vLIST);
				CFile vTIMG( strFILE, CFile::modeRead);
				BITMAPFILEHEADER vTHEADER;

				strDATA = ReadTMAPLIST(&vLIST);
				sscanf( LPCSTR(strDATA),
					_T("%d %d"),
					&dwUnitX,
					&dwUnitZ);

				LPTMAPIMG pTUNIT = FindTMAPIMG(
					pTMAPDATA,
					(WORD) dwUnitX,
					(WORD) dwUnitZ);

				if(!pTUNIT)
				{
					pTUNIT = new TMAPIMG();

					pTUNIT->m_vID.m_wUnitX = (WORD) dwUnitX;
					pTUNIT->m_vID.m_wUnitZ = (WORD) dwUnitZ;

					pTMAPDATA->m_mapTUNIT.insert( MAPTMAPIMG::value_type(
						pTUNIT->m_vID.m_dwID,
						pTUNIT));
				}

				if(pTUNIT->m_vTBMPINFO.m_pTBUF)
				{
					delete[] pTUNIT->m_vTBMPINFO.m_pTBUF;
					pTUNIT->m_vTBMPINFO.m_pTBUF = NULL;
				}

				if(pTUNIT->m_vTBMPBITS.m_pTBUF)
				{
					delete[] pTUNIT->m_vTBMPBITS.m_pTBUF;
					pTUNIT->m_vTBMPBITS.m_pTBUF = NULL;
				}

				pTUNIT->m_vTBMPINFO.m_dwSIZE = 0;
				pTUNIT->m_vTBMPBITS.m_dwSIZE = 0;

				vTIMG.Read( &vTHEADER, sizeof(BITMAPFILEHEADER));
				int nSIZE = vTHEADER.bfOffBits - sizeof(BITMAPFILEHEADER);

				if( nSIZE > 0 )
				{
					pTUNIT->m_vTBMPINFO.m_pTBUF = new BYTE[nSIZE];
					pTUNIT->m_vTBMPINFO.m_dwSIZE = nSIZE;
					vTIMG.Read( pTUNIT->m_vTBMPINFO.m_pTBUF, nSIZE);

					nSIZE = (int) vTIMG.GetLength() - vTHEADER.bfOffBits;
					if( nSIZE > 0 )
					{
						pTUNIT->m_vTBMPBITS.m_pTBUF = new BYTE[nSIZE];
						pTUNIT->m_vTBMPBITS.m_dwSIZE = nSIZE;
						vTIMG.Read( pTUNIT->m_vTBMPBITS.m_pTBUF, nSIZE);
					}
				}
			}
		}

		ResetMode();
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL

	return TRUE;
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
					pTQCLASS->m_itemPOS = pTree->InsertItem( strTEXT, 0, 1);

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
						0, 1,
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
						0, 1,
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

	vTQUEST.MergeVDWORD(
		&vTQUEST.m_mapTMONITEMDATA,
		&m_mapTMONITEM);

	vTQUEST.MergeVDWORD(
		&vTQUEST.m_mapTNPCITEMDATA,
		&m_mapTNPCITEM);

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

	for( int i=0; i<INT(pTQITEM->m_pMission->m_vTTERM.size()); i++)
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

LPTMAPDATA CTQuestPathDoc::FindTMAPDATA( DWORD dwMapID)
{
	MAPTMAPDATA::iterator finder = m_mapTMAPDB.find(dwMapID);

	if( finder != m_mapTMAPDB.end() )
		return (*finder).second;

	return NULL;
}

LPTMAPIMG CTQuestPathDoc::FindTMAPIMG( DWORD dwMapID,
									   FLOAT fPosX,
									   FLOAT fPosZ)
{
	LPTMAPDATA pTMAP = FindTMAPDATA(dwMapID);

	if(pTMAP)
	{
		WORD wUnitX = WORD(fPosX / pTMAP->m_dwUnitX);
		WORD wUnitZ = WORD(fPosZ / pTMAP->m_dwUnitZ);

		return FindTMAPIMG(
			pTMAP,
			wUnitX,
			wUnitZ);
	}

	return NULL;
}

LPTMAPIMG CTQuestPathDoc::FindTMAPIMG( LPTMAPDATA pTMAP,
									   WORD wUnitX,
									   WORD wUnitZ)
{
	if(pTMAP)
	{
		MAPTMAPIMG::iterator finder = pTMAP->m_mapTUNIT.find(MAKELONG( wUnitX, wUnitZ));

		if( finder != pTMAP->m_mapTUNIT.end() )
			return (*finder).second;
	}

	return NULL;
}

WORD CTQuestPathDoc::FindSimilarPoint( LPTMAPIMG pTUNIT,
									   T2DPOINT lPoint,
									   int nBound)
{
	if(!pTUNIT)
		return 0;

	DWORD dwPosX = GET2DL_X(lPoint);
	DWORD dwPosY = GET2DL_Y(lPoint);

	int nCountX = nBound;
	int nCountY = nBound;

	if( dwPosX >= DWORD(nBound) )
	{
		dwPosX -= nBound;
		nCountX *= 2;
	}

	if( dwPosY >= DWORD(nBound) )
	{
		dwPosY -= nBound;
		nCountY *= 2;
	}

	nCountX++;
	nCountY++;

	for( int i=0; i<nCountX; i++)
	{
		DWORD dwLocalX = dwPosX + i;

		for( int j=0; j<nCountY; j++)
		{
			DWORD dwLocalY = dwPosY + j;
			MAPT2DINDEX::iterator finder = pTUNIT->m_mapTINDEX.find(MAKET2DPOINTL( dwLocalX, dwLocalY));

			if( finder != pTUNIT->m_mapTINDEX.end() )
				return (*finder).second;
		}
	}

	return 0;
}

BYTE CTQuestPathDoc::CalcTMONKIND( LPTMDATA pTMDATA,
								   DWORD dwMonKind)
{
	LPMAPTMDATA pMAPTMDATA = m_vTGAMEDB.FindMAPTMDATA(
		&m_vTGAMEDB.m_mapTKINDINDEX,
		dwMonKind);

	if(pMAPTMDATA)
	{
		MAPTMDATA::iterator itTMDATA;
		LPVTMDATA pVTMDATA = NULL;

		DWORD dwCount = 0;
		DWORD dwMapID = 0;

		for( itTMDATA = pMAPTMDATA->begin(); itTMDATA != pMAPTMDATA->end(); itTMDATA++)
		{
			DWORD dwLocal = DWORD((*itTMDATA).second->size());

			if( dwCount < dwLocal )
			{
				pVTMDATA = (*itTMDATA).second;
				dwMapID = (*itTMDATA).first;
				dwCount = dwLocal;
			}
		}

		if(pVTMDATA)
		{
			pTMDATA->m_wKind = (WORD) dwMonKind;
			pTMDATA->m_wMapID = (WORD) dwMapID;

			pTMDATA->m_fPosX = 0.0f;
			pTMDATA->m_fPosY = 0.0f;
			pTMDATA->m_fPosZ = 0.0f;

			for( DWORD i=0; i<dwCount; i++)
			{
				pTMDATA->m_fPosX += (*pVTMDATA)[i]->m_fPosX;
				pTMDATA->m_fPosY += (*pVTMDATA)[i]->m_fPosY;
				pTMDATA->m_fPosZ += (*pVTMDATA)[i]->m_fPosZ;
			}

			pTMDATA->m_fPosX /= FLOAT(dwCount);
			pTMDATA->m_fPosY /= FLOAT(dwCount);
			pTMDATA->m_fPosZ /= FLOAT(dwCount);

			return TRUE;
		}
	}

	return FALSE;
}

void CTQuestPathDoc::UpdateTMONName( CListCtrl *pLIST,
									 DWORD dwKind,
									 DWORD dwMapID)
{
	LPMAPTMDATA pMAPTMDATA = m_vTGAMEDB.FindMAPTMDATA(
		&m_vTGAMEDB.m_mapTKINDINDEX,
		dwKind);

	if(pMAPTMDATA)
	{
		LPVTMDATA pVTMDATA = m_vTGAMEDB.FindVTMDATA(
			pMAPTMDATA,
			dwMapID);

		if( pVTMDATA && !pVTMDATA->empty() )
		{
			MAPDWORD mapID;
			mapID.clear();

			for( int i=0; i<INT(pVTMDATA->size()); i++)
				if( mapID.find((*pVTMDATA)[i]->m_wMonID) == mapID.end() )
				{
					CString strID;

					strID.Format( _T("%d"), (*pVTMDATA)[i]->m_wMonID);
					int nIndex = pLIST->InsertItem(
						pLIST->GetItemCount(),
						strID);

					pLIST->SetItemText(
						nIndex, 1,
						GetTFMTName(
						(*pVTMDATA)[i]->m_strTITLE,
						(*pVTMDATA)[i]->m_strNAME));

					mapID.insert( MAPDWORD::value_type( (*pVTMDATA)[i]->m_wMonID, 0));
				}

			mapID.clear();
		}
	}
}

CString CTQuestPathDoc::GetTNPCName( WORD wID)
{
	CString strRESULT;

	if(wID)
	{
		LPTNDATA pTNDATA = m_vTGAMEDB.FindTNDATA(wID);

		if(pTNDATA)
			strRESULT = GetTFMTName( pTNDATA->m_strTITLE, pTNDATA->m_strNAME);
		else
			strRESULT.LoadString(IDS_NPC_NAME_NOT_FOUND);
	}
	else
		strRESULT.LoadString(IDS_NPC_NAME_NONE);

	return strRESULT;
}

CString CTQuestPathDoc::GetTFMTName( CString strTITLE,
									 CString strNAME)
{
	CString strRESULT;

	strTITLE.Replace( '_', ' ');
	strNAME.Replace( '_', ' ');

	strTITLE.Trim();
	strNAME.Trim();

	if(!strTITLE.IsEmpty())
		strRESULT.Format( IDS_FMT_NPC_NAME, strTITLE, strNAME);
	else
		strRESULT = strNAME;

	return strRESULT;
}

void CTQuestPathDoc::EnableTPATHWND( int nIndex)
{
	CTQuestPathView *pView = GetTQuestPathView();
	CTreeCtrl *pTree = GetQuestTreeCtrl();

	if(pTree)
		pTree->EnableWindow(nIndex == TPATHWND_MAIN ? TRUE : FALSE);

	if(pView)
		pView->EnableTPATHWND(nIndex);
}

void CTQuestPathDoc::UpdateDATA( int nIndex,
								 int nType,
								 LPARAM lParam)
{
	CTQuestPathView *pView = GetTQuestPathView();

	if(pView)
		pView->UpdateDATA( nIndex, nType, lParam);
}

void CTQuestPathDoc::UpdateDATA( LPTQITEM pTQITEM)
{
	m_pTQITEM = pTQITEM;
	UpdateTMAP(NULL);
	UpdateDATA( TPATHWND_MAIN, TUPDATE_RESET, 0);
}

void CTQuestPathDoc::UpdateTMAP( LPTTITEM pTTITEM)
{
	CTMapView *pView = GetTMapView();

	if(pView)
		pView->UpdateDATA(pTTITEM);
}

void CTQuestPathDoc::RedrawTMAP()
{
	CTMapView *pView = GetTMapView();

	if(pView)
		pView->Invalidate(FALSE);
}

void CTQuestPathDoc::OnMapGotoTpoint()
{
	CTMapView *pView = GetTMapView();

	if(pView)
		pView->GoToTPOINT();
}

void CTQuestPathDoc::OnMapScale()
{
	CTMapView *pView = GetTMapView();

	if(pView)
		pView->OnMapScale();
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

void CTQuestPathDoc::OnDbLoadMap()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Map List Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		if( AfxMessageBox( IDS_MSG_CLEAR_MAP, MB_YESNO) == IDYES )
			ReleaseTMAPDB();

		LoadTMAPDB(dlg.GetPathName());
	}
}

BYTE CTQuestPathDoc::GetAutosetData( LPTTITEM pTRESULT,
									 LPTQITEM pTQITEM,
									 LPTTITEM pTTITEM)
{
	if( !pTRESULT || !pTQITEM || !pTTITEM )
		return FALSE;

	switch(pTTITEM->m_vTTERM.m_bTermType)
	{
	case QTT_TSTART_POS		:
		{
			if( !pTQITEM->m_pMission || pTQITEM->m_pMission->m_bTriggerType != TT_TALKNPC )
				return FALSE;

			LPTNDATA pTNDATA = m_vTGAMEDB.FindTNDATA(pTQITEM->m_pMission->m_dwTriggerID);
			if(!pTNDATA)
				return FALSE;

			pTRESULT->m_dwMapID = pTNDATA->m_wMapID;
			pTRESULT->m_dwNpcID = pTNDATA->m_wID;
			pTRESULT->m_dwMonKind = 0;

			pTRESULT->m_fPosX = pTNDATA->m_fPosX;
			pTRESULT->m_fPosY = pTNDATA->m_fPosY;
			pTRESULT->m_fPosZ = pTNDATA->m_fPosZ;
		}

		break;

	case QTT_QUESTCOMPLETED	:
		{
			pTQITEM = FindTMISSION(pTTITEM->m_vTTERM.m_dwTermID);

			if( !pTQITEM || !pTQITEM->m_pMission || pTQITEM->m_pMission->m_bTriggerType != TT_TALKNPC )
				return FALSE;

			LPTNDATA pTNDATA = m_vTGAMEDB.FindTNDATA(pTQITEM->m_pMission->m_dwTriggerID);
			if(!pTNDATA)
				return FALSE;

			pTRESULT->m_dwMapID = pTNDATA->m_wMapID;
			pTRESULT->m_dwNpcID = pTNDATA->m_wID;
			pTRESULT->m_dwMonKind = 0;

			pTRESULT->m_fPosX = pTNDATA->m_fPosX;
			pTRESULT->m_fPosY = pTNDATA->m_fPosY;
			pTRESULT->m_fPosZ = pTNDATA->m_fPosZ;
		}

		break;

	case QTT_GETITEM		:
		{
			LPVDWORD pVDWORD = CTQuestDB::FindVDWORD(
				&m_mapTMONITEM,
				pTTITEM->m_vTTERM.m_dwTermID);

			if(!pVDWORD)
			{
				pVDWORD = CTQuestDB::FindVDWORD(
					&m_mapTNPCITEM,
					pTTITEM->m_vTTERM.m_dwTermID);

				if( !pVDWORD || pVDWORD->empty() )
					return FALSE;

				LPTNDATA pTNDATA = m_vTGAMEDB.FindTNDATA((*pVDWORD)[0]);
				if(!pTNDATA)
					return FALSE;

				pTRESULT->m_dwMapID = pTNDATA->m_wMapID;
				pTRESULT->m_dwNpcID = pTNDATA->m_wID;
				pTRESULT->m_dwMonKind = 0;

				pTRESULT->m_fPosX = pTNDATA->m_fPosX;
				pTRESULT->m_fPosY = pTNDATA->m_fPosY;
				pTRESULT->m_fPosZ = pTNDATA->m_fPosZ;
			}
			else if(!pVDWORD->empty())
			{
				TMDATA vTMDATA;

				if(!CalcTMONKIND( &vTMDATA, (*pVDWORD)[0]))
					return FALSE;

				pTRESULT->m_dwMonKind = vTMDATA.m_wKind;
				pTRESULT->m_dwMapID = vTMDATA.m_wMapID;
				pTRESULT->m_dwNpcID = 0;

				pTRESULT->m_fPosX = vTMDATA.m_fPosX;
				pTRESULT->m_fPosY = vTMDATA.m_fPosY;
				pTRESULT->m_fPosZ = vTMDATA.m_fPosZ;
			}
			else
				return FALSE;
		}

		break;

	case QTT_HUNT			:
		{
			TMDATA vTMDATA;

			if(!CalcTMONKIND( &vTMDATA, pTTITEM->m_vTTERM.m_dwTermID))
				return FALSE;

			pTRESULT->m_dwMonKind = vTMDATA.m_wKind;
			pTRESULT->m_dwMapID = vTMDATA.m_wMapID;
			pTRESULT->m_dwNpcID = 0;

			pTRESULT->m_fPosX = vTMDATA.m_fPosX;
			pTRESULT->m_fPosY = vTMDATA.m_fPosY;
			pTRESULT->m_fPosZ = vTMDATA.m_fPosZ;
		}

		break;

	case QTT_TALK			:
		{
			LPTNDATA pTNDATA = m_vTGAMEDB.FindTNDATA(pTTITEM->m_vTTERM.m_dwTermID);
			if(!pTNDATA)
				return FALSE;

			pTRESULT->m_dwMapID = pTNDATA->m_wMapID;
			pTRESULT->m_dwNpcID = pTNDATA->m_wID;
			pTRESULT->m_dwMonKind = 0;

			pTRESULT->m_fPosX = pTNDATA->m_fPosX;
			pTRESULT->m_fPosY = pTNDATA->m_fPosY;
			pTRESULT->m_fPosZ = pTNDATA->m_fPosZ;
		}

		break;

	case QTT_TCOMP_POS		:
		{
			if( !pTQITEM->m_pComplete || pTQITEM->m_pComplete->m_bTriggerType != TT_TALKNPC )
				return FALSE;

			LPTNDATA pTNDATA = m_vTGAMEDB.FindTNDATA(pTQITEM->m_pComplete->m_dwTriggerID);
			if(!pTNDATA)
				return FALSE;

			pTRESULT->m_dwMapID = pTNDATA->m_wMapID;
			pTRESULT->m_dwNpcID = pTNDATA->m_wID;
			pTRESULT->m_dwMonKind = 0;

			pTRESULT->m_fPosX = pTNDATA->m_fPosX;
			pTRESULT->m_fPosY = pTNDATA->m_fPosY;
			pTRESULT->m_fPosZ = pTNDATA->m_fPosZ;
		}

		break;
	}
	pTRESULT->m_vTTERM.m_strTermMSG = pTTITEM->m_vTTERM.m_strTermMSG;

	return TRUE;
}

DWORD CTQuestPathDoc::GetChildCount( HTREEITEM hITEM,
									 CTreeCtrl& cTree)
{
	DWORD dwCount = 0;

	hITEM = cTree.GetChildItem(hITEM);
	while(hITEM)
	{
		hITEM = cTree.GetNextSiblingItem(hITEM);
		dwCount++;
	}

	return dwCount;
}

void CTQuestPathDoc::WriteHTREEITEM( HTREEITEM hITEM,
									 CTreeCtrl& cTree,
									 CArchive& vAR)
{
	LPTQITEM pTQITEM = (LPTQITEM) cTree.GetItemData(hITEM);
	CString strTITLE = cTree.GetItemText(hITEM);
	DWORD dwCount = DWORD(pTQITEM->m_vTTERM.size());

	vAR << strTITLE
		<< pTQITEM->m_bType
		<< pTQITEM->m_dwID
		<< dwCount;

	for( DWORD i=0; i<dwCount; i++)
	{
		vAR << pTQITEM->m_vTTERM[i]->m_vTTERM.m_strTermMSG
			<< pTQITEM->m_vTTERM[i]->m_vTTERM.m_bTermType
			<< pTQITEM->m_vTTERM[i]->m_vTTERM.m_dwTermID
			<< pTQITEM->m_vTTERM[i]->m_dwMapID
			<< pTQITEM->m_vTTERM[i]->m_dwMonKind
			<< pTQITEM->m_vTTERM[i]->m_dwNpcID
			<< pTQITEM->m_vTTERM[i]->m_fPosX
			<< pTQITEM->m_vTTERM[i]->m_fPosY
			<< pTQITEM->m_vTTERM[i]->m_fPosZ
			<< pTQITEM->m_vTTERM[i]->m_bLOCK;
	}

	if(pTQITEM->m_pMission)
	{
		dwCount = DWORD(pTQITEM->m_pMission->m_vTTERM.size());

		vAR << BYTE(TRUE)
			<< pTQITEM->m_pMission->m_strTITLE
			<< pTQITEM->m_pMission->m_strTriggerMSG
			<< pTQITEM->m_pMission->m_strCompleteMSG
			<< pTQITEM->m_pMission->m_dwClassID
			<< pTQITEM->m_pMission->m_dwQuestID
			<< pTQITEM->m_pMission->m_bType
			<< pTQITEM->m_pMission->m_bTriggerType
			<< pTQITEM->m_pMission->m_dwTriggerID
			<< dwCount;

		for( DWORD i=0; i<dwCount; i++)
		{
			vAR << pTQITEM->m_pMission->m_vTTERM[i]->m_strTermMSG
				<< pTQITEM->m_pMission->m_vTTERM[i]->m_bTermType
				<< pTQITEM->m_pMission->m_vTTERM[i]->m_dwTermID;
		}
	}
	else
		vAR << BYTE(FALSE);

	if(pTQITEM->m_pComplete)
	{
		dwCount = DWORD(pTQITEM->m_pComplete->m_vTTERM.size());

		vAR << BYTE(TRUE)
			<< pTQITEM->m_pComplete->m_strTITLE
			<< pTQITEM->m_pComplete->m_strTriggerMSG
			<< pTQITEM->m_pComplete->m_strCompleteMSG
			<< pTQITEM->m_pComplete->m_dwClassID
			<< pTQITEM->m_pComplete->m_dwQuestID
			<< pTQITEM->m_pComplete->m_bType
			<< pTQITEM->m_pComplete->m_bTriggerType
			<< pTQITEM->m_pComplete->m_dwTriggerID
			<< dwCount;

		for( DWORD i=0; i<dwCount; i++)
		{
			vAR << pTQITEM->m_pComplete->m_vTTERM[i]->m_strTermMSG
				<< pTQITEM->m_pComplete->m_vTTERM[i]->m_bTermType
				<< pTQITEM->m_pComplete->m_vTTERM[i]->m_dwTermID;
		}
	}
	else
		vAR << BYTE(FALSE);

	vAR << GetChildCount( hITEM, cTree);
	hITEM = cTree.GetChildItem(hITEM);

	while(hITEM)
	{
		WriteHTREEITEM( hITEM, cTree, vAR);
		hITEM = cTree.GetNextSiblingItem(hITEM);
	}
}

void CTQuestPathDoc::ReadHTREEITEM( HTREEITEM hITEM,
								    CTreeCtrl& cTree,
									CArchive& vAR)
{
	LPTQITEM pTQITEM = new TQITEM();
	CString strTITLE;

	BYTE bLoad = FALSE;
	DWORD dwCount = 0;

	vAR >> strTITLE
		>> pTQITEM->m_bType
		>> pTQITEM->m_dwID
		>> dwCount;

	for( DWORD i=0; i<dwCount; i++)
	{
		LPTTITEM pTTITEM = new TTITEM();

		vAR >> pTTITEM->m_vTTERM.m_strTermMSG
			>> pTTITEM->m_vTTERM.m_bTermType
			>> pTTITEM->m_vTTERM.m_dwTermID
			>> pTTITEM->m_dwMapID
			>> pTTITEM->m_dwMonKind
			>> pTTITEM->m_dwNpcID
			>> pTTITEM->m_fPosX
			>> pTTITEM->m_fPosY
			>> pTTITEM->m_fPosZ
			>> pTTITEM->m_bLOCK;

		pTQITEM->m_vTTERM.push_back(pTTITEM);
	}

	vAR >> bLoad;
	if(bLoad)
	{
		pTQITEM->m_pMission = new TQDATA();

		vAR >> pTQITEM->m_pMission->m_strTITLE
			>> pTQITEM->m_pMission->m_strTriggerMSG
			>> pTQITEM->m_pMission->m_strCompleteMSG
			>> pTQITEM->m_pMission->m_dwClassID
			>> pTQITEM->m_pMission->m_dwQuestID
			>> pTQITEM->m_pMission->m_bType
			>> pTQITEM->m_pMission->m_bTriggerType
			>> pTQITEM->m_pMission->m_dwTriggerID
			>> dwCount;

		for( DWORD i=0; i<dwCount; i++)
		{
			LPTTDATA pTTDATA = new TTDATA();

			vAR >> pTTDATA->m_strTermMSG
				>> pTTDATA->m_bTermType
				>> pTTDATA->m_dwTermID;

			pTQITEM->m_pMission->m_vTTERM.push_back(pTTDATA);
		}
	}

	vAR >> bLoad;
	if(bLoad)
	{
		pTQITEM->m_pComplete = new TQDATA();

		vAR >> pTQITEM->m_pComplete->m_strTITLE
			>> pTQITEM->m_pComplete->m_strTriggerMSG
			>> pTQITEM->m_pComplete->m_strCompleteMSG
			>> pTQITEM->m_pComplete->m_dwClassID
			>> pTQITEM->m_pComplete->m_dwQuestID
			>> pTQITEM->m_pComplete->m_bType
			>> pTQITEM->m_pComplete->m_bTriggerType
			>> pTQITEM->m_pComplete->m_dwTriggerID
			>> dwCount;

		for( DWORD i=0; i<dwCount; i++)
		{
			LPTTDATA pTTDATA = new TTDATA();

			vAR >> pTTDATA->m_strTermMSG
				>> pTTDATA->m_bTermType
				>> pTTDATA->m_dwTermID;

			pTQITEM->m_pComplete->m_vTTERM.push_back(pTTDATA);
		}
	}

	pTQITEM->m_itemPOS = cTree.InsertItem(
		strTITLE,
		0, 1,
		hITEM);

	cTree.SetItemData(
		pTQITEM->m_itemPOS,
		(DWORD_PTR) pTQITEM);

	switch(pTQITEM->m_bType)
	{
	case TQITEM_MISSION	:
		if( m_mapTMISSION.find(pTQITEM->m_dwID) == m_mapTMISSION.end() )
			m_mapTMISSION.insert( MAPTQITEM::value_type( pTQITEM->m_dwID, pTQITEM));

		break;

	case TQITEM_CLASS	:
		if( m_mapTQCLASS.find(pTQITEM->m_dwID) == m_mapTQCLASS.end() )
			m_mapTQCLASS.insert( MAPTQITEM::value_type( pTQITEM->m_dwID, pTQITEM));

		break;
	}
	vAR >> dwCount;

	for( DWORD i=0; i<dwCount; i++)
		ReadHTREEITEM( pTQITEM->m_itemPOS, cTree, vAR);
}

void CTQuestPathDoc::WriteMAPVDWORD( LPMAPVDWORD pTDATA,
									 CArchive& vAR)
{
	MAPVDWORD::iterator itVDWORD;
	vAR << DWORD(pTDATA->size());

	for( itVDWORD = pTDATA->begin(); itVDWORD != pTDATA->end(); itVDWORD++)
	{
		DWORD dwCount = DWORD((*itVDWORD).second->size());

		vAR << DWORD((*itVDWORD).first)
			<< dwCount;

		for( DWORD i=0; i<dwCount; i++)
			vAR << (*(*itVDWORD).second)[i];
	}
}

void CTQuestPathDoc::ReadMAPVDWORD( LPMAPVDWORD pTDATA,
								    CArchive& vAR)
{
	DWORD dwCount = 0;
	MAPVDWORD vTDATA;

	vAR >> dwCount;
	vTDATA.clear();

	for( DWORD i=0; i<dwCount; i++)
	{
		DWORD dwItemID = 0;
		DWORD dwLength = 0;

		vAR >> dwItemID
			>> dwLength;

		LPVDWORD pVDWORD = CTQuestDB::FindVDWORD(
			&vTDATA,
			dwItemID);

		if(!pVDWORD)
		{
			pVDWORD = new VDWORD();
			vTDATA.insert( MAPVDWORD::value_type( dwItemID, pVDWORD));
		}

		for( DWORD j=0; j<dwLength; j++)
		{
			vAR >> dwItemID;
			pVDWORD->push_back(dwItemID);
		}
	}

	CTQuestDB::MergeVDWORD(
		&vTDATA,
		pTDATA);

	CTQuestDB::ClearMAPVDWORD(&vTDATA);
}

void CTQuestPathDoc::WriteMAPTMAPDATA( LPMAPTMAPDATA pTDATA,
									   CArchive& vAR)
{
	MAPTMAPDATA::iterator itTMAPDATA;
	vAR << DWORD(pTDATA->size());

	for( itTMAPDATA = pTDATA->begin(); itTMAPDATA != pTDATA->end(); itTMAPDATA++)
	{
		LPTMAPDATA pTMAP = (*itTMAPDATA).second;

		if(pTMAP)
		{
			MAPTMAPIMG::iterator itTMAPIMG;

			vAR << pTMAP->m_dwID
				<< pTMAP->m_dwUnitX
				<< pTMAP->m_dwUnitZ
				<< DWORD(pTMAP->m_mapTUNIT.size());

			for( itTMAPIMG = pTMAP->m_mapTUNIT.begin(); itTMAPIMG != pTMAP->m_mapTUNIT.end(); itTMAPIMG++)
			{
				LPTMAPIMG pTUNIT = (*itTMAPIMG).second;

				if(pTUNIT)
				{
					MAPT2DPOINT::iterator itT2DPOINT;
					MAPDWORD::iterator itTLINE;

					vAR << pTUNIT->m_vID.m_dwID
						<< pTUNIT->m_vTBMPINFO.m_dwSIZE;

					if(pTUNIT->m_vTBMPINFO.m_dwSIZE)
						vAR.Write( pTUNIT->m_vTBMPINFO.m_pTBUF, pTUNIT->m_vTBMPINFO.m_dwSIZE);

					vAR << pTUNIT->m_vTBMPBITS.m_dwSIZE;
					if(pTUNIT->m_vTBMPBITS.m_dwSIZE)
						vAR.Write( pTUNIT->m_vTBMPBITS.m_pTBUF, pTUNIT->m_vTBMPBITS.m_dwSIZE);

					vAR << DWORD(pTUNIT->m_mapTPOINT.size());
					for( itT2DPOINT = pTUNIT->m_mapTPOINT.begin(); itT2DPOINT != pTUNIT->m_mapTPOINT.end(); itT2DPOINT++)
					{
						vAR << (*itT2DPOINT).first
							<< (*itT2DPOINT).second;
					}

					vAR << DWORD(pTUNIT->m_mapTLINE.size());
					for( itTLINE = pTUNIT->m_mapTLINE.begin(); itTLINE != pTUNIT->m_mapTLINE.end(); itTLINE++)
					{
						vAR << (*itTLINE).first
							<< (*itTLINE).second;
					}
				}
			}
		}
	}
}

void CTQuestPathDoc::ReadMAPTMAPDATA( LPMAPTMAPDATA pTDATA,
									  CArchive& vAR)
{
	DWORD dwCount = 0;
	vAR >> dwCount;

	for( DWORD i=0; i<dwCount; i++)
	{
		LPTMAPDATA pTMAP = new TMAPDATA();
		DWORD dwUnitCount = 0;

		vAR >> pTMAP->m_dwID
			>> pTMAP->m_dwUnitX
			>> pTMAP->m_dwUnitZ
			>> dwUnitCount;

		for( DWORD j=0; j<dwUnitCount; j++)
		{
			LPTMAPIMG pTUNIT = new TMAPIMG();
			DWORD dwLineCount = 0;

			vAR >> pTUNIT->m_vID.m_dwID
				>> pTUNIT->m_vTBMPINFO.m_dwSIZE;

			if(pTUNIT->m_vTBMPINFO.m_dwSIZE)
			{
				pTUNIT->m_vTBMPINFO.m_pTBUF = new BYTE[pTUNIT->m_vTBMPINFO.m_dwSIZE];
				vAR.Read( pTUNIT->m_vTBMPINFO.m_pTBUF, pTUNIT->m_vTBMPINFO.m_dwSIZE);
			}

			vAR >> pTUNIT->m_vTBMPBITS.m_dwSIZE;
			if(pTUNIT->m_vTBMPBITS.m_dwSIZE)
			{
				pTUNIT->m_vTBMPBITS.m_pTBUF = new BYTE[pTUNIT->m_vTBMPBITS.m_dwSIZE];
				vAR.Read( pTUNIT->m_vTBMPBITS.m_pTBUF, pTUNIT->m_vTBMPBITS.m_dwSIZE);
			}

			vAR >> dwLineCount;
			for( DWORD k=0; k<dwLineCount; k++)
			{
				T2DPOINT lPoint;
				WORD wPointID;

				vAR >> wPointID
					>> lPoint;

				pTUNIT->m_mapTPOINT.insert( MAPT2DPOINT::value_type( wPointID, lPoint));
				pTUNIT->m_mapTINDEX.insert( MAPT2DINDEX::value_type( lPoint, wPointID));
			}

			vAR >> dwLineCount;
			for( DWORD k=0; k<dwLineCount; k++)
			{
				DWORD dwLineID;
				DWORD dwKind;

				vAR >> dwLineID
					>> dwKind;

				pTUNIT->m_mapTLINE.insert( MAPDWORD::value_type( dwLineID, dwKind));
			}

			pTMAP->m_mapTUNIT.insert( MAPTMAPIMG::value_type(
				pTUNIT->m_vID.m_dwID,
				pTUNIT));
		}

		pTDATA->insert( MAPTMAPDATA::value_type(
			pTMAP->m_dwID,
			pTMAP));
	}
}

void CTQuestPathDoc::OnDbLockAll()
{
	MAPTQITEM::iterator itTQITEM;

	for( itTQITEM = m_mapTMISSION.begin(); itTQITEM != m_mapTMISSION.end(); itTQITEM++)
	{
		LPTQITEM pTQITEM = (*itTQITEM).second;
		int nCount = pTQITEM ? INT(pTQITEM->m_vTTERM.size()) : 0;

		for( int i=0; i<nCount; i++)
			pTQITEM->m_vTTERM[i]->m_bLOCK = TRUE;
	}

	AfxMessageBox(IDS_MSG_LOCK);
	UpdateDATA(m_pTQITEM);
}

void CTQuestPathDoc::OnDbUnlockAll()
{
	MAPTQITEM::iterator itTQITEM;

	for( itTQITEM = m_mapTMISSION.begin(); itTQITEM != m_mapTMISSION.end(); itTQITEM++)
	{
		LPTQITEM pTQITEM = (*itTQITEM).second;
		int nCount = pTQITEM ? INT(pTQITEM->m_vTTERM.size()) : 0;

		for( int i=0; i<nCount; i++)
			pTQITEM->m_vTTERM[i]->m_bLOCK = FALSE;
	}

	AfxMessageBox(IDS_MSG_UNLOCK);
	UpdateDATA(m_pTQITEM);
}

void CTQuestPathDoc::OnDbAutosetAll()
{
	MAPTQITEM::iterator itTQITEM;

	for( itTQITEM = m_mapTMISSION.begin(); itTQITEM != m_mapTMISSION.end(); itTQITEM++)
	{
		LPTQITEM pTQITEM = (*itTQITEM).second;
		int nCount = pTQITEM ? INT(pTQITEM->m_vTTERM.size()) : 0;

		for( int i=0; i<nCount; i++)
			if(!pTQITEM->m_vTTERM[i]->m_bLOCK)
				GetAutosetData( pTQITEM->m_vTTERM[i], pTQITEM, pTQITEM->m_vTTERM[i]);
	}

	AfxMessageBox(IDS_MSG_AUTOSET);
	UpdateDATA(m_pTQITEM);
}

void CTQuestPathDoc::OnMapEditmode()
{
	m_bEditPATH = !m_bEditPATH;
	ResetMode();
}

void CTQuestPathDoc::ResetMode()
{
	if(m_bEditPATH)
	{
		CTMapView *pTMAP = GetTMapView();
		TTITEM vTPOS;

		if(pTMAP)
		{
			vTPOS.m_fPosX = pTMAP->m_fPosX + FLOAT(pTMAP->m_wUnitX) * pTMAP->m_fSizeX;
			vTPOS.m_fPosZ = pTMAP->m_fPosZ + FLOAT(pTMAP->m_wUnitZ) * pTMAP->m_fSizeZ;
			vTPOS.m_dwMapID = pTMAP->m_dwMapID;
		}

		UpdateDATA( TPATHWND_MAP, TUPDATE_RESET, LPARAM(pTMAP && pTMAP->m_bDRAW ? &vTPOS : NULL));
	}
	else
		m_nPathMode = TEDITPATH_TEST_PATH;

	EnableTPATHWND(m_bEditPATH ? TPATHWND_MAP : TPATHWND_MAIN);
}

void CTQuestPathDoc::OnUpdateMapEditmode( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bEditPATH);
}

void CTQuestPathDoc::MovePathPoint( LPTMAPIMG pTUNIT,
								    WORD wCurrentID,
									FLOAT fPosX,
									FLOAT fPosZ)
{
	MAPT2DPOINT::iterator finder = pTUNIT->m_mapTPOINT.find(wCurrentID);

	if( finder != pTUNIT->m_mapTPOINT.end() )
	{
		MAPT2DINDEX::iterator itT2DINDEX = pTUNIT->m_mapTINDEX.find((*finder).second);

		if( itT2DINDEX != pTUNIT->m_mapTINDEX.end() )
			pTUNIT->m_mapTINDEX.erase(itT2DINDEX);

		T2DPOINT lPoint = MAKET2DPOINT(
			fPosX,
			fPosZ);

		itT2DINDEX = pTUNIT->m_mapTINDEX.find(lPoint);
		if( itT2DINDEX != pTUNIT->m_mapTINDEX.end() )
			DeletePathPoint( pTUNIT, WORD((*itT2DINDEX).second), wCurrentID);

		pTUNIT->m_mapTINDEX.insert( MAPT2DINDEX::value_type( lPoint, wCurrentID));
		(*finder).second = lPoint;
	}
}

void CTQuestPathDoc::DeletePathPoint( LPTMAPIMG pTUNIT,
									  WORD wPointID,
									  WORD wReplaceID)
{
	MAPT2DPOINT::iterator finder = pTUNIT->m_mapTPOINT.find(wPointID);

	if( finder != pTUNIT->m_mapTPOINT.end() )
	{
		MAPT2DINDEX::iterator itT2DINDEX = pTUNIT->m_mapTINDEX.find((*finder).second);
		MAPDWORD::iterator itTLINE = pTUNIT->m_mapTLINE.begin();

		MAPDWORD mapTLINE;
		mapTLINE.clear();

		if( itT2DINDEX != pTUNIT->m_mapTINDEX.end() )
			pTUNIT->m_mapTINDEX.erase(itT2DINDEX);
		pTUNIT->m_mapTPOINT.erase(finder);

		while(itTLINE != pTUNIT->m_mapTLINE.end())
		{
			MAPDWORD::iterator itTNEXT = itTLINE;
			itTNEXT++;

			WORD wLineID[2] = {
				LOWORD((*itTLINE).first),
				HIWORD((*itTLINE).first)};

			if( wLineID[0] == wPointID ||
				wLineID[1] == wPointID )
			{
				if(wReplaceID)
				{
					if( wLineID[0] == wPointID )
						wLineID[0] = wReplaceID;
					else
						wLineID[1] = wReplaceID;

					if( wLineID[0] != wLineID[1] )
					{
						DWORD dwLineID = MAKELONG( wLineID[0], wLineID[1]);

						if( mapTLINE.find(dwLineID) == mapTLINE.end() )
							mapTLINE.insert( MAPDWORD::value_type( dwLineID, (*itTLINE).second));
					}
				}

				pTUNIT->m_mapTLINE.erase(itTLINE);
			}

			itTLINE = itTNEXT;
		}

		for( itTLINE = mapTLINE.begin(); itTLINE != mapTLINE.end(); itTLINE++)
			if( pTUNIT->m_mapTLINE.find(MAKELONG( LOWORD((*itTLINE).first), HIWORD((*itTLINE).first))) == pTUNIT->m_mapTLINE.end() &&
				pTUNIT->m_mapTLINE.find(MAKELONG( HIWORD((*itTLINE).first), LOWORD((*itTLINE).first))) == pTUNIT->m_mapTLINE.end() )
				pTUNIT->m_mapTLINE.insert( MAPDWORD::value_type( (*itTLINE).first, (*itTLINE).second));

		mapTLINE.clear();
	}
}

void CTQuestPathDoc::OnFileBuild()
{
	CFileDialog dlg(
		FALSE,
		_T("*.qpd"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Quest Path Data Files (*.qpd)|*.qpd||"));

	if( dlg.DoModal() == IDOK )
	{
		CFile vFILE(
			dlg.GetPathName(),
			CFile::modeCreate|
			CFile::modeWrite|
			CFile::typeBinary);

		CArchive vAR(
			&vFILE,
			CArchive::store);

		CProgressDlg vTPROGRESS;
		TBUILDINFO vTPARAM;

		vTPROGRESS.m_strCancel.LoadString(IDS_MSG_BUILD_CANCEL);
		vTPROGRESS.m_strTitle.LoadString(IDS_TITLE_BUILD);
		vTPROGRESS.m_pThreadFunc = (AFX_THREADPROC) _Build;
		vTPROGRESS.m_lParam = &vTPARAM;

		vTPARAM.m_pWnd = &vTPROGRESS;
		vTPARAM.m_pTAR = &vAR;
		vTPARAM.m_pDOC = this;

		vTPROGRESS.DoModal();
	}
}

void CTQuestPathDoc::OnMapImpErrBound()
{
	CTImpErrBoundDlg dlg;

	dlg.m_dwBound = m_nBound;
	if( dlg.DoModal() == IDOK )
		m_nBound = max( 0, INT(dlg.m_dwBound));
}

void CTQuestPathDoc::_Build( LPTBUILDINFO lParam)
{
	lParam->m_pDOC->Build(lParam);
}

void CTQuestPathDoc::Build( LPTBUILDINFO lParam)
{
	lParam->m_pWnd->m_strStatus.LoadString(IDS_MSG_BUILD_TQUEST);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	BuildTQUEST(lParam);
	BuildTMAP(lParam);

	lParam->m_pWnd->m_cProgressBar.SetPos(100);
	lParam->m_pWnd->m_bPercent = 100;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CTQuestPathDoc::BuildTQUEST( LPTBUILDINFO lParam)
{
	MAPTQITEM::iterator itTQUEST;

	DWORD dwTMISSION = DWORD(m_mapTMISSION.size());
	DWORD dwTPATH = GetAllPathCount();
	DWORD dwTotal = dwTMISSION + dwTPATH;
	DWORD dwTCUR = 0;

	*(lParam->m_pTAR)
		<< dwTMISSION;

	for( itTQUEST = m_mapTMISSION.begin(); itTQUEST != m_mapTMISSION.end(); itTQUEST++)
	{
		LPTQITEM pTQITEM = (*itTQUEST).second;

		if(pTQITEM)
		{
			DWORD dwCount = DWORD(pTQITEM->m_vTTERM.size());

			*(lParam->m_pTAR)
				<< pTQITEM->m_dwID
				<< dwCount;

			for( DWORD i=0; i<dwCount; i++)
			{
				*(lParam->m_pTAR)
					<< pTQITEM->m_vTTERM[i]->m_vTTERM.m_bTermType
					<< pTQITEM->m_vTTERM[i]->m_vTTERM.m_dwTermID
					<< pTQITEM->m_vTTERM[i]->m_dwMonKind
					<< pTQITEM->m_vTTERM[i]->m_dwNpcID
					<< pTQITEM->m_vTTERM[i]->m_dwMapID
					<< pTQITEM->m_vTTERM[i]->m_fPosX
					<< pTQITEM->m_vTTERM[i]->m_fPosY
					<< pTQITEM->m_vTTERM[i]->m_fPosZ;
			}
		}
		else
		{
			*(lParam->m_pTAR)
				<< DWORD(0)
				<< DWORD(0);
		}

		dwTCUR++;
		lParam->m_pWnd->m_bPercent = BYTE(dwTCUR * 100 / dwTotal);

		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}
}

void CTQuestPathDoc::BuildTMAP( LPTBUILDINFO lParam)
{
	MAPTMAPDATA::iterator itTMAP;

	DWORD dwTMISSION = DWORD(m_mapTMISSION.size());
	DWORD dwTPATH = GetAllPathCount();
	DWORD dwTotal = dwTMISSION + dwTPATH;
	DWORD dwTCUR = dwTMISSION;

	CFile *pTFILE = lParam->m_pTAR->GetFile();
	CString strTPATH = pTFILE->GetFilePath();
	CString strTNAME = pTFILE->GetFileName();

	strTPATH = strTPATH.Left(strTPATH.GetLength() - strTNAME.GetLength());
	strTPATH.TrimRight('\\');
	strTPATH = strTPATH + CString(_T("\\Node"));
	CreateDirectory( LPCSTR(strTPATH), NULL);

	TRY
	{
		for( itTMAP = m_mapTMAPDB.begin(); itTMAP != m_mapTMAPDB.end(); itTMAP++)
		{
			LPTMAPDATA pTMAP = (*itTMAP).second;
			MAPTMAPIMG::iterator itTUNIT;

			for( itTUNIT = pTMAP->m_mapTUNIT.begin(); itTUNIT != pTMAP->m_mapTUNIT.end(); itTUNIT++)
			{
				LPTMAPIMG pTUNIT = (*itTUNIT).second;

				lParam->m_pWnd->m_strStatus.Format(
					IDS_FMT_BUILD_TMAP,
					pTMAP->m_dwID,
					pTUNIT ? pTUNIT->m_vID.m_wUnitX : 0,
					pTUNIT ? pTUNIT->m_vID.m_wUnitZ : 0);

				lParam->m_pWnd->SendMessage(
					WM_COMMAND,
					ID_PROGRESS_UPDATE);

				if(pTUNIT)
				{
					strTNAME.Format(
						IDS_FMT_PATH_DATA,
						strTPATH,
						pTMAP->m_dwID,
						pTUNIT->m_vID.m_wUnitX,
						pTUNIT->m_vID.m_wUnitZ);

					CFile vFILE(
						strTNAME,
						CFile::modeCreate|
						CFile::modeWrite|
						CFile::typeBinary);

					CArchive vAR(
						&vFILE,
						CArchive::store);

					MAPTPATHDATA::iterator itTPATH;
					MAPT2DPOINT::iterator itTPOINT;
					MAPDWORD::iterator itDWORD;

					MAPTPATHDATA mapTPATHDATA;
					VDWORD vTBLOCK;

					MakePathData(
						&mapTPATHDATA,
						pTUNIT);

					vAR << DWORD(pTUNIT->m_mapTPOINT.size());
					vTBLOCK.clear();

					for( itTPOINT = pTUNIT->m_mapTPOINT.begin(); itTPOINT != pTUNIT->m_mapTPOINT.end(); itTPOINT++)
					{
						vAR << WORD((*itTPOINT).first)
							<< T2DPOINT((*itTPOINT).second);
					}

					for( itDWORD = pTUNIT->m_mapTLINE.begin(); itDWORD != pTUNIT->m_mapTLINE.end(); itDWORD++)
						if( (*itDWORD).second == TLINE_BLOCK )
							vTBLOCK.push_back((*itDWORD).first);

					DWORD dwCount = DWORD(vTBLOCK.size());
					vAR << dwCount;

					for( DWORD i=0; i<dwCount; i++)
						vAR << DWORD(vTBLOCK[i]);
					vAR << DWORD(mapTPATHDATA.size());

					for( itTPATH = mapTPATHDATA.begin(); itTPATH != mapTPATHDATA.end(); itTPATH++)
					{
						MAPTPATHINFO::iterator itTINFO;
						MAPTPATHINFO mapTFULLPATH;

						MAPT2DPOINT mapTPOINT;
						VDWORD vTPATH;

						mapTFULLPATH.clear();
						mapTPOINT.clear();
						vTPATH.clear();

						MakeFullPathData(
							&mapTPATHDATA,
							&mapTFULLPATH,
							(*itTPATH).first,
							&mapTPOINT,
							&vTPATH, 0.0f);

						vAR << WORD((*itTPATH).first)
							<< DWORD(mapTFULLPATH.size());

						for( itTINFO = mapTFULLPATH.begin(); itTINFO != mapTFULLPATH.end(); itTINFO++)
						{
							LPTPATHINFO pTPATHINFO = (*itTINFO).second;

							dwCount = DWORD(pTPATHINFO->m_vTPATH.size());
							vAR << (*itTINFO).first
								<< pTPATHINFO->m_fDIST
								<< dwCount;

							for( DWORD i=0; i<dwCount; i++)
								vAR << WORD(pTPATHINFO->m_vTPATH[i]);

							delete pTPATHINFO;
						}

						mapTFULLPATH.clear();
						mapTPOINT.clear();
						vTPATH.clear();

						dwTCUR++;
						lParam->m_pWnd->m_bPercent = BYTE(dwTCUR * 100 / dwTotal);

						lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
						lParam->m_pWnd->SendMessage(
							WM_COMMAND,
							ID_PROGRESS_UPDATE);
					}

					ClearPathData(&mapTPATHDATA);
					vTBLOCK.clear();
				}
			}
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL
}

FLOAT CTQuestPathDoc::GetLineLength( T2DPOINT lStart,
									 T2DPOINT lEnd)
{
	FLOAT fX = GET2D_X(lStart) - GET2D_X(lEnd);
	FLOAT fY = GET2D_Y(lStart) - GET2D_Y(lEnd);

	return sqrt(fX * fX + fY * fY);
}

DWORD CTQuestPathDoc::GetAllPathCount()
{
	MAPTMAPDATA::iterator itTMAP;
	DWORD dwCount = 0;

	for( itTMAP = m_mapTMAPDB.begin(); itTMAP != m_mapTMAPDB.end(); itTMAP++)
	{
		LPTMAPDATA pTMAP = (*itTMAP).second;

		if(pTMAP)
		{
			MAPTMAPIMG::iterator itTUNIT;

			for( itTUNIT = pTMAP->m_mapTUNIT.begin(); itTUNIT != pTMAP->m_mapTUNIT.end(); itTUNIT++)
			{
				MAPTPATHDATA mapTPATHDATA;

				MakePathData( &mapTPATHDATA, (*itTUNIT).second);
				dwCount += DWORD(mapTPATHDATA.size());
				ClearPathData(&mapTPATHDATA);
			}
		}
	}

	return dwCount;
}

void CTQuestPathDoc::MakeFullPathData( LPMAPTPATHDATA pTPATHDATA,
									   LPMAPTPATHINFO pTFULLPATH,
									   WORD wPointID,
									   LPMAPT2DPOINT pTPOINT,
									   LPVDWORD pTPATH,
									   FLOAT fDIST)
{
	MAPTPATHDATA::iterator itTPATH = pTPATHDATA->find(wPointID);
	MAPTPATHINFO::iterator itTINFO = pTFULLPATH->find(wPointID);

	pTPOINT->insert( MAPT2DPOINT::value_type( wPointID, 0));
	pTPATH->push_back(wPointID);

	if( itTINFO == pTFULLPATH->end() )
	{
		LPTPATHINFO pTPATHINFO = new TPATHINFO();

		for( int i=0; i<INT(pTPATH->size()); i++)
			pTPATHINFO->m_vTPATH.push_back((*pTPATH)[i]);
		pTPATHINFO->m_fDIST = fDIST;

		pTFULLPATH->insert( MAPTPATHINFO::value_type( wPointID, pTPATHINFO));
	}
	else
	{
		LPTPATHINFO pTPATHINFO = (*itTINFO).second;

		if( pTPATHINFO && pTPATHINFO->m_fDIST > fDIST )
		{
			pTPATHINFO->m_vTPATH.clear();
			pTPATHINFO->m_fDIST = fDIST;

			for( int i=0; i<INT(pTPATH->size()); i++)
				pTPATHINFO->m_vTPATH.push_back((*pTPATH)[i]);
		}
		else
			return;
	}

	if( itTPATH != pTPATHDATA->end() )
	{
		LPMAPFLOAT pTPATHINFO = (*itTPATH).second;
		MAPFLOAT::iterator itFLOAT;

		for( itFLOAT = pTPATHINFO->begin(); itFLOAT != pTPATHINFO->end(); itFLOAT++)
		{
			WORD wTargetID = LOWORD((*itFLOAT).first);

			if( wTargetID == wPointID )
				wTargetID = HIWORD((*itFLOAT).first);

			if( pTPOINT->find(wTargetID) == pTPOINT->end() )
			{
				MAPT2DPOINT::iterator itTPOINT;
				MAPT2DPOINT mapTPOINT;
				VDWORD vTPATH;

				mapTPOINT.clear();
				vTPATH.clear();

				for( itTPOINT = pTPOINT->begin(); itTPOINT != pTPOINT->end(); itTPOINT++)
					mapTPOINT.insert( MAPT2DPOINT::value_type( (*itTPOINT).first, (*itTPOINT).second));

				for( int i=0; i<INT(pTPATH->size()); i++)
					vTPATH.push_back((*pTPATH)[i]);

				MakeFullPathData(
					pTPATHDATA,
					pTFULLPATH,
					wTargetID,
					&mapTPOINT,
					&vTPATH,
					fDIST + (*itFLOAT).second);

				mapTPOINT.clear();
				vTPATH.clear();
			}
		}
	}
}

void CTQuestPathDoc::MakePathData( LPMAPTPATHDATA pTPATHDATA,
								   LPTMAPIMG pTUNIT)
{
	MAPDWORD::iterator itTLINE;
	MAPFLOAT::iterator itTDIST;
	MAPFLOAT mapTDIST;

	ClearPathData(pTPATHDATA);
	mapTDIST.clear();

	for( itTLINE = pTUNIT->m_mapTLINE.begin(); itTLINE != pTUNIT->m_mapTLINE.end(); itTLINE++)
		if( (*itTLINE).second == TLINE_PATH )
		{
			WORD vPointID[2] = {
				LOWORD((*itTLINE).first),
				HIWORD((*itTLINE).first)};
			T2DPOINT vPoint[2] = { 0, 0};

			for( int i=0; i<2; i++)
			{
				MAPT2DPOINT::iterator finder = pTUNIT->m_mapTPOINT.find(vPointID[i]);

				if( finder != pTUNIT->m_mapTPOINT.end() )
					vPoint[i] = (*finder).second;
			}

			if( vPoint[0] && vPoint[1] )
				mapTDIST.insert( MAPFLOAT::value_type( (*itTLINE).first, GetLineLength( vPoint[0], vPoint[1])));
		}

	for( itTDIST = mapTDIST.begin(); itTDIST != mapTDIST.end(); itTDIST++)
	{
		WORD vPointID[2] = {
			LOWORD((*itTDIST).first),
			HIWORD((*itTDIST).first)};

		for( int i=0; i<2; i++)
		{
			MAPTPATHDATA::iterator finder = pTPATHDATA->find(vPointID[i]);
			LPMAPFLOAT pTPATHINFO = NULL;

			if( finder == pTPATHDATA->end() )
			{
				pTPATHINFO = new MAPFLOAT();
				pTPATHDATA->insert( MAPTPATHDATA::value_type( vPointID[i], pTPATHINFO));
			}
			else
				pTPATHINFO = (*finder).second;

			MAPFLOAT::iterator itFLOAT = pTPATHINFO->find((*itTDIST).first);
			if( itFLOAT == pTPATHINFO->end() )
				pTPATHINFO->insert( MAPFLOAT::value_type( (*itTDIST).first, (*itTDIST).second));
			else
				(*itFLOAT).second = (*itTDIST).second;
		}
	}
}

void CTQuestPathDoc::ClearPathData( LPMAPTPATHDATA pTPATHDATA)
{
	MAPTPATHDATA::iterator itTDATA;

	for( itTDATA = pTPATHDATA->begin(); itTDATA != pTPATHDATA->end(); itTDATA++)
	{
		LPMAPFLOAT pTPATHINFO = (*itTDATA).second;

		if(pTPATHINFO)
		{
			pTPATHINFO->clear();
			delete pTPATHINFO;
		}
	}

	pTPATHDATA->clear();
}
