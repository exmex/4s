#include "Stdafx.h"
#include "TGameSettingFrame.h"
#include "TClientGame.h"



LPBYTE CTGameSettingFrame::TOPTIONPTR[OPTION_COUNT] =
{
	&CTClientGame::m_vTOPTION.m_bNpcNAME,
	&CTClientGame::m_vTOPTION.m_bPcNAME,
	&CTClientGame::m_vTOPTION.m_bMonNAME,
	&CTClientGame::m_vTOPTION.m_bAutoHelp,
	&CTClientGame::m_vTOPTION.m_bTalkBox,
	&CTClientGame::m_vTOPTION.m_bDenyWhisper,
	&CTClientGame::m_vTOPTION.m_bDenyCommunity,
	&CTClientGame::m_vTOPTION.m_bHUD,
	&CTClientGame::m_vTOPTION.m_bContinualChat,
	&CTClientGame::m_vTOPTION.m_bMouseClickMove,
	&CTClientGame::m_vTOPTION.m_bAutoTargeting
};

DWORD CTGameSettingFrame::TNAME[OPTION_COUNT] = 
{
	TSTR_GAMEOPTION_SHOW_NPC_NAME, //NPC 이름 표시
	TSTR_GAMEOPTION_SHOW_PLAYER_NAME, //캐릭터 이름 표시
	TSTR_GAMEOPTION_SHOW_MONSTER_NAME, //몬스터 이름 표시
	TSTR_GAMEOPTION_AUTO_HELP, //자동 도움말
	TSTR_GAMEOPTION_SHOW_TALK_BOX, //말풍선 보이기
	TSTR_GAMEOPTION_DENY_WHISPER, //귓말 거부
	TSTR_GAMEOPTION_DENY_COMMUNITY, //커뮤니티 거부
	TSTR_GAMEOPTION_SHOW_HUD, //HUD 보이기
	TSTR_GAMEOPTION_CONTINUOUS_CHAT, //연속 채팅
	TSTR_GAMEOPTION_MOUSE_CLICK_MOVE, //마우스 클릭 이동
	TSTR_GAMEOPTION_AUTO_TARGETING //자동 타겟팅 사용
};

// ===============================================================================
CTGameSettingFrame::CTGameSettingFrame(TComponent *pParent, LP_FRAMEDESC pDesc)
	: ITInnerFrame(pParent, pDesc, TSETTING_FRAME_GAME)
{
	m_pTLIST = (TList*) FindKid( ID_CTRLINST_LIST );

	for( int i=0 ; i < OPTION_COUNT ; ++i )
	{
		int nLine = m_pTLIST->AddString( CTChart::LoadString( (TSTRING) CTGameSettingFrame::TNAME[i] ) );
		m_pTLIST->SetItemData( nLine, 0, i );
	}

	CPoint ptBasisPos;
	m_pTLIST->GetComponentPos(&ptBasisPos);
    
	TButton* pTCHECK = (TButton*) FindKid( ID_CTRLINST_CHECK );
	RemoveKid(pTCHECK);

	CPoint ptOffset;
	pTCHECK->GetComponentPos(&ptOffset);
	ptOffset = ptOffset - ptBasisPos;
	int n = m_pTLIST->GetMaxItemHeight();
	int nTotal = m_pTLIST->GetItemPerPage();
	m_pTCHECK = new TButton*[nTotal];

	for( int i=0 ; i < nTotal ; ++i )
	{
		m_pTCHECK[ i ] = new TButton( this, pTCHECK->m_pDESC );
		m_pTCHECK[ i ]->m_id = GetUniqueID();
		m_pTCHECK[ i ]->SetStateButton();
		m_pTCHECK[ i ]->MoveComponent( ptBasisPos + ptOffset );
		AddKid( m_pTCHECK[ i ] );
		ptOffset.y += n;
	}

	delete pTCHECK;

	UpdateUI();
}
// -------------------------------------------------------------------------------
CTGameSettingFrame::~CTGameSettingFrame()
{
	if( m_pTCHECK )
		delete [] m_pTCHECK;
}
// ===============================================================================
void CTGameSettingFrame::UpdateUI()
{
	for( INT i=0 ; i < m_pTLIST->GetItemPerPage() ; ++i )
	{
		BOOL bValid = ( m_pTLIST->GetTop() + i < m_pTLIST->GetItemCount() );
		if( bValid )
		{
			m_pTCHECK[ i ]->Select( *(CTGameSettingFrame::TOPTIONPTR[ m_pTLIST->GetTop()+i ]) );
			RemoveKid(m_pTCHECK[ i ] );
			AddKid(m_pTCHECK[ i ] );
			m_pTCHECK[ i ]->ShowComponent(TRUE);
		}
		else
		{
			m_pTCHECK[ i ]->ShowComponent(FALSE);
		}
	}
}
// ===============================================================================
void CTGameSettingFrame::OnLButtonDown( UINT nFlags, CPoint pt)
{
	ITInnerFrame::OnLButtonDown(nFlags, pt);

	if( IsVisible() )
	{
		for( int i=0 ; i < m_pTLIST->GetItemPerPage() ; ++i )
		{
			BOOL bChecked = m_pTCHECK[ i ]->IsStateDown();

			BOOL bValid = ( m_pTLIST->GetTop() + i < m_pTLIST->GetItemCount() );
			if( bValid )
			{
				int index = m_pTLIST->GetTop() + i;
				BOOL bChecked = m_pTCHECK[i]->IsStateDown() ? TRUE : FALSE;
				BOOL bCurrent = *(CTGameSettingFrame::TOPTIONPTR[ index ]);

				if( bChecked != bCurrent && index == 0 ) // 등급표시 변경
				{
					CTGaugePannel* pTGAUGE = static_cast<CTGaugePannel*>( CTClientGame::GetInstance()->GetFrame( TFRAME_GAUGE ) );
					if( bChecked  )
						pTGAUGE->AttachCaution15();
					else
						pTGAUGE->DettachCaution15();
				}

				*(CTGameSettingFrame::TOPTIONPTR[ index ]) = bChecked;
			}
		}
	}
}
// ===============================================================================

HRESULT CTGameSettingFrame::Render(DWORD dwTickCount)
{
	if( IsVisible() )
	{
		UpdateUI();
	}

	return ITInnerFrame::Render(dwTickCount);
}
