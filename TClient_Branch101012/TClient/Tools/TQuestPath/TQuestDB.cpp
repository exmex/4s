#include "StdAfx.h"
#include "TQuestDB.h"
#include "TRewardItemSet.h"
#include "TTermItemSet.h"
#include "TQClassSet.h"
#include "TQTermSet.h"
#include "TQuestSet.h"
#include "Resource.h"


CTQuestDB::CTQuestDB()
{
	m_mapTMONITEMDATA.clear();
	m_mapTNPCITEMDATA.clear();

	m_mapTQDATA.clear();
	m_mapTCDATA.clear();
}

CTQuestDB::~CTQuestDB()
{
	ReleaseDATA();
}

void CTQuestDB::ReleaseDATA()
{
	MAPTQDATA::iterator itTQDATA;
	MAPTCDATA::iterator itTCDATA;
	MAPVDWORD::iterator itVDWORD;

	for( itVDWORD = m_mapTMONITEMDATA.begin(); itVDWORD != m_mapTMONITEMDATA.end(); itVDWORD++)
	{
		(*itVDWORD).second->clear();
		delete (*itVDWORD).second;
	}

	for( itVDWORD = m_mapTNPCITEMDATA.begin(); itVDWORD != m_mapTNPCITEMDATA.end(); itVDWORD++)
	{
		(*itVDWORD).second->clear();
		delete (*itVDWORD).second;
	}

	for( itTQDATA = m_mapTQDATA.begin(); itTQDATA != m_mapTQDATA.end(); itTQDATA++)
		delete (*itTQDATA).second;

	for( itTCDATA = m_mapTCDATA.begin(); itTCDATA != m_mapTCDATA.end(); itTCDATA++)
		delete (*itTCDATA).second;

	m_mapTMONITEMDATA.clear();
	m_mapTNPCITEMDATA.clear();
	m_mapTQDATA.clear();
	m_mapTCDATA.clear();
}

BYTE CTQuestDB::LoadDATA()
{
	MAPTQDATA::iterator itTQDATA;

	CDatabase vDB;
	ReleaseDATA();

	if(!vDB.Open(_T("TQUEST")))
		return FALSE;

	CTQClassSet vTQCLASS(&vDB);
	if(!vTQCLASS.Open())
		return FALSE;

	if(!vTQCLASS.IsEOF())
		vTQCLASS.MoveFirst();

	while(!vTQCLASS.IsEOF())
	{
		LPTCDATA pTCDATA = FindTCDATA(vTQCLASS.m_dwClassID);

		if(!pTCDATA)
		{
			pTCDATA = new TCDATA();
			m_mapTCDATA.insert( MAPTCDATA::value_type( vTQCLASS.m_dwClassID, pTCDATA));
		}

		pTCDATA->m_dwClassID = vTQCLASS.m_dwClassID;
		pTCDATA->m_strNAME = vTQCLASS.m_szNAME;

		vTQCLASS.MoveNext();
	}
	vTQCLASS.Close();

	CTQuestSet vTQUEST(&vDB);
	if(!vTQUEST.Open())
	{
		ReleaseDATA();
		return FALSE;
	}

	if(!vTQUEST.IsEOF())
		vTQUEST.MoveFirst();

	while(!vTQUEST.IsEOF())
	{
		LPTQDATA pTQDATA = FindTQDATA(vTQUEST.m_dwQuestID);

		if(!pTQDATA)
		{
			pTQDATA = new TQDATA();
			m_mapTQDATA.insert( MAPTQDATA::value_type( vTQUEST.m_dwQuestID, pTQDATA));
		}

		pTQDATA->m_strCompleteMSG = vTQUEST.m_szComplete;
		pTQDATA->m_strTriggerMSG = vTQUEST.m_szMessage;
		pTQDATA->m_strTITLE = vTQUEST.m_szTitle;

		pTQDATA->m_dwTriggerID = vTQUEST.m_dwTriggerID;
		pTQDATA->m_dwClassID = vTQUEST.m_dwClassID;
		pTQDATA->m_dwQuestID = vTQUEST.m_dwQuestID;

		pTQDATA->m_bTriggerType = vTQUEST.m_bTriggerType;
		pTQDATA->m_bType = vTQUEST.m_bType;

		vTQUEST.MoveNext();
	}
	vTQUEST.Close();

	for( itTQDATA = m_mapTQDATA.begin(); itTQDATA != m_mapTQDATA.end(); itTQDATA++)
		if(!LoadTTERM( (*itTQDATA).second, &vDB))
		{
			ReleaseDATA();
			return FALSE;
		}

	CTRewardItemSet vTREWARD(&vDB);
	if(!vTREWARD.Open())
	{
		ReleaseDATA();
		return FALSE;
	}

	if(!vTREWARD.IsEOF())
		vTREWARD.MoveFirst();

	while(!vTREWARD.IsEOF())
	{
		LPVDWORD pVTNPC = FindVDWORD(
			&m_mapTNPCITEMDATA,
			vTREWARD.m_dwRewardID);

		if(!pVTNPC)
		{
			pVTNPC = new VDWORD();
			m_mapTNPCITEMDATA.insert( MAPVDWORD::value_type( vTREWARD.m_dwRewardID, pVTNPC));
		}

		pVTNPC->push_back(vTREWARD.m_dwTriggerID);
		vTREWARD.MoveNext();
	}
	vTREWARD.Close();

	CTTermItemSet vTTERM(&vDB);
	if(!vTTERM.Open())
	{
		ReleaseDATA();
		return FALSE;
	}

	if(!vTTERM.IsEOF())
		vTTERM.MoveFirst();

	while(!vTTERM.IsEOF())
	{
		BYTE bType = TITEM_LOC_NONE;
		DWORD dwID = 0;

		switch(vTTERM.m_bType)
		{
		case QT_GIVEITEM	:
			switch(vTTERM.m_bTriggerType)
			{
			case TT_EXECQUEST	:
			case TT_COMPLETE	:
				{
					LPTQDATA pTQDATA = FindTQDATA(vTTERM.m_dwTriggerID);

					if( pTQDATA && pTQDATA->m_bTriggerType == TT_TALKNPC )
					{
						dwID = pTQDATA->m_dwTriggerID;
						bType = TITEM_LOC_NPC;
					}
				}

				break;
			}

			break;

		case QT_DROPITEM	:
			if( vTTERM.m_bTriggerType == TT_KILLMON )
			{
				dwID = vTTERM.m_dwTriggerID;
				bType = TITEM_LOC_MON;
			}

			break;

		case QT_ROUTING		:
			if( vTTERM.m_bTriggerType == TT_TALKNPC )
			{
				dwID = vTTERM.m_dwTriggerID;
				bType = TITEM_LOC_NPC;
			}

			break;
		}

		if( bType != TITEM_LOC_NONE )
		{
			LPMAPVDWORD pTITEMDATA = NULL;

			switch(bType)
			{
			case TITEM_LOC_MON	: pTITEMDATA = &m_mapTMONITEMDATA; break;
			case TITEM_LOC_NPC	: pTITEMDATA = &m_mapTNPCITEMDATA; break;
			}

			if(pTITEMDATA)
			{
				LPVDWORD pVDWORD = FindVDWORD(
					pTITEMDATA,
					vTTERM.m_dwTermID);

				if(!pVDWORD)
				{
					pVDWORD = new VDWORD();
					pTITEMDATA->insert( MAPVDWORD::value_type( vTTERM.m_dwTermID, pVDWORD));
				}

				pVDWORD->push_back(dwID);
			}
		}

		vTTERM.MoveNext();
	}
	vTTERM.Close();

	return TRUE;
}

BYTE CTQuestDB::LoadTTERM( LPTQDATA pTQDATA,
						   CDatabase *pDB)
{
	CTQTermSet vTTERM(pDB);

	while(!pTQDATA->m_vTTERM.empty())
	{
		delete pTQDATA->m_vTTERM.back();
		pTQDATA->m_vTTERM.pop_back();
	}

	vTTERM.m_strFilter.Format( IDS_FMT_TTERM_FILTER, pTQDATA->m_dwQuestID);
	if(!vTTERM.Open())
		return FALSE;

	if(!vTTERM.IsEOF())
		vTTERM.MoveFirst();

	while(!vTTERM.IsEOF())
	{
		LPTTDATA pTTDATA = new TTDATA();

		pTTDATA->m_strTermMSG = vTTERM.m_szObjective;
		pTTDATA->m_bTermType = vTTERM.m_bTermType;
		pTTDATA->m_dwTermID = vTTERM.m_dwTermID;
		pTQDATA->m_vTTERM.push_back(pTTDATA);

		vTTERM.MoveNext();
	}

	return TRUE;
}

LPTQDATA CTQuestDB::FindTMISSION( LPTQDATA pTQDATA)
{
	for( BYTE i=0; i<INT(pTQDATA->m_vTTERM.size()); i++)
		if( pTQDATA->m_vTTERM[i]->m_bTermType == QTT_COMPQUEST )
			return FindTQDATA(pTQDATA->m_vTTERM[i]->m_dwTermID);

	return NULL;
}

LPTQDATA CTQuestDB::FindTQDATA( DWORD dwQuestID)
{
	MAPTQDATA::iterator finder = m_mapTQDATA.find(dwQuestID);

	if( finder != m_mapTQDATA.end() )
		return (*finder).second;

	return NULL;
}

LPTCDATA CTQuestDB::FindTCDATA( DWORD dwClassID)
{
	MAPTCDATA::iterator finder = m_mapTCDATA.find(dwClassID);

	if( finder != m_mapTCDATA.end() )
		return (*finder).second;

	return NULL;
}

LPVDWORD CTQuestDB::FindVDWORD( LPMAPVDWORD pTITEMDATA, DWORD dwItemID)
{
	MAPVDWORD::iterator finder = pTITEMDATA->find(dwItemID);

	if( finder != pTITEMDATA->end() )
		return (*finder).second;

	return NULL;
}
