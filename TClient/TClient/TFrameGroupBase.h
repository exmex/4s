#pragma once

class CTFrameGroupBase : public CTClientUIBase
{
public:
	struct FrameInfo
	{
		UINT			m_nFrameID;
		ITInnerFrame*	m_pFrameCtrl;
		TButton*		m_pTabCtrl;
		BOOL			m_bUseBlank;
		CString			m_strBlankMsg;
	};

	typedef std::vector<FrameInfo>	FrameInfoArray;

public:
	FrameInfoArray	m_FrameInfoArray;
	CPoint			m_ptInnerFrame;
	INT				m_nSelectedFrame;
	TComponent*		m_pBlank;

public:
	/**
		새로운 프레임을 이 창의 자식으로 추가한다.

		@param pFrame		추가하고자 하는 프레임
		@param dwTabID		이 프레임의 탭버튼으로 사용할 버튼 아이디
		@param strBlankMsg	만약 이 탭을 비어있는 상태로 만들고자 할 
							경우 프레임대신 띄우게될 메시지
	*/
	virtual UINT AddFrame(ITInnerFrame* pFrame, DWORD dwTabID, const CString& strBlankMsg="");
	/// 전체 소속 프레임을 모두 제거한다.
	virtual void ClearFrame();
	
	/// 전체 소속 프레임의 갯수를 얻는다.
	virtual UINT GetFrameCount() const;
	/// 주어진 인덱스의 프레임을 얻는다.
	virtual ITInnerFrame* GetInnerFrame(UINT nFrame) const;
	
	/// 주어진 프레임을 비어있게 하거나 다시 원상복귀시킨다.
	virtual void SetBlank(UINT nFrame, BOOL bBlank, const CString& strBlankMsg="");

	/// 주어진 프레임을 선택한다.
	virtual void SetSelect(UINT nFrame);
	/// 선택된 프레임ID를 얻어온다.
	virtual INT GetSelectedFrame();
	/// 주어진 프레임이 현재 선택되었는가를 얻는다.
	virtual BOOL IsSelect(UINT nFrame) const;

	/**
		이 프레임의 갱신이 필요함을 알린다. 주로 서버 클라이언트 구조에서
		이 함수를 통해 서버로 알리고 ResetInfo() 를 통해 그 내용을 갱신
		하는 구조를 위해 사용된다.
	*/
	virtual void RequestInfo();
	/// RequestInfo() 를 참조.
	virtual void ResetInfo();

public:
	virtual void ShowComponent(BOOL bVisible = TRUE);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);

public:
	CTFrameGroupBase(TComponent* pParent, LP_FRAMEDESC pDesc, DWORD dwInnerPosID);
	CTFrameGroupBase(TComponent* pParent, LP_FRAMEDESC pDesc, const CPoint& ptInnerPos);
	virtual ~CTFrameGroupBase();
};