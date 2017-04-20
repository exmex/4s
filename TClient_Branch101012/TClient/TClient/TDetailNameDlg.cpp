#include "StdAfx.h"
#include "TClientGame.h"
#include "Resource.h"

CTDetailNameDlg::CTDetailNameDlg( TComponent*pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase(pParent, pDesc)
{
	SIZE size;

	m_pMID = FindKid( ID_CTRLINST_MID);
	m_pMID->GetComponentSize( &size );
	m_nDefaultWidth = size.cx;
	m_nBlockWidth = size.cx;
	m_nHeight = size.cy;

    m_pHEAD = FindKid( ID_CTRLINST_HEAD);
	m_pHEAD->GetComponentSize( &size );
	m_nDefaultWidth += size.cx;

	m_pTAIL = FindKid( ID_CTRLINST_TAIL);
	m_pTAIL->GetComponentSize( &size );
	m_nDefaultWidth += size.cx;

	m_pTEXT = FindKid( ID_CTRLINST_TEXT);

	m_nTextWidth = 0;
}

CTDetailNameDlg::~CTDetailNameDlg()
{
}

void CTDetailNameDlg::ResetText( CString& str)
{
	if( m_pTEXT->m_strText != str )
	{
		CSize size;
		m_pTEXT->GetTextExtentPoint( str, size);
		m_nTextWidth = size.cx;
		m_pTEXT->m_strText = str;

		if( m_nTextWidth < m_nDefaultWidth )
			RemoveBlock( m_vExtBLOCK.size() );
		else
		{
			INT nExtWidth = m_nTextWidth - m_nDefaultWidth;
			INT nNeedCount = (INT)( floorf( FLOAT(nExtWidth) / m_nBlockWidth + 0.5f )) + 1;

			INT n = nNeedCount - m_vExtBLOCK.size();

			if( n > 0 ) // nNeedCount > m_vExtBLOCK.size()
				AddBlock( n );
			else if( n < 0 ) // nNeedCount < m_vExtBLOCK.size()
				RemoveBlock( -n );
		}
	}
}

void CTDetailNameDlg::ClearText()
{
	m_pTEXT->m_strText.Empty();

	TCOMP_LIST::iterator it = m_vExtBLOCK.begin();
	while( it != m_vExtBLOCK.end() )
	{
		RemoveKid( (*it) );
		delete (*it);
		it = m_vExtBLOCK.erase( it );
	}
}

void CTDetailNameDlg::SetPos( POINT ptCursor )
{
	BOOL bDown = FALSE;
	if( ptCursor.y > m_nHeight )
	{
		ptCursor.y -= m_nHeight;
	}
	else
		bDown = TRUE;

	if( CTClientGame::GetInstance()->GetDevice()->m_option.m_dwScreenX - ptCursor.x < m_nTextWidth )
	{
		ptCursor.x -= (m_nTextWidth+m_nBlockWidth);
	}
	else
		if( bDown )
			ptCursor.x += 32;

	MoveComponent( ptCursor);
}

void CTDetailNameDlg::AddBlock( INT nCount )
{
	for( INT i=0 ; i < nCount ; ++i )
	{
		TComponent* pBLOCK = new TComponent( this, m_pMID->m_pDESC );
		pBLOCK->m_id = GetUniqueID();
		AddKid( pBLOCK );
		m_vExtBLOCK.push_front( pBLOCK );
	}

	RemoveKid(m_pTEXT);
	AddKid(m_pTEXT);

	ReAlignBlock();
}

void CTDetailNameDlg::RemoveBlock( INT nCount )
{
	nCount = m_vExtBLOCK.size() < nCount ? m_vExtBLOCK.size() : nCount;

	TCOMP_LIST::iterator it = m_vExtBLOCK.begin();

	while(nCount--)
	{
		RemoveKid( 	(*it) );
		delete (*it);
		it = m_vExtBLOCK.erase( it );
	}

	RemoveKid(m_pTEXT);
	AddKid(m_pTEXT);

	ReAlignBlock();
}

void CTDetailNameDlg::ReAlignBlock()
{
	INT nFrontWidth = 0;
	SIZE size;
	m_pHEAD->GetComponentSize( &size );
	nFrontWidth += size.cx;
	m_pMID->GetComponentSize( &size );
	nFrontWidth += size.cx;

	TCOMP_LIST::iterator it, end;
	it = m_vExtBLOCK.begin();
	end = m_vExtBLOCK.end();

	CPoint pt( nFrontWidth, 0 );
	for(; it != end ; ++it )
	{
		(*it)->MoveComponent( pt );
		pt.x += m_nBlockWidth;
	}

	m_pTAIL->MoveComponent( pt );
}

BOOL CTDetailNameDlg::CanWithItemUI()
{
	return TRUE;
}