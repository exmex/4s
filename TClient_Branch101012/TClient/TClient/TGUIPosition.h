#pragma once

class CTGUIPosition
{
public:
	static CTGUIPosition	m_NetInitPos[TNETFRAME_COUNT];
	static CTGUIPosition	m_ClientInitPos[TFRAME_COUNT];

protected:
	/// 위치 타입
	TGUI_POS_TYPE	m_eType;
	
	/// X축 기준 위치
	TGUI_XBASE		m_eBaseX;
	/// Y축 기준 위치
	TGUI_YBASE		m_eBaseY;

	/**
		m_eType 에 따라 다른 의미를 지닌다.

		TGUI_POS_ABSOLUTE		: (int)		좌표값
		TGUI_POS_RELATIVE		: (float)	화면 비율 값 (0 ~ 1)
		TGUI_POS_EXTEND			: 사용하지	않음.
		TGUI_POS_CENTER			:
		TGUI_POS_LEFT_CENTER	:
		TGUI_POS_RIGHT_CENTER	: (float)	기준위치로 부터의 오프셋. 비율값. (-1 ~ 1)
	*/
	LPVOID			m_pParamX, m_pParamY;

protected:
	void Apply_ABS(TComponent* pComp);
	void Apply_REL(TComponent* pComp);
	void Apply_EXT(TComponent* pComp);
	void Apply_CET(TComponent* pComp);
	void Apply_LCT(TComponent* pComp);
	void Apply_RCT(TComponent* pComp);

	int ToAbsLeft(TComponent* pComp, int dwPos, TGUI_XBASE eBaseX);
	int ToAbsTop(TComponent* pComp, int dwPos, TGUI_YBASE eBaseY);

	CPoint GetScreen();

public:
	void Init( TGUI_POS_TYPE type,
			   TGUI_XBASE baseX, TGUI_YBASE baseY,
			   LPVOID pParamX, LPVOID pParamY );

	void Apply(TComponent* pComp);

public:
	static void InitNetFrame();

public:
	CTGUIPosition();
	~CTGUIPosition();
};