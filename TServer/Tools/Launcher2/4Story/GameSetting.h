#pragma once
#include "afxwin.h"

#include "xSkinButton.h"
#include "bitmapslider.h"
#include "PlaySetting.h"
#include "Bitmappickercombo.h"

// CGameSetting 대화 상자입니다.
// 게임 시스템 설정

// 그래픽 설정
#define REG_WINDOW				_T("WindowedMode")	// 창모드
#define REG_SHADER				_T("UseShader")		// Shader 사용유무
#define REG_CHARACTER			_T("ObjDETAIL")		// 캐릭터 품질
#define REG_MAPDETAIL			_T("MapDETAIL")		// 지형 품질
#define REG_TEXDETAIL			_T("TextureDETAIL")	// 배경시야

#define TMIN_RANGEOPTION				((FLOAT) 0.7f)
#define TMAX_RANGEOPTION				((FLOAT) 1.0f)

class CGameSetting : public CDialog
{
	DECLARE_DYNAMIC(CGameSetting)

public:
	CGameSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGameSetting();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnBnClickedCheckShader();
	afx_msg void OnBnClickedCheckWinmode();
	afx_msg void OnBnClickedRadioChar1(); // 캐릭터 품질
	afx_msg void OnBnClickedRadioChar2();
	afx_msg void OnBnClickedRadioChar3();
	afx_msg void OnBnClickedRadioGeo1(); // 지형 품질
	afx_msg void OnBnClickedRadioGeo2();
	afx_msg void OnBnClickedRadioGeo3();
	afx_msg void OnBnClickedRadioView1(); // 텍스쳐 품질
	afx_msg void OnBnClickedRadioView2();
	afx_msg void OnBnClickedRadioView3();
	afx_msg void OnBnClickedBtnClose(); // 닫기 버튼
	afx_msg void OnBnClickedBtnGameTab(); // 게임 [플레이] 설정 대화상자 보이기. 이대화상자는 게임 [시스템] 설정 대화상자임

	afx_msg LRESULT OnBitmapSliderMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBitmapSliderMoving(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedChkBgshadow();
	afx_msg void OnBnClickedChkDlightmap();
	afx_msg void OnBnClickedChkBgsfx();
	afx_msg void OnBnClickedChkFlightmap();
	afx_msg void OnBnClickedChkFarimg();
	afx_msg void OnBnClickedChkVolume();
	afx_msg void OnBnClickedChkBgm();
	afx_msg void OnBnClickedChkSfxvolume();
	afx_msg void OnCbnSelchangeCbResolution();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);



	// 그래픽 설정
	void SetGraphicMode(DWORD _window, DWORD _BGShadow, DWORD _DLightMap, DWORD _BGSFX, DWORD _FLightMap, DWORD _FarImg);
	void GetGraphicMode(DWORD *_wiindow, DWORD *_BGShadow, DWORD *_DLightMap, DWORD *_BGSFX, DWORD *_FLightMap, DWORD *_FarImg);
	
	// 볼륨 사용여부 설정
	void SetVolumeMode(DWORD _volume, DWORD _BGM, DWORD _SFXVolume);
	void GetVolumeMode(DWORD *_volume, DWORD *_BGM, DWORD *_SFXVolume);
	
	// 볼륨 값 설정
	void SetVolumeValue(DWORD _objRange, DWORD _volume, DWORD _BGMVolume, DWORD _SFXVolume);
	void GetVolumeValue(DWORD *_objRange, DWORD *_volume, DWORD *_BGMVolume, DWORD *_SFXVolume);

	// 캐릭터 품질, 지형품질, 텍스쳐 품질 라디오 버튼 초기 설정
	void SetRadioButtons(DWORD _char, DWORD _map, DWORD _texture);

	BOOL ReadRegistry();
	BOOL WriteRegistry();
	void LoadBGSkin();
	void LoadControlSkin();
	void SetDefaultOption();
	void InitResolution();
	void SetResolutionCtrl(CString strMode);
	void SetResolution(CString strMode);

	// 스킨 입힌 다이얼로그에서 특정색을 투명색으로 바꾸기.
	// Load한 비트맵(CBitmap)을 넘겨 준다.
	void TransparentDialog(CBitmap* _pCBitmap, COLORREF  _color);
	void TransparentDialog2(COLORREF _color); // winNT이상 user32.dll 사용
	
	static CGameSetting* GetInstance()	{static CGameSetting inst; return &inst;}

public:
	
	//배경스킨
	HBITMAP		hBmp;
	CBitmap		m_bmpBG;
	BYTE		sFlag;
	int			m_iBGWidth;
	int			m_iBGHeight;

	CString		m_strCountry;
	CString		m_strResolution;

	//
	DWORD		m_dwObjRange;	// 시야거리
	DWORD		m_dwMainVolume;	// 전체 볼륨
	DWORD		m_dwBGMVolume;	// 배경음악
	DWORD		m_dwSFXVolume;	// 효과음
	DWORD		m_dwObjDetailValue; // 캐릭터 품질
	DWORD		m_dwMapDetailValue; // 지형 품질
	DWORD		m_dwTextureDetailValue; // 텍스쳐 품질 
	DWORD		m_dwScreenWidth;
	DWORD		m_dwScreenHeight;

	BOOL m_bShader;
	BOOL m_bWindow;
	BOOL m_bChar;
	BOOL m_bPaper;
	BOOL m_bBack;

	BOOL m_bBGShadow;
	BOOL m_bDLightMap;
	BOOL m_bBGSFX;
	BOOL m_bFLightMap;
	BOOL m_bFarImg;
	BOOL m_bVolume;
	BOOL m_bBGM;
	BOOL m_bSFXVolume;


	CxSkinButton m_chkShader;    // Shader 사용여부
	CxSkinButton m_chkWinMode;   // 창모드 
	CxSkinButton m_chkBGShadow;  // 배경 그림자.   레지스트리 연동 항목 : MapSHADOW
	CxSkinButton m_chkDLightMap; // 던전 라이트맵. 
	CxSkinButton m_chkBGSFX;	 // 배경 특수효과. 레지스트리 연동 항목 : MapSFX
	CxSkinButton m_chkFLightMap; // 필드 라이트맵. 
	CxSkinButton m_chkFarImg;	 // 원경 이미지.   
	
	CxSkinButton m_radioCHR3;	 // 캐릭터 품질 상.	레지스트리 연동 항목 :  ObjDETAIL
	CxSkinButton m_radioCHR2;	 
	CxSkinButton m_radioCHR1;	
	
	CxSkinButton m_radioMapDetail3; // 지형 품질 상
	CxSkinButton m_radioMapDetail2; 
	CxSkinButton m_radioMapDetail1; 
	
	CxSkinButton m_radioTextureDetail3; // 텍스쳐 품질 상
	CxSkinButton m_radioTextureDetail2;
	CxSkinButton m_radioTextureDetail1;
	
		
	CBitmapSlider m_sliderViewRange; // 시야 거리. 레지스트리 연동 항목 : OBJRange(문자열)
	CBitmapSlider m_sliderVolume;	 // 전체 볼륨. 레지스트리 연동 항목 : MainVolume
	CBitmapSlider m_sliderBGM;		 // 배경음악 . 레지스트리 연동 항목 : BGMVolume
	CBitmapSlider m_sliderSFXVolume; // 효과음.    레지스트리 연동 항목 : SFXVolume

	CxSkinButton m_chkVolume;		// 전체 볼륨 사용여부
	CxSkinButton m_chkBGM;			// 배경음 사용여부
	CxSkinButton m_chkSFXVolume;	// 효과음 사용여부

	CxSkinButton m_btnClose; 
	CxSkinButton m_btnGame; // 게임설정 버튼(탭버튼)	
	
	CBitmapPickerCombo m_cbResolution;
	
};
