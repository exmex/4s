#pragma once

class CTGuildNormalFrame : public ITInnerFrame
{
public:
	enum InfoEnum
	{
		INFO_GNAME,							///< 길드명
		INFO_GDATE,							///< 길드 개설 일
		INFO_GMASTER,						///< 길드장
		INFO_GMASTER_PEER,					///< 길드장 작위
		INFO_GSUBMASTER1,					///< 부 길드장
		INFO_GSUBMASTER2,					///< 부 길드장
		INFO_GMBCNT,						///< 길드원의 수
		INFO_GLEV,							///< 길드 레벨
		INFO_GEXP,							///< 길드 경험치
		INFO_MYDUTY,						///< 나의 직위
		INFO_MYPEER,						///< 나의 작위
		INFO_GMONEY_RUNE,					///< 길드 자금
		INFO_GMONEY_LUNA,					///< 길드 자금
		INFO_GMONEY_CRON,					///< 길드 자금
		INFO_NOTIFY,						///< 길드 공지사항
		INFO_GUILD_RANK,
		INFO_GUILD_POINT,
		INFO_GUILD_USEABLE_POINT,
		INFO_GUILD_MONTH_POINT,
		INFO_GUILD_RANK_TOTAL,
		INFO_GUILD_RANK_MONTH,
		INFO_COUNT,
	};

protected:
	TComponent*		m_vInfoCtrl[INFO_COUNT];
	TButton* m_pGuildMarkButton;
	TButton* m_pGuildExitButton;
	
	TImageList*		m_pMarkImgset;
	TImageList*		m_pBackImgset;
	TComponent*		m_pBackBase;

public:
	virtual void RequestInfo();
	virtual void ResetInfo();

	void HideComponentWhenTactics();

public:
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void ShowComponent( BOOL bVisible );

public:
	CTGuildNormalFrame(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTGuildNormalFrame();
};