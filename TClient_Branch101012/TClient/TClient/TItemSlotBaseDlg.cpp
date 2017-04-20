#include "stdafx.h"
#include "TItemSlotBaseDlg.h"
#include "Resource.h"
#include "TClientGame.h"

// ====================================================================================================
CTItemSlotBaseDlg::CTItemSlotBaseDlg(TComponent* pParent, LP_FRAMEDESC pDesc, int nSlotCnt)
:	CTClientUIBase(pParent, pDesc), m_nSlotCount(nSlotCnt)
{
	m_pSlotCtrl = new TImageList*[nSlotCnt];
	m_pSlotImage = new TComponent*[nSlotCnt];
	m_ItemSlot  = new TITEM_SLOT[nSlotCnt];
	m_pTitles = new CString[nSlotCnt+1];
	m_pToolTips = new CString[nSlotCnt+1];

	for(int i=0; i<nSlotCnt; ++i)
	{
		m_pSlotCtrl[i] = NULL;
		m_pSlotImage[i] = NULL;

		m_ItemSlot[i].pItem = NULL;
		m_ItemSlot[i].bInven = (BYTE)T_INVALID;
		m_ItemSlot[i].bInvenSlot = (BYTE)T_INVALID;
	}

	for( i = 0; i < nSlotCnt+1; ++i )
	{
		m_pTitles[i].Empty();
		m_pToolTips[i].Empty();
	}

	m_pButtonOK = static_cast< TButton* >( FindKid( ID_CTRLINST_OK ) );
}
// ----------------------------------------------------------------------------------------------------
CTItemSlotBaseDlg::~CTItemSlotBaseDlg()
{
	SAFE_DELETE_ARRAY(m_pSlotCtrl);
	SAFE_DELETE_ARRAY(m_pSlotImage);
	SAFE_DELETE_ARRAY(m_ItemSlot);
	SAFE_DELETE_ARRAY(m_pTitles);
	SAFE_DELETE_ARRAY(m_pToolTips);
}
// ====================================================================================================

// ====================================================================================================
BOOL CTItemSlotBaseDlg::CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const
{
	return TRUE;
}
// ----------------------------------------------------------------------------------------------------
BOOL CTItemSlotBaseDlg::CheckFrame(DWORD dwFrame) const
{
	return  (dwFrame == TFRAME_DEFINVEN) ||
			(dwFrame == TFRAME_INVEN_1) ||
			(dwFrame == TFRAME_INVEN_2) ||
			(dwFrame == TFRAME_INVEN_3) ||
			(dwFrame == TFRAME_INVEN_4) ||
			(dwFrame == TFRAME_INVEN_5) ;
}
// ====================================================================================================
BOOL CTItemSlotBaseDlg::SetItem(BYTE bSlotID, LPTITEM pItem, BYTE bInven, BYTE bInvenSlot)
{
	m_strLastError.Empty();

    CTClientItem* pClientItem;
	if( pItem )
	{
		pClientItem = CTClientGame::GetInstance()
			->FindMainItemByInven(bInven, bInvenSlot);

		if( !pClientItem )
			return FALSE;
	}

	for( BYTE i=0; i<m_nSlotCount; ++i )
	{
		if( bSlotID == i )
		{
			if( pItem == NULL )
			{
				m_ItemSlot[i].pItem = NULL;

				m_pSlotCtrl[i]->ShowComponent(FALSE);
			}
			else if( CheckItem(i, pClientItem) )
			{
				m_ItemSlot[i].bInven = bInven;
				m_ItemSlot[i].bInvenSlot = bInvenSlot;
				m_ItemSlot[i].pItem = pItem;

				LPTITEMVISUAL pTVISUAL = pClientItem->GetVisual();
				WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;

				m_pSlotCtrl[i]->SetCurImage(wImg);

				DWORD qt = 0;
				if ( pClientItem )
					qt = pClientItem->GetQuality();

				if( pClientItem && pClientItem->GetWrap() )
					m_pSlotCtrl[i]->SetSkinImage( TITEM_INDEX_WRAP_SKIN );
				else if ( pClientItem && (qt & TITEM_QUALITY_GRADE) )
					m_pSlotCtrl[i]->SetSkinImage( TITEM_INDEX_GRADE_SKIN + pClientItem->GetGrade() );
				else if ( pClientItem && (qt & TITEM_QUALITY_RARE) )
					m_pSlotCtrl[i]->SetSkinImage( TITEM_INDEX_RARE_SKIN );
				else if ( pClientItem && (qt & TITEM_QUALITY_MAGIC) )
					m_pSlotCtrl[i]->SetSkinImage( TITEM_INDEX_MAGIC_SKIN );
				else
					m_pSlotCtrl[i]->SetSkinImageEmpty();

				m_pSlotCtrl[i]->ShowComponent(TRUE);
			}
			else
				return FALSE;

			// 아이템이 모든 슬롯에 올라가면 확인 버튼 활성화..
			UpdateState_ButtonOK();

			return TRUE;
		}
	}

	return FALSE;
}
// ====================================================================================================
void CTItemSlotBaseDlg::UpdateState_ButtonOK()
{
	m_pButtonOK->EnableComponent( FALSE );
	m_pButtonOK->SetTextClr( 0xFF888888 );

	int i = 0;
	for( ; i < m_nSlotCount; ++i )
	{
		if( m_ItemSlot[i].pItem == NULL )
			return ;
	}

	m_pButtonOK->EnableComponent( TRUE );
	m_pButtonOK->SetTextClr( TCOLOR_TXT_NORMAL );
}
// ====================================================================================================
void CTItemSlotBaseDlg::ClearItem()
{
	for(int i=0; i<m_nSlotCount; ++i)
	{
		m_ItemSlot[i].pItem = NULL;
		m_ItemSlot[i].bInven = (BYTE)T_INVALID;
		m_ItemSlot[i].bInvenSlot = (BYTE)T_INVALID;

		if( m_pSlotCtrl[i] )
			m_pSlotCtrl[i]->ShowComponent(FALSE);

		m_pButtonOK->EnableComponent( FALSE );
		m_pButtonOK->SetTextClr( 0xFF888888 );
	}
}
// ====================================================================================================
CTClientItem* CTItemSlotBaseDlg::GetLastClientItem(BYTE bSlotID) const
{
	if( m_ItemSlot[bSlotID].pItem )
	{
		return CTClientGame::GetInstance()->FindMainItemByInven(
			m_ItemSlot[bSlotID].bInven, 
			m_ItemSlot[bSlotID].bInvenSlot);
	}

	return NULL;
}
// ====================================================================================================

// ====================================================================================================
void CTItemSlotBaseDlg::ShowComponent(BOOL bVisible)
{
	TComponent::ShowComponent(bVisible);

	if( bVisible )
	{
		for( BYTE i=0; i<m_nSlotCount; ++i )
			m_pSlotCtrl[i]->ShowComponent( m_ItemSlot[i].pItem != NULL );
	}
}
// ====================================================================================================
ITDetailInfoPtr CTItemSlotBaseDlg::GetTInfoKey( const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	CRect rc;
	GetComponentRect( &rc );

	size_t i = 0;
	for( ; i < m_nSlotCount; ++i )
	{
		if( m_pSlotImage[i] && 
			m_pSlotImage[i]->HitTest( point ) &&
			!m_ItemSlot[i].pItem &&
			!m_pToolTips[i].IsEmpty() )	// 아이템이 안올라갔을때 툴팁표시.
		{
			return CTDetailInfoManager::NewDefTooltipInst( m_pTitles[i], m_pToolTips[i], rc );
		}
	}

	if( !m_pToolTips[m_nSlotCount].IsEmpty() )
	{
		bool bFrame_Tooltip = true;

		for( i = 0; i < m_nSlotCount; ++i )
		{
			if( m_pSlotCtrl[i]->HitTest(point) && m_ItemSlot[i].pItem )
			{
				bFrame_Tooltip = false;
				break;
			}
		}

		if( bFrame_Tooltip ) // 아이템 & 빈 슬롯이 아닌 프레임 위에 mouse over 되었을때 Tooltip.
			return CTDetailInfoManager::NewDefTooltipInst( m_pTitles[i], m_pToolTips[m_nSlotCount], rc );
	}

	for( BYTE i=0; i<m_nSlotCount; ++i )
	{
		if( m_pSlotCtrl[i]->HitTest(point) && m_ItemSlot[i].pItem ) 
		{
			CTClientItem* pItem = GetLastClientItem(i);
			if( pItem )
			{
				pInfo = CTDetailInfoManager::NewItemInst(pItem, rc);
			}
		}
	}

	return pInfo;
}
// ----------------------------------------------------------------------------------------------------
BOOL CTItemSlotBaseDlg::GetTChatInfo( const CPoint& point, TCHATINFO& outInfo )
{
	for( BYTE i=0; i<m_nSlotCount; ++i )
	{
		if( m_pSlotCtrl[i]->HitTest(point) && m_ItemSlot[i].pItem )
		{
			CTClientItem* pItem = GetLastClientItem(i);

			if( pItem )
			{
				outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
				outInfo.m_pItem = pItem->GetTITEM();
				outInfo.m_pClientItem = pItem;
			}
			
			return TRUE;
		}
	}

	return FALSE;
}
// ====================================================================================================
TDROPINFO CTItemSlotBaseDlg::OnDrop(CPoint point)
{
	TDROPINFO vResult;

	if( HitTest(point) )
	{
		vResult.m_bDrop = TRUE;
		
		for( BYTE i=0; i<m_nSlotCount; ++i )
		{
			if( m_pSlotCtrl[i]->HitRect(point) )
			{
				vResult.m_bSlotID = (BYTE)i;
				return vResult;
			}
		}

		vResult.m_bSlotID = (BYTE)m_nSlotCount;
	}
	
	return vResult;
}
// ----------------------------------------------------------------------------------------------------
BYTE CTItemSlotBaseDlg::OnBeginDrag(LPTDRAG pDRAG, CPoint point)
{
	for( BYTE i=0; i<m_nSlotCount; ++i )
	{
		if( m_ItemSlot[i].pItem && m_pSlotCtrl[i]->HitTest(point) )
		{
			if( pDRAG )
			{
				CTClientItem* pItem = GetLastClientItem(i);
				if( pItem )
				{
					pDRAG->m_pIMAGE = new TImageList(NULL, m_pSlotCtrl[i]->m_pDESC);
					pDRAG->m_pIMAGE->m_strText = m_pSlotCtrl[i]->m_strText;
					
					LPTITEMVISUAL pTVISUAL = pItem->GetVisual();
					WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;

					pDRAG->m_pIMAGE->SetCurImage( wImg );
					pDRAG->m_pIMAGE->ShowComponent(TRUE);
					pDRAG->m_pIMAGE->MoveComponent(point);

					SetItem(i,NULL);
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}
// ====================================================================================================






















