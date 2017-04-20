#include "StdAfx.h"
#include "Resource.h"
#include "TClientGame.h"


CTMaintainPannel::CTMaintainPannel( TComponent *pParent, LP_FRAMEDESC pDesc)
:CTClientUIBase( pParent, pDesc)
{
	static DWORD dwIconID[TMTYPE_COUNT][TMAINTAIN_COUNT] =
	{
		{
			ID_CTRLINST_P1,
			ID_CTRLINST_P2,
			ID_CTRLINST_P3,
			ID_CTRLINST_P4,
			ID_CTRLINST_P5,
			ID_CTRLINST_P6
		},
		{
			ID_CTRLINST_N1,
			ID_CTRLINST_N2,
			ID_CTRLINST_N3,
			ID_CTRLINST_N4,
			ID_CTRLINST_N5,
			ID_CTRLINST_N6
		}
	};

	static DWORD dwTextID[TMTYPE_COUNT][TMAINTAIN_COUNT] =
	{
		{
			ID_CTRLINST_PT1,
			ID_CTRLINST_PT2,
			ID_CTRLINST_PT3,
			ID_CTRLINST_PT4,
			ID_CTRLINST_PT5,
			ID_CTRLINST_PT6
		},
		{
			ID_CTRLINST_NT1,
			ID_CTRLINST_NT2,
			ID_CTRLINST_NT3,
			ID_CTRLINST_NT4,
			ID_CTRLINST_NT5,
			ID_CTRLINST_NT6
		}
	};

	for( BYTE i=0; i<TMTYPE_COUNT; i++)
	{
		m_bCount[i] = 0;

		for( BYTE j=0; j<TMAINTAIN_COUNT; j++)
		{
			m_pMAINTAIN_ICON[i][j] = (TImageList *) FindKid(dwIconID[i][j]);
			m_pMAINTAIN_TIME[i][j] = (TComponent*) FindKid(dwTextID[i][j]);

			if(m_pMAINTAIN_ICON[i][j])
			{
				m_pMAINTAIN_ICON[i][j]->EnableComponent(TRUE);
				m_pMAINTAIN_ICON[i][j]->ShowComponent(TRUE);
				m_pMAINTAIN_ICON[i][j]->SetCurImage(0);
			}

			if( m_pMAINTAIN_TIME[i][j])
				m_pMAINTAIN_TIME[i][j]->m_strText = "";

			m_vMAINTAININFO_TSKILL[i][j] = NULL;
			m_vMAINTAININFO_LEVEL[i][j] = 0;
		}
	}

	m_bSND = FALSE;

	m_pNextBtn[TMTYPE_P] = static_cast<TButton*>( FindKid( ID_CTRLINST_PB ) );
	m_pNextBtn[TMTYPE_N] = static_cast<TButton*>( FindKid( ID_CTRLINST_NB ) );

	for( int i=0; i<TMTYPE_COUNT; ++i )
		if( m_pNextBtn[i] )
		{
			m_pNextBtn[i]->ShowComponent( FALSE );
			m_pNextBtn[i]->m_bHitTestType = HITTEST_TYPE_RECT;
		}

	m_pOBJ = NULL;
}

CTMaintainPannel::~CTMaintainPannel()
{
}

BOOL CTMaintainPannel::HitTest( CPoint pt)
{
	for( BYTE i=0; i<TMTYPE_COUNT; ++i )
		if( m_pNextBtn[i] &&
			m_pNextBtn[i]->IsVisible() &&
			m_pNextBtn[i]->HitTest( pt ) )
			return TRUE;

	for( BYTE i=0; i<TMTYPE_COUNT; i++)
		for( BYTE j=0; j<TMAINTAIN_COUNT; j++)
			if( m_pMAINTAIN_ICON[i][j] &&
				m_pMAINTAIN_ICON[i][j]->GetCurImage() &&
				m_pMAINTAIN_ICON[i][j]->HitTest(pt) )
				return TRUE;

	return FALSE;
}

void CTMaintainPannel::OnLButtonDown(UINT nFlags, CPoint pt)
{
	for( BYTE i=0; i<TMTYPE_COUNT; ++i )
	{
		if( m_pNextBtn[i] &&
			m_pNextBtn[i]->IsVisible() &&
			m_pNextBtn[i]->HitTest( pt ) )
		{
			m_pNextBtn[i]->OnLButtonDown( nFlags, pt );

			if( i == TMTYPE_P )
				ClickedPNext();
			else
				ClickedNNext();
		}
	}
}

BOOL CTMaintainPannel::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !HitTest( pt ) )
		return FALSE;

	ITDetailInfoPtr pInfo = GetTInfoKey(pt);
	if( pInfo )
	{
		CTSkillDetInfoPtr pSkillInfo = pInfo.staticCast<CTSkillDetInfo>();
		LPTSKILL pTSKILL = pSkillInfo->GetSkillTemp();

		if( pTSKILL &&
			pTSKILL->m_bCanCancel &&
			IsPREMIUMMaintain( pTSKILL ) )
			return FALSE;
	}
	else
	{
		for( int i=0; i<TMTYPE_COUNT; ++i )
			if( !m_pNextBtn[i]->HitTest( pt ) )
				return FALSE;
			else
				break;
	}

	LISTTMAINTAIN::iterator itor;
	LISTTMAINTAIN::iterator end;

	for( BYTE i=0; i<TMTYPE_COUNT; ++i )
	{
		if( !m_pNextBtn[i] )
			continue;

		if( i == TMTYPE_P && m_pNextBtn[i]->IsVisible() )
		{
			itor = m_pOBJ->m_vTPRotation.begin();
			end = m_pOBJ->m_vTPRotation.end();
		}
		else if( i == TMTYPE_N && m_pNextBtn[i]->IsVisible() )
		{
			itor = m_pOBJ->m_vTNRotation.begin();
			end = m_pOBJ->m_vTNRotation.end();
		}
		else
		{
			return FALSE;
		}

		while( itor != end )
		{
			if( zDelta < 0 )
			{
				i == 0 ? ClickedPNext() : ClickedNNext();
				return TRUE;
			}
			else if( zDelta > 0 )
			{
				i == 0 ? ClickedPPrev() : ClickedNPrev();
				return TRUE;
			}
			++itor;
		}
	}

	return TRUE;
}

bool IsPREMIUMMaintain( CTClientMaintain* pMAINTAIN )
{
	static DWORD dwPremiumMaintain[] =
	{
		PREMIUM_MAINTAIN1,
		PREMIUM_MAINTAIN2,
		PREMIUM_MAINTAIN3,
		PREMIUM_MAINTAIN4,
		PREMIUM_MAINTAIN5,
		PREMIUM_MAINTAIN6,
		PREMIUM_MAINTAIN7
	};

	if( pMAINTAIN && pMAINTAIN->m_pTSKILL)
	{
		for( INT i=0 ; i < PREMIUM_MAINTAIN_COUNT ; ++i )
			if( dwPremiumMaintain[i] == pMAINTAIN->m_pTSKILL->m_wSkillID )
				return true;
	}
	return false;
}

bool IsPREMIUMMaintain( LPTSKILL pTSKILL )
{
	static DWORD dwPremiumMaintain[] =
	{
		PREMIUM_MAINTAIN1,
		PREMIUM_MAINTAIN2,
		PREMIUM_MAINTAIN3,
		PREMIUM_MAINTAIN4,
		PREMIUM_MAINTAIN5,
		PREMIUM_MAINTAIN6,
		PREMIUM_MAINTAIN7
	};

	if( pTSKILL)
	{
		for( INT i=0 ; i < PREMIUM_MAINTAIN_COUNT ; ++i )
			if( dwPremiumMaintain[i] == pTSKILL->m_wSkillID )
				return true;
	}
	return false;
}

bool SortCancelFirst( CTClientMaintain* p1, CTClientMaintain* p2 )
{
	if( p1->m_pTSKILL && p2->m_pTSKILL )
	{
		BOOL bPremium1 = IsPREMIUMMaintain(p1);
		BOOL bPremium2 = IsPREMIUMMaintain(p2);

		if( bPremium1 && bPremium2 )
			return true;
		else if( bPremium1 || bPremium2 )
		{
			if( bPremium1 )
				return true;
			else
				return false;
		}
		else
		{
			if( p2->m_pTSKILL->m_bCanCancel && !p1->m_pTSKILL->m_bCanCancel)
				return false;
		}
	}

	return true;
}

void CTMaintainPannel::ResetMAINTAIN( CTClientObjBase* pOBJ, DWORD dwTick)
{
	HideAll();

	m_pOBJ = pOBJ;

	LPMAPTMAINTAIN pTMAINTAIN = pOBJ->GetTMAINTAIN();
	VTMAINTAIN vTMAINTAIN;
	VTMAINTAIN vTPRotation;
	VTMAINTAIN vTNRotation;

	{
		MAPTMAINTAIN::iterator itMAINTAIN;
		for( itMAINTAIN = pTMAINTAIN->begin(); itMAINTAIN != pTMAINTAIN->end(); itMAINTAIN++)
			vTMAINTAIN.push_back( itMAINTAIN->second );

		std::sort( vTMAINTAIN.begin(), vTMAINTAIN.end(), SortCancelFirst);
	}

	for( int i=0; i<TMTYPE_COUNT; ++i )
		m_bCount[i] = 0;

	VTMAINTAIN::iterator itMAINTAIN;

	for( int i=0; i<vTMAINTAIN.size(); ++ i )
	{
		CTClientMaintain *pTSKILL = vTMAINTAIN[i];

		if( !IsPREMIUMMaintain( pTSKILL ) )
			if( !pTSKILL->m_pTSKILL->m_bCanCancel )
				break;

		if( pTSKILL && pTSKILL->m_pTSKILL && pTSKILL->m_pTSKILL->m_bShowIcon )
		{
			BYTE bMType = (pTSKILL->m_pTSKILL->m_bPositive % 2) ? TMTYPE_P : TMTYPE_N;
			BYTE bTMCNT = m_bCount[bMType];
			TImageList* pTIMGLIST = m_pMAINTAIN_ICON[bMType][bTMCNT];
			TComponent* pTTIME = m_pMAINTAIN_TIME[bMType][bTMCNT];

			if( bTMCNT < TMAINTAIN_COUNT && pTIMGLIST )
			{
				if(pTSKILL->m_pTSKILL->m_bShowTime)
				{
					pTTIME->m_strText = CTClientGame::ToTimeOneString(pTSKILL->GetLeftTick(dwTick));
					pTTIME->ShowComponent(TRUE);
				}
				else
				{
					pTTIME->m_strText.Empty();
					pTTIME->ShowComponent(FALSE);
				}

				if( pTSKILL->m_pTSKILL->m_wIconID != 0 )
				{
					pTIMGLIST->SetCurImage(pTSKILL->m_pTSKILL->m_wIconID);
					pTIMGLIST->ShowComponent(TRUE);
				}
				else
					pTIMGLIST->ShowComponent(FALSE);

				// Detail 정보창을 생성하기 위한 기반 정보
				m_vMAINTAININFO_TSKILL[ bMType ][ bTMCNT ] = pTSKILL->m_pTSKILL;
				m_vMAINTAININFO_LEVEL[ bMType ][ bTMCNT ] = pTSKILL->m_bLevel;
			}

			++m_bCount[bMType];
		}
	}

	ResetROTATION(dwTick);

	for( BYTE i=0; i<TMTYPE_COUNT; ++i)
	{
		for( BYTE j=m_bCount[i] ; j<TMAINTAIN_COUNT; ++j)
		{
			if( m_pMAINTAIN_ICON[i][j] )
				m_pMAINTAIN_ICON[i][j]->ShowComponent(FALSE);
			if( m_pMAINTAIN_TIME[i][j] )
				m_pMAINTAIN_TIME[i][j]->ShowComponent(FALSE);
		}
	}
}

void CTMaintainPannel::HideAll()
{
	for( BYTE i=0; i<TMTYPE_COUNT; ++i )
	{
		if( m_pNextBtn[i] )
			m_pNextBtn[i]->ShowComponent( FALSE );
	}

	for( BYTE i=0; i<TMTYPE_COUNT; ++i)
		for( BYTE j=0; j<TMAINTAIN_COUNT; ++j)
			if(m_pMAINTAIN_ICON[i][j])
			{
				m_pMAINTAIN_TIME[i][j]->m_strText.Empty();
				m_pMAINTAIN_TIME[i][j]->ShowComponent(FALSE);
				
				m_pMAINTAIN_ICON[i][j]->SetCurImage(0);
				m_pMAINTAIN_ICON[i][j]->ShowComponent(FALSE);

				m_vMAINTAININFO_TSKILL[i][j] = NULL;
				m_vMAINTAININFO_LEVEL[i][j] = 0;
			}
}

ITDetailInfoPtr CTMaintainPannel::GetTInfoKey( const CPoint& point )
{
	ITDetailInfoPtr pInfo(NULL);

	for( BYTE i=0; i<TMTYPE_COUNT; ++i)
	{
		for( BYTE j=0; j<TMAINTAIN_COUNT; ++j)
		{
			if(m_pMAINTAIN_ICON[i][j] &&
				m_pMAINTAIN_ICON[i][j]->HitTest(point) &&
				m_vMAINTAININFO_TSKILL[i][j] != NULL )
			{
				CRect rc;
				m_pMAINTAIN_ICON[i][j]->GetComponentRect(&rc);
				m_pMAINTAIN_ICON[i][j]->ComponentToScreen(&rc);

				pInfo = CTDetailInfoManager::NewSkillInst( 
					m_vMAINTAININFO_TSKILL[i][j],
					m_vMAINTAININFO_LEVEL[i][j],
					FALSE,
					rc);

				pInfo->SetDir(TRUE, FALSE, TRUE);
				return pInfo;
			}
		}
	}

	return pInfo;
}

void CTMaintainPannel::ClickedPNext()
{
	if( !m_pOBJ || m_pOBJ->m_vTPRotation.empty() )
		return ;

	CTClientMaintain* pFront = m_pOBJ->m_vTPRotation.front();
	m_pOBJ->m_vTPRotation.push_back( pFront );
	m_pOBJ->m_vTPRotation.erase( m_pOBJ->m_vTPRotation.begin() );
}

void CTMaintainPannel::ClickedPPrev()
{
	if( !m_pOBJ || m_pOBJ->m_vTPRotation.empty() )
		return ;

	CTClientMaintain* pBack = m_pOBJ->m_vTPRotation.back();
	m_pOBJ->m_vTPRotation.push_front( pBack );
	m_pOBJ->m_vTPRotation.pop_back();
}

void CTMaintainPannel::ClickedNNext()
{
	if( !m_pOBJ || m_pOBJ->m_vTNRotation.empty() )
		return ;

	CTClientMaintain* pFront = m_pOBJ->m_vTNRotation.front();
	m_pOBJ->m_vTNRotation.push_back( pFront );
	m_pOBJ->m_vTNRotation.erase( m_pOBJ->m_vTNRotation.begin() );
}

void CTMaintainPannel::ClickedNPrev()
{
	if( !m_pOBJ || m_pOBJ->m_vTNRotation.empty() )
		return ;

	CTClientMaintain* pBack = m_pOBJ->m_vTNRotation.back();
	m_pOBJ->m_vTNRotation.push_front( pBack );
	m_pOBJ->m_vTNRotation.pop_back();
}

void CTMaintainPannel::ResetROTATION( DWORD dwTick)
{
	LISTTMAINTAIN::iterator itor;
	LISTTMAINTAIN::iterator end;

	for( int i=0; i<TMTYPE_COUNT; ++i )
	{
		if( i == TMTYPE_P )
		{
			itor = m_pOBJ->m_vTPRotation.begin();
			end = m_pOBJ->m_vTPRotation.end();
		}
		else
		{
			itor = m_pOBJ->m_vTNRotation.begin();
			end = m_pOBJ->m_vTNRotation.end();
		}

		while( itor != end )
		{
			BYTE bRCount = m_bCount[i];

			CTClientMaintain* pTSKILL = (*itor);

			if( pTSKILL && pTSKILL->m_pTSKILL && pTSKILL->m_pTSKILL->m_bShowIcon )
			{
				TImageList* pTIMGLIST = m_pMAINTAIN_ICON[i][bRCount];
				TComponent* pTTIME = m_pMAINTAIN_TIME[i][bRCount];

				if( bRCount < TMAINTAIN_COUNT && pTIMGLIST )
				{
					if(pTSKILL->m_pTSKILL->m_bShowTime)
					{
						pTTIME->m_strText = CTClientGame::ToTimeOneString(pTSKILL->GetLeftTick(dwTick));
						pTTIME->ShowComponent(TRUE);
					}
					else
					{
						pTTIME->m_strText.Empty();
						pTTIME->ShowComponent(FALSE);
					}

					if( pTSKILL->m_pTSKILL->m_wIconID != 0 )
					{
						pTIMGLIST->SetCurImage(pTSKILL->m_pTSKILL->m_wIconID);
						pTIMGLIST->ShowComponent(TRUE);
					}
					else
						pTIMGLIST->ShowComponent(FALSE);

					m_vMAINTAININFO_TSKILL[ i ][ bRCount ] = pTSKILL->m_pTSKILL;
					m_vMAINTAININFO_LEVEL[ i ][ bRCount ] = pTSKILL->m_bLevel;
				}

				++m_bCount[i];
			}		
			++itor;
		}

		if( m_pNextBtn[i] && IsVisible() )
		{
			if( m_bCount[i] > TMAINTAIN_COUNT )
				m_pNextBtn[i]->ShowComponent( TRUE );
			else
				m_pNextBtn[i]->ShowComponent( FALSE );
		}
	}
}