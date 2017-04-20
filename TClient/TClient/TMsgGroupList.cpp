#include "Stdafx.h"
#include "TMsgGroupList.h"
#include "TMiniPopupDlg.h"
#include "Resource.h"

void SetSoulmate( CTMsgGroupList::CMember* m,
	DWORD dwSoulID,
	CString strSoulName,
	DWORD dwSoulRegion,
	BYTE bSoulLevel,
	BYTE bSoulClass,
	BYTE bSoulConnected );

// ======================================================================
#define LIST_DOWN_MARGIN 10
// ======================================================================
CTMsgGroupList::CTMsgGroupList(TComponent *pParent, LP_FRAMEDESC pDesc)
	: ITInnerFrame(pParent,pDesc,NULL), m_MemberTplt(0,0,0,0,0), m_GroupTplt(0,0,0,0), m_bNeedUpdate(TRUE),
	  m_nPrvScrPos(0), m_pSelectItem(NULL), m_bShutDown(FALSE), m_bShowPopup(FALSE)
{
	static const DWORD dwTXTIDs[] = 
	{
		ID_CTRLINST_TXT_NAME,	//MTXT_NAME,
		ID_CTRLINST_TXT_JOB,	//MTXT_JOB,
		ID_CTRLINST_TXT_LEVEL,	//MTXT_LEVEL
		ID_CTRLINST_TXT_STATE, 	//MTXT_STATE
		ID_CTRLINST_TXT_POS, 	//MTXT_POS,
	};

	// --------------------------------------------------------
	for(INT i=0; i<MTXT_COUNT; ++i)
	{
		m_MemberTplt.m_vTxt[i] = FindKid(dwTXTIDs[i]);
		RemoveKid(m_MemberTplt.m_vTxt[i]);
	}
	
	m_MemberTplt.m_pTopFrame[ISTATE_NORMAL] = FindKid(ID_CTRLINST_ITEM_TOP_NORMAL);
	RemoveKid(m_MemberTplt.m_pTopFrame[ISTATE_NORMAL]);
	m_MemberTplt.m_pTopFrame[ISTATE_SELECT] = FindKid(ID_CTRLINST_ITEM_TOP_SELECT);
	RemoveKid(m_MemberTplt.m_pTopFrame[ISTATE_SELECT]);

	m_MemberTplt.m_pDownFrame[ISTATE_NORMAL] = FindKid(ID_CTRLINST_ITEM_DOWN_NORMAL);
	RemoveKid(m_MemberTplt.m_pDownFrame[ISTATE_NORMAL]);
	m_MemberTplt.m_pDownFrame[ISTATE_SELECT] = FindKid(ID_CTRLINST_ITEM_DOWN_SELECT);
	RemoveKid(m_MemberTplt.m_pDownFrame[ISTATE_SELECT]);

	// --------------------------------------------------------
	m_GroupTplt.m_pStateBtn = static_cast<TButton*>( FindKid(ID_CTRLINST_BTN_GROUP) );
	RemoveKid(m_GroupTplt.m_pStateBtn);

	m_GroupTplt.m_pGroupName = FindKid(ID_CTRLINST_TXT_GROUP);
	RemoveKid(m_GroupTplt.m_pGroupName);

	m_GroupTplt.m_pFrame[ISTATE_NORMAL] = FindKid(ID_CTRLINST_BTN_NORMAL);
	RemoveKid(m_GroupTplt.m_pFrame[ISTATE_NORMAL]);
	m_GroupTplt.m_pFrame[ISTATE_SELECT] = FindKid(ID_CTRLINST_BTN_SELECT);
	RemoveKid(m_GroupTplt.m_pFrame[ISTATE_SELECT]);

	// --------------------------------------------------------
	m_pScroll = static_cast<TScroll*>( FindKid(ID_CTRLINST_SCROLL) );
	m_pScroll->SetScrollType(TRUE);
	m_pScroll->SetScrollPos(0,0);

	// --------------------------------------------------------
	CSize szItem;
	m_GroupTplt.m_pFrame[ISTATE_NORMAL]->GetComponentSize(&szItem);
	m_nItemHeight = szItem.cy;
	m_nLinePerPage = (m_rc.Height() - LIST_DOWN_MARGIN) / m_nItemHeight;

	// 소울메이트 그룹 생성
	m_pSoulmateGroup = AddGroup();
	m_pSoulmateGroup->m_dwID = 0;
	m_pSoulmateGroup->m_bSoulmate = TRUE;
	m_pSoulmateGroup->m_pGroupName->m_strText = CTChart::LoadString( TSTR_GROUP_SOULMATE );
	m_pSoulmateMember = NULL;
	m_pEtcGroup = NULL;

	GetEtcGroup();
}
// ----------------------------------------------------------------------
CTMsgGroupList::~CTMsgGroupList()
{
	m_bShutDown = TRUE;

	for(INT i=0; i<MTXT_COUNT; ++i)
		delete m_MemberTplt.m_vTxt[i];

	for(INT i=0; i<ISTATE_COUNT; ++i)
	{
		delete m_MemberTplt.m_pTopFrame[i];
		delete m_MemberTplt.m_pDownFrame[i];
		delete m_GroupTplt.m_pFrame[i];
	}

	delete m_GroupTplt.m_pStateBtn;
	delete m_GroupTplt.m_pGroupName;

	for(size_t i=0; i<m_ItemArray.size(); ++i)
		delete m_ItemArray[i];

	ZeroMemory(m_vPopup, sizeof(m_vPopup));
}
// ======================================================================

// ======================================================================
void CTMsgGroupList::InitPopup(LP_FRAMEDESC pPopupDESC)
{
	/*static const DWORD STR_NORMALPOPUP[] = 
	{
		TSTR_NPOPUP_INVITE,				// NPOPUP_INVITE,
		TSTR_MPOPUP_BLOCKLIST,			// NPOPUP_BLOCKLIST
	};*/

	static const DWORD STR_MEMBERPOPUP[] = 
	{
		//TSTR_MPOPUP_CHAT,				// MPOPUP_CHAT,
		TSTR_MENU_PARTY_REQ,				// MPOPUP_PARTY,
		TSTR_MPOPUP_SENDMAIL,			// MPOPUP_SENDMAIL,
		TSTR_MPOPUP_MOVEGROUP,			// MPOPUP_MOVEGROUP
		TSTR_MPOPUP_BLOCK,				// MPOPUP_BLOCK,
		//TSTR_MPOPUP_BLOCKLIST,			// MPOPUP_BLOCKLIST,
		TSTR_MPOPUP_DELETE,				// MPOPUP_DELETE,
		//TSTR_MPOPUP_NEWGROUP				// MPOPUP_NEWGROUP,
	};								

	static const DWORD STR_GROUPPOPUP[] = 
	{
		TSTR_GPOPUP_CHAT,				// GPOPUP_CHAT,
		TSTR_GPOPUP_CHANGENAME,			// GPOPUP_CHANGENAME,
		TSTR_GPOPUP_DELETE,				// GPOPUP_DELETE,
		TSTR_GPOPUP_NEWGROUP,			// GPOPUP_NEWGROUP,
	};

	static const DWORD GM_NORMALPOPUP[] = 
	{
		GM_ADD_FRIEND,					// NPOPUP_INVITE,
		GM_TOGGLE_BLOCKLIST,			// NPOPUP_BLOCKLIST
	};

	static const DWORD GM_MEMBERPOPUP[] = 
	{
		//GM_NEW_MESSENGER_CHAT,			// MPOPUP_CHAT,
		GM_INVITE_FRIEND_TO_PARTY,		// MPOPUP_PARTY,
		GM_SHOW_TARGETMAIL,				// MPOPUP_SENDMAIL,
		GM_SHOW_GROUPLIST_POPUP,		// MPOPUP_MOVEGROUP
		GM_ADDTOBLOCKLIST_TARGET,		// MPOPUP_BLOCK,
		//GM_TOGGLE_BLOCKLIST,			// MPOPUP_BLOCKLIST,
		GM_DELETE_FRIENDITEM,			// MPOPUP_DELETE,
		//GM_ASK_NEW_FRIEND_GROUP			// MPOPUP_NEWGROUP,
	};	

	static const DWORD GM_GROUPPOPUP[] = 
	{
		GM_NEW_MESSENGER_GROUPCHAT,		// GPOPUP_CHAT,
		GM_ASK_CHANGE_FRIEND_GROUP,		// GPOPUP_CHANGENAME,
		GM_DELETE_FRIENDITEM,			// GPOPUP_DELETE,
		GM_ASK_NEW_FRIEND_GROUP			// GPOPUP_NEWGROUP,
	};

	for(INT i=0; i<POPUPTYPE_COUNT; ++i)
	{
		m_vPopup[i] = new CTMiniPopupDlg(this, pPopupDESC, FALSE);
		m_vPopup[i]->m_id = GetUniqueID();
		
		switch(i)
		{
		case POPUPTYPE_MEMBER:
			{
				CString strTitle;
				for(INT j=0; j<MPOPUP_COUNT; ++j)
				{
					strTitle = CTChart::LoadString( (TSTRING) STR_MEMBERPOPUP[j]);
					m_vPopup[i]->AddButton(strTitle, GM_MEMBERPOPUP[j]);
				}
			}
			break;

		case POPUPTYPE_GROUP:
			{
				CString strTitle;
				for(INT j=0; j<GPOPUP_COUNT; ++j)
				{
					strTitle = CTChart::LoadString( (TSTRING) STR_GROUPPOPUP[j]);
					m_vPopup[i]->AddButton(strTitle, GM_GROUPPOPUP[j]);
				}
			}
			break;

		case POPUPTYPE_SOULMATE:
			{
				m_vPopup[i]->AddButton(
					CTChart::LoadString( TSTR_RELEASE_SOULMATE ),
					GM_SOULMATE_END );
			}
			break;
		}
		
		AddKid(m_vPopup[i]);
		m_vPopup[i]->ShowComponent(FALSE);
	}

	m_bShowPopup = FALSE;
}
// ======================================================================
void CTMsgGroupList::HideAllPopup()
{
	for(INT i=0; i<POPUPTYPE_COUNT; ++i)
		m_vPopup[i]->ShowComponent(FALSE);

	m_bShowPopup = FALSE;
}
// ----------------------------------------------------------------------
void CTMsgGroupList::ShowPopup(PopupType type, const CPoint* pt)
{
	if( type == POPUPTYPE_GROUPLIST )
	{
		m_vPopup[type]->ClearButtons();

		UINT nItemCnt = GetItemCount();
		for(UINT i=0; i<nItemCnt; ++i)
		{
			CItem* pItem = GetItem(i);
			if( pItem->GetType() == ITYPE_GROUP &&
				pItem->m_bSoulmate == FALSE &&
				pItem->m_bEtcGroup == FALSE )
			{
				m_vPopup[type]->AddButton(
					pItem->GetNameComp()->m_strText,
					GM_MOVE_FRIEND_GROUP );
			}
		}

		if( !pt )
		{
			CRect rcMemPopup;
			m_vPopup[POPUPTYPE_MEMBER]->GetComponentRect(&rcMemPopup);
			
			INT y = rcMemPopup.top + m_vPopup[POPUPTYPE_MEMBER]->GetUpperHeight() + 
				m_vPopup[POPUPTYPE_MEMBER]->GetButtonHeight()*MPOPUP_MOVEGROUP;
			INT x = rcMemPopup.right;

			m_vPopup[type]->MoveComponent(CPoint(x,y));
		}
	}

	m_vPopup[type]->ShowComponent(TRUE);

	RemoveKid(m_vPopup[type]);
	AddKid(m_vPopup[type]);

	if( pt )
	{
		CPoint ptComp = *pt;
		ScreenToComponent(&ptComp);

		ptComp.x -= m_rc.left;
		ptComp.y -= m_rc.top;

		m_vPopup[type]->MoveComponent(ptComp);
	}

	m_bShowPopup = TRUE;
}
// ======================================================================

// ======================================================================
CTMsgGroupList::CGroup* CTMsgGroupList::AddGroup()
{
	INT nCnt = (INT)m_ItemArray.size();

	INT nLastLine = 0;
	if( nCnt )
	{
		CItem* pItem = m_ItemArray.back();
		nLastLine = pItem->GetLastLine()+1;
	}

	CGroup* pGrp = new CGroup(this,nCnt,nLastLine,&m_GroupTplt);
	m_ItemArray.push_back(pGrp);

	NotifyUpdate();

	return pGrp;
}
// ----------------------------------------------------------------------
CTMsgGroupList::CMember* CTMsgGroupList::AddMember(CGroup* pParent)
{
	CItemArray* pItemArray;
	if( pParent )
		pItemArray = &(pParent->m_ItemArray);
	else
		pItemArray = &m_ItemArray;

    INT nCnt = (INT)pItemArray->size();

	INT nLastLine = 0;
	if( nCnt )
	{
		CItem* pItem = pItemArray->back();
		nLastLine = pItem->GetLastLine()+1;
	}
	else if( pParent )
		nLastLine = pParent->GetLastLine()+1;

	CMember* pMember = new CMember(this,pParent,nCnt,nLastLine,&m_MemberTplt);
	pItemArray->push_back(pMember);
	
	if( pParent && pParent->m_pStateBtn->IsStateDown() )
	{
		CItem* pNext = NextItem(pParent);
		if( pNext )
			MoveLine(pMember->GetLineHeight(), pNext);
	}

	NotifyUpdate();

	return pMember;
}
// ======================================================================
CTMsgGroupList::CGroup* CTMsgGroupList::InsertGroup(INT nIdx)
{
	CGroup* pGrp;
	if( !m_ItemArray.empty() )
	{
		CItemArray::iterator itr = m_ItemArray.begin() + nIdx;
		CItem* pInsItem = *itr;

		pGrp = new CGroup(this,nIdx,pInsItem->m_nLine,&m_GroupTplt);
		
		MoveLine(pGrp->GetLineHeight(), pInsItem);
		MoveIndex(1, pInsItem);
		
		m_ItemArray.insert(itr, pGrp);
	}
	else
	{
		pGrp = new CGroup(this,0,0,&m_GroupTplt);
		m_ItemArray.push_back(pGrp);
	}

	NotifyUpdate();

	return pGrp;
}
// ----------------------------------------------------------------------
CTMsgGroupList::CMember* CTMsgGroupList::InsertMember(INT nIdx, CGroup* pParent)
{
	CItemArray* pItemArray;
	if( pParent )
		pItemArray = &(pParent->m_ItemArray);
	else
		pItemArray = &m_ItemArray;

	CMember* pMember;
	if( !pItemArray->empty() )
	{
		CItemArray::iterator itr = pItemArray->begin() + nIdx;
		CItem* pInsItem = *itr;

		pMember = new CMember(this,pParent,nIdx,pInsItem->m_nLine,&m_MemberTplt);
		
		if( !pParent || pParent->m_pStateBtn->IsStateDown() )
			MoveLine(pMember->GetLineHeight(), pInsItem);

		MoveIndex(1,pInsItem);
		pItemArray->insert(itr, pMember);
	}
	else
	{
		pMember = new CMember(this,pParent,0,0,&m_MemberTplt);

		if( pParent && pParent->m_pStateBtn->IsStateDown() )
		{
			pMember->m_nLine = pParent->GetLastLine() + pParent->GetLineHeight();

			CItem* pNext = NextItem(pParent);
			if( pNext )
				MoveLine(pMember->GetLineHeight(),pNext);
		}

		pItemArray->push_back(pMember);
	}

	NotifyUpdate();

	return pMember;
}	
// ======================================================================
UINT CTMsgGroupList::GetItemCount(CGroup* pParent) const
{
	const CItemArray* pItemArray;
	if( pParent )
		pItemArray = &(pParent->m_ItemArray);
	else
		pItemArray = &m_ItemArray;

	return (UINT)(pItemArray->size());
}
// ======================================================================
CTMsgGroupList::CItem* CTMsgGroupList::GetItem(INT nIdx, CGroup* pParent) const
{
	const CItemArray* pItemArray;
	if( pParent )
		pItemArray = &(pParent->m_ItemArray);
	else
		pItemArray = &m_ItemArray;

	return pItemArray->at(nIdx);
}
// ----------------------------------------------------------------------
CTMsgGroupList::CItem* CTMsgGroupList::NextItem(CItem* pItem, CGroup* pParent) const
{
	const CItemArray* pItemArray;
	if( pParent )
		pItemArray = &(pParent->m_ItemArray);
	else
		pItemArray = &m_ItemArray;

	INT nNextIdx = pItem->m_nIndex+1;
	if( nNextIdx < (INT)pItemArray->size() )
		return pItemArray->at(nNextIdx);

	return NULL;
}
// ----------------------------------------------------------------------
CTMsgGroupList::CItem* CTMsgGroupList::PrevItem(CItem* pItem, CGroup* pParent) const
{
	const CItemArray* pItemArray;
	if( pParent )
		pItemArray = &(pParent->m_ItemArray);
	else
		pItemArray = &m_ItemArray;

	INT nPrevIdx = pItem->m_nIndex-1;
	if( nPrevIdx >= 0 )
		return pItemArray->at(nPrevIdx);

	return NULL;
}
// ----------------------------------------------------------------------
CTMsgGroupList::CItem* CTMsgGroupList::GetLastItem(CGroup* pParent) const
{
	const CItemArray* pItemArray;
	if( pParent )
		pItemArray = &(pParent->m_ItemArray);
	else
		pItemArray = &m_ItemArray;

	if( pItemArray->empty() )
		return NULL;

	return pItemArray->back();
}
// ----------------------------------------------------------------------
CTMsgGroupList::CItem* CTMsgGroupList::GetHitItem(const CPoint& pt)
{
	for(size_t i=0; i<m_ItemArray.size(); ++i)
	{
		CItem* pItem = m_ItemArray[i]->GetHitItem(pt);
		if( pItem )
			return pItem;
	}

	return NULL;
}
// ======================================================================
CTMsgGroupList::CMember* CTMsgGroupList::FindMemberByName(const CString& strName)
{
	for(size_t i=0; i<m_ItemArray.size(); ++i)
	{
		CItem* pItem = m_ItemArray[i];
		if( pItem->GetType() == ITYPE_GROUP )
		{
			CMember* pMember = static_cast<CGroup*>(pItem)->FindMemberByName(strName);
			if( pMember )
				return pMember;
		}
	}

	return NULL;
}
// ----------------------------------------------------------------------
CTMsgGroupList::CGroup* CTMsgGroupList::FindGroupByName(const CString& strName)
{
	for(size_t i=0; i<m_ItemArray.size(); ++i)
	{
		CItem* pItem = m_ItemArray[i];
		if( pItem->GetType() == ITYPE_GROUP &&
			pItem->m_bSoulmate == FALSE &&
			pItem->m_bEtcGroup == FALSE )
		{
			if( pItem->GetNameComp()->m_strText == strName )
				return static_cast<CGroup*>(pItem);
		}
	}

	return NULL;
}
// ----------------------------------------------------------------------
CTMsgGroupList::CMember* CTMsgGroupList::FindMemberByID(DWORD dwID)
{
	for(size_t i=0; i<m_ItemArray.size(); ++i)
	{
		CItem* pItem = m_ItemArray[i];
		if( pItem->GetType() == ITYPE_GROUP )
		{
			CMember* pMember = static_cast<CGroup*>(pItem)->FindMemberByID(dwID);
			if( pMember )
				return pMember;
		}
	}

	return NULL;
}
// ----------------------------------------------------------------------
CTMsgGroupList::CGroup* CTMsgGroupList::FindGroupByID(DWORD dwID)
{
	for(size_t i=0; i<m_ItemArray.size(); ++i)
	{
		CItem* pItem = m_ItemArray[i];
		if( pItem->GetType() == ITYPE_GROUP &&
			pItem->m_bSoulmate == FALSE &&
			pItem->m_bEtcGroup == FALSE )
		{
			if( pItem->m_dwID == dwID )
				return static_cast<CGroup*>(pItem);
		}
	}

	return NULL;
}
// ======================================================================
void CTMsgGroupList::Clear()
{
	CItemArray::iterator it = m_ItemArray.begin();
	while(  it != m_ItemArray.end() )
	{
		if( (*it)->m_bSoulmate )
		{
			++it;
			continue;
		}

		delete (*it);
		it = m_ItemArray.erase( it );
	}

	m_pSelectItem = NULL;
	m_pEtcGroup = NULL;
	NotifyUpdate();
}
// ----------------------------------------------------------------------
void CTMsgGroupList::RemoveItem(INT nIdx, CGroup* pParent)
{
	CItemArray* pItemArray;
	if( pParent )
		pItemArray = &(pParent->m_ItemArray);
	else
		pItemArray = &m_ItemArray;

	CItemArray::iterator itr = pItemArray->begin() + nIdx; 
	CItem* pDelItem = (*itr);

	if( !pParent || pParent->m_pStateBtn->IsStateDown() )
	{
		INT nMove;
		if( pDelItem->GetType() == ITYPE_GROUP )
		{
			CGroup* pDelGrp = static_cast<CGroup*>(pDelItem);
			nMove = pDelGrp->GetLastLine() - pDelGrp->m_nLine + pDelGrp->GetLineHeight();
		}
		else
			nMove = pDelItem->GetLineHeight();

		MoveLine(-nMove, pDelItem);
	}

	if( pItemArray->size() > 1 )
		MoveIndex(-1, pDelItem);

	if( m_pSelectItem == pDelItem )
		m_pSelectItem = NULL;

	delete pDelItem;
	pItemArray->erase(itr);

	NotifyUpdate();
}
// ======================================================================
void CTMsgGroupList::ChangerGroup(CMember* pMember, CGroup* pGroup)
{
	CItemArray* pItemArray;

	CGroup* pParentGrp = pMember->m_pParentGrp;
	if( pParentGrp )
		pItemArray = &(pParentGrp->m_ItemArray);
	else
		pItemArray = &m_ItemArray;

	CItemArray::iterator itr = pItemArray->begin() + pMember->m_nIndex; 

	if( !pParentGrp || pParentGrp->m_pStateBtn->IsStateDown() )
	{
		INT nMove = pMember->GetLineHeight();
		MoveLine(-nMove, pMember);
	}

	if( pItemArray->size() > 1 )
		MoveIndex(-1, pMember);
	
	pItemArray->erase(itr);

	if( pGroup )
		pItemArray = &(pGroup->m_ItemArray);
	else
		pItemArray = &m_ItemArray;

	pMember->m_nIndex = 0;
	pMember->m_nLine = 0;
	pMember->m_pParentGrp = pGroup;

	if( !pItemArray->empty() )
	{
		itr = pItemArray->begin();
		CItem* pInsItem = *itr;

		pMember->m_nLine = pInsItem->m_nLine;

		if( !pGroup || pGroup->m_pStateBtn->IsStateDown() )
			MoveLine(pMember->GetLineHeight(), pInsItem);

		MoveIndex(1,pInsItem);
		pItemArray->insert(itr, pMember);
	}
	else
	{
		if( pGroup && pGroup->m_pStateBtn->IsStateDown() )
		{
			pMember->m_nLine = pGroup->GetLastLine() + pGroup->GetLineHeight();

			CItem* pNext = NextItem(pGroup);
			if( pNext )
				MoveLine(pMember->GetLineHeight(),pNext);
		}
		
		pItemArray->push_back(pMember);
	}

	NotifyUpdate();
}
// ======================================================================
void CTMsgGroupList::MoveIndex(INT nMove, CItem* pStartItem)
{
	if( m_ItemArray.empty() )
		return;

	if( !pStartItem )
		pStartItem = m_ItemArray.front();

	CItemArray* pItemArray = &m_ItemArray;
	if( pStartItem->GetType() == ITYPE_MEMBER )
	{
		CGroup* pParentGrp = static_cast<CMember*>(pStartItem)->m_pParentGrp;
		if( pParentGrp )
			pItemArray = &(pParentGrp->m_ItemArray);
	}

	for(size_t i=pStartItem->m_nIndex; i<pItemArray->size(); ++i)
	{
		CItem* pItem = pItemArray->at(i);
		pItem->m_nIndex += nMove;
	}

	NotifyUpdate();
}
// ----------------------------------------------------------------------
void CTMsgGroupList::MoveLine(INT nMove, CItem* pStartItem)
{
	if( m_ItemArray.empty() )
		return;

	if( !pStartItem )
		pStartItem = m_ItemArray.front();

	pStartItem->MoveLine(nMove);

	INT nIdx = pStartItem->m_nIndex+1;
	if( pStartItem->GetType() == ITYPE_MEMBER )
	{
		CGroup* pParentGrp = static_cast<CMember*>(pStartItem)->m_pParentGrp;
		if( pParentGrp )
		{
			size_t nCnt = pParentGrp->m_ItemArray.size();
			for(size_t i=pStartItem->m_nIndex+1; i<nCnt; ++i)
				pParentGrp->m_ItemArray[i]->MoveLine(nMove);

			nIdx = pParentGrp->m_nIndex+1;
		}
	}

	INT nCnt = (INT)m_ItemArray.size();
	while( nIdx < nCnt ) 
		m_ItemArray[nIdx++]->MoveLine(nMove);

	NotifyUpdate();
}
// ======================================================================
void CTMsgGroupList::NotifyUpdate()
{
	m_bNeedUpdate = TRUE;
}
// ======================================================================

// ======================================================================
void CTMsgGroupList::SwitchFocus(TComponent* pCandidate)
{
	if( m_pFocus == pCandidate )
		return;

	if(m_pFocus)
	{
		m_pFocus->SetFocus(FALSE);
		m_pFocus->OnLoseFocus(pCandidate);
	}

	if(pCandidate)
	{
		pCandidate->SetFocus(TRUE);
		pCandidate->OnSetFocus(m_pFocus);
	}

	m_pFocus = pCandidate;
}
// ======================================================================
void CTMsgGroupList::ShowComponent(BOOL bVisible)
{
	if( m_bVisible != bVisible )
	{
		if( bVisible )
		{
			m_bVisible = TRUE;
			m_pScroll->ShowComponent(TRUE);
				
			NotifyUpdate();
			Update();
		}
		else
			ITInnerFrame::ShowComponent(FALSE);
	}
}
// ======================================================================
HRESULT CTMsgGroupList::Render(DWORD dwTickCount)
{
	INT nScrRange, nScrPos;
	m_pScroll->GetScrollPos(nScrRange,nScrPos);
	
	if( m_nPrvScrPos != nScrPos )
	{
		m_nPrvScrPos = nScrPos;
		NotifyUpdate();
	}

	Update();

	CItemArray::iterator itr,end;
	itr = m_ItemArray.begin();
	end = m_ItemArray.end();

	for(; itr!=end; ++itr)
		(*itr)->FrameUpdate( dwTickCount );

	return ITInnerFrame::Render(dwTickCount);
}
// ======================================================================

// ======================================================================
void CTMsgGroupList::Update()
{
	if( !m_bNeedUpdate )
		return;

	m_bNeedUpdate = FALSE;

	INT nCnt = (INT)m_ItemArray.size();

	INT nLineCnt = 0;
	if( nCnt )
	{
		CItem* pItem = m_ItemArray.back();
		nLineCnt = pItem->GetLastLine();
	}

	INT nScrRange, nLineStartPos;
	m_pScroll->GetScrollPos(nScrRange,nLineStartPos);
	if( nLineCnt >= m_nLinePerPage )
	{
		nScrRange = nLineCnt - m_nLinePerPage + 1;
		if( nLineStartPos > nScrRange )
			nLineStartPos = nScrRange;

		m_nPrvScrPos = nLineStartPos;
		m_pScroll->SetScrollPos(nScrRange,nLineStartPos);
	}
	else
		m_pScroll->SetScrollPos(0,0);

	INT nLineEndPos = nLineStartPos+m_nLinePerPage-1;

	CItemArray::iterator itr,end;
	itr = m_ItemArray.begin();
	end = m_ItemArray.end();

	for(; itr!=end; ++itr)
		(*itr)->Update(nLineStartPos, nLineEndPos);
}
// ======================================================================
void CTMsgGroupList::OnChar(UINT nChar, int nRepCnt, UINT nFlags)
{
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnKeyUp(UINT nChar, int nRepCnt, UINT nFlags)
{
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnMouseMove(UINT nFlags, CPoint pt)
{
	if( CanProcess() )
	{
		m_pScroll->OnMouseMove(nFlags,pt);

		for(INT i=0; i<POPUPTYPE_COUNT; ++i)
		{
			if( m_vPopup[i]->IsVisible() )
				m_vPopup[i]->OnMouseMove(nFlags,pt);
		}
	}
}
// ----------------------------------------------------------------------
BOOL CTMsgGroupList::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if(HitTest(pt))
	{
		INT nRng,nPos;
		m_pScroll->GetScrollPos(nRng,nPos);

		if( zDelta > 0 )
			nPos = min(nPos+1, nRng);
		else
			nPos = max(nPos-1, 0);

		m_pScroll->SetScrollPos(nRng,nPos);

		return TRUE;
	}

	return FALSE;
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if( !CanProcess() )
		return;

	for(INT i=0; i<POPUPTYPE_COUNT; ++i)
		m_vPopup[i]->OnLButtonUp(nFlags,pt);

	m_pScroll->OnLButtonUp(nFlags,pt);
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnMButtonUp(UINT nFlags, CPoint pt)
{
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnRButtonUp(UINT nFlags, CPoint pt)
{
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if( !CanProcess() )
		return;

	BOOL bHitPopup = FALSE;
	for(INT i=0; i<POPUPTYPE_COUNT; ++i)
	{
		if( m_vPopup[i]->IsVisible() && m_vPopup[i]->HitTest(pt) )
		{
			m_vPopup[i]->OnLButtonDown(nFlags,pt);
			bHitPopup = TRUE;
			break;
		}
	}

	if( !bHitPopup )
	{
		HideAllPopup();
	
		if( m_pScroll->HitTest(pt) )
			m_pScroll->OnLButtonDown(nFlags,pt);
		else
		{
			for(size_t i=0; i<m_ItemArray.size(); ++i)
				m_ItemArray[i]->OnLButtonDown(pt);
		}
	}
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnMButtonDown(UINT nFlags, CPoint pt)
{
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnRButtonDown(UINT nFlags, CPoint pt)
{
	if( !CanProcess() )
		return;

	HideAllPopup();

	for(size_t i=0; i<m_ItemArray.size(); ++i)
		m_ItemArray[i]->OnRButtonDown(pt);
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnLButtonDblClk(UINT nFlags, CPoint pt)
{
	if( !CanProcess() )
		return;

	if( m_pSelectItem &&
		m_pSelectItem->GetType() == ITYPE_GROUP )
	{
		CGroup* pSelGrp = static_cast<CGroup*>(m_pSelectItem);
		if( !pSelGrp->m_pStateBtn->HitTest(pt) )
			pSelGrp->ToggleSpread();
	}
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnMButtonDblClk(UINT nFlags, CPoint pt)
{
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnRButtonDblClk(UINT nFlags, CPoint pt)
{
}
// ----------------------------------------------------------------------
void CTMsgGroupList::OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
}
// ======================================================================


// CItem
// ======================================================================
CTMsgGroupList::CItem::CItem(CTMsgGroupList* pParent, INT nIdx, INT nLine)
: m_pParent(pParent),
m_nIndex(nIdx),
m_nLine(nLine),
m_bSelect(FALSE),
m_dwID(0),
m_bSoulmate(FALSE),
m_bEtcGroup(FALSE)
{
}
// ----------------------------------------------------------------------
CTMsgGroupList::CItem::~CItem()
{
}
// ======================================================================
BOOL CTMsgGroupList::CItem::IsVisible() const
{
	INT nRng, nPos;
	TScroll* pScroll = m_pParent->GetScroll();
	pScroll->GetScrollPos(nRng,nPos);

	INT nStart = nPos;
	INT nEnd = nPos + m_pParent->GetItemHeight() - 1;

	if( m_nLine > nEnd )
		return FALSE;

	INT nLastLine = GetLastLine();
	if( nLastLine < nStart )
		return FALSE;

	return TRUE;
}
// ======================================================================




// CGroup
// ======================================================================
CTMsgGroupList::CGroup::CGroup(CTMsgGroupList* pParent, INT nIdx, INT nLine, CGroup* pTplt)
	: CItem(pParent,nIdx,nLine)
{
	if( pTplt )
	{
		for(INT i=0; i<ISTATE_COUNT; ++i)
		{
			m_pFrame[i] = new TComponent(pParent, pTplt->m_pFrame[i]->m_pDESC);
			m_pFrame[i]->m_id = pParent->GetUniqueID();
			pParent->AddKid(m_pFrame[i]);
			m_pFrame[i]->ShowComponent(FALSE);
		}

		m_pStateBtn = new TButton(pParent, pTplt->m_pStateBtn->m_pDESC);
		m_pStateBtn->m_id = pParent->GetUniqueID();
		m_pStateBtn->Select(FALSE);
		pParent->AddKid(m_pStateBtn);
		m_pStateBtn->ShowComponent(FALSE);

		m_pGroupName = new TComponent(pParent, pTplt->m_pGroupName->m_pDESC);
		m_pGroupName->m_id = pParent->GetUniqueID();
		pParent->AddKid(m_pGroupName);
		m_pGroupName->ShowComponent(FALSE);
		m_pGroupName->m_strText = "";
	}

	m_pTplt = pTplt;
}
// ----------------------------------------------------------------------
CTMsgGroupList::CGroup::~CGroup()
{
	for(size_t i=0; i<m_ItemArray.size(); ++i)
		delete m_ItemArray[i];

	if( m_pTplt && !m_pParent->IsShutDown() )
	{
		for(INT i=0; i<ISTATE_COUNT; ++i)
		{
			m_pParent->RemoveKid(m_pFrame[i]);
			delete m_pFrame[i];
		}

		m_pParent->RemoveKid(m_pStateBtn);
		delete m_pStateBtn;

		m_pParent->RemoveKid(m_pGroupName);
		delete m_pGroupName;
	}
}
// ======================================================================
void CTMsgGroupList::CGroup::Show(BOOL bShow)
{
	for(INT i=0; i<ISTATE_COUNT; ++i)
	{
		BOOL bShowFrame = bShow;
		if( bShowFrame )
			bShowFrame = m_bSelect? (i==ISTATE_SELECT): (i==ISTATE_NORMAL);
		
		m_pFrame[i]->ShowComponent(bShowFrame);
	}

	m_pStateBtn->ShowComponent(bShow);
	m_pGroupName->ShowComponent(bShow);
}
// ----------------------------------------------------------------------
INT CTMsgGroupList::CGroup::GetLastLine() const
{
	if( m_pStateBtn->IsStateDown() && !m_ItemArray.empty() )
	{
		CItem* pItem = m_ItemArray.back();
		return pItem->GetLastLine();
	}

	return m_nLine;
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CGroup::Update(INT nShowRangeStart, INT nShowRangeEnd)
{
	if( m_nLine > nShowRangeEnd )
	{
		Show(FALSE);
		HideAllChildren();
		
		return;
	}

	INT nLastLine = GetLastLine();
	if( nLastLine < nShowRangeStart )
	{
		Show(FALSE);
		HideAllChildren();
		
		return;
	}

	if( m_nLine >= nShowRangeStart )
	{
		Show(TRUE);
		SetPos(m_nLine-nShowRangeStart);
	}

	for(size_t i=0; i<m_ItemArray.size(); ++i)
	{
		if( m_pStateBtn->IsStateDown() )
			m_ItemArray[i]->Update(nShowRangeStart,nShowRangeEnd);
		else
			m_ItemArray[i]->Show(FALSE);
	}	
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CGroup::SetPos(INT nOffset)
{
	if(!m_pTplt)
		return;

	INT nItemH = m_pParent->GetItemHeight();

	CPoint ptBase;
	for(INT i=0; i<ISTATE_COUNT; ++i)
	{
		m_pTplt->m_pFrame[i]->GetComponentPos(&ptBase);
		ptBase.y += nItemH * nOffset;
		m_pFrame[i]->MoveComponent(ptBase);
	}

	m_pTplt->m_pStateBtn->GetComponentPos(&ptBase);
	ptBase.y += nItemH * nOffset;
	m_pStateBtn->MoveComponent(ptBase);

	m_pTplt->m_pGroupName->GetComponentPos(&ptBase);
	ptBase.y += nItemH * nOffset;
	m_pGroupName->MoveComponent(ptBase);
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CGroup::MoveLine(INT nMove)
{
	m_nLine += nMove;
	
	if( m_pStateBtn->IsStateDown() )
	{
		for(size_t i=0; i<m_ItemArray.size(); ++i)
			m_ItemArray[i]->MoveLine(nMove);
	}
}
// ----------------------------------------------------------------------
BOOL CTMsgGroupList::CGroup::HitTest(const CPoint& pt)
{
	TComponent* pFrame = m_bSelect? m_pFrame[ISTATE_SELECT]: m_pFrame[ISTATE_NORMAL];
	return pFrame->HitTest(pt);
}
// ----------------------------------------------------------------------
CTMsgGroupList::CItem* CTMsgGroupList::CGroup::GetHitItem(const CPoint& pt)
{
	if( HitTest(pt) )
		return this;
	else if( m_pStateBtn->IsStateDown() )
	{
		for(size_t i=0; i<m_ItemArray.size(); ++i)
		{
			CItem* pItem = m_ItemArray[i];
			if( pItem->HitTest(pt) )
				return pItem;
		}
	}
	
	return NULL;
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CGroup::Select(BOOL bSelect)
{
	if( m_bSelect != bSelect )
	{
		m_bSelect = bSelect;

		if( bSelect )
			m_pParent->SetSelectItem(this);

		Show( IsVisible() );
	}
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CGroup::OnLButtonDown(const CPoint& pt)
{
	BOOL bHit = HitTest(pt);
	Select(bHit);

	if(bHit && m_pStateBtn->HitTest(pt))
	{
		ToggleSpread();
	}
	
	if(m_pStateBtn->IsStateDown())
	{
		for(size_t i=0; i<m_ItemArray.size(); ++i)
			m_ItemArray[i]->OnLButtonDown(pt);
	}
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CGroup::OnRButtonDown(const CPoint& pt)
{
	BOOL bHit = HitTest(pt);
	Select(bHit);

	if( bHit &&
		!m_bSoulmate &&
		!m_bEtcGroup )
	{
		m_pParent->ShowPopup(POPUPTYPE_GROUP, &pt);
	}

	if(m_pStateBtn->IsStateDown())
	{
		for(size_t i=0; i<m_ItemArray.size(); ++i)
			m_ItemArray[i]->OnRButtonDown(pt);
	}
}
// ======================================================================
void CTMsgGroupList::CGroup::FrameUpdate( DWORD dwTickTime )
{
	CItemArray::iterator it, end;
	it = m_ItemArray.begin();
	end = m_ItemArray.end();

	for(; it != end ; ++it )
		(*it)->FrameUpdate( dwTickTime );
}
// ======================================================================
void CTMsgGroupList::CGroup::HideAllChildren()
{
	for(size_t i=0; i<m_ItemArray.size(); ++i)
		m_ItemArray[i]->Show(FALSE);
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CGroup::Spread( BOOL bSpread )
{
	if( m_pStateBtn->IsStateDown() == TRUE )
		return ;

	m_pStateBtn->Select(bSpread);

	if( bSpread )
	{
		INT nCurLine = m_nLine + GetLineHeight();
		for(size_t i=0; i<m_ItemArray.size(); ++i)
		{
			m_ItemArray[i]->m_nLine = nCurLine;
			nCurLine += m_ItemArray[i]->GetLineHeight();
		}
	}

	INT nMov = 0;
	if( !m_ItemArray.empty() )
	{
		CItem* pItem = m_ItemArray.back();
		nMov = pItem->GetLastLine() - m_nLine;
	}

	if( nMov )
	{
		if( !bSpread )	
			nMov = -nMov;

		CItem* pNext = m_pParent->NextItem(this);
		if( pNext )
			m_pParent->MoveLine(nMov, pNext);
	}

	m_pParent->NotifyUpdate();
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CGroup::ToggleSpread()
{
	BOOL bSpread = !(m_pStateBtn->IsStateDown());
	m_pStateBtn->Select(bSpread);

	if( bSpread )
	{
		INT nCurLine = m_nLine + GetLineHeight();
		for(size_t i=0; i<m_ItemArray.size(); ++i)
		{
			m_ItemArray[i]->m_nLine = nCurLine;
			nCurLine += m_ItemArray[i]->GetLineHeight();
		}
	}

	INT nMov = 0;
	if( !m_ItemArray.empty() )
	{
		CItem* pItem = m_ItemArray.back();
		nMov = pItem->GetLastLine() - m_nLine;
	}

	if( nMov )
	{
		if( !bSpread )	
			nMov = -nMov;

		CItem* pNext = m_pParent->NextItem(this);
		if( pNext )
			m_pParent->MoveLine(nMov, pNext);
	}

	m_pParent->NotifyUpdate();
}
// ----------------------------------------------------------------------
CTMsgGroupList::CMember* CTMsgGroupList::CGroup::FindMemberByName(const CString& strName)
{
	for(size_t i=0; i<m_ItemArray.size(); ++i)
	{
		if( m_ItemArray[i]->GetNameComp()->m_strText == strName )
			return static_cast<CMember*>(m_ItemArray[i]);
	}

	return NULL;
}
// ----------------------------------------------------------------------
CTMsgGroupList::CMember* CTMsgGroupList::CGroup::FindMemberByID(DWORD dwID)
{
	for(size_t i=0; i<m_ItemArray.size(); ++i)
	{
		if( m_ItemArray[i]->m_dwID == dwID )
			return static_cast<CMember*>(m_ItemArray[i]);
	}

	return NULL;
}
// ======================================================================




// CMember
// ======================================================================
CTMsgGroupList::CMember::CMember(CTMsgGroupList* pParent, CGroup* pParentGrp, INT nIdx, INT nLine, CMember* pTplt)
	: CItem(pParent,nIdx,nLine), m_pParentGrp(pParentGrp)
{
	m_bTwinkle = FALSE;
	m_dwTwinkleAccTime = 0;
	m_bShowTop = FALSE;
	m_bShowDown = FALSE;

	if( pTplt )
	{
		for(INT i=0; i<ISTATE_COUNT; ++i)
		{
			m_pTopFrame[i]  = new TComponent(pParent, pTplt->m_pTopFrame[i]->m_pDESC);
			m_pDownFrame[i] = new TComponent(pParent, pTplt->m_pDownFrame[i]->m_pDESC);

			m_pTopFrame[i]->m_id = pParent->GetUniqueID();
			m_pDownFrame[i]->m_id = pParent->GetUniqueID();

			pParent->AddKid(m_pTopFrame[i]);
			pParent->AddKid(m_pDownFrame[i]);

			m_pTopFrame[i]->ShowComponent(FALSE);
			m_pDownFrame[i]->ShowComponent(FALSE);

			m_pTopTplt[ i ] = m_pTopFrame[ i ];
			m_pDownTplt[ i ] = m_pDownFrame[ i ];
		}

		for(INT i=0; i<MTXT_COUNT; ++i)
		{
			m_vTxt[i] = new TComponent(pParent, pTplt->m_vTxt[i]->m_pDESC);
			m_vTxt[i]->m_id = pParent->GetUniqueID();
			pParent->AddKid(m_vTxt[i]);
			m_vTxt[i]->ShowComponent(FALSE);
			m_vTxt[i]->m_strText = "";
		}
	}

	m_pTplt = pTplt;
}
// ----------------------------------------------------------------------
CTMsgGroupList::CMember::~CMember()
{
	if( m_pTplt && !m_pParent->IsShutDown() )
	{
		for(INT i=0; i<ISTATE_COUNT; ++i)
		{
			m_pParent->RemoveKid(m_pTopFrame[i]);
			m_pParent->RemoveKid(m_pDownFrame[i]);

			delete m_pTopFrame[i];
			delete m_pDownFrame[i];
		}

		for(INT i=0; i<MTXT_COUNT; ++i)
		{
			m_pParent->RemoveKid(m_vTxt[i]);
			delete m_vTxt[i];
		}
	}
}
// ======================================================================
void CTMsgGroupList::CMember::Show(BOOL bShow)
{
	ShowTop(bShow);
	ShowDown(bShow);
}
// ----------------------------------------------------------------------
INT CTMsgGroupList::CMember::GetLastLine() const
{
	return m_nLine+1;
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CMember::Update(INT nShowRangeStart, INT nShowRangeEnd)
{
	if( m_nLine > nShowRangeEnd )
	{
		Show(FALSE);
		return;
	}

	INT nLastLine = GetLastLine();
	if( nLastLine < nShowRangeStart )
	{
		Show(FALSE);
		return;
	}

	ShowTop(m_nLine >= nShowRangeStart);
	ShowDown(nLastLine <= nShowRangeEnd);

	SetPos(m_nLine-nShowRangeStart);
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CMember::SetPos(INT nOffset)
{
	if(!m_pTplt)
		return;

	INT nItemH = m_pParent->GetItemHeight();

	CPoint ptBase;
	for(INT i=0; i<ISTATE_COUNT; ++i)
	{
		m_pTplt->m_pTopFrame[i]->GetComponentPos(&ptBase);
		ptBase.y += nItemH * nOffset;
		m_pTopFrame[i]->MoveComponent(ptBase);
		
		m_pTplt->m_pDownFrame[i]->GetComponentPos(&ptBase);
		ptBase.y += nItemH * (nOffset+1);
		m_pDownFrame[i]->MoveComponent(ptBase);
	}

	for(INT i=0; i<MTXT_COUNT; ++i)
	{
		m_pTplt->m_vTxt[i]->GetComponentPos(&ptBase);
		ptBase.y += nItemH * nOffset + nItemH;
		if( i == MTXT_NAME )
		{
			ptBase.y -= nItemH;
		}

		m_vTxt[i]->MoveComponent(ptBase);
	}
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CMember::MoveLine(INT nMove)
{
	m_nLine += nMove;
}
// ----------------------------------------------------------------------
BOOL CTMsgGroupList::CMember::HitTest(const CPoint& pt)
{
	TComponent* pFrame;
	
	pFrame = m_bSelect? m_pTopFrame[ISTATE_SELECT]: m_pTopFrame[ISTATE_NORMAL];
	if( pFrame->HitTest(pt) )
		return TRUE;

	pFrame = m_bSelect? m_pDownFrame[ISTATE_SELECT]: m_pDownFrame[ISTATE_NORMAL];
	if( pFrame->HitTest(pt) )
		return TRUE;

	return FALSE;
}
// ----------------------------------------------------------------------
CTMsgGroupList::CItem* CTMsgGroupList::CMember::GetHitItem(const CPoint& pt)
{
	if( HitTest(pt) )
		return this;
	else
		return NULL;
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CMember::Select(BOOL bSelect)
{
	if( m_bSelect != bSelect )
	{
		m_bSelect = bSelect;

		if( bSelect )
			m_pParent->SetSelectItem(this);

		ShowTop( m_bShowTop );
		ShowDown( m_bShowDown );
	}
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CMember::OnLButtonDown(const CPoint& pt)
{
	if( HitTest(pt) )
	{
		StopTwinkle();
		Select( TRUE );
	}
	else
		Select( FALSE );
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CMember::OnRButtonDown(const CPoint& pt)
{
	BOOL bHit = HitTest(pt);
	Select( bHit );

	if( bHit )
	{
		StopTwinkle();
		m_pParent->ShowPopup(m_bSoulmate ? POPUPTYPE_SOULMATE : POPUPTYPE_MEMBER, &pt);
	}
}
// ======================================================================
void CTMsgGroupList::CMember::ShowTop(BOOL bShow)
{
	m_bShowTop = bShow;

	for(INT i=0; i<ISTATE_COUNT; ++i)
	{
		BOOL bShowFrame = bShow;
		if( bShowFrame )
			bShowFrame = m_bSelect? (i==ISTATE_SELECT): (i==ISTATE_NORMAL);

		m_pTopFrame[i]->ShowComponent(bShowFrame);
	}

	m_vTxt[MTXT_NAME]->ShowComponent(bShow);
	m_vTxt[MTXT_JOB]->ShowComponent(bShow);
	m_vTxt[MTXT_LEVEL]->ShowComponent(bShow);
	m_vTxt[MTXT_STATE]->ShowComponent(bShow);
}
// ----------------------------------------------------------------------
void CTMsgGroupList::CMember::ShowDown(BOOL bShow)
{
	m_bShowDown = bShow;

	for(INT i=0; i<ISTATE_COUNT; ++i)
	{
		BOOL bShowFrame = bShow;
		if( bShowFrame )
			bShowFrame = m_bSelect? (i==ISTATE_SELECT): (i==ISTATE_NORMAL);

		m_pDownFrame[i]->ShowComponent(bShowFrame);
	}

	m_vTxt[MTXT_POS]->ShowComponent(bShow);
}
// ======================================================================
void CTMsgGroupList::CMember::StopTwinkle()
{
	if( m_bTwinkle )
	{
		for( INT i=0 ; i < ISTATE_COUNT ; ++i )
		{
			m_pTopFrame[i] = m_pTopTplt[i];
			m_pDownFrame[i] = m_pDownTplt[i];
		}

		m_bTwinkle = FALSE;
	}

	ShowTop( m_bShowTop );
	ShowDown( m_bShowDown );
}
// ======================================================================
void CTMsgGroupList::CMember::FrameUpdate( DWORD dwTickTime )
{
	if( m_bTwinkle )
	{
		m_dwTwinkleAccTime += dwTickTime;

		if( m_dwTwinkleAccTime > 500 )
		{
			std::swap( m_pTopFrame[ ISTATE_NORMAL ], m_pTopFrame[ ISTATE_SELECT ] );
			std::swap( m_pDownFrame[ ISTATE_NORMAL ], m_pDownFrame[ ISTATE_SELECT ] );

			m_dwTwinkleAccTime = 0;

			ShowTop( m_bShowTop );
			ShowDown( m_bShowDown );
		}
	}
	else
	{
		for( INT i=0 ; i < ISTATE_COUNT ; ++i )
		{
			m_pTopFrame[i] = m_pTopTplt[i];
			m_pDownFrame[i] = m_pDownTplt[i];
		}
	}
}
// ======================================================================
void CTMsgGroupList::UpdateSoulmateInfo(
	DWORD dwSoulID,
	CString strSoulName,
	DWORD dwSoulRegion,
	BYTE bSoulLevel,
	BYTE bSoulClass,
	BYTE bSoulConnected )
{
	if( dwSoulID == 0 || strSoulName.IsEmpty() )
	{
		if( m_pSoulmateMember )
		{
			RemoveItem( m_pSoulmateMember->m_nIndex, m_pSoulmateMember->m_pParentGrp );
			m_pSoulmateMember = NULL;
		}

		return ;
	}

	if( m_pSoulmateMember == NULL )
	{
		m_pSoulmateMember = AddMember( m_pSoulmateGroup );
		m_pSoulmateMember->m_bSoulmate = TRUE;
	}

	SetSoulmate(
		m_pSoulmateMember,
		dwSoulID,
		strSoulName,
		dwSoulRegion,
		bSoulLevel,
		bSoulClass,
		bSoulConnected );
}

CTMsgGroupList::CGroup* CTMsgGroupList::GetEtcGroup()
{
	if( NULL == m_pEtcGroup )
	{
		m_pEtcGroup = AddGroup();
		m_pEtcGroup->m_dwID = 0;
		m_pEtcGroup->m_bEtcGroup = TRUE;
		m_pEtcGroup->m_pGroupName->m_strText = CTChart::LoadString( TSTR_GROUP_ETC );
	}

	return m_pEtcGroup;
}

void CTMsgGroupList::CollectTwinkle( std::map< DWORD, BOOL >& vTWINKLE )
{
	CItemArray::iterator it, end;
	it = m_ItemArray.begin();
	end = m_ItemArray.end();

	for(; it != end ; ++it )
	{
		if( (*it)->GetType() == ITYPE_GROUP )
			((CGroup*) (*it))->CollectTwinkle( vTWINKLE );
		else if( (*it)->GetType() == ITYPE_MEMBER )
		{
			vTWINKLE.insert( make_pair( (*it)->m_dwID, ((CMember*) (*it))->m_bTwinkle ) );
		}
	}
}

void CTMsgGroupList::CGroup::CollectTwinkle( std::map< DWORD, BOOL >& vTWINKLE )
{
	CItemArray::iterator it, end;
	it = m_ItemArray.begin();
	end = m_ItemArray.end();

	for(; it != end ; ++it )
	{
		if( (*it)->GetType() == ITYPE_GROUP )
			((CGroup*) (*it))->CollectTwinkle( vTWINKLE );
		else if( (*it)->GetType() == ITYPE_MEMBER )
		{
			vTWINKLE.insert( make_pair( (*it)->m_dwID, ((CMember*) (*it))->m_bTwinkle ) );
		}
	}
}