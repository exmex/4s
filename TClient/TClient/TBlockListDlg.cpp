#include "Stdafx.h"
#include "TBlockListDlg.h"
#include "TClientWnd.h"
#include "Resource.h"

CTBlockList::CTBlockList(CTBlockListDlg *pParent, LP_FRAMEDESC pDesc)
: TList( pParent, pDesc ),
m_pParent( pParent )
{
}

void CTBlockList::OnNotify( DWORD from, WORD msg, LPVOID param )
{
	TList::OnNotify( from, msg, param );
	m_pParent->_UpdateOpButton();
}

BOOL CTBlockList::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	BOOL bRet = TList::DoMouseWheel( nFlags, zDelta, pt );
	m_pParent->_UpdateOpButton();
	return bRet;
}

CTBlockList::~CTBlockList()
{
}

// =======================================================
CTBlockListDlg::CTBlockListDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
	: ITInnerFrame( pParent, pDesc, TMESSENGER_FRAME_BLOCKLIST)
{
	TList* pList = static_cast<TList*>(FindKid(ID_CTRLINST_LIST));
	RemoveKid( pList );

	m_pList = new CTBlockList( this, pList->m_pDESC );
	m_pList->m_id = pList->m_id;

	delete pList;
	AddKid( m_pList );

	m_pAddBtn = FindKid(ID_CTRLINST_BTN_ADD);
	m_pDelBtn = FindKid(ID_CTRLINST_BTN_DEL);

	CPoint ptList;
	m_pList->GetComponentPos( &ptList );

	m_pOpButton[0] = FindKid( ID_CTRLINST_OP_1 );
	m_pOpButton[1] = FindKid( ID_CTRLINST_OP_2 );
	m_pOpButton[2] = FindKid( ID_CTRLINST_OP_3 );
	m_pOpButton[3] = FindKid( ID_CTRLINST_OP_4 );

	RemoveKid( m_pOpButton[0] );
	RemoveKid( m_pOpButton[1] );
	RemoveKid( m_pOpButton[2] );
	RemoveKid( m_pOpButton[3] );

	m_pOpButton[0]->GetComponentPos( &m_vOpBtnOffset[0] );
	m_pOpButton[1]->GetComponentPos( &m_vOpBtnOffset[1] );
	m_pOpButton[2]->GetComponentPos( &m_vOpBtnOffset[2] );
	m_pOpButton[3]->GetComponentPos( &m_vOpBtnOffset[3] );

	m_vOpBtnOffset[0] -= ptList;
	m_vOpBtnOffset[1] -= ptList;
	m_vOpBtnOffset[2] -= ptList;
	m_vOpBtnOffset[3] -= ptList;
}
// -------------------------------------------------------
CTBlockListDlg::~CTBlockListDlg()
{
	delete m_pOpButton[0];
	delete m_pOpButton[1];
	delete m_pOpButton[2];
	delete m_pOpButton[3];
}
// =======================================================

// =======================================================
void CTBlockListDlg::Add(
	DWORD dwCharID,
	const CString& strName,
	BYTE bOption )
{
	BLOCKCHAR vChar;
	vChar.m_dwCharID = dwCharID;
	vChar.m_strCharName = strName;
	vChar.m_bBlockOption = bOption;

	m_vBlockChars.push_back( vChar );
	m_pList->AddString(strName);

	_UpdateOpButton();
}
// -------------------------------------------------------
void CTBlockListDlg::Remvoe(INT nIndex)
{
	m_pList->DelItem(nIndex);
	m_vBlockChars.erase( m_vBlockChars.begin() + nIndex );

	_UpdateOpButton();
}
// -------------------------------------------------------
void CTBlockListDlg::Clear()
{
	m_pList->RemoveAll();
	m_vBlockChars.clear();

	_UpdateOpButton();
}
// =======================================================
BOOL CTBlockListDlg::IsEmpty() const
{
	return ( m_pList->GetItemCount() == 0 );
}
// -------------------------------------------------------
UINT CTBlockListDlg::GetCount() const
{
	return m_pList->GetItemCount();
}
// =======================================================
CString CTBlockListDlg::GetName(INT nIndex)
{
	if( 0 <= nIndex && nIndex < m_vBlockChars.size() )
	{
		return m_vBlockChars[ nIndex ].m_strCharName;
	}
	return 0;
}
// -------------------------------------------------------
BYTE CTBlockListDlg::GetBlockOption(INT nIndex)
{
	if( 0 <= nIndex && nIndex < m_vBlockChars.size() )
	{
		return m_vBlockChars[ nIndex ].m_bBlockOption;
	}
	return 0;
}
// -------------------------------------------------------
INT CTBlockListDlg::Find(const CString& strName)
{
	for(INT i=0; i<GetCount(); ++i)
	{
		if( GetName(i) == strName )
			return i;
	}

	return T_INVALID;
}
// =======================================================
void CTBlockListDlg::_UpdateOpButton()
{
	int nTop = m_pList->GetTop();
	for( INT i=0 ; i < m_pList->GetItemPerPage() ; ++i )
	{
		BOOL bValid = ( nTop+i < m_pList->GetItemCount() );

		if( bValid )
		{
			if( i < m_vBlockOpBtns.size() )
			{
				BYTE bOp = m_vBlockChars[ nTop+i ].m_bBlockOption;

				m_vBlockOpBtns[ i ].m_nIndex = nTop + i;
				m_vBlockOpBtns[ i ].m_bOpShow[0] = TRUE;
				m_vBlockOpBtns[ i ].m_bOpShow[1] = TRUE;
				m_vBlockOpBtns[ i ].m_bOpShow[2] = TRUE;
				m_vBlockOpBtns[ i ].m_bOpShow[3] = TRUE;

				m_vBlockOpBtns[ i ].m_pOpButton[0]->Select( (BOOL) (bOp & PROTECTED_CHAT) == PROTECTED_CHAT );
				m_vBlockOpBtns[ i ].m_pOpButton[1]->Select( (BOOL) (bOp & PROTECTED_INVITE) == PROTECTED_INVITE );
				m_vBlockOpBtns[ i ].m_pOpButton[2]->Select( (BOOL) (bOp & PROTECTED_DEAL) == PROTECTED_DEAL );
				m_vBlockOpBtns[ i ].m_pOpButton[3]->Select( (BOOL) (bOp & PROTECTED_DUEL) == PROTECTED_DUEL );
			}
			else
			{
				CPoint ptBasisPos;
				m_pList->GetComponentPos( &ptBasisPos );
				int nHeight = m_pList->GetMaxItemHeight();
				ptBasisPos.y += nHeight * i;

				// 추가
				BLOCKOPBTN vOP;
				vOP.m_pOpButton[0] = new TButton( this, m_pOpButton[0]->m_pDESC );
				vOP.m_pOpButton[1] = new TButton( this, m_pOpButton[1]->m_pDESC );
				vOP.m_pOpButton[2] = new TButton( this, m_pOpButton[2]->m_pDESC );
				vOP.m_pOpButton[3] = new TButton( this, m_pOpButton[3]->m_pDESC );

				vOP.m_pOpButton[0]->m_id = GetUniqueID();
				vOP.m_pOpButton[1]->m_id = GetUniqueID();
				vOP.m_pOpButton[2]->m_id = GetUniqueID();
				vOP.m_pOpButton[3]->m_id = GetUniqueID();

				static_cast<TButton*>( vOP.m_pOpButton[0] )->SetStateButton();
				static_cast<TButton*>( vOP.m_pOpButton[1] )->SetStateButton();
				static_cast<TButton*>( vOP.m_pOpButton[2] )->SetStateButton();
				static_cast<TButton*>( vOP.m_pOpButton[3] )->SetStateButton();

				vOP.m_pOpButton[0]->MoveComponent( ptBasisPos + m_vOpBtnOffset[0] );
				vOP.m_pOpButton[1]->MoveComponent( ptBasisPos + m_vOpBtnOffset[1] );
				vOP.m_pOpButton[2]->MoveComponent( ptBasisPos + m_vOpBtnOffset[2] );
				vOP.m_pOpButton[3]->MoveComponent( ptBasisPos + m_vOpBtnOffset[3] );
				m_vBlockOpBtns.push_back( vOP );
				
				// 버튼별 세팅.
				BYTE bOp = m_vBlockChars[ nTop+i ].m_bBlockOption;

				m_vBlockOpBtns[ i ].m_nIndex = nTop + i;
				m_vBlockOpBtns[ i ].m_bOpShow[0] = TRUE;
				m_vBlockOpBtns[ i ].m_bOpShow[1] = TRUE;
				m_vBlockOpBtns[ i ].m_bOpShow[2] = TRUE;
				m_vBlockOpBtns[ i ].m_bOpShow[3] = TRUE;

				m_vBlockOpBtns[ i ].m_pOpButton[0]->Select( (BOOL) (bOp & PROTECTED_CHAT) == PROTECTED_CHAT );
				m_vBlockOpBtns[ i ].m_pOpButton[1]->Select( (BOOL) (bOp & PROTECTED_INVITE) == PROTECTED_INVITE );
				m_vBlockOpBtns[ i ].m_pOpButton[2]->Select( (BOOL) (bOp & PROTECTED_DEAL) == PROTECTED_DEAL );
				m_vBlockOpBtns[ i ].m_pOpButton[3]->Select( (BOOL) (bOp & PROTECTED_DUEL) == PROTECTED_DUEL );

				AddKid( m_vBlockOpBtns[ i ].m_pOpButton[0] );
				AddKid( m_vBlockOpBtns[ i ].m_pOpButton[1] );
				AddKid( m_vBlockOpBtns[ i ].m_pOpButton[2] );
				AddKid( m_vBlockOpBtns[ i ].m_pOpButton[3] );
			}
		}
		else
		{
			if( i < m_vBlockOpBtns.size() )
			{
				m_vBlockOpBtns[ i ].m_nIndex = -1;
				m_vBlockOpBtns[ i ].m_bOpShow[0] = FALSE;
				m_vBlockOpBtns[ i ].m_bOpShow[1] = FALSE;
				m_vBlockOpBtns[ i ].m_bOpShow[2] = FALSE;
				m_vBlockOpBtns[ i ].m_bOpShow[3] = FALSE;
			}
		}
	}

	if( m_bVisible )
		ShowComponent( TRUE );
}
// =======================================================
void CTBlockListDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonDown(nFlags,pt);

	if( m_pAddBtn->HitTest(pt) )
	{
		CTClientWnd* pWnd = CTClientWnd::GetInstance();
		pWnd->MessageBox(
			TSTR_MBOX_ADDTOBLOCKLIST,
			TSTR_NAME,
			0,
			TSTR_YES,
			TSTR_NO,
			ID_FRAME_2BTNMSGBOX,
			GM_ADDTOBLOCKLIST,
			GM_CLOSE_MSGBOX,
			0,
			TRUE,
			TRUE);
	}
	else if( m_pDelBtn->HitTest(pt) )
	{
		MAP_LISTITEM* pITEMS = m_pList->GetCurSelItem();
		if( pITEMS )
		{
			CTMessengerCommander::GetInstance()
				->DoDeleteBlock( (*pITEMS)[0]->m_strText );
		}
	}
	else
	{
		BOOL bFind = FALSE;
		BOOL bFindChat = FALSE;
		std::vector<BLOCKOPBTN>::iterator it, end;
		it = m_vBlockOpBtns.begin();
		end = m_vBlockOpBtns.end();

		for(; it != end ; ++it )
		{
			TButton* pBtn = static_cast<TButton*>( it->m_pOpButton[0] );
			if( pBtn->IsVisible() && pBtn->HitTest( pt ) )
			{
				if( pBtn->IsStateDown() )
					m_vBlockChars[ it->m_nIndex ].m_bBlockOption |= PROTECTED_CHAT;
				else
					m_vBlockChars[ it->m_nIndex ].m_bBlockOption &= (~PROTECTED_CHAT);

				bFind = TRUE;
				bFindChat = TRUE;
			}
			
			pBtn = static_cast<TButton*>( it->m_pOpButton[1] );
			if( pBtn->IsVisible() && pBtn->HitTest( pt ) )
			{
				if( pBtn->IsStateDown() )
					m_vBlockChars[ it->m_nIndex ].m_bBlockOption |= PROTECTED_INVITE;
				else
					m_vBlockChars[ it->m_nIndex ].m_bBlockOption &= (~PROTECTED_INVITE);

				bFind = TRUE;
			}

			pBtn = static_cast<TButton*>( it->m_pOpButton[2] );
			if( pBtn->IsVisible() && pBtn->HitTest( pt ) )
			{
				if( pBtn->IsStateDown() )
					m_vBlockChars[ it->m_nIndex ].m_bBlockOption |= PROTECTED_DEAL;
				else
					m_vBlockChars[ it->m_nIndex ].m_bBlockOption &= (~PROTECTED_DEAL);

				bFind = TRUE;
			}

			pBtn =static_cast<TButton*>( it->m_pOpButton[3] );
			if( pBtn->IsVisible() && pBtn->HitTest( pt ) )
			{
				if( pBtn->IsStateDown() )
					m_vBlockChars[ it->m_nIndex ].m_bBlockOption |= PROTECTED_DUEL;
				else
					m_vBlockChars[ it->m_nIndex ].m_bBlockOption &= (~PROTECTED_DUEL);

				bFind = TRUE;
			}

			if( bFind )
			{
				CTClientSession* pSESSION = CTClientGame::GetInstance()->GetSession();
				CTClientSession* pRELAY = CTClientGame::GetInstance()->GetRelaySession();

				if( pSESSION )
				{
					pSESSION->SendCS_PROTECTEDOPTION_REQ(
						m_vBlockChars[ it->m_nIndex ].m_dwCharID,
						m_vBlockChars[ it->m_nIndex ].m_bBlockOption);
				}

				if( bFindChat && pRELAY )
				{
					pRELAY->SendCS_RELAYPROTECTEDOPTION_REQ(
						m_vBlockChars[ it->m_nIndex ].m_strCharName,
						m_vBlockChars[ it->m_nIndex ].m_bBlockOption );
				}

				return ;
			}
		}
	}
}
// =======================================================
void CTBlockListDlg::RequestInfo()
{
}
// =======================================================
void CTBlockListDlg::ResetInfo()
{
}
// =======================================================
void CTBlockListDlg::ShowComponent( BOOL bVisible )
{
	ITInnerFrame::ShowComponent( bVisible );
	
	if( bVisible )
	{
		std::vector<BLOCKOPBTN>::iterator it, end;
		it = m_vBlockOpBtns.begin();
		end = m_vBlockOpBtns.end();

		for(; it != end ; ++it )
		{
			it->m_pOpButton[0]->ShowComponent( it->m_bOpShow[0] );
			it->m_pOpButton[1]->ShowComponent( it->m_bOpShow[1] );
			it->m_pOpButton[2]->ShowComponent( it->m_bOpShow[2] );
			it->m_pOpButton[3]->ShowComponent( it->m_bOpShow[3] );
		}
	}
}


































