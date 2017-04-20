// FrameGroup.cpp: implementation of the CFrameGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFrameGroup::CFrameGroup():
TFrame( NULL, NULL)
{
	m_mapFrame.clear();

	m_pGDIFont = NULL;
	m_pTParser = NULL;
	m_dwLevel = 0;
}

CFrameGroup::~CFrameGroup()
{
	m_mapFrame.clear();
}

DWORD CFrameGroup::GetLevel()
{
	return m_dwLevel;
}

void CFrameGroup::ClearFrame()
{
	m_mapFrame.clear();
	Clear();
}

void CFrameGroup::SetLevel( DWORD dwLevel)
{
	MAPFRAME::iterator it;

	for( it = m_mapFrame.begin(); it != m_mapFrame.end(); it++)
	{
		(*it).second->EnableComponent((*it).first == dwLevel);
		(*it).second->ShowComponent((*it).first == dwLevel);
		((TComponent *) (*it).second)->SetFocus((*it).first == dwLevel);

		if( (*it).first == dwLevel )
		{
			(*it).second->GetComponentRect(&m_rc);
			m_id = (*it).second->m_id;
		}
	}

	m_dwLevel = dwLevel;
	SetFocus(m_id);
}

void CFrameGroup::ResetFocus()
{
	CRect rect = m_rc;

	SetLevel(m_dwLevel);
	m_rc = rect;
}

void CFrameGroup::AddFrame( DWORD dwFrameID, DWORD dwLevel)
{
	if(!m_pTParser)
		return;
	DelFrame(dwLevel);

	m_mapFrame.insert( MAPFRAME::value_type(
		dwLevel,
		(TFrame *) AddKid(m_pTParser->FindFrameTemplate(dwFrameID))));
}

void CFrameGroup::DelFrame( DWORD dwLevel)
{
	MAPFRAME::iterator finder = m_mapFrame.find(dwLevel);

	if( finder != m_mapFrame.end() )
	{
		RemoveKid((*finder).second);
		delete (*finder).second;

		m_mapFrame.erase(finder);
	}

	ResetFocus();
}

void CFrameGroup::SetParent( TComponent *pParent)
{
	if( m_pParent )
	{
		TComponent* pBackup = m_pParent;

		pBackup->RemoveKid(this);
		pBackup->ResetFocus();

		m_pParent = NULL;
	}

	if( pParent )
	{
		pParent->AddKid(this);
		pParent->SetFocus(m_id);
		
		m_pParent = pParent;
	}
}

TFrame *CFrameGroup::GetCurrentFrame()
{
	return FindFrame(m_dwLevel);
}

TFrame *CFrameGroup::FindFrame( DWORD dwLevel)
{
	MAPFRAME::iterator finder = m_mapFrame.find(dwLevel);

	if( finder != m_mapFrame.end() )
		return (*finder).second;

	return NULL;
}
