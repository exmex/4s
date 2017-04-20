#include "stdafx.h"

#ifdef ADD_RPS

#include "TRpcDlg.h"
#include "TClientGame.h"

CTRpsDlg::CTRpsDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc)
{
	m_pRPSNpc = (TImageList*) FindKid( ID_CTRLINST_RPS_NPC );
	m_pRPSPlayer = (TImageList*) FindKid( ID_CTRLINST_RPS_PLAYER );
	m_pR = (TButton*) FindKid( ID_CTRLINST_ROCK );
	m_pP = (TButton*) FindKid( ID_CTRLINST_PAPER );
	m_pS = (TButton*) FindKid( ID_CTRLINST_SCISSOR );
	m_p1 = (TButton*) FindKid( ID_CTRLINST_1 );
	m_p2 = (TButton*) FindKid( ID_CTRLINST_2 );
	m_p3 = (TButton*) FindKid( ID_CTRLINST_3 );

	m_pNPC = NULL;
	m_bSelectMode = TRUE;
	m_ePlayer = RPSCARD_COUNT;
	m_eNpc = RPSCARD_COUNT;

	GetComponentSize(&m_Size);

	m_dwRandTick = 0;
	m_nRand = (INT) RPSCARD_ROCK;
}

CTRpsDlg::~CTRpsDlg()
{
}

HRESULT CTRpsDlg::Render(DWORD dwTickCount)
{
	if( IsVisible() )
	{
		CTClientGame* pTGAME = CTClientGame::GetInstance();

		// NPC 렌더링
		if( m_pNPC )
		{
			if( !m_bSelectMode )
			{
				m_pRPSNpc->SetCurImage(m_eNpc);
				m_pRPSNpc->ShowComponent(TRUE);
			}
			else
			{
				m_dwRandTick -= min(m_dwRandTick, dwTickCount);
				if( m_dwRandTick == 0 )
				{
					m_nRand = ++m_nRand % RPSCARD_COUNT;
					m_pRPSNpc->SetCurImage(m_nRand);
					m_dwRandTick = 200;
				}
				m_pRPSNpc->ShowComponent(TRUE);
			}
		}
		else
			m_pRPSNpc->ShowComponent(FALSE);

		// 플레이어 렌더링
		CTClientMoveObj* pTOBJ = pTGAME->GetCtrlHost();
		if( pTOBJ )
		{
			m_pR->ShowComponent(m_bSelectMode);
			m_pP->ShowComponent(m_bSelectMode);
			m_pS->ShowComponent(m_bSelectMode);
			m_p1->ShowComponent(m_bSelectMode);
			m_p2->ShowComponent(m_bSelectMode);
			m_p3->ShowComponent(m_bSelectMode);
			m_pRPSPlayer->ShowComponent(!m_bSelectMode);
			if( !m_bSelectMode )
				m_pRPSPlayer->SetCurImage(m_ePlayer);

			D3DXVECTOR3 vTPOS = pTGAME->GetBaseNamePosition(pTOBJ);
			pTGAME->ResetObjNamePos(pTOBJ, vTPOS);

			CPoint vPOS(
				INT(pTGAME->GetCtrlHost()->m_vNAMEPOS.x + pTGAME->GetCtrlHost()->m_vNAME.GetWidth() / 2 - m_Size.cx / 2),
				INT(pTGAME->GetCtrlHost()->m_vNAMEPOS.y - m_Size.cy));
			MoveComponent(vPOS);

			CTClientUIBase::Render(dwTickCount);
		}
	}

	return S_OK;
}

BOOL CTRpsDlg::CanDisable()
{
	return !m_bSelectMode; // 선택 중일땐 창을 닫을 수 없다.
}

BOOL CTRpsDlg::CanWithItemUI()
{
	return FALSE;
}

#endif
