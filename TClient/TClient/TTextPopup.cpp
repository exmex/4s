#include "stdafx.h"
#include "TTextPopup.h"

CTTextPopup::CTTextPopup( TComponent *pParent, LP_FRAMEDESC pDesc)
:TFrame( pParent, pDesc)
{
	m_pTOP = FindKid( ID_CTRLINST_UPPER );
	m_pBOTTOM = FindKid( ID_CTRLINST_LOWER );

	CPoint ptBLOCK;
	m_pMIDDLE = FindKid( ID_CTRLINST_MIDDLE );
	m_pMIDDLE->GetComponentPos(&ptBLOCK);

	SIZE size;
	m_pMIDDLE->GetComponentRealSize(&size);
	m_nHeightBLOCK = size.cy;

	CPoint ptTEXT;
	m_pTEXT = FindKid( ID_CTRLINST_TEXT );
	m_pTEXT->GetComponentPos(&ptTEXT);
	m_pTEXT->GetComponentRealSize(&m_sizeTEXT);

	m_ptTEXTOffset = ptTEXT - ptBLOCK;

	RemoveKid( m_pTOP );
	RemoveKid( m_pBOTTOM );
	RemoveKid( m_pMIDDLE );
	RemoveKid( m_pTEXT );
}

void CTTextPopup::ClearBlock()
{
	for( int i=0 ; i < m_vBLOCK.size() ; ++i )
		delete m_vBLOCK[ i ];

	for( int i=0 ; i < m_vTEXT.size() ; ++i )
		delete m_vTEXT[ i ];

	m_vBLOCK.clear();
	m_vTEXT.clear();
}

void CTTextPopup::SetText( CString& strTEXT )
{
	ClearBlock();

	VECTORSTRING outResult;
	m_pTEXT->SeparateTextFitSize(
		strTEXT,
		m_sizeTEXT,
		outResult );

	for( int i=0 ; i < outResult.size() ; ++i )
	{
		TComponent* pNewTEXT = new TComponent(NULL, m_pTEXT->m_pDESC);
		pNewTEXT->m_strText = outResult[ i ];
		m_vTEXT.push_back( pNewTEXT );

		TComponent* pNewBLOCK = new TComponent(NULL, m_pMIDDLE->m_pDESC);
		m_vBLOCK.push_back( pNewBLOCK );
	}

	RealignPos();
}

void CTTextPopup::RealignPos()
{
	CPoint pt(m_rc.left, m_rc.top);
	m_pTOP->MoveComponent(pt);

	CSize size;
	m_pTOP->GetComponentRealSize(&size);
	pt.y += size.cy;

    for( int i=0 ; i < m_vBLOCK.size() ; ++i )
	{
		TComponent* pBLOCK = m_vBLOCK[ i ];
		TComponent* pTEXT = m_vTEXT[ i ];
		
		pBLOCK->MoveComponent(pt);
		CPoint ptText = pt + m_ptTEXTOffset;
		pTEXT->MoveComponent(ptText);
		pt.y += m_nHeightBLOCK;
	}

	m_pBOTTOM->MoveComponent(pt);
}

HRESULT CTTextPopup::Render( DWORD dwTickCount )
{
	m_pTOP->Render(dwTickCount);
	
	m_pBOTTOM->Render(dwTickCount);
	
	for( int i=0 ; i < m_vBLOCK.size() ; ++i )
		m_vBLOCK[ i ]->Render(dwTickCount);

	for( int i=0 ; i < m_vTEXT.size() ; ++i )
		m_vTEXT[ i ]->Render(dwTickCount);

	return S_OK;
}

CTTextPopup::~CTTextPopup()
{
	ClearBlock();

	delete m_pTOP;
	delete m_pBOTTOM;
	delete m_pMIDDLE;
	delete m_pTEXT;
}