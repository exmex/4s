#pragma once

class CTGuildMarkMakeDlg : public CTClientUIBase
{
public:
	enum TYPE
	{
		TYPE_MARK,			///< ¹®¾ç
		TYPE_BACK,			///< ¹ÙÅÁ
		TYPE_MARKCOLOR,		///< ¹®¾ç»ö
		TYPE_BACKCOLOR1,	///< ¹ÙÅÁ»ö1
		TYPE_BACKCOLOR2,	///< ¹ÙÅÁ»ö2
		TYPE_COUNT
	};

protected:
	TComponent*		m_pLeft[TYPE_COUNT];
	TComponent*		m_pRight[TYPE_COUNT];
	TComponent*		m_pMiddle[TYPE_COUNT];

	TImageList*		m_pMarkImgset;
	TImageList*		m_pBackImgset;
	TComponent*		m_pBackBase;

	CTClientGuildMark* m_pGuildMark;

public:
	void Init();

	void SetMark(BYTE bID);
	void SetBack(BYTE bID);

	void SetMarkColor(BYTE bColor);
	void SetBackColor1(BYTE bColor);
	void SetBackColor2(BYTE bColor);

	void MovMark(BOOL bLeft);
	void MovBack(BOOL bLeft);

	void MovMarkColor(BOOL bLeft);
	void MovBackColor1(BOOL bLeft);
	void MovBackColor2(BOOL bLeft);

	BYTE GetMark()						{ return m_pGuildMark->GetFront(); }
	BYTE GetBack()						{ return m_pGuildMark->GetBack(); }

	BYTE GetMarkColor()					{ return m_pGuildMark->GetFrontColorID(); }
	BYTE GetBackColor1()				{ return m_pGuildMark->GetBackColorID1(); }
	BYTE GetBackColor2()				{ return m_pGuildMark->GetBackColorID2(); }

	CTClientGuildMark* GetGuildMark()	{ return m_pGuildMark; }

public:
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void ShowComponent( BOOL bVisible = TRUE);

public:
	CTGuildMarkMakeDlg(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTGuildMarkMakeDlg();
};