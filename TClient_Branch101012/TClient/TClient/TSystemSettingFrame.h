#pragma once

class CTSystemSettingFrame : public ITInnerFrame
{
public:
	CTMiniPopupDlg *m_pResolutionListPopup;
	CD3DDevice	*m_pDevice;

	TButton 	*m_pTMAPDETAIL[TOPTIONLEVEL_COUNT];
	TButton 	*m_pTOBJDETAIL[TOPTIONLEVEL_COUNT];
	TButton 	*m_pTSYSCHECK[TSYSTEM_CHECK_COUNT];
	TButton		*m_pTTEXDETAIL[TEXTURE_DETAIL_COUNT];
	TButton		*m_pTSFXDETAIL[TOPTIONLEVEL_COUNT];

	TButton 	*m_pTSNDCHECK[TSOUND_OPTION_COUNT];
	TScroll 	*m_pTSNDSLIDE[TSOUND_OPTION_COUNT];

	TScroll		*m_pTRANGESLIDE;

	LPBYTE 		m_pTSNDENABLE[TSOUND_OPTION_COUNT];
	LPBYTE 		m_pTSNDVOLUME[TSOUND_OPTION_COUNT];

	LPBYTE		m_pTSYSCHECKOPTION[TSYSTEM_CHECK_COUNT];
	LPBYTE		m_pTMAPDETAILOPTION;
	LPBYTE		m_pTOBJDETAILOPTION;
	BYTE		m_bTSFXDETAILOPTION;
	FLOAT 		*m_pTRANGEOPTION;

public:
	virtual void RequestInfo()	{}
	virtual void ResetInfo()	{}

public:
	virtual void OnLButtonDown( UINT nFlags, CPoint pt);
	virtual void OnMouseMove( UINT nFlags, CPoint pt);
	virtual void ShowComponent( BOOL bVisible = TRUE);

public:
	void UpdateUI( CD3DDevice *pDevice);

public:
	CTSystemSettingFrame(TComponent *pParent, LP_FRAMEDESC pDesc, CD3DDevice *pDevice);
	virtual ~CTSystemSettingFrame();
};