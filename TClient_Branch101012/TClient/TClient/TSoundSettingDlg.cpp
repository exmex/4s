#include "StdAfx.h"

//
//CTSoundSettingDlg::CTSoundSettingDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
//:CTClientUIBase( pParent, pDesc)
//{
//	static DWORD dwTCHECK[] = {
//		ID_CTRLINST_MAIN,
//		ID_CTRLINST_BGM,
//		ID_CTRLINST_SFX};
//
//	static DWORD dwTSLIDE[] = {
//		ID_CTRLINST_MAIN_VOL,
//		ID_CTRLINST_BGM_VOL,
//		ID_CTRLINST_SFX_VOL};
//
//	static LPBYTE pTVOLUME[] = {
//		&CTachyonMedia::m_bMasterVolume,
//		&CTShow::m_bMasterVolume,
//		&CD3DSound::m_bMasterVolume};
//
//	static LPBYTE pTCHECK[] = {
//		&CTachyonMedia::m_bON,
//		&CTShow::m_bON,
//		&CD3DSound::m_bON};
//
//	for( BYTE i=0; i<TSOUND_OPTION_COUNT; i++)
//	{
//		m_pTCHECK[i] = (TButton *) FindKid(dwTCHECK[i]);
//		m_pTSLIDE[i] = (TScroll *) FindKid(dwTSLIDE[i]);
//
//		m_pTVOLUME[i] = pTVOLUME[i];
//		m_pTENABLE[i] = pTCHECK[i];
//
//		m_pTSLIDE[i]->SetScrollType(FALSE);
//		m_pTSLIDE[i]->SetScrollPos( VOLUME_MAX, (*m_pTVOLUME[i]));
//		m_pTCHECK[i]->Select((*m_pTENABLE[i]) ? TRUE : FALSE);
//	}
//}
//
//CTSoundSettingDlg::~CTSoundSettingDlg()
//{
//}
//
//void CTSoundSettingDlg::OnLButtonDown( UINT nFlags, CPoint pt)
//{
//	CTClientUIBase::OnLButtonDown( nFlags, pt);
//
//	if(IsVisible())
//	{
//		for( BYTE i=0; i<TSOUND_OPTION_COUNT; i++)
//			(*m_pTENABLE[i]) = m_pTCHECK[i]->IsStateDown() ? TRUE : FALSE;
//
//		if((*m_pTENABLE[TSOUND_OPTION_MASTER]))
//		{
//			if(!(*m_pTENABLE[TSOUND_OPTION_SFX]))
//				CTachyonRes::m_MEDIA.Stop(MEDIA_DSOUND);
//
//			if(!(*m_pTENABLE[TSOUND_OPTION_BGM]))
//				CTachyonRes::m_MEDIA.Stop(MEDIA_DSHOW);
//		}
//		else
//		{
//			CTachyonRes::m_MEDIA.Stop(MEDIA_DSOUND);
//			CTachyonRes::m_MEDIA.Stop(MEDIA_DSHOW);
//		}
//	}
//}
//
//void CTSoundSettingDlg::OnMouseMove( UINT nFlags, CPoint pt)
//{
//	CTClientUIBase::OnMouseMove( nFlags, pt);
//
//	if(IsVisible())
//	{
//		for( BYTE i=0; i<TSOUND_OPTION_COUNT; i++)
//		{
//			int nRange;
//			int nPos;
//
//			m_pTSLIDE[i]->GetScrollPos( nRange, nPos);
//			(*m_pTVOLUME[i]) = BYTE(nPos);
//		}
//
//		CTachyonRes::m_MEDIA.ResetVolume(MEDIA_DSOUND);
//		CTachyonRes::m_MEDIA.ResetVolume(MEDIA_DSHOW);
//	}
//}
