#include "Stdafx.h"
#include "TClientGame.h"

// ===============================================================================
CRect CTChatList::s_rcDragZone(0,0,TBASE_SCREEN_X,TBASE_SCREEN_Y);
BYTE CTChatList::m_bUpdateScroll = FALSE;
// ===============================================================================
CTChatList::CTChatList(CTChatFrame* pHost, LP_FRAMEDESC pDesc, TComponent* pOuterParent)
	: TList(pHost, pDesc), m_dwChatFlag(TCHAT_FLAG_ALL_CHAT_WITH_INFO),
	  m_bEnableDrag(FALSE), m_bMoveDragging(FALSE), m_bResizeDragging(FALSE), m_bOuter(FALSE), m_pHost(pHost),
	  m_pOuterHost(pOuterParent), m_bHideBackground(FALSE),
	  m_bNotifyMessage(FALSE), m_bAutoScroll(TRUE), m_nResizeDir(1), m_eCursorType(TCURSOR_COUNT),
	  m_pSizeBarUP( NULL), m_pSizeBarDOWN( NULL)
{
	m_bWordBreak = FALSE;
	m_blUserColor = TRUE;
	SetUseShadowFont(TRUE);
}
// -------------------------------------------------------------------------------
CTChatList::~CTChatList()
{
}
// ===============================================================================


// ===============================================================================
void CTChatList::Init(UINT iIndex, const CString& strTitle, DWORD dwFlag, const CRect& rc, LP_FRAMEDESC pDescFastDown)
{
	m_IndexInGroup = iIndex;
	m_strTitle = strTitle;
	m_dwChatFlag = dwFlag;
	m_rc = rc;

	m_bNotifyMessage = FALSE;
	m_bAutoScroll = TRUE;

	if( m_pScroll && m_pScroll->IsTypeOf(TCML_TYPE_SCROLL) )
	{
		CRect rcScroll;
		m_pScroll->GetComponentRect(&rcScroll);
	}
}
// ===============================================================================
void CTChatList::StartDrag(const CPoint& pt)
{
	if( !m_bOuter )
		return;

	m_bEnableDrag = TRUE;
	m_bMoveDragging = TRUE;

	m_ptDragOffset = CPoint( m_rc.Width()/2, m_rc.Height()/2 );
	MoveComponent(pt-m_ptDragOffset);

	CheckDragZone();
}
// -------------------------------------------------------------------------------
void CTChatList::SetDragEnable(BOOL bEnable)
{
	m_bEnableDrag = bEnable && m_bOuter;	
}
// ===============================================================================

// ===============================================================================
void CTChatList::SetOuter(BOOL bOuter)
{
	if( m_bOuter == bOuter )
		return;

	m_bOuter = bOuter;
	
	if( !m_bOuter )
	{
		m_bEnableDrag = FALSE;

		m_pOuterHost->RemoveKid(this);
		m_pHost->AddKid(this);
		m_pParent = m_pHost;

		CRect rtDefault = m_pHost->GetDefaultListRect();
		MoveComponent( CPoint(rtDefault.left, rtDefault.bottom-m_rc.Height()) );
	}
	else
	{
		m_bEnableDrag = TRUE;

		m_pHost->RemoveKid(this);
		m_pOuterHost->AddKid(this);
		m_pParent = m_pOuterHost;
	}

	ShowComponent( m_pHost->IsVisible() && m_bOuter );
}
// ===============================================================================
INT CTChatList::AddMsgLine(
	const CString& strTYPENAME,
	const CString& strLINE,
	DWORD dwColor,
	CString strSenderName,
	DWORD dwSenderID,
	DWORD dwChatFlag,
	INT nHMargine )
{
	MAP_LISTITEM* pItemArray = new MAP_LISTITEM;

	if( !pItemArray )
		return -1;

	int nLineCount = GetItemCount();
	BOOL bDelTop = FALSE;
	BYTE bAutoScroll =
		IsVisible() &&
		!m_bUpdateScroll &&
		(!m_bAutoScroll || m_nItemPerPage < nLineCount - m_nTop) ? FALSE : TRUE;

	if( nLineCount == TCHAT_SIZE )
	{
		bDelTop = TRUE;
		DelItem(0);
	}
	
	CTChatListItem *pItem;

	switch( dwChatFlag )
	{
	case TCHAT_FLAG_INFO:
	case TCHAT_FLAG_SHOW:
		{
			pItem = new CTChatListItem();
			pItem->m_strText = strLINE;
			pItem->m_strName = strSenderName;
			pItem->m_param = dwSenderID;
			pItem->m_dwTextColor = dwColor;
			pItem->m_nHMargine = nHMargine;
			pItemArray->insert( std::make_pair(TCHATCOL_INFO,pItem) );
		}
		break;
	default:
		{
			pItem = new CTChatListItem();
			pItem->m_strText = strTYPENAME;
			pItem->m_strName = strSenderName;
			pItem->m_param = dwSenderID;
			pItem->m_dwTextColor = dwColor;
			pItemArray->insert( std::make_pair(TCHATCOL_NAME,pItem) );

			pItem = new CTChatListItem();
			pItem->m_strText = strLINE;
			pItem->m_strName = strSenderName;
			pItem->m_param = dwSenderID;
			pItem->m_dwTextColor = dwColor;
			pItemArray->insert( std::make_pair(TCHATCOL_MSG,pItem) );
		}
		break;
	};

	m_items.push_back( pItemArray );

	if(bAutoScroll)
		m_nTop = max( GetItemCount()-m_nItemPerPage, 0 );
	else 
	{
		if( bDelTop )
			m_nTop = max(m_nTop-1,0);

		m_bNotifyMessage = TRUE;
		ShowComponent(m_bVisible);
	}
	UpdateScrollPosition();

	return  m_items.size() - 1;
}
// ===============================================================================
void CTChatList::SetHideBackground(BOOL bHide)
{
	if( m_bHideBackground != bHide )
	{
		m_bHideBackground = bHide;
		ShowComponent(m_bVisible);
	}
}
// ===============================================================================



// ===============================================================================
BOOL CTChatList::HitTest(CPoint pt)
{
	if( m_pHost->IsLock() )
	{
		if( m_pScroll && m_pScroll->HitTest(pt) )
			return TRUE;

		DWORD dwInfo;
		if( HitTestTextSetting(pt, &dwInfo) )
		{
			CTTextLinkData* pData = CTTextLinker::GetInstance()->GetTextLinkData(dwInfo);
			if( pData && pData->IsItemType() )
				return TRUE;
		}

		return FALSE; 
	}

	return TList::HitTest(pt);
}
// ===============================================================================
void CTChatList::MoveComponent( CPoint pt )
{
	TComponent::MoveComponent( pt );

	CRect rcDrag( CRect( CPoint(0,0), CTClientUIBase::m_vBasis[TBASISPOINT_RIGHT_BOTTOM] ) );

	CPoint vCenter(
		m_rc.left + m_rc.Width()/2 + rcDrag.left,
		m_rc.top + m_rc.Height()/2 + rcDrag.top ); // rcDrag 좌표계에서 현 컴포넌트의 중앙점 위치값.

	FLOAT unit_w = rcDrag.Width() / 3.0f;
	FLOAT unit_h = rcDrag.Height() / 3.0f;

	INT unit_x = (INT)( FLOAT(vCenter.x) / unit_w );
	INT unit_y = (INT)( FLOAT(vCenter.y) / unit_h );

	static BYTE vBASIS[][3] =
	{
		{ TBASISPOINT_LEFT_TOP, TBASISPOINT_LEFT_MIDDLE, TBASISPOINT_LEFT_BOTTOM },
		{ TBASISPOINT_CENTER_TOP, TBASISPOINT_CENTER_MIDDLE, TBASISPOINT_CENTER_BOTTOM },
		{ TBASISPOINT_RIGHT_TOP, TBASISPOINT_RIGHT_MIDDLE, TBASISPOINT_RIGHT_BOTTOM },
	};

	CPoint vBasis;

	switch( unit_x )
	{
	case 0: // TBASISPOINT_LEFT_X
			vBasis.x = rcDrag.left;
		break;
	case 1: // TBASISPOINT_CENTER_X
			vBasis.x = rcDrag.left + rcDrag.Width()/2;
		break;
	case 2: // TBASISPOINT_RIGHT_X
			vBasis.x = rcDrag.right;
		break;
	}

	switch( unit_y )
	{
	case 0:
		vBasis.y = rcDrag.top;
		break;
	case 1:
		vBasis.y = rcDrag.top + rcDrag.Height()/2;
		break;
	case 2:
		vBasis.y = rcDrag.bottom;
		break;
	}

	m_vCompOffset = CPoint(m_rc.left, m_rc.top)-vBasis;
	m_bBasisPoint = vBASIS[unit_x][unit_y];	
}

// ===============================================================================
void CTChatList::ShowComponent( BOOL bVisible )
{
	m_bVisible = bVisible;
	TComponent::ShowComponent( m_bVisible );

	if( !m_bHideBackground && m_bVisible )
	{
		if( m_pScroll )
			m_pScroll->ShowComponent( TRUE );
		if( m_pSizeBarUP )
			m_pSizeBarUP->ShowComponent( !m_pHost->IsLock() );
		if( m_pSizeBarDOWN  )
			m_pSizeBarDOWN->ShowComponent( !m_pHost->IsLock() && IsOuter() );
	}
	else
	{
		if( m_pScroll )
			m_pScroll->ShowComponent( FALSE );
		if( m_pSizeBarUP )
			m_pSizeBarUP->ShowComponent( FALSE );
		if( m_pSizeBarDOWN )
			m_pSizeBarDOWN->ShowComponent( FALSE );
	}
}
// ===============================================================================
void CTChatList::ResetPosition()
{
	CTChatList::MoveComponent( CTClientUIBase::m_vBasis[m_bBasisPoint] + m_vCompOffset );
}
// ===============================================================================
void CTChatList::SetHeight( INT nHeight )
{
	TList::SetHeight2( nHeight );

	if( m_pSizeBarUP )
	{
		SIZE size;
		m_pSizeBarUP->GetComponentRealSize( &size );
		m_pSizeBarUP->MoveComponent( CPoint(0, -size.cy-1) );
	}

	if( m_pSizeBarDOWN )
	{
		m_pSizeBarDOWN->MoveComponent( CPoint(0, m_rc.Height()) );
	}
}
// ===============================================================================
void CTChatList::OnLButtonDown(UINT nFlags, CPoint pt)
{
	TList::OnLButtonDown(nFlags,pt);
	DWORD dwInfo;

	if(HitTestTextSetting(pt, &dwInfo))
	{
		CTTextLinkData *pLinkData = CTTextLinker::GetInstance()->GetTextLinkData(dwInfo);

		if( pLinkData && pLinkData->IsItemType() )
		{
			CTClientItem *pItem = pLinkData->GetLastUnkpackItem();

			if(pItem)
				CTClientGame::GetInstance()->ShowChatDetailInfo(pItem);

			return;
		}
	}

	if( !m_pHost->IsLock() )
	{
		CRect rtResizer;

		if( m_pSizeBarUP && m_pSizeBarUP->HitTest( pt ) )
		{
			m_bResizeDragging = TRUE;
			m_nResizeDir = 1;

			m_ptDragOffset.x = m_rc.left;
			m_ptDragOffset.y = m_rc.bottom;
			return ;
		}

		if( IsOuter() )
		{
			if( m_pSizeBarDOWN && m_pSizeBarDOWN->HitTest( pt ) )
			{
				m_bResizeDragging = TRUE;
				m_nResizeDir = 0;
				
				m_ptDragOffset.x = m_rc.left;
				m_ptDragOffset.y = m_rc.top;
				return ;
			}
		}

		if( m_bEnableDrag && HitTest(pt) && (!m_pScroll || !m_pScroll->HitTest(pt)) )
		{
			m_bMoveDragging = TRUE;
			m_ptDragOffset = pt - CPoint(m_rc.left, m_rc.top);
			return ;
		}
	}
}
// -------------------------------------------------------------------------------
void CTChatList::OnLButtonUp(UINT nFlags, CPoint pt)
{
	TList::OnLButtonUp(nFlags,pt);

	if( m_pHost )
	{
		if( m_bMoveDragging && !m_pHost->IsLock() )
		{
			if( m_pHost->HitTest(pt) )
			{
				CTChatTabButton* pTab = m_pHost->FindTab(this);

				if( pTab )
				{
					m_pHost->AddTabAndList(pTab);
					m_pHost->ForceFocus();
				}
			}
		}
		
		if( GetFocus() && IsOuter() )
			m_pHost->ForceFocus();

		m_bMoveDragging = FALSE;
		m_bResizeDragging = FALSE;
	}
}
// -------------------------------------------------------------------------------
void CTChatList::OnMouseMove(UINT nFlags, CPoint pt)
{
	m_eCursorType = TCURSOR_COUNT;

	TList::OnMouseMove(nFlags,pt);

	if( !m_pHost->IsLock() )
	{
		if( m_bResizeDragging )
		{
			m_pHost->ResetTick();
			
			CPoint ptInit( m_ptDragOffset );
			ComponentToScreen( &ptInit );

			if( m_nResizeDir )
			{
				if( 100 > pt.y )
					pt.y = 100;

				INT nHeight = ptInit.y - pt.y;

				if(  nHeight < 0 || nHeight < m_pHost->GetDefaultListHeight() )
					nHeight = m_pHost->GetDefaultListHeight();

				if( IsOuter() )
				{
					SetHeight( nHeight );
					MoveComponent( CPoint( m_ptDragOffset.x, m_ptDragOffset.y - m_rc.Height() ) );
				}
				else
				{
					m_pHost->SetHeightAllInnerChatList( nHeight );
				}				
			}
			else
			{
				CRect rtScreen = CTClientUIBase::GetScreenRect();

				if( rtScreen.bottom - 100 < pt.y )
					pt.y = rtScreen.bottom - 100;

				INT nHeight = pt.y - ptInit.y;

				if( nHeight < 0 || nHeight < m_pHost->GetDefaultListHeight() )
					nHeight = m_pHost->GetDefaultListHeight();

				SetHeight( nHeight );
			}

			m_nTop = max( GetItemCount()-m_nItemPerPage, 0 );
			UpdateScrollPosition();

			m_eCursorType = TCURSOR_SIZE;
		}
		else if( m_bEnableDrag && m_bMoveDragging )
		{
			m_pHost->ResetTick();

			TComponent::MoveComponent(pt-m_ptDragOffset);

			CRect rcSnap;
			if( m_pHost->SnapList(this, rcSnap) )
				SetComponentRect(rcSnap);

			CheckDragZone();
		}
		else
		{
			if( m_pSizeBarUP && m_pSizeBarUP->HitTest( pt ) )
				m_eCursorType = TCURSOR_SIZE;

			if( m_pSizeBarDOWN && IsOuter() && m_pSizeBarDOWN->HitTest( pt) )
				m_eCursorType = TCURSOR_SIZE;
		}
	}
}
// ===============================================================================
HRESULT CTChatList::Render( DWORD dwTickCount )
{
	if( !m_bHideBackground && m_bVisible )
	{
		LPDIRECT3DDEVICE9 pDevice = CTachyonRes::m_pDEVICE->m_pDevice;

		pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0x81000000 );

		pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
		pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
		pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
		pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

		pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

		pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);

		pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
		pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);

		CRect rtScreen = CTClientUIBase::GetScreenRect();
		FLOAT fScreenWidth = (FLOAT) rtScreen.Width();
		FLOAT fScreenHeight = (FLOAT) rtScreen.Height();

		CRect rc = m_rc;
		ComponentToScreen( &rc );

		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity( &mat );

		mat._11 = 2.0f * (rc.Width()+1) / fScreenWidth;
		mat._22 = 1.0f;
		mat._33 = 2.0f * (rc.Height()+1) / fScreenHeight;
		mat._41 = (rc.left-0.5f) * 2.0f / fScreenWidth - 1.0f;
		mat._42 = 0.0f;
		mat._43 = -(rc.top-0.5f) * 2.0f / fScreenHeight + 1.0f;

		pDevice->SetTransform( D3DTS_WORLD, &mat );

		D3DXMatrixIdentity( &mat );
		mat._12 = 0.0f;
		mat._22 = 0.0f;
		mat._32 = 1.0f;
		mat._13 = 0.0f;
		mat._23 = 1.0f;
		mat._33 = 0.0f;
		pDevice->SetTransform( D3DTS_VIEW, &mat );

		D3DXMatrixIdentity( &mat );
		pDevice->SetTransform( D3DTS_PROJECTION, &mat );

		pDevice->SetFVF(T3DFVF_PVERTEX);
		pDevice->SetStreamSource( 0, CTClientGame::m_vTRECTVB.GetVB(), 0, sizeof(PVERTEX));

		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	}

	return TList::Render(dwTickCount);
}
// ===============================================================================

// ===============================================================================
void CTChatList::CheckDragZone()
{
	if( m_rc.left < s_rcDragZone.left )
	{
		m_rc.right = m_rc.Width();
		m_rc.left = 0;
	}
	else if( m_rc.right > s_rcDragZone.right )
	{
		m_rc.left = s_rcDragZone.right - m_rc.Width();
		m_rc.right = s_rcDragZone.right;
	}

	if( m_rc.top < s_rcDragZone.top )
	{
		m_rc.bottom = m_rc.Height();
		m_rc.top = 0;
	}
	else if( m_rc.bottom > s_rcDragZone.bottom )
	{
		m_rc.top = s_rcDragZone.bottom - m_rc.Height();
		m_rc.bottom = s_rcDragZone.bottom;
	}

	MoveComponent( CPoint( m_rc.left, m_rc.top ));
}

BOOL CTChatList::GetApplyCursor( LPTCURSOR_TYPE pbCursor )
{
	*pbCursor = m_eCursorType;
	return m_eCursorType != TCURSOR_COUNT;
}