#include "stdafx.h"
#include "TCandidateFrame.h"

CTCandidateFrame::CTCandidateFrame( TComponent *pParent, LP_FRAMEDESC pDesc)
: TCandidate( pParent, pDesc)
{
	static DWORD dwBK[] =
	{
		ID_CTRLINST_BK0,
		ID_CTRLINST_BK1,
		ID_CTRLINST_BK2,
		ID_CTRLINST_BK3,
		ID_CTRLINST_BK4,
		ID_CTRLINST_BK5,
		ID_CTRLINST_BK6,
		ID_CTRLINST_BK7,
		ID_CTRLINST_BK8,
	};

	static DWORD dwSEL[] =
	{
		ID_CTRLINST_SEL0,
		ID_CTRLINST_SEL1,
		ID_CTRLINST_SEL2,
		ID_CTRLINST_SEL3,
		ID_CTRLINST_SEL4,
		ID_CTRLINST_SEL5,
		ID_CTRLINST_SEL6,
		ID_CTRLINST_SEL7,
		ID_CTRLINST_SEL8,
	};

	static DWORD dwNum[] =
	{
		ID_CTRLINST_N0,
		ID_CTRLINST_N1,
		ID_CTRLINST_N2,
		ID_CTRLINST_N3,
		ID_CTRLINST_N4,
		ID_CTRLINST_N5,
		ID_CTRLINST_N6,
		ID_CTRLINST_N7,
		ID_CTRLINST_N8,
	};

	static DWORD dwText[] =
	{
		ID_CTRLINST_TEXT0,
		ID_CTRLINST_TEXT1,
		ID_CTRLINST_TEXT2,
		ID_CTRLINST_TEXT3,
		ID_CTRLINST_TEXT4,
		ID_CTRLINST_TEXT5,
		ID_CTRLINST_TEXT6,
		ID_CTRLINST_TEXT7,
		ID_CTRLINST_TEXT8,
	};

	for( INT i=0 ; i < 9 ; ++i )
	{
		m_pBK[ i ] = FindKid( dwBK[ i ] );
		m_pSEL[ i ] = FindKid( dwSEL[ i ] );
		m_pNUM[ i ] = FindKid( dwNum[ i ] );
		m_pTEXT[ i ] = FindKid( dwText[ i ] );
	}

	m_pUP = FindKid( ID_CTRLINST_UP);
	m_pDOWN = FindKid( ID_CTRLINST_DOWN);

	m_nSelection = -1;
}

CTCandidateFrame::~CTCandidateFrame()
{
}

void CTCandidateFrame::ResetCandidateChar()
{
	m_nSelection = -1;
	m_vCandidateChar.clear();
}

void CTCandidateFrame::SetCandidateChar( std::vector<CString>& vString )
{
	m_vCandidateChar.clear();
	m_vCandidateChar = vString;
}

void CTCandidateFrame::SetSelection( INT nIndex )
{
	m_nSelection = nIndex;
}

HRESULT CTCandidateFrame::Render( DWORD dwTickCount )
{
	INT nCount = m_vCandidateChar.size();

	for( INT i=0 ; i < 9 ; ++i )
	{
		m_pBK[ i ]->ShowComponent( FALSE );
		m_pSEL[ i ]->ShowComponent( FALSE );
		m_pNUM[ i ]->ShowComponent( FALSE );
		m_pTEXT[ i ]->ShowComponent( FALSE );
	}

	for( INT i=0 ; i < nCount ; ++i )
	{
		m_pBK[ i ]->ShowComponent( m_nSelection != i );
		m_pSEL[ i ]->ShowComponent( m_nSelection == i );
		m_pNUM[ i ]->ShowComponent( TRUE );
		m_pTEXT[ i ]->m_strText = m_vCandidateChar[ i ];
		m_pTEXT[ i ]->ShowComponent( TRUE );
	}

	CSize sizeUP, sizeBK;
	CPoint pointDOWN;
	
	m_pUP->GetComponentRealSize(&sizeUP);
	m_pBK[0]->GetComponentRealSize(&sizeBK);
	m_pDOWN->GetComponentPos( &pointDOWN);
	pointDOWN.y = sizeUP.cy + sizeBK.cy*nCount;
	m_pDOWN->MoveComponent( pointDOWN);
	
	return TFrame::Render( dwTickCount );
}

void CTCandidateFrame::MoveComponentAdjustText( CRect& rtTEXT )
{
	CPoint pointRESULT(0,0);

	CSize sizeUP, sizeDOWN, sizeBK;
	m_pUP->GetComponentRealSize(&sizeUP);
	m_pDOWN->GetComponentRealSize(&sizeDOWN);
	m_pBK[0]->GetComponentRealSize(&sizeBK);

	INT nHeight = sizeUP.cy + sizeDOWN.cy + sizeBK.cy*m_vCandidateChar.size();
	INT nWidth = sizeUP.cx;

	CPoint pointScreen = CTClientUIBase::GetScreenRect().BottomRight();
	if( pointScreen.y - rtTEXT.bottom > nHeight )
		pointRESULT.y = rtTEXT.bottom;
	else
		pointRESULT.y = rtTEXT.top - nHeight;

	pointRESULT.x = rtTEXT.left;

	TFrame::MoveComponent( pointRESULT );
}