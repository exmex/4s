#include "StdAfx.h"
#include "TClientGame.h"


//CTGraphicSettingDlg::CTGraphicSettingDlg( CD3DDevice *pDevice, TComponent *pParent, LP_FRAMEDESC pDesc)
//:CTClientUIBase( pParent, pDesc)
//{
//	static DWORD dwMAPDETAIL[TOPTIONLEVEL_COUNT] = {
//		ID_CTRLINST_MAPDETAIL_LOW,
//		ID_CTRLINST_MAPDETAIL_MED,
//		ID_CTRLINST_MAPDETAIL_HI};
//
//	static DWORD dwOBJDETAIL[TOPTIONLEVEL_COUNT] = {
//		ID_CTRLINST_OBJDETAIL_LOW,
//		ID_CTRLINST_OBJDETAIL_MED,
//		ID_CTRLINST_OBJDETAIL_HI};
//
//	static DWORD dwTCHECK[TGRAPHIC_CHECK_COUNT] = {
//		ID_CTRLINST_WINDOW_MODE,
//		ID_CTRLINST_GLOW,
//		ID_CTRLINST_MAP_SHADOW,
//		ID_CTRLINST_MAP_SFX,
//		ID_CTRLINST_LIGHTMAP,
//		ID_CTRLINST_FARIMAGE,
//		ID_CTRLINST_NPC_NAME,
//		ID_CTRLINST_MON_NAME,
//		ID_CTRLINST_PC_NAME};
//
//	LPBYTE pTOPTION[TGRAPHIC_CHECK_COUNT] = {
//		&pDevice->m_option.m_bWindowedMode,
//		&CTClientGame::m_vTOPTION.m_bGLOW,
//		&CTClientGame::m_vTOPTION.m_bMapSHADOW,
//		&CTClientGame::m_vTOPTION.m_bMapSFX,
//		&CTClientGame::m_vTOPTION.m_bLIGHTMAP,
//		&CTClientGame::m_vTOPTION.m_bFarIMAGE,
//		&CTClientGame::m_vTOPTION.m_bNpcNAME,
//		&CTClientGame::m_vTOPTION.m_bMonNAME,
//		&CTClientGame::m_vTOPTION.m_bPcNAME};
//
//	m_pTMAPDETAILOPTION = &CTClientGame::m_vTOPTION.m_bMapDETAIL;
//	m_pTOBJDETAILOPTION = &CTClientGame::m_vTOPTION.m_bObjDETAIL;
//	m_pTRANGEOPTION = &CTClientGame::m_vTOPTION.m_fOBJRange;
//
//	for( BYTE i=0; i<TOPTIONLEVEL_COUNT; i++)
//	{
//		m_pTMAPDETAIL[i] = (TButton *) FindKid(dwMAPDETAIL[i]);
//		m_pTOBJDETAIL[i] = (TButton *) FindKid(dwOBJDETAIL[i]);
//
//		m_pTMAPDETAIL[i]->Select(i == (*m_pTMAPDETAILOPTION) ? TRUE : FALSE);
//		m_pTOBJDETAIL[i]->Select(i == (*m_pTOBJDETAILOPTION) ? TRUE : FALSE);
//	}
//
//	for( i=0; i<TGRAPHIC_CHECK_COUNT; i++)
//	{
//		m_pTCHECK[i] = (TButton *) FindKid(dwTCHECK[i]);
//
//		m_pTCHECK[i]->Select((*pTOPTION[i]) ? TRUE : FALSE);
//		m_pTCHECKOPTION[i] = pTOPTION[i];
//	}
//
//	m_pTRANGE = (TScroll *) FindKid(ID_CTRLINST_OBJ_RANGE);
//	m_pTRANGE->SetScrollType(FALSE);
//	m_pTRANGE->SetScrollPos( 100, INT(((*m_pTRANGEOPTION) - TMIN_RANGEOPTION) * 100.0f / (TMAX_RANGEOPTION - TMIN_RANGEOPTION)));
//}
//
//CTGraphicSettingDlg::~CTGraphicSettingDlg()
//{
//}
//
//void CTGraphicSettingDlg::OnLButtonDown( UINT nFlags, CPoint pt)
//{
//	CTClientUIBase::OnLButtonDown( nFlags, pt);
//
//	if(IsVisible())
//	{
//		BYTE bTMAPDETAIL = (*m_pTMAPDETAILOPTION);
//		BYTE bTOBJDETAIL = (*m_pTOBJDETAILOPTION);
//
//		for( BYTE i=0; i<TOPTIONLEVEL_COUNT; i++)
//		{
//			if( (*m_pTMAPDETAILOPTION) != i && m_pTMAPDETAIL[i]->IsStateDown() )
//				bTMAPDETAIL = i;
//
//			if( (*m_pTOBJDETAILOPTION) != i && m_pTOBJDETAIL[i]->IsStateDown() )
//				bTOBJDETAIL = i;
//		}
//
//		(*m_pTMAPDETAILOPTION) = bTMAPDETAIL;
//		(*m_pTOBJDETAILOPTION) = bTOBJDETAIL;
//
//		for( i=0; i<TOPTIONLEVEL_COUNT; i++)
//		{
//			m_pTMAPDETAIL[i]->Select(i == (*m_pTMAPDETAILOPTION) ? TRUE : FALSE);
//			m_pTOBJDETAIL[i]->Select(i == (*m_pTOBJDETAILOPTION) ? TRUE : FALSE);
//		}
//
//		if( (*m_pTCHECKOPTION[TGRAPHIC_CHECK_SCREEN_MODE]) != m_pTCHECK[TGRAPHIC_CHECK_SCREEN_MODE]->IsStateDown() )
//			m_pCommandHandler->m_vCOMMAND.push_back(GM_TOGGLE_SCREEN_MODE);
//
//		for( i=0; i<TGRAPHIC_CHECK_COUNT; i++)
//			(*m_pTCHECKOPTION[i]) = m_pTCHECK[i]->IsStateDown() ? TRUE : FALSE;
//	}
//}
//
//void CTGraphicSettingDlg::OnMouseMove( UINT nFlags, CPoint pt)
//{
//	CTClientUIBase::OnMouseMove( nFlags, pt);
//
//	if(IsVisible())
//	{
//		int nRange;
//		int nPos;
//
//		m_pTRANGE->GetScrollPos( nRange, nPos);
//		(*m_pTRANGEOPTION) = TMIN_RANGEOPTION + (TMAX_RANGEOPTION - TMIN_RANGEOPTION) * FLOAT(nPos) / FLOAT(nRange);
//	}
//}
