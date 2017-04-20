// TPathTimeDoc.cpp : implementation of the CTPathTimeDoc class
//

#include "stdafx.h"
#include "TPathTime.h"

#include "TPathTimeDoc.h"
#include "TPathTimeView.h"
#include "MainFrm.h"

#include "TTimePerMeterDlg.h"
#include "SetJointSizeDlg.h"
#include "UnitDlg.h"
#include "SnapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTPathTimeDoc

IMPLEMENT_DYNCREATE(CTPathTimeDoc, CDocument)

BEGIN_MESSAGE_MAP(CTPathTimeDoc, CDocument)
	ON_COMMAND(ID_FILE_LOADBMP, OnFileLoadbmp)
	ON_COMMAND(ID_SPOT_SPOTDISABLE, OnSpotSpotdisable)
	ON_UPDATE_COMMAND_UI(ID_SPOT_SPOTDISABLE, OnUpdateSpotSpotdisable)
	ON_COMMAND(ID_SPOT_SPOTSELECT, OnSpotSpotselect)
	ON_UPDATE_COMMAND_UI(ID_SPOT_SPOTSELECT, OnUpdateSpotSpotselect)
	ON_COMMAND(ID_SPOT_SPOTADD, OnSpotSpotadd)
	ON_UPDATE_COMMAND_UI(ID_SPOT_SPOTADD, OnUpdateSpotSpotadd)
	ON_COMMAND(ID_SPOT_SPOTDELETE, OnSpotSpotdelete)
	ON_UPDATE_COMMAND_UI(ID_SPOT_SPOTDELETE, OnUpdateSpotSpotdelete)
	ON_COMMAND(ID_SPOT_CALCRETURN, OnSpotCalcreturn)
	ON_UPDATE_COMMAND_UI(ID_SPOT_CALCRETURN, OnUpdateSpotCalcreturn)
	ON_COMMAND(ID_SPOT_SETTIMEPERMETER, OnSpotSettimepermeter)
	ON_COMMAND(ID_SPOT_SPOTCANCELLASTSELECT, OnSpotSpotcancellastselect)
	ON_UPDATE_COMMAND_UI(ID_SPOT_SPOTCANCELLASTSELECT, OnUpdateSpotSpotcancellastselect)
	ON_COMMAND(ID_UNIT_SELECT, OnUnitSelect)
	ON_COMMAND(ID_JOINT_ADD, OnJointAdd)
	ON_UPDATE_COMMAND_UI(ID_JOINT_ADD, OnUpdateJointAdd)
	ON_COMMAND(ID_JOINT_DELETE, OnJointDelete)
	ON_UPDATE_COMMAND_UI(ID_JOINT_DELETE, OnUpdateJointDelete)
	ON_COMMAND(ID_JOINT_EDIT, OnJointEdit)
	ON_UPDATE_COMMAND_UI(ID_JOINT_EDIT, OnUpdateJointEdit)
	ON_COMMAND(ID_JOINT_MOVE, OnJointMove)
	ON_UPDATE_COMMAND_UI(ID_JOINT_MOVE, OnUpdateJointMove)
	ON_COMMAND(ID_JOINT_SETJOINTSIZE, OnJointSetjointsize)
	ON_UPDATE_COMMAND_UI(ID_JOINT_SETJOINTSIZE, OnUpdateJointSetjointsize)
	ON_COMMAND(ID_JOINT_SETSNAPSIZE, OnJointSetsnapsize)
	ON_UPDATE_COMMAND_UI(ID_JOINT_SETSNAPSIZE, OnUpdateJointSetsnapsize)
END_MESSAGE_MAP()


// CTPathTimeDoc construction/destruction

CTPathTimeDoc::CTPathTimeDoc()
{
	// TODO: add one-time construction code here
	m_bCalcReturn = FALSE;
	m_bEditMode = TEDIT_INFO_TSPOT;
	m_wUnitID = 0;

	m_strBMP.Empty();
	m_pTMOVE = NULL;
	m_hBMP = NULL;

	m_vTSPOTSEL.clear();
	m_vTSPOT.clear();

	m_mapTJOINT.clear();
}

CTPathTimeDoc::~CTPathTimeDoc()
{
	MAPTJOINT::iterator itTJOINT;

	if(m_hBMP)
		DeleteObject(m_hBMP);

	for( itTJOINT = m_mapTJOINT.begin(); itTJOINT != m_mapTJOINT.end(); itTJOINT++)
		delete (*itTJOINT).second;
	m_mapTJOINT.clear();

	while(!m_vTSPOT.empty())
	{
		delete m_vTSPOT.back();
		m_vTSPOT.pop_back();
	}
	m_vTSPOTSEL.clear();
}

BOOL CTPathTimeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	MAPTJOINT::iterator itTJOINT;

	if(m_hBMP)
	{
		DeleteObject(m_hBMP);
		m_hBMP = NULL;
	}

	m_strBMP.Empty();
	m_pTMOVE = NULL;

	for( itTJOINT = m_mapTJOINT.begin(); itTJOINT != m_mapTJOINT.end(); itTJOINT++)
		delete (*itTJOINT).second;
	m_mapTJOINT.clear();

	while(!m_vTSPOT.empty())
	{
		delete m_vTSPOT.back();
		m_vTSPOT.pop_back();
	}
	m_vTSPOTSEL.clear();

	return TRUE;
}

// CTPathTimeDoc serialization

void CTPathTimeDoc::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
	}
	else
	{
	}
}


// CTPathTimeDoc diagnostics

#ifdef _DEBUG
void CTPathTimeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTPathTimeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTPathTimeDoc commands

void CTPathTimeDoc::OnFileLoadbmp()
{
	CFileDialog dlg(
		TRUE,
		_T("*.bmp"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = theApp.m_strPATH;
	if( dlg.DoModal() == IDOK )
	{
		CString strFileName = dlg.GetFileName();
		CString strPathName = dlg.GetPathName();

		theApp.m_strPATH = strPathName.Left(strPathName.GetLength() - strFileName.GetLength());
		theApp.m_strPATH.TrimRight(_T("\\"));
		theApp.m_strPATH = theApp.m_strPATH + CString(_T("\\"));
		LoadBMP(strFileName);

		POSITION pos = GetFirstViewPosition();
		if(pos)
		{
			CTPathTimeView *pVIEW = (CTPathTimeView *) GetNextView(pos);

			if(pVIEW)
				pVIEW->UpdateView();
		}
	}
}

void CTPathTimeDoc::LoadBMP( CString& strFileName)
{
	CString strPathName;

	if(m_hBMP)
	{
		DeleteObject(m_hBMP);
		m_hBMP = NULL;
	}
	m_strBMP.Empty();

	strPathName.Format( _T("%s%s"), theApp.m_strPATH, strFileName);
	CFileFind finder;

	if( !strFileName.IsEmpty() && finder.FindFile(LPCSTR(strPathName)) )
	{
		m_hBMP = (HBITMAP) LoadImage(
			NULL,
			LPCSTR(strPathName),
			IMAGE_BITMAP,
			0, 0,
			LR_CREATEDIBSECTION|
			LR_DEFAULTSIZE|
			LR_LOADFROMFILE);
		m_strBMP = strFileName;
	}
}

void CTPathTimeDoc::OnSpotSpotdisable()
{
	m_bEditMode = TEDIT_INFO_TSPOT;
	ClearTSPOTSEL();
}

void CTPathTimeDoc::OnUpdateSpotSpotdisable( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bEditMode == TEDIT_INFO_TSPOT ? 1 : 0);
}

void CTPathTimeDoc::OnSpotSpotselect()
{
	m_bEditMode = TEDIT_SELECT_TSPOT;

	POSITION pos = GetFirstViewPosition();
	if(pos)
	{
		CTPathTimeView *pVIEW = (CTPathTimeView *) GetNextView(pos);

		if(pVIEW)
			pVIEW->UpdateView();
	}
}

void CTPathTimeDoc::OnUpdateSpotSpotselect( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bEditMode == TEDIT_SELECT_TSPOT ? 1 : 0);
}

void CTPathTimeDoc::OnSpotSpotadd()
{
	m_bEditMode = TEDIT_ADD_TSPOT;
	ClearTSPOTSEL();
}

void CTPathTimeDoc::OnUpdateSpotSpotadd( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bEditMode == TEDIT_ADD_TSPOT ? 1 : 0);
}

void CTPathTimeDoc::OnSpotSpotdelete()
{
	m_bEditMode = TEDIT_DELETE_TSPOT;
	ClearTSPOTSEL();
}

void CTPathTimeDoc::OnUpdateSpotSpotdelete( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bEditMode == TEDIT_DELETE_TSPOT ? 1 : 0);
}

void CTPathTimeDoc::UpdatePathTime()
{
	CMainFrame *pFRAME = (CMainFrame *) AfxGetMainWnd();

	if(pFRAME)
		pFRAME->UpdatePathTime( CalcPathTime(), CalcPathDist());
}

float CTPathTimeDoc::CalcPathDist()
{
	float fResult = 0.0f;

	if( INT(m_vTSPOTSEL.size()) > 1 )
	{
		float fPosX = (float) m_vTSPOTSEL[0]->m_nPosX;
		float fPosY = (float) m_vTSPOTSEL[0]->m_nPosY;

		for( int i=1; i<INT(m_vTSPOTSEL.size()); i++)
		{
			fPosX -= (float) m_vTSPOTSEL[i]->m_nPosX;
			fPosY -= (float) m_vTSPOTSEL[i]->m_nPosY;
			fResult += sqrtf(fPosX * fPosX + fPosY * fPosY);

			fPosX = (float) m_vTSPOTSEL[i]->m_nPosX;
			fPosY = (float) m_vTSPOTSEL[i]->m_nPosY;
		}
	}

	if(m_bCalcReturn)
		fResult *= 2.0f;

	return fResult;
}

int CTPathTimeDoc::CalcPathTime()
{
	return INT(CalcPathDist() * theApp.m_fTIME + 0.99f);
}

void CTPathTimeDoc::OnSpotCalcreturn()
{
	m_bCalcReturn = !m_bCalcReturn;
	UpdatePathTime();
}

void CTPathTimeDoc::OnUpdateSpotCalcreturn( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bCalcReturn ? 1 : 0);
}

void CTPathTimeDoc::OnSpotSettimepermeter()
{
	CTTimePerMeterDlg dlg;

	if( dlg.DoModal() == IDOK && dlg.m_fTIME > 0.0f )
	{
		theApp.m_fTIME = dlg.m_fTIME;
		UpdatePathTime();
	}
}

BOOL CTPathTimeDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	TRY
	{
		CStdioFile file( lpszPathName, CFile::typeText|CFile::modeRead);
		CString strTEXT;
		MAPTJOINT::iterator itTJOINT;

		for( itTJOINT = m_mapTJOINT.begin(); itTJOINT != m_mapTJOINT.end(); itTJOINT++)
			delete (*itTJOINT).second;
		m_mapTJOINT.clear();
		m_pTMOVE = NULL;

		while(!m_vTSPOT.empty())
		{
			delete m_vTSPOT.back();
			m_vTSPOT.pop_back();
		}
		m_vTSPOTSEL.clear();

		file.ReadString(strTEXT);
		strTEXT.Trim();
		LoadBMP(strTEXT);

		int nUnitX;
		int nUnitZ;
		int nCount;

		file.ReadString(strTEXT);
		sscanf( LPCSTR(strTEXT), _T("%d %d"), &nUnitX, &nUnitZ);
		m_wUnitID = MAKEWORD( BYTE(nUnitX), BYTE(nUnitZ));

		file.ReadString(strTEXT);
		sscanf( LPCSTR(strTEXT), _T("%d"), &nCount);

		for( int i=0; i<nCount; i++)
		{
			file.ReadString(strTEXT);
			strTEXT.Trim();

			if(!strTEXT.IsEmpty())
			{
				LPTSPOT pTSPOT = new TSPOT();

				sscanf( LPCSTR(strTEXT), _T("%d %d"), &pTSPOT->m_nPosX, &pTSPOT->m_nPosY);
				m_vTSPOT.push_back(pTSPOT);
			}
		}

		file.ReadString(strTEXT);
		sscanf( LPCSTR(strTEXT), _T("%d"), &nCount);

		for( i=0; i<nCount; i++)
		{
			file.ReadString(strTEXT);
			strTEXT.Trim();

			if(!strTEXT.IsEmpty())
			{
				LPTJOINT pTJOINT = new TJOINT();
				DWORD dwJOINT[8];
				DWORD dwID;

				sscanf( LPCSTR(strTEXT), _T("%d %d %d %d %d %d %d %d %d %d %d %d %d"),
					&dwID,
					&pTJOINT->m_vRECT.left,
					&pTJOINT->m_vRECT.top,
					&pTJOINT->m_vRECT.right,
					&pTJOINT->m_vRECT.bottom,
					&dwJOINT[0],
					&dwJOINT[1],
					&dwJOINT[2],
					&dwJOINT[3],
					&dwJOINT[4],
					&dwJOINT[5],
					&dwJOINT[6],
					&dwJOINT[7]);

				pTJOINT->m_bJOINT = 0;
				for( int j=0; j<8; j++)
					if(dwJOINT[j])
						pTJOINT->m_bJOINT |= BYTE(1) << j;

				m_mapTJOINT.insert( MAPTJOINT::value_type(
					dwID,
					pTJOINT));
			}
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL
	SetModifiedFlag(FALSE);

	return TRUE;
}

BOOL CTPathTimeDoc::OnSaveDocument( LPCTSTR lpszPathName)
{
	TRY
	{
		CStdioFile file( lpszPathName, CFile::typeText|CFile::modeCreate|CFile::modeWrite);
		MAPTJOINT::iterator itTJOINT;
		CString strTEXT;

		m_strBMP.Trim();
		strTEXT.Format( _T("%s\n"), m_strBMP);
		file.WriteString(LPCSTR(strTEXT));

		strTEXT.Format( _T("%d\t%d\n"), LOBYTE(m_wUnitID), HIBYTE(m_wUnitID));
		file.WriteString(LPCSTR(strTEXT));

		strTEXT.Format( _T("%d\n"), INT(m_vTSPOT.size()));
		file.WriteString(LPCSTR(strTEXT));

		for( int i=0; i<INT(m_vTSPOT.size()); i++)
		{
			strTEXT.Format( _T("%d\t%d\n"), m_vTSPOT[i]->m_nPosX, m_vTSPOT[i]->m_nPosY);
			file.WriteString(LPCSTR(strTEXT));
		}

		strTEXT.Format( _T("%d\n"), INT(m_mapTJOINT.size()));
		file.WriteString(LPCSTR(strTEXT));

		for( itTJOINT = m_mapTJOINT.begin(); itTJOINT != m_mapTJOINT.end(); itTJOINT++)
		{
			LPTJOINT pTJOINT = (*itTJOINT).second;

			strTEXT.Format( _T("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"),
				(*itTJOINT).first,
				pTJOINT->m_vRECT.left,
				pTJOINT->m_vRECT.top,
				pTJOINT->m_vRECT.right,
				pTJOINT->m_vRECT.bottom,
				pTJOINT->m_bJOINT & (BYTE(1) << 0) ? TRUE : FALSE,
				pTJOINT->m_bJOINT & (BYTE(1) << 1) ? TRUE : FALSE,
				pTJOINT->m_bJOINT & (BYTE(1) << 2) ? TRUE : FALSE,
				pTJOINT->m_bJOINT & (BYTE(1) << 3) ? TRUE : FALSE,
				pTJOINT->m_bJOINT & (BYTE(1) << 4) ? TRUE : FALSE,
				pTJOINT->m_bJOINT & (BYTE(1) << 5) ? TRUE : FALSE,
				pTJOINT->m_bJOINT & (BYTE(1) << 6) ? TRUE : FALSE,
				pTJOINT->m_bJOINT & (BYTE(1) << 7) ? TRUE : FALSE);
			file.WriteString(LPCSTR(strTEXT));
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL
	SetModifiedFlag(FALSE);

	return TRUE;
}

void CTPathTimeDoc::OnSpotSpotcancellastselect()
{
	if(!m_vTSPOTSEL.empty())
	{
		POSITION pos = GetFirstViewPosition();

		m_vTSPOTSEL.pop_back();
		if(pos)
		{
			CTPathTimeView *pVIEW = (CTPathTimeView *) GetNextView(pos);

			if(pVIEW)
				pVIEW->UpdateView();
		}
	}
}

void CTPathTimeDoc::OnUpdateSpotSpotcancellastselect( CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_vTSPOTSEL.empty() ? FALSE : TRUE);
}

void CTPathTimeDoc::OnUnitSelect()
{
	CUnitDlg dlg;

	dlg.m_wUnitID = m_wUnitID;
	if( dlg.DoModal() == IDOK )
	{
		m_wUnitID = dlg.m_wUnitID;
		UpdateUnitID();
	}
}

void CTPathTimeDoc::UpdateUnitID()
{
	CMainFrame *pFRAME = (CMainFrame *) AfxGetMainWnd();

	if(pFRAME)
		pFRAME->UpdateUnitID(m_wUnitID);
}

void CTPathTimeDoc::ClearTSPOTSEL()
{
	m_vTSPOTSEL.clear();

	POSITION pos = GetFirstViewPosition();
	if(pos)
	{
		CTPathTimeView *pVIEW = (CTPathTimeView *) GetNextView(pos);

		if(pVIEW)
			pVIEW->UpdateView();
	}
}

DWORD CTPathTimeDoc::GetNewJointID()
{
	DWORD dwID = 0;

	while( m_mapTJOINT.find(dwID) != m_mapTJOINT.end() )
		dwID++;

	return dwID;
}

void CTPathTimeDoc::OnJointAdd()
{
	m_bEditMode = TEDIT_ADD_JOINT;
	ClearTSPOTSEL();
}

void CTPathTimeDoc::OnUpdateJointAdd(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bEditMode == TEDIT_ADD_JOINT ? 1 : 0);
}

void CTPathTimeDoc::OnJointDelete()
{
	m_bEditMode = TEDIT_DELETE_JOINT;
	ClearTSPOTSEL();
}

void CTPathTimeDoc::OnUpdateJointDelete(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bEditMode == TEDIT_DELETE_JOINT ? 1 : 0);
}

void CTPathTimeDoc::OnJointEdit()
{
	m_bEditMode = TEDIT_EDIT_JOINT;
	ClearTSPOTSEL();
}

void CTPathTimeDoc::OnUpdateJointEdit(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bEditMode == TEDIT_EDIT_JOINT ? 1 : 0);
}

void CTPathTimeDoc::OnJointMove()
{
	m_bEditMode = TEDIT_MOVE_JOINT;
	ClearTSPOTSEL();
}

void CTPathTimeDoc::OnUpdateJointMove(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bEditMode == TEDIT_MOVE_JOINT ? 1 : 0);
}

void CTPathTimeDoc::OnJointSetjointsize()
{
	CSetJointSizeDlg dlg;
	dlg.DoModal();
}

void CTPathTimeDoc::OnUpdateJointSetjointsize(CCmdUI *pCmdUI)
{
}

void CTPathTimeDoc::OnJointSetsnapsize()
{
	CSnapDlg dlg;

	if( dlg.DoModal() == IDOK )
		theApp.m_nSNAP = dlg.m_nSNAP;
}

void CTPathTimeDoc::OnUpdateJointSetsnapsize(CCmdUI *pCmdUI)
{
}
