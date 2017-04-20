#include "Stdafx.h"
#include "TSystemSettingFrame.h"
#include "TClientGame.h"

// ===============================================================================
CTSystemSettingFrame::CTSystemSettingFrame(TComponent *pParent, LP_FRAMEDESC pDesc, CD3DDevice *pDevice)
	: ITInnerFrame(pParent, pDesc, TSETTING_FRAME_SYSTEM), m_pDevice(pDevice)
{
	static const DWORD dwTSYSCHECK[TSYSTEM_CHECK_COUNT] = {
		ID_CTRLINST_WINDOW_MODE,
		ID_CTRLINST_MAP_SHADOW,
		ID_CTRLINST_MAP_SFX,
		ID_CTRLINST_D_LIGHTMAP,
		ID_CTRLINST_F_LIGHTMAP,
		ID_CTRLINST_FARIMAGE};

	static const DWORD dwMAPDETAIL[TOPTIONLEVEL_COUNT] = {
		ID_CTRLINST_MAPDETAIL_LOW,
		ID_CTRLINST_MAPDETAIL_MED,
		ID_CTRLINST_MAPDETAIL_HI};

	static const DWORD dwOBJDETAIL[TOPTIONLEVEL_COUNT] = {
		ID_CTRLINST_OBJDETAIL_LOW,
		ID_CTRLINST_OBJDETAIL_MED,
		ID_CTRLINST_OBJDETAIL_HI};

	static const DWORD dwTEXDETAIL[TEXTURE_DETAIL_COUNT] = {
		ID_CTRLINST_TEXTUREDET_LOW,
		ID_CTRLINST_TEXTUREDET_MED,
		ID_CTRLINST_TEXTUREDET_HI};

	static const DWORD dwSFXDETAIL[TOPTIONLEVEL_COUNT] = {
		ID_CTRLINST_EFFECT_LOW,
		ID_CTRLINST_EFFECT_MED,
		ID_CTRLINST_EFFECT_HI};

	static const DWORD dwTSNDCHECK[] = {
		ID_CTRLINST_CHK_MAIN_VOL,
		ID_CTRLINST_CHK_BGM_VOL,
		ID_CTRLINST_CHK_SFX_VOL};

	static const DWORD dwTSNDSLIDE[] = {
		ID_CTRLINST_MAIN_VOL,
		ID_CTRLINST_BGM_VOL,
		ID_CTRLINST_SFX_VOL};

	static LPBYTE pTOPTION[TSYSTEM_CHECK_COUNT] = {
		&pDevice->m_option.m_bWindowedMode,
		&CTClientGame::m_vTOPTION.m_bMapSHADOW,
		&CTClientGame::m_vTOPTION.m_bMapSFX,
		&CTClientGame::m_vTOPTION.m_bDungeonLIGHTMAP,
		&CTClientGame::m_vTOPTION.m_bFieldLIGHTMAP,
		&CTClientGame::m_vTOPTION.m_bFarIMAGE};

	static LPBYTE pTSNDVOLUME[] = {
		&CTachyonMedia::m_bMasterVolume,
		&CT3DMusic::m_bMasterVolume,
		&CD3DSound::m_bMasterVolume};

	static LPBYTE pTSNDCHECK[] = {
		&CTachyonMedia::m_bON,
		&CT3DMusic::m_bON,
		&CD3DSound::m_bON};

	m_pTMAPDETAILOPTION = &CTClientGame::m_vTOPTION.m_bMapDETAIL;
	m_pTOBJDETAILOPTION = &CTClientGame::m_vTOPTION.m_bObjDETAIL;
	m_bTSFXDETAILOPTION = CTachyonSFX::GetSystemOptionLOD();
	m_pTRANGEOPTION = &CTClientGame::m_vTOPTION.m_fOBJRange;
	m_pResolutionListPopup = NULL;

	for( BYTE i=0; i<TOPTIONLEVEL_COUNT; i++)
	{
		m_pTMAPDETAIL[i] = (TButton *) FindKid(dwMAPDETAIL[i]);
		m_pTOBJDETAIL[i] = (TButton *) FindKid(dwOBJDETAIL[i]);
		m_pTSFXDETAIL[i] = (TButton *) FindKid(dwSFXDETAIL[i]);
	}

	for( i=0; i<TEXTURE_DETAIL_COUNT; i++)
	{
		m_pTTEXDETAIL[i] = (TButton *) FindKid(dwTEXDETAIL[i]);
		m_pTTEXDETAIL[i]->m_menu[TNM_LCLICK] = GM_SELECT_TEXTURE_DETAIL;
	}

	for( i=0; i<TSYSTEM_CHECK_COUNT; i++)
	{
		m_pTSYSCHECK[i] = (TButton *) FindKid(dwTSYSCHECK[i]);
		m_pTSYSCHECKOPTION[i] = pTOPTION[i];
	}

	m_pTRANGESLIDE = (TScroll *) FindKid(ID_CTRLINST_OBJ_RANGE);
	m_pTRANGESLIDE->SetScrollType(FALSE);

	for( i=0; i<TSOUND_OPTION_COUNT; i++)
	{
		m_pTSNDCHECK[i] = (TButton *) FindKid(dwTSNDCHECK[i]);
		m_pTSNDSLIDE[i] = (TScroll *) FindKid(dwTSNDSLIDE[i]);

		m_pTSNDVOLUME[i] = pTSNDVOLUME[i];
		m_pTSNDENABLE[i] = pTSNDCHECK[i];

		m_pTSNDSLIDE[i]->SetScrollType(FALSE);
	}

	UpdateUI(pDevice);
}
// -------------------------------------------------------------------------------
CTSystemSettingFrame::~CTSystemSettingFrame()
{
}
// ===============================================================================
void CTSystemSettingFrame::UpdateUI( CD3DDevice *pDevice)
{
	for( BYTE i=0; i<TOPTIONLEVEL_COUNT; i++)
	{
		m_pTMAPDETAIL[i]->Select(i == (*m_pTMAPDETAILOPTION) ? TRUE : FALSE);
		m_pTOBJDETAIL[i]->Select(i == (*m_pTOBJDETAILOPTION) ? TRUE : FALSE);
		m_pTSFXDETAIL[i]->Select(i == m_bTSFXDETAILOPTION ? TRUE : FALSE);
	}

	for( i=0; i<TEXTURE_DETAIL_COUNT; i++)
		m_pTTEXDETAIL[i]->Select((i == pDevice->m_option.m_nTextureDetail) ? TRUE : FALSE);

	for( i=0; i<TSYSTEM_CHECK_COUNT; i++)
		m_pTSYSCHECK[i]->Select((*m_pTSYSCHECKOPTION[i]) ? TRUE : FALSE);
	m_pTRANGESLIDE->SetScrollPos( 100, INT(((*m_pTRANGEOPTION) - TMIN_RANGEOPTION) * 100.0f / (TMAX_RANGEOPTION - TMIN_RANGEOPTION)));

	for( i=0; i<TSOUND_OPTION_COUNT; i++)
	{
		m_pTSNDSLIDE[i]->SetScrollPos( VOLUME_MAX, (*m_pTSNDVOLUME[i]));
		m_pTSNDCHECK[i]->Select((*m_pTSNDENABLE[i]) ? TRUE : FALSE);
	}
}
// ===============================================================================
void CTSystemSettingFrame::OnLButtonDown( UINT nFlags, CPoint pt)
{
	ITInnerFrame::OnLButtonDown( nFlags, pt);

	if(IsVisible())
	{
		BYTE bTMAPDETAIL = (*m_pTMAPDETAILOPTION);
		BYTE bTOBJDETAIL = (*m_pTOBJDETAILOPTION);
		BYTE bTSFXDETAIL = m_bTSFXDETAILOPTION;

		for( BYTE i=0; i<TOPTIONLEVEL_COUNT; i++)
		{
			if( (*m_pTMAPDETAILOPTION) != i && m_pTMAPDETAIL[i]->IsStateDown() )
				bTMAPDETAIL = i;

			if( (*m_pTOBJDETAILOPTION) != i && m_pTOBJDETAIL[i]->IsStateDown() )
				bTOBJDETAIL = i;

			if( m_bTSFXDETAILOPTION != i && m_pTSFXDETAIL[i]->IsStateDown() )
				bTSFXDETAIL = i;
		}

		(*m_pTMAPDETAILOPTION) = bTMAPDETAIL;
		(*m_pTOBJDETAILOPTION) = bTOBJDETAIL;
		m_bTSFXDETAILOPTION = bTSFXDETAIL;
		CTachyonSFX::SetOptionLOD( m_bTSFXDETAILOPTION );

		for( i=0; i<TOPTIONLEVEL_COUNT; i++)
		{
			m_pTMAPDETAIL[i]->Select(i == (*m_pTMAPDETAILOPTION) ? TRUE : FALSE);
			m_pTOBJDETAIL[i]->Select(i == (*m_pTOBJDETAILOPTION) ? TRUE : FALSE);
			m_pTSFXDETAIL[i]->Select(i == m_bTSFXDETAILOPTION ? TRUE : FALSE);
		}

		if( (*m_pTSYSCHECKOPTION[TSYSTEM_CHECK_SCREEN_MODE]) != m_pTSYSCHECK[TSYSTEM_CHECK_SCREEN_MODE]->IsStateDown() )
			m_pCommandHandler->m_vCOMMAND.push_back(GM_TOGGLE_SCREEN_MODE);

		for( i=0; i<TSYSTEM_CHECK_COUNT; i++)
			(*m_pTSYSCHECKOPTION[i]) = m_pTSYSCHECK[i]->IsStateDown() ? TRUE : FALSE;

		for( i=0; i<TSOUND_OPTION_COUNT; i++)
			(*m_pTSNDENABLE[i]) = m_pTSNDCHECK[i]->IsStateDown() ? TRUE : FALSE;

		if((*m_pTSNDENABLE[TSOUND_OPTION_MASTER]))
		{
			if(!(*m_pTSNDENABLE[TSOUND_OPTION_SFX]))
				CTachyonRes::m_MEDIA.Stop(MEDIA_TSOUND);

			if(!(*m_pTSNDENABLE[TSOUND_OPTION_BGM]))
				CTachyonRes::m_MEDIA.Stop(MEDIA_TMUSIC);
		}
		else
		{
			CTachyonRes::m_MEDIA.Stop(MEDIA_TSOUND);
			CTachyonRes::m_MEDIA.Stop(MEDIA_TMUSIC);
		}
	}
}
// -------------------------------------------------------------------------------
void CTSystemSettingFrame::OnMouseMove( UINT nFlags, CPoint pt)
{
	ITInnerFrame::OnMouseMove( nFlags, pt);

	if(IsVisible())
	{
		int nRange;
		int nPos;

		m_pTRANGESLIDE->GetScrollPos( nRange, nPos);
		(*m_pTRANGEOPTION) = TMIN_RANGEOPTION + (TMAX_RANGEOPTION - TMIN_RANGEOPTION) * FLOAT(nPos) / FLOAT(nRange);

		for( BYTE i=0; i<TSOUND_OPTION_COUNT; i++)
		{
			m_pTSNDSLIDE[i]->GetScrollPos( nRange, nPos);
			(*m_pTSNDVOLUME[i]) = BYTE(nPos);
		}

		CTachyonRes::m_MEDIA.ResetVolume(MEDIA_TSOUND);
		CTachyonRes::m_MEDIA.ResetVolume(MEDIA_TMUSIC);
	}
}
// ===============================================================================
void CTSystemSettingFrame::ShowComponent( BOOL bVisible)
{
	if( m_bVisible != bVisible && m_bSND )
		if( bVisible && m_dwOPENSND )
			CTachyonRes::m_MEDIA.Play( MEDIA_TSOUND, m_dwOPENSND);
		else if( !bVisible && m_dwCLOSESND )
			CTachyonRes::m_MEDIA.Play( MEDIA_TSOUND, m_dwCLOSESND);

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	m_bVisible = bVisible;

	while( !EndOfKids( it ) )
	{
		TComponent* pKid = GetNextKid( it );

		if( pKid && !pKid->m_blIsCaret && (pKid != m_pResolutionListPopup || !bVisible) )
			pKid->ShowComponent( bVisible );
	}
}
